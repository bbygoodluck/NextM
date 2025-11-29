#include "../ginc.h"
#include "../system/Sysinc.h"
#include "NextMProcessDetailView.h"

wxBEGIN_EVENT_TABLE(CNextMProcessDetailView, wxFrame)
	EVT_PAINT(CNextMProcessDetailView::OnPaint)
	EVT_ERASE_BACKGROUND(CNextMProcessDetailView::OnErase)
	EVT_SIZE(CNextMProcessDetailView::OnSize)
wxEND_EVENT_TABLE()

CNextMProcessDetailView::CNextMProcessDetailView(wxWindow* parent, const int nID, const wxPoint& pt, const wxSize& sz, long lStyle)
	: wxFrame(parent, nID, wxT("Detail"), pt, sz, wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP | wxBORDER_DOUBLE)
	, m_colBackPen(0, 0, 0)
	, m_colBackBrush(20, 20, 20)
{
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	SetTransparent(230);

	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CNextMProcessDetailView::OnTimer));
	m_timer.SetOwner(this, wxTIMER_ID++);

#ifdef __WXMSW__
	m_font.Create(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Segoe UI"));
#else
	m_PathViewFont = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
#endif
}

CNextMProcessDetailView::~CNextMProcessDetailView()
{
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(CNextMProcessDetailView::OnTimer));
	Stop();
}

void CNextMProcessDetailView::OnErase(wxEraseEvent& event)
{

}

void CNextMProcessDetailView::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

    theDCUtil->Refresh(this, m_viewRect);
}

void CNextMProcessDetailView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

    m_viewRect = GetClientRect();

    int w = m_viewRect.GetWidth();
	int h = m_viewRect.GetHeight();

	wxMemoryDC* pMemDC = m_memDC.SelectObjectOnly(&dc, m_viewRect);

	pMemDC->SetFont(m_font);
	DrawBackGround(pMemDC, m_colBackPen, m_colBackBrush);
	Render(pMemDC);

	dc.Blit(0, 0, w, h, pMemDC, 0 ,0);
}

void CNextMProcessDetailView::DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush)
{
	pDC->SetPen(colPen);
	pDC->SetBrush(colBrush);
	pDC->DrawRectangle(m_viewRect);
}

void CNextMProcessDetailView::Render(wxDC* pDC)
{
	if(m_viewRect.GetWidth() == 0 || !_pPsInfo)
		return;

	pDC->SetTextForeground(wxColour(255, 255, 233));

	unsigned int widths = m_viewRect.GetWidth() / 10;
	unsigned int heights = 16;

	int xPos = m_viewRect.GetLeft();
	int yPos = m_viewRect.GetTop() + 3;

	wxRect rcCPUText(xPos, yPos, widths, heights);
	pDC->DrawLabel(wxT("CPU"), rcCPUText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcCPU(xPos, rcCPUText.GetBottom(), widths, heights);
	pDC->DrawLabel(wxString::Format(wxT("%.2f%"), _pPsInfo->fCPUUsage), rcCPU, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcPidText(xPos + (widths * 1), yPos, widths, heights);
	pDC->DrawLabel(wxT("Pid"), rcPidText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcPid(xPos + (widths * 1), rcPidText.GetBottom(), widths, heights);
	pDC->DrawLabel(_strProcessId, rcPid, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcNameText(xPos + (widths * 2), yPos, widths, heights);
	pDC->DrawLabel(wxT("Name"), rcNameText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcName(xPos + (widths * 2), rcNameText.GetBottom(), widths, heights);
	pDC->DrawLabel(_pPsInfo->_strProcessName, rcName, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcStatusText(xPos + (widths * 3), yPos, widths, heights);
	pDC->DrawLabel(wxT("Status:"), rcStatusText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	pDC->SetTextForeground(wxColour(0, 255, 0));
	wxRect rcStatus(xPos + (widths * 3), rcStatusText.GetBottom(), widths, heights);
	pDC->DrawLabel(_pPsInfo->_strStatus, rcStatus, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	pDC->SetTextForeground(wxColour(255, 255, 255));
	wxRect rcElapsedText(xPos + (widths * 4), yPos, widths, heights);
	pDC->DrawLabel(wxT("Elapsed:"), rcElapsedText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcElapsed(xPos + (widths * 4), rcElapsedText.GetBottom(), widths, heights);
	pDC->DrawLabel(_pPsInfo->_strElapsedTime, rcElapsed, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	unsigned long long ullIORead = 0;
	_pPsInfo->_readTransferCount.ToULongLong(&ullIORead);
	wxString strIORead = theUtility->floating_humanizer(ullIORead, false, false, 0, false, false);

	wxRect rcIOReadText(xPos + (widths * 5), yPos, widths, heights);
	pDC->DrawLabel(wxT("IO/R"), rcIOReadText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcIORead(xPos + (widths * 5), rcIOReadText.GetBottom(), widths, heights);
	pDC->DrawLabel(strIORead, rcIORead, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	unsigned long long ullIOWrite = 0;
	_pPsInfo->_writeTransferCount.ToULongLong(&ullIOWrite);
	wxString strIOWrite = theUtility->floating_humanizer(ullIOWrite, false, false, 0, false, false);

	wxRect rcIOWriteText(xPos + (widths * 6), yPos, widths, heights);
	pDC->DrawLabel(wxT("IO/W"), rcIOWriteText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcIOWrite(xPos + (widths * 6), rcIOWriteText.GetBottom(), widths, heights);
	pDC->DrawLabel(strIOWrite, rcIOWrite, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcParentText(xPos + (widths * 7), yPos, widths, heights);
	pDC->DrawLabel(wxT("Parent : "), rcParentText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcParent(xPos + (widths * 7), rcParentText.GetBottom(), widths, heights);
	pDC->DrawLabel(_pPsInfo->_strParentProcessName, rcParent, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcUserText(xPos + (widths * 8), yPos, widths, heights);
	pDC->DrawLabel(wxT("User : "), rcUserText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcUser(xPos + (widths * 8), rcUserText.GetBottom(), widths, heights);
	pDC->DrawLabel(_pPsInfo->_strUserName, rcUser, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcThreadText(xPos + (widths * 9), yPos, widths, heights);
	pDC->DrawLabel(wxT("Threads : "), rcThreadText, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	wxRect rcThread(xPos + (widths * 9), rcThreadText.GetBottom(), widths, heights);
	pDC->DrawLabel(wxString::Format(wxT("%d"), _pPsInfo->_threadsCount), rcThread, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	pDC->SetPen(wxPen(wxColour(128, 128, 128), 1, wxPENSTYLE_DOT));
	pDC->DrawLine(wxPoint(0, rcThread.GetBottom() + 3), wxPoint(m_viewRect.GetRight(), rcThread.GetBottom() + 3));

	//CPU Graph Area
	m_viewCPURect.SetLeft(m_viewRect.GetLeft());
	m_viewCPURect.SetTop(rcName.GetBottom() + 4);
	m_viewCPURect.SetRight(rcName.GetRight());
	m_viewCPURect.SetBottom(m_viewRect.GetBottom());

	//Memory Area
	int memYPos = m_viewRect.GetHeight() / 2;
	m_viewMemTextRect.SetLeft(xPos + (widths * 4));
	m_viewMemTextRect.SetTop(yPos + memYPos);
	m_viewMemTextRect.SetRight(xPos + (widths * 5));
	m_viewMemTextRect.SetBottom((yPos + memYPos) + 25);

	m_viewMemRect.SetLeft(m_viewMemTextRect.GetRight() + 2);
	m_viewMemRect.SetTop(m_viewMemTextRect.GetTop());
	m_viewMemRect.SetRight(m_viewRect.GetRight() - 250);
	m_viewMemRect.SetBottom(m_viewMemTextRect.GetBottom());

	//Command area
	m_viewCommandRect.SetLeft(m_viewMemTextRect.GetLeft());
	m_viewCommandRect.SetTop(m_viewMemRect.GetBottom());
	m_viewCommandRect.SetRight(m_viewRect.GetRight());
	m_viewCommandRect.SetBottom(m_viewRect.GetBottom());

	DrawData(pDC);
}

void CNextMProcessDetailView::DrawData(wxDC* pDC)
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
				x2 = m_viewCPURect.GetRight();
				y2 = y1;

				col = pNode->_col;
			}

			pDC->SetPen(col);
			pDC->DrawLine(x1, y1, x2, y2);

			nodeList.update(pNode);
		}

		nodeList.InitNodeIndex();
	}

	//memory
	unsigned long totalMem = theSystem->MEM()->GetTotalMemory();
	unsigned long usedMem = (unsigned long)_pPsInfo->_PrivateSize;

	float fTotalGB = (float)totalMem / 1024.0f;
	float fUsedGB = (float)usedMem / GIGABYTE;

	float usedPercent = (100.0f * fUsedGB) / fTotalGB;

	pDC->DrawLabel(wxString::Format(wxT("Memory : %.1f%"), usedPercent), m_viewMemTextRect, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);

	wxRect m_viewMemRectShadow;

	int shadowGap = 2;
	m_viewMemRectShadow.SetLeft(m_viewMemRect.GetLeft() + shadowGap);
	m_viewMemRectShadow.SetTop(m_viewMemRect.GetTop() + shadowGap);
	m_viewMemRectShadow.SetRight(m_viewMemRect.GetRight() + shadowGap);
	m_viewMemRectShadow.SetBottom(m_viewMemRect.GetBottom() + shadowGap);

	pDC->SetPen(wxColour(128, 128, 128));
	pDC->SetBrush(wxColour(128, 128, 128));
	pDC->DrawRectangle(m_viewMemRectShadow);

	int iRed = (int)(255 * fUsedGB) / 100.0f;
	int iGreen = iRed == 0 ? 255 : 255 - iRed;

	wxColour col(iRed, iGreen, 0);
	pDC->SetPen(wxPen(col, 1));
	pDC->SetBrush(wxBrush(col));

	pDC->DrawRectangle(m_viewMemRect);

	wxRect rcMem;
	rcMem.SetLeft(m_viewMemRect.GetRight() + 4);
	rcMem.SetTop(m_viewMemRect.GetTop());
	rcMem.SetRight(m_viewRect.GetRight());
	rcMem.SetBottom(m_viewMemRect.GetBottom());

	wxString strPrivateSize = theUtility->floating_humanizer(_pPsInfo->_PrivateSize, false, false, 0, false, false);
	pDC->DrawLabel(strPrivateSize, rcMem, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	wxSize szSrc = pDC->GetTextExtent(_pPsInfo->_strCommandLine);
	wxString strDisp = theDCUtil->EllipseStr(pDC, _pPsInfo->_strCommandLine, m_viewCommandRect.GetWidth() - 10);

	pDC->SetTextForeground(wxColour(255, 255, 255));
	pDC->DrawLabel(strDisp, m_viewCommandRect, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
}

void CNextMProcessDetailView::SetWindowAlpha(BYTE alpha)
{
	HWND hWnd = this->GetHWND();
	if(!IsWindow(hWnd))
		return;

	LONG exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	if(!(exStyle & WS_EX_LAYERED))
		SetWindowLong(hWnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);

	SetLayeredWindowAttributes(hWnd, 0, alpha, ULW_ALPHA | ULW_OPAQUE);
}

void CNextMProcessDetailView::SetProcessId(const wxString& strProcessId)
{
	_strProcessId = strProcessId;

	wxString strPid(strProcessId);
	strPid.ToCULong(&_ulProcessId);

	_pPsInfo = theSystem->PROCESS()->GetInfo(_ulProcessId);
	UpdateData();
}

void CNextMProcessDetailView::OnTimer(wxTimerEvent& event)
{
	if(!_pPsInfo)
		return;

	UpdateData();
}

void CNextMProcessDetailView::UpdateData()
{
	theSystem->PROCESS()->UpdateIOReadWrite(_pPsInfo);
	theSystem->PROCESS()->UpdateElapsedTime(_pPsInfo);

	int xPos = m_viewCPURect.GetRight();
	unsigned int processCPU = (unsigned int)_pPsInfo->fCPUUsage;

	nodeList.push_back(processCPU, xPos, m_viewCPURect.GetBottom(), m_viewCPURect.GetHeight(), false, false);

	GRAPH_DATA_NODE<unsigned int>* firstNode = nodeList.GetFirst();
	if((int)firstNode->_iXPos < m_viewCPURect.GetLeft())
		nodeList.pop_front();

	theDCUtil->Refresh(this, m_viewRect);
}

void CNextMProcessDetailView::Clear()
{
	nodeList.clear();
}
