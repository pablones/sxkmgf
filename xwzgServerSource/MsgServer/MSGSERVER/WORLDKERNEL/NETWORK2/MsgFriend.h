// MsgFriend.h: interface for the CMsgGoodFriend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFRIEND_H__DC85CF8B_3B54_4C8D_94EA_F68172C0A179__INCLUDED_)
#define AFX_MSGFRIEND_H__DC85CF8B_3B54_4C8D_94EA_F68172C0A179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// include head file(s) hrere...
#include "NetMsg.h"

WORLDKERNEL_BEGIN
// define constant(s) here...
//--------------------------------------
//独孤求败添加[2008.12.12]
const int _FRIEND_ADDFRIEND					= 1;//申请移为密友 
//const int _FRIEND_MOVETOGOODFRIENDAPPLY		= 1;//申请移为密友 
const int _FRIEND_MOVETOGOODFRIENDACCEPT	= 2;//同意移为密友
const int _FRIEND_MOVEAPPLY					= 3;//申请移动好友(不能把非密友移为密友)
const int _FRIEND_MOVESUCESS				= 4;//移动好友成功(不能把非密友移为密友)

const int _FRIEND_CONTACTHISTORY_APPLY		= 5;//申请交互关系
const int _FRIEND_CONTACTHISTORY_GETINFO	= 6;//获取交互历史的好友信息
const int _FRIEND_CONTACTHISTORY_ONLINE		= 7;//交互关系好友上线
const int _FRIEND_CONTACTHISTORY_OFFLINE	= 8;//交互关系好友下线

const int _FRIEND_ADDTEMP					= 9;//加为临时好友,无需对方确认
//--------------------------------------
const int _FRIEND_APPLY			= 10;
const int _FRIEND_ACCEPT		= 11;
const int _FRIEND_ONLINE		= 12;
const int _FRIEND_OFFLINE		= 13;
const int _FRIEND_BREAK			= 14;
const int _FRIEND_GETINFO		= 15;
const int _ENEMY_ONLINE			= 16;			// to client // 更新到在线状态 
const int _ENEMY_OFFLINE		= 17;			// to client // 更新到离线状态 
const int _ENEMY_DEL			= 18;			// to server // to client // 删除一个仇人 
const int _ENEMY_ADD			= 19;			// to client // 添加一个仇人 

//[2010-06-07 goto]
const int _FRIEND_ADDFAIL		= 23; //添加好友失败 to client
const int _FRIEND_ADDSUCCESS	= 24; //添加好友成功 to client
const int _FRIEND_CHANGEGROUP	= 25; //修改好友分组 to client //to server
const int _FRIEND_CHANGEFAIL	= 26; //修改分组失败 to client
const int _FRIEND_DELFRIEND		= 27; //删除好友	 to client //to server
const int _FRIEND_DELFRIENDFAIL	= 28; //删除好友失败 to client
const int _FRIEND_SETFEEL		= 29; //修改心情	 to client //to server
const int _FRIEND_SETFEELFAIL	= 30; //修改心情失败 to client
const int _FRIEND_BESETFEEL		= 31; //好友修改心情	 to client
const int _FRIEND_GETFRIENDINFO	= 32; //获取好友的详细信息	 to client //to server
const int _FRIEND_DELCONTACT	= 33; //删除已经看过的留言	 to server
const int _FRIEND_HAVETEAM		= 34; //好友是否在队伍中	 to client //to server
const int _FRIEND_FEEL_SWITH	= 35; //心情开关
const int _FRIEND_QUERYFRIENDINFO = 36;
const int _FRIEND_REPLYFRIENDINFO = 37;///回应查询玩家的详细信息
const int _FRIEND_ADDSUCCESSREPLY	= 38; //添加好友成功后验证好友是否在线消息 to client
const int _FRIEND_ADDSUCCESS_PASSIVE =39;
const int _FRIEND_HAVETEAM_MT	= 101;//其他地图组发来的消息

class CMsgFriend : public CNetMsg
{
public:
	CMsgFriend();
	virtual ~CMsgFriend();

	enum { OFFLINE_STATUS, ONLINE_STATUS };
	//BOOL	Create	(int nAction, OBJID idFriend, LPCTSTR szName, bool bOnline = false, DWORD nLookFace=0, int nLevel=0,int nFriendType = 1);
	BOOL	Create	(int nAction, 
		OBJID idFriend, 
		LPCTSTR szName, 
		short bOnline = 0, 
		DWORD nLookFace=0, 
		int nLevel=0,
		unsigned int profession/*职业*/=0,
		unsigned int friendship/*友情度*/=0,
		unsigned int nSex = 0,
		unsigned int nFriendType = 1,
		LPCTSTR szFeel=NULL,//[2010-06-05 goto]增加好友心情
		int nRelation=0,//关系
		LPCTSTR szFaction=NULL,//帮派
		LPCTSTR szTitle=NULL,//称号
		LPCTSTR szMateName=NULL,//配偶
		LPCTSTR szTeamName=NULL,//队伍
		OBJID idMap=0//位置
		);

public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);

//private:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID			idFriend;	
		DWORD			dwLookFace;
		unsigned short	ucAction;
		unsigned short	ucOnlineFlag;
		unsigned short	ucLevel;
		unsigned short  ucRelation;//关系,师徒、夫妻...
		unsigned int	ucFriendship;//20061012修罗.友情度.预留
		unsigned int	ucProfession;//20061012修罗.职业
		unsigned int    ucSex;
		unsigned int	ucFriendType;
		//union{
		char			szName[_MAX_NAMESIZE];				//@ 可优化。只修改在线状态时，不传此数据。
		//OBJID			idTransmit;				// use for transmit
		//};
		char	szBuf[1];//[2010-06-05 goto]增加好友心情	
	}MSG_Info;

	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;
};
WORLDKERNEL_END

#endif // !defined(AFX_MSGFRIEND_H__DC85CF8B_3B54_4C8D_94EA_F68172C0A179__INCLUDED_)
