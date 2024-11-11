#ifndef FILELISTVIEW_H_INCLUDED
#define FILELISTVIEW_H_INCLUDED

#include "../interface/NextMDirData.h"

#include "../utility/MemoryDC.h"
#include "../utility/Sorting.h"
#include "../utility/ImageList.h"

#include "../interface/PositionInfo.h"
#include "../engine/LocalFileSystem.h"

#include "NextMViewManager.h"
#include "NextMTooltipView.h"

#ifdef __WXMSW__
	#include "../interface/msw/NextMDropTarget.h"
#else
#endif
struct _HistoryItem
{
	int iCurrentIndex;
	int iStartIndex;
};

WX_DECLARE_STRING_HASH_MAP(_HistoryItem, _HISTORY);

typedef struct stFileImageInfo {
	bool bPreset = false;
	int iIconIndex = 0;
	int iOverlayIconIndex = 0;
	wxString strFullPath = wxT("");
} FILE_IMAGE_INFO, *PFILE_IMAGE_INFO;

class CFileListView : public wxWindow, public wxThreadHelper
{
protected:
	enum VIEW_SORT {
		VIEW_SORT_DEFAULT = 0,
		VIEW_SORT_TIME,
		VIEW_SORT_SIZE,
	};

	//컬럼보기 목록
	const COLUMN_VIEW_OPTION _gColumnViewOPEnum[10] = {
		VIEW_COLUMN_AUTO,
		VIEW_COLUMN_ONE,
		VIEW_COLUMN_TWO,
		VIEW_COLUMN_THREE,
		VIEW_COLUMN_FOUR,
		VIEW_COLUMN_FIVE,
		VIEW_COLUMN_SIX,
		VIEW_COLUMN_SEVEN,
		VIEW_COLUMN_EIGHT,
		VIEW_COLUMN_NINE
	};


	class CColumnPoint
	{
	public:
		wxPoint pt1;
		wxPoint pt2;
	};

public:
    CFileListView(wxWindow* parent, const int nID, const wxSize& sz);
	virtual ~CFileListView();

	typedef struct _SELITEM_INFO {
		int m_iSelIndex;
		wxString m_strFullPath;
		bool m_bFile;
	} SELITEM_INFO, *PSELITEM_INFO;

	virtual void LoadDirectory(const wxString& strPath) = 0;
	void SelectedItemsClear(bool bDeleted = false);

	//아이템 전체 선택 또는 해제
	void SelectAllItemOrRelease(bool bAllSelect);

	wxString GetCurrentPath() const { return m_strCurrentPath; }
	wxString GetCurrentItem() {
		std::vector<CNextMDirData>::iterator iter = m_itemList.begin() + m_iCurrentItemIndex;
		return iter->GetFullPath();
	}

	void GetSelectedItems(std::list<wxString>& lstSrc, bool IsCut = false);

	//디렉토리인지...파일인지..드라이브인지..
	/* 1 : dir
	   2 : file
	   3 : drive
	   0 : etc
	*/
#ifdef __WXMSW__
	int DropWindowSelectItemType();
#endif
	//아이템존재여부 확인
	std::vector<CNextMDirData>::iterator FindItem(const wxString& strName);

	std::vector<wxString>::iterator BackFowardDirBegin() { return m_vecVisitDir.begin(); }
	std::vector<wxString>::iterator BackFowardDirEnd() { return m_vecVisitDir.end(); }

	int GetBackFowardIndex() { return m_iBackFowardIndex; }
	void SetBackForwardIndex(int iIndex) { m_iBackFowardIndex = iIndex; }

	wxPoint GetItemPos();
	//압축 관련
	void ShowCompressPopupMenu();
	bool IsAvaliableDecompress();

	bool FindItemInMousePoint(const wxPoint& pt, bool IsMouseMove = false);
	void SetDnDUpdate();

protected:
	virtual void Render(wxDC* pDC) = 0;
	virtual void ApplyChangedViewSize() = 0;
	virtual void GotoTopDir() = 0;

	void Initialize();
	void SortStart();
	void Clear();

	//컬럼계산
	virtual void CalcColumn(wxDC* pDC);

	//컬럼자동
	virtual bool CalcAutoColumn(wxDC* pDC, const wxRect& viewRect);

	//아이템 표시 위치
	virtual void CalcPosition(wxDC* pDC);

	//항목 재계산
	virtual void ReCalcPage();

	//컬럼 그리기
	virtual void DrawColumn(wxDC* pDC);

	//정보표시
	virtual void DrawInfoArea(wxDC* pDC);

	//항목표시
	virtual void DisplayItems(wxDC* pDC);

	//디렉토리 정보 표시
	virtual void DisplayDirInfo(wxDC* pDC);

	//상세정보 표시
	virtual void DisplayDetailInfo(wxDC* pDC, const CNextMDirData& data, int iIconIndex, int iOverlayIconIndex);

	//파일 이미지 로딩
	wxThread::ExitCode Entry() override;

	bool RunFileImageInfoRead();
	void StopFileImageRead();

	//이미지 그리기
	void DrawItemImage(wxDC* pDC, int x, int y, int flags, int nIconIndex = 0, int nOverlayIndex = 0);

	//문자 ...표시
	wxString CalcStrEllipse(wxDC* pDC, const wxString& strSrc, bool IsDrive);

	//화면 디렉토리 넘버링
	bool IsShowDirectoryNumber() const { return m_bDirectoryNumbering; }

	//마우스이벤트 처리
	void DoMouseProcess(const wxPoint& pt, bool bDblClick = false);

	//키 입력처리
	void PreTranslateKeyEvent(wxKeyEvent& event);
	bool ProcessKeyEvent(int iKeyCode);
	bool ProcessEnterKey(int iKeyCode);

	void CalcLeftAndUpKey();
	void CalcRightAndDownKey();
	void CalcPageDown();
	void CalcPageUp();
	void CalcEnd();
	//아이템 선택
	bool SetSelectedItem();
	//화면 표시용 디스크용량
	virtual void SetDiskSpace(std::vector<CNextMDirData>::iterator it) = 0;
	//이아템 매칭처리
	void FindMatchItems();
	void DoMatchClear();
	void InitKeyInputTooltip();

	//콘텍스트 메뉴 보기
	void DisplayContextMenu(const wxPoint& pt);

	//즐겨찾기 보기
	void ShowBookmark();

	//이름변경
	void RenameOn(const wxString& strRename);
	void RenameStart(const wxString& strOldPathName, const wxString& strNewPathName);

	//파일 에디터 프로그램 보이기
	void ShowExternalProgramList();
	void ExecuteExternProgram(int iIndex);

private:
	void DisplaySelectedItemInfo(const wxString& strMsg, int xPos, int yPos);
	void DisplayRenameTooltip(const wxString& strMsg, int xPos, int yPos);
	void GotoVisitDirectory();

#ifdef __WXMSW__ //DragDrop
	HGLOBAL CopySelection();
#endif

protected:
	//디스크 스페이스 정보
	double m_dblTotalSpace      = 0.0f;
	double m_dblFreeSpace       = 0.0f;
	wxString m_strVolume        = wxT("");
	wxString m_strDiskSpaceDisp = wxT("");

	const int ICON_WIDTH              = 16;
	const int ICON_HEIGHT             = 16;
	const int ICON_SELECTED_WIDTH_GAP = 10;
	const int GAP_WIDTH               = 2;
	const int GAP_HEIGHT              = 2;

	//아이콘 컬러
	wxIcon   m_icoUpDir;
	wxColour m_colDefault;
	wxColour m_colDrive;
	wxColour m_colBackground;
	wxColour m_colDir;
	wxColour m_colSize;
	wxColour m_colTime;
	wxColour m_colAttr;
	wxColour m_colType;
	wxColour m_colMatch;
	wxColour m_colColumn;

	//화면폰트
    wxFont m_viewFont = wxNullFont;

	//Memory DC
	CMemoryDC m_memDC;

	//화면 사이즈
    wxRect m_viewRect = wxRect(0, 0, 0, 0);

    //디렉토리 정보 표시영역
	wxRect m_viewDirInfo      = wxRect(0, 0, 0, 0);

	//상세보기 영역
	wxRect m_viewRectDetail   = wxRect(0, 0, 0, 0);

	//화면 표시 영역
	wxRect m_viewRectDisp     = wxRect(0, 0, 0, 0);

    //윈도우 포커스 플래그
	bool m_bSetFocus    = false;

	//화면 사이즈 변경 플래그
    bool m_bSizeOrColumnChanged = false;

    //디렉토리 Load 플래그
	bool m_bDirLoaded           = false;

	// 화면에 표시가능한 아이템
	bool m_bDispFlag[6] = {false, };

	// 쓰레드 시작플래그
	bool m_bFileImageInfoReadStarted = false;

	//마우스 클릭&아이템찾기
	bool m_bMouseClickItemFound = false;

	//이름변경 준비
	bool m_bRename = false;

	//디렉토리 번호 표시 플래그
	bool m_bDirectoryNumbering  = false;

#ifdef __WXMSW__
	bool m_bImeMode = false;
#endif

	bool m_bMouseDown = false;

	// 전체아이템수
	int	m_iTotalItems         = 0;
	// 디렉토리수
	int m_iDirCount           = 0;
	// 파일수
	int m_iFileCount          = 0;
	// 컬럼수
	int	m_iDispColumn         = 0;
	// 문자 높이
	int m_iCharHeight         = 0;
	// 컬럼당 표시가능한 아이템수
	int m_iItemCountInColumn  = 0;
	// 컬럼끝 포인트
	int	m_iDispColumnEndPoint = 0;
	// 화면에 표시가능한 아이템수
	int	m_iDisplayItemInView  = 0;
	// 화면표시 아이템 시작위치
	int m_iStartIndex         = 0;
	//전체 포지션 카운터
	int m_iTotalPositionCnt   = 0;
	// 현재커서위치
	int m_iCurrentItemIndex   = 0;
	//선택 파일수
	int m_iSelFileCnt         = 0;
	//선택 디렉토리 수
	int m_iSelDirCnt          = 0;

	//Back,Forward index
	int m_iBackFowardIndex    = 0;
	//폴더 사이즈
	double m_dblFileSizeInDir = 0.0f;

	//Drag And Drop Index
	int m_iDnDBeforeIndex     = -1;
	// 가장긴 이름
	wxString m_strMaxName     = wxT("");
	// 가장긴 파일유형
	wxString m_strMaxTypeName = wxT("");
	//현재 패스
	wxString m_strCurrentPath = wxT("");
	//키 입력값
	wxString m_strKeyInput    = wxT("");

	//이름변경
	wxString m_strItemToRename = wxT("");

	//압축파일
	wxString m_strCompressedFile = wxT("");
	//표시컬럼
	COLUMN_VIEW_OPTION m_enumColumnViewOption;

	//디렉토리방문이력
	_HISTORY m_hashHistory;

	//선택항목 표시
	std::unique_ptr<CNextMTooltipView> m_pSelectedItemView = nullptr;
	//이름변경시 입력제한 툴팁
	std::unique_ptr<CNextMTooltipView> m_pRenameTooltip = nullptr;
	//키 입력
	std::unique_ptr<CNextMTooltipView> m_pKeyInputToolTip = nullptr;

	// 데이터
	std::vector<CNextMDirData> m_itemList;

	// 컬럼 라인 정보
	std::vector<CColumnPoint> m_ptList;

	// 포지션정보
	std::vector<CPositionInfo> m_posList;

	//Match 아이템 정보
	std::vector<int> m_matchItems;

	// 화면표시이름
	std::unordered_map<wxString, wxString> m_dispNameInfoMap;

	//선택 아이템
	std::unordered_map<int, SELITEM_INFO> m_hashSelectedItem;

	//이미지리스트
//	std::unique_ptr<CImageList> m_pImageList = nullptr;

	// 이름변경 텍스트컨트롤
	std::unique_ptr<wxTextCtrl> m_pTxtCtrlForRename = nullptr;

	//디렉토리 방문리스트
	//vector를 이용해서 처리함(디렉토리 탐색은 인덱스로 처리)
	std::vector<wxString> m_vecVisitDir;

#ifdef __WXMSW__
	std::unique_ptr<CNextMDropTarget> m_pDropTarget;
#endif // __WXMSW__
protected:
	//Default Event
    void OnCharHook(wxKeyEvent& event);
    void OnChar(wxKeyEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnKillFocus(wxFocusEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);

	//마우스 이벤트
	void OnMouseLBottonDown(wxMouseEvent& event);
	void OnMouseLBottonUp(wxMouseEvent& event);
	void OnMouseLButtonDBClick(wxMouseEvent& event);
	void OnMouseRButtonDown(wxMouseEvent& event);
	void OnMouseRButtonUp(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);

	void OnShowContextMenu(wxCommandEvent& event);

	//Custom Event
	//이름변경 이벤트
	void OnItemRename(wxCommandEvent &event);
	void OnEnterTextCtrl(wxCommandEvent& event);
	void OnKeyDownTextCtrl(wxKeyEvent& event);
	void OnKillFocusTxtCtrl(wxFocusEvent& event);
	//파일편집
	void OnFileEdit(wxCommandEvent &event);
	void OnExecuteExternalProgram(wxCommandEvent& event);

	//경로메뉴 처리
	void OnPathMenuOperation(wxCommandEvent& event);

	//보기메뉴
	//컬럼 변경
	void OnChangeViewColumn(wxCommandEvent& event);
	//Sorting
	void OnChangeSorting(wxCommandEvent& event);

	//상태바 즐겨찾기 클릭
	void OnShowFavoriteFromStatus(wxCommandEvent& event);

	//디렉토리 번호 표시
	void OnShowDirectoryNumber(wxCommandEvent& event);

	//압축관련

	//압축메뉴 선택 후..
	void OnCompressMenuExecute(wxCommandEvent& event);
	void OnCompressMenuUpdate(wxUpdateUIEvent& event);
	void OnDeCompressMenuExecute(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif // FILELISTVIEW_H_INCLUDED
