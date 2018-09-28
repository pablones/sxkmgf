#include <winsock2.h>
#include <assert.h>
#include "IoHandler.h"
#include "Acceptor.h"
//#include "Connector.h"
#include "SessionPool.h"
#include "NetBase.h"
#include "Session.h"
#include "SendBuffer.h"
#include "RecvBuffer.h"
#include "IOCPServer.h"
//#include "NetworkObject.h"
#include "../share/define.h"

//unsigned __stdcall update_thread(LPVOID param)
//{
//	IoHandler* pIoHandler = (IoHandler*)param;
//
//	while(!(pIoHandler->m_bShutdown))
//	{
//		pIoHandler->Update();
//
//		Sleep(10);
//	}
//
//	return 0;
//}

//=============================================================================================================================
/**
	@remarks
			I/O completion�� �����ϰ� ó���ϴ� ������
	@param	param
			IOCP ������ ������
*/
//=============================================================================================================================
unsigned __stdcall io_thread( LPVOID param )
{
	IoHandler *pIoHandler = (IoHandler*)param;

	BOOL			bSuccess = FALSE;
	DWORD			dwIoSize = 0;

	Session			*pSession = NULL;
	OVERLAPPEDEX	*pOverlappedEx = NULL;

	while( 1 )
	{
		DEBUG_TRY
		bSuccess = GetQueuedCompletionStatus( pIoHandler->m_hIOCP, &dwIoSize, (LPDWORD)&pSession,
											  (LPOVERLAPPED*)&pOverlappedEx, INFINITE );

		// �������� ���� ���� ����� ������ ���
		if( pSession == NULL ) 
			break;

		// GQCS�� ������ ��� Ȥ�� ���� ����
		if( !bSuccess )
		{
			if( GetLastError() == ERROR_NETNAME_DELETED )
			{
				// �̹� ������ ���� ��� ����
				//printf("[REMOVE][%d] GetLastError() == ERROR_NETNAME_DELETED\n", (int)pSession->GetSocket());
				
				//pSession->Remove();
			}
			pSession->ReleaseOverlappedNum();
			pSession->Remove();
			//LOGERROR("GetLastError=%d,ClientID=%d,SocketID=%d",GetLastError(),pSession->GetClientID(),pSession->GetSocket());
			continue;
		}

		// DISCONNECT_POSTED�� �ƴѵ��� IoSize�� 0�� ��� ���� ����
		if( pOverlappedEx->dwOperationType != DISCONNECT_POSTED && dwIoSize == 0 )
		{
			//printf("[REMOVE][%d] dwIoSize == 0\n", (int)pSession->GetSocket());
			pSession->ReleaseOverlappedNum();
			pSession->Remove();
			continue;
		}

		// Completion ó��
		switch( pOverlappedEx->dwOperationType )
		{
		case SEND_POSTED:
			DEBUG_TRY
			pSession->ReleaseOverlappedNum();
 			if(pSession->ShouldBeRemoved())
				break;
			
			pSession->GetSendBuffer()->Completion( dwIoSize );
			DEBUG_CATCH("SEND_POSTED")
			break;

		case RECV_POSTED:
			DEBUG_TRY
			pSession->ReleaseOverlappedNum();
 			if(pSession->ShouldBeRemoved())
 				break;
			//�Խ��ܵ������ݽ��н���
			
//			pSession->EncyRecvData(dwIoSize);
			pSession->GetRecvBuffer()->Completion( dwIoSize );
			// ���� Recv	
			if( !pSession->PreRecv(2) )	
			{
				//printf("[REMOVE][%d] pSession->PreRecv()\n", (int)pSession->GetSocket());
				pSession->ReleaseOverlappedNum();
				pSession->Remove();
			}
			DEBUG_CATCH("RECV_POSTED")
			break;

		case DISCONNECT_POSTED:
			DEBUG_TRY
			pSession->PreAccept( pIoHandler->m_pAcceptor->GetListenSocket() );
			DEBUG_CATCH("DISCONNECT_POSTED")
			break;
		}
		DEBUG_CATCH("io_thread")
	}

	return 0;
}

IoHandler::IoHandler()
{
	m_pAcceptSessionPool		= NULL;
	//m_pConnectSessionPool		= NULL;
	m_pAcceptor					= NULL;
	//m_pConnector				= NULL;
	m_pActiveSessionList		= NULL;
	m_pAcceptedSessionList		= NULL;
	m_pReuseSessionList			= NULL;
	m_pDeadSessionList			= NULL;
	//m_pConnectSuccessList		= NULL;
	//m_pConnectFailList			= NULL;
	m_pTempList					= NULL;
	m_numActiveSessions			= 0;
	m_bShutdown					= FALSE;
	for( DWORD i = 0; i < MAX_IO_WORKER_THREAD; ++i ) m_hIoThread[i] = NULL;

	// m_hUpdateThread = NULL;
	//m_fnCreateAcceptedObject	= NULL;
	//m_fnDestroyAcceptedObject	= NULL;
	//m_fnDestroyConnectedObject	= NULL;
}

IoHandler::~IoHandler()
{
	if( !m_bShutdown ) Shutdown();

	for( DWORD i = 0; i < m_numIoThreads; ++i )
	{
		if( m_hIoThread[i] ) CloseHandle( m_hIoThread[i] );
	}

//	if(m_hUpdateThread)
//		CloseHandle(m_hUpdateThread);

	if( m_pAcceptSessionPool )		delete m_pAcceptSessionPool;
	//if( m_pConnectSessionPool )		delete m_pConnectSessionPool;
	if( m_pAcceptor )				delete m_pAcceptor;
	//if( m_pConnector )				delete m_pConnector;
	if( m_pActiveSessionList )		delete m_pActiveSessionList;
	if( m_pAcceptedSessionList )	delete m_pAcceptedSessionList;
	if( m_pReuseSessionList )		delete m_pReuseSessionList;
	//if( m_pConnectSuccessList )		delete m_pConnectSuccessList;
	//if( m_pConnectFailList )		delete m_pConnectFailList;
	if( m_pTempList )				delete m_pTempList;
	if(m_pDeadSessionList)			delete m_pDeadSessionList;
}

VOID IoHandler::Init( IOCPServer *pIOCPServer, LPIOHANDLER_DESC lpDesc )
{
	m_pIOCPServer	= pIOCPServer;

	// �ݹ� �Լ� ������ ��ȿ�� �˻�
//	assert( !IsBadReadPtr( lpDesc->fnCreateAcceptedObject, sizeof(lpDesc->fnCreateAcceptedObject) ) );
//	assert( !IsBadReadPtr( lpDesc->fnDestroyAcceptedObject, sizeof(lpDesc->fnDestroyAcceptedObject) ) );
//	assert( !IsBadReadPtr( lpDesc->fnDestroyConnectedObject, sizeof(lpDesc->fnDestroyConnectedObject) ) );
//	
	// �ݹ� �Լ� ������ ����
//	m_fnCreateAcceptedObject		= lpDesc->fnCreateAcceptedObject;
//	m_fnDestroyAcceptedObject		= lpDesc->fnDestroyAcceptedObject;
//	m_fnDestroyConnectedObject		= lpDesc->fnDestroyConnectedObject;

	m_dwKey				= lpDesc->dwIoHandlerKey;
	
	// ���� ����Ʈ ����
	m_pActiveSessionList	= new SessionList;
	m_pAcceptedSessionList	= new SessionList;
	m_pReuseSessionList		= new SessionList;
	//m_pConnectSuccessList	= new SessionList;
	//m_pConnectFailList		= new SessionList;
	m_pTempList				= new SessionList;
	m_pDeadSessionList		= new SessionList;

	// �ִ� ���� ���� �ο�
	m_dwMaxAcceptSession	= lpDesc->dwMaxAcceptSession;

	// ���� Ǯ ����
	m_pAcceptSessionPool	= new SessionPool( lpDesc->dwMaxAcceptSession,
											   lpDesc->dwSendBufferSize,
											   lpDesc->dwRecvBufferSize,
											   lpDesc->dwMaxPacketSize,
											   /*lpDesc->dwTimeOut,*/
											   1,
											   TRUE );

//	m_pConnectSessionPool	= new SessionPool( lpDesc->dwMaxConnectSession,
//											   lpDesc->dwSendBufferSize,
//											   lpDesc->dwRecvBufferSize,
//											   lpDesc->dwMaxPacketSize,
//											   lpDesc->dwTimeOut,
//											   m_pAcceptSessionPool->GetLength() + 1,
//											   FALSE );

	
	// �ִ� ��Ŷ ũ�� ����
	//assert( lpDesc->dwMaxPacketSize > sizeof(PACKET_HEADER) );
	m_dwMaxPacketSize =	lpDesc->dwMaxPacketSize;
  
	// IOCP ����
	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	unsigned threadID;

	// IO ��Ŀ ������ ����
	assert( lpDesc->dwNumberOfIoThreads <= MAX_IO_WORKER_THREAD );
	m_numIoThreads = lpDesc->dwNumberOfIoThreads;
	for( DWORD i = 0; i < m_numIoThreads; ++i )
	{
		m_hIoThread[i] = (HANDLE)_beginthreadex( NULL, 0, io_thread, (LPVOID)this, 0, &threadID );
	}


// 	m_hUpdateThread = (HANDLE)_beginthreadex( NULL, 0, update_thread, (LPVOID)this, 0, &threadID );

	// ����� ����
	m_pAcceptor		= new Acceptor;
	m_pAcceptor->Init( this, lpDesc->dwNumberOfAcceptThreads );

	// Ŀ���� ����	
	//m_pConnector	= new Connector;
	//m_pConnector->Init( this, lpDesc->dwNumberOfConnectThreads );
}

//DWORD IoHandler::Connect( NetworkObject *pNetworkObject, char *pszIP, WORD wPort )
//{
//	// �ش� NetworkObject�� ������ �̹� Ŀ��Ʈ �õ����̸� �����Ѵ�.
//	if( pNetworkObject->m_pSession != NULL ) return 0;
//
//	// ���� ����
//	SOCKET socket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
//
//	// �ּ� ����
//	SOCKADDR_IN addr;
//
//	addr.sin_family			= AF_INET;
//	addr.sin_addr.s_addr	= inet_addr( pszIP );
//	addr.sin_port			= htons( wPort );
//
//	// ���� �Ҵ�
//	Session *pSession = AllocConnectSession();
//	assert( pSession != NULL && "������ ���ڶ� Connect �� �� �����ϴ�. dwMaxConnectSession ������ Ȯ�����ּ���" );
//
//	// ���ǿ� ���� �� �ּ� ����
//	pSession->SetSocket( socket );
//	pSession->SetSockAddr( addr );
//
//	// ������ ��Ʈ�� ������Ʈ�� ���ε�
//	assert( pNetworkObject != NULL );
//	pSession->BindNetworkObject( pNetworkObject );
//
//	m_pConnector->Connect( pSession );
//
//	return pSession->GetIndex();
//}

//=============================================================================================================================
/**
	@remarks
			���� ������ �����ϰ� ������ �����Ѵ�.
	@retval	BOOL
			���� ���� ������ �����ϸ� FALSE�� �����Ѵ�.
	@param	pIP
			������ ����� IP
	@param	wPort
			������ ����� ��Ʈ
*/
//=============================================================================================================================
BOOL IoHandler::StartListen( char *pIP, WORD wPort )
{
	assert( m_dwMaxAcceptSession > 0 );

	// �̹� �������̶�� ���� ó��
	if( IsListening() ) return TRUE;

	if( !m_pAcceptor->StartListen( pIP, wPort ) )
	{
		//printf( "Listen socket creation failed.\n" );
		return FALSE;
	}

	Session *pSession;
	while( pSession = AllocAcceptSession() )
	{
		pSession->PreAccept( m_pAcceptor->GetListenSocket() );
		m_sessionListHolder.push_back(pSession);///���Ự���������Ա�����ͷŵ�
	}

	return TRUE;
}

BOOL IoHandler::IsListening()
{
	return m_pAcceptor->IsListening();
}

//=============================================================================================================================
/**
	@remarks
			������̴� ���ǵ��� ��� ����Ǯ�� ��ȯ�Ѵ�.
*/
//=============================================================================================================================

Session* IoHandler::AllocAcceptSession()
{
	return m_pAcceptSessionPool->Alloc();
}

//Session* IoHandler::AllocConnectSession()
//{
//	return m_pConnectSessionPool->Alloc();
//}

VOID IoHandler::FreeSession( Session *pSession )
{
	int nStep = 0;
	DEBUG_TRY
	//printf("[FreeSession][%d]\n", (int)pSession->GetSocket());
	pSession->CloseSocket();
	nStep = 1;
	pSession->Init();
	nStep = 2;
	if( pSession->IsAcceptSocket() )
	{
		nStep = 3;
		m_pAcceptSessionPool->Free( pSession );
	}
	DEBUG_CATCH("FreeSession nStep =%d",nStep)
//	else
//	{
//		m_pConnectSessionPool->Free( pSession );
//	}
}

VOID IoHandler::ReuseSession( Session *pSession )
{
	//printf("[ReuseSession][%d]\n", (int)pSession->GetSocket());
	pSession->Reuse( m_hIOCP );
}

//=============================================================================================================================
/**
	@remarks
			���ӿ� ������ ���� ����Ʈ ó��
	@par
			ConnectSuccessList�� �ִ� ���ǵ��� IOCP�� ����ϰ� OnConnect(TRUE)�� ȣ���ϰ� ActiveSessionList�� �߰��Ѵ�.
*/
//=============================================================================================================================
//VOID IoHandler::ProcessConnectSuccessList()
//{
//	SESSION_LIST_ITER		it;
//	Session					*pSession;
//
//	// Ŀ��Ʈ�� ������ ���ǵ��� �ӽ� ����Ʈ�� �ű�
//	m_pConnectSuccessList->Lock();
//	m_pTempList->splice( m_pTempList->end(), *m_pConnectSuccessList );
//	m_pConnectSuccessList->Unlock();
//
//	for( it = m_pTempList->begin(); it != m_pTempList->end(); ++it )
//	{
//		pSession = *it;
//
//		// IOCP�� �ڵ� ���
//		CreateIoCompletionPort( (HANDLE)pSession->GetSocket(), m_hIOCP, (ULONG_PTR)pSession, 0 );
//
//		if( pSession->PreRecv() )
//		{
//			pSession->OnConnect( TRUE );
//		}
//		else
//		{
//			m_pTempList->erase( it-- );
//
//			// ���� Ǯ�� ��ȯ
//			FreeSession( pSession );
//
//			pSession->OnConnect( FALSE );
//		}
//	}
//
//	if( !m_pTempList->empty() )
//	{
//		// ������ ����
//		m_numActiveSessions += (DWORD)m_pTempList->size();
//
//		// Ŀ��Ʈ�� ������ ���ǵ��� ActiveSessionList�� �߰��Ѵ�.
//		m_pActiveSessionList->Lock();
//		m_pActiveSessionList->splice( m_pActiveSessionList->end(), *m_pTempList );
//		m_pActiveSessionList->Unlock();
//	}
//}

//=============================================================================================================================
/**
	@remarks
			���ӿ� ������ ���� ����Ʈ ó��
	@par
			ConnectFailList�� �ִ� ���ǵ鿡 ���� OnConnect(FALSE)�� ȣ���Ѵ�.
*/
//=============================================================================================================================
//VOID IoHandler::ProcessConnectFailList()
//{
//	SESSION_LIST_ITER		it;
//	Session					*pSession;
//
//	m_pConnectFailList->Lock();
//
//	for( it = m_pConnectFailList->begin(); it != m_pConnectFailList->end(); ++it )
//	{
//		pSession = *it;
//
//		// ���� Ǯ�� ��ȯ
//		FreeSession( pSession );
//
//		pSession->OnConnect( FALSE );
//	}
//
//	m_pConnectFailList->clear();
//	m_pConnectFailList->Unlock();
//}

//=============================================================================================================================
/**
	@remarks
			TempSessionList�� �ִ� ���ǵ��� IOCP�� ����ϰ� ActiveSessionList�� �ű�� OnAccpet()�� ȣ���Ѵ�.
*/
//=============================================================================================================================
VOID IoHandler::ProcessAcceptedSessionList()
{
	SESSION_LIST_ITER		it;
	Session					*pSession;

	// ���ӿ� ������ ���ǵ��� �޾Ƶ� �ӽ� ����Ʈ�� �ű�
	m_pAcceptedSessionList->Lock();
	m_pTempList->splice( m_pTempList->end(), *m_pAcceptedSessionList );
	m_pAcceptedSessionList->Unlock();

	// ���ӿ� ������ ���ǿ� ���� ó��
	for( it = m_pTempList->begin(); it != m_pTempList->end(); ++it )
	{
		pSession = *it;

		// �ְ������� �ʰ��ϴ� ��� ����
		if( m_numActiveSessions >= m_dwMaxAcceptSession )
		{
			LOGWARNING("connection full! m_numActiveSessions = %u,m_dwMaxAcceptSession = %u",m_numActiveSessions,m_dwMaxAcceptSession);
			//printf( "connection full! no available accept socket!\n" );
			m_pTempList->erase( it-- );
			ReuseSession( pSession );
			//pSession->CloseSocket();
			m_pReuseSessionList->push_back(pSession);
			continue;
		}

		// IOCP�� �ڵ� ���
		CreateIoCompletionPort( (HANDLE)pSession->GetSocket(), m_hIOCP, (ULONG_PTR)pSession, 0 );

		pSession->SetIoHandler(this);
		pSession->SetClientID(m_pIOCPServer->SetNewClientSocket(pSession));

		// Recv�� �����ϴ� ��� ó��
		if( !pSession->PreRecv(1) )
		{
			m_pTempList->erase( it-- );
			//pSession->CloseSocket();
			ReuseSession(pSession);
			m_pReuseSessionList->push_back(pSession);
			//ReuseSession( pSession );
			continue;
		}

		//--------------------------------
		// ���������� ���ӵ� ���� ó��
		//--------------------------------

		// ��Ʈ�� ������Ʈ ���� ��û
		//NetworkObject *pNetworkObject = m_fnCreateAcceptedObject();
		//assert( pNetworkObject );

		// ��Ʈ�� ������Ʈ ���ε�
		//pSession->BindNetworkObject( pNetworkObject );

		// ���ӽ� �ʱ�ȭ �� NetworkObject�� ���� ����
		pSession->OnAccept();

		// ������ ����
		++m_numActiveSessions;
	}

	if( !m_pTempList->empty() )
	{
		// ���ӿ� ������ ���ǵ��� ActiveSessionList�� �߰�
		//m_pActiveSessionList->Lock();
		m_pActiveSessionList->splice( m_pActiveSessionList->begin(), *m_pTempList );
		//m_pActiveSessionList->Unlock();
	}
}

//=============================================================================================================================
/**
	@remarks
			���� accept �Ǵ� connect�Ǿ� Ȱ��ȭ�� ���ǵ��� ��ȸ�ϸ� ���� ��Ŷ ���� ó���Ѵ�.
*/
//=============================================================================================================================
VOID IoHandler::ProcessActiveSessionList()
{
	SESSION_LIST_ITER	it;
	Session				*pSession;

	for( it = m_pActiveSessionList->begin(); it != m_pActiveSessionList->end(); ++it )
	{
		int nStep = 0;
		char szTemp[24] = "";
		DEBUG_TRY
		pSession = *it;

		strcpy(szTemp,pSession->GetIP());
		nStep = 1;
		// if( pSession->ShouldBeRemoved() ) continue;

		if( pSession->ShouldBeRemoved() )
		{
			//m_pActiveSessionList->erase( it-- );//������� xingbarking
			m_pDeadSessionList->push_back( pSession );
			continue;
		}
		nStep = 2;
		//������
		if( pSession->PreSend() == FALSE ) 
		{
			//printf("[REMOVE][%d] pSession->PreSend() == FALSE\n", (int)pSession->GetSocket());
			nStep = 21;
			pSession->Remove();
		}

		nStep = 3;
		// �ޱ� ���� ó��
		if( !pSession->ProcessRecvdPacket( m_dwMaxPacketSize ) )
		{
			pSession->Remove();
		}

		}catch(...)
		{
			pSession->Remove();
			LOGWARNING("ProcessActiveSessionList step = %u ip = '%s'",nStep,szTemp);
		}
	}
}

//=============================================================================================================================
/**
	@remarks
			���� �ð� �̻� ��Ŷ �۽��� ���� ���ǵ��� �����Ѵ�.
*/
//=============================================================================================================================
VOID IoHandler::KickDeadSessions()
{
	SESSION_LIST_ITER	it;
// 	Session				*pSession;

	// ���ŵ� ���ǵ��� ActiveSessionList���� �����ϰ� �ӽ� ����Ʈ�� ��´�.
	//m_pActiveSessionList->Lock();

	/*
	for( it = m_pActiveSessionList->begin(); it != m_pActiveSessionList->end(); ++it )
	{
		pSession = *it;

		if( pSession->ShouldBeRemoved() )
		{
			m_pActiveSessionList->erase( it-- );
			m_pTempList->push_back( pSession );
		}
	}

	*/
	//m_pActiveSessionList->Unlock();

	// ���ŵ� ���ǵ鿡 ���� ó��
	for( it = m_pDeadSessionList->begin(); it != m_pDeadSessionList->end(); ++it )
	{
		int nStep = 0;
		DEBUG_TRY
		Session *pSession = *it;		

		//NetworkObject *pNetworkObject = pSession->GetNetworkObject();

		// ��Ʈ�� ������Ʈ ����ε�
		//pSession->UnbindNetworkObject();

		// �ݹ��Լ� ȣ�� ���� ������ free ���ش�. �ݹ� �ȿ��� Connect��û�� ���� �����Ƿ�..
		// accept�� �����̸� ������ �����Ͽ� �ٽ� accept�� �ɰ�, connect�� �����̸� Ǯ�� ��ȯ�Ѵ�.
		if( pSession->IsAcceptSocket() && !m_bShutdown )
		{
			nStep = 1;
			pSession->CloseSocket();
			nStep =2;
			m_pReuseSessionList->push_back(pSession);
		}
			//ReuseSession( pSession );
		else
		{
			nStep = 3;
			FreeSession( pSession );
		}

		nStep = 4;
		m_pIOCPServer->ProcessDisconnectClient(pSession->GetClientID());

		 // ������ ����
		if(m_numActiveSessions > 0)
			--m_numActiveSessions;
		else
			LOGWARNING("--m_numActiveSessions m_numActiveSessions == 0");
		

		 DEBUG_CATCH2("m_pIOCPServer->ProcessDisconnectClient() nStep = %u",nStep)
		// NetworkObject�� ��Ŀ��Ʈ ����
		//pNetworkObject->OnDisconnect();

		// ��Ʈ�� ������Ʈ �Ҹ� ��û		
		//if( pSession->IsAcceptSocket() )	m_fnDestroyAcceptedObject( pNetworkObject );						
		//else								m_fnDestroyConnectedObject( pNetworkObject );			
	}

	m_pDeadSessionList->clear();
}

VOID IoHandler::ProcessSend()
{
	SESSION_LIST_ITER	it;
	Session				*pSession;

	// ActiveSessionList�� ������ ������ �۹� ����
	m_pActiveSessionList->Lock();
	for( it = m_pActiveSessionList->begin(); it != m_pActiveSessionList->end(); ++it )
	{
		pSession = *it;

		if( pSession->ShouldBeRemoved() ) continue;

		if( pSession->PreSend() == FALSE ) 
		{
			//printf("[REMOVE][%d] pSession->PreSend() == FALSE\n", (int)pSession->GetSocket());
			pSession->Remove();
		}
	}
	m_pActiveSessionList->Unlock();
}

//=============================================================================================================================
/**
	@remarks
			��� ���ǵ��� �����Ѵ�.
*/
//=============================================================================================================================
VOID IoHandler::KickAllSessions()
{
	SESSION_LIST_ITER	it;

	for( it = m_pActiveSessionList->begin(); it != m_pActiveSessionList->end(); ++it )
	{
		(*it)->Remove();
	}
}

//=============================================================================================================================
/**
	@remarks
			Accept�� ���ǰ� Connect�� ���ǵ��� IOCP�� ����ϸ鼭 ActiveSessionList�� �ű��
			��� ������ �ޱ� ���۸� �˻��Ͽ� ��Ŷ�� ó���ϴ� ���� �۾��� �Ѵ�.
	@par
			�� �Լ��� �ֱ������� ȣ���� �־�� �Ѵ�.
*/
//=============================================================================================================================
VOID IoHandler::Update()
{
	int nStep = 0;
	DEBUG_TRY
	ProcessActiveSessionList();

	nStep = 1;
	if( !m_pAcceptedSessionList->empty() )
	{
		ProcessAcceptedSessionList();
	}

//	if( !m_pConnectSuccessList->empty() )
//	{
//		ProcessConnectSuccessList();
//	}
//
//	if( !m_pConnectFailList->empty() )
//	{
//		ProcessConnectFailList();
//	}

	nStep = 2;
	KickDeadSessions();

	nStep = 3;
	ProcessReuseSessionList();
	DEBUG_CATCH2("IoHandler::Update() step = %u",nStep)
}


VOID IoHandler::Shutdown()
{
	m_bShutdown = TRUE;

	KickAllSessions();

	ProcessActiveSessionList();

	KickDeadSessions();

	m_pAcceptor->Shutdown();

	///��Σ����
// 	for(int j=0;j<m_sessionListHolder.size();++j)
// 	{
// 		Session* pSession=m_sessionListHolder[j];
// 		if(pSession->GetSocket()!=INVALID_SOCKET)
// 			FreeSession(pSession);
// 	}
// 	m_sessionListHolder.clear();
	///��Σ����

	//m_pConnector->Shutdown();

	// IOCP ������ ����
	for( DWORD i = 0; i < m_numIoThreads; ++i )
	{
		PostQueuedCompletionStatus( m_hIOCP, 0, 0, NULL );
	}

	// IO ��Ŀ �����尡 ��� ����� ������ ��ٸ���.
	WaitForMultipleObjects( m_numIoThreads, m_hIoThread, TRUE, INFINITE );
}

VOID IoHandler::ProcessReuseSessionList()
{
	SESSION_LIST_ITER	it;

	for( it = m_pReuseSessionList->begin(); it != m_pReuseSessionList->end(); ++it )
	{
 		if((*it)->CanReuse())
 		{
			Session* pSession = *it;
			ReuseSession(pSession);
			//m_pReuseSessionList->erase(it--);//������� xingbarking
		}
	}
}