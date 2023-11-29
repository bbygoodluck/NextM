#include "../ginc.h"
#include "NextMDirData.h"
#include "../engine/FileSystem.h"

#include <wx/bitmap.h>

CNextMDirData::CNextMDirData()
{
	Init();
}

CNextMDirData::~CNextMDirData()
{
}

void CNextMDirData::Init()
{
	m_strName		= wxT("");
//	m_strPath		= wxT("");
	m_strDriveName	= wxT("");
	m_isDir			= false;
	m_isFile		= false;
	m_isDrive		= false;
	m_bSelected		= false;
	m_isCut			= false;
	m_ulattr		= 999999999;
	m_llSize		= 0;
	m_dt			= 0.0;
	m_iType			= item_type::unknown;

	m_iIconIndex = 0;
	m_iOvelayIndex = 0;
}

void CNextMDirData::SetName(const wxString& strName)
{
	m_strName = strName;
}

void CNextMDirData::SetPath(const wxString& strPath)
{
	m_strPath = strPath;
}

void CNextMDirData::SetType(item_type iType)
{
	m_iType = iType;
	switch(m_iType)
	{
		case item_type::dir:
			m_isDir = true;
			break;

		case item_type::file:
			m_isFile = true;
			break;

		case item_type::drive:
			m_isDrive = true;
			break;

		default:
			break;
	}
}

void CNextMDirData::SetAttribute(unsigned long ulAttr)
{
	m_ulattr = ulAttr;
}

void CNextMDirData::SetSize(const wxLongLong& llSize)
{
	m_llSize = llSize;
	if (m_iType == item_type::file)
	{
		wxString strSizeType;
		wxString strSize;
		m_colorType = wxColour(192, 192, 192);

		double dblFileSize = llSize.ToDouble();
		theUtility->GetSizeInfo(dblFileSize, m_strSize, m_strSizeType, m_colorType);
	}
}

void CNextMDirData::SetDateTime(const wxDateTime& dt)
{
	m_dt = 0.0;
	m_dt = dt;
}

void CNextMDirData::SetItemSelected(const bool bSelected)
{
	m_bSelected = bSelected;
}

void CNextMDirData::SetExt(const wxString& strExt)
{
	m_strExt = strExt;
}

void CNextMDirData::SetDescription(const wxString& strDesc)
{
	m_strDesc = strDesc;
}

wxString CNextMDirData::GetPermission()
{
	if(m_strPermission.IsEmpty())
	{
		CFileSystem fs;
		m_strPermission = fs.GetPermission(GetFullPath());
	}

	return m_strPermission;
}
