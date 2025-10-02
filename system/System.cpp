#include "../ginc.h"
#include "Sysinc.h"
#include "System.h"

std::unique_ptr<CSystem> CSystem::m_pInstance(nullptr);

CSystem::CSystem()
{
	m_pCPU     = new CCPU();
	m_pMemory  = new CMemory();
	m_pNetwork = new CNetwork();
	m_pProcess = new CProcess();
}

CSystem::~CSystem()
{
	delete m_pCPU;
	delete m_pMemory;
	delete m_pNetwork;
	delete m_pProcess;
}

CSystem* CSystem::Get()
{
	if (m_pInstance.get() == NULL)
		m_pInstance.reset(new CSystem());

	return m_pInstance.get();
}
