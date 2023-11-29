#include "../ginc.h"
#include "ViewsPanel.h"

#ifdef __WXMSW__
	#include "./msw/LocalFileListView.h"
#else

#endif

#include "PathView.h"

wxBEGIN_EVENT_TABLE(CViewsPanel, wxPanel)
	EVT_ERASE_BACKGROUND(CViewsPanel::OnErase)
wxEND_EVENT_TABLE()


CViewsPanel::CViewsPanel(wxWindow* parent, int iID, const wxString& strPath, bool bRemote)
	: wxPanel(parent, iID, wxDefaultPosition, wxDefaultSize, CHILD_WINDOW_STYLE)
//	, m_Mainsizer(nullptr)
	, m_bRemote(bRemote)
	, m_strPath(strPath)
{
    CreatePanelItems(m_bRemote);
}

CViewsPanel::~CViewsPanel()
{
	wxDELETE(m_pPathView);
	wxDELETE(m_pFileListView);
}

void CViewsPanel::CreatePanelItems(bool bRemote)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxBoxSizer* pMainsizer = new wxBoxSizer(wxVERTICAL);

	//패스뷰 등록
	m_pPathView = new CPathView(this, wxPATHVIEW_ID++, wxSize(-1, 20));

	//파일 리스트뷰 생성
	if(!bRemote)
		m_pFileListView = new CLocalFileListView(this, wxFILFELIST_VIEWID++, wxDefaultSize);

	pMainsizer->Add(m_pPathView, 0, wxEXPAND, 0);
	pMainsizer->Add( m_pFileListView, 1, wxEXPAND, 0);


	SetSizer(pMainsizer);
	this->Layout();
}

void CViewsPanel::OnErase(wxEraseEvent& event)
{

}
