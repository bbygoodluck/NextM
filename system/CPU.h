#ifndef CPUUSAGEINFO_H_INCLUDED
#define CPUUSAGEINFO_H_INCLUDED

#include "SystemInterface.h"

class CCPU : public CSystemInterface
{
public:
	CCPU();
	~CCPU();

public:
	virtual void Init()   override;
	virtual void Update() override;

	wxString GetProcessorName()   const { return m_strProcessor; }
	size_t GetCoreCount()         const { return m_CPUCoreCount; }
	unsigned long GetTotalUsage() const { return m_ulCPUTotalUage; }
	unsigned long GetCPUCoreUsage(unsigned int index);
	double GetUsage();

private:
	ULONGLONG SubtractTimes(const FILETIME& a, const FILETIME& b) {
		ULONGLONG a64 = (((ULONGLONG)a.dwHighDateTime) << 32) | a.dwLowDateTime;
		ULONGLONG b64 = (((ULONGLONG)b.dwHighDateTime) << 32) | b.dwLowDateTime;

		return a64 - b64;
	}

private:
	wxString m_strProcessor = wxT("");

	unsigned long  m_ulCPUTotalUage = 0;
	size_t    m_CPUCoreCount = 0;

	FILETIME idlePrev, kernelPrev, userPrev;

#ifdef __WXMSW__
	unsigned long* m_pArrayCore = nullptr;
	HQUERY m_hCpuQuery;

	HCOUNTER  m_hCounterCPUTotal;
	HCOUNTER* m_phCounterCPUCore = nullptr;

	bool m_bPhdOK = false;
#else
	FILETIME m_ftPrevSysIdle;
	FILETIME m_ftPrevSysKernel;
	FILETIME m_ftPrevSysUser;
#endif // __WXMSW__

private:
	virtual void OnTimer(wxTimerEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // CPUUSAGEINFO_H_INCLUDED
