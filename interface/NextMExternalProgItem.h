#ifndef NEXTMEXTERNALPROGRAMINFO_H_INCLUDED
#define NEXTMEXTERNALPROGRAMINFO_H_INCLUDED

class CExternalProgItem
{
public:
	CExternalProgItem() {}
	~CExternalProgItem() {}

public:
	wxString _strPGName = wxT("");
	wxString _strPGPath = wxT("");
	wxString _strPGArgs = wxT("");
};

#endif // NEXTMEXTERNALPROGRAMINFO_H_INCLUDED
