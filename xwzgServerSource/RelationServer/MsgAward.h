// MsgAward.h: interface for the CMsgAward class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGAWARD_H__7BB71E43_20EB_4493_B65A_3173FF7F77EA__INCLUDED_)
#define AFX_MSGAWARD_H__7BB71E43_20EB_4493_B65A_3173FF7F77EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "Define.h"

enum _GAINAWARD
{
	_GAINAWARD_QUERY = 0,
	_GAINAWARD_INIT,
	_GAINAWARD_UPDATE,
	_GAINAWARD_EXTRA,
};

class CMsgAward : public CNetMsg  
{
public:
	CMsgAward();
	virtual ~CMsgAward();

	BOOL Create(char* pMsgBuf,DWORD dwSize);

	typedef struct
	{
		unsigned short  	unMsgSize;
		unsigned short   	unMsgType;

		UINT				nAction;
		OBJID               accountID;
		OBJID               idUser;
		char                szServerName[16];
	    UINT                nAmount;
		GainAwardInfo       gaInfo[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGAWARD_H__7BB71E43_20EB_4493_B65A_3173FF7F77EA__INCLUDED_)
