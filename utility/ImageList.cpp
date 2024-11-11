#include "../ginc.h"
#include "ImageList.h"

std::unique_ptr<CImageList> CImageList::m_pImageListInstance(nullptr);

CImageList::CImageList()
{

}

CImageList::~CImageList()
{

}

CImageList* CImageList::Get()
{
	if (m_pImageListInstance.get() == NULL)
		m_pImageListInstance.reset(new CImageList());

	return m_pImageListInstance.get();
}

bool CImageList::Create(int width, int height, int flags)
{
#ifdef __WXMSW__
	ImageList_Create(width, height, flags, 1, 1);
#else
#endif // __WXMSW__

	return true;
}

#ifdef __WXMSW__
void CImageList::Attach(HIMAGELIST hImageList)
{
	m_bSetHImageList = true;
	m_hImageList = hImageList;
}
#else
void CImageList::Attach(wxImageList* pImageList)
{
	m_bSetHImageList = false;
	m_pImageList = pImageList;
}
#endif

void CImageList::LoadImageList()
{
#ifdef __WXMSW__
	// IID_IImageList {46EB5926-582E-4017-9FDF-E8998DAA0950}
	static const GUID IID_IImageList = { 0x46EB5926, 0x582E, 0x4017, { 0x9F, 0xDF, 0xE8, 0x99, 0x8D, 0xAA, 0x9, 0x50 } };
	// IID_IImageList2 {192B9D83-50FC-457B-90A0-2B82A8B5DAE1}
	static const GUID IID_IImageList2 = { 0x192B9D83, 0x50FC, 0x457B, { 0x90, 0xA0, 0x2B, 0x82, 0xA8, 0xB5, 0xDA, 0xE1 } };

	_gImageList = nullptr;
	HRESULT res = SHGetImageList(SHIL_SMALL, IID_IImageList2, (void**)& _gImageList);
	if (FAILED(res))
		res = SHGetImageList(SHIL_SMALL, IID_IImageList, (void**)& _gImageList);
#else
	_gImageList = wxTheFileIconsTable->GetSmallImageList();
#endif
}

void CImageList::GetIconIndex(const wxString& strPath, int &nIconIndex, int &nOverlayIndex, bool bExtFind)
{
#ifdef __WXMSW__
	nIconIndex = 0;
	nOverlayIndex = 0;

	DWORD dwNum = GetFileAttributes(strPath);
	DWORD attr = 0;

	attr = dwNum & FILE_ATTRIBUTE_DIRECTORY ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL;

	SHFILEINFO sfi;
	wxZeroMemory(sfi);

	UINT flag = IMAGELIST_FLAG;

	if(bExtFind)
	{
		attr = 0;
		flag |= SHGFI_USEFILEATTRIBUTES;
	}

	SHGetFileInfo(strPath, attr, &sfi, sizeof(sfi), flag);

	nIconIndex = (sfi.iIcon & 0x00FFFFFF);
	nOverlayIndex = (sfi.iIcon >> 24) - 1;

	DestroyIcon(sfi.hIcon);
#else

#endif
}

bool CImageList::Draw(int index, wxDC* pDC, int x, int y, int flags)
{
#ifdef __WXMSW__
	if(!m_hImageList)
	{
		wxMessageBox(wxT("HIMAGELIST is null"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return false;
	}

	HDC hdc = wx_static_cast(HDC, pDC->GetHDC());
	bool bDraw = ImageList_DrawEx(m_hImageList
		, index
		, hdc
		, x
		, y
		, 16
		, 16
		, RGB(0, 0, 0)
		, RGB(0, 0, 0)
		, flags);

	pDC->ReleaseHDC(hdc);
	return bDraw;
#else
	if(!m_bSetHImageList)
	{
		if(!m_pImageList)
		{
			wxMessageBox(wxT("wxImageList is null"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
			return false;
		}

		return m_pImageList->Draw(index, *pDC, x, y, flags);
	}

#endif // __WXMSW__
}
