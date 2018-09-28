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
			리슨 소켓을 열고 접속을 처리하는 워커 쓰레드
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
		// 쓰레드 종료 명령이 내려진 경우
		if( pDummy == NULL )	break;
		
		if( !bSuccess )
		{
			err = WSAGetLastError();

			if( err == ERROR_OPERATION_ABORTED )
			{
				// 리슨 소켓을 종료시 그 소켓에 AcceptEx 걸어놓은 세션들에 대해 ERROR_OPERATION_ABORTED 발생
// 				if(pOverlappedEx&&pOverlappedEx->pSession)
// 					delete pOverlappedEx->pSession;
				continue;
			}
			else if( err != ERROR_NETNAME_DELETED )
			{
				// AcceptEx에서 ERROR_NETNAME_DELETED는 에러가 아님(그 이외의 에러는 continue)
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
			// SockAddr 받아오기
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
			// 초기화에 성공한 세션을 PreAcceptedList에 추가한다.
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

	// Accept용 IOCP 생성
	m_hIOCP		= CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	// 워커 쓰레드 생성
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
			포트를 초기화한다.
	@param	pIP
			접속할 IP
	@param	wPort
			접속할 포트
	@retval	BOOL
			리슨에 실패하면 FALSE 리턴
*/
//=============================================================================================================================
BOOL Acceptor::StartListen( char *pIP, WORD wPort )
{	
	// 리슨 소켓 생성
	if( !CreateListenSocket( pIP, wPort ) ) return FALSE;

	// IOCP에 리슨 소켓 등록
	CreateIoCompletionPort( (HANDLE)m_listenSocket, m_hIOCP, 1, 0 );

	//Dll에서 함수 로드(AcceptEx, TransmitFile)
	MsWinsockUtil::LoadExtensionFunction( m_listenSocket );

	return TRUE;
}

//=============================================================================================================================
/**
	@remarks
			리슨 소켓을 생성하고 주소를 설정한다.
	@retval	BOOL
			리슨 소켓 생성에 실패하면 FALSE를 리턴한다.
			(IP와 port가 겹치는 경우일 것이다.)
*/
//=============================================================================================================================
BOOL Acceptor::CreateListenSocket( char *pIP, WORD wPort )
{
	// 이미 리슨 소켓이 생성되어 있으면 실패
	if( m_listenSocket != INVALID_SOCKET )
	{
		return FALSE;
	}

	// 리슨 소켓 생성
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

	// 주소 셋팅
	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family		= AF_INET;
	m_sockaddr.sin_addr.s_addr	= ( pIP == NULL || strlen( pIP ) == 0 ) ? htonl(INADDR_ANY) : inet_addr(pIP);
	m_sockaddr.sin_port			= htons( wPort );

	// 바인드
	int err = bind( m_listenSocket, (SOCKADDR *)&m_sockaddr, sizeof(m_sockaddr) );

	if( err == SOCKET_ERROR )
	{
		closesocket( m_listenSocket );
		return FALSE;
	}

	// 리슨
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
			리슨 쓰레드를 잠시 멈춘다.
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
			리슨 쓰레드를 다시 구동시킨다.
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
			리슨 쓰레드를 종료시킨다.
*/
//=============================================================================================================================
VOID Acceptor::Shutdown()
{
	// 리슨 소켓 종료
	if( m_listenSocket != INVALID_SOCKET )		closesocket( m_listenSocket );

	// acceptor 쓰레드 종료
	for( DWORD i = 0; i < m_numThreads; ++i )	PostQueuedCompletionStatus( m_hIOCP, 0, 0, NULL );

	// 모든 워커 쓰레드가 종료될 때까지 기다린다.
	WaitForMultipleObjects( m_numThreads, m_hThread, TRUE, INFINITE );
}
