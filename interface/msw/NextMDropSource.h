#ifndef NEXTMDROPSOURCE_H_INCLUDED
#define NEXTMDROPSOURCE_H_INCLUDED

#include <oleidl.h>

class CDragSourceNotify : public IDropSourceNotify
{
private:
	LONG refCount;

public:

	CDragSourceNotify(void)
	{
		refCount = 0;
	}

	~CDragSourceNotify(void)
	{
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject)
	{
		if(!ppvObject)
		{
			return E_POINTER;
		}

		if (IsEqualIID(riid, IID_IUnknown))
		{
			*ppvObject = static_cast<IUnknown*>(this);
		}
		else if (IsEqualIID(riid, IID_IDropSourceNotify))
		{
			*ppvObject = static_cast<IDropSourceNotify*>(this);
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef(void)
	{
		return InterlockedIncrement(&refCount);
	}

	ULONG STDMETHODCALLTYPE Release(void)
	{
		ULONG ret = InterlockedDecrement(&refCount);
		if(!ret) {
			delete this;
		}
		return ret;
	}

	HRESULT STDMETHODCALLTYPE DragEnterTarget(HWND /*hWndTarget*/)
	{
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DragLeaveTarget(void)
	{
		return S_OK;
	}
};

class CNextMDropSource : public IDropSource
{
	CDragSourceNotify* m_pDragSourceNotify = nullptr;

	//
    // Constructor / Destructor
	//
public:
	explicit CNextMDropSource();
	~CNextMDropSource();
	//
    // IUnknown members
	//
    HRESULT STDMETHODCALLTYPE QueryInterface	(REFIID iid, void ** ppvObject) wxOVERRIDE;
    ULONG STDMETHODCALLTYPE AddRef			(void) wxOVERRIDE;
    ULONG STDMETHODCALLTYPE Release			(void) wxOVERRIDE;

    //
	// IDropSource members
	//
    HRESULT STDMETHODCALLTYPE QueryContinueDrag	(BOOL fEscapePressed, DWORD grfKeyState) wxOVERRIDE;
	HRESULT STDMETHODCALLTYPE GiveFeedback		(DWORD dwEffect) wxOVERRIDE;

	void SetDataObject(IDataObject* pDataObject);
private:
    //
	// private members and functions
	//
    LONG	   m_lRefCount;
    IDataObject* m_pDataObject = nullptr;
};

#endif // NEXTMDROPSOURCE_H_INCLUDED
