// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKNPC_H__D7A2DFC3_2C2C_4F67_BB52_8F55409E76BC__INCLUDED_)
#define AFX_TASKNPC_H__D7A2DFC3_2C2C_4F67_BB52_8F55409E76BC__INCLUDED_

#include "MyHeap.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "NpcShop.h"
#include "NpcStorage.h"
#include "NpcTable.h"
#include "Myheap.h"
#include "Role.h"
#include "AutoPtr.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUser;
class CGameMap;

/////////////////////////////////////////////////////////////////////////////////////
//define constant ..
const	int	ADD_NEW_NPC				= 0;
const	int LAY_NEW_NPC				= 10;
const	int PRE_CHANGE_POSITION		= 11;
const	int CHANGE_POSITION			= 12;
const	int DYNANPCDIEDELAY_SECS	= 3;		// DYNA_NPC死亡多少秒后删除
const	int	GOAL_EXP_PERCENT		= 10;		// 打靶子的经验值百分比(按次的经验值是5%的机会)
const	int	SYNFLAG_SIZEADD			= 5;		// 帮派柱子的附加尺寸
const	int	SYNWAR_GAINMONEY_PERCENT			= 10;		// 帮派战得到对方基金的比例
const	int	DEFAULT_NPC_SIZEADD			= 4;		// 每个方面加两格
const	int SYNWARLETTERNPC_CHECKINTERVAL = 60*20;//每间隔20分钟检查占领帮派是否到了7周
/////////////////////////////////////////////////////////////////////////////////////
const	int	_MAX_NPCTASK			=8;
const	int	MAX_NPCDATA				=4;		// 必须小于_MAX_NPCTASK

/////////////////////////////////////////////////////////////////////////////////////
// 程序使用的DATA情况
enum {
		SYNFLAGNPC_FIXFUND=0,SYNFLAGNPC_FIGHTBEGIN,SYNFLAGNPC_FIGHTEND,SYNFLAGNPC_MOREMAPID,					// 帮派标志(time:WWHHMMSS, sun day = 7)
		SYNWARLETTER_ISATTACK=0,SYNWARLETTER_ATTACK_SYNID=1,SYNWARLETTER_OCCUPYTIME=2,SYNWARLETTER_DEPOSIT=3,
		SYNDRESS_SORT=0,SYNDRESS_SETTING=1,
		GOALNPC_LEVEL=3,
		TIMERNPC_TIME=3,
		TRUNKNPC_SIZE=3,
		EUDEMON_BROODER_NPC_SIZE=2, EUDEMON_STORAGE_NPC_SIZE=3,
		STORAGENPC_SIZE=3,
		NEIGHBORNPC_X=1,NEIGHBORNPC_Y=2,NEIGHBORNPC_SERIAL=3,
};			//? 目前一组最多支持4个变量

/////////////////////////////////////////////////////////////////////////////////////
enum NPCTYPE{
		_NPC_NONE				= 0,			// 通用NPC
		_SHOPKEEPER_NPC			= 1,			// 商店NPC
		_TASK_NPC				= 2,			// 任务NPC(已作废，仅用于兼容旧数据)
		_STORAGE_NPC			= 3,			// 寄存处NPC
		_TRUNCK_NPC				= 4,			// 箱子NPC
		_FACE_NPC				= 5,			// 变头像NPC
		_FORGE_NPC				= 6,			// 锻造NPC		(only use for client)
		_EMBED_NPC				= 7,			// 镶嵌NPC
		_STATUARY_NPC			= 9,			// 雕像NPC
		_SYNFLAG_NPC			= 10,			// 帮派标记NPC
		_ROLE_PLAYER			= 11,			// 其他玩家		(only use for client)
		_TASK_NPC_NEW			= 12,			// 自己			(only use for client)
		_ROLE_MONSTER			= 13,			// 怪物			(only use for client)
		_BOOTH_NPC				= 14,			// 摆摊NPC		(CBooth class)
		_SYNTRANS_NPC			= 15,			// 帮派传送NPC, 固定那个不要用此类型! (用于00:00收费)(LINKID为固定NPC的ID，与其它使用LINKID的互斥)
		_ROLE_BOOTH_FLAG_NPC	= 16,			// 摊位标志NPC	(only use for client)
		_ROLE_MOUSE_NPC			= 17,			// 鼠标上的NPC	(only use for client)
		_ROLE_MAGICITEM			= 18,			// 陷阱火墙		(only use for client)
		_ROLE_DICE_NPC			= 19,			// 骰子NPC
		_ROLE_SHELF_NPC			= 20,			// 物品架
		_WEAPONGOAL_NPC			= 21,			// 武器靶子NPC
		_MAGICGOAL_NPC			= 22,			// 魔法靶子NPC
		_BOWGOAL_NPC			= 23,			// 弓箭靶子NPC
		_ROLE_TARGET_NPC		= 24,			// 挨打，不触发任务	(only use for client)
		_ROLE_FURNITURE_NPC		= 25,			// 家具NPC	(only use for client)
		_ROLE_CITY_GATE_NPC		= 26,			// 城门NPC	(only use for client)
		_ROLE_NEIGHBOR_DOOR		= 27,			// 邻居的门
		_ROLE_CALL_PET			= 28,			// 召唤兽	(only use for client)
		_EUDEMON_TRAINPLACE_NPC	= 29,			// 幻兽驯养所
		 _AUCTION_NPC			= 30,			// 拍买NPC	物品领取NPC  LW
		 _ROLE_MINE_NPC			= 31,			// 矿石NPC
		 
		 _ROLE_GRASS_MINE       = 32,           // 草矿
		 _ROLE_PET_MINE         = 33,           //动物尸体矿
         _ROLE_BOX_MINE         = 34,           //宝箱

		 //探索系任务
		_ROLE_TASK_HAND         = 35,  //手   
		_ROLE_TASK_SWORD        = 36,  //利剑
	    _ROLE_TASK_CHU          = 37,  //锄头
		_ROLE_TASK_SKIFE        = 38,  //小刀
		_ROLE_TASK_CHOPPER     = 39,  //斧头
		_ROLE_TASK_KEY          = 40,  //钥匙
		_ROLE_TASK_SAW          = 41,  //锯子
		_ROLE_TASK_HAMMAER      = 42,  //锥子
		_ROLE_TASK_DUKE         = 43,  //拳头
		_ROLE_TASK_BARK         = 44,  //鱼钩

		 _EUDEMON_EVO_NPC		= 32,			// 幻兽进化NPC
		 
		 _ROLE_3DFURNITURE_NPC	= 101,			// 3D家具NPC 
		 
		 _SYN_NPC_WARLETTER     = 110,			//增加新的ＮＰＣ类型　专门用来　下战书的　帮派ＮＰＣ
};

/////////////////////////////////////////////////////////////////////////////////////
enum NPCSORT{
		NPCSORT_NONE		= 0,
		NPCSORT_TASK		= 1,			// 任务类
		NPCSORT_RECYCLE		= 2,			// 可回收类
		NPCSORT_SCENE		= 4,			// 场景类(带地图物件)
		NPCSORT_LINKMAP		= 8,			// 挂地图类(LINKID为地图ID，与其它使用LINKID的互斥)
		NPCSORT_DIEACTION	= 16,			// 带死亡任务(LINKID为ACTION_ID，与其它使用LINKID的互斥)
		NPCSORT_DELENABLE	= 32,			// 可以手动删除(不是指通过任务)
		NPCSORT_EVENT		= 64,			// 带定时任务, 时间在data3中，格式为MMWWHHMMSS。(LINKID为ACTION_ID，与其它使用LINKID的互斥)
		NPCSORT_TABLE		= 128,			// 带数据表类型

//		NPCSORT_SHOP		= ,			// 商店类
//		NPCSORT_DICE		= ,			// 骰子NPC

		NPCSORT_USELINKID	= NPCSORT_LINKMAP | NPCSORT_DIEACTION | NPCSORT_EVENT,
		NPCSORT_DYNANPC_INSTANCE     = 256,			//副本专用NPC
		
};

/////////////////////////////////////////////////////////////////////////////////////
struct NPCINFO_ST{
	OBJID	idOwner;
	int		nOwnerType;
	OBJID	idMap;
	int		nPosX;
	int		nPosY;
	int		nType;
	int		nLookFace;
	DWORD	nAction;
	int		nSort;
	UINT    nNpcType;
	USHORT	nLev;
};

/////////////////////////////////////////////////////////////////////////////////////
enum NPCDATA{
	    NPCDATA_ID,
		NPCDATA_NAME = 1,
		NPCDATA_LOOKFACE, STATUARYDATA_LOOKFACE = NPCDATA_LOOKFACE,
		NPCDATA_MAPID,
		NPCDATA_POSX_,		// 外部不要直接使用，注意与m_nPosX的同步
		NPCDATA_POSY_,		// 外部不要直接使用，注意与m_nPosY的同步
		NPCDATA_SORT,
		NPCDATA_TYPE,
		NPCDATA_ACTION,
		NPCDATA_LEVEL,
		NPCDATA_SHOWTIME,
		NPCDATA_HIDETIME,
		NPCDATA_SHOWSPAN,
		NPCDATA_HIDEMSPAN,
};

typedef	IGameData<NPCDATA>	INpcData;
char	szNpcTable[];
typedef	CGameData<NPCDATA,szNpcTable,szID>	CNpcData;
char	szDynTable[];
typedef	CGameData<NPCDATA,szDynTable,szID>	CDynaNpcData;

/////////////////////////////////////////////////////////////////////////////////////
//class CNpcTrunk;
// class CNpcTask;
// class CNpcScene;
// class CNpcLink;
// class CNpcBet;
class CNpc : public CGameObj, public CRole
{
protected:
	CNpc();
	virtual ~CNpc();
public:
	static CNpc* CreateNew()								{ return new CNpc; }
	bool	CreateSynMapNpc(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName);
	bool	Create(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName);
	bool	Create(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName,CGameMap* pGameMap);
	bool	Create(PROCESS_ID idProcess, IRecordset* pRecordset, bool bDynaNpc);
	IRole*			QueryRole()									{ return (IRole*)this; }
	virtual		bool		FindAroundObj	(OBJID idObjType, OBJID idObj, void** pp)		{ return GetMap()->QueryObj(GetPosX(), GetPosY(), idObjType, idObj, pp); }
public: // IRole ================================================================================
	virtual void	ReleaseByOwner()							{ delete this; }
	virtual OBJID	GetID()										{ return m_id; }
	virtual int 	GetObjType()								{ return CGameObj::GetObjType(); }
	virtual int		GetPosX()									{ return m_nPosX; }//m_pData->GetInt(NPCDATA_POSX); }
	virtual int		GetPosY()									{ return m_nPosY; }//m_pData->GetInt(NPCDATA_POSY); }
	virtual void	SendShow(IRole* pRole);							// 向玩家发送物件外形数据

public: // get attrib
	virtual LPCTSTR	GetName				();
//	virtual LPCTSTR	GetMate				()						{ return NOMATE_NAME; }

	virtual DWORD	GetLookFace			()						{ return m_pData->GetInt(NPCDATA_LOOKFACE); }
	virtual DWORD	GetHair				()						{ return /*m_pData->GetInt(STATUARYDATA_RANKSHOW_HAIR)&0xFFFF*/0; }
	virtual int 	GetDir				()						{ return /*MaskDir(m_pData->GetInt(NPCDATA_LOOKFACE))%MAX_DIRSIZE*/2; }
	virtual I64		GetEffect			()						{ return m_i64Effect; }
	virtual DWORD	GetLev				()						{ return m_pData->GetInt(NPCDATA_LEVEL); }

	virtual DWORD	GetLife				()						{ return /*m_pData->GetInt(NPCDATA_LIFE)*/100; }
	virtual DWORD	GetMaxLife			()						{ return /*m_pData->GetInt(NPCDATA_MAXLIFE)*/100; }

	virtual DWORD	GetAtkHitRate		()						{ ASSERT(!"GetAtkHitRate"); return 0; }
	virtual DWORD	GetAddAttByType     (int nType)				{ return 0; }
	virtual DWORD	GetAtkType			()						{ return 0; }

	virtual int		GetArmorTypeID		();
	virtual int		GetWeaponRTypeID	();
	virtual int 	GetWeaponLTypeID	();
	virtual int 	GetMountTypeID		();
	virtual int		GetWeaponSpecialView()						{ return 0; }

//	virtual CGameMap*	GetMap			()						{ CHECKF(m_pMap); return m_pMap; }

	virtual DWORD	GetSynID			()						{ return /*m_pData->GetInt(NPCDATA_OWNERTYPE)==OWNER_SYN ? m_pData->GetInt(NPCDATA_OWNERID) : ID_NONE*/0; }
	virtual DWORD	GetSynRankShow		()						{ return /*m_pData->GetInt(STATUARYDATA_RANKSHOW_HAIR)>>16*/0; }

public: // const
	virtual bool	IsAlive				()						{ return /*!(IsDynaNpc() && m_pData->GetInt(NPCDATA_MAXLIFE)>0 && m_pData->GetInt(NPCDATA_LIFE)<=0 ||*/ !IsDeleted()/*)*/; }		// NPCDATA_LIFE is a flag
	virtual bool	IsDelThis			()						{ return IsDeleted() && m_tDie.IsTimeOut(); }		// ASSERT(!IsAlive()); 
	virtual BOOL	IsNpc				()						{ return true; }

	// return true: ppv返回对象指针
	virtual bool	QueryObj(OBJID idObjType, void** ppv)		{ if(idObjType==OBJ_NPC) return *ppv=this,true; return *ppv=NULL,false; }
	virtual bool	QueryRole(void** ppv)						{ return *ppv=QueryRole(),true; }
	virtual CUser*	QueryOwnerUser		()						{ return NULL; }		//@@@ 略
	virtual void    ClsBuff				(int nStatus)			{}
public:	// modify attrib ------------------------------
	virtual	void	SetStatus			(int nStatus, bool bSynchro=true, int nPower=0);
	virtual	void	ClsStatus			(int nStatus, bool bSynchro=true);
	virtual bool	AddAttrib			(int idxAttr, __int64 i64Data, int bSynchro);
	virtual void	ProcessOnMove		(int nMoveMode)						{}

	virtual	IStatus*	QueryStatus(OBJID idType)
	{
		return CRole::QueryStatus(idType);
	}
	virtual	IStatusSet*	QueryStatusSet()									
	{
		return CRole::QueryStatusSet();
	}

public: // money --------------------------------
	virtual bool	InitNpc(int nParam1,int nParam2){ return true;}
public: // system
	virtual void	BroadcastRoomMsg(CNetMsg* pMsg, 
		bool bSendSelf = false,
		EM_SENDNPCFLAG sendNpc = EM_SENDNPCFLAG_TRUE);
	//-------------------------------------
	virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false);
	virtual void	OnTimer(time_t tCurr);

public: // map
	virtual int		MoveToward(int nDir)									{ return 0; }
	virtual void	TransPos(int nPosX, int nPosY);
	virtual void	JumpPos(int nPosX, int nPosY)							{}
	virtual bool	SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction)		{ return 0; }
	virtual void	GetFootPrint	(int& nPosX, int& nPosY)				{ nPosX = GetPosX(); nPosY = GetPosY(); }

public: // fight ------------------------------
	virtual void	ClrAttackTarget		()									{ SetAttackTarget(NULL); }
	virtual bool	SetAttackTarget(IRole* pTarget)							{ return true; }
	virtual bool	AutoSkillAttack(IRole* pTarget)							{ return false; }
	virtual int		Attack( IRole* pTarget,DWORD& dwBattleAddInfoRef )		{ ASSERT(!"Attack"); return 0; }
	virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable=true);
	virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y,bool needLearn = false,int learnLev = 0)	{ ASSERT(!"MagicAttack"); return 0; }
	virtual void	Kill(IRole* pTarget, DWORD dwDieWay)					{ ASSERT(!"Kill"); }
	virtual void	BeKill(IRole* pTarget = NULL);
	virtual bool	IsAtkable(IRole* pTarget, bool bSendHint=false)			{ return false; }
	virtual bool	IsBeAtkable();
	virtual int		GetAttackRange(int nTargetSizeAdd)						{ ASSERT(!"GetAttackRange"); return 0; }
	virtual int		GetSizeAdd()											{ if(IsSynFlag()) return SYNFLAG_SIZEADD; return DEFAULT_NPC_SIZEADD; }
	virtual void	SendDamageMsg( OBJID idTarget,int nDamage,DWORD dwBattleAddInfo )		{ ASSERT(!"SendDamageMsg"); }
private: // IRole ================================================================================
//	int			m_nPosX, m_nPosY;

public: // const
	int 			GetSort()									{ return m_pData->GetInt(NPCDATA_SORT); }
	int 			GetType()									{ return m_pData->GetInt(NPCDATA_TYPE); }
	void			ChangeDir(int nLook)						{ m_pData->SetInt(NPCDATA_LOOKFACE, MakeLookFace(MaskLook(GetLookFace()), MaskDir(nLook))); }
	bool			IsRecycleEnable()							{ return (GetSort() & NPCSORT_RECYCLE) != 0; }
	enum { MATE_ENABLE=true };
	bool			IsOwnerOf(CUser* pUser, bool bMateEnable=false);
	bool			IsDieAction()								{ return (GetSort() & NPCSORT_DIEACTION) != 0; }
	bool			IsStatuary()								{ return GetType() == _STATUARY_NPC; }		// 自动设置CGameMap::m_idSyn
	bool			IsGoal()									{ return GetType()==_WEAPONGOAL_NPC || GetType()==_MAGICGOAL_NPC || GetType()==_BOWGOAL_NPC; }
	bool			IsSynFlag()									{ return GetType() == _SYNFLAG_NPC; }
	bool			IsAwardScore()								{ return GetType() == _SYNFLAG_NPC; }
	bool			IsDelEnable();
	bool			IsSynNpc()									{ return /*GetInt(NPCDATA_OWNERTYPE)==OWNER_SYN && GetInt(NPCDATA_OWNERID)!=ID_NONE*/false; }
	bool			IsUserNpc()									{ return /*GetInt(NPCDATA_OWNERTYPE)==OWNER_USER && GetInt(NPCDATA_OWNERID)!=ID_NONE*/false; }
	int 			GetStorageSize()							{ return /*GetData(STORAGENPC_SIZE)*/0; }
	bool			IsSynMoneyEmpty();
	bool			IsShelfNpc()								{ return /*m_pData->GetInt(NPCDATA_TYPE) == _ROLE_SHELF_NPC*/0; }
	bool			IsSynWarLetter()							{ return GetType() == _SYN_NPC_WARLETTER; }//是否是下战书的ＮＰＣ
	
public: // application
	bool			SendSelfToBlock();
	bool			SendLeaveFromBlock();
	bool			ActivateNpc(IRole* pRole, int nAction);			// 对物件进行操作
	bool			ActivateShop(IRole* pRole);
	CItem*			Recycle(OBJID idUser);			//??? need call DelNpc() follow
	bool			DelNpc();						// call this mast !IsDeleted()
	bool			EnterMapGroup();
protected:
	bool			DeleteRecord()								{ CHECKF(IsDynaNpc()); return m_pData->DeleteRecord(); }
	void			CheckFightTime();
	void			RecruitLife();
	void			BroadcastScore();

public:
	//---------- NPC 属性存取函数 --------------------------
	int				GetInt		(NPCDATA idx)					{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR			GetStr		(NPCDATA idx)					{ CHECKF(m_pData); return m_pData->GetStr(idx); }
	OBJID			GetMapID	();

	//---------- 修改NPC属性 ------------------
	void			SetInt		(NPCDATA idx, int nData, bool bUpdate = false)	{ CHECK(m_pData && IsDynaNpc()); m_pData->SetInt(idx, nData); if(bUpdate) m_pData->Update(); }
	void			SetStr		(NPCDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false)	{ CHECK(m_pData && IsDynaNpc()); m_pData->SetStr(idx, szData, nSize); if(bUpdate) m_pData->Update(); }
/*	void			SetData		(int idx, int nData, BOOL bUpdate)	{ 
										CHECK(m_pData && IsDynaNpc()); 
										CHECK(idx < MAX_NPCDATA); 
										m_pData->SetInt(NPCDATA(NPCDATA_DATA0 + idx), nData); 
										if(bUpdate) m_pData->Update(); }*/
	void			SaveInfo	();
	BOOL			ChangePos		(OBJID idMap, int nPosX, int nPosY);
	bool			SetSynOwnerID(OBJID idSyn, BOOL bWithLinkMap);		// true: with link map
	bool			SetUserOwnerID(OBJID idUser, BOOL bWithLinkMap);		// true: with link map

public: // 分类接口 ------------------------------------------------------------------	
	virtual bool		IsShopNpc()							{ return m_pData->GetInt(NPCDATA_SORT) == _SHOPKEEPER_NPC; }		// || (m_pData->GetInt(NPCDATA_SORT)&NPCSORT_SHOP)!=0
	virtual bool		IsTaskShopNpc()						{ return ( m_pData->GetInt(NPCDATA_SORT) == _TASK_NPC || m_pData->GetInt(NPCDATA_SORT) == _TASK_NPC_NEW) && (m_pData->GetInt(NPCDATA_TYPE) == 1 );  }
	virtual bool		IsRandTaskShopNpc()					{ return ( m_pData->GetInt(NPCDATA_SORT) == _TASK_NPC || m_pData->GetInt(NPCDATA_SORT) == _TASK_NPC_NEW) && (m_pData->GetInt(NPCDATA_TYPE) == 2 );  }
	virtual CNpcShop*	QueryShop()							{ CHECKF(m_pShop); return m_pShop; }

	OBJID				GetRecordID()						{ CHECKF(m_pData); return m_pData->GetID(); } 
	bool				IsDeleted()							{ return m_tDie.IsActive(); }

	bool				IsDynaNpc()							{ return m_bDynaNpc; }
	bool                IsTimeDuring(unsigned long nBegin, unsigned long nEnd, time_t tCurr/* =0 */);
private:
	bool				CreateFuncObj(PROCESS_ID idProcess);
	bool				CreateFuncTrunk(PROCESS_ID idProcess);
	bool				CreateFuncEudemonBrooder(PROCESS_ID idProcess);
	bool				CreateFuncEudemonStorage(PROCESS_ID idProcess);
	bool				CreateFuncEudemonEvo(PROCESS_ID idProcess);
	bool				CheckSortMutex();
	void				UpdateHoldFloor(bool bEnter);
public:
    void                SetNpcOwner(OBJID idUser);
	OBJID               GetNPcOwner(){return m_nToken;}
	bool                IsActive(unsigned long nBegin);//是否激活
	bool                MovePos(int posX,int poxY);   //Npc移动
	UINT                GetNpcType()  { return m_nNpcType; };
	bool				IsShow();
	void				SetDelShow();
protected:
	CNpcShop*			m_pShop;

protected: // data
	OBJID			m_id;
	INpcData*		m_pData;
	bool			m_bDynaNpc;
	I64				m_i64Effect;
public:
   clock_t         m_beginTime;
   int             m_startPosX;
   int             m_startPosY;
protected: // ctrl
	CTimeOut		m_tSynEvent;
	CTimeOut		m_tBroadcastScore;
	CTimeOut		m_tDie;
	CTimeOut		m_tSynWarLetter;//每间隔20分钟检查占领帮派是否到了7周
	CTimeOut		m_tUpdateShow;
	bool			m_bFeeOK;

	bool            m_bTimeDuring; //是否在刷新时间间隔之内
	bool            m_bNPCShow;
	bool            m_bSetShow;
protected:
	OBJID           m_nToken;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TASKNPC_H__D7A2DFC3_2C2C_4F67_BB52_8F55409E76BC__INCLUDED_)
