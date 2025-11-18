#ifndef NEXTMPROCESSVIEW_H_INCLUDED
#define NEXTMPROCESSVIEW_H_INCLUDED

#include <wx/listctrl.h>
#include <atomic>
#include <thread>

class CNextMProcessView : public wxListCtrl
{
	struct sListColumnItem {
		wxString _strColumnName = wxT("");
		unsigned int _width = 0;
		wxListColumnFormat _align = wxLIST_FORMAT_LEFT;
	};

	typedef struct _stCONTEXT_MENU {
		int iId = -1;
		wxString strMenuName = wxT("");
	} CONTEXT_MENU;

public:
	explicit CNextMProcessView(wxWindow* _parent, long style);
	virtual ~CNextMProcessView();

	bool IsAscending()      { return m_bAscending; }
	int  GetSortingColumn() { return m_iSortingColumn; }
	void SetItemIcon(int iItemIndex, int iIconIndex);

#ifdef __WXMSW__
	void AddAndDelProcessEvent(const wxString& type, const wxString& name, long pid, const wxString& cmdline);
#endif // __WXMSW__

protected:
	void InitList();
	void InitColumn();
	static int wxCALLBACK ListCompareFunction(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);

	void CreateContextMenu();
	void SetItemFocus(int _itemIndex);

private:
	wxMenu* m_pContextMenu{nullptr};
	wxBitmap m_bitmapKillProcess;
	wxBitmap m_bitmapDetail;

	bool m_bSorted{false};
	bool m_bSelected{false};
	bool m_bFocus{false};

	int  m_iSelIndex          = -1;
	bool m_bAscending         = true;
	int  m_iSortingColumn     = -1;
	int  m_iSelItemRightClick = -1;
	unsigned long m_ulSelProcessID = 0;

	wxColour m_colBack;

	wxCriticalSection m_critSection;
	bool m_bNewOrDelProcess{false};

	wxTimer m_timer;

	bool m_bEnded{false};
//	std::thread m_thread;
//	std::atomic<bool> m_keepRunning{false};
private:
	void OnTimer(wxTimerEvent& event);
	void OnListContextMenu(wxCommandEvent& event);
	void OnListItemRightClick( wxListEvent& event );
	void OnItemSelected(wxListEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnKillFocus(wxFocusEvent& event);
	wxDECLARE_EVENT_TABLE();

};

#endif // NEXTMPROCESSVIEW_H_INCLUDED
