// MsgFatigue.h: interface for the CMsgFatigue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFATIGUE_H__0CAAE42A_1143_4E01_A146_E8DA7ABDA6F1__INCLUDED_)
#define AFX_MSGFATIGUE_H__0CAAE42A_1143_4E01_A146_E8DA7ABDA6F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
WORLDKERNEL_BEGIN

class CMsgFatigue : public world_kernel::CNetMsg
{
public:
	CMsgFatigue();
	virtual ~CMsgFatigue();
public:
	BOOL Create( OBJID AccountID, OBJID PlayerID,  int Fatigue = 0);

	void Process(void *pInfo);

	BOOL Create(char* pMsgBuf,DWORD dwSize);
	typedef struct tag_MSG_Info
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		
		OBJID			AccountID;///’ ∫≈ID
		OBJID			PlayerID;///Ω«…´ID
		int				Fatigue	;//∑¿≥¡√‘
	}MSG_Info;
	MSG_Info*	m_pInfo;  

};
WORLDKERNEL_END

#endif // !defined(AFX_MSGFATIGUE_H__0CAAE42A_1143_4E01_A146_E8DA7ABDA6F1__INCLUDED_)
