#ifndef NEXTMSPLITTERMANAGER_H_INCLUDED
#define NEXTMSPLITTERMANAGER_H_INCLUDED

#include "NextMSplitterWnd.h"
#include "./views/NextMViewManager.h"

class CNextMSplitterManager// : public wxEvtHandler
{
private:
	explicit CNextMSplitterManager() {};
    ~CNextMSplitterManager();

public:
	static CNextMSplitterManager *Get();

	CNextMSplitterManager* GetObject() {
		return this;
	}

public:
	CNextMSplitterWindow* GetSplitterWnd()
    {
        if(m_pSpliiterWnd)
            return m_pSpliiterWnd.get();

        return nullptr;
    }

	void SplitWindow(const wxSize& _size = wxSize(0,0));
	void SetSashGravity(double gravity);

	CNextMViewManager* GetCurrentViewManager() {
		return m_pCurrentViewManager;
	}

	CNextMViewManager* GetViewManager2() {
		return m_pCurrentViewManager == m_pLeftViewManager.get() ? m_pRightViewManager.get() :
			                                                       m_pLeftViewManager.get();
	}

	void SetCurrentActive();
	//Tab키 입력시 포커스 이동
	void ChangeSplitView();
	//디스크 드라이브 추가/삭제시
#ifdef __WXMSW__
	void MSWDiskDriveInit();
	void MSWUpdateDriveSize(const wxString& strVolume);

#endif // __WXMSW__

private:
	void SetWindowArrange(int nSplitStyle, const wxSize& size);
	void Init();

private:
	bool m_Initialized = false;
	static CNextMSplitterManager* m_pSplitterInstance;

	std::unique_ptr<CNextMSplitterWindow> m_pSpliiterWnd = nullptr;
	wxFrame* m_pParent = nullptr;

	CNextMViewManager* m_pCurrentViewManager = nullptr;
	std::unique_ptr<CNextMViewManager> m_pLeftViewManager = nullptr;
	std::unique_ptr<CNextMViewManager> m_pRightViewManager = nullptr;


private:
//	wxDECLARE_EVENT_TABLE();

};

#endif // NEXTMSPLITTERMANAGER_H_INCLUDED
