// MsgWeapensoulShow.h: interface for the CMsgWeapensoulShow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWEAPENSOULSHOW_H__7577DE41_745A_42ED_873C_F18BD52A26D0__INCLUDED_)
#define AFX_MSGWEAPENSOULSHOW_H__7577DE41_745A_42ED_873C_F18BD52A26D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"
#include "Item.h"

enum EM_MSGWPSLSHOW
{
	EM_MSGWPSLSHOW_OUT= 1,
	EM_MSGWPSLSHOW_IN,
};

class CMsgWeapensoulShow : public CNetMsg  
{
public:
	CMsgWeapensoulShow();
	virtual ~CMsgWeapensoulShow();

public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);
public:
	BOOL Create(ST_WEAPONSOULDATA stWSLData,int nAction = EM_MSGWPSLSHOW_IN,OBJID idUser=ID_NONE,OBJID idItem=ID_NONE);
protected:
	typedef struct {
		MSGHEAD_DEFINE
		OBJID idItem;
		OBJID idUser;
		DWORD dwAction;
		USHORT usType;
		USHORT usLevel;
		USHORT usGrow;
		USHORT usMixLev;
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGWEAPENSOULSHOW_H__7577DE41_745A_42ED_873C_F18BD52A26D0__INCLUDED_)
