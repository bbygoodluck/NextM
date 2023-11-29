#ifndef NEXTMNCDVIEW_H_INCLUDED
#define NEXTMNCDVIEW_H_INCLUDED

#include "../utility/MemoryDC.h"
#include "../interface/NextMNCD.h"
#include "NextMTooltipView.h"

class DlgDirManager;
class CNextMNCDView : public wxWindow
{
	const int GAP_POINT = 3;
	const int ICON_WIDTH  = 16;
	const int ICON_HEIGHT = 16;

	const unsigned int DISP_COLS_DEPTH = 13;
	unsigned int DISP_ROW_HEIGHT = 20;

public:
	CNextMNCDView(wxWindow* parent, const int nID, const wxSize& sz, const wxString& strTitle);
	virtual ~CNextMNCDView();

public:
	void LoadDirInfo();
	void AddDir();
	void RenameDir();
	void DeleteDir();
	void NextFind();

	DirNode* GetCurNode() {
		return (*m_itIndex);
	}
protected:
	//화면 표시
	void Render(wxDC* pDC);
	//화면 사이즈 계산
	void CalcRowInView(wxDC* pDC);
	//데이터 표시
	void DisplayNCD(wxDC* pDC);
	//표시 문자의 ...표시 계산
	wxString CalcEllipse(wxDC* pDC, const wxString& strSrc, const int iDispWidth);

	//키 처리
	void ProcessKeyEvent(const int iKeyCode);

	//←
	bool MoveLeft();

	//→
	bool MoveRight();

	//↑
	bool MoveUp();

	//↓
	bool MoveDown();

	//화면 이동시 시작위치 설정
	void ResetBaseIteratorDepth();
	void ResetBaseIteratorRow();

	//라인 그리기
	void DrawLine(wxDC* pDC, std::vector<DirNode *>::iterator itStart);

	//부모 패스 표시
	void DisplayParentPath(wxDC* pDC, DirNode* pSelNode, bool IsStart = false);

	//항목 찾기
	void FindMatch(const wxString& strKey);

	//스크롤처리
	void ScrollVerticalProcess(wxEventType evtType, bool bWheel, int iThumbTrackPos = 0);
	void ScrollHorzProcess(wxEventType evtType, int iThumbTrackPos = 0);

	//Rename
	void RenameStart(const wxString& strName);

	//find mouse click point
	bool FindMouseClickItem(const wxPoint& pt);

	//디렉토리 추가
	void AddDirectory();

	//디렉토리 삭제
	void DeleteDirectory();

private:
	wxRect m_viewRect;
	wxRect m_viewDispRect;

	wxIcon m_icoDrive;
	wxIcon m_icoDir;

	CMemoryDC m_memDC;

	wxColour m_colStartIndex = wxColour(75, 75, 152);
	wxColour m_colParent     = wxColour(255, 51, 51);

	wxColour m_crBackGround;
	wxColour m_crDefaultText;
	wxColour m_crSelectedText;
	wxColour m_crSelectedBackground;
	wxColour m_crSelectedParentBackground;
	wxColour m_crLine;

	bool m_bSizeChanged;

	unsigned int m_iDispColsDepth = DISP_COLS_DEPTH;
	unsigned int m_iDispColsWidth = 0;

	int m_iRowInView = 0;
	int m_iCurRow = 0;
	int m_iCurDepth = 0;
	//스크롤바 클릭시
	int m_iThumbTrackPos = 0;
	unsigned int m_iNCDSize = 0;
	unsigned int m_iStartIndex = 0;
	unsigned int m_iEndWidthItem = 0;

	std::vector<DirNode *>::iterator m_itStartNode;
	std::vector<DirNode *>::iterator m_itEndDispNode;
	std::vector<DirNode *>::iterator m_itIndex;

	DlgDirManager* m_pParent;

#ifdef __WXMSW__
	wxString m_strVolume = wxT("");
#endif
	//키 입력
	std::unique_ptr<CNextMTooltipView> m_pKeyInputToolTip = nullptr;
	wxString m_strKeyInput = wxT("");

	wxTextCtrl* m_pTxtCtrlForRename;

private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnScroll(wxScrollWinEvent& event);

	//Rename
	void OnTextCtrlEnter(wxCommandEvent& event);
	void OnCharHookFromTextCtrl(wxKeyEvent& event);

	//마우스 이벤트
	//LButton Click
	void OnMouseLBClick(wxMouseEvent& event);
	//LButton Double click
	void OnMouseLBDBClick(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMNCDVIEW_H_INCLUDED
