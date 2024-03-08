#ifndef __VERSION_H__
#define __VERSION_H__

/********************************************************************************
	class Name :
	설      명 : 버전정보 표시
*********************************************************************************/
#ifdef _WIN64
	const wxString ENV_OS_VER = wxT(" - x64");
#else
	const wxString ENV_OS_VER = wxT(" - x86");
#endif

const wxString PROGRAM_NAME			= wxT("NextM");
const wxString PROGRAM_MAJOR_VER	= wxT("1");
const wxString PROGRAM_MINOR_HI_VER = wxT("0");
const wxString PROGRAM_MINOR_LO_VER = wxT("1");
const wxString PROGRAM_VERSION		= wxT(" III Ver ") + PROGRAM_MAJOR_VER + wxT(".") + PROGRAM_MINOR_HI_VER + wxT(".") + PROGRAM_MINOR_LO_VER;
const wxString PROGRAM_FULL_NAME	= PROGRAM_NAME + PROGRAM_VERSION + ENV_OS_VER;


#endif
