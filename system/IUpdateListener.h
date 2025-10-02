#ifndef IUPDATELISTENER_H_INCLUDED
#define IUPDATELISTENER_H_INCLUDED

class IUpdateListener
{
public:
	virtual void UpdateListener(wxCommandEvent& event) = 0;
};

#endif // IUPDATELISTENER_H_INCLUDED
