#ifndef MSWFILESYSTEMWATCHER_H_INCLUDED
#define MSWFILESYSTEMWATCHER_H_INCLUDED

#include "../FileSystemWatcherBase.h"
class CLocalFileSystemWatcher : public CFileSystemWatcherBase
{
public:
	CLocalFileSystemWatcher();
	CLocalFileSystemWatcher(wxWindow* pOwner);
	~CLocalFileSystemWatcher();

	void Clear() override;
	bool StartWatch(const wxString& strPath) override;

	wxThread::ExitCode Entry() override;

private:
	HANDLE m_hFile = NULL;
	OVERLAPPED	m_PollingOverlap;
	HANDLE		m_hIOCP = NULL;
	ULONG_PTR   m_CompletionKey = 0;

	unsigned long m_ulNotifyFilter;

	char m_buffer[4096] = {0x00, };
};

#endif // MSWFILESYSTEMWATCHER_H_INCLUDED
