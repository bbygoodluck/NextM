#include "../ginc.h"
#include "FileListView.h"

#ifdef __WXMSW__
	#include "../utility/msw/MSWContextMenuHandler.h"
	#include "../interface/msw/NextMDragSourceHelper.h"
	#include "../interface/msw/NextMDropSource.h"

	#include "../interface/msw/NextMDataObject.h"
//	#include "../interface/msw/NextMEnumFormatEtc.h"
#else
#endif

wxBEGIN_EVENT_TABLE(CFileListView, wxWindow)
	EVT_CHAR_HOOK(CFileListView::OnCharHook)
    EVT_CHAR(CFileListView::OnChar)
    EVT_SET_FOCUS(CFileListView::OnSetFocus)
	EVT_KILL_FOCUS(CFileListView::OnKillFocus)
    EVT_PAINT(CFileListView::OnPaint)
	EVT_ERASE_BACKGROUND(CFileListView::OnErase)
	EVT_SIZE(CFileListView::OnSize)
	EVT_LEFT_DOWN(CFileListView::OnMouseLBottonDown)
	EVT_LEFT_UP(CFileListView::OnMouseLBottonUp)
	EVT_LEFT_DCLICK(CFileListView::OnMouseLButtonDBClick)
	EVT_RIGHT_DOWN(CFileListView::OnMouseRButtonDown)
	EVT_RIGHT_UP(CFileListView::OnMouseRButtonUp)
	EVT_MOTION(CFileListView::OnMouseMove)
	EVT_MOUSEWHEEL(CFileListView::OnMouseWheel)

	//사용자 정의 이벤트
	EVT_MY_CUSTOM_COMMAND(wxEVT_SHOW_CONTEXTMENU, wxID_ANY, CFileListView::OnShowContextMenu)
	EVT_MY_CUSTOM_COMMAND(wxEVT_ITEM_RENAME, wxID_ANY, CFileListView::OnItemRename)
	EVT_MY_CUSTOM_COMMAND(wxEVT_FILE_EDIT, wxID_ANY, CFileListView::OnFileEdit)
	EVT_MENU_RANGE(EXTERNAL_PROGRAM_START_ID, EXTERNAL_PROGRAM_END_ID, CFileListView::OnExecuteExternalProgram)
	EVT_MY_CUSTOM_COMMAND(wxEVT_EXEC_MENU_OPERATION, wxID_ANY, CFileListView::OnPathMenuOperation)
	EVT_MY_CUSTOM_COMMAND(wxEVT_CHANGE_VIEW_COLUMN, wxID_ANY, CFileListView::OnChangeViewColumn)
	EVT_MY_CUSTOM_COMMAND(wxEVT_CHANGE_VIEW_SORT, wxID_ANY, CFileListView::OnChangeSorting)
	EVT_MY_CUSTOM_COMMAND(wxEVT_VIEW_FAVORITE_FROM_STATUS, wxID_ANY, CFileListView::OnShowFavoriteFromStatus)
	EVT_MY_CUSTOM_COMMAND(wxEVT_VIEW_DIR_NUM, wxID_ANY, CFileListView::OnShowDirectoryNumber)

	//압축메뉴 보기
//	EVT_MY_CUSTOM_COMMAND(wxEVT_SHOW_COMPRESS_MENU, wxID_ANY, CFileListView::OnShowCompressMenu)
	//압축메뉴 실행
	EVT_MENU_RANGE(COMPRESS_START_ID, COMPRESS_END_ID, CFileListView::OnCompressMenuExecute)
	EVT_UPDATE_UI_RANGE(COMPRESS_START_ID, DECOMPRESS_END_ID, CFileListView::OnCompressMenuUpdate)
	EVT_MENU_RANGE(DECOMPRESS_START_ID, DECOMPRESS_END_ID, CFileListView::OnDeCompressMenuExecute)
//	EVT_UPDATE_UI_RANGE(DECOMPRESS_START_ID, DECOMPRESS_END_ID, CFileListView::OnDeCompressMenuUpdate)
wxEND_EVENT_TABLE()

CFileListView::CFileListView(wxWindow* parent, const int nID, const wxSize& sz)
    : wxWindow(parent, nID, wxDefaultPosition, sz, FILELISTVIEW_STYLE)
{
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	//상위폴더 이동 이미지
	m_icoUpDir = wxArtProvider::GetIcon(wxART_GO_DIR_UP, wxART_OTHER, wxSize(16, 16));

	m_itemList.reserve(500);
	m_ptList.reserve(100);
	m_posList.reserve(100);
	m_matchItems.reserve(100);
	m_vecVisitDir.reserve(500);

	m_viewFont = *_gViewFont;

//	m_pImageList = std::make_unique<CImageList>();
 //   m_pImageList->Attach(_gImageList);

    m_colDefault    = theColor->GetDefaultColor();
	m_colDrive      = theColor->GetDriveColor();
	m_colBackground = theColor->GetBackgroundColor();
	m_colDir        = theColor->GetDirColor();
	m_colSize       = theColor->GetDefaultColor();
	m_colTime       = theColor->GetDefaultColor();
	m_colAttr       = theColor->GetAttributeColor();
	m_colType       = theColor->GetDescriptionColor();
	m_colMatch      = theColor->GetMatchColor();
	m_colColumn     = theColor->GetColumnColor();

	m_pSelectedItemView = std::make_unique<CNextMTooltipView>(this);
	m_pSelectedItemView->SetSize(60, 20);
	m_pSelectedItemView->Show(false);

	m_pRenameTooltip = std::make_unique<CNextMTooltipView>(this);
	m_pRenameTooltip->SetSize(60, 20);
	m_pRenameTooltip->Show(false);

	m_pKeyInputToolTip = std::make_unique<CNextMTooltipView>(this);
	m_pKeyInputToolTip->SetSize(60, 20);
	m_pKeyInputToolTip->Show(false);

	//이름변경 컨트롤
	m_pTxtCtrlForRename = std::make_unique<wxTextCtrl>(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxBORDER_THEME);
	m_pTxtCtrlForRename->SetBackgroundColour(wxColour(220, 220, 220));

	m_pTxtCtrlForRename->SetFont(m_viewFont);
	m_pTxtCtrlForRename->Show(false);

	m_pTxtCtrlForRename->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CFileListView::OnKeyDownTextCtrl), NULL, this);
	m_pTxtCtrlForRename->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(CFileListView::OnEnterTextCtrl), NULL, this);
	m_pTxtCtrlForRename->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(CFileListView::OnKillFocusTxtCtrl), NULL, this);

#ifdef __WXMSW__
	m_pDropTarget = std::make_unique<CNextMDropTarget>(this->GetHWND());
	m_pDropTarget->SetDropTargetWindow(this);
	m_pDropTarget->AddRef();

	::RegisterDragDrop(this->GetHWND(), m_pDropTarget.get());
#endif // __WXMSW__
}

CFileListView::~CFileListView()
{
	m_bFileImageInfoReadStarted = false;
	StopFileImageRead();
	Clear();

	std::vector<CNextMDirData>().swap(m_itemList);
	std::vector<CColumnPoint>().swap(m_ptList);
	std::vector<CPositionInfo>().swap(m_posList);

	m_pTxtCtrlForRename->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(CFileListView::OnKeyDownTextCtrl), NULL, this);
	m_pTxtCtrlForRename->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(CFileListView::OnEnterTextCtrl), NULL, this);
	m_pTxtCtrlForRename->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(CFileListView::OnKillFocusTxtCtrl), NULL, this);

#ifdef __WXMSW__
	::RevokeDragDrop(this->GetHWND());
#endif
}

void CFileListView::Clear()
{
	m_itemList.clear();
	m_dispNameInfoMap.clear();
	m_hashSelectedItem.clear();

	if(m_itemList.capacity() > 500)
	{
		std::vector<CNextMDirData>().swap(m_itemList);
		m_itemList.reserve(500);
	}
}

void CFileListView::Initialize()
{
	Clear();

	//벡터 용량 재설정
//	int itemCapacity = m_itemList.capacity();
//	if(itemCapacity > 1000)
//		m_itemList.reserve(500);

	//화면 변경 플래그
	//최초수행시는 true 로 설정
	m_bSizeOrColumnChanged = true;

	//디렉토리 Load 플래그
	m_bDirLoaded = false;

	//디렉토리 번호 표시 플래그
	m_bDirectoryNumbering = false;

	//아이템 전체수
	m_iTotalItems = 0;
	//화면표시 시작 인덱스
	m_iStartIndex = 0;
	// 디렉토리수
	m_iDirCount   = 0;
	// 파일수
	m_iFileCount  = 0;
	//선택 파일수
	m_iSelFileCnt = 0;
	//선택 디렉토리 수
	m_iSelDirCnt = 0;

	//현재 인덱스
	m_iCurrentItemIndex = 0;

	//디렉토리내 파일사이즈
	m_dblFileSizeInDir = 0.0f;

	//현재 디렉토리
	m_strCurrentPath = wxT("");
	//Max Name
	m_strMaxName     = wxT("");
	//Max Type Name
	m_strMaxTypeName = wxT("");
	//키 입력값
	m_strKeyInput    = wxT("");

	//선택항목 표시
	m_pSelectedItemView->SetTooltipText(wxT(""));
	m_pSelectedItemView->Show(false);

	//이름변경시
	m_pRenameTooltip->SetTooltipText(wxT(""));
	m_pRenameTooltip->Show(false);

	//키 입력
	m_pKeyInputToolTip->SetTooltipText(wxT(""));
	m_pKeyInputToolTip->Show(false);
}

void CFileListView::OnSetFocus(wxFocusEvent& event)
{
	m_bSetFocus = true;
	theUtility->RefreshWindow(this, m_viewRect);
}

void CFileListView::OnKillFocus(wxFocusEvent& event)
{
	m_bSetFocus = false;
	theUtility->RefreshWindow(this, m_viewRect);
}

void CFileListView::OnErase(wxEraseEvent& event)
{

}

void CFileListView::OnCharHook(wxKeyEvent& event)
{
	int iKeyCode = event.GetKeyCode();
/*
	if(m_bRename || (!m_strKeyInput.IsEmpty() && iKeyCode == 46))
	{
		event.Skip();
		wxPostEvent(m_pTxtCtrlForRename.get(), event);
		return;
	}
*/
	bool bControlDown = wxIsCtrlDown();
	bool bAltDown     = wxIsAltDown();
	bool bShiftDown   = wxIsShiftDown();

	if(bShiftDown)
	{
		if(bAltDown || bControlDown)
		{
			event.Skip();
			return;
		}

		if (theConfig->IsShift_IME_KOR_MoveDrive())
		{
			m_bImeMode = false;
			theUtility->SetImeModeToEnglish(this);
		}
	}

	event.Skip();
}

void CFileListView::OnChar(wxKeyEvent& event)
{
	int iKeyCode = event.GetKeyCode();

	std::unordered_map<int, int>::const_iterator cIterFind = _gmapAllowProcessKey.find(iKeyCode);
	if(cIterFind != _gmapAllowProcessKey.end())
	{
		if(ProcessKeyEvent(iKeyCode))
			return;
	}

	PreTranslateKeyEvent(event);
	event.Skip();
}

void CFileListView::PreTranslateKeyEvent(wxKeyEvent& event)
{
	int iKeyCode = event.GetKeyCode();
	if(iKeyCode == WXK_ESCAPE)
	{
		SelectedItemsClear();
		InitKeyInputTooltip();

		if(m_bDirectoryNumbering)
		{
			wxCommandEvent evt(wxEVT_VIEW_DIR_NUM);
			wxPostEvent(this, evt);
		}

		return;
	}

	bool bControlDown = wxIsCtrlDown();
	bool bAltDown     = wxIsAltDown();
	bool bShiftDown   = wxIsShiftDown();

	wxString strKeyName(theUtility->GetKeyName(event));
	if(strKeyName.CmpNoCase(wxT("UnKnown")) == 0)
		return;

	if (bShiftDown)
	{
		if(bControlDown)
			return;

		if(m_strKeyInput.IsEmpty())
		{
			//Shift + 드라이브명으로 이동
			wxString strVolume = strKeyName.MakeUpper();
			std::vector<CNextMDirData>::iterator itVol = theDriveInfo->GetDriveItem(strVolume);
			//드라이브가 존재하지 않음
			if(itVol == theDriveInfo->EndIter())
				return;

			wxString strDrive = itVol->GetDriveName();
			if(!wxFileName::DirExists(strDrive))
				return;

			LoadDirectory(strDrive);

			theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strDrive, CHANGE_DIR_TAB);
			theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strDrive, CHANGE_DIR_PATHVIEW);

			return;
		}
	}

	if(bAltDown || bControlDown)
		return;

	int iLenKeyInput = 0;
	bool bRefresh = false;

	if(iKeyCode == WXK_BACK)
	{
		if(!m_strKeyInput.IsEmpty())
		{
			iLenKeyInput = m_strKeyInput.Len();
			m_strKeyInput = m_strKeyInput.Left(iLenKeyInput - 1);
		}
	}
	else
	{
		if(m_bDirectoryNumbering && !isdigit(iKeyCode))
			return;

		if(!isdigit(iKeyCode) && !isalpha(iKeyCode) && iKeyCode != WXK_SPACE)
			return;

		m_strKeyInput += strKeyName;
		iLenKeyInput = m_strKeyInput.Len();
	}

	if(iLenKeyInput > 0)
	{
		if(!m_strKeyInput.IsEmpty())
		{
			DoMatchClear();

			wxClientDC dc(this);

			wxSize sztip = dc.GetTextExtent(m_strKeyInput);
			wxSize szTooltip;

			wxRect rcWindow =this->GetClientRect();
			wxPoint ptTooltip(rcWindow.GetRight() / 2 - sztip.GetWidth(), rcWindow.GetHeight() / 2);

			szTooltip.SetWidth(sztip.GetWidth() + 10);
			szTooltip.SetHeight(sztip.GetHeight() + 5);

			m_pKeyInputToolTip->SetTooltipText(m_strKeyInput);
			m_pKeyInputToolTip->SetThemeEnabled(true);
			m_pKeyInputToolTip->SetPosition(ptTooltip);
			m_pKeyInputToolTip->SetSize(szTooltip);
			m_pKeyInputToolTip->Show(true);

			if(!m_bDirectoryNumbering)
				FindMatchItems();

			bRefresh = true;
		}
		else
		{
			InitKeyInputTooltip();
			m_bImeMode = false;

			bRefresh = true;
		}
	}
	else
	{
		m_bImeMode = false;
		InitKeyInputTooltip();
	}

	if(bRefresh)
		theUtility->RefreshWindow(this, m_viewRect);
}

bool CFileListView::ProcessKeyEvent(int iKeyCode)
{
	switch (iKeyCode)
	{
		case WXK_F4:
			ShowBookmark();
			break;

		case WXK_F8:
			ShowCompressPopupMenu();
			break;

		case WXK_REVERSE_SLASH:
			GotoTopDir();
			break;

		case WXK_TAB:
			if(m_bDirectoryNumbering)
				m_bDirectoryNumbering = false;

			theSplitterManager->ChangeSplitView();
			break;

		case WXK_SPACE:
		{
			if(!m_strKeyInput.IsEmpty())
				return false;

			if(!SetSelectedItem())
				return true;

			m_iCurrentItemIndex++;
			CalcRightAndDownKey();
		}
			break;

		case wxKEY_DOT:
		case WXK_BACK:
		{
			if(!m_strKeyInput.IsEmpty())
				return false;

			//최상위 Path
			if(m_strCurrentPath.Right(1).CmpNoCase(SLASH) == 0)
				return true;

			//상위 폴더 이동을 위해서 강제로 ..을 선택하도록 한다.
			m_iCurrentItemIndex = 0;
			if (!ProcessEnterKey(iKeyCode))
				return true;
		}
			break;

		case WXK_RETURN: //Enter key
			if(m_bDirectoryNumbering && !m_strKeyInput.IsEmpty())
			{
				int iDirNum = 0;
				m_strKeyInput.ToInt(&iDirNum);

				if(iDirNum <= 0)
					return false;

				m_iCurrentItemIndex = iDirNum - 1;
			}

			ProcessEnterKey(iKeyCode);
			return true;
			break;

		case WXK_LEFT:
			if (m_iDispColumn > 1)
			{
				m_iCurrentItemIndex -= m_iItemCountInColumn;
				CalcLeftAndUpKey();
			}

			break;

		case WXK_RIGHT:
			if (m_iDispColumn > 1)
			{
				m_iCurrentItemIndex += m_iItemCountInColumn;
				CalcRightAndDownKey();
			}

			break;

		case WXK_UP:
			if (wxGetKeyState(WXK_SHIFT))
			{
				if (!SetSelectedItem())
					return true;
			}

			m_iCurrentItemIndex--;
			CalcLeftAndUpKey();

			break;

		case WXK_DOWN:
			if (wxGetKeyState(WXK_SHIFT))
			{
				if (!SetSelectedItem())
					return true;
			}

			m_iCurrentItemIndex++;
			CalcRightAndDownKey();

			break;

		case WXK_PAGEDOWN:
			CalcPageDown();
			break;

		case WXK_PAGEUP:
			CalcPageUp();
			break;

		case WXK_HOME:
			m_iCurrentItemIndex = 0;
			m_iStartIndex = 0;
			break;

		case WXK_END:
			CalcEnd();
			break;

		case WXK_WINDOWS_KEY:
			return true;
			break;

		default:
			return false;
	}

	theUtility->RefreshWindow(this, m_viewRect);
	return true;
}

bool CFileListView::ProcessEnterKey(int iKeyCode)
{
	std::cout << "ProcessEnterKey" << std::endl;

	std::vector<CNextMDirData>::iterator it = m_itemList.begin() + m_iCurrentItemIndex;
	wxString strName(it->GetName());
	wxString strSearchDir(wxT(""));

	bool bReadDirectory = true;
	if (it->IsDir())
	{
		if( (strName.CmpNoCase(wxT("..")) == 0) || (iKeyCode == wxKEY_DOT) )
		{
			strSearchDir = theUtility->GetParentPath(m_strCurrentPath);
			if (strSearchDir.IsEmpty())
				return false;

			//상위 폴더로의 이동시 현재 디렉토리의 히스토리는 삭제한다.
			_HISTORY::iterator fIter = m_hashHistory.find(m_strCurrentPath);
			if(fIter != m_hashHistory.end())
				m_hashHistory.erase(m_strCurrentPath);
		}
		else
		{
			strSearchDir = m_strCurrentPath + (m_strCurrentPath.Right(1).CmpNoCase(SLASH) == 0 ? strName : SLASH + strName);
			//접근여부 체크
			if (!wxFileName::DirExists(strSearchDir))
				return false;

			//디렉토리 방문 히스토리 저장
			_HISTORY::iterator fIter = m_hashHistory.find(m_strCurrentPath);
			_HistoryItem historyItem;
			if(fIter != m_hashHistory.end())
				historyItem = fIter->second;

			historyItem.iCurrentIndex = m_iCurrentItemIndex;
			historyItem.iStartIndex = m_iStartIndex;

			m_hashHistory[m_strCurrentPath] = historyItem;

			if(m_vecVisitDir.size() == 500)
				m_vecVisitDir.clear();

			m_vecVisitDir.emplace_back(strSearchDir);
			m_iBackFowardIndex = m_vecVisitDir.size() - 1;
		}
	}
	else if(it->IsFile())
	{
		theUtility->LaunchAndExec(it->GetFullPath(), it->GetPath());
		bReadDirectory = false;
	}
	else
	{
		strSearchDir = it->GetDriveName();
		if(!wxFileName::DirExists(strSearchDir))
		{
			theUtility->ShowMessageBox(strSearchDir + wxT(" is not exist"), wxOK | wxICON_INFORMATION);
			return false;
		}
	}

	if(bReadDirectory)
	{
		LoadDirectory(strSearchDir);

		theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strSearchDir, CHANGE_DIR_TAB);
		theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strSearchDir, CHANGE_DIR_PATHVIEW);
	}

	return true;
}

void CFileListView::FindMatchItems()
{
	m_matchItems.clear();

	wxString strKeyInput(m_strKeyInput);
#ifdef __WXMSW__
	strKeyInput.MakeLower();
#endif
	//문자열 포함
	wxString strRegEx(wxT(".*") + strKeyInput);// + wxT(".*"));
	wxRegEx _regex(strRegEx);

	wxString strName;
	for (int iIndex = 0; iIndex < m_iTotalItems; iIndex++)
	{
		std::vector<CNextMDirData>::iterator iter = m_itemList.begin() + iIndex;
		strName = iter->GetName();

		if ((strName == wxT("..")) || iter->IsDrive())
			continue;

#ifdef __WXMSW__
		strName.MakeLower();
#endif
		if (!strKeyInput.IsEmpty())
		{
			if (_regex.Matches(strName))
			{
				iter->SetMatch(true);
				m_matchItems.emplace_back(iIndex);
			}
			else
				iter->SetMatch(false);
		}
		else
			iter->SetMatch(false);
	}

	if (m_matchItems.size() > 0)
	{
		m_iCurrentItemIndex = m_matchItems.at(0);
		ReCalcPage();
	}
}

void CFileListView::DoMatchClear()
{
	std::vector<int>::const_iterator fIter = m_matchItems.begin();
	while(fIter != m_matchItems.end())
	{
		std::vector<CNextMDirData>::iterator iter = m_itemList.begin() + *fIter;
		iter->SetMatch(false);

		fIter++;
	}

	m_matchItems.clear();
}

void CFileListView::InitKeyInputTooltip()
{
	DoMatchClear();

	m_pKeyInputToolTip->SetTooltipText(wxT(""));
	m_pKeyInputToolTip->Show(false);
	m_strKeyInput = wxT("");
}

bool CFileListView::SetSelectedItem()
{
	int iTotalItem = m_itemList.size();
	if(m_iCurrentItemIndex >= iTotalItem - 1)
		return false;

	std::vector<CNextMDirData>::iterator fIter = m_itemList.begin() + m_iCurrentItemIndex;
	if( (theUtility->Compare(fIter->GetName(), wxT("..")) != 0) && !fIter->IsDrive() )
	{
		//선택이 되어 있는 상태면 선택이 안된상태로 변경
		bool bNewSelected = fIter->IsItemSelected() ? false : true;
		fIter->SetItemSelected(bNewSelected);

		if (bNewSelected) //아이템 선택
		{
			SELITEM_INFO _Info;

			_Info.m_iSelIndex = m_iCurrentItemIndex;
			_Info.m_bFile = fIter->IsFile();
			_Info.m_strFullPath = fIter->GetFullPath();

			m_hashSelectedItem.insert(std::make_pair(m_iCurrentItemIndex, _Info));

			if(fIter->IsFile())
				m_iSelFileCnt++;
			else
				m_iSelDirCnt++;
		}
		else
		{
			std::unordered_map<int, SELITEM_INFO>::const_iterator findKey = m_hashSelectedItem.find(m_iCurrentItemIndex);
			if (findKey != m_hashSelectedItem.end())
			{
				SELITEM_INFO _Info = findKey->second;
				if(_Info.m_bFile)
					m_iSelFileCnt--;
				else
					m_iSelDirCnt--;

				m_hashSelectedItem.erase(m_iCurrentItemIndex);
			}
		}

		if(m_hashSelectedItem.size() > 0)
		{
			wxString strSelString(theMsg->GetMessage(wxT("MSG_DETAILINFO_VIEW_SELINFO")));
			wxString strSelItems = strSelString + wxString::Format(theMsg->GetMessage(wxT("MSG_DETAILINFO_VIEW_ITEM_SELECT")), m_iSelDirCnt, m_iSelFileCnt);

			DisplaySelectedItemInfo(strSelItems, 0, 0);
		}
		else
		{
			m_pSelectedItemView->Show(false);
			m_iSelDirCnt = 0;
			m_iSelFileCnt = 0;
		}
	}

	return true;
}

void CFileListView::DisplaySelectedItemInfo(const wxString& strMsg, int xPos, int yPos)
{
	m_pSelectedItemView->Show(false);

	wxClientDC dc(this);
	wxSize sztip = dc.GetTextExtent(strMsg);
	wxSize szTooltip;

	szTooltip.SetWidth(sztip.GetWidth() + 10);
	szTooltip.SetHeight(sztip.GetHeight() + 5);

	xPos = m_viewRectDisp.GetRight() - szTooltip.GetWidth();
	yPos = m_viewRectDisp.GetBottom() - szTooltip.GetHeight();

	m_pSelectedItemView->SetTooltipText(strMsg);
	m_pSelectedItemView->SetThemeEnabled(true);
	m_pSelectedItemView->SetPosition(wxPoint(xPos, yPos));
	m_pSelectedItemView->SetSize(szTooltip);
	m_pSelectedItemView->Show(true);
}

void CFileListView::DisplayRenameTooltip(const wxString& strMsg, int xPos, int yPos)
{
	m_pRenameTooltip->Show(false);

	wxClientDC dc(this);
	wxSize sztip = dc.GetTextExtent(strMsg);
	wxSize szTooltip;

	szTooltip.SetWidth(sztip.GetWidth() + (sztip.GetWidth() * 0.3));
	szTooltip.SetHeight(sztip.GetHeight() + 5);

	m_pRenameTooltip->SetTooltipText(strMsg);
	m_pRenameTooltip->SetThemeEnabled(true);
	m_pRenameTooltip->SetPosition(wxPoint(xPos, yPos));
	m_pRenameTooltip->SetSize(szTooltip);
	m_pRenameTooltip->Show(true);
}

void CFileListView::CalcLeftAndUpKey()
{
	if (!((m_iCurrentItemIndex >= m_iStartIndex) && (m_iCurrentItemIndex <= (m_iStartIndex + m_iDisplayItemInView))))
		m_iStartIndex -= m_iItemCountInColumn;

	if (m_iCurrentItemIndex <= 0)
	{
		m_iCurrentItemIndex = 0;
		m_iStartIndex = 0;
	}
}

void CFileListView::CalcRightAndDownKey()
{
	if ((m_iStartIndex + m_iDisplayItemInView) <= (m_iTotalItems - 1))
	{
		if (m_iCurrentItemIndex > ((m_iStartIndex + m_iDisplayItemInView) - 1))
			m_iStartIndex += m_iItemCountInColumn;
	}

	if (m_iCurrentItemIndex >= (m_iTotalItems - 1))
		m_iCurrentItemIndex = m_iTotalItems - 1;
}

void CFileListView::CalcPageDown()
{
	m_iCurrentItemIndex += m_iDisplayItemInView;

	if ((m_iStartIndex + (m_iItemCountInColumn * m_iDispColumn)) < m_iTotalItems)
		m_iStartIndex += (m_iItemCountInColumn * m_iDispColumn);

	if (m_iCurrentItemIndex >= (m_iTotalItems - 1))
		m_iCurrentItemIndex = m_iTotalItems - 1;
}

void CFileListView::CalcPageUp()
{
	m_iCurrentItemIndex -= (m_iItemCountInColumn * m_iDispColumn);
	m_iStartIndex -= (m_iItemCountInColumn * m_iDispColumn);

	if (m_iStartIndex <= 0)
		m_iStartIndex = 0;

	if (m_iCurrentItemIndex <= 0)
		m_iCurrentItemIndex = 0;
}

void CFileListView::CalcEnd()
{
	if (m_iTotalItems > m_iDisplayItemInView)
	{
		int iMod = m_iTotalItems % m_iDisplayItemInView;
		m_iStartIndex = m_iTotalItems - iMod;

		if (m_iStartIndex <= 0)
			m_iStartIndex = 0;
	}

	m_iCurrentItemIndex = m_iTotalItems - 1;
}

void CFileListView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

    m_viewRect = GetClientRect();
	wxMemoryDC* pMemDC = m_memDC.CreateMemoryDC(&dc, m_viewRect, m_colBackground, m_colBackground);

    pMemDC->SetFont(m_viewFont);
	Render(pMemDC);

	dc.Blit(0, 0, m_viewRect.GetWidth(), m_viewRect.GetHeight(), pMemDC, 0, 0);
}

void CFileListView::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

	m_bSizeOrColumnChanged = true;
	m_dispNameInfoMap.clear();

    theUtility->RefreshWindow(this, m_viewRect);
}

void CFileListView::OnMouseLBottonDown(wxMouseEvent& event)
{
	m_bMouseDown = true;
	DoMouseProcess(event.GetPosition());

	if(!m_bSetFocus)
		theSplitterManager->ChangeSplitView();
}

void CFileListView::OnMouseLBottonUp(wxMouseEvent& event)
{
	m_bMouseDown = false;
}

void CFileListView::OnMouseLButtonDBClick(wxMouseEvent& event)
{
	DoMouseProcess(event.GetPosition(), true);
}

void CFileListView::OnMouseRButtonDown(wxMouseEvent& event)
{
	DoMouseProcess(event.GetPosition());
}

void CFileListView::OnMouseRButtonUp(wxMouseEvent& event)
{
	DisplayContextMenu(event.GetPosition());
}

void CFileListView::OnMouseMove(wxMouseEvent& event)
{
	if(m_bMouseDown && !theDnD->IsDragging())
	{
#ifdef __WXMSW__
		IDataObject *pDataObject;
		IDropSource *pDropSource;

        DWORD        dwEffect;

        FORMATETC fmtetc = { CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        STGMEDIUM stgmed = { TYMED_HGLOBAL   , { 0 }, 0 };

        // 전송할 데이터를 IDataObject로 설정한다.
		HGLOBAL hGlobal = CopySelection();
		if(hGlobal == NULL)
		{
			m_bMouseDown = false;
			return;
		}

        stgmed.hGlobal = CopySelection();
		if (stgmed.hGlobal)
        {
			theDnD->CreateDataObject(&fmtetc, &stgmed, 1, &pDataObject);
			theDnD->CreateDropSource(&pDropSource, this);

			if(!pDropSource)
			{
				m_bMouseDown = false;
				return;
			}

			if(!pDataObject)
			{
				m_bMouseDown = false;
				return;
			}

			((CNextMDropSource *)pDropSource)->SetDataObject(pDataObject);

			CNextMDragSourceHelper dragSrcHelper;
			wxPoint pt;

			pt.x = event.GetPosition().x;
			pt.y = event.GetPosition().y;

			dragSrcHelper.InitializeFromWindow(this->GetHWND(), pt, pDataObject);

		//	dragSrcHelper.InitializeFromBitmap(GetCurrentItem(), pDataObject);

			::DoDragDrop(pDataObject, pDropSource, DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK, &dwEffect);

			pDataObject->Release();
            pDropSource->Release();

            m_bMouseDown = false;
        }
#endif
	}

	event.Skip();
}

void CFileListView::OnMouseWheel(wxMouseEvent& event)
{
	if(!m_bSetFocus)
		return;

	int iWheelRotation = event.GetWheelRotation();
	if (iWheelRotation < 0) //Down
	{
		m_iCurrentItemIndex++;
		if(m_iCurrentItemIndex > (m_iTotalItems - 1))
			m_iCurrentItemIndex = m_iTotalItems - 1;
	}
	else //Up
	{
		m_iCurrentItemIndex--;
		if(m_iCurrentItemIndex < 0)
			m_iCurrentItemIndex = 0;
	}

	theUtility->RefreshWindow(this, m_viewRect);
}

void CFileListView::DoMouseProcess(const wxPoint& pt, bool bDblClick)
{
	m_bMouseClickItemFound = false;
	if (FindItemInMousePoint(pt))
	{
		if (bDblClick)
			ProcessEnterKey(WXK_RETURN);

		m_bMouseClickItemFound = true;
		theUtility->RefreshWindow(this, m_viewRect);
	}
}

bool CFileListView::FindItemInMousePoint(const wxPoint& pt, bool IsMouseMove)
{
	bool bFoundOK = false;
	std::vector<CPositionInfo>::const_iterator cIt = m_posList.begin();

	int iClickPosIndex = 0;
	while(cIt != m_posList.end())
	{
		if(cIt->m_mainRect.Contains(pt))
		{	//아이템 시작인덱스 + 클릭인덱스 < 전체 아이템수
			bFoundOK = (m_iStartIndex + iClickPosIndex) < m_iTotalItems ? true : false;
			break;
		}

		iClickPosIndex++;
		cIt++;
	}

	if(bFoundOK)
	{
		if(!IsMouseMove || theDnD->IsDragging())
			m_iCurrentItemIndex = m_iStartIndex + iClickPosIndex;
	}

	return bFoundOK;
}


void CFileListView::CalcColumn(wxDC* pDC)
{
	if (m_iTotalItems <= 0)
		return;

	if(!m_bSizeOrColumnChanged && !m_bDirLoaded)
		return;

	bool bPosReset = false;
	m_iDispColumn = theConfig->GetColumnCount();
	COLUMN_VIEW_OPTION enumColumnView = _gColumnViewOPEnum[m_iDispColumn];
	if(m_enumColumnViewOption != enumColumnView)
	{
		m_enumColumnViewOption = enumColumnView;
		bPosReset = true;
	}

	if(m_bSizeOrColumnChanged)
		bPosReset = true;

	m_iCharHeight = pDC->GetCharHeight() + 2;

	m_iItemCountInColumn = (m_viewRectDisp.GetHeight() / m_iCharHeight);
	if (m_iItemCountInColumn == 0)
		return;

	if(m_enumColumnViewOption == VIEW_COLUMN_AUTO)
	{
		bPosReset = true;
		//자동컬럼인경우
		if (!CalcAutoColumn(pDC, m_viewRectDisp))
			return;
	}

	//화면안에 표시될 아이템 수
	m_iDisplayItemInView = m_iItemCountInColumn * m_iDispColumn;
	int iWidth = m_viewRectDisp.GetWidth();
	int iBottom = m_viewRectDisp.GetBottom();
	if(bPosReset)
	{
		m_ptList.clear();

		int iDispWidth = static_cast<int>(iWidth / m_iDispColumn);
		if (m_iDispColumn > 1)
		{
			//컬럼구분선 좌표
			wxPoint pt1(0, 0);
			wxPoint pt2(0, 0);
			//컬럼 좌표 설정
			for (int iColumnCount = 0; iColumnCount < (m_iDispColumn - 1); iColumnCount++)
			{
				CColumnPoint colPoint;
				colPoint.pt1.x = iDispWidth * (iColumnCount + 1);
				colPoint.pt1.y = 0;

				colPoint.pt2.x = iDispWidth * (iColumnCount + 1);
				colPoint.pt2.y = iBottom;

				m_ptList.emplace_back(colPoint);
			}
		}

		m_iDispColumnEndPoint = iDispWidth;
	}

	//아이템 표시좌표 계산
	CalcPosition(pDC);
}

bool CFileListView::CalcAutoColumn(wxDC* pDC, const wxRect& viewRect)
{
	//전체건수 / 컬럼당 아이템수 <= 0 이면 컬럼은 1
	m_iDispColumn = m_iTotalItems / m_iItemCountInColumn;
	if (m_iDispColumn <= 0)
		m_iDispColumn = 1;
	else
	{	//컬럼당 표시아이템수 > 전체 아이템수 ==> 컬럼은 1
		if (m_iItemCountInColumn > m_iTotalItems)
			m_iDispColumn = 1;
		else
		{	//표시가능 컬럼수
			int iAvalibleDispItem = 0;

			wxSize szEng = pDC->GetTextExtent(wxString("A"));
			wxSize szHan = pDC->GetTextExtent(wxString("가"));
			//한글과 영문의 평균 Width
			int iAvgCharWidth = (szEng.GetWidth() + szHan.GetWidth()) / 2;
			//최대 표시가능 Width
			int iMaxDispWidth = viewRect.GetWidth();
			//자동컬럼시 표시글자수의 최대값은 16
			int iMaxDispCharWidth = iAvgCharWidth * 16;
			int iDispColumn = 0;

			while (1)
			{	//전체 표시가능 Width > 최대 표시가능 Width
				if (iMaxDispWidth > iMaxDispCharWidth)
				{	//표시컬럼 증가
					iDispColumn++;
					//표시가능 아이템수 증가
					iAvalibleDispItem += m_iItemCountInColumn;
					//전체 표시가능 Width -= 최대표시가능Width
					iMaxDispWidth -= iMaxDispCharWidth;

					if (iAvalibleDispItem >= m_iTotalItems)
						break;
				}
				else
					break;
			}

			//자동 컬럼 선택시 최대 9개의 컬럼
			m_iDispColumn = iDispColumn >= 9 ? 9 : iDispColumn;
		}
	}

	if (m_iDispColumn <= 0)
		m_iDispColumn = 1;

	return true;
}

void CFileListView::CalcPosition(wxDC* pDC)
{
	m_posList.clear();

	m_bDispFlag[0] = false;
	m_bDispFlag[1] = false;
	m_bDispFlag[2] = false;
	m_bDispFlag[3] = false;
	m_bDispFlag[4] = false;
	m_bDispFlag[5] = false;

	wxString strAttr(wxT("RHSA"));
	wxString strTime(wxT("0000-00-00 00:00"));
	wxString strSize(wxT("123.45 MB"));
	wxString strFileSize(wxT("123.45 "));
	wxString strFileSizeType(wxT("MB"));
	wxString strPerms(wxT("rwxrwxrwx"));

	wxSize sizeMaxName     = pDC->GetTextExtent(m_strMaxName);
	wxSize sizeMaxTypeName = pDC->GetTextExtent(m_strMaxTypeName);
	wxSize sizeAttr        = pDC->GetTextExtent(strAttr);
	wxSize sizeTime        = pDC->GetTextExtent(strTime);
	wxSize sizeSize        = pDC->GetTextExtent(strSize);
	wxSize sizeFileSize    = pDC->GetTextExtent(strFileSize);
	wxSize szFileSizeType  = pDC->GetTextExtent(strFileSizeType);
	wxSize szPerms         = pDC->GetTextExtent(strPerms);

	int iNameWidth  = sizeMaxName.GetWidth();
	int iSizeWidth  = sizeSize.GetWidth();
	int iTimeWidth  = sizeTime.GetWidth();
	int iAttrWidth  = sizeAttr.GetWidth();
	int iTypeWidth  = sizeMaxTypeName.GetWidth();
	int iPermsWidth = szPerms.GetWidth();

	unsigned int arrWidth[6] = { 0, };
	arrWidth[0] = iNameWidth;
	arrWidth[1] = iSizeWidth;
	arrWidth[2] = iTimeWidth;
	arrWidth[3] = iAttrWidth;
	arrWidth[4] = iPermsWidth;
	arrWidth[5] = iTypeWidth;

	int iSizeSum = 0;
	int iAvaliableWidth = m_iDispColumnEndPoint - (ICON_SELECTED_WIDTH_GAP + (GAP_WIDTH * 21));

	for (int i = 0; i < 6; i++)
	{
		iSizeSum += arrWidth[i];
		if (iAvaliableWidth > iSizeSum)
			m_bDispFlag[i] = true;
	}

	m_bDispFlag[0] = true;
	if (!m_bDispFlag[1])
		iAvaliableWidth = m_iDispColumnEndPoint - (ICON_SELECTED_WIDTH_GAP + (GAP_WIDTH * 12));

	int iTempColumn = 0;
	int iDispIndex = 0;
	int iStartX = 1;
	int iStartY = m_viewRectDisp.GetTop();

	int iDisp_x1 = 0;
	int iDisp_y1 = 0;
	int iDisp_x2 = 0;
	int iDisp_y2 = 0;

	//2019.04.17 ADD
	int iGapWidth = GAP_WIDTH * 2;

	for (int nIndex = 0; nIndex < m_iDisplayItemInView; nIndex++)
	{
		if (nIndex > (m_iItemCountInColumn - 1))
		{
			if ((nIndex % m_iItemCountInColumn) == 0)
			{
				iTempColumn++;
				iDispIndex = 0;
			}
		}

		int x1 = iStartX + (iTempColumn * m_iDispColumnEndPoint);
		int y1 = (iStartY + 1) + (m_iCharHeight * iDispIndex);
		int x2 = m_iDispColumnEndPoint - 1;
		int y2 = m_iCharHeight;

		wxRect rect(x1, y1, x2, y2);

		CPositionInfo posInfo;
		posInfo.m_mainRect = rect;

		int iIcon_x1 = x1 + ICON_SELECTED_WIDTH_GAP + GAP_WIDTH;
		int iIcon_y1 = y1 + 2;
		int iIcon_x2 = ICON_WIDTH;
		int iIcon_y2 = y2;

		wxRect rcIcon(iIcon_x1, iIcon_y1, iIcon_x2, iIcon_y2);
		posInfo.m_iconRect = rcIcon;

		if (m_bDispFlag[0])
		{
			iDisp_x1 = rcIcon.GetRight() + (GAP_WIDTH * 3);
			iDisp_y1 = y1 + 1;
			iDisp_x2 = m_bDispFlag[1] == true ? iNameWidth : iAvaliableWidth;
			iDisp_y2 = y2;

			wxRect rcName(iDisp_x1, iDisp_y1, iDisp_x2, iDisp_y2);
			posInfo.m_nameRect = rcName;
		}

		//사이즈 표시
		if (m_bDispFlag[1])
		{
			iDisp_x1 = posInfo.m_nameRect.GetRight() + iGapWidth;
			iDisp_y1 = y1 + 1;
			iDisp_x2 = iSizeWidth;
			iDisp_y2 = y2;

			wxRect rcSize(iDisp_x1, iDisp_y1, iDisp_x2, iDisp_y2);
			posInfo.m_sizeRect = rcSize;

			int iFileSizeWidth = sizeFileSize.GetWidth();

			iDisp_x1 = posInfo.m_sizeRect.GetLeft();
			iDisp_y1 = y1 + 1;
			iDisp_x2 = iFileSizeWidth;
			iDisp_y2 = y2;

			wxRect rcFileSize(iDisp_x1, iDisp_y1, iDisp_x2, iDisp_y2);
			posInfo.m_sizeRectFile = rcFileSize;

			int iSizeTypeWidth = szFileSizeType.GetWidth();

			iDisp_x1 = rcFileSize.GetRight() + iGapWidth;
			iDisp_y1 = y1 + 1;
			iDisp_x2 = iSizeTypeWidth;
			iDisp_y2 = y2;

			wxRect rcFileSizeType(iDisp_x1, iDisp_y1, iDisp_x2, iDisp_y2);
			posInfo.m_sizeRectType = rcFileSizeType;
		}

		//수정시간
		if (m_bDispFlag[2])
		{
			iDisp_x1 = posInfo.m_sizeRect.GetRight() + (iGapWidth * 2);
			iDisp_y1 = y1 + 1;
			iDisp_x2 = iTimeWidth;
			iDisp_y2 = y2;

			wxRect rcTime(iDisp_x1, iDisp_y1, iDisp_x2, iDisp_y2);
			posInfo.m_timeRect = rcTime;
		}

		//속성
		if (m_bDispFlag[3])
		{
			iDisp_x1 = posInfo.m_timeRect.GetRight() + (iGapWidth * 2);
			iDisp_y1 = y1 + 1;
			iDisp_x2 = iAttrWidth;
			iDisp_y2 = y2;

			wxRect rcAttr(iDisp_x1, iDisp_y1, iDisp_x2, iDisp_y2);
			posInfo.m_attrRect = rcAttr;
		}

		//Permission
		if (m_bDispFlag[4])
		{
			iDisp_x1 = posInfo.m_attrRect.GetRight() + (iGapWidth * 2);
			iDisp_y1 = y1 + 1;
			iDisp_x2 = iPermsWidth;
			iDisp_y2 = y2;

			wxRect rcPerms(iDisp_x1, iDisp_y1, iDisp_x2, iDisp_y2);
			posInfo.m_permsRect = rcPerms;
		}

		//설명
		if (m_bDispFlag[5])
		{
			iDisp_x1 = posInfo.m_permsRect.GetRight() + (iGapWidth * 2);
			iDisp_y1 = y1;
			iDisp_x2 = iTypeWidth;
			iDisp_y2 = y2;

			wxRect rcType(iDisp_x1, iDisp_y1, iDisp_x2, iDisp_y2);
			posInfo.m_typeNameRect = rcType;
		}

		m_posList.emplace_back(posInfo);
		iDispIndex++;
	}

	m_iTotalPositionCnt = m_posList.size();
}

void CFileListView::DrawColumn(wxDC* pDC)
{
	pDC->SetPen(wxPen(m_colColumn));

	for (auto &colItem : m_ptList)
		pDC->DrawLine(colItem.pt1, colItem.pt2);
}

void CFileListView::DisplayItems(wxDC* pDC)
{
	if (m_iTotalItems <= 0)
		return;

	int iDisplayItemCount = m_iStartIndex + m_iDisplayItemInView;
	if (iDisplayItemCount >= m_iTotalItems)
		iDisplayItemCount = m_iTotalItems;

	if (iDisplayItemCount <= 0)
		return;

	int iPosIndex = 0;
	wxColour dispColor;

	wxString strName(wxT(""));
	wxString strSrcName(wxT(""));
	wxString strFullPathName(wxT(""));

	for (int iIndex = m_iStartIndex; iIndex < iDisplayItemCount; iIndex++)
	{
		if (iIndex >= m_iTotalItems)
			iIndex = m_iTotalItems - 1;

		if (iPosIndex >= m_iTotalPositionCnt)
			iPosIndex = m_iTotalPositionCnt - 1;

		CPositionInfo posInfo = m_posList.at(iPosIndex);

		std::vector<CNextMDirData>::iterator Iter = m_itemList.begin() + iIndex;

		bool isDrive = Iter->IsDrive() ? true : false;
		bool isDir   = Iter->IsDir() ? true : false;
		bool isFile  = Iter->IsFile() ? true : false;
		bool isCut   = Iter->IsCut() ? true : false;

		strName = Iter->GetName();

		bool bSelected = Iter->IsItemSelected();
		bool bMatched = Iter->IsMatch();

		int iIconIndex = 0;
		int iOverlayIconIndex = 0;

		Iter->GetIconIndex(&iIconIndex, &iOverlayIconIndex);

		//표시 색상
		if (isDrive)
		{
			if(Iter->GetDriveType() == wxFS_VOL_DISK)
				strName = strName + wxT("   ") + Iter->GetDriveSpace();

			dispColor = m_colDrive;
		}
		else if (isDir)
			dispColor = m_colDir;
		else
		{
			wxString strExt(Iter->GetExt());
			dispColor = theColor->GetExtColor(strExt.Upper());
		}

		if (bMatched)
			dispColor = m_colMatch;

		if (bSelected)
			dispColor = wxColour(255, 255, 80);

		if (m_iCurrentItemIndex == iIndex)
		{
			DisplayDetailInfo(pDC, *Iter, iIconIndex, iOverlayIconIndex);

			wxRect rcFillRect(posInfo.m_mainRect);
			rcFillRect.SetBottom(rcFillRect.GetBottom() + 1);

			wxPen   pen;
			wxBrush brush;

			if(theDnD->IsDragging())
			{
				if(isDir)
				{
					pen = wxPen(wxColour(255, 255, 0), 1, wxPENSTYLE_DOT);
					brush = wxColour(40, 40, 40);
				}
			}
			else
			{
				//포커스가 없는경우
				if (!m_bSetFocus)
				{
					pen = wxPen(wxColour(30, 30, 30), 1);
					brush = wxColour(30, 30, 30);
				}
				else
				{
					pen = dispColor;
					brush = dispColor;
				}
			}

			pDC->SetPen(pen);
			pDC->SetBrush(brush);

			pDC->DrawRectangle(rcFillRect);

			if(theDnD->IsDragging())
			{
				dispColor = wxColour(128, 128, 150);
			}
			else
			{
				//포커스가 없는경우
				if (!m_bSetFocus)
					dispColor = wxColour(90, 90, 90);
				else
					dispColor = wxColour(0, 0, 0);
			}
		}
		else
		{
			if (bSelected || bMatched)
			{
				wxRect rcFillRect(posInfo.m_mainRect);
				rcFillRect.SetBottom(rcFillRect.GetBottom() + 1);

				wxPen   pen(wxColour(30, 30, 30));
				wxBrush brush(wxColour(19, 102, 142));

				pDC->SetPen(pen);
				pDC->SetBrush(brush);

				pDC->DrawRectangle(rcFillRect);

				pDC->SetPen(wxNullPen);
				pDC->SetBrush(wxNullBrush);
			}
		}

		wxColour colSelected = (m_iCurrentItemIndex == iIndex) ? dispColor : m_colDefault;
		wxString strDisp = CalcStrEllipse(pDC, strName, isDrive);

		pDC->SetTextForeground(dispColor);
		pDC->DrawLabel(strDisp, posInfo.m_nameRect, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

		if(!isDrive)
		{
			if (m_bDispFlag[1])
			{
				if (isDir)
					pDC->DrawLabel(theMsg->GetMessage(wxT("MSG_DIR_FILESIZE_POS")), posInfo.m_sizeRect, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
				else
				{
					wxString strFileSize(Iter->GetSizeString());
					wxString strFileSizeType(Iter->GetSizeTypeString());
					wxColour colorType(Iter->GetSizeColor());
					wxColour colDispColor;

					if (Iter->GetAttribute() & ATTR_RDONLY)
					{
						colDispColor = *wxYELLOW;
						colorType = *wxYELLOW;
					}
					else
						colDispColor = m_colDefault;

					pDC->SetTextForeground(m_iCurrentItemIndex == iIndex ? dispColor : colDispColor);

					if (strFileSizeType.Cmp(_T("")) == 0)	//사이즈가 KB이하인경우
						pDC->DrawLabel(strFileSize, posInfo.m_sizeRect, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
					else
					{	//사이즈가 KB이상인경우
						pDC->DrawLabel(strFileSize, posInfo.m_sizeRectFile, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
						pDC->SetTextForeground(m_iCurrentItemIndex == iIndex ? dispColor : colorType);
						pDC->DrawLabel(strFileSizeType, posInfo.m_sizeRectType, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
					}
				}
			}

			//시간정보
			if (m_bDispFlag[2])
			{
				wxString strTime = Iter->GetDateTimeToString();
				pDC->SetTextForeground(colSelected);
				pDC->DrawLabel(strTime, posInfo.m_timeRect, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
			}

			//속성정보
			if (m_bDispFlag[3])
			{
				wxString strAttr = Iter->GetAttributeToString();
				pDC->SetTextForeground(colSelected);
				pDC->DrawLabel(strAttr, posInfo.m_attrRect, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
			}

			//Permission
			if (m_bDispFlag[4])
			{
				wxString strPerms = Iter->GetPermission();
				pDC->SetTextForeground(colSelected);
				pDC->DrawLabel(strPerms, posInfo.m_permsRect, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
			}

			//type정보
			if (m_bDispFlag[5])
			{
				wxString strDesc = Iter->GetDescription();
				pDC->SetTextForeground(m_iCurrentItemIndex == iIndex ? dispColor : m_colType);
				pDC->DrawLabel(strDesc, posInfo.m_typeNameRect, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
			}
		}

		if (bSelected)
		{	//아이템이 선택되었을때 선택표시를 Polygon으로 처리(▶)
			wxPoint ptSel[3];

			ptSel[0].x = 0;
			ptSel[0].y = 0;

			ptSel[1].x = 0;
			ptSel[1].y = 0;

			ptSel[2].x = 0;
			ptSel[2].y = 0;

			//Polygon 보정
			wxRect rcMain(posInfo.m_mainRect);

			ptSel[0].x = rcMain.GetLeft() + 3;
			ptSel[0].y = rcMain.GetTop() + 3;

			ptSel[1].x = rcMain.GetLeft() + 9;
			ptSel[1].y = rcMain.GetTop() + 9;

			ptSel[2].x = rcMain.GetLeft() + 3;
			ptSel[2].y = rcMain.GetBottom() - 2;

			wxPen penSel(wxColour(225, 113, 0));
			wxBrush brushSel(wxColour(225, 113, 0));

			pDC->SetPen(penSel);
			pDC->SetBrush(brushSel);

			pDC->DrawPolygon(3, ptSel, -1);

			pDC->SetPen(wxNullPen);
			pDC->SetBrush(wxNullBrush);
		}

		if (theUtility->Compare(Iter->GetName(), wxT("..")) == 0)
			pDC->DrawIcon(m_icoUpDir, wxPoint(posInfo.m_iconRect.GetLeft(), posInfo.m_iconRect.GetTop()));
		else
		{
			int iImageFlag = 0;
#ifdef __WXMSW__
			iImageFlag = ILD_NORMAL | ILC_MASK;

			if (isDir || isFile)
			{
				if (isCut)
					iImageFlag |= ILD_BLEND25;

				if (Iter->GetAttribute() & ATTR_HIDDEN)
					iImageFlag |= ILD_BLEND25;
			}
#else
#endif
			int iImageX = posInfo.m_iconRect.GetLeft();
			int iImageY = posInfo.m_iconRect.GetTop();

			DrawItemImage(pDC, iImageX, iImageY, iImageFlag, iIconIndex, iOverlayIconIndex);
		}

		if(m_bDirectoryNumbering && isDir)
		{
			wxString strDirNum = wxString::Format("%d", iIndex + 1);
			int nDirNumX1 = 0;
			int nDirNumY1 = 0;
			int nDirNumX2 = 0;
			int nDirNumY2 = 0;

			nDirNumX1 = posInfo.m_nameRect.GetRight() - ((posInfo.m_nameRect.GetWidth() / 2) + (10 * strDirNum.Len()));
			nDirNumY1 = posInfo.m_nameRect.GetTop();
			nDirNumX2 = posInfo.m_nameRect.GetRight();
			nDirNumY2 = posInfo.m_nameRect.GetBottom();

			wxRect rcDirNum(wxPoint(nDirNumX1, nDirNumY1), wxPoint(nDirNumX2, nDirNumY2));
			wxRect rcDirNumShadow(wxPoint(nDirNumX1 + 1, nDirNumY1 + 1), wxPoint(nDirNumX2 + 1, nDirNumY2 + 1));

			pDC->SetTextForeground(wxColour(128, 128, 128));
			pDC->DrawLabel(strDirNum, rcDirNumShadow, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

			pDC->SetTextForeground(wxColour(189, 224, 244));
			pDC->DrawLabel(strDirNum, rcDirNum, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
		}

		iPosIndex++;
	}
}

void CFileListView::DisplayDirInfo(wxDC* pDC)
{
	wxFont fontNormal;
	wxFont fontBold;

	fontNormal.Create(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Segoe UI"));
	fontBold.Create(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, _T("Segoe UI"));

	wxString strDirCount = wxString::Format(wxT("%d"), m_iDirCount);
	wxString strFileCount = wxString::Format(wxT("%d"), m_iFileCount);
	wxString strDirName(wxT(" ") + theMsg->GetMessage(wxT("MSG_DIR")) + wxT(", "));
	wxString strFileName(wxT(" ") + theMsg->GetMessage(wxT("MSG_FILE_DESCRIPTION")) + wxT(" "));

	int xPos = m_viewDirInfo.GetLeft() + 5;
	int yPos = m_viewDirInfo.GetTop() + 3;

	pDC->SetFont(fontBold);
	pDC->SetTextForeground(wxColour(0, 0, 255));
	pDC->DrawText(strDirCount, xPos, yPos);

	wxSize sizeInfo = pDC->GetTextExtent(strDirCount);
	xPos += sizeInfo.GetWidth();

	pDC->SetFont(fontNormal);
	pDC->SetTextForeground(wxColour(0, 0, 0));
	pDC->DrawText(strDirName, xPos , yPos);

	sizeInfo = pDC->GetTextExtent(strDirName);
	xPos += sizeInfo.GetWidth();

	pDC->SetFont(fontBold);
	pDC->SetTextForeground(wxColour(0, 0, 255));
	pDC->DrawText(strFileCount, xPos, yPos);

	sizeInfo =pDC->GetTextExtent(strFileCount);
	xPos += sizeInfo.GetWidth();

	pDC->SetFont(fontNormal);
	pDC->SetTextForeground(wxColour(0, 0, 0));
	pDC->DrawText(strFileName, xPos, yPos);

	sizeInfo = pDC->GetTextExtent(strFileName);
	xPos += sizeInfo.GetWidth();

	wxString strFileSize(wxT(""));
	wxString strFileSizeType(wxT(""));

	wxColour colorType(wxColour(192, 192, 192));

	theUtility->GetSizeInfo(m_dblFileSizeInDir, strFileSize, strFileSizeType, colorType);
	if (strFileSizeType.IsEmpty())
		strFileSizeType = wxT("Bytes");

	wxString strSizeInfo = wxString::Format(wxT("(%s %s)"), strFileSize, strFileSizeType);
	pDC->SetFont(fontBold);
	pDC->DrawText(strSizeInfo, xPos, yPos);

	pDC->SetFont(fontNormal);
	sizeInfo = pDC->GetTextExtent(m_strDiskSpaceDisp);//wxT("m_strDiskSpaceDisp"));

	xPos = m_viewDirInfo.GetRight() - sizeInfo.GetWidth();
	pDC->DrawText(m_strDiskSpaceDisp, xPos - 5, yPos);

	wxRect rcDiskSpace;
	rcDiskSpace.SetLeft(xPos - 150);
	rcDiskSpace.SetTop(m_viewDirInfo.GetTop() + 2);
	rcDiskSpace.SetRight(xPos - 10);
	rcDiskSpace.SetBottom(m_viewDirInfo.GetBottom() - 2);

	wxPen penDisk(wxColour(100, 100, 100));
	wxBrush brushDisk(wxColour(120, 120, 120));

	pDC->SetPen(penDisk);
	pDC->SetBrush(brushDisk);
	pDC->DrawRectangle(rcDiskSpace);

	double dblTotalWidth = rcDiskSpace.GetWidth() * 1.0;
	double dblUsed = m_dblTotalSpace - m_dblFreeSpace;

	int iUsedWidth = (int)((dblUsed * dblTotalWidth) / m_dblTotalSpace);

	wxPen penUsed(wxColour(100, 100, 100));
	wxBrush brushUsed(wxColour(44, 184, 65));
	wxRect rcDiskSpaceUsed(rcDiskSpace.GetLeft() + 1, rcDiskSpace.GetTop() + 1, iUsedWidth, rcDiskSpace.GetHeight() - 1);

	int iRed = 44 + (211 * iUsedWidth / dblTotalWidth);
	int iGreen = 150 - (105 * iUsedWidth / dblTotalWidth);
	int iBlue = 150 -  (105 * iUsedWidth / dblTotalWidth);

	iRed = iRed > 255 ? 255 : iRed;
	iGreen = iGreen < 0 ? 0 : iGreen;
	iBlue = iBlue < 0 ? 0 : iBlue;

	pDC->GradientFillLinear(rcDiskSpaceUsed, wxColour(44, 184, 65), wxColour(iRed , iGreen , iBlue));

	wxPen penLine(wxColour(255, 255, 255));
	pDC->SetPen(penLine);

	pDC->DrawLine(rcDiskSpace.GetLeft(), rcDiskSpace.GetBottom(), rcDiskSpace.GetRight() + 1, rcDiskSpace.GetBottom());
	pDC->DrawLine(rcDiskSpace.GetRight(), rcDiskSpace.GetTop(), rcDiskSpace.GetRight(), rcDiskSpace.GetBottom());

	wxString strUsed = wxString::Format(wxT("%5.1f GB"), (dblUsed / GIGABYTE));

	pDC->SetTextForeground(wxColour(255, 255, 255));
	pDC->DrawLabel(strUsed, rcDiskSpace, wxALIGN_CENTER);

	pDC->SetFont(m_viewFont);
}

void CFileListView::DrawInfoArea(wxDC* pDC)
{
	wxPen pen(wxColour(0, 0, 0));
	wxBrush brush(wxColour(240, 240, 240));

	pDC->SetPen(pen);
	pDC->SetBrush(brush);

	pDC->DrawRectangle(m_viewDirInfo);

	wxPen penDetail(wxColour(0, 0, 0));
	wxBrush brushDetail(wxColour(192, 192, 192));

	pDC->SetPen(penDetail);
	pDC->SetBrush(brushDetail);

	pDC->DrawRectangle(m_viewRectDetail);

	pDC->SetPen(wxNullPen);
	pDC->SetBrush(wxNullBrush);
}

void CFileListView::DisplayDetailInfo(wxDC* pDC, const CNextMDirData& data, int iIconIndex, int iOverlayIconIndex)
{
	int iImageFlag = 0;
#ifdef __WXMSW__
		iImageFlag = ILD_NORMAL | ILC_MASK;
#endif // __WXMSW__;

	DrawItemImage(pDC, m_viewRectDetail.GetLeft() + 3, m_viewRectDetail.GetTop() + 3, iImageFlag, iIconIndex, iOverlayIconIndex);

	wxString strDetailInfo = data.GetName();
	strDetailInfo.append(wxT("  |  "));
#ifdef __WXMSW__
	if(data.IsDrive())
	{
		CNextMDirData& _driveSpace = const_cast<CNextMDirData&>(data);
		wxString strDriveSpace(_driveSpace.GetDriveSpace());
		strDetailInfo += strDriveSpace;
	}
	else
	{
#endif
		if(data.IsFile())
		{
			strDetailInfo += theUtility->SetComma(data.GetSize().ToString());
			strDetailInfo += wxT(" Bytes");
			strDetailInfo.append(wxT("  |  "));
		}

		wxString strAttr = data.GetAttributeToString();

		strDetailInfo.append(strAttr);
		strDetailInfo.append(wxT("  |  "));


		wxString strTime = data.GetDateTimeToString();
		strDetailInfo.append(strTime);
		strDetailInfo.append(wxT("  |  "));
		strDetailInfo.append(data.GetDescription());
#ifdef __WXMSW__
	}
#endif

	wxFont font;
	font.Create(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Segoe UI"));

	pDC->SetFont(font);
	pDC->SetTextForeground(wxColour(0, 0, 0));
	pDC->DrawText(strDetailInfo, m_viewRectDetail.GetLeft() + 22, m_viewRectDetail.GetTop() + 3);

	pDC->SetFont(m_viewFont);
}

wxString CFileListView::CalcStrEllipse(wxDC* pDC, const wxString& strSrc, bool IsDrive)
{
	wxString strKey(strSrc);

#ifdef __WXMSW__
		strKey.MakeLower();
#endif

	CPositionInfo posInfo = m_posList.at(0);

	wxString strName(strSrc);
	wxString strDisp(wxT(""));

	wxSize szNameSize = pDC->GetTextExtent(strName);

	int iDispWidth = IsDrive ? posInfo.m_mainRect.GetWidth() - 35 : posInfo.m_nameRect.GetWidth();
	int iNameWidth = szNameSize.GetWidth();

	int iLen = strName.Len();

	if (iNameWidth > iDispWidth)
	{
		std::unordered_map<wxString, wxString>::const_iterator fIter = m_dispNameInfoMap.find(strKey);
		if (fIter != m_dispNameInfoMap.end())
			return fIter->second;

		for (int iIndex = 0; iIndex < iLen; iIndex++)
		{
			strDisp = strName.Left(iIndex + 1);
			wxSize sizeText = pDC->GetTextExtent(wxString(strDisp + wxT("...")));

			if ((sizeText.GetWidth()) > iDispWidth)
			{
				strDisp = strName.Left(iIndex);
				break;
			}
		}

		strDisp += wxT("...");
		m_dispNameInfoMap.insert(std::make_pair(strKey, strDisp));
	}
	else
		strDisp = strName;

	return strDisp;
}

void CFileListView::ReCalcPage()
{
	int iStartIndex = 0;
	while (1)
	{
		if ((m_iCurrentItemIndex >= iStartIndex) && (m_iCurrentItemIndex <= (iStartIndex + m_iDisplayItemInView)))
			break;

		iStartIndex += m_iDisplayItemInView;
		if (iStartIndex > m_iTotalItems)
		{
			iStartIndex = m_iTotalItems - m_iDisplayItemInView;
			break;
		}
	}

	m_iStartIndex = iStartIndex;
	if (m_iStartIndex < 0)
		m_iStartIndex = 0;

	if (m_iCurrentItemIndex < 0)
		m_iCurrentItemIndex = 0;
}

void CFileListView::SortStart()
{
	int iSortType = theConfig->GetSortType();
	std::vector<CNextMDirData>::const_iterator iter = m_itemList.begin();
	wxString strName = iter->GetName();
	int iSortStartIndex = 0;

	if(strName.Cmp(wxT("..")) == 0)
		iSortStartIndex = 1;

	switch(iSortType)
	{
		case VIEW_SORT_DEFAULT:
			std::sort(m_itemList.begin() + iSortStartIndex, m_itemList.end()/* - iSortEndIndex */, CSorting::DirSortOfName);
			break;

		case VIEW_SORT_TIME:
			std::sort(m_itemList.begin() + iSortStartIndex, m_itemList.end()/* - iSortEndIndex */, CSorting::DirSortOfTime);
			break;

		case VIEW_SORT_SIZE:
			std::sort(m_itemList.begin() + iSortStartIndex, m_itemList.end()/* - iSortEndIndex */, CSorting::DirSortOfSize);
			break;

		default:
			break;
	}
}

void CFileListView::DrawItemImage(wxDC* pDC, int x, int y, int flags, int nIconIndex, int nOverlayIndex)
{
#ifdef __WXMSW__
	flags |= INDEXTOOVERLAYMASK(nOverlayIndex + 1);
#endif // __WXMSW__

	theImageList->Draw(nIconIndex, pDC, x, y, flags);
}

void CFileListView::GetSelectedItems(std::list<wxString>& lstSrc, bool IsCut)
{
	if(m_hashSelectedItem.size() == 0)
	{
		std::vector<CNextMDirData>::iterator it = m_itemList.begin() + m_iCurrentItemIndex;
		if(it->IsDrive())
		{
			theUtility->ShowMessageBox(theMsg->GetMessage(wxT("MSG_COPY_MOVE_NOT_SUPPORT_DRIVE")), wxOK | wxICON_ERROR);
			return;
		}

		it->SetCut(false);
		if(IsCut)
			it->SetCut(true);

		lstSrc.push_back(it->GetFullPath());
	}
	else
	{
		std::unordered_map<int, SELITEM_INFO>::iterator it = m_hashSelectedItem.begin();
		while(it != m_hashSelectedItem.end())
		{
			SELITEM_INFO _Info = it->second;
			std::vector<CNextMDirData>::iterator itemIt = m_itemList.begin() + _Info.m_iSelIndex;

			itemIt->SetCut(false);
			if(IsCut)
				itemIt->SetCut(true);

			lstSrc.push_back(itemIt->GetFullPath());
			it++;
		}
	}

	if(IsCut)
		theUtility->RefreshWindow(this, m_viewRect);
}

void CFileListView::SelectedItemsClear(bool bDeleted)
{
	if(!bDeleted)
	{
		std::unordered_map<int, SELITEM_INFO>::iterator itSel = m_hashSelectedItem.begin();
		while(itSel != m_hashSelectedItem.end())
		{
			SELITEM_INFO _Info = itSel->second;
			std::vector<CNextMDirData>::iterator itData = m_itemList.begin() + _Info.m_iSelIndex;

			itData->SetItemSelected(false);
			itSel++;
		}

		theUtility->RefreshWindow(this, m_viewRect);
	}

	m_hashSelectedItem.clear();

	m_pSelectedItemView->SetTooltipText(wxT(""));
	m_pSelectedItemView->Show(false);

	m_iSelDirCnt = 0;
	m_iSelFileCnt = 0;
}

void CFileListView::StopFileImageRead()
{
	m_bFileImageInfoReadStarted = false;
	//실행중인 쓰레드가 종료되길 기다린다.
	if(GetThread() && GetThread()->IsRunning())
		GetThread()->Wait();

	//새 쓰레드 생성
	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
    {
        wxLogError("Could not create the worker thread!");
        return;
    }
}

bool CFileListView::RunFileImageInfoRead()
{
	m_bFileImageInfoReadStarted = true;
	if (GetThread()->Run() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Could not run the worker thread!");
		return false;
	}

	return true;
}

wxThread::ExitCode CFileListView::Entry()
{
	int iStartIndex = m_iStartIndex;
	int iPosIndex = 0;

	int iIconIndex = 0;
	int iOverlayIconIndex = 0;

	for(int iIndex = 0; iIndex < m_iTotalItems; iIndex++)
	{
		std::vector<CNextMDirData>::iterator it = m_itemList.begin() + iIndex;

		if(!m_bFileImageInfoReadStarted)
			break;

#ifdef __WXMSW__
		if(it->IsSetImageIcon() || it->IsDrive())
			continue;
#endif // __WXMSW__

		theImageList->GetIconIndex(it->GetFullPath(), iIconIndex, iOverlayIconIndex);
		it->SetIconIndex(iIconIndex, iOverlayIconIndex);
		it->SetImageIconFlag(true);

		if(iStartIndex != m_iStartIndex)
		{
			iPosIndex = m_iCurrentItemIndex % m_iDisplayItemInView;
			iStartIndex = m_iStartIndex;
		}

		int iPositionSize = m_posList.size();
		if((iPositionSize > 0) && (iPosIndex < iPositionSize) )
		{
			std::vector<CPositionInfo>::const_iterator posIterator = m_posList.begin() + iPosIndex;
			theUtility->RefreshWindow(this, posIterator->m_iconRect);

			iPosIndex++;
		}
	}

	if(m_bFileImageInfoReadStarted)
		theUtility->RefreshWindow(this, m_viewRect);

	return (wxThread::ExitCode)0;
}


void CFileListView::OnShowContextMenu(wxCommandEvent& event)
{
	int iItemPosition = m_iCurrentItemIndex % m_iDisplayItemInView;
	CPositionInfo posInfo = m_posList.at(iItemPosition);

	wxPoint pt(posInfo.m_nameRect.GetRight() / 4, posInfo.m_nameRect.GetTop() + ICON_HEIGHT);

	DisplayContextMenu(pt);
}

void CFileListView::DisplayContextMenu(const wxPoint& pt)
{
	int iSelectedItemIndex = m_iCurrentItemIndex;
	int iItemCount = m_itemList.size();

	if (iItemCount < m_iCurrentItemIndex)
		m_iCurrentItemIndex = iSelectedItemIndex;

	wxArrayString arrString;
	int nSelectedItems = m_hashSelectedItem.size();
	if (nSelectedItems != 0)
	{
		std::unordered_map<int, SELITEM_INFO>::const_iterator iter = m_hashSelectedItem.begin();
		while(iter != m_hashSelectedItem.end())
		{
			SELITEM_INFO _ItemInfo = iter->second;
			arrString.Add(_ItemInfo.m_strFullPath);

			iter++;
		}
	}
	else
	{
		wxString strContextPath = m_strCurrentPath;
		std::vector<CNextMDirData>::iterator cit = m_itemList.begin() + m_iCurrentItemIndex;
		if (cit->GetName().Cmp(wxT("..")) == 0)
			return;

#ifdef __WXMSW__
		strContextPath = cit->IsDrive() ? cit->GetDriveName() : cit->GetFullPath();
#else
		strContextPath = cit->GetFullPath();
#endif

		arrString.Add(strContextPath);
	}

	CContextMenuHandler ctxMenu;
	ctxMenu.SetObject(arrString);
	ctxMenu.ShowContextMenu(this, pt);
}

std::vector<CNextMDirData>::iterator CFileListView::FindItem(const wxString& strName)
{
	auto it = std::find_if(m_itemList.begin(), m_itemList.end(), [&strName](const CNextMDirData& data) {
		wxString strTgt(data.GetName());
		wxString strSrc(strName);

		return (strSrc.Cmp(strTgt) == 0);
	});

//	if (it == m_itemList.end())
//		return m_itemList.end();

	return it;
}

void CFileListView::ShowBookmark()
{
	if (theBookmark->GetBookmarkSize() == 0)
	{
		theUtility->ShowMessageBox(wxT("Registered bookmark item is nothing"), wxICON_INFORMATION | wxOK);
		return;
	}

	int iItemPosition = m_iCurrentItemIndex - m_iStartIndex;
	CPositionInfo posInfo = m_posList.at(iItemPosition);

	wxRect rcPos(posInfo.m_nameRect);
	rcPos.SetLeft(posInfo.m_nameRect.GetLeft() + 10);
	rcPos.SetBottom(posInfo.m_nameRect.GetBottom() + 1);

	int iFavoriteRight = posInfo.m_nameRect.GetRight() + 3;
	int iViewRight = m_viewRect.GetRight();

	if (iFavoriteRight >= iViewRight)
		rcPos.SetLeft(rcPos.GetLeft() - 20);

	wxPoint pt(rcPos.GetLeft() , rcPos.GetTop() + rcPos.GetHeight());
	wxMenu menuFavorite;
	theBookmark->CreateBookmarkMenu(&menuFavorite);

	this->PopupMenu(&menuFavorite, pt);
}

void CFileListView::OnItemRename(wxCommandEvent &event)
{
	std::vector<CNextMDirData>::const_iterator fIter = m_itemList.begin() + m_iCurrentItemIndex;
	m_strItemToRename = fIter->GetName();

#ifdef __WXMSW__
	if(fIter->IsDrive())
		return;
#endif // __WXMSW__
	if((theUtility->Compare(wxT(".."), m_strItemToRename) == 0))
		return;

	m_bRename = true;
	RenameOn(m_strItemToRename);
}

void CFileListView::RenameOn(const wxString& strRename)
{
	int iCurrentPosition = m_iCurrentItemIndex % m_iDisplayItemInView;
	CPositionInfo posInfo = m_posList.at(iCurrentPosition);

	int iPosX1 = posInfo.m_mainRect.GetLeft();
	int iPosY1 = posInfo.m_mainRect.GetTop();
	int iPosX2 = posInfo.m_mainRect.GetWidth();
	int iPosY2 = posInfo.m_mainRect.GetHeight() + 1;

	int iLength = strRename.Len();
	wxString strExt = theUtility->GetExt(strRename);
	int iExtLen = strExt.Len();
	if(iExtLen != 0)
	{
		iExtLen += 1;
		iLength -= iExtLen;
	}

	m_pTxtCtrlForRename->SetSize(wxRect(iPosX1, iPosY1, iPosX2, iPosY2));
	m_pTxtCtrlForRename->SetLabelText(strRename);
	m_pTxtCtrlForRename->SetSelection(0, iLength);
	m_pTxtCtrlForRename->Show(true);
	m_pTxtCtrlForRename->SetFocus();

	wxString strDontUse(theMsg->GetMessage(wxT("MSG_INFO_RENAME_DONTUSE")) + theMsg->GetMessage(wxT("MSG_INFO_RENAME_DONTUSE_STRING")));
	DisplayRenameTooltip(strDontUse, iPosX1 + 30, iPosY1 + iPosY2);
}

void CFileListView::RenameStart(const wxString& strOldPathName, const wxString& strNewPathName)
{
	int iResult = wxRename(strOldPathName, strNewPathName);

	if(iResult != 0)
	{
		wxString strMsg = wxT("Could not rename ") + strOldPathName + wxT(" to ") + strNewPathName;
		theUtility->ShowMessageBox(strMsg, wxICON_ERROR | wxOK);
	}

	m_bRename = true;
}

void CFileListView::OnEnterTextCtrl(wxCommandEvent& event)
{
	m_bRename = false;
	wxString strNewRename = m_pTxtCtrlForRename->GetValue();

	wxString strOldPathName = theUtility->MakeFullPathName(m_strCurrentPath, m_strItemToRename);
	wxString strNewPathName = theUtility->MakeFullPathName(m_strCurrentPath, strNewRename);

	if(CLocalFileSystem::IsFileWritePermission(strOldPathName))
	{
		std::vector<CNextMDirData>::iterator fIter = FindItem(strNewRename);

		if (fIter != m_itemList.end())
		{
			wxString strMsg = strNewPathName + theMsg->GetMessage(wxT("MSG_DLG_ADD_DIR_EXIST"));
			theUtility->ShowMessageBox(strMsg, wxOK | wxICON_INFORMATION);
			return;
		}

		if(!CLocalFileSystem::CheckCreatable(strNewRename))
		{
			RenameOn(strNewRename);
			return;
		}

		RenameStart(strOldPathName, strNewPathName);
	}
	else
	{
		wxString strUseAnotherPG(theMsg->GetMessage(wxT("MSG_DLG_DELETE_USE_ANOTHER_PG")));
		wxString strMsg = strOldPathName + strUseAnotherPG;

		theUtility->ShowMessageBox(strMsg, wxICON_ERROR | wxOK);
	}

	m_pTxtCtrlForRename->SetValue(wxT(""));
	m_pTxtCtrlForRename->Show(false);

	m_pRenameTooltip->Show(false);
	event.Skip();
}

void CFileListView::OnKeyDownTextCtrl(wxKeyEvent& event)
{
	int iKeyCode = event.GetKeyCode();
	event.Skip();

	if (iKeyCode == WXK_ESCAPE)
	{
		m_bRename = false;
		m_pTxtCtrlForRename->SetLabelText(wxT(""));
		m_pTxtCtrlForRename->Show(false);

		m_pRenameTooltip->Show(false);
	}
}

void CFileListView::OnKillFocusTxtCtrl(wxFocusEvent& event)
{
	event.Skip();

	m_pTxtCtrlForRename->SetLabelText(wxT(""));
	m_pTxtCtrlForRename->Show(false);

	m_pRenameTooltip->Show(false);
	this->SetFocus();
}

void CFileListView::OnFileEdit(wxCommandEvent &event)
{
	if(theExternal->GetExternalProgCount() == 0)
	{
		theUtility->ShowMessageBox(theMsg->GetMessage(wxT("MSG_FILE_MENU_EDIT_NOT_SET")), wxICON_INFORMATION | wxOK);
		return;
	}

	int iSelCount = m_hashSelectedItem.size();
	if(iSelCount == 0)
	{
		std::vector<CNextMDirData>::const_iterator cit = m_itemList.begin() + m_iCurrentItemIndex;
#ifdef __WXMSW__
		if(cit->IsDir() || cit->IsDrive())
		{
			wxString strMsg = cit->IsDrive() ? wxT("MSG_FILE_MENU_EDIT_NOT_SUPPORT_DRV") : wxT("MSG_FILE_MENU_EDIT_NOT_SUPPORT_DIR");
			theUtility->ShowMessageBox(theMsg->GetMessage(strMsg), wxICON_INFORMATION | wxOK);
			return;
		}
#else
#endif
	}
	else
	{
		bool bIncludedDir = false;
		std::unordered_map<int, SELITEM_INFO>::const_iterator fIter = m_hashSelectedItem.begin();
		while(fIter != m_hashSelectedItem.end())
		{
			SELITEM_INFO _info = fIter->second;
			if(!_info.m_bFile)
			{
				bIncludedDir = true;
				break;
			}

			fIter++;
		}

		if(bIncludedDir)
		{
			theUtility->ShowMessageBox(wxT("The directory is included in the selection"), wxICON_INFORMATION | wxOK);
			return;
		}
	}

	ShowExternalProgramList();
}

void CFileListView::ShowExternalProgramList()
{
	if(theExternal->GetExternalProgCount() == 1)
	{
		ExecuteExternProgram(0);
		return;
	}

//	wxBitmap bmpExternalPG = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER, wxSize(16, 16));

	int iItemPosition = m_iCurrentItemIndex - m_iStartIndex;
	std::vector<CPositionInfo>::const_iterator cItPos = m_posList.begin() + iItemPosition;

	wxPoint pt(cItPos->m_nameRect.GetLeft() + 5, cItPos->m_nameRect.GetTop() + ICON_HEIGHT + 5);

	wxMenu* pMenu = theExternal->GetMenu();
	this->PopupMenu(pMenu, pt);
}

void CFileListView::ExecuteExternProgram(int iIndex)
{
	std::vector<CExternalProgItem>::const_iterator cIt = theExternal->BeginIter() + iIndex;

	wxString strFilePG = cIt->_strPGPath;
	wxString strArgs = cIt->_strPGArgs;
	if (strFilePG == wxT(""))
	{
		theUtility->ShowMessageBox(theMsg->GetMessage(wxT("MSG_FILE_MENU_EDIT_NOT_SET")), wxOK | wxICON_ERROR);
		return;
	}

	if (!wxFileExists(strFilePG))
	{
		wxString strMsg = wxString::Format(theMsg->GetMessage(wxT("MSG_FILE_MENU_EDIT_NOT_EXIST_PROGRAM")), strFilePG);
		theUtility->ShowMessageBox(strMsg, wxOK | wxICON_ERROR);
		return;
	}

	std::unordered_map<int, SELITEM_INFO>::iterator iTer = m_hashSelectedItem.begin();

	std::vector<CNextMDirData>::iterator cItItem;
	if (iTer == m_hashSelectedItem.end())
	{
		cItItem = m_itemList.begin() + m_iCurrentItemIndex;
		wxExecute(strFilePG + wxT(" ") + strArgs + wxT(" ") + wxT("\"") + cItItem->GetFullPath() + wxT("\""));
	}
	else
	{
		while(iTer != m_hashSelectedItem.end())
		{
			SELITEM_INFO _Info = iTer->second;

			cItItem = m_itemList.begin() + _Info.m_iSelIndex;
			wxExecute(strFilePG + wxT(" ") + strArgs + wxT(" ") + wxT("\"") + cItItem->GetFullPath() + wxT("\""));

			iTer++;
		}

		SelectedItemsClear();
		theUtility->RefreshWindow(this, m_viewRect);
	}
}

void CFileListView::OnExecuteExternalProgram(wxCommandEvent& event)
{
	int iID = event.GetId();
	int iIndex = iID - EXTERNAL_PROGRAM_START_ID;

	ExecuteExternProgram(iIndex);
}

void CFileListView::SelectAllItemOrRelease(bool bAllSelect)
{
	m_hashSelectedItem.clear();
	m_iSelDirCnt = 0;
	m_iSelFileCnt = 0;

	std::vector<CNextMDirData>::iterator fIter = m_itemList.begin();
	int iIndex = 0;
	while(fIter != m_itemList.end())
	{
		if(!fIter->IsDrive() && (fIter->GetName().Cmp(wxT("..")) != 0))
		{
			fIter->SetItemSelected(bAllSelect);
			if(bAllSelect)
			{
				if(fIter->IsDir())
					m_iSelDirCnt++;
				else
					m_iSelFileCnt++;

				SELITEM_INFO _Info;
				_Info.m_iSelIndex = iIndex;
				_Info.m_bFile = fIter->IsFile();
				_Info.m_strFullPath = fIter->GetFullPath();

				m_hashSelectedItem.insert(std::make_pair(iIndex, _Info));
			}
		}

		fIter++;
		iIndex++;
	}

	if(bAllSelect)
	{
		wxString strSelString(theMsg->GetMessage(wxT("MSG_DETAILINFO_VIEW_SELINFO")));
		wxString strSelItems = strSelString + wxString::Format(theMsg->GetMessage(wxT("MSG_DETAILINFO_VIEW_ITEM_SELECT")), m_iSelDirCnt, m_iSelFileCnt);

		DisplaySelectedItemInfo(strSelItems, 0, 0);
	}
	else
		m_pSelectedItemView->Show(false);


	theUtility->RefreshWindow(this, m_viewRect);
}


void CFileListView::OnPathMenuOperation(wxCommandEvent& event)
{
	int iId = event.GetId();
	switch(iId)
	{
		case _MENU_PATH_REFRESH:
		{
			wxString strNewPath(m_strCurrentPath);
			LoadDirectory(strNewPath);
		}
			break;

		case _MENU_PATH_GOROOT:
			GotoTopDir();
			break;

		case _MENU_PATH_GOUP:
			ProcessKeyEvent(wxKEY_DOT);
			break;

		case _MENU_PATH_GODOWN:
		{
			std::vector<CNextMDirData>::const_iterator cit = m_itemList.begin() + m_iCurrentItemIndex;
			if(cit->IsFile())
			{
				theUtility->ShowMessageBox(wxT("you can select a directory only") , wxOK | wxICON_INFORMATION);
				return;
			}

			ProcessKeyEvent(WXK_RETURN);
		}
			break;

		case _MENU_PATH_BACK:
		{
			m_iBackFowardIndex--;
			if(m_iBackFowardIndex < 0)
			{
				m_iBackFowardIndex = 0;
				return;
			}

			GotoVisitDirectory();
		}
			break;

		case _MENU_PATH_FOWARD:
		{
			m_iBackFowardIndex++;
			if(m_iBackFowardIndex >= (int)m_vecVisitDir.size())
			{
				m_iBackFowardIndex = (int)(m_vecVisitDir.size() - 1);
				return;
			}

			GotoVisitDirectory();
		}
			break;

		default:
			break;
	}
}

void CFileListView::GotoVisitDirectory()
{
	std::vector<wxString>::const_iterator cit = m_vecVisitDir.begin() + m_iBackFowardIndex;

	wxString strVisitDir = *cit;

	LoadDirectory(strVisitDir);

	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strVisitDir, CHANGE_DIR_TAB);
	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strVisitDir, CHANGE_DIR_PATHVIEW);
}

void CFileListView::OnChangeViewColumn(wxCommandEvent& event)
{
	m_bSizeOrColumnChanged = true;
	theUtility->RefreshWindow(this, m_viewRect);
}

void CFileListView::OnChangeSorting(wxCommandEvent& event)
{
	SortStart();
	theUtility->RefreshWindow(this, m_viewRect);
}

void CFileListView::OnShowFavoriteFromStatus(wxCommandEvent& event)
{
	ShowBookmark();
}

void CFileListView::OnShowDirectoryNumber(wxCommandEvent& event)
{
	m_bDirectoryNumbering = !m_bDirectoryNumbering;
	theUtility->RefreshWindow(this, m_viewRect);
}


//압축메뉴 선택 후..
void CFileListView::OnCompressMenuExecute(wxCommandEvent& event)
{
	std::vector<CNextMDirData>::iterator iter = m_itemList.begin() + m_iCurrentItemIndex;

	int nSelCount = m_hashSelectedItem.size();

	if(nSelCount == 0)
	{
		if (iter->IsDrive())
		{
			wxMessageBox(theMsg->GetMessage(wxT("MSG_INFO_DRIVE_COMPRESS_ERROR")), PROGRAM_FULL_NAME, wxICON_INFORMATION | wxOK);
			return;
		}
	}

	theComDec->SetImmediatelyCompress(true);
	int nId = event.GetId();

	wxCommandEvent evt(wxEVT_COMPRESS_EXEC);
	evt.SetId(nId);

	wxPostEvent(_gMenuEvent, evt);
}

void CFileListView::ShowCompressPopupMenu()
{
	if(wxGetKeyState(WXK_CONTROL))
	{
		if(!IsAvaliableDecompress())
			return;
	}

	wxMenu* pPopupMenu = theComDec->GetPopupMenu( wxGetKeyState(WXK_CONTROL) ? true : false);
	this->PopupMenu(pPopupMenu, GetItemPos());
}

wxPoint CFileListView::GetItemPos()
{
	int iItemPosition = m_iCurrentItemIndex - m_iStartIndex;
	std::vector<CPositionInfo>::const_iterator cItPos = m_posList.begin() + iItemPosition;

	return wxPoint(cItPos->m_nameRect.GetLeft() + 5, cItPos->m_nameRect.GetTop() + ICON_HEIGHT + 5);
}

void CFileListView::OnCompressMenuUpdate(wxUpdateUIEvent& event)
{
	if(event.IsCheckable())
		event.Check(false);
}

bool CFileListView::IsAvaliableDecompress()
{
	std::vector<CNextMDirData>::iterator iter = m_itemList.begin() + m_iCurrentItemIndex;

	wxString strFullPath(iter->GetFullPath());
	wxString strFileName(iter->GetName());
	wxString strTmp(iter->GetName());

	wxString strMsg(wxT(""));
	//디렉토리 인경우
	if(iter->IsDir())
	{
		strMsg = wxString::Format(theMsg->GetMessage(wxT("MSG_DECOMPRESS_DLG_DECOMPRESS_ERR_DIR")), strFileName);
		wxMessageBox(strMsg, PROGRAM_FULL_NAME, wxICON_ERROR | wxOK);
		return false;
	}
	else
	{
		int extIndex = -1;
		if(!theComDec->IsIncludeCompressedFileExt(strFileName, extIndex))
		{
			strMsg = wxString::Format(theMsg->GetMessage(wxT("MSG_DECOMPRESS_DLG_DECOMPRESS_ERR_FILE")), strFileName);
			wxMessageBox(strMsg, PROGRAM_FULL_NAME, wxICON_ERROR | wxOK);
			return false;
		}
	}

	m_strCompressedFile = strFullPath;
	return true;
}

void CFileListView::OnDeCompressMenuExecute(wxCommandEvent& event)
{
	int nId = event.GetId();

	wxCommandEvent evt(wxEVT_COMPRESS_EXEC);

	evt.SetString(m_strCompressedFile);
	evt.SetId(nId);

	wxPostEvent(_gMenuEvent, evt);
}

void CFileListView::SetDnDUpdate()
{
	if(!m_bSetFocus && theDnD->IsDropped())
		theSplitterManager->ChangeSplitView();

	theUtility->RefreshWindow(this, m_viewRectDisp);
}

int CFileListView::DropWindowSelectItemType()
{
	std::vector<CNextMDirData>::iterator iter = m_itemList.begin() + m_iCurrentItemIndex;
	int iType = 0;
	if(iter->IsDir())
		iType = 1;

	return iType;
}

#ifdef __WXMSW__
HGLOBAL CFileListView::CopySelection()
{
	HGLOBAL hMem;
	wxArrayString arrDragFiles;
	UINT uiLen = 0;

	int iSelectedItems = m_hashSelectedItem.size();
	if (iSelectedItems == 0)
	{
		std::vector<CNextMDirData>::iterator itemIt = m_itemList.begin() + m_iCurrentItemIndex;
		if(itemIt->IsDrive())
		{
			wxMessageBox(theMsg->GetMessage(wxT("MSG_COPY_MOVE_NOT_SUPPORT_DRIVE")), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
			return NULL;
		}

		arrDragFiles.Add(itemIt->GetFullPath());
	}
	else
	{
		std::unordered_map<int, SELITEM_INFO>::const_iterator iter;

		for (iter = m_hashSelectedItem.begin(); iter != m_hashSelectedItem.end(); iter++)
		{
			SELITEM_INFO _Info = iter->second;
			std::vector<CNextMDirData>::iterator itemIt = m_itemList.begin() + _Info.m_iSelIndex;

			arrDragFiles.Add(itemIt->GetFullPath());
		}
	}

	if (arrDragFiles.size() == 0)
		return NULL;

	int iItemCount = wx_static_cast(int, arrDragFiles.size());

	wxString strFileLists;
	wxString strDragItem(wxT(""));
	for (int iIndex = 0; iIndex < iItemCount; iIndex++)
	{
		strFileLists = arrDragFiles.Item(iIndex);

#if defined(_UNICODE) || defined(UNICODE)
		uiLen += strFileLists.Len() + 1;
#else
		uiLen += WideCharToMultiByte(CP_ACP, 0, strFileLists, -1, NULL, 0, NULL, NULL);
		uiLen += 1;
#endif
	}

	#if defined(_UNICODE) || defined(UNICODE)
	uiLen = sizeof(DROPFILES) + sizeof(TCHAR) * (uiLen + 1);
#else
	uiLen = sizeof(DROPFILES) + sizeof(char) * (uiLen + 1);
#endif

	hMem = GlobalAlloc(GHND | GMEM_SHARE, uiLen);
	if (!hMem)
		return NULL;

	DROPFILES* dfiles = (DROPFILES *)GlobalLock(hMem);
	if (!dfiles)
	{
		GlobalFree(hMem);
		return NULL;
	}

	dfiles->pFiles = sizeof(DROPFILES);
	GetCursorPos(&(dfiles->pt));
	dfiles->fNC = TRUE;
	dfiles->fWide = FALSE;

#if defined(_UNICODE) || defined(UNICODE)
	dfiles->fWide = TRUE;
	TCHAR* pFile = (TCHAR *)(LPBYTE(dfiles) + sizeof(DROPFILES));
#else
	char* pFile = (char *)(LPBYTE(dfiles) + sizeof(DROPFILES));// (char *)&dfiles[1];
#endif

	for (int iIndex = 0; iIndex < iItemCount; iIndex++)
	{
		wxString strItem = arrDragFiles.Item(iIndex);
		lstrcpy(pFile, CONVSTR(strItem));

#if defined(_UNICODE) || defined(UNICODE)
		pFile = 1 + _tcschr(pFile, '\0');
#else
		pFile = 1 + strchr(pFile, '\0');
#endif
	}

	GlobalUnlock(hMem);
	return hMem;
}

#endif
