// �ӿ��ࡣ�ӿڣ�������ϢͨѶ���Ƶ��̼߳�ͨѶ�ӿڡ�
// �ɽ��ޣ�2002.8.28

#include "../MessagePort.h"
#include "BaseFunc.h"
#include <stdlib.h>

using namespace message_port;


long	g_nMessagePortErrorCount;
///////////////////////////////////////////////////////////////////////////////////////
// interface
///////////////////////////////////////////////////////////////////////////////////////
// ��ʼ�������ýӿ�ID�ţ���ʼ������Ϣ�����ظ�����(PORT_ID���ܸı�)��
bool CMessagePort::Open	()
{
	m_nState = STATE_OK;
	for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
		m_setRecycle.push_back(*iter);
	m_setMsg.clear();
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// ������Ϣ��ָ���ӿڡ�������ϢID���������͡����ݡ�
bool CMessagePort::Send	(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf)
{
	//LOCKOBJ;		//VVVVV			//��ӵ�---
	//-----------06.10.28�������----begin---------------------------------
	if( !buf )
	{
		::LogSave( "CMessagePort::Send buf's len is zero.%d send to %d,nPacket=%d",m_id,nPort,nPacket );
//		return false;
	}
	//---------------------------------end-----------------------------------
	ASSERT(buf);

	if(!IsOpen())
		return false;

	if(nPort == INVALID_PORT)
		return true;

	if(nPort < 0 || nPort >= m_setPort.size())
	{
		ASSERT(!"nPort");
		return false;
	}

	//UNLOCKOBJ;		//AAAAA ������ⲿ�ӿڣ��ͷŻ��⡣�����ͷţ�����������

	return m_setPort[nPort]->PushMsg(m_id, nPacket, nVarType, buf);
}

///////////////////////////////////////////////////////////////////////////////////////
// ����ָ���ӿ�(�����нӿ�)��������Ϣ����ָ����ϢID��Ҳ�ɲ�ָ����
bool CMessagePort::Recv	(int nPort, int nPacket, VAR_TYPE nVarType, void* buf, CMessageStatus* pStatus /*= NULL*/)	// return false: û����
{
//	LOCKOBJ;		//���

	ASSERT(buf);

	if(m_nState == STATE_CLOSED)
	{
		if(pStatus)
			pStatus->m_nError	= STATUS_FLAG_CLOSE;
		return false;
	}

	int	nRcvPort = nPort, nRcvPacket = nPacket;
	VAR_TYPE	nRcvVarType;
	if(PopMsg(&nRcvPort, &nRcvPacket, &nRcvVarType, buf))		// �ڲ����������ô򿪻�����
	{
		// �������
		if(SIZE_OF_TYPE(nRcvVarType) > SIZE_OF_TYPE(nVarType))
		{
			ASSERT(!"VarType");
			if(pStatus)
				pStatus->m_nError		= STATUS_FLAG_ERROR;			//? �Ժ��֧���Զ�ת������
			return false;
		}

		if(pStatus)
		{
			pStatus->m_nPortFrom	= nRcvPort;
			pStatus->m_nPacket		= nRcvPacket;
			pStatus->m_nVarType		= nRcvVarType;
			pStatus->m_nError		= STATUS_FLAG_OK;
		}
		return true;
	}

	if(pStatus)
		pStatus->m_nError	= STATUS_FLAG_OK;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
// ����ָ���ӿ�(�����нӿ�)��������Ϣ����ָ����ϢID��Ҳ�ɲ�ָ����return false: ��ʱ�����
bool CMessagePort::WaitMsg(int nMilliSec)
{
	return TRUE;
//	LOCKOBJ;		//���

//	if(m_nState == STATE_CLOSED)
//		return false;
//	LOCKOBJ;		//���
//	// ԭ�Ӳ���
//	if(GetMsgSize()/*m_setMsg.size()*/)
//		return true;
//	::ResetEvent(m_hHaveMsg);		// ��ΪPushMsg()��SetEvent����LOCK״̬���õģ�size()��Event��ԭ���ԣ��޹����ͻ��
//
//	UNLOCKOBJ;		//AAAAA ��ȴ������̵߳�SEND()���ã��ͷŻ��⡣�����ͷţ�����������
//
//	DWORD	ret = ::WaitForSingleObject(m_hHaveMsg, nMilliSec);
//	if(ret == WAIT_OBJECT_0)
//	{
//		return true;
//	}
//	if(ret == WAIT_ABANDONED)
//	{
//		LOCKOBJ;		//? �ڲ����ã�����
//		Close();		// ���غ��������ô򿪻�����
//		return false;
//	}
//	else // if(ret == WAIT_TIMEOUT)
//	{
//		return false;
//	}
}

///////////////////////////////////////////////////////////////////////////////////////
// �رսӿڣ����ٽ�����Ϣ�����ظ����á�
bool CMessagePort::Close()
{
	//LOCKOBJ;

	m_nState = STATE_CLOSED;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// CMessagePort
///////////////////////////////////////////////////////////////////////////////////////
int CMessagePort::SIZE_OF_TYPE		(int type)
{
	const int	fixlen_type_size = 10;
	const int	size_of[fixlen_type_size] = {1,1,2,2, 4,4,4,4, 4,8};
	if(type < VARTYPE_NONE)
		return type;
	else if(type == VARTYPE_NONE)
	{
		ASSERT(!"SIZE_OF_TYPE");
		return 0;
	}
	else if(type - (VARTYPE_NONE+1) < fixlen_type_size)
		return size_of[type - (VARTYPE_NONE+1)];

	ASSERT(!"SIZE_OF_TYPE.");
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::PushMsg(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf)	// nData�еĴ��ͽṹ���ᱻCOPY
{
	ASSERT(buf);

	if(m_nState != STATE_OK)		// Close()�󣬲�������Ϣ
		return false;

	if(nPort < 0 || nPort >= m_setPort.size())
	{
		::LogSave("PushMsg(nPort) ERROR! nPort = %u,size = %u",nPort,m_setPort.size());
		//ASSERT(!"PushMsg(nPort)");
		return false;
	}

	int	nSize = SIZE_OF_TYPE(nVarType);
	if(nSize == 0 || nSize > MAX_MSGPACKSIZE)
	{
		ASSERT(!"PushMsg(nSize)");
		return false;
	}

	//ashulanTest---------
	clock_t clkDebug = clock();
	//--------------------

	//LOCKOBJ;		//���

	//--------------------
	clock_t clkUsed = clock() - clkDebug;
	if(clkUsed > 30)
	{
		LogSave("CMessagePort::PushMsg overTime(%d)",
			clkUsed);
	}
	//--------------------

	EnterCriticalSection(&m_cs);
	
	CMessagePacket*	pMsg = NULL;

	if(!m_setRecycle.empty())
	{
		pMsg = m_setRecycle.front();
		m_setRecycle.pop_front();
	}
	else
		pMsg = new CMessagePacket;		// VVVVVVVVVVVVVVVVVVV
	if(!pMsg)
	{
		ASSERT(!"new");
		LeaveCriticalSection(&m_cs);
		return false;
	}
	pMsg->m_nPortFrom		= nPort;
	pMsg->m_nPacket			= nPacket;
	pMsg->m_nVarType		= nVarType;
	memcpy(pMsg->m_bufData, buf, nSize);

	// ԭ�Ӳ���
	m_setMsg.push_back(pMsg);		// AAAAAAAAAAAAAAAAAAAAAAAA
	//++m_nMsgAmount;
	//::SetEvent(m_hHaveMsg);					// ����Ϣ��
	LeaveCriticalSection(&m_cs);
	// ������
	const int _warningMessagePacket=50000;//20070303����:�澯������
	int nSetSize = GetMsgSize(); /*m_setMsg.size();*/
	if(nSetSize >= _warningMessagePacket && (nSetSize%1000) == 0 && nSetSize != m_debugPackets)		//? 100����
	{
		m_debugPackets = nSetSize;
		LOGERROR("CMessagePacket[%d]�ڵ�������ﵽ[%u]��[%d][%d][%s]", m_id, nSetSize, nPort, nPacket, (LPCTSTR)DumpBuffer((char*)buf, __min(16,nSize)));
		InterlockedExchange(&g_nMessagePortErrorCount, nSetSize);
	}

	// ���ȡ��ȫ�ֱ���
	if (::RandGet(300) == 123)
		InterlockedExchange(&g_nMessagePortErrorCount, nSetSize);

	const int _maxMessagePacket=400000;//20070302����.ԭ10W...����
	if(GetMsgSize()/*m_setMsg.size()*/ > _maxMessagePacket)
	{
		LOGERROR("ERROR: ��Ϣ�������MessagePort[%d]�ر�!", m_id);
		Close();
	}

	return /*m_setMsg.size()*/ GetMsgSize() <= _maxMessagePacket;		//? 1000����
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::PopMsg(int *pPort, int *pPacket, VAR_TYPE *pVarType, void* buf)
{

	ASSERT(pPort && pPacket && pVarType && buf);

	if(m_nState != STATE_OK)		// Close()�󣬲�������Ϣ
		return false;
	
	if(GetMsgSize()==0)
		return FALSE;

	EnterCriticalSection(&m_cs);
	
	CMessagePacket* pMsg = m_setMsg.front();
	m_setMsg.pop_front();
	
	*pPort		= pMsg->m_nPortFrom;
	*pPacket	= pMsg->m_nPacket;
	*pVarType	= (VAR_TYPE)pMsg->m_nVarType;

	int	nSize = SIZE_OF_TYPE(*pVarType);
	if(nSize)
	{
		memcpy(buf, pMsg->m_bufData, nSize);
	}

	m_setRecycle.push_back(pMsg);

	LeaveCriticalSection(&m_cs);

	return TRUE;


//	LOCKOBJ;		//���
//	if(GetMsgSize() /*m_setMsg.size()*/ == 0)
//		return false;
//
//	for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
//	{
//		CMessagePacket* pMsg = *iter;
//		if((*pPort == PORT_ANY || *pPort == pMsg->m_nPortFrom) && (*pPacket == PACKET_ANY || *pPacket == pMsg->m_nPacket))
//		{
//			*pPort		= pMsg->m_nPortFrom;
//			*pPacket	= pMsg->m_nPacket;
//			*pVarType	= (VAR_TYPE)pMsg->m_nVarType;
//
//			int	nSize = SIZE_OF_TYPE(*pVarType);
//			if(nSize)
//			{
//				memcpy(buf, pMsg->m_bufData, nSize);
//			}
//
//			m_setRecycle.push(pMsg);
//			m_setMsg.erase(iter);
//			--m_nMsgAmount;
//			return true;
//		}
//	}
//
//	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::Init()
{
//	if((m_hHaveMsg	= ::CreateEvent(NULL, false, false, NULL)) == NULL)
//	{
//		LOGERROR("�޷�����CMessagePort���!");
//		return false;
//	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void CMessagePort::Clear()
{
	MSG_SET::iterator iter;
	for( iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
		delete(*iter);
	m_setMsg.clear();
	// m_nMsgAmount = 0;

	for(iter = m_setRecycle.begin();iter!=m_setRecycle.end();++iter)
		delete(*iter);

	m_setRecycle.clear();

//	while(m_setRecycle.size())
//		delete(m_setRecycle.pop());

//	if(m_hHaveMsg != NULL)
//		::CloseHandle(m_hHaveMsg);
//	m_hHaveMsg = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// static
///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::InitPortSet(int nPortNum)
{
	ASSERT(nPortNum >= 1);

	for(int i = 0; i < nPortNum; i++)
	{
		CMessagePort*	pPort = new CMessagePort(i);		// VVVVVVVVVVVVVVVV
		if(!pPort)
			return false;
		m_setPort.push_back(pPort);			// AAAAAAAAAAAAAAAAAAAAAAAAAA
		if(!pPort->Init())
			return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void CMessagePort::ClearPortSet()
{
	for(int i = 0; i < m_setPort.size(); i++)
	{
		//m_setPort[i]->Clear();
		delete(m_setPort[i]);
	}

	m_setPort.clear();
}

///////////////////////////////////////////////////////////////////////////////////////
CMessagePort::PORT_SET CMessagePort::m_setPort;
//CRITSECT CMessagePort::m_xSetCtrl;			// ����m_setPort����












