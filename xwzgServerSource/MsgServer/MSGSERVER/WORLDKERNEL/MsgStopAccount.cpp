// MsgStopAccount.cpp: implementation of the CMsgStopAccount class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "WorldKernel.h"
using namespace world_kernel;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgStopAccount::CMsgStopAccount()
{
	Init();
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

BOOL CMsgStopAccount::Create(USHORT nAction, OBJID playerID, UINT nParam)
{
	this->Init();

	// fill info now
	m_pInfo->playerID = playerID;
	m_pInfo->nParam = nParam;

	m_pInfo->unAciton=nAction;
	m_unMsgType	=_MSG_GMSTOPACCOUNT;
	m_unMsgSize	=sizeof( MSG_Info );

	return true;
}

void CMsgStopAccount::Process(void *pInfo)
{
	GameWorld()->SendStopAccount(m_pInfo->ucStopIDAccount,m_unMsgType,m_unMsgSize,m_pInfo->unAciton,m_pInfo->playerID);
}