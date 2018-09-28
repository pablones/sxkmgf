// MapInstance.h: interface for the CMapInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPINSTANCE_H__1CB83F41_11BA_4625_B1B8_249123985025__INCLUDED_)
#define AFX_MAPINSTANCE_H__1CB83F41_11BA_4625_B1B8_249123985025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sigslot.h"
#include "../share/protocol.h"
using namespace sigslot;

#include <string>
#include <set>
#include <vector>
using namespace std;

const int INSTANCETYPE_USER    = 0; //普通副本（个人）
const int INSTANCETYPE_TEAM    = 1; //普通副本（组队）

const int SYNMEMPOSUPDATE_INTERVAL = 300;  //更新军团成员位置间隔
const int MUSTKICKOUT_INTERVAL = 10*1000;  //强制踢人间隔10秒


struct StateItemSortInfo
{
	OBJID idUser;
	UINT  nAmount;
};

struct StateItemSortInfoLess: public binary_function<StateItemSortInfo&, StateItemSortInfo&, bool>
{
	bool operator()(const StateItemSortInfo& info1, const StateItemSortInfo& info2) const
	{
		return info1.nAmount < info2.nAmount;
	}
};

class CInstanceUser///记录副本中的玩家信息
{
public:
	CInstanceUser(){m_idUser=ID_NONE;m_nTeamGroup=0;m_dwSynMemUpdateTime=0;m_bKicked=false;}
	CInstanceUser(OBJID idUser){m_idUser=idUser;}
	CTimeOut& GetTeamLimitTimer(){return m_tTeamLimit;}
	CTimeOut& GetSynLimitTimer(){return m_tSynLimit;}
	OBJID	GetID(){return m_idUser;}
	void	SetID(OBJID idUser){m_idUser=idUser;}
	OBJID	GetTeamGroup(){return m_nTeamGroup;}
	void	SetTeamGroup(UINT nTeamGroup){m_nTeamGroup=nTeamGroup;}
	void    SetSynMemTime(DWORD dwTime) { m_dwSynMemUpdateTime = dwTime; }
	DWORD   GetSynMemTime()  { return m_dwSynMemUpdateTime; }
	bool	GetBeKickOut() { return m_bKicked;}
	void	SetBeKickout() { m_bKicked = true;}
	OBJID	m_idUser;
private:
	CTimeOut m_tTeamLimit;///离队定时器]
	CTimeOut m_tSynLimit;///退帮定时器
	UINT	 m_nTeamGroup;///分队标识,1,2,3,4,5
	DWORD    m_dwSynMemUpdateTime;
	bool	 m_bKicked;
};

typedef map<OBJID,CInstanceUser> InstanceUserList;	
typedef vector<OBJID> VECMEM;
typedef vector<OBJID>::iterator ITMEM;
typedef set<OBJID> setFreeUser;
typedef set<OBJID>::iterator FreeUserIt;

///softworms-副本类-2008-01-04

char	szDummyDataObjectTable[];
typedef	CGameData<LPCSTR, szDummyDataObjectTable, szDummyDataObjectTable>	CInstanceTypeData;///错误的模板方式,真是无聊,应该后面的参数应该变成函数参数.
typedef ISingleObjSet2<CInstanceTypeData> IInstanceTypeSet;
typedef	CSingleObjSet2<CInstanceTypeData>  CInstanceTypeSet;

class CDataObject;

class CMapInstance  : public has_slots<>
{
public:
	static const int InstanceTeam;
	static const int InstanceSyn;
	static const int InstanceCity;
	enum InstanceType
	{
		itTaskInstance=1,
		itRoomInstance=2,
	};
	enum InstaceStatus
	{
		isInit=1,
	};
	static CMapInstance* CreateMapInstance();///用工厂模式创建副本实例
public:
	virtual ~CMapInstance();
	bool	IsLeaveSave();
	BOOL	AddPlayer(OBJID idUser);
	void	EnterScript(OBJID idUser);
	BOOL	RemovePlayer(OBJID idUser);
	BOOL    CityWarBeKill(CUser* pUser);
	BOOL	ClearPlayerList();
	BOOL	IsActive();///判断副本是否过期,目前简单规则,人数和定时器;
	BOOL	IsTimeOut();///判断副本是否超时
	BOOL	IsRecycle();///判断副本是否回收,目前简单规则,人数和定时器;
	BOOL	InitMapInstance(int idProcess,PROCESS_ID pid,OBJID idUser,UINT nInstanceType,UINT nInstanceScope, UINT nDoMain=0);///初始化副本,可在这里面刷怪或做一些准备工作
	BOOL	ResetMapInstance();///重置副本
	BOOL	CloseInstance();///关闭副本,当生命周期结束或副本目标达成时调用
	BOOL    CloseCityWarInstance(bool bAttack); //准备关闭城战副本
	BOOL    CloseSynWarInstance(bool bAttack); //准备关闭帮战战副本
	void    ProcessSWTBeKill(bool bAttack, UINT type);  //帮战怪物被杀
	void	ShutdownInstance(UINT nTimeOut);///处理副本生命周期超时事件
	friend  class CMapInstanceMgr;///允许CMapInstanceMgr深度调用
	OBJID	GetType(){return m_pData->GetInt("type");}
	bool    IsAttackFull(int nAmount);
	bool    IsDefenceFull(int nAmount);
	int		GetLifeTime()
	{
		//如果策划没有在脚本里填写该值，直接从数据库读
		if(INSTANCE_EXPIRED_LIFETIME==0)
		{
			INSTANCE_EXPIRED_LIFETIME = m_pData->GetInt("lifetime");
		}

		return INSTANCE_EXPIRED_LIFETIME;
	}
//	void	SetType(OBJID idOwd){m_idOwn=idOwd;}
	OBJID	GetOwnID(){return m_idOwn;}
	void	SetOwnID(OBJID idOwd){m_idOwn=idOwd;}
	OBJID	GetInstanceType()
	{
		return m_pData->GetInt("id");
	}
//	void	SetInstanceType(UINT nInstanceType){m_nInstanceType=nInstanceType;}
	UINT	GetMapType(){return m_nMapType;}
//	void	SetMapType(UINT nMapType){m_idMapType=nMapType;}
	OBJID	GetMapID(){return m_idMap;}
	void	SetMapID(OBJID idMap){m_idMap=idMap;}
//	OBJID	GetNpcID(){return m_idNpc;}
//	void	SetNpcID(OBJID idNpc){m_idNpc=idNpc;}
	
	OBJID	GetOwnProcessID(){return m_idOwnProcess;}
	BOOL	IsEmpty();
	signal1<OBJID>	slotUserLeaveTeam;///用户在副本内离队事件
	signal1<OBJID>	slotUserRejoinTeam;///用户在副本内回队事件
	signal0<>		slotLifeTimeout;///副本生命周期超时事件
	signal0<>	slotInit;///副本初始化事件

	void  SetLeaveSynExpire(UINT nExpire);
	UINT  GetLeaveSynExpire();
	void  ResetInstanceLife(UINT nLiftTime);

	UINT	GetBornX() { return m_posRebornX;}
	UINT	GetBornY() { return m_posRebornY;}

	//向策划开放超时时间接口
	void SetNoUserExpire(UINT nExpire)
	{
		INSTANCE_EXPIRED_TIME= nExpire;
	}
	void SetRecycleExpire(UINT nExpire)
	{
		INSTANCE_EXPIRED_RECYCLETIME = nExpire;
	}
	void SetLeaveTeamExpire(UINT nExpire)
	{
		INSTANCE_EXPIRED_TEAMTIME = nExpire;
	}
	void SetInstanceLife(UINT nLiftTime)
	{
		ResetInstanceLife(nLiftTime);
	}

	void SetInstanceActiveOut(UINT nLiftTime)
	{
		m_tTimeActiveOut.Startup(nLiftTime);
		m_bAllowIn = false;
	}

	void SetUserMaxNumbers(UINT MaxNumbers)
	{
		INSTANCE_USER_MAX_NUMBERS = MaxNumbers;
	}

	void SetNewGenerageInterval(UINT timeValue)
	{
		INSTANCE_NEWGENERATE_TIME = timeValue;
	}

	void SetUserMinNumbers(UINT MinNumbers)
	{
		INSTANCE_USER_MIN_NUMBERS = MinNumbers;
	}

	void SetCountDownInstance(UINT nCountDownInstance)
	{
// 		INSTANCE_COUNTDOWN_TIME = nCountDownInstance;
// 		m_tCountDownInstance.Startup(GetLifeTime()-INSTANCE_COUNTDOWN_TIME);
	}
	
	//城战副本相关设置
	void SetDomainMapID(UINT nDomainMapID)
	{
		m_nDomainMapID = nDomainMapID;
	}

	void SetUserLev(UINT nUserLev)
	{
		m_nUserLev = nUserLev;
	}

	void SetAttackSynID(UINT nAttackSynID)
	{
		m_nAttackSynID = nAttackSynID;
	}

	void SetDefenceSynID(UINT nDefenceSynID)
	{
		m_nDefenceSynID = nDefenceSynID;
	}

	void SetInstanceScope(UINT nInstanceScope)
	{
		m_nInstanceScope = nInstanceScope;
	}
	UINT GetInstanceScope()
	{
		return m_nInstanceScope;
	}
	//对应的GET函数，不向策划开放
	UINT GetNoUserExpire()
	{
		return INSTANCE_EXPIRED_TIME;
	}
	UINT GetRecycleExpire()
	{
		return INSTANCE_EXPIRED_RECYCLETIME;
	}
	UINT GetLeaveTeamExpire()
	{
		return INSTANCE_EXPIRED_TEAMTIME;
	}
	UINT GetInstanceLife()
	{
		return INSTANCE_EXPIRED_LIFETIME;
	}

	UINT GetInstanceNewGenerage()
	{
		return INSTANCE_NEWGENERATE_TIME;
	}

	UINT GetUserMaxNumbers()
	{
		return INSTANCE_USER_MAX_NUMBERS;
	}

	UINT GetUserMinNumbers()
	{
		return INSTANCE_USER_MIN_NUMBERS;
	}

	UINT CreateInstanceNpc(UINT nNpcType,int cellx,int celly);	//创建副本NPC
	UINT CreateChessMonsters(UINT nMonsterTypeA,int nMonsterACount,UINT nMonsterTypeB,int nMonsterBCount,int StartX,int StartY,int nMonsterWidth);
	UINT SetRebronPos(UINT posX,UINT posY);

	void CreateMonsterInInstance(UINT nMonsterType,int cellx,int celly);	//在副本里创建指定类型的怪，类似GM刷怪命令
    BOOL DeleteMonster(int nType);
	bool DeleteNpc(UINT nNpcType);

	void RegisterLuaInterface();
	void RegisterLuaInterfaceOntime(int nAction);

	BOOL IsInstanceUserFull();

	void SendInstanceMsg(const char* pMsg);				//给副本里面的所有玩家发送消息

	void MoveMonsterDest(UINT ntype,int x,int y);

	void SetOutInstancePos(UINT idMap,int cellx,int celly);

	void SetInstanceRebornPos(int attackPosX, int attackPosY, int defencePosX, int defencePosY, int nType);
	BOOL CanEnterInstance();
// 	void SynWarTargetBekill(bool bBoss, bool bAttack); //帮战目标被杀

	//zeng清除副本里面的玩家,方便回收副本
	void ClearUser();

	bool BuyBeDestroy();
	bool ChgMapBeDestroy();
	bool TaskBeDestroy();
	bool HasMonster(int nType,int nCheckAmount = 1);
	bool HasMonster2(int nType,int nCheckAmount);
	BOOL ActiveKickAllOut();

	void addFreeUser(OBJID idUser);
	bool IsFreeUser(OBJID idUser);
	//竞技场相关函数
	void RequestEnterArena(const string& TeamName,UINT UserID);
	void EnterArena(const string& TeamName,UINT UserID);
	bool IsEndArena() {return m_bArenaEnd;}
	bool OperateData(int nIndex,const char* szParam);
	bool SetInstanceTimer(int nAction,UINT nPassSec);
	int	 GetOperateData(int nIndex);
	void SetMonsterKillEvent(OBJID nMonsterType,UINT nMonsterCount,UINT nContext);
protected:
	void   OnMonsterBeKillEvent(UINT nMonsterType);
	void   OnMonsterBeKill(OBJID idMonster);
	BOOL	RunScript(InstaceStatus isStatus);
	BOOL	RunScriptOntime(int nAction);
	BOOL    RunScriptMonsterKill(int nAction);
	void	GeneratMonster(UINT nMonsterType,UINT nPosX,UINT nPosY);
	void    NewGenerat(OBJID idGen);
	BOOL	RemoveInstance();///清除副本,和InitMapInstance相对
	void	NotifyOwnDestory();///通知副本拥有者副本已关闭
	BOOL	CreateInstanceMap();///为副本创建它所拥有的地图,向地图管理器发出请求
	void	OnLifeTimeout();///处理副本生命周期超时事件
	void	OnInit();///处理副本生命周期超时事件
	void	OnUserQuitTeam(OBJID idUser);///处理玩家离队事件
	void	OnUserRejoinTeam(OBJID idUser);///处理玩家回队事件
	BOOL	IsUserLevelTeam(CUserPtr UserPtr);
	BOOL	RemoveInstanceMap();///删除副本创建它所拥有的地图,向地图管理器发出请求
	BOOL	KickOutAllPlayer();///踢出所有玩家
	BOOL	KickOutPlayer(OBJID idPlayer,OBJID idMap=3001, int nPosX=200, int nPosY=300);///踢出指定玩家
	BOOL	ProcessAllPlayerEvent();///处理所有玩家在副本中的事件
	BOOL	ProcessInstanceEvent();///处理副本自身事件
	void	ProcessMonsterEvent();///处理怪物事件
	BOOL	ProcessPlayerEvent(CInstanceUser& userptr);//////处理单个玩家事件
	BOOL	IsUserInstanceTeam(CUserPtr UserPtr);///判断该用户是否属于副本队伍中
	void	CloseMonsterGenerat(OBJID idGen);///关闭副本中的GEN
	CMapInstance();
	void	OnTimer(time_t tCurr);///定时器处理函数
	void    StartUpNewGen(UINT timeVal);//开启刷怪时间段


    BOOL    PvpSample(const char *pText);
	bool    CitySynWarJoin(OBJID idUser, int nType);   //帮战，城战成员进入副本
	bool    SynWarAward(bool bAttack);                 //帮战奖励发放
	void    AddAttackPlayer(OBJID idPlayer);
	void    AddDefencePlayer(OBJID idPlayer);
	void    ClearFreeUser();

	int     instanceData[2];
	int     m_nActionForTimer;
private:
	CInstanceTypeData * m_pData;
//	OBJID	m_idInstance;
//	UINT	m_nInstanceType;///留做扩展
//	UINT	m_nInstanceStatus;///留做扩展
	OBJID	m_idMap;///副本所在地图id
//	OBJID	m_idNpc;///副本所在地图类型id
	OBJID	m_idOwn;///所有者编号,可能是队伍ID或是playerid
//	UINT	m_nExpireTime;///回收周期有效时间,过期将删除该副本
//	UINT	m_nLifeTime;///副本的生命周期,过期将所有玩家传出副本,删除该副本
//    typedef vector<OBJID> PlayerList;	
//    typedef map<OBJID,CTimeOut> TeamLimitTimerList;	
//	PlayerList m_PlayerList;
	InstanceUserList m_InstanceUserList;
	VECMEM   m_vecAttackMember;
	VECMEM   m_vecDefenceMember;
	CTimeOut m_tForDelay;
	CTimeOut m_tExpiredInstance;
	CTimeOut m_tLifeTimeInstance;
	CTimeOut m_tRecycleInstance;
	CTimeOut m_tCountDownInstance;
	CTimeOut m_tLifeTimeNewGenerage;
	CTimeOut m_tLifeCityWarVictory;
	CTimeOut m_tTimeActiveOut;
	
	int		m_idProcess;//地图组ID
	int		m_idOwnProcess;//拥有者地图组ID
	UINT	m_InstanceType;

	
	UINT	m_posRebornX;
	UINT	m_posRebornY;
//private: // ctrl
//	MYHEAP_DECLARATION(s_heap)
	typedef struct NPCINFO {
		UINT		NpcType;
		CNpc*		pNpc;
		NPCINFO(){}
		NPCINFO(UINT npctype,CNpc* pnpc){NpcType = npctype;pNpc = pnpc;}
	}NPCINFO;
	map<UINT,NPCINFO>	m_InstanceNpcs;
//	vector<OBJID>       m_Monster;
	map<OBJID,UINT>		m_MonsterCount;///副本怪物数量表
	setFreeUser			m_SetFreeUser;
//	BOOL m_bSetExpiredTime;
	

	//玩家传出副本的位置
	UINT	m_OutMapID;
	int		m_OutCellX;
	int		m_OutCellY;

	UINT    m_nMapType;  //地图类型
	UINT    m_nUserLev;

	//副本内重生坐标
	int     m_nRebornAttackX;
	int     m_nRebornAttackY;
	int     m_nRebornDefenceX;
	int     m_nRebornDefenceY;

	//zeng修改，写的太死了，策划不能设置这些时间，这是不行的，必须提供LUA接口供策划使用
	UINT INSTANCE_EXPIRED_TIME;///超时时限秒
	UINT INSTANCE_EXPIRED_TEAMTIME;///超时离队容忍时限秒
	UINT INSTANCE_EXPIRED_LIFETIME;///超时离队容忍时限秒
	UINT INSTANCE_EXPIRED_RECYCLETIME;///超时离队容忍时限秒
// 	UINT INSTANCE_COUNTDOWN_TIME;		//倒计时秒数，默认设定为120秒
	UINT INSTANCE_USER_MAX_NUMBERS;//副本中玩家个数限制上限
	UINT INSTANCE_USER_MIN_NUMBERS;//副本中玩家个数限制下限

    UINT INSTANCE_NEWGENERATE_TIME;//副本中刷怪时间段

	UINT m_nLeaveSynTime;           //退帮容忍时限
	UINT m_nCityWarVictoryTime;           //城战胜利倒计时
	
	//为支持军团副本和城战副本添加的
	int  m_nInstanceScope;			//副本规模
	
	UINT m_SynID;					//军团副本ID

	UINT m_nDomainMapID;			//可以占领的区域
	UINT m_nAttackSynID;			//攻方军团ID
	UINT m_nDefenceSynID;			//防守方军团ID

	OBJID m_nAttackBossID;          //城战攻方boss
	OBJID m_nDefenceBossID;         //城战守方boss

	UINT  m_nLimitCityWar;          //城战人数限制

	BOOL m_CityWarClose;
	BOOL m_CityMsg;

//	vector<CityWarNPCInfo> m_vecCityWarNpc;
    DWORD m_dwKickPlayerInterval;   //强制踢人间隔
	int  m_nSWABCount;
	int  m_nSWDBCount;
	UINT m_nSWBeKillMark;           //帮战怪物被杀标记

	//竞技场

	map<string,set<UINT> >			m_RequstUserMap;
	map<string,vector<UINT> >		m_EnterArenaUserMap;
	CTimeOut						m_ArenaBegin;
	bool							m_bArenaEnd;
	UINT							m_TeamType;
	OBJID m_nMonsterTypeEvent;///怪物类型
	UINT  m_nMonsterCountEvent;///怪物数量
	UINT  m_nMonsterEventContext;///怪物事件上下文
	bool  m_bAllowIn;
	bool  m_bNeedKickout;
};

//typedef CDataObjectSet CInstanceTypeData;
#endif // !defined(AFX_MAPINSTANCE_H__1CB83F41_11BA_4625_B1B8_249123985025__INCLUDED_)
