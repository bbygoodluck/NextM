#include "../ginc.h"
#include "MemoryDC.h"

CMemoryDC::CMemoryDC()
{

}

CMemoryDC::~CMemoryDC()
{
	if(m_pDoubleBuffer)
		delete m_pDoubleBuffer;

	m_pDoubleBuffer = nullptr;
}

void CMemoryDC::CreateMemoryBuffer(wxDC* pDC)
{
	if(m_pMemDC == nullptr)
	{
		m_pMemDC = std::make_unique<wxMemoryDC>(pDC);
		ChangeViewSize(wxSize(m_rc.GetWidth(), m_rc.GetHeight()));
	}
}

wxMemoryDC* CMemoryDC::SelectObjectOnly(wxDC* pDC, const wxRect& rc)
{
	m_rc = rc;
	CreateMemoryBuffer(pDC);

	DetachDC();

	m_pMemDC->SelectObject(*m_pDoubleBuffer);
	return m_pMemDC.get();
}

wxMemoryDC* CMemoryDC::CreateMemoryDC(wxDC* pDC, const wxRect& rc, const wxColour& colPen, const wxColour& colBrush)
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

wxMemoryDC* CMemoryDC::CreateRoundRectMemoryDC(wxDC* pDC, const wxRect& rc, double dblradius, const wxColour& colPen, const wxColour& colBrush)
{
	m_rc = rc;
	CreateMemoryBuffer(pDC);

	DetachDC();

	if(m_pDoubleBuffer)
	{
		m_pMemDC->SelectObject(*m_pDoubleBuffer);
		m_pMemDC->SetPen(colPen);
		m_pMemDC->SetBrush(colBrush);
		m_pMemDC->DrawRoundedRectangle(rc, dblradius);
	}

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
	wxSize szTmp(sz);
	int width = szTmp.GetWidth();
	int height = szTmp.GetHeight();

	if ((width == 0) && (height == 0))
		return;

	int oldWidth = m_szChagned.GetWidth();
	int oldHeight = m_szChagned.GetHeight();

//	if (m_szChagned.x != sz.x)
	if ((oldWidth != width) || (oldHeight != height))
	{
		m_szChagned = szTmp;
		if (m_pDoubleBuffer)
			delete m_pDoubleBuffer;

		int newWidth = width == 0 ? 10 : width;
		int newHeight = height == 0 ? 10 : height;

		m_pDoubleBuffer = new wxBitmap(newWidth, newHeight);
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
