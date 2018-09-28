// MsgFightTeamAction.cpp: implementation of the CMsgFightTeamAction class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgFightTeamAction.h"
#include "UserList.h"
#include "WorldKernel.h"
using namespace world_kernel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgFightTeamAction::CMsgFightTeamAction()
{
	Init();
	m_pInfo = (MSG_Info *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf,_MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgFightTeamAction::~CMsgFightTeamAction()
{

}


BOOL CMsgFightTeamAction::Create(int nType, const char* pszFightTeamName, const char* pszMemberName, DWORD dwData,DWORD dwType)
{
	this->Init();

	m_unMsgSize = sizeof(MSG_Info);
	m_unMsgType = _MSG_FIGHTTEAMACTION;

	m_pInfo->dwData = dwData;
	m_pInfo->ucFightTeamType = (unsigned char)nType;
	m_pInfo->dwType = dwType;
	if(pszFightTeamName)
		m_StrPacker.AddString(pszFightTeamName);

	if(pszMemberName)
		m_StrPacker.AddString(pszMemberName);
	m_unMsgSize = sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();

	return true;
}

BOOL CMsgFightTeamAction::Create(char* pMsgBuf, DWORD dwSize)
{
	if(!CNetMsg::Create(pMsgBuf,dwSize))
	   return false;

	if(_MSG_FIGHTTEAMACTION != this->GetType())
		 return false;

	return true;
}

void CMsgFightTeamAction::Process(void* pInfo)
{
	CPlayer* pUser = UserList()->GetPlayerBySocket(GetSocketID());

	switch (m_pInfo->ucFightTeamType)
	{
	case  FightTeamAction_Create:        //创建战队
		break;
 	case  FightTeamAction_Destroy:       //删除战队
 		break;
 	case  FightTeamAction_InviteMember:  //邀请成员
 		break;
 	case  FightTeamAction_Refruse:       //邀请拒绝
 		{
// 			CMsgTalk msg;
// 			if (msg.Create(SYSTEM_NAME, pUser->szName, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
// 				return this->SendMsg(&msg);
// 
//  #ifdef _DEBUG
//  			pUser->SendSysMsg("邀请拒绝!");
//  #endif
		}
 		break;
 	case  FightTeamAction_OK:            //邀请成功
 		break;
 	case  FightTeamAction_ChangeLeader:  //更换队长
 		{
			char szName[2][32];
			m_StrPacker.GetString(0,szName[0],32 * sizeof(char));  //战队名字
 			m_StrPacker.GetString(1,szName[1],32 * sizeof(char));  //新队长名字

			CPlayer *pPlayer = UserList()->GetPlayer(szName[1]); //获得新队长的指针
			if(pPlayer)
			{
				char szWord[128];
				sprintf(szWord,"你已经成为%s战队的新队长!",szName[1]);
				CMsgTalk msg;
				if(msg.Create(SYSTEM_NAME,szName[1],szWord,NULL,0xff0000,_TXTATR_SYSTEM))
				     pPlayer->SendMsg(&msg);
				return;
			}
			else
			{
				char szWord[128];
				sprintf(szWord,"没有找到新队长:%s",szName[1]);
				CMsgTalk msg;
				if(msg.Create(SYSTEM_NAME,szName[1],szWord,NULL,0xff0000,_TXTATR_SYSTEM))
					pUser->SendMsg(&msg);
			}

#ifdef _DEBUG
			enum{Active_2v2 = 2,Active_3v3 = 3,Active_5v5 = 5};
			//进入竞技场FB界面弹出来
			CMsgFightTeamAction msg1;
			if(msg1.Create(FightTeamAction_InviteEnterInstace,NULL,NULL,Active_2v2))
				pUser->SendMsg(&msg1);

 			char szWord[32];
 			sprintf(szWord,"UserId:[%d] 战队%s 变更队长为:%s",m_pInfo->dwData,szName[0],szName[1]);
			CMsgTalk msg2;
			if(msg2.Create(SYSTEM_NAME,pUser->szName,szWord,NULL,0xff0000,_TXTATR_SYSTEM))
 			   pUser->SendMsg(&msg2);
#endif
 		}
 		break;
 	case  FightTeamAction_DelMember:     //删除队友
 		{

 		}
 		break;
 	case  FightTeamAction_ExitTeam:      //退出队伍
 		{

 		}
 		break;
 	case  FightTeamAction_OnLine:        //是否在线
 		{

 		}
 		break;
 	case FightTeamAction_Query:          //查询战队
		{

		}
		break;
	case FightTeamAction_InviteEnterInstace:  //邀请进入FB
		{

		}
		break;
	}
}
