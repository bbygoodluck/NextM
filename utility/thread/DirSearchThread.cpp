#include "../../ginc.h"
#include "../../dialog/DlgSearchDir.h"
#include "DirSearchThread.h"
#include "../../engine/LocalFileSystem.h"

CDirSearchThread::CDirSearchThread(DlgSearchDir* pDlg)
	: wxThread(wxTHREAD_JOINABLE)
	, m_pDlg(pDlg)
{
}

CDirSearchThread::~CDirSearchThread()
{

}

void CDirSearchThread::SetSearchCurrentDirectory(const wxString& strDir, const wxString& strFile)
{
	m_strDir = strDir;
	m_strFile = strFile;

	m_strDrive = strDir.Left(1);
	m_strDrive += wxT(":");
	m_strDrive += SLASH;
	m_pDlg->SetCurrentDrive(m_strDrive);
}

wxThread::ExitCode CDirSearchThread::Entry()
{
	wxMilliSleep(50);
	m_depth = 0;
	m_dirCount = 0;

	if (!wxFileName::FileExists(m_strFile))
	{
		if (!m_file.Create(m_strFile))
		{
			wxMessageBox(wxT("Failed to create file!"), wxT("Search Directory"), wxOK | wxICON_ERROR);
			return (wxThread::ExitCode)0;
		}
	}

	m_file.Open(m_strFile, wxFile::OpenMode::read_write);

	m_strSave = wxString::Format(wxT("%d^"), m_depth);
	m_strSave += m_strDrive;
	m_strSave += wxT("\n");

	const wxWX2MBbuf tmp_buf = wxConvCurrent->cWX2MB(m_strSave);
	const char *tmp_str = (const char*) tmp_buf;

	m_file.Write(tmp_str, strlen(tmp_str));

	DoSearchDir(m_strDrive, 0);

	m_file.Close();

	wxThreadEvent event(wxEVT_THREAD, SCD_DIRECTORY_SEARCH_THREAD_EVENT);
	event.SetInt(-1);
	wxQueueEvent(m_pDlg, event.Clone());

	return (wxThread::ExitCode)0;
}

#include <wx/strconv.h>

void CDirSearchThread::DoSearchDir(const wxString& strPath, int depth)
{
#if defined(__WXMSW__)
	CLocalFileSystem localSys;
	bool isDir = false;
	unsigned long lattr = 0;

	if (!localSys.BeginFindFiles(strPath, true))
		return;

	int iLen = strPath.Len();
	wxString strNewPath = strPath.Right(1) == SLASH ? strPath.Left(iLen - 1) : strPath;
	wxString strName(wxT(""));

	++depth;

	while (localSys.GetNextFile(strName, &isDir, &lattr))
	{
		m_pDlg->SetCurrentDir(strPath);

		if (m_pDlg->IsCancel())
			break;

		if (std::find(_gVecIgnore.begin(), _gVecIgnore.end(), strName.ToStdString()) != _gVecIgnore.end())
			continue;

		if(lattr & ATTR_HIDDEN)
			continue;

		m_strSave = wxString::Format(wxT("%d^"), depth) + strName + wxT("\n");

		const wxWX2MBbuf tmp_buf = wxConvCurrent->cWX2MB(m_strSave);
		const char *tmp_str = (const char*) tmp_buf;

		m_file.Write(tmp_str, strlen(tmp_str));

		wxString strDirCount = wxString::Format(wxT("%d"), ++m_dirCount);
		m_pDlg->SetReadDirectory(m_dirCount, strName);// strFullPath);

		wxString strFullPath(strNewPath);
		strFullPath += SLASH + strName;
		//읽을 수 있는 디렉토리 && 디렉토리 존재
		if(wxFileName::IsDirReadable(strFullPath) && wxDirExists(strFullPath))
			DoSearchDir(strFullPath, depth);
	}
}
#else
#endif
