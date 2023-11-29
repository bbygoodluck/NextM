#ifndef VIEWSPANEL_H_INCLUDED
#define VIEWSPANEL_H_INCLUDED

class CFileListView;
class CPathView;

class CViewsPanel : public wxPanel
{
public:
	explicit CViewsPanel(wxWindow* parent, int iID, const wxString& strPath, bool bRemote = false);
	virtual ~CViewsPanel();

public:
	CFileListView* GetFileListView() const { return m_pFileListView; }
	CPathView* GetPathView() const { return m_pPathView; }
	bool IsRemote() { return m_bRemote; }

private:
	void CreatePanelItems(bool bRemote = false);

private:
    bool m_bRemote;

    wxString m_strPath = wxT("");

	CFileListView* m_pFileListView = nullptr;
	CPathView* m_pPathView         = nullptr;

private:
	void OnErase(wxEraseEvent& event);
	wxDECLARE_EVENT_TABLE();
};
#endif // VIEWSPANEL_H_INCLUDED
