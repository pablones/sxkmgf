// ClientSocket.cpp
// paled, 2002.6.16

// ��������SOCKET���ӵĿͻ���һ�������������ڿͻ��˳���Ҳ��������С��Ϸ�������������ݷ�������
// ʹ�÷�������ʽʵ�֡�

#include "ClientSocket.h"
//using namespace world_kernel;

/////////////////////////////////////////////////////////////////////////////////
// ȫ�ֺ���
/////////////////////////////////////////////////////////////////////////////////
bool	ClientSocketInit()
{
	// ��ʼ������
	WSADATA		wsaData;
	int ret = WSAStartup(0x0002, &wsaData);			// 0x0101
	if(ret!=0)
	{
		LOGERROR("ERROR: Init WSAStartup() failed.");
		return false;
	}

	// ���汾
	if(wsaData.wVersion != 0x0002)				// 0x0101
	{
		LOGERROR("ERROR: WSAStartup Version not match 2.0");		// 1.1
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////
bool	ClientSocketFinal()
{
	WSACleanup();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////
// CClientSocket
/////////////////////////////////////////////////////////////////////////////////
CClientSocket::CClientSocket()
{
	m_sock		= INVALID_SOCKET;
	m_nLen		= 0;
	m_nSendLen	= 0;
	m_nState	= STATE_CLOSED;

	m_szIP[0]	= 0;
}

/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::Open(const char* szName, int nPort, int nBlockSecs /*= 0*/, int nSndBuf /*= 0*/, int nRcvBuf /*= 0*/)
{
	ASSERT(m_nState == STATE_CLOSED);

	SafeCopy(m_szIP, szName, IPSTR_SIZE);
	m_nPort		= nPort;
	m_nSndSize	= nSndBuf;
	m_nRcvSize	= nRcvBuf;

	if(m_nState == STATE_OPEN)
		return true;

	if(m_nState == STATE_CLOSED)
	{
#ifdef	ENCRYPT_ENABLE
		m_cEncryptSend.Init();
		m_cEncryptRecv.Init();
#endif
		ASSERT(m_sock == INVALID_SOCKET);
		// �����׽���
		m_sock = socket(PF_INET, SOCK_STREAM, 0);
		if(m_sock == INVALID_SOCKET)
		{
			LOGERROR("Init socket() failed.");
			return false;
		}

		// ��ʼ��
		m_nLen		= 0;
		// ����SOCKETΪKEEPALIVE
		int		optval=1;
		if(setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))
		{
			closesocket(m_sock);
			LOGERROR("setsockopt() set SO_KEEPALIVE failed.");
			return false;
		}

		// ����SENDBUG
				optval = m_nSndSize;
		if(m_nSndSize && setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char *) &optval, sizeof(optval)))
		{
			int err = WSAGetLastError();
			LOGERROR("setsockopt() set SO_SNDBUF failed[%d].", err);
			Close();
			return false;
		}

		// ����RECVBUG
				optval = m_nRcvSize;
		if(m_nRcvSize && setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char *) &optval, sizeof(optval)))
		{
			int err = WSAGetLastError();
			LOGERROR("setsockopt() set SO_RCVBUF failed[%d].", err);
			Close();
			return false;
		}

		// ����Ϊ��������ʽ
		unsigned long	i = 1;
		if(ioctlsocket(m_sock, FIONBIO, &i))
		{
			LOGERROR("Init ioctlsocket() failed.");
			Close();
			return false;
		}

		// ����->IP��ַ
		UINT	nAddr = inet_addr(m_szIP);			// ����Ϊ UINT, �Ա���in_addr����
		if(nAddr == INADDR_NONE)
		{
			hostent	* hp;
			hp = gethostbyname(m_szIP);
			if(hp == 0)
			{
				LOGERROR("�ʺŷ�������������.");
				Close();
				return false;
			}
			nAddr = *(UINT*)(hp->h_addr_list[0]);	// �� h_addr
		}

		sockaddr_in	addr_in;
		memset((void *)&addr_in, sizeof(addr_in), 0);
		addr_in.sin_family = AF_INET;
		addr_in.sin_port = htons(m_nPort);
		addr_in.sin_addr.s_addr = nAddr;
		int ret = connect(m_sock, (sockaddr *)&addr_in, sizeof(addr_in));
		if(ret == -1)
		{
			int err = WSAGetLastError();
			if(err != WSAEWOULDBLOCK)
			{
				Close();
				return false;
			}
		}

		m_nState = STATE_CONNECT;
	}

	ASSERT(m_nState == STATE_CONNECT);
	// �ظ���
	timeval timeout;
	timeout.tv_sec	= nBlockSecs;
	timeout.tv_usec	= 0;
	fd_set writeset, exceptset;
	FD_ZERO(&writeset);
	FD_ZERO(&exceptset);
	FD_SET(m_sock, &writeset);
	FD_SET(m_sock, &exceptset);

	int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
	if (ret > 0 && FD_ISSET(m_sock, &exceptset))
	{
		Close();
		return false;
	}
	else if(ret > 0 && FD_ISSET(m_sock, &writeset))
	{
		m_nState	= STATE_OPEN;
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::Open(int nBlockSecs /*= 0*/)
{
	ASSERT(m_szIP[0]);

	if(m_nState == STATE_OPEN)
		return true;

	if(m_nState == STATE_CLOSED)
	{
#ifdef	ENCRYPT_ENABLE
		m_cEncryptSend.Init();
		m_cEncryptRecv.Init();
		m_NewEncryptSend.Init();		//06.10.11��������......
#endif
		ASSERT(m_sock == INVALID_SOCKET);
		// �����׽���
		m_sock = socket(PF_INET, SOCK_STREAM, 0);
		if(m_sock == INVALID_SOCKET)
		{
			LOGERROR("Init socket() failed.");
			return false;
		}

		// ��ʼ��
		m_nLen		= 0;
		m_nSendLen	= 0;//06.10.11����
		memset( m_bufSendMsg,0,sizeof( m_bufSendMsg ) );//06.10.11����
		// ����SOCKETΪKEEPALIVE
		int		optval=1;
		if(setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))
		{
			closesocket(m_sock);
			LOGERROR("setsockopt() set SO_KEEPALIVE failed.");
			return false;
		}

		// ����SENDBUG
				optval = m_nSndSize;
		if(m_nSndSize && setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char *) &optval, sizeof(optval)))
		{
			int err = WSAGetLastError();
			LOGERROR("setsockopt() set SO_SNDBUF failed[%d].", err);
			Close();
			return false;
		}

		// ����RECVBUG
				optval = m_nRcvSize;
		if(m_nRcvSize && setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char *) &optval, sizeof(optval)))
		{
			int err = WSAGetLastError();
			LOGERROR("setsockopt() set SO_RCVBUF failed[%d].", err);
			Close();
			return false;
		}

		// ����Ϊ��������ʽ
		unsigned long	i = 1;
		if(ioctlsocket(m_sock, FIONBIO, &i))
		{
			LOGERROR("Init ioctlsocket() failed.");
			Close();
			return false;
		}

		// ����->IP��ַ
		UINT	nAddr = inet_addr(m_szIP);			// ����Ϊ UINT, �Ա���in_addr����
		if(nAddr == INADDR_NONE)
		{
			hostent	* hp;
			hp = gethostbyname(m_szIP);
			if(hp == 0)
			{
				LOGERROR("�ʺŷ�������������.");
				Close();
				return false;
			}
			nAddr = *(UINT*)(hp->h_addr_list[0]);	// �� h_addr
		}

		sockaddr_in	addr_in;
		memset((void *)&addr_in, sizeof(addr_in), 0);
		addr_in.sin_family = AF_INET;
		addr_in.sin_port = htons(m_nPort);
		addr_in.sin_addr.s_addr = nAddr;
		int ret = connect(m_sock, (sockaddr *)&addr_in, sizeof(addr_in));
		if(ret == -1)
		{
			int err = WSAGetLastError();
			if(err != WSAEWOULDBLOCK)
			{
				Close();
				return false;
			}
		}

		m_nState = STATE_CONNECT;
	}

	ASSERT(m_nState == STATE_CONNECT);
	// �ظ���
	timeval timeout;
	timeout.tv_sec	= nBlockSecs;
	timeout.tv_usec	= 0;
	fd_set writeset, exceptset;
	FD_ZERO(&writeset);
	FD_ZERO(&exceptset);
	FD_SET(m_sock, &writeset);
	FD_SET(m_sock, &exceptset);

	int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
	if (ret > 0 && FD_ISSET(m_sock, &exceptset))
	{
		Close();
		return false;
	}
	else if(ret > 0 && FD_ISSET(m_sock, &writeset))
	{
		m_nState	= STATE_OPEN;
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////
const char* CClientSocket::GetPacket(int* pBufSize, bool bFromNet /*= true*/)
{
	CHECKF(pBufSize);

	*pBufSize = 0;

	if(m_nState != STATE_OPEN)
		return NULL;

	if(bFromNet && m_sock != INVALID_SOCKET && m_nLen < RECV_BUFFER_SIZE)
	{
		int ret = recv(m_sock, m_bufMsg + m_nLen, RECV_BUFFER_SIZE - m_nLen, 0);
		//ashulan Debug------------------
//		int ashulanTest = WSAGetLastError();
//		char strTemp[512]; 
//		sprintf(strTemp,"\nret=%d lastError=%d",ret,ashulanTest);
//		::OutputDebugString(strTemp);
		//-------------------------------
		if(ret > 0)
		{
// #ifdef	ENCRYPT_ENABLE
// 				// ���ܹ���
// 				m_cEncryptRecv.Encrypt((unsigned char *)m_bufMsg + m_nLen, ret);
// #endif
			m_nLen += ret;
		}
		else if(ret == 0)
		{
			::LogSave("CClientSocket::GetPacket ret == 0 �ر��׽���,Զ�������ر���?,ip:%s",m_szIP);
			Close();
		}
		else if(ret < 0)
		{
			int err = WSAGetLastError();
			if(err != WSAEWOULDBLOCK)
			{
				::LogSave("CClientSocket::GetPacket ret < 0 �ر��׽���,���������?err:%d,ip:%s",err,m_szIP);
				Close();
			}
		}
	}

	*pBufSize	= m_nLen;
	return m_bufMsg;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::ClearPacket	(int nLen)
{
	ASSERT(nLen <= m_nLen);

	if(m_nState != STATE_OPEN)
		return false;

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

/////////////////////////////////////////////////////////////////////////////////
void CClientSocket::Close()
{
	if(m_nState != STATE_CLOSED)
	{
		if(m_sock != INVALID_SOCKET)
			closesocket(m_sock);
		m_sock		= INVALID_SOCKET;
		m_nState	= STATE_CLOSED;
	}
}

/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::SendPacket(const char* buf, int nLen, bool bFlush /*= false*/)
{
	if(m_nState != STATE_OPEN)
		return false;

	if(nLen + m_nSendLen > SEND_BUFFER_SIZE)
		Flush();

	if(nLen + m_nSendLen > SEND_BUFFER_SIZE)
	{
		LOGERROR("SOCKET���!");
		::LogSave("CClientSocket::SendPacket nLen + m_nSendLen > SEND_BUFFER_SIZE �ر��׽���,SOCKET���?nLen:%d,m_nSendLen:%d,ip:%s",nLen,m_nSendLen,m_szIP);
		Close();
		return false;
	}

	memcpy(m_bufSendMsg + m_nSendLen, buf, nLen);
	m_nSendLen	+= nLen;
// #ifdef	ENCRYPT_ENABLE
// 		// ���ܹ���
// 		m_cEncryptSend.Encrypt((unsigned char*)m_bufSendMsg + m_nSendLen - nLen, nLen);
// #endif

	if(bFlush)
		return Flush();

	return true;
}

bool CClientSocket::Send2StateServer(const char* buf, int nLen, bool bFlush)
{
	if(m_nState != STATE_OPEN)
		return false;

	if(nLen + m_nSendLen > SEND_BUFFER_SIZE)
		Flush();

	if(nLen + m_nSendLen > SEND_BUFFER_SIZE)
	{
		LOGERROR("SOCKET���!");
		Close();
		return false;
	}
	memcpy(m_bufSendMsg + m_nSendLen, buf, nLen);
	m_nSendLen	+= nLen;
// #ifdef	ENCRYPT_ENABLE
// 		// ���ܹ���
// 		m_NewEncryptSend.Encrypt((unsigned char*)m_bufSendMsg + m_nSendLen - nLen, nLen);
// #endif

	if(bFlush)
		return Flush();

	return true;
}
/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::Flush()				// return false : no empty
{
	if(m_nState != STATE_OPEN)
		return false;

	if(!m_nSendLen)
		return true;

	int ret = send(m_sock, m_bufSendMsg, m_nSendLen, 0);
	if(ret > 0)
	{
		if(m_nSendLen - ret > 0)
			memcpy(m_bufSendMsg, m_bufSendMsg + ret, m_nSendLen - ret);
		m_nSendLen	-= ret;
		ASSERT(m_nSendLen >= 0);

		return !m_nSendLen;
	}
	else // ret < 0
	{
		int err = WSAGetLastError();
		if(err != WSAEWOULDBLOCK)
		{
			::LogSave("CClientSocket::Flush ret <= 0 ��������ʱ������,�ر��׽���.err:%d,ip:%s",err,m_szIP);
		}
		Close();
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::IsOpen()
{
//	if(m_sock == INVALID_SOCKET)
//		return false;

	return (m_nState == STATE_OPEN);
}



