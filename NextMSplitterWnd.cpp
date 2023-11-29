#include "ginc.h"
#include "NextMSplitterWnd.h"

wxBEGIN_EVENT_TABLE(CNextMSplitterWindow, wxSplitterWindow)
	EVT_SPLITTER_SASH_POS_CHANGED(wxID_ANY, CNextMSplitterWindow::OnPositionChanged)
	EVT_SPLITTER_SASH_POS_CHANGING(wxID_ANY, CNextMSplitterWindow::OnPositionChanging)
	EVT_SPLITTER_DCLICK(wxID_ANY, CNextMSplitterWindow::OnDClick)
	EVT_SPLITTER_UNSPLIT(wxID_ANY, CNextMSplitterWindow::OnUnsplitEvent)
wxEND_EVENT_TABLE()


CNextMSplitterWindow::CNextMSplitterWindow(wxWindow* pWindow)
    : wxSplitterWindow(pWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, /*wxSP_3D*/ wxSP_NOBORDER | wxSP_LIVE_UPDATE | wxNO_BORDER)// | wxCLIP_CHILDREN | wxCLIP_SIBLINGS | wxSP_NO_XP_THEME)
{

}

CNextMSplitterWindow::~CNextMSplitterWindow()
{

}

void CNextMSplitterWindow::OnPositionChanged(wxSplitterEvent& event)
{
	event.Skip();
}

void CNextMSplitterWindow::OnPositionChanging(wxSplitterEvent& event)
{
	event.Skip();
}

void CNextMSplitterWindow::OnDClick(wxSplitterEvent& event)
{
	event.Skip();
}

void CNextMSplitterWindow::OnUnsplitEvent(wxSplitterEvent& event)
{
	event.Skip();
}
