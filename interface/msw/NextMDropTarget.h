#ifndef NEXTMDROPTARGET_H_INCLUDED
#define NEXTMDROPTARGET_H_INCLUDED

#include <oleidl.h>
class CNextMDropTarget : public IDropTarget
{
	bool m_bAllowDrop;
	IDataObject*       m_pIDataObject;

	IDropTargetHelper *m_pDropTargetHelper;

public:
	// Constructor
	explicit CNextMDropTarget(HWND hwnd);
	~CNextMDropTarget();

	// IUnknown implementation
	virtual HRESULT STDMETHODCALLTYPE QueryInterface (REFIID iid, void ** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef (void);
	virtual ULONG STDMETHODCALLTYPE Release (void);

	// IDropTarget implementation
	virtual HRESULT STDMETHODCALLTYPE DragEnter (IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	virtual HRESULT STDMETHODCALLTYPE DragOver (DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	virtual HRESULT STDMETHODCALLTYPE DragLeave (void);
	virtual HRESULT STDMETHODCALLTYPE Drop (IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);

	bool QueryDrop(DWORD grfKeyState, LPDWORD pdwEffect);

	void      SetDropTargetWindow(wxWindow* pTargetWnd);
	wxWindow* GetDropTargetWindow() { return m_pTargetWindow; }
	HWND      GetDropTargetWndHandle() { return m_pTargetWindow->GetHWND(); }

private:
	LONG	m_lRefCount;
	HWND	m_hWnd;
	wxWindow* m_pTargetWindow = nullptr;

	bool    m_bDropDir = true;
};

#endif // NEXTMDROPTARGET_H_INCLUDED
