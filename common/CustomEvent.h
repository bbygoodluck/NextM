#ifndef CUTOMEVENT_H_INCLUDED
#define CUTOMEVENT_H_INCLUDED

//디렉토리 이동시 Tab의 디렉토리명 표시
//wxDECLARE_EVENT(wxEVT_DISPLAY_DIR, wxCommandEvent);
//이름변경
wxDECLARE_EVENT(wxEVT_ITEM_RENAME, wxCommandEvent);

//파일편집
wxDECLARE_EVENT(wxEVT_FILE_EDIT, wxCommandEvent);

#ifdef __WXMSW__
wxDECLARE_EVENT(wxEVT_DISK_DRIVE_INIT, wxCommandEvent);
//디스크 용량이 변경(파일추가, 삭제시..)
wxDECLARE_EVENT(wxEVT_UPDATE_DRIVE_SIZEINFO, wxCommandEvent);
#endif // __WXMSW__

//컨택스트 메뉴 보기
wxDECLARE_EVENT(wxEVT_SHOW_CONTEXTMENU, wxCommandEvent);

//파일 시스템 Watcher
wxDECLARE_EVENT(wxEVT_FS_WATCHER, wxCommandEvent);

//Refresh, Up, Down, Back, Forward 메뉴 실행
wxDECLARE_EVENT(wxEVT_EXEC_MENU_OPERATION, wxCommandEvent);

//화면 컬럼 변경
wxDECLARE_EVENT(wxEVT_CHANGE_VIEW_COLUMN, wxCommandEvent);

//정렬방식 변경
wxDECLARE_EVENT(wxEVT_CHANGE_VIEW_SORT, wxCommandEvent);

//상태바 즐겨찾기 클릭
wxDECLARE_EVENT(wxEVT_VIEW_FAVORITE_FROM_STATUS, wxCommandEvent);

//디렉토리 번호 표시(디렉토리 이동시 사용)
wxDECLARE_EVENT(wxEVT_VIEW_DIR_NUM, wxCommandEvent);
#endif // CUTOMEVENT_H_INCLUDED
