// Friend.h: interface for the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIEND_H__5845339C_B1A7_4C13_AC56_D1374692D706__INCLUDED_)
#define AFX_FRIEND_H__5845339C_B1A7_4C13_AC56_D1374692D706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FriendData.h"
#include "Myheap.h"
#include "TimeOut.h"

//////////////////////////////////////////////////////////////////////
const int	LEAVEWORD_INTERVAL_SECS	= 5;							// 留言间隔秒数
const int	MAX_GROUP_FRIEND		= 10;							// 每个好友分组最大好友数
const int	MAX_GROUP				= 5;							// 好友的分组个数
const int	TEMP_FRIEND_GROUP		= 5;							// 临时好友的分组号
const int	ENEMY_FRIEND_GROUP		= 6;							// 黑名单的分组号
const int	ENEMY_GROUP	= 7;										// 仇人分组
//////////////////////////////////////////////////////////////////////
class CUser;
class CFriend : public CFriendData  
{
public:
	CFriend();
	virtual ~CFriend();
	ULONG	ReleaseByOwner() { delete this; return 0; }

public:
	static CFriend*	CreateNewFriend		(const FriendInfoStruct* pInfo);

public: // static
	static CFriend*	CreateNew() { return (new CFriend); }

public: // application
	bool	IsLeaveWordEnable()				{ return m_tLeaveWord.ToNextTime(LEAVEWORD_INTERVAL_SECS); }//一分钟太长，改成5秒

protected:
	CTimeOut	m_tLeaveWord;

public: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_FRIEND_H__5845339C_B1A7_4C13_AC56_D1374692D706__INCLUDED_)
