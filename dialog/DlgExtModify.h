#ifndef __DLG_EXT_MODIFY_H__
#define __DLG_EXT_MODIFY_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DlgExtModify
///////////////////////////////////////////////////////////////////////////////
class DlgExtModify : public wxDialog
{
private:
	wxString m_strExts;
protected:
	wxStaticText* m_staticText6;
	wxTextCtrl* m_txtExts;
	wxButton* m_btnEditOk;
	wxButton* m_btnCancel;

	// Virtual event handlers, overide them in your derived class
	virtual void OnDialogClose(wxCloseEvent& event);
	virtual void OnInitDialog(wxInitDialogEvent& event);
	virtual void OnEditOKClick(wxCommandEvent& event);
	virtual void OnCancelClick(wxCommandEvent& event);


public:
	void SetExtInfo(const wxString& strExts) {
		m_strExts = strExts;
	}

	wxString GetExts() { return m_strExts; }
public:

	DlgExtModify(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Edit Extension"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(628, 131), long style = wxDEFAULT_DIALOG_STYLE);
	~DlgExtModify();

};

#endif