#include <winsock2.h>
#include <conio.h>
#include <iostream>
#include <assert.h>
#include <map>
#include "IOCPServer.h"
#include "IoHandler.h"
#include "SessionList.h"

#include "../share/define.h"
#include "protocol.h"
#include "EncryptServer.h"
//#include "SSocket.h"
#include "I_Shell.h"
#include "ServerSocket.h"
#include "I_MessagePort.h"
#include "basefunc.h"


//=============================================================================================================================
/**
	@remarks
			세션 리스트를 순회하며 보내기 버퍼에 있는 패킷들을 전송하는 쓰레드
	@param	param
			IOCP 서버의 포인터
*/
//=============================================================================================================================
unsigned __stdcall send_thread( LPVOID param )
{
	IOCPServer *pIOCPServer = (IOCPServer*)param;
	
	IOHANDLER_MAP_ITER	it;
	
	while( !pIOCPServer->m_bShutdown )
	{
		
		pIOCPServer->Update();
		Sleep( /*10*/1 );
	}

	return 0;
}

IOCPServer::IOCPServer()
{
	m_bShutdown				= FALSE;
	// m_hSendThread			= NULL;
}

IOCPServer::~IOCPServer()
{
	if( !m_bShutdown )		Shutdown();

	// if( m_hSendThread )		CloseHandle( m_hSendThread );
	
	WSACleanup();
}

//=============================================================================================================================
/**
	@remarks
			서버를 초기화한다.
	@param	lpDesc
			서버 초기화에 필요한 정보를 담은 구조체의 포인터
	@retval	BOOL
			윈속 초기화에 실패하거나 리슨 소켓 생성에 실패하면 FALSE를 리턴한다.
*/
//=============================================================================================================================
BOOL IOCPServer::Init( LPIOHANDLER_DESC lpDesc, DWORD dwNumberofIoHandlers )
{
	// 윈속 초기화
	if( !InitWinsock() )	return FALSE;

	for( DWORD i = 0; i < dwNumberofIoHandlers; ++i )
	{
		CreateIoHandler( lpDesc + i );
	}

	// Send 워커 쓰레드 생성
//	unsigned threadID;
//	m_hSendThread = (HANDLE)_beginthreadex( NULL, 0, send_thread, (LPVOID)this, 0, &threadID );

	return TRUE;
}

VOID IOCPServer::CreateIoHandler( LPIOHANDLER_DESC lpDesc )
{
	IoHandler *pIoHandler = new IoHandler;

	pIoHandler->Init( this, lpDesc );

	m_mapIoHandlers.insert( IOHANDLER_MAP_PAIR( pIoHandler->GetKey(), pIoHandler ) );
}


VOID IOCPServer::Shutdown()
{
	m_bShutdown = TRUE;

	// Send 워커 쓰레드가 종료될 때까지 기다린다.
	// WaitForSingleObject( m_hSendThread, INFINITE );

	// IoHandler들을 종료시키고 포인터를 삭제한다.
	IOHANDLER_MAP_ITER		it;
	IoHandler				*pIoHandler;

	for( it = m_mapIoHandlers.begin(); it != m_mapIoHandlers.end(); ++it )
	{
		pIoHandler = it->second;
		pIoHandler->Shutdown();
		delete pIoHandler;
	}
	m_mapIoHandlers.clear();
}

//=============================================================================================================================
/**
	@remarks
			Accept된 세션과 Connect된 세션들을 IOCP에 등록하면서 활성 리스트로 옮기고
			모든 세션의 받기 버퍼를 검사하여 패킷을 처리하는 등의 작업을 한다.
*/
//=============================================================================================================================
VOID IOCPServer::Update()
{
	IOHANDLER_MAP_ITER it;

	for( it = m_mapIoHandlers.begin(); it != m_mapIoHandlers.end(); ++it )
	{
		it->second->Update();
	}
}

void IOCPServer::ProcessSend()
{
	IOHANDLER_MAP_ITER it;
	
	for( it = m_mapIoHandlers.begin(); it != m_mapIoHandlers.end(); ++it )
	{
		it->second->ProcessSend();
	}
}

BOOL IOCPServer::StartListen( DWORD dwIoHandlerKey, char *pIP, WORD wPort )
{
	IOHANDLER_MAP_ITER it = m_mapIoHandlers.find( dwIoHandlerKey );

	assert( it != m_mapIoHandlers.end() );

	return it->second->StartListen( pIP, wPort );
}

BOOL IOCPServer::StartAllListen( char *pIP, WORD wPort )
{
	IOHANDLER_MAP_ITER it;

	BOOL bSuc = TRUE;
	
	for(it = m_mapIoHandlers.begin();it!=m_mapIoHandlers.end();++it)
		if(!(it->second->StartListen(pIP,wPort)))
		{
			bSuc = FALSE;
		}

	return bSuc;

	
}

//=============================================================================================================================
/**
	@remarks
			- connect 쓰레드를 사용하여 접속을 시도한다.
	@param	dwIoHandlerKey
			- 접속을 시도하기 위한 세션을 가지고 있는 I/O 핸들러의 키
			- 접속 성공 또는 실패시 OnConnect( BOOL )이 호출된다.
	@param	szIP
			접속할 IP
	@param	wPort
			접속할 포트
	@retval	DWORD
			접속을 시도할 세션의 인덱스
*/
//=============================================================================================================================
//DWORD IOCPServer::Connect( DWORD dwIoHandlerKey, NetworkObject *pNetworkObject, char *pszIP, WORD wPort )
//{
//	if( pNetworkObject == NULL ) return 0;
//
//	IOHANDLER_MAP_ITER it = m_mapIoHandlers.find( dwIoHandlerKey );
//
//	assert( it != m_mapIoHandlers.end() );
//
//	return it->second->Connect( pNetworkObject, pszIP, wPort );
//}

BOOL IOCPServer::IsListening( DWORD dwIoHandlerKey )
{
	IOHANDLER_MAP_ITER it = m_mapIoHandlers.find( dwIoHandlerKey );

	assert( it != m_mapIoHandlers.end() );

	return it->second->IsListening();
}

DWORD IOCPServer::GetNumberOfConnections( DWORD dwIoHandlerKey )
{
	IOHANDLER_MAP_ITER it = m_mapIoHandlers.find( dwIoHandlerKey );

	assert( it != m_mapIoHandlers.end() );

	return it->second->GetNumberOfConnections();
}

//=============================================================================================================================
/**
	@remarks
			윈속을 초기화한다.
*/
//=============================================================================================================================
BOOL IOCPServer::InitWinsock()
{
	WSADATA wsaData;

	WSAStartup( MAKEWORD( 2, 2 ), &wsaData );

	if( wsaData.wVersion != MAKEWORD( 2, 2 ) )
	{
		WSACleanup();

		return FALSE;
	}
	
	return TRUE;
}

void IOCPServer::SetKernel(CSocketKernel* pKernel)
{
	m_pSocketKernel = pKernel;
}

int IOCPServer::ProcessData(const char* pRecvBuf,int nSize,Session* pSession)
{
	return m_pSocketKernel->ProcessRecvMsg(pRecvBuf,nSize,pSession);
}

int	IOCPServer::SetNewClientSocket(Session* pSession)
{
	return  m_pSocketKernel->SetNewSocket(pSession);
}

void IOCPServer::ProcessDisconnectClient(int i)
{
	m_pSocketKernel->ProcessDisconnServer(i);
}

// BOOL IOCPServer::IsNpcID(int i)
// {
// 	return m_pSocketKernel->IsNpcSocketID(i);
// }

BOOL IOCPServer::StartSendProcessThread()
{
	unsigned threadID;
	m_hSendThread = (HANDLE)_beginthreadex( NULL, 0, send_thread, (LPVOID)this, 0, &threadID );
	return TRUE;
}