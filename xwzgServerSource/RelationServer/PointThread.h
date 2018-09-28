// PointThread.h: 计点线程类
// 仙剑修：2001.11.20
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTTHREAD_H__6188DE47_48CA_459B_9207_A425EB6C6A0F__INCLUDED_)
#define AFX_POINTTHREAD_H__6188DE47_48CA_459B_9207_A425EB6C6A0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"
#include "T_Index.h"
#include "ThreadBase.h"
#include "VASDBManager.h"
#include "DataInspectManage.h"

enum	//	类型,放在itemAmount中
{
	_ITEM_LIFE_TIME_CONTINUE_TYPE_FULL = 1,		//	全额续费
	_ITEM_LIFE_TIME_CONTINUE_TYPE_HALF = 2,		//	半额续费
};

enum
{
	_VAS_CONSUMELOG_TYPE				=	0,	//	普通消息
	_VAS_CONSUMELOG_TYPE_CONTINUE_FULL	=	1,	//	全额续费
	_VAS_CONSUMELOG_TYPE_CONTINUE_HALF	=	2,	//	半额续费
	_VAS_CONSUMELOG_TYPE_PRESENT		=	3,	//	赠送
};


struct	CServerAccount
{
	char	m_szServerName[SERVERNAMESIZE];
	char	m_szLoginName[SERVERNAMESIZE];
	char	m_szPassword[SERVERNAMESIZE];
	char	m_szServerURL[SERVERNAMESIZE];
	int     m_nServerLoginMode;
	int		m_bFree;
	CServerAccount() { 
		m_szServerName[0] = m_szLoginName[0] = m_szPassword[0] = m_bFree = 0;
	}
};
/*
class CServerConnect
{
public:
	CServerConnect(SOCKET sock);
	~CServerConnect();
public:
	Update() { m_tHeartbeat = clock(); }
	Heartbeat() { m_tHeartbeatLast = clock(); }
protected:
	enum	{ STATE_NONE=0, STATE_SOCKET, STATE_LOGIN, STATE_NORMAL, STATE_BREAK };
	int			m_state;
	CServerSocket<POINT_KEY1, POINT_KEY2>	m_sockServer;
	char		m_szServerName[SERVERNAMESIZE];
	time_t		m_tHeartbeat;
	time_t		m_tHeartbeatLast;
};
//*/
class CPointThread : public CThreadBase  
{
public:		// 构造、析构
	CPointThread(u_short nPort, int nSndSize = 0);
	virtual ~CPointThread();

public:		// 共享函数，所有函数开始前必须互斥，仅能调用内部函数或“下级”函数
	enum { ERR_NONE=0, ERR_NO_SERVER, ERR_BUSY, ERR_FULL };
	int		NewLogin(OBJID idAccount, DWORD nAuthenID, const char * szClientIP, const char * pGameServer);	// 外部调用，要互斥
	bool	GetServerIP(char * bufIP, const char * pServerName);
//	int 	GetServerIndex(const char * pServerName) { CSingleLock(&m_xCtrl, true); return GetServerIndex_0(pServerName); }	// 返回0：错误
	int		GetServerCount();
	int		GetServerState(LPCTSTR szServerName);
//	bool	GetServerName(OBJID idServer, char bufName[SERVERNAMESIZE]);
	bool	CheckHeartbeatAll();
	void	LogCount();
	void	LogSerialCount();
	void	LogServer();
	bool	Kickout(OBJID idAccount);
	bool	AddTooLoginInfo(OBJID idAccount , const char * pGameServer);///[2009.3.18--张冬]监控工具数据添加
	int	    GetServerLoginMode(const char * pServerName);
	bool	GetServerURL(char * bufURL, const char * pServerName);
protected:	// 派生
	virtual void	OnInit();
	virtual bool	OnProcess();
	virtual void	OnDestroy();

public:
	enum { STATE_OFFLINE=0, STATE_NORMAL, STATE_BUSY, STATE_FULL };\
	CServerSocket<POINT_KEY1, POINT_KEY2>	m_aServerSocket[MAXGAMESERVERS];		//? 注意与m_aServerName同步

//protected:	// 变量
	int 	m_aState[MAXGAMESERVERS];
	char	m_aServerName[MAXGAMESERVERS][SERVERNAMESIZE];	//? 注意与m_aServerSocket同步，有名表示在线。
	CListenSocket	m_cListenSocket;
	time_t	m_aHeartbeat[MAXGAMESERVERS];
	time_t	m_aHeartbeatLast[MAXGAMESERVERS];
	time_t	m_aServerDelay[MAXGAMESERVERS];
	int		m_nServerLoginMode[MAXGAMESERVERS];
	char	m_aServerURL[MAXGAMESERVERS][SERVERNAMESIZE];

	int		m_aServerUserNum[MAXGAMESERVERS];
private:	// 仅共内部使用，不能有互斥
	bool	Kickout_0(OBJID idAccount, LPCTSTR szServerName);
	bool	ProcessMsg(int nServerIndex,  char * pBuf, int nLen);
	OBJID	GetServerIndex_0(const char * pServerName);	// 返回0：错误
	bool	CheckHeartbeat(int nIndex);
	void	Clear(int nIndex, bool flag = true);
	void	SetServerBusy(int nServerIndex);
	void	SetServerFull(int nServerIndex);
private:
	CDataInspectManage m_objInspectManage;
	CVASDBManager m_objManager;
};

#endif // !defined(AFX_POINTTHREAD_H__6188DE47_48CA_459B_9207_A425EB6C6A0F__INCLUDED_)
