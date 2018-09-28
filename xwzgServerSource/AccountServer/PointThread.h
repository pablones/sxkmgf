// PointThread.h: �Ƶ��߳���
// �ɽ��ޣ�2001.11.20
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
#include "MsgConnect.h"
#include "MsgAccount.h"
#include "FatiGueManager.h"

#define DEBUG_TRY try{
#define	DEBUG_CATCH(x)	}catch(...){ LOGERROR("CATCH: *** " x " crash()! ***\n"); }

enum	//	����,����itemAmount��
{
	_ITEM_LIFE_TIME_CONTINUE_TYPE_FULL = 1,		//	ȫ������
	_ITEM_LIFE_TIME_CONTINUE_TYPE_HALF = 2,		//	�������
};

enum
{
	_VAS_CONSUMELOG_TYPE				=	0,	//	��ͨ��Ϣ
	_VAS_CONSUMELOG_TYPE_CONTINUE_FULL	=	1,	//	ȫ������
	_VAS_CONSUMELOG_TYPE_CONTINUE_HALF	=	2,	//	�������
	_VAS_CONSUMELOG_TYPE_PRESENT		=	3,	//	����
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
public:		// ���졢����
	CPointThread(u_short nPort, int nSndSize = 0);
	virtual ~CPointThread();

public:		// �����������к�����ʼǰ���뻥�⣬���ܵ����ڲ��������¼�������
	enum { ERR_NONE=0, ERR_NO_SERVER, ERR_BUSY, ERR_FULL };
	int		NewLogin(OBJID idAccount, DWORD nAuthenID, const char * szClientIP, const char * pGameServer,bool bYouke = false);	// �ⲿ���ã�Ҫ����
// 	void	CheckNewLogin(OBJID idAccount,const char * pGameServer);///����Ϸ���������Ͳ�ѯ��Ϣ,ѯ���Ƿ�����ظ�����
	bool	GetServerIP(char * bufIP, const char * pServerName);
	bool	GetServerURL(char * bufURL, const char * pServerName);
	UINT	GetServerPort(const char * pServerName);
	void	MakeConnectMsg(CMsgConnect& msg,OBJID idAccount, DWORD nAuthenID,LPCTSTR szServer);
	void	TakeBestServer(CMsgAccount& msg);
	int		GetServerLoginMode( const char * pServerName);
//	int 	GetServerIndex(const char * pServerName) { CSingleLock(&m_xCtrl, true); return GetServerIndex_0(pServerName); }	// ����0������
	int		GetServerCount();
	int		GetServerState(LPCTSTR szServerName);
//	bool	GetServerName(OBJID idServer, char bufName[SERVERNAMESIZE]);
	bool	CheckHeartbeatAll();
	void	LogCount();
	void	LogSerialCount();
	void	LogServer();
	bool	Kickout(OBJID idAccount);
	bool    Kickout(OBJID idAccount,LPCTSTR szServerName);
	bool	AddTooLoginInfo(OBJID idAccount , const char * pGameServer);///[2009.3.18--�Ŷ�]��ع����������

protected:	// ����
	virtual void	OnInit();
	virtual bool	OnProcess();
	virtual void	OnDestroy();
	void OnBreathe();///ר����һЩ��ʱ����
	void UpServerListUI();
	void WriteServerOnlinePlayerCount();
public:
	enum { STATE_OFFLINE=0, STATE_NORMAL, STATE_BUSY, STATE_FULL };
protected:	// ����
	int 	m_aState[MAXGAMESERVERS];
	CServerSocket<POINT_KEY1, POINT_KEY2>	m_aServerSocket[MAXGAMESERVERS];		//? ע����m_aServerNameͬ��
	char	m_aServerName[MAXGAMESERVERS][SERVERNAMESIZE];	//? ע����m_aServerSocketͬ����������ʾ���ߡ�
	char	m_aServerURL[MAXGAMESERVERS][SERVERNAMESIZE];
	int		m_nServerLoginMode[MAXGAMESERVERS];
	CListenSocket	m_cListenSocket;
	time_t	m_aHeartbeat[MAXGAMESERVERS];
	time_t	m_aHeartbeatLast[MAXGAMESERVERS];
	time_t	m_aServerDelay[MAXGAMESERVERS];
	
	int		m_aServerUserNum[MAXGAMESERVERS];
	int		m_aServerPort[MAXGAMESERVERS];///������Ϸ�����������ͻ��˵Ķ˿��б�
private:	// �����ڲ�ʹ�ã������л���
	bool	Kickout_0(OBJID idAccount, LPCTSTR szServerName);
	bool	ProcessMsg(int nServerIndex,  char * pBuf, int nLen);
	OBJID	GetServerIndex_0(const char * pServerName);	// ����0������
	bool	CheckHeartbeat(int nIndex);
	void	Clear(int nIndex, bool flag = true);
	void	SetServerBusy(int nServerIndex);
	void	SetServerFull(int nServerIndex);
private:
	CDataInspectManage m_objInspectManage;
	CVASDBManager m_objManager;
	CFatiGueManager m_FatiManager;
	
};

#endif // !defined(AFX_POINTTHREAD_H__6188DE47_48CA_459B_9207_A425EB6C6A0F__INCLUDED_)
