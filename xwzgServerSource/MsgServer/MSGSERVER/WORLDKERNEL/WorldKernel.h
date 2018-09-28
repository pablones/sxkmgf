// WorldKernel.h: interface for the CWorldKernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLDKERNEL_H__9A393F65_C167_402E_8A17_7EA70AC5879E__INCLUDED_)
#define AFX_WORLDKERNEL_H__9A393F65_C167_402E_8A17_7EA70AC5879E__INCLUDED_

#include <time.h>
#include <winsock2.h>
#include "../../GameBaseCodeMT/I_mydb.h"
#include "NetMsg.h"
#include "I_Shell.h"
#include "T_Singlemap.h"
#include "ClientSocket.h"
#include "SynWorldManager.h"
#include "AutoPtr.h"

#include "ShopManager.h"//[��;���� 2009.01.06]����ϵͳ������
#include "ShopVAS.h"
#include "MsgRelay.h"

#include "arenamanager.h"
//#include "TutorWorld.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol.h"
using namespace world_kernel;
#include "UserList.h"
#include "MapList.h"
#include "ConnectUser.h"
#include "T_SingleMap.h"

#define NEW_ROLE_STR		"NEW_ROLE"			// ����������
const int	MAX_GM_ID			= 2000;			// С��1000��ACCOUNT_ID����¼��������������

typedef	ISingleMap<CConnectUser>	IConnectSet;
typedef	CSingleMap<CConnectUser>	CConnectSet;

const	DWORD	MONITOR_SERVER_SEND_INTERVAL = 5000;

		extern char g_szKernelState[1024 * 10];
		extern char g_szShellState[1024 * 10];

struct ConnectInfo_ST
{
	OBJID			idAccount;
	SOCKET_ID		idSocket;
	ULONG			nIP;
	char			szInfo[_MAX_NAMESIZE];
};
typedef vector<ConnectInfo_ST> CONNECT_INFO_SET;

class ISynMessage
{
public:
	virtual bool	CreateSyndicate		(const CreateSyndicateInfo* pInfo)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	DestroySyndicate	(OBJID idSyn, OBJID idTarget = ID_NONE)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	ChangeSyndicate		(const SynFuncInfo0* pFuncInfo)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	StopSayByGM			(char* szname, int sec)							PURE_VIRTUAL_FUNCTION_0
};

class CWorldKernel : public IWorld, ISocket, ISynMessage
{
public:
	CWorldKernel() 
	{
		m_setConnectClient=0;m_nState=0;m_WorldTime = 0;dwConnectAmount = 0;m_SyndicateTime = 0;m_nServerPort=0;
		ZeroMemory(m_szMsgServerList,sizeof(m_szMsgServerList));			
	}		// ��������
	virtual ~CWorldKernel() {}		// ��������
//  [2/12/2007 Halei]------------------------------------------------------
public:
	struct ConnectInfo_WaitQueue
	{
		DWORD		usAction;	//�Ƿ�����Ŷ�	
		OBJID		idAccount;	//�ʺţ��������Ψһ��ʶ	
		DWORD		EndWaitTime;   //�����Ŷ�ʱ��
		DWORD		idSocket;		//�׽���
	};	
typedef deque<ConnectInfo_WaitQueue > CONNENT_INFO_WAITQUEUE;//�Ŷ�halei
 CONNENT_INFO_WAITQUEUE WaitQueue;
 CTimeOut	m_WaitQueueTimer;
//  [2/12/2007 Halei]------------------------------------------------------
public:
	IWorld*	GetInterface()				{ return (IWorld*)this; }
	ISocket*	GetSocketInterface()	{ return (ISocket*)this; }
	CUserList*	QueryUserList()			{ ASSERT(m_pUserList); return m_pUserList; }
	CMapList*	QueryMapList()			{ ASSERT(m_pMapList); return m_pMapList; }
	ISynMessage*	QuerySynMessage()	{ return (ISynMessage*)this; }
	CSynWorldManager*	QuerySynManager()	{ ASSERT(m_pSynManager); return m_pSynManager; }
	CShopVAS*	QueryShopVAS()	{ASSERT(m_pShopVAS); return m_pShopVAS;}
protected: // IWorld
	virtual bool	Create(IMessagePort* pPort);
	virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom);
	virtual bool	ProcessSocketMsg();
	virtual bool	OnTimer(time_t tCurr);
	virtual bool	Release();

protected: // ISocket
	virtual bool SendMsg			(CNetMsg* pNetMsg);
	virtual bool SendMsg			(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen);
//	virtual bool SendNpcMsg			(CNetMsg* pNetMsg);
//	virtual bool SendNpcMsg			(OBJID idNpc, OBJID idMsg, const char* pBuf, int nMsgLen);
	virtual bool CloseSocket		(SOCKET_ID idSocket);			// ֱ�ӹر�socket

public: // send
	virtual bool SendClientMsg		(SOCKET_ID idSocket, CNetMsg* pNetMsg);
//	virtual bool SendNpcMsg			(OBJID idNpc, CNetMsg* pNetMsg);
	virtual bool PrintText			(LPCTSTR szText);
	virtual bool ChangeCode			(SOCKET_ID idSocket, DWORD dwData);

public: // account server
	//---------07.01.05���Ϊ��ֵ������Ϣ���ʺŷ�������---------------
	bool SendNewPlayerExam(  int UserID, int nAccountID,int m_unMsgType, int m_unMsgSize );
//	bool SendVASRequest( int id, int itemAmount, OBJID itemID, int usAction, OBJID AccountID, OBJID PlayerID, int usVASPoint, int un_MsgType, int m_unMsgSize );
//	bool SendVASRequest( int id, int itemAmount, OBJID itemID, DWORD dwData, int usAction, OBJID AccountID, OBJID PlayerID, int usVASPoint, int un_MsgType, int m_unMsgSize ,int nJuan = 0,int nRealID = 0);
	bool SendVASRequest(USHORT unMsgType, USHORT unMsgSize, ULONG nVASPoint, OBJID AccountID, OBJID PlayerID, USHORT usAction, USHORT usGoodSize, CMsgVAS::MsgTraffic* nGoods);
	bool SendNewCardRequest(USHORT unMsgType, USHORT unMsgSize, DWORD idUser , DWORD idAcount , DWORD dwType, char* pszBuf);
//	void ProcessVAS( void* m_pInfo,UINT usVASPoint,OBJID AccountID,OBJID PlayerID,int nJuan  = 0 );
	void ProcessVAS(USHORT unMsgType, USHORT unMsgSize, ULONG nVASPoint, OBJID AccountID, OBJID PlayerID, USHORT usAction, USHORT usGoodSize, CMsgVAS::MsgTraffic* nGoods);
	bool SendLastLineToAcc(UINT nMsgType,UINT nAction, OBJID accountID, OBJID idUser, const char* szServerName, const GainAwardInfo* gainInfo, int nAmount, int nMsgSize);
	bool SendAwardInfoToMapGroup(const char* pInfo, int nSize);

	bool SendStopAccount(OBJID StopAccountID,int un_MsgType, int m_unMsgSize ,USHORT nAction ,OBJID playerID);
	//.................................................
	//  [2/12/2007 Halei]-------------------------------------------------------����Ƿ���Ҫ�Ŷ��㷨
	BOOL CheckWaitQueue(OBJID idAccount,SOCKET_ID usersock );//HALEI
	//  [2/12/2007 Halei]-------------------------------------------------------
	void WaitQueueSocketShutDown( SOCKET_ID usersock );		//  [2/12/2007 Halei]����ҽ���ȴ������쳣�˳�ʱ
	void ConnectShutDown( SOCKET_ID usersock );	
	bool IsAccountLoginOK()						{ return m_nState == STATE_NORMAL; }
	bool SendAccount(LPCTSTR szAccount, LPCTSTR szPassword, LPCTSTR szServer,UINT nServerPort);
	bool SendLogin(OBJID idAccount, DWORD dwCheckData, int usVersion, LPCTSTR szUserName);
	bool SendFee(OBJID idAccount, int nType);		

	void SendRelationMsg(MsgRelayAction msgAction,OBJID srcRoleID,LPCTSTR szRoleName="",OBJID dstRoleID=0);
	void SendRelationRelayMsg(OBJID idMsg,const char* pack, int nLen,int nExtra=0);///ת����Ϣ����ϵ��������,ÿһ�����͵���Ϣ��������Ϣβ����4���ֽ�����,�������涪ʧ��Դ�û�ID
	void BroadcastMsg(OBJID idMsg, const char *pBuf, int nMsgLen);

	bool SendQueryFee(OBJID idAccount, DWORD dwData, int nTime, int ucType);
	void ProcessAccountNetMsg();
	void ProcessRelationNetMsg();
	void ProcessLogin(OBJID idAccount, DWORD dwCheckData, int usVersion, LPCTSTR szUserName);
	void ProcessFee(OBJID idAccount, int ucType);
	void ProcessQueryFee(OBJID idAccount, DWORD dwData, int nTime, int ucType);
	bool ClientConnect(OBJID idAccount, DWORD dwData, LPCTSTR szInfo, SOCKET_ID idSocket,int state,int nYoukeLogin = 0,ULONG nIP=0);			// return true: pass barrier
	bool SendRelationAccount(LPCTSTR szAccount, LPCTSTR szPassword, LPCTSTR szServer);///��������
	void SendRelationAllUser();///��������ӵ�е����������Ϣ���ǵ���ϵ��������
	void SendAccountAllUser();///�����������е����������Ϣ���ǵ��ʺŷ�������
	bool	EndConnectUserByAccount( OBJID idAccount );					//[7/2/2007]�������
	bool	EndConnectUserBySocket( SOCKET_ID idSocket );				//07.5.26�������
	SOCKET_ID GetSocketFromConnectUser( OBJID idAccount );				//07.6.7������� 
protected:
	bool ProcessNpcMsg		(OBJID idNpc, OBJID idMsg, const char* pBuf, int nMsgLen, int nTrans=0);
	bool ProcessClientMsg	(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen, int nTrans=0, ULONG nIP=0);
	PROCESS_ID	GetProcessID() { return m_idProcess; }

public:
	IMessagePort*	GetMsgPort()							{ return m_pMsgPort; }
	IDatabase*		GetDatabase()							{ return m_pDb; }
	SOCKET_ID		GetNpcSocketID()						{ return m_idNpcSocket; }
	void			SetNpcSocketID(SOCKET_ID idNpcSocket)	{ m_idNpcSocket = idNpcSocket; }
	bool			LoadConfig(IDatabase* pDb);

public:
	bool SendTutorToMapGroup(const ChgTutorInfo* pInfo);        //����ʦͽ��Ϣ����ͼ��
		bool SendMapGroupMsg(CNetMsg *msg);//ת����ͼ����Ϣ
	void BroadcastMainServerMsg(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize, int nTrans,PROCESS_ID idIProcess);
	void BroadcastRegisterMsg(OBJID idMsg, const char *pBuf, int nMsgLen, SOCKET_ID idSocketIgnore=-1);
	bool SendMapGroupMsgForVAS(int nMapGroup,SOCKET_ID idSocket,CNetMsg *pMsg);

public: // MessageBoard ---------------------
	bool TransmitMapGroupMsg(PROCESS_ID idProcess,CNetMsg* pMsg);
	CMessageBoard*	QueryTradeMsgBd()						{ CHECKF(m_ptrTradeMsgBd); return m_ptrTradeMsgBd; }
	CMessageBoard*	QueryFriendMsgBd()						{ CHECKF(m_ptrFriendMsgBd); return m_ptrFriendMsgBd; }
	CMessageBoard*	QueryTeamMsgBd()						{ CHECKF(m_ptrTeamMsgBd); return m_ptrTeamMsgBd; }
	CMessageBoard*	QueryOtherMsgBd()						{ CHECKF(m_ptrOtherMsgBd); return m_ptrOtherMsgBd; }
	CMessageBoard*	QuerySystemMsgBd()						{ CHECKF(m_ptrSystemMsgBd); return m_ptrSystemMsgBd; }
protected:
	typedef	CAutoPtr<CMessageBoard>	MSGBD_PTR;
	MSGBD_PTR		m_ptrTradeMsgBd;
	MSGBD_PTR		m_ptrFriendMsgBd;
	MSGBD_PTR		m_ptrTeamMsgBd;
	MSGBD_PTR		m_ptrOtherMsgBd;
	MSGBD_PTR		m_ptrSystemMsgBd;

protected: // Syndicate
	virtual bool	CreateSyndicate		(const CreateSyndicateInfo* pInfo);
	virtual bool	DestroySyndicate	(OBJID idSyn, OBJID idTarget = ID_NONE);
	virtual bool	ChangeSyndicate		(const SynFuncInfo0* pFuncInfo);

public:
	bool DeleteRole(OBJID idAccount,DWORD dwData,LPCTSTR password,LPCTSTR name);
	bool LoginUserSnap(const ConnectInfo_ST &LoginInfo,int nYouke = 0);
	void KickOutSocket(SOCKET_ID idSocket);
	bool			LoginUserByInfo(const ConnectInfo_ST& info);
	bool IsRelationServerOpen()///�жϹ�ϵ�������Ƿ����
	{
		return m_sockForRelationServer.IsOpen();
	}
	void TransmitBroadcastMapGroupMsg(CNetMsg* pMsg,PROCESS_ID idIProcessIgnore=-1);
	void TransmitBroadcastMapGroupMsg(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize, int nTrans,PROCESS_ID idIProcessIgnore=-1);
	bool SendMapGroupMsgForChatRoom(int nMapGroup,SOCKET_ID idSocket,CNetMsg *pMsg);
//modify code by �ֵ�Ԩ begin 2011.5.30
	bool SendVASLVRequest(USHORT unMsgType,unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan);
//modify code by �ֵ�Ԩ end

protected:
	PROCESS_ID		m_idProcess;
	IDatabase*		m_pDb;
	IMessagePort*	m_pMsgPort;
	CUserList*		m_pUserList;
	CMapList*		m_pMapList;
//	CShop			*m_pShop;//[��;���� 2009.01.04]
	IRecordset		*m_pShopItemDataDefault;
	CSynWorldManager*	m_pSynManager;
	CShopVAS		*m_pShopVAS;

	SOCKET_ID		m_idNpcSocket;
//	CTutorWorld*	m_pTutorWord;

protected: // account serve
	
	///��ϵ���������
	CClientSocket	m_sockForRelationServer;
	int				m_RServerPort;
	char			m_szMsgServerList[64*5];///���浱ǰ������Ϸ�������б�
	///��ϵ���������

	CClientSocket	m_sock;
/*------------------------------begin-----------------------------------------------------------------*/
	CClientSocket	m_sockForStateServer;		//06.8.7�������,����״̬�������õ�socket
	int				m_nState2;					//06.8.7�������,����״̬�������õı�ʶ..
	clock_t			m_dwTimeForStateServer;		//06.8.7�������,����״̬��������

	int				m_nState3;	
	clock_t			m_dwSendMonitorServer;	

	CClientSocket	m_sockMonitor;
#pragma pack(1)
struct
{
	unsigned short	unMsgType;
	char			szServerName[ 16 ];//
	int				nOnlineUserAmount;		
}m_SendToStateInfo;
#pragma pack()
/*---------------------------------end----------------------------------------------------------------*/
public:		//  [2/12/2007 Halei]	�ı�m_setConnectClinetΪPublicΪ�ŶӺ���ʹ��
	void ProcessFATI(USHORT unMsgType, USHORT unMsgSize, OBJID AccountID, OBJID PlayerID, int Fatigue);
	bool SendFATIRequest(USHORT unMsgType, USHORT unMsgSize, OBJID AccountID, OBJID PlayerID,  int Fatigue);
	void ProcessNewCard(USHORT unMsgType, USHORT unMsgSize, DWORD idUser , DWORD idAcount , DWORD dwType, char* pszBuf);
	void processStopAccount(USHORT nAction, OBJID playerID, UINT nParam);
	void ProcessMonitorNetMsg();
	IConnectSet*		m_setConnectClient;
	DWORD				dwConnectAmount;		// ������� [6/5/2007]
	NAMESTR				m_szServer;
	static CWorldKernel*	m_pWorld;

	int GetLineIndex();
protected:
	OBJID		m_idServer;			// ��Ϸ��������ID
	enum { STATE_NONE, STATE_CONNECTOK, STATE_ACCOUNTOK, STATE_NORMAL, STATE_STATEOK };//06.8.7����������һ��
	int			m_nState;
	NAMESTR		m_szAccount;
	NAMESTR		m_szPassword;
	UINT		m_nServerPort;
	int			m_WorldTime;	//���0322 ��ʱ��
	int			m_SyndicateTime;
	bool		m_bArenaUpdate;

protected: // static this
	SYSTEMTIME m_stLastDatabaseReconnectTime;
	bool ReconnectDB();
	void SendServerTrace(int nType, char* pszTrace);
	friend CWorldKernel* GameWorld();
};

inline CWorldKernel* GameWorld()		{ ASSERT(CWorldKernel::m_pWorld); return CWorldKernel::m_pWorld; }
inline CUserList*	UserList()			{ return GameWorld()->QueryUserList(); }
inline CMapList*	MapList()			{ return GameWorld()->QueryMapList(); }
inline CSynWorldManager*	SynWorldManager()	{ return GameWorld()->QuerySynManager(); }
//inline CTutorWorld*	TutorWorld()    	{ return GameWorld()->QueryTutorWorld(); }

#define	g_UserList	(*UserList())

#endif // !defined(AFX_WORLDKERNEL_H__9A393F65_C167_402E_8A17_7EA70AC5879E__INCLUDED_)
