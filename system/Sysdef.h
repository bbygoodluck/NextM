#ifndef SYSDEF_H_INCLUDED
#define SYSDEF_H_INCLUDED

#define NEW_PROCESS_ADDED 1
#define DEL_PROCESS       0

#ifdef __WXMSW__
typedef struct _PROCESS_INFO {
	HANDLE _hProcess = nullptr;
	unsigned long _ulProcessID = 0x00;
	wxString _strProcessName = wxT("");
	wxString _strProcessFullPath = wxT("");
	wxString _strFileDescription = wxT("");
	wxString _strCommandLine     = wxT("");
	wxString _strCompanyName     = wxT("");
	wxString _strUserName        = wxT("");
	wxString _strDomainName      = wxT("");
	wxString _strDomainAndUser   = wxT("");

	unsigned long long _PrivateSize = 0;
	unsigned long long _WorkingSetSize = 0;

	float fCPUUsage = 0.0f;

	int iIconIndex = -1;
	int iOvelayIndex = -1;

	FILETIME _ftSysKernel;
	FILETIME _ftSysUser;
	FILETIME _ftProcKernel;
	FILETIME _ftProcUser;

	FILETIME _ftLastCPU;
	FILETIME _ftLastSysCPU;
	FILETIME _ftLastUserCPU;

	ULARGE_INTEGER _lastCPU;
	ULARGE_INTEGER _lastSysCPU;
	ULARGE_INTEGER _lastUserCPU;

} PROCESS_INFO, *PPROCESS_INFO;
#else
#endif

#endif // SYSDEF_H_INCLUDED
