#include "../ginc.h"
#include "PositionInfo.h"

CPositionInfo::CPositionInfo()
	: m_mainRect(0, 0, 0, 0),
	  m_nameRect(0, 0, 0, 0),
	  m_iconRect(0, 0, 0, 0),
	  m_ptSize(0, 0),
	  m_ptSizeType(0, 0),
	  m_ptTime(0, 0),
	  m_ptAttr(0, 0),
	  m_permsRect(0, 0, 0, 0),
	  m_typeNameRect(0, 0, 0, 0)
{

}

CPositionInfo::~CPositionInfo()
{

}
