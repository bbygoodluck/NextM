#ifndef __MAKE_TREE_INFO_THREAD_H__
#define __MAKE_TREE_INFO_THREAD_H__

#include <wx/textfile.h>
#include <filesystem>

using namespace std::filesystem;

class DlgMakeNCDInfo;
class CNCDInfoThread : public wxThread
{
public:
	CNCDInfoThread(DlgMakeNCDInfo* pDlg);
	~CNCDInfoThread();

	virtual wxThread::ExitCode Entry() wxOVERRIDE;

	void SetLoadFromFile(bool bLoadFromFile) {
		m_bLoadFromFile = bLoadFromFile;
	}

	void SetNCDFile(const wxString& strFile, const wxString& strPath, size_t lineCount = 0);
	void SetSubDirInfo(DirNode* pNode) {
		m_pCurrNode = pNode;
	}

protected:
	void LoadSubDir(const wxString& strPath, DirNode* pNode);

private:
	void SendThreadTerminateMessage();
	bool IsCancelOrClose();
	int GetNumberofDirectory(path p);

private:
	DlgMakeNCDInfo* m_pDlg;
	wxString m_strNCDFile;
	wxString m_strCurrentPath;
	size_t m_lineCnt;

	bool m_bLoadFromFile;
	int m_iCurDepth = 0;
	int m_iCurRow = 0;
//	int m_iCount;
	DirNode* m_pCurrNode;
};
#endif
