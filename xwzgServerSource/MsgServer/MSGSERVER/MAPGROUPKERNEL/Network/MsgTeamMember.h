// MsgTeamMember.h: interface for the CMsgTeamMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTEAMMEMBER_H__0C122D39_8E5B_48A9_94BA_0C20ED3C3E23__INCLUDED_)
#define AFX_MSGTEAMMEMBER_H__0C122D39_8E5B_48A9_94BA_0C20ED3C3E23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "../User.h"

enum
{
	_MSG_TEAMMEMBER_ADDMEMBER = 0,
	_MSG_TEAMMEMBER_DROP_ADDMEMBER = 1,
	_MSG_TEAMMEMBER_CHGMAP = 2 ,
	_MSG_TEAMMEMBER_LEV = 3,
	_MSG_TEAMMEMBER_ANSWERINFO = 4,
	_MSG_TEAMMEMBER_ANSWERINVITEINFO = 5,
};

#define _MAX_TEAMAPPLY 8

// By Minsky,2010-05-28，申请人的信息
struct TeamApplyInfo
{
	OBJID			idPlayer;// 玩家的ID
	OBJID			dwLookFace;// 玩家的头像
	char            szPlayerName[_MAX_NAMESIZE];// 玩家名字
	USHORT			usLev;// 申请人的级别,不高于999
	USHORT			usPro;// 申请人的职业
	USHORT			usProLev;// 申请人的技能等级,不高于999
//	USHORT          idMap;// 玩家所在的地图
};

class CMsgTeamMember : public CNetMsg  
{
public:
	CMsgTeamMember();
	virtual ~CMsgTeamMember();

public:
	BOOL	Create			(CUser* pMember);
	BOOL	Create			(int nAction, TeamMemberInfo setInfo);
    BOOL    Create          (int nAction, UINT nMapID, UINT id, int nData = 0);
	BOOL	Create			(char* pbufMsg, DWORD dwMsgSize);
	BOOL	Append			(int nAction,TeamMemberInfo info);

	void	Process			(void* pInfo);

private:
	typedef struct 
	{
		MSGHEAD_DEFINE
		UINT				ucAction;
		UINT        		ucAmount;
		UINT                id;
		int                 nData;
		USHORT				usSizeIn;// 队内成员的人数
		USHORT				usSizeOut;// 等待申请的人数
		TeamMemberInfo		memInfo[1];
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTEAMMEMBER_H__0C122D39_8E5B_48A9_94BA_0C20ED3C3E23__INCLUDED_)
