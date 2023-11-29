#ifndef SHWFILESYSTEMWATCHERLISTENER_H_INCLUDED
#define SHWFILESYSTEMWATCHERLISTENER_H_INCLUDED

#include "IFileSystemWatcherListener.h"
class CFileSystemWatcherListener : public IFileSystemWatcherListener
{
public:
	CFileSystemWatcherListener();
	CFileSystemWatcherListener(wxWindow* pOwner);
	~CFileSystemWatcherListener();

	void SetOwner(wxWindow* pOwner) {
		m_pOwner = pOwner;
	}

	void OnFileChange(const wxString& strName) override;
	void OnFileAdded(const wxString& strName) override;
	void OnFileRemoved(const wxString& strName) override;
	void OnFileRenamed(const wxString& strOldName, const wxString& strNewName) override;

	void SetWatchPath(const wxString& strPath) { m_strPath = strPath; }
	wxString GetWatchPath() { return m_strPath; }
private:
	wxWindow* m_pOwner;
	wxString m_strPath = wxT("");
};

#endif // SHWFILESYSTEMWATCHERLISTENER_H_INCLUDED
