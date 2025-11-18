#include "../../ginc.h"
#include "../Sysdef.h"
#include "NextMWMI.h"
#include "EventSink.h"

CNextMWMI::CNextMWMI()
{

}

CNextMWMI::~CNextMWMI()
{

}

void CNextMWMI::Initialize()
{
	if(this->IsInitialize())
		return;

	HRESULT hr;

    // Initialize COM for this thread
    hr = CoInitializeEx(0, COINIT_APARTMENTTHREADED);//COINIT_MULTITHREADED);
    if (FAILED(hr))
		return;

    hr = CoInitializeSecurity(
        NULL,
        -1, // COM negotiates service
        NULL, // Authentication services
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT, // default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // default Impersonation
        NULL,
        EOAC_NONE,
        NULL
    );

    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&m_pLoc);
    if (FAILED(hr) || !m_pLoc)
    	return;

    // Connect to the root\cimv2 namespace with current user.
    hr = m_pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        0,//NULL,
        0,
        0,
        &m_pSvc);

	if (FAILED(hr) || !m_pSvc)
	{
		m_pLoc->Release();
        CoUninitialize();

        return;
	}

	hr = CoSetProxyBlanket(
        m_pSvc, // the proxy to set
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );

    if (FAILED(hr))
    {
		m_pSvc->Release();
        m_pLoc->Release();

        CoUninitialize();
        return;
    }

    m_bInitialize = true;
}

void CNextMWMI::RegisterEventWindow(wxWindow* pWindow)
{
	m_pEventSink = new CEventSink(pWindow);
	m_pEventSink->AddRef();

	HRESULT hr = S_OK;
	hr = m_pSvc->ExecNotificationQueryAsync(
        Q.WQL
        , Q.newProcess
        , WBEM_FLAG_SEND_STATUS
        , NULL
        , m_pEventSink);

	if (FAILED(hr))
	{
		m_pEventSink->Release();
		return;
	}

    hr = m_pSvc->ExecNotificationQueryAsync(
        Q.WQL
        , Q.delProcess
        , WBEM_FLAG_SEND_STATUS
        , NULL
        , m_pEventSink);
}

void CNextMWMI::UnRegisterEventWindow()
{
	if(m_pEventSink)
	{
		m_pSvc->CancelAsyncCall(m_pEventSink);
		m_pEventSink->Release();

		m_pEventSink = nullptr;
	}
}

void CNextMWMI::UnInitialize()
{
	if(m_pEventSink)
		UnRegisterEventWindow();

	m_pSvc->Release();
    m_pLoc->Release();

    CoUninitialize();
}

IEnumWbemClassObject* CNextMWMI::Collect()
{
	if(!m_bInitialize)
		return nullptr;

	HRESULT hr = S_OK;

	IEnumWbemClassObject* pEnumerator = nullptr;

	hr = m_pSvc->ExecQuery(
		Q.WQL
		, Q.SELECTPROC
		, WBEM_RETURN_WHEN_COMPLETE
		, NULL
		, &pEnumerator);

	if (FAILED(hr))
		return nullptr;

	return pEnumerator;
}

IEnumWbemClassObject* CNextMWMI::Collect(unsigned long ulProcessId)
{
	if(!m_bInitialize)
		return nullptr;

	HRESULT hr = S_OK;

	IEnumWbemClassObject* pEnumerator = nullptr;
#ifdef _UNICODE
	std::wstring sQ(Q.SELECTPROCPID, SysStringLen(Q.SELECTPROCPID));
#else
	std::string sQ(Q.SELECTPROCPID, SysStringLen(Q.SELECTPROCPID));
#endif // _UNICODE

	wxString strQuery = sQ;
	wxString sQuery  = wxString::Format(strQuery, ulProcessId);

	hr = m_pSvc->ExecQuery(
		Q.WQL
		, BSTR(sQuery.t_str())
		, WBEM_RETURN_WHEN_COMPLETE
		, NULL
		, &pEnumerator);

	if (FAILED(hr))
		return nullptr;

	return pEnumerator;
}
