#ifndef NEXTMFRAME_H_INCLUDED
#define NEXTMFRAME_H_INCLUDED

class CNextMStatusBar;

class CNextMFrame : public wxFrame
{
public:
	CNextMFrame(wxFrame *frame, const wxString& strTitle);
	virtual ~CNextMFrame();

private:
#ifdef __WXMSW__
	virtual WXLRESULT MSWDefWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);
	void OnDeviceChange(WXWPARAM nEventType, WXLPARAM dwData);
#endif

	void Initialize();

private:
	void CreateToolBar();

private:
	bool m_bMaximize    = false;
    bool m_bInitialized = false;

    wxAuiManager	m_mgr;
    std::unique_ptr<CNextMStatusBar> m_pStatusBar;

    std::unique_ptr<wxAuiToolBar>   m_pAuiToolbar = nullptr;
	//장치 등록/해제 이벤트 처리
#ifdef __WXMSW__
	HDEVNOTIFY m_hNotifyDev;
#endif

private:
   	void OnClose(wxCloseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnMaximized(wxMaximizeEvent& event);
	void OnIconized(wxIconizeEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_H_INCLUDED
