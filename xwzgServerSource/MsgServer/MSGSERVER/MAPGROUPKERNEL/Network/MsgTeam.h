// MsgTeam.h: interface for the CMsgTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTEAM_H__3E7E3CFB_3522_4444_941B_BFDCABD1EFB7__INCLUDED_)
#define AFX_MSGTEAM_H__3E7E3CFB_3522_4444_941B_BFDCABD1EFB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

const int TEAMINFO_AUTO	=	1;
const int TEAMINFO_LOCK =   2;
const int TEAMINFO_EXP_PRIVATE = 4;
const int TEAMINFO_EXP_PUBLIC = 8;
const int TEAMINFO_ITEM_PRIVATE = 16;
const int TEAMINFO_ITEM_PUBLIC  = 32;
const int TEAMINFO_ITEM_RANDOM  = 64;

enum 
{
	_MSGTEAM_CREATE				=	0,	// 建立组队
	_MSGTEAM_APPLYJOIN			=	1,  // 申请加入	

	_MSGTEAM_LEAVE				=	2,	// 离开组队

	_MSGTEAM_ACCEPTINVITE		=	3,	// 接受邀请	
	_MSGTEAM_INVITE				=	4,	// 邀请加入组队

	_MSGTEAM_AGREEJOIN			=	5,  // 同意加入组队	

	_MSGTEAM_DISMISS			=	6,	// 解散组队
	_MSGTEAM_KICKOUT			=	7,	// 将某人踢出组队
	_MSGTEAM_JOINTEAM			=	10,	// 加入组队
	_MSGTEAM_LEAVETEAM			=   11,	// 某人离开某队
	_MSGTEAM_NOTALLOWTEAM		=	12,	// 设置自己为不允许组队
	_MSGTEAM_OFLEADER			=	13,	// 移交队长

	//20070201修罗:小队换名----------------------
	_MSGTEAM_CHG_NAME_REQ		=	14,//C2S
	_MSGTEAM_CHG_NAME_APPLY		=	15,//S2C
	_MSGTEAM_CHG_NAME_ROOMBROADCAST_REQ	=	16,//C2S
	_MSGTEAM_CHG_NAME_ROOMBROADCAST_APPLY	=	17,//S2C
	//-------------------------------------------

	_MSGTEAM_CHG_LEADER_APPLY	= 18,// 变更组队队长

	_MSGTEAM_CHG_CONFIG			= 19,// 组队属性改变
	
	//20070307softworms:小队中某人共享状态改变通知
	_MSGTEAM_CHG_SHARESTATUS_IN			= 20,//进入共享区域
	_MSGTEAM_CHG_SHARESTATUS_OUT		= 21,//离开共享区域
	_MSGTEAM_SEARCH_TEAM                = 22,
	_MSGTEAM_SEARCH_ROLE                = 23,

	_MSGTEAM_ASKINFO					= 24,			

	_MSGTEAM_SEARCH_BEGIN,
	_MSGTEAM_SEARCH_END,
	_MSGTEAM_TEAM_INFO,
//	_MSGTEAM_DISCRIPT,
	_MSGTEAM_JOIN_WAIT,
	_MSGTEAM_INVITE_WAIT,
	_MSGTEAM_CHANGE_TEXT,

	_MSGTEAM_DENY_INVITE				= 31,// 
	_MSGTEAM_DENY_APPLY					= 32,// 

	_MSGTEAM_MEMMBER_APPLY  			= 33,// 
	_MSGTEAM_MEMADDLIFE_EFFECT,

	_MSGTEAM_TARGET_INTEAM				= 35,// 对方在队伍中
	_MSGTEAM_USER_INTEAM				= 36,// 自己在队伍中
	_MSGTEAM_ASKINVITE					= 37,
	_MSGTEAM_DENY_MEMMBERAPPLY  		= 38, 
	_MSGTEAM_MIXTEAM			  		= 39, 
 	_MSGTEAM_UPDATA_ONLINEINFO			= 40,//队员在线状态
};

class CMsgTeam : public CNetMsg  
{
public:
	CMsgTeam();
	virtual ~CMsgTeam();
	
public:
	BOOL	Create	(char* pbufMsg, DWORD dwMsgSize);
	
	void	Process	(void* pInfo);
	BOOL	Create	(OBJID idPlayer, int nAction, UINT nMap = 1001);
	
	//  [2007-9-8] 朱斌
	BOOL 	Create	(OBJID idPlayer, const char* szPlayerName, int nAction,  UINT idMap = 0, UINT nTarget = 0, const char* szTargetName = NULL);
		
	BOOL 	Create	(int nAction, UINT nTarget, UINT nTeamLev, UINT nLeaderID, const char* szLeaderName, UINT nLeaderLev, UINT nMemAmount );
	BOOL    CreateMemInfo(int nAction, OBJID idPlayer, const char *pName, int nLev);

	BOOL	CreateAskInfo(int nAction);
private:
	typedef struct 
	{
		MSGHEAD_DEFINE
		USHORT			unAction;
		USHORT			usLev;
		USHORT			usPro;
		USHORT			usProLev;
		OBJID			idPlayer;		
		UINT            idMap;
		UINT            idTarget;
		char            szPlayerName[_MAX_NAMESIZE];
		char            szTargetName[_MAX_NAMESIZE];
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTEAM_H__3E7E3CFB_3522_4444_941B_BFDCABD1EFB7__INCLUDED_)
