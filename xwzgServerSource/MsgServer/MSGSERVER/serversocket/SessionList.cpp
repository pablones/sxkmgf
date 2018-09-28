#include "SessionList.h"
#include "Session.h"

SessionList::SessionList()
{
	InitializeCriticalSection( &m_cs );
}

SessionList::~SessionList()
{
	DeleteCriticalSection( &m_cs );
	//Clear();
}

VOID SessionList::Clear()
{
	SESSION_LIST_ITER it;

	Session *pSession;

	Lock();
	for( it = begin(); it != end(); ++it )
	{
		pSession = *it;
		delete pSession;
	}
	Unlock();

	clear();
}
