// MsgInsMemInfo.cpp: implementation of the CMsgInsMemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgInsMemInfo::CMsgInsMemInfo()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgInsMemInfo::~CMsgInsMemInfo()
{

}

BOOL CMsgInsMemInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_INSMEMINFO != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgInsMemInfo::Create(const char* szName,bool condition1, bool condition2, bool condition3, bool condition4, bool condition5)
{
	this->Init();
	// fill info now
	m_pInfo->dwData			= 0;

	strcpy(m_pInfo->stInfo[m_pInfo->dwData].szName,szName);
	m_pInfo->stInfo[m_pInfo->dwData].dwCondition		= 0;
	if(condition1)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 1;
	if(condition2)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 10;
	if(condition3)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 100;
	if(condition4)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 1000;
	if(condition5)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 10000;

	m_pInfo->dwData ++;

	m_unMsgType	=_MSG_INSMEMINFO;
	m_unMsgSize	=sizeof(MSG_Info);
	return true;
}

BOOL CMsgInsMemInfo::AddMem(const char* szName,bool condition1, bool condition2, bool condition3, bool condition4, bool condition5)
{
	m_unMsgSize += sizeof(MemInfo);
	CHECKF(m_unMsgSize < GetMaxSize());
	
	strcpy(m_pInfo->stInfo[m_pInfo->dwData].szName,szName);
	m_pInfo->stInfo[m_pInfo->dwData].dwCondition		= 0;
	if(condition1)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 1;
	if(condition2)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 10;
	if(condition3)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 100;
	if(condition4)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 1000;
	if(condition5)
		m_pInfo->stInfo[m_pInfo->dwData].dwCondition += 10000;
	
	m_pInfo->dwData ++;
	return true;
}

void CMsgInsMemInfo::Process(void *pInfo)
{
	return;
}