#ifndef CPUUSAGEINFO_H_INCLUDED
#define CPUUSAGEINFO_H_INCLUDED

#include "SystemInterface.h"

class CCPU : public CSystemInterface
{
public:
	CCPU();
	~CCPU();

public:
	void Init()   override;
	void Update() override;
	void Free()   override;

	wxString GetProcessorName()   const { return m_strProcessor; }
	size_t GetCoreCount()         const { return m_CPUCoreCount; }

	double GetUsage();

	void UnInitialize();
	auto GetValue(const wxString& strKey) -> unsigned long;

	ULONGLONG SubtractTimes(const FILETIME& ftA, const FILETIME& ftB)
	{
		ULONGLONG a64 = FileTimeToULL(ftA);//(((ULONGLONG)ftA.dwHighDateTime) << 32) | ftA.dwLowDateTime;
		ULONGLONG b64 = FileTimeToULL(ftB);//(((ULONGLONG)ftB.dwHighDateTime) << 32) | ftB.dwLowDateTime;

		return a64 - b64;
	}

	ULONGLONG FileTimeToULL(const FILETIME& ft) {
		return ((((ULONGLONG)ft.dwHighDateTime) << 32) | ft.dwLowDateTime);
	}

	void GetSystemCPUTime(ULONGLONG& idle, ULONGLONG& kernel, ULONGLONG& user) {
		FILETIME idleFT, kernelFT, userFT;

		GetSystemTimes(&idleFT, &kernelFT, &userFT);
		idle   = FileTimeToULL(idleFT);
		kernel = FileTimeToULL(kernelFT);
		user   = FileTimeToULL(userFT);
	}

#ifdef __WXMSW__
	bool CollectQueryData();
	bool InitializePDH();
	bool IsInitializePDH() { return m_bPhdOK; }
	bool AddCounter(const wxString& counterName);
#endif // __WXMSW__

private:
	wxString m_strProcessor = wxT("");

	unsigned long  m_ulCPUTotalUage = 0;
	size_t    m_CPUCoreCount = 0;

	FILETIME idlePrev, kernelPrev, userPrev;

#ifdef __WXMSW__
	HQUERY m_hCpuQuery = 0;
	bool m_bPhdOK = false;

	robin_hood::unordered_flat_map<wxString, Cpu::PPDHCOUNTERSTRUCT> m_Counters;
	std::vector<wxString> m_vecCounters;
#else
	FILETIME m_ftPrevSysIdle;
	FILETIME m_ftPrevSysKernel;
	FILETIME m_ftPrevSysUser;
#endif // __WXMSW__

private:
	void OnTimer(wxTimerEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // CPUUSAGEINFO_H_INCLUDED
