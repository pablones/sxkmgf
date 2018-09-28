// MsgAward.cpp: implementation of the CMsgAward class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgAward.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAward::CMsgAward()
{
	Init();///³õÊ¼»¯°ü
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgAward::~CMsgAward()
{

}

BOOL CMsgAward::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_GAINAWARDS != this->GetType())
		return false;

	return true;
}
BOOL CMsgAward::Create(int nAction)
{
	m_unMsgType  = _MSG_GAINAWARDS;
	m_unMsgSize  = sizeof(MSG_Info);

	m_pInfo->nAction = nAction;
	m_pInfo->accountID = 0;
	m_pInfo->idUser = 0;
	strcpy(m_pInfo->szServerName, "");
	m_pInfo->nAmount = 1;
	GainAwardInfo gainInfo={0};
	memcpy(m_pInfo->gaInfo, &gainInfo, sizeof(GainAwardInfo));
	return TRUE;
}