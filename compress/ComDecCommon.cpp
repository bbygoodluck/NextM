///////////////////////////////////////////////////////////////////////////////
// Name:        CComDecCommon.cpp
// Purpose:     Compress / Decompress
// Author:      Bae ByoungYong
// Modified by:
// Created:     10.10.2024
// Copyright:   (c) 2024 BaeByoungYong <bbygoodluck@gmail.com>
// Licence:     GPL3
///////////////////////////////////////////////////////////////////////////////

#include "../ginc.h"
#include "ComDecCommon.h"
#include "ComDecImpl.h"
#include "../dialog/GenericDirDialog.h"

std::unique_ptr<CComDecCommon> CComDecCommon::m_pInstance = nullptr;
CComDecCommon* CComDecCommon::Get()
{
	if (m_pInstance.get() == nullptr)
	{
		m_pInstance.reset(new CComDecCommon());
		m_pInstance->Init();
		m_pInstance->CreatePopupMenu();
	}

	return m_pInstance.get();
}

CComDecCommon::~CComDecCommon()
{
	wxDELETE(m_pComDecImpl);
}

void CComDecCommon::Init()
{
	m_pComDecImpl = new CComDecImpl();

	//압축지원파일
	m_vCompressExt.emplace_back(wxT("Zip"));
	m_vCompressExt.emplace_back(wxT("7z"));
	m_vCompressExt.emplace_back(wxT("Gz"));
	m_vCompressExt.emplace_back(wxT("Tar"));
	m_vCompressExt.emplace_back(wxT("Xz"));
	m_vCompressExt.emplace_back(wxT("BZip2"));
	//압축해제 지원파일
	m_vDecompressExt.emplace_back(wxT("zip"));
	m_vDecompressExt.emplace_back(wxT("7z"));
	m_vDecompressExt.emplace_back(wxT("arj"));
	m_vDecompressExt.emplace_back(wxT("bz2"));
	m_vDecompressExt.emplace_back(wxT("cab"));
	m_vDecompressExt.emplace_back(wxT("msi"));
	m_vDecompressExt.emplace_back(wxT("chm"));
	m_vDecompressExt.emplace_back(wxT("gz"));
	m_vDecompressExt.emplace_back(wxT("iso"));
	m_vDecompressExt.emplace_back(wxT("lzh"));
	m_vDecompressExt.emplace_back(wxT("lzma"));
	m_vDecompressExt.emplace_back(wxT("rar"));
	m_vDecompressExt.emplace_back(wxT("tar"));
	m_vDecompressExt.emplace_back(wxT("xz"));
	m_vDecompressExt.emplace_back(wxT("z"));

	m_vDecompressMenu.emplace_back(theMsg->GetMessage(wxT("MSG_DECOMPRESS_CURRENT_FOLDER")));
	m_vDecompressMenu.emplace_back(theMsg->GetMessage(wxT("MSG_DECOMPRESS_CREATE_FOLDER")));
	m_vDecompressMenu.emplace_back(theMsg->GetMessage(wxT("MSG_DECOMPRESS_SEL_FOLDER")));

	wxString strCompressIconPath = theUtility->GetWorkDir() + SLASH + wxT("img") + SLASH;

	wxString strZipIcon        = strCompressIconPath + wxT("compress") + SLASH + wxT("icon-zip.png");
	wxString str7zIcon         = strCompressIconPath + wxT("compress") + SLASH + wxT("icon-7z.png");
	wxString strGzIcon         = strCompressIconPath + wxT("compress") + SLASH + wxT("icon-gz.png");
	wxString strTarIcon        = strCompressIconPath + wxT("compress") + SLASH + wxT("icon-tar.png");
	wxString strXzIcon         = strCompressIconPath + wxT("compress") + SLASH + wxT("icon-xz.png");
	wxString strBZip2Icon      = strCompressIconPath + wxT("compress") + SLASH + wxT("icon-bzip2.png");
	wxString strDeCompressIcon = strCompressIconPath + wxT("compress") + SLASH + wxT("decompress1.png");

	m_compressBmp[0] = wxBitmap(strZipIcon, wxBITMAP_TYPE_PNG);
	m_compressBmp[1] = wxBitmap(str7zIcon, wxBITMAP_TYPE_PNG);
	m_compressBmp[2] = wxBitmap(strGzIcon, wxBITMAP_TYPE_PNG);
	m_compressBmp[3] = wxBitmap(strTarIcon, wxBITMAP_TYPE_PNG);
	m_compressBmp[4] = wxBitmap(strXzIcon, wxBITMAP_TYPE_PNG);
	m_compressBmp[5] = wxBitmap(strBZip2Icon, wxBITMAP_TYPE_PNG);
	m_compressBmp[6] = wxBitmap(strDeCompressIcon, wxBITMAP_TYPE_PNG);
}

void CComDecCommon::CreatePopupMenu()
{
	std::vector<wxString>::iterator ItBegin = m_vCompressExt.begin();
	std::vector<wxString>::iterator ItEnd   = m_vCompressExt.end();

	int iIndex = 0;
	while(ItBegin != ItEnd)
	{
		wxMenuItem* pFindItem = m_PopupMenu.FindItem(COMPRESS_START_ID + iIndex);
		if(pFindItem != nullptr)
			m_PopupMenu.Remove(COMPRESS_START_ID + iIndex);

		wxMenuItem* pMenuItem = m_PopupMenu.Append(COMPRESS_START_ID + iIndex, *ItBegin);
		pMenuItem->SetBitmap(m_compressBmp[iIndex]);

		ItBegin++;
		iIndex++;
	}

	std::vector<wxString>::iterator ItDecBegin = m_vDecompressMenu.begin();
	std::vector<wxString>::iterator ItDecEnd   = m_vDecompressMenu.end();

	iIndex = 0;

	while(ItDecBegin != ItDecEnd)
	{
		wxMenuItem* pFindItem = m_PopupDecompressMenu.FindItem(DECOMPRESS_START_ID + iIndex);
		if(pFindItem != nullptr)
			m_PopupDecompressMenu.Remove(DECOMPRESS_START_ID + iIndex);

		wxMenuItem* pMenuItem = m_PopupDecompressMenu.Append(DECOMPRESS_START_ID + iIndex, *ItDecBegin);
		pMenuItem->SetBitmap(m_compressBmp[6]);

		ItDecBegin++;
		iIndex++;
	}
}

void CComDecCommon::SetOwnerDialog(wxDialog* pDialog)
{
	m_pCompressReleaseDlg = pDialog;
}

void CComDecCommon::Start()
{
	m_pComDecImpl->Run();
}

void CComDecCommon::SetItemsInfo(const std::list<wxString>& lstDatas,
								 const wxString& strCompressedFile,
								 const wxString& strTargetDir,
								 int nType,
								 const wxString& strPassword,
								 bool bCompress)
{
	wxString strTgDir(strTargetDir);

	/* 폴더 선택 후 해제시
	 * ComDecImpl 클래스에서 수행할 경우 쓰레드 내부에서 다이얼로그를 호출하게 되어 충돌 발생
	 * 그래서 폴더 선택 후 압축해제시만 이곳에서 폴더를 선택하게 함..
	*/
	if(!bCompress && nType == (DECOMPRESS_START_ID + 2))
	{
		wxGenericDirDialog dlgTargetPath(_gMainFrame, theMsg->GetMessage(wxT("MSG_GOTODIR_DLG_SEL_FOLDER")), strTgDir);
		dlgTargetPath.SetSize(wxSize(600, 450));
		dlgTargetPath.CenterOnScreen();

		if(dlgTargetPath.ShowModal() == wxID_OK)
			strTgDir = dlgTargetPath.GetPath();
	}

	m_pComDecImpl->SetCompress(bCompress);
	m_pComDecImpl->SetItemsInfo(lstDatas, strCompressedFile, strTgDir, nType, strPassword);
}

bool CComDecCommon::IsIncludeCompressedFileExt(const wxString& strCompressedFileName, int &compressExtIndex)
{
	std::vector<wxString>::const_iterator cItStart = m_vDecompressExt.cbegin();
	std::vector<wxString>::const_iterator cItEnd = m_vDecompressExt.cend();

	wxString strTmp(strCompressedFileName);

	strTmp = strTmp.MakeLower();
	bool bCompressedFile = true;

	int extIndex = 0;
	//압축파일인지 체크
	while(cItStart != cItEnd)
	{
		wxString strExt = (*cItStart);
		strExt = strExt.MakeLower();

		if(strTmp.find(strExt) == wxString::npos)
			bCompressedFile = false;
		else
		{
			bCompressedFile = true;
			break;
		}

		extIndex++;
		++cItStart;
	}

	compressExtIndex = extIndex;
	return bCompressedFile;
}
