// MsgFriend.cpp: interface for the CMsgFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgFriend.h"
#include "../MapGroup.h"
#pragma	warning(disable:4786)
#include "MsgFriend.h"
#include "MsgPlayer.h"
#include "I_MessagePort.h"
#include "MsgUserAttrib.h"
#include "SKPostOffice.h"

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
				 OBJID idMap,//位置
				 LPCTSTR szServerName//线路
				 )
{
	//20070528修罗:加强日志
	//CHECKF(idFriend && nAction && szName);
	//IF_NOT(idFriend && nAction && szName)
	IF_NOT(nAction && szName)
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
	SafeCopy(m_pInfo->szName, szName, _MAX_NAMESIZE);

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
	if(idMap && szServerName && strlen(szServerName)>0)//位置,地图和线路要有就都有
	{
		char szMap[20];
		sprintf(szMap,"%s_%u", strcmp(szServerName,"六线")==0?"专线":szServerName, idMap);
		bSucMake &= m_StrPacker.AddString(szMap);
	}
	else
		bSucMake &= m_StrPacker.AddString("未知");

	m_unMsgType	=_MSG_FRIEND;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return bSucMake;
}

//////////////////////////////////////////////////////////////////////
void CMsgFriend::Process(void *pInfo)
{
#ifdef _MSGDEBUG
	::LogMsg("Process CMsgFriend, id:%u", m_pInfo->id);
#endif

	char szDebugAction[32];
    char szDebugStep[32];
  // try
   {
	   sprintf(szDebugAction,"ucAction:%d ",m_pInfo->ucAction);


	CUserPtr pUser = UserManager()->GetUser(this);
	DEBUG_TRY
	if (!pUser)
	{
		if(IsNpcMsg())
			return;

		// TransmitMsg
		switch(m_pInfo->ucAction)
		{
		case _FRIEND_GETINFO:
			{
				OBJID idFriend = m_pInfo->idFriend;
				CUser* pTarget = UserManager()->GetUser(idFriend);
				if(pTarget)
				{
					sprintf(szDebugStep,"%d",1);
					//20061020修罗.发送职业----------------
					IF_OK(Create(_FRIEND_ONLINE, 
						idFriend, 
						pTarget->GetName(), 
						ONLINE_STATUS, 
						pTarget->GetLookFace(), 
						pTarget->GetLev(),
						pTarget->GetProfession(),
						0,
						pTarget->GetSex() ,m_pInfo->ucFriendType))
					//IF_OK(Create(_FRIEND_ONLINE, idFriend, pTarget->GetName(), ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev()))
						SendMsg(this);
					//-------------------------------------
					sprintf(szDebugStep,"%d",2);
				}
			}
			break;
		case _ENEMY_ADD:
			return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
			{
				OBJID idFriend = m_pInfo->idFriend;
				CUser* pTarget = UserManager()->GetUser(idFriend);
				if(pTarget)
				{
					sprintf(szDebugStep,"%d",1);
					IF_OK(Create(_ENEMY_ONLINE, idFriend, m_pInfo->szName, ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev(), pTarget->GetProfession(), 0, pTarget->GetSex()))
						SendMsg(this);
					sprintf(szDebugStep,"%d",2);
				}
			}
			break;
		case _FRIEND_BREAK:
			return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
			{
				OBJID idFriend = m_pInfo->idFriend;
				CUser* pTarget = UserManager()->GetUser(idFriend);
				if(pTarget)
				{
					sprintf(szDebugStep,"%d",1);
					pTarget->DelFriend(m_pInfo->idFriend);
					sprintf(szDebugStep,"%d",2);
					if (pUser)
						pUser->DelBeFriend(pTarget->GetID());//独孤求败[2008/12/17]
					sprintf(szDebugStep,"%d",3);
				}
			}
			break;
		}

		return;
	}
	DEBUG_CATCH2("MsgFriendChannel:%d ERROR! (When !pUser)", m_pInfo->ucAction);

// 	if (!pUser->IsAlive())
// 	{
// 		pUser->SendSysMsg(g_objStrRes.GetStr(12069));//STR_DIE
// 		return;
// 	}
	DEBUG_TRY
	switch(m_pInfo->ucAction)
	{
	case _FRIEND_ADDSUCCESS_PASSIVE:
		{
			///被动加成功只需要加被动好友即可
			pUser->AddBeFriend(0,GetTransData(),m_pInfo->ucFriendType,m_pInfo->szName);//如果A已经是B的好友，不需要再通知B
		}
		break;
	case _FRIEND_ADDSUCCESS:///OK,在这里被调用时只有可能是收到别的服务器发过的好友添加成功的通知
		{
			///别人同意加我为好友了，我在本地可以先将它加为离线好友

			CMsgFriend pFriendMsg;
			OBJID dwFriendID = pUser->AddLeaveFriend(m_pInfo->szName, m_pInfo->ucFriendType, &pFriendMsg);
			if(dwFriendID)
			{
				m_pInfo->idFriend=dwFriendID;
				pUser->SendMsg(this);

				///离线加好友时向关系服务器求助查询好友在线状态
// 				pFriendMsg.SetTransData(pUser->GetID());
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pFriendMsg);
				
				///屏蔽了线程组发消息，都拜托给世界线程了
				//					pUser->SendMsg(&pFriendMsg);
				char szTemp[128];
				sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",m_pInfo->szName);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
			}
			else
			{
				LOGERROR("_FRIEND_WHETHER_ADDFRIEND 加离线好友失败");
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);
			}

// 			CMsgFriend pMsg;
// 			pUser = UserManager()->GetUser(m_pInfo->idFriend);
// 			
// 			if (pUser && pUser->AddBeFriend(0,GetTransData(),m_pInfo->ucFriendType) && m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP)//如果A已经是B的好友，不需要再通知B
// 			{
// 				if (pUser->GetFriend(GetTransData()))
// 				{
// 					char szTemp[128];
// //					sprintf(szTemp,"<font color=\"#ffff00\">用户</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">将你加为了好友!</font>",pUser->GetName());
// 					sprintf(szTemp,"<font color=\"#ffff00\">用户</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">将你加为了好友!</font>",m_pInfo->szName);
// 					pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
// 				}
// 				else
// 				{
// 				//	pMsg.Create(_FRIEND_ADDFRIEND,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,NULL,0);
// 					pMsg.Create(_FRIEND_ADDFRIEND,GetTransData(),m_pInfo->szName,0,0,0,0,0,0,0,NULL,0);
// 					pUser->SendMsg(&pMsg);
// 				}
// 			}
			
		}
		break;
	case _FRIEND_CONTACTHISTORY_APPLY:
		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误		
	case _FRIEND_ADDFRIEND://由A发送的请求  [2010-06-07 goto]不需要B的同意，直接加为好友，同时通知B
		{			
			if (pUser->GetID()==m_pInfo->idFriend || stricmp(pUser->GetName(),m_pInfo->szName)==0)
				return;
			if (stricmp("系统",m_pInfo->szName)==0)
				return;
			if( m_pInfo->ucFriendType>MAX_GROUP || m_pInfo->ucFriendType<=0 )//好友分组,1-4组，5临时好友，6黑名单
				return;

			if(m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP && pUser->GetFriendGroupAmount(m_pInfo->ucFriendType)>=MAX_GROUP_FRIEND )
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);

				pUser->SendSysMsg("此好友分组已达到人数上限");
				return;
			}

			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			if (pFriend)//B已经是A的好友
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);
				char szTemp[128];
				sprintf(szTemp,"%s已经是你的好友",m_pInfo->szName);
				pUser->SendSysMsg(szTemp);
				return;
			}
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (pTarget)
			{
				CMsgFriend pMsg;
				if(m_pInfo->ucFriendType==TEMP_FRIEND_GROUP)
				{
					pMsg.Create(_FRIEND_ADDSUCCESS,
						pTarget->GetID(),
						m_pInfo->szName,
						ONLINE_STATUS,
						pTarget->GetLookFace(),
						pTarget->GetLev(),
						pTarget->GetProfession(),
						0,//friendship
						pTarget->GetSex(),
						m_pInfo->ucFriendType,//frinedgroup
						pTarget->GetFeel(),
								 0);
					pUser->SendMsg(&pMsg);
					pUser->AddFriend(pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType,pTarget->GetMate());
					pTarget->AddBeFriend(0,pUser->GetID(),m_pInfo->ucFriendType,pUser->GetName());
					pUser->SetTempFriend(true);
                }
				else
				{
					if (!pTarget->IsAgreeFriendApply())
					{
						pUser->SendSysMsg(_TXTATR_TOPSHOW,"对方已打开拒绝加好友开关");//STR_TEAM_FULL
					   return;
					}
					pMsg.Create(_FRIEND_ADDFRIEND,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,m_pInfo->ucFriendType,NULL,0);
                    pTarget->SendMsg(&pMsg);
				}
				
			} 
			else
			{ 
				CMsgFriend msg;
				if(m_pInfo->ucFriendType==TEMP_FRIEND_GROUP)
				{
// 					pMsg.Create(_FRIEND_ADDSUCCESS,
// 						pTarget->GetID(),
// 						m_pInfo->szName,
// 						ONLINE_STATUS,
// 						pTarget->GetLookFace(),
// 						pTarget->GetLev(),
// 						pTarget->GetProfession(),
// 						0,//friendship
// 						pTarget->GetSex(),
// 						m_pInfo->ucFriendType,//frinedgroup
// 						pTarget->GetFeel(),
// 						0);
// 					pUser->SendMsg(&pMsg);
// 					pUser->AddFriend(pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType,pTarget->GetMate());
//					OBJID dwFriendID = pUser->AddLeaveFriend(m_pInfo->szName, m_pInfo->ucFriendType, &pFriendMsg);
                }
				else
				{
					msg.Create(_FRIEND_ADDFRIEND,pUser->GetID(),m_pInfo->szName,0,0,0,0,0,0,m_pInfo->ucFriendType,NULL,0);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
				//pUser->SendSysMsg("对方不在线，不能加为好友");
			}
		}
		break;

    case _FRIEND_WHETHER_ADDFRIEND:
		{
			if(!pUser)
				return;
			if (stricmp(pUser->GetName(),m_pInfo->szName)==0)
				return;
			if(m_pInfo->ucSex<1||m_pInfo->ucSex>MAX_GROUP)
				return;
			
			DWORD optnumber=m_pInfo->dwLookFace;
			if(optnumber<0||optnumber>3)
				return;
            CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (!pTarget)
			{
				switch(optnumber)
				{
					case 1:
					case 2:
					{
						CMsgFriend pMsg;
						pMsg.Create(_FRIEND_ADDSUCCESS,
							pUser->GetID(),
							m_pInfo->szName,
							ONLINE_STATUS,
							pUser->GetLookFace(),
							pUser->GetLev(),
							pUser->GetProfession(),
							0,//friendship
							pUser->GetSex(),
							m_pInfo->ucSex,//frinedgroup
							pUser->GetFeel(),
						0);//relation
						///离线加好友时向关系服务器求助查询好友在线状态
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pMsg,OBJID(0));
						
						///这里查询一下数据库将用户名转成用户id
						if(pUser->AddBeFriend(0,m_pInfo->idFriend,m_pInfo->ucSex,m_pInfo->szName))//如果A已经是B的好友，不需要再通知B
						{
							char szTemp[128]="";
							sprintf(szTemp,"<font color=\"#ffff00\">用户</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">将你加为了好友!</font>",m_pInfo->szName);
							pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						}

						///同意加好友同时加对方为好友
						if(optnumber==2)
						{
							if(m_pInfo->ucFriendType<1||m_pInfo->ucFriendType>MAX_GROUP)
								return;
							///防止重复添加好友
							if(pUser->GetFriend(m_pInfo->idFriend))
							{
								char szTemp[128]="";
								sprintf(szTemp,"<font color=\"#ffff00\">用户</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">已经是你的好友,不用重复添加!</font>",m_pInfo->szName);
								pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
								break;
							}
							CMsgFriend pFriendMsg;
							OBJID dwFriendID = pUser->AddLeaveFriend(m_pInfo->szName, m_pInfo->ucFriendType, &pFriendMsg);
							if(dwFriendID)
							{
								pFriendMsg.m_pInfo->idFriend=dwFriendID;

								
// 								pUser->SendMsg(&pFriendMsg);
								pFriendMsg.m_pInfo->ucAction=_FRIEND_ADDSUCCESS_PASSIVE;
								///离线加好友时向关系服务器求助查询好友在线状态
								pFriendMsg.SetTransData(pUser->GetID());
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pFriendMsg);
								
								///屏蔽了线程组发消息，都拜托给世界线程了
								//					pUser->SendMsg(&pFriendMsg);
								char szTemp[128];
								sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",m_pInfo->szName);
								pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
							}
							else
								LOGERROR("_FRIEND_WHETHER_ADDFRIEND 加离线好友失败");

							
						}
							///屏蔽了线程组发消息，都拜托给世界线程了
							//					pUser->SendMsg(&pFriendMsg);
// 							char szTemp[128];
// 							sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",m_pInfo->szName);
// 							pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
					}
						break;
// 					case 2:
// 						{
// 
// 						}
// 						break;
					case 3:///拒绝流程
						{
							CMsgFriend pMsg;
							pMsg.Create(_FRIEND_ADDFAIL,pUser->GetID(),m_pInfo->szName,0,0,0,0,0,0,0,NULL,0);
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pMsg,OBJID(0));
//							pTarget->SendMsg(&pMsg);
// 							char szTemp[128];
// 							sprintf(szTemp,"<font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">拒绝了您的好友请求!</font>",pUser->GetName());
// 							pTarget->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						}
						break;
					default:
						break;
				}
				return;
			}
			if (optnumber==1)
			{
				if(pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucSex,pUser->GetMate()))
				{ CMsgFriend pMsg;
				  pMsg.Create(_FRIEND_ADDSUCCESS,
						pUser->GetID(),
						pUser->GetName(),
						ONLINE_STATUS,
						pUser->GetLookFace(),
						pUser->GetLev(),
						pUser->GetProfession(),
						0,//friendship
						pUser->GetSex(),
						m_pInfo->ucSex,//frinedgroup
						pUser->GetFeel(),
						0);//relation
				  pTarget->SendMsg(&pMsg);
				  char szTemp[128];
				  sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pUser->GetName());
				  pTarget->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
				  pUser->AddBeFriend(0,m_pInfo->idFriend,m_pInfo->ucSex,pTarget->GetName());
                }
				else
				{
					CMsgFriend pMsg;
					pMsg.Create(_FRIEND_ADDFAIL,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,NULL,0);
					pTarget->SendMsg(&pMsg);
					return;
				}
			}
			else if (optnumber==2)
			{  
				///防止重复添加好友
				if(m_pInfo->ucFriendType<1||m_pInfo->ucFriendType>MAX_GROUP)
					return;
				if(pUser->GetFriend(m_pInfo->idFriend))
				{
					char szTemp[128]="";
					sprintf(szTemp,"<font color=\"#ffff00\">用户</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">已经是你的好友,不用重复添加!</font>",m_pInfo->szName);
					pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
					break;
				}
				if(pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucSex,pUser->GetMate()))
				{ CMsgFriend pMsg;
				  pMsg.Create(_FRIEND_ADDSUCCESS,
					pUser->GetID(),
					pUser->GetName(),
					ONLINE_STATUS,
					pUser->GetLookFace(),
					pUser->GetLev(),
					pUser->GetProfession(),
					0,//friendship
					pUser->GetSex(),
					m_pInfo->ucSex,//frinedgroup
					pUser->GetFeel(),
					0);//relation
				pTarget->SendMsg(&pMsg);
				char szTemp[128];
				sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
				pUser->AddBeFriend(0,m_pInfo->idFriend,m_pInfo->ucSex,pTarget->GetName());
                }
				else
				{
					CMsgFriend pMsg;
					pMsg.Create(_FRIEND_ADDFAIL,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,NULL,0);
					pTarget->SendMsg(&pMsg);
					return;
				}
			   if(pUser->AddFriend(pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType,pTarget->GetMate()))
				{ CMsgFriend pMsg;
				pMsg.Create(_FRIEND_ADDSUCCESS,
					pTarget->GetID(),
					m_pInfo->szName,
					ONLINE_STATUS,
					pTarget->GetLookFace(),
					pTarget->GetLev(),
					pTarget->GetProfession(),
					0,//friendship
					pTarget->GetSex(),
					m_pInfo->ucFriendType,//frinedgroup
					pTarget->GetFeel(),
					0);//relation
				pUser->SendMsg(&pMsg);
				char szTemp[128];
				sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",m_pInfo->szName);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
				pTarget->AddBeFriend(0,pUser->GetID(),m_pInfo->ucFriendType,pUser->GetName());
                }
				else
				{
					CMsgFriend pMsg;
					pMsg.Create(_FRIEND_ADDFAIL,pTarget->GetID(),m_pInfo->szName,0,0,0,0,0,0,0,NULL,0);
					pUser->SendMsg(&pMsg);
					return;
				}
			}
			else if (optnumber==3)
			{
				CMsgFriend pMsg;
			    pMsg.Create(_FRIEND_ADDFAIL,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,NULL,0);
				pTarget->SendMsg(&pMsg);
                char szTemp[128];
				sprintf(szTemp,"<font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">拒绝了您的好友请求!</font>",pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
			}
		}
		break;

	case _FRIEND_CHANGEGROUP://[2010-06-07 goto]更换好友分组
		{
			if( m_pInfo->ucFriendType>(MAX_GROUP+1) || m_pInfo->ucFriendType<=0 )//好友分组,1-4组，5临时好友，6黑名单
				return;
			
			if( pUser->GetFriendGroupAmount(m_pInfo->ucFriendType)>=MAX_GROUP_FRIEND )
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_CHANGEFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);
				
				pUser->SendSysMsg("此好友分组已达到人数上限");
				return;
			}

			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			if(!pFriend)
				return;
			
			unsigned int uoldType = pFriend->GetFriendType();
			//pFriend->SetFriendType(m_pInfo->ucFriendType);//修改好友分组
			if( pUser->MoveFriend(pFriend,m_pInfo->ucFriendType) )//修改好友分组
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_CHANGEGROUP,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,uoldType,0,m_pInfo->ucFriendType,NULL,0))
					pUser->SendMsg(&pMsg);
			}
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if(pTarget)
			{
				CFriend* pTargetFriend = pTarget->GetBeFriend(pUser->GetID());
				if (pTargetFriend)
					pTargetFriend->SetFriendType(m_pInfo->ucFriendType);
			}
		}
		break;
	case _FRIEND_DELFRIEND://[2010-06-08 goto]删除好友
		{
			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			if(!pFriend)///如果在本地找不到这个好友,就尝试删除被动好友
			{
				pUser->DelBeFriend(GetTransData());
			}
			if (!pFriend)
				return;

			OBJID nFriendID=pFriend->GetFriendID();
			int nFriendType=pFriend->GetFriendType();
			if(pUser->DelFriend(pFriend->GetFriendID()))
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_DELFRIEND,nFriendID,m_pInfo->szName,0,0,0,0,0,0,nFriendType,NULL,0))
					pUser->SendMsg(&pMsg);
			}
			else
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_DELFRIENDFAIL,pFriend->GetFriendID(),pFriend->GetFriendName(),0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);
				return;
			}

			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (pTarget)
			{
				pTarget->DelBeFriend(pUser->GetID());
			}
			else
			{
				///找不到好友时尝试转到关系服务器处理
				SetTransData(pUser->GetID());
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);

//				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, nFriendID);
			}
		}
		break;
	case _FRIEND_SETFEEL://[2010-06-08 goto]修改心情
		{
			if(m_pInfo->szName)
			{
				if(strlen(m_pInfo->szName)>_MAX_FEELSIZE)
					return;

				pUser->SetFeel(m_pInfo->szName);

				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_SETFEEL,pUser->GetID(),pUser->GetName(),ONLINE_STATUS,0,0,0,0,0,0,m_pInfo->szName,0))
					pUser->SendMsg(&pMsg);

				CMsgPlayer msgPlayer;
				if (msgPlayer.Create(pUser->QueryRole()))
					pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
			}
		}
		break;
	case _FRIEND_QUERYFRIENDINFO:///收到其它线来的查询信息
		{
			///目前有效时才处理
			if(pUser && pUser->CanBeSee()==false)
				return;

			char szUserTitle[10];
			sprintf(szUserTitle, "%u", pUser->GetUseTitle());
			int nFriendShip=0,nMapID=0;
			char szTeamName[32] = "";
			sprintf(szTeamName, "%s_%d", "无", m_pInfo->ucSex%10);
			CTeam* pTeam = pUser->GetTeam();
			if(pTeam)
				sprintf(szTeamName, "%s_%d", pTeam->GetTeamName(), m_pInfo->ucSex%10);//在这里告诉客户端是否要弹出详细资料窗口，如果是聊天时请求详细信息不需要弹出
			
			CSyndicate* pSyndicate = pUser->GetSyndicate();
			CFriend* pFriend= pUser->GetFriend(m_pInfo->dwLookFace);
			if(pFriend)
				nFriendShip=pFriend->GetFriendShip();
			if(pUser->GetMap())
				nMapID=pUser->GetMap()->GetDocID();
			CMsgFriend pMsg;
			if (pMsg.Create(_FRIEND_REPLYFRIENDINFO,
				pUser->GetID(),
				pUser->GetName(),
				pUser->GetProfession(2),//副职业
				pUser->GetLookFace(),//lookface
				pUser->GetLev(),//人物等级
				pUser->GetProfession(1),//主职业
				nFriendShip,
				pUser->GetJobLev(1)*10,//发出去的时候会除10,所以要乘一下
				pUser->GetJobLev(2),//副职业等级
				pUser->GetFeel(),
				0,//关系
				pSyndicate==NULL? "无" : pSyndicate->GetStr(SYNDATA_NAME),//帮派
				szUserTitle,//称号
				pUser->GetMate(),//配偶
				szTeamName,//队伍
				nMapID,//位置
				::MapGroup(PID)->GetServerName()//线路
				))
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&pMsg,m_pInfo->dwLookFace);
		}
		break;
	case _FRIEND_GETFRIENDINFO://获取好友的详细信息
		{
			if (stricmp("系统",m_pInfo->szName)==0)
				return;
			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);

			///目前有效时才处理
			if(pTarget && pTarget->CanBeSee()==false)
				return;

			if (!pFriend)//非好友
			{
				if(!pTarget)
				{
// 					CMsgFriend pMsg;
// 					pMsg.Create(_FRIEND_GETFRIENDINFO,m_pInfo->idFriend,m_pInfo->szName,
// 						0,//副职业
// 						0,//lookface
// 						0,//人物等级
// 						0,//主职业
// 						0,
// 						0,//主职业等级
// 						0,//副职业等级
// 						NULL,
// 						0);//关系
// 					pUser->SendMsg(&pMsg);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(this,pUser->GetID());

//					pUser->SendSysMsg(_TXTATR_TOPSHOW, "此人不在线，或在副本中");
				}
				else
				{
					char szUserTitle[10];
					sprintf(szUserTitle, "%u", pTarget->GetUseTitle());
					
					char szTeamName[32] = "";
					sprintf(szTeamName, "%s_%d", "无", m_pInfo->ucSex%10);
					CTeam* pTeam = pTarget->GetTeam();
					if(pTeam)
						sprintf(szTeamName, "%s_%d", pTeam->GetTeamName(), m_pInfo->ucSex%10);//在这里告诉客户端是否要弹出详细资料窗口，如果是聊天时请求详细信息不需要弹出
// 					CTeam* pTeam = pTarget->GetTeam();
// 					if(pTeam)
// 						sprintf(szTeamName,pTeam->GetTeamName());

					CSyndicate* pSyndicate = pTarget->GetSyndicate();
					UINT nMapID=0;
					if(pTarget->GetMap())
						nMapID=pTarget->GetMap()->GetDocID();
					CMsgFriend pMsg;
					if (pMsg.Create(_FRIEND_GETFRIENDINFO,
						pTarget->GetID(),
						pTarget->GetName(),
						pTarget->GetInfo()->SecJob,//副职业
						pTarget->GetLookFace(),//lookface
						pTarget->GetLev(),//人物等级
						pTarget->GetInfo()->FirJob,//主职业
						0,//友好度
						pTarget->GetInfo()->FirJobLev,//主职业等级
						pTarget->GetInfo()->SecJobLev,//副职业等级
						pTarget->GetFeel(),
						0,//关系
						pSyndicate==NULL? "无" : pSyndicate->GetStr(SYNDATA_NAME),//帮派
						szUserTitle,//称号
						pTarget->GetMate(),//配偶
						szTeamName,//队伍
						nMapID,//位置
						::MapGroup(PID)->GetServerName()//线路
						))
					pUser->SendMsg(&pMsg);
				}
				return;
			}
			if (!pTarget)//不在线时部分信息无法获取
			{
				char szUserTitle[10];
				sprintf(szUserTitle, "%u",pFriend->GetFriendTitle());
				OBJID idSyn = pFriend->GetFriendSynID();
				char szSyn[16] = "无";
				if (idSyn!=ID_NONE)
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
					if (pSyn)
						sprintf(szSyn, pSyn->GetStr(SYNDATA_NAME));
				}
				char szTeam[32] = "";
				sprintf(szTeam, "%s_%d", "无", m_pInfo->ucSex);//goto不在本线是肯定没队的，但是为了带另一个信息过去只好加上了
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_GETFRIENDINFO,
							pFriend->GetFriendID(),
							pFriend->GetFriendName(),
							pFriend->GetFriendSecJob(),//副职业
							pFriend->GetFriendLookFace(),//lookface
							pFriend->GetFriendLevel(),//人物等级
							pFriend->GetFriendFirJob(),//主职业
							pFriend->GetFriendShip(),
							pFriend->GetFriendFirJobLev()*10+m_pInfo->ucSex,//主职业等级,个位是告诉客户端是否要弹出对话框
							pFriend->GetFriendSecJobLev(),//副职业等级
							pFriend->GetFriendFeel(),
							0,//关系
							szSyn,//"帮派",
							szUserTitle,
							pFriend->GetFriendMateName(),
							szTeam))

				///好友不在线时也需要转发到关系服务器去处理
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&pMsg,pUser->GetID());
//				pUser->SendMsg(&pMsg);
			}
			else
			{
				char szUserTitle[10];
				sprintf(szUserTitle, "%u", pTarget->GetUseTitle());
				
				char szTeamName[32] = "";
				sprintf(szTeamName, "%s_%d", "无", m_pInfo->ucSex%10);
				CTeam* pTeam = pTarget->GetTeam();
				if(pTeam)
					sprintf(szTeamName, "%s_%d", pTeam->GetTeamName(), m_pInfo->ucSex%10);//在这里告诉客户端是否要弹出详细资料窗口，如果是聊天时请求详细信息不需要弹出
// 				CTeam* pTeam = pTarget->GetTeam();
// 				if(pTeam)
// 					sprintf(szTeamName, pTeam->GetTeamName());

				CSyndicate* pSyndicate = pTarget->GetSyndicate();
				UINT nMapID=0;
				if(pTarget->GetMap())
					nMapID=pTarget->GetMap()->GetDocID();
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_GETFRIENDINFO,
							pTarget->GetID(),
							pTarget->GetName(),
							pTarget->GetProfession(2),//副职业
							pTarget->GetLookFace(),//lookface
							pTarget->GetLev(),//人物等级
							pTarget->GetProfession(1),//主职业
							pFriend->GetFriendShip(),
							pTarget->GetJobLev(1),//主职业等级
							pTarget->GetJobLev(2),//副职业等级
							pTarget->GetFeel(),
							0,//关系
							pSyndicate==NULL? "无" : pSyndicate->GetStr(SYNDATA_NAME),//帮派
							szUserTitle,//称号
							pTarget->GetMate(),//配偶
							szTeamName,//队伍
							nMapID,//位置
							::MapGroup(PID)->GetServerName()//线路
							))
				pUser->SendMsg(&pMsg);
			}
		}
		break;
	case _FRIEND_DELCONTACT://删除已经看过的留言
		{
			if(pUser->GetID()!=m_pInfo->idFriend)
				return;
			pUser->DelFriendLeaveWord(m_pInfo->szName,m_pInfo->dwLookFace);
		}
		break;
	case _FRIEND_HAVETEAM://查看好友队伍信息
		{
			CUserPtr pFriend = UserManager()->GetUser(m_pInfo->idFriend);
			if (!pFriend)
			{
				CMsgFriend msg;
				if (msg.Create(_FRIEND_HAVETEAM,0,pUser->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
				{
					pUser->SendMsg(&msg);
				}
				return;
			}

			int nStep = 1;
			DEBUG_TRY
			CTeam* pTeam = pFriend->GetTeam();
			nStep = 2;
			if (!pTeam)//好友无队伍
			{
				nStep = 3;
				CMsgFriend msg;
				if (msg.Create(_FRIEND_HAVETEAM,0,pFriend->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
					pUser->SendMsg(&msg);
			}
			else//好友有队伍
			{
				nStep = 4;
				OBJID idTeam = pTeam->GetID();
				OBJID idTeamLeader = pTeam->GetLeader();
				LPCTSTR szTeam = pTeam->GetTeamName();

				nStep = 5;
				CMsgFriend msg;
				if (msg.Create(_FRIEND_HAVETEAM,idTeam,szTeam,0,idTeamLeader,0,0,0,0,0))
					pUser->SendMsg(&msg);
			}
			DEBUG_CATCH2("_FRIEND_HAVETEAM %u",nStep);
		}
		break;
	case _FRIEND_HAVETEAM_MT://世界线程查找玩家的返回结果
		{
// 			CUserPtr pFriend = UserManager()->GetUser(m_pInfo->idFriend);
// 			if (!pFriend)
// 			{
// 				CMsgFriend msg;
// 				if (msg.Create(_FRIEND_HAVETEAM,0,pUser->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// 					pUser->SendMsg(&msg);
// 				return;
// 			}
// 			
// 			CTeam* pTeam = pFriend->GetTeam();
// 			if (!pTeam)//好友无队伍
// 			{
// 				CMsgFriend msg;
// 				if (msg.Create(_FRIEND_HAVETEAM,0,pFriend->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// 					pUser->SendMsg(&msg);
// 			}
// 			else//好友有队伍
// 			{
// 				OBJID idTeam = pTeam->GetID();
// 				OBJID idTeamLeader = pTeam->GetLeader();
// 				LPCTSTR szTeam = pTeam->GetTeamName();
// 				CMsgFriend msg;
// 				msg.Create(_FRIEND_HAVETEAM,idTeam,szTeam,0,idTeamLeader,0,0,0,0,0);
// 				pUser->SendMsg(&msg);
// 			}
		}
		break;
	case _FRIEND_FEEL_SWITH:
		{
			pUser->SetFeelSwitch(m_pInfo->idFriend);
			CMsgUserAttrib msg;
			msg.Create(pUser->GetID(), _USERATTRIB_FEEL_SWITH, m_pInfo->idFriend);
			pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}
		break;
	case _FRIEND_MOVETOGOODFRIENDACCEPT://B同意A的请求
		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
		{
			if (m_pInfo->ucFriendType != 1)
				return;
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (!pTarget)
				return;
			sprintf(szDebugStep,"%d",1);
			CFriend* pBeFriend = pTarget->GetFriend(pUser->GetID());//我必然已经他的某种好友
			if (!pBeFriend)
				return;
			CFriend* pFriend= pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID());//他是否也是我的某种好友
			sprintf(szDebugStep,"%d",2);
			if (!pFriend)//不是
			{
				sprintf(szDebugStep,"%d",3);
				if (pUser->AddFriend(/*m_pInfo->idFriend*/pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType)
					&& pTarget->MoveFriend(pBeFriend,1))
				{
					sprintf(szDebugStep,"%d",4);
					//-----A加了B为好友,则在B中保存A的信息----------
					pFriend = pUser->GetFriend(pTarget->GetID()/*m_pInfo->idFriend这里的m_pInfo->idFriend可能为零*/);
					pTarget->AddBeFriend(pFriend->GetID(),pUser->GetID(),pFriend->GetFriendType(),pUser->GetName());
					sprintf(szDebugStep,"%d",5);
					//----------------------------
					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
						pTarget->GetID(), 
						pTarget->GetName(), 
						ONLINE_STATUS, 
						pTarget->GetLookFace(), 
						pTarget->GetLev(),
						pTarget->GetProfession(),
						0,
						pTarget->GetSex(),
						m_pInfo->ucFriendType) )
						pUser->SendMsg(this);
					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
						pUser->GetID(), 
						pUser->GetName(), 
						ONLINE_STATUS, 
						pUser->GetLookFace(), 
						pUser->GetLev(),
						pUser->GetProfession(),
						0,
						pUser->GetSex(),
						m_pInfo->ucFriendType) )
						pTarget->SendMsg(this);
					MSGBUF	szMsg;
						sprintf(szMsg,g_objStrRes.GetStr(13057), pUser->GetName(), pTarget->GetName());//STR_MAKE_FRIEND
						pUser->BroadcastRoomMsg(szMsg, true);
						sprintf(szDebugStep,"%d",6);
				}
				sprintf(szDebugStep,"%d",7);
			}
			else//是
			{
				sprintf(szDebugStep,"%d",8);
				if (pUser->MoveFriend(pFriend,1) && pTarget->MoveFriend(pBeFriend,1))
				{
					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
						pTarget->GetID(), 
						pTarget->GetName(), 
						ONLINE_STATUS, 
						pTarget->GetLookFace(), 
						pTarget->GetLev(),
						pTarget->GetProfession(),
						0,
						pTarget->GetSex(),
						m_pInfo->ucFriendType) )
						pUser->SendMsg(this);
					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
						pUser->GetID(), 
						pUser->GetName(), 
						ONLINE_STATUS, 
						pUser->GetLookFace(), 
						pUser->GetLev(),
						pUser->GetProfession(),
						0,
						pUser->GetSex(),
						m_pInfo->ucFriendType) )
						pTarget->SendMsg(this);
					MSGBUF	szMsg;
						sprintf(szMsg,g_objStrRes.GetStr(13057), pUser->GetName(), pTarget->GetName());//STR_MAKE_FRIEND
						pUser->BroadcastRoomMsg(szMsg, true);
				}
				sprintf(szDebugStep,"%d",9);
			}
		}
		break;
	case _FRIEND_MOVEAPPLY:
		return;//goto 屏蔽未使用的功能，以免消息错误导致服务器错误
		{
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (!pTarget)
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(28023));
				return;
			}
			sprintf(szDebugStep,"%d",1);
			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			if (pFriend == NULL || pFriend->GetFriendType() == m_pInfo->ucFriendType)
				return;
			if (m_pInfo->ucFriendType == 1)//如果想移为GOOD FRIEND,必须要经过对方确认,相当于发送了一个申请加好友消息
			{
			}
			sprintf(szDebugStep,"%d",2);
			if (pUser->MoveFriend(pFriend,m_pInfo->ucFriendType))
			{
				sprintf(szDebugStep,"%d",3);
				if(this->Create(_FRIEND_MOVESUCESS, 
					pFriend->GetFriendID(), 
					pFriend->GetFriendName(), 
					pTarget?true:false, 
					pTarget?pTarget->GetLookFace():0, 
					pTarget?pTarget->GetLev():0,
					pTarget?pTarget->GetProfession():0,
					0,
					pTarget?pTarget->GetSex():0,
					m_pInfo->ucFriendType) )
						pUser->SendMsg(this);
				sprintf(szDebugStep,"%d",4);
			}
		}
		break;
	case _FRIEND_PVP:  //Satan2009.08.07 这里完全用于切磋
		{			
			OBJID idFriend = m_pInfo->idFriend;
			CUser* pTarget = UserManager()->GetUser(idFriend);
			if (!pUser->GetMap())
			{
				return;
			}
			if (pUser->QuerySquareDeal())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
				pUser->SetPVPTargetID(0);
				return;
			}
			if (pUser->QueryBooth())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
				pUser->SetPVPTargetID(0);
				return;
			}
			if (pUser->GetMap()->isDisableStruggle())
			{
				pUser->SendSysMsg("该地图不能发起切磋请求！");
				return;
			}
			if (pUser->GetMap()->IsPkGameMap())
			{
				pUser->SendSysMsg("该地图不能发起切磋请求");
				return;
			}
			if (pUser && pUser->IsRedName())
			{
				pUser->SendSysMsg("你不能发起切磋！");
				return;
			}
			if (pTarget && pTarget->IsRedName())
			{
				pUser->SendSysMsg("对方不能成为切磋的对象！");
				return;
			}

			if (pUser->GetMapID() > DYNAMIC_MAP_ID)
			{
				pUser->SendSysMsg("副本之内禁止切磋！");
				return;				
			}
			if(!pTarget)
			{
//				pUser->SendSysMsg("找不到目标!");
				return;
			}
			if (pTarget->QuerySquareDeal())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方正在交易中");
				pUser->SetPVPTargetID(0);
				pTarget->SetPVPTargetID(0);
				return;
			}
			if (pTarget->QueryBooth())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方正在摆摊中");
				pUser->SetPVPTargetID(0);
				pTarget->SetPVPTargetID(0);
				return;
			}
			if(100 == m_pInfo->ucFriendType) //超距离后 数秒后的结果
			{				
				MapGroup(PID)->GetPVPManger()->EndPVP(idFriend);
				return;
			}
			if (99 == m_pInfo->ucFriendType)
			{
				pTarget->SendSysMsg("不能发起切磋");
				return;
			}

			if(4 == m_pInfo->ucFriendType) //对方拒绝
			{
				pTarget->FetchApply(CUser::APPLY_PVP);
				char temp[128];
				sprintf(temp,"玩家 %s 拒绝了你的切磋邀请",pUser->GetName());
				pTarget->SendSysMsg(temp);
				pTarget->SetPVPTargetID(0);
				return;
			}
			
			IStatus* pStatus_Self = pUser->QueryStatus( STATUS_PVPMODE );
			IStatus* pStatus = pTarget->QueryStatus( STATUS_PVPMODE );
			if( pStatus_Self )
			{
				pUser->SendSysMsg("你正在切磋中!");
				return;
			}
			if( pStatus )
			{
				pUser->SendSysMsg("对方正在切磋中!");
				return;
			}
			if(m_pInfo->ucFriendType == 1)
			{
				if (pUser->GetPVPTargetID() == 2 || pTarget->GetPVPTargetID() == 1 || 
					pUser->GetPVPTargetID() == 1 || pTarget->GetPVPTargetID() == 2)
				{
					pUser->SendSysMsg("你已经发起了切磋请求或者对方正在切磋中。");
					return;
				}
			}

			if(1 == m_pInfo->ucFriendType)
			{
				if (pTarget->QueryBooth())
				{
					pUser->SendSysMsg("对方正在摆摊中！");
					return;
				}
				if (!pTarget->IsAgreePKApply())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"对方已打开拒绝切磋开关");//STR_TEAM_FULL
					return;
				}
				pUser->SetPVPTargetID(m_pInfo->ucFriendType);
				pUser->SetApply(CUser::APPLY_PVP, pTarget->GetID());
				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,m_pInfo->ucFriendType))
					pTarget->SendMsg(this);
				pUser->SendSysMsg("你发起了切磋请求");
				return;
			}
			
			if(2 == m_pInfo->ucFriendType)
			{
				pUser->SetPVPTargetID(m_pInfo->ucFriendType);
				pUser->SetApply(CUser::APPLY_PVP, pTarget->GetID());
				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,m_pInfo->ucFriendType))
					pTarget->SendMsg(this);
				return;
			}

			if(pUser->FetchApply(CUser::APPLY_PVP) != pTarget->GetID())
			{		
				pUser->SendSysMsg("切磋邀请已经过期！");
				return;
// 				pUser->SetPVPTargetID(m_pInfo->ucFriendType);
// 				pUser->SetApply(CUser::APPLY_PVP, pTarget->GetID());
// 				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,m_pInfo->ucFriendType))
// 					pTarget->SendMsg(this);
			}
			else if(3 == m_pInfo->ucFriendType)
			{
				pUser->FetchApply(CUser::APPLY_PVP);

				if(pUser->GetMapID() != pTarget->GetMapID()               //同地图
					|| abs(pUser->GetPosX()- pTarget->GetPosX()) > 10     //X距离
					|| abs(pUser->GetPosY()- pTarget->GetPosY()) > 10)    //Y距离       
				{
					pUser->SetPVPTargetID(0);
					pTarget->SetPVPTargetID(0);
					pUser->SendSysMsg("切磋双方距离太远!");
					pTarget->SendSysMsg("切磋双方距离太远!");
					return;
				}

				CGameMap* pMap = pUser->GetMap();
//				if(!pMap || !pMap->IsPVPEnable())
//				{
//					pUser->SendSysMsg("当前地图不允许切磋!");
//					pTarget->SendSysMsg("当前地图不允许切磋!");
//					return;
//				}

// 				if(pUser->GetLife() != pUser->GetMaxLife()
// 					||pUser->GetMana() != pUser->GetMaxMana()
// 					||pTarget->GetLife() != pTarget->GetMaxLife()
// 					||pTarget->GetMana() != pTarget->GetMaxMana())
// 				{
// 					pUser->SendSysMsg("切磋前必须双方完全恢复!");
// 					pTarget->SendSysMsg("切磋前必须双方完全恢复!");
// 					return;
// 				}

				if(MapGroup(PID)->GetPVPManger()->findPVP(pUser->GetID()))
				{
					//MapGroup(PID)->GetPVPManger()->EndPVP(pUser->GetID());
				}
				if(MapGroup(PID)->GetPVPManger()->findPVP(pTarget->GetID()))
				{
					//MapGroup(PID)->GetPVPManger()->EndPVP(pTarget->GetID());
				}				
				//开始。。。。。。。。。。。

				pUser->SendSysMsg("切磋开始!");
				pTarget->SendSysMsg("切磋开始!");

				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,5))
					pTarget->SendMsg(this);

				IF_OK(Create(_FRIEND_PVP, pTarget->GetID(), pTarget->GetName(), true,0,0,0,0,0,5))
					pUser->SendMsg(this);

				

				//刷旗帜
				int flagX = (pUser->GetPosX() + pTarget->GetPosX()) / 2;
				int flagY = (pUser->GetPosY() + pTarget->GetPosY()) / 2;
				ST_CREATENEWNPC	info;
				memset(&info, 0, sizeof(ST_CREATENEWNPC));
				info.idMap			= pUser->GetMapID();
				info.idData			= 0;
				info.idOwner		= 0;		
				info.usPosX			= flagX;
				info.usPosY			= flagY;
				info.usType			= 9999;
				info.ucOwnerType	= 0;
				info.nData			= 0;
				int nType = pTarget->GetPVPTargetID();
				OBJID flagID = NpcManager()->CreateSynPetReturnID(&info);

				CRole::AttachStatus( pUser, STATUS_PVPMODE, 100, 3 * 60 + 5);
				CRole::AttachStatus( pTarget, STATUS_PVPMODE, 100, 3 * 60 + 5);

				if(2 == nType )
				{
					CRole::AttachStatus( pUser, STATUS_BIGBLOOD_USERNOT, 100, 3 * 60 + 5);
					CRole::AttachStatus( pTarget, STATUS_BIGBLOOD_USERNOT, 100, 3 * 60 + 5);
				}

				//为玩家加装爱河属性
				pUser->SetPVPTargetID(pTarget->GetID());
				pUser->SetPVPX(flagX);
				pUser->SetPVPY(flagY);
				pUser->SetPVPMap(pUser->GetMapID());

				pTarget->SetPVPTargetID(pUser->GetID());
				pTarget->SetPVPX(flagX);
				pTarget->SetPVPY(flagY);
				pTarget->SetPVPMap(pUser->GetMapID());
				MapGroup(PID)->GetPVPManger()->AddPVP(pUser->GetID(),pTarget->GetID(),nType,flagID);
			}
		}
		break;
		}
		DEBUG_CATCH2("MsgFriendChannel:%d ERROR!", m_pInfo->ucAction);
	}

// 	   catch (...)
// 	   {
// 		   ::LogSave("CMsgFriend %s %s %d",szDebugAction,szDebugStep, m_pInfo->ucAction);
// 		   
// 	   }
}
