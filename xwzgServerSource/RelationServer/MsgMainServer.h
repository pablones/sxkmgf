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

	BOOL	Create(OBJID dwMainServer, DWORD dwData, bool bExtend);//第三个参数bExtend只是为了避免重载歧义，参数的值无意义
	
public:
	virtual BOOL			Create	(char* pbufMsg, DWORD dwMsgSize);	
	//	virtual void			Process		(void *pInfo);
	
	virtual unsigned short	GetStdSize	(void){return sizeof(MSG_Info);}
	
	//private:
	typedef struct {
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		
		DWORD			dwMainServer;//0非主游戏服务器，1主游戏服务器，2只同步数据，不任命服务器
		DWORD			dwData;
	}MSG_Info;
	
	MSG_Info* m_pInfo;

	typedef struct {
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		//游戏服务器过来的消息多两个字段，但是游戏服务器的又不好去掉，只好再定义一个结构了
		unsigned short	unMsgSize_A;
		unsigned short	unMsgType_A;
		
		DWORD			dwMainServer;//0非主游戏服务器，1主游戏服务器，2只同步数据，不任命服务器
		DWORD			dwData;
	}MSG_Info_A;
};

#endif // !defined(AFX_MSGMAINSERVER_H__B283B417_7513_48BA_A6E9_444FCA7A048D__INCLUDED_)
