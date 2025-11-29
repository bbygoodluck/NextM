#ifndef DLGPROCESSDETAIL_H
#define DLGPROCESSDETAIL_H

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>

///////////////////////////////////////////////////////////////////////////////
class DlgProcessDetail : public wxDialog
{
public:
	DlgProcessDetail( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Process Detail"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1100,682 ), long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	~DlgProcessDetail();

public:
	void SetProcessID(unsigned long ulProcessID);

private:
	void InitProcessDetail();

private:
	struct sListColumnItem {
		wxString _strColumnName = wxT("");
		unsigned int _width = 0;
		wxListColumnFormat _align = wxLIST_FORMAT_LEFT;
	};

protected:
//	wxTreeCtrl* m_treeCtrlDetail;
	wxStaticText* m_staticText3;
	wxStaticText* m_staticProgramName;
	wxListCtrl* m_lstItems;

	wxButton* m_btnClose;
	wxStaticText* m_staticText1;
	wxStaticText* m_staticCommandLine;
	// Virtual event handlers, overide them in your derived class
	virtual void OnInitDialog( wxInitDialogEvent& event );
	virtual void OnClose( wxCommandEvent& event );

private:
	unsigned long m_ulProcessID = 0;

};


#endif // DLGPROCESSDETAIL_H
