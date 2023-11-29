#ifndef HEADER_8A34219F81979AA4
#define HEADER_8A34219F81979AA4

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DLG_ADD_DIR_H__
#define __DLG_ADD_DIR_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DlgAddDir
///////////////////////////////////////////////////////////////////////////////
class DlgAddDir : public wxDialog
{
public:
	DlgAddDir(wxWindow* parent
		, wxWindowID id = wxID_ANY
		, const wxString& title = theMsg->GetMessage(wxT("MSG_DLG_ADD_DIR_TITLE"))
		, const wxPoint& pos = wxDefaultPosition
		, const wxSize& size = wxSize(500, 150)
		, long style = wxDEFAULT_DIALOG_STYLE);
	virtual ~DlgAddDir();

public:
	void SetCurrentPath(const wxString& strCurrentPath) {
		m_strParentPath = strCurrentPath;
	}

	wxString GetNewPath() const;
private:
	wxBoxSizer* m_bMainSizer;
	wxTextCtrl* m_txtInput;
	wxWindow* m_pParent;
	wxString m_strNewName = wxT("");
	wxString m_strParentPath = wxT("");
	wxString m_strNewPath = wxT("");

protected:
	wxStaticBitmap* m_bmpNewDir;
	wxStaticText* m_staticDontUse;
	wxStaticText* m_staticName;
	wxStaticLine* m_staticline2;
	wxStdDialogButtonSizer* m_sdbSizer2;
	wxButton* m_sdbSizer2OK;
	wxButton* m_sdbSizer2Cancel;

	// Virtual event handlers, overide them in your derived class
	virtual void OnInitDialog(wxInitDialogEvent& event);

private:
	void DoDialogProc(int nRetCode);
	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
};

#endif //__NONAME_H__
#endif // header guard 

