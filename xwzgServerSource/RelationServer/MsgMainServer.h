// MsgMainServer.h: interface for the CMsgMainServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMAINSERVER_H__B283B417_7513_48BA_A6E9_444FCA7A048D__INCLUDED_)
#define AFX_MSGMAINSERVER_H__B283B417_7513_48BA_A6E9_444FCA7A048D__INCLUDED_

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

	typedef struct {
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		//��Ϸ��������������Ϣ�������ֶΣ�������Ϸ���������ֲ���ȥ����ֻ���ٶ���һ���ṹ��
		unsigned short	unMsgSize_A;
		unsigned short	unMsgType_A;
		
		DWORD			dwMainServer;//0������Ϸ��������1����Ϸ��������2ֻͬ�����ݣ�������������
		DWORD			dwData;
	}MSG_Info_A;
};

#endif // !defined(AFX_MSGMAINSERVER_H__B283B417_7513_48BA_A6E9_444FCA7A048D__INCLUDED_)
