#include "ginc.h"
#include "NextMApp.h"
#include "NextMFrame.h"

wxIMPLEMENT_APP(CNextMApp);

CNextMApp::CNextMApp()
{

}

CNextMApp::~CNextMApp()
{

}

bool CNextMApp::OnInit()
{
	if (!wxApp::OnInit())
	{
		wxMessageBox(wxT("Windows Initialize failed"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return false;
	}

	if (!Initialize())
		return false;

#ifdef NDEBUG
	m_pSingleInstance = new wxSingleInstanceChecker;
	if (m_pSingleInstance->IsAnotherRunning())
	{
		delete m_pSingleInstance;
		m_pSingleInstance = NULL;

		wxMessageBox(theMsg->GetMessage(wxT("MSG_ALREADY_EXEC_PROCESS")), PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
		return false;
	}
#endif

	wxString strTitle(PROGRAM_FULL_NAME);
#ifndef NDEBUG
	strTitle += wxT(" - Debug Version");
#endif

	strTitle += wxT("(Beta) - Made by BaeByoungYong");
	CNextMFrame* pMainFrame = new CNextMFrame(0L, strTitle);

	_gMainFrame = pMainFrame;
	this->SetTopWindow(pMainFrame);

    pMainFrame->SetIcon(wxICON(wxwin));
    pMainFrame->Show();

	return true;
}

int CNextMApp::OnExit()
{
#ifdef NDEBUG
	wxDELETE(m_pSingleInstance);
#endif

	_gmapAllowProcessKey.clear();
	return 0;
}

bool CNextMApp::Initialize()
{
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F1, WXK_F1));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F2, WXK_F2));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F3, WXK_F3));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F4, WXK_F4));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F5, WXK_F5));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F6, WXK_F6));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F7, WXK_F7));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F8, WXK_F8));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F9, WXK_F9));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F10, WXK_F10));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F11, WXK_F11));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F12, WXK_F12));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F13, WXK_F13));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F14, WXK_F14));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F15, WXK_F15));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F16, WXK_F16));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F17, WXK_F17));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F18, WXK_F18));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F19, WXK_F19));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F20, WXK_F20));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F21, WXK_F21));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F22, WXK_F22));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F23, WXK_F23));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F24, WXK_F24));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_NUMLOCK, WXK_NUMLOCK));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_F4, WXK_F4));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_REVERSE_SLASH, WXK_REVERSE_SLASH));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_TAB, WXK_TAB));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_SPACE, WXK_SPACE));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_INSERT, WXK_INSERT));
	_gmapAllowProcessKey.insert(std::make_pair(wxKEY_DOT, wxKEY_DOT));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_BACK, WXK_BACK));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_RETURN, WXK_RETURN));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_LEFT, WXK_LEFT));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_RIGHT, WXK_RIGHT));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_UP, WXK_UP));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_DOWN, WXK_DOWN));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_PAGEDOWN, WXK_PAGEDOWN));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_PAGEUP, WXK_PAGEUP));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_HOME, WXK_HOME));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_END, WXK_END));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_DELETE, WXK_DELETE));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_LEFT_M_G, WXK_LEFT_M_G));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_RIGHT_M_G, WXK_RIGHT_M_G));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_V_LINE, WXK_V_LINE));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_WINDOWS_KEY, WXK_WINDOWS_KEY));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_ESCAPE, WXK_ESCAPE));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_WINDOWS_LEFT, WXK_WINDOWS_LEFT));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_WINDOWS_RIGHT, WXK_WINDOWS_RIGHT));
	_gmapAllowProcessKey.insert(std::make_pair(WXK_WINDOWS_MENU, WXK_WINDOWS_MENU));

	//Read System Language
	_gSystemLang = theUtility->GetSystemLang();

	//이미지리스트
	theImageList->LoadImageList();
	theImageList->Attach(_gImageList);

	_strCurrentPath = theUtility->GetWorkDir();

	if (!InitConfig())
		return false;

	//폰트 설정
	wxString strFontName(theConfig->GetFontName());
	int nFontSize = theConfig->GetFontSize();

#ifdef __WXMSW__
	theDriveInfo->Init();

	//디렉토리 트리 탐색시 무시 항목리스트
	_gVecIgnore.emplace_back(wxT(""));
	_gVecIgnore.emplace_back(wxT("."));
	_gVecIgnore.emplace_back(wxT(".."));
	_gVecIgnore.emplace_back(wxT(".git"));
	_gVecIgnore.emplace_back(wxT("winsxs"));
	_gVecIgnore.emplace_back(wxT("WinSxS"));

	//폰트 설정
	wxString strFixedsysFontPath(wxT(""));
	strFixedsysFontPath = _strCurrentPath + SLASH + wxT("font") + SLASH + wxT("hvgafix.fon");// wxT("FSEX300.ttf");

	//등록하고자 하는 폰트가 존재하는지 체크
	wxArrayString arrFontList = wxFontEnumerator::GetFacenames();
	bool bFontExist = false;
	for (auto item : arrFontList)
	{
		if (item.Cmp(wxT("Fixedsys")) == 0)
		{
			bFontExist = true;
			break;
		}
	}

	if (!bFontExist)
	{
		int iErr = AddFontResource(CONVSTR(strFixedsysFontPath));
		if (iErr != 0)
			m_bAddFontRC = true;
	}

	_gViewFont = new wxFont(nFontSize, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, strFontName);
#else
	_gViewFont = new wxFont(wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT));
#endif

	_gNormalFolderIco = wxArtProvider::GetIcon(wxART_FOLDER, wxART_OTHER, wxSize(16, 16));
	_gNormalFileIco   = wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));

	//메뉴읽기
	wxXmlResource::Get()->InitAllHandlers();
	wxString strMenuXRC = wxT("menus_en.xrc");

	if (_gSystemLang.Cmp(wxT("ko")) == 0)
		strMenuXRC = wxT("menus_ko.xrc");
	else
		strMenuXRC = wxT("menus_en.xrc");

	wxString strXrcPath = _strCurrentPath + SLASH + wxT("xrc") + SLASH + strMenuXRC;
	bool bXRCLoaded = wxXmlResource::Get()->Load(strXrcPath);

	if (!bXRCLoaded)
	{
		wxString strMsg = theMsg->GetMessage(wxT("MSG_XRC_MENU_LOAD_FAILED"));
		wxMessageBox(strMsg, PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return false;
	}

	return true;
}

bool CNextMApp::InitConfig()
{
	wxString strMsg(wxT(""));
	if(!theMsg->Load())
	{
		theUtility->ShowMessageBox(wxT("Message file load is failed"), wxICON_ERROR | wxOK);
		return false;
	}

	if(!theConfig->Load())
	{
		strMsg = theMsg->GetMessage(wxT("MSG_INITFAIL_WINDOW_SETTING"));
		theUtility->ShowMessageBox(strMsg, wxICON_ERROR | wxOK);
		return false;
	}

	if(!theColor->Load())
	{
		theUtility->ShowMessageBox(wxT("The color file load is failed"), wxICON_ERROR | wxOK);
		return false;
	}

	if (!theBookmark->Load())
	{
		theUtility->ShowMessageBox(theMsg->GetMessage(wxT("MSG_INITFAIL_WINDOW_BOOKMARK")), wxICON_ERROR | wxOK);
		return false;
	}

	if (!theExternal->Load())
	{
		theUtility->ShowMessageBox(wxT("The external program file load is failed"), wxICON_ERROR | wxOK);
		return false;
	}
#ifdef __WXMSW__
	if(!theIconInfo->Load())
	{
		theUtility->ShowMessageBox(wxT("The iconfile load is failed"), wxICON_ERROR | wxOK);
		return false;
	}
#endif // __WXMSW__
	return true;
}

