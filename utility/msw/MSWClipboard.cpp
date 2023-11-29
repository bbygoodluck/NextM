#include "../../ginc.h"
#include "MSWClipboard.h"

std::unique_ptr<CMSWClipboard> CMSWClipboard::m_pInstance(nullptr);

CMSWClipboard* CMSWClipboard::Get()
{
	if (m_pInstance.get() == NULL)
		m_pInstance.reset(new CMSWClipboard());

	return m_pInstance.get();
}

void CMSWClipboard::Clear()
{
	m_aDatas.clear();
	m_strSrcPath = wxT("");
}

void CMSWClipboard::AddData(const wxString& strData)
{
	m_aDatas.push_back(strData);
}

void CMSWClipboard::AddData(const std::list<wxString>& lstDatas)
{
	m_aDatas = lstDatas;
}

void CMSWClipboard::CopyToClipboard()
{
	wxTheClipboard->UsePrimarySelection();
	if (wxTheClipboard->Open())
	{
		wxFileDataObject* pFileDataObject = new wxFileDataObject();
		for (auto item : m_aDatas)
			pFileDataObject->AddFile(item);

		wxTheClipboard->AddData(pFileDataObject);
		wxTheClipboard->Close();
	}
}

void CMSWClipboard::GetData(std::list<wxString>& lstDatas)
{
	wxTheClipboard->UsePrimarySelection();
	wxDataFormat fmt(wxDF_FILENAME);

	if (wxTheClipboard->IsSupported(fmt))
	{
		if (wxTheClipboard->Open())
		{
			wxArrayString arrDatas;
			wxFileDataObject fileDataObject;
			if (wxTheClipboard->GetData(fileDataObject))
				arrDatas = fileDataObject.GetFilenames();

			wxTheClipboard->Close();

			for (auto item : arrDatas)
				lstDatas.push_back(item);
		}
	}
}
