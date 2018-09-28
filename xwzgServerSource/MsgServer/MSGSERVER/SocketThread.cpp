// SocketThread.cpp: implementation of the CSocketThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "share/I_Shell.h"
#include "SocketThread.h"
#include "MessagePort.h"
#include "MsgServer.h"
#include "protocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketThread::CSocketThread(IMessagePort* pPort)
{
	ASSERT(pPort);
	m_pMsgPort		= pPort;
	m_pServerSocket	= NULL;
	m_tNextClock	= 0;

	m_hMutexThread	= NULL;
}

CSocketThread::~CSocketThread()
{
}

//////////////////////////////////////////////////////////////////////
// overload
//////////////////////////////////////////////////////////////////////
bool CSocketThread::CreateThread(bool bRun /*= true*/,int SERVER_LISTEN_PORT,int SERVER_LISTEN_NPC_PORT)
{
#ifdef	DEBUG_MULTITHREAD
	m_hMutexThread    =::CreateMutex(NULL, false, "FW_DEBUG_MULTITHREAD");
	if(!m_hMutexThread)
	{
		LOGERROR("����mutex���ʧ��!");
		return false;
	}
#endif
	if(!CThreadBase::CreateThread(bRun))
		return false;

	m_pServerSocket	= ISocketKernel::CreateNew();
	return m_pServerSocket->Create(m_pMsgPort,SERVER_LISTEN_PORT,SERVER_LISTEN_NPC_PORT);
}

//////////////////////////////////////////////////////////////////////
void CSocketThread::OnDestroy()
{
	if(m_pServerSocket)
		m_pServerSocket->Release();
	m_pServerSocket = NULL;

#ifdef	DEBUG_MULTITHREAD
	if(m_hMutexThread)
	{
		::CloseHandle(m_hMutexThread);
		m_hMutexThread = NULL;
	}
#endif
}

//////////////////////////////////////////////////////////////////////
void CSocketThread::OnInit()	
{ 
	m_tNextClock = clock() + CLOCKS_PER_SEC; 
	char	szText[1024];
	sprintf(szText, "#%u: Socket kernel thread running.", m_pMsgPort->GetID());
	m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
}

//////////////////////////////////////////////////////////////////////
bool CSocketThread::OnProcess()
{
#ifdef	DEBUG_MULTITHREAD
	if(::WaitForSingleObject(m_hMutexThread, INFINITE) == WAIT_ABANDONED)
		return false;
#endif
	char	buf[MAX_MESSAGESIZE];
	CMessageStatus	cStatus;
	clock_t	clkStart = clock();

	clock_t	clkDebug = clock();		// debug
	/////////////////////////////////////////////////////////////////////
	static DWORD s_dwCnt=0,s_dwTick=GetTickCount();
	
	s_dwCnt += m_pMsgPort->GetMsgSize();//20070519����
	
	if(GetTickCount() - s_dwTick >= 10000)
	{
		s_dwTick = GetTickCount();
		// LogSave("CP  : %d", s_dwCnt/10);
		s_dwCnt = 0;
	}
	/////////////////////////////////////////////////////////////////////	
	// �ڲ���Ϣ����
	DWORD dwWhileCount = 0;//20070621����:��һ��������,��ǿ��־
	int nprocessmsgcnt = 0;
	int nmsgsize = m_pMsgPort->GetMsgSize();
	nmsgsize/=4;		//ȡ1/4��Ϣ���д���
	try{
		while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
		{
			clock_t	clkDebugWhile = clock();
			if(!m_pServerSocket->ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
			{
				LOGERROR("CSocketThread�ڲ���Ϣ����");
			}
	//------------begin-------------------------
			{
				extern struct STAT_STRUCT	g_stat;
				clock_t clkDebugProcMsgTime = clock() - clkDebugWhile;
				InterlockedExchangeAdd(&g_stat.setDebug[1], clkDebugProcMsgTime);		// debug
				if(clkDebugProcMsgTime > 30)//��ʱ��¼
				{ 
					char szLog[512];
					
					if( SOCKET_SENDCLIENTMSG == cStatus.m_nPacket )
					{
						const SENDCLIENTMSG_PACKET0* pPack = (SENDCLIENTMSG_PACKET0*)buf;
						if( pPack )
						{
							
							sprintf(szLog,"[��ʱ]client��Ϣ����id:%d,len:%d,time:%d",pPack->idPacket,
							pPack->nSize,clkDebugProcMsgTime );
							LOGERROR(szLog);
						}
					}
// 					else if( SOCKET_SENDNPCMSG == cStatus.m_nPacket )
// 					{
// 						const SENDNPCMSG_PACKET0* pPack = (SENDNPCMSG_PACKET0*)buf;
// 						if( pPack )
// 						{
// 							sprintf(szLog,"[��ʱ]npc��Ϣ����idNpc:%d,idPacket:%d,nSize:%d,time:%d",
// 								pPack->idNpc,pPack->idPacket,pPack->nSize,clkDebugProcMsgTime );
// 							LOGERROR(szLog);
// 						}
// 					}
					else
					{
						sprintf(szLog,"[��ʱ]other��Ϣ����FromPort:%d,Packet:%d,time:%d",cStatus.m_nPortFrom,
							cStatus.m_nPacket,clkDebugProcMsgTime );
					}
					

				}
				dwWhileCount++;
				if((++nprocessmsgcnt>=3500) && (nprocessmsgcnt > nmsgsize))
				{
					//LOGERROR("SocketThread process msg 1000 break!");
					break;
				}

			}		
	//------------end--07.2.8����ע��-------------------------
		}
	}
	catch(...)
	{
		LOGERROR("CSocketThread::OnProcess crash! nPacket:%d,nVarType:%d,nPortFrom:%d",cStatus.m_nPacket,cStatus.m_nVarType,cStatus.m_nPortFrom);
	}
// 	DEBUG_CATCH("CMapGroupThread::OnProcess(msg)")	// AAAAAAAAAAA
	//����һ�������һ��û���κι�ϵ,07.2.8�������
	{
		extern struct STAT_STRUCT	g_stat;
		InterlockedExchangeAdd(&g_stat.setDebug[2], clock() - clkDebug);		// debug

		//���while����
		if(g_stat.setDebug[12] < dwWhileCount)
			InterlockedExchange(&g_stat.setDebug[12], dwWhileCount);
	//	clkDebug = clock();		// debug
	}

	// ʱ�Ӵ���
	clock_t	tCurr = clock();
	if(tCurr >= m_tNextClock)
	{
		m_tNextClock	+= SOCKETKERNEL_ONTIMER_MS;

		// �Զ�����
		{
			int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
			if(abs(nOffset) > TIMER_OFFSET_LIMIT)
			{
				m_tNextClock = tCurr + SOCKETKERNEL_ONTIMER_MS;
				LOGWARNING("CSocketThread::OnProcess() ʱ��ƫ��%d�룬���Զ�������", nOffset);
			}
		}

		DEBUG_TRY
		m_pServerSocket->OnTimer(tCurr);
		DEBUG_CATCH("m_pServerSocket->OnTimer()")
	}

	// SOCKET��Ϣ����
	DEBUG_TRY	// VVVVVVVVVVVVVV
// 	if(m_tProcessSocket.ToNextTime(SOCKET_PROCESS_INTERVAL))
// 	{
// 		clock_t clkDebugProcSocket = clock();		// debug
// 		if(!m_pServerSocket->ProcessSocket())
// 		{
// 			LOGERROR("CSocketThread��SOCKET�������");
// 		}
// 		{
// 			extern struct STAT_STRUCT	g_stat;
// 			InterlockedExchangeAdd(&g_stat.setDebug[3], clock() - clkDebugProcSocket);		// debug
// 		}
// 	}

	//�ŵ����洦��
	if(!m_pServerSocket->ProcessSocket())
	{
		LOGERROR("CSocketThread��SOCKET�������");
	}

	DEBUG_CATCH("CMapGroupThread::OnProcess(socket)")	// AAAAAAAAAAA

	{
		extern struct STAT_STRUCT	g_stat;
		InterlockedExchangeAdd(&g_stat.setDebug[4], clock() - clkStart);		// debug
	}
#ifdef	DEBUG_MULTITHREAD
	::ReleaseMutex(m_hMutexThread);
#endif
//	Sleep(0);

	// ��ʱ
	clock_t	 clkLeave	= SOCKET_PROCESS_INTERVAL - (clock() - clkStart);
	if(clkLeave < 0 || clkLeave > SOCKET_PROCESS_INTERVAL)
		clkLeave = 0;

	{	// stat
		clock_t	 clkUsed	= clock() - clkStart;
		extern struct STAT_STRUCT	g_stat;
		InterlockedExchangeAdd(&g_stat.nAllSocketMS, clkUsed);
		InterlockedIncrement(&g_stat.nSocketTimes);
		if(g_stat.nMaxSocketMS < clkUsed)
		{
			g_stat.nMaxSocketMS = clkUsed;
		}
	}
//	DEBUG_TRY	// VVVVVVVVVVVVVV
//	m_pMsgPort->WaitMsg(clkLeave);			//@ Ӧ��̽��MsgList������
//	DEBUG_CATCH("CMapGroupThread::OnProcess(wait msg)")	// AAAAAAAAAAA
	 //if(!(m_pMsgPort->GetSize()))
		Sleep(1);
	return true;
}




