#include "../ginc.h"
#include "DlgAddExtColor.h"

DlgAddExtColor::DlgAddExtColor(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	SetIcon(wxIcon("wxwin"));
	CenterOnScreen();

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer46;
	bSizer46 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText28 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_ADD_EXT_COLOR_DLG_EXT_ADD")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText28->Wrap(-1);
	bSizer46->Add(m_staticText28, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_txtExt = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CAPITALIZE);
	bSizer46->Add(m_txtExt, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_colourExt = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer46->Add(m_colourExt, 0, wxALL, 5);


	bSizer44->Add(bSizer46, 1, wxEXPAND, 5);


	bSizer39->Add(bSizer44, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer(wxHORIZONTAL);

	m_btnAdd = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_ADD_EXT_COLOR_DLG_ADD")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnAdd->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
	bSizer45->Add(m_btnAdd, 0, wxALL, 5);

	m_btnCancel = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CANCEL")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnCancel->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU));
	bSizer45->Add(m_btnCancel, 0, wxALL, 5);


	bSizer39->Add(bSizer45, 0, wxALIGN_RIGHT, 5);


	this->SetSizer(bSizer39);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgAddExtColor::OnDialogClose));
	this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgAddExtColor::OnInitDialog));
	m_btnAdd->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgAddExtColor::OnAddClick), NULL, this);
	m_btnCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgAddExtColor::OnCancelClick), NULL, this);
}

DlgAddExtColor::~DlgAddExtColor()
{
	// Disconnect Events
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgAddExtColor::OnDialogClose));
	this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgAddExtColor::OnInitDialog));
	m_btnAdd->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgAddExtColor::OnAddClick), NULL, this);
	m_btnCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgAddExtColor::OnCancelClick), NULL, this);

}

void DlgAddExtColor::OnInitDialog(wxInitDialogEvent& event)
{
	m_txtExt->SetFocus();
}

void DlgAddExtColor::OnDialogClose(wxCloseEvent& event)
{
	EndDialog(wxID_CANCEL);
}

void DlgAddExtColor::OnAddClick(wxCommandEvent& event)
{
	m_strRegExt = m_txtExt->GetLineText(1).MakeUpper();
	if (m_strRegExt.IsEmpty())
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_ADD_EXT_COLOR_DLG_EXT_ADD_MSG")), PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
		m_txtExt->SetFocus();
	}

	m_colReg = m_colourExt->GetColour();
	EndDialog(wxID_OK);
}

void DlgAddExtColor::OnCancelClick(wxCommandEvent& event)
{
	EndDialog(wxID_CANCEL);
}
