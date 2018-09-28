#ifndef _IOGROUP_H_
#define _IOGROUP_H_
#pragma warning(disable:4786)
#include <windows.h>
#include <process.h>
#include <Ws2tcpip.h>

#define MAX_ACCEPT_THREAD	16

class Session;
class IoHandler;
class SessionPool;

typedef struct tagIOHANDLER_DESC IOHANDLER_DESC, *LPIOHANDLER_DESC;

//=============================================================================================================================
/// 리슨 소켓을 열고 접속을 처리하는 클래스
//	- I/O 핸들러는 하나의 포트를 가지고 리슨할 수 있으며 별도의 세션 풀을 가지고 있다.
//	- 접속이 이루어지면 세션풀에서 하나의 세션을 할당받아 소켓을 초기화한 후 TempSessionList에 추가한다.
//=============================================================================================================================
class Acceptor
{
	friend unsigned __stdcall accept_thread( LPVOID param );

public:
	Acceptor();
	~Acceptor();

	VOID				Init( IoHandler *pIoHandler, DWORD dwNumberOfThreads );
	BOOL				StartListen( char *pIP, WORD wPort );
	VOID				Shutdown();
	inline BOOL			IsListening() { return m_listenSocket != INVALID_SOCKET; }
	VOID				SuspendListenThread();
	VOID				ResumeListenThread();
	inline SOCKET		GetListenSocket() { return m_listenSocket; }

private:
	BOOL				CreateListenSocket( char *pIP, WORD wPort );

	IoHandler			*m_pIoHandler;
	HANDLE				m_hThread[MAX_ACCEPT_THREAD];
	HANDLE				m_hIOCP;
	SOCKET				m_listenSocket;
	SOCKADDR_IN			m_sockaddr;
	DWORD				m_numThreads;

	//HANDLE				m_hEvent;
};

#endif


