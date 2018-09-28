// �����̲߳���
// �ɽ���, 2001.10.10

#include "allheads.h"
#include "ListenSocket.h"

CListenSocket::CListenSocket(u_short nPort, int nSndBuf /*= 0*/)	// 0: ȱʡ
		 : m_nPort(nPort), m_nSndBuf(nSndBuf)
{
	m_sockListen	= INVALID_SOCKET;
	m_bState		= false;
//	m_nPort			= nPort;
	m_tCloseTime	= 0;
}

CListenSocket::~CListenSocket()
{
	Close();
}

//	��������˼���SOCKET������ѡ���뿴���롣ʧ��ʱ����false��
bool	CListenSocket::Open()		// ���ظ�����
{
	if(m_bState)
		return true;

//*
	// ��ʼ������
	WSADATA		wsaData;
	int	err;
	if((err = WSAStartup(0x0002, &wsaData)) != 0)
	{
		LOGERROR("Init WSAStartup() failed[%d].", err);
		return FALSE;
	}

	// �����쳣�ж��ź�
//	signal(SIGINT, ::CloseListenSock);
//	signal(SIGABRT, ::CloseListenSock);
//	signal(SIGTERM, ::CloseListenSock);

	// ���汾
	if(wsaData.wVersion != 0x0002)
	{
//		WSACleanup();
		LOGERROR("WSAStartup Version[0x%04X] not match 2.0", wsaData.wVersion);
		return FALSE;
	}
//*/
	// �������׽���
	m_sockListen=socket(AF_INET, SOCK_STREAM, 0);
	if(m_sockListen==INVALID_SOCKET)
	{
		int err = WSAGetLastError();
//		WSACleanup();
		LOGERROR("Init socket() failed[%d].", err);
		return FALSE;
	}
	else
	{
		InterlockedIncrement(&s_nSocketCount);
		m_bState = true;
	}

	// ����SOCKET��REUSEADDR
	int		optval = 1;
	if(setsockopt(m_sockListen, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(optval)))
	{
		int err = WSAGetLastError();
		Close();
		LOGERROR("Init setsockopt() SO_REUSEADDR failed[%d].", err);
		return FALSE;
	}

	// ����SOCKET��KEEPALIVE
	optval = 1;
	if(setsockopt(m_sockListen, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))
	{
		int err = WSAGetLastError();
		Close();
		LOGERROR("Init setsockopt() SO_KEEPALIVE failed[%d].", err);
		return FALSE;
	}
//*
	// ����SOCKET��SNDBUF
	if(m_nSndBuf)		// ��0��ָ��SNDBUG
	{
		optval = m_nSndBuf;
		if(setsockopt(m_sockListen, SOL_SOCKET, SO_SNDBUF, (char *) &optval, sizeof(optval)))
		{
			int err = WSAGetLastError();
			Close();
			LOGERROR("Init setsockopt() SO_SNDBUF failed[%d].", err);
			return FALSE;
		}

		// ��ȡSOCKET��SNDBUF
		int ret = sizeof(optval);
		if(getsockopt(m_sockListen, SOL_SOCKET, SO_SNDBUF, (char *) &optval, &ret) == 0)
		{
			LOGMSG("�ʺ�SOCKET�ڲ����ͻ������ߴ�Ϊ[%d]K.", optval/1024);
		}
	}
//*/
	// ��
	CString s;
	s.Format("%d",m_nPort);
//	AfxMessageBox(s);
	TRACE("\n%d\n",m_nPort);
	SOCKADDR_IN		sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family		= AF_INET;
	sin.sin_addr.s_addr	= 0;
	sin.sin_port		= htons(m_nPort);
	if(bind(m_sockListen, (LPSOCKADDR) &sin, sizeof(sin)))
	{
		int err = WSAGetLastError();
		Close();
		LOGERROR("Init bind() failed[%d].", err);
		return FALSE;
	}

	// ����Ϊ��������ʽ
	//*
	unsigned long	i = 1;
	if(ioctlsocket(m_sockListen, FIONBIO, &i))
	{
		int err = WSAGetLastError();
		Close();
		LOGERROR("Init ioctlsocket() failed[%d].", err);
		return FALSE;
	}
	//*/

	// �����˿�
	if(listen(m_sockListen, SOMAXCONN))		// SOMAXCONN: WIN�ĺ궨��
	{
		int err = WSAGetLastError();
		Close();
		LOGERROR("Init listen() failed[%d].", err);
		return FALSE;
	}

	return TRUE;
}

bool	CListenSocket::Rebuild()
{
	if(!m_bState)
	{
		if(clock() < m_tCloseTime + REBUILDLISTENDELAYSEC*CLOCKS_PER_SEC)
			return false;
		else
		{
			LOGMSG("��������������������SOCKET[%d]���½��� . . .", m_nPort);
			if(Open())
			{
				LOGMSG(". . . ����SOCKET�ؽ��ɹ���");
				PrintText("����SOCKET %d ʧ�ܣ��ؽ�SOCKET�ɹ�������˼���������...", m_nPort);
			}
			else
			{
				LOGERROR(". . . ����SOCKET�ؽ�ʧ�ܡ�����˽��޷�����������");
				PrintText("����SOCKET %d ����%d ����ؽ�SOCKET������˼���������...", m_nPort, REBUILDLISTENDELAYSEC);		// nDelaySec);
				return false;
			}
			m_tCloseTime = clock();
		}
	}

	return true;
}

//	����ʱ����INVALID_SOCKET
SOCKET	CListenSocket::Accept(u_long & nClientIP)
{
/*
	// �ؽ�SOCKET
	if(!m_bState)
	{
		if(clock() < m_tCloseTime + REBUILDLISTENDELAYSEC*CLOCKS_PER_SEC)
			return INVALID_SOCKET;
		else
		{
			LOGMSG("��������������������SOCKET[%d]���½��� . . .", m_nPort);
			if(Open())
			{
				LOGMSG(". . . ����SOCKET�ؽ��ɹ���");
				PrintText("����SOCKET[%d]ʧ�ܣ��ؽ�SOCKET�ɹ�������˼���������...", m_nPort);
			}
			else
			{
				LOGERROR(". . . ����SOCKET�ؽ�ʧ�ܡ�����˽��޷�����������");
				PrintText("����SOCKET[%d]����[%d]����ؽ�SOCKET������˼���������...", m_nPort, REBUILDLISTENDELAYSEC);		// nDelaySec);
			}
			m_tCloseTime = clock();
		}
	}
//*/
	if(!m_bState)
	{
		LOGERROR("ACCEPT�ڲ��쳣a1");
		return INVALID_SOCKET;
	}

	// ACCEPT
	struct sockaddr_in	addr;
	memset(&addr, 0, sizeof(addr));
	int	   len = sizeof(addr);
	SOCKET newsock = accept(m_sockListen, (sockaddr *) &addr, (int *) &len);		// ���յ��Է��ĵ�ͼ����ʹ��
#ifdef	PROFILE_X
			// ����Accept�ٶ�(ѭ���ٶ�)
			const int	nTimes2 = ACCEPTPROFILESEC;		// ÿ30��ͳ��һ��ACCEPT�ٶ�
			static	clock_t tNextTime2 = clock() + nTimes2 * CLOCKS_PER_SEC;	//? ����һ�������̣߳��޹����ͻ
			static long	nCount2	= 0;								//? ����һ�������̣߳��޹����ͻ
			if(clock() >= tNextTime2)
			{
				LOGPROFILE("ÿ[%d]��ִ����[%d]��Accept()", nTimes2, InterlockedExchange(&nCount2, 0));
				tNextTime2 = clock() + nTimes2 * CLOCKS_PER_SEC;
			}
			else
			{
				InterlockedIncrement(&nCount2);
			}
#endif	// PROFILE
	if(newsock == INVALID_SOCKET)
	{
		// �������
		int err = WSAGetLastError();
		if(err != WSAEWOULDBLOCK)
		{
			PrintText("����SOCKET %d ʧ�ܣ�%s ����ؽ�SOCKET.", m_nPort, REBUILDLISTENDELAYSEC);
			LOGERROR("����SOCKET[%d]ʧ��[%d]��[%s]����ؽ�SOCKET.", m_nPort, err, REBUILDLISTENDELAYSEC);
			Close();
		}
		else
			LOGERROR("ACCEPT�ڲ��쳣a2");
		return INVALID_SOCKET;
	}
	else
	{
		nClientIP = addr.sin_addr.S_un.S_addr;
		InterlockedIncrement(&s_nSocketCount);
	}

	// ���SOCKET�Ƿ��ѹر�
	fd_set	readmask;
	FD_ZERO(&readmask);
	FD_SET(newsock, &readmask);
	struct timeval	timeout = {0, 0};
/*
	char nTemp;
	if(select(FD_SETSIZE, &readmask, (fd_set *) 0, (fd_set *) 0, &timeout) 
			&& recv(newsock, &nTemp, 1, MSG_PEEK) == 0)
	{
#ifdef	ALPHA_X
		LOGMSG("ACCEPTһ����SOCKET����ЧSOCKET��");
#endif
		closesocket(newsock);
		InterlockedDecrement(&s_nSocketCount);
		return INVALID_SOCKET;
	}
//else*/
//*
	fd_set	exceptmask;
	FD_ZERO(&exceptmask);
	FD_SET(newsock, &exceptmask);
	int ret = select(FD_SETSIZE, &readmask, (fd_set *) 0, (fd_set *) &exceptmask, &timeout);
	if(ret < 0)
	{
		LOGERROR("message: ACCEPTһ����SOCKET����ЧSOCKET��can't read");		// ���ᴥ��
		closesocket(newsock);
		InterlockedDecrement(&s_nSocketCount);
		return INVALID_SOCKET;
	}
	else if(ret > 0)
	{
		if(FD_ISSET(newsock, &exceptmask))
		{
			LOGMSG("ACCEPTһ����SOCKET����ЧSOCKET��except");		// ���ᴥ��
			closesocket(newsock);
			InterlockedDecrement(&s_nSocketCount);
			return INVALID_SOCKET;
		}
		else if(FD_ISSET(newsock, &readmask))
		{
			char nTemp;
			if(recv(newsock, &nTemp, 1, MSG_PEEK) == 0)
			{
#ifdef	ALPHA_X
				LOGMSG("ACCEPTһ����SOCKET����ЧSOCKET��recv==0");		// �Ѿ��ر���
#endif
				closesocket(newsock);
				InterlockedDecrement(&s_nSocketCount);
				return INVALID_SOCKET;
			}
		}
	}
//*/
#ifdef	PROFILE_X
			// ����Accept�ٶ�(���յ���ЧSOCKET)
			const int	nTimes = ACCEPTPROFILESEC;		// ÿ10��ͳ��һ��ACCEPT�ٶ�
			static	clock_t tNextTime = clock() + nTimes * CLOCKS_PER_SEC;		//? ����һ�������̣߳��޹����ͻ
			static long	nCount	= 0;							//? ����һ�������̣߳��޹����ͻ
			if(clock() >= tNextTime)
			{
				LOGPROFILE("Port[%d]ÿ[%d]��ɹ�ִ����[%d]��Accept()", 
							m_nPort, nTimes, InterlockedExchange(&nCount, 0));
				tNextTime = clock() + nTimes * CLOCKS_PER_SEC;
			}
			else
			{
				InterlockedIncrement(&nCount);
			}
#endif	// PROFILE

	return newsock;
}

//	���ظ����á�ͨ�����ã������������ر�SOCKET��
//	�رպ�����µ���Open()�ظ�ʹ�øö���
void	CListenSocket::Close()		// ���ظ�����
{
	if(m_bState)
	{
//		if(bLinger0)
		{
			//	�޸�SOCKET���Գ������ر���
			struct linger ling;
			ling.l_onoff = 1;
			ling.l_linger = 0;
			setsockopt(m_sockListen, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));
		}
		m_bState = false;
		closesocket(m_sockListen);
		InterlockedDecrement(&s_nSocketCount);
//		WSACleanup();
		m_sockListen = INVALID_SOCKET;
		m_tCloseTime = clock();
	}
}
