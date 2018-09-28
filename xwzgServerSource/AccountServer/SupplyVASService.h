// SupplyVASService.h: interface for the CSupplyVASService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPPLYVASSERVICE_H__C3816C0B_D974_4367_9538_D58E9AFACAD9__INCLUDED_)
#define AFX_SUPPLYVASSERVICE_H__C3816C0B_D974_4367_9538_D58E9AFACAD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "VASSocket.h"
#include "vector"
#include "AFXMT.H"
#include "VASStringPacket.h"
#include "VASDBManager.h"

using namespace std;

//struct Section//同步代理
//{
//	Section()
//	{
//		InitializeCriticalSection(&CriticalSection);//初始化线程同步对象
//		::EnterCriticalSection(&CriticalSection);//lock		
//	}
//	~Section()
//	{
//		LeaveCriticalSection(&CriticalSection);//unlock
//		DeleteCriticalSection(&CriticalSection);		
//	}
//	CRITICAL_SECTION CriticalSection;
//};

//struct VASSupply
//{
//	string strMsg;
//	string strServerName;
//	UINT    nServerPort;
//	VASSupply()
//	{
//		nServerPort=0;
//	}
//	VASSupply(const CString& msg,const CString& ServerName,UINT ServerPort)
//	{
//		strMsg=msg;
//		strServerName=ServerName;
//		nServerPort=ServerPort;
//	}
//};

struct MSG_VASSupply_INFO
{
	struct dummy
	{
		WORD	MSG_SIZE;///包长度,最大65535字节,一般固定在1K以内——2字节
		BYTE	MSG_TYPE;///包类型——1字节
		WORD	WebServerID;///网站标识符,保留,为0——2字节
		BYTE    Type;///充值类型,保留为0——1字节
		DWORD	Supply_VAS_Point;///充值点数——4字节
	};
	dummy   info;
	char	comboString[_MAX_NAMESIZE*2+2];///游戏服务器名称;帐号登录名
};

struct vas_ProcessContext
{
	string				strServerName;///主机IP
	UINT				nServerPort;///主机端口
	MSG_VASSupply_INFO	context;///内容
};

enum VASMSGTYPE
{
	vmtSupplyRequest,///充值请求
	vmtSupplyReply///充值受答
};

class CSupplyVASService  
{
public:
	CSupplyVASService();
	virtual ~CSupplyVASService();

	BOOL StartService(UINT nPort=3468,UINT nInitBufferSize=2048);///开始服务,开套接字,开处理线程
	BOOL StopService();///关套接字,结束处理线程

	///互斥操作请求
	BOOL Add(const vas_ProcessContext& vasMessage);///添加一个消息到缓冲区内
//	BOOL Get(CString& strMessage);///得到并处理一个消息,完成后将它从队列中删除
	BOOL Clear();///清空队列
	///互斥操作请求
	
	typedef Section SupplyVASLock;

///内部调用的函数
protected:
	static UINT ProcessFn(LPVOID pParam);///线程函数,处理队中的发送请求
	BOOL		ProcessMsg(vas_ProcessContext& objVAS);///处理一个消息请求
	BOOL		ProcessSupplyVAS(MSG_VASSupply_INFO& objContext);///处理充充值请求

	void		ProcessPacket(const ParamList& paramlist);///信号槽通知函数
private:
	CVASSocket			m_skVasSocket;///侦听套接字,用于接受网站发过来的充值或查询信息
//	CVASStringPacket	m_packet;///协议处理对象
	CVASDBManager		m_manager;///数据库管理器,用于对数据库进行充值和查询操作
	CWinThread*			m_pWorkThread;///处理线程,用于从缓冲队列中取得数据进行处理
	vector<vas_ProcessContext>	m_bufferlist;///处理队列,用于存储套接字收到的数据
	CEvent				m_etWorkStop;///线程运行事件,通过它来关闭线程
};

#endif // !defined(AFX_SUPPLYVASSERVICE_H__C3816C0B_D974_4367_9538_D58E9AFACAD9__INCLUDED_)
