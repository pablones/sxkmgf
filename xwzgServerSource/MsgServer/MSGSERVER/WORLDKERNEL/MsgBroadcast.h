// MsgBroadcast.h: interface for the CMsgBroadcast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGBROADCAST_H__D877597B_9780_4B3C_B2B5_106436A70787__INCLUDED_)
#define AFX_MSGBROADCAST_H__D877597B_9780_4B3C_B2B5_106436A70787__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK2\NetMsg.h"

WORLDKERNEL_BEGIN
class CMsgBroadcast : public CNetMsg  
{
public:
	CMsgBroadcast();
	virtual ~CMsgBroadcast();
	BOOL			Create		(char* pbufMsg, DWORD dwMsgSize);
	void			Process		(void *pInfo);

	BOOL			Create		(/*USHORT uType, */const char* wTime, UINT uProfession, UINT downLevel,
								UINT upLevel, /*UINT uTimes, UINT uInterval,*/const char* str);

private:
	typedef struct {
		MSGHEAD_DEFINE

		//USHORT uType;
		UINT uProfession;
		UINT downLevel;
		UINT upLevel;
		//UINT uTimes;
		//UINT uInterval;
		char wTime[16];
		char strContent[512];

	}MSG_Info;

	MSG_Info*	m_pInfo;
};
WORLDKERNEL_END

#endif // !defined(AFX_MSGBROADCAST_H__D877597B_9780_4B3C_B2B5_106436A70787__INCLUDED_)
