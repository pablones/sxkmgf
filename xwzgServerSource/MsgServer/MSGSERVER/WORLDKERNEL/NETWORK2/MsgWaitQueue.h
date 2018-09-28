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
//  [2/12/2007 Halei]	������ҵ�½�Ŷ���Ϣ
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
		DWORD		usAction;	//�Ƿ�����Ŷ�	
		OBJID		idAccount;	//�ʺţ��������Ψһ��ʶ	
		DWORD		EndWaitTime;   //�����Ŷӵ�ʱ��
		DWORD		idSocket;		//�׽���
		DWORD		CurPos;			//������ڵ�λ��
		DWORD		QueueSize;		//�������
	}MSG_info;
	MSG_info* m_pInfo;

};
enum
{

	NETWORK_LOGIN	=	0,			//��һ�ε�½�������½
	NETWORK_WAITSUSSECS,			//�ȴ��ɹ������½
	NETWORK_WAITBREAK,				//�����ж�
	NETWORK_WAITINFO,				//�ȴ���Ϣ
};
WORLDKERNEL_END

#endif // !defined(AFX_MSGWAITQUEUE_H__4193D1D5_BAD6_4429_B8AF_BD49C6829E62__INCLUDED_)
