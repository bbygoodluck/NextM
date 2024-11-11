#ifndef NEXTMENUMFORMATETC_H_INCLUDED
#define NEXTMENUMFORMATETC_H_INCLUDED

class CNextMEnumFormatEtc : public IEnumFORMATETC
{
public:
	CNextMEnumFormatEtc(FORMATETC* pFormatEtc, int nNumFormats);
	~CNextMEnumFormatEtc();

	//IUnknown members
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject);

	//IEnumFormatEtc members
	HRESULT __stdcall Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched);
	HRESULT __stdcall Skip(ULONG celt);
	HRESULT __stdcall Reset();
	HRESULT __stdcall Clone(IEnumFORMATETC** ppenum);

private:
	void DeepCopyFormatEtc(FORMATETC* dest, FORMATETC* source);

private:
	LONG m_lRefCount;
	ULONG m_nIndex;
	ULONG m_nNumFormats;
	FORMATETC* m_pFormatEtc;
};

#endif // NEXTMENUMFORMATETC_H_INCLUDED
