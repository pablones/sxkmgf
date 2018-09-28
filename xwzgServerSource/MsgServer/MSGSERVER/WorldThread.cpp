// WorldThread.cpp: implementation of the CWorldThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MessagePort.h"
#include "MsgServer.h"
#include "share/I_Shell.h"
#include "WorldThread.h"
#include "protocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorldThread::CWorldThread(IMessagePort* pPort)
{
	ASSERT(pPort);
	m_pMsgPort		= pPort;
	m_pWorld		= NULL;
	m_tNextClock	= 0;

	m_hMutexThread	= NULL;
}

CWorldThread::~CWorldThread()
{
}

//////////////////////////////////////////////////////////////////////
// overload
//////////////////////////////////////////////////////////////////////
bool CWorldThread::CreateThread(bool bRun /*= true*/)
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

	m_pWorld	= IWorld::CreateNew();
	return m_pWorld->Create(m_pMsgPort);
}

//////////////////////////////////////////////////////////////////////
void CWorldThread::OnDestroy()
{
	if(m_pWorld)
		m_pWorld->Release();
	m_pWorld = NULL;

#ifdef	DEBUG_MULTITHREAD
	if(m_hMutexThread)
	{
		::CloseHandle(m_hMutexThread);
		m_hMutexThread = NULL;
	}
#endif
}

//////////////////////////////////////////////////////////////////////
void CWorldThread::OnInit()	
{ 
	m_tNextClock = clock() + CLOCKS_PER_SEC; 
	char	szText[1024];
	sprintf(szText, "#%u: World kernel thread running.", m_pMsgPort->GetID());
	m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
}

//////////////////////////////////////////////////////////////////////
bool CWorldThread::OnProcess()
{
#ifdef	DEBUG_MULTITHREAD
	if(::WaitForSingleObject(m_hMutexThread, INFINITE) == WAIT_ABANDONED)
		return false;
#endif
	char	buf[MAX_MESSAGESIZE];
	CMessageStatus	cStatus;
	clock_t	clkStart = clock();
	int nmsgsize = m_pMsgPort->GetMsgSize();
	nmsgsize/=4;
	int nprocessmsgcnt = 0;
	DEBUG_TRY	// VVVVVVVVVVVVVV
	/*if*/while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
	{
		if(!m_pWorld->ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
		{
			LOGERROR("m_pWorld->ProcessMsg内部消息出错");
//			return false;
		}
		if((++nprocessmsgcnt>=600) && (nprocessmsgcnt > nmsgsize))
		{
			//LOGERROR("WorldThread process msg 600 break!");
			break;
		}
	}
//	DEBUG_CATCH("CWorldThread::OnProcess()")	// AAAAAAAAAAA
	DEBUG_CATCH2("CWorldThread::OnProcess() cStatus.m_nPacket[%d]", cStatus.m_nPacket);

	//及时取SOCKET数据
	m_pWorld->ProcessSocketMsg();

	clock_t	tCurr = clock();
	if(tCurr >= m_tNextClock)
	{
		m_tNextClock	+= WORLDKERNEL_ONTIMER_MS;

		// 自动修正
		{
			int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
			if(abs(nOffset) > TIMER_OFFSET_LIMIT)
			{
				m_tNextClock = tCurr + WORLDKERNEL_ONTIMER_MS;
				LOGWARNING("CWorldThread::OnProcess() 时钟偏移%d秒，已自动修正。", nOffset);
			}
		}

		DEBUG_TRY
		m_pWorld->OnTimer(tCurr);
		DEBUG_CATCH("m_pWorld->OnTimer()")
	}

#ifdef	DEBUG_MULTITHREAD
	::ReleaseMutex(m_hMutexThread);
#endif
//	Sleep(0);

	
		// stat
		clock_t	 clkUsed	= clock() - clkStart;
		extern struct STAT_STRUCT	g_stat;
		InterlockedExchangeAdd(&g_stat.nAllWorldMS, clkUsed);
		InterlockedIncrement(&g_stat.nWorldTimes);
		if(g_stat.nMaxWorldMS < clkUsed)
			g_stat.nMaxWorldMS = clkUsed;
	
//	DEBUG_TRY	// VVVVVVVVVVVVVV
//	m_pMsgPort->WaitMsg(__min(THREADDELAY_MS, WORLDKERNEL_ONTIMER_MS));		// 响应“关闭线程消息”的频度
//	DEBUG_CATCH("CMapGroupThread::OnProcess(2)")	// AAAAAAAAAAA
	Sleep(5);
	return true;
}



