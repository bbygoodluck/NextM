#ifndef HEADER_DLG_COMPRESS
#define HEADER_DLG_COMPRESS

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DLG_COMPRESS_H_INCLUDED
#define __DLG_COMPRESS_H_INCLUDED

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/gauge.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////////
/// Class DlgCompress
///////////////////////////////////////////////////////////////////////////////
class DlgCompress : public wxDialog
{
private:
	std::list<wxString> m_vCompressList;
	wxString m_strCompressedFile = wxT("");
	wxString m_strCompressType   = wxT("");
	wxString m_strTargetDir      = wxT("");
	uint64_t m_ulTotalSize = 0;
	int m_iTotalItems = 0;

protected:
	wxListCtrl* m_lstDatas;
		wxStaticText* m_staticText5;
		wxChoice* m_cmbCompressType;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticCompressedFile;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_txtPassword;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticCurrentTxt;
		wxGauge* m_progressComTotal;
		wxStaticText* m_staticPercent;
		wxButton* m_btnCompressStart;
		wxButton* m_btnCancel;
		wxButton* m_btnAdd;
		wxButton* m_btnAddFolder;
		wxButton* m_btnClose;

		// Virtual event handlers, overide them in your derived class
		virtual void OnInitDialog( wxInitDialogEvent& event );
		virtual void OnChoice( wxCommandEvent& event );
		virtual void OnCancelClick( wxCommandEvent& event );
		virtual void OnBtnCompress( wxCommandEvent& event );
		virtual void OnBtnAdd( wxCommandEvent& event );
		virtual void OnAddFolder( wxCommandEvent& event );
		virtual void OnClose( wxCommandEvent& event );

public:
	DlgCompress( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = theMsg->GetMessage(wxT("MSG_DLG_COMPRESS_TITLE")), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 644,416 ), long style = wxDEFAULT_DIALOG_STYLE );
	~DlgCompress();

public:
	void SetCompressInfo(const std::list<wxString>& vCompressList, const wxString& strCompressedFile, const wxString& strTargetDir, const wxString& strCompressType);
	void SetCurrentFile(const wxString& strCurrentFile);
	void SetCurrentFileSize(uint64_t ulSize);
	void SetCurrentFileTotalSize(uint64_t ulTotalSize);

private:
	void OnCompressThreadEnd(wxThreadEvent& event);
	void DoCompress(int iCompressType);

	void ResetCompressFile(const wxString& strCompressExt);
	void AddItems(const wxArrayString& arrItems);
};

#endif
#endif // header guard

