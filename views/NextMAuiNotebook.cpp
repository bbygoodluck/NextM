#include "../ginc.h"
#include "NextMAuiNotebook.h"
#include "ViewsPanel.h"
#include "NextMViewManager.h"

wxBEGIN_EVENT_TABLE(CNextMAuiNotebook, wxAuiNotebook)
	EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, OnPageClose)
	EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, OnPageChanged)
wxEND_EVENT_TABLE()


CNextMAuiNotebook::CNextMAuiNotebook(wxWindow* parent, long style)
	: wxAuiNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style)
{
	m_PageBMP = wxArtProvider::GetBitmap(wxART_NEW_DIR, wxART_MENU, wxSize(16, 16));
	this->SetArtProvider(new wxAuiGenericTabArt);
}

CNextMAuiNotebook::~CNextMAuiNotebook()
{
	DeleteAllPages();
}

void CNextMAuiNotebook::OnPageClose(wxAuiNotebookEvent& event)
{
	event.Skip();
}

void CNextMAuiNotebook::OnPageChanged(wxAuiNotebookEvent& event)
{
	((CNextMViewManager *)GetParent())->SetFocusPage();
	event.Skip();
}

void CNextMAuiNotebook::AddTabPage(const wxString& strPath, bool bRemote)
{
	AddPage(new CViewsPanel(this, ++wxTABWindowID, strPath, bRemote), wxT(""), false, m_PageBMP);

	int nPageCount = GetPageCount();
	this->SetSelection(nPageCount - 1);

	SetTabNameChange(strPath);
}

void CNextMAuiNotebook::SetTabNameChange(const wxString& strPath)
{
	wxString::size_type n = strPath.Find(SLASH);
	wxString strTabName = (n == wxString::npos ? strPath + SLASH : theUtility->GetPathName(strPath));

	int iSel = this->GetSelection();
	if(iSel < 0)
		iSel = 0;

	SetPageToolTip(iSel, strPath);
	this->SetPageText(iSel, strTabName);
}
