// MsgFightTeamAction.h: interface for the CMsgFightTeamAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFIGHTTEAMACTION_H__49F9023E_2AE8_4B39_9BDD_4F28F600F45F__INCLUDED_)
#define AFX_MSGFIGHTTEAMACTION_H__49F9023E_2AE8_4B39_9BDD_4F28F600F45F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AllMsg.h"

WORLDKERNEL_BEGIN
enum FightTeamAction
{
	FightTeamAction_Create,        //创建战队
		FightTeamAction_Destroy,       //删除战队
		FightTeamAction_InviteMember,  //邀请成员
		FightTeamAction_Refruse,       //邀请拒绝
		FightTeamAction_OK,            //邀请成功
		FightTeamAction_ChangeLeader,  //更换队长
		FightTeamAction_DelMember,     //删除队友
		FightTeamAction_ExitTeam,      //退出队伍
		FightTeamAction_OnLine,        //是否在线
		FightTeamAction_Query,         //获取战队信息
		FightTeamAction_InviteEnterInstace,  //邀请进入FB
		FightTeamAction_ExitInstace,		//退出竞技场
		FightTeamAction_IsInArena,
		FightTeamAction_UpdateTeamInfo,
		FightTeamAction_LeaveTeamOK,
};

class CMsgFightTeamAction : public CNetMsg  
{
public:
	CMsgFightTeamAction();
	virtual ~CMsgFightTeamAction();

	BOOL	Create(int nType, const char* pszFightTeamName, const char* pszMemberName,DWORD dwData,DWORD dwType=0);
	
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);
	
private:
	typedef struct{
		MSGHEAD_DEFINE
		

		unsigned short ucFightTeamType; //操作类型
        DWORD  dwData;  //战队类型或是用户ID
        DWORD  dwType;
        char szBuf[1]; //用于记录战队名字和成员名字
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
private:
	CNetStringPacker	m_StrPacker;
};
WORLDKERNEL_END
#endif // !defined(AFX_MSGFIGHTTEAMACTION_H__49F9023E_2AE8_4B39_9BDD_4F28F600F45F__INCLUDED_)
