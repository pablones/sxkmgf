// MsgSynMemberInfo.cpp: interface for the CMsgSynMemberInfo class.
//
//////////////////////////////////////////////////////////////////////

//#include "AllMsg.h"
//#include "SynAttr.h"
//#include "MapGroup.h"
#include "MsgSynMemberInfo.h"

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
// BOOL CMsgSynMemberInfo::Append(OBJID idUser,USHORT level,USHORT firJob,USHORT firJobLev,USHORT rank,DWORD proff,const char* szName,int nOnline)
// {
// 	CHECKF(m_unMsgSize + sizeof(MemberInfo) < MAX_PACKETSIZE);
// 
// 	m_unMsgType	=_MSG_SYNMEMBERINFO;
// 	m_unMsgSize	+= sizeof(MemberInfo);
// 
// 	m_pInfo->memInfo[m_pInfo->ucAmount].idUser = idUser;
// 	m_pInfo->memInfo[m_pInfo->ucAmount].level = level;
// 	m_pInfo->memInfo[m_pInfo->ucAmount].firJob = firJob;
// 	m_pInfo->memInfo[m_pInfo->ucAmount].firJobLev = firJobLev;
// 	m_pInfo->memInfo[m_pInfo->ucAmount].rank = rank;
// 	m_pInfo->memInfo[m_pInfo->ucAmount].proff = proff;
// 	m_pInfo->memInfo[m_pInfo->ucAmount].online = nOnline;
// 	SafeCopy(m_pInfo->memInfo[m_pInfo->ucAmount].szName,szName,16);
// 	m_pInfo->ucAmount ++;
// 
// 	return true;
// }

//////////////////////////////////////////////////////////////////////
// void CMsgSynMemberInfo::Process(void *pInfo)
// {
// 	char szWord[1024]="";
// 	///这个消息里面只允许存在两个成员
// 	if (m_pInfo->ucAmount!=2)
// 			return;
// 	OBJID idSyn=m_pInfo->memInfo[0].idUser;
// 	CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// 	if(pSyn==NULL)///如果找不到这个帮派尝试重新构建一下
// 	{
// 		SynManager()->Create(idSyn);///请求帮派管理器在本地创建一个帮派实例
// 		pSyn = SynManager()->QuerySyndicate(idSyn);
// 	}
// 	if(pSyn==NULL)
// 		return;
// 
// 	switch(m_pInfo->ucActiom)
// 	{
// 		///这里是别的线发过来的帮派发送成功的消息
// 		case SynMemberAdd:
// 			{
// 				::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->memInfo[1].idUser,1);
// 				if(info==NULL)
// 				{
// 					::MemberInfo info={0};
// 					info.nId = m_pInfo->memInfo[1].idUser;
// 					info.nRank = m_pInfo->memInfo[1].rank;
// 					info.nProffer = m_pInfo->memInfo[1].proff;
// 					info.nLevel = m_pInfo->memInfo[1].level;
// 					info.firJob = m_pInfo->memInfo[1].firJob;
// 					info.firLevel = m_pInfo->memInfo[1].firJobLev;
// 					info.nOnline = m_pInfo->memInfo[1].online;
// 					SafeCopy(info.szName,m_pInfo->memInfo[1].szName,16);
// 					pSyn->AddMemberList(info,m_pInfo->ucPage);///这里的页数被作类似使用,1是正常加,2是申请加
// 				}
// 				else
// 				{
// 					///如果已存在,则更新之
// 					info->nId = m_pInfo->memInfo[1].idUser;
// 					info->nRank = m_pInfo->memInfo[1].rank;
// 					info->nProffer = m_pInfo->memInfo[1].proff;
// 					info->nLevel = m_pInfo->memInfo[1].level;
// 					info->firJob = m_pInfo->memInfo[1].firJob;
// 					info->firLevel = m_pInfo->memInfo[1].firJobLev;
// 					info->nOnline = m_pInfo->memInfo[1].online;
// 					SafeCopy(info->szName,m_pInfo->memInfo[1].szName,16);
// 				}
// 			}	
// 			break;
// 		case SynMemberRemove:
// 			{
// 				if(pSyn->Hasmember( m_pInfo->memInfo[1].idUser))
// 				{
// 					pSyn->DelMemberList(m_pInfo->memInfo[1].idUser);
// 
// 					sprintf(szWord, "%s离开帮派", m_pInfo->memInfo[1].szName);
// 					CMsgTalk msg;
// 					if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
// 						pSyn->BroadcastSynMsg(&msg,NULL);
// 				}
// 			}
// 			break;
// 		case SynMemberOnlineStatusChange:
// 			{
// 				if(pSyn->Hasmember( m_pInfo->memInfo[1].idUser))
// 				{
// 					pSyn->SetOnline(m_pInfo->memInfo[1].idUser,m_pInfo->memInfo[1].level);
// // 					if(m_pInfo->memInfo[1].level)
// // 						pSyn->incOnline();
// // 					else
// // 						pSyn->DecOnline();
// 				}
// 				///将帮派成员上线消息在本地广播
// 				pSyn->BroadcastSynMemberInfoMsg(m_pInfo->memInfo[1].idUser);
// //				pSyn->BroadcastSynMemberStatusChangeMsg(m_pInfo->memInfo[1].idUser);
// 
// 			}
// 			break;
// 		case SynMemberKickout:
// 			{
// 				CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].idUser);
// 				if(pThisUser)
// 				{
// 					pThisUser->SendSysMsg("你被踢出帮派");
// 					
// 					pThisUser->QuerySynAttr()->LeaveSyn(true);
// // 					CMsgSyndicate msg;
// // 					msg.Create(SYN_UPDATE,pThisUser->GetID());
// // 					pThisUser->SendMsg(&msg);
// 				}
// 				else if(pSyn->Hasmember( m_pInfo->memInfo[1].idUser))
// 				{
// 					::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->memInfo[1].idUser,1);
// 					if(info)
// 					{
// 						sprintf(szWord, "%s被踢出帮派", info->szName);
// 						CMsgTalk msg;
// 						if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
// 							pSyn->BroadcastSynMsg(&msg,NULL);
// 						pSyn->DelMemberList(m_pInfo->memInfo[1].idUser);
// 					}
// 				}
// //				pSyn->DecOnline();
// 			}
// 			break;
// 		case SynMemberSetRank:///从别的线过来的帮派等级设置请求
// 			{
// 				CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].idUser);
// 				USHORT usRank=m_pInfo->memInfo[1].level;
// 				if(pThisUser)
// 				{
// 					if(pThisUser->QuerySynAttr()->SetIdRank(m_pInfo->memInfo[1].idUser,pSyn->GetID(),usRank))
// 					{
// 						CMsgSyndicate msg;
// 						msg.Create(SYN_UPDATE,m_pInfo->memInfo[1].idUser);
// 						pThisUser->SendMsg(&msg);
// //						pThisUser->SendSysMsg("任命成功");
// 					}
// 				}
// 				else
// 				{
// 					pSyn->ChgMemberRank(m_pInfo->memInfo[1].idUser,usRank,true,NULL);
// 
// 				}
// // 				if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),m_pInfo->dwData))
// // 				{
// // 					this->Create(SYN_UPDATE,m_pInfo->idTarget);
// // 					pUser->SendMsg(this);
// // 					pUser->SendSysMsg("任命成功");
// // 					
// // 					///任命成功通知其它线更新
// // 					///批准申请入帮，通知其它线更新数据
// // 					CMsgSynMemberInfo msg;
// // 					if (!msg.Create(SynMemberSetRank,2,2))
// // 						break;
// // 					msg.Append(pSyn->GetID(),0,0,0,0,0,"",0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
// // 					msg.Append(m_pInfo->idTarget,0,0,0,0,0,"",1);
// // 					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,0);
// // 				}
// 			}
// 			break;
// 		case SynMemberAnswerNO:
// 			{
// 				pSyn->DelMemberList(m_pInfo->memInfo[1].idUser,2);
// 				CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].idUser);
// 				if(pThisUser)
// 				{
// 					pThisUser->SendSysMsg("被%s拒绝",pSyn->GetStr(SYNDATA_NAME));
// 				}				
// 			}
// 			break;
// 		case SynMemberAnswerOK:
// 			{
// 				pSyn->DelMemberList(m_pInfo->memInfo[1].idUser,3);
// 				CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->memInfo[1].idUser);
// 				if(pThisUser)
// 				{
// 					pThisUser->QuerySynAttr()->JoinSyn(pSyn->GetID(),RANK_NORMAL,0);
// 
// 					pThisUser->SendSysMsg("加入%s通过",pSyn->GetStr(SYNDATA_NAME));
// 					
// 					CMsgSyndicate msg;
// 					msg.Create(SYN_UPDATE,pThisUser->GetID());
// 					pThisUser->SendMsg(this);
// 
// 					///广播自己的帮派上线通知给别的线
// 					CMsgSynMemberInfo msgSynMemberInfo;
// 					if (msgSynMemberInfo.Create(SynMemberOnlineStatusChange,1,2))
// 					{
// 						msgSynMemberInfo.Append(pSyn->GetID(),0,0,0,0,0,"",0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
// 						msgSynMemberInfo.Append(pThisUser->GetID(),SynMemberOnline,0,0,0,0,"",1);
// 						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSynMemberInfo,0);
// 					}
// 				}
// 				::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->memInfo[1].idUser,1);
// 				if(info)
// 				{
// 					sprintf(szWord,"%s加入帮派",info->szName);
// 
// 					///除外转给其它线，还要本地帮派广播一下
// //					pSyn->BroadcastSynMemberInfoMsg(info->nId);
// 					CMsgSynMemberInfo msgMem;
// 					if (msgMem.Create(3,1,1))
// 					{
// 						msgMem.Append(info->nId,info->nLevel,info->firJob,info->firLevel,info->nRank,info->nProffer,info->szName,info->nOnline);
// 						pSyn->BroadcastSynMsg(&msgMem,NULL);
// 					}
// 					CMsgTalk	msg;
// 					if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
// 						pSyn->BroadcastSynMsg(&msg,NULL);
// 				}
// 			}
// 		default:
// 			break;
// 	}
// 
// }
