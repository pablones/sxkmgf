// MsgInfoNpcUserExit.h: interface for the CMsgInfoNpcUserExit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGINFONPCUSEREXIT_H__DA8EBC75_8157_45E2_BCBA_DF03EA36702D__INCLUDED_)
#define AFX_MSGINFONPCUSEREXIT_H__DA8EBC75_8157_45E2_BCBA_DF03EA36702D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"

class CMsgInfoNpcUserExit : public CNetMsg  
{
public:
	CMsgInfoNpcUserExit();
	virtual ~CMsgInfoNpcUserExit();

	public:
	virtual BOOL Create		(char* pMsgBuf, DWORD dwSize);
	

public:
	BOOL Create(UINT idUser);						
	
private:
	typedef struct
	{
		UINT				idUser;	
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGINFONPCUSEREXIT_H__DA8EBC75_8157_45E2_BCBA_DF03EA36702D__INCLUDED_)
