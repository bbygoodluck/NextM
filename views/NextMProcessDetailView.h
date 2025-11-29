#ifndef NEXTMPROCESSDETAILVIEW_H_INCLUDED
#define NEXTMPROCESSDETAILVIEW_H_INCLUDED

#include "../utility/MemoryDC.h"
#include "../utility/GraphNode.h"

class CNextMProcessDetailView : public wxFrame
{
public:
	explicit CNextMProcessDetailView(wxWindow* parent, const int nID, const wxPoint& pt, const wxSize& sz, long lStyle = CHILD_WINDOW_STYLE);
	~CNextMProcessDetailView();

	void SetWindowAlpha(BYTE alpha);
	void SetProcessId(const wxString& strProcessId);

	void Start(int millisecond) {
		m_timer.Start(millisecond);
	}

	void Stop() {
		m_timer.Stop();
	}

	void Clear();

private:
	void Render(wxDC* pDC);
	void DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush);
	void DrawData(wxDC* pDC);
	void UpdateData();

private:
	//배경
	wxColour m_colBackPen;
	wxColour m_colBackBrush;

	CMemoryDC m_memDC;

	//전체크기
	wxRect m_viewRect{0, 0, 0, 0};

	//CPU Graph
	wxRect m_viewCPURect{0, 0, 0, 0};

	// Memory Area
	wxRect m_viewMemTextRect{0, 0, 0, 0};
	wxRect m_viewMemRect{0, 0, 0, 0};

	//Command Area
	wxRect m_viewCommandRect{0, 0, 0, 0};

	wxString _strProcessId{wxT("")};
	unsigned long _ulProcessId{0};
	Proc::_PROCESS_INFO* _pPsInfo = nullptr;

	wxTimer m_timer;
	wxFont m_font;

	CGLinkedList<unsigned int> nodeList;

private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMPROCESSDETAILVIEW_H_INCLUDED
