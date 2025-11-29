#ifndef NEXTMGRAPHVIEW_H_INCLUDED
#define NEXTMGRAPHVIEW_H_INCLUDED

#include "../utility/MemoryDC.h"
#include "../utility/GraphNode.h"
#include "../system/IUpdateListener.h"

class CNextMGraphView : public wxWindow, public IUpdateListener
{
	unsigned int rulerRGB  = 128;
	unsigned int secondRGB = 128;
	unsigned int timeVal   = 100;
public:
	explicit CNextMGraphView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle = CHILD_WINDOW_STYLE);
	~CNextMGraphView();

public:
	void SetCoreIndex(unsigned int indx) {
		m_coreIndex = indx;
	}

private:
	void Render(wxDC* pDC);
	void DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush);
	void DrawGraph(wxDC* pDC);
	void DrawRuler(wxDC* pDC);

private:
	CMemoryDC m_memDC;
	//배경
	wxColour m_colBackPen;
	wxColour m_colBackBrush;
	//눈금컬러
	wxColour m_colRuler;
	wxColour m_colSecond;
	//전체크기
	wxRect m_viewRect = wxRect(0, 0, 0, 0);
    //그래프 표시 영역
    wxRect m_viewGraphRect = wxRect(0, 0, 0, 0);

	CGLinkedList<unsigned int> nodeList;

	wxFont m_viewFont = wxNullFont;
	bool m_bThreadStop = false;

	unsigned int m_coreIndex = 0;
	unsigned int m_Index = 0;;
	unsigned long m_coreUsage = 0;

private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);

	void UpdateListener(wxCommandEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMGRAPHVIEW_H_INCLUDED
