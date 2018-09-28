// MsgPlayer.h: interface for the CMsgPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGPLAYER_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_)
#define AFX_MSGPLAYER_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"
#include "../AiNpc.h"

//////////////////////////////////////////////////////////////////////
// 用于dwSynID_Rank
const DWORD	MASK_SYNID			= 0x00FFFFFF;
const DWORD	MASK_RANK_SHIFT		= 24;

class CUser;
class CMsgPlayer : public CNetMsg  
{
public:
	CMsgPlayer();
	virtual ~CMsgPlayer();

public:	
//	BOOL			Create		(CBooth* pBooth);
	BOOL			Create		(IRole* pRole,bool justCome = false);
	BOOL			CreateBooth(OBJID idBooth,OBJID idMap,USHORT x,USHORT y,char* szName,OBJID idOwner);
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);

protected:
	typedef struct {
		MSGHEAD_DEFINE
		//在这个头文件中加成员一定要注意，要在NPCSERVER中也要添加,虽然它并不一定需要
		
		OBJID	id;
		USHORT  usLookFace;
		USHORT  usLevel;
		OBJID	usMapID;
		USHORT	usPosX;
		USHORT	usPosY;
		USHORT  ucDir;
		USHORT  nPK;
		UCHAR	bIsPMHide;//PM隐身
		UCHAR	nSex;
		

		DWORD	dwMaxLife;
		DWORD	dwLife;
		DWORD	dwMaxMana;
		DWORD	dwMana;
		DWORD	dwMaxSp;
		DWORD	dwSp;

		DWORD   dwMountType;
		DWORD	dwWeapenType;
		DWORD	dwDressType;
		DWORD	dwWingType;	
		DWORD	dwProfession;

		OBJID	idOwner;
		DWORD	dwAttackUser;
		OBJID	idMonsterType;

		OBJID	idTeam;
		OBJID	idTeamLeader;

		OBJID	idSyn;
		OBJID	nSynRank;

		OBJID	idBooth;

		DWORD	dwIsVip;

		DWORD	dwIsDress;

		char szBuf[1];	
	}MSG_Info;
	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGPLAYER_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_)
