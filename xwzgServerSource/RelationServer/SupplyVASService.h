// SupplyVASService.h: interface for the CSupplyVASService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPPLYVASSERVICE_H__C3816C0B_D974_4367_9538_D58E9AFACAD9__INCLUDED_)
#define AFX_SUPPLYVASSERVICE_H__C3816C0B_D974_4367_9538_D58E9AFACAD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "VASSocket.h"
#include "vector"
#include "AFXMT.H"
#include "VASStringPacket.h"
#include "VASDBManager.h"

using namespace std;

//struct Section//ͬ������
//{
//	Section()
//	{
//		InitializeCriticalSection(&CriticalSection);//��ʼ���߳�ͬ������
//		::EnterCriticalSection(&CriticalSection);//lock		
//	}
//	~Section()
//	{
//		LeaveCriticalSection(&CriticalSection);//unlock
//		DeleteCriticalSection(&CriticalSection);		
//	}
//	CRITICAL_SECTION CriticalSection;
//};

//struct VASSupply
//{
//	string strMsg;
//	string strServerName;
//	UINT    nServerPort;
//	VASSupply()
//	{
//		nServerPort=0;
//	}
//	VASSupply(const CString& msg,const CString& ServerName,UINT ServerPort)
//	{
//		strMsg=msg;
//		strServerName=ServerName;
//		nServerPort=ServerPort;
//	}
//};

struct MSG_VASSupply_INFO
{
	struct dummy
	{
		WORD	MSG_SIZE;///������,���65535�ֽ�,һ��̶���1K���ڡ���2�ֽ�
		BYTE	MSG_TYPE;///�����͡���1�ֽ�
		WORD	WebServerID;///��վ��ʶ��,����,Ϊ0����2�ֽ�
		BYTE    Type;///��ֵ����,����Ϊ0����1�ֽ�
		DWORD	Supply_VAS_Point;///��ֵ��������4�ֽ�
	};
	dummy   info;
	char	comboString[_MAX_NAMESIZE*2+2];///��Ϸ����������;�ʺŵ�¼��
};

struct vas_ProcessContext
{
	string				strServerName;///����IP
	UINT				nServerPort;///�����˿�
	MSG_VASSupply_INFO	context;///����
};

enum VASMSGTYPE
{
	vmtSupplyRequest,///��ֵ����
	vmtSupplyReply///��ֵ�ܴ�
};

class CSupplyVASService  
{
public:
	CSupplyVASService();
	virtual ~CSupplyVASService();

	BOOL StartService(UINT nPort=3468,UINT nInitBufferSize=2048);///��ʼ����,���׽���,�������߳�
	BOOL StopService();///���׽���,���������߳�

	///�����������
	BOOL Add(const vas_ProcessContext& vasMessage);///���һ����Ϣ����������
//	BOOL Get(CString& strMessage);///�õ�������һ����Ϣ,��ɺ����Ӷ�����ɾ��
	BOOL Clear();///��ն���
	///�����������
	
	typedef Section SupplyVASLock;

///�ڲ����õĺ���
protected:
	static UINT ProcessFn(LPVOID pParam);///�̺߳���,������еķ�������
	BOOL		ProcessMsg(vas_ProcessContext& objVAS);///����һ����Ϣ����
	BOOL		ProcessSupplyVAS(MSG_VASSupply_INFO& objContext);///������ֵ����

	void		ProcessPacket(const ParamList& paramlist);///�źŲ�֪ͨ����
private:
	CVASSocket			m_skVasSocket;///�����׽���,���ڽ�����վ�������ĳ�ֵ���ѯ��Ϣ
//	CVASStringPacket	m_packet;///Э�鴦�����
	CVASDBManager		m_manager;///���ݿ������,���ڶ����ݿ���г�ֵ�Ͳ�ѯ����
	CWinThread*			m_pWorkThread;///�����߳�,���ڴӻ��������ȡ�����ݽ��д���
	vector<vas_ProcessContext>	m_bufferlist;///�������,���ڴ洢�׽����յ�������
	CEvent				m_etWorkStop;///�߳������¼�,ͨ�������ر��߳�
};

#endif // !defined(AFX_SUPPLYVASSERVICE_H__C3816C0B_D974_4367_9538_D58E9AFACAD9__INCLUDED_)
