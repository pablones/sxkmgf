// MsgFatigue.h: interface for the MsgFatigue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFATIGUE_H__A9BCEDB8_E651_4229_8B98_3DDAC9C47269__INCLUDED_)
#define AFX_MSGFATIGUE_H__A9BCEDB8_E651_4229_8B98_3DDAC9C47269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"
class CMsgFatigue  : public CNetMsg 
{
public:
	CMsgFatigue();
	virtual ~CMsgFatigue();
	BOOL Create( OBJID AccountID, OBJID PlayerID,  int Fatigue);
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

#endif // !defined(AFX_MSGFATIGUE_H__A9BCEDB8_E651_4229_8B98_3DDAC9C47269__INCLUDED_)
