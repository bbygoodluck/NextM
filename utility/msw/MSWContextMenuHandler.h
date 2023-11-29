#ifndef __MSW_CONTEXT_MENU_HANDLER_H__
#define __MSW_CONTEXT_MENU_HANDLER_H__

#include "../ContextMenuHandler.h"

class CMSWContextMenuHandler : public CContextMenuHandlerBase
{
public:
	CMSWContextMenuHandler();
	virtual ~CMSWContextMenuHandler();

public:
	void SetObject(wxArrayString& strArray);
	void ShowContextMenu(wxWindow* pWnd, const wxPoint& pt);
	void Clear();

private:
	bool GetContextMenu(void ** ppContextMenu, int& iMenuType);

private:
	IShellFolder* m_psfFolder;
	LPCONTEXTMENU m_pContextMenu;
	std::vector<LPCITEMIDLIST> m_items;
	int	m_iItem;


private:
	void OnInvokeCommand(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};
typedef CMSWContextMenuHandler CContextMenuHandler;
#endif
