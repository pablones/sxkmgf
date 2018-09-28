// MsgLogin.cpp: implementation of the CMsgLogin class.
//
//////////////////////////////////////////////////////////////////////

#include "MessagePort.h"
#include "protocol.h"
#include "../../share/define.h"
#include "AllMsg.h"
#include "WorldKernel.h"
using namespace world_kernel;
#include "UserList.h"
extern int g_SpecilLine;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgLogin::CMsgLogin()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgLogin::~CMsgLogin()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgLogin::Create(DWORD dwVersion, const char* pszAccount, const char* pszPassword)
{
	// check param
	if(!pszAccount || strlen(pszAccount) >= _MAX_NAMESIZE)
		return false;

	if(!pszPassword || strlen(pszPassword) >= _MAX_NAMESIZE)
		return false;

	// init
	this->Init();
	
	// fill structure
	m_unMsgType	=_MSG_LOGIN;
	m_unMsgSize	=sizeof(MSG_Info);

	m_pInfo->dwVersion	=dwVersion;
	strcpy(m_pInfo->szAccount, pszAccount);
	strcpy(m_pInfo->szPassword, pszPassword);
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgLogin::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_LOGIN != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgLogin::Process(void *pInfo)
{

#ifdef _MSGDEBUG
	::LogMsg("Process CMsgLogin, Account:%d, Name:%s", 
					m_pInfo->idAccount, 
					m_pInfo->szUserName);
#endif
// 	if(m_pInfo->dwVersion == NPCSERVER_VERSION || m_pInfo->dwVersion == NPCSERVER_VERSION_RELOGIN)
// 	{
// 		ProcessNpcServerLogin(pInfo, m_pInfo->dwVersion == NPCSERVER_VERSION);	
// 		return ;
// 	}
}

//////////////////////////////////////////////////////////////////////
// bool CMsgLogin::ProcessNpcServerLogin(void *pInfo, bool bDelAllMonster)
// {
// 	//原NPC socket
// 	SOCKET_ID idNpcSocket = GameWorld()->GetNpcSocketID();
// 	if(idNpcSocket != SOCKET_NONE)
// 	{
// 		LOGMSG("NPC server repeat login, break the last socket!");
// 	}
// 
// 	//验证
// 	if(strcmp(m_pInfo->szAccount, NPCSERVER_ACCOUNT) != 0 
// 			|| strcmp(m_pInfo->szPassword, NPCSERVER_PASSWORD) != 0)
// 	{
// 		CMsgTalk	msg;
// 		if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, "ERROR: login refuse!", NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
// 			GameWorld()->SendNpcMsg(ID_NONE, &msg);
// 		return false;
// 	}
// 
// 	
// 	IMessagePort* pMsgPort = GameWorld()->GetMsgPort();
// 	if(bDelAllMonster)
// 	{
// 		// 地图组删除怪物
// 		for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
// 			pMsgPort->Send(i, MAPGROUP_DELALLMONSTER, VARTYPE_INT, &m_pInfo->dwVersion);
// 
// 		//网络线程 清除
// 		pMsgPort->Send(MSGPORT_SOCKET, MAPGROUP_DELALLMONSTER, VARTYPE_INT, &m_pInfo->dwVersion);
// 		//UserList()->DelAllAgent();
// 	}
// 
// 	//设置新NPC  socket
// 	idNpcSocket	= m_idSocket;
// 	GameWorld()->SetNpcSocketID(idNpcSocket);
// 	pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SETNPCSOCKETID, VARTYPE_INT, &idNpcSocket);
// 
// 	//通知NPC
// // 	CMsgTalk	msg;
// // 	if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, REPLAY_OK_STR, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
// // 		GameWorld()->SendNpcMsg(ID_NONE, &msg);
// // 	if (g_SpecilLine==1)
// // 	{
// // 		CMsgTalk	msg;
// // 		if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, REPLAY_OK_STR, NULL, _COLOR_WHITE, _TXTATR_SPECILLINE))
// // 			GameWorld()->SendNpcMsg(ID_NONE, &msg);
// // 	}
// 	//msg界面显示
// 	char*	pText = "NPC server login OK.";
// 	pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pText), pText);
// 	return true;
// }










