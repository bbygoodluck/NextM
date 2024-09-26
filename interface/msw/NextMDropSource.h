#ifndef NEXTMDROPSOURCE_H_INCLUDED
#define NEXTMDROPSOURCE_H_INCLUDED

#include <oleidl.h>
class CNextMDropSource : public IDropSource
{
	//
    // Constructor / Destructor
	//
public:
	explicit CNextMDropSource();
	~CNextMDropSource();
	//
    // IUnknown members
	//
    HRESULT STDMETHODCALLTYPE QueryInterface	(REFIID iid, void ** ppvObject) wxOVERRIDE;
    ULONG STDMETHODCALLTYPE AddRef			(void) wxOVERRIDE;
    ULONG STDMETHODCALLTYPE Release			(void) wxOVERRIDE;

    //
	// IDropSource members
	//
    HRESULT STDMETHODCALLTYPE QueryContinueDrag	(BOOL fEscapePressed, DWORD grfKeyState) wxOVERRIDE;
	HRESULT STDMETHODCALLTYPE GiveFeedback		(DWORD dwEffect) wxOVERRIDE;

private:
    //
	// private members and functions
	//
    LONG	   m_lRefCount;
};

#endif // NEXTMDROPSOURCE_H_INCLUDED
