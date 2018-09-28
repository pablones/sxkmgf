// VASSocket.cpp : implementation file
//

#include "stdafx.h"
#include "accountserver.h"
#include "VASSocket.h"
#include "SupplyVASService.h"
#include "atlbase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVASSocket

CVASSocket::CVASSocket()
{
	///��Ա�����ʼ��
	m_pHandleObject=NULL;
	nByteRead=0;
	m_nPeerPort=0;
	ZeroMemory(&m_cbBuffer,sizeof(m_cbBuffer));
}

CVASSocket::~CVASSocket()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CVASSocket, CSocket)
	//{{AFX_MSG_MAP(CVASSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CVASSocket member functions

//BOOL CVASSocket::ProcessSupplyVAS(CVASStringPacket& packet,CString& strReplyMsg)
//{
//	static i=0;
//	CString strBuffer,strServerName;///��Ϸ����������
//	ULONG	idAccount=0;///�ʺ�ID
//	UINT	nPoint=0;
//	BOOL	bResult=-1;///���
//	UINT	nSupplyType=0,nWebSiteID=0;
//	idAccount=packet.GetFieldValue("account");///ȡ�ó�ֵ�ʺ�ID
//	packet.GetFieldValue("servername",strServerName);///ȡ����Ϸ��������
//	nPoint=packet.GetFieldValue("vaspoint");///ȡ�ó�ֵ����
//	nSupplyType=packet.GetFieldValue("tradetype");///ȡ�ó�ֵ����,����
//	nWebSiteID=packet.GetFieldValue("webid");///ȡ�ó�ֵ��վID,����
//	if(manager.InitMananger(idAccount,strServerName))
//	{
//		bResult=manager.ExecSupply(nPoint,idAccount,strServerName,nWebSiteID,nSupplyType);///����ʧ�ܷ���-1,�ɹ��������
//	}
//	packet.Clear();///����ϴν��
//	packet.AddField("id",i++);
//	packet.AddField("cmd","supplyvas_reply");
//	packet.AddField("account",idAccount);
//	packet.AddField("servername",strServerName);
//	packet.AddField("piont",bResult);
//	packet.PacketString(strReplyMsg);
//	return strReplyMsg.GetLength();
//}

void CVASSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
//	Section lock;
////	CString strBuffer;//,strServerName;///��Ϸ����������
//	vas_ProcessContext	tmpObject;
//	UINT nPacketSize=0;
//	ZeroMemory(&tmpObject.context,sizeof(tmpObject.context));
//	nByteRead=ReceiveFrom(m_cbBuffer,1024,m_strPeerName,m_nPeerPort);
//	tmpObject.nServerPort=m_nPeerPort;
//	tmpObject.strServerName=m_strPeerName;
//	memcpy(&nPacketSize,m_cbBuffer,2);
//	if(nPacketSize!=nByteRead)
//		AfxOutputDebugString("�յ�һ���ߴ���Ч�İ�!\n");
//	else
//	{
//		memcpy(&tmpObject.context,m_cbBuffer,nPacketSize);
//		if (m_pHandleObject)
//			m_pHandleObject->Add(tmpObject);
//	}
	nByteRead=ReceiveFrom(m_cbBuffer,1024,m_strPeerName,m_nPeerPort);///�հ�

	///�����в���ѹ�����������б�
	ParamList paramlist;
	CComVariant PeerName=m_strPeerName;
	paramlist.push_back(PeerName);
	paramlist.push_back(CComVariant((short)m_nPeerPort));

	paramlist.push_back(CComVariant(nByteRead));///����С
	paramlist.push_back(CComVariant((int)m_cbBuffer));//////������ָ��
	sltRecievePacket(paramlist);///���źŲ�֪ͨ�����¼�������
	CSocket::OnReceive(nErrorCode);
}
