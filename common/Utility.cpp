#include "common.h"
#include "Utility.h"

std::unique_ptr<CUtility> CUtility::m_pInstance(nullptr);

CUtility* CUtility::Get()
{
	if (m_pInstance == nullptr)
		m_pInstance.reset(new CUtility());

	m_pInstance->SetWorkingDirectory();
	m_pInstance->Init();

	return m_pInstance.get();
}

#ifdef __WXMSW__
int CUtility::GetWindowsVersion()
{
	int nVersion = -1;
	OSVERSIONINFOEX osvi = { 0, };
	BOOL bVersion_ex_flag = 0;

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bVersion_ex_flag = GetVersionEx((OSVERSIONINFO *)&osvi);
	if (!bVersion_ex_flag)
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *)&osvi))
			return -1;
	}

	switch (osvi.dwPlatformId)
	{
		case VER_PLATFORM_WIN32_WINDOWS:		//Windows 9x
			{
				if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) nVersion = WIN_OS_95;
				else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) nVersion = WIN_OS_98;
				else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)	nVersion = WIN_OS_ME;
			}

			break;

		case VER_PLATFORM_WIN32_NT:				//Windows NT 기술 기반의 운영체제 버전
			{
				if (osvi.dwMajorVersion <= 4) nVersion = WIN_OS_NT;
				else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) nVersion = WIN_OS_2000;
				else if (bVersion_ex_flag)
				{
					if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) nVersion = WIN_OS_XP;
					else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) nVersion = WIN_OS_2003;
					else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) nVersion = WIN_OS_VISTA_OR_2008;
					else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) nVersion = WIN_OS_7_OR_2008R2;
					else nVersion = WIN_OS_7_OVER;
				}
			}

			break;
		default:
			break;
	}

	return nVersion;
}
#endif

wxString CUtility::GetSystemLang()
{
#ifdef __WXMSW__
	int icchLocalName = LOCALE_NAME_MAX_LENGTH;
	#if defined(_UNICODE) || defined(UNICODE)
		WCHAR lpBuffer[LOCALE_NAME_MAX_LENGTH];
	#else
		LPSTR lpBuffer[LOCALE_NAME_MAX_LENGTH];
	#endif

	wxZeroMemory(lpBuffer);
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, lpBuffer, icchLocalName);
	return wxString(lpBuffer);
#else
#endif
}

void CUtility::SetWorkingDirectory()
{
	if(m_strWorkingDirectory.IsEmpty())
	{
		m_strWorkingDirectory = wxGetCwd();
#ifndef NDEBUG
		m_strWorkingDirectory += SLASH;
		m_strWorkingDirectory += wxT("src");
#endif // __WXDEBUG__
	}
}

void CUtility::Init()
{
	larrAttr[0] = ATTR_HIDDEN;// | ATTR_SYSTEM;
	larrAttr[1] = ATTR_SYSTEM;
	larrAttr[2] = 0;//ATTR_SYSTEM;
}

//이미지리스트 초기
void CUtility::LoadImageList()
{
#ifdef __WXMSW__
	// IID_IImageList {46EB5926-582E-4017-9FDF-E8998DAA0950}
	static const GUID IID_IImageList = { 0x46EB5926, 0x582E, 0x4017, { 0x9F, 0xDF, 0xE8, 0x99, 0x8D, 0xAA, 0x9, 0x50 } };
	// IID_IImageList2 {192B9D83-50FC-457B-90A0-2B82A8B5DAE1}
	static const GUID IID_IImageList2 = { 0x192B9D83, 0x50FC, 0x457B, { 0x90, 0xA0, 0x2B, 0x82, 0xA8, 0xB5, 0xDA, 0xE1 } };

	_gImageList = nullptr;
	HRESULT res = SHGetImageList(SHIL_SMALL, IID_IImageList2, (void**)& _gImageList);
	if (FAILED(res))
		res = SHGetImageList(SHIL_SMALL, IID_IImageList, (void**)& _gImageList);
#else
	_gImageList = wxTheFileIconsTable->GetSmallImageList();
#endif
}

wxString CUtility::GetPathName(const wxString& strFullPath)
{
	if(strFullPath.Len() == 3)
		return strFullPath;

	int iSlashPos = strFullPath.rfind(SLASH);
	return (strFullPath.Mid(iSlashPos + 1, strFullPath.Len()));
}

wxString CUtility::GetParentPath(const wxString& strFullPath)
{
	wxString _strVolume;
	wxString _strPath;
	wxString _strName;
	wxString _strExt;

	wxFileName::SplitPath(strFullPath, &_strVolume, &_strPath, &_strName, &_strExt);

	return (_strPath.IsEmpty() ? _strVolume + wxT(":") + SLASH : _strVolume + wxT(":") + _strPath);
}

void CUtility::RefreshWindow(wxWindow* pWindow, const wxRect& rect, bool bUpdate)
{
#ifdef __WXMSW__
	HWND hWnd = pWindow->GetHWND();
	if (rect.GetWidth() != 0)
	{
		RECT mswRect;
		const RECT* pRect;

		wxCopyRectToRECT(rect, mswRect);
		pRect = &mswRect;

		//	INT flags = RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW | RDW_ERASE;
		//	::RedrawWindow(hWnd, pRect, NULL, flags);
		::InvalidateRect(hWnd, pRect, false);
	}
	else
		pWindow->Refresh(false);

	if (bUpdate)
		pWindow->Update();
#else
	pWindow->RefreshRect(rect, false);
	pWindow->Update();
#endif
}

bool CUtility::IsAllowAttr(const unsigned long _lattr)
{
	unsigned long lattr = _lattr & larrAttr[_gAttrIndex];
	return (lattr > 0);
}

int CUtility::Compare(const wxString& strOld, const wxString& strNew)
{
#ifdef __WXMSW__
	return strOld.CmpNoCase(strNew);
#else
	return strOld.Cmp(strNew);
#endif
}

wxString CUtility::GetMaxData(wxWindow* pWindow, const wxFont& _font, const wxString& strSrc, const wxString& strTgt)
{
	wxClientDC clientDC(pWindow);
	clientDC.SetFont(_font);

	wxSize szA = clientDC.GetTextExtent(strSrc);
	wxSize szB = clientDC.GetTextExtent(strTgt);

	wxString strMaxData = szA.GetWidth() < szB.GetWidth() ? strTgt : strSrc;

	clientDC.SetFont(wxNullFont);
	return strMaxData;
}

wxString CUtility::GetKeyName(const wxKeyEvent& event)
{
	int nKeyCode = event.GetKeyCode();
	const char* virt = GetVirtualKeyCodeName(nKeyCode);

	if (virt)
		return virt;

	if (nKeyCode > 0 && nKeyCode < 32)
		return wxString::Format(wxT("Ctrl-%c"), (unsigned char)('A' + nKeyCode - 1));

	if (nKeyCode >= 32 && nKeyCode < 128)
		return wxString::Format(wxT("%c"), (unsigned char)nKeyCode);

#if wxUSE_UNICODE
	int uc = event.GetUnicodeKey();
	if (uc != WXK_NONE)
		return wxString::Format(wxT("%c"), uc);
#endif

	return wxT("UnKnown");
}

const char* CUtility::GetVirtualKeyCodeName(int nKeyCode)
{
	switch (nKeyCode)
	{
#define WXK_(x) \
        case WXK_##x: return #x;

		WXK_(BACK)
			WXK_(TAB)
			WXK_(RETURN)
			WXK_(ESCAPE)
//			WXK_(SPACE)
			WXK_(DELETE)
			WXK_(START)
			WXK_(LBUTTON)
			WXK_(RBUTTON)
			WXK_(CANCEL)
			WXK_(MBUTTON)
			WXK_(CLEAR)
			WXK_(SHIFT)
			WXK_(ALT)
			WXK_(CONTROL)
			WXK_(MENU)
			WXK_(PAUSE)
			WXK_(CAPITAL)
			WXK_(END)
			WXK_(HOME)
			WXK_(LEFT)
			WXK_(UP)
			WXK_(RIGHT)
			WXK_(DOWN)
			WXK_(SELECT)
			WXK_(PRINT)
			WXK_(EXECUTE)
			WXK_(SNAPSHOT)
			WXK_(INSERT)
			WXK_(HELP)
			WXK_(NUMPAD0)
			WXK_(NUMPAD1)
			WXK_(NUMPAD2)
			WXK_(NUMPAD3)
			WXK_(NUMPAD4)
			WXK_(NUMPAD5)
			WXK_(NUMPAD6)
			WXK_(NUMPAD7)
			WXK_(NUMPAD8)
			WXK_(NUMPAD9)
			WXK_(MULTIPLY)
			WXK_(ADD)
			WXK_(SEPARATOR)
			WXK_(SUBTRACT)
			WXK_(DECIMAL)
			WXK_(DIVIDE)
			WXK_(F1)
			WXK_(F2)
			WXK_(F3)
			WXK_(F4)
			WXK_(F5)
			WXK_(F6)
			WXK_(F7)
			WXK_(F8)
			WXK_(F9)
			WXK_(F10)
			WXK_(F11)
			WXK_(F12)
			WXK_(F13)
			WXK_(F14)
			WXK_(F15)
			WXK_(F16)
			WXK_(F17)
			WXK_(F18)
			WXK_(F19)
			WXK_(F20)
			WXK_(F21)
			WXK_(F22)
			WXK_(F23)
			WXK_(F24)
			WXK_(NUMLOCK)
			WXK_(SCROLL)
			WXK_(PAGEUP)
			WXK_(PAGEDOWN)
			WXK_(NUMPAD_SPACE)
			WXK_(NUMPAD_TAB)
			WXK_(NUMPAD_ENTER)
			WXK_(NUMPAD_F1)
			WXK_(NUMPAD_F2)
			WXK_(NUMPAD_F3)
			WXK_(NUMPAD_F4)
			WXK_(NUMPAD_HOME)
			WXK_(NUMPAD_LEFT)
			WXK_(NUMPAD_UP)
			WXK_(NUMPAD_RIGHT)
			WXK_(NUMPAD_DOWN)
			WXK_(NUMPAD_PAGEUP)
			WXK_(NUMPAD_PAGEDOWN)
			WXK_(NUMPAD_END)
			WXK_(NUMPAD_BEGIN)
			WXK_(NUMPAD_INSERT)
			WXK_(NUMPAD_DELETE)
			WXK_(NUMPAD_EQUAL)
			WXK_(NUMPAD_MULTIPLY)
			WXK_(NUMPAD_ADD)
			WXK_(NUMPAD_SEPARATOR)
			WXK_(NUMPAD_SUBTRACT)
			WXK_(NUMPAD_DECIMAL)
			WXK_(NUMPAD_DIVIDE)
			WXK_(WINDOWS_LEFT)
			WXK_(WINDOWS_RIGHT)
#if defined(__WXOSX__)
			WXK_(RAW_CONTROL)
#endif
#undef WXK_

		default:
			return NULL;
	}
}

wxString CUtility::ChangeName(const wxString& strName)
{
	wxString sName, sNewName, sResult;
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char name[MAX_PATH];
	char ext[MAX_PATH];
	_splitpath(strName, drive, dir, name, ext);
	sName = name;

	int pos = sName.rfind(wxT("_Copy"));//.Find("_");
	if (pos == -1)
		sNewName = sName + wxT("_Copy") + wxString(ext);
	else
	{
		if(Compare(sName.Right(1), wxT(")")) != 0)
		{
			sNewName = sName;
			sNewName.Replace(wxT("_Copy"), wxT(""));
			sNewName = sNewName + wxT("_Copy(01)") + wxString(ext);
		}
		else
		{
			int pos2 = sName.rfind(wxT(")"));//.Find(wxT(")"));
			wxString sCount;
			wxString strFileName = sName.Mid(0, pos2 - 3);// , sName.Len());
			sCount = sName.Mid(strFileName.Len() + 1, 2);
			int iCount = atoi(sCount);
			iCount++;
			sNewName = wxString::Format(wxT("%s%s%02d%s%s"), strFileName, wxT("("), iCount, wxT(")"), ext);
		}
	}

	bool bExistFile = false;
	bool bExistDir = false;

	wxString strExist = wxString(drive) + wxString(dir) + sNewName;
	if (wxFileExists(strExist))
		bExistFile = true;

	if (wxDirExists(strExist))
		bExistDir = true;

	if (!bExistFile && !bExistDir)
		sResult = sNewName;
	else
		sResult = ChangeName(strExist);

	return sResult;
}

wxString CUtility::GetExt(const wxString& strFileName)
{
	wxString strExt(wxT(""));
	wxString strName(strFileName);

	if (strName.find_last_of(wxT(".")) != wxString::npos)
		strExt = strName.substr(strName.find_last_of(wxT(".")) + 1);

	return strExt;
}

wxString CUtility::GetFileDescription(const wxString& strExt, const wxString& strFullPathName)
{
	wxString _strExt(strExt);
	wxString strExtInfo(wxT(""));

#ifdef __WXMSW__
	int flags = SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES;
	SHFILEINFO shfinfo;
	wxZeroMemory(shfinfo);

	HRESULT hr = SHGetFileInfo(strFullPathName, FILE_ATTRIBUTE_NORMAL, &shfinfo, sizeof(shfinfo), flags);
	if(SUCCEEDED(hr))
	{
		strExtInfo = shfinfo.szTypeName;

		if (strExtInfo.IsEmpty())
		{
			if (!_strExt.IsEmpty())
				strExtInfo = _strExt + theMsg->GetMessage(wxT("MSG_FILE_DESCRIPTION"));
			else
				strExtInfo = theMsg->GetMessage(wxT("MSG_FILE_GENERIC"));
		}
	}
#else
#endif

	return strExtInfo;
}

void CUtility::GetIconIndex(const wxString& strPath, int &nIconIndex, int &nOverlayIndex, bool bExtFind)
{
#ifdef __WXMSW__
	nIconIndex = 0;
	nOverlayIndex = 0;

	DWORD dwNum = GetFileAttributes(strPath);
	DWORD attr = 0;

	attr = dwNum & FILE_ATTRIBUTE_DIRECTORY ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL;

	SHFILEINFO sfi;
	wxZeroMemory(sfi);

	UINT flag = IMAGELIST_FLAG;

	if(bExtFind)
	{
		attr = 0;
		flag |= SHGFI_USEFILEATTRIBUTES;
	}

	SHGetFileInfo(strPath, attr, &sfi, sizeof(sfi), flag);

	nIconIndex = (sfi.iIcon & 0x00FFFFFF);
	nOverlayIndex = (sfi.iIcon >> 24) - 1;

	DestroyIcon(sfi.hIcon);
#else

#endif
}

void CUtility::GetSizeInfo(const double dblFileSize, wxString& strFileSize, wxString& strFileSizeType, wxColour& dispColor)
{
	double dblSize = 0.0;
	dispColor = theColor->GetDefaultColor();//wxColour(192, 192, 192);

	strFileSizeType.clear();

	if (dblFileSize < KILOBYTE)
		strFileSize = wxString::Format(wxT("%3.0f"), dblFileSize);

	if (dblFileSize >= KILOBYTE)
	{
		dblSize = dblFileSize / KILOBYTE;
		strFileSizeType = _T("KB");
	}

	if (dblFileSize >= MEGABYTE)
	{
		dblSize = dblFileSize / MEGABYTE;
		strFileSizeType = _T("MB");

		dispColor = *wxRED;
	}

	if (dblFileSize >= GIGABYTE)
	{
		dblSize = dblFileSize / GIGABYTE;
		strFileSizeType = _T("GB");

		dispColor = *wxGREEN;
	}

	if (dblFileSize >= TERABYTE)
	{
		dblSize = dblFileSize / TERABYTE;
		strFileSizeType = _T("TB");

		dispColor = *wxBLUE;
	}

	if (strFileSizeType.Cmp(wxT("")) != 0)
		strFileSize = wxString::Format(wxT("%3.2f"), dblSize);
}

void CUtility::LaunchAndExec(const wxString& strExecProgram, const wxString& strPath)
{
#ifdef __WXMSW__
	wxString strExt = GetExt(strExecProgram);
	if(strExt.IsEmpty())
	{
		wxMessageBox(strExecProgram + wxT(" is not exist file extention info"), PROGRAM_FULL_NAME, wxICON_INFORMATION | wxOK);
		return;
	}

	if(Compare(strExt.MakeLower(), wxT("exe")) == 0)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		wxZeroMemory(si);
		wxZeroMemory(pi);

		si.cb = sizeof(si);
		SetErrorMode(0);
		CreateProcess(NULL,
					CONVSTR(strExecProgram),
					NULL,
					NULL,
					FALSE,
					0,
					NULL,
					CONVSTR(strPath),
					&si,
					&pi);
	}
	else
		wxLaunchDefaultApplication(strExecProgram);
#else
	wxExecute(strExecProgram, wxEXEC_ASYNC);
#endif
}

wxString CUtility::SetComma(const wxString& strSize)
{
	wxString strVal(strSize);

	wxChar ch = wxT(',');
	int iThousandSep = 3;

	int pos = strVal.length();
	while(pos > iThousandSep)
	{
		pos -= iThousandSep;
		strVal.insert(pos, ch);
	}

	return strVal;
}

wxString CUtility::SetComma(unsigned long long val)
{
	wxString strVal = wxString::Format(wxT("%llu"), val);
	return SetComma(strVal);
}

wxString CUtility::MakeFullPathName(const wxString& strCurrentPath, const wxString& strNewPathName)
{
	wxString strFullPathName(strCurrentPath);
	if(strCurrentPath.Right(1).CmpNoCase(SLASH) != 0)
		strFullPathName += SLASH;

	strFullPathName += strNewPathName;

	return strFullPathName;
}

std::vector<wxString> CUtility::Split(const wxString& strSource, const wxString& strDelimiter)
{
	std::vector<wxString> tokens;

	wxStringTokenizer tokenizer(strSource, strDelimiter);
	while (tokenizer.HasMoreTokens())
	{
		wxString strToken = tokenizer.GetNextToken();
		tokens.emplace_back(strToken);
	}

	return tokens;
}

const char* CUtility::ConvertWC2MB(const wxString& strData)
{
	wxString strTmp(strData);
	const wxWX2MBbuf tmp_buf = wxConvCurrent->cWX2MB(strTmp);
	const char *tmp_str = (const char*)tmp_buf;

	return tmp_str;
}

#ifdef __WXMSW__
void CUtility::SetImeModeToEnglish(wxWindow* pWindow)
{
	HIMC hIMC;
	DWORD dwConversion;
	DWORD dwSentence;

	hIMC = ::ImmGetContext(pWindow->GetHWND());
	ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence);
	if (dwConversion & IME_CMODE_NATIVE)     // 한글 mode
		ImmSetConversionStatus(hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);    // 한글모드아니게 설정

	ImmReleaseContext(pWindow->GetHWND(), hIMC);
}
#endif // __WXMSW__

//메세지 표시
void CUtility::ShowMessageBox(const wxString& strMsg, int iRetCode)
{
	wxMessageBox(strMsg, PROGRAM_FULL_NAME, iRetCode);
}


#ifndef NDEBUG
void CUtility::LogDisplay(const wxString& strMsg)
{
#if defined(_UNICODE) || defined(UNICODE)
	std::wcout << strMsg << std::endl;
#else
	std::cout << strMsg << std::endl;
#endif
}
#endif
