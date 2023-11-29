#include "../ginc.h"
#include "NextMNCD.h"

std::unique_ptr<CNextMNCD> CNextMNCD::m_pInstance(nullptr);

CNextMNCD::CNextMNCD()
	: m_pRoot(nullptr)

{

}

CNextMNCD::~CNextMNCD()
{

}

CNextMNCD* CNextMNCD::Get()
{
	if (m_pInstance.get() == NULL)
		m_pInstance.reset(new CNextMNCD());

	return m_pInstance.get();
}

void CNextMNCD::Initialize(size_t iInitializeSize)
{
	m_bShouldbeSaved = false;

	UnInitialize();
	m_nodeDatas.reserve(iInitializeSize + 100);
}

void CNextMNCD::UnInitialize()
{
	if(m_bShouldbeSaved)
		SaveNCD();

	Clear(m_pRoot);

	delete m_pRoot;
	m_pRoot = nullptr;
	m_nodeDatas.clear();

	//메모리 해제..
	std::vector<DirNode *>().swap(m_nodeDatas);
}

/*
 * 노드 삭제
 *
 */
void CNextMNCD::Clear(DirNode* pNode)
{
	if (pNode == nullptr)
		return;

	std::vector<DirNode *>::iterator iter;
	for (iter = pNode->_childNode.begin(); iter != pNode->_childNode.end(); ++iter)
	{
		if (*iter)
		{
			DirNode* _node = *iter;
			Clear(_node);

			delete _node;
		}
	}

	pNode->_childNode.clear();
	std::vector<DirNode *>().swap(pNode->_childNode);

}

void CNextMNCD::SetNCDFile(const wxString& strNCDFile)
{
	m_strNCDFile = strNCDFile;
}

/*
 * 디렉토리 노드 생성
 *
 */
DirNode* CNextMNCD::AddNode(const wxString& strName, DirNode* parent, int iDepth)
{
	DirNode* pNode = new DirNode();
	pNode->_name = strName;
	pNode->_depth = iDepth;

	if(_maxDepth < iDepth)
		_maxDepth = iDepth;

	if(m_pRoot == nullptr || parent == nullptr)
	{
		m_pRoot = pNode;
		m_pRoot->_parent = nullptr;
		m_pRoot->_fullName = strName + ( (strName.Right(1).CmpNoCase(SLASH) == 0) ? wxT("") : SLASH);
		m_pRoot->_row = 0;
		_curRow = 0;
		_curDepth = iDepth;
	}
	else
	{
		wxString strParentPath = parent->_fullName;
		if(_curDepth < iDepth)
		{
			pNode->_parent = parent;
			pNode->_fullName = strParentPath + strName + SLASH;
			pNode->_row = _curRow;

			parent->_childNode.emplace_back(pNode);
		}
		else
		{
			DirNode* _parentNode = FindParentNode(iDepth, parent);

			strParentPath = _parentNode->_fullName;

			pNode->_fullName = strParentPath + strName + SLASH;
			pNode->_row = ++_curRow;
			pNode->_parent = _parentNode;
			_parentNode->_childNode.emplace_back(pNode);
		}

		_curDepth = iDepth;
	}

	return pNode;
}

void CNextMNCD::DeleteNode(DirNode* pNode)
{
	DirNode* parentNode = pNode->_parent;

	Clear(pNode);
	nodeIterator iter = parentNode->_childNode.begin();
	while(iter != parentNode->_childNode.end())
	{
		if ((*iter) == pNode)
			break;

		++iter;
	}

	parentNode->_childNode.erase(iter);
	delete pNode;

	SetReOrder(parentNode->_fullName);
}

/*
 * 상위 노드 찾기
 *
 */
DirNode* CNextMNCD::FindParentNode(int iDepth, DirNode* pCurrDirNode)
{
	DirNode* pNode = pCurrDirNode;
	while (iDepth <= pNode->_depth)
		pNode = pNode->_parent;

	return pNode;
}

/*
 * N-Ary Tree를 std::vector로 옮긴다.
 * 실제 파일을 읽은 순으로 적재함.
 *
 */
void CNextMNCD::SetOrder()
{
	std::vector<DirNode *> vNode;

	DirNode* pNode;
	m_nodeDatas.clear();

	int index = 0;
	int iRow = 0;
	int iDepth = -1;

	vNode.emplace_back(GetRoot());
	while (!vNode.empty())
	{
		pNode = vNode.back();
		vNode.pop_back();

		//표시 Row값을 다시 설정한다.
		if (pNode->_depth <= iDepth) iRow++;
		iDepth = pNode->_depth;

		pNode->_row = iRow;
		pNode->_index = index;

		m_nodeDatas.emplace_back(pNode);

		std::reverse_copy(std::begin(pNode->_childNode), std::end(pNode->_childNode), std::back_inserter(vNode));
		index++;
	}
}

void CNextMNCD::SetReOrder(const wxString& strPath)
{
	SetOrder();
//	SaveNCD();
	m_bShouldbeSaved = true;

	//패스변화가 없으면 ...
	if(strPath.IsEmpty())
		return;

	SetCur(strPath);
}

/*
 * 현재노드 가져오기
 * @Parameter
 *   strPath : 경로(FullPath)
 */
void CNextMNCD::SetCur(const wxString& strPath)
{
	wxString strPathName(strPath);
	if(strPathName.Right(1).CmpNoCase(SLASH) != 0)
		strPathName += SLASH;

	if(strPathName.IsEmpty())
	{
		m_pCurNode = m_nodeDatas.begin();
		return;
	}

	//루트노드이면..
	if(m_pRoot->_fullName.CmpNoCase(strPathName) == 0)
	{
		m_pCurNode = m_nodeDatas.begin();
		return;
	}

	std::vector<DirNode *>::iterator it = m_nodeDatas.begin();
	std::vector<DirNode *>::iterator itEnd = m_nodeDatas.end();

	std::vector<wxString> vItems = theUtility->Split(strPathName, SLASH);
	// root의 depth가 0 이므로 현재 패스는 size - 1
	size_t iDepth = vItems.size() - 1;

	do
	{
		DirNode* pNode = *it;
		if(pNode->_depth != iDepth)
			continue;

		if(IsEqual(pNode, strPathName))
		{
			m_pCurNode = it;
			break;
		}
	}while(++it != itEnd);
}

bool CNextMNCD::IsEqual(DirNode* pNode, const wxString& strPath)
{
	bool bRet = false;
#ifdef __WXMSW__
	if(pNode->_fullName.CmpNoCase(strPath) == 0)
		bRet = true;
#else
	if(pNode->_fullName.Cmp(strPath) == 0)
		bRet = true;
#endif // __WXMSW__

	return bRet;
}

DirNode* CNextMNCD::GetChild(DirNode* pNode, int ichildIndex, bool bFindLast)
{
	size_t size = pNode->_childNode.size();
	if(size == 0)
		return nullptr;

	if(!bFindLast)
	{
		if(ichildIndex > size)
			return nullptr;

		return *(pNode->_childNode.begin() + ichildIndex);
	}

	return *(pNode->_childNode.begin() + size);
}

std::vector<DirNode *> CNextMNCD::GetAllChild(DirNode* pNode)
{
	return pNode->_childNode;
}

bool CNextMNCD::IsExistChildNode(const wxString& strChNodeName, DirNode* pParentNode)
{
	if(pParentNode->_childNode.size() == 0)
		return false;

	const_nodeIterator cItBegin = pParentNode->_childNode.begin();
	const_nodeIterator cItEnd   = pParentNode->_childNode.end();

	wxString strName(wxT(""));
	int _cmpName;
	bool bRet = false;

	while(cItBegin != cItEnd)
	{
		DirNode* pNode = (*cItBegin);

		strName = pNode->_name;
		#ifdef __WXMSW__
		_cmpName = strChNodeName.CmpNoCase(strName);
#else
		_cmpName = strChNodeName.Cmp(strName);
#endif
		if (_cmpName == 0)
		{
			bRet = true;
			break;
		}

		++cItBegin;
	}

	return bRet;
}

int CNextMNCD::GetChildSize(DirNode* pNode)
{
	return pNode->_childNode.size();
}

void CNextMNCD::ChangeNodeName(DirNode* pNode, const wxString& strNewName)
{
	wxString strParent = pNode->_parent->_fullName;

	wxString strFullOld = pNode->_fullName;
	wxString strFullNew = strParent + strNewName;

	pNode->_name = strNewName;

	wxRename(strFullOld, strFullNew);
	m_bShouldbeSaved = true;
}

void CNextMNCD::SaveNCD()
{
	wxFile fn(m_strNCDFile, wxFile::write);
	std::vector<DirNode *>::const_iterator cItBegin = Begin();

	wxString strSave(wxT(""));
	while(cItBegin != End())
	{
		DirNode* pNode = (*cItBegin);
		strSave = wxString::Format(wxT("%d^"), pNode->_depth) + pNode->_name + wxT("\n");

		const wxWX2MBbuf tmp_buf = wxConvCurrent->cWX2MB(strSave);
		const char *tmp_str = (const char*)tmp_buf;

		fn.Write(tmp_str, strlen(tmp_str));
		++cItBegin;
	}

	fn.Close();
}
