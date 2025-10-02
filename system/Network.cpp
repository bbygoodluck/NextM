#include "../ginc.h"
#include "Network.h"

wxBEGIN_EVENT_TABLE(CNetwork, CSystemInterface)
wxEND_EVENT_TABLE()

CNetwork::CNetwork()
{
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CNetwork::OnTimer));
	m_timer.SetOwner(this, wxTIMER_ID++);

	Init();
}

CNetwork::~CNetwork()
{
	Stop();
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(CNetwork::OnTimer));
}

void CNetwork::Init()
{
	Update();
}

void CNetwork::OnTimer(wxTimerEvent& event)
{
	Update();
}


void CNetwork::Update()
{
#ifdef __WXMSW__
	IP_ADAPTER_INFO  *pAdapterInfo;
	ULONG            ulOutBufLen;
	DWORD            dwRetVal;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
	{
		free (pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc ( ulOutBufLen );
		if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) != ERROR_SUCCESS)
			return;
	}

	if(!bSetNetInfo_)
	{
		bSetNetInfo_ = true;
		strNetAdapterName_ = pAdapterInfo->AdapterName;
		strNetDescription_ = pAdapterInfo->Description;
		strNetIP_ = pAdapterInfo->IpAddressList.IpAddress.String;
	}

	MIB_IFROW ifrow;
	wxZeroMemory(ifrow);

	ifrow.dwIndex = pAdapterInfo->Index;
	if(GetIfEntry(&ifrow) != NO_ERROR)
		return;

	dwcurrent_sent_ = ifrow.dwOutOctets - dwlast_sent_;
	dwcurrent_recv_ = ifrow.dwInOctets - dwlast_recv_;

	dwlast_sent_ = ifrow.dwOutOctets;
	dwlast_recv_ = ifrow.dwInOctets;

	unsigned long currentTickTime = GetTickCount();
	float diftime = currentTickTime - dw_refreshtime_;//(GetTickCount() - dw_refreshtime_);

	dblrecvspeed_ = dwcurrent_recv_ / diftime;
	dblsentspeed_ = dwcurrent_sent_ / diftime;

	dw_refreshtime_ = currentTickTime;//GetTickCount();

	if (pAdapterInfo)
        free(pAdapterInfo);
#else

#endif

}

wxString CNetwork::GetBandWidth(unsigned long _ulbps, bool _bMaxDisp)
{
	unsigned int iMega = 1000;
	unsigned int iGiga = 1000 * 1000;

	unsigned long ulM = _ulbps / iMega;
	unsigned long ulG = _ulbps / iGiga;

	wxString strRet(wxT(""));
	if(_ulbps >= iGiga)
	{
		strRet = _bMaxDisp ? wxString::Format(wxT("Max : %ldGbps") , ulG) :
							 wxString::Format(wxT("%ldGbps"), ulG);
	}
	else if(_ulbps >= iMega)
	{
		strRet = _bMaxDisp ? wxString::Format(wxT("Max : %ldMbps") , ulM) :
							 wxString::Format(wxT("%ldMbps"), ulM);
	}
	else
	{
		strRet = _bMaxDisp ? wxString::Format(wxT("Max : %ldKbps") , _ulbps) :
							 wxString::Format(wxT("%ldKbps"), _ulbps);
	}

	return strRet;
}

