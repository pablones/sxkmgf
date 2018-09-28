// MsgAward.cpp: implementation of the CMsgAward class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgAward.h"
#include "MapGroup.h"
#include "IniFile.h"

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
	CUserPtr pUser	= UserManager()->GetUser(this);
	if (!pUser)
		return;

	switch(m_pInfo->nAction)
	{
	case _GAINAWARD_QUERY:
		{
// 			strcpy(m_pInfo->szServerName, MapGroup(PID)->GetServerName());
// 			m_pInfo->accountID = pUser->GetAccountID();
// 			m_pInfo->idUser   = pUser->GetID();
// 
// 	        m_pInfo->unMsgSize = sizeof(MSG_Info);
// 			m_pInfo->unMsgType = _MSG_GAINAWARDS;
// 			MapGroup(PID)->QueryIntraMsg()->SendAwardToWorld((LPCTSTR)m_pInfo, sizeof(MSG_Info));
		}
		break;
	case  _GAINAWARD_RESETNAME:
		{
			int i =0;
		}
		break;
	default:
		break;
	}
}
