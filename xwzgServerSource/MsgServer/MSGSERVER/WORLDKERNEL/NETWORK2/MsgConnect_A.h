// MsgConnect.h: interface for the CMsgConnect_A class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCONNECT_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_A)
#define AFX_MSGCONNECT_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_A

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgConnect_A : public CNetMsg  
{
public:
	CMsgConnect_A();
	virtual ~CMsgConnect_A();

	BOOL	Create	(OBJID idAccount, DWORD dwData, char* pszInfo);

public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pSocket);

//protected:
// 	typedef struct {
// 		unsigned short	unMsgSize;
// 		unsigned short	unMsgType;
// 
// 		OBJID			idAccount;
// 		DWORD			dwData;
// 		char			szInfo[64];
// 	}MSG_Info;
	struct UserSnapInfo
	{
		DWORD				userid;
		USHORT				Lev;
		USHORT				PhotoIndex;
		USHORT				firJob;				
		USHORT				firLev;				
		USHORT				secJob;					
		USHORT				secLev;				
		DWORD               firCoat; 
		DWORD				firWapon;				
		DWORD				firMount;				
		DWORD				secCoat;					
		DWORD				secWapon;				
		DWORD               secMount; 
		char			    szName[_MAX_NAMESIZE];
	};
	
	typedef struct {
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		OBJID			idAccount;
		DWORD			dwData;
		UINT            nServerIndex;
		char			szInfo[64*5];
		int				nRoleCount;
		UserSnapInfo    snapInfo[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGCONNECT_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_A)
