#include "../ginc.h"
#include "../system/Sysinc.h"
#include "NextMMemView.h"
#include "../dialog/DlgSystemResource.h"

wxBEGIN_EVENT_TABLE(CNextMMemView, wxWindow)
	EVT_PAINT(CNextMMemView::OnPaint)
	EVT_ERASE_BACKGROUND(CNextMMemView::OnErase)
	EVT_SIZE(CNextMMemView::OnSize)
	EVT_MY_CUSTOM_COMMAND(wxEVT_RESOURCE_MONITORING, wxID_ANY, CNextMMemView::UpdateListener)
wxEND_EVENT_TABLE()

CNextMMemView::CNextMMemView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle)
	: wxWindow(parent, nID, wxDefaultPosition, sz, lStyle)
	, m_colBackPen(0, 0, 0)
	, m_colBackBrush(0, 0, 0)
{
	this->SetBackgroundStyle(wxBG_STYLE_ERASE);
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CNextMMemView::OnTimer));

	m_timer.SetOwner(this, wxTIMER_ID++);
#if BALL
	m_timer.Start(16); // ~60fps
#else
	totalMem = theSystem->MEM()->GetTotalMemory();
	usedMem = theSystem->MEM()->GetUsedMemory();

	avaliable = theSystem->MEM()->GetAvaliableMemory();

	_memVal = (100 * usedMem) / totalMem;

	m_timer.Start(30); // ~60fps
#endif

#ifdef __WXMSW__
	m_viewFont.Create(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Segoe UI"));
#else
	m_viewFont = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
#endif
}

CNextMMemView::~CNextMMemView()
{
	Stop();
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(CNextMMemView::OnTimer));
}

void CNextMMemView::Start(int _milliseconds)
{
	m_timer.Start(_milliseconds);
}

void CNextMMemView::Stop()
{
	m_timer.Stop();
}

void CNextMMemView::OnErase(wxEraseEvent& event)
{

}

void CNextMMemView::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

#if BALL
	m_viewRect = GetClientRect();
	RecomputeParabola();
#endif // BALL
    theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMMemView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

    m_viewRect = GetClientRect();
    wxMemoryDC* pMemDC = m_memDC.SelectObjectOnly(&dc, m_viewRect);
	pMemDC->SetFont(m_viewFont);

#if BALL
    m_viewMemGraph.SetLeft(m_viewRect.GetLeft());
    m_viewMemGraph.SetTop(m_viewRect.GetTop());
    m_viewMemGraph.SetRight(m_viewRect.GetRight());
    m_viewMemGraph.SetBottom(m_viewRect.GetTop() + 50);

	m_viewMemBall.SetLeft(m_viewRect.GetLeft());
	m_viewMemBall.SetTop(m_viewMemGraph.GetBottom() + 2);
	m_viewMemBall.SetRight(m_viewRect.GetRight());
	m_viewMemBall.SetBottom(m_viewRect.GetBottom());

	if(!bStart)
	{
		RecomputeParabola();
		bStart = true;

		totalMem = theSystem->MEM()->GetTotalMemory();
		usedMem = theSystem->MEM()->GetUsedMemory();
		avaliable = theSystem->MEM()->GetAvaliableMemory();
	}
#endif

	DrawBackGround(pMemDC, m_colBackPen, m_colBackBrush);

#if BALL
	DrawRuler(pMemDC);
#endif

	Render(pMemDC);
	dc.Blit(0, 0, m_viewRect.GetWidth(), m_viewRect.GetHeight(), pMemDC, 0 ,0);
}

void CNextMMemView::Render(wxDC* pDC)
{
#if BALL
	if (ballActive)
	{
		int iRed = (255 * _memVal) / 100;
		int iGreen = iRed == 0 ? 255 : 255 - iRed;

		pDC->SetBrush(wxBrush(wxColour(iRed, iGreen, 0)));
		pDC->SetPen(wxPen(wxColour(iRed, iGreen, 0),2));

		if(bRight)
			pDC->DrawLine(0, _memLineY, (int)_memLineX, _memLineY);
		else
			pDC->DrawLine((int)_memLineX, _memLineY, m_viewMemGraph.GetRight(), _memLineY);

		pDC->DrawCircle(wxPoint((int)ballX,(int)ballY),(int)ballR);

		pDC->SetTextForeground(wxColour(iRed, iGreen, 0));
		pDC->DrawText(wxString::Format(wxT("%d%"), _memVal), wxPoint((int)ballX - 7,(int)ballY - 25));
	}

	float fTotalGB = (float)totalMem / 1024.0f;
	float fUsedGB = (float)usedMem / 1024.0f;

	wxString strMem = wxString::Format(wxT("%3.2fGB / %3.2fGB"), fUsedGB, fTotalGB);
	pDC->DrawText(strMem, m_viewRect.GetWidth() / 3 + 15, m_viewRect.GetHeight() / 2);
#else
	fDelay -= fElaspsedTime;
	if (fDelay <= 0.0f)
	{
		if(_memVal <= 25)
			ballCnt = 3;
		else if(_memVal <= 50)
			ballCnt = 4;
		else if(_memVal <= 75)
			ballCnt = 5;
		else
			ballCnt = 6;

		fDelay = ((float)rand() / (float)RAND_MAX) * 1.5f + 0.1f;
		for(int i = 0; i < ballCnt ; i++)
			listFireworks.push_back({m_viewRect,  _memVal, (float)m_viewRect.GetRight() / 2, (float)m_viewRect.GetHeight(), 120 + (rand() % 100)});//(rand() % 500)});
	}

	for (auto &f : listFireworks)
	{
		f.Update(fElaspsedTime);
		f.DrawSelf(pDC);
	}

	listFireworks.remove_if([](const cFireworks &f) {return f.bExpired; });
#endif
}

void CNextMMemView::DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush)
{
    pDC->SetPen(colPen);
	pDC->SetBrush(colBrush);
	pDC->DrawRectangle(m_viewRect);
#if BALL
	pDC->SetPen(colPen);
	pDC->SetBrush(wxColour(227, 206, 253));
	pDC->DrawRectangle(m_viewMemGraph);
#endif
}
#if BALL
void CNextMMemView::DrawRuler(wxDC* pDC)
{
	wxPen penRow(wxColour(80, 80, 80), 1, wxPENSTYLE_DOT);
	pDC->SetPen(penRow);

	unsigned int y_axis = 0;
	int y_axisRatio = m_viewMemBall.GetHeight() / CONST_YAXIS_RATIO;

	for(y_axis; y_axis < CONST_YAXIS_RATIO; y_axis++ )
		pDC->DrawLine(m_viewMemBall.GetLeft(), m_viewMemBall.GetBottom() - (y_axis * y_axisRatio), m_viewMemBall.GetRight(), m_viewMemBall.GetBottom() - (y_axis * y_axisRatio));
}
#endif // BALL

void CNextMMemView::OnTimer(wxTimerEvent& event)
{
	UpdateData();
	theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMMemView::UpdateData()
{
#if BALL
	if(!ballActive)
	{
		_membefore = 0;
		_memLineX = m_viewMemGraph.GetRight();

		bRight = false;

		totalMem = theSystem->MEM()->GetTotalMemory();
		usedMem = theSystem->MEM()->GetUsedMemory();

		avaliable = theSystem->MEM()->GetAvaliableMemory();

		_memVal = (100 * usedMem) / totalMem;
		if(_membefore != _memVal)
		{
			_membefore = _memVal;
			_memLineY = m_viewMemGraph.GetBottom() - (_membefore * m_viewMemGraph.GetHeight()) / 100;
		}

		int x = (_memVal * m_viewMemBall.GetWidth()) / 100;
		int y = (_memVal * m_viewMemBall.GetHeight()) / 100;

		int xPos = m_viewMemBall.GetRight() - 10;//x;
		int yPos = m_viewMemBall.GetBottom() - y;

		if(_memVal > 50)
		{
			bRight = true;
			xPos = m_viewRect.GetLeft() + 10;
			_memLineX = 0;
		}

		StartDropAtX(xPos, yPos);
	}

	UpdatePhysics();
#else
	if((secondCount % 40) == 0 )
	{
		secondCount = 0;

		theSystem->MEM()->Update();
		totalMem = theSystem->MEM()->GetTotalMemory();
		usedMem = theSystem->MEM()->GetUsedMemory();

		avaliable = theSystem->MEM()->GetAvaliableMemory();

		_memVal = (100 * usedMem) / totalMem;

		DlgSystemResource* pDlg = (DlgSystemResource *)this->GetParent();
		pDlg->UpdateMemory(totalMem, usedMem);
	}

	++secondCount;
#endif
}

void CNextMMemView::UpdateListener(wxCommandEvent& event)
{
	totalMem = theSystem->MEM()->GetTotalMemory();
	usedMem = theSystem->MEM()->GetUsedMemory();

	avaliable = theSystem->MEM()->GetAvaliableMemory();

	_memVal = (100 * usedMem) / totalMem;

	DlgSystemResource* pDlg = (DlgSystemResource *)this->GetParent();
	pDlg->UpdateMemory(totalMem, usedMem);
}
