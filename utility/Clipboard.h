#ifndef CLIPBOARD_H_INCLUDED
#define CLIPBOARD_H_INCLUDED

class CClipboardBase
{
public:
	explicit CClipboardBase() {}
	~CClipboardBase() {}

public:
	virtual void AddData(const wxString& strData) = 0;
	virtual void AddData(const std::list<wxString>& lstDatas) = 0;
	virtual void GetData(std::list<wxString>& lstDatas) = 0;

	virtual void CopyToClipboard() = 0;

	void SetFileOperation(_MENU_EVENT_TYPE _menuType) { m_menuType = _menuType; }
	_MENU_EVENT_TYPE GetFileOperation() const { return m_menuType; }

private:
	_MENU_EVENT_TYPE m_menuType = _MENU_EVENT_NONE;
};

#endif // CLIPBOARD_H_INCLUDED
