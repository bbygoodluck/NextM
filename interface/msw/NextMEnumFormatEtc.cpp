#include "../../ginc.h"
#include "NextMEnumFormatEtc.h"

CNextMEnumFormatEtc::CNextMEnumFormatEtc(FORMATETC* pFormatEtc, int nNumFormats)
{
	m_lRefCount = 1;
	m_nIndex = 0;
	m_nNumFormats = nNumFormats;
	m_pFormatEtc = new FORMATETC[nNumFormats];

	for(int i = 0; i < nNumFormats; i++)
		DeepCopyFormatEtc(&m_pFormatEtc[i], &pFormatEtc[i]);
}

CNextMEnumFormatEtc::~CNextMEnumFormatEtc()
{
	if(m_pFormatEtc)
	{
		for(ULONG i = 0; i < m_nNumFormats; i++)
		{
			if(m_pFormatEtc[i].ptd)
				CoTaskMemFree(m_pFormatEtc[i].ptd);
		}

		delete[] m_pFormatEtc;
	}
}

//IUnknown members
ULONG CNextMEnumFormatEtc::AddRef(void)
{
	return InterlockedIncrement(&m_lRefCount);
}

ULONG CNextMEnumFormatEtc::Release(void)
{
	LONG count = InterlockedDecrement(&m_lRefCount);
	if(count == 0)
	{
		delete this;
		return 0;
	}

	return count;
}

HRESULT CNextMEnumFormatEtc::QueryInterface(REFIID riid, void** ppvObject)
{
	if(riid == IID_IEnumFORMATETC || riid == IID_IUnknown)
	{
		AddRef();
		*ppvObject = this;
		return S_OK;
	}

	*ppvObject = 0;
	return E_NOINTERFACE;
}

//IEnumFormatEtc members
HRESULT CNextMEnumFormatEtc::Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched)
{
	ULONG copied = 0;
	if(celt == 0 || rgelt == 0)
		return E_INVALIDARG;

	while(m_nIndex < m_nNumFormats && copied < celt)
	{
		DeepCopyFormatEtc(&rgelt[copied], &m_pFormatEtc[m_nIndex]);
		copied++;
		m_nIndex++;
	}

	if(pceltFetched != 0)
		*pceltFetched = copied;


	return (copied == celt) ? S_OK : S_FALSE;
}

HRESULT CNextMEnumFormatEtc::Skip(ULONG celt)
{
	m_nIndex += celt;
	return (m_nIndex < m_nNumFormats) ? S_OK : S_FALSE;
}

HRESULT CNextMEnumFormatEtc::Reset()
{
	m_nIndex = 0;
	return S_OK;
}

HRESULT CNextMEnumFormatEtc::Clone(IEnumFORMATETC** ppenum)
{
	HRESULT hResult;
	hResult = theDnD->CreateEnumFormatEtc(m_nNumFormats, m_pFormatEtc, ppenum);
	if(hResult == S_OK)
		((CNextMEnumFormatEtc *)*ppenum)->m_nIndex = m_nIndex;

	return hResult;
}

void CNextMEnumFormatEtc::DeepCopyFormatEtc(FORMATETC* dest, FORMATETC* source)
{
	*dest = *source;
	if(source->ptd)
	{
		dest->ptd = (DVTARGETDEVICE *)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
		*(dest->ptd) = *(source->ptd);
	}
}
