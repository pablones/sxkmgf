#include <winsock2.h>
#include <mswsock.h>
#include <iostream>
#include <assert.h>
#include "Acceptor.h"
#include "IoHandler.h"
#include "IOCPServer.h"
#include "Session.h"
#include "RecvBuffer.h"
#include "SessionList.h"
#include "SessionPool.h"
#include "MsWinsockUtil.h"
#include "common.h"

//=============================================================================================================================
/**
	@remarks
			���� ������ ���� ������ ó���ϴ� ��Ŀ ������
*/
//=============================================================================================================================
unsigned __stdcall accept_thread( LPVOID param )
{
	Acceptor		*pAcceptor = (Acceptor*)param;
	IoHandler		*pIoHandler = pAcceptor->m_pIoHandler;

	BOOL			bSuccess = FALSE;
	DWORD			dwIoSize = 0;
	OVERLAPPEDEX	*pOverlappedEx = NULL;
	VOID			*pDummy = NULL;

	BYTE			*pRecvBuf = NULL;
	int				nLen;

	SOCKADDR		*lpLocalSockaddr = NULL;
	SOCKADDR		*lpRemoteSockaddr = NULL;
	int				nLocalSockaddrLen = 0;
	int				nRemoteSockaddrLen = 0;

	int				err;

	while(TRUE)
	{
		int nStep = 0;
		DEBUG_TRY
		bSuccess = GetQueuedCompletionStatus( pAcceptor->m_hIOCP, &dwIoSize, (LPDWORD)&pDummy,
											  (LPOVERLAPPED*)&pOverlappedEx, INFINITE );

		nStep = 1;
		// ������ ���� ����� ������ ���
		if( pDummy == NULL )	break;
		
		if( !bSuccess )
		{
			err = WSAGetLastError();

			if( err == ERROR_OPERATION_ABORTED )
			{
				// ���� ������ ����� �� ���Ͽ� AcceptEx �ɾ���� ���ǵ鿡 ���� ERROR_OPERATION_ABORTED �߻�
// 				if(pOverlappedEx&&pOverlappedEx->pSession)
// 					delete pOverlappedEx->pSession;
				continue;
			}
			else if( err != ERROR_NETNAME_DELETED )
			{
				// AcceptEx���� ERROR_NETNAME_DELETED�� ������ �ƴ�(�� �̿��� ������ continue)
				continue;
			}
		}

		nStep = 2;
		switch( pOverlappedEx->dwOperationType ) 
		{
		case ACCEPT_POSTED:
			nStep = 3;
			if(!pOverlappedEx||(!(pOverlappedEx->pSession))||(!(pOverlappedEx->pSession->GetRecvBuffer())))
					break;

			pOverlappedEx->pSession->GetRecvBuffer()->GetRecvParam( &pRecvBuf, nLen );

			nStep = 4;
			// SockAddr �޾ƿ���
			MsWinsockUtil::m_lpfnGetAcceptExSockAddrs( pRecvBuf,
													   0,
													   sizeof(SOCKADDR_IN) + 16,
													   sizeof(SOCKADDR_IN) + 16,
													   &lpLocalSockaddr,
													   &nLocalSockaddrLen,
													   &lpRemoteSockaddr,
													   &nRemoteSockaddrLen );

			nStep = 5;
			pOverlappedEx->pSession->SetSockAddr( (*(SOCKADDR_IN*)lpRemoteSockaddr) );

			nStep = 6;
			// �ʱ�ȭ�� ������ ������ PreAcceptedList�� �߰��Ѵ�.
			pIoHandler->m_pAcceptedSessionList->Lock();
			pIoHandler->m_pAcceptedSessionList->push_back( pOverlappedEx->pSession );
			pIoHandler->m_pAcceptedSessionList->Unlock();
			break;
		}
		DEBUG_CATCH2("accept_thread nStep = %d",nStep)
	}

	return 0;
}


Acceptor::Acceptor()
{
	m_listenSocket	= INVALID_SOCKET;
	m_hIOCP			= INVALID_HANDLE_VALUE;
	for( DWORD i = 0; i < MAX_ACCEPT_THREAD; ++i ) m_hThread[i] = INVALID_HANDLE_VALUE;
	m_numThreads	= 0;

	//m_hEvent = NULL;
}

Acceptor::~Acceptor()
{
	Shutdown();

	if( m_hIOCP != INVALID_HANDLE_VALUE )			CloseHandle( m_hIOCP );
	for( DWORD i = 0; i < m_numThreads; ++i )
	{
		if( m_hThread[i] != INVALID_HANDLE_VALUE ) CloseHandle( m_hThread[i] );
	}

//	if(m_hEvent)
//		CloseHandle(m_hEvent);
}

VOID Acceptor::Init( IoHandler *pIoHandler, DWORD dwNumberOfThreads )
{
	assert( dwNumberOfThreads <= MAX_ACCEPT_THREAD );

	m_pIoHandler	= pIoHandler;
	m_numThreads	= dwNumberOfThreads;

	// Accept�� IOCP ����
	m_hIOCP		= CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	// ��Ŀ ������ ����
	unsigned threadID;
	for( DWORD i = 0; i < m_numThreads; ++i )
	{
		m_hThread[i] = (HANDLE)_beginthreadex( NULL, 0, accept_thread, (LPVOID)this, 0/*CREATE_SUSPENDED*/, &threadID );
	}

	//m_hEvent = WSACreateEvent();
}

//=============================================================================================================================
/**
	@remarks
			��Ʈ�� �ʱ�ȭ�Ѵ�.
	@param	pIP
			������ IP
	@param	wPort
			������ ��Ʈ
	@retval	BOOL
			������ �����ϸ� FALSE ����
*/
//=============================================================================================================================
BOOL Acceptor::StartListen( char *pIP, WORD wPort )
{	
	// ���� ���� ����
	if( !CreateListenSocket( pIP, wPort ) ) return FALSE;

	// IOCP�� ���� ���� ���
	CreateIoCompletionPort( (HANDLE)m_listenSocket, m_hIOCP, 1, 0 );

	//Dll���� �Լ� �ε�(AcceptEx, TransmitFile)
	MsWinsockUtil::LoadExtensionFunction( m_listenSocket );

	return TRUE;
}

//=============================================================================================================================
/**
	@remarks
			���� ������ �����ϰ� �ּҸ� �����Ѵ�.
	@retval	BOOL
			���� ���� ������ �����ϸ� FALSE�� �����Ѵ�.
			(IP�� port�� ��ġ�� ����� ���̴�.)
*/
//=============================================================================================================================
BOOL Acceptor::CreateListenSocket( char *pIP, WORD wPort )
{
	// �̹� ���� ������ �����Ǿ� ������ ����
	if( m_listenSocket != INVALID_SOCKET )
	{
		return FALSE;
	}

	// ���� ���� ����
	m_listenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );

	if( m_listenSocket == INVALID_SOCKET )
	{
		return FALSE;
	}

//	int nRet = WSAEventSelect(m_listenSocket,
//		m_hEvent,
//		FD_ACCEPT);
//	if ( nRet == SOCKET_ERROR )
//	{
//		closesocket(m_listenSocket);
//		return FALSE;
//	}

	// �ּ� ����
	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family		= AF_INET;
	m_sockaddr.sin_addr.s_addr	= ( pIP == NULL || strlen( pIP ) == 0 ) ? htonl(INADDR_ANY) : inet_addr(pIP);
	m_sockaddr.sin_port			= htons( wPort );

	// ���ε�
	int err = bind( m_listenSocket, (SOCKADDR *)&m_sockaddr, sizeof(m_sockaddr) );

	if( err == SOCKET_ERROR )
	{
		closesocket( m_listenSocket );
		return FALSE;
	}

	// ����
	err = listen( m_listenSocket, SOMAXCONN );

	if( err == SOCKET_ERROR )
	{
		closesocket( m_listenSocket );
		return FALSE;
	}

	return TRUE;
}

//=============================================================================================================================
/**
	@remarks
			���� �����带 ��� �����.
*/
//=============================================================================================================================
VOID Acceptor::SuspendListenThread()
{
	for( DWORD i = 0; i < m_numThreads; ++i )
	{
		assert( m_hThread[i] != INVALID_HANDLE_VALUE );

		SuspendThread( m_hThread[i] );
	}
}

//=============================================================================================================================
/**
	@remarks
			���� �����带 �ٽ� ������Ų��.
*/
//=============================================================================================================================
VOID Acceptor::ResumeListenThread()
{
	for( DWORD i = 0; i < m_numThreads; ++i )
	{
		assert( m_hThread != INVALID_HANDLE_VALUE );

		ResumeThread( m_hThread[i] );
	}
}

//=============================================================================================================================
/**
	@remarks
			���� �����带 �����Ų��.
*/
//=============================================================================================================================
VOID Acceptor::Shutdown()
{
	// ���� ���� ����
	if( m_listenSocket != INVALID_SOCKET )		closesocket( m_listenSocket );

	// acceptor ������ ����
	for( DWORD i = 0; i < m_numThreads; ++i )	PostQueuedCompletionStatus( m_hIOCP, 0, 0, NULL );

	// ��� ��Ŀ �����尡 ����� ������ ��ٸ���.
	WaitForMultipleObjects( m_numThreads, m_hThread, TRUE, INFINITE );
}
