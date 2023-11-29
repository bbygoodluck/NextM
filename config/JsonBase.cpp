#include "../common/common.h"
#include "JsonBase.h"

bool CJsonBase::DoLoad(const wxString& strPath)
{
	_strJsonPath = strPath;
	std::ifstream ifs;
	ifs.open(strPath.char_str());

	IStreamWrapper isw(ifs);

	bool bReturn = !_jsonDoc.ParseStream(isw).HasParseError();
	return bReturn;
}
