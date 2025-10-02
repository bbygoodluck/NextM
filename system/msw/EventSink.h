#ifndef EVENTSINK_H_INCLUDED
#define EVENTSINK_H_INCLUDED

//#define _WIN32_DCOM
//#include <comdef.h>
//#include <wbemidl.h>

class CEventSink : public IWbemObjectSink
{
	LONG m_lRef;
	bool bDone;
	CRITICAL_SECTION threadlock;

public:
	CEventSink(wxWindow* pWindow);
	~CEventSink();

	virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

    virtual HRESULT STDMETHODCALLTYPE Indicate(
		/* [in] */
		LONG lObjectCount,
		/* [size_is][in] */
		IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray
	);

    virtual HRESULT STDMETHODCALLTYPE SetStatus(
		/* [in] */ LONG lFlags,
		/* [in] */ HRESULT hResult,
		/* [in] */ BSTR strParam,
		/* [in] */ IWbemClassObject __RPC_FAR *pObjParam
	);

	bool IsDone();

private:
	wxWindow* m_pWindow;
};

#endif // EVENTSINK_H_INCLUDED
