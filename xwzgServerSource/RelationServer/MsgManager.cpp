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

///转发消息到关系服务器上
void MsgManager::SendRelayMsg(int nServerIndex,CNetMsg &msg)
{
	g_pPointThread->m_aServerSocket[nServerIndex].Send(msg.GetBuf(), msg.GetSize());
}

void MsgManager::UpServerListUI()
{
	char *szServerInfo=new char[256];
	ZeroMemory(szServerInfo,sizeof(szServerInfo));
	int nServerInfoOffset=0;
	for(int i = 1; i < MAXGAMESERVERS; i++)	// 保留0，0为错误
	{
		if(g_pPointThread->m_aServerSocket[i].IsOpen())
		{
			///套按字打开,但未收到msgaccount消息的服务器也不在服务范围
			if(strlen(g_pPointThread->m_aServerName[i])<=0)
				continue;

			if(nServerInfoOffset>256)
				break;///缓冲区溢出，不玩了
			nServerInfoOffset=strlen(szServerInfo);
			sprintf(szServerInfo+nServerInfoOffset,"%s:%d;",g_pPointThread->m_aServerName[i],RoleManager::Instance().GetRoleCountAtServer(i));
		}
	}
	if(strlen(szServerInfo)<=0)
		sprintf(szServerInfo,"所有服务器离线");
	PrintText(szServerInfo);
  	//AfxGetMainWnd()->PostMessage(WM_RBUTTONDBLCLK,(WPARAM)szServerInfo);
}

void MsgManager::OnBreathe()///专门做一些定时任务
{
	DEBUG_TRY
	const UINT nUpdateServerListDelay=10000;//1000*60*2;///2分钟
	static UINT nUpdateServerListTime=GetTickCount();
	if(GetTickCount()>(nUpdateServerListTime+nUpdateServerListDelay))
	{
		nUpdateServerListTime=GetTickCount();
		BroadcastGameServerListMsg();///到点广播一下在线服务器列表到游戏服务器中缓存
	}

	///更新一下各服器在线人数
	const UINT nUpdateUIServerListDelay=1000*5;///5秒
	static UINT nUpdateUIServerListTime=GetTickCount();
	if(GetTickCount()>(nUpdateUIServerListTime+nUpdateUIServerListDelay))
	{
		nUpdateUIServerListTime=GetTickCount();
		UpServerListUI();///到点广播一下在线服务器列表到游戏服务器中缓存
	}

	BreakTimeoutSynLock();
	BreakTimeoutUserLock();
	DEBUG_CATCH("MsgManager::OnBreathe");
}

void MsgManager::BroadcastGameServerListMsg(int nIgnoreServerIndex)
{
	CMsgTalk msg;
	MakeGameServerList2TalkMsg(msg);///构建消息
	BroadcastRelayMsg(msg,nIgnoreServerIndex);///广播消息
}
										 
void MsgManager::ProcessMsgServerDisconnect(int nServerIndex)///处理游戏服务器掉线的情况
{
	DEBUG_TRY
	RoleManager::Instance().PureServerRoleList(nServerIndex);///当服务器与关系服务器失去连接后，清空跟它相关的用户信息
	///将服务器列表组成msgtalk消息广播给所有在线服务器去更新
	BroadcastGameServerListMsg();
	DEBUG_CATCH("MsgManager::ProcessMsgServerDisconnect");
}
//任命主服务器
void MsgManager::AppointMainServer(int nIgnoreServerIndex)
{
	CMsgMainServer msg;

	for(int i =1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
	{
		if (i==nIgnoreServerIndex && stricmp(g_szMainServer, g_pPointThread->m_aServerName[i])==0)//加这一句，以防之前没有去掉已失效的主游戏服务器
			strcpy(g_szMainServer,"");
		if(g_pPointThread->m_aServerSocket[i].IsOpen() && i!=nIgnoreServerIndex)
		{
			if (strlen(g_szMainServer)<=0)//当前没有有效的主游戏服务器
			{
				strcpy(g_szMainServer, g_pPointThread->m_aServerName[i]);
				PrintText("任命“%s”为主游戏服务器", g_pPointThread->m_aServerName[i]);
				if (msg.Create(1,0,true))
					g_pPointThread->m_aServerSocket[i].Send(msg.GetBuf(), msg.GetSize());//通知主游戏服务器
				if (msg.Create(0,0,true))
					BroadcastRelayMsg(msg,i);//通知非主游戏服务器
				break;
			}
			else//已经有了主游戏服务器，只广播一下
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
		PrintText("无游戏服务器在线，无法任命");
}
///广播消息到所有游戏服务器上
void MsgManager::BroadcastRelayMsg(CNetMsg &msg,int nIgnoreServerIndex)
{
	for(int i =1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
	{
		if(g_pPointThread->m_aServerSocket[i].IsOpen() && i!=nIgnoreServerIndex)///源服不广播
		{
			g_pPointThread->m_aServerSocket[i].Send(msg.GetBuf(), msg.GetSize());
		}
	}
}

///发送消息到关系服务器上
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

///踢掉所有线上这个玩家名
bool MsgManager::KickoutRole(OBJID idRole,int nOldServerIndex)
{
	bool hr=false;
	Role* ptrRole=RoleManager::Instance().GetRole(idRole);
	if(ptrRole)
	{
// 		CMsgFee	cMsg;
// 		cMsg.Create(idRole, CMsgFee::FEE_KICKOUT);
// 		///踢掉前一个
// 		SendRelationRelayMsg(ptrRole->idServer,cMsg.GetType(),(const char*)&cMsg.m_pInfo->idAccount,cMsg.GetSize());
// 		///踢掉新来的
// 		SendRelationRelayMsg(nOldServerIndex,cMsg.GetType(),(const char*)&cMsg.m_pInfo->idAccount,cMsg.GetSize());

		///检测到重复的角色在线，出日志
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
	case raRoleOnline:///玩家上线
 //		if(KickoutRole(info.idRole,nServerIndex)==false)///踢掉前面上线的用户
		KickoutRole(info.idRole,nServerIndex);
		RoleManager::Instance().OnlineRole(info.idRole,info.szUserName,nServerIndex);
		break;
	case raRoleOffline:///玩家下线
		RoleManager::Instance().OfflineRole(info.idRole);
		break;
	case raQueryRole_Friend:///查询其它玩家
		RoleManager::Instance().QueryRole(nServerIndex,msgAction,info.dstRoleID,info.idRole);
		break;
	default:
		break;
	}
	DEBUG_CATCH("MsgManager::ProcessRelayMsg");	
}

void MsgManager::ProcessSyndicateMsg(int nServerIndex,CNetMsg& msg)
{
//	MsgManager::Instance().BroadcastRelayMsg(msg,nServerIndex);///全线广播 nServerIndex
	DEBUG_TRY

	CMsgSyndicate::MSG_Info& info=*(CMsgSyndicate::MSG_Info*)msg.m_bufMsg;

	char szSender[_MAX_NAMESIZE];
	
	CNetStringPacker	StrPacker;
	StrPacker.SeekBuf(info.szBuf, _MAX_MSGSIZE-sizeof(info)+1);
	StrPacker.GetString(0, szSender, sizeof(szSender));
	Role* ptrRole=RoleManager::Instance().GetRole(szSender);
	OBJID idSyndicate=0;
	memcpy(&idSyndicate,msg.m_bufMsg+msg.GetSize()-4,4);///提取extra数据,里面保存的是源用户的id
 	SyndicateAction msgAction=(SyndicateAction)info.usAction;
	switch (msgAction)
 	{
		///同时批准时还要加用户锁
		case SYN_ANSERR_OK:
		{
			if(Lock(idSyndicate) && LockUser(info.idTarget))
			{
				///锁定成功,发还消息
				if(ptrRole)
				{
					SendRelayMsg(ptrRole->idServer,msg);
				}
			}
			else
			{
				if(ptrRole)
				{
					///锁定失败,吃掉这个消息,并发回错误信息
					SendSysMsg(ptrRole->idServer,ptrRole->strRoleName,_TXTATR_TOPSHOW,"帮派系统忙,请稍候再试");///非好友离线
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
				///锁定成功,发还消息
				if(ptrRole)
				{
					SendRelayMsg(ptrRole->idServer,msg);
				}
			}
			else
			{
				if(ptrRole)
				{
					///锁定失败,吃掉这个消息,并发回错误信息
					SendSysMsg(ptrRole->idServer,ptrRole->strRoleName,_TXTATR_TOPSHOW,"帮派系统忙,请稍候再试");///非好友离线
				}
			}
		}
		break;
//		case SYN_ANNONCE_OTHERLINE:
// 		case SYN_CREATECOMPLETE:///军团创建成功广播到其它线
// 		case SYN_CREATE_OK:///帮派创建成功,转发到除源服之外的所有服务器去创建帮派
// 			MsgManager::Instance().BroadcastRelayMsg(msg,nServerIndex);///全线广播 nServerIndex
// 			break;
// 	// 	case raRoleOffline:///玩家下线
// 	// 		RoleManager::Instance().OfflineRole(info.idRole);
// 	// 		break;
// 	// 	case raQueryRole_Friend:///查询其它玩家
// 	// 		RoleManager::Instance().QueryRole(nServerIndex,msgAction,info.dstRoleID,info.idRole);
// 	// 		break;
 		default:
			BroadcastRelayMsg(msg,nServerIndex);///全线广播 nServerIndex
 			break;
 	}
	DEBUG_CATCH("MsgManager::ProcessSyndicateMsg");
	
}


void MsgManager::ProcessSyndicateInfoMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	CMsgSynInfo::MSG_Info& info=*(CMsgSynInfo::MSG_Info*)msg.m_bufMsg;
	BroadcastRelayMsg(msg,nServerIndex);///全线广播 nServerIndex	
	DEBUG_CATCH("MsgManager::ProcessSyndicateInfoMsg");
}

void MsgManager::ProcessEudemoniInfoMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	OBJID idUser=0;
	memcpy(&idUser,msg.m_bufMsg+msg.GetSize()-4,4);///提取extra数据,里面保存的是源用户的id
	Role* ptrRole=NULL;
	ptrRole=RoleManager::Instance().GetRole(idUser);
	if(ptrRole)
	{
		SendRelayMsg(ptrRole->idServer,msg);
	}
	DEBUG_CATCH("MsgManager::ProcessEudemoniInfoMsg");
}

///解除所有用户已超时的锁
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

///解除所有帮派已超时的锁
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

bool MsgManager::LockUser(OBJID idUser)///锁定角色,true为锁定成功,false为锁定失败
{
	bool hr=false;
	if(m_userLockStateList[idUser]==0)
	{
		m_userLockStateList[idUser]=GetTickCount()+10000;///超时解锁时间为10秒
		hr=true;///锁定成功
	}
	return hr;
}

void MsgManager::UnLockUser(OBJID idUser)///解锁角色
{
	m_userLockStateList[idUser]=false;
}

bool MsgManager::Lock(OBJID idSyndicate)///锁定帮派
{
	bool hr=false;
	if(m_synLockStateList[idSyndicate]==0)
	{
		m_synLockStateList[idSyndicate]=GetTickCount()+10000;///超时解锁时间为10秒
		hr=true;///锁定成功
	}
	return hr;
}

void MsgManager::UnLock(OBJID idSynDicate)///解锁帮派
{
	m_synLockStateList[idSynDicate]=false;
}

///游戏服务器列表组成MsgTalk消息
void MsgManager::MakeGameServerList2TalkMsg(CMsgTalk& msg)
{
	///取得客户端适合去的服务器索引
	///取得在线服务器列表
//	int nMode =0;
//	char szServerURL[URLSTRSIZE] = "N/A";
	char szServerInfo[64*5]="";
//	char szTemp[64*5]="";
	int nFullNum=800;///规定800人为服务器最高负载
	int ServerCount=0;
	int nServerInfoOffset=0;///标出组包时的偏移位置
//	UINT nServerPort=0;
	for(int i = 1; i < MAXGAMESERVERS; i++)	// 保留0，0为错误
	{
		if(g_pPointThread->m_aServerSocket[i].IsOpen())
		{
			///套按字打开,但未收到msgaccount消息的服务器也不在服务范围
			if(strlen(g_pPointThread->m_aServerName[i])<=0)
				continue;
			///默认取IP地址
// 			nMode = g_pPointThread->m_nServerLoginMode[i];///得到模式
// 			if(nMode==1)
// 				strcpy(szServerURL,g_pPointThread->m_aServerSocket[i].GetPeerIP());
// 			else if(nMode == 2)///网址模式取域名
// 				strcpy(szServerURL,g_pPointThread->m_aServerURL[i]);
// 			else
// 			{
// 				char szLog[512];
// 				sprintf(szLog,"服务端地址模式不对,szServer:%s",g_pPointThread->m_aServerName[i]);
// 				::LogSave(szLog);
// 				continue;
// 			}
			if(ServerCount>5)///最大只支持一条线5个服务器
				break;
			nServerInfoOffset=strlen(szServerInfo);
			if (strcmp(g_pPointThread->m_aServerName[i],"六线")==0)
				sprintf(szServerInfo+nServerInfoOffset,"s:%s:%d:%d;","N/A",nFullNum,RoleManager::Instance().GetRoleCountAtServer(i));
			else
				sprintf(szServerInfo+nServerInfoOffset,"%s:%s:%d:%d;",g_pPointThread->m_aServerName[i],"N/A",nFullNum,RoleManager::Instance().GetRoleCountAtServer(i));
			ServerCount++;///服务器数量加1
		}
	}
//	PrintText("服务器列表:%s",szServerInfo);
	if (strlen(g_szMainServer)<=0)
		PrintText("警告：没有任命主服务器");
	msg.Create(_TXTATR_MSG_SERVERLIST,szServerInfo);
}

void MsgManager::ProcessFriendMsg(int nServerIndex,CNetMsg& msg)
{
	DEBUG_TRY
	CMsgFriend::MSG_Info& info=*(CMsgFriend::MSG_Info*)msg.m_bufMsg;
	OBJID idUser=0;
	memcpy(&idUser,msg.m_bufMsg+msg.GetSize()-4,4);///提取extra数据,里面保存的是源用户的id
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
				info.idFriend=ptrRole->idRole;///篡改消息，很杯具
				info.dwLookFace=idUser;
				info.ucAction=_FRIEND_QUERYFRIENDINFO;
				SendRelayMsg(ptrRole->idServer,msg);///通知接受者有人加他,跨线加好友成功后需要通知对方
			}
			else
			{
				ptrSrcRole=RoleManager::Instance().GetRole(idUser);
				if(ptrSrcRole)
				{
					if(info.ucLevel>0)///由等级来判断这是否是一个好友的离线快照数据
					{
						SendRelayMsg(ptrSrcRole->idServer,msg);///好友离线消息，再还给它
					}
					else
						SendSysMsg(ptrSrcRole->idServer,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,STR_TARGET_OFFLINE);///非好友离线
// 					CMsgTalk msg
// 					CMsgTalk msg;
// 					if(msg.Create(SYSTEM_NAME, ptrSrcRole->strRoleName, STR_TARGET_OFFLINE))
// 						MsgManager::Instance().SendRelayMsg(ptrSrcRole->idServer,msg);
				}
			}
		}
			break;
		case _FRIEND_GETINFO:///玩家上线
		{
			///判断请求信息的玩家是否在线,如果玩家在其它线服务器活着则将这个消息回给源服务器
			//OBJID nIgnoreServerIndex=nServerIndex;///广播时默认忽略请求的服务器索引
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
				sprintf(szTemp,"<font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">拒绝了您的好友请求!</font>",ptrSrcRole->strRoleName);
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

					///改变消息源与目标信息
					strncpy(info.szName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);

					info.idFriend=ptrRole->idRole;
					///对方同意加好友,将此消息转发到地图组处理
					SendRelayMsg(ptrRole->idServer,msg);///通知接受者有人加他,跨线加好友成功后需要通知对方

				}
			}
			break;
		case _FRIEND_ADDSUCCESS_PASSIVE:///被动添加好友成功时发送
		{
//			ptrSrcRole=RoleManager::Instance().GetRole(info.idFriend);
			ptrRole=RoleManager::Instance().GetRole(info.idFriend);
			if(ptrRole)
			{
				///将消息名字暂时切换成发送者
				info.ucOnlineFlag = CMsgFriend::ONLINE_STATUS;
				SendRelayMsg(ptrRole->idServer,msg);
			}
			///篡改
			info.ucAction=_FRIEND_ADDSUCCESSREPLY;///一路篡改
			SendRelayMsg(nServerIndex,msg);

		}
			break;
		case _FRIEND_ADDFRIEND:
//		case _FRIEND_ADDSUCCESS:///加好友成功后先通知发起者对方是不否在线，再通知接受者有人加他
		{
			ptrSrcRole=RoleManager::Instance().GetRole(info.idFriend);
			ptrRole=RoleManager::Instance().GetRole(info.szName);
			if(ptrRole)
			{
				///将消息名字暂时切换成发送者
 				if(ptrSrcRole)
 					strncpy(info.szName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);

				info.dwLookFace=ptrSrcRole->idRole;
// 
// 				info.ucOnlineFlag = CMsgFriend::ONLINE_STATUS;
				info.idFriend=ptrRole->idRole;
				SendRelayMsg(ptrRole->idServer,msg);///通知接受者有人加他,跨线加好友成功后需要通知对方
// 				strncpy(info.szName,ptrRole->strRoleName,_MAX_NAMESIZE);
			}
			else
			{
				///对方不在线
				SendSysMsg(ptrSrcRole->idServer,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,"对方不在线，不能加为好友");
			}
// 			///篡改
// 			info.ucAction=_FRIEND_ADDSUCCESSREPLY;///一路篡改
// 			SendRelayMsg(nServerIndex,msg);
		}
			break;
		case _FRIEND_REPLYFRIENDINFO:
			info.ucAction=_FRIEND_GETFRIENDINFO;///一路篡改
///			break;   ///这里确实没有break
		case _FRIEND_OFFLINE:
		case _FRIEND_ONLINE:
			{
				///玩家好友上线的源玩家ID是在附加数据中
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
// 	case raRoleOffline:///玩家下线
// 		RoleManager::Instance().OfflineRole(info.idRole);
// 		break;
// 	case raQueryRole_Friend:///查询其它玩家
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

	OBJID nIgnoreServerIndex=nServerIndex;///广播时默认忽略请求的服务器索引
	Role* ptrRole=RoleManager::Instance().GetRole(szSender);
	if(ptrRole)
		nIgnoreServerIndex=ptrRole->idServer;
	if(strcmp(szReceiver,ALLUSERS_NAME)==0)
	{
		BroadcastRelayMsg(msg,nIgnoreServerIndex);///全线广播
// 		MsgManager::Instance().BroadcastRelayMsg(msg,nIgnoreServerIndex);///全线广播
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
			///目标不在线发回对方离线消息
			ptrRole=RoleManager::Instance().GetRole(szSender);
			if(info.unTxtAttribute==_TXTATR_CHATWITH)
			{
				///找不到接受者发回源服务器中
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
	BroadcastRelayMsg(msg,nServerIndex);///全线广播 nServerIndex
	CMsgSynMemberInfo::MSG_Info& info=*(CMsgSynMemberInfo::MSG_Info*)msg.m_bufMsg;
	OBJID idSyn=info.memInfo[0].idUser;
	///同步时解锁指定帮派
	UnLock(idSyn);
	DEBUG_CATCH("MsgManager::ProcessSyndicateMemberInfoMsg");
//	MsgManager::Instance().BroadcastRelayMsg(msg,0);///全线广播 nServerIndex
}

///转发消息到关系服务器上
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
	memcpy(&idUser,msg.m_bufMsg+msg.GetSize()-4,4);///提取extra数据,里面保存的是源用户的id
	Role* ptrRole=NULL,*ptrSrcRole=NULL;
	switch (info.usAction)
	{
		case ITEMACT_GETINFO:
		case ITEMACT_GETEUDEMONINFO:///玩家上线
		{
			ptrSrcRole=RoleManager::Instance().GetRole(info.m_itemInfo[0].typeId);
			ptrRole=RoleManager::Instance().GetRole(info.idTarget);
			if(ptrRole==NULL)
				ptrRole=RoleManager::Instance().GetRole(info.szTargetName);
			if(ptrRole)
			{
				///放入ID防止名字干扰
				info.idTarget=ptrRole->idRole;
				if(ptrSrcRole)
					strncpy(info.szTargetName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);
				///对方在线发到对方所在服务器进行远程查询
				SendRelayMsg(ptrRole->idServer,msg);
			}
			else///对方不在线,返回错误信息
			{
//				ptrRole=RoleManager::Instance().GetRole(info.m_itemInfo[0].typeId);
				if(ptrSrcRole)
					SendSysMsg(nServerIndex,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,"对方距离太远或不在线");
			}
		}
		break;
// 	case _FRIEND_ADDSUCCESS:///加好友成功后先通知发起者对方是不否在线，再通知接受者有人加他
// 		{
// 			
// 			ptrRole=RoleManager::Instance().GetRole(info.idFriend);
// 			if(ptrRole)
// 			{
// 				///将消息名字暂时切换成发送者
// 				ptrSrcRole=RoleManager::Instance().GetRole(idUser);
// 				if(ptrRole)
// 					strncpy(info.szName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);
// 				info.ucOnlineFlag = CMsgFriend::ONLINE_STATUS;
// 				SendRelayMsg(ptrRole->idServer,msg);///通知接受者有人加他,跨线加好友成功后需要通知对方
// 				strncpy(info.szName,ptrRole->strRoleName,_MAX_NAMESIZE);
// 			}
// 			SendRelayMsg(nServerIndex,msg);
// 		}
// 		break;
// 	case _FRIEND_OFFLINE:
// 	case _FRIEND_ONLINE:
// 		{
// 			///玩家好友上线的源玩家ID是在附加数据中
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
		// 	case raRoleOffline:///玩家下线
		// 		RoleManager::Instance().OfflineRole(info.idRole);
		// 		break;
		// 	case raQueryRole_Friend:///查询其它玩家
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
				///放入ID防止名字干扰
				info.idUser=ptrRole->idRole;
				///对方在线发到对方所在服务器进行远程查询
				SendRelayMsg(ptrRole->idServer,msg);
			}
			else///对方不在线,返回错误信息
			{
				if(ptrSrcRole)
					SendSysMsg(nServerIndex,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,"对方距离太远或不在线");
			}
		}
		break;
	case EM_MSGWPSL_UPDATE:
		{
			ptrSrcRole=RoleManager::Instance().GetRole(info.stWpSl[0].idItem);
			ptrRole=RoleManager::Instance().GetRole(info.idUser);
			
			if(ptrSrcRole)
			{
				///对方在线发到对方所在服务器进行远程查询
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
					info.setMember[0].idMember = pRole->idRole;//ID换成接收者
					strncpy(info.setMember[0].szName,ptrSrcRole->strRoleName,_MAX_NAMESIZE);//名字换成发送者
					SendRelayMsg(pRole->idServer,msg);
				}
				else
				{
					SendSysMsg(ptrSrcRole->idServer,ptrSrcRole->strRoleName,_TXTATR_TOPSHOW,"对方不在线");
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
//在这里加了消息类型的话一定要在CPointThread::ProcessMsg里面也加上，否则会记日志
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
			ProcessRegisterMsg(nServerIndex, msg);//只是转发一下
			break;
		case _MSG_ACCLOCK:
			ProcessRegisterMsg(nServerIndex,msg);
			break;
		case _MSG_WARGAME:
			ProcessRegisterMsg(nServerIndex, msg);//只是转发一下
			break;
		default:
			break;
	}
	DEBUG_CATCH("MsgManager::ProcessMsg");
}
