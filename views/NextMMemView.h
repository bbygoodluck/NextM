#ifndef NEXTMMEMVIEW_H_INCLUDED
#define NEXTMMEMVIEW_H_INCLUDED

#include "../system/IUpdateListener.h"
#define BALL 0

class CNextMMemView : public wxWindow, public IUpdateListener
{
public:
	explicit CNextMMemView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle = CHILD_WINDOW_STYLE);
	~CNextMMemView();

public:
	void Start(int _milliseconds);
	void Stop();

protected:
#if BALL
	void RecomputeParabola()
    {
    	int W = m_viewRect.GetWidth();
    	int H = m_viewRect.GetHeight();

        if (W <= 0 || H <= 0) return;

        h = W * 0.5;
        k = 80.0;
        groundY = H - 5;

        a = (groundY - k) / (h * h);
    }

	double ParabolaY(double x) const
	{
        return a * (x - h) * (x - h) + k;
    }

    void StartDropAtX(int xPos, int yPos)
    {
    	int W = m_viewRect.GetWidth();
    	int H = m_viewRect.GetHeight();

        if (xPos < 0) xPos = 0;
        if (xPos >= W) xPos = W - 1;

        ballX = (double)xPos;
        ballY = yPos * 1.0f;//ParabolaY(H);//ballX);
        ballVY = 0.0;
        ballActive = true;
    }

    void UpdatePhysics()
    {
        if (!ballActive) return;

        // 수평 이동 및 그래프 이동
        if(bRight)
		{
			ballX += ballVX * dt;
			_memLineX += restitution * 6.1f;
		}
		else
		{
			ballX -= ballVX * dt;
			_memLineX -= restitution * 6.1f;
		}

        ballVY += g * dt;
        ballY += ballVY * dt;

        double bottom = groundY - ballR;
        if (ballY >= bottom)
		{
            ballY = bottom;
            ballVY = -ballVY * restitution;

            // 충분히 느려지면 멈추기
            if (std::fabs(ballVY) < stopThreshold)
			{
                ballVY = 0.0;
//              ballActive = false;
            }
        }

        if(bRight)
        {
			if (ballX > m_viewRect.GetWidth() + ballR)
				ballActive = false;
        }
        else
        {
			if (ballX < ballR)
				ballActive = false;
        }
    }
#else //PARTICAL
#endif

private:
	void Render(wxDC* pDC);
	void DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush);
#if BALL
	void DrawRuler(wxDC* pDC);
#endif

	void UpdateData();
private:
	CMemoryDC m_memDC;
	wxTimer m_timer;

	//배경
	wxColour m_colBackPen;
	wxColour m_colBackBrush;

	//전체크기
	wxRect m_viewRect     = wxRect(0, 0, 0, 0);
	wxFont m_viewFont = wxNullFont;

	unsigned long totalMem = 0;
	unsigned long usedMem = 0;
	unsigned long avaliable = 0;

    unsigned int _memVal = 0;

#if BALL
	wxRect m_viewMemGraph = wxRect(0, 0, 0, 0);
	wxRect m_viewMemBall  = wxRect(0, 0, 0, 0);

	// 포물선 파라미터
    double a = 0.001;
    double h = 0.0;
    double k = 80.0;

    // 공 상태
    bool ballActive = false;
    double ballX = 0.0;
    double ballY = 0.0;
    double ballVY = 0.0;
    double ballVX = 200.0; // px/s, 오른쪽 속도
    double ballR = 7.0;

    // 물리 파라미터
    double g = 3500.0; // px/s^2
    double dt = 1.0/60; // 60fps 고정 시간 간격
    double restitution = 0.6; // 반발계수 (0~1 사이, 1=완전 탄성, 0=멈춤)
    double stopThreshold = 30.0; // 속도가 이 값보다 작으면 멈춤

    int groundY = 0;

    bool bStart = false;
	bool bRight  = false;

	unsigned int _membefore = 0;
	int _memLineY = 0;
	double _memLineX = 0;
#else
	struct sParticle {
		float x = 0.0f;
		float y = 0.0f;
		float vx = 0.0f;
		float vy = 0.0f;
		float fuse = 0.0f;
		float lifetime = 0.0f;
		wxColour col;
	};

	class cFireworks : public sParticle
	{
	public:
		cFireworks(const wxRect& rc, unsigned int memVal, float xPos, float yPos, int size)
		{
			x = xPos;
			y = yPos;
			vx = RandomFloat(100.0f) - 50.0f;
			vy = -100.0f;
			fuse = RandomFloat(3.0f);// + 1.0f;
			starCount = size;

			m_rc = rc;
			_memVal = memVal;

			ballCol = wxColour(rand() % 100 + 155, rand() % 240 + 15, rand() % 240 + 15);
		}

		void Update(float fElaspsedTime)
		{	//불꽃이 생성된 후 떨어지는 속도
			float fGravity = 25.0f;
			//터진후 벌어지는 X의크기
			float fDrag = 0.999f;
			//터진 후 번지는 y의크기
			float fExplodedGravity = 45.0f;

			lifetime += fElaspsedTime;
			if (lifetime <= fuse)
			{
				//were launching
				vx *= fDrag;
				x += vx * fElaspsedTime;
				y += (vy + fGravity) * fElaspsedTime;

				if(x > m_rc.GetRight())
					x = m_rc.GetRight();
				else if(x < m_rc.GetLeft())
					x = m_rc.GetLeft();

				if(y < m_rc.GetTop())
					y = m_rc.GetTop();
			}
			else
			{
				if (!bHasExploded)
				{
					bHasExploded = true;
					//Explode
					int red = 0;
					int green = 0;

					wxColour col = GetRedGreenColor(_memVal );//wxColour(rand() % 100 + 155, rand() % 240 + 15, rand() % 240 + 15);

					for (int i = 0; i < starCount; i++)
					{
						sParticle s;
						s.x = x;
						s.y = y;
						s.col = col;

						float angle = RandomFloat((float)(2.0f * M_PI));
						float power = RandomFloat(50.0f);

						s.fuse = RandomFloat(3.0f);// + 1.0f;
						s.vx = cosf(angle) * power;
						s.vy = sinf(angle) * power;

						vecStarts.push_back(s);
					}
				}
				else
				{
					for (auto &s : vecStarts)
					{
						s.lifetime += fElaspsedTime;

						s.vx *= fDrag;
						s.x += s.vx * fElaspsedTime;
						float fDisp = (s.vy + fExplodedGravity) * fElaspsedTime;
						s.y += fDisp;
					}
				}
			}
		}

		wxColour GetRedGreenColor(unsigned int val)
		{
			int red = (255 * val) / 100;
			int green = red == 0 ? 255 : 255 - red;

			return wxColour(red, green, 0);
		}


		float RandomFloat(float fMax)
		{
			return ((float)rand() / (float)RAND_MAX) * fMax;
		}

		void DrawSelf(wxDC* pDC)
		{
			wxPen pen(wxColour(255, 255, 255));
			wxBrush brush(ballCol);//wxColour(255, 255, 255));

			pDC->SetPen(pen);
			pDC->SetBrush(brush);
			if(lifetime <= fuse)
				pDC->DrawCircle(x, y, 4);
			else
			{
				bExpired = true;
				for (auto &s : vecStarts)
				{
					if (s.lifetime <= s.fuse)
					{
						bExpired = false;
						wxPen penParticle(wxColour(s.col));
						pDC->SetPen(penParticle);
						pDC->DrawPoint(s.x, s.y);
					}
				}
			}
		}

		std::vector<sParticle> vecStarts;
		int starCount = 0;

		bool bHasExploded = false;
		bool bExpired = false;

		wxRect m_rc;
		unsigned int _memVal = 0;
		wxColour ballCol;
	};

	std::list<cFireworks> listFireworks;
	float fDelay = 1.0f;
	float fElaspsedTime = 0.1f;
	int secondCount = 0;
	int ballCnt = 0;
#endif
private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);

	virtual void UpdateListener(wxCommandEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMMEMVIEW_H_INCLUDED
