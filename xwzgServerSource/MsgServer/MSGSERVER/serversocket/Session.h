#ifndef _SESSION_H_
#define _SESSION_H_

#include <windows.h>

#include "EncryptServer.h"

#define LOGIN_KEY1				0xa61fce5e	// A = 0x20, B = 0xFD, C = 0x07, first = 0x1F, key = a61fce5e
#define LOGIN_KEY2				0x443ffc04	// A = 0x7A, B = 0xCF, C = 0xE5, first = 0x3F, key = 443ffc04


#define SEND_POSTED			1
#define RECV_POSTED			2
#define ACCEPT_POSTED		3
#define DISCONNECT_POSTED	4

class SendBuffer;
class RecvBuffer;
class SessionPool;
//class NetworkObject;
class Session;
class IoHandler;

/// OVERLAPPED 확장 구조체
struct OVERLAPPEDEX : public OVERLAPPED
{
	DWORD		dwIoSize;
	DWORD		dwFlags;
	DWORD		dwOperationType;
	Session*	pSession;
};

//=============================================================================================================================
/// 기본적인 소켓 처리 기능을 갖고 있는 세션 클래스
/**
	@remarks
			이 클래스를 상속 받아 확장 유져 클래스를 만들 수 있다.
	@par
			- 세션을 상속받은 클래스에서는 다음의 함수들을 재정의 해서 사용한다.
			- OnAccept:		클라이언트가 accept되었을 때 호출된다.
			- OnDisConnect:	클라이언트의 접속이 해제 되었을 때 호출된다.
			- OnRecv:		클라리언트가 패킷을 수신했을 때 호출된다.
			- OnConnect:	다른 서버로의 connect가 성공 또는 실패했을 때 호출된다.
			- OnUpdate:		매프레임 IO 엔진이 Update될 때마다 호출된다.
	@note
			- ws2_32.lib 추가할 것
			- winsock2.h 포함할 것

*/
//=============================================================================================================================
class Session
{
	friend unsigned __stdcall io_thread( LPVOID param );
	friend class SessionPool;

public:
	/// dwTimeOut 값이 0이면 메세지 송신이 없어도 접속을 끊지 않는다.
	Session( DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize/*, DWORD dwTimeOut*/ );
	virtual ~Session();

	VOID					Init();
	VOID					Reuse( HANDLE hIOCP );
	BOOL					Send( BYTE *pMsg, WORD wSize );
//	BOOL					SendNPC( BYTE *pMsg, WORD wSize );
	BOOL					SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	BOOL					PreSend();
	BOOL					PreRecv(int nCode);
	BOOL					PreAccept( SOCKET listenSocket );
	SOCKET					CreateSocket();
	BOOL					ProcessRecvdPacket( DWORD dwMaxPacketSize );
	//VOID					BindNetworkObject( NetworkObject *pNetworkObject );
	//VOID					UnbindNetworkObject();
	VOID					OnAccept();
	//VOID					OnConnect( BOOL bSuccess );
	//VOID					OnLogString( char *pszLog, ... );

	inline VOID				SetSocket( SOCKET socket ) { m_socket = socket; }
	inline VOID				SetSockAddr( SOCKADDR_IN& sockaddr ) { m_sockaddr = sockaddr; }
	inline VOID				CloseSocket() { closesocket( m_socket ); m_socket = INVALID_SOCKET; }
	//inline NetworkObject*	GetNetworkObject() { return m_pNetworkObject; }
	inline SendBuffer*		GetSendBuffer() { return m_pSendBuffer; }				///< 보내기 버퍼 포인터를 리턴한다.
	inline RecvBuffer*		GetRecvBuffer() { return m_pRecvBuffer; }				///< 받기 버퍼 포인터를 리턴한다.
	inline SOCKET			GetSocket()		{ return m_socket; }					///< 소켓 핸들을 리턴한다.
	inline SOCKADDR*		GetSockAddr()	{ return (SOCKADDR*)&m_sockaddr; }		///< 소켓 주소의 포인터를 리턴한다.
	inline char*			GetIP() { return inet_ntoa( m_sockaddr.sin_addr ); }	///< 세션에 물린 소켓의 IP를 스트링으로 반환
	inline ULONG			GetULongIP()	{ return m_sockaddr.sin_addr.s_addr; }

	//inline DWORD			GetIdleTick() const	{ return m_dwTimeOut? m_dwLastSyncTick + m_dwTimeOut : 0; }
	//inline BOOL				IsOnIdle()
	//	{ return m_dwTimeOut ? m_dwLastSyncTick + m_dwTimeOut < GetTickCount() : FALSE; }
	inline DWORD			GetIndex() { return m_dwIndex; }
	inline BOOL				IsAcceptSocket() { return m_bAcceptSocket; }
	inline VOID				SetAcceptSocketFlag() { m_bAcceptSocket = TRUE; }
	VOID					Remove();
	inline VOID				ResetKillFlag() { InterlockedExchange( (long *)&m_bRemove, FALSE ); }
	inline BOOL				ShouldBeRemoved() { return m_bRemove; }
	VOID					Disconnect( BOOL bGracefulDisconnect );
	inline BOOL				HasDisconnectOrdered() { return m_bDisconnectOrdered; }

//	void					ChangeCode(DWORD dwData) { m_cEncryptRecv.ChangeCode(dwData); }
	void					EncyRecvData(int nSize);
	void					SetIoHandler(IoHandler* pHandler){m_pIoHandler = pHandler;}
	void					SetClientID(int nClientID){m_ClientID = nClientID;}
	int						GetClientID(){return m_ClientID;}

	void					AddOverlappedNum(){InterlockedIncrement(&m_OverlappedIONum);}
	void					ReleaseOverlappedNum(){InterlockedDecrement(&m_OverlappedIONum);}
	BOOL					CanReuse(){return m_OverlappedIONum<=0;}

// 	typedef	CEncryptServer<LOGIN_KEY1, LOGIN_KEY2>	EncryptServer;
// 	EncryptServer	m_cEncryptRecv, m_cEncryptSend;

private:
	VOID					SetIndex( DWORD index ) { m_dwIndex = index; }
	//inline VOID				ResetTimeOut() { m_dwLastSyncTick = GetTickCount(); }

	//NetworkObject			*m_pNetworkObject;
	IoHandler				*m_pIoHandler;
	SendBuffer				*m_pSendBuffer;
	RecvBuffer				*m_pRecvBuffer;

	SOCKET					m_socket;
	SOCKADDR_IN				m_sockaddr;

	OVERLAPPEDEX			m_sendIoData;
	OVERLAPPEDEX			m_recvIoData;
	OVERLAPPEDEX			m_acceptIoData;
	OVERLAPPEDEX			m_disconnectIoData;
	//DWORD					m_dwLastSyncTick;
	BOOL					m_bRemove;
	//DWORD					m_dwTimeOut;
	DWORD					m_dwIndex;
	BOOL					m_bAcceptSocket;
	BOOL					m_bDisconnectOrdered;

	int						m_ClientID;

	long						m_OverlappedIONum;

	// 임시 테스트 용
//	int						m_iCategory;
//	int						m_iProtocol;
//	int						m_iSize;
};

#endif
