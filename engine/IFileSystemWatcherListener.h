#ifndef ISHWFILEWATCHERLISTENER_H_INCLUDED
#define ISHWFILEWATCHERLISTENER_H_INCLUDED

class IFileSystemWatcherListener
{
public:
	virtual void OnFileChange(const wxString& strName) = 0;
	virtual void OnFileAdded(const wxString& strName) = 0;
	virtual void OnFileRemoved(const wxString& strName) = 0;
	virtual void OnFileRenamed(const wxString& strOldName, const wxString& strNewName) = 0;
};

#endif // ISHWFILEWATCHERLISTENER_H_INCLUDED
