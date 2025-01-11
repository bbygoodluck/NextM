#include "../ginc.h"
#include "LocalFileSystem.h"
#include <io.h>

static wxInt64 EPOCH_OFFSET_IN_MSEC = wxLL(11644473600000);

CLocalFileSystem::CLocalFileSystem()
	: m_bDirs_only(false)
#ifdef __WXMSW__
	, m_hFind(INVALID_HANDLE_VALUE)
	, m_bfound(false)
	, m_strFind_path(wxT(""))
#else
	, m_raw_path()
	, m_file_part()
	, m_buffer_length()
	, m_dir()
#endif
{
}

CLocalFileSystem::~CLocalFileSystem()
{
	EndFindFiles();
}

bool CLocalFileSystem::BeginFindFiles(wxString path, bool dirs_only)
{
	EndFindFiles();
	m_bDirs_only = dirs_only;

	m_strFind_path = path;
	m_IsLastSlash = false;
#ifdef __WXMSW__
	if( path.Right(1).CmpNoCase(SLASH) != 0)
	{
		m_strFind_path = path + SLASH;
		m_strFind_path += wxT("*");
	}
	else
	{
		m_IsLastSlash = true;
		m_strFind_path = path;
		m_strFind_path += wxT("*");
	}

	m_hFind = FindFirstFileEx(m_strFind_path, FindExInfoStandard, &m_find_data, dirs_only ? FindExSearchLimitToDirectories : FindExSearchNameMatch, NULL, 0);
	if (m_hFind == INVALID_HANDLE_VALUE)
	{
		m_bfound = false;
		return false;
	}

	m_bfound = true;
	return true;
#else
	if (path != _T("/") && path.Last() == '/')
		path.RemoveLast();

	const wxCharBuffer s = path.fn_str();

	m_dir = opendir(s);
	if (!m_dir)
		return false;

	const wxCharBuffer p = path.fn_str();
	const int len = strlen(p);
	m_raw_path = new char[len + 2048 + 2];
	m_buffer_length = len + 2048 + 2;
	strcpy(m_raw_path, p);
	if (len > 1)
	{
		m_raw_path[len] = '/';
		m_file_part = m_raw_path + len + 1;
	}
	else
		m_file_part = m_raw_path + len;

	return true;
#endif
}

bool CLocalFileSystem::GetNextFile(wxString& strName, bool* isDir, unsigned long* lattr, wxLongLong* llSize, wxDateTime* dt)
{
#ifdef __WXMSW__
	if (!m_bfound)
		return false;
	do
	{
		if (!m_find_data.cFileName[0])
		{
			m_bfound = FindNextFile(m_hFind, &m_find_data) != 0;
			return true;
		}

		if (m_bDirs_only && !(m_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			continue;

		wxString strTmpName(m_find_data.cFileName);
		if(strTmpName.CmpNoCase(wxT(".")) == 0)
			continue;

		/*
		 * 공유드라이브(또는 공유폴더)의 최상위 폴더일경우 .. 표시가 되어
		 * 공유드라이브(또는 공유폴더)일경우 ..표시를 화면에 나타내지 않음
		 */
		if(m_IsLastSlash)
		{
			if(strTmpName.CmpNoCase(wxT("..")) == 0)
				continue;
		}

		strName = strTmpName;
		if(lattr != nullptr)
			*lattr = m_find_data.dwFileAttributes;

		if(isDir != nullptr)
			*isDir = (m_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (llSize != nullptr)
		{
			if (*isDir)
				*llSize = -1;
			else
				*llSize = wxLongLong(m_find_data.nFileSizeHigh, m_find_data.nFileSizeLow);
		}

		if (dt != nullptr)
		{
			wxLongLong t(m_find_data.ftLastWriteTime.dwHighDateTime, m_find_data.ftLastWriteTime.dwLowDateTime);
			t /= 10000; // Convert hundreds of nanoseconds to milliseconds.
			t -= EPOCH_OFFSET_IN_MSEC;

			*dt = wxDateTime(t);
		}

		m_bfound = FindNextFile(m_hFind, &m_find_data) != 0;
		return true;
	} while ((m_bfound = FindNextFile(m_hFind, &m_find_data) != 0));

	return false;
#else
	return false;
#endif
}

void CLocalFileSystem::EndFindFiles()
{
#ifdef __WXMSW__
	m_bfound = false;
	if (m_hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(m_hFind);
		m_hFind = INVALID_HANDLE_VALUE;
	}
#else
	if (m_dir)
	{
		closedir(m_dir);
		m_dir = 0;
	}

	delete[] m_raw_path;
	m_raw_path = 0;
	m_file_part = 0;
#endif
}

bool CLocalFileSystem::IsFileWritePermission(const wxString& strFullPathFileName)
{
//	int                  result      = NONFATAL;
/*    SECURITY_INFORMATION sec_info    = 0;
    unsigned int         desc_length = 0;
    unsigned char *      psec_desc   = NULL;

	sec_info |= OWNER_SECURITY_INFORMATION;
    sec_info |= GROUP_SECURITY_INFORMATION;
    sec_info |= DACL_SECURITY_INFORMATION;

	bool bSecurity = GetFileSecurity(CONVSTR(strFullPathFileName), sec_info, NULL, 0, &desc_length);
	if(!bSecurity)
		return false;

	return true;
*/
/*
#ifdef __WXMSW__
	DWORD dwAttr = GetFileAttributes(strFullPathFileName);
	DWORD dwDesiredAccess = GENERIC_READ;
	DWORD dwFlagsAndAttr = 0;

	if(dwAttr == FILE_ATTRIBUTE_DIRECTORY)
	{
		dwDesiredAccess = FILE_GENERIC_READ;
		dwFlagsAndAttr = FILE_FLAG_BACKUP_SEMANTICS;
	}

	if(dwAttr == (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_ARCHIVE))
		return true;

	HANDLE hFile = INVALID_HANDLE_VALUE;

    hFile = CreateFile(CONVSTR(strFullPathFileName),
					   dwDesiredAccess,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       dwFlagsAndAttr,
                       NULL);

	DWORD dwErr = GetLastError();
    if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		dwErr = GetLastError();
		if(dwErr == 0x0020) //다른 프로세스에서 이 파일을 사용하고 있기 때문에 이 파일을 액세스할 수 없습니다.(잘라내기가 아닌경우 체크)
			return true;

		return false;
	}

    CloseHandle(hFile);
    return true;
#else

#endif
*/
/*	std::fstream fs;
	fs.open(CONVSTR(strFullPathFileName), std::ios::app | std::ios::binary);

	if(!fs.is_open())
		return false;

	fs.close();
*/
/*
	FILE* f;
	f = fopen(strFullPathFileName.c_str(), "ra");
	if(f == nullptr)
		return false;

	fclose(f);
*/

	int iMode = R_OK | W_OK;
	int iRet = 0;
	iRet = access(strFullPathFileName.c_str(), iMode);
	if(iRet == -1)
		return false;

	return true;
//	bool bRet = bWrite ? wxIsWritable(strFullPathFileName) : wxIsReadable(strFullPathFileName);
//	return bRet;
//	return true;
}

bool CLocalFileSystem::CheckFileUseAnotherProcess(const std::list<wxString>& lstSrc, std::list<wxString>& lstOPTgt)
{
	size_t itemCount = lstSrc.size();
	for (auto const& item : lstSrc)
	{
		DWORD dwRet = GetFileAttributes(item);
		if(dwRet == FILE_ATTRIBUTE_NORMAL || dwRet == FILE_ATTRIBUTE_ARCHIVE)
		{
			bool bWritable = IsFileWritePermission(item);
			wxString strMsg(item);

			if(!bWritable)
			{
				if(itemCount == 1)
				{
					wxMessageBox(strMsg + wxT(" is opened!. you can't file operation!"), wxT("FileOperation"), wxICON_ERROR, _gMainFrame);
					return false;
				}
				else
				{
					int iRet = wxMessageBox(item + wxT(" is opend another program!. this file to be skipped! \n Continue?"), wxT("File Operation"), wxYES_NO | wxICON_ERROR, _gMainFrame);
					if (iRet == wxYES)
						continue;

					lstOPTgt.clear();
					return false;
				}
			}
		}

		lstOPTgt.push_back(item);
	}

	return lstOPTgt.size() > 0 ? true : false;
}

bool CLocalFileSystem::GetAttribute(const wxString& strFilePathName, bool* isDir, unsigned long* lattr, wxLongLong* llSize, wxDateTime* dt)
{
#ifdef __WXMSW__
	WIN32_FILE_ATTRIBUTE_DATA fileAttr = { 0, };
	BOOL result = GetFileAttributesEx(strFilePathName, GetFileExInfoStandard, &fileAttr);

	if(!result)
		return false;

	*isDir = (fileAttr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

	*lattr = fileAttr.dwFileAttributes;

	*llSize = 0;
	if(!*isDir)
		*llSize = wxLongLong(fileAttr.nFileSizeHigh, fileAttr.nFileSizeLow);

	wxLongLong t(fileAttr.ftLastWriteTime.dwHighDateTime, fileAttr.ftLastWriteTime.dwLowDateTime);
	t /= 10000; // Convert hundreds of nanoseconds to milliseconds.
	t -= EPOCH_OFFSET_IN_MSEC;

	*dt = wxDateTime(t);

#else
#endif // __WXMSW__

	return true;
}

bool CLocalFileSystem::GetStat(const wxString& strPathFileName)
{
	bool bRet = true;
#ifdef __WXMSW__
	struct _stat64 sb = {0x00, };
	if(_wstat64(CONVSTR(strPathFileName), &sb) < 0)
		return false;
#else

	struct stat sb= {0x00, };
	if(stat(CONVSTR_TO_C_STR(strPathFileName), &sb) < 0)
		return false;
#endif

	//권한체크
	int i_stmode = (sb.st_mode & S_IFMT);
	if(!i_stmode)
		return false;


	switch(i_stmode)
	{
	    case S_IFDIR: //디렉토리
        case S_IFREG: //Regular file
       	case S_IWUSR: //Owner Write
        	break;

        default:
        	return false;
	}

	return bRet;
}

bool CLocalFileSystem::CheckCreatable(const wxString& strFilePathName)
{
	wxString strDontUse(theMsg->GetMessage(wxT("MSG_INFO_RENAME_DONTUSE")));
	strDontUse.Replace(wxT(" "), wxT(""));

	wxString strRegEx(wxT("[") + strDontUse + wxT("]"));
	wxRegEx re(strRegEx);
	if(re.Matches(strFilePathName))
	{
		wxMessageBox(wxT("The '") + strFilePathName + wxT("' directory contains characters that cannot be used"), PROGRAM_FULL_NAME, wxICON_ERROR | wxOK);
		return false;
	}

	return true;
}

unsigned int CLocalFileSystem::GetFileType(const wxString& path)
{
#ifdef __WXMSW__
	DWORD result = GetFileAttributes(path);
	if (result == INVALID_FILE_ATTRIBUTES)
		return FTYPE_UNKNOWN;

//	WIN32_FILE_ATTRIBUTE_DATA fileAttr = { 0, };
//	BOOL result = GetFileAttributesEx(path, GetFileExInfoStandard, &fileAttr);

	if (result == INVALID_FILE_ATTRIBUTES)
		return FTYPE_UNKNOWN;


	const unsigned long attrubute_mapping[][2] = {
		{ FILE_ATTRIBUTE_DIRECTORY,			FTYPE_DIR     },
		{ FILE_ATTRIBUTE_REPARSE_POINT,		FTYPE_LINK    },
		{ FILE_ATTRIBUTE_READONLY,          FTYPE_FILE    },
		{ FILE_ATTRIBUTE_HIDDEN,            FTYPE_FILE    },
		{ FILE_ATTRIBUTE_SYSTEM,            FTYPE_FILE    }
	};

	for (unsigned long i = 0; i < WXSIZEOF(attrubute_mapping); ++i)
	{
		if (result & attrubute_mapping[i][0])
			return attrubute_mapping[i][1];
	}
	/*
	if (result & FILE_ATTRIBUTE_DIRECTORY)
		return FTYPE_DIR;



	if (result & FILE_ATTRIBUTE_REPARSE_POINT)
		return FTYPE_LINK;
	*/
/*
	const HANDLE h = ::CreateFile( path.t_str() ,
                                   0,
                                   FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                   NULL,
                                   OPEN_EXISTING,
                                   result & FILE_ATTRIBUTE_DIRECTORY ? FILE_FLAG_BACKUP_SEMANTICS : FILE_ATTRIBUTE_NORMAL,
                                   NULL);

    if ( h != INVALID_HANDLE_VALUE )
	{
        CloseHandle(h);
        return FTYPE_FILE;
	}
*/
	int nExist;
	int nAccess;

#ifdef _UNICODE
	nExist = _waccess(path.wchar_str(), 0);
	nAccess = _waccess(path.wchar_str(), 2);

	if(nExist == -1)
		return FTYPE_UNKNOWN;

	if(nExist == 0)
	{
		if(nAccess == -1)
			return FTYPE_UNKNOWN;

		return FTYPE_FILE;
	}
#else
	nExist = _assess(path.c_str(), 0);
	nAccess = _assess(path.c_str(), 2);

	if(nExist != -1)
		return FTYPE_UNKNOWN;

	if(nExist)
	{
		if(nAccess == -1)
			return FTYPE_UNKNOWN;

		return FTYPE_FILE;
	}
#endif

	return FTYPE_UNKNOWN;
#else
	if (path.Last() == '/' && path != _T("/"))
	{
		wxString tmp = path;
		tmp.RemoveLast();
		return GetFileType(tmp);
	}

	wxStructStat buf;
	int result = wxLstat(path, &buf);
	if (result)
		return FTYPE_UNKNOWN;

#ifdef S_ISLNK
	if (S_ISLNK(buf.st_mode))
		return FTYPE_LINK;
#endif

	if (S_ISDIR(buf.st_mode))
		return FTYPE_DIR;

	return FTYPE_FILE;
#endif
}
