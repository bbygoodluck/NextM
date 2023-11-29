#ifndef HEADER_FF96400F85CCEDF6
#define HEADER_FF96400F85CCEDF6


#ifndef __DLG_MCD_FIND_H__
#define __DLG_MCD_FIND_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/radiobut.h>
#include <wx/listbox.h>
#include <wx/dialog.h>

#include "../common/Lock.h"
///////////////////////////////////////////////////////////////////////////////
/// Class DlgNCDFind
///////////////////////////////////////////////////////////////////////////////
class DlgNCDFind : public wxDialog, public wxThreadHelper
{
public:
	DlgNCDFind(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("NCD Dir Searching..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(763, 646), long style = wxDEFAULT_DIALOG_STYLE);
	~DlgNCDFind();

public:
	bool IsMoveDir() { return m_bMoveDir; }

private:
	bool m_bFindInclude;
	bool m_bClose;
	bool m_bFindCancel;
	bool m_bMoveDir;

	wxString m_strFindDirName;
	wxString m_strRegEx;
	CLock m_Lock;

protected:
	wxStaticText* m_staticText1;
	wxTextCtrl* m_txtFind;
	wxButton* m_btnFind;
	wxButton* m_btnSearchCancel;
	wxRadioButton* m_radioBtnInclude;
	wxRadioButton* m_radioBtnEqual;
	wxStaticText* m_staticFindingText;
	wxListBox* m_listBox1;
	wxButton* m_btnClose;

	// Virtual event handlers, overide them in your derived class
	virtual void OnInitDialog(wxInitDialogEvent& event);
	virtual void OnTextEnter(wxCommandEvent& event);
	virtual void OnFind(wxCommandEvent& event);
	virtual void OnFindCancel(wxCommandEvent& event);
	virtual void OnRdo_IncludeClick(wxCommandEvent& event);
	virtual void OnRdo_EqualClick(wxCommandEvent& event);
	virtual void OnClose(wxCommandEvent& event);
	virtual void m_listBox1OnLeftDClick(wxMouseEvent& event);
	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnLstBoxKeyDown(wxKeyEvent& event);
	virtual void OnTextCharHook(wxKeyEvent& event);

protected:
	wxThread::ExitCode Entry();
	bool InitThread();

private:
	void SearchStart();
	void ProcessKeyEvent(int iKeyCode);
};

#endif
#endif // header guard

