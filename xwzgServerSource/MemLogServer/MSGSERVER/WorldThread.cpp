// WorldThread.cpp: implementation of the CWorldThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MessagePort.h"
#include "MsgServer.h"
#include "I_Shell.h"
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
}

CWorldThread::~CWorldThread()
{
}

//////////////////////////////////////////////////////////////////////
// overload
//////////////////////////////////////////////////////////////////////
bool CWorldThread::CreateThread(bool bRun /*= true*/)
{
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
	char	buf[MAX_MESSAGESIZE];
	CMessageStatus	cStatus;
	clock_t	clkStart = clock();
	extern struct STAT_STRUCT	g_stat;


	/////////////////////////////////////////////////////////////////////	
	// 内部消息处理
	int nMsgSize = m_pMsgPort->GetMsgSize();
	//状态信息
	{
		if(nMsgSize > g_stat.nMaxWorldSize)
			InterlockedExchange(&g_stat.nMaxWorldSize, nMsgSize);	
	}

	int nProcessMsgNeed = max(1000,nMsgSize/4);		//取1/4消息进行处理
	int nprocessmsgcnt = 0;

	DEBUG_TRY	// VVVVVVVVVVVVVV
	while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
	{
		clock_t	clkDebugWhile = clock();
		if(!m_pWorld->ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
		{
			LOGERROR("m_pWorld->ProcessMsg内部消息出错");
		}
		
		//状态信息
		{
			++nprocessmsgcnt;
			if(nprocessmsgcnt > g_stat.nMaxWorldProcessSize)
				InterlockedExchange(&g_stat.nMaxWorldProcessSize, nprocessmsgcnt);
		}
		
		
		if(nprocessmsgcnt >= nProcessMsgNeed)
			break;
	}
	DEBUG_CATCH2("CWorldThread::OnProcess() cStatus.m_nPacket[%d]", cStatus.m_nPacket);

	clock_t	tCurr = clock();
	if(tCurr >= m_tNextClock)
	{
		m_tNextClock	+= WORLDKERNEL_ONTIMER_MS;

		//如果重置说明处理慢
		int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
		if(abs(nOffset) > TIMER_OFFSET_LIMIT)
		{
			m_tNextClock = tCurr + WORLDKERNEL_ONTIMER_MS;
			LOGWARNING("CWorldThread::OnProcess() 时钟偏移%d秒，已自动修正。", nOffset);
		}

		DEBUG_TRY
		m_pWorld->OnTimer(tCurr);
		DEBUG_CATCH("m_pWorld->OnTimer()")
	}


	{	// stat
		clock_t	 clkUsed	= clock() - clkStart;
		extern struct STAT_STRUCT	g_stat;
		InterlockedExchangeAdd(&g_stat.nAllWorldMS, clkUsed);
		if(g_stat.nMaxWorldMS < clkUsed)
		{
			InterlockedExchange(&g_stat.nMaxWorldMS, clkUsed);
		}
	}

	//放弃CPU片
	Sleep(1);
	return true;
}



