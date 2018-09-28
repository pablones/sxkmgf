// MsgSynAttrInfo.cpp: interface for the CMsgSynAttrInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "SynAttr.h"
#include "MapGroup.h"
#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgSynAttrInfo::CMsgSynAttrInfo()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgSynAttrInfo::~CMsgSynAttrInfo()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynAttrInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_SYNATTRINFO != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynAttrInfo::Create(SynAttrInfoStruct* pInfo, CSyndicate* pSyn)		// pSyn may be null
{
	// init
	this->Init();

	// fill info now
	m_pInfo->idSyn			= pInfo->idSyn;
	m_pInfo->ucRankShow		= pInfo->ucRankShow;
	m_pInfo->nProffer		= pInfo->nProffer;
	m_pInfo->usMantle		= pInfo->usMantle;
	m_pInfo->ucLevel		= pInfo->ucLevel;

	if(pSyn)
	{
		m_pInfo->dwSyndicateFund	= pSyn->GetInt(SYNDATA_MONEY);
		m_pInfo->dwSyndicatePopulation	= pSyn->GetSynAmount();
// 		m_pInfo->dwSyndicateRepute	 = pSyn->GetInt(SYNDATA_REPUTE);
// 		m_pInfo->ucSynRank = pSyn->GetInt(SYNDATA_RANK);
// 		m_pInfo->nSynProffer = pSyn->GetSynProffer();//20070417修罗:军团贡献度
// 		m_pInfo->idWarSyn = pSyn->GetWarSynID(); //约战军团
// 		m_pInfo->warState = pSyn->GetState();
// 		m_pInfo->mainLev = pSyn->GetMainLev();
// 		m_pInfo->buyLev = pSyn->GetBuyLev();
// 		m_pInfo->chgLev = pSyn->GetChgLev();
// 		m_pInfo->taskLev = pSyn->GetTaskLev();
		SafeCopy(m_pInfo->szLeader, pSyn->GetStr(SYNDATA_LEADERNAME), _MAX_NAMESIZE);
		SafeCopy(m_pInfo->szSyndicateName, pSyn->GetStr(SYNDATA_NAME), _MAX_NAMESIZE);
	}
	else
	{
		m_pInfo->dwSyndicateFund		= 0;
		m_pInfo->dwSyndicatePopulation	= 0;
		m_pInfo->szLeader[0]			= 0;
		m_pInfo->dwSyndicateRepute = 0;
		m_pInfo->ucSynRank = 0;
		m_pInfo->nSynProffer = 0;//20070417修罗:军团贡献度
		m_pInfo->idWarSyn = 0; //约战军团
		m_pInfo->warState = 0;
		m_pInfo->mainLev = 0;
		m_pInfo->buyLev = 0;
		m_pInfo->chgLev = 0;
		m_pInfo->taskLev = 0;
	}

	m_unMsgType	=_MSG_SYNATTRINFO;
	m_unMsgSize	=sizeof(MSG_Info);

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgSynAttrInfo::Process(void *pInfo)
{
	ASSERT(!"CMsgSynAttrInfo::Process()");
	return;

#ifdef _MSGDEBUG
	::LogMsg("Process CMsgSynAttrInfo, id:%u", m_info.id);
#endif
}
