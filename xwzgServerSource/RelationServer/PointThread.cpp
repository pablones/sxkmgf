// PointThread.cpp: �Ƶ��߳���
// �ɽ��ޣ�2001.11.20
//////////////////////////////////////////////////////////////////////

#include "AllHeads.h"
#include "PointThread.h"
#include "MsgConnect.h"
#include "MsgAccount.h"
#include "MsgLogin.h"
#include "MsgFee.h"
#include "MsgQueryFee.h"
#include "MsgVAS.h"
#include "MsgStopAccount.h"
#include "MsgNewPlayerExam.h"
#include "MsgAward.h"
#include "MsgManager.h"
#include "RoleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		MAX_SERVER_USER_NUM		1200		//��Ϸ����������������
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointThread::CPointThread(u_short nPort, int nSndBuf /*= 0*/)
			: CThreadBase(), m_cListenSocket(nPort, nSndBuf)
{
	for(int i = 0; i < MAXGAMESERVERS; i++)
	{
		Clear(i, false);
	}

	// �����ʺű�
	GetServerAccount();		// Ԥ��
	memset(m_aServerUserNum,0,sizeof(m_aServerUserNum));
}

CPointThread::~CPointThread()
{

}

/////////////////////////////////////////////////////////////////////////////
// ��������ע�⻥��

//#define	LOCK	{LOCKTHREAD;
//#define	UNLOCK	}

// ��ʹ�ù����Ա�����������������������ⲿ�����������Ƚ�����
void	CPointThread::OnInit()
{
	LOCKTHREAD;
	try{

		m_cListenSocket.Open();
		LOGMSG("�Ƶ��߳���������");
	}catch(...) { LOGCATCH("�Ƶ��̳߳�ʼ��ʱ�쳣�˳�");}
}

//20070726����:��try�ֿ�
bool	CPointThread::OnProcess()
{
//	time_t	tLockStart = clock();
	LOCKTHREAD;
//		long	nLockUsed = clock() - tLockStart;
//		InterlockedExchangeAdd(&s_nPointThreadLockTime,nLockUsed);//20070804����:�Ƶ��߳�process����ʱ

		time_t	tStart = clock();
		// select
		fd_set	readmask;
		FD_ZERO(&readmask);
		if(!m_cListenSocket.IsOpen())
			m_cListenSocket.Rebuild();
		if(m_cListenSocket.IsOpen())
			FD_SET(m_cListenSocket.Socket(), &readmask);
		for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
		{
			if(m_aServerSocket[i].IsOpen())
				FD_SET(m_aServerSocket[i].Socket(), &readmask);
		}

		struct timeval	timeout = {0,0};
		int ret = select(FD_SETSIZE, &readmask, (fd_set *) 0, (fd_set *) 0, &timeout);

	try
	{
		// ����Ƿ���յ�������
		if(ret == -1)	// error
		{
			m_cListenSocket.Close();
			LOGERROR("�Ƶ��߳� select ����[%d]", WSAGetLastError());
			PrintText("�Ƶ��̳߳���SOCKET���رա�%d ��󽫻��Զ��ؽ�", REBUILDLISTENDELAYSEC);
		}
		else if(ret > 0 && FD_ISSET(m_cListenSocket.Socket(), &readmask))
		{
			FD_CLR(m_cListenSocket.Socket(), &readmask);
			ret--;
			u_long	nClientIP;
			SOCKET	sockNew = m_cListenSocket.Accept(nClientIP);
			if(sockNew != INVALID_SOCKET)
			{
				// �ҿ�λ��
				bool	bSuccess = false;
				for(int i =1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
				{
					if(!m_aServerSocket[i].IsOpen())
					{
						bSuccess = true;
						break;	// �ɹ�
					}
				}

				// �������
				struct in_addr	in;
				in.S_un.S_addr = nClientIP;
				char * pAddr = inet_ntoa(in);
				if(bSuccess)
				{
					m_aServerSocket[i].Open(sockNew, nClientIP);
					m_aHeartbeat[i] = clock();
					LOGMSG("һ������Ϸ���������ӽ�����������ID�ŷ���Ϊ[%d], IP��ַΪ[%s]", i, pAddr);
					::SetServerState(i, c_flagSocket);
				}
				else
				{
					LOGWARNING("�ѽ��յ�һ��������[%s]�������ӱ����������޸�MAXGAMESERVERS�������±������"
								, pAddr);
					//	�޸�SOCKET���Գ������ر���
					struct linger ling;
					ling.l_onoff = 1;
					ling.l_linger = 0;
					setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));
					closesocket(sockNew);
					InterlockedDecrement(&s_nSocketCount);
				}
			}
		}
	}
	catch(...)
	{
		LogSave("CPointThread::OnProcess() catch");
	}

	try
	{
		// ���յ���¼��Ϣ������Ҫ�Ż�??
		for( i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
		{
			if(ret <= 0)
				break;	// û������������

			if(m_aServerSocket[i].IsOpen())
			{
				if(FD_ISSET(m_aServerSocket[i].Socket(), &readmask))
				{
					FD_CLR(m_aServerSocket[i].Socket(), &readmask);
					ret--;
					char	bufMsg[_MAX_MSGSIZE];
					int		nMsgLen = 0;
					if(m_aServerSocket[i].Recv(bufMsg, nMsgLen))
					{
						if(nMsgLen)
						{
							m_aHeartbeat[i] = clock();				//�յ���Ϣ����Ϊ�յ�����.....
							m_aHeartbeatLast[i] = 0;
							ProcessMsg(i, bufMsg, nMsgLen);
							//20070804����
							InterlockedIncrement(&s_nPointThreadPocMsgCount);
						}
					}
					else	// �����ر�
					{
						LOGWARNING("Recv��������[%s]�رա�", m_aServerName[i]);
						LOGSERVER("���ߣ�������[%s]�رա�", m_aServerName[i]);
						if (stricmp(g_szMainServer, m_aServerName[i])==0)
						{
							strcpy(g_szMainServer, "");
							PrintText("����������%s���رգ�����������������...", m_aServerName[i]);
							MsgManager::Instance().AppointMainServer(i);//������������
						}
						else
							PrintText("��������%s���رա�", m_aServerName[i]);

						MsgManager::Instance().ProcessMsgServerDisconnect(i);///���������Ϣ
						Clear(i);
					}
				
				}
			}
		}

		UNLOCKHEAD;

		long	nUsed = clock() - tStart;
		InterlockedExchangeAdd(&s_nPointThreadPocessTime,nUsed);//20070804����:�Ƶ��߳�process����ʱ
		InterlockedExchangeAdd(&s_nAllTimeSum, nUsed);
		InterlockedIncrement(&s_nPointCount);

		long	nRemain = POINTLOOPDELAY - nUsed;
		if(nRemain < 0 || nRemain > POINTLOOPDELAY)
			nRemain = 0;
		//20070803����:���л�

		///����Ƿ����������׽�����,����оͲ�����Ϣ
		for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
		{
			if(m_aServerSocket[i].IsOpen())
				FD_SET(m_aServerSocket[i].Socket(), &readmask);
		}

		struct timeval	timeout = {0,0};
		int ret = select(FD_SETSIZE, &readmask, (fd_set *) 0, (fd_set *) 0, &timeout);
		///����Ƿ����������׽�����,����оͲ�����Ϣ
		
//		ret=0;
		if(nRemain && ret<=0)		//? ���л�����¼�̡߳��Ʒ��߳�����//06.10.21����ע��,��֤��½�߳�����
		{
			Sleep(nRemain);
			InterlockedExchangeAdd(&s_nPointThreadLockTime, nRemain);
		}
		else
		{
			nRemain=0;
		}
		MsgManager::Instance().OnBreathe();///����ʱ����
//		Sleep(30);
 		return true;

	}catch(...) { 
		LOGCATCH("�Ƶ��߳���ѭ���쳣��������������"); 
		PrintText("�Ƶ��߳���ѭ������������������...");
		return true; 
	}
}

void	CPointThread::OnDestroy()
{
	LOCKTHREAD;

//	LOCK	// VVVVVVVVV
	try{
		m_cListenSocket.Close();
		for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
		{
			if(m_aServerSocket[i].IsOpen())
				m_aServerSocket[i].Close();
		}
		LOGMSG("�Ƶ��߳������˳�");

	}catch(...) { LOGCATCH("�Ƶ��߳̽���ʱ�쳣�˳�"); }

//	UNLOCK	// AAAAAAAAA
}

// �ﹲ��������Ҫ���⡣��LOGIN�̵߳��ã���ø�Ϊ��Ϣ����ģʽ??
// return ERR_NONE: OK
int	CPointThread::NewLogin(OBJID idAccount, DWORD nAuthenID, const char * szClientIP, const char * pGameServer)
{
	//---------------
	clock_t clkNewLoginBegin = clock();
	//---------------
	LOCKTHREAD;
	//----------------
	clock_t clkNewLoginUsed = clock() - clkNewLoginBegin;
	if(clkNewLoginUsed>20)//proc��ʱ
	{
		char szLog[512];
		sprintf(szLog,"NewLogin��ʱ(%d):idAccount:%d,pGameServer:%s",
			clkNewLoginUsed,idAccount,pGameServer);
		::LogSave(szLog);
	}
	//----------------

	CMsgConnect	cMsg;
	ASSERT(IPSTRSIZE == _MAX_NAMESIZE);
	cMsg.Create(idAccount, nAuthenID, (char *)szClientIP);
	OBJID		nIndex = GetServerIndex_0(pGameServer);		// ���󷵻�NULL
	if(nIndex && m_aServerSocket[nIndex].IsOpen())
	{
		if(m_aServerUserNum[nIndex]>=MAX_SERVER_USER_NUM)
			return ERR_FULL;
		if(m_aState[nIndex] == STATE_NORMAL || idAccount <= MAX_GM_ID)
		{
			m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize());	// ����ʧ��ʱ���Դ����´λ�ɾ����Ϸ��������
//			LOGACCOUNT("���͸�[%s]���[%d]�ĵ�¼��Ϣ����֤ID[%08X]���ͻ���IP[%s]", 
//						m_aServerName[nIndex], idAccount, nAuthenID, szClientIP);

			m_aServerUserNum[nIndex]++;

			return ERR_NONE;
		}
		else if(m_aState[nIndex] == STATE_BUSY)
			return ERR_BUSY;
		else if(m_aState[nIndex] == STATE_FULL)
			return ERR_FULL;
	}

	return ERR_NO_SERVER;	// ����ķ��������������δ������
}

bool	CPointThread::GetServerIP(char * bufIP, const char * pServerName)
{
	LOCKTHREAD;

	for(int i = 1; i < MAXGAMESERVERS; i++)	// ����0��0Ϊ����
	{
		if(strcmp(m_aServerName[i], pServerName) == 0)
			break;
	}

	if(bufIP && i < MAXGAMESERVERS)
	{
		if(m_aServerSocket[i].IsOpen())
		{
			strcpy(bufIP, m_aServerSocket[i].GetPeerIP());
			return true;
		}
		else
		{
			if(m_aServerName[i][0])
			{
				LOGWARNING("ȡIPʱ����Ϸ������[%s]�Ѿ��ر�", m_aServerName[i]);
				LOGSERVER("������[%s]�Ѿ��ر���", m_aServerName[i]);
				PrintText("��������%s���Ѿ��ر���", m_aServerName[i]);
			}
			Clear(i);
		}
	}

	bufIP[0] = 0;
	return false;
}

bool	CPointThread::GetServerURL(char * bufURL, const char * pServerName)
{
	LOCKTHREAD;
	
	for(int i = 1; i < MAXGAMESERVERS; i++)	// ����0��0Ϊ����
	{
		if(strcmp(m_aServerName[i], pServerName) == 0)
			break;
	}
	
	if(bufURL && i < MAXGAMESERVERS)
	{
		if(m_aServerSocket[i].IsOpen())
		{
			strcpy(bufURL, m_aServerURL[i]);
			return true;
		}
		else
		{
			if(m_aServerName[i][0])
			{
				LOGWARNING("ȡIPʱ����Ϸ������[%s]�Ѿ��ر�", m_aServerName[i]);
				LOGSERVER("������[%s]�Ѿ��ر���", m_aServerName[i]);
				PrintText("��������%s���Ѿ��ر���", m_aServerName[i]);
			}
			Clear(i);
		}
	}
	
	bufURL[0] = 0;
	return false;
}

int	CPointThread::GetServerLoginMode(const char * pServerName)
{
	LOCKTHREAD;
	
	for(int i = 1; i < MAXGAMESERVERS; i++)	// ����0��0Ϊ����
	{
		if(strcmp(m_aServerName[i], pServerName) == 0)
			break;
	}
	
	if(i < MAXGAMESERVERS)
	{
		if(m_aServerSocket[i].IsOpen())
		{
			return m_nServerLoginMode[i];
		}
	}
	return -1;
}

int	CPointThread::GetServerState(LPCTSTR szServerName)
{
	LOCKTHREAD;

	int	nIndex = GetServerIndex_0(szServerName);
	if(nIndex)
		return m_aState[nIndex];
	else
		return STATE_OFFLINE;
}

// int	CPointThread::GetServerLoginMode(const char * pServerName)
// {
// 	LOCKTHREAD;
// 	
// 	for(int i = 1; i < MAXGAMESERVERS; i++)	// ����0��0Ϊ����
// 	{
// 		if(strcmp(m_aServerName[i], pServerName) == 0)
// 			break;
// 	}
// 	
// 	if(i < MAXGAMESERVERS)
// 	{
// 		if(m_aServerSocket[i].IsOpen())
// 		{
// 			return m_nServerLoginMode[i];
// 		}
// 	}
// 	return -1;
// }

int	CPointThread::GetServerCount()
{
	LOCKTHREAD;

	bool	nCount = 0;
	for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
	{
		if(m_aState[i] != STATE_OFFLINE)
			nCount++;
	}

	return nCount;
}

bool CPointThread::CheckHeartbeatAll()
{
	LOCKTHREAD;

	bool	ret = false;
	for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: ��1��ʼ��
	{
		if(m_aServerSocket[i].IsOpen())
		{
			CheckHeartbeat(i);
			ret = true;
		}
	}
	return ret;
}

/*bool	CPointThread::GetServerName(OBJID idServer, char bufName[SERVERNAMESIZE])
{
	LOCKTHREAD;

	ASSERT(bufName);

	bufName[0] = 0;
	if(!idServer)
		return false;

	if(m_aServerSocket[idServer].IsOpen())
	{
		SafeCopy(bufName, m_aServerName[idServer], _MAX_NAMESIZE);
		return true;
	}
	else
	{
		if(m_aServerName[idServer][0])
		{
			LOGWARNING("ȡ��ʱ����Ϸ������[%s]�Ѿ��ر�", m_aServerName[idServer]);
			LOGSERVER("������[%s]�Ѿ��رա�", m_aServerName[idServer]);
			PrintText("��������%s���Ѿ��رա�", m_aServerName[idServer]);
		}
		Clear(idServer);		// SOCKET�رպ�ɾ����Ϸ��������������ͬ��
		return false;
	}
}*/

void	CPointThread::LogCount()
{
	LOCKTHREAD;

	char	bufTime[20];
	DateTime(bufTime, time(NULL));
	char	bufLine[4096]="";			//?
	int		nAmount = g_pOnlineTable->GetPlayerCount();
	for(int i = 1; i < g_nServerAccount; i++)	// 1: ��1��ʼ��
	{
		int	nCount = g_pOnlineTable->GetPlayerCount(g_aServerAccount[i].m_szServerName);
		int	nState = g_pPointThread->GetServerState(g_aServerAccount[i].m_szServerName);
		if(nState == CPointThread::STATE_OFFLINE)
		{
			nAmount	-= nCount;
			nCount	= 0;
		}
		char	buf[256];
		sprintf(buf, " | %12d", nCount);
		strcat(bufLine, buf);
	}
	strcat(bufLine, "\n");

	char	buf[4096];
	sprintf(buf, "%s | %12d", bufTime, nAmount);
	strcat(buf, bufLine);
	log_SaveFile("count", buf, g_szCountHeadLine);		//? LOGFILE�����Լ��Ļ�������˫�ػ���Ч�ʽϵ�
}

void	CPointThread::LogSerialCount()
{
	LOCKTHREAD;

	char	bufTime[20];
	DateTime(bufTime, time(NULL));
	char	bufLine[4096]="";			//?
	int		nAmount = 0;
	int		nCount = 0;
	for(COnlineTable::SERIAL_INDEX::iterator iter = g_pOnlineTable->GetSerialSet()->Begin();
			iter != g_pOnlineTable->GetSerialSet()->End();
			iter++, nCount++)
	{
		char	buf[256];
		sprintf(buf, " | %4d:%4d", iter->first, iter->second);
		strcat(bufLine, buf);
		nAmount	+= iter->second;
		if(strlen(bufLine) > 4000)
			break;
		if(nCount > 50000)
		{
			LOGERROR("��LogSerialCount()������");
			break;
		}
	}
	strcat(bufLine, "\n");

	char	buf[4196];
	sprintf(buf, "%s | %8d", bufTime, nAmount);
	strcat(buf, bufLine);
	log_SaveFile("serial", buf, "ʱ��                |   �ϼ�   | ���к�:����\n");		//? LOGFILE�����Լ��Ļ�������˫�ػ���Ч�ʽϵ�
}

void	CPointThread::LogServer()
{
	LOCKTHREAD;

	char	bufTime[20];
	DateTime(bufTime, time(NULL));
	char	bufLine[4096];			//?
	sprintf(bufLine, "%s | %12d", bufTime, GetServerCount());
	for(int i = 1; i < g_nServerAccount; i++)	// 1: ��1��ʼ��
	{
		BOOL	bState = GetServerState(g_aServerAccount[i].m_szServerName);
		char	buf[256];
		if(bState != STATE_OFFLINE)
			sprintf(buf, " | %12s", "��");
		else
			sprintf(buf, " | %12s", "�w");
		strcat(bufLine, buf);
	}
	strcat(bufLine, "\n");

	log_SaveFile("server", bufLine, g_szCountHeadLine);		//? LOGFILE�����Լ��Ļ�������˫�ػ���Ч�ʽϵ�
}

bool	CPointThread::Kickout(OBJID idAccount)
{
	LOCKTHREAD;

	char	bufServer[256];
	if(g_pOnlineTable->GetServerName(idAccount, bufServer))
	{
		return Kickout_0(idAccount, bufServer);
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////
// ��ǹ������������̶߳�ռ���á����ܵ����ⲿ������

bool	CPointThread::Kickout_0(OBJID idAccount, LPCTSTR szServerName)
{
	int nIndex = GetServerIndex_0(szServerName);

	if(nIndex != 0 )
	{
		CMsgFee	cMsg;
		cMsg.Create(idAccount, CMsgFee::FEE_KICKOUT);
		m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
		return true;
	}


	return false;
}

bool	CPointThread::ProcessMsg(int nServerIndex,  char * pBuf, int nLen)
{
	unsigned short unMsgType = CNetMsg::GetType(pBuf, nLen);
	MsgManager::Instance().ProcessMsg(nServerIndex,pBuf, nLen);///�ӹ���Ϣ����
	switch(unMsgType)
	{
		case _MSG_RELAY:
		case _MSG_TALK:
		case _MSG_FRIEND:
		case _MSG_SYNDICATE:
		case _MSG_ITEMINFO:
		case _MSG_EUDEMONINFO:
		case _MSG_SYNINFO:
		case _MSG_SYNMEMBERINFO:
		case _MSG_ITEM:
		case _MSG_WEAPONSOUL:
		case _MSG_MAINSERVER:
		case _MSG_REGISTER:
		case _MSG_SCHOOLMEMBER:
		case _MSG_VIPLIST:
		case _MSG_ACCLOCK:
		case _MSG_WARGAME:
		{
			break;
		}
	case _MSG_ACCOUNT:
		{
			try
			{
				CMsgAccount	cMsg;
				cMsg.Create(pBuf, nLen);
				LOGMSG("���յ�[%s]��Ϸ������ACCOUNT��Ϣ���ʺ�[%s],����[***]��"
					, cMsg.m_pInfo->szServer
					, cMsg.m_pInfo->szAccount
					//, cMsg.m_pInfo->szPassword
					);
				
				// ��Ϣ���
				if(strlen(cMsg.m_pInfo->szAccount) == 0 
					|| strlen(cMsg.m_pInfo->szPassword) == 0 
					|| strlen(cMsg.m_pInfo->szServer) == 0)
				{
					LOGERROR("����ĵ�¼��Ϣ[%s][%s][%s]", 
						cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, cMsg.m_pInfo->szServer);
					return false;
				}
				
				GetServerAccount();		// ��ʱ����
				
				// ����ʺ�
				int		nAccount = 0;		// ���ʺű��е���ţ�����Ϸ������ID�š�
				for(int i = 1; i < g_nServerAccount; i++)	// 1: ��1��ʼ��
				{
					if(strcmp(g_aServerAccount[i].m_szServerName, cMsg.m_pInfo->szServer) == 0)		// �Է�������ƥ��Ϊ׼
					{
						nAccount = i;
						break;
					}
				}
				
				m_aServerUserNum[nAccount] = 0;

				CMsgLogin	cRetMsg;
				//		AfxMessageBox(g_aServerAccount[nAccount].m_szLoginName);
				//		AfxMessageBox(g_aServerAccount[nAccount].m_szPassword);
				if(nAccount					// 0���޴��ʺ�
					&& strcmp(g_aServerAccount[nAccount].m_szLoginName, cMsg.m_pInfo->szAccount) == 0
					&& strcmp(g_aServerAccount[nAccount].m_szPassword, cMsg.m_pInfo->szPassword) == 0)	// �ҵ�
				{
					for(int i = 1; i < MAXGAMESERVERS; i++)
					{
						if(strcmp(g_aServerAccount[nAccount].m_szServerName, m_aServerName[i]) == 0)
						{
							// �ظ���¼
							if(m_aServerSocket[i].IsOpen())
							{
								LOGMSG("��Ϸ������[%s]�ظ�ACCOUNT��¼��ǰһ������[%d]�ѶϿ���", m_aServerName[i], i);
								m_aServerSocket[i].Close();		// �ر�ǰһ����¼������
								Clear(i);
							}
							m_aServerName[i][0] = 0;
							//��С����ȫ��һ��	break;
						}
					}
					
					// ������Ϸ����������/IP
					strcpy(m_aServerName[nServerIndex], g_aServerAccount[nAccount].m_szServerName);	// cMsg.m_pInfo->szServer);
					// ������Ϸ����������/IP
					strcpy(m_aServerName[nServerIndex], g_aServerAccount[nAccount].m_szServerName);	// cMsg.m_pInfo->szServer);
					strcpy(m_aServerURL[nServerIndex], g_aServerAccount[nAccount].m_szServerURL);
					m_nServerLoginMode[nServerIndex] = g_aServerAccount[nAccount].m_nServerLoginMode;

					//?? BUG����ǰ�����¼�ˣ�Ӧ�ý��յ�_MSG_LOGIN��ʼ�����¼
					PrintText("��%d����Ϸ��������%s���ѵ�¼", nServerIndex, m_aServerName[nServerIndex]);
					LOGSERVER("��%d����Ϸ��������%s���ѵ�¼", nServerIndex, m_aServerName[nServerIndex]);
					//?? BUG����ǰ�����¼�ˣ�Ӧ�ý��յ�_MSG_LOGIN��ʼ�����¼
					if(cRetMsg.Create(nServerIndex, 0, "", ACCOUNTVERSION))
					{
						if(m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize())==false)
							LOGMSG("��Ϸ��������¼��Ӧ��,����ʧ��!%s", m_aServerName[nServerIndex]);
					}
					else
					{
						LOGMSG("��Ϸ��������¼��Ӧ��,���ʧ��!,%s", m_aServerName[nServerIndex]);
					}
					LOGMSG("�µ���Ϸ������[%s]ACCOUNT�ʺ��Ͽɡ����ط�����ID[%d],��Ϸ�汾[%04X]", 
						m_aServerName[nServerIndex], nServerIndex, ACCOUNTVERSION);
					LOGMSG("�µ���Ϸ������[%s]ACCOUNT��¼��......", m_aServerName[nServerIndex]);
					::SetServerState(nServerIndex, c_flagAccount);
					MsgManager::Instance().AppointMainServer(-1);//������������
					// �������
					//				m_idxID.Add(nServerIndex, nAccount);
					//				g_aServerAccount[nAccount].m_nIndex = nServerIndex;
				}
				else
				{
					cRetMsg.Create(0, 0, "", ACCOUNTVERSION);
					m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
					LOGMSG("�µ���Ϸ������[%s]��¼ʧ��", cMsg.m_pInfo->szServer);
				}
				///!!!!!!!!!!!!!!!softworms
				//			CMsgVAS msgVAS;
				//			msgVAS.m_pInfo=new CMsgVAS::MSG_Info;
				//			CMsgVAS::MsgTraffic mt={0};
				//			mt.itemAmount=1;
				//			mt.itemID=131090;
				//			mt.unMsgType=_MSG_GENERAL + 1046;
				//			mt.unMsgSize=sizeof(mt);
				//			msgVAS.Create(mt,4900,240,14003832);
				//			ProcessMsg(nServerIndex,(char*)msgVAS.m_pInfo,msgVAS.m_pInfo->unMsgSize);
				//			delete msgVAS.m_pInfo;
				///!!!!!!!!!!!!!!!
				MsgManager::Instance().BroadcastGameServerListMsg(nServerIndex);
			}
			catch(...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_ACCOUNT");
			}
		}
		break;
	case _MSG_LOGIN:
		{
			try
			{
				// ��Ϣ���(����Ҫ)
				
				//			CMsgConnect	cMsg;
				//			cMsg.Create(nServerIndex, 0, "��¼�ɹ�");
				
				// ���򵥻ش�
				//			m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				m_aState[nServerIndex] = STATE_NORMAL;		// ��¼���
				
				LOGMSG("......[%s]��Ϸ������LOGIN��¼�ɹ�", m_aServerName[nServerIndex]);
				LOGMSG("���յ���¼��Ϣ���µ���Ϸ������[%s]LOGIN��¼�ɹ�", m_aServerName[nServerIndex]);
				::SetServerState(nServerIndex, c_flagNormal);
			}
			catch(...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_LOGIN");
			}
		}
		break;
	case _MSG_GAINAWARDS:
		{
			try
			{
				CMsgAward cMsg;
				cMsg.Create(pBuf, nLen);

				switch(cMsg.m_pInfo->nAction)
				{
				case _GAINAWARD_QUERY:
					{
						VECAWARD vecAwards;
						if(m_objInspectManage.ReadAwardEx(cMsg.m_pInfo->accountID, cMsg.m_pInfo->gaInfo[0].nType, cMsg.m_pInfo->szServerName,  vecAwards))
						{
							if(!vecAwards.empty())
							{
								cMsg.m_pInfo->nAction = _GAINAWARD_INIT;
								int nLimitAmount = (_MAX_MSGSIZE - sizeof(CMsgAward::MSG_Info))/sizeof(GainAwardInfo);
								int nLeaveAmount = vecAwards.size();
								int i = 0;

								while(nLeaveAmount > nLimitAmount)
								{
									cMsg.m_pInfo->nAmount = nLimitAmount;
									cMsg.m_pInfo->unMsgSize = sizeof(CMsgAward::MSG_Info) + sizeof(GainAwardInfo)*(nLimitAmount-1);
									memcpy(cMsg.m_pInfo->gaInfo, &vecAwards[i], sizeof(GainAwardInfo)*nLimitAmount);
									m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());

									i += nLimitAmount;
									nLeaveAmount -= nLimitAmount;
								}

								cMsg.m_pInfo->nAmount = nLeaveAmount;
								cMsg.m_pInfo->unMsgSize = sizeof(CMsgAward::MSG_Info) + sizeof(GainAwardInfo)*(nLeaveAmount-1);
								memcpy(cMsg.m_pInfo->gaInfo, &vecAwards[i], sizeof(GainAwardInfo)*nLeaveAmount);
								m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
							}
						}
					}
					break;
					
				case _GAINAWARD_UPDATE:
					{
						m_objInspectManage.WriteAwardeEx(cMsg.m_pInfo->accountID, cMsg.m_pInfo->gaInfo[0].nType, cMsg.m_pInfo->szServerName, cMsg.m_pInfo->gaInfo[0].nValue);
					}
					break;
				}
			}
			catch(...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_GAINAWARDS");
			}
		}
		break;
	case _MSG_EXAM:
		{
			CMsgNewPlayerExam cMsg;
			cMsg.Create(pBuf, nLen);
			const int saveSize = 512;
			m_objInspectManage.GetNewPlayerExamState(cMsg.m_pInfo->nAccountID,cMsg.m_pInfo->szBuf,saveSize,cMsg.m_pInfo->usSzSize);
			cMsg.Create( 0,cMsg.m_pInfo->szBuf,cMsg.m_pInfo->usSzSize );
			m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
		}
		break;
	case _MSG_VAS:
		{
			time_t	tStart = clock();
			try
			{
				BOOL bResult=0;
				CMsgVAS		cMsg;
				cMsg.Create(pBuf, nLen);
				OBJID	idAccount = cMsg.m_pInfo->AccountID;///�õ��ʺ�
				::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
				//	usActionֵ˵����1��ʾ����ɹ���2��ʾ����ʧ�ܣ�3��ʾ����ѯ��100��ʾ��Ϣ����Դ�������߳�
				//	  10��ʾ���ƺ�״̬��ѯ��      11��ʾ��1������װ����      12��ʾ��20��װ����      13��ʾ��40��װ����
				//	1000��ʾ���ƺ�״̬��ѯ����
				//[2010-07-26 goto]�޸�usAction,1001��ʾ�����̷߳����Ĳ�ѯ���
				//[2010-07-26 goto]�޸�CMsgVAS��Ϣ�ṹ��ע����ǰ����Ϣ����
				if( cMsg.m_pInfo->usAction >= 10 && cMsg.m_pInfo->usAction < 100 )
				{
					switch( cMsg.m_pInfo->usAction )
					{
					case 10:
						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = 999 + cMsg.m_pInfo->usVASPoint;
// 							cMsg.m_pInfo->usVASPoint = m_objInspectManage.ManageGeneralize( cMsg.m_pInfo->AccountID, 0, cMsg.m_pInfo->usVASPoint );
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
						}
						break;
					case 11:
						{
// 							if( 0 > m_objInspectManage.ManageGeneralize( cMsg.m_pInfo->AccountID, cMsg.m_pInfo->mtExtraInfo.itemAmount, cMsg.m_pInfo->usVASPoint ) )
// 							{
// 								//��ȡһ��װ��ʧ��д��־
// 								::LogSave("ERROR: case _MSG_VAS: NO Present_1 AccountID:%d",cMsg.m_pInfo->AccountID );///��־��¼			
// 							}
						}
						break;
					case 12:
						{
//							m_objInspectManage.ManageGeneralize( cMsg.m_pInfo->AccountID,1 , cMsg.m_pInfo->usVASPoint,cMsg.m_pInfo->mtExtraInfo.itemID ) ;							
						}
						break;
					}
				}
				//////////////////////////////////////////////////////////////////////////
				//  [2007-9-15] ���
				else if(cMsg.m_pInfo->usAction == _MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE)
				{
// 					int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 					if(!nResult)
// 					{
// 						cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE_FAL;	
// 					}
// 					else
// 					{
// 						UINT uType = _VAS_CONSUMELOG_TYPE_CONTINUE_FULL;
// 						if(cMsg.m_pInfo->mtExtraInfo.itemAmount == _ITEM_LIFE_TIME_CONTINUE_TYPE_HALF)
// 							uType = _VAS_CONSUMELOG_TYPE_CONTINUE_HALF;
// 					//	nResult = m_objManager.ExecItemLifeContinue(cMsg.m_pInfo->usVASPoint, cMsg.m_pInfo->PlayerID, cMsg.m_pInfo->mtExtraInfo.itemID,1, m_aServerName[nServerIndex], uType);
// 					//  [2/16/2008 ���]	д��ITEMTYPEID
// 						nResult = m_objManager.ExecItemLifeContinue(cMsg.m_pInfo->usVASPoint, cMsg.m_pInfo->PlayerID, cMsg.m_pInfo->mtExtraInfo.dwData, 1, m_aServerName[nServerIndex], uType);
// 						if(nResult >= 0)	//  [3/14/2008 ���]	���ѻ��ѵ����˻��Ľ�Ǯʱ��Ǯ������ʧ������
// 						{
// 							//[��;���� 2009.01.09]�������
// 							cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 							cMsg.m_pInfo->usVASPoint=nResult;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE_SUC;
// 							//cMsg.m_pInfo->usVASPoint = nResult;		//	���������׽�����������ʧ���򷵻�-1
// 						}
// 						else
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE_FAL;
// 							cMsg.m_pInfo->usVASPoint = nResult;		//	���������׽�����������ʧ���򷵻�-1
// 						}
// 					}
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				else if(cMsg.m_pInfo->usAction == 101)
				{
// 					OBJID	idAccount = cMsg.m_pInfo->AccountID;///�õ��ʺ�
// 					int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 					if(!nResult)
// 					{
// 						cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_FAL;	
// 					}
// 					else
// 					{
// 						nResult=m_objManager.ExecChg(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex],1);
// 						if(nResult == -1)
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_FAL;	
// 						}
// 						else
// 						{
// 							//[��;���� 2009.01.09]�������
// 							cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 							int usVASPoint = cMsg.m_pInfo->usVASPoint;
// 							cMsg.m_pInfo->usVASPoint=nResult;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 							cMsg.m_pInfo->usVASPoint = usVASPoint;
// 							//-------------------------------------------
// 							idAccount = cMsg.m_pInfo->mtExtraInfo.itemAmount;//cMsg.m_pInfo->PlayerID;
// 							int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 							if(!nResult)
// 							{
// 								cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_FAL;	
// 							}
// 							else
// 							{
// 								nResult=m_objManager.ExecChg(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex],2);
// 								//[��;���� 2009.01.09]�������
// 								cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 								cMsg.m_pInfo->usVASPoint=nResult;
// 								idAccount = cMsg.m_pInfo->AccountID;
// 								OBJID idPlayer = cMsg.m_pInfo->PlayerID;
// 								cMsg.m_pInfo->AccountID = cMsg.m_pInfo->mtExtraInfo.itemAmount;
// 								cMsg.m_pInfo->PlayerID = cMsg.m_pInfo->mtExtraInfo.id;
// 								m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 								cMsg.m_pInfo->AccountID = idAccount;
// 								cMsg.m_pInfo->PlayerID = idPlayer;
// 
// 								cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_SUC;
// 							}
// 						}
// 					}
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				else if(cMsg.m_pInfo->usAction == 102)
				{
// 					OBJID	idAccount = cMsg.m_pInfo->AccountID;///�õ��ʺ�
// 					int nResult = 0;
// 					if(m_objManager.InitServer(idAccount))
// 					{
// 						nResult=m_objManager.ExecServerChk(cMsg.m_pInfo->nJuan,cMsg.m_pInfo->nRealID,cMsg.m_pInfo->usVASPoint);
// 						if(nResult >= 0)
// 						{							
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_SERVER_SUC;
// 							cMsg.m_pInfo->usVASPoint=nResult;	
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 						}
// // 						else
// // 						{
// // 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_SERVER_FAL;
// // 							cMsg.m_pInfo->usVASPoint=nResult;							
// // 						}
// 					}
				}
				else if(cMsg.m_pInfo->usAction == _MSGVAS_CHG_TRADE)//[��;���� 2008/12/3]�ı���ֵ������
				{
// 					OBJID idAccount = cMsg.m_pInfo->mtExtraInfo.itemAmount;
// 					int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 					if (!nResult)
// 					{
// 						cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;	
// 					}
// 					else
// 					{
// 						nResult=m_objManager.ExecChg(m_aServerName[nServerIndex], -cMsg.m_pInfo->usVASPoint);
// 						if (nResult < 0)
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;
// 						}
// 						else
// 						{
// 							idAccount = cMsg.m_pInfo->AccountID;///�õ��ʺ�
// 
// 							int nResult2 = nResult;
// 							nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 							if (!nResult)
// 							{
// 								idAccount = cMsg.m_pInfo->mtExtraInfo.itemAmount;
// 								m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 								m_objManager.ExecChg(m_aServerName[nServerIndex], cMsg.m_pInfo->usVASPoint);
// 								cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;	
// 							}
// 							else
// 							{
// 								nResult=m_objManager.ExecChg(m_aServerName[nServerIndex], cMsg.m_pInfo->usVASPoint);
// 								cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_SUC;
// 								//[��;���� 2009.01.09]�������
// 								cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 								cMsg.m_pInfo->usVASPoint=nResult;
// 								m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 								cMsg.m_pInfo->usVASPoint=nResult2;
// 								cMsg.m_pInfo->AccountID = cMsg.m_pInfo->mtExtraInfo.itemAmount;
// 								cMsg.m_pInfo->PlayerID = cMsg.m_pInfo->mtExtraInfo.id;
// 								m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 							}
// 						}
// 					}
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				else if(cMsg.m_pInfo->usAction == _MSGVAS_CHG)//[��;���� 2009.01.08]�޸���ֵ������
				{
// 					OBJID idAccount = cMsg.m_pInfo->AccountID;
// 					int nResult = m_objManager.InitMananger(idAccount, m_aServerName[nServerIndex]);
// 					if (!nResult)
// 					{
// 						cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;
// 						m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 					}
// 					else
// 					{
// 						nResult=m_objManager.ExecChg(m_aServerName[nServerIndex], cMsg.m_pInfo->usVASPoint);
// 						if (nResult < 0)
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_FAL;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 						}
// 						else
// 						{
// 							cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_CHG_TRADE_SUC;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 							cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 							cMsg.m_pInfo->usVASPoint=nResult;
// 							m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
// 						}
// 					}
				}
				else if(cMsg.m_pInfo->usAction == _MSGVAS_DAYMASK_ACC_GET)
				{
// 					OBJID idAccount = cMsg.m_pInfo->AccountID;
// 					int nResult = m_objManager.InitServer(idAccount);
// 					if (nResult)
// 					{
// 						nResult = m_objManager.ExecMaskDo(cMsg.m_pInfo->usVASPoint,1,cMsg.m_pInfo->mtExtraInfo.itemAmount);
// 					}
// 					
// 					cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_DAYMASK_ACC_RETURN;
// 					cMsg.m_pInfo->usVASPoint=nResult;
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				else if(cMsg.m_pInfo->usAction == _MSGVAS_DAYMASK_ACC_SET)
				{
// 					OBJID idAccount = cMsg.m_pInfo->AccountID;
// 					int nResult = m_objManager.InitServer(idAccount);
// 					if (nResult)
// 					{
// 						nResult = m_objManager.ExecMaskDo(cMsg.m_pInfo->usVASPoint,2,cMsg.m_pInfo->mtExtraInfo.itemAmount);
// 					}
// 					
// 					cMsg.m_pInfo->mtExtraInfo.usAction = _MSGVAS_DAYMASK_ACC_RETURN;
// 					cMsg.m_pInfo->usVASPoint=nResult;
// 					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				//////////////////////////////////////////////////////////////////////////
				else
				{
					int nResult = -1;
					bResult=m_objManager.InitMananger(idAccount,m_aServerName[nServerIndex]);
					if( bResult )
					{
						switch( cMsg.m_pInfo->usAction )
						{
						case 0:
						case 3:
							{
								//nResult=m_objManager.ExecTrade(cMsg.m_pInfo->usVASPoint,cMsg.m_pInfo->PlayerID,cMsg.m_pInfo->mtExtraInfo.itemID,cMsg.m_pInfo->mtExtraInfo.itemAmount,m_aServerName[nServerIndex],cMsg.m_pInfo->nJuan);///����ʧ�ܷ���-1,�ɹ��������
							}
							break;
						case 4:
							{
// 								nResult=m_objManager.ExecSupply(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex],0,4,cMsg.m_pInfo->mtExtraInfo.itemAmount);///����ʧ�ܷ���-1,�ɹ��������
// 								//����Ƕһ��͸���ƷID��4444
// 								//������Ϸ�����������ж�
// 								cMsg.m_pInfo->mtExtraInfo.itemID = 4444;// ��� [6/26/2007]
							}
							break;
						case 5:
							{
// 								cMsg.m_pInfo->nJuan = 5;
// 								nResult=m_objManager.ExecTrade(cMsg.m_pInfo->usVASPoint,cMsg.m_pInfo->PlayerID,0,0,m_aServerName[nServerIndex],cMsg.m_pInfo->nJuan);
// 								//����Ƕһ��͸���ƷID��4444
// 								//������Ϸ�����������ж�
// 								cMsg.m_pInfo->nJuan = 1;
// 								cMsg.m_pInfo->mtExtraInfo.itemID = 0;// ��� [6/26/2007]
							}
							break;
						case 6:
							{
// 								cMsg.m_pInfo->nJuan = 6;
// 								nResult=m_objManager.ExecTrade(cMsg.m_pInfo->usVASPoint,cMsg.m_pInfo->PlayerID,0,0,m_aServerName[nServerIndex],cMsg.m_pInfo->nJuan);
// 								//����Ƕһ��͸���ƷID��4444
// 								//������Ϸ�����������ж�
// 								cMsg.m_pInfo->nJuan = 1;
// 								cMsg.m_pInfo->mtExtraInfo.itemID = 0;// ��� [6/26/2007]
							}
							break;
						//[2010-07-26 goto]������²���
						case _MSGVAS_QUERY_BALANCE://��ѯ���
							{
								nResult=m_objManager.GetBalance();
								cMsg.m_pInfo->usVASPoint = nResult;
								cMsg.m_pInfo->usAction = _MSGVAS_RETURN_BALANCE;
							}
							break;
						case _MSGVAS_ADD_VAS:
							{
								nResult=m_objManager.ExecChg(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex], 2);
								if (nResult < 0)
								{
									cMsg.m_pInfo->usAction = _MSGVAS_ADD_VAS_FAIL;
								}
								else
								{
									cMsg.m_pInfo->usAction = _MSGVAS_ADD_VAS_SUC;
								}
								
								cMsg.m_pInfo->usVASPoint=nResult;
							}
							break;
						case _MSGVAS_SPEND_VAS:
							{
								nResult=m_objManager.ExecChg(cMsg.m_pInfo->usVASPoint,m_aServerName[nServerIndex], 1);
								if (nResult < 0)
								{
									cMsg.m_pInfo->usAction = _MSGVAS_SPEND_VAS_FAIL;
								}
								else
								{
									cMsg.m_pInfo->usAction = _MSGVAS_SPEND_VAS_SUC;
								}
								
								cMsg.m_pInfo->usVASPoint=nResult;
							}
							break;
						case _MSGVAS_BUY_ITEM:
							{
								nResult=m_objManager.ExecTrade(cMsg.m_pInfo->usVASPoint, cMsg.m_pInfo->PlayerID, m_aServerName[nServerIndex], cMsg.m_pInfo->nGoods, cMsg.m_pInfo->usGoodSize);
								if (nResult < 0)
								{
									cMsg.m_pInfo->usAction = _MSGVAS_BUY_FAIL;
								}
								else
								{
									cMsg.m_pInfo->usAction = _MSGVAS_BUY_SUC;
								}
								
								cMsg.m_pInfo->usVASPoint=nResult;
							}
							break;
						}
					}
// 					if(3 == cMsg.m_pInfo->mtExtraInfo.usAction && -1 == nResult)
// 						cMsg.m_pInfo->mtExtraInfo.itemID = 3;
// 					cMsg.m_pInfo->mtExtraInfo.usAction = 100;
// 					cMsg.m_pInfo->usVASPoint=nResult;///���������׽�����������ʧ���򷵻�-1
					//cMsg.m_pInfo->nJuan = m_pInfo->nJuan;
					m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
				}
				::ReleaseMutex(g_xDatabase);	//------------------------------------------
			}
			catch(...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_VAS");
			}
			long nUsed = clock() - tStart;
			InterlockedExchangeAdd(&s_nPointThreadVASPocessTime,nUsed);//20071006-softworms:�Ƶ��߳�VASprocess����ʱ
		}
		break;
	case _MSG_GMSTOPACCOUNT:
		{
			try
			{
				CMsgStopAccount	cMsg;
				cMsg.Create(pBuf, nLen);

				if (1 == cMsg.m_pInfo->unAction)
				{
					OBJID	idAccount = cMsg.m_pInfo->ucStopIDAccount;///�õ��ʺ�
					USHORT  accountStatus = m_objInspectManage.GetAccountStatus(idAccount);
					if (cMsg.Create(1,accountStatus,cMsg.m_pInfo->playerID))
					{
						m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
					}
					break;
				}
				else if (2 == cMsg.m_pInfo->unAction)
				{
					OBJID	idAccount = cMsg.m_pInfo->ucStopIDAccount;///�õ��ʺ�
					m_objInspectManage.ActiveAccountByGMtool(idAccount);					
					break;
				}
				else
				{		
					OBJID	idAccount = cMsg.m_pInfo->ucStopIDAccount;///�õ��ʺ�
					if( !m_objInspectManage.StopAccountByGMtool( idAccount ) )
					{
						//���ʧ��д��־
						LogSave("CPointThread::ProcessMsg...StopAccountByGMtool() fail..");		
					}
				}
			}
			catch (...)
			{
				LogSave("CPointThread::ProcessMsg...case _MSG_VAS");

			}
		}
	break;
	case _MSG_FEE:
		{
			CMsgFee		cMsg;
			cMsg.Create(pBuf, nLen);
			OBJID	idAccount = cMsg.m_pInfo->idAccount;
			
			//ashulanTest
#ifdef _DEBUG
//			if(idAccount>1125)
//			{
//				int ashulanTest;
//				ashulanTest = 1;
//			}
#endif

			switch(cMsg.m_pInfo->ucType)
			{
			case CMsgFee::FEE_BEGIN:
				{
					try
					{
						// ��Ϣ���
						if(idAccount == ID_NONE)
						{
							LOGERROR("�����FEE_BEGIN��Ϣ[%d]��", idAccount);
							return false;
						}
						
						// ������߱�����������ͬ��
						if(!g_pOnlineTable->IsOnline(idAccount))		//? ��������APPENDNEW()�ķ�ԭ���Բ���
						{
							DWORD	nAuthenID = 0xCCCCCCCC;		// ������֤��
							::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
							if(g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVV
							{
								char* pName = g_cDatabase.GetName();
								g_pOnlineTable->AppendNew(idAccount, nAuthenID, "StartFee", pName, "", 
									//											g_cDatabase.GetType(), m_aServerName[nServerIndex], idAccount, pName);
									c_typeNone, m_aServerName[nServerIndex], idAccount, pName);	// return 0: error��c_typeNone : δ֪�ļƷ����͡�
								//?							if(g_bEnableLogin)
								LOGERROR("����FEE_BEGIN, ������[%s]�ʺ�[%s][%d]����ΪISPģʽ��Ƶ�ģʽ�����ٿ۵��", 
									m_aServerName[nServerIndex], pName, idAccount);
								g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
							}
							else
							{
								LOGERROR("��Ϸ������[%s]�ϴ���һ��δ֪�ʺ�[%d]��FEE_BEGIN��Ϣ��δ����", 
									m_aServerName[nServerIndex], idAccount);
							}
							::ReleaseMutex(g_xDatabase);	//------------------------------------------
						}
						
						//#ifdef	SERVER_X
						//					LOGPOINT("[%s]�ϴ�[%d]��START_FEE", m_aServerName[nServerIndex], idAccount);
						//#endif	// SERVER_X
						int nRet = g_pOnlineTable->StartFee(idAccount, m_aServerName[nServerIndex]);		// return -n: ��������ƥ��
						// ˫�ص�¼������
						if(nRet < 0)		// <0 : error
						{
							LOGERROR("...˫�ص�¼�������START_FEE���ʺ�[%d]�������ߡ�", idAccount);
							Kickout_0(idAccount, m_aServerName[nServerIndex]);
						}
						
						if(m_aState[nServerIndex] == STATE_NORMAL)
							::SetServerState(nServerIndex, c_flagBegin);
						else
							::SetServerState(nServerIndex, c_flagBegin2);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::FEE_BEGIN");
					}
				}
				break;
			case CMsgFee::FEE_END:
			case CMsgFee::FEE_OFFLINE:
				{
					try
					{
						// ��Ϣ���
						if(idAccount == ID_NONE)
						{
							LOGERROR("�����FEE_END��Ϣ[%d]��", idAccount);
							return false;
						}
						
						g_pOnlineTable->EndFee(idAccount, m_aServerName[nServerIndex], 
							cMsg.m_pInfo->ucType == CMsgFee::FEE_OFFLINE);
						
						// ɾ�����߱�
						//��ENDFEEɾ��					g_pOnlineTable->Remove(idAccount);				// ����ɾ�����ʺ�
						if(m_aState[nServerIndex] == STATE_NORMAL)
							::SetServerState(nServerIndex, c_flagEnd);
						else
							::SetServerState(nServerIndex, c_flagEnd2);

						///softworms ���δ˴�
						//m_objInspectManage.WatchToolInfo(idAccount,m_aServerName[nServerIndex],2);///[2009.3.18--�Ŷ�]ɾ����ع���������Ϣ
						m_objInspectManage.UpdateAccoutOnlineTime(idAccount,m_aServerName[nServerIndex]);
						m_aServerUserNum[nServerIndex]--;
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::FEE_OFFLINE");
					}
				}
				break;
			case CMsgFee::FEE_POINT:
			case CMsgFee::FEE_TICK:
				{
					try
					{
						// ��Ϣ���
						if(idAccount == ID_NONE)
						{
							LOGERROR("�����FEE_POINT��Ϣ[%d]��", idAccount);
							return false;
						}
						
						// ������߱�����������ͬ��
						if(!g_pOnlineTable->IsOnline(idAccount))		//? ��������APPENDNEW()�ķ�ԭ���Բ���
						{
							DWORD	nAuthenID = 0xCCCCCCCC;		// ������֤��
							::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
							if(g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVV
							{
								char* pName = g_cDatabase.GetName();
								g_pOnlineTable->AppendNew(idAccount, nAuthenID, "PointFee", pName, "", 
									//											g_cDatabase.GetType(), m_aServerName[nServerIndex], idAccount, pName);			// return 0: error
									c_typeNone, m_aServerName[nServerIndex], idAccount, pName);	// return 0: error��c_typeNone : δ֪�ļƷ����͡�
								//?							if(g_bEnableLogin)
								LOGERROR("����FEE_POINT��������[%s]�ʺ�[%s][%d]����ΪISPģʽ��Ƶ�ģʽ�����ٿ۵��", 
									m_aServerName[nServerIndex], pName, idAccount);
								g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
							}
							else
							{
								LOGERROR("��Ϸ������[%s]�ϴ���һ��δ֪�ʺ�[%d]��FEE_POINT��Ϣ��δ����", 
									m_aServerName[nServerIndex], idAccount);
							}
							::ReleaseMutex(g_xDatabase);	//------------------------------------------
						}
						
						int		nFeeType		= c_typeNone;
						char	szServerName[_MAX_NAMESIZE] = "(δ֪)";
						OBJID	idFeeAccount	= ID_NONE;
						char	szFeeAccount[_MAX_ACCOUNT_NAMESIZE] = "(δ֪)";
						int		nPoint			= 0;
						if(g_pOnlineTable->GetAttr2(idAccount, nFeeType, szServerName, idFeeAccount, szFeeAccount))	//? ��������APPENDNEW()�ķ�ԭ���Բ���
						{
							if((nFeeType == c_typePoint || nFeeType == c_typeNetBarPoint 
								|| nFeeType == c_typeISP) && cMsg.m_pInfo->ucType != CMsgFee::FEE_TICK)		//? �ǼƵ����Ͳ�����
							{
								if(strcmp(szServerName, m_aServerName[nServerIndex]) == 0)
								{
									//  [5/26/2007]	����ע��----------------begin----------------------------
									//	˵�������յ��������ļƵ���Ϣ�����㹦��ȥ����������Ϊ�����Ӫģʽ
									//	�������˲��ٷ�TICK��Ϣ��������FEE_POINT��Ϣ����Ƶ����ԭ��TICK��Ƶ�ʣ�ÿ3���ӣ�,����������ΪҪʹ�������ⲿ�ֹ��ܣ���Ϊ��cMsg.m_pInfo->ucType != CMsgFee::FEE_TICK
									//								bool	bOk		= false;
									//								::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
									//								if(g_cDatabase.Create(idFeeAccount))	// VVVVVVVVVVVVVVV
									//								{
									//									bOk		= true;
									//									nPoint	= g_cDatabase.GetPoint();
									//									g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
									//								}
									//								::ReleaseMutex(g_xDatabase);	//------------------------------------------
									//								
									//								if(bOk)
									//								{
									//									if(nPoint > 0)
									//									{
									//										if(g_cDatabase.DecPoint(idFeeAccount))		//?? ����ҪOPEN��ֱ��ʹ��CDATABASE�Ĳ�ѯ��䡣
									//										{
									//											InterlockedIncrement(&s_nPointFee);
									//											InterlockedIncrement(&s_nPointSum);
									//										}
									//										else
									//										{
									//											LOGERROR("���ݿ�����ʺ�ID[%d]�޷��۵㡣��", idFeeAccount);
									//											LOGPOINT("���ݿ�����ʺ�ID[%d]�޷��۵㡣", idFeeAccount);
									//										}
									//									}
									//									else if(KICKOUT_USER_WHEN_NO_POINT)
									//									{
									//										LOGPOINT("������[%s]�����[%d]���ʺ�[%s][%d]�����ľ����ߡ�", 
									//											m_aServerName[nServerIndex], idAccount, szFeeAccount, idFeeAccount);
									//										Kickout_0(idAccount, m_aServerName[nServerIndex]);
									//									}
									//								}
									//								else
									//								{
									//									LOGERROR("��Ϸ������[%s]�ϴ���һ��δ֪�ʺ�[%d]�ļƵ���Ϣ������δ�Ǽǡ�", 
									//										m_aServerName[nServerIndex], idFeeAccount);
									//									LOGPOINT("��Ϸ������[%s]�ϴ���һ��δ֪�ʺ�[%d]�ļƵ���Ϣ������δ�Ǽ�", 
									//										m_aServerName[nServerIndex], idFeeAccount);
									//								}
									//----------------------------------------------end-------------------------------------------------
								}
								else
								{
									LOGERROR("˫�ص�¼�����ID[%d]ʹ���ʺ�[%s][%d]��¼��[%s]�����յ�[%s]�ϴ��˵ĵļƵ���Ϣ����ұ������ߡ�", 
										idAccount, szFeeAccount, idFeeAccount, szServerName, m_aServerName[nServerIndex]);
									Kickout_0(idAccount, m_aServerName[nServerIndex]);
								}
							}
							
							int	nPointCount = g_pOnlineTable->PointFee(idAccount, m_aServerName[nServerIndex]);		// return -n: ��������ƥ��
							//							LOGPOINT("[%s]�ϴ�[%d]��POINT_FEE������[%d]��ʣ��[%d]", 
							//								m_aServerName[nServerIndex], idAccount, nPointCount, nPoint-1);
							// ˫�ص�¼������
							//						if(nPointCount < 0)		// <0 : error
							{
								//							LOGERROR("...˫�ص�¼�������POINT_FEE���ʺ�[%d]�������ߡ�", idAccount);
								//							Kickout_0(idAccount, m_aServerName[nServerIndex]);
							}
							
							if(m_aState[nServerIndex] == STATE_NORMAL)
								::SetServerState(nServerIndex, c_flagPoint);
							else
								::SetServerState(nServerIndex, c_flagPoint2);
						} // GetAttr()
						else
						{
							LOGERROR("�����ͻ��FEE_POINT����¼�ʺŲ��ɹ�");
						}
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::FEE_TICK");
					}
				}
				break;
			case CMsgFee::HEARTBEAT:		//���յ���Ϸ�������ϴ���������Ϣ
				{
					try
					{
					/*// ��Ϣ���
					if(idAccount == ID_NONE)
					{
					LOGERROR("�����HEARTBEAT��Ϣ[%d]��", idAccount);
					return false;
					}*/
						
						LOGMSG("���յ�[%s]��Ϸ�������ϴ���������Ϣ.", m_aServerName[nServerIndex]);
						// �޲�����ONPROCESS�л��Զ�ˢ������ʱ�䡣
						::SetServerState(nServerIndex, c_flagHeartbeat);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::HEARTBEAT");
					}
				}
				break;
			case CMsgFee::SERVER_BUSY:					//��Ϸ��������Ӧ�ò����ϴ�����Ϣ����Ϊ�����Ŷӻ���
				{
					try
					{
						SetServerBusy(nServerIndex);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::SERVER_BUSY");
					}
				}
				break;
			case CMsgFee::SERVER_FULL:					//��Ϸ��������Ӧ�ò����ϴ�����Ϣ����Ϊ�����Ŷӻ���
				{
					try
					{
						SetServerFull(nServerIndex);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::SERVER_FULL");
					}
				}
				break;
			case CMsgFee::SERVER_CLOSE:					//���ʣ�δ���κδ���
				{
					try
					{
						PrintText("���յ�������[%s]�Ĺر���Ϣ��", m_aServerName[nServerIndex]);
						//??? SetServerClose(nServerIndex);
						//??? ::SetServerState(nIndex, c_flagOffline);
					}
					catch(...)
					{
						LogSave("CPointThread::ProcessMsg...case _MSG_FEE:case CMsgFee::SERVER_CLOSE");
					}
				}
				break;
			default:
				LOGERROR("��Ϸ������[%s]�Ʒ���Ϣ������[%d]����", m_aServerName[nServerIndex], cMsg.m_pInfo->ucType);
			}
		}
		break;
	case _MSG_QUERYFEE:
		{
			CMsgQueryFee	cMsg;
			cMsg.Create(pBuf, nLen);

			// ��Ϣ���
			if(cMsg.m_pInfo->idAccount == ID_NONE)
			{
				LOGERROR("�����_MSG_QUERYFEE��Ϣ[%d]��", cMsg.m_pInfo->idAccount);
				return false;
			}

			int		nFeeType	= c_typeNone;
			DWORD	dwData		= 0;
			int		nTime		= 0;
			char	szServerName[_MAX_NAMESIZE];
			OBJID	idFeeAccount;
			char	szFeeAccount[_MAX_ACCOUNT_NAMESIZE];
			if(g_pOnlineTable->GetAttr2(cMsg.m_pInfo->idAccount, nFeeType, szServerName, idFeeAccount, szFeeAccount))
			{
				switch(nFeeType)
				{
				case	c_typePoint:
					::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
					if(g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
					{
						dwData	= g_cDatabase.GetPoint();
						g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
					}
					else
					{
						LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
					}
					::ReleaseMutex(g_xDatabase);	//------------------------------------------
					break;
				case	c_typeTime:
					::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
					if(g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
					{
						nTime	= g_cDatabase.GetPointTime();
						g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
					}
					else
					{
						LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
					}
					::ReleaseMutex(g_xDatabase);	//------------------------------------------
					break;
				case	c_typeNetBarTime:
					/* �ݲ�����
					::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
					if(g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
					{
						dwData	= g_cDatabase.GetLicence();
						g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
					}
					else
					{
						LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
					}
					::ReleaseMutex(g_xDatabase);	//------------------------------------------
					break;
					//*/
				case	c_typeNetBarPoint:
				case	c_typeISP:
				case	c_typeFree:
					break;
				}
			}
/*
			::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
			if(g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
			{
				nType	= g_cDatabase.GetType();
				dwData	= g_cDatabase.GetPoint();
				nTime	= g_cDatabase.GetPointTime();
				g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
			}
			else
			{
				LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪���[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
			}
			::ReleaseMutex(g_xDatabase);	//------------------------------------------
//*/
			// ������Ϣ
			cMsg.m_pInfo->ucType	= nFeeType;
			cMsg.m_pInfo->dwData	= dwData;
			cMsg.m_pInfo->nTime		= nTime;
			m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
//			LOGPOINT("���͸�[%s]��Ϸ������[%d]�ʺŵļƷѲ�ѯ��Ϣ������[%d]������[%d]��ʱ��[%04d-%02d-%02d]", 
//						m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount, 
//						nFeeType, dwData, nTime/10000, (nTime/100)%100, nTime%100);
		}
		break;
	default:
		LOGERROR("��Ϸ�������ϴ���δ֪����Ϣ����[%d]", unMsgType);
		LOGPOINT("���յ�[%s]��Ϸ�������ϴ���δ֪��Ϣ.", m_aServerName[nServerIndex], unMsgType);
	}

	return true;
}

// ����0������
OBJID	CPointThread::GetServerIndex_0(const char * pServerName)
{
	for(int i = 1; i < MAXGAMESERVERS; i++)	// ����0��0Ϊ����
	{
		if(strcmp(m_aServerName[i], pServerName) == 0)
			break;
	}

	if(i < MAXGAMESERVERS)
	{
		if(m_aServerSocket[i].IsOpen())
		{
			return i;
		}
		else
		{
			if(m_aServerName[i][0])
			{
				LOGWARNING("ȡ������IDʱ����Ϸ������[%s]�Ѿ��ر�", m_aServerName[i]);
				LOGSERVER("������[%s]�Ѿ��ر��ˡ�", m_aServerName[i]);
				PrintText("��������%s���Ѿ��ر��ˡ�", m_aServerName[i]);
			}
			Clear(i);		// SOCKET�رպ�ɾ����Ϸ��������������ͬ��
		}
	}

	return 0;
}

// ���������Ƿ����ߣ�������ʱת����ʾ״̬����������ҵ�¼��
bool	CPointThread::CheckHeartbeat(int nIndex)
{
	// �ָ�BUSY��FULL
	if((m_aState[nIndex] == STATE_BUSY || m_aState[nIndex] == STATE_FULL) && clock() > m_aServerDelay[nIndex])
	{
		m_aState[nIndex] = STATE_NORMAL;
		::SetServerState(nIndex, c_flagNormal);
	}

	bool	ret = false;
	// �������
	if(m_aHeartbeatLast[nIndex] && (clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC > HEARTBEATKICKSECS)
	{
		LOGWARNING("��Ϸ������[%s]��Ϊ��Ӧ��ʱ[%d]�룬���ӱ�ǿ�жϿ�", 
						m_aServerName[nIndex], (clock() - m_aHeartbeat[nIndex])/CLOCKS_PER_SEC);
		LOGSERVER("��Ӧ��ʱ��������[%s]�ѶϿ�����!", m_aServerName[nIndex]);
		PrintText("��Ӧ��ʱ����������%s���ѶϿ�����!", m_aServerName[nIndex]);
		m_aServerSocket[nIndex].Close();
		Clear(nIndex);
		ret = true;
	}
	else if((clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC > HEARTBEATINTERVALSECS
				&& (clock() - m_aHeartbeatLast[nIndex]) / CLOCKS_PER_SEC > HEARTBEATINTERVALSECS)
	{
		CMsgFee	cMsg;
		cMsg.Create(666666, CMsgFee::HEARTBEAT);		// 666666: ����ʺ�ID��������
		if(m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize()))
		{
			LOGERROR("��Ϸ������[%s]��Ӧ��ʱ[%d]�룬�ѷ���һheartbeat��Ϣ", 
						m_aServerName[nIndex], (clock() - m_aHeartbeat[nIndex])/CLOCKS_PER_SEC);
		}
		else
		{
			LOGWARNING("��Ϸ������[%s]�����ѹرգ��޷�SEND������Ϣ", m_aServerName[nIndex]);
			LOGSERVER("��������ʧ�ܣ�������[%s]�ѶϿ�����!", m_aServerName[nIndex]);
			PrintText("��������ʧ�ܣ���������%s���ѶϿ�����!", m_aServerName[nIndex]);
			Clear(nIndex);
		}
		m_aHeartbeatLast[nIndex] = clock();
	}

	return ret;
}

void CPointThread::Clear(int nIndex, bool flag /*= true*/) 
{ 
	m_aState[nIndex] = STATE_OFFLINE;
	
	//  [5/30/2007]�������-----------begin---------------------------
	if( flag )
	{
		g_pOnlineTable->RemoveByServerName( m_aServerName[ nIndex] );

	}
	//----------------------------------end-----------------------------

	m_aServerName[nIndex][0] = m_aHeartbeat[nIndex] = m_aHeartbeatLast[nIndex] = m_aServerDelay[nIndex] = 0; 
	if(flag)
		::SetServerState(nIndex, c_flagOffline);
}

void CPointThread::SetServerBusy(int nServerIndex) { 
	m_aServerDelay[nServerIndex] = clock() + SERVER_BUSY_DELAY_SECS*CLOCKS_PER_SEC;
	m_aState[nServerIndex] = STATE_BUSY;
	::SetServerState(nServerIndex, c_flagStop);
}

void CPointThread::SetServerFull(int nServerIndex) {
	m_aServerDelay[nServerIndex] = clock() + SERVER_FULL_DELAY_SECS*CLOCKS_PER_SEC;
	m_aState[nServerIndex] = STATE_FULL;
	::SetServerState(nServerIndex, c_flagStop);
}

bool CPointThread::AddTooLoginInfo(OBJID idAccount , const char * pGameServer)
{
	bool hr=false;
	hr = m_objInspectManage.WatchToolInfo(idAccount,pGameServer,1);
	return hr;
}






