// MsgWeapensoulShow.cpp: implementation of the CMsgWeapensoulShow class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgWeapensoulShow.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWeapensoulShow::CMsgWeapensoulShow()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgWeapensoulShow::~CMsgWeapensoulShow()
{

}

BOOL CMsgWeapensoulShow::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_WEAPONSOULSHOW != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgWeapensoulShow::Create(ST_WEAPONSOULDATA stWSLData,int nAction/* = EM_MSGWPSLSHOW_IN*/,OBJID idUser/*=ID_NONE*/,OBJID idItem/*=ID_NONE*/)
{
	// init
	this->Init();
	
	// fill info now
	m_pInfo->idItem = idItem;
	m_pInfo->idUser = idUser;
	m_pInfo->dwAction = nAction;

	m_pInfo->usType = stWSLData.nType;
	m_pInfo->usLevel = stWSLData.level;
	m_pInfo->usGrow = stWSLData.grow;
	m_pInfo->usMixLev = stWSLData.mixlev;

	m_unMsgType	=_MSG_WEAPONSOULSHOW;
	m_unMsgSize	=sizeof(MSG_Info);
	return true;
}

//--------------------------------
void CMsgWeapensoulShow::Process(void *pInfo)
{
}
