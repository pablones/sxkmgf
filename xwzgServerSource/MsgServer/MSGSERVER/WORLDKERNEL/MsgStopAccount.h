// MsgStopAccount.h: interface for the CMsgStopAccount class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSTOPACCOUNT_H__6C0E4E4C_66EE_4FBA_8652_2D37EF50C226__INCLUDED_)
#define AFX_MSGSTOPACCOUNT_H__6C0E4E4C_66EE_4FBA_8652_2D37EF50C226__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
WORLDKERNEL_BEGIN
class CMsgStopAccount : public CNetMsg  
{
public:
	CMsgStopAccount();
	virtual ~CMsgStopAccount();
public:	
	BOOL Create(char* pMsgBuf,DWORD dwSize);
	BOOL Create(USHORT nAction, OBJID playerID, UINT nParam);
	void Process(void *pInfo);
//	BOOL Create(OBJID StopAccountID);
	typedef struct
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		unsigned short	unAciton;

		OBJID		ucStopIDAccount;			// 行为模式
		OBJID		playerID;
		UINT		nParam;
	}MSG_Info;
	MSG_Info* m_pInfo;

};
WORLDKERNEL_END
#endif // !defined(AFX_MSGSTOPACCOUNT_H__6C0E4E4C_66EE_4FBA_8652_2D37EF50C226__INCLUDED_)
