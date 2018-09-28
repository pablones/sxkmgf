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

	m_nMsgAdd = 0;//20071016����:
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
	sprintf(szText, "#%u: map group kernel thread running��", m_pMsgPort->GetID());
	m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);

//	m_pMapGroup->SynchroData();
}

//////////////////////////////////////////////////////////////////////
bool CMapGroupThread::OnProcess()
{
		//20070122����:����
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
		
		//20070122����:
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
			//̽��MSG����
			int nProcessmsgcnt = 0;
		int nSize = m_pMsgPort->GetMsgSize();//��ǰ��������Ϣ��
		int nmsgsize = nSize/4;	//ȡ1/4����Ϣ���д���
		extern struct STAT_STRUCT	g_stat;
		//InterlockedExchangeAdd(&g_stat.setMapGroupMsgSizeOnTimer[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST],nSize);
		int realProcCount = 0;//ʵ�ʴ������
		//LogSave( "��ͼ���߳�%d��ʱ���нڵ�%d",m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST,nSize );
		//20070518����:ʮ����Ϣһ�鴦��
		//		for(int ashulanTest=0;ashulanTest<1;ashulanTest++)
		//		{
		/*if*/while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
		{
			nstep = 1;
			if(!m_pMapGroup->ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
			{
				LOGERROR("m_pMapGroup->ProcessMsg�ڲ���Ϣ����");
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
		
		
		//20071016����:����[��������Ϣ��]---------------
		if(g_stat.setMapGroupMsgSizeOnTimer[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST])
		{
			//��ֵʱ,ֻ�ݼ�����
			InterlockedExchangeAdd(&g_stat.setMapGroupMsgSizeOnTimer[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST],
				nSize - m_nMsgAdd);
		}
		else
		{
			//Ϊ��ʱ,��size����
			InterlockedExchangeAdd(&g_stat.setMapGroupMsgSizeOnTimer[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST],
				nSize);
		}
		
		InterlockedExchangeAdd(&g_stat.setMapGroupMsgSizeOnTimerProc[m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST],realProcCount);
		m_nMsgAdd = nSize - realProcCount;//20071016����:�˴δ���ʣ����Ϣ��
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
		//20070122����:����
		//--------------------------------
		//m_tNextClock	+= MAPGROUP_ONTIMER_MS;
		//ASSERT(MAPGROUP_ONTIMER_MS == 500);
		m_tNextClock	+= _AshulanTestMs;
		//---------------------------------

		// �Զ�����
		{
			int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
			if(abs(nOffset) > TIMER_OFFSET_LIMIT)
			{
				//20070215����:��־ǿ��
				int idx = m_pMsgPort->GetID() - MSGPORT_MAPGROUP_FIRST;//�ڼ�����ͼ��
//				LOGWARNING("CMapGroupThread::OnProcess() ʱ��ƫ��%d��[��ͼ��%d]�����Զ�������tLastClock:%d tCurr:%d��[%d]", 
//					nOffset,idx,m_tNextClock, tCurr, clock());
				if(abs(nOffset) > 100)
				{
					LOGWARNING("CMapGroupThread::OnProcess() ʱ��ƫ��>100��[��ͼ��%d]��tLastClock:%d tCurr:%d��[%d]", 
						idx,m_tNextClock, tCurr, clock());
				}
				//20070122����:����
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
//		//20070122����:����
//		//--------------------------------
//	//m_pMsgPort->WaitMsg(__min(THREADDELAY_MS, MAPGROUP_ONTIMER_MS));		// ��Ӧ���ر��߳���Ϣ����Ƶ��
//	//20070516����:bug.��������
//	//�Ļ�ԭ�ȴ�ֵTHREADDELAY_MS--------------------
//	m_pMsgPort->WaitMsg(__min(THREADDELAY_MS, _AshulanTestMs));		// ��Ӧ���ر��߳���Ϣ����Ƶ��
//	//------------------------------------
//	DEBUG_CATCH("CMapGroupThread::OnProcess(2)")	// AAAAAAAAAAA
	Sleep(5);
	return true;
}



