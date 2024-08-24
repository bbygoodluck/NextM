#include "../common/common.h"
#include "NextMExternalProgInfo.h"

#include <iostream>
#include <fstream>
#include <streambuf>

std::unique_ptr<CExternalProgInfo> CExternalProgInfo::m_pInstance(nullptr);

CExternalProgInfo::CExternalProgInfo()
{
	m_external.reserve(100);
}

CExternalProgInfo::~CExternalProgInfo()
{
	_jsonDoc.RemoveAllMembers();
}

CExternalProgInfo* CExternalProgInfo::Get()
{
	if (m_pInstance.get() == nullptr)
		m_pInstance.reset(new CExternalProgInfo());

	return m_pInstance.get();
}

bool CExternalProgInfo::Load()
{
	_strJsonPath = theUtility->GetWorkDir() + SLASH + wxT("default") + SLASH + wxT("externaledit.cfg");
	if (!wxFileName::FileExists(_strJsonPath))
	{
		theUtility->ShowMessageBox(_strJsonPath + wxT(" file is not exist"), wxICON_ERROR | wxOK);
		return false;
	}

	if(!DoLoad(_strJsonPath))
		return false;

	SetExternalProgramItems();
	CreateExternalMenuList();
	return true;
}

void CExternalProgInfo::SetExternalProgramItems()
{
	assert(_jsonDoc.IsObject());
	assert(HasMember(wxT("externalEdit")));

	const Value& FileEditPGList = _jsonDoc["externalEdit"];
	assert(FileEditPGList.IsArray());

	for (SizeType i = 0; i < FileEditPGList.Size(); i++)
	{
		CExternalProgItem& external = m_external.emplace_back();
		external._strPGName = FileEditPGList[i]["name"].GetString();
		external._strPGPath = FileEditPGList[i]["path"].GetString();
		external._strPGArgs = FileEditPGList[i]["args"].GetString();
	}
}

//복사
void CExternalProgInfo::ResetExternalProgram(const std::vector<CExternalProgItem>& exItems)
{
	ClearExternalItems();
	std::copy(exItems.begin(), exItems.end(), std::back_inserter(m_external));

	CreateExternalMenuList();
	UpdateDocument();
}

void CExternalProgInfo::AddExternalProgram(const CExternalProgItem& exItem)
{
	m_external.emplace_back(exItem);
	CreateExternalMenuList();

	UpdateDocument();
}

void CExternalProgInfo::UpdateDocument()
{
	Document::AllocatorType& allocator = _jsonDoc.GetAllocator();
	Value valExtPGs(kArrayType);

	std::vector<CExternalProgItem>::iterator iterItemStart = BeginIter();
	std::vector<CExternalProgItem>::iterator iterItemEnd = EndIter();

	while(iterItemStart != iterItemEnd)
	{
		CExternalProgItem exPg = *iterItemStart;

		Value item;
		item.SetObject();

		Value _name(exPg._strPGName.c_str(), allocator);
		Value _path(exPg._strPGPath.c_str(), allocator);
		Value _args(exPg._strPGArgs.c_str(), allocator);

		item.AddMember("name", _name, allocator);
		item.AddMember("path", _path, allocator);
		item.AddMember("args", _args, allocator);

		valExtPGs.PushBack(item, allocator);

		iterItemStart++;
	}

	if(GetExternalProgCount() > 0)
		_jsonDoc["externalEdit"] = valExtPGs;
}

void CExternalProgInfo::CreateExternalMenuList()
{
	wxBitmap bmpExternalPG = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER, wxSize(16, 16));

	std::vector<CExternalProgItem>::iterator cItBegin = BeginIter();
	std::vector<CExternalProgItem>::iterator cItEnd   = EndIter();

	int iIndex = 0;
	while(cItBegin != cItEnd)
	{
		wxMenuItem* pFindItem = m_menu.FindItem(EXTERNAL_PROGRAM_START_ID + iIndex);
		if(pFindItem != nullptr)
			m_menu.Remove(EXTERNAL_PROGRAM_START_ID + iIndex);

		wxMenuItem* pMenuItem = m_menu.Append(EXTERNAL_PROGRAM_START_ID + iIndex, cItBegin->_strPGName);

		int iIconIndex;
		int iOverlayIndex;

		theUtility->GetIconIndex(cItBegin->_strPGPath, iIconIndex, iOverlayIndex);
		wxIcon ico;

#ifdef __WXMSW__
		HICON hIcon = ImageList_GetIcon(_gImageList, iIconIndex, ILD_NORMAL);
		ico.CreateFromHICON(hIcon);
#else

#endif
		if (ico.IsOk())
		{
			wxBitmap bmp;
			bmp.CopyFromIcon(ico);

			if(bmp.IsOk())
				pMenuItem->SetBitmap(bmp);
			else
				pMenuItem->SetBitmap(bmpExternalPG);
		}
		else
			pMenuItem->SetBitmap(bmpExternalPG);

		DestroyIcon(hIcon);

		cItBegin++;
		iIndex++;
	}
}
