// MsgNewCard.h: interface for the CMsgNewCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNEWCARD_H__FFA7C09A_3FCD_4176_B23C_F10F99CE958C__INCLUDED_)
#define AFX_MSGNEWCARD_H__FFA7C09A_3FCD_4176_B23C_F10F99CE958C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum 
{	
	NEWCARDACT_NONE				= 0, 
	NEWCARDACT_PICK				= 1,
	NEWCARDACT_SET              = 2,

	NEWCARDACT_PICKSUC          = 1001,
	NEWCARDACT_PICKFAIL			= 1002,
};

class CMsgNewCard_w : public CNetMsg     
{
public:
	CMsgNewCard_w();
	virtual ~CMsgNewCard_w();
	
	BOOL	Create(int nType,OBJID idUser,OBJID idAccount, const char* pszCard);
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);
public:
	typedef struct tag_MSG_Info{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		
		DWORD	dwType;
		OBJID   idUser;
		OBJID   idAccount;
		char	szBuf[64];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGNEWCARD_H__FFA7C09A_3FCD_4176_B23C_F10F99CE958C__INCLUDED_)
