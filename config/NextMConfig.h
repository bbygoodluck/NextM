#ifndef NEXTMCONFIG_H_INCLUDED
#define NEXTMCONFIG_H_INCLUDED

#include "JsonBase.h"
class CNextMConfig : public CJsonBase
{
private:
	CNextMConfig();

public:
	~CNextMConfig();
	static CNextMConfig* Get();

//	bool IsExistMember(const wxString& strGroup, const wxString& strMember);

public:
	bool Load() override;

	const wxRect& GetDefaultRect()         { return m_rcDefault; }
	bool IsViewSystemFile()			const  { return (_jsonDoc["globalsettings"]["DispSystemFile"].GetInt() == 1 ? true : false); }
	bool IsViewHiddenFile()			const  { return (_jsonDoc["globalsettings"]["DispHiddenFile"].GetInt() == 1 ? true : false); }
	bool IsViewAllFile()			const  { return (_jsonDoc["globalsettings"]["DispAllFile"].GetInt() == 1 ? true : false); }

	const int GetPreIconReadCount() const  { return _jsonDoc["globalsettings"]["preIconRead"].GetInt();}
	const int GetWidth()            const  { return _jsonDoc["globalsettings"]["Width"].GetInt();}
	const int GetHeight()			const  { return _jsonDoc["globalsettings"]["Height"].GetInt();}
	const int GetXPos()				const  { return _jsonDoc["globalsettings"]["XPos"].GetInt();}
	const int GetYPos()				const  { return _jsonDoc["globalsettings"]["YPos"].GetInt();}
	const int GetSplitStyle()		const  { return _jsonDoc["globalsettings"]["SplitWindowStyle"].GetInt();}
	const int GetViewToolbar()		const  { return _jsonDoc["globalsettings"]["ViewToolBar"].GetInt();}
	const int GetSortType()			const  { return  _jsonDoc["globalsettings"]["SortType"].GetInt();}

	const int GetColumnCount()		const  { return _jsonDoc["globalsettings"]["DispColumn"].GetInt();}

	wxString GetFontName()          const  { return _jsonDoc["globalsettings"]["fontname"].GetString(); }
	const int GetFontSize()		    const  { return _jsonDoc["globalsettings"]["fontsize"].GetInt(); }

	const long GetTabStyle()        const  { return m_tabStyle; }
	const int GetMonitorPos()       const  { return _jsonDoc["globalsettings"]["DispMonitor"].GetInt(); }

	const bool IsShift_IME_KOR_MoveDrive() { return _jsonDoc["globalsettings"]["Shift_IME_KOR_MoveDrv"].GetInt() ? true : false; }

	const int IsMaximized() const          { return _jsonDoc["globalsettings"]["Maximized"].GetInt();  }
	const bool IsSaveLastSession()         { return _jsonDoc["globalsettings"]["LastSessionSave"].GetInt() ? true : false; }

	bool IsCenterOnScreen()                { return _jsonDoc["globalsettings"]["DisplayCenterOnScreen"].GetInt() ? true : false; }
	bool IsSelectedSCDItemToParent()       { return _jsonDoc["globalsettings"]["SelectSCDItem_toparent"].GetInt() ? true : false; }

	wxString GetLeftPath()
	{
		wxString strPath = _jsonDoc["globalsettings"]["LeftPath"].GetString();
		if(strPath.Right(1).CmpNoCase(SLASH) != 0)
			strPath += SLASH;

		if (!wxDirExists(strPath))
			return wxT("C:\\");

		return strPath;
	}

	wxString GetRightPath()
	{
		wxString strPath = _jsonDoc["globalsettings"]["RightPath"].GetString();

		if(strPath.Right(1).CmpNoCase(SLASH) != 0)
			strPath += SLASH;

		if (!wxDirExists(strPath))
			return wxT("C:\\");

		return  strPath;
	}

	const int GetUseSHBrowseForFolder()  const       { return _jsonDoc["globalsettings"]["UseWindowSHBrowseFolder"].GetInt(); }
	const int GetUseWindowShellFileOperation() const { return _jsonDoc["globalsettings"]["UseWindowShell"].GetInt(); }
	const wxString GetColorTheme() const             { return _jsonDoc["globalsettings"]["color"].GetString(); }

//	void SaveConfig();

	void SetChangeGlobalVal(const wxString& strMember, int iVal);
	void SetChangeGlobalVal(const wxString& strMember, const wxString& strVal);

private:
	static std::unique_ptr<CNextMConfig> m_pInstance;
//	void SetDefaultConfig();

private:
	int m_nWidth;
	int m_nHeight;
	int m_nXPos;
	int m_nYPos;
	int m_nSplitWindowStyle;
	int m_nViewToolBar;
	int m_nSortType;
	int m_nViewFileList;
	int m_nDispColumn;
	int m_nDispMsgBoxDirWatcher;
	int m_nDispFloppy;
	int m_nDispSystemFile;
	int m_nDispHiddenFile;
	int m_nDispAllFile;
	int m_nDispHeader;
	int m_nDispDriveListView;
	int m_nMinThreadRead;
	int m_nFileInfoView;
	int m_nTimeFormat;
	int m_nFontSize;
	int m_nPreIconReadCount;
	int m_nCurrentMonitor;
	int m_nMaximized;
	int m_nLastSessionSave;
	int m_iPreRead;
	int m_iCopyMoveSamePath;
	int m_iCopyMoveUseWindowShell;
	int m_iCopyMoveActiveWindowToNonActiveWindow;
	int m_iShift_IME_KOR_MoveDrv;
	int m_iDirectoryWatcherAsync;
	int m_iUseSHBrowseForFolder;
	int m_iItemDisplayFromBack;
	int m_iCenterScreen;
	int m_iTopSCDParent;
	int m_iUseWindowShellFileOperation;

	wxString m_strFontName;
	wxString m_strLeftPath;
	wxString m_strRightPath;

	long m_tabStyle;
	wxRect m_rcDefault;

	wxString m_strColorTheme = wxT("");
};

#endif // NEXTMCONFIG_H_INCLUDED
