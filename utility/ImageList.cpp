#include "../ginc.h"
#include "ImageList.h"

CImageList::CImageList()
{

}

CImageList::~CImageList()
{

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
