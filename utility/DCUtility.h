#ifndef DCUTILITY_H_INCLUDED
#define DCUTILITY_H_INCLUDED

class CDCUtility final
{
private:
	explicit CDCUtility() {}

public:
	~CDCUtility() {}
	static CDCUtility* Get();

public:
	//화면 다시그리기
	void Refresh(wxWindow* pWindow, const wxRect& rect = wxRect(0, 0, 0, 0), bool bUpdate = false);
	wxString EllipseStr(wxDC* pDC, const wxString& strSrc, int destW);

private:
	static std::unique_ptr<CDCUtility> m_pInstance;
};

#endif // DCUTILITY_H_INCLUDED
