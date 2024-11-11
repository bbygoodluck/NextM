///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////
#include "../ginc.h"
#include "DlgCompress.h"

///////////////////////////////////////////////////////////////////////////
DlgCompress::DlgCompress( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	SetIcon(wxIcon("wxwin"));

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	m_lstDatas = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES);

#ifdef __WXMSW__
	ListView_SetImageList(m_lstDatas->GetHWND(), _gImageList, LVSIL_SMALL);
#else
	m_lstDatas->SetImageList(_gImageList);
#endif // __WXMSW__


	bSizer7->Add( m_lstDatas, 1, wxALL|wxEXPAND, 5 );
	bSizer1->Add( bSizer7, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText6 = new wxStaticText( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_FILE")), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer9->Add( m_staticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticCompressedFile = new wxStaticText( this, wxID_ANY, wxT("C:\\aaa.zip"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticCompressedFile->Wrap( -1 );
	bSizer9->Add( m_staticCompressedFile, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText5 = new wxStaticText( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_TYPE")), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer8->Add( m_staticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxArrayString cmbCompressTypeChoices;
	std::vector<wxString> vCompressType = theComDec->GetCompressList();
	for(auto item : vCompressType)
		cmbCompressTypeChoices.Add(item);

	m_cmbCompressType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, cmbCompressTypeChoices, 0 );
	m_cmbCompressType->SetSelection( 0 );
	bSizer8->Add( m_cmbCompressType, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

	m_staticText8 = new wxStaticText( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_PASSWORD")), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer8->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_txtPassword = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	bSizer8->Add( m_txtPassword, 0, wxALL, 5 );

	bSizer9->Add( bSizer8, 0, wxEXPAND, 5 );


	bSizer1->Add( bSizer9, 0, wxEXPAND, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_staticCurrentTxt = new wxStaticText( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_PROGRESS")), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticCurrentTxt->Wrap( -1 );
	bSizer2->Add( m_staticCurrentTxt, 0, wxALL|wxEXPAND, 5 );

	m_progressComTotal = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,18 ), wxGA_HORIZONTAL );
	m_progressComTotal->SetValue( 0 );
	bSizer2->Add( m_progressComTotal, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_staticPercent = new wxStaticText( this, wxID_ANY, wxT("100%"), wxDefaultPosition, wxSize(29, -1), 0 );
	m_staticPercent->Wrap( -1 );
	bSizer6->Add( m_staticPercent, 1, 0, 5 );


	bSizer2->Add( bSizer6, 0, wxALIGN_RIGHT|wxALL, 5 );


	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_btnAddFolder = new wxButton( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_ADD_FOLDER")), wxDefaultPosition, wxSize( -1,30 ), 0 );

	m_btnAddFolder->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_FOLDER), wxASCII_STR(wxART_BUTTON) ) );
	bSizer3->Add( m_btnAddFolder, 0, wxALL|wxEXPAND, 5 );

	m_btnAdd = new wxButton( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_ADD_FILE")), wxDefaultPosition, wxSize( -1,30 ), 0 );
	m_btnAdd->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_FILE_OPEN), wxASCII_STR(wxART_BUTTON) ) );

	bSizer3->Add( m_btnAdd, 0, wxALL|wxEXPAND, 5 );

	m_btnCompressStart = new wxButton( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_TITLE")), wxDefaultPosition, wxSize( -1,30 ), 0 );
	m_btnCompressStart->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_NEW), wxASCII_STR(wxART_BUTTON) ) );

	bSizer3->Add( m_btnCompressStart, 0, wxALL|wxEXPAND, 5 );

	m_btnCancel = new wxButton( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_CANCEL")), wxDefaultPosition, wxSize( -1,30 ), 0 );
	m_btnCancel->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_CLOSE), wxASCII_STR(wxART_BUTTON) ) );

	bSizer3->Add( m_btnCancel, 0, wxALL, 5 );

	m_btnClose = new wxButton( this, wxID_ANY, theMsg->GetMessage(wxT("MSG_CLOSE")), wxDefaultPosition, wxSize( -1,30 ), 0);
	m_btnClose->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_QUIT), wxASCII_STR(wxART_BUTTON) ) );

	bSizer3->Add( m_btnClose, 0, wxALL|wxEXPAND, 5 );

	bSizer1->Add( bSizer3, 0, wxALIGN_RIGHT, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgCompress::OnInitDialog ) );
	m_cmbCompressType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DlgCompress::OnChoice ), NULL, this );
	m_btnAddFolder->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnAddFolder ), NULL, this );
	m_btnAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnBtnAdd ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnCancelClick ), NULL, this );
	m_btnCompressStart->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnBtnCompress ), NULL, this );
	m_btnClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnClose ), NULL, this );

	Bind(wxEVT_THREAD, &DlgCompress::OnCompressThreadEnd, this);
	CenterOnScreen();
}

DlgCompress::~DlgCompress()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgCompress::OnInitDialog ) );
	m_cmbCompressType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DlgCompress::OnChoice ), NULL, this );
	m_btnAddFolder->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnAddFolder ), NULL, this );
	m_btnAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnBtnAdd ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnCancelClick ), NULL, this );
	m_btnCompressStart->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnBtnCompress ), NULL, this );
	m_btnClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgCompress::OnClose ), NULL, this );

	Unbind(wxEVT_THREAD, &DlgCompress::OnCompressThreadEnd, this);
}

void DlgCompress::OnInitDialog( wxInitDialogEvent& event )
{
	m_progressComTotal->SetRange(100);
	if(theComDec->IsImmediatelyCompress())
	{
		m_btnCompressStart->Show(false);
		m_staticText8->Show(false);
		m_txtPassword->Show(false);
		m_btnAddFolder->Show(false);
		m_btnAdd->Show(false);
		m_btnClose->Show(false);
	}

	m_staticPercent->SetLabelText(wxT("0%"));
}

void DlgCompress::SetCompressInfo(const std::list<wxString>& vCompressList,
								  const wxString& strCompressedFile,
								  const wxString& strTargetDir,
								  const wxString& strCompressType)
{
	m_vCompressList     = vCompressList;
	m_strCompressedFile = strCompressedFile;
	m_strCompressType   = strCompressType;
	m_strTargetDir      = strTargetDir;

	m_lstDatas->DeleteAllItems();
	m_lstDatas->AppendColumn(theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_LIST_HEADER")), wxLIST_FORMAT_LEFT, 760);

	int iIndex = 0;
	for(auto item : m_vCompressList)
	{
		int iconIndex = 0;
		int overlayIndex = 0;

		theImageList->GetIconIndex(item, iconIndex, overlayIndex);

		m_lstDatas->InsertItem(iIndex, item);
		m_lstDatas->SetItemImage(iIndex, iconIndex);

		iIndex++;
	}

	m_iTotalItems = m_vCompressList.size();

	m_staticCompressedFile->SetLabelText(m_strCompressedFile);

	int iTypeCount = m_cmbCompressType->GetCount();
	int iCompressType = 0;
	for(int i = 0; i < iTypeCount; i++)
	{
		wxString strCompType = m_cmbCompressType->GetString(i);
		if(strCompType.CmpNoCase(m_strCompressType) == 0)
		{
			m_cmbCompressType->SetSelection(i);
			iCompressType = i;
			break;
		}
	}

	if(theComDec->IsImmediatelyCompress())
		DoCompress(iCompressType);
}

void DlgCompress::DoCompress(int iCompressType)
{
	wxString strPassword = m_txtPassword->GetLineText(0);

	theComDec->SetOwnerDialog(this);
	theComDec->SetItemsInfo(m_vCompressList, m_strCompressedFile, m_strTargetDir, iCompressType, strPassword);
	theComDec->Start();
}

void DlgCompress::OnChoice( wxCommandEvent& event )
{
	unsigned int iSel = event.GetSelection();

	wxString strExt[6] = {
		wxT("zip"),
		wxT("7z"),
		wxT("gz"),
		wxT("tar"),
		wxT("xz"),
		wxT("bzip2")
	};

	ResetCompressFile(strExt[iSel]);
}

void DlgCompress::ResetCompressFile(const wxString& strCompressExt)
{
	int nExtIndex = m_strCompressedFile.rfind('.');

	wxString strCompressPathName = m_strCompressedFile.Left(nExtIndex + 1);

	strCompressPathName += strCompressExt;
	m_strCompressedFile = strCompressPathName;

	m_staticCompressedFile->SetLabelText(m_strCompressedFile);
}

void DlgCompress::OnCancelClick( wxCommandEvent& event )
{
	theComDec->SetCancel(true);
}

void DlgCompress::OnClose( wxCommandEvent& event )
{
	EndModal(wxID_CLOSE);
}

void DlgCompress::SetCurrentFile(const wxString& strCurrentFile)
{
	wxString strCompressedMsg = theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_CURRENT_FILE"));
	strCompressedMsg += strCurrentFile;

	m_staticCurrentTxt->SetLabelText(strCompressedMsg);
}

void DlgCompress::SetCurrentFileSize(uint64_t ulSize)
{
	int iPercent = 0;
	if(ulSize > 0)
		iPercent = ((ulSize * 100) / m_ulTotalSize);

	m_progressComTotal->SetValue(iPercent);

	wxString strFile = wxString::Format(wxT("%3d%"), iPercent);
	m_staticPercent->SetLabelText(strFile);
}

void DlgCompress::SetCurrentFileTotalSize(uint64_t ulTotalSize)
{
	m_ulTotalSize = ulTotalSize;
}

void DlgCompress::OnCompressThreadEnd(wxThreadEvent& event)
{
//	theComDec->ClearCompressInfo();
	EndModal(wxID_OK);
}

void DlgCompress::OnBtnCompress( wxCommandEvent& event )
{
	DoCompress(0);
}

void DlgCompress::OnAddFolder( wxCommandEvent& event )
{
	wxDirDialog  dDlg(this, theMsg->GetMessage(wxT("MSG_DLG_ENV_SELECT_ITEM")), wxT(""), wxDD_MULTIPLE | wxDD_DIR_MUST_EXIST);

	if(dDlg.ShowModal() == wxID_OK)
	{
		wxArrayString arrItems;
		dDlg.GetPaths(arrItems);

		AddItems(arrItems);
	}
}

void DlgCompress::OnBtnAdd( wxCommandEvent& event )
{
	wxFileDialog fdlg(this,
					theMsg->GetMessage(wxT("MSG_DLG_ENV_SELECT_ITEM")),
					wxT(""),
					wxT(""),
                    wxT("*.*"), wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);

	if(fdlg.ShowModal() == wxID_OK)
	{
		wxArrayString arrItems;
		fdlg.GetPaths(arrItems);

		AddItems(arrItems);
	}
}

void DlgCompress::AddItems(const wxArrayString& arrItems)
{
	wxString strFilePathName(wxT(""));

	int lstCount = m_lstDatas->GetItemCount();
	int iIndex = lstCount;

	for(size_t i = 0; i < arrItems.Count(); i++)
	{
		strFilePathName = arrItems.Item(i);

		int iconIndex = 0;
		int overlayIndex = 0;

		theImageList->GetIconIndex(strFilePathName, iconIndex, overlayIndex);

		m_lstDatas->InsertItem(iIndex, strFilePathName);
		m_lstDatas->SetItemImage(iIndex, iconIndex);

		m_vCompressList.push_back(strFilePathName);

		iIndex++;
	}
}
