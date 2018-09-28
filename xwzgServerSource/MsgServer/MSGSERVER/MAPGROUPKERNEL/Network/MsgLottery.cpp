// MsgLottery.cpp: implementation of the CMsgLottery class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgLottery.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgLottery::CMsgLottery()
{
	this->Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgLottery::~CMsgLottery()
{

}
//////////////////////////////////////////////////////////////////////
BOOL CMsgLottery::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return FALSE;
	
	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgLottery::Create(int nAction, OBJID idUser)
{
	if (idUser==ID_NONE)
		return FALSE;
	// init
	this->Init();
	
	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_LOTTERY;
	
	m_pInfo->uAction	= nAction;
	m_pInfo->idUser		= idUser;
	m_pInfo->uAmount	= 0;
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgLottery::Append(OBJID idTreasure, OBJID itemType, USHORT usAmount, USHORT usDivide)
{
	m_unMsgSize += sizeof(TreasureInfo);
	
	TreasureInfo info;
	memset(&info, 0L, sizeof(TreasureInfo));
	info.idTreasure = idTreasure;
	info.itemType	= itemType;
	info.usAmount	= usAmount;
	info.usDivide	= usDivide;
	memcpy(&m_pInfo->treasureInfo[m_pInfo->uAmount],&info,sizeof(TreasureInfo));
	m_pInfo->uAmount++;
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
void CMsgLottery::Process(void* pInfo)
{
}