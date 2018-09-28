// MapGroupKernel.h: interface for the CMapGroupKernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPGROUPKERNEL_H__5901B393_10B2_410C_A305_C639044FF8B2__INCLUDED_)
#define AFX_MAPGROUPKERNEL_H__5901B393_10B2_410C_A305_C639044FF8B2__INCLUDED_

#include <time.h>
#include <winsock2.h>
#include "../../GameBaseCodeMT/I_mydb.h"
#include "../share/define.h"
#include "usermanager.h"
#include "network\\NetMsg.h"
#include "I_Shell.h"
#include <map>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapGroupKernel : public IMapGroup, ISocket
{
public:
	CMapGroupKernel() {}		// 建议留空
	virtual ~CMapGroupKernel() {}		// 建议留空

public:
	IMapGroup*	GetInterface() { return (IMapGroup*)this; }
	ISocket*	GetSocketInterface() { return (ISocket*)this; }

protected: // IMapGroup
	virtual bool	Create(IMessagePort* pPort);
	virtual void	SynchroData();
	virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom);
	virtual bool	OnTimer(time_t tCurr);
	virtual bool	Release();

protected: // ISocket
	bool SendMsg			(CNetMsg* pNetMsg);
	bool SendMsg			(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen);
//	bool SendNpcMsg			(OBJID idNpc, OBJID idMsg, const char* pBuf, int nMsgLen);
	bool CloseSocket		(SOCKET_ID idSocket);		// 直接关闭socket

protected:
	bool ProcessClientMsg	(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen, int nTrans=0);
//	bool ProcessNpcMsg		(OBJID idNpc, OBJID idMsg, const char* pBuf, int nMsgLen, int nTrans=0);
	bool BroadcastMapGroupMsg		(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen);
	//王玉波添加
	bool BroadcastPhyleMapGroupMsg ( int nPhyle,SOCKET_ID idSocket,OBJID idMsg,const char* pBuf,int nMsgLen );

	PROCESS_ID	GetProcessID() { return m_idProcess; }

protected:
	SYSTEMTIME m_stLastDatabaseReconnectTime;
	bool ReconnectDB();
	void ProcessFriendApply(ST_FRIEND_APPLY* pInfo);
	void ProcessTeamInviteMsg(ST_TEAM_INVITE* pTeamInvite);
	PROCESS_ID		m_idProcess;
	IDatabase*		m_pDb;
	IMessagePort*	m_pMsgPort;
	map<int,int> m_setCheckMsg;

public:
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAPGROUPKERNEL_H__5901B393_10B2_410C_A305_C639044FF8B2__INCLUDED_)
