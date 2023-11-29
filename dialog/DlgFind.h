#ifndef HEADER_E9BD862A5DFC1F63
#define HEADER_E9BD862A5DFC1F63

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DLGFIND_H__
#define __DLGFIND_H__

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
#include <wx/statline.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/dialog.h>

#include "../common/Lock.h"
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DlgFind
///////////////////////////////////////////////////////////////////////////////
class DlgFind : public wxDialog , public wxThreadHelper
{
public:
	DlgFind(wxWindow* parent,
			wxWindowID id = wxID_ANY,
			const wxString& title = theMsg->GetMessage(wxT("MSG_DLG_FIND_TITLE")),
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxSize(800, 584),
			long style = wxCAPTION | wxRESIZE_BORDER);
	virtual ~DlgFind();

	virtual void OnInitDialog(wxInitDialogEvent& event);
	virtual void OnLBoxDbClick(wxCommandEvent& event);

public:
	void setSearchDir(wxString const& strPath);

private:
	void SearchDirectory(const wxString& strPath);
	void ProcessKeyEvent(int iKeyCode);
	void DoSearchStart();
	void DoClose();
	void DoCancel();

private:
	wxString m_strPath;
	wxString m_strRegEx;
	bool m_bSearchSubDir;
	bool m_bCancel;
	bool m_bClose;
	bool m_IsRunning = false;
	int m_iResultCount;
	CLock m_Lock;

protected:
	wxStaticText* m_staticSrch;
	wxTextCtrl* m_txtSourcePath;
	wxStaticText* m_staticSearchTitle;
	wxStaticText* m_staticMSG;
	wxTextCtrl* m_txtSearchName;
	wxStaticLine* m_staticline1;
	wxRadioButton* m_radioMatch;
	wxRadioButton* m_radioInclude;
	wxRadioButton* m_radioStart;
	wxRadioButton* m_radioRegex;
	wxCheckBox* m_chkIncSubDir;
	wxButton* m_btnSearch;
	wxButton* m_btnCancel;
	wxButton* m_btnClose;
	wxStaticLine* m_staticline2;
	wxStaticLine* m_staticline3;
	wxListBox* m_lstFindResult;
	wxStaticText* m_staticPathTitle;
	wxStaticText* m_staticTextPathInfo;
	wxStaticText* m_staticText8;
	wxStaticText* m_staticTextContent;

	wxStaticText* m_staticText81;
	wxStaticText* m_staticFindResult;

	wxStaticBitmap* m_bmpFolder;
	wxStaticBitmap* m_bmpFindName;
	wxStaticBitmap* m_bmpSearchPos;
	wxStaticBitmap* m_bmpContent;
	wxStaticBitmap* m_bmpFindResult;


protected:
	wxThread::ExitCode Entry();

//Event
private:
	void OnSearch(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);
};

#endif //__NONAME_H__
#endif // header guard

