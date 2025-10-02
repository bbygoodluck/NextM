#include "../ginc.h"
#include "../system/Sysinc.h"
#include "../dialog/DlgSystemResource.h"
#include "NextMNetView.h"

wxBEGIN_EVENT_TABLE(CNextMNetView, wxWindow)
	EVT_PAINT(CNextMNetView::OnPaint)
	EVT_ERASE_BACKGROUND(CNextMNetView::OnErase)
	EVT_SIZE(CNextMNetView::OnSize)
wxEND_EVENT_TABLE()

CNextMNetView::CNextMNetView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle)
	: wxWindow(parent, nID, wxDefaultPosition, sz, lStyle)
	, m_colBackPen(0, 0, 0)
	, m_colBackBrush(80, 80, 80)
{
	this->SetBackgroundStyle(wxBG_STYLE_ERASE);
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CNextMNetView::OnTimer));

	//최초 데이터 적재
//	_fTimerDelay = 0.1f;
//	UpdateData();

	m_timer.SetOwner(this, wxTIMER_ID++);
	Start(30);

#ifdef __WXMSW__
	m_viewFont.Create(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Segoe UI"));
#else
	m_viewFont = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
#endif
}

CNextMNetView::~CNextMNetView()
{
	Stop();
}

void CNextMNetView::Start(int _milliseconds)
{
	m_timer.Start(_milliseconds);
}

void CNextMNetView::Stop()
{
	m_timer.Stop();
}

void CNextMNetView::OnErase(wxEraseEvent& event)
{

}

void CNextMNetView::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

    theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMNetView::OnTimer(wxTimerEvent& event)
{
	UpdateData();
	theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMNetView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

    m_viewRect = GetClientRect();

	int w = m_viewRect.GetWidth();
	int h = m_viewRect.GetHeight();

	m_viewReceived.SetLeft(m_viewRect.GetLeft());
	m_viewReceived.SetTop(m_viewRect.GetTop());
	m_viewReceived.SetRight(m_viewRect.GetRight() - 200);
	m_viewReceived.SetBottom((m_viewRect.GetBottom() / 2) - 20);

	m_viewSend.SetLeft(m_viewRect.GetLeft() + 200);
	m_viewSend.SetTop(m_viewReceived.GetBottom() + 10);
	m_viewSend.SetRight(m_viewRect.GetRight());
	m_viewSend.SetBottom(m_viewRect.GetBottom());

	wxMemoryDC* pMemDC = m_memDC.SelectObjectOnly(&dc, m_viewRect);

	pMemDC->SetFont(m_viewFont);

	DrawBackGround(pMemDC, m_colBackPen, m_colBackBrush);
	Render(pMemDC);

	dc.Blit(0, 0, w, h, pMemDC, 0 ,0);
}

void CNextMNetView::CreateBoilData()
{
	bool bMinus = false;
	for(int i = 0; i < 3; i++)
	{
		CBoilling s;
		s.x = m_viewReceived.GetRight();
		s.y = ((rand() % 10) * bMinus == true ? -1 : 1) + m_viewReceived.GetHeight() / 2;

		wxColour col = wxColour(70,140,255);//rand() % 100 + 155, rand() % 100 + 155, rand() % 240 + 15);
		s.col = col;

		vecBoildata.push_back(s);

		bMinus = !bMinus;
	}
}

void CNextMNetView::Render(wxDC* pDC)
{
	if(!_bMonitoringStart)
		return;

	DrawData(pDC);
	DisplayNetInfo(pDC);
/*
	_fBoilDelay -= _fElaspsedTime;

	if(_fBoilDelay <= 0.0f)
	{
		_fBoilDelay = 1.2f;
		CreateBoilData();
	}

	for(auto &s : vecBoildata)
	{
		s.UpdateBoil(0.2f);
		s.DrawBoil(pDC);
	}

	vecBoildata.remove_if([](const CBoilling &b) {return b.bExpired; });
*/
}

void CNextMNetView::DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush)
{
    pDC->SetPen(colPen);
	pDC->SetBrush(colBrush);
	pDC->DrawRectangle(m_viewRect);
}

void CNextMNetView::DrawData(wxDC* pDC)
{
	_fDelay -= _fElaspsedTime;
	if(_fDelay <= 0.0f)
	{
		double dblGB = 1000.0f * 1000.0f;
		double dblMB = 1000.0f;

		int DnStarCount = 200;
		int UpStarCount = 200;

		if(dblDownSpeed_ >= dblGB)
			DnStarCount = 400;
		else if(dblDownSpeed_ >= dblMB)
			DnStarCount = 300;

		if(dblUpSpeed_ >= dblGB )
			UpStarCount = 400;
		else if(dblUpSpeed_ >= dblMB)
			UpStarCount = 300;

		lstReceiveDatas.push_back({m_viewReceived, false, _fRSpeed, DnStarCount});
		lstSentDatas.push_back({m_viewSend, true, _fSSpeen, UpStarCount});

		_fDelay = ((float)rand() / (float)RAND_MAX) * 1.5f + 1.0f;
	}

	for (auto &r : lstReceiveDatas)
	{
		r.Update(_fElaspsedTime);
		r.DrawSelf(pDC);
	}

	for (auto &s : lstSentDatas)
	{
		s.Update(_fElaspsedTime);
		s.DrawSelf(pDC);
	}

	lstReceiveDatas.remove_if([](const CNetData &f) {return f.bExpired; });
	lstSentDatas.remove_if([](const CNetData &f) {return f.bExpired; });
}

void CNextMNetView::DisplayNetInfo(wxDC* pDC)
{
	auto fnSpeed = [](double _dblSpeed, bool _bDown) {
		double dblGB = 1000.0f * 1000.0f;
		double dblMB = 1000.0f;

		double dblspdG = _dblSpeed / dblGB;
		double dblspdM = _dblSpeed / dblMB;

		wxString strRet(wxT(""));
		if(_dblSpeed >= dblGB)
			strRet = _bDown ? wxString::Format(wxT("Recv Spd : %.2f GiB/sec"), dblspdG) : wxString::Format(wxT("Send Spd : %.2f GiB/sec"), dblspdG);
		else if(_dblSpeed >= dblMB)
			strRet = _bDown ? wxString::Format(wxT("Recv Spd : %.2f MiB/sec"), dblspdM) : wxString::Format(wxT("Send Spd : %.2f MiB/sec"), dblspdM);
		else
			strRet = _bDown ? wxString::Format(wxT("Recv Spd : %.2f KiB/sec"), _dblSpeed) : wxString::Format(wxT("Send Spd : %.2f KiB/sec"), _dblSpeed);

		return strRet;
	};

	wxSize szText = pDC->GetTextExtent(wxT("Kbps"));
	//Down Info
	int dnlPadding = 40;
	int dntPadding = 1;

	wxString strDownSpeed = fnSpeed(dblDownSpeed_, true);
	pDC->SetTextForeground(m_colIn);
	pDC->DrawText(wxT("Down : ") + m_strInData, m_viewReceived.GetRight() + dnlPadding, m_viewReceived.GetTop() + dntPadding);
	pDC->DrawText(m_strInMaxBps, m_viewReceived.GetRight() + dnlPadding, m_viewReceived.GetTop() + dntPadding + szText.GetHeight());
	pDC->DrawText(strDownSpeed, m_viewReceived.GetRight() + dnlPadding, m_viewReceived.GetTop() + dntPadding + (szText.GetHeight() * 2));

	//Sent Info
	int uplPadding = 180;
	int uptPadding = 10;

	wxString strUpSpeed = fnSpeed(dblUpSpeed_, true);
	pDC->SetTextForeground(m_colOut);
	pDC->DrawText(wxT("Sent : ") + m_strOutData, m_viewSend.GetLeft() - uplPadding, m_viewSend.GetTop() + uptPadding);
	pDC->DrawText(m_strOutMaxBps, m_viewSend.GetLeft() - uplPadding, m_viewSend.GetTop() + uptPadding + szText.GetHeight());
	pDC->DrawText(strUpSpeed, m_viewSend.GetLeft() - uplPadding,m_viewSend.GetTop() + uptPadding + (szText.GetHeight() * 2));
}

void CNextMNetView::UpdateData()
{
	_fTimerDelay -= _fElaspsedTime;
	if(_fTimerDelay <= 0.0f)
	{
		_fTimerDelay = 3.5f;
		_initMaxDataCount++;

		theSystem->NET()->Update();

		ulInData_     = theSystem->NET()->GetInBytes();
		ulOutData_    = theSystem->NET()->GetOutBytes();

		dblDownSpeed_ = theSystem->NET()->GetDownSpeed();
		dblUpSpeed_   = theSystem->NET()->GetUpSpeed();

		if(ulInData_ > 0)
			ulInData_ = ulInData_ / 1000; //kpbs

		if(ulInMaxbps_ < ulInData_)
			ulInMaxbps_ = ulInData_;

		if(ulOutData_ > 0)
			ulOutData_ = ulOutData_ / 1000;

		if(ulOutMaxbps_ < ulOutData_)
			ulOutMaxbps_ = ulOutData_;

		m_strInData = theSystem->NET()->GetBandWidth(ulInData_, false);
		m_strOutData = theSystem->NET()->GetBandWidth(ulOutData_, false);

		m_strInMaxBps  = theSystem->NET()->GetBandWidth(ulInMaxbps_, true);
		m_strOutMaxBps = theSystem->NET()->GetBandWidth(ulOutMaxbps_, true);

		_bMonitoringStart = true;

		if(_initMaxDataCount == 40)
		{
			_initMaxDataCount = 0;

			ulInMaxbps_  = 0;
			ulOutMaxbps_ = 0;
		}
	}
}
