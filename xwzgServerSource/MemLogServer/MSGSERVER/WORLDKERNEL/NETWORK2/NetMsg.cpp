// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////
#define _WINSOCKAPI_
#include "AllMsg.h"
#include "basefunc.h"
#include "common.h"
#include "WorldKernel.h"
using namespace world_kernel;
//#include "AccountManage.h"
//#include "GatewayInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNetMsg::CNetMsg()
{
	Init();
}

CNetMsg::~CNetMsg()
{

}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Init()
{
	memset(m_bufMsg, 0L, sizeof(m_bufMsg));
	m_unMsgSize	=0;
	m_unMsgType	=_MSG_NONE;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::IsValid(void)
{
	if(_MSG_NONE == this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Process(void *pInfo)
{
	//--------------------
	char szMsg[1024];
	sprintf(szMsg, "Process Msg:%d, Size:%d", m_unMsgType, m_unMsgSize);
	::LogSave(szMsg);	
}

//////////////////////////////////////////////////////////////////////
bool CNetMsg::SendMsg(CNetMsg* pMsg)
{
	return GameWorld()->SendClientMsg(m_idSocket, pMsg);
}

//////////////////////////////////////////////////////////////////////
// static functions;
//////////////////////////////////////////////////////////////////////
CNetMsg* CNetMsg::CreateClientMsg(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idMsg,const char* pbufMsg, DWORD dwMsgSize)
{
	// check it...
	if(idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
		return NULL;

	CNetMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
	if(!pMsg)
		return NULL;

	pMsg->m_idSocket	= idSocket;
	pMsg->m_unMsgType	= idMsg;
	pMsg->m_unMsgSize	= dwMsgSize;

	if(!pMsg->CNetMsg::Create((char*)pbufMsg, dwMsgSize))
	{
		SAFE_DELETE(pMsg);
		return NULL;
	}
	else
	{
	#ifdef _MYDEBUG
		::LogMsg("\n--Receive Msg:%u, Size:%u", pMsg->GetType(), pMsg->GetSize());
	#endif
		return pMsg;
	}
}

CNetMsg* CNetMsg::CreateSocketMsg(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idMsg,const char* pbufMsg, DWORD dwMsgSize)
{
	// check it...
	if(idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
		return NULL;
	
	CNetMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
	if(!pMsg)
		return NULL;

	pMsg->m_idSocket	= idSocket;
	pMsg->m_unMsgType	= idMsg;
	pMsg->m_unMsgSize	= dwMsgSize;
	
	if(!pMsg->CNetMsg::Create((char*)pbufMsg, dwMsgSize))
	{
		SAFE_DELETE(pMsg);
		return NULL;
	}
	else
	{
#ifdef _MYDEBUG
		::LogMsg("\n--Receive Msg:%u, Size:%u", pMsg->GetType(), pMsg->GetSize());
#endif
		return pMsg;
	}
}

CNetMsg* CNetMsg::CreateMsg(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize)
{
	// make it...
	CNetMsg* pMsg	=NULL;
	switch(idMsg)
	{	 
// 	case _MSG_SERVER_LOGIN:
// 		pMsg = new CMsgServerConnect;
// 		break;
// 	case _MSG_SERVER_STATUS:
// 		pMsg = new CMsgServerStatus;
// 		break;
// 	case _MSG_SERVER_ACTION:
// 		pMsg = new CMsgServerAction;
// 		break;
// 	case _MSG_SERVER_POST:
// 		pMsg = new CMsgServerPost;
// 		break;
	default:
		break;
	}
	return pMsg;
}
