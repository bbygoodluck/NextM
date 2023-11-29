#ifndef NEXTMNDC_H_INCLUDED
#define NEXTMNDC_H_INCLUDED

typedef struct _DirNode {
	wxString _name = wxT("");
	wxString _dispName = wxT("");
	_DirNode* _parent = nullptr;

	std::vector<_DirNode *> _childNode;

} DirNode, *PDirNode;

class CNextMNCD
{
private:
	CNextMNCD();

public:
	~CNextMNCD();
	static CNextMNCD* Get();

public:
	void Initialize();

private:
	static std::unique_ptr<CNextMNCD> m_pInstance;

	DirNode* m_pRoot = nullptr;
};
#endif // NEXTMNDC_H_INCLUDED
