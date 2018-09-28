// MsgConnect.h: interface for the CMsgConnect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCONNECT_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_)
#define AFX_MSGCONNECT_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgConnect : public CNetMsg  
{
public:
	CMsgConnect();
	virtual ~CMsgConnect();

	BOOL	Create	(OBJID idAccount, DWORD dwData, char* pszInfo);																	
	bool    Add(DWORD id,USHORT level,USHORT photoIndex,char* pszInfo,USHORT firJob,USHORT firLev,USHORT secJob,USHORT secLev,DWORD firCoat,DWORD firWapon,DWORD firMount,DWORD secCoat,DWORD secWapon,DWORD secMount);
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pSocket);

protected:
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
		MSGHEAD_DEFINE
		OBJID			idAccount;
		DWORD			dwData;
		UINT            nServerIndex;
		char			szInfo[64*5];
		int				nRoleCount;
		UserSnapInfo    snapInfo[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGCONNECT_H__F6692EA5_5D32_4E87_AA3C_4B37A6BE6695__INCLUDED_)
