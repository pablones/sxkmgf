// SupplyVASService.cpp: implementation of the CSupplyVASService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SupplyVASService.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSupplyVASService::CSupplyVASService():m_etWorkStop(false,true)///��m_etWorkStop��ʼ��Ϊ�ֹ����źŻ�
{
	m_pWorkThread=NULL;///���г�Ա�����������ʼ��
}

CSupplyVASService::~CSupplyVASService()
{
	StopService();///ֹͣ����
}

BOOL CSupplyVASService::StopService()///���׽���,���������߳�
{
	if(m_pWorkThread)///�߳��������о�ֹͣ����
	{
		m_skVasSocket.sltRecievePacket.Remove(this,&CSupplyVASService::ProcessPacket);///����ź�֪ͨ
		m_etWorkStop.SetEvent();///���̷߳��źţ����߳��˳�����ѭ��
		WaitForSingleObject(*m_pWorkThread,-1);///�ȴ������߳̽���
		m_skVasSocket.Close();///�ر��׽���
		Clear();///���������
		m_pWorkThread=NULL;///�߳��ÿ�
	}
	return true;
}

void CSupplyVASService::ProcessPacket(const ParamList& paramlist)///�źŲ۽��ܺ���
{
	vas_ProcessContext	tmpObject;
//	ZeroMemory(&tmpObject.context,sizeof(tmpObject.context));
	tmpObject.strServerName=CString(paramlist[0].bstrVal);///Զ�˷�����IP
	tmpObject.nServerPort=paramlist[1].iVal;///Զ�˷������˿�
	tmpObject.context=*(MSG_VASSupply_INFO*)paramlist[3].pvRecord;///�յ����׽�������
	memcpy(&tmpObject.context,paramlist[3].pvRecord,paramlist[2].iVal);///�õ����ߴ�
	if(tmpObject.context.info.MSG_SIZE==paramlist[2].iVal)///�ж�ʵ�ʳߴ��Ƿ���ȷ
	{
		Add(tmpObject);///��Ӱ�����������
	}
	else
		AfxOutputDebugString("�յ�һ���ߴ���Ч�İ�!\n");
}

BOOL CSupplyVASService::StartService(UINT nPort,UINT nInitBufferSize)///��ʼ����,���׽���,�������߳�
{
	BOOL bResult=0;
	StopService();///��ʼ����ǰ,���޴�ر�ǰһ�η���
	m_etWorkStop.ResetEvent();///�����һ�ε�ֹͣ�¼���־
//	m_skVasSocket.SetHandleObject(this);///���ô������
	bResult=m_skVasSocket.Create(nPort,SOCK_DGRAM);///���׽��֣�ʹ��UDPЭ��
	if (!bResult)///��VAS�׽���
	{
		AfxMessageBox("����VAS�׽���ʧ��!,�˿ڱ�ռ��?");
	}
	else
	{
		m_skVasSocket.sltRecievePacket.Connect(this,&CSupplyVASService::ProcessPacket);///�ҽ��źŲ�
		m_bufferlist.reserve(nInitBufferSize);///����buffer��ʼ��С
		m_pWorkThread=AfxBeginThread(ProcessFn,this);///����һ���߳�
	}
	return bResult;
}

BOOL CSupplyVASService::ProcessMsg(vas_ProcessContext& objVAS)///����һ����Ϣ����
{
	///����һ����Ϣ
	BOOL bResult=0;///���ؽ��
	switch(objVAS.context.info.MSG_TYPE)
	{
		case vmtSupplyRequest:///��ֵ����
			bResult=ProcessSupplyVAS(objVAS.context);///�����ֵ�����
			break;
	}
	if(bResult)///bResult��ʾ�ɹ�Ϊ���,ʧ��Ϊ-1,0Ϊ��Ч��
		bResult=m_skVasSocket.SendTo(&objVAS.context,objVAS.context.info.MSG_SIZE,objVAS.nServerPort,objVAS.strServerName.c_str());///���ͻ�Ӧ��
	else
		OutputDebugString("�յ�һ��������Ч���������!");
//	CString strPacket;
//	CString strTemp=strVAS.strMsg.c_str();
//	BOOL bResult=0;
//	if(m_packet.ParseString(strTemp))///�������һ���Ϸ��İ�
//	{
//		m_packet.GetFieldValue("cmd",strTemp);///ȡ����������
//		if(strTemp=="supplyvas")///��ֵ��Ϣ
//		{
//			if(ProcessSupplyVAS(m_packet,strPacket))
//				m_skVasSocket.SendTo(strPacket,strPacket.GetLength(),strVAS.nServerPort,strVAS.strServerName.c_str());
//		}
//	}
//	else
//	{
//		TRACE("�յ�һ���Ƿ�����ֵ������");
//	}
	return bResult;
}

UINT CSupplyVASService::ProcessFn(LPVOID pParam)///�̺߳���,������еķ�������
{
	CSupplyVASService* pVas=(CSupplyVASService*)pParam;
	while(1)
	{
		SupplyVASLock lock;///��ס������
		if(WaitForSingleObject(pVas->m_etWorkStop,0)==WAIT_OBJECT_0)
			break;///�����������߳�,�˳�
		for(UINT i=0;i<pVas->m_bufferlist.size();++i)
		{
			pVas->ProcessMsg(pVas->m_bufferlist[i]);///�������ɹ��򷢻�Ӧ
			pVas->m_bufferlist.erase(pVas->m_bufferlist.begin()+i);///ɾ���������е�����
		}
		Sleep(1000);///�߳���Ϣ,�ͷ�CPUռ����
	}
	return 0;
}

BOOL CSupplyVASService::Clear()///��ն���
{
	SupplyVASLock lock;///��ס������
	m_bufferlist.clear();
	return true;
}

BOOL CSupplyVASService::Add(const vas_ProcessContext& vasMessage)///���һ����Ϣ
{
	SupplyVASLock lock;///��ס������
	m_bufferlist.push_back(vasMessage);///�������
	return m_bufferlist.size();
}

BOOL CSupplyVASService::ProcessSupplyVAS(MSG_VASSupply_INFO& objContext)
{
	static i=0;
	CString strBuffer,strServerName,strAccount;///��Ϸ����������
//	ULONG	idAccount=0;///�ʺ�ID
	UINT	nPoint=0;
	BOOL	bResult=-1;///���
	UINT	nSupplyType=0,nWebSiteID=0;
//	idAccount=objContext.Account_ID;///ȡ�ó�ֵ�ʺ�ID
	AfxExtractSubString(strAccount,objContext.comboString,0,';');///�ʺ���
	AfxExtractSubString(strServerName,objContext.comboString,1,';');///��������
	nPoint=objContext.info.Supply_VAS_Point;///ȡ�ó�ֵ����
	nSupplyType=objContext.info.Type;///ȡ�ó�ֵ����,����
	nWebSiteID=objContext.info.WebServerID;///ȡ�ó�ֵ��վID,����
	BOOL idAccount=m_manager.GetAccountID(strAccount);///�õ��ʺ�ID
//	if(idAccount!=-1 && m_manager.InitMananger(idAccount,strServerName))
//		bResult=m_manager.ExecSupply(nPoint,idAccount,strServerName,nWebSiteID,nSupplyType);///����ʧ�ܷ���-1,�ɹ��������
	objContext.info.MSG_TYPE=vmtSupplyReply;///����Ӧ��
	objContext.info.Supply_VAS_Point=bResult;///�ɹ�Ϊ���,ʧ��Ϊ-1
	return bResult;
}