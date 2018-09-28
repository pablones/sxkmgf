// MsgInteract.h: interface for the CMsgInteract class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSTEAL_H__05C99CA2_5829_11D5_A212_0000E8E7F527__INCLUDED_)
#define AFX_MSGSTEAL_H__05C99CA2_5829_11D5_A212_0000E8E7F527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum {	INTERACT_NONE =0, 
		INTERACT_STEAL, 
		INTERACT_ATTACK, 
		INTERACT_HEAL, 
		INTERACT_POISON, 
		INTERACT_ASSASSINATE,
		INTERACT_FREEZE,
		INTERACT_UNFREEZE,
		INTERACT_COURT,
		INTERACT_MARRY,
		INTERACT_DIVORCE			= 10,
		INTERACT_PRESENTMONEY,
		INTERACT_PRESENTITEM,
		INTERACT_SENDFLOWERS,
		INTERACT_KILL				= 14,
		INTERACT_JOINSYN,
		INTERACT_ACCEPTSYNMEMBER,
		INTERACT_KICKOUTSYNMEMBER,
		INTERACT_PRESENTPOWER,
		INTERACT_QUERYINFO,
		INTERACT_RUSHATK			= 20,
		INTERACT_MAGICATTACK		= 21, 
		INTERACT_ABORTMAGIC,
		INTERACT_REFLECTWEAPON,
		INTERACT_BUMP,
		INTERACT_SHOOT				= 25,
		INTERACT_REFLECTMAGIC,
		INTERACT_CUTMEAT,					// 暂时没用： 割肉 idSender:HeroID, iDTarget:Target ID, usPosX HeroPosX, usPosY HeroPosY
		INTERACT_MINE,                      // 挖矿 idSender:HeroID, iDTarget:Target ID, usPosX HeroPosX, usPosY HeroPosY 
		INTERACT_QUARRY,					// 暂时没用： 采石 idSender:HeroID, iDTarget:Target ID, usPosX HeroPosX, usPosY HeroPosY
		INTERACT_CHOP				= 30,	// 暂时没用： 伐木 idSender:HeroID, iDTarget:Target ID, usPosX HeroPosX, usPosY HeroPosY
		INTERACT_HUSTLE,					// 暂时没用： 挤人，dwData为下一步的方向。(注意：玩家坐标同时向该方向移动一步)
		INTERACT_SOUL,						// 幻兽灵魂吸取怪物魂魄表现
		INTERACT_OPENWARSKILL,		//33,	战士技能启动 //06.3.21王玉波
		INTERACT_CLOSEWARSKILL,		//34	战士技能关闭
		INTERACT_INTONESCROLL,		//35,    哈雷070324，添加使用卷轴状态
		INTERACT_OPENSAVEITEM,      //打开道具赎回
		ITEMACT_SAVEITEMSHOW,        //道具显示
		INTERACT_MAYYRTYPE,         //婚礼类型
		//20080108修罗:AI相关--------------------
		INTERACT_SynEnmity	= 201,		//msg 2 npc 同步仇恨列表
		//---------------------------------------
		INTERACT_MOUNT	=	300,	//[2010-08-09 goto]坐骑
		INTERACT_MOUNT_ON	=	301,//[2010-08-09 goto]上坐骑
		INTERACT_MOUNT_OFF	=	302,//[2010-08-09 goto]下坐骑
};

class CMsgInteract : public CNetMsg  
{
public:
	CMsgInteract();
	virtual ~CMsgInteract();

	BOOL	Create2(USHORT unType, OBJID idSender, OBJID idTarget, USHORT unPosX, USHORT unPosY,DWORD dwData = 0,DWORD dwBattleAddInfo=0);
	BOOL	Create(USHORT unType, OBJID idSender, OBJID idTarget, USHORT unPosX, USHORT unPosY, USHORT usMagicType, USHORT usMagicLev, DWORD nIntoneTime=0);

public:	
	BOOL		Create		(char* pMsgBuf, DWORD dwSize);
	void		Process		(void* pInfo);
	
private:
	typedef struct{
		MSGHEAD_DEFINE
		OBJID	idSender;
		OBJID	idTarget;

		USHORT	unType;
		DWORD	dwData;
		USHORT	usPosX;
		USHORT	usPosY;
		USHORT	usMagicType;
		USHORT	usMagicLev;
		DWORD dwBattleAddInfo;
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSTEAL_H__05C99CA2_5829_11D5_A212_0000E8E7F527__INCLUDED_)
