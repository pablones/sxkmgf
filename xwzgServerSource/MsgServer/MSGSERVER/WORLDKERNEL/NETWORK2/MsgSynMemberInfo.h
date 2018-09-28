// MsgSynMemberInfo.h: interface for the CMsgSynMemberInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNMEMBERINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGSYNMEMBERINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum MsgSynMemberInfoAction
{
	SynMemberAdd=4,///加一个成员
	SynMemberRemove,///删一个成员
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
	BOOL	Append		(OBJID	id,	const char* szName,	USHORT	level,
							   USHORT	firJob,	USHORT	sex,USHORT	rank,USHORT   line,USHORT	vip,
						 OBJID   lastlogin,OBJID   hiscon,OBJID   hismoney,OBJID   hisgrow,OBJID	maxpower);
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

public:
	struct MemberInfo
	{
		OBJID	id;
		char	szName[16];	
		USHORT	level;
		USHORT	firJob;
		USHORT	sex;
		USHORT	rank;
		USHORT  line;
		USHORT	vip;
		OBJID   lastlogin;
		OBJID   hiscon;
		OBJID   hismoney;
		OBJID   hisgrow;
		OBJID	maxpower;
	};

	typedef struct {
		MSGHEAD_DEFINE
		UCHAR	ucActiom;
		UCHAR	ucMaxAmount;
		UCHAR	ucAmount;
		UCHAR	ucPage;
		MemberInfo memInfo[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSYNMEMBERINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
