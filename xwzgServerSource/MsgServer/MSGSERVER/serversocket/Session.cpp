#include <iostream>
#include <assert.h>
#include <winsock2.h>
#include <mswsock.h>
#include "Session.h"
#include "SessionPool.h"
#include "SendBuffer.h"
#include "RecvBuffer.h"
#include "NetBase.h"
//#include "NetworkObject.h"

#include "IoHandler.h"
#include "IOCPServer.h"
#include "MsWinsockUtil.h"

#include "../share/define.h"

using namespace std;
//modify code by �ֵ�Ԩ begin 2011.5.19
#include ".\\..\\MAPGROUPKERNEL\\statistical.h"
//modify code by �ֵ�Ԩ end

const char _SX_RETURN[]= "<cross-domain-policy> "  
"<allow-access-from domain=\"*\" to-ports=\"*\"/>"     
        "</cross-domain-policy> ";//

//=============================================================================================================================
/**
	@remarks
			���� ������.\n
			������ ���ۿ� �ޱ� ���۸� �����ϰ� ��� �޸� �ð��� �����Ѵ�.
	@param	dwSendBufferSize
			������ ���� ������
	@param	dwRecvBufferSize
			�ޱ� ���� ������
	@param	dwTimeOut
			�� �ð����� �������� ������ �޼����� ������ ������ ���´�.
			�� ���� 0�̸� �޼��� �۽��� ��� ������ ���� �ʴ´�.
*/
//=============================================================================================================================
Session::Session( DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize/*, DWORD dwTimeOut*/ )
{
	m_pSendBuffer = new SendBuffer;
	m_pSendBuffer->Create( dwSendBufferSize, dwMaxPacketSize );

	m_pRecvBuffer = new RecvBuffer;
	m_pRecvBuffer->Create( dwRecvBufferSize, dwMaxPacketSize );

	//m_dwTimeOut			= dwTimeOut;
	m_socket			= INVALID_SOCKET;
	m_bAcceptSocket		= FALSE;
}

Session::~Session()
{
	CloseSocket();

	if( m_pSendBuffer )		delete m_pSendBuffer;
	if( m_pRecvBuffer )		delete m_pRecvBuffer;
}

//=============================================================================================================================
/**
	@remarks
			������ �ʱ�ȭ�Ѵ�.
	@param	socket
			���ǿ� �Ҵ�� ���� �ڵ�
	@param	sockaddr
			���ǿ� �Ҵ�� ���� �ּ�
	@retval	BOOL
			�־��� ���� �ڵ��� ��ȿ���� ���� ��� FALSE�� �����Ѵ�.
*/
//=============================================================================================================================
VOID Session::Init()
{
	m_pSendBuffer->Clear();
	m_pRecvBuffer->Clear();

	ResetKillFlag();

	m_bDisconnectOrdered = FALSE;

//	m_cEncryptRecv.Init();
//	m_cEncryptSend.Init();

	m_OverlappedIONum = 0;
}

VOID Session::Reuse( HANDLE hIOCP )
{
	//Disconnect�� ovl IO ����
// 	ZeroMemory( &m_disconnectIoData, sizeof(OVERLAPPEDEX) );
// 	m_disconnectIoData.dwOperationType	= DISCONNECT_POSTED;

	// TransmitFile�� �����ϸ� ��� IOCP�� ������ �־��ش�.
// 	if( !MsWinsockUtil::m_lpfnTransmitFile( m_socket, 0, 0, 0, &m_disconnectIoData, 0, TF_DISCONNECT | TF_REUSE_SOCKET )
// 		&& WSAGetLastError() != WSA_IO_PENDING )
// 	{		
// 		CloseSocket();		
// 		PostQueuedCompletionStatus( hIOCP, 0, (ULONG_PTR)this, &m_disconnectIoData );
// 	}


	ZeroMemory( &m_disconnectIoData, sizeof(OVERLAPPEDEX) );
	m_disconnectIoData.dwOperationType	= DISCONNECT_POSTED;

		
	PostQueuedCompletionStatus( hIOCP, 0, (ULONG_PTR)this, &m_disconnectIoData );
	
}

//=============================================================================================================================
/**
	@remarks
			�ش� ��Ŷ�� ������ ���ۿ� ����.
	@param	pMsg
			��Ŷ ������
	@param	wSize
			��� ũ�⸦ ������ ��Ŷ ������
	@retval	BOOL
			���ۿ� ���������� ���⸦ ��ġ�� TRUE�� �����ϰ� ������ ���۰� ���� ��� FALSE�� �����Ѵ�.
*/
//=============================================================================================================================
BOOL Session::Send( BYTE *pMsg, WORD wSize )
{
	//PACKET_HEADER header;

	// ��� ����
	//header.size = wSize;

	if(m_bRemove)
		return FALSE;

//	m_cEncryptSend.Encrypt((unsigned char *)pMsg,wSize);
	// ������ ���ۿ� ����
	if( /*m_pSendBuffer->Write( &header, pMsg ) == FALSE*/ m_pSendBuffer->Write( pMsg, wSize ) == FALSE)
	{
		//OnLogString( "������ ���۰� �� á���Ƿ� ������ �����ϴ�." );
		LOGERROR("���ͻ��������,��ǰ����������%d,ʣ�໺��������%d,���͵��ֽ���%d",m_pSendBuffer->GetLength(),m_pSendBuffer->GetSpace(),wSize);
		LOGERROR("���ͻ��������,ip:%s,������Ǹ��ͻ�������,���Կ�����ջ��������ǶϿ�",GetIP());
		Remove();
		return FALSE;
	}

	return TRUE;
}

// BOOL Session::SendNPC( BYTE *pMsg, WORD wSize )
// {
// 	//PACKET_HEADER header;
// 
// 	// ��� ����
// 	//header.size = wSize;
// 
// 	if(m_bRemove)
// 		return FALSE;
// 	//���͸�npc����Ϣ
// //	m_cEncryptSend.Encrypt((unsigned char *)pMsg,wSize);
// 	// ������ ���ۿ� ����
// 	if( /*m_pSendBuffer->Write( &header, pMsg ) == FALSE*/ m_pSendBuffer->Write( pMsg, wSize ) == FALSE)
// 	{
// 		//OnLogString( "������ ���۰� �� á���Ƿ� ������ �����ϴ�." );
// 		LOGERROR("NPC���ͻ��������,��ǰ����������%d,ʣ�໺��������%d,���͵��ֽ���%d,�����ð�",m_pSendBuffer->GetLength(),m_pSendBuffer->GetSpace(),wSize);
// 		Remove();
// 		return FALSE;
// 	}
// // 	if(PreSend()==false)
// // 	{
// // 		Remove();
// // 		return false;
// // 	}
// 	return TRUE;
// }

//��Ҫ���øú���,û�޸Ķ�Ӧ��Դ����
BOOL Session::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	assert( !IsBadReadPtr( ppMsg, sizeof(ppMsg) * dwNumberOfMessages ) );
	assert( !IsBadReadPtr( pwSize, sizeof(pwSize) * dwNumberOfMessages ) );

	PACKET_HEADER header;

	// ��� ����
	header.size = 0;
	DWORD i;
	for( i = 0; i < dwNumberOfMessages; ++i )
	{
		header.size += pwSize[i];
	}

	// ������ ���ۿ� ����(���)
	if( !m_pSendBuffer->Write( (BYTE*)&header, sizeof(PACKET_HEADER) ) )
	{
		//OnLogString( "������ ���۰� �� á���Ƿ� ������ �����ϴ�." );
		Remove();
		return FALSE;
	}

	// ������ ���ۿ� ����(dwNumberOfMessages���� �޼���)
	for( i = 0; i < dwNumberOfMessages; ++i )
	{
		if( !m_pSendBuffer->Write( ppMsg[i], pwSize[i] ) )
		{
			//OnLogString( "������ ���۰� �� á���Ƿ� ������ �����ϴ�." );
			Remove();
			return FALSE;
		}
	}

	return TRUE;
}


BOOL Session::ProcessRecvdPacket( DWORD dwMaxPacketSize )
{
	BYTE			*pPacket;
	
	int nRet;
	
	short header;
	
	int nStep = 0;

	DEBUG_TRY
// 	if(m_pIoHandler->GetIOCPServer()->IsNpcID(m_ClientID))
// 	{
// 		nStep = 11;
// 		while( pPacket = GetRecvBuffer()->GetFirstPacketPtr() )
// 		{
// 			nStep = 12;
// 			header = *((short*)pPacket);
// 			nRet = m_pIoHandler->GetIOCPServer()->ProcessData((char*)pPacket,header/*GetRecvBuffer()->GetLength()*/,this);
// 
// 			nStep = 13;
// 			if(nRet)
// 			{
// 				nStep = 14;
// 				GetRecvBuffer()->RemoveFirstPacket(nRet);
// 			}
// 			else
// 				break;
// 		}
// 
// 	}
// 	else
// 	{
		nStep = 21;
		if( pPacket = GetRecvBuffer()->GetFirstPacketPtr() )
		{
			nStep = 22;
			header = *((short*)pPacket);
			nRet = m_pIoHandler->GetIOCPServer()->ProcessData((char*)pPacket,header,this);

			nStep = 23;
			if(nRet)
			{
				nStep = 24;
				GetRecvBuffer()->RemoveFirstPacket(nRet);
			}
		}

//	}
	DEBUG_CATCH2("Session::ProcessRecvdPacket() nStep = %u",nStep)
	return TRUE;
}

//=============================================================================================================================
/**
	@remarks
			IOCP�� send�� �ɾ� ���´�.
	@retval BOOL
			- ������ ���۰� ����ְų� ���� �غ� �ȵ� �����̸� �׳� TRUE�� �����Ѵ�.
			- send�� �����ϸ� FALSE�� �����Ѵ�.
*/
//=============================================================================================================================
BOOL Session::PreSend()
{
	WSABUF wsabuf;
	
	int nStep = 0;
	try{
		if( GetSendBuffer()->GetSendParam( (BYTE**)&wsabuf.buf, (int&)wsabuf.len ) == FALSE )
		{
			return TRUE;
		}

		nStep = 1;
		ZeroMemory( &m_sendIoData, sizeof(OVERLAPPEDEX) );

		nStep = 2;
		m_sendIoData.dwOperationType = SEND_POSTED;
		AddOverlappedNum();

		nStep = 3;
		int ret = WSASend( GetSocket(), &wsabuf, 1, &m_sendIoData.dwIoSize, m_sendIoData.dwFlags, &m_sendIoData, NULL );

		nStep = 4;
		if( ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING )
		{
			nStep = 5;
			ReleaseOverlappedNum();
			return FALSE;
			
		}
	}
	catch(...)
	{
		LOGWARNING("Session::PreSend() ERROR! nStep = %u ip = %s",nStep,GetIP());
		return FALSE;
	}

	return TRUE;
}

//=============================================================================================================================
/**
	@remarks
			IOCP�� recv�� �ɾ� ���´�.
	@retval BOOL
			recv�� �����ϸ� FALSE�� �����Ѵ�.
*/
//=============================================================================================================================
BOOL Session::PreRecv(int nCode)
{
	WSABUF wsabuf;

	m_pRecvBuffer->GetRecvParam( (BYTE**)&wsabuf.buf, (int&)wsabuf.len );

	if(wsabuf.len == 0)
	{
		LOGERROR("���ܻ�������������ʱ�޷���������[%d]", nCode);
		LOGERROR("���ܻ��������,ip:%s,���Ҫô�ǶԷ������ˣ�Ҫô�Ƿ����������ˣ�����̫��",GetIP());
		//Remove();
		return FALSE;
	}

	ZeroMemory( &m_recvIoData, sizeof(OVERLAPPEDEX) );

	m_recvIoData.dwOperationType = RECV_POSTED;

	AddOverlappedNum();

	int ret = WSARecv( GetSocket(), &wsabuf, 1, &m_recvIoData.dwIoSize, &m_recvIoData.dwFlags, &m_recvIoData, NULL ); 

	if( ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING )
	{
		ReleaseOverlappedNum();
		return FALSE;
	}

	return TRUE;
}

BOOL Session::PreAccept( SOCKET listenSocket )
{
	DWORD	dwBytes		= 0;
	int		nLen		= 0;
	BYTE	*pRecvBuf	= NULL;

	int nStep = 0;
	try{

	if( GetSocket() == INVALID_SOCKET )
	{
		SetSocket( CreateSocket() );
	}

	nStep = 1;
	Init();
	

	nStep = 2;
	//Session ���� Recv Ptr���� �¾�..
	GetRecvBuffer()->GetRecvParam( &pRecvBuf, nLen );

	}catch(...)
	{
		LOGWARNING("Session::PreAccept1(),error!!! nStep = %u",nStep);
		return FALSE;
	}

	try{
	nStep = 3;
	//AcceptEx �� ovl IO ����
	ZeroMemory( &m_acceptIoData, sizeof(OVERLAPPEDEX) );
	m_acceptIoData.dwOperationType	= ACCEPT_POSTED;
	m_acceptIoData.pSession			= this;				//�ڽ��� ptr

	//printf("[ACCEPT][%d] Session::PreAccept, m_bRemove=%d\n", (int)GetSocket(), (int)m_bRemove);

	nStep = 4;
	//AcceptEx�� ���� Add
	int nRet = 	MsWinsockUtil::m_lpfnAccepteEx(	listenSocket, 
												GetSocket(), 
												pRecvBuf, 
												0, 
												sizeof(SOCKADDR_IN) + 16,
												sizeof(SOCKADDR_IN) + 16,
												&dwBytes,
												&m_acceptIoData );

	nStep = 5;
	if( nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING ) 
	{
		return FALSE;
	}

	}catch(...)
	{
		LOGWARNING("Session::PreAccept2(),error!!! nStep = %u",nStep);
		return FALSE;
	}

	return TRUE;
}

SOCKET Session::CreateSocket() 
{
	int nRet		= 0;
	int nZero		= 0;

	SOCKET newSocket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED ); 

	if( newSocket == INVALID_SOCKET ) 
	{
		return newSocket;
	}

	/*
	nZero	= 0;
	nRet	= setsockopt( newSocket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero) );

	if( nRet == SOCKET_ERROR ) 
	{
		return INVALID_SOCKET;
	}
	*/

	return newSocket;
}

//VOID Session::BindNetworkObject( NetworkObject *pNetworkObject )
//{
//	m_pNetworkObject = pNetworkObject;
//	pNetworkObject->SetSession( this );
//}
//
//VOID Session::UnbindNetworkObject()
//{
//	if( m_pNetworkObject == NULL )
//	{
//		return;
//	}
//	m_pNetworkObject->SetSession( NULL );
//
//	m_pNetworkObject = NULL;
//}

VOID Session::OnAccept()
{
	ResetKillFlag();

	//ResetTimeOut();

	//m_pNetworkObject->OnAccept( GetIndex() );

	//printf("[Session::OnAccept][%d] m_bRemove=%d\n", (int)GetSocket(), (int)m_bRemove);
}

//VOID Session::OnConnect( BOOL bSuccess )
//{
//	Init();
//
//	NetworkObject *pNetworkObject = m_pNetworkObject;
//
//	if( !bSuccess )
//	{		
//		UnbindNetworkObject();
//	}
//
//	pNetworkObject->OnConnect( bSuccess, GetIndex() );
//}

//VOID Session::OnLogString( char *pszLog, ... )
//{
//	if( !m_pNetworkObject ) return;
//
//	char		szBuffer[512] = "";
//	va_list		pArguments;
//
//	va_start( pArguments, pszLog );
//	vsprintf( szBuffer, pszLog, pArguments );
//	va_end( pArguments );
//
//	printf( "%s(%s)\n", szBuffer, GetIP() );
//
//	m_pNetworkObject->OnLogString( szBuffer );
//}

VOID Session::Disconnect( BOOL bGracefulDisconnect )
{ 
	if( bGracefulDisconnect ) 
	{ 
		//printf("[REMOVE][%d] bGracefulDisconnect\n", (int)GetSocket()); 
		Remove(); 
	} 
	else 
	{
		m_bDisconnectOrdered = TRUE; 
	}
}


void Session::EncyRecvData(int nSize)
{
	
//	m_cEncryptRecv.Encrypt((unsigned char *)(m_pRecvBuffer->GetWritePtr()),nSize);
}

VOID Session::Remove() 
{ 
	if(!m_bRemove)
	{
//modify code by �ֵ�Ԩ begin 2011.5.19
//		g_StatisticalData.EraseMaxRecv(GetClientID());
//modify code by �ֵ�Ԩ end
		InterlockedExchange( (long*)&m_bRemove, TRUE );
	}
}
