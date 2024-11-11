#ifndef IMAGELIST_H_INCLUDED
#define IMAGELIST_H_INCLUDED

class CImageList
{
private:
	CImageList();

public:
	~CImageList();
	static CImageList *Get();

	bool Create(int width, int height, int flags);
	bool Draw(int index, wxDC* pDC, int x, int y, int flags);
	void LoadImageList();
	void GetIconIndex(const wxString& strPath, int &nIconIndex, int &nOverlayIndex, bool bExtFind = false);  //아이콘 인덱스

#ifdef __WXMSW__
	void Attach(HIMAGELIST hImageList);
#else
	void Attach(wxImageList* pImageList);
#endif

private:
#ifdef __WXMSW__
	HIMAGELIST m_hImageList = NULL;
#else
	wxImageList* m_pImageList = nullptr;
#endif

	bool m_bSetHImageList = false;
	static std::unique_ptr<CImageList> m_pImageListInstance;
};

#endif // IMAGELIST_H_INCLUDED
