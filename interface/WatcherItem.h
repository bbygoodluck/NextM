#ifndef WATCHERITEM_H_INCLUDED
#define WATCHERITEM_H_INCLUDED

class CWatcherItem
{
public:
	CWatcherItem() {}
	~CWatcherItem() {}

public:
	CWatcherItem* Clone()
	{
		CWatcherItem* newItem = new CWatcherItem();
		newItem->m_strOld = m_strOld;
		newItem->m_strNew = m_strNew;
		newItem->m_Action = m_Action;

		return newItem;
	}

public:
	wxString m_strPath = wxT("");
	wxString m_strNew  = wxT("");
	wxString m_strOld  = wxT("");
	int m_Action;
};

#endif // WATCHERITEM_H_INCLUDED
