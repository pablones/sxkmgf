// MsgAward.cpp: implementation of the CMsgAward class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "WorldKernel.h"
using namespace world_kernel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAward::CMsgAward()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgSize = sizeof(MSG_Info);
}

CMsgAward::~CMsgAward()
{

}

BOOL CMsgAward::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_GAINAWARDS != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgAward::Create(UINT nAction, OBJID accountID, OBJID idUser, const char* szServerName, const GainAwardInfo* gainInfo)
{
	m_unMsgType  = _MSG_GAINAWARDS;
	m_unMsgSize  = sizeof(MSG_Info);
	
	m_pInfo->nAction = nAction;
	m_pInfo->accountID = accountID;
	m_pInfo->idUser = idUser;
	strcpy(m_pInfo->szServerName, szServerName);
	m_pInfo->nAmount = 1;
	memcpy(m_pInfo->gaInfo, gainInfo, sizeof(GainAwardInfo));
	
	return TRUE;
}

void CMsgAward::Process(void *pInfo)
{
	switch(m_pInfo->nAction)
	{
	case _GAINAWARD_SPECILLINE:
		{
			GameWorld()->SendLastLineToAcc(_MSG_GAINAWARDS, m_pInfo->nAction, m_pInfo->accountID, m_pInfo->idUser, m_pInfo->szServerName, m_pInfo->gaInfo, m_pInfo->nAmount, m_unMsgSize);
		}
		break;
	}
}