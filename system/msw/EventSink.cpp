#include "../../ginc.h"
#include "../Sysdef.h"
#include "../../views/NextMProcessView.h"
#include "EventSink.h"

static wxString CurrentTimeString()
{
    wxDateTime now = wxDateTime::Now();
    return now.FormatISOCombined(' ');
}

CEventSink::CEventSink(wxWindow* pWindow)
	: m_pWindow(pWindow)
{
	m_lRef = 0;
	bDone = false;
	InitializeCriticalSection(&threadlock);
}
CEventSink::~CEventSink()
{
	bDone = true;
	DeleteCriticalSection(&threadlock);
}

ULONG STDMETHODCALLTYPE CEventSink::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}

ULONG STDMETHODCALLTYPE CEventSink::Release()
{
	LONG lRef = InterlockedDecrement(&m_lRef);
    if(lRef == 0)
        delete this;

	return lRef;
}

HRESULT STDMETHODCALLTYPE CEventSink::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
    {
        *ppv = (IWbemObjectSink *) this;
		AddRef();

        return WBEM_S_NO_ERROR;
    }
    else
		return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE CEventSink::Indicate(/* [in] */LONG lObjectCount, /* [size_is][in] */IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray)
{
	for (LONG i = 0; i < lObjectCount; ++i)
	{
		VARIANT var;
		VariantInit(&var);

		HRESULT hr = apObjArray[i]->Get(L"TargetInstance", 0, &var, nullptr, nullptr);
		if (SUCCEEDED(hr) && var.vt == VT_UNKNOWN)
		{
			int newOrDel = -1;
			IWbemClassObject* pInstance = nullptr;
			var.punkVal->QueryInterface(IID_IWbemClassObject, (void**)&pInstance);
			if (pInstance)
			{
				// Get Name
				VARIANT vName; VariantInit(&vName);
				VARIANT vPid; VariantInit(&vPid);
				VARIANT vCmd; VariantInit(&vCmd);

				pInstance->Get(L"Name", 0, &vName, nullptr, nullptr);
				pInstance->Get(L"ProcessId", 0, &vPid, nullptr, nullptr);
				pInstance->Get(L"CommandLine", 0, &vCmd, nullptr, nullptr);

				wxString name = (vName.vt == VT_BSTR && vName.bstrVal) ? wxString(vName.bstrVal) : wxString("N/A");
				long pid = (vPid.vt == VT_I4) ? vPid.lVal : -1;
				wxString cmd = (vCmd.vt == VT_BSTR && vCmd.bstrVal) ? wxString(vCmd.bstrVal) : wxString("");

				// Determine event type by reading __CLASS or __RELPATH? Simpler: try to get "__CLASS" from the wrapper object
				VARIANT vClass; VariantInit(&vClass);
				apObjArray[i]->Get(L"__CLASS", 0, &vClass, nullptr, nullptr);
				wxString eventType = wxString::FromUTF8("Event");
				if (vClass.vt == VT_BSTR && vClass.bstrVal)
				{
					std::wstring cls(vClass.bstrVal, SysStringLen(vClass.bstrVal));
					if (cls == L"__InstanceCreationEvent")
					{
						eventType = "Created";
						newOrDel = NEW_PROCESS_ADDED;
					}
					else if (cls == L"__InstanceDeletionEvent")
					{
						eventType = "Deleted";
						newOrDel = DEL_PROCESS;
					}
					else
						eventType = wxString(cls);


					if(newOrDel != -1)
					{
						wxString time = CurrentTimeString();
						wxTheApp->CallAfter([this, time, eventType, name, pid, cmd]()
						{
							if (m_pWindow)
								((CNextMProcessView *)m_pWindow)->AddAndDelProcessEvent(eventType, name, pid, cmd);
						});
					}
				}

				VariantClear(&vName);
				VariantClear(&vPid);
				VariantClear(&vCmd);

				pInstance->Release();
			}
		}

		VariantClear(&var);
	}

	return WBEM_S_NO_ERROR;
}

HRESULT STDMETHODCALLTYPE CEventSink::SetStatus(
	/* [in] */ LONG lFlags,
	/* [in] */ HRESULT hResult,
	/* [in] */ BSTR strParam,
	/* [in] */ IWbemClassObject __RPC_FAR *pObjParam)
{
	if(lFlags == WBEM_STATUS_COMPLETE)
	{
		EnterCriticalSection(&threadlock);
		bDone = true;
		LeaveCriticalSection(&threadlock);
	}
	else if(lFlags == WBEM_STATUS_PROGRESS)
	{

	}

	return WBEM_S_NO_ERROR;
}

bool CEventSink::IsDone()
{
	bool IsDone = true;
	EnterCriticalSection(&threadlock);
	IsDone = bDone;
	LeaveCriticalSection(&threadlock);

	return IsDone;
}
