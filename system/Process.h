#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include "Sysinc.h"
#include "SystemInterface.h"

#ifdef __WXMSW__
	#include <wtsapi32.h>
	#include "./msw/NextMWMI.h"
#endif // __WXMSW__

class CProcess : public CSystemInterface
{
public:
	CProcess();
	~CProcess();

	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator Begin();
	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator End();

public:
	void Init() override;
	void Update() override;
	void Free() override;

#ifdef __WXMSW__
	CNextMWMI* GetWMI() { return &m_wmi;};
#endif

	//프로세스 추가
	CProcess* AddProcess(unsigned long ulProcessID);
#ifdef __WXMSW__
	CProcess* AddProcess(IWbemClassObject* pObj);
//	void AddProcessInfo(const WTS_PROCESS_INFO& wtsProcessInfo, bool bNewProcess = false);
#endif

	void DeleteProcess(unsigned long ulProcessID);
	int KillProcess(unsigned long ulProcessID);

	//Process CPU
	float GetProcessCPUUsage(Proc::PROCESS_INFO* _pPsInfo);
	void InitProcessCPUTime(Proc::PROCESS_INFO* _pPsInfo);
	void UpdateProcessCPUTime(Proc::PROCESS_INFO* _pPsInfo);

	//IO/Read, Write
	void UpdateIOReadWrite(Proc::PROCESS_INFO* _pPsInfo);

	//Process Memory
	void UpdateProcessMemory(Proc::PROCESS_INFO* _pPsInfo);

	//Parent Process Info
	void GetParentProcessInfo(Proc::PROCESS_INFO* _pPsInfo);

	Proc::PROCESS_INFO* UpdateProcessInfo(unsigned long _ulProcessId);
	Proc::PROCESS_INFO* GetInfo(unsigned long ulProcessID);

	void UpdateElapsedTime(Proc::PROCESS_INFO* _pPsInfo);

private:
#ifdef __WXMSW__
	bool SetPrivilige(LPCTSTR lpszPrivilege, bool bEnablePrivilege = true);
	int SetUserNameAndDomainNameFromSid(Proc::PROCESS_INFO* pInfo);
	void GetUserNameFromToken(Proc::PROCESS_INFO* pInfo);
	void AddProcessFromEnum(IEnumWbemClassObject* pWbemClsObj);
#else
#endif

private:
	std::unordered_map<unsigned long, Proc::PROCESS_INFO *> m_mapProcessList;
	unsigned int numProcessors = 0;

	wxWindow* m_pWindow = nullptr;

	uint64_t _systime;
#ifdef __WXMSW__
	CNextMWMI m_wmi;
	Proc::WMIProcQuerys procQuery{};
#else
#endif // __WXMSW__

private:
	void OnTimer(wxTimerEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // PROCESS_H_INCLUDED
