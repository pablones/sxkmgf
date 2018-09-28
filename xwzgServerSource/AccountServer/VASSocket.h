#if !defined(AFX_VASSOCKET_H__572E2819_2DB9_4516_B882_61C15FD968AB__INCLUDED_)
#define AFX_VASSOCKET_H__572E2819_2DB9_4516_B882_61C15FD968AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VASSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CVASSocket command target
#include "AFXSOCK.H"
//#include "VASStringPacket.h"
//#include "VASDBManager.h"
#include "WormSolt.h"

class CSupplyVASService;
///���׽�������������վ�����ĳ�ֵ����
class CVASSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CVASSocket();
	virtual ~CVASSocket();

//	void SetHandleObject(CSupplyVASService* pHandleObject)///���ô������ 
//	{
//		m_pHandleObject=pHandleObject;
//	}
	Singnal_v1				sltRecievePacket;///�źŲ۶���,���ڴ���

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVASSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CVASSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	///�����ֵ��Ϣ
//	BOOL ProcessSupplyVAS(CVASStringPacket& packet,CString& strReplyMsg);
private:
	CSupplyVASService*		m_pHandleObject;///��Ϣ�������
	CString					m_strPeerName;///Զ��IP
	UINT					m_nPeerPort;///Զ�˶˿�
	BYTE					m_cbBuffer[1024];///���ջ�����
	short nPacketSize,nByteRead;///���ߴ�Ͷ�ȡ����
//	CVASStringPacket packcet;///Э�鴦�����
//	CVASDBManager manager;///���ݿ������
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VASSOCKET_H__572E2819_2DB9_4516_B882_61C15FD968AB__INCLUDED_)
