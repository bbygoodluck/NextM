#include "../ginc.h"
#include "../system/Sysinc.h"
#include "../dialog/DlgSystemResource.h"
#include "NextMNetView.h"

wxBEGIN_EVENT_TABLE(CNextMNetView, wxWindow)
	EVT_PAINT(CNextMNetView::OnPaint)
	EVT_ERASE_BACKGROUND(CNextMNetView::OnErase)
	EVT_SIZE(CNextMNetView::OnSize)
	EVT_MY_CUSTOM_COMMAND(wxEVT_RESOURCE_MONITORING, wxID_ANY, CNextMNetView::UpdateListener)
wxEND_EVENT_TABLE()

CNextMNetView::CNextMNetView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle)
	: wxWindow(parent, nID, wxDefaultPosition, sz, lStyle)
	, m_colBackPen(0, 0, 0)
	, m_colBackBrush(20, 20, 20)
{
	this->SetBackgroundStyle(wxBG_STYLE_ERASE);
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CNextMNetView::OnTimer));

	//최초 데이터 적재
//	_fTimerDelay = 0.1f;
//	UpdateData();

	m_timer.SetOwner(this, wxTIMER_ID++);
	Start(80);

#ifdef __WXMSW__
	m_viewFont.Create(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Fixedsys"));
#else
	m_viewFont = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
#endif
}

CNextMNetView::~CNextMNetView()
{
	Stop();
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(CNextMNetView::OnTimer));
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

    theDCUtil->Refresh(this, m_viewRect);
}

void CNextMNetView::OnTimer(wxTimerEvent& event)
{
//	UpdateData();
	theDCUtil->Refresh(this, m_viewRect);
}

void CNextMNetView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

    m_viewRect = GetClientRect();

	int w = m_viewRect.GetWidth();
	int h = m_viewRect.GetHeight();

	int center = m_viewRect.GetWidth() / 2;
	int heightcenter = m_viewRect.GetHeight() / 2;

	m_viewDataArea.SetLeft(m_viewRect.GetLeft());
	m_viewDataArea.SetTop(m_viewRect.GetTop());
	m_viewDataArea.SetRight(m_viewRect.GetRight() - 280);
	m_viewDataArea.SetBottom(m_viewRect.GetBottom());

	m_viewTextArea.SetLeft(m_viewDataArea.GetRight());
	m_viewTextArea.SetTop(m_viewDataArea.GetTop());
	m_viewTextArea.SetRight(m_viewRect.GetRight());
	m_viewTextArea.SetBottom(m_viewRect.GetBottom());

	m_viewSendArea.SetLeft(m_viewRect.GetLeft());
	m_viewSendArea.SetTop(m_viewRect.GetTop());
	m_viewSendArea.SetRight(m_viewDataArea.GetRight() - 10);
	m_viewSendArea.SetBottom(heightcenter);

	m_viewRecvArea.SetLeft(m_viewRect.GetLeft());
	m_viewRecvArea.SetTop(m_viewRect.GetTop() + heightcenter);
	m_viewRecvArea.SetRight(m_viewDataArea.GetRight() - 10);
	m_viewRecvArea.SetBottom(m_viewRect.GetBottom());

//	m_viewRecvText.SetLeft(m_viewRect.GetLeft());
//	m_viewRecvText.SetTop(m_viewRect.GetTop());
//	m_viewRecvText.SetRight(150);
//	m_viewRecvText.SetBottom(m_viewRect.GetBottom());

//	m_viewReceived.SetLeft(m_viewRect.GetLeft() + 280);
//	m_viewReceived.SetTop(m_viewRect.GetTop());
//	m_viewReceived.SetRight(center);
//	m_viewReceived.SetBottom(m_viewRect.GetBottom());
//
//	m_viewSend.SetLeft(m_viewReceived.GetRight() + 10);
//	m_viewSend.SetTop(m_viewRect.GetTop());
//	m_viewSend.SetRight(m_viewRect.GetRight() - 280);
//	m_viewSend.SetBottom(m_viewRect.GetBottom());

//	m_viewSendText.SetLeft(m_viewSend.GetRight() + 10);
//	m_viewSendText.SetTop(m_viewSend.GetTop());
//	m_viewSendText.SetRight(m_viewRect.GetRight());
//	m_viewSendText.SetBottom(m_viewRect.GetBottom());

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
	if(m_viewRect.GetWidth() == 0)
		return;

	DisplayNetInfo(pDC);
	DrawData(pDC);
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

//	pDC->SetPen(wxPen(wxColour(128, 128, 128), 1, wxPENSTYLE_DOT));
//	pDC->DrawLine(wxPoint(m_viewRecvArea.GetLeft(), m_viewRecvArea.GetTop()), wxPoint(m_viewRecvArea.GetRight(), m_viewRecvArea.GetTop()));
//	pDC->SetPen(wxPen(wxColour(255, 255, 255)));
//	pDC->DrawRectangle(m_viewSendArea);
//	pDC->DrawRectangle(m_viewRecvArea);

	pDC->SetPen(wxPen(wxColour(128, 128, 128), 1, wxPENSTYLE_DOT));
	pDC->DrawLine(wxPoint(m_viewTextArea.GetLeft(), m_viewTextArea.GetHeight() / 2), wxPoint(m_viewTextArea.GetRight(), m_viewTextArea.GetHeight() / 2));
}

void CNextMNetView::DrawData(wxDC* pDC)
{
//	_fDelay -= _fElaspsedTime;
//	if(_fDelay <= 0.0f)
//	{
//		double dblGB = 1000.0f * 1000.0f;
//		double dblMB = 1000.0f;
//
//		int DnStarCount = 200;
//		int UpStarCount = 200;
//
//		if(dblDownSpeed_ >= dblGB)
//			DnStarCount = 400;
//		else if(dblDownSpeed_ >= dblMB)
//			DnStarCount = 300;
//
//		if(dblUpSpeed_ >= dblGB )
//			UpStarCount = 400;
//		else if(dblUpSpeed_ >= dblMB)
//			UpStarCount = 300;
//
//		lstReceiveDatas.push_back({m_viewReceived, false, _fRSpeed, DnStarCount});
//		lstSentDatas.push_back({m_viewSend, true, _fSSpeen, UpStarCount});
//
//		_fDelay = ((float)rand() / (float)RAND_MAX) * 1.5f + 1.0f;
//	}
//
//	for (auto &r : lstReceiveDatas)
//	{
//		r.Update(_fElaspsedTime);
//		r.DrawSelf(pDC);
//	}
//
//	for (auto &s : lstSentDatas)
//	{
//		s.Update(_fElaspsedTime);
//		s.DrawSelf(pDC);
//	}
//
//	lstReceiveDatas.remove_if([](const CNetData &f) {return f.bExpired; });
//	lstSentDatas.remove_if([](const CNetData &f) {return f.bExpired; });

	for (auto &data : m_data)
	{
		data.Update();
		data.DrawSelf(pDC);
	}

	m_data.remove_if([](const CNetworkAdpData &f) {return f.bExpired; });
}

void CNextMNetView::DisplayNetInfo(wxDC* pDC)
{
	if(_netInfo.stat.size() == 0)
		return;

	int dnXpadding = 10;
	int dnYpadding = 5;

	wxString speed{wxT("")};
	wxString speed_bits{wxT("")};
	wxString top{wxT("")};
	wxString total{wxT("")};

	wxString strSpeedDisp{wxT("")};
	for(const wxString sKey : {wxT("download"), wxT("upload")})
	{
		bool bDownload = false;
		auto net = _netInfo.stat.at(sKey);

		if(sKey.CmpNoCase(wxT("download")) == 0)
		{
			bDownload = true;
			speed      = wxT("Recv  : ");
		}
		else
			speed      = wxT("Send  : ");

		speed      += theUtility->floating_humanizer(net.speed, false, false, 0, false, true);
		speed_bits = theUtility->floating_humanizer(net.speed, false, false, 0, true, true);
		top        = wxT("Top   : ") + theUtility->floating_humanizer(net.top, false, false, 0, false, true);
		total      = wxT("Total : ") + theUtility->floating_humanizer(net.total, true, false, 0, false, false);

		strSpeedDisp = speed + wxT(" (") + speed_bits + wxT(")");

		wxRect rcDispArea = bDownload ? m_viewRecvArea : m_viewSendArea;

		int xPos = m_viewTextArea.GetLeft() + rcDispArea.GetLeft();
		int yPos = m_viewTextArea.GetTop() + rcDispArea.GetTop() + 10;

		pDC->SetTextForeground(bDownload ? m_colIn : wxColour(110, 189, 211));

		pDC->DrawText(strSpeedDisp, xPos, yPos);
		pDC->DrawText(top,          xPos, yPos + (dnYpadding * 6));
		pDC->DrawText(total,        xPos, yPos + (dnYpadding * 12));
	}

//	auto fnSpeed = [](double _dblSpeed, bool _bDown) {
//		double dblGB = 1000.0f * 1000.0f;
//		double dblMB = 1000.0f;
//
//		double dblspdG = _dblSpeed / dblGB;
//		double dblspdM = _dblSpeed / dblMB;
//
//		wxString strRet(wxT(""));
//		if(_dblSpeed >= dblGB)
//			strRet = _bDown ? wxString::Format(wxT("Recv Spd : %.2f GiB/sec"), dblspdG) : wxString::Format(wxT("Send Spd : %.2f GiB/sec"), dblspdG);
//		else if(_dblSpeed >= dblMB)
//			strRet = _bDown ? wxString::Format(wxT("Recv Spd : %.2f MiB/sec"), dblspdM) : wxString::Format(wxT("Send Spd : %.2f MiB/sec"), dblspdM);
//		else
//			strRet = _bDown ? wxString::Format(wxT("Recv Spd : %.2f KiB/sec"), _dblSpeed) : wxString::Format(wxT("Send Spd : %.2f KiB/sec"), _dblSpeed);
//
//		return strRet;
//	};
//
//	wxSize szText = pDC->GetTextExtent(wxT("Kbps"));
//	//Down Info
//	int dnlPadding = 40;
//	int dntPadding = 1;
//
//	wxString strDownSpeed = fnSpeed(dblDownSpeed_, true);
//	pDC->SetTextForeground(m_colIn);
//	pDC->DrawText(wxT("Down : ") + m_strInData, m_viewReceived.GetRight() + dnlPadding, m_viewReceived.GetTop() + dntPadding);
//	pDC->DrawText(m_strInMaxBps, m_viewReceived.GetRight() + dnlPadding, m_viewReceived.GetTop() + dntPadding + szText.GetHeight());
//	pDC->DrawText(strDownSpeed, m_viewReceived.GetRight() + dnlPadding, m_viewReceived.GetTop() + dntPadding + (szText.GetHeight() * 2));
//
//	//Sent Info
//	int uplPadding = 180;
//	int uptPadding = 10;
//
//	wxString strUpSpeed = fnSpeed(dblUpSpeed_, true);
//	pDC->SetTextForeground(m_colOut);
//	pDC->DrawText(wxT("Sent : ") + m_strOutData, m_viewSend.GetLeft() - uplPadding, m_viewSend.GetTop() + uptPadding);
//	pDC->DrawText(m_strOutMaxBps, m_viewSend.GetLeft() - uplPadding, m_viewSend.GetTop() + uptPadding + szText.GetHeight());
//	pDC->DrawText(strUpSpeed, m_viewSend.GetLeft() - uplPadding,m_viewSend.GetTop() + uptPadding + (szText.GetHeight() * 2));
}

void CNextMNetView::UpdateListener(wxCommandEvent& event)
{
	if(m_viewRect.GetWidth() == 0)
		return;

	_netInfo = theSystem->NET()->GetNetworkInfo();

//	auto stat = net_info.stat.at(wxT("download"));
	for(const wxString sKey : {wxT("download"), wxT("upload")})
	{
		auto net = _netInfo.stat.at(sKey);
		bool bDownload = (sKey.CmpNoCase(wxT("download")) == 0) ? true : false;

		wxRect rc = bDownload ? m_viewRecvArea : m_viewSendArea;

		if(net.speed > 0)
			m_data.push_back({rc, bDownload, net});
	}

	theDCUtil->Refresh(this, m_viewRect);
}

//void CNextMNetView::UpdateData()
//{
//	auto net_info = theSystem->NET()->GetNetworkInfo();
//	auto stat = net_info.stat.at(wxT("download"));
//	wxString strS{""};
//
////	_fTimerDelay -= _fElaspsedTime;
////	if(_fTimerDelay <= 0.0f)
////	{
////		_fTimerDelay = 3.5f;
////		_initMaxDataCount++;
////
////		auto net_info = theSystem->NET()->GetNetworkInfo();
////		theSystem->NET()->Update();
////
////		ulInData_     = theSystem->NET()->GetInBytes();
////		ulOutData_    = theSystem->NET()->GetOutBytes();
////
////		dblDownSpeed_ = theSystem->NET()->GetDownSpeed();
////		dblUpSpeed_   = theSystem->NET()->GetUpSpeed();
////
////		if(ulInData_ > 0)
////			ulInData_ = ulInData_ / 1000; //kpbs
////
////		if(ulInMaxbps_ < ulInData_)
////			ulInMaxbps_ = ulInData_;
////
////		if(ulOutData_ > 0)
////			ulOutData_ = ulOutData_ / 1000;
////
////		if(ulOutMaxbps_ < ulOutData_)
////			ulOutMaxbps_ = ulOutData_;
////
////		m_strInData = theSystem->NET()->GetBandWidth(ulInData_, false);
////		m_strOutData = theSystem->NET()->GetBandWidth(ulOutData_, false);
////
////		m_strInMaxBps  = theSystem->NET()->GetBandWidth(ulInMaxbps_, true);
////		m_strOutMaxBps = theSystem->NET()->GetBandWidth(ulOutMaxbps_, true);
////
////		_bMonitoringStart = true;
////
////		if(_initMaxDataCount == 40)
////		{
////			_initMaxDataCount = 0;
////
////			ulInMaxbps_  = 0;
////			ulOutMaxbps_ = 0;
////		}
////	}
//}
