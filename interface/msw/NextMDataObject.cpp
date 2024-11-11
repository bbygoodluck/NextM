#include "../../ginc.h"
#include "NextMDataObject.h"

#include <Strsafe.h>

CNextMDataObject::CNextMDataObject(FORMATETC* fmtect, STGMEDIUM* stgmed, int count)
	: m_lRefCount(0)
	, m_nNumformats(count)
{
	m_pFormatEtc = new FORMATETC[count];
	m_pStgMedium = new STGMEDIUM[count];

	for(int i = 0 ; i < count; i++)
	{
		m_pFormatEtc[i] = fmtect[i];
		m_pStgMedium[i] = stgmed[i];
	}
}

CNextMDataObject::~CNextMDataObject()
{
	if(m_pFormatEtc) delete[] m_pFormatEtc;
	if(m_pStgMedium) delete[] m_pStgMedium;
}

//IUnknown members
ULONG CNextMDataObject::AddRef()
{
	return InterlockedIncrement(&m_lRefCount);
}

ULONG CNextMDataObject::Release()
{
	LONG count = InterlockedDecrement(&m_lRefCount);
	if(count == 0)
	{
		delete this;
		return 0;
	}

	return count;
}

HRESULT CNextMDataObject::QueryInterface(REFIID riid, void** ppvObject)
{
	if(IsEqualIID(riid, IID_IDataObject) || IsEqualIID(riid, IID_IUnknown))
		*ppvObject = this;
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

//IDataObject memebers
HRESULT CNextMDataObject::GetData(FORMATETC* pFormatEtc, STGMEDIUM* pmedium)
{
	int idx;
	if( (idx = LookupFormatEtc(pFormatEtc)) == -1)
		return DV_E_FORMATETC;

	pmedium->tymed = m_pFormatEtc[idx].tymed;
	pmedium->pUnkForRelease = 0;

	switch(m_pFormatEtc[idx].tymed)
	{
		case TYMED_HGLOBAL:
			pmedium->hGlobal = DumpMem(m_pStgMedium[idx].hGlobal);
			break;

		default:
			return DV_E_FORMATETC;
	}

	return S_OK;
}

HRESULT CNextMDataObject::GetDataHere(FORMATETC* pFormatEtc, STGMEDIUM* pmedium)
{
	return DATA_E_FORMATETC;
}

HRESULT CNextMDataObject::QueryGetData(FORMATETC *pformatetc)
{
	if(pformatetc == NULL)
		return E_INVALIDARG;

	if (!(DVASPECT_CONTENT & pformatetc->dwAspect))
		return (DV_E_DVASPECT);

	HRESULT  hr = DV_E_TYMED;

	for(int i = 0; i < m_nNumformats; i++)
	{
		if(pformatetc->tymed & m_pFormatEtc[i].tymed)
		{
			if(pformatetc->cfFormat == m_pFormatEtc[i].cfFormat)
				return S_OK;
			else
				hr = DV_E_CLIPFORMAT;
		}
		else
			hr = DV_E_TYMED;
	}

	return hr;

//	return (LookupFormatEtc(pformatetc) == -1) ? DV_E_FORMATETC : S_OK;
}

HRESULT CNextMDataObject::GetCanonicalFormatEtc(FORMATETC* pformatectIn, FORMATETC* pformatetcOut)
{
	pformatetcOut->ptd = nullptr;
	return E_NOTIMPL;
}

HRESULT CNextMDataObject::SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, WINBOOL fRelease)
{
	if(pformatetc == NULL || pmedium == NULL)
		return E_INVALIDARG;

	assert(pformatetc->tymed == pmedium->tymed);

	FORMATETC* fetc = new (std::nothrow) FORMATETC;
	STGMEDIUM* pStgMed = new (std::nothrow) STGMEDIUM;

	if(fetc == NULL || pStgMed == NULL)
	{
		delete fetc;
		delete pStgMed;
		return E_OUTOFMEMORY;
	}

	SecureZeroMemory(fetc, sizeof(FORMATETC));
	SecureZeroMemory(pStgMed, sizeof(STGMEDIUM));

	int indx = LookupFormatEtc(pformatetc);

	if(indx < 0)
		return E_NOTIMPL;

	*fetc = *pformatetc;

	if(fRelease)
		*pStgMed = *pmedium;
	else
	{
		CopyMedium(pStgMed, pmedium, pformatetc);
	}

	m_pStgMedium[indx] = *pStgMed;

	return E_NOTIMPL;
}

HRESULT CNextMDataObject::SetDropDescription(DROPIMAGETYPE image, LPCTSTR format, LPCTSTR insert)
{
	if(format == NULL || insert == NULL)
		return E_INVALIDARG;

	FORMATETC fetc = {0};
	fetc.cfFormat = (CLIPFORMAT)RegisterClipboardFormat(CFSTR_DROPDESCRIPTION);
	fetc.dwAspect = DVASPECT_CONTENT;
	fetc.lindex = -1;
	fetc.tymed = TYMED_HGLOBAL;

	STGMEDIUM medium = {0};
	medium.tymed = TYMED_HGLOBAL;
	medium.hGlobal = GlobalAlloc(GHND, sizeof(DROPDESCRIPTION));

	if(medium.hGlobal == 0)
		return E_OUTOFMEMORY;

	DROPDESCRIPTION* pDropDescription = (DROPDESCRIPTION*)GlobalLock(medium.hGlobal);

	StringCchCopy(pDropDescription->szInsert, _countof(pDropDescription->szInsert), insert);
	StringCchCopy(pDropDescription->szMessage, _countof(pDropDescription->szMessage), format);
	pDropDescription->type = image;

	GlobalUnlock(medium.hGlobal);

	return SetData(&fetc, &medium, TRUE);
}

void CNextMDataObject::CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc)
{
	switch(pMedSrc->tymed)
	{
		case TYMED_HGLOBAL:
			pMedDest->hGlobal = (HGLOBAL)OleDuplicateData(pMedSrc->hGlobal,pFmtSrc->cfFormat, NULL);
			break;

		case TYMED_GDI:
			pMedDest->hBitmap = (HBITMAP)OleDuplicateData(pMedSrc->hBitmap,pFmtSrc->cfFormat, NULL);
			break;

		case TYMED_MFPICT:
			pMedDest->hMetaFilePict = (HMETAFILEPICT)OleDuplicateData(pMedSrc->hMetaFilePict,pFmtSrc->cfFormat, NULL);
			break;

		case TYMED_ENHMF:
			pMedDest->hEnhMetaFile = (HENHMETAFILE)OleDuplicateData(pMedSrc->hEnhMetaFile,pFmtSrc->cfFormat, NULL);
			break;

		case TYMED_FILE:
			pMedSrc->lpszFileName = (LPOLESTR)OleDuplicateData(pMedSrc->lpszFileName,pFmtSrc->cfFormat, NULL);
			break;

		case TYMED_ISTREAM:
			pMedDest->pstm = pMedSrc->pstm;
			pMedSrc->pstm->AddRef();
			break;

		case TYMED_ISTORAGE:
			pMedDest->pstg = pMedSrc->pstg;
			pMedSrc->pstg->AddRef();
			break;

		case TYMED_NULL:
		default:
			break;
	}

	pMedDest->tymed = pMedSrc->tymed;
	pMedDest->pUnkForRelease = NULL;
	if(pMedSrc->pUnkForRelease != NULL)
	{
		pMedDest->pUnkForRelease = pMedSrc->pUnkForRelease;
		pMedSrc->pUnkForRelease->AddRef();
	}
}

HRESULT CNextMDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc)
{
	if(dwDirection == DATADIR_GET)
		return theDnD->CreateEnumFormatEtc(m_nNumformats, m_pFormatEtc, ppenumFormatEtc);

	return E_NOTIMPL;
}

HRESULT CNextMDataObject::DAdvise(FORMATETC* pformatetc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT CNextMDataObject::DUnadvise(DWORD dwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT CNextMDataObject::EnumDAdvise(IEnumSTATDATA** ppenumAdvise)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

int CNextMDataObject::LookupFormatEtc(FORMATETC* pFormatEtc)
{
	for(int i = 0; i < m_nNumformats; i++)
	{
		if( (pFormatEtc->tymed & m_pFormatEtc[i].tymed) &&
			(pFormatEtc->cfFormat == m_pFormatEtc[i].cfFormat) &&
			(pFormatEtc->dwAspect == m_pFormatEtc[i].dwAspect) )
		{
			return i;
		}
	}

	return -1;
}

HGLOBAL CNextMDataObject::DumpMem(HGLOBAL hGlobal)
{
	DWORD len = GlobalSize(hGlobal);
	PVOID source = GlobalLock(hGlobal);

	PVOID dest = GlobalAlloc(GMEM_FIXED, len);
	memcpy(dest, source, len);

	GlobalUnlock(hGlobal);

	return dest;
}
