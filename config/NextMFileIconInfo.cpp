/******************************************************************************************
 *
 * CNextMFileIconInfo
 *   파일의 아이콘 정보를 미리 저장하기 위한 클래스
 *
*******************************************************************************************/
#include "../common/common.h"
#include "NextMFileIconInfo.h"

#include <iostream>
#include <fstream>
#include <streambuf>

std::unique_ptr<CNextMFileIconInfo> CNextMFileIconInfo::m_pInstance(nullptr);

CNextMFileIconInfo::CNextMFileIconInfo()
{

}

CNextMFileIconInfo::~CNextMFileIconInfo()
{
	_jsonDoc.RemoveAllMembers();
}

CNextMFileIconInfo* CNextMFileIconInfo::Get()
{
	if (m_pInstance.get() == NULL)
		m_pInstance.reset(new CNextMFileIconInfo());

	return m_pInstance.get();
}

bool CNextMFileIconInfo::Load()
{
	_strJsonPath = theUtility->GetWorkDir() + SLASH + wxT("default") + SLASH + wxT("fileiconinfo.cfg");
	if (!wxFileName::FileExists(_strJsonPath))
		return false;

	bool bReturn = DoLoad(_strJsonPath);
	if(bReturn)
        Init();

    return bReturn;
}

void CNextMFileIconInfo::Init()
{
    const Value& FileExtIconInfo = _jsonDoc["FileExtIconInfo"];
	assert(FileExtIconInfo.IsArray());

    for (SizeType i = 0; i < FileExtIconInfo.Size(); i++)
    {
        wxString strExt = FileExtIconInfo[i]["extname"].GetString();
        wxString strExtSend = wxT(".") + strExt;

        int iIconIndex = 0;
        int iOverlayIndex = 0;
		wxString strTypeName = wxT("");

        theUtility->GetIconIndex(strExtSend, iIconIndex, iOverlayIndex, true);
        strTypeName = theUtility->GetFileDescription(strExtSend, strExtSend);

        _stFileIconInfo stIconInfo;
        stIconInfo.m_iIconIndex = iIconIndex;
        stIconInfo.m_iOverayIndex = iOverlayIndex;
		stIconInfo.m_strTypeName = strTypeName;

        m_mapFileIconInfo.insert(std::make_pair(strExt.MakeLower(), stIconInfo));
    }
}

bool CNextMFileIconInfo::GetFileIconInfo(const wxString& strExt, int& IconIndex, int& overlayIndex, wxString& strTypeName)
{
    wxString strTempExt(strExt);
    strTempExt = strTempExt.MakeLower();

    std::unordered_map<wxString, _stFileIconInfo>::const_iterator fIter = m_mapFileIconInfo.find(strTempExt);
    if(fIter == m_mapFileIconInfo.end())
        return false;

    _stFileIconInfo stIconInfo = fIter->second;

    IconIndex    = stIconInfo.m_iIconIndex;
    overlayIndex = stIconInfo.m_iOverayIndex;
	strTypeName  = stIconInfo.m_strTypeName;

    return true;
}

