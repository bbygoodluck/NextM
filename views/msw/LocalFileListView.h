#ifndef LOCALFILELISTVIEW_H_INCLUDED
#define LOCALFILELISTVIEW_H_INCLUDED

#include "../FileListView.h"
#include "../IWatchViewEventListener.h"
#include "../../engine/msw/LocalFileSystemWatcher.h"

class CLocalFileListView : public CFileListView, public IWatchViewEventListener
{
public:
	CLocalFileListView(wxWindow* parent, const int nID, const wxSize& sz);
	virtual ~CLocalFileListView();

public:
	bool LoadDirectory(const wxString& strPath) override;
	void Render(wxDC* pDC) override;
    void ApplyChangedViewSize() override;
	void GotoTopDir() override;

	void InsertDiskDriveItems();
	void DeleteDiskDriveItems();

private:
	std::unique_ptr<CLocalFileSystemWatcher> m_pFileSystemWatcher = nullptr;

private:
	void ResetDiskDriveItems();

	void SetDiskSpace(const wxString& strVolume);
	void SetDiskSpace(std::vector<CNextMDirData>::iterator it) override;

	void DoCreate(CWatcherItem* pItem) override;
	void DoModify(CWatcherItem* pItem) override;
	void DoDelete(CWatcherItem* pItem) override;
	void DoRename(CWatcherItem* pItem) override;

private:
	bool m_bWatcherModify = false;
	int m_iDriveItemIndex = 0;

private:
	void OnDiskDriveInit(wxCommandEvent& event);
	void OnUpdateDriveSizeInfo(wxCommandEvent& event);
	void OnFileSystemWatcher(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};


#endif // LOCALFILELISTVIEW_H_INCLUDED
