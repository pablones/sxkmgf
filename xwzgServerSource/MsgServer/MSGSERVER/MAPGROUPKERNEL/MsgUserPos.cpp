// MsgUserPos.cpp: implementation of the CMsgUserPos class.
//
//////////////////////////////////////////////////////////////////////
#include "MapGroup.h"
#include "MsgUserPos.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgUserPos::CMsgUserPos()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgUserPos::~CMsgUserPos()
{

}

BOOL CMsgUserPos::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_USERPOS != this->GetType())
		return false;

	return true;
}

void CMsgUserPos::Process(void *pInfo)
{
// 	CUser* pUser = MapGroup(PID)->GetUserManager()->GetUser(m_pInfo->idUser);
// 	if(pUser)
// 	{
// 		m_pInfo->usPosX = pUser->GetPosX();
// 		m_pInfo->usPosY = pUser->GetPosY();
// 	}
// 	else
// 	{
// 		m_pInfo->usPosX = -1;
// 		m_pInfo->usPosY = -1;
// 	}
// 
// 	//发送给npc服务器
// 	MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(m_idNpc,this);
}



