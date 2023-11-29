#ifndef __DLGDELETE_H__
#define __DLGDELETE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/gauge.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

#include <filesystem>
#include "../common/Lock.h"

using namespace std::filesystem;

class DlgDelete : public wxDialog, public wxThreadHelper
{
private:

protected:
	wxStaticText* m_staticText1;
	wxStaticText* m_staticDelItem;
	wxGauge* m_progressDel;
	wxStaticText* m_staticDelCnt;
	wxButton* m_btnClose;
	wxButton* m_btnCancel;

	// Virtual event handlers, overide them in your derived class
	virtual void OnClose(wxCloseEvent& event);
	virtual void OnInitialog(wxInitDialogEvent& event);
	virtual void OnDlgClose(wxCommandEvent& event);
	virtual void OnDlgCancel(wxCommandEvent& event);

protected:
	virtual wxThread::ExitCode Entry();

public:
	DlgDelete(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Deleting..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(559, 180), long style = wxDEFAULT_DIALOG_STYLE);
	~DlgDelete();

public:
	void SetDeleteInfo(const std::list<wxString>& lstDelList, bool bGoTrash)
	{
		m_lstDelList = lstDelList;
		m_bGoTrash = bGoTrash;
	}

private:
	std::list<wxString> m_lstDelList;
	wxArrayString m_arrSubLst;
	wxArrayString m_arrDirLst;

	bool m_bGoTrash;
	bool m_bWorking;
	bool m_bCancelOK;
	bool m_bStopping;
	bool m_bError;
	CLock m_Lock;

private:
	int GetNumberOf_Files_in_Directory(path p);

private:
	void OnFileOperationDeleteEnd(wxThreadEvent& event);
	wxDECLARE_EVENT_TABLE();

};

#endif //__NONAME_H__
