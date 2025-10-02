#include "../ginc.h"
#include "FileVersionInfo.h"

CFileVersionInfo::CFileVersionInfo()
{

}

CFileVersionInfo::~CFileVersionInfo()
{

}

void CFileVersionInfo::Reset()
{
	wxZeroMemory(m_FileInfo);

	m_strCompanyName.clear();
	m_strFileDescription.clear();
	m_strFileVersion.clear();
	m_strInternalName.clear();
	m_strLegalCopyright.clear();
	m_strOriginalFileName.clear();
	m_strProductName.clear();
	m_strProductVersion.clear();
	m_strComments.clear();
	m_strLegalTrademarks.clear();
	m_strPrivateBuild.clear();
	m_strSpecialBuild.clear();
}

bool CFileVersionInfo::Create(const wxString& lpszFileName)
{
	Reset();
#ifdef __WXMSW__
	DWORD	dwHandle;
	DWORD	dwFileVersionInfoSize = GetFileVersionInfoSize(lpszFileName.wc_str(), &dwHandle);
	if (!dwFileVersionInfoSize)
		return false;

	void* lpData = (void *)new BYTE[dwFileVersionInfoSize];
	if (!lpData)
		return false;

	try
	{
		if (!GetFileVersionInfo(lpszFileName.wc_str(), dwHandle, dwFileVersionInfoSize, lpData))
			throw false;

		// catch default information
		void*	lpInfo;
		UINT	unInfoLen;
		if (VerQueryValue(lpData, _T("\\"), &lpInfo, &unInfoLen))
		{
			//ASSERT(unInfoLen == sizeof(m_FileInfo));
			if (unInfoLen == sizeof(m_FileInfo))
				memcpy(&m_FileInfo, lpInfo, unInfoLen);
		}

		// find best matching language and codepage
		VerQueryValue(lpData, _T("\\VarFileInfo\\Translation"), &lpInfo, &unInfoLen);

		DWORD	dwLangCode = 0;
		if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, false))
		{
			if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, true))
			{
				if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), dwLangCode, true))
				{
					if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), dwLangCode, false))
						// use the first one we can get
						dwLangCode = *((DWORD*)lpInfo);
				}
			}
		}

		wxString	strSubBlock;
		TCHAR buf[1024];
		_stprintf(buf, _T("\\StringFileInfo\\%04X%04X\\"), dwLangCode & 0x0000FFFF, (dwLangCode & 0xFFFF0000) >> 16);
		strSubBlock = buf;

		// catch string table
		wxString sBuf(wxT(""));

		sBuf = strSubBlock;
		sBuf += _T("CompanyName");
		if (VerQueryValue(lpData, sBuf.c_str (), &lpInfo, &unInfoLen))
			m_strCompanyName = wxString((LPCTSTR)lpInfo);

		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("FileDescription");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strFileDescription = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("FileVersion");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strFileVersion = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("InternalName");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strInternalName = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("LegalCopyright");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strLegalCopyright = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("OriginalFileName");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strOriginalFileName = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("ProductName");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strProductName = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("ProductVersion");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strProductVersion = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("Comments");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strComments = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("LegalTrademarks");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strLegalTrademarks = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("PrivateBuild");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strPrivateBuild = wxString((LPCTSTR)lpInfo);


		sBuf.clear();
		sBuf = strSubBlock;
		sBuf += _T("SpecialBuild");
		if (VerQueryValue(lpData, sBuf.c_str(), &lpInfo, &unInfoLen))
			m_strSpecialBuild = wxString((LPCTSTR)lpInfo);

		delete[] lpData;
	}
	catch (BOOL)
	{
		delete[] lpData;
		return false;
	}

	return true;
#else
#endif
}

#ifdef __WXMSW__
bool CFileVersionInfo::GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, bool bPrimaryEnough/*= FALSE*/)
{
	LPWORD lpwData;
	for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData) + unBlockSize; lpwData += 2)
	{
		if (*lpwData == wLangId)
		{
			dwId = *((DWORD*)lpwData);
			return true;
		}
	}

	if (!bPrimaryEnough)
		return false;

	for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData) + unBlockSize; lpwData += 2)
	{
		if (((*lpwData) & 0x00FF) == (wLangId & 0x00FF))
		{
			dwId = *((DWORD*)lpwData);
			return true;
		}
	}

	return false;
}

WORD CFileVersionInfo::GetFileVersion(int nIndex) const
{
	if (nIndex == 0)
		return (WORD)(m_FileInfo.dwFileVersionLS & 0x0000FFFF);
	else if (nIndex == 1)
		return (WORD)((m_FileInfo.dwFileVersionLS & 0xFFFF0000) >> 16);
	else if (nIndex == 2)
		return (WORD)(m_FileInfo.dwFileVersionMS & 0x0000FFFF);
	else if (nIndex == 3)
		return (WORD)((m_FileInfo.dwFileVersionMS & 0xFFFF0000) >> 16);
	else
		return 0;
}


WORD CFileVersionInfo::GetProductVersion(int nIndex) const
{
	if (nIndex == 0)
		return (WORD)(m_FileInfo.dwProductVersionLS & 0x0000FFFF);
	else if (nIndex == 1)
		return (WORD)((m_FileInfo.dwProductVersionLS & 0xFFFF0000) >> 16);
	else if (nIndex == 2)
		return (WORD)(m_FileInfo.dwProductVersionMS & 0x0000FFFF);
	else if (nIndex == 3)
		return (WORD)((m_FileInfo.dwProductVersionMS & 0xFFFF0000) >> 16);
	else
		return 0;
}


DWORD CFileVersionInfo::GetFileFlagsMask() const
{
	return m_FileInfo.dwFileFlagsMask;
}


DWORD CFileVersionInfo::GetFileFlags() const
{
	return m_FileInfo.dwFileFlags;
}


DWORD CFileVersionInfo::GetFileOs() const
{
	return m_FileInfo.dwFileOS;
}


DWORD CFileVersionInfo::GetFileType() const
{
	return m_FileInfo.dwFileType;
}


DWORD CFileVersionInfo::GetFileSubtype() const
{
	return m_FileInfo.dwFileSubtype;
}
#endif // __WXMSW__

wxString CFileVersionInfo::GetCompanyName() const
{
	return m_strCompanyName;
}

wxString CFileVersionInfo::GetFileDescription() const
{
	return m_strFileDescription;
}


wxString CFileVersionInfo::GetFileVersion() const
{
	return m_strFileVersion;
}


wxString CFileVersionInfo::GetInternalName() const
{
	return m_strInternalName;
}


wxString CFileVersionInfo::GetLegalCopyright() const
{
	return m_strLegalCopyright;
}


wxString CFileVersionInfo::GetOriginalFileName() const
{
	return m_strOriginalFileName;
}


wxString CFileVersionInfo::GetProductName() const
{
	return m_strProductName;
}


wxString CFileVersionInfo::GetProductVersion() const
{
	return m_strProductVersion;
}


wxString CFileVersionInfo::GetComments() const
{
	return m_strComments;
}


wxString CFileVersionInfo::GetLegalTrademarks() const
{
	return m_strLegalTrademarks;
}


wxString CFileVersionInfo::GetPrivateBuild() const
{
	return m_strPrivateBuild;
}


wxString CFileVersionInfo::GetSpecialBuild() const
{
	return m_strSpecialBuild;
}
