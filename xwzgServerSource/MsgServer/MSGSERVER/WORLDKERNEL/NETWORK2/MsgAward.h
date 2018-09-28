// MsgAward.h: interface for the CMsgAward class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGAWARD_H__22DA781F_8053_4716_BA0D_C86CED590280__INCLUDED_)
#define AFX_MSGAWARD_H__22DA781F_8053_4716_BA0D_C86CED590280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "../share/protocol.h"
typedef struct
{
	int nType;
	int nValue;
}GainAwardInfo;
WORLDKERNEL_BEGIN

enum _GAINAWARD
{
	_GAINAWARD_QUERY = 0,
		_GAINAWARD_INIT,
		_GAINAWARD_UPDATE,
		_GAINAWARD_EXTRA,
		_GAINAWARD_SPECILLINE,
		_GAINAWARD_RESETNAME,
};

class CMsgAward : public CNetMsg  
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
WORLDKERNEL_END
#endif // !defined(AFX_MSGAWARD_H__22DA781F_8053_4716_BA0D_C86CED590280__INCLUDED_)
