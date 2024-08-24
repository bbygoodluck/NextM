#include "../common/common.h"
#include "NextMConfig.h"

#include <iostream>
#include <fstream>
#include <streambuf>

std::unique_ptr<CNextMColor> CNextMColor::m_pInstance(nullptr);

CNextMColor::CNextMColor()
{

}

CNextMColor::~CNextMColor()
{
	_jsonDoc.RemoveAllMembers();
}

CNextMColor* CNextMColor::Get()
{
	if (m_pInstance.get() == NULL)
		m_pInstance.reset(new CNextMColor());

	return m_pInstance.get();
}

bool CNextMColor::Load()
{
	_strJsonPath = theUtility->GetWorkDir() + SLASH + wxT("colors") + SLASH + theConfig->GetColorTheme();
	if (!wxFileName::FileExists(_strJsonPath))
	{
		theUtility->ShowMessageBox(_strJsonPath + wxT(" file is not exist"), wxICON_ERROR | wxOK);
		return false;
	}

	if(!DoLoad(_strJsonPath))
		return false;

	SetColors();

	return true;
}


void CNextMColor::SetColors()
{
	wxString strdefaultColor(_jsonDoc["defaultcolor"]["DefaultColor"].GetString());
	wxString strdirColor(_jsonDoc["defaultcolor"]["DirColor"].GetString());
	wxString strdrvColor(_jsonDoc["defaultcolor"]["DrvColor"].GetString());
	wxString strreadOnlyColor(_jsonDoc["defaultcolor"]["ReadOnlyColor"].GetString());
	wxString strselectItemColor(_jsonDoc["defaultcolor"]["SelectItemColor"].GetString());
	wxString strbackgroundColor(_jsonDoc["defaultcolor"]["BackgroundColor"].GetString());
	wxString strattributeColor(_jsonDoc["defaultcolor"]["AttributeColor"].GetString());
	wxString strdescColor(_jsonDoc["defaultcolor"]["DescColor"].GetString());
	wxString strcolumnColor(_jsonDoc["defaultcolor"]["ColumnColor"].GetString());
	wxString strinfoviewColor(_jsonDoc["defaultcolor"]["InfoViewBackColor"].GetString());
	wxString strinfoviewtxtColor(_jsonDoc["defaultcolor"]["InfoViewTextColor"].GetString());
	wxString str_matchColor(_jsonDoc["defaultcolor"]["matchColor"].GetString());

	unsigned long nDefaultColor     = ConvertStrToColor(strdefaultColor);
	unsigned long nDirColor         = ConvertStrToColor(strdirColor);
	unsigned long nDrvColor         = ConvertStrToColor(strdrvColor);
	unsigned long nReadOnlyColor    = ConvertStrToColor(strreadOnlyColor);
	unsigned long nSelectItemColor  = ConvertStrToColor(strselectItemColor);
	unsigned long nBackgroundColor  = ConvertStrToColor(strbackgroundColor);
	unsigned long nAttributeColor   = ConvertStrToColor(strattributeColor);
	unsigned long nDescColor        = ConvertStrToColor(strdescColor);
	unsigned long nColumnColor      = ConvertStrToColor(strcolumnColor);
	unsigned long ninfoviewColor    = ConvertStrToColor(strinfoviewColor);
	unsigned long ninfoviewtxtColor = ConvertStrToColor(strinfoviewtxtColor);
	unsigned long ulmatchColor      = ConvertStrToColor(str_matchColor);

	m_defaultColor     = theUtility->GetColor(nDefaultColor);
	m_dirColor         = theUtility->GetColor(nDirColor);
	m_drvColor         = theUtility->GetColor(nDrvColor);
	m_readOnlyColor    = theUtility->GetColor(nReadOnlyColor);
	m_selectItemColor  = theUtility->GetColor(nSelectItemColor);
	m_backgroundColor  = theUtility->GetColor(nBackgroundColor);
	m_attributeColor   = theUtility->GetColor(nAttributeColor);
	m_descColor        = theUtility->GetColor(nDescColor);
	m_columnColor      = theUtility->GetColor(nColumnColor);
	m_infoviewColor    = theUtility->GetColor(ninfoviewColor);
	m_infoviewtxtColor = theUtility->GetColor(ninfoviewtxtColor);
	m_matchColor       = theUtility->GetColor(ulmatchColor);
}

const wxColour CNextMColor::GetExtColor(const wxString& strExt)
{
	wxString _strExt(strExt);

	Value::ConstMemberIterator Iter = _jsonDoc["extcolor"].FindMember(_strExt.c_str());
	if (Iter == _jsonDoc["extcolor"].MemberEnd())
		return m_defaultColor;

	wxString strColorValue(Iter->value.GetString());

	unsigned long lColorValue = ConvertStrToColor(strColorValue);
	wxColour col = theUtility->GetColor(lColorValue);
	return col;
}

unsigned long CNextMColor::ConvertStrToColor(wxString& strColor)
{
	unsigned long lvalue = 0;
	strColor.ToULong(&lvalue, 16);

	return lvalue;
}

void CNextMColor::SetChangeColorVal(const wxString& strKey, const wxString& strMember, const wxString& strVal)
{
	if (!HasMember(strKey))
		return;

	if (!HasMember(strMember, strKey))
		return;

	Value _Key(strKey.c_str(), _jsonDoc.GetAllocator());
	Value _Member(strMember.c_str(), _jsonDoc.GetAllocator());
	Value _Val(strVal.c_str(), _jsonDoc.GetAllocator());

	_jsonDoc[_Key][_Member] = _Val;
}
