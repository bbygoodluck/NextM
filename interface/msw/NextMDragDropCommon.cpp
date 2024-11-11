#include "../../ginc.h"
#include "../../NextMExecuteMenuOperation.h"

#include "NextMDragDropCommon.h"
#include "NextMDropSource.h"
#include "NextMDataObject.h"
#include "NextMEnumFormatEtc.h"

std::unique_ptr<CNextMDnDCommon> CNextMDnDCommon::m_pInstance(nullptr);

CNextMDnDCommon::CNextMDnDCommon()
{

}

CNextMDnDCommon::~CNextMDnDCommon()
{

}

CNextMDnDCommon* CNextMDnDCommon::Get()
{
	if (m_pInstance.get() == NULL)
		m_pInstance.reset(new CNextMDnDCommon());

	return m_pInstance.get();
}

#ifdef __WXMSW__
HRESULT CNextMDnDCommon::CreateDataObject(FORMATETC* pFmtetc, STGMEDIUM* pStgMedium, int count, IDataObject** ppDataObject)
{
	if(ppDataObject == 0)
		return E_INVALIDARG;

	*ppDataObject = new CNextMDataObject(pFmtetc, pStgMedium, count);
	((CNextMDataObject *)*ppDataObject)->AddRef();

	return (*ppDataObject) ? S_OK : E_OUTOFMEMORY;
}

HRESULT CNextMDnDCommon::CreateEnumFormatEtc(UINT nNumFormats, FORMATETC* pFormatEtc, IEnumFORMATETC** ppEnumFormatEtc)
{
	if(nNumFormats == 0 || pFormatEtc == 0 || ppEnumFormatEtc == 0)
		return E_INVALIDARG;

	*ppEnumFormatEtc = new CNextMEnumFormatEtc(pFormatEtc, nNumFormats);
	return (ppEnumFormatEtc) ? S_OK : E_OUTOFMEMORY;
}

HRESULT CNextMDnDCommon::CreateDropSource(IDropSource **ppDropSource, wxWindow* pDragWindow)
{
	if(ppDropSource == 0)
		return E_INVALIDARG;

	*ppDropSource = new CNextMDropSource();
	((CNextMDropSource *)*ppDropSource)->AddRef();

	m_pDragWindow = pDragWindow;

	return (*ppDropSource) ? S_OK : E_OUTOFMEMORY;
}
#endif

bool CNextMDnDCommon::FindDropWindowItem(const wxPoint& pt, wxWindow* pDropWindow)
{
	bool bRet = true;
	/*
	   1 : dir
	   2 : file
	   3 : drive
	   0 : etc
	*/
	bool bFound = ((CFileListView *)pDropWindow)->FindItemInMousePoint(pt);
	if(bFound)
	{
		int itemType = ((CFileListView *)pDropWindow)->DropWindowSelectItemType();
		if(itemType == 1)
			UpdateDropWindow(pDropWindow);
		else
			bRet = false;
	}
	else
		bRet = false;

	return bRet;
}

void CNextMDnDCommon::UpdateDropWindow(wxWindow* pDropWindow)
{
	((CFileListView *)pDropWindow)->SetDnDUpdate();
}

void CNextMDnDCommon::SendDragDropOperationEvent()
{
	_gMenuEvent->GetMenuOperationPtr()->DragDropOperation();
}
