#include "../ginc.h"
#include "Sorting.h"

CSorting::CSorting(int iSortType)
	: m_iSortType(iSortType)
{
}

CSorting::~CSorting()
{
}

bool CSorting::DirSortOfName(const CNextMDirData& lhs, const CNextMDirData& rhs)
{
	bool blhsDrive = lhs.IsDrive();
	bool brhsDrive = rhs.IsDrive();

	if(blhsDrive)
	{
		if(!brhsDrive)
			return false;
	}
	else
	{
		if (brhsDrive)
			return true;
	}

	bool blhsDir = lhs.IsDir();
	bool brhsDir = rhs.IsDir();

	int iRes = CmpDir(blhsDir, brhsDir);
	if (iRes < 0)
		return true;
	else if (iRes > 0)
		return false;

	wxString strLName = lhs.GetName();
	wxString strRName = rhs.GetName();

	iRes = CmpName(strLName, strRName);

	if (iRes < 0)
		return true;
	else
		return false;
}

bool CSorting::DirSortOfSize(const CNextMDirData& lhs, const CNextMDirData& rhs)
{
	bool blhsDrive = lhs.IsDrive();
	bool brhsDrive = rhs.IsDrive();

	if(blhsDrive)
	{
		if(!brhsDrive)
			return false;
	}
	else
	{
		if (brhsDrive)
			return true;
	}

	wxLongLong lllhs = lhs.GetSize();
	wxLongLong llrhs = rhs.GetSize();

	return lllhs.GetValue() > llrhs.GetValue();
}

bool CSorting::DirSortOfTime(const CNextMDirData& lhs, const CNextMDirData& rhs)
{
	bool blhsDrive = lhs.IsDrive();
	bool brhsDrive = rhs.IsDrive();

	if(blhsDrive)
	{
		if(!brhsDrive)
			return false;
	}
	else
	{
		if (brhsDrive)
			return true;
	}

	wxDateTime dtlhs = lhs.GetDateTime();
	wxDateTime dtrhs = rhs.GetDateTime();

	return dtlhs > dtrhs;
}

int CSorting::CmpDir(bool blhsDir, bool brhsDir)
{
	if (blhsDir)
	{
		if (!brhsDir)
			return -1;
		else
			return 0;
	}
	else
	{
		if (brhsDir)
			return 1;
		else
			return 0;
	}
}

int CSorting::CmpName(const wxString& strLeft, const wxString& strRight)
{
	wxString strLName(strLeft);
	wxString strRName(strRight);

	int cmp = strLName.CmpNoCase(strRName);
	if(strLName.Cmp(wxT("..")) == 0)
		return -1;
	else if(strRName.Cmp(wxT("..")) == 0)
		return 1;

	return cmp;
}
