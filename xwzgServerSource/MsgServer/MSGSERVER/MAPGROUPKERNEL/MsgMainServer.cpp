// MsgMainServer.cpp: implementation of the CMsgMainServer class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgMainServer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgMainServer::CMsgMainServer()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgMainServer::~CMsgMainServer()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMainServer::Create(OBJID dwMainServer, DWORD dwData, bool bExtend)
{
	// init
	this->Init();
	
	// fill structure
	m_pInfo->unMsgType	=_MSG_MAINSERVER;
	m_pInfo->unMsgSize	= GetStdSize();
	
	m_pInfo->dwMainServer	=dwMainServer;
	m_pInfo->dwData			=dwData;

	m_unMsgType				= _MSG_MAINSERVER;
	m_unMsgSize				= GetStdSize();
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMainServer::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_MAINSERVER != this->GetType())
		return false;
	
	return true;
}