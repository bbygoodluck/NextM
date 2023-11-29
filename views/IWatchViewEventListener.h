#ifndef VIEWEVENTLISTENER_H_INCLUDED
#define VIEWEVENTLISTENER_H_INCLUDED

#include "../interface/WatcherItem.h"
class IWatchViewEventListener
{
public:
	virtual void DoCreate(CWatcherItem* pItem) = 0;
	virtual void DoModify(CWatcherItem* pItem) = 0;
	virtual void DoDelete(CWatcherItem* pItem) = 0;
	virtual void DoRename(CWatcherItem* pItem) = 0;
};

#endif // VIEWEVENTLISTENER_H_INCLUDED
