#include "../ginc.h"
#include "../system/Sysinc.h"
#include "../dialog/DlgSystemResource.h"
#include "NextMCPUSegView.h"

wxBEGIN_EVENT_TABLE(CNextMCPUSegView, wxWindow)
	EVT_PAINT(CNextMCPUSegView::OnPaint)
	EVT_ERASE_BACKGROUND(CNextMCPUSegView::OnErase)
	EVT_SIZE(CNextMCPUSegView::OnSize)
	EVT_MY_CUSTOM_COMMAND(wxEVT_RESOURCE_MONITORING, wxID_ANY, CNextMCPUSegView::UpdateListener)
wxEND_EVENT_TABLE()

CNextMCPUSegView::CNextMCPUSegView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle)
	: wxWindow(parent, nID, wxDefaultPosition, sz, lStyle)
{
}

CNextMCPUSegView::~CNextMCPUSegView()
{
}

void CNextMCPUSegView::OnErase(wxEraseEvent& event)
{

}

void CNextMCPUSegView::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	m_memDC.ChangeViewSize(size);

    theDCUtil->Refresh(this, m_viewRect);
}

void CNextMCPUSegView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

    m_viewRect = GetClientRect();

	int w = m_viewRect.GetWidth();
	int h = m_viewRect.GetHeight();

	wxMemoryDC* pMemDC = m_memDC.CreateMemoryDC(&dc, m_viewRect, wxColour(30, 30, 30), wxColour(30, 30, 30));

	Render(pMemDC);
	dc.Blit(0, 0, w, h, pMemDC, 0 ,0);
}

void CNextMCPUSegView::Render(wxDC* pDC)
{
	if(vecRects.size() == 0)
		CreateSegmentArea(m_viewRect, m_segCount);

	SeperateDigit((int)m_ulUsage);

	int red = (255 * (int)m_ulUsage) / 100;
	int green = red == 0 ? 255 : 255 - red;

	wxColour col = wxColour(red, green, 0);

	int numbers = 0;
	int values[m_segCount]{m_hunders, m_tens, m_ones};
	for(auto& s : vecRects)
	{
		s.DrawSeventSegment(pDC, values[numbers], col);
		numbers++;
	}
}

void CNextMCPUSegView::SeperateDigit(int val)
{
	m_hunders = val / 100;
	m_tens = (val / 10) % 10;
	m_ones = val % 10;
}

void CNextMCPUSegView::CreateSegmentArea(const wxRect& rc, int segCount)
{
	wxRect rcSeg{rc};
	rcSeg.Deflate(5);

	int index = 0;
	int segWidth = rcSeg.GetWidth() / segCount;

	wxRect rcHundred;
	rcHundred.SetLeft(rcSeg.GetLeft() + 5);
	rcHundred.SetTop(rcSeg.GetTop());
	rcHundred.SetRight(rcSeg.GetLeft() + (segWidth * ++index) - 5);
	rcHundred.SetBottom(rcSeg.GetBottom());

	CNextMSevenSegment sdHundred;
	sdHundred.CreateSevenSegment(rcHundred);

	vecRects.emplace_back(sdHundred);

	wxRect rcTens;
	rcTens.SetLeft(rcHundred.GetRight() + 5);
	rcTens.SetTop(rcSeg.GetTop());
	rcTens.SetRight(rcSeg.GetLeft() + (segWidth * ++index) - 5);
	rcTens.SetBottom(rcSeg.GetBottom());

	CNextMSevenSegment sdTens;
	sdTens.CreateSevenSegment(rcTens);

	vecRects.emplace_back(sdTens);

	wxRect rcOnes;
	rcOnes.SetLeft(rcTens.GetRight() + 5);
	rcOnes.SetTop(rcSeg.GetTop());
	rcOnes.SetRight(rcSeg.GetLeft() + (segWidth * ++index) - 5);
	rcOnes.SetBottom(rcSeg.GetBottom());

	CNextMSevenSegment sdOnes;
	sdOnes.CreateSevenSegment(rcOnes);

	vecRects.emplace_back(sdOnes);
}

void CNextMCPUSegView::UpdateListener(wxCommandEvent& event)
{
	m_ulUsage = theSystem->CPU()->GetValue(CPU_TOTAL);

	DlgSystemResource* pDlg = (DlgSystemResource *)this->GetParent();
	pDlg->UpdateCPUUsage(m_ulUsage);

	theDCUtil->Refresh(this, m_viewRect);
}
