// MsgRelay.cpp: implementation of the CMsgRelay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgRelay.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgRelay::CMsgRelay()
{
	Init();///��ʼ����
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgRelay::~CMsgRelay()
{

}

///�������
BOOL CMsgRelay::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;
	
	if(_MSG_RELAY != this->GetType())
		return false;
	
	return true;
}