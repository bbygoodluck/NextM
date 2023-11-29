#ifndef NEXTM_H_INCLUDED
#define NEXTM_H_INCLUDED

/***************************************************************
 * Name:      NextM.h
 * Purpose:   Defines Application Class
 * Author:    BaeByoungYong (bbygoodluck@gmail.com)
 * Created:   2023-09-26
 * Copyright: BaeByoungYong (https://github.com/bbygoodluck)
 * License:
 **************************************************************/

class CNextMApp : public wxApp
{
public:
	explicit CNextMApp();
	virtual ~CNextMApp();

	bool OnInit() override;
	int OnExit() override;

#ifdef NDEBUG
	wxSingleInstanceChecker* m_pSingleInstance;
#endif

private:
    wxString m_strTitle = wxT("");
    bool m_bAddFontRC = false;

	wxString _strCurrentPath = wxT("");
private:
	bool Initialize();
	bool InitConfig();
};

#endif // NEXTM_H_INCLUDED
