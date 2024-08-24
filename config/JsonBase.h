#ifndef __JSON_BASE_H__
#define __JSON_BASE_H__

#include "../rapidjson/document.h"
using namespace rapidjson;

#include "../rapidjson/istreamwrapper.h"
#include "../rapidjson/ostreamwrapper.h"
#include "../rapidjson/prettywriter.h"

#include <iostream>
#include <fstream>
#include <streambuf>

class CJsonBase
{
public:
	CJsonBase() {}
	~CJsonBase() {}

	Document& GetJsonDoc() { return _jsonDoc; };
	virtual bool Load() = 0;

	bool HasMember(const wxString& _strMember, const wxString& _strKey = wxT(""))
	{
		if(_strKey.IsEmpty())
			return _jsonDoc.HasMember(_strMember.char_str());

		Value _Key(_strKey.c_str(), _jsonDoc.GetAllocator());
		return _jsonDoc[_Key].HasMember(_strMember.char_str());
	}

	bool IsExistMember(const wxString& strGroup, const wxString& strMember)
	{
		assert(_jsonDoc.IsObject());

		wxString _strKey(strGroup);
		wxString _strMember(strMember);

		if (_strKey.IsEmpty()) return HasMember(_strMember);
		else                   return HasMember(_strMember, _strKey);

		return true;
	}

	void SaveDoc()
	{
		wxString strOutJson(_strJsonPath);
		std::ofstream ofs;

		ofs.open(strOutJson.char_str());

		OStreamWrapper osw(ofs);
		PrettyWriter<OStreamWrapper> writer(osw);

		_jsonDoc.Accept(writer);
	}
protected:
	bool DoLoad(const wxString& strPath);

protected:
	Document _jsonDoc;
	wxString _strJsonPath;
};
#endif
