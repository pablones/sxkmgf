// ServerSocket.h: interface for the CSocketKernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSOCKET_H__44A9ACE7_FE12_4CDC_9492_F39CD1423D77__INCLUDED_)
#define AFX_SERVERSOCKET_H__44A9ACE7_FE12_4CDC_9492_F39CD1423D77__INCLUDED_

#pragma warning(disable:4786)
#include <time.h>
#include "../share/define.h"

//zeng 修改为IOCP



#include <vector>
#include <list>
#include <map>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Session.h"
//#include "IOCPServer.h"

class Session;
class IOCPServer;

class CSocketKernel :public ISocketKernel
{
	//friend class IOCPS;
	//friend class CWorkUnit;
public:
	CSocketKernel() {}		// 建议留空
	virtual ~CSocketKernel() {}		// 建议留空

public:
	ISocketKernel*	GetInterface() { return (ISocketKernel*)this; }

public:
	virtual bool	Create(IMessagePort* pPort,int SERVER_LISTEN_PORT,int SERVER_LISTEN_NPC_PORT);
	virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom);
	virtual bool	ProcessSocket();
	virtual bool	OnTimer(time_t tCurr);
	virtual bool	Release();

public:
	bool	SendCloseNotify(SOCKET_ID idSocket);
	//? 暂时用于控制只通知核心关闭一次
	bool	IsNetBreakFlag(SOCKET_ID idSocket)			{ return (idSocket>=0 && idSocket<m_setSocket.size() && m_setSocket[idSocket] && m_setNetBreakFlag[idSocket]); }
	void	SetNetBreakFlag(SOCKET_ID idSocket)			{ if(idSocket>=0 && idSocket<m_setSocket.size() && m_setSocket[idSocket]) m_setNetBreakFlag[idSocket] = true; }

public:
	//void			AcceptNewSocket();
	int				SetNewSocket(Session* sNew);
	int				ProcessRecvMsg(const char* pRecvBuf,int nSize,Session* pSession);
	void			ProcessDisconnServer(int i);
//	BOOL			IsNpcSocketID(int i){return i==m_idNpcSocket;}
protected:
	//IListenSocket*	m_pListen;
	//CListenSocket	m_cListen;

	//lock
	//CRITICAL_SECTION	m_setSocketLock;
	vector<Session*>	m_setSocket;				// SOCKET_ID对应索引,以下四个SET同步添加删除!
	//vector<SOCKET>	m_setServerSocket;
	CRITICAL_SECTION	m_ReuseIDListLock;			//同步用的临界区
	list<UINT>				m_ReuseIDList;				//记录m_setSocket中可以使用的idsocket
	//CRITICAL_SECTION	m_setProcessIDLock;
	vector<PROCESS_ID>		m_setProcessID;
	
	// PROCESS_ID对应MSGPORT号
	//CRITICAL_SECTION		m_setNetBreakFlagLock;
	vector<bool>			m_setNetBreakFlag;
	
	IMessagePort*	m_pMsgPort;
//	SOCKET_ID		m_idNpcSocket;
//	CRITICAL_SECTION	m_setNpcProcessIDLock;
//	typedef map<OBJID, PROCESS_ID>	NPCPROCESSID_MAP;
//	NPCPROCESSID_MAP	m_setNpcProcessID;			// 每个NPC的PROCESS_ID，新NPC的消息发到WORLD
	IOCPServer*				m_piocpServer;
//	IOCPServer*				m_piocpNPCServer;		//为NPC服务的IOCP

	time_t m_tNextClock;
	map<USHORT, ULONG>	m_setMsgStat;//消息统计
};

#endif // !defined(AFX_SERVERSOCKET_H__44A9ACE7_FE12_4CDC_9492_F39CD1423D77__INCLUDED_)
