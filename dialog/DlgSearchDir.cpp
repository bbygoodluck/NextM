#include "../ginc.h"
#include "DlgSearchDir.h"

wxBEGIN_EVENT_TABLE(DlgSearchDir, wxDialog)
	EVT_THREAD(SCD_DIRECTORY_SEARCH_THREAD_EVENT, DlgSearchDir::OnDirSearchThreadEvent)
wxEND_EVENT_TABLE()

DlgSearchDir::DlgSearchDir(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	SetIcon(wxIcon("wxwin"));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	m_staticText3 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCHING_MSG")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText3->Wrap(-1);
	bSizer4->Add(m_staticText3, 0, wxALIGN_CENTER | wxALL, 5);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText4 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCHING_DIR")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	bSizer6->Add(m_staticText4, 0, wxLEFT, 5);

	m_staticTxtCurrentPath = new wxStaticText(this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_MIDDLE | wxST_NO_AUTORESIZE);
	m_staticTxtCurrentPath->Wrap(-1);
	bSizer6->Add(m_staticTxtCurrentPath, 1, 0, 5);


	bSizer4->Add(bSizer6, 1, wxEXPAND, 5);


	bSizer1->Add(bSizer4, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxVERTICAL);

	m_staticline2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	bSizer5->Add(m_staticline2, 0, wxEXPAND, 5);


	bSizer1->Add(bSizer5, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	m_staticCurrDir = new wxStaticText(this, wxID_ANY, wxT("dd"), wxDefaultPosition, wxSize(640, 23), wxALIGN_CENTER_HORIZONTAL);
	m_staticCurrDir->Wrap(-1);
	m_staticCurrDir->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Tahoma")));

	bSizer2->Add(m_staticCurrDir, 0, wxSHAPED | wxTOP, 5);

	m_staticDirCount = new wxStaticText(this, wxID_ANY, wxT("Count"), wxDefaultPosition, wxSize(640, 23), wxALIGN_CENTER_HORIZONTAL);
	m_staticDirCount->Wrap(-1);
	bSizer2->Add(m_staticDirCount, 0, wxSHAPED, 5);


	bSizer1->Add(bSizer2, 0, wxSHAPED, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_btnCancel = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CANCEL")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnCancel->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU));
	bSizer3->Add(m_btnCancel, 1, 0, 5);


	bSizer1->Add(bSizer3, 1, wxALIGN_RIGHT | wxALL, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);


	// Connect Events
	this->Connect(wxEVT_CHAR_HOOK, wxKeyEventHandler(DlgSearchDir::OnCharHook));
	this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgSearchDir::OnInitDialog));
	m_btnCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgSearchDir::OnBtnCancel), NULL, this);
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgSearchDir::OnDlgClose));
}

DlgSearchDir::~DlgSearchDir()
{
	// Disconnect Events
	this->Disconnect(wxEVT_CHAR_HOOK, wxKeyEventHandler(DlgSearchDir::OnCharHook));
	this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgSearchDir::OnInitDialog));
	m_btnCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgSearchDir::OnBtnCancel), NULL, this);
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgSearchDir::OnDlgClose));
}

void DlgSearchDir::OnDlgClose(wxCloseEvent& event)
{
	m_bCancel = true;
	wxString strMsg = m_strDrive + theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCHING_CANCEL_MSG"));
	m_staticText3->SetLabelText(strMsg);
	m_staticText3->SetAutoLayout(true);
}

void DlgSearchDir::OnInitDialog(wxInitDialogEvent& event)
{
}

void DlgSearchDir::SetCurrentDrive(const wxString& strDriveName)
{
	m_strDrive = strDriveName;
	wxString strMsg = m_strDrive + theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCHING_DRV_MSG"));
	m_staticText3->SetLabelText(strMsg);
}

void DlgSearchDir::OnBtnCancel(wxCommandEvent& event)
{
	m_bCancel = true;
	wxString strMsg = m_strDrive + theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCHING_CANCEL_MSG"));
	m_staticText3->SetLabelText(strMsg);
	m_staticText3->SetAutoLayout(true);
}

void DlgSearchDir::SetReadDirectory(int iDirCount, const wxString& strReadPath)
{
	m_staticDirCount->SetLabelText(wxString::Format(wxT("%d"), iDirCount));
	m_staticDirCount->SetAutoLayout(true);

	m_staticCurrDir->SetLabelText(strReadPath);
	m_staticCurrDir->SetAutoLayout(true);
}

void DlgSearchDir::OnDirSearchThreadEvent(wxThreadEvent& event)
{
	int iNum = event.GetInt();
	if (iNum == -1)
	{
		wxMilliSleep(100);
		Destroy();
	}
}

void DlgSearchDir::SetCurrentDir(const wxString& strCurrPath)
{
	m_staticTxtCurrentPath->SetLabelText(strCurrPath);
}

void DlgSearchDir::OnCharHook(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_ESCAPE)
	{
		m_bCancel = true;
		wxString strMsg = m_strDrive + theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCHING_CANCEL_MSG"));
		m_staticText3->SetLabelText(strMsg);
		m_staticText3->SetAutoLayout(true);
	}

	event.Skip();
}
