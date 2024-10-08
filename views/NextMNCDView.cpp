#include "../ginc.h"
#include "NextMNCDView.h"
#include "NextMViewManager.h"

#include "../dialog/DlgDirManager.h"
#include "../dialog/DlgDelete.h"
#include "../dialog/DlgAddDir.h"

wxBEGIN_EVENT_TABLE(CNextMNCDView, wxWindow)
	EVT_ERASE_BACKGROUND(CNextMNCDView::OnErase)
	EVT_PAINT(CNextMNCDView::OnPaint)
	EVT_SIZE(CNextMNCDView::OnSize)
	EVT_CHAR(CNextMNCDView::OnChar)
	EVT_SET_FOCUS(CNextMNCDView::OnSetFocus)
	EVT_LEFT_DOWN(CNextMNCDView::OnMouseLBClick)
	EVT_LEFT_DCLICK(CNextMNCDView::OnMouseLBDBClick)
	EVT_MOUSEWHEEL(CNextMNCDView::OnMouseWheel)
wxEND_EVENT_TABLE()

CNextMNCDView::CNextMNCDView(wxWindow* parent, const int nID, const wxSize& sz, const wxString& strTitle)
	: wxWindow(parent, nID, wxDefaultPosition, sz, CHILD_WINDOW_STYLE | wxWANTS_CHARS | wxVSCROLL | wxHSCROLL)
	, m_viewDispRect(0, 0, 0, 0)
	, m_crBackGround(wxColour(210, 210, 210))
	, m_crDefaultText(wxColour(70, 70, 70))
	, m_crSelectedText(wxColour(255, 51, 51))
	, m_crSelectedBackground(wxColour(60, 60, 60))
	, m_crSelectedParentBackground(wxColour(60, 60, 60))
	, m_crLine(wxColour(90, 90, 90))
	, m_pParent((DlgDirManager *)parent)
{
	m_icoDrive = wxArtProvider::GetIcon(wxART_HARDDISK, wxART_OTHER, wxSize(16, 16));
	m_icoDir = wxArtProvider::GetIcon(wxART_FOLDER, wxART_OTHER, wxSize(16, 16));

	if (theConfig->IsSelectedSCDItemToParent())
	{
		m_crSelectedBackground = wxColour(0, 220, 200);
		m_crSelectedText = wxColour(30, 30, 30);
	}

	Connect(wxID_ANY, wxEVT_SCROLLWIN_THUMBTRACK, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Connect(wxID_ANY, wxEVT_SCROLLWIN_THUMBRELEASE, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Connect(wxID_ANY, wxEVT_SCROLLWIN_LINEUP, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Connect(wxID_ANY, wxEVT_SCROLLWIN_LINEDOWN, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Connect(wxID_ANY, wxEVT_SCROLLWIN_PAGEUP, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Connect(wxID_ANY, wxEVT_SCROLLWIN_PAGEDOWN, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);

	m_pKeyInputToolTip = std::make_unique<CNextMTooltipView>(this);
	m_pKeyInputToolTip->SetSize(60, 20);
	m_pKeyInputToolTip->Show(false);

	m_pTxtCtrlForRename = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxBORDER_THEME);
	m_pTxtCtrlForRename->SetThemeEnabled(true);

	m_pTxtCtrlForRename->SetBackgroundColour(wxColour(220, 220, 220));
	m_pTxtCtrlForRename->SetBackgroundStyle(wxBG_STYLE_PAINT);
	m_pTxtCtrlForRename->SetFont(*_gViewFont);
	m_pTxtCtrlForRename->Show(false);

	m_pTxtCtrlForRename->Connect(wxEVT_CHAR_HOOK, wxKeyEventHandler(CNextMNCDView::OnCharHookFromTextCtrl), NULL, this);
	m_pTxtCtrlForRename->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(CNextMNCDView::OnTextCtrlEnter), NULL, this);
}

CNextMNCDView::~CNextMNCDView()
{
	Disconnect(wxID_ANY, wxEVT_SCROLLWIN_THUMBTRACK, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Disconnect(wxID_ANY, wxEVT_SCROLLWIN_THUMBRELEASE, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Disconnect(wxID_ANY, wxEVT_SCROLLWIN_LINEUP, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Disconnect(wxID_ANY, wxEVT_SCROLLWIN_LINEDOWN, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Disconnect(wxID_ANY, wxEVT_SCROLLWIN_PAGEUP, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);
	Disconnect(wxID_ANY, wxEVT_SCROLLWIN_PAGEDOWN, wxScrollWinEventHandler(CNextMNCDView::OnScroll), NULL, this);

	m_pTxtCtrlForRename->Disconnect(wxEVT_CHAR_HOOK, wxKeyEventHandler(CNextMNCDView::OnCharHookFromTextCtrl), NULL, this);
	m_pTxtCtrlForRename->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(CNextMNCDView::OnTextCtrlEnter), NULL, this);
}

void CNextMNCDView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

    m_viewRect = GetClientRect();
	wxMemoryDC* pMemDC = m_memDC.CreateMemoryDC(&dc, m_viewRect, wxColour(0, 0, 0), m_crBackGround);

#ifdef __WXMSW__
	pMemDC->SetFont(*_gViewFont);
#else
	pMemDC->SetFont(m_font);
#endif

	Render(pMemDC);

	dc.Blit(0, 0, m_viewRect.GetWidth(), m_viewRect.GetHeight(), pMemDC, 0, 0);
}

void CNextMNCDView::OnErase(wxEraseEvent& event)
{

}

void CNextMNCDView::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

	m_bSizeChanged = true;
    theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMNCDView::Render(wxDC* pDC)
{
	if(m_bSizeChanged)
		CalcRowInView(pDC);

	DisplayNCD(pDC);
	m_bSizeChanged = false;
}

void CNextMNCDView::DisplayNCD(wxDC* pDC)
{
	if(m_iNCDSize == 0 || m_iRowInView == 0)
		return;

#ifdef __WXMSW__
	wxFont font(m_viewRect.GetWidth() * 0.12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Tahoma"));
	pDC->SetFont(font);

	wxSize szVolume = pDC->GetTextExtent(m_strVolume);

	int iVolL = m_viewDispRect.GetRight() - (szVolume.GetWidth() + (GAP_POINT * 40));
	int iVolR = m_viewDispRect.GetTop() - (GAP_POINT * 5);

	pDC->SetTextForeground(wxColour(80, 80, 80));
	pDC->DrawText(m_strVolume, wxPoint(iVolL + 2, iVolR + 2));

	pDC->SetTextForeground(wxColour(160, 160, 160));
	pDC->DrawText(m_strVolume, wxPoint(iVolL, iVolR));

	pDC->SetFont(*_gViewFont);
#endif // __WXMSW__

	//화면에 표시될 시작 노드
	std::vector<DirNode *>::iterator itStart = m_itStartNode;

	//전체노드의 마지막 노드
	std::vector<DirNode *>::iterator itEnd   = theNCD->End();

	int iRow = 0;
	int iDepth = 0;

	int iIconLeft = 0;
	int iIconTop = 0;

	int iLeft   = 0;
	int iTop    = 0;
	int iRight  = 0;
	int iBottom = 0;

	//시작노드를 기점으로 화면의 해당 row가 어디인지를 나타냄
	m_iCurRow = (*m_itStartNode)->_row % m_iRowInView;

	wxSize szName;
	wxRect rcDispName;
	wxIcon dispIco;

	do
	{
		DirNode* pNode = *itStart;
	//	if(pNode->_depth < m_iCurDepth)
	//		continue;

		//화면에 표시될 row 계산
		//MoveDown()함수 호출시 동일 depth의 아이템을 찾을 경우 startindex의 시작위치를 빼고 계산하면
		//다음 화면으로 넘어갈 경우 다음선택 아이템을 항상 위로 이동시킬 수 있음
		int iRow = (pNode->_row - m_iCurRow) % m_iRowInView;

		iIconLeft = m_viewDispRect.GetLeft() + ((pNode->_depth - m_iCurDepth) * m_iDispColsWidth);
		iIconTop  = m_viewDispRect.GetTop() + (iRow * DISP_ROW_HEIGHT);

		iLeft = iIconLeft + (ICON_WIDTH + GAP_POINT);
		iTop  = iIconTop - 1;

		szName = pDC->GetTextExtent(pNode->_name);

		//자식 노드가 있을경우는 크기를 지정하고 자식노드가 없으면 전체 Row
		iRight = pNode->_childNode.size() > 0 ? iLeft + m_iEndWidthItem : iLeft + szName.GetWidth();
		iBottom = iTop + (szName.GetHeight()) + 2;

		rcDispName.SetLeft(iLeft);
		rcDispName.SetTop(iTop + 2);
		rcDispName.SetRight(iRight);

		if(szName.GetWidth() < m_iEndWidthItem)
			rcDispName.SetRight(iLeft + szName.GetWidth() + GAP_POINT);

		rcDispName.SetBottom(iBottom);

		pNode->_rc = rcDispName;

		wxString strDisp = CalcEllipse(pDC, pNode->_name, rcDispName.GetWidth());

		bool bSelected = false;
		if(m_itIndex == itStart)
		{
			bSelected = true;
			wxRect rcSelected;

			rcSelected.SetLeft(rcDispName.GetLeft() - (ICON_WIDTH + 5));
			rcSelected.SetTop(rcDispName.GetTop() - 4);
			rcSelected.SetRight(rcDispName.GetRight() + 2);
			rcSelected.SetBottom(rcDispName.GetBottom() + 2);

			wxBrush bkBrush(m_crSelectedBackground);
			wxPen bkPen(m_crSelectedBackground);

			pDC->SetBrush(bkBrush);
			pDC->SetPen(bkPen);

			pDC->DrawRectangle(rcSelected);

			//현재 선택된 노드의 부모노드를 표시한다.
			DisplayParentPath(pDC, (*m_itIndex));
		}

		pDC->SetTextForeground(m_crDefaultText);

		if (bSelected)
			pDC->SetTextForeground(m_crSelectedText);

		dispIco = pNode->_parent == nullptr ? m_icoDrive : m_icoDir;

		pDC->DrawLabel(strDisp, rcDispName, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
		pDC->DrawIcon(dispIco, wxPoint(iIconLeft, iIconTop));

		//시작포인트의 부모노드를 표시한다.
		DisplayParentPath(pDC, *m_itStartNode, true);

		//row의 시작은 0이므로 row + 1을 해야 화면에 디렉토리 tree가 표시됨
		//그렇지 않으면 드라이브만 표시됨
		if( (iRow + 1) % m_iRowInView == 0)
		{
			if(pNode->_childNode.size() == 0)
				break;
		}
	} while(++itStart != itEnd);

	//가장 마지막에 출력된 항목을 저장함.
	m_itEndDispNode = itStart;
	if(m_itEndDispNode == theNCD->End())
		--m_itEndDispNode;

	//라인을 그린다.
	DrawLine(pDC, m_itStartNode);
}

void CNextMNCDView::DisplayParentPath(wxDC* pDC, DirNode* pSelNode, bool IsStart)
{
	DirNode* pNode = pSelNode->_parent;
	if(pNode == nullptr)
		return;

	wxIcon dispParent;

	do
	{
		if(pSelNode->_row != pNode->_row)
		{
			if(pNode->_depth < m_iCurDepth)
				break;

			wxRect rcPath;

			wxSize szName = pDC->GetTextExtent(pNode->_name);

			int iLeft = m_viewDispRect.GetLeft() + ((pNode->_depth - m_iCurDepth) * m_iDispColsWidth) + (ICON_WIDTH + GAP_POINT); //pNode->_rc.GetLeft();
			int iTop = pSelNode->_rc.GetTop();
			int iRight = pNode->_childNode.size() > 0 ? iLeft + m_iEndWidthItem : iLeft + szName.GetWidth(); //iLeft + szName.GetWidth(); //pNode->_rc.GetRight();
			int iBottom = pSelNode->_rc.GetBottom();

			rcPath.SetLeft(iLeft);
			rcPath.SetTop(iTop);
			rcPath.SetRight(iRight);
			rcPath.SetBottom(iBottom);

		/*	if(rcPath.GetWidth() == 0)
			{
				wxSize szName = pDC->GetTextExtent(pNode->_name);

				rcPath.SetLeft(iLeft + (((pNode->_depth - m_iCurDepth) * m_iDispColsWidth) +  (2 * GAP_POINT) + ICON_WIDTH));
				rcPath.SetRight(rcPath.GetLeft() + szName.GetWidth() + 1);
			}
		*/
			wxString strDisp = CalcEllipse(pDC, pNode->_name, rcPath.GetWidth());

			pDC->SetTextForeground(IsStart ? m_colStartIndex : m_colParent);
			pDC->DrawLabel(strDisp, rcPath, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

			dispParent = pNode->_parent == nullptr ? m_icoDrive : m_icoDir;

			int iIconL = rcPath.GetLeft() - (ICON_WIDTH + GAP_POINT);
			int iIconT = rcPath.GetTop();

			pDC->DrawIcon(dispParent, wxPoint(iIconL, iIconT));
		}

		pNode = pNode->_parent;

	} while(pNode != nullptr);
}

/*
 * 디렉토리 라인을 그린다.
 * @Parameter
 *  pDC : 화면에 라인을 그리기 위한 Device Context
 *  itStart : 라인을 그리기 위한 시작 iterator
 */
void CNextMNCDView::DrawLine(wxDC* pDC, std::vector<DirNode *>::iterator itStart)
{
	wxPen bkPen(m_crLine, 1, wxPENSTYLE_SOLID);
	pDC->SetPen(bkPen);

	//마지막 노드일때까지..
	while(itStart <= m_itEndDispNode)
	{
		DirNode* pDispNode = (*itStart);

		int px1 = pDispNode->_rc.GetRight() + GAP_POINT;
		int py1 = pDispNode->_rc.GetTop() + (pDispNode->_rc.GetHeight() / 2);

		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;

		int x1_first = 0;
		int y1_first = 0;

		std::vector<DirNode *>::const_iterator cIt = pDispNode->_childNode.begin();
		//노드의 자식 노드에 대한 라인을 그린다.
		while(cIt != pDispNode->_childNode.end())
		{
			DirNode *pChildNode = (*cIt);

			x1 = pChildNode->_rc.GetLeft() - (ICON_WIDTH + (GAP_POINT * 2));
			y1 = pChildNode->_rc.GetTop() + (pChildNode->_rc.GetHeight() / 2);

			//자식노드와 row가 같은경우(첫번째 child의 경우)
			if(pDispNode->_row == pChildNode->_row)
			{
				pDC->DrawLine(px1, py1, x1, y1);
				x1_first = x1 - GAP_POINT * 4;
				y1_first = y1;
			}
			else
			{	//자식노드가 현재 표시되고 있는 마지막 노드보다 아래에 있는경우는 바닥까지 라인그리고
				//그 외에는 ├ 모양의 라인을 그린다.
				if(pChildNode->_row > (*m_itEndDispNode)->_row)
					pDC->DrawLine(x1_first, y1_first, x1_first, m_viewDispRect.GetBottom());
				else
				{
					pDC->DrawLine(x1_first, y1_first, x1_first, y1);
					pDC->DrawLine(x1_first, y1, x1, y1);
				}
			}

			++cIt;
		}

		/*
		 * 부모노드에 대한 라인을 그린다.
		 * 부모노드가 화면에 보이지 않을않고 자식노드들이 화면상에 다 표시되지 않을경우
		 * 화면전체에 라인을 그린다.
		 */
		if(pDispNode->_parent != nullptr)
		{
			while((pDispNode->_depth - m_iCurDepth) >= 1)
			{
				std::vector<DirNode *>::const_iterator cItChBegin = pDispNode->_parent->_childNode.begin();
				std::vector<DirNode *>::const_iterator cItChEnd   = pDispNode->_parent->_childNode.end();

				//현재노드가 자식노드의 어느위치인지 찾음.
				int cmp = 0;
				for(cItChBegin; cItChBegin != cItChEnd; ++cItChBegin)
				{
			#ifdef __WXMSW__
					cmp = (*cItChBegin)->_name.CmpNoCase(pDispNode->_name);
			#else
					cmp = (*cItChBegin)->_name.Cmp(pDispNode->_name);
			#endif
					if(cmp == 0)
						break;
				}

				x2 = pDispNode->_rc.GetLeft() - (ICON_WIDTH + (GAP_POINT * 2));
				y2 = pDispNode->_rc.GetTop() + (pDispNode->_rc.GetHeight() / 2);

				//마지막 자식노드
				DirNode* pLastChildNode = *(cItChEnd - 1);

				//자식노드가 화면에 보이고 부모노드가 화면에 보지 않을경우
				if(pDispNode->_parent->_row < (*m_itStartNode)->_row)
				{
					if( (*cItChBegin)->_row >= (*m_itStartNode)->_row)
					{
						pDC->DrawLine(x2 - (GAP_POINT * 4), y2, x2, y2);
						pDC->DrawLine(x2 - (GAP_POINT * 4), y2, x2 - (GAP_POINT * 4), m_viewDispRect.GetTop());

						//미자막 자식노드가 화면에 보이지 않을경우
						if( pLastChildNode->_row > (*m_itEndDispNode)->_row)
							pDC->DrawLine(x2 - (GAP_POINT * 4), y2, x2 - (GAP_POINT * 4), m_viewDispRect.GetBottom());
					}
					else
					{
						//미자막 자식노드가 화면에 보이지 않을경우
						if( pLastChildNode->_row > (*m_itEndDispNode)->_row)
							pDC->DrawLine(x2 - (GAP_POINT * 4), m_viewDispRect.GetTop(), x2 - (GAP_POINT * 4), m_viewDispRect.GetBottom());
					}
				}

				pDispNode = pDispNode->_parent;
			}
		}

		++itStart;
	}
}

/*
 * 화면문자에 ...을 표시한다.
 *
 *
 */
wxString CNextMNCDView::CalcEllipse(wxDC* pDC, const wxString& strSrc, const int iDispWidth)
{
	wxString strDisp(wxT(""));
	wxString strName(strSrc);

	wxSize szNameSize = pDC->GetTextExtent(strName);

	int iLen = strName.length();

	if (szNameSize.GetWidth() < iDispWidth)
		strDisp = strName;
	else
	{
		for (int iIndex = 0; iIndex < iLen; iIndex++)
		{
			strDisp = strName.Left(iIndex + 1);
			wxSize sizeText = pDC->GetTextExtent(wxString(strDisp + wxT("...")));
			if ((sizeText.GetWidth()) >= iDispWidth)
			{
				strDisp = strName.Left(iIndex);
				break;
			}
		}

		strDisp += wxT("...");
	}

	return strDisp;
}

/*
 * 화면의 표시영역 계산
 * 화면에 표시할 수 있는 row를 계산하여 현재의 시작위치가 화면표시 row보다 작을경우 시작위치는 root
 * 그렇지 않으면 현재 노드의 row와 같은 노드까지 시작위치 이동..
 *
 */
void CNextMNCDView::CalcRowInView(wxDC* pDC)
{
	//디렉토리 트리를 그리기 위한 영역
	m_viewDispRect.SetLeft(m_viewRect.GetLeft()     + GAP_POINT * 2);
	m_viewDispRect.SetTop(m_viewRect.GetTop()       + GAP_POINT * 2);
	m_viewDispRect.SetRight(m_viewRect.GetRight()   - GAP_POINT);
	m_viewDispRect.SetBottom(m_viewRect.GetBottom() - GAP_POINT);

	//화면에 표시할 수 있는 Rows
	m_iRowInView     = (m_viewDispRect.GetHeight() / DISP_ROW_HEIGHT) + 1;

	//화면에 표시할 수 있는 Cols
	m_iDispColsWidth = m_viewDispRect.GetWidth() / m_iDispColsDepth;
	m_iEndWidthItem = m_iDispColsWidth / 2 + 20;

	//현재 노드의 위치가 화면표시Row값보다 작은경우는 시작위치를 root 노드로 설정
	if( (*m_itIndex)->_row < m_iRowInView)
		m_itStartNode = theNCD->Begin();
	else
		ResetBaseIteratorRow();

	const int maxDepth = theNCD->GetMaxDepth();
	const int maxRow   = theNCD->GetMaxRow();

	this->SetScrollbar(wxVERTICAL, 0, 1, maxRow);
	this->SetScrollPos(wxVERTICAL, (*m_itIndex)->_row);
	this->SetScrollbar(wxHORIZONTAL, 0, 1, maxDepth);
	this->SetScrollPos(wxHORIZONTAL, (*m_itIndex)->_depth);
}

void CNextMNCDView::LoadDirInfo()
{
	m_itStartNode = theNCD->GetCur();
	m_itIndex   = m_itStartNode;//theNCD->GetCur();
	m_iNCDSize  = theNCD->GetSize();

	DirNode* pNode = *m_itIndex;
	m_pParent->SetNodePath(pNode->_fullName);

	m_bSizeChanged = true;

#ifdef __WXMSW__
	std::vector<DirNode *>::const_iterator cIt = theNCD->Begin();
	m_strVolume = (*cIt)->_name.Left(2);
#endif // __WXMSW__
}

void CNextMNCDView::OnChar(wxKeyEvent& event)
{
	event.Skip();

	int iKeyCode = event.GetKeyCode();

	bool bShift = wxIsShiftDown();
	bool bCtrl = wxIsCtrlDown();

	wxString strKeyName(theUtility->GetKeyName(event));
	if(bShift)
	{
#ifdef __WXMSW__
		if (theConfig->IsShift_IME_KOR_MoveDrive())
			theUtility->SetImeModeToEnglish(this);

		wxString strDriveName = strKeyName;
		std::vector<CNextMDirData>::const_iterator cIt = theDriveInfo->GetDriveItem(strDriveName);
		if(cIt != theDriveInfo->EndIter())
		{
			wxString strTreeDataPath = theUtility->GetWorkDir() + SLASH + wxT("treedata");
			if (!wxDirExists(strTreeDataPath))
				wxMkDir(strTreeDataPath);

			wxString strNCDFilePath = strTreeDataPath + SLASH + strDriveName + wxT(".NCD");
			m_pParent->SetNCDFileInfo(strNCDFilePath, cIt->GetDriveName());
			m_pParent->LoadNCDData(true);

			return;
		}
#endif // __WXMSW__
	}
	else if(bCtrl)
	{
		bool bPressFKey = false;
#ifdef __WXMSW__
		bPressFKey = (strKeyName.CmpNoCase(wxT("Ctrl-F")) == 0);
#else
		bPressFKey = (strKeyName.Cmp(wxT("Ctrl-F")) == 0);
#endif
		if (bPressFKey)
		{
			this->Freeze();
			m_pParent->FindDir();
			this->Thaw();

			return;
		}
	}

	std::unordered_map<int, int>::const_iterator cIterFind = _gmapAllowProcessKey.find(iKeyCode);
	if(cIterFind != _gmapAllowProcessKey.end())
	{
		ProcessKeyEvent(event.GetKeyCode());

		if(!m_strKeyInput.IsEmpty())
		{
			m_pKeyInputToolTip->SetTooltipText(wxT(""));
			m_pKeyInputToolTip->Show(false);
			m_strKeyInput = wxT("");
		}

		return;
	}

	if(!bCtrl)
	{
		//일반키 입력시 일치항목 찾기...
		if(!isdigit(iKeyCode) && !isalpha(iKeyCode) && iKeyCode != WXK_SPACE)
			return;

		m_strKeyInput += strKeyName;

		wxClientDC dc(this);

		wxSize sztip = dc.GetTextExtent(m_strKeyInput);
		wxSize szTooltip;

		wxPoint ptTooltip(m_viewDispRect.GetRight() / 2 - sztip.GetWidth(), m_viewDispRect.GetHeight() / 2);

		szTooltip.SetWidth(sztip.GetWidth() + 10);
		szTooltip.SetHeight(sztip.GetHeight() + 5);

		m_pKeyInputToolTip->SetTooltipText(m_strKeyInput);
		m_pKeyInputToolTip->SetThemeEnabled(true);
		m_pKeyInputToolTip->SetPosition(ptTooltip);
		m_pKeyInputToolTip->SetSize(szTooltip);
		m_pKeyInputToolTip->Show(true);
	}
	else
	{
		bool bPressNKey = false;
#ifdef __WXMSW__
		bPressNKey = (strKeyName.CmpNoCase(wxT("Ctrl-N")) == 0);
#else
		bPressNKey = (strKeyName.Cmp(wxT("Ctrl-N")) == 0);
#endif
		if(!bPressNKey)
			return;
	}

	FindMatch(strKeyName);
}

void CNextMNCDView::ProcessKeyEvent(const int iKeyCode)
{
	switch(iKeyCode)
	{
		case WXK_RETURN:
		{
			wxString strPath = (*m_itIndex)->_fullName;
			strPath = strPath.Left(strPath.Len() - SLASH.Len());

			theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strPath, CHANGE_DIR_TAB);
			theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strPath, CHANGE_DIR_PATHVIEW);
			theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strPath, CHANGE_DIR_FILELISTIVEW);

			wxCloseEvent evt(wxEVT_CLOSE_WINDOW);
			wxPostEvent(m_pParent, evt);
		}
			break;

		case WXK_ESCAPE:
		{
			if(!m_strKeyInput.IsEmpty())
				return;

			wxCloseEvent evt(wxEVT_CLOSE_WINDOW);
			wxPostEvent(m_pParent, evt);
		}
			break;

		case WXK_LEFT:
			if(!MoveLeft())
				return;

			break;

		case WXK_RIGHT:
			if(!MoveRight())
				return;

			break;

		case WXK_UP:
			if(!MoveUp())
				return;

			break;

		case WXK_DOWN:
			if(!MoveDown())
				return;

			break;

		case WXK_F2:
			RenameStart((*m_itIndex)->_name);
			break;

		case WXK_F5:
		{
			int iRet = wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_SCD_SEARCH_ENTIRE_DRV")), PROGRAM_FULL_NAME, wxICON_QUESTION | wxYES_NO);
			if (iRet == wxYES)
			{
				wxString strFullPath = (*m_itIndex)->_fullName;
				wxString strNCDFilePath = theUtility->GetWorkDir() + SLASH + wxT("treedata") + SLASH + strFullPath.Left(1) + wxT(".NCD");

				m_pParent->SetNCDFileInfo(strNCDFilePath, strFullPath);
				m_pParent->RefreshAll();
			}
		}

			break;

		case WXK_F6:
		{
			if(m_itIndex == theNCD->Begin())
			{
				wxString strMsg = wxString::Format(theMsg->GetMessage(wxT("MSG_MCD_DRIVE_SELECT_AND_RELOAD")), (*m_itIndex)->_fullName);
				theUtility->ShowMessageBox(strMsg, wxICON_QUESTION | wxOK);
				return;
			}

			DirNode* pCurNode = *m_itIndex;
			m_pParent->LoadSubDir(pCurNode);
		}
			break;

		case WXK_F7:
			AddDirectory();
			break;

		case WXK_DELETE:
			DeleteDirectory();
			break;

		default:
			return;
			break;
	}

	m_pParent->SetNodePath((*m_itIndex)->_fullName);
	theUtility->RefreshWindow(this, m_viewRect);
}

bool CNextMNCDView::MoveLeft()
{
	if(m_itIndex != theNCD->Begin())
	{
		DirNode* pParentNode = (*m_itIndex)->_parent;
		m_itIndex = theNCD->Begin() + pParentNode->_index;

		if( (*m_itIndex)->_row < (*m_itStartNode)->_row)
			ResetBaseIteratorDepth();

		DirNode* pNode = (*m_itIndex);
		if(pNode->_rc.GetRight() < m_viewDispRect.GetRight())
		{
			m_iCurDepth--;
			if(m_iCurDepth <= 0)
				m_iCurDepth = 0;

			this->SetScrollPos(wxHORIZONTAL, m_iCurDepth);
		}

		return true;
	}

	return false;
}

bool CNextMNCDView::MoveRight()
{
	if(m_itIndex != theNCD->End() - 1)
	{
		m_itIndex++;
		if( (*m_itEndDispNode)->_row < (*m_itIndex)->_row)
			m_itStartNode = m_itIndex;

		DirNode* pNode = (*m_itIndex);
		if(pNode->_rc.GetRight() > m_viewDispRect.GetRight())
		{
			m_iCurDepth++;
			if(m_iCurDepth >= theNCD->GetMaxDepth())
				m_iCurDepth = theNCD->GetMaxDepth();

			this->SetScrollPos(wxHORIZONTAL, m_iCurDepth);
		}

		return true;
	}

	return false;
}

bool CNextMNCDView::MoveUp()
{
	if((*m_itIndex)->_row == 0)
		return MoveLeft();

	//root node
	if( (*m_itIndex)->_parent == nullptr)
		return false;

	std::vector<DirNode *>::iterator itBegin = theNCD->Begin();
	std::vector<DirNode *>::iterator itEnd   = m_itIndex - 1;

	bool bFound = false;
	do
	{
		if((*itEnd)->_depth == (*m_itIndex)->_depth)
		{
			m_itIndex = itEnd;
			bFound = true;
			break;
		}
	} while(--itEnd != itBegin); //현재노드의 이전노드까지 체크

	if(!bFound)
		return MoveLeft();

	if( (*m_itIndex)->_row < (*m_itStartNode)->_row)
	{
		ResetBaseIteratorDepth();
		if(m_itStartNode != theNCD->Begin())
				ResetBaseIteratorRow();
	}

	return true;
}

bool CNextMNCDView::MoveDown()
{
	std::vector<DirNode *>::iterator itBegin = m_itIndex + 1;
	std::vector<DirNode *>::iterator itEnd   = theNCD->End() - 1;

	if(m_itEndDispNode == theNCD->End())
		m_itEndDispNode = itEnd;

	if(m_itIndex == itEnd)
		return false;

	DirNode* pNode = *m_itIndex;
	bool bFound = false;

	if(pNode->_row < (*itEnd)->_row)
	{
		while(itBegin != theNCD->End())
		{
			if((*itBegin)->_depth == (*m_itIndex)->_depth)
			{
				m_itIndex = itBegin;

				bFound = true;
				break;
			}

			++itBegin;
		} //while(++itBegin != itEnd); //마지막 노드까지

		if(!bFound)
			return MoveRight();

		if( (*m_itIndex)->_row > (*m_itEndDispNode)->_row)
		{
			m_itStartNode = m_itIndex;
			ResetBaseIteratorRow();
		}
	}
	else
		return false;

	return true;
}

/*
 * 노드의 시작위치 계산(row)
 *
 *
 */
void CNextMNCDView::ResetBaseIteratorRow()
{
	do
	{
		if((*m_itStartNode)->_parent->_row == (*m_itIndex)->_row)
		{
			m_itStartNode--;
			continue;
		}

		break;

	} while(1);
}

/*
 * 노드의 시작위치 계산(Depth)
 *
 *
 */
void CNextMNCDView::ResetBaseIteratorDepth()
{
	--m_itStartNode;

	do
	{
		if((*m_itStartNode)->_depth == (*m_itIndex)->_depth)
		{
			if( (*m_itStartNode)->_row < m_iRowInView)
				m_itStartNode = theNCD->Begin();

			break;
		}

		--m_itStartNode;
	} while(1);
}

void CNextMNCDView::OnSetFocus(wxFocusEvent& event)
{
	theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMNCDView::OnScroll(wxScrollWinEvent& event)
{
	wxEventType evtType = event.GetEventType();
	int iOrientation = event.GetOrientation();
	int iPos = event.GetPosition();

	if (iOrientation == wxVERTICAL)
	{
		ScrollVerticalProcess(evtType, false, iPos);
		this->SetScrollPos(wxVERTICAL, (*m_itStartNode)->_row);

		m_itIndex = m_itStartNode;
	}
	else if (iOrientation == wxHORIZONTAL)
	{
		ScrollHorzProcess(evtType, iPos);
		this->SetScrollPos(wxHORIZONTAL, m_iCurDepth);
	}

	theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMNCDView::ScrollVerticalProcess(wxEventType evtType, bool bWheel, int iThumbTrackPos)
{
	if (evtType == wxEVT_SCROLLWIN_LINEDOWN)
	{
		std::vector<DirNode *>::iterator It = m_itStartNode;
		while(1)
		{
			if( (*It)->_row != (*m_itStartNode)->_row)
				break;

			++It;
		}

		m_itStartNode = It;
	}
	else if(evtType == wxEVT_SCROLLWIN_LINEUP)
	{
		std::vector<DirNode *>::iterator It = m_itStartNode;
		if((*m_itStartNode)->_row > m_iRowInView)
		{
			while(1)
			{
				if( (*It)->_row != (*m_itStartNode)->_row)
					break;

				--It;
			}

			m_itStartNode = It;
		}
		else
			m_itStartNode = theNCD->Begin();
	}
	else if(evtType == wxEVT_SCROLLWIN_THUMBTRACK)
	{
		std::vector<DirNode *>::iterator it = m_itStartNode;
		bool bUp = false;

		if (m_iThumbTrackPos > iThumbTrackPos)
			bUp = true;

		while(1)
		{
			DirNode* pNode = (*it);
			if(pNode->_row == iThumbTrackPos)
			{
				m_itStartNode = it;
				break;
			}

			if(bUp) --it;
			else   	++it;
		}

		if(bUp)
		{
			if((*m_itStartNode)->_row < m_iRowInView)
				m_itStartNode = theNCD->Begin();
		}

		m_iThumbTrackPos = iThumbTrackPos;
	}
	else if (evtType == wxEVT_SCROLLWIN_PAGEUP)
	{
		int iCurRow = (*m_itIndex)->_row - m_iRowInView;
		std::vector<DirNode *>::iterator it = m_itStartNode;

		if(m_itStartNode == theNCD->Begin())
			return;

		while(1)
		{
			DirNode* pNode = (*it);
			if( (pNode->_row == iCurRow) || it == theNCD->Begin())
			{
				m_itStartNode = it;
				break;
			}

			--it;
		}

		m_itIndex = m_itStartNode;

	}
	else if (evtType == wxEVT_SCROLLWIN_PAGEDOWN)
	{
		int iCurRow = (*m_itIndex)->_row + m_iRowInView;
		std::vector<DirNode *>::iterator it = m_itStartNode;

		if(m_itEndDispNode == theNCD->End() - 1)
			return;

		while(1)
		{
			DirNode* pNode = (*it);
			if(pNode->_row == iCurRow)
			{
				m_itStartNode = it;
				break;
			}

			++it;
		}

		m_itIndex = m_itStartNode;
	}
}

void CNextMNCDView::ScrollHorzProcess(wxEventType evtType, int iThumbTrackPos)
{
	if(evtType == wxEVT_SCROLLWIN_LINEDOWN)
	{
		m_iCurDepth++;
		if(m_iCurDepth >= theNCD->GetMaxDepth())
			m_iCurDepth == theNCD->GetMaxDepth();
	}
	else if(evtType == wxEVT_SCROLLWIN_LINEUP)
	{
		m_iCurDepth--;
		if(m_iCurDepth <= 0)
			m_iCurDepth = 0;
	}
}

void CNextMNCDView::FindMatch(const wxString& strKey)
{
	if(m_strKeyInput.IsEmpty())
		return;

	wxString strKeyInput(m_strKeyInput);
	wxString strRegEx = wxT("(^") + strKeyInput.MakeLower() + wxT(")");

	wxRegEx _regex(strRegEx);

	int _cmpContrlAndNKey;
#ifdef __WXMSW__
	_cmpContrlAndNKey = strKey.CmpNoCase(wxT("Ctrl-N"))	;
#else
	_cmpContrlAndNKey = strKey.Cmp(wxT("Ctrl-N"))	;
#endif // __WXMSW__

	std::vector<DirNode *>::iterator It;
	if(_cmpContrlAndNKey == 0)
		It = m_itIndex + 1;
	else
		It = theNCD->Begin();

	while(It != theNCD->End())
	{
		DirNode* pNode = (*It);
		wxString strName(pNode->_name);

		if (_regex.Matches(strName.MakeLower()))
		{
			m_itIndex = It;
			if((*m_itIndex)->_row > m_iRowInView)
			{
				m_itStartNode = It - 5;
				ResetBaseIteratorRow();
			}
			else
			{
				if((*m_itStartNode)->_row > m_iRowInView)
					m_itStartNode = theNCD->Begin();
			}

			theUtility->RefreshWindow(this, m_viewRect);

			break;
		}

		++It;
	}
}

void CNextMNCDView::OnTextCtrlEnter(wxCommandEvent& event)
{
	DirNode* pNode = (*m_itIndex);
	wxString strNewName = m_pTxtCtrlForRename->GetValue();
	wxString strOldName = pNode->_name;

	if (strNewName.Cmp(strOldName) == 0)
	{
		wxMessageBox(strNewName + wxT(" is the same dir exist\nInput different name"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return;
	}

	if(!CLocalFileSystem::CheckCreatable(strNewName))
	{
		RenameStart(strNewName);
		return;
	}

	theNCD->ChangeNodeName(pNode, strNewName);

	m_pTxtCtrlForRename->SetLabelText(wxT(""));
	m_pTxtCtrlForRename->Show(false);

	m_pKeyInputToolTip->SetTooltipText(wxT(""));
	m_pKeyInputToolTip->Show(false);

	event.Skip();

	theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMNCDView::OnCharHookFromTextCtrl(wxKeyEvent& event)
{
	int iKeyCode = event.GetKeyCode();
	if (iKeyCode == WXK_ESCAPE)
	{
		m_pTxtCtrlForRename->SetLabelText(wxT(""));
		m_pTxtCtrlForRename->Show(false);

		m_pKeyInputToolTip->SetTooltipText(wxT(""));
		m_pKeyInputToolTip->Show(false);
	}
	else
		event.Skip();
}

void CNextMNCDView::RenameStart(const wxString& strName)
{
	wxClientDC dc(this);

	DirNode* pNode = *m_itIndex;

	int iPosX1 = pNode->_rc.GetLeft() - (ICON_WIDTH + GAP_POINT * 2);
	int iPosY1 = pNode->_rc.GetTop() - 5;
	int iPosX2 = pNode->_rc.GetWidth() + GAP_POINT * 7;
	int iPosY2 = pNode->_rc.GetHeight() + 5;

	m_pTxtCtrlForRename->SetSize(wxRect(iPosX1, iPosY1, iPosX2, iPosY2));
	m_pTxtCtrlForRename->SetLabelText(strName);
	m_pTxtCtrlForRename->SetSelection(0, strName.Length());
	m_pTxtCtrlForRename->Show(true);
	m_pTxtCtrlForRename->SetFocus();

	wxString strDontUse(theMsg->GetMessage(wxT("MSG_INFO_RENAME_DONTUSE")) + theMsg->GetMessage(wxT("MSG_INFO_RENAME_DONTUSE_STRING")));

	wxSize sztip = dc.GetTextExtent(strDontUse);
	wxSize szTooltip;

	szTooltip.SetWidth(sztip.GetWidth() + 10);
	szTooltip.SetHeight(sztip.GetHeight() + 5);

	m_pKeyInputToolTip->SetTooltipText(strDontUse);
	m_pKeyInputToolTip->SetThemeEnabled(true);
	m_pKeyInputToolTip->SetPosition(wxPoint(iPosX1 + 30, iPosY1 + iPosY2));
	m_pKeyInputToolTip->SetSize(szTooltip);
	m_pKeyInputToolTip->Show(true);
}

void CNextMNCDView::OnMouseLBClick(wxMouseEvent& event)
{
	wxPoint pt = event.GetPosition();
	if(FindMouseClickItem(pt))
		theUtility->RefreshWindow(this, m_viewRect);

	event.Skip();
}

bool CNextMNCDView::FindMouseClickItem(const wxPoint& pt)
{
	bool bRet = false;
	std::vector<DirNode *>::const_iterator cIt = m_itStartNode;
	while(cIt != m_itEndDispNode)
	{
		if( (*cIt)->_rc.Contains(pt))
		{
			m_itIndex = theNCD->Begin() + (*cIt)->_index;
			bRet = true;
			break;
		}

		++cIt;
	}

	return bRet;
}

void CNextMNCDView::OnMouseLBDBClick(wxMouseEvent& event)
{
	event.Skip();
	wxPoint pt = event.GetPosition();
	if (FindMouseClickItem(pt))
		ProcessKeyEvent(WXK_RETURN);
}

void CNextMNCDView::OnMouseWheel(wxMouseEvent& event)
{
	wxEventType evtType;
	int iWheelRotation = event.GetWheelRotation();

	if (iWheelRotation < 0)
	{
	//	evtType = wxEVT_SCROLLWIN_LINEDOWN;
		evtType = wxEVT_SCROLLWIN_PAGEDOWN;
		ScrollVerticalProcess(evtType, true);
	}
	else
	{
	//	evtType = wxEVT_SCROLLWIN_LINEUP;
		evtType = wxEVT_SCROLLWIN_PAGEUP;
		ScrollVerticalProcess(evtType, true);
	}

	this->SetScrollPos(wxVERTICAL, (*m_itStartNode)->_row);
	theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMNCDView::AddDirectory()
{
	Freeze();

	DirNode* pNode = (*m_itIndex);

	wxString strFullPath(pNode->_fullName);

	DlgAddDir dlgDir(this);
	dlgDir.SetCurrentPath(strFullPath);
	int iRet = dlgDir.ShowModal();

	wxString strNewPath = dlgDir.GetNewPath();
	dlgDir.Destroy();

	if (iRet == wxID_CANCEL)
	{
		Thaw();
		return;
	}

	std::vector<wxString> vecSplit = theUtility->Split(strNewPath, SLASH);
	size_t splitSize = vecSplit.size();

	wxString strNewName = *(vecSplit.end() - 1);
	int iDepth = splitSize - 1;

	theNCD->AddNode(strNewName, pNode, iDepth);
	theNCD->SetReOrder(strNewPath);

	LoadDirInfo();
	Thaw();

	theUtility->RefreshWindow(this, m_viewRect);
}

void CNextMNCDView::DeleteDirectory()
{
	Freeze();

	bool bGoTrash = true;
	if (wxIsShiftDown())
		bGoTrash = false;

	wxString strMsg(wxT(""));
	DirNode* pNode = (*m_itIndex);
	if(pNode->_parent == nullptr)
	{
		strMsg = wxT("The top-level directory is a drive and cannot be deleted.");
		theUtility->ShowMessageBox(strMsg, wxICON_INFORMATION | wxOK);

		Thaw();
		return;
	}

	wxString strFullPath(pNode->_fullName);
	if (!wxDirExists(strFullPath))
	{
		strMsg = strFullPath + wxT(" is not exist directory(or file) so that can't will be deleted.");
		theUtility->ShowMessageBox(strMsg, wxICON_ERROR | wxOK);

		Thaw();
		return;
	}

	strMsg = wxT("If you delete ") + strFullPath + wxT(", all subdirectorys and files in that path are deleted \n\nAre you sure you want to delete it anyway?");
	int iRet = wxMessageBox(strMsg, PROGRAM_FULL_NAME, wxICON_QUESTION | wxYES_NO);
	if (iRet == wxNO)
	{
		Thaw();
		return;
	}

	std::list<wxString> lst;
	lst.push_back(strFullPath);

	DlgDelete dlgDelete(_gMainFrame);
	dlgDelete.SetDeleteInfo(lst, bGoTrash);
	iRet = dlgDelete.ShowModal();
	dlgDelete.Destroy();
	if (iRet == wxID_CANCEL)
		return;

	theNCD->DeleteNode(pNode);
	LoadDirInfo();

	Thaw();
}

void CNextMNCDView::AddDir()
{
	ProcessKeyEvent(WXK_F7);
}

void CNextMNCDView::RenameDir()
{
	ProcessKeyEvent(WXK_F2);
}

void CNextMNCDView::DeleteDir()
{
	ProcessKeyEvent(WXK_DELETE);
}

void CNextMNCDView::NextFind()
{
	FindMatch(wxT("Ctrl-N"));
}
