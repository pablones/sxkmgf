// MsgSynMemberInfo.cpp: interface for the CMsgSynMemberInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "SynAttr.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgSynMemberInfo::CMsgSynMemberInfo()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgSynMemberInfo::~CMsgSynMemberInfo()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynMemberInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_SYNMEMBERINFO != this->GetType())
		return false;

	return true;
}

BOOL CMsgSynMemberInfo::Create (USHORT nAction,USHORT nPage,USHORT nMaxAmount)
{
	this->Init();

	m_unMsgType	=_MSG_SYNMEMBERINFO;
	m_unMsgSize	=sizeof(MSG_Info) - sizeof(MemberInfo);
	m_pInfo->ucActiom = nAction;
	m_pInfo->ucMaxAmount = nMaxAmount;
	m_pInfo->ucAmount = 0;
	m_pInfo->ucPage = nPage;
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynMemberInfo::Append(OBJID	id,	const char* szName,	USHORT	level,
							   USHORT	firJob,	USHORT	sex,USHORT	rank,USHORT   line,USHORT	vip,
						 OBJID   lastlogin,OBJID   hiscon,OBJID   hismoney,OBJID   hisgrow,OBJID	maxpower)
{
	CHECKF(m_unMsgSize + sizeof(MemberInfo) < MAX_PACKETSIZE);

	m_unMsgType	=_MSG_SYNMEMBERINFO;
	m_unMsgSize	+= sizeof(MemberInfo);

	m_pInfo->memInfo[m_pInfo->ucAmount].id = id;
	SafeCopy(m_pInfo->memInfo[m_pInfo->ucAmount].szName,szName,16);
	m_pInfo->memInfo[m_pInfo->ucAmount].level = level;
	m_pInfo->memInfo[m_pInfo->ucAmount].firJob = firJob;
	m_pInfo->memInfo[m_pInfo->ucAmount].sex = sex;
	m_pInfo->memInfo[m_pInfo->ucAmount].rank = rank;
	m_pInfo->memInfo[m_pInfo->ucAmount].line = line;
	m_pInfo->memInfo[m_pInfo->ucAmount].vip = vip;
	m_pInfo->memInfo[m_pInfo->ucAmount].lastlogin = lastlogin;
	m_pInfo->memInfo[m_pInfo->ucAmount].hiscon = hiscon;
	m_pInfo->memInfo[m_pInfo->ucAmount].hismoney = hismoney;
	m_pInfo->memInfo[m_pInfo->ucAmount].hisgrow = hisgrow;
	m_pInfo->memInfo[m_pInfo->ucAmount].maxpower = maxpower;
	m_pInfo->ucAmount ++;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgSynMemberInfo::Process(void *pInfo)
{
	DEBUG_TRY

	CHECK(m_pInfo);

	char szWord[1024]="";
	///这个消息里面只允许存在两个成员
	if (m_pInfo->ucAmount!=2)
			return;
	OBJID idSyn=m_pInfo->memInfo[0].id;
	CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
	if(pSyn==NULL)///如果找不到这个帮派尝试重新构建一下
	{
		SynManager()->Create(idSyn);///请求帮派管理器在本地创建一个帮派实例
		pSyn = SynManager()->QuerySyndicate(idSyn);
	}
	if(pSyn==NULL)
		return;

	switch(m_pInfo->ucActiom)
	{
		///这里是别的线发过来的帮派发送成功的消息
		case SynMemberAdd:
			{
				switch (m_pInfo->ucPage)
				{
				case 1://成员列表
					{
						pSyn->DelMemberList(m_pInfo->memInfo[1].id,3);
						CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].id);
						if(pThisUser&&pThisUser->QuerySynAttr()!=NULL)
						{
							pThisUser->QuerySynAttr()->JoinSyn(pSyn->GetID(),false,m_pInfo->memInfo[1].id,false);
							pThisUser->SendSysMsg("加入%s通过",pSyn->GetStr(SYNDATA_NAME));
							CMsgSyndicate msg;
							msg.Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_ALLOW);
							pThisUser->SendMsg(&msg);
							///广播自己的帮派上线通知给别的线
							CMsgSynMemberInfo msgSynMemberInfo;
							if (msgSynMemberInfo.Create(SynMemberOnlineStatusChange,1,2))
							{
								msgSynMemberInfo.Append(pSyn->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
								msgSynMemberInfo.Append(pThisUser->GetID(),"",0,0,0,0,CVipList::GetLineByName(MapGroup(PID)->GetServerName()),0,0,0,0,0,0);
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSynMemberInfo,0);
							}
						}
						else
						{
							::MemberInfo info={0};
							info.id = m_pInfo->memInfo[1].id;
							SafeCopy(info.szName,m_pInfo->memInfo[1].szName,16);
							info.level = m_pInfo->memInfo[1].level;
							info.firJob = m_pInfo->memInfo[1].firJob;
							info.rank = m_pInfo->memInfo[1].rank;
							info.sex=m_pInfo->memInfo[1].sex;
							info.line=m_pInfo->memInfo[1].line;
							info.vip=m_pInfo->memInfo[1].vip;
							info.lastlogin=m_pInfo->memInfo[1].lastlogin;
							info.hiscon=m_pInfo->memInfo[1].hiscon ;
							info.hismoney=m_pInfo->memInfo[1].hismoney;
							info.hisgrow=m_pInfo->memInfo[1].hisgrow ;
							info.maxpower=	m_pInfo->memInfo[1].maxpower ;
							pSyn->AddMemberList(info);
						}
						::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->memInfo[1].id,1);
						if(info)
						{
							sprintf(szWord,"%s加入帮派",info->szName);
							CMsgTalk	msg;
							if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
								pSyn->BroadcastSynMsg(&msg);
							///除外转给其它线，还要本地帮派广播一下
							//					pSyn->BroadcastSynMemberInfoMsg(info->nId);
							CMsgSynMemberInfo msgMem;
							if (msgMem.Create(AddMember,1,1))
							{
								msgMem.Append(info->id,info->szName,info->level,info->firJob,info->sex,info->rank,info->line,info->vip,info->lastlogin,info->hiscon,info->hismoney,info->hisgrow,info->maxpower);
								if (pThisUser)
									pSyn->BroadcastSynMsg(&msgMem,pThisUser);
								else
									pSyn->BroadcastSynMsg(&msgMem);
							}
							
						}
					}
					break;
				case 2://申请列表
					{
						::MemberInfo* pinfo =pSyn->GetMemberFromList(m_pInfo->memInfo[1].id,m_pInfo->ucPage);
						if(pinfo!=NULL)
						{
							pSyn->DelMemberList(m_pInfo->memInfo[1].id,3);
						}
						::MemberInfo info={0};
						info.id = m_pInfo->memInfo[1].id;
						SafeCopy(info.szName,m_pInfo->memInfo[1].szName,16);
						info.level = m_pInfo->memInfo[1].level;
						info.firJob = m_pInfo->memInfo[1].firJob;
						info.rank = m_pInfo->memInfo[1].rank;
						info.sex=m_pInfo->memInfo[1].sex;
						info.line=m_pInfo->memInfo[1].line;
						info.vip=m_pInfo->memInfo[1].vip;
						info.lastlogin=m_pInfo->memInfo[1].lastlogin;
						info.hiscon=m_pInfo->memInfo[1].hiscon ;
						info.hismoney=m_pInfo->memInfo[1].hismoney;
						info.hisgrow=m_pInfo->memInfo[1].hisgrow ;
						info.maxpower=	m_pInfo->memInfo[1].maxpower ;
						pSyn->AddMemberList(info,m_pInfo->ucPage);///这里的页数被作类似使用,1是正常加,2是申请加
					
					}
					break;
				}
			
			}	
			break;
		case SynMemberRemove:
			{
				if(pSyn->Hasmember( m_pInfo->memInfo[1].id))
				{
					pSyn->DelMemberList(m_pInfo->memInfo[1].id);
					sprintf(szWord, "%s离开帮派", m_pInfo->memInfo[1].szName);
					CMsgTalk msg;
					if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
						pSyn->BroadcastSynMsg(&msg);
				}
			}
			break;
		case SynMemberOnlineStatusChange:
			{
				OBJID idUser=ID_NONE;
				UINT  nlevel=0;
				DEBUG_TRY
				///防止出入副本时收到重复的上下线消息
				idUser=m_pInfo->memInfo[1].id;
				CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].id);
				if(pThisUser)
					break;
				if(pSyn->Hasmember( m_pInfo->memInfo[1].id))
				{
					pSyn->SetOnline(m_pInfo->memInfo[1].id,m_pInfo->memInfo[1].line);
// 					if(m_pInfo->memInfo[1].level)
// 						pSyn->incOnline();
// 					else
// 						pSyn->DecOnline();
				}
				///将帮派成员上线消息在本地广播
				pSyn->BroadcastSynMemberInfoMsg(m_pInfo->memInfo[1].id);
//				pSyn->BroadcastSynMemberStatusChangeMsg(m_pInfo->memInfo[1].idUser);
				DEBUG_CATCH3("SynMemberOnlineStatusChange userid:%d-level:%d",idUser,nlevel);
			}
			break;
		case SynMemberKickout:
			{
				CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].id);
				if(pThisUser&&pThisUser->QuerySynAttr()!=NULL)
				{
					pThisUser->QuerySynAttr()->LeaveSyn(true);
					pThisUser->SendSysMsg("你被踢出帮派");
					CMsgSyndicate msgtoc;
					msgtoc.Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_KICK);
					pThisUser->SendMsg(&msgtoc);
				}
				else if(pSyn->Hasmember( m_pInfo->memInfo[1].id))
				{
					::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->memInfo[1].id,1);
					if(info)
					{
						sprintf(szWord, "%s被踢出帮派", info->szName);
						pSyn->DelMemberList(m_pInfo->memInfo[1].id);
						CMsgTalk msg;
						if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
							pSyn->BroadcastSynMsg(&msg);
						
					}
				}
			}
			break;
		case SynProfferChange://从别线过来的帮派成员贡献调整
			{
				::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->memInfo[1].id,1);
				if(info==NULL)
				{
					break;
				}
				else
				{
					///如果已存在,则更新之
					info->id = m_pInfo->memInfo[1].id;
					info->hiscon = m_pInfo->memInfo[1].hiscon;
					info->hisgrow=m_pInfo->memInfo[1].hisgrow;
					info->hismoney=m_pInfo->memInfo[1].hismoney;
					SafeCopy(info->szName,m_pInfo->memInfo[1].szName,16);
				}
			}
			break;
		case SynMemberSetRank:///从别的线过来的帮派等级设置请求
			{
				USHORT usRank=m_pInfo->memInfo[1].rank;
				CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].id);
				if(pThisUser&&pThisUser->QuerySynAttr()!=NULL)
				{
					pThisUser->QuerySynAttr()->SetIdRank(m_pInfo->memInfo[1].id,pSyn->GetID(),usRank,true);
				}
				else
				{
					pSyn->ChgMemberRank(m_pInfo->memInfo[1].id,usRank,true,NULL);
				}
				CMsgSynMemberInfo msgtoc;
				if (msgtoc.Create(SetRank,1,1))
					{
						msgtoc.Append(m_pInfo->memInfo[1].id,"",0,0,0,usRank,0,0,0,0,0,0,0);
						pSyn->BroadcastSynMsg(&msgtoc);
/*							pUser->SendMsg(&msgtoc);*/
					}
			}
			break;
		case SynMemberAnswerNO:
			{
				pSyn->DelMemberList(m_pInfo->memInfo[1].id,2);
				CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].id);
				if(pThisUser)
				{
					pThisUser->SendSysMsg("被%s拒绝",pSyn->GetStr(SYNDATA_NAME));
					this->Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_NO);
					pThisUser->SendMsg(this);
				}
			}
			break;
		case SynMemberAnswerOK:
			{
// 				pSyn->DelMemberList(m_pInfo->memInfo[1].id,3);
// 				CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].id);
// 				if(pThisUser&&pThisUser->QuerySynAttr()!=NULL)
// 				{
// 					pThisUser->QuerySynAttr()->JoinSyn(pSyn->GetID(),false,pThisUser->GetID());
// 				
// 					pThisUser->SendSysMsg("加入%s通过",pSyn->GetStr(SYNDATA_NAME));
// 					
// 					CMsgSyndicate msg;
// 					msg.Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_ALLOW);
// 					pThisUser->SendMsg(&msg);
// 
// 					///广播自己的帮派上线通知给别的线
// 					CMsgSynMemberInfo msgSynMemberInfo;
// 					if (msgSynMemberInfo.Create(SynMemberOnlineStatusChange,1,2))
// 					{
// 						msgSynMemberInfo.Append(pSyn->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
// 						msgSynMemberInfo.Append(pThisUser->GetID(),"",0,0,0,0,CVipList::GetLineByName(MapGroup(PID)->GetServerName()),0,0,0,0,0,0);
// 						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSynMemberInfo,0);
// 					}
// 				}
// 				else
// 					pThisUser->QuerySynAttr()->JoinSyn(pSyn->GetID(),true,pThisUser->GetID());
// 
// 				::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->memInfo[1].id,1);
// 				if(info)
// 				{
// 					sprintf(szWord,"%s加入帮派",info->szName);
// 					CMsgTalk	msg;
// 					if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
// 						pSyn->BroadcastSynMsg(&msg);
// 					///除外转给其它线，还要本地帮派广播一下
// //					pSyn->BroadcastSynMemberInfoMsg(info->nId);
// 					CMsgSynMemberInfo msgMem;
// 					if (msgMem.Create(AddMember,1,1))
// 					{
//  						msgMem.Append(info->id,info->szName,info->level,info->firJob,info->sex,info->rank,info->line,info->vip,info->lastlogin,info->hiscon,info->hismoney,info->hisgrow,info->maxpower);
//  						if (pThisUser)
//  							pSyn->BroadcastSynMsg(&msgMem,pThisUser);
// 						else
// 							pSyn->BroadcastSynMsg(&msgMem);
//  					}
// 				
// 				}
			}
		default:
			break;
	}

	DEBUG_CATCH4("CMsgSynMemberInfo::Process ucActiom:%d-ucMaxAmount:%d-ucAmount:%d",m_pInfo->ucActiom,m_pInfo->ucMaxAmount,m_pInfo->ucAmount);
}
