#include "../ginc.h"
#include "Memory.h"

wxBEGIN_EVENT_TABLE(CMemory, CSystemInterface)
wxEND_EVENT_TABLE()

CMemory::CMemory()
{
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CMemory::OnTimer));
	m_timer.SetOwner(this, wxTIMER_ID++);
}

CMemory::~CMemory()
{
	Stop();
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(CMemory::OnTimer));
}

void CMemory::Free()
{

}

void CMemory::Init()
{
	Update();
}

void CMemory::OnTimer(wxTimerEvent& event)
{
	Update();
	SendEvent();
}

void CMemory::Update()
{
	wxZeroMemory(_memroyStatus);
	_memroyStatus.dwLength = sizeof(_memroyStatus);
	::GlobalMemoryStatusEx(&_memroyStatus);

	//물리메모리
	_ullTotalPhys  = _memroyStatus.ullTotalPhys / MEGABYTE;
	_ullAvailPhys  = _memroyStatus.ullAvailPhys / MEGABYTE;
	_ullUsedPhys   = (_memroyStatus.ullTotalPhys - _memroyStatus.ullAvailPhys) / MEGABYTE;

	//가상메모리
	_ullTotalVirtual = _memroyStatus.ullTotalVirtual / MEGABYTE;
	_ullAvailVirtual = _memroyStatus.ullAvailVirtual / MEGABYTE;
	_ullUsedVirtual  = (_memroyStatus.ullTotalVirtual - _memroyStatus.ullAvailVirtual) / MEGABYTE;

	//페이지파일
	_ullTotalPageFile = _memroyStatus.ullTotalPageFile / MEGABYTE;
	_ullAvailPageFile = _memroyStatus.ullAvailPageFile / MEGABYTE;
	_ullUsedPageFile  = (_memroyStatus.ullTotalPageFile - _memroyStatus.ullAvailPageFile) / MEGABYTE;
}
