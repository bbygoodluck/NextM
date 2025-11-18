#include "../ginc.h"
#include "Sysdef.h"
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

}

void CCPU::Init()
{
	Free();
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
	InitializePDH();
#endif // __WXMSW__
}

#ifdef __WXMSW__
bool CCPU::InitializePDH()
{
	if(m_bPhdOK)
		return m_bPhdOK;

	PDH_STATUS status = PdhOpenQuery(NULL, 0, &m_hCpuQuery);
	if(status != ERROR_SUCCESS)
		return false;

	// CPU core 정보
	SYSTEM_INFO SystemInfo = { 0 };
	GetSystemInfo(&SystemInfo);

	if (SystemInfo.dwNumberOfProcessors > 0)
		m_CPUCoreCount = SystemInfo.dwNumberOfProcessors;

	//CPU전체 사용량
	AddCounter(CPU_TOTAL);

	wxString strCore{wxT("")};
	for (size_t cnt = 0; cnt < m_CPUCoreCount; cnt++)
	{
		strCore = wxString::Format(CPU_CORE, (int)cnt);
		AddCounter(strCore);
	}

	m_bPhdOK = true;

	return true;
}
#endif

void CCPU::Free()
{
#ifdef __WXMSW__
	robin_hood::unordered_flat_map<wxString, Cpu::PPDHCOUNTERSTRUCT>::iterator iter = m_Counters.begin();
	for (iter; iter != m_Counters.end(); ++iter)
	{
		Cpu::PPDHCOUNTERSTRUCT pdhCounter = iter->second;
		PdhRemoveCounter(pdhCounter->hCounter);

		delete pdhCounter;
	}

	m_Counters.clear();
	if(m_hCpuQuery)
		PdhCloseQuery(m_hCpuQuery);

	m_bPhdOK = false;
#else

#endif
}

#ifdef __WXMSW__
bool CCPU::CollectQueryData()
{
	if (PdhCollectQueryData(m_hCpuQuery) != ERROR_SUCCESS)
		return false;

	return true;
}

bool CCPU::AddCounter(const wxString& counterName)
{
	Cpu::PPDHCOUNTERSTRUCT pdhCounter = new Cpu::PDHCOUNTERSTRUCT;
	if (PdhAddCounter(m_hCpuQuery, counterName.wc_str(), (DWORD_PTR)pdhCounter, &(pdhCounter->hCounter)) != ERROR_SUCCESS)
		return false;

	m_vecCounters.emplace_back(counterName);

	robin_hood::unordered_flat_map<wxString, Cpu::PPDHCOUNTERSTRUCT>::value_type val(counterName, pdhCounter);
	m_Counters.insert(val);

	return true;
}
#endif // __WXMSW__

auto CCPU::GetValue(const wxString& strKey) -> unsigned long
{
#ifdef __WXMSW__
	robin_hood::unordered_flat_map<wxString, Cpu::PPDHCOUNTERSTRUCT>::const_iterator iter = m_Counters.find(strKey);
	if(iter == m_Counters.end() )
		return 0;

	Cpu::PPDHCOUNTERSTRUCT pdhCounter = iter->second;
	return pdhCounter->ulValue;
#else
	return 0;
#endif
}

void CCPU::OnTimer(wxTimerEvent& event)
{
	Update();
	SendEvent();
}

void CCPU::Update()
{
#ifdef __WXMSW__
	if(!m_bPhdOK)
		return;

	PDH_STATUS status = ERROR_SUCCESS;
	if(!CollectQueryData())
		return;

	int count = (int)m_vecCounters.size();
	for(int i = 0 ; i < count ; i++)
	{
		wxString strKey = m_vecCounters.at(i);

		robin_hood::unordered_flat_map<wxString, Cpu::PPDHCOUNTERSTRUCT>::const_iterator iter = m_Counters.find(strKey);
		if(iter != m_Counters.end() )
		{
			Cpu::PPDHCOUNTERSTRUCT pdhCounter = iter->second;

			PDH_FMT_COUNTERVALUE pdhFormattedValue = {0};
			status = PdhGetFormattedCounterValue(pdhCounter->hCounter, PDH_FMT_LONG, NULL, &pdhFormattedValue);
			if (status != ERROR_SUCCESS)
			{
				pdhCounter->ulValue = 0;
				continue;
			}

			pdhCounter->ulValue = pdhFormattedValue.longValue;
		}
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
