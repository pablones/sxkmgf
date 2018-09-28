// MsgInfoNpcUserExit.h: interface for the CMsgInfoNpcUserExit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGINFONPCUSEREXIT_H__69C2A2EA_F579_4104_B4DF_2111C18A3629__INCLUDED_)
#define AFX_MSGINFONPCUSEREXIT_H__69C2A2EA_F579_4104_B4DF_2111C18A3629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
using namespace world_kernel;
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

#endif // !defined(AFX_MSGINFONPCUSEREXIT_H__69C2A2EA_F579_4104_B4DF_2111C18A3629__INCLUDED_)
