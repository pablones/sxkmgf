// MsgStopAccount.cpp: implementation of the CMsgStopAccount class.
//
//////////////////////////////////////////////////////////////////////
#include "MsgStopAccount.h"
#include "MapGroup.h"
#include "MsgUserAttrib.h"
#include "AdjustRate.h"

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
BOOL CMsgStopAccount::Create(OBJID StopAccountID , OBJID playerID, USHORT nAction)
{
	this->Init();

	// fill info now
	m_pInfo->ucStopIDAccount = StopAccountID;///ÕÊºÅID
	m_pInfo->playerID = playerID;

	m_pInfo->unAction = nAction;
	m_unMsgType	=_MSG_GMSTOPACCOUNT;
	m_unMsgSize	=sizeof( MSG_Info );

	return true;

}

void CMsgStopAccount::Process(void *pInfo)
{
	CUserPtr pUser	= UserManager()->GetUser(this);
	
	if (!pUser) 
	{
		return;
	}

	switch(m_pInfo->unAction) 
	{
	case _ACTION_ISSTOPACCOUNT:
		{	
			//-------------------------------------
			if (m_pInfo->nParam)
			{
//				pUser->Unmarry(CUser::_TYPE_SPECIALUNMARRY);
			}
			else
			{
				///´ËºÅÃ»±»·âÍ£
				pUser->SendSysMsg(g_objStrRes.GetStr(28064));
			}
		}
		break;
	default:
		break;
	}
}