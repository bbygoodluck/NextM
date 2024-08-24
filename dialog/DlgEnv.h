#ifndef __DLG_ENVIRONMENT_H__
#define __DLG_ENVIRONMENT_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/listctrl.h>
#include <wx/clrpicker.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

class DlgEnv : public wxDialog
{
	typedef struct _COLORINFO {
		wxColour _col;
		wxString _strExt;
	} COLORINFO, *LPCOLORINFO;
private:
	int m_iExtPGSelected;

	wxVector<COLORINFO> m_vecColorInfo;
protected:
	wxNotebook* m_tabBook;
	wxPanel* m_panelDefault;
	wxStaticText* m_staticText1;
	wxSpinCtrl* m_spinCtrlIcoPreLoad;
	wxCheckBox* m_chkLastSession;
	wxCheckBox* m_chkUseIME;
	wxStaticText* m_staticText2;
	wxStaticText* m_staticFontName;
	wxStaticText* m_staticText4;
	wxSpinCtrl* m_spinFontSize;
	wxButton* m_btnFontSelect;
	wxStaticText* m_staticText21;
	wxStaticText* m_staticText5;
	wxTextCtrl* m_txtExternalPGName;
	wxStaticText* m_staticText6;
	wxStaticText* m_staticExternalPGPath;
	wxButton* m_btnFindExternlPG;
	wxStaticText* m_staticText8;
	wxTextCtrl* m_txtArgs;
	wxButton* m_btnAddExternalPG;
	wxButton* m_btnModExternalPG;
	wxButton* m_btnDelExternalPG;
	wxListCtrl* m_lstExternalPG;
	wxPanel* m_panelColor;
	wxStaticText* m_staticText9;
	wxColourPickerCtrl* m_colourDefault;
	wxStaticText* m_staticText91;
	wxColourPickerCtrl* m_colourDir;
	wxStaticText* m_staticText14;
	wxColourPickerCtrl* m_colourDrive;
	wxStaticText* m_staticText92;
	wxColourPickerCtrl* m_colourReadOnly;
	wxStaticText* m_staticText93;
	wxColourPickerCtrl* m_colourSelectItem;
	wxStaticText* m_staticText94;
	wxColourPickerCtrl* m_colourBackGrnd;
	wxStaticText* m_staticText95;
	wxColourPickerCtrl* m_colourAttr;
	wxStaticText* m_staticText96;
	wxColourPickerCtrl* m_colourDesc;
	wxStaticText* m_staticText97;
	wxColourPickerCtrl* m_colourColumn;
	wxStaticText* m_staticText98;
	wxColourPickerCtrl* m_colourMatch;
	wxStaticText* m_staticText99;
	wxColourPickerCtrl* m_colourInfoViewTxt;
	wxStaticText* m_staticText910;
	wxColourPickerCtrl* m_colourInfoViewBack;
	wxListBox* m_lstExtColor;
	wxButton* m_btnAddExt;
	wxButton* m_btnEdit;
	wxButton* m_btnExtDel;
	wxPanel* m_panelEtc;
	wxButton* m_btnCancel;
	wxButton* m_btnApply;
	wxButton* m_btnClose;
	wxSpinButton* m_spinSort;

	// Virtual event handlers, overide them in your derived class
	virtual void OnDlgCloseClick(wxCloseEvent& event);
	virtual void OnInitDialog(wxInitDialogEvent& event);
	virtual void OnFontFindClick(wxCommandEvent& event);
	virtual void OnExternalPG_FindClick(wxCommandEvent& event);
	virtual void OnExternalPG_AddClick(wxCommandEvent& event);
	virtual void OnExternalPG_ModClick(wxCommandEvent& event);
	virtual void OnExternalPG_DelClick(wxCommandEvent& event);
	virtual void OnLstExtPGSelected(wxListEvent& event);
	virtual void OnBackColorChanged(wxColourPickerEvent& event);
	virtual void OnListBoxDBClick(wxCommandEvent& event);
	virtual void OnExtAddClick(wxCommandEvent& event);
	virtual void OnExtEditClick(wxCommandEvent& event);
	virtual void OnExtDelClick(wxCommandEvent& event);
	virtual void OnEnvCancelClick(wxCommandEvent& event);
	virtual void OnApplyClick(wxCommandEvent& event);
	virtual void OnCloseClick(wxCommandEvent& event);

	virtual void OnSpinDown( wxSpinEvent& event );
	virtual void OnSpinUp( wxSpinEvent& event );

public:
	DlgEnv(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(780, 559), long style = wxDEFAULT_DIALOG_STYLE);
	~DlgEnv();

private:
	void DoApply();
	void DoApplyColorInfo();
	void DoApplyExtColor(const wxString& strExts, const wxString& strCol);
	void DoMakeColorInfo(const wxColour& col, const wxString& strExt);
	void DoSetExtColorInfoToList();
	void DoExternalPGOrdering(bool bMoveUp);
};
#endif
