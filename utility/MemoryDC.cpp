#include "../ginc.h"
#include "MemoryDC.h"

CMemoryDC::CMemoryDC()
{

}

CMemoryDC::~CMemoryDC()
{

}

void CMemoryDC::CreateMemoryBuffer(wxDC* pDC)
{
	if(m_pMemDC == nullptr)
	{
		m_pMemDC = std::make_unique<wxMemoryDC>(pDC);
		ChangeViewSize(wxSize(m_rc.GetWidth(), m_rc.GetHeight()));
	}
}

wxMemoryDC* CMemoryDC::CreateMemoryDC(wxDC* pDC, const wxRect& rc, const wxColour& colPen, const wxColour colBrush)
{
	m_rc = rc;
	CreateMemoryBuffer(pDC);

	DetachDC();

	m_pMemDC->SelectObject(*m_pDoubleBuffer);
    m_pMemDC->SetPen(colPen);
	m_pMemDC->SetBrush(colBrush);
	m_pMemDC->DrawRectangle(rc);

	return m_pMemDC.get();
}

wxMemoryDC* CMemoryDC::CreateRoundRectMemoryDC(wxDC* pDC, const wxRect& rc, double dblradius, const wxColour& colPen, const wxColour colBrush)
{
	CreateMemoryBuffer(pDC);

	DetachDC();

	m_pMemDC->SelectObject(*m_pDoubleBuffer);
    m_pMemDC->SetPen(colPen);
	m_pMemDC->SetBrush(colBrush);
	m_pMemDC->DrawRoundedRectangle(rc, dblradius);

	return m_pMemDC.get();
}

bool CMemoryDC::IsCreatedMemDC()
{
	if (!m_pDoubleBuffer->IsOk() || m_pDoubleBuffer == nullptr)
		return false;

	return true;
}

unsigned int CMemoryDC::GetWidth()
{
	if(IsCreatedMemDC())
		return 0;

	return m_pDoubleBuffer->GetWidth();
}

unsigned int CMemoryDC::GetHeight()
{
	if(IsCreatedMemDC())
		return 0;

	return m_pDoubleBuffer->GetHeight();
}

void CMemoryDC::ChangeViewSize(const wxSize& sz)
{
	if ((sz.x == 0) || (sz.y == 0))
		return;

	if (m_szChagned.x != sz.x)
	{
		m_szChagned = sz;
		if (m_pDoubleBuffer)
			delete m_pDoubleBuffer;

		m_pDoubleBuffer = new wxBitmap(m_szChagned.x, m_szChagned.y);
	}
}

void CMemoryDC::DetachDC()
{
	ClearMemoryDC();

	m_pMemDC->SetPen(wxNullPen);
	m_pMemDC->SetBrush(wxNullBrush);
	m_pMemDC->SetFont(wxNullFont);
	m_pMemDC->SelectObject(wxNullBitmap);
}

void CMemoryDC::ClearMemoryDC()
{
	m_pMemDC->Clear();
}

void CMemoryDC::ClearMemoryDC(wxMemoryDC* pMemDC)
{
	pMemDC->Clear();
}
