// Wargame.h: interface for the CWargame class.
//
//////////////////////////////////////////////////////////////////////
/********************************************************************
	created:	2007/08/27
	created:	27:8:2007   17:21
	filename: 	E:\mySVN\MSGServer\branch\TEST-activitySYS\MSGSERVER\MAPGROUPKERNEL\Wargame.h
	file path:	E:\mySVN\MSGServer\branch\TEST-activitySYS\MSGSERVER\MAPGROUPKERNEL
	file base:	Wargame
	file ext:	h
	author:		风不息
	
	purpose:	战争活动
*********************************************************************/
#if !defined(AFX_WARGAME_H__97C41ECC_03E6_4811_B5A9_87A9EFC503DE__INCLUDED_)
#define AFX_WARGAME_H__97C41ECC_03E6_4811_B5A9_87A9EFC503DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "WargameStatus.h" 
#include "I_Role.h"
#include "MsgWargame.h"
#include <map>

const int	WARGAME_ONTIMER_INTEVAL	=	1000;

enum	//	战争活动类型
{
	WARGAME_TYPE_ARENA	=	1,	//竞技场
};

enum	//	当前活动状态
{
	WARGAME_STATUS_STOP		=	0,	//	停止中
	WARGAME_STATUS_PREPARE	=	1,	//	准备
	WARGAME_STATUS_WORK		=	2,	//	进行中
};


typedef struct	//	活动信息
{
	OBJID	id;				//id
	USHORT	nType;			//类型
	USHORT	nCamps;			//阵营个数
	int		nSinginBegin;	//报名开始时间
	int		nSinginEnd;		//报名结束时间
	int		nStartTime;		//活动开始时间
	int		nEndTime;		//活动结束时间
	USHORT	nLevBegin;		//活动等级下限
	USHORT	nLevEnd;		//活动等级上限
	USHORT	nMaxPlayer;		//人数上限
	USHORT	nBaseScore;		//积分基值
	USHORT	nScoreParam;	//积分参数
} WARGAME_INFO;

typedef struct	//	活动信息
{
	OBJID	id;				//id
	UCHAR	ucCamp;			//所在阵营
	UCHAR	ucVipLev;		//vip等级
	USHORT	usLev;			//等级
	DWORD	dwPro;			//职业
	UINT	nKilled;		//杀人次数
	UINT	nBeKilled;		//死亡次数
	UINT	nScore;			//本场积分
	UINT	nAwardScore;	//奖励积分
	OBJID	idLastKilled;	//杀死的玩家ID
	UINT	nKillTime;		//上次杀人的时间
	NAMESTR	szName;			//姓名
	clock_t tDieTime;		//死亡时间
	bool	bEntered;		//是否进场
} WARGAME_PLAYER;

typedef struct	//	活动信息
{
	OBJID	id;				//id
	USHORT	nType;			//活动类型
	OBJID	idMap;			//地图ID
	int		nBorn_X;//出生点坐标
	int		nBorn_Y;
	int		nReBorn_X;//复活点坐标
	int		nReBorn_Y;
} WARGAMEMAP_INFO;

typedef struct
{
	int	nScore;
	int nPlayerAmount;	//只记录进入竞技场的人数
	WARGAMEMAP_INFO mapInfo[1];
} WARGAME_CAMPS;

extern int g_SpecilLine;

class CWargame  
{ 
public:
	CWargame();
	virtual ~CWargame();

	static CWargame* CreateNew(int nType);
	bool CreateWargame(WARGAME_INFO &info, int idProcess);	//	创建
	void Release();									//	当施放时被调用
	int  LoadMap();
	void Stop();
public:
	virtual void Init();							//	初始化,初始化各个状态
	virtual void OnTimer(time_t tCurr);
	virtual void OnProcess();						//	当运行时被OnTimer调用
	virtual bool CanSignup();						//是否在报名时间
	virtual bool EnterWar(CUser* pUser);
	virtual void LeaveWar(CUser* pUser);
	virtual void OnPrepare();
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnPlayerKill(IRole* pAtker, IRole* pTarget);//杀人后的处理
	virtual void ProcessReborn(OBJID idUser);
	virtual void BroadCaseEnterMsg();

public:
	int GetCurrentStatus();
	int GetExpectStatusByTime();						//当前时间的状态
	
	//int GetType();									//得活动类型
	OBJID	GetWarGameID();								//得到活动的唯一ID

	bool Signup(CUser* pUser, bool bSynchro=true);		//玩家报名
	bool Signup(WARGAME_PLAYERSCORE* pInfo);
	
	
	int	 AllocCamp();//新选手划分阵营
	int  SendScoreRank(CUser* pUser);//发送战场积分榜数据,传NULL表示竞技场内广播
	bool  SendCampScore(CUser* pUser=NULL);

	bool IsMaxPlayer();
	bool IsMaxSigninPlayer();
	bool CanUserEnter();
	bool AddWarGamePlayer(WARGAME_PLAYER info);			//未检查重复添加
	bool GetRebornPos(OBJID idUser, int &idMap, int &nPosX, int &nPosY);

	void BroadCaseWarGameMsg(CMsgWargame* pMsg);
	
	int  GetWinCamp(int &nMaxScore);//取得获胜阵营，返回0表示平手

	WARGAME_CAMPS*	GetCampsInfo(int nCamps);
	WARGAME_PLAYER* GetWarGamePlayer(OBJID idUser);

	void SynchroPlayerAmount(WARGAME_PLAYERSCORE* pInfo, int nAmount);

	CGameMap* GetWarGameMap(CUser* pUser, int& nPosX, int& nPosY);

	int GetPlayerInWar();
	void PlayerLogout(CUser* pUser);

	static bool IsWarGameMap(OBJID idMap);

	void BroadCastCampMsg(OBJID idSender, CNetMsg* pMsg);//阵营聊天

protected:
	WARGAME_INFO	m_wargameInfo;				//	活动信息
	vector<WARGAMEMAP_INFO>		m_vetMapInfo;	//地图信息
	vector<WARGAME_PLAYER>		m_vetPlayerInfo;//玩家信息
	map<int, WARGAME_CAMPS>		m_mapCampScore;	//阵营信息
	int m_nStatus;
	int m_idProcess;					//	地图组ID
	clock_t m_tLastOnTimer;
	clock_t m_tStopTime;//结束时间
	//int m_nProfession;
};

#endif // !defined(AFX_WARGAME_H__97C41ECC_03E6_4811_B5A9_87A9EFC503DE__INCLUDED_)
