// MsgBetOutside.cpp: implementation of the CMsgAtk class.
// 仙剑修，2002.1.8
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "basefunc.h"
#include "MapGroup.h"
#include "SynManager.h"

int levMemberAmount[] = {0,50,55,60,65,70,75,80,85,90,95};//帮会人数应从0级0人数开始--jay
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
		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}

	// init
	this->Init();

	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;
	m_pInfo->dwData	=idFealty;
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
void	CMsgSyndicate::Process		(CGameSocket* pSocket)
{
	try {
	// 检查消息格式
	//来自其他地图组的没有参数
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
	char szName[_MAX_NAMESIZE] = "";
	char szWord[256] = "";
	if (m_StrPacker.GetStrAmount() > 0)
		m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
	if (m_StrPacker.GetStrAmount() > 1)
		m_StrPacker.GetString(1, szWord, 256);

	// 状态检查
	CUserPtr pUser = UserManager()->GetUser(this);
 	if(pUser==NULL)
	{
		///这里处理的消息不依赖用户指针
		switch(m_pInfo->usAction)
		{
			///这里是别的线发过来的帮派发送成功的消息
			case SYN_CREATE_OK:
				{
					///先看本地是否已存在这个帮派，如果没有再尝试从数据库中创建
					///如果已存在实例则进行更新操作
					SynManager()->Create(m_pInfo->idTarget);///请求帮派管理器在本地创建一个帮派实例
					///凡是收到这个消息的线均将帮主设置为在线状态
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
	// 				if(pSyn)
	// 				{
	// 					pSyn->SetOnline(pSyn->GetLeaderID(),true);
	// 				}
				}	
				break;
			case SYN_ANNONCE_OTHERLINE:///副本收到的修改消息
//			case SYN_ANNONCE:
				{
//  					CUserPtr pMasterUser = UserManager()->GetUser(this);
// 					if(pMasterUser==NULL)
//  					{
						///如果帮主的对象为空则表示这个消息是从其它线过来的
						CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
						if(pSyn)
						{
							pSyn->SetStr(SYNDATA_ANNOUNCE,szWord,256,true);
							CMsgTalk	msg;
							IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szWord, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE))
								pSyn->BroadcastSynMsg(&msg);
						}
	//  					MyStackWalker sw;
	//  					sw.ShowCallstack();
// 					}
				}
				break;
			case SYN_CREATECOMPLETE:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
					if(pSyn==NULL)///如果找不到这个帮派尝试重新构建一下
					{
						SynManager()->Create(m_pInfo->idTarget);///请求帮派管理器在本地创建一个帮派实例
						pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
					}
					if(pSyn)
						pSyn->BroadcastSynMsg(this);
				}
				break;

				
			case	SYN_UPLEV_OTHERLINE:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
					if (pSyn)
					{
							pSyn->UpdateLevel(m_pInfo->dwData);
						if (pUser)
						{
							if (m_pInfo->dwData!=0)
								pSyn->SendSciToC(pUser);
							else
								pSyn->SendAllInfoToClient(pUser);
						}
					}
				}
				break;
			case	SYN_UPSKILLSUB_OTHERLINE:
				{
// 					int nSub = 0;
// 					int nSkill = 0;
// 					nSub = m_pInfo->dwData / 10;
// 					nSkill = m_pInfo->dwData % 10;
// 					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 					if (pSyn)
// 					{
// 						pSyn->UpSubSkill(nSub, nSkill);
// 						CMsgTalk	msg;
// 						sprintf(szWord, "%d", m_pInfo->dwData);
// 						IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szWord, NULL, 0xFFFFFF, _TXTATR_SYNUPSKILL))
// 						pSyn->BroadcastSynMsg(&msg);
// 					}
				}
				break;
			case SYN_SETBUILDOTHERLINE:
				{
					UINT Money = 0;
					UINT Build = 0;
					int nType0 = 0;
					int nType = m_pInfo->dwData % 10;
					int idSyn = m_pInfo->dwData / 10;
					CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
					if (pSyn)
					{
						pSyn->SetGrow(nType, m_pInfo->idTarget, false);
					}
				}
				break;
			case	SYN_INVITE_OTHERLINE:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->dwData);
					if(!pSyn)
						return;
					OBJID pTargetid = m_pInfo->idTarget;
					CUser* pTarget	= UserManager()->GetUser(pTargetid);
					if (pTarget)
					{
						this->Create(SYN_INVITE, m_pInfo->dwData, pUser->GetName(), 0, "");
						pTarget->SendMsg(this);
						return;
					}
				}
				break;
			case 	SYN_SYNTALKMSG:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->dwData);
					if(!pSyn)
						return;
					CMsgTalk	msg;
					if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
						pSyn->BroadcastSynMsg(&msg);					
				}
				break;
			case SYN_UPDATEOTHERNLINE:
				{	
						CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->dwData);
						if(!pSyn)
							return;
						CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
						if(pThisUser)
						{
							if (pThisUser->GetMap()==pThisUser->GetSynMap())
								pThisUser->LeaveSynMap();
							pThisUser->QuerySynAttr()->LeaveSyn(true);
							pThisUser->SendSysMsg("你被踢出帮派");
						}
						else if(pSyn->Hasmember( m_pInfo->idTarget))
						{
							::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->idTarget,1);
							if(info)
							{
								sprintf(szWord, "%s被踢出帮派", info->szName);
								CMsgTalk msg;
								if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
									pSyn->BroadcastSynMsg(&msg);
								pSyn->DelMemberList(m_pInfo->idTarget);
							}
						}
					}

				break;
			case SYN_SETSYNMAPID:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
					if(!pSyn)
							return;
					if (m_idProcess!=MSGPORT_MAPGROUP_FIRST)
							pSyn->SetSynMapID(m_pInfo->dwData);
				}
				break;
			default:
				break;
		}
		return;
	}
	// 参数检查
//	ASSERT(pSocket);
	if(!pSocket)
		return;

	IF_NOT(pUser)
		return;

	OBJID idUser = pUser->GetID();
	IF_NOT(idUser != ID_NONE)
		return;

	if(!pUser->IsAlive())
	{
		pUser->SendSysMsg("你已经死了");//STR_DIE
		return ;
	}


	switch(m_pInfo->usAction)
	{
	case SYN_CREATE:
		{
			CHECK(m_pInfo->idTarget);
			CItem* pItem = pUser->GetItemByType(630007);
			if(!pItem)
				return;

			if(pUser->GetMoney() < 1000000)
				return;
		
			CSyndicate* pSynOld = pUser->GetSyndicate();
			if(pSynOld )
			{
				if(pSynOld->GetInt(SYNDATA_STATE) == 1)
				{
					if(pUser->QuerySynAttr()->GetSynRank() == 100)
					{
						pUser->SendSysMsg("您的帮派在创建中，不能建立别的帮派");
						return;
					}
					else
					{
						if(!pUser->QuerySynAttr()->LeaveSyn())
						{
							pUser->SendSysMsg("您的帮派在创建中，不能建立别的帮派");
							return;
						}
					}
				}
				else
				{
					pUser->SendSysMsg("你已经在帮派中，退出后才能加入其它帮派");
					return;
				}
			}
			CHECK(pUser->QuerySynAttr());
			OBJID idSyn = pUser->QuerySynAttr()->CreateSyndicate(szName, szWord);
			if(idSyn != ID_NONE)
			{
				if (pUser->SpendItem(pItem))
				{
					if(pUser->SpendMoney(1000000,true))
					{
						this->Create(SYN_CREATE_OK,idSyn);
					}
					else
					{
						LogSave("ERROR: 帮派扣除资金出错! idSyn=%u , iduser=%u",idSyn,pUser->GetID());
						this->Create(SYN_CREATE_ERROR,pUser->GetID());
					}
				}
				else
				{
					LogSave("ERROR: 帮派扣除物品出错! itemtype=%u , idSyn=%u , iduser=%u",pItem->GetItemType(),idSyn,pUser->GetID());
					this->Create(SYN_CREATE_ERROR,pUser->GetID());
				}
			}
			else
			{
				this->Create(SYN_CREATE_ERROR,pUser->GetID());
			}
			pUser->SendMsg(this);
		}
		break;
	case SYN_QUERY_CREATE:
		{
			SynManager()->SendCreateSynToUser(pUser,m_pInfo->dwData);
		}
		break;
	case SYN_JOIN:
		{
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
// 
// 					}
// 
// 				}
// 				pUser->QuerySynAttr()->JoinSyn(m_pInfo->idTarget);
// 				pUser->SendSysMsg("响应成功");
// 
// 				///响应成功,将帮派信息广播到其它线,仍走创建成功的流程
// //  				CMsgSyndicate msg;
// //  				if(msg.Create(SYN_CREATE_OK,m_pInfo->idTarget,pUser->GetID()))
// //  					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, ALLUSERS_NAME);
// 			}
// 			else
// 			{
// 				pUser->SendSysMsg("对应帮派已经创建成功");
// 			}
// 
// 			CMsgSyndicateList msg;
// 			if (msg.Create(SYNLISTACTION_UPDATA,pSyn->GetID(),pSyn->GetStr(SYNDATA_NAME),pSyn->GetStr(SYNDATA_LEADERNAME),pSyn->GetInt(SYNDATA_LEVEL),pSyn->GetInt(SYNDATA_AMOUNT)))
// 				pUser->SendMsg(&msg);
		}
		break;
	case SYN_QUERY_INFO:
		{
			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);

			if(!pSyn)
				return;
			pSyn->SendSynInfoToClient(pUser);
		}
		break;

	case SYN_EVERYSUB:   
		{
			CSyndicate* pSyn = NULL/*SynManager()->QuerySyndicate(m_pInfo->idTarget)*/;
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			pSyn->SendSubToC(pUser);
		}
		break;
	case SYN_CONLIST://帮贡列表
	case SYN_MEMBERLIST://成员列表
		{
			CSyndicate* pSyn;
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			
			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
				return;
			
			pSyn->SendMenberListToClient(pUser);

		}
		break;
	case SYN_QUERYLIST://申请列表
		{
			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			
			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
				return;
			
			pSyn->SendQueryListToClient(pUser,m_pInfo->dwData);

		}
		break;
	case SYN_QUERYSYNLIST:		//帮派列表
		{
			SynManager()->SendSynList(pUser,m_pInfo->dwData);
		}
		break;
	case SYN_SYNINFO:		//帮派信息
		{
			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			pSyn->SendAllInfoToClient(pUser);
		}
		break;

	case 		SYN_LEVELUP://升级
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
			{
				if (pUser->IsGM())
					pUser->SendSysMsg("没找到帮");
				return;
			}
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}
			int nRank = pUser->QuerySynRank();
			if (nRank < RANK_SUBLEADER)
			{
				pUser->SendSysMsg("您的权限不足！");
				return;
			}
			if(pSyn->UpLevel(pUser, m_pInfo->dwData))
			{
				CMsgSyndicate msgSyndicate;
				m_pInfo->idTarget = pSyn->GetID();
				msgSyndicate.Create(SYN_UPLEV_OTHERLINE,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData,szWord);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,(OBJID)0);
				if (m_pInfo->dwData!=0)
					pSyn->SendSciToC(pUser);
				else
					pSyn->SendAllInfoToClient(pUser);                      
			}
			else
				pUser->SendSysMsg("升级失败！");
		}
		break;
	case SYN_SCIINFO:   //获取科技信息
		{
			CSyndicate* pSyn = NULL/*SynManager()->QuerySyndicate(m_pInfo->idTarget)*/;
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(!pSyn)
				return;
			pSyn->SendSciToC(pUser);
		}
		break;
	case SYN_ACTION_ENTERSYN:
		{
			//走技能流程了!
		}
		break;
	case SYN_SUB1_ACTION://青龙
		{
			pUser->ProcessAction(1018);
		}
		break;
	case SYN_SUB2_ACTION://白虎
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
			{
				pUser->SendSysMsg("你还没进帮!");
				return;
			}
			if(!pSyn->GetCallTask())
			{
				if (m_pInfo->idTarget>10 || m_pInfo->idTarget <= 0)
					return;
				LuaScriptMachine()->InitVar(m_pInfo->idTarget);//传给脚本
				if(pUser->ProcessAction(1011))
					pSyn->SetCallTask(true);
			}
			else
			{
				pUser->SendSysMsg("本次敌袭已结束");
			}
		}
		break;
	case SYN_SUB3_ACTION://玄武
		{
			pUser->ProcessAction(2904604);
		}
		break;
	case SYN_SUB4_ACTION://朱雀
		{
			pUser->ProcessAction(1016);
		}
		break;
	case SYN_ACTION_WELFARE:		//领取福利
		{
			
		}
		break;
	case SYN_ACTION_SHOP:		//商店
		{
			pUser->ProcessAction(1017);
		}
		break;
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
// //			pSyn->SendMenberToClient(pUser,m_pInfo->dwData);
// 		}
// 		break;
	case SYN_QUERY_LIST:
		{
			SynManager()->SendSynList(pUser,m_pInfo->dwData);
		}
		break;
	case SYN_JOIN_QUERY:
		{
			CSyndicate* pSynOld = pUser->GetSyndicate();
			if(pSynOld )
			{
				if(pSynOld->GetInt(SYNDATA_STATE) == 1)
				{
					if(pUser->QuerySynAttr()->GetSynRank() == 100)
					{
						pUser->SendSysMsg("您的帮派在创建中，不能加入别的帮派");
						return;
					}
					else
					{
						if(!pUser->QuerySynAttr()->LeaveSyn())
						{
							pUser->SendSysMsg("您的帮派在创建中，不能加入别的帮派");
							return;
						}
					}
				}
				else
				{
					pUser->SendSysMsg("你已经在帮派中，退出后才能加入其它帮派");
					return;
				}
			}


			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			
			if(pSyn->GetInt(SYNDATA_STATE) != 2) 
				return;

			if(pSyn->GetMemberAmount() >= levMemberAmount[pSyn->GetInt(SYNDATA_LEVEL)])
			{
				pUser->SendSysMsg("帮会人数已满");
				return;
			}
			
			MemberInfo info={0};
			info.id = pUser->GetID();
			info.level = pUser->GetLev();
			info.firJob = pUser->GetProfession(1);
			info.sex=pUser->GetSex();
			info.maxpower=pUser->GetMaxScore();
			if (pUser->IsVipActive())
				info.vip=pUser->GetVipLev();
			else
				info.vip=0;
			info.line=CVipList::GetLineByName(MapGroup(PID)->GetServerName());
			SafeCopy(info.szName,pUser->GetName());
			if(pSyn->GetMemberFromList(info.id,2)==NULL)
			{
				if(pSyn->AddMemberList(info,2) == RANK_NORMAL)
				{
					///有人申请入帮，通知其它线更新数据
					CMsgSynMemberInfo msg;
					if (msg.Create(SynMemberAdd,2,2))
					{
						msg.Append(pSyn->GetID(),info.szName,info.level,info.firJob,info.sex,10,info.line,info.vip,info.lastlogin,0,0,0,info.maxpower);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
						msg.Append(info.id,info.szName,info.level,info.firJob,info.sex,10,info.line,info.vip,info.lastlogin,0,0,0,info.maxpower);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
					}
					pUser->SendSysMsg("申请发送成功");
				}
			}
			else
			{
				pUser->SendSysMsg("你已经在对方的申请列表中，请不要重复申请");
			}
		}
		break;
 	case SYN_QUERY_QUERYMEMBER:
	{
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
// //			pSyn->SendQueryMenberToClient(pUser,m_pInfo->dwData);
		}
		break;
	case SYN_ANSERR_OK:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			
			///回包内含用角色名字则表明此包为得锁包
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
				return;

			int nRank = pUser->QuerySynRank();
			if(nRank < RANK_FEELLEADER)
			{
				pUser->SendSysMsg("你的权限不足");
				return;
			}

			if(pSyn->GetMemberAmount() >= levMemberAmount[pSyn->GetInt(SYNDATA_LEVEL)])
			{
				pUser->SendSysMsg("帮会人数已满");
				return;
			}

			if(!pSyn->AllowMenber(m_pInfo->idTarget,pUser))
			{
				pUser->SendSysMsg("对方已经加入了帮派");
				return;
			}
			OBJID idTarget=m_pInfo->idTarget;
			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
			if(pThisUser)
			{
				pThisUser->SendSysMsg("加入%s通过",pSyn->GetStr(SYNDATA_NAME));

				this->Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_ALLOW);
				pThisUser->SendMsg(this);
				pUser->SendMsg(this);
			}
			else
			{
				this->Create(SYN_UPDATE,pUser->GetID(),SYN_UPDATE_ALLOW);
				pUser->SendMsg(this);
			}
		}
		break;
	case SYN_ANSERR_NO:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			
			///回包内含用角色名字则表明此包为得锁包
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
				return;

			int nRank = pUser->QuerySynRank();
			if(nRank < RANK_FEELLEADER)
			{
				pUser->SendSysMsg("你的权限不足");
				return;
			}
			OBJID idTarget=m_pInfo->idTarget;
			pSyn->DelMemberList(m_pInfo->idTarget,2);
			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
			if(pThisUser)
			{
				pThisUser->SendSysMsg("被%s拒绝",pSyn->GetStr(SYNDATA_NAME));
				this->Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_NO);
				pThisUser->SendMsg(this);
			}
			///批准申请入帮，通知其它线更新数据
			CMsgSynMemberInfo msg;
			if (msg.Create(SynMemberAnswerNO,2,2))
			{
				msg.Append(pSyn->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
				msg.Append(idTarget,"",0,0,0,0,0,0,0,0,0,0,0);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
			}
		}
		break;
	case SYN_KICKOUT:
		{
			bool bIgnorData = false;
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
			{
				pUser->SendSysMsg("没找到帮派");
				return;
			}
			///回包内含用角色名字则表明此包为得锁包
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			
			int nMyRank = pUser->QuerySynRank();
			OBJID idTarget=m_pInfo->idTarget;
			if(nMyRank < RANK_FEELLEADER)
			{
				pUser->SendSysMsg("你的权限不足");
				return;
			}
			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
			if(pThisUser&&pThisUser->QuerySynAttr()!=NULL)
			{
				if ( pThisUser->QuerySynRank() == RANK_NONE)
				{
					pUser->SendSysMsg("对方已经离开了帮派");
					return;
				}
				if ((pThisUser->GetSyndicate()->GetID() != pSyn->GetID()) || nMyRank < pThisUser->QuerySynRank() )
				{
					pUser->SendSysMsg("你的权限不足");
					return;
				}
				if (pUser->GetID() == idTarget)		//确保不能自己T自己
				{
					pUser->SendSysMsg("不能踢自己");
					return;
				}
				if (pThisUser->QuerySynAttr()->LeaveSyn(true,true,true))
				{
					pThisUser->SendSysMsg("你被踢出帮派");
					this->Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_KICK);
					pThisUser->SendMsg(this);
					pThisUser->DelHoldTitle(_TITLE_SYNDICATE);//goto 删除称号
					CMsgSynMemberInfo msgtoc;
					if (msgtoc.Create(DelMember,1,1))
					{
						msgtoc.Append(pThisUser->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);
						pSyn->BroadcastSynMsg(&msgtoc);
					}
					pUser->SendSysMsg("踢出成功");
				}
				else
					pUser->SendSysMsg("踢出失败");
			}
			else
			{
				int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
				if(nThisRank == RANK_NONE)
				{
					pUser->SendSysMsg("对方已经离开了帮派");
					return;
				}
				if ((pUser->GetSyndicate()->GetID() != pSyn->GetID()) || nMyRank < nThisRank)
				{
					pUser->SendSysMsg("你的权限不足");
					return;
				}
				if (nThisRank == RANK_NONE)//异常数据
				{
					bIgnorData = true;
				}
				if (pUser->GetID() != idTarget)		//确保不能自己T自己
				{
					if(pUser->QuerySynAttr()->KickoutMember(m_pInfo->idTarget, bIgnorData))
						pUser->SendSysMsg("踢出成功");
					else
						pUser->SendSysMsg("踢出失败");
// 					if (nThisRank >= RANK_FEELLEADER)
// 						CUserTitle::DelOffLineUserTitle(idTarget, _TITLE_SYNDICATE, Database());
					CMsgSynMemberInfo msgtoc;
					if (msgtoc.Create(DelMember,1,1))
					{
						msgtoc.Append(idTarget,"",0,0,0,0,0,0,0,0,0,0,0);
						pSyn->BroadcastSynMsg(&msgtoc);
					}
				}
				else
					pUser->SendSysMsg("不能踢自己");

			}
			///有人退帮，通知其它线更新数据
			CMsgSynMemberInfo msg;
			if (msg.Create(SynMemberKickout,1,2))
			{
				msg.Append(pSyn->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
				msg.Append(idTarget,"",0,0,0,0,0,0,0,0,0,0,0);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
			}
		
		}
		break;
	case SYN_LEAVER:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			///回包内含用角色名字则表明此包为得锁包
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			int nMyRank = pUser->QuerySynRank();
			if(nMyRank == RANK_LEADER)
				return;
			
			if (pUser->QuerySynAttr()->LeaveSyn())	
			{
				pUser->DelHoldTitle(_TITLE_SYNDICATE);//goto 删除称号
				pUser->SendSysMsg("离开成功");
			}
			else
			{
				pUser->SendSysMsg("离开失败");
				return;
			}
			///有人退帮，通知其它线更新数据
			CMsgSynMemberInfo msg;
			if (msg.Create(SynMemberRemove,1,2))
			{
				msg.Append(pSyn->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
				msg.Append(pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,0,0,0,0);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
			}
			this->Create(SYN_UPDATE,pUser->GetID(),SYN_UPDATE_LEAVE);
			pUser->SendMsg(this);
		}
		break;
	case SYN_SETRANK:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
		
			///回包内含用角色名字则表明此包为得锁包
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			int nMyRank = pUser->QuerySynRank();
			if(nMyRank <= m_pInfo->dwData)//比他高级才能定他职位
			{
				pUser->SendSysMsg("你的权限不足");
				return;
			}

			int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
			if(nThisRank == RANK_NONE)
			{
				pUser->SendSysMsg("对方已经离开了帮派");
				return;
			}

			if(m_pInfo->dwData < RANK_NORMAL || m_pInfo->dwData > RANK_SUBLEADER)
			{
				pUser->SendSysMsg("职位出错");
				return;
			}
			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),m_pInfo->dwData,true))
			{
				///任命成功通知其它线更新
				///批准申请入帮，通知其它线更新数据
				CMsgSynMemberInfo msg;
				if (msg.Create(SynMemberSetRank,2,2))
				{
					msg.Append(pSyn->GetID(),"",0,0,0,m_pInfo->dwData,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
					msg.Append(m_pInfo->idTarget,"",0,0,0,m_pInfo->dwData,0,0,0,0,0,0,0);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
				}
				CMsgSynMemberInfo msgtoc;
				if (msgtoc.Create(SetRank,1,1))
				{
					msgtoc.Append(m_pInfo->idTarget,"",0,0,0,m_pInfo->dwData,0,0,0,0,0,0,0);
					pSyn->BroadcastSynMsg(&msgtoc);
				}
				pUser->SendSysMsg("任命成功");
			}
		
		}
		break;
	case SYN_SETLEADER:		//任命帮主
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if (!pUser->QuerySynAttr())
				return;
			if(!pSyn)
			{
				pUser->SendSysMsg("你还没有帮！");
				return;
			}
			///回包内含用角色名字则表明此包为得锁包
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}	
			if (pUser->QuerySynRank() != RANK_LEADER)
			{
				pUser->SendSysMsg("你没权限！");
				return;
			}
			if(m_pInfo->idTarget != pSyn->GetSubLeader2ID())
			{ 
				if(m_pInfo->idTarget != pSyn->GetSubLeaderID())
				{
					pUser->SendSysMsg("只能任命副帮主为帮主");
					return;
				}
			}
			CMsgSynMemberInfo msg;
			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),RANK_LEADER,true))
			{
				if (msg.Create(SynMemberSetRank,2,2))
				{
					msg.Append(pSyn->GetID(),"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
					msg.Append(m_pInfo->idTarget,"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
					CMsgSynMemberInfo msgtoc;
					if (msgtoc.Create(SetRank,1,1))
					{
						msgtoc.Append(m_pInfo->idTarget,"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);
						pSyn->BroadcastSynMsg(&msgtoc);
					}
				}
				else
					LogSave("ERROR: 创建消息出错! idsyn=%u,iduser=%u,idleader=%u",pSyn->GetID(),pUser->GetID(),pSyn->GetLeaderID());
				if (pUser->QuerySynAttr()->SetIdRank(pUser->GetID(),pSyn->GetID(),RANK_SUBLEADER,true))
				{
					if (msg.Create(SynMemberSetRank,2,2))
					{
						msg.Append(pSyn->GetID(),"",0,0,0,RANK_SUBLEADER,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
						msg.Append(pUser->GetID(),"",0,0,0,RANK_SUBLEADER,0,0,0,0,0,0,0);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
					}
					CMsgSynMemberInfo msgtoc;
					if (msgtoc.Create(SetRank,1,1))
					{
						msgtoc.Append(pUser->GetID(),"",0,0,0,RANK_SUBLEADER,0,0,0,0,0,0,0);
						pSyn->BroadcastSynMsg(&msgtoc);
					}
					pUser->SendSysMsg("任命成功");
					CMsgSynInfo msgup;
					if(msgup.CreateNew(pSyn,pUser))
						pSyn->BroadcastSynMsg(&msgup);
				}
				else
					LogSave("ERROR: 任命帮主出错了! idsyn=%u,iduser=%u,idleader=%u",pSyn->GetID(),pUser->GetID(),pSyn->GetLeaderID());
			}
			else
				pUser->SendSysMsg("任命失败!");
		}
		break;
	case SYN_DEMISE:
		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			///回包内含用角色名字则表明此包为得锁包
// 			if(strcmp(szName,pUser->GetName())!=0)
// 			{
// 				CMsgSyndicate msg;
// 				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
// 				return;
// 			}			
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
// 			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),RANK_LEADER),true)
// 			{
// 				pUser->SendSysMsg("让位成功");
// 				///任命成功通知其它线更新
// 				///批准申请入帮，通知其它线更新数据
// 				CMsgSynMemberInfo msg;
// 				if (msg.Create(SynMemberSetRank,1,1))
// 				{
// 				
// 					msg.Append(m_pInfo->idTarget,"",RANK_LEADER,0,0,0,0,0,0,0,0,0,0);
// 					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,(OBJID)0);
// 				}
// 			}
		}
		break;
	case SYN_IMPACH:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if (!pUser->QuerySynAttr())
			{
				return;
			}
			if(!pSyn)
			{
				pUser->SendSysMsg("你还没有帮！");
				return;
			}
			///回包内含用角色名字则表明此包为得锁包
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}	
			if (pUser->QuerySynRank() != RANK_SUBLEADER)
			{
				pUser->SendSysMsg("你没权限！");
				return;
			}
			if(pUser->QuerySynRank() == RANK_LEADER)
			{
				pUser->SendSysMsg("帮主不能弹劾自己！");
				return;
			}
			CUserPtr pThisUser = UserManager()->GetUser(pSyn->GetLeaderID());
			if(pThisUser)
			{
				pUser->SendSysMsg("帮主连续三天不在线才能弹劾帮主");
				return;
			}
			stTempUser* pTempData = UserManager()->QueryTempUser(pSyn->GetLeaderID());
			if(!pTempData)
				return;
			DWORD passdays=pUser->GetPassedTime(pTempData->Exit_Time,3);
			if (passdays < 3)
			{
				pUser->SendSysMsg("帮主连续三天不在线才能弹劾帮主");
				return;
			}
			OBJID idoldleader=pSyn->GetLeaderID();
			if (pUser->GetSynCon() >= 250)
			{
				CMsgSynMemberInfo msg;
				if(pUser->QuerySynAttr()->SetIdRank(idoldleader,pSyn->GetID(),RANK_NORMAL,true))
				{
					if (msg.Create(SynMemberSetRank,2,2))
					{
						msg.Append(pSyn->GetID(),"",0,0,0,RANK_NORMAL,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
						msg.Append(idoldleader,"",0,0,0,RANK_NORMAL,0,0,0,0,0,0,0);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
						CMsgSynMemberInfo msgtoc;
						if (msgtoc.Create(SetRank,1,1))
						{
							msgtoc.Append(idoldleader,"",0,0,0,RANK_NORMAL,0,0,0,0,0,0,0);
							pSyn->BroadcastSynMsg(&msgtoc);
						}
					}
					else
						LogSave("ERROR: 创建消息出错! idsyn=%u,iduser=%u,idleader=%u",pSyn->GetID(),pUser->GetID(),pSyn->GetLeaderID());
					if (pUser->QuerySynAttr()->SetIdRank(pUser->GetID(),pSyn->GetID(),RANK_LEADER,true))
					{
						if (msg.Create(SynMemberSetRank,2,2))
						{
							msg.Append(pSyn->GetID(),"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
							msg.Append(pUser->GetID(),"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
							CMsgSynMemberInfo msgtoc;
							if (msgtoc.Create(SetRank,1,1))
							{
								msgtoc.Append(pUser->GetID(),"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);
								pSyn->BroadcastSynMsg(&msgtoc);
							}
						}
						pUser->SetSynCon(pUser->GetSynCon()-250);
						pUser->SendSysMsg("弹劾成功");
						CMsgSynInfo msgup;
						if(msgup.CreateNew(pSyn,pUser))
							pSyn->BroadcastSynMsg(&msgup);
					}
					else
						LogSave("ERROR: 弹劾出错了! idsyn=%u,iduser=%u,idleader=%u",pSyn->GetID(),pUser->GetID(),pSyn->GetLeaderID());
				}
				else
					pUser->SendSysMsg("弹劾失败!");
			}
			else
			{
				pUser->SendSysMsg("帮贡不足");
			}
		}
		break;
	case SYN_ANNONCE:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			
			///回包内含用角色名字则表明此包为得锁包
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,pSyn->GetID(),pUser->GetName(),m_pInfo->dwData,szWord);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			int nMyRank = pUser->QuerySynRank();
			if(nMyRank != RANK_LEADER && nMyRank != RANK_SUBLEADER)
			{
				pUser->SendSysMsg("你的权限不足");
				return;
			}
			
			pSyn->SetStr(SYNDATA_ANNOUNCE,szWord,256,true);


			CMsgTalk	msg;
			IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szWord, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE))
				pSyn->BroadcastSynMsg(&msg);

			pUser->SendSysMsg("修改成功");

			///修改公告时更新其它线
			m_pInfo->idTarget=pSyn->GetID();
			CMsgSyndicate msgSyndicate;
			msgSyndicate.Create(SYN_ANNONCE_OTHERLINE,m_pInfo->idTarget,"",0,szWord);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,0);
		}
		break;
// 	case	SYN_UPLEV:
// 		{
// 			///回包内含用角色名字则表明此包为得锁包
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			if(strcmp(szName,pUser->GetName())!=0)
// 			{
// 				CMsgSyndicate msg;
// 				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
// 				return;
// 			}
// 			int nRank = pUser->QuerySynRank();
// 			if (nRank < RANK_SUBLEADER)
// 			{
// 				pUser->SendSysMsg("您的权限不足！");
// 				return;
// 			}
// 			if(pSyn->UpLevel(pUser, m_pInfo->dwData))
// 			{
// 				CMsgSyndicate msgSyndicate;
// 				m_pInfo->idTarget = pSyn->GetID();
// 				msgSyndicate.Create(SYN_UPLEV_OTHERLINE,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData,szWord);
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,0);
// 			}
// 			else
// 				pUser->SendSysMsg("升级失败！");
// 
// 		}
// 		break;

	case	SYN_CONTRIBUTE:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			UINT	Addup	= m_pInfo->idTarget / 10;
			int		Endflag	= m_pInfo->idTarget % 10;
			if (m_pInfo->dwData)
			{
				pUser->ConItem(Addup, Endflag);
			}
			else
			{
				pUser->ConMoney(Addup, true, Endflag);
			}
		}
		break;
	case	SYN_INVITE:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			if (pUser->GetSynRank() < RANK_FEELLEADER1)
			{
				return;
			}
			CUser* pTarget	= UserManager()->GetUser(szName);
			if (pTarget)
			{
				return;
			}
			else
			{
				CMsgSyndicate msgSyndicate;
				m_pInfo->idTarget = pTarget->GetID();
				msgSyndicate.Create(SYN_INVITE_OTHERLINE ,m_pInfo->idTarget ,pUser->GetName() ,pSyn->GetID() ,szWord);
				pUser->SendSysMsg("邀请请求已经发出，请等待对方回应！");
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,0);
			}
		}
		break;
	default:
		ASSERT(!"switch MsgSyndicate");
		break;
	}

	}
	catch(...)
	{	
		::LogSave("MsgSyndicate error: %d,%d,%d",m_pInfo->usAction,m_pInfo->idTarget,m_pInfo->dwData);//20061025修罗.测试用.自己手动来解开来看
	}
}
//--------------------------------------------------
//20070403修罗:用来发送目标名字
BOOL CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget, const char *szTargetName,unsigned short dwData,const char *szWords)
{
	// check param
	if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
	{
		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}

	// init
	this->Init();
	
	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;
	m_pInfo->dwData		=dwData;

	m_StrPacker.AddString( szTargetName );
	m_StrPacker.AddString(	szWords		);
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	m_unMsgType	=_MSG_SYNDICATE;


	return true;
}

BOOL CMsgSyndicate::Create(unsigned short usAction, unsigned short dwData, OBJID idTarget, char *szBuf)
{
	if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
	{
		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}
	
	// init
	this->Init();
	
	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;
	m_pInfo->dwData		=dwData;
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_SYNDICATE;
	return true;
}
