// MsgSuperWeaponSoul.h: interface for the CMsgSuperWeaponSoul class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSUPERWEAPONSOUL_H__779A2F43_5B9E_4A3D_9A56_85488B83ACC2__INCLUDED_)
#define AFX_MSGSUPERWEAPONSOUL_H__779A2F43_5B9E_4A3D_9A56_85488B83ACC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"
#include "ItemType.h"
#include "WeaponSoulData.h"
enum{
	ACTION_WEAPONSOULERROR		= 0,
	ACTION_GETSUPERWEAPONSOULINFO	= 1,
	ACTION_CREATESUPERWEAPONSOUL	= 2,
};

class CMsgSuperWeaponSoul : public CNetMsg  
{
public:
	CMsgSuperWeaponSoul();
	virtual ~CMsgSuperWeaponSoul();
public:
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);
	BOOL			Create		(ST_WEAPONSOULDATA* WepeonSoulItemInfo, USHORT usAction,USHORT usSuc);
protected:
	typedef struct {

//	unsigned short	unMsgSize;
// 	unsigned short	unMsgType;

	ST_WEAPONSOULDATA  unWeaponSoulItemInfo;
	USHORT usAction;
	USHORT usSuc;
	
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSUPERWEAPONSOUL_H__779A2F43_5B9E_4A3D_9A56_85488B83ACC2__INCLUDED_)
