#ifndef __SHCD_DLG_H__
#define __SHCD_DLG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

class DlgSearchDir : public wxDialog
{
private:

protected:
	wxStaticText* m_staticText3;
	wxStaticText* m_staticText4;
	wxStaticText* m_staticTxtCurrentPath;
	wxStaticLine* m_staticline2;
	wxStaticText* m_staticCurrDir;
	wxStaticText* m_staticDirCount;
	wxButton* m_btnCancel;

	// Virtual event handlers, overide them in your derived class
	virtual void OnInitDialog(wxInitDialogEvent& event);
	virtual void OnBtnCancel(wxCommandEvent& event);

public:
	DlgSearchDir(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Collecting directory information..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(650, 172), long style = wxDEFAULT_DIALOG_STYLE);
	~DlgSearchDir();

public:
	void SetCurrentDrive(const wxString& strDriveName);
	void SetCurrentDir(const wxString& strCurrPath);
	void SetReadDirectory(int iDirCount, const wxString& strReadPath);

	bool IsCancel() { return m_bCancel; }

private:
	bool m_bCancel = false;
#ifdef __WXMSW__
	wxString m_strDrive;
#endif
private:
	void OnCharHook(wxKeyEvent& event);
	void OnDlgClose(wxCloseEvent& event);
	void OnDirSearchThreadEvent(wxThreadEvent& event);
	wxDECLARE_EVENT_TABLE();
};
#endif
