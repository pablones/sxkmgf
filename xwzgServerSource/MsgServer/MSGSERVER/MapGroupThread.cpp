// MapGroupThread.cpp: implementation of the CMapGroupThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MessagePort.h"
#include "MsgServer.h"
#include "share/I_Shell.h"
#include "MapGroupThread.h"
#include "protocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapGroupThread::CMapGroupThread(IMessagePort* pPort)
{
	ASSERT(pPort);
	m_pMsgPort		= pPort;
	m_pMapGroup		= NULL;
	m_tNextClock	= 0;

	m_hMutexThread	= NULL;

	m_nCnt = 0;

	m_nMsgAdd = 0;//20071016修罗:
}

CMapGroupThread::~CMapGroupThread()
{
}

//////////////////////////////////////////////////////////////////////
// overload
//////////////////////////////////////////////////////////////////////
bool CMapGroupThread::CreateThread(bool bRun /*= true*/)
{
#ifdef	DEBUG_MULTITHREAD
	m_hMutexThread    =::CreateMutex(NULL, false, "FW_DEBUG_MULTITHREAD");
	if(!m_hMutexThread)
	{
		LOGERROR("Create mutex handle failed!");
		return false;
	}
#endif
	if(!CThreadBase::CreateThread(bRun))
		return false;

	m_pMapGroup	= IMapGroup::CreateNew();
	return m_pMapGroup->Create(m_pMsgPort);
}

//////////////////////////////////////////////////////////////////////
void CMapGroupThread::OnDestroy()
{ 
	if(m_pMapGroup)
		m_pMapGroup->Release();
	m_pMapGroup = NULL;

#ifdef	DEBUG_MULTITHREAD
	if(m_hMutexThread)
	{
		::CloseHandle(m_hMutexThread);
		m_hMutexThread = NULL;
	}
#endif
}

//////////////////////////////////////////////////////////////////////
void CMapGroupThread::OnInit()	
{ 
	m_tNextClock = clock() + CLOCKS_PER_SEC; 
	char	szText[1024];
	sprintf(szText, "#%u: map group kernel thread running。", m_pMsgPort->GetID());
	m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);

//	m_pMapGroup->SynchroData();
}

//////////////////////////////////////////////////////////////////////
bool CMapGroupThread::OnProcess()
{
		//20070122修罗:测试
		//-----------------------------------
		const int _AshulanTestMs = 200;
		//-----------------------------------
	#ifdef	DEBUG_MULTITHREAD
		if(::WaitForSingleObject(m_hMutexThread, INFINITE) == WAIT_ABANDONED)
			return false;
	#endif
		char	buf[MAX_MESSAGESIZE];
		CMessageStatus	cStatus = {0};
		clock_t	clkStart = clock();
		
		//20070122修罗:
		//-------------------------------
		m_nCnt++;
		if(m_tCnt.ToNextTime(10000))
		{
			// 		LogSave("MapGroupThread Cnt[%d]: %d",
			// 			m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST,
			// 			m_nCnt/10);
			m_nCnt = 0;
		}
		//------------------------------
		int nstep = 0;
		DEBUG_TRY	// VVVVVVVVVVVVVV
			//探测MSG数量
			int nProcessmsgcnt = 0;
		int nSize = m_pMsgPort->GetMsgSize();//当前待处理消息量
		int nmsgsize = nSize/4;	//取1/4的消息进行处理
		extern struct STAT_STRUCT	g_stat;
		//InterlockedExchangeAdd(&g_stat.setMapGroupMsgSizeOnTimer[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST],nSize);
		int realProcCount = 0;//实际处理个数
		//LogSave( "地图组线程%d此时队列节点%d",m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST,nSize );
		//20070518修罗:十个消息一组处理
		//		for(int ashulanTest=0;ashulanTest<1;ashulanTest++)
		//		{
		/*if*/while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
		{
			nstep = 1;
			if(!m_pMapGroup->ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
			{
				LOGERROR("m_pMapGroup->ProcessMsg内部消息出错");
				//			return false;
			}
			nstep = 0;
			realProcCount ++;
			if((++nProcessmsgcnt>=600) && (nProcessmsgcnt > nmsgsize))
			{
				//LOGERROR("MapGroup process 600 msg break!");
				break;
			}
		}
		//			else
		//			{
		//				break;
		//			}
		// }
		
		
		//20071016修罗:修正[待处理消息量]---------------
		if(g_stat.setMapGroupMsgSizeOnTimer[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST])
		{
			//有值时,只递加增量
			InterlockedExchangeAdd(&g_stat.setMapGroupMsgSizeOnTimer[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST],
				nSize - m_nMsgAdd);
		}
		else
		{
			//为空时,将size给它
			InterlockedExchangeAdd(&g_stat.setMapGroupMsgSizeOnTimer[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST],
				nSize);
		}
		
		InterlockedExchangeAdd(&g_stat.setMapGroupMsgSizeOnTimerProc[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST],realProcCount);
		m_nMsgAdd = nSize - realProcCount;//20071016修罗:此次处理剩余消息量
		//----------------------------------------------
		
		DEBUG_CATCH2("CMapGroupThread::OnProcess() %u", cStatus.m_nPacket)	// AAAAAAAAAAA
// 	}
// 	catch (...)
// 	{
// 		if(cStatus.m_nPacket==10038)
// 		{
// 			TRANSMSG_PACKET0*	pMsg = (TRANSMSG_PACKET0*)buf;
// 			//SOCKET_ID	idSocket= pMsg->idSocket;
// 			//OBJID		idNpc	= pMsg->idNpc;
// 			OBJID	idMsg	= pMsg->idPacket;
// 			//char*	pBuf	= pMsg->buf;
// 			//int		nMsgLen	= pMsg->nSize;
// 			//int		nTrans	= pMsg->nTrans;
// 			UINT		usAction;
// 			memcpy(&usAction,pMsg->buf,sizeof(UINT));
// 			char szlog[1024];
// 			sprintf(szlog,"CMapGroupThread::OnProcess()-nstep=%d,m_nPacket=%d,m_nPortFrom=%d,m_nError=%d,m_nVarType=%d,idMsg=%d,usAction=%d",nstep,cStatus.m_nPacket,cStatus.m_nPortFrom,cStatus.m_nError,cStatus.m_nVarType,idMsg,usAction);
// 			::LogSave(szlog);
// 		}
// 		else
// 		{
// 			char szlog[1024];
// 			sprintf(szlog,"CMapGroupThread::OnProcess()-nstep=%d,m_nPacket=%d,m_nPortFrom=%d,m_nError=%d,m_nVarType=%d",nstep,cStatus.m_nPacket,cStatus.m_nPortFrom,cStatus.m_nError,cStatus.m_nVarType);
// 			::LogSave(szlog);
// 		}
// 	}
	

	clock_t	tCurr = clock();
	clock_t clkTimerUsed = 0;
	if(tCurr >= m_tNextClock)
	{
		//20070122修罗:测试
		//--------------------------------
		//m_tNextClock	+= MAPGROUP_ONTIMER_MS;
		//ASSERT(MAPGROUP_ONTIMER_MS == 500);
		m_tNextClock	+= _AshulanTestMs;
		//---------------------------------

		// 自动修正
		{
			int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
			if(abs(nOffset) > TIMER_OFFSET_LIMIT)
			{
				//20070215修罗:日志强化
				int idx = m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST;//第几个地图组
//				LOGWARNING("CMapGroupThread::OnProcess() 时钟偏移%d秒[地图组%d]，已自动修正。tLastClock:%d tCurr:%d。[%d]", 
//					nOffset,idx,m_tNextClock, tCurr, clock());
				if(abs(nOffset) > 100)
				{
					LOGWARNING("CMapGroupThread::OnProcess() 时钟偏移>100秒[地图组%d]，tLastClock:%d tCurr:%d。[%d]", 
						idx,m_tNextClock, tCurr, clock());
				}
				//20070122修罗:测试
				//--------------------------------
				//m_tNextClock = tCurr + MAPGROUP_ONTIMER_MS;
				m_tNextClock = tCurr + _AshulanTestMs;
				//-------------------------------
			}
		}

		clock_t clkTimerStart = clock();
		DEBUG_TRY
		m_pMapGroup->OnTimer(tCurr);
		DEBUG_CATCH("m_pMapGroup->OnTimer()")
		clkTimerUsed = clock() - clkTimerStart;
	}

#ifdef	DEBUG_MULTITHREAD
	::ReleaseMutex(m_hMutexThread);
#endif
//	Sleep(0);
	{
		// stat
		clock_t	 clkUsed	= clock() - clkStart;
		extern struct STAT_STRUCT	g_stat;
		int idx = m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST;
		if(idx >= 0 && idx < STAT_MAX_MAPGROUP)
		{
			InterlockedExchangeAdd(&g_stat.setMapGroupTimerMS[idx], clkTimerUsed);
			InterlockedExchangeAdd(&g_stat.setAllMapGroupMS[idx], clkUsed);
			InterlockedIncrement(&g_stat.setMapGroupTimes[idx]);
			if(g_stat.setMaxMapGroupMS[idx] < clkUsed)
			{
				g_stat.setMaxMapGroupMS[idx] = clkUsed;
				if(cStatus.m_nPacket == KERNEL_CLIENTMSG/* || cStatus.m_nPacket == KERNEL_NPCMSG*/)
					g_stat.nPacketID[ idx ]	= *((long*)buf + 1);
				else
					g_stat.nPacketID[ idx ]	= -1*cStatus.m_nPacket;
			}
		}
	}
//	DEBUG_TRY	// VVVVVVVVVVVVVV
//		//20070122修罗:测试
//		//--------------------------------
//	//m_pMsgPort->WaitMsg(__min(THREADDELAY_MS, MAPGROUP_ONTIMER_MS));		// 响应“关闭线程消息”的频度
//	//20070516修罗:bug.加速误判
//	//改回原等待值THREADDELAY_MS--------------------
//	m_pMsgPort->WaitMsg(__min(THREADDELAY_MS, _AshulanTestMs));		// 响应“关闭线程消息”的频度
//	//------------------------------------
//	DEBUG_CATCH("CMapGroupThread::OnProcess(2)")	// AAAAAAAAAAA
	Sleep(5);
	return true;
}



