#include "../ginc.h"
#include "../system/Sysinc.h"
#include "NextMProcessView.h"

#define CONTEXT_MENU_KILL_PROCESS        PROCESSVIEW_CONTEXT_MENU_START
#define CONTEXT_MENU_VIEW_PROCESS_DETAIL PROCESSVIEW_CONTEXT_MENU_START + 1

#ifdef __WXMSW__
#include "../system/msw/EventSink.h"
void WMIWorker(CNextMProcessView* frame, std::atomic<bool>& keepRunning)
{
	HRESULT hr;

    // Initialize COM for this thread
    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        wxTheApp->CallAfter([frame]() {
            wxMessageBox(wxT("CoInitializeEx failed for WMI thread"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR, frame);
        });
        return;
    }

    // Set general COM security
//    hr = CoInitializeSecurity(
//        NULL,
//        -1, // COM negotiates service
//        NULL, // Authentication services
//        NULL,
//        RPC_C_AUTHN_LEVEL_DEFAULT, // default authentication
//        RPC_C_IMP_LEVEL_IMPERSONATE, // default Impersonation
//        NULL,
//        EOAC_NONE,
//        NULL
//    );
//
//    if (FAILED(hr))
//    {
//        wxTheApp->CallAfter([frame]() {
//            wxMessageBox("CoInitializeSecurity failed", "Error", wxOK | wxICON_ERROR);
//        });
//        CoUninitialize();
//        return;
//    }

    IWbemLocator* pLoc = NULL;
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hr) || !pLoc)
    {
        wxTheApp->CallAfter([frame]() { wxMessageBox(wxT("Failed to create IWbemLocator"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR, frame); });

        CoUninitialize();
        return;
    }

    IWbemServices* pSvc = NULL;
    // Connect to the root\cimv2 namespace with current user.
    hr = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        0,//NULL,
        0,
        0,
        &pSvc
    );

    if (FAILED(hr) || !pSvc)
    {
        wxTheApp->CallAfter([frame]() {
					wxMessageBox(wxT("Could not connect to WMI namespace ROOT\\CIMV2.\nTry running as administrator."),
								PROGRAM_FULL_NAME + " - WMI Connect Failed",
								wxOK | wxICON_ERROR,
								frame);
		});
        pLoc->Release();
        CoUninitialize();
        return;
    }

    // Set security levels on the proxy
    hr = CoSetProxyBlanket(
        pSvc, // the proxy to set
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );
    if (FAILED(hr))
    {
        wxTheApp->CallAfter([frame]() { wxMessageBox(wxT("CoSetProxyBlanket failed"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR, frame); });
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    CEventSink* sink = new CEventSink((wxWindow *)frame);
	sink->AddRef();

    // Query for process creation
    BSTR wql = SysAllocString(L"WQL");
    BSTR creationQuery = SysAllocString(L"SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'");
    hr = pSvc->ExecNotificationQueryAsync(
        wql, //bstr_t("WQL"),
        creationQuery,
        WBEM_FLAG_SEND_STATUS,
        NULL,
        sink);

    SysFreeString(creationQuery);

    // Query for process deletion
    BSTR deletionQuery = SysAllocString(L"SELECT * FROM __InstanceDeletionEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'");
    hr = pSvc->ExecNotificationQueryAsync(
        wql,//bstr_t("WQL"),
        deletionQuery,
        WBEM_FLAG_SEND_STATUS,
        NULL,
        sink);

    SysFreeString(deletionQuery);
    SysFreeString(wql);

    // Keep thread alive until stop requested
    while (keepRunning.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // Cancel async queries by calling CancelAsyncCall
    pSvc->CancelAsyncCall(sink);

    // Cleanup
    sink->Release();
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();
}
#endif // __WXMSW__

wxBEGIN_EVENT_TABLE(CNextMProcessView, wxListCtrl)
	EVT_MENU_RANGE(PROCESSVIEW_CONTEXT_MENU_START, PROCESSVIEW_CONTEXT_MENU_END, CNextMProcessView::OnListContextMenu)
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

	m_keepRunning.store(true);
#ifdef __WXMSW__
	m_thread = std::thread(WMIWorker, this, std::ref(m_keepRunning));
#else

#endif
}

CNextMProcessView::~CNextMProcessView()
{
	m_timer.Stop();

	if(m_pContextMenu)
	{
		delete m_pContextMenu;
		m_pContextMenu = nullptr;
	}

	m_keepRunning.store(false);
	if (m_thread.joinable()) m_thread.join();

	this->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( CNextMProcessView::OnListItemRightClick ), NULL, this );
	this->Disconnect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(CNextMProcessView::OnItemSelected), NULL, this);
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
		{wxT("ProcessName")     , 250, wxLIST_FORMAT_LEFT},
		{wxT("PID")             , 70 , wxLIST_FORMAT_RIGHT},
		{wxT("UserName")        , 230, wxLIST_FORMAT_RIGHT},
		{wxT("CPU")             , 70 , wxLIST_FORMAT_RIGHT},
		{wxT("Private KBytes")  , 140, wxLIST_FORMAT_RIGHT},
		{wxT("WorkingSet")      , 140, wxLIST_FORMAT_RIGHT},
		{wxT("File Description"), 270, wxLIST_FORMAT_LEFT},
		{wxT("Company Name")    , 180, wxLIST_FORMAT_LEFT},
		{wxT("FullPath")        , 540, wxLIST_FORMAT_LEFT},
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
	std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator iter    = theSystem->PROCESS()->Begin();
	std::unordered_map<unsigned long, PROCESS_INFO *>::const_iterator iterEnd = theSystem->PROCESS()->End();

	wxString strSize(wxT(""));
	int iIndex = 0;
	while(iter != iterEnd)
	{
		/*std::pair<unsigned long, PROCESS_INFO *> item = *vecIterS;*/
		PROCESS_INFO* pProcessInfo = iter->second;

		InsertItem(iIndex, pProcessInfo->_strProcessName);
		SetItem(iIndex, 1, wxString::Format(wxT("%u"), pProcessInfo->_ulProcessID));
		SetItem(iIndex, 2, pProcessInfo->_strDomainAndUser);
		SetItem(iIndex, 3, wxT("0.00%"));
		SetItem(iIndex, 4, wxT("0 KB"));
		SetItem(iIndex, 5, wxT("0 KB"));
		SetItem(iIndex, 6, pProcessInfo->_strFileDescription);
		SetItem(iIndex, 7, pProcessInfo->_strCompanyName);
		SetItem(iIndex, 8, pProcessInfo->_strProcessFullPath);

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
		{
			SetItemFocus(index);
			this->SetItemBackgroundColour(index, wxColour(255, 255, 0));
		}

		strCPUUsage = wxT("0.00");
		strPrivateSize = wxT("0 KB");
		strWorkingSet = wxT("0 KB");

		float fCPUUsage = 0.0f;

		PROCESS_INFO* pProcessInfo = theSystem->PROCESS()->UpdateProcessInfo(ulProcessID);
		if(pProcessInfo)
		{
			fCPUUsage = pProcessInfo->fCPUUsage;
			strCPUUsage = wxString::Format(wxT("%.2f"), fCPUUsage);

			strPrivateSize = theUtility->SetComma(wxString::Format(wxT("%.0f"), (float)pProcessInfo->_PrivateSize / (1024.0f)));

			strWorkingSet = theUtility->SetComma(wxString::Format(wxT("%.0f"), (float)pProcessInfo->_WorkingSetSize / (1024.0f)));

			this->SetItem(index, 3, strCPUUsage    + wxT("%"));
			this->SetItem(index, 4, strPrivateSize + wxT(" KB"));
			this->SetItem(index, 5, strWorkingSet  + wxT(" KB"));

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
	wxCriticalSectionLocker lock(m_critSection);
	m_bNewOrDelProcess = true;

	this->m_bAscending = true;
	this->m_iSortingColumn = 0;

	if(type.CmpNoCase("Created") == 0)
	{
		theSystem->PROCESS()->AddProcessInfo(pid, name);
		PROCESS_INFO* pProcessInfo = theSystem->PROCESS()->GetInfo(pid);

		if(pProcessInfo)
		{
			pProcessInfo->_strCommandLine = cmdline;
			int iLstCnt = this->GetItemCount();

			this->InsertItem(iLstCnt, pProcessInfo->_strProcessName);
			this->SetItem(iLstCnt, 1, wxString::Format(wxT("%u"), pProcessInfo->_ulProcessID));
			this->SetItem(iLstCnt, 2, pProcessInfo->_strDomainAndUser);
			this->SetItem(iLstCnt, 3, wxT("0.0%"));
			this->SetItem(iLstCnt, 4, wxT("0 KB"));
			this->SetItem(iLstCnt, 5, wxT("0 KB"));
			this->SetItem(iLstCnt, 6, pProcessInfo->_strFileDescription);
			this->SetItem(iLstCnt, 7, pProcessInfo->_strCompanyName);
			this->SetItem(iLstCnt, 8, pProcessInfo->_strProcessFullPath);

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
}

void CNextMProcessView::OnItemSelected(wxListEvent& event)
{
	int iSel = event.GetIndex();
	SetItemFocus(iSel);
}

void CNextMProcessView::SetItemFocus(int _itemIndex)
{
	if(m_iSelIndex == _itemIndex)
		return;

	m_iSelIndex = _itemIndex;
	wxString strProcessID = GetItemText(_itemIndex, 1);

	strProcessID.ToCULong(&m_ulSelProcessID);

	SetItemState(_itemIndex, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
//	SetItemState(_itemIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	EnsureVisible(_itemIndex);
//	SetItemState(iSel, 0, wxLIST_STATE_SELECTED);
}

