// MsgStopAccount.h: interface for the CMsgStopAccount class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSTOPACCOUNT_H__521ABAF8_8B8C_4DE2_A0CD_4532AAC306C7__INCLUDED_)
#define AFX_MSGSTOPACCOUNT_H__521ABAF8_8B8C_4DE2_A0CD_4532AAC306C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"

class CMsgStopAccount : public CNetMsg  
{
public:
	enum{_ACTION_NONE=0, _ACTION_ISSTOPACCOUNT=1,};
public:
	CMsgStopAccount();
	virtual ~CMsgStopAccount();
	BOOL Create(char* pMsgBuf,DWORD dwSize);
	BOOL Create(OBJID StopAccountID, OBJID playerID=0, USHORT nAction=0);
	void Process(void *pInfo);
	typedef struct
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		unsigned short	unAction;

		OBJID		ucStopIDAccount;			// ��Ϊģʽ
		OBJID		playerID;
		UINT		nParam;		//[2009.2.2--�Ŷ�]��Ϣ����
	}MSG_Info;
	MSG_Info* m_pInfo;


};

#endif // !defined(AFX_MSGSTOPACCOUNT_H__521ABAF8_8B8C_4DE2_A0CD_4532AAC306C7__INCLUDED_)
