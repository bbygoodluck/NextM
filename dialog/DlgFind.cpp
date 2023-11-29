///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////
#include "../ginc.h"
#include "DlgFind.h"
#include "../engine/LocalFileSystem.h"
#include "../views/NextMViewManager.h"

#include <wx/regex.h>
///////////////////////////////////////////////////////////////////////////

DlgFind::DlgFind(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style),
	m_strPath(wxT("")),
	m_strRegEx(wxT("")),
	m_bSearchSubDir(false),
	m_bCancel(false),
	m_bClose(false),
	m_iResultCount(0)
{
	SetIcon(wxIcon("wxwin"));
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* m_bxSizerMain;
	m_bxSizerMain = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* m_bxSizerSource;
	m_bxSizerSource = new wxBoxSizer(wxHORIZONTAL);

	m_bmpFolder = new wxStaticBitmap(this, wxID_ANY, wxArtProvider::GetBitmap(wxART_NEW_DIR, wxART_OTHER, wxSize(16, 15)), wxDefaultPosition, wxSize(16, 16), 0);
	m_bxSizerSource->Add(m_bmpFolder, 0, wxALL, 5);

	m_staticSrch = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_TITLE_DIR")), wxDefaultPosition, wxSize(55, -1), 0);
	m_staticSrch->Wrap(-1);
	m_bxSizerSource->Add(m_staticSrch, 0, wxALL, 5);

	m_txtSourcePath = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_bxSizerSource->Add(m_txtSourcePath, 1, wxALL, 2);

//	wxString strSLASHMsg = wxString::Format(theMsg->GetMessage(wxT("MSG_DLG_FIND_SLASH_MSG")), SLASH);
//	m_staticMSG = new wxStaticText(this, wxID_ANY, strSLASHMsg, wxDefaultPosition, wxDefaultSize, 0);
//	m_staticMSG->Wrap(-1);
//	m_bxSizerSource->Add(m_staticMSG, 0, wxALL, 5);

	m_bxSizerMain->Add(m_bxSizerSource, 0, wxEXPAND, 5);

	wxBoxSizer* m_bxSizerSearchTop;
	m_bxSizerSearchTop = new wxBoxSizer(wxHORIZONTAL);

	m_bmpFindName = new wxStaticBitmap(this, wxID_ANY, wxArtProvider::GetBitmap(wxART_TIP, wxART_OTHER, wxSize(16, 15)), wxDefaultPosition, wxSize(16, 16), 0);
	m_bxSizerSearchTop->Add(m_bmpFindName, 0, wxALL, 5);

	m_staticSearchTitle = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_NAME")), wxDefaultPosition, wxSize(55, -1), 0);
	m_staticSearchTitle->Wrap(-1);
	m_bxSizerSearchTop->Add(m_staticSearchTitle, 0, wxALL, 5);

	m_txtSearchName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
	m_bxSizerSearchTop->Add(m_txtSearchName, 1, wxALL, 2);


	m_bxSizerMain->Add(m_bxSizerSearchTop, 0, wxEXPAND, 5);

	m_staticline1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	m_bxSizerMain->Add(m_staticline1, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer* m_bxSizerOption;
	m_bxSizerOption = new wxBoxSizer(wxHORIZONTAL);

	m_radioMatch = new wxRadioButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_INPUT_MATCH")), wxDefaultPosition, wxDefaultSize, 0);
	m_bxSizerOption->Add(m_radioMatch, 0, wxALL, 5);

	m_radioInclude = new wxRadioButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_INPUT_INC")), wxDefaultPosition, wxDefaultSize, 0);
	m_bxSizerOption->Add(m_radioInclude, 0, wxALL, 5);

	m_radioStart = new wxRadioButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_INPUT_START")), wxDefaultPosition, wxDefaultSize, 0);
	m_bxSizerOption->Add(m_radioStart, 0, wxALL, 5);

	m_radioRegex = new wxRadioButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_INPUT_REGEX")), wxDefaultPosition, wxDefaultSize, 0);
	m_bxSizerOption->Add(m_radioRegex, 0, wxALL, 5);

	m_chkIncSubDir = new wxCheckBox(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_SUBDIR")), wxDefaultPosition, wxDefaultSize, 0);
	m_chkIncSubDir->SetValue(true);

	m_bxSizerOption->Add(m_chkIncSubDir, 0, wxALL, 5);


	m_bxSizerMain->Add(m_bxSizerOption, 0, wxEXPAND, 5);

	wxBoxSizer* m_bxSizerButton;
	m_bxSizerButton = new wxBoxSizer(wxHORIZONTAL);

	m_btnSearch = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_GO")), wxDefaultPosition, wxDefaultSize, 0);
	m_bxSizerButton->Add(m_btnSearch, 0, wxALL, 5);

	m_btnCancel = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_CANCEL")), wxDefaultPosition, wxDefaultSize, 0);
	m_bxSizerButton->Add(m_btnCancel, 0, wxALL, 5);

	m_btnClose = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_CLOSE")), wxDefaultPosition, wxDefaultSize, 0);
	m_bxSizerButton->Add(m_btnClose, 0, wxALL, 5);


	m_bxSizerMain->Add(m_bxSizerButton, 0, wxALIGN_RIGHT, 5);

	m_staticline3 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	m_bxSizerMain->Add(m_staticline3, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer* m_bxSizerPathInfo;
	m_bxSizerPathInfo = new wxBoxSizer(wxHORIZONTAL);

	m_bmpSearchPos = new wxStaticBitmap(this, wxID_ANY, wxArtProvider::GetBitmap(wxART_FIND_AND_REPLACE, wxART_OTHER, wxSize(16, 15)), wxDefaultPosition, wxSize(16, 15), 0);
	m_bxSizerPathInfo->Add(m_bmpSearchPos, 0, wxALL, 5);

	m_staticPathTitle = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_POSITION")), wxDefaultPosition, wxSize(55, -1), 0);
	m_staticPathTitle->Wrap(-1);
	m_bxSizerPathInfo->Add(m_staticPathTitle, 0, wxALL, 5);

	m_staticTextPathInfo = new wxStaticText(this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticTextPathInfo->Wrap(-1);
	m_bxSizerPathInfo->Add(m_staticTextPathInfo, 1, wxALL, 5);

	m_bxSizerMain->Add(m_bxSizerPathInfo, 0, wxEXPAND, 5);

	wxBoxSizer* m_bxSizerSearchContent;
	m_bxSizerSearchContent = new wxBoxSizer(wxHORIZONTAL);

	m_bmpContent = new wxStaticBitmap(this, wxID_ANY, wxArtProvider::GetBitmap(wxART_LIST_VIEW, wxART_OTHER, wxSize(16, 15)), wxDefaultPosition, wxSize(16, 15), 0);
	m_bxSizerSearchContent->Add(m_bmpContent, 0, wxALL, 5);

	m_staticText8 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_CONTENT")), wxDefaultPosition, wxSize(55, -1), 0);
	m_staticText8->Wrap(-1);
	m_bxSizerSearchContent->Add(m_staticText8, 0, wxALL, 5);

	m_staticTextContent = new wxStaticText(this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticTextContent->Wrap(-1);
	m_bxSizerSearchContent->Add(m_staticTextContent, 1, wxALL, 5);


	m_bxSizerMain->Add(m_bxSizerSearchContent, 0, wxEXPAND, 5);

	wxBoxSizer* m_bxSizerResult;
	m_bxSizerResult = new wxBoxSizer(wxHORIZONTAL);

	m_bmpFindResult = new wxStaticBitmap(this, wxID_ANY, wxArtProvider::GetBitmap(wxART_PRINT, wxART_OTHER, wxSize(16, 15)), wxDefaultPosition, wxSize(16, 15), 0);
	m_bxSizerResult->Add(m_bmpFindResult, 0, wxALL, 5);

	m_staticText81 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_RESULT")), wxDefaultPosition, wxSize(55, -1), 0);
	m_staticText81->Wrap(-1);
	m_bxSizerResult->Add(m_staticText81, 0, wxALL, 5);

	m_staticFindResult = new wxStaticText(this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticFindResult->Wrap(-1);
	m_bxSizerResult->Add(m_staticFindResult, 1, wxALL, 5);


	m_bxSizerMain->Add(m_bxSizerResult, 0, wxEXPAND, 5);

	m_staticline2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	m_bxSizerMain->Add(m_staticline2, 0, wxEXPAND | wxALL, 5);

	m_lstFindResult = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	m_bxSizerMain->Add(m_lstFindResult, 1, wxALL | wxEXPAND, 5);


	this->SetSizer(m_bxSizerMain);
	this->Layout();

	this->Centre(wxBOTH);


	this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgFind::OnInitDialog));
	this->Connect(wxEVT_CHAR_HOOK, wxKeyEventHandler(DlgFind::OnKeyDown));
	m_btnSearch->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFind::OnSearch), NULL, this);
	m_btnCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFind::OnCancel), NULL, this);
	m_btnClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFind::OnClose), NULL, this);
	m_lstFindResult->Connect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(DlgFind::OnLBoxDbClick), NULL, this);
}

DlgFind::~DlgFind()
{
	this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgFind::OnInitDialog));
	this->Disconnect(wxEVT_CHAR_HOOK, wxKeyEventHandler(DlgFind::OnKeyDown));
	m_btnSearch->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFind::OnSearch), NULL, this);
	m_btnCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFind::OnCancel), NULL, this);
	m_btnClose->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFind::OnClose), NULL, this);
	m_lstFindResult->Disconnect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(DlgFind::OnLBoxDbClick), NULL, this);
}

void DlgFind::OnInitDialog(wxInitDialogEvent& event)
{
	m_txtSourcePath->SetLabelText(m_strPath);
	m_radioStart->SetValue(true);
	m_btnCancel->Enable(false);
	m_staticTextPathInfo->SetLabelText(wxT(""));
	m_staticTextContent->SetLabelText(wxT(""));
	m_staticFindResult->SetLabelText(wxString::Format(wxT("%d"), m_iResultCount) + theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_COUNT")));

	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
	{
		wxMessageBox(wxT("Could not create the worker thread!"), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR, this);
		return;
	}

	GetThread()->Run();

	m_txtSearchName->SetFocus();
}

void DlgFind::OnKeyDown(wxKeyEvent& event)
{
	ProcessKeyEvent(event.GetKeyCode());
	event.Skip();
}

void DlgFind::ProcessKeyEvent(int iKeyCode)
{
	switch(iKeyCode)
	{
		case WXK_ESCAPE:
		{
			if(m_IsRunning)
			{
				DoCancel();
				return;
			}

			DoClose();
		}
			break;

		case WXK_RETURN:
			DoSearchStart();
			break;

		default:
			break;

	}
}

void DlgFind::setSearchDir(wxString const& strPath)
{
	m_strPath = strPath;
}

void DlgFind::DoSearchStart()
{
	m_IsRunning = true;
	this->SetFocus();
	wxString strSearchName = m_txtSearchName->GetValue();
#ifdef __WXMSW__
	//윈도우는 대소문자를 따로 구분하지 않음
	strSearchName.MakeLower();
#endif

	if (strSearchName.IsEmpty())
	{
		theUtility->ShowMessageBox(theMsg->GetMessage(wxT("MSG_DLG_FIND_INPUT_SEARCHWORD")), wxICON_INFORMATION | wxOK);
		m_txtSearchName->SetFocus();
		return;
	}

	wxString strRegEx(wxT(".*"));
	//일치값 및 정규식 검색
	if (m_radioMatch->GetValue() || m_radioRegex->GetValue())
		m_strRegEx = strSearchName;

	//입력문자 포함
	if (m_radioInclude->GetValue())
		m_strRegEx = strSearchName + strRegEx;

	//입력문자로 시작
	if (m_radioStart->GetValue())
		m_strRegEx = wxT("(^") + strSearchName + wxT(")");// +strRegEx;

	m_bSearchSubDir = m_chkIncSubDir->GetValue();
	m_lstFindResult->Clear();

	m_strPath = m_txtSourcePath->GetValue();
	m_txtSourcePath->Enable(false);
	m_txtSearchName->Enable(false);
	m_radioMatch->Enable(false);
	m_radioInclude->Enable(false);
	m_radioStart->Enable(false);
	m_radioRegex->Enable(false);
	m_chkIncSubDir->Enable(false);
	m_btnSearch->Enable(false);
	m_btnCancel->Enable(true);
	m_btnClose->Enable(false);

	m_iResultCount = 0;
	m_Lock.getCondition()->Signal();
}


void DlgFind::OnSearch(wxCommandEvent& event)
{
	DoSearchStart();
}

void DlgFind::OnCancel(wxCommandEvent& event)
{
	DoCancel();
}

void DlgFind::DoCancel()
{
	m_bCancel = true;
	m_txtSourcePath->Enable(true);
	m_txtSearchName->Enable(true);
	m_radioMatch->Enable(true);
	m_radioInclude->Enable(true);
	m_radioStart->Enable(true);
	m_chkIncSubDir->Enable(true);
	m_radioRegex->Enable(true);
	m_btnSearch->Enable(true);
	m_btnCancel->Enable(false);
	m_btnClose->Enable(true);
}

void DlgFind::OnClose(wxCommandEvent& event)
{
	DoClose();
}

void DlgFind::DoClose()
{
	m_bClose = true;
	m_Lock.getCondition()->Signal();

	if (GetThread() && GetThread()->IsRunning())
		GetThread()->Wait();

	EndDialog(wxID_CLOSE);
}

void DlgFind::OnLBoxDbClick(wxCommandEvent& event)
{
//	int iSelPos = event.GetInt();
	wxString strSelItem = event.GetString();
	//파일인경우
	if(wxFileExists(strSelItem))
		strSelItem = theUtility->GetParentPath(strSelItem);

	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strSelItem, CHANGE_DIR_TAB);
	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strSelItem, CHANGE_DIR_PATHVIEW);
	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strSelItem, CHANGE_DIR_FILELISTIVEW);

	//선택된 아이템으로 이동
//	wxCommandEvent evt(wxEVT_CHANGE_DIRECTORY);
//	evt.SetString(strSelItem);
//	evt.SetInt(SEND_PATH_TO_FILELIST);

//	wxWindow* pActivateViewPanel = _gContextManager->GetActivatePanel(_gActivatedTab);
//	wxPostEvent(pActivateViewPanel, evt);
}

wxThread::ExitCode DlgFind::Entry()
{
	while (true)
	{
		m_Lock.getCondition()->Wait();
		if (m_bClose)
			break;

		SearchDirectory(m_strPath);

		m_bCancel = false;
		m_IsRunning = false;

		m_txtSourcePath->Enable(true);
		m_txtSearchName->Enable(true);
		m_radioMatch->Enable(true);
		m_radioInclude->Enable(true);
		m_radioStart->Enable(true);
		m_chkIncSubDir->Enable(true);
		m_radioRegex->Enable(true);
		m_btnSearch->Enable(true);
		m_btnCancel->Enable(false);
		m_btnClose->Enable(true);
	}

	return (wxThread::ExitCode)0;
}

void DlgFind::SearchDirectory(const wxString& strPath)
{
	wxString strReadDir(strPath);
	m_staticTextPathInfo->SetLabelText(strReadDir);
	CLocalFileSystem localFileSys;

	if (!localFileSys.BeginFindFiles(strReadDir, false))
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_SEARCH_DIR_INIT_ERR")), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return;
	}

	wxRegEx re(m_strRegEx);

	wxString strName(wxT(""));
	wxString strMatchName(wxT(""));

	bool isDir = false;
	unsigned long lattr = 0;

	while (localFileSys.GetNextFile(strName, &isDir, &lattr))//, &llSize, &dt))
	{
		if (m_bCancel)
		{
			localFileSys.EndFindFiles();
			return;
		}

		if (!theConfig->IsViewHiddenFile())
		{
			if (lattr & ATTR_HIDDEN)
				continue;
		}

		if (!theConfig->IsViewSystemFile())
		{
			if (lattr & ATTR_SYSTEM)
				continue;
		}

		if (std::find(_gVecIgnore.begin(), _gVecIgnore.end(), strName.ToStdString()) != _gVecIgnore.end())
			continue;

		m_staticTextContent->SetLabelText(strName);

		strMatchName = strName;

#ifdef __WXMSW__
		strMatchName.MakeLower();
#endif

		if (re.Matches(strMatchName))
		{
			m_lstFindResult->Append(strReadDir + SLASH + strName);
			m_lstFindResult->SetSelection(m_lstFindResult->GetCount() - 1);
			m_iResultCount++;
			m_staticFindResult->SetLabelText(wxString::Format(wxT("%d"), m_iResultCount) + theMsg->GetMessage(wxT("MSG_DLG_FIND_SEARCH_COUNT")));
		}

		if (lattr & ATTR_DIR)
		{
			if (m_bSearchSubDir)
				this->SearchDirectory(strReadDir + SLASH + strName);
		}
	}
}
