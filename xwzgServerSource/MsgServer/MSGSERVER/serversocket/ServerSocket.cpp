// ServerSocket.cpp: implementation of the CSocketKernel class.
//
//////////////////////////////////////////////////////////////////////

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#define FD_SETSIZE      2048				// 限制最多2000个客户端登录
#include <winsock2.h>
#include <windows.h>
#pragma	warning(disable:4786)


#include "Session.h"
#include "IOCPServer.h"

#include "../share/define.h"
#include "protocol.h"
#include "EncryptServer.h"
#include "SSocket.h"
#include "I_Shell.h"

#include "I_MessagePort.h"
#include "basefunc.h"

#include "ServerSocket.h"

//modify code by 林德渊 begin 2011.5.19
#include ".\\..\\MAPGROUPKERNEL\\statistical.h"
//modify code by 林德渊 end
//#define		MAX_USERSIZE	(FD_SETSIZE - 48)

#define			MAX_CLIENTCONN				1000
#define			HANDLEIONUM					1
#define			MaxAcceptSession			1000
#define			SendBufferSize				256*1024
#define			RecvBufferSize				32*1024
#define			MAXPACKETSIZE				1024

//#define			MSGSTATLOG	//消息统计日志开关
//////////////////////////////////////////////////////////////////////
// interface
//////////////////////////////////////////////////////////////////////
bool CSocketKernel::Create(IMessagePort* pPort,int SERVER_LISTEN_PORT,int SERVER_LISTEN_NPC_PORT)
{
//	m_idNpcSocket = SOCKET_NONE;
	m_pMsgPort = pPort;
	m_pMsgPort->Open();
	// TODO: 请在此添加初始化代码

	//InitializeCriticalSection(&m_setSocketLock);
	//InitializeCriticalSection(&m_setProcessIDLock);
	//InitializeCriticalSection(&m_setNetBreakFlagLock);
	InitializeCriticalSection(&m_ReuseIDListLock);
//	InitializeCriticalSection(&m_setNpcProcessIDLock);

	m_setSocket.reserve(MaxAcceptSession+2);
	m_setProcessID.reserve(MaxAcceptSession+2);
	m_setNetBreakFlag.reserve(MaxAcceptSession+2);

	m_piocpServer = new IOCPServer;

//	m_piocpNPCServer = new IOCPServer;

	m_piocpServer->SetKernel(this);

//	m_piocpNPCServer->SetKernel(this);
	
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	IOHANDLER_DESC desc[HANDLEIONUM];
	int i;
	for(i=0;i<HANDLEIONUM;++i)
	{
		desc[i].dwIoHandlerKey					= i;
		desc[i].dwMaxAcceptSession				= MaxAcceptSession;
		desc[i].dwMaxConnectSession			= 0;
		desc[i].dwSendBufferSize				= SendBufferSize;
		desc[i].dwRecvBufferSize				= RecvBufferSize;
		//desc.dwTimeOut						= pServerEnv->ServerIoHandler.dwTimeOut;
		desc[i].dwNumberOfAcceptThreads		= /*si.dwNumberOfProcessors*/1;
		desc[i].dwNumberOfIoThreads			= 2*si.dwNumberOfProcessors/*+2*/;
		desc[i].dwNumberOfConnectThreads		= 0;
		desc[i].dwMaxPacketSize				= MAXPACKETSIZE;
		//desc.fnCreateAcceptedObject			= CreateServerSideAcceptedObject;
		//desc.fnDestroyAcceptedObject		= DestroyServerSideAcceptedObject;
		//desc.fnDestroyConnectedObject		= DestroyServerSideConnectedObject;
	}

	m_piocpServer->Init( desc, HANDLEIONUM );

	if(!(m_piocpServer->StartAllListen(NULL,SERVER_LISTEN_PORT)))
		return false;
	m_piocpServer->StartSendProcessThread();

// 	IOHANDLER_DESC npcdesc[HANDLEIONUM];
// 	
// 	for(i=0;i<HANDLEIONUM;++i)
// 	{
// 		npcdesc[i].dwIoHandlerKey					= i;
// 		npcdesc[i].dwMaxAcceptSession				= 2;
// 		npcdesc[i].dwMaxConnectSession			= 0;
// 		npcdesc[i].dwSendBufferSize				= 20*SendBufferSize;
// 		npcdesc[i].dwRecvBufferSize				= 20*RecvBufferSize;
// 		
// 		npcdesc[i].dwNumberOfAcceptThreads		= 1;
// 		npcdesc[i].dwNumberOfIoThreads			= si.dwNumberOfProcessors;
// 		npcdesc[i].dwNumberOfConnectThreads		= 0;
// 		npcdesc[i].dwMaxPacketSize				= MAXPACKETSIZE;
// 		
// 	}
// 	
// 	m_piocpNPCServer->Init(npcdesc,HANDLEIONUM);
// 	if(!(m_piocpNPCServer->StartAllListen(NULL,SERVER_LISTEN_NPC_PORT)))
// 		return false;
// 	m_piocpNPCServer->StartSendProcessThread();

	m_tNextClock	= clock() + CLOCKS_PER_SEC;
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSocketKernel::ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)
{
	// TODO: 请在此添加消息处理代码
	switch(idPacket)
	{
	case	SOCKET_CLOSESOCKET:
		{
			SOCKET_ID	idSocket	= *(int*)buf;
			if(idSocket >= m_setSocket.size())
				break;
			LOGDEBUG("DEBUG：通讯核心收到关闭网络消息，SOCKET_ID[%u]", idSocket);

			//EnterCriticalSection(&m_setSocketLock);
			if(m_setSocket[idSocket])			// readme.txt (1-7)
			{
				m_setSocket[idSocket]		= NULL;
				
				
				
				m_setProcessID[idSocket]	= PROCESS_NONE;
				
				
				m_setNetBreakFlag[idSocket]	= false;
				
// 				if(m_idNpcSocket == idSocket)
// 				{
// 					m_idNpcSocket = SOCKET_NONE;
// 				}

				//把idsocket加入到m_ReuseIDList中供以后使用
				EnterCriticalSection(&m_ReuseIDListLock);

				m_ReuseIDList.push_back(idSocket);

				LeaveCriticalSection(&m_ReuseIDListLock);
			}
			//LeaveCriticalSection(&m_setSocketLock);
		}
		break;
	case	SOCKET_SETPROCESSID:
		{
			const CHANGE_USERDATA*	pPack = (CHANGE_USERDATA*)buf;
			if((pPack->nData > MSGPORT_MAPGROUP_INSTANCE) || (pPack->nData < MSGPORT_WORLD))
			{
				LOGWARNING("SOCKET_SETPROCESSID set error data = %u",pPack->nData);
				return true;
			}

			//EnterCriticalSection(&m_setSocketLock);
			
			if(pPack->idSocket >= 0 && pPack->idSocket < m_setSocket.size() 
					&& m_setSocket[pPack->idSocket]
					&& !IsNetBreakFlag(pPack->idSocket) )		// 网络正常
			{
				//EnterCriticalSection(&m_setProcessIDLock);				
				m_setProcessID[pPack->idSocket] = pPack->nData;
				//LeaveCriticalSection(&m_setProcessIDLock);
			}
			else
			{
				::LogSave("发现切线或上线的时候网络中断,socketid = %u,chgto = %u,msgFrom = %u",pPack->idSocket,pPack->nData,nFrom);
				m_pMsgPort->Send(nFrom, KERNEL_CLOSEKERNEL, VARTYPE_INT, &pPack->idSocket);			// readme.txt (1-6)
			}
			
			//LeaveCriticalSection(&m_setSocketLock);
		}
		break;
	case	SOCKET_SENDCLIENTMSG:
		{
			int nStep = 0;
			unsigned int nMsgType = 0;
			SOCKET_ID idSocket=ID_NONE;
			char szIP[64]="";
			try{
			const SENDCLIENTMSG_PACKET0* pPack = (SENDCLIENTMSG_PACKET0*)buf;
			if(pPack->idSocket >= m_setSocket.size())
				break;
			idSocket=pPack->idSocket;
			nStep = 1;
			if(m_setSocket[pPack->idSocket])
			{
				strncpy(szIP,m_setSocket[pPack->idSocket]->GetIP(),sizeof(szIP));
				static char buf[MAX_PACKETSIZE];
				nStep = 2;
				int	nNetPacketSize = UnitePacket(buf, MAX_PACKETSIZE, pPack->idPacket, pPack->buf, pPack->nSize,nMsgType);
				ASSERT(nNetPacketSize);
				if(nNetPacketSize)
				{
					//m_iocpServer.PostPackage(m_setSocket[pPack->idSocket],buf,nNetPacketSize,IOCPS::SENDCLIENT);
					nStep = 3;
					m_setSocket[pPack->idSocket]->Send((BYTE*)buf,nNetPacketSize);
				}
			}
			}catch(...){LogSave("SOCKET_SENDCLIENTMSG step = %u MsgType = %u,idSocket=%d,szIP:%s",nStep,nMsgType,idSocket,szIP);}
		}
		break;
// 	case	SOCKET_SENDNPCMSG:
// 		{
// 			const SENDNPCMSG_PACKET0* pPack = (SENDNPCMSG_PACKET0*)buf;
// 			OBJID	idNpc = pPack->idNpc;
// 
// 			if(m_idNpcSocket != SOCKET_NONE && m_idNpcSocket<m_setSocket.size() && m_setSocket[m_idNpcSocket])
// 			{
// 				static char buf[MAX_PACKETSIZE];
// 				int	nLen = UniteNpcPacket(buf, MAX_PACKETSIZE, pPack->idPacket, pPack->buf, pPack->nSize, idNpc);
// 				ASSERT(nLen);
// 				if(nLen)
// 				{
// 					m_setSocket[m_idNpcSocket]->SendNPC((BYTE*)buf,nLen);
// 					//m_iocpServer.PostPackage(m_setSocket[m_idNpcSocket],buf,nLen,IOCPS::SENDNPC);	
// 				}
// 				break;		// break for
// 			}
// 		}
// 		break;
// 	case	SOCKET_SETNPCSOCKETID:
// 		{
// 			SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
// 			// repeat set npc socket id
// 			if(idSocket != m_idNpcSocket && m_idNpcSocket != SOCKET_NONE && m_idNpcSocket<m_setSocket.size() && m_setSocket[m_idNpcSocket])
// 			{
// 				//LOGDUMP("m_idNpcSocket(%d)->Close()", m_idNpcSocket);
// 				//m_setSocket[m_idNpcSocket]->Close();		// 断开SOCKET，其余事件由SOCKET中断引发。可能有问题???
// 				//m_iocpServer.DisconnectClient(m_setSocket[m_idNpcSocket]);
// 				m_setSocket[m_idNpcSocket]->Remove();
// 				LOGWARNING("NPC服务器重复登录，前一个SOCKET已断开!");
// 			}
// 			m_idNpcSocket = idSocket;
// 		}
// 		break;
// 	case	SOCKET_SETNPCPROCESSID:
// 		{
// 			const CHANGE_NPCPROCESSID*	pPack = (CHANGE_NPCPROCESSID*)buf;
// 			if(pPack->idProcess == PROCESS_NONE)
// 			{
// 				//EnterCriticalSection(&m_setNpcProcessIDLock);
// 				m_setNpcProcessID.erase(pPack->idNpc);
// 				//LeaveCriticalSection(&m_setNpcProcessIDLock);
// 			}
// 			else
// 			{
// 				//EnterCriticalSection(&m_setNpcProcessIDLock);
// 				m_setNpcProcessID[pPack->idNpc] = pPack->idProcess;
// 				//LeaveCriticalSection(&m_setNpcProcessIDLock);
// 			}
// 		}
// 		break;
// 	case	MAPGROUP_DELALLMONSTER:
// 		{
// 			m_setNpcProcessID.clear();
// 			LOGDUMP("Del all monster in socket thread.");
// 		}
// 		break;
	case	SOCKET_CHANGECODE:
		{
			const CHANGE_USERDATA*	pPack = (CHANGE_USERDATA*)buf;
			SOCKET_ID idSocket	= pPack->idSocket;
			DWORD	dwData		= pPack->nData;
			CHECKF(idSocket>=0 && idSocket < m_setSocket.size());
			
			//m_iocpServer.ChangeCode(m_setSocket[idSocket],dwData);
			//m_setSocket[idSocket]->ChangeCode(dwData);
		}
		break;
	case	SOCKET_BREAKCONNECT:
		{
			SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
 			CHECKF(idSocket>=0 && idSocket < m_setSocket.size());
			//LOGDEBUG("DEBUG：网络核心应核心要求关闭SOCKET，SOCKET_ID[%d]", idSocket);
			if(m_setSocket[idSocket] && !IsNetBreakFlag(idSocket))
			{
				
				//m_iocpServer.DisconnectClient(m_setSocket[idSocket]);

				//m_setSocket[idSocket]->Remove();

//				EnterCriticalSection(&m_setSocketLock);
//
//				m_setSocket[idSocket] = NULL;
//				
//				m_setProcessID[idSocket] = PROCESS_NONE;
//				
//				SetNetBreakFlag(idSocket);
//				
//				m_ReuseIDList.push_back(idSocket);
//
//				LeaveCriticalSection(&m_setSocketLock);

				m_setSocket[idSocket]->Remove();
			}
		}
		break;
	case	SOCKET_BREAKALLCONNECT:
		{
			SOCKET_ID	idGmSocket = *(SOCKET_ID*)buf;

			LOGDEBUG("DEBUG：网络核心应GM要求关闭所有SOCKET，SOCKET_ID[%d]", idGmSocket);
			extern long g_nServerClosed;
			g_nServerClosed = true;
			for(int i = m_setSocket.size()-1; i >= 0; i--)
			{

				if(i != idGmSocket && m_setSocket[i])
				{
					//LOGDUMP("m_setSocket[%d]->Close(true);", i);
					
					//m_iocpServer.DisconnectClient(m_setSocket[i]);
					m_setSocket[i]->Remove();
				}
			}
		}
		break;
	default:
		//LOGERROR("CSocketKernel::ProcessMsg()");
		return false;
	}

	return true;		// return false : 消息处理异常，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CSocketKernel::ProcessSocket()
{
	// m_piocpServer->ProcessSend();
	//m_piocpServer->Update();
	//m_piocpNPCServer->Update();
	//m_piocpServer->Update();
	return true;		// return false : 消息处理异常，程序关闭。
}


//还需进一步处理
int CSocketKernel::ProcessRecvMsg(const char* pRecvBuf,int nSize,Session* pSession)
{	
	//处理客户端消息
	if( pSession->GetClientID()>=0 && pSession->GetClientID()<m_setProcessID.size() && m_setProcessID[pSession->GetClientID()]!=PROCESS_NONE)
	{
		PROCESS_ID idProc = m_setProcessID[pSession->GetClientID()];
		DEBUG_TRY
		if(idProc != MSGPORT_WORLD && idProc != MSGPORT_MAPGROUP_FIRST && idProc != MSGPORT_MAPGROUP_INSTANCE)
		{
			LOGWARNING("ERROR idProc = %u",idProc);
			pSession->Remove();
			return 0;
		}
		DEBUG_CATCH("idProc != MSGPORT_WORLD && idProc != MSGPORT_MAPGROUP_FIRST && idProc != MSGPORT_MAPGROUP_INSTANCE");

		OBJID	idPacket;
		char*	pMsg;
		int		nMsgSize;
		int		nNetPacketSize = 0;
		int		nStep = 0;

		DEBUG_TRY
		if(( nNetPacketSize = SplitPacket((char*)pRecvBuf, nSize, &idPacket, &pMsg, &nMsgSize) ))
		{
			nStep = 1;
			static char	bufPacket[MAX_MESSAGESIZE];
			CLIENTMSG_PACKET0* pPacket = (CLIENTMSG_PACKET0*)bufPacket;
			pPacket->idSocket	= pSession->GetClientID();
			pPacket->idPacket	= idPacket;
			pPacket->nSize		= nMsgSize;
			nStep = 2;
			pPacket->nIP		= pSession->GetULongIP();
			memcpy(pPacket->buf, pMsg, nMsgSize);
			nStep = 3;
			m_pMsgPort->Send(idProc, KERNEL_CLIENTMSG, STRUCT_TYPE(bufPacket), bufPacket);		
//modify code by 林德渊 begin 2011.5.19
// 			if( IsNpcSocketID(pSession->GetClientID()) )
// 				g_StatisticalData.Addnpcrecvcount(pPacket->idSocket);
// 			else
// 			{
// 				if( g_StatisticalData.Addrecvcount(pPacket->idSocket) )
// 				{
// 					pSession->Remove();
// 					char szrecvstring[120] = {0};
// 					sprintf(szrecvstring,"KickOutSocket [%s] > maxrecvcount",pSession->GetIP());
// 					LogSave(szrecvstring);
// 				}
// 			}
//modify code by 林德渊 end
			nStep = 4;
			#ifdef MSGSTATLOG
			map<USHORT, ULONG>::iterator it = m_setMsgStat.find(idPacket);
			if(it!=m_setMsgStat.end())
				it->second++;
			else
				m_setMsgStat.insert(make_pair(idPacket, 1));
			#endif
		}
		DEBUG_CATCH3("ProcessRecvMsg1 nStep = %u,idSocket = %u,ProcessID = %u",nStep,pSession->GetClientID(),idProc)
		return nNetPacketSize;
	}
// 	else
// 	{			
// 		OBJID	idNpc;
// 		OBJID	idPacket;
// 		char*	pMsg;
// 		int		nMsgSize;
// 		int		nNetPacketSize = 0;
// 		int		nStep = 0;
// 		DEBUG_TRY
// 		if((nNetPacketSize=SplitNpcPacket(pRecvBuf,nSize, &idPacket, &pMsg, &nMsgSize, &idNpc)))
// 		{
// 			nStep = 1;
// 			PROCESS_ID	idProcess = MSGPORT_WORLD;
// 
// 			//添加临界区
// 			//EnterCriticalSection(&m_setNpcProcessIDLock);
// 
// 			NPCPROCESSID_MAP::iterator iter = m_setNpcProcessID.find(idNpc);
// 			if(iter != m_setNpcProcessID.end())
// 				idProcess = iter->second;
// 
// 			nStep = 2;
// 			//LeaveCriticalSection(&m_setNpcProcessIDLock);
// 
// 			static char	bufPacket[MAX_MESSAGESIZE];
// 			NPCMSG_PACKET0* pPacket = (NPCMSG_PACKET0*)bufPacket;
// 			pPacket->idNpc		= idNpc;
// 			pPacket->idPacket	= idPacket;
// 			pPacket->nSize		= nMsgSize;
// 			memcpy(pPacket->buf, pMsg, nMsgSize);
// 			nStep = 3;
// 			m_pMsgPort->Send(idProcess, KERNEL_NPCMSG, STRUCT_TYPE(bufPacket), bufPacket);
// 		}
// 		DEBUG_CATCH3("ProcessRecvMsg2 nStep = %u,idPacket = %u",nStep,idPacket)
// 		return nNetPacketSize;			
// 	}
}


void CSocketKernel::ProcessDisconnServer(int i)
{
	SendCloseNotify(i);
	//EnterCriticalSection(&m_setSocketLock);
	SetNetBreakFlag(i);
	//LeaveCriticalSection(&m_setSocketLock);
}


//////////////////////////////////////////////////////////////////////
bool CSocketKernel::OnTimer(time_t tCurr)
{
	// TODO: 请在此添加时钟处理代码

	#ifdef MSGSTATLOG
	// 时钟处理
	if(tCurr >= m_tNextClock)
	{
		m_tNextClock	+= 300000;//5分钟一次
		
		// 自动修正
		{
			int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
			if(abs(nOffset) > 30)
			{
				m_tNextClock = tCurr + 300000;//5分钟一次
				//LOGWARNING("CSocketThread::OnProcess() 时钟偏移%d秒，已自动修正。", nOffset);
			}
		}

		DEBUG_TRY
		char szbuf[1024] = "";
		char szTemp[64] = "";
		map<USHORT, ULONG>::iterator it = m_setMsgStat.begin();
		for(; it!=m_setMsgStat.end(); it++)
		{
			sprintf(szTemp, "msgtype:%d--%u\n", it->first, it->second);
			strncat(szbuf, szTemp, strlen(szTemp));
			if(strlen(szbuf)>900)
			{
				::MyLogSave("syslog\\msgstat", szbuf);
				memset(szbuf, 0L, 1024);
			}
		}
		if(strlen(szbuf)>0)
			::MyLogSave("syslog\\msgstat", szbuf);
		m_setMsgStat.clear();
		DEBUG_CATCH("CSocketKernel::OnTimer(time_t tCurr)")
	}
	#endif

	return true;		// return false : 消息处理异常，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CSocketKernel::Release()
{
	// TODO: 请在此添加代码
	
	for(int i = m_setSocket.size()-1; i >= 0; i--)
	{
		if(m_setSocket[i])
		{

			//m_iocpServer.DisconnectClient(m_setSocket[i]);
			m_setSocket[i]->Remove();
		}
	}

	
	m_pMsgPort->Close();

	//DeleteCriticalSection(&m_setSocketLock);
	//DeleteCriticalSection(&m_setProcessIDLock);
	//DeleteCriticalSection(&m_setNetBreakFlagLock);
	DeleteCriticalSection(&m_ReuseIDListLock);
//	DeleteCriticalSection(&m_setNpcProcessIDLock);
	delete m_piocpServer;

//	delete m_piocpNPCServer;

	delete this;
	return true;		// return false : 无意义。
}



int CSocketKernel::SetNewSocket(Session* sNew)
{
	int ret = -1;
	if(sNew != NULL)
	{
		
		EnterCriticalSection(&m_ReuseIDListLock);

		BOOL	bInsertOk = false;
		//判断m_ReuseIDList中有没有socketid

		if(!m_ReuseIDList.empty())
		{
			ret = m_ReuseIDList.front();
			m_ReuseIDList.erase(m_ReuseIDList.begin());

			m_setSocket[ret]			= sNew;
			m_setProcessID[ret]		= MSGPORT_WORLD;		// 初始化时指向世界核心
			m_setNetBreakFlag[ret]	= false;
			bInsertOk	= true;	
		}


		if(!bInsertOk)
		{
			
			m_setSocket.push_back(sNew);
			m_setProcessID.push_back(MSGPORT_WORLD);		// 初始化时指向世界核心
			m_setNetBreakFlag.push_back(false);
			ret = m_setSocket.size()-1;
		}
		LeaveCriticalSection(&m_ReuseIDListLock);
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CSocketKernel::SendCloseNotify(SOCKET_ID idSocket)
{
	LOGDEBUG("DEBUG：网络核心SOCKET关闭，踢出SOCKET_ID[%d]", idSocket);
#ifdef	NEW_LOGOUT
	//20070518修罗:同时发给世界线程.转发地图组线程.控制某玩家终止onTimer
	//20070623修罗:屏蔽下面功能
	//m_pMsgPort->Send(MSGPORT_WORLD, WORLD_USERCLOSEONTIMER, VARTYPE_INT, &idSocket);			// readme.txt (1-1)

	m_pMsgPort->Send(/*m_pMsgPort->GetSize()-1*/m_setProcessID[idSocket], KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);			// readme.txt (1-1)
#else
	if(m_setProcessID[idSocket] == MSGPORT_WORLD)
		m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);			// readme.txt (1-1)
	else
	{
		for(int j = MSGPORT_MAPGROUP_FIRST; j < m_pMsgPort->GetSize(); j++)			// readme.txt (1-2)
			m_pMsgPort->Send(j, KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);
	}
#endif

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// global entry
///////////////////////////////////////////////////////////////////////////////////////
ISocketKernel* ISocketKernel::CreateNew()
{
	return new CSocketKernel;
}

