#ifndef NEXTMEXTERNALPROGINFO_H_INCLUDED
#define NEXTMEXTERNALPROGINFO_H_INCLUDED

#include "JsonBase.h"
#include "../interface/NextMExternalProgItem.h"
class CExternalProgInfo : public CJsonBase
{
private:
	CExternalProgInfo();

public:
	~CExternalProgInfo();
	static CExternalProgInfo* Get();

	std::vector<CExternalProgItem>::iterator BeginIter() { return m_external.begin(); }
	std::vector<CExternalProgItem>::iterator EndIter() { return m_external.end(); }

	int GetExternalProgCount() { return m_external.size(); }
	wxMenu* GetMenu() { return &m_menu; }

public:
	bool Load() override;
	void ResetExternalProgram(const std::vector<CExternalProgItem>& exItems);
	void AddExternalProgram(const CExternalProgItem& exItem);

	void ClearExternalItems() {
		m_external.clear();
	}

private:
	void UpdateDocument();
	void SetExternalProgramItems();
	void CreateExternalMenuList();

private:
	static std::unique_ptr<CExternalProgInfo> m_pInstance;

	std::vector<CExternalProgItem> m_external;
	wxMenu m_menu;
};

#endif // NEXTMEXTERNALPROGINFO_H_INCLUDED
