#include "../common/common.h"
#include "NextMConfig.h"

#include <iostream>
#include <fstream>
#include <streambuf>

std::unique_ptr<CNextMConfig> CNextMConfig::m_pInstance(nullptr);

CNextMConfig::CNextMConfig()
	: m_nWidth(1480)
	, m_nHeight(800)
	, m_nXPos(0)
	, m_nYPos(0)
	, m_nSplitWindowStyle(WINDOW_SPLIT_NONE)
	, m_nDispColumn(1)
	, m_nDispFloppy(0)
	, m_nDispSystemFile(0)
	, m_nDispHiddenFile(0)
	, m_nDispHeader(0)
	, m_nMinThreadRead(50)
	, m_nFileInfoView(1)
	, m_nTimeFormat(1)
	, m_nFontSize(10)
	, m_nPreIconReadCount(500)
	, m_nCurrentMonitor(0)
	, m_nMaximized(0)
	, m_nLastSessionSave(0)
	, m_iPreRead(0)
	, m_iUseSHBrowseForFolder(-1)
	, m_iUseWindowShellFileOperation(0)
	, m_strFontName(wxT("consolas"))
	, m_strLeftPath(wxT("C:"))
	, m_strRightPath(wxT("C:"))
	, m_tabStyle(0)
	, m_rcDefault(100, 100, 1280, 900)
{
	m_tabStyle = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;
	m_tabStyle |= wxAUI_NB_CLOSE_ON_ALL_TABS;
	m_tabStyle |= wxAUI_NB_WINDOWLIST_BUTTON;
	m_tabStyle |= wxAUI_NB_SCROLL_BUTTONS;
}

CNextMConfig::~CNextMConfig()
{
	_jsonDoc.RemoveAllMembers();
}

CNextMConfig* CNextMConfig::Get()
{
	if (m_pInstance.get() == NULL)
		m_pInstance.reset(new CNextMConfig());

	return m_pInstance.get();
}
/*
bool CNextMConfig::IsExistMember(const wxString& strGroup, const wxString& strMember)
{
	assert(_jsonDoc.IsObject());

	wxString _strKey(strGroup);
	wxString _strMember(strMember);

	if (_strKey.IsEmpty()) return HasMember(_strMember);
	else                   return HasMember(_strMember, _strKey);

	return true;
}
*/
bool CNextMConfig::Load()
{
	_strJsonPath = theUtility->GetWorkDir() + SLASH + wxT("default") + SLASH + wxT("global.cfg");
	if (!wxFileName::FileExists(_strJsonPath))
	{
		theUtility->ShowMessageBox(_strJsonPath + wxT(" file is not exist"), wxICON_ERROR | wxOK);
		return false;
	}

	if(!DoLoad(_strJsonPath))
		return false;

//	SetDefaultConfig();
	return true;
}

/*
void CNextMConfig::SetDefaultConfig()
{
	assert(_jsonDoc.IsObject());
	assert(HasMember(wxT("globalsettings")));

	m_nDispMsgBoxDirWatcher     = _jsonDoc["globalsettings"]["DispMsgBoxDirWatcher"].GetInt();
	//플로피 디스크 보이기
	m_nDispFloppy               = _jsonDoc["globalsettings"]["DispFloppy"].GetInt();

	//시스템파일 보이기
	m_nDispSystemFile           = _jsonDoc["globalsettings"]["DispSystemFile"].GetInt();

	//히든파일 보이기
	m_nDispHiddenFile           = _jsonDoc["globalsettings"]["DispHiddenFile"].GetInt();

	//전체파일 보기
	m_nDispAllFile              = _jsonDoc["globalsettings"]["DispAllFile"].GetInt();

	//리스트 헤더 보이기
	m_nDispHeader               = _jsonDoc["globalsettings"]["DispHeader"].GetInt();

	//마지막 드라이브 보기
	m_nDispDriveListView        = _jsonDoc["globalsettings"]["DispDriveListView"].GetInt();

	//최대화
	m_nMaximized                = _jsonDoc["globalsettings"]["Maximized"].GetInt();

	//현재 모니터
	m_nCurrentMonitor           = _jsonDoc["globalsettings"]["DispMonitor"].GetInt();
//	m_iDirectoryWatcherAsync    = _jsonDoc["globalsettings"]["DirectoryWatcherAsync"].GetInt();
	//SBBrowser 사용
	m_iUseSHBrowseForFolder     = _jsonDoc["globalsettings"]["UseWindowSHBrowseFolder"].GetInt();

	//IFileOperation interfaece 사용여부(파일 삭제시)
	//회사의 경우 보안프로그램으로 인해 IFileOperation interface 사용이 안될수도 있음..
	m_iUseWindowShellFileOperation = _jsonDoc["globalsettings"]["UseWindowShell"].GetInt();

	//한글상태 + Shift 키를 누르면 드라이브이동
    m_iShift_IME_KOR_MoveDrv    = _jsonDoc["globalsettings"]["Shift_IME_KOR_MoveDrv"].GetInt();

    //마지막 세션 저장
    m_nLastSessionSave          = _jsonDoc["globalsettings"]["LastSessionSave"].GetInt();

    //센터 스크린
	m_iCenterScreen             = _jsonDoc["globalsettings"]["DisplayCenterOnScreen"].GetInt();

	//디렉토리 관리자 부모노드 표시
	m_iTopSCDParent             = _jsonDoc["globalsettings"]["SelectSCDItem_toparent"].GetInt();

	//왼쪽창 Default 디렉토리
	m_strLeftPath               = _jsonDoc["globalsettings"]["LeftPath"].GetString();

	//오른쪽창 Default 디렉토리
	m_strRightPath              = _jsonDoc["globalsettings"]["RightPath"].GetString();

	//윈도우 분할 스타일
	m_nSplitWindowStyle         = _jsonDoc["globalsettings"]["SplitWindowStyle"].GetInt();

	//툴바 보이기
	m_nViewToolBar              = _jsonDoc["globalsettings"]["ViewToolBar"].GetInt();

	//컬럼
	m_nDispColumn               = _jsonDoc["globalsettings"]["DispColumn"].GetInt();

	//폰트
	m_strFontName               = _jsonDoc["globalsettings"]["fontname"].GetString();

	//폰트크기
	m_nFontSize                 = _jsonDoc["globalsettings"]["fontsize"].GetInt();

	//컬러테마
	m_strColorTheme             = _jsonDoc["globalsettings"]["color"].GetString();

	m_nSortType                 = _jsonDoc["globalsettings"]["SortType"].GetInt();
}
*/

void CNextMConfig::SetChangeGlobalVal(const wxString& strMember, int iVal)
{
	wxString _strMember(strMember);

	Value _Key(_strMember.c_str(), _jsonDoc.GetAllocator());
	_jsonDoc["globalsettings"][_Key] = iVal;
}

void CNextMConfig::SetChangeGlobalVal(const wxString& strMember, const wxString& strVal)
{
	wxString _strMember(strMember);
	wxString _strVal(strVal);

	Value _Key(_strMember.c_str(), _jsonDoc.GetAllocator());
	Value _Val(_strVal.c_str(), _jsonDoc.GetAllocator());

	_jsonDoc["globalsettings"][_Key] = _Val;
}

/*
void CNextMConfig::SaveConfig()
{
	wxString strOutJson(_strJsonPath);
	std::ofstream ofs;

	ofs.open(strOutJson.char_str());

	OStreamWrapper osw(ofs);
	PrettyWriter<OStreamWrapper> writer(osw);

	_jsonDoc.Accept(writer);
}
*/
