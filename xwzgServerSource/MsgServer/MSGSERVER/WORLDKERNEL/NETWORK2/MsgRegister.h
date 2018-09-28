// MsgRegister.h: interface for the CMsgRegister class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGREGISTER_H__CBFA01B1_4965_4A7E_B412_2CD9D70D19E6__INCLUDED_)
#define AFX_MSGREGISTER_H__CBFA01B1_4965_4A7E_B412_2CD9D70D19E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgRegister : public CNetMsg  
{
public:
	CMsgRegister();
	virtual ~CMsgRegister();

public:	
	BOOL		Create		(char* pMsgBuf, DWORD dwSize);
	BOOL		Create(const char* szName, const char* szPassword, OBJID idAccount, USHORT unLook, USHORT unData);
	void		Process		(void *pInfo);

//protected:
	typedef struct {
		MSGHEAD_DEFINE

// 		USHORT	unMsgSize;
// 		USHORT	unMsgType;
		char	szName[_MAX_NAMESIZE];
		char	szPassword[_MAX_NAMESIZE];
		OBJID	idAccount;
		USHORT	unLook;
		USHORT	unData;	//ְҵ + �Ա�
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGREGISTER_H__CBFA01B1_4965_4A7E_B412_2CD9D70D19E6__INCLUDED_)
