#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <chrono>
class CUtility final
{
private:
	explicit CUtility() {}

public:
	~CUtility() {}
	static CUtility* Get();

public:
#ifdef __WXMSW__
	int GetWindowsVersion();
#endif
    //색상정보가져오기
	wxColour GetColor(unsigned long &ulColor) {
		unsigned long r = GetRedColor(ulColor);
		unsigned long g = GetGreenColor(ulColor);
		unsigned long b = GetBlueColor(ulColor);

		return wxColour(r, g, b);
	}

	//RGB colour 값 구하기
	const unsigned long GetRedColor(unsigned long ulColor)   { return ((ulColor & 0x00FF0000) >> 16); }
	const unsigned long GetGreenColor(unsigned long ulColor) { return ((ulColor & 0x0000FF00) >> 8); }
	const unsigned long GetBlueColor(unsigned long ulColor)  { return (ulColor & 0x000000FF); }

	//platform 언어가져오기
	wxString GetSystemLang();

    //실행디렉토리 가져오기
    const wxString& GetWorkDir() const { return m_strWorkingDirectory;  }

    //패스이름 가져오기
    wxString GetPathName(const wxString& strFullPath);
	//부모 패스정보 가져오기
    wxString GetParentPath(const wxString& strFullPath);

    //파일명 가져오기
	wxString GetFileName(const wxString& strFullPath, bool IsAppendExt = true);

    //FullPath 만들기
    wxString MakeFullPathName(const wxString& strCurrentPath, const wxString& strNewPathName);

    //메세지 표시
	void ShowMessageBox(const wxString& strMsg, int iRetCode);

	//화면 다시그리기
	void RefreshWindow(wxWindow* pWindow, const wxRect& rect = wxRect(0, 0, 0, 0), bool bUpdate = false);

	//허용 속성
	bool IsAllowAttr(const unsigned long _lattr);

	//비교하기
	int Compare(const wxString& strOld, const wxString& strNew);

	//최대값 가져오기
	wxString GetMaxData(wxWindow* pWindow, const wxFont& _font, const wxString& strSrc, const wxString& strTgt);

	//키이름 가져오기
	wxString GetKeyName(const wxKeyEvent& event);
	const char* GetVirtualKeyCodeName(int nKeyCode);

	//복사할때 사용
	wxString ChangeName(const wxString& strName);

	//확장자 가져오기
	wxString GetExt(const wxString& strFileName);
	//파일 설명 가져오기
	wxString GetFileDescription(const wxString& strExt, const wxString& strFullPathName);

	//파일사이즈 정보가져오기
	void GetSizeInfo(const double dblFileSize, wxString& strFileSize, wxString& strFileSizeType, wxColour& dispColor);
#ifdef __WXMSW__
	//IME 설정
	void SetImeModeToEnglish(wxWindow* pWindow);
#endif // __WXMSW__

	//사이즈에 콤마찍기
	wxString SetComma(const wxString& strSize);
	wxString SetComma(unsigned long long val);

	//프로그램 실행
	void LaunchAndExec(const wxString& strExecProgram, const wxString& strPath);

	//자르기
	std::vector<wxString> Split(const wxString& strSource, const wxString& strDelimiter);

	//문자변환(wchar to char)
	const char* ConvertWC2MB(const wxString& strData);

#ifndef NDEBUG
	void LogDisplay(const wxString& strMsg);
#endif

	void SetTopWindow(HWND hWndInsertAfter);

	//! Much of the following code is based on the implementation used in btop4win
	//! See: https://github.com/aristocratos/btop4win
	//* Return current time since epoch in seconds
	inline uint64_t time_s() {
		return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	//* Return current time since epoch in milliseconds
	inline uint64_t time_ms() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	//* Return current time since epoch in microseconds
	inline uint64_t time_micros() {
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	wxString floating_humanizer(uint64_t value, const bool is_mega = false, const bool shorten = false, size_t start = 0, const bool bit = false, const bool per_second = false);
private:
	void SetWorkingDirectory();
	void Init();

private:
	static std::unique_ptr<CUtility> m_pInstance;
	wxString m_strWorkingDirectory = wxT("");

	unsigned long larrAttr[3] = {0, };
};

#endif // UTILITY_H_INCLUDED
