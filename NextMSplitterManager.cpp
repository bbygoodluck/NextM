#include "ginc.h"
#include "NextMSplitterManager.h"

/*
wxBEGIN_EVENT_TABLE(CNextMSplitterManager, wxEvtHandler)
wxEND_EVENT_TABLE()
*/
CNextMSplitterManager* CNextMSplitterManager::m_pSplitterInstance = nullptr;

CNextMSplitterManager* CNextMSplitterManager::Get()
{
	if (m_pSplitterInstance == nullptr)
	{
        m_pSplitterInstance = new CNextMSplitterManager();
        m_pSplitterInstance->Init();
	}

	return m_pSplitterInstance;
}

CNextMSplitterManager::~CNextMSplitterManager()
{
}

void CNextMSplitterManager::Init()
{
	m_pParent = (wxFrame *)((wxTopLevelWindow*)wxTheApp->GetTopWindow());

    m_pSpliiterWnd = std::make_unique<CNextMSplitterWindow>(m_pParent);

    m_pLeftViewManager = std::make_unique<CNextMViewManager>(m_pSpliiterWnd.get());
	m_pCurrentViewManager = m_pLeftViewManager.get();
}

void CNextMSplitterManager::SetSashGravity(double gravity)
{
    if(m_pSpliiterWnd == nullptr)
        Init();

    m_pSpliiterWnd->SetSashGravity(gravity);
}

void CNextMSplitterManager::SplitWindow(const wxSize& _size)
{
	if(m_pSpliiterWnd == nullptr)
		return;

//	m_pSpliiterWnd->SetSize(_size);

	int iSplitStyle = theConfig->GetSplitStyle();

	if(iSplitStyle == WINDOW_SPLIT_NONE)
	{
		if (m_pSpliiterWnd->IsSplit())
		{
			bool bUnSplit = m_pSpliiterWnd->Unsplit(m_pRightViewManager.get());
			if(!bUnSplit)
			{
				CNextMViewManager* pRightViewManager = m_pRightViewManager.release();
				if(pRightViewManager)
				{
					wxDELETE(pRightViewManager);
					m_pRightViewManager = nullptr;
				}
			}
		}

		m_pSpliiterWnd->Initialize(m_pLeftViewManager.get());
		m_pCurrentViewManager = m_pLeftViewManager.get();
	}
	else
	{
		if(m_pRightViewManager == nullptr)
			m_pRightViewManager = std::make_unique<CNextMViewManager>(m_pSpliiterWnd.get(), false);

		SetWindowArrange(iSplitStyle, _size);
	}

    m_Initialized = true;
}

void CNextMSplitterManager::SetWindowArrange(int nSplitStyle, const wxSize& size)
{
	int iCenterWidth = size.GetWidth() / 2;
	int iCenterHeight = size.GetHeight() / 2;

	if (m_pSpliiterWnd->IsSplit())
		m_pSpliiterWnd->Unsplit();

	m_pLeftViewManager->Show(true);
	m_pRightViewManager->Show(true);

	if (nSplitStyle == WINDOW_SPLIT_HORZ)
	{
		m_pSpliiterWnd->SetMinimumPaneSize(iCenterHeight);
		m_pSpliiterWnd->SplitHorizontally(m_pLeftViewManager.get(), m_pRightViewManager.get());
	}
	else
	{
		m_pSpliiterWnd->SetMinimumPaneSize(iCenterWidth);
		m_pSpliiterWnd->SplitVertically(m_pLeftViewManager.get(), m_pRightViewManager.get());
	}
}

void CNextMSplitterManager::SetCurrentActive()
{
	if(m_pCurrentViewManager == nullptr)
		return;

	m_pCurrentViewManager->SetFocusPage();
}

void CNextMSplitterManager::ChangeSplitView()
{
	int iSplitStyle = theConfig->GetSplitStyle();
	if(iSplitStyle != WINDOW_SPLIT_NONE)
	{
		m_pCurrentViewManager = m_pCurrentViewManager == m_pLeftViewManager.get() ? m_pRightViewManager.get() : m_pLeftViewManager.get();
		SetCurrentActive();
	}
}

#ifdef __WXMSW__
void CNextMSplitterManager::MSWDiskDriveInit()
{
	m_pLeftViewManager->MSWDiskDriveInit();
	int iSplitStyle = theConfig->GetSplitStyle();
	if(iSplitStyle != WINDOW_SPLIT_NONE)
		m_pRightViewManager->MSWDiskDriveInit();
}

void CNextMSplitterManager::MSWUpdateDriveSize(const wxString& strVolume)
{
	m_pLeftViewManager->MSWUpdateDriveSize(strVolume);
	int iSplitStyle = theConfig->GetSplitStyle();
	if(iSplitStyle != WINDOW_SPLIT_NONE)
		m_pRightViewManager->MSWUpdateDriveSize(strVolume);
}
#endif // __WXMSW__
