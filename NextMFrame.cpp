#include "ginc.h"
#include "NextMFrame.h"
#include "NextMSplitterWnd.h"
#include "NextMStatusBar.h"
#include "NextMNewStatusBar.h"

#ifdef __WXMSW__
	static const GUID usb_hid = { 0xA5DCBF10L, 0x6530, 0x11D2, {0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED} };
#endif

wxBEGIN_EVENT_TABLE(CNextMFrame, wxFrame)
	EVT_SIZE(CNextMFrame::OnSize)
	EVT_CLOSE(CNextMFrame::OnClose)
	EVT_MAXIMIZE(CNextMFrame::OnMaximized)
	EVT_ICONIZE(CNextMFrame::OnIconized)
wxEND_EVENT_TABLE()


CNextMFrame::CNextMFrame(wxFrame *frame, const wxString& strTitle)
	: wxFrame(frame, -1, strTitle, wxPoint(theConfig->GetXPos(), theConfig->GetYPos()), wxSize(theConfig->GetWidth(), theConfig->GetHeight()))
{
    wxInitAllImageHandlers();
	wxMenuBar* pMenuBar = wxXmlResource::Get()->LoadMenuBar(this, wxT("NextMMenu"));

	if (pMenuBar == nullptr)
	{
		wxMessageBox(wxT("Failed to load MenuBar resource"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return;
	}
	//메뉴바 등록
	SetMenuBar(pMenuBar);
	//상태바등록
//	m_pStatusBar = std::make_unique<CNextMStatusBar>(this);
//	SetStatusBar(m_pStatusBar.get());

	m_pStatusBarNew = std::make_unique<CNextMNewStatusBar>(this, wxID_ANY);
//	m_pStatusBarNew->Create(this);


	Initialize();
	CreateToolBar();

	theSplitterManager->SetSashGravity(1.0);

	m_mgr.SetManagedWindow(this);
	m_mgr.AddPane(theSplitterManager->GetSplitterWnd(), wxAuiPaneInfo().CenterPane().PaneBorder(false));
	m_mgr.AddPane(m_pStatusBarNew.get(), wxAuiPaneInfo().CenterPane().Bottom().DockFixed().CaptionVisible(false).CloseButton(false).PaneBorder(false));
	m_mgr.Update();

	m_bInitialized = true;

#ifdef __WXMSW__
	//RegisterDeviceNotification()함수를 이용해서 usb 포트를 감시할 경우 주의해야 할 사항
	//    - DEV_BROADCAST_DEVICEINTERFACE 구조체의 dbcc_devicetype값을 DBT_DEVTYP_DEVICEINTERFACE로 설정할 경우
	//      usb포트에 장치가 연결될때마다 호출이 되므로 usb카드삽입시 새로운 드라이브가 생성되었을때만 이벤트를 발생시킬 경우는
	//      DBT_DEVTYP_VOLUME 값을 셋팅해야 정상적으로 usb카드삽입후 실제로 usb 드라이브가 생성된 이후에 이벤트가 발생한다.
	//      DBT_DEVTYP_DEVICEINTERFACE로 셋팅하게 되면 usb포트에 장치가 삽입되자마자 이벤트가 발생하게 됨
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_VOLUME;// DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = usb_hid;

	m_hNotifyDev = ::RegisterDeviceNotification(this->GetHWND(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
#endif

	if (theConfig->IsMaximized())
		this->Maximize();

	if(theConfig->IsCenterOnScreen())
		CenterOnScreen();

	theSplitterManager->SetCurrentActive();
}

CNextMFrame::~CNextMFrame()
{

}

void CNextMFrame::Initialize()
{
	int iMonitors = wxDisplay::GetCount();

	int nCurrDisp = wxDisplay::GetFromWindow(this);
	if (nCurrDisp < 0)
		nCurrDisp = 0;

	wxDisplay disp(nCurrDisp);

	if (iMonitors == 1)
	{	//모니터가 하나일경우 기존 모니터 인덱스와 다를 경우는 Default로 셋팅
		int iMonitorPos = theConfig->GetMonitorPos();
		if (nCurrDisp != iMonitorPos)
		{
			wxRect rcDefault = theConfig->GetDefaultRect();

			theConfig->SetChangeGlobalVal(wxT("DispMonitor"), 0);
			theConfig->SetChangeGlobalVal(wxT("XPos"), rcDefault.GetLeft());
			theConfig->SetChangeGlobalVal(wxT("YPos"), rcDefault.GetRight());
			theConfig->SetChangeGlobalVal(wxT("Width"), rcDefault.GetWidth());
			theConfig->SetChangeGlobalVal(wxT("Height"), rcDefault.GetHeight());
		}
	}

	//메뉴이벤트 처리 생성
	_gMenuEvent = new CNextMMenuEvent(this);
	PushEventHandler(_gMenuEvent);

	_gMenuEvent->LoadBookmarkToMenu(false);
}

void CNextMFrame::OnClose(wxCloseEvent& event)
{
	m_mgr.UnInit();

	PopEventHandler(true);
	Destroy();
}

void CNextMFrame::OnSize(wxSizeEvent& event)
{
	if (!this->IsMaximized())
	{	//최대화에서 원래상태로 복원되었을 경우
		if(m_bMaximize)
		{
			int nCurrDisp = wxDisplay::GetFromWindow(this);
			if (nCurrDisp < 0)
				nCurrDisp = 0;

			wxDisplay disp(nCurrDisp);

			theConfig->SetChangeGlobalVal(wxT("Maximized"), 0);
			if (theConfig->IsCenterOnScreen())
				CenterOnScreen();
		}

		m_bMaximize = false;
	}

	wxSize szFrame = event.GetSize();

	theConfig->SetChangeGlobalVal(wxT("Width"), szFrame.GetWidth());
	theConfig->SetChangeGlobalVal(wxT("Height"), szFrame.GetHeight());

	theSplitterManager->SplitWindow(szFrame);
}

void CNextMFrame::OnMaximized(wxMaximizeEvent& event)
{
	m_bMaximize = true;
	theConfig->SetChangeGlobalVal(wxT("Maximized"), 1);
}

void CNextMFrame::OnIconized(wxIconizeEvent& event)
{

}

void CNextMFrame::CreateToolBar()
{
	m_pAuiToolbar = std::make_unique<wxAuiToolBar>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORZ_TEXT);

	_gMenuEvent->InitToolBar(m_pAuiToolbar.get());

	m_mgr.AddPane(m_pAuiToolbar.get(), wxAuiPaneInfo().Name("Toolbar").Caption("Toolbar").ToolbarPane().Top());
}

#ifdef __WXMSW__
WXLRESULT CNextMFrame::MSWDefWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
	WXLRESULT rc = ::DefWindowProc(this->GetHWND(), nMsg, wParam, lParam);

	switch(nMsg)
	{
		case WM_DEVICECHANGE:
			OnDeviceChange(wParam, lParam);
			break;

		case WM_SETFOCUS:
			theSplitterManager->SetCurrentActive();
			break;

		default:
			break;
	}

	return rc;
}

void CNextMFrame::OnDeviceChange(WXWPARAM nEventType, WXLPARAM dwData)
{
	if ((nEventType != DBT_DEVICEARRIVAL) && (nEventType != DBT_DEVICEREMOVECOMPLETE))
		return;

	PDEV_BROADCAST_HDR pdbh = reinterpret_cast<DEV_BROADCAST_HDR *>(dwData);
	DWORD dbhSize = pdbh->dbch_size;

	if(dbhSize == 0)
		return;

	if (pdbh->dbch_devicetype != DBT_DEVTYP_VOLUME)
		return;

	switch (nEventType)
	{
		case DBT_DEVICEARRIVAL:
		case DBT_DEVICEREMOVECOMPLETE:
		{
			theDriveInfo->Init();
			theSplitterManager->MSWDiskDriveInit();
		}
			break;

		default:
			break;
	}
}
#endif // __WXMSW__
