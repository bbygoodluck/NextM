#include "../ginc.h"
#include "DlgFavoriteManager.h"
#include "GenericDirDialog.h"
DlgFavoriteManager::DlgFavoriteManager(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style)
	, m_strAddPath(wxT(""))
	, m_bChanged(false)
	, m_bAddMode(false)
{
	SetIcon(wxIcon("wxwin"));
	CenterOnScreen();

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	m_treeFavorite = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_FULL_ROW_HIGHLIGHT | wxTR_SINGLE);
	bSizer2->Add(m_treeFavorite, 1, wxALL | wxEXPAND, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText2 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_FAVORITE_NAME")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	bSizer4->Add(m_staticText2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_txtFavoriteName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 | wxBORDER_SIMPLE);
	bSizer4->Add(m_txtFavoriteName, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);


	bSizer3->Add(bSizer4, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText3 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_FAVORITE_TYPE")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText3->Wrap(-1);
	bSizer5->Add(m_staticText3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_cmbType = new wxComboBox(this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
	m_cmbType->Append(theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_FAVORITE_ITEM")));
	m_cmbType->Append(theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_FAVORITE_GROUP")));
	m_cmbType->SetSelection(0);
	bSizer5->Add(m_cmbType, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);


	bSizer3->Add(bSizer5, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText4 = new wxStaticText(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_FAVORITE_PATH")), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	bSizer6->Add(m_staticText4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_txtPath = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), 0 | wxBORDER_SIMPLE);
	bSizer6->Add(m_txtPath, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	m_btnPathFind = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_SEL_MSG")), wxPoint(-1, -1), wxSize(50, 27), wxBU_NOTEXT);

	m_btnPathFind->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW_DIR, wxART_MENU));
	bSizer6->Add(m_btnPathFind, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);


	bSizer3->Add(bSizer6, 0, wxEXPAND, 5);


	bSizer2->Add(bSizer3, 0, wxEXPAND, 5);


	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	m_staticline1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	bSizer1->Add(m_staticline1, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxHORIZONTAL);

	m_btnAdd = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_ADD")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnAdd->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
	bSizer8->Add(m_btnAdd, 0, wxALL, 5);

	m_btnDelete = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_DEL")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnDelete->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_MENU));
	bSizer8->Add(m_btnDelete, 0, wxALL, 5);

	m_btnModify = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_MODIFY")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnModify->SetBitmap(wxArtProvider::GetBitmap(wxART_FIND_AND_REPLACE, wxART_MENU));
	bSizer8->Add(m_btnModify, 0, wxALL, 5);

	m_btnCancel = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CANCEL")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnCancel->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU));
	bSizer8->Add(m_btnCancel, 0, wxALL, 5);

	m_btnSave = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_SAVE")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnSave->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_MENU));
	bSizer8->Add(m_btnSave, 0, wxALL, 5);

	m_btnClose = new wxButton(this, wxID_ANY, theMsg->GetMessage(wxT("MSG_DLG_OPTION_BTN_CLOSE")), wxDefaultPosition, wxDefaultSize, 0);

	m_btnClose->SetBitmap(wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU));
	bSizer8->Add(m_btnClose, 0, wxALL, 5);


	bSizer7->Add(bSizer8, 0, wxALIGN_RIGHT, 5);


	bSizer1->Add(bSizer7, 0, wxEXPAND, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgFavoriteManager::OnDialogClose));
	this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgFavoriteManager::OnInitDialog));
	this->Connect(wxEVT_CHAR_HOOK, wxKeyEventHandler(DlgFavoriteManager::OnCharHook));
	m_treeFavorite->Connect(wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler(DlgFavoriteManager::OnTreeBeginDrag), NULL, this);
	m_treeFavorite->Connect(wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler(DlgFavoriteManager::OnTreeEndDrag), NULL, this);
	m_treeFavorite->Connect(wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler(DlgFavoriteManager::OnTreeEndLabelEdit), NULL, this);
	m_treeFavorite->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(DlgFavoriteManager::OnTreeItemChanged), NULL, this);
	m_cmbType->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(DlgFavoriteManager::OnCmbItemSelected), NULL, this);
	m_btnPathFind->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnDirSelectClick), NULL, this);
	m_btnAdd->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnAddClick), NULL, this);
	m_btnDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnDelClick), NULL, this);
	m_btnModify->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnModifyClick), NULL, this);
	m_btnCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnCancelClick), NULL, this);
	m_btnSave->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnSaveClick), NULL, this);
	m_btnClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnQuitClick), NULL, this);
}

DlgFavoriteManager::~DlgFavoriteManager()
{
	// Disconnect Events
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(DlgFavoriteManager::OnDialogClose));
	this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(DlgFavoriteManager::OnInitDialog));
	this->Disconnect(wxEVT_CHAR_HOOK, wxKeyEventHandler(DlgFavoriteManager::OnCharHook));
	m_treeFavorite->Disconnect(wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler(DlgFavoriteManager::OnTreeBeginDrag), NULL, this);
	m_treeFavorite->Disconnect(wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler(DlgFavoriteManager::OnTreeEndDrag), NULL, this);
	m_treeFavorite->Disconnect(wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler(DlgFavoriteManager::OnTreeEndLabelEdit), NULL, this);
	m_treeFavorite->Disconnect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(DlgFavoriteManager::OnTreeItemChanged), NULL, this);
	m_cmbType->Disconnect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(DlgFavoriteManager::OnCmbItemSelected), NULL, this);
	m_btnPathFind->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnDirSelectClick), NULL, this);
	m_btnAdd->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnAddClick), NULL, this);
	m_btnDelete->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnDelClick), NULL, this);
	m_btnModify->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnModifyClick), NULL, this);
	m_btnCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnCancelClick), NULL, this);
	m_btnSave->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnSaveClick), NULL, this);
	m_btnClose->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DlgFavoriteManager::OnQuitClick), NULL, this);
}

void DlgFavoriteManager::OnInitDialog(wxInitDialogEvent& event)
{
	m_txtPath->SetLabelText(m_strAddPath);

	CreateTreeImages(16);
	wxTreeItemId rootId = m_treeFavorite->AddRoot(theMsg->GetMessage(wxT("MSG_TOOLBAR_FAVORITE")));
	m_treeFavorite->SetItemImage(rootId, TreeCtrlIcon_Root);

	m_bChanged = false;

	m_selectedItem = rootId;
	if (theBookmark->GetBookmarkSize() == 0)
		return;

	CreateFavoriteTree(rootId, theBookmark->GetJsonDoc()["bookmarks"]);
	m_treeFavorite->ExpandAll();

	m_rootItem = m_treeFavorite->GetRootItem();

	if (m_bAddMode)
	{
		m_btnDelete->Enable(false);
		m_btnModify->Enable(false);
	}
}

void DlgFavoriteManager::CreateTreeImages(int size)
{
	wxIcon icons[3];
	wxSize iconSize(size, size);
	wxImageList* images = new wxImageList(size, size, true);

	icons[0] = wxArtProvider::GetIcon(wxART_ADD_BOOKMARK, wxART_LIST, wxSize(16, 16));
	icons[1] = wxArtProvider::GetIcon(wxART_FOLDER_OPEN, wxART_LIST, wxSize(16, 16));
	icons[2] = wxArtProvider::GetIcon(wxART_NEW_DIR, wxART_LIST, wxSize(16, 16));

	for (size_t i = 0; i < WXSIZEOF(icons); i++)
	{
		int sizeOrig = icons[i].GetWidth();
		if (size == sizeOrig)
			images->Add(icons[i]);
		else
			images->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
	}

	m_treeFavorite->AssignImageList(images);
}

void DlgFavoriteManager::CreateFavoriteTree(wxTreeItemId& treeNodeId, const Value& items)
{
	assert(items.IsArray());
	unsigned int iBookMSize = items.Size();

	for (unsigned int i = 0; i < iBookMSize; i++)
	{
		wxString strName = items[i]["name"].GetString();
		wxString strType = items[i]["type"].GetString();
		wxString strPath = items[i]["path"].GetString();
		wxTreeItemId TreeItemId = nullptr;

		if(strType.Cmp(wxT("item")) == 0)
			TreeItemId = m_treeFavorite->AppendItem(treeNodeId, strName, TreeCtrlIcon_Item, -1, new CFavoriteItem(strName, strType, strPath));
		else
		{
			TreeItemId = m_treeFavorite->AppendItem(treeNodeId, strName, TreeCtrlIcon_Group, -1, new CFavoriteItem(strName, strType, strPath));
			const Value& arrChilds = items[i]["child"];
			if(arrChilds.Size() > 0)
				CreateFavoriteTree(TreeItemId, arrChilds);
		}
	}
}

void DlgFavoriteManager::OnCmbItemSelected(wxCommandEvent& event)
{
	m_txtPath->Enable(true);
	int iSel = m_cmbType->GetSelection();
	if (iSel == 1)
	{
		m_txtPath->SetLabelText(wxT(""));
		m_txtPath->Enable(false);
	}
}

void DlgFavoriteManager::OnDirSelectClick(wxCommandEvent& event)
{
	wxGenericDirDialog dlgFavorite(_gMainFrame, wxT("Select target directory"), wxGetHomeDir());
	dlgFavorite.SetSize(wxSize(600, 600));
	dlgFavorite.CenterOnScreen();
	if(dlgFavorite.ShowModal() == wxID_OK)
	{
		wxString strPath = dlgFavorite.GetPath();
		m_txtPath->SetLabelText(strPath);
	}
}

void DlgFavoriteManager::OnAddClick(wxCommandEvent& event)
{
	if (!m_selectedItem)
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_ITEM_NOT_SELECTED")), PROGRAM_FULL_NAME, wxICON_ERROR | wxOK);
		return;
	}

	CFavoriteItem* pTreeItem = (CFavoriteItem *)m_treeFavorite->GetItemData(m_selectedItem);

	if (pTreeItem != nullptr)
	{
		if (pTreeItem->_strType.Cmp(wxT("item")) == 0)
		{
			wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_ITEM_ADD_FAILED")), PROGRAM_FULL_NAME, wxICON_ERROR | wxOK);
			return;
		}
	}

	wxString strName = m_txtFavoriteName->GetValue();
	wxString strType = m_cmbType->GetSelection() == 0 ? wxT("item") : wxT("group");
	wxString strPath = m_txtPath->GetValue();

	if (strName.IsEmpty())
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_ITEM_NOT_INPUT_REGNAME")), PROGRAM_FULL_NAME, wxICON_INFORMATION | wxOK);
		m_txtFavoriteName->SetFocus();
		return;
	}

	if (strType.Cmp(wxT("item")) == 0 && strPath.IsEmpty())
	{
		wxMessageBox(theMsg->GetMessage(("MSG_DLG_BOOKMARK_ADD_ITEM_FAILED")), PROGRAM_FULL_NAME, wxICON_INFORMATION | wxOK);
		return;
	}

	int iImageNum = strType.Cmp(wxT("item")) == 0 ? TreeCtrlIcon_Item : TreeCtrlIcon_Group;

	wxTreeItemId newTreeItemId = m_treeFavorite->AppendItem(m_selectedItem, strName, iImageNum, -1, new CFavoriteItem(strName, strType, strPath));

	m_treeFavorite->SelectItem(newTreeItemId);
	m_treeFavorite->SetFocus();
	m_selectedItem = newTreeItemId;

	m_bChanged = true;

	m_txtFavoriteName->SetLabelText(wxT(""));
	m_cmbType->SetSelection(0);
	m_txtPath->SetLabelText(wxT(""));
}

void DlgFavoriteManager::OnDelClick(wxCommandEvent& event)
{
	wxTreeItemId parentItem = m_treeFavorite->GetItemParent(m_selectedItem);
	m_treeFavorite->Delete(m_selectedItem);

	m_selectedItem = parentItem;
	m_treeFavorite->SelectItem(m_selectedItem);
	m_treeFavorite->SetFocus();

	m_bChanged = true;
}

void DlgFavoriteManager::OnModifyClick(wxCommandEvent& event)
{
	CFavoriteItem* pTreeItem = (CFavoriteItem *)m_treeFavorite->GetItemData(m_selectedItem);

	wxString strName = m_txtFavoriteName->GetValue();
	wxString strType = m_cmbType->GetSelection() == 0 ? wxT("item") : wxT("group");
	wxString strPath = m_txtPath->GetValue();

	pTreeItem->_strName = strName;

	if (m_strType.Cmp(strType) != 0)
	{
		int iRet = wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_CHANGE_TYPE")),
			PROGRAM_FULL_NAME,
			wxICON_QUESTION | wxYES_NO);

		if (iRet == wxYES)
		{
			pTreeItem->_strType = strType;
			pTreeItem->_strPath = wxT("");

			int iImageNum = strType.Cmp(wxT("item")) == 0 ? TreeCtrlIcon_Item : TreeCtrlIcon_Group;
			m_treeFavorite->SetItemImage(m_selectedItem, iImageNum);
		}
	}
	else
	{
		pTreeItem->_strType = strType;
		pTreeItem->_strPath = strPath;
	}

	m_treeFavorite->SetItemText(m_selectedItem, strName);
	m_treeFavorite->SetItemData(m_selectedItem, pTreeItem);
	m_treeFavorite->EnsureVisible(m_selectedItem);

	m_bChanged = true;
}

void DlgFavoriteManager::OnSaveClick(wxCommandEvent& event)
{
	SaveProc();
	m_bChanged = false;
}

void DlgFavoriteManager::OnCancelClick(wxCommandEvent& event)
{
	EndDialog(wxID_CANCEL);
}

void DlgFavoriteManager::CloseProc()
{
	if (m_bChanged)
	{
		int iRet = wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_BOOKMARK_CHANGE_AND_SAVE")), PROGRAM_FULL_NAME, wxYES_NO | wxICON_QUESTION);
		if (iRet == wxYES)
			SaveProc();
	}

	EndDialog(wxID_CLOSE);
}

void DlgFavoriteManager::OnDialogClose(wxCloseEvent& event)
{
	CloseProc();
}

void DlgFavoriteManager::OnCharHook(wxKeyEvent& event)
{
	int iKeyCode = event.GetKeyCode();
	if (iKeyCode == WXK_ESCAPE)
		EndDialog(wxID_CLOSE);

	event.Skip();
}

void DlgFavoriteManager::OnQuitClick(wxCommandEvent& event)
{
	CloseProc();
}

void DlgFavoriteManager::OnTreeItemChanged(wxTreeEvent& event)
{
	if (m_bAddMode)
		return;

	wxTreeItemId treeItemId = event.GetItem();
	CFavoriteItem* pTreeItem = (CFavoriteItem *)m_treeFavorite->GetItemData(treeItemId);

	m_selectedItem = treeItemId;

	if (m_rootItem == treeItemId)
	{
		m_txtFavoriteName->SetLabelText(wxT(""));
		m_txtPath->SetLabelText(wxT(""));
		m_cmbType->SetSelection(0);
		return;
	}

	if (pTreeItem)
	{
		m_strName = pTreeItem->_strName;
		m_strType = pTreeItem->_strType;
		m_strPath = pTreeItem->_strPath;

		m_txtFavoriteName->SetLabelText(m_strName);
		m_txtPath->SetLabelText(m_strPath);

		int iSelIndex = m_strType.Cmp("item") == 0 ? 0 : 1;
		m_cmbType->SetSelection(iSelIndex);

		m_btnDelete->Enable(true);
		m_btnModify->Enable(true);
	}
}

void DlgFavoriteManager::OnTreeBeginDrag(wxTreeEvent& event)
{
	if (event.GetItem() != m_treeFavorite->GetRootItem())
	{
		m_draggedItem = event.GetItem();

//		wxPoint clientpt = event.GetPoint();
//		wxPoint screenpt = ClientToScreen(clientpt);

		event.Allow();
	}
}

void DlgFavoriteManager::OnTreeEndDrag(wxTreeEvent& event)
{
	wxTreeItemId itemSrc = m_draggedItem;
	wxTreeItemId itemDst = event.GetItem();

	m_draggedItem = (wxTreeItemId)0l;

	m_bChanged = true;
	if (DoTreeItemDrop(itemSrc, itemDst))
		m_treeFavorite->Delete(itemSrc);
	else
		m_treeFavorite->SelectItem(itemSrc);

	m_treeFavorite->SetFocus();
}

bool DlgFavoriteManager::DoTreeItemDrop(wxTreeItemId& treeNodeSrc, wxTreeItemId& treeNodeDst)
{
	CFavoriteItem* pTreeItemSrc = (CFavoriteItem *)m_treeFavorite->GetItemData(treeNodeSrc);
	CFavoriteItem* pTreeItemDst = (CFavoriteItem *)m_treeFavorite->GetItemData(treeNodeDst);

	if(pTreeItemDst == nullptr)
	{
		treeNodeDst = m_treeFavorite->GetRootItem();
		pTreeItemDst = (CFavoriteItem *)m_treeFavorite->GetItemData(treeNodeDst);
	}
	else
	{
		if (pTreeItemDst->_strType.Cmp(wxT("item")) == 0)
		{
			wxMessageBox(theMsg->GetMessage(wxT("MSG_DLG_FAVORITE_MOVE_GROUP")), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
			return false;
		}
	}

	wxString strName = pTreeItemSrc->_strName;
	wxString strType = pTreeItemSrc->_strType;
	wxString strPath = pTreeItemSrc->_strPath;

	int iImageNum = strType.Cmp(wxT("item")) == 0 ? TreeCtrlIcon_Item : TreeCtrlIcon_Group;
//	wxTreeItemId newTreeItemId = m_treeFavorite->AppendItem(treeNodeDst, strName, iImageNum, -1, new CFavoriteItem(strName, strType, strPath));
	wxTreeItemId newTreeItemId = m_treeFavorite->InsertItem(treeNodeDst, nullptr, strName, iImageNum, -1, new CFavoriteItem(strName, strType, strPath));

	m_treeFavorite->SelectItem(newTreeItemId);

	if (pTreeItemSrc->_strType.Cmp(wxT("group")) == 0)
	{
		if (m_treeFavorite->ItemHasChildren(treeNodeSrc))
		{
			wxTreeItemId childId;
			wxTreeItemIdValue itemValue = 0;

			bool bFirst = true;
			while (1)
			{
				childId = bFirst ? m_treeFavorite->GetFirstChild(treeNodeSrc, itemValue) : m_treeFavorite->GetNextChild(treeNodeSrc, itemValue);
				if (!childId)
					break;

				bFirst = false;
				DoTreeItemDrop(childId, newTreeItemId);
			}
		}
	}

	return true;
}

void DlgFavoriteManager::OnTreeEndLabelEdit(wxTreeEvent& event)
{
	wxTreeItemId selectedTreeId = event.GetItem();
	CFavoriteItem* pTreeItem = (CFavoriteItem *)m_treeFavorite->GetItemData(selectedTreeId);

	wxString strName(event.GetLabel());

	if (pTreeItem)
	{
		pTreeItem->_strName = strName;
		m_staticText2->SetLabelText(strName);
	}
}

void DlgFavoriteManager::SaveProc()
{
	theBookmark->RemoveAll();
	theBookmark->GetJsonDoc().SetObject();

	Document::AllocatorType& allocator = theBookmark->GetJsonDoc().GetAllocator();
	Value valBookmark(kArrayType);

	wxTreeItemId root = m_treeFavorite->GetRootItem();
	DoSave(root , valBookmark, allocator);

	theBookmark->GetJsonDoc().AddMember("bookmarks", valBookmark, allocator);
	theBookmark->Save();

	wxMessageBox(wxT("The bookmark has been saved"), PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
}

void DlgFavoriteManager::DoSave(wxTreeItemId& treeNodeId, Value& valParent, Document::AllocatorType& allocator)
{
	if (!treeNodeId.IsOk()) return;

	if(m_treeFavorite->ItemHasChildren(treeNodeId))
	{
		wxTreeItemId childId;
		wxTreeItemIdValue itemValue = 0;

		wxString strName(wxT(""));
		wxString strType(wxT(""));
		wxString strPath(wxT(""));

		bool bFirst = true;

		while (1)
		{
			childId = bFirst ? m_treeFavorite->GetFirstChild(treeNodeId, itemValue) : m_treeFavorite->GetNextChild(treeNodeId, itemValue);
			if (!childId)
				break;

			bFirst = false;
			CFavoriteItem* pTreeItem = (CFavoriteItem *)m_treeFavorite->GetItemData(childId);

			strName = pTreeItem->_strName;
			strType = pTreeItem->_strType;
			strPath = pTreeItem->_strPath;

			Value item;
			item.SetObject();

			Value _name(strName.c_str(), allocator);
			Value _type(strType.c_str(), allocator);
			Value _path(strPath.c_str(), allocator);

			item.AddMember("name", _name, allocator);
			item.AddMember("type", _type, allocator);
			item.AddMember("path", _path, allocator);

			Value valChildItem(kArrayType);

			if (strType.Cmp(wxT("group")) == 0)
				DoSave(childId, valChildItem, allocator);

			item.AddMember("child", valChildItem, allocator);
			valParent.PushBack(item, allocator);
		}
	}
}
