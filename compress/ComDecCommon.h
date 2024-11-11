#ifndef CDCOMMON_H_INCLUDED
#define CDCOMMON_H_INCLUDED

///////////////////////////////////////////////////////////////////////////////
// Name:        CComDecCommon.h
// Purpose:     Compress / Decompress
// Author:      Bae ByoungYong
// Modified by:
// Created:     10.10.2024
// Copyright:   (c) 2024 BaeByoungYong <bbygoodluck@gmail.com>
// Licence:     GPL3
///////////////////////////////////////////////////////////////////////////////

class CComDecImpl;
class CComDecCommon
{
private:
	CComDecCommon() {}

public:
	~CComDecCommon();
	static CComDecCommon* Get();

	const std::vector<wxString>& GetCompressList(bool bCompress = true) const
	{
		if(bCompress)
			return m_vCompressExt;

		return m_vDecompressMenu;
	}

	void SetOwnerDialog(wxDialog* pDialog);
	wxDialog* GetOwnerDialog() { return m_pCompressReleaseDlg; }
	void Start();

	wxMenu* GetPopupMenu(bool bDecompress = false) {
		if(!bDecompress)
			return &m_PopupMenu;

		return &m_PopupDecompressMenu;
	}

	void SetItemsInfo(const std::list<wxString>& lstDatas, const wxString& strCompressedFile, const wxString& strTargetDir, int nType, const wxString& strPassword, bool bCompress = true);

	void SetImmediatelyCompress(bool bImmediately = false) {
		m_bImmediately = bImmediately;
	}

	const IsImmediatelyCompress() const { return m_bImmediately; }

	void SetCancel(bool bCancel = false) {
		m_bCancel = bCancel;
	}

	const IsCancel() const { return m_bCancel; }

	bool IsIncludeCompressedFileExt(const wxString& strCompressedFileName, int &compressExtIndex);

private:
	void Init();
	void CreatePopupMenu();

private:
	std::vector<wxString> m_vCompressExt;
	std::vector<wxString> m_vDecompressMenu;
	std::vector<wxString> m_vDecompressExt;

	wxBitmap m_compressBmp[7];

	wxMenu m_PopupMenu;
	wxMenu m_PopupDecompressMenu;
	wxDialog* m_pCompressReleaseDlg = nullptr;

	CComDecImpl* m_pComDecImpl = nullptr;
	bool m_bImmediately = false;

	bool m_bCancel = false;

private:
	static std::unique_ptr<CComDecCommon> m_pInstance;
};

#endif // CDCOMMON_H_INCLUDED
