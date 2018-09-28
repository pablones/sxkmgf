// MsgFatigue.cpp: implementation of the MsgFatigue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgFatigue.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgFatigue::CMsgFatigue()
{
	Init();///初始化包
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgFatigue::~CMsgFatigue()
{

}

BOOL CMsgFatigue::Create( OBJID AccountID, OBJID PlayerID,  int Fatigue)
{
	this->Init();
	m_pInfo->AccountID = AccountID;
	m_pInfo->PlayerID = PlayerID;
	m_pInfo->Fatigue  = Fatigue;
	
	m_unMsgType	=_MSG_Fatigue;
	m_unMsgSize	=sizeof(MSG_Info);
	return TRUE;
}
///解包函数
BOOL CMsgFatigue::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_VAS != this->GetType())
		return false;
	
	return true;
}