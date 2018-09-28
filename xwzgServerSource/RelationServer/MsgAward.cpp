// MsgAward.cpp: implementation of the CMsgAward class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgAward.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAward::CMsgAward()
{
	Init();///³õÊ¼»¯°ü
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgAward::~CMsgAward()
{

}

BOOL CMsgAward::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_GAINAWARDS != this->GetType())
		return false;

	return true;
}
