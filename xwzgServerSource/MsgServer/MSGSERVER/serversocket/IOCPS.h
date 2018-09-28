// IOCPS.h: interface for the IOCPS class. V 1.15
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCPS_H__4D63F25E_B852_46D7_9A42_CF060F5E544D__INCLUDED_)
#define AFX_IOCPS_H__4D63F25E_B852_46D7_9A42_CF060F5E544D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
数据格式short，short，data，即大小，类型，数据
*/

// Determines the size of the first bytes who tells you how big the message are. (pakage heap)  
#define MINIMUMPACKAGESIZE sizeof(short)
#define MAXIMUMPACKAGESIZE 1024
#define MAXIMUMSEQUENSENUMBER 5001
#define MAXIMUMPAYLOADSIZE MAXIMUMPACKAGESIZE-MINIMUMPACKAGESIZE

#define MAXSENDBUFFERSIZE	8*MAXIMUMPACKAGESIZE

#define IOCPSERVERVERSION "IOCP Server/Client system"



// Winsock 2. Works only under Win XP and Win NT2k. 
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "mswsock.h"

#include "IOCPBuffer.h"

#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>
using namespace std;

#include <assert.h>
#include "EncryptServer.h"
#include "SSocket.h"
#include "IOCPMemPool.h"



class CSocketKernel;

class CIOCPBuffer;
struct ClientContext;


typedef map<unsigned int,ClientContext* > ContextMap;
typedef map<unsigned int,CIOCPBuffer* > BufferMap;


/*
* Type of operations. 
*
*/
enum IOType 
{
	IOInitialize, // The client just connected
	IORead, // Read from the client. 
	IOReadCompleted, // Read completed
	IOWrite, // Write to the Client
	IOWriteCompleted, // Write Completed.
	IOZeroByteRead, // Read zero Byte from client (dummy for avoiding The System Blocking error) 
	IOZeroReadCompleted, // Read Zero Byte  completed. (se IOZeroByteRead)
	IOTransmitFileCompleted, //TransmitFileCompleted.
	IOPostedPackage, // Used to post Packages into IOCP port. 
	IOAccept
};



/*
* This struct is used to past around some information about the 
* client. 
*
*/
struct ClientContext
{
	SOCKET				m_Socket; // The Connection socket. 
	CRITICAL_SECTION	m_ContextLock; // The lock used to update and read variabels. 
	unsigned int		m_ID; // Reserved for DisconnectClient if needed. 
	int					m_nNumberOfPendlingIO; // Very Important variable used with ReleaseClientContext. (Avoids Access Violation)
	int								m_ClientID;		//记录ID
	
//	void	ChangeCode(DWORD dwData) { m_cEncryptRecv.ChangeCode(dwData); }

	
//	typedef	CEncryptServer<LOGIN_KEY1, LOGIN_KEY2>	EncryptServer;
//	EncryptServer	m_cEncryptRecv, m_cEncryptSend;

	// Package Overlapped Buffer..
	// Used to get a complete package when we have several pending reads. 
	CIOCPBuffer* m_pBuffOverlappedPackage;	

	ClientContext(){InitializeCriticalSection(&m_ContextLock);}
	~ClientContext(){DeleteCriticalSection(&m_ContextLock);}
};

class IOCPS  
{

private:
	// Aborts A socket without removing it from contextmap.	
	inline void AbortiveClose(ClientContext *mp);
	// Adds nSize bytes to buffer and flush the other buffer. 
	inline BOOL AddAndFlush(CIOCPBuffer *pFromBuff, CIOCPBuffer *pToBuff, UINT nSize);
	// Add a client Context to hashMap,.
	inline BOOL AddClientContext(ClientContext* mp);
	// Allocates a ClientContext and return a pointer ot it. 
	inline ClientContext* AllocateContext();
	
	// Used to bin sockets to Completionport. 
	inline BOOL AssociateSocketWithCompletionPort(SOCKET socket, HANDLE hCompletionPort, DWORD dwCompletionKey);
	// Makes tha last peperation for an connection so IOWORKER can start to work with it. 
	inline BOOL AssociateIncomingClientWithContext(/*SOCKET clientSocket*/CIOCPBuffer* pBuff,DWORD dwSize);
	
	// Creates a CreateCompletionPort
	inline BOOL CreateCompletionPort();
	// Functions used to post request into IOCP (simulate received packages)
	BOOL PostPackage(ClientContext *pContext,CIOCPBuffer *pOverlapBuff);

	// Clears the memory of the ClientContext (Also disconnects) 
	inline void FreeClientContext();
	
	// Used to avoid access violation..
	inline int ExitIOLoop(ClientContext *pContext);
	// Used to avoid access violation..
	inline void EnterIOLoop(ClientContext *pContext);
	
	// Used by IO Workers. 
	inline void OnWriteCompleted(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff);
	inline void OnWrite(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff);
	inline void OnReadCompleted(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff=NULL);
	inline void OnRead(ClientContext *pContext,CIOCPBuffer *pOverlapBuff=NULL);
	inline void OnInitialize(ClientContext* pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff=NULL);
	inline void OnPostedPackage(ClientContext *pContext,CIOCPBuffer *pOverlapBuff);


	//dispatch process fuction
	void ProcessReadCompleted(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff=NULL);
	//write io process fuction
	void ProcessWriteIO(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff=NULL);
	void ProcessWriteIOComplete(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff=NULL);
	
	// Process the internal messages. 
	inline void ProcessIOMessage(CIOCPBuffer *pOverlapBuff, ClientContext* pContext, DWORD dwSize);
	void ProcessDispatchMessage(CIOCPBuffer *pOverlapBuff, ClientContext* pContext, DWORD dwSize);
	void ProcessWriteMessage(CIOCPBuffer *pOverlapBuff, ClientContext* pContext, DWORD dwSize);
	// Process received Packages 
	inline void ProcessPackage(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff);
	
	// Deletes the ClientContext or just put it in the FreeClientContext list. 
	inline BOOL ReleaseClientContext(ClientContext *pContext);
	// Release buffers. 
	inline void ReleaseBufferMap(BufferMap *buffmap);
	void ReleaseSendBufferMap(BufferMap *buffmap);
	
	// Closes The IO Workers
	inline void ShutDownIOWorkers();
	void ShutDownDispatchThread();
	void ShutDownWriteThread();
	//  Splits a buffer into two. Used to handle halffinished received messages. 
	inline CIOCPBuffer * SplitBuffer(CIOCPBuffer * pBuff,UINT nSize);
	
	// Worker, Listener, IOWorker.  
	
	static DWORD WINAPI IOWorkerThreadProc(LPVOID pParam);
	static DWORD WINAPI ListnerThreadProc(LPVOID pParam);
	static DWORD WINAPI DispatchThreadProc(LPVOID pParam);
	static DWORD WINAPI WriteThreadProc(LPVOID pParam);
	//Signals No more Accepting Connections.. 
	BOOL m_bServerStarted;

	//Signals ShutDown
	volatile BOOL m_bShutDown;
	//Signals No more Accepting Connections.. 
	volatile BOOL m_bAcceptConnections;

	HANDLE					m_hCompletionPort;		//IOPort
	HANDLE					m_hDispatchPort;		//DispatchPort
	HANDLE					m_hWritePort;			//WritePort
	
	HANDLE					m_hEvent; 
	HANDLE					m_hAcpEvent;
	
	SOCKET					m_socListen;
	ClientContext*			m_pListenContext;
	

	//CWinThread* m_pListenThread;
	HANDLE	m_ListenThreadHandle;
	// IO Worker Thread list. 
	list<HANDLE> m_IOWorkerList;

	

	//socket缓冲池，供AcceptEx使用的缓冲socket句柄,此缓冲池是listen thread独享的，没必要对他使用临界区
	
	list<SOCKET>	m_FreeSocket;
	
	SOCKET AllocAcceptSocket();
	void BatchCreateSocket(int ncounts);

	int m_InitAcceptNum;		//预先分配的socket数量

	// Maximum number of buffer which is not used. 
	int m_iMaxNumberOfFreeBuffer;

	// Maximum number of Contexts who is in the FreeContextList
	int m_iMaxNumberOfFreeContext;
	

	//预分配内存池
	CIOCPMemPool<CIOCPBuffer>		m_BuffMemPool;
	CIOCPMemPool<ClientContext>		m_ContextMemPool;
	CIOCPMemPool<CIOCPBuffer>		m_SendClientMemPool;
//	CIOCPMemPool<CIOCPBuffer>		m_SendNpcMemPool;

	
	BOOL SetupIOWorkers();
	BOOL SetupDispatch();
	BOOL SetupWriteProcess();
	// Starts The Connection Listner Thread. 
	BOOL SetupListner();

	// Contains the result of winsock init. 
	int m_iWSAInitResult;

	// Number of IOWorkers running..
	int m_nIOWorkers;
	// Number of IOWorker. 
	int m_iMaxIOWorkers;
	// Maximum Number of Connections. 
	int m_iMaxNumConnections;

	// Port number. 
	int m_nPortNumber; 

	// Number of Workers running intitially. 
	int m_nOfWorkers;

	
	
	// Number of Pendling Reads, used for performance
	int m_iNumberOfPendlingReads;
	// When Set to TRUE Reads are processed In Order.. 
	//BOOL m_bReadInOrder;
	// Make the sends in order. 
	//BOOL m_bSendInOrder;
	
	CSocketKernel*	m_pKernel;

	static GUID g_GUIDAcceptEx;
	LPFN_ACCEPTEX lpAcceptEx;

	BOOL PostAcceptEx(SOCKET ListenSocket,int nNumSockets);
	void ProcessAcceptExData(ClientContext* pContext,DWORD dwSize,CIOCPBuffer* pOverlapBuff);

public: 

	enum
	{
		SENDCLIENT = 1,
//		SENDNPC
	};
	// Functions used to post request into IOCP (simulate received packages)
	BOOL PostPackage(int iClientId, CIOCPBuffer *pOverlapBuff);
	BOOL PostPackage(ClientContext* pContext,const char* pBuff,int nSize,UINT nAllocType);
	
	// Returns TRUE if The server/client are started. 
	BOOL IsStarted();
	int GetNumberOfConnections();
	// Returns the Ip Adress of a Remote hoste given the Socket, 
	string GetHostAddress(SOCKET socket);
	// Send the Data in pBuff to all the clients. 
	BOOL ASendToAll(CIOCPBuffer *pBuff);
	
	// Disconnects A client. 
	inline void DisconnectClient(ClientContext* pContext, BOOL bGraceful=FALSE);
	
	// Return the computers IP Address. 
	string GetHostIP();
	// Starts the server, 
	BOOL Start(int nPort=999,int iMaxNumConnections=2000,int iMaxIOWorkers=1,int nOfWorkers=0,int iMaxNumberOfFreeBuffer=800,int iMaxNumberOfFreeContext=550,BOOL bOrderedSend=TRUE, BOOL bOrderedRead=TRUE,int iNumberOfPendlingReads=3,int iInitAcceptNum = 1024);

	IOCPS();
	virtual ~IOCPS();


	void SetKernel(CSocketKernel* pKernel);

	void ChangeCode(ClientContext* pClient,DWORD dwData);
	


	

	// Disconnects all the clients. 
	void DisconnectAll();

	BOOL ASend(int ClientId,CIOCPBuffer *pOverlapBuff);



	// Finds a clien in the Client context Hashmap (NOT THREAD SAFE)..
	ClientContext* FindClient(unsigned iClient);

	// ShutDowns The Server. 
	void ShutDown();

	// Starts the server. 
	BOOL Startup();

	// We put all the Context (Open connections) into this String2Pointer HashMap. 
	CRITICAL_SECTION m_ContextMapLock;
	ContextMap m_ContextMap;

	// return the Key (The hostnamn+ socketnr) of a socket. (must be Unique). 
	string GetHostKeyName(SOCKET socket);	
protected:
	volatile int m_NumberOfActiveConnections;
	// Called when a new connection have been established.. 
	virtual void NotifyNewConnection(ClientContext *pcontext);
	// Called when a empty ClientContext structure are allocated. 
	virtual void NotifyNewClientContext(ClientContext *pContext);
	// A client have Disconnected. 
	virtual void NotifyDisconnectedClient(ClientContext *pContext);
	// File send/Transefer Completed. 
	virtual void NotifyFileCompleted(ClientContext *pcontext);
	// A Package have arrived. 
	virtual inline void NotifyReceivedPackage(CIOCPBuffer *pOverlapBuff,int nSize,ClientContext *pContext);
	// An ClientContext is going to be deleted insert more cleanup code if nesseary.  
	virtual void NotifyContextRelease(ClientContext *pContext);
	// An Write have been Completed
	virtual inline void NotifyWriteCompleted(ClientContext *pContext, DWORD dwIoSize,CIOCPBuffer *pOverlapBuff);
	// Used for log
	virtual void AppendLog(string msg);
	// Do a Asyncorn Send. Never call this function outside of Notifyxxxx(...) functions. 
	BOOL ASend(ClientContext *pContext,CIOCPBuffer *pOverlapBuff);

	// deletes the buffer or just put it in the FreeBufferList to optimze performance. 
	BOOL ReleaseBuffer(CIOCPBuffer *pBuff);

	// Creates a new buffer or returns a buffer from the FreeBufferList and configure the buffer. 
	CIOCPBuffer* AllocateBuffer(int nType);


	//专门分配给NPC和CLIENT的buffer

	CIOCPBuffer* AllocateClientNpcBuffer(int nType,UINT AllocType);
	BOOL ReleaseClientNpcBuffer(CIOCPBuffer* pBuffer);

	// Error Convertion.. 
	string ErrorCode2Text(DWORD dw);

};
#endif // !defined(AFX_MYIOCPSERVER_H__4D63F25E_B852_46D7_9A42_CF060F5E544D__INCLUDED_)
