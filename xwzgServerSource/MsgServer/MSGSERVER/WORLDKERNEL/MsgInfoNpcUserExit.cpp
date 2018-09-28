// MsgInfoNpcUserExit.cpp: implementation of the CMsgInfoNpcUserExit class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgInfoNpcUserExit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgInfoNpcUserExit::CMsgInfoNpcUserExit()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgSize = sizeof(MSG_Info);
}

CMsgInfoNpcUserExit::~CMsgInfoNpcUserExit()
{

}

BOOL CMsgInfoNpcUserExit::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_INFONPCUSEREXIT != this->GetType())
		return false;

	return true;
}

BOOL CMsgInfoNpcUserExit::Create(UINT idUser)
{
	m_unMsgType = _MSG_INFONPCUSEREXIT;
	m_pInfo->idUser = idUser;
	return TRUE;
}