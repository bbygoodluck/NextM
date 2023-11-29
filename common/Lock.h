#ifndef _LOCK_H_
#define _LOCK_H_
/********************************************************************************
// class Name : CLock
*********************************************************************************/
class CLock
{
public:
	explicit CLock();
	~CLock();

public:
	wxMutex* getMutex() { return m_pMutex; }
	wxCondition* getCondition() { return m_pCondition; }

	void setBoolean(const bool flag);
	inline const bool isTrue() { return m_bool; }
	
private:
	bool m_bool;
	wxMutex* m_pMutex;
	wxCondition* m_pCondition;
};

#endif