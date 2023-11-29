#include "common.h"
#include "Lock.h"

CLock::CLock()
	: m_bool(false)
	, m_pMutex(nullptr)
	, m_pCondition(nullptr)
{
	m_pMutex = new wxMutex();
	m_pCondition = new wxCondition(*m_pMutex);
}

CLock::~CLock()
{
	m_pCondition->Broadcast();
	if (m_pCondition)
		delete m_pCondition;

	m_pCondition = nullptr;

	if (m_pMutex)
		delete m_pMutex;

	m_pMutex = nullptr;
}

void CLock::setBoolean(const bool flag)
{
	m_bool = flag;
}
