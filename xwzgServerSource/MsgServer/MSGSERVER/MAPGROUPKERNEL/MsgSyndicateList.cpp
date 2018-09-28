// MsgSyndicateList.cpp: implementation of the CMsgSyndicateList class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "basefunc.h"
#include "MapGroup.h"
#include "SynManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSyndicateList::CMsgSyndicateList()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgSyndicateList::~CMsgSyndicateList()
{

}

BOOL CMsgSyndicateList::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_SYNLIST != this->GetType())
		return false;
	
	return true;
}


BOOL CMsgSyndicateList::Create(unsigned short usAction,OBJID idSyn,const char* szName,const char* szLeaderName,int nLev , int nAmount)
{
	this->Init();
	
	// fill info now
	m_unMsgType		= _MSG_SYNLIST;
	m_unMsgSize		= sizeof(MSG_Info) - sizeof(synMsgData);

	m_pInfo->usAction	=usAction;
	m_pInfo->usData	= 0;

	if(strlen(szName) > 0)
	{
		m_unMsgSize		+= sizeof(synMsgData);
		m_pInfo->usData ++;
		m_pInfo->m_data[0].idSyn  = idSyn;
		m_pInfo->m_data[0].lev = nLev;
		m_pInfo->m_data[0].amount = nAmount;
		SafeCopy(m_pInfo->m_data[0].szName,szName,16);
		SafeCopy(m_pInfo->m_data[0].szLeaderName,szLeaderName,16);
	}
	return true;
}
BOOL CMsgSyndicateList::Append(OBJID idSyn,const char* szName,const char* szLeaderName,int nLev , int nAmount)
{
	CHECKF(m_unMsgSize + sizeof(synMsgData) < MAX_PACKETSIZE);

	m_unMsgSize		+= sizeof(synMsgData);
	m_pInfo->m_data[m_pInfo->usData].idSyn  = idSyn;
	m_pInfo->m_data[m_pInfo->usData].lev	= nLev;
	m_pInfo->m_data[m_pInfo->usData].amount	= nAmount;
	SafeCopy(m_pInfo->m_data[m_pInfo->usData].szName,szName,16);
	SafeCopy(m_pInfo->m_data[m_pInfo->usData].szLeaderName,szLeaderName,16);
	m_pInfo->usData++;

	return true;

}

void CMsgSyndicateList::SetMaxData(int data)
{
	m_pInfo->usData += data * 100;
}

void CMsgSyndicateList::Process		(CGameSocket* pSocket)
{
}
