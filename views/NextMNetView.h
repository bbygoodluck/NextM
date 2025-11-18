#ifndef NEXTMNETVIEW_H_INCLUDED
#define NEXTMNETVIEW_H_INCLUDED

#include "../system/IUpdateListener.h"

struct sParticle {
	float x = 0.0f;
	float y = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
	float fuse = 0.0f;
	float lifetime = 0.0f;
	wxColour col;
};

class CNetData
{
public:
	CNetData(const wxRect& rc, bool bSent, float fMaxSpeed, int starCount)
	{
		m_rc = rc;

		_fForwardSpeed  = 6.0f * fMaxSpeed + 8.1f;//((float)rand() / (float)RAND_MAX) * 8.5f + 4.1f;
		_fBackwardSpeed = ((float)rand() / (float)RAND_MAX) * 2.0f + 2.1f;

		bActiveBack = false;
		bExpired = false;

		if(bSent)
			col = wxColour(rand() % 100 + 155, rand() % 100 + 155, rand() % 240 + 15);
		else
			col = wxColour(rand() % 100 + 155, rand() % 240 + 15, rand() % 100 + 155);

		yPos = rand() % m_rc.GetHeight() + 10;

		m_bSent = bSent;
		if(bSent)
		{
			x1Pos = m_rc.GetRight();
			x2Pos = m_rc.GetRight();
			yPos = m_rc.GetTop() + rand() % m_rc.GetHeight() + 12;
			if(yPos >= m_rc.GetBottom())
				yPos -= 2;
		}

		starCnt = starCount;
	}

	~CNetData() {}

public:
	void Update(float fElaspsedTime)
	{
		//불꽃이 생성된 후 떨어지는 속도
		float fGravity = 25.0f;
		//터진후 벌어지는 X의크기
		float fDrag = 0.999f;
		//터진 후 번지는 y의크기
		float fExplodedGravity = 30.0f;

		if(m_bSent)
		{
			if(x1Pos > m_rc.GetLeft())
				x1Pos -= _fForwardSpeed;

			if(x1Pos <= (m_rc.GetLeft() + 200))
			{
				bActiveBack = true;
				bForwardEnd = true;
			}

			if(bActiveBack)
				x2Pos -= bForwardEnd ?_fForwardSpeed : _fBackwardSpeed;

			if(x1Pos <= m_rc.GetLeft() && x2Pos <= m_rc.GetLeft())
			{
				bExpired = true;
				//폭발...
			}
		}
		else
		{
			if(x1Pos < m_rc.GetRight())
				x1Pos += _fForwardSpeed;

			if(x1Pos >= (m_rc.GetRight() - 200))
			{
				bActiveBack = true;
				bForwardEnd = true;
			}

			if(bActiveBack)
				x2Pos += bForwardEnd ?_fForwardSpeed : _fBackwardSpeed;

			if(x1Pos >= m_rc.GetRight() && x2Pos >= m_rc.GetRight())
			{
				bExpired = true;
				//폭발...
			}
		}

		if(!m_bHasExploded)
		{
			m_bHasExploded = true;
		//	wxColour col = wxColour(rand() % 100 + 155, rand() % 240 + 15, rand() % 240 + 15);

			for(int i = 0 ; i < starCnt; i++)
			{
				sParticle s;

				s.x = m_bSent ? m_rc.GetLeft() : m_rc.GetRight();
				s.y = yPos;

				s.col = col;

				float angle = RandomFloat((float)(2.0f * M_PI));
				float power = RandomFloat(50.0f);

				s.fuse = 1.2f;//RandomFloat(3.0f) + 1.0f;

				s.vx = cosf(angle) * power;
				s.vy = sinf(angle) * power;

				vecParticle.emplace_back(s);
			}
		}

		bool bParticleUpdate = false;
		if(m_bSent)
		{
			if(x1Pos <= m_rc.GetLeft())
				bParticleUpdate = true;
		}
		else
		{
			if(x1Pos >= m_rc.GetRight())
				bParticleUpdate = true;
		}

		if(bParticleUpdate)//x1Pos >= m_rc.GetRight())
		{
			for (auto &s : vecParticle)
			{
				s.lifetime += fElaspsedTime;

				float fDisp = (s.vx + fExplodedGravity) * fElaspsedTime;
				if(m_bSent)
					s.x -= fDisp;
				else
					s.x += fDisp;

				s.vy *= fDrag;

				if(m_bSent)
					s.y -= s.vy * fElaspsedTime;
				else
					s.y += s.vy * fElaspsedTime;

				/*
				s.vx *= fDrag;
				s.x += s.vx * fElaspsedTime;
				float fDisp = (s.vy + fExplodedGravity) * fElaspsedTime;
				s.y += fDisp;
				*/
			}
		}
	}

	void DrawSelf(wxDC* pDC)
	{
		if(!bExpired)
		{
			pDC->SetPen(wxPen(col, 1, wxPENSTYLE_DOT));
			pDC->DrawLine(x1Pos, yPos, x2Pos , yPos);

			for (auto &s : vecParticle)
			{
				if (s.lifetime <= s.fuse)
				{
					wxPen penParticle(wxColour(s.col));
					pDC->SetPen(penParticle);
					pDC->DrawPoint(s.x, s.y);
				}
			}
		}
	}

	float RandomFloat(float fMax)
	{
		return ((float)rand() / (float)RAND_MAX) * fMax;
	}

	bool GetForwardEnd() { return bForwardEnd; }
	bool bExpired = false;

private:
	float _fForwardSpeed = 0.0f;
	float _fBackwardSpeed = 0.0f;

	int x1Pos = 0;
	int x2Pos = 0;
	int yPos  = 0;
	int starCnt = 80;

	wxColour col;
	wxRect m_rc;

	bool bActiveBack    = false;
	bool bForwardEnd    = false;
	bool m_bSent        = false;
	bool m_bHasExploded = false;

	std::vector<sParticle> vecParticle;
};

class CBoilling
{
public:
	CBoilling() {}
	~CBoilling() {}

public:
	float x = 0.0f;
	float y = 0.0f;
	float r = 0.0f;
	float fuse = 3.5f;
	float boiltime = 0.0f;
	bool bExpired = false;
	wxColour col;

public:
	void UpdateBoil(float fElaspedTime)
	{
		r += 2.7f * fElaspedTime;
		boiltime += fElaspedTime;
	}

	void DrawBoil(wxDC* pDC)
	{
		if(boiltime <= fuse)
		{
			pDC->SetPen(col);
			pDC->SetBrush(wxBrush(col));
			pDC->DrawEllipse(x, y, -r * 4, 10);
		}
		else
			bExpired = true;
	}
};

class CNetworkAdpData
{
	int speedVals[11] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22};
	int widthVals[11] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33};

public:
	CNetworkAdpData(const wxRect& rc, bool bDownload, const Net::net_stat& data)
		: m_rc(rc)
		, m_fSpeed(6.0f)
		, IsDownload(bDownload)
		, _data(data)
	{
		_xPos = bDownload ? rc.GetLeft() : rc.GetRight();
		_yPos = rc.GetTop() + rand() % rc.GetHeight();

		_speedindex = GetUnitIndex(data.speed, false, false, 0, false);
		_str_speed = theUtility->floating_humanizer(data.speed, false, false, 0, false, true);

		m_fSpeed = 6.0f + speedVals[_speedindex];
		m_fWidth = 2.0f + (float)widthVals[_speedindex];
	}

	~CNetworkAdpData() {}

public:
	void Update() {

		if(IsDownload)
		{
			if((m_rc.GetRight() - 10) < _xPos)
				bExpired = true;

			_xPos += m_fSpeed;
		}
		else
		{
			if(m_rc.GetLeft() > _xPos)
				bExpired = true;

			_xPos -= m_fSpeed;
		}
	}

	void DrawSelf(wxDC* pDC)
	{
		pDC->SetPen(wxPen(wxColour(80, 80, 80)));
		pDC->SetBrush(wxBrush(wxColour(80, 80, 80)));
		pDC->DrawCircle(wxPoint(_xPos + 1, _yPos + 1), m_fWidth);

		wxColour col = IsDownload ? colDn : colUp;

		pDC->SetPen(wxPen(col));
		pDC->SetBrush(wxBrush(col));

		pDC->DrawCircle(wxPoint(_xPos, _yPos), m_fWidth);

		if(_speedindex >= 2)
		{
			wxFont font(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, _T("Segoe UI"));

			col = IsDownload ? wxColour(0, 255, 255) : wxColour(255, 255, 0);

			pDC->SetFont(font);
			pDC->SetTextForeground(col);
			pDC->DrawText(_str_speed, _xPos - 20, _yPos - 7);
		}
	}

private:

	int GetUnitIndex(uint64_t value, const bool is_mega, const bool shorten, size_t start, const bool bit)
	{
		std::string out;
		const size_t mult = (bit) ? 8 : 1;
		const bool mega = is_mega;

		value *= 100 * mult;

		if (mega)
		{
			while (value >= 100000)
			{
				value /= 1000;
				if (value < 100)
					break;

				start++;
			}
		}
		else
		{
			while (value >= 102400)
			{
				value >>= 10;
				if (value < 100)
					break;

				start++;
			}
		}

		return start;
	}
public:
	bool bExpired = false;
	bool IsDownload = true;

private:
	wxRect m_rc;
	float m_fSpeed;

	int _xPos = 0;
	int _yPos = 0;
	int _speedindex = 0;
	wxString _str_speed{wxT("")};
	wxString _str_speed_bit{wxT("")};

	Net::net_stat _data;

	float m_fWidth = 3.0f;

	wxColour colDn{255, 30, 128};
	wxColour colUp{169, 207, 214};
};

class CNextMNetView : public wxWindow, public IUpdateListener
{
public:
	explicit CNextMNetView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle = CHILD_WINDOW_STYLE);
	~CNextMNetView();

public:
	void Start(int _milliseconds);
	void Stop();

	void AddNetIntefaces();

protected:
	void Render(wxDC* pDC);
	void DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush);
	void DrawData(wxDC* pDC);
	void DisplayNetInfo(wxDC* pDC);

//	void UpdateData();

	void CreateBoilData();

private:
	CMemoryDC m_memDC;
	wxTimer m_timer;
	//배경
	wxColour m_colBackPen;
	wxColour m_colBackBrush;

	//In Color
	wxColour m_colIn = wxColour(0, 220, 0);
	//Out Color
	wxColour m_colOut = wxColour(249, 124, 0);

	wxFont m_viewFont = wxNullFont;

	//전체크기
	wxRect m_viewRect     = wxRect(0, 0, 0, 0);

	//데이터 영역
	wxRect m_viewDataArea{0, 0, 0, 0};
	wxRect m_viewTextArea{0, 0, 0, 0};
	wxRect m_viewRecvArea{0, 0, 0, 0};
	wxRect m_viewSendArea{0, 0, 0, 0};

	wxRect m_viewReceived{0, 0, 0, 0};
	wxRect m_viewSend{0, 0, 0, 0};

	float _fDelay = 1.0f;
	float _fElaspsedTime = 0.1f;
	float _fRSpeed = 2.0f;
	float _fSSpeen = 2.0f;

	std::list<CNetData> lstReceiveDatas;
	std::list<CNetData> lstSentDatas;

	double dblDownSpeed_ = 0.0f;
	double dblUpSpeed_ = 0.0f;

	unsigned long ulInData_ = 0;
	unsigned long ulInMaxbps_ = 0;

	unsigned long ulOutData_ = 0;
	unsigned long ulOutMaxbps_ = 0;

	wxString m_strInData    = wxT("");
	wxString m_strOutData   = wxT("");
	wxString m_strInMaxBps  = (wxT(""));
	wxString m_strOutMaxBps = (wxT(""));

	float _fTimerDelay = 3.5f;
	bool _bMonitoringStart = false;

	unsigned int _initMaxDataCount = 0;

	std::list<CBoilling> vecBoildata;
	float _fBoilDelay = 1.2f;

	std::list<CNetworkAdpData> m_data;

	Net::net_info _netInfo;
private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);

	void UpdateListener(wxCommandEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMNETVIEW_H_INCLUDED
