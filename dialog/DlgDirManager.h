#ifndef __SHCD_VIEW_DLG_H__
#define __SHCD_VIEW_DLG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/infobar.h>
#include <wx/statline.h>

#define wxID_SCD_REFRESH 1000
#define wxID_DIR_DELETE 1001
#define wxID_DIR_FIND 1002
#define wxID_DIR_NEXT_FIND 1003
#define wxID_DIR_MNG_CLOSE 1004
#define wxID_DIR_DRVLIST 1005

class CNextMNCDView;
class CDirSearchThread;
class CNCDInfoThread;
///////////////////////////////////////////////////////////////////////////////
/// Class DlgDirManager
///////////////////////////////////////////////////////////////////////////////
class DlgDirManager : public wxDialog
{
private:

protected:
	wxAuiToolBar* m_scdToolBar;
	wxAuiToolBarItem* m_toolRefreshAll;
	wxAuiToolBarItem* m_toolRefreshDir;
	wxAuiToolBarItem* m_toolAddFolder;
	wxAuiToolBarItem* m_toolRenameFolder;
	wxAuiToolBarItem* m_toolDelete;
	wxAuiToolBarItem* m_toolFind;
	wxAuiToolBarItem* m_toolFindNext;
	wxAuiToolBarItem* m_toolClose;
	wxStaticLine* m_staticline1;
	wxStaticText* m_staticText2;
	wxStaticText* m_staticText31;
	wxStaticText* m_staticText3;

	CNextMNCDView* m_pNCDView;

public:
	DlgDirManager(wxWindow* parent,
					wxWindowID id = wxID_ANY,
					const wxString& title = theMsg->GetMessage(wxT("MSG_TOOLBAR_MCD")),
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxSize(1000, 900),
					long style = wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCLIP_CHILDREN | wxBORDER_NONE);
	~DlgDirManager();

	void SetNCDFileInfo(const wxString& treeInfoFile, const wxString& strCurrPath) {
		m_strNCDFilePath = treeInfoFile;
		m_strCurrPath = strCurrPath;
	}

	void SetNodePath(const wxString& strFullPath);
	void LoadNCDData(bool bFirst, bool bSetFocus = true);
	void LoadSubDir(DirNode* pNode);

	void RefreshAll();
	void FindDir();
/*	void ReleaseWait() {
		m_Lock.getCondition()->Signal();
	}
*/
private:

private:
	wxString m_strNCDFilePath;
	wxString m_strCurrPath;

	CDirSearchThread* m_pDirSearchThread;
	CNCDInfoThread* m_pNCDInfoThread;

//	CLock m_Lock;
protected:
	virtual void OnRefreshAll(wxCommandEvent& event);
	virtual void OnSCDRefreshDir(wxCommandEvent& event);
	virtual void OnAddDir(wxCommandEvent& event);
	virtual void OnRenameDir(wxCommandEvent& event);
	virtual void OnDeleteDir(wxCommandEvent& event);
	virtual void OnFindDir(wxCommandEvent& event);
	virtual void OnNextFindDir(wxCommandEvent& event);
	virtual void OnDirManagerClose(wxCommandEvent& event);
	virtual void OnInitDialog(wxInitDialogEvent& event);
	virtual void OnClose(wxCloseEvent& event);

private:
//	void OnMakeTreeInfoThreadEvent(wxThreadEvent& event);
//	wxDECLARE_EVENT_TABLE();
};

#endif
