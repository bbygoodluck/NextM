#include "../ginc.h"
#include "CPU.h"

#include <intrin.h>

wxBEGIN_EVENT_TABLE(CCPU, CSystemInterface)
wxEND_EVENT_TABLE()

CCPU::CCPU()
{
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CCPU::OnTimer));
	m_timer.SetOwner(this, wxTIMER_ID++);
}

CCPU::~CCPU()
{
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(CCPU::OnTimer));

#ifdef __WXMSW__
	PdhCloseQuery(m_hCpuQuery);
	PdhRemoveCounter(m_hCounterCPUTotal);

	for (size_t cnt = 0; cnt < m_CPUCoreCount; cnt++)
		PdhRemoveCounter(m_phCounterCPUCore[cnt]);

	wxDELETE(m_phCounterCPUCore);
	delete[] m_pArrayCore;
#endif // __WXMSW__
}

void CCPU::Init()
{
	GetSystemTimes(&idlePrev, &kernelPrev, &userPrev);

	// Reference StackOverflow
	// 4 is essentially hardcoded due to the __cpuid function requirements.
	// NOTE: Results are limited to whatever the sizeof(int) * 4 is...

	std::array<int, 4> integerBuffer = {};
	constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();
	std::array<char, 64> charBuffer = {};

	// The information you wanna query __cpuid for.
	// https://docs.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=vs-2019
	constexpr std::array<unsigned int, 3> functionIds = {
		// Manufacturer
		//  EX: "Intel(R) Core(TM"
		0x80000002,
		// Model
		//  EX: ") i7-8700K CPU @"
		0x80000003,
		// Clockspeed
		//  EX: " 3.70GHz"
		0x80000004
	};

	m_strProcessor = wxT("");
	for (int id : functionIds)
	{
		// Get the data for the current ID.
		__cpuid(integerBuffer.data(), id);

		// Copy the raw data from the integer buffer into the character buffer
		std::memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);

		// Copy that data into a std::string
		m_strProcessor += wxString::Format(wxT("%s"), charBuffer.data());
	}

#ifdef __WXMSW__
	PDH_STATUS status = PdhOpenQuery(NULL, 0, &m_hCpuQuery);
	if(status != ERROR_SUCCESS)
		return;

	if( PdhAddEnglishCounter(m_hCpuQuery, TEXT("\\Processor(_Total)\\% Processor Time"), 0, &m_hCounterCPUTotal) != ERROR_SUCCESS ||
		PdhCollectQueryData(m_hCpuQuery) != ERROR_SUCCESS)
	{
		PdhCloseQuery(m_hCpuQuery);
		return;
	}

	// CPU core 정보
	SYSTEM_INFO SystemInfo = { 0 };
	GetSystemInfo(&SystemInfo);

	if (SystemInfo.dwNumberOfProcessors > 0)
	{
		m_CPUCoreCount = SystemInfo.dwNumberOfProcessors;

		m_phCounterCPUCore = new HCOUNTER[m_CPUCoreCount];
		//CPU코어 수만큼 생성하고 0으로 초기화
		m_pArrayCore = new unsigned long[m_CPUCoreCount]();

		for (size_t cnt = 0; cnt < m_CPUCoreCount; cnt++)
		{
			TCHAR szFullCounterPath[1024] = { 0x00, };
			wsprintf(szFullCounterPath, TEXT("\\Processor(%d)\\%% Processor Time"), cnt);
			PDH_STATUS status = PdhAddEnglishCounter(m_hCpuQuery, szFullCounterPath, 1, &m_phCounterCPUCore[cnt]);
			assert(status == ERROR_SUCCESS);
		}
	}

	m_bPhdOK = true;

#endif // __WXMSW__
}

void CCPU::Update()
{
#ifdef __WXMSW__
	if(!m_bPhdOK)
		return;

	PDH_STATUS status = PdhCollectQueryData(m_hCpuQuery);

	if (status == ERROR_SUCCESS)
	{
		// 전체 CPU 사용량
		PDH_FMT_COUNTERVALUE PFC_Total = { 0 };
		status = PdhGetFormattedCounterValue(m_hCounterCPUTotal, PDH_FMT_LONG, NULL, &PFC_Total);
		if (status == ERROR_SUCCESS)
			m_ulCPUTotalUage = PFC_Total.longValue;
	}

	// 개별 코어 사용량
	for (size_t c1 = 0; c1 < m_CPUCoreCount; c1++)
	{
		PDH_FMT_COUNTERVALUE PFC_Value = { 0 };
		status = PdhGetFormattedCounterValue(m_phCounterCPUCore[c1], PDH_FMT_LONG, NULL, &PFC_Value);
		if (status != ERROR_SUCCESS)
		{
			m_pArrayCore[c1] = 0;
			continue;
		}

		m_pArrayCore[c1] = PFC_Value.longValue;
	}
#else
	FILETIME sysIdle, sysKernel, sysUser;
	// GetSystemTimes func FAILED return
	if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) == 0)
        return;

	//lamda function
	auto SubSysTimes = [](const FILETIME ft1, const FILETIME ft2) {
		ULONGLONG a,b = 0;

		memcpy( &a, &ft1, sizeof (ULONGLONG) );
		memcpy( &b, &ft2, sizeof (ULONGLONG) );

		return a - b;
	};

	ULONGLONG ullsysIdleDiff, ullsysKernelDiff, ullsysUserDiff;
	ullsysIdleDiff = SubSysTimes(sysIdle, m_ftPrevSysIdle);
	ullsysKernelDiff = SubSysTimes(sysKernel, m_ftPrevSysKernel);
	ullsysUserDiff = SubSysTimes(sysUser, m_ftPrevSysUser);

	ULONGLONG ullSysTotall = ullsysKernelDiff + ullsysUserDiff;
	// kernelTime - IdleTime = kernelTime, because sysKernel include IdleTime
	ULONGLONG ullkernelTotal = ullsysKernelDiff - ullsysIdleDiff;

	double dblCPUUsage = 0.0f;
	// sometimes kernelTime > idleTime
	if (ullSysTotall == 0)
		return;

	dblCPUUsage = (double)(((ullkernelTotal + ullsysUserDiff) * 100.0) / ullSysTotall);
	m_ulCPUTotalUage = (unsigned long)dblCPUUsage;

	m_ftPrevSysIdle = sysIdle;
    m_ftPrevSysKernel = sysKernel;
    m_ftPrevSysUser = sysUser;
#endif
}

unsigned long CCPU::GetCPUCoreUsage(unsigned int index)
{
	if(!m_bPhdOK)
		return 0;

	if(index > m_CPUCoreCount)
		return 0;

	return m_pArrayCore[index];
}


double CCPU::GetUsage()
{
	FILETIME idleNow, kernelNow, userNow;
	GetSystemTimes(&idleNow, &kernelNow, &userNow);

	ULONGLONG idle = SubtractTimes(idleNow, idlePrev);
	ULONGLONG kernel = SubtractTimes(kernelNow, kernelPrev);
	ULONGLONG user = SubtractTimes(userNow, userPrev);

	ULONGLONG total = kernel + user;

	idlePrev   = idleNow;
	kernelPrev = kernelNow;
	userPrev   = userNow;

	if(total == 0.0)
		return 0.0;

	return (1.0 - (idle * 1.0 / total)) * 100.0;
}

void CCPU::OnTimer(wxTimerEvent& event)
{
	Update();
	SendEvent();
}
