#ifndef __DLG_DECOMPRESS_H_INCLUDED__
#define __DLG_DECOMPRESS_H_INCLUDED__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/gauge.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DlgDeCompress
///////////////////////////////////////////////////////////////////////////////
class DlgDeCompress : public wxDialog
{
public:
	DlgDeCompress( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = theMsg->GetMessage(wxT("MSG_DECOMPRESS_DLG_CAPTION")), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 662,155 ), long style = wxDEFAULT_DIALOG_STYLE );
	~DlgDeCompress();

public:
	void SetDecompressInfo(const wxString& strDecompressedFile, const wxString& strDecompressDir, int dtype);
	void SetExtractTotal(uint64_t ulTotal);
	void SetExtractCurrent(uint64_t ulCurrent);
	void SetExtractCurrentFile(const wxString& strFileName);

protected:
	wxStaticText* m_staticText1;
	wxStaticText* m_staticSelCompressFile;
	wxStaticText* m_staticText3;
	wxStaticText* m_staticCurrentDecompress;
	wxGauge* m_ExtractProgress;
	wxStaticText* m_staticPercent;
	wxButton* m_btnCancel;

	// Virtual event handlers, overide them in your derived class
	virtual void OnInitDialog( wxInitDialogEvent& event );
	virtual void OnCancel( wxCommandEvent& event );

private:
	wxString m_strDecompressedFile = wxT("");
	wxString m_strDecompressDir = wxT("");

	uint64_t m_ulTotal = 0;

	std::list<wxString> m_vCompressList;
private:
	void OnDeCompressThreadEnd(wxThreadEvent& event);

};

#endif
