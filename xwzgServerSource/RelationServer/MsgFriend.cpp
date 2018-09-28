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
//20061012修罗.新增两个参数
BOOL	CMsgFriend::Create	(int nAction, 
				 OBJID idFriend, 
				 LPCTSTR szName, 
				 short bOnline/* = false*/, 
				 DWORD nLookFace/*=0*/, 
				 int nLevel/*=0*/,
				 unsigned int profession/*职业=0*/,
				 unsigned int friendship/*友情度=0*/,
				 unsigned int nSex,
				 unsigned int nFriendType,
				 LPCTSTR szFeel,//[2010-06-05 goto]增加好友心情
				 int nRelation,//[2010-06-05 goto]关系,师徒、夫妻...
				 LPCTSTR szFaction,//帮派
				 LPCTSTR szTitle,//称号
				 LPCTSTR szMateName,//配偶
				 LPCTSTR szTeamName,//队伍
				 OBJID idMap//位置
				 )
{
	//20070528修罗:加强日志
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
	//20061012修罗.新增两个参数
	//------------------------------
	m_pInfo->ucProfession	= profession;
	m_pInfo->ucFriendship	= friendship;
	m_pInfo->ucSex          = nSex;
	//------------------------------
	m_pInfo->ucFriendType	= nFriendType;
	m_pInfo->ucRelation		= nRelation;
//	strcpy(m_pInfo->szName, szName, _MAX_NAMESIZE);

	BOOL bSucMake	=true;
	if(szFeel)//[2010-06-05 goto]好友心情
		bSucMake &= m_StrPacker.AddString(szFeel);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(szFaction)//帮派
		bSucMake &= m_StrPacker.AddString(szFaction);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(szTitle)//称号
		bSucMake &= m_StrPacker.AddString(szTitle);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(szMateName)//配偶
		bSucMake &= m_StrPacker.AddString(szMateName);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(szTeamName)//队伍
		bSucMake &= m_StrPacker.AddString(szTeamName);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(idMap)//位置
	{
		char szMap[10];
		sprintf(szMap,"%u",idMap);
		bSucMake &= m_StrPacker.AddString(szMap);
	}
	else
		bSucMake &= m_StrPacker.AddString("未知");

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
// 					//20061020修罗.发送职业----------------
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
// 			return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
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
// 			return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
// 			{
// 				OBJID idFriend = m_pInfo->idFriend;
// 				CUser* pTarget = UserManager()->GetUser(idFriend);
// 				if(pTarget)
// 				{
// 					sprintf(szDebugStep,"%d",1);
// 					pTarget->DelFriend(m_pInfo->idFriend);
// 					sprintf(szDebugStep,"%d",2);
// 					if (pUser)
// 						pUser->DelBeFriend(pTarget->GetID());//独孤求败[2008/12/17]
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
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
// 		{
// 			CUserPtr	pTarget = NULL;
// 			sprintf(szDebugStep,"%d",1);
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if (!pTarget)
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(28023));
// 				return;
// 			}
// 			//---------本人添加或更新交互记录------------------------------------------------------------------------------------------
// 			CFriendContactHistory* pFriend= pUser->GetFriendContact(pTarget->GetID(),m_pInfo->ucFriendType);
// 			sprintf(szDebugStep,"%d",2);
// 			if ( pFriend  )
// 			{
// 				sprintf(szDebugStep,"%d",3);
// 				if (pUser->UpDateContactHistory(pFriend,pTarget->GetID(),time(NULL),m_pInfo->ucFriendType))//更改交互时间
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
// 				if ( pUser->AddFriendContact(pTarget->GetID(),m_pInfo->szName,m_pInfo->ucFriendType) )//添加交互记录
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
// 			//----------对方添加或更新交互记录--------------------------------------------------------------------------
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
// 				if ( pTarget->AddFriendContact(pUser->GetID(),pUser->GetName(),m_pInfo->ucFriendType) )//添加交互记录
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
// 	case _FRIEND_ADDFRIEND://由A发送的请求  [2010-06-07 goto]不需要B的同意，直接加为好友，同时通知B
// 		{
// 			if (pUser->GetID()==m_pInfo->idFriend || stricmp(pUser->GetName(),m_pInfo->szName)==0)
// 				return;
// 			if( m_pInfo->ucFriendType>MAX_GROUP || m_pInfo->ucFriendType<=0 )//好友分组,1-4组，5临时好友，6黑名单
// 				return;
// 
// 			if(m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP && pUser->GetFriendGroupAmount(m_pInfo->ucFriendType)>=MAX_GROUP_FRIEND )
// 			{
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
// 					pUser->SendMsg(&pMsg);
// 
// 				pUser->SendSysMsg("此好友分组已达到人数上限");
// 				return;
// 			}
// 
// 			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
// 			if (pFriend)//B已经是A的好友
// 			{
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
// 					pUser->SendMsg(&pMsg);
// 				char szTemp[128];
// 				sprintf(szTemp,"%s已经是你的好友",m_pInfo->szName);
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
// 						sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",m_pInfo->szName);
// 						pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
// 					}
// 				}
// 				else
// 				{
// 					pMsg.Create(_FRIEND_ADDFAIL,pTarget->GetID(),m_pInfo->szName,0,0,0,0,0,0,0,NULL,0);
// 					pUser->SendMsg(&pMsg);
// 					return;
// 				}
// 				if ( pTarget->AddBeFriend(0,pUser->GetID(),m_pInfo->ucFriendType) && m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP)//如果A已经是B的好友，不需要再通知B
// 				{
// 					if (pTarget->GetFriend( pUser->GetID()))
// 					{
// 						char szTemp[128];
// 						sprintf(szTemp,"<font color=\"#ffff00\">用户</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">将你加为了好友!</font>",pUser->GetName());
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
// 						sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",m_pInfo->szName);
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
// 	case _FRIEND_CHANGEGROUP://[2010-06-07 goto]更换好友分组
// 		{
// 			if( m_pInfo->ucFriendType>(MAX_GROUP+1) || m_pInfo->ucFriendType<=0 )//好友分组,1-4组，5临时好友，6黑名单
// 				return;
// 			
// 			if( pUser->GetFriendGroupAmount(m_pInfo->ucFriendType)>=MAX_GROUP_FRIEND )
// 			{
// 				CMsgFriend pMsg;
// 				if (pMsg.Create(_FRIEND_CHANGEFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
// 					pUser->SendMsg(&pMsg);
// 				
// 				pUser->SendSysMsg("此好友分组已达到人数上限");
// 				return;
// 			}
// 
// 			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
// 			if(!pFriend)
// 				return;
// 			
// 			unsigned int uoldType = pFriend->GetFriendType();
// 			//pFriend->SetFriendType(m_pInfo->ucFriendType);//修改好友分组
// 			if( pUser->MoveFriend(pFriend,m_pInfo->ucFriendType) )//修改好友分组
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
// 	case _FRIEND_DELFRIEND://[2010-06-08 goto]删除好友
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
// 	case _FRIEND_SETFEEL://[2010-06-08 goto]修改心情
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
// 	case _FRIEND_GETFRIENDINFO://获取好友的详细信息
// 		{
// 			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 
// 			if (!pFriend)//临时好友
// 			{
// 				if(!pTarget)
// 				{
// // 					CMsgFriend pMsg;
// // 					pMsg.Create(_FRIEND_GETFRIENDINFO,m_pInfo->idFriend,m_pInfo->szName,
// // 						0,//副职业
// // 						0,//lookface
// // 						0,//人物等级
// // 						0,//主职业
// // 						0,
// // 						0,//主职业等级
// // 						0,//副职业等级
// // 						NULL,
// // 						0);//关系
// // 					pUser->SendMsg(&pMsg);
// 					pUser->SendSysMsg(_TXTATR_TOPSHOW, "此人不在线，或在副本中");
// 				}
// 				else
// 				{
// 					char szUserTitle[10];
// 					sprintf(szUserTitle, "%u", pTarget->GetUseTitle());
// 					
// 					char szTeamName[32] = "无";
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
// 						pTarget->GetInfo()->SecJob,//副职业
// 						pTarget->GetLookFace(),//lookface
// 						pTarget->GetLev(),//人物等级
// 						pTarget->GetInfo()->FirJob,//主职业
// 						0,//友好度
// 						pTarget->GetInfo()->FirJobLev,//主职业等级
// 						pTarget->GetInfo()->SecJobLev,//副职业等级
// 						pTarget->GetFeel(),
// 						0,//关系
// 						pSyndicate==NULL? "无" : pSyndicate->GetStr(SYNDATA_NAME),//帮派
// 						szUserTitle,//称号
// 						pTarget->GetMate(),//配偶
// 						szTeamName,//队伍
// 						pTarget->GetMapID()//位置
// 						))
// 					pUser->SendMsg(&pMsg);
// 				}
// 				return;
// 			}
// 			if (!pTarget)//不在线时部分信息无法获取
// 			{
// 				char szUserTitle[10];
// 				sprintf(szUserTitle, "%u",pFriend->GetFriendTitle());
// 				OBJID idSyn = pFriend->GetFriendSynID();
// 				char szSyn[16] = "无";
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
// 							pFriend->GetFriendSecJob(),//副职业
// 							pFriend->GetFriendLookFace(),//lookface
// 							pFriend->GetFriendLevel(),//人物等级
// 							pFriend->GetFriendFirJob(),//主职业
// 							pFriend->GetFriendShip(),
// 							pFriend->GetFriendFirJobLev(),//主职业等级
// 							pFriend->GetFriendSecJobLev(),//副职业等级
// 							pFriend->GetFriendFeel(),
// 							0,//关系
// 							szSyn,//"帮派",
// 							szUserTitle,
// 							pFriend->GetFriendMateName()))
// 				pUser->SendMsg(&pMsg);
// 			}
// 			else
// 			{
// 				char szUserTitle[10];
// 				sprintf(szUserTitle, "%u", pTarget->GetUseTitle());
// 				
// 				char szTeamName[32] = "无";
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
// 							pTarget->GetProfession(2),//副职业
// 							pTarget->GetLookFace(),//lookface
// 							pTarget->GetLev(),//人物等级
// 							pTarget->GetProfession(2),//主职业
// 							pFriend->GetFriendShip(),
// 							pTarget->GetJobLev(1),//主职业等级
// 							pTarget->GetJobLev(2),//副职业等级
// 							pTarget->GetFeel(),
// 							0,//关系
// 							pSyndicate==NULL? "无" : pSyndicate->GetStr(SYNDATA_NAME),//帮派
// 							szUserTitle,//称号
// 							pTarget->GetMate(),//配偶
// 							szTeamName,//队伍
// 							pTarget->GetMapID()//位置
// 							))
// 				pUser->SendMsg(&pMsg);
// 			}
// 		}
// 		break;
// 	case _FRIEND_DELCONTACT://删除已经看过的留言
// 		{
// 			if(pUser->GetID()!=m_pInfo->idFriend)
// 				return;
// 			pUser->DelFriendLeaveWord(pUser->GetName(),m_pInfo->dwLookFace);
// 		}
// 		break;
// 	case _FRIEND_HAVETEAM://查看好友队伍信息
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
// 			if (!pTeam)//好友无队伍
// 			{
// 				nStep = 3;
// 				CMsgFriend msg;
// 				if (msg.Create(_FRIEND_HAVETEAM,0,pFriend->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// 					pUser->SendMsg(&msg);
// 			}
// 			else//好友有队伍
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
// 	case _FRIEND_HAVETEAM_MT://世界线程查找玩家的返回结果
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
// // 			if (!pTeam)//好友无队伍
// // 			{
// // 				CMsgFriend msg;
// // 				if (msg.Create(_FRIEND_HAVETEAM,0,pFriend->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// // 					pUser->SendMsg(&msg);
// // 			}
// // 			else//好友有队伍
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
// 	case _FRIEND_MOVETOGOODFRIENDACCEPT://B同意A的请求
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
// 		{
// 			if (m_pInfo->ucFriendType != 1)
// 				return;
// 			CUserPtr	pTarget = NULL;
// 			pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if (!pTarget)
// 				return;
// 			sprintf(szDebugStep,"%d",1);
// 			CFriend* pBeFriend = pTarget->GetFriend(pUser->GetID());//我必然已经他的某种好友
// 			if (!pBeFriend)
// 				return;
// 			CFriend* pFriend= pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID());//他是否也是我的某种好友
// 			sprintf(szDebugStep,"%d",2);
// 			if (!pFriend)//不是
// 			{
// 				sprintf(szDebugStep,"%d",3);
// 				if (pUser->AddFriend(/*m_pInfo->idFriend*/pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType)
// 					&& pTarget->MoveFriend(pBeFriend,1))
// 				{
// 					sprintf(szDebugStep,"%d",4);
// 					//-----A加了B为好友,则在B中保存A的信息----------
// 					pFriend = pUser->GetFriend(pTarget->GetID()/*m_pInfo->idFriend这里的m_pInfo->idFriend可能为零*/);
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
// 			else//是
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
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
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
// 			if (m_pInfo->ucFriendType == 1)//如果想移为GOOD FRIEND,必须要经过对方确认,相当于发送了一个申请加好友消息
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
// 	case _FRIEND_ADDTEMP://独孤求败[2008/12/12]加临时好友,无需对方确认
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
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
// 				if( pTarget->QueryEnemy()->GetName(pUser->GetID()))// 如果对方加你入黑名单，你无法申请加对方为好友 [10/10/2007]
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(16041));//-对方已经加您到黑名单，无法加为好友!
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
// 					//-----A加了B为好友,则在B中保存A的信息----------
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
// 						sprintf(szTemp,"你已经加%s为临时好友!!!",pTarget->GetName());
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
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
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
// 						//	回复
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
// 					// 如果对方未曾向自己发出过加入好友请求
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
// 					// 如果对方已经向自己发出过请求，直接为双方加好友
// 					//------------独孤求败[2008/12/16]查看我的好友圈子里有没有'这个人'---------
// 					sprintf(szDebugStep,"%d",10);
// 					if (pUser->GetFriend(pTarget->GetID() ))//如果有
// 					{
// 						sprintf(szDebugStep,"%d",11);
// 						if (pUser->MoveFriend(pUser->GetFriend(pTarget->GetID()),m_pInfo->ucFriendType)//就将其移到密友里
// 							&& pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucFriendType))
// 						{
// 							sprintf(szDebugStep,"%d",12);
// 							//-----A加了B为好友,则在B中保存A的信息----------
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
// 							pUser->SendMsg(this);//通知'我'将'这个人'MOVE到密友里
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
// 							pTarget->SendMsg(this);//通知'这个人'加我为密友成功，并将'我'的信息发送给他
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
// 						//-----A加了B为好友,则在B中保存A的信息----------
// 						CFriend* pFriend = pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID());
// 						pTarget->AddBeFriend(pFriend->GetID(),pUser->GetID(),pFriend->GetFriendType());
// 						//----------------------------
// 
// 						sprintf(szDebugStep,"%d",16);
// 						//-----A加了B为好友,则在B中保存A的信息----------
// 						pFriend = pTarget->GetFriend(pUser->GetID());
// 						pUser->AddBeFriend(pFriend->GetID(),pTarget->GetID(),pFriend->GetFriendType());
// 						//----------------------------
// 
// 						sprintf(szDebugStep,"%d",17);
// 						//20061020修罗bug新加好友时职业显示erro.
// 						//create新加了两个参数------------------------------------
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
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
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
// 						//	回复
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
// 			if( pTarget->QueryEnemy()->GetName(pUser->GetID()))// 如果对方加你入黑名单，你无法申请加对方为好友 [10/10/2007]
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(16041));//-对方已经加您到黑名单，无法加为好友!
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
// 				// 如果对方未曾向自己发出过加入好友请求
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
// 				// 如果对方已经向自己发出过请求，直接为双方加好友
// 				if (pUser->AddFriend(/*m_pInfo->idFriend*/pTarget->GetID(), /*m_pInfo->szName*/pTarget->GetName(),m_pInfo->ucFriendType) &&
// 						pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucFriendType))
// 				{
// 					sprintf(szDebugStep,"%d",7);
// 					//-----A加了B为好友,则在B中保存A的信息----------
// 					CFriend* pFriend = pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID());
// 					pTarget->AddBeFriend(pFriend->GetID(),pUser->GetID(),pFriend->GetFriendType());
// 					//----------------------------
// 					sprintf(szDebugStep,"%d",8);
// 					//-----A加了B为好友,则在B中保存A的信息----------
// 					pFriend = pTarget->GetFriend(pUser->GetID());
// 					pUser->AddBeFriend(pFriend->GetID(),pTarget->GetID(),pFriend->GetFriendType());
// 					//----------------------------
// 					sprintf(szDebugStep,"%d",9);
// 
// 					//20061020修罗bug新加好友时职业显示erro.
// 					//create新加了两个参数------------------------------------
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
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
// 		{
// 			CUser* pTarget = UserManager()->GetUser(m_pInfo->idFriend);		// 可能为NULL
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
// 					pTarget->DelBeFriend(pUser->GetID());//独孤求败[2008/12/17]
// 				//20090420haocq好友删除提示
// 				if (nFriendType == 1)
// 				{
// 					MSGBUF	szMsg;
// 					sprintf(szMsg,g_objStrRes.GetStr(13058) , pUser->GetName(), szFriend);//STR_BREAK_FRIEND
// 					pUser->BroadcastRoomMsg(szMsg, true);
// 				}
// 				else if (nFriendType == 2)
// 				{
// 					MSGBUF	szMsg;
// 					sprintf(szMsg,"你已将%s从临时好友列表中删除" , szFriend);//STR_BREAK_FRIEND
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
// 						pUser->DelBeFriend(pTarget->GetID());//独孤求败[2008/12/17]
// 					if(this->Create(_FRIEND_BREAK, pUser->GetID(), pUser->GetName()))
// 						pTarget->SendMsg(this);
// 				}
// 				sprintf(szDebugStep,"%d",6);
// 			}
// 			else if(!pTarget && nFriendType == 1)	// 对方不在线
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
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
// 		{
// 			pUser->QueryEnemy()->Del(m_pInfo->idFriend, UPDATE_TRUE);
// 		}
// 		break;
// 	case _ENEMY_APPLY:		//李京哲0211 目前只可以添加在线玩家入黑名单
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
// 		{
// 			CUser* pTarget = UserManager()->GetUser(m_pInfo->szName);
// 			if(pTarget&&pTarget->GetID() == pUser->GetID())
// 			{
// 				pUser->SendSysMsg("你不把自己加入黑名单！");//STR_NOT_ALLOW_TEAM
// 				return;
// 			}
// 			DWORD nLookFace = 0 ;
// 			int nLevel = 0 ;
// 			DWORD profession = 0 ;				//玩家职业
// 			int nSex = 0;
// 			if(pTarget!=NULL)					//目标在线
// 			{
// 				sprintf(szDebugStep,"%d",1);
// 				if(pUser->GetFriend(pTarget->GetID())!=NULL)//是自己的好友
// 				{
// 					sprintf(szDebugStep,"%d",2);
// 					if (pUser->DelFriend(pTarget->GetID()))//发消息让客户端彼此删除
// 					{
// 						pTarget->DelBeFriend(pUser->GetID());//独孤求败[2008/12/17]
// 						if(this->Create(_FRIEND_BREAK, pTarget->GetID(), pTarget->GetName()))
// 							pUser->SendMsg(this);
// 					}
// 					sprintf(szDebugStep,"%d",3);
// 					if(pTarget->DelFriend(pUser->GetID()))//发消息让客户端彼此删除
// 					{
// 						pUser->DelBeFriend(pTarget->GetID());//独孤求败[2008/12/17]
// 						if(this->Create(_FRIEND_BREAK, pUser->GetID(), pUser->GetName()))
// 							pTarget->SendMsg(this);
// 					}
// 					sprintf(szDebugStep,"%d",4);
// 				}
// 				m_pInfo->idFriend=pTarget->GetID();
// 				sprintf(szDebugStep,"%d",5);
// 					// 2007/02/01 党魁修
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
// 			else if(m_pInfo->idFriend!=0&&m_pInfo->szName!=NULL)//如果对方不在线，但是自己的好友
// 			{
// 				//ID和NAME通过客户端发过来，直接添加数据
// 				SQLBUF	szSQL;
// 				sprintf(szSQL, "DELETE FROM %s WHERE userid=%u && friendid=%u", _TBL_FRIEND, m_pInfo->idFriend, pUser->GetID());
// 				Database()->ExecuteSQL(szSQL);
// 			}
// 			else if(m_pInfo->szName!=NULL)	//如果客户端只发过来目标的名字，则从数据库中先得到对方的ID
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
// 			if ( m_pInfo->idFriend != 0)  // 2007.8.11 陆从尧修改
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
// 				pUser->SendSysMsg(g_objStrRes.GetStr(15085));//"目标玩家不存在!"
// 			}
// 		}
// 		break;
// 	case _FRIEND_NOTALLOW:
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
// 		{
// 			pUser->SetIsAllowFriend(m_pInfo->idFriend);
// 		}
// 		break;
// 
// 	case _FRIEND_PVP:  //Satan2009.08.07 这里完全用于决斗
// 		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
// 		{			
// 			OBJID idFriend = m_pInfo->idFriend;
// 			CUser* pTarget = UserManager()->GetUser(idFriend);
// 			if(!pTarget)
// 			{
// 				pUser->SendSysMsg("找不到目标!");
// 				return;
// 			}
// 
// 			if(100 == m_pInfo->ucFriendType) //超距离后 数秒后的结果
// 			{				
// 				MapGroup(PID)->GetPVPManger()->EndPVP(idFriend);
// 				return;
// 			}
// 
// 			if(4 == m_pInfo->ucFriendType) //对方拒绝
// 			{
// 				pTarget->FetchApply(CUser::APPLY_PVP);
// 				char temp[128];
// 				sprintf(temp,"玩家 %s 拒绝了你的决斗邀请",pUser->GetName());
// 				pTarget->SendSysMsg(temp);
// 				return;
// 			}
// 			
// 			IStatus* pStatus = pTarget->QueryStatus( STATUS_PVPMODE );
// 			if( pStatus )
// 			{
// 				pUser->SendSysMsg("你正在决斗中!");
// 				return;
// 			}
// 			pStatus = pUser->QueryStatus( STATUS_PVPMODE );
// 			if( pStatus )
// 			{
// 				pUser->SendSysMsg("对方正在决斗中!");
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
// 				pUser->SendSysMsg("决斗邀请已经过期！");
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
// 				if(pUser->GetMapID() != pTarget->GetMapID()               //同地图
// 					|| abs(pUser->GetPosX()- pTarget->GetPosX()) > 20     //X距离
// 					|| abs(pUser->GetPosY()- pTarget->GetPosY()) > 20)    //Y距离       
// 				{
// 					pUser->SendSysMsg("决斗双方距离太远!");
// 					pTarget->SendSysMsg("决斗双方距离太远!");
// 					return;
// 				}
// 
// 				CGameMap* pMap = pUser->GetMap();
// 				if(!pMap || !pMap->IsPVPEnable())
// 				{
// 					pUser->SendSysMsg("当前地图不允许决斗!");
// 					pTarget->SendSysMsg("当前地图不允许决斗!");
// 					return;
// 				}
// 
// // 				if(pUser->GetLife() != pUser->GetMaxLife()
// // 					||pUser->GetMana() != pUser->GetMaxMana()
// // 					||pTarget->GetLife() != pTarget->GetMaxLife()
// // 					||pTarget->GetMana() != pTarget->GetMaxMana())
// // 				{
// // 					pUser->SendSysMsg("决斗前必须双方完全恢复!");
// // 					pTarget->SendSysMsg("决斗前必须双方完全恢复!");
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
// 				//开始。。。。。。。。。。。
// 
// 				pUser->SendSysMsg("决斗开始!");
// 				pTarget->SendSysMsg("决斗开始!");
// 
// 				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,5))
// 					pTarget->SendMsg(this);
// 
// 				IF_OK(Create(_FRIEND_PVP, pTarget->GetID(), pTarget->GetName(), true,0,0,0,0,0,5))
// 					pUser->SendMsg(this);
// 
// 				//刷旗帜
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
// 				//为玩家加装爱河属性
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
