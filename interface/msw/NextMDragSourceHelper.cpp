#include "../../ginc.h"
#include "NextMDragSourceHelper.h"

CNextMDragSourceHelper::CNextMDragSourceHelper()
{
	Init();
}

CNextMDragSourceHelper::~CNextMDragSourceHelper()
{
	Release();
}

void CNextMDragSourceHelper::Init()
{
	if(FAILED(CoCreateInstance(CLSID_DragDropHelper,
							   nullptr,
                               CLSCTX_INPROC_SERVER,
                               IID_IDragSourceHelper,
                               (void**)&pDragSourceHelper)))
		pDragSourceHelper = nullptr;
}

void CNextMDragSourceHelper::Release()
{
	if( pDragSourceHelper != nullptr )
	{
		pDragSourceHelper->Release();
		pDragSourceHelper = nullptr;
	}
}

HRESULT CNextMDragSourceHelper::InitializeFromBitmap(const wxString& strFileName, IDataObject* pDataObject)
{
	HRESULT hr = S_OK;

	if(pDragSourceHelper == nullptr)
		return E_FAIL;

	HIMAGELIST himl;
	SHDRAGIMAGE sdi;
	SIZE sz;

	ZeroMemory(&sdi, sizeof(SHDRAGIMAGE));

	SHFILEINFO sfi = {};

	int uFlag = SHGFI_ICON | SHGFI_SMALLICON;
	uFlag ^= SHGFI_SMALLICON;

	himl = (HIMAGELIST)SHGetFileInfo(strFileName, 0, &sfi, sizeof(sfi), uFlag | SHGFI_SYSICONINDEX);
	if(himl)
	{
		int cx, cy;
		ImageList_GetIconSize(himl, &cx, &cy);

		sdi.sizeDragImage.cx = cx;
		sdi.sizeDragImage.cy = cy;

		sdi.ptOffset.x = cx;
		sdi.ptOffset.y = cy;

		sdi.hbmpDragImage = CreateBitmap(cx, cy, 1, 32, nullptr);
		sdi.crColorKey = GetSysColor(COLOR_WINDOW);

		HDC hDC = CreateCompatibleDC(NULL);
		if(sdi.hbmpDragImage)
		{
			HBITMAP hbmPrev = (HBITMAP)SelectObject(hDC, (HGDIOBJ)sdi.hbmpDragImage);
			ImageList_Draw(himl, sfi.iIcon, hDC, 0, 0, ILD_SCALE);
			SelectObject(hDC, hbmPrev);

			pDragSourceHelper->InitializeFromBitmap(&sdi, pDataObject);

			DeleteDC(hDC);
			DeleteObject(sdi.hbmpDragImage);
		}
	}

	return hr;
}

HRESULT CNextMDragSourceHelper::InitializeFromWindow(HWND hwnd, const wxPoint& pt,IDataObject* pDataObject)
{
	if(pDragSourceHelper == nullptr)
		return E_FAIL;

	POINT pt1;
	pt1.x = pt.x;
	pt1.y = pt.y;

	return pDragSourceHelper->InitializeFromWindow(hwnd, &pt1, pDataObject);
}
