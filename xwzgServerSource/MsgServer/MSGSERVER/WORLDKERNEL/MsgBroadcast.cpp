// MsgBroadcast.cpp: implementation of the CMsgBroadcast class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "WorldKernel.h"
using namespace world_kernel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgBroadcast::CMsgBroadcast()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;

}

CMsgBroadcast::~CMsgBroadcast()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgBroadcast::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgBroadcast::Create(/*USHORT uType, */const char* wTime, UINT uProfession, UINT downLevel,
							UINT upLevel,/* UINT uTimes, UINT uInterval,*/const char* str)
{
	this->Init();

	//m_pInfo->uType = uType;
	m_pInfo->uProfession = uProfession;
	m_pInfo->downLevel = downLevel;
	m_pInfo->upLevel = upLevel;
	//m_pInfo->uTimes = uTimes;
	//m_pInfo->uInterval = uInterval;
	strcpy(m_pInfo->wTime,wTime);
	strcpy(m_pInfo->strContent,str);
	m_unMsgType	=_MSG_BROADCAST;
	m_unMsgSize	=sizeof( MSG_Info );
	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgBroadcast::Process(void *pInfo)
{

}
