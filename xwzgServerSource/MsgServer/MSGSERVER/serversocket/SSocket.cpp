// ServerSocket.cpp
// paled, 2002.6.3

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */

#define FD_SETSIZE      2048				// 限制最多2000个客户端登录
#include <winsock2.h>
#include <windows.h>
#include <assert.h>
#include "EncryptServer.h"
#include "SSocket.h"
#include "../share/define.h"

#define ENCRYPT


/////////////////////////////////////////////////////////////////////////////////////////
// 全局函数
/////////////////////////////////////////////////////////////////////////////////////////
bool	ServerSocketInit()
{
	// 初始化网络
	WSADATA		wsaData;
	int ret = WSAStartup(0x0002, &wsaData);
	if(ret!=0)
	{
		LOGERROR("ERROR: Init WSAStartup() failed.");
		return false;
	}

	// 检查版本
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
// nSndBuf、nRcvBuf指SOCKET系统的缓冲区
CListenSocket::CListenSocket()
{
	m_sock = INVALID_SOCKET;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool		CListenSocket::Listen(int nPort, int nSndBuf /*= 0*/, int nRcvBuf /*= 0*/)		// 0为缺省值
{
	if(m_sock != INVALID_SOCKET)
	{
		Close();
		m_sock = INVALID_SOCKET;
	}

	// 创建主套接字
	SOCKADDR_IN		sin;
	m_sock=socket(AF_INET, SOCK_STREAM, 0);
	if(m_sock==INVALID_SOCKET)
	{
		LOGERROR("ERROR: Init socket() failed.");
		return false;
	}

	// 设置SOCKET为KEEPALIVE
	int		optval=1;
	if(setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))
	{
		closesocket(m_sock);
		LOGERROR("setsockopt() set SO_KEEPALIVE failed.");
		return false;
	}

	// 设置SENDBUG
			optval = nSndBuf;
	if(nSndBuf && setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char *) &optval, sizeof(optval)))
	{
		int err = WSAGetLastError();
		LOGERROR("setsockopt() set SO_SNDBUF failed[%d].", err);
		Close();
		return false;
	}

	// 设置RECVBUG
			optval = nRcvBuf;
	if(nRcvBuf && setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char *) &optval, sizeof(optval)))
	{
		int err = WSAGetLastError();
		LOGERROR("setsockopt() set SO_RCVBUF failed[%d].", err);
		Close();
		return false;
	}

	// 设置SOCKET为可重复使用
	if(setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(optval)))
	{
		closesocket(m_sock);
		LOGERROR("ERROR: Init setsockopt() failed.");
		return false;
	}

	// 绑定
	sin.sin_family=AF_INET;
	sin.sin_addr.S_un.S_addr=0;
	sin.sin_port=htons(nPort);
	if(bind(m_sock, (LPSOCKADDR) &sin, sizeof(sin)))
	{
		closesocket(m_sock);
		LOGERROR("ERROR: Init bind() failed.");
		return false;
	}

	// 设置为非阻塞方式
	unsigned long	i = 1;
	if(ioctlsocket(m_sock, FIONBIO, &i))
	{
		closesocket(m_sock);
		LOGERROR("ERROR: Init ioctlsocket() failed.");
		return false;
	}

	// keep_alive

	// 监听端口
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
//构造函数
CServerSocket::CServerSocket( SOCKET sock )
{ 
	m_sock = sock;
	
	m_nLen = 0; 
	ZeroMemory(m_bufMsg,sizeof(m_bufMsg));
//----------------07.5.21王玉波添加----begin-------------------
	m_nLeaveLen = 0;
	ZeroMemory( m_sendBuffer,sizeof( m_sendBuffer ) );
	//设置为非阻塞模式	
	unsigned long	i = 1;
	if( ioctlsocket( m_sock,FIONBIO,&i ) )
	{
		int nErrCode = WSAGetLastError();
		LOGERROR( "CServerSocket设置为非阻塞模式出错,ERR CODE %d",nErrCode );
	}
	//socket系统缓冲区
	int nSendSize = 0;			//系统默认值为128k	
	int nRecvSize = 8 * 1024;	//未更改，系统默认值
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
//王玉波
//发送数据到网络上，从应用程序缓冲区
//  [7/11/2007] 王玉波第二次修改，更改缓冲区为环形
int CServerSocket::SendPacket2Net()
{
//	CHECKF( m_nLeaveLen >= 0 );//小于0 ERROR
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
//				LOGERROR( "ERROR,SendPacket2Net send ret 0,客户端关闭了" );
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
//					LOGERROR( "ERROR,SendPacket2Net,网络异常,socket关闭,ErrCode:%d",nErrCode );
//				}
//				Close( true );
//				return 0;
//			}
//		}
//	}
//	return 0;
//  [7/11/2007]-------------王玉波注释以上并添加

	CHECKF( GetSendLeaveLen() >= 0 );//小于0 ERROR,以下必有要发送的数据
	if( m_sock != INVALID_SOCKET )
	{
		clock_t clkDebug = clock();//debug
		if( m_nTrailPos > m_nHeadPos ) //正常情况
		{
			DEBUG_TRY
			int nRet = send( m_sock,m_sendBuffer + m_nHeadPos,m_nTrailPos - m_nHeadPos,0 );
			if( nRet > 0 )
			{
					//更改头指针
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
		else//发送缓冲区分为了两个部分，没有等于的情况
		{
			DEBUG_TRY
			int nFirstLen = SEND_BUFFER_SIZE - m_nHeadPos;
			int nRet = send( m_sock,m_sendBuffer + m_nHeadPos,nFirstLen,0 );
			if( nRet == nFirstLen )//继续发送第二部分
			{
				//移动头指针
				ASSERT( (m_nHeadPos + nRet) == SEND_BUFFER_SIZE );
				m_nHeadPos = 0;
				if( m_nTrailPos - m_nHeadPos > 0 )//有可能尾指针在缓冲区0位置
				{
					int nRet2 = send( m_sock,m_sendBuffer + m_nHeadPos,m_nTrailPos - m_nHeadPos,0 );
					if( nRet2 > 0 )
					{
						//更改头指针
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
			else if( nRet > 0 )//第一段数据发了一部分
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
// 				// 解密功能
// 				m_cEncryptRecv.Encrypt((unsigned char *)m_bufMsg + m_nLen, ret);
// #endif
			m_nLen += ret;
		}
		else if(ret == 0)
		{
			LOGDEBUG("DEBUG：客户端主动关闭SOCKET。");
			Close();
		}
		else if(ret < 0)
		{
			int err = WSAGetLastError();
			if(err != WSAEWOULDBLOCK)
			{
				if( err != WSAECONNRESET )
				{
					LOGERROR( "ERROR,GetPacket(),网络异常,ErrCode:%d",err );
				}
				//LOGERROR("DEBUG：网络异常，SOCKET出错。将关闭socket");
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
		LOGDEBUG("DEBUG：SOCKET关闭中。");
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;
		LOGDEBUG("DEBUG：SOCKET关闭了。");
		//? bNoLinger暂未使用
	}
//	m_nLen = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
DWORD	CServerSocket::GetPeerIP()
{
	// 取对方IP
	sockaddr_in	inAddr;
	memset(&inAddr, 0, sizeof(inAddr));
	int		nLen = sizeof(inAddr);
	if(getpeername(m_sock, (sockaddr *)&inAddr, &nLen))
	{
		int err = WSAGetLastError();
		LOGERROR("ERROR: getpeername()错误.");
		Close(true);
		return NULL;
	}

	return	inAddr.sin_addr.S_un.S_addr;
}
////////////////////////////////////////////////////////////////////////////
// 王玉波添加,获得客户端的IP,字符串形式 [6/1/2007]
bool CServerSocket::GetPeerStrIP( char* szClientIP )
{
	if( !szClientIP )
	{
		return false;
	}
	// 取对方IP
	sockaddr_in	inAddr;
	memset(&inAddr, 0, sizeof(inAddr));
	int		nLen = sizeof(inAddr);
	if(getpeername(m_sock, (sockaddr *)&inAddr, &nLen))
	{
		int err = WSAGetLastError();
		LOGERROR( "ERROR: getpeername()错误.ErrCode:%d",err );
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
	// 设置SENDBUG
//07.5.21王玉波注释并修改
//	int		optval = nSndBuf;
	int optval = 1;	
	if(nSndBuf && setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char *) &optval, sizeof(optval)))
	{
		int err = WSAGetLastError();
		LOGERROR("setsockopt() set SO_SNDBUF failed[%d].", err);
		Close();
		return false;
	}

	// 设置RECVBUG
//07.5.21王玉波注释并修改
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
		LOGWARNING( "CopyDataToBuffer Error,socket 关闭!" );
		Close( true );
		return false;
	}
	if( nLen > ( SEND_BUFFER_SIZE - m_nTrailPos ) )//遇到截断了
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
		//变尾部
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
// 	// 加密功能
// 	char	buf[MAX_PACKETSIZE];
// 	memcpy(buf, pack, nLen);
// 	pack = buf;		//? 替换掉参数的值
// 	m_cEncryptSend.Encrypt((unsigned char *)pack, nLen);
// #endif
//----------------07.5.21王玉波添加-------begin-------------------------------------
	if( GetSendLeaveLen() + nLen >= SEND_BUFFER_SIZE )//加上此次大于等于缓冲区
	{
//  [7/11/2007]----------王玉波注释，改变环状缓冲区-----------
//		LOGWARNING( "发送效率下降,m_nLeaveLen + nLen >= SEND_BUFFER_SIZE" );
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
//			else//发送的空间不足以容纳此次要添加的
//			{
//				if( nRet > 0 )
//				{
//					memcpy( m_sendBuffer,m_sendBuffer + nRet,m_nLeaveLen - nRet );
//					m_nLeaveLen -= nRet;
//				}
//				LOGWARNING( "SendPacket(),发送缓冲区不足，此次发送小于添加长度，丢弃此包" );
//				return false;
//			}
//		}//发生错误
//		else
//		{
//			int nErrCode = WSAGetLastError();
//			if( WSAEWOULDBLOCK == nErrCode )
//			{
//				//不处理，丢弃此包
//				LOGWARNING( "SendPacket(),发送缓冲区不足,发送返回WSAEWOULDBLOCK,丢弃此包!" );
//			}
//			else//网络错误
//			{
//				Close( true );
//				return false;
//			}
//		}
//--------------------end------------------------------------------
		//调用一次发送
		SendPacket2Net();
		//再判断一次
		if( GetSendLeaveLen() + nLen > SEND_BUFFER_SIZE )//缓冲区不足以容纳此次数据COPY
		{
			//关闭套接字
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




//-----------07.1.30王玉波注释并修改--------begin---------------------------------
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




