// MsgNewCard.cpp: implementation of the CMsgNewCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgNewCard.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgNewCard::CMsgNewCard()
{
	Init();///³õÊ¼»¯°ü
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgNewCard::~CMsgNewCard()
{

}

BOOL CMsgNewCard::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_NEWCARD != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgNewCard::Create(int nType,OBJID idUser,OBJID idAccount, const char* pszCard)
{
	if (nType == NEWCARDACT_NONE || !pszCard)
		return false;
	
	this->Init();
	
	m_pInfo->dwType	= nType;
	m_pInfo->idUser = idUser;
	m_pInfo->idAccount = idAccount;
	strcpy(m_pInfo->szBuf,pszCard);
	
	m_unMsgType	=_MSG_NEWCARD;
	m_unMsgSize	=sizeof(MSG_Info);
	
	return true;
}
