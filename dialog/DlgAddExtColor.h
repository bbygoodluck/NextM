#ifndef __DLG_ADD_EXT_COLOR_H__
#define __DLG_ADD_EXT_COLOR_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/clrpicker.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/dialog.h>

class DlgAddExtColor : public wxDialog
{
private:
	wxString m_strRegExt;
	wxColour m_colReg;

protected:
	wxStaticText* m_staticText28;
	wxTextCtrl* m_txtExt;
	wxColourPickerCtrl* m_colourExt;
	wxButton* m_btnAdd;
	wxButton* m_btnCancel;

	// Virtual event handlers, overide them in your derived class
	virtual void OnDialogClose(wxCloseEvent& event);
	virtual void OnInitDialog(wxInitDialogEvent& event);
	virtual void OnAddClick(wxCommandEvent& event);
	virtual void OnCancelClick(wxCommandEvent& event);

public:
	wxString GetAddExt() {
		return m_strRegExt;
	}

	wxColour GetAddColour() {
		return m_colReg;
	}

public:
	DlgAddExtColor(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Ext Color add & update"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(558, 122), long style = wxDEFAULT_DIALOG_STYLE);
	~DlgAddExtColor();

};

#endif