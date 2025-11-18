#ifndef NEXTMCPUSEGVIEW_H_INCLUDED
#define NEXTMCPUSEGVIEW_H_INCLUDED

#include "../utility/MemoryDC.h"
#include "../system/IUpdateListener.h"
#include "../interface/NextMSevenSegment.h"

class CNextMCPUSegView : public wxWindow, public IUpdateListener
{
public:
	explicit CNextMCPUSegView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle = CHILD_WINDOW_STYLE);
	~CNextMCPUSegView();

//	void SetIndex(int index) { m_CPUIndex = index; }

private:
	void Render(wxDC* pDC);
	void SeperateDigit(int val);
	void CreateSegmentArea(const wxRect& rc, int segCount);

private:
	CMemoryDC m_memDC;
	//전체크기
	wxRect m_viewRect = wxRect(0, 0, 0, 0);

	// 세그먼트별 스페이스(Default)
    int m_hunders = 0;
    int m_tens    = 0;
    int m_ones    = 0;

	std::vector<CNextMSevenSegment> vecRects;

	unsigned int m_segCount = 3;
	unsigned long m_ulUsage = 0;

private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);

	void UpdateListener(wxCommandEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMCPUSEGVIEW_H_INCLUDED
