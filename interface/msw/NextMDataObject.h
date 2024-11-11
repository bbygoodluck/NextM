#ifndef NEXTMDATAOBJECT_H_INCLUDED
#define NEXTMDATAOBJECT_H_INCLUDED

class CNextMDataObject : public IDataObject
{
public:
	CNextMDataObject(FORMATETC* fmtect, STGMEDIUM* stgmed, int count);
	~CNextMDataObject();

	//IUnknown members
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	//IDataObject memebers
	HRESULT __stdcall GetData(FORMATETC* pFormatEtc, STGMEDIUM* pmedium);
	HRESULT __stdcall GetDataHere(FORMATETC* pFormatEtc, STGMEDIUM* pmedium);
	HRESULT __stdcall QueryGetData(FORMATETC* pformatetc);
	HRESULT __stdcall GetCanonicalFormatEtc(FORMATETC* pformatectIn, FORMATETC* pformatetcOut);
	HRESULT __stdcall SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, WINBOOL fRelease);
	HRESULT __stdcall EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc);
	HRESULT __stdcall DAdvise(FORMATETC* pformatetc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection);
	HRESULT __stdcall DUnadvise(DWORD dwConnection);
	HRESULT __stdcall EnumDAdvise(IEnumSTATDATA** ppenumAdvise);

	HRESULT SetDropDescription(DROPIMAGETYPE image, LPCTSTR format, LPCTSTR insert);

private:
	HGLOBAL DumpMem(HGLOBAL hGlobal);
	void CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc);

private:
	LONG m_lRefCount;
	int LookupFormatEtc(FORMATETC* pFormatEtc);

	FORMATETC* m_pFormatEtc = nullptr;
	STGMEDIUM* m_pStgMedium = nullptr;

	LONG       m_nNumformats;
};

#endif // NEXTMDATAOBJECT_H_INCLUDED
