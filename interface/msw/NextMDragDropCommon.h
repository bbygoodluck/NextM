#ifndef NEXTMDNDCOMMON_H_INCLUDED
#define NEXTMDNDCOMMON_H_INCLUDED

class CNextMDnDCommon
{
private:
	CNextMDnDCommon();

public:
	~CNextMDnDCommon();
	static CNextMDnDCommon* Get();

#ifdef __WXMSW__
	HRESULT CreateDataObject(FORMATETC* pFmtetc, STGMEDIUM* pStgMedium, int count, IDataObject** ppDataObject);
	HRESULT CreateEnumFormatEtc(UINT nNumFormats, FORMATETC* pFormatEtc, IEnumFORMATETC** ppEnumFormatEtc);
	HRESULT CreateDropSource(IDropSource **ppDropSource, wxWindow* pDragWindow);
#endif

	void ClearDatas() {
		m_lstDropItems.clear();
	}

	void AddDropItem(const wxString& strDropItem) {
		m_lstDropItems.push_back(strDropItem);
	}

	std::list<wxString> GetDropDatas() {  return m_lstDropItems; }

	void SetStartDrag(bool bStart = true) { m_bDragging = bStart; }
	bool IsDragging() { return m_bDragging; }

	void SetDropped(bool bDrop) { m_bDrop = bDrop; }
	bool IsDropped() { return m_bDrop; }

	wxWindow* GetDragWindow() { return m_pDragWindow; }
	wxWindow* GetDropWindow() { return m_pDropWindow; }
	DWORD GetDropOperation()  { return m_dwEffect; }
	bool IsDropDirectory()    { return m_bDropDir; }

#ifdef __WXMSW__
	void SetDropOperation(wxWindow* pDropWindow, DWORD dwEffect, bool bDropDir) {
		m_pDropWindow = pDropWindow;
		m_dwEffect = dwEffect;

		m_bDropDir = bDropDir;
	}
#else
#endif // __WXMSW__

	bool FindDropWindowItem(const wxPoint& pt, wxWindow* pDropWindow);
	void UpdateDropWindow(wxWindow* pDropWindow);
	void SendDragDropOperationEvent();

private:
	static std::unique_ptr<CNextMDnDCommon> m_pInstance;
	std::list<wxString> m_lstDropItems;

	bool m_bDropDir  = true;
	bool m_bDragging = false;
	bool m_bDrop     = false;

	wxWindow* m_pDropWindow = nullptr;
	wxWindow* m_pDragWindow = nullptr;

#ifdef __WXMSW__
	DWORD m_dwEffect = DROPEFFECT_NONE;
#else
#endif // __WXMSW__
};

#endif // NEXTMDRAGDROPCOMMON_H_INCLUDED
