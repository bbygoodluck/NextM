#ifndef JSONIMAGEMAP_H_INCLUDED
#define JSONIMAGEMAP_H_INCLUDED

#include "JsonBase.h"
class CNextMFileIconInfo : public CJsonBase
{
    struct _stFileIconInfo{
        int m_iIconIndex;
        int m_iOverayIndex;
        wxString m_strTypeName;
    };

private:
	CNextMFileIconInfo();
    void Init();

public:
	~CNextMFileIconInfo();
	static CNextMFileIconInfo* Get();

	virtual bool Load();
	bool GetFileIconInfo(const wxString& strExt, int& IconIndex, int& overlayIndex, wxString& strTypeName);

private:
    std::unordered_map<wxString, _stFileIconInfo> m_mapFileIconInfo;

private:
	static std::unique_ptr<CNextMFileIconInfo> m_pInstance;
};

#endif // JSONIMAGEMAP_H_INCLUDED
