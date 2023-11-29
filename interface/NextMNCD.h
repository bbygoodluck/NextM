#ifndef NEXTMNDC_H_INCLUDED
#define NEXTMNDC_H_INCLUDED

/*
 * 노드데이터
 *
 */
typedef struct _DirNode {
	//노드 표시 이름
	wxString _name = wxT("");
	//Not used
	wxString _dispName = wxT("");
	//디렉토리의 전체경로
	wxString _fullName = wxT("");

	//디렉토리 표시 영역
	wxRect _rc = wxRect(0, 0, 0, 0);
	//depth
	int _depth = 0;
	//row
	int _row   = 0;
	//vector index
	int _index = 0;

	//부모노드
	_DirNode* _parent = nullptr;
	//자식노드
	std::vector<_DirNode *> _childNode;

} DirNode, *PDirNode;

class CNextMNCD
{
	//vector 컨테이너
	typedef std::vector<DirNode *> nodeContainer;
	//iterator
	typedef std::vector<DirNode *>::iterator nodeIterator;
	//const iterator
	typedef std::vector<DirNode *>::const_iterator const_nodeIterator;

private:
	CNextMNCD();

public:
	~CNextMNCD();
	static CNextMNCD* Get();

	//시작 iterator
	nodeIterator Begin() { return m_nodeDatas.begin(); }
	//마지막 iterator
	nodeIterator End() { return m_nodeDatas.end(); }

	//시작 iterator
	const_nodeIterator CBegin() { return m_nodeDatas.cbegin(); }
	//마지막 iterator
	const_nodeIterator CEnd() { return m_nodeDatas.cend(); }

public:
	/*
	 * 초기화 작업 진행(resere 등..)
	 */
	void Initialize(size_t iInitializeSize = 1000);

	/*
	 * 메모리 해제...
	 *
	 *
	 */
	void UnInitialize();

	/*
	 * 노드 삭제
	 * @Parameter
	 *  pNode : Clear하기위한 노드(하위노드 포함)
	 */
	void Clear(DirNode* pNode);

	/*
	 * NCDFile 저장용
	 * @Parameter
	 *  strNCDFile : NCD파일 저장위치
	 */
	void SetNCDFile(const wxString& strNCDFile);

	/*
	 * Root 노드 가져오기
	 *
	 */
	DirNode* GetRoot() { return m_pRoot; }

	/*
	 * 노드 추가
	 * @Parameter
	 *  strName : 추가할 노드명
	 *  parent  : 부모노드
	 *  iDepth  : 추가되는 노드의 depth
	 */
	DirNode* AddNode(const wxString& strName, DirNode* parent, int iDepth);

	/*
	 * 노드삭제
	 * @Parameter
	 *  pNode : 삭제노드
	 */
	void DeleteNode(DirNode* pNode);

	/*
	 * 부모노드 찾기
	 * @Parameter
	 *   iDepth       : 현재노드의 depth
	 *   pCurrDirNode : 부모노드를 찾기위한 현재노드
	 */
	DirNode* FindParentNode(int iDepth, DirNode* pCurrDirNode);

	/*
	 * 자식노드 가져오기
	 * @Parameter
	 *  pNode       : 자식노드를 가져오기 위한 현재노드
	 *  ichildIndex : 자식노드 인덱스
	 *  bFindLast   : 마지막 자식노드를 가져오기위한 플래그
	 */
	DirNode* GetChild(DirNode* pNode, int ichildIndex = 0, bool bFindLast = false);

	/*
	 * 자식노드의 존재여부 확인
	 * @Parameter
	 *  strChNodeName : 자식노드명(윈도우는 대소문자 구문없음)
	 *  pParentNode   : 부모노드..
	 */
	bool IsExistChildNode(const wxString& strChNodeName, DirNode* pParentNode);


	/*
	 * 자식노드 전체를 가져옴
	 * @Parameter
	 *  pNode : 현재노드
	 */
	std::vector<DirNode *> GetAllChild(DirNode* pNode);

	/*
	 * 자식노드의 크기 가져오기
	 * @Parameter
	 *   pNode : 자식노드의 크기를 가져오기위한 노드
	 */
	int GetChildSize(DirNode* pNode);

	/*
	 * Tree노드들을 Vector에 적재함
	 *
	 */
	void SetOrder();

	/*
	 * Reordering
	 *
	 *
	 */
	void SetReOrder(const wxString& strPath = wxT(""));
	/*
	 * 현재노드 가져오기
	 * @Parameter
	 *   strPath : 경로(FullPath)
	 */
	void SetCur(const wxString& strPath);

	/*
	 * 현재 노드를 가져옴
	 *
	 */
	nodeIterator& GetCur() { return m_pCurNode; }

	/*
	 * strPath와 같은 노드인지 체크함
	 * @Parameter
	 *  pNode : 체크 노드
	 *  strPath : 체크패스
	 */
	bool IsEqual(DirNode* pNode, const wxString& strPath);

	/*
	 * 전체 노드사이즈를 가져옴
	 *
	 */
	unsigned int GetSize() const { return m_nodeDatas.size(); }

	/*
	 * 마지막 depth
	 *
	 */
	int GetMaxDepth() const { return _maxDepth; }
	int GetMaxRow() const {return _curRow; }

	/*
	 * 노드명 변경
	 * @Parameter
	 *  pNode : 선택된 노드
	 *  strNewName : 변경할 이름
	 */
	void ChangeNodeName(DirNode* pNode, const wxString& strNewName);

	/*
	 * NCD 저장
	 *
	 *
	 */
	void SaveNCD();

private:
	static std::unique_ptr<CNextMNCD> m_pInstance;

	DirNode* m_pRoot = nullptr;
	int _curRow   = 0;
	int _curDepth = 0;
	int _maxDepth = -1;

	std::vector<DirNode *> m_nodeDatas;
	std::vector<DirNode *>::iterator m_pCurNode;

	wxString m_strNCDFile = wxT("");
	bool m_bShouldbeSaved = false;
};
#endif // NEXTMNDC_H_INCLUDED
