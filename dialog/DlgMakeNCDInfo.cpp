#include "../ginc.h"
#include "DlgMakeNCDInfo.h"

wxBEGIN_EVENT_TABLE(DlgMakeNCDInfo, wxDialog)
	EVT_THREAD(SCD_MAKE_TREE_INFO_THREAD_EVENT, DlgMakeNCDInfo::OnMakeTreeInfoThreadEvent)
wxEND_EVENT_TABLE()

DlgMakeNCDInfo::DlgMakeNCDInfo(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style)
	, m_pParent(parent)
	, m_bClose(false)
{
	SetIcon(wxIcon("wxwin"));
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	m_staticText1 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_READING_DIRECTORYINFO")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizer1->Add(m_staticText1, 0, wxALIGN_CENTER|wxALL, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	m_btnCancel = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CANCEL")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnCancel->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_BUTTON));
	bSizer2->Add(m_btnCancel, 0, wxALL, 5);


	bSizer1->Add(bSizer2, 1, wxALIGN_RIGHT, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgMakeNCDInfo::OnClose));
	this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgMakeNCDInfo::OnInitDialog));
	m_btnCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgMakeNCDInfo::OnBtnCancelClick), NULL, this);
}

DlgMakeNCDInfo::~DlgMakeNCDInfo()
{
	// Disconnect Events
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgMakeNCDInfo::OnClose));
	this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgMakeNCDInfo::OnInitDialog));
	m_btnCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgMakeNCDInfo::OnBtnCancelClick), NULL, this);
}

void DlgMakeNCDInfo::OnInitDialog(wxInitDialogEvent& event)
{
	m_bClose = false;
}

void DlgMakeNCDInfo::OnClose(wxCloseEvent& event)
{
	m_bClose = true;
}

void DlgMakeNCDInfo::OnMakeTreeInfoThreadEvent(wxThreadEvent& event)
{
	int iNum = event.GetInt();
	if (iNum == -1)
	{
		wxMilliSleep(100);
		EndDialog(wxID_CLOSE);
	}
}

void DlgMakeNCDInfo::OnBtnCancelClick(wxCommandEvent& event)
{
	m_bClose = true;
}
