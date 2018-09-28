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
/// I/O �ڵ鷯 �ʱ�ȭ�� �ʿ��� ������ ��� ����ü
//-------------------------------------------------------------------------------------------------
typedef struct tagIOHANDLER_DESC
{
	DWORD								dwIoHandlerKey;					///< I/O �ڵ鷯 Ű
	DWORD								dwMaxAcceptSession;				///< ���ӹ��� �ִ� ���� ��
	DWORD								dwMaxConnectSession;			///< �ٸ� ������ ������ �ִ� ���� ��
	DWORD								dwSendBufferSize;				///< ������ ���� ũ��
	DWORD								dwRecvBufferSize;				///< �ޱ� ���� ũ��
	//DWORD								dwTimeOut;						///< ���� ���� ������ Ÿ�� �ƿ� ƽ
	DWORD								dwMaxPacketSize;				///< �� ��Ŷ�� �ִ� ũ��
	DWORD								dwNumberOfIoThreads;			///< I/O ��Ŀ ������ ����
	DWORD								dwNumberOfAcceptThreads;		///< Accept ��Ŀ ������ ����
	DWORD								dwNumberOfConnectThreads;		///< Connect ��Ŀ ������ ����
	//fnCallBackCreateAcceptedObject		fnCreateAcceptedObject;			///< ���ӽ� NetworkObject ���� �ݹ�
	//fnCallBackDestroyAcceptedObject		fnDestroyAcceptedObject;		///< ���� ������ NetworkObject �Ҹ� �ݹ�
	//fnCallBackDestroyConnectedObject	fnDestroyConnectedObject;		///< �ٸ� ������ ������ NetworkObject ��ü�� ���� �Ҹ� �ݹ�
} IOHANDLER_DESC, *LPIOHANDLER_DESC;

//-------------------------------------------------------------------------------------------------
/// IOCP ���� Ŭ����
/**
	@remarks
			- IOCP ������ �ټ��� I/O �ڵ鷯�� ���� �� �ִ�.
			  (2�� �̻��� I/O �ڵ鷯�� �ʱ�ȭ�� ��� LPIOHANDLER_DESC�� �迭�� �ʱ�ȭ)
			- I/O �ڵ鷯�� �ϳ��� ���� ��Ʈ�� ������.
			- Update�� ȣ���� �� ��� ������ �ޱ� ���۸� �˻��Ͽ� �ϼ��� ��Ŷ�� ó���Ѵ�.
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
