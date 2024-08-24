#include "../ginc.h"
#include "DlgExtModify.h"

DlgExtModify::DlgExtModify(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	SetIcon(wxIcon("wxwin"));
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxVERTICAL);

	m_staticText6 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_EXT_EDIT_DLG")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText6->Wrap(-1);
	bSizer8->Add(m_staticText6, 0, wxALL, 5);

	m_txtExts = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer8->Add(m_txtExts, 0, wxALL | wxEXPAND, 5);


	bSizer7->Add(bSizer8, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxHORIZONTAL);

	m_btnEditOk = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CONFIRM")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnEditOk->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_MENU));
	bSizer9->Add(m_btnEditOk, 0, wxALL, 5);

	m_btnCancel = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CANCEL")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnCancel->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU));
	bSizer9->Add(m_btnCancel, 0, wxALL, 5);


	bSizer7->Add(bSizer9, 0, wxALIGN_RIGHT, 5);


	this->SetSizer(bSizer7);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgExtModify::OnDialogClose));
	this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgExtModify::OnInitDialog));
	m_btnEditOk->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgExtModify::OnEditOKClick), NULL, this);
	m_btnCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgExtModify::OnCancelClick), NULL, this);
}

DlgExtModify::~DlgExtModify()
{
	// Disconnect Events
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgExtModify::OnDialogClose));
	this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgExtModify::OnInitDialog));
	m_btnEditOk->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgExtModify::OnEditOKClick), NULL, this);
	m_btnCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgExtModify::OnCancelClick), NULL, this);

}

void DlgExtModify::OnDialogClose(wxCloseEvent& event)
{
	EndDialog(wxID_CANCEL);
}

void DlgExtModify::OnInitDialog(wxInitDialogEvent& event)
{
	m_txtExts->SetLabelText(m_strExts);
	m_txtExts->SetSelection(0, m_strExts.Len());
}

void DlgExtModify::OnEditOKClick(wxCommandEvent& event)
{
	m_strExts = m_txtExts->GetLineText(1);
	EndDialog(wxID_OK);
}

void DlgExtModify::OnCancelClick(wxCommandEvent& event)
{
	EndDialog(wxID_CANCEL);
}
