// SupplyVASService.cpp: implementation of the CSupplyVASService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SupplyVASService.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSupplyVASService::CSupplyVASService():m_etWorkStop(false,true)///将m_etWorkStop初始化为手工的信号机
{
	m_pWorkThread=NULL;///所有成员变量都必须初始化
}

CSupplyVASService::~CSupplyVASService()
{
	StopService();///停止服务
}

BOOL CSupplyVASService::StopService()///关套接字,结束处理线程
{
	if(m_pWorkThread)///线程正在运行就停止服务
	{
		m_skVasSocket.sltRecievePacket.Remove(this,&CSupplyVASService::ProcessPacket);///解除信号通知
		m_etWorkStop.SetEvent();///给线程发信号，让线程退出生命循环
		WaitForSingleObject(*m_pWorkThread,-1);///等待处理线程结束
		m_skVasSocket.Close();///关闭套接字
		Clear();///清除缓冲区
		m_pWorkThread=NULL;///线程置空
	}
	return true;
}

void CSupplyVASService::ProcessPacket(const ParamList& paramlist)///信号槽接受函数
{
	vas_ProcessContext	tmpObject;
//	ZeroMemory(&tmpObject.context,sizeof(tmpObject.context));
	tmpObject.strServerName=CString(paramlist[0].bstrVal);///远端服务器IP
	tmpObject.nServerPort=paramlist[1].iVal;///远端服务器端口
	tmpObject.context=*(MSG_VASSupply_INFO*)paramlist[3].pvRecord;///收到的套接字数据
	memcpy(&tmpObject.context,paramlist[3].pvRecord,paramlist[2].iVal);///得到包尺寸
	if(tmpObject.context.info.MSG_SIZE==paramlist[2].iVal)///判断实际尺寸是否正确
	{
		Add(tmpObject);///添加包到缓冲区中
	}
	else
		AfxOutputDebugString("收到一个尺寸无效的包!\n");
}

BOOL CSupplyVASService::StartService(UINT nPort,UINT nInitBufferSize)///开始服务,开套接字,开处理线程
{
	BOOL bResult=0;
	StopService();///开始服务前,先无错关闭前一次服务
	m_etWorkStop.ResetEvent();///清除上一次的停止事件标志
//	m_skVasSocket.SetHandleObject(this);///设置处理对象
	bResult=m_skVasSocket.Create(nPort,SOCK_DGRAM);///打开套接字，使用UDP协议
	if (!bResult)///打开VAS套接字
	{
		AfxMessageBox("创建VAS套接字失败!,端口被占用?");
	}
	else
	{
		m_skVasSocket.sltRecievePacket.Connect(this,&CSupplyVASService::ProcessPacket);///挂接信号槽
		m_bufferlist.reserve(nInitBufferSize);///设置buffer初始大小
		m_pWorkThread=AfxBeginThread(ProcessFn,this);///生成一个线程
	}
	return bResult;
}

BOOL CSupplyVASService::ProcessMsg(vas_ProcessContext& objVAS)///处理一个消息请求
{
	///处理一个消息
	BOOL bResult=0;///返回结果
	switch(objVAS.context.info.MSG_TYPE)
	{
		case vmtSupplyRequest:///充值请求
			bResult=ProcessSupplyVAS(objVAS.context);///处理充值请求包
			break;
	}
	if(bResult)///bResult表示成功为余额,失败为-1,0为无效包
		bResult=m_skVasSocket.SendTo(&objVAS.context,objVAS.context.info.MSG_SIZE,objVAS.nServerPort,objVAS.strServerName.c_str());///发送回应包
	else
		OutputDebugString("收到一个类型无效的增请求包!");
//	CString strPacket;
//	CString strTemp=strVAS.strMsg.c_str();
//	BOOL bResult=0;
//	if(m_packet.ParseString(strTemp))///如果这是一个合法的包
//	{
//		m_packet.GetFieldValue("cmd",strTemp);///取得命令类型
//		if(strTemp=="supplyvas")///充值消息
//		{
//			if(ProcessSupplyVAS(m_packet,strPacket))
//				m_skVasSocket.SendTo(strPacket,strPacket.GetLength(),strVAS.nServerPort,strVAS.strServerName.c_str());
//		}
//	}
//	else
//	{
//		TRACE("收到一个非法的增值服务器");
//	}
	return bResult;
}

UINT CSupplyVASService::ProcessFn(LPVOID pParam)///线程函数,处理队中的发送请求
{
	CSupplyVASService* pVas=(CSupplyVASService*)pParam;
	while(1)
	{
		SupplyVASLock lock;///锁住缓冲区
		if(WaitForSingleObject(pVas->m_etWorkStop,0)==WAIT_OBJECT_0)
			break;///外界请求结束线程,退出
		for(UINT i=0;i<pVas->m_bufferlist.size();++i)
		{
			pVas->ProcessMsg(pVas->m_bufferlist[i]);///如果处理成功则发回应
			pVas->m_bufferlist.erase(pVas->m_bufferlist.begin()+i);///删除缓冲区中的数据
		}
		Sleep(1000);///线程休息,释放CPU占有率
	}
	return 0;
}

BOOL CSupplyVASService::Clear()///清空队列
{
	SupplyVASLock lock;///锁住缓冲区
	m_bufferlist.clear();
	return true;
}

BOOL CSupplyVASService::Add(const vas_ProcessContext& vasMessage)///添加一个消息
{
	SupplyVASLock lock;///锁住缓冲区
	m_bufferlist.push_back(vasMessage);///进入队列
	return m_bufferlist.size();
}

BOOL CSupplyVASService::ProcessSupplyVAS(MSG_VASSupply_INFO& objContext)
{
	static i=0;
	CString strBuffer,strServerName,strAccount;///游戏服务器名字
//	ULONG	idAccount=0;///帐号ID
	UINT	nPoint=0;
	BOOL	bResult=-1;///结果
	UINT	nSupplyType=0,nWebSiteID=0;
//	idAccount=objContext.Account_ID;///取得充值帐号ID
	AfxExtractSubString(strAccount,objContext.comboString,0,';');///帐号名
	AfxExtractSubString(strServerName,objContext.comboString,1,';');///服务器名
	nPoint=objContext.info.Supply_VAS_Point;///取得充值点数
	nSupplyType=objContext.info.Type;///取得充值类型,保留
	nWebSiteID=objContext.info.WebServerID;///取得充值网站ID,保留
	BOOL idAccount=m_manager.GetAccountID(strAccount);///得到帐号ID
//	if(idAccount!=-1 && m_manager.InitMananger(idAccount,strServerName))
//		bResult=m_manager.ExecSupply(nPoint,idAccount,strServerName,nWebSiteID,nSupplyType);///交易失败返回-1,成功返回余额
	objContext.info.MSG_TYPE=vmtSupplyReply;///发回应包
	objContext.info.Supply_VAS_Point=bResult;///成功为余额,失败为-1
	return bResult;
}