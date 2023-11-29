/**************************************************************************************************
* Class명 : CMSWContextMenuHandler
* 설   명 : 파일리스트뷰의 Mouse R Click Up 이벤트 호출시 화면에 컨텍스트 메뉴를 보여줌
*		    윈도우의 경우는 System Shell Context메뉴를 보여줌
*			윈도우가 아닌경우는 자체 ContextMenu를 보여줌
* 참   고 : http://www.codeproject.com/
***************************************************************************************************/
#include "../../ginc.h"
#include "MSWContextMenuHandler.h"
#include "../../views/NextMViewManager.h"

#define CONTEXT_MENU_ID_START 1
#define CONTEXT_MENU_ID_LAST 0x7FFF

IContextMenu2 * g_IContext2 = nullptr;
IContextMenu3 * g_IContext3 = nullptr;

#if defined(_WIN64)
	#define GWL_PROC GWLP_WNDPROC
#else
	#define GWL_PROC GWL_WNDPROC
#endif

WNDPROC OldWndProc = nullptr;

LRESULT CALLBACK HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MENUCHAR:	// only supported by IContextMenu3
			if (g_IContext3)
			{
				LRESULT lResult = 0;
				g_IContext3->HandleMenuMsg2(message, wParam, lParam, &lResult);
				return (lResult);
			}

			break;

		case WM_DRAWITEM:
		case WM_MEASUREITEM:
			if (wParam)
				break; // if wParam != 0 then the message is not menu-related

		case WM_INITMENUPOPUP:
			if (g_IContext2)
				g_IContext2->HandleMenuMsg(message, wParam, lParam);
			else	// version 3
			{
				LRESULT lResult = 0;
				g_IContext3->HandleMenuMsg2(message, wParam, lParam, &lResult);
			//	return (lResult);
			}

			return (message == WM_INITMENUPOPUP ? 0 : TRUE); // inform caller that we handled WM_INITPOPUPMENU by ourself
			break;

		case WM_UNINITMENUPOPUP:	//wxWidgets의 경우 컨택스트 메뉴의 서브메뉴가 보여지고 난후 다른 메류를 선택할때 에러가 발생함
			return 1;			    //이유는 WM_UNINITMENUPOPUP메세지를 wxApp가 처리하게 될경우 문제 발생 => WM_UNINITMENUPOPUP 메세지의 경우는 true를 리턴함
			break;

		default:
			break;
	}

	// call original WndProc of window to prevent undefined bevhaviour of window
	return ::CallWindowProc(OldWndProc, hWnd, message, wParam, lParam);
}

wxBEGIN_EVENT_TABLE(CMSWContextMenuHandler, wxEvtHandler)
    EVT_MENU_RANGE(CONTEXT_MENU_ID_START, CONTEXT_MENU_ID_LAST, CMSWContextMenuHandler::OnInvokeCommand)
wxEND_EVENT_TABLE()

CMSWContextMenuHandler::CMSWContextMenuHandler()
    : m_psfFolder(NULL)
	, m_pContextMenu(NULL)
	, m_iItem(0)
{
	m_psfFolder = nullptr;
	m_pMenu = nullptr;
}

CMSWContextMenuHandler::~CMSWContextMenuHandler()
{
	Clear();
}

void CMSWContextMenuHandler::Clear()
{
	if (m_psfFolder)
		m_psfFolder->Release();

	m_psfFolder = NULL;

	if (m_pMenu)
		delete m_pMenu;

	m_pContextMenu = NULL;
	m_pMenu = NULL;

	m_iItem = 0;
	m_items.clear();
}

void CMSWContextMenuHandler::SetObject(wxArrayString& strArray)
{
	m_iItem = strArray.GetCount();

	std::vector<LPITEMIDLIST> ids;

	for (size_t i = 0; i < strArray.size(); i++)
	{
		std::replace(strArray[i].begin(), strArray[i].end(), '/', '\\');
		ids.push_back(0);

		HRESULT result = SHParseDisplayName(strArray[i].c_str(), 0, &ids.back(), 0, 0);
		if (!SUCCEEDED(result) || !ids.back())
		{
			DWORD dwErrCode = GetLastError();
			ids.pop_back();
			continue;
		}

		m_items.push_back(0);
		result = SHBindToParent(ids.back(), __uuidof(IShellFolder), (void**)&m_psfFolder, &m_items.back());
		if (!SUCCEEDED(result) || !m_items.back())
			m_items.pop_back();

		else if (i < strArray.size() - 1 && m_psfFolder)
		{
			m_psfFolder->Release();
			m_psfFolder = nullptr;
		}
	}
}

bool CMSWContextMenuHandler::GetContextMenu(void ** ppContextMenu, int& iMenuType)
{
	*ppContextMenu = NULL;
	LPCONTEXTMENU icm1 = NULL;

	m_psfFolder->GetUIObjectOf(nullptr, UINT(m_iItem),  (const ITEMIDLIST **)m_items.data(), __uuidof(IContextMenu), 0, reinterpret_cast<void**>(&icm1));

	if (icm1)
	{	// IContextMenu 인터페이스보다 높은 버전에 대한 ContextMenu 핸들을 가져옴
		if (icm1->QueryInterface(__uuidof(IContextMenu3), ppContextMenu) == NOERROR)
			iMenuType = 3;
		else if (icm1->QueryInterface(__uuidof(IContextMenu2), ppContextMenu) == NOERROR)
			iMenuType = 2;

		if (*ppContextMenu)
			icm1->Release();
		else
		{
			iMenuType = 1;
			*ppContextMenu = icm1;	//IContextMenu 인터페이스보다 높은버전의 IContextMenu 인터페이스가 없을경우
		}
	}
	else
		return false;

	return true;
}

void CMSWContextMenuHandler::ShowContextMenu(wxWindow* pWnd, const wxPoint& pt)
{
	int iMenuType = 0;	//to know which version of IContextMenu is supported
	if(m_psfFolder == nullptr)
	{
		theUtility->ShowMessageBox(theMsg->GetMessage(wxT("MSG_CONTEXTMENU_ERROR")), wxOK | wxICON_ERROR);
		return;
	}

	if (!GetContextMenu((void**)&m_pContextMenu, iMenuType))
		return;

	if (!m_pMenu)
	{
		m_pMenu = NULL;
		m_pMenu = new wxMenu();
		m_pMenu->SetEventHandler(this);
	}

	//컨텍스트 메뉴 가져오기
	m_pContextMenu->QueryContextMenu(m_pMenu->GetHMenu(),
		0,
		CONTEXT_MENU_ID_START,
		CONTEXT_MENU_ID_LAST,
		CMF_NORMAL | CMF_EXPLORE);

	if (iMenuType > 1)	// only subclass if its version 2 or 3
	{
		if (iMenuType == 2)
			g_IContext2 = (LPCONTEXTMENU2)m_pContextMenu;
		else	// version 3
			g_IContext3 = (LPCONTEXTMENU3)m_pContextMenu;


#if defined(_WIN64)
		OldWndProc = (WNDPROC)SetWindowLongPtr(pWnd->GetHWND(), GWL_PROC, (DWORD_PTR)HookWndProc);
#else
		OldWndProc = (WNDPROC)SetWindowLong(pWnd->GetHWND(), GWL_PROC, (DWORD)HookWndProc);
#endif
	}
	else
		OldWndProc = NULL;

	//메뉴표시
	pWnd->PopupMenu(m_pMenu, pt.x, pt.y);

	if (OldWndProc)
#if defined(_WIN64)
		SetWindowLongPtr(pWnd->GetHWND(), GWL_PROC, (DWORD_PTR)OldWndProc);
#else
		SetWindowLong(pWnd->GetHWND(), GWL_PROC, (DWORD)OldWndProc);
#endif

	if (iMenuType == 2)
		g_IContext2->Release();
	else
		g_IContext3->Release();

	g_IContext2 = NULL;
	g_IContext3 = NULL;
}

/*
 * 컨텍스트 메뉴선택
 *    - 윈도우    : OnInvokeCommand를 윈도우쉘이 처리
 *    - wxWidgets : 메뉴에 대한 처리를 OnInvokeCommand 이벤트를 등록하여 처리
 */
void CMSWContextMenuHandler::OnInvokeCommand(wxCommandEvent& event)
{
	int iId = event.GetId();
	int iCommand = iId - CONTEXT_MENU_ID_START;

	if((iId >= CONTEXT_MENU_ID_START) && (iId <= CONTEXT_MENU_ID_LAST))
	{
		CMINVOKECOMMANDINFO info = {0};
		info.lpParameters = NULL;
		info.lpDirectory = NULL;
		info.cbSize = sizeof(info);
		info.lpVerb = wx_reinterpret_cast(LPSTR, iCommand);// (LPSTR)(iCommand);
		info.nShow = SW_SHOWNORMAL;
		m_pContextMenu->InvokeCommand(&info);

		CFileListView* pListView = theSplitterManager->GetCurrentViewManager()->GetFileListView();
		pListView->SelectedItemsClear();
		pListView->SetFocus();
	}
}
