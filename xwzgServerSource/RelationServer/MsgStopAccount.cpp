// MsgStopAccount.cpp: implementation of the CMsgStopAccount class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgStopAccount.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgStopAccount::CMsgStopAccount()
{
	Init();///³õÊ¼»¯°ü
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgStopAccount::~CMsgStopAccount()
{

}
BOOL CMsgStopAccount::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_GMSTOPACCOUNT != this->GetType())
		return false;

	return true;

}

BOOL CMsgStopAccount::Create(USHORT nAction, UINT nParam ,OBJID playerID)
{
	// fill info now
	this->Init();
	
	m_pInfo->playerID = playerID;
	m_pInfo->nParam = nParam;
	m_pInfo->unAction = nAction;
	m_unMsgType	=_MSG_GMSTOPACCOUNT;
	m_unMsgSize	=sizeof( MSG_Info );
	return true;
}