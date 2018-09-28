// MsgManager.cpp: implementation of the MsgManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgManager.h"
#include "RoleManager.h"
#include "MsgFriend.h"
#include "Define.h"
#include "MsgSyndicate.h"
#include "MsgSynInfo.h"
#include "MsgItem.h"
#include "MsgFee.h"
#include "MsgSynMemberInfo.h"
#include "MsgMainServer.h"
#include "MsgSchoolMember.h"
#include "MsgWeaponSoul.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MsgManager::MsgManager()
{

}

MsgManager::~MsgManager()
{

}

///ת����Ϣ����ϵ��������
void MsgManager::SendRelayMsg(int nServerIndex,CNetMsg &msg)
{
	g_pPointThread->m_aServerSocket[nServerIndex].Send(msg.GetBuf(), msg.GetSize());
}

void MsgManager::UpServerListUI()
{
	char *szServerInfo=new char[256];
	ZeroMemory(szServerInfo,sizeof(szServerInfo));
	int nServerInfoOffset=0;
	for(int i = 1; i < MAXGAMESERVERS; i++)	// ����0��0Ϊ����
	{
		if(g_pPointThread->m_aServerSocket[i].IsOpen())
		{
			///�װ��ִ�,��δ�յ�msgaccount��Ϣ�ķ�����Ҳ���ڷ���Χ
			if(strlen(g_pPointThread->m_aServerName[i])<=0)
				continue;

			if(nServerInfoOffset>256)
				break;///�����������������
			nServerInfoOffset=strlen(szServerInfo);
			sprintf(szServerInfo+nServerInfoOffset,"%s:%d;",g_pPointThread->m_aServerName[i],RoleManager::Instance().GetRoleCountAtServer(i));
		}
	}
	if(strlen(szServerInfo)<=0)
		sprintf(szServerInfo,"���з���������");
	PrintText(szServerInfo);
  	//AfxGetMainWnd()->PostMessage(WM_RBUTTONDBLCLK,(WPARAM)szServerInfo);
}

void MsgManager::OnBreathe()///ר����һЩ��ʱ����
{
	DEBUG_TRY
	const UINT nUpdateServerListDelay=10000;//1000*60*2;///2����
	static UINT nUpdateServerListTime=GetTickCount();
	if(GetTickCount()>(nUpdateServerListTime+nUpdateServerListDelay))
	{
		nUpdateServerListTime=GetTickCount();
		BroadcastGameServerListMsg();///����㲥һ�����߷������б���Ϸ�������л���
	}

	///����һ�¸�������������
	const UINT nUpdateUIServerListDelay=1000*5;///5��
	static UINT nUpdateUIServerListTime=GetTickCount();
	if(GetTickCount()>(nUpdateUIServerListTime+nUpdateUIServerListDelay))
	{
		nUpdateUIServerListTime=GetTickCount();
		UpServerListUI();///����㲥һ�����߷������б���Ϸ�������л���
	}

	BreakTimeoutSynLock();
	BreakTimeoutUserLock();
	DEBUG_CATCH("MsgManager::OnBreathe");
}

void MsgManager::BroadcastGameServerListMsg(int nIgnoreServerIndex)
{
	CMsgTalk msg;
	MakeGameServerList2TalkMsg(msg);///������Ϣ
	BroadcastRelayMsg(msg,nIgnoreServerIndex);///�㲥��Ϣ
}
										 
void MsgManager::ProcessMsgServerDisconnect(int nServerIndex)///������Ϸ���������ߵ����
{
	DEBUG_TRY
	RoleManager::Instance().PureServerRoleList(nServerIndex);///�����������ϵ������ʧȥ���Ӻ���ո�����ص��û���Ϣ
	///���������б����msgtalk��Ϣ�㲥���������߷�����ȥ����
	BroadcastGameServerListMsg();
	DEBUG_CATCH("MsgManager::ProcessMsgServerDisconnect");
}
//������������
void MsgManager::AppointMainServer(int nIgnoreServerIndex)
{
	CMsgMainServer msg;

	for(int i =1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
	{
		if (i==nIgnoreServerIndex && stricmp(g_szMainServer, g_pPointThread->m_aServerName[i])==0)//����һ�䣬�Է�֮ǰû��ȥ����ʧЧ������Ϸ������
			strcpy(g_szMainServer,"");
		if(g_pPointThread->m_aServerSocket[i].IsOpen() && i!=nIgnoreServerIndex)
		{
			if (strlen(g_szMainServer)<=0)//��ǰû����Ч������Ϸ������
			{
				strcpy(g_szMainServer, g_pPointThread->m_aServerName[i]);
				PrintText("������%s��Ϊ����Ϸ������", g_pPointThread->m_aServerName[i]);
				if (msg.Create(1,0,true))
					g_pPointThread->m_aServerSocket[i].Send(msg.GetBuf(), msg.GetSize());//֪ͨ����Ϸ������
				if (msg.Create(0,0,true))
					BroadcastRelayMsg(msg,i);//֪ͨ������Ϸ������
				break;
			}
			else//�Ѿ���������Ϸ��������ֻ�㲥һ��
			{
				if (stricmp(g_szMainServer, g_pPointThread->m_aServerName[i])==0)
					msg.Create(1,0,true);
				else
					msg.Create(0,0,true);
				g_pPointThread->m_aServerSocket[i].Send(msg.GetBuf(), msg.GetSize());
			}
		}
	}
	if (strlen(g_szMainServer)<=0)
		PrintText("����Ϸ���������ߣ��޷�����");
}
///�㲥��Ϣ��������Ϸ��������
void MsgManager::BroadcastRelayMsg(CNetMsg &msg,int nIgnoreServerIndex)
{
	for(int i =1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
	{
		if(g_pPointThread->m_aServerSocket[i].IsOpen() && i!=nIgnoreServerIndex)///Դ�����㲥
		{
			g_pPointThread->m_aServerSocket[i].Send(msg.GetBuf(), msg.GetSize());
		}
	}
}

///������Ϣ����ϵ��������
void MsgManager::SendRelationReplyMsg(int nServerIndex,MsgRelayAction msgAction,OBJID srcRoleID,OBJID dstRoleID,LPCTSTR szName)
{
	CMsgRelay::MSG_Info	msg;
	msg.unMsgSize	= sizeof(msg);
	msg.unMsgType	= _MSG_RELAY;
	
	msg.usAction	= msgAction;
	msg.idRole		= srcRoleID;
	msg.dstRoleID	= dstRoleID;

	strncpy(msg.szServerName,szName,_MAX_NAMESIZE);
	g_pPointThread->m_aServerSocket[nServerIndex].Send((LPCTSTR)&msg, msg.unMsgSize);
}

///�ߵ�����������������
bool MsgManager::KickoutRole(OBJID idRole,int nOldServerIndex)
{
	bool hr=false;
	Role* ptrRole=RoleManager::Instance().GetRole(idRole);
	if(ptrRole)
	{
// 		CMsgFee	cMsg;
// 		cMsg.Create(idRole, CMsgFee::FEE_KICKOUT);
// 		///�ߵ�ǰһ��
// 		SendRelationRelayMsg(ptrRole->idServer,cMsg.GetType(),(const char*)&cMsg.m_pInfo->idAccount,cMsg.GetSize());
// 		///�ߵ�������
// 		SendRelationRelayMsg(nOldServerIndex,cMsg.GetType(),(const char*)&cMsg.m_pInfo->idAccount,cMsg.GetSize());

		///��⵽�ظ��Ľ�ɫ���ߣ�����־
		LOGERROR("MsgManager::KickoutRole idRole:%d,nOldServerIndex:%d",idRole,nOldServerIndex);
		hr=true;
	}
	return hr;
}

void MsgManager::ProcessRelayMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	CMsgRelay::MSG_Info& info=*(CMsgRelay::MSG_Info*)msg.m_bufMsg;
	MsgRelayAction msgAction=(MsgRelayAction)info.usAction;
	switch (msgAction)
	{
	case raRoleOnline:///�������
 //		if(KickoutRole(info.idRole,nServerIndex)==false)///�ߵ�ǰ�����ߵ��û�
		KickoutRole(info.idRole,nServerIndex);
		RoleManager::Instance().OnlineRole(info.idRole,info.szUserName,nServerIndex);
		break;
	case raRoleOffline:///�������
		RoleManager::Instance().OfflineRole(info.idRole);
		break;
	case raQueryRole_Friend:///��ѯ�������
		RoleManager::Instance().QueryRole(nServerIndex,msgAction,info.dstRoleID,info.idRole);
		break;
	default:
		break;
	}
	DEBUG_CATCH("MsgManager::ProcessRelayMsg");	
}

void MsgManager::ProcessSyndicateMsg(int nServerIndex,CNetMsg& msg)
{
//	MsgManager::Instance().BroadcastRelayMsg(msg,nServerIndex);///ȫ�߹㲥 nServerIndex
	DEBUG_TRY

	CMsgSyndicate::MSG_Info& info=*(CMsgSyndicate::MSG_Info*)msg.m_bufMsg;

	char szSender[_MAX_NAMESIZE];
	
	CNetStringPacker	StrPacker;
	StrPacker.SeekBuf(info.szBuf, _MAX_MSGSIZE-sizeof(info)+1);
	StrPacker.GetString(0, szSender, sizeof(szSender));
	Role* ptrRole=RoleManager::Instance().GetRole(szSender);
	OBJID idSyndicate=0;
	memcpy(&idSyndicate,msg.m_bufMsg+msg.GetSize()-4,4);///��ȡextra����,���汣�����Դ�û���id
 	SyndicateAction msgAction=(SyndicateAction)info.usAction;
	switch (msgAction)
 	{
		///ͬʱ��׼ʱ��Ҫ���û���
		case SYN_ANSERR_OK:
		{
			if(Lock(idSyndicate) && LockUser(info.idTarget))
			{
				///�����ɹ�,������Ϣ
				if(ptrRole)
				{
					SendRelayMsg(ptrRole->idServer,msg);
				}
			}
			else
			{
				if(ptrRole)
				{
					///����ʧ��,�Ե������Ϣ,�����ش�����Ϣ
					SendSysMsg(ptrRole->idServer,ptrRole->strRoleName,_TXTATR_TOPSHOW,"����ϵͳæ,���Ժ�����");///�Ǻ�������
				}
			}

		}
		break;
		case SYN_KICKOUT:
		case SYN_ANNONCE:
		case SYN_SETRANK:
		case SYN_DEMISE:
		case SYN_LEAVER:
		case SYN_ANSERR_NO:
		case SYN_LEVELUP:
		case SYN_IMPACH:
		case SYN_SETLEADER:
		case SYN_EMPOLY:
		case SYN_UPLEV:
		case SYN_UPSKILLSUB:
		case SYN_INVITE:
		{
			if(Lock(idSyndicate))
			{
				///�����ɹ�,������Ϣ
				if(ptrRole)
				{
					SendRelayMsg(ptrRole->idServer,msg);
				}
			}
			else
			{
				if(ptrRole)
				{
					///����ʧ��,�Ե������Ϣ,�����ش�����Ϣ
					SendSysMsg(ptrRole->idServer,ptrRole->strRoleName,_TXTATR_TOPSHOW,"����ϵͳæ,���Ժ�����");///�Ǻ�������
				}
			}
		}
		break;
//		case SYN_ANNONCE_OTHERLINE:
// 		case SYN_CREATECOMPLETE:///���Ŵ����ɹ��㲥��������
// 		case SYN_CREATE_OK:///���ɴ����ɹ�,ת������Դ��֮������з�����ȥ��������
// 			MsgManager::Instance().BroadcastRelayMsg(msg,nServerIndex);///ȫ�߹㲥 nServerIndex
// 			break;
// 	// 	case raRoleOffline:///�������
// 	// 		RoleManager::Instance().OfflineRole(info.idRole);
// 	// 		break;
// 	// 	case raQueryRole_Friend:///��ѯ�������
// 	// 		RoleManager::Instance().QueryRole(nServerIndex,msgAction,info.dstRoleID,info.idRole);
// 	// 		break;
 		default:
			BroadcastRelayMsg(msg,nServerIndex);///ȫ�߹㲥 nServerIndex
 			break;
 	}
	DEBUG_CATCH("MsgManager::ProcessSyndicateMsg");
	
}


void MsgManager::ProcessSyndicateInfoMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	CMsgSynInfo::MSG_Info& info=*(CMsgSynInfo::MSG_Info*)msg.m_bufMsg;
	BroadcastRelayMsg(msg,nServerIndex);///ȫ�߹㲥 nServerIndex	
	DEBUG_CATCH("MsgManager::ProcessSyndicateInfoMsg");
}

void MsgManager::ProcessEudemoniInfoMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	OBJID idUser=0;
	memcpy(&idUser,msg.m_bufMsg+msg.GetSize()-4,4);///��ȡextra����,���汣�����Դ�û���id
	Role* ptrRole=NULL;
	ptrRole=RoleManager::Instance().GetRole(idUser);
	if(ptrRole)
	{
		SendRelayMsg(ptrRole->idServer,msg);
	}
	DEBUG_CATCH("MsgManager::ProcessEudemoniInfoMsg");
}

///��������û��ѳ�ʱ����
void MsgManager::BreakTimeoutUserLock()
{
	SyndicateLockStateList::iterator it;
	for(it=m_userLockStateList.begin();it!=m_userLockStateList.end();++it)
	{
		if(GetTickCount()>it->second)
		{
			it->second=0;
		}
	}
}

///������а����ѳ�ʱ����
void MsgManager::BreakTimeoutSynLock()
{
	SyndicateLockStateList::iterator it;
	for(it=m_synLockStateList.begin();it!=m_synLockStateList.end();++it)
	{
		if(GetTickCount()>it->second)
		{
			it->second=0;
		}
	}
}

bool MsgManager::LockUser(OBJID idUser)///������ɫ,trueΪ�����ɹ�,falseΪ����ʧ��
{
	bool hr=false;
	if(m_userLockStateList[idUser]==0)
	{
		m_userLockStateList[idUser]=GetTickCount()+10000;///��ʱ����ʱ��Ϊ10��
		hr=true;///�����ɹ�
	}
	return hr;
}

void MsgManager::UnLockUser(OBJID idUser)///������ɫ
{
	m_userLockStateList[idUser]=false;
}

bool MsgManager::Lock(OBJID idSyndicate)///��������
{
	bool hr=false;
	if(m_synLockStateList[idSyndicate]==0)
	{
		m_synLockStateList[idSyndicate]=GetTickCount()+10000;///��ʱ����ʱ��Ϊ10��
		hr=true;///�����ɹ�
	}
	return hr;
}

void MsgManager::UnLock(OBJID idSynDicate)///��������
{
	m_synLockStateList[idSynDicate]=false;
}

///��Ϸ�������б����MsgTalk��Ϣ
void MsgManager::MakeGameServerList2TalkMsg(CMsgTalk& msg)
{
	///ȡ�ÿͻ����ʺ�ȥ�ķ���������
	///ȡ�����߷������б�
//	int nMode =0;
//	char szServerURL[URLSTRSIZE] = "N/A";
	char szServerInfo[64*5]="";
//	char szTemp[64*5]="";
	int nFullNum=800;///�涨800��Ϊ��������߸���
	int ServerCount=0;
	int nServerInfoOffset=0;///������ʱ��ƫ��λ��
//	UINT nServerPort=0;
	for(int i = 1; i < MAXGAMESERVERS; i++)	// ����0��0Ϊ����
	{
		if(g_pPointThread->m_aServerSocket[i].IsOpen())
		{
			///�װ��ִ�,��δ�յ�msgaccount��Ϣ�ķ�����Ҳ���ڷ���Χ
			if(strlen(g_pPointThread->m_aServerName[i])<=0)
				continue;
			///Ĭ��ȡIP��ַ
// 			nMode = g_pPointThread->m_nServerLoginMode[i];///�õ�ģʽ
// 			if(nMode==1)
// 				strcpy(szServerURL,g_pPointThread->m_aServerSocket[i].GetPeerIP());
// 			else if(nMode == 2)///��ַģʽȡ����
// 				strcpy(szServerURL,g_pPointThread->m_aServerURL[i]);
// 			else
// 			{
// 				char szLog[512];
// 				sprintf(szLog,"����˵�ַģʽ����,szServer:%s",g_pPointThread->m_aServerName[i]);
// 				::LogSave(szLog);
// 				continue;
// 			}
			if(ServerCount>5)///���ֻ֧��һ����5��������
				break;
			nServerInfoOffset=strlen(szServerInfo);
			if (strcmp(g_pPointThread->m_aServerName[i],"����")==0)
				sprintf(szServerInfo+nServerInfoOffset,"s:%s:%d:%d;","N/A",nFullNum,RoleManager::Instance().GetRoleCountAtServer(i));
			else
				sprintf(szServerInfo+nServerInfoOffset,"%s:%s:%d:%d;",g_pPointThread->m_aServerName[i],"N/A",nFullNum,RoleManager::Instance().GetRoleCountAtServer(i));
			ServerCount++;///������������1
		}
	}
//	PrintText("�������б�:%s",szServerInfo);
	if (strlen(g_szMainServer)<=0)
		PrintText("���棺û��������������");
	msg.Create(_TXTATR_MSG_SERVERLIST,szServerInfo);
}

void MsgManager::ProcessFriendMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	CMsgFriend::MSG_Info& info=*(CMsgFriend::MSG_Info*)msg.m_bufMsg;
	OBJID idUser=0;
	memcpy(&idUser,msg.m_bufMsg+msg.GetSize()-4,4);///��ȡextra����,���汣�����Դ�û���id
	Role* ptrRole=NULL,*ptrSrcRole=NULL;
	switch (info.ucAction)
	{
		case _FRIEND_GETFRIENDINFO:
		{
			ptrRole=RoleManager::Instance().GetRole(info.idFriend);
			if(ptrRole==NULL)
				ptrRole=RoleManager::Instance().GetRole(info.szName);
			if(ptrRole)
			{
				info.idFriend=ptrRole->idRole;///�۸���Ϣ���ܱ���
				info.dwLookFace=idUser;
				info.ucAction=_FRIEND_QUERYFRIENDINFO;
				SendRelayMsg(ptrRole->idServer,msg);///֪ͨ���������˼���,���߼Ӻ��ѳɹ�����Ҫ֪ͨ�Է�
			}
			else
			{
				ptrSrcRole=RoleManager::Instance().GetRole(idUser);
				if(ptrSrcRole)
				{
					if(info.ucLevel>0)///�ɵȼ����ж����Ƿ���һ�����ѵ����߿�������
					{
						SendRelayMsg(ptrSrcRole->idServer,msg);///����������Ϣ���ٻ�����
					}
					else
						SendSysMsg(ptrSrcRole->idServer,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,STR_TARGET_OFFLINE);///�Ǻ�������
// 					CMsgTalk msg
// 					CMsgTalk msg;
// 					if(msg.Create(SYSTEM_NAME, ptrSrcRole->strRoleName, STR_TARGET_OFFLINE))
// 						MsgManager::Instance().SendRelayMsg(ptrSrcRole->idServer,msg);
				}
			}
		}
			break;
		case _FRIEND_GETINFO:///�������
		{
			///�ж�������Ϣ������Ƿ�����,�������������߷����������������Ϣ�ظ�Դ������
			//OBJID nIgnoreServerIndex=nServerIndex;///�㲥ʱĬ�Ϻ�������ķ���������
			ptrRole=RoleManager::Instance().GetRole(info.idFriend);
			if(ptrRole==NULL)
				ptrRole=RoleManager::Instance().GetRole(info.szName);
			if(ptrRole)
			{
				info.ucOnlineFlag = RoleManager::Instance().GetRoleLine(g_aServerAccount[ptrRole->idServer].m_szServerName);//CMsgFriend::ONLINE_STATUS;
			}
			SendRelayMsg(nServerIndex,msg);
		}
			break;
		case _FRIEND_ADDFAIL:
		{
			ptrRole=RoleManager::Instance().GetRole(info.szName);
			ptrSrcRole=RoleManager::Instance().GetRole(info.idFriend);
			if(ptrRole && ptrSrcRole)
			{
				char szTemp[128];
				sprintf(szTemp,"<font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">�ܾ������ĺ�������!</font>",ptrSrcRole->strRoleName);
				SendSysMsg(ptrRole->idServer,ptrRole->strRoleName,_TXTATR_TOPSHOW,szTemp);
			}
		}
			break;
		case _FRIEND_ADDSUCCESS:
			{
 				ptrSrcRole=RoleManager::Instance().GetRole(info.idFriend);
				ptrRole=RoleManager::Instance().GetRole(info.szName);
				if(ptrRole && ptrSrcRole)
				{

					///�ı���ϢԴ��Ŀ����Ϣ
					strncpy(info.szName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);

					info.idFriend=ptrRole->idRole;
					///�Է�ͬ��Ӻ���,������Ϣת������ͼ�鴦��
					SendRelayMsg(ptrRole->idServer,msg);///֪ͨ���������˼���,���߼Ӻ��ѳɹ�����Ҫ֪ͨ�Է�

				}
			}
			break;
		case _FRIEND_ADDSUCCESS_PASSIVE:///������Ӻ��ѳɹ�ʱ����
		{
//			ptrSrcRole=RoleManager::Instance().GetRole(info.idFriend);
			ptrRole=RoleManager::Instance().GetRole(info.idFriend);
			if(ptrRole)
			{
				///����Ϣ������ʱ�л��ɷ�����
				info.ucOnlineFlag = CMsgFriend::ONLINE_STATUS;
				SendRelayMsg(ptrRole->idServer,msg);
			}
			///�۸�
			info.ucAction=_FRIEND_ADDSUCCESSREPLY;///һ·�۸�
			SendRelayMsg(nServerIndex,msg);

		}
			break;
		case _FRIEND_ADDFRIEND:
//		case _FRIEND_ADDSUCCESS:///�Ӻ��ѳɹ�����֪ͨ�����߶Է��ǲ������ߣ���֪ͨ���������˼���
		{
			ptrSrcRole=RoleManager::Instance().GetRole(info.idFriend);
			ptrRole=RoleManager::Instance().GetRole(info.szName);
			if(ptrRole)
			{
				///����Ϣ������ʱ�л��ɷ�����
 				if(ptrSrcRole)
 					strncpy(info.szName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);

				info.dwLookFace=ptrSrcRole->idRole;
// 
// 				info.ucOnlineFlag = CMsgFriend::ONLINE_STATUS;
				info.idFriend=ptrRole->idRole;
				SendRelayMsg(ptrRole->idServer,msg);///֪ͨ���������˼���,���߼Ӻ��ѳɹ�����Ҫ֪ͨ�Է�
// 				strncpy(info.szName,ptrRole->strRoleName,_MAX_NAMESIZE);
			}
			else
			{
				///�Է�������
				SendSysMsg(ptrSrcRole->idServer,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,"�Է������ߣ����ܼ�Ϊ����");
			}
// 			///�۸�
// 			info.ucAction=_FRIEND_ADDSUCCESSREPLY;///һ·�۸�
// 			SendRelayMsg(nServerIndex,msg);
		}
			break;
		case _FRIEND_REPLYFRIENDINFO:
			info.ucAction=_FRIEND_GETFRIENDINFO;///һ·�۸�
///			break;   ///����ȷʵû��break
		case _FRIEND_OFFLINE:
		case _FRIEND_ONLINE:
			{
				///��Һ������ߵ�Դ���ID���ڸ���������
				ptrRole=RoleManager::Instance().GetRole(idUser);
				if(ptrRole)
				{
					SendRelayMsg(ptrRole->idServer,msg);
				}
			}
			break;
		case _FRIEND_DELFRIEND:
		{
//			ptrSrcRole=RoleManager::Instance().GetRole(info.idFriend);
			ptrRole=RoleManager::Instance().GetRole(info.szName);
			if(ptrRole)
			{
				info.idFriend=ptrRole->idRole;
				//info.ucOnlineFlag = CMsgFriend::ONLINE_STATUS;
				SendRelayMsg(ptrRole->idServer,msg);
			}
		}
			break;
// 	case raRoleOffline:///�������
// 		RoleManager::Instance().OfflineRole(info.idRole);
// 		break;
// 	case raQueryRole_Friend:///��ѯ�������
// 		RoleManager::Instance().QueryRole(nServerIndex,msgAction,info.dstRoleID,info.idRole);
// 		break;
	default:
		break;
	}
	DEBUG_CATCH("MsgManager::ProcessFriendMsg");	
}

void MsgManager::ProcessTalkMsg(int nServerIndex,CNetMsg &msg)
{
	DEBUG_TRY

 	char szSender[_MAX_NAMESIZE];
	char szReceiver[_MAX_NAMESIZE];

	CMsgTalk::MSG_Info& info=*(CMsgTalk::MSG_Info*)msg.m_bufMsg;

	CNetStringPacker	StrPacker;
 	StrPacker.SeekBuf(info.szBuf, _MAX_MSGSIZE-sizeof(info)+1);
 	StrPacker.GetString(0, szSender, sizeof(szSender));
	StrPacker.GetString(1, szReceiver, sizeof(szReceiver));

	OBJID nIgnoreServerIndex=nServerIndex;///�㲥ʱĬ�Ϻ�������ķ���������
	Role* ptrRole=RoleManager::Instance().GetRole(szSender);
	if(ptrRole)
		nIgnoreServerIndex=ptrRole->idServer;
	if(strcmp(szReceiver,ALLUSERS_NAME)==0)
	{
		BroadcastRelayMsg(msg,nIgnoreServerIndex);///ȫ�߹㲥
// 		MsgManager::Instance().BroadcastRelayMsg(msg,nIgnoreServerIndex);///ȫ�߹㲥
	}
	else
	{
		ptrRole=RoleManager::Instance().GetRole(szReceiver);
		if(ptrRole)
		{
			SendRelayMsg(ptrRole->idServer,msg);
// 			if(info.unTxtAttribute ==_TXTATR_PRIVATE && (ptrRole->idServer != nIgnoreServerIndex))
// 			{
// 				Role* ptrRole=RoleManager::Instance().GetRole(szSender);
// 				if(ptrRole)
// 					MsgManager::Instance().SendRelayMsg(ptrRole->idServer, msg);
// 			}
		}
		else
		{
			///Ŀ�겻���߷��ضԷ�������Ϣ
			ptrRole=RoleManager::Instance().GetRole(szSender);
			if(info.unTxtAttribute==_TXTATR_CHATWITH)
			{
				///�Ҳ��������߷���Դ��������
				SendRelayMsg(nServerIndex,msg);
				return;
			}

			if(ptrRole)
			{
				SendSysMsg(ptrRole->idServer,ptrRole->strRoleName,_TXTATR_TOPSHOW,STR_TARGET_OFFLINE);

// 				CMsgTalk msg;
// 				if(msg.Create(SYSTEM_NAME, szSender, STR_TARGET_OFFLINE))
// 					MsgManager::Instance().SendRelayMsg(ptrRole->idServer,msg);
			}
		}
	}
	DEBUG_CATCH("MsgManager::ProcessTalkMsg");
}
// 
// void MsgManager::ProcessAccountMsg(int nServerIndex,CMsgAccount* pMsg)
// {
// 
// }

void MsgManager::ProcessSyndicateMemberInfoMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	BroadcastRelayMsg(msg,nServerIndex);///ȫ�߹㲥 nServerIndex
	CMsgSynMemberInfo::MSG_Info& info=*(CMsgSynMemberInfo::MSG_Info*)msg.m_bufMsg;
	OBJID idSyn=info.memInfo[0].idUser;
	///ͬ��ʱ����ָ������
	UnLock(idSyn);
	DEBUG_CATCH("MsgManager::ProcessSyndicateMemberInfoMsg");
//	MsgManager::Instance().BroadcastRelayMsg(msg,0);///ȫ�߹㲥 nServerIndex
}

///ת����Ϣ����ϵ��������
void MsgManager::SendRelationRelayMsg(int nServerIndex,OBJID idMsg,const char* pack, int nLen,int nExtra)
{
	RelayData packet;
	memcpy(&packet.buffer,pack,nLen);
	memcpy(packet.buffer+nLen,&nExtra,sizeof(nExtra));
	nLen+=8;
	packet.unMsgSize	= nLen;
	packet.unMsgType	= idMsg;
	g_pPointThread->m_aServerSocket[nServerIndex].Send((const char*)&packet, nLen);
}

void MsgManager::SendSysMsg(int nServerIndex,const char* szRever,int nChannel,const char* fmt, ...)
{ 
	MSGBUF	szMsg;
    vsprintf( szMsg, fmt, (char*) ((&fmt)+1) );
	
	CMsgTalk msg;
	if (msg.Create(SYSTEM_NAME, szRever, szMsg, NULL, 0xff0000, _TXTATR_TOPSHOW))
		SendRelationRelayMsg(nServerIndex,msg.GetType(),(const char*)&msg.m_pInfo->dwWordsColor,msg.GetSize());
}

void MsgManager::ProcessItemMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	CMsgItem::MSG_Info& info=*(CMsgItem::MSG_Info*)msg.m_bufMsg;
	OBJID idUser=0;
	memcpy(&idUser,msg.m_bufMsg+msg.GetSize()-4,4);///��ȡextra����,���汣�����Դ�û���id
	Role* ptrRole=NULL,*ptrSrcRole=NULL;
	switch (info.usAction)
	{
		case ITEMACT_GETINFO:
		case ITEMACT_GETEUDEMONINFO:///�������
		{
			ptrSrcRole=RoleManager::Instance().GetRole(info.m_itemInfo[0].typeId);
			ptrRole=RoleManager::Instance().GetRole(info.idTarget);
			if(ptrRole==NULL)
				ptrRole=RoleManager::Instance().GetRole(info.szTargetName);
			if(ptrRole)
			{
				///����ID��ֹ���ָ���
				info.idTarget=ptrRole->idRole;
				if(ptrSrcRole)
					strncpy(info.szTargetName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);
				///�Է����߷����Է����ڷ���������Զ�̲�ѯ
				SendRelayMsg(ptrRole->idServer,msg);
			}
			else///�Է�������,���ش�����Ϣ
			{
//				ptrRole=RoleManager::Instance().GetRole(info.m_itemInfo[0].typeId);
				if(ptrSrcRole)
					SendSysMsg(nServerIndex,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,"�Է�����̫Զ������");
			}
		}
		break;
// 	case _FRIEND_ADDSUCCESS:///�Ӻ��ѳɹ�����֪ͨ�����߶Է��ǲ������ߣ���֪ͨ���������˼���
// 		{
// 			
// 			ptrRole=RoleManager::Instance().GetRole(info.idFriend);
// 			if(ptrRole)
// 			{
// 				///����Ϣ������ʱ�л��ɷ�����
// 				ptrSrcRole=RoleManager::Instance().GetRole(idUser);
// 				if(ptrRole)
// 					strncpy(info.szName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);
// 				info.ucOnlineFlag = CMsgFriend::ONLINE_STATUS;
// 				SendRelayMsg(ptrRole->idServer,msg);///֪ͨ���������˼���,���߼Ӻ��ѳɹ�����Ҫ֪ͨ�Է�
// 				strncpy(info.szName,ptrRole->strRoleName,_MAX_NAMESIZE);
// 			}
// 			SendRelayMsg(nServerIndex,msg);
// 		}
// 		break;
// 	case _FRIEND_OFFLINE:
// 	case _FRIEND_ONLINE:
// 		{
// 			///��Һ������ߵ�Դ���ID���ڸ���������
// 			ptrRole=RoleManager::Instance().GetRole(idUser);
// 			if(ptrRole)
// 			{
// 				SendRelayMsg(ptrRole->idServer,msg);
// 			}
// 		}
// 		break;
// 	case _FRIEND_DELFRIEND:
// 		{
// 			ptrRole=RoleManager::Instance().GetRole(info.idFriend);
// 			if(ptrRole)
// 			{
// 				//info.ucOnlineFlag = CMsgFriend::ONLINE_STATUS;
// 				SendRelayMsg(ptrRole->idServer,msg);
// 			}
// 		}
// 		break;
		// 	case raRoleOffline:///�������
		// 		RoleManager::Instance().OfflineRole(info.idRole);
		// 		break;
		// 	case raQueryRole_Friend:///��ѯ�������
		// 		RoleManager::Instance().QueryRole(nServerIndex,msgAction,info.dstRoleID,info.idRole);
		// 		break;
	default:
		break;
	}
	DEBUG_CATCH("MsgManager::ProcessMsg");	
}

void MsgManager::ProcessWeapenSoulMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	CMsgWeaponSoul::MSG_Info& info=*(CMsgWeaponSoul::MSG_Info*)msg.m_bufMsg;
	Role* ptrRole=NULL,*ptrSrcRole=NULL;
	switch (info.usAction)
	{
	case EM_MSGWPSL_QUERY:
		{
			ptrSrcRole=RoleManager::Instance().GetRole(info.stWpSl[0].idItem);
			ptrRole=RoleManager::Instance().GetRole(info.idUser);

			if(ptrRole)
			{
				///����ID��ֹ���ָ���
				info.idUser=ptrRole->idRole;
				///�Է����߷����Է����ڷ���������Զ�̲�ѯ
				SendRelayMsg(ptrRole->idServer,msg);
			}
			else///�Է�������,���ش�����Ϣ
			{
				if(ptrSrcRole)
					SendSysMsg(nServerIndex,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,"�Է�����̫Զ������");
			}
		}
		break;
	case EM_MSGWPSL_UPDATE:
		{
			ptrSrcRole=RoleManager::Instance().GetRole(info.stWpSl[0].idItem);
			ptrRole=RoleManager::Instance().GetRole(info.idUser);
			
			if(ptrSrcRole)
			{
				///�Է����߷����Է����ڷ���������Զ�̲�ѯ
				SendRelayMsg(ptrSrcRole->idServer,msg);
			}
		}
		break;
	default:
		break;
	}
	DEBUG_CATCH("MsgManager::ProcessMsg");	
}

void MsgManager::ProcessMainServerMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	CMsgMainServer::MSG_Info_A& info=*(CMsgMainServer::MSG_Info_A*)msg.m_bufMsg;
	CMsgMainServer msg;
	msg.Create(info.dwMainServer, info.dwData, true);
	BroadcastRelayMsg(msg, nServerIndex);
	DEBUG_CATCH("MsgManager::ProcessMainServerMsg");
}
void MsgManager::ProcessRegisterMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	BroadcastRelayMsg(msg, nServerIndex);
	DEBUG_CATCH("MsgManager::ProcessRegisterMsg");
}
void MsgManager::ProcessTutorMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	CMsgSchoolMember::MSG_Info& info=*(CMsgSchoolMember::MSG_Info*)msg.m_bufMsg;
	switch(info.ucAction)
	{
	case MESSAGEBOARD_SYNLINE:
		BroadcastRelayMsg(msg, nServerIndex);
		break;
	case MESSAGEBOARD_LINEBROAD:
		{
			OBJID idUser = 0;
			if (info.ucPage == LINEBROAD_APPLYTUTOR || info.ucPage == LINEBROAD_ONLINESTATE || info.ucPage == LINEBROAD_ONLINESTATEREF)
				idUser = info.idUser;
			else if (info.ucPage == LINEBROAD_APPLYBYNAME)
			{
				Role* pRole = NULL;
				if(info.setMember[0].szName && strlen(info.setMember[0].szName)>0)
					Role* pRole = RoleManager::Instance().GetRole(info.setMember[0].szName);

				Role* ptrSrcRole = RoleManager::Instance().GetRole(info.idUser);
				if(!ptrSrcRole)
					return;

				if (pRole)
				{
					info.setMember[0].idMember = pRole->idRole;//ID���ɽ�����
					strncpy(info.setMember[0].szName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);//���ֻ��ɷ�����
					SendRelayMsg(pRole->idServer,msg);
				}
				else
				{
					SendSysMsg(ptrSrcRole->idServer,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,"�Է�������");
				}

				return;
			}
			else
				idUser = info.setMember[0].idMember;
			Role* pRole = RoleManager::Instance().GetRole(idUser);
			if (pRole)
			{
				SendRelayMsg(pRole->idServer,msg);
			}
		}
		break;
	default:
		break;
	}
	DEBUG_CATCH("MsgManager::ProcessRegisterMsg");
}
//�����������Ϣ���͵Ļ�һ��Ҫ��CPointThread::ProcessMsg����Ҳ���ϣ���������־
void MsgManager::ProcessMsg(int nServerIndex,  char * pBuf, int nLen)
{
	DEBUG_TRY
	unsigned short unMsgType = CNetMsg::GetType(pBuf, nLen);
	CNetMsg msg;
	msg.Create(pBuf, nLen);
	switch (unMsgType)
	{
		case _MSG_ACCOUNT:
//			ProcessAccountMsg(nServerIndex,(CMsgAccount*)&msg);	
			break;
		case _MSG_RELAY:
			ProcessRelayMsg(nServerIndex,msg);
			break;
		case _MSG_TALK:
			ProcessTalkMsg(nServerIndex,msg);
			break;
		case _MSG_FRIEND:
			ProcessFriendMsg(nServerIndex,msg);
			break;
		case _MSG_SYNDICATE:
			ProcessSyndicateMsg(nServerIndex,msg);
			break;
		case _MSG_ITEMINFO:
		case _MSG_EUDEMONINFO:
			ProcessEudemoniInfoMsg(nServerIndex,msg);
			break;
		case _MSG_SYNINFO:
			ProcessSyndicateInfoMsg(nServerIndex,msg);
			break;
		case _MSG_SYNMEMBERINFO:
			ProcessSyndicateMemberInfoMsg(nServerIndex,msg);
			break;
		case _MSG_ITEM:
			ProcessItemMsg(nServerIndex,msg);
			break;
		case _MSG_WEAPONSOUL:
			ProcessWeapenSoulMsg(nServerIndex,msg);
			break;
		case _MSG_MAINSERVER:
			ProcessMainServerMsg(nServerIndex, msg);
			break;
		case _MSG_REGISTER:
			ProcessRegisterMsg(nServerIndex, msg);
			break;
		case _MSG_SCHOOLMEMBER:
			ProcessTutorMsg(nServerIndex, msg);
			break;
		case _MSG_VIPLIST:
			ProcessRegisterMsg(nServerIndex, msg);//ֻ��ת��һ��
			break;
		case _MSG_ACCLOCK:
			ProcessRegisterMsg(nServerIndex,msg);
			break;
		case _MSG_WARGAME:
			ProcessRegisterMsg(nServerIndex, msg);//ֻ��ת��һ��
			break;
		default:
			break;
	}
	DEBUG_CATCH("MsgManager::ProcessMsg");
}
