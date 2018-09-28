// MsgAward.h: interface for the CMsgAward class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGAWARD_H__03C9D895_53B9_411D_A779_73962752FA90__INCLUDED_)
#define AFX_MSGAWARD_H__03C9D895_53B9_411D_A779_73962752FA90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"
#include "../share/protocol.h"
enum _GAINAWARD
{
	_GAINAWARD_QUERY = 0,
	_GAINAWARD_INIT,
	_GAINAWARD_UPDATE,
	_GAINAWARD_EXTRA,
	_GAINAWARD_SPECILLINE,
	_GAINAWARD_RESETNAME,
};
typedef struct
	{
	int a;
}GainAwardInfo;
class CMsgAward :public CNetMsg  
{
public:
	CMsgAward();
	virtual ~CMsgAward();

	virtual BOOL Create		(char* pMsgBuf, DWORD dwSize);
	virtual void Process	(void *pInfo);

	BOOL Create(UINT nAction, OBJID accountID, OBJID idUser, const char* szServerName, const GainAwardInfo* gainInfo);

	typedef struct
	{
		unsigned short	    unMsgSize;
		unsigned short	    unMsgType;
		
        UINT				nAction;
		OBJID               accountID;
		OBJID               idUser;
		char                szServerName[16];
	    UINT                nAmount;
		GainAwardInfo       gaInfo[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGAWARD_H__03C9D895_53B9_411D_A779_73962752FA90__INCLUDED_)
