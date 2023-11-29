#ifndef NEXTMAUINOTEBOOK_H_INCLUDED
#define NEXTMAUINOTEBOOK_H_INCLUDED

class CViewsPanel;
class CNextMAuiNotebook : public wxAuiNotebook
{
public:
	CNextMAuiNotebook(wxWindow* parent, long style);
	virtual ~CNextMAuiNotebook();

public:
	void AddTabPage(const wxString& strPath, bool bRemote = false);
	void SetTabNameChange(const wxString& strPath);

private:
	wxBitmap m_PageBMP;

private:
	void OnPageClose(wxAuiNotebookEvent& event);
	void OnPageChanged(wxAuiNotebookEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMAUINOTEBOOK_H_INCLUDED
