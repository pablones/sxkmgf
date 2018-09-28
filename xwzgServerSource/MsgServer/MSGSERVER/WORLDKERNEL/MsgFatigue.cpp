// MsgFatigue.cpp: implementation of the CMsgFatigue class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "WorldKernel.h"
using namespace world_kernel;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgFatigue::CMsgFatigue()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgFatigue::~CMsgFatigue()
{

}
BOOL CMsgFatigue::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_Fatigue != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgFatigue::Create(OBJID AccountID, OBJID PlayerID, int Fatigue)
{
	this->Init();
	m_pInfo->AccountID = AccountID;
	m_pInfo->PlayerID = PlayerID;
	m_pInfo->Fatigue  = Fatigue;
	
	m_unMsgType	=_MSG_Fatigue;
	m_unMsgSize	=sizeof(MSG_Info);
	return TRUE;
}

void CMsgFatigue::Process(void *pInfo)
{
	GameWorld()->SendFATIRequest(m_unMsgType, m_unMsgSize,m_pInfo->AccountID, m_pInfo->PlayerID, m_pInfo->Fatigue);
}
