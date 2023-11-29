#include "./ginc.h"
#include "NextMStatusBar.h"

static const char *numlockIndicators[] = { "OFF", "NUM" };
static const char *capslockIndicators[] = { "", "CAPS" };
static const char *ctrlIndicators[] = { "", "CTRL" };
static const char *shiftIndicators[] = { "", "SHIFT" };
static const char *altIndicators[] = { "", "ALT" };

wxBEGIN_EVENT_TABLE(CNextMStatusBar, wxStatusBar)
	EVT_SIZE(CNextMStatusBar::OnSize)
	EVT_TIMER(wxID_ANY, CNextMStatusBar::OnTimer)
	EVT_IDLE(CNextMStatusBar::OnIdle)
	EVT_LEFT_DOWN(CNextMStatusBar::OnMouseLBottonDown)
wxEND_EVENT_TABLE()

CNextMStatusBar::CNextMStatusBar(wxWindow *parent, long style)
	: wxStatusBar(parent, wxID_ANY, style, wxT("CNextMStatusBar"))
	, m_timer(this)
	, m_bSet(false)
{
	Setting();

	m_timer.Start(1000);
	UpdateClock();
}

CNextMStatusBar::~CNextMStatusBar()
{
	if (m_timer.IsRunning())
		m_timer.Stop();
}

void CNextMStatusBar::OnSize(wxSizeEvent& event)
{
	Setting();
}

void CNextMStatusBar::OnIdle(wxIdleEvent& event)
{
	SetStatusText(ctrlIndicators[wxGetKeyState(WXK_CONTROL)], Field_Ctrl);
	SetStatusText(shiftIndicators[wxGetKeyState(WXK_SHIFT)], Field_Shift);
	SetStatusText(altIndicators[wxGetKeyState(WXK_ALT)], Field_Alt);
	SetStatusText(numlockIndicators[wxGetKeyState(WXK_NUMLOCK)],Field_NumLockIndicator);
	SetStatusText(capslockIndicators[wxGetKeyState(WXK_CAPITAL)],Field_CapsLockIndicator);

	if (wxGetKeyState(WXK_CONTROL))
	{
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_NEW_TAB")), Field_F2);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_COPY")), Field_F3);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_CUT")), Field_F4);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_PASTE")), Field_F5);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_CONTEXT_MENU")), Field_F7);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_BAR_OPTION")), Field_F10);
		SetStatusText(wxT(" "), Field_F8);
	}
	else if (wxGetKeyState(WXK_ALT))
	{

		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_COPY")), Field_F2);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_MOVE")), Field_F3);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_ONEVIEW")), Field_F4);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_SPLITVERT")), Field_F5);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_ALT_ANOTHER_COMPREALEASE")), Field_F7);
		SetStatusText(wxT(" "), Field_F10);
		SetStatusText(wxT(" "), Field_F8);
	}
	else
	{
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_RENAME")), Field_F2);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_FILEEDIT")), Field_F3);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_VIEWFAVORITE")), Field_F4);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_REFRESH")), Field_F5);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_MAKE_FOLDER")), Field_F7);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_COMPRESS")), Field_F8);
		SetStatusText(theMsg->GetMessage(wxT("MSG_STATUS_DEFAULT_DIRMANAGER")), Field_F10);
	}

	int fields = this->GetFieldsCount();

	wxClientDC dc(this);
    dc.SetPen(wxColour(157, 120, 98));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    for ( int i = 0; i < fields; i++ )
    {
        wxRect r;
        if ( this->GetFieldRect(i, r) )
            dc.DrawRectangle(r);
    }

	event.Skip();
}

void CNextMStatusBar::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	UpdateClock();
}

void CNextMStatusBar::Setting()
{
	wxClientDC dc(this);

	wxSize sz = GetClientSize();
	int iClientWidth = sz.GetWidth();
	if(iClientWidth == 0)
		return;

	int iFuncSize = (iClientWidth - 170) / 8;
	SetBackgroundColour(wxColour(220, 220, 220));

	wxSize sizeNumLock = dc.GetTextExtent(numlockIndicators[0]);
	sizeNumLock.IncTo(dc.GetTextExtent(numlockIndicators[1]));

	int widths[Field_Max];
	widths[Field_F2]                = iFuncSize;
	widths[Field_F3]                = iFuncSize;
	widths[Field_F4]                = iFuncSize;
	widths[Field_F5]                = iFuncSize;
	widths[Field_F7]                = iFuncSize;
	widths[Field_F8]                = iFuncSize;
	widths[Field_F10]               = iFuncSize;
	widths[Field_Ctrl]              = dc.GetTextExtent(ctrlIndicators[1]).x;
	widths[Field_Shift]             = dc.GetTextExtent(shiftIndicators[1]).x;
	widths[Field_Alt]               = dc.GetTextExtent(altIndicators[1]).x;
	widths[Field_NumLockIndicator]  = sizeNumLock.x;
	widths[Field_CapsLockIndicator] = dc.GetTextExtent(capslockIndicators[1]).x;
	widths[Field_Clock]             = iFuncSize;

	SetFieldsCount(Field_Max);
	SetStatusWidths(Field_Max, widths);

	m_bSet = true;

	m_defaultMap.insert(std::make_pair(Field_F2, wxT("m_fMenu_Rename")));
	m_defaultMap.insert(std::make_pair(Field_F3, wxT("m_fMenu_FileEdit")));
	m_defaultMap.insert(std::make_pair(Field_F4, wxT("")));
	m_defaultMap.insert(std::make_pair(Field_F5, wxT("m_pMenu_Refresh")));
	m_defaultMap.insert(std::make_pair(Field_F7, wxT("m_fMenu_MakeFolder")));
	m_defaultMap.insert(std::make_pair(Field_F8, wxT("m_menuComp_Progress")));
	m_defaultMap.insert(std::make_pair(Field_F10, wxT("m_pMenu_PathMng")));

	m_ctrlMap.insert(std::make_pair(Field_F2, wxT("m_fMenu_NewTab")));
	m_ctrlMap.insert(std::make_pair(Field_F3, wxT("m_eMenu_Copy")));
	m_ctrlMap.insert(std::make_pair(Field_F4, wxT("m_eMenu_Cut")));
	m_ctrlMap.insert(std::make_pair(Field_F5, wxT("m_eMenu_Paste")));
	m_ctrlMap.insert(std::make_pair(Field_F7, wxT("m_eMenu_ContextMenu")));
	m_ctrlMap.insert(std::make_pair(Field_F10, wxT("m_toolMenu_ENV")));

	m_altMap.insert(std::make_pair(Field_F2, wxT("m_fMenu_Copy")));
	m_altMap.insert(std::make_pair(Field_F3, wxT("m_fMenu_Move")));
	m_altMap.insert(std::make_pair(Field_F4, wxT("m_viewMenu_Window_0")));
	m_altMap.insert(std::make_pair(Field_F5, wxT("m_viewMenu_Window_1")));
	m_altMap.insert(std::make_pair(Field_F7, wxT("m_menuComp_CurrentDirCompRelease")));

}

void CNextMStatusBar::UpdateClock()
{
	if (!m_bSet) return;
	SetStatusText(wxDateTime::Now().FormatTime(), Field_Clock);
}

void CNextMStatusBar::OnMouseLBottonDown(wxMouseEvent& event)
{
	wxPoint pt = event.GetPosition();
	int fields = this->GetFieldsCount();

	int nClickIndex = 0;
    for ( int i = 0; i < fields; i++ )
    {
        wxRect r;
        if ( this->GetFieldRect(i, r) )
		{
			if(r.Contains(pt))
			{
				nClickIndex = i;
				break;
			}
		}
    }

	if (wxGetKeyState(WXK_CONTROL))
		ExecuteStatusBarClick(m_ctrlMap, nClickIndex);
	else if (wxGetKeyState(WXK_ALT))
		ExecuteStatusBarClick(m_altMap, nClickIndex);
	else
	{
	//	if(nClickIndex == Field_F4)
	//		_gContextManager->ShowBookmarkWhenStatusbarClick();
	//	else
	//		ExecuteStatusBarClick(m_defaultMap, nClickIndex);

	}
}

void CNextMStatusBar::ExecuteStatusBarClick(const std::unordered_map<int, wxString>& _statusMap, int nIndex)
{
	std::unordered_map<int, wxString>::const_iterator fIter = _statusMap.find(nIndex);
	if(fIter != _statusMap.end())
	{
//		int nMenuID = XRCID(fIter->second);
//		_gMenuEvent->DoMenuOperation(nMenuID);
	}
}
