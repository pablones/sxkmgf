// ServerSocket.h
// paled, 2002.6.2

// ��������SOCKET���ӵķ����һ������������С��Ϸ������LISTEN�ͻ��˵�¼��Ҳ�������������ݷ�������
// ʹ�÷�������ʽʵ�֡�


#ifndef	SSOCKET_H
#define	SSOCKET_H


////////////////////////////////////////////////////////////////////////////////////////////
#include "Common.h"				// ���ú�����

#define	PURE_VIRTUAL_DECLARE		PURE_VIRTUAL_FUNCTION		//???
#define	PURE_VIRTUAL_DECLARE_0		PURE_VIRTUAL_FUNCTION_0		//???

#define LOGIN_KEY1				0xa61fce5e	// A = 0x20, B = 0xFD, C = 0x07, first = 0x1F, key = a61fce5e
#define LOGIN_KEY2				0x443ffc04	// A = 0x7A, B = 0xCF, C = 0xE5, first = 0x3F, key = 443ffc04

////////////////////////////////////////////////////////////////////////////////////////////

const int		RECV_BUFFER_SIZE		= 4096;		// ���ջ������ߴ�
const int		SEND_BUFFER_SIZE		= 1024 * 1024;//256 * 1024;// ���ͻ������ߴ�
// SOCKET�ĳ�ʼ�����ս�(ע�������ʹ�á�)
// ʵ�ַ�ʽΪ����ʼ�������WINSOCK2.0���DLL��
bool	ServerSocketInit();			// �൱��WSAStartup()
bool	ServerSocketFinal();		// �൱��WSACleanup()

typedef	char* CPacket;

class	IListenSocket
{
public:
	// ��ʼ������SOCKET�����ظ����ã��Լ�����һ���˿ڡ�
	// nPort:	�����Ķ˿�
	// return:	�ɹ�����ture
	virtual bool Listen(int nPort, int nSndBuf = 0, int nRcvBuf = 0)	PURE_VIRTUAL_DECLARE_0

	// ����һ�������ӵ�SOCKET��
	// SOCKET:		�����ӵ�SOCKET�����������򷵻�INVALID_SOCKET�������ú����������ء�
	virtual SOCKET		Accept()	PURE_VIRTUAL_DECLARE_0

	// �رռ���SOCKET�����ظ����á�
	virtual void		Close()		PURE_VIRTUAL_DECLARE

	// ����SOCKET���Ա���SELECT()
	virtual SOCKET		Socket()	PURE_VIRTUAL_DECLARE_0
};

class	CListenSocket : IListenSocket
{
public:
	IListenSocket*	GetInterface() { return (IListenSocket*)this; }
public:
	// nSndBuf:	ϵͳ����BUFFER�ĳ��ȡ�(ע������ָӦ�ó����BUFFER)
	// nRcvBuf:	ϵͳ����BUFFER�ĳ��ȡ�(ע������ָӦ�ó����BUFFER)
	CListenSocket();
	~CListenSocket() { Close(); }
protected: // interface
	virtual bool		Listen(int nPort, int nSndBuf = 0, int nRcvBuf = 0);		// 0 : ���޸�ϵͳ��ȱʡֵ��
	virtual SOCKET		Accept();
	virtual void		Close();
	virtual SOCKET		Socket() { return m_sock; }

////////////////////////////////////////////////
// �ڲ����� 
protected:
	SOCKET		m_sock;
};


class	IServerSocket
{
public:
	// ȡ��һ����Ϣ��������رջ����Ҫʹ��IsOpen()��顣
	virtual const char*	GetPacket	(int* nLen, BOOL bFromNet = true)				PURE_VIRTUAL_DECLARE_0
	// ȡ��һ����Ϣ���󣬱�����ô˺��������Ϣ����
	virtual bool		ClearPacket	(int nLen)				PURE_VIRTUAL_DECLARE_0
	// ����һ����Ϣ���������ϣ��������ͣ�����Ӧ�ó����л��塣
	// return false: ������������رա�
	virtual bool		SendPacket	(const char* pack, int nLen)	PURE_VIRTUAL_DECLARE_0
	// ��SOCKET�Ƿ��ѹر�
	virtual bool		IsOpen		()						PURE_VIRTUAL_DECLARE_0
	// �ر�����SOCKET�����ظ�����
	// bNoLinger: ��������SOCKET��Դ��(ͨ������SOCKET�쳣�ر�)
	virtual void		Close		(BOOL bNoLinger = false)	PURE_VIRTUAL_DECLARE		
	// ȡ�öԷ���SOCKET��
	// return: �������ֽ�˳���ʾ��
	virtual DWORD		GetPeerIP	()						PURE_VIRTUAL_DECLARE_0

	// ����SOCKET���Ա���SELECT()
	virtual SOCKET		Socket()							PURE_VIRTUAL_DECLARE_0

	// �޸�һ������SOCKET�ģ� 0: ���޸�
	virtual int 		SetBufSize(int nSendBuf, int nRecvBuf=0)			PURE_VIRTUAL_DECLARE_0

	// �޸Ķ�̬����
	virtual void		ChangeCode(DWORD dwData)			PURE_VIRTUAL_DECLARE
	//07.5.21�������,�������ݵ�������
	virtual int			SendPacket2Net()					PURE_VIRTUAL_DECLARE_0
	virtual int			GetSendLeaveLen()					PURE_VIRTUAL_DECLARE_0
	virtual int			GetRecvBufferLen()					PURE_VIRTUAL_DECLARE_0
	virtual const char* GetRecvBuffer()						PURE_VIRTUAL_DECLARE_0	
	//  [7/11/2007]�������
	virtual bool		CopyDataToBuffer( const char* pSource,int nLen )	PURE_VIRTUAL_DECLARE_0	
};
class	CServerSocket : IServerSocket
{
public:
	IServerSocket*	GetInterface() { return (IServerSocket*)this; }
public: // static
	// �Ӷ��з���һ���󣬲���ʼ����
	// sock:	�����׽���
	// GetPacketLen:	����ָ�룬��ȡ����Ϣ���ĳ��ȡ�
	static CServerSocket*	CreateNew(SOCKET sock);

	// �߻���CreateNew�����Ķ���
	static void				Destroy(CServerSocket* pSocket);

protected: // interface
	virtual const char*	GetPacket	(int* pLen, BOOL bFromNet /*= true*/);
	virtual bool		ClearPacket	(int nLen);
	virtual bool		SendPacket	(const char* pack, int nLen);
	virtual bool		IsOpen		(){if(m_sock == INVALID_SOCKET) return false;return true;}
	virtual	void		Close		(BOOL bNoLinger = false);
	virtual DWORD		GetPeerIP	();
	virtual SOCKET		Socket() { return m_sock; }
	virtual int			SetBufSize	(int nSendBuf, int nRecvBuf);

	virtual int			SendPacket2Net();//07.5.21�������
	virtual int			GetSendLeaveLen();

	bool				GetPeerStrIP( char* szClientIP );//  [6/1/2007]�������

	virtual int			GetRecvBufferLen()	{ return m_nLen;	}				
	virtual const char* GetRecvBuffer()		{ return m_bufMsg;	}
	
	virtual bool		CopyDataToBuffer( const char* pSource,int nLen );	//  [7/11/2007]�������						


	int	ProcessSendFail( int nRet )//  [7/11/2007] �������ʧ�ܴ���
	{		
		if( 0 == nRet )
		{
			LOGERROR( "ERROR,SendPacket2Net send ret 0,�ͻ��˹ر���" );
			Close( true );
		}
		else
		{
			int nErrCode = WSAGetLastError();
			if( nErrCode != WSAEWOULDBLOCK )
			{
				if( nErrCode != WSAECONNRESET )
				{
					LOGERROR( "ERROR,SendPacket2Net,�����쳣,socket�ر�,ErrCode:%d",nErrCode );
				}
				Close( true );
			}
		}
		return 0;
	}
// �ڲ����� ////////////////////////////////////////////////
public:
	//20070302����:���� m_bufMsg ��ʼ��
	CServerSocket(SOCKET sock);
	~CServerSocket();
protected:
	SOCKET		m_sock;
	char		m_bufMsg[RECV_BUFFER_SIZE];
	int			m_nLen;
//----------------07.5.21�������-----------------------
	char		m_sendBuffer[ SEND_BUFFER_SIZE ];
	int			m_nLeaveLen;//���ͻ��������ж�������
//----------------end-------------------------------------
//  [7/11/2007]----�������-----------------------------
	int			m_nHeadPos;		//���ͻ�����ͷָ��
	int			m_nTrailPos;	//���ͻ�����βָ��	
//--------------------------------------------------------	

//#ifdef	ENCRYPT	
// public:
// 	void	ChangeCode(DWORD dwData) { m_cEncryptRecv.ChangeCode(dwData); }
// protected:
// 	typedef	CEncryptServer<LOGIN_KEY1, LOGIN_KEY2>	EncryptServer;
// 	EncryptServer	m_cEncryptRecv, m_cEncryptSend;
//#endif
};


#endif // SSOCKET_H