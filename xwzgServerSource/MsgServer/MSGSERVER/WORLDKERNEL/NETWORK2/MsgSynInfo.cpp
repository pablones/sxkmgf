// MsgSynAttrInfo.cpp: interface for the CMsgSynInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
//#include "Syndicate.h"
//#include "MapGroup.h"
#pragma	warning(disable:4786)
#include "MsgSynInfo.h"
//#include "SynManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgSynInfo::CMsgSynInfo()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
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
// BOOL CMsgSynInfo::Create(CSyndicate* pSyn)		// pSyn may be null
// {
// 	// init
// 	this->Init();
// 
// 	m_pInfo->idSyn = pSyn->GetID();
// 	m_pInfo->level = pSyn->GetInt(SYNDATA_LEVEL);
// 	m_pInfo->affluent = 100;
// 	m_pInfo->amount = pSyn->GetMemberAmount();
// 	m_pInfo->shopAmount = pSyn->GetShopMemberAmount();
// 	m_pInfo->nBuld = pSyn->GetInt(SYNDATA_GROW);
// 	m_pInfo->nMove = pSyn->GetInt(SYNDATA_DATA1);
// 	m_pInfo->dwMoney = pSyn->GetInt(SYNDATA_MONEY);
// 	m_pInfo->dwCreateTime = pSyn->GetInt(SYNDATA_CREATETIME);
// 	m_pInfo->state = pSyn->GetInt(SYNDATA_STATE);
// 	m_pInfo->onlineUser = pSyn->GetOnlineAmount();
// 
// 	m_StrPacker.AddString(pSyn->GetStr(SYNDATA_NAME));
// 	m_StrPacker.AddString(pSyn->GetStr(SYNDATA_LEADERNAME));
// 	m_StrPacker.AddString(pSyn->GetStr(SYNDATA_CREATELEADERNAME));
// 	m_StrPacker.AddString(pSyn->GetStr(SYNDATA_ANNOUNCE));
// 
// 	m_unMsgType	=_MSG_SYNINFO;
// 	m_unMsgSize = sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// 	CHECKF(m_unMsgSize < MAX_PACKETSIZE);
// 	return true;
// }

//////////////////////////////////////////////////////////////////////
void CMsgSynInfo::Process(void *pInfo)
{

}
