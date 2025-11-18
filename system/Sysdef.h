#ifndef SYSDEF_H_INCLUDED
#define SYSDEF_H_INCLUDED

#define NEW_PROCESS_ADDED 1
#define DEL_PROCESS       0

#include <string>
#include <deque>

namespace Cpu {
#ifdef __WXMSW__
	#define CPU_TOTAL wxT("\\Processor(_Total)\\% Processor Time")
	#define CPU_CORE wxT("\\Processor(%d)\\%% Processor Time")

	typedef struct _PDHCounterStruct {
		//Handle to the counter
		HCOUNTER hCounter;
		unsigned long ulValue = 0;
	} PDHCOUNTERSTRUCT, *PPDHCOUNTERSTRUCT;
#else
#endif // __WXMSW__
}

namespace Net
{
	struct net_stat {
		uint64_t speed = 0, top = 0, total = 0, last = 0, offset = 0, rollover = 0, bandwidth = 0;
	};

	struct net_info {
	//	robin_hood::unordered_flat_map<wxString, std::deque<long long>> bandwidth = { {wxT("download"), {}}, {wxT("upload"), {}} };
		robin_hood::unordered_flat_map<wxString, net_stat> stat = { {wxT("download"), {}}, {wxT("upload"), {}} };
		wxString ipv4 = "";
		wxString ipv6 = "";
		wxString description{wxT("")};

		bool connected = false;
	};
}

namespace Proc {
#ifdef __WXMSW__
	struct WMIProcQuerys {
		_bstr_t WQL                = L"WQL";
		_bstr_t SELECTPROC         = L"SELECT * FROM Win32_Process";
		_bstr_t SELECTPROCPID      = L"SELECT * FROM Win32_Process WHERE ProcessId = '%u'";
		_bstr_t ProcessID          = L"ProcessId";
		_bstr_t Name               = L"Name";
		_bstr_t Description        = L"Description";
		_bstr_t CommandLine        = L"CommandLine";
		_bstr_t ExecutablePath     = L"ExecutablePath";
		_bstr_t KernelModeTime     = L"KernelModeTime";
		_bstr_t UserModeTime       = L"UserModeTime";
		_bstr_t CreationDate       = L"CreationDate";
		_bstr_t ThreadCount        = L"ThreadCount";
		_bstr_t PrivateMemory      = L"PrivatePageCount";
		_bstr_t WorkingSet         = L"WorkingSetSize";
		_bstr_t ProcessorTime      = L"PercentProcessorTime";
		_bstr_t ReadTransferCount  = L"ReadTransferCount";
		_bstr_t WriteTransferCount = L"WriteTransferCount";
		_bstr_t ParentProcessId    = L"ParentProcessId";
		_bstr_t newProcess         = L"SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'";
		_bstr_t delProcess         = L"SELECT * FROM __InstanceDeletionEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'";
	};

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
		wxString _strUserModeTime    = wxT("");
		wxString _strKernelModeTime  = wxT("");
		wxString _strStatus          = wxT("");
		unsigned int _threadsCount   = 0;
		unsigned long long _readTransferCount = 0;
		unsigned long long _writeTransferCount = 0;
		unsigned long long _PrivateSize = 0;
		unsigned long long _WorkingSetSize = 0;

		float fCPUUsage = 0.0f;

		int iIconIndex = -1;
		int iOvelayIndex = -1;

		//system total times
		FILETIME m_ftPrevSysKernel;
		FILETIME m_ftPrevSysUser;

		//process times
		FILETIME m_ftPrevProcKernel;
		FILETIME m_ftPrevProcUser;

		short m_nCpuUsage = -1;
		volatile LONG m_lRunCount;
		ULONGLONG m_dwLastRun = 0;

		bool EnoughTimePassed() {
			const int minElapsedMS = 250;//milliseconds

			ULONGLONG dwCurrentTickCount = GetTickCount64();
			return (dwCurrentTickCount - m_dwLastRun) > minElapsedMS;
		};

		inline bool IsFirstRun() const { return (m_dwLastRun == 0); }

//		FILETIME _ftSysKernel;
//		FILETIME _ftSysUser;
//		FILETIME _ftProcKernel;
//		FILETIME _ftProcUser;
//
//		FILETIME _ftLastCPU;
//		FILETIME _ftLastSysCPU;
//		FILETIME _ftLastUserCPU;
//
//		ULARGE_INTEGER _lastCPU;
//		ULARGE_INTEGER _lastSysCPU;
//		ULARGE_INTEGER _lastUserCPU;

	} PROCESS_INFO, *PPROCESS_INFO;
#else
#endif // __WXMSW__
}
#endif // SYSDEF_H_INCLUDED
