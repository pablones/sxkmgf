// ��¼�߳���
// �ɽ��ޣ�2001.11.20
//_version 0.81100324
//////////////////////////////////////////////////////////////////////
#include "AllHeads.h"
#include "LoginThread.h"
#include "MsgAccount.h"
#include "MsgConnect.h"
#include "MsgLogin.h"
#include "MsgQueryFee.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
#undef	LOCKTHREAD		// ��¼�̲߳���Ҫ�����
#define	LOCKTHREAD
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoginThread::CLoginThread(u_short nPort) 
		: CThreadBase(), m_cListenSocket(nPort)
{
	LOCKTHREAD;

	m_pBanIPs = new CBanIP[MAXBANIPS];
}

CLoginThread::~CLoginThread()
{
	LOCKTHREAD;

	delete m_pBanIPs;
}

//#define	CRITSECT	//	CSingleLock(&m_xCtrl, true);

//#define	LOCK	//?	{CSingleLock(&m_xCtrl, true);
//#define	UNLOCK	//?	}

// ���߳���û���ⲿ��Ϣ���룬û�й����ͻ
void	CLoginThread::OnInit()
{
	LOCKTHREAD;

	try{
		m_cRc5.Rc5InitKey(RC5PASSWORD_KEY);
		m_cRc5NetBar.Rc5InitKey(RC5BARPASSWORD_KEY);

		m_cListenSocket.Open();
		LOGMSG("��¼�߳���������");

		m_AccountManager.PureServerOnlineMonth();///��¼�̳߳�ʼ���ɹ�����������һ���������߼�¼

	}catch(...) { LOGCATCH("��¼�̳߳�ʼ���쳣�˳�"); }
}

bool	CLoginThread::OnProcess()
{
//	LOCKTHREAD;
	CSingleLock xLock(&m_xCtrl, true); ASSERT(xLock.IsLocked());
	//////////////////////////////////////////////////////////////////////////
//	Sleep(5000);////softworms-debug֮��-2007-10-07
	//////
	try{
		time_t	tStart = clock();
		long	nUsed = 0;
		
		if(g_bEnableLogin)		// �����¼�ˣ�)
		{
			// select
			fd_set	readmask;
			FD_ZERO(&readmask);
			if(!m_cListenSocket.IsOpen())
				m_cListenSocket.Rebuild();
			if(m_cListenSocket.IsOpen())
				FD_SET(m_cListenSocket.Socket(), &readmask);//�����װ��ּ���readmask
			
			//06.10.19�������ע��:�˴�����Ҫ�Ż�Ϊʵ������,������Ч�ʸ��Ʋ���,����ͻ��˹���,���޸�MAXCONNECTSֵ
			//			for(int i = 0; i < MAXCONNECTS; i++)
			//			{
			//				if(m_aServerSocket[i].IsOpen())
			//					FD_SET(m_aServerSocket[i].Socket(), &readmask);
			//			}
			struct timeval	timeout = {0,0};
			int ret = select(FD_SETSIZE, &readmask, (fd_set *) 0, (fd_set *) 0, &timeout);
			
			// ����Ƿ���յ�������
			if(ret == -1)	// error
			{
				//			m_cListenSocket.Close();
				LOGERROR("��¼�߳� select ����[%d]", WSAGetLastError());
				//			PrintText("��¼�̳߳���SOCKET���رա�%d ��󽫻��Զ��ؽ�", REBUILDLISTENDELAYSEC);
			}
			// �µ�����--------------------------------------begin------------------------------------------------------------------------------
			else if(ret > 0 && FD_ISSET(m_cListenSocket.Socket(), &readmask))
			{
				//				FD_CLR(m_cListenSocket.Socket(), &readmask);
				//				ret--;
				u_long	nClientIP;
				SOCKET sockNew = m_cListenSocket.Accept(nClientIP);
				if(sockNew != INVALID_SOCKET)
				{
					InterlockedIncrement(&s_nLoginAccept);
					
					// ȡ�Է�IP����BAN��
					bool	bBan = false;
					//				sockaddr_in	inAddr;
					//				memset(&inAddr, 0, sizeof(inAddr));
					//				int		nLen = sizeof(inAddr);
					//				if(getpeername(sockNew, (sockaddr *)&inAddr, &nLen) == 0)		// ����ֱ����ACCEPT����
					{
						//					unsigned long	nClientIP = inAddr.sin_addr.S_un.S_addr;
						//06.10.19�������:���¿��Ż�,���˶���IP,ѭ�����ٴ�,�������ܸ���Ч�ʲ��Ǻܴ�
						for(int i = 0; i < MAXBANIPS; i++)
						{
							if(m_pBanIPs[i].ClientIP() == nClientIP && m_pBanIPs[i].IsBan())
							{
								bBan = true;		// ���ø�IP��¼
								break;
							}
						}
					}
					
					// ������߱�
					if(bBan)
					{
						// �ر�SOCKET
						//�޸�SOCKET���Գ������ر���
						struct linger ling;
						ling.l_onoff = 1;
						ling.l_linger = 0;
						setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));
						closesocket(sockNew);		// �������ر�SOCKET���������ó������ر�
						InterlockedDecrement(&s_nSocketCount);
						
						//��¼��־
						union FORMATIP
						{
							unsigned long ul;
							struct 
							{
								unsigned char a;
								unsigned char b;
								unsigned char c;
								unsigned char d;
							};
						};
						FORMATIP ip;
						ip.ul = nClientIP;
						
						LOGWARNING("isBan��IP[%d.%d.%d.%d]......",ip.a,ip.b,ip.c,ip.d );
					}
					else
					{
						// �ҿ�λ��
						bool	bSuccess = false;
						for(int i = 0; i < MAXCONNECTS; i++)
						{
							if(!m_aServerSocket[i].IsOpen())
							{
								bSuccess = true;
								break;	// �ɹ�
							}
						}
						
						// �������
						if(bSuccess)
						{
							m_aServerSocket[i].Open(sockNew, nClientIP);
							m_aServerSocket[i].GetPeerIP();		// ����ȡIP
						}
						else
						{
						/*
						LOGWARNING("�ѽ��յ�һ�������ӣ������ӱ����������޸�MAXCONNECTS�������±������");
						
						  //	�޸�SOCKET���Գ������ر���
						  struct linger ling;
						  ling.l_onoff = 1;
						  ling.l_linger = 0;
						  setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));
						  closesocket(sockNew);
						  InterlockedDecrement(&s_nSocketCount);
							//*/
							//*else
							// ��ռ���е����ӣ��ԶԿ����������ӹ����������Ż�Ϊ�ҳ������ߣ�����BAN��??
							srand((unsigned int) clock());
							int		nPos = rand() % MAXCONNECTS;
							char	bufTemp[IPSTRSIZE];
							ASSERT(m_aServerSocket[nPos].IsOpen());
							SafeCopy(bufTemp, m_aServerSocket[nPos].GetPeerIP(), IPSTRSIZE);
							if(FD_ISSET(m_aServerSocket[nPos].Socket(), &readmask))
							{
								FD_CLR(m_aServerSocket[nPos].Socket(), &readmask);
								ret--;
							}
							LOGERROR("CLoginThread::OnProcess �յ�һ��������,�������ر��˾��׽���!");
							m_aServerSocket[nPos].Close(true);
							// ����SOCKETʹ��
							m_aServerSocket[nPos].Open(sockNew, nClientIP);
							LOGWARNING("���ӱ�������MAXCONNECTS����̫С��[%s]��ռ��[%s]�����ӱ�", 
								m_aServerSocket[nPos].GetPeerIP(), bufTemp);
							//*/
						}
					}
				}
			}
			//---------------------------------------------end------------------------------------------------------------------------------
			//			for(int i = 0; i < MAXCONNECTS; i++)
			//			{
			//				if(m_aServerSocket[i].IsOpen())
			//					FD_SET(m_aServerSocket[i].Socket(), &readmask);
			//			}
			//			struct timeval	timeout = {0,0};
			
			// ���յ���¼��Ϣ������Ҫ�Ż�??
			
			
			
			int realProcCount = 0;//ʵ�ʴ������
			for( int i = 0; i < MAXCONNECTS; i++)
			{
				fd_set	readmaskProc;
				FD_ZERO(&readmaskProc);
				if(m_aServerSocket[i].IsOpen())
				{
					FD_SET(m_aServerSocket[i].Socket(), &readmaskProc);
				}
				else
				{
					continue;
				}
				
				
				struct timeval	timeoutProc = {0,0};
				int retProc = select(FD_SETSIZE, &readmaskProc, (fd_set *) 0, (fd_set *) 0, &timeoutProc);
				
				if(retProc == 0 )//��ʱ.�´�����
				{
					//					char szDebug[512];
					//					sprintf(szDebug,"\nif(retProc == 0 || retProc == -1) i:%d",i);
					//					::OutputDebugString(szDebug);
					//					::LogSave(szDebug);
					continue;
				}
				
				if(retProc == -1)//����
				{
					//�ر�.continue
					char szDebug[512];
					sprintf(szDebug,"\nif(retProc == -1) i:%d",i);
					::OutputDebugString(szDebug);
					::LogSave(szDebug);
					LOGERROR("CLoginThread::OnProcess �յ�һ��������,�������ر��˾��׽���!");
					m_aServerSocket[i].Close();//������ص�
					continue;			
				}
				
				if(FD_ISSET(m_aServerSocket[i].Socket(), &readmaskProc))
				{
					//					FD_CLR(m_aServerSocket[i].Socket(), &readmask);
					//					ret--;
					
					char	bufMsg[_MAX_MSGSIZE];
					int		nMsgLen = 0;
					if(m_aServerSocket[i].Recv(bufMsg, nMsgLen) && nMsgLen)
					{
						ProcessMsg(i, bufMsg, nMsgLen);
						realProcCount++;
						char szDebug[512];
						sprintf(szDebug,"\nProcMsg i:%d,curTime:%d",i,clock());
						::OutputDebugString(szDebug);
						continue;
					}
					else
					{
						//m_aServerSocket[i].Close();//������ص�
						continue;
					}
				}
				else
				{
					::OutputDebugString("else //if(FD_ISSET(m_aServerSocket[i].Socket(), &readmask))");
				}
			}
			nUsed = clock() - tStart;
			InterlockedExchangeAdd(&s_nAllTimeSum, nUsed);
			InterlockedIncrement(&s_nLoginCount);
			InterlockedExchangeAdd(&s_nRealProcLogin,realProcCount);
			InterlockedExchangeAdd(&s_nAvgServerTime, nUsed);
			if(nUsed > InterlockedExchangeAdd(&s_nMaxServerTime, 0))
				InterlockedExchange(&s_nMaxServerTime, nUsed);
		}	// if(g_bEnableLogin)
		
		long	nRemain = LOGINLOOPDELAY - nUsed;
		if(nRemain < 0 || nRemain > LOGINLOOPDELAY)
			nRemain = 0;		//? �л����Ƶ��̡߳��Ʒ��߳�����
		
		if( nRemain )//06.10.21�������,��֤��½�߳�����
		{
			UNLOCKHEAD;
			Sleep(nRemain);
		
		}
		return true;
	}
	catch(...) 
	{ 
		LOGCATCH("��¼�߳���ѭ���쳣���Լ�������С�"); 
		PrintText("��¼�߳���ѭ�������Լ��������...");
		return true; 
	}

}

void	CLoginThread::OnDestroy()
{
	LOCKTHREAD;

	try{
		m_cListenSocket.Close();
		for(int i = 0; i < MAXCONNECTS; i++)
		{
//			if(m_aServerSocket[i].IsOpen())
				m_aServerSocket[i].Close();
		}

		LOGMSG("��¼�߳������ر�");

	}catch(...) { LOGCATCH("��¼�̹߳ر�ʱ�쳣�˳�"); }
}

///////////////////////////////////////////////////////////////////////////////
// �ڲ�����
///////////////////////////////////////////////////////////////////////////////
bool	CLoginThread::ProcessMsg(int nIndex, char * pBuf, int nLen)
{
// 	  	if(key1==ACCOUNT_KEY1 && key2==ACCOUNT_KEY2)
	EncryptPacket((BYTE*)pBuf,nLen);///�Գ��˰�ͷ֮������ݽ��������

	unsigned short unMsgType = CNetMsg::GetType(pBuf, nLen);
	int	nMsgLen = CNetMsg::GetSize(pBuf, nLen);

	if(unMsgType == _MSG_ACCOUNT && nMsgLen == nLen )
	{
		return ProcessMsgAccount(nIndex, pBuf, nLen);
	}
	if(unMsgType == _MSG_BARACCOUNT && nMsgLen == nLen )
	{
		return ProcessMsgBarAccount(nIndex, pBuf, nLen);
	}
	else if(unMsgType == _MSG_CONNECT && nMsgLen == nLen)
	{
		if(STAT_SERIAL)
			return ProcessMsgConnect(nIndex, pBuf, nLen);
		return true;
	}
	else
	{
		char *	pClientIP	= m_aServerSocket[nIndex].GetPeerIP();
		unsigned long nClientIP	= inet_addr(pClientIP);
		AddBan(nClientIP, pClientIP, "δ֪");
		if(nLen >= 20)
		{
			pBuf[19] = 0;
			LOGWARNING("һ���ͻ���[%s]�ϴ��˷Ƿ���¼��Ϣ����Ϣͷ[%d][%d]������[%d]��������[%s]", 
								pClientIP, nMsgLen, unMsgType, nLen, (pBuf+4));
		}
		else
			LOGWARNING("һ���ͻ���[%s]�ϴ��˷Ƿ���¼��Ϣ����Ϣͷ[%d][%d]������[%d]", 
							pClientIP, nMsgLen, unMsgType, nLen);
		m_aServerSocket[nIndex].Close(true);			// �����ر�
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ˽�к���
/////////////////////////////////////////////////////////////////////////////////////////////
bool	AppendPassword(OBJID idAccount, LPCTSTR szAccount, LPCTSTR szPassword)
{
	bool	bUpdate = false;
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(szAccount, szPassword))		// ���ʺ�
	{	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
		g_cDatabase.Destroy();
		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		g_pOnlineTable->SetPassword(idAccount, szPassword);
		bUpdate = true;
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------
	return bUpdate;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	CheckISP(OBJID idAccount, unsigned long nClientIP, LPCTSTR pClientIP, 
				 OBJID& idFeeAccount, char* szFeeAccount, int& nPoint, DWORD& nPointTime, char* szNotifyMsg)	// ����ֵ
{
	bool	bIsISP = false;

	unsigned char ip1,ip2,ip3,ip4;
	ip1 = *( ((unsigned char*)&nClientIP) + 0);
	ip2 = *( ((unsigned char*)&nClientIP) + 1);
	ip3 = *( ((unsigned char*)&nClientIP) + 2);
	ip4 = *( ((unsigned char*)&nClientIP) + 3);

	for(int i = 0; i < g_setISPList.size(); i++)
	{
		int	fi1, fi2, fi3, fi4;
		if(sscanf(g_setISPList[i].szFirstIP, "%d.%d.%d.%d", &fi1, &fi2, &fi3, &fi4) != 4)
		{
			LOGERROR("invalud first ip format [%s]", g_setISPList[i].szFirstIP);
			continue;
		}

		int	la1, la2, la3, la4;
		if(sscanf(g_setISPList[i].szLastIP, "%d.%d.%d.%d", &la1, &la2, &la3, &la4) != 4)
		{
			LOGERROR("invalud last ip format [%s]", g_setISPList[i].szLastIP);
			continue;
		}

		if(ip1 >= fi1 && ip1 <= la1 
			&& ip2 >= fi2 && ip2 <= la2 
			&& ip3 >= fi3 && ip3 <= la3 
			&& ip4 >= fi4 && ip4 <= la4 )
		{
			if(pClientIP && g_pOnlineTable->CheckUniqueIP(pClientIP, idAccount))
			{
				::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
				if(g_cDatabase.Create(g_setISPList[i].szAccount, g_setISPList[i].szPassword))
				{	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
					if(g_cDatabase.GetPoint() > 0)
					{
						SafeCopy(szFeeAccount, g_cDatabase.GetName(), _MAX_ACCOUNT_NAMESIZE);
						idFeeAccount	= g_cDatabase.GetID();
						nPoint			= g_cDatabase.GetPoint();
						bIsISP	= true;
						nPointTime		= g_cDatabase.GetPointTime();
					}
					else
					{
						// ISPת��ͨ�ʺ�
						sprintf(szNotifyMsg, FLAG_ISP_TO_NORMAL);
					}
					g_cDatabase.Destroy();
					// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
				}
				::ReleaseMutex(g_xDatabase);	//------------------------------------------
			}
			else
			{
				// ISPת��ͨ�ʺ�
				sprintf(szNotifyMsg, FLAG_ISP_TO_NORMAL);
			}

			return bIsISP;	// found it!
		}
	} // for

	if(pClientIP)
	{
		::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
		if(g_cDatabase.Create(c_typeISP, pClientIP))
		{	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
			SafeCopy(szFeeAccount, g_cDatabase.GetName(), _MAX_ACCOUNT_NAMESIZE);
			idFeeAccount	= g_cDatabase.GetID();
			nPoint			= g_cDatabase.GetPoint();
			bIsISP	= true;
			nPointTime		= g_cDatabase.GetPointTime();
			g_cDatabase.Destroy();
			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		}
		::ReleaseMutex(g_xDatabase);	//------------------------------------------
	}

	return bIsISP;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	Check7DaysNodify(DWORD nPointTime, char* szNotifyMsg)		// return szNotifyMsg
{
	time_t	tCurr = time(NULL);
	struct tm	tmCurr;
	tmCurr = *localtime(&tCurr);
	tmCurr.tm_mday	+= ACCOUNT_AHEAD_DAYS;
	time_t	tAhead	= mktime(&tmCurr);
	if(tAhead != (time_t)-1)
	{
		tm*		pTm	= localtime(&tAhead);
		int		nAhead	= (pTm->tm_year+1900)*10000 + (pTm->tm_mon+1)*100 + pTm->tm_mday;
		if(nAhead > nPointTime)
		{
			nPointTime %= 100000000;	// ��ֹszNotifyMsg���
			sprintf(szNotifyMsg, "%d��%d��%d��", 			// ��ע�⣺��һ�ֽڱ���Ϊ���֡��Ա���Ϸ������ͬ����
						(nPointTime/10000) % 10000, (nPointTime/100) % 100, nPointTime % 100);
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	GetAccount(LPCTSTR szAccount, LPCTSTR szPassword, 
					OBJID& idAccount, int& nType, int& nPoint, DWORD& nPointTime, int& nBlock)			// ����ֵ
{
	idAccount	= ID_NONE;
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(szAccount, szPassword))
	{	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
		idAccount		= g_cDatabase.GetID();
		nBlock			= g_cDatabase.GetOnline()||g_cDatabase.GetStatus();			// ��ONLINE
		nPoint			= g_cDatabase.GetPoint();
		nType			= /*g_cDatabase.GetType()*/6;
		nPointTime		= g_cDatabase.GetPointTime();
		g_cDatabase.Destroy();	
		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
#ifdef _THUNDER_LOGIN
	else
	{
		if(g_cDatabase.CreateNewAccount(szAccount, szPassword))
		{
			idAccount		= g_cDatabase.GetID();
			nBlock			= g_cDatabase.GetOnline()||g_cDatabase.GetStatus();			// ��ONLINE
			nPoint			= g_cDatabase.GetPoint();
			nType			= g_cDatabase.GetType();
			nPointTime		= g_cDatabase.GetPointTime();
			g_cDatabase.Destroy();	
		}
	}
#endif
	::ReleaseMutex(g_xDatabase);	//------------------------------------------
	return	(idAccount != ID_NONE);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	GetFeeType(LPCTSTR szAccount, LPCTSTR szPassword, int& nFeeType, int& nPoint)		// return nType
{
	nFeeType	= c_typeNone;
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(szAccount, szPassword))		// ���ʺ�
	{	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
		nFeeType	= g_cDatabase.GetType();
		nPoint		= g_cDatabase.GetPoint();
		g_cDatabase.Destroy();
		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------
	return (nFeeType != c_typeNone);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	GetLicence(LPCTSTR szBarAccount, LPCTSTR szBarPassword, 
					OBJID& idFeeAccount, int& nPoint, DWORD& nPointTime, 						// ����ֵ
					int& nLicenceType, int& nLicence, char* szNetBarIP, char* szIPMask)			// ����ֵ
{
	idFeeAccount	= ID_NONE;
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(szBarAccount, szBarPassword))		// ���ʺ�
	{	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
		idFeeAccount	= g_cDatabase.GetID();
		nPoint			= g_cDatabase.GetPoint();
		nPointTime		= g_cDatabase.GetPointTime();
		nLicenceType	= g_cDatabase.GetType();
		nLicence		= g_cDatabase.GetLicence();
		SafeCopy(szNetBarIP, g_cDatabase.GetNetBarIP(), _MAX_NAMESIZE);
		SafeCopy(szIPMask, g_cDatabase.GetIPMask(), _MAX_NAMESIZE);
		g_cDatabase.Destroy();
		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------
	return (idFeeAccount != ID_NONE);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	IsFreeServer(LPCTSTR szServer)
{
	for(int i = 0; i < g_nServerAccount; i++)
	{
		if(g_aServerAccount[i].m_bFree && strcmp(g_aServerAccount[i].m_szServerName, szServer) == 0)
			return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	CLoginThread::ProcessMsgAccount(int nIndex, char * pBuf, int nLen)
{
	CMsgAccount	cMsg;
	cMsg.Create(pBuf, nLen);
// #ifdef	RC5ENCRYPT
// 	m_cRc5.Rc5Decrypt(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);
// #endif
	FormatStr(cMsg.m_pInfo->szAccount, _MAX_ACCOUNT_NAMESIZE);		// ��β0��ɾβ��
	FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// ��β0��ɾβ��
	FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// ��β0��ɾβ��

	FormatStr(cMsg.m_pInfo->szMacClient, _MAX_NAMESIZE);		// ��β0��ɾβ��
	
	char *	pClientIP						= m_aServerSocket[nIndex].GetPeerIP();
	unsigned long nClientIP					= inet_addr(pClientIP);
	DWORD	nAuthenID						= NewAuthenID(nClientIP);
	char *	pErrMsg							= "��¼ʧ��";
	char	szNotifyMsg[_MAX_NAMESIZE]		= "";
	OBJID	idAccount						= ID_NONE;
	OBJID	idFeeAccount					= ID_NONE;
	char	szFeeAccount[_MAX_ACCOUNT_NAMESIZE]		= "";
	int		nFeeType						= c_typeNone;
	int		nPoint							= 0;
	DWORD	nPointTime						= 0;
	int		nBlock							= false;
	bool	bRejoin							= false;
	char	szOldServerName[SERVERNAMESIZE]	= "";
	char	szOldFeeAccount[_MAX_ACCOUNT_NAMESIZE]	= "";
	char 	szPassword[_MAX_NAMESIZE]	= "";		// ���ñ���
	OBJID	idOldFeeAccount				= ID_NONE;	// ���ñ���
	char	szOldIP[_MAX_NAMESIZE]		= "";		// ���ñ���

	if(!GetAccount(cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, 
		idAccount, nFeeType, nPoint, nPointTime, nBlock))			// ����ֵ
	{
		AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
		RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errPassword, "�ʺ���������");
		LOGACCOUNT("���[%s]����[%s]��¼���ʺ���������, IP��ַ[%s]", 
			cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, pClientIP);
		return false;
	}


	if(g_pOnlineTable->GetAttr(cMsg.m_pInfo->szAccount, idAccount, szPassword, nAuthenID,			// ��nAuthenID��szOldServerName��szOldFeeAccount ������
		szOldServerName, szOldFeeAccount, idOldFeeAccount, szOldIP))		//??? ���ߡ��ú�����������ADDNEW�ķ�ԭ���Բ���
	{
// 		bRejoin	= true;
		if(nAuthenID)///������Ʊ��û���õ�,����
		{
			AllowLogin(nIndex, idAccount, nAuthenID, szOldServerName);
			return true;
		}

		if(g_pOnlineTable->IsAccountTimeout(idAccount))
		{
			g_pOnlineTable->RemoveAccount(idAccount);
		}
		else
		{
			UINT nTimeout=g_pOnlineTable->GetAccountTimeout(idAccount);
			if(nTimeout==0)
			{
				g_pOnlineTable->SetAccountTimeout(idAccount);///����ʺ�������Ч�ȴ�6��,�´γ��Ե�¼ʱɾ֮
				g_pPointThread->Kickout( idAccount );
			}
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errOnline, "�ظ�ʹ���ʺ�,���Ժ�����!");
			return false;
		}

	}



	///�µ��ظ��ʺŴ�������
	if(m_AccountManager.IsOnlineAccount(idAccount))
	{
		AccountInfo info={0};
		m_AccountManager.GetAccount(idAccount,info);

		if(info.nAuthenID)///������Ʊ��û���õ�,����
		{
			AllowLogin(nIndex, info.idAccount, info.nAuthenID, info.szServerName);
			return true;
		}

		///������ʺų�ʱ��ɾ������Ч���ʺŲ�����ҽ�ȥ
		if(m_AccountManager.IsAccountTimeout(idAccount))
			m_AccountManager.RemoveAccount(idAccount);
		else
		{
			if(info.nTimeout==0)
			{
				m_AccountManager.SetAccountTimeout(idAccount);///����ʺ�������Ч�ȴ�6��,�´γ��Ե�¼ʱɾ֮
				///���ִ��ʺ��ظ���¼,�߳�����¼��־
 				g_pPointThread->Kickout( idAccount,info.szServerName);
			}
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errOnline, "�ظ�ʹ���û��ʺ�,���Ժ�����!");
			LOGACCOUNT("���[%s]����[%s]��¼�����µ��ظ��ʺŻ��Ƽ�⵽���ظ���¼, IP��ַ[%s]", 
				cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, pClientIP);
			return false;
		}
	}

	///ʹ���ʺŷ������Ż�����ѡ���㷨�滻�ͻ��˵�Ĭ��ѡ��
	if(strlen(cMsg.m_pInfo->szServer)<=0)
		g_pPointThread->TakeBestServer(cMsg);///�ͻ���û��ѡ�ߣ���ָ��һ����õķ���������

	if(strlen(cMsg.m_pInfo->szServer)<=0)///���δ��ѡ����ѷ������򷵻ط������������Ĵ�����Ϣ
	{
		RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerFull, "��������������,���Ժ�����!");
		return false;
	}

	// ����
	if(nBlock)
	{
		RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBan, "���ʺű����");
		return false;
	}

	// ���Ƿ���¼
	if(nFeeType == c_typeNetBarPoint || nFeeType == c_typeNetBarTime || nFeeType == c_typeISP)
	{
		RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "���ʺŲ��ܵ�¼");
		return false;
	}

	if(IsFreeServer(cMsg.m_pInfo->szServer))
	{
		// ��ѷ�����
		nFeeType		= c_typeFree;
//		SafeCopy(szNotifyMsg, "��ӭ�μӲ���", _MAX_NAMESIZE);
	}
	else if(CheckISP(idAccount, nClientIP, pClientIP, 
						idFeeAccount, szFeeAccount, nPoint, nPointTime, szNotifyMsg))		// ����ֵ
	{
		// ISP����
		nFeeType		= c_typeISP;
	}
	else
	{
		idFeeAccount	= idAccount;
		SafeCopy(szFeeAccount, cMsg.m_pInfo->szAccount, _MAX_ACCOUNT_NAMESIZE);
	}

// 	if(bRejoin)
// 	{
// 		g_pPointThread->Kickout( idAccount );
		//RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errOnline, "�ظ�ʹ���ʺ�");
		//return false;

// 	}

	// ���������
	switch(nFeeType)
	{
	case c_typePoint:	// û�д���Ƶ㿨��ʱ��(�㿨ҲӦ����ʱ��)
	case c_typeISP:
		if(nPoint <= 0)
		{
			if(ENABLE_LOGIN_NO_POINT)
			{
				SafeCopy(szNotifyMsg, FLAG_NO_POINT, _MAX_NAMESIZE);
			}
			else
			{
				RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errNotPoint, "Сʱ�����þ�");
				return false;
			}
		}
		break;
	case c_typeTime:
		{
			time_t	tCurr = time(NULL);
			tm *	pTm = localtime(&tCurr);
			DWORD	nCurrTime = (pTm->tm_year+1900)*10000 + (pTm->tm_mon+1)*100 + pTm->tm_mday;
			if(nCurrTime > nPointTime)
			{
				if(MONTH_CARD_TO_POINT_CARD && nPoint > 0)
				{
					nFeeType	= c_typePoint;
				}
				else if(MONTH_CARD_TO_POINT_CARD && ENABLE_LOGIN_NO_POINT)
				{
					nFeeType	= c_typePoint;
					SafeCopy(szNotifyMsg, FLAG_NO_POINT, _MAX_NAMESIZE);
				}
				else
				{
					RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errNotTime, "�ʺ��ѹ���");
					return false;
				}
			}
			else
			{
				// ����Ƿ�ʣ7��
				if(szNotifyMsg[0] == 0)		// û��֪ͨ��Ϣʱ
				{
					Check7DaysNodify(nPointTime, szNotifyMsg);		// return szNotifyMsg
				}
			}
		}
		break;
	case c_typeFree:
		break;
	default:
		RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknown, "���ݿ����");
		LOGERROR("���[%s]�Ʒ����ʹ���[%d]������������", cMsg.m_pInfo->szAccount, nFeeType);
		return false;
	}

	// ��¼
	ASSERT(idAccount);

	//-------------------------------------
//	clock_t clkProcMsgAccBegin = clock();
	//--------------------------------------
	int nRet =0;
	if(strlen(cMsg.m_pInfo->szServer)<=0)///���δ��ѡ����ѷ������򷵻ط������������Ĵ�����Ϣ
		nRet=CPointThread::ERR_NO_SERVER;
	else
	{
		bool bYouke = false;
		char szAccountChek[64];
		strcpy(szAccountChek,cMsg.m_pInfo->szAccount);
		szAccountChek[7] = 0;
		if(strcmp(szAccountChek,"@youke_") == 0)
		bYouke = true;

		// ֪ͨ��Ϸ������
		nRet = g_pPointThread->NewLogin(idAccount, nAuthenID, szNotifyMsg, cMsg.m_pInfo->szServer,bYouke);
	}

	
	//--------------------------------------
//	clock_t clkProcMsgAccUsed = clock() - clkProcMsgAccBegin;
//	if(clkProcMsgAccUsed>20)//proc��ʱ
//	{
//		char szLog[512];
//		sprintf(szLog,"ProcMsgAcc��ʱ(%d):index:%d",
//			clkProcMsgAccUsed,nIndex);
//		::LogSave(szLog);
//	}
	//---------------------------------------
	switch(nRet)
	{
	case CPointThread::ERR_NO_SERVER:
		RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknowServer, "������δ����");
		return false;
	case CPointThread::ERR_BUSY:
		RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerBusy, "������æ���Ժ�");
		return false;
	case CPointThread::ERR_FULL:
		RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerFull, "������������");
		return false;
	case CPointThread::ERR_NONE:
		{
// 			::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
// 			//-------���---2007/05/12-----��¼��½����----begin------------
// 			if( !objDataInspectManage.WriteMananger(cMsg.m_pInfo->szServer,cMsg.m_pInfo->szAccount, idAccount,pClientIP,cMsg.m_pInfo->szMacClient) )
// 			{
// 				//��¼��½ʧ��---------
// 				//--------����¼------------------
// 			}
// 			::ReleaseMutex(g_xDatabase);	//------------------------------------------

			//-----------------------------end----------------------------------
// 			if(bRejoin)
// 			{
				// ���½�����Ϸ���޸�һЩ����
// 				g_pOnlineTable->Rejoin(idAccount, nAuthenID, pClientIP, 
// 					cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, 
// 					nFeeType, cMsg.m_pInfo->szServer, idFeeAccount, szFeeAccount);
// 				AllowLogin(nIndex, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
// 				LOGACCOUNT("ͬ�����[%s][%d]��[%d]�������µ�¼[%s]����֤ID[%08X]��IP[%s]���Ʒ��ʺ�[%s]��֪ͨ[%s]", 
// 					cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer, 
// 					nAuthenID, pClientIP, szFeeAccount, szNotifyMsg);
// 			}
// 			else
// 			{
				///����µ����߱�
				m_AccountManager.OnlineAccount(idAccount,nAuthenID,cMsg.m_pInfo->szAccount,cMsg.m_pInfo->szServer);

				// ������߱�
				g_pOnlineTable->AddNew(idAccount, nAuthenID, pClientIP, 
					cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, 
					nFeeType, cMsg.m_pInfo->szServer, idFeeAccount, szFeeAccount);			// return 0: error
				
				AllowLogin(nIndex, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
// 				LOGACCOUNT("ͬ�����[%s][%d]��[%d]���͵�¼[%s]����֤ID[%08X]��IP[%s]���Ʒ��ʺ�[%s]��֪ͨ[%s]", 
// 					cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer, 
// 					nAuthenID, pClientIP, szFeeAccount, szNotifyMsg);
// 			}
			///[2009.3.18--�Ŷ�]��ع����������

			///softworms ���δ˾�

			//g_pPointThread->AddTooLoginInfo(idAccount,cMsg.m_pInfo->szServer);
			
		}
		return true;
	default:
		ASSERT(!"NewLogin");
		return false;
	}
}

bool	CLoginThread::ProcessMsgBarAccount(int nIndex, char * pBuf, int nLen)
{
	CMsgBarAccount	cMsg;
	cMsg.Create(pBuf, nLen);
#ifdef	RC5ENCRYPT
	m_cRc5.Rc5Decrypt(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);
	m_cRc5NetBar.Rc5Decrypt(cMsg.m_pInfo->szBarPassword, _MAX_NAMESIZE);
//?	m_cRc5.Rc5Decrypt(cMsg.m_pInfo->szBarPassword, _MAX_NAMESIZE);
#endif
	FormatStr(cMsg.m_pInfo->szAccount, _MAX_ACCOUNT_NAMESIZE);		// ��β0��ɾβ��
	FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// ��β0��ɾβ��
	FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// ��β0��ɾβ��
	FormatStr(cMsg.m_pInfo->szBarAccount, _MAX_ACCOUNT_NAMESIZE);		// ��β0��ɾβ��
	FormatStr(cMsg.m_pInfo->szBarPassword, _MAX_NAMESIZE);		// ��β0��ɾβ��

	OBJID	idAccount		= ID_NONE;
	OBJID	idFeeAccount	= ID_NONE;
	char 	szPassword[_MAX_NAMESIZE]	= "";
	char *	pClientIP	= m_aServerSocket[nIndex].GetPeerIP();
	unsigned long nClientIP	= inet_addr(pClientIP);
	DWORD	nAuthenID	= NewAuthenID(nClientIP);
	char	szServerName[SERVERNAMESIZE]	= "";
	char	szBarAccount[_MAX_ACCOUNT_NAMESIZE]		= "";
	char	szNotifyMsg[_MAX_NAMESIZE]	= "";
	char	szOldIP[_MAX_NAMESIZE]			= "";
	char *	pErrMsg = "��¼ʧ��";

	if(IsFreeServer(cMsg.m_pInfo->szServer))
	{
		RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknowServer, "��ʹ����ͨ�ʺ�");
		return false;
	}

	// ��¼
	if(g_pOnlineTable->GetAttr(cMsg.m_pInfo->szAccount, idAccount, szPassword, nAuthenID, 
						szServerName, szBarAccount, idFeeAccount, szOldIP))		//??? ���ߡ��ú�����������ADDNEW�ķ�ԭ���Բ���
	{
		// ����¼������޿���
		if(szPassword[0] == 0)
		{
			if(AppendPassword(idAccount, cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword))
				SafeCopy(szPassword, cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);
		}

		// �����
		if(strcmp(cMsg.m_pInfo->szPassword, szPassword))		// �����
		{
			LOGACCOUNT("���[%s]�ٴε�¼������[%s]��IP��ַ[%s]", 
				cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, pClientIP);
			AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errPassword, "�ʺ���������3");
			return false;
		}

		// �ʺš�������ȷ
		// �����
		//? ���ز飬����ʱ���߱���Զ�ɾ��
		
		// ȡLICENCETYPE(���Ż�����ȡLICENCETYPE����ONLINE����)
		int		nLicenceType = c_typeNone;
		int		nPoint		= 0;	// ������
		GetFeeType(cMsg.m_pInfo->szBarAccount, cMsg.m_pInfo->szBarPassword, nLicenceType, nPoint);		// return nLicenceType

		// ������ͬʱ��¼����һ̨������������������ͼƷ��ʺ����Ƿ���ͬ
		if(nLicenceType == c_typeNone || strcmp(cMsg.m_pInfo->szServer, szServerName) != 0 
									|| strcmp(cMsg.m_pInfo->szBarAccount, szBarAccount) != 0
									|| strcmp(pClientIP, szOldIP) != 0  )	// �Ƚ��ϴε�¼�������ʺ��Ƿ���ͬ
		{
			if(!g_pOnlineTable->IsTimeWait(idAccount))
				g_pPointThread->Kickout(idAccount);		// ֪ͨ��Ϸ����������

			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errOnline, "���Ժ����µ�¼");
			return false;
		}

		// ��ʹ������֤ID					nAuthenID	= NewAuthenID(nClientIP);	
		
		// ֪ͨ��Ϸ������
		int nRet = g_pPointThread->NewLogin(idAccount, nAuthenID, szNotifyMsg, cMsg.m_pInfo->szServer);
		switch(nRet)
		{
		case CPointThread::ERR_NO_SERVER:
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknowServer, "������δ����");
			return false;
		case CPointThread::ERR_BUSY:
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerBusy, "������æ���Ժ�");
			return false;
		case CPointThread::ERR_FULL:
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerFull, "��������������");
			return false;
		case CPointThread::ERR_NONE:
			// ���½�����Ϸ���޸�һЩ����
			g_pOnlineTable->Rejoin(idAccount, nAuthenID, pClientIP, 
				cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, 
				nLicenceType, cMsg.m_pInfo->szServer, idFeeAccount, cMsg.m_pInfo->szBarAccount);
			AllowLogin(nIndex, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
// 			LOGACCOUNT("ͬ�����[%s][%d]�������ʺ�[%s][%d]�ٴε�¼[%s]����֤ID[%08X]��IP[%s]���Ʒ��ʺ�[%s]", 
// 							cMsg.m_pInfo->szAccount, idAccount, 
// 							cMsg.m_pInfo->szBarAccount, idFeeAccount, cMsg.m_pInfo->szServer, 
// 							nAuthenID, pClientIP, cMsg.m_pInfo->szBarAccount);
			return true;
		default:
			ASSERT(!"NewLogin");
			return false;
		}
	}
	else	// û����
	{
		// ���LICENCE�ʺ�//////////////////////////////////////////////////////////////////////
		ASSERT(idFeeAccount	== ID_NONE);
		int		nPoint			= 0;
		DWORD	nPointTime		= 0;
		int		nLicenceType	= c_typeNone;
		int		nLicence		= 0;
		char	szNetBarIP[IPSTRSIZE]	= "127.0.0.1";
		char	szIPMask[IPSTRSIZE]		= "255.255.255.255";
		if(!GetLicence(cMsg.m_pInfo->szBarAccount, cMsg.m_pInfo->szBarPassword, 
				idFeeAccount, nPoint, nPointTime, nLicenceType, nLicence, szNetBarIP, szIPMask))			// ����ֵ
		{
			LOGACCOUNT("���֤�ʺ�[%s]�����[%s]��IP��ַ[%s]", 
						cMsg.m_pInfo->szBarAccount, cMsg.m_pInfo->szBarPassword, pClientIP);
			AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "���֤�������");
			return false;
		}

		// ���TYPE���������
		switch(nLicenceType)
		{
		case c_typePoint:
		case c_typeTime:
		case c_typeISP:
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "���֤���ʹ���");
			return false;
		case c_typeNetBarPoint:
			if(nPoint <= 0)
			{
				RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errNotTime, "���֤Сʱ�ľ�");
				return false;
			}
			break;
		case c_typeNetBarTime:
			{
				// ������
				time_t	tCurr = time(NULL);
				tm *	pTm = localtime(&tCurr);
				DWORD	nCurrTime = (pTm->tm_year+1900)*10000 + (pTm->tm_mon+1)*100 + pTm->tm_mday;
				if(nCurrTime > nPointTime)
				{
					RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errNotTime, "���֤����");
					return false;
				}

				// �������
				int nCount = g_pOnlineTable->GetFeeCount(idFeeAccount);
				if(nCount >= nLicence)
				{
//					LOGACCOUNT("���֤[%s]��������[%d/%d]��IP��ַ[%s]", 
//								cMsg.m_pInfo->szBarAccount, nCount, nLicence, pClientIP);
					RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarOverflow, "���֤��������");
					return false;
				}

				// ����Ƿ�ʣ7��
				if(szNotifyMsg[0] == 0)		// û��֪ͨ��Ϣʱ
				{
					Check7DaysNodify(nPointTime, szNotifyMsg);		// return szNotifyMsg
				}
			}
			break;
		default:
			ASSERT(!"switch(nLicenceType)");
			return false;
		}

		// ���IP
		DWORD	dwNetBarIP	= inet_addr(szNetBarIP);
		DWORD	dwIPMask	= inet_addr(szIPMask);
		ASSERT(dwNetBarIP != INADDR_NONE);
//		ASSERT(dwIPMask != INADDR_NONE);
		if((nClientIP & dwIPMask) != (dwNetBarIP & dwIPMask))
		{
			LOGACCOUNT("���֤[%s]�ĵ�¼IP������Χ[%s][%s]��IP��ַ[%s]��", 
						cMsg.m_pInfo->szBarAccount, szNetBarIP, szIPMask, pClientIP);
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "���֤IP����");
			return false;
		}

		ASSERT(idFeeAccount != ID_NONE);

		// �����ҵ��ʺ�//////////////////////////////////////////////////////////////////////
		ASSERT(idAccount == ID_NONE);		// ����
		int		nAccountType	= c_typeNone;
		int		nBlock			= false;
		int		nUserPoint;					// ����
		DWORD	nUserPointTime;				// ����
		if(!GetAccount(cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, 
					idAccount, nAccountType, nUserPoint, nUserPointTime, nBlock))		// ����ֵ
		{
			LOGACCOUNT("���[%s]����[%s]��¼���ʺ���������, IP��ַ[%s]", 
				cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, pClientIP);
			AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errPassword, "�ʺ���������4");
			return false;
		}

		// ��ONLINE
		if(nBlock)
		{
//			LOGACCOUNT("���[%s]�����[%d]", cMsg.m_pInfo->szAccount, nBlock);
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBan, "���ʺű����");
			return false;
		}

		// ������
		switch(nAccountType)
		{
		case c_typePoint:
		case c_typeTime:
			break;
		case c_typeNetBarPoint:
		case c_typeNetBarTime:
		case c_typeISP:
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errBarPassword, "���ʺŲ��ܵ�¼");
			return false;
		default:
			LOGERROR("���[%s]�Ʒ����ʹ���[%d]������������", cMsg.m_pInfo->szAccount, nAccountType);
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknown, "���ݿ����");
			return false;
		}

		// ��¼
		ASSERT(idAccount);

		// ֪ͨ��Ϸ������
		int nRet = g_pPointThread->NewLogin(idAccount, nAuthenID, szNotifyMsg, cMsg.m_pInfo->szServer);
		switch(nRet)
		{
		case CPointThread::ERR_NO_SERVER:
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errUnknowServer, "������δ����");
			return false;
		case CPointThread::ERR_BUSY:
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerBusy, "������æ���Ժ�");
			return false;
		case CPointThread::ERR_FULL:
			RefuseLogin(nIndex, cMsg.m_pInfo->szAccount, c_errServerFull, "��������������");
			return false;
		case CPointThread::ERR_NONE:
			// ������߱�
			g_pOnlineTable->AddNew(idAccount, nAuthenID, pClientIP, 
						cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, 
						nLicenceType, cMsg.m_pInfo->szServer, idFeeAccount, cMsg.m_pInfo->szBarAccount);			// return 0: error
			AllowLogin(nIndex, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
// 			LOGACCOUNT("ͬ�����[%s][%d]�������ʺ�[%s][%d]��¼[%s]����֤ID[%08X]��IP[%s]���Ʒ��ʺ�[%s]", 
// 							cMsg.m_pInfo->szAccount, idAccount, 
// 							cMsg.m_pInfo->szBarAccount, idFeeAccount, cMsg.m_pInfo->szServer, 
// 							nAuthenID, pClientIP, cMsg.m_pInfo->szBarAccount);
			return true;
		default:
			ASSERT(!"NewLogin");
			return false;
		}
	}  // if(���ߡ�û����)
}




void CLoginThread::AllowLogin(int nIndex, OBJID idAccount, DWORD nAuthenID, LPCTSTR szServer)
{
	CMsgConnect cRetMsg;
// 	char	szServerIP[IPSTRSIZE] = "";
// 	char	szServerURL[URLSTRSIZE] = "";
// 	g_pPointThread->GetServerIP(szServerIP, szServer);
// 	g_pPointThread->GetServerURL(szServerURL,szServer);
// 
// 	int nMode = g_pPointThread->GetServerLoginMode(szServer);
// 
// 	if(nMode == 1)
// 		cRetMsg.Create(idAccount, nAuthenID, szServerIP);
// 	else if(nMode == 2)
// 		cRetMsg.Create(idAccount, nAuthenID, szServerURL);
// 	else
// 	{
// 		char szLog[512];
// 		sprintf(szLog,"����˵�ַģʽ����,szServer:%s",szServer);
// 		::LogSave(szLog);
// 	}
	g_pPointThread->MakeConnectMsg(cRetMsg,idAccount,nAuthenID,szServer);

	//m_aServerSocket[nIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());

	clock_t clkSendBegin= clock();
	BOOL bSendResult = m_aServerSocket[nIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
	clock_t clkSendUsed= clock() - clkSendBegin;
	if(!bSendResult)//send����
	{
		char szLog[512];
		sprintf(szLog,"��ͻ���Send()����:nIndex:%d,idAccount:%d,szServer:%s",
			nIndex,idAccount,szServer);
		::LogSave(szLog);
	}
	if(clkSendUsed > 20)//Send��ʱ
	{
		char szLog[512];
		sprintf(szLog,"��ͻ���Send()��ʱ(%d):nIndex:%d,idAccount:%d,szServer:%s",
			clkSendUsed,nIndex,idAccount,szServer);
		::LogSave(szLog);
	}



	if(!STAT_SERIAL)
		m_aServerSocket[nIndex].ShutDown();		//?? �رգ��öԷ��ȹر�(ע�⣺���û�յ�CMsgConnect��Ϣ�򲻻�ر�)
}

void CLoginThread::RefuseLogin(int nIndex, LPCTSTR szLoginName, int nType, LPCTSTR szText)
{
	try
	{
		CMsgConnect cRetMsg;
		cRetMsg.Create(ID_NONE, nType, (char*)szText);
		m_aServerSocket[nIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
		//m_aServerSocket[nIndex].ShutDown();		//?? �رգ��öԷ��ȹر�
		LOGACCOUNT("��¼���ɹ�[%s]��ԭ����[%s][%d]", szLoginName, szText, nType);
	}
	catch(...)
	{
		LogSave( "CLoginThread::RefuseLogin name:%s,nType:%d,ԭ��:%s",szLoginName,nType,szText );
	}
}

void CLoginThread::AddBan(DWORD nClientIP, LPCTSTR szClientIP, LPCTSTR szAccount)
{
	// ����BAN��
	bool	bFoundBan = false;
	int		nFreeSlot = -1;
	int		nBanCount = 0;
	// �Ҹ���λ��
	for(int i = 0; i < MAXBANIPS; i++)
	{
		if(m_pBanIPs[i].ClientIP() == 0)
		{
			if(nFreeSlot == -1)
				nFreeSlot = i;
		}
		else
		{
			nBanCount++;
		}

		if(m_pBanIPs[i].ClientIP() == nClientIP)	// �Ѽ�¼
		{
			bFoundBan = true;
			m_pBanIPs[i].IncError();				//? �������ߣ���������
			if(m_pBanIPs[i].IsBan())
			{
				LOGERROR("ĳ�ͻ����ѷǷ���¼[%d]�Σ��������˹����ʺŷ�������IP��ַ[%s]", BANERRORS, szClientIP);
				LOGACCOUNT("ĳ���[%s]��¼���ʺŷ�������������[%d]�Ρ�IP��ַ[%s]����ֹ[%d]����", 
							szAccount, BANERRORS, szClientIP, BANSECS);
				PrintText("IP��ַ��%s����������%d����, ��%d�����ڽ��޷���¼", 
							szClientIP, BANERRORS, BANSECS);
			}
		}
	}
	if(!bFoundBan)
	{
		// �����BAN
		if(nFreeSlot != -1)
		{
			m_pBanIPs[nFreeSlot].Create(nClientIP);
			if(nBanCount*100/MAXBANIPS > 78)		// ������
				LOGWARNING("���BAN��[%d/%d]��IPΪ[%s]", nBanCount+1, MAXBANIPS, szClientIP);
		}
		else
		{
			LOGERROR("BAN ��̫С���и�IP[%s]û���ȥ�����ʵ����� MAXBANIPS ����", szClientIP);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	CLoginThread::ProcessMsgConnect(int nIndex, char * pBuf, int nLen)
{
	CMsgConnect	cMsg;
	cMsg.Create(pBuf, nLen);

	g_pOnlineTable->SetSerial(cMsg.m_pInfo->idAccount, cMsg.m_pInfo->dwData);
	m_aServerSocket[nIndex].ShutDown();		//?? �رգ��öԷ��ȹر�

	return true;
}















