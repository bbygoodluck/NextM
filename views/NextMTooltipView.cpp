#include "../ginc.h"
#include "NextMToolTipView.h"

wxBEGIN_EVENT_TABLE(CNextMTooltipView, wxWindow)
	EVT_PAINT(CNextMTooltipView::OnPaint)
	EVT_ERASE_BACKGROUND(CNextMTooltipView::OnErase)
	EVT_SIZE(CNextMTooltipView::OnSize)
wxEND_EVENT_TABLE()

CNextMTooltipView::CNextMTooltipView(wxWindow* pParent)
	: wxWindow(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, CHILD_WINDOW_STYLE)
{
#ifdef __WXMSW__
	m_toolTipFont.Create(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Tahoma"));
#else
	m_toolTipFont = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
#endif
}

CNextMTooltipView::~CNextMTooltipView()
{

}

void CNextMTooltipView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

	m_viewRect = GetClientRect();
	wxMemoryDC* pMemDC = m_memDC.CreateMemoryDC(&dc, m_viewRect, wxColour(252, 242, 156), wxColour(252, 242, 156));
	pMemDC->SetFont(m_toolTipFont);

	Render(pMemDC);
	dc.Blit(0, 0, m_viewRect.GetWidth(), m_viewRect.GetHeight(), pMemDC, 0, 0);
}

void CNextMTooltipView::Render(wxDC* pDC)
{
	pDC->SetTextBackground(wxColour(0, 0, 0));
	pDC->DrawLabel(m_strTooltip, m_viewRect, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
}

void CNextMTooltipView::OnErase(wxEraseEvent& event)
{

}

void CNextMTooltipView::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

	theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMTooltipView::SetTooltipText(const wxString& strTooltip)
{
	m_strTooltip = strTooltip;
}
