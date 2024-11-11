#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#define EVT_MY_CUSTOM_COMMAND(custom_cmd, id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        custom_cmd, id, wxID_ANY, \
        wxCommandEventHandler(fn), \
        (wxObject *) NULL \
    ),

#if defined(_UNICODE) || defined(UNICODE)
	#define CONVSTR(x)        (x.wchar_str())
    #define CONVSTR_TO_STD(x) (x.ToStdWstring())
#else
	#define CONVSTR(x)        (x.char_str())
    #define CONVSTR_TO_STD(x) (x.ToStdString())
#endif

#define CONVSTR_TO_C_STR(x)   (x.c_str())

const wxString SLASH = wxString(wxFILE_SEP_PATH);

//constexpr long CHILD_WINDOW_STYLE = wxBORDER_NONE | /*wxCLIP_CHILDREN |  wxTRANSPARENT_WINDOW  | wxCLIP_SIBLINGS | */ wxNO_FULL_REPAINT_ON_RESIZE;
//constexpr long IMAGELIST_FLAG = SHGFI_ICON | SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_OVERLAYINDEX;// | SHGFI_USEFILEATTRIBUTES ;
//constexpr long FILELISTVIEW_STYLE = wxBORDER_NONE | wxWANTS_CHARS | /*wxCLIP_CHILDREN | wxTRANSPARENT_WINDOW  | wxCLIP_SIBLINGS | */ wxNO_FULL_REPAINT_ON_RESIZE;

constexpr long CHILD_WINDOW_STYLE = wxBORDER_NONE | wxCLIP_CHILDREN | /* wxTRANSPARENT_WINDOW  |*/ wxCLIP_SIBLINGS | wxNO_FULL_REPAINT_ON_RESIZE;
constexpr long IMAGELIST_FLAG = SHGFI_ICON | SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_OVERLAYINDEX;// | SHGFI_USEFILEATTRIBUTES ;
constexpr long FILELISTVIEW_STYLE = wxBORDER_NONE | wxWANTS_CHARS | wxCLIP_CHILDREN | /* wxTRANSPARENT_WINDOW  |*/ wxCLIP_SIBLINGS | wxNO_FULL_REPAINT_ON_RESIZE;

constexpr int wxKEY_DOT			= 46;
//const int wxKEY_SLASH		= 92;
constexpr int WXK_REVERSE_SLASH = 92;  // [\]
constexpr int WXK_SLASH         = 47;  // [/]
constexpr int WXK_COLONE        = 58;  // [:]
constexpr int WXK_QUESTION      = 63;  // [?]
constexpr int WXK_DBQUATATION   = 34;  // ["]
constexpr int WXK_LEFT_M_G      = 60;  // [<]
constexpr int WXK_RIGHT_M_G     = 62;  // [>]
constexpr int WXK_V_LINE        = 124; // [|]
constexpr int WXK_WINDOWS_KEY   = 393;

constexpr unsigned long ATTR_DIR	 = FILE_ATTRIBUTE_DIRECTORY;
constexpr unsigned long ATTR_ARCHIVE = FILE_ATTRIBUTE_ARCHIVE;
constexpr unsigned long ATTR_HIDDEN	 = FILE_ATTRIBUTE_HIDDEN;
constexpr unsigned long ATTR_SYSTEM	 = FILE_ATTRIBUTE_SYSTEM;
constexpr unsigned long ATTR_RDONLY	 = FILE_ATTRIBUTE_READONLY;

constexpr int wxSHC_MENU_TOP = 0;
constexpr int wxSHC_MENU			= wxID_HIGHEST + 1001;	//wxWidgets의 가장 마지막 ID값은 wxID_HIGHEST(5999)
constexpr int wxSHC_MENU_FILE		= wxSHC_MENU;			//파일메뉴 ID시작
constexpr int wxSHC_MENU_EDIT		= wxSHC_MENU + 100;
constexpr int wxSHC_MENU_PATH		= wxSHC_MENU + 200;
constexpr int wxSHC_MENU_COMP		= wxSHC_MENU + 300;
constexpr int wxSHC_MENU_VIEW		= wxSHC_MENU + 400;
constexpr int wxSHC_MENU_FAVORITE	= wxSHC_MENU + 500;
constexpr int wxSHC_MENU_TOOLS		= wxSHC_MENU + 600;
constexpr int wxSHC_MENU_HELP		= wxSHC_MENU + 700;
constexpr int wxSHC_MENU_SEPERATOR	= 9999;

static int wxTABWindowID		= wxSHC_MENU + 2000;
static int wxFILFELIST_VIEWID	= wxSHC_MENU + 3000;
static int wxINFOVIEW_ID		= wxSHC_MENU + 3500;
static int wxDRIVEVIEW_ID		= wxSHC_MENU + 3600;
static int wxHISTORY_ID			= wxSHC_MENU + 4000;
static int wxPATHVIEW_ID		= wxSHC_MENU + 4500;
static int wxDRIVE_ID_START		= wxSHC_MENU + 5000;		//드라이브목록ID는 100개(default)
static int wxDRIVE_ID_END		= wxDRIVE_ID_START + 100;
//디렉토리 하위목록리스
static int wxSUBDIR_START       = wxDRIVE_ID_END + 100;
static int wxSUBDIR_END         = wxSUBDIR_START + 1000;

//즐겨찾기ID
constexpr int wxFAVIROTE_ID_START	= wxSHC_MENU + 5500;
constexpr int wxFAVIROTE_ID_END     = wxFAVIROTE_ID_START + 499;

constexpr double KILOBYTE = 1000.0f;
constexpr double MEGABYTE = (1000.0f * 1000.0f);
constexpr double GIGABYTE = (1000.0f * 1000.0f * 1000.0f);
constexpr double TERABYTE = (1000.0f * 1000.0f * 1000.0f * 1000.0f);

const wxString TIME_FORMAT_DEFAULT = wxT("%04d-%02d-%02d %02d:%02d");
const wxString SPACE_FORMAT_DEFAULT = wxT("%s / %s");

constexpr int TOOLBAR_START = 500;

constexpr int TOOLBAR_ID_GO_CMD                  = TOOLBAR_START + 1;
constexpr int TOOLBAR_ID_GO_ROOT                 = TOOLBAR_START + 2;
constexpr int TOOLBAR_ID_GO_UP                   = TOOLBAR_START + 3;
constexpr int TOOLBAR_ID_GO_DOWN                 = TOOLBAR_START + 4;
constexpr int TOOLBAR_ID_GO_BACK                 = TOOLBAR_START + 5;
constexpr int TOOLBAR_ID_GO_FORWARD              = TOOLBAR_START + 6;
constexpr int TOOLBAR_ID_REFRESH                 = TOOLBAR_START + 7;
constexpr int TOOLBAR_ID_FILE_EDIT               = TOOLBAR_START + 8;
constexpr int TOOLBAR_ID_FIND                    = TOOLBAR_START + 9;
constexpr int TOOLBAR_ID_DIR_MANAGER             = TOOLBAR_START + 10;
constexpr int TOOLBAR_ID_BOOKMARK                = TOOLBAR_START + 11;
constexpr int TOOLBAR_ID_FUNCTION_KEY            = TOOLBAR_START + 12;
constexpr int TOOLBAR_ID_ENV_SETTING             = TOOLBAR_START + 13;
constexpr int TOOLBAR_ID_ENV_SAVE                = TOOLBAR_START + 14;
constexpr int TOOLBAR_ID_RESOURCE_USAGE          = TOOLBAR_START + 15;
constexpr int TOOLBAR_ID_DISK_USAGE              = TOOLBAR_START + 16;
constexpr int TOOLBAR_ID_THIS_PROGRAM            = TOOLBAR_START + 17;

constexpr int VIEW_SUBDIR_START_ID = 1000;
constexpr int VIEW_SUBDIR_START_END = 1500;

constexpr int EXTERNAL_PROGRAM_START_ID = 31000;
constexpr int EXTERNAL_PROGRAM_END_ID   = 31010;

constexpr int wxBACKFORWARD_START = 32000;
constexpr int wxBACKFORWARD_END   = 32500;

typedef struct _EXTERNAL_EDIT_PROGRAM {
	wxString _strPGName = wxT("");
	wxString _strPGPath = wxT("");
	wxString _strPGArgs = wxT("");
} EXTERNAL_EDIT_PROGRAM, *PEXTERNAL_EDIT_PROGRAM;

constexpr int COMPRESS_START_ID     = 32000;
constexpr int COMPRESS_END_ID       = 32100;
constexpr int DECOMPRESS_START_ID   = 32200;
constexpr int DECOMPRESS_END_ID     = 32300;

#endif // DEFS_H_INCLUDED
