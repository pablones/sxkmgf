// WorldKernel.cpp: implementation of the CWorldKernel class.
//
//////////////////////////////////////////////////////////////////////
#define	_WINSOCKAPI_		// ��ֹ����winsock.h
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
const int WORLDDAY	=	1000*60*60*6;	//���0327 ÿ����Ϸ��Ϊ6Сʱ����ʱ
const int WAITQUEUETIME = 15;			//  [2/12/2007 Halei]	�Ŷ�ʱÿ�˵ȴ�ʱ��
const int NOWAITPLAYER  = 1000;			//  [2/12/2007 Halei]	�����Ŷӵ�����
//////////////////////////////////////////////////////////////////////
CWorldKernel*	CWorldKernel::m_pWorld = NULL;


char g_szKernelState[1024 * 10] = {0};
char g_szShellState[1024 * 10] = {0};

//modify code by �ֵ�Ԩ begin 2011.5.19
#include ".\\..\\MAPGROUPKERNEL\\statistical.h"
//modify code by �ֵ�Ԩ end
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

	// ��ʼ�����ݿ�
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
		LOGERROR("���ݿ�����ʧ��!");
		return false;
	}

	//  [1/15/2008 ���]
	::GetLocalTime(&m_stLastDatabaseReconnectTime);

	// TODO: ���ڴ���ӳ�ʼ������
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

	///����ͻ��������˿ں�
	ini.SetSection("System");
	m_nServerPort=ini.GetInt("SERVER_LISTEN_PORT");
	if(!m_nServerPort)
		m_nServerPort=SERVER_LISTEN_PORT_DEFAULT;

	if(ACCOUNT_PORT == 0)
	{
		LOGERROR("INI���ô���!");
		return false;
	}
	if(m_sock.Open(ACCOUNT_IP, ACCOUNT_PORT, 0, SOCKET_ACCOUNTBUFSIZE, SOCKET_ACCOUNTBUFSIZE))
		m_nState = STATE_CONNECTOK;
/*---------------------06.8.7������ӣ�����״̬������------------------------------------*/
//�˴���ini�ж�ȡIP���˿�
	ini.SetSection( "StateServer" );
	NAMESTR		STATE_IP;
	int			STATE_PORT;
	
	ini.GetString( STATE_IP,"STATE_IP",_MAX_NAMESIZE );
	STATE_PORT = ini.GetInt( "STATE_PORT" );
	if( 0 == STATE_PORT )
	{
		LOGERROR("INI���ô���! 0 == STATE_PORT");
		return false;
	}	
	if( m_sockForStateServer.Open( STATE_IP,STATE_PORT,0,SOCKET_ACCOUNTBUFSIZE,SOCKET_ACCOUNTBUFSIZE ) )
		m_nState2 = STATE_CONNECTOK;
	memset( &m_SendToStateInfo,0,sizeof( m_SendToStateInfo ) );
	m_SendToStateInfo.unMsgType = 4004;
	strcpy( m_SendToStateInfo.szServerName,m_szServer );


/*----------------------���п��޵Ĺ�ϵ������-----------------------------------------*/
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
	
/*----------------------���п��޵Ĺ�ϵ������-----------------------------------------*/


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
//			if( pMsg->idPacket == _MSG_VAS )  07.01.08�����ֵϵͳ
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
			// TODO: ���ڴ������������ı��Ĵ������
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
	// TODO: ���ڴ���������ڲ���Ϣ�������
	case	KERNEL_CLOSEKERNEL:
		{
			SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
//			GameWorld()->ConnectShutDown( idSocket );
//			GameWorld()->WaitQueueSocketShutDown( idSocket );		//  [2/12/2007 Halei] ������쳣�˳�,���ŶӶ�����ɾ��
			LOGDEBUG("DEBUG����������յ��رպ�����Ϣ��SOCKET_ID[%u]", idSocket);
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
				int idProcess = pPlayer->idProcess;//������ڵ�ͼ��
				m_pMsgPort->Send(idProcess, MAPGROUP_USERCLOSEONTIMER, VARTYPE_INT, &idSocket);
			}
		}
		break;
		//----------------------------------
	case	WORLD_CLOSESOCKET:		// ��MAPGROUP������
		{
#ifdef	NEW_LOGOUT
			ASSERT(!"WORLD_CLOSESOCKET");
#else
			SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
			LOGDEBUG("DEBUG����������յ��ر�������Ϣ��SOCKET_ID[%u]", idSocket);
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
//				// ����Ƶ���ж�
//				typedef struct {
//					DWORD	dwWordsColor;
//					USHORT	unTxtAttribute;
//					// ... ��������û��Ҫд��
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
			else if(idMsg == _MSG_TALK && idSocket != SOCKET_NONE && m_sockForRelationServer.IsOpen()==false)//����ϵ������������ʱ�ߴ�ͳ����
			{
				// ˽��
				CPlayer* pSender = UserList()->GetPlayerBySocket(idSocket);
				if(pSender && strcmp(pMsg->szName,ALLUSERS_NAME))///�㲥��talk��Ϣ�������޴���
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
				if(msg.m_pInfo->unTxtAttribute==_TXTATR_MSG_SERVERLIST)///�����������ʱ��Ҫ���µķ������б���Ϣ
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
			///���ϵ����������
// 			CMsgTalk	msg;
// 			msg.Create(pBuf,nMsgLen);
// 			msg.unMsgSize	= sizeof(msg);
// 			msg.unMsgType	= _MSG_RELAY;

			///�������ҵ���Ҳ����ϵ�����������Ҳ���
			if(IsRelationServerOpen() && pPlayer==NULL)
				SendRelationRelayMsg(idMsg,pBuf,nMsgLen);

		}
		break;
	case WORLD_RELATIONMSG:
		{
			USERIDMSG_PACKET0*	pMsg = (USERIDMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;///�����idSocket���������ͷ���ͼ���id
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			///�Է������ߣ���ͷ�۰ɣ�û��ϵ�й�ϵ������������
			if(IsRelationServerOpen())
				SendRelationRelayMsg(idMsg,pBuf,nMsgLen,pMsg->idUser);

			///��Ϊ��ϵ�������㲥��϶,��Ҫ��һ�ݸ�����ͼ��
			TransmitBroadcastMapGroupMsg(idMsg,(const char*)pBuf,nMsgLen,pMsg->idUser,idSocket);

// 			CNetMsg* msgptr=CNetMsg::CreateClientMsg(SOCKET_NONE,SOCKET_NONE,idMsg,(const char*)pBuf,nMsgLen,pMsg->idUser);
// 			if(msgptr)
// 			{
// 				///�㲥����Դ��ͼ��֮������е�ͼ��
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
					if(msg.m_pInfo->unTxtAttribute==_TXTATR_TEAM)///����Ƶ����Ϣ��������ϵ������
					{
						break;
					}
				}
				if(idMsg==_MSG_BUFF)
					break;
				///�Է������ߣ���ͷ�۰ɣ�û��ϵ�й�ϵ������������
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
			if(pPlayer && g_UserList.IsLoginMapGroup(pPlayer->idSocket))//�������
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
			//20070406����:��������ΪidUser��nLevel
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
		//  [2007-9-10] ���
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

	return true;		// return false : ��Ϣ�����쳣������رա�
}
//modify code by �ֵ�Ԩ begin 2011.5.19
int g_statnum = 1;
//modify code by �ֵ�Ԩ end
//////////////////////////////////////////////////////////////////////
//20070709����:��ǿcatch�ֿ�.����Ϊĳ��catchӰ����������
bool CWorldKernel::OnTimer(time_t tCurr)
{
	// TODO: ���ڴ����ʱ�Ӵ������
	DEBUG_TRY
	g_UserList.OnTimer(tCurr);
	DEBUG_CATCH("CWorldKernel::OnTimer::g_UserList.OnTimer(tCurr)")
//modify code by �ֵ�Ԩ begin 2011.5.19
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
//modify code by �ֵ�Ԩ end
// 	[2009.3.2--�Ŷ�]ϵͳ�㲥��Ϣ
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
// 		//[2009.2.17--�Ŷ�]�����ݽ��в���
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
// 				::LogSave("CWorldKernel::OnTimer ExecuteSQL Fail,sql:%s",strSql);///���ʧ��д��־
// 	
// 			sprintf(strSql,sqlUpdateLoveChange,5,5);
// 			if(!m_pDb->ExecuteSQL(strSql))
// 				::LogSave("CWorldKernel::OnTimer ExecuteSQL Fail,sql:%s",strSql);///���ʧ��д��־
// 	
// 			m_bUpdate = false;
// 		}
// 		DEBUG_CATCH("CWorldKernel::OnTimer::SQL")
// 	
// 		//[2009.7.14--�Ŷ�]
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
//������ӣ�������ѡ���ɫ���棬�ϴ�������Ϣ���ʺŷ�����
	DEBUG_TRY
	CConnectSet::Iter it;
	for( it = m_setConnectClient->Begin();it != m_setConnectClient->End(); )
	{
		CConnectUser* pConnectUser = m_setConnectClient->GetObjByIter( it );
		it ++;

		if( pConnectUser != NULL )
		{
			OBJID idAccount = ID_NONE;
			if( pConnectUser->CheckHeartBeat( idAccount ) )//��Ҫ
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
	DEBUG_CATCH("CWorldKernel::OnTimer::m_setConnectClient����")
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
/*---------------06.8.7������Ӵ�������STATE SERVER--------------------------*/

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
// 	const DWORD SEND_USERAMOUNT_TIMER = 5 * 1000;	//60��,һ���ӷ�һ��......
// 	if( !m_sockForStateServer.IsOpen() )
// 	{
// 		m_nState2 = STATE_NONE;
// 		//20070215����:bug
// 		//�˾������д�뵽sk_taskDetail��ĳ��.ԭ����.
// 		//���ټ�¼����Ϣ.����ʱ��Ȼ��ok����
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
// 			m_sockForStateServer.Send2StateServer( ( LPCTSTR )&m_SendToStateInfo,sizeof( m_SendToStateInfo),true );//������Ϣ
// 
// 			m_dwTimeForStateServer = clock();//����ʱ��
// 		}
// 		
// 	}
// 	DEBUG_CATCH("CWorldKernel::OnTimer::����״̬������")
/*-----------------��������STATE SERVER end -----------------------------------*/

	///��ϵ������������������ ������Ҫ���ǲ���̫������  ������Ϊ10S
	DEBUG_TRY
	static DWORD dwLastConnectReserver = 0;
	if(m_RServerPort)///ֻ�й�ϵ��������Ч������²���������
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
	DEBUG_CATCH("CWorldKernel::OnTimer::���ӹ�ϵ������")

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
 	char Temp = 'w';//��Ϊsendʱ���size Ϊ0,��ᶪ���˰���������ʱ��һ�����õ��ַ�..
 
// 	if(::GetTickCount()-m_SyndicateTime>= 1000*60 )//ÿ���ӵ�һ��ͼ�������Լս���
// 	{
// 		m_SyndicateTime	 =	::GetTickCount();
// 		m_pMsgPort->Send(MSGPORT_MAPGROUP_FIRST,MAPGROUP_NEW_SYN_TIME,STRUCT_TYPE( Temp ),&Temp);
// 		//m_pShopManager->Updata();//[��;���� 2009.02.17]���µ�����Ϣ
// 	}

	if(::GetTickCount()-m_WorldTime>= WORLDDAY )//���0321 �µ�һ��
	{
		m_WorldTime	 =	::GetTickCount();
		m_pMsgPort->Send(MSGPORT_MAPGROUP_FIRST,MAPGROUP_NEWDAY,STRUCT_TYPE( Temp ),&Temp);
	}
	DEBUG_CATCH("CWorldKernel::OnTimer::�µ�һ��")

//	g_objChatRoomManager.OnTimer(tCurr);
	//  [2/28/2007 HALEI] ��ҵ�½�Ŷ�ϵͳ,���ӹ��ܵ���������Ŷӷ���Ŀǰ��������������Ҫ�Ŷ������ж�
	//���ȵõ���Ҫ�Ŷӵ�����(ʱ��),��MustWaitTime����
	DEBUG_TRY
	int MustWaitTime  = GameWorld()->QueryUserList()->GetUserAmount()+1/*+m_setConnectClient->GetAmount()-WaitQueue.size()*/-NOWAITPLAYER;
	
	if ( MustWaitTime <= 0 )		//�ж��Ƿ���Ҫ�Ŷ�.
	{	//�������Ҫ�Ŷ�
		int infosize = WaitQueue.size();//�õ��ŶӶ��еĴ�С,���ж��������Ŷ�
		if (infosize != 0)			//������滹�����
		{
			//�������ö����������Ƚ���.
			ConnectInfo_WaitQueue info;
			info = WaitQueue.front();
			WaitQueue.erase( WaitQueue.begin() );
			CMsgWaitQueue msg;
			if ( msg.Create( info.idAccount,0,NETWORK_WAITSUSSECS ) )//������Ϣ�����������Խ�����.
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
				//������ҿ���
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
		//�����Ŷ���Ϣ����
		int infosize = WaitQueue.size();
		if(infosize!=0 && m_WaitQueueTimer.IsTimeOut())
		{
			//��ÿλ��ҷ����Ŷӵȴ���Ϣ
			CMsgWaitQueue waitmsg;
			for (int i = 0;i<infosize;++i)
			{
				
				waitmsg.Create(WaitQueue[i].idAccount,NETWORK_WAITINFO,i+1,infosize);
				GameWorld()->SendClientMsg(WaitQueue[i].idSocket,&waitmsg);
			}

			m_WaitQueueTimer.Update();
		}
	}
	DEBUG_CATCH("CWorldKernel::OnTimer::�Ŷ�")
	

	DEBUG_TRY
	ReconnectDB();
	DEBUG_CATCH("CWorldKernel::OnTimer::ReconnectDB");

	return true;		// return false : ��Ϣ�����쳣������رա�
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::Release()
{
	// TODO: ���ڴ���Ӵ���
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

	::LogSave("CWorldKernel::Release m_sock.Close() �ʺŷ��������ӱ��ر�,�����رյ�?");
	m_sock.Close();

	m_sockMonitor.Close();

	//���ͷ�����������Ϣ��StateServer
	m_SendToStateInfo.nOnlineUserAmount = -1;
	m_sockForStateServer.Send2StateServer( ( LPCTSTR )&m_SendToStateInfo,sizeof( m_SendToStateInfo),true );//������Ϣ
	m_sockForStateServer.Close();

	m_sockForRelationServer.Close();
	SAFE_RELEASE (m_setConnectClient);

	SAFE_RELEASE (m_pDb);
	
	delete this;
	return true;		// return false : �����塣
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ProcessClientMsg(SOCKET_ID idSocket, OBJID idMsg, const char *pbufMsg, int nSize, int nTrans, ULONG nIP)
{
	// TODO: ���ڴ���ӿͻ����ϴ���Ϣ�Ĵ������
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
		//-------���---2007/05/23-----
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
	// TODO: ���ڴ����NPC�������´���Ϣ�Ĵ���
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
// 	// TODO: ���ڴ����NPC�������ϴ���Ϣ�Ĵ������
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
// 		//-------���---2007/05/23-----
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
// 	// TODO: ���ڴ����NPC�������´���Ϣ�Ĵ���
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
bool CWorldKernel::CloseSocket(SOCKET_ID idSocket)			// ֱ�ӹر�socket
{
	// TODO: ���ڴ���ӹرտͻ���SOCKET�Ĵ���
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
 			SendFee(player->m_idAccount, CMsgFee_A::FEE_HOLDASS);///����
	}
	DEBUG_CATCH("CWorldKernel::SendAccountAllUser")
}

void CWorldKernel::SendRelationAllUser()///��������ӵ�е����������Ϣ���ǵ���ϵ��������
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
////////////////////////07.01.05�����ֵ///////////////////////////////////////////////////////////////
//bool CWorldKernel::SendVASRequest( int id, int itemAmount, OBJID itemID, int usAction, OBJID AccountID, OBJID PlayerID, int usVASPoint, int unMsgType, int unMsgSize )
//  [2/16/2008 ���]	��������
//bool CWorldKernel::SendVASRequest( int id, int itemAmount, OBJID itemID, DWORD dwData, int usAction, OBJID AccountID, OBJID PlayerID, int usVASPoint, int unMsgType, int unMsgSize ,int nJuan,int nRealID)
//[2010-07-26 goto]��Ϣ�ṹ�޸�
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

	//[��;���� 2009.01.08]
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

///�㲥ת������Ϣ
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

///ת����Ϣ����ϵ��������
void CWorldKernel::SendRelationRelayMsg(OBJID idMsg,const char* pack, int nLen,int nExtra)
{
	///������Ϣ���Ϲ�ϵ����������
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

///������Ϣ����ϵ��������
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
			LogSave("�յ�һ�������,������ֱ���˳�,size:%d",nMsgSize);
			m_sockForRelationServer.ClearPacket(nMsgSize);
			break;
		}
		///�յ���Ϣ�����Ͻ����������ݷ����ջ�д���
		memcpy(buffer,pMsgsock,nMsgSize);
		m_sockForRelationServer.ClearPacket(nMsgSize);
		bGetData=false;
		LPCTSTR pMsg =buffer;

		// ��������¼ ////////////////////////////////////////
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
				PrintText("Relation server login OK��");
			}
// 			m_sockForRelationServer.ClearPacket(nMsgSize);
			continue;
		}
		DEBUG_CATCH("ProcessRelationNetMsg::pMsgLogin")

		DEBUG_TRY
		if (pMsgLogin->unMsgType == _MSG_MAINSERVER)//����������Ϣ
		{
			CMsgMainServer::MSG_Info* msgptr = (CMsgMainServer::MSG_Info*)pMsgLogin;
			//CHECK(pMsgLogin->unMsgSize == sizeof(CMsgMainServer::MSG_Info));
			
			CMsgMainServer::MSG_Info msg;
			msg.unMsgSize	= msgptr->unMsgSize;
			msg.unMsgType	= msgptr->unMsgType;
			msg.dwMainServer= msgptr->dwMainServer;
			msg.dwData		= msgptr->dwData;
			//goto ֪ͨ��ͼ��ȷ������Ϸ������
			BroadcastMainServerMsg(_MSG_MAINSERVER, (LPCTSTR)&msg, msg.unMsgSize, msg.dwData*10+msg.dwMainServer, MSGPORT_MAPGROUP_FIRST);
			if(msg.dwMainServer==1)
				PrintText("Be appointed to the main server��");
			else if (msg.dwMainServer==0)
				PrintText("Be appointed to an auxiliary server��");
// 			m_sockForRelationServer.ClearPacket(nMsgSize);
			continue;
		}
		DEBUG_CATCH("ProcessRelationNetMsg::CMsgMainServer")

		///ȥ����Ϣ��С�������ֶ�
		int nRealMsgSize=nMsgSize-8;///ȥ���˸������ݳ���
		switch (pMsgLogin->unMsgType)
		{
			case _MSG_FEE_A:
			{
				DEBUG_TRY
				CMsgFee_A::MSG_Info* msgptr = (CMsgFee_A::MSG_Info*)pMsgLogin;
				if(msgptr->ucType==CMsgFee_A::FEE_KICKOUT)
				{
					LOGMSG("DEBUG:���˳���ͨ�������ظ���¼,��ϵָʾ��Ϸ�������߳��û�id[%u]", msgptr->idAccount);
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

				if(pMsgTalk->unTxtAttribute==_TXTATR_MSG_SERVERLIST)///����Ƿ������б������Ϣ��ֱ�ӻ������ڴ��ڲ��㲥
					StrPacker.GetString(3, m_szMsgServerList, sizeof(m_szMsgServerList));					
				else if(strcmp(szReceiver,ALLUSERS_NAME)==0)
				{
					///��ԭ��Ϣ
	 				msgTalk.CreateClientMsg2(-1,-1,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize,0);
					switch(pMsgTalk->unTxtAttribute)
					{
						case _TXTATR_SYNDICATE:///����Ƶ�����⴦��
							///Ϊ�˷�ֹ��Ϣ��ѭ��,���������ϢƵ�����д۸�
//							msgTalk.m_pInfo->unTxtAttribute=_TXTATR_SYNDICATE_OTHERLINE;
							msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
							GameWorld()->TransmitBroadcastMapGroupMsg(&msgTalk);
							break;
						case _TXTATR_PRO://����Ƶ�����⴦��
							msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
							GameWorld()->TransmitBroadcastMapGroupMsg(&msgTalk);
							break;

						case _TXTATR_FORGMTOOL://GM���ߴ���
							msgTalk.SetTransData(msgTalk.m_pInfo->dwUseGMToolPsw);
							GameWorld()->TransmitBroadcastMapGroupMsg(&msgTalk);
							break;

						default:
							UserList()->BroadcastMsg(&msgTalk);
							break;
					}
				}
				else if(pMsgTalk->unTxtAttribute==_TXTATR_CHATWITH)///����Ǻ���������Ϣ
				{
					///�����ҽ�����,�ҵ���ֱ�ӷ���ȥ
					CPlayer* pPlayer = UserList()->GetPlayer(szReceiver);
					if(pPlayer)
					{
						if(msgTalk.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket,pMsgLogin->unMsgType,(const char*)pMsgTalk, nRealMsgSize, 0))
							pPlayer->SendMsg(&msgTalk);
					}
					else
					{
						///�Ҳ���������ʱ������Ѱ�ҷ����ߣ����������Ե�����
						pPlayer = UserList()->GetPlayer(szSender);
						if(pPlayer)
						{
							pMsgTalk->unTxtAttribute=_TXTATR_LEAVEWORD;///�޸�Ϊ��������
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
						if (strncmp("!����", szBuf, 5) == 0 || strncmp("!kickout", szBuf, 8) == 0 || strncmp("!���", szBuf, 5) == 0)
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
// 						if (strncmp("!����", szBuf, 5) == 0 || strncmp("!kickout", szBuf, 8) == 0 || strncmp("!���", szBuf, 5))
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
					memcpy(&idUser,pMsg+nMsgSize-4,4);///��ȡextra����,���汣�����Դ�û���id
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
					memcpy(&idUser,pMsg+nMsgSize-4,4);///��ȡextra����,���汣�����Դ�û���id
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
									///�۸���Ϣ��֧�ֿ��
									msgItem.m_pInfo->usAction=ITEMACT_QUERYITEMINFO;
									GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgItem);
								}
								break;
								case ITEMACT_GETEUDEMONINFO:
								{
									///�۸���Ϣ��֧�ֿ��
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
// 									///�۸���Ϣ��֧�ֿ��
// 									pMsg->m_pInfo->usAction=ITEMACT_QUERYITEMINFO;
// 									GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,pMsg);
// 								}
// 								break;
// 								case ITEMACT_GETEUDEMONINFO:
// 								{
// 									///�۸���Ϣ��֧�ֿ��
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
								///�۸���Ϣ��֧�ֿ��
								msgItem.m_pInfo->usAction=EM_MSGWPSL_QUERY_LINE;
								GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgItem);
							}
							break;
						case EM_MSGWPSL_UPDATE:
							{
								///�۸���Ϣ��֧�ֿ��
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
					memcpy(&idUser,pMsg+nMsgSize-4,4);///��ȡextra����,���汣�����Դ�û���id
					CPlayer* pPlayer = UserList()->GetPlayer(idUser);
					switch(msgptr->ucAction)///��ʹ�࣬�����Ϣ�������鷳���Ժ���Ʒ������ܹ�һ��Ҫ���ߵ�
					{
						case _FRIEND_ADDSUCCESSREPLY:
							{
								msgptr->ucAction=_FRIEND_ADDSUCCESS;
								if(pPlayer)///����ָ����Ч˵������һ���ش���Ϣ
									SendMsg(pPlayer->idSocket, pMsgLogin->unMsgType,(const char*)msgptr, nRealMsgSize);
							}
							break;
						case _FRIEND_ADDFRIEND:
							{
								pPlayer = UserList()->GetPlayer(msgptr->idFriend);///�õ���֪ͨ�߶���
								if(pPlayer)
								{
									msgptr->idFriend=msgptr->dwLookFace;
									SendMsg(pPlayer->idSocket, pMsgLogin->unMsgType,(const char*)msgptr, nRealMsgSize);
								}
							}
							break;
						case _FRIEND_ADDSUCCESS_PASSIVE:
						case _FRIEND_ADDSUCCESS:///����ж���Ϣ
							{
								///��ӱ�����Ӻ��ѳɹ���������һ��
								pPlayer = UserList()->GetPlayer(msgptr->idFriend);///�õ���֪ͨ�߶���
								if(pPlayer)
								{
//									msgptr->idFriend=idUser;
									if(msgFriend.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket, _MSG_FRIEND,(const char*)msgptr, nRealMsgSize, idUser))
										GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgFriend);

									///����Դ������Ŀ��ID�Ѽ��ݺ���Ĵ���
//									msgptr->idFriend=idUser;
// 									CMsgFriend* pMsg=(CMsgFriend*)CNetMsg::CreateClientMsg(pPlayer->idProcess,pPlayer->idSocket, _MSG_FRIEND,(const char*)msgptr, nRealMsgSize, idUser);
// 									if(pMsg)
// 									{
// 										///�Լ�����ǰд�Ĵ��룬��̾��
// 										GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,pMsg);
// 										SAFE_DELETE(pMsg);
// 									}
								}
							}
							break;
						case _FRIEND_QUERYFRIENDINFO:
						case _FRIEND_DELFRIEND:
							{
								///���˰���ɾ��������֪ͨ��
								pPlayer = UserList()->GetPlayer(msgptr->idFriend);///�õ���֪ͨ�߶���
								if(pPlayer)
								{
									if(msgFriend.CreateClientMsg2(pPlayer->idProcess,pPlayer->idSocket, _MSG_FRIEND,(const char*)msgptr, nRealMsgSize, idUser))
										GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,&msgFriend);

// 									CMsgFriend* pMsg=(CMsgFriend*)CNetMsg::CreateClientMsg(pPlayer->idProcess,pPlayer->idSocket, _MSG_FRIEND,(const char*)msgptr, nRealMsgSize, idUser);
// 									if(pMsg)
// 									{
// 										///�Լ�����ǰд�Ĵ��룬��̾��
// 										GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,pMsg);
// 										SAFE_DELETE(pMsg);
// 									}
								}
							}
							break;
						case _FRIEND_GETFRIENDINFO:
						case _FRIEND_OFFLINE:
						case _FRIEND_ONLINE:
						case _FRIEND_GETINFO:///�������
							if(pPlayer)
							{
								msgptr->ucSex /= 10;//goto�����һλ�ɵ�
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
		DEBUG_CATCH("CWorldKernel::ProcessRelationNetMsg ��ѭ������������")
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//20070709����:catch�ֿ�.���ⵥ��catchӰ��������������
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
			LogSave("�յ�һ�������,������ֱ���˳�,size:%d",nMsgSize);
			m_sockForRelationServer.ClearPacket(nMsgSize);
			break;
		}
		///�յ���Ϣ�����Ͻ����������ݷ����ջ�д���
		memcpy(buffer,pMsgsock,nMsgSize);
		m_sock.ClearPacket(nMsgSize);
		bGetData=false;
		LPCTSTR pMsg =buffer;

		// ��������¼ ////////////////////////////////////////
		DEBUG_TRY
		CMsgLogin_A::MSG_Info*	pMsgLogin = (CMsgLogin_A::MSG_Info*)pMsg;
		if(pMsgLogin->unMsgType == _MSG_LOGIN_A)
		{
			CHECK(pMsgLogin->unMsgSize == sizeof(CMsgLogin_A::MSG_Info));
			ProcessLogin(pMsgLogin->idAccount, pMsgLogin->dwCheckData, pMsgLogin->usVersion, pMsgLogin->szUserName);
			continue;
		}
		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg::pMsgLogin")

		// ��Ҳ��� ////////////////////////////////////////
		DEBUG_TRY
		CMsgConnect_A::MSG_Info*	pMsgConnect = (CMsgConnect_A::MSG_Info*)pMsg;
		if(pMsgConnect->unMsgType == _MSG_CONNECT)
		{
//			extern struct STAT_STRUCT	g_stat;
//			InterlockedIncrement( &g_stat.nRecvAccountPlayers );//07.5.21������ӣ����ڲ����ʺŷ���������
			
			CHECK(pMsgConnect->unMsgSize == sizeof(CMsgConnect_A::MSG_Info));
		//  [7/2/2007]	����ע�Ͳ��޸�----------begin-----------------------------------------------------------
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
	//----------------07.5.25����ע������һ��,���ʺŷ������������ļƷ���Ϣ------------------
	//		ProcessQueryFee(pMsgQFee->idAccount, pMsgQFee->dwData, pMsgQFee->nTime, pMsgQFee->ucType);
			continue;
		}
	//-----------07.01.06���----�����ֵ���񷵻���Ϣ------
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
//modify code by �ֵ�Ԩ begin 2011.5.30
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
//modify code by �ֵ�Ԩ end

		bGetData = false;

		DEBUG_CATCH("CWorldKernel::ProcessAccountNetMsg ��ѭ������������")
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

	//??? δ���汾��

	m_idServer	= idAccount;
	PrintText("Account server login OK��");
	SendLogin(idAccount, dwCheckData, usVersion, szUserName);
	m_nState = STATE_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::ClientConnect(OBJID idAccount, DWORD dwData, LPCTSTR szInfo, SOCKET_ID idSocket,int state,int nYoukeLogin/* = 0*/,ULONG nIP/*=0*/)			// return true: pass barrier
{
	CHECKF( !( idSocket == SOCKET_NONE && state !=  LOGINOK ) );//�����ʺŷ���������ʱ��idSocketΪ-1

	CConnectUser* pConnect = m_setConnectClient->GetObj(idAccount);

	//pConnectû��   1.�˺ŷ����� �ͻ��� ��һ�η�����   ����
	if(pConnect==NULL)
	{
		m_setConnectClient->DelObj(idAccount);	

		if( state != JOIN  )
		{
			//��ǰ�߳�  �ú�һ���˽�
// 			if(UserList()->GetPlayerByAccountID(idAccount))
// 			{
// 				UserList()->KickOutAccount(idAccount, STR_LOGIN_AGAIN);
// 			}

			pConnect = CConnectUser::CreateNew();
			CHECKF(pConnect->Create(idAccount, dwData, szInfo, idSocket,state,nYoukeLogin,nIP));
			m_setConnectClient->AddObj(pConnect);
		}
	}
	//��  ���Բ�ȫ  ����
	else if((pConnect->GetLoginState() ^ state ) == JOIN )	//����ͻ��˺���֤�������������˵�½��Ϣ
	{
		if(pConnect->CheckData(dwData))//ƥ��
		{
			ConnectInfo_ST info;
			info.idAccount	= pConnect->GetID();

			if( state == CLIENTOK )
				pConnect->SetSocketID( idSocket );	

			info.idSocket   = pConnect->GetSocketID();
			if (pConnect->GetULongIP()==0 && nIP!=0)//������˺ŷ�����Ϣ�ȵ������ֵ�п�����0
				pConnect->SetULongIP(nIP);
			info.nIP		= pConnect->GetULongIP();
			//��ȥ���Ŷ�
// 			BOOL NotWaitQueue = CheckWaitQueue(info.idAccount,info.idSocket);//  [2/12/2007 Halei]--����Ƿ���Ҫ�Ŷ�
// 			if( NotWaitQueue )//��������Ŷ�
// 			{

			pConnect->m_Youke |= nYoukeLogin;
			pConnect->SetLoginState( JOIN );//ֱ������ҵ�½
			if(!LoginUserSnap(info,pConnect->m_Youke))		//���Ϳ���
			{
				m_setConnectClient->DelObj(idAccount);

			}
// 			}
// 			else
// 			{
// 			}
		}
		else //��ƥ��  �൱��������
		{
			//��ǰ�߳�  �ú�һ���˽�
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
	else if(pConnect->GetLoginState() == JOIN && state == JOIN  /*( LOGINOK | CLIENTOK )*/ )	//�����ѡ���ɫ
	{
		ConnectInfo_ST info;
		info.idAccount = pConnect->GetID();
		info.idSocket	= pConnect->GetSocketID();
		if (pConnect->GetULongIP()==0 && nIP!=0)//������˺ŷ�����Ϣ�ȵ������ֵ�п�����0
			pConnect->SetULongIP(nIP);
		info.nIP		= pConnect->GetULongIP();
		SafeCopy(info.szInfo, szInfo, _MAX_NAMESIZE);  // client szInfo is invalid!
		LoginUserByInfo(info);
	
		m_setConnectClient->DelObj(idAccount);
	}
	else//�����ĵ�������
	{
		//��ǰ�߳�  �ú�һ���˽�
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
//		LOGMSG("DEBUG���ʺŷ������߳��ʺ�[%u]", idAccount);
		UserList()->KickOutAccount(idAccount, STR_LOGIN_ANOTHER);	// ������

// 		if(!UserList()->KickOutAccount(idAccount, STR_LOGIN_ANOTHER))		// ������
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
		//LOGMSG("ERROR���Ҳ�������Ҷ���ID��%d,��ǰusVASPointֵ��%d,��ǰusAction: %d", PlayerID, nVASPoint,MsgVAS.m_pInfo->usAction );
// 		for (int i=0; i<usGoodSize; i++)
// 			::LogSave("CWorldKernel::ProcessVAS�̳ǹ���ʱ����Ѳ����� user=%u,itemtype=%u,amount=%d", PlayerID, nGoods[i].itemID, nGoods[i].itemAmount);
		//��¼��ϸ��־
		if (usAction==_MSGVAS_BUY_SUC || usAction==_MSGVAS_POINT_BUY_SUC || usAction==_MSGVAS_BUY_ASALE_SUC)
		{
			for (int i=0; i<usGoodSize; i++)
				::LogSave("CWorldKernel::ProcessVAS�̳ǹ���,��Ҳ����� user=%u,itemtype=%u,amount=%d", PlayerID, nGoods[i].itemID, nGoods[i].itemAmount);
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
			::LogSave("CWorldKernel::ProcessVAS ���ͨ��Ԫ��Ʊ��ȡ��Ԫ������δ���ҵ�����Ҳ�ɾ��Ԫ��Ʊ����Ԫ���ۻ� user=%u,itemtype=%u,amount=%d", PlayerID, nGoods[0].itemID, nGoods[0].itemAmount);
		}
		if (usAction==_MSGVAS_SPEND_VAS_SUC)
		{
			if (nGoods[0].itemID==_SPCITEM_VASEXCHANGE)
				::LogSave("CWorldKernel::ProcessVAS ���Ԫ��Ʊʱ�Ҳ������(userid:%u,amount:%u)",PlayerID, nGoods[0].dwData);
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
		if(strcmp(pUser->szNotify, FLAG_ISP_TO_NORMAL) == 0)			// ��ע�⣺���ַ����������ʺŷ�����ͬ��
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
		LOGERROR("�ʺŷ�����������δ��¼��ҵĲ�ѯ�Ʒ���Ϣ");
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
// �㲥��ָ����ͼ�鴦��
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

// �㲥��ָ����ͼ�鴦��
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

// �㲥��ָ����ͼ�鴦��
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
// return true: ��¼�ɹ���������Ҫ��¼�����Ϣ
bool CWorldKernel::LoginUserByInfo(const ConnectInfo_ST& LoginInfo)
{	
	OBJID idAccount = LoginInfo.idAccount;
	// check repeat user_id
	if(UserList()->GetPlayerByAccountID(idAccount))
	{
		//LOGMSG("DEBUG���ʺ�[%u]�ظ���¼����ǰһ������ !", idAccount);
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
		if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, "�������㣬�뼰ʱ��ֵ!", NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
			SendClientMsg(LoginInfo.idSocket, &msg);
		return false;
	}
	/************************************************************************/
	/*[��;���� 2009.07.13]                                                 */
	else
	{
		CMsgTalk msg;
		if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME,
			           "������Ϸ��������ѯ����,������!",
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

bool CWorldKernel::LoginUserSnap(const ConnectInfo_ST &LoginInfo,int nYouke)//���0307 ��ҿ���
{
	bool bYouke = false;
// 	if(nYouke > 0)
// 		bYouke = true;

	UserList()->LoginUserSnap(LoginInfo.idAccount,LoginInfo.idSocket,bYouke);
	return true;
}


bool CWorldKernel::DeleteRole(OBJID idAccount, DWORD dwData,LPCTSTR password, LPCTSTR name)//���0327 �ڵ�½����ɾ������
{
	SQLBUF	szSQL;				
	sprintf(szSQL, "SELECT id, mate,syn_id FROM %s WHERE account_id='%u' and name = '%s' and protect_passwd = '%s'",_TBL_USER, idAccount,name,password);
	
	GameWorld()->GetDatabase();
	IRecordset*	pRes = GetDatabase()->CreateNewRecordset(szSQL);		
	if(pRes)
	{
			//������ϵͳ��ʱ���޸�
// 			char text[16];				
// 			pRes->LoadString(text, "mate", _MAX_NAMESIZE);			
// 			sprintf(szSQL, "UPDATE %s SET mate='%s' WHERE name='%s' LIMIT 1",
// 				_TBL_USER,
// 				NOMATE_NAME,
// 				text);
// 			GetDatabase()->ExecuteSQL(szSQL);
// 
 		OBJID	idUser	= pRes->LoadDWord("id");				//���������ϵ
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
// 			// delete all contact_history info�������20081217
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
//  [2/12/2007 HALEI]//��ҵ�½�ȴ�����Ƿ���Ҫ�Ŷ��㷨--------------------------------------------------
BOOL CWorldKernel::CheckWaitQueue( OBJID idAccount,SOCKET_ID usersock )//����Ƿ���Ҫ�Ŷӵ��㷨
{
	//���ȵõ���Ҫ�Ŷӵ�����(ʱ��),��MustWaitTime����
	int MustWaitTime  = GameWorld()->QueryUserList()->GetUserAmount()+1/*+m_setConnectClient->GetAmount()-WaitQueue.size()*/-NOWAITPLAYER;
	bool FindInQueue = false;		//��ʼ�����������ҵ�����Ϊû���ҵ�.
	if ( MustWaitTime <= 0 )		//�ж��Ƿ���Ҫ�Ŷ�.
	{	//�������Ҫ�Ŷ�
		int infosize = WaitQueue.size();//�õ��ŶӶ��еĴ�С,���ж��������Ŷ�
		if (infosize != 0)			//������滹�����
		{			
			for (int i =0;i<infosize;i++)
			{
				if (WaitQueue[i].idAccount == idAccount)	//�ȱ����Ƿ��������Һ�Ŀǰ��½�������ͬ.
				{
					FindInQueue = true;						//֤���ڶ������ҵ�����.
					CMsgWaitQueue msg;
					if (msg.Create(idAccount,0,NETWORK_WAITBREAK))	//�ж�����ǰ��½���Ŷ�	
						GameWorld()->SendClientMsg(WaitQueue[i].idSocket,&msg);					
					WaitQueue.erase( WaitQueue.begin()+i );//��������ǰ�Ŷӵ�����.					
					break;
				}				
			}
		}

		CMsgWaitQueue msg;
		if (msg.Create(idAccount,0,1))	//������Ϣ�����������Խ�����.
		{
			GameWorld()->SendClientMsg(usersock,&msg);
		}
		return true;
	}
	else
	{	//�����Ҫ�Ŷ�

		int infosize = WaitQueue.size();//���ȵõ����еĴ�С
		if (infosize != 0)//��������������
		{
			int i = 0;
// 			for ( i =0; i<infosize && i< 10; i++ )	//  ��������,����ȷ�Ŷӵ�ʱ����߶������ǰ10�����.
// 			{	//����������ĵȴ�ʱ��    ����   ��ʵ��Ҫ�Ŷӵ�ʱ��
// 				if( WaitQueue[i].EndWaitTime > ( GetTickCount()/1000+MustWaitTime*WAITQUEUETIME+infosize*10 ) )
// 				{
// 					WaitQueue[i].EndWaitTime = MustWaitTime*WAITQUEUETIME+infosize*10;
// 					CMsgWaitQueue waitqueuemsg;		//���͸��ͻ�����ȷ���Ŷ�ʱ��
// 					if (waitqueuemsg.Create( WaitQueue[i].idAccount,MustWaitTime*WAITQUEUETIME+infosize*10,NETWORK_LOGIN ) )
// 					{
// 						GameWorld()->SendClientMsg(WaitQueue[i].idSocket,&waitqueuemsg);
// 					}					
// 				}
// 			}
			for ( i =0;i<infosize;i++)
			{
				if (WaitQueue[i].idAccount == idAccount)	//�ȱ����Ƿ��������Һ�Ŀǰ��½�������ͬ.
				{
					int testsocket = WaitQueue[i].idSocket;
					FindInQueue = true;						//֤���ڶ������ҵ�����.
					CMsgWaitQueue msg;
					if (msg.Create(idAccount,0,NETWORK_WAITBREAK))	//�ж�����ǰ��½���Ŷ�	
						GameWorld()->SendClientMsg(WaitQueue[i].idSocket,&msg);
					WaitQueue.erase( WaitQueue.begin()+i );//��������ǰ�Ŷӵ�����.���������Ŷ�.
					ConnectInfo_WaitQueue info;
					info.EndWaitTime = (GetTickCount()/1000+MustWaitTime*WAITQUEUETIME+infosize*10);
					info.idAccount = idAccount;	
					info.idSocket = usersock;
					WaitQueue.push_back( info );		//��������ѹ�����
					
					CMsgWaitQueue waitqueuemsg;
					waitqueuemsg.Create( info.idAccount,MustWaitTime*WAITQUEUETIME+infosize*10,NETWORK_LOGIN );
					GameWorld()->SendClientMsg(usersock,&waitqueuemsg);	//���߿ͻ���.����Ҫ�Ŷ�
					return false;
				}
				
			}
			if (!FindInQueue)	//���û���ڶ������ҵ���.֤�������µ�½�Ŷ�.
			{
			
				ConnectInfo_WaitQueue info;
				info.EndWaitTime = (GetTickCount()/1000+MustWaitTime*WAITQUEUETIME+infosize*10);
				info.idAccount = idAccount;	
				info.idSocket = usersock;
				WaitQueue.push_back( info );		//ֱ��ѹ�����
				
				CMsgWaitQueue msg;//��������Ҫ�Ŷ�
				if ( msg.Create( info.idAccount,MustWaitTime*WAITQUEUETIME+infosize*10,NETWORK_LOGIN ) )
				{
						GameWorld()->SendClientMsg(usersock,&msg);
				}
				
			
				return false;
			}
		}
		else
		{
			
			ConnectInfo_WaitQueue info;		//���������û��.
			info.EndWaitTime = (GetTickCount()/1000+MustWaitTime*WAITQUEUETIME);
			info.idAccount = idAccount;	
			info.idSocket = usersock;
			WaitQueue.push_back( info );	//ֱ�Ӱ���ѹ��ȥ
			
			CMsgWaitQueue msg;
			msg.Create( info.idAccount,MustWaitTime*WAITQUEUETIME,NETWORK_LOGIN );
			GameWorld()->SendClientMsg(usersock,&msg);		//��������Ҫ�Ŷ�
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

//  [2/12/2007 Halei] ��������Ŷ�ʱ,�����ĵȴ��쳣�˳������
void CWorldKernel::WaitQueueSocketShutDown( SOCKET_ID usersock )
{
	int QueueSize = WaitQueue.size();
	for (int i =0; i< QueueSize; i++ )
	{
		if ( usersock == WaitQueue[i].idSocket )
		{
			WaitQueue.erase( WaitQueue.begin()+i );		//�Ӷ�����ɾ��
			break;
		}
	}
}
// ���� [5/26/2007]
// �ͻ��˹رգ�����δ������Ϸ���Ͳ�����userlist��
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
//	����
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
// ���� [6/7/2007]
// �ʺŷ�����Ҫ���߳����,����ʱ���Ҳ��δ������Ϸ,��ʱ��UserList���Ҳ���
// ��ConnectUser���ҵ�����ҵ�socket,�ر�
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
// ���� [6/07/2007]
// �ʺŷ�����Ҫ��رմ��ʺŵ�����,����ʱ���˲�δ��userlist��
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
//				//�ر���ͻ��˵�����
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
		BOOL bLastOne = (i == (nMsgCount - 1)) ? TRUE : FALSE;	//	�Ƿ�Ϊ���һ����
		int	 nLength = bLastOne ? (nTraceLength % MAX_STRING_SIZE_PER_MSG) : MAX_STRING_SIZE_PER_MSG;	//	�ַ�������
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
		// ��ʼ�����ݿ�
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
			LOGERROR("���ݿ���������ʧ��!, m_idProcess = %d", m_idProcess);
			return false;
		}
		
		LOGMSG("���ݿ����ӳɹ�, m_idProcess = %d", m_idProcess);

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
	
	//[��;���� 2009.01.08]
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
		//LOGMSG("ERROR���Ҳ�������Ҷ���ID��%d,typeֵ��%d", idUser, dwType);
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
		//LOGMSG("ERROR���Ҳ�������Ҷ���ID��%d", playerID);
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
		//LOGMSG("ERROR���Ҳ�������Ҷ���ID��%d,��ǰusFATIGUEֵ��%d", PlayerID, Fatigue );
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
//��������ɫ�е��û��㲥��Ϣ
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
//Ԫ������Ϣ
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
//modify code by �ֵ�Ԩ begin 2011.5.30
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
//modify code by �ֵ�Ԩ end

int CWorldKernel::GetLineIndex()
{
	int nLine = 0;
	if(m_szServer && strlen(m_szServer)>0)
	{
		if (strcmp(m_szServer, "һ��") == 0)
			nLine = 1;
		else if (strcmp(m_szServer, "����") == 0)
			nLine = 2;
		else if (strcmp(m_szServer, "����") == 0)
			nLine = 3;
		else if (strcmp(m_szServer, "����") == 0)
			nLine = 4;
		else if (strcmp(m_szServer, "����") == 0)
			nLine = 5;
		else if (strcmp(m_szServer, "����") == 0)
			nLine = 6;
	}
	
	return nLine;
}