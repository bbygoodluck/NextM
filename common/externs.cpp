#include "common.h"

// 메인윈도우
wxFrame* _gMainFrame = nullptr;

// 이미지리스트
SYSTEM_IMAGE_LIST _gImageList = nullptr;

// System language
wxString _gSystemLang         = wxT("");

// Default 폴더 이미지
wxIcon _gNormalFolderIco      = wxNullIcon;

// Default 파일 이미지
wxIcon _gNormalFileIco        = wxNullIcon;

// 전역뷰폰트
wxFont* _gViewFont            = nullptr;

//파일속성처리 인덱스
unsigned int _gAttrIndex = 0;

// 디렉토리 탐색시 제외 항목
std::vector<wxString> _gVecIgnore;

//화면이동에 대한 허용키
std::unordered_map<int, int> _gmapAllowProcessKey;

//메뉴이벤트처리
CNextMMenuEvent* _gMenuEvent = nullptr;
