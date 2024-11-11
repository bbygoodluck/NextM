#include "../../ginc.h"
#include "NextMDropTarget.h"

CNextMDropTarget::CNextMDropTarget(HWND hwnd)
	: m_hWnd(hwnd)
	, m_lRefCount(0)
	, m_pDropTargetHelper(nullptr)
{
	if(FAILED(CoCreateInstance(CLSID_DragDropHelper,
							   NULL,
							   CLSCTX_INPROC_SERVER,
							   IID_IDropTargetHelper,
							   (LPVOID*)&m_pDropTargetHelper)))
	{
		m_pDropTargetHelper = nullptr;
	}
}

CNextMDropTarget::~CNextMDropTarget()
{
	if(m_pDropTargetHelper)
		m_pDropTargetHelper = nullptr;
}

// IUnknown implementation
HRESULT __stdcall CNextMDropTarget::QueryInterface (REFIID iid, void ** ppvObject)
{
	if(IsEqualIID(iid, IID_IDropTarget) || IsEqualIID(iid, IID_IUnknown))
		*ppvObject = this;
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

ULONG	__stdcall CNextMDropTarget::AddRef (void)
{
	return InterlockedIncrement(&m_lRefCount);
}

ULONG	__stdcall CNextMDropTarget::Release (void)
{
	LONG count = InterlockedDecrement(&m_lRefCount);

	if(count == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return count;
	}
}

// IDropTarget implementation
HRESULT __stdcall CNextMDropTarget::DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
    FORMATETC fmtetc;

    fmtetc.cfFormat = CF_HDROP;
    fmtetc.ptd      = NULL;
    fmtetc.dwAspect = DVASPECT_CONTENT;
    fmtetc.lindex   = -1;
    fmtetc.tymed    = TYMED_HGLOBAL;

    // Does the drag source provide CF_HDROP, which is the only format we accept.
    m_bAllowDrop = (NOERROR == pDataObject->QueryGetData(&fmtetc)) ? true : false;
	if(m_bAllowDrop)
	{
		QueryDrop(grfKeyState, pdwEffect);
		theDnD->SetStartDrag(true);
	}

	if (m_pDropTargetHelper)
		m_pDropTargetHelper->DragEnter(m_hWnd, pDataObject, (LPPOINT)&pt, *pdwEffect);

	return S_OK;
}

HRESULT __stdcall CNextMDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	if(pdwEffect == 0)
		return E_POINTER;

	if(m_bAllowDrop)
	{
		QueryDrop(grfKeyState, pdwEffect);

		if (m_pDropTargetHelper)
			m_pDropTargetHelper->DragOver((LPPOINT)&pt, *pdwEffect);

		POINT point;
		point.x = pt.x;
		point.y = pt.y;

		ScreenToClient(m_pTargetWindow->GetHWND(), &point);

		wxPoint ptFinal(point.x, point.y);
		m_bDropDir = theDnD->FindDropWindowItem(ptFinal, m_pTargetWindow);
	}

	return S_OK;
}

HRESULT __stdcall CNextMDropTarget::DragLeave(void)
{
	if(m_pDropTargetHelper)
		m_pDropTargetHelper->DragLeave();

	theDnD->SetStartDrag(false);
	theDnD->SetDropped(false);

	theDnD->UpdateDropWindow(m_pTargetWindow);
	m_bAllowDrop = false;

	return S_OK;
}

HRESULT __stdcall CNextMDropTarget::Drop (IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	if (pDataObject == nullptr)
		return E_INVALIDARG;

	if(pdwEffect == 0)
		return E_POINTER;

	if(QueryDrop(grfKeyState, pdwEffect))
	{
		FORMATETC fmtetc;
		STGMEDIUM medium;

		if(m_bAllowDrop)
		{
			fmtetc.cfFormat = CF_HDROP;
			fmtetc.ptd = NULL;
			fmtetc.dwAspect = DVASPECT_CONTENT;
			fmtetc.lindex = -1;
			fmtetc.tymed = TYMED_HGLOBAL;

			HRESULT hr = pDataObject->GetData(&fmtetc, &medium);
			if(FAILED(hr))
				return DROPEFFECT_NONE;

			if ((fmtetc.cfFormat == CF_HDROP) && (medium.tymed == TYMED_HGLOBAL))
			{
				HDROP hdrop = (HDROP)GlobalLock(medium.hGlobal);
				if (hdrop == NULL)
					return DROPEFFECT_NONE;

				UINT numOfFiles = DragQueryFile((HDROP)hdrop, 0xFFFFFFFF, NULL, 0);
				if (numOfFiles <= 0)
					return DROPEFFECT_NONE;

				theDnD->ClearDatas();
				TCHAR buffer[MAX_PATH] = { '\0', };

				for (UINT i = 0; i < numOfFiles; i++)
				{
					ZeroMemory(buffer, MAX_PATH);
					UINT charsCopied = DragQueryFile((HDROP)hdrop, i, buffer, MAX_PATH);
					wxString strDropItem(buffer);

					theDnD->AddDropItem(strDropItem);
				}

				GlobalUnlock(medium.hGlobal);

				theDnD->SetDropOperation(m_pTargetWindow, *pdwEffect, m_bDropDir);
				theDnD->SendDragDropOperationEvent();
				theDnD->SetDropped(true);

				theUtility->SetTopWindow(HWND_TOPMOST);
			}

			ReleaseStgMedium(&medium);
		}
	}

	if (m_pDropTargetHelper)
		m_pDropTargetHelper->Drop(pDataObject, (LPPOINT)&pt, *pdwEffect);

	theDnD->SetStartDrag(false);
	theDnD->UpdateDropWindow(m_pTargetWindow);

	_gMainFrame->SetFocus();
	theUtility->SetTopWindow(HWND_NOTOPMOST);

	theDnD->SetDropped(false);

	m_bAllowDrop = false;
	*pdwEffect = DROPEFFECT_NONE;

	return S_OK;
}

bool CNextMDropTarget::QueryDrop(DWORD grfKeyState, LPDWORD pdwEffect)
{
	DWORD dwOKEffects = *pdwEffect;

	if(!m_bAllowDrop)
	{
		*pdwEffect = DROPEFFECT_NONE;
		return false;
	}
	//CTRL+SHIFT  -- DROPEFFECT_LINK
	//CTRL        -- DROPEFFECT_COPY
	//SHIFT       -- DROPEFFECT_MOVE
	//no modifier -- DROPEFFECT_MOVE or whatever is allowed by src
 	*pdwEffect = (grfKeyState & MK_CONTROL) ?
				 ( (grfKeyState & MK_SHIFT) ? DROPEFFECT_LINK : DROPEFFECT_COPY ):
				 ( (grfKeyState & MK_SHIFT) ? DROPEFFECT_MOVE : 0 );

	if(*pdwEffect == 0)
	{
		// No modifier keys used by user while dragging.
		if (DROPEFFECT_COPY & dwOKEffects)
			*pdwEffect = DROPEFFECT_COPY;
		else if (DROPEFFECT_MOVE & dwOKEffects)
			*pdwEffect = DROPEFFECT_MOVE;
		else if (DROPEFFECT_LINK & dwOKEffects)
			*pdwEffect = DROPEFFECT_LINK;
		else
		{
			*pdwEffect = DROPEFFECT_NONE;
		}
	}
	else
	{
	   // Check if the drag source application allows the drop effect desired by user.
	   // The drag source specifies this in DoDragDrop
		if(!(*pdwEffect & dwOKEffects))
			*pdwEffect = DROPEFFECT_NONE;
	}

	return (DROPEFFECT_NONE == *pdwEffect) ? false : true;
}

void CNextMDropTarget::SetDropTargetWindow(wxWindow* pTargetWnd)
{
	m_pTargetWindow = pTargetWnd;
}
