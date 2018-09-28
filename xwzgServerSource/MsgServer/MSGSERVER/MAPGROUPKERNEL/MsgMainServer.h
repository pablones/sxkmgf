// MsgMainServer.h: interface for the CMsgMainServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMAINSERVER_H__56F93ADD_739D_4003_9624_F3DFC93849B0__INCLUDED_)
#define AFX_MSGMAINSERVER_H__56F93ADD_739D_4003_9624_F3DFC93849B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgMainServer : public CNetMsg
{
public:
	CMsgMainServer();
	virtual ~CMsgMainServer();

	BOOL	Create(OBJID dwMainServer, DWORD dwData, bool bExtend);//����������bExtendֻ��Ϊ�˱����������壬������ֵ������
	
public:
	virtual BOOL			Create	(char* pbufMsg, DWORD dwMsgSize);	
	//	virtual void			Process		(void *pInfo);
	
	virtual unsigned short	GetStdSize	(void){return sizeof(MSG_Info);}
	
	//private:
	typedef struct {
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		
		DWORD			dwMainServer;//0������Ϸ��������1����Ϸ��������2ֻͬ�����ݣ�������������
		DWORD			dwData;
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGMAINSERVER_H__56F93ADD_739D_4003_9624_F3DFC93849B0__INCLUDED_)
