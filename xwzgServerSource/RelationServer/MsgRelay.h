// MsgRelay.h: interface for the CMsgRelay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGRELAY_H__94B6EC05_0B53_4CBC_B0B8_3C0579419356__INCLUDED_)
#define AFX_MSGRELAY_H__94B6EC05_0B53_4CBC_B0B8_3C0579419356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NetMsg.h"

enum MsgRelayAction
{
	raNone,
	raRoleOnline,///玩家上线
	raRoleOffline,///玩家下线
	raQueryRole_Friend,///查询玩家好友
	raQueryRole_Syndicate,///查询玩家军团
	raQueryRole_Marry,///查询玩家婚姻
};

///游戏服务器与关系服务器沟通专用消息，应答均使用同一消息，缩小代码规模
class CMsgRelay : public CNetMsg  
{
public:
	CMsgRelay();
	virtual ~CMsgRelay();
	
public:	
	BOOL Create(char* pMsgBuf,DWORD dwSize);///解包
	
	//[2010-07-26 goto]
	typedef struct tag_MSG_Info
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
	
		USHORT			usAction;///消息内部操作,匹配 MsgRelayAction
		OBJID			idRole;///角色ID
		OBJID			dstRoleID;///查询目标角色ID

		union
		{
			char	szUserName[_MAX_NAMESIZE];///角色名,登录时使用
			char	szServerName[_MAX_NAMESIZE];///线路名，查询其它服角色时返回，空则角色不在线			
		};

		///扩充内容，暂不使用
		USHORT			nRoleLevel;///玩家当前等级
		USHORT			nRoleMapID;///玩家所在地图ID

	}MSG_Info;
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGRELAY_H__94B6EC05_0B53_4CBC_B0B8_3C0579419356__INCLUDED_)
