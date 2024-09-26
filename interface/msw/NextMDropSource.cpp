#include "../../ginc.h"
#include "NextMDropSource.h"

CNextMDropSource::CNextMDropSource()
	: m_lRefCount(1)
{

}

CNextMDropSource::~CNextMDropSource()
{

}

//
// IUnknown members
//
HRESULT STDMETHODCALLTYPE CNextMDropSource::QueryInterface(REFIID iid, void ** ppvObject)
{
	// check to see what interface has been requested
    if(iid == IID_IDropSource || iid == IID_IUnknown)
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
	if(grfKeyState & (MK_LBUTTON | MK_RBUTTON))
		return DRAGDROP_S_DROP;

	//그외에는 아무것도 안함
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CNextMDropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}
