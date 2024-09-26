#include "../../ginc.h"
#include "NextMDropTarget.h"

CNextMDropTarget::CNextMDropTarget(HWND hwnd)
	: m_hWnd(hwnd)
	, m_lRefCount(1)
{

}

CNextMDropTarget::~CNextMDropTarget()
{

}

// IUnknown implementation
HRESULT __stdcall CNextMDropTarget::QueryInterface (REFIID iid, void ** ppvObject)
{
	if(iid == IID_IDropTarget || iid == IID_IUnknown)
	{
		AddRef();
		*ppvObject = this;
		return S_OK;
	}
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}
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
HRESULT __stdcall CNextMDropTarget::DragEnter (IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	return S_OK;
}

HRESULT __stdcall CNextMDropTarget::DragOver (DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	return S_OK;
}

HRESULT __stdcall CNextMDropTarget::DragLeave (void)
{
	return S_OK;
}

HRESULT __stdcall CNextMDropTarget::Drop (IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
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
