// MsgFatigue.h: interface for the CMsgFatigue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFATIGUE_H__FD98CF03_BA5E_4400_95AE_83F2770501F2__INCLUDED_)
#define AFX_MSGFATIGUE_H__FD98CF03_BA5E_4400_95AE_83F2770501F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"
#include "NetStringPacker.h"
#define _MSG_FATIGUE_NOTALLOWLOGIN 10000

class CMsgFatigue  : public CNetMsg  
{
public:
	BOOL Create( OBJID AccountID, OBJID PlayerID,  int Fatigue = 0);
	void Process(void *pInfo);
	CMsgFatigue();
	virtual ~CMsgFatigue();
	BOOL Create(char* pMsgBuf,DWORD dwSize);
	typedef struct tag_MSG_Info
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		
		OBJID			AccountID;///’ ∫≈ID
		OBJID			PlayerID;///Ω«…´ID
		int			Fatigue	;//∑¿≥¡√‘
	}MSG_Info;
	MSG_Info*	m_pInfo;  

};

#endif // !defined(AFX_MSGFATIGUE_H__FD98CF03_BA5E_4400_95AE_83F2770501F2__INCLUDED_)
