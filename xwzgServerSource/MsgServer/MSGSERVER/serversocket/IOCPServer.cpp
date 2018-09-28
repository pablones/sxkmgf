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
			���� ����Ʈ�� ��ȸ�ϸ� ������ ���ۿ� �ִ� ��Ŷ���� �����ϴ� ������
	@param	param
			IOCP ������ ������
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
			������ �ʱ�ȭ�Ѵ�.
	@param	lpDesc
			���� �ʱ�ȭ�� �ʿ��� ������ ���� ����ü�� ������
	@retval	BOOL
			���� �ʱ�ȭ�� �����ϰų� ���� ���� ������ �����ϸ� FALSE�� �����Ѵ�.
*/
//=============================================================================================================================
BOOL IOCPServer::Init( LPIOHANDLER_DESC lpDesc, DWORD dwNumberofIoHandlers )
{
	// ���� �ʱ�ȭ
	if( !InitWinsock() )	return FALSE;

	for( DWORD i = 0; i < dwNumberofIoHandlers; ++i )
	{
		CreateIoHandler( lpDesc + i );
	}

	// Send ��Ŀ ������ ����
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

	// Send ��Ŀ �����尡 ����� ������ ��ٸ���.
	// WaitForSingleObject( m_hSendThread, INFINITE );

	// IoHandler���� �����Ű�� �����͸� �����Ѵ�.
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
			Accept�� ���ǰ� Connect�� ���ǵ��� IOCP�� ����ϸ鼭 Ȱ�� ����Ʈ�� �ű��
			��� ������ �ޱ� ���۸� �˻��Ͽ� ��Ŷ�� ó���ϴ� ���� �۾��� �Ѵ�.
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
			- connect �����带 ����Ͽ� ������ �õ��Ѵ�.
	@param	dwIoHandlerKey
			- ������ �õ��ϱ� ���� ������ ������ �ִ� I/O �ڵ鷯�� Ű
			- ���� ���� �Ǵ� ���н� OnConnect( BOOL )�� ȣ��ȴ�.
	@param	szIP
			������ IP
	@param	wPort
			������ ��Ʈ
	@retval	DWORD
			������ �õ��� ������ �ε���
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
			������ �ʱ�ȭ�Ѵ�.
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