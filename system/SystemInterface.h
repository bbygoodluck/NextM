#ifndef SYSTEMCOMMON_H_INCLUDED
#define SYSTEMCOMMON_H_INCLUDED

#ifdef __WXMSW__
	#include <Pdh.h>
	#include <PdhMsg.h>
	#include <Iphlpapi.h>
#endif

class CSystemInterface : public wxEvtHandler
{
public:
	CSystemInterface() {}
	~CSystemInterface() {}

public:
	virtual void Init()   = 0;
	virtual void Update() = 0;

	void Start(int milliseconds) {
		m_bStarted = true;
		m_timer.Start(milliseconds);
	}

	void Stop() {
		m_bStarted = false;
		m_timer.Stop();
	}

	void AddEventListener(wxWindow* pWindow) {
		m_evtListeners.emplace_back(pWindow);
	}

	virtual void Clear() {
		m_evtListeners.clear();

		std::vector<wxWindow *>().swap(m_evtListeners);
		m_evtListeners.reserve(0);
	}

protected:
	void SendEvent() {
		std::vector<wxWindow *>::const_iterator cIter;
		for(cIter = m_evtListeners.begin(); cIter != m_evtListeners.end(); ++cIter)
		{
			wxWindow* pWEvtHandler = *cIter;
			wxCommandEvent evt(wxEVT_RESOURCE_MONITORING);

			if(!m_bStarted)
				return;

			if(pWEvtHandler)
				wxQueueEvent(pWEvtHandler, evt.Clone());
//				wxPostEvent(pWEvtHandler, evt);
		}
	}

protected:
	wxTimer m_timer;
	bool m_bStarted = false;

	std::vector<wxWindow *> m_evtListeners;

protected:
	virtual void OnTimer(wxTimerEvent& event) = 0;
};

#endif // SYSTEMCOMMON_H_INCLUDED
