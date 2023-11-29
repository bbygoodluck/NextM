#ifndef _CONTEXT_MENU_HANDLER_H_
#define _CONTEXT_MENU_HANDLER_H_

class CContextMenuHandlerBase : public wxEvtHandler
{
public:
	CContextMenuHandlerBase() {}
	virtual ~CContextMenuHandlerBase() {}

public:
	virtual void SetObject(wxArrayString& strArray) = 0;
	virtual void ShowContextMenu(wxWindow* pWnd, const wxPoint& pt) = 0;
	virtual void Clear() = 0;

protected:
	wxMenu* m_pMenu;
};

#endif
