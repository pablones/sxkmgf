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
	///成员变更初始化
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
//	CString strBuffer,strServerName;///游戏服务器名字
//	ULONG	idAccount=0;///帐号ID
//	UINT	nPoint=0;
//	BOOL	bResult=-1;///结果
//	UINT	nSupplyType=0,nWebSiteID=0;
//	idAccount=packet.GetFieldValue("account");///取得充值帐号ID
//	packet.GetFieldValue("servername",strServerName);///取得游戏服务器名
//	nPoint=packet.GetFieldValue("vaspoint");///取得充值点数
//	nSupplyType=packet.GetFieldValue("tradetype");///取得充值类型,保留
//	nWebSiteID=packet.GetFieldValue("webid");///取得充值网站ID,保留
//	if(manager.InitMananger(idAccount,strServerName))
//	{
//		bResult=manager.ExecSupply(nPoint,idAccount,strServerName,nWebSiteID,nSupplyType);///交易失败返回-1,成功返回余额
//	}
//	packet.Clear();///清查上次结果
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
////	CString strBuffer;//,strServerName;///游戏服务器名字
//	vas_ProcessContext	tmpObject;
//	UINT nPacketSize=0;
//	ZeroMemory(&tmpObject.context,sizeof(tmpObject.context));
//	nByteRead=ReceiveFrom(m_cbBuffer,1024,m_strPeerName,m_nPeerPort);
//	tmpObject.nServerPort=m_nPeerPort;
//	tmpObject.strServerName=m_strPeerName;
//	memcpy(&nPacketSize,m_cbBuffer,2);
//	if(nPacketSize!=nByteRead)
//		AfxOutputDebugString("收到一个尺寸无效的包!\n");
//	else
//	{
//		memcpy(&tmpObject.context,m_cbBuffer,nPacketSize);
//		if (m_pHandleObject)
//			m_pHandleObject->Add(tmpObject);
//	}
	nByteRead=ReceiveFrom(m_cbBuffer,1024,m_strPeerName,m_nPeerPort);///收包

	///将所有参数压入万能列数列表
	ParamList paramlist;
	CComVariant PeerName=m_strPeerName;
	paramlist.push_back(PeerName);
	paramlist.push_back(CComVariant((short)m_nPeerPort));

	paramlist.push_back(CComVariant(nByteRead));///包大小
	paramlist.push_back(CComVariant((int)m_cbBuffer));//////包内容指针
	sltRecievePacket(paramlist);///让信号槽通知所有事件接收者
	CSocket::OnReceive(nErrorCode);
}
