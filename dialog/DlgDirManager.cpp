#include "../ginc.h"
#include "../utility/thread/DirSearchThread.h"
#include "../utility/thread/NCDInfoThread.h"
#include "../views/NextMNCDView.h"
//#include "../engine/LocalFileSystem.h"

#include "DlgSearchDir.h"
#include "DlgMakeNCDInfo.h"
#include "DlgNCDFind.h"
#include "DlgDirManager.h"

#include <iostream>
#include <fstream>
#include <streambuf>

DlgDirManager::DlgDirManager(wxWindow* parent,
									wxWindowID id,
									const wxString& title,
									const wxPoint& pos,
									const wxSize& size,
									long style)
	: wxDialog(parent, id, title, pos, wxSize(theConfig->GetWidth() - 50, theConfig->GetHeight() - 50), style)
{
	SetIcon(wxIcon("wxwin"));
	CenterOnScreen();

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxHORIZONTAL);

	m_scdToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_PLAIN_BACKGROUND | wxAUI_TB_HORZ_TEXT);
	m_toolRefreshAll = m_scdToolBar->AddTool(wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_SCD_REFRESH_ALL")), wxBitmap(theUtility->GetWorkDir() + SLASH + wxT("\\img\\refresh_11.bmp"), wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	m_toolRefreshDir = m_scdToolBar->AddTool(wxID_SCD_REFRESH, theMsg->GetMessage(wxT("MSG_DLG_SCD_REFRESH_DIR")), wxBitmap(theUtility->GetWorkDir() + SLASH + wxT("\\img\\refresh.bmp"), wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	m_scdToolBar->AddSeparator();

	m_toolAddFolder = m_scdToolBar->AddTool(wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_SCD_DIR_ADD")), wxArtProvider::GetBitmap(wxART_PLUS, wxART_BUTTON), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	m_toolRenameFolder = m_scdToolBar->AddTool(wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_SCD_DIR_RENAME")), wxArtProvider::GetBitmap(wxART_FIND_AND_REPLACE, wxART_BUTTON), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	m_toolDelete = m_scdToolBar->AddTool(wxID_DIR_DELETE, theMsg->GetMessage(wxT("MSG_DLG_SCD_DIR_DEL")), wxArtProvider::GetBitmap(wxART_CUT, wxART_BUTTON), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	m_scdToolBar->AddSeparator();

	m_toolFind = m_scdToolBar->AddTool(wxID_DIR_FIND, theMsg->GetMessage(wxT("MSG_DLG_SCD_DIR_FIND")), wxArtProvider::GetBitmap(wxART_FIND, wxART_BUTTON), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	m_toolFindNext = m_scdToolBar->AddTool(wxID_DIR_NEXT_FIND, theMsg->GetMessage(wxT("MSG_DLG_SCD_DIR_FIND_NEXT")), wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_BUTTON), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	m_scdToolBar->AddStretchSpacer();

	m_toolClose = m_scdToolBar->AddTool(wxID_DIR_MNG_CLOSE, theMsg->GetMessage(wxT("MSG_DLG_SCD_EXIT")), wxArtProvider::GetBitmap(wxART_QUIT, wxART_BUTTON), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	m_scdToolBar->Realize();

	bSizer6->Add(m_scdToolBar, 1, 0, 5);


	bSizer4->Add(bSizer6, 1, wxEXPAND, 5);


	bSizer1->Add(bSizer4, 0, wxEXPAND, 5);

	m_staticline1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	bSizer1->Add(m_staticline1, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText2 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_PATH")) + wxT(" "), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	bSizer2->Add(m_staticText2, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	m_staticText31 = new wxStaticText(this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText31->Wrap(-1);
	m_staticText31->SetFont(*_gViewFont);
	m_staticText31->SetForegroundColour(wxColour(80, 80, 80));

	bSizer2->Add(m_staticText31, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);


	bSizer1->Add(bSizer2, 0, wxBOTTOM | wxEXPAND | wxTOP, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_pNCDView = new CNextMNCDView(this, ++wxFILFELIST_VIEWID, wxDefaultSize, wxT(""));
	bSizer3->Add(m_pNCDView, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgDirManager::OnInitDialog));
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgDirManager::OnClose));

	// Connect Events
	this->Connect(m_toolRefreshAll->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnRefreshAll));
	this->Connect(m_toolRefreshDir->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnSCDRefreshDir));
	this->Connect(m_toolAddFolder->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnAddDir));
	this->Connect(m_toolRenameFolder->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnRenameDir));
	this->Connect(m_toolDelete->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnDeleteDir));
	this->Connect(m_toolFind->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnFindDir));
	this->Connect(m_toolFindNext->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnNextFindDir));
	this->Connect(m_toolClose->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnDirManagerClose));

	this->Maximize(true);
}

DlgDirManager::~DlgDirManager()
{
	theNCD->UnInitialize();

	if(m_pNCDView)
		wxDELETE(m_pNCDView);

	this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgDirManager::OnInitDialog));
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgDirManager::OnClose));

	// Disconnect Events
	this->Disconnect(m_toolRefreshAll->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnRefreshAll));
	this->Disconnect(m_toolRefreshDir->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnSCDRefreshDir));
	this->Disconnect(m_toolAddFolder->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnAddDir));
	this->Disconnect(m_toolRenameFolder->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnRenameDir));
	this->Disconnect(m_toolDelete->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnDeleteDir));
	this->Disconnect(m_toolFind->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnFindDir));
	this->Disconnect(m_toolFindNext->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnNextFindDir));
	this->Disconnect(m_toolClose->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(DlgDirManager::OnDirManagerClose));
}

void DlgDirManager::OnClose(wxCloseEvent& event)
{
	EndDialog(wxID_CLOSE);
}

void DlgDirManager::OnInitDialog(wxInitDialogEvent& event)
{
	LoadNCDData(true);
}

void DlgDirManager::SetNodePath(const wxString& strFullPath)
{
	m_staticText31->SetLabelText(strFullPath);
}

void DlgDirManager::LoadNCDData(bool bFirst, bool bSetFocus)
{
	m_pNCDView->Freeze();

	wxString strNCDFilePath(m_strNCDFilePath);
	wxString strCurrPath(m_strCurrPath);
	if (strNCDFilePath.IsEmpty() && strCurrPath.IsEmpty())
		return;

	if (!wxFileExists(strNCDFilePath))
	{
		DlgSearchDir dlg(_gMainFrame);

		CDirSearchThread* m_pDirSearchThread = new CDirSearchThread(&dlg);

		if (m_pDirSearchThread->Create() != wxTHREAD_NO_ERROR)
		{
			wxLogError(wxT("Can't create thread!"));
			return;
		}

		m_pDirSearchThread->SetSearchCurrentDirectory(strCurrPath, strNCDFilePath);
		m_pDirSearchThread->Run();

		dlg.ShowModal();
		dlg.Destroy();

		m_pDirSearchThread->Delete();
		wxDELETE(m_pDirSearchThread);
	}

	std::ifstream inFile;
	inFile.open(strNCDFilePath.char_str());

	size_t lineCount = std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n'); //inFile.GetLineCount();//

	theNCD->Initialize(lineCount);
	theNCD->SetNCDFile(m_strNCDFilePath);

	DlgMakeNCDInfo dlgMakeInfo(_gMainFrame);

	m_pNCDInfoThread = new CNCDInfoThread(&dlgMakeInfo);
	if (m_pNCDInfoThread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError(wxT("Can't create thread!"));
		delete m_pNCDInfoThread;
		return;
	}

	m_pNCDInfoThread->SetLoadFromFile(true);
	m_pNCDInfoThread->SetNCDFile(strNCDFilePath, strCurrPath, lineCount);
	m_pNCDInfoThread->Run();

	dlgMakeInfo.ShowModal();
	dlgMakeInfo.Destroy();

	m_pNCDInfoThread->Delete();

	delete m_pNCDInfoThread;
	m_pNCDInfoThread = nullptr;

	m_pNCDView->Thaw();
	m_pNCDView->LoadDirInfo();
	m_pNCDView->SetFocus();
}

void DlgDirManager::LoadSubDir(DirNode* pNode)
{
	m_pNCDView->Freeze();

	DlgMakeNCDInfo dlgMakeInfo(_gMainFrame);

	m_pNCDInfoThread = new CNCDInfoThread(&dlgMakeInfo);
	if (m_pNCDInfoThread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError(wxT("Can't create thread!"));
		delete m_pNCDInfoThread;
		return;
	}

	m_pNCDInfoThread->SetLoadFromFile(false);
	m_pNCDInfoThread->SetSubDirInfo(pNode);
	m_pNCDInfoThread->Run();

	dlgMakeInfo.ShowModal();
	dlgMakeInfo.Destroy();

	m_pNCDInfoThread->Delete();
	delete m_pNCDInfoThread;
	m_pNCDInfoThread = nullptr;

	m_pNCDView->Thaw();

	m_pNCDView->LoadDirInfo();
	m_pNCDView->SetFocus();
}

void DlgDirManager::OnRefreshAll(wxCommandEvent& event)
{
	RefreshAll();
}

void DlgDirManager::RefreshAll()
{
	m_strCurrPath = m_staticText31->GetLabelText();

	wxString strNCDFilePath(m_strNCDFilePath);

	if (wxFileExists(strNCDFilePath))
	{
		int iRet = 0;

		if (!wxRemoveFile(strNCDFilePath))
		{
			wxString strMsg(strNCDFilePath);
			strMsg += theMsg->GetMessage(wxT("MSG_DLG_SCD_FILE_DEL_ERR"));

			wxMessageBox(strMsg, PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
			return;
		}

		LoadNCDData(false, false);
	}
}

void DlgDirManager::OnDirManagerClose(wxCommandEvent& event)
{
	EndDialog(wxID_CLOSE);
}

void DlgDirManager::OnRenameDir(wxCommandEvent& event)
{
	m_pNCDView->RenameDir();
}

void DlgDirManager::OnDeleteDir(wxCommandEvent& event)
{
	m_pNCDView->DeleteDir();
}

void DlgDirManager::OnFindDir(wxCommandEvent& event)
{
	FindDir();
}

void DlgDirManager::FindDir()
{
	DlgNCDFind dlgFind(_gMainFrame);
	dlgFind.ShowModal();

	bool bMoveDir = dlgFind.IsMoveDir();
	dlgFind.Destroy();

	if (bMoveDir)
		EndDialog(wxID_CLOSE);
}

void DlgDirManager::OnNextFindDir(wxCommandEvent& event)
{
	m_pNCDView->NextFind();
}

void DlgDirManager::OnAddDir(wxCommandEvent& event)
{
	m_pNCDView->AddDir();
}

void DlgDirManager::OnSCDRefreshDir(wxCommandEvent& event)
{
	DirNode* pCurNode = m_pNCDView->GetCurNode();
	LoadSubDir(pCurNode);
}
