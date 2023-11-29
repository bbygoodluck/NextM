#ifndef __DLG_FAVORITE_MANAGER_H__
#define __DLG_FAVORITE_MANAGER_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/bmpcbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/statline.h>

class DlgFavoriteManager : public wxDialog
{
public:
	enum
	{
		TreeCtrlIcon_Root,
		TreeCtrlIcon_Item,
		TreeCtrlIcon_Group,
	};
	
	class CFavoriteItem : public wxTreeItemData
	{
	public:
		CFavoriteItem(const wxString& strName, const wxString& strType, const wxString& strPath)
			: _strName(strName)
			, _strType(strType)
			, _strPath(strPath)
		{

		}
		~CFavoriteItem() {}

	public:
		wxString _strName;
		wxString _strType;
		wxString _strPath;
	};

private:
	wxString m_strAddPath;
	wxString m_strName;
	wxString m_strType;
	wxString m_strPath;

	wxTreeItemId m_selectedItem;
	wxTreeItemId m_rootItem;
	wxTreeItemId m_draggedItem;

	bool m_bChanged;
	bool m_bAddMode;
protected:
	wxTreeCtrl* m_treeFavorite;
	wxStaticText* m_staticText2;
	wxTextCtrl* m_txtFavoriteName;
	wxStaticText* m_staticText3;
	wxComboBox* m_cmbType;
	wxStaticText* m_staticText4;
	wxTextCtrl* m_txtPath;
	wxButton* m_btnPathFind;
	wxStaticLine* m_staticline1;
	wxButton* m_btnAdd;
	wxButton* m_btnDelete;
	wxButton* m_btnModify;
	wxButton* m_btnCancel;
	wxButton* m_btnSave;
	wxButton* m_btnClose;

	// Virtual event handlers, overide them in your derived class
	virtual void OnCharHook(wxKeyEvent& event);
	virtual void OnDialogClose(wxCloseEvent& event);
	virtual void OnInitDialog(wxInitDialogEvent& event);
	virtual void OnTreeBeginDrag(wxTreeEvent& event);
	virtual void OnTreeEndDrag(wxTreeEvent& event);
	virtual void OnDirSelectClick(wxCommandEvent& event);
	virtual void OnModifyClick(wxCommandEvent& event);
	virtual void OnAddClick(wxCommandEvent& event);
	virtual void OnDelClick(wxCommandEvent& event);
	virtual void OnSaveClick(wxCommandEvent& event);
	virtual void OnCancelClick(wxCommandEvent& event);
	virtual void OnQuitClick(wxCommandEvent& event);
	virtual void OnCmbItemSelected(wxCommandEvent& event);
	virtual void OnTreeItemChanged(wxTreeEvent& event);
	virtual void OnTreeEndLabelEdit(wxTreeEvent& event);
public:
	void SetAddPath(const wxString& strPath, bool bAddMode = false) {
		m_strAddPath = strPath;
		m_bAddMode = bAddMode;
	}

private:
	void CreateFavoriteTree(wxTreeItemId& treeNodeId, const Value& items);
	void CreateTreeImages(int size);
	bool DoTreeItemDrop(wxTreeItemId& treeNodeSrc, wxTreeItemId& treeNodeDst);

	void CloseProc();
	void SaveProc();
	void DoSave(wxTreeItemId& treeNodeId, Value& valParent, Document::AllocatorType& allocator);
public:
	DlgFavoriteManager(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Favorite Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(803, 534), long style = wxDEFAULT_DIALOG_STYLE);
	~DlgFavoriteManager();

};

#endif