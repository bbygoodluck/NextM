#ifndef NEXTMMENUEVENT_H_INCLUDED
#define NEXTMMENUEVENT_H_INCLUDED

class CNextMFrame;
class CNextMExecuteMenuOperation;

class CNextMMenuEvent : public wxEvtHandler
{
public:
	CNextMMenuEvent(CNextMFrame* _pMainFrame);
	~CNextMMenuEvent();

	void DoMenuOperation(int nMenuID);
	void InitToolBar(wxAuiToolBar* pToolbar);
	void LoadBookmarkToMenu(bool bFromFavoriteMangaer);
	void SetBookmarkMenuEvent(int nMenuID);

private:
	void Init();

private:
	CNextMFrame* m_pMainFrame;
	CNextMExecuteMenuOperation* m_pMenuOperation;

	std::unordered_map<int, _MENU_EVENT_TYPE> m_mapMenu;
	std::unordered_map<int, wxString> m_mapToolBarMenu;

	wxAuiToolBar* m_pToolBar = nullptr;

private:
	void OnMenuEvent(wxCommandEvent& event);
	void OnMenuEventUpdate(wxUpdateUIEvent& event);
	void OnToolbarEvent(wxCommandEvent& event);
	void OnDropdownBookmark(wxAuiToolBarEvent& event);
	void OnSelectedBookmark(wxCommandEvent& event);
	void OnGoBackForward(wxAuiToolBarEvent& event);
	void OnBackForwardMenuClick(wxCommandEvent & event);

	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMMENUEVENT_H_INCLUDED
