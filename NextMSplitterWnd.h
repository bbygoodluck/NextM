#ifndef NEXTMSPLITTERWND_H_INCLUDED
#define NEXTMSPLITTERWND_H_INCLUDED

class CNextMSplitterWindow : public wxSplitterWindow
{
public:
	CNextMSplitterWindow(wxWindow* pWindow);
	virtual ~CNextMSplitterWindow();

private:
	// event handlers
	void OnPositionChanged(wxSplitterEvent& event);
	void OnPositionChanging(wxSplitterEvent& event);
	void OnDClick(wxSplitterEvent& event);
	void OnUnsplitEvent(wxSplitterEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMSPLITTERWND_H_INCLUDED
