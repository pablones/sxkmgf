// MsgNewPlayerExam.h: interface for the CMsgNewPlayerExam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNEWPLAYEREXAM_H__93A93E08_0786_406E_BEC1_449670896972__INCLUDED_)
#define AFX_MSGNEWPLAYEREXAM_H__93A93E08_0786_406E_BEC1_449670896972__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"

class CMsgNewPlayerExam : public CNetMsg
{
public:
	CMsgNewPlayerExam();
	virtual ~CMsgNewPlayerExam();

public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	BOOL			Create		(USHORT usAction, char* pSzTemp,USHORT usSize);
	void			Process		(void* pInfo);

	typedef struct{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		USHORT		usAction;
		UINT	nAccountID;
		UINT	nUserID;
		USHORT	usSzSize;//szBuf size
		char	szBuf[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGNEWPLAYEREXAM_H__93A93E08_0786_406E_BEC1_449670896972__INCLUDED_)
