#include "../../ginc.h"
#include "MSWFileOperation.h"
#include "../../engine/LocalFileSystem.h"

std::unique_ptr<CMSWNextMFileOperation> CMSWNextMFileOperation::m_pInstance = nullptr;

CMSWNextMFileOperation* CMSWNextMFileOperation::Get()
{
	if (m_pInstance == nullptr)
		m_pInstance.reset(new CMSWNextMFileOperation());

	return m_pInstance.get();
}

bool CMSWNextMFileOperation::Init()
{
	//COM 초기화
	m_hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	SetErrorCode();
	if (FAILED(m_hr))
		return false;

	//IFileOperation 생성
	m_hr = CreateAndInitFileOperation(IID_PPV_ARGS(&m_pfo));
	SetErrorCode();
	if (FAILED(m_hr))
		return false;

	return true;
}

HRESULT CMSWNextMFileOperation::CreateAndInitFileOperation(REFIID riid, void **ppv)
{
	*ppv = NULL;
	// Create the IFileOperation object
	IFileOperation *pfo;

	m_hr = CoCreateInstance(__uuidof(FileOperation), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pfo));
	SetErrorCode();
	if (SUCCEEDED(m_hr))
	{
		// Set the operation flags.  Turn off  all UI
		// from being shown to the user during the
		// operation.  This includes error, confirmation
		// and progress dialogs.
		m_hr = pfo->SetOperationFlags(FOF_NOCONFIRMMKDIR);
		SetErrorCode();
		if (SUCCEEDED(m_hr))
			m_hr = pfo->QueryInterface(riid, ppv);

		pfo->Release();
	}

	return m_hr;
}

void CMSWNextMFileOperation::Release()
{
//	m_pfo->Release();
	CoUninitialize();
}

bool CMSWNextMFileOperation::CopyOrMoveOperations(const wxString& strItem, const wxString& strDest, wxWindow* parent, bool IsCopy, bool bSameDir)
{
	std::list<wxString> items;
	items.push_back(strItem);
	return CopyOrMoveOperations(items, strDest, parent, IsCopy, bSameDir);
}

bool CMSWNextMFileOperation::CopyOrMoveOperationsToSameDir(std::list<wxString>& dirsToVisit, const wxString& strDest, bool IsCopy)
{
	wxString strName(wxT(""));
	wxString strFullPath(wxT(""));
	bool bReturn = true;

	IShellItem* psiTo = nullptr;
	m_hr = SHCreateItemFromParsingName(CONVSTR(strDest), NULL, IID_PPV_ARGS(&psiTo));

	if(FAILED(m_hr))
		return false;

	for (auto strItem : dirsToVisit)
	{
		IShellItem* psiFrom = nullptr;
		m_hr = SHCreateItemFromParsingName(CONVSTR(strItem), NULL, IID_PPV_ARGS(&psiFrom));

		if(SUCCEEDED(m_hr))
		{
			strName = theUtility->GetPathName(strItem);
			if(theUtility->Compare(strDest.Right(1), SLASH) == 0)
				strFullPath = strDest + strName;
			else
				strFullPath = strDest + SLASH + strName;

			if (strFullPath.CmpNoCase(strItem) == 0)
				strName  = theUtility->ChangeName(strFullPath);

			m_hr = IsCopy ? m_pfo->CopyItem(psiFrom, psiTo, strName, nullptr) : m_pfo->MoveItem(psiFrom, psiTo, strName, nullptr);
			if(SUCCEEDED(m_hr))
				m_hr = m_pfo->PerformOperations();

			psiFrom->Release();
		}
		else
		{
			bReturn = false;
			break;
		}
	}

	psiTo->Release();

	Release();
	return bReturn;
}

bool CMSWNextMFileOperation::CopyOrMoveOperations(std::list<wxString>& dirsToVisit, const wxString& strDest, wxWindow* parent, bool IsCopy, bool bSameDir)
{
	CLocalFileSystem fs;

	std::list<wxString> lstOPTgt;
	if(!IsCopy)
	{
		if(!fs.CheckFileUseAnotherProcess(dirsToVisit, lstOPTgt))
			return false;
	}
	else
		lstOPTgt.assign(dirsToVisit.begin(), dirsToVisit.end());

	if (!Init())
		return false;

	if(bSameDir)
		return CopyOrMoveOperationsToSameDir(lstOPTgt, strDest, IsCopy);

	//보안프로그램으로 인해 IFileOperation 오류 발생으로
	//SHFILEOPSTRUCT를 사용하여 처리
	if(theConfig->GetUseWindowShellFileOperation())
	{
		//복사(잘라내기) Target IShellItem 설정
		IShellItem* psiDest = NULL;
		m_hr = SHCreateItemFromParsingName(CONVSTR(strDest), NULL, IID_PPV_ARGS(&psiDest));
		SetErrorCode();
		if (FAILED(m_hr))
			return false;

		std::vector<PIDLIST_ABSOLUTE> vctIDLists;
		vctIDLists.reserve(100);

		//복사(잘라내기)대상자의 ShellItem를 vector에 저정
		for (auto const& item : lstOPTgt)
		{
			IShellItem* shlItem = NULL;
			//ShellItem 생성
			m_hr = SHCreateItemFromParsingName(CONVSTR(item), NULL, IID_PPV_ARGS(&shlItem));
			SetErrorCode();
			if (FAILED(m_hr))
				return false;

			PIDLIST_ABSOLUTE _pidlistAbsolute = NULL;
			m_hr = SHGetIDListFromObject(shlItem, &_pidlistAbsolute);
			if (SUCCEEDED(m_hr))
				vctIDLists.emplace_back(_pidlistAbsolute);

			shlItem->Release();
		}

		int iItemCounts = vctIDLists.size();
		if (iItemCounts > 0)
		{
			PIDLIST_ABSOLUTE* paIDs = new PIDLIST_ABSOLUTE[iItemCounts];
			for (int idx = 0; idx < iItemCounts; idx++)
				paIDs[idx] = vctIDLists[idx];

			IShellItemArray* psia = NULL;
			m_hr = SHCreateShellItemArrayFromIDLists(iItemCounts, (LPCITEMIDLIST *)paIDs, &psia);
			SetErrorCode();
			if (SUCCEEDED(m_hr))
			{
				m_hr = IsCopy ? m_pfo->CopyItems(psia, psiDest) : m_pfo->MoveItems(psia, psiDest);
				if (SUCCEEDED(m_hr))
				{
					m_hr = m_pfo->PerformOperations();
					psiDest->Release();
				}
			}

			for (int idx = 0; idx < iItemCounts; idx++)
				CoTaskMemFree(paIDs[idx]);

			delete[] paIDs;
		}

		Release();
		return true;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	int len = 1;

	for (auto const& dirItem : lstOPTgt)
		len += dirItem.size() + 1;

	wxChar* from = new wxChar[len];
	wxChar* p = from;

	for (auto dir : lstOPTgt)
	{
		_tcscpy(p, dir);
		p += dir.size() + 1;
	}

	*p = 0; // End of list

	wxChar* to = new wxChar[strDest.Len() + 2];
	wxStrcpy(to, strDest);
	to[strDest.Len() + 1] = 0; // End of list

	SHFILEOPSTRUCT op;
	wxZeroMemory(op);

	op.hwnd = parent ? (HWND)parent->GetHandle() : 0;
	op.pFrom = from;
	op.pTo = to;
	op.wFunc = IsCopy ? FO_COPY : FO_MOVE;

	int iRet = SHFileOperation(&op);

	delete [] to;
	delete [] from;

	if(iRet != 0)
		return false;

	return true;
}

bool CMSWNextMFileOperation::RecursiveDelete(const wxString& path, wxWindow* parent, bool bGoTrash, int& iRet)
{
	std::list<wxString> paths;
	paths.push_back(path);
	return RecursiveDelete(paths, parent, bGoTrash, iRet);
}

bool CMSWNextMFileOperation::RecursiveDelete(const std::list<wxString>& dirsToVisit, wxWindow* parent, bool bGoTrash, int& iRet)
{
	CLocalFileSystem fs;

	std::list<wxString> lstOPTgt;
	if(!fs.CheckFileUseAnotherProcess(dirsToVisit, lstOPTgt))
		return false;

	if(bGoTrash && lstOPTgt.size() > 0)
	{
		wxString strMsg(wxT(""));
		int iOperationItemCounts = lstOPTgt.size();
		for(auto const& tgtItem: lstOPTgt)
		{
			strMsg += tgtItem;
			strMsg += wxT("\n");
		}

		strMsg += wxString::Format(wxT("%d "), iOperationItemCounts);
		strMsg += theMsg->GetMessage(wxT("MSG_DLG_DELETE_SELITEM"));
		strMsg += wxT("\n");

		int nRet = wxMessageBox(strMsg, theMsg->GetMessage(wxT("MSG_DLG_DELETE_GO_DELETE")), wxYES_NO | wxICON_EXCLAMATION, _gMainFrame);
		if(nRet != wxYES)
		{
			lstOPTgt.clear();
			return false;
		}
	}

	return ExecuteDeleteProcess(lstOPTgt, parent, bGoTrash, iRet);
}

bool CMSWNextMFileOperation::ExecuteDeleteProcess(const std::list<wxString>& dirsToVisit, wxWindow* parent, bool bGoTrash, int& iRet)
{
	bool bRet = true;
	//파일 삭제시 IFileOperation을 사용할 경우
	//회사 보안프로그램으로 인해 정상작동이 하지 않음
	if(theConfig->GetUseWindowShellFileOperation())
	{
		std::vector<ITEMIDLIST *> idLists;
		for (auto& path : dirsToVisit)
		{
			__unaligned ITEMIDLIST* idl = ILCreateFromPath(path);
			if(!idl)
			{
				for(auto& pid: idLists)
					ILFree(pid);

				return false;
			}

			idLists.push_back(idl);
		}

		IShellItemArray* iArray = 0;
		HRESULT hr = SHCreateShellItemArrayFromIDLists((UINT)idLists.size(), (LPCITEMIDLIST *)idLists.data(), &iArray);

		for(auto& pid: idLists)
			ILFree(pid);

		idLists.clear();

		if(!SUCCEEDED(hr) || !iArray)
		{
			Release();
			iArray->Release();
			return false;
		}

		if (!Init())
			return false;

		hr = m_pfo->SetOperationFlags(bGoTrash ? FOF_ALLOWUNDO : FOF_WANTNUKEWARNING);
		hr = m_pfo->SetOwnerWindow(parent->GetHWND());

		hr = m_pfo->DeleteItems(iArray);
		if(FAILED(hr))
		{
			Release();
			iArray->Release();
			return false;
		}

		hr = m_pfo->PerformOperations();
		if(FAILED(hr))
			bRet = false;

		iArray->Release();

		Release();
		return bRet;
	}

	size_t len = 1;

	for (auto const& dirItem : dirsToVisit)
		len += dirItem.size() + 1;

	// Allocate memory
	wxChar* pBuffer = new wxChar[len];
	wxChar* p = pBuffer;

	for (auto dir : dirsToVisit)
	{
		if (dir.Last() == wxFileName::GetPathSeparator())
			dir.RemoveLast();

		_tcscpy(p, dir);
		p += dir.size() + 1;
	}

	if (p != pBuffer)
	{
		*p = 0;

		SHFILEOPSTRUCT fileop;
		wxZeroMemory(fileop);

		fileop.hwnd = parent ? (HWND)parent->GetHandle() : 0;
		fileop.wFunc = FO_DELETE;
		fileop.pFrom = pBuffer;

		if (parent)
			fileop.fFlags = bGoTrash ? FOF_ALLOWUNDO : 0;
		else
			fileop.fFlags = FOF_NOCONFIRMATION;

		int ret = SHFileOperation(&fileop);
		if (ret != 0)
			bRet = false;
	}

	delete[] pBuffer;
	return bRet;
}
