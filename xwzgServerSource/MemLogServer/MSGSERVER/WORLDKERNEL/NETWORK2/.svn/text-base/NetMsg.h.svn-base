// NetMsg.h: interface for the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NETMSG_H
#define _NETMSG_H

/////////////////////////////////////////////////////////////////////////////////////////////////
// 用于隐藏内部名空间
#define WORLDKERNEL_BEGIN	namespace world_kernel {
#define WORLDKERNEL_END		};

/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma	warning(disable:4786)
#include "basefunc.h"
#include "NetworkDef.h"
#include "../../share/define.h"

//////////////////////////////////////////////////////////////////////
WORLDKERNEL_BEGIN
class CNetMsg;
class ISocket
{
public:
	virtual bool SendServerMsg			(CNetMsg* pNetMsg)				PURE_VIRTUAL_FUNCTION_0
	virtual bool SendServerMsg			(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen)	PURE_VIRTUAL_FUNCTION_0
	virtual bool SendClientMsg			(SOCKET_ID idSocket,CNetMsg* pNetMsg)				PURE_VIRTUAL_FUNCTION_0
	virtual bool SendClientMsg			(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen)	PURE_VIRTUAL_FUNCTION_0
};


class CNetMsg
{
public:
	CNetMsg();
	virtual ~CNetMsg();

	void	Init(void);
	void	Reset(void)	{Init();}

	SOCKET_ID	GetSocketID()	{return m_idSocket;}
	OBJID	GetType	(void)		{return m_unMsgType;}
	int		GetSize	(void)		{return m_unMsgSize;}
	char*	GetBuf(void)	{return m_bufMsg;}
public:	
	virtual BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	virtual BOOL			IsValid		(void);
	virtual void			Process		(void *pInfo);
	virtual bool			SendMsg		(CNetMsg* pMsg);
public:
	static int	GetMaxSize	(void)		{return _MAX_MSGSIZE;}
	static CNetMsg*	CreateClientMsg	(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize);
	static CNetMsg*	CreateSocketMsg	(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idMsg,	const char* pbufMsg, DWORD dwMsgSize);
	static CNetMsg*	CreateMsg	(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize);

public:
	void	AppendInfo(SOCKET_ID idSocket)	// 仅由CUser,CUserManager中的SendMsg调用
			{ m_idSocket = idSocket;}

protected:
	OBJID		m_unMsgType;
	int			m_unMsgSize;
	char		m_bufMsg[_MAX_MSGSIZE];

	SOCKET_ID	m_idSocket;
};
WORLDKERNEL_END

#endif 
