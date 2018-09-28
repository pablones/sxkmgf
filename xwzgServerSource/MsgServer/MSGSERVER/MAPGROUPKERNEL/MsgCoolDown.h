
#ifndef _MSGCOOLDWON_H
#define _MSGCOOLDWON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
enum
{

	MAGICCOOLDOWN_BEGIN			=	0,		//开始技能冷却时间
	MAGICCOOLDOWN_END			=	1,		//结束技能冷却时间
};
enum INFO_TYPE
{
	MAGIC_INFO	= 0,								//技能
	ITEM_INFO	= 1,								//物品
};
//FileName:	MsgCoolDown.h
//Time:		2006.9.4
//Author:	王玉波
//About:	关于技能冷却时间加的消息类


class CMsgCoolDown:public CNetMsg
{
	
public:
	CMsgCoolDown();
	virtual ~CMsgCoolDown();
	
	BOOL	Create		( OBJID id,USHORT usAction,USHORT usInfoType,OBJID idObjType,DWORD dwPrivateCDTime,DWORD dwPublicCDTime );
	void	Process		( void* pInfo );
	
protected:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID		idUser;							//user id
		USHORT		usAction;						//action
		USHORT		usInfoType;						// 目前为魔法与物品
		OBJID		idObjType;						//技能编号或者物品TYPE ID
		DWORD		dwPrivateCDTime;				//此技能的CD时间
		DWORD		dwPublicCDTime;					//公共CD时间
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};











#endif // _MSGCOOLDWON_H