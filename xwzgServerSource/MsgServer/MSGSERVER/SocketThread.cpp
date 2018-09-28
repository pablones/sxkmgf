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
		LOGERROR("创建mutex句柄失败!");
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
	
	s_dwCnt += m_pMsgPort->GetMsgSize();//20070519修罗
	
	if(GetTickCount() - s_dwTick >= 10000)
	{
		s_dwTick = GetTickCount();
		// LogSave("CP  : %d", s_dwCnt/10);
		s_dwCnt = 0;
	}
	/////////////////////////////////////////////////////////////////////	
	// 内部消息处理
	DWORD dwWhileCount = 0;//20070621修罗:加一个计数器,增强日志
	int nprocessmsgcnt = 0;
	int nmsgsize = m_pMsgPort->GetMsgSize();
	nmsgsize/=4;		//取1/4消息进行处理
	try{
		while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
		{
			clock_t	clkDebugWhile = clock();
			if(!m_pServerSocket->ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
			{
				LOGERROR("CSocketThread内部消息出错");
			}
	//------------begin-------------------------
			{
				extern struct STAT_STRUCT	g_stat;
				clock_t clkDebugProcMsgTime = clock() - clkDebugWhile;
				InterlockedExchangeAdd(&g_stat.setDebug[1], clkDebugProcMsgTime);		// debug
				if(clkDebugProcMsgTime > 30)//超时记录
				{ 
					char szLog[512];
					
					if( SOCKET_SENDCLIENTMSG == cStatus.m_nPacket )
					{
						const SENDCLIENTMSG_PACKET0* pPack = (SENDCLIENTMSG_PACKET0*)buf;
						if( pPack )
						{
							
							sprintf(szLog,"[超时]client消息处理id:%d,len:%d,time:%d",pPack->idPacket,
							pPack->nSize,clkDebugProcMsgTime );
							LOGERROR(szLog);
						}
					}
// 					else if( SOCKET_SENDNPCMSG == cStatus.m_nPacket )
// 					{
// 						const SENDNPCMSG_PACKET0* pPack = (SENDNPCMSG_PACKET0*)buf;
// 						if( pPack )
// 						{
// 							sprintf(szLog,"[超时]npc消息处理idNpc:%d,idPacket:%d,nSize:%d,time:%d",
// 								pPack->idNpc,pPack->idPacket,pPack->nSize,clkDebugProcMsgTime );
// 							LOGERROR(szLog);
// 						}
// 					}
					else
					{
						sprintf(szLog,"[超时]other消息处理FromPort:%d,Packet:%d,time:%d",cStatus.m_nPortFrom,
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
	//------------end--07.2.8王玉波注释-------------------------
		}
	}
	catch(...)
	{
		LOGERROR("CSocketThread::OnProcess crash! nPacket:%d,nVarType:%d,nPortFrom:%d",cStatus.m_nPacket,cStatus.m_nVarType,cStatus.m_nPortFrom);
	}
// 	DEBUG_CATCH("CMapGroupThread::OnProcess(msg)")	// AAAAAAAAAAA
	//以上一句和以下一句没有任何关系,07.2.8王玉波添加
	{
		extern struct STAT_STRUCT	g_stat;
		InterlockedExchangeAdd(&g_stat.setDebug[2], clock() - clkDebug);		// debug

		//最大while次数
		if(g_stat.setDebug[12] < dwWhileCount)
			InterlockedExchange(&g_stat.setDebug[12], dwWhileCount);
	//	clkDebug = clock();		// debug
	}

	// 时钟处理
	clock_t	tCurr = clock();
	if(tCurr >= m_tNextClock)
	{
		m_tNextClock	+= SOCKETKERNEL_ONTIMER_MS;

		// 自动修正
		{
			int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
			if(abs(nOffset) > TIMER_OFFSET_LIMIT)
			{
				m_tNextClock = tCurr + SOCKETKERNEL_ONTIMER_MS;
				LOGWARNING("CSocketThread::OnProcess() 时钟偏移%d秒，已自动修正。", nOffset);
			}
		}

		DEBUG_TRY
		m_pServerSocket->OnTimer(tCurr);
		DEBUG_CATCH("m_pServerSocket->OnTimer()")
	}

	// SOCKET消息处理
	DEBUG_TRY	// VVVVVVVVVVVVVV
// 	if(m_tProcessSocket.ToNextTime(SOCKET_PROCESS_INTERVAL))
// 	{
// 		clock_t clkDebugProcSocket = clock();		// debug
// 		if(!m_pServerSocket->ProcessSocket())
// 		{
// 			LOGERROR("CSocketThread的SOCKET处理出错");
// 		}
// 		{
// 			extern struct STAT_STRUCT	g_stat;
// 			InterlockedExchangeAdd(&g_stat.setDebug[3], clock() - clkDebugProcSocket);		// debug
// 		}
// 	}

	//放到外面处理
	if(!m_pServerSocket->ProcessSocket())
	{
		LOGERROR("CSocketThread的SOCKET处理出错");
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

	// 延时
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
//	m_pMsgPort->WaitMsg(clkLeave);			//@ 应该探测MsgList的数量
//	DEBUG_CATCH("CMapGroupThread::OnProcess(wait msg)")	// AAAAAAAAAAA
	 //if(!(m_pMsgPort->GetSize()))
		Sleep(1);
	return true;
}




