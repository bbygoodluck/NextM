#ifndef COMDECIMPL_H_INCLUDED
#define COMDECIMPL_H_INCLUDED

#include "../external/include/bit7z/bit7z.hpp"

class CComDecImpl : public wxThreadHelper
{
	enum _DECOMPRESS_OP {
		DECOMPRESS_CURRENT_FOLDER = DECOMPRESS_START_ID,
		DECOMPRESS_MAKE_FOLDER,
		DECOMPRESS_SEL_FOLDER
	};

	enum _DECOMPRESS_EXT {
		_DECOMPRESS_EXT_ZIP = 0,
		_DECOMPRESS_EXT_7Z,
		_DECOMPRESS_EXT_ARJ,
		_DECOMPRESS_EXT_BZ2,
		_DECOMPRESS_EXT_CAB,
		_DECOMPRESS_EXT_CHM,
		_DECOMPRESS_EXT_GZ,
		_DECOMPRESS_EXT_ISO,
		_DECOMPRESS_EXT_LZH,
		_DECOMPRESS_EXT_LZMA,
		_DECOMPRESS_EXT_RAR,
		_DECOMPRESS_EXT_TAR,
		_DECOMPRESS_EXT_XZ,
		_DECOMPRESS_EXT_Z,
	};
public:
	CComDecImpl();
	~CComDecImpl();

	void Run();
	void SetCompress(bool bCompress = true) {
		m_bCompress = bCompress;
	}

	const IsCompress() const { return m_bCompress; }
	void SetItemsInfo(const std::list<wxString>& lstDatas, const wxString& strCompresssedFile, const wxString& strTargetDir, int nType, const wxString& strPassword);

private:
	void InitDecompressInformation();
	void DoCompress(const bit7z::BitArchiveWriter& archive, wxDialog* pOwnerDialog);
	void DoDecompress(const bit7z::BitFileExtractor& extractor, wxDialog* pOwnerDialog);
	void DoDecompress(const bit7z::BitArchiveReader& reader, wxDialog* pOwnerDialog);

	void MakeExtractDir();

protected:
	wxThread::ExitCode Entry() wxOVERRIDE;

	wxString strSevenZip = wxT("");

	bool m_bCompress = true;

	std::list<wxString> m_lstDatas;
	wxString m_strCompressedFile = wxT("");
	wxString m_strPassword       = wxT("");
	wxString m_strTargetDir      = wxT("");

	int m_nType                  = -1;

};

#endif // COMDECIMPL_H_INCLUDED
