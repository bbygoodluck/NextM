#ifndef __MESSAGE_MANAGER_H__
#define __MESSAGE_MANAGER_H__

#include "JsonBase.h"
class CNextMMsg : public CJsonBase
{
private:
	CNextMMsg() {};

public:
	~CNextMMsg() {
		_jsonDoc.RemoveAllMembers();
	};

public:
	virtual bool Load();
	wxString GetMessage(const wxString& strMessageDefine);

public:
	static CNextMMsg *Get();

private:
	static std::unique_ptr<CNextMMsg> m_pMsgInstance;
};
#endif
