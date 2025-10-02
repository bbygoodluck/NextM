#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

class CCPU;
class CMemory;
class CNetwork;
class CProcess;

class CSystem
{
private:
	CSystem();

public:
	~CSystem();
	static CSystem* Get();

	CCPU*     CPU()     const { return m_pCPU;     }
	CMemory*  MEM()     const { return m_pMemory;  }
	CNetwork* NET()     const { return m_pNetwork; }
	CProcess* PROCESS() const { return m_pProcess; }

private:
	static std::unique_ptr<CSystem> m_pInstance;

private:
	CCPU     *m_pCPU{nullptr};
	CMemory  *m_pMemory{nullptr};
	CNetwork *m_pNetwork{nullptr};
	CProcess *m_pProcess{nullptr};
};

#endif // SYSTEM_H_INCLUDED
