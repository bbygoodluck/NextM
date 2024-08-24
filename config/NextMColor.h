#ifndef NEXTMCOLOR_H_INCLUDED
#define NEXTMCOLOR_H_INCLUDED

#include "JsonBase.h"
class CNextMColor : public CJsonBase
{
private:
	CNextMColor();

public:
	~CNextMColor();
	static CNextMColor* Get();

public:
	bool Load() override;

	const wxColour GetExtColor(const wxString& strExt);

	const wxColour GetDefaultColor()      { return m_defaultColor; }
	const wxColour GetDirColor()          { return m_dirColor; }
	const wxColour GetDriveColor()        { return m_drvColor; }
	const wxColour GetReadOnlyColor()     { return m_readOnlyColor; }
	const wxColour GetSelectItemColor()   { return m_selectItemColor; }
	const wxColour GetBackgroundColor()   { return m_backgroundColor; }
	const wxColour GetAttributeColor()    { return m_attributeColor; }
	const wxColour GetDescriptionColor()  { return m_descColor; }
	const wxColour GetColumnColor()       { return m_columnColor; }
	const wxColour GetInfoViewBackColor() { return m_infoviewColor; }
	const wxColour GetInfoViewTextColor() { return m_infoviewtxtColor; }
	const wxColour GetMatchColor()        { return m_matchColor; }

	void SetColors();
	void SetChangeColorVal(const wxString& strKey, const wxString& strMember, const wxString& strVal);

private:

	unsigned long ConvertStrToColor(wxString& strColor);

private:
	static std::unique_ptr<CNextMColor> m_pInstance;

	wxColour m_defaultColor;
	wxColour m_dirColor;
	wxColour m_drvColor;
	wxColour m_readOnlyColor;
	wxColour m_selectItemColor;
	wxColour m_backgroundColor;
	wxColour m_attributeColor;
	wxColour m_descColor;
	wxColour m_columnColor;
	wxColour m_infoviewColor;
	wxColour m_infoviewtxtColor;
	wxColour m_matchColor;
};


#endif // NEXTMCOLOR_H_INCLUDED
