// MsgTeam.cpp: implementation of the CMsgTeam class.
// 
// History��
//	1.	�޸����ʵ�ַ�ʽ By Minsky,2010-05-28
//////////////////////////////////////////////////////////////////////
#include "afx.h"
#include "allmsg.h"
#include "mapgroup.h"
#include "I_MessagePort.h"
#include "protocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTeam::CMsgTeam()
{
	this->Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgTeam::~CMsgTeam()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTeam::Create(OBJID idPlayer,  int nAction, UINT nMap )
{
	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TEAM;

	m_pInfo->idPlayer	= idPlayer;
	m_pInfo->idMap      = nMap;
	m_pInfo->unAction	= nAction;
	return true;
}

BOOL CMsgTeam::Create(OBJID idPlayer, const char* szPlayerName, int nAction,  UINT idMap, UINT nTarget, const char* szTargetName)
{
	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TEAM;

	m_pInfo->idPlayer	= idPlayer;
	m_pInfo->unAction	= nAction;
	
	m_pInfo->idMap      = idMap;
	m_pInfo->idTarget = nTarget;

	if(szPlayerName && strlen(szPlayerName) > 0)
		strcpy(m_pInfo->szPlayerName, szPlayerName);
	if(szTargetName && strlen(szTargetName) > 0)
		strcpy(m_pInfo->szTargetName, szTargetName);

	return true;
}

BOOL CMsgTeam::Create(int nAction, UINT nTarget, UINT nTeamLev, UINT nLeaderID, const char* szLeaderName, UINT nLeaderLev, UINT nMemAmount )
{
	this->Init();
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TEAM;

	m_pInfo->unAction = nAction;
	m_pInfo->idTarget = nTarget;

	m_pInfo->idPlayer = nLeaderID;
	strcpy(m_pInfo->szPlayerName, szLeaderName);

	return TRUE;
}

BOOL CMsgTeam::CreateMemInfo(int nAction, OBJID idPlayer, const char *pName, int nLev)
{
	this->Init();
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TEAM;

	m_pInfo->unAction = nAction;
	strcpy(m_pInfo->szPlayerName, pName);
	m_pInfo->idPlayer = idPlayer;

	if(_MSGTEAM_INVITE_WAIT == nAction)
	{
		// By Minsky, to be modified!!!!!!!!!!!!!
		m_pInfo->usLev = nLev;   //��ɫ����
	}
	else
	{
		// By Minsky, to be modified!!!!!!!!!!!!!
		m_pInfo->idMap = nLev;
	}

	return TRUE;
}

BOOL CMsgTeam::CreateAskInfo(int nAction)
{
	this->Init();
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TEAM;

	m_pInfo->unAction = nAction;
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTeam::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTeam::Process(void* pInfo)
{
	int nStep = 0;
	DEBUG_TRY
	CUserPtr pUser	= UserManager()->GetUser(this);
	if (!pUser)
		return;

	nStep = 1;
	if (pUser->IsGM() && !pUser->IsPM())
	{
		pUser->SendSysMsg(g_objStrRes.GetStr(16007));//"��GM�����ܽ��ж��������"
		return;
	}

	nStep = 2;
	CGameMap* pMap = pUser->GetMap();
	if (pMap && pMap->IsTeamDisable())
		return;

	nStep = 3;
	switch(m_pInfo->unAction)
	{
	case _MSGTEAM_CREATE:
		{
			if(!pUser->GetIsAllowTeam())
			{
				pUser->SendSysMsg("���趨Ϊ�������");
				return ;
			}
			DEBUG_TRY
			CTeam* pTeam = pUser->GetTeam() ;
			if( pTeam == NULL)
			{
				char teamName[64] = {0};//Ĭ��С����
				sprintf(teamName,"%s��С��",pUser->GetName());

				if (pUser->CreateTeam())
				{
					CTeam* pTeam = pUser->GetTeam();
					CHECK(pTeam);
					pTeam->SetTeamName(teamName);
					if(this->Create(pUser->GetID(), _MSGTEAM_CREATE, pUser->GetMapID()))
					{
						pUser->SendMsg(this);
						//pUser->BroadcastRoomMsg(this,true);
					}

					//����Ǯ������˷���
					char szTalkWords[128] = {0};
					sprintf(szTalkWords,"%s�����˶��飬������뱻�ܾ�",pUser->GetName());
					OBJID InviteUserID = ID_NONE;
					for(int i = 0 ; i < 5 ; i++)
					{
						InviteUserID = pUser->GetInviteUserID(i);
						if(InviteUserID != ID_NONE)
						{
							CUser *pInviteUser = UserManager()->GetUser(InviteUserID);
							if(pInviteUser)
							{
								pInviteUser->SendSysMsg(szTalkWords);
							}
						}
					}

					pUser->ClrInviteUserID(ID_NONE);

					CMsgPlayer msgPlayer;
					if (msgPlayer.Create(pUser->QueryRole()))
						pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

					//����ʦͽ״̬
					if (pUser->GetTutorManager())
						pUser->GetTutorManager()->BroadCastTutorState();
				}
			}
			else // ���ж��飬���ɴ���
			{
				pUser->SendSysMsg("�����ڶ�����");
				return;
			}
			DEBUG_CATCH("MsgTeam proc create team crash.")
		}
		break;
	case _MSGTEAM_CHG_LEADER_APPLY://�ƽ��ӳ�
		{
			CTeam* pTeam = pUser->GetTeam();
			if(pTeam != NULL)
			{
				CHECK(m_pInfo->idPlayer);

				CUser* pPlayer = UserManager()->GetUser(m_pInfo->idPlayer);
				if(pPlayer)//�ڱ���ͼ��
				{
					pTeam->ChangeTeamLeader(m_pInfo->idPlayer);
				
					if(this->Create(m_pInfo->idPlayer,pPlayer->GetName(), _MSGTEAM_CHG_LEADER_APPLY))
					{
						// �㲥����ӳ���Ϣ�������Լ�
						pTeam->BroadcastTeamMsg(this);
					}
				}
				else
				{
					this->Create(pUser->GetID(), pUser->GetName(),_MSGTEAM_ASKINFO);
					pUser->SendMsg(this);
					pUser->SendSysMsg("�Է�����̫Զ");
				}
			}
		}	
		break;
	case _MSGTEAM_APPLYJOIN:
		{
			if (m_pInfo->idPlayer == ID_NONE)
				return;

			if (pUser->GetTeam())
			{
				// By Minsky,2010-05-28���㲻�����ܿ�������Ϣ
				this->Create(m_pInfo->idPlayer, _MSGTEAM_USER_INTEAM);
				pUser->SendMsg(this);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"���ڶ�����");//STR_INTEAM_NOJOIN
				return;
			}

			OBJID	idTarget = m_pInfo->idPlayer;
			CUser* pTarget = NULL;

			DEBUG_TRY
			pTarget = UserManager()->GetUser(idTarget);
			if(!pTarget)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"���󲻴���");//STR_NO_APPLICANT
				return;
			}
			IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);
			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
			{
				if(pStatus->GetParam1() != pStatusTarget->GetParam1())//��ͬ��Ӫ
				{
					pUser->SendSysMsg("����Է��ڲ�ͬ����Ӫ�У��������");
					return;
				}
			}
			DEBUG_CATCH("MsgTeam apply join Queryobj by ID crash.");

			CTeam* pTeam	= pTarget->GetTeam();
			if (!pTeam)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"����Ҷ��鲻����");//STR_NO_TEAM
				return;
			}

			if (pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"����Ҷ�������");//STR_TEAM_FULL
				return;
			}

			// By Minsky,2010-05-28
			if (pTeam->GetMemberApplyAmount() >= _MAX_TEAMAMOUNTOUT)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�ö���������б�����");
			}

			pTarget = UserManager()->GetUser(pTeam->GetLeader());
			if(pTarget==NULL)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�ӳ�����̫Զ");
				return;
			}

			if (!pTarget->IsAgreeTeamApply())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѵ򿪾ܾ���ӿ���");
				return;
			}

//			pUser->SetApply(CUser::APPLY_TEAMAPPLY, pTeam->GetLeader());
			pTeam->AddMemberApply(pUser->GetID());

			IF_NOT(this->Create(pUser->GetID(), (char*)pUser->GetName(), _MSGTEAM_APPLYJOIN, NULL))
				return;
			pTeam->BroadcastTeamMsg(this);

			pUser->SendSysMsg(_TXTATR_TOPSHOW,"�����������ѳɹ�����");//STR_JOIN_REQUEST_SENT
		}
		break;
	case _MSGTEAM_AGREEJOIN:			//ͬ������
		{
			CUserPtr pTarget = UserManager()->GetUser(m_pInfo->idPlayer);
			if(pTarget == NULL)
			{		
				return;
// 				ST_TEAM_INVITE st;
// 				memset(&st, 0, sizeof(ST_TEAM_INVITE));
// 				st.idPlayer = m_pInfo->idPlayer;
// 				st.idSocket = 0;
// 				strcpy(st.szPlayerName, m_pInfo->szPlayerName);
// 				st.idTarget = pUser->GetID();
// 				st.idSocketTarget = pUser->GetSocketID();
// 				strcpy(st.szTargetName, pUser->GetName());
// 
// 				CTeam* pTeam = pUser->GetTeam();
// 				if(pTeam)
// 				{
// 					int nRet;
// 					BOOL bIsValid = TRUE;
// 					if(pTeam->GetMemberAmount() > _MAX_TEAMAMOUNT)
// 					{
// 						nRet = 13029;	
// 						bIsValid = FALSE;
// 					}
// 					else if (pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
// 					{
// 						nRet = 13035;	
// 						bIsValid = FALSE;
// 					}
// 					
// 					else if (pTeam->GetLeader() != pUser->GetID())
// 					{
// 						nRet = 13015;	
// 						bIsValid = FALSE;
// 					}
// 					
// 					if(!bIsValid)
// 					{
// 						st.nRet = nRet;
// 						st.unAction = TEAM_INVITE_ACTION_TYPE_JOIN_NANK;
// 						MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);	
// 						return;
// 					}	
// 				}
// 				
// 				st.nRet = 0;
// 
// 				TeamInfoStruct info;
// 				pTeam->GetInfo(&info);
// 				st.teamInfo.id = info.id;
// 				st.teamInfo.idLeader = info.idLeader;
// 				st.teamInfo.bCloseMoney = info.bCloseMoney;
// 				st.teamInfo.bCloseItem = info.bCloseItem;
// 				st.teamInfo.nTeamMode = info.nTeamMode;
// 				st.teamInfo.nExpMode = info.nExpMode;
// 				st.teamInfo.nItemMode = info.nItemMode;
// 				memcpy(st.teamInfo.setMember, info.setMember, sizeof(info.setMember));
// 				strcpy(st.teamInfo.sTeamName, info.sTeamName);
// 				st.teamInfo.m_nLev = info.m_nLev;
// 				st.teamInfo.m_nTarget = info.m_nTarget;
// 				strcpy(st.teamInfo.m_sLeadName, info.m_sLeadName);
// 				strcpy(st.teamInfo.m_sTeamText, info.m_sTeamText);
// 
// 				st.unAction = TEAM_INVITE_ACTION_TYPE_JOIN_ANK;
// 				MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);	
// 				return;
			}

			IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);
			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
			{
				if(pStatus->GetParam1() != pStatusTarget->GetParam1())//��ͬ��Ӫ
				{
					pUser->SendSysMsg("����Է��ڲ�ͬ����Ӫ�У��������");
					return;
				}
			}
			if (m_pInfo->idPlayer == ID_NONE)
				return;

			CTeam* pTeam	= pUser->GetTeam();
			if (!pTeam)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"���鲻����");//STR_NO_TEAM_TO_INVITE
				return;
			}
			if (pTeam->GetLeader() != pUser->GetID())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�㲻�Ƕӳ�");//STR_NOT_CAPTAIN_INVITE
				return;
			}
			if (pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�ö�������");//STR_YOUR_TEAM_FULL
				return;
			}

			OBJID	idTarget = m_pInfo->idPlayer;

			DEBUG_TRY
			if(pTarget == NULL && !pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pTarget)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Ҳ���������");//STR_APPLICANT_NOT_FOUND
				return;
			}
			DEBUG_CATCH("CMsgTeam proc accept join Queryobj by idtarget crash.")

			if (pTarget->GetTeam())
			{
				this->Create(m_pInfo->idTarget,pTarget->GetName(), _MSGTEAM_TARGET_INTEAM);
				pTeam->BroadcastTeamMsg(this);
				//pTarget->SendSysMsg(g_objStrRes.GetStr(13022));//STR_HAVE_JOIN_TEAM
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"%s�Ѿ������˱��˵Ķ���",pTarget->GetName());//"�Է��Ѿ��ж����ˡ�"
				return;
			}
			
			if(pTeam->DelMemberApply(m_pInfo->idPlayer))//�����
			{
				if (pUser->AcceptTeamMember(pTarget))
				{
					pTarget->SetTeam(pTeam);

					//����Ǯ������˷���
					char szTalkWords[128] = {0};
					sprintf(szTalkWords,"%s�����˶��飬������뱻�ܾ�",pUser->GetName());
					OBJID InviteUserID = ID_NONE;
					for(int i = 0 ; i < 5 ; i++)
					{
						InviteUserID = pUser->GetInviteUserID(i);
						if(InviteUserID != ID_NONE)
						{
							CUser *pInviteUser = UserManager()->GetUser(InviteUserID);
							if(pInviteUser)
							{
								pInviteUser->SendSysMsg(_TXTATR_TOPSHOW,szTalkWords);
							}
						}
					}

					pTarget->ClrInviteUserID(ID_NONE);
					

					this->Create(pTarget->GetID(),pTarget->GetName(), _MSGTEAM_AGREEJOIN);
					pTarget->SendMsg(this);

					if(this->Create(pTarget->GetID(),pTarget->GetName(), _MSGTEAM_JOINTEAM))
					{
						pTeam->BroadcastTeamMsg(this);
					}

					CMsgPlayer msgPlayer;
					if (msgPlayer.Create(pTarget->QueryRole()))
						pTarget->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

					pTarget->SendPosToTeamMem(pTarget->GetMapID(), pTarget->GetPosX(), pTarget->GetPosY());//goto ֪ͨ�����Լ���λ��
					pTarget->GetPosFromTeamMem();//goto ��ȡ����λ����Ϣ
					//����ʦͽ״̬
					if (pTarget->GetTutorManager())
						pTarget->GetTutorManager()->BroadCastTutorState();
				}
			}
		}
		break;
	case _MSGTEAM_ACCEPTINVITE:		//��������
		{
			if (m_pInfo->idPlayer == ID_NONE)
			{
				// �������˳��������
				return;
			}

			if (pUser->GetTeam())
			{
				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
				pUser->SendMsg(this);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"���Ѿ��Ƕ�Ա�ˣ����ܽ�����������¶���");
				return;
			}

			OBJID	idTarget = m_pInfo->idPlayer;
			CUserPtr pTarget = NULL;
			if(m_pInfo->idPlayer != ID_NONE)
				pTarget = UserManager()->GetUser(m_pInfo->idPlayer);
			else 
				pTarget = UserManager()->GetUser(m_pInfo->szPlayerName);
			if(pTarget == NULL)
			{				
				//	���ܲ��ڴ˵�ͼ��,�㲥
// 				ST_TEAM_INVITE st;
// 				memset(&st, 0, sizeof(ST_TEAM_INVITE));
// 				st.unAction = TEAM_INVITE_ACTION_TYPE_INVITE_ANK;
// 				st.idPlayer = m_pInfo->idPlayer;
// 				st.idSocket = 0;
// 				strcpy(st.szPlayerName, m_pInfo->szPlayerName);
// 				st.idTarget = pUser->GetID();
// 				st.idSocketTarget = pUser->GetSocketID();
// 				strcpy(st.szTargetName, pUser->GetName());
// 				st.teamMemberInfo[0].id			= pUser->GetID();
// 				st.teamMemberInfo[0].dwLookFace	= pUser->GetLookFace();
// 				st.teamMemberInfo[0].dwLev      = pUser->GetLev();
// 				st.teamMemberInfo[0].usProfession = pUser->GetProfession();
// 				st.teamMemberInfo[0].usHp		= pUser->GetLife();
// 				st.teamMemberInfo[0].usMaxHp	= pUser->GetMaxLife();
// 				st.teamMemberInfo[0].usMp		= pUser->GetMana();
// 				st.teamMemberInfo[0].usMaxMp	= pUser->GetMaxMana();
// 				st.teamMemberInfo[0].usPosX		= pUser->GetPosX();
// 				st.teamMemberInfo[0].usPosY		= pUser->GetPosY();
// 				strcpy(st.teamMemberInfo[0].szName, pUser->GetName());
// 				MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);
				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
				pUser->SendMsg(this);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Ҳ���������");
				return;
			}
// 			DEBUG_TRY
// 			if(pTarget == NULL)
// 			{
// 				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
// 				pUser->SendMsg(this);
// 				pUser->SendSysMsg("[����]û���ҵ������ߡ�");
// 				return;
// 			}
// 			DEBUG_CATCH("CMsgTeam proc accept invite queryobj by id crash.")

			IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);
			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
			{
				if(pStatus->GetParam1() != pStatusTarget->GetParam1())//��ͬ��Ӫ
				{
					pUser->SendSysMsg("����Է��ڲ�ͬ����Ӫ�У��������");
					return;
				}
			}
			CTeam* pTeam	= pTarget->GetTeam();
			if (!pTeam)
			{
				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
				pUser->SendMsg(this);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է����鲻����");//STR_NOT_CREATE_TEAM
				return;
			}
			if (pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
			{
				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
				pUser->SendMsg(this);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է���������");//STR_HIS_TEAM_FULL
				return;
			}

// 			if (pTeam->GetLeader() != pTarget->GetID())
// 			{
// 				// By Minsky,2010-05-28������ˢ��
// 				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
// 				pUser->SendMsg(this);
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�ӳ��ѻ��ˣ������¼���");//STR_NO_CAPTAIN_CLOSE
// 				return;
// 			}

			
			if (pUser->AcceptTeamInvit(pTarget))
			{
				//����Ǯ������˷���
				char szTalkWords[128] = {0};
				sprintf(szTalkWords,"%s�����˶��飬������뱻�ܾ�",pUser->GetName());
				OBJID InviteUserID = ID_NONE;
				for(int i = 0 ; i < 5 ; i++)
				{
					InviteUserID = pUser->GetInviteUserID(i);
					if(InviteUserID != ID_NONE)
					{
						CUser *pInviteUser = UserManager()->GetUser(InviteUserID);
						if(pInviteUser)
						{
							pInviteUser->SendSysMsg(_TXTATR_TOPSHOW,szTalkWords);
						}
					}
				}

				pUser->ClrInviteUserID(ID_NONE);
				pUser->SetTeam(pTeam);
				if(this->Create(pUser->GetID(), (char*)pUser->GetName(), _MSGTEAM_JOINTEAM, NULL))
				{
					CTeam *pTeam = pUser->GetTeam();
					pTeam->BroadcastTeamMsg(this);
				}
				
				CMsgPlayer msgPlayer;
				if (msgPlayer.Create(pUser->QueryRole()))
					pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

				pUser->SendPosToTeamMem(pUser->GetMapID(), pUser->GetPosX(), pUser->GetPosY());//goto ֪ͨ�����Լ���λ��
				pUser->GetPosFromTeamMem();//goto ��ȡ����λ����Ϣ
				//����ʦͽ״̬
				if (pUser->GetTutorManager())
					pUser->GetTutorManager()->BroadCastTutorState();
			}
			else
			{
				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
				pUser->SendMsg(this);
			}
		}
		break;
		
	case _MSGTEAM_INVITE:			//Ҫ�������ӻ��������
		{
			CUserPtr pTarget = NULL;
			if(m_pInfo->idPlayer != ID_NONE)
				pTarget = UserManager()->GetUser(m_pInfo->idPlayer);
			else
				pTarget = UserManager()->GetUser(m_pInfo->szPlayerName);

// 			if(pTarget == NULL)
// 			{
// 				return;
// 				if(strlen(m_pInfo->szPlayerName) < 1)
// 				{
// 					return;
// 				}
// 				else
// 				{	
// 					CTeam * pTeam = NULL;
// 					if((pTeam = pUser->GetTeam()) != NULL )		//����Լ��ж���
// 					{
// 						if(pTeam->GetLeader() != pUser->GetID())
// 						{
// 							// ����Ϣ�����ܷ���
// 							pUser->SendSysMsg(g_objStrRes.GetStr(16009));//"�����Ƕӳ�"
// 							return ;
// 						}
// 						
// 						if (pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
// 						{
// 							// By Minsky,2010-05-28������ˢ��
// 							this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
// 							pUser->SendMsg(this);
// 							pUser->SendSysMsg(g_objStrRes.GetStr(13020));//STR_YOUR_TEAM_FULL
// 							return;
// 						}
// 					}
// 									
// 					//////////////////////////////////////////////////////////////////////////
// 					//  [2007-9-7] ��� �㲥,ȥ������ͼ���������
// 					//	pUser->SendSysMsg(g_objStrRes.GetStr(13027));//STR_INVITED_NOT_FOUND
// 					ST_TEAM_INVITE st;
// 					memset(&st, 0, sizeof(ST_TEAM_INVITE));
// 					st.idPlayer = pUser->GetID();
// 					strcpy(st.szPlayerName, pUser->GetName());
// 					st.idSocket = pUser->GetSocketID();
// 					st.unAction = m_pInfo->unAction;
// 					st.idTarget = m_pInfo->idPlayer;
// 					strcpy(st.szTargetName, m_pInfo->szPlayerName);
// 					if(pTeam)
// 					{
// 						st.unAction = TEAM_INVITE_ACTION_TYPE_INVITE_REQ;	//	����Է������Լ��Ķ�
// 
// 						TeamInfoStruct info;
// 						pTeam->GetInfo(&info);
// 						st.teamInfo.id = info.id;
// 						st.teamInfo.idLeader = info.idLeader;
// 						st.teamInfo.bCloseMoney = info.bCloseMoney;
// 						st.teamInfo.bCloseItem = info.bCloseItem;
// 						st.teamInfo.nTeamMode = info.nTeamMode;
// 						st.teamInfo.nExpMode = info.nExpMode;
// 						st.teamInfo.nItemMode = info.nItemMode;
// 						memcpy(st.teamInfo.setMember, info.setMember, sizeof(info.setMember));
// 						strcpy(st.teamInfo.sTeamName, info.sTeamName);
// 						st.teamInfo.m_nLev = info.m_nLev;
// 						st.teamInfo.m_nTarget = info.m_nTarget;
// 						strcpy(st.teamInfo.m_sLeadName, info.m_sLeadName);
// 						strcpy(st.teamInfo.m_sTeamText, info.m_sTeamText);
// 					}
// 					else
// 					{
// 						st.unAction = TEAM_INVITE_ACTION_TYPE_JOIN_REQ;	//	�������Է��Ķ�
// 						st.teamInfo.id = 0;
// 						st.teamInfo.idLeader = 0;
// 					}
// 					pUser->SetApplyName(CUser::APPLY_TEAMINVIT, m_pInfo->szPlayerName);
// 					MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);
// 					pUser->SendSysMsg(g_objStrRes.GetStr(13030));	//	STR_INVITE_SENT
// 					return;
// 				}
//			}

			OBJID	idTarget = m_pInfo->idPlayer;

			DEBUG_TRY
			if(pTarget == NULL)
			{
				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
				pUser->SendMsg(this);
				pUser->SendSysMsg("�Է�������,���ڸ�����");//STR_INVITED_NOT_FOUND
				return;
			}

			IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);
			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
			{
				if(pStatus->GetParam1() != pStatusTarget->GetParam1())//��ͬ��Ӫ
				{
					pUser->SendSysMsg("����Է��ڲ�ͬ����Ӫ�У��������");
					return;
				}
			}
			DEBUG_CATCH("CMsgTeam proc invite queryobj by id crash.");
			CTeam * pTeam = pUser->GetTeam();
			//�޶� �Ƚ�������
			if(!pTeam)
			{
				char teamName[64] = {0};//Ĭ��С����
				sprintf(teamName,"%s��С��",pUser->GetName());
				
				if (pUser->CreateTeam())
				{
					pTeam = pUser->GetTeam();
					pTeam->SetTeamName(teamName);

					CMsgTeam msg;
					if(msg.Create(pUser->GetID(),_MSGTEAM_CREATE, pUser->GetMapID()))
						pUser->SendMsg(&msg);

					CMsgPlayer msgPlayer;
					if (msgPlayer.Create(pUser->QueryRole()))
						pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

					//����ʦͽ״̬
					if (pUser->GetTutorManager())
						pUser->GetTutorManager()->BroadCastTutorState();
				}
			}
			if(!pTeam)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"��������ʧ��");
				return ;
			}

			if(pTeam->GetLeader() != pUser->GetID())
			{
				//�Լ����Ƕӳ��������
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�����Ƕӳ�");//"�����Ƕӳ�"
				return ;
			}
			if(pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"���Ķ������������ܷ�������");
				return ;
			}
			if(pTarget->GetTeam() != NULL)
			{
				this->Create(m_pInfo->idPlayer,_MSGTEAM_TARGET_INTEAM);
				pUser->SendMsg(this);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѿ��ж�����");//"�Է��Ѿ��ж����ˡ�"
				return ;
			}
			if (!pTarget->IsAgreeTeamApply())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѵ򿪾ܾ���ӿ���");
				return ;
			}
			if (pTarget->SetInviteUserID(pUser->GetID()))
			{
				CMsgTeam msg;
				if (msg.Create(pUser->GetID(), pUser->GetName(),_MSGTEAM_INVITE,0,m_pInfo->idTarget,m_pInfo->szTargetName))
					pTarget->SendMsg(&msg);

				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�����������ѳɹ�����");
				return ;
// 					this->Create(pTarget->GetID(),pTarget->GetName(),_MSGTEAM_JOINTEAM);
// 					pUser->SendMsg(this);
			}
			else
			{
// 					this->Create(m_pInfo->idTarget,_MSGTEAM_TARGET_INTEAM);
// 					pUser->SendMsg(this);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��������б��������޷�����");
				return ;
			}
			return;
		}
		break;
	case _MSGTEAM_MIXTEAM:
		{
			CUserPtr pTarget = NULL;

			if(m_pInfo->idPlayer != ID_NONE)
				pTarget = UserManager()->GetUser(m_pInfo->idPlayer);
			else
				pTarget = UserManager()->GetUser(m_pInfo->szPlayerName);
			
			if(pTarget == NULL)
			{
				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
				pUser->SendMsg(this);
				pUser->SendSysMsg("�Է�������,���ڸ�����");//STR_INVITED_NOT_FOUND
				return;
			}

			IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);
			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
			{
				if(pStatus->GetParam1() != pStatusTarget->GetParam1())//��ͬ��Ӫ
				{
					pUser->SendSysMsg("����Է��ڲ�ͬ����Ӫ�У��������");
					return;
				}
			}
			CTeam * pTeam1 = pUser->GetTeam();
			CTeam * pTeam2 = pTarget->GetTeam();
			if(pTeam1 && pTeam2)
			{
				this->Create(pUser->GetID(),_MSGTEAM_TARGET_INTEAM);
				pUser->SendMsg(this);
				pUser->SendSysMsg("�Է��ڶ�����");//STR_INVITED_NOT_FOUND
			}
			else if(!pTeam2)
			{
				char teamName[64] = {0};//Ĭ��С����
				sprintf(teamName,"%s��С��",pUser->GetName());
				
				if(!pTeam1)
				{
					if (pUser->CreateTeam())
					{
						pTeam1 = pUser->GetTeam();
						pTeam1->SetTeamName(teamName);
						
						CMsgTeam msg;
						if(msg.Create(pUser->GetID(),_MSGTEAM_CREATE, pUser->GetMapID()))
							pUser->SendMsg(&msg);
						
						CMsgPlayer msgPlayer;
						if (msgPlayer.Create(pUser->QueryRole()))
							pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

						//����ʦͽ״̬
						if (pUser->GetTutorManager())
							pUser->GetTutorManager()->BroadCastTutorState();
					}
				}

				if(!pTeam1)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"��������ʧ��");
					return ;
				}
					
				if(pTeam1->GetLeader() != pUser->GetID())
				{
					OBJID idLeader =	pTeam1->GetLeader();
					CUser *pLeader = UserManager()->GetUser(idLeader);
					if (NULL != pLeader)
					{
						this->Create(m_pInfo->idPlayer, pTarget->GetName(),_MSGTEAM_MEMMBER_APPLY,0,pUser->GetID(),pUser->GetName());
						pLeader->SendMsg(this);
					}
					return;
				}
				if(pTeam1->GetMemberAmount() >= _MAX_TEAMAMOUNT)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"���Ķ������������ܷ�������");
					return ;
				}
					
				if (pTarget->SetInviteUserID(pUser->GetID()))
				{
					CMsgTeam msg;
					if (msg.Create(pUser->GetID(), pUser->GetName(),_MSGTEAM_INVITE,0,m_pInfo->idTarget,m_pInfo->szTargetName))
						pTarget->SendMsg(&msg);
						
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"�����������ѳɹ�����");
					return ;
				}
				else
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��������б��������޷�����");
					return ;
				}
			}
			else
			{
				if (pTeam2->GetMemberAmount() >= _MAX_TEAMAMOUNT)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"����Ҷ�������");//STR_TEAM_FULL
					return;
				}
				
				if (pTeam2->GetMemberApplyAmount() >= _MAX_TEAMAMOUNTOUT)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"�ö���������б�����");
				}
				
				pTeam2->AddMemberApply(pUser->GetID());
				
				IF_NOT(this->Create(pUser->GetID(), (char*)pUser->GetName(), _MSGTEAM_APPLYJOIN, NULL))
					return;

				pUser->SendSysMsg("���������뷢�ͳɹ�");
				pTeam2->BroadcastTeamMsg(this);
			}
		}
		break;
	case _MSGTEAM_ASKINVITE:
		{
			CTeam* pTeam = pUser->GetTeam();
			if (pTeam)
			{
				this->Create(pUser->GetID(), pUser->GetName(),_MSGTEAM_ASKINFO);
				pUser->SendMsg(this);
			}

			int nHasInvite = 0;
			OBJID AllUser[5];
			OBJID InviteUserID = ID_NONE;
			for(int i = 0 ; i < 5 ; i++)
			{
				InviteUserID = pUser->GetInviteUserID(i);
				if(InviteUserID != ID_NONE)
				{
					CUser *pInviteUser = UserManager()->GetUser(InviteUserID);
					if(pInviteUser)
					{
						AllUser[nHasInvite ++] = InviteUserID;
					}
					else
					{
						pUser->ClrInviteUserID(InviteUserID);
					}
				}
			}

			TeamMemberInfo *pInfoIn = NULL;
			if(nHasInvite > 0)
			{
				pInfoIn = new TeamMemberInfo[nHasInvite];
				for(i = 0 ;i < nHasInvite; i ++)
				{
					pInfoIn[i].id = AllUser[i];
				}
			}

			CMsgTeamMember msgTeamMem;
			if(nHasInvite > 0)
			{
				if (!msgTeamMem.Create(_MSG_TEAMMEMBER_ANSWERINVITEINFO,pInfoIn[0]))
					return;
				for(int nn = 1; nn < nHasInvite ; nn ++)
				{
					msgTeamMem.Append(1,pInfoIn[nn]);
				}
			}
			else
			{
				TeamMemberInfo infoMsg;
				infoMsg.id = 0;
				msgTeamMem.Create(_MSG_TEAMMEMBER_ANSWERINVITEINFO,infoMsg);
			}
			
			pUser->SendMsg(&msgTeamMem);

			if (NULL != pInfoIn)
			{
				delete []pInfoIn;
				pInfoIn = NULL;
			}
		}
		break;
	case _MSGTEAM_LEAVE:
		{
			//pUser->AbandonTeamLead();
			pUser->QuitTeam();

			CMsgPlayer msgPlayer;
			if (msgPlayer.Create(pUser->QueryRole()))
				pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

			//����ʦͽ״̬
			if (pUser->GetTutorManager())
				pUser->GetTutorManager()->BroadCastTutorState();
		}
		break;
	case _MSGTEAM_DISMISS:
		{
			pUser->DismissTeam();
		}
		break;

// By Minsky,2010-05-28,ɾ���ô���ʽ������ӷ�ʽ�в����ڶ���Ŀ���

	case _MSGTEAM_KICKOUT:
		{
			DEBUG_TRY 

			CHECK(pUser);
			TeamMemberInfo info;
			ZeroMemory(&info,sizeof(TeamMemberInfo));
			if(pUser->GetTeam())
			{
				pUser->GetTeam()->GetMemberInfo(info,m_pInfo->idPlayer);
				strncpy(m_pInfo->szPlayerName,info.szName,_MAX_NAMESIZE);
			}
			pUser->SetInviteUserID(ID_NONE);
			pUser->KickOutTeamMem(m_pInfo->idPlayer);
			
			CUserPtr pTarget = UserManager()->GetUser(m_pInfo->idPlayer);
			if(pTarget)
			{
				CMsgPlayer msgPlayer;
				if (msgPlayer.Create(pTarget->QueryRole()))
					pTarget->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

				//����ʦͽ״̬
				if (pTarget->GetTutorManager())
					pTarget->GetTutorManager()->BroadCastTutorState();
			}
			else
			{
				if(pUser->GetTeam())
				{
					CMsgTeam msg;
					if (msg.Create(m_pInfo->idPlayer,info.szName,_MSGTEAM_LEAVE ))
						pUser->GetTeam()->BroadcastTeamMsg(&msg);
				}
				pUser->SendMsg(this);
			}
			DEBUG_CATCH3("_MSGTEAM_KICKOUT crash!m_pInfo->idPlayer:%d,PID:%d",m_pInfo->idPlayer,PID);
		}
		break;

// By Minsky,2010-05-28����ʱû��ע��
// 	case _MSGTEAM_NOTALLOWTEAM:
// 		{
// 			pUser->SetIsAllowTeam(m_pInfo->idPlayer);
// 		}
// 		break;

// By Minsky,2010-05-28��ɾ���ô���ʽ������ӷ��������ڶ�����

	case _MSGTEAM_SEARCH_TEAM:
		{
			if(!pUser->GetIsAllowTeam())
			{
				pUser->SendSysMsg("���趨Ϊ�������");
				return ;
			}
			if(pUser->CheckSearchTeam())
			{
				m_pInfo->unAction = _MSGTEAM_SEARCH_BEGIN;
				m_pInfo->idMap    = 2;
				pUser->SendMsg(this);
				
				UserManager()->RoleSearchTeam(pUser);
				
				m_pInfo->unAction = _MSGTEAM_SEARCH_END;
				m_pInfo->idMap    = 2;
				pUser->SendMsg(this);
			}
		}
		break;
	case _MSGTEAM_SEARCH_ROLE:
		{
			if(pUser->CheckSearchRole())
			{
				
				m_pInfo->unAction = _MSGTEAM_SEARCH_BEGIN;
				m_pInfo->idMap    = 1;
				pUser->SendMsg(this);
				
				UserManager()->TeamSearchRole(pUser);
				
				m_pInfo->unAction = _MSGTEAM_SEARCH_END;
				m_pInfo->idMap    = 1;
				pUser->SendMsg(this);
			}
		}
		break;
	case _MSGTEAM_DENY_APPLY:
		{
			char szWord[128] = {0};
			CTeam *pTeam = pUser->GetTeam();
			//sprintf(szWord, "%s�ܾ���������Ķ���", pUser->GetName());
			CUser *pTarget = UserManager()->GetUser(m_pInfo->idPlayer);
			if(!pTarget)
				return;

			IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);
			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
			{
				if(pStatus->GetParam1() != pStatusTarget->GetParam1())//��ͬ��Ӫ
				{
					pUser->SendSysMsg("����Է��ڲ�ͬ����Ӫ�У��������");
					return;
				}
			}
		
			CMsgTeam msg;
			if (msg.Create(pUser->GetID(),pUser->GetName(),_MSGTEAM_DENY_APPLY))
				pTarget->SendMsg(&msg);// ���Է���һ��
			//pTarget->SendSysMsg(szWord);

			if( pTeam)
			{
//				pTeam->DelMember(m_pInfo->idPlayer);
				pTeam->DelMemberApply(m_pInfo->idPlayer);
				if(this->Create(pTarget->GetID(), (char*)pTarget->GetName(), _MSGTEAM_ASKINFO, NULL))
					pTeam->BroadcastTeamMsg(this);
			}
		}
		break;
	case _MSGTEAM_DENY_INVITE:
		{
			char szWord[128] = {0};
			CUser *pTarget = UserManager()->GetUser(m_pInfo->idPlayer);
			if(!pTarget)
				return;
			CTeam *pTeam = pTarget->GetTeam();
			if(!pTeam)
				return;

			
			pUser->ClrInviteUserID(m_pInfo->idPlayer);
			if (pTeam)
			{
				CMsgTeam msg;
				if (msg.Create(pUser->GetID(),pUser->GetName(),_MSGTEAM_DENY_INVITE))
					pTarget->SendMsg(&msg);
			}
		}
		break;
	case _MSGTEAM_ASKINFO:
		{
//			afxDump<<"�յ�һ����ѯ�����Ա��Ӧ���!"<<"\n";
			int nStep = 0;
			DEBUG_TRY
			CMsgTeamMember msgTeamMem;
			TeamMemberInfo sendInfo;
			sendInfo.id = ID_NONE;
			if (!msgTeamMem.Create(_MSG_TEAMMEMBER_ANSWERINFO,sendInfo))
				return;
			nStep = 1;
			CTeam* pTeam = pUser->GetTeam();
			if (NULL == pTeam)
			{
				nStep = 2;
				if(m_pInfo->idPlayer == ID_NONE)
					return;
				if(m_pInfo->idPlayer != pUser->GetID())
				{
					if(pUser->IsInviteUserID(m_pInfo->idPlayer))
					{
						CUser *pInviteUser = UserManager()->GetUser(m_pInfo->idPlayer);
						if(pInviteUser)
							pTeam = pInviteUser->GetTeam();
					}
				}
			}

			nStep = 3;
			if(!pTeam)
			{
				pUser->SendMsg(&msgTeamMem);
				return;
			}

			nStep = 4;
			pTeam->UpdateMemberInfo();
			
			pTeam->GetMemberInfo(sendInfo,pTeam->GetLeader());
			if(sendInfo.id != pTeam->GetLeader())
			{
				LogSave("_MSGTEAM_ASKINFO ����!�����г�Ա�б��ж�ʧ�ӳ���Ϣ,leadid:%d",pTeam->GetLeader());
				return;
			}

			nStep = 5;
			msgTeamMem.Append(1,sendInfo);


			for(int k = 0 ; k < 6 ; k ++)
			{
				if(pTeam->m_TeamIn[k].id != ID_NONE   && pTeam->m_TeamIn[k].id != pTeam->GetLeader())
				{
					pTeam->GetMemberInfo(sendInfo,pTeam->m_TeamIn[k].id);
					if(sendInfo.id != pTeam->m_TeamIn[k].id)
						return;					
					msgTeamMem.Append(1,sendInfo);
				}
			}
			
			nStep = 6;
			int nAmountApply = pTeam->GetMemberApplyAmount();
			TeamApplyInfo *pInfoOut = NULL;
			if (nAmountApply > 0)
			{
				pInfoOut = new TeamApplyInfo[nAmountApply];
			}

			nStep = 7;
			if ( pInfoOut)
			{
				int nCur = 0;
				for (int j=0; j<nAmountApply; j++)
				{
					OBJID idMember = pTeam->GetMemberApplyByIndex(j);
					CUser* pMember = UserManager()->GetUser(idMember);
						
					if(pMember && !pMember->GetTeam())
					{
						pInfoOut[nCur].idPlayer			= pMember->GetID();
						pInfoOut[nCur].dwLookFace	= pMember->GetLookFace();
						pInfoOut[nCur].usLev      = pMember->GetLev();
						pInfoOut[nCur].usPro = pMember->GetProfession();
						pInfoOut[nCur].usProLev = pMember->GetJobLev();
						strcpy(pInfoOut[nCur].szPlayerName, pMember->GetName());
//						pInfoOut[nCur].idMap    = pMember->GetMapID();

						nCur++;
					}
				}

				nAmountApply = nCur;
			}

			nStep = 8;
			
			for(int n = 0 ; n < nAmountApply ; n ++)
			{
				sendInfo.id = pInfoOut[n].idPlayer;
				sendInfo.dwLookFace = pInfoOut[n].dwLookFace;
				sendInfo.dwLev = pInfoOut[n].usLev;
				sendInfo.usProfession = pInfoOut[n].usPro;
				sendInfo.usProLev = pInfoOut[n].usProLev;
				SafeCopy(sendInfo.szName, pInfoOut[n].szPlayerName,_MAX_NAMESIZE);
				msgTeamMem.Append(2,sendInfo);
			}

			if(pInfoOut)
			{
				delete[] pInfoOut;
			}

			nStep = 9;
			pUser->SendMsg(&msgTeamMem);
			DEBUG_CATCH2("_MSGTEAM_ASKINFO error! step = %u",nStep)
//			afxDump<<"����һ����ѯ�����Ա��Ӧ���!"<<"\n";
		}
		break;
	case _MSGTEAM_MEMMBER_APPLY:
		{
			CTeam *pTeam = NULL;
			pTeam = pUser->GetTeam();
			if (NULL != pTeam)
			{			
			    if (pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"����Ҷ�������");//STR_TEAM_FULL
					return;
				}

			   OBJID idLeader =	pTeam->GetLeader();
			   CUser *pLeader = UserManager()->GetUser(idLeader);
			   if (NULL != pLeader)
			   {
				   IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
				   IStatus* pStatusTarget = pLeader->QueryStatus(STATUS_WARGAME);
				   if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
				   {
					   if(pStatus->GetParam1() != pStatusTarget->GetParam1())//��ͬ��Ӫ
					   {
						   pUser->SendSysMsg("����Է��ڲ�ͬ����Ӫ�У��������");
						   return;
					   }
				   }
				   if (!pLeader->IsAgreeTeamApply())
				   {
					   pUser->SendSysMsg(_TXTATR_TOPSHOW,"�ӳ��Ѵ򿪾ܾ���ӿ���");//STR_TEAM_FULL
					   return;
				   }
				   CUser *pThisUser = UserManager()->GetUser(m_pInfo->idPlayer);
				   if ( pThisUser && pUser)
				   {
					   this->Create(m_pInfo->idPlayer, pThisUser->GetName(),_MSGTEAM_MEMMBER_APPLY,0,pUser->GetID(),pUser->GetName());
					   pLeader->SendMsg(this);
				   }
			   }
			}
		}
		break;
	case _MSGTEAM_DENY_MEMMBERAPPLY:
		{
			char szWord[128] = {0};
			CUser *pTarget = UserManager()->GetUser(m_pInfo->idPlayer);
			if(!pTarget)
				return;
			CTeam *pTeam = pTarget->GetTeam();
			if(!pTeam)
				return;
			
			sprintf(szWord, "%s�ܾ������������", pUser->GetName());
			pTarget->SendSysMsg(_TXTATR_TOPSHOW,szWord);
		}
		break;
	default:
		pUser->SendSysMsg("�����ڵ������Ϣ������Ϸ�ܰ�Ȩ������������Ŀͻ���");
		break;
		//---------------------------------------------
	}
	DEBUG_CATCH3("CMsgTeam %u %u",m_pInfo->unAction,nStep)
}
