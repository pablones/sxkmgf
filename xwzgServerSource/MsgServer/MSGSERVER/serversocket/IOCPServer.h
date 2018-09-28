#pragma once

#pragma comment( lib, "ws2_32.lib" )

#include <windows.h>
#include <map>

class CSocketKernel;

class Session;
class IoHandler;

typedef std::map<DWORD, IoHandler*>		IOHANDLER_MAP;
typedef std::pair<DWORD, IoHandler*>	IOHANDLER_MAP_PAIR;
typedef IOHANDLER_MAP::iterator			IOHANDLER_MAP_ITER;

//typedef NetworkObject* (*fnCallBackCreateAcceptedObject)();
//typedef VOID (*fnCallBackDestroyAcceptedObject)( NetworkObject *pNetworkObject );
//typedef VOID (*fnCallBackDestroyConnectedObject)( NetworkObject *pNetworkObject );

//-------------------------------------------------------------------------------------------------
/// I/O 핸들러 초기화에 필요한 정보를 담는 구조체
//-------------------------------------------------------------------------------------------------
typedef struct tagIOHANDLER_DESC
{
	DWORD								dwIoHandlerKey;					///< I/O 핸들러 키
	DWORD								dwMaxAcceptSession;				///< 접속받을 최대 세션 수
	DWORD								dwMaxConnectSession;			///< 다른 서버로 접속할 최대 세션 수
	DWORD								dwSendBufferSize;				///< 보내기 버퍼 크기
	DWORD								dwRecvBufferSize;				///< 받기 버퍼 크기
	//DWORD								dwTimeOut;						///< 접속 받은 세션의 타임 아웃 틱
	DWORD								dwMaxPacketSize;				///< 한 패킷의 최대 크기
	DWORD								dwNumberOfIoThreads;			///< I/O 워커 쓰레드 개수
	DWORD								dwNumberOfAcceptThreads;		///< Accept 워커 쓰레드 개수
	DWORD								dwNumberOfConnectThreads;		///< Connect 워커 쓰레드 개수
	//fnCallBackCreateAcceptedObject		fnCreateAcceptedObject;			///< 접속시 NetworkObject 생성 콜백
	//fnCallBackDestroyAcceptedObject		fnDestroyAcceptedObject;		///< 접속 해제시 NetworkObject 소멸 콜백
	//fnCallBackDestroyConnectedObject	fnDestroyConnectedObject;		///< 다른 서버로 접속한 NetworkObject 객체에 대한 소멸 콜백
} IOHANDLER_DESC, *LPIOHANDLER_DESC;

//-------------------------------------------------------------------------------------------------
/// IOCP 서버 클래스
/**
	@remarks
			- IOCP 서버는 다수의 I/O 핸들러를 가질 수 있다.
			  (2개 이상의 I/O 핸들러를 초기화할 경우 LPIOHANDLER_DESC를 배열로 초기화)
			- I/O 핸들러는 하나의 리슨 포트를 가진다.
			- Update를 호출할 때 모든 세션의 받기 버퍼를 검사하여 완성된 패킷을 처리한다.
	@see
			IoHandler
			tagIOHANDLER_DESC
*/
//-------------------------------------------------------------------------------------------------
class IOCPServer
{
	friend unsigned __stdcall send_thread( LPVOID param );

public:
	IOCPServer();
	virtual ~IOCPServer();

	BOOL				Init( LPIOHANDLER_DESC lpDesc, DWORD dwNumberofIoHandlers );
	BOOL				StartListen( DWORD dwIoHandlerKey, char *pIP, WORD wPort );
	BOOL				StartAllListen( char *pIP, WORD wPort );
	BOOL				StartSendProcessThread();
	VOID				Update();
	VOID				Shutdown();
	//DWORD				Connect( DWORD dwIoHandlerKey, NetworkObject *pNetworkObject, char *pszIP, WORD wPort );
	BOOL				IsListening( DWORD dwIoHandlerKey );
	DWORD				GetNumberOfConnections( DWORD dwIoHandlerKey );

	void				SetKernel(CSocketKernel* pKernel);

	int					SetNewClientSocket(Session* pSession);

	int					ProcessData(const char* pRecvBuf,int nSize,Session* pSession);
	void				ProcessDisconnectClient(int i);
//	BOOL				IsNpcID(int i);
	void				ProcessSend();
private:
	BOOL				InitWinsock();
	VOID				CreateIoHandler( LPIOHANDLER_DESC lpDesc );

	HANDLE				m_hSendThread;
	BOOL				m_bShutdown;
	IOHANDLER_MAP		m_mapIoHandlers;

	CSocketKernel*		m_pSocketKernel;
};
