// MsgStopAccount.h: interface for the CMsgStopAccount class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSTOPACCOUNT_H__4C23D2F8_CC5B_4AEB_B1F8_3362621A1D1C__INCLUDED_)
#define AFX_MSGSTOPACCOUNT_H__4C23D2F8_CC5B_4AEB_B1F8_3362621A1D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgStopAccount : public CNetMsg  
{
public:
	CMsgStopAccount();
	virtual ~CMsgStopAccount();
public:	
	BOOL Create(char* pMsgBuf,DWORD dwSize);///���
	BOOL Create(USHORT nAction, UINT nParam = 0, OBJID playerID = 0);///[2009.2.2--�Ŷ�]
	typedef struct
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		unsigned short  unAction;

		OBJID		ucStopIDAccount;			// ��Ϊģʽ
		OBJID		playerID;
		UINT		nParam;		//[2009.2.2--�Ŷ�]��Ϣ����
	}MSG_Info;
	MSG_Info* m_pInfo;

};

#endif // !defined(AFX_MSGSTOPACCOUNT_H__4C23D2F8_CC5B_4AEB_B1F8_3362621A1D1C__INCLUDED_)
