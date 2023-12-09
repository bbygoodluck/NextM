#include "../ginc.h"
#include "PathView.h"
#include "NextMViewManager.h"

#include "../engine/LocalFileSystem.h"
#include "../img/path_next.xpm"

wxBEGIN_EVENT_TABLE(CPathView, wxWindow)
	EVT_PAINT(CPathView::OnPaint)
	EVT_SIZE(CPathView::OnSize)
	EVT_ERASE_BACKGROUND(CPathView::OnErase)
	EVT_LEAVE_WINDOW(CPathView::OnLeaveWindow)
	EVT_LEFT_DOWN(CPathView::OnMouseLButtonClick)
	EVT_RIGHT_DOWN(CPathView::OnMouseRButtonClick)
	EVT_LEFT_DCLICK(CPathView::OnMouseDBClick)
	EVT_MOTION(CPathView::OnMouseMove)
#ifdef __WXMSW__
	EVT_MENU_RANGE(wxDRIVE_ID_START, wxDRIVE_ID_END, CPathView::OnDriveMenuClick)
	EVT_UPDATE_UI_RANGE(wxDRIVE_ID_START, wxDRIVE_ID_END, CPathView::OnDrvieMenuUpdate)
#endif

	EVT_MENU_RANGE(wxSUBDIR_START, wxSUBDIR_END, CPathView::OnMoveSubDir)
wxEND_EVENT_TABLE()

CPathView::CPathView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle)
	: wxWindow(parent, nID, wxDefaultPosition, sz, wxBORDER_NONE)
{
	wxBitmap bmpDrive = wxArtProvider::GetBitmap(wxART_FLOPPY, wxART_OTHER, wxSize(16, 16));
	m_icoDrive.CopyFromBitmap(bmpDrive);

	m_bitmapNext = wxBitmap(path_next);//, wxBITMAP_TYPE_XPM);
	m_bitmapNext.SetMask(new wxMask(m_bitmapNext, *wxLIGHT_GREY));

	m_bitmapNextDisable = m_bitmapNext.ConvertToDisabled(230);

	m_pToolTip = new wxToolTip(wxT(""));
	this->SetToolTip(m_pToolTip);

	m_vecPathDatas.reserve(100);
#ifdef __WXMSW__
	m_PathViewFont.Create(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Segoe UI"));
#else
	m_PathViewFont = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
#endif

	m_pTxtCtrl = std::make_unique<wxTextCtrl>(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxBORDER_THEME);
	m_pTxtCtrl->SetBackgroundColour(wxColour(240, 240, 220));
	m_pTxtCtrl->SetBackgroundStyle(wxBG_STYLE_PAINT);
	m_pTxtCtrl->SetFont(m_PathViewFont);
	m_pTxtCtrl->Show(false);

	m_pTxtCtrl->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CPathView::OnKeyDownTextCtrl), NULL, this);
	m_pTxtCtrl->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(CPathView::OnEnterTextCtrl), NULL, this);
	m_pTxtCtrl->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(CPathView::OnKillFocusTxtCtrl), NULL, this);

//	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	Init();
}

CPathView::~CPathView()
{
	ClearPathItems();
	std::vector<CPathItemData>().swap(m_vecPathDatas);

	m_pTxtCtrl->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(CPathView::OnKeyDownTextCtrl), NULL, this);
	m_pTxtCtrl->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(CPathView::OnEnterTextCtrl), NULL, this);
	m_pTxtCtrl->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(CPathView::OnKillFocusTxtCtrl), NULL, this);
}

void CPathView::ClearPathItems()
{
	m_vecPathDatas.clear();
}

void CPathView::Init()
{
	ClearPathItems();

	m_iRED   = 208;
	m_iGREEN = 208;
	m_iBLUE  = 232;

	m_iPrevItemIndex = -1;
}

#ifdef __WXMSW__
void CPathView::AddDriveItem()
{

}
#endif

void CPathView::OnErase(wxEraseEvent& event)
{

}

void CPathView::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

    theUtility->RefreshWindow(this, m_viewRect);
}

void CPathView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

	wxPen pen(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));
	wxBrush brush(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

    m_viewRect = GetClientRect();

	wxMemoryDC* pMemDC = m_memDC.CreateMemoryDC(&dc, m_viewRect, wxSystemSettings::GetColour(wxSYS_COLOUR_MENU), wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	pMemDC->SetFont(m_PathViewFont);
	pMemDC->DrawRectangle(m_viewRect);

	Render(pMemDC);

	dc.Blit(0, 0, m_viewRect.GetWidth(), m_viewRect.GetHeight(), pMemDC, 0, 0);
}

void CPathView::Render(wxDC* pDC)
{

	pDC->DrawRectangle(m_viewRect);

    if(m_bChangedPath)
		CalcPathPolygon(pDC);

	DisplayPathItem(pDC);
    //라인 그리기
	pDC->SetPen(wxPen(*wxBLACK, 1, wxPENSTYLE_SOLID));
	pDC->DrawLine(wxPoint(m_viewRect.GetLeft(), m_viewRect.GetTop()), wxPoint(m_viewRect.GetRight(), m_viewRect.GetTop()));
	pDC->DrawLine(wxPoint(m_viewRect.GetLeft(), m_viewRect.GetTop()), wxPoint(m_viewRect.GetLeft(), m_viewRect.GetBottom()));
	pDC->DrawLine(wxPoint(m_viewRect.GetRight(), m_viewRect.GetTop()), wxPoint(m_viewRect.GetRight(), m_viewRect.GetBottom()));
	pDC->DrawLine(wxPoint(m_viewRect.GetLeft(), m_viewRect.GetBottom()), wxPoint(m_viewRect.GetRight(), m_viewRect.GetBottom()));

	m_bChangedPath = false;
}

void CPathView::CalcPathPolygon(wxDC* pDC)
{
	m_vecPathDatas.clear();

	wxStringTokenizer tokenizer(m_strPath, SLASH);
	int iTokenCount = tokenizer.CountTokens();
	if (iTokenCount > 0)
	{
		wxString strPathName(wxT(""));
		int iItemIndex = 0;
		m_iDepthCount = 0;
		while (tokenizer.HasMoreTokens())
		{
			strPathName = tokenizer.GetNextToken();
			if (!strPathName.IsEmpty())
			{
				AddPathPolygon(pDC, strPathName, iItemIndex);
				iItemIndex++;
			}
		}
	}
}

void CPathView::AddPathPolygon(wxDC* pDC, const wxString& strPath, int iPrevIndex)
{
    int iDataCount = m_vecPathDatas.size();
	wxPoint ptPolygon[6] = { { 0, 0 }, };

    wxRect rcOld;
    wxRect rcNew;

    wxSize szCharWidth = pDC->GetTextExtent(strPath);

    CPathItemData item;

    if(iDataCount > 0)
    {
        item = m_vecPathDatas.at(iPrevIndex - 1);
        rcOld = item.m_rcDisp;
    }
    else
        rcOld = wxRect(1, 0, 0, 20);

    CPathItemData pNewItem;

	rcNew.SetLeft(rcOld.GetRight());
	rcNew.SetTop(rcOld.GetTop());
	rcNew.SetRight(rcNew.GetLeft() + szCharWidth.GetWidth() + 15);
	rcNew.SetBottom(rcOld.GetBottom());

    if(iDataCount == 0)
    {
        ptPolygon[0].x = rcNew.GetLeft();
        ptPolygon[0].y = rcNew.GetTop();

        ptPolygon[1].x = rcNew.GetRight() - m_iPolygonGap;
        ptPolygon[1].y = rcNew.GetTop();

        ptPolygon[2].x = rcNew.GetRight();
        ptPolygon[2].y = rcNew.GetHeight() / 2;

        ptPolygon[3].x = rcNew.GetRight() - m_iPolygonGap;
        ptPolygon[3].y = rcNew.GetBottom();

        ptPolygon[4].x = rcNew.GetLeft();
        ptPolygon[4].y = rcNew.GetBottom();

        ptPolygon[5].x = rcNew.GetLeft();
        ptPolygon[5].y = rcNew.GetTop();
    }
    else
    {
        ptPolygon[0] = item.m_ptPolygon[1];

        ptPolygon[1].x = rcNew.GetRight() - 5;
        ptPolygon[1].y = rcNew.GetTop();

        ptPolygon[2].x = rcNew.GetRight();
        ptPolygon[2].y = rcNew.GetHeight() / 2;

        ptPolygon[3].x = rcNew.GetRight() - 5;
        ptPolygon[3].y = rcNew.GetBottom();

        ptPolygon[4] = item.m_ptPolygon[3];
        ptPolygon[5] = item.m_ptPolygon[2];
    }

    pNewItem.m_strName = strPath;
	pNewItem.m_rcDisp = rcNew;

    pNewItem.m_ptPolygon[0] = ptPolygon[0];
    pNewItem.m_ptPolygon[1] = ptPolygon[1];
    pNewItem.m_ptPolygon[2] = ptPolygon[2];
    pNewItem.m_ptPolygon[3] = ptPolygon[3];
    pNewItem.m_ptPolygon[4] = ptPolygon[4];
    pNewItem.m_ptPolygon[5] = ptPolygon[5];

	m_vecPathDatas.push_back(pNewItem);

	m_iDispWidth += rcNew.GetWidth();
	if (m_iDispWidth >= m_viewRect.GetWidth() - 25)
	{
		if (!m_bNextPath)
			m_bNextPath = true;

		m_iDepthCount++;
	}
	else
	{
		m_bNextPath = false;
		m_iPrevItemIndex = m_vecPathDatas.size();
	}
}

void CPathView::DisplayPathItem(wxDC* pDC)
{
	m_iTotalItemCount = wx_static_cast(int, m_vecPathDatas.size());
	if (m_iTotalItemCount == 0)
		return;

	int xPos = 0;
	int yPos = 0;

	m_iDispWidth = 0;

	for (int iIndex = 0; iIndex < m_iPrevItemIndex; iIndex++)
	{
		CPathItemData pItem = m_vecPathDatas.at(iIndex);
#ifdef __WXMSW__
		if(iIndex == 0)
			m_strCurrentDrive = pItem.m_strName + SLASH;
#endif
		wxPen pen(wxColour(0, 0, 0));
		wxBrush brush;

		pDC->SetTextForeground(wxColour(0, 0, 0));

		if (m_iMouseOverIndex == iIndex)
		{
			brush.SetColour(*wxWHITE);
			pDC->SetTextForeground(wxColour(255, 0, 255));
		}
		else
			brush.SetColour(wxColour(200, 200, 200));

		pDC->SetPen(pen);
		pDC->SetBrush(brush);

		pDC->DrawPolygon(6, pItem.m_ptPolygon, -1);

		pDC->SetPen(wxNullPen);
		pDC->SetBrush(wxNullBrush);


        wxRect rcName = pItem.m_rcDisp;
        rcName.SetLeft(rcName.GetLeft() + 5);
        rcName.SetTop(rcName.GetTop());

        pDC->DrawLabel(pItem.m_strName, rcName, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	}
}

void CPathView::OnMouseMove(wxMouseEvent& event)
{
	m_bMouseOver = true;
	m_bNextPathMouseOver = false;
	m_bFounded = false;

	m_iMouseOverIndex = 0;

	wxPoint ptMouse = event.GetPosition();
	wxString strToolTipPath(wxT(""));

	std::vector<CPathItemData>::const_iterator iter = m_vecPathDatas.begin();
	for (iter; iter != m_vecPathDatas.end(); iter++)
	{
		if (iter->m_rcDisp.Contains(ptMouse))
		{
			m_bFounded = true;
			m_rcClick = iter->m_rcDisp;
			if (!iter->m_strName.IsEmpty())
			{
				strToolTipPath = strToolTipPath.Cmp(wxT("")) == 0 ? iter->m_strName + SLASH : strToolTipPath + iter->m_strName;
				m_pToolTip->SetTip(strToolTipPath);
			}

			break;
		}
		else
			m_iMouseOverIndex++;

		if (iter->m_strName.Cmp(wxT("")) != 0)
			strToolTipPath += iter->m_strName + SLASH;
	}

	theUtility->RefreshWindow(this, m_viewRect);
}

void CPathView::OnMouseLButtonClick(wxMouseEvent& event)
{
	if(m_bFounded)
	{
		wxString strNewPath(wxT(""));
		int iPathCount = m_vecPathDatas.size();
		int iIndex;

        iIndex = 0;
/*
	#ifdef __WXMSW__
		iIndex = 1;
	#else
		iIndex = 0;
	#endif
*/
		for(iIndex; iIndex <= m_iMouseOverIndex; iIndex++)
		{
			if(iIndex > wx_static_cast(int, m_vecPathDatas.size()))
				break;

			std::vector<CPathItemData>::const_iterator cIt = m_vecPathDatas.begin() + iIndex;
			strNewPath += cIt->m_strName;
			if(iIndex != m_iMouseOverIndex)
				strNewPath += SLASH;
		}

		if(!strNewPath.IsEmpty())
			UpdatePath(strNewPath);
	}
}

void CPathView::OnMouseDBClick(wxMouseEvent& event)
{
	m_pTxtCtrl->SetLabelText(m_strPath);
	m_pTxtCtrl->SetSize(m_viewRect);
	m_pTxtCtrl->SetInsertionPointEnd();
	m_pTxtCtrl->Show(true);
	m_pTxtCtrl->SetFocus();
}

void CPathView::OnMouseRButtonClick(wxMouseEvent& event)
{
	if(m_bFounded)
	{
		wxString strNewPath(wxT(""));
		int iPathCount = m_vecPathDatas.size();
		int iIndex;

        iIndex = 0;
/*
	#ifdef __WXMSW__
		iIndex = 1;
	#else
		iIndex = 0;
	#endif
*/
		for(iIndex; iIndex <= m_iMouseOverIndex; iIndex++)
		{
			if(iIndex > wx_static_cast(int, m_vecPathDatas.size()))
				break;

			std::vector<CPathItemData>::const_iterator cIt = m_vecPathDatas.begin() + iIndex;
			strNewPath += cIt->m_strName;
			if(iIndex != m_iMouseOverIndex)
				strNewPath += SLASH;
		}

		iIndex = 0;
		wxWindow* pWnd = static_cast<wxWindow *>(event.GetEventObject());
		wxMenu menuPopup;
#ifdef __WXMSW__
		if(m_iMouseOverIndex == 0)
		{
			//popup메뉴
			wxMenu menuPopup;
			wxBitmap bmp = wxArtProvider::GetBitmap(wxART_HARDDISK, wxART_OTHER, wxSize(16,16));

			wxString strDriveName(wxT(""));
			wxString strDriveDispalyName(wxT(""));

			std::vector<CNextMDirData>::const_iterator cit;
			for(cit = theDriveInfo->BeginIter(); cit != theDriveInfo->EndIter(); ++cit )
			{
				strDriveName = cit->GetDriveName();
				strDriveDispalyName = cit->GetDisplayName();

				wxMenuItem* menu = menuPopup.AppendCheckItem(wxDRIVE_ID_START + iIndex, strDriveDispalyName);
				if(m_strCurrentDrive.CompareTo(strDriveName) == 0)
					m_iDriveSelectedIndex = iIndex;

				iIndex++;
			}

			wxPoint pt = pWnd->ClientToScreen(m_rcClick.GetBottomLeft());
			pt = ScreenToClient(pt);

			PopupMenu(&menuPopup, pt);
			return;
		}
#endif

#ifdef __WXMSW__

	#ifdef _UNICODE
		typedef WIN32_FIND_DATAW WIN32_DATA;
	#else
		typedef WIN32_FIND_DATA WIN32_DATA;
	#endif

		wxString strFindDir = strNewPath;
		if(strNewPath.Right(1).CmpNoCase(SLASH) != 0)
		{
			strNewPath += SLASH;

			strFindDir += SLASH;
			strFindDir += wxT("*");
		}
		else
			strFindDir += wxT("*");

		WIN32_DATA fd;
		HANDLE hFind;
		bool bFound = false;

		hFind = FindFirstFileEx(strFindDir, FindExInfoStandard, &fd, FindExSearchLimitToDirectories, NULL, 0);
		if(hFind == INVALID_HANDLE_VALUE)
			return;

		m_subdirMap.clear();
		wxBitmap bmp = wxArtProvider::GetBitmap(wxART_NEW_DIR, wxART_MENU, wxSize(16,16));

		bFound = true;
		iIndex = 0;

		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;

			wxString strName(fd.cFileName);
			if(strName.CmpNoCase(wxT(".")) == 0 || strName.CmpNoCase(wxT("..")) == 0)
				continue;

			wxMenuItem* menu = menuPopup.Append(wxSUBDIR_START + iIndex, strName);

			wxString strFullPath = strNewPath + strName;
			m_subdirMap.insert(std::make_pair(wxSUBDIR_START + iIndex, strFullPath));

			iIndex++;
		} while ((bFound = FindNextFile(hFind, &fd) != 0));

		wxPoint pt = pWnd->ClientToScreen(m_rcClick.GetBottomLeft());
		pt = ScreenToClient(pt);

		this->PopupMenu(&menuPopup, pt);

#endif // __WXMSW__

	}
}

void CPathView::OnLeaveWindow(wxMouseEvent& event)
{
	m_bMouseOver = false;
	m_iMouseOverIndex = -1;

	theUtility->RefreshWindow(this, m_viewRect);
}

void CPathView::AddPath(const wxString& strPath)
{
	Init();

	m_bChangedPath = true;
	m_strPath = strPath;

	theUtility->RefreshWindow(this, m_viewRect);
}

void CPathView::OnEnterTextCtrl(wxCommandEvent& event)
{
	wxString strNewPath = m_pTxtCtrl->GetValue();
	if(!wxDirExists(strNewPath))
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_READABLE_DIR_FALSE")), PROGRAM_FULL_NAME, wxICON_ERROR | wxOK);
		return;
	}

	if(theUtility->Compare(strNewPath.Right(1), SLASH) == 0)
		strNewPath = strNewPath.Left(strNewPath.Len() - 1);

	event.Skip();

	UpdatePath(strNewPath);

	m_pTxtCtrl->SetLabelText(wxT(""));
	m_pTxtCtrl->Show(false);

	theSplitterManager->GetCurrentViewManager()->SetFocusPage();
}

void CPathView::OnKeyDownTextCtrl(wxKeyEvent& event)
{
	int iKeyCode = event.GetKeyCode();
	if (iKeyCode == WXK_ESCAPE)
	{
		m_pTxtCtrl->SetLabelText(wxT(""));
		m_pTxtCtrl->Show(false);


		theSplitterManager->GetCurrentViewManager()->SetFocusPage();
	}

	event.Skip();
}

void CPathView::OnKillFocusTxtCtrl(wxFocusEvent& event)
{
	m_pTxtCtrl->SetLabelText(wxT(""));
	m_pTxtCtrl->Show(false);

	event.Skip();
}

#ifdef __WXMSW__
void CPathView::OnDriveMenuClick(wxCommandEvent& event)
{
	if(event.GetEventType() == wxEVT_COMMAND_MENU_SELECTED)
	{
		int iMenuID = event.GetId();
        int iClickItem = iMenuID - wxDRIVE_ID_START;

		std::vector<CNextMDirData>::const_iterator cit = theDriveInfo->BeginIter() + iClickItem;
		m_iDriveSelectedIndex = iClickItem;

		wxString strDriveName = cit->GetDriveName();
		UpdatePath(strDriveName);
	}
}

void CPathView::OnDrvieMenuUpdate(wxUpdateUIEvent& event)
{
	event.Check(false);
	int nMenuID = event.GetId();

	if(m_iDriveSelectedIndex == (nMenuID - wxDRIVE_ID_START))
		event.Check(true);
}
#endif

void CPathView::OnMoveSubDir(wxCommandEvent& event)
{
	int iMenuID = event.GetId();
	int iSelItem = iMenuID - wxSUBDIR_START;

	std::unordered_map<int, wxString>::const_iterator cit = m_subdirMap.find(wxSUBDIR_START + iSelItem);
	if(cit == m_subdirMap.end())
		return;

	wxString strFullPath = cit->second;
	UpdatePath(strFullPath);
}

void CPathView::UpdatePath(const wxString& strPath)
{
	AddPath(strPath);

	CViewsPanel* parent = (CViewsPanel *)GetParent();
	CViewsPanel* currPanel = theSplitterManager->GetCurrentViewManager()->GetCurrentViewsPanels();

	if(parent != currPanel)
		theSplitterManager->ChangeSplitView();

	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strPath, CHANGE_DIR_FILELISTIVEW);
	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strPath, CHANGE_DIR_TAB);
}
