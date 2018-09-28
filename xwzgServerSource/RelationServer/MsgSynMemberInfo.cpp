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
// 	///�����Ϣ����ֻ�������������Ա
// 	if (m_pInfo->ucAmount!=2)
// 			return;
// 	OBJID idSyn=m_pInfo->memInfo[0].idUser;
// 	CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// 	if(pSyn==NULL)///����Ҳ���������ɳ������¹���һ��
// 	{
// 		SynManager()->Create(idSyn);///������ɹ������ڱ��ش���һ������ʵ��
// 		pSyn = SynManager()->QuerySyndicate(idSyn);
// 	}
// 	if(pSyn==NULL)
// 		return;
// 
// 	switch(m_pInfo->ucActiom)
// 	{
// 		///�����Ǳ���߷������İ��ɷ��ͳɹ�����Ϣ
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
// 					pSyn->AddMemberList(info,m_pInfo->ucPage);///�����ҳ����������ʹ��,1��������,2�������
// 				}
// 				else
// 				{
// 					///����Ѵ���,�����֮
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
// 					sprintf(szWord, "%s�뿪����", m_pInfo->memInfo[1].szName);
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
// 				///�����ɳ�Ա������Ϣ�ڱ��ع㲥
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
// 					pThisUser->SendSysMsg("�㱻�߳�����");
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
// 						sprintf(szWord, "%s���߳�����", info->szName);
// 						CMsgTalk msg;
// 						if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
// 							pSyn->BroadcastSynMsg(&msg,NULL);
// 						pSyn->DelMemberList(m_pInfo->memInfo[1].idUser);
// 					}
// 				}
// //				pSyn->DecOnline();
// 			}
// 			break;
// 		case SynMemberSetRank:///�ӱ���߹����İ��ɵȼ���������
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
// //						pThisUser->SendSysMsg("�����ɹ�");
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
// // 					pUser->SendSysMsg("�����ɹ�");
// // 					
// // 					///�����ɹ�֪ͨ�����߸���
// // 					///��׼������֪ͨ�����߸�������
// // 					CMsgSynMemberInfo msg;
// // 					if (!msg.Create(SynMemberSetRank,2,2))
// // 						break;
// // 					msg.Append(pSyn->GetID(),0,0,0,0,0,"",0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
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
// 					pThisUser->SendSysMsg("��%s�ܾ�",pSyn->GetStr(SYNDATA_NAME));
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
// 					pThisUser->SendSysMsg("����%sͨ��",pSyn->GetStr(SYNDATA_NAME));
// 					
// 					CMsgSyndicate msg;
// 					msg.Create(SYN_UPDATE,pThisUser->GetID());
// 					pThisUser->SendMsg(this);
// 
// 					///�㲥�Լ��İ�������֪ͨ�������
// 					CMsgSynMemberInfo msgSynMemberInfo;
// 					if (msgSynMemberInfo.Create(SynMemberOnlineStatusChange,1,2))
// 					{
// 						msgSynMemberInfo.Append(pSyn->GetID(),0,0,0,0,0,"",0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
// 						msgSynMemberInfo.Append(pThisUser->GetID(),SynMemberOnline,0,0,0,0,"",1);
// 						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSynMemberInfo,0);
// 					}
// 				}
// 				::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->memInfo[1].idUser,1);
// 				if(info)
// 				{
// 					sprintf(szWord,"%s�������",info->szName);
// 
// 					///����ת�������ߣ���Ҫ���ذ��ɹ㲥һ��
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
