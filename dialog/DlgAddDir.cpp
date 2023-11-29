///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../ginc.h"
#include "DlgAddDir.h"

///////////////////////////////////////////////////////////////////////////

DlgAddDir::DlgAddDir(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
			: wxDialog(parent, id, title, pos, size, style), m_pParent(parent)
{
	SetIcon(wxIcon("wxwin"));
	CenterOnScreen();

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_bMainSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	m_staticDontUse = new wxStaticText( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_INFO_RENAME_DONTUSE")) + theMsg->GetMessage(wxT("MSG_INFO_RENAME_DONTUSE_STRING")) , wxDefaultPosition, wxDefaultSize, 0 );
	m_staticDontUse->Wrap( -1 );
	bSizer12->Add( m_staticDontUse, 0, wxALIGN_CENTER|wxLEFT, 5 );
	m_bMainSizer->Add( bSizer12, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	m_bmpNewDir = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap(wxART_NEW_DIR), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer11->Add( m_bmpNewDir, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	bSizer14->SetMinSize( wxSize( 30,-1 ) );
	m_staticName = new wxStaticText( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DIR_NAME")) + wxT(" : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticName->Wrap( -1 );
	bSizer14->Add( m_staticName, 0, wxALIGN_CENTER|wxLEFT, 5 );

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	m_txtInput = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_txtInput, 0, wxEXPAND, 5 );
	bSizer14->Add( bSizer15, 1, wxALL, 5 );

	bSizer11->Add( bSizer14, 1, 0, 5 );

	m_bMainSizer->Add( bSizer11, 1, wxEXPAND, 5 );

	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_bMainSizer->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	m_sdbSizer2 = CreateStdDialogButtonSizer(wxOK | wxCANCEL);// wxStdDialogButtonSizer();
	m_bMainSizer->Add(m_sdbSizer2, 15, wxALL | wxEXPAND, 3);

	this->SetSizer( m_bMainSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgAddDir::OnInitDialog ) );
	this->Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgAddDir::OnOk), NULL, this);
	this->Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgAddDir::OnCancel), NULL, this);
}

DlgAddDir::~DlgAddDir()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgAddDir::OnInitDialog ) );
	this->Disconnect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgAddDir::OnOk), NULL, this);
	this->Disconnect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgAddDir::OnCancel), NULL, this);
}

void DlgAddDir::OnInitDialog(wxInitDialogEvent& event)
{
	m_txtInput->SetFocus();
}

void DlgAddDir::OnOk(wxCommandEvent& event)
{
	m_strNewName = m_txtInput->GetLineText(0);
	DoDialogProc(wxID_OK);
}

void DlgAddDir::OnCancel(wxCommandEvent& event)
{
	DoDialogProc(wxID_CANCEL);
}

void DlgAddDir::DoDialogProc(int nRetCode)
{
	if (nRetCode == wxID_OK)
	{
		if (m_strNewName.IsEmpty())
		{
			wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_ADD_DIR_INPUT")), PROGRAM_FULL_NAME, wxICON_INFORMATION | wxOK);
			m_txtInput->SetFocus();
			return;
		}

		if(!CLocalFileSystem::CheckCreatable(m_strNewName))
		{
			m_txtInput->SetFocus();
			return;
		}

		m_strNewPath = m_strParentPath + SLASH + m_strNewName;
		if(theUtility->Compare(m_strParentPath.Right(1), SLASH) == 0)
			m_strNewPath = m_strParentPath + m_strNewName;

		if (wxDirExists(m_strNewPath))
		{
			wxMessageBox(m_strNewPath + theMsg->GetMessage(wxT("MSG_DLG_ADD_DIR_EXIST")), PROGRAM_FULL_NAME, wxICON_ERROR | wxOK);
			return;
		}

		wxMkdir(m_strNewPath);
	}

	EndModal(nRetCode);
}

wxString DlgAddDir::GetNewPath() const
{
	return m_strNewPath;
}
