#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#ifdef __WXMSW__
#include <wtsapi32.h>
#endif // __WXMSW__

#include "Sysinc.h"
#include "SystemInterface.h"

class CProcess : public CSystemInterface
{
public:
	CProcess();
	~CProcess();

	std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator Begin();
	std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator End();

public:
	virtual void Init() override;
	virtual void Update() override;

	void Clear();
#ifdef __WXMSW__
	void AddProcessInfo(const WTS_PROCESS_INFO& wtsProcessInfo, bool bNewProcess = false);
#endif

	void AddProcessInfo(unsigned long ulProcessID, const wxString& _strProcessName);
	void DeleteProcess(unsigned long ulProcessID);
	int KillProcess(unsigned long ulProcessID);

	//Process CPU
	void InitProcessCPUTime(PROCESS_INFO* _pPsInfo);
	void UpdateProcessCPUTime(PROCESS_INFO* _pPsInfo);
	//Process Memory
	void UpdateProcessMemory(PROCESS_INFO* _pPsInfo);
	PROCESS_INFO* UpdateProcessInfo(unsigned long _ulProcessId);
	PROCESS_INFO* GetInfo(unsigned long ulProcessID);

private:
#ifdef __WXMSW__
	bool SetPrivilige(LPCTSTR lpszPrivilege, bool bEnablePrivilege = true);
	int SetUserNameAndDomainNameFromSid(PROCESS_INFO* pInfo);
	void GetUserNameFromToken(PROCESS_INFO* pInfo);
#else
#endif

private:
	std::unordered_map<unsigned long, PROCESS_INFO *> m_mapProcessList;
	unsigned int numProcessors = 0;
private:
	virtual void OnTimer(wxTimerEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // PROCESS_H_INCLUDED
