// MsgSynMemberInfo.h: interface for the CMsgSynMemberInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNMEMBERINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGSYNMEMBERINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "windows.h"
enum OnlineStatusChange
{
	SynMemberOffline,
	SynMemberOnline,
};

enum MsgSynMemberInfoAction
{
	SynMemberAdd=4,///加一个成员
	SynMemberRemove,///删一个成员
	SynMemberAnswerOK,///批准入帮
	SynMemberAnswerNO,///拒绝入帮
	SynMemberKickout,///踢出帮派
	SynMemberSetRank,///踢出帮派
	SynMemberSetAnnounce,///更新公告
	SynMemberOnlineStatusChange,///成员在线状态改变,参数1为上线,0为下线
// 	raRoleOffline,///玩家下线
// 	raQueryRole_Friend,///查询玩家好友
// 	raQueryRole_Syndicate,///查询玩家军团
// 	raQueryRole_Marry,///查询玩家婚姻
};

class CMsgSynMemberInfo : public CNetMsg  
{
public:
	CMsgSynMemberInfo();
	virtual ~CMsgSynMemberInfo();

	BOOL	Create		(USHORT nAction,USHORT nPage,USHORT nMaxAmount);
	BOOL	Append		(OBJID idUser,USHORT level,USHORT firJob,USHORT firJobLev,USHORT rank,DWORD proff,const char* szName,int nOnline);
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

//protected:
	struct MemberInfo
	{
		USHORT level;
		USHORT firJob;
		USHORT firJobLev;
		USHORT rank;
		OBJID idUser;
		USHORT proff;
		USHORT online;
		char szName[16];
	};

	typedef struct {
		unsigned short	unMsgSize;
		unsigned short	unMsgType;

		UCHAR	ucActiom;
		UCHAR	ucMaxAmount;
		UCHAR	ucAmount;
		UCHAR	ucPage;
		MemberInfo memInfo[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSYNMEMBERINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
