#ifndef NEXTMEXECUTEMENUOPERATION_H_INCLUDED
#define NEXTMEXECUTEMENUOPERATION_H_INCLUDED

class CNextMMenuEvent;
class CFileListView;
class CNextMExecuteMenuOperation final
{
public:
	CNextMExecuteMenuOperation(CNextMMenuEvent* pMenuEvent);
	~CNextMExecuteMenuOperation();

public:
	CFileListView* GetFileListView();

	void ExecuteMenuOperation(int iMenuID, _MENU_EVENT_TYPE _mType);
	void SetVisitDirDropDownMenu(wxAuiToolBar* tb, wxAuiToolBarEvent& event);
	void UpdateBackFowrdMenu(wxUpdateUIEvent& event);
	void GotoBackForwardDirectory(int iBackForwardIndex);
	void ShowFavoriteFromStatusBar();
	void ShowCompressPopupMenu();

	//압축 실행
	void CompressMenu_ExecuteCompress(int nId);
	void CompressMenu_ExecuteCompress(int nId, const wxString& strCompressedFile);
	void CompressMenu_ExecuteDecompress(int nId, const wxString& strCompressedFile);
	void DragDropOperation();

private:
	void Init();

	//파일메뉴
	void FileMenu_Execution();
	void FileMenu_NewTab();
	void FileMenu_Copy();
	void FileMenu_Move();
	void FileMenu_Rename();
	void FileMenu_CreateFolder();
	void FileMenu_Edit();
	void FileMenu_Trash();
	void FileMenu_CompletionDel();
	void FileMenu_ExecCMD();
	void FileMenu_Terminate();

	//편집메뉴
	void EditMenu_AllRelease();
	void EditMenu_AllSelect();
	void EditMenu_Copy_UseClipboard();
	void EditMenu_Move_UseClipboard();
	void EditMenu_Paste_UseClipboard();
	void EditMenu_FileFind();
	void EditMenu_ViewContextMenu();

	//경로메뉴
	void PathMenu_DirManager();
	void PathMenu_Refresh();
	void PathMenu_GoRoot();
	void PathMenu_GoUp();
	void PathMenu_GoDown();
	void PathMenu_GoBack();
	void PathMenu_GoFoward();
	void ExecutePathMenuOperation(_MENU_EVENT_TYPE mType);
	//압축
	void CompressMenu_DoCompress();

	void CompressMenu_DeCompressCurDir();
	void CompressMenu_DeCompressMkDir();
	void CompressMenu_DeCompressSelDir();

	void ShowDecompressMenu();
	//보기메뉴
	void ViewMenu_FullScreen();
	void ViewMenu_Window_Single();
	void ViewMenu_Window_Vert();
	void ViewMenu_Window_Horz();

	void ChangeSplitStyle(int iSplitStyle);

	void ViewMenu_Column_0();
	void ViewMenu_Column_1();
	void ViewMenu_Column_2();
	void ViewMenu_Column_3();
	void ViewMenu_Column_4();
	void ViewMenu_Column_5();
	void ViewMenu_Column_6();
	void ViewMenu_Column_7();
	void ViewMenu_Column_8();
	void ViewMenu_Column_9();
	void ChangeViewColumn(COLUMN_VIEW_OPTION _colNum);

	void ViewMenu_FileView_Default();
	void ViewMenu_FileView_Hidden();
	void ViewMenu_FileView_All();
	void ChangeViewFileListOption(FILE_VIEW_OPTION _fvo);

	void ViewMenu_SortDefault();
	void ViewMenu_SortTime();
	void ViewMenu_SortSize();
	void ViewMenu_SubDir();
	void ChangeViewSortOption(VIEW_SORT sType);

	void ViewMenu_DirNum();
	//즐겨찾기
	void FavoriteMenu_Add();
	void FavoriteMenu_Manager();
	void FavoriteMenu_ItemSelected(int iMenuID);

	void BookmarkOperation(_MENU_EVENT_TYPE mType);
	//System메뉴
	void SystemMenu_ResourceUsage();
	void SystemMenu_DiskUsage();
	//도구메뉴
	void ToolMenu_Setting();
	void ToolMenu_SaveSetting();
	//이프로그램은..
	void HelpMenu_ThisProgram();

	void PathMenu_GotoDirDirectly();

	void DoFileOperation(FILE_OPERATION fop, bool IsUseClipboard = false);
	void DoClipboardOperation(bool IsCut);

	wxString GetDirectorySelect();

	void ExecuteDeleteProcess(_MENU_EVENT_TYPE mType);

private:
	std::unordered_map< int, std::function<void()> > m_mapMenuFunctionPtr;
	CNextMMenuEvent* m_pMenuEvent = nullptr;
};

#endif // NEXTMEXECUTEMENUOPERATION_H_INCLUDED
