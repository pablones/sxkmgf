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
/// ���� ������ ���� ������ ó���ϴ� Ŭ����
//	- I/O �ڵ鷯�� �ϳ��� ��Ʈ�� ������ ������ �� ������ ������ ���� Ǯ�� ������ �ִ�.
//	- ������ �̷������ ����Ǯ���� �ϳ��� ������ �Ҵ�޾� ������ �ʱ�ȭ�� �� TempSessionList�� �߰��Ѵ�.
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


