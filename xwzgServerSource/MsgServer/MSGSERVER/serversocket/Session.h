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

/// OVERLAPPED Ȯ�� ����ü
struct OVERLAPPEDEX : public OVERLAPPED
{
	DWORD		dwIoSize;
	DWORD		dwFlags;
	DWORD		dwOperationType;
	Session*	pSession;
};

//=============================================================================================================================
/// �⺻���� ���� ó�� ����� ���� �ִ� ���� Ŭ����
/**
	@remarks
			�� Ŭ������ ��� �޾� Ȯ�� ���� Ŭ������ ���� �� �ִ�.
	@par
			- ������ ��ӹ��� Ŭ���������� ������ �Լ����� ������ �ؼ� ����Ѵ�.
			- OnAccept:		Ŭ���̾�Ʈ�� accept�Ǿ��� �� ȣ��ȴ�.
			- OnDisConnect:	Ŭ���̾�Ʈ�� ������ ���� �Ǿ��� �� ȣ��ȴ�.
			- OnRecv:		Ŭ�󸮾�Ʈ�� ��Ŷ�� �������� �� ȣ��ȴ�.
			- OnConnect:	�ٸ� �������� connect�� ���� �Ǵ� �������� �� ȣ��ȴ�.
			- OnUpdate:		�������� IO ������ Update�� ������ ȣ��ȴ�.
	@note
			- ws2_32.lib �߰��� ��
			- winsock2.h ������ ��

*/
//=============================================================================================================================
class Session
{
	friend unsigned __stdcall io_thread( LPVOID param );
	friend class SessionPool;

public:
	/// dwTimeOut ���� 0�̸� �޼��� �۽��� ��� ������ ���� �ʴ´�.
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
	inline SendBuffer*		GetSendBuffer() { return m_pSendBuffer; }				///< ������ ���� �����͸� �����Ѵ�.
	inline RecvBuffer*		GetRecvBuffer() { return m_pRecvBuffer; }				///< �ޱ� ���� �����͸� �����Ѵ�.
	inline SOCKET			GetSocket()		{ return m_socket; }					///< ���� �ڵ��� �����Ѵ�.
	inline SOCKADDR*		GetSockAddr()	{ return (SOCKADDR*)&m_sockaddr; }		///< ���� �ּ��� �����͸� �����Ѵ�.
	inline char*			GetIP() { return inet_ntoa( m_sockaddr.sin_addr ); }	///< ���ǿ� ���� ������ IP�� ��Ʈ������ ��ȯ
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

	// �ӽ� �׽�Ʈ ��
//	int						m_iCategory;
//	int						m_iProtocol;
//	int						m_iSize;
};

#endif
