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
	case  FightTeamAction_Create:        //����ս��
		break;
 	case  FightTeamAction_Destroy:       //ɾ��ս��
 		break;
 	case  FightTeamAction_InviteMember:  //�����Ա
 		break;
 	case  FightTeamAction_Refruse:       //����ܾ�
 		{
// 			CMsgTalk msg;
// 			if (msg.Create(SYSTEM_NAME, pUser->szName, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
// 				return this->SendMsg(&msg);
// 
//  #ifdef _DEBUG
//  			pUser->SendSysMsg("����ܾ�!");
//  #endif
		}
 		break;
 	case  FightTeamAction_OK:            //����ɹ�
 		break;
 	case  FightTeamAction_ChangeLeader:  //�����ӳ�
 		{
			char szName[2][32];
			m_StrPacker.GetString(0,szName[0],32 * sizeof(char));  //ս������
 			m_StrPacker.GetString(1,szName[1],32 * sizeof(char));  //�¶ӳ�����

			CPlayer *pPlayer = UserList()->GetPlayer(szName[1]); //����¶ӳ���ָ��
			if(pPlayer)
			{
				char szWord[128];
				sprintf(szWord,"���Ѿ���Ϊ%sս�ӵ��¶ӳ�!",szName[1]);
				CMsgTalk msg;
				if(msg.Create(SYSTEM_NAME,szName[1],szWord,NULL,0xff0000,_TXTATR_SYSTEM))
				     pPlayer->SendMsg(&msg);
				return;
			}
			else
			{
				char szWord[128];
				sprintf(szWord,"û���ҵ��¶ӳ�:%s",szName[1]);
				CMsgTalk msg;
				if(msg.Create(SYSTEM_NAME,szName[1],szWord,NULL,0xff0000,_TXTATR_SYSTEM))
					pUser->SendMsg(&msg);
			}

#ifdef _DEBUG
			enum{Active_2v2 = 2,Active_3v3 = 3,Active_5v5 = 5};
			//���뾺����FB���浯����
			CMsgFightTeamAction msg1;
			if(msg1.Create(FightTeamAction_InviteEnterInstace,NULL,NULL,Active_2v2))
				pUser->SendMsg(&msg1);

 			char szWord[32];
 			sprintf(szWord,"UserId:[%d] ս��%s ����ӳ�Ϊ:%s",m_pInfo->dwData,szName[0],szName[1]);
			CMsgTalk msg2;
			if(msg2.Create(SYSTEM_NAME,pUser->szName,szWord,NULL,0xff0000,_TXTATR_SYSTEM))
 			   pUser->SendMsg(&msg2);
#endif
 		}
 		break;
 	case  FightTeamAction_DelMember:     //ɾ������
 		{

 		}
 		break;
 	case  FightTeamAction_ExitTeam:      //�˳�����
 		{

 		}
 		break;
 	case  FightTeamAction_OnLine:        //�Ƿ�����
 		{

 		}
 		break;
 	case FightTeamAction_Query:          //��ѯս��
		{

		}
		break;
	case FightTeamAction_InviteEnterInstace:  //�������FB
		{

		}
		break;
	}
}
