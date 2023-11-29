#include "../../ginc.h"
#include "LocalFileSystemWatcher.h"

CLocalFileSystemWatcher::CLocalFileSystemWatcher()
{
	m_ulNotifyFilter = FILE_NOTIFY_CHANGE_SECURITY    |
		               FILE_NOTIFY_CHANGE_CREATION    |
		               FILE_NOTIFY_CHANGE_LAST_ACCESS |
		               FILE_NOTIFY_CHANGE_LAST_WRITE  |
		               FILE_NOTIFY_CHANGE_SIZE        |
		               FILE_NOTIFY_CHANGE_ATTRIBUTES  |
		               FILE_NOTIFY_CHANGE_DIR_NAME    |
		               FILE_NOTIFY_CHANGE_FILE_NAME;

	wxZeroMemory(m_PollingOverlap);
	wxZeroMemory(m_buffer);
}

CLocalFileSystemWatcher::CLocalFileSystemWatcher(wxWindow* pOwner)
	: CFileSystemWatcherBase(pOwner)
{
	m_ulNotifyFilter = FILE_NOTIFY_CHANGE_SECURITY    |
		               FILE_NOTIFY_CHANGE_CREATION    |
		               FILE_NOTIFY_CHANGE_LAST_ACCESS |
		               FILE_NOTIFY_CHANGE_LAST_WRITE  |
		               FILE_NOTIFY_CHANGE_SIZE        |
		               FILE_NOTIFY_CHANGE_ATTRIBUTES  |
		               FILE_NOTIFY_CHANGE_DIR_NAME    |
		               FILE_NOTIFY_CHANGE_FILE_NAME;

	wxZeroMemory(m_PollingOverlap);
	wxZeroMemory(m_buffer);
}

CLocalFileSystemWatcher::~CLocalFileSystemWatcher()
{
	Clear();
}

void CLocalFileSystemWatcher::Clear()
{
	m_bRemove = true;

	if (m_hIOCP != INVALID_HANDLE_VALUE)
	{
		PostQueuedCompletionStatus(m_hIOCP, 0, (ULONG_PTR)NULL, NULL);
		if (GetThread() && GetThread()->IsRunning())
			GetThread()->Wait();

		CloseHandle(m_hIOCP);
		m_hIOCP = INVALID_HANDLE_VALUE;
	}

	CancelIo(m_hFile);
	CloseHandle(m_hFile);

	m_hFile = NULL;
	m_bRemove = false;
}

bool CLocalFileSystemWatcher::StartWatch(const wxString& strPath)
{
	Clear();

	m_strPath = strPath;
	m_pListener->SetWatchPath(strPath);

	//디렉토리 감시를 비동기로 변경함.
	if (!(m_hIOCP = CreateIoCompletionPort((HANDLE)INVALID_HANDLE_VALUE, NULL, 0, 0)))
		return false;

	m_hFile = CreateFile(strPath.c_str(),                          // pointer to the file name
		FILE_LIST_DIRECTORY,                                       // access (read/write) mode
		FILE_SHARE_READ | FILE_SHARE_WRITE |FILE_SHARE_DELETE,     // share mode
		NULL,                                                      // security descriptor
		OPEN_EXISTING,                                             // how to create
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,         // file attributes
		NULL);                                                     // file with attributes to copy

	if(m_hFile == INVALID_HANDLE_VALUE)
		return false;

	m_PollingOverlap.OffsetHigh = 0;
	m_CompletionKey = (ULONG_PTR)m_hFile;
	if (CreateIoCompletionPort(m_hFile, m_hIOCP, m_CompletionKey, 0) == NULL)
	{
		CloseHandle(m_hFile);
		return false;
	}

	RunWatchThread();
	return true;
}

wxThread::ExitCode CLocalFileSystemWatcher::Entry()
{
	const int bufferSize = MAX_PATH * 4;
	TCHAR filename[bufferSize] = { 0, };
	DWORD dwBytesXFered = 0;
	DWORD dwBytesReturned = 0;

	while (true)
	{
		ReadDirectoryChangesW(
							m_hFile,                  // handle to directory
							m_buffer,                 // read results buffer
							BUFFER_SIZE,              // length of buffer
							FALSE,                    // monitoring option
							m_ulNotifyFilter,         // filter conditions
							&dwBytesReturned,         // bytes returned
							&m_PollingOverlap,        // overlapped buffer
							NULL);                    // completion routine

		bool bResult = GetQueuedCompletionStatus(m_hIOCP, &dwBytesXFered, &m_CompletionKey, (LPOVERLAPPED* )&(m_PollingOverlap), INFINITE);
		if (!bResult && dwBytesXFered == 0)
			break;

		if (m_bRemove)
			break;

		PFILE_NOTIFY_INFORMATION pNotify = (PFILE_NOTIFY_INFORMATION)m_buffer;

		DWORD dwOffset = 0;
		wxString strOldName(wxT(""));
		wxString strNewName(wxT(""));

		do
		{
			dwOffset = pNotify->NextEntryOffset;

			wxZeroMemory(filename);
			wcsncpy(filename, pNotify->FileName, pNotify->FileNameLength / 2);

			filename[pNotify->FileNameLength / 2] = 0x00;
			wxString strFileName(filename);

			switch (pNotify->Action)
			{
				case FILE_ACTION_MODIFIED:
					m_pListener->OnFileChange(strFileName);
					break;

				case FILE_ACTION_ADDED:
					m_pListener->OnFileAdded(strFileName);
					break;

				case FILE_ACTION_REMOVED:
					m_pListener->OnFileRemoved(strFileName);
					break;

				case FILE_ACTION_RENAMED_OLD_NAME:
					strOldName = strFileName;
					break;

				case FILE_ACTION_RENAMED_NEW_NAME:
					m_pListener->OnFileRenamed(strOldName, strFileName);
					break;

				default:
					break;
			}

			pNotify = (PFILE_NOTIFY_INFORMATION)((LPBYTE)pNotify + dwOffset);
		} while(dwOffset);
	}

	return (wxThread::ExitCode)0;
}
