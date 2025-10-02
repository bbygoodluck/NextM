#include <aclapi.h>
#include <psapi.h>

#include "../ginc.h"
#include "FileVersionInfo.h"
#include "Process.h"

wxBEGIN_EVENT_TABLE(CProcess, CSystemInterface)
wxEND_EVENT_TABLE()

CProcess::CProcess()
{
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CProcess::OnTimer));
	m_timer.SetOwner(this, wxTIMER_ID++);
}

CProcess::~CProcess()
{
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(CProcess::OnTimer));
}

void CProcess::Clear()
{
	std::unordered_map<unsigned long, PROCESS_INFO *>::iterator fIter;
	for (fIter = m_mapProcessList.begin(); fIter != m_mapProcessList.end(); ++fIter)
	{
		PROCESS_INFO* pProcessInfo = fIter->second;
		wxDELETE(pProcessInfo);
	}

	m_mapProcessList.clear();
}

void CProcess::Init()
{
#ifdef __WXMSW__
	numProcessors = (unsigned int)theSystem->CPU()->GetCoreCount();
	SetPrivilige(SE_DEBUG_NAME);

	HANDLE hServer = WTS_CURRENT_SERVER_HANDLE;    // get local machine processes
    unsigned long ulLevel = 1;                     // get array of WTS_PROCESS_INFO_EX
    unsigned long ulCount = 0;                     // returns the number of processes
    PWTS_PROCESS_INFO pProcessInfo = nullptr;      // output data

	if(WTSEnumerateProcesses(hServer, 0, ulLevel, &pProcessInfo, &ulCount))
	{
		for(unsigned long i = 0; i < ulCount; i++)
			AddProcessInfo(pProcessInfo[i]);
	}

    WTSFreeMemory(pProcessInfo);
	pProcessInfo = nullptr;
#endif
}

void CProcess::AddProcessInfo(unsigned long ulProcessID, const wxString& _strProcessName)
{
	wxString strProcessName(_strProcessName);
	std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator fIter = m_mapProcessList.find(ulProcessID);
	if (fIter != m_mapProcessList.end())
		return;

	PROCESS_INFO *pPSInfo = new PROCESS_INFO();
	pPSInfo->_ulProcessID = ulProcessID;

	if(ulProcessID == 0)
		strProcessName = wxT("System Idle Process");

	pPSInfo->_strProcessName = strProcessName;
	pPSInfo->_hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ, FALSE, ulProcessID);
	pPSInfo->_strProcessFullPath = wxT("");

	DWORD dwLen = 0;
	TCHAR szImagePath[MAX_PATH] = { 0, };

	ZeroMemory(szImagePath, sizeof(szImagePath));
	dwLen = sizeof(szImagePath) / sizeof(TCHAR);
	if(QueryFullProcessImageName(pPSInfo->_hProcess, 0, szImagePath, &dwLen))
		pPSInfo->_strProcessFullPath = szImagePath;

	SetUserNameAndDomainNameFromSid(pPSInfo);
	theImageList->GetIconIndex(pPSInfo->_strProcessFullPath, pPSInfo->iIconIndex, pPSInfo->iOvelayIndex);

	wxString strExeFile(pPSInfo->_strProcessFullPath);
	CFileVersionInfo data;
	if(data.Create(strExeFile))
	{
		pPSInfo->_strFileDescription = data.GetFileDescription();
		pPSInfo->_strCompanyName = data.GetCompanyName();
	}

//	GetCommandLineOfProcess(ulProcessID, pPSInfo->_strCommandLine);
	InitProcessCPUTime(pPSInfo);

	std::unordered_map<unsigned long, PROCESS_INFO *>::value_type val(ulProcessID, pPSInfo);
	m_mapProcessList.insert(val);
}

void CProcess::DeleteProcess(unsigned long ulProcessID)
{
	std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator fIter = m_mapProcessList.find(ulProcessID);
	if (fIter == m_mapProcessList.end())
		return;

	PROCESS_INFO* pProcessInfo = fIter->second;
	wxDELETE(pProcessInfo);

	m_mapProcessList.erase(fIter);
}

#ifdef __WXMSW__
void CProcess::AddProcessInfo(const WTS_PROCESS_INFO& wtsProcessInfo, bool bNewProcess)
{
	wxString strProcessName(wtsProcessInfo.pProcessName);
	AddProcessInfo(wtsProcessInfo.ProcessId, strProcessName);
}

bool CProcess::SetPrivilige(LPCTSTR lpszPrivilege, bool bEnablePrivilege)
{
	HANDLE hToken = 0;
	if (!OpenProcessToken(GetCurrentProcess(),
						  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
						  &hToken))
	{
		wxMessageBox(wxString::Format(wxT("OpenProcessToken function error=%0x"), GetLastError()), PROGRAM_FULL_NAME,  wxOK | wxICON_ERROR);
        return false;
	}

	TOKEN_PRIVILEGES tp;
	// Used by local system to identify the privilege
	LUID luid;

	if(!LookupPrivilegeValue(NULL,          // lookup privilege on local system
							lpszPrivilege,  // privilege to lookup
							&luid))         // receives LUID of privilege

	{
		wxMessageBox(wxString::Format(wxT("LLookupPrivilegeValue() failed, error : %0x\n"), GetLastError()), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return false;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;

	// If TRUE, enable
	if(bEnablePrivilege) tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else tp.Privileges[0].Attributes = 0; // else, disable

	if(!AdjustTokenPrivileges(hToken,
							  FALSE,
							  &tp,
							  sizeof(TOKEN_PRIVILEGES),
							  (PTOKEN_PRIVILEGES) NULL,
							  (PDWORD) NULL))
	{
		wxMessageBox(wxString::Format(wxT("AdjustTokenPrivileges() failed, error : %0x\n"), GetLastError()), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return false;
	}

	return true;
}
int CProcess::SetUserNameAndDomainNameFromSid(PROCESS_INFO* pInfo)
{
	DWORD dwRtnCode = 0;
	DWORD dwErrorCode = 0;
	BOOL bRtnBool = TRUE;
	PSID pSidOwner = NULL;
	DWORD dwAcctName = 1, dwDomainName = 1;
	SID_NAME_USE eUse = SidTypeUnknown;
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD = NULL;

	// Get the handle of the file object.
	hFile = CreateFile(
                  pInfo->_strProcessFullPath,
                  GENERIC_READ,
                  FILE_SHARE_READ,
                  NULL,
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL,
                  NULL);

	// Check GetLastError for CreateFile error code.
	if (hFile == INVALID_HANDLE_VALUE)
	{
		dwErrorCode = GetLastError();
		return -1;
	}

	// Get the owner SID of the file.
	dwRtnCode = GetSecurityInfo(
                  hFile,
                  SE_FILE_OBJECT,
                  OWNER_SECURITY_INFORMATION,
                  &pSidOwner,
                  NULL,
                  NULL,
                  NULL,
                  &pSD);

	CloseHandle(hFile);
	// Check GetLastError for GetSecurityInfo error condition.
	if (dwRtnCode != ERROR_SUCCESS)
	{
		dwErrorCode = GetLastError();
		return -1;
	}

	TCHAR AcctName[255] = {0x00, };
	TCHAR DomainName[255] = {0x00, };
	// First call to LookupAccountSid to get the buffer sizes.
	bRtnBool = LookupAccountSid(
                  NULL,           // local computer
                  pSidOwner,
                  AcctName,
                  (LPDWORD)&dwAcctName,
                  DomainName,
                  (LPDWORD)&dwDomainName,
                  &eUse);


    // Second call to LookupAccountSid to get the account name.
    bRtnBool = LookupAccountSid(
          NULL,                   // name of local or remote computer
          pSidOwner,              // security identifier
          AcctName,               // account name buffer
          (LPDWORD)&dwAcctName,   // size of account name buffer
          DomainName,             // domain name
          (LPDWORD)&dwDomainName, // size of domain name buffer
          &eUse);                 // SID type

    // Check GetLastError for LookupAccountSid error condition.
    if (bRtnBool == FALSE)
	{
		dwErrorCode = GetLastError();
		if( dwErrorCode == ERROR_NONE_MAPPED )
			pInfo->_strDomainAndUser = wxT("NONE_MAPPED");

		GetUserNameFromToken(pInfo);
		return -1;
	}

	pInfo->_strUserName = wxString::Format(TEXT("%s"), AcctName);
	pInfo->_strDomainName = wxString::Format(TEXT("%s"), DomainName);
	pInfo->_strDomainAndUser = wxString::Format(TEXT("%s\\%s"), DomainName, AcctName);

	return 0;
}

void CProcess::GetUserNameFromToken(PROCESS_INFO* pInfo)
{
	HANDLE tok = 0;
	TOKEN_USER* ptu;
	wchar_t name[300], dom[300], tubuf[300];
	DWORD nlen, dlen;
	int iUse;
	if(!OpenProcessToken(pInfo->_hProcess, TOKEN_QUERY, &tok))
		return;

	ptu = (TOKEN_USER *)tubuf;
	if(!GetTokenInformation(tok, (TOKEN_INFORMATION_CLASS)1, ptu, 300, &nlen))
		return;

	dlen = 300;
	nlen = 300;
	if(!LookupAccountSid(0, ptu->User.Sid, name, &nlen, dom, &dlen, (PSID_NAME_USE)&iUse))
		return;

	pInfo->_strUserName = wxString::Format(TEXT("%s"), name);
	pInfo->_strDomainName = wxString::Format(TEXT("%s"), dom);
	pInfo->_strDomainAndUser = wxString::Format(TEXT("%s\\%s"), dom, name);
}
#else
#endif

//Process CPU
void CProcess::InitProcessCPUTime(PROCESS_INFO* _pPsInfo)
{
	//cpu 사용시간
	FILETIME ftime, fsys, fuser;
	//System시간을 읽어옴
	GetSystemTimeAsFileTime(&ftime);
	memcpy(&_pPsInfo->_ftLastCPU, &ftime, sizeof(FILETIME));

	//오픈한 프로세스에 대한 사용시간 읽어옴
	GetProcessTimes(_pPsInfo->_hProcess, &ftime, &ftime, &fsys, &fuser);

	memcpy(&_pPsInfo->_ftLastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&_pPsInfo->_ftLastUserCPU, &fuser, sizeof(FILETIME));
}

void CProcess::UpdateProcessCPUTime(PROCESS_INFO* _pPsInfo)
{
	//lamda function
	auto SubSysTimes = [](const FILETIME ft1, const FILETIME ft2) {
		ULONGLONG a,b = 0;

		memcpy( &a, &ft1, sizeof (ULONGLONG) );
		memcpy( &b, &ft2, sizeof (ULONGLONG) );

		return a - b;
	};

	FILETIME ftime, fsys, fuser;
	FILETIME ftNow;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&ftNow, &ftime, sizeof(FILETIME));

	GetProcessTimes(_pPsInfo->_hProcess, &ftime, &ftime, &fsys, &fuser);

	ULONGLONG ullSysDiff = SubSysTimes(fsys, _pPsInfo->_ftLastSysCPU);
	ULONGLONG ullUsrDiff = SubSysTimes(fuser, _pPsInfo->_ftLastUserCPU);
	ULONGLONG ullNowDiff = SubSysTimes(ftNow, _pPsInfo->_ftLastCPU);
	ULONGLONG ullTotal   = ullSysDiff + ullUsrDiff;

	float fPercent = float( ullTotal ) / float( ullNowDiff );
	fPercent /= numProcessors;
	fPercent *= 100.0f;

	_pPsInfo->_ftLastCPU     = ftNow;
	_pPsInfo->_ftLastUserCPU = fuser;
	_pPsInfo->_ftLastSysCPU  = fsys;
	_pPsInfo->fCPUUsage      = fPercent;
}
//Process Memory
void CProcess::UpdateProcessMemory(PROCESS_INFO* _pPsInfo)
{
#ifdef __WXMSW__
	PROCESS_MEMORY_COUNTERS_EX pmc;
	BOOL bInfo = GetProcessMemoryInfo(_pPsInfo->_hProcess, (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));

	if (bInfo)
	{
		_pPsInfo->_PrivateSize = pmc.PrivateUsage;
		_pPsInfo->_WorkingSetSize = pmc.WorkingSetSize;
	}
#else

#endif
}

PROCESS_INFO* CProcess::UpdateProcessInfo(unsigned long _ulProcessId)
{
	std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator fIter = m_mapProcessList.find(_ulProcessId);
	if (fIter == m_mapProcessList.end())
		return nullptr;

	PROCESS_INFO* pProcessInfo = fIter->second;
	UpdateProcessCPUTime(pProcessInfo);
	UpdateProcessMemory(pProcessInfo);

	return pProcessInfo;
}

PROCESS_INFO* CProcess::GetInfo(unsigned long ulProcessID)
{
	std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator fIter = m_mapProcessList.find(ulProcessID);
	if (fIter == m_mapProcessList.end())
		return nullptr;

	PROCESS_INFO* pProcessInfo = fIter->second;
	if (pProcessInfo == nullptr)
		return nullptr;

	return pProcessInfo;
}

std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator CProcess::Begin()
{
	return m_mapProcessList.begin();
}

std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator CProcess::End()
{
	return m_mapProcessList.end();
}

void CProcess::OnTimer(wxTimerEvent& event)
{
	SendEvent();
}

void CProcess::Update()
{

}

int CProcess::KillProcess(unsigned long ulProcessID)
{
	//프로세스 항목이 존재하는지 검사만 한다.
	//프로세스가 종료되고 나면 DelProcess() 함수를 CWatchProcess 에서 호출하게 되므로
	//PROCESS_INFO의 _hProcess Handle의 Close는 DelProcess에서 처리
	PROCESS_INFO* pProcess = GetInfo(ulProcessID);
	if(pProcess == nullptr)
		return KILL_PROCESS_MSG_NOT_PROCESSID;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ulProcessID);

	bool bTerminate = ::TerminateProcess(hProcess, 0);
	if(!bTerminate)
		return KILL_PROCESS_MSG_PROCESS_TERMINATE_FAIL;

	unsigned long ulRetCode = 0;
	GetExitCodeProcess(hProcess, &ulRetCode);

	if (ulRetCode == STILL_ACTIVE)
		return KILL_PROCESS_MSG_PROCESS_ALIVE;

	CloseHandle(hProcess);
	return KILL_PROCESS_MSG_SUCCESS;
}
