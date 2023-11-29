#ifndef __MY_TOOLTIP_VIEW_H_INCLUDED
#define __MY_TOOLTIP_VIEW_H_INCLUDED

#include "../utility/MemoryDC.h"
class CNextMTooltipView : public wxWindow
{
public:
	CNextMTooltipView(wxWindow* pParent);
	~CNextMTooltipView();

public:
	void SetTooltipText(const wxString& strTooltip);

private:
	void Render(wxDC* pDC);

private:
	CMemoryDC m_memDC;
	//화면 영역 크기
	wxRect m_viewRect = wxRect(0, 0, 0, 0);
	wxFont m_toolTipFont = wxNullFont;

	wxString m_strTooltip = wxT("");
private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif
