#ifndef NEXTMVIEWMANAGER_H_INCLUDED
#define NEXTMVIEWMANAGER_H_INCLUDED

#include "FileListView.h"
#include "PathView.h"

class CNextMAuiNotebook;
class CViewsPanel;

class CNextMViewManager : public wxPanel
{
public:
	CNextMViewManager(wxWindow* _parent, bool bLeft = true);
	~CNextMViewManager();

public:
	void AddRemotePage(const wxString& strRemoteAddress);
	void AddTabPage(const wxString& strPath);
	void AddTabPage();
	void SetFocusPage();

	void ChangeDirectoryPath(const wxString& strNewPath, CHANGE_DIR_OPERATION chdirOperation);
	void SendEvent(wxEventType evtType);

	CFileListView* GetFileListView();
	CPathView* GetPathView();
	CViewsPanel* GetCurrentViewsPanels();

#ifdef __WXMSW__
	void MSWDiskDriveInit();
	void MSWUpdateDriveSize(const wxString& strVolume);
#endif // __WXMSW__

	bool IsLeft() { return m_bLeft; }
private:
	void CreateNotebook();
	void Initialize();

private:
	bool m_bLeft = true;
	wxAuiManager m_auiMgr;

	std::unique_ptr<CNextMAuiNotebook> m_pAuiNotebook;

	wxString m_strDefaultPath = wxT("");

private:
	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMVIEWMANAGER_H_INCLUDED
