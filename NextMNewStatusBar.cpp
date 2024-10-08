#include "ginc.h"
#include "NextMNewStatusBar.h"
#include "NextMExecuteMenuOperation.h"

#include <wx/arrimpl.cpp>

const char *numlockIndicators[] = { "OFF", "NUM" };
const char *capslockIndicators[] = { "", "CAPS" };
const char *ctrlIndicators[] = { "", "CTRL" };
const char *shiftIndicators[] = { "", "SHIFT" };
const char *altIndicators[] = { "", "ALT" };

wxIMPLEMENT_DYNAMIC_CLASS(CNextMNewStatusBar, wxWindow);

WX_DEFINE_EXPORTED_OBJARRAY(NextMStatusBarItemArray)

wxBEGIN_EVENT_TABLE(CNextMNewStatusBar, wxControl)
    EVT_PAINT(CNextMNewStatusBar::OnPaint)
	EVT_ERASE_BACKGROUND(CNextMNewStatusBar::OnErase)
	EVT_SIZE(CNextMNewStatusBar::OnSize)
	EVT_TIMER(wxID_ANY, CNextMNewStatusBar::OnTimer)
	EVT_IDLE(CNextMNewStatusBar::OnIdle)
	//Mouse Event
	EVT_MOTION(CNextMNewStatusBar::OnMouseMove)
	EVT_ENTER_WINDOW(CNextMNewStatusBar::OnMouseEnter)
	EVT_LEAVE_WINDOW(CNextMNewStatusBar::OnMouseLeave)
	EVT_LEFT_DOWN(CNextMNewStatusBar::OnMouseLBottonDown)
wxEND_EVENT_TABLE()

CNextMNewStatusBar::CNextMNewStatusBar()
{
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

CNextMNewStatusBar::CNextMNewStatusBar(wxWindow *parent,
									   wxWindowID id,
									   long style,
									   const wxString& name)
	: m_spacing(1)
	, m_timer(this)
{
	m_shapeType = theConfig->GetStatusbarType();
	m_timer.Start(1000);
#ifdef __WXMSW__
	m_viewFont.Create(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Segoe UI"));
#else
	m_viewFont = wxSystemSettings::GetFont(wxSYS_DEVICE_DEFAULT_FONT);
#endif

	m_defaultMap.insert(std::make_pair(Field_F2, wxT("m_fMenu_Rename")));
	m_defaultMap.insert(std::make_pair(Field_F3, wxT("m_fMenu_FileEdit")));
	m_defaultMap.insert(std::make_pair(Field_F4, wxT("")));
	m_defaultMap.insert(std::make_pair(Field_F5, wxT("m_pMenu_Refresh")));
	m_defaultMap.insert(std::make_pair(Field_F7, wxT("m_fMenu_MakeFolder")));
	m_defaultMap.insert(std::make_pair(Field_F8, wxT("m_menuComp_Progress")));
	m_defaultMap.insert(std::make_pair(Field_F10, wxT("m_pMenu_PathMng")));

	m_ctrlMap.insert(std::make_pair(CTRL_Field_T, wxT("m_fMenu_NewTab")));
	m_ctrlMap.insert(std::make_pair(CTRL_Field_C, wxT("m_eMenu_Copy")));
	m_ctrlMap.insert(std::make_pair(CTRL_Field_X, wxT("m_eMenu_Cut")));
	m_ctrlMap.insert(std::make_pair(CTRL_Field_V, wxT("m_eMenu_Paste")));
	m_ctrlMap.insert(std::make_pair(CTRL_Field_M, wxT("m_eMenu_ContextMenu")));
	m_ctrlMap.insert(std::make_pair(CTRL_Field_F11, wxT("m_toolMenu_ENV")));

	m_altMap.insert(std::make_pair(ALT_Field_C, wxT("m_fMenu_Copy")));
	m_altMap.insert(std::make_pair(ALT_Field_M, wxT("m_fMenu_Move")));
	m_altMap.insert(std::make_pair(ALT_Field_S, wxT("m_viewMenu_Window_0")));
	m_altMap.insert(std::make_pair(ALT_Field_V, wxT("m_viewMenu_Window_1")));
	m_altMap.insert(std::make_pair(ALT_Field_H, wxT("m_viewMenu_Window_2")));
	m_altMap.insert(std::make_pair(ALT_Field_X, wxT("m_menuComp_CurrentDirCompRelease")));

	(void)Create(parent, id, style, name);
}

CNextMNewStatusBar::~CNextMNewStatusBar()
{
	if (m_timer.IsRunning())
		m_timer.Stop();
}

bool CNextMNewStatusBar::Create(wxWindow *parent,
								wxWindowID id,
							    long style,
								const wxString& name)
{
	if ( !CreateControl(parent, id, wxDefaultPosition, wxSize(m_defaultWidth, m_defaultHeight),
                        style, wxDefaultValidator, name) )
        return false;

#ifdef __WXMSW__
    if ( !MSWCreateControl(STATUSCLASSNAME, wxString(),
                           wxDefaultPosition, wxSize(m_defaultWidth, m_defaultHeight)) )
        return false;
#else

#endif

	return true;
}

void CNextMNewStatusBar::OnErase(wxEraseEvent& event)
{

}

void CNextMNewStatusBar::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

    m_viewClientRect = m_viewRect = GetClientRect();
	wxMemoryDC* pMemDC = m_memDC.CreateMemoryDC(&dc, m_viewRect, m_colBackground, m_colBackground);

	pMemDC->SetFont(m_viewFont);

	m_viewClientRect.Inflate(-1, -1);

	if(m_bChangedSize || !m_bSet)
	{
		Settings(pMemDC);
		m_bChangedSize = false;
	}

	Render(pMemDC);
	dc.Blit(0, 0, m_viewRect.GetWidth(), m_viewRect.GetHeight(), pMemDC, 0, 0);
}

void CNextMNewStatusBar::Render(wxDC* pDC)
{
	if(m_panes.GetCount() == 0)
	{
		pDC->SetPen(m_colEdgeColour);
		pDC->DrawRectangle(m_viewClientRect);

		pDC->DrawLabel(wxT(" NextMNewStatus"), m_viewClientRect, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
		return;
	}

	pDC->SetTextForeground(wxColour(0, 0, 0));
	if(m_shapeType != SHAPE_NONE)
		DrawShape(pDC);

	for(size_t i = 0; i < m_panes.GetCount(); i++)
	{
		wxString strDisp = m_panes[i].GetText();

		wxRect rcDisp = m_panes[i].GetRect();
		wxRect rcText = m_panes[i].GetRect();

		rcText.SetTop(rcDisp.GetTop() + 1);
		if(m_panes[i].GetMouseMove())
		{
			pDC->SetBrush(m_panes[i].GetMouseOverBackgroundColour());
			pDC->DrawRectangle(rcDisp);

			pDC->SetTextForeground(m_panes[i].GetMouseOverTextColour());
		}

		pDC->DrawLabel(strDisp, rcText, m_panes[i].GetAlign());
	}
}

void CNextMNewStatusBar::DrawShape(wxDC* pDC)
{
	for(size_t i = 0; i < m_panes.GetCount(); i++)
	{
		wxRect rcDisp = m_panes[i].GetRect();
		int x1 = rcDisp.GetLeft();
		int y1 = rcDisp.GetTop();
		int x2 = rcDisp.GetRight();
		int y2 = rcDisp.GetBottom();

		if((m_shapeType == SHAPE_LEFT_LINE) || (m_shapeType == SHAPE_RIGHT_LINE))
		{
			pDC->SetPen(m_colEdgeColour);
			switch(m_shapeType)
			{
				case SHAPE_LEFT_LINE:
					pDC->DrawLine(x1, y1, x1, y2);
					break;

				case SHAPE_RIGHT_LINE:
					pDC->DrawLine(x2, y1, x2, y2);
					break;
			}
		}
		else if( (m_shapeType == SHAPE_SHADOW_LINE_RAISED) || (m_shapeType == SHAPE_SHADOW_LINE_SUNKEN) )
		{
			wxColour colWhite = wxColour(255, 255, 255);
			wxColour colShadow =  wxColour(128, 128, 128);

			pDC->SetPen(m_shapeType == SHAPE_SHADOW_LINE_RAISED ? colWhite : colShadow);
			pDC->DrawLine(x1, y1, x1, y2);
			pDC->DrawLine(x1, y1, x2, y1);

			pDC->SetPen(m_shapeType == SHAPE_SHADOW_LINE_RAISED ? colShadow : colWhite);
			pDC->DrawLine(x2, y1, x2, y2);
			pDC->DrawLine(x1, y2, x2, y2);
		}
		else if(m_shapeType == SHAPE_RECTANGLE)
		{
			pDC->SetPen(m_colEdgeColour);
			pDC->DrawRectangle(rcDisp);
		}
	}
}

void CNextMNewStatusBar::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

	m_bChangedSize = true;
	theUtility->RefreshWindow(this, m_viewRect);
}

/*
 * Refer to wxStatusBar::SetFieldsCount
 *
*/
void CNextMNewStatusBar::SetFieldsCount(int number, const int *widths)
{
	m_panes.Clear();

	wxCHECK_RET( number > 0, wxT("invalid field number in SetFieldsCount") );
    if ( (size_t)number > m_panes.GetCount() )
    {
        CNextMStatusBarItem item;
		for (size_t i = m_panes.GetCount(); i < (size_t)number; ++i)
            m_panes.Add(item);
    }
    else if ( (size_t)number < m_panes.GetCount() )
        m_panes.RemoveAt(number, m_panes.GetCount() - number);

    SetStatusWidths(number, widths);
}

/*
 * Refer to wxStatusBar::SetStatusWidths
 *
*/
void CNextMNewStatusBar::SetStatusWidths(int n, const int widths[])
{
	wxASSERT_MSG( (size_t)n == m_panes.GetCount(), wxT("field number mismatch") );
	if (widths == nullptr)
    {
        // special value meaning: override explicit pane widths and make them all
        // of the same size
        m_bSameWidthForAllPanes = true;
    }
    else
    {
        for ( size_t i = 0; i < m_panes.GetCount(); i++ )
            m_panes[i].SetWidth(widths[i]);

		m_bSameWidthForAllPanes = false;
    }
}

void CNextMNewStatusBar::SetStatusText(const wxString& text, int n)
{
	if(m_panes.GetCount() == 0)
		return;

//	if(!m_bSet)
//		return;

	m_panes[n].SetText(text);
	bool bReturn = false;
	if(m_bKeyPress[KEY_DEFAULT])
	{
		bool bMore = false;
		switch(n)
		{
			case Field_NumLockIndicator:
			{
				int numlock_status = wxGetKeyState(WXK_NUMLOCK);
				if(m_Num_Caps_Shift[KEY_NUMLOCKS] != numlock_status)
				{
					bMore = true;
					m_Num_Caps_Shift[KEY_NUMLOCKS] = numlock_status;
				}

				bReturn = true;
			}
				break;

			case Field_CapsLockIndicator:
			{
				int capslock_status = wxGetKeyState(WXK_CAPITAL);
				if(m_Num_Caps_Shift[KEY_CAPSLOCK] != capslock_status)
				{
					bMore = true;
					m_Num_Caps_Shift[KEY_CAPSLOCK] = capslock_status;
				}

				bReturn = true;
			}
				break;
			case Field_Shift:
			{
				int shift_status = wxGetKeyState(WXK_SHIFT);
				if(m_Num_Caps_Shift[KEY_SHIFT] != shift_status)
				{
					bMore = true;
					m_Num_Caps_Shift[KEY_SHIFT] = shift_status;
				}

				bReturn = true;
			}
				break;
			default:
				break;
		}

		if(bMore)
			theUtility->RefreshWindow(this, m_panes[n].GetRect());
	}

	if(m_bSet && !bReturn)
		theUtility->RefreshWindow(this, m_panes[n].GetRect());
}

void CNextMNewStatusBar::SetStatusRect(int n, const wxRect& rc)
{
	if(m_panes.GetCount() == 0)
		return;

	m_panes[n].SetRect(rc);
}

void CNextMNewStatusBar::SetStatusTextAlign(int n, wxAlignment align)
{
	if(m_panes.GetCount() == 0)
		return;

	m_panes[n].SetTextAlignment(align);
}

const CNextMStatusBarItem& CNextMNewStatusBar::GetField(int index) const
{
	return m_panes[index];
}

void CNextMNewStatusBar::SetBackgroundColor(const wxColour& colBackground)
{
	m_colBackground = colBackground;
}

void CNextMNewStatusBar::SetPaneColor(const wxColour& colPane)
{
	m_colEdgeColour = colPane;
}

void CNextMNewStatusBar::SetColSpacing(const int space)
{
	m_colSpacing = space;
}

void CNextMNewStatusBar::SetStatusMouseOverColor(int n, bool bMouseOver)
{
	m_panes[n].SetMouseOver(bMouseOver);
	theUtility->RefreshWindow(this, m_panes[n].GetRect());
}

void CNextMNewStatusBar::Settings(wxDC* pDC)
{
	if(wxGetKeyState(WXK_CONTROL))
		InitCtrlInfo(pDC);
	else if(wxGetKeyState(WXK_ALT))
		InitAltInfo(pDC);
	else
		InitDefaultInfo(pDC);

	m_bSet = true;

	if(m_bMouseEnter)
	{
		if(FindItemInMousePoint(m_ptMouse))
		{
			if(m_indxMouseOver != -1)
				m_panes[m_indxMouseOver].SetMouseOver(true);
		}
	}
}

void CNextMNewStatusBar::InitDefaultInfo(wxDC* pDC)
{
	wxString strClick = wxDateTime::Now().FormatTime();

	m_Num_Caps_Shift[KEY_NUMLOCKS] = 0;
	m_Num_Caps_Shift[KEY_CAPSLOCK] = 0;
	m_Num_Caps_Shift[KEY_SHIFT] = 0;

	wxSize numLockSize = pDC->GetTextExtent(wxT("NUM"));//numlockIndicators[1]);
	wxSize capsLock    = pDC->GetTextExtent(wxT("CAPS"));//capslockIndicators[1]);
	wxSize shiftSize   = pDC->GetTextExtent(wxT("SHIFT"));//shiftIndicators[1]);
	wxSize clockSize   = pDC->GetTextExtent(strClick);

	int numlockWidth  = numLockSize.GetWidth() + (m_colSpacing * 3);
	int capsLockWidth = capsLock.GetWidth() + (m_colSpacing * 3);
	int shiftWidth    = shiftSize.GetWidth() + (m_colSpacing * 3);
	int clockWidth    = clockSize.GetWidth() + 15;

	int dispWidth = m_viewClientRect.GetWidth() - (numlockWidth + capsLockWidth + shiftWidth + clockWidth);

	dispWidth -= (m_colSpacing * 5);
	dispWidth = dispWidth / (Field_Max - 4);

	int widths[Field_Max];

	widths[Field_F2]                = dispWidth;
	widths[Field_F3]                = dispWidth;
	widths[Field_F4]                = dispWidth;
	widths[Field_F5]                = dispWidth;
	widths[Field_F7]                = dispWidth;
	widths[Field_F8]                = dispWidth;
	widths[Field_F10]               = dispWidth;
	widths[Field_NumLockIndicator]  = numlockWidth;
	widths[Field_CapsLockIndicator] = capsLockWidth;
	widths[Field_Shift]             = shiftWidth;
	widths[Field_Clock]             = clockWidth;

	SetFieldsCount(Field_Max, widths);

	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_RENAME")),       Field_F2);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_FILEEDIT")),     Field_F3);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_VIEWFAVORITE")), Field_F4);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_REFRESH")),      Field_F5);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_MAKE_FOLDER")),  Field_F7);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_COMPRESS")),     Field_F8);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_DIRMANAGER")),   Field_F10);

	SetStatusText(shiftIndicators[wxGetKeyState(WXK_SHIFT)], Field_Shift);
	SetStatusText(numlockIndicators[wxGetKeyState(WXK_NUMLOCK)], Field_NumLockIndicator);
	SetStatusText(capslockIndicators[wxGetKeyState(WXK_CAPITAL)],Field_CapsLockIndicator);

//	SetStatusText(wxT(""),  Field_NumLockIndicator);
//	SetStatusText(wxT(""),  Field_CapsLockIndicator);
//	SetStatusText(wxT(""),  Field_Shift);
	SetStatusText(wxDateTime::Now().FormatTime(), Field_Clock);

	SetStatusTextAlign(Field_NumLockIndicator, wxALIGN_CENTER);
	SetStatusTextAlign(Field_CapsLockIndicator, wxALIGN_CENTER);
	SetStatusTextAlign(Field_Shift, wxALIGN_CENTER);
	SetStatusTextAlign(Field_Clock, wxALIGN_CENTER);

	CalcRect(Field_Max, widths);
}

void CNextMNewStatusBar::InitCtrlInfo(wxDC* pDC)
{
	int dispWidth = (m_viewClientRect.GetWidth() - 4) / CTRL_Field_Max;
	int widths[CTRL_Field_Max];

	widths[CTRL_Field_T]     = dispWidth;
	widths[CTRL_Field_C]     = dispWidth;
	widths[CTRL_Field_X]     = dispWidth;
	widths[CTRL_Field_V]     = dispWidth;
	widths[CTRL_Field_M]     = dispWidth;
	widths[CTRL_Field_F11]   = dispWidth;

	SetFieldsCount(CTRL_Field_Max, widths);

	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_NEW_TAB")), CTRL_Field_T);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_COPY")), CTRL_Field_C);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_CUT")), CTRL_Field_X);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_PASTE")), CTRL_Field_V);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_CONTEXT_MENU")), CTRL_Field_M);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_OPTION")), CTRL_Field_F11);

	CalcRect(CTRL_Field_Max, widths);
}

void CNextMNewStatusBar::InitAltInfo(wxDC* pDC)
{
	int dispWidth = (m_viewClientRect.GetWidth() - 4) / ALT_Field_Max;
	int widths[ALT_Field_Max];

	widths[ALT_Field_C] = dispWidth;
	widths[ALT_Field_M] = dispWidth;
	widths[ALT_Field_S] = dispWidth;
	widths[ALT_Field_V] = dispWidth;
	widths[ALT_Field_H] = dispWidth;
	widths[ALT_Field_X] = dispWidth;

	SetFieldsCount(ALT_Field_Max, widths);

	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_COPY")),         ALT_Field_C);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_MOVE")),         ALT_Field_M);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_ONEVIEW")),      ALT_Field_S);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_SPLITVERT")),    ALT_Field_V);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_SPLITHORZ")),    ALT_Field_H);
	SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_COMPREALEASE")), ALT_Field_X);

	CalcRect(ALT_Field_Max, widths);
}

void CNextMNewStatusBar::CalcRect(int itemCount, const int widths[])
{
	wxRect rc( 2, m_spacing, widths[0], m_viewClientRect.GetHeight());
	SetStatusRect(0, rc);

	for(int i = 1; i < itemCount; i++)
	{
		wxRect rcBefore = m_panes[i - 1].GetRect();
		wxRect rcNext( rcBefore.GetRight() + m_colSpacing, m_spacing, widths[i], m_viewClientRect.GetHeight());
		if(i == itemCount - 1)
		{
			int right = rcNext.GetRight();
			if(right < m_viewClientRect.GetRight())
				rcNext.SetRight(m_viewClientRect.GetRight());
		}

		SetStatusRect(i, rcNext);
	}
}

void CNextMNewStatusBar::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	UpdateClock();
}

void CNextMNewStatusBar::UpdateClock()
{
	if (!m_bSet) return;

	if(m_bKeyPress[KEY_DEFAULT])
		SetStatusText(wxDateTime::Now().FormatTime(), Field_Clock);
}

void CNextMNewStatusBar::OnIdle(wxIdleEvent& event)
{
	if(m_bKeyPress[KEY_DEFAULT] && m_bSet)
	{
		SetStatusText(shiftIndicators[wxGetKeyState(WXK_SHIFT)], Field_Shift);
		SetStatusText(numlockIndicators[wxGetKeyState(WXK_NUMLOCK)], Field_NumLockIndicator);
		SetStatusText(capslockIndicators[wxGetKeyState(WXK_CAPITAL)],Field_CapsLockIndicator);
	}

	bool more = event.MoreRequested();
	if(wxGetKeyState(WXK_CONTROL))
	{
		more = !m_bKeyPress[KEY_CTRL];
		m_bKeyPress[KEY_DEFAULT] = false;

		if(!m_bKeyPress[KEY_CTRL])
			m_bKeyPress[KEY_CTRL] = true;
	}
	else if(wxGetKeyState(WXK_ALT))
	{
		more = !m_bKeyPress[KEY_ALT];
		m_bKeyPress[KEY_DEFAULT] = false;

		if(!m_bKeyPress[KEY_ALT])
			m_bKeyPress[KEY_ALT] = true;
	}
	else
	{
		m_bKeyPress[KEY_CTRL] = false;
		m_bKeyPress[KEY_ALT]  = false;

		more = !m_bKeyPress[KEY_DEFAULT];

 		if(!m_bKeyPress[KEY_DEFAULT])
			m_bKeyPress[KEY_DEFAULT] = true;
	}

	if(more)
	{
		m_indxMouseOver = -1;
		m_bSet = false;

		Refresh();
	}

	event.Skip();
}

void CNextMNewStatusBar::OnMouseMove(wxMouseEvent& event)
{
	m_ptMouse = event.GetPosition();
	if(m_bMouseEnter)
	{
		int panesCount = (int)m_panes.GetCount();
		if(panesCount > m_indxMouseOver)
		{
			if(m_indxMouseOver > -1)
			{
				SetStatusMouseOverColor(m_indxMouseOver, false);
				m_indxMouseOver = -1;
			}

			if(FindItemInMousePoint(event.GetPosition()))
				SetStatusMouseOverColor(m_indxMouseOver, true);
		}
	}

	event.Skip();
}

void CNextMNewStatusBar::OnMouseEnter(wxMouseEvent& event)
{
	m_bMouseEnter = true;
	m_ptMouse.x = -1;
	m_ptMouse.y = -1;
	event.Skip();
}

void CNextMNewStatusBar::OnMouseLeave(wxMouseEvent& event)
{
	m_indxMouseOver = -1;
	m_bMouseEnter = false;

	m_ptMouse.x = -1;
	m_ptMouse.y = -1;

	for(size_t i = 0; i < m_panes.GetCount(); i++)
		m_panes[i].SetMouseOver(false);

	theUtility->RefreshWindow(this, m_viewRect);
	event.Skip();
}

bool CNextMNewStatusBar::FindItemInMousePoint(const wxPoint& pt)
{
	bool bFoundOK = false;

	for(size_t i = 0; i < m_panes.GetCount(); i++)
	{
		wxRect rc = m_panes[i].GetRect();
		if(rc.Contains(pt))
		{
			m_indxMouseOver = (int)i;
			bFoundOK = true;

			break;
		}
	}

	m_bMouseFound = bFoundOK;
	return bFoundOK;
}

void CNextMNewStatusBar::OnMouseLBottonDown(wxMouseEvent& event)
{
	if(m_indxMouseOver != -1)
	{
		if (wxGetKeyState(WXK_CONTROL))
			ExecuteStatusBarClick(m_ctrlMap, m_indxMouseOver);
		else if (wxGetKeyState(WXK_ALT))
			ExecuteStatusBarClick(m_altMap, m_indxMouseOver);
		else
		{
			if(m_indxMouseOver == Field_F4)
				_gMenuEvent->GetMenuOperationPtr()->ShowFavoriteFromStatusBar();
			else
				ExecuteStatusBarClick(m_defaultMap, m_indxMouseOver);
		}
	}

	event.Skip();
}

void CNextMNewStatusBar::ExecuteStatusBarClick(const std::unordered_map<int, wxString>& _statusMap, int nIndex)
{
	std::unordered_map<int, wxString>::const_iterator fIter = _statusMap.find(nIndex);
	if(fIter != _statusMap.end())
	{
		int nMenuID = XRCID(fIter->second);
		_gMenuEvent->ExecuteMenu(nMenuID);
	}
}

