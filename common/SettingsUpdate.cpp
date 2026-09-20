#include "common.h"
#include "SettingsUpdate.h"
#include "../views/NextMViewManager.h"
#include "../views/NextMAuiNotebook.h"
#include "../views/ViewsPanel.h"
#include "../views/FileListView.h"

std::unique_ptr<CSettingsUpdate> CSettingsUpdate::m_pInstance(nullptr);

CSettingsUpdate* CSettingsUpdate::Get()
{
	if (m_pInstance == nullptr)
		m_pInstance.reset(new CSettingsUpdate());

	return m_pInstance.get();
}

void CSettingsUpdate::UpdateFontForView()
{
    CNextMViewManager* pViewManager = theSplitterManager->GetCurrentViewManager();
    CNextMAuiNotebook* pAuiNotebook = pViewManager->GetAuiNotebook();

    DoUpdateFontForView((wxAuiNotebook *)pAuiNotebook);

    if(theSplitterManager->IsSplit())
    {
        pViewManager = theSplitterManager->GetViewManager2();
        pAuiNotebook = pViewManager->GetAuiNotebook();

        DoUpdateFontForView((wxAuiNotebook *)pAuiNotebook);
    }
}

void CSettingsUpdate::DoUpdateFontForView(wxAuiNotebook* pAuiNotebook)
{
    size_t iPageCount = pAuiNotebook->GetPageCount();
	for(size_t index = 0; index < iPageCount; index++)
	{
		CViewsPanel *pViewsPanel = (CViewsPanel *)pAuiNotebook->GetPage(index);
        CFileListView* pFileListView = (CFileListView *)pViewsPanel->GetFileListView();

        pFileListView->ChangeViewFont();
         wxRect rc = pFileListView->GetClientRect();
        theDCUtil->Refresh(pFileListView, rc);
	}
}
