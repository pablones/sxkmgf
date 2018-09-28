// MsgBetOutside.cpp: implementation of the CMsgAtk class.
// 仙剑修，2002.1.8
//////////////////////////////////////////////////////////////////////
//#include "AllMsg.h"
//#include "basefunc.h"
//#include "MapGroup.h"
//#include "SynManager.h"
#include "MsgSyndicate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSyndicate::CMsgSyndicate()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgSyndicate::~CMsgSyndicate()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget, OBJID idFealty)
{
	// check param
	if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
	{
//		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}

	// init
	this->Init();

	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;
//	m_pInfo->idFealty	=idFealty;
	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_SYNDICATE;


	return true;
}
BOOL CMsgSyndicate::Create(unsigned short usAction, char* pMsgBuf)
{
	this->Init();
	m_StrPacker.AddString(pMsgBuf);
	m_pInfo->usAction = usAction;
	
	m_unMsgType = _MSG_SYNDICATE;
	m_unMsgSize = sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return true;
}
//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create		(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_SYNDICATE != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
// void	CMsgSyndicate::Process		(CGameSocket* pSocket)
// {
// 	try {
// 	// 检查消息格式
// 	//来自其他地图组的没有参数
// 	if(SYN_WAR_SET == m_pInfo->usAction)//第一地图组广播
// 	{
// 		if(PID == MSGPORT_MAPGROUP_FIRST)
// 			return;
// 		OBJID thisID = 0,targetID = 0;
// 		int state = 0;
// 
// 		targetID = m_pInfo->idTarget;
// 		char szName[_MAX_NAMESIZE] = "";
// 		if (m_StrPacker.GetStrAmount() > 0)
// 		m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
// 		if(sscanf(szName,"%d %d",&thisID,&state) == 2)
// 		{
// 			CSynPtr pSyn = SynManager()->QuerySyndicate(thisID);
// 			if(!pSyn)
// 				return ;
// 			CSynPtr pSynTarget = SynManager()->QuerySyndicate(targetID);
// 			if(!pSynTarget)
// 				return ;
// 
// 			pSyn->SetSynWar(targetID,false);
// 			pSynTarget->SetSynWar(thisID,false);
// 
// 			pSyn->SetState(state);
// 			pSynTarget->SetState(state);
// 
// 			pSyn->SynchroInfo();
// 			pSynTarget->SynchroInfo();
// 			
// 		}		
// 
// 		if(PID < MSGPORT_MAPGROUP_INSTANCE - 1)
// 			CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(PID + 1,this);
// 		return;
// 	}
// 	else if(SYN_WAR_SEND == m_pInfo->usAction)//第一地图组接受
// 	{
// 		if(PID != MSGPORT_MAPGROUP_FIRST)
// 			return;
// 
// 		OBJID thisID = 0,targetID = 0;
// 		int state = 0;
// 		
// 		targetID = m_pInfo->idTarget;
// 		char szName[_MAX_NAMESIZE] = "";
// 		if (m_StrPacker.GetStrAmount() > 0)
// 			m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
// 		if(sscanf(szName,"%d %d",&thisID,&state) == 2)
// 		{
// 			CSynPtr pSyn = SynManager()->QuerySyndicate(thisID);
// 			if(!pSyn)
// 				return ;
// 			CSynPtr pSynTarget = SynManager()->QuerySyndicate(targetID);
// 			if(!pSynTarget)
// 					return ;
// 
// 			if(0 == state)//开始
// 			{
// 				pSyn->SetSynWar(targetID);
// 				pSynTarget->SetSynWar(thisID);
// 				MSGBUF szMsg;
// 				sprintf(szMsg, "%s帮会和%s帮会开始约战!", pSyn->GetStr(SYNDATA_NAME), pSynTarget->GetStr(SYNDATA_NAME));
// 				UserManager()->BroadcastMsg(szMsg, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);
// 			}
// 			else if(1 == state)//杀人
// 			{
// 				int synKill = pSynTarget->GetKill();
// 				pSynTarget->SetKill(synKill + 1);
// 
// 			}
// 			
// 			
// 		}
// 		return;
// 	}
	// 参数检查
// 	ASSERT(pSocket);
// 	if(!pSocket)
// 		return;
// 
// 	// 状态检查
// 	CUserPtr pUser = UserManager()->GetUser(this);
// 	IF_NOT(pUser)
// 		return;
// 
// 	OBJID idUser = pUser->GetID();
// 	IF_NOT(idUser != ID_NONE)
// 		return;
// 
// 	if(!pUser->IsAlive())
// 	{
// 		pUser->SendSysMsg("你已经死了");//STR_DIE
// 		return ;
// 	}
// 
// 	char szName[_MAX_NAMESIZE] = "";
// 	char szWord[256] = "";
// 	if (m_StrPacker.GetStrAmount() > 0)
// 		m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
// 	if (m_StrPacker.GetStrAmount() > 1)
// 		m_StrPacker.GetString(1, szWord, 256);
// 
// 	switch(m_pInfo->usAction)
// 	{
// 	case SYN_CREATE:
// 		{
// 			CItem* pItem = pUser->GetItem(m_pInfo->idTarget,false);
// 			if(!pItem || pItem->GetTypeID() != 630007)
// 				return;
// 
// 			if(pUser->GetMoney() < 1000000)
// 				return;
// 		
// 			CSyndicate* pSynOld = pUser->GetSyndicate();
// 			if(pSynOld )
// 			{
// 				if(pSynOld->GetInt(SYNDATA_STATE) == 1)
// 				{
// 					if(pUser->QuerySynAttr()->GetSynRank() == 100)
// 					{
// 						pUser->SendSysMsg("您的帮派在创建中，不能建立别的帮派");
// 						return;
// 					}
// 					else
// 					{
// 						if(!pUser->QuerySynAttr()->LeaveSyn())
// 						{
// 							pUser->SendSysMsg("您的帮派在创建中，不能建立别的帮派");
// 							return;
// 						}
// 					}
// 				}
// 				else
// 				{
// 					pUser->SendSysMsg("你已经在帮派中，退出后才能加入其它帮派");
// 					return;
// 				}
// 			}
// 
// 			OBJID idSyn =8888;/// pUser->QuerySynAttr()->CreateSyndicate(szName, szWord);
// 			if(idSyn != ID_NONE)
// 			{
// 				pUser->SpendItem(pItem);
// 				pUser->SpendMoney(1000000,true);
// 				
// 				this->Create(SYN_CREATE_OK,idSyn);
// 				pUser->SendMsg(this);
// 
// 				///帮派创建成功后尝试通过关系服务器广播到其它线去，让它们也创建这个帮派,以便其它线的玩家可以响应这个帮派
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, ALLUSERS_NAME);
// 
// 			}
// 			else
// 			{
// 				this->Create(SYN_CREATE_ERROR,pUser->GetID());
// 				pUser->SendMsg(this);
// 			}
// 		}
// 		break;
// 	case SYN_QUERY_CREATE:
// 		{
// 			SynManager()->SendCreateSynToUser(pUser,m_pInfo->dwData);
// 		}
// 		break;
// 	case SYN_JOIN:
// 		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(!pSyn)
// 				return;
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) //响应，不需要帮主请求 自动退上次相应
// 			{
// 				CSyndicate* pSynOld = pUser->GetSyndicate();
// 				if(pSynOld && (pSynOld->GetInt(SYNDATA_STATE) == 1))
// 				{
// 					if(pUser->QuerySynAttr()->GetSynRank() == 100)
// 					{
// 						pUser->SendSysMsg("您的帮派在创建中，不能响应别人的帮派");
// 					}
// 					else
// 					{
// 						if(!pUser->QuerySynAttr()->LeaveSyn())
// 							pUser->SendSysMsg("您的帮派在创建中，不能响应别人的帮派");
// 					}
// 
// 				}
// 				pUser->QuerySynAttr()->JoinSyn(m_pInfo->idTarget);
// 				pUser->SendSysMsg("响应成功");
// 			}
// 			else
// 			{
// 				pUser->SendSysMsg("对应帮派已经创建成功");
// 			}
// 
// 			CMsgSyndicateList msg;
// 			if (msg.Create(SYNLISTACTION_UPDATA,pSyn->GetID(),pSyn->GetStr(SYNDATA_NAME),pSyn->GetStr(SYNDATA_LEADERNAME),pSyn->GetInt(SYNDATA_LEVEL),pSyn->GetInt(SYNDATA_AMOUNT)))
// 				pUser->SendMsg(&msg);
// 		}
// 		break;
// 	case SYN_QUERY_INFO:
// 		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 
// 			if(!pSyn)
// 				return;
// 			pSyn->SendInfoToClient(pUser);
// 		}
// 		break;
// 
// 	case SYN_QUERY_MEMBER:
// 		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			
// 			if(!pSyn)
// 				return;
// 
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 			
// 			pSyn->SendMenberToClient(pUser,m_pInfo->dwData);
// 		}
// 		break;
// 	case SYN_QUERY_LIST:
// 		{
// 			SynManager()->SendSynList(pUser,m_pInfo->dwData);
// 		}
// 		break;
// 	case SYN_JOIN_QUERY:
// 		{
// 
// 			CSyndicate* pSynOld = pUser->GetSyndicate();
// 			if(pSynOld )
// 			{
// 				if(pSynOld->GetInt(SYNDATA_STATE) == 1)
// 				{
// 					if(pUser->QuerySynAttr()->GetSynRank() == 100)
// 					{
// 						pUser->SendSysMsg("您的帮派在创建中，不能加入别的帮派");
// 						return;
// 					}
// 					else
// 					{
// 						if(!pUser->QuerySynAttr()->LeaveSyn())
// 						{
// 							pUser->SendSysMsg("您的帮派在创建中，不能加入别的帮派");
// 							return;
// 						}
// 					}
// 				}
// 				else
// 				{
// 					pUser->SendSysMsg("你已经在帮派中，退出后才能加入其它帮派");
// 					return;
// 				}
// 			}
// 
// 
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			
// 			if(!pSyn)
// 				return;
// 			
// 			if(pSyn->GetInt(SYNDATA_STATE) != 2) 
// 				return;
// 
// 			int levMemberAmount[] = {50,50,100,150,250};
// 			if(pSyn->GetMemberAmount() >= levMemberAmount[pSyn->GetInt(SYNDATA_LEVEL)])
// 			{
// 				pUser->SendSysMsg("帮会人数已满");
// 				return;
// 			}
// 			
// 			CSyndicate::MemberInfo info;
// 			info.nId = pUser->GetID();
// 			info.nLevel = pUser->GetLev();
// 			info.firJob = pUser->GetProfession(1);
// 			info.firLevel = pUser->GetJobLev(1);
// 			info.nRank = 10;
// 			info.nProffer = 0;
// 			SafeCopy(info.szName,pUser->GetName());
// 
// 			if(pSyn->AddMemberList(info,2) == RANK_NORMAL)
// 			{
// 				pUser->SendSysMsg("申请发送成功");
// 			}
// 			else
// 			{
// 				pUser->SendSysMsg("你已经在对方的申请列表中，请不要重复申请");
// 			}
// 		}
// 		break;
// 	case SYN_QUERY_QUERYMEMBER:
// 		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			
// 			if(!pSyn)
// 				return;
// 			
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 			
// 			pSyn->SendQueryMenberToClient(pUser,m_pInfo->dwData);
// 		}
// 		break;
// 	case SYN_ANSERR_OK:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 
// 			int nRank = pUser->QuerySynRank();
// 			if(nRank < RANK_FEELLEADER1)
// 			{
// 				pUser->SendSysMsg("你的权限不足");
// 				return;
// 			}
// 
// 			int levMemberAmount[] = {50,50,100,150,250};
// 			if(pSyn->GetMemberAmount() >= levMemberAmount[pSyn->GetInt(SYNDATA_LEVEL)])
// 			{
// 				pUser->SendSysMsg("帮会人数已满");
// 				return;
// 			}
// 
// 			if(!pSyn->AllowMenber(m_pInfo->idTarget,pUser))
// 			{
// 				pUser->SendSysMsg("对方已经加入了帮派");
// 				return;
// 			}
// 
// 			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
// 			if(pThisUser)
// 			{
// 				pThisUser->SendSysMsg("加入%s通过",pSyn->GetStr(SYNDATA_NAME));
// 
// 				this->Create(SYN_UPDATE,pThisUser->GetID());
// 				pThisUser->SendMsg(this);
// 				pUser->SendMsg(this);
// 			}
// 			else
// 			{
// 				this->Create(SYN_UPDATE,pUser->GetID());
// 				pUser->SendMsg(this);
// 			}
// 		}
// 		break;
// 	case SYN_ANSERR_NO:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 
// 			int nRank = pUser->QuerySynRank();
// 			if(nRank < RANK_FEELLEADER1)
// 			{
// 				pUser->SendSysMsg("你的权限不足");
// 				return;
// 			}
// 
// 			pSyn->DelMemberList(m_pInfo->idTarget,2);
// 			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
// 			if(pThisUser)
// 			{
// 				pThisUser->SendSysMsg("被%s拒绝",pSyn->GetStr(SYNDATA_NAME));
// 			}
// 
// 			this->Create(SYN_UPDATE,pUser->GetID());
// 			pUser->SendMsg(this);
// 		}
// 		break;
// 	case SYN_KICKOUT:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 
// 			int nMyRank = pUser->QuerySynRank();
// 
// 			if(nMyRank < RANK_FEELLEADER1)
// 			{
// 				pUser->SendSysMsg("你的权限不足");
// 				return;
// 			}
// 
// 			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
// 			if(pThisUser)
// 			{
// 				if((pThisUser->GetSyndicate()->GetID() != pSyn->GetID()) || (nMyRank / 10  <= pThisUser->QuerySynRank() / 10))
// 				{
// 					pUser->SendSysMsg("你的权限不足");
// 					return;
// 				}
// 
// 				if(nMyRank / 10 == 8)
// 				{
// 					if( pThisUser->QuerySynRank() % 10 != nMyRank % 10 )
// 					{
// 						pUser->SendSysMsg("你不能踢出其他堂的成员");
// 						return;
// 					}
// 				}
// 
// 				pThisUser->QuerySynAttr()->LeaveSyn(true);
// 				pThisUser->SendSysMsg("你被踢出帮派");
// 
// 				this->Create(SYN_UPDATE,pThisUser->GetID());
// 				pThisUser->SendMsg(this);
// 				pUser->SendMsg(this);
// 			}
// 			else
// 			{
// 				int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
// 				if(nThisRank == RANK_NONE)
// 				{
// 					pUser->SendSysMsg("对方已经离开了帮派");
// 					return;
// 				}
// 
// 				if(nMyRank / 10  <= nThisRank / 10)
// 				{
// 					pUser->SendSysMsg("你的权限不足");
// 					return;
// 				}
// 
// 				if(nMyRank / 10 == 8)
// 				{
// 					if(nThisRank % 10 != nMyRank % 10 )
// 					{
// 						pUser->SendSysMsg("你不能踢出其他堂的成员");
// 						return;
// 					}
// 				}
// 
// 				pUser->QuerySynAttr()->KickoutMember(m_pInfo->idTarget);
// 
// 				this->Create(SYN_UPDATE,pUser->GetID());
// 				pUser->SendMsg(this);
// 			}
// 			pUser->SendSysMsg("踢出成功");
// 		}
// 		break;
// 	case SYN_LEAVER:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			int nMyRank = pUser->QuerySynRank();
// 			if(nMyRank == RANK_LEADER)
// 				return;
// 
// 			pUser->QuerySynAttr()->LeaveSyn();
// 			pUser->SendSysMsg("离开成功");
// 
// 			this->Create(SYN_UPDATE,pUser->GetID());
// 			pUser->SendMsg(this);
// 		}
// 		break;
// 	case SYN_SETRANK:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			int nMyRank = pUser->QuerySynRank();
// 			if(nMyRank < RANK_FEELLEADER1)
// 			{
// 				pUser->SendSysMsg("你的权限不足");
// 				return;
// 			}
// 
// 			int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
// 			if(nThisRank == RANK_NONE)
// 			{
// 				pUser->SendSysMsg("对方已经离开了帮派");
// 				return;
// 			}
// 
// 			if(nMyRank / 10  <= nThisRank / 10)
// 			{
// 				pUser->SendSysMsg("你的权限不足");
// 				return;
// 			}
// 
// 			if(nMyRank / 10 == 8)
// 			{
// 				if(m_pInfo->dwData % 10 != nMyRank % 10 )
// 				{
// 					pUser->SendSysMsg("你不能任命其他堂的职位");
// 					return;
// 				}
// 			}
// 
// 			if(m_pInfo->dwData < RANK_NORMAL || m_pInfo->dwData > RANK_SUBLEADER)
// 			{
// 				pUser->SendSysMsg("职位出错");
// 				return;
// 			}
// 
// 			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),m_pInfo->dwData))
// 			{
// 				this->Create(SYN_UPDATE,m_pInfo->idTarget);
// 				pUser->SendMsg(this);
// 				pUser->SendSysMsg("任命成功");
// 			}
// 		}
// 		break;
// 
// 	case SYN_DEMISE:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			int nMyRank = pUser->QuerySynRank();
// 			if(nMyRank != RANK_LEADER)
// 			{
// 				pUser->SendSysMsg("你的权限不足");
// 				return;
// 			}
// 
// 			int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
// 			if(nThisRank == RANK_NONE)
// 			{
// 				pUser->SendSysMsg("对方已经离开了帮派");
// 				return;
// 			}
// 
// 			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),RANK_LEADER))
// 				pUser->SendSysMsg("让位成功");
// 
// 		}
// 		break;
// 	case SYN_ANNONCE:
// 		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			int nMyRank = pUser->QuerySynRank();
// 			if(nMyRank != RANK_LEADER && nMyRank != RANK_SUBLEADER)
// 			{
// 				pUser->SendSysMsg("你的权限不足");
// 				return;
// 			}
// 			
// 			pSyn->SetStr(SYNDATA_ANNOUNCE,szWord,256,true);
// 
// 
// 			CMsgTalk	msg;
// 			IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szWord, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE))
// 				pSyn->BroadcastSynMsg(&msg);
// 
// 			pUser->SendSysMsg("修改成功");
// 		}
// 		break;
// 		
// // 	case SYN_DEMISE:
// // 		{
// // 			//  [1/15/2008 朱斌]	军团人事开关
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// 检查帮派
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			
// // 
// // 			//[游途道标 2008/9/27]对方必须是副帮主
// // 			// 检查权限
// // 			
// // 			if(pUser->GetSynRank() != RANK_LEADER
// // 				|| pUser->QuerySynAttr()->GetPlayRankByName(szName) != 90 
// // 				|| pUser->QuerySynAttr()->GetSynIDByOffLinePlay(szName) != idSyn)
// // 			{
// // 				return;
// // 			}
// // 
// // 			// target syn
// // 			CUser* pTarget = UserManager()->GetUser(szName);
// // 
// // 			//[游途道标 2008/9/27]被提降职者是否在线
// // 			bool bOnline = true;
// // 			bool bWifeOnline = true;
// // 			bool bWife = true;
// // 			bool bSubWifeOnline = false;
// // 			bool bSubWife = false;
// // 			CUser* pwife = NULL;
// // 			CUser* pSubwife = NULL;
// // 
// // 			OBJID subLeadID = pUser->QuerySynAttr()->GetUserIDByName(szName); //副帮主ID
// // 			OBJID LeadWifeID = pSyn->GetLeaderWifeID();                       //帮主夫人ID
// // 			OBJID subLeadWifeID = ID_NONE;
// // 			
// // 
// // 			if(!pTarget)
// // 			{
// // 				//[游途道标 2008/9/27]修改不在线的直接记录其状态后面直接写数据库
// // 				bOnline = false;
// // 				//pUser->SendSysMsg(g_objStrRes.GetStr(12011));//STR_NOT_HERE
// // 				//return ;
// // 			}
// // 
// // 			//[游途道标 2008/9/27]屏蔽等级判断和一些无用的判断
// // 			//int nLevel = 40;
// // 			//int nProffer = 50000;		
// // 			//if(pTarget->GetLev() < nLevel || pTarget->QuerySynProffer() < nProffer)
// // 			//{
// // 			//	char szBuff[128];
// // 			//	sprintf(szBuff,g_objStrRes.GetStr(12014),pTarget->GetName());//STR_LEVEL_NOT_ENOUGH
// // 			//	pUser->SendSysMsg(szBuff);
// // 			//	return ;
// // 			//}
// // 
// // 			//[游途道标 2008/9/27]不在线和在线的分别对待
// // 			OBJID	idTargetSyn;
// // 			if (bOnline)
// // 			{
// // 				idTargetSyn = pTarget->GetSynID();
// // 			}
// // 			else
// // 			{
// // 				idTargetSyn = pUser->QuerySynAttr()->GetSynIDByOffLinePlay(szName);
// // 			}
// // 
// // 			//[游途道标 2008/9/27]不在线和在线的分别对待
// // 			if (bOnline)
// // 			{
// // 				if((pTarget->GetID() == pUser->GetID()) || (pTarget->GetSynID() != pUser->GetSynID()))
// // 				{
// // 					return;
// // 				}
// // 				if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(pTarget->GetID()) )
// // 				{
// // 					pUser->SendSysMsg("操作失败！对方已经申请退帮！");
// // 					return;
// // 				}
// // 			}
// // 			else
// // 			{
// // 				OBJID targetID = pUser->QuerySynAttr()->GetUserIDByName(szName);
// // 				if((targetID == pUser->GetID()) || (idTargetSyn != pUser->GetSynID()))
// // 				{
// // 					return;
// // 				}
// // 				if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(targetID) )
// // 				{
// // 					pUser->SendSysMsg("操作失败！对方已经申请退帮！");
// // 					return;
// // 				}
// // 			}
// // 
// // 			if(idTargetSyn == ID_NONE || idTargetSyn != idSyn)
// // 				return ;
// // 
// // 			if (bOnline)
// // 			{
// // 				pSyn->QueryModify()->Demise(pUser->GetID(),pUser->QuerySynAttr()->GetSynMemberLevel(), pTarget->GetID(), pTarget->GetName(), pTarget->QuerySynAttr()->GetSynMemberLevel(),pSyn->GetID());
// // 				pTarget->QuerySynAttr()->SetRank(100, true);
// // 				pUser->QuerySynAttr()->SetRank(10, true);
// // 				pTarget->QuerySynAttr()->SynchroInfo();
// // 				pUser->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else
// // 			{
// // 				pUser->QuerySynAttr()->Demise(pUser->GetID(),(char *)pUser->GetName(), pUser->QuerySynAttr()->GetUserIDByName(szName), (char *)szName);
// // 				pSyn->QueryModify()->Demise(0,0, pUser->QuerySynAttr()->GetUserIDByName(szName), (char *)szName, 0, pSyn->GetID());
// // 			}
// // 
// // // 			pSyn->SetSubLeaderID(0);
// // 			pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // //---------------------------------------------------------
// // 			if(LeadWifeID == ID_NONE)
// // 				bWife = false;
// // 			if(bWife)
// // 			{
// // 				pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, 0, true);
// // 				pwife = UserManager()->GetUser(LeadWifeID);
// // 				if(!pwife)
// // 				{
// // 					bWifeOnline = false;
// // 				}
// // 			}
// // 			char temp[16] = {0};
// // 			
// // 			pUser->QuerySynAttr()->GetMateIDByID(subLeadWifeID, subLeadID);
// // //			pUser->QuerySynAttr()->GetMateNameByID(temp , subLeadID);
// // //			if(strcmp("无",temp) != 0)
// // //				subLeadWifeID = pUser->QuerySynAttr()->GetUserIDByName(temp); //副帮主夫人ID
// // 			if(subLeadWifeID != ID_NONE && pUser->QuerySynAttr()->GetSynIDByUserID(subLeadWifeID) == pSyn->GetID())
// // 				bSubWife = true;
// // 			if(bSubWife)
// // 			{
// // 				pSubwife = UserManager()->GetUser(subLeadWifeID);
// // 				if(pSubwife)
// // 				{
// // 					bSubWifeOnline = true;
// // 				}
// // 			}
// // 			//----------------------------------------
// // 			if(bSubWife && bSubWifeOnline)
// // 			{			
// // 				pSubwife->QuerySynAttr()->SetRank(80, true);				
// // 				pSubwife->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else if(bSubWife)
// // 			{				
// // 			// 	pUser->QuerySynAttr()->SetRankByName(80,temp);
// // 				pUser->QuerySynAttr()->SetRankByID(80,subLeadWifeID);
// // 			}
// // 			if(bSubWife)
// // 			{
// // // 				pSyn->SetLeaderWifeID(subLeadWifeID);
// // 				pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, subLeadWifeID, true);
// // 				pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), "新帮主配偶产生");
// // 			}
// // 
// // 			if(bWife && bWifeOnline)
// // 			{
// // 				pwife->QuerySynAttr()->SetRank(10, true);				
// // 				pwife->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else if(bWife)
// // 			{
// // //				pUser->QuerySynAttr()->GetMateNameByID(temp , pUser->GetID());
// // //				pUser->QuerySynAttr()->SetRankByName(10,temp);
// // 
// // 				pUser->QuerySynAttr()->SetRankByID(10, LeadWifeID);
// // 			}
// // 			
// // 
// // 			
// // 			char szBuff[128];
// // 			sprintf(szBuff,g_objStrRes.GetStr(10043),szName);//STR_SYN_DEMISE_SUCC
// // 			pUser->SendSysMsg(szBuff);
// // 			if (bOnline)
// // 			{
// // 				sprintf(szBuff,g_objStrRes.GetStr(10044),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_DEMISE_CON
// // 				pTarget->SendSysMsg(szBuff);
// // 				sprintf(szBuff,g_objStrRes.GetStr(10045),pTarget->GetName(),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_DEMISE_PROMPT
// // 				pTarget->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 			}	
// // 		}
// // 		break;
// // 	case SYN_SET_ASSISTANT://[游途道标 2008/9/28]对帮众进行提职
// // 		{
// // 						// 检查帮派
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			// 检查权限
// // 			int nRank = pUser->GetSynRank();
// // 
// // 			//[游途道标 2008/9/27]屏蔽帮判断
// // 			//if(nRank != RANK_LEADER)
// // 			//	return ;
// // 
// // 			//[游途道标 2008/9/27]屏蔽此处判断,下面统一判断.
// // 			// check amount, temporary code.
// // 			//if(pUser->QuerySynAttr()->GetAssistantCount() >= MAX_ASSISTANTSIZE)
// // 			//{
// // 			//	pUser->SendSysMsg(g_objStrRes.GetStr(11099));//STR_SYN_PLACE_FULL
// // 			//	return ;
// // 			//}
// // 
// // 			// target syn
// // 			CUser* pTarget = UserManager()->GetUser(szName);
// // 			//[游途道标 2008/9/27]被提职者是否在线
// // 			bool bOnline = true;
// // 			if(!pTarget )/*|| (!pTarget->GetSyndicate()->IsSubSyn())*/
// // 			{
// // 				//[游途道标 2008/9/27]修改不在线的直接记录其状态后面直接写数据库
// // 				bOnline = false;
// // 				//pUser->SendSysMsg(g_objStrRes.GetStr(12011));//STR_NOT_HERE
// // 				//return ;
// // 			}
// // 			//20070428修罗:策划需求,去掉判断
// // //			int nLevel = 35;
// // //			int nProffer = 30000;		
// // //			int rand = 50;
// // //			if(pTarget->GetLev() < nLevel || pTarget->QuerySynProffer() < nProffer || pTarget->GetSynRank()<50)
// // //			{
// // //				char szBuff[128];
// // //				sprintf(szBuff,STR_LEVEL_NOT_ENOUGH,pTarget->GetName());
// // //				pUser->SendSysMsg(szBuff);
// // //				pTarget->SendSysMsg(STR_SYN_SETSUBLEADER_ERR);
// // //				return ;
// // //			}
// // 
// // 			//[游途道标 2008/9/27]不在线和在线的分别对待
// // 			OBJID	idTargetSyn;
// // 			if (bOnline)
// // 			{
// // 				idTargetSyn = pTarget->GetSynID();
// // 			}
// // 			else
// // 			{
// // 				idTargetSyn = pUser->QuerySynAttr()->GetSynIDByOffLinePlay(szName);
// // 			}
// // 			if(idTargetSyn == ID_NONE || idTargetSyn != idSyn)
// // 				return ;
// // 			CSynPtr pSubSyn = SynManager()->QuerySyndicate(idTargetSyn);
// // 			CHECK(pSubSyn);
// // 			
// // 			int nTargetRank = pUser->QuerySynAttr()->GetPlayRankByName(szName);
// // 			OBJID targetID = pUser->QuerySynAttr()->GetUserIDByName(szName);
// // 
// // 			if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(targetID) )
// // 			{
// // 				pUser->SendSysMsg("操作失败！对方已经申请退帮！");
// // 				return;
// // 			}
// // 	
// // 
// // 			int nNewRank = 10;
// // 
// // 			switch (nTargetRank)
// // 			{
// // 			case 100:
// // 				return;
// // 			case 90:
// // 				return;
// // 			case 80:
// // 				return;
// // 			case 60:
// // 				if(/*pUser->QuerySynAttr()->GetRankCount(90)*/pSyn->GetSubLeaderID() != ID_NONE || (100 != nRank))
// // 				{
// // 					return;
// // 				}
// // 				nNewRank = 90;
// // 				break;
// // 			//[游途道标 2008/9/27]暂时不需要多个等级的会员
// // 			//case 30:
// // 			default:
// // 				if (((pSyn->GetInt(SYNDATA_RANK) * 2 + 2) <= pUser->QuerySynAttr()->GetRankCount(60))
// // 					&& (90 > nRank))
// // 				{
// // 					return;
// // 				}
// // 				nNewRank = 60;
// // 				break;
// // 			/*default:
// // 				if (80 > nRank)
// // 				{
// // 					return;
// // 				}
// // 				nNewRank = nTargetRank + 10;
// // 				break;*/
// // 			}
// // 
// // 			//[游途道标 2008/9/27]不在线和在线的分别对待
// // 			if(nNewRank == 90)
// // 			{
// // 			// 	pSyn->SetSubLeaderID(targetID);
// // 				pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, targetID, true);
// // 			}
// // 			if (bOnline)
// // 			{
// // 				pTarget->QuerySynAttr()->SetRank(nNewRank, true);				
// // 				pUser->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else
// // 			{
// // 				pUser->QuerySynAttr()->SetRankByName(nNewRank, szName);
// // 			}
// // 	/*		CMsgSyndicate msg;
// // 			if(msg.Create(SYN_SET_BUS,(char*)pTarget->GetName()))
// // 			{
// // 				msg.m_pInfo->dwData	=	90;
// // 				pSubSyn->BroadcastSynMsg(&msg, pUser);
// // 			}*/
// // 
// // 			//[游途道标 2008/9/27]屏蔽不必要部分,导致副帮主提升出错
// // 			/*SQLBUF szSQL = "";
// // 			sprintf(szSQL,"SELECT s.id,u.name from %s AS s LEFT JOIN sk_user AS u ON u.id = s.id  where s.syn_id= %d and s.rank = 90",_TBL_SYNATTR,idSyn);
// // 
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				CUser *pUser = UserManager()->GetUser(pRes->GetInt(0));
// // 				if(pUser)
// // 				{
// // 					pUser->QuerySynAttr()->SetRank(50);
// // 				}
// // 				else
// // 				{
// // 					MESSAGESTR	buf;
// // 					SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// // 					pInfo->idSyn		= idSyn;
// // 					pInfo->nRank		= 50;
// // 					pInfo->ucFuncType	= SYNFUNC_SET_RANK;
// // 					SafeCopy(pInfo->StrParam, pRes->GetStr(1), _MAX_NAMESIZE);
// // 					pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
// // 					MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
// // 					sprintf(szSQL, "UPDATE %s SET rank = 50  WHERE id = %u LIMIT 1",
// // 								_TBL_SYNATTR,pRes->GetInt(0));
// // 					Database()->ExecuteSQL(szSQL);
// // 				}
// // 			}*/
// // 
// // 			char szBuff[128];
// // 			if(nNewRank == 90)
// // 			{
// // 				sprintf(szBuff,g_objStrRes.GetStr(10047),szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_SUCC
// // 				pUser->SendSysMsg(szBuff);
// // 				//[游途道标 2008/9/27]不在线和在线的分别对待
// // 				if (bOnline)
// // 				{
// // 					sprintf(szBuff,g_objStrRes.GetStr(10048),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_CON
// // 					pTarget->SendSysMsg(szBuff);
// // 				}
// // 				sprintf(szBuff,g_objStrRes.GetStr(10049),szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_PROMPT
// // 					pUser->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 			}
// // 			else
// // 			{
// // 				sprintf(szBuff,"您册封了%s为%s舵主一职",szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_SUCC
// // 				pUser->SendSysMsg(szBuff);
// // 				//[游途道标 2008/9/27]不在线和在线的分别对待
// // 				if (bOnline)
// // 				{
// // 					sprintf(szBuff,"恭喜您成为了%s舵主",pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_CON
// // 					pTarget->SendSysMsg(szBuff);
// // 				}
// // 				sprintf(szBuff,"%s出任%s舵主一职",szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_PROMPT
// // 					pUser->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 			}
// // 			
// // 			
// // 		}
// // 		break;
// // 	case SYN_SET_BUS://[游途道标 2008/9/28]对帮众进行降职
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			// 检查权限
// // 			int nRank = pUser->GetSynRank();
// // 
// // 			//[游途道标 2008/9/27]屏蔽帮判断
// // 			//if(nRank != RANK_LEADER)
// // 			//	return ;
// // 
// // 			//[游途道标 2008/9/27]屏蔽此处判断,下面统一判断.
// // 			// check amount, temporary code.
// // 			//if(pUser->QuerySynAttr()->GetAssistantCount() >= MAX_ASSISTANTSIZE)
// // 			//{
// // 			//	pUser->SendSysMsg(g_objStrRes.GetStr(11099));//STR_SYN_PLACE_FULL
// // 			//	return ;
// // 			//}
// // 
// // 			// target syn
// // 			CUser* pTarget = UserManager()->GetUser(szName);
// // 			//[游途道标 2008/9/27]被提职者是否在线
// // 			bool bOnline = true;
// // 			if(!pTarget /*|| (!pTarget->GetSyndicate()->IsSubSyn())*/)
// // 			{
// // 				//[游途道标 2008/9/27]修改不在线的直接记录其状态后面直接写数据库
// // 				bOnline = false;
// // 				//pUser->SendSysMsg(g_objStrRes.GetStr(12011));//STR_NOT_HERE
// // 				//return ;
// // 			}
// // 			//20070428修罗:策划需求,去掉判断
// // //			int nLevel = 35;
// // //			int nProffer = 30000;		
// // //			int rand = 50;
// // //			if(pTarget->GetLev() < nLevel || pTarget->QuerySynProffer() < nProffer || pTarget->GetSynRank()<50)
// // //			{
// // //				char szBuff[128];
// // //				sprintf(szBuff,STR_LEVEL_NOT_ENOUGH,pTarget->GetName());
// // //				pUser->SendSysMsg(szBuff);
// // //				pTarget->SendSysMsg(STR_SYN_SETSUBLEADER_ERR);
// // //				return ;
// // //			}
// // 
// // 			OBJID targetID = pUser->QuerySynAttr()->GetUserIDByName(szName);			
// // 			if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(targetID) )
// // 			{
// // 				pUser->SendSysMsg("操作失败！对方已经申请退帮！");
// // 				return;
// // 			}
// // 
// // 			//[游途道标 2008/9/27]不在线和在线的分别对待
// // 			OBJID	idTargetSyn;
// // 			if (bOnline)
// // 			{
// // 				idTargetSyn = pTarget->GetSynID();
// // 			}
// // 			else
// // 			{
// // 				idTargetSyn = pUser->QuerySynAttr()->GetSynIDByOffLinePlay(szName);
// // 			}
// // 			if(idTargetSyn == ID_NONE || idTargetSyn != idSyn)
// // 				return ;
// // 			CSynPtr pSubSyn = SynManager()->QuerySyndicate(idTargetSyn);
// // 			CHECK(pSubSyn);
// // 			
// // 			int nTargetRank = pUser->QuerySynAttr()->GetPlayRankByName(szName);
// // 
// // 			int nNewRank = 10;
// // 
// // 			switch (nTargetRank)
// // 			{
// // 			case 100:
// // 				return;
// // 			case 80:
// // 				return;
// // 			case 90:
// // 				if (((pSyn->GetInt(SYNDATA_RANK) * 2 + 2) <= pUser->QuerySynAttr()->GetRankCount(80))
// // 					&& (90 > nRank))
// // 				{
// // 					return;
// // 				}
// // 				nNewRank = 60;
// // 				break;
// // 			case 60:
// // 				nNewRank = 10;
// // 				break;
// // 			//[游途道标 2008/9/27]暂时不需要多个等级的会员
// // 			//case 30:
// // 			default:
// // 				return;
// // 			}
// // 
// // 			//[游途道标 2008/9/27]不在线和在线的分别对待
// // 			if(nTargetRank == 90)
// // 			{
// // 			// 	pSyn->SetSubLeaderID(0);
// // 				pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 			}
// // 			if (bOnline)
// // 			{
// // 				pTarget->QuerySynAttr()->SetRank(nNewRank, true);
// // 				pUser->QuerySynAttr()->SynchroInfo();
// // 			}
// // 			else
// // 			{
// // 				pUser->QuerySynAttr()->SetRankByName(nNewRank, szName);
// // 			}
// // 	/*		CMsgSyndicate msg;
// // 			if(msg.Create(SYN_SET_BUS,(char*)pTarget->GetName()))
// // 			{
// // 				msg.m_pInfo->dwData	=	90;
// // 				pSubSyn->BroadcastSynMsg(&msg, pUser);
// // 			}*/
// // 
// // 			//[游途道标 2008/9/27]屏蔽不必要部分,导致副帮主提升出错
// // 			/*SQLBUF szSQL = "";
// // 			sprintf(szSQL,"SELECT s.id,u.name from %s AS s LEFT JOIN sk_user AS u ON u.id = s.id  where s.syn_id= %d and s.rank = 90",_TBL_SYNATTR,idSyn);
// // 
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				CUser *pUser = UserManager()->GetUser(pRes->GetInt(0));
// // 				if(pUser)
// // 				{
// // 					pUser->QuerySynAttr()->SetRank(50);
// // 				}
// // 				else
// // 				{
// // 					MESSAGESTR	buf;
// // 					SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
// // 					pInfo->idSyn		= idSyn;
// // 					pInfo->nRank		= 50;
// // 					pInfo->ucFuncType	= SYNFUNC_SET_RANK;
// // 					SafeCopy(pInfo->StrParam, pRes->GetStr(1), _MAX_NAMESIZE);
// // 					pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
// // 					MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
// // 					sprintf(szSQL, "UPDATE %s SET rank = 50  WHERE id = %u LIMIT 1",
// // 								_TBL_SYNATTR,pRes->GetInt(0));
// // 					Database()->ExecuteSQL(szSQL);
// // 				}
// // 			}*/
// // 
// // 			char szBuff[128];
// // 			if(nNewRank == 60)
// // 			{
// // 				sprintf(szBuff,"您册封了%s为%s舵主一职",szName,pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_SUCC
// // 				pUser->SendSysMsg(szBuff);
// // 				//[游途道标 2008/9/27]不在线和在线的分别对待
// // 				if (bOnline)
// // 				{
// // 					sprintf(szBuff,"您成为了%s舵主",pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_CON
// // 					pTarget->SendSysMsg(szBuff);
// // 					sprintf(szBuff,"%s出任%s舵主一职",pTarget->GetName(),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBLEADER_PROMPT
// // 					pTarget->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 				}
// // 
// // 			}
// // 			else
// // 			{
// // 				sprintf(szBuff,"您解除了%s的职务",szName);//STR_SYN_SETSUBLEADER_SUCC
// // 				pUser->SendSysMsg(szBuff);
// // 				//[游途道标 2008/9/27]不在线和在线的分别对待
// // 				if (bOnline)
// // 				{
// // 					strcpy(szBuff,"您的职务被解除了");//STR_SYN_SETSUBLEADER_CON
// // 					pTarget->SendSysMsg(szBuff);
// // 					sprintf(szBuff,"%s的职务被解除",pTarget->GetName());//STR_SYN_SETSUBLEADER_PROMPT
// // 					pTarget->SendSysMsg(_TXTATR_SYNDICATE,szBuff);
// // 				}
// // 
// // 			}
// // 			
// // 			
// // 		}
// // 		break;
// // 	case INVITE_RETIRE://[游途道标 2008/10/7]帮众辞职
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			if( KICKOUTDATA_OUTDO_FALSE != pUser->QuerySynAttr()->GetLeaveSyn(pUser->GetID()) )
// // 			{
// // 				pUser->SendSysMsg("操作失败！你已经申请退帮！");
// // 				return;
// // 			}
// // 
// // 			int nTargetRank = pUser->QuerySynAttr()->GetPlayRankByName(szName);
// // 			if(nTargetRank == 90)
// // 			{
// // 		// 		pSyn->SetSubLeaderID(0);
// // 				pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 			}
// // 
// // 			char szBuff[128];
// // 			sprintf(szBuff,"%s %s 辞去 （%s） 职位！",pUser->QuerySynAttr()->GetRankTitle(),pUser->GetName(),pUser->QuerySynAttr()->GetRankTitle());
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szBuff);
// // 		
// // 			pUser->QuerySynAttr()->SetRank(10, true);
// // 			pUser->QuerySynAttr()->SynchroInfo();			
// // 
// // 			sprintf(szBuff,"你已经辞职成功.");//STR_SYN_SETSUBLEADER_SUCC
// // 			pUser->SendSysMsg(szBuff);	
// // 		}
// // 		break;
// // 	case SYN_BUY_UP:
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			int level = pSyn->GetBuyLev();
// // 			if(level < 3 )
// // 			{
// // 				int nAmount = 0;
// // 				int nMoney = 0;
// // 				for(int i = 0; i < SynHomeLevSet()->GetAmount(); i++)
// // 				{
// // 					CSynHomeLevData* pType = SynHomeLevSet()->GetObjByIndex(i);
// // 					if(11 + level == pType->GetInt(SYNHOMELEVDATA_TYPE))
// // 					{
// // 						nAmount = pType->GetInt(SYNHOMELEVDATA_RANK);
// // 						nMoney = pType->GetInt(SYNHOMELEVDATA_MONEY);
// // 						break;
// // 					}
// // 				}
// // 					
// // 				int enReput = pSyn->GetInt(SYNDATA_REPUTE);
// // 				if(nAmount > enReput)
// // 				{
// // 					pUser->SendSysMsg("贡献度不够!");
// // 					return;
// // 				}
// // 				int data = max(0,enReput - nAmount);
// // 			// 	pSyn->SetRepute(data);	
// // 				pSyn->QueryModify()->SetData(SYNDATA_REPUTE, data, true);
// // 				
// // 				int enMoney = pUser->GetMoney();
// // 				if(nMoney > enMoney)
// // 				{
// // 					pUser->SendSysMsg("资金不足!");
// // 					return;
// // 				}
// // 				pUser->SpendMoney(nMoney,true);
// // 
// // //				pSyn->SetBuyLev(level + 1);
// // //				pSyn->SynchroInfo();
// // 				pSyn->QueryModify()->SetData(SYNDATA_BUYLEV, level + 1, true);
// // 			}
// // 		}
// // 		break;
// // 	case SYN_MAIN_UP:		
// // 		break;
// // 	case SYN_TASK_UP:
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			int level = pSyn->GetTaskLev();
// // 			if(level < 3 )
// // 			{
// // 				int nAmount = 0;
// // 				int nMoney = 0;
// // 				for(int i = 0; i < SynHomeLevSet()->GetAmount(); i++)
// // 				{
// // 					CSynHomeLevData* pType = SynHomeLevSet()->GetObjByIndex(i);
// // 					if(21 + level == pType->GetInt(SYNHOMELEVDATA_TYPE))
// // 					{
// // 						nAmount = pType->GetInt(SYNHOMELEVDATA_RANK);
// // 						nMoney = pType->GetInt(SYNHOMELEVDATA_MONEY);
// // 						break;
// // 					}
// // 				}
// // 					
// // 				int enReput = pSyn->GetInt(SYNDATA_REPUTE);
// // 				if(nAmount > enReput)
// // 				{
// // 					pUser->SendSysMsg("贡献度不够!");
// // 					return;
// // 				}
// // 				int data = max(0,enReput - nAmount);
// // 			// 	pSyn->SetRepute(data);	
// // 				pSyn->QueryModify()->SetData(SYNDATA_REPUTE, data, true);
// // 				
// // 				int enMoney = pUser->GetMoney();
// // 				if(nMoney > enMoney)
// // 				{
// // 					pUser->SendSysMsg("资金不足!");
// // 					return;
// // 				}
// // 				pUser->SpendMoney(nMoney,true);
// // 
// // //				pSyn->SetTaskLev(level + 1);
// // //				pSyn->SynchroInfo();
// // 				pSyn->QueryModify()->SetData(SYNDATA_TASKLEV, level + 1, true);
// // 			}
// // 		}
// // 		break;
// // 	case SYN_CHG_UP:
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			int level = pSyn->GetChgLev();
// // 			if(level < 3 )
// // 			{
// // 				int nAmount = 0;
// // 				int nMoney = 0;
// // 				for(int i = 0; i < SynHomeLevSet()->GetAmount(); i++)
// // 				{
// // 					CSynHomeLevData* pType = SynHomeLevSet()->GetObjByIndex(i);
// // 					if(31 + level == pType->GetInt(SYNHOMELEVDATA_TYPE))
// // 					{
// // 						nAmount = pType->GetInt(SYNHOMELEVDATA_RANK);
// // 						nMoney = pType->GetInt(SYNHOMELEVDATA_MONEY);
// // 						break;
// // 					}
// // 				}
// // 					
// // 				int enReput = pSyn->GetInt(SYNDATA_REPUTE);
// // 				if(nAmount > enReput)
// // 				{
// // 					pUser->SendSysMsg("贡献度不够!");
// // 					return;
// // 				}
// // 				int data = max(0,enReput - nAmount);
// // 			// 	pSyn->SetRepute(data);	
// // 				pSyn->QueryModify()->SetData(SYNDATA_REPUTE, data, true);
// // 				
// // 				int enMoney = pUser->GetMoney();
// // 				if(nMoney > enMoney)
// // 				{
// // 					pUser->SendSysMsg("资金不足!");
// // 					return;
// // 				}
// // 				pUser->SpendMoney(nMoney,true);
// // 
// // //				pSyn->SetChgLev(level + 1);
// // //				pSyn->SynchroInfo();
// // 				pSyn->QueryModify()->SetData(SYNDATA_CHGLEV, level + 1, true);
// // 			}
// // 		}
// // 		break;
// // 	case SYN_SET_TEAMLEADER:
// // 		{
// // 			//  [1/15/2008 朱斌]	军团人事开关
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			// 检查权限
// // 			int nRank = pUser->GetSynRank();
// // 			if(nRank != RANK_LEADER || nRank != RANK_SUBLEADER)
// // 				return ;
// // 
// // 			// check amount, temporary code.
// // 			if(pUser->QuerySynAttr()->GetAssistantCount() >= MAX_ASSISTANTSIZE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11099));//STR_SYN_PLACE_FULL
// // 				return ;
// // 			}
// // 
// // 			// target syn
// // 			CUser* pTarget = UserManager()->GetUser(szName);
// // 			if(!pTarget || (!pTarget->GetSyndicate()->IsSubSubSyn()))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(12011));//STR_NOT_HERE
// // 				return ;
// // 			}
// // 			int nLevel = 30;
// // 			int nProffer = 1000;			
// // 
// // 			if(pTarget->GetLev() < nLevel && pTarget->QuerySynProffer() < nProffer )
// // 			{				
// // 				char szBuff[128];
// // 				sprintf(szBuff,g_objStrRes.GetStr(12014),pTarget->GetName());//STR_LEVEL_NOT_ENOUGH
// // 				pTarget->SendSysMsg(szBuff);
// // 				return ;
// // 			}
// // 
// // 			OBJID	idTargetSyn = pTarget->GetSynID();
// // 			if(idTargetSyn == ID_NONE || idTargetSyn == idSyn)
// // 				return ;
// // 			CSynPtr pSubSubSyn = SynManager()->QuerySyndicate(idTargetSyn);
// // 			CHECK(pSubSubSyn);
// // 
// // 			if(nRank == RANK_SUBLEADER)
// // 			{
// // 			}
// // 
// // 			pSubSubSyn->QueryModify()->Demise(pSubSubSyn->GetInt(SYNDATA_LEADERID),0, pTarget->GetID(), pTarget->GetName(), pTarget->QuerySynAttr()->GetSynMemberLevel(),pSubSubSyn->GetID());
// // 				
// // 			char szBuff[128];
// // 			sprintf(szBuff,g_objStrRes.GetStr(10050),szName,pTarget->GetSyndicate()->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBSUBLEADER_SUCC
// // 			pUser->SendSysMsg(szBuff);
// // 			sprintf(szBuff,g_objStrRes.GetStr(10051),pSyn->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBSUBLEADER_CON
// // 			pTarget->SendSysMsg(szBuff);
// // 			sprintf(szBuff,g_objStrRes.GetStr(10052),pTarget->GetName(),pTarget->GetSyndicate()->GetStr(SYNDATA_NAME));//STR_SYN_SETSUBSUBLEADER_PROMPT
// // 			pTarget->SendSysMsg(_TXTATR_SYNDICATE,szBuff);		
// // 		}
// // 		break;
// // /*
// // 	case SYN_CHANGE_SUBSUBNAME://改队名
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			int	  nRank = pUser->GetSynRankShow();
// // 			if(idSyn == ID_NONE || nRank != RANK_TEAMLEADER)
// // 				return ;
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 
// // 			if(!pSyn || strlen(pSyn->GetStr(SYNDATA_NAME)) <= 6 || !pSyn->IsSubSubSyn())
// // 				return ;
// // 
// // 			pSyn->SetNewSubName(szName);
// // 
// // 			CMsgSyndicate	msg;
// // 			if(msg.Create(SYN_CHANGE_SUBSUBNAME, pSyn->GetID(), szName))
// // 				pSyn->BroadcastSynMsg(&msg);
// // 			
// // 		}
// // 		break;
// // 	case SYN_CHANGE_SUBNAME://改分团名
// // 		{
// // 			OBJID idSyn = pUser->GetSynID();
// // 			int	  nRank = pUser->GetSynRankShow();
// // 			if(idSyn == ID_NONE || nRank != RANK_SUBLEADER)
// // 				return ;
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 
// // 			if(!pSyn || strlen(pSyn->GetStr(SYNDATA_NAME)) <= 6 || !pSyn->IsSubSyn())
// // 				return ;
// // 
// // 			pSyn->SetNewSubName(szName);
// // 
// // 			CMsgSyndicate	msg;
// // 			if(msg.Create(SYN_CHANGE_SUBNAME,  pSyn->GetID(), szName))
// // 				pSyn->BroadcastSynMsg(&msg);
// // 		}
// // 		break;
// // */
// // 
// // 	case SYNMEMBER_ASSIGN:			//帮派成员编制---
// // 		{
// // 			//  [1/15/2008 朱斌]	军团人事开关
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			int nRank = pUser->GetSynRank();
// // 			if(nRank!=RANK_LEADER || nRank!=RANK_SUBLEADER)
// // 			{
// // 				return;
// // 			}
// // 			CSyndicate * pSyn = pUser->GetSyndicate();
// // 			CHECK(pSyn);
// // 
// // 			OBJID idSynAssigned = m_pInfo->idTarget;
// // 			CSyndicate * pSynAssigned = SynManager()->QuerySyndicate(idSynAssigned);
// // 			CHECK(pSynAssigned);
// // 			
// // 			OBJID idUser = /*m_pInfo->idUser*/0;
// // 			CUser * pAssignUser = UserManager()->GetUser(idUser);
// // 			CHECK(pAssignUser);
// // 			
// // 			CSyndicate * pCurSyn = pAssignUser->GetSyndicate();
// // 			if(pCurSyn->GetID()==pSynAssigned->GetID())//不能为同一个帮派
// // 				return;
// // 			//---开始分配---begin			
// // 			if(pAssignUser->QuerySynAttr()->ChangeSyn(pCurSyn, pSynAssigned))
// // 			{
// // 				//成功后，回应消息
// // 				pCurSyn->BroadcastSynMsg(this);				
// // 				pAssignUser->QuerySynAttr()->SynchroInfo(true);				
// // 			}					
// // 			//---开始分配---end			
// // 			
// // 		}
// // 		break;
// //	case	APPLY_JOIN:					// 申请加入黑社会
// //		{
// //			//  [1/15/2008 朱斌]	军团人事开关
// //			if(!g_bEnableSynModificate)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// //				return;
// //			}
// //
// //			// 查参数
// //			if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// //				return;
// //
// //			CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
// //			if(!pTarget)
// //				return ;
// ////			if(pTarget->GetPhyle() != pUser->GetPhyle())
// ////				return ;
// //			// 检查帮派
// //			OBJID	idSyn	= pUser->GetSynID();
// //			int		nRank	= pUser->GetSynRank();
// //			OBJID	idTargetSyn = pTarget->GetSynID();
// //			int		nTargetRank	= pTarget->GetSynRank();
// //
// //			//检查 级别
// //			if(idSyn != ID_NONE || idTargetSyn == ID_NONE ||  nTargetRank < RANK_TEAMLEADER || nTargetRank == RANK_LEADERWIFE )
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(10033));//STR_SYN_APPLY_JOIN_FAILED
// //				return;
// //			}
// //
// //			
// //			// no money ?
// //			CSyndicate* pTargetSyn = SynManager()->QuerySyndicate(idTargetSyn);
// //			if (!pTargetSyn)
// //				return;
// //
// //			if (pTargetSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// //				return;
// //			}
// //
// //			// is full?
// //			//int MemberAmount[7]={0,10,15,20,25,30,35};
// ////军团级别	军团人数
// ////S级	120
// ////A级	100
// ////B级	80
// ////C级	60
// ////D级	40
// ////E级	20
// //
// //			//[游途道标 2008/9/27]帮派成员人数提高一倍
// //			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807修罗:策划案
// //			if(pTargetSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pTargetSyn->GetInt(SYNDATA_RANK)])
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// //				return;
// //			}
// //			
// //
// //			if(!pTarget->FetchApply(CUser::APPLY_INVITE_JOIN_SYN, idUser))
// //			{
// //				// 保存申请项
// //				pUser->SetApply(CUser::APPLY_JOIN_SYN, m_pInfo->idTarget);
// //				//20070403修罗:发送名字
// //				//IF_OK_(Create(APPLY_JOIN, idUser))
// //				IF_OK_(Create(APPLY_JOIN, idUser,pUser->GetName()))
// //					pTarget->SendMsg(this);
// //				return;
// //			}
// //
// //			nRank = RANK_NORMAL;
// ////			if( pUser->GetLev() < SYN_CONDITION_NEWBIE)
// ////				nRank = RANK_LEV1;
// //			if(pUser->Get_MateID() == pTargetSyn->GetLeaderID())
// //			{
// //				nRank = RANK_LEADERWIFE;
// //				pTargetSyn->SetLeaderWifeID(m_pInfo->idTarget);
// //			}
// //			
// //			pUser->QuerySynAttr()->JoinSyn(idTargetSyn, nRank);
// //			pUser->SendSysMsg(g_objStrRes.GetStr(11039), pTargetSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_JOIN_SYN_s
// //			MSGBUF szMsg;
// //			sprintf(szMsg,g_objStrRes.GetStr(11040) , pUser->GetName());//STR_SYN_NOTIFY_JOIN_s
// //			if(pUser->Get_MateID() == pTargetSyn->GetLeaderID())
// //			{
// //				sprintf(szMsg,"%s来到本帮，作为本帮的帮主夫人!", pUser->GetName());
// //			}
// //			pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// //		}
// //		break;
// // 	case INVITE_JOIN_NAME://[游途道标 2008/10/6]邀请玩家加入帮会
// // 		{
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// 检查帮派
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			int		nRank	= pUser->GetSynRank();
// // 			if(idSyn == ID_NONE || (nRank < RANK_TEAMLEADER || nRank == RANK_LEADERWIFE))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11037));
// // 				return;
// // 			}
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if (!pSyn)
// // 				return;
// // 			
// // 			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11011));
// // 				return;
// // 			}
// 			
// 			//int MemberAmount[7]={0,10,15,20,25,30,35};
// 			//军团级别	军团人数
// //S级	120
// //A级	100
// //B级	80
// //C级	60
// //D级	40
// //E级	20
// // 
// // 			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807修罗:策划案
// // 			if(pSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pSyn->GetInt(SYNDATA_RANK)])
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// // 				return;
// // 			}
// 
// // 			CUser *pUserTarget = UserManager()->GetUser(szName);
// // 			if (!pUserTarget)
// // 			{
// // 				pUser->SendSysMsg("该玩家与你不在同一地图，请稍后再试。");
// // 				return;
// // 			}
// // 			else
// // 			{
// // 			m_pInfo->idTarget = idSyn;
// // 			m_pInfo->idFealty = pUser->GetID();
// // 			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
// // 			}
// // 			m_pInfo->idTarget = pUserTarget->GetID();
// // 		}
// // 		break;
// 		//[游途道标 2008/10/6]往下继续执行INVITE_JOIN的老操作
// 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	INVITE_JOIN_RE:
// // 		{
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 			if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// // 				return;
// // 
// // 			CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
// // 			if(!pTarget)
// // 				return ;
// // 			
// // 			// 检查帮派
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			int		nRank	= pUser->GetSynRank();
// // 			OBJID	idTargetSyn = pTarget->GetSynID();
// // 			int		nTargetRank	= pTarget->GetSynRank();
// // 			if(idSyn == ID_NONE || (nRank < RANK_TEAMLEADER || nRank == RANK_LEADERWIFE) || idTargetSyn != ID_NONE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11037));//STR_JOIN_SYN_FAILED
// // 				return;
// // 			}
// // 
// // 			// no money ?
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if (!pSyn)
// // 				return;
// // 			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// // 				return;
// // 			}
// // 			//int MemberAmount[7]={0,10,15,20,25,30,35};
// // 			//军团级别	军团人数
// // //S级	120
// // //A级	100
// // //B级	80
// // //C级	60
// // //D级	40
// // //E级	20
// // 
// // 			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807修罗:策划案
// // 			if(pSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pSyn->GetInt(SYNDATA_RANK)])
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// // 				return;
// // 			}
// // 
// // 			if(pUser->FetchApply(CUser::APPLY_JOIN_SYN, m_pInfo->idTarget))
// // 			{
// // 				pUser->SetApply(0, 0);
// // 			}
// // 			else
// // 				return;
// // 			
// // 			nRank = RANK_NORMAL;
// // 			OBJID idMate = 0;
// // 			pTarget->QuerySynAttr()->GetMateIDByID(idMate, pTarget->GetID());
// // 			if(/*pTarget->Get_MateID()*/idMate == pSyn->GetLeaderID())
// // 			{
// // 				nRank = RANK_LEADERWIFE;
// // // 				pSyn->SetLeaderWifeID(m_pInfo->idTarget);
// // 				pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, m_pInfo->idTarget, true);
// // 			}
// // 			
// // 			pTarget->QuerySynAttr()->JoinSyn(idSyn, nRank);
// // 			pTarget->SendSysMsg(g_objStrRes.GetStr(11039), pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_JOIN_SYN_s
// // 			MSGBUF szMsg;
// // 			sprintf(szMsg,g_objStrRes.GetStr(11040), pTarget->GetName());//STR_SYN_NOTIFY_JOIN_s
// // 			if(idMate == pSyn->GetLeaderID())
// // 			{
// // 				sprintf(szMsg,"%s来到本帮，作为本帮的帮主配偶!", pTarget->GetName());
// // 			}
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 		}
// // 		break;
// // 	case	INVITE_JOIN:				// 邀请加入黑社会
// // 		{
// // 		//  [1/15/2008 朱斌]	军团人事开关
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// 查参数
// // 			if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// // 				return;
// // 
// // 			CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
// // 			if(!pTarget)
// // 				return ;
// // //			if(pTarget->GetPhyle() != pUser->GetPhyle())
// // //				return ;
// // 
// // 			// 检查帮派
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			int		nRank	= pUser->GetSynRank();
// // 			OBJID	idTargetSyn = pTarget->GetSynID();
// // 			int		nTargetRank	= pTarget->GetSynRank();
// // 			if(idTargetSyn == ID_NONE || (nTargetRank < RANK_TEAMLEADER || nTargetRank == RANK_LEADERWIFE) || idSyn != ID_NONE)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11037));//STR_JOIN_SYN_FAILED
// // 				return;
// // 			}
// // 
// // 			// no money ?
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idTargetSyn);
// // 			if (!pSyn)
// // 				return;
// // //			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // //			{
// // //				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// // //				return;
// // //			}
// // 			//int MemberAmount[7]={0,10,15,20,25,30,35};
// // 			//军团级别	军团人数
// // //S级	120
// // //A级	100
// // //B级	80
// // //C级	60
// // //D级	40
// // //E级	20
// // 
// // 			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807修罗:策划案
// // 			if(pSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pSyn->GetInt(SYNDATA_RANK)])
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// // 				return;
// // 			}
// // 
// // 			if(!pTarget->FetchApply(CUser::APPLY_JOIN_SYN, idUser))
// // 			{
// // 				// 保存申请项
// // 				pTarget->SetApply(CUser::APPLY_JOIN_SYN, idUser);
// // 				//20070403修罗:发送名字
// // 				//IF_OK_(Create(INVITE_JOIN, idUser))
// // 				IF_OK_(Create(/*INVITE_JOIN*/APPLY_JOIN, idUser,pUser->GetName()))
// // 					pTarget->SendMsg(this);
// // 				return;
// // 			}
// // 		}
// // 		break;
// // 	case	APPLY_JOIN:	
// // 		{
// // 			if(pUser->GetSynID() == 0)
// // 			{
// // 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
// // 			}
// 			//  [1/15/2008 朱斌]	军团人事开关
// //			if(!g_bEnableSynModificate)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// //				return;
// //			}
// //
// //			// 查参数
// //			if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// //				return;
// //
// //			CUserPtr pTarget = g_UserManager.GetUser(m_pInfo->idTarget);
// //			if(!pTarget)
// //				return ;
// ////			if(pTarget->GetPhyle() != pUser->GetPhyle())
// ////				return ;
// //
// //			// 检查帮派
// //			OBJID	idSyn	= pUser->GetSynID();
// //			int		nRank	= pUser->GetSynRank();
// //			OBJID	idTargetSyn = pTarget->GetSynID();
// //			int		nTargetRank	= pTarget->GetSynRank();
// //			if(idSyn == ID_NONE || (nRank < RANK_TEAMLEADER && nRank != RANK_LEADERWIFE) || idTargetSyn != ID_NONE)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11037));//STR_JOIN_SYN_FAILED
// //				return;
// //			}
// //
// //			// no money ?
// //			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// //			if (!pSyn)
// //				return;
// //			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// //				return;
// //			}
// //			//int MemberAmount[7]={0,10,15,20,25,30,35};
// //			//军团级别	军团人数
// ////S级	120
// ////A级	100
// ////B级	80
// ////C级	60
// ////D级	40
// ////E级	20
// //
// //			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807修罗:策划案
// //			if(pSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pSyn->GetInt(SYNDATA_RANK)])
// //			{
// //				pUser->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
// //				return;
// //			}
// //
// //			if(!pTarget->FetchApply(CUser::APPLY_JOIN_SYN, idUser))
// //			{
// //				// 保存申请项
// //				pUser->SetApply(CUser::APPLY_INVITE_JOIN_SYN, m_pInfo->idTarget);
// //				//20070403修罗:发送名字
// //				//IF_OK_(Create(INVITE_JOIN, idUser))
// //				IF_OK_(Create(INVITE_JOIN, idUser,pUser->GetName()))
// //					pTarget->SendMsg(this);
// //				return;
// //			}
// //
// //			//20070406修罗:策划需求.所有团员初进入军团
// //			//级别低于20级 是预备团员
// ////			nRank = RANK_LEV2;
// ////			if( pTarget->GetLev() < SYN_CONDITION_NEWBIE)
// ////				nRank = RANK_LEV1;
// //			nRank = RANK_NORMAL;
// //			if(pTarget->Get_MateID() == pSyn->GetLeaderID())
// //			{
// //				nRank = RANK_LEADERWIFE;
// //				pSyn->SetLeaderWifeID(m_pInfo->idTarget);
// //			}
// //			
// //			pTarget->QuerySynAttr()->JoinSyn(idSyn, nRank);
// //			pTarget->SendSysMsg(g_objStrRes.GetStr(11039), pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_JOIN_SYN_s
// //			MSGBUF szMsg;
// //			sprintf(szMsg,g_objStrRes.GetStr(11040), pTarget->GetName());//STR_SYN_NOTIFY_JOIN_s
// //			if(pTarget->Get_MateID() == pSyn->GetLeaderID())
// //			{
// //				sprintf(szMsg,"%s来到本帮，作为本帮的帮主夫人!", pTarget->GetName());
// //			}
// //			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 		}
// // 		break;
// 
// 		// the operation of members
// 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	LEAVE_SYN:				// 脱离黑社会
// // 		{
// // 			//  [1/15/2008 朱斌]	军团人事开关
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// 检查帮派
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return;
// // 			
// // 			// 检查帮主
// // 			int	nRank = pUser->GetSynRank();
// // 			if(nRank == RANK_LEADER)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(12008));//STR_NO_DISBAND
// // 				return;
// // 			}
// // 
// // 			int reData = pUser->QuerySynAttr()->SetLeaveSyn(1,pUser->GetID());
// // 			if(KICKOUTDATA_OUTDO_TRUE == reData)
// // 			{
// // 				pUser->SendSysMsg("申请成功，1天后登录时完成退出操作！");
// // 			}
// // 			else if(KICKOUTDATA_OUTCHECK_BYKICK == reData)
// // 			{
// // 				pUser->SendSysMsg("你已经被踢出帮会，请耐心等待。（申请成功1天后，登录时完成退出操作）");
// // 			}
// // 			else if(KICKOUTDATA_OUTCHECK_BYUSER == reData)
// // 			{
// // 				pUser->SendSysMsg("你已经申请退出帮会了，请耐心等待。（申请成功1天后，登录时完成退出操作）");
// // 			}
// // 			
// // 			return;
// // 			//pUser->QuerySynAttr()->GetLeaveSyn(pUser->GetID());
// // 		
// // 			
// // 			
// // 			pUser->QuerySynAttr()->LeaveSyn();
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if (pSyn)
// // 			{
// // 				if(RANK_SUBLEADER == nRank)
// // 				{
// // 		// 			pSyn->SetSubLeaderID(0);
// // 					pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 				}
// // 				else if(RANK_LEADERWIFE == nRank)	
// // 				{
// // 	// 				pSyn->SetLeaderWifeID(0);
// // 					pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, 0, true);
// // 				}
// // 				
// // 				char szMsg[256];
// // 				sprintf(szMsg,g_objStrRes.GetStr(10035) , pUser->GetName());//STR_SYN_LEAVE_s
// // 			
// // 				CMsgTalk	msg;
// // 				IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xFFFFFF, _TXTATR_SYNDICATE))
// // 					pSyn->BroadcastSynMsg(&msg);
// // 				m_StrPacker.AddString(pUser->GetName());
// // 				m_unMsgSize = sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// // 				pSyn->BroadcastSynMsg(this);				
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(14095));//"您已经退出了军团"
// // 
// // 			}
// // 			
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case KICKOUT_MEMBER_NOAGREE:
// // 		{
// // 			//  [1/15/2008 朱斌]	军团人事开关
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			char	szSQL[1024];
// // 			sprintf(szSQL, "SELECT id FROM sk_synattr WHERE syn_id = %d and rank = 90",idSyn);	
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				CUser* pTargetUser = UserManager()->GetUser(pRes->GetInt(0));
// // 				if(pTargetUser)
// // 				{
// // 					pTargetUser->SendSysMsg(g_objStrRes.GetStr(15087));//"团长拒绝了您的申请"
// // 				}
// // 			}
// // 
// // 			//////////////////////////////////////////////////////////////////////////
// // 		/*	char	szSQL[1024];
// // 			sprintf(szSQL, "SELECT * FROM %s WHERE words LIKE 'ApplyDismiss%'", _TBL_LEAVEWORD);	
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				for(int i = 0; i < pRes->RecordCount(); i++)
// // 				{
// // 					char	szRecvUser[_MAX_NAMESIZE];
// // 					SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
// // 					
// // 					CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
// // 					if(pUser && !pUser->IsMoreLeaveWord())
// // 					{
// // 						char	szTime[_MAX_NAMESIZE];
// // 						char	szWords[_MAX_WORDSSIZE];					
// // 						SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
// // 						SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);						
// // 						//---jinggy---2004-12-20---判断是否是 申请开除会员的留言---begin
// // 						char szBuffer[255]=""; //开除成员的标志
// // 						OBJID idSyn; //要开除的成员的帮派ID
// // 						char  szNameDismissed[16]; //开除的成员ID		
// // 						char  szLeaderName[16];
// // 						
// // 						if(4!=sscanf(szWords,"%s %d %s %s",szBuffer,&idSyn,szLeaderName,szNameDismissed))
// // 							continue;
// // 						if(strcmp(szBuffer,"ApplyDismiss")==0 && idSyn>0 ) //是申请开除留言
// // 						{
// // 							CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 							CHECK(pSyn);
// // 							DWORD dwMainSynID = pSyn->GetMasterSynID();
// // 							
// // 							char szCurLeaderName[16];
// // 							sprintf(szCurLeaderName,"%s",pSyn->GetStr(SYNDATA_LEADERNAME));
// // 							
// // 							if(strcmp(szNameDismissed,szName)==0) //有效期内此人还是领导则发送请求
// // 							{
// // 								//删除记录
// // 								OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
// // 								sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
// // 								g_db.ExecuteSQL(szSQL);								
// // 								break;								
// // 							}						
// // 						}						
// // 					}
// // 				}
// // 				pRes->MoveNext();
// // 			}
// // 			pRes->Release();*/
// // 		
// // 		}
// // 		break;
// // 
// // 	case KICKOUT_MEMBER_INFO_QUERY: //帮主查列表
// // 		{
// // 			//  [1/15/2008 朱斌]	军团人事开关
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			if(pUser->GetSynRank() != RANK_LEADER)  
// // 				return;
// // 
// // 			char	szSQL[1024];
// // 			sprintf(szSQL, "SELECT * FROM %s WHERE words LIKE 'ApplyDismiss%'", _TBL_LEAVEWORD);	
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				for(int i = 0; i < pRes->RecordCount(); i++)
// // 				{
// // 					char	szRecvUser[_MAX_NAMESIZE];
// // 					SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
// // 					
// // 					CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
// // 					if(pUser && !pUser->IsMoreLeaveWord())
// // 					{
// // 					//	char	szSender[_MAX_NAMESIZE];
// // 						char	szTime[_MAX_NAMESIZE];
// // 						char	szWords[_MAX_WORDSSIZE];
// // 					//	SafeCopy(szSender, pRes->GetStr(LEAVEWORDDATA_SENDER), _MAX_NAMESIZE);
// // 						SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
// // 						SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);
// // 						
// // 						
// // 						//---jinggy---2004-12-20---判断是否是 申请开除会员的留言---begin
// // 						char szBuffer[255]=""; //开除成员的标志
// // 						OBJID idSyn; //要开除的成员的帮派ID
// // 						char  szNameDismissed[16]; //开除的成员ID		
// // 						char  szLeaderName[16];
// // 						
// // 						if(4!=sscanf(szWords,"%s %d %s %s",szBuffer,&idSyn,szLeaderName,szNameDismissed))
// // 							continue;
// // 						
// // 						if(strcmp(szBuffer,"ApplyDismiss")==0 && idSyn>0 ) //是申请开除留言
// // 						{
// // 							time_t	tDelta = time(NULL);
// // 							tDelta -= 7 * 86400;  //超过7天，帮主没有相应就表示同意开除
// // 							char	szDelta[_MAX_NAMESIZE];
// // 							
// // 							tm*	pTm		= localtime(&tDelta);
// // 							if(pTm)
// // 							{
// // 								sprintf(szDelta, "%04d%02d%02d%02d%02d%02d", 
// // 									pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);								
// // 							}
// // 							
// // 							if(strcmp(szTime,szDelta) < 0)
// // 							{
// // 								//响应过期											
// // 								CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // //								if(pSyn && pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // //								{
// // //									pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// // //									return;
// // //								}
// // 								pUser->QuerySynAttr()->KickoutMember(szNameDismissed);						
// // 								
// // 								MSGBUF	szMsg;//STR_KICKOUT_MEMBER_ss
// // 								sprintf(szMsg,g_objStrRes.GetStr(11041), szNameDismissed, pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));
// // 								if(pSyn)
// // 									pSyn->BroadcastSubSynMsg(pUser->GetSocketID(), 0, szMsg);
// // 								
// // 								
// // 								//删除记录
// // 								OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
// // 								sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
// // 								g_db.ExecuteSQL(szSQL);
// // 							}
// // 							else  
// // 							{
// // 									
// // 								CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 								DWORD dwMainSynID = pSyn->GetMasterSynID();
// // 								
// // 								char szCurLeaderName[16];
// // 								sprintf(szCurLeaderName,"%s",pSyn->GetStr(SYNDATA_LEADERNAME));
// // 
// // 								if(strcmp(szCurLeaderName,szLeaderName)==0) //有效期内此人还是领导则发送请求
// // 								{
// // 									//向帮主发送申请开除的成员的信息									
// // //									if(Create(KICKOUT_MEMBER_INFO_QUERY, szNameDismissed))
// // //										pUser->SendMsg(this);								
// // 								}
// // 								else //在有效期内若是换了帮主，则取消开除
// // 								{
// // 									//删除记录
// // 									OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
// // 									sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
// // 									g_db.ExecuteSQL(szSQL);
// // 								}
// // 							}							
// // 						}
// // 					}
// // 					pRes->MoveNext();
// // 				}
// // 				pRes->Release();
// // 			}
// // 		}
// // 		break;
// // 	case KICKOUT_MEMBER_AGREE:  //帮主同意申请的开除会员
// // 
// // 		{
// // 		/*	// 检查帮派
// // 			OBJID idSyn = pUser->GetSynID();
// // 			int nRank = pUser->GetSynRank();
// // 			
// // 			if(idSyn == ID_NONE || nRank != RANK_LEADER )
// // 			{
// // 				pUser->SendSysMsg(STR_NOT_AUTHORIZED);
// // 				return;
// // 			}
// // 
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 
// // 			// no money ?
// // 			if(pSyn && pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // 			{
// // 				pUser->SendSysMsg(STR_TOO_LOWER_SYN_MONEY);
// // 				return;
// // 			}
// // 
// // 			//pTarget->QuerySynAttr()->LeaveSyn();		// 改成立刻踢出
// // 			pUser->QuerySynAttr()->KickoutMember(szName);
// // 
// // 			MSGBUF	szMsg;
// // 			sprintf(szMsg, STR_KICKOUT_MEMBER_ss, LPCTSTR(szName), pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));
// // 			IF_OK(pSyn)
// // 				pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);		
// // 			
// // 			//////////////////////////////////////////////////////////////////////////
// // 			char	szSQL[1024];
// // 			sprintf(szSQL, "SELECT * FROM %s WHERE words LIKE 'ApplyDismiss%'", _TBL_LEAVEWORD);	
// // 			IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// // 			if(pRes)
// // 			{
// // 				for(int i = 0; i < pRes->RecordCount(); i++)
// // 				{
// // 					char	szRecvUser[_MAX_NAMESIZE];
// // 					SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
// // 					
// // 					CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
// // 					if(pUser && !pUser->IsMoreLeaveWord())
// // 					{
// // 						char	szTime[_MAX_NAMESIZE];
// // 						char	szWords[_MAX_WORDSSIZE];					
// // 						SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
// // 						SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);						
// // 						//---jinggy---2004-12-20---判断是否是 申请开除会员的留言---begin
// // 						char szBuffer[255]=""; //开除成员的标志
// // 						OBJID idSyn; //要开除的成员的帮派ID
// // 						char  szNameDismissed[16]; //开除的成员ID		
// // 						char  szLeaderName[16];
// // 						
// // 						if(4!=sscanf(szWords,"%s %d %s %s",szBuffer,&idSyn,szLeaderName,szNameDismissed))
// // 							continue;
// // 						if(strcmp(szBuffer,"ApplyDismiss")==0 && idSyn>0 ) //是申请开除留言
// // 						{
// // 							CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 							CHECK(pSyn);
// // 							DWORD dwMainSynID = pSyn->GetMasterSynID();
// // 							
// // 							char szCurLeaderName[16];
// // 							sprintf(szCurLeaderName,"%s",pSyn->GetStr(SYNDATA_LEADERNAME));
// // 							
// // 							if(strcmp(szNameDismissed,szName)==0) //有效期内此人还是领导则发送请求
// // 							{
// // 								//删除记录
// // 								OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
// // 								sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
// // 								g_db.ExecuteSQL(szSQL);								
// // 								break;								
// // 							}						
// // 						}						
// // 					}
// // 				}
// // 				pRes->MoveNext();
// // 			}
// // 			pRes->Release();*/
// // 		}
// // 		break;
// // 		
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	KICKOUT_MEMBER:				// 开除黑社会成员
// // 		{
// // 			//  [1/15/2008 朱斌]	军团人事开关
// // 			if(!g_bEnableSynModificate)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(19029));
// // 				return;
// // 			}
// // 
// // 			// 检查帮派
// // 			OBJID idSyn = pUser->GetSynID();
// // 			int nRank = pUser->GetSynRank();
// // 			if(idSyn == ID_NONE || nRank < 90)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11097));//STR_NOT_AUTHORIZED
// // 				return;
// // 			}
// // 			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			CHECK(pSyn);
// // 			
// // 			// no money ?
// // //			if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
// // //			{
// // //				pUser->SendSysMsg(g_objStrRes.GetStr(11011));//STR_TOO_LOWER_SYN_MONEY
// // //				return;
// // //			}
// // 			if( nRank == 100 || nRank == 90)//团长和副团长直接开除
// // 			{				
// // 				// 开
// // 				CUser* pTargetUser = UserManager()->GetUser(szName);
// // 				if (pTargetUser)
// // 				{
// // 					if(pTargetUser->GetSynRank() == 80)
// // 					{
// // 						pUser->SendSysMsg("不能对帮主配偶进行操作!");//STR_TOO_LOWER_SYN_MONEY
// // 						return;
// // 					}
// // 					if(pTargetUser->GetSynRank() == 90)
// // 					{
// // 				// 		pSyn->SetSubLeaderID(0);
// // 						pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 					}
// // 					//pTargetUser->SendSysMsg(g_objStrRes.GetStr(11041), pTargetUser->GetName(), pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_KICKOUT_MEMBER_ss
// // 					if (pTargetUser->GetSynID() == idSyn)
// // 					{
// // 						OBJID idData = pUser->QuerySynAttr()->GetUserIDByName(szName);
// // 						int reData = pUser->QuerySynAttr()->SetLeaveSyn(2,idData);
// // 						if(KICKOUTDATA_OUTDO_TRUE == reData)
// // 						{
// // 							pUser->SendSysMsg("申请成功，1天后登录时完成踢出操作！");
// // 							pTargetUser->SendSysMsg("帮会领导申请踢你出帮会，，1天后登录时完成踢出操作！");
// // 						}
// // 						else if(KICKOUTDATA_OUTCHECK_BYKICK == reData)
// // 						{
// // 							pUser->SendSysMsg("他已经被踢出帮会，请耐心等待。（申请成功1天后，登录时完成退出操作）");
// // 						}
// // 						else if(KICKOUTDATA_OUTCHECK_BYUSER == reData)
// // 						{
// // 							pUser->SendSysMsg("他已经申请退出帮会了，请耐心等待。（申请成功1天后，登录时完成退出操作）");
// // 						}
// // 					}					
// // 					//	pTargetUser->QuerySynAttr()->LeaveSyn();
// // 				}
// // 				else
// // 				{
// // 					if(pUser->QuerySynAttr()->GetPlayRankByName(szName) == 80)
// // 					{
// // 						pUser->SendSysMsg("不能对帮主配偶进行操作!");//STR_TOO_LOWER_SYN_MONEY
// // 					}
// // 					if(pUser->QuerySynAttr()->GetPlayRankByName(szName) == 90)
// // 					{
// // // 						pSyn->SetSubLeaderID(0);
// // 						pSyn->QueryModify()->SetData(SYNDATA_SUBLEADER, 0, true);
// // 					}
// // 
// // 					OBJID idData = pUser->QuerySynAttr()->GetUserIDByName(szName);
// // 					int reData = pUser->QuerySynAttr()->SetLeaveSyn(2,idData);
// // 					if(KICKOUTDATA_OUTDO_TRUE == reData)
// // 					{
// // 						pUser->SendSysMsg("申请成功，1天后登录时完成踢出操作！");
// // 					}
// // 					else if(KICKOUTDATA_OUTCHECK_BYKICK == reData)
// // 					{
// // 						pUser->SendSysMsg("他已经被踢出帮会，请耐心等待。（申请成功1天后，登录时完成退出操作）");
// // 					}
// // 					else if(KICKOUTDATA_OUTCHECK_BYUSER == reData)
// // 					{
// // 						pUser->SendSysMsg("他已经申请退出帮会了，请耐心等待。（申请成功1天后，登录时完成退出操作）");
// // 					}
// // 					pUser->QuerySynAttr()->KickoutMember(szName);
// // 				}
// // 				
// // // 				MSGBUF	szMsg;
// // // 				//			// 通知对方
// // // 				//			sprintf(szMsg, STR_KICKOUT_ME_s, pUser->GetName());
// // // 				//			CMsgTalk msg;
// // // 				//			if (msg.Create(SYSTEM_NAME, szName, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
// // // 				//				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, szName);
// // // 				
// // // 				sprintf(szMsg, g_objStrRes.GetStr(11041), szName, pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_KICKOUT_MEMBER_ss
// // // 				IF_OK(pSyn)
// // // 					pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // // 				//m_StrPacker.AddString(pTargetUser->GetName());
// // // 				m_StrPacker.AddString(szName);//20061027修罗,队员不在线时可能非法访问
// // // 				m_unMsgSize = sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// // // 				pSyn->BroadcastSynMsg(this);
// // 			}
// // //			else	//副团长向团长申请
// // //			{
// // //				CUser *pLeader = UserManager()->GetUser(pSyn->GetLeaderID());
// // //				if(pLeader == NULL)
// // //				{
// // //					pUser->SendSysMsg(g_objStrRes.GetStr(15088));//"对不起，团长不在线，此次操作无法执行。"
// // //				}
// // //				else
// // //				{
// // //					if(Create(KICKOUT_MEMBER_INFO_QUERY,szName))
// // //						pLeader->SendMsg(this);
// // //				}
// // //			}
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	QUERY_SYN_NAME:
// // 		{
// // 			OBJID idSyn	= m_pInfo->idTarget;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 			{
// // //				LOGWARNING("玩家[%s]未找到帮派名[%d]!", pUser->GetName(), idSyn);
// // 				return;
// // 			}
// // 
// // 			CSynPtr pMasterSyn = pSyn->GetMasterSyn();
// // 			CHECK(pMasterSyn);
// // 
// // 			CMsgName	msg;
// // 			IF_OK(msg.Create(NAMEACT_SYNDICATE, pMasterSyn->GetStr(SYNDATA_NAME), idSyn))
// // 			{
// // 				if(pSyn->GetID() != pMasterSyn->GetID())
// // 				{
// // 					ASSERT(msg.AppendStr(pSyn->GetStr(SYNDATA_NAME)));
// // 				}
// // 				pUser->SendMsg(&msg);
// // 			}
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	DONATE_MONEY:
// // 		{
// // 			if(pUser->GetSynID() == ID_NONE)
// // 				return ;
// // 
// // 			int nMoney = m_pInfo->idTarget;
// // 
// // 		//	pUser->GiveSynVAS(nMoney);
// // 			//pUser->QuerySynAttr()->DonateMoney(nMoney);
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	QUERY_SYNATTR:
// // 		{
// // 			pUser->QuerySynAttr()->SendInfoToClient();
// // 		}
// // 		break;
// // 	////////////////////////////////////////////////////////////////////////////////////////////////
// // 	case	SYN_WAR:
// // 		{
// // 			if(m_pInfo->szBuf == NULL )
// // 				return;
// // 
// // 			CSynPtr pTargetSyn = SynManager()->QuerySynByName(szName);			
// // 			if( !pTargetSyn )			
// // 			{
// // 				pUser->SendSysMsg("帮会名称错误!");
// // 				return;
// // 			}
// // 
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			OBJID	idTargetSyn = pTargetSyn->GetID();
// // 		
// // 			CSynPtr pThisSyn = SynManager()->QuerySyndicate(idSyn);
// // 
// // 			if(pThisSyn->GetState() != 0)
// // 			{
// // 				pUser->SendSysMsg("我帮会还在约战中!");
// // 				return;
// // 			}
// // 			if(pTargetSyn->GetState() != 0)
// // 			{
// // 				pUser->SendSysMsg("对方还在约战中!");
// // 				return;
// // 			}
// // 			if(pTargetSyn->GetInt(SYNDATA_RANK) + 2 <= pThisSyn->GetInt(SYNDATA_RANK))
// // 			{
// // 				pUser->SendSysMsg("不许欺负肉小帮会!");
// // 				return;
// // 			}
// // 
// 
// // 			
// // 			bool isOK = false;
// // 			OBJID	idTemp =	pTargetSyn->GetLeaderID();
// // 			CUserPtr pTargetUser = UserManager()->GetUser(idTemp);
// // 			if(pTargetUser)
// // 			{
// // 				isOK = true;
// // 				m_pInfo->idTarget = idTemp;
// // 			}
// // 			if(!isOK)
// // 			{
// // 				idTemp	=	pTargetSyn->GetSubLeaderID();
// // 				pTargetUser = UserManager()->GetUser(idTemp);
// // 				if(pTargetUser)
// // 				{
// // 					isOK = true;
// // 					m_pInfo->idTarget = idTemp;
// // 				}
// // 			}
// // 			if(!isOK)
// // 			{
// // 				idTemp	=	pTargetSyn->GetLeaderWifeID();
// // 				pTargetUser = UserManager()->GetUser(idTemp);
// // 				if(pTargetUser)
// // 				{
// // 					isOK = true;
// // 					m_pInfo->idTarget = idTemp;
// // 				}
// // 			}
// // 			
// // 			if(!pTargetUser || !isOK)
// // 			{			
// // 				pUser->SendSysMsg(/*g_objStrRes.GetStr(15089)*/"对方帮会领导者不在线或不在本地图!");//"对方军团长不在线"
// // 				return;
// // 			}
// // 
// // 			// 检查帮派
// // 			int		nRank	= pUser->GetSynRankShow();
// // 			int		nTargetRank	= pTargetUser->GetSynRankShow();
// // 
// // 			if(idSyn == ID_NONE || (nRank != RANK_LEADER && nRank != RANK_SUBLEADER && nRank != RANK_LEADERWIFE)
// // 				|| idTargetSyn == ID_NONE || (nTargetRank != RANK_LEADER && nTargetRank != RANK_SUBLEADER && nRank != RANK_LEADERWIFE))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11014));//STR_APPLY_ALLY_FAILED
// // 				return;
// // 			}
// // 
// // 			if(!pTargetUser->FetchApply(CUser::APPLY_SYNWAR, idUser))
// // 			{
// // 				// 保存申请项
// // 				pUser->SetApply(CUser::APPLY_SYNWAR, m_pInfo->idTarget);
// // 				char tempName[_MAX_NAMESIZE] = "";
// // 				
// // 				strcpy(tempName,pThisSyn->GetStr(SYNDATA_NAME));
// // 				IF_OK_(Create(SYN_WAR,/*szName*/tempName))
// // 					pTargetUser->SendMsg(this);
// // 				return;
// // 			}
// // 
// // 			
// //   			CSyndicate* pSyn = pUser->GetSyndicate();
// // 			CHECK(pSyn);	
// // 			CHECK(pTargetSyn);			
// // 
// // 
// // 			char sendTemp[128];
// // 			sprintf(sendTemp,"%d %d",pSyn->GetID(),0);
// // 			CMsgSyndicate msg;
// // 			msg.Create(SYN_WAR_SEND,pTargetSyn->GetID(),sendTemp);
// // 			CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST,&msg);
// // 
// // // 			pSyn->SetSynWar(idTargetSyn);
// // // 			pTargetSyn->SetSynWar(idSyn);
// // // 			
// // // 			MSGBUF szMsg;
// // // 			sprintf(szMsg, "%s帮会和%s帮会开始约战!", pSyn->GetStr(SYNDATA_NAME), pTargetSyn->GetStr(SYNDATA_NAME));
// // // 			UserManager()->BroadcastMsg(szMsg, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);		
// // 						
// // 		}
// // 		break;
// // 	case	_APPLY_ALLY:	// 请求添加结盟帮派
// // 		{
// // 			// 查参数
// // 		//	if(m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == idUser)
// // //				return;
// // 			if(m_pInfo->szBuf == NULL )
// // 				return;
// // 
// // 			CSynPtr pTargetSyn = SynManager()->QuerySynByName(szName);			
// // 			if( !pTargetSyn )
// // 			{
// // 				pUser->SendSysMsg("帮会名称错误!");
// // 				return;
// // 			}
// // 			
// // 			OBJID	idSyn	= pUser->GetSynID();
// // 			OBJID	idTargetSyn = pTargetSyn->GetID();
// // 			if(idSyn == idTargetSyn)
// // 				return;
// // 			
// // 			bool isOK = false;
// // 			OBJID	idTemp =	pTargetSyn->GetLeaderID();
// // 			CUserPtr pTargetUser = UserManager()->GetUser(idTemp);
// // 			if(pTargetUser)
// // 			{
// // 				isOK = true;
// // 				m_pInfo->idTarget = idTemp;
// // 			}
// // 			if(!isOK)
// // 			{
// // 				idTemp	=	pTargetSyn->GetSubLeaderID();
// // 				pTargetUser = UserManager()->GetUser(idTemp);
// // 				if(pTargetUser)
// // 				{
// // 					isOK = true;
// // 					m_pInfo->idTarget = idTemp;
// // 				}
// // 			}
// // 			if(!isOK)
// // 			{
// // 				idTemp	=	pTargetSyn->GetLeaderWifeID();
// // 				pTargetUser = UserManager()->GetUser(idTemp);
// // 				if(pTargetUser)
// // 				{
// // 					isOK = true;
// // 					m_pInfo->idTarget = idTemp;
// // 				}
// // 			}
// // 			
// // 			if(!pTargetUser || !isOK)
// // 			{			
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(15089));//"对方军团长不在线"
// // 				return;
// // 			}
// // 
// // 			// 检查帮派
// // 			int		nRank	= pUser->GetSynRankShow();
// // 			int		nTargetRank	= pTargetUser->GetSynRankShow();
// // 
// // 			if(idSyn == ID_NONE || (nRank != RANK_LEADER && nRank != RANK_SUBLEADER && nRank != RANK_LEADERWIFE)
// // 				|| idTargetSyn == ID_NONE || (nTargetRank != RANK_LEADER && nTargetRank != RANK_SUBLEADER && nRank != RANK_LEADERWIFE))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11014));//STR_APPLY_ALLY_FAILED
// // 				return;
// // 			}
// // 
// // 			if(!pTargetUser->FetchApply(CUser::APPLY_ALLY_SYN, idUser))
// // 			{
// // 				// 保存申请项
// // 				pUser->SetApply(CUser::APPLY_ALLY_SYN, m_pInfo->idTarget);
// // 				char tempName[_MAX_NAMESIZE] = "";
// // 				CSynPtr pThisSyn = SynManager()->QuerySyndicate(idSyn);
// // 				strcpy(tempName,pThisSyn->GetStr(SYNDATA_NAME));
// // 				IF_OK_(Create(_APPLY_ALLY,/*szName*/tempName))
// // 					pTargetUser->SendMsg(this);
// // 				return;
// // 			}
// // 
// // 			// set ally
// //   			CSyndicate* pSyn = pUser->GetSyndicate();
// // 			CHECK(pSyn);
// // //			CSyndicate* pTargetSyn = pTargetUser->GetSyndicate();
// // 			CHECK(pTargetSyn);
// // 
// // 			// 先解除敌对关系
// // 			pSyn->ClearEnemySyn(pTargetSyn);
// // 			pTargetSyn->ClearEnemySyn(pSyn);
// // 
// // 			int nIdx = -1;
// 
// // 			if(nIdx == -1)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(11098));//STR_ALLY_FULL
// // 				return ;
// // 			}
// // 			int nIdx2 = -1;
// //
// // 			if(nIdx2 == -1)
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(12010));//STR_HIS_ALLY_FULL
// // 				return ;
// // 			}
// 
// // 			MSGBUF szMsg;//STR_SYN_ALLY_s
// // 			sprintf(szMsg, g_objStrRes.GetStr(11043), pSyn->GetStr(SYNDATA_NAME), pTargetSyn->GetStr(SYNDATA_NAME));
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 			pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 
// // 			
// // 			// synchro npc server
// // 			CMsgSyndicate	msg;
// // 			IF_OK(msg.Create(SET_ALLY, pTargetSyn->GetID()))
// // 			MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(pSyn->GetID(), &msg);			
// // 		}
// // 		break;
// // 	case _CLEAR_ALLY: //客户端向服务器 解除结盟
// // 		{
// // 			// 检查帮派
// // 			OBJID idSyn = pUser->QuerySynAttr()->GetSynID();
// // 			int	nRankShow	= pUser->QuerySynAttr()->GetSynRankShow();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			if(nRankShow != RANK_LEADER && nRankShow != RANK_LEADERWIFE && nRankShow != RANK_SUBLEADER)
// // 			{
// // 				pUser->SendSysMsg("你权力不够!!");
// // 			}
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			CHECK(pSyn);
// // 
// // 			CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szName);
// // 			if (!pTargetSyn)
// // 				return ;
// // 
// // 			pSyn->ClearAllySyn(pTargetSyn);
// // 
// // 			MSGBUF szMsg;
// // 			sprintf(szMsg,g_objStrRes.GetStr(11044) , szName);//STR_SYN_CLEAR_ALLY1_s
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 
// // 			sprintf(szMsg, g_objStrRes.GetStr(11045), pSyn->GetStr(SYNDATA_NAME));//STR_SYN_CLEAR_ALLY2_s
// // 			pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 		}	
// // 		break;		
// // 	case _SET_ANTAGONIZE:	// 添加敌对军团
// // 		{
// // 			// 检查帮派
// // 			OBJID idSyn = pUser->QuerySynAttr()->GetSynID();
// // 			int nRankShow = pUser->QuerySynAttr()->GetSynRankShow();
// // 			if (idSyn == ID_NONE || (nRankShow != RANK_LEADER && nRankShow != RANK_SUBLEADER && nRankShow != RANK_LEADERWIFE))
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			CHECK(pSyn);
// // 
// // 			// 检查帮派
// // 			CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szName);
// // 			if (!pTargetSyn)
// // 			{
// // 				pUser->SendSysMsg("帮会名字错误!");
// // 				return;
// // 			}
// // 
// // 			// 先解除同盟关系
// // 			pSyn->ClearAllySyn(pTargetSyn);
// // 			pTargetSyn->ClearAllySyn(pSyn);
// // 
// // 			if((!pTargetSyn->IsMasterSyn()) || pTargetSyn->GetID() == idSyn)
// // 				return ;
// // 			
// 	
// // 			CMsgSyndicate	msg;
// // 			IF_OK(msg.Create(SET_ANTAGONIZE, pTargetSyn->GetID()))
// // 				MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(pSyn->GetID(), &msg);
// // 		}
// // 		break;
// // 	case _CLEAR_ANTAGONIZE:	// 删除敌对军团
// // 		{
// // 			// 检查帮派
// // 			OBJID idSyn = pUser->QuerySynAttr()->GetSynID();
// // 			int nRankShow = pUser->QuerySynAttr()->GetSynRankShow();
// // 			if (idSyn == ID_NONE || (nRankShow != RANK_LEADER && nRankShow != RANK_SUBLEADER && nRankShow != RANK_LEADERWIFE ))
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			CHECK(pSyn);
// // 
// // 			CSyndicate* pTargetSyn = SynManager()->QuerySynByName(szName);
// // 			if (!pTargetSyn)
// // 				return;
// // 
// // 			pSyn->ClearEnemySyn(pTargetSyn);
// // 
// // 			MSGBUF szMsg;
// // 			sprintf(szMsg, g_objStrRes.GetStr(11047), pSyn->GetStr(SYNDATA_NAME));//STR_SYN_CLEAR_ENEMY_s
// // 			pTargetSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 			sprintf(szMsg, "本帮会已与%s帮会和解!", pTargetSyn->GetStr(SYNDATA_NAME));//STR_SYN_CLEAR_ENEMY_s
// // 			pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), szMsg);
// // 		}	
// // 		break;
// // 	case SYN_PAYOFF://20070413修罗:发工资
// // 		{
// // 			// 检查帮派
// // 			OBJID idSyn = pUser->GetSynID();
// // 			if(idSyn == ID_NONE)
// // 				return ;
// // 			CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
// // 			if(!pSyn)
// // 				return ;
// // 			// 检查权限
// // 			if(pUser->GetSynRank() != RANK_LEADER)
// // 				return ;
// // 
// // 
// // 			if(!pSyn->ReqPayOff(m_pInfo->idTarget))
// // 			{
// // 				pUser->SendSysMsg(g_objStrRes.GetStr(15090));//"错误:发工资失败"
// // 			}
// // 		}
// // 		break;
// // 	default:
// // 		ASSERT(!"switch MsgSyndicate");
// // 		return;
// // 	}
// // 
// // 	}
// // 	catch(...)
// // 	{	
// // 		::LogSave("MsgSyndicate error: %d,%d,%d",m_pInfo->usAction,m_pInfo->idTarget,m_pInfo->dwData);//20061025修罗.测试用.自己手动来解开来看
// // 	}
// //}

//--------------------------------------------------
//20070403修罗:用来发送目标名字
BOOL CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget, const char *szTargetName)
{
	// check param
	if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
	{
//		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}

	// init
	this->Init();

	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;

	m_StrPacker.AddString( szTargetName );
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	m_unMsgType	=_MSG_SYNDICATE;


	return true;
}
