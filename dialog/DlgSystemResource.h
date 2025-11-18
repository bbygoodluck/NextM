#ifndef DLGSYSTEMRESOURCE_H_INCLUDED
#define DLGSYSTEMRESOURCE_H_INCLUDED

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/dialog.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>

class CNextMGraphView;
class CNextMMemView;
class CNextMNetView;
class CNextMCPUSegView;
class CNextMProcessView;
///////////////////////////////////////////////////////////////////////////////
/// Class DlgSystemResource
///////////////////////////////////////////////////////////////////////////////
class DlgSystemResource : public wxDialog
{
public:
	DlgSystemResource( wxWindow* parent,
					   wxWindowID id = wxID_ANY,
					   const wxString& title = wxT("System Resource - cpu info"),
					   const wxPoint& pos = wxDefaultPosition,
					   const wxSize& size = wxSize( 1103,718 ),
					   long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

	~DlgSystemResource();

public:
	void UpdateMemory(unsigned long total, unsigned long used);
	void UpdateCPUUsage(unsigned int usage);
	void UpdateNETInfo(unsigned int index);

private:
	void GetColorForUsage(int* red, int* green, unsigned int value);
	void CreateCPUCoreList(wxSizer* pSizer);
	void InitializeResource();

protected:
	virtual void OnInitDialog( wxInitDialogEvent& event );
	virtual void OnClickClose( wxCommandEvent& event );
	virtual void OnComboSelected( wxCommandEvent& event );

protected:
	wxButton* m_btnClose;
	CNextMProcessView* m_lstCtrlProcess;

	wxStaticBoxSizer* sbSizer_Mem;
	wxStaticBoxSizer* sbSizer_CPU;
	wxStaticBoxSizer* sbSizer_Net;
	wxStaticBoxSizer* sbSizer_core;

	CNextMGraphView*   m_pCPUGraphView = nullptr;
	CNextMMemView*    m_pMEMView       = nullptr;
	CNextMNetView*    m_pNetView       = nullptr;
	CNextMCPUSegView* m_pCPUSegView    = nullptr;

	wxComboBox* m_cmbInterface;
};


#endif // DLGSYSTEMRESOURCE_H_INCLUDED
