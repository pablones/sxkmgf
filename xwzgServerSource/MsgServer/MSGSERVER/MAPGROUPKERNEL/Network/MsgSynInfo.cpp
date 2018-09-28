// MsgSynAttrInfo.cpp: interface for the CMsgSynInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Syndicate.h"
#include "MapGroup.h"
#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgSynInfo::CMsgSynInfo()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_pSubInfo	=(SUBMSG_INFO *)m_bufMsg;
	m_pNewInfo	=(NEWMSG_INFO *)m_bufMsg;
	m_pSciInfo	=(SCIMSG_INFO *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
	m_StrPacker2.SetBuf(m_pNewInfo->szBuf, _MAX_MSGSIZE-sizeof(NEWMSG_INFO)+1);
}

CMsgSynInfo::~CMsgSynInfo()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_SYNINFO != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynInfo::Create(CSyndicate* pSyn)		// pSyn may be null
{
	// init
	CHECKF(pSyn);
	this->Init();

	m_pInfo->idSyn = pSyn->GetID();
	m_pInfo->level = pSyn->GetInt(SYNDATA_LEVEL);
	m_pInfo->affluent = pSyn->GetInt(SYNDATA_ACTIVEAMOUNT);
	m_pInfo->amount = pSyn->GetMemberAmount();
	m_pInfo->shopAmount = pSyn->GetShopMemberAmount();
	m_pInfo->nMove = pSyn->GetInt(SYNDATA_DATA1);
	m_pInfo->dwMoney = pSyn->GetInt(SYNDATA_MONEY);
	m_pInfo->dwCreateTime = pSyn->GetInt(SYNDATA_CREATETIME);
	m_pInfo->state = pSyn->GetInt(SYNDATA_STATE);
	m_pInfo->onlineUser = pSyn->GetOnlineAmount();
	m_pInfo->nBuild = pSyn->GetInt(SYNDATA_GROW);
	m_StrPacker.AddString(pSyn->GetStr(SYNDATA_NAME));
	m_StrPacker.AddString(pSyn->GetStr(SYNDATA_LEADERNAME));
	m_StrPacker.AddString(pSyn->GetStr(SYNDATA_CREATELEADERNAME));
	m_StrPacker.AddString(pSyn->GetStr(SYNDATA_ANNOUNCE));

	m_unMsgType	=_MSG_SYNINFO;
	m_unMsgSize = sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	CHECKF(m_unMsgSize < MAX_PACKETSIZE);
	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgSynInfo::Process(void *pInfo)
{
	ASSERT(!"CMsgSynInfo::Process()");
	return;

#ifdef _MSGDEBUG
	::LogMsg("Process CMsgSynInfo, id:%u", m_info.id);
#endif
}
BOOL CMsgSynInfo::CreateSci	(CSyndicate* pSyn)
{
	CHECKF(pSyn);
	this->Init();
	m_pSciInfo->sub1=pSyn->GetLev(1);
	m_pSciInfo->sub2=pSyn->GetLev(2);
	m_pSciInfo->sub3=pSyn->GetLev(3);
	m_pSciInfo->sub4=pSyn->GetLev(4);
	m_unMsgType	=_MSG_SUBINFO;
	m_unMsgSize = sizeof(SUBMSG_INFO)-1+m_StrPacker.GetSize();
	CHECKF(m_unMsgSize < MAX_PACKETSIZE);
	return TRUE;
}
BOOL CMsgSynInfo::CreateUpdate(OBJID nAction,OBJID nValue,OBJID nValue2/*=0*/,OBJID nValue3/*=0*/)
{
	this->Init();
	m_pSciInfo->sub1=nAction;
	m_pSciInfo->sub2=nValue;
	m_pSciInfo->sub3=nValue2;
	m_pSciInfo->sub4=nValue3;
	m_unMsgType	=_MSG_SUBINFO;
	m_unMsgSize = sizeof(SUBMSG_INFO)-1+m_StrPacker.GetSize();
	CHECKF(m_unMsgSize < MAX_PACKETSIZE);
	return TRUE;
}
BOOL	CMsgSynInfo::CreateInfo(CSyndicate *pSyn)
{
	CHECKF(pSyn);
	this->Init();
	m_pNewInfo->Synid=pSyn->GetID();//帮id
	m_pNewInfo->Level=pSyn->GetLev();//帮等级
	m_pNewInfo->Rank=0;
	m_pNewInfo->Online=pSyn->GetOnlineAmount();//上线人数
	m_pNewInfo->Amount=pSyn->GetMemberAmount();//帮会人数
	m_pNewInfo->HisCon=pSyn->GetInt(SYNDATA_CREATETIME);
	m_pNewInfo->NowCon=0;
	m_pNewInfo->Money=pSyn->GetMoney();//帮会资金
	m_pNewInfo->Contribut=pSyn->GetGrow();//帮会建设
	m_StrPacker2.AddString(pSyn->GetStr(SYNDATA_NAME));
	m_StrPacker2.AddString(pSyn->GetStr(SYNDATA_LEADERNAME));
	m_StrPacker2.AddString("0");
	m_StrPacker2.AddString("0");
	m_StrPacker2.AddString(pSyn->GetStr(SYNDATA_ANNOUNCE));
	if (pSyn->NoPay())
		m_StrPacker2.AddString("1");
	else
		m_StrPacker2.AddString("0");
	m_unMsgType	=_MSG_SYNINFO;
	m_unMsgSize = sizeof(NEWMSG_INFO)-1+m_StrPacker2.GetSize();
	CHECKF(m_unMsgSize < MAX_PACKETSIZE);
	return true;
}
bool CMsgSynInfo::CreateSub(CSyndicate* pSyn)
{
	CHECKF(pSyn);
	this->Init();

	m_pSubInfo->sub1_lev		= pSyn->GetInt(SYNDATA_SUBLEV1)			;
	m_pSubInfo->sub1_skill1		= pSyn->GetInt(SYNDATA_SUB1_SKILL1)		;
	m_pSubInfo->sub1_skill2		= pSyn->GetInt(SYNDATA_SUB1_SKILL2)		;
	m_pSubInfo->sub1_skill3		= pSyn->GetInt(SYNDATA_SUB1_SKILL3)		;
	m_pSubInfo->sub1_Builder	= pSyn->GetInt(SYNDATA_SUB1_BUILDER)	;
	m_pSubInfo->sub1_Kongfu		= pSyn->GetInt(SYNDATA_SUB1_KONGFU)		;
	m_pSubInfo->sub1_Busman		= pSyn->GetInt(SYNDATA_SUB1_BUSMAN)		;
	m_pSubInfo->sub1_Growing	= pSyn->GetInt(SYNDATA_SUBGROW1)		;

	m_pSubInfo->sub2_lev		= pSyn->GetInt(SYNDATA_SUBLEV2)			;
	m_pSubInfo->sub2_skill1		= pSyn->GetInt(SYNDATA_SUB2_SKILL1)		;
	m_pSubInfo->sub2_skill2		= pSyn->GetInt(SYNDATA_SUB2_SKILL2)		;
	m_pSubInfo->sub2_skill3		= pSyn->GetInt(SYNDATA_SUB2_SKILL3)		;
	m_pSubInfo->sub2_Builder	= pSyn->GetInt(SYNDATA_SUB2_BUILDER)	;
	m_pSubInfo->sub2_Kongfu		= pSyn->GetInt(SYNDATA_SUB2_KONGFU)		;
	m_pSubInfo->sub2_Busman		= pSyn->GetInt(SYNDATA_SUB2_BUSMAN)		;
	m_pSubInfo->sub2_Growing	= pSyn->GetInt(SYNDATA_SUBGROW2)		;

	m_pSubInfo->sub3_lev		= pSyn->GetInt(SYNDATA_SUBLEV3)			;
	m_pSubInfo->sub3_skill1		= pSyn->GetInt(SYNDATA_SUB3_SKILL1)		;
	m_pSubInfo->sub3_skill2		= pSyn->GetInt(SYNDATA_SUB3_SKILL2)		;
	m_pSubInfo->sub3_skill3		= pSyn->GetInt(SYNDATA_SUB3_SKILL3)		;
	m_pSubInfo->sub3_Builder	= pSyn->GetInt(SYNDATA_SUB3_BUILDER)	;
	m_pSubInfo->sub3_Kongfu		= pSyn->GetInt(SYNDATA_SUB3_KONGFU)		;
	m_pSubInfo->sub3_Busman		= pSyn->GetInt(SYNDATA_SUB3_BUSMAN)		;
	m_pSubInfo->sub3_Growing	= pSyn->GetInt(SYNDATA_SUBGROW3)		;

	m_pSubInfo->sub4_lev		= pSyn->GetInt(SYNDATA_SUBLEV4)			;
	m_pSubInfo->sub4_skill1		= pSyn->GetInt(SYNDATA_SUB4_SKILL1)		;
	m_pSubInfo->sub4_skill2		= pSyn->GetInt(SYNDATA_SUB4_SKILL2)		;
	m_pSubInfo->sub4_skill3		= pSyn->GetInt(SYNDATA_SUB4_SKILL3)		;
	m_pSubInfo->sub4_Builder	= pSyn->GetInt(SYNDATA_SUB4_BUILDER)	;
	m_pSubInfo->sub4_Kongfu		= pSyn->GetInt(SYNDATA_SUB4_KONGFU)		;
	m_pSubInfo->sub4_Busman		= pSyn->GetInt(SYNDATA_SUB4_BUSMAN)		;
	m_pSubInfo->sub4_Growing	= pSyn->GetInt(SYNDATA_SUBGROW4)		;

	m_pSubInfo->sub_Stop		= pSyn->GetInt(SYNDATA_SUB1_START)		;
	m_pSubInfo->Pay				= pSyn->GetInt(SYNDATA_PAY)				;

	
	m_unMsgType	=_MSG_SUBINFO;
	m_unMsgSize = sizeof(SUBMSG_INFO)-1+m_StrPacker.GetSize();
	CHECKF(m_unMsgSize < MAX_PACKETSIZE);
	return true;
}
BOOL	CMsgSynInfo::CreateNew(CSyndicate* pSyn,CUserPtr pUser)
{
	CHECKF(pSyn);
	this->Init();
	m_pNewInfo->Synid=pSyn->GetID();//帮id
	m_pNewInfo->Level=pSyn->GetInt(SYNDATA_LEVEL);//帮等级
	m_pNewInfo->Rank=pUser->GetMySynRank();//职位
	m_pNewInfo->Online=pSyn->GetOnlineAmount();//上线人数
	m_pNewInfo->Amount=pSyn->GetInt(SYNDATA_AMOUNT);//帮会人数
	m_pNewInfo->HisCon=pUser->GetHistorySynCon();//历史帮贡
	m_pNewInfo->NowCon=pUser->GetSynCon();//现在帮贡
	m_pNewInfo->Money=pSyn->GetInt(SYNDATA_MONEY);//帮会资金
	m_pNewInfo->Contribut=pSyn->GetInt(SYNDATA_GROW);//帮会建设
	m_StrPacker2.AddString(pSyn->GetStr(SYNDATA_NAME));
	m_StrPacker2.AddString(pSyn->GetStr(SYNDATA_LEADERNAME));
	m_StrPacker2.AddString(pSyn->GetSubLeaderID()==0?"0":pUser->GetNameByID(pSyn->GetSubLeaderID()));
	m_StrPacker2.AddString(pSyn->GetSubLeader2ID()==0?"0":pUser->GetNameByID(pSyn->GetSubLeader2ID()));
	m_StrPacker2.AddString(pSyn->GetStr(SYNDATA_ANNOUNCE));
	if (pSyn->NoPay())
		m_StrPacker2.AddString("1");
	else
		m_StrPacker2.AddString("0");
	m_unMsgType	=_MSG_SYNINFO;
	m_unMsgSize = sizeof(NEWMSG_INFO)-1+m_StrPacker2.GetSize();
	CHECKF(m_unMsgSize < MAX_PACKETSIZE);
	return true;
}
