#include "../ginc.h"
#include "DlgEnv.h"
#include "DlgAddExtColor.h"
#include "DlgExtModify.h"

#include <wx/fontdlg.h>
#include <wx/colordlg.h>
DlgEnv::DlgEnv(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style)
{
	SetIcon(wxIcon("wxwin"));
	CenterOnScreen();

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxVERTICAL);

	m_tabBook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 | wxCLIP_CHILDREN | wxTAB_TRAVERSAL);
	m_tabBook->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	m_panelDefault = new wxPanel(m_tabBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxTAB_TRAVERSAL);
	m_panelDefault->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(m_panelDefault, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_DEFAULT"))), wxVERTICAL);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText1 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_ICON_PREREAD")), wxDefaultPosition, wxSize(-1, -1), 0);
	m_staticText1->Wrap(-1);
	bSizer5->Add(m_staticText1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_spinCtrlIcoPreLoad = new wxSpinCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxSize(50, -1), wxALIGN_CENTER_HORIZONTAL | wxALIGN_RIGHT | wxSP_ARROW_KEYS | wxBORDER_SIMPLE, 0, 100, 0);
	bSizer5->Add(m_spinCtrlIcoPreLoad, 0, 0, 5);


	sbSizer3->Add(bSizer5, 0, 0, 5);

	m_chkLastSession = new wxCheckBox(sbSizer3->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_SAVE_LAST_SESSION")), wxDefaultPosition, wxDefaultSize, 0);
	sbSizer3->Add(m_chkLastSession, 0, wxALL, 5);

	m_chkUseIME = new wxCheckBox(sbSizer3->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_SHIFT_DRV_USE_IN_IME")), wxDefaultPosition, wxDefaultSize, 0);
	sbSizer3->Add(m_chkUseIME, 0, wxALL, 5);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText2 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_FONT_NAME")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	bSizer8->Add(m_staticText2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_staticFontName = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxSize(150, -1), 0);
	m_staticFontName->Wrap(-1);
	bSizer8->Add(m_staticFontName, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_staticText4 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_FONT_SIZE")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	bSizer8->Add(m_staticText4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_spinFontSize = new wxSpinCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxSize(50, -1), wxALIGN_CENTER_HORIZONTAL | wxALIGN_RIGHT | wxSP_ARROW_KEYS | wxBORDER_SIMPLE, 0, 100, 0);
	bSizer8->Add(m_spinFontSize, 0, wxTOP, 5);

	m_btnFontSelect = new wxButton(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnFontSelect->SetBitmap(wxArtProvider::GetBitmap(wxART_FIND, wxART_MENU));
	bSizer8->Add(m_btnFontSelect, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_staticText21 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_FONT_VALIABLE_WIDTH_MSG")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText21->Wrap(-1);
	bSizer8->Add(m_staticText21, 1, wxALIGN_CENTER_VERTICAL, 5);


	bSizer7->Add(bSizer8, 1, wxEXPAND, 5);


	sbSizer3->Add(bSizer7, 0, wxEXPAND, 5);


	bSizer4->Add(sbSizer3, 0, wxEXPAND, 5);

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(m_panelDefault, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG"))), wxVERTICAL);

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText5 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_NAME")), wxDefaultPosition, wxSize(90, -1), 0);
	m_staticText5->Wrap(-1);
	bSizer101->Add(m_staticText5, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_txtExternalPGName = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer101->Add(m_txtExternalPGName, 1, wxALIGN_CENTER_VERTICAL, 5);


	sbSizer2->Add(bSizer101, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText6 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_PATH")), wxDefaultPosition, wxSize(90, -1), 0);
	m_staticText6->Wrap(-1);
	bSizer111->Add(m_staticText6, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_staticExternalPGPath = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticExternalPGPath->Wrap(-1);
	bSizer111->Add(m_staticExternalPGPath, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_btnFindExternlPG = new wxButton(sbSizer2->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_SEL")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnFindExternlPG->SetBitmap(wxArtProvider::GetBitmap(wxART_FIND, wxART_MENU));
	bSizer111->Add(m_btnFindExternlPG, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	sbSizer2->Add(bSizer111, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText8 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_ARGS")), wxDefaultPosition, wxSize(90, -1), 0);
	m_staticText8->Wrap(-1);
	bSizer12->Add(m_staticText8, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_txtArgs = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer12->Add(m_txtArgs, 1, wxALIGN_CENTER_VERTICAL, 5);


	sbSizer2->Add(bSizer12, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer(wxHORIZONTAL);

	m_btnAddExternalPG = new wxButton(sbSizer2->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_ADD")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnAddExternalPG->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
	bSizer14->Add(m_btnAddExternalPG, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_btnModExternalPG = new wxButton(sbSizer2->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_MODIFY")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnModExternalPG->SetBitmap(wxArtProvider::GetBitmap(wxART_FIND_AND_REPLACE, wxART_MENU));
	bSizer14->Add(m_btnModExternalPG, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_btnDelExternalPG = new wxButton(sbSizer2->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_DEL")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnDelExternalPG->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_MENU));
	bSizer14->Add(m_btnDelExternalPG, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


	sbSizer2->Add(bSizer14, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxHORIZONTAL );


	m_lstExternalPG = new wxListCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL | wxBORDER_SIMPLE);
	bSizer40->Add( m_lstExternalPG, 1, wxALL|wxEXPAND, 5 );

	m_spinSort = new wxSpinButton( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 20,50 ), 0 );
	bSizer40->Add( m_spinSort, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	bSizer13->Add(bSizer40, 1, wxALL | wxEXPAND, 5);

	sbSizer2->Add(bSizer13, 1, wxEXPAND, 5);


	bSizer4->Add(sbSizer2, 1, wxEXPAND, 5);


	m_panelDefault->SetSizer(bSizer4);
	m_panelDefault->Layout();
	bSizer4->Fit(m_panelDefault);
	m_tabBook->AddPage(m_panelDefault, theMsg->GetMessage(wxT("MSG_DLG_ENV_DEFAULT_COLOR_SET")), false);
	m_panelColor = new wxPanel(m_tabBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer131;
	bSizer131 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer(new wxStaticBox(m_panelColor, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_DEFAULT_COLOR"))), wxVERTICAL);

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText9 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_DEFAULT")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText9->Wrap(-1);
	bSizer15->Add(m_staticText9, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer(wxVERTICAL);

	m_colourDefault = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer23->Add(m_colourDefault, 0, wxALIGN_RIGHT, 5);


	bSizer15->Add(bSizer23, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer15, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer151;
	bSizer151 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText91 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DIR")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText91->Wrap(-1);
	bSizer151->Add(m_staticText91, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer(wxVERTICAL);

	m_colourDir = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer24->Add(m_colourDir, 0, wxALIGN_RIGHT, 5);


	bSizer151->Add(bSizer24, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer151, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText14 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_DEFAULT_DRIVE")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText14->Wrap(-1);
	bSizer22->Add(m_staticText14, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer(wxVERTICAL);

	m_colourDrive = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer25->Add(m_colourDrive, 0, wxALIGN_RIGHT, 5);


	bSizer22->Add(bSizer25, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer22, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer152;
	bSizer152 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText92 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_READONLY")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText92->Wrap(-1);
	bSizer152->Add(m_staticText92, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer231;
	bSizer231 = new wxBoxSizer(wxVERTICAL);

	m_colourReadOnly = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer231->Add(m_colourReadOnly, 0, wxALIGN_RIGHT, 5);


	bSizer152->Add(bSizer231, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer152, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer153;
	bSizer153 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText93 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_SELECT_ITEM")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText93->Wrap(-1);
	bSizer153->Add(m_staticText93, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer232;
	bSizer232 = new wxBoxSizer(wxVERTICAL);

	m_colourSelectItem = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer232->Add(m_colourSelectItem, 0, wxALIGN_RIGHT, 5);


	bSizer153->Add(bSizer232, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer153, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer154;
	bSizer154 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText94 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_BACKGROUND")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText94->Wrap(-1);
	bSizer154->Add(m_staticText94, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer233;
	bSizer233 = new wxBoxSizer(wxVERTICAL);

	m_colourBackGrnd = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer233->Add(m_colourBackGrnd, 0, wxALIGN_RIGHT, 5);


	bSizer154->Add(bSizer233, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer154, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer155;
	bSizer155 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText95 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_ATTRIBUTE")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText95->Wrap(-1);
	bSizer155->Add(m_staticText95, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer234;
	bSizer234 = new wxBoxSizer(wxVERTICAL);

	m_colourAttr = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer234->Add(m_colourAttr, 0, wxALIGN_RIGHT, 5);


	bSizer155->Add(bSizer234, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer155, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer156;
	bSizer156 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText96 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_DESCRIPTION")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText96->Wrap(-1);
	bSizer156->Add(m_staticText96, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer235;
	bSizer235 = new wxBoxSizer(wxVERTICAL);

	m_colourDesc = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer235->Add(m_colourDesc, 0, wxALIGN_RIGHT, 5);


	bSizer156->Add(bSizer235, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer156, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer157;
	bSizer157 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText97 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_COLUMN")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText97->Wrap(-1);
	bSizer157->Add(m_staticText97, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer236;
	bSizer236 = new wxBoxSizer(wxVERTICAL);

	m_colourColumn = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer236->Add(m_colourColumn, 0, wxALIGN_RIGHT, 5);


	bSizer157->Add(bSizer236, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer157, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer158;
	bSizer158 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText98 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_MATCHING_ITEM")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText98->Wrap(-1);
	bSizer158->Add(m_staticText98, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer237;
	bSizer237 = new wxBoxSizer(wxVERTICAL);

	m_colourMatch = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer237->Add(m_colourMatch, 0, wxALIGN_RIGHT, 5);


	bSizer158->Add(bSizer237, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer158, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer159;
	bSizer159 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText99 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_INFOWIN_TEXT_COL")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText99->Wrap(-1);
	bSizer159->Add(m_staticText99, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer238;
	bSizer238 = new wxBoxSizer(wxVERTICAL);

	m_colourInfoViewTxt = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer238->Add(m_colourInfoViewTxt, 0, wxALIGN_RIGHT, 5);


	bSizer159->Add(bSizer238, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer159, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer1510;
	bSizer1510 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText910 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_INFOWIN_BACK_COL")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText910->Wrap(-1);
	bSizer1510->Add(m_staticText910, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer* bSizer239;
	bSizer239 = new wxBoxSizer(wxVERTICAL);

	m_colourInfoViewBack = new wxColourPickerCtrl(sbSizer4->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	bSizer239->Add(m_colourInfoViewBack, 0, wxALIGN_RIGHT, 5);


	bSizer1510->Add(bSizer239, 1, wxEXPAND, 5);


	sbSizer4->Add(bSizer1510, 0, wxEXPAND, 5);


	bSizer141->Add(sbSizer4, 0, wxEXPAND, 5);

	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer(new wxStaticBox(m_panelColor, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_ENV_INFOWIN_EXT_COL"))), wxVERTICAL);

	m_lstExtColor = new wxListBox(sbSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_OWNERDRAW | wxCLIP_CHILDREN);
	sbSizer5->Add(m_lstExtColor, 1, wxALL | wxEXPAND, 5);

	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer(wxHORIZONTAL);

	m_btnAddExt = new wxButton(sbSizer5->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_ADD")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnAddExt->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU));
	bSizer39->Add(m_btnAddExt, 0, wxALL, 5);

	m_btnEdit = new wxButton(sbSizer5->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_EDIT")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnEdit->SetBitmap(wxArtProvider::GetBitmap(wxART_FIND_AND_REPLACE, wxART_MENU));
	bSizer39->Add(m_btnEdit, 0, wxALL, 5);

	m_btnExtDel = new wxButton(sbSizer5->GetStaticBox(), wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_DEL")), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	m_btnExtDel->SetBitmap(wxArtProvider::GetBitmap(wxART_DELETE, wxART_MENU));
	bSizer39->Add(m_btnExtDel, 0, wxALL, 5);


	sbSizer5->Add(bSizer39, 0, wxALIGN_CENTER, 5);


	bSizer141->Add(sbSizer5, 1, wxEXPAND, 5);


	bSizer131->Add(bSizer141, 1, wxALL | wxEXPAND, 5);


	m_panelColor->SetSizer(bSizer131);
	m_panelColor->Layout();
	bSizer131->Fit(m_panelColor);
	m_tabBook->AddPage(m_panelColor, theMsg->GetMessage(wxT("MSG_DLG_ENV_COLOR")), true);
	m_panelEtc = new wxPanel(m_tabBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxTAB_TRAVERSAL);
	m_tabBook->AddPage(m_panelEtc, theMsg->GetMessage(wxT("MSG_DLG_ENV_ETC")), false);

	bSizer9->Add(m_tabBook, 1, wxEXPAND | wxALL, 5);

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer(wxHORIZONTAL);

	m_btnCancel = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CANCEL")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnCancel->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU));
	bSizer11->Add(m_btnCancel, 0, wxALL, 5);

	m_btnApply = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_APPLY")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnApply->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));
	bSizer11->Add(m_btnApply, 0, wxALL, 5);

	m_btnClose = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CLOSE")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnClose->SetBitmap(wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU));
	bSizer11->Add(m_btnClose, 0, wxALL, 5);


	bSizer10->Add(bSizer11, 1, wxALIGN_RIGHT, 5);


	bSizer9->Add(bSizer10, 0, wxEXPAND, 5);


	this->SetSizer(bSizer9);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgEnv::OnDlgCloseClick));
	this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgEnv::OnInitDialog));

	m_btnFontSelect->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnFontFindClick), NULL, this);
	m_btnFindExternlPG->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExternalPG_FindClick), NULL, this);
	m_btnAddExternalPG->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExternalPG_AddClick), NULL, this);
	m_btnModExternalPG->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExternalPG_ModClick), NULL, this);
	m_btnDelExternalPG->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExternalPG_DelClick), NULL, this);
	m_lstExternalPG->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(DlgEnv::OnLstExtPGSelected), NULL, this);
	m_colourBackGrnd->Connect(wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler(DlgEnv::OnBackColorChanged), NULL, this);
	m_lstExtColor->Connect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(DlgEnv::OnListBoxDBClick), NULL, this);
	m_btnAddExt->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExtAddClick), NULL, this);
	m_btnEdit->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExtEditClick), NULL, this);
	m_btnExtDel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExtDelClick), NULL, this);
	m_btnCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnEnvCancelClick), NULL, this);
	m_btnApply->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnApplyClick), NULL, this);
	m_btnClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnCloseClick), NULL, this);

	m_spinSort->Connect( wxEVT_SPIN_DOWN, wxSpinEventHandler( DlgEnv::OnSpinDown ), NULL, this );
	m_spinSort->Connect( wxEVT_SPIN_UP, wxSpinEventHandler( DlgEnv::OnSpinUp ), NULL, this );
}

DlgEnv::~DlgEnv()
{
	// Disconnect Events
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgEnv::OnDlgCloseClick));
	this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgEnv::OnInitDialog));
	m_btnFontSelect->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnFontFindClick), NULL, this);
	m_btnFindExternlPG->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExternalPG_FindClick), NULL, this);
	m_btnAddExternalPG->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExternalPG_AddClick), NULL, this);
	m_btnModExternalPG->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExternalPG_ModClick), NULL, this);
	m_btnDelExternalPG->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExternalPG_DelClick), NULL, this);
	m_lstExternalPG->Disconnect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(DlgEnv::OnLstExtPGSelected), NULL, this);
	m_colourBackGrnd->Disconnect(wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler(DlgEnv::OnBackColorChanged), NULL, this);
	m_lstExtColor->Disconnect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(DlgEnv::OnListBoxDBClick), NULL, this);
	m_btnAddExt->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExtAddClick), NULL, this);
	m_btnEdit->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExtEditClick), NULL, this);
	m_btnExtDel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnExtDelClick), NULL, this);
	m_btnCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnEnvCancelClick), NULL, this);
	m_btnApply->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnApplyClick), NULL, this);
	m_btnClose->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgEnv::OnCloseClick), NULL, this);

	m_spinSort->Disconnect( wxEVT_SPIN_DOWN, wxSpinEventHandler( DlgEnv::OnSpinDown ), NULL, this );
	m_spinSort->Disconnect( wxEVT_SPIN_UP, wxSpinEventHandler( DlgEnv::OnSpinUp ), NULL, this );
}

void DlgEnv::OnDlgCloseClick(wxCloseEvent& event)
{
	EndDialog(wxID_CLOSE);
}

void DlgEnv::OnInitDialog(wxInitDialogEvent& event)
{
	m_tabBook->SetSelection(0);
	m_staticExternalPGPath->SetLabelText(wxT(""));

	m_chkUseIME->SetValue(theConfig->IsShift_IME_KOR_MoveDrive());
	m_chkLastSession->SetValue(theConfig->IsSaveLastSession());

	m_spinCtrlIcoPreLoad->SetMin(10);
	m_spinCtrlIcoPreLoad->SetMax(500);

	m_spinFontSize->SetMin(1);
	m_spinFontSize->SetMax(30);

	m_spinCtrlIcoPreLoad->SetValue(theConfig->GetPreIconReadCount());

	wxString strFontName = theConfig->GetFontName();
	wxString strFixedWidth = _gViewFont->IsFixedWidth() ? wxT(" (Fixed Width)") : wxT(" (Variable Width)");
	strFontName += strFixedWidth;

	m_staticFontName->SetLabelText(strFontName);
	m_spinFontSize->SetValue(theConfig->GetFontSize());

	//외부 편집 프로그램
	m_lstExternalPG->AppendColumn(theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_NAME_LIST")), wxLIST_FORMAT_LEFT, 120);
	m_lstExternalPG->AppendColumn(theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_PATH_LIST")), wxLIST_FORMAT_LEFT, 330);
	m_lstExternalPG->AppendColumn(theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_ARGS_LIST")), wxLIST_FORMAT_LEFT, 150);

	int iExtPGCount = theExternal->GetExternalProgCount();

	m_spinSort->SetRange(0, iExtPGCount - 1);
	m_spinSort->SetIncrement(1);

	std::vector<CExternalProgItem>::iterator iterExStart = theExternal->BeginIter();
	std::vector<CExternalProgItem>::iterator iterExEnd = theExternal->EndIter();

	int indexExternal = 0;
	while(iterExStart != iterExEnd)
	{
		CExternalProgItem exItem = *iterExStart;

		m_lstExternalPG->InsertItem(indexExternal, exItem._strPGName);
		m_lstExternalPG->SetItem(indexExternal, 1, exItem._strPGPath);
		m_lstExternalPG->SetItem(indexExternal, 2, exItem._strPGArgs);

		iterExStart++;
		indexExternal++;
	}

	//색상설정
	m_colourDefault->SetColour(theColor->GetDefaultColor());
	m_colourDir->SetColour(theColor->GetDirColor());
	m_colourDrive->SetColour(theColor->GetDriveColor());
	m_colourReadOnly->SetColour(theColor->GetReadOnlyColor());
	m_colourSelectItem->SetColour(theColor->GetSelectItemColor());
	m_colourBackGrnd->SetColour(theColor->GetBackgroundColor());
	m_colourAttr->SetColour(theColor->GetAttributeColor());
	m_colourDesc->SetColour(theColor->GetDescriptionColor());
	m_colourColumn->SetColour(theColor->GetColumnColor());
	m_colourMatch->SetColour(theColor->GetMatchColor());
	m_colourInfoViewTxt->SetColour(theColor->GetInfoViewTextColor());
	m_colourInfoViewBack->SetColour(theColor->GetInfoViewBackColor());

	m_lstExtColor->SetBackgroundColour(theColor->GetBackgroundColor());

	m_vecColorInfo.clear();

	Value& extVal = theColor->GetJsonDoc()["extcolor"];
	Value::ConstMemberIterator iter = extVal.MemberBegin();
	for (iter; iter != extVal.MemberEnd(); ++iter)
	{
		wxString strExt = iter->name.GetString();
		wxColour Col = theColor->GetExtColor(strExt);

		DoMakeColorInfo(Col, strExt);
	}

	DoSetExtColorInfoToList();
}

void DlgEnv::DoMakeColorInfo(const wxColour& col, const wxString& strExt)
{
	int iColRegCount = m_vecColorInfo.size();
	if (iColRegCount == 0)
	{
		COLORINFO colInfo;
		colInfo._col = col;
		colInfo._strExt.Append(strExt);

		m_vecColorInfo.push_back(colInfo);
	}
	else
	{
		bool bExist = false;
		for (int i = 0; i < iColRegCount; i++)
		{
			COLORINFO* pColInfo = (COLORINFO *)&m_vecColorInfo.at(i);
			if (pColInfo->_col == col)
			{
				pColInfo->_strExt += wxT(";");
				pColInfo->_strExt += strExt;
				bExist = true;
				break;
			}
		}

		if (!bExist)
		{
			COLORINFO colInfo;
			colInfo._col = col;
			colInfo._strExt.Append(strExt);

			m_vecColorInfo.push_back(colInfo);
		}
	}
}

void DlgEnv::DoSetExtColorInfoToList()
{
	int iColRegCount = m_vecColorInfo.size();
	for (int i = 0; i < iColRegCount; i++)
	{
		COLORINFO colInfo = m_vecColorInfo.at(i);
		m_lstExtColor->Insert(colInfo._strExt, i);
		m_lstExtColor->GetItem(i)->SetBackgroundColour(theColor->GetBackgroundColor());
		m_lstExtColor->GetItem(i)->SetTextColour(colInfo._col);
	}
}

void DlgEnv::OnLstExtPGSelected(wxListEvent& event)
{
	m_iExtPGSelected = event.GetIndex();
	wxString strPGName = m_lstExternalPG->GetItemText(m_iExtPGSelected, 0);
	wxString strPGPath = m_lstExternalPG->GetItemText(m_iExtPGSelected, 1);
	wxString strPGArgs = m_lstExternalPG->GetItemText(m_iExtPGSelected, 2);

	m_txtExternalPGName->SetLabelText(strPGName);
	m_staticExternalPGPath->SetLabelText(strPGPath);
	m_txtArgs->SetLabelText(strPGArgs);
}

void DlgEnv::OnFontFindClick(wxCommandEvent& event)
{
	wxFontData data;
	data.SetInitialFont(*_gViewFont);

	wxFontDialog dialog(this, data);
	if (dialog.ShowModal() == wxID_OK)
	{
		wxFontData retData = dialog.GetFontData();
		wxFont font = retData.GetChosenFont();;
		wxColour colour = retData.GetColour();

		wxString strFontName = font.GetFaceName();
		wxString strFixedWidth = font.IsFixedWidth() ? wxT(" (Fixed Width)") : wxT(" (Variable Width)");
		strFontName += strFixedWidth;

		m_staticFontName->SetLabelText(strFontName);
		m_spinFontSize->SetValue(font.GetPointSize());
	}
}

void DlgEnv::OnExternalPG_FindClick(wxCommandEvent& event)
{
	wxFileDialog dialog
	(
		this,
		theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_SEL")),
		wxEmptyString,
		wxEmptyString,
		"exe files (*.exe)|*.exe"
	);

	dialog.CentreOnParent();
	dialog.SetDirectory(wxGetHomeDir());

	if (dialog.ShowModal() == wxID_OK)
		m_staticExternalPGPath->SetLabelText(dialog.GetPath());
}

void DlgEnv::OnExternalPG_AddClick(wxCommandEvent& event)
{
	wxString strPGName = m_txtExternalPGName->GetValue();
	wxString strPGPath = m_staticExternalPGPath->GetLabelText();
	wxString strPGArgs = m_txtArgs->GetValue();

	if (strPGName.IsEmpty())
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_INPUT_MSG")), PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
		m_txtExternalPGName->SetFocus();
		return;
	}

	if (strPGPath.IsEmpty())
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_ENV_EXTERNAL_EDIT_PG_SEL_MSG")), PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
		m_btnFindExternlPG->SetFocus();
		return;
	}

	int iExtPGTotal = m_lstExternalPG->GetItemCount();

	for (int i = 0; i < iExtPGTotal; i++)
	{
		wxString strTmpName = m_lstExternalPG->GetItemText(i, 0);
		wxString strTmpPath = m_lstExternalPG->GetItemText(i, 1);

		if (strPGName.Trim().Cmp(strTmpName) == 0)
		{
			wxMessageBox(strPGName + wxT(" is already registered"), PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
			m_txtExternalPGName->SetFocus();
			return;
		}

		if (strPGPath.Cmp(strTmpPath) == 0)
		{
			wxMessageBox(wxT("A program like ") + strPGPath + wxT(" is already registred"), PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
			m_btnFindExternlPG->SetFocus();
			return;
		}
	}

	m_lstExternalPG->InsertItem(iExtPGTotal, strPGName);
	m_lstExternalPG->SetItem(iExtPGTotal, 1, strPGPath);
	m_lstExternalPG->SetItem(iExtPGTotal, 2, strPGArgs);
}

void DlgEnv::OnExternalPG_ModClick(wxCommandEvent& event)
{
	m_lstExternalPG->SetItem(m_iExtPGSelected, 0, m_txtExternalPGName->GetValue());
	m_lstExternalPG->SetItem(m_iExtPGSelected, 1, m_staticExternalPGPath->GetLabelText());
	m_lstExternalPG->SetItem(m_iExtPGSelected, 2, m_txtArgs->GetValue());

	m_lstExternalPG->SetItemState(m_iExtPGSelected, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void DlgEnv::OnExternalPG_DelClick(wxCommandEvent& event)
{
	m_lstExternalPG->DeleteItem(m_iExtPGSelected);
	m_iExtPGSelected = -1;

	m_txtExternalPGName->SetLabelText(wxT(""));
	m_staticExternalPGPath->SetLabelText(wxT(""));
	m_txtArgs->SetLabelText(wxT(""));
}

void DlgEnv::OnApplyClick(wxCommandEvent& event)
{
	DoApply();
	DoApplyColorInfo();

	theConfig->SaveDoc();
	theColor->SaveDoc();
	theExternal->SaveDoc();

	wxMessageBox(theMsg->GetMessage(wxT("MSG_ENV_CONFIG_SAVE")), PROGRAM_FULL_NAME,wxOK | wxICON_INFORMATION);
}

void DlgEnv::OnEnvCancelClick(wxCommandEvent& event)
{
	EndDialog(wxID_CANCEL);
}

void DlgEnv::OnCloseClick(wxCommandEvent& event)
{
	EndDialog(wxID_CLOSE);
}

void DlgEnv::DoApply()
{
	int iIME = m_chkUseIME->GetValue() == true ? 1 : 0;
	int iLastSession = m_chkLastSession->GetValue() == true ? 1 : 0;

	theConfig->SetChangeGlobalVal(wxT("Shift_IME_KOR_MoveDrv"), iIME);
	theConfig->SetChangeGlobalVal(wxT("LastSessionSave"), iLastSession);

	wxString strFontName = m_staticFontName->GetLabelText();
	int iPos = strFontName.Find(wxT("("));
	strFontName = strFontName.Left(iPos - 1);

	int iFontSize = m_spinFontSize->GetValue();

	wxString strBeforeFontName = _gViewFont->GetFaceName();
	if (strBeforeFontName.Cmp(strFontName) != 0)
	{
		theConfig->SetChangeGlobalVal(wxT("fontname"), strFontName);
		theConfig->SetChangeGlobalVal(wxT("fontsize"), iFontSize);

		wxDELETE(_gViewFont);

		_gViewFont = new wxFont(iFontSize, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, strFontName);
	}

	int iExtPGCount = m_lstExternalPG->GetItemCount();

	if (iExtPGCount > 0)
	{
		std::vector<CExternalProgItem> _vecPgs;

		for (int i = 0; i < iExtPGCount; i++)
		{
			CExternalProgItem exItem;
			exItem._strPGName = m_lstExternalPG->GetItemText(i, 0);
			exItem._strPGPath = m_lstExternalPG->GetItemText(i, 1);
			exItem._strPGArgs = m_lstExternalPG->GetItemText(i, 2);

			_vecPgs.emplace_back(exItem);
		}

		theExternal->ResetExternalProgram(_vecPgs);
	}
}

void DlgEnv::DoApplyColorInfo()
{
	wxString strDefaultCol = m_colourDefault->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strDirCol = m_colourDir->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strDriveCol = m_colourDrive->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strReadOnlyCol = m_colourReadOnly->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strSelItemCol = m_colourSelectItem->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strBackGrdCol = m_colourBackGrnd->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strAttrCol = m_colourAttr->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strDescCol = m_colourDesc->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strColumnCol = m_colourColumn->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strMatchCol = m_colourMatch->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strInfoViewTxtCol = m_colourInfoViewTxt->GetColour().GetAsString(wxC2S_HTML_SYNTAX);
	wxString strInfiViewBackCol = m_colourInfoViewBack->GetColour().GetAsString(wxC2S_HTML_SYNTAX);

	strDefaultCol.Replace(wxT("#"), wxT(""));
	strDirCol.Replace(wxT("#"), wxT(""));
	strDriveCol.Replace(wxT("#"), wxT(""));
	strReadOnlyCol.Replace(wxT("#"), wxT(""));
	strSelItemCol.Replace(wxT("#"), wxT(""));
	strBackGrdCol.Replace(wxT("#"), wxT(""));
	strAttrCol.Replace(wxT("#"), wxT(""));
	strDescCol.Replace(wxT("#"), wxT(""));
	strColumnCol.Replace(wxT("#"), wxT(""));
	strMatchCol.Replace(wxT("#"), wxT(""));
	strInfoViewTxtCol.Replace(wxT("#"), wxT(""));
	strInfiViewBackCol.Replace(wxT("#"), wxT(""));

	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("DefaultColor"), strDefaultCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("DirColor"), strDirCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("DrvColor"), strDriveCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("ReadOnlyColor"), strReadOnlyCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("SelectItemColor"), strSelItemCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("BackgroundColor"), strBackGrdCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("InfoViewBackColor"), strInfiViewBackCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("InfoViewTextColor"), strInfoViewTxtCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("AttributeColor"), strAttrCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("DescColor"), strDescCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("ColumnColor"), strColumnCol);
	theColor->SetChangeColorVal(wxT("defaultcolor"), wxT("matchColor"), strMatchCol);

	theColor->SetColors();

	int iLstCount = m_lstExtColor->GetCount();
	for (int i = 0; i < iLstCount; i++)
	{
		wxString strItem = m_lstExtColor->GetString(i);
		wxString strCol = m_lstExtColor->GetItem(i)->GetTextColour().GetAsString(wxC2S_HTML_SYNTAX);
		strCol.Replace(wxT("#"), wxT(""));

		DoApplyExtColor(strItem, strCol);
	}
}

void DlgEnv::DoApplyExtColor(const wxString& strExts, const wxString& strCol)
{
	wxString _strExts(strExts);
	wxStringTokenizer tokenizer(_strExts, wxT(";"));
	if (tokenizer.HasMoreTokens())
	{
		while (tokenizer.HasMoreTokens())
		{
			wxString strExt = tokenizer.GetNextToken();
			theColor->SetChangeColorVal(wxT("extcolor"), strExt.MakeUpper(), strCol);
		}
	}
	else
		theColor->SetChangeColorVal(wxT("extcolor"), _strExts.MakeUpper(), strCol);
}

void DlgEnv::OnListBoxDBClick(wxCommandEvent& event)
{
	int iSel = event.GetSelection();
	wxColour col = m_lstExtColor->GetItem(iSel)->GetTextColour();

	wxColourData colData;
	colData.SetColour(col);

	wxColourDialog dlgCol(this, &colData);

	if (dlgCol.ShowModal() == wxID_OK)
	{
		colData = dlgCol.GetColourData();
		col = colData.GetColour();
		m_lstExtColor->GetItem(iSel)->SetTextColour(col);

		m_lstExtColor->Select(-1);
	}
}

void DlgEnv::OnExtAddClick(wxCommandEvent& event)
{
	DlgAddExtColor dlg(this);
	int iRet = dlg.ShowModal();
	if (iRet == wxID_OK)
	{
		wxString strExt = dlg.GetAddExt();
		wxColour col = dlg.GetAddColour();

		int iRegCount = m_lstExtColor->GetCount();
		m_lstExtColor->Insert(strExt, iRegCount);

		m_lstExtColor->GetItem(iRegCount)->SetBackgroundColour(theColor->GetBackgroundColor());
		m_lstExtColor->GetItem(iRegCount)->SetTextColour(col);

		m_lstExtColor->Select(-1);
	}

	dlg.Destroy();
}

void DlgEnv::OnExtEditClick(wxCommandEvent& event)
{
	int iSel = m_lstExtColor->GetSelection();
	if (iSel == -1)
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_ENV_INPUT_EXT_EDIT_MSG")), PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
		return;
	}

	wxString strExts = m_lstExtColor->GetItem(iSel)->GetName();
	DlgExtModify dlg(this);
	dlg.SetExtInfo(strExts);

	if (dlg.ShowModal() == wxID_OK)
	{
		wxString strExts = dlg.GetExts();
		m_lstExtColor->SetString(iSel, strExts);
	}
}

void DlgEnv::OnExtDelClick(wxCommandEvent& event)
{
	int iSel = m_lstExtColor->GetSelection();
	if (iSel == -1)
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_ENV_DEL_ITEM_SEL_MSG")), PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
		return;
	}

	m_lstExtColor->Delete(iSel);
}

void DlgEnv::OnBackColorChanged(wxColourPickerEvent& event)
{
	wxColour colBack = event.GetColour();
	wxColour colTxt;

	m_lstExtColor->SetBackgroundColour(colBack);
	int iExtCount = m_lstExtColor->GetCount();

	for (int i = 0; i < iExtCount; i++)
	{
		colTxt = m_lstExtColor->GetItem(i)->GetTextColour();
		m_lstExtColor->GetItem(i)->SetBackgroundColour(colBack);
		m_lstExtColor->GetItem(i)->SetTextColour(colTxt);
	}

	m_lstExtColor->Refresh();
	m_lstExtColor->Update();
}

void DlgEnv::OnSpinDown( wxSpinEvent& event )
{
	DoExternalPGOrdering(false);
}

void DlgEnv::OnSpinUp( wxSpinEvent& event )
{
	DoExternalPGOrdering(true);
}

void DlgEnv::DoExternalPGOrdering(bool bMoveUp)
{
	int iItemCounts = m_lstExternalPG->GetItemCount();
	if(iItemCounts == 1 || m_iExtPGSelected > iItemCounts)
		return;

	if(bMoveUp && m_iExtPGSelected == 0)
		return;

	wxString strPGName = m_lstExternalPG->GetItemText(m_iExtPGSelected, 0);
	wxString strPGPath = m_lstExternalPG->GetItemText(m_iExtPGSelected, 1);
	wxString strPGArgs = m_lstExternalPG->GetItemText(m_iExtPGSelected, 2);

	m_lstExternalPG->DeleteItem(m_iExtPGSelected);
	if(bMoveUp)
		m_iExtPGSelected--;
	else
		m_iExtPGSelected++;

	m_lstExternalPG->InsertItem(m_iExtPGSelected, strPGName);
	m_lstExternalPG->SetItem(m_iExtPGSelected, 1, strPGPath);
	m_lstExternalPG->SetItem(m_iExtPGSelected, 2, strPGArgs);

	m_lstExternalPG->SetItemState(m_iExtPGSelected, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}
