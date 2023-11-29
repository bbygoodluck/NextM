#include "../common/common.h"
#include "NextMMsg.h"

std::unique_ptr<CNextMMsg> CNextMMsg::m_pMsgInstance(nullptr);

bool CNextMMsg::Load()
{
	wxString strMsgPath(theUtility->GetWorkDir() + SLASH + wxT("lang") + SLASH);

	if (_gSystemLang.CmpNoCase(wxT("ko")) == 0)
		strMsgPath += wxT("ko_lang.json");
	else
		strMsgPath += wxT("en_lang.json");

	if(!wxFileExists(strMsgPath))
		return false;

	bool bReturn = DoLoad(strMsgPath);
	return bReturn;
}

wxString CNextMMsg::GetMessage(const wxString& strMessageDefine)
{
	wxString _strKey(strMessageDefine);

	assert(_jsonDoc.IsObject());
	assert(_jsonDoc.HasMember(_strKey.char_str()));

	Value::ConstMemberIterator Iter = _jsonDoc.FindMember(_strKey.c_str());
	if (Iter == _jsonDoc.MemberEnd())
		return wxT("");

	wxString strRetMsg(Iter->value.GetString());
	return strRetMsg;
}

/* static */ CNextMMsg* CNextMMsg::Get()
{
	if (m_pMsgInstance.get() == nullptr)
		m_pMsgInstance.reset(new CNextMMsg());

	return m_pMsgInstance.get();
}
