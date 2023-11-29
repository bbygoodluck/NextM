#ifndef VIEWDC_H_INCLUDED
#define VIEWDC_H_INCLUDED

class CMemoryDC
{
public:
    CMemoryDC();
    ~CMemoryDC();

public:
    wxMemoryDC* CreateMemoryDC(wxDC* pDC, const wxRect& rc, const wxColour& colPen = wxColour(0, 0, 0), const wxColour colBrush = wxColour(0, 0, 0));
    wxMemoryDC* CreateRoundRectMemoryDC(wxDC* pDC, const wxRect& rc, double dblradius, const wxColour& colPen = wxColour(0, 0, 0), const wxColour colBrush = wxColour(0, 0, 0));
    void ClearMemoryDC(wxMemoryDC* pMemDC);
	void ClearMemoryDC();

	void ChangeViewSize(const wxSize& sz);

    bool IsCreatedMemDC();
    unsigned int GetWidth();
    unsigned int GetHeight();

protected:
    void DetachDC();

private:
	void CreateMemoryBuffer(wxDC* pDC);

protected:
    // 화면 버퍼(더블버퍼링 이용)
	wxBitmap* m_pDoubleBuffer = nullptr;
	std::unique_ptr<wxMemoryDC> m_pMemDC = nullptr;

    //변경사이즈(OnSize Event)
	wxSize m_szChagned = wxSize(0, 0);
	wxRect m_rc = wxRect(0, 0, 0, 0);

private:
};

#endif // VIEWDC_H_INCLUDED
