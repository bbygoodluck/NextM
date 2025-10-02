#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

//wxWidgets header
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/snglinst.h>
#include <wx/filename.h>
#include <wx/xrc/xmlres.h>
#include <wx/aui/aui.h>
#include <wx/display.h>
#include <wx/splitter.h>
#include <wx/tokenzr.h>
#include <wx/volume.h>
#include <wx/regex.h>
#include <wx/thread.h>
#include <wx/clipbrd.h>
#include <wx/fontenum.h>
#include <wx/busyinfo.h>

#ifdef __WXMSW__
	#define _WIN32_DCOM
	#include <comdef.h>
	#include <wbemidl.h>
	#include <wx/msw/private.h>
	#include <windows.h>
	#include <CommCtrl.h>
	#include <imm.h>
	#include <Dbt.h>
	#include <shobjidl.h>
	#include <shlobj.h>
#endif
//std header
#include <memory>
#include <algorithm>
#include <map>
#include <fstream>
#include <iterator>
#include <list>

//버전관리
#include "Version.h"

//defines
#include "defs.h"

//enums
#include "enums.h"

//메뉴이벤트
#include "../NextMMenuEvent.h"

//extern 변수
#include "externs.h"

//이벤트 목록
#include "CustomEvent.h"

//유틸리티
#include "Utility.h"
#define theUtility   	    (CUtility::Get())

//Default Config
#include "../config/NextMConfig.h"
#define theConfig           (CNextMConfig::Get())

//Message defined
#include "../config/NextMMsg.h"
#define theMsg              (CNextMMsg::Get())

//colors
#include "../config/NextMColor.h"
#define theColor            (CNextMColor::Get())

//북마크(즐겨찾기)
#include "../config/NextMBookmark.h"
#define theBookmark         (CNextMBookmark::Get())

//외부편집 프로그램
#include "../config/NextMExternalProgInfo.h"
#define theExternal         (CExternalProgInfo::Get())

//splitter window manager
#include "../NextMSplitterManager.h"
#define theSplitterManager  (CNextMSplitterManager::Get())

#ifdef __WXMSW__
	#include "../utility/msw/DriveInfo.h"
	#include "../utility/msw/MSWFileOperation.h"
	#include "../utility/msw/MSWClipboard.h"
	#include "../config/NextMFileIconInfo.h"
	#include "../interface/msw/NextMDragDropCommon.h"

	#define theDriveInfo        (CDriveInfo::Get())
	#define theIconInfo         (CNextMFileIconInfo::Get())
	#define theFileOperation    (CMSWNextMFileOperation::Get())
	#define theClipboard        (CMSWClipboard::Get())
	#define theDnD              (CNextMDnDCommon::Get())
#else

#endif

//이미지리스트
#include "../utility/ImageList.h"
#define theImageList (CImageList::Get())

#include "../interface/NextMNCD.h"
//디렉토리관리
#define theNCD              (CNextMNCD::Get())

//압축
#include "../compress/ComDecCommon.h"
#define theComDec           (CComDecCommon::Get())

//시스템관련
#include "../system/System.h"
#define theSystem           (CSystem::Get())
#endif // COMMON_H_INCLUDED
