#include "../../ginc.h"
#include "../../dialog/DlgMakeNCDInfo.h"
#include "NCDInfoThread.h"
#include "../../engine/LocalFileSystem.h"
CNCDInfoThread::CNCDInfoThread(DlgMakeNCDInfo* pDlg)
	: wxThread(wxTHREAD_JOINABLE)
	, m_pDlg(pDlg)
{
}

CNCDInfoThread::~CNCDInfoThread()
{

}

void CNCDInfoThread::SetNCDFile(const wxString& strFile, const wxString& strPath, size_t lineCount)
{
	m_strNCDFile = strFile;
	m_lineCnt = lineCount;
	m_strCurrentPath = strPath;

	int iFind = m_strCurrentPath.find(SLASH);
	if (iFind == -1)
		m_strCurrentPath += SLASH;
}

int CNCDInfoThread::GetNumberofDirectory(path p)
{
	int n = 0;
	for (recursive_directory_iterator it(p); it != recursive_directory_iterator{}; ++it)
	{
		if (is_directory(*it))
			n++;
	}

	return n;
}

#include <wx/encconv.h>
wxThread::ExitCode CNCDInfoThread::Entry()
{
	if (m_bLoadFromFile)
	{
		if (m_strNCDFile.IsEmpty())
		{
			wxString strMsg(m_strNCDFile);
			strMsg += wxT(" is not file exist");

			wxMessageBox(strMsg, PROGRAM_FULL_NAME, wxICON_ERROR | wxOK);

			SendThreadTerminateMessage();
			return (wxThread::ExitCode)0;
		}

		FILE* fp = fopen(m_strNCDFile.char_str(), "rb");
		if(fp != nullptr)
		{
			fseek(fp, 0, SEEK_END);
			unsigned long ulSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char *buff = (char *)malloc(sizeof(char) * ulSize);

			unsigned int totnum = 0;
			unsigned int curnum = 0;

			while( (curnum = fread(&buff[totnum], sizeof(char), ulSize - totnum, fp)) > 0)
				totnum += curnum;

			unsigned char *buff2 = nullptr;
			buff2 = (unsigned char *)strtok(buff, "\n");

			unsigned int linenum = 0;
			int iDepth = 0;

			wxString strDepth(wxT(""));
			wxString strName(wxT(""));

			wxString strPath(wxT(""));
			std::vector<wxString> vecSplit;
			DirNode* pNode = nullptr;

			do
			{
				vecSplit.clear();
				strPath = buff2;

				if(strPath.IsEmpty())
					break;

				std::vector<wxString> vecSplit = theUtility->Split(strPath, wxT("^"));
				if (vecSplit.size() < 2)
				{
//					wxString strMsg(strPath);
//					strMsg += wxT("The file is in the wrong format.\n Delete that file and create directory information again!");

//					theUtility->ShowMessageBox(strMsg, wxICON_ERROR | wxOK);
					break;
				}

				strDepth = vecSplit.at(0);
				strName = vecSplit.at(1);

				strDepth.ToInt(&iDepth);

				//root node Depth = 0
				if(linenum == 0)
					pNode = theNCD->AddNode(strName, nullptr, iDepth);
				else
					pNode = theNCD->AddNode(strName, pNode, iDepth);

				buff2 = (unsigned char *)strtok(nullptr, "\n");
				linenum++;

			} while(buff2);

			delete[] buff;
			fclose(fp);

			theNCD->SetOrder();
			theNCD->SetCur(m_strCurrentPath);
		}
	}
	else
	{
		wxString strPath = m_pCurrNode->_fullName;
		LoadSubDir(strPath, m_pCurrNode);

		theNCD->SetReOrder(wxT(""));
	}

	SendThreadTerminateMessage();
	return (wxThread::ExitCode)0;
}

void CNCDInfoThread::LoadSubDir(const wxString& strPath, DirNode* pNode)
{
#ifdef __WXMSW__
	if(pNode == nullptr)
		return;

	wxString sPath(strPath);
	wxString strFullPath(strPath);

	CLocalFileSystem localSys;
	bool isDir = false;
	unsigned long lattr = 0;

	if (!localSys.BeginFindFiles(sPath, true))
		return;

	int iDepth = pNode->_depth + 1;
	DirNode* _pAddNode = nullptr;

	wxString strName(wxT(""));
	while (localSys.GetNextFile(strName, nullptr, nullptr, nullptr, nullptr))
	{
		if (IsCancelOrClose())
			break;

		if (std::find(_gVecIgnore.begin(), _gVecIgnore.end(), CONVSTR_TO_STD(strName)) != _gVecIgnore.end())
			continue;

		if (!theNCD->IsExistChildNode(strName, pNode))
			_pAddNode = theNCD->AddNode(strName, pNode, iDepth);

		wxString strNewFullPath(strFullPath);
		strNewFullPath += SLASH;
		strNewFullPath += strName;

		LoadSubDir(strNewFullPath, _pAddNode);
	}

	if (GetLastError() != ERROR_NO_MORE_FILES)
		wxMessageBox(wxT("Something went wrong during searcing..."), PROGRAM_FULL_NAME, wxOK | wxICON_ERROR);

#else
#endif
}

bool CNCDInfoThread::IsCancelOrClose()
{
	return (m_pDlg->IsCancel() || m_pDlg->IsClose());
}

void CNCDInfoThread::SendThreadTerminateMessage()
{
	wxThreadEvent event(wxEVT_THREAD, SCD_MAKE_TREE_INFO_THREAD_EVENT);
	event.SetInt(-1);
	wxQueueEvent(m_pDlg, event.Clone());
}
