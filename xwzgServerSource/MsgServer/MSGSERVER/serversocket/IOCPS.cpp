// IOCPS.cpp: implementation of the IOCPS class.
//
//////////////////////////////////////////////////////////////////////

/*#include "stdafx.h"*/
#include "IOCPS.h"
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

#include "mswsock.h"

#include "../share/define.h"
#include "protocol.h"
#include "EncryptServer.h"
#include "SSocket.h"
#include "I_Shell.h"
#include "ServerSocket.h"
#include "I_MessagePort.h"
#include "basefunc.h"

#include "ServerSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define POSTACCEPTNUM	50
#define	NEXTSOCKETNUM	50
#define CURSOCKETNUM	600
#define	PREALLOCBUFF	4096
#define	PREALLOCCONTECT	1024
#define	POSTACPSOCKET	50

GUID IOCPS::g_GUIDAcceptEx = WSAID_ACCEPTEX;

IOCPS::IOCPS() :m_BuffMemPool(/*PREALLOCBUFF/4*/-1),
				m_SendClientMemPool(/*PREALLOCBUFF*/-1),
				m_SendNpcMemPool(/*PREALLOCBUFF/4*/-1),
				m_ContextMemPool(/*PREALLOCCONTECT*/-1)
{
	
	m_bShutDown=FALSE;
	WSADATA wsaData;
	m_iWSAInitResult=WSAStartup(MAKEWORD(2,2), &wsaData);	
	m_socListen=NULL;
	m_ListenThreadHandle=NULL;
	m_iMaxNumberOfFreeContext=2;
	m_bAcceptConnections=TRUE;
	m_bServerStarted=FALSE;
	m_iMaxNumConnections=1201;
	//m_bAcceptJobs=TRUE;
	m_nOfWorkers=1;
	m_nPortNumber=999;
	
	m_NumberOfActiveConnections=0;
	//m_bSendInOrder=FALSE;
	//m_bReadInOrder=FALSE;
	m_iNumberOfPendlingReads=3;

	//InitializeCriticalSection(&m_FreeContextListLock);
	//InitializeCriticalSection(&m_FreeBufferListLock);
	//InitializeCriticalSection(&m_BufferListLock);
	InitializeCriticalSection(&m_ContextMapLock);
	//InitializeCriticalSection(&m_FreeSocketLock);
	//InitializeCriticalSection(&m_SendNpcMemPoolLock);
	
}

IOCPS::~IOCPS()
{
	ShutDown();

//	DeleteCriticalSection(&m_FreeContextListLock);
//	DeleteCriticalSection(&m_FreeBufferListLock);
//	DeleteCriticalSection(&m_BufferListLock);
	DeleteCriticalSection(&m_ContextMapLock);
	//DeleteCriticalSection(&m_FreeSocketLock);
	//DeleteCriticalSection(&m_SendNpcMemPoolLock);

	if(m_iWSAInitResult == NO_ERROR)
		WSACleanup();	
}




/*
* Start the Client/Server.
* 
*
*/


BOOL IOCPS::Start(int nPort,int iMaxNumConnections,int iMaxIOWorkers,int nOfWorkers,int iMaxNumberOfFreeBuffer,int iMaxNumberOfFreeContext, BOOL bOrderedSend, BOOL bOrderedRead, int iNumberOfPendlingReads,int iInitAcceptNum)
{
	m_iMaxNumberOfFreeBuffer=iMaxNumberOfFreeBuffer;
	m_iMaxNumConnections=iMaxNumConnections;
	m_nPortNumber=nPort;
	m_iMaxIOWorkers=iMaxIOWorkers;
	m_nOfWorkers=nOfWorkers;
	m_iMaxNumberOfFreeContext=iMaxNumberOfFreeContext;
	m_bShutDown=FALSE;
	//m_bReadInOrder=bOrderedRead;
	//m_bSendInOrder=bOrderedSend;
	m_iNumberOfPendlingReads=iNumberOfPendlingReads;
	m_InitAcceptNum = iInitAcceptNum;
	return Startup();
}

BOOL IOCPS::Startup()
{
	// Some special cleanup 


	m_NumberOfActiveConnections=0;


	if ( m_iWSAInitResult != NO_ERROR )
	{
		
		return FALSE;	
	}

	BOOL bRet=TRUE;
	m_bAcceptConnections=TRUE;

	if ( !m_bServerStarted )
	{
		m_bShutDown=FALSE;

		//计算CPU个数

		//SYSTEM_INFO si;

		//GetSystemInfo(&si);

		//m_iMaxIOWorkers = si.dwNumberOfProcessors;
		m_iMaxIOWorkers = 1;
		/*
		*	When using multiple pendling reads (eg m_iNumberOfPendlingReads>1) 
		*  with multiple IOworkers (eg m_iMaxIOWorkers>1), the order of the 
		*  packages are broken, because of IOCP internal design, Furthermore there is no 
		*  Performance gain to have multiple pendling reads if we use more than one IO-worker.
		*  The throughput is going upp because of serveral pendling reads, but is going down because of 
		*  the ordering part. 
		*/
		//if ( m_iMaxIOWorkers>1 ) // we have some sort of bugg somewhere.. 
		//	m_iNumberOfPendlingReads=si.dwNumberOfProcessors;

		if(m_iNumberOfPendlingReads<=0)
			m_iNumberOfPendlingReads=1;
	

		// The map must be empty
		m_ContextMap.clear();
		
		//初始化socket缓冲池
		BatchCreateSocket(m_InitAcceptNum);

		// Create the CompletionPort used by IO Worker Threads.  
		bRet&=CreateCompletionPort();
		
		// Config the Listner.. 
		if ( m_nPortNumber>0 )
		{
			bRet&=SetupListner();
		}
		// Setup the IOWorkers.. 
		bRet&=SetupIOWorkers();


		//setup the dispatch
		bRet&=SetupDispatch();

		//setup write thread
		bRet&=SetupWriteProcess();

		//m_bAcceptJobs=TRUE;
		m_bServerStarted=TRUE;

	}
	return bRet; 	
}


DWORD IOCPS::ListnerThreadProc(LPVOID pParam)
{
	IOCPS* pThis = reinterpret_cast<IOCPS*>(pParam);
	HANDLE EventArray[2];
	EventArray[0] = pThis->m_hEvent;
	EventArray[1] = pThis->m_hAcpEvent;
	if ( pThis )
	{
		while ( !pThis->m_bShutDown )
		{


			DWORD dwRet;
			dwRet = WSAWaitForMultipleEvents(2,
				EventArray,
				FALSE,
				500,
				FALSE);

			if(pThis->m_bShutDown)
				break;

			
			if (dwRet == WSA_WAIT_TIMEOUT)
			{


				
				continue;
				
			}

			
			
			dwRet -=WSA_WAIT_EVENT_0;
			if(dwRet==0)
			{
				WSANETWORKEVENTS events;
				int nRet = WSAEnumNetworkEvents(pThis->m_socListen,
					pThis->m_hEvent,
					&events);

				if (nRet == SOCKET_ERROR)
				{
					//TRACE(_T("WSAEnumNetworkEvents error %ld\n"),WSAGetLastError());
					break;
				}
				if ( events.lNetworkEvents & FD_ACCEPT)
				{
					if ( events.iErrorCode[FD_ACCEPT_BIT] == 0&&pThis->m_bAcceptConnections&&!pThis->m_bShutDown)
					{
						
						pThis->PostAcceptEx(pThis->m_socListen,POSTACPSOCKET);
						
					}
					else
					{
						//TRACE(_T("Unknown network event error %ld\n"),WSAGetLastError());
						break;

					}
				}
			}
			else if(dwRet==1)
			{
				pThis->PostAcceptEx(pThis->m_socListen,1);
			}
		}
	}
//	TRACE("Listner Died!\r\n");
	return 0xdead;

}

DWORD IOCPS::IOWorkerThreadProc(LPVOID pParam)
{
	IOCPS* pThis = reinterpret_cast<IOCPS*>(pParam);
	if ( pThis )
	{
		DWORD dwIoSize;
		ClientContext* lpClientContext;
		CIOCPBuffer *pOverlapBuff;
		bool bError=false;
		HANDLE hCompletionPort = pThis->m_hCompletionPort;
		LPOVERLAPPED lpOverlapped;
		
		while ( !bError )
		{
			pOverlapBuff=NULL;
			lpClientContext=NULL;
			// Get a completed IO request.
			BOOL bIORet = GetQueuedCompletionStatus(
				hCompletionPort,
				&dwIoSize,
				(LPDWORD) &lpClientContext,
				&lpOverlapped, INFINITE);
			

			// If Something whent wrong..
			if (!bIORet)  
			{
					
				if (lpClientContext!=NULL)
				{
	
					if(lpClientContext->m_Socket!=pThis->m_socListen)//是客户端socket
					{
						DWORD nLastError = GetLastError();
						
						
						DWORD nflags;

						if(!WSAGetOverlappedResult(lpClientContext->m_Socket,lpOverlapped,&dwIoSize,FALSE,&nflags))
						{
							DWORD nWsaError = WSAGetLastError();

							LOGERROR("IOWorkerThreadProc中客户端socket关闭 ErrorCode:%d-%d SOCKETID:%d",nLastError,nWsaError,lpClientContext->m_Socket);
						}
						
						pThis->DisconnectClient(lpClientContext); 
						
						pThis->ReleaseClientContext(lpClientContext);
					
					}
					
					// Clear the buffer if returned. 
					pOverlapBuff=NULL;
					if(lpOverlapped!=NULL)
						pOverlapBuff=CONTAINING_RECORD(lpOverlapped, CIOCPBuffer, m_ol);
					if(pOverlapBuff!=NULL)
					{
						if(lpClientContext->m_Socket==pThis->m_socListen)
						{
							//释放刚刚连接到服务端的socket
							closesocket(pOverlapBuff->m_Accept);
							pThis->ReleaseBuffer(pOverlapBuff);
							LOGERROR("IOWorkerThreadProc中,释放刚刚连接到服务端的socket SOCKETID=%d",lpClientContext->m_Socket);
						}
						else if(pOverlapBuff->m_Accept==IOCPS::SENDCLIENT)
						{
							//释放client的write buffer
							pThis->m_SendClientMemPool.ReleaseObject(pOverlapBuff);
							LOGERROR("IOWorkerThreadProc中,释放client的write buffer SOCKETID=%d",lpClientContext->m_Socket);
						}
						else if(pOverlapBuff->m_Accept==IOCPS::SENDNPC)
						{
							//释放NPC的write buffer
							pThis->m_SendNpcMemPool.ReleaseObject(pOverlapBuff);
							LOGERROR("IOWorkerThreadProc中,释放NPC的write buffer SOCKETID=%d",lpClientContext->m_Socket);
						}
						else
						{
							//释放read的buffer
							pThis->ReleaseBuffer(pOverlapBuff);
							LOGERROR("IOWorkerThreadProc中,释放read的buffer SOCKETID=%d",lpClientContext->m_Socket);
						}
					}
					continue;
				}
				// We shall never come here  
				// anyway this was an error and we should exit the worker thread
				bError = true;
//				pThis->AppendLog(pThis->ErrorCode2Text(dwIOError)); 
//				pThis->AppendLog("IOWORKER KILLED BECAUSE OF ERROR IN GetQueuedCompletionStatus");
				LOGERROR("IOWorkerThreadProc BE KILLED BECAUSE OF ERROR IN GetQueuedCompletionStatus");	
				pOverlapBuff=NULL;
				if(lpOverlapped!=NULL)
					pOverlapBuff=CONTAINING_RECORD(lpOverlapped, CIOCPBuffer, m_ol);
				if(pOverlapBuff!=NULL)
					pThis->ReleaseBuffer(pOverlapBuff);
				continue;
				
			}

			if(bIORet && lpOverlapped && lpClientContext) 
			{
				pOverlapBuff=CONTAINING_RECORD(lpOverlapped, CIOCPBuffer, m_ol);
				if(pOverlapBuff!=NULL)
					pThis->ProcessIOMessage(pOverlapBuff, lpClientContext, dwIoSize);
			}	

			if(lpClientContext==NULL&&pOverlapBuff==NULL&&pThis->m_bShutDown)
			{
				
				bError=true;
			}

		} 

	}

	
	return 0L;
}

//主要是完成read complete处理逻辑
DWORD IOCPS::DispatchThreadProc(LPVOID pParam)
{
	IOCPS* pThis = reinterpret_cast<IOCPS*>(pParam);
	if ( pThis )
	{
		DWORD dwIoSize;
		ClientContext* lpClientContext;
		CIOCPBuffer *pOverlapBuff;
		bool bError=false;
		HANDLE hCompletionPort = pThis->m_hDispatchPort;
		LPOVERLAPPED lpOverlapped;
		
		while ( !bError )
		{
			pOverlapBuff=NULL;
			lpClientContext=NULL;
			// Get a completed IO request.
			BOOL bIORet = GetQueuedCompletionStatus(
				hCompletionPort,
				&dwIoSize,
				(LPDWORD) &lpClientContext,
				&lpOverlapped, INFINITE);
			
			if(!bIORet)
			{
				LOGERROR("DispatchThreadProc函数处理GetQueuedCompletionStatus失败 ErrorCode:%d",GetLastError());
				pOverlapBuff = NULL;

				if(lpOverlapped!=NULL)
					pOverlapBuff=CONTAINING_RECORD(lpOverlapped, CIOCPBuffer, m_ol);

				if(pOverlapBuff!=NULL)
					pThis->ReleaseBuffer(pOverlapBuff);

				

				continue;
			}

			if(bIORet && lpOverlapped && lpClientContext) 
			{
				pOverlapBuff=CONTAINING_RECORD(lpOverlapped, CIOCPBuffer, m_ol);
				if(pOverlapBuff!=NULL)
					pThis->ProcessDispatchMessage(pOverlapBuff, lpClientContext, dwIoSize);
			}	

			if(lpClientContext==NULL&&pOverlapBuff==NULL&&pThis->m_bShutDown)
			{
				
				bError=true;
			}
		}
	}
	return 0L;
}

//主要是完成write处理逻辑ProcessWriteMessage函数完成WSASend操作
DWORD IOCPS::WriteThreadProc(LPVOID pParam)
{
	IOCPS* pThis = reinterpret_cast<IOCPS*>(pParam);
	if ( pThis )
	{
		DWORD dwIoSize;
		ClientContext* lpClientContext;
		CIOCPBuffer *pOverlapBuff;
		bool bError=false;
		HANDLE hCompletionPort = pThis->m_hWritePort;
		LPOVERLAPPED lpOverlapped;
		
		while ( !bError )
		{
			pOverlapBuff=NULL;
			lpClientContext=NULL;
			// Get a completed IO request.
			BOOL bIORet = GetQueuedCompletionStatus(
				hCompletionPort,
				&dwIoSize,
				(LPDWORD) &lpClientContext,
				&lpOverlapped, INFINITE);
			if(!bIORet)
			{
				LOGERROR("WriteThreadProc函数处理GetQueuedCompletionStatus失败 ErrorCode:%d",GetLastError());
				pOverlapBuff = NULL;
				if(lpOverlapped!=NULL)
					pOverlapBuff=CONTAINING_RECORD(lpOverlapped, CIOCPBuffer, m_ol);
				if(pOverlapBuff!=NULL)
					pThis->ReleaseBuffer(pOverlapBuff);

				

				continue;
			}

			if(bIORet && lpOverlapped && lpClientContext) 
			{
				pOverlapBuff=CONTAINING_RECORD(lpOverlapped, CIOCPBuffer, m_ol);
				if(pOverlapBuff!=NULL)
					pThis->ProcessWriteMessage(pOverlapBuff, lpClientContext, dwIoSize);
			}	

			if(lpClientContext==NULL&&pOverlapBuff==NULL&&pThis->m_bShutDown)
			{
				
				bError=true;
			}
		}
	}
	return 0L;
}

/*
* AssociateIncomingClientWithContext
* 
* This function do the Following: 
* 1) Does some simpleSecutity Stuff (e.g one connection per client, etc..)
* 2) Allocates an Context for the Socket. 
* 3) Configures the Socket. 
* 4) Associate the Socket and the context with the completion port. 
* 5) Fires an IOInitialize So the IOWORKERS Start to work on the connection. 
*/
//IO WORKER thread 将会调用该函数
BOOL IOCPS::AssociateIncomingClientWithContext(CIOCPBuffer* pBuff,DWORD dwSize)
{
	if(!pBuff)
		return FALSE;

	if (m_bShutDown||!m_bAcceptConnections||pBuff->m_Accept==INVALID_SOCKET)
	{
		ReleaseBuffer(pBuff);
		return FALSE;
	}

	// Close connection if we have reached the maximum nr of connections...
	//EnterCriticalSection(&m_ContextMapLock);
	if(m_NumberOfActiveConnections>=m_iMaxNumConnections||dwSize==0)
	{
		// Disconnect. 
		LINGER lingerStruct;
		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0;
		setsockopt( pBuff->m_Accept, SOL_SOCKET, SO_LINGER,
			(char *)&lingerStruct, sizeof(lingerStruct) );

		// Now close the socket handle. This will do an abortive or graceful close, as requested. 
		CancelIo((HANDLE) (pBuff->m_Accept));

		closesocket( pBuff->m_Accept );
		pBuff->m_Accept = INVALID_SOCKET;
		ReleaseBuffer(pBuff);
		return FALSE; 
	}

	// Create the Client context to be associted with the completion port
	ClientContext* pContext = AllocateContext();

	if(pContext!=NULL)
	{

		pContext->m_Socket = pBuff->m_Accept;
		// M_ID is Used for Delete(). Should we remove this soon ? 
		pContext->m_ID=pBuff->m_Accept;

		/* 
		* TCP_NODELAY	BOOL=TRUE Disables the "nagle algorithm for send coalescing" which delays
		* short packets in the hope that the application will send more data and allow
		* it to combine them into a single one to improve network efficiency.
		*/
//		const char chOpt = 1;
//		int nErr = setsockopt(pContext->m_Socket, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));
//		if (nErr == -1)
//		{
//
//			closesocket(pContext->m_Socket);
//			ReleaseBuffer(pBuff);
//			m_ContextMemPool.ReleaseObject(pContext);
//			return FALSE;
//		}

		if(AddClientContext(pContext))
		{	


			// Associate the new socket with a completion port.
			if (!AssociateSocketWithCompletionPort(pBuff->m_Accept, m_hCompletionPort, (DWORD) pContext))
			{

				ReleaseBuffer(pBuff);
				DisconnectClient(pContext);
				m_ContextMemPool.ReleaseObject(pContext);
				return FALSE;
			}

			//
			//向kernel注册自己
			pContext->m_ClientID = m_pKernel->SetNewSocket(pContext);

			

			//处理第一块数据,统一交给dispatch线程处理
			

			//投递给dispatch thread 处理
			pBuff->SetOperation(IOReadCompleted);

			BOOL bSucessed = PostQueuedCompletionStatus(m_hDispatchPort, dwSize, (DWORD) pContext, &pBuff->m_ol);
			
			if ( (!bSucessed && GetLastError( ) != ERROR_IO_PENDING))
			{   
				
				ReleaseBuffer(pBuff);
				DisconnectClient(pContext);
				m_ContextMemPool.ReleaseObject(pContext);
				
				return FALSE;
			}

			// Trigger first IO Completion Request
			// Otherwise the Worker thread will remain blocked waiting for GetQueuedCompletionStatus...
			

			// Important!! EnterIOLoop must notify that the socket and the structure  
			// pContext have an Pendling IO operation ant should not be deleted.
			// This is nessesary to avoid Access violation. 
			
			for(int i=0;i<m_iNumberOfPendlingReads;i++)
			{
				EnterIOLoop(pContext); // One for each Read Loop
				OnRead(pContext);
			}
			
			//test
			if(m_NumberOfActiveConnections>=600||m_NumberOfActiveConnections>=700||m_NumberOfActiveConnections>=750||m_NumberOfActiveConnections>=800||m_NumberOfActiveConnections>=850)
			{
				//output new count
				LOGERROR("Connection=%d",m_NumberOfActiveConnections);
				LOGERROR("m_BuffMemPool: %d",m_BuffMemPool.newcount);
				LOGERROR("m_SendClientMemPool: %d",m_SendClientMemPool.newcount);
				LOGERROR("m_SendNpcMemPool: %d",m_SendNpcMemPool.newcount);
			}

		}else
		{
			//添加不成功,pcontext不需要释放,在AddClientContext中已经释放了
			ReleaseBuffer(pBuff);
			//DisconnectClient(pContext);
			//m_ContextMemPool.ReleaseObject(pContext);
			return FALSE;
		}
	}else
	{

		closesocket(pBuff->m_Accept);
		ReleaseBuffer(pBuff);
		return FALSE;
	}
	return TRUE;
}


BOOL IOCPS::AssociateSocketWithCompletionPort(SOCKET socket, HANDLE hCompletionPort, DWORD dwCompletionKey)
{
	HANDLE h = CreateIoCompletionPort((HANDLE) socket, hCompletionPort, dwCompletionKey, 0);
	return h == hCompletionPort;
}


BOOL IOCPS::AddClientContext(ClientContext *mp)
{
	EnterCriticalSection(&m_ContextMapLock);

	unsigned int KeyID=mp->m_Socket;

	mp->m_ID=KeyID;
	
	pair<ContextMap::iterator,bool> ret =  m_ContextMap.insert(make_pair(KeyID,mp));
	if(ret.second==false)
	{
		//插入失败
		AbortiveClose(mp);
		LeaveCriticalSection(&m_ContextMapLock);
		LOGERROR("AddClientContext Error");
		return FALSE;
	}
	m_NumberOfActiveConnections++;

	LeaveCriticalSection(&m_ContextMapLock);
	return TRUE;
}



/*
* Closes all the Sockets and removes all the buffer and ClientContext.  
*
*/ 
void IOCPS::FreeClientContext()
{
	
	EnterCriticalSection(&m_ContextMapLock);
	
	ContextMap::iterator iter;
	for(iter=m_ContextMap.begin();iter!=m_ContextMap.end();++iter)
	{
		//unsigned int iKey;
		ClientContext *pContext=NULL;
		//m_ContextMap.GetNextAssoc (pos, iKey,pContext);
		pContext = iter->second;
		if(pContext)
		{
			// Socket open we have to kill it.. 
			if(pContext->m_Socket>=0)
			{
				LINGER lingerStruct;
				lingerStruct.l_onoff = 1;
				lingerStruct.l_linger = 0;
				setsockopt( pContext->m_Socket, SOL_SOCKET, SO_LINGER,
					(char *)&lingerStruct, sizeof(lingerStruct) );
				// Now close the socket handle.  This will do an abortive or  graceful close, as requested.  
				CancelIo((HANDLE) pContext->m_Socket);
				//while (!HasOverlappedIoCompleted((LPOVERLAPPED)pContext)) 
				//	Sleep(2);
				closesocket( pContext->m_Socket );

			}
			pContext->m_Socket = INVALID_SOCKET;


			ReleaseBuffer(pContext->m_pBuffOverlappedPackage);


			//ReleaseBufferMap(&pContext->m_ReadBufferMap);
			//ReleaseSendBufferMap(&pContext->m_SendBufferMap);

			m_ContextMemPool.ReleaseObject(pContext);
		}
	}
	// Now remove all the keys.. 
	m_ContextMap.clear();
	LeaveCriticalSection(&m_ContextMapLock); 
}

//目前只需处理IOREADCOMPLETE
void IOCPS::ProcessDispatchMessage(CIOCPBuffer *pOverlapBuff, ClientContext* pContext, DWORD dwSize)
{
	if(pOverlapBuff==NULL)
		return;

	switch (pOverlapBuff->GetOperation())
	{

	case IOReadCompleted :
		ProcessReadCompleted(pContext,dwSize,pOverlapBuff);
		break;
	} 

}

//目前只需处理IOWRITE  消息
void IOCPS::ProcessWriteMessage(CIOCPBuffer *pOverlapBuff, ClientContext* pContext, DWORD dwSize)
{
	if(pOverlapBuff==NULL)
		return;

	switch (pOverlapBuff->GetOperation())
	{

	case IOWrite :
		ProcessWriteIO(pContext,dwSize,pOverlapBuff);
		break;
	
	} 
}

void IOCPS::ProcessIOMessage(CIOCPBuffer *pOverlapBuff, ClientContext *pContext, DWORD dwSize)
{

	if(pOverlapBuff==NULL)
		return;

	switch (pOverlapBuff->GetOperation())
	{

//	case IOInitialize :
//		OnInitialize(pContext,dwSize,pOverlapBuff);
//		break;

//	case IORead :
//		OnRead(pContext,pOverlapBuff);
//		break;	


	case IOReadCompleted :
		OnReadCompleted(pContext,dwSize,pOverlapBuff);
		break;


//	case IOWrite :
//		OnWrite(pContext,dwSize,pOverlapBuff);
//		break;

	case IOWriteCompleted :
		OnWriteCompleted(pContext,dwSize,pOverlapBuff);
		break;

//	case IOZeroByteRead : //  Workaround the the WSAENOBUFS error problem..See OnZeroByteRead. 
//		OnZeroByteRead(pContext,pOverlapBuff);
//		break;
//
//	case IOZeroReadCompleted : //  Workaround the the WSAENOBUFS error problem.. 
//		OnZeroByteReadCompleted(pContext,dwSize,pOverlapBuff);
//		break;

//	case IOPostedPackage : 
//		OnPostedPackage(pContext,pOverlapBuff);
//		break;

	case IOAccept:
		
		{
			ProcessAcceptExData(pContext,dwSize,pOverlapBuff);
		}
		break;

	default:
		ReleaseBuffer(pOverlapBuff);
		break;

	} 
}

void IOCPS::ProcessAcceptExData(ClientContext* pContext,DWORD dwSize,CIOCPBuffer* pOverlapBuff)
{
	if(!AssociateIncomingClientWithContext(pOverlapBuff,dwSize))
	{
		
		LOGERROR("AssociateIncomingClientWithContext(pOverlapBuff,dwSize) error!");
	}
	
	SetEvent(m_hAcpEvent);
}

void IOCPS::OnInitialize(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff)
{
	// Do some init here.. 

	// Notify new connection. 
//	EnterCriticalSection(&(pContext->m_ContextLock));
//	NotifyNewConnection(pContext);
//	LeaveCriticalSection(&(pContext->m_ContextLock));
	/*
	Operations using the IO completion port will always complete in the order that they were submitted.
	Therefore we start A number of pendling read loops (R) and at last a Zero byte read to avoid the WSAENOBUFS problem.  	
	The number of m_iNumberOfPendlingReads should not be so big that we get the WSAENOBUFS problem. 
	*/
	// A ZeroByteLoop. EnterIOLoop is not needed here. Already done in previus call.
	//AZeroByteRead(pContext,pOverlapBuff);
	
	// m_iNumberOfPendlingReads=1 by default. 
	for(int i=0;i<m_iNumberOfPendlingReads;i++)
	{
		EnterIOLoop(pContext); // One for each Read Loop
		OnRead(pContext);
	}

}

void IOCPS::OnRead(ClientContext *pContext,CIOCPBuffer *pOverlapBuff)
{
	// issue a read request 
	if(pContext&&!m_bShutDown)
	{
		if(pOverlapBuff==NULL)
		{
			pOverlapBuff=AllocateBuffer(IOReadCompleted);
			if(pOverlapBuff==NULL)
			{
				DisconnectClient(pContext);
				ReleaseClientContext(pContext);
				return;
			}
		}
		if(pOverlapBuff!=NULL)
		{
			//pOverlapBuff->SetOperation(IOReadCompleted);
			pOverlapBuff->SetupRead();
//			if(m_bReadInOrder)
//				MakeOrderdRead(pContext,pOverlapBuff);
//			else
			{
				DWORD dwIoSize=0;
				ULONG			ulFlags = MSG_PARTIAL;
				UINT nRetVal = WSARecv(pContext->m_Socket, 
					pOverlapBuff->GetWSABuffer(),
					1,
					&dwIoSize, 
					&ulFlags,
					&pOverlapBuff->m_ol, 
					NULL);
				if ( nRetVal == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) 
				{
					LOGERROR("OnRead Error ErrorCode:%d SOCKETID=%d",GetLastError(),pContext->m_Socket);
					ReleaseBuffer(pOverlapBuff);
					DisconnectClient(pContext);
					ReleaseClientContext(pContext);
				}
			}
		}
	}
}


void IOCPS::OnPostedPackage(ClientContext *pContext, CIOCPBuffer *pOverlapBuff)
{
	if( !pContext )
	{
		AppendLog("FATAL ERROR OnPostedPackage pContext=NULL");
		return; 
	}

	if ( pOverlapBuff)
	{
		short nSize = 0;
		memmove(&nSize,pOverlapBuff->GetBuffer(),MINIMUMPACKAGESIZE);
		EnterCriticalSection(&pContext->m_ContextLock);
		NotifyReceivedPackage(pOverlapBuff,nSize,pContext);
		LeaveCriticalSection(&pContext->m_ContextLock);
		ReleaseBuffer(pOverlapBuff);
	}
	ReleaseClientContext(pContext);// Exit IOLoop	
}

void IOCPS::OnReadCompleted(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff)
{

	if (dwIoSize == 0||pOverlapBuff==NULL)
	{
		DisconnectClient(pContext);
		ReleaseClientContext(pContext);
		ReleaseBuffer(pOverlapBuff);
		return;
	}


	if(pContext)
	{
		//投递给dispatch thread 处理
		pOverlapBuff->SetOperation(IOReadCompleted);

		BOOL bSucessed = PostQueuedCompletionStatus(m_hDispatchPort, dwIoSize, (DWORD) pContext, &pOverlapBuff->m_ol);
		
		if ( (!bSucessed && GetLastError( ) != ERROR_IO_PENDING))
		{   
			
			DisconnectClient(pContext);
			ReleaseClientContext(pContext);
			ReleaseBuffer(pOverlapBuff);
			return;
		}

		OnRead(pContext);
	}
}

//由dispatch thread调用
void IOCPS::ProcessReadCompleted(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff)
{
//	if(m_bReadInOrder)
//			pOverlapBuff=GetNextReadBuffer(pContext,pOverlapBuff);


		if(pOverlapBuff!=NULL)
		{
			

			// Mark that we are Using the buffer..
			pOverlapBuff->Use(dwIoSize);

			ProcessPackage(pContext,dwIoSize,pOverlapBuff);

			
//			pOverlapBuff=NULL;
//			if(m_bReadInOrder)
//				pOverlapBuff=GetNextReadBuffer(pContext);
		}
}

//由write线程调用
void IOCPS::ProcessWriteIO(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff)
{
	OnWrite(pContext,dwIoSize,pOverlapBuff);
}

void IOCPS::ProcessWriteIOComplete(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff)
{
}

/*
* Assumes that Packages arrive with A MINIMUMPACKAGESIZE header and builds Packages that  
* are noticed by the virtual function NotifyReceivedPackage 
*
*/


void IOCPS::ProcessPackage(ClientContext *pContext, DWORD dwIoSize, CIOCPBuffer *pOverlapBuff)
{

	//解密
	if(pOverlapBuff)
	{
	
// #ifdef	ENCRYPT

		pContext->m_cEncryptRecv.Encrypt((unsigned char *)(pOverlapBuff->GetBuffer()),pOverlapBuff->GetUsed());

// #endif
			
	}
	//
	// We may have Several Pending reads. And therefor we have to 
	// check and handle partial Messages.  
	//

	//
	// First handle partial packages. 
	//
	CIOCPBuffer *pBuffPartialMessage=NULL;
	pBuffPartialMessage=pContext->m_pBuffOverlappedPackage;

	// if we had a partial message in previus messageprocess. 
	if ( pBuffPartialMessage!=NULL )
	{

		// Check how big the message is...
		UINT nUsedBuffer=pBuffPartialMessage->GetUsed();

		if ( nUsedBuffer < MINIMUMPACKAGESIZE )
		{
			//Header to small.. 
			UINT nHowMuchIsNeeded=MINIMUMPACKAGESIZE-nUsedBuffer;
			// too little Data to determine the size. 
			if ( nHowMuchIsNeeded > pOverlapBuff->GetUsed() )
			{
				AddAndFlush(pOverlapBuff,pBuffPartialMessage,pOverlapBuff->GetUsed());
				// Release the buffer if not used. 
				if ( pOverlapBuff!=NULL )
				{
					ReleaseBuffer(pOverlapBuff);
				}
				return; // wait for more data..
			}else
				AddAndFlush(pOverlapBuff,pBuffPartialMessage,nHowMuchIsNeeded);
		}
		// Check how big the message is...
		nUsedBuffer=pBuffPartialMessage->GetUsed();
		if ( nUsedBuffer >= MINIMUMPACKAGESIZE )
		{
			// Get The size.. 
			short nSize = 0;
			UINT nHowMuchIsNeeded=0;
			memmove(&nSize,pBuffPartialMessage->GetBuffer(),MINIMUMPACKAGESIZE);
			// The Overlapped Package is good. Never send packages bigger that the MAXIMUMPACKAGESIZE-MINIMUMPACKAGESIZE
			if ( nSize<=(MAXIMUMPACKAGESIZE/*-MINIMUMPACKAGESIZE*/) )
			{
				nHowMuchIsNeeded=nSize-(nUsedBuffer/*-MINIMUMPACKAGESIZE*/);
				// If we need just a little data add it.. 
				if ( nHowMuchIsNeeded<=pOverlapBuff->GetUsed() )
				{
					// Add the remain into pBuffPartialMessage. 
					AddAndFlush(pOverlapBuff,pBuffPartialMessage,nHowMuchIsNeeded);
					NotifyReceivedPackage(pBuffPartialMessage,nSize,pContext);
					ReleaseBuffer(pContext->m_pBuffOverlappedPackage);
					pContext->m_pBuffOverlappedPackage=NULL;
				}else
				{
					// Put everything in.. 
					AddAndFlush(pOverlapBuff,pBuffPartialMessage,pOverlapBuff->GetUsed());
				}

			}
			else
			{
//				CString msg;
//				msg.Format("Package size error > MAXIMUMPACKAGESIZE-MINIMUMPACKAGESIZE: %s",ErrorCode2Text(WSAGetLastError()));
//				AppendLog(msg);
				ReleaseBuffer(pOverlapBuff);
				pOverlapBuff=NULL;
				ReleaseBuffer(pContext->m_pBuffOverlappedPackage);
				pContext->m_pBuffOverlappedPackage=NULL;
				DisconnectClient(pContext);
				return;

			}
		}
	}


	//
	// Process the incoming byte stream in pOverlapBuff
	//
	bool done;
	do 
	{
		UINT nUsedBuffer=pOverlapBuff->GetUsed();
		done=true;	
		if ( nUsedBuffer >= MINIMUMPACKAGESIZE )
		{
			short nSize = 0;
			memmove(&nSize,pOverlapBuff->GetBuffer(),MINIMUMPACKAGESIZE);
			// We Have a full Package..
			if ( nSize==nUsedBuffer/*-MINIMUMPACKAGESIZE*/ )
			{
				NotifyReceivedPackage(pOverlapBuff,nSize,pContext);
				pOverlapBuff->EmptyUsed();
				done = true;
			}else if ( nUsedBuffer/*-MINIMUMPACKAGESIZE*/ >nSize )
			{
				// We have more data 
				CIOCPBuffer *pBuff=SplitBuffer(pOverlapBuff,nSize/*+MINIMUMPACKAGESIZE*/);
				NotifyReceivedPackage(pBuff,nSize,pContext);
				ReleaseBuffer(pBuff);
				// loop again, we may have another complete message in there...
				done = false;
			}else if ( nUsedBuffer/*-MINIMUMPACKAGESIZE*/<nSize && nSize<MAXIMUMPACKAGESIZE)
			{
				//
				// The package is overlapped between this byte chunk stream and the next. 
				//
				pContext->m_pBuffOverlappedPackage=pOverlapBuff;
				pOverlapBuff=NULL;
				// TRACE("new Partial Buffer >\r\n");
				done=true;
			}else if ( nSize>MAXIMUMPACKAGESIZE )
			{			
				//pOverlapBuff->DUMP();
//				CString msg;
//				msg.Format("Messagesize error: %s",ErrorCode2Text(WSAGetLastError()));
//				AppendLog(msg);

				ReleaseBuffer(pOverlapBuff);
				pOverlapBuff=NULL;
				DisconnectClient(pContext);
				break; 
			}
		}else if( nUsedBuffer>0 )
		{   //  Header  too small. 
			// nUsedBuffer < MINIMUMPACKAGESIZE
			// Add it to to the package overlapped buffer. 	
			// Let the remain be handled later. 
			pContext->m_pBuffOverlappedPackage=pOverlapBuff;
			pOverlapBuff=NULL;
			done=true;
		}
	} while ( !done );
	// Release the buffer if not used. 
	if ( pOverlapBuff!=NULL )
	{
		ReleaseBuffer(pOverlapBuff);
	}
}



void IOCPS::OnWrite(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff)
{
	if(pContext!=NULL&&pContext->m_Socket!=INVALID_SOCKET)
	{	
		if(pOverlapBuff!=NULL)
		{
	
			pOverlapBuff->SetOperation(IOWriteCompleted);
			pOverlapBuff->SetupWrite();	
			ULONG ulFlags = MSG_PARTIAL;
			DWORD dwSendNumBytes = 0;

			int nRetVal = WSASend(pContext->m_Socket, 
				pOverlapBuff->GetWSABuffer(),
				1,
				&dwSendNumBytes, 
				ulFlags,
				&pOverlapBuff->m_ol, 
				NULL);

			if ( nRetVal == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING ) 
			{
				LOGERROR("OnWrite Error ErrorCode:%d SOCKETID=%d",WSAGetLastError(),pContext->m_Socket);
				ReleaseClientNpcBuffer(pOverlapBuff);
				DisconnectClient(pContext);
				
						
				ReleaseClientContext(pContext); 
				
				
				
			}
		}
		
	}else
	{
		
		ReleaseClientNpcBuffer(pOverlapBuff);
		ReleaseClientContext(pContext);
	}

}



void IOCPS::OnWriteCompleted(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff)
{
	if(pContext!=NULL)
	{

		if (pOverlapBuff!=NULL)
		{
			ReleaseClientNpcBuffer(pOverlapBuff);
		}
		
		ReleaseClientContext(pContext);
	}
}



BOOL IOCPS::ASend(int ClientId, CIOCPBuffer *pOverlapBuff)
{
	BOOL bRet=FALSE;
	EnterCriticalSection(&m_ContextMapLock);
	ClientContext* pContext = FindClient(ClientId);
	if (pContext == NULL)
	{
		LeaveCriticalSection(&m_ContextMapLock);
		ReleaseBuffer(pOverlapBuff);
		return FALSE;
	}
	bRet=ASend(pContext,pOverlapBuff); // if ASend(pContext,..) Fails, ASend Takes Care of The buffer.. 
	LeaveCriticalSection(&m_ContextMapLock);
	return bRet;
}


BOOL IOCPS::PostPackage(ClientContext* pContext,const char* pBuff,int nSize,UINT nAllocType)
{

	if(pContext)
	{
		CIOCPBuffer* pIOCPBuff = AllocateClientNpcBuffer(IOWrite,nAllocType);
		if(pIOCPBuff)
		{			pIOCPBuff->AddData(pBuff,nSize);

			// #ifdef	ENCRYPT	
			pContext->m_cEncryptSend.Encrypt((unsigned char *)(pIOCPBuff->GetBuffer()), nSize);
			// #endif

			ASend(pContext,pIOCPBuff);
		}
	}
	
	
	return TRUE;
}



BOOL IOCPS::PostPackage(int iClientId, CIOCPBuffer *pOverlapBuff)
{

	BOOL bRet=FALSE;
	EnterCriticalSection(&m_ContextMapLock);
	ClientContext* pContext = FindClient(iClientId);
	if (pContext == NULL)
	{
		LeaveCriticalSection(&m_ContextMapLock);
		ReleaseBuffer(pOverlapBuff);
		return FALSE;
	}
	bRet=PostPackage(pContext,pOverlapBuff); // if ASend(pContext,..) Fails, ASend Takes Care of The buffer.. 
	LeaveCriticalSection(&m_ContextMapLock);
	return bRet;
}


BOOL IOCPS::PostPackage(ClientContext *pContext,CIOCPBuffer *pOverlapBuff)
{
	if( pContext==NULL ||! m_bServerStarted )
	{
		if ( pOverlapBuff )
			ReleaseBuffer(pOverlapBuff);
		return FALSE;
	}

	if ( pContext->m_Socket!=INVALID_SOCKET && pOverlapBuff!=NULL )
	{
		EnterIOLoop(pContext); 	
		pOverlapBuff->SetOperation(IOPostedPackage);
		BOOL bSuccess = PostQueuedCompletionStatus(m_hCompletionPort, pOverlapBuff->GetUsed(), (DWORD) pContext, &pOverlapBuff->m_ol);
		if ( (!bSuccess && GetLastError( ) != ERROR_IO_PENDING))
		{            
			ReleaseBuffer(pOverlapBuff);
			//TRACE(">POST(%x) FAILED.. \r\n",pContext);
			ReleaseClientContext(pContext); // Because of EnterIOLoop. 
			return FALSE;
		}
		return TRUE;
	}

	// Some Thing is Wrong Release the buffer and return. 
	if ( pOverlapBuff )
	{
		ReleaseBuffer(pOverlapBuff);
		pOverlapBuff=NULL;
	}

	return FALSE;
}



BOOL IOCPS::ASendToAll(CIOCPBuffer *pBuff)
{

	if(pBuff==NULL)
		return FALSE;

	BOOL bRet=TRUE;
	EnterCriticalSection(&m_ContextMapLock);
	//POSITION pos = m_ContextMap.GetStartPosition ();
	
	ContextMap::iterator iter;

	for(iter = m_ContextMap.begin();iter!=m_ContextMap.end();++iter)
	{
		//unsigned int iKey;
		ClientContext *pContext=NULL;
		//m_ContextMap.GetNextAssoc (pos, iKey,pContext);
		pContext = iter->second;
		if(pContext!=NULL)
		{
			CIOCPBuffer *pOverlapBuff=AllocateBuffer(IOWrite);
			if(pOverlapBuff!=NULL)
			{
				if(pOverlapBuff->AddData(pBuff->GetBuffer(),pBuff->GetUsed()))
				{
					if(!ASend(pContext,pOverlapBuff))
					{
						//ReleaseBuffer(pOverlapBuff);
						bRet&=FALSE;
					}

				}else
				{
//					CString msg;
//					msg.Format("Buffer Error in  ASendToAll, AddData Failed: %s",ErrorCode2Text(WSAGetLastError()));
//					AppendLog(msg);
					ReleaseBuffer(pOverlapBuff);
					bRet&=FALSE;
					break;	
				}
			}else
			{
//				CString msg;
//				msg.Format("Could not allocate memory ASendToAll: %s",ErrorCode2Text(WSAGetLastError()));
//				AppendLog(msg);
				bRet=FALSE;
				break;
			}
		}
	}
	LeaveCriticalSection(&m_ContextMapLock); 
	ReleaseBuffer(pBuff);
	return bRet;
}




ClientContext* IOCPS::FindClient(unsigned int iClient)
{
	//ClientContext* pCC=NULL;	
	//if(iClient<=0)
	//	return NULL;
	//pCC=(ClientContext*)m_ContextMap[iClient];
	if(m_ContextMap.count(iClient))
		return m_ContextMap[iClient];
	else
		return NULL;
}

string IOCPS::GetHostKeyName(SOCKET socket)
{
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);

	BOOL bResult = getpeername(socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);

	// return bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
	//TRACE("GetHostKeyName-> Socket nr: %i\r\n",socket);
	string szReturn;

	if (bResult != INVALID_SOCKET)
		szReturn = inet_ntoa(sockAddr.sin_addr);
		//szReturn.Format("%d>%s_%d",socket,inet_ntoa(sockAddr.sin_addr), sockAddr.sin_port);
//	else
//		szReturn.Empty();

	return szReturn;
}




string IOCPS::GetHostAddress(SOCKET socket)
{
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);

	BOOL bResult = getpeername(socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	string szReturn;

	if (bResult != INVALID_SOCKET)
		szReturn = inet_ntoa(sockAddr.sin_addr);
//		szReturn.Format("%s", inet_ntoa(sockAddr.sin_addr));
//	else
//		szReturn.Empty();

	return szReturn;
}


/*
* Makes An Asyncorn Send. 发送给IOWRITEPORT
*
*/
BOOL IOCPS::ASend(ClientContext *pContext,CIOCPBuffer *pOverlapBuff)
{
	if(pContext==NULL||!m_bServerStarted)
	{
		ReleaseClientNpcBuffer(pOverlapBuff);
		return FALSE;
	}
	
	BOOL bSucessed = TRUE;
	if(pContext->m_Socket!=INVALID_SOCKET&&pOverlapBuff!=NULL)
	{
		
		EnterIOLoop(pContext); 

//		if(m_bSendInOrder)
//			SetSendSequenceNumber(pContext,pOverlapBuff); 

		bSucessed = PostQueuedCompletionStatus(m_hWritePort, pOverlapBuff->GetUsed(), (DWORD) pContext, &pOverlapBuff->m_ol);
		if ( (!bSucessed && GetLastError( ) != ERROR_IO_PENDING))
		{   
			
			LOGERROR("ASend Error ErrorCode:%d SOCKETID=%d",WSAGetLastError(),pContext->m_Socket);

			ReleaseClientNpcBuffer(pOverlapBuff);
			
			ReleaseClientContext(pContext); // Because of EnterIOLoop. 
			return FALSE;
		}
		return TRUE;
		
	}
	
	return FALSE;
}
/*
* Disconnects the Client. 
*
*
*/

void IOCPS::DisconnectClient(ClientContext *pContext, BOOL bGraceful)
{	
	if(pContext!=NULL)
	{	
		EnterCriticalSection(&(pContext->m_ContextLock));
		BOOL bDisconnect=pContext->m_Socket!=INVALID_SOCKET;
		LeaveCriticalSection(&(pContext->m_ContextLock));
		// If we have an active  socket close it. 
		if(bDisconnect)
		{		

			
			// Remove it From m_ContextMap. 
			
			EnterCriticalSection(&m_ContextMapLock);
			
			
			ContextMap::iterator iter =  m_ContextMap.find(pContext->m_Socket);
			if(iter!=m_ContextMap.end())
			{
				m_ContextMap.erase(iter);
				
				m_NumberOfActiveConnections--;
			}	
			LeaveCriticalSection(&m_ContextMapLock);

			if ( !bGraceful ) 
			{

				LINGER lingerStruct;
				lingerStruct.l_onoff = 1;
				lingerStruct.l_linger = 0;
				setsockopt( pContext->m_Socket, SOL_SOCKET, SO_LINGER,
					(char *)&lingerStruct, sizeof(lingerStruct) );
			}
			//
			// Now close the socket handle.  This will do an abortive or  graceful close, as requested.  
			CancelIo((HANDLE) pContext->m_Socket);
			closesocket( pContext->m_Socket );
			pContext->m_Socket = INVALID_SOCKET;

			if(pContext)
			{
				m_pKernel->ProcessDisconnServer(pContext->m_ClientID);
			}
		}
	}
}


void IOCPS::ReleaseBufferMap(BufferMap *buffmap)
{	
	
	BufferMap::iterator iter = buffmap->begin();
	for(;iter!=buffmap->end();++iter) 
	{
		//unsigned int iKey;
		CIOCPBuffer *pBuffer=NULL;
		pBuffer = iter->second;
		//map->GetNextAssoc (pos, iKey,pBuffer);
		if(pBuffer!=NULL)
			ReleaseBuffer(pBuffer);
	}
	buffmap->clear();
}

void IOCPS::ReleaseSendBufferMap(BufferMap *buffmap)
{
	BufferMap::iterator iter = buffmap->begin();
	for(;iter!=buffmap->end();++iter) 
	{
		//unsigned int iKey;
		CIOCPBuffer *pBuffer=NULL;
		pBuffer = iter->second;
		
		if(pBuffer!=NULL)
		{
			if(pBuffer->m_Accept==IOCPS::SENDCLIENT)
			{
				m_SendClientMemPool.ReleaseObject(pBuffer);
			}
			else
			{
				m_SendNpcMemPool.ReleaseObject(pBuffer);
			}
		}
		
	}
	buffmap->clear();
}


/*
* Same as Disconnect Client but we does not try to 
* remove the context from the Context Map m_ContextMap.
*
*/

void IOCPS::AbortiveClose(ClientContext *mp)
{

	// If we have an active  socket close it. 
	if(mp->m_Socket>=0)
	{

		LINGER lingerStruct;
		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0;
		setsockopt( mp->m_Socket, SOL_SOCKET, SO_LINGER,
			(char *)&lingerStruct, sizeof(lingerStruct) );	
		
		CancelIo((HANDLE) mp->m_Socket);
		closesocket( mp->m_Socket );
		mp->m_Socket = INVALID_SOCKET;
	}

	// Move the Context to the free context list or kill it. 

	m_ContextMemPool.ReleaseObject(mp);

}


/*
* Closes the Server and frees the memory. 
*
*
* We are leaking some small amount of memory (OVERLAPPEDSTRUKTUR)
*
*
*/

// Error in Disconnect.. (some how we have to wait until the Competionport is finished with the data). 

void IOCPS::ShutDown()
{
	// CString msg;
	if(m_bServerStarted)
	{


		//AppendLog("Shutdown initialized.");

		m_bAcceptConnections=FALSE;

		//AppendLog("Sending shutdown signal to logical worker threads.");
		//ShutDownWorkers();

		// We Let the IOWorker Take care of the last packets and die. 
		//AppendLog("Disconnecting all the Connections...");
		DisconnectAll();



		//AppendLog("Sending shutdown signal to IO worker threads.");
		m_bShutDown=TRUE;
		ShutDownIOWorkers();
		
		ShutDownDispatchThread();

		ShutDownWriteThread();
		
		Sleep(800);
		
		CloseHandle(m_hCompletionPort);
		CloseHandle(m_hDispatchPort);
		CloseHandle(m_hWritePort);
		if(m_nPortNumber>0)
		{
			//AppendLog("Closing listner thread.");
			WSACloseEvent(m_hEvent);
			CloseHandle(m_hAcpEvent);
			closesocket(m_socListen);
			m_ContextMemPool.ReleaseObject(m_pListenContext);
		}
		//AppendLog("Deallocate memory used for Clients. ");
		FreeClientContext();
		//AppendLog("Deallocate memory used for Buffers. ");
		//FreeBuffers();

		//释放socket缓冲池
		list<SOCKET>::iterator iter;

		for(iter=m_FreeSocket.begin();iter!=m_FreeSocket.end();++iter)
		{
			closesocket(*iter);
		}
		m_FreeSocket.clear();

		m_bServerStarted=FALSE;
	}
}









/* 
OnZeroByteRead(ClientContext *pContext) the workaround 
the WSAENOBUFS error problem.  


This Bugg was a very difficult one.. When I stresstested this server code the
server hung after a while. I first thougt that this was a memleak problem or 
deadlock problem. But after a some hours I found that it is because of the system 
WSAENOBUFS error. 

With every overlapped send or receive operation, it is probable that 
the data buffers submitted will be locked. When memory is locked, it 
cannot be paged out of physical memory. The operating system imposes 
a limit on the amount of memory that may be locked. When this limit 
is reached, overlapped operations will fail with the WSAENOBUFS error. 
If a server posts many overlapped receives on each connection, this 
limit will be reached as the number of connections grow. If a server a
nticipates handling a very high number of concurrent clients, the server 
can post a single zero byte receive on each connection. Because there is 
no buffer associated with the receive operation, no memory needs to be 
locked. With this approach, the per-socket receive buffer should be left 
intact because once the zero-byte receive operation completes, the server 
can simply perform a non-blocking receive to retrieve all the data buffered 
in the socket's receive buffer. There is no more data pending when the 
non-blocking receive fails with WSAEWOULDBLOCK. This design would be for 
servers that require the maximum possible concurrent connections while 
sacrificing the data throughput on each connection.
Of course, the more you are aware of how the clients will be interacting 
with the server, the better. In the previous example, a non-blocking receive 
is performed once the zero-byte receive completes to retrieve the buffered 
data. If the server knows that clients send data in bursts, then once the 
zero-byte receive completes, it may post one or more overlapped receives 
in case the client sends a substantial amount of data 
(greater than the per-socket receive buffer that is 8 KB by default). */
//void IOCPS::OnZeroByteRead(ClientContext *pContext,CIOCPBuffer *pOverlapBuff)
//{
//
//	if(pContext)
//	{
//		// issue a Zeroread request 
//		DWORD dwIoSize=0;
//		ULONG			ulFlags = MSG_PARTIAL;
//		if(pOverlapBuff==NULL)
//		{
//			pOverlapBuff=AllocateBuffer(IOZeroReadCompleted);
//			if(pOverlapBuff==NULL)
//			{
//
////				CString msg;
////				msg.Format("FAILED TO Allocate buffer memory in OnZeroByteRead: %s",ErrorCode2Text(WSAGetLastError()));
////				AppendLog(msg);
//				DisconnectClient(pContext);
//				ReleaseClientContext(pContext);
//				return; 
//			}
//		}
//		pOverlapBuff->SetOperation(IOZeroReadCompleted);
//		pOverlapBuff->SetupZeroByteRead();
//
//
//		UINT nRetVal = WSARecv(pContext->m_Socket, 
//			pOverlapBuff->GetWSABuffer(),
//			1,
//			&dwIoSize, 
//			&ulFlags,
//			&pOverlapBuff->m_ol, 
//			NULL);
//
//		if ( nRetVal == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) 
//		{
//			if(WSAGetLastError()!=WSAENOTSOCK)
//			{
//#ifndef _DEBUG
//				// Remove Unnecessary disconnect messages in release mode.. 
//				if(WSAGetLastError()!=WSAECONNRESET&&WSAGetLastError()!=WSAECONNABORTED)
//#endif	
//				{
////					CString msg;
////					msg.Format("Disconnect in ZeroByteRead Possible Socket Error: %s",ErrorCode2Text(WSAGetLastError()));
////					AppendLog(msg);
//				}
//			}
//			ReleaseBuffer(pOverlapBuff);
//			//TRACE(">OnZeroByteRead(%x)\r\n",pContext);
//			DisconnectClient(pContext);			
//			ReleaseClientContext(pContext); //Later Implementation 
//		}
//	}
//
//}

//void IOCPS::OnZeroByteReadCompleted(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff)
//{
//
//	if(pContext)
//	{
//		// Make a Loop. 
//		AZeroByteRead(pContext,pOverlapBuff);	
//	}
//}


/*
* Makes a asynchrony Read by posting a IORead message into completion port 
* who invoces a Onread.
*
* The read is not made directly to distribute CPU power fairly between the connections. 
*/

//BOOL IOCPS::ARead(ClientContext *pContext,CIOCPBuffer *pOverlapBuff)
//{
//
//	if (pContext == NULL)
//		return FALSE;
//
//	OnRead(pContext,pOverlapBuff);
//
//	
//
////	if(pContext->m_Socket!=INVALID_SOCKET )
////	{
////		if(pOverlapBuff==NULL) 
////			pOverlapBuff=AllocateBuffer(IORead);
////		pOverlapBuff->SetOperation(IORead);
////		BOOL bSuccess = PostQueuedCompletionStatus(m_hCompletionPort, 0, (DWORD) pContext, &pOverlapBuff->m_ol);  	
////		if ( (!bSuccess && GetLastError( ) != ERROR_IO_PENDING))
////		{            
////			ReleaseBuffer(pOverlapBuff);
////			//TRACE(">ARead(%x)\r\n",pContext);
////			ReleaseClientContext(pContext);
////			return FALSE;
////		}
////
////	}else
////	{
////		//TRACE(">ARead2(%x)\r\n",pContext);
////		ReleaseBuffer(pOverlapBuff);
////		ReleaseClientContext(pContext);// Take care of it.
////		return FALSE;
////	}
//	return TRUE;
//}

/*
* A workaround the WSAENOBUFS error problem. (For more info please see OnZeroBytesRead  
* 
* Unlock the memory used by the OVELAPPED struktures. 
*
*/
//BOOL IOCPS::AZeroByteRead(ClientContext *pContext, CIOCPBuffer *pOverlapBuff)
//{
//	if (pContext == NULL)
//		return FALSE;
//
//	if(pContext->m_Socket!=INVALID_SOCKET )
//	{
//		if(pOverlapBuff==NULL) 
//			pOverlapBuff=AllocateBuffer(IOZeroByteRead);
//		pOverlapBuff->SetOperation(IOZeroByteRead);
//
//		BOOL bSuccess = PostQueuedCompletionStatus(m_hCompletionPort, 0, (DWORD) pContext, &pOverlapBuff->m_ol); 	
//		if ( (!bSuccess && GetLastError( ) != ERROR_IO_PENDING))
//		{            
//			ReleaseBuffer(pOverlapBuff);
//			DisconnectClient(pContext);
//			//TRACE(">OnZeroByteRead(%x)\r\n",pContext);
//			ReleaseClientContext(pContext);
////			CString msg;
////			msg.Format("Disconnect in AZeroByteRead Possible Socket Error: %s",ErrorCode2Text(WSAGetLastError()));
////			AppendLog(msg);
//			return FALSE;
//		}
//
//	}else
//	{
//		//TRACE(">OnZeroByteRead2(%x)\r\n",pContext);
//		ReleaseBuffer(pOverlapBuff);
//		ReleaseClientContext(pContext);// Take care of it.
//		return FALSE;
//	}
//	return TRUE;
//}




string IOCPS::ErrorCode2Text(DWORD dw)
{
	string error="";
	// Put your own common error text here (give more explaination etc..) 
	switch(dw)
	{
	case WSAEFAULT:
		error="WSAEFAULT	The buf parameter is not completely contained in a valid part of the user address space.";
		break; 
	case WSAENOTCONN:
		error="WSAENOTCONN	The socket is not connected."; 
		break;
	case WSAEINTR:
		error="WSAEINTR	The (blocking) call was canceled through WSACancelBlockingCall.	"; 
		break;
	case WSAENOTSOCK:
		error=" WSAENOTSOCK	The descriptor s is not a socket."; 
		break; 
	case WSANOTINITIALISED:
		error="WSANOTINITIALISED: A successful WSAStartup call must occur before using this function.";
		break; 
	case WSAENETDOWN:
		error="WSAENETDOWN	The network subsystem has failed."; 
		break;
	case WSAEINPROGRESS:
		error="WSAEINPROGRESS	A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."; 
		break;
	case WSAENETRESET:
		error=" WSAENETRESET	The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress."; 
		break; 
	case WSAEOPNOTSUPP:
		error="WSAEOPNOTSUPP	MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.	"; 
		break; 
	case WSAESHUTDOWN:
		error="WSAESHUTDOWN	The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH."; 
		break;
	case WSAEWOULDBLOCK:
		error=" WSAEWOULDBLOCK	The socket is marked as nonblocking and the receive operation would block.	"; 
		break; 
	case WSAEMSGSIZE:
		error=" WSAENOTSOCK		The message was too large to fit into the specified buffer and was truncated."; 
		break;
	case WSAEINVAL:
		error="WSAEINVAL	The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.	"; 
	case WSAECONNABORTED:
		error=" 	WSAECONNABORTED	The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable."; 
		break; 
	case WSAETIMEDOUT:
		error="WSAETIMEDOUT	The connection has been dropped because of a network failure or because the peer system failed to respond.	"; 
		break; 
	case WSAECONNRESET:
		//error="WSAECONNRESET	The virtual circuit was reset by the remote side executing a hard or abortive close."; 
		error="WSAECONNRESET Connection dropped..";
		break;

	default:
		error="";  
		break;
	}

	// Use system format..  
//	if(error.IsEmpty())
//	{
//		LPVOID lpMsgBuf;
//		error="";
//		FormatMessage(
//			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
//			FORMAT_MESSAGE_FROM_SYSTEM,
//			NULL,
//			dw,
//			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//			(LPTSTR) &lpMsgBuf,
//			0, NULL );
//		error.Format("%s",lpMsgBuf);
//		LocalFree(lpMsgBuf);
//	}
	return error;
}


/*
* Notifyes that this Client Context Strukture is currently in the 
* IOCompetetion lopp and are used by a another thread. 
* This funktion and ExitIOLoop is used to avoid possible Access Violation 
*/
void IOCPS::EnterIOLoop(ClientContext *pContext)
{
	if(pContext!=NULL)
	{
		EnterCriticalSection(&(pContext->m_ContextLock));
		pContext->m_nNumberOfPendlingIO++;
		LeaveCriticalSection(&(pContext->m_ContextLock));
	}
}

/*
* Notifies that the ClientContext is no longer in used by thread x, and 
* have been removed from the competition port. This function decreses the  
* m_nNumberOfPendlingIO and returns it. 
* 
* if it return zero (0) then it is safe to delete the structure from the heap. 
*/


int IOCPS::ExitIOLoop(ClientContext *pContext)
{
	int nNumberOfPendlingIO=0;
	if(pContext!=NULL)
	{
		EnterCriticalSection(&(pContext->m_ContextLock));
		pContext->m_nNumberOfPendlingIO--;
		nNumberOfPendlingIO=pContext->m_nNumberOfPendlingIO;
//#ifdef _DEBUG
//		if(nNumberOfPendlingIO<0)
//		{
//			TRACE("Strange nNumberOfPendlingIO ERROR\r\n");
//		}
//		if(pContext->m_Socket==INVALID_SOCKET)
//			TRACE("ExitIOLoop->nNumberOfPendlingIO %i, Socket: %i (%x)\r\n", pContext->m_nNumberOfPendlingIO,pContext->m_Socket,pContext);
//#endif
		LeaveCriticalSection(&(pContext->m_ContextLock));

	}
	return nNumberOfPendlingIO; 
}


/*
* Just fakes that the client side have closed the connection.. 
* We leave everyting to the IOWorkers to handle with Disconnectclient. 
*/
void IOCPS::DisconnectAll()
{
	EnterCriticalSection(&m_ContextMapLock);
	
	ContextMap::iterator iter;
	for(iter=m_ContextMap.begin();iter!=m_ContextMap.end();++iter) 
	{
		//unsigned int iKey;
		ClientContext *pContext=NULL;
		//m_ContextMap.GetNextAssoc (pos, iKey,pContext);
		pContext = iter->second;
		DisconnectClient(pContext);
	}
	LeaveCriticalSection(&m_ContextMapLock); 


}






/*
* Shuttingdown the IOWOrkers.. 
* 
* We put a NULL in CompletionPort and set the m_bShutDown FLAG.
* Then we wait for the IOWorkes to finish the works in the CompletionPort and exit. 
*
*/

void IOCPS::ShutDownWriteThread()
{
	PostQueuedCompletionStatus(m_hWritePort, 0, (DWORD) NULL, NULL);
}

void IOCPS::ShutDownDispatchThread()
{
	PostQueuedCompletionStatus(m_hDispatchPort, 0, (DWORD) NULL, NULL);
}

void IOCPS::ShutDownIOWorkers()
{
	
	m_bShutDown=TRUE;
	
	list<HANDLE>::iterator iter;
	
	for(iter=m_IOWorkerList.begin();iter!=m_IOWorkerList.end();++iter) 
	{
		PostQueuedCompletionStatus(m_hCompletionPort, 0, (DWORD) NULL, NULL);
		CloseHandle(*iter);
	}

	m_IOWorkerList.clear();
}






/*
* Sets the number of Workers (NOT IOWorkers that deals with Send/Receive  
*
*/


/*

BOOL IOCPS::SetWorkers(int nThreads)
{
	int iNumberToKill=0;
	int iNumberToStart=0;

	m_WorkerThreadMapLock.Lock();
	int iNumberOfThreads=m_WorkerThreadMap.GetCount();
	m_WorkerThreadMapLock.Unlock();

	if(nThreads<iNumberOfThreads)
		iNumberToKill=iNumberOfThreads-nThreads;
	else
		iNumberToStart=nThreads-iNumberOfThreads;

	// No interference while admin the threads. 
	BOOL bAcceptJobs=m_bAcceptJobs;
	m_bAcceptJobs=FALSE;





	//
	// if nThreads is bigger than our current thread count, remove all excess threads
	//

	//
	// Kill some of the workers. 
	// ej helt klart... 

	m_WorkerThreadMapLock.Lock();
	POSITION pos = m_WorkerThreadMap.GetStartPosition ();
	while (pos != NULL&&iNumberToKill>0) 
	{
		WORD strKey;
		CWinThread* pThread=NULL;
		m_WorkerThreadMap.GetNextAssoc (pos, strKey,(void *&)pThread);
		if(pThread)
		{

			HANDLE hThread = pThread->m_hThread;

			// notify the thread that it should die. 
			pThread->m_hThread=INVALID_HANDLE_VALUE;  
			// now let the thread terminate itself

			//::GetExitCodeThread(hThread, &dwExit) && (dwExit != 0xdead)

			::ResumeThread(hThread);


			DWORD dwExit = NULL;
			while(::GetExitCodeThread(hThread, &dwExit) && (dwExit != 0xdead))
			{
				::Sleep(50);	// give it a chance to finish
			}
			::CloseHandle (hThread);
			iNumberToKill--;
			m_WorkerThreadMap.RemoveKey(strKey);
			delete[] pThread;
		}	
	}
	m_WorkerThreadMapLock.Unlock();

	//
	// Start some Workers. 
	//
	m_WorkerThreadMapLock.Lock();
	while (iNumberToStart>0) 
	{
		CWinThread* pWorkerThread=AfxBeginThread(IOCPS::WorkerThreadProc, (void*)this,
			THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);  
		pWorkerThread->m_bAutoDelete=FALSE;  
		if(pWorkerThread)
		{
			pWorkerThread->ResumeThread();
			m_WorkerThreadMap[(WORD)pWorkerThread->m_nThreadID]=(void*)pWorkerThread;
			iNumberToStart--;
		}else
			return FALSE;
	}

	m_WorkerThreadMapLock.Unlock();
	m_bAcceptJobs=bAcceptJobs;
	return TRUE;
}

*/

/*

UINT IOCPS::WorkerThreadProc(LPVOID pParam)
{
	IOCPS* pPoolServer= reinterpret_cast<IOCPS*>(pParam);
	CWinThread* pThis=NULL;
	if(pPoolServer)
		pThis=pPoolServer->GetWorker((WORD)::GetCurrentThreadId());

	if(pThis)
	{


		TRACE("Thread %i is alive.\r\n",::GetCurrentThreadId());
		JobItem *pJob=NULL;
		while(pThis->m_hThread!=INVALID_HANDLE_VALUE)	
		{
			pJob=NULL;	
			pJob=pPoolServer->GetJob();
			if(pJob) 
			{
				pPoolServer->ProcessJob(pJob,pPoolServer);
				pPoolServer->FreeJob(pJob);
			}else
				::SuspendThread(::GetCurrentThread());
		}


	}
	TRACE("Thread %i RIP.\r\n",::GetCurrentThreadId());
	return 0xdead;
}

*/

/*
* Returns a Worker in the Worker Pool..
*
*/

/*
CWinThread* IOCPS::GetWorker(WORD WorkerID)
{
	CWinThread* pWorker=NULL;
	m_WorkerThreadMapLock.Lock();
	pWorker=(CWinThread*)m_WorkerThreadMap[WorkerID];
	m_WorkerThreadMapLock.Unlock();
	return pWorker;
}

*/


/*
* Closes all the logic Workers and emptys the workqueue. 
*
*
*/

/*
void IOCPS::ShutDownWorkers()
{
	//Close The Workers. 
	m_bAcceptJobs=FALSE;
	SetWorkers(0);
	m_WorkerThreadMapLock.Lock();
	m_WorkerThreadMap.RemoveAll();
	m_WorkerThreadMapLock.Unlock();

	// Empty the JobQueue. 

	JobItem *pJob=NULL;
	m_JobQueueLock.Lock();
	POSITION pos = m_JobQueueList.GetHeadPosition ();
	while (pos != NULL) 
	{
		pJob = (JobItem *)m_JobQueueList.GetNext (pos);
		FreeJob(pJob);
	}
	m_JobQueueList.RemoveAll();
	m_JobQueueLock.Unlock();

}

inline void IOCPS::FreeJob(JobItem *pJob)
{
	if(pJob)
		delete pJob;
}

*/
/*
* Adds a job to the jobqueue and wakes someone upp to do the job. 
*
*
*
*/
/*
BOOL IOCPS::AddJob(JobItem *pJob)
{
	BOOL bRetur=FALSE;
	if(m_bAcceptJobs&&pJob!=NULL)
	{
		m_JobQueueLock.Lock();
		bRetur= (m_JobQueueList.AddHead((void*)pJob)!=NULL);
		m_JobQueueLock.Unlock();
		//
		// Wake someone upp to do the job..
		if(bRetur)
		{
			m_WorkerThreadMapLock.Lock();
			POSITION pos = m_WorkerThreadMap.GetStartPosition ();
			while (pos != NULL) 
			{
				WORD strKey;
				CWinThread* pThread=NULL;
				m_WorkerThreadMap.GetNextAssoc (pos, strKey,(void *&)pThread);
				if(pThread)
				{
					if(pThread->ResumeThread()==1) // Some one wake up. 
						break;
				}	
			}
			m_WorkerThreadMapLock.Unlock();

		}
		return bRetur; 
	}
	return FALSE;
}
*/

/*
* Gets a Job from the queue. 
*
*/ 
/*
JobItem* IOCPS::GetJob()
{
	JobItem* pJob=NULL;
	m_JobQueueLock.Lock();
	if(!m_JobQueueList.IsEmpty())
		pJob=(JobItem*)m_JobQueueList.RemoveTail();
	m_JobQueueLock.Unlock();
	return pJob; 
}
*/

/*
* Virtual Function who Processes a Job. Used to do rare heavy computation or 
* calls that blocks the calling thread for a while. 
*/
/*
void IOCPS::ProcessJob(JobItem *pJob,IOCPS* pServer)
{
}
*/
/*
*	Called when a write is completed, this function is ofen used
*  for progressbars etc (e.g indicates how much is send in bytes) 
*/

void IOCPS::NotifyWriteCompleted(ClientContext *pContext, DWORD dwIoSize, CIOCPBuffer *pOverlapBuff)
{
	// The pOverlapBuff Buffer are Successfully sended. 
}

/*
*	An package is received.. 
*/

void IOCPS::NotifyReceivedPackage(CIOCPBuffer *pOverlapBuff,int nSize,ClientContext *pContext)
{
	m_pKernel->ProcessRecvMsg(pOverlapBuff,nSize,pContext);	
}



/*
* AllocateContext. Creates a context in the heap with new or reuse context 
* which is in the m_FreeContextList linked list. 
*
*/

ClientContext* IOCPS::AllocateContext()
{

	ClientContext* pContext = NULL;

	if ( !m_bShutDown )
	{

		pContext = m_ContextMemPool.GetObject();
		if(pContext)
		{
			pContext->m_ID=0;
			pContext->m_Socket=INVALID_SOCKET;
			pContext->m_nNumberOfPendlingIO=0;
			//pContext->m_SendSequenceNumber=pContext->m_ReadSequenceNumber=0;
			//pContext->m_CurrentSendSequenceNumber=pContext->m_CurrentReadSequenceNumber=0;
			pContext->m_pBuffOverlappedPackage=NULL;
			pContext->m_cEncryptRecv.Init();
			pContext->m_cEncryptSend.Init();
		}

	}
	return pContext;
}




/*
* Allocates an unique buffer for nType operation.(from m_FreeBufferList if possible) 
* The allocated buffer is placed in the m_BufferList. 
*
*/

CIOCPBuffer *IOCPS::AllocateBuffer(int nType)
{
	CIOCPBuffer *pBuff=NULL;
	
	pBuff = m_BuffMemPool.GetObject();
	if(pBuff)
	{
		pBuff->EmptyUsed();
		pBuff->SetOperation(nType);
		//pBuff->SetSequenceNumber(0);
	}
	
	return pBuff;
}


CIOCPBuffer* IOCPS::AllocateClientNpcBuffer(int nType,UINT AllocType)
{
	CIOCPBuffer *pBuff=NULL;
	
	if(AllocType==IOCPS::SENDCLIENT)
	{
		pBuff = m_SendClientMemPool.GetObject();	
	}
// 	else if(AllocType==IOCPS::SENDNPC)
// 	{
// 		pBuff = m_SendNpcMemPool.GetObject();
// 	}

	if(pBuff)
	{
		pBuff->EmptyUsed();
		pBuff->SetOperation(nType);
//		pBuff->SetSequenceNumber(0);
		pBuff->m_Accept = AllocType;
	}
	return pBuff;
}

BOOL IOCPS::ReleaseClientNpcBuffer(CIOCPBuffer* pBuff)
{
	if(pBuff==NULL)
		return FALSE;
	if(pBuff->m_Accept==IOCPS::SENDCLIENT)
	{
		m_SendClientMemPool.ReleaseObject(pBuff);
	}
// 	else if(pBuff->m_Accept==IOCPS::SENDNPC)
// 	{
// 		m_SendNpcMemPool.ReleaseObject(pBuff);
// 	}
	else
	{
		LOGERROR("ReleaseClientNpcBuffer Error!");
	}
	return TRUE;
}

/*
* ReleaseBuffer releases the buffer (put it into freebufferlist or just delete it). 
*
*/


BOOL IOCPS::ReleaseBuffer(CIOCPBuffer *pBuff)
{
	if(pBuff==NULL)
		return FALSE;
	
	m_BuffMemPool.ReleaseObject(pBuff);
	
	return TRUE;
}






/* 
* Releases the Client Context. (put it into freeClientContext or delete it)
*
*/

inline BOOL IOCPS::ReleaseClientContext(ClientContext *pContext)
{
	BOOL bRet=FALSE;	
	if(pContext!=NULL)
	{

		
		int nNumberOfPendlingIO=ExitIOLoop(pContext);

		// If no one else is using this pContext and we are the only owner. Delete it. 
		if(nNumberOfPendlingIO==0)
		{
			

			EnterCriticalSection(&(pContext->m_ContextLock));
			
			ReleaseBuffer(pContext->m_pBuffOverlappedPackage);


			//ReleaseBufferMap(&pContext->m_ReadBufferMap);
			//ReleaseSendBufferMap(&pContext->m_SendBufferMap);
			
			LeaveCriticalSection(&(pContext->m_ContextLock));

			m_ContextMemPool.ReleaseObject(pContext);

			return TRUE; 	
		}
	}
	return FALSE; 
}


CIOCPBuffer * IOCPS::SplitBuffer(CIOCPBuffer *pBuff, UINT nSize)
{
	CIOCPBuffer *pBuff2=NULL;
	pBuff2=AllocateBuffer(0);
	if(pBuff2==NULL)
		return NULL;
	//pBuff2->SetSequenceNumber(pBuff->GetSequenceNumber());


	if(!pBuff2->AddData(pBuff->GetBuffer(),nSize))
	{
		
		ReleaseBuffer(pBuff2);
		return NULL;
	}

	if(!pBuff->Flush(nSize))
	{
		
		ReleaseBuffer(pBuff2);
		return NULL;
	}

	return pBuff2;
}


/*
* Adds the nSize bytes from pFromBuff to pToBuff, and 
* removes the data from pFromBuff.  
*
*/


BOOL IOCPS::AddAndFlush(CIOCPBuffer *pFromBuff, CIOCPBuffer *pToBuff, UINT nSize)
{
	if(pFromBuff==NULL||pToBuff==NULL||nSize<=0)
		return FALSE;

	if(!pToBuff->AddData(pFromBuff->GetBuffer(),nSize))
	{
		return FALSE;
	}

	if(!pFromBuff->Flush(nSize))
	{
		return FALSE;
	}
	return TRUE;
}

/*
* Setups the listner..  
*
*/
BOOL IOCPS::SetupListner()
{

	m_socListen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);	
	if ( m_socListen == INVALID_SOCKET )
	{

		
		return FALSE;
	}

	DWORD dwResult;

	//
	//获取微软SOCKET扩展函数指针
	//
	int nResult = WSAIoctl( 
						m_socListen,
						SIO_GET_EXTENSION_FUNCTION_POINTER,
						&g_GUIDAcceptEx,
						sizeof(g_GUIDAcceptEx),
						&lpAcceptEx,
						sizeof(lpAcceptEx),
					   &dwResult,
						NULL,
						NULL
						);
	if(nResult==SOCKET_ERROR)
	{
		return FALSE;
	}

	// Event for handling Network IO
	m_hEvent = WSACreateEvent();
	m_hAcpEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	if ( m_hEvent == WSA_INVALID_EVENT )
	{	

		closesocket(m_socListen);
		return FALSE;
	}


	// The listener is ONLY interested in FD_ACCEPT
	// That is when a client connects to or IP/Port
	// Request async notification
	int nRet = WSAEventSelect(m_socListen,
		m_hEvent,
		FD_ACCEPT);

	if ( nRet == SOCKET_ERROR )
	{
		closesocket(m_socListen);
		return FALSE;
	}


	SOCKADDR_IN		saServer;		


	// Listen on our designated Port#
	saServer.sin_port = htons(m_nPortNumber);

	// Fill in the rest of the address structure
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;

	// bind our name to the socket
	nRet = bind(m_socListen, 
		(LPSOCKADDR)&saServer, 
		sizeof(struct sockaddr));

	if ( nRet == SOCKET_ERROR )
	{

//		CString msg;
//		msg.Format("bind() error : %s",ErrorCode2Text(WSAGetLastError()));
//		AppendLog(msg);
		closesocket(m_socListen);
		return FALSE;
	}

	// Set the socket to listen
	//nRet = listen(m_socListen, nConnections);
	nRet=listen(m_socListen, 5);
	if ( nRet == SOCKET_ERROR )
	{

//		CString msg;
//		msg.Format("listen() error: %s",ErrorCode2Text(WSAGetLastError()));
//		AppendLog(msg);
		closesocket(m_socListen);
		return FALSE;
	}

	//将监听socket和完成端口绑定
	m_pListenContext =  AllocateContext();
	m_pListenContext->m_Socket = m_socListen;
	AssociateSocketWithCompletionPort(m_socListen,m_hCompletionPort,(DWORD)m_pListenContext);
	
	m_bAcceptConnections=TRUE;
	DWORD ThreadID;
	m_ListenThreadHandle = CreateThread(NULL,0,ListnerThreadProc,(void*)this,0,&ThreadID);

//	m_pListenThread =
//		AfxBeginThread(IOCPS::ListnerThreadProc, (void*)this,
//		THREAD_PRIORITY_NORMAL);

	if ( !m_ListenThreadHandle )
	{

//		CString msg;
//		msg.Format("Error Could not start the ListnerThreadProc: %s",ErrorCode2Text(WSAGetLastError()));
//		AppendLog(msg);
		return FALSE;
	}
	return TRUE; 
}

void IOCPS::BatchCreateSocket(int ncounts)
{
	int i = 0;
	SOCKET AcceptSocket;
	for(;i<ncounts;++i)
	{
		AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if(AcceptSocket!=INVALID_SOCKET)
		{
			m_FreeSocket.push_front(AcceptSocket);
		}
	}
}

SOCKET IOCPS::AllocAcceptSocket()
{
	SOCKET sAccept = INVALID_SOCKET;

	if(!m_FreeSocket.empty())
	{
		sAccept = m_FreeSocket.front();
		m_FreeSocket.pop_front();
		return sAccept;
	}
	else
	{
		
		
		BatchCreateSocket(NEXTSOCKETNUM);

		if(!m_FreeSocket.empty())
		{
			sAccept = m_FreeSocket.front();
			m_FreeSocket.pop_front();
			return sAccept;
		}

		return INVALID_SOCKET;
	}
}

BOOL IOCPS::PostAcceptEx(SOCKET ListenSocket,int nNumSockets)
{
	
	int i = 0;

	while (i < nNumSockets)
	{
		//没必要设置临界区,该函数只被listen thread调用
		
		SOCKET AcceptSocket = AllocAcceptSocket();
		

		if (INVALID_SOCKET == AcceptSocket)
		{
			return FALSE;
		}
		
	
		CIOCPBuffer* pBuffer =  AllocateBuffer(IOAccept);
		pBuffer->m_Accept = AcceptSocket;
		pBuffer->SetupRead();
		
		DWORD dwBytes;
		BOOL bSuccess = lpAcceptEx(
			ListenSocket,
			pBuffer->m_Accept,
			pBuffer->GetBuffer(),
			pBuffer->GetWSABuffer()->len - ((sizeof(SOCKADDR_IN) + 16) * 2),
			sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16,
			&dwBytes,
			&(pBuffer->m_ol));
		
		if (FALSE == bSuccess)
		{
			int nResult = WSAGetLastError();
			if (nResult != ERROR_IO_PENDING)
			{
				
				closesocket(AcceptSocket);
				
				ReleaseBuffer(pBuffer);
				
				return FALSE;
			}
			
				
			
		} 
		i++;
	}

	return TRUE;
}

/*
* Creates the  Completion Port m_hCompletionPort used by 
* IO worker Threads. 
*/
BOOL IOCPS::CreateCompletionPort()
{
	// Create the completion port that will be used by all the workers
	// threads.
	m_hCompletionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if ( m_hCompletionPort == NULL ) 
	{	
		
		return FALSE;
	}

	//create the dispatch port

	m_hDispatchPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);

	//create the write port

	m_hWritePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	
	return TRUE;
}


/*
* Starts the IOWorkers. 
*
*/

BOOL IOCPS::SetupIOWorkers()
{
	//CWinThread* pWorkerThread=NULL;
	HANDLE hThreadHandle;
	DWORD ThreadID;
	for(int i=0; i<m_iMaxIOWorkers;i++)
	{

		hThreadHandle = CreateThread(NULL,0,IOWorkerThreadProc,(void*)this,0,&ThreadID);

//		pWorkerThread=AfxBeginThread(IOCPS::IOWorkerThreadProc, (void*)this,
//			THREAD_PRIORITY_NORMAL);
		if(hThreadHandle)
			m_IOWorkerList.push_front(hThreadHandle);
			//m_IOWorkerList.AddHead((void*)pWorkerThread);
		else
		{

//			CString msg;
//			msg.Format("Error Couldnot start worker: %s",ErrorCode2Text(WSAGetLastError()));
//			AppendLog(msg);

			return FALSE;
		}
	}
	m_nIOWorkers=m_IOWorkerList.size();
	return TRUE; 
}

BOOL IOCPS::SetupDispatch()
{
	HANDLE hThreadHandle;
	DWORD ThreadID;
	

	hThreadHandle = CreateThread(NULL,0,DispatchThreadProc,(void*)this,0,&ThreadID);
	if(hThreadHandle)
	{
		CloseHandle(hThreadHandle);
		return TRUE;
	}
	return FALSE;
}

BOOL IOCPS::SetupWriteProcess()
{
	HANDLE hThreadHandle;
	DWORD ThreadID;
	

	hThreadHandle = CreateThread(NULL,0,WriteThreadProc,(void*)this,0,&ThreadID);
	if(hThreadHandle)
	{
		CloseHandle(hThreadHandle);
		return TRUE;
	}
	return FALSE;
}

/*
* Returns the local IP Adress.. 
*/


string IOCPS::GetHostIP()
{
	string sRet="";
	hostent* thisHost;
	char* ip;
	thisHost = gethostbyname("");
	ip = inet_ntoa (*(struct in_addr *)*thisHost->h_addr_list);
	sRet=ip;
	return sRet; 
}

/*
* Return The number of Connections. 
*/
int IOCPS::GetNumberOfConnections()
{
	int iRet=0;
	EnterCriticalSection(&m_ContextMapLock);
	iRet=m_NumberOfActiveConnections;
	LeaveCriticalSection(&m_ContextMapLock);
	return iRet;
}

/*
* Connects to a IP Adress. 
*
*/
/*
BOOL IOCPS::Connect(const string &strIPAddr, int nPort)
{

	if (m_bShutDown)
		return FALSE; 

	SOCKADDR_IN	SockAddr;
	SOCKET		clientSocket=INVALID_SOCKET;
	int			nRet=-1;
	int			nLen=-1;

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientSocket == INVALID_SOCKET)
	{

//		CString msg;
//		msg.Format("Connect()->socket() Could not create  socket: %s",ErrorCode2Text(WSAGetLastError()));
//		AppendLog(msg);
		return FALSE;
	}
	// Clear the SockAddr. 
	memset(&SockAddr,0,sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = inet_addr(strIPAddr.c_str());
	SockAddr.sin_port = htons(nPort);
	nRet = connect(clientSocket,(sockaddr*)&SockAddr, sizeof(SockAddr));

	if (nRet == SOCKET_ERROR &&
		WSAGetLastError() != WSAEWOULDBLOCK)
	{
//		CString msg;
//		msg.Format("Connection Error: %s",ErrorCode2Text(WSAGetLastError()));
//		AppendLog(msg);
		closesocket(clientSocket);
		return FALSE;
	}else
		return AssociateIncomingClientWithContext(clientSocket,0);
}
*/

/*
* Used to avoid inorder packaging. 
* Returns The inorder Buffer or NULL if not processed. 
*
*/

//在该函数中增加发送序列号

//CIOCPBuffer* IOCPS::GetNextSendBuffer(ClientContext *pContext,CIOCPBuffer *pBuff)
//{
//	// We must have a ClientContext to begin with. 
//	if (pContext==NULL)
//		return NULL;
//
//	
//
//	CIOCPBuffer* pRetBuff=NULL;
//	// We have a buffer
//	if (pBuff!=NULL)
//	{
//		// Is the Buffer inorder ? 
//		unsigned int iBufferSequenceNumber=pBuff->GetSequenceNumber();
//		if (iBufferSequenceNumber==pContext->m_CurrentSendSequenceNumber)
//		{
//			//TRACE("GetNextSendBuffer()_1: m_SendSequenceNumber= %i, m_CurrentSendSequenceNumber=%i\r\n",pContext->m_SendSequenceNumber,pContext->m_CurrentSendSequenceNumber); 
//
//			pContext->m_CurrentSendSequenceNumber=(pContext->m_CurrentSendSequenceNumber+1)%MAXIMUMSEQUENSENUMBER;
//			// Unlock the Context Lock. 
//			
//			// return the Buffer to be processed. 
//			return pBuff;
//		}
//		//TRACE("OutOforder: SS: %i, CurrentS: %i, pkg: %i\r\n",pContext->m_SendSequenceNumber,pContext->m_CurrentSendSequenceNumber,iBufferSequenceNumber); 
//		//
//		// Check if we already have a such key. 
//		//
//		//pRetBuff=NULL;
//		//pRetBuff=(CIOCPBuffer*) pContext->m_SendBufferMap[iBufferSequenceNumber];
//		BufferMap::iterator iter =  pContext->m_SendBufferMap.find(iBufferSequenceNumber);
//		if(iter==pContext->m_SendBufferMap.end())
//		{
//			pContext->m_SendBufferMap[iBufferSequenceNumber]=pBuff;
//		}	
//	}
//
//	// return the Ordered Context. 
//	//pRetBuff=NULL;
//	BufferMap::iterator iter = pContext->m_SendBufferMap.find(pContext->m_CurrentSendSequenceNumber);
//	if(iter!=pContext->m_SendBufferMap.end())
//	{
//		pRetBuff = iter->second;
//		pContext->m_SendBufferMap.erase(/*pContext->m_CurrentSendSequenceNumber*/iter);
//
//		pContext->m_CurrentSendSequenceNumber=(pContext->m_CurrentSendSequenceNumber+1)%MAXIMUMSEQUENSENUMBER;
//		
//		//TRACE("GetNextSendBuffer()_2: m_SendSequenceNumber= %i, m_CurrentSendSequenceNumber=%i\r\n",pContext->m_SendSequenceNumber,pContext->m_CurrentSendSequenceNumber); 
//	}
//
//
//	return pRetBuff;
//}

// Sets the Send Sequence number to the Buffer.  

//void IOCPS::SetSendSequenceNumber(ClientContext *pContext,CIOCPBuffer *pBuff)
//{
//	if (pContext!=NULL&&pBuff!=NULL)
//	{
//		//没必要添加临界区，只有一个线程改写该变量
//		//EnterCriticalSection(&(pContext->m_ContextLock));
//		pBuff->SetSequenceNumber(pContext->m_SendSequenceNumber);
//		pContext->m_SendSequenceNumber=(pContext->m_SendSequenceNumber+1)%MAXIMUMSEQUENSENUMBER;
//		//TRACE("SetSendSequenceNumber(): m_SendSequenceNumber= %i, m_CurrentSendSequenceNumber=%i\r\n",pContext->m_SendSequenceNumber,pContext->m_CurrentSendSequenceNumber); 
//		//LeaveCriticalSection(&(pContext->m_ContextLock));
//	}
//}

/*
* Used to avoid inorder packaging. 
* Returns The inorder Buffer or NULL if not processed. 
* Same as GetReadBuffer
*/
//
//CIOCPBuffer * IOCPS::GetNextReadBuffer(ClientContext *pContext, CIOCPBuffer *pBuff)
//{
//	// We must have a ClientContext to begin with. 
//	if (pContext==NULL)
//		return NULL;
//	EnterCriticalSection(&(pContext->m_ContextLock));
//	CIOCPBuffer* pRetBuff=NULL;
//	// We have a buffer
//	if (pBuff!=NULL)
//	{
//
//		// Is the Buffer inorder ? 
//		unsigned int iBufferSequenceNumber=pBuff->GetSequenceNumber();
//		if (iBufferSequenceNumber==pContext->m_CurrentReadSequenceNumber)
//		{
//
//			//TRACE("GetNextReadBuffer()_1: m_ReadSequenceNumber= %i, m_CurrentReadSequenceNumber=%i\r\n",pContext->m_ReadSequenceNumber,pContext->m_CurrentReadSequenceNumber); 
//
//			// Unlock the Context Lock. 
//			//增加序列号
//			pContext->m_CurrentReadSequenceNumber=(pContext->m_CurrentReadSequenceNumber+1)%MAXIMUMSEQUENSENUMBER;
//			LeaveCriticalSection(&(pContext->m_ContextLock));
//			// return the Buffer to be processed. 
//			return pBuff;
//		}
//		//TRACE("OutOforderRead: SS: %i, CurrentS: %i, pkg: %i\r\n",pContext->m_ReadSequenceNumber,pContext->m_CurrentReadSequenceNumber,iBufferSequenceNumber); 
//		//
//		// Check if we already have a such key. 
//		//
//		//pRetBuff=NULL;
//		// pContext->m_ReadBufferMap.Lookup(iBufferSequenceNumber,pRetBuff)
//		BufferMap::iterator iter = pContext->m_ReadBufferMap.find(iBufferSequenceNumber);
//		//pRetBuff=pContext->m_ReadBufferMap[iBufferSequenceNumber];
//		if(iter==pContext->m_ReadBufferMap.end())
//		{
////			CString msg;
////			msg.Format("Dublicate Key! in ReadbufferMap");
////			AppendLog(msg);
//			//LeaveCriticalSection(&(pContext->m_ContextLock));
//			//return NULL;
//
//			pContext->m_ReadBufferMap[iBufferSequenceNumber]=pBuff;
//		}
//
//		//
//		// Add it to the Map. 
//		//
//
//		// What if this fail?
//		
////		pRetBuff=NULL;
////		pRetBuff=pContext->m_ReadBufferMap[iBufferSequenceNumber];
////		if(pRetBuff==NULL)
////		{
////			CString msg;
////			msg.Format("ERROR CAN NOT SAVE INTO BUFFER!!!");
////			AppendLog(msg);
////			pContext->m_ContextLock.Unlock();
////			return NULL;
////		}
//	}
//
//	// return the Ordered Buffer. 
//	pRetBuff=NULL;
//	BufferMap::iterator iter = pContext->m_ReadBufferMap.find(pContext->m_CurrentReadSequenceNumber);
//	if(iter!=pContext->m_ReadBufferMap.end())
//	{
//		pRetBuff = iter->second;
//		pContext->m_ReadBufferMap.erase(iter);
//		//增加序列号
//		pContext->m_CurrentReadSequenceNumber=(pContext->m_CurrentReadSequenceNumber+1)%MAXIMUMSEQUENSENUMBER;
//		//if(!pContext->m_ReadBufferMap.RemoveKey(pContext->m_CurrentReadSequenceNumber))
//		//	TRACE("FATAL ERROR pContext->m_ReadBufferMap.RemoveKey(pContext->m_CurrentReadSequenceNumber) FAILED\r\n");
//		// increase or reset the sequence number 
//		//if(pContext->m_CurrentReadSequenceNumber==pContext->m_ReadSequenceNumber-1)
//		//	pContext->m_CurrentReadSequenceNumber=pContext->m_ReadSequenceNumber=0;
//		//else
//		//	pContext->m_CurrentReadSequenceNumber++; // Increase
//		//TRACE("GetNextReadBuffer()_2: m_ReadSequenceNumber= %i, m_CurrentReadSequenceNumber=%i\r\n",pContext->m_ReadSequenceNumber,pContext->m_CurrentReadSequenceNumber); 
//	}
//
//	LeaveCriticalSection(&(pContext->m_ContextLock));
//	return pRetBuff;
//}

/*
* Sets the Sequence number to a Buffer and adds the sequence buffer. 
*/
//void IOCPS::MakeOrderdRead(ClientContext *pContext, CIOCPBuffer *pBuff)
//{
//	if (pContext!=NULL&&pBuff!=NULL)
//	{
//		
//		EnterCriticalSection(&(pContext->m_ContextLock));
//		pBuff->SetSequenceNumber(pContext->m_ReadSequenceNumber);
//		//TRACE("MakeOrderdRead> %i\r\n",pBuff->GetSequenceNumber());
//		DWORD dwIoSize=0;
//		ULONG ulFlags = MSG_PARTIAL;
//		UINT nRetVal = WSARecv(pContext->m_Socket, 
//			pBuff->GetWSABuffer(),
//			1,
//			&dwIoSize, 
//			&ulFlags,
//			&pBuff->m_ol, 
//			NULL);
//
//		if ( nRetVal == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) 
//		{
//
//			if(WSAGetLastError()!=WSAENOTSOCK)
//			{
////				CString msg;
////				msg.Format("Disconnect in Onread Possible Socket Error: %s",ErrorCode2Text(WSAGetLastError()));
////				AppendLog(msg);
//			}
//			//pContext->m_ContextLock.Unlock();
//			ReleaseBuffer(pBuff);
//			//TRACE(">MakeOrderdRead(%x)\r\n",pContext);
//			DisconnectClient(pContext);
//			ReleaseClientContext(pContext);
//		}else
//		{
//			pContext->m_ReadSequenceNumber=(pContext->m_ReadSequenceNumber+1)%MAXIMUMSEQUENSENUMBER;
//			
//		}
//
//		LeaveCriticalSection(&(pContext->m_ContextLock));
//	}
//}


/*
* Increase the Read Sequence Number. 
*
*/

//void IOCPS::IncreaseReadSequenceNumber(ClientContext *pContext)
//{
//	if (pContext!=NULL)
//	{
//		EnterCriticalSection(&(pContext->m_ContextLock));
//		// increase or reset the sequence number 
//		pContext->m_CurrentReadSequenceNumber=(pContext->m_CurrentReadSequenceNumber+1)%MAXIMUMSEQUENSENUMBER;
//		LeaveCriticalSection(&(pContext->m_ContextLock));
//	}
//}
//
//
//void IOCPS::IncreaseSendSequenceNumber(ClientContext *pContext)
//{
//	if (pContext!=NULL)
//	{
//		EnterCriticalSection(&(pContext->m_ContextLock));
//		// increase or reset the sequence number 
//		pContext->m_CurrentSendSequenceNumber=(pContext->m_CurrentSendSequenceNumber+1)%MAXIMUMSEQUENSENUMBER;
//		LeaveCriticalSection(&(pContext->m_ContextLock));
//	}
//}


void IOCPS::NotifyDisconnectedClient(ClientContext *pContext)
{

}

void IOCPS::NotifyNewConnection(ClientContext *pcontext)
{

}

void IOCPS::NotifyNewClientContext(ClientContext *pContext)
{
	// Add some init here 	
}

void IOCPS::NotifyContextRelease(ClientContext *pContext)
{
	// add some extra clean up here. 

	

}

void IOCPS::AppendLog(string msg)
{
}



BOOL IOCPS::IsStarted()
{
	return m_bServerStarted;
}



void IOCPS::NotifyFileCompleted(ClientContext *pcontext)
{
	// pcontext is locked here. 
//#ifdef TRANSFERFILEFUNCTIONALITY
//	TRACE("FILE FINISHED"); 
//	TRACE("%s> %d of %d.\r\n",pcontext->m_File.GetFileTitle(),pcontext->m_iFileBytes,pcontext->m_iMaxFileBytes);
//#endif
}


void IOCPS::ChangeCode(ClientContext* pClient,DWORD dwData)
{
	//ClientContext* pClient = FindClient(ClientID);
	pClient->ChangeCode(dwData);
}

void IOCPS::SetKernel(CSocketKernel* pKernel)
{
	m_pKernel = pKernel;
}


/*
When you are developing serverapplication you may whant to protect your server 
againsd SYN attacks.. 

The SYN flooding attack protection feature of TCP detects symptoms of SYN flooding and responds by reducing the time server spends on connection requests that it cannot acknowledge.

Specifically, TCP shortens the required interval between SYN-ACK (connection request acknowledgements) retransmissions. (TCP retransmits SYN-ACKS when they are not answered.) As a result, the allotted number of retransmissions is consumed quicker and the unacknowledgeable connection request is discarded faster.

The SYN attack protection is obtained by setting some values in the windows registery before you 
start the server. Using this windows XP and Windows NT own protection is easy. 

The registry key "SynAttackProtect" causes Transmission Control Protocol (TCP) to 
adjust retransmission of SYN-ACKS. When you configure this value, the connection 
responses time out more quickly in the event of a SYN attack 
(a type of denial of service attack).

Below you can see the default values. 

0 (default) - typical protection against SYN attacks 
1 - better protection against SYN attacks that uses the advanced values below. 
2 (recommended) - best protection against SYN attacks. 


TcpMaxHalfOpen - default value is "100" 
Determines how many connections the server can maintain in the half-open (SYN-RCVD) state before TCP/IP initiates SYN flooding attack protection.
TcpMaxHalfOpenRetried - default value is "80" 
Determines how many connections the server can maintain in the half-open (SYN-RCVD) state even after a connection request has been retransmitted. If the number of exceeds the value of this entry, TCP/IP initiates SYN flooding attack protection.

TcpMaxPortsExhausted - default value is "5" 
Determines how many connection requests the system can refuse before TCP/IP initiates SYN flooding attack protection. The system must refuse all connection requests when its reserve of open connection ports runs out.

TcpMaxConnectResponseRetransmissions - default value is "3"

Determines how many times TCP retransmits an unanswered SYN-ACK (connection request acknowledgment). TCP retransmits acknowledgments until they are answered or until this value expires. This entry is designed to minimize the effect of denial-of-service attacks (also known as SYN flooding) on the server.


The Function below manipulates these values, restart of the machine is needed to, maintain the effects!
*/

/*
BOOL IOCPS::XPNTSYNFloodProtection(int iValue, int iTcpMaxHalfOpen, int iTcpMaxHalfOpenRetried,int iTcpMaxPortsExhausted,int iTcpMaxConnectResponseRetransmissions)
{
	CString sKey_PATH="\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters";
	CString sKey_SynAttackProtect="SynAttackProtect";
	CString sKey_TcpMaxHalfOpen="TcpMaxHalfOpen";
	CString sKey_TcpMaxHalfOpenRetried="TcpMaxHalfOpenRetried";
	CString sKey_TcpMaxPortsExhausted="TcpMaxPortsExhausted";
	CString sKey_TcpMaxConnectResponseRetransmissions="TcpMaxConnectResponseRetransmissions";


	HKEY    hKey;
	DWORD   val=0;
	LONG	r=0;
	BOOL bRet=TRUE;

	//
	// Set the sKey_SynAttackProtect
	//
	val=iValue;
	if (RegOpenKey(HKEY_LOCAL_MACHINE, sKey_PATH, &hKey) != ERROR_SUCCESS)
		if (RegCreateKey(HKEY_LOCAL_MACHINE, sKey_SynAttackProtect, &hKey) != ERROR_SUCCESS)
			return FALSE;
	r = RegSetValueEx(hKey, sKey_SynAttackProtect, 0, REG_DWORD, (BYTE *)&val, sizeof(val));
	RegCloseKey(hKey);
	bRet&= (r == ERROR_SUCCESS);

	//
	// Special Parameters is used.
	//
	if(iValue==1)
	{
		//
		// Set the sKey_SynAttackProtect
		//
		val=iTcpMaxHalfOpenRetried;
		if (RegOpenKey(HKEY_LOCAL_MACHINE, sKey_PATH, &hKey) != ERROR_SUCCESS)
			if (RegCreateKey(HKEY_LOCAL_MACHINE, sKey_TcpMaxHalfOpen, &hKey) != ERROR_SUCCESS)
				return FALSE;
		r = RegSetValueEx(hKey, sKey_TcpMaxHalfOpen, 0, REG_DWORD, (BYTE *)&val, sizeof(val));
		RegCloseKey(hKey);
		bRet&= (r == ERROR_SUCCESS);

		//
		// Set the sKey_TcpMaxHalfOpenRetried
		//
		val=iTcpMaxHalfOpenRetried;
		if (RegOpenKey(HKEY_LOCAL_MACHINE, sKey_PATH, &hKey) != ERROR_SUCCESS)
			if (RegCreateKey(HKEY_LOCAL_MACHINE, sKey_TcpMaxHalfOpenRetried, &hKey) != ERROR_SUCCESS)
				return FALSE;
		r = RegSetValueEx(hKey, sKey_TcpMaxHalfOpenRetried, 0, REG_DWORD, (BYTE *)&val, sizeof(val));
		RegCloseKey(hKey);
		bRet&= (r == ERROR_SUCCESS);


		//
		// Set the sKey_TcpMaxPortsExhausted
		//
		val=iTcpMaxPortsExhausted;
		if (RegOpenKey(HKEY_LOCAL_MACHINE, sKey_PATH, &hKey) != ERROR_SUCCESS)
			if (RegCreateKey(HKEY_LOCAL_MACHINE, sKey_TcpMaxPortsExhausted, &hKey) != ERROR_SUCCESS)
				return FALSE;
		r = RegSetValueEx(hKey, sKey_TcpMaxPortsExhausted, 0, REG_DWORD, (BYTE *)&val, sizeof(val));
		RegCloseKey(hKey);
		bRet&= (r == ERROR_SUCCESS);


		//
		// Set sKey_TcpMaxConnectResponseRetransmissions
		//
		val=iTcpMaxConnectResponseRetransmissions;
		if (RegOpenKey(HKEY_LOCAL_MACHINE, sKey_PATH, &hKey) != ERROR_SUCCESS)
			if (RegCreateKey(HKEY_LOCAL_MACHINE, sKey_TcpMaxConnectResponseRetransmissions, &hKey) != ERROR_SUCCESS)
				return FALSE;
		r = RegSetValueEx(hKey, sKey_TcpMaxConnectResponseRetransmissions, 0, REG_DWORD, (BYTE *)&val, sizeof(val));
		RegCloseKey(hKey);
		bRet&= (r == ERROR_SUCCESS);

	}

	return bRet;

}


*/


//void IOCPS::PreAllocBuffer(int nNum,list<CIOCPBuffer*>& FreeBufferList,CIOCPMemPool<CIOCPBuffer>& IOCPMemPool)
//{
//	CIOCPBuffer* p = NULL;
//
//	for(int i=0;i<nNum;++i)
//	{
//		
//		p = IOCPMemPool.GetObject();
//		if(p)
//			FreeBufferList.push_back(p);
//
//	}
//	
//}
//
//
//void IOCPS::PreAllocContext(int nNum)
//{
//	
//	
//	ClientContext* pContext =NULL;
//
//	
//
//	//EnterCriticalSection(&m_FreeContextListLock);
//
//	
//	
//	for(int i=0;i<nNum;++i)
//	{
//		pContext = m_ContextMemPool.GetObject();
//		if(pContext)
//			m_FreeContextList.push_back(pContext);
//		
//	}
//	
//	//LeaveCriticalSection(&m_FreeContextListLock);
//
//}