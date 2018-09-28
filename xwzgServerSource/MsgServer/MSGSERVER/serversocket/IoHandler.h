#ifndef _IOHANDLER_H_
#define _IOHANDLER_H_

#include <windows.h>
#include <process.h>
#include <vector>
#include "SessionList.h"

using namespace std;

#define MAX_IO_WORKER_THREAD	32
#define EXTRA_ACCEPTEX_NUM		10

class Session;
class SessionPool;
class Acceptor;
class Connector;
class SessionList;
class IOCPServer;
class NetworkObject;
struct tagIOHANDLER_DESC;

typedef tagIOHANDLER_DESC IOHANDLER_DESC, *LPIOHANDLER_DESC;

//typedef NetworkObject* (*fnCallBackCreateAcceptedObject)();
//typedef VOID (*fnCallBackDestroyAcceptedObject)( NetworkObject *pNetworkObject );
//typedef VOID (*fnCallBackDestroyConnectedObject)( NetworkObject *pNetworkObject );
//typedef VOID (*fnCallBackLog)( NetworkObject pNetworkObject, char *pszLog );

//=============================================================================================================================
/// 리슨 소켓을 열고 접속을 처리하는 클래스
//			- I/O 핸들러는 하나의 포트를 가지고 리슨할 수 있으며 별도의 세션 풀을 가지고 있다.
//			- 접속이 이루어지면 세션풀에서 하나의 세션을 할당받아 소켓을 초기화한 후 TempSessionList에 추가한다.
//=============================================================================================================================
class IoHandler
{
	friend unsigned __stdcall io_thread( LPVOID param );
	friend unsigned __stdcall send_thread( LPVOID param );
	friend unsigned __stdcall accept_thread( LPVOID param );
	friend unsigned __stdcall connect_thread( LPVOID param );
	friend unsigned __stdcall update_thread(LPVOID param);
public:
	IoHandler();
	~IoHandler();

	VOID				Init( IOCPServer *pIOCPServer, LPIOHANDLER_DESC lpDesc );
	BOOL				StartListen( char *pIP, WORD wPort );
	VOID				Update();
	VOID				Shutdown();
	//DWORD				Connect( NetworkObject *pNetworkObject, char *pszIP, WORD wPort );
	BOOL				IsListening();
	inline DWORD		GetNumberOfConnections()		{ return m_numActiveSessions; }
	inline DWORD		GetKey() { return m_dwKey; }
	IOCPServer*			GetIOCPServer() {return m_pIOCPServer; }
	VOID				ProcessSend();
private:
	Session*			AllocAcceptSession();
	//Session*			AllocConnectSession();
	VOID				FreeSession( Session *pSession );
	VOID				ReuseSession( Session *pSession );
	//VOID				ProcessConnectSuccessList();
	//VOID				ProcessConnectFailList();
	VOID				ProcessAcceptedSessionList();
	VOID				ProcessActiveSessionList();
	VOID				KickDeadSessions();

	VOID				KickAllSessions();
	VOID				ProcessReuseSessionList();

	IOCPServer			*m_pIOCPServer;
	SessionPool			*m_pAcceptSessionPool;
	//SessionPool			*m_pConnectSessionPool;
	Acceptor			*m_pAcceptor;
	//Connector			*m_pConnector;
	SessionList			*m_pActiveSessionList;
	SessionList			*m_pAcceptedSessionList;
	SessionList			*m_pReuseSessionList;
	//SessionList			*m_pConnectSuccessList;
	//SessionList			*m_pConnectFailList;
	SessionList			*m_pTempList;
	SessionList			*m_pDeadSessionList;
	vector<Session*>	m_sessionListHolder;///괏닸杰唐渡롸토돨삔뺐뚤蹶

	BOOL				m_bShutdown;

	DWORD				m_dwKey;
	HANDLE				m_hIOCP;							// IOCP 핸들
	DWORD				m_numIoThreads;						// IO 워커 쓰레드 개수
	HANDLE				m_hIoThread[MAX_IO_WORKER_THREAD];	// IO 쓰레드 핸들
	DWORD				m_dwMaxPacketSize;
	DWORD				m_numActiveSessions;
	DWORD				m_dwMaxAcceptSession;

	// HANDLE				m_hUpdateThread;

	// 콜백 함수 포인터
	//fnCallBackCreateAcceptedObject		m_fnCreateAcceptedObject;
	//fnCallBackDestroyAcceptedObject		m_fnDestroyAcceptedObject;
	//fnCallBackDestroyConnectedObject	m_fnDestroyConnectedObject;
};

#endif


