#include <wx/busyinfo.h>

#include "../ginc.h"
#include "DlgSystemResource.h"
#include "../system/Sysinc.h"
#include "../views/NextMGraphView.h"
#include "../views/NextMMemView.h"
#include "../views/NextMNetView.h"
#include "../views/NextMCPUSegView.h"
#include "../views/NextMProcessView.h"

#ifdef __WXMSW__
#include "../system/msw/NextMWMI.h"
#endif // __WXMSW__

DlgSystemResource::DlgSystemResource( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	SetIcon(wxIcon("wxwin"));
	CenterOnScreen();

	wxBusyInfo info
    (
        wxBusyInfoFlags()
            .Parent(nullptr)
            .Icon(wxArtProvider::GetIcon(wxART_INFORMATION,
                                         wxART_OTHER, wxSize(64, 64)))
            .Title("<b>Initialize System resource...</b>")
            .Text("please wait...")
            .Foreground(*wxWHITE)
            .Background(wxColour(100, 100, 100))
            .Transparency(5 * (wxALPHA_OPAQUE / 5))
    );

	const int cpu_mem_height = 180;
	//시스템자원 초기화
	InitializeResource();

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	//윈도우 사이즈 변경
	wxSize szMainFrame = _gMainFrame->GetSize();
	wxSize szThisDlg(szMainFrame.GetX() - 50, szMainFrame.GetY() - 50);

	this->SetSize(szThisDlg);

	wxBoxSizer* bSizer_Main;
	bSizer_Main = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer_Resource;
	bSizer_Resource = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer_CPU;
	bSizer_CPU = new wxBoxSizer( wxHORIZONTAL );

//	wxStaticBoxSizer* sbSizer_CPU;
	sbSizer_CPU = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("CPU - Total") ), wxHORIZONTAL );

//	m_wndCPU = new wxPanel( sbSizer_CPU->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 300,200 ), wxTAB_TRAVERSAL );
	m_pCPUSegView = new CNextMCPUSegView(this, wxID_ANY, wxSize( 300, cpu_mem_height ));
//	m_pCPUSegView->SetIndex(0);

	theSystem->CPU()->AddEventListener((wxWindow *)m_pCPUSegView);

	sbSizer_CPU->Add( m_pCPUSegView, 1, wxEXPAND | wxALL, 5 );


	bSizer_CPU->Add( sbSizer_CPU, 0, wxALL|wxEXPAND, 5 );

	sbSizer_core = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("CPU - Core") ), wxVERTICAL );

	wxBoxSizer* bSizerCore;
	bSizerCore = new wxBoxSizer( wxVERTICAL );

	CreateCPUCoreList(bSizerCore);

	sbSizer_core->Add( bSizerCore, 1, wxEXPAND, 5 );

	bSizer_CPU->Add( sbSizer_core, 1, wxALL|wxEXPAND, 5 );

	bSizer_Resource->Add( bSizer_CPU, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer_Mem_Net;
	bSizer_Mem_Net = new wxBoxSizer( wxHORIZONTAL );

	sbSizer_Mem = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Memory") ), wxHORIZONTAL );

	m_pMEMView = new CNextMMemView( this, wxID_ANY, wxSize( 300, cpu_mem_height + 40 ));
	sbSizer_Mem->Add( m_pMEMView, 1, wxEXPAND | wxALL, 5 );

//	theSystem->MEM()->AddEventListener((wxWindow *)m_pMEMView);

	bSizer_Mem_Net->Add( sbSizer_Mem, 0, wxALL|wxEXPAND, 5 );

//	bSizer_Resource->Add( bSizer_CPU_MEM, 0, wxEXPAND, 5 );

	sbSizer_Net = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Network") ), wxVERTICAL );

	std::vector<wxString> intefaces = theSystem->NET()->GetInterfaces();

	m_cmbInterface = new wxComboBox(this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxSize(300, -1), intefaces, wxCB_DROPDOWN|wxCB_READONLY );
	m_cmbInterface->SetSelection(0);

	sbSizer_Net->Add( m_cmbInterface, 0, wxALL, 5 );

	m_pNetView = new CNextMNetView(this, wxID_ANY, wxDefaultSize);
	theSystem->NET()->AddEventListener((wxWindow *)m_pNetView);

	sbSizer_Net->Add( m_pNetView, 1, wxEXPAND | wxALL, 5 );

	bSizer_Mem_Net->Add( sbSizer_Net, 1, wxALL|wxEXPAND, 5 );

	bSizer_Resource->Add( bSizer_Mem_Net, 1, wxEXPAND, 5 );

	bSizer_Main->Add( bSizer_Resource, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

//	wxBoxSizer* bSizer_MEM_NET;
//	bSizer_MEM_NET = new wxBoxSizer( wxHORIZONTAL );

//	bSizer_Main->Add( bSizer_Resource, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxStaticBoxSizer* sbSizer_Process;
	sbSizer_Process = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Process") ), wxVERTICAL );

	m_lstCtrlProcess = new CNextMProcessView(this, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxBORDER_NONE);
	sbSizer_Process->Add( m_lstCtrlProcess, 1, wxALL|wxEXPAND, 5 );

	theSystem->PROCESS()->AddEventListener((wxWindow *)m_lstCtrlProcess);

	bSizer_Main->Add( sbSizer_Process, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer_Button;
	bSizer_Button = new wxBoxSizer( wxVERTICAL );

	m_btnClose = new wxButton( this, wxID_ANY, wxT("닫기"), wxDefaultPosition, wxSize(-1, 30), 0 );
	m_btnClose->SetBitmap( wxArtProvider::GetBitmap( wxASCII_STR(wxART_CLOSE), wxASCII_STR(wxART_BUTTON) ) );

	bSizer_Button->Add( m_btnClose, 0, wxALL, 5 );

	bSizer_Main->Add( bSizer_Button, 0, wxALIGN_RIGHT, 5 );

	this->SetSizer( bSizer_Main );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgSystemResource::OnInitDialog ) );
	m_btnClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgSystemResource::OnClickClose ), NULL, this );
	m_cmbInterface->Connect( wxEVT_COMBOBOX, wxCommandEventHandler( DlgSystemResource::OnComboSelected ), NULL, this );


	UpdateNETInfo(0);
#ifdef __WXMSW__
	//프로세스 생성/삭제 이벤트 등록
	CNextMWMI* pWMI = theSystem->PROCESS()->GetWMI();
	pWMI->RegisterEventWindow((wxWindow *)m_lstCtrlProcess);
#endif // __WXMSW__

	theSystem->CPU()->Start(1200);
	theSystem->NET()->Start(1100);
}

DlgSystemResource::~DlgSystemResource()
{
	//cpu info clear
	theSystem->CPU()->Stop();
	theSystem->CPU()->Clear();

	//Network Clear
	theSystem->NET()->Stop();
	theSystem->NET()->Clear();

	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgSystemResource::OnInitDialog ) );
	m_btnClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgSystemResource::OnClickClose ), NULL, this );
	m_cmbInterface->Disconnect( wxEVT_COMBOBOX, wxCommandEventHandler( DlgSystemResource::OnComboSelected ), NULL, this );
}

void DlgSystemResource::InitializeResource()
{
	//시스템 자원 초기화
	//CPU초기화
	theSystem->CPU()->Init();

	//메모리 초기화
	theSystem->MEM()->Init();

	//네트워크 정보 초기화
	theSystem->NET()->Init();

	//프로세스 정보 초기화
	theSystem->PROCESS()->Init();
}

void DlgSystemResource::OnInitDialog( wxInitDialogEvent& event )
{
	wxString strProcessorName = theSystem->CPU()->GetProcessorName();
	this->SetTitle(wxT("System Resource - ") + strProcessorName);
}

void DlgSystemResource::UpdateMemory(unsigned long total, unsigned long used)
{
	float fTotalGB = (float)total / 1024.0f;
	float fUsedGB = (float)used / 1024.0f;

	unsigned int usedPercent = (100 * used) / total;
	wxString strMem = wxString::Format(wxT("Memory - %3.2fGB / %3.2fGB (%02d%)"), fUsedGB, fTotalGB, usedPercent);

	wxStaticBox *staticBox = sbSizer_Mem->GetStaticBox();

	int iRed;
	int iGreen;

	GetColorForUsage(&iRed, &iGreen, usedPercent);

//	staticBox->SetForegroundColour(wxColour(iRed, iGreen, 0));
	staticBox->SetLabel(strMem);
}

void DlgSystemResource::UpdateCPUUsage(unsigned int usage)
{
	wxStaticBox* cpuStaticBox = sbSizer_CPU->GetStaticBox();
	wxString strUsage(wxString::Format(wxT("CPU - %d%"), usage));

	cpuStaticBox->SetLabel(strUsage);
}

void DlgSystemResource::GetColorForUsage(int* red, int* green, unsigned int value)
{
	*red = (255 * value) / 100;
	*green = *red == 0 ? 255 : 255 - *red;
}

void DlgSystemResource::UpdateNETInfo(unsigned int index)
{
	wxStaticBox *netStaticBox = sbSizer_Net->GetStaticBox();
	wxString strInterfaceInfo = theSystem->NET()->GetInterfaceInfo(index);

	netStaticBox->SetLabel(wxT("Network - ") + strInterfaceInfo);
}

void DlgSystemResource::CreateCPUCoreList(wxSizer* pSizer)
{
	size_t nCoreCount = theSystem->CPU()->GetCoreCount();

	unsigned int nDispViewPerRow = nCoreCount / 2;
	unsigned int nTotalRow = nCoreCount / nDispViewPerRow;

	unsigned int corenum = 0;
	for(unsigned int indx = 0; indx < nTotalRow; indx++)
	{
		wxBoxSizer* coreSizer = new wxBoxSizer( wxHORIZONTAL );
		for(unsigned int rowindex = 0; rowindex < nDispViewPerRow; rowindex++)
		{
			CNextMGraphView *pCPUGraphView = new CNextMGraphView(this, wxID_ANY, wxDefaultSize);
			pCPUGraphView->SetCoreIndex(corenum++);

			coreSizer->Add( pCPUGraphView, 1, wxEXPAND | wxALL, 5 );
			theSystem->CPU()->AddEventListener(pCPUGraphView);
		}

		pSizer->Add( coreSizer, 1, wxEXPAND, 5 );
	}

	wxStaticBox* cpuCoreStaticBox = sbSizer_core->GetStaticBox();
	wxString strCoreCount{wxString::Format(wxT("CPU - %d Core"), (int)nCoreCount)};

	cpuCoreStaticBox->SetLabel(strCoreCount);
}

void DlgSystemResource::OnClickClose( wxCommandEvent& event )
{
	EndDialog(wxID_CLOSE);
}

void DlgSystemResource::OnComboSelected( wxCommandEvent& event )
{
	int iSel = event.GetSelection();
	UpdateNETInfo(iSel);

	theSystem->NET()->SetSelectAdapter(iSel);
}
