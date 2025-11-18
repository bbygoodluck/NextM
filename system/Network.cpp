#include "../ginc.h"
#include "Sysdef.h"
#include "Network.h"

#include <locale>
//#include <ws2tcpip.h>
//#include <Ipifcons.h>
#include <limits.h>

template <typename T, typename T2>
bool v_contains(const std::vector<T>& vec, const T2& find_val)
{
	return std::ranges::find(vec, find_val) != vec.end();
}

wxBEGIN_EVENT_TABLE(CNetwork, CSystemInterface)
wxEND_EVENT_TABLE()

CNetwork::CNetwork()
{
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CNetwork::OnTimer));
	m_timer.SetOwner(this, wxTIMER_ID++);
}

CNetwork::~CNetwork()
{
	Stop();
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(CNetwork::OnTimer));

	Free();
}

void CNetwork::Free()
{
#ifdef __WXMSW__
	if(m_pAdapters)
		std::free(m_pAdapters);

	m_pAdapters = nullptr;
#else

#endif
	current_net.clear();
	interfaces.clear();
	failed.clear();
}

void CNetwork::Init()
{
	//Update();
	InitNetworkInfo();
}

void CNetwork::OnTimer(wxTimerEvent& event)
{
//	Update();
	SendEvent();
}

void CNetwork::InitNetworkInfo()
{
	Free();

	timestamp = 0;
	auto& net = current_net;
//	auto new_timestamp = theUtility->time_ms();

	//! Much of the following code is based on the implementation used in btop4win
	//! See: https://github.com/aristocratos/btop4win

	ULONG bufSize = 0;
	if (GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, nullptr, &bufSize) != ERROR_BUFFER_OVERFLOW) {
		throw std::runtime_error("Net::collect() -> GetAdaptersAddresses() failed to get buffer size!");
	}

	//네트워크 어뎁터는 초기한번만 리스트로 가져오도록 변경
//	auto adapters = std::unique_ptr<IP_ADAPTER_ADDRESSES, decltype(std::free)*>{reinterpret_cast<IP_ADAPTER_ADDRESSES*>(std::malloc(bufSize)), std::free};
	m_pAdapters = (IP_ADAPTER_ADDRESSES *)std::malloc(bufSize);
	if (GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, m_pAdapters, &bufSize)) {
		throw std::runtime_error("Net::collect() -> GetAdaptersAddresses() failed to get adapter list!");
	}

//	if (GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, adapters.get(), &bufSize)) {
//		throw std::runtime_error("Net::collect() -> GetAdaptersAddresses() failed to get adapter list!");
//	}

	interfaces.clear();

//	for (auto a = adapters.get(); a != nullptr; a = a->Next)
	for (auto a = m_pAdapters; a != nullptr; a = a->Next)
	{
		wxString strIface(a->FriendlyName);
		wxString description(a->Description);

		interfaces.push_back(strIface);

		net[strIface].description = description;
		net[strIface].connected = (a->OperStatus == IfOperStatusUp);

		//? Get IP adresses associated with adapter

		bool ip4 = false, ip6 = false;
		for (auto u = a->FirstUnicastAddress; u != nullptr and not ip4; u = u->Next)
		{
			auto family = u->Address.lpSockaddr->sa_family;
			if (family == AF_INET and not ip4)
			{
				auto sa_in = reinterpret_cast<sockaddr_in *>(u->Address.lpSockaddr);
				std::array<char, 256> ipAddress;
				if (inet_ntop(AF_INET, &sa_in->sin_addr, ipAddress.data(), 256) == NULL)
					continue;

				net[strIface].ipv4 = wxString(ipAddress.data());
				ip4 = not net[strIface].ipv4.empty();
			}
			else if (family == AF_INET6 and not ip6)
			{
				auto sa_in = reinterpret_cast<sockaddr_in6 *>(u->Address.lpSockaddr);
				std::array<char, 256> ipAddress;

				if (inet_ntop(AF_INET6, &sa_in->sin6_addr, ipAddress.data(), 256) == NULL)
					continue;

				net[strIface].ipv6 = wxString(ipAddress.data());
				ip6 = not net[strIface].ipv6.empty();
			}
			else
				continue;
		}
	}

//	timestamp = new_timestamp;
}

wxString CNetwork::GetInterfaceInfo(const int index)
{
	const int count_of_interfaces = interfaces.size();

	if(index > count_of_interfaces && index > 0)
		return wxT("");

	wxString strKey = interfaces.at(index);

//	wxString strInterfaceInfo = current_net[strKey].description;
//	strInterfaceInfo += wxT("(");
//	strInterfaceInfo += current_net[strKey].ipv4;
//	strInterfaceInfo += wxT(")");

	return (current_net[strKey].description + wxT("(") + current_net[strKey].ipv4 + wxT(")"));
}

auto CNetwork::GetNetworkInfo() -> Net::net_info
{
	auto& net = current_net;
	failed.clear();

	auto new_timestamp = theUtility->time_ms();

	if(!m_pAdapters)
		return empty_net;

	for (auto a = m_pAdapters; a != nullptr; a = a->Next)
	{
		wxString strIface(a->FriendlyName);

		MIB_IF_ROW2 ifEntry;
		SecureZeroMemory((PVOID)&ifEntry, sizeof(MIB_IF_ROW2));
		ifEntry.InterfaceIndex = a->IfIndex;
		if (GetIfEntry2(&ifEntry) != NO_ERROR)
		{
			if (not v_contains(failed, strIface))
				failed.push_back(strIface);

			continue;
		}

		for (const wxString dir : {wxT("download"), wxT("upload")})
		{
			auto& saved_stat = net.at(strIface).stat.at(dir);
		//	auto& bandwidth = net.at(strIface).bandwidth.at(dir);

			uint64_t val = (dir.CmpNoCase(wxT("download")) == 0 ? ifEntry.InOctets : ifEntry.OutOctets);

			//? Update speed, total and top values
			if (val < saved_stat.last)
			{
				saved_stat.rollover += saved_stat.last;
				saved_stat.last = 0;
			}

			if (std::cmp_greater((unsigned long long)saved_stat.rollover + (unsigned long long)val, std::numeric_limits<uint64_t>::max()))
			{
				saved_stat.rollover = 0;
				saved_stat.last = 0;
			}

			saved_stat.speed = round((double)(val - saved_stat.last) / ((double)(new_timestamp - timestamp) / 1000));
			if (saved_stat.speed > saved_stat.top) saved_stat.top = saved_stat.speed;
			if (saved_stat.offset > val + saved_stat.rollover) saved_stat.offset = 0;

			saved_stat.total = (val + saved_stat.rollover) - saved_stat.offset;
			saved_stat.last = val;

			//? Add values to graph
		//	bandwidth.push_back(saved_stat.speed);
		//	while (std::cmp_greater(bandwidth.size(), width * 2)) bandwidth.pop_front();
		}
	}

	timestamp = new_timestamp;

	wxString strSelectIndex = interfaces[select_index];
	return net.at(strSelectIndex);
}

void CNetwork::Update()
{
#ifdef __WXMSW__
#else
#endif // __WXMSW__
//#ifdef __WXMSW__
//	IP_ADAPTER_INFO  *pAdapterInfo;
//	ULONG            ulOutBufLen;
//	DWORD            dwRetVal;
//
//	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
//	ulOutBufLen = sizeof(IP_ADAPTER_INFO);
//
//	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
//	{
//		free (pAdapterInfo);
//		pAdapterInfo = (IP_ADAPTER_INFO *)malloc ( ulOutBufLen );
//		if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) != ERROR_SUCCESS)
//			return;
//	}
//
//	if(!bSetNetInfo_)
//	{
//		bSetNetInfo_ = true;
//		strNetAdapterName_ = pAdapterInfo->AdapterName;
//		strNetDescription_ = pAdapterInfo->Description;
//		strNetIP_ = pAdapterInfo->IpAddressList.IpAddress.String;
//	}
//
//	MIB_IFROW ifrow;
//	wxZeroMemory(ifrow);
//
//	ifrow.dwIndex = pAdapterInfo->Index;
//	if(GetIfEntry(&ifrow) != NO_ERROR)
//		return;
//
//	dwcurrent_sent_ = ifrow.dwOutOctets - dwlast_sent_;
//	dwcurrent_recv_ = ifrow.dwInOctets - dwlast_recv_;
//
//	dwlast_sent_ = ifrow.dwOutOctets;
//	dwlast_recv_ = ifrow.dwInOctets;
//
//	unsigned long currentTickTime = GetTickCount();
//	float diftime = (float)(currentTickTime - dw_refreshtime_) / 1000;//(GetTickCount() - dw_refreshtime_);
//
//	dblrecvspeed_ = std::round(dwcurrent_recv_ / diftime);
//	dblsentspeed_ = std::round(dwcurrent_sent_ / diftime);
//
//	dw_refreshtime_ = currentTickTime;//GetTickCount();
//
//	if (pAdapterInfo)
//        free(pAdapterInfo);
//#else
//
//#endif

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

