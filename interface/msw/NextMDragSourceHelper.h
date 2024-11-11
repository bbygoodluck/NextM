#ifndef NEXTMDRAGSOURCEHELPER_H_INCLUDED
#define NEXTMDRAGSOURCEHELPER_H_INCLUDED

class CNextMDragSourceHelper
{
	IDragSourceHelper* m_pDragSourceHelper = nullptr;

	void Init();
	void Release();

public:
	explicit CNextMDragSourceHelper();
	~CNextMDragSourceHelper();

	IDragSourceHelper* GetDragSourceHelper() { return m_pDragSourceHelper; }
public:
	HRESULT InitializeFromBitmap(const wxString& strFileName, IDataObject* pDataObject);
	HRESULT InitializeFromWindow(HWND hwnd, const wxPoint& pt,IDataObject* pDataObject);
};

#endif // NEXTMDRAGSOURCEHELPER_H_INCLUDED
