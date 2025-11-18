#ifndef NETWORKINFO_H_INCLUDED
#define NETWORKINFO_H_INCLUDED

#include "SystemInterface.h"

class CNetwork : public CSystemInterface
{
public:
	CNetwork();
	~CNetwork();

public:
	void Init()   override;
	void Update() override;
	void Free()   override;

	unsigned long GetInBytes() { return dwcurrent_recv_; }
	unsigned long GetOutBytes() { return dwcurrent_sent_; }

	double GetDownSpeed() { return dblrecvspeed_; }
	double GetUpSpeed() { return dblsentspeed_; }

	wxString GetAdapterName() const { return strNetAdapterName_; }
	wxString GetDescription() const { return strNetDescription_; }
	wxString GetIP() const { return strNetIP_; }

	wxString GetBandWidth(unsigned long _ulbps, bool _bMaxDisp);

	auto GetNetworkInfo() -> Net::net_info;
	wxString GetInterfaceInfo(const int index);

	std::vector<wxString>& GetInterfaces() {
		return interfaces;
	}
	void SetSelectAdapter(const unsigned int indx) {
		select_index = indx;
	}

private:
	void InitNetworkInfo();


private:
	robin_hood::unordered_flat_map<wxString, Net::net_info> current_net;
	Net::net_info empty_net = {};

	std::vector<wxString> interfaces;
	std::vector<wxString> failed;

	wxString selected_iface{wxT("")};

	uint64_t timestamp = 0;

	wxString strNetAdapterName_ = wxT("");
	wxString strNetDescription_ = wxT("");
	wxString strNetIP_          = wxT("");
	bool bSetNetInfo_           = false;

	unsigned int select_index = 0;
#ifdef __WXMSW__
	IP_ADAPTER_ADDRESSES* m_pAdapters = nullptr;
	DWORD dwcurrent_sent_ = 0;
	DWORD dwcurrent_recv_ = 0;
	DWORD dwlast_sent_ = 0;
	DWORD dwlast_recv_ = 0;

	DWORD dw_refreshtime_ = 0;
	double dblrecvspeed_ = 0;
	double dblsentspeed_ = 0;
#else
#endif
private:
	virtual void OnTimer(wxTimerEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // NETWORKINFO_H_INCLUDED
