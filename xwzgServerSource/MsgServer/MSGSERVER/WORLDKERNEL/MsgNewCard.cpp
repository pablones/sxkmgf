// MsgNewCard.cpp: implementation of the CMsgNewCard class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "WorldKernel.h"
using namespace world_kernel;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgNewCard_w::CMsgNewCard_w()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
}

CMsgNewCard_w::~CMsgNewCard_w()
{
	
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgNewCard_w::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_NEWCARD != this->GetType())
		return false;
	
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgNewCard_w::Create(int nType,OBJID idUser,OBJID idAccount, const char* pszCard)
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

void CMsgNewCard_w::Process(void *pInfo)
{
	GameWorld()->SendNewCardRequest(m_unMsgType, m_unMsgSize, m_pInfo->idUser, m_pInfo->idAccount, m_pInfo->dwType, m_pInfo->szBuf);
}