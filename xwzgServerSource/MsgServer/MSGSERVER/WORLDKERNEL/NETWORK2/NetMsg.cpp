// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////
#define _WINSOCKAPI_
#include "AllMsg.h"
#include "basefunc.h"
#include "common.h"
#include "WorldKernel.h"
using namespace world_kernel;
#include "UserList.h"
#include "MsgIssue.h"
#include "MsgSynMemberInfo.h"
#include "MsgItem.h"
#include "MsgEudemonInfo.h"
#include "MsgItemInfo.h"

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
#ifdef _MYDEBUG
	::LogMsg("SendMsg:%d, size:%d", m_unMsgType, m_unMsgSize);

#endif
	if(!IsNpcMsg())
		return GameWorld()->SendClientMsg(m_idSocket, pMsg);
// 	else
// 		return GameWorld()->SendNpcMsg(m_idNpc, pMsg);
}

//////////////////////////////////////////////////////////////////////
// object functions;
//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::CreateClientMsg2(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idMsg,
								  const char* pbufMsg, DWORD dwMsgSize, int nTrans)
{
	// check it...
	if(idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
		return NULL;
	
// 	CNetMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
// 	if(!pMsg)
// 		return NULL;
	
	m_idProcess	= idProcess;
	m_idSocket	= idSocket;
	m_idNpc		= ID_NONE;
	m_unMsgType	= idMsg;
	m_unMsgSize	= dwMsgSize;
	m_nTransData	= nTrans;
	
	if(!CNetMsg::Create((char*)pbufMsg, dwMsgSize))
	{
		return FALSE;
	}
	else
	{
#ifdef _MYDEBUG
		::LogMsg("\n--Receive Msg:%u, Size:%u", pMsg->GetType(), pMsg->GetSize());
#endif
		return TRUE;
	}
}

//////////////////////////////////////////////////////////////////////
// static functions;
//////////////////////////////////////////////////////////////////////
CNetMsg* CNetMsg::CreateClientMsg(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idMsg,
										const char* pbufMsg, DWORD dwMsgSize, int nTrans,ULONG nIP)
{
	int nStep = 0;
	DEBUG_TRY
	// check it...
	if(idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
		return NULL;

	nStep = 1;
	CNetMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
	if(!pMsg)
		return NULL;

	nStep = 2;
	pMsg->m_idProcess	= idProcess;
	pMsg->m_idSocket	= idSocket;
	pMsg->m_idNpc		= ID_NONE;
	pMsg->m_unMsgType	= idMsg;
	pMsg->m_unMsgSize	= dwMsgSize;
	pMsg->m_nTransData	= nTrans;
	pMsg->m_nIP			= nIP;

	nStep = 3;
	if(!pMsg->CNetMsg::Create((char*)pbufMsg, dwMsgSize))
	{
		nStep = 4;
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
	DEBUG_CATCH3("CNetMsg::CreateClientMsg nStep = %u idMsg = %u",nStep,idMsg)
	return NULL;
}

//////////////////////////////////////////////////////////////////////
CNetMsg* CNetMsg::CreateNpcMsg(PROCESS_ID idProcess, OBJID idNpc, OBJID idMsg, 
										const char* pbufMsg, DWORD dwMsgSize, int nTrans)
{
	// check it...
	if(idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
		return NULL;

	CNetMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
	if(!pMsg)
		return NULL;

	pMsg->m_idProcess	= idProcess;
	pMsg->m_idSocket	= SOCKET_NONE;
	pMsg->m_idNpc		= idNpc;
	pMsg->m_unMsgType	= idMsg;
	pMsg->m_unMsgSize	= dwMsgSize;
	pMsg->m_nTransData	= nTrans;

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
	case _MSG_ITEMINFO:
		pMsg=new CMsgItemInfo;
		break;
	case _MSG_EUDEMONINFO:
		pMsg=new CMsgEudemonInfo;
		break;
	case _MSG_ITEM:
		pMsg=new CMsgItem;
		break;
	case _MSG_TALK:
		pMsg	=new CMsgTalk;
		break;
	case _MSG_REGISTER:
		pMsg	=new CMsgRegister;
		break;
	case _MSG_LOGIN:
		pMsg	=new CMsgLogin;
		break;
	case _MSG_AINPCINFO:
		pMsg	=new CMsgAiNpcInfo;
		break;
	case _MSG_CONNECT:
		pMsg	=new CMsgConnect;
		break;
	case _MSG_TIME:
		pMsg	=new CMsgTime;
		break;
	case _MSG_NAME:
		pMsg	=new CMsgName;
		break;
	case _MSG_FRIEND:
		pMsg	=new CMsgFriend;
		break;
	case _MSG_SYNDICATE:
		pMsg	=new CMsgSyndicate;
		break;
	case _MSG_VAS:
		pMsg = new CMsgVAS;
		break;
	case _MSG_NEWCARD:
		pMsg    = new CMsgNewCard_w;
		break;
//  [2/13/2007 Halei]----------
	case _MSG_WAITQUEUE:
		pMsg = new CMsgWaitQueue;
		break;
//  [2/13/2007 Halei]----------
	case _MSG_GMSTOPACCOUNT:
		pMsg = new CMsgStopAccount;//  [8/15/2007 Add By HaLei] 封号消息
		break;
	case _MSG_EXAM:
		pMsg = new CMsgNewPlayerExam;//  [9/22/2007 Administrator]
		break;
	case _MSG_BROADCAST:
		pMsg = new CMsgBroadcast;//[2009.3.2--张冬]系统广播消息
		break;
	case _MSG_ISSUE:	
		pMsg = new CMsgIssue;
		break;
	case _MSG_INFONPCUSEREXIT:
		pMsg = new CMsgInfoNpcUserExit;
		break;
	case _MSG_FIGHTTEAMACTION:
		pMsg = new CMsgFightTeamAction;
		break;
	case _MSG_SYNMEMBERINFO:
		pMsg = new CMsgSynMemberInfo;
		break;
	case _MSG_Fatigue:
		pMsg = new CMsgFatigue;
		break;
	case _MSG_TEAMPOS:
		pMsg = new CMsgTeamPos;
		break;
	case _MSG_GAINAWARDS:
		pMsg = new CMsgAward;
		break;
//modify code by 林德渊 begin 2011.5.30
	case _MSG_VASLV:
		pMsg = new CMsgVasLv1;
		break;
//modify code by 林德渊 end
	default:
		break;
	}
	return pMsg;
}
