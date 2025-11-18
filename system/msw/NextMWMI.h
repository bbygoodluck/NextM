#ifndef NEXTMWMI_H_INCLUDED
#define NEXTMWMI_H_INCLUDED

class CEventSink;
class CNextMWMI
{
public:
	CNextMWMI();
	~CNextMWMI();

public:
	void Initialize();
	void UnInitialize();

	IWbemServices* GetWbemService() { return m_pSvc; }
	IEnumWbemClassObject* Collect();
	IEnumWbemClassObject* Collect(unsigned long ulProcessId);

	bool IsInitialize() { return m_bInitialize; }

	void RegisterEventWindow(wxWindow* pWindow);
	void UnRegisterEventWindow();

private:
//	wxString BSTR2STR(BSTR source) {
//		if (source == nullptr) return "";
//		using convert_type = std::codecvt_utf8<wchar_t>;
//		std::wstring_convert<convert_type, wchar_t> converter;
//		converter.to_bytes(_bstr_t(source));
//		wxString strConv(converter);
//		return strConv;
//	}


private:
	IWbemLocator* m_pLoc = nullptr;
	IWbemServices* m_pSvc = nullptr;

	bool m_bInitialize = false;

	Proc::WMIProcQuerys Q{};

	CEventSink* m_pEventSink = nullptr;

};

#endif // NEXTMWMI_H_INCLUDED
