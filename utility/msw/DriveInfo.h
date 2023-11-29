#ifndef DRIVEINFO_H_INCLUDED
#define DRIVEINFO_H_INCLUDED

#include "../../interface/NextMDirData.h"
class CDriveInfo final
{
private:
    CDriveInfo() {}

public:
    ~CDriveInfo() {}
    static CDriveInfo* Get();

public:
    void Init();

	void GetDiskSpace(const wxString& strVolume, double& dblTotalSpace, double& dblFreeSpace, bool bPhysicalDiskRead = true);
	void UpdateDriveSpace(const wxString& strVolume);

	std::vector<CNextMDirData>::iterator GetDriveItem(const wxString& strVolume);

	std::vector<CNextMDirData>::iterator BeginIter() { return m_vecDriveList.begin(); }
	std::vector<CNextMDirData>::iterator EndIter() { return m_vecDriveList.end(); }

	int GetDiskDriveCount() { return m_vecDriveList.size(); }

	wxString GetMaxDriveDisp() { return m_strMaxDisp; }
private:
	wxString MakeDisplayName(const wxString& strVolume, const wxString& strVolumeDisplayName, bool IsNetDrive);

private:
	static std::unique_ptr<CDriveInfo> m_pInstance;
	std::vector<CNextMDirData> m_vecDriveList;

	wxString m_strMaxDisp = wxT("");
	int m_nDriveCount = 0;
};

#endif // DRIVEINFO_H_INCLUDED
