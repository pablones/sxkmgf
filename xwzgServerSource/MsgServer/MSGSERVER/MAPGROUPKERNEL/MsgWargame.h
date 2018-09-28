// MsgWargame.h: interface for the CMsgWargame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWARGAME_H__0B8223DA_7D96_430F_9C0D_27BFA4ED4A84__INCLUDED_)
#define AFX_MSGWARGAME_H__0B8223DA_7D96_430F_9C0D_27BFA4ED4A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"

// enum	//	胜利标志
// {
// 	WARGAME_WIN_FLAG_NONE = 0,	//	无效
// 	WARGAME_WIN_FLAG_WIN =  1,	//	胜利
// 	WARGAME_WIN_FLAG_LOST = 2,	//	失败
// 	WARGAME_WIN_FLAG_DRAW = 3,	//	平局
// };
enum
{
	WARGAME_NONE	=	0,
	WARGAME_LINEBORAD,	//用于线路广播
	WARGAME_SIGNIN,		//报名
	WARGAME_SCORERANK,	//积分榜
	WARGAME_ENTER,		//通知客户端可以进场
	WARGAME_CAMPSCORE,	//各阵营积分
};

enum
{
	WARGAMELINEBORAD_NONE	=	0,
	WARGAMELINEBORAD_LOADMAP,
	WARGAMELINEBORAD_START,		//通知进场，由专线通知
	WARGAMELINEBORAD_SIGNIN,	//报名
	WARGAMELINEBORAD_ENTER,		//进场
	WARGAMELINEBORAD_LEAVE		//出场
};

typedef struct  
{
	OBJID	idPlayer;
	USHORT	usRank;		//名次
	USHORT	usLev;		//等级
	int		nScore;		//当前积分
	int		nAwardScore;//奖励积分
	USHORT	usKilled;	//杀人数
	UCHAR	ucCamp;		//阵营
	UCHAR	ucVipLev;	//vip等级
	DWORD	dwProfession;//职业
	NAMESTR	szName;		//名字
}WARGAME_PLAYERSCORE;	//	统计条目

class CMsgWargame : public CNetMsg  
{
public:
	CMsgWargame();
	virtual ~CMsgWargame();

	BOOL Create(char* pbufMsg, DWORD dwMsgSize);
	BOOL Create(OBJID idUser, USHORT usAction, UCHAR ucPage);

	BOOL Append(WARGAME_PLAYERSCORE info);
	BOOL Append(OBJID idUser, USHORT usLev, int nScore, UCHAR ucCamp, UCHAR ucVipLev, DWORD dwProfession, LPCTSTR szName);

	void Process(void* pInfo);
	void ProcessLineMsg(void *pInfo);

	int  GetAmount()	{ return m_pInfo->ucAmount; }

public:
	typedef struct {
		MSGHEAD_DEFINE

		OBJID	idUser;
		USHORT	usAction;
		UCHAR	ucPage;
		UCHAR	ucAmount;
		WARGAME_PLAYERSCORE	stUserList[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGWARGAME_H__0B8223DA_7D96_430F_9C0D_27BFA4ED4A84__INCLUDED_)
