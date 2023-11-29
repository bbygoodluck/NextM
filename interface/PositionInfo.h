#ifndef POSITIONINFO_H
#define POSITIONINFO_H

class CPositionInfo
{
public:
	explicit CPositionInfo();
	~CPositionInfo();

public:
	wxRect	m_mainRect;
	wxRect	m_nameRect;
	wxRect	m_iconRect;
	wxRect	m_sizeRect;
	wxRect	m_sizeRectFile;
	wxRect  m_sizeRectType;
	wxRect	m_timeRect;
	wxRect	m_attrRect;
	wxPoint m_ptSize;
	wxPoint m_ptSizeType;
	wxPoint	m_ptTime;
	wxPoint m_ptAttr;
	wxRect  m_permsRect;
	wxRect	m_typeNameRect;
};

#endif // POSITIONINFO_H

