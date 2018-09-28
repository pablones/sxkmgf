// WorldKernel.cpp: implementation of the CWorldKernel class.
//
//////////////////////////////////////////////////////////////////////
#define	_WINSOCKAPI_		// 阻止加载winsock.h
#include "inifile.h"
#include "protocol.h"
#include "windows.h"
#include "Winuser.h"
#include "BaseFunc.h"
#include "I_MessagePort.h"
#include "AllMsg.h"
#include "NetMsg.h"
#include "MessageBoard.h"
#include "WorldKernel.h"
#include "ClientSocket.h"
#include "MsgAccount_A.h"
#include "MsgConnect_A.h"
#include "MsgLogin_A.h"
#include "MsgFee_A.h"
#include "MsgQueryFee_A.h"
#include "SynWorldManager.h"
#include "SupermanList.h"
#include "AutoPtr.h"
#include "ChatRoomManager.h"
#include "MsgVAS.h"
//#include "SynWorldShell.cpp"
#include "MsgNewPlayerExam.h"
#include "StrRes.h"
#include "MsgSynInfo.h"
#include "MsgItem.h"
#include "MsgWeaponSoul.h"
#include "MsgEudemonInfo.h"
#include "MsgItemInfo.h"
#include "MsgSynMemberInfo.h"
#include "MsgFatigue.h"

using namespace world_kernel;
const int WORLDDAY	=	1000*60*60*6;	//李京哲0327 每个游戏日为6小时，暂时
const int WAITQUEUETIME = 15;			//  [2/12/2007 Halei]	排队时每人等待时间
const int NOWAITPLAYER  = 1000;			//  [2/12/2007 Halei]	不用排队的人数
//////////////////////////////////////////////////////////////////////
CWorldKernel*	CWorldKernel::m_pWorld = NULL;


char g_szKernelState[1024 * 10] = {0};
char g_szShellState[1024 * 10] = {0};

//modify code by 林德渊 begin 2011.5.19
#include ".\\..\\MAPGROUPKERNEL\\statistical.h"
//modify code by 林德渊 end
//extern CChatRoomManager g_objChatRoomManager;
//////////////////////////////////////////////////////////////////////
// interface
//////////////////////////////////////////////////////////////////////
bool CWorldKernel::Create(IMessagePort* pPort)
{
	m_pMsgPort		= pPort;
	m_idProcess		= m_pMsgPort->GetID();		// process id == msg_port_id
	m_pMsgPort->Open();
	ASSERT(m_idProcess == MSGPORT_WORLD);
	m_idNpcSocket	= SOCKET_NONE;

	// 初始化数据库
	char	DB_IP	[DBSTR_SIZE] = "";
	char	DB_USER	[DBSTR_SIZE] = "";
	char	DB_PW	[DBSTR_SIZE] = "";
	char	DB_DB	[DBSTR_SIZE] = "";

	CIniFile	ini(CONFIG_FILENAME, "Database");
	ini.GetString(DB_IP,	"DB_IP",	DBSTR_SIZE);
	ini.GetString(DB_USER,	"DB_USER",	DBSTR_SIZE);
	ini.GetString(DB_PW,	"DB_PW",	DBSTR_SIZE);
	ini.GetString(DB_DB,	"DB_DB",	DBSTR_SIZE);
	m_pDb	= ::CreateDatabase(DB_IP, DB_USER, DB_PW, DB_DB);

	if(!m_pDb)
	{
		LOGERROR("数据库连接失败!");
		return false;
	}

	//  [1/15/2008 朱斌]
	::GetLocalTime(&m_stLastDatabaseReconnectTime);

	// TODO: 请在此添加初始化代码
	m_pUserList	= CUserList::CreateNew();
	if(!m_pUserList)
		return false;
	m_pUserList->Create(GetSocketInterface(), m_pDb);

	m_pMapList	= CMapList::CreateNew();
	if(!m_pMapList)
		return false;
	m_pMapList->Create(m_pDb);

	m_pWorld	= this;

// 	m_pSynManager	= new CSynWorldManager;
// 	CHECKF(m_pSynManager);
// 	m_pSynManager->Create(m_pDb);
// 
// 	m_pTutorWord = new CTutorWorld;
// 	CHECKF(m_pTutorWord);
// 	m_pTutorWord->Create(m_pDb);
// 	
// 	m_pShopManager	= new CShopManager;
// 	CHECKF(m_pShopManager);
// 	m_pShopManager->Create(m_pDb);
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	
// 	m_pShopVAS  = new CShopVAS;
// 	CHECKF(m_pShopVAS);
// 	m_pShopVAS->Create(m_pDb);

	m_setConnectClient		= CConnectSet::CreateNew(true);

	// account server
//	CIniFile	ini(CONFIG_FILENAME, "AccountServer");
	ini.SetSection("AccountServer");
	NAMESTR		ACCOUNT_IP;
	int			ACCOUNT_PORT;

	ini.GetString(ACCOUNT_IP, "ACCOUNT_IP", _MAX_NAMESIZE);
	ACCOUNT_PORT	= ini.GetInt("ACCOUNT_PORT");
	ini.GetString(m_szServer, "SERVERNAME", _MAX_NAMESIZE);
	ini.GetString(m_szAccount, "LOGINNAME", _MAX_NAMESIZE);
	ini.GetString(m_szPassword, "PASSWORD", _MAX_NAMESIZE);

	///处理客户端侦听端口号
	ini.SetSection("System");
	m_nServerPort=ini.GetInt("SERVER_LISTEN_PORT");
	if(!m_nServerPort)
		m_nServerPort=SERVER_LISTEN_PORT_DEFAULT;

	if(ACCOUNT_PORT == 0)
	{
		LOGERROR("INI配置错误!");
		return false;
	}
	if(m_sock.Open(ACCOUNT_IP, ACCOUNT_PORT, 0, SOCKET_ACCOUNTBUFSIZE, SOCKET_ACCOUNTBUFSIZE))
		m_nState = STATE_CONNECTOK;
/*---------------------06.8.7王玉波添加，连接状态服务器------------------------------------*/
//此处从ini中读取IP及端口
	ini.SetSection( "StateServer" );
	NAMESTR		STATE_IP;
	int			STATE_PORT;
	
	ini.GetString( STATE_IP,"STATE_IP",_MAX_NAMESIZE );
	STATE_PORT = ini.GetInt( "STATE_PORT" );
	if( 0 == STATE_PORT )
	{
		LOGERROR("INI配置错误! 0 == STATE_PORT");
		return false;
	}	
	if( m_sockForStateServer.Open( STATE_IP,STATE_PORT,0,SOCKET_ACCOUNTBUFSIZE,SOCKET_ACCOUNTBUFSIZE ) )
		m_nState2 = STATE_CONNECTOK;
	memset( &m_SendToStateInfo,0,sizeof( m_SendToStateInfo ) );
	m_SendToStateInfo.unMsgType = 4004;
	strcpy( m_SendToStateInfo.szServerName,m_szServer );


/*----------------------可有可无的关系服务器-----------------------------------------*/
	ini.SetSection( "RelationServer" );
	NAMESTR		RSERVER_IP="";
		
	ini.GetString( RSERVER_IP,"RSERVER_IP",_MAX_NAMESIZE );
	m_RServerPort = ini.GetInt( "RSERVER_PORT" );
	if(m_RServerPort)
	{
		m_sockForRelationServer.Open( RSERVER_IP,m_RServerPort,0,SOCKET_ACCOUNTBUFSIZE,SOCKET_ACCOUNTBUFSIZE);
		if(IsRelationServerOpen())
			SendRelationAccount(m_szAccount, m_szPassword, m_szServer);
	}	
	
/*----------------------可有可无的关系服务器-----------------------------------------*/


	m_ptrTradeMsgBd		= CMessageBoard::CreateNew(_TXTATR_MSG_TRADE);
	m_ptrFriendMsgBd	= CMessageBoard::CreateNew(_TXTATR_MSG_FRIEND);
	m_ptrTeamMsgBd		= CMessageBoard::CreateNew(_TXTATR_MSG_TEAM);
	m_ptrOtherMsgBd		= CMessageBoard::CreateNew(_TXTATR_MSG_OTHER);
	m_ptrSystemMsgBd	= CMessageBoard::CreateNew(_TXTATR_MSG_SYSTEM);

	LoadConfig(m_pDb);

	m_WorldTime	= ::GetTickCount();

	m_bArenaUpdate = false;

	m_WaitQueueTimer.SetInterval(60);
	m_WaitQueueTimer.Update();

	return true;	
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ProcessSocketMsg()
{
	if(m_sock.IsOpen())
		ProcessAccountNetMsg();
	if(m_sockForRelationServer.IsOpen())
		ProcessRelationNetMsg();
	return true;
}

bool CWorldKernel::ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)
{
	switch(idPacket)
	{
	case	KERNEL_CLIENTMSG:
		{
			CLIENTMSG_PACKET0*	pMsg = (CLIENTMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			ULONG	nIP		= pMsg->nIP;
			ProcessClientMsg(idSocket, idMsg, pBuf, nMsgLen, 0, nIP);
		}
		break;
	case	WORLD_TRANSMITMSG:
		{

			TRANSMSG_PACKET0*	pMsg = (TRANSMSG_PACKET0*)buf;
//			if( pMsg->idPacket == _MSG_VAS )  07.01.08彭峰增值系统
//			{
//				CMsgTraffic msg;
//				msg.Create(pMsg->buf,pMsg->nSize);
//				SendVASRequest( pMsg->buf );
//				return true;
//			}
			SOCKET_ID	idSocket= pMsg->idSocket;
			OBJID		idNpc	= pMsg->idNpc;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			int		nTrans	= pMsg->nTrans;

// 			if(idSocket == SOCKET_NONE)
// 				ProcessNpcMsg(idNpc, idMsg, pBuf, nMsgLen, nTrans);
			if(idSocket != SOCKET_NONE)
			{
				DEBUG_TRY
				ProcessClientMsg(idSocket, idMsg, pBuf, nMsgLen, nTrans);
				DEBUG_CATCH("CWorldKernel::ProcessMsg() ProcessClientMsg")
			}
		}
		break;
// 	case	KERNEL_NPCMSG:
// 		{
// 			NPCMSG_PACKET0*	pMsg = (NPCMSG_PACKET0*)buf;
// 			OBJID	idNpc	= pMsg->idNpc;
// 			OBJID	idMsg	= pMsg->idPacket;
// 			char*	pBuf	= pMsg->buf;
// 			int		nMsgLen	= pMsg->nSize;
// 			ProcessNpcMsg(idNpc, idMsg, pBuf, nMsgLen);
// 		}
// 		break;
	case	WORLD_SHELLTALK:
		{
			char* pText = (char*)buf;
			// TODO: 请在此添加外壳输入文本的处理代码
			m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pText), pText);

			char szCmd[256] = "";
			if (sscanf(pText, "%s", szCmd) == 1)
			{
				if( 0 == stricmp( szCmd,"exportconset") )
				{
					CConnectSet::Iter it;
					for( it = m_setConnectClient->Begin();it != m_setConnectClient->End();it++ )
					{
						CConnectUser* pConnectUser = m_setConnectClient->GetObjByIter( it );
						if( pConnectUser != NULL )
						{
						//	LogSave( "AccountID:%d,State:%d,SocketID:%d,info:%s",
						//		pConnectUser->GetID(),pConnectUser->GetLoginState(),pConnectUser->GetSocketID(),pConnectUser->GetInfo() );
							::MyLogSave( "syslog/exportconset","EXPORT:AccountID:%d,State:%d,SocketID:%d,info:%s",
								pConnectUser->GetID(),pConnectUser->GetLoginState(),pConnectUser->GetSocketID(),pConnectUser->GetInfo() );
						}
					}
				}
				else if (0 == stricmp(szCmd, "setmaxplayer"))
				{
					int nMaxPlayers = 0;
					if (2 == sscanf(pText, "%s %d", szCmd, &nMaxPlayers))
					{
						extern DWORD g_dwMaxPlayer;
						InterlockedExchange((long*)&g_dwMaxPlayer, nMaxPlayers);
						{
							char szMsg[256] = "";
							sprintf(szMsg, "Set max player limit to %d", g_dwMaxPlayer);
							m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szMsg), szMsg);
						}
					}
				}
				else if (0 == stricmp(szCmd, "kickoutcheat"))
				{
					int	nData = 0;
					if (2 == sscanf(pText, "%s %d", szCmd, &nData))
					{
						extern long	g_sKickoutCheat;
						long nOld = InterlockedExchange(&g_sKickoutCheat, nData);
					}
				}
				else if (0 == stricmp(szCmd, "kickoutall"))
				{
					OBJID idGmSocket = -1;		// -1: for console
					m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_BREAKALLCONNECT, VARTYPE_INT, &idGmSocket);
				}
				else if (0 == stricmp(szCmd, "broadcast"))
				{
					char* pCmd = pText + strlen(szCmd);
					if(*pCmd == ' ')
						pCmd++;

					GameWorld()->QueryUserList()->BroadcastMsg(pCmd, NULL, NULL, 0x00FF0000, _TXTATR_IMPORTINFO);
				}
				else if (0 == stricmp(szCmd, "shutdown"))//STR_SHUTDOWN_NOTIFY
				{
					GameWorld()->QueryUserList()->BroadcastMsg(g_objStrRes.GetStr(11084), NULL, NULL, 0x00FF0000, _TXTATR_IMPORTINFO);

					char* pMsg = "Send shut down broadcast message.";
					m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pMsg), pMsg);
				}
				else if (0 == stricmp(szCmd, "restartserver"))
				{
					extern long g_nServerClosed;
					if(g_nServerClosed)
					{
						extern long g_nRestart;
						g_nRestart = true;
					}
					else
					{
						LPCTSTR pMsg = "Please <kickoutall> before <restartserver>!";
						m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pMsg), pMsg);
					}
				}
				else if (0 == stricmp(szCmd, "restartwindows"))
				{
					extern long g_nServerClosed;
					if(g_nServerClosed)
					{
						HANDLE hToken;
						TOKEN_PRIVILEGES tkp;

						// Get a token for this process.
						if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
						{
							LOGERROR("OpenProcessToken");
							break;
						}

						// Get the LUID for the shutdown privilege.
						LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
						tkp.PrivilegeCount = 1; // one privilege to set
						tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

						// Get the shutdown privilege for this process.
						AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

						// Cannot test the return value of AdjustTokenPrivileges.
						if (GetLastError() != ERROR_SUCCESS)
						{
							LOGERROR("AdjustTokenPrivileges");
							break;
						}

						// Shut down the system and force all applications to close. 
#define EWX_FORCEIFHUNG      0x00000010
						if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCEIFHUNG, 0))
						{
							LOGERROR("ExitWindowsEx");
							break;
						}

//						::ExitWindowsEx(EWX_REBOOT, 0);		// | EWX_FORCEIFHUNG
					}
					else
					{
						LPCTSTR pMsg = "Please <kickoutall> before <restartserver>!";
						m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pMsg), pMsg);
					}
				}
			}
		}
		break;
	// TODO: 请在此添加其它内部消息处理代码
	case	KERNEL_CLOSEKERNEL:
		{
			SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
//			GameWorld()->ConnectShutDown( idSocket );
//			GameWorld()->WaitQueueSocketShutDown( idSocket );		//  [2/12/2007 Halei] 当玩家异常退出,从排队队列中删除
			LOGDEBUG("DEBUG：世界核心收到关闭核心消息，SOCKET_ID[%u]", idSocket);
#ifdef	NEW_LOGOUT
			g_UserList.LogoutUser(idSocket);
			m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CLOSESOCKET, VARTYPE_INT, &idSocket);		// readme.txt(2-1)
#else
			if(g_UserList.IsLoginMapGroup(idSocket))
			{
				for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
					m_pMsgPort->Send(i, KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);		// readme.txt(2-2)
			}
			else
				m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CLOSESOCKET, VARTYPE_INT, &idSocket);		// readme.txt(2-1)
#endif
		}
		break;
		//----------------------------------
	case WORLD_USERCLOSEONTIMER:
		{
			SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
			CPlayer *pPlayer = g_UserList.GetPlayerBySocket(idSocket);
			if( pPlayer )
			{
				int idProcess = pPlayer->idProcess;//玩家所在地图组
				m_pMsgPort->Send(idProcess, MAPGROUP_USERCLOSEONTIMER, VARTYPE_INT, &idSocket);
			}
		}
		break;
		//----------------------------------
	case	WORLD_CLOSESOCKET:		// 由MAPGROUP发过来
		{
#ifdef	NEW_LOGOUT
			ASSERT(!"WORLD_CLOSESOCKET");
#else
			SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
			LOGDEBUG("DEBUG：世界核心收到关闭网络消息，SOCKET_ID[%u]", idSocket);
			g_UserList.LogoutUser(idSocket);
			m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CLOSESOCKET, VARTYPE_INT, &idSocket);		// readme.txt(2-6)
#endif
		}
		break;
	case	WORLD_SETPROCESSID:
		{
			CHANGE_USERDATA* pPakcet = (CHANGE_USERDATA*)buf;
			g_UserList.ChangeProcessID(pPakcet->idSocket, pPakcet->nData);
		}
		break;
	case	WORLD_USERNAMEMSG:
		{
			USERNAMEMSG_PACKET0*	pMsg = (USERNAMEMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;

//			if (idMsg == _MSG_TALK)
//			{
//				// 服务频道判断
//				typedef struct {
//					DWORD	dwWordsColor;
//					USHORT	unTxtAttribute;
//					// ... 其它定义没必要写了
//				}MSG_Info;
//				MSG_Info* pInfo = (MSG_Info*)pBuf;
//				if (pInfo->unTxtAttribute == _TXTATR_SERVE)
//				{
//					CPlayer* pTarget = UserList()->GetGM();
//					if (pTarget)
//					{
//						SendMsg(pTarget->idSocket, idMsg, pBuf, nMsgLen);
//						break;
//					}
//				}
//			}

			CPlayer* pPlayer = UserList()->GetPlayer(pMsg->szName);
			if(pPlayer)
			{
				SendMsg(pPlayer->idSocket, idMsg, pBuf, nMsgLen);
			}
			else if(idMsg == _MSG_TALK && idSocket != SOCKET_NONE && m_sockForRelationServer.IsOpen()==false)//当关系服务器不可用时走传统流程
			{
				// 私聊
				CPlayer* pSender = UserList()->GetPlayerBySocket(idSocket);
				if(pSender && strcmp(pMsg->szName,ALLUSERS_NAME))///广播的talk消息不发查无此人
				{
					CMsgTalk	msg;
					IF_OK(msg.Create(SYSTEM_NAME, pSender->szName, STR_TARGET_OFFLINE))
						pSender->SendMsg(&msg);
				}
			}

			if(idMsg==_MSG_TALK)
			{
				CMsgTalk msg;
				msg.Create(pBuf,nMsgLen);
				if(msg.m_pInfo->unTxtAttribute==_TXTATR_MSG_SERVERLIST)///处理玩家心跳时需要更新的服务器列表消息
				{
					char szSender[_MAX_NAMESIZE];
					CNetStringPacker	StrPacker;
					StrPacker.SeekBuf(msg.m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(msg.m_pInfo)+1);
					StrPacker.GetString(0, szSender, sizeof(szSender));
					CPlayer* requesterptr =UserList()->GetPlayerBySocket(msg.m_pInfo->dwWordsColor);// UserList()->GetPlayer(szSender);				
					if(requesterptr)
					{
						CMsgTalk rcMsg;
						rcMsg.Create(_TXTATR_MSG_SERVERLIST,m_szMsgServerList);
						requesterptr->SendMsg(&rcMsg);
					}
					break;
				}
			}
			///向关系服务器求助
// 			CMsgTalk	msg;
// 			msg.Create(pBuf,nMsgLen);
// 			msg.unMsgSize	= sizeof(msg);
// 			msg.unMsgType	= _MSG_RELAY;

			///世界上找到玩家才向关系服务器求助找不到
			if(IsRelationServerOpen() && pPlayer==NULL)
				SendRelationRelayMsg(idMsg,pBuf,nMsgLen);

		}
		break;
	case WORLD_RELATIONMSG:
		{
			USERIDMSG_PACKET0*	pMsg = (USERIDMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;///这里的idSocket被用作发送方地图组的id
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			///对方不在线，很头疼吧，没关系有关系服务器来帮助
			if(IsRelationServerOpen())
				SendRelationRelayMsg(idMsg,pBuf,nMsgLen,pMsg->idUser);

			///因为关系服务器广播缝隙,需要给一份副本地图组
			TransmitBroadcastMapGroupMsg(idMsg,(const char*)pBuf,nMsgLen,pMsg->idUser,idSocket);

// 			CNetMsg* msgptr=CNetMsg::CreateClientMsg(SOCKET_NONE,SOCKET_NONE,idMsg,(const char*)pBuf,nMsgLen,pMsg->idUser);
// 			if(msgptr)
// 			{
// 				///广播除了源地图组之外的所有地图组
// 				TransmitBroadcastMapGroupMsg(msgptr,idSocket);
// 				SAFE_DELETE(msgptr);
// 			}

		}
		break;
	case	WORLD_USERIDMSG:
		{
			USERIDMSG_PACKET0*	pMsg = (USERIDMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			CPlayer* pPlayer = UserList()->GetPlayer(pMsg->idUser);
			if(pPlayer)
			{
				SendMsg(pPlayer->idSocket, idMsg, pBuf, nMsgLen);
			}
			else
			{
				if(idMsg==_MSG_TALK)
				{
					CMsgTalk msg;
					msg.Create(pBuf,nMsgLen);
					if(msg.m_pInfo->unTxtAttribute==_TXTATR_TEAM)///队伍频道消息不许传到关系服务器
					{
						break;
					}
				}
				if(idMsg==_MSG_BUFF)
					break;
				///对方不在线，很头疼吧，没关系有关系服务器来帮助
				if(IsRelationServerOpen() )
					SendRelationRelayMsg(idMsg,pBuf,nMsgLen,pMsg->idUser);
			}
		}
		break;
	// syndicate -------------------------------------------------------------------------------
	case	KERNEL_CREATESYNDICATE:
		{
			CreateSyndicateInfo* pPacket = (CreateSyndicateInfo*)buf;
			GameWorld()->QuerySynManager()->QueryModify()->CreateSyndicate(pPacket);
		}
		break;
	case	KERNEL_DESTROYSYNDICATE:
		{
			OBJID* pId	= (OBJID*)buf;
			OBJID idSyn		= pId[0];
			OBJID idTarget	= pId[1];
			GameWorld()->QuerySynManager()->QueryModify()->DestroySyndicate(idSyn, idTarget);
		}
		break;
	case	KERNEL_CHANGESYNDICATE:
		{
			SynFuncInfo0* pPacket = (SynFuncInfo0*)buf;
			CSyndicateWorld* pSyn = GameWorld()->QuerySynManager()->QuerySyndicate(pPacket->idSyn);
			IF_OK(pSyn)
			{
				pSyn->ChangeSyndicate(pPacket);
			}
		}
		break;
	case	WORLD_STOPSAY:
		{
			GM_STOPSAY pPakcet = *(GM_STOPSAY*)buf;
			CPlayer* pPlayer = NULL;
			pPlayer =  g_UserList.GetPlayer(pPakcet.szname);
		//	GameWorld()->QueryUserList()->GetPlayerByAccountID(pPakcet->UserID);
			if(pPlayer && g_UserList.IsLoginMapGroup(pPlayer->idSocket))//玩家在线
			{
				IMessagePort* pMsgPort = GameWorld()->GetMsgPort();
				pMsgPort->Send(pPlayer->idProcess, 
					MAPGROUP_GMSTOPSAY, 
					STRUCT_TYPE(pPakcet), 
					&pPakcet);

			}
		}
		break;
	case	WORLD_LEVELUP:
		{
			//20070406修罗:发过来的为idUser和nLevel
			//OBJID idUser = *(OBJID*)buf;
			__int64 idUserAndLevel = *(__int64*)buf;
			int nLevel = idUserAndLevel & 0x00000000ffffffff;
			OBJID idUser = idUserAndLevel>>32;
			CPlayer* pPlayer = GameWorld()->QueryUserList()->GetPlayer(idUser);
			IF_OK(pPlayer)
			{
				//pPlayer->m_nLevel	= LOGIN_FREE_LEVEL;
				pPlayer->m_nLevel	= nLevel;
			}
		}
		break;
	case	WORLD_QUERYFEE:
		{
			OBJID idAccount = *(OBJID*)buf;
			if(IsAccountLoginOK())
				SendQueryFee(idAccount, 0, 0, 0);
		}
		break;
	case	WORLD_SENDTIME:
		{
			SOCKET_ID idSocket = *(SOCKET_ID*)buf;
			IF_OK(idSocket != SOCKET_NONE)
				UserList()->SendTimeToSocket(idSocket);
		}
		break;
	case	WORLD_TEAMINVITE:
		{
			for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
				m_pMsgPort->Send(i, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), buf);		// readme.txt(2-2)
		}
		break;
		//////////////////////////////////////////////////////////////////////////
		//  [2007-9-10] 朱斌
	case WORLD_FRIENDAPPLY:
		{
			for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
				m_pMsgPort->Send(i, WORLD_FRIENDAPPLY, STRUCT_TYPE(ST_TEAM_INVITE), buf);		// readme.txt(2-2)
		}
		break;
	case WORLD_TUTOR_INFO:
		{
// 			ChgTutorInfo *pInfo = static_cast<ChgTutorInfo *>(buf);
// 			bool bRet = TutorWorld()->ProcessTutorInfo(pInfo);
// 			if(bRet)
// 			{
// 				GameWorld()->SendTutorToMapGroup(pInfo);
// 			}
		}
		break;
	default:
		LOGWARNING("CWorldKernel::ProcessMsg() idPacket = %u",idPacket);
	}

	return true;		// return false : 消息处理异常，程序关闭。
}
//modify code by 林德渊 begin 2011.5.19
int g_statnum = 1;
//modify code by 林德渊 end
//////////////////////////////////////////////////////////////////////
//20070709修罗:增强catch分块.不因为某段catch影响其他功能
bool CWorldKernel::OnTimer(time_t tCurr)
{
	// TODO: 请在此添加时钟处理代码
	DEBUG_TRY
	g_UserList.OnTimer(tCurr);
	DEBUG_CATCH("CWorldKernel::OnTimer::g_UserList.OnTimer(tCurr)")
//modify code by 林德渊 begin 2011.5.19
// 	g_statnum++;
// 	if( (g_statnum % 60) == 0 )
// 	{
// 		g_StatisticalData.Clear();
// 		g_statnum = 0;
// 	}
/*	if( 60 * 10 == g_statnum )
	{
		char szstring[256] = {0};
		char sznpcrecvstring[60] = {0};
		char szrecvstring[60] = {0};
		char szwalkstring[60] = {0};
		char szintestring[60] = {0};
		SOCKET_ID idSocket = (SOCKET_ID)g_StatisticalData.GetRecvKey();
		CPlayer* pPlayer = g_UserList.GetPlayerBySocket(idSocket);
		if( NULL != pPlayer )
			strcpy(szrecvstring,pPlayer->szName);
		pPlayer = g_UserList.GetPlayerBySocket((SOCKET_ID)g_StatisticalData.GetNpcRecvKey());
		if( NULL != pPlayer )
			strcpy(sznpcrecvstring,pPlayer->szName);
		pPlayer = g_UserList.GetPlayer((OBJID)g_StatisticalData.GetWalkKey());
		if( NULL != pPlayer )
			strcpy(szwalkstring,pPlayer->szName);
		if( strlen(szwalkstring) <= 0 )
			sprintf(szwalkstring,"%u",g_StatisticalData.GetWalkKey());
		pPlayer = g_UserList.GetPlayer((OBJID)g_StatisticalData.GetInteKey());
		if( NULL != pPlayer )
			strcpy(szintestring,pPlayer->szName);
		if( strlen(szintestring) <= 0 )
			sprintf(szintestring,"%u",g_StatisticalData.GetInteKey());
		sprintf(szstring,"maxnpcrec=%d[%s],maxrec=%d[%s],maxwal=%d[%s],maxint=%d[%s]",g_StatisticalData.GetMaxnpcrecvcount(),sznpcrecvstring,g_StatisticalData.GetMaxrecvcount(),szrecvstring,g_StatisticalData.GetMaxwalkcount(),szwalkstring,g_StatisticalData.GetMaxinteract(),szintestring);
		PrintText(szstring);
		g_statnum = 0;
	}*/
//modify code by 林德渊 end
// 	[2009.3.2--张冬]系统广播消息
// 		DEBUG_TRY
// 		SYSTEMTIME cTime;
// 		::GetLocalTime( &cTime );
// 		static WORD nextMinute=cTime.wMinute;
// 		if (cTime.wMinute == nextMinute && !g_objStrRes.GetIsLock())
// 		{
// 			nextMinute=(++nextMinute)%60;
// 			char strSys[16];
// 			sprintf(strSys,"%d_%d_%d_%d_%d",cTime.wYear, cTime.wMonth,
// 					cTime.wDay, cTime.wHour,cTime.wMinute);
// 			SYSBROADCAST* sys = g_objStrRes.GetSysBroadcast(strSys);
// 			if (sys && (-1 == sys->wDayOfWeek || sys->wDayOfWeek == cTime.wDayOfWeek))
// 			{
// 				CMsgBroadcast msg;
// 				if (msg.Create(sys->wTime,sys->uProfession,sys->downLevel,
// 					sys->upLevel,sys->strContent))
// 				{
// 	
// 					UserList()->BroadcastMsg(&msg);	
// 				}	
// 			}
// 			
// 			sprintf(strSys,"%d_%d_%d_%d", cTime.wMonth,
// 				 cTime.wDay, cTime.wHour,cTime.wMinute);
// 			sys = g_objStrRes.GetSysBroadcast(strSys);
// 			if (sys && (-1 == sys->wDayOfWeek || sys->wDayOfWeek == cTime.wDayOfWeek)) 
// 			{
// 				CMsgBroadcast msg;
// 				if (msg.Create(sys->wTime,sys->uProfession,sys->downLevel,
// 					sys->upLevel,sys->strContent))
// 				{
// 	
// 					UserList()->BroadcastMsg(&msg);	
// 				}	
// 			}
// 			
// 			sprintf(strSys,"%d_%d_%d",
// 					cTime.wDay, cTime.wHour,cTime.wMinute);
// 			sys = g_objStrRes.GetSysBroadcast(strSys);
// 			if (sys && (-1 == sys->wDayOfWeek || sys->wDayOfWeek == cTime.wDayOfWeek) ) 
// 			{
// 				CMsgBroadcast msg;
// 				if (msg.Create(sys->wTime,sys->uProfession,sys->downLevel,
// 					sys->upLevel,sys->strContent))
// 				{
// 	
// 					UserList()->BroadcastMsg(&msg);	
// 				}	
// 			}
// 			 
// 			sprintf(strSys,"%d_%d",
// 				 cTime.wHour,cTime.wMinute);
// 			sys = g_objStrRes.GetSysBroadcast(strSys);
// 			if (sys && (-1 == sys->wDayOfWeek || sys->wDayOfWeek == cTime.wDayOfWeek) ) 
// 			{
// 				CMsgBroadcast msg;
// 				if (msg.Create(sys->wTime,sys->uProfession,sys->downLevel,
// 					sys->upLevel,sys->strContent))
// 				{
// 	
// 					UserList()->BroadcastMsg(&msg);	
// 				}		
// 			}
// 		}
// 		DEBUG_CATCH("CWorldKernel::OnTimer::SYSBROADCAST")
// 			
// 		//[2009.2.17--张冬]对数据进行操作
// 		DEBUG_TRY
// 		SYSTEMTIME currentTime;
// 		::GetLocalTime( &currentTime );
// 		static m_bUpdate=false;
// 		if ( 23 == currentTime.wHour && 59 == currentTime.wMinute && !m_bUpdate)
// 		{
// 			 m_bUpdate=true;
// 		}
// 	
// 		if (0 == currentTime.wHour && 0 == currentTime.wMinute && m_bUpdate)
// 		{
// 			char strSql[128];
// 			sprintf(strSql,sqlUpdateLove,0,5);
// 			if(!m_pDb->ExecuteSQL(strSql))
// 				::LogSave("CWorldKernel::OnTimer ExecuteSQL Fail,sql:%s",strSql);///如果失败写日志
// 	
// 			sprintf(strSql,sqlUpdateLoveChange,5,5);
// 			if(!m_pDb->ExecuteSQL(strSql))
// 				::LogSave("CWorldKernel::OnTimer ExecuteSQL Fail,sql:%s",strSql);///如果失败写日志
// 	
// 			m_bUpdate = false;
// 		}
// 		DEBUG_CATCH("CWorldKernel::OnTimer::SQL")
// 	
// 		//[2009.7.14--张冬]
// 		DEBUG_TRY
// 		if (m_pSynManager)
// 		{
// 			m_pSynManager->OnSynTimer();
// 		}
// 		DEBUG_CATCH("CWorldKernel::OnTimer::m_pSynManager")
// 	
// 		DEBUG_TRY
// 		if (m_pTutorWord)
// 		{
// 			m_pTutorWord->OnTimer();
// 		}
// 		DEBUG_CATCH("CWorldKernel::OnTimer::m_pTutorWord")
//-----------------------07.5.26----begin-------------------
//王玉波添加，在人物选择角色界面，上传心跳消息至帐号服务器
	DEBUG_TRY
	CConnectSet::Iter it;
	for( it = m_setConnectClient->Begin();it != m_setConnectClient->End(); )
	{
		CConnectUser* pConnectUser = m_setConnectClient->GetObjByIter( it );
		it ++;

		if( pConnectUser != NULL )
		{
			OBJID idAccount = ID_NONE;
			if( pConnectUser->CheckHeartBeat( idAccount ) )//重要
			{
				ASSERT( idAccount != ID_NONE );
				if(UserList()->GetPlayerByAccountID(idAccount))
				{
					UserList()->KickOutAccount(idAccount, STR_LOGIN_AGAIN);
				}
				else
				{
					UserList()->LogoutUser(pConnectUser->GetSocketID());
					//KickOutSocket(pConnectUser->GetSocketID());
				}
				m_setConnectClient->DelObj(idAccount);
				//GameWorld()->SendFee( idAccount,CMsgFee_A::FEE_POINT );
			}
		}
	}
	extern struct STAT_STRUCT	g_stat;
	InterlockedExchange( &g_stat.nConnectUserSize,m_setConnectClient->GetAmount() );		// debug
	DEBUG_CATCH("CWorldKernel::OnTimer::m_setConnectClient心跳")
//----------------------------------end---------------------
#ifdef	ACCOUNT_ENABLE
	// account server
	if(!m_sock.IsOpen())
	{
		m_nState = STATE_NONE;
		PrintText("Connect to account server again...");
		if(m_sock.Open(0))
			m_nState = STATE_CONNECTOK;
	}
#endif
/*---------------06.8.7王玉波添加处理连接STATE SERVER--------------------------*/

//	if(!m_sockMonitor.IsOpen())
//	{
//		m_nState3 = STATE_NONE;
//		PrintText("Connect to monitor server again...");
//		if(m_sockMonitor.Open(0))
//		{
//			m_nState3 = STATE_CONNECTOK;
//			PrintText("Monitor server connect ok!");
//		}
//	}
	
// 	DEBUG_TRY
// 	const DWORD SEND_USERAMOUNT_TIMER = 5 * 1000;	//60秒,一分钟发一次......
// 	if( !m_sockForStateServer.IsOpen() )
// 	{
// 		m_nState2 = STATE_NONE;
// 		//20070215修罗:bug
// 		//此句意外的写入到sk_taskDetail表某行.原因不明.
// 		//不再记录此信息.连接时自然有ok字样
// 		//GameWorld()->PrintText( "Connect to state server again..." );
// 		if( m_sockForStateServer.Open( 0 ) )
// 			m_nState2 = STATE_CONNECTOK;
// 	}
// 	else
// 	{
// 		if( m_nState2 == STATE_CONNECTOK )
// 		{
// 			m_nState2 = STATE_STATEOK;
// 			PrintText( "State Server Login OK!" );
// 			m_dwTimeForStateServer = clock();
// 		}
// 		if( ( clock() - m_dwTimeForStateServer ) >= SEND_USERAMOUNT_TIMER ) 
// 		{
// 			m_SendToStateInfo.nOnlineUserAmount = m_pUserList->GetUserAmount();
// 			m_sockForStateServer.Send2StateServer( ( LPCTSTR )&m_SendToStateInfo,sizeof( m_SendToStateInfo),true );//发送信息
// 
// 			m_dwTimeForStateServer = clock();//更新时间
// 		}
// 		
// 	}
// 	DEBUG_CATCH("CWorldKernel::OnTimer::连接状态服务器")
/*-----------------处理连接STATE SERVER end -----------------------------------*/

	///关系服务器断线重连机制 由于需要补登不能太快重连  重连定为10S
	DEBUG_TRY
	static DWORD dwLastConnectReserver = 0;
	if(m_RServerPort)///只有关系服务器有效的情况下才允许重连
	{
		if(!m_sockForRelationServer.IsOpen())
		{
			if(dwLastConnectReserver != 0)
			{
				dwLastConnectReserver ++;
				if(dwLastConnectReserver >= 10)
					dwLastConnectReserver = 0;
			}
			if(dwLastConnectReserver == 0)
			{
				dwLastConnectReserver ++;
				GameWorld()->PrintText( "Connect to relation server again..." );
				m_sockForRelationServer.Open();
				if(m_sockForRelationServer.IsOpen())
				{
					dwLastConnectReserver = 1;
					SendRelationAccount(m_szAccount, m_szPassword, m_szServer);
					SendRelationAllUser();
				}
			}
		}
	}
	DEBUG_CATCH("CWorldKernel::OnTimer::连接关系服务器")

	DEBUG_TRY
	if(m_sock.IsOpen())
	{
		if(m_nState == STATE_CONNECTOK)
		{
			SendAccount(m_szAccount, m_szPassword, m_szServer,m_nServerPort);
			SendAccountAllUser();
			m_nState = STATE_ACCOUNTOK;
		}
	}
	DEBUG_CATCH("CWorldKernel::OnTimer::ProcessAccountNetMsg()")

//	DEBUG_TRY
//	if(m_sockMonitor.IsOpen())
//	{
//		ProcessMonitorNetMsg();
//	}
//	DEBUG_CATCH("CWorldKernel::OnTimer::ProcessMonitorNetMsg()")

 	DEBUG_TRY
 	char Temp = 'w';//因为send时如果size 为0,则会丢弃此包，所以暂时发一个无用的字符..
 
// 	if(::GetTickCount()-m_SyndicateTime>= 1000*60 )//每分钟第一地图组检查军团约战情况
// 	{
// 		m_SyndicateTime	 =	::GetTickCount();
// 		m_pMsgPort->Send(MSGPORT_MAPGROUP_FIRST,MAPGROUP_NEW_SYN_TIME,STRUCT_TYPE( Temp ),&Temp);
// 		//m_pShopManager->Updata();//[游途道标 2009.02.17]更新店铺信息
// 	}

	if(::GetTickCount()-m_WorldTime>= WORLDDAY )//李京哲0321 新的一天
	{
		m_WorldTime	 =	::GetTickCount();
		m_pMsgPort->Send(MSGPORT_MAPGROUP_FIRST,MAPGROUP_NEWDAY,STRUCT_TYPE( Temp ),&Temp);
	}
	DEBUG_CATCH("CWorldKernel::OnTimer::新的一天")

//	g_objChatRoomManager.OnTimer(tCurr);
	//  [2/28/2007 HALEI] 玩家登陆排队系统,增加功能当玩家正在排队发现目前在线人数少于需要排队人数判断
	//首先得到需要排队的人数(时间),用MustWaitTime保存
	DEBUG_TRY
	int MustWaitTime  = GameWorld()->QueryUserList()->GetUserAmount()+1/*+m_setConnectClient->GetAmount()-WaitQueue.size()*/-NOWAITPLAYER;
	
	if ( MustWaitTime <= 0 )		//判断是否需要排队.
	{	//如果不需要排队
		int infosize = WaitQueue.size();//得到排队队列的大小,看有多少人在排队
		if (infosize != 0)			//如果里面还有玩家
		{
			//可以先让队列里的玩家先进入.
			ConnectInfo_WaitQueue info;
			info = WaitQueue.front();
			WaitQueue.erase( WaitQueue.begin() );
			CMsgWaitQueue msg;
			if ( msg.Create( info.idAccount,0,NETWORK_WAITSUSSECS ) )//发送消息告诉玩家你可以进入了.
			{
				GameWorld()->SendClientMsg(info.idSocket,&msg);
			}		
			CConnectUser* pConnect = GameWorld()->m_setConnectClient->GetObj( info.idAccount );
			if( pConnect )
			{
				int tempidAccount  =info.idAccount;
				int tempidSocket   = info.idSocket;
				ConnectInfo_ST tempInfo;
				tempInfo.idAccount	= tempidAccount;
				tempInfo.idSocket	= tempidSocket;
				//发送玩家快照
				if( !GameWorld()->LoginUserSnap( tempInfo ) )		//halei
				{
					GameWorld()->m_setConnectClient->DelObj( info.idAccount );
				}
				pConnect->SetLoginState( JOIN );				
			}									
		
		}
	}
	else
	{
		//处理排队信息发送
		int infosize = WaitQueue.size();
		if(infosize!=0 && m_WaitQueueTimer.IsTimeOut())
		{
			//给每位玩家发送排队等待信息
			CMsgWaitQueue waitmsg;
			for (int i = 0;i<infosize;++i)
			{
				
				waitmsg.Create(WaitQueue[i].idAccount,NETWORK_WAITINFO,i+1,infosize);
				GameWorld()->SendClientMsg(WaitQueue[i].idSocket,&waitmsg);
			}

			m_WaitQueueTimer.Update();
		}
	}
	DEBUG_CATCH("CWorldKernel::OnTimer::排队")
	

	DEBUG_TRY
	ReconnectDB();
	DEBUG_CATCH("CWorldKernel::OnTimer::ReconnectDB");

	return true;		// return false : 消息处理异常，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::Release()
{
	// TODO: 请在此添加代码
	g_UserList.LogoutAllUser();

	SAFE_RELEASE (m_pUserList);
	SAFE_RELEASE (m_pMapList);
//	SAFE_RELEASE (m_pSynManager);
//  SAFE_RELEASE (m_pTutorWord);

// 	if(m_pShopManager)
// 	{
// 		SAFE_DELETE(m_pShopManager);
// 	}

// 	if (m_pShopVAS)
// 	{
// 		SAFE_DELETE(m_pShopVAS);
// 	}

	m_pMsgPort->Close();

	GameWorld()->SendFee(ID_NONE, CMsgFee_A::SERVER_CLOSE);

	::LogSave("CWorldKernel::Release m_sock.Close() 帐号服务器连接被关闭,正常关闭的?");
	m_sock.Close();

	m_sockMonitor.Close();

	//发送服务器当机消息到StateServer
	m_SendToStateInfo.nOnlineUserAmount = -1;
	m_sockForStateServer.Send2StateServer( ( LPCTSTR )&m_SendToStateInfo,sizeof( m_SendToStateInfo),true );//发送信息
	m_sockForStateServer.Close();

	m_sockForRelationServer.Close();
	SAFE_RELEASE (m_setConnectClient);

	SAFE_RELEASE (m_pDb);
	
	delete this;
	return true;		// return false : 无意义。
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ProcessClientMsg(SOCKET_ID idSocket, OBJID idMsg, const char *pbufMsg, int nSize, int nTrans, ULONG nIP)
{
	// TODO: 请在此添加客户端上传消息的处理代码
	if(idMsg == _MSG_NONE || !pbufMsg || nSize <= 0 || nSize >_MAX_MSGSIZE)
        return false;

	CNetMsg* pMsg	=CNetMsg::CreateClientMsg(m_idProcess, idSocket, idMsg, pbufMsg, nSize, nTrans, nIP);
	if(pMsg)
	{
		/*
		try {
			pMsg->Process(this);
		}
		catch(...)
		{
//			CNetMsg::DumpMsg(pMsg);
			ASSERT(!"pMsg->Process(this);");
			::LogSave("exception catch at CGameSocket::ProcessMsg()! MsgType:%d, SocketID:%u", idMsg, idSocket);
		}
		*/
		//-------彭峰---2007/05/23-----
		UINT		usAction=0;		
		DEBUG_TRY
			memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
			pMsg->Process(this);
		DEBUG_CATCH4("exception catch at CWorldKernel::ProcessClientMsg! MsgType:%d, SocketID:%u,action:%u", idMsg, idSocket,usAction)

		delete pMsg;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendMsg(CNetMsg* pNetMsg)
{
	ASSERT(pNetMsg);
	if(!pNetMsg)
		return false;

	if(!pNetMsg->IsNpcMsg())
		return SendMsg(pNetMsg->GetSocketID(), pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
// 	else
// 		return SendNpcMsg(pNetMsg->GetNpcID(), pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendMsg(SOCKET_ID idSocket, OBJID idMsg, const char *pBuf, int nMsgLen)
{
	// TODO: 请在此添加NPC服务器下传消息的代码
	if (!pBuf)
	{
		::LogSave("Error: null msg point found in CGameSocket::SendMsg.");
		return false;
	}

	if (_MSG_NONE == idMsg)
	{
		::LogSave("Error: invalid msg type in CGameSocket::SendMsg().");
		return false;
	}
/*
	try {
		MESSAGESTR	bufPacket;
		SENDCLIENTMSG_PACKET0* pPacket = (SENDCLIENTMSG_PACKET0*)bufPacket;
		pPacket->idSocket	= idSocket;
		pPacket->idPacket	= idMsg;
		pPacket->nSize		= nMsgLen;
		memcpy(pPacket->buf, pBuf, nMsgLen);
		m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDCLIENTMSG, STRUCT_TYPE(bufPacket), &bufPacket);
		return true;
	}
	catch(...)
	{
		ASSERT(!"catch");
		::LogSave("Error: exception catched in CGameSocket::SendMsg().");
		return false;
	}			
*/
	DEBUG_TRY
		MESSAGESTR	bufPacket;
		SENDCLIENTMSG_PACKET0* pPacket = (SENDCLIENTMSG_PACKET0*)bufPacket;
		pPacket->idSocket	= idSocket;
		pPacket->idPacket	= idMsg;
		pPacket->nSize		= nMsgLen;

		memcpy(pPacket->buf, pBuf, nMsgLen);
		m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDCLIENTMSG, STRUCT_TYPE(bufPacket), &bufPacket);
		return true;
	DEBUG_CATCHF("Error: exception catched in CGameSocket::SendMsg().")
		
	return true;
}

//////////////////////////////////////////////////////////////////////
// bool CWorldKernel::ProcessNpcMsg(OBJID idNpc, OBJID idMsg, const char *pbufMsg, int nSize, int nTrans)
// {
// 	// TODO: 请在此添加NPC服务器上传消息的处理代码
// 	if(idMsg == _MSG_NONE || !pbufMsg || nSize <= 0 || nSize >_MAX_MSGSIZE)
//         return false;
// 
// 	CNetMsg* pMsg	=CNetMsg::CreateNpcMsg(m_idProcess, idNpc, idMsg, pbufMsg, nSize, nTrans);
// 	if(pMsg)
// 	{
// 		/*
// 		try {
// 			pMsg->Process(this);
// 		}
// 		catch(...)
// 		{
// //			CNetMsg::DumpMsg(pMsg);
// 			ASSERT(!"pMsg->Process(this);");
// 			::LogSave("exception catch at CGameSocket::ProcessMsg()! MsgType:%d, NPCID:%u", idMsg, idNpc);
// 		}
// 		*/
// 		//-------彭峰---2007/05/23-----
// 		DEBUG_TRY
// 			pMsg->Process(this);
// 		DEBUG_CATCH3("exception catch at CWorldKernel::ProcessNpcMsg! MsgType:%d, NPCID:%u", idMsg, idNpc)
// 		delete pMsg;
// 	}
// 
// 	return true;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CWorldKernel::SendNpcMsg(CNetMsg* pNetMsg)
// {
// 	ASSERT(pNetMsg);
// 	if(!pNetMsg)
// 		return false;
// 
// 	return SendNpcMsg(pNetMsg->GetNpcID(), pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CWorldKernel::SendNpcMsg(OBJID idNpc, OBJID idMsg, const char *pBuf, int nMsgLen)
// {
// 	// TODO: 请在此添加NPC服务器下传消息的代码
// 	if (!pBuf)
// 	{
// 		::LogSave("Error: null msg point found in CGameSocket::SendMsg.");
// 		return false;
// 	}
// 
// 	if (_MSG_NONE == idMsg)
// 	{
// 		::LogSave("Error: invalid msg type in CGameSocket::SendMsg().");
// 		return false;
// 	}
// /*
// 	try {
// 		MESSAGESTR	bufPacket;
// 		SENDNPCMSG_PACKET0* pPacket = (SENDNPCMSG_PACKET0*)bufPacket;
// 		pPacket->idNpc		= idNpc;
// 		pPacket->idPacket	= idMsg;
// 		pPacket->nSize		= nMsgLen;
// 		memcpy(pPacket->buf, pBuf, nMsgLen);
// 		m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDNPCMSG, STRUCT_TYPE(bufPacket), &bufPacket);
// 		return true;
// 	}
// 	catch(...)
// 	{
// 		ASSERT(!"catch");
// 		::LogSave("Error: exception catched in CGameSocket::SendMsg().");
// 		return false;
// 	}			
// */
// 	DEBUG_TRY
// 		MESSAGESTR	bufPacket;
// 		SENDNPCMSG_PACKET0* pPacket = (SENDNPCMSG_PACKET0*)bufPacket;
// 		pPacket->idNpc		= idNpc;
// 		pPacket->idPacket	= idMsg;
// 		pPacket->nSize		= nMsgLen;
// 		memcpy(pPacket->buf, pBuf, nMsgLen);
// 		m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDNPCMSG, STRUCT_TYPE(bufPacket), &bufPacket);
// 		return true;
// 
// 	DEBUG_CATCHF("Error: exception catched in CGameSocket::SendMsg().")
// 
// 	return true;
// }

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::CloseSocket(SOCKET_ID idSocket)			// 直接关闭socket
{
	// TODO: 请在此添加关闭客户端SOCKET的代码
	m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_BREAKCONNECT, VARTYPE_INT, &idSocket);
#ifdef WYB_TEST
	LOGMSG( "worldkernel:send close socket msg!" );
#endif
	if(idSocket == m_idNpcSocket)
	{
		LOGMSG("CWorldKernel::CloseSocket break npc connect!", idSocket);
		m_idNpcSocket	= SOCKET_NONE;		//@???
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendClientMsg(SOCKET_ID idSocket, CNetMsg* pMsg)
{
#ifdef _MYDEBUG
	::LogMsg("SendMsg:%d, size:%d", m_unMsgType, m_unMsgSize);

#endif

	pMsg->AppendInfo(MSGPORT_WORLD, idSocket, ID_NONE);
    return SendMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
// bool CWorldKernel::SendNpcMsg(OBJID idNpc, CNetMsg* pMsg)
// {
// #ifdef _MYDEBUG
// 	::LogMsg("SendMsg:%d, size:%d", m_unMsgType, m_unMsgSize);
// 
// #endif
// 
// 	pMsg->AppendInfo(MSGPORT_WORLD, SOCKET_NONE, idNpc);
//     return SendNpcMsg(pMsg);
// }

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::PrintText			(LPCTSTR szText)
{
	return m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ChangeCode(SOCKET_ID idSocket, DWORD dwData)
{
	CHANGE_USERDATA	st;
	st.idSocket		= idSocket;
	st.nData		= dwData;
	return m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CHANGECODE, STRUCT_TYPE(CHANGE_USERDATA), &st);
}

///////////////////////////////////////////////////////////////////////////////////////
// account server
///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendAccount(LPCTSTR szAccount, LPCTSTR szPassword, LPCTSTR szServer,UINT nServerPort)
{
	CMsgAccount_A::MSG_Info	msg;
	msg.unMsgSize	= sizeof(msg);
	msg.unMsgType	= _MSG_ACCOUNT_A;

	SafeCopy(msg.szAccount	, szAccount	, _MAX_NAMESIZE);
	SafeCopy(msg.szPassword	, szPassword, _MAX_NAMESIZE);
	SafeCopy(msg.szServer	, szServer	, _MAX_NAMESIZE);

	char szPlayerCount[256]="";
	sprintf(szPlayerCount,"%d",UserList()->GetUserAmount());
	SafeCopy(msg.szMacClient, szPlayerCount, _MAX_NAMESIZE);

	msg.nServerPort=nServerPort;
	return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

///////////////////////////////////////////////////////////////////////////////////////
// relation server
///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendRelationAccount(LPCTSTR szAccount, LPCTSTR szPassword, LPCTSTR szServer)
{
	CMsgAccount_A::MSG_Info	msg;
	msg.unMsgSize	= sizeof(msg);
	msg.unMsgType	= _MSG_ACCOUNT_A;
	
	SafeCopy(msg.szAccount	, szAccount	, _MAX_NAMESIZE);
	SafeCopy(msg.szPassword	, szPassword, _MAX_NAMESIZE);
	SafeCopy(msg.szServer	, szServer	, _MAX_NAMESIZE);
	
	return m_sockForRelationServer.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

void CWorldKernel::SendAccountAllUser()
{
	DEBUG_TRY
	CPlayer *player=NULL;
	for(int i=0;i<UserList()->GetUserAmount();++i)
	{
		player=UserList()->GetPlayerByIndex(i);
 		if(player)
 			SendFee(player->m_idAccount, CMsgFee_A::FEE_HOLDASS);///补登
	}
	DEBUG_CATCH("CWorldKernel::SendAccountAllUser")
}

void CWorldKernel::SendRelationAllUser()///将服务器拥有的所有玩家信息补登到关系服务器上
{
	DEBUG_TRY
	CPlayer *player=NULL;
	for(int i=0;i<UserList()->GetUserAmount();++i)
	{
		player=UserList()->GetPlayerByIndex(i);
		if(player)
			SendRelationMsg(raRoleOnline,player->GetID(),player->szName);
	}
	DEBUG_CATCH("CWorldKernel::SendRelationAllUser")
}

///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendLogin(OBJID idAccount, DWORD dwCheckData, int usVersion, LPCTSTR szUserName)
{
	CMsgLogin_A::MSG_Info	msg;
	msg.unMsgSize	= sizeof(msg);
	msg.unMsgType	= _MSG_LOGIN_A;

	msg.idAccount	= idAccount;
	msg.dwCheckData	= dwCheckData;
	msg.usVersion	= usVersion;
	SafeCopy(msg.szUserName, szUserName);

	return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

bool CWorldKernel::SendStopAccount(OBJID StopAccountID,int un_MsgType, int m_unMsgSize ,USHORT nAction ,OBJID playerID)
{
	CMsgStopAccount::MSG_Info msg;
	msg.unMsgType = un_MsgType;
	msg.unMsgSize = m_unMsgSize;
	msg.unAciton=nAction;
	msg.ucStopIDAccount = StopAccountID;
	msg.playerID = playerID;
	
	return m_sock.SendPacket((LPCTSTR)&msg,msg.unMsgSize,true);
}
////////////////////////07.01.05彭峰增值///////////////////////////////////////////////////////////////
//bool CWorldKernel::SendVASRequest( int id, int itemAmount, OBJID itemID, int usAction, OBJID AccountID, OBJID PlayerID, int usVASPoint, int unMsgType, int unMsgSize )
//  [2/16/2008 朱斌]	附加属性
//bool CWorldKernel::SendVASRequest( int id, int itemAmount, OBJID itemID, DWORD dwData, int usAction, OBJID AccountID, OBJID PlayerID, int usVASPoint, int unMsgType, int unMsgSize ,int nJuan,int nRealID)
//[2010-07-26 goto]消息结构修改
bool CWorldKernel::SendVASRequest(USHORT unMsgType, USHORT unMsgSize, ULONG nVASPoint, OBJID AccountID, OBJID PlayerID, USHORT usAction, USHORT usGoodSize, CMsgVAS::MsgTraffic* nGoods)
{
	CMsgVAS::MSG_Info msg;
	msg.unMsgType = unMsgType;
	msg.unMsgSize = unMsgSize;
	msg.usVASPoint = nVASPoint;
	msg.AccountID = AccountID;
	msg.PlayerID = PlayerID;
	msg.usAction = usAction;
	msg.usGoodSize = usGoodSize;
	for (int i=0; i<usGoodSize; i++)
	{
		msg.nGoods[i].itemID = nGoods[i].itemID;
		msg.nGoods[i].itemAmount = nGoods[i].itemAmount;
		msg.nGoods[i].dwData = nGoods[i].dwData;
	}

	//[游途道标 2009.01.08]
	if (!msg.unMsgSize)
	{
		msg.unMsgSize = sizeof(CMsgVAS::MSG_Info);
	}

	return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

bool CWorldKernel::SendNewCardRequest(USHORT unMsgType, USHORT unMsgSize, DWORD idUser , DWORD idAcount , DWORD dwType, char* pszBuf)
{
	CMsgNewCard_w::MSG_Info msg;
	msg.unMsgType = unMsgType;
	msg.unMsgSize = unMsgSize;

	msg.dwType = dwType;
	msg.idUser = idUser;
	msg.idAccount = idAcount;
	strcpy(msg.szBuf,pszBuf);
	
	return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

bool CWorldKernel::SendNewPlayerExam( int UserID, int nAccountID,int m_unMsgType, int m_unMsgSize )
{
	CMsgNewPlayerExam::MSG_Info msg;
	msg.unMsgType = m_unMsgType;
	msg.unMsgSize = m_unMsgSize;
	msg.nAccountID = nAccountID;
	msg.nUserID	 = UserID;

	return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

///广播转发的消息
void CWorldKernel::BroadcastMsg(OBJID idMsg, const char *pBuf, int nMsgLen)
{
	CPlayer *player=NULL;
	for(int i=0;i<UserList()->GetUserAmount();++i)
	{
		player=UserList()->GetPlayerByIndex(i);
		if(player)
		{
			SendMsg(player->idSocket,idMsg,pBuf,nMsgLen);
		}
	}
}

///转发消息到关系服务器上
void CWorldKernel::SendRelationRelayMsg(OBJID idMsg,const char* pack, int nLen,int nExtra)
{
	///队伍消息不上关系服务器传发
	if(idMsg==_MSG_TEAM)
		return;

	RelayData packet;
	memcpy(&packet.buffer,pack,nLen);
	memcpy(packet.buffer+nLen,&nExtra,sizeof(nExtra));
	nLen+=8;
	packet.unMsgSize	= nLen;
	packet.unMsgType	= idMsg;
	m_sockForRelationServer.SendPacket((const char*)&packet, nLen, true);	
}

///发送消息到关系服务器上
void CWorldKernel::SendRelationMsg(MsgRelayAction msgAction,OBJID srcRoleID,LPCTSTR szRoleName,OBJID dstRoleID)
{
	if (szRoleName==NULL)
		szRoleName="";

	CMsgRelay::MSG_Info	msg;
	msg.unMsgSize	= sizeof(msg);
	msg.unMsgType	= _MSG_RELAY;
	
	msg.usAction	= msgAction;
	msg.idRole		= srcRoleID;
	msg.dstRoleID	= dstRoleID;
	strncpy(msg.szUserName,szRoleName,_MAX_NAMESIZE);
	m_sockForRelationServer.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendFee(OBJID idAccount, int nType)
{
	CMsgFee_A::MSG_Info	msg;
	msg.unMsgSize	= sizeof(msg);
	msg.unMsgType	= _MSG_FEE_A;

	msg.idAccount	= idAccount;
	msg.ucType		= nType;
	
	return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendQueryFee(OBJID idAccount, DWORD dwData, int nTime, int ucType)
{
	CMsgQueryFee_A::MSG_Info	msg;
	msg.unMsgSize	= sizeof(msg);
	msg.unMsgType	= _MSG_QUERYFEE_A;

	msg.idAccount	= idAccount;
	msg.dwData		= dwData;
	msg.nTime		= nTime;
	msg.ucType		= ucType;
	
	return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

void CWorldKernel::ProcessRelationNetMsg()
{
	bool	bGetData = true;
	char buffer[2048]="";
	while(true)
	{
		DEBUG_TRY
		int	nNetSize = 0;
		LPCTSTR pMsgsock = m_sockForRelationServer.GetPacket(&nNetSize, bGetData);
		if(!pMsgsock || nNetSize == 0)
			break;
		int nMsgSize = *((unsigned short*)pMsgsock);
		if(nNetSize < nMsgSize)
			break;

		if(nMsgSize>1024)
		{
			LogSave("收到一个超大包,不处理直接退出,size:%d",nMsgSize);
			m_sockForRelationServer.ClearPacket(nMsgSize);
			break;
		}
		///收到消息后马上将缓冲区数据放入堆栈中处理
		memcpy(buffer,pMsgsock,nMsgSize);
		m_sockForRelationServer.ClearPacket(nMsgSize);
		bGetData=false;
		LPCTSTR pMsg =buffer;

		// 服务器登录 ////////////////////////////////////////
		CMsgLogin_A::MSG_Info*	pMsgLogin = (CMsgLogin_A::MSG_Info*)pMsg;
		DEBUG_TRY
		if(pMsgLogin->unMsgType == _MSG_LOGIN_A)
		{
			if (pMsgLogin->unMsgSize != sizeof(CMsgLogin_A::MSG_Info))
			{
				::LogSave("error:(pMsgLogin->unMsgSize != sizeof(CMsgLogin_A::MSG_Info)");
// 				m_sockForRelationServer.ClearPacket(nMsgSize);
				continue;
			}

			if(pMsgLogin->idAccount == ID_NONE)
			{
				PrintText("Account server login failed!");
// 				m_sockForRelationServer.ClearPacket(nMsgSize);
				continue;
			}
			else
			{
				CMsgLogin_A::MSG_Info	msg;
				msg.unMsgSize	= sizeof(msg);
				msg.unMsgType	= _MSG_LOGIN_A;
				msg.idAccount	= pMsgLogin->idAccount;
				msg.dwCheckData	= pMsgLogin->dwCheckData;
				msg.usVersion	= pMsgLogin->usVersion;
				SafeCopy(msg.szUserName, pMsgLogin->szUserName);
				m_sockForRelationServer.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
				PrintText("Relation server login OK。");
			}
// 			m_sockForRelationServer.ClearPacket(nMsgSize);
			continue;
		}
		DEBUG_CATCH("ProcessRelationNetMsg::pMsgLogin")

		DEBUG_TRY
		if (pMsgLogin->unMsgType == _MSG_MAINSERVER)//主服务器消息
		{
			CMsgMainServer::MSG_Info* msgptr = (CMsgMainServer::MSG_Info*)pMsgLogin;
			//CHECK(pMsgLogin->unMsgSize == sizeof(CMsgMainServer::MSG_Info));
			
			CMsgMainServer::MSG_Info msg;
			msg.unMsgSize	= msgptr->unMsgSize;
			msg.unMsgType	= msgptr->unMsgType;
			msg.dwMainServer= msgptr->dwMainServer;
			msg.dwData		= msgptr->dwData;
			//goto 通知地图组确定主游戏服务器
			BroadcastMainServerMsg(_MSG_MAINSERVER, (LPCTSTR)&msg, msg.unMsgSize, msg.dwData*10+msg.dwMainServer, MSGPORT_MAPGROUP_FIRST);
			if(msg.dwMainServer==1)
				PrintText("Be appointed to the main server。");
			else if (msg.dwMainServer==0)
				PrintText("Be appointed to an auxiliary server。");
// 			m_sockForRelationServer.ClearPacket(nMsgSize);
			continue;
		}
		DEBUG_CATCH("ProcessRelationNetMsg::CMsgMainServer")

		///去掉消息大小与类型字段
		int nRealMsgSize=nMsgSize-8;///去掉了附加数据长度
		switch (pMsgLogin->unMsgType)
		{
			case _MSG_FEE_A:
			{
				DEBUG_TRY
				CMsgFee_A::MSG_Info* msgptr = (CMsgFee_A::MSG_Info*)pMsgLogin;
				if(msgptr->ucType==CMsgFee_A::FEE_KICKOUT)
				{
					LOGMSG("DEBUG:有人尝试通过切线重复登录,关系指示游戏服务器踢出用户id[%u]", msgptr->idAccount);
					CPlayer* pPlayer = UserList()->GetPlayer(msgptr->idAccount);
					if(pPlayer)
					{
						UserList()->KickOutAccount(pPlayer->m_idAccount, STR_LOGIN_ANOTHER);
					}
				}
				DEBUG_CATCH("ProcessRelationNetMsg::CMsgFee_A")
			}
				break;
			case _MSG_TALK:
			{
				DEBUG_TRY
				CMsgTalk msgTalk;
				bool bOnline = false;
				char szReceiver[_MAX_NAMESIZE]="";
				char szSender[_MAX_NAMESIZE]="";
				char szBuf[512] = "";
				CNetStringPacker	StrPacker;
				CMsgTalk::MSG_Info*	pMsgTalk = (CMsgTalk::MSG_Info*)&pMsgLogin->idAccount;
				StrPacker.SeekBuf(pMsgTalk->szBuf, _MAX_MSGSIZE-sizeof(CMsgTalk::MSG_Info)+1);
				StrPacker.GetString(1, szReceiver, sizeof(szReceiver));
				StrPacker.GetString(0, szSender, sizeof(szSender));
// 				StrPacker.GetString(2, szBuf, sizeof(szBuf));
 				StrPacker.GetString(3, szBuf, sizeof(szBuf));

				if(pMsgTalk->unTxtAttribute==_TXTATR_MSG_SERVERLIST)///如果是服务器列表更新消息则直接缓冲在内存内不广播
					StrPacker.GetString(3, m_szMsgServerList, sizeof(m_szMsgServerList));					
				else if(strcmp(szReceiver,ALLUSERS_NAME)==0)
				{
					///还原消息
	 				msgTalk.CreateClientMsg2(-1,-1,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize,0);
					switch(pMsgTalk->unTxtAttribute)
					{
						case _TXTATR_SYNDICATE:///帮派频道特殊处理
							///为了防止消息死循环,在这里对消息频道进行篡改
//							msgTalk.m_pInfo->unTxtAttribute=_TXTATR_SYNDICATE_OTHERLINE;
							msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
							GameWorld()->TransmitBroadcastMapGroupMsg(&msgTalk);
							break;
						case _TXTATR_PRO://门派频道特殊处理
							msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
							GameWorld()->TransmitBroadcastMapGroupMsg(&msgTalk);
							break;

						case _TXTATR_FORGMTOOL://GM工具处理
							msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
							GameWorld()->TransmitBroadcastMapGroupMsg(&msgTalk);
							break;

						default:
							UserList()->BroadcastMsg(&msgTalk);
							break;
					}
				}
				else if(pMsgTalk->unTxtAttribute==_TXTATR_CHATWITH)///如果是好友留言消息
				{
					///尝试找接收者,找到则直接发过去
					CPlayer* pPlayer = UserList()->GetPlayer(szReceiver);
					if(pPlayer)
					{
						if(msgTalk.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0))
							pPlayer->SendMsg(&msgTalk);
					}
					else
					{
						///找不到接受者时，尝试寻找发送者，走离线留言的流程
						pPlayer = UserList()->GetPlayer(szSender);
						if(pPlayer)
						{
							pMsgTalk->unTxtAttribute=_TXTATR_LEAVEWORD;///修改为离线留言
							if(msgTalk.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0))
							{
								GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess, &msgTalk);
							}
						}
					}
				}
				else
				{
					CPlayer* pPlayer = NULL;
					DEBUG_TRY
					pPlayer =  UserList()->GetPlayer(szReceiver);
					DEBUG_CATCH("UserList()->GetPlayer(szReceiver); Error!");
				//	CPlayer* pPlayers= UserList()->GetPlayer(szSender);
					if(pPlayer)
					{
						if (strncmp("!禁言", szBuf, 5) == 0 || strncmp("!kickout", szBuf, 8) == 0 || strncmp("!封号", szBuf, 5) == 0)
						{										
							if(msgTalk.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0))
							{
								msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
//								msgTalk.m_pInfo->unTxtAttribute = _TXTATR_PRIVATE_OTHERLINE;
								GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess, &msgTalk);
								break;
							}
						}
						else
						{
							if(msgTalk.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0))
							{
								msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
						/*		msgTalk.m_pInfo->unTxtAttribute = _TXTATR_PRIVATE_OTHERLINE;*/
								SendMsg(pPlayer->idSocket, _MSG_TALK,(const char*) pMsgTalk, nRealMsgSize);
//								GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess, &msgTalk);
							}
						}

// 						CMsgTalk* msgptr=(CMsgTalk*)CNetMsg::CreateClientMsg(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0);
// 						if(msgptr)
// 						{
// 							pPlayer->SendMsg(msgptr);
// 							SAFE_DELETE(msgptr);
// 						}

					}
// 					else	if(msgTalk.CreateClientMsg2(pPlayers->idProcess,pPlayers->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0))
// 					{
// 						SendMsg(pPlayers->idSocket, _MSG_TALK,(const char*) pMsgTalk, nRealMsgSize);
// 					}
// 					if (pPlayers)
// 					{
// 						if(msgTalk.CreateClientMsg2(pPlayers->idProcess,pPlayers->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0))
// 						{
// 							msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
// 							msgTalk.m_pInfo->unTxtAttribute = _TXTATR_PRIVATE_OTHERLINE;
// 							GameWorld()->TransmitMapGroupMsg(pPlayers->idProcess, &msgTalk);
// 						}
// 					}
// 					CPlayer* pPlayer = UserList()->GetPlayer(szReceiver);
// 					if(pPlayer)
// 						SendMsg(pPlayer->idSocket, _MSG_TALK,(const char*) pMsgTalk, nRealMsgSize);
				}

// 				else
// 				{
// 					CPlayer* pPlayer = UserList()->GetPlayer(szReceiver);
// 					if(pPlayer)
// 					{
// 						if (strncmp("!禁言", szBuf, 5) == 0 || strncmp("!kickout", szBuf, 8) == 0 || strncmp("!封号", szBuf, 5))
// 						{										
// 							if(msgTalk.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0))
// 							{
// 								msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
// 								GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess, &msgTalk);
// 							}
// 						}
// 						else
// 						{
// 							if(msgTalk.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0))
// 								pPlayer->SendMsg(&msgTalk);
// 						}
// 					}
// 				}
				}
				catch(...)
				{
					LogSave("ProcessRelationNetMsg::CMsgTalk  crash!");
					CMsgTalk::Dump((char*)&pMsgLogin->idAccount, nRealMsgSize);
				}
//				DEBUG_CATCH("ProcessRelationNetMsg::CMsgTalk")
				break;
			}
			case _MSG_SYNDICATE:
				{
					DEBUG_TRY	
					CMsgSyndicate msgSyndicate;
					if(msgSyndicate.CreateClientMsg2(-1,-1,pMsgLogin->unMsgType,(const char*)(const char*)&pMsgLogin->idAccount, nRealMsgSize, 0))
						msgSyndicate.Process(0);
//						pPlayer->SendMsg(&msgSyndicate);


// 					CMsgSyndicate* msgptr=(CMsgSyndicate*)CNetMsg::CreateClientMsg(0,0,pMsgLogin->unMsgType,(const char*)&pMsgLogin->idAccount, nRealMsgSize, 0);
// 					if(msgptr)
// 					{
// 						msgptr->Process(0);
// 						SAFE_DELETE(msgptr);
// 					}
					DEBUG_CATCH("ProcessRelationNetMsg::CMsgSyndicate")					
				}
				break;
			case _MSG_ACCLOCK:
				{
					DEBUG_TRY
					CNetMsg msg;
					if(msg.CreateClientMsg2(-1,-1,_MSG_ACCLOCK, (const char*)&pMsgLogin->idAccount, nMsgSize, 0))
					{
						GameWorld()->TransmitBroadcastMapGroupMsg(&msg);
					}
					DEBUG_CATCH("ProcessRelationNetMsg::_MSG_VIPLIST")
				}
				break;
			case _MSG_ITEMINFO:
				{
					DEBUG_TRY	
					OBJID idUser=0;
					memcpy(&idUser,pMsg+nMsgSize-4,4);///提取extra数据,里面保存的是源用户的id
					CPlayer* pPlayer = UserList()->GetPlayer(idUser);
					if(pPlayer)
					{
						CMsgItemInfo msgItemInfo;
						if(msgItemInfo.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)&pMsgLogin->idAccount, nRealMsgSize,0))
							pPlayer->SendMsg(&msgItemInfo);
					}
					DEBUG_CATCH("ProcessRelationNetMsg::CMsgItemInfo")					
				}
			case _MSG_EUDEMONINFO:
				{
					DEBUG_TRY	
//					CMsgEudemonInfo::MSG_Info*	msgptr = (CMsgEudemonInfo::MSG_Info*)&pMsgLogin->idAccount;
					OBJID idUser=0;
					memcpy(&idUser,pMsg+nMsgSize-4,4);///提取extra数据,里面保存的是源用户的id
					CPlayer* pPlayer = UserList()->GetPlayer(idUser);
					if(pPlayer)
					{
						CMsgEudemonInfo msgEudemonInfo;
						if(msgEudemonInfo.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)&pMsgLogin->idAccount, nRealMsgSize,0))
							pPlayer->SendMsg(&msgEudemonInfo);

// 						CMsgEudemonInfo* pMsg=(CMsgEudemonInfo*)CNetMsg::CreateClientMsg(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)&pMsgLogin->idAccount, nRealMsgSize, 0);
// 						if(pMsg)
// 						{
// 							pPlayer->SendMsg(pMsg);
// 							SAFE_DELETE(pMsg);
// 						}
						//SendMsg(pPlayer->idSocket, pMsgLogin->unMsgType,(const char*)msgptr, nRealMsgSize);	
					}
					DEBUG_CATCH("ProcessRelationNetMsg::CMsgEudemonInfo")					
				}
				break;
			case _MSG_ITEM:
				{
					DEBUG_TRY	
					CMsgItem::MSG_Info*	msgptr = (CMsgItem::MSG_Info*)&pMsgLogin->idAccount;
					CPlayer* pPlayer = UserList()->GetPlayer(msgptr->idTarget);
					if(pPlayer)
					{
						CMsgItem msgItem;
						if(msgItem.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)&pMsgLogin->idAccount, nRealMsgSize, 0))
						{
							switch(msgItem.m_pInfo->usAction)
							{
								case ITEMACT_GETINFO:
								{
									///篡改消息以支持跨服
									msgItem.m_pInfo->usAction=ITEMACT_QUERYITEMINFO;
									GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgItem);
								}
								break;
								case ITEMACT_GETEUDEMONINFO:
								{
									///篡改消息以支持跨服
									msgItem.m_pInfo->usAction=ITEMACT_QUERYEUDEMONINFO;
									GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgItem);
								}
								break;
							}
						}
// 						CMsgItem* pMsg=(CMsgItem*)CNetMsg::CreateClientMsg(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)&pMsgLogin->idAccount, nRealMsgSize, 0);
//  						if(pMsg)
//  						{
// 							switch(msgptr->usAction)
// 							{
// 								case ITEMACT_GETINFO:
// 								{
// 									///篡改消息以支持跨服
// 									pMsg->m_pInfo->usAction=ITEMACT_QUERYITEMINFO;
// 									GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,pMsg);
// 								}
// 								break;
// 								case ITEMACT_GETEUDEMONINFO:
// 								{
// 									///篡改消息以支持跨服
// 									pMsg->m_pInfo->usAction=ITEMACT_QUERYEUDEMONINFO;
// 									GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,pMsg);
// 								}
// 								break;
// 							}
// 							SAFE_DELETE(pMsg);
//  						}
 					}
					DEBUG_CATCH("ProcessRelationNetMsg::CMsgItem")
				}
				break;
			case _MSG_WEAPONSOUL:
				DEBUG_TRY	
				CMsgWeaponSoul::MSG_Info*	msgptr = (CMsgWeaponSoul::MSG_Info*)&pMsgLogin->idAccount;

				CPlayer* pPlayer = NULL;
				switch(msgptr->usAction)
				{
				case EM_MSGWPSL_QUERY:
					{
						pPlayer = UserList()->GetPlayer(msgptr->idUser);
					}
					break;
				case EM_MSGWPSL_UPDATE:
					{
						pPlayer = UserList()->GetPlayer(msgptr->idItem2);
						msgptr->idItem2 = msgptr->idItem;
					}
					break;
				}
				if(pPlayer)
				{
					CMsgItem msgItem;
					if(msgItem.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)&pMsgLogin->idAccount, nRealMsgSize, 0))
					{
						switch(msgItem.m_pInfo->usAction)
						{
						case EM_MSGWPSL_QUERY:
							{
								///篡改消息以支持跨服
								msgItem.m_pInfo->usAction=EM_MSGWPSL_QUERY_LINE;
								GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgItem);
							}
							break;
						case EM_MSGWPSL_UPDATE:
							{
								///篡改消息以支持跨服
								msgItem.m_pInfo->usAction=EM_MSGWPSL_UPDATE_LINE;
								GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgItem);
							}
							break;
						}
					}
				}
				DEBUG_CATCH("ProcessRelationNetMsg::_MSG_WEAPONSOUL")
				break;
			case _MSG_SYNMEMBERINFO:
				{
					DEBUG_TRY
					CMsgSynMemberInfo::MSG_Info*	msgptr = (CMsgSynMemberInfo::MSG_Info*)&pMsgLogin->idAccount;
					CMsgSynMemberInfo msgSynMemberInfo;
					if(msgSynMemberInfo.CreateClientMsg2(-1,-1,pMsgLogin->unMsgType,(const char*)&pMsgLogin->idAccount, nRealMsgSize, 0))
					{
// 						CPlayer* pPlayer=NULL;
// 						if(msgptr->ucAmount>1)
// 							pPlayer = UserList()->GetPlayer(msgptr->memInfo[1].id);
// 						else
// 							pPlayer =UserList()->GetPlayer(msgptr->memInfo[0].id);
// 						if (pPlayer)
// 							GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgSynMemberInfo);
// 						else
							GameWorld()->TransmitBroadcastMapGroupMsg(&msgSynMemberInfo);

					}
// 					CNetMsg* msgptr=CNetMsg::CreateClientMsg(0,0,pMsgLogin->unMsgType,(const char*)&pMsgLogin->idAccount, nRealMsgSize, 0);
// 					if(msgptr)
// 					{
// 						GameWorld()->TransmitBroadcastMapGroupMsg(msgptr);
// 						SAFE_DELETE(msgptr);
// 					}
					DEBUG_CATCH("ProcessRelationNetMsg::CMsgSynMemberInfo")
				}
				break;
			case _MSG_FRIEND:
				{
					DEBUG_TRY	
					CMsgFriend msgFriend;
					CMsgFriend::MSG_Info*	msgptr = (CMsgFriend::MSG_Info*)&pMsgLogin->idAccount;
					OBJID idUser=0;
					memcpy(&idUser,pMsg+nMsgSize-4,4);///提取extra数据,里面保存的是源用户的id
					CPlayer* pPlayer = UserList()->GetPlayer(idUser);
					switch(msgptr->ucAction)///真痛苦，跨服消息处理真麻烦，以后设计服务器架构一定要多线的
					{
						case _FRIEND_ADDSUCCESSREPLY:
							{
								msgptr->ucAction=_FRIEND_ADDSUCCESS;
								if(pPlayer)///这里指针有效说明这是一个回传消息
									SendMsg(pPlayer->idSocket, pMsgLogin->unMsgType,(const char*)msgptr, nRealMsgSize);
							}
							break;
						case _FRIEND_ADDFRIEND:
							{
								pPlayer = UserList()->GetPlayer(msgptr->idFriend);///得到被通知者对象
								if(pPlayer)
								{
									msgptr->idFriend=msgptr->dwLookFace;
									SendMsg(pPlayer->idSocket, pMsgLogin->unMsgType,(const char*)msgptr, nRealMsgSize);
								}
							}
							break;
						case _FRIEND_ADDSUCCESS_PASSIVE:
						case _FRIEND_ADDSUCCESS:///跨服中断消息
							{
								///添加别人添加好友成功，告诉你一声
								pPlayer = UserList()->GetPlayer(msgptr->idFriend);///得到被通知者对象
								if(pPlayer)
								{
//									msgptr->idFriend=idUser;
									if(msgFriend.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket, _MSG_FRIEND,(const char*)msgptr, nRealMsgSize, idUser))
										GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgFriend);

									///交换源代码与目标ID已兼容后面的代码
//									msgptr->idFriend=idUser;
// 									CMsgFriend* pMsg=(CMsgFriend*)CNetMsg::CreateClientMsg(pPlayer->idProcess,pPlayer->idSocket, _MSG_FRIEND,(const char*)msgptr, nRealMsgSize, idUser);
// 									if(pMsg)
// 									{
// 										///自己两年前写的代码，感叹啊
// 										GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,pMsg);
// 										SAFE_DELETE(pMsg);
// 									}
								}
							}
							break;
						case _FRIEND_QUERYFRIENDINFO:
						case _FRIEND_DELFRIEND:
							{
								///别人把我删除，正在通知我
								pPlayer = UserList()->GetPlayer(msgptr->idFriend);///得到被通知者对象
								if(pPlayer)
								{
									if(msgFriend.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket, _MSG_FRIEND,(const char*)msgptr, nRealMsgSize, idUser))
										GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgFriend);

// 									CMsgFriend* pMsg=(CMsgFriend*)CNetMsg::CreateClientMsg(pPlayer->idProcess,pPlayer->idSocket, _MSG_FRIEND,(const char*)msgptr, nRealMsgSize, idUser);
// 									if(pMsg)
// 									{
// 										///自己两年前写的代码，感叹啊
// 										GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,pMsg);
// 										SAFE_DELETE(pMsg);
// 									}
								}
							}
							break;
						case _FRIEND_GETFRIENDINFO:
						case _FRIEND_OFFLINE:
						case _FRIEND_ONLINE:
						case _FRIEND_GETINFO:///玩家上线
							if(pPlayer)
							{
								msgptr->ucSex /= 10;//goto把最后一位干掉
								SendMsg(pPlayer->idSocket, pMsgLogin->unMsgType,(const char*)msgptr, nRealMsgSize);
							}
							break;
						default:
							break;
					}
					DEBUG_CATCH("ProcessRelationNetMsg::CMsgFriend")
				}
				break;
			case _MSG_REGISTER:
				{
					DEBUG_TRY
					CMsgRegister::MSG_Info* msgptr = (CMsgRegister::MSG_Info*)&pMsgLogin->idAccount;
					CMsgRegister msg;
					if (msg.Create(msgptr->szName, msgptr->szPassword, msgptr->idAccount, msgptr->unLook, msgptr->unData))
						BroadcastRegisterMsg(_MSG_REGISTER, msg.GetBuf(), msg.GetSize());
					DEBUG_CATCH("ProcessRelationNetMsg::_MSG_REGISTER")
				}
				break;
			case _MSG_SCHOOLMEMBER:
				{
					DEBUG_TRY
					CMsgSchoolMember::MSG_Info* msgptr = (CMsgSchoolMember::MSG_Info*)&pMsgLogin->idAccount;
					
					CNetMsg msg;
					msg.CreateClientMsg2(-1,-1,_MSG_SCHOOLMEMBER, (const char*)&pMsgLogin->idAccount, nMsgSize, 0);

					if (msgptr->ucAction==MESSAGEBOARD_SYNLINE)
						GameWorld()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST, &msg);
					else if (msgptr->ucAction==MESSAGEBOARD_LINEBROAD)
					{
						OBJID idUser = 0;
						if (msgptr->ucPage == LINEBROAD_APPLYTUTOR || msgptr->ucPage == LINEBROAD_ONLINESTATE || msgptr->ucPage == LINEBROAD_ONLINESTATEREF)
							idUser = msgptr->idUser;
						else
							idUser = msgptr->setMember[0].idMember;
						CPlayer* pPlayer = UserList()->GetPlayer(idUser);
						if (pPlayer)
							GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msg);
					}
					DEBUG_CATCH("ProcessRelationNetMsg::_MSG_SCHOOLMEMBER")
				}
				break;
			case _MSG_VIPLIST:
				{
					DEBUG_TRY
					CNetMsg msg;
					if(msg.CreateClientMsg2(-1,-1,_MSG_VIPLIST, (const char*)&pMsgLogin->idAccount, nMsgSize, 0))
						GameWorld()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST, &msg);
					DEBUG_CATCH("ProcessRelationNetMsg::_MSG_VIPLIST")
				}
				break;
			case _MSG_WARGAME:
				{
					DEBUG_TRY
					CNetMsg msg;
					if(msg.CreateClientMsg2(-1,-1,_MSG_WARGAME, (const char*)&pMsgLogin->idAccount, nMsgSize, 0))
						GameWorld()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST, &msg);
					DEBUG_CATCH("ProcessRelationNetMsg::_MSG_WARGAME")
				}
				break;
			default:
				break;
		}			
// 		m_sockForRelationServer.ClearPacket(nMsgSize);
		DEBUG_CATCH("CWorldKernel::ProcessRelationNetMsg 主循环出错坚持运行")
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//20070709修罗:catch分块.避免单个catch影响其余正常功能
void CWorldKernel::ProcessAccountNetMsg()
{
	bool	bGetData = true;
	char buffer[2048]="";
	while(true)
	{
		DEBUG_TRY
		int	nNetSize = 0;
		LPCTSTR pMsgsock = m_sock.GetPacket(&nNetSize, bGetData);
		if(!pMsgsock || nNetSize == 0)
			break;
		int nMsgSize = *((unsigned short*)pMsgsock);
		if(nNetSize < nMsgSize)
			break;
		
		if(nMsgSize>1024)
		{
			LogSave("收到一个超大包,不处理直接退出,size:%d",nMsgSize);
			m_sockForRelationServer.ClearPacket(nMsgSize);
			break;
		}
		///收到消息后马上将缓冲区数据放入堆栈中处理
		memcpy(buffer,pMsgsock,nMsgSize);
		m_sock.ClearPacket(nMsgSize);
		bGetData=false;
		LPCTSTR pMsg =buffer;

		// 服务器登录 ////////////////////////////////////////
		DEBUG_TRY
		CMsgLogin_A::MSG_Info*	pMsgLogin = (CMsgLogin_A::MSG_Info*)pMsg;
		if(pMsgLogin->unMsgType == _MSG_LOGIN_A)
		{
			CHECK(pMsgLogin->unMsgSize == sizeof(CMsgLogin_A::MSG_Info));
			ProcessLogin(pMsgLogin->idAccount, pMsgLogin->dwCheckData, pMsgLogin->usVersion, pMsgLogin->szUserName);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::pMsgLogin")

		// 玩家部分 ////////////////////////////////////////
		DEBUG_TRY
		CMsgConnect_A::MSG_Info*	pMsgConnect = (CMsgConnect_A::MSG_Info*)pMsg;
		if(pMsgConnect->unMsgType == _MSG_CONNECT)
		{
//			extern struct STAT_STRUCT	g_stat;
//			InterlockedIncrement( &g_stat.nRecvAccountPlayers );//07.5.21王玉波添加，用于测试帐号服务器性能
			
			CHECK(pMsgConnect->unMsgSize == sizeof(CMsgConnect_A::MSG_Info));
		//  [7/2/2007]	王玉波注释并修改----------begin-----------------------------------------------------------
		//	ClientConnect(pMsgConnect->idAccount, pMsgConnect->dwData, pMsgConnect->szInfo,0,LOGINOK);
			ClientConnect(pMsgConnect->idAccount, pMsgConnect->dwData, pMsgConnect->szInfo,-1,LOGINOK,pMsgConnect->nRoleCount);
		//	--------------------------------------end-------------------------------------------------------------
			//{
			//	CPlayer* pPlayer	= g_UserList.GetPlayerByAccountID(pMsgConnect->idAccount);
			//	ASSERT(pPlayer);
			//	if(pPlayer)
			//		UserList()->LoginToMapGroup(pPlayer);
			//}
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::pMsgConnect")


		DEBUG_TRY
		CMsgFee_A::MSG_Info*	pMsgFee = (CMsgFee_A::MSG_Info*)pMsg;
		if(pMsgFee->unMsgType == _MSG_FEE_A)
		{
			CHECK(pMsgFee->unMsgSize == sizeof(CMsgFee_A::MSG_Info));
			ProcessFee(pMsgFee->idAccount, pMsgFee->ucType);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::pMsgFee")

		DEBUG_TRY
		CMsgQueryFee_A::MSG_Info*	pMsgQFee = (CMsgQueryFee_A::MSG_Info*)pMsg;
		if(pMsgQFee->unMsgType == _MSG_QUERYFEE_A)
		{
			CHECK(pMsgQFee->unMsgSize == sizeof(CMsgQueryFee_A::MSG_Info));
	//----------------07.5.25王玉波注释以下一句,由帐号服务器发过来的计费信息------------------
	//		ProcessQueryFee(pMsgQFee->idAccount, pMsgQFee->dwData, pMsgQFee->nTime, pMsgQFee->ucType);
			continue;
		}
	//-----------07.01.06彭峰----检查增值服务返回消息------
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::pMsgQFee")



		DEBUG_TRY
		CMsgVAS::MSG_Info*	pMsgVAS = (CMsgVAS::MSG_Info*)pMsg;
		if(	pMsgVAS->unMsgType == _MSG_VAS)
		{
			CHECK( sizeof( *pMsgVAS ) == sizeof( CMsgVAS::MSG_Info ) );
			ProcessVAS(pMsgVAS->unMsgType, pMsgVAS->unMsgSize, pMsgVAS->usVASPoint, pMsgVAS->AccountID, pMsgVAS->PlayerID, pMsgVAS->usAction, pMsgVAS->usGoodSize, pMsgVAS->nGoods);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::pMsgVAS")
	
		DEBUG_TRY
		CMsgNewCard_w::MSG_Info*	pMsgNewCard = (CMsgNewCard_w::MSG_Info*)pMsg;
		if(	pMsgNewCard->unMsgType == _MSG_NEWCARD)
		{
			CHECK( sizeof( *pMsgNewCard ) == sizeof( CMsgNewCard_w::MSG_Info ) );
			ProcessNewCard(pMsgNewCard->unMsgType, pMsgNewCard->unMsgSize, pMsgNewCard->idUser, pMsgNewCard->idAccount, pMsgNewCard->dwType, pMsgNewCard->szBuf);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::pMsgVAS")

		DEBUG_TRY
		CMsgFatigue::MSG_Info*	pMsgFati = (CMsgFatigue::MSG_Info*)pMsg;
		if(	pMsgFati->unMsgType == _MSG_Fatigue)
		{
			CHECK( sizeof( *pMsgFati ) == sizeof( CMsgFatigue::MSG_Info ) );
			ProcessFATI(pMsgFati->unMsgSize, pMsgFati->unMsgSize, pMsgFati->AccountID, pMsgFati->PlayerID, pMsgFati->Fatigue);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::CMsgFatigue")
		DEBUG_TRY
		CMsgVAS::MSG_Info*	pMsgAward = (CMsgVAS::MSG_Info*)pMsg;
		if(	pMsgAward->unMsgType == _MSG_GAINAWARDS)
		{
			SendAwardInfoToMapGroup((const char*)pMsg, pMsgAward->unMsgSize);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::pMsgAward")
		DEBUG_TRY
		CMsgStopAccount::MSG_Info*	pMsgStopAccount = (CMsgStopAccount::MSG_Info*)pMsg;
		if(	pMsgStopAccount->unMsgType == _MSG_GMSTOPACCOUNT)
		{
			processStopAccount(pMsgStopAccount->unAciton,pMsgStopAccount->playerID,pMsgStopAccount->nParam);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::CMsgStopAccount")
//modify code by 林德渊 begin 2011.5.30
		DEBUG_TRY
		CMsgVasLv1::MSG_Info*	pMsgVASLv = (CMsgVasLv1::MSG_Info*)pMsg;
		if(	pMsgVASLv->unMsgType == _MSG_VASLV)
		{
			CMsgVasLv1 cmsg;
			cmsg.Create((char*)pMsg,pMsgVASLv->unMsgSize);
			cmsg.Process(NULL);
//			m_sock.ClearPacket(nMsgSize);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::pMsgVASLv1")
//modify code by 林德渊 end

		bGetData = false;

		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg 主循环出错坚持运行")
	} // for
}

///////////////////////////////////////////////////////////////////////////////////////
void CWorldKernel::ProcessLogin(OBJID idAccount, DWORD dwCheckData, int usVersion, LPCTSTR szUserName)
{
	if(idAccount == ID_NONE)
	{
		PrintText("Account server login failed!");
		return;
	}

	//??? 未检查版本号

	m_idServer	= idAccount;
	PrintText("Account server login OK。");
	SendLogin(idAccount, dwCheckData, usVersion, szUserName);
	m_nState = STATE_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::ClientConnect(OBJID idAccount, DWORD dwData, LPCTSTR szInfo, SOCKET_ID idSocket,int state,int nYoukeLogin/* = 0*/,ULONG nIP/*=0*/)			// return true: pass barrier
{
	CHECKF( !( idSocket == SOCKET_NONE && state !=  LOGINOK ) );//仅有帐号服务器发送时，idSocket为-1

	CConnectUser* pConnect = m_setConnectClient->GetObj(idAccount);

	//pConnect没有   1.账号服务器 客户端 第一次发过来   建立
	if(pConnect==NULL)
	{
		m_setConnectClient->DelObj(idAccount);	

		if( state != JOIN  )
		{
			//提前踢出  让后一个人进
// 			if(UserList()->GetPlayerByAccountID(idAccount))
// 			{
// 				UserList()->KickOutAccount(idAccount, STR_LOGIN_AGAIN);
// 			}

			pConnect = CConnectUser::CreateNew();
			CHECKF(pConnect->Create(idAccount, dwData, szInfo, idSocket,state,nYoukeLogin,nIP));
			m_setConnectClient->AddObj(pConnect);
		}
	}
	//有  可以补全  进入
	else if((pConnect->GetLoginState() ^ state ) == JOIN )	//如果客户端和验证服务器都发送了登陆信息
	{
		if(pConnect->CheckData(dwData))//匹配
		{
			ConnectInfo_ST info;
			info.idAccount	= pConnect->GetID();

			if( state == CLIENTOK )
				pConnect->SetSocketID( idSocket );	

			info.idSocket   = pConnect->GetSocketID();
			if (pConnect->GetULongIP()==0 && nIP!=0)//如果是账号服的消息先到，这个值有可能是0
				pConnect->SetULongIP(nIP);
			info.nIP		= pConnect->GetULongIP();
			//先去掉排队
// 			BOOL NotWaitQueue = CheckWaitQueue(info.idAccount,info.idSocket);//  [2/12/2007 Halei]--检查是否需要排队
// 			if( NotWaitQueue )//如果不用排队
// 			{

			pConnect->m_Youke |= nYoukeLogin;
			pConnect->SetLoginState( JOIN );//直接让玩家登陆
			if(!LoginUserSnap(info,pConnect->m_Youke))		//发送快照
			{
				m_setConnectClient->DelObj(idAccount);

			}
// 			}
// 			else
// 			{
// 			}
		}
		else //不匹配  相当于新连接
		{
			//提前踢出  让后一个人进
// 			if(UserList()->GetPlayerByAccountID(idAccount))
// 			{
// 				UserList()->KickOutAccount(idAccount, STR_LOGIN_AGAIN);
// 			}
			
			m_setConnectClient->DelObj(idAccount);
			pConnect = CConnectUser::CreateNew();
			CHECKF(pConnect->Create(idAccount, dwData, szInfo, idSocket,state,nYoukeLogin, nIP));
			m_setConnectClient->AddObj(pConnect);
		}

	}
	else if(pConnect->GetLoginState() == JOIN && state == JOIN  /*( LOGINOK | CLIENTOK )*/ )	//玩家已选择角色
	{
		ConnectInfo_ST info;
		info.idAccount = pConnect->GetID();
		info.idSocket	= pConnect->GetSocketID();
		if (pConnect->GetULongIP()==0 && nIP!=0)//如果是账号服的消息先到，这个值有可能是0
			pConnect->SetULongIP(nIP);
		info.nIP		= pConnect->GetULongIP();
		SafeCopy(info.szInfo, szInfo, _MAX_NAMESIZE);  // client szInfo is invalid!
		LoginUserByInfo(info);
	
		m_setConnectClient->DelObj(idAccount);
	}
	else//其他的当新连接
	{
		//提前踢出  让后一个人进
// 		if(UserList()->GetPlayerByAccountID(idAccount))
// 		{
// 			UserList()->KickOutAccount(idAccount, STR_LOGIN_AGAIN);
// 		}

		if(pConnect)
			pConnect->SetLoginState(pConnect->GetLoginState() ^ state);
		else
		{
			m_setConnectClient->DelObj(idAccount);
			pConnect = CConnectUser::CreateNew();
			CHECKF(pConnect->Create(idAccount, dwData, szInfo, idSocket,state,nYoukeLogin,nIP));
			m_setConnectClient->AddObj(pConnect);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
void CWorldKernel::ProcessFee(OBJID idAccount, int nType)
{
	switch(nType)
	{
	case	CMsgFee_A::HEARTBEAT:
		SendFee(idAccount, CMsgFee_A::HEARTBEAT);
		break;
	case	CMsgFee_A::FEE_KICKOUT:
//		LOGMSG("DEBUG：帐号服务器踢出帐号[%u]", idAccount);
		UserList()->KickOutAccount(idAccount, STR_LOGIN_ANOTHER);	// 不在线

// 		if(!UserList()->KickOutAccount(idAccount, STR_LOGIN_ANOTHER))		// 不在线
// 			SendFee(idAccount, CMsgFee_A::FEE_OFFLINE);
		break;
	default:
		break;
	}

}

void CWorldKernel::ProcessVAS(USHORT unMsgType, USHORT unMsgSize, ULONG nVASPoint, OBJID AccountID, OBJID PlayerID, USHORT usAction, USHORT usGoodSize, CMsgVAS::MsgTraffic* nGoods)
{

	CMsgVAS MsgVAS;
//	memcpy( &MsgVAS.m_pInfo->mtExtraInfo, m_pInfo, sizeof( MsgVAS.m_pInfo->mtExtraInfo ) );
	if( !UserList()->GetPlayer( PlayerID ) )
	{
		//LOGMSG("ERROR：找不到该玩家对象，ID：%d,当前usVASPoint值：%d,当前usAction: %d", PlayerID, nVASPoint,MsgVAS.m_pInfo->usAction );
// 		for (int i=0; i<usGoodSize; i++)
// 			::LogSave("CWorldKernel::ProcessVAS商城购物时玩家已不存在 user=%u,itemtype=%u,amount=%d", PlayerID, nGoods[i].itemID, nGoods[i].itemAmount);
		//记录详细日志
		if (usAction==_MSGVAS_BUY_SUC || usAction==_MSGVAS_POINT_BUY_SUC || usAction==_MSGVAS_BUY_ASALE_SUC)
		{
			for (int i=0; i<usGoodSize; i++)
				::LogSave("CWorldKernel::ProcessVAS商城购物,玩家不存在 user=%u,itemtype=%u,amount=%d", PlayerID, nGoods[i].itemID, nGoods[i].itemAmount);
		}
		if (usAction==_MSGVAS_ADD_VAS_SUC)
		{
			CMsgVAS msg;
			if (msg.Create(nGoods[0].itemAmount, AccountID, PlayerID, _MSGVAS_SPEND_VAS, 0, 0, 0))
			{
				msg.m_pInfo->nGoods[0].dwData = 0;
				msg.m_pInfo->nGoods[0].itemAmount = 0;
				msg.m_pInfo->nGoods[0].itemID = 0;
				msg.m_pInfo->usGoodSize = 1;
				GameWorld()->SendVASRequest(unMsgType, unMsgSize, msg.m_pInfo->usVASPoint, msg.m_pInfo->AccountID, msg.m_pInfo->PlayerID, msg.m_pInfo->usAction, msg.m_pInfo->usGoodSize, msg.m_pInfo->nGoods);
			}
			::LogSave("CWorldKernel::ProcessVAS 玩家通过元宝票领取了元宝，但未能找到此玩家并删除元宝票，将元宝扣回 user=%u,itemtype=%u,amount=%d", PlayerID, nGoods[0].itemID, nGoods[0].itemAmount);
		}
		if (usAction==_MSGVAS_SPEND_VAS_SUC)
		{
			if (nGoods[0].itemID==_SPCITEM_VASEXCHANGE)
				::LogSave("CWorldKernel::ProcessVAS 添加元宝票时找不到玩家(userid:%u,amount:%u)",PlayerID, nGoods[0].dwData);
		}
		return ;
	}
	int nMapGroup = UserList()->GetPlayer( PlayerID )->GetProcessID();
	SOCKET_ID idSocket = UserList()->GetPlayer( PlayerID )->idSocket;
	MsgVAS.Create(nVASPoint, AccountID, PlayerID, usAction);
	if (usGoodSize>0)
	{
		for (int i=0; i<usGoodSize; i++)
			MsgVAS.Add(nGoods[i].itemID, nGoods[i].itemAmount, nGoods[i].dwData);
	}
	SendMapGroupMsgForVAS( nMapGroup, idSocket,  &MsgVAS );
}

///////////////////////////////////////////////////////////////////////////////////////
enum { c_typeNone = 0, c_typePoint, c_typeTime, c_typeNetBarPoint, c_typeNetBarTime, c_typeISP, c_typeFree, c_typeAll };
void CWorldKernel::ProcessQueryFee(OBJID idAccount, DWORD dwData, int nTime, int ucType)
{
	CPlayer*  pUser	=UserList()->GetPlayerByAccountID(idAccount);
	if(pUser)
	{
		char szMsg[1024] = STR_FEETYPE_KNOWN;
		switch(ucType)
		{
		case	c_typePoint:
			sprintf(szMsg, STR_FEETYPE_HOUR, dwData/20, (dwData/2)%10);
			break;
		case	c_typeTime:
			sprintf(szMsg, STR_FEETYPE_MONTH, nTime/10000, (nTime/100) % 100, nTime % 100);
			break;
		case	c_typeNetBarPoint:
			sprintf(szMsg, STR_FEETYPE_BARHOUR, dwData);
			break;
		case	c_typeNetBarTime:
			sprintf(szMsg, STR_FEETYPE_BARMONTH, dwData);
			break;
		case	c_typeISP:
			sprintf(szMsg, STR_FEETYPE_ISP, dwData);
			break;
		case	c_typeFree:
			sprintf(szMsg, STR_FEETYPE_FREE, dwData);
			break;
		}

		CMsgTalk	msg;
		IF_OK(msg.Create(SYSTEM_NAME, pUser->szName, szMsg))
			pUser->SendMsg(&msg);


		szMsg[0]	= 0;
		if(strcmp(pUser->szNotify, FLAG_ISP_TO_NORMAL) == 0)			// ★注意：该字符串必须与帐号服务器同步
			SafeCopy(szMsg, STR_LOGIN_ISP_FAIL);
		else if(strcmp(pUser->szNotify, FLAG_NO_POINT) == 0)
			SafeCopy(szMsg, STR_ACCOUNT_NOFEE);
		else if(pUser->szNotify[0] >= '0' && pUser->szNotify[0] <= '9')
			sprintf(szMsg, STR_FEW_FEE_NOTIFY, pUser->szNotify);
		else
			SafeCopy(szMsg, pUser->szNotify, _MAX_NAMESIZE);
		if(strlen(szMsg) > 0)
		{
			CMsgTalk	msg;
			IF_OK(msg.Create(SYSTEM_NAME, pUser->szName, szMsg))
				pUser->SendMsg(&msg);
		}
	}
	else
		LOGERROR("帐号服务器返回了未登录玩家的查询计费消息");
}

///////////////////////////////////////////////////////////////////////////////////////
// global entry
///////////////////////////////////////////////////////////////////////////////////////
IWorld* IWorld::CreateNew()
{
	return new CWorldKernel;
}

//////////////////////////////////////////////////////////////////////
// syndicate
//////////////////////////////////////////////////////////////////////
bool CWorldKernel::CreateSyndicate		(const CreateSyndicateInfo* pInfo)
{
	for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
	{
		m_pMsgPort->Send(i, KERNEL_CREATESYNDICATE, STRUCT_TYPE(CreateSyndicateInfo), pInfo);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::DestroySyndicate	(OBJID idSyn, OBJID idTarget)
{
	OBJID	setID[2];
	setID[0]	= idSyn;
	setID[1]	= idTarget;
	for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
	{
		m_pMsgPort->Send(i, KERNEL_DESTROYSYNDICATE, STRUCT_TYPE(setID), &setID);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ChangeSyndicate		(const SynFuncInfo0* pFuncInfo)
{
	for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
	{
		m_pMsgPort->Send(i, KERNEL_CHANGESYNDICATE, BUFFER_TYPE(pFuncInfo->nSize), pFuncInfo);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
enum { CONFIGDATA_NAME=1, CONFIGDATA_DATA, CONFIGDATA_STRING };
bool CWorldKernel::LoadConfig(IDatabase *pDb)
{
	return true;
}

//////////////////////////////////////////////////////////////////////
// 广播给指定地图组处理
bool CWorldKernel::TransmitMapGroupMsg(PROCESS_ID idProcess,CNetMsg* pMsg)
{
	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= pMsg->GetSocketID();
	pPacket->idNpc		= pMsg->GetNpcID();
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->nTrans		= pMsg->GetTransData();
	
	//	return m_pMsgPort->Send(idProcess, MAPGROUP_SENDMSG,BUFFER_TYPE(pMsg->GetSize()), pMsg->GetBuf());
	return m_pMsgPort->Send(idProcess, MAPGROUP_SENDMSG, STRUCT_TYPE(buf), pPacket);
}

// 广播给指定地图组处理
void CWorldKernel::TransmitBroadcastMapGroupMsg(CNetMsg* pMsg,PROCESS_ID idIProcessIgnore)
{
	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= pMsg->GetSocketID();
	pPacket->idNpc		= pMsg->GetNpcID();
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->nTrans		= pMsg->GetTransData();
	
	for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
	{
		if(i!=idIProcessIgnore)
			m_pMsgPort->Send(i, MAPGROUP_SENDMSG, STRUCT_TYPE(buf), pPacket);
	}
}

// 广播给指定地图组处理
void CWorldKernel::TransmitBroadcastMapGroupMsg(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize, int nTrans,PROCESS_ID idIProcessIgnore)
{
	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= -1;
	pPacket->idNpc		= -1;
	pPacket->idPacket	= idMsg;
	memcpy(pPacket->buf, pbufMsg, dwMsgSize);
	pPacket->nSize		= dwMsgSize;
	pPacket->nTrans		= nTrans;
	
	for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
	{
		if(i!=idIProcessIgnore)
			m_pMsgPort->Send(i, MAPGROUP_SENDMSG, STRUCT_TYPE(buf), pPacket);
	}
}

//////////////////////////////////////////////////////////////////////
// return true: 登录成功，不再需要登录配对信息
bool CWorldKernel::LoginUserByInfo(const ConnectInfo_ST& LoginInfo)
{	
	OBJID idAccount = LoginInfo.idAccount;
	// check repeat user_id
	if(UserList()->GetPlayerByAccountID(idAccount))
	{
		//LOGMSG("DEBUG：帐号[%u]重复登录，踢前一人下线 !", idAccount);
		UserList()->KickOutAccount(idAccount, STR_LOGIN_AGAIN);
// 		CMsgTalk msg;
// 		if( msg.Create( SYSTEM_NAME, ALLUSERS_NAME, "MultiLogin", NULL, _COLOR_WHITE, _TXTATR_ENTRANCE ) )
// 			SendClientMsg( LoginInfo.idSocket, &msg );
 		return false;
	}
	// OK
	int ret = UserList()->LoginUser(idAccount, LoginInfo.idSocket, LoginInfo.szInfo, LoginInfo.nIP);		// enum { LOGIN_OK, LOGIN_NEW, LOGIN_BAN };
	if (ret == LOGIN_OK) 
	{
		CPlayer* pPlayer	= pPlayer = g_UserList.GetPlayerByAccountID(idAccount);	
		ASSERT(pPlayer);
		if(pPlayer)
			UserList()->LoginToMapGroup(pPlayer);
		return true;
	}
	else if(ret == LOGIN_BAN)
	{
		CMsgTalk msg;
		if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, "错误：余额不足，请及时充值!", NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
			SendClientMsg(LoginInfo.idSocket, &msg);
		return false;
	}
	/************************************************************************/
	/*[游途道标 2009.07.13]                                                 */
	else
	{
		CMsgTalk msg;
		if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME,
			           "错误：游戏服务器查询出错,请重试!",
					   NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
			SendClientMsg(LoginInfo.idSocket, &msg);
		return false;
	}
	/************************************************************************/

	return false;
}

void CWorldKernel::KickOutSocket(SOCKET_ID idSocket)
{
	UserList()->KickOutSocket(idSocket);
}

bool CWorldKernel::LoginUserSnap(const ConnectInfo_ST &LoginInfo,int nYouke)//李京哲0307 玩家快照
{
	bool bYouke = false;
// 	if(nYouke > 0)
// 		bYouke = true;

	UserList()->LoginUserSnap(LoginInfo.idAccount,LoginInfo.idSocket,bYouke);
	return true;
}


bool CWorldKernel::DeleteRole(OBJID idAccount, DWORD dwData,LPCTSTR password, LPCTSTR name)//李京哲0327 在登陆界面删除人物
{
	SQLBUF	szSQL;				
	sprintf(szSQL, "SELECT id, mate,syn_id FROM %s WHERE account_id='%u' and name = '%s' and protect_passwd = '%s'",_TBL_USER, idAccount,name,password);
	
	GameWorld()->GetDatabase();
	IRecordset*	pRes = GetDatabase()->CreateNewRecordset(szSQL);		
	if(pRes)
	{
			//做其他系统的时候修改
// 			char text[16];				
// 			pRes->LoadString(text, "mate", _MAX_NAMESIZE);			
// 			sprintf(szSQL, "UPDATE %s SET mate='%s' WHERE name='%s' LIMIT 1",
// 				_TBL_USER,
// 				NOMATE_NAME,
// 				text);
// 			GetDatabase()->ExecuteSQL(szSQL);
// 
 		OBJID	idUser	= pRes->LoadDWord("id");				//清除其他关系
// 			OBJID   idSyn	= pRes->LoadDWord("syn_id");
// 			CSyndicateWorld *pSyn = QuerySynManager()->QuerySyndicate(idSyn);
// 			if(pSyn != NULL)
// 				pSyn->SynAttorn(idUser);
// 			// delete all friend info
// 			sprintf(szSQL, "DELETE FROM %s WHERE userid=%u or friendid=%u", _TBL_FRIEND,idUser, idUser);
// 			GetDatabase()->ExecuteSQL(szSQL);
// 			// delete all enemy info
// 			sprintf(szSQL, "DELETE FROM %s WHERE userid=%u or enemy=%u", _TBL_ENEMY, idUser, idUser);
// 			GetDatabase()->ExecuteSQL(szSQL);
// 			// delete all contact_history info独孤求败20081217
// 			sprintf(szSQL, "DELETE FROM %s WHERE userid=%u or friendid=%u", _TBL_FRIEND_CONTACTHISTORY, idUser, idUser);
// 			GetDatabase()->ExecuteSQL(szSQL);
// 			// delete all task info
// 			//sprintf(szSQL, "DELETE FROM %s WHERE userid=%u", _TBL_TASKDETAIL, idUser);
// 			//GetDatabase()->ExecuteSQL(szSQL);
// 			// copy user record to cq_deluser
//			sprintf(szSQL, "DELETE FROM %s WHERE userid=%u", _TBL_USERKEY, idUser);
//			GetDatabase()->ExecuteSQL(szSQL);


		sprintf(szSQL, "INSERT INTO %s SELECT * FROM %s WHERE id=%u", _TBL_DELUSER, _TBL_USER, idUser);
		GetDatabase()->ExecuteSQL(szSQL);

		sprintf(szSQL, "DELETE FROM %s WHERE id=%u", _TBL_USER, idUser);
		GetDatabase()->ExecuteSQL(szSQL);


		pRes->Release();
		return true;
	}
	return false;
}
//  [2/12/2007 HALEI]//玩家登陆等待检查是否需要排队算法--------------------------------------------------
BOOL CWorldKernel::CheckWaitQueue( OBJID idAccount,SOCKET_ID usersock )//检查是否需要排队的算法
{
	//首先得到需要排队的人数(时间),用MustWaitTime保存
	int MustWaitTime  = GameWorld()->QueryUserList()->GetUserAmount()+1/*+m_setConnectClient->GetAmount()-WaitQueue.size()*/-NOWAITPLAYER;
	bool FindInQueue = false;		//初始化在数组中找到对象为没有找到.
	if ( MustWaitTime <= 0 )		//判断是否需要排队.
	{	//如果不需要排队
		int infosize = WaitQueue.size();//得到排队队列的大小,看有多少人在排队
		if (infosize != 0)			//如果里面还有玩家
		{			
			for (int i =0;i<infosize;i++)
			{
				if (WaitQueue[i].idAccount == idAccount)	//先遍历是否队列中玩家和目前登陆的玩家相同.
				{
					FindInQueue = true;						//证明在队列里找到他了.
					CMsgWaitQueue msg;
					if (msg.Create(idAccount,0,NETWORK_WAITBREAK))	//中断他以前登陆的排队	
						GameWorld()->SendClientMsg(WaitQueue[i].idSocket,&msg);					
					WaitQueue.erase( WaitQueue.begin()+i );//擦掉他以前排队的数据.					
					break;
				}				
			}
		}

		CMsgWaitQueue msg;
		if (msg.Create(idAccount,0,1))	//发送消息告诉玩家你可以进入了.
		{
			GameWorld()->SendClientMsg(usersock,&msg);
		}
		return true;
	}
	else
	{	//如果需要排队

		int infosize = WaitQueue.size();//首先得到队列的大小
		if (infosize != 0)//如果队列中有玩家
		{
			int i = 0;
// 			for ( i =0; i<infosize && i< 10; i++ )	//  如果有玩家,把正确排队的时间告诉队列里的前10个玩家.
// 			{	//如果队列里存的等待时间    大于   真实需要排队的时间
// 				if( WaitQueue[i].EndWaitTime > ( GetTickCount()/1000+MustWaitTime*WAITQUEUETIME+infosize*10 ) )
// 				{
// 					WaitQueue[i].EndWaitTime = MustWaitTime*WAITQUEUETIME+infosize*10;
// 					CMsgWaitQueue waitqueuemsg;		//发送给客户端正确的排队时间
// 					if (waitqueuemsg.Create( WaitQueue[i].idAccount,MustWaitTime*WAITQUEUETIME+infosize*10,NETWORK_LOGIN ) )
// 					{
// 						GameWorld()->SendClientMsg(WaitQueue[i].idSocket,&waitqueuemsg);
// 					}					
// 				}
// 			}
			for ( i =0;i<infosize;i++)
			{
				if (WaitQueue[i].idAccount == idAccount)	//先遍历是否队列中玩家和目前登陆的玩家相同.
				{
					int testsocket = WaitQueue[i].idSocket;
					FindInQueue = true;						//证明在队列里找到他了.
					CMsgWaitQueue msg;
					if (msg.Create(idAccount,0,NETWORK_WAITBREAK))	//中断他以前登陆的排队	
						GameWorld()->SendClientMsg(WaitQueue[i].idSocket,&msg);
					WaitQueue.erase( WaitQueue.begin()+i );//擦掉他以前排队的数据.重新让他排队.
					ConnectInfo_WaitQueue info;
					info.EndWaitTime = (GetTickCount()/1000+MustWaitTime*WAITQUEUETIME+infosize*10);
					info.idAccount = idAccount;	
					info.idSocket = usersock;
					WaitQueue.push_back( info );		//把他重新压入队列
					
					CMsgWaitQueue waitqueuemsg;
					waitqueuemsg.Create( info.idAccount,MustWaitTime*WAITQUEUETIME+infosize*10,NETWORK_LOGIN );
					GameWorld()->SendClientMsg(usersock,&waitqueuemsg);	//告诉客户端.他需要排队
					return false;
				}
				
			}
			if (!FindInQueue)	//如果没有在队列里找到他.证明他是新登陆排队.
			{
			
				ConnectInfo_WaitQueue info;
				info.EndWaitTime = (GetTickCount()/1000+MustWaitTime*WAITQUEUETIME+infosize*10);
				info.idAccount = idAccount;	
				info.idSocket = usersock;
				WaitQueue.push_back( info );		//直接压入队列
				
				CMsgWaitQueue msg;//告诉他需要排队
				if ( msg.Create( info.idAccount,MustWaitTime*WAITQUEUETIME+infosize*10,NETWORK_LOGIN ) )
				{
						GameWorld()->SendClientMsg(usersock,&msg);
				}
				
			
				return false;
			}
		}
		else
		{
			
			ConnectInfo_WaitQueue info;		//如果队列里没人.
			info.EndWaitTime = (GetTickCount()/1000+MustWaitTime*WAITQUEUETIME);
			info.idAccount = idAccount;	
			info.idSocket = usersock;
			WaitQueue.push_back( info );	//直接把他压进去
			
			CMsgWaitQueue msg;
			msg.Create( info.idAccount,MustWaitTime*WAITQUEUETIME,NETWORK_LOGIN );
			GameWorld()->SendClientMsg(usersock,&msg);		//告诉他需要排队
			return false;
			
		}
	
	return false;
	}
	return false;
}

void CWorldKernel::ConnectShutDown(SOCKET_ID usersock)
{
// 	CConnectSet::Iter it = NULL;
// 	for( it = m_setConnectClient->Begin();it != m_setConnectClient->End();it++ )
// 	{
// 		CConnectUser* pConnectUser = m_setConnectClient->GetObjByIter( it );
// 		if( pConnectUser != NULL )
// 		{
// 			m_setConnectClient->DelObj(pConnectUser->GetID());
// 			return;
// 		}
// 	}
}

//  [2/12/2007 Halei] 处理当玩家排队时,无耐心等待异常退出的情况
void CWorldKernel::WaitQueueSocketShutDown( SOCKET_ID usersock )
{
	int QueueSize = WaitQueue.size();
	for (int i =0; i< QueueSize; i++ )
	{
		if ( usersock == WaitQueue[i].idSocket )
		{
			WaitQueue.erase( WaitQueue.begin()+i );		//从队列中删除
			break;
		}
	}
}
// 王玉波 [5/26/2007]
// 客户端关闭，此人未进入游戏，就不会在userlist中
bool CWorldKernel::EndConnectUserBySocket( SOCKET_ID idSocket )
{
	CConnectSet::Iter it;
	for( it = m_setConnectClient->Begin();it != m_setConnectClient->End();it++ )
	{
		CConnectUser* pConnectUser = m_setConnectClient->GetObjByIter( it );
		if( pConnectUser != NULL )
		{
			if( pConnectUser->GetSocketID() == idSocket )
			{
				OBJID idAccount = pConnectUser->GetID();
				if( idAccount != ID_NONE )
				{
					GameWorld()->SendFee( idAccount,CMsgFee_A::FEE_END );
					GameWorld()->SendFee(idAccount,CMsgFee_A::FEE_KICKASS);
					m_setConnectClient->DelObj( idAccount );	//
					return true;
				}
			}
		}
	}
	return false;
}
//  [7/2/2007]
//	王玉波
bool CWorldKernel::EndConnectUserByAccount( OBJID idAccount )
{
	CHECKF( idAccount != ID_NONE );

	CConnectSet::Iter it;
	for( it = m_setConnectClient->Begin();it != m_setConnectClient->End();it++ )
	{
		CConnectUser* pConnectUser = m_setConnectClient->GetObjByIter( it );
		if( pConnectUser && pConnectUser->GetID() == idAccount )
		{
			
			GameWorld()->SendFee( idAccount,CMsgFee_A::FEE_END );
			m_setConnectClient->DelObj( idAccount );	//
			return true;
		}
	}
	return false;
}
// 王玉波 [6/7/2007]
// 帐号服务器要求踢出玩家,但此时玩家也许并未进入游戏,此时在UserList中找不到
// 在ConnectUser中找到此玩家的socket,关闭
SOCKET_ID CWorldKernel::GetSocketFromConnectUser( OBJID idAccount )
{
	CHECKF( idAccount != ID_NONE );
	CConnectSet::Iter it;
	for( it = m_setConnectClient->Begin();it != m_setConnectClient->End();it++ )
	{
		CConnectUser* pConnectUser = m_setConnectClient->GetObjByIter( it );
		if( pConnectUser != NULL )
		{
			if( pConnectUser->GetID() == idAccount )
			{
				return pConnectUser->GetSocketID();
			}
		}
	}
	return SOCKET_NONE;
}
// 王玉波 [6/07/2007]
// 帐号服务器要求关闭此帐号的连结,而此时此人并未在userlist中
//void CWorldKernel::KickOutConnectUserByAccount( OBJID idAccount )
//{
//	CHECK( idAccount != ID_NONE );
//	CConnectSet::Iter it = NULL;
//	for( it = m_setConnectClient->Begin();it != m_setConnectClient->End();it++ )
//	{
//		CConnectUser* pConnectUser = m_setConnectClient->GetObjByIter( it );
//		if( pConnectUser != NULL )
//		{
//			if( pConnectUser->GetID() == idAccount )
//			{
//				//关闭与客户端的连接
//				GameWorld()->GetSocketInterface()->CloseSocket( pConnectUser->GetSocketID() );
//				//senfFee to AccountServer
//				GameWorld()->SendFee( idAccount,CMsgFee_A::FEE_END );
//				m_setConnectClient->DelObj( idAccount );	//
//				break;
//			}
//		}
//	}
//}

void CWorldKernel::ProcessMonitorNetMsg()
{
	bool	bGetData = true;

	if(clock() - m_dwSendMonitorServer > MONITOR_SERVER_SEND_INTERVAL)
	{	

		SendServerTrace(MONITOR_LOG2, g_szKernelState);
		SendServerTrace(MONITOR_LOG1, g_szShellState);

		m_dwSendMonitorServer = MONITOR_SERVER_SEND_INTERVAL;
	}

	while(true)
	{
		int	nNetSize = 0;
		LPCTSTR pMsg = m_sockMonitor.GetPacket(&nNetSize, bGetData);
		if(!pMsg || nNetSize == 0)
			break;
		int nMsgSize = *((unsigned short*)pMsg);
		if(nNetSize < nMsgSize)
			break;

		DEBUG_TRY
		CMsgCentralMonitor::MSG_Info*	pMsgMonitor = (CMsgCentralMonitor::MSG_Info*)pMsg;
		if(	pMsgMonitor->unMsgType == _MSG_MONITORSERVER_MONITOR)
		{
			if(pMsgMonitor->nTraceType == MONITOR_SHUTDONW)
			{
				int n = 0;
				m_pMsgPort->Send(MSGPORT_SHELL, SHELL_REMOTE_CLOSE, VARTYPE_INT, &n);
			}

			m_sockMonitor.ClearPacket(nMsgSize);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessMonitorNetMsg::CMsgCentralMonitor")
		bGetData = false;
	} // for
}

void CWorldKernel::SendServerTrace(int nType, char* pszTrace)
{
	
	int nTraceLength = strlen(pszTrace);
	int nMsgCount = (nTraceLength + MAX_STRING_SIZE_PER_MSG - 1 ) / MAX_STRING_SIZE_PER_MSG;

	for(int i = 0; i < nMsgCount; i++)
	{
		BOOL bLastOne = (i == (nMsgCount - 1)) ? TRUE : FALSE;	//	是否为最后一个包
		int	 nLength = bLastOne ? (nTraceLength % MAX_STRING_SIZE_PER_MSG) : MAX_STRING_SIZE_PER_MSG;	//	字符串长度
// 
// 		CMsgAccount_A::MSG_Info	msg;
// 		msg.unMsgSize	= sizeof(msg);
// 		msg.unMsgType	= _MSG_MONITORSERVER_MONITOR;
// 		
// 		
// 		strcpy(msg.szServerName, m_szServer);
// 		strcpy(msg.szServerIP, m_szServer);
// 		msg.nTraceType = nType;
// 		msg.nSerialNumber = nSerialNumber;
// 		msg.bLastOne = bLastOne;
// 		strncpy(msg.szText, pszTrace, nLength);
// 		msg.nTextSize = nLength;
// 		
		CMsgCentralMonitor msg;
		IF_NOT(msg.Create(m_szServer, m_szServer, nType, i, bLastOne, 
			pszTrace + i * 900, nLength))
			return;

		m_sockMonitor.SendPacket((LPCTSTR)msg.m_pInfo, sizeof(CMsgCentralMonitor::MSG_Info) + nLength - 1, true);
	}
}

bool CWorldKernel::ReconnectDB()
{
	if(!g_bEnableDatabaseReconnect)
		return true;

	SYSTEMTIME curTime;
	::GetLocalTime( &curTime );
	if( curTime.wDay != m_stLastDatabaseReconnectTime.wDay 
		&& curTime.wHour == 4)	
//	if( curTime.wMinute != m_stLastDatabaseReconnectTime.wMinute)
	
	{
		// 初始化数据库
		char	DB_IP	[DBSTR_SIZE] = "";
		char	DB_USER	[DBSTR_SIZE] = "";
		char	DB_PW	[DBSTR_SIZE] = "";
		char	DB_DB	[DBSTR_SIZE] = "";
		
		CIniFile	ini(CONFIG_FILENAME, "Database");
		ini.GetString(DB_IP,	"DB_IP",	DBSTR_SIZE);
		ini.GetString(DB_USER,	"DB_USER",	DBSTR_SIZE);
		ini.GetString(DB_PW,	"DB_PW",	DBSTR_SIZE);
		ini.GetString(DB_DB,	"DB_DB",	DBSTR_SIZE);
	//	m_pDb	= ::CreateDatabase(DB_IP, DB_USER, DB_PW, DB_DB);
		BOOL bSuccess	= ::ReconnectDatabase(m_pDb, DB_IP, DB_USER, DB_PW, DB_DB);
		
		
		if(!bSuccess)
		{
			LOGERROR("数据库重新连接失败!, m_idProcess = %d", m_idProcess);
			return false;
		}
		
		LOGMSG("数据库连接成功, m_idProcess = %d", m_idProcess);

		::GetLocalTime(&m_stLastDatabaseReconnectTime);
	}

	return true;
}

bool CWorldKernel::SendTutorToMapGroup(const ChgTutorInfo* pInfo)
{
	for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
	{
		m_pMsgPort->Send(i, MAPGROUP_CHGTUTOR, STRUCT_TYPE(ChgTutorInfo), pInfo);
	}
	return true;
}
bool CWorldKernel::SendMapGroupMsg(CNetMsg *msg)
{
	for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
	{
		TransmitMapGroupMsg(i,msg);
	}
	return true;
}
void CWorldKernel::BroadcastMainServerMsg(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize, int nTrans,PROCESS_ID idIProcess)
{
	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= -1;
	pPacket->idNpc		= -1;
	pPacket->idPacket	= idMsg;
	memcpy(pPacket->buf, pbufMsg, dwMsgSize);
	pPacket->nSize		= dwMsgSize;
	pPacket->nTrans		= nTrans;
	
// 	for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
// 	{
// 		if(i!=idIProcessIgnore)
			m_pMsgPort->Send(idIProcess, MAPGROUP_MAINSERVER, STRUCT_TYPE(buf), pPacket);
//	}
}

bool CWorldKernel::SendFATIRequest(USHORT unMsgType, USHORT unMsgSize, OBJID AccountID, OBJID PlayerID, int Fatigue)
{
	CMsgFatigue::MSG_Info msg;
	msg.unMsgType = unMsgType;
	msg.unMsgSize = unMsgSize;
	msg.AccountID	=	AccountID;
	msg.PlayerID	=	PlayerID;
	msg.Fatigue		=	Fatigue;
	
	//[游途道标 2009.01.08]
	if (!msg.unMsgSize)
	{
		msg.unMsgSize = sizeof(CMsgFatigue::MSG_Info);
	}
	
	return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);

}

void CWorldKernel::ProcessNewCard(USHORT unMsgType, USHORT unMsgSize, DWORD idUser , DWORD idAcount , DWORD dwType, char* pszBuf)
{
	CMsgNewCard_w msgNewCard;
	if( !UserList()->GetPlayer( idUser ) )
	{
		//LOGMSG("ERROR：找不到该玩家对象，ID：%d,type值：%d", idUser, dwType);
		return ;
	}
	
	int nMapGroup = UserList()->GetPlayer( idUser )->GetProcessID();
	SOCKET_ID idSocket = UserList()->GetPlayer( idUser )->idSocket;
	msgNewCard.Create(dwType, idUser, idAcount, pszBuf);
	
	SendMapGroupMsgForChatRoom( nMapGroup, idSocket,  &msgNewCard );
}

void CWorldKernel::processStopAccount(USHORT nAction, OBJID playerID, UINT nParam)
{
	if( !UserList()->GetPlayer( playerID ) )
	{
		//LOGMSG("ERROR：找不到该玩家对象，ID：%d", playerID);
		return ;
	}
	int nMapGroup = UserList()->GetPlayer(playerID)->GetProcessID();
	SOCKET_ID idSocket = UserList()->GetPlayer(playerID)->idSocket;
	CMsgStopAccount msg;
	msg.Create(nAction,playerID,nParam);
	SendMapGroupMsgForChatRoom( nMapGroup, idSocket,  &msg );
}

void CWorldKernel::ProcessFATI(USHORT unMsgType, USHORT unMsgSize, OBJID AccountID, OBJID PlayerID, int Fatigue)
{
	CMsgFatigue MsgFatigue;
	//	memcpy( &MsgVAS.m_pInfo->mtExtraInfo, m_pInfo, sizeof( MsgVAS.m_pInfo->mtExtraInfo ) );
	if( !UserList()->GetPlayer( PlayerID ) )
	{
		//LOGMSG("ERROR：找不到该玩家对象，ID：%d,当前usFATIGUE值：%d", PlayerID, Fatigue );
		return ;
	}
	int nMapGroup = UserList()->GetPlayer( PlayerID )->GetProcessID();
	SOCKET_ID idSocket = UserList()->GetPlayer( PlayerID )->idSocket;
	MsgFatigue.Create( AccountID, PlayerID, Fatigue);

	SendMapGroupMsgForChatRoom(nMapGroup, idSocket,  &MsgFatigue);
}

bool CWorldKernel::SendMapGroupMsgForChatRoom(int nMapGroup,SOCKET_ID idSocket,CNetMsg *pMsg)
{
	ASSERT(idSocket != SOCKET_NONE);
	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= idSocket;
	pPacket->idNpc		= ID_NONE;
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->nTrans		= pMsg->GetTransData();
	CHECKF( nMapGroup >= MSGPORT_MAPGROUP_FIRST && nMapGroup < m_pMsgPort->GetSize());
	
	m_pMsgPort->Send(nMapGroup, MAPGROUP_TRANSMITMSG_FORCHATROOM, STRUCT_TYPE(buf), pPacket);
	
	return true;
}
//给创建角色中的用户广播信息
void CWorldKernel::BroadcastRegisterMsg(OBJID idMsg, const char *pBuf, int nMsgLen, SOCKET_ID idSocketIgnore)
{
	CConnectSet::Iter it;
	for( it = m_setConnectClient->Begin();it != m_setConnectClient->End();it++ )
	{
		CConnectUser* pConnectUser = m_setConnectClient->GetObjByIter( it );
		if( pConnectUser != NULL && pConnectUser->GetSocketID()!=idSocketIgnore)
		{
			SendMsg(pConnectUser->GetSocketID(),idMsg,pBuf,nMsgLen);
		}
	}
}
//元宝的消息
bool CWorldKernel::SendMapGroupMsgForVAS(int nMapGroup,SOCKET_ID idSocket,CNetMsg *pMsg)
{
	ASSERT(idSocket != SOCKET_NONE);
	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= idSocket;
	pPacket->idNpc		= ID_NONE;
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->nTrans		= pMsg->GetTransData();
	CHECKF( nMapGroup >= MSGPORT_MAPGROUP_FIRST && nMapGroup < m_pMsgPort->GetSize());
	
	m_pMsgPort->Send(nMapGroup, MAPGROUP_VASMSG, STRUCT_TYPE(buf), pPacket);
	
	return true;
}

bool CWorldKernel::SendLastLineToAcc(UINT nMsgType, UINT nAction, OBJID accountID, OBJID idUser, const char* szServerName, const GainAwardInfo* gainInfo, int nAmount, int nMsgSize)
{
	CHECKF(nMsgType);
	CHECKF(nMsgSize);

	CMsgAward::MSG_Info msg;
	msg.unMsgType = nMsgType;
	msg.unMsgSize = nMsgSize;
	msg.nAction = nAction;
	msg.accountID = accountID;
	msg.idUser = idUser;
	SafeCopy(msg.szServerName, szServerName, _MAX_NAMESIZE);

	for (int i=0; i<nAmount; i++)
	{
		msg.gaInfo[i].nType = gainInfo[i].nType;
		msg.gaInfo[i].nValue = gainInfo[i].nValue;
	}
	
	int nSize = nMsgSize;
	if (!nSize)
	{
		nSize = sizeof(CMsgVAS::MSG_Info);
	}

	return m_sock.SendPacket((LPCTSTR)&msg, nSize, true);
}

bool CWorldKernel::SendAwardInfoToMapGroup(const char* pInfo, int nSize)
{
	MESSAGESTR	buf;
    CLIENTMSG_PACKET0* pPacket = (CLIENTMSG_PACKET0*)buf;
	pPacket->nSize = nSize;
	pPacket->idSocket	= 0;
	pPacket->idPacket	= 0;
	memcpy(pPacket->buf, pInfo, nSize);
	
	for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
	{
		m_pMsgPort->Send(i, MAPGROUP_AWARDINFO, STRUCT_TYPE(buf), pInfo);
	}
	return true;
}
//modify code by 林德渊 begin 2011.5.30
bool CWorldKernel::SendVASLVRequest(USHORT unMsgType,unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan)
{
	CMsgVasLv1::MSG_Info msg;
	msg.unMsgType = unMsgType;
	msg.usAction = usAction;
	msg.id = id;
	msg.vastotal = vastotal;
	msg.lvtotal = lvtotal;
	msg.dwcount = dwcount;
	msg.vaslv = vaslv;
	msg.awarditem = awarditem;
	msg.viscou = viscou;
	msg.maxvis = maxvis;
	msg.vascount = vascount;
	msg.questionid = questionid;
	msg.questionan = questionan;
	
	msg.unMsgSize = sizeof(CMsgVasLv1::MSG_Info);
	
	return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}
//modify code by 林德渊 end

int CWorldKernel::GetLineIndex()
{
	int nLine = 0;
	if(m_szServer && strlen(m_szServer)>0)
	{
		if (strcmp(m_szServer, "一线") == 0)
			nLine = 1;
		else if (strcmp(m_szServer, "二线") == 0)
			nLine = 2;
		else if (strcmp(m_szServer, "三线") == 0)
			nLine = 3;
		else if (strcmp(m_szServer, "四线") == 0)
			nLine = 4;
		else if (strcmp(m_szServer, "五线") == 0)
			nLine = 5;
		else if (strcmp(m_szServer, "六线") == 0)
			nLine = 6;
	}
	
	return nLine;
}