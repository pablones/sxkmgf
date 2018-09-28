// MyCriticalSection.cpp: implementation of the CMyCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#include "MyCriticalSection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyCriticalSection::CMyCriticalSection()
{
	Init();
}

CMyCriticalSection::~CMyCriticalSection()
{
	Term();
}

void CMyCriticalSection::Init()
{
	::InitializeCriticalSection(&m_cs);
}

void CMyCriticalSection::Lock()
{
	::EnterCriticalSection(&m_cs);
}

void CMyCriticalSection::Unlock()
{
	::LeaveCriticalSection(&m_cs);
}

void CMyCriticalSection::Term()
{
	::DeleteCriticalSection(&m_cs);	
}
