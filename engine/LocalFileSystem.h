#ifndef LOCALFILESYSTEM_H_INCLUDED
#define LOCALFILESYSTEM_H_INCLUDED

#include "FileSystem.h"
class CLocalFileSystem : public CFileSystem
{
public:
	CLocalFileSystem();
	virtual ~CLocalFileSystem();

public:
	void EndFindFiles();
	bool BeginFindFiles(wxString path, bool dirs_only = false);
	bool GetNextFile(wxString& strName, bool* isDir = nullptr, unsigned long* latt = nullptr, wxLongLong* llSize = nullptr, wxDateTime* dt = nullptr);

	static bool IsFileWritePermission(const wxString& strFullPathFileName);
	static bool CheckFileUseAnotherProcess(const std::list<wxString>& lstSrc, std::list<wxString>& lstOPTgt);
	static bool GetAttribute(const wxString& strFilePathName, bool* isDir, unsigned long* lattr, wxLongLong* llSize, wxDateTime* dt);
	static bool GetStat(const wxString& strPathFileName);
	static bool CheckCreatable(const wxString& strFilePathName);
	static unsigned int GetFileType(const wxString& path);

private:
	bool m_bDirs_only;
#ifdef __WXMSW__
	#ifdef _UNICODE
		typedef WIN32_FIND_DATAW WIN32_DATA;
	#else
		typedef WIN32_FIND_DATA WIN32_DATA;
	#endif

	WIN32_DATA m_find_data;
	HANDLE m_hFind;
	bool m_bfound;
	wxString m_strFind_path;

	bool m_IsLastSlash = false;
#else
	char* m_raw_path;
	char* m_file_part; // Points into m_raw_path past the trailing slash of the path part
	int m_buffer_length;
	DIR* m_dir;
#endif
};


#endif // LOCALFILESYSTEM_H_INCLUDED
