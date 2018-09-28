// ServerSocket.h: interface for the CSocketKernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSOCKET_H__44A9ACE7_FE12_4CDC_9492_F39CD1423D77__INCLUDED_)
#define AFX_SERVERSOCKET_H__44A9ACE7_FE12_4CDC_9492_F39CD1423D77__INCLUDED_

#pragma warning(disable:4786)
#include <time.h>
#include "../share/define.h"

//zeng �޸�ΪIOCP



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
	CSocketKernel() {}		// ��������
	virtual ~CSocketKernel() {}		// ��������

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
	//? ��ʱ���ڿ���ֻ֪ͨ���Ĺر�һ��
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
	vector<Session*>	m_setSocket;				// SOCKET_ID��Ӧ����,�����ĸ�SETͬ�����ɾ��!
	//vector<SOCKET>	m_setServerSocket;
	CRITICAL_SECTION	m_ReuseIDListLock;			//ͬ���õ��ٽ���
	list<UINT>				m_ReuseIDList;				//��¼m_setSocket�п���ʹ�õ�idsocket
	//CRITICAL_SECTION	m_setProcessIDLock;
	vector<PROCESS_ID>		m_setProcessID;
	
	// PROCESS_ID��ӦMSGPORT��
	//CRITICAL_SECTION		m_setNetBreakFlagLock;
	vector<bool>			m_setNetBreakFlag;
	
	IMessagePort*	m_pMsgPort;
//	SOCKET_ID		m_idNpcSocket;
//	CRITICAL_SECTION	m_setNpcProcessIDLock;
//	typedef map<OBJID, PROCESS_ID>	NPCPROCESSID_MAP;
//	NPCPROCESSID_MAP	m_setNpcProcessID;			// ÿ��NPC��PROCESS_ID����NPC����Ϣ����WORLD
	IOCPServer*				m_piocpServer;
//	IOCPServer*				m_piocpNPCServer;		//ΪNPC�����IOCP

	time_t m_tNextClock;
	map<USHORT, ULONG>	m_setMsgStat;//��Ϣͳ��
};

#endif // !defined(AFX_SERVERSOCKET_H__44A9ACE7_FE12_4CDC_9492_F39CD1423D77__INCLUDED_)
