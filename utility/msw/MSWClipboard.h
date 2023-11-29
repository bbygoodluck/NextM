#ifndef MSWCLIPBOARD_H_INCLUDED
#define MSWCLIPBOARD_H_INCLUDED

#include "../Clipboard.h"
class CMSWClipboard : public CClipboardBase
{
private:
	CMSWClipboard() {};

public:
	virtual ~CMSWClipboard() {
		Clear();
	};

	static CMSWClipboard* Get();

public:
	void AddData(const wxString& strData) override;
	void AddData(const std::list<wxString>& lstDatas) override;
	void CopyToClipboard() override;
	void GetData(std::list<wxString>& lstDatas) override;
	void Clear();

	void SetSrcPath(const wxString& strSrcPath) { m_strSrcPath = strSrcPath; }
	wxString GetSrcPath() { return m_strSrcPath; }

private:
	static std::unique_ptr<CMSWClipboard> m_pInstance;
	std::list<wxString> m_aDatas;

	wxString m_strSrcPath = wxT("");
};

#endif // MSWCLIPBOARD_H_INCLUDED
