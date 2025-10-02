#include "../ginc.h"
#include "../system/Sysinc.h"
#include "NextMGraphView.h"
#include "../dialog/DlgSystemResource.h"

wxBEGIN_EVENT_TABLE(CNextMGraphView, wxWindow)
	EVT_PAINT(CNextMGraphView::OnPaint)
	EVT_ERASE_BACKGROUND(CNextMGraphView::OnErase)
	EVT_SIZE(CNextMGraphView::OnSize)
	EVT_MY_CUSTOM_COMMAND(wxEVT_RESOURCE_MONITORING, wxID_ANY, CNextMGraphView::UpdateListener)
wxEND_EVENT_TABLE()

CNextMGraphView::CNextMGraphView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle)
	: wxWindow(parent, nID, wxDefaultPosition, sz, lStyle)
	, m_colBackPen(0, 0, 0)
	, m_colBackBrush(225, 225, 225)
{
	this->SetBackgroundStyle(wxBG_STYLE_ERASE);

	m_colRuler = wxColour(rulerRGB, rulerRGB, rulerRGB);
	m_colSecond = wxColour(0, 0, secondRGB);

#ifdef __WXMSW__
	m_viewFont.Create(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Segoe UI"));
#else
	m_viewFont = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
#endif
}

CNextMGraphView::~CNextMGraphView()
{
	nodeList.clear();
}

void CNextMGraphView::OnErase(wxEraseEvent& event)
{

}

void CNextMGraphView::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

    theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMGraphView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

    m_viewRect = GetClientRect();

	wxMemoryDC* pMemDC = m_memDC.SelectObjectOnly(&dc, m_viewRect);

	int w = m_viewRect.GetWidth();
	int h = m_viewRect.GetHeight();

#ifdef __WXMSW__
    // Windows HDC 획득 및 GDI 좌표 설정
	HDC hdc = pMemDC->GetHDC();

	::SetMapMode(hdc, MM_ANISOTROPIC);

    ::SetWindowExtEx(hdc, w, h, NULL);
    ::SetViewportExtEx(hdc, -w, -h, NULL); // X, Y 반전
    ::SetViewportOrgEx(hdc, w, h, NULL);   // 오른쪽 하단을 원점으로

	m_viewGraphRect.SetLeft(m_viewRect.GetLeft() + (CONST_RATIO_GAP * 2));
	m_viewGraphRect.SetTop(m_viewRect.GetTop());
	m_viewGraphRect.SetRight(m_viewRect.GetRight());
	m_viewGraphRect.SetBottom(m_viewRect.GetBottom());

	pMemDC->SetFont(m_viewFont);

	DrawBackGround(pMemDC, m_colBackPen, m_colBackBrush);
	Render(pMemDC);

	//실제로 화면상에 오른쪽 하단을 0,0으로 하고 그리기 처리가 왼료된 후
	//memoryDC상의 좌표를 다시 원래대로 처리해주어야 화면에 제대로 표시됨.
	::SetViewportExtEx(hdc, w, h, NULL); // X, Y 반전
    ::SetViewportOrgEx(hdc, 0, 0, NULL); // 왼쪽 상단을 다시 원점으로..
#endif

	dc.Blit(0, 0, w, h, pMemDC, 0 ,0);
}

void CNextMGraphView::Render(wxDC* pDC)
{
	DrawRuler(pDC);
	DrawGraph(pDC);

	wxString strCoreIndexUsage{wxString::Format(wxT("#%02d - %u%"), m_coreIndex + 1, m_coreUsage)};
	pDC->DrawText(strCoreIndexUsage, m_viewRect.GetRight(), m_viewRect.GetBottom());
}

void CNextMGraphView::DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush)
{
    pDC->SetPen(colPen);
	pDC->SetBrush(colBrush);
	pDC->DrawRectangle(m_viewRect);
}

void CNextMGraphView::DrawRuler(wxDC* pDC)
{
	//Y축 라인(25%, 50%, 75%, 100%)
	wxPen penRow(m_colRuler, 1, wxPENSTYLE_DOT);
	pDC->SetPen(penRow);

	unsigned int y_axis = 1;
	unsigned int x_axis = 0;

	int y_axisRatio = m_viewGraphRect.GetHeight() / CONST_YAXIS_RATIO;
	int r,g,b = 0;

	for(y_axis; y_axis < CONST_YAXIS_RATIO; y_axis++ )
	{
		pDC->DrawLine(CONST_YAXIS_START + m_viewGraphRect.GetLeft(), y_axis * y_axisRatio, m_viewGraphRect.GetRight(), y_axis * y_axisRatio);

		r = ((25 * y_axis) * 255) / 100;
		g = r == 0 ? 255 : 255 - r;

		pDC->SetTextForeground(wxColour(0, 0, 0));
		pDC->DrawText(wxString::Format(wxT("%d%"), 25 * y_axis), (m_viewGraphRect.GetLeft() - (CONST_XAXIS_GAP_PIXEL * 2)) - 1, ((y_axis * y_axisRatio) + CONST_XAXIS_GAP_PIXEL + 3) - 1);

		pDC->SetTextForeground(wxColour(r, g, b));
		pDC->DrawText(wxString::Format(wxT("%d%"), 25 * y_axis), m_viewGraphRect.GetLeft() - (CONST_XAXIS_GAP_PIXEL * 2), (y_axis * y_axisRatio) + CONST_XAXIS_GAP_PIXEL + 3);
	}

	//x축
	unsigned int x_axisCount = m_viewGraphRect.GetWidth() / CONST_SECOND_GAP;

	wxPen penSecond(m_colSecond, 1, wxPENSTYLE_DOT);
	for(x_axis = 0; x_axis < x_axisCount; x_axis++)
	{
		pDC->DrawLine(m_viewGraphRect.GetLeft() + (x_axis * CONST_SECOND_GAP), 0, m_viewGraphRect.GetLeft() + (x_axis * CONST_SECOND_GAP), CONST_SECOND_HEIGHT);
		if((x_axis % 10) == 0)
		{
			pDC->SetPen(wxPen(wxColour(0,120,215), 1));
			pDC->DrawLine(m_viewGraphRect.GetLeft() + (x_axis * CONST_SECOND_GAP), 0, m_viewGraphRect.GetLeft() + (x_axis * CONST_SECOND_GAP), CONST_SECOND_HEIGHT * 2);
			pDC->SetPen(penRow);
		}
	}
}

void CNextMGraphView::DrawGraph(wxDC* pDC)
{
	if(nodeList.GetSize())
	{
		wxColour col;
		int width = 5;
		int gap = 20;

		int x1, y1, x2, y2;

		int pointCnt = 0;
		GRAPH_DATA_NODE<unsigned int>* prevNode = nullptr;
		while(1)
		{
			GRAPH_DATA_NODE<unsigned int>* pNode = nodeList.GetNext();
			if(pNode == nullptr)
				break;

//			if(pNode->value >= 75.0f)
//			{
//				int xTextPos = pNode->_iXPos + 7;
//				int yTextPos = pNode->_iYPos + (pNode->value >= 85.0 ? -3 : gap);
//
//				int r = (pNode->value * 255) / 100;
//				int g = r == 0 ? 255 : 255 - r;
//
//				pDC->SetTextForeground(wxColour(r, g, 0));
//				pDC->DrawText(wxString::Format(wxT("%d%"), (int)pNode->value), xTextPos, yTextPos);
//			}

			if(pNode->_bLineDraw)
			{
				pDC->SetPen(wxPen(pNode->_col, 1, wxPENSTYLE_DOT));
				pDC->DrawLine(pNode->_iXPos, 0, pNode->_iXPos, pNode->_iYPos);
			}

			if(pNode->value >= 75.0f)
			{
				pDC->SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));
				pDC->SetBrush(pNode->_col);
				pDC->DrawCircle(pNode->_iXPos - 1, pNode->_iYPos, width - 3);
			}

			if(pNode->_next)
			{
				x1 = (int)pNode->_iXPos;
				y1 = (int)pNode->_iYPos;
				x2 = (int)pNode->_next->_iXPos;
				y2 = (int)pNode->_next->_iYPos;

				col = pNode->_next->_col;
			}
			else
			{
				x1 = (int)pNode->_iXPos;
				y1 = (int)pNode->_iYPos;
				x2 = m_viewGraphRect.GetLeft();
				y2 = y1;//m_viewGraphRect.GetHeight() / 2;

				col = pNode->_col;
			}

			pDC->SetPen(col);
			pDC->DrawLine(x1, y1, x2, y2);

			nodeList.update(pNode);
		}

		nodeList.InitNodeIndex();
	}
}

void CNextMGraphView::UpdateListener(wxCommandEvent& event)
{
	if (m_viewGraphRect.GetWidth() == 0)
		return;

	int xPos = m_viewGraphRect.GetLeft();
	m_coreUsage = theSystem->CPU()->GetCPUCoreUsage(m_coreIndex);

	bool bLineDraw = false;
	if(m_coreUsage >= 75)
		bLineDraw = true;

	nodeList.push_back(m_coreUsage, xPos, m_viewGraphRect.GetHeight(), bLineDraw);

	GRAPH_DATA_NODE<unsigned int>* firstNode = nodeList.GetFirst();
	if(firstNode->_iXPos > m_viewGraphRect.GetRight())
		nodeList.pop_front();

	theUtility->RefreshWindow(this, m_viewRect);
}
