#ifndef PATHVIEW_H_INCLUDED
#define PATHVIEW_H_INCLUDED

#include "../utility/MemoryDC.h"

class CPathView : public wxWindow
{
	class CPathItemData
	{
	public:
		explicit CPathItemData() {}
		virtual ~CPathItemData() {}

	public:
		wxString	m_strName;
		wxPoint		m_ptPolygon[6] = { {0, 0}, };
		wxRect		m_rcDisp;
	};

public:
	explicit CPathView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle = CHILD_WINDOW_STYLE);
	~CPathView();

	enum path_op {
		unknown = -1,
		add,
		updir,
		root,
		changedrive
	};

	void AddPath(const wxString& strPath);

private:
	void ClearPathItems();
	void Init();
	void Render(wxDC* dc);

	void CalcPathPolygon(wxDC* pDC);
	void DisplayPathItem(wxDC* pDC);

#ifdef __WXMSW__
	void AddDriveItem();
#endif

	void AddPathPolygon(wxDC* pDC, const wxString& strPath, int iPrevIndex);
	void UpdatePath(const wxString& strPath);

private:
	wxIcon m_icoDrive;
	std::vector<CPathItemData> m_vecPathDatas;

	wxBitmap m_bitmapNext;
	wxBitmap m_bitmapNextDisable;
	wxRect   m_bmpRect;
	wxRect   m_rcClick = wxRect(0, 0, 0, 0);

	wxToolTip* m_pToolTip = nullptr;

	wxString m_strCurrentDrive = wxT("");
	wxString m_strPath = wxT("");

	int m_iRED;
	int m_iGREEN;
	int m_iBLUE;

	int	m_iMouseOverIndex = -1;

	int m_iCurrentPathIndex = 0;
	int m_iDepthCount = 0;
	int	m_iPrevItemIndex = -1;
	int m_iTotalItemCount = 0;
	int m_iDispWidth = 0;

    const int m_iPolygonGap = 5;

#ifdef __WXMSW__
    int	 m_iDriveSelectedIndex = 0;
#endif

	bool m_bNextPath = false;
	bool m_bMouseOver = false;
	bool m_bNextPathMouseOver = false;
	bool m_bChangedPath = false;
	bool m_bFounded = false;

	std::unique_ptr<wxTextCtrl> m_pTxtCtrl = nullptr;

	//화면 사이즈
    wxRect m_viewRect = wxRect(0, 0, 0, 0);

	CMemoryDC m_memDC;
	wxFont m_PathViewFont = wxNullFont;

	std::unordered_map<int, wxString> m_subdirMap;
private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);

	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLButtonClick(wxMouseEvent& event);
	void OnMouseRButtonClick(wxMouseEvent& event);
	void OnMouseDBClick(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);

#ifdef __WXMSW__
	void OnDriveMenuClick(wxCommandEvent& event);
	void OnDrvieMenuUpdate(wxUpdateUIEvent& event);
#endif
	void OnMoveSubDir(wxCommandEvent& event);

	//디렉토리 이동이벤트
	void OnEnterTextCtrl(wxCommandEvent& event);
	void OnKeyDownTextCtrl(wxKeyEvent& event);
	void OnKillFocusTxtCtrl(wxFocusEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif // PATHVIEW_H_INCLUDED
