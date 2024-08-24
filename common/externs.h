#ifndef EXTERNS_H_INCLUDED
#define EXTERNS_H_INCLUDED

#ifdef __WXMSW__
	typedef HIMAGELIST SYSTEM_IMAGE_LIST;
#else
	typedef wxImageList* SYSTEM_IMAGE_LIST;
#endif

// 메인윈도우
extern wxFrame* _gMainFrame;

// 이미지리스트
extern SYSTEM_IMAGE_LIST _gImageList;

// System language
extern wxString _gSystemLang;

// Default 폴더 이미지
extern wxIcon _gNormalFolderIco;

// Default 파일 이미지
extern wxIcon _gNormalFileIco;

// 전역뷰폰트
extern wxFont* _gViewFont;

//속성
extern unsigned int _gAttrIndex;

// 디렉토리 탐색시 제외 항목
extern std::vector<wxString> _gVecIgnore;

//화면이동에 대한 허용키
extern std::unordered_map<int, int> _gmapAllowProcessKey;

//메뉴이벤트처리
extern CNextMMenuEvent* _gMenuEvent;
#endif // EXTERNS_H_INCLUDED
