#ifndef __SORTING_H__
#define __SORTING_H__

#include "../interface/NextMDirData.h"
class CSorting
{
public:
	CSorting(int iSortType);
	~CSorting();

public:
	static bool DirSortOfName(const CNextMDirData& lhs, const CNextMDirData& rhs);
	static bool DirSortOfSize(const CNextMDirData& lhs, const CNextMDirData& rhs);
	static bool DirSortOfTime(const CNextMDirData& lhs, const CNextMDirData& rhs);

private:
	static int CmpDir(bool blhsDir, bool brhsDir);
	static int CmpName(const wxString& strLeft, const wxString& strRight);

private:
	int m_iSortType;
};
#endif
