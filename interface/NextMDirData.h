#ifndef NEXTMDIRDATA_H_INCLUDED
#define NEXTMDIRDATA_H_INCLUDED

class CNextMDirData
{
public:
	CNextMDirData();
	~CNextMDirData();

public:
	enum item_type {
		unknown = -1,
		dir,
		file,
		drive
	};

	//이름설정
	void SetName(const wxString& strName);

	//경로설정
	void SetPath(const wxString& strPath);

	//타입내용
	void SetType(item_type iType);

	void SetAttribute(unsigned long ulAttr);
	void SetSize(const wxLongLong& llSize);
	void SetDateTime(const wxDateTime& dt);
	void SetItemSelected(const bool bSelected);
	void SetExt(const wxString& strExt);
	void SetDescription(const wxString& strDesc);

	void SetMatch(bool isMatch) { m_bMatched = isMatch; }
	void SetCut(bool isCut)     { m_isCut = isCut; }
	//이름가져오기
	wxString GetName() const { return m_strName; }
	//경로가져오기
	wxString GetPath() const { return m_strPath; }
	//타입명 가져오기
	wxString GetDescription() const { return m_strDesc; }
	//확장자 가져오기
	wxString GetExt()  const {return m_strExt; }

	//전체경로가져오기
	wxString GetFullPath() {
		return ((theUtility->Compare(SLASH, m_strPath.Right(1)) == 0) ? m_strPath + m_strName :  m_strPath + SLASH + m_strName);
	}

	const item_type GetType() { return m_iType; }

	wxLongLong GetSize() const { return m_llSize; }
	wxDateTime GetDateTime() const { return m_dt; }

	const wxString GetSizeString() const     { return m_strSize; }
	const wxString GetSizeTypeString() const { return m_strSizeType; }
	wxColour GetSizeColor()                  { return m_colorType; }

	unsigned long GetAttribute() { return m_ulattr; }
	wxString GetAttributeToString() const {
		wxString strAttr(wxT(""));

		if (m_ulattr & ATTR_RDONLY) strAttr += "R";
		else strAttr += "_";

		//Hidden
		if (m_ulattr & ATTR_HIDDEN) strAttr += "H";
		else strAttr += "_";

		//System
		if (m_ulattr & ATTR_SYSTEM) strAttr += "S";
		else strAttr += "_";

		//Arch
		if (m_ulattr & ATTR_ARCHIVE) strAttr += "A";
		else strAttr += "_";

		return strAttr;
	}

	wxString GetDateTimeToString() const {
		if(!m_dt.IsValid())
			return wxT("0000-00-00 00:00");

		wxLongLong llValue = m_dt.GetValue();
		if(llValue < 0)
			return wxT("0000-00-00 00:00");

		int iYear = m_dt.GetYear();
		int iMonth = m_dt.GetMonth() + 1;
		int iDay = m_dt.GetDay();
		int iHour = m_dt.GetHour();
		int iMin = m_dt.GetMinute();

		return (wxString::Format(TIME_FORMAT_DEFAULT, iYear, iMonth, iDay, iHour, iMin));
	}

	bool IsDir() const { return m_isDir; }
	bool IsFile() const { return m_isFile; }

	const bool IsMatch() const { return m_bMatched; }
	const bool IsCut() const   { return m_isCut; }
	const bool IsItemSelected() const { return m_bSelected; }

	void SetIconIndex(int iIconIndex, int iOverlayIconIndex) {
		m_iIconIndex = iIconIndex;
		m_iOvelayIndex = iOverlayIconIndex;
	}

	void GetIconIndex(int *iIconIndex, int *iOverlayIconIndex) {
		*iIconIndex = m_iIconIndex;
		*iOverlayIconIndex = m_iOvelayIndex;
	}

	void SetImageIconFlag(bool bSet) { m_bPresetIcon = bSet; }
	bool IsSetImageIcon() { return m_bPresetIcon; }

#ifdef __WXMSW__
	bool IsDrive() const { return m_isDrive; }

	void SetDriveType(wxFSVolumeKind fsvKind) { m_fsvDriveKind = fsvKind; }
	wxFSVolumeKind GetDriveType() { return m_fsvDriveKind; }

	void SetVolume(const wxString& strVolume) { m_strVolume = strVolume; }
	wxString GetVolume() const { return m_strVolume; }

	void SetDriveName(const wxString& strDriveName) { m_strDriveName = strDriveName; }
	wxString GetDriveName() { return m_strDriveName; }

	void SetDisplayName(const wxString& strDisplayName) { m_strDisplayName = strDisplayName; }
	wxString GetDisplayName() { return m_strDisplayName; }

	//드라이브 용량정보
	void SetDriveSpaceInfo(const wxString& strDriveSpaceInfo) { m_strDriveSpace = strDriveSpaceInfo; }
	//드라이브 정보 가져오기
	const wxString GetDriveSpaceInfo() const { return m_strDriveSpace; }

	void SetDriveSpace(double dblTotalSpace, double dblUseSpace) {
		m_dblTotalSpace = dblTotalSpace;
		m_dblUseSpace   = dblUseSpace;
	}

	void GetDriveSpace(double& dblTotalSpace, double& dblUseSpace) {
		dblTotalSpace = m_dblTotalSpace;
		dblUseSpace = m_dblUseSpace;
	}

	wxString GetDriveSpace()
	{
		if( (m_fsvDriveKind == wxFS_VOL_CDROM) ||
			(m_fsvDriveKind == wxFS_VOL_DVDROM) )
				return wxT("");

		wxString strTotalSpace = wxString::Format(wxT("%5.1f GB"), (m_dblTotalSpace / GIGABYTE));
		wxString strFreeSpace = wxString::Format(wxT("%5.1f GB"), (m_dblUseSpace / GIGABYTE));

		return (strFreeSpace + wxT(" / ") + strTotalSpace);
	}

#endif

	wxString GetPermission();
private:
	void Init();

private:
	//이름
	wxString m_strName = wxT("");
	//전체경로
	wxString m_strPath = wxT("");

	//타입설명
	wxString m_strTypeName = wxT("");

	//확장자
	wxString m_strExt = wxT("");
	//설명
	wxString m_strDesc = wxT("");

	//파일 사이즈 Type(KB, MB, GB)
	wxString m_strSizeType;
	//파일 사이즈
	wxString m_strSize;
	//사이즈 컬러
	wxColour m_colorType;

	//폴더플래그
	bool m_isDir = false;
	//파일플래그
	bool m_isFile = false;
	//드라이브플래그
	bool m_isDrive = false;
	//선택 플래그
	bool m_bSelected = false;
	//match flag
	bool m_bMatched = false;
	//잘라내기 속성
	bool m_isCut;

	//아이콘 셋팅 플래그
	bool m_bPresetIcon = false;
	//속성
	unsigned long m_ulattr = 999999999;

	//사이즈
	wxLongLong m_llSize = 0;
	//시간
	wxDateTime m_dt = 0.0;
	//아이템종류
	item_type m_iType = item_type::unknown;


	int m_iIconIndex = 0;
	int m_iOvelayIndex = 0;

	wxString m_strPermission = wxT("");

#ifdef __WXMSW__
	wxFSVolumeKind m_fsvDriveKind = wxFS_VOL_MAX;
	//드라이브 용량정보
	wxString m_strDriveSpace;
	//볼륨명
	wxString m_strVolume        = wxT("");
	//드라이브이름
   	wxString m_strDriveName     = wxT("");
   	//화면표시이름
	wxString m_strDisplayName   = wxT("");

	double m_dblTotalSpace = 0.0f;
	double m_dblUseSpace   = 0.0f;
#endif
};

#endif // NEXTMDIRDATA_H_INCLUDED
