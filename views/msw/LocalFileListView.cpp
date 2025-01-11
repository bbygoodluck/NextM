#include "../../ginc.h"
#include "LocalFileListView.h"

/*
 * CLocalFileListView
 *   - 윈도우버전용 파일리스트 뷰
 *   - 윈도우 디렉토리의 하위 디렉토리, 파일을 읽어서 화면에 보여줌.
 *
*/
wxBEGIN_EVENT_TABLE(CLocalFileListView, CFileListView)
	//디스크 드라이브 초기화
	EVT_MY_CUSTOM_COMMAND(wxEVT_DISK_DRIVE_INIT, wxID_ANY, CLocalFileListView::OnDiskDriveInit)
	//드라이브 사이즈 변경시 해당 디스크 사이즈 갱신
	EVT_MY_CUSTOM_COMMAND(wxEVT_UPDATE_DRIVE_SIZEINFO, wxID_ANY, CLocalFileListView::OnUpdateDriveSizeInfo)
	//파일 시스템 Watcher
	EVT_MY_CUSTOM_COMMAND(wxEVT_FS_WATCHER, wxID_ANY, CLocalFileListView::OnFileSystemWatcher)
wxEND_EVENT_TABLE()

CLocalFileListView::CLocalFileListView(wxWindow* parent, const int nID, const wxSize& sz)
	: CFileListView(parent, nID, sz)
{
	m_pFileSystemWatcher = std::make_unique<CLocalFileSystemWatcher>(this);
}

CLocalFileListView::~CLocalFileListView()
{
}

bool CLocalFileListView::LoadDirectory(const wxString& strPath)
{
	wxBusyCursor wait;

	if(!wxIsReadable(strPath))
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_DIREDTORY_READ_FAIL")), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return false;
	}

	//파일 시스템 아이콘 읽기 쓰레드 중지
	StopFileImageRead();

	//초기화
    Initialize();

	//상위디렉토리 이동시 해당 상위 디렉토리의 시작 인덱스를 가져옴
	_HISTORY::const_iterator fIter = m_hashHistory.find(strPath);
	if(fIter != m_hashHistory.end())
	{
		_HistoryItem historyItem = fIter->second;

		m_iCurrentItemIndex = historyItem.iCurrentIndex;
		m_iStartIndex = historyItem.iStartIndex;
	}

	//현재 패스 저장
    m_strCurrentPath = strPath;

	//디스크용량 설정(드라이브 변경시)
	wxString strVolume = strPath.Left(1);
	if(m_strVolume.IsEmpty() || m_strVolume.CmpNoCase(strVolume) != 0)
	{
		m_strVolume = strVolume;
		SetDiskSpace(strVolume);
	}

	//디렉토리와 파일을 읽어옴..
	CLocalFileSystem localFileSys;
	if (!localFileSys.BeginFindFiles(strPath, false))
	{
		wxMessageBox(theMsg->GetMessage(wxT("MSG_SEARCH_DIR_INIT_ERR")), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);
		return false;
	}

	wxString strDesc(wxT(""));
	wxString strName(wxT(""));
	wxString strExt(wxT(""));
	bool isDir = false;
	unsigned long lattr = 0;
	wxLongLong llSize(0);
	wxDateTime dt(0.0);

	wxString strFullPath(wxT(""));
	wxString strTmpName(wxT(""));

	while (localFileSys.GetNextFile(strName, &isDir, &lattr, &llSize, &dt))
	{
		/**************************************************************************
		 * 윈도우 11에서는 .. 의 속성이 HIDDEN을 포함
		 *    아래의 경우해당
		 *       C:\                     - 해당없음
		 *       C:\Windows              - 해당(최상위 + 1 단계의 경우만 해당함)
		 *       C:\Program Files\폴더명 - 해당없음
		 * > 상위폴더 이동표시는 모든 하위 폴더에서 표시가 되어야 하므로 강제 표시
		 **************************************************************************/
		if(strName.CmpNoCase(wxT("..")) != 0)
		{
			if (!theConfig->IsViewAllFile())
			{
				if(theUtility->IsAllowAttr(lattr))
					continue;
			}
		}

		strTmpName = strName;
		strTmpName.MakeLower();

		strFullPath = m_strCurrentPath + (m_strCurrentPath.Right(1).CmpNoCase(SLASH) == 0 ? strName : SLASH + strName);

		//아이템 등록
		CNextMDirData& dirItem = m_itemList.emplace_back();

		dirItem.SetName(strName);
		dirItem.SetIconIndex(0, 0);

		if (isDir)
		{
			dirItem.SetType(CNextMDirData::item_type::dir);
			m_iDirCount++;
			strDesc = theMsg->GetMessage(wxT("MSG_DIR_DESCRIPTION"));
		}
		else
		{
			dirItem.SetType(CNextMDirData::item_type::file);
			strExt = theUtility->GetExt(strName);

			m_iFileCount++;
			m_dblFileSizeInDir += llSize.ToDouble();

			int iIconIndex = 0;
			int iOverlayIconIndex = 0;

			if(theIconInfo->GetFileIconInfo(strExt, iIconIndex, iOverlayIconIndex, strDesc))
			{
				dirItem.SetImageIconFlag(true);
				dirItem.SetIconIndex(iIconIndex, iOverlayIconIndex);
			}
			else
				strDesc = theUtility->GetFileDescription(strExt,  strFullPath);
		}

		dirItem.SetAttribute(lattr);
		dirItem.SetSize(llSize);
		dirItem.SetDateTime(dt);
		dirItem.SetPath(m_strCurrentPath);
		dirItem.SetMatch(false);
		dirItem.SetExt(strExt);
		dirItem.SetDescription(strDesc);

//		m_itemList.push_back(dirItem);

		m_strMaxName = theUtility->GetMaxData(this, m_viewFont, strName, m_strMaxName);
		m_strMaxTypeName = theUtility->GetMaxData(this, m_viewFont, strDesc, m_strMaxTypeName);

		strName = wxT("");
		strDesc = wxT("");
		strExt = wxT("");

		isDir = false;
		lattr = 0;
		llSize = 0;
		dt = 0;
	}

	localFileSys.EndFindFiles();

	//Sorting 시작
	if(m_itemList.size() > 0)
		SortStart();

	m_iDriveItemIndex = m_itemList.size();
	InsertDiskDriveItems();

	m_iTotalItems = m_itemList.size();

	//파일(폴더) 이미지 읽기
	RunFileImageInfoRead();

	//디렉토리 감시 시작
	m_pFileSystemWatcher->StartWatch(strPath);

	m_bDirLoaded = true;
	theUtility->RefreshWindow(this, m_viewRect);

	return true;
}

void CLocalFileListView::InsertDiskDriveItems()
{
	//드라이브정보 복사
	std::copy(theDriveInfo->BeginIter(), theDriveInfo->EndIter(), std::back_inserter(m_itemList));
}

void CLocalFileListView::DeleteDiskDriveItems()
{
	while(1)
	{
		std::vector<CNextMDirData>::iterator it = m_itemList.end() - 1;
		if(!it->IsDrive())
			break;

		m_itemList.erase(it);
	}
}

void CLocalFileListView::ResetDiskDriveItems()
{
	DeleteDiskDriveItems();
	InsertDiskDriveItems();

	m_iTotalItems = m_itemList.size();

	ReCalcPage();

	m_bSizeOrColumnChanged = true;
	//파일시스템 감시의 변경사항이 아닌경우만 처리
	//파일시스템 감시상의 Refresh는 OnFileSystemWatcher함수에서 별도 처리됨
	theUtility->RefreshWindow(this, m_viewRect);
}

void CLocalFileListView::Render(wxDC* pDC)
{
	if(m_bSizeOrColumnChanged)
        ApplyChangedViewSize();

	// 컬럼계산
	CalcColumn(pDC);
	// 컬럼그리기
	DrawColumn(pDC);
	//정보표시창
	DrawInfoArea(pDC);
	//디렉토리 정보표시
	DisplayDirInfo(pDC);
	//데이터 표시
	DisplayItems(pDC);

	m_bSizeOrColumnChanged = false;
    m_bDirLoaded = false;
}

void CLocalFileListView::ApplyChangedViewSize()
{
	m_viewDirInfo.SetLeft(m_viewRect.GetLeft());
	m_viewDirInfo.SetTop(m_viewRect.GetTop() - 1);
	m_viewDirInfo.SetRight(m_viewRect.GetRight());
	m_viewDirInfo.SetBottom(m_viewRect.GetTop() + 20);

	m_viewRectDetail.SetLeft(m_viewRect.GetLeft());
	m_viewRectDetail.SetTop(m_viewRect.GetBottom() - 20);
	m_viewRectDetail.SetRight(m_viewRect.GetRight());
	m_viewRectDetail.SetBottom(m_viewRect.GetBottom());

	m_viewRectDisp.SetLeft(m_viewRect.GetLeft());
	m_viewRectDisp.SetTop(m_viewRect.GetTop() + 20);
	m_viewRectDisp.SetRight(m_viewRect.GetRight());
	m_viewRectDisp.SetBottom(m_viewRect.GetBottom() - 20);
}

void CLocalFileListView::GotoTopDir()
{
	wxString strTopDir = m_strCurrentPath.Left(1) + wxT(":") + SLASH;

	bool bSameDir = strTopDir.CmpNoCase(m_strCurrentPath) == 0;
	bool bExist = wxFileName::DirExists(strTopDir);
	if (bSameDir || !bExist)
	{
		if(!bExist)
		{
			wxString strMsg;
			strMsg = strTopDir + theMsg->GetMessage(wxT("MSG_IS")) + wxT(" ") + theMsg->GetMessage(wxT("MSG_DIREDTORY_READ_FAIL"));
			wxMessageBox(strMsg, PROGRAM_FULL_NAME, wxOK | wxICON_INFORMATION);
		}

		return;
	}

	LoadDirectory(strTopDir);

	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strTopDir, CHANGE_DIR_TAB);
	theSplitterManager->GetCurrentViewManager()->ChangeDirectoryPath(strTopDir, CHANGE_DIR_PATHVIEW);
}

void CLocalFileListView::SetDiskSpace(const wxString& strVolume)
{
	std::vector<CNextMDirData>::iterator it = theDriveInfo->GetDriveItem(strVolume);
	SetDiskSpace(it);
}

void CLocalFileListView::SetDiskSpace(std::vector<CNextMDirData>::iterator it)
{
	wxFSVolumeKind fsk = it->GetDriveType();

	if ( (fsk != wxFS_VOL_CDROM) &&
		 (fsk != wxFS_VOL_DVDROM))
	{
		wxString strTotalSpace(wxT(""));
		wxString strFreeSpace(wxT(""));

		it->GetDriveSpace(m_dblTotalSpace, m_dblFreeSpace);

		double dblFreePrecent = m_dblFreeSpace * 100.0 / m_dblTotalSpace;

		strTotalSpace = wxString::Format(wxT("%04.2f GB"), (m_dblTotalSpace / GIGABYTE));
		strFreeSpace = wxString::Format(wxT("%04.2f GB"), (m_dblFreeSpace / GIGABYTE));

		m_strDiskSpaceDisp = it->GetDriveName();
		m_strDiskSpaceDisp += wxT(" ");
		m_strDiskSpaceDisp += strFreeSpace;
		m_strDiskSpaceDisp += wxString::Format(wxT("(%.2f%) "), dblFreePrecent);
		m_strDiskSpaceDisp += theMsg->GetMessage(wxT("MSG_DISK_FREE"));
		m_strDiskSpaceDisp += wxT(" (");
		m_strDiskSpaceDisp += theMsg->GetMessage(wxT("MSG_DISK_TOTAL"));
		m_strDiskSpaceDisp += wxT(" ");
		m_strDiskSpaceDisp += strTotalSpace;
		m_strDiskSpaceDisp += wxT(")");
	}
	else
		m_strDiskSpaceDisp = wxT("");
}

void CLocalFileListView::OnDiskDriveInit(wxCommandEvent& event)
{
	ResetDiskDriveItems();
}

void CLocalFileListView::OnFileSystemWatcher(wxCommandEvent& event)
{
	CWatcherItem* pItem = (CWatcherItem *)event.GetClientData();
	switch(pItem->m_Action)
	{
		case FS_WATCHER_CREATE:
			DoCreate(pItem);
			break;

		case FS_WATCHER_MODIFY:
			DoModify(pItem);
			break;

		case FS_WATCHER_DELETE:
			DoDelete(pItem);
			break;

		case FS_WATCHER_RENAME:
			DoRename(pItem);
			break;

		default:
			return;
	}

	theUtility->RefreshWindow(this, m_viewRect);
}

void CLocalFileListView::DoCreate(CWatcherItem* pItem)
{
	wxString strName(pItem->m_strNew);
	wxString strFullPath = theUtility->MakeFullPathName(m_strCurrentPath, strName);

	bool isDir = false;
	unsigned long lattr = 0;
	wxLongLong llSize(0);
	wxDateTime dt(0.0);
	wxString strExt(wxT(""));
	wxString strDesc(wxT(""));

//	FILE_TYPE ftype = FTYPE_UNKNOWN;
	unsigned int ftype = FTYPE_UNKNOWN;
	ftype = CLocalFileSystem::GetFileType(strFullPath);

	CNextMDirData dirItem;
	dirItem.SetName(strName);
	dirItem.SetPath(m_strCurrentPath);
	dirItem.SetType(CNextMDirData::item_type::file);

	bool bIconSet = false;
	int iIconIndex = 0;
	int iOverlayIconIndex = 0;

	bool IsAttr = CLocalFileSystem::GetAttribute(strFullPath, &isDir, &lattr, &llSize, &dt);

	if(ftype != FTYPE_UNKNOWN && IsAttr)
	{
		if (!theConfig->IsViewAllFile())
		{
			if(theUtility->IsAllowAttr(lattr))
				return;
		}

		if (isDir)
		{
			dirItem.SetType(CNextMDirData::item_type::dir);
			m_iDirCount++;
			strDesc = theMsg->GetMessage(wxT("MSG_DIR_DESCRIPTION"));
		}
		else
		{
			dirItem.SetType(CNextMDirData::item_type::file);

			strExt = theUtility->GetExt(strName);
			m_iFileCount++;

			m_dblFileSizeInDir += llSize.ToDouble();

			if(theIconInfo->GetFileIconInfo(strExt, iIconIndex, iOverlayIconIndex, strDesc))
			{
				dirItem.SetImageIconFlag(true);
				bIconSet = true;
			}
		}

		if(!bIconSet)
		{
			strDesc = theUtility->GetFileDescription(strExt,  strFullPath);
			theImageList->GetIconIndex(strFullPath, iIconIndex, iOverlayIconIndex);
		}

		dirItem.SetIconIndex(iIconIndex, iOverlayIconIndex);

		dirItem.SetAttribute(lattr);
		dirItem.SetSize(llSize);
		dirItem.SetDateTime(dt);
		dirItem.SetPath(m_strCurrentPath);

		dirItem.SetMatch(false);
		dirItem.SetExt(strExt);
		dirItem.SetDescription(strDesc);
	}

	m_itemList.push_back(dirItem);
	m_iTotalItems = m_itemList.size();

	m_strMaxName = theUtility->GetMaxData(this, m_viewFont, strName, m_strMaxName);
	m_strMaxTypeName = theUtility->GetMaxData(this, m_viewFont, strDesc, m_strMaxTypeName);

	m_iDriveItemIndex++;

	SortStart();
	m_bSizeOrColumnChanged = true;

/*	wxString strName(pItem->m_strNew);
	wxString strFullPath = theUtility->MakeFullPathName(m_strCurrentPath, strName);

	bool isDir = false;
	unsigned long lattr = 0;
	wxLongLong llSize(0);
	wxDateTime dt(0.0);
	wxString strExt(wxT(""));
	wxString strDesc(wxT(""));

	FILE_TYPE ftype = FTYPE_UNKNOWN;
	ftype = CLocalFileSystem::GetFileType(strFullPath);

	if(ftype != FTYPE_UNKNOWN)
	{
		CNextMDirData dirItem;
		dirItem.SetName(strName);
		dirItem.SetPath(m_strCurrentPath);
		dirItem.SetType(CNextMDirData::item_type::file);
		//속성읽기
		if(!CLocalFileSystem::GetAttribute(strFullPath, &isDir, &lattr, &llSize, &dt))
			return;

		if (!theConfig->IsViewAllFile())
		{
			if(theUtility->IsAllowAttr(lattr))
				return;
		}

		bool bIconSet = false;

		int iIconIndex = 0;
		int iOverlayIconIndex = 0;

		if (isDir)
		{
			dirItem.SetType(CNextMDirData::item_type::dir);
			m_iDirCount++;
			strDesc = theMsg->GetMessage(wxT("MSG_DIR_DESCRIPTION"));
		}
		else
		{
			dirItem.SetType(CNextMDirData::item_type::file);

			strExt = theUtility->GetExt(strName);
			m_iFileCount++;

			m_dblFileSizeInDir += llSize.ToDouble();

			if(theIconInfo->GetFileIconInfo(strExt, iIconIndex, iOverlayIconIndex, strDesc))
			{
				dirItem.SetImageIconFlag(true);
				bIconSet = true;
			}
		}

		if(!bIconSet)
		{
			strDesc = theUtility->GetFileDescription(strExt,  strFullPath);
			theUtility->GetIconIndex(strFullPath, iIconIndex, iOverlayIconIndex);
		}

		dirItem.SetIconIndex(iIconIndex, iOverlayIconIndex);


		dirItem.SetAttribute(lattr);
		dirItem.SetSize(llSize);
		dirItem.SetDateTime(dt);
		dirItem.SetPath(m_strCurrentPath);

		dirItem.SetMatch(false);
		dirItem.SetExt(strExt);
		dirItem.SetDescription(strDesc);

		m_itemList.push_back(dirItem);
		m_iTotalItems = m_itemList.size();

		m_strMaxName = theUtility->GetMaxData(this, m_viewFont, strName, m_strMaxName);
		m_strMaxTypeName = theUtility->GetMaxData(this, m_viewFont, strDesc, m_strMaxTypeName);

		m_iDriveItemIndex++;

		SortStart();
		m_bSizeOrColumnChanged = true;
	}
*/
}

void CLocalFileListView::DoModify(CWatcherItem* pItem)
{
	m_bWatcherModify = false;
	wxString strName(pItem->m_strNew);
	wxString strFullPath = theUtility->MakeFullPathName(m_strCurrentPath, strName);

//	FILE_TYPE ftype = FTYPE_UNKNOWN;
	unsigned int ftype = FTYPE_UNKNOWN;
	ftype = CLocalFileSystem::GetFileType(strFullPath);

	bool isDir = false;
	unsigned long lattr = 0;
	wxLongLong llSize(0);
	wxDateTime dt(0.0);

	bool IsAttr = CLocalFileSystem::GetAttribute(strFullPath, &isDir, &lattr, &llSize, &dt);
	if(ftype != FTYPE_UNKNOWN && IsAttr)
	{
		std::vector<CNextMDirData>::iterator it = FindItem(strName);
		if(it == m_itemList.end())
			return;


		if (!theConfig->IsViewAllFile())
		{
			if(theUtility->IsAllowAttr(lattr))
				return;
		}

		if(it->IsFile())
		{
			if(it->GetSize().ToDouble() != 0)
				m_dblFileSizeInDir -= it->GetSize().ToDouble();

			m_dblFileSizeInDir += llSize.ToDouble();
		}

		it->SetAttribute(lattr);
		it->SetSize(llSize);
		it->SetDateTime(dt);

		if(it->IsFile())
		{
			m_bWatcherModify = true;
			int iSortType = theConfig->GetSortType();

			if(iSortType == CFileListView::VIEW_SORT::VIEW_SORT_SIZE)
				SortStart();
		}

		theSplitterManager->MSWUpdateDriveSize(m_strVolume);
	}
}

void CLocalFileListView::DoDelete(CWatcherItem* pItem)
{
	wxString strName(pItem->m_strNew);
	std::vector<CNextMDirData>::iterator it = FindItem(strName);

	if(it == m_itemList.end())
		return;

	wxString strDesc = it->GetDescription();

	m_strMaxName = theUtility->GetMaxData(this, m_viewFont, strName, m_strMaxName);
	m_strMaxTypeName = theUtility->GetMaxData(this, m_viewFont, strDesc, m_strMaxTypeName);

	if (it->IsDir())
		m_iDirCount--;
	else
	{
		m_dblFileSizeInDir -= it->GetSize().ToDouble();
		m_iFileCount--;
	}

	m_itemList.erase(it);
	m_iTotalItems = m_itemList.size();

	if(m_iCurrentItemIndex > m_iTotalItems)
	{
		m_iStartIndex = 0;
		m_iCurrentItemIndex = 0;
	}

	std::unordered_map<wxString, wxString>::iterator itDel = m_dispNameInfoMap.find(strName.MakeLower());
	if(itDel != m_dispNameInfoMap.end())
		m_dispNameInfoMap.erase(itDel);

	m_iDriveItemIndex--;

	m_bSizeOrColumnChanged = true;
	theSplitterManager->MSWUpdateDriveSize(m_strVolume);
}

void CLocalFileListView::DoRename(CWatcherItem* pItem)
{
	m_bRename = false;
	wxString strNewName(pItem->m_strNew);
	wxString strOldName(pItem->m_strOld);

	wxString strFullPath = theUtility->MakeFullPathName(m_strCurrentPath, strNewName);

//	if(!CLocalFileSystem::IsFileWritePermission(strFullPath))
//		return;

	std::vector<CNextMDirData>::iterator it = FindItem(strOldName);
	if(it == m_itemList.end())
		return;

	it->SetName(strNewName);

	wxString strExt = theUtility->GetExt(strNewName);
	wxString strDesc(wxT(""));

	int iIconIndex = 0;
	int iOverlayIconIndex = 0;

	if(!theIconInfo->GetFileIconInfo(strExt, iIconIndex, iOverlayIconIndex, strDesc))
	{
		strDesc = theUtility->GetFileDescription(strExt, strFullPath);
		theImageList->GetIconIndex(it->GetFullPath(), iIconIndex, iOverlayIconIndex);
	}

	it->SetExt(strExt);
	it->SetIconIndex(iIconIndex, iOverlayIconIndex);
	it->SetDescription(strDesc);

	m_strMaxName = theUtility->GetMaxData(this, m_viewFont, strNewName, m_strMaxName);
	m_strMaxTypeName = theUtility->GetMaxData(this, m_viewFont, strDesc, m_strMaxTypeName);

	SortStart();
	m_bSizeOrColumnChanged = true;
/*
	wxString strNewName(pItem->m_strNew);
	wxString strOldName(pItem->m_strOld);

	wxString strFullPath = theUtility->MakeFullPathName(m_strCurrentPath, strNewName);

	bool bExist = true;
	std::vector<CNextMDirData>::iterator it = FindItem(strOldName);
	if(it == m_itemList.end())
		return;

	FILE_TYPE ftype = FTYPE_UNKNOWN;
	ftype = CLocalFileSystem::GetFileType(strFullPath);
	if(ftype == FTYPE_UNKNOWN)
		return;

	CWatcherItem* pWatcherItem = new CWatcherItem();
	pWatcherItem->m_strNew = pItem->m_strOld;

	DoDelete(pWatcherItem);
	wxDELETE(pWatcherItem);

	if(it->IsFile())
	{
		bool IsExist = wxFileExists(strFullPath);
		if(IsExist)
			return;
	}

	DoCreate(pItem);
	m_bSizeOrColumnChanged = true;
*/
}

void CLocalFileListView::OnUpdateDriveSizeInfo(wxCommandEvent& event)
{
	wxString strCurrentVol = m_strCurrentPath.Left(1);

	wxString strVolume = event.GetString();
	wxString strDriveName = strVolume + wxT(":") + SLASH;

	std::vector<CNextMDirData>::reverse_iterator rit;

	for(rit = m_itemList.rbegin(); rit != m_itemList.rend(); ++rit)
	{
		if(!rit->IsDrive())
			break;

		//사이즈가 변경된 드라이브에 대해서 용량변경 표시 적
		wxString strVol = rit->GetVolume();
		if(strVolume.CmpNoCase(strVol) == 0)
		{
			//용량이 변경된 디스크는 용량을 갱신한다.
			theDriveInfo->UpdateDriveSpace(strVolume);

			double dblTotalSpace = 0.0f;
			double dblFreeSpace  = 0.0f;

			//디스크용량이 변경된 드라이브에 대해서 변경용량을 다시 읽어와 해당 드라이브정보를 갱신한다.
			theDriveInfo->GetDiskSpace(strVolume, dblTotalSpace, dblFreeSpace, false);
			rit->SetDriveSpace(dblTotalSpace, dblFreeSpace);

			//현재 Volume과 변경된 Volume 이 같으면 화면에 변경된 용량을 표시한다.
			if(strCurrentVol.CmpNoCase(strVolume) == 0)
			{
				std::vector<CNextMDirData>::iterator it = theDriveInfo->GetDriveItem(strVolume);
				SetDiskSpace(it);
			}

			break;
		}
	}

	theUtility->RefreshWindow(this, m_viewRect);
}
