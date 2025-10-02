#ifndef NEXTMNETVIEW_H_INCLUDED
#define NEXTMNETVIEW_H_INCLUDED

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

class CNextMNetView : public wxWindow
{
public:
	explicit CNextMNetView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle = CHILD_WINDOW_STYLE);
	~CNextMNetView();

public:
	void Start(int _milliseconds);
	void Stop();

protected:
	void Render(wxDC* pDC);
	void DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush);
	void DrawData(wxDC* pDC);
	void DisplayNetInfo(wxDC* pDC);

	void UpdateData();

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
	wxRect m_viewReceived = wxRect(0, 0, 0, 0);
	wxRect m_viewSend     = wxRect(0, 0, 0, 0);

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

	wxString m_strInData   = wxT("");
	wxString m_strOutData  = wxT("");
	wxString m_strInMaxBps  = (wxT(""));
	wxString m_strOutMaxBps = (wxT(""));

	float _fTimerDelay = 3.5f;
	bool _bMonitoringStart = false;

	unsigned int _initMaxDataCount = 0;

	std::list<CBoilling> vecBoildata;
	float _fBoilDelay = 1.2f;

private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);

	void OnTimer(wxTimerEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMNETVIEW_H_INCLUDED
