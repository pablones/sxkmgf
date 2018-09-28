// MsgFatigue.cpp: implementation of the CMsgFatigue class.
//
//////////////////////////////////////////////////////////////////////

#include <afxdisp.h>
#include "MsgFatigue.h"
#include "MapGroup.h"
#include "MsgUserAttrib.h"
#include "AdjustRate.h"
#include "MsgItemAttrib.h"

#include "WargameManager.h"
#include "MapGroup.h"
#include "SKPostOffice.h"


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
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;
	pUser->SetFatigue(m_pInfo->Fatigue);//∑¿≥¡√‘
	return;
}
