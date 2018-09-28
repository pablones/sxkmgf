// MsgCompensationMsg.h: interface for the CMsgCompensationMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCOMPENSATIONMSG_H__7CE11BDF_86C1_4AC8_8CED_3D03FFFABE42__INCLUDED_)
#define AFX_MSGCOMPENSATIONMSG_H__7CE11BDF_86C1_4AC8_8CED_3D03FFFABE42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"
#include "NetStringPacker.h"

class CMsgCompensationMsg : public CNetMsg  
{
public:
	CMsgCompensationMsg();
	virtual ~CMsgCompensationMsg();

	BOOL	Create		(USHORT nAction,char* szWord);
public:	
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);
	
	
	typedef struct {
		MSGHEAD_DEFINE		
		USHORT		usAction;
		char		szBuf[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
private:
	CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGCOMPENSATIONMSG_H__7CE11BDF_86C1_4AC8_8CED_3D03FFFABE42__INCLUDED_)
