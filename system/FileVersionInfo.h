#ifndef _FILE_VERSION_INFO_H_INCLUDED
#define _FILE_VERSION_INFO_H_INCLUDED

/*
 * 참조 : http://www.cplusplus.com/forum/windows/181615/
*/
class CFileVersionInfo
{
public:
	CFileVersionInfo();
	~CFileVersionInfo();

public:
	bool Create(const wxString& lpszFileName);
#ifdef __WXMSW__
	WORD GetFileVersion(int nIndex) const;
	WORD GetProductVersion(int nIndex) const;
	DWORD GetFileFlagsMask() const;
	DWORD GetFileFlags() const;
	DWORD GetFileOs() const;
	DWORD GetFileType() const;
	DWORD GetFileSubtype() const;
#endif
	wxString GetCompanyName() const;
	wxString GetFileDescription() const;
	wxString GetFileVersion() const;
	wxString GetInternalName() const;
	wxString GetLegalCopyright() const;
	wxString GetOriginalFileName() const;
	wxString GetProductName() const;
	wxString GetProductVersion() const;
	wxString GetComments() const;
	wxString GetLegalTrademarks() const;
	wxString GetPrivateBuild() const;
	wxString GetSpecialBuild() const;

protected:
	void Reset();
#ifdef __WXMSW__
	bool GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, bool bPrimaryEnough = false);
#endif
private:
#ifdef __WXMSW__
	VS_FIXEDFILEINFO m_FileInfo;
#endif
	wxString m_strCompanyName;
	wxString m_strFileDescription;
	wxString m_strFileVersion;
	wxString m_strInternalName;
	wxString m_strLegalCopyright;
	wxString m_strOriginalFileName;
	wxString m_strProductName;
	wxString m_strProductVersion;
	wxString m_strComments;
	wxString m_strLegalTrademarks;
	wxString m_strPrivateBuild;
	wxString m_strSpecialBuild;

};
#endif
