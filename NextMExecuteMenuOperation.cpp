#include "ginc.h"
#include "NextMExecuteMenuOperation.h"
#include "NextMMenuEvent.h"

#include "./views/NextMViewManager.h"

#include "./dialog/DlgAddDir.h"
#include "./dialog/DlgFind.h"
#include "./dialog/DlgFavoriteManager.h"
#include "./dialog/DlgDirManager.h"
#include "./dialog/DlgCompress.h"
#include "./dialog/DlgDeCompress.h"
#include "./dialog/GenericDirDialog.h"
#include "./dialog/DlgEnv.h"
#include "./dialog/wxMozillaLikeAboutBoxDialog.h"
#include "./dialog/header_png.h"

CNextMExecuteMenuOperation::CNextMExecuteMenuOperation(CNextMMenuEvent* pMenuEvent)
	: m_pMenuEvent(pMenuEvent)
{
	Init();
}

CNextMExecuteMenuOperation::~CNextMExecuteMenuOperation()
{

}
void CNextMExecuteMenuOperation::Init()
{
	typedef struct stMenuList {
		wxString strMenuID;
		std::function<void()> funcPtr;
	} MENU_FNPTRLIST;

	MENU_FNPTRLIST stMenufnList[] = {
		{ wxT("m_fMenu_Execute"),	                std::bind(&CNextMExecuteMenuOperation::FileMenu_Execution            , this) },
		{ wxT("m_fMenu_NewTab"),	                std::bind(&CNextMExecuteMenuOperation::FileMenu_NewTab               , this) },
		{ wxT("m_fMenu_Copy"),	                    std::bind(&CNextMExecuteMenuOperation::FileMenu_Copy	             , this) },
		{ wxT("m_fMenu_Move"),	                    std::bind(&CNextMExecuteMenuOperation::FileMenu_Move                 , this) },
		{ wxT("m_fMenu_Rename"),	                std::bind(&CNextMExecuteMenuOperation::FileMenu_Rename               , this) },
		{ wxT("m_fMenu_MakeFolder"),	            std::bind(&CNextMExecuteMenuOperation::FileMenu_CreateFolder         , this) },
		{ wxT("m_fMenu_FileEdit"),	                std::bind(&CNextMExecuteMenuOperation::FileMenu_Edit                 , this) },
		{ wxT("m_fMenu_goTrash"),	                std::bind(&CNextMExecuteMenuOperation::FileMenu_Trash                , this) },
		{ wxT("m_fMenu_goDel"),	                    std::bind(&CNextMExecuteMenuOperation::FileMenu_CompletionDel        , this) },
		{ wxT("m_fMenu_CMD"),	                    std::bind(&CNextMExecuteMenuOperation::FileMenu_ExecCMD              , this) },
		{ wxT("m_fMenu_Exit"),	                    std::bind(&CNextMExecuteMenuOperation::FileMenu_Terminate            , this) },
		//편집 메뉴
		{ wxT("m_eMenu_Release"),	                std::bind(&CNextMExecuteMenuOperation::EditMenu_AllRelease	        , this) },
		{ wxT("m_eMenu_SelAll"),	                std::bind(&CNextMExecuteMenuOperation::EditMenu_AllSelect            , this) },
		{ wxT("m_eMenu_Copy"),	                    std::bind(&CNextMExecuteMenuOperation::EditMenu_Copy_UseClipboard    , this) },
		{ wxT("m_eMenu_Cut"),	                    std::bind(&CNextMExecuteMenuOperation::EditMenu_Move_UseClipboard    , this) },
		{ wxT("m_eMenu_Paste"),        	            std::bind(&CNextMExecuteMenuOperation::EditMenu_Paste_UseClipboard   , this) },
		{ wxT("m_eMenu_Search"),	                std::bind(&CNextMExecuteMenuOperation::EditMenu_FileFind             , this) },
		{ wxT("m_eMenu_ContextMenu"),	            std::bind(&CNextMExecuteMenuOperation::EditMenu_ViewContextMenu      , this) },
		//경로메뉴
		{ wxT("m_pMenu_PathMng"),	                std::bind(&CNextMExecuteMenuOperation::PathMenu_DirManager           , this) },
		{ wxT("m_pMenu_Refresh"),	                std::bind(&CNextMExecuteMenuOperation::PathMenu_Refresh              , this) },
		{ wxT("m_pMenu_Root"),	                    std::bind(&CNextMExecuteMenuOperation::PathMenu_GoRoot               , this) },
		{ wxT("m_pMenu_Goup"),	                    std::bind(&CNextMExecuteMenuOperation::PathMenu_GoUp                 , this) },
		{ wxT("m_pMenu_Godown"),	                std::bind(&CNextMExecuteMenuOperation::PathMenu_GoDown               , this) },
		{ wxT("m_pMenu_Back"),	                    std::bind(&CNextMExecuteMenuOperation::PathMenu_GoBack               , this) },
		{ wxT("m_pMenu_foward"),	                std::bind(&CNextMExecuteMenuOperation::PathMenu_GoFoward             , this) },
		{ wxT("m_pMenu_GotoDirDirectly"), 	        std::bind(&CNextMExecuteMenuOperation::PathMenu_GotoDirDirectly      , this) },
		//압축
		{ wxT("m_menuComp_Progress"),	            std::bind(&CNextMExecuteMenuOperation::CompressMenu_DoCompress       , this ) },
		{ wxT("m_menuComp_CurrentDirCompRelease"),	std::bind(&CNextMExecuteMenuOperation::CompressMenu_DeCompressCurDir , this ) },
		{ wxT("m_menuComp_MakeDirCompRelease"),	    std::bind(&CNextMExecuteMenuOperation::CompressMenu_DeCompressMkDir  , this ) },
		{ wxT("m_menuComp_SelDirCompRelease"),	    std::bind(&CNextMExecuteMenuOperation::CompressMenu_DeCompressSelDir , this ) },
		{ wxT("m_eMenu_DecompressF8"),	            std::bind(&CNextMExecuteMenuOperation::ShowDecompressMenu            , this ) },


		//보기메뉴
		{ wxT("m_viewMenu_FullScr"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_FullScreen           , this ) },
		{ wxT("m_viewMenu_Window_0"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Window_Single        , this ) },
		{ wxT("m_viewMenu_Window_1"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Window_Vert          , this ) },
		{ wxT("m_viewMenu_Window_2"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Window_Horz          , this ) },
		{ wxT("m_viewMenu_Column_0"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_0             , this ) },
		{ wxT("m_viewMenu_Column_1"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_1             , this ) },
		{ wxT("m_viewMenu_Column_2"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_2             , this ) },
		{ wxT("m_viewMenu_Column_3"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_3             , this ) },
		{ wxT("m_viewMenu_Column_4"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_4             , this ) },
		{ wxT("m_viewMenu_Column_5"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_5             , this ) },
		{ wxT("m_viewMenu_Column_6"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_6             , this ) },
		{ wxT("m_viewMenu_Column_7"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_7             , this ) },
		{ wxT("m_viewMenu_Column_8"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_8             , this ) },
		{ wxT("m_viewMenu_Column_9"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_Column_9             , this ) },
		{ wxT("m_viewMenu_DefaultFile"),	        std::bind(&CNextMExecuteMenuOperation::ViewMenu_FileView_Default     , this ) },
		{ wxT("m_viewMenu_HiddenFile"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_FileView_Hidden      , this ) },
		{ wxT("m_viewMenu_AllFile"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_FileView_All         , this ) },
		{ wxT("m_viewMenu_SortName"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_SortDefault          , this ) },
		{ wxT("m_viewMenu_SortTime"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_SortTime             , this ) },
		{ wxT("m_viewMenu_SortSize"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_SortSize             , this ) },
		{ wxT("m_viewMenu_SubDir"),	                std::bind(&CNextMExecuteMenuOperation::ViewMenu_SubDir               , this ) },
		{ wxT("m_viewMenu_DirNumber"),	            std::bind(&CNextMExecuteMenuOperation::ViewMenu_DirNum               , this ) },
		//즐겨찾기
		{ wxT("m_favoriteMenu_Add"),	            std::bind(&CNextMExecuteMenuOperation::FavoriteMenu_Add              , this ) },
		{ wxT("m_favoriteMenu_Manager"),	        std::bind(&CNextMExecuteMenuOperation::FavoriteMenu_Manager          , this ) },

		//System 메뉴
		{ wxT("m_SystemMenu_Resource"),	            std::bind(&CNextMExecuteMenuOperation::SystemMenu_ResourceUsage      , this ) },
		{ wxT("m_toolSystem_DiskSize"),	            std::bind(&CNextMExecuteMenuOperation::SystemMenu_DiskUsage          , this ) },
		//도구메뉴
		{ wxT("m_toolMenu_ENV"),	                std::bind(&CNextMExecuteMenuOperation::ToolMenu_Setting              , this) },
		{ wxT("m_toolMenu_CurSave"),	            std::bind(&CNextMExecuteMenuOperation::ToolMenu_SaveSetting          , this) },
		//도움말
		{ wxT("m_helpMenu_thisProgram"),	        std::bind(&CNextMExecuteMenuOperation::HelpMenu_ThisProgram          , this) },
	};

	for(unsigned int i = 0; i < WXSIZEOF(stMenufnList); i++)
	{
		int iMenuID = XRCID(stMenufnList[i].strMenuID);
		m_mapMenuFunctionPtr.insert(std::make_pair(iMenuID, stMenufnList[i].funcPtr));
	}
}

void CNextMExecuteMenuOperation::ExecuteMenuOperation(int iMenuID, _MENU_EVENT_TYPE _mType)
{
	if(_mType != _MENU_FAVORITE_ITEMS)
	{
		std::unordered_map< int, std::function<void()> >::const_iterator fIter = m_mapMenuFunctionPtr.find(iMenuID);
		if(fIter == m_mapMenuFunctionPtr.end())
			return;

		auto functionPtr = fIter->second;
		(functionPtr)();
	}
	else
	{
		FavoriteMenu_ItemSelected(iMenuID);
	}
}

CFileListView* CNextMExecuteMenuOperation::GetFileListView()
{
	return theSplitterManager->GetCurrentViewManager()->GetFileListView();
}

//파일메뉴
void CNextMExecuteMenuOperation::FileMenu_Execution()
{

}

void CNextMExecuteMenuOperation::FileMenu_NewTab()
{
	CNextMViewManager* pCurrentViewManager = theSplitterManager->GetCurrentViewManager();
	pCurrentViewManager->AddTabPage();
}

void CNextMExecuteMenuOperation::FileMenu_Copy()
{
	DoFileOperation(FILE_OP_COPY);
}

void CNextMExecuteMenuOperation::FileMenu_Move()
{
	DoFileOperation(FILE_OP_CUT);
}

void CNextMExecuteMenuOperation::DoFileOperation(FILE_OPERATION fop, bool IsUseClipboard)
{
	std::list<wxString> lstSrc;

	wxString strDest(wxT(""));
	wxString strTgtDirTmp(wxT(""));
	bool bSameDir = false;

	CFileListView* pFileListView = nullptr;

	if(!theDnD->IsDragging())
	{
		pFileListView = GetFileListView();

		strDest = pFileListView->GetCurrentPath();

		if(!IsUseClipboard)
		{
			strTgtDirTmp = GetDirectorySelect();
			pFileListView->GetSelectedItems(lstSrc);
		}
		else
		{
			fop = theClipboard->GetFileOperation() == _MENU_EDIT_COPY_CLIPBOARD ? FILE_OP_COPY : FILE_OP_CUT;
			theClipboard->GetData(lstSrc);

			strTgtDirTmp = theClipboard->GetSrcPath();
		}

		if(!IsUseClipboard)
			strDest = strTgtDirTmp;
	}
	else
	{
		lstSrc = theDnD->GetDropDatas();
		CFileListView* pTargetView = (CFileListView *)theDnD->GetDropWindow();
		CFileListView* pSourceView = nullptr;
		if(theDnD->GetDragWindow() != nullptr)
		{
			pSourceView = (CFileListView *)theDnD->GetDragWindow();
			strTgtDirTmp = pSourceView->GetCurrentPath();
		}

		strDest = pTargetView->GetCurrentPath();

		if(theDnD->IsDropDirectory())
			strDest = pTargetView->GetCurrentItem();
	}

	if(lstSrc.size() == 0)
		return;

	if(strDest.CmpNoCase(strTgtDirTmp) == 0)
		bSameDir = true;

#ifdef __WXMSW__
	bool IsCopy = fop == FILE_OP_COPY ? true : false;
	bool bRet = theFileOperation->CopyOrMoveOperations(lstSrc, strDest, _gMainFrame, IsCopy, bSameDir);

#endif // __WXMSW__

	if(!theDnD->IsDragging())
		pFileListView->SelectedItemsClear();
}

#ifdef __WXMSW__
static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM dwData)
{
	switch (uMsg)
	{
		case BFFM_INITIALIZED:
		{
			//캡션에서 Help 삭제
			DWORD dwStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			::SetWindowLong(hWnd, GWL_EXSTYLE, dwStyle & ~WS_EX_CONTEXTHELP);

			HWND hStatic = GetDlgItem(hWnd, 0x00003742);//14146); // 0x00003742(Static Control)
			if(hStatic)
			{
				::EnableWindow(hStatic, FALSE);
				::ShowWindow(hStatic, SW_HIDE);
			}

			//지정된 폴더가 선택되도록 한다.
			::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, dwData);

			//대화상자가 중앙에 오도록 한다.
			RECT rc;
			GetClientRect(hWnd, &rc);
			::SetWindowPos(hWnd
				    	 , NULL
					     , (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2
					     , (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2
					     , 0
					     , 0
					     , SWP_NOZORDER | SWP_NOSIZE);

			//Status text에 3D 테두리 첨가
			HWND hwndLabel = ::GetDlgItem(hWnd, 0x3743);
			if (IsWindow(hwndLabel))
			{
				TCHAR szClass[MAX_PATH] = {0};
				::GetClassName(hwndLabel, szClass, MAX_PATH);
				if (lstrcmpi(szClass, __TEXT("static")))
					break;
			}
			else
				break;

			dwStyle = ::GetWindowLong(hwndLabel, GWL_EXSTYLE);
			::SetWindowLong(hwndLabel, GWL_EXSTYLE, dwStyle | WS_EX_STATICEDGE);
			::SetWindowPos(hwndLabel, NULL, 0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE | SWP_DRAWFRAME);
		}
			break;

		case BFFM_SELCHANGED:
		{
			TCHAR szText[MAX_PATH] = {0};
			::SHGetPathFromIDList(reinterpret_cast<LPITEMIDLIST>(lParam), szText);
			::SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, reinterpret_cast<LPARAM>(szText));
		}
			break;

		case BFFM_VALIDATEFAILED:
			break;
	}

	return 0;
}
#endif

wxString CNextMExecuteMenuOperation::GetDirectorySelect()
{
	CFileListView* pFileListView = GetFileListView();
	wxString strTargetPath(wxT(""));

	if(theConfig->GetSplitStyle() == WINDOW_SPLIT_NONE)
	{
		strTargetPath = pFileListView->GetCurrentPath();
		bool bWindowSelectDir = false;
	#ifdef __WXMSW__
		if(theConfig->GetUseSHBrowseForFolder())
		{
			bWindowSelectDir = true;
			LPITEMIDLIST pidlBrowse;
			TCHAR szDir[MAX_PATH] = {0, };

			BROWSEINFO brInfo;
			wxZeroMemory(brInfo);

			brInfo.hwndOwner = _gMainFrame->GetHWND();
			brInfo.pidlRoot = NULL;
			brInfo.pszDisplayName = szDir;
			brInfo.lpszTitle = _T("Select copy or move target directory");
			brInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_VALIDATE;
			brInfo.lpfn = BrowseCallbackProc;
			brInfo.lParam = (LPARAM)strTargetPath.wc_str();

			pidlBrowse = ::SHBrowseForFolder(&brInfo);
			if(pidlBrowse != NULL)
			{
				::SHGetPathFromIDList(pidlBrowse, szDir);
				strTargetPath = wxString::Format(wxT("%s"), szDir);
			}
		}
	#endif

		if(!bWindowSelectDir)
		{
			wxGenericDirDialog dlgTargetPath(_gMainFrame, wxT("Select target directory"), strTargetPath);
			dlgTargetPath.SetSize(wxSize(800, 600));
			dlgTargetPath.CenterOnScreen();

			if(dlgTargetPath.ShowModal() == wxID_OK)
				strTargetPath = dlgTargetPath.GetPath();

			dlgTargetPath.Destroy();
		}
	}
	else
	{
		pFileListView = theSplitterManager->GetViewManager2()->GetFileListView();
		strTargetPath = pFileListView->GetCurrentPath();
	}

	return strTargetPath;
}

void CNextMExecuteMenuOperation::FileMenu_Rename()
{
	theSplitterManager->GetCurrentViewManager()->SendEvent(wxEVT_ITEM_RENAME);
}

void CNextMExecuteMenuOperation::FileMenu_CreateFolder()
{
	CFileListView* pFileListView = GetFileListView();
	wxString strDest(pFileListView->GetCurrentPath());

	DlgAddDir dlgDir(_gMainFrame);
	dlgDir.SetCurrentPath(strDest);
	dlgDir.ShowModal();
	dlgDir.Destroy();
}

void CNextMExecuteMenuOperation::FileMenu_Edit()
{
	theSplitterManager->GetCurrentViewManager()->SendEvent(wxEVT_FILE_EDIT);
}

void CNextMExecuteMenuOperation::FileMenu_Trash()
{
	ExecuteDeleteProcess(_MENU_FILE_DEL_TRASH);
}

void CNextMExecuteMenuOperation::FileMenu_CompletionDel()
{
	ExecuteDeleteProcess(_MENU_FILE_DEL_COMP);
}

void CNextMExecuteMenuOperation::ExecuteDeleteProcess(_MENU_EVENT_TYPE mType)
{
	bool bTrash = (mType == _MENU_FILE_DEL_TRASH) ? true : false;

	std::list<wxString> lstSrc;
	CFileListView* pFileListView = GetFileListView();

	pFileListView->GetSelectedItems(lstSrc);

	if(lstSrc.size() == 0)
		return;

	int iRet = 0;
	theFileOperation->RecursiveDelete(lstSrc, _gMainFrame, bTrash, iRet);

	pFileListView->SelectedItemsClear();
	pFileListView->SetFocus();
}

void CNextMExecuteMenuOperation::FileMenu_ExecCMD()
{
	CFileListView* pFileListView = GetFileListView();
	wxString strPath = pFileListView->GetCurrentPath();

		wxString strCommand(wxT(""));
#ifdef __WXMSW__
	wxString strVolume = strPath.Left(2);
	wxString strChildPath(wxT(""));
	size_t nSlash = 3;
	if(nSlash < strPath.Len())
		strChildPath = strPath.Mid(nSlash, strPath.Len());

	wxString strArgs(wxT(" /s /k "));
	if(!strChildPath.IsEmpty())
	{
		strArgs += strVolume;
		strArgs += wxT(" && cd ");
		strArgs += wxT("\"") + strPath + wxT("\"");
	}
	else
	{
		strArgs += strVolume;
		strArgs += wxT(" && cd \\");
	}

	strCommand = wxT("C:\\Windows\\System32\\CMD.exe") + strArgs;
#else

#endif

	wxExecute(strCommand);

}

void CNextMExecuteMenuOperation::FileMenu_Terminate()
{
	wxTheApp->GetTopWindow()->Close();
}

//편집메뉴
void CNextMExecuteMenuOperation::EditMenu_AllRelease()
{
	CFileListView* pFileListView = GetFileListView();
	pFileListView->SelectAllItemOrRelease(false);
}

void CNextMExecuteMenuOperation::EditMenu_AllSelect()
{
	CFileListView* pFileListView = GetFileListView();
	pFileListView->SelectAllItemOrRelease(true);
}

void CNextMExecuteMenuOperation::EditMenu_Copy_UseClipboard()
{
	DoClipboardOperation(false);
}

void CNextMExecuteMenuOperation::EditMenu_Move_UseClipboard()
{
	DoClipboardOperation(true);
}

void CNextMExecuteMenuOperation::DoClipboardOperation(bool IsCut)
{
	std::list<wxString> lstSrc;
	CFileListView* pFileListView = GetFileListView();

	pFileListView->GetSelectedItems(lstSrc, IsCut);

	if(lstSrc.size() == 0)
		return;

	theClipboard->Clear();
	theClipboard->SetFileOperation(IsCut ? _MENU_EDIT_MOVE_CLIPBOARD : _MENU_EDIT_COPY_CLIPBOARD);
	theClipboard->AddData(lstSrc);
	theClipboard->CopyToClipboard();

#ifdef __WXMSW__
	theClipboard->SetSrcPath(pFileListView->GetCurrentPath());
#endif // __WXMSW__

}

void CNextMExecuteMenuOperation::EditMenu_Paste_UseClipboard()
{
	DoFileOperation(FILE_OP_NONE, true);
}

void CNextMExecuteMenuOperation::EditMenu_FileFind()
{
	CFileListView* pFileListView = GetFileListView();
	wxString strPath = pFileListView->GetCurrentPath();

	DlgFind findDlg(_gMainFrame);
	findDlg.setSearchDir(strPath);
	findDlg.ShowModal();
	findDlg.Destroy();
}

void CNextMExecuteMenuOperation::EditMenu_ViewContextMenu()
{
	CFileListView* pFileListView = GetFileListView();

	wxCommandEvent evt(wxEVT_SHOW_CONTEXTMENU);
	wxPostEvent(pFileListView, evt);
}

//경로메뉴
void CNextMExecuteMenuOperation::PathMenu_DirManager()
{
	CFileListView* pFileListView = GetFileListView();
	wxString strPath = pFileListView->GetCurrentPath();

	wxString strVolume = strPath.Left(1);
	wxString strTreeDataPath = theUtility->GetWorkDir() + SLASH + wxT("treedata");
	if (!wxDirExists(strTreeDataPath))
		wxMkDir(strTreeDataPath);

	wxString strNCDFilePath = strTreeDataPath + SLASH + strPath.Left(1) + wxT(".NCD");

	DlgDirManager dlgManager(_gMainFrame);
	dlgManager.SetNCDFileInfo(strNCDFilePath, strPath);
	dlgManager.ShowModal();
	dlgManager.Destroy();
}

void CNextMExecuteMenuOperation::PathMenu_Refresh()
{
	ExecutePathMenuOperation(_MENU_PATH_REFRESH);
}

void CNextMExecuteMenuOperation::PathMenu_GoRoot()
{
	ExecutePathMenuOperation(_MENU_PATH_GOROOT);
}

void CNextMExecuteMenuOperation::PathMenu_GoUp()
{
	ExecutePathMenuOperation(_MENU_PATH_GOUP);
}

void CNextMExecuteMenuOperation::PathMenu_GoDown()
{
	ExecutePathMenuOperation(_MENU_PATH_GODOWN);
}

void CNextMExecuteMenuOperation::PathMenu_GoBack()
{
	ExecutePathMenuOperation(_MENU_PATH_BACK);
}

void CNextMExecuteMenuOperation::PathMenu_GoFoward()
{
	ExecutePathMenuOperation(_MENU_PATH_FOWARD);
}

void CNextMExecuteMenuOperation::ExecutePathMenuOperation(_MENU_EVENT_TYPE mType)
{
	CFileListView* pFileListView = GetFileListView();

	wxCommandEvent evt(wxEVT_EXEC_MENU_OPERATION);
	evt.SetId(mType);

	wxPostEvent(pFileListView, evt);
}

void CNextMExecuteMenuOperation::PathMenu_GotoDirDirectly()
{
	CFileListView* pFileListView = GetFileListView();
	wxString strPath(pFileListView->GetCurrentPath());

	wxGenericDirDialog dlgTargetPath(_gMainFrame, wxT("Select target directory"), strPath);
	dlgTargetPath.SetSize(wxSize(600, 450));
	dlgTargetPath.CenterOnScreen();

	if(dlgTargetPath.ShowModal() == wxID_OK)
	{
		wxString strTargetDir(dlgTargetPath.GetPath());

		theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strTargetDir, CHANGE_DIR_FILELISTIVEW);
		theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strTargetDir, CHANGE_DIR_TAB);
		theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strTargetDir, CHANGE_DIR_PATHVIEW);
	}
}

//압축
void CNextMExecuteMenuOperation::ShowCompressPopupMenu()
{
	CFileListView* pFileListView = GetFileListView();
	pFileListView->ShowCompressPopupMenu();
}

void CNextMExecuteMenuOperation::ShowDecompressMenu()
{
	CFileListView* pFileListView = GetFileListView();
	pFileListView->ShowCompressPopupMenu();
}

void CNextMExecuteMenuOperation::CompressMenu_DoCompress()
{
	theComDec->SetImmediatelyCompress(false);
	CompressMenu_ExecuteCompress(COMPRESS_START_ID);
}

void CNextMExecuteMenuOperation::CompressMenu_DeCompressCurDir()
{
	CFileListView* pFileListView = GetFileListView();
	if(!pFileListView->IsAvaliableDecompress())
		return;

	wxString strCompressedFile = pFileListView->GetCurrentItem();
	CompressMenu_ExecuteDecompress(DECOMPRESS_START_ID, strCompressedFile);
}

void CNextMExecuteMenuOperation::CompressMenu_DeCompressMkDir()
{
	CFileListView* pFileListView = GetFileListView();
	if(!pFileListView->IsAvaliableDecompress())
		return;

	wxString strCompressedFile = pFileListView->GetCurrentItem();
	CompressMenu_ExecuteDecompress(DECOMPRESS_START_ID + 1, strCompressedFile);
}

void CNextMExecuteMenuOperation::CompressMenu_DeCompressSelDir()
{
	CFileListView* pFileListView = GetFileListView();
	if(!pFileListView->IsAvaliableDecompress())
		return;

	wxString strCompressedFile = pFileListView->GetCurrentItem();
	CompressMenu_ExecuteDecompress(DECOMPRESS_START_ID + 2, strCompressedFile);
}

void CNextMExecuteMenuOperation::CompressMenu_ExecuteCompress(int nId)
{
	CompressMenu_ExecuteCompress(nId, wxT(""));
}

void CNextMExecuteMenuOperation::CompressMenu_ExecuteCompress(int nId, const wxString& strCompressedFile)
{
	std::list<wxString> lstSrc;

	CFileListView* pFileListView = GetFileListView();
	pFileListView->GetSelectedItems(lstSrc);

	if(lstSrc.size() == 0)
		return;

	pFileListView->SelectedItemsClear();

	int compressExtIndex = nId - COMPRESS_START_ID;

	wxString strFullPath(pFileListView->GetCurrentPath());

	wxString strCompressedExt = theComDec->GetCompressList().at(compressExtIndex);
	strCompressedExt = strCompressedExt.MakeLower();

	wxString strCompressedPath(wxT(""));
	wxString strCompressedName(wxT(""));

	if(strCompressedFile.CmpNoCase(wxT("")) == 0)
	{
		if(lstSrc.size() == 1)
		{
			wxString strCompressItem = lstSrc.front();
			wxString strPathFileName = theUtility->GetPathName(strCompressItem);
			strCompressedName = theUtility->GetFileName(strPathFileName, false);
		}
		else
			strCompressedName = strFullPath.Len() == 3 ? strFullPath.Left(1) : theUtility->GetPathName(strFullPath);

		strCompressedPath = strFullPath.Len() == 3 ? strFullPath : strFullPath + SLASH;
		strCompressedFile = strCompressedPath + strCompressedName + wxT(".") + strCompressedExt;
	}

	DlgCompress dlg(_gMainFrame);
	dlg.SetCompressInfo(lstSrc, strCompressedFile, wxT(""), strCompressedExt);
	dlg.ShowModal();
	dlg.Destroy();
}

void CNextMExecuteMenuOperation::CompressMenu_ExecuteDecompress(int nId, const wxString& strCompressedFile)
{
	CFileListView* pFileListView = GetFileListView();
	wxString strFullPath(pFileListView->GetCurrentPath());

	DlgDeCompress dlg(_gMainFrame);

	dlg.SetDecompressInfo(strCompressedFile, strFullPath, nId);
	dlg.ShowModal();
	dlg.Destroy();
}

//보기메뉴
void CNextMExecuteMenuOperation::ViewMenu_FullScreen()
{
	_gMainFrame->Maximize();
}

void CNextMExecuteMenuOperation::ViewMenu_Window_Single()
{
	ChangeSplitStyle(WINDOW_SPLIT_NONE);
}

void CNextMExecuteMenuOperation::ViewMenu_Window_Vert()
{
	ChangeSplitStyle(WINDOW_SPLIT_VERT);
}

void CNextMExecuteMenuOperation::ViewMenu_Window_Horz()
{
	ChangeSplitStyle(WINDOW_SPLIT_HORZ);
}

void CNextMExecuteMenuOperation::ChangeSplitStyle(int iSplitStyle)
{
	int iCurrentSplitStyle = theConfig->GetSplitStyle();
	if(iCurrentSplitStyle != iSplitStyle)
	{
		theConfig->SetChangeGlobalVal(wxT("SplitWindowStyle"), iSplitStyle);
		theSplitterManager->SplitWindow();

		CFileListView* pFileListView = GetFileListView();
		pFileListView->SetFocus();
	}
}

void CNextMExecuteMenuOperation::ViewMenu_Column_0()
{
	ChangeViewColumn(VIEW_COLUMN_AUTO);
}

void CNextMExecuteMenuOperation::ViewMenu_Column_1()
{
	ChangeViewColumn(VIEW_COLUMN_ONE);
}

void CNextMExecuteMenuOperation::ViewMenu_Column_2()
{
	ChangeViewColumn(VIEW_COLUMN_TWO);
}

void CNextMExecuteMenuOperation::ViewMenu_Column_3()
{
	ChangeViewColumn(VIEW_COLUMN_THREE);
}

void CNextMExecuteMenuOperation::ViewMenu_Column_4()
{
	ChangeViewColumn(VIEW_COLUMN_FOUR);
}

void CNextMExecuteMenuOperation::ViewMenu_Column_5()
{
	ChangeViewColumn(VIEW_COLUMN_FIVE);
}

void CNextMExecuteMenuOperation::ViewMenu_Column_6()
{
	ChangeViewColumn(VIEW_COLUMN_SIX);
}

void CNextMExecuteMenuOperation::ViewMenu_Column_7()
{
	ChangeViewColumn(VIEW_COLUMN_SEVEN);
}

void CNextMExecuteMenuOperation::ViewMenu_Column_8()
{
	ChangeViewColumn(VIEW_COLUMN_EIGHT);
}

void CNextMExecuteMenuOperation::ViewMenu_Column_9()
{
	ChangeViewColumn(VIEW_COLUMN_NINE);
}

void CNextMExecuteMenuOperation::ChangeViewColumn(COLUMN_VIEW_OPTION _colNum)
{
	theConfig->SetChangeGlobalVal(wxT("DispColumn"), _colNum);
	theSplitterManager->GetCurrentViewManager()->SendEvent(wxEVT_CHANGE_VIEW_COLUMN);
}

void CNextMExecuteMenuOperation::ViewMenu_FileView_Default()
{
	_gAttrIndex = 0;

	theConfig->SetChangeGlobalVal(wxT("DispHiddenFile"), 0);
	theConfig->SetChangeGlobalVal(wxT("DispSystemFile"), 0);
	theConfig->SetChangeGlobalVal(wxT("DispAllFile"), 0);

	ChangeViewFileListOption(FILE_VIEW_OP_DEFAULT);
}

void CNextMExecuteMenuOperation::ViewMenu_FileView_Hidden()
{
	_gAttrIndex = 1;

	bool bHidden = theConfig->IsViewHiddenFile() ? false : true;

	theConfig->SetChangeGlobalVal(wxT("DispHiddenFile"), bHidden ? 1 : 0);
	theConfig->SetChangeGlobalVal(wxT("DispAllFile"), 0);

	ChangeViewFileListOption(FILE_VIEW_OP_HIDDEN);
}

void CNextMExecuteMenuOperation::ViewMenu_FileView_All()
{
	_gAttrIndex = 2;

	bool bAll = theConfig->IsViewAllFile() ? false : true;

	theConfig->SetChangeGlobalVal(wxT("DispHiddenFile"), 0);
	theConfig->SetChangeGlobalVal(wxT("DispSystemFile"), 0);
	theConfig->SetChangeGlobalVal(wxT("DispAllFile"), bAll ? 1 : 0);

	ChangeViewFileListOption(FILE_VIEW_OP_ALL);
}

void CNextMExecuteMenuOperation::ChangeViewFileListOption(FILE_VIEW_OPTION _fvo)
{
	ExecutePathMenuOperation(_MENU_PATH_REFRESH);
}

void CNextMExecuteMenuOperation::ViewMenu_SortDefault()
{
	ChangeViewSortOption(VIEW_SORT_DEFAULT);
}

void CNextMExecuteMenuOperation::ViewMenu_SortTime()
{
	ChangeViewSortOption(VIEW_SORT_TIME);
}

void CNextMExecuteMenuOperation::ViewMenu_SortSize()
{
	ChangeViewSortOption(VIEW_SORT_SIZE);
}

void CNextMExecuteMenuOperation::ChangeViewSortOption(VIEW_SORT sType)
{
	theConfig->SetChangeGlobalVal(wxT("SortType"), sType);
	theSplitterManager->GetCurrentViewManager()->SendEvent(wxEVT_CHANGE_VIEW_SORT);
}

void CNextMExecuteMenuOperation::ViewMenu_SubDir()
{
}

void CNextMExecuteMenuOperation::ViewMenu_DirNum()
{
	CFileListView* pFileListView = GetFileListView();

	wxCommandEvent evt(wxEVT_VIEW_DIR_NUM);
	wxPostEvent(pFileListView, evt);
}
//즐겨찾기
void CNextMExecuteMenuOperation::FavoriteMenu_Add()
{
	BookmarkOperation(_MENU_FAVORITE_ITEM_ADD);
}

void CNextMExecuteMenuOperation::FavoriteMenu_Manager()
{
	BookmarkOperation(_MENU_FAVORITE_MANAGER);
}

void CNextMExecuteMenuOperation::BookmarkOperation(_MENU_EVENT_TYPE mType)
{
	DlgFavoriteManager dlg(_gMainFrame);
	if (mType == _MENU_FAVORITE_ITEM_ADD)
	{
		CFileListView* pFileListView = GetFileListView();
		wxString strCurrentPath(pFileListView->GetCurrentPath());

		dlg.SetAddPath(strCurrentPath, true);
	}

	dlg.ShowModal();
	dlg.Destroy();

	m_pMenuEvent->LoadBookmarkToMenu(true);
}

void CNextMExecuteMenuOperation::FavoriteMenu_ItemSelected(int iMenuID)
{
	wxString strBookmarkPath = theBookmark->GetBookmarkPath(iMenuID);

	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strBookmarkPath, CHANGE_DIR_FILELISTIVEW);
	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strBookmarkPath, CHANGE_DIR_TAB);
	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strBookmarkPath, CHANGE_DIR_PATHVIEW);
}
//System메뉴
void CNextMExecuteMenuOperation::SystemMenu_ResourceUsage()
{
}

void CNextMExecuteMenuOperation::SystemMenu_DiskUsage()
{
}
//도구메뉴
void CNextMExecuteMenuOperation::ToolMenu_Setting()
{
	DlgEnv dlg(_gMainFrame);
	dlg.ShowModal();
	dlg.Destroy();
}

void CNextMExecuteMenuOperation::ToolMenu_SaveSetting()
{
	int xPos = 0;
	int yPos = 0;

	int nCurrDisp = wxDisplay::GetFromWindow(_gMainFrame);

	wxDisplay disp(nCurrDisp);
	wxRect rcMonitor = disp.GetGeometry();

	wxTopLevelWindow* pTopWindow = (wxTopLevelWindow *)wxTheApp->GetTopWindow();
	pTopWindow->GetScreenPosition(&xPos, &yPos);
	wxRect rcScreen = pTopWindow->GetScreenRect();

	int iMaximized = _gMainFrame->IsMaximized() ? 1 : 0;

	if (rcMonitor.GetLeft() != 0)
		xPos = rcMonitor.GetLeft();

	int iWidth = rcScreen.GetWidth();
	int iHeight = rcScreen.GetHeight();

	theConfig->SetChangeGlobalVal(wxT("Maximized"), iMaximized);
	theConfig->SetChangeGlobalVal(wxT("DispMonitor"), nCurrDisp);
	theConfig->SetChangeGlobalVal(wxT("XPos"), xPos);
	theConfig->SetChangeGlobalVal(wxT("YPos"), yPos);
	theConfig->SetChangeGlobalVal(wxT("Width"), iWidth);
	theConfig->SetChangeGlobalVal(wxT("Height"), iHeight);

	if (theConfig->IsSaveLastSession())
	{
		CFileListView* pFileListView = GetFileListView();
		CFileListView* pFileListView2 = nullptr;

		if(theConfig->GetSplitStyle() != WINDOW_SPLIT_NONE)
			pFileListView2 = theSplitterManager->GetViewManager2()->GetFileListView();

		wxString strActiveKey(wxT(""));
		wxString strAnotherKey(wxT(""));

		bool bLeft = theSplitterManager->GetCurrentViewManager()->IsLeft();
		if(bLeft)
		{
			strActiveKey = wxT("LeftPath");
			strAnotherKey = wxT("RightPath");
		}
		else
		{
			strActiveKey = wxT("RightPath");
			strAnotherKey = wxT("LeftPath");
		}

		wxString strCurrentPath = pFileListView->GetCurrentPath();
		theConfig->SetChangeGlobalVal(strActiveKey, strCurrentPath);

		if(pFileListView2)
		{
			strCurrentPath = pFileListView2->GetCurrentPath();
			theConfig->SetChangeGlobalVal(strAnotherKey, strCurrentPath);
		}
	}

	theConfig->SaveDoc();
	theUtility->ShowMessageBox(theMsg->GetMessage(wxT("MSG_ENV_CONFIG_SAVE")), wxOK | wxICON_INFORMATION);
}
//이프로그램은..
void CNextMExecuteMenuOperation::HelpMenu_ThisProgram()
{
		// Create About box
	wxMozillaLikeAboutBoxDialog * dlg = new wxMozillaLikeAboutBoxDialog(_gMainFrame);

	// Set application name
	dlg->SetAppName(PROGRAM_NAME);
	// Set application version
	dlg->SetVersion(PROGRAM_MAJOR_VER + wxT(".") + PROGRAM_MINOR_HI_VER + wxT(".") + PROGRAM_MINOR_LO_VER + ENV_OS_VER);
	// Set copyright message
	wxDateTime dt = wxDateTime::Now();
	int iYear = dt.GetYear();
	wxString strCopyRight = wxString::Format(theMsg->GetMessage(wxT("MSG_PROGRAMM_COPYRIGHT")), iYear);
	dlg->SetCopyright(strCopyRight);
	// Set header bitmap
	//	dlg->SetHeaderBitmap(wxGetBitmapFromMemory(header_png, sizeof(header_png)));

	wxString strBuildDisp(wxT(""));
	wxString strBuildInfo(wxMozillaLikeAboutBoxDialog::GetBuildInfo(wxMozillaLikeAboutBoxDialog::wxBUILDINFO_LONG).GetData());
	wxString strDeveloper(wxT("Developed by "));
	strDeveloper += theMsg->GetMessage(wxT("MSG_PROGRAMM_DEVELOPER"));

	strBuildInfo = wxString::Format(wxT("%s. \n%s"), strBuildInfo, strDeveloper);
	// Set build info message. This is optional step. If you don't specify build info message then
	// default one will be used
	dlg->SetCustomBuildInfo(wxT("Advanced File Manager with ") + strBuildInfo);
	// Apply changes
	dlg->ApplyInfo();
	// Show dialog
	dlg->ShowModal();
	// Destroy dialog
	dlg->Destroy();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNextMExecuteMenuOperation::SetVisitDirDropDownMenu(wxAuiToolBar* tb, wxAuiToolBarEvent& event)
{
	CFileListView* pFileListView = GetFileListView();

	std::vector<wxString>::iterator itBegin = pFileListView->BackFowardDirBegin();
	std::vector<wxString>::iterator itEnd = pFileListView->BackFowardDirEnd();

	tb->SetToolSticky(event.GetId(), true);

	wxMenu menuPopup;

	int iIndex = 0;
	for(itBegin; itBegin != itEnd; ++itBegin)
	{
		menuPopup.AppendCheckItem(wxBACKFORWARD_START + iIndex, *itBegin);
		iIndex++;
	}

	wxRect rect = tb->GetToolRect(event.GetId());
	wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());

	pt = _gMainFrame->ScreenToClient(pt);
	_gMainFrame->PopupMenu(&menuPopup, pt);

	tb->SetToolSticky(event.GetId(), false);
}

void CNextMExecuteMenuOperation::UpdateBackFowrdMenu(wxUpdateUIEvent& event)
{
	CFileListView* pFileListView = GetFileListView();
	int iBackForwardIndex = pFileListView->GetBackFowardIndex();

	event.Check(false);
	int nMenuID = event.GetId();

	if(iBackForwardIndex == (nMenuID - wxBACKFORWARD_START))
		event.Check(true);
}

void CNextMExecuteMenuOperation::GotoBackForwardDirectory(int iBackForwardIndex)
{
	CFileListView* pFileListView = GetFileListView();
	std::vector<wxString>::iterator it = pFileListView->BackFowardDirBegin() + iBackForwardIndex;

	pFileListView->SetBackForwardIndex(iBackForwardIndex);

	wxString strDirectory(*it);

	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strDirectory, CHANGE_DIR_FILELISTIVEW);
	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strDirectory, CHANGE_DIR_TAB);
	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strDirectory, CHANGE_DIR_PATHVIEW);
}

void CNextMExecuteMenuOperation::ShowFavoriteFromStatusBar()
{
	CFileListView* pFileListView = GetFileListView();

	wxCommandEvent evt(wxEVT_VIEW_FAVORITE_FROM_STATUS);
	wxPostEvent(pFileListView, evt);

}

void CNextMExecuteMenuOperation::DragDropOperation()
{
	FILE_OPERATION fop;

#ifdef __WXMSW__
	DWORD dwEffect = theDnD->GetDropOperation();
	fop = dwEffect == DROPEFFECT_COPY ? FILE_OP_COPY :
		  dwEffect == DROPEFFECT_MOVE ? FILE_OP_CUT : FILE_OP_NONE;
#endif // __WXMSW__

	DoFileOperation(fop);
}
