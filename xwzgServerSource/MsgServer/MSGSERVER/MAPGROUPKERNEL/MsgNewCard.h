// MsgNewCard.h: interface for the CMsgNewCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNEWCARD_H__F729C82C_7627_4AD0_9F93_80486DDBD914__INCLUDED_)
#define AFX_MSGNEWCARD_H__F729C82C_7627_4AD0_9F93_80486DDBD914__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum 
{	
	NEWCARDACT_NONE				= 0, 
	NEWCARDACT_PICK				= 1,
	NEWCARDACT_SET              = 2,

	VOTCARD_PICK				= 3,
	VOTCARD_SET					= 4,
	
	VOTCARD2_PICK				= 5,
	VOTCARD2_SET				= 6,	

	NEWCARDACT2_PICK			= 7,
	NEWCARDACT2_SET             = 8,

	VOTCARD3_PICK				= 9,
	VOTCARD3_SET				= 10,
	NEWCARDACT3_PICK             = 16,
	VOTCARD_PICKFAIL			= 999,
	VOTCARD_PICKSUC				= 1000,

	NEWCARDACT_PICKSUC          = 1001,
	NEWCARDACT_PICKFAIL			= 1002,

	VOTCARD2_PICKFAIL			= 1003,
	VOTCARD2_PICKSUC			= 1004,

	NEWCARDACT2_PICKSUC         = 1005,
	NEWCARDACT2_PICKFAIL		= 1006,

	VOTCARD3_PICKFAIL			= 1007,
	VOTCARD3_PICKSUC			= 1008,
	

};

class CMsgNewCard : public CNetMsg     
{
public:
	CMsgNewCard();
	virtual ~CMsgNewCard();

	BOOL	Create(int nType,OBJID idUser,OBJID idAccount, const char* pszCard);
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);
private:
	typedef struct{
		MSGHEAD_DEFINE
		unsigned short	unMsgSize;
		unsigned short	unMsgType;

		DWORD	dwType;
		OBJID   idUser;
		OBJID   idAccount;
		char	szBuf[64];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGNEWCARD_H__F729C82C_7627_4AD0_9F93_80486DDBD914__INCLUDED_)
