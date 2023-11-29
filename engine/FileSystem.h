#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

#include <filesystem>
class CFileSystem
{
public:
	CFileSystem() {}
	~CFileSystem() {}

public:
	wxString GetPermission(const wxString& strFullPathFileName)
	{
		wxString strPermission(wxT(""));
		std::filesystem::perms p = std::filesystem::status(CONVSTR_TO_STD(strFullPathFileName)).permissions();

		using std::filesystem::perms;
		auto GetPerms = [&](const wxString& strPerms, perms perm)
		{
			if(perms::none == (perm & p))
				strPermission += wxT("-");
			else
				strPermission += strPerms;
		};

		GetPerms(wxT("r"), perms::owner_read);
		GetPerms(wxT("w"), perms::owner_write);
		GetPerms(wxT("x"), perms::owner_exec);
		GetPerms(wxT("r"), perms::group_read);
		GetPerms(wxT("w"), perms::group_write);
		GetPerms(wxT("x"), perms::group_exec);
		GetPerms(wxT("r"), perms::others_read);
		GetPerms(wxT("w"), perms::others_write);
		GetPerms(wxT("x"), perms::others_exec);

		return strPermission;
	}
};

#endif // FILESYSTEM_H_INCLUDED
