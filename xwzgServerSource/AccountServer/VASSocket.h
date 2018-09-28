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
///该套接字用于侦听网站发来的充值请求
class CVASSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CVASSocket();
	virtual ~CVASSocket();

//	void SetHandleObject(CSupplyVASService* pHandleObject)///设置处理对象 
//	{
//		m_pHandleObject=pHandleObject;
//	}
	Singnal_v1				sltRecievePacket;///信号槽对象,用于触发

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
	///处理充值消息
//	BOOL ProcessSupplyVAS(CVASStringPacket& packet,CString& strReplyMsg);
private:
	CSupplyVASService*		m_pHandleObject;///消息处理对象
	CString					m_strPeerName;///远端IP
	UINT					m_nPeerPort;///远端端口
	BYTE					m_cbBuffer[1024];///接收缓冲区
	short nPacketSize,nByteRead;///包尺寸和读取长度
//	CVASStringPacket packcet;///协议处理对象
//	CVASDBManager manager;///数据库管理器
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VASSOCKET_H__572E2819_2DB9_4516_B882_61C15FD968AB__INCLUDED_)
