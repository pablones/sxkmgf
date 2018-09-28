// MsgFriend.cpp: interface for the CMsgFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgFriend.h"
//#include "../MapGroup.h"
#pragma	warning(disable:4786)
//#include "MsgFriend.h"
//#include "MsgPlayer.h"
//#include "I_MessagePort.h"
//#include "MsgUserAttrib.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgFriend::CMsgFriend()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgFriend::~CMsgFriend()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFriend::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_FRIEND != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
//20061012����.������������
BOOL	CMsgFriend::Create	(int nAction, 
				 OBJID idFriend, 
				 LPCTSTR szName, 
				 short bOnline/* = false*/, 
				 DWORD nLookFace/*=0*/, 
				 int nLevel/*=0*/,
				 unsigned int profession/*ְҵ=0*/,
				 unsigned int friendship/*�����=0*/,
				 unsigned int nSex,
				 unsigned int nFriendType,
				 LPCTSTR szFeel,//[2010-06-05 goto]���Ӻ�������
				 int nRelation,//[2010-06-05 goto]��ϵ,ʦͽ������...
				 LPCTSTR szFaction,//����
				 LPCTSTR szTitle,//�ƺ�
				 LPCTSTR szMateName,//��ż
				 LPCTSTR szTeamName,//����
				 OBJID idMap//λ��
				 )
{
	//20070528����:��ǿ��־
	//CHECKF(idFriend && nAction && szName);
	//IF_NOT(idFriend && nAction && szName)
	if(nAction && szName)
	{
		char szLog[512];
		sprintf(szLog,"nAction:%d,idFriend:%d.",nAction,idFriend);
		return false;
	}
	
	// init
	this->Init();
	// fill info now
	m_pInfo->idFriend		= idFriend;
	m_pInfo->dwLookFace		= nLookFace;
	m_pInfo->ucAction		= nAction;
	m_pInfo->ucOnlineFlag	= bOnline;
	m_pInfo->ucLevel		= nLevel;
	//20061012����.������������
	//------------------------------
	m_pInfo->ucProfession	= profession;
	m_pInfo->ucFriendship	= friendship;
	m_pInfo->ucSex          = nSex;
	//------------------------------
	m_pInfo->ucFriendType	= nFriendType;
	m_pInfo->ucRelation		= nRelation;
//	strcpy(m_pInfo->szName, szName, _MAX_NAMESIZE);

	BOOL bSucMake	=true;
	if(szFeel)//[2010-06-05 goto]��������
		bSucMake &= m_StrPacker.AddString(szFeel);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(szFaction)//����
		bSucMake &= m_StrPacker.AddString(szFaction);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(szTitle)//�ƺ�
		bSucMake &= m_StrPacker.AddString(szTitle);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(szMateName)//��ż
		bSucMake &= m_StrPacker.AddString(szMateName);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(szTeamName)//����
		bSucMake &= m_StrPacker.AddString(szTeamName);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(idMap)//λ��
	{
		char szMap[10];
		sprintf(szMap,"%u",idMap);
		bSucMake &= m_StrPacker.AddString(szMap);
	}
	else
		bSucMake &= m_StrPacker.AddString("δ֪");

	m_unMsgType	=_MSG_FRIEND;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return bSucMake;
}

//////////////////////////////////////////////////////////////////////
// void CMsgFriend::Process(void *pInfo)
// {
// #ifdef _MSGDEBUG
// 	::LogMsg("Process CMsgFriend, id:%u", m_pInfo->id);
// #endif
// 
// 	char szDebugAction[32];
//     char szDebugStep[32];
//    try
//    {
// 	   sprintf(szDebugAction,"ucAction:%d ",m_pInfo->ucAction);
// 
// 
// 	CUserPtr pUser = UserManager()->GetUser(this);
// 	if (!pUser)
// 	{
// 		if(IsNpcMsg())
// 			return;
// 
// 		// TransmitMsg
// 		switch(m_pInfo->ucAction)
// 		{
// 		case _FRIEND_GETINFO:
// 			{
// 				OBJID idFriend = m_pInfo->idFriend;
// 				CUser* pTarget = UserManager()->GetUser(idFriend);
// 				if(pTarget)
// 				{
// 					sprintf(szDebugStep,"%d",1);
// 					//20061020����.����ְҵ----------------
// 					IF_OK(Create(_FRIEND_ONLINE, 
// 						idFriend, 
// 						pTarget->GetName(), 
// 						ONLINE_STATUS, 
// 						pTarget->GetLookFace(), 
// 						pTarget->GetLev(),
// 						pTarget->GetProfession(),
// 						0,
// 						pTarget->GetSex() ,m_pInfo->ucFriendType))
// 					//IF_OK(Create(_FRIEND_ONLINE, idFriend, pTarget->GetName(), ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev()))
// 						SendMsg(this);
// 					//-------------------------------------
// 					sprintf(szDebugStep,"%d",2);
// 				}
// 			}
// 			break;
// 		case _ENEMY_ADD:
// 			return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 			{
// 				OBJID idFriend = m_pInfo->idFriend;
// 				CUser* pTarget = UserManager()->GetUser(idFriend);
// 				if(pTarget)
// 				{
// 					sprintf(szDebugStep,"%d",1);
// 					IF_OK(Create(_ENEMY_ONLINE, idFriend, m_pInfo->szName, ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev(), pTarget->GetProfession(), 0, pTarget->GetSex()))
// 						SendMsg(this);
// 					sprintf(szDebugStep,"%d",2);
// 				}
// 			}
// 			break;
// 		case _FRIEND_BREAK:
// 			return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 			{
// 				OBJID idFriend = m_pInfo->idFriend;
// 				CUser* pTarget = UserManager()->GetUser(idFriend);
// 				if(pTarget)
// 				{
// 					sprintf(szDebugStep,"%d",1);
// 					pTarget->DelFriend(m_pInfo->idFriend);
// 					sprintf(szDebugStep,"%d",2);
// 					if (pUser)
// 						pUser->DelBeFriend(pTarget->GetID());//�������[2008/12/17]
// 					sprintf(szDebugStep,"%d",3);
// 				}
// 			}
// 			break;
// 		}
// 
// 		return;
// 	}
// 
// // 	if (!pUser->IsAlive())
// // 	{
// // 		pUser->SendSysMsg(g_objStrRes.GetStr(12069));//STR_DIE
// // 		return;
// // 	}
// 
// 	switch(m_pInfo->ucAction)
// 	{
// 	case _FRIEND_CONTACTHISTORY_APPLY:
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			CUserPtr	pTarget = NULL;
// 			sprintf(szDebugStep,"%d",1);
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if (!pTarget)
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(28023));
// 				return;
// 			}
// 			//---------������ӻ���½�����¼------------------------------------------------------------------------------------------
// 			CFriendContactHistory* pFriend= pUser->GetFriendContact(pTarget->GetID(),m_pInfo->ucFriendType);
// 			sprintf(szDebugStep,"%d",2);
// 			if ( pFriend  )
// 			{
// 				sprintf(szDebugStep,"%d",3);
// 				if (pUser->UpDateContactHistory(pFriend,pTarget->GetID(),time(NULL),m_pInfo->ucFriendType))//���Ľ���ʱ��
// 				{
// 					if(this->Create(_FRIEND_CONTACTHISTORY_GETINFO, 
// 							pTarget->GetID(), 
// 							pTarget->GetName(), 
// 							ONLINE_STATUS, 
// 							pTarget->GetLookFace(), 
// 							pTarget->GetLev(),
// 							pTarget->GetProfession(),
// 							0,
// 							pTarget->GetSex(),
// 							m_pInfo->ucFriendType) )
// 							pUser->SendMsg(this);
// 				}
// 				sprintf(szDebugStep,"%d",4);
// 			}
// 			else
// 			{
// 				sprintf(szDebugStep,"%d",5);
// 				if ( pUser->AddFriendContact(pTarget->GetID(),m_pInfo->szName,m_pInfo->ucFriendType) )//��ӽ�����¼
// 				{
// 					sprintf(szDebugStep,"%d",6);
// 					if(this->Create(_FRIEND_CONTACTHISTORY_GETINFO, 
// 						pTarget->GetID(), 
// 						pTarget->GetName(), 
// 						ONLINE_STATUS, 
// 						pTarget->GetLookFace(), 
// 						pTarget->GetLev(),
// 						pTarget->GetProfession(),
// 						0,
// 						pTarget->GetSex(),
// 						m_pInfo->ucFriendType) )
// 						pUser->SendMsg(this);
// 					
// 					sprintf(szDebugStep,"%d",7);
// 					CFriendContactHistory* pFriend1 = pUser->GetFriendContact(pTarget->GetID(),m_pInfo->ucFriendType);
// 
// 					if (pFriend1)
// 					{
// 						sprintf(szDebugStep,"%d",8);
// 						pTarget->AddBeFriendContact(pFriend1->GetID(),pUser->GetID(),m_pInfo->ucFriendType);																		
// 					}					
// 				}
// 			}
// 
// 			//----------�Է���ӻ���½�����¼--------------------------------------------------------------------------
// 
// 			sprintf(szDebugStep,"%d",9);
// 			pFriend = pTarget->GetFriendContact(pUser->GetID(),m_pInfo->ucFriendType);
// 			if (pFriend)
// 			{
// 				sprintf(szDebugStep,"%d",10);
// 				if (pTarget->UpDateContactHistory(pFriend,pUser->GetID(),time(NULL) ,m_pInfo->ucFriendType))
// 				{
// 					sprintf(szDebugStep,"%d",11);
// 					if(this->Create(_FRIEND_CONTACTHISTORY_GETINFO, 
// 						pUser->GetID(), 
// 						pUser->GetName(), 
// 						ONLINE_STATUS, 
// 						pUser->GetLookFace(), 
// 						pUser->GetLev(),
// 						pUser->GetProfession(),
// 						0,
// 						pUser->GetSex(),
// 						m_pInfo->ucFriendType) )
// 						pTarget->SendMsg(this);
// 					sprintf(szDebugStep,"%d",12);
// 				}
// 			}
// 			else
// 			{
// 				sprintf(szDebugStep,"%d",13);
// 				if ( pTarget->AddFriendContact(pUser->GetID(),pUser->GetName(),m_pInfo->ucFriendType) )//��ӽ�����¼
// 				{
// 					sprintf(szDebugStep,"%d",14);
// 					if(this->Create(_FRIEND_CONTACTHISTORY_GETINFO, 
// 						pUser->GetID(), 
// 						pUser->GetName(), 
// 						ONLINE_STATUS, 
// 						pUser->GetLookFace(), 
// 						pUser->GetLev(),
// 						pUser->GetProfession(),
// 						0,
// 						pUser->GetSex(),
// 						m_pInfo->ucFriendType) )
// 						pTarget->SendMsg(this);	
// 
// 					sprintf(szDebugStep,"%d",15);
// 					CFriendContactHistory* pFriend1 = pTarget->GetFriendContact(pUser->GetID(),m_pInfo->ucFriendType);
// 					if (pFriend1)
// 					{
// 						sprintf(szDebugStep,"%d",16);
// 						pUser->AddBeFriendContact(pFriend1->GetID(),pTarget->GetID(),m_pInfo->ucFriendType);
// 					}			
// 				}
// 			}
// 		}
// 		break;
// 	case _FRIEND_ADDFRIEND://��A���͵�����  [2010-06-07 goto]����ҪB��ͬ�⣬ֱ�Ӽ�Ϊ���ѣ�ͬʱ֪ͨB
// 		{
// 			if (pUser->GetID()==m_pInfo->idFriend || stricmp(pUser->GetName(),m_pInfo->szName)==0)
// 				return;
// 			if( m_pInfo->ucFriendType>MAX_GROUP || m_pInfo->ucFriendType<=0 )//���ѷ���,1-4�飬5��ʱ���ѣ�6������
// 				return;
// 
// 			if(m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP && pUser->GetFriendGroupAmount(m_pInfo->ucFriendType)>=MAX_GROUP_FRIEND )
// 			{
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
// 					pUser->SendMsg(&pMsg);
// 
// 				pUser->SendSysMsg("�˺��ѷ����Ѵﵽ��������");
// 				return;
// 			}
// 
// 			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
// 			if (pFriend)//B�Ѿ���A�ĺ���
// 			{
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
// 					pUser->SendMsg(&pMsg);
// 				char szTemp[128];
// 				sprintf(szTemp,"%s�Ѿ�����ĺ���",m_pInfo->szName);
// 				pUser->SendSysMsg(szTemp);
// 				return;
// 			}
// 
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if(pTarget)
// 			{
// 				CMsgFriend pMsg;
// 				if (pUser->AddFriend(pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType,pTarget->GetMate()))
// 				{
// 					if (pMsg.Create(_FRIEND_ADDSUCCESS,
// 								 pTarget->GetID(),
// 								 m_pInfo->szName,
// 								 ONLINE_STATUS,
// 								 pTarget->GetLookFace(),
// 								 pTarget->GetLev(),
// 								 pTarget->GetProfession(),
// 								 0,//friendship
// 								 pTarget->GetSex(),
// 								 m_pInfo->ucFriendType,//frinedgroup
// 								 pTarget->GetFeel(),
// 								 0))//relation
// 					pUser->SendMsg(&pMsg);
// 					if (m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP)
// 					{
// 						char szTemp[128];
// 						sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",m_pInfo->szName);
// 						pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
// 					}
// 				}
// 				else
// 				{
// 					pMsg.Create(_FRIEND_ADDFAIL,pTarget->GetID(),m_pInfo->szName,0,0,0,0,0,0,0,NULL,0);
// 					pUser->SendMsg(&pMsg);
// 					return;
// 				}
// 				if ( pTarget->AddBeFriend(0,pUser->GetID(),m_pInfo->ucFriendType) && m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP)//���A�Ѿ���B�ĺ��ѣ�����Ҫ��֪ͨB
// 				{
// 					if (pTarget->GetFriend( pUser->GetID()))
// 					{
// 						char szTemp[128];
// 						sprintf(szTemp,"<font color=\"#ffff00\">�û�</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">�����Ϊ�˺���!</font>",pUser->GetName());
// 						pTarget->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
// 					}
// 					else
// 					{
// 						pMsg.Create(_FRIEND_ADDFRIEND,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,NULL,0);
// 						pTarget->SendMsg(&pMsg);
// 					}
// 				}
// 			}
// 			else
// 			{
// 				CMsgFriend pFriendMsg;
// 				OBJID dwFriendID = pUser->AddLeaveFriend(m_pInfo->szName, m_pInfo->ucFriendType, &pFriendMsg);
// 				if(dwFriendID)
// 				{
// 					pUser->SendMsg(&pFriendMsg);
// 					if (m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP)
// 					{
// 						char szTemp[128];
// 						sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",m_pInfo->szName);
// 						pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
// 					}
// 				}
// 				else
// 				{
// 					CMsgFriend pMsg;
// 					if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
// 						pUser->SendMsg(&pMsg);
// 				}
// 			}
// 		}
// 		break;
// 	case _FRIEND_CHANGEGROUP://[2010-06-07 goto]�������ѷ���
// 		{
// 			if( m_pInfo->ucFriendType>(MAX_GROUP+1) || m_pInfo->ucFriendType<=0 )//���ѷ���,1-4�飬5��ʱ���ѣ�6������
// 				return;
// 			
// 			if( pUser->GetFriendGroupAmount(m_pInfo->ucFriendType)>=MAX_GROUP_FRIEND )
// 			{
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_CHANGEFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
// 					pUser->SendMsg(&pMsg);
// 				
// 				pUser->SendSysMsg("�˺��ѷ����Ѵﵽ��������");
// 				return;
// 			}
// 
// 			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
// 			if(!pFriend)
// 				return;
// 			
// 			unsigned int uoldType = pFriend->GetFriendType();
// 			//pFriend->SetFriendType(m_pInfo->ucFriendType);//�޸ĺ��ѷ���
// 			if( pUser->MoveFriend(pFriend,m_pInfo->ucFriendType) )//�޸ĺ��ѷ���
// 			{
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_CHANGEGROUP,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,uoldType,0,m_pInfo->ucFriendType,NULL,0))
// 					pUser->SendMsg(&pMsg);
// 			}
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if(pTarget)
// 			{
// 				CFriend* pTargetFriend = pTarget->GetBeFriend(pUser->GetID());
// 				pTargetFriend->SetFriendType(m_pInfo->ucFriendType);
// 			}
// 		}
// 		break;
// 	case _FRIEND_DELFRIEND://[2010-06-08 goto]ɾ������
// 		{
// 			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
// 			if (!pFriend)
// 				return;
// 
// 			OBJID nFriendID=pFriend->GetFriendID();
// 			if(pUser->DelFriend(pFriend->GetFriendID()))
// 			{
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_DELFRIEND,nFriendID,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
// 					pUser->SendMsg(&pMsg);
// 			}
// 			else
// 			{
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_DELFRIENDFAIL,pFriend->GetFriendID(),pFriend->GetFriendName(),0,0,0,0,0,0,0,NULL,0))
// 					pUser->SendMsg(&pMsg);
// 				return;
// 			}
// 
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if (pTarget)
// 			{
// 				pTarget->DelBeFriend(pUser->GetID());
// 			}
// 		}
// 		break;
// 	case _FRIEND_SETFEEL://[2010-06-08 goto]�޸�����
// 		{
// 			if(m_pInfo->szName)
// 			{
// 				if(strlen(m_pInfo->szName)>_MAX_FEELSIZE)
// 					return;
// 
// 				pUser->SetFeel(m_pInfo->szName);
// 
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_SETFEEL,pUser->GetID(),pUser->GetName(),ONLINE_STATUS,0,0,0,0,0,0,m_pInfo->szName,0))
// 					pUser->SendMsg(&pMsg);
// 
// 				CMsgPlayer msgPlayer;
// 				if (msgPlayer.Create(pUser->QueryRole()))
// 					pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
// 			}
// 		}
// 		break;
// 	case _FRIEND_GETFRIENDINFO://��ȡ���ѵ���ϸ��Ϣ
// 		{
// 			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 
// 			if (!pFriend)//��ʱ����
// 			{
// 				if(!pTarget)
// 				{
// // 					CMsgFriend pMsg;
// // 					pMsg.Create(_FRIEND_GETFRIENDINFO,m_pInfo->idFriend,m_pInfo->szName,
// // 						0,//��ְҵ
// // 						0,//lookface
// // 						0,//����ȼ�
// // 						0,//��ְҵ
// // 						0,
// // 						0,//��ְҵ�ȼ�
// // 						0,//��ְҵ�ȼ�
// // 						NULL,
// // 						0);//��ϵ
// // 					pUser->SendMsg(&pMsg);
// 					pUser->SendSysMsg(_TXTATR_TOPSHOW, "���˲����ߣ����ڸ�����");
// 				}
// 				else
// 				{
// 					char szUserTitle[10];
// 					sprintf(szUserTitle, "%u", pTarget->GetUseTitle());
// 					
// 					char szTeamName[32] = "��";
// 					CTeam* pTeam = pTarget->GetTeam();
// 					if(pTeam)
// 						sprintf(szTeamName,pTeam->GetTeamName());
// 
// 					CSyndicate* pSyndicate = pTarget->GetSyndicate();
// 					
// 					CMsgFriend pMsg;
// 					if (pMsg.Create(_FRIEND_GETFRIENDINFO,
// 						pTarget->GetID(),
// 						pTarget->GetName(),
// 						pTarget->GetInfo()->SecJob,//��ְҵ
// 						pTarget->GetLookFace(),//lookface
// 						pTarget->GetLev(),//����ȼ�
// 						pTarget->GetInfo()->FirJob,//��ְҵ
// 						0,//�Ѻö�
// 						pTarget->GetInfo()->FirJobLev,//��ְҵ�ȼ�
// 						pTarget->GetInfo()->SecJobLev,//��ְҵ�ȼ�
// 						pTarget->GetFeel(),
// 						0,//��ϵ
// 						pSyndicate==NULL? "��" : pSyndicate->GetStr(SYNDATA_NAME),//����
// 						szUserTitle,//�ƺ�
// 						pTarget->GetMate(),//��ż
// 						szTeamName,//����
// 						pTarget->GetMapID()//λ��
// 						))
// 					pUser->SendMsg(&pMsg);
// 				}
// 				return;
// 			}
// 			if (!pTarget)//������ʱ������Ϣ�޷���ȡ
// 			{
// 				char szUserTitle[10];
// 				sprintf(szUserTitle, "%u",pFriend->GetFriendTitle());
// 				OBJID idSyn = pFriend->GetFriendSynID();
// 				char szSyn[16] = "��";
// 				if (idSyn!=ID_NONE)
// 				{
// 					CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// 					if (pSyn)
// 						sprintf(szSyn, pSyn->GetStr(SYNDATA_NAME));
// 				}
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_GETFRIENDINFO,
// 							pFriend->GetFriendID(),
// 							pFriend->GetFriendName(),
// 							pFriend->GetFriendSecJob(),//��ְҵ
// 							pFriend->GetFriendLookFace(),//lookface
// 							pFriend->GetFriendLevel(),//����ȼ�
// 							pFriend->GetFriendFirJob(),//��ְҵ
// 							pFriend->GetFriendShip(),
// 							pFriend->GetFriendFirJobLev(),//��ְҵ�ȼ�
// 							pFriend->GetFriendSecJobLev(),//��ְҵ�ȼ�
// 							pFriend->GetFriendFeel(),
// 							0,//��ϵ
// 							szSyn,//"����",
// 							szUserTitle,
// 							pFriend->GetFriendMateName()))
// 				pUser->SendMsg(&pMsg);
// 			}
// 			else
// 			{
// 				char szUserTitle[10];
// 				sprintf(szUserTitle, "%u", pTarget->GetUseTitle());
// 				
// 				char szTeamName[32] = "��";
// 				CTeam* pTeam = pTarget->GetTeam();
// 				if(pTeam)
// 					sprintf(szTeamName, pTeam->GetTeamName());
// 
// 				CSyndicate* pSyndicate = pTarget->GetSyndicate();
// 
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_GETFRIENDINFO,
// 							pTarget->GetID(),
// 							pTarget->GetName(),
// 							pTarget->GetProfession(2),//��ְҵ
// 							pTarget->GetLookFace(),//lookface
// 							pTarget->GetLev(),//����ȼ�
// 							pTarget->GetProfession(2),//��ְҵ
// 							pFriend->GetFriendShip(),
// 							pTarget->GetJobLev(1),//��ְҵ�ȼ�
// 							pTarget->GetJobLev(2),//��ְҵ�ȼ�
// 							pTarget->GetFeel(),
// 							0,//��ϵ
// 							pSyndicate==NULL? "��" : pSyndicate->GetStr(SYNDATA_NAME),//����
// 							szUserTitle,//�ƺ�
// 							pTarget->GetMate(),//��ż
// 							szTeamName,//����
// 							pTarget->GetMapID()//λ��
// 							))
// 				pUser->SendMsg(&pMsg);
// 			}
// 		}
// 		break;
// 	case _FRIEND_DELCONTACT://ɾ���Ѿ�����������
// 		{
// 			if(pUser->GetID()!=m_pInfo->idFriend)
// 				return;
// 			pUser->DelFriendLeaveWord(pUser->GetName(),m_pInfo->dwLookFace);
// 		}
// 		break;
// 	case _FRIEND_HAVETEAM://�鿴���Ѷ�����Ϣ
// 		{
// 			CUserPtr pFriend = UserManager()->GetUser(m_pInfo->idFriend);
// 			if (!pFriend)
// 			{
// 				CMsgFriend msg;
// 				if (msg.Create(_FRIEND_HAVETEAM,0,pUser->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// 				{
// 					pUser->SendMsg(&msg);
// 				}
// 				return;
// 			}
// 
// 			int nStep = 1;
// 			DEBUG_TRY
// 			CTeam* pTeam = pFriend->GetTeam();
// 			nStep = 2;
// 			if (!pTeam)//�����޶���
// 			{
// 				nStep = 3;
// 				CMsgFriend msg;
// 				if (msg.Create(_FRIEND_HAVETEAM,0,pFriend->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// 					pUser->SendMsg(&msg);
// 			}
// 			else//�����ж���
// 			{
// 				nStep = 4;
// 				OBJID idTeam = pTeam->GetID();
// 				OBJID idTeamLeader = pTeam->GetLeader();
// 				LPCTSTR szTeam = pTeam->GetTeamName();
// 
// 				nStep = 5;
// 				CMsgFriend msg;
// 				if (msg.Create(_FRIEND_HAVETEAM,idTeam,szTeam,0,idTeamLeader,0,0,0,0,0))
// 					pUser->SendMsg(&msg);
// 			}
// 			DEBUG_CATCH2("_FRIEND_HAVETEAM %u",nStep);
// 		}
// 		break;
// 	case _FRIEND_HAVETEAM_MT://�����̲߳�����ҵķ��ؽ��
// 		{
// // 			CUserPtr pFriend = UserManager()->GetUser(m_pInfo->idFriend);
// // 			if (!pFriend)
// // 			{
// // 				CMsgFriend msg;
// // 				if (msg.Create(_FRIEND_HAVETEAM,0,pUser->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// // 					pUser->SendMsg(&msg);
// // 				return;
// // 			}
// // 			
// // 			CTeam* pTeam = pFriend->GetTeam();
// // 			if (!pTeam)//�����޶���
// // 			{
// // 				CMsgFriend msg;
// // 				if (msg.Create(_FRIEND_HAVETEAM,0,pFriend->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// // 					pUser->SendMsg(&msg);
// // 			}
// // 			else//�����ж���
// // 			{
// // 				OBJID idTeam = pTeam->GetID();
// // 				OBJID idTeamLeader = pTeam->GetLeader();
// // 				LPCTSTR szTeam = pTeam->GetTeamName();
// // 				CMsgFriend msg;
// // 				msg.Create(_FRIEND_HAVETEAM,idTeam,szTeam,0,idTeamLeader,0,0,0,0,0);
// // 				pUser->SendMsg(&msg);
// // 			}
// 		}
// 		break;
// 	case _FRIEND_FEEL_SWITH:
// 		{
// 			pUser->SetFeelSwitch(m_pInfo->idFriend);
// 			CMsgUserAttrib msg;
// 			msg.Create(pUser->GetID(), _USERATTRIB_FEEL_SWITH, m_pInfo->idFriend);
// 			pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 		}
// 		break;
// 	case _FRIEND_MOVETOGOODFRIENDACCEPT://Bͬ��A������
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			if (m_pInfo->ucFriendType != 1)
// 				return;
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if (!pTarget)
// 				return;
// 			sprintf(szDebugStep,"%d",1);
// 			CFriend* pBeFriend = pTarget->GetFriend(pUser->GetID());//�ұ�Ȼ�Ѿ�����ĳ�ֺ���
// 			if (!pBeFriend)
// 				return;
// 			CFriend* pFriend= pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID());//���Ƿ�Ҳ���ҵ�ĳ�ֺ���
// 			sprintf(szDebugStep,"%d",2);
// 			if (!pFriend)//����
// 			{
// 				sprintf(szDebugStep,"%d",3);
// 				if (pUser->AddFriend(/*m_pInfo->idFriend*/pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType)
// 					&& pTarget->MoveFriend(pBeFriend,1))
// 				{
// 					sprintf(szDebugStep,"%d",4);
// 					//-----A����BΪ����,����B�б���A����Ϣ----------
// 					pFriend = pUser->GetFriend(pTarget->GetID()/*m_pInfo->idFriend�����m_pInfo->idFriend����Ϊ��*/);
// 					pTarget->AddBeFriend(pFriend->GetID(),pUser->GetID(),pFriend->GetFriendType());
// 					sprintf(szDebugStep,"%d",5);
// 					//----------------------------
// 					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
// 						pTarget->GetID(), 
// 						pTarget->GetName(), 
// 						ONLINE_STATUS, 
// 						pTarget->GetLookFace(), 
// 						pTarget->GetLev(),
// 						pTarget->GetProfession(),
// 						0,
// 						pTarget->GetSex(),
// 						m_pInfo->ucFriendType) )
// 						pUser->SendMsg(this);
// 					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
// 						pUser->GetID(), 
// 						pUser->GetName(), 
// 						ONLINE_STATUS, 
// 						pUser->GetLookFace(), 
// 						pUser->GetLev(),
// 						pUser->GetProfession(),
// 						0,
// 						pUser->GetSex(),
// 						m_pInfo->ucFriendType) )
// 						pTarget->SendMsg(this);
// 					MSGBUF	szMsg;
// 						sprintf(szMsg,g_objStrRes.GetStr(13057), pUser->GetName(), pTarget->GetName());//STR_MAKE_FRIEND
// 						pUser->BroadcastRoomMsg(szMsg, true);
// 						sprintf(szDebugStep,"%d",6);
// 				}
// 				sprintf(szDebugStep,"%d",7);
// 			}
// 			else//��
// 			{
// 				sprintf(szDebugStep,"%d",8);
// 				if (pUser->MoveFriend(pFriend,1) && pTarget->MoveFriend(pBeFriend,1))
// 				{
// 					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
// 						pTarget->GetID(), 
// 						pTarget->GetName(), 
// 						ONLINE_STATUS, 
// 						pTarget->GetLookFace(), 
// 						pTarget->GetLev(),
// 						pTarget->GetProfession(),
// 						0,
// 						pTarget->GetSex(),
// 						m_pInfo->ucFriendType) )
// 						pUser->SendMsg(this);
// 					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
// 						pUser->GetID(), 
// 						pUser->GetName(), 
// 						ONLINE_STATUS, 
// 						pUser->GetLookFace(), 
// 						pUser->GetLev(),
// 						pUser->GetProfession(),
// 						0,
// 						pUser->GetSex(),
// 						m_pInfo->ucFriendType) )
// 						pTarget->SendMsg(this);
// 					MSGBUF	szMsg;
// 						sprintf(szMsg,g_objStrRes.GetStr(13057), pUser->GetName(), pTarget->GetName());//STR_MAKE_FRIEND
// 						pUser->BroadcastRoomMsg(szMsg, true);
// 				}
// 				sprintf(szDebugStep,"%d",9);
// 			}
// 		}
// 		break;
// 	case _FRIEND_MOVEAPPLY:
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if (!pTarget)
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(28023));
// 				return;
// 			}
// 			sprintf(szDebugStep,"%d",1);
// 			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
// 			if (pFriend == NULL || pFriend->GetFriendType() == m_pInfo->ucFriendType)
// 				return;
// 			if (m_pInfo->ucFriendType == 1)//�������ΪGOOD FRIEND,����Ҫ�����Է�ȷ��,�൱�ڷ�����һ������Ӻ�����Ϣ
// 			{
// 			}
// 			sprintf(szDebugStep,"%d",2);
// 			if (pUser->MoveFriend(pFriend,m_pInfo->ucFriendType))
// 			{
// 				sprintf(szDebugStep,"%d",3);
// 				if(this->Create(_FRIEND_MOVESUCESS, 
// 					pFriend->GetFriendID(), 
// 					pFriend->GetFriendName(), 
// 					pTarget?true:false, 
// 					pTarget?pTarget->GetLookFace():0, 
// 					pTarget?pTarget->GetLev():0,
// 					pTarget?pTarget->GetProfession():0,
// 					0,
// 					pTarget?pTarget->GetSex():0,
// 					m_pInfo->ucFriendType) )
// 						pUser->SendMsg(this);
// 				sprintf(szDebugStep,"%d",4);
// 			}
// 			
// 		}
// 		break;
// 	case _FRIEND_ADDTEMP://�������[2008/12/12]����ʱ����,����Է�ȷ��
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if (!pTarget)
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(28023));
// 				return;
// 			}
// 			else
// 			{
// 				sprintf(szDebugStep,"%d",1);
// 				if( pTarget->QueryEnemy()->GetName(pUser->GetID()))// ����Է�����������������޷�����ӶԷ�Ϊ���� [10/10/2007]
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(16041));//-�Է��Ѿ����������������޷���Ϊ����!
// 					break;
// 				}
// 				sprintf(szDebugStep,"%d",2);
// 				
// 				if(pUser->GetFriendAmount() >= _MAX_USERFRIENDSIZE)
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12073));//STR_FRIEND_LIST_FULL
// 					break;
// 				}
// 				sprintf(szDebugStep,"%d",3);
// 				if(pUser->GetFriendByName(m_pInfo->szName))
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12074));//STR_YOUR_FRIEND_ALREADY
// 					break;
// 				}
// 				sprintf(szDebugStep,"%d",4);
// 				if(!pTarget->GetIsAllowFriend())
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12075));//STR_NOTALLOWFRIEND
// 					break;
// 				}
// 				sprintf(szDebugStep,"%d",5);
// 
// 				if (pUser->AddFriend(pTarget->GetID()/*m_pInfo->idFriend*/, m_pInfo->szName,m_pInfo->ucFriendType) /*&&
// 					pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucFriendType)*/)
// 				{
// 					sprintf(szDebugStep,"%d",6);
// 					//-----A����BΪ����,����B�б���A����Ϣ----------
// 					CFriend* pFriend = pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID());
// 					pTarget->AddBeFriend(pFriend->GetID(),pUser->GetID(),pFriend->GetFriendType());
// 					//----------------------------
// 					sprintf(szDebugStep,"%d",7);
// 					if(this->Create(_FRIEND_GETINFO, 
// 						pTarget->GetID(), 
// 						pTarget->GetName(), 
// 						ONLINE_STATUS, 
// 						pTarget->GetLookFace(), 
// 						pTarget->GetLev(),
// 						pTarget->GetProfession(),
// 						0,
// 						pTarget->GetSex(),
// 						m_pInfo->ucFriendType) )
// 					{
// 						pUser->SendMsg(this);
// 						char szTemp[128];
// 						sprintf(szTemp,"���Ѿ���%sΪ��ʱ����!!!",pTarget->GetName());
// 						pUser->SendSysMsg(szTemp);
// 					}
// 					sprintf(szDebugStep,"%d",7);
// 						/*if(this->Create(_FRIEND_GETINFO, 
// 							pUser->GetID(), 
// 							pUser->GetName(), 
// 							ONLINE_STATUS, 
// 							pUser->GetLookFace(), 
// 							pUser->GetLev(),
// 							pUser->GetProfession(),
// 							0,
// 							pUser->GetSex(),
// 							m_pInfo->ucFriendType) )
// 							pTarget->SendMsg(this);*/
// 						
// 					}
// 		}
// 		break;
// 		}
// 	case _FRIEND_ACCEPT:
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if(!pTarget)
// 			{
// 				if(strlen(m_pInfo->szName) > 1)
// 				{
// 					ST_FRIEND_APPLY st;
// 					memset(&st, 0, sizeof(ST_FRIEND_APPLY));
// 
// 					sprintf(szDebugStep,"%d",1);
// 					if(pUser->FetchApplyName(CUser::APPLY_FRIEND, m_pInfo->szName))
// 					{
// 						sprintf(szDebugStep,"%d",2);
// 						//	�ظ�
// 						BOOL bIsValid = TRUE;
// 						int nRet = 0;
// 						if(pUser->GetFriendAmount() >= _MAX_USERFRIENDSIZE)
// 						{
// 							nRet = 12073;	//STR_FRIEND_LIST_FULL
// 							bIsValid = FALSE;
// 						}
// 						else if(pUser->GetFriendByName(m_pInfo->szName))
// 						{
// 							nRet = 12074;//STR_YOUR_FRIEND_ALREADY
// 							bIsValid = FALSE;
// 						}
// 						else if(!pUser->GetIsAllowFriend())
// 						{
// 							nRet = 12075;//STR_NOTALLOWFRIEND
// 							bIsValid = FALSE;
// 						}
// 
// 						sprintf(szDebugStep,"%d",3);
// 						if(!bIsValid)
// 						{
// 							ST_FRIEND_APPLY st;
// 							memcpy(&st, pInfo, sizeof(ST_FRIEND_APPLY));
// 							st.nRet = nRet;
// 							st.unAction = FRIEND_APPLY_ACTION_TYPE_APPLY_NANK;
// 							MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_FRIENDAPPLY, STRUCT_TYPE(ST_FRIEND_APPLY), (const char*)&st);
// 							return;
// 						}
// 						sprintf(szDebugStep,"%d",4);
// 						st.unAction = FRIEND_APPLY_ACTION_TYPE_APPLY_ANK;	
// 
// 						st.idPlayer = m_pInfo->idFriend;
// 						strcpy(st.szPlayerName, m_pInfo->szName);
// 
// 						st.idTarget = pUser->GetID();
// 						strcpy(st.szTargetName, pUser->GetName());
// 						st.nTargetLev = pUser->GetLev();
// 						st.nTargetLookFace = pUser->GetLookFace();
// 						st.nTargetProfession = pUser->GetProfession();
// 						st.nSex  = pUser->GetSex();
// 						MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_FRIENDAPPLY, STRUCT_TYPE(ST_FRIEND_APPLY), (const char*)&st);
// 						sprintf(szDebugStep,"%d",5);
// 					}
// 				}
// 			}
// 			else
// 			{
// 				sprintf(szDebugStep,"%d",5);
// 				if(pUser->GetFriendAmount() >= _MAX_USERFRIENDSIZE)
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12073));//STR_FRIEND_LIST_FULL
// 					break;
// 				}
// 				
// 				if(pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID()) 
// 					&& pUser->GetFriend(pTarget->GetID())->GetFriendType() == m_pInfo->ucFriendType)
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12074));//STR_YOUR_FRIEND_ALREADY
// 					break;
// 				}
// 				
// 				sprintf(szDebugStep,"%d",6);
// 				if(!pTarget->GetIsAllowFriend())
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12075));//STR_NOTALLOWFRIEND
// 					break;
// 				}
// 				/*
// 				if(m_pInfo->idFriend == 0 && pTarget->FetchApplyName(CUser::APPLY_FRIEND, m_pInfo->szName))
// 				{
// 					pUser->SetApplyName(CUser::APPLY_FRIEND, m_pInfo->szName);
// 					IF_OK(Create(_FRIEND_APPLY, pUser->GetID(), pUser->GetName(), true))
// 						pTarget->SendMsg(this);
// 					
// 					pTarget->SendSysMsg(g_objStrRes.GetStr(13056), pUser->GetName());//STR_TO_MAKE_FRIEND
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12077));//STR_MAKE_FRIEND_SENT
// 				}
// 				else
// 				*/
// 				sprintf(szDebugStep,"%d",7);
// 				if(pTarget->FetchApply(CUser::APPLY_FRIEND) != pUser->GetID())
// 				{
// 					sprintf(szDebugStep,"%d",8);
// 					// ����Է�δ�����Լ������������������
// 					pUser->SetApply(CUser::APPLY_FRIEND, pTarget->GetID());
// 					IF_OK(Create(_FRIEND_APPLY, pUser->GetID(), pUser->GetName(), true))
// 						pTarget->SendMsg(this);
// 					
// 					pTarget->SendSysMsg(g_objStrRes.GetStr(13056), pUser->GetName());//STR_TO_MAKE_FRIEND
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12077));//STR_MAKE_FRIEND_SENT
// 					sprintf(szDebugStep,"%d",9);
// 				}
// 				else
// 				{
// 					// ����Է��Ѿ����Լ�����������ֱ��Ϊ˫���Ӻ���
// 					//------------�������[2008/12/16]�鿴�ҵĺ���Ȧ������û��'�����'---------
// 					sprintf(szDebugStep,"%d",10);
// 					if (pUser->GetFriend(pTarget->GetID() ))//�����
// 					{
// 						sprintf(szDebugStep,"%d",11);
// 						if (pUser->MoveFriend(pUser->GetFriend(pTarget->GetID()),m_pInfo->ucFriendType)//�ͽ����Ƶ�������
// 							&& pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucFriendType))
// 						{
// 							sprintf(szDebugStep,"%d",12);
// 							//-----A����BΪ����,����B�б���A����Ϣ----------
// 							CFriend* pFriend = pTarget->GetFriend(pUser->GetID());
// 							pUser->AddBeFriend(pFriend->GetID(),pTarget->GetID(),pFriend->GetFriendType());
// 							//----------------------------
// 
// 							sprintf(szDebugStep,"%d",13);
// 							if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
// 							pTarget->GetID(), 
// 							pTarget->GetName(), 
// 							ONLINE_STATUS, 
// 							pTarget->GetLookFace(), 
// 							pTarget->GetLev(),
// 							pTarget->GetProfession(),
// 							0,
// 							pTarget->GetSex(),
// 							m_pInfo->ucFriendType) )
// 							pUser->SendMsg(this);//֪ͨ'��'��'�����'MOVE��������
// 						if(this->Create(_FRIEND_GETINFO, 
// 							pUser->GetID(), 
// 							pUser->GetName(), 
// 							ONLINE_STATUS, 
// 							pUser->GetLookFace(), 
// 							pUser->GetLev(),
// 							pUser->GetProfession(),
// 							0,
// 							pUser->GetSex(),
// 							m_pInfo->ucFriendType) )
// 							pTarget->SendMsg(this);//֪ͨ'�����'����Ϊ���ѳɹ�������'��'����Ϣ���͸���
// 						}
// 						MSGBUF	szMsg;
// 						sprintf(szMsg,g_objStrRes.GetStr(13057), pUser->GetName(), pTarget->GetName());//STR_MAKE_FRIEND
// 						pUser->BroadcastRoomMsg(szMsg, true);
// 						sprintf(szDebugStep,"%d",14);
// 					}
// 					//---------------------------------------------------
// 					else if (pUser->AddFriend(pTarget->GetID(),pTarget->GetName(), /*m_pInfo->idFriend, m_pInfo->szName,*/m_pInfo->ucFriendType) &&
// 						pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucFriendType))
// 					{
// 						sprintf(szDebugStep,"%d",15);
// 						//-----A����BΪ����,����B�б���A����Ϣ----------
// 						CFriend* pFriend = pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID());
// 						pTarget->AddBeFriend(pFriend->GetID(),pUser->GetID(),pFriend->GetFriendType());
// 						//----------------------------
// 
// 						sprintf(szDebugStep,"%d",16);
// 						//-----A����BΪ����,����B�б���A����Ϣ----------
// 						pFriend = pTarget->GetFriend(pUser->GetID());
// 						pUser->AddBeFriend(pFriend->GetID(),pTarget->GetID(),pFriend->GetFriendType());
// 						//----------------------------
// 
// 						sprintf(szDebugStep,"%d",17);
// 						//20061020����bug�¼Ӻ���ʱְҵ��ʾerro.
// 						//create�¼�����������------------------------------------
// 						if(this->Create(_FRIEND_GETINFO, 
// 							pTarget->GetID(), 
// 							pTarget->GetName(), 
// 							ONLINE_STATUS, 
// 							pTarget->GetLookFace(), 
// 							pTarget->GetLev(),
// 							pTarget->GetProfession(),
// 							0,
// 							pTarget->GetSex(),
// 							m_pInfo->ucFriendType) )
// 							pUser->SendMsg(this);
// 						if(this->Create(_FRIEND_GETINFO, 
// 							pUser->GetID(), 
// 							pUser->GetName(), 
// 							ONLINE_STATUS, 
// 							pUser->GetLookFace(), 
// 							pUser->GetLev(),
// 							pUser->GetProfession(),
// 							0,
// 							pUser->GetSex(),
// 							m_pInfo->ucFriendType) )
// 							pTarget->SendMsg(this);
// 						
// 							/*
// 							if(this->Create(_FRIEND_GETINFO, pTarget->GetID(), pTarget->GetName(), ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev()))
// 							pUser->SendMsg(this);
// 							
// 							  if(this->Create(_FRIEND_GETINFO, pUser->GetID(), pUser->GetName(), ONLINE_STATUS, pUser->GetLookFace(), pUser->GetLev()))
// 							  pTarget->SendMsg(this);
// 						*/
// 						//--------------------------------------------------------
// 						
// 						MSGBUF	szMsg;
// 						sprintf(szMsg,g_objStrRes.GetStr(13057), pUser->GetName(), pTarget->GetName());//STR_MAKE_FRIEND
// 						//pUser->BroadcastRoomMsg(szMsg, true);
// 						if(pUser)
// 							pUser->SendSysMsg(szMsg);
// 						if(pTarget)
// 							pTarget->SendSysMsg(szMsg);
// 					}
// 					else
// 					{
// 						::LogSave("Error: add friend failed");
// 					}
// 				}
// 			}
// 		}
// 		break;
// 	case _FRIEND_APPLY:
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			CUser* pTarget = NULL;
// 			
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if (!pTarget)
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(28023));
// 				return;
// 			}
// 			if(!pTarget)
// 		//	if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, m_pInfo->idFriend, IPP_OF(pTarget)))
// 			{
// 				if(strlen(m_pInfo->szName) > 1)
// 				{
// 					ST_FRIEND_APPLY st;
// 					memset(&st, 0, sizeof(ST_FRIEND_APPLY));
// 
// 					sprintf(szDebugStep,"%d",1);
// 			/*		if(pUser->FetchApplyName(CUser::APPLY_FRIEND, m_pInfo->szName))
// 			//		{
// 						//	�ظ�
// 						BOOL bIsValid = TRUE;
// 						int nRet = 0;
// 						if(pUser->GetFriendAmount() >= _MAX_USERFRIENDSIZE)
// 						{
// 							nRet = 12073;	//STR_FRIEND_LIST_FULL
// 							bIsValid = FALSE;
// 						}
// 						else if(pUser->GetFriendByName(m_pInfo->szName))
// 						{
// 							nRet = 12074;//STR_YOUR_FRIEND_ALREADY
// 							bIsValid = FALSE;
// 						}
// 						else if(!pUser->GetIsAllowFriend())
// 						{
// 							nRet = 12075;//STR_NOTALLOWFRIEND
// 							bIsValid = FALSE;
// 						}
// 						
// 						if(!bIsValid)
// 						{
// 							ST_FRIEND_APPLY st;
// 							memcpy(&st, pInfo, sizeof(ST_FRIEND_APPLY));
// 							st.nRet = nRet;
// 							st.unAction = FRIEND_APPLY_ACTION_TYPE_APPLY_NANK;
// 							MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_FRIENDAPPLY, STRUCT_TYPE(ST_FRIEND_APPLY), (const char*)&st);
// 							return;
// 						}
// 						
// 						st.unAction = FRIEND_APPLY_ACTION_TYPE_APPLY_ANK;	
// 
// 						st.idPlayer = m_pInfo->idFriend;
// 						strcpy(st.szPlayerName, m_pInfo->szName);
// 
// 						st.idTarget = pUser->GetID();
// 						strcpy(st.szTargetName, pUser->GetName());
// 						st.nTargetLev = pUser->GetLev();
// 						st.nTargetLookFace = pUser->GetLookFace();
// 						st.nTargetProfession = pUser->GetProfession();
// 						
// 						MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_FRIENDAPPLY, STRUCT_TYPE(ST_FRIEND_APPLY), (const char*)&st);
// 						/*
// 					}
// 					else
// 				*/	{
// 						st.idPlayer = pUser->GetID();
// 						strcpy(st.szPlayerName, pUser->GetName());
// 						st.nLev = pUser->GetLev();
// 						st.nLookFace = pUser->GetLookFace();
// 						st.nProfession = pUser->GetProfession();
// 						st.nSex = pUser->GetSex();
// 						st.idTarget = m_pInfo->idFriend;
// 						strcpy(st.szTargetName, m_pInfo->szName);
// 						st.unAction = FRIEND_APPLY_ACTION_TYPE_APPLY_REQ;
// 						st.nRet = 0;
// 						pUser->SetApplyName(CUser::APPLY_FRIEND, m_pInfo->szName);
// 						MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_FRIENDAPPLY, STRUCT_TYPE(ST_FRIEND_APPLY), (const char*)&st);
// 						sprintf(szDebugStep,"%d",2);
// 					}
// 				}
// 				break;
// 			}
// 			
// 			else if(m_pInfo->idFriend != 0 && !pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, m_pInfo->idFriend, IPP_OF(pTarget)))
// 				break;
// 
// 			if( pTarget->QueryEnemy()->GetName(pUser->GetID()))// ����Է�����������������޷�����ӶԷ�Ϊ���� [10/10/2007]
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(16041));//-�Է��Ѿ����������������޷���Ϊ����!
// 				break;
// 			}
// 
// 			if(pUser->GetFriendAmount() >= _MAX_USERFRIENDSIZE)
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(12073));//STR_FRIEND_LIST_FULL
// 				break;
// 			}
// 
// 			if(pUser->GetFriend(m_pInfo->idFriend))
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(12074));//STR_YOUR_FRIEND_ALREADY
// 				break;
// 			}
// 
// 			if(!pTarget->GetIsAllowFriend())
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(12075));//STR_NOTALLOWFRIEND
// 				break;
// 			}
// 			/*
// 			if(m_pInfo->idFriend == 0 && pTarget->FetchApplyName(CUser::APPLY_FRIEND, m_pInfo->szName))
// 			{
// 				pUser->SetApplyName(CUser::APPLY_FRIEND, m_pInfo->szName);
// 				IF_OK(Create(_FRIEND_APPLY, pUser->GetID(), pUser->GetName(), true))
// 					pTarget->SendMsg(this);
// 				
// 				pTarget->SendSysMsg(g_objStrRes.GetStr(13056), pUser->GetName());//STR_TO_MAKE_FRIEND
// 				pUser->SendSysMsg(g_objStrRes.GetStr(12077));//STR_MAKE_FRIEND_SENT
// 			}
// 			
// 			else*/	
// 			sprintf(szDebugStep,"%d",3);
// 			if(pTarget->FetchApply(CUser::APPLY_FRIEND) != pUser->GetID())
// 			{
// 				sprintf(szDebugStep,"%d",4);
// 				// ����Է�δ�����Լ������������������
// 				pUser->SetApply(CUser::APPLY_FRIEND, pTarget->GetID());
// 				IF_OK(Create(_FRIEND_APPLY, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,m_pInfo->ucFriendType))
// 					pTarget->SendMsg(this);
// 
// 				pTarget->SendSysMsg(g_objStrRes.GetStr(13056), pUser->GetName());//STR_TO_MAKE_FRIEND
// 				pUser->SendSysMsg(g_objStrRes.GetStr(12077));//STR_MAKE_FRIEND_SENT
// 				sprintf(szDebugStep,"%d",5);
// 			}
// 			else
// 			{
// 				sprintf(szDebugStep,"%d",6);
// 				// ����Է��Ѿ����Լ�����������ֱ��Ϊ˫���Ӻ���
// 				if (pUser->AddFriend(/*m_pInfo->idFriend*/pTarget->GetID(), /*m_pInfo->szName*/pTarget->GetName(),m_pInfo->ucFriendType) &&
// 						pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucFriendType))
// 				{
// 					sprintf(szDebugStep,"%d",7);
// 					//-----A����BΪ����,����B�б���A����Ϣ----------
// 					CFriend* pFriend = pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID());
// 					pTarget->AddBeFriend(pFriend->GetID(),pUser->GetID(),pFriend->GetFriendType());
// 					//----------------------------
// 					sprintf(szDebugStep,"%d",8);
// 					//-----A����BΪ����,����B�б���A����Ϣ----------
// 					pFriend = pTarget->GetFriend(pUser->GetID());
// 					pUser->AddBeFriend(pFriend->GetID(),pTarget->GetID(),pFriend->GetFriendType());
// 					//----------------------------
// 					sprintf(szDebugStep,"%d",9);
// 
// 					//20061020����bug�¼Ӻ���ʱְҵ��ʾerro.
// 					//create�¼�����������------------------------------------
// 					if(this->Create(_FRIEND_GETINFO, 
// 						pTarget->GetID(), 
// 						pTarget->GetName(), 
// 						ONLINE_STATUS, 
// 						pTarget->GetLookFace(), 
// 						pTarget->GetLev(),
// 						pTarget->GetProfession(),
// 						0,
// 						pTarget->GetSex(), 
// 						m_pInfo->ucFriendType))
// 						pUser->SendMsg(this);
// 					
// 					if(this->Create(_FRIEND_GETINFO, 
// 						pUser->GetID(), 
// 						pUser->GetName(), 
// 						ONLINE_STATUS, 
// 						pUser->GetLookFace(), 
// 						pUser->GetLev(),
// 						pUser->GetProfession(),
// 						0,
// 						pUser->GetSex(),
// 						m_pInfo->ucFriendType) )
// 						pTarget->SendMsg(this);
// 					/*
// 					if(this->Create(_FRIEND_GETINFO, pTarget->GetID(), pTarget->GetName(), ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev()))
// 						pUser->SendMsg(this);
// 
// 					if(this->Create(_FRIEND_GETINFO, pUser->GetID(), pUser->GetName(), ONLINE_STATUS, pUser->GetLookFace(), pUser->GetLev()))
// 						pTarget->SendMsg(this);
// 						*/
// 					//--------------------------------------------------------
// 
// 					MSGBUF	szMsg;
// 					sprintf(szMsg, STR_MAKE_FRIEND, pUser->GetName(), pTarget->GetName());
// 					//pUser->BroadcastRoomMsg(szMsg, true);
// 					if(pUser)
// 					   pUser->SendSysMsg(szMsg);
// 					if(pTarget)
// 					   pTarget->SendSysMsg(szMsg);
// 				}
// 				else
// 				{
// 					::LogSave("Error: add friend failed");
// 				}
// 			}
// 		}
// 		break;
// 
// 	case _FRIEND_ONLINE:
// 		{
// 			ASSERT(!"_FRIEND_ONLINE");
// 		}
// 		break;
// 	case _FRIEND_OFFLINE:
// 		{
// 			ASSERT(!"_FRIEND_OFFLINE");
// 		}
// 		break;
// 
// 	case _FRIEND_BREAK:
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			CUser* pTarget = UserManager()->GetUser(m_pInfo->idFriend);		// ����ΪNULL
// 
// 			CFriendPtr pFriend = pUser->GetFriend(m_pInfo->idFriend);
// 			if (!pFriend)
// 			{
// 				if(pTarget)
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12078), pTarget->GetName());//STR_NOT_YOUR_FRIEND
// 				break;
// 			}
// 			sprintf(szDebugStep,"%d",1);
// 			int nFriendType = pFriend->GetFriendType();
// 			NAMESTR	szFriend = "";
// 			SafeCopy(szFriend, pFriend->GetFriendName(), _MAX_NAMESIZE);
// 			sprintf(szDebugStep,"%d",2);
// 			if (pUser->DelFriend(m_pInfo->idFriend))
// 			{
// 				sprintf(szDebugStep,"%d",3);
// 				if (pTarget)
// 					pTarget->DelBeFriend(pUser->GetID());//�������[2008/12/17]
// 				//20090420haocq����ɾ����ʾ
// 				if (nFriendType == 1)
// 				{
// 					MSGBUF	szMsg;
// 					sprintf(szMsg,g_objStrRes.GetStr(13058) , pUser->GetName(), szFriend);//STR_BREAK_FRIEND
// 					pUser->BroadcastRoomMsg(szMsg, true);
// 				}
// 				else if (nFriendType == 2)
// 				{
// 					MSGBUF	szMsg;
// 					sprintf(szMsg,"���ѽ�%s����ʱ�����б���ɾ��" , szFriend);//STR_BREAK_FRIEND
// 					pUser->SendSysMsg(szMsg);
// 				}
// 
// 				pUser->SendMsg(this);
// 				sprintf(szDebugStep,"%d",4);
// 			}
// 
// 			if (pTarget && nFriendType == 1)
// 			{
// 				sprintf(szDebugStep,"%d",5);
// 				if(pTarget->DelFriend(pUser->GetID()))
// 				{
// 					if (pUser)
// 						pUser->DelBeFriend(pTarget->GetID());//�������[2008/12/17]
// 					if(this->Create(_FRIEND_BREAK, pUser->GetID(), pUser->GetName()))
// 						pTarget->SendMsg(this);
// 				}
// 				sprintf(szDebugStep,"%d",6);
// 			}
// 			else if(!pTarget && nFriendType == 1)	// �Է�������
// 			{
// 				SQLBUF	szSQL;
// 				sprintf(szSQL, "DELETE FROM %s WHERE userid=%u && friendid=%u", _TBL_FRIEND, m_pInfo->idFriend, pUser->GetID());
// 				Database()->ExecuteSQL(szSQL);
// 
// 				m_pInfo->idFriend	= pUser->GetID();
// 				MapGroup(PID)->QueryIntraMsg()->TransmitMsg(this, GetSocketID(), GetNpcID());
// 			}
// 		}
// 		break;
// 
// 	case _FRIEND_GETINFO:
// 		{
// 			ASSERT(!"_FRIEND_GETINFO");
// 		}
// 		break;
// 	case _ENEMY_DEL:
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			pUser->QueryEnemy()->Del(m_pInfo->idFriend, UPDATE_TRUE);
// 		}
// 		break;
// 	case _ENEMY_APPLY:		//���0211 Ŀǰֻ���������������������
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			CUser* pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if(pTarget&&pTarget->GetID() == pUser->GetID())
// 			{
// 				pUser->SendSysMsg("�㲻���Լ������������");//STR_NOT_ALLOW_TEAM
// 				return;
// 			}
// 			DWORD nLookFace = 0 ;
// 			int nLevel = 0 ;
// 			DWORD profession = 0 ;				//���ְҵ
// 			int nSex = 0;
// 			if(pTarget!=NULL)					//Ŀ������
// 			{
// 				sprintf(szDebugStep,"%d",1);
// 				if(pUser->GetFriend(pTarget->GetID())!=NULL)//���Լ��ĺ���
// 				{
// 					sprintf(szDebugStep,"%d",2);
// 					if (pUser->DelFriend(pTarget->GetID()))//����Ϣ�ÿͻ��˱˴�ɾ��
// 					{
// 						pTarget->DelBeFriend(pUser->GetID());//�������[2008/12/17]
// 						if(this->Create(_FRIEND_BREAK, pTarget->GetID(), pTarget->GetName()))
// 							pUser->SendMsg(this);
// 					}
// 					sprintf(szDebugStep,"%d",3);
// 					if(pTarget->DelFriend(pUser->GetID()))//����Ϣ�ÿͻ��˱˴�ɾ��
// 					{
// 						pUser->DelBeFriend(pTarget->GetID());//�������[2008/12/17]
// 						if(this->Create(_FRIEND_BREAK, pUser->GetID(), pUser->GetName()))
// 							pTarget->SendMsg(this);
// 					}
// 					sprintf(szDebugStep,"%d",4);
// 				}
// 				m_pInfo->idFriend=pTarget->GetID();
// 				sprintf(szDebugStep,"%d",5);
// 					// 2007/02/01 ������
// 				SafeCopy(m_pInfo->szName, pTarget->GetName(), _MAX_NAMESIZE);
// 				sprintf(szDebugStep,"%d",6);
// 
// 				nLookFace = pTarget->GetLookFace() ;
// 
// 				nLevel = pTarget->GetLev() ; 
// 
// 				profession = pTarget->GetProfession() ;
// 
// 				nSex = pTarget->GetSex();
// 
// 			}
// 			else if(m_pInfo->idFriend!=0&&m_pInfo->szName!=NULL)//����Է������ߣ������Լ��ĺ���
// 			{
// 				//ID��NAMEͨ���ͻ��˷�������ֱ���������
// 				SQLBUF	szSQL;
// 				sprintf(szSQL, "DELETE FROM %s WHERE userid=%u && friendid=%u", _TBL_FRIEND, m_pInfo->idFriend, pUser->GetID());
// 				Database()->ExecuteSQL(szSQL);
// 			}
// 			else if(m_pInfo->szName!=NULL)	//����ͻ���ֻ������Ŀ������֣�������ݿ����ȵõ��Է���ID
// 			{	
// 				SQLBUF	szSQL;
// 				sprintf(szSQL, "SELECT id, lookface, sex, level, profession FROM %s WHERE name='%s' LIMIT 1", _TBL_USER,m_pInfo->szName);
// 				IRecordset*	pRes = Database()->CreateNewRecordset(szSQL, false);
// 				if(pRes)
// 				{
// 					m_pInfo->idFriend=pRes->LoadDWord("id");
// 					nLookFace=pRes->LoadDWord("lookface");
// 					nLevel=pRes->LoadDWord("level");
// 					profession=pRes->LoadDWord("profession");
// 					nSex=pRes->LoadDWord("sex");
// 					pRes->Release();
// 				}
// 			}
// 			if ( m_pInfo->idFriend != 0)  // 2007.8.11 ½��Ң�޸�
// 			{
// 				sprintf(szDebugStep,"%d",8);
// 				pUser->QueryEnemy()->Add(m_pInfo->idFriend,
// 					m_pInfo->szName,
// 					SYNCHRO_TRUE,
// 					UPDATE_TRUE,
// 					nLookFace,
// 					nLevel,
// 					profession,
// 					nSex);
// 				sprintf(szDebugStep,"%d",9);
// 			}
// 			else
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(15085));//"Ŀ����Ҳ�����!"
// 			}
// 		}
// 		break;
// 	case _FRIEND_NOTALLOW:
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{
// 			pUser->SetIsAllowFriend(m_pInfo->idFriend);
// 		}
// 		break;
// 
// 	case _FRIEND_PVP:  //Satan2009.08.07 ������ȫ���ھ���
// 		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
// 		{			
// 			OBJID idFriend = m_pInfo->idFriend;
// 			CUser* pTarget = UserManager()->GetUser(idFriend);
// 			if(!pTarget)
// 			{
// 				pUser->SendSysMsg("�Ҳ���Ŀ��!");
// 				return;
// 			}
// 
// 			if(100 == m_pInfo->ucFriendType) //������� �����Ľ��
// 			{				
// 				MapGroup(PID)->GetPVPManger()->EndPVP(idFriend);
// 				return;
// 			}
// 
// 			if(4 == m_pInfo->ucFriendType) //�Է��ܾ�
// 			{
// 				pTarget->FetchApply(CUser::APPLY_PVP);
// 				char temp[128];
// 				sprintf(temp,"��� %s �ܾ�����ľ�������",pUser->GetName());
// 				pTarget->SendSysMsg(temp);
// 				return;
// 			}
// 			
// 			IStatus* pStatus = pTarget->QueryStatus( STATUS_PVPMODE );
// 			if( pStatus )
// 			{
// 				pUser->SendSysMsg("�����ھ�����!");
// 				return;
// 			}
// 			pStatus = pUser->QueryStatus( STATUS_PVPMODE );
// 			if( pStatus )
// 			{
// 				pUser->SendSysMsg("�Է����ھ�����!");
// 				return;
// 			}
// 
// 			if(1 == m_pInfo->ucFriendType || 2 == m_pInfo->ucFriendType)
// 			{
// 				pUser->SetPVPTargetID(m_pInfo->ucFriendType);
// 				pUser->SetApply(CUser::APPLY_PVP, pTarget->GetID());
// 				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,m_pInfo->ucFriendType))
// 					pTarget->SendMsg(this);
// 				return;
// 			}
// 		
// 
// 			if(pTarget->FetchApply(CUser::APPLY_PVP) != pUser->GetID())
// 			{		
// 				pUser->SendSysMsg("���������Ѿ����ڣ�");
// 				return;
// // 				pUser->SetPVPTargetID(m_pInfo->ucFriendType);
// // 				pUser->SetApply(CUser::APPLY_PVP, pTarget->GetID());
// // 				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,m_pInfo->ucFriendType))
// // 					pTarget->SendMsg(this);
// 			}
// 			else
// 			{
// 				pUser->FetchApply(CUser::APPLY_PVP);
// 
// 				if(pUser->GetMapID() != pTarget->GetMapID()               //ͬ��ͼ
// 					|| abs(pUser->GetPosX()- pTarget->GetPosX()) > 20     //X����
// 					|| abs(pUser->GetPosY()- pTarget->GetPosY()) > 20)    //Y����       
// 				{
// 					pUser->SendSysMsg("����˫������̫Զ!");
// 					pTarget->SendSysMsg("����˫������̫Զ!");
// 					return;
// 				}
// 
// 				CGameMap* pMap = pUser->GetMap();
// 				if(!pMap || !pMap->IsPVPEnable())
// 				{
// 					pUser->SendSysMsg("��ǰ��ͼ���������!");
// 					pTarget->SendSysMsg("��ǰ��ͼ���������!");
// 					return;
// 				}
// 
// // 				if(pUser->GetLife() != pUser->GetMaxLife()
// // 					||pUser->GetMana() != pUser->GetMaxMana()
// // 					||pTarget->GetLife() != pTarget->GetMaxLife()
// // 					||pTarget->GetMana() != pTarget->GetMaxMana())
// // 				{
// // 					pUser->SendSysMsg("����ǰ����˫����ȫ�ָ�!");
// // 					pTarget->SendSysMsg("����ǰ����˫����ȫ�ָ�!");
// // 					return;
// // 				}
// 
// 				if(MapGroup(PID)->GetPVPManger()->findPVP(pUser->GetID()))
// 				{
// 					MapGroup(PID)->GetPVPManger()->EndPVP(pUser->GetID());
// 				}
// 				if(MapGroup(PID)->GetPVPManger()->findPVP(pTarget->GetID()))
// 				{
// 					MapGroup(PID)->GetPVPManger()->EndPVP(pTarget->GetID());
// 				}				
// 				//��ʼ����������������������
// 
// 				pUser->SendSysMsg("������ʼ!");
// 				pTarget->SendSysMsg("������ʼ!");
// 
// 				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,5))
// 					pTarget->SendMsg(this);
// 
// 				IF_OK(Create(_FRIEND_PVP, pTarget->GetID(), pTarget->GetName(), true,0,0,0,0,0,5))
// 					pUser->SendMsg(this);
// 
// 				//ˢ����
// 				int flagX = (pUser->GetPosX() + pTarget->GetPosX()) / 2;
// 				int flagY = (pUser->GetPosY() + pTarget->GetPosY()) / 2;
// 				ST_CREATENEWNPC	info;
// 				memset(&info, 0, sizeof(ST_CREATENEWNPC));
// 				info.idMap			= pUser->GetMapID();
// 				info.idData			= 0;
// 				info.idOwner		= 0;		
// 				info.usPosX			= flagX;
// 				info.usPosY			= flagY;
// 				info.usType			= 815;
// 				info.ucOwnerType	= 0;
// 				info.nData			= 0;
// 				int nType = pTarget->GetPVPTargetID();
// 				OBJID flagID = NpcManager()->CreateSynPetReturnID(&info);
// 
// 				CRole::AttachStatus( pUser, STATUS_PVPMODE, 100, 3 * 60 + 5);
// 				CRole::AttachStatus( pTarget, STATUS_PVPMODE, 100, 3 * 60 + 5);
// 
// 				if(2 == nType )
// 				{
// 					CRole::AttachStatus( pUser, STATUS_BIGBLOOD_USERNOT, 100, 3 * 60 + 5);
// 					CRole::AttachStatus( pTarget, STATUS_BIGBLOOD_USERNOT, 100, 3 * 60 + 5);
// 				}
// 
// 				//Ϊ��Ҽ�װ��������
// 				pUser->SetPVPTargetID(pTarget->GetID());
// 				pUser->SetPVPX(flagX);
// 				pUser->SetPVPY(flagY);
// 				pUser->SetPVPMap(pUser->GetMapID());
// 
// 				pTarget->SetPVPTargetID(pUser->GetID());
// 				pTarget->SetPVPX(flagX);
// 				pTarget->SetPVPY(flagY);
// 				pTarget->SetPVPMap(pUser->GetMapID());
// 				MapGroup(PID)->GetPVPManger()->AddPVP(pUser->GetID(),pTarget->GetID(),nType,flagID);				
// 			}
// 		}
// 		break;
// 	}
// 	   }
// 	   catch (...)
// 	   {
// 		   ::LogSave("CMsgFriend %s %s",szDebugAction,szDebugStep);
// 		   
// 	   }
// }
