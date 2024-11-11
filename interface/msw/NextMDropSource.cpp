#include "../../ginc.h"
#include "NextMDropSource.h"
#include "NextMDataObject.h"

CNextMDropSource::CNextMDropSource()
	: m_lRefCount(0)
{
	m_pDragSourceNotify = new CDragSourceNotify();
	m_pDragSourceNotify->AddRef();
}

CNextMDropSource::~CNextMDropSource()
{
	if (m_pDragSourceNotify)
	{
		m_pDragSourceNotify->Release();
		m_pDragSourceNotify = nullptr;
	}
}

//
// IUnknown members
//
HRESULT STDMETHODCALLTYPE CNextMDropSource::QueryInterface(REFIID iid, void ** ppvObject)
{
   if(IsEqualIID(iid, IID_IDropSource) || IsEqualIID(iid, IID_IUnknown))
        *ppvObject = this;
	else if(IsEqualIID(iid, IID_IDropSourceNotify) && (m_pDragSourceNotify != nullptr))
		return m_pDragSourceNotify->QueryInterface(iid, ppvObject);
    else
    {
        *ppvObject = 0;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE CNextMDropSource::AddRef(void)
{
	// increment object reference count
    return InterlockedIncrement(&m_lRefCount);
}

ULONG STDMETHODCALLTYPE CNextMDropSource::Release(void)
{
	// decrement object reference count
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

//
// IDropSource members
//
//	Called by OLE whenever Escape/Control/Shift/Mouse buttons have changed
//
HRESULT STDMETHODCALLTYPE CNextMDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
	//ESC키를 누르게 되면 Drop을 취소함
	if(fEscapePressed == TRUE)
		return DRAGDROP_S_CANCEL;

	//마우스 왼쪽/오른쪽 버튼이 Release 되었을때 Drop 시작
	if(!(grfKeyState & (MK_LBUTTON | MK_RBUTTON)))
		return DRAGDROP_S_DROP;

	//그외에는 아무것도 안함
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CNextMDropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

void CNextMDropSource::SetDataObject(IDataObject* pDataObject)
{
	m_pDataObject = pDataObject;
}
