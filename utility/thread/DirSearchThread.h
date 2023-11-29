#ifndef __DIR_SEARCH_THREAD_H__
#define __DIR_SEARCH_THREAD_H__

class DlgSearchDir;
class CDirSearchThread : public wxThread
{
public:
	CDirSearchThread(DlgSearchDir* pDlg);
	~CDirSearchThread();

	virtual ExitCode Entry() wxOVERRIDE;

public:
	void SetSearchCurrentDirectory(const wxString& strDir, const wxString& strFile);

private:
	void DoSearchDir(const wxString& strPath, int depth = 0);


private:
	DlgSearchDir * m_pDlg;
	
	wxString m_strDrive;
	wxString m_strDir;
	wxString m_strFile;
	wxString m_strSave;

	int m_depth = 0;
	int m_dirCount = 0;
	bool m_bCancel = false;

	wxFile m_file;
#if defined(__WXMSW__)
	WIN32_FIND_DATA m_find_data;
	HANDLE m_hFind;
	wxString m_strFind_path;
#else
	char* m_raw_path;
	char* m_file_part; // Points into m_raw_path past the trailing slash of the path part
	int m_buffer_length;
	DIR* m_dir;
#endif
};
#endif
