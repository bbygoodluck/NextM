#include "../ginc.h"
#include "../dialog/DlgCompress.h"
#include "../dialog/DlgDeCompress.h"

#include "ComDecImpl.h"

CComDecImpl::CComDecImpl()
	: wxThreadHelper(wxTHREAD_JOINABLE)
{
	wxString strWork = theUtility->GetWorkDir();
	wxString str7zLibDir = strWork + SLASH + wxT("7z") + SLASH;

	strSevenZip = str7zLibDir + wxT("7z.dll");
}

CComDecImpl::~CComDecImpl()
{
	if(GetThread() && GetThread()->IsRunning())
		GetThread()->Wait();
}

void CComDecImpl::Run()
{
	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
    {
        wxLogError("Could not create the worker thread!");
        return;
    }

	GetThread()->Run();
}

wxThread::ExitCode CComDecImpl::Entry()
{
	if(m_lstDatas.size() == 0)
		(wxThread::ExitCode)0;


	wxDialog* pOwnerDialog = nullptr;
	bit7z::Bit7zLibrary lib(CONVSTR_TO_STD(strSevenZip));

	if(IsCompress())
	{
		pOwnerDialog = (DlgCompress *)theComDec->GetOwnerDialog();
		switch(m_nType)
		{
			case COMPRESS_FILE_ZIP :
			{
				bit7z::BitArchiveWriter archive(lib, bit7z::BitFormat::Zip);
				DoCompress(archive, pOwnerDialog);
			}
				break;

			case COMPRESS_FILE_7Z :
			{
				bit7z::BitArchiveWriter archive(lib, bit7z::BitFormat::SevenZip);
				DoCompress(archive, pOwnerDialog);
			}
				break;

			case COMPRESS_FILE_GZ :
			{
				bit7z::BitArchiveWriter archive(lib, bit7z::BitFormat::GZip);
				DoCompress(archive, pOwnerDialog);
			}
				break;

			case COMPRESS_FILE_TAR :
			{
				bit7z::BitArchiveWriter archive(lib, bit7z::BitFormat::Tar);
				DoCompress(archive, pOwnerDialog);
			}
				break;

			case COMPRESS_FILE_XZ :
			{
				bit7z::BitArchiveWriter archive(lib, bit7z::BitFormat::Xz);
				DoCompress(archive, pOwnerDialog);
			}
				break;

			case COMPRESS_FILE_BZIP2 :
			{
				bit7z::BitArchiveWriter archive(lib, bit7z::BitFormat::BZip2);
				DoCompress(archive, pOwnerDialog);
			}
				break;

			default:
				break;
		}
	}
	else
	{
		InitDecompressInformation();
		pOwnerDialog = (DlgDeCompress *)theComDec->GetOwnerDialog();

		switch(m_nType)
		{
			case _DECOMPRESS_EXT_ZIP :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Zip);
			//	bit7z::BitArchiveReader reader(lib, CONVSTR_TO_STD(m_strCompressedFile), bit7z::BitFormat::Zip);
			//	DoDecompress(reader, pOwnerDialog);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_7Z :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::SevenZip);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_ARJ :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Arj);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_BZ2 :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::BZip2);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_CAB :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Cab);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_CHM :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Chm);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_GZ :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::GZip);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_ISO :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Iso);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_LZH :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Lzh);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_LZMA :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Lzma);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_RAR :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Rar);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_TAR :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Tar);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_XZ :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Xz);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			case _DECOMPRESS_EXT_Z :
			{
				bit7z::BitFileExtractor extractor(lib, bit7z::BitFormat::Z);
				DoDecompress(extractor, pOwnerDialog);
			}
				break;

			default:
				break;
		}
	}

	wxQueueEvent(pOwnerDialog, new wxThreadEvent());
	return (wxThread::ExitCode)0;
}

void CComDecImpl::DoCompress(bit7z::BitArchiveWriter& archive, wxDialog* pOwnerDialog)
{
	std::list<wxString>::const_iterator cItStart = m_lstDatas.begin();
	std::list<wxString>::const_iterator cItEnd = m_lstDatas.end();

	while(cItStart != cItEnd)
	{
		wxString strItem = *cItStart;

		bool bDir = false;
#ifdef __WXMSW__
		DWORD result = GetFileAttributes(strItem);
		if(result & FILE_ATTRIBUTE_DIRECTORY)
			bDir = true;
#else
#endif // __WXMSW__

		if(bDir)
			archive.addDirectory(CONVSTR_TO_STD(strItem));
		else
			archive.addFile(CONVSTR_TO_STD(strItem));

		++cItStart;
	}

	archive.setTotalCallback([pOwnerDialog](uint64_t total_size) {
		((DlgCompress *)pOwnerDialog)->SetCurrentFileTotalSize(total_size);
	});

	archive.setProgressCallback([pOwnerDialog](uint64_t progressed_size) -> bool {
		((DlgCompress *)pOwnerDialog)->SetCurrentFileSize(progressed_size);
		return true;
	});

	archive.setFileCallback([pOwnerDialog](std::wstring file_path) {
		wxString strPath(file_path);
		((DlgCompress *)pOwnerDialog)->SetCurrentFile(strPath);
	});

	if(!m_strPassword.IsEmpty())
		archive.setPassword(CONVSTR_TO_STD(m_strPassword));

	archive.compressTo(CONVSTR_TO_STD(m_strCompressedFile));
}

void CComDecImpl::InitDecompressInformation()
{
	switch(m_nType)
	{
		case DECOMPRESS_MAKE_FOLDER:
			MakeExtractDir();
			break;

		default:
			break;
	}

	//압축파일 확장자 체크
	int extIndex = -1;
	theComDec->IsIncludeCompressedFileExt(m_strCompressedFile, extIndex);
	m_nType = extIndex;
}

void CComDecImpl::MakeExtractDir()
{
	wxString strFileName = theUtility->GetFileName(m_strCompressedFile, false);
	wxString strTmpDir(m_strTargetDir);

	m_strTargetDir = (m_strTargetDir.Right(1).CmpNoCase(SLASH) == 0) ? strTmpDir + strFileName :
    	                                                               strTmpDir + SLASH + strFileName;

	if(!wxDirExists(m_strTargetDir))
		wxMkDir(m_strTargetDir);
}

void CComDecImpl::DoDecompress(bit7z::BitFileExtractor& extractor, wxDialog* pOwnerDialog)
{
	extractor.setOverwriteMode(bit7z::OverwriteMode::Overwrite);

	extractor.setTotalCallback([pOwnerDialog](uint64_t total_size) {
		((DlgDeCompress *)pOwnerDialog)->SetExtractTotal(total_size);
	});

	extractor.setProgressCallback([pOwnerDialog](uint64_t progressed_size) -> bool {
		((DlgDeCompress *)pOwnerDialog)->SetExtractCurrent(progressed_size);
		return true;
	});

	extractor.setFileCallback([pOwnerDialog](std::wstring file_path) {
		wxString strPath(file_path);
		((DlgDeCompress *)pOwnerDialog)->SetExtractCurrentFile(strPath);
	});
/*
	extractor.setPasswordCallback([extractor]() -> std::wstring) {
		if(extractor.isPasswordDefined())
		{
			return CONVSTR_TO_STD(wxString("1111"));
		}

		return CONVSTR_TO_STD(wxString(""));
	});
	*/
	extractor.extract(CONVSTR_TO_STD(m_strCompressedFile), CONVSTR_TO_STD(m_strTargetDir));
}

void CComDecImpl::DoDecompress(bit7z::BitArchiveReader& reader, wxDialog* pOwnerDialog)
{
	if(reader.isPasswordDefined())
	{
		wxMessageBox(wxT("패스워드를 입력하세요.."));
		return;
	}

	reader.extractTo(CONVSTR_TO_STD(m_strTargetDir));
}

void CComDecImpl::SetItemsInfo(const std::list<wxString>& lstDatas, const wxString& strCompresssedFile, const wxString& strTargetDir, int nType, const wxString& strPassword)
{
	m_lstDatas          = lstDatas;
	m_strCompressedFile = strCompresssedFile;
	m_strTargetDir     = strTargetDir;
	m_nType             = nType;
	m_strPassword       = strPassword;
}
