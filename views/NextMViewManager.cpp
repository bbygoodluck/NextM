#include "../ginc.h"
#include "NextMViewManager.h"
#include "NextMAuiNotebook.h"
#include "ViewsPanel.h"

wxBEGIN_EVENT_TABLE(CNextMViewManager, wxPanel)
//	EVT_MY_CUSTOM_COMMAND(wxEVT_DISPLAY_DIR, wxID_ANY, CNextMViewManager::OnDisplayDir)
wxEND_EVENT_TABLE()

CNextMViewManager::CNextMViewManager(wxWindow* _parent, bool bLeft)
	: wxPanel(_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxCLIP_CHILDREN | wxCLIP_SIBLINGS)// | wxNO_FULL_REPAINT_ON_RESIZE)
	, m_bLeft(bLeft)
	, m_pAuiNotebook(nullptr)
{
	CreateNotebook();
	Initialize();

	m_auiMgr.SetManagedWindow(this);
	m_auiMgr.AddPane(m_pAuiNotebook.get(), wxAuiPaneInfo().CenterPane().PaneBorder(false));
	m_auiMgr.Update();
}

CNextMViewManager::~CNextMViewManager()
{

}

void CNextMViewManager::CreateNotebook()
{
	if(m_pAuiNotebook != nullptr)
		return;

	m_pAuiNotebook = std::make_unique<CNextMAuiNotebook>(this, theConfig->GetTabStyle());
}

void CNextMViewManager::Initialize()
{
	m_strDefaultPath = m_bLeft ? theConfig->GetLeftPath() : theConfig->GetRightPath();

	if(!m_bLeft)
	{
		if( !wxFileName::DirExists(m_strDefaultPath) )
			m_bLeft = wxT("C:\\");
	}

	AddTabPage(m_strDefaultPath);
}

void CNextMViewManager::AddTabPage()
{
	AddTabPage(m_strDefaultPath);
}

void CNextMViewManager::AddTabPage(const wxString& strPath)
{
	m_pAuiNotebook->AddTabPage(strPath);

	ChangeDirectoryPath(strPath, CHANGE_DIR_FILELISTIVEW);
	ChangeDirectoryPath(strPath, CHANGE_DIR_TAB);
	ChangeDirectoryPath(strPath, CHANGE_DIR_PATHVIEW);
}

void CNextMViewManager::AddRemotePage(const wxString& strRemoteAddress)
{
	m_pAuiNotebook->AddTabPage(strRemoteAddress, true);
}

void CNextMViewManager::SetFocusPage()
{
	CViewsPanel *pViewsPanel = GetCurrentViewsPanels();
	if(pViewsPanel == nullptr)
		return;

	pViewsPanel->GetFileListView()->SetFocus();
}

CViewsPanel* CNextMViewManager::GetCurrentViewsPanels()
{
	if(m_pAuiNotebook == nullptr)
		return nullptr;

	CViewsPanel *pViewsPanel = (CViewsPanel *)m_pAuiNotebook->GetCurrentPage();
	return pViewsPanel;
}

CFileListView* CNextMViewManager::GetFileListView()
{
	CViewsPanel* pViewsPanel = GetCurrentViewsPanels();
	if(pViewsPanel == nullptr)
		return nullptr;

	return pViewsPanel->GetFileListView();
}

CPathView* CNextMViewManager::GetPathView()
{
	CViewsPanel* pViewsPanel = GetCurrentViewsPanels();
	if(pViewsPanel == nullptr)
		return nullptr;

	return pViewsPanel->GetPathView();
}

void CNextMViewManager::ChangeDirectoryPath(const wxString& strNewPath, CHANGE_DIR_OPERATION chdirOperation)
{
	switch(chdirOperation)
	{
		case CHANGE_DIR_TAB:
			m_pAuiNotebook->SetTabNameChange(strNewPath);
			break;

		case CHANGE_DIR_PATHVIEW:
			GetPathView()->AddPath(strNewPath);
			break;

		case CHANGE_DIR_FILELISTIVEW:
			GetFileListView()->LoadDirectory(strNewPath);
			break;

		default:
			break;

	}
}

void CNextMViewManager::SendEvent(wxEventType evtType)
{
	wxCommandEvent evt(evtType);
	wxPostEvent(GetFileListView(), evt);
}

#ifdef __WXMSW__
void CNextMViewManager::MSWDiskDriveInit()
{
	size_t iPageCount = m_pAuiNotebook->GetPageCount();
	for(size_t index = 0; index < iPageCount; index++)
	{
		CViewsPanel *pViewsPanel = (CViewsPanel *)m_pAuiNotebook->GetPage(index);

		wxCommandEvent evt(wxEVT_DISK_DRIVE_INIT);
		wxPostEvent(pViewsPanel->GetFileListView(), evt);
	}
}

void CNextMViewManager::MSWUpdateDriveSize(const wxString& strVolume)
{
	size_t iPageCount = m_pAuiNotebook->GetPageCount();
	for(size_t index = 0; index < iPageCount; index++)
	{
		CViewsPanel *pViewsPanel = (CViewsPanel *)m_pAuiNotebook->GetPage(index);

		wxCommandEvent evt(wxEVT_UPDATE_DRIVE_SIZEINFO);
		evt.SetString(strVolume);

		wxPostEvent(pViewsPanel->GetFileListView(), evt);
	}
}
#endif // __WXMSW__
