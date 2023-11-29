#ifndef MSWFILEOPERATION_H_INCLUDED
#define MSWFILEOPERATION_H_INCLUDED

#include "../FileOperation.h"
class CMSWNextMFileOperation : public CFileOperation
{
private:
    CMSWNextMFileOperation() {}

public:
    ~CMSWNextMFileOperation() { Release(); }
    static CMSWNextMFileOperation* Get();

    //복사 또는 이동
    bool CopyOrMoveOperations(std::list<wxString>& dirsToVisit, const wxString& strDest, wxWindow* parent, bool IsCopy, bool bSameDir = false) override;
    bool CopyOrMoveOperations(const wxString& strItem, const wxString& strDest, wxWindow* parent, bool IsCopy, bool bSameDir = false);

    //삭제
	bool RecursiveDelete(const std::list<wxString>& dirsToVisit, wxWindow* parent, bool bGoTrash, int& iRet) override;
	bool RecursiveDelete(const wxString& path, wxWindow* parent, bool bGoTrash, int& iRet);

	//윈도우의 경우 같은 디렉토리로 파일을 복사할 경우
	//복사본을 만들어서 처리해야 함.
	bool CopyOrMoveOperationsToSameDir(std::list<wxString>& dirsToVisit, const wxString& strDest, bool IsCopy);
	bool ExecuteDeleteProcess(const std::list<wxString>& dirsToVisit, wxWindow* parent, bool bGoTrash, int& iRet);

	DWORD GetLastErrorCode() const { return m_dwErrorCode; }

private:
	bool Init();
	void Release();

	HRESULT CreateAndInitFileOperation(REFIID riid, void **ppv);
	void SetErrorCode() { m_dwErrorCode = ::GetLastError(); }


private:
	static std::unique_ptr<CMSWNextMFileOperation> m_pInstance;

	IFileOperation *m_pfo;
	DWORD m_dwErrorCode;
	HRESULT m_hr;
};

#endif // MSWFILEOPERATION_H_INCLUDED
