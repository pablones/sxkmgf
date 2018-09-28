// ServerSocket.cpp
// paled, 2002.6.3

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */

#define FD_SETSIZE      2048				// �������2000���ͻ��˵�¼
#include <winsock2.h>
#include <windows.h>
#include <assert.h>
#include "EncryptServer.h"
#include "SSocket.h"
#include "../share/define.h"

#define ENCRYPT


/////////////////////////////////////////////////////////////////////////////////////////
// ȫ�ֺ���
/////////////////////////////////////////////////////////////////////////////////////////
bool	ServerSocketInit()
{
	// ��ʼ������
	WSADATA		wsaData;
	int ret = WSAStartup(0x0002, &wsaData);
	if(ret!=0)
	{
		LOGERROR("ERROR: Init WSAStartup() failed.");
		return false;
	}

	// ���汾
	if(LOBYTE(wsaData.wVersion) != 0x02 || HIBYTE(wsaData.wVersion) != 0x00)
	{
		LOGERROR("ERROR: WSAStartup Version not match 2.0");
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool	ServerSocketFinal()
{
	WSACleanup();
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////
// class	CListenSocket
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// nSndBuf��nRcvBufָSOCKETϵͳ�Ļ�����
CListenSocket::CListenSocket()
{
	m_sock = INVALID_SOCKET;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool		CListenSocket::Listen(int nPort, int nSndBuf /*= 0*/, int nRcvBuf /*= 0*/)		// 0Ϊȱʡֵ
{
	if(m_sock != INVALID_SOCKET)
	{
		Close();
		m_sock = INVALID_SOCKET;
	}

	// �������׽���
	SOCKADDR_IN		sin;
	m_sock=socket(AF_INET, SOCK_STREAM, 0);
	if(m_sock==INVALID_SOCKET)
	{
		LOGERROR("ERROR: Init socket() failed.");
		return false;
	}

	// ����SOCKETΪKEEPALIVE
	int		optval=1;
	if(setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))
	{
		closesocket(m_sock);
		LOGERROR("setsockopt() set SO_KEEPALIVE failed.");
		return false;
	}

	// ����SENDBUG
			optval = nSndBuf;
	if(nSndBuf && setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char *) &optval, sizeof(optval)))
	{
		int err = WSAGetLastError();
		LOGERROR("setsockopt() set SO_SNDBUF failed[%d].", err);
		Close();
		return false;
	}

	// ����RECVBUG
			optval = nRcvBuf;
	if(nRcvBuf && setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char *) &optval, sizeof(optval)))
	{
		int err = WSAGetLastError();
		LOGERROR("setsockopt() set SO_RCVBUF failed[%d].", err);
		Close();
		return false;
	}

	// ����SOCKETΪ���ظ�ʹ��
	if(setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(optval)))
	{
		closesocket(m_sock);
		LOGERROR("ERROR: Init setsockopt() failed.");
		return false;
	}

	// ��
	sin.sin_family=AF_INET;
	sin.sin_addr.S_un.S_addr=0;
	sin.sin_port=htons(nPort);
	if(bind(m_sock, (LPSOCKADDR) &sin, sizeof(sin)))
	{
		closesocket(m_sock);
		LOGERROR("ERROR: Init bind() failed.");
		return false;
	}

	// ����Ϊ��������ʽ
	unsigned long	i = 1;
	if(ioctlsocket(m_sock, FIONBIO, &i))
	{
		closesocket(m_sock);
		LOGERROR("ERROR: Init ioctlsocket() failed.");
		return false;
	}

	// keep_alive

	// �����˿�
	if( listen( m_sock,SOMAXCONN ) )
	{
		closesocket(m_sock);
		LOGERROR("ERROR: Init listen() failed.");
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
SOCKET		CListenSocket::Accept()
{

	fd_set	readmask;
	FD_ZERO(&readmask);
	FD_SET(m_sock, &readmask);

	struct timeval	timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if(select(FD_SETSIZE, &readmask, (fd_set *) 0, (fd_set *) 0, &timeout) > 0)
	{
		if(FD_ISSET(m_sock, &readmask))
		{
			struct sockaddr_in	addr;
			int	   len = sizeof(addr);
			SOCKET newsock = accept(m_sock, (sockaddr *) &addr, (int *) &len);

			if(newsock == INVALID_SOCKET)
				LOGERROR("ERROR: accept() failed.");
			else
				return newsock;
		}
	}
	return INVALID_SOCKET;
}

/////////////////////////////////////////////////////////////////////////////////////////
void		CListenSocket::Close()
{
	if(m_sock != INVALID_SOCKET)
	{
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// class	CServerSocket
/////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CServerSocket::CServerSocket( SOCKET sock )
{ 
	m_sock = sock;
	
	m_nLen = 0; 
	ZeroMemory(m_bufMsg,sizeof(m_bufMsg));
//----------------07.5.21�������----begin-------------------
	m_nLeaveLen = 0;
	ZeroMemory( m_sendBuffer,sizeof( m_sendBuffer ) );
	//����Ϊ������ģʽ	
	unsigned long	i = 1;
	if( ioctlsocket( m_sock,FIONBIO,&i ) )
	{
		int nErrCode = WSAGetLastError();
		LOGERROR( "CServerSocket����Ϊ������ģʽ����,ERR CODE %d",nErrCode );
	}
	//socketϵͳ������
	int nSendSize = 0;			//ϵͳĬ��ֵΪ128k	
	int nRecvSize = 8 * 1024;	//δ���ģ�ϵͳĬ��ֵ
	SetBufSize( nSendSize,nRecvSize );
//-------------------------------------end--------------------
	m_nHeadPos = 0;
	m_nTrailPos = 0;
}
CServerSocket::~CServerSocket()
{ 
	Close(); 
}
//07.5.21
//����
//�������ݵ������ϣ���Ӧ�ó��򻺳���
//  [7/11/2007] ���񲨵ڶ����޸ģ����Ļ�����Ϊ����
int CServerSocket::SendPacket2Net()
{
//	CHECKF( m_nLeaveLen >= 0 );//С��0 ERROR
//	if( m_sock != INVALID_SOCKET && m_nLeaveLen )
//	{
//		clock_t clkDebug = clock();//debug
//		int nRet = send( m_sock,m_sendBuffer,m_nLeaveLen,0 );
//		if( SOCKET_ERROR != nRet )
//		{
//			if( nRet > 0 )
//			{
//				memcpy( m_sendBuffer,m_sendBuffer + nRet,m_nLeaveLen - nRet );
//				m_nLeaveLen -= nRet;
//				
//				return nRet;
//
//			}
//			else
//			{
//				LOGERROR( "ERROR,SendPacket2Net send ret 0,�ͻ��˹ر���" );
//				Close( true );
//				return 0;
//			}
//
//		}
//		else
//		{
//			int nErrCode = WSAGetLastError();
//			if( nErrCode != WSAEWOULDBLOCK )
//			{
//				if( nErrCode != WSAECONNRESET )
//				{
//					LOGERROR( "ERROR,SendPacket2Net,�����쳣,socket�ر�,ErrCode:%d",nErrCode );
//				}
//				Close( true );
//				return 0;
//			}
//		}
//	}
//	return 0;
//  [7/11/2007]-------------����ע�����ϲ����

	CHECKF( GetSendLeaveLen() >= 0 );//С��0 ERROR,���±���Ҫ���͵�����
	if( m_sock != INVALID_SOCKET )
	{
		clock_t clkDebug = clock();//debug
		if( m_nTrailPos > m_nHeadPos ) //�������
		{
			DEBUG_TRY
			int nRet = send( m_sock,m_sendBuffer + m_nHeadPos,m_nTrailPos - m_nHeadPos,0 );
			if( nRet > 0 )
			{
					//����ͷָ��
				m_nHeadPos += nRet;
				m_nHeadPos %= SEND_BUFFER_SIZE;
				return nRet;	
			}
			else
			{
				ProcessSendFail( nRet );
				return 0;
			}	
			DEBUG_CATCH( "SendPacket2Net m_nTrailPos > m_nHeadPos catch!" );
		}
		else//���ͻ�������Ϊ���������֣�û�е��ڵ����
		{
			DEBUG_TRY
			int nFirstLen = SEND_BUFFER_SIZE - m_nHeadPos;
			int nRet = send( m_sock,m_sendBuffer + m_nHeadPos,nFirstLen,0 );
			if( nRet == nFirstLen )//�������͵ڶ�����
			{
				//�ƶ�ͷָ��
				ASSERT( (m_nHeadPos + nRet) == SEND_BUFFER_SIZE );
				m_nHeadPos = 0;
				if( m_nTrailPos - m_nHeadPos > 0 )//�п���βָ���ڻ�����0λ��
				{
					int nRet2 = send( m_sock,m_sendBuffer + m_nHeadPos,m_nTrailPos - m_nHeadPos,0 );
					if( nRet2 > 0 )
					{
						//����ͷָ��
						m_nHeadPos += nRet2;
						m_nHeadPos %= SEND_BUFFER_SIZE;
						return nRet2;
						
					}
					else
					{
						ProcessSendFail( nRet2 );
						return 0;
					}
					
				}
			}
			else if( nRet > 0 )//��һ�����ݷ���һ����
			{
				m_nHeadPos += nRet;
				m_nHeadPos %= SEND_BUFFER_SIZE;
				return nRet;
			}
			else 
			{
				ProcessSendFail( nRet );
				return 0;
			}
			DEBUG_CATCH( "SendPacket2Net m_nTrailPos < m_nHeadPos catch!" );//
		}
	}
	return 0;
}
const char* CServerSocket::GetPacket(int* pLen, BOOL bFromNet /*= true*/)
{
	CHECKF(pLen);

	*pLen = 0;

	if(bFromNet && m_sock != INVALID_SOCKET && m_nLen < RECV_BUFFER_SIZE)
	{
		int ret = recv(m_sock, m_bufMsg + m_nLen, RECV_BUFFER_SIZE - m_nLen, 0);
		if(ret > 0)
		{
// #ifdef	ENCRYPT
// 				// ���ܹ���
// 				m_cEncryptRecv.Encrypt((unsigned char *)m_bufMsg + m_nLen, ret);
// #endif
			m_nLen += ret;
		}
		else if(ret == 0)
		{
			LOGDEBUG("DEBUG���ͻ��������ر�SOCKET��");
			Close();
		}
		else if(ret < 0)
		{
			int err = WSAGetLastError();
			if(err != WSAEWOULDBLOCK)
			{
				if( err != WSAECONNRESET )
				{
					LOGERROR( "ERROR,GetPacket(),�����쳣,ErrCode:%d",err );
				}
				//LOGERROR("DEBUG�������쳣��SOCKET�������ر�socket");
				Close(true);
			}
		}
	}

	*pLen	= m_nLen;
	return m_bufMsg;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool CServerSocket::ClearPacket	(int nLen)
{
	ASSERT(nLen <= m_nLen);

	if(m_nLen - nLen > 0)
		memcpy(m_bufMsg, m_bufMsg + nLen, m_nLen - nLen);

	m_nLen	-= nLen;
	if(m_nLen < 0)
	{
		m_nLen = 0;
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//bool CServerSocket::IsOpen()
//{
//	if(m_sock == INVALID_SOCKET)
//		return false;
//
//	return true;
//}

/////////////////////////////////////////////////////////////////////////////////////////
void CServerSocket::Close(BOOL bNoLinger /*= false*/)
{
	if(m_sock != INVALID_SOCKET)
	{
		LOGDEBUG("DEBUG��SOCKET�ر��С�");
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;
		LOGDEBUG("DEBUG��SOCKET�ر��ˡ�");
		//? bNoLinger��δʹ��
	}
//	m_nLen = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
DWORD	CServerSocket::GetPeerIP()
{
	// ȡ�Է�IP
	sockaddr_in	inAddr;
	memset(&inAddr, 0, sizeof(inAddr));
	int		nLen = sizeof(inAddr);
	if(getpeername(m_sock, (sockaddr *)&inAddr, &nLen))
	{
		int err = WSAGetLastError();
		LOGERROR("ERROR: getpeername()����.");
		Close(true);
		return NULL;
	}

	return	inAddr.sin_addr.S_un.S_addr;
}
////////////////////////////////////////////////////////////////////////////
// �������,��ÿͻ��˵�IP,�ַ�����ʽ [6/1/2007]
bool CServerSocket::GetPeerStrIP( char* szClientIP )
{
	if( !szClientIP )
	{
		return false;
	}
	// ȡ�Է�IP
	sockaddr_in	inAddr;
	memset(&inAddr, 0, sizeof(inAddr));
	int		nLen = sizeof(inAddr);
	if(getpeername(m_sock, (sockaddr *)&inAddr, &nLen))
	{
		int err = WSAGetLastError();
		LOGERROR( "ERROR: getpeername()����.ErrCode:%d",err );
		Close(true);
		return false;
	}
	sprintf( szClientIP,"%d.%d.%d.%d",inAddr.sin_addr.S_un.S_un_b.s_b1,
		inAddr.sin_addr.S_un.S_un_b.s_b2,
		inAddr.sin_addr.S_un.S_un_b.s_b3,
		inAddr.sin_addr.S_un.S_un_b.s_b4 );
	
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////
int	CServerSocket::SetBufSize(int nSndBuf, int nRcvBuf)
{
	// ����SENDBUG
//07.5.21����ע�Ͳ��޸�
//	int		optval = nSndBuf;
	int optval = 1;	
	if(nSndBuf && setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char *) &optval, sizeof(optval)))
	{
		int err = WSAGetLastError();
		LOGERROR("setsockopt() set SO_SNDBUF failed[%d].", err);
		Close();
		return false;
	}

	// ����RECVBUG
//07.5.21����ע�Ͳ��޸�
//	optval = nRcvBuf;
	optval = 1;	

	if(nRcvBuf && setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char *) &optval, sizeof(optval)))
	{
		int err = WSAGetLastError();
		LOGERROR("setsockopt() set SO_RCVBUF failed[%d].", err);
		Close();
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
// static 
CServerSocket* CServerSocket::CreateNew(SOCKET sock)
{
	CServerSocket* pSocket = ::new CServerSocket(sock);

// #ifdef	ENCRYPT
// 		pSocket->m_cEncryptSend.Init();
// 		pSocket->m_cEncryptRecv.Init();
// #endif
	return	pSocket;
}

/////////////////////////////////////////////////////////////////////////////////////////
void CServerSocket::Destroy(CServerSocket* pSocket)
{
	::delete	pSocket;
}
/////////////////////////////////////////////////////////////////////////////////////////
int CServerSocket::GetSendLeaveLen()
{
	if( m_nTrailPos >= m_nHeadPos )
	{
		return ( m_nTrailPos - m_nHeadPos );
	}
	else
	{
		return ( SEND_BUFFER_SIZE - m_nHeadPos + m_nTrailPos );
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
bool CServerSocket::CopyDataToBuffer( const char* pSource,int nLen )
{
	if( GetSendLeaveLen() + nLen > SEND_BUFFER_SIZE )
	{
		LOGWARNING( "CopyDataToBuffer Error,socket �ر�!" );
		Close( true );
		return false;
	}
	if( nLen > ( SEND_BUFFER_SIZE - m_nTrailPos ) )//�����ض���
	{
		int nFirstLen = SEND_BUFFER_SIZE - m_nTrailPos;
		int nSecondLen = nLen - nFirstLen;
		memcpy( m_sendBuffer + m_nTrailPos,pSource,nFirstLen );
		m_nTrailPos += nFirstLen;
		m_nTrailPos %= SEND_BUFFER_SIZE;
		memcpy( m_sendBuffer + m_nTrailPos,pSource + nFirstLen,nSecondLen );
		m_nTrailPos += nSecondLen;
		m_nTrailPos %= SEND_BUFFER_SIZE;

	}
	else
	{
		memcpy( m_sendBuffer + m_nTrailPos,pSource,nLen );
		//��β��
		m_nTrailPos += nLen;
		m_nTrailPos %= SEND_BUFFER_SIZE;
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool CServerSocket::SendPacket(const char* pack, int nLen)
{	
	OBJID idPack = *((unsigned short*)pack+1);
// #ifdef	ENCRYPT		
// 	// ���ܹ���
// 	char	buf[MAX_PACKETSIZE];
// 	memcpy(buf, pack, nLen);
// 	pack = buf;		//? �滻��������ֵ
// 	m_cEncryptSend.Encrypt((unsigned char *)pack, nLen);
// #endif
//----------------07.5.21�������-------begin-------------------------------------
	if( GetSendLeaveLen() + nLen >= SEND_BUFFER_SIZE )//���ϴ˴δ��ڵ��ڻ�����
	{
//  [7/11/2007]----------����ע�ͣ��ı价״������-----------
//		LOGWARNING( "����Ч���½�,m_nLeaveLen + nLen >= SEND_BUFFER_SIZE" );
//		int nRet = send( m_sock,m_sendBuffer,m_nLeaveLen,0 );
//		if( nRet != SOCKET_ERROR )
//		{
//			if( nRet >= nLen )//ok
//			{
//				memcpy( m_sendBuffer,m_sendBuffer + nRet,m_nLeaveLen - nRet );
//				m_nLeaveLen -= nRet;
//				memcpy( m_sendBuffer + m_nLeaveLen,pack,nLen );
//				m_nLeaveLen += nLen;
//			}
//			else//���͵Ŀռ䲻�������ɴ˴�Ҫ��ӵ�
//			{
//				if( nRet > 0 )
//				{
//					memcpy( m_sendBuffer,m_sendBuffer + nRet,m_nLeaveLen - nRet );
//					m_nLeaveLen -= nRet;
//				}
//				LOGWARNING( "SendPacket(),���ͻ��������㣬�˴η���С����ӳ��ȣ������˰�" );
//				return false;
//			}
//		}//��������
//		else
//		{
//			int nErrCode = WSAGetLastError();
//			if( WSAEWOULDBLOCK == nErrCode )
//			{
//				//�����������˰�
//				LOGWARNING( "SendPacket(),���ͻ���������,���ͷ���WSAEWOULDBLOCK,�����˰�!" );
//			}
//			else//�������
//			{
//				Close( true );
//				return false;
//			}
//		}
//--------------------end------------------------------------------
		//����һ�η���
		SendPacket2Net();
		//���ж�һ��
		if( GetSendLeaveLen() + nLen > SEND_BUFFER_SIZE )//���������������ɴ˴�����COPY
		{
			//�ر��׽���
			LOGWARNING( "SendPacket ,send buff not enough! close socket" );
			Close( true );
			return false;
		}
	}
	//	memcpy( m_sendBuffer + m_nLeaveLen,pack,nLen );
	//	m_nLeaveLen += nLen;
	CopyDataToBuffer( pack,nLen );	
	return true;

//----------------------------------------end---------------------------------------




//-----------07.1.30����ע�Ͳ��޸�--------begin---------------------------------
//	int ret = send(m_sock, pack, nLen, 0);
//	if(ret == nLen)
//	{
//		return true;
//	}
//	else
//	{
//		if( WSAGetLastError() == WSAEWOULDBLOCK )
//		{
//			LOGWARNING( "SendPacket err WOULDBLOCK id:%d,len:%d",idPack,nLen );
//		}
//		Close(true);
//		return false;
//	}
//-----------------------------------------end--------------------------------------------
}




