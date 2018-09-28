// MsgSynMemberInfo.cpp: interface for the CMsgSynMemberInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
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
// 	switch(m_pInfo->ucActiom)
// 	{
// 		///这里是别的线发过来的帮派发送成功的消息
// 		case SynMemberAdd:
// 			{
// 				///这个消息里面只允许存在两个成员
// 				if (m_pInfo->ucAmount==2)
// 				{
// 					OBJID idSyn=m_pInfo->memInfo[0].idUser;
// 					CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// 					if(pSyn==NULL)///如果找不到这个帮派尝试重新构建一下
// 					{
// 						SynManager()->Create(idSyn);///请求帮派管理器在本地创建一个帮派实例
// 						pSyn = SynManager()->QuerySyndicate(idSyn);
// 					}
// 				}
// // 				if(pSyn)
// // 					pSyn->BroadcastLocalSynMsg(this);
// 			}	
// 			break;
// 		case SynMemberRemove:
// 			break;
// 		default:
// 			break;
// 	}

}
