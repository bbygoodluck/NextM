#include "../ginc.h"
#include "FileSystemWatcherListener.h"
#include "../interface/WatcherItem.h"

CFileSystemWatcherListener::CFileSystemWatcherListener()
{

}

CFileSystemWatcherListener::CFileSystemWatcherListener(wxWindow* pOwner)
	: m_pOwner(pOwner)
{

}

CFileSystemWatcherListener::~CFileSystemWatcherListener()
{

}

void CFileSystemWatcherListener::OnFileChange(const wxString& strName)
{
	CWatcherItem item;

	item.m_strPath = GetWatchPath();
	item.m_strOld = wxT("");
	item.m_strNew = strName;
	item.m_Action = FS_WATCHER_MODIFY;

	wxCommandEvent evt(wxEVT_FS_WATCHER);
	evt.SetClientData(item.Clone());
	wxQueueEvent(m_pOwner, evt.Clone());
}

void CFileSystemWatcherListener::OnFileAdded(const wxString& strName)
{
	CWatcherItem item;

	item.m_strPath = GetWatchPath();
	item.m_strOld = wxT("");
	item.m_strNew = strName;
	item.m_Action = FS_WATCHER_CREATE;

	wxCommandEvent evt(wxEVT_FS_WATCHER);
	evt.SetClientData(item.Clone());
	wxQueueEvent(m_pOwner, evt.Clone());
}

void CFileSystemWatcherListener::OnFileRemoved(const wxString& strName)
{
	CWatcherItem item;

	item.m_strPath = GetWatchPath();
	item.m_strOld = wxT("");
	item.m_strNew = strName;
	item.m_Action = FS_WATCHER_DELETE;

	wxCommandEvent evt(wxEVT_FS_WATCHER);
	evt.SetClientData(item.Clone());
	wxQueueEvent(m_pOwner, evt.Clone());
}

void CFileSystemWatcherListener::OnFileRenamed(const wxString& strOldName, const wxString& strNewName)
{
	CWatcherItem item;

	item.m_strPath = GetWatchPath();
	item.m_strOld = strOldName;
	item.m_strNew = strNewName;
	item.m_Action = FS_WATCHER_RENAME;

	wxCommandEvent evt(wxEVT_FS_WATCHER);
	evt.SetClientData(item.Clone());
	wxQueueEvent(m_pOwner, evt.Clone());
}
