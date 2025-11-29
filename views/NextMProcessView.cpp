#include "../ginc.h"
#include "../system/Sysinc.h"
#include "NextMProcessView.h"
#include "NextMProcessDetailView.h"
#include "../dialog/DlgProcessDetail.h"

#define CONTEXT_MENU_KILL_PROCESS        PROCESSVIEW_CONTEXT_MENU_START
#define CONTEXT_MENU_VIEW_PROCESS_DETAIL PROCESSVIEW_CONTEXT_MENU_START + 1

wxBEGIN_EVENT_TABLE(CNextMProcessView, wxListCtrl)
	EVT_MENU_RANGE(PROCESSVIEW_CONTEXT_MENU_START, PROCESSVIEW_CONTEXT_MENU_END, CNextMProcessView::OnListContextMenu)
	EVT_SET_FOCUS(CNextMProcessView::OnSetFocus)
	EVT_KILL_FOCUS(CNextMProcessView::OnKillFocus)
	EVT_KEY_DOWN(CNextMProcessView::OnKeyDown)
	EVT_LEFT_DCLICK(CNextMProcessView::OnMouseLButtonDBClick)

wxEND_EVENT_TABLE()

CNextMProcessView::CNextMProcessView(wxWindow* _parent, long style)
	: wxListCtrl(_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style)
	, m_colBack(240, 240, 240)
{
	this->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( CNextMProcessView::OnListItemRightClick ), NULL, this );
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CNextMProcessView::OnTimer));
	this->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(CNextMProcessView::OnItemSelected), NULL, this);
	m_timer.SetOwner(this, wxTIMER_ID++);

#ifdef __WXMSW__
	ListView_SetImageList(GetHWND(), _gImageList, LVSIL_SMALL);
#else
	this->SetImageList(_gImageList);
#endif

	this->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	m_bitmapKillProcess = wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU, wxSize(16, 16));
	m_bitmapDetail = wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU, wxSize(16, 16));

	wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Fixedsys"));
	this->SetFont(font);

	SetBackgroundColour(m_colBack);
	SetTextColour(wxColour(60, 60, 60));

	CreateContextMenu();

	InitColumn();
	InitList();

	m_timer.Start(1200);

	m_detailViewWidth = 150;
	m_pViewDetail = nullptr;
	m_pViewDetail = new CNextMProcessDetailView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);//wxPoint(ptLeftTop.x + 3, ptScreen.y + 15), wxSize(rcThis.GetWidth() - 10, m_detailViewWidth);//detailW));
	m_pViewDetail->Show(false);
}

CNextMProcessView::~CNextMProcessView()
{
	m_bEnded = true;
	m_timer.Stop();

	if(m_pContextMenu)
	{
		delete m_pContextMenu;
		m_pContextMenu = nullptr;
	}

	this->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( CNextMProcessView::OnListItemRightClick ), NULL, this );
	this->Disconnect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(CNextMProcessView::OnItemSelected), NULL, this);

	if(m_pViewDetail)
	{
		m_pViewDetail->Stop();
		m_pViewDetail->Show(false);
		m_pViewDetail->Clear();

		delete m_pViewDetail;

		m_pViewDetail = nullptr;
	}
}

void CNextMProcessView::CreateContextMenu()
{
	m_pContextMenu = new wxMenu();

	CONTEXT_MENU cMenu[] = {
		{ CONTEXT_MENU_KILL_PROCESS,        wxT("kill process") },
		{ CONTEXT_MENU_VIEW_PROCESS_DETAIL, wxT("View Detail")  },
	};

	int iMenuCount = WXSIZEOF(cMenu);
	for(int i = 0; i < iMenuCount; i++)
	{
		wxMenuItem* pItem = m_pContextMenu->Append(cMenu[i].iId, cMenu[i].strMenuName);

		if(cMenu[i].iId == CONTEXT_MENU_KILL_PROCESS)
			pItem->SetBitmap(m_bitmapKillProcess);

		if(cMenu[i].iId == CONTEXT_MENU_VIEW_PROCESS_DETAIL)
			pItem->SetBitmap(m_bitmapDetail);

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &CNextMProcessView::OnListContextMenu, this, cMenu[i].iId);
	}
}

void CNextMProcessView::InitColumn()
{
	sListColumnItem lstColumn[] = {
		{wxT("Name")            , 250, wxLIST_FORMAT_LEFT},
		{wxT("PID")             , 70,  wxLIST_FORMAT_RIGHT},
		{wxT("User")            , 150, wxLIST_FORMAT_RIGHT},
		{wxT("MemB")            , 70, wxLIST_FORMAT_RIGHT},
		{wxT("Cpu%")            , 70 , wxLIST_FORMAT_RIGHT},
		{wxT("Path")            , 600, wxLIST_FORMAT_LEFT},
		{wxT("File Description"), 270, wxLIST_FORMAT_LEFT},
		{wxT("Company Name")    , 200, wxLIST_FORMAT_LEFT},
		{wxT("Threads")         , 70 , wxLIST_FORMAT_RIGHT},
		{wxT("Command")         , 640, wxLIST_FORMAT_LEFT},
//		{wxT("WorkingSet")      , 140, wxLIST_FORMAT_RIGHT},
//		{wxT("FullPath")        , 540, wxLIST_FORMAT_LEFT},
	};

	for (unsigned int i = 0; i < WXSIZEOF(lstColumn); i++)
	{
		wxListItem lstItemInfo;
		lstItemInfo.SetText(lstColumn[i]._strColumnName);
		lstItemInfo.SetAlign(lstColumn[i]._align);
		lstItemInfo.SetWidth(lstColumn[i]._width);

		this->InsertColumn(i, lstItemInfo);
	}
}

void CNextMProcessView::InitList()
{
	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator iter    = theSystem->PROCESS()->Begin();
	std::unordered_map<unsigned long, Proc::PROCESS_INFO *>::const_iterator iterEnd = theSystem->PROCESS()->End();

	wxString strPrivateSize(wxT(""));
	int iIndex = 0;

	while(iter != iterEnd)
	{
		Proc::PROCESS_INFO* pProcessInfo = iter->second;
		strPrivateSize = theUtility->floating_humanizer(pProcessInfo->_PrivateSize, true, true);

		InsertItem(iIndex, pProcessInfo->_strProcessName);
		SetItem(iIndex, 1, wxString::Format(wxT("%u"), pProcessInfo->_ulProcessID));
		SetItem(iIndex, 2, pProcessInfo->_strUserName);
		SetItem(iIndex, 3, strPrivateSize);
		SetItem(iIndex, 4, wxString::Format(wxT("%.2f"), pProcessInfo->fCPUUsage));
		SetItem(iIndex, 5, pProcessInfo->_strProcessFullPath);
		SetItem(iIndex, 6, pProcessInfo->_strFileDescription);
		SetItem(iIndex, 7, pProcessInfo->_strCompanyName);
		SetItem(iIndex, 8, wxString::Format(wxT("%d"), pProcessInfo->_threadsCount));
		SetItem(iIndex, 9, pProcessInfo->_strCommandLine);

		this->SetItemIcon(iIndex, pProcessInfo->iIconIndex);
		this->SetItemData(iIndex, iIndex);

		iIndex++;
		++iter;
	}

	this->SortItems(CNextMProcessView::ListCompareFunction, (LPARAM)this);
	m_bSorted = true;
}

void CNextMProcessView::SetItemIcon(int iItemIndex, int iIconIndex)
{
	SetItemImage(iItemIndex, iIconIndex);
}

int wxCALLBACK CNextMProcessView::ListCompareFunction(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
	CNextMProcessView* pListCtrl = (CNextMProcessView *)sortData;
	if(pListCtrl == nullptr)
	{
		if (item1 < item2)
			return 1;

		if (item1 > item2)
			return -1;

		return 0;
	}

	int iSortingColumn = pListCtrl->GetSortingColumn();
	bool bAscending = pListCtrl->IsAscending();

	wxString str1 = pListCtrl->GetItemText(item1, iSortingColumn);
	wxString str2 = pListCtrl->GetItemText(item2, iSortingColumn);

	int iRet = 0;
	if(str1.CmpNoCase(str2) > 0)
	{
		if(bAscending)
			iRet = 1;
		else
			iRet = -1;
	}

	if(str1.CmpNoCase(str2) < 0)
	{
		if(bAscending)
			iRet = -1;
		else
			iRet = 1;
	}

	return iRet;
}

void CNextMProcessView::OnTimer(wxTimerEvent& event)
{
	wxCriticalSectionLocker lock(m_critSection);
	wxString strCPUUsage(wxT(""));
	wxString strPrivateSize(wxT(""));
	wxString strWorkingSet(wxT(""));

	unsigned long _ulForegroundProcessID = 0;
#ifdef __WXMSW__
	HWND hForeWindow = ::GetForegroundWindow();
	if(hForeWindow != NULL)
	{
		TCHAR tzText[MAX_PATH] = {0, };
		::GetWindowText(hForeWindow, tzText, MAX_PATH);
		::GetWindowThreadProcessId(hForeWindow, &_ulForegroundProcessID);
	}
#else
#endif

	int _processListCount = this->GetItemCount();
	int iSelectedItemIndex = -1;
	int iSystemIdleIndex = -1;

//	theSystem->PROCESS()->InitKernalTime();

	for(int index = 0; index < _processListCount; index++)
	{
		this->SetItemBackgroundColour(index, m_colBack);

		if(m_bNewOrDelProcess)
		{
			m_bNewOrDelProcess = false;
			break;
		}

		wxString strProcessID = this->GetItemText(index, 1);
		unsigned long ulProcessID = 0;

		strProcessID.ToCULong(&ulProcessID);

		if(_ulForegroundProcessID == ulProcessID)
			this->SetItemBackgroundColour(index, wxColour(255, 255, 0));

		strCPUUsage = wxT("0.00");
		strPrivateSize = wxT("0 KB");
		strWorkingSet = wxT("0 KB");

		float fCPUUsage = 0.0f;

		Proc::PROCESS_INFO* pProcessInfo = theSystem->PROCESS()->UpdateProcessInfo(ulProcessID);
		if(pProcessInfo)
		{
			fCPUUsage = pProcessInfo->fCPUUsage;
			strCPUUsage = wxString::Format(wxT("%.2f"), fCPUUsage);

			strPrivateSize = theUtility->floating_humanizer(pProcessInfo->_PrivateSize, true, true);

			this->SetItem(index, 3, strPrivateSize);
			this->SetItem(index, 4, strCPUUsage);

			if(fCPUUsage >= 30.0f)
			{
				int red = (255 * (int)fCPUUsage) / 100;
				int green = red == 0 ? 255 : 255 - red;

				wxColour backcol{wxColour(red, green, 0)};
				this->SetItemBackgroundColour(index, backcol);
			}
		}
	}
}

void CNextMProcessView::AddAndDelProcessEvent(const wxString& type, const wxString& name, long pid, const wxString& cmdline)
{
	if(m_bEnded)
		return;

	if(m_critSection.TryEnter())
	{
		wxCriticalSectionLocker lock(m_critSection);
		m_bNewOrDelProcess = true;

		this->m_bAscending = true;
		this->m_iSortingColumn = 0;

		if(type.CmpNoCase("Created") == 0)
		{
			Proc::PROCESS_INFO* pProcessInfo = theSystem->PROCESS()->AddProcess(pid)->GetInfo(pid);

			if(pProcessInfo)
			{
				pProcessInfo->_strCommandLine = cmdline;
				int iLstCnt = this->GetItemCount();

				wxString strPrivateSize = theUtility->floating_humanizer(pProcessInfo->_PrivateSize, true, true);

				InsertItem(iLstCnt, pProcessInfo->_strProcessName);
				SetItem(iLstCnt, 1, wxString::Format(wxT("%u"), pProcessInfo->_ulProcessID));
				SetItem(iLstCnt, 2, pProcessInfo->_strDomainAndUser);
				SetItem(iLstCnt, 3, strPrivateSize);
				SetItem(iLstCnt, 4, wxString::Format(wxT("%.2f"), pProcessInfo->fCPUUsage));
				SetItem(iLstCnt, 5, pProcessInfo->_strProcessFullPath);
				SetItem(iLstCnt, 6, pProcessInfo->_strFileDescription);
				SetItem(iLstCnt, 7, pProcessInfo->_strCompanyName);
				SetItem(iLstCnt, 8, wxString::Format(wxT("%d"), pProcessInfo->_threadsCount));
				SetItem(iLstCnt, 9, pProcessInfo->_strCommandLine);

				SetItemIcon(iLstCnt, pProcessInfo->iIconIndex);

				iLstCnt = this->GetItemCount();
				for(int index = 0 ; index < iLstCnt; index++)
					this->SetItemData(index, index);

				this->SortItems(CNextMProcessView::ListCompareFunction, (LPARAM)this);
			}
		}
		else if(type.CmpNoCase("Deleted") == 0)
		{
			int iLstCnt = this->GetItemCount();

			wxString strProcessID(wxT(""));
			int iDelItem = -1;
			for (int i = 0; i < iLstCnt; i++)
			{
				strProcessID = this->GetItemText(i, 1);
				unsigned long _ulProcessID = 0;

				strProcessID.ToCULong(&_ulProcessID);
				if((unsigned long)pid == _ulProcessID)
				{
					iDelItem = i;
					break;
				}
			}

			if(iDelItem != -1)
			{
				theSystem->PROCESS()->DeleteProcess((unsigned long)pid);
				this->DeleteItem(iDelItem);
			}
		}
	}
}

void CNextMProcessView::OnListItemRightClick( wxListEvent& event )
{
	if(m_pContextMenu == nullptr)
		return;

	m_iSelItemRightClick = event.GetIndex();
	wxPoint pt = event.GetPoint();

	this->PopupMenu(m_pContextMenu, pt.x, pt.y);
}

void CNextMProcessView::OnListContextMenu(wxCommandEvent& event)
{
	int iColumnCount = GetColumnCount();

	int iID = event.GetId();

	wxString strProcessID = this->GetItemText(m_iSelItemRightClick, 1);
	unsigned long ulProcessID = 0;
	strProcessID.ToULong(&ulProcessID);

	if(iID == CONTEXT_MENU_KILL_PROCESS)
	{
		wxString strMsgProcessKill(wxT(""));

		strMsgProcessKill = wxT("프로세스명 : ");
		strMsgProcessKill += this->GetItemText(m_iSelItemRightClick, 0);  //프로세스명
		strMsgProcessKill += wxT("\n");
		strMsgProcessKill += wxT("ProcessID : ");

		strMsgProcessKill += strProcessID;  //PID
		strMsgProcessKill += wxT("\n");
		strMsgProcessKill += wxT("File Description : ");
		strMsgProcessKill += this->GetItemText(m_iSelItemRightClick, 6);  //설명
		strMsgProcessKill += wxT("\n");
		strMsgProcessKill += wxT("Company Name : ");
		strMsgProcessKill += this->GetItemText(m_iSelItemRightClick, 7);  //회사명
		strMsgProcessKill += wxT("\n");
		strMsgProcessKill += wxT("FullPath : ");
		strMsgProcessKill += this->GetItemText(m_iSelItemRightClick, 8);  //패스
		strMsgProcessKill += wxT("\n");
		strMsgProcessKill += wxT("\n위 프로세스를 정말로 삭제하시겠습니까?");

		int iRetID = wxMessageBox(strMsgProcessKill, PROGRAM_FULL_NAME, wxICON_QUESTION | wxYES_NO);
		if(iRetID == wxYES)
		{
			int iRet = theSystem->PROCESS()->KillProcess(ulProcessID);
			if(iRet != KILL_PROCESS_MSG_SUCCESS)
			{
				wxString strErrMsg(wxT(""));
				switch(iRet)
				{
					case KILL_PROCESS_MSG_NOT_PROCESSID:
						strErrMsg = wxT("프로세스 ID가 존재하지 않습니다.");
						break;

					case KILL_PROCESS_MSG_PROCESS_NULL:
						strErrMsg = wxT("프로세스가 존재 않습니다.");
						break;

					case KILL_PROCESS_MSG_PROCESS_TERMINATE_FAIL:
						strErrMsg = wxT("프로세스 종료에 실패하였습니다.");
						break;

					case KILL_PROCESS_MSG_PROCESS_ALIVE:
						strErrMsg = wxT("프로세스가 아직 살아 있습니다.");
						break;

					default:
						break;
				}

				wxMessageBox(strErrMsg, PROGRAM_FULL_NAME, wxICON_INFORMATION | wxOK);
				return;
			}
		}
	}
	else
	{
		DlgProcessDetail dlg(this);
		dlg.SetProcessID(ulProcessID);

		dlg.ShowModal();
	}
}

void CNextMProcessView::OnItemSelected(wxListEvent& event)
{
	m_clickPoint = event.GetPoint();

	m_iSelIndex = event.GetIndex();
	SetItemFocus(m_iSelIndex);
}

void CNextMProcessView::SetItemFocus(int _itemIndex)
{
	if(_itemIndex < 0)
		return;

	wxString strProcessID = GetItemText(_itemIndex, 1);

	strProcessID.ToCULong(&m_ulSelProcessID);

	SetItemState(_itemIndex, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
//	SetItemState(_itemIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	EnsureVisible(_itemIndex);
//	SetItemState(iSel, 0, wxLIST_STATE_SELECTED);
}

void CNextMProcessView::OnSetFocus(wxFocusEvent& event)
{
	if(!m_bFocus)
	{
		SetItemFocus(m_iSelIndex);
		m_bFocus = true;
	}
}

void CNextMProcessView::OnKillFocus(wxFocusEvent& event)
{
	m_bFocus = false;
}

void CNextMProcessView::OnKeyDown(wxKeyEvent& event)
{
	int iKeyCode = event.GetKeyCode();
	if (iKeyCode == WXK_ESCAPE)
	{
		if(m_pViewDetail)
		{
			m_pViewDetail->Stop();
			m_pViewDetail->Show(false);
			m_pViewDetail->Clear();

			m_bSelected = false;
		}
	}
}

void CNextMProcessView::OnMouseLButtonDBClick(wxMouseEvent& event)
{
	if(m_iSelIndex < 0)
		return;

	m_bSelected = true;

	wxPoint pt = event.GetPosition();
	wxRect rcThis = this->GetClientRect();

	int left   = rcThis.GetLeft();
	int top    = rcThis.GetTop();
	int right  = rcThis.GetRight();
	int bottom = rcThis.GetBottom();

	wxPoint ptScreen      = ClientToScreen(pt);
	wxPoint ptLeftTop     = ClientToScreen(wxPoint(left, top));
	wxPoint ptRightBottom = ClientToScreen(wxPoint(right, bottom));

	wxString strProcessID = this->GetItemText(m_iSelIndex, 1);

//	int detailW = 150;
//	m_pViewDetail = nullptr;
//	m_pViewDetail = new CNextMProcessDetailView(this, wxID_ANY, wxPoint(ptLeftTop.x + 3, ptScreen.y + 15), wxSize(rcThis.GetWidth() - 10, detailW));
	m_pViewDetail->SetProcessId(strProcessID);
	m_pViewDetail->Show();

	m_pViewDetail->SetPosition(wxPoint(ptLeftTop.x + 3, ptScreen.y + 15));
	m_pViewDetail->SetSize(wxSize(rcThis.GetWidth() - 10, m_detailViewWidth));

	wxPoint ptDetail = m_pViewDetail->GetPosition();

	int yPos = ptDetail.y;

	if((yPos + m_detailViewWidth /* detailW */) > ptRightBottom.y)
	{
		wxPoint ptNew{ptLeftTop.x, ptScreen.y - 180};
		m_pViewDetail->Move(ptNew);
	}

	m_pViewDetail->Start(1100);
}
