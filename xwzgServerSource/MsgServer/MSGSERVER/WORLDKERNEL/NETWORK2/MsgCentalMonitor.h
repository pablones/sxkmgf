// MsgCentalMonitor.h: interface for the CMsgCentralMonitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCENTALMONITOR_H__F8B962C0_6E21_4D27_B498_C2C469328790__INCLUDED_)
#define AFX_MSGCENTALMONITOR_H__F8B962C0_6E21_4D27_B498_C2C469328790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum
{
	MONITOR_SHUTDONW,
	MONITOR_LOG1,
	MONITOR_LOG2,
	MONITOR_LOG3,	
};

const	DWORD MAX_STRING_SIZE_PER_MSG = 900;

using namespace world_kernel;
class CMsgCentralMonitor : public CNetMsg  
{
public:
	BOOL Create(char* pszServerName, char* pszServerIP, int nType, int nSerialNumber, BOOL bLastOne, char* pszTrace, int nLength);
	CMsgCentralMonitor();
	virtual ~CMsgCentralMonitor();
	virtual void			Process		(void *pInfo);
	typedef struct {
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		char			szServerName[_MAX_NAMESIZE];
		char			szServerIP[_MAX_NAMESIZE];
		int				nTraceType;
		int				nSerialNumber;
		BOOL			bLastOne;	//	是否为最后一个
		int				nTextSize;	//	字符串长度
		char			szText[1];		
	}MSG_Info;
	
	MSG_Info* m_pInfo;

};

#endif // !defined(AFX_MSGCENTALMONITOR_H__F8B962C0_6E21_4D27_B498_C2C469328790__INCLUDED_)
