// MsgWaitQueue.h: interface for the CMsgWaitQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWAITQUEUE_H__4193D1D5_BAD6_4429_B8AF_BD49C6829E62__INCLUDED_)
#define AFX_MSGWAITQUEUE_H__4193D1D5_BAD6_4429_B8AF_BD49C6829E62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

WORLDKERNEL_BEGIN
//////////////////////////////////////////////////////////////////////////
//  [2/12/2007 Halei]	处理玩家登陆排队消息
//////////////////////////////////////////////////////////////////////////
class CMsgWaitQueue : public CNetMsg
{
public:
	CMsgWaitQueue();
	virtual ~CMsgWaitQueue();
	BOOL	Create	( char* pMsgBuf, DWORD dwSize );
	BOOL	Create( OBJID idAccount,DWORD dwEndTime,int usAction );
	BOOL	Create(OBJID idAccount,int usAction,DWORD curpos,DWORD queuesize);
	void	Process	( void* pInfo );

	typedef struct
	{
// 		MSGHEAD_DEFINE
//		unsigned short	unMsgSize;
//		unsigned short	unMsgType;
		DWORD		usAction;	//是否完成排队	
		OBJID		idAccount;	//帐号，队列里的唯一标识	
		DWORD		EndWaitTime;   //结束排队的时间
		DWORD		idSocket;		//套接字
		DWORD		CurPos;			//玩家所在的位置
		DWORD		QueueSize;		//玩家总数
	}MSG_info;
	MSG_info* m_pInfo;

};
enum
{

	NETWORK_LOGIN	=	0,			//第一次登陆，请求登陆
	NETWORK_WAITSUSSECS,			//等待成功请求登陆
	NETWORK_WAITBREAK,				//被迫中断
	NETWORK_WAITINFO,				//等待信息
};
WORLDKERNEL_END

#endif // !defined(AFX_MSGWAITQUEUE_H__4193D1D5_BAD6_4429_B8AF_BD49C6829E62__INCLUDED_)
