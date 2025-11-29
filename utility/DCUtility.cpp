#include "../ginc.h"
#include "DCUtility.h"

std::unique_ptr<CDCUtility> CDCUtility::m_pInstance(nullptr);

CDCUtility* CDCUtility::Get()
{
	if (m_pInstance == nullptr)
		m_pInstance.reset(new CDCUtility());

	return m_pInstance.get();
}

void CDCUtility::Refresh(wxWindow* pWindow, const wxRect& rect, bool bUpdate)
{
#ifdef __WXMSW__
	HWND hWnd = pWindow->GetHWND();
	if (rect.GetWidth() != 0)
	{
		RECT mswRect;
		const RECT* pRect;

		wxCopyRectToRECT(rect, mswRect);
		pRect = &mswRect;

		//	INT flags = RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW | RDW_ERASE;
		//	::RedrawWindow(hWnd, pRect, NULL, flags);
		::InvalidateRect(hWnd, pRect, false);
	}
	else
		pWindow->Refresh(false);

	if (bUpdate)
		pWindow->Update();
#else
	pWindow->RefreshRect(rect, false);
	pWindow->Update();
#endif
}

wxString CDCUtility::EllipseStr(wxDC* pDC, const wxString& strSrc, int destW)
{
	wxString strName(strSrc);
	wxString strDisp(strSrc);

	wxSize szName = pDC->GetTextExtent(strName);
	int srcW = szName.GetWidth();

	int iLen = strName.Len();

	if (srcW > destW)
	{
		strDisp.clear();

		for (int iIndex = 0; iIndex < iLen; iIndex++)
		{
			strDisp = strName.Left(iIndex + 1);
			wxSize sizeText = pDC->GetTextExtent(wxString(strDisp + wxT("...")));

			if ((sizeText.GetWidth()) > destW)
			{
				strDisp = strName.Left(iIndex);
				break;
			}
		}

		strDisp += wxT("...");
	}

	return strDisp;
}
