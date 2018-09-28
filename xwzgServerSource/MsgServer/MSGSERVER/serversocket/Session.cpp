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
//modify code by ÁÖµÂÔ¨ begin 2011.5.19
#include ".\\..\\MAPGROUPKERNEL\\statistical.h"
//modify code by ÁÖµÂÔ¨ end

const char _SX_RETURN[]= "<cross-domain-policy> "  
"<allow-access-from domain=\"*\" to-ports=\"*\"/>"     
        "</cross-domain-policy> ";//

//=============================================================================================================================
/**
	@remarks
			¼¼¼Ç »ý¼ºÀÚ.\n
			º¸³»±â ¹öÆÛ¿Í ¹Þ±â ¹öÆÛ¸¦ »ý¼ºÇÏ°í Çã¿ë ÈÞ¸é ½Ã°£À» ¼ÂÆÃÇÑ´Ù.
	@param	dwSendBufferSize
			º¸³»±â ¹öÆÛ »çÀÌÁî
	@param	dwRecvBufferSize
			¹Þ±â ¹öÆÛ »çÀÌÁî
	@param	dwTimeOut
			ÀÌ ½Ã°£µ¿¾È ¼¼¼ÇÀ¸·Î µé¾î¿À´Â ¸Þ¼¼Áö°¡ ¾øÀ¸¸é Á¢¼ÓÀ» ²÷´Â´Ù.
			ÀÌ °ªÀÌ 0ÀÌ¸é ¸Þ¼¼Áö ¼Û½ÅÀÌ ¾ø¾îµµ Á¢¼ÓÀ» ²÷Áö ¾Ê´Â´Ù.
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
			¼¼¼ÇÀ» ÃÊ±âÈ­ÇÑ´Ù.
	@param	socket
			¼¼¼Ç¿¡ ÇÒ´çµÉ ¼ÒÄÏ ÇÚµé
	@param	sockaddr
			¼¼¼Ç¿¡ ÇÒ´çµÉ ¼ÒÄÏ ÁÖ¼Ò
	@retval	BOOL
			ÁÖ¾îÁø ¼ÒÄÏ ÇÚµéÀÌ À¯È¿ÇÏÁö ¾ÊÀº °æ¿ì FALSE¸¦ ¸®ÅÏÇÑ´Ù.
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
	//Disconnect¿ë ovl IO »ý¼º
// 	ZeroMemory( &m_disconnectIoData, sizeof(OVERLAPPEDEX) );
// 	m_disconnectIoData.dwOperationType	= DISCONNECT_POSTED;

	// TransmitFile¿¡ ½ÇÆÐÇÏ¸é °æ¿ì IOCP¿¡ °­Á¦·Î ³Ö¾îÁØ´Ù.
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
			ÇØ´ç ÆÐÅ¶À» º¸³»±â ¹öÆÛ¿¡ ¾´´Ù.
	@param	pMsg
			ÆÐÅ¶ Æ÷ÀÎÅÍ
	@param	wSize
			Çì´õ Å©±â¸¦ Á¦¿ÜÇÑ ÆÐÅ¶ »çÀÌÁî
	@retval	BOOL
			¹öÆÛ¿¡ ¼º°øÀûÀ¸·Î ¾²±â¸¦ ¸¶Ä¡¸é TRUE¸¦ ¸®ÅÏÇÏ°í º¸³»±â ¹öÆÛ°¡ ²ËÂù °æ¿ì FALSE¸¦ ¸®ÅÏÇÑ´Ù.
*/
//=============================================================================================================================
BOOL Session::Send( BYTE *pMsg, WORD wSize )
{
	//PACKET_HEADER header;

	// Çì´õ ¼ÂÆÃ
	//header.size = wSize;

	if(m_bRemove)
		return FALSE;

//	m_cEncryptSend.Encrypt((unsigned char *)pMsg,wSize);
	// º¸³»±â ¹öÆÛ¿¡ ¾²±â
	if( /*m_pSendBuffer->Write( &header, pMsg ) == FALSE*/ m_pSendBuffer->Write( pMsg, wSize ) == FALSE)
	{
		//OnLogString( "º¸³»±â ¹öÆÛ°¡ ²Ë Ã¡À¸¹Ç·Î ¿¬°áÀ» ²÷½À´Ï´Ù." );
		LOGERROR("·¢ËÍ»º³åÇøÒç³ö,µ±Ç°»º³åÇø³¤¶È%d,Ê£Óà»º³åÇø³¤¶È%d,·¢ËÍµÄ×Ö½ÚÊý%d",m_pSendBuffer->GetLength(),m_pSendBuffer->GetSpace(),wSize);
		LOGERROR("·¢ËÍ»º³åÇøÒç³ö,ip:%s,Èç¹ûÕâÊÇ¸ö¿Í»§¶ËÁ¬½Ó,¿ÉÒÔ¿¼ÂÇÇå¿Õ»º³åÇø»òÊÇ¶Ï¿ª",GetIP());
		Remove();
		return FALSE;
	}

	return TRUE;
}

// BOOL Session::SendNPC( BYTE *pMsg, WORD wSize )
// {
// 	//PACKET_HEADER header;
// 
// 	// Çì´õ ¼ÂÆÃ
// 	//header.size = wSize;
// 
// 	if(m_bRemove)
// 		return FALSE;
// 	//·¢ËÍ¸ønpcµÄÏûÏ¢
// //	m_cEncryptSend.Encrypt((unsigned char *)pMsg,wSize);
// 	// º¸³»±â ¹öÆÛ¿¡ ¾²±â
// 	if( /*m_pSendBuffer->Write( &header, pMsg ) == FALSE*/ m_pSendBuffer->Write( pMsg, wSize ) == FALSE)
// 	{
// 		//OnLogString( "º¸³»±â ¹öÆÛ°¡ ²Ë Ã¡À¸¹Ç·Î ¿¬°áÀ» ²÷½À´Ï´Ù." );
// 		LOGERROR("NPC·¢ËÍ»º³åÇøÒç³ö,µ±Ç°»º³åÇø³¤¶È%d,Ê£Óà»º³åÇø³¤¶È%d,·¢ËÍµÄ×Ö½ÚÊý%d,Å×Æú¸Ã°ü",m_pSendBuffer->GetLength(),m_pSendBuffer->GetSpace(),wSize);
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

//²»Òªµ÷ÓÃ¸Ãº¯Êý,Ã»ÐÞ¸Ä¶ÔÓ¦µÄÔ´´úÂë
BOOL Session::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	assert( !IsBadReadPtr( ppMsg, sizeof(ppMsg) * dwNumberOfMessages ) );
	assert( !IsBadReadPtr( pwSize, sizeof(pwSize) * dwNumberOfMessages ) );

	PACKET_HEADER header;

	// Çì´õ ¼ÂÆÃ
	header.size = 0;
	DWORD i;
	for( i = 0; i < dwNumberOfMessages; ++i )
	{
		header.size += pwSize[i];
	}

	// º¸³»±â ¹öÆÛ¿¡ ¾²±â(Çì´õ)
	if( !m_pSendBuffer->Write( (BYTE*)&header, sizeof(PACKET_HEADER) ) )
	{
		//OnLogString( "º¸³»±â ¹öÆÛ°¡ ²Ë Ã¡À¸¹Ç·Î ¿¬°áÀ» ²÷½À´Ï´Ù." );
		Remove();
		return FALSE;
	}

	// º¸³»±â ¹öÆÛ¿¡ ¾²±â(dwNumberOfMessages°³ÀÇ ¸Þ¼¼Áö)
	for( i = 0; i < dwNumberOfMessages; ++i )
	{
		if( !m_pSendBuffer->Write( ppMsg[i], pwSize[i] ) )
		{
			//OnLogString( "º¸³»±â ¹öÆÛ°¡ ²Ë Ã¡À¸¹Ç·Î ¿¬°áÀ» ²÷½À´Ï´Ù." );
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
			IOCP¿¡ send¸¦ °É¾î ³õ´Â´Ù.
	@retval BOOL
			- º¸³»±â ¹öÆÛ°¡ ºñ¾îÀÖ°Å³ª º¸³¾ ÁØºñ°¡ ¾ÈµÈ »óÅÂÀÌ¸é ±×³É TRUE¸¦ ¸®ÅÏÇÑ´Ù.
			- send¿¡ ½ÇÆÐÇÏ¸é FALSE¸¦ ¸®ÅÏÇÑ´Ù.
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
			IOCP¿¡ recv¸¦ °É¾î ³õ´Â´Ù.
	@retval BOOL
			recv¿¡ ½ÇÆÐÇÏ¸é FALSE¸¦ ¸®ÅÏÇÑ´Ù.
*/
//=============================================================================================================================
BOOL Session::PreRecv(int nCode)
{
	WSABUF wsabuf;

	m_pRecvBuffer->GetRecvParam( (BYTE**)&wsabuf.buf, (int&)wsabuf.len );

	if(wsabuf.len == 0)
	{
		LOGERROR("½ÓÊÜ»º³åÇøÒÑÂú£¬ÔÝÊ±ÎÞ·¨½ÓÊÜÊý¾Ý[%d]", nCode);
		LOGERROR("½ÓÊÜ»º³åÇøÒç³ö,ip:%s,Õâ¸öÒªÃ´ÊÇ¶Ô·½·¢¿ìÁË£¬ÒªÃ´ÊÇ·þÎñÆ÷ÊÕÂýÁË£¬´¦ÀíÌ«Âý",GetIP());
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
	//Session ¿¡¼­ Recv Ptr¾ò¾î¿Í ¼Â¾÷..
	GetRecvBuffer()->GetRecvParam( &pRecvBuf, nLen );

	}catch(...)
	{
		LOGWARNING("Session::PreAccept1(),error!!! nStep = %u",nStep);
		return FALSE;
	}

	try{
	nStep = 3;
	//AcceptEx ¿ë ovl IO »ý¼º
	ZeroMemory( &m_acceptIoData, sizeof(OVERLAPPEDEX) );
	m_acceptIoData.dwOperationType	= ACCEPT_POSTED;
	m_acceptIoData.pSession			= this;				//ÀÚ½ÅÀÇ ptr

	//printf("[ACCEPT][%d] Session::PreAccept, m_bRemove=%d\n", (int)GetSocket(), (int)m_bRemove);

	nStep = 4;
	//AcceptEx¿¡ ¼ÒÄÏ Add
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
//modify code by ÁÖµÂÔ¨ begin 2011.5.19
//		g_StatisticalData.EraseMaxRecv(GetClientID());
//modify code by ÁÖµÂÔ¨ end
		InterlockedExchange( (long*)&m_bRemove, TRUE );
	}
}
