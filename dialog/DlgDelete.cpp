///////////////////////////////////////////////////////////////////////////
#include "../ginc.h"
#include "../engine/LocalFileSystem.h"
#include "DlgDelete.h"

///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(DlgDelete, wxDialog)
EVT_THREAD(FILE_OPERATION_DELETE_THREAD, DlgDelete::OnFileOperationDeleteEnd)
wxEND_EVENT_TABLE()

DlgDelete::DlgDelete(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	SetIcon(wxIcon("wxwin"));
	CenterOnScreen();

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	m_staticText1 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_DELETE_INFO")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizer1->Add(m_staticText1, 0, wxALL, 5);

	m_staticDelItem = new wxStaticText(this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticDelItem->Wrap(-1);
	bSizer1->Add(m_staticDelItem, 0, wxALL | wxEXPAND, 5);

	m_progressDel = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	m_progressDel->SetValue(0);
	bSizer1->Add(m_progressDel, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	m_staticDelCnt = new wxStaticText(this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticDelCnt->Wrap(-1);
	bSizer4->Add(m_staticDelCnt, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);


	bSizer3->Add(bSizer4, 1, wxEXPAND, 5);


	bSizer1->Add(bSizer3, 1, wxEXPAND | wxRIGHT, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	m_btnClose = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CLOSE")), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_btnClose, 0, wxALL, 5);

	m_btnCancel = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CANCEL")), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_btnCancel, 0, wxALL, 5);


	bSizer1->Add(bSizer2, 0, wxALIGN_RIGHT, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);
	// Connect Events
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgDelete::OnClose));
	this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgDelete::OnInitialog));
	m_btnClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgDelete::OnDlgClose), NULL, this);
	m_btnCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgDelete::OnDlgCancel), NULL, this);
}

DlgDelete::~DlgDelete()
{
	// Disconnect Events
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgDelete::OnClose));
	this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgDelete::OnInitialog));
	m_btnClose->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgDelete::OnDlgClose), NULL, this);
	m_btnCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgDelete::OnDlgCancel), NULL, this);

}

void DlgDelete::OnClose(wxCloseEvent& event)
{
	if (GetThread()->IsAlive() && GetThread()->IsRunning())
		GetThread()->Wait();

	Destroy();
}

void DlgDelete::OnInitialog(wxInitDialogEvent& event)
{
	m_bWorking = false;
	m_bCancelOK = false;
	m_bStopping = false;

	m_staticDelCnt->SetLabelText(wxT("(0 / 0)"));

	m_staticText1->SetLabelText(theMsg->GetMessage(wxT("MSG_DLG_DELETE_COLLECT_DEL_INFO")));
	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
	{
		wxLogError("Could not create the worker thread!");
		return;
	}
	if (GetThread()->Run() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Could not run the worker thread!");
		return;
	}

	m_btnClose->Enable(false);
	m_bWorking = true;
}

void DlgDelete::OnDlgClose(wxCommandEvent& event)
{
	EndDialog(wxID_CLOSE);
}

void DlgDelete::OnDlgCancel(wxCommandEvent& event)
{
	if (m_bWorking)
	{
		m_bStopping = true;

		int iRetID = wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_DELETE_CANCEL_MSG")), PROGRAM_FULL_NAME, wxYES_NO | wxICON_INFORMATION, this);
		if (iRetID == wxYES)
		{
			m_bCancelOK = true;
			m_btnClose->Enable(true);
			m_btnCancel->Enable(false);
		}

		if (!m_bCancelOK)
			m_bStopping = false;

		m_Lock.getCondition()->Signal();
	}

	if (m_bCancelOK)
		EndDialog(wxID_CANCEL);
	//	Destroy();
}

wxThread::ExitCode DlgDelete::Entry()
{
	m_arrSubLst.Clear();
	m_arrDirLst.Clear();

	m_bError = false;
	int iDelTotal = 0;
	int iDelCount = 0;
	if (m_lstDelList.size() > 0)
	{
		for(auto item : m_lstDelList)
		{
			if (m_bStopping)
				m_Lock.getCondition()->Wait();

			if (m_bCancelOK)
				break;

			if (wxDirExists(item))
			{
				path p(item.ToStdString());
				int iCount = GetNumberOf_Files_in_Directory(p);
				iDelTotal += iCount;

				m_arrDirLst.Add(item);
			}
			else
			{
				m_arrSubLst.Add(item);
				iDelTotal++;
			}
		}

		m_progressDel->SetRange(iDelTotal);
		if(!m_bStopping && !m_bCancelOK)
		{
			m_staticText1->SetLabelText(theMsg->GetMessage(wxT("MSG_DLG_DELETE_INFO")));

			for (auto sItem : m_arrSubLst)
			{
				if (m_bStopping)
					m_Lock.getCondition()->Wait();

				if (m_bCancelOK)
					break;

				m_staticDelItem->SetLabelText(sItem);

				int iRet = 0;
				bool bDel = theFileOperation->RecursiveDelete(sItem, this, m_bGoTrash, iRet);
				if (!bDel)
				{
					if(iRet == 0x0020)
						sItem += theMsg->GetMessage(wxT("MSG_DLG_DELETE_USE_ANOTHER_PG"));

					theUtility->ShowMessageBox(sItem, wxICON_ERROR | wxOK);
					m_bError = true;
					break;
				}

				iDelCount++;

				wxString strDisp = wxString::Format(wxT("(%d / %d)"), iDelCount, iDelTotal);
				m_staticDelCnt->SetLabelText(strDisp);
				m_progressDel->SetValue(iDelCount);
			}

			if (!m_bError)
			{
				if (!m_bStopping && !m_bCancelOK)
				{
					int iRet = 0;
					for (auto sDirItem : m_arrDirLst)
					{
						bool bDel = theFileOperation->RecursiveDelete(sDirItem, this, false, iRet);
						if (!bDel)
						{
							if(iRet == 0x0020)
								sDirItem += theMsg->GetMessage(wxT("MSG_DLG_DELETE_USE_ANOTHER_PG"));

							theUtility->ShowMessageBox(sDirItem, wxICON_ERROR | wxOK);
							m_bError = true;
							break;
						}
					}
				}
			}
		}
	}

	m_bWorking = false;
	m_bCancelOK = false;
	m_bStopping = false;

	wxThreadEvent event(wxEVT_THREAD, FILE_OPERATION_DELETE_THREAD);
	event.SetInt(-1);
	wxQueueEvent(this, event.Clone());

	return (wxThread::ExitCode)0;
}

void DlgDelete::OnFileOperationDeleteEnd(wxThreadEvent& event)
{
	int iNum = event.GetInt();
	if (iNum == -1)
	{
		wxMilliSleep(100);
		if (m_bError)
			EndDialog(wxID_ABORT);
		else
			EndDialog(wxID_CLOSE);
	}
	else
	{
		wxMilliSleep(100);
		EndDialog(wxID_CLOSE);
	}
}

int DlgDelete::GetNumberOf_Files_in_Directory(path p)
{
	//디렉토리 복사/이동시에 복사할 총 파일갯수를 구함
	int n = 0;
	for (recursive_directory_iterator it(p); it != recursive_directory_iterator{}; ++it)
	{
		wxString strPath(it->path().string());

#if defined(__WXMSW__)
		DWORD dwNum = GetFileAttributes(strPath);

		if (dwNum & FILE_ATTRIBUTE_DIRECTORY)
		{
			m_staticDelItem->SetLabelText(strPath + theMsg->GetMessage(wxT("MSG_DLG_DELETE_COLLECT_DEL_INFO_FOLDER")));
			continue;
		}

#else
#endif
		m_arrSubLst.Add(strPath);
		n++;
	}

	return n;
}
