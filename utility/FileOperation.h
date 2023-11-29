#ifndef FILEOPERATION_H_INCLUDED
#define FILEOPERATION_H_INCLUDED

class CFileOperation
{
public:
	CFileOperation() {}
	~CFileOperation() {}

	virtual bool CopyOrMoveOperations(std::list<wxString>& dirsToVisit, const wxString& strDest, wxWindow* parent, bool IsCopy, bool bSameDir = false) = 0;
	virtual bool RecursiveDelete(const std::list<wxString>& dirsToVisit, wxWindow* parent, bool bGoTrash, int& iRet) = 0;
};

#endif // FILEOPERATION_H_INCLUDED
