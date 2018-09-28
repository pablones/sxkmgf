// MsgWeaponSoul.h: interface for the CMsgWeaponSoul class.
//	20071214:修罗
//	泪灵同步专用消息
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWEAPONSOUL_H__AA434F7B_2065_45B4_8384_4220F2EDB7C3__INCLUDED_)
#define AFX_MSGWEAPONSOUL_H__AA434F7B_2065_45B4_8384_4220F2EDB7C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

WORLDKERNEL_BEGIN
enum EM_MSGWPSL
{
	EM_MSGWPSL_UPDATE	= 1,

	EM_MSGWPSL_QUERY	= 101,
	EM_MSGWPSL_MIX,
	EM_MSGWPSL_PROPOPEN,
	EM_MSGWPSL_PROPRAND,
	EM_MSGWPSL_PROPLEAN,
	EM_MSGWPSL_PROPUPLEV,
	EM_MSGWPSL_SKILLLEAN,
	EM_MSGWPSL_SKILLLEVUP,
	EM_MSGWPSL_SKILLRAND,
	EM_MSGWPSL_TYPERAND,
	EM_MSGWPSL_GROWUP,
	EM_MSGWPSL_MIXITEM,
	EM_MSGWPSL_QUERY_LINE,
	EM_MSGWPSL_UPDATE_LINE,
};

class CMsgWeaponSoul : public world_kernel::CNetMsg 
{
public:
	CMsgWeaponSoul();
	virtual ~CMsgWeaponSoul();

public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);

public:
	BOOL Create(int nAction = EM_MSGWPSL_UPDATE,OBJID idUser=ID_NONE,OBJID idItem=ID_NONE,OBJID idNeedUser = ID_NONE);

	typedef struct {
		MSGHEAD_DEFINE
		USHORT usAction;

		OBJID idItem;
		OBJID idUser;

		OBJID ID;//ID
		OBJID idItem2;
		UINT  nType;
		UINT  level;//等级
		UINT  exp;
		UINT  lifespan;
		UINT  grow;
		UINT  mixlev;
		UINT  phyattlev;
		UINT  magattlev;
		UINT  basestr;
		UINT  baseint;
		UINT  basesta;
		UINT  basespi;
		UINT  baseagi;
		UINT  addition[10];
		UINT  skill[3];
	}MSG_Info;

	MSG_Info*	m_pInfo;

};
WORLDKERNEL_END
#endif // !defined(AFX_MSGWEAPONSOUL_H__AA434F7B_2065_45B4_8384_4220F2EDB7C3__INCLUDED_)
