#include "../../ginc.h"
#include "DriveInfo.h"

std::unique_ptr<CDriveInfo> CDriveInfo::m_pInstance = nullptr;

CDriveInfo* CDriveInfo::Get()
{
	if (m_pInstance == nullptr)
		m_pInstance.reset(new CDriveInfo());

	return m_pInstance.get();
}


void CDriveInfo::Init()
{
	m_vecDriveList.clear();
	m_vecDriveList.reserve(100);

	wxArrayString arrdrvList = wxFSVolume::GetVolumes();
	int nTotalDriveCount = arrdrvList.GetCount();

	for (int nCnt = 0; nCnt < nTotalDriveCount; nCnt++)
    {
        wxString strDriveItem = arrdrvList.Item(nCnt);

        int iSlash = strDriveItem.find_first_of(SLASH);
        if(iSlash == 0)
			continue;

        wxFSVolume fsv(strDriveItem);

        if(fsv.IsOk())
		{
			wxFSVolumeKind fskind   = fsv.GetKind();
			wxString strDriveName   = fsv.GetName();
			wxString strDisplayName = fsv.GetDisplayName();
			wxString strVolume      = strDriveName.Left(1);

			bool IsNetDrive = (fskind == wxFS_VOL_NETWORK) ? true : false;

			wxString strDisplay = MakeDisplayName(strVolume, strDisplayName, IsNetDrive);

		//	m_strMaxDisp = theUtility->GetMaxData(_gMainFrame, *_gViewFont, strDisplay, m_strMaxDisp);

			double dblTotalSpace = 0.0f;
			double dblFreeSpace  = 0.0f;

			GetDiskSpace(strDriveName, dblTotalSpace, dblFreeSpace);

			int iIconIndex = -1;
			int iOverlayIconIndex = -1;

			theImageList->GetIconIndex(strDriveName, iIconIndex, iOverlayIconIndex);

			CNextMDirData& data = m_vecDriveList.emplace_back();

			data.SetVolume(strVolume);
			data.SetName(strDisplay);
			data.SetDriveName(strDriveName);
			data.SetDisplayName(strDisplay);
			data.SetDriveType(fskind);
			data.SetType(CNextMDirData::item_type::drive);
			data.SetDriveSpace(dblTotalSpace, dblFreeSpace);
			data.SetIconIndex(iIconIndex, iOverlayIconIndex);

//			m_vecDriveList.push_back(data);
		}
    }
}

wxString CDriveInfo::MakeDisplayName(const wxString& strVolume, const wxString& strVolumeDisplayName, bool IsNetDrive)
{
	int nDisplayStartIndx = strVolumeDisplayName.find_first_of(wxT("("));
	wxString strDisplayDiskType = strVolumeDisplayName.Left(nDisplayStartIndx - 1);

	if(IsNetDrive)
		strDisplayDiskType = strDisplayDiskType.insert(0, wxT("NET "));

	wxString strDisp = nDisplayStartIndx == -1 ? strDisplayDiskType : wxString::Format(wxT("[-%s-] %s"), strVolume, strDisplayDiskType);
	return strDisp;
}

void CDriveInfo::GetDiskSpace(const wxString& strVolume, double& dblTotalSpace, double& dblFreeSpace, bool bPhysicalDiskRead)
{
	if(bPhysicalDiskRead)
	{
		wxDiskspaceSize_t total;
		wxDiskspaceSize_t free;

		wxGetDiskSpace(strVolume, &total, &free);

		dblTotalSpace = total.ToDouble();
		dblFreeSpace = free.ToDouble();
	}
	else
	{
		std::vector<CNextMDirData>::iterator it = GetDriveItem(strVolume);
		it->GetDriveSpace(dblTotalSpace, dblFreeSpace);
	}
}

std::vector<CNextMDirData>::iterator CDriveInfo::GetDriveItem(const wxString& strVolume)
{
	std::vector<CNextMDirData>::iterator it;
	for(it = m_vecDriveList.begin(); it != m_vecDriveList.end(); ++it)
	{
		if(strVolume.CmpNoCase(it->GetVolume()) == 0)
			break;
	}

	return it;
}

void CDriveInfo::UpdateDriveSpace(const wxString& strVolume)
{
	std::vector<CNextMDirData>::iterator it;
	for(it = m_vecDriveList.begin(); it != m_vecDriveList.end(); ++it)
	{
		if(strVolume.CmpNoCase(it->GetVolume()) == 0)
		{
			double dblTotalSpace = 0.0f;
			double dblFreeSpace  = 0.0f;

			GetDiskSpace(it->GetDriveName(), dblTotalSpace, dblFreeSpace);
			it->SetDriveSpace(dblTotalSpace, dblFreeSpace);

			break;
		}
	}
}
