#ifndef NETWORKINFO_H_INCLUDED
#define NETWORKINFO_H_INCLUDED

#include "SystemInterface.h"

class CNetwork : public CSystemInterface
{
public:
	CNetwork();
	~CNetwork();

public:
	virtual void Init() override;
	virtual void Update() override;

	unsigned long GetInBytes() { return dwcurrent_recv_; }
	unsigned long GetOutBytes() { return dwcurrent_sent_; }

	double GetDownSpeed() { return dblrecvspeed_; }
	double GetUpSpeed() { return dblsentspeed_; }

	wxString GetAdapterName() const { return strNetAdapterName_; }
	wxString GetDescription() const { return strNetDescription_; }
	wxString GetIP() const { return strNetIP_; }

	wxString GetBandWidth(unsigned long _ulbps, bool _bMaxDisp);
private:


private:
	wxString strNetAdapterName_ = wxT("");
	wxString strNetDescription_ = wxT("");
	wxString strNetIP_          = wxT("");
	bool bSetNetInfo_           = false;

#ifdef __WXMSW__
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
