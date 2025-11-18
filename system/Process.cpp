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
#ifdef __WXMSW__
	m_wmi.UnInitialize();
#else
#endif // __WXMSW__
}

void CProcess::Free()
{
	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::iterator fIter;
	for (fIter = m_mapProcessList.begin(); fIter != m_mapProcessList.end(); ++fIter)
	{
		Proc::PROCESS_INFO* pProcessInfo = fIter->second;
		wxDELETE(pProcessInfo);
	}

	m_mapProcessList.clear();

#ifdef __WXMSW__
	m_wmi.UnRegisterEventWindow();
#endif // __WXMSW__
}

void CProcess::Init()
{
#ifdef __WXMSW__
	Free();

	numProcessors = (unsigned int)theSystem->CPU()->GetCoreCount();
	SetPrivilige(SE_DEBUG_NAME);

	if(!m_wmi.IsInitialize())
		m_wmi.Initialize();

	//프로세스 CPU 시간 측정을 위한 Kernal시간 초기화
//	InitKernalTime();

	IEnumWbemClassObject* pEnumerator = m_wmi.Collect();
	if(pEnumerator)
		AddProcessFromEnum(pEnumerator);
#endif
}

CProcess* CProcess::AddProcess(unsigned long ulProcessID)
{
#ifdef __WXMSW__
	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator fIter = m_mapProcessList.find(ulProcessID);
	if (fIter != m_mapProcessList.end())
		return this;

	IEnumWbemClassObject* pEnumerator = m_wmi.Collect(ulProcessID);
	if(pEnumerator)
		AddProcessFromEnum(pEnumerator);
#else

#endif

	return this;
}

void CProcess::DeleteProcess(unsigned long ulProcessID)
{
	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator fIter = m_mapProcessList.find(ulProcessID);
	if (fIter == m_mapProcessList.end())
		return;

	Proc::PROCESS_INFO* pProcessInfo = fIter->second;
	wxDELETE(pProcessInfo);

	m_mapProcessList.erase(fIter);
}

#ifdef __WXMSW__
void CProcess::AddProcessFromEnum(IEnumWbemClassObject* pWbemClsObj)
{
	ULONG uReturn = 0;
	IWbemClassObject* pclsObj = nullptr;

	while(pWbemClsObj)
	{
		pWbemClsObj->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if(0 == uReturn)
			break;

		AddProcess(pclsObj);
		pclsObj->Release();
	}
}

CProcess* CProcess::AddProcess(IWbemClassObject* pObj)
{
	VARIANT vtProcessName;
	VariantInit(&vtProcessName);

	VARIANT vtProcessId;
	VariantInit(&vtProcessId);

	VARIANT vtDescription;
	VariantInit(&vtDescription);

	VARIANT vtExecutablePath;
	VariantInit(&vtExecutablePath);

	VARIANT vtCommandLine;
	VariantInit(&vtCommandLine);

	VARIANT vtPrivateMemory;
	VariantInit(&vtPrivateMemory);

	VARIANT vtThreadCount;
	VariantInit(&vtThreadCount);

	VARIANT vtProcessorTime;
	VariantInit(&vtProcessorTime);

	VARIANT vtUserModeTime;
	VariantInit(&vtUserModeTime);

	VARIANT vtKernelModeTime;
	VariantInit(&vtKernelModeTime);

	VARIANT vtReadTransferCount;
	VariantInit(&vtReadTransferCount);

	VARIANT vtWriteTransferCount;
	VariantInit(&vtWriteTransferCount);

	if(pObj->Get(procQuery.ProcessID, 0, &vtProcessId, nullptr, nullptr) != S_OK)
		return this;

	unsigned long ulProcessId = (unsigned long)vtProcessId.lVal;

	Proc::PROCESS_INFO *pPSInfo = new Proc::PROCESS_INFO();
	pPSInfo->_ulProcessID = ulProcessId;
	pPSInfo->_hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ, FALSE, ulProcessId);

	pPSInfo->m_lRunCount = 0;
	pPSInfo->m_dwLastRun = 0;

	wxZeroMemory(pPSInfo->m_ftPrevSysKernel);
    wxZeroMemory(pPSInfo->m_ftPrevSysUser);
    wxZeroMemory(pPSInfo->m_ftPrevProcKernel);
    wxZeroMemory(pPSInfo->m_ftPrevProcUser);

	if(pObj->Get(procQuery.Name, 0, &vtProcessName, nullptr, nullptr) == S_OK)
		pPSInfo->_strProcessName = wxString(vtProcessName.bstrVal);

	if(pObj->Get(procQuery.Description, 0, &vtDescription, nullptr, nullptr) == S_OK)
		pPSInfo->_strFileDescription = wxString(vtProcessName.bstrVal);

	if(pObj->Get(procQuery.CommandLine, 0, &vtCommandLine, nullptr, nullptr) == S_OK)
		pPSInfo->_strCommandLine = wxString(vtCommandLine.bstrVal);

	if(pObj->Get(procQuery.ExecutablePath, 0, &vtExecutablePath, nullptr, nullptr) == S_OK)
		pPSInfo->_strProcessFullPath = wxString(vtExecutablePath.bstrVal);

	if(pObj->Get(procQuery.WorkingSet, 0, &vtPrivateMemory, nullptr, nullptr) == S_OK)
	{
		wxString strMem = wxString(vtPrivateMemory.bstrVal);
		strMem.ToULongLong(&pPSInfo->_PrivateSize);
	}

	if(pObj->Get(procQuery.ThreadCount, 0, &vtThreadCount, nullptr, nullptr) == S_OK)
		pPSInfo->_threadsCount = (unsigned int)vtThreadCount.intVal;

	if(pObj->Get(procQuery.UserModeTime, 0, &vtUserModeTime, nullptr, nullptr) == S_OK)
		pPSInfo->_strUserModeTime = wxString(vtUserModeTime.bstrVal);

	if(pObj->Get(procQuery.KernelModeTime, 0, &vtKernelModeTime, nullptr, nullptr) == S_OK)
		pPSInfo->_strKernelModeTime = wxString(vtKernelModeTime.bstrVal);

	if(pObj->Get(procQuery.ReadTransferCount, 0, &vtReadTransferCount, nullptr, nullptr) == S_OK)
		pPSInfo->_readTransferCount = vtReadTransferCount.lVal;

	if(pObj->Get(procQuery.WriteTransferCount, 0, &vtWriteTransferCount, nullptr, nullptr) == S_OK)
		pPSInfo->_writeTransferCount = vtWriteTransferCount.llVal;

	SetUserNameAndDomainNameFromSid(pPSInfo);
	theImageList->GetIconIndex(pPSInfo->_strProcessFullPath, pPSInfo->iIconIndex, pPSInfo->iOvelayIndex);

	wxString strExeFile(pPSInfo->_strProcessFullPath);
	CFileVersionInfo data;
	if(data.Create(strExeFile))
	{
		pPSInfo->_strFileDescription = data.GetFileDescription();
		pPSInfo->_strCompanyName = data.GetCompanyName();
	}

	VariantClear(&vtProcessId);
	VariantClear(&vtProcessName);
	VariantClear(&vtDescription);
	VariantClear(&vtExecutablePath);
	VariantClear(&vtCommandLine);
	VariantClear(&vtPrivateMemory);
	VariantClear(&vtThreadCount);
	VariantClear(&vtUserModeTime);
	VariantClear(&vtKernelModeTime);
	VariantClear(&vtReadTransferCount);
	VariantClear(&vtWriteTransferCount);

	InitProcessCPUTime(pPSInfo);

	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::value_type val(ulProcessId, pPSInfo);
	m_mapProcessList.insert(val);

	return this;
}

//void CProcess::AddProcessInfo(const WTS_PROCESS_INFO& wtsProcessInfo, bool bNewProcess)
//{
//	wxString strProcessName(wtsProcessInfo.pProcessName);
//	AddProcessInfo(wtsProcessInfo.ProcessId, strProcessName);
//}

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
int CProcess::SetUserNameAndDomainNameFromSid(Proc::PROCESS_INFO* pInfo)
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

void CProcess::GetUserNameFromToken(Proc::PROCESS_INFO* pInfo)
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
void CProcess::InitProcessCPUTime(Proc::PROCESS_INFO* _pPsInfo)
{
//	//cpu 사용시간
//	FILETIME ftime, fsys, fuser;
//	//System시간을 읽어옴
//	GetSystemTimeAsFileTime(&ftime);
//	memcpy(&_pPsInfo->_lastCPU, &ftime, sizeof(FILETIME));
//
//	//오픈한 프로세스에 대한 사용시간 읽어옴
//	GetProcessTimes(_pPsInfo->_hProcess, &ftime, &ftime, &fsys, &fuser);
//
//	memcpy(&_pPsInfo->_lastSysCPU, &fsys, sizeof(FILETIME));
//	memcpy(&_pPsInfo->_lastUserCPU, &fuser, sizeof(FILETIME));
//
//	_pPsInfo->fCPUUsage = 0.0f;

	_pPsInfo->fCPUUsage = GetProcessCPUUsage(_pPsInfo);
}

void CProcess::UpdateProcessCPUTime(Proc::PROCESS_INFO* _pPsInfo)
{
//	FILETIME ftime, fsys, fuser;
//	FILETIME ftNow;
//
//	GetSystemTimeAsFileTime(&ftime);
//	memcpy(&ftNow, &ftime, sizeof(FILETIME));
//
//	GetProcessTimes(_pPsInfo->_hProcess, &ftime, &ftime, &fsys, &fuser);
//
//	ULONGLONG ullSysDiff = theSystem->CPU()->SubtractTimes(fsys, _pPsInfo->_ftLastSysCPU);
//	ULONGLONG ullUsrDiff = theSystem->CPU()->SubtractTimes(fuser, _pPsInfo->_ftLastUserCPU);
//	ULONGLONG ullNowDiff = theSystem->CPU()->SubtractTimes(ftNow, _pPsInfo->_ftLastCPU);
//	ULONGLONG ullTotal   = ullSysDiff + ullUsrDiff;
//
//	float fPercent = float( ullTotal ) / float( ullNowDiff );
//	fPercent /= numProcessors;
//	fPercent *= 100.0f;
//
//	_pPsInfo->_ftLastCPU     = ftNow;
//	_pPsInfo->_ftLastUserCPU = fuser;
//	_pPsInfo->_ftLastSysCPU  = fsys;
//	_pPsInfo->fCPUUsage      = fPercent;

	_pPsInfo->fCPUUsage = GetProcessCPUUsage(_pPsInfo);
}

float CProcess::GetProcessCPUUsage(Proc::PROCESS_INFO* _pPsInfo)
{
	float fCpuUsage = _pPsInfo->fCPUUsage;
	if (::InterlockedIncrement(&_pPsInfo->m_lRunCount) == 1)
	{
		if(!_pPsInfo->EnoughTimePassed())
		{
			::InterlockedDecrement(&_pPsInfo->m_lRunCount);
			return fCpuUsage;
		}

		FILETIME ftSysIdle, ftSysKernel, ftSysUser;
        FILETIME ftProcCreation, ftProcExit, ftProcKernel, ftProcUser;

		if (!GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser) || !GetProcessTimes(_pPsInfo->_hProcess, &ftProcCreation, &ftProcExit, &ftProcKernel, &ftProcUser))
        {
            ::InterlockedDecrement(&_pPsInfo->m_lRunCount);
            return fCpuUsage;
        }

        if (!_pPsInfo->IsFirstRun())
		{
			ULONGLONG ftSysKernelDiff = theSystem->CPU()->SubtractTimes(ftSysKernel, _pPsInfo->m_ftPrevSysKernel);
            ULONGLONG ftSysUserDiff = theSystem->CPU()->SubtractTimes(ftSysUser, _pPsInfo->m_ftPrevSysUser);

            ULONGLONG ftProcKernelDiff = theSystem->CPU()->SubtractTimes(ftProcKernel, _pPsInfo->m_ftPrevProcKernel);
            ULONGLONG ftProcUserDiff = theSystem->CPU()->SubtractTimes(ftProcUser, _pPsInfo->m_ftPrevProcUser);

            ULONGLONG nTotalSys =  ftSysKernelDiff + ftSysUserDiff;
            ULONGLONG nTotalProc = ftProcKernelDiff + ftProcUserDiff;

            if (nTotalSys > 0)
                fCpuUsage = (float)((100.0f * nTotalProc) / nTotalSys);
		}

		_pPsInfo->m_ftPrevSysKernel = ftSysKernel;
        _pPsInfo->m_ftPrevSysUser = ftSysUser;
        _pPsInfo->m_ftPrevProcKernel = ftProcKernel;
        _pPsInfo->m_ftPrevProcUser = ftProcUser;

        _pPsInfo->m_dwLastRun = GetTickCount64();
	}

	::InterlockedDecrement(&_pPsInfo->m_lRunCount);
	return fCpuUsage;
}

//Process Memory
void CProcess::UpdateProcessMemory(Proc::PROCESS_INFO* _pPsInfo)
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

void CProcess::UpdateIOReadWrite(Proc::PROCESS_INFO* _pPsInfo)
{
#ifdef __WXMSW__
	IEnumWbemClassObject* pEnumerator = m_wmi.Collect(_pPsInfo->_ulProcessID);
	if(pEnumerator)
	{
		ULONG uReturn = 0;
		IWbemClassObject* pclsObj = nullptr;

		while(pEnumerator)
		{
			pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
			if(0 == uReturn)
				break;

			VARIANT vtReadTransferCount;
			VARIANT vtWriteTransferCount;

			if(pclsObj->Get(procQuery.ReadTransferCount, 0, &vtReadTransferCount, nullptr, nullptr) == S_OK)
				_pPsInfo->_readTransferCount = vtReadTransferCount.lVal;

			if(pclsObj->Get(procQuery.WriteTransferCount, 0, &vtWriteTransferCount, nullptr, nullptr) == S_OK)
				_pPsInfo->_writeTransferCount =vtWriteTransferCount.llVal;

			VariantClear(&vtReadTransferCount);
			VariantClear(&vtWriteTransferCount);

			pclsObj->Release();
		}
	}


#else

#endif
}

Proc::PROCESS_INFO* CProcess::UpdateProcessInfo(unsigned long _ulProcessId)
{
	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator fIter = m_mapProcessList.find(_ulProcessId);
	if (fIter == m_mapProcessList.end())
		return nullptr;

	Proc::PROCESS_INFO* pProcessInfo = fIter->second;

	UpdateProcessCPUTime(pProcessInfo);
	UpdateProcessMemory(pProcessInfo);
//	UpdateIOReadWrite(pProcessInfo);

	return pProcessInfo;
}

Proc::PROCESS_INFO* CProcess::GetInfo(unsigned long ulProcessID)
{
	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator fIter = m_mapProcessList.find(ulProcessID);
	if (fIter == m_mapProcessList.end())
		return nullptr;

	Proc::PROCESS_INFO* pProcessInfo = fIter->second;
	if (pProcessInfo == nullptr)
		return nullptr;

	return pProcessInfo;
}

std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator CProcess::Begin()
{
	return m_mapProcessList.begin();
}

std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator CProcess::End()
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
	Proc::PROCESS_INFO* pProcess = GetInfo(ulProcessID);
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
