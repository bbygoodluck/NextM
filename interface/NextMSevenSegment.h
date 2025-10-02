#ifndef NEXTMSEVENSEGMENT_H_INCLUDED
#define NEXTMSEVENSEGMENT_H_INCLUDED

class CNextMSevenSegment
{
public:
	CNextMSevenSegment() {}
	~CNextMSevenSegment() {}

public:
	void CreateSevenSegment(const wxRect& rc)
	{
		if(rc.GetWidth() == 0 || rc.GetHeight() == 0)
			return;

		m_rc = rc;
		SEG_SPACE = m_rc.GetHeight() * m_segmentPercent / 100;

		wxRect rcArea{rc};
		wxPoint segA[6]{
			{ rcArea.GetLeft()  + SEG_SPACE / 2 + 4,   rcArea.GetTop() + SEG_SPACE / 2},
			{ rcArea.GetLeft()  + SEG_SPACE + 3,       rcArea.GetTop()},
			{ rcArea.GetRight() - (SEG_SPACE + 2),     rcArea.GetTop()},
			{ rcArea.GetRight() - (SEG_SPACE / 2 + 3), rcArea.GetTop() + SEG_SPACE / 2},
			{ rcArea.GetRight() - (SEG_SPACE + 3),     rcArea.GetTop() + SEG_SPACE},
			{ rcArea.GetLeft()  + SEG_SPACE + 4,       rcArea.GetTop() + SEG_SPACE}
		};

		wxPoint segB[6]{
			{ rcArea.GetRight() - SEG_SPACE / 2, (rcArea.GetTop() + SEG_SPACE / 2) + 4},
			{ rcArea.GetRight(),                 rcArea.GetTop()  + SEG_SPACE + 3},
			{ rcArea.GetRight(),                 rcArea.GetTop()  + rcArea.GetHeight() / 2 - (SEG_SPACE / 2) - 2},
			{ rcArea.GetRight() - SEG_SPACE / 2, rcArea.GetTop()  + rcArea.GetHeight() / 2 - 2},
			{ rcArea.GetRight() - SEG_SPACE,     rcArea.GetTop()  + rcArea.GetHeight() / 2 - (SEG_SPACE / 2) - 3},
			{ rcArea.GetRight() - SEG_SPACE,     rcArea.GetTop()  + SEG_SPACE + 4}
		};

		wxPoint segC[6]{
			{ rcArea.GetRight() - SEG_SPACE / 2, rcArea.GetTop()    + rcArea.GetHeight() / 2 + 2},
			{ rcArea.GetRight(),                 rcArea.GetTop()    + rcArea.GetHeight() / 2 + 2 + SEG_SPACE / 2},
			{ rcArea.GetRight(),                 rcArea.GetBottom() - (SEG_SPACE + 4)},
			{ rcArea.GetRight() - SEG_SPACE / 2, rcArea.GetBottom() - (SEG_SPACE / 2 + 5)},
			{ rcArea.GetRight() - SEG_SPACE,     rcArea.GetBottom() - (SEG_SPACE + 5)},
			{ rcArea.GetRight() - SEG_SPACE,     rcArea.GetTop()    + rcArea.GetHeight() / 2 + 3 + SEG_SPACE / 2}
		};

		wxPoint segD[6]{
			{ rcArea.GetRight() - (SEG_SPACE / 2 + 3), rcArea.GetBottom() - SEG_SPACE / 2},
			{ rcArea.GetRight() - (SEG_SPACE + 2) ,    rcArea.GetBottom()},
			{ rcArea.GetLeft()  + SEG_SPACE + 3,       rcArea.GetBottom()},
			{ rcArea.GetLeft()  + SEG_SPACE / 2 + 4,   rcArea.GetBottom() - SEG_SPACE / 2},
			{ rcArea.GetLeft()  + SEG_SPACE + 4,       rcArea.GetBottom() - SEG_SPACE},
			{ rcArea.GetRight() - (SEG_SPACE + 3),     rcArea.GetBottom() - SEG_SPACE}
		};

		wxPoint segE[6]{
			{ rcArea.GetLeft() + SEG_SPACE / 2, rcArea.GetTop()    + rcArea.GetHeight() / 2 + 2},
			{ rcArea.GetLeft(),                 rcArea.GetTop()    + rcArea.GetHeight() / 2 + 2 + SEG_SPACE / 2},
			{ rcArea.GetLeft(),                 rcArea.GetBottom() - (SEG_SPACE + 4)},
			{ rcArea.GetLeft() + SEG_SPACE / 2, rcArea.GetBottom() - (SEG_SPACE / 2 + 5)},
			{ rcArea.GetLeft() + SEG_SPACE,     rcArea.GetBottom() - (SEG_SPACE + 5)},
			{ rcArea.GetLeft() + SEG_SPACE,     rcArea.GetTop()    + rcArea.GetHeight() / 2 + 3 + SEG_SPACE / 2},
		};

		wxPoint segF[6]{
			{ rcArea.GetLeft() + SEG_SPACE / 2, (rcArea.GetTop() + SEG_SPACE / 2) + 4},
			{ rcArea.GetLeft(),                 rcArea.GetTop()  + SEG_SPACE + 3},
			{ rcArea.GetLeft(),                 rcArea.GetTop()  + rcArea.GetHeight() / 2 - (SEG_SPACE / 2) - 2},
			{ rcArea.GetLeft() + SEG_SPACE / 2, rcArea.GetTop()  + rcArea.GetHeight() / 2 - 2},
			{ rcArea.GetLeft() + SEG_SPACE,     rcArea.GetTop()  + rcArea.GetHeight() / 2 - (SEG_SPACE / 2) - 3},
			{ rcArea.GetLeft() + SEG_SPACE,     rcArea.GetTop()  + SEG_SPACE + 4}
		};

		wxPoint segG[6]{
			{ rcArea.GetLeft()  + SEG_SPACE / 2 + 5,   rcArea.GetTop() + rcArea.GetHeight() / 2},
			{ rcArea.GetLeft()  + SEG_SPACE + 4,       rcArea.GetTop() + rcArea.GetHeight() / 2 - SEG_SPACE / 2 },
			{ rcArea.GetRight() - (SEG_SPACE + 4),     rcArea.GetTop() + rcArea.GetHeight() / 2 - SEG_SPACE / 2 },
			{ rcArea.GetRight() - (SEG_SPACE / 2 + 5), rcArea.GetTop() + rcArea.GetHeight() / 2},
			{ rcArea.GetRight() - (SEG_SPACE + 4),     rcArea.GetTop() + rcArea.GetHeight() / 2 + SEG_SPACE / 2},
			{ rcArea.GetLeft()  + SEG_SPACE + 4,       rcArea.GetTop() + rcArea.GetHeight() / 2 + SEG_SPACE / 2 }
		};

		std::copy(std::begin(segA), std::end(segA), std::begin(m_segA));
		std::copy(std::begin(segB), std::end(segB), std::begin(m_segB));
		std::copy(std::begin(segC), std::end(segC), std::begin(m_segC));
		std::copy(std::begin(segD), std::end(segD), std::begin(m_segD));
		std::copy(std::begin(segE), std::end(segE), std::begin(m_segE));
		std::copy(std::begin(segF), std::end(segF), std::begin(m_segF));
		std::copy(std::begin(segG), std::end(segG), std::begin(m_segG));
	}

	void DrawSeventSegment(wxDC* pDC, int value, const wxColour col)
	{
		const bool digits[10][7] = {
            {1, 1, 1, 1, 1, 1, 0}, // 0
            {0, 1, 1, 0, 0, 0, 0}, // 1
            {1, 1, 0, 1, 1, 0, 1}, // 2
            {1, 1, 1, 1, 0, 0, 1}, // 3
            {0, 1, 1, 0, 0, 1, 1}, // 4
            {1, 0, 1, 1, 0, 1, 1}, // 5
            {1, 0, 1, 1, 1, 1, 1}, // 6
            {1, 1, 1, 0, 0, 0, 0}, // 7
            {1, 1, 1, 1, 1, 1, 1}, // 8
            {1, 1, 1, 1, 0, 1, 1}  // 9
        };

        const wxPoint* segments[7] = {m_segA, m_segB, m_segC, m_segD, m_segE, m_segF, m_segG};

		for (int i = 0; i < 7; i++)
			Draw(pDC, segments[i], digits[value][i], col);
	}

	void Draw(wxDC* pDC, const wxPoint pts[], bool on, const wxColour& col)
    {
		pDC->SetPen(*wxTRANSPARENT_PEN);
        pDC->SetBrush(on ? col : wxColour(50, 50, 50));
		pDC->DrawPolygon(6, pts);
    }

private:
	int SEG_SPACE = 9;
	const unsigned int m_segmentPercent = 9;
	wxRect m_rc{0, 0, 0, 0};

	wxPoint m_segA[6]{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	wxPoint m_segB[6]{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	wxPoint m_segC[6]{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	wxPoint m_segD[6]{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	wxPoint m_segE[6]{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	wxPoint m_segF[6]{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	wxPoint m_segG[6]{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
};

#endif // NEXTMSEVENSEGMENT_H_INCLUDED
