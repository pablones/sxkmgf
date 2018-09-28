// Role.h: interface for the CRole class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROLE_H__A27D6633_2132_4F63_AA77_4EB7740D2D67__INCLUDED_)
#define AFX_ROLE_H__A27D6633_2132_4F63_AA77_4EB7740D2D67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include <list>
#include <windows.h>
#include "common.h"
#include "../share/define.h"
#include "I_Role.h"
#include "gameMap.h"

#include <algorithm>
using namespace std;

#include "StackWalker.h"
//////////////////////////////////////////////////////////////////////////
//20080105修罗:ENMITY.nEnmAddi的上下限
const int _MAX_ENMITYADDITION = 1000*1000*1000;//10Y
const int _MIN_ENMITYADDITION = 1000*1000*1000*(-1);//-10Y


//  [2007-8-30] 朱斌 移到外面来
//20070209修罗:从CMonster拿过来----------------
//仇恨表
typedef struct
{
	OBJID userID;
	int	  enmity;
	DWORD lastUpdateTime;//最后更新时间
	int	  nEnmAddi;	//仇恨加权.数值型.仅用做AI.分配物品与此加权无关
	int   nMax;     //用于嘲讽技能
}ENMITY,*LPENMITY;
typedef vector<ENMITY> SET_ENMITY;

struct MAPITEMFINDINFO
{
	OBJID itemID;
	int	  x;	
	int   y;     
};

typedef std::list<MAPITEMFINDINFO>	BROADCASTMAPITEM_SET;

//////////////////////////////////////////////////////////////////////
class CRole : public IRole, public IMapThing
{
protected:
	CRole();
	virtual ~CRole();



public: // get attrib
	virtual OBJID	GetID				()						PURE_VIRTUAL_FUNCTION_0
	virtual LPCTSTR	GetName				()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetLookFace			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetHair				()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetPosX				()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetPosY				()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetDir				()						PURE_VIRTUAL_FUNCTION_0
	virtual I64		GetEffect			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetLev				()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetLife				()						{ return 0; }
	virtual DWORD	GetMaxLife			()						{ return 0; }

	virtual DWORD	GetAtk			()						{ return 0; }
	virtual DWORD	GetMgcAtk		()						{ return 0; }
	virtual DWORD	GetAttack			()						{ return 0; }
	virtual DWORD	GetAtkHitRate		()						{ return 0; }
	virtual DWORD	GetAddAttByType     (int nType)				{ return 0; }
	virtual DWORD	GetAtkType			()						{ return 0; }
	virtual DWORD	GetCrit()									{ return 0; }
	virtual DWORD	GetTough()									{ return 0; }
	virtual DWORD	GetDef				()						{ return 0; }
	virtual DWORD	GetDefence			()						{ return 0; }

	virtual DWORD	GetDex				()						{ return 0; }
	virtual DWORD	GetDexterity		()						{ return 0; }

	virtual DWORD	GetDdg				()						{ return 0; }
	virtual DWORD	GetDodge			()						{ return 0; }

	virtual DWORD	GetMagicDef			()						{ return 0; }

	virtual	DWORD	GetInterAtkRate		()						{ return 0; }
	virtual DWORD	GetIntervalAtkRate	()						{ return 0; }

	virtual int		GetHelmetTypeID		()						{ return 0; }
	virtual int		GetArmorTypeID		()						{ return 0; }
	virtual int		GetWeaponRTypeID	()						{ return 0; }
	virtual int 	GetWeaponLTypeID	()						{ return 0; }
	virtual int     GetDressTypeID		()						{ return 0; }
	virtual int     GetWingTypeID		()						{ return 0; }//翅膀

	virtual int 	GetMountTypeID		()						{ return 0; }
	virtual int		GetMantleTypeID		()						{ return 0; }// 坐骑 [陈剑飞7/31/2007]
	virtual int		GetWeaponSpecialView()						{ return 0; }

//	virtual DWORD 	GetMedalSelect		()						{ return 0; }
//	virtual DWORD	GetTitleSelect		()						{ return 0; }

	virtual int		GetDistance			(int x, int y)			{ return __max(abs(GetPosX() - x), abs(GetPosY() - y)); }
	virtual int		GetDistance			(IMapThing* pTarget)	{ return __max(abs(GetPosX() - pTarget->GetPosX()), abs(GetPosY() - pTarget->GetPosY())); }

	virtual CGameMap*	GetMap			()						
	{
		//zeng添加
// 		if(m_pMap==NULL)
// 		{
// 			MyStackWalker sw;
// 			sw.ShowCallstack();
//  		}
		//zeng添加
		CHECKF(m_pMap); return m_pMap; 
	}

	virtual DWORD	GetSynID			()						{ return ID_NONE; }
	virtual DWORD	GetSynRank			()						{ return RANK_NONE; }
	virtual DWORD	GetSynRankShow		()						{ return RANK_NONE; }

	virtual bool	IsSimpleMagicAtk	()						{ return 0; }
	virtual int 	GetSimpleMagicHitRate()						{ return 0; }

	virtual int		GetTutorExp			()						{ return 0; }
	virtual DWORD	GetTutorLevel		()						{ return 0; }
	virtual UCHAR	GetMercenaryRank	()						{ return 0; }
	virtual DWORD	GetMercenaryExp		()						{ return 0; }
	virtual UCHAR	GetNobilityRank		()						{ return 0; }
	virtual DWORD	GetExploit			()						{ return 0; }
public: // const

	virtual bool	IsAlive				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsTalkEnable		()						{ return false; }
	virtual bool	IsGM				()						{ return false; }
	virtual bool	IsPM				()						{ return false; }
	virtual bool	IsMonster			()						{ return false; }
	//20070316修罗----------------
	virtual BOOL	IsNpc				()						{ return false; }
	virtual BOOL	IsUser				()						{ return false; }
	//----------------------------
	virtual bool	IsDelThis			()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsVirtuous			()						{ return false; }
	virtual bool	IsEvil				()						{ return true; }
	virtual bool	IsBowman			()						{ return false; }
	virtual bool	IsShieldEquip		()						{ return false; }
	virtual bool	IsWing				()						{ return false; }
	virtual bool	IsAtkable			()						{ return false; }
	// return true: ppv返回对象指针
	virtual bool	QueryObj			(OBJID idObjType, void** ppv)	PURE_VIRTUAL_FUNCTION_0
	virtual IMapThing*	QueryMapThing	()						{ return (IMapThing*)this; }
	virtual IRole*	FindAroundRole		(OBJID id)				{ return GetMap()->QueryRole(GetPosX(), GetPosY(), id); }
	virtual CUser*	QueryOwnerUser		()						{ return NULL; }

public:	// modify attrib ------------------------------
	virtual void	SetDir				(int nDir)							{}
	virtual	void	SetStatus			(int nStatus, bool bSynchro=true, int nPower=0)	PURE_VIRTUAL_FUNCTION
	virtual	void	ClsStatus			(int nStatus, bool bSynchro=true)	PURE_VIRTUAL_FUNCTION
	//20061024修罗bug纯虚函数调用.改成非纯虚

	virtual	void	AddBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )		{}//07.4.26王玉波添加
	virtual void	UpdateBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )	{}//07.4.26王玉波添加

	virtual void    ClsBuff				(int nStatus)						{ }
	virtual bool	AddAttrib			(int idxAttr, __int64 i64Data, int bSynchro)			PURE_VIRTUAL_FUNCTION_0
	virtual void	ProcessOnMove		(int nMoveMode)						PURE_VIRTUAL_FUNCTION
	virtual void	ProcessAfterMove	()									PURE_VIRTUAL_FUNCTION
	virtual void	GetFootPrint		(int& x, int& y)					PURE_VIRTUAL_FUNCTION

	virtual bool	SpendArrow			()									{ return true; }

	virtual	IStatus*	QueryStatus(OBJID idType)							{ CHECKF(m_setStatus); return m_setStatus->GetObj(idType); }
	virtual	IStatusSet*	QueryStatusSet()									{ CHECKF(m_setStatus); return m_setStatus; }

	virtual DWORD	AwardBattleExp		(int nExp, bool bGemEffect=true, bool bIncludeOwner=true)	{ return 0;}
	virtual void	SetFightPause		(int nInterval)						{}
	virtual void	SaveInfo			()												PURE_VIRTUAL_FUNCTION

public: // system
	//20080111修罗:加入发送NPC标志---------
	//virtual void	BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf = false);
	virtual void	BroadcastRoomMsg(CNetMsg* pMsg, 
		bool bSendSelf = false,
		EM_SENDNPCFLAG sendNpc = EM_SENDNPCFLAG_TRUE);
	//-------------------------------------

	virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
	virtual bool	SendMsg(CNetMsg* pMsg)									{ return true; }
	virtual bool	SendSysMsg(const char* fmt, ...)						{ return true; }
	virtual void	OnTimer(time_t tCurr)									PURE_VIRTUAL_FUNCTION

public: // map
	virtual int		MoveToward(int nDir, bool bSync = true);
	virtual void	TransPos(int nPosX, int nPosY);
	virtual void	JumpPos(int nPosX, int nPosY);
	virtual bool	SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction);
	virtual void	ActiveTransPos(int nPosX, int nPosY);
	virtual bool	SendSelfToBlock()										PURE_VIRTUAL_FUNCTION_0

public:
	OBJID FindAttackUser(int nRange);

public: // fight -----------------------------
	virtual void	ClrAttackTarget		()									PURE_VIRTUAL_FUNCTION
	virtual bool	SetAttackTarget(IRole* pTarget = NULL)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	AutoSkillAttack(IRole* pTarget)							PURE_VIRTUAL_FUNCTION_0
	virtual int		Attack( IRole* pTarget,DWORD& dwBattleAddInfoRef )		PURE_VIRTUAL_FUNCTION_0
	virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower,int addi = 0, bool bReflectEnable=true)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y,bool needLearn = false,int learnLev = 0)	PURE_VIRTUAL_FUNCTION_0
	virtual void	Kill(IRole* pTarget, DWORD dwDieWay)					PURE_VIRTUAL_FUNCTION
	virtual void	BeKill(IRole* pTarget = NULL)							PURE_VIRTUAL_FUNCTION
	virtual bool	IsAtkable(IRole* pTarget, bool bSendHint=false)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsBeAtkable()											PURE_VIRTUAL_FUNCTION_0
	virtual int		GetAttackRange(int nTargetSizeAdd)						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetSizeAdd()											PURE_VIRTUAL_FUNCTION_0
	virtual void	SendDamageMsg( OBJID idTarget, int nDamage,DWORD dwBattleAddInfo )				PURE_VIRTUAL_FUNCTION
	virtual DWORD 	AdjustDamage(DWORD nDamage,int nAttribType) PURE_VIRTUAL_FUNCTION_0
	virtual int 	AdjustUserAttrib(DWORD nAttrib,int nAttribType) PURE_VIRTUAL_FUNCTION_0
	virtual bool	IgnoreTargetDodge()										{ return false;	}	//07.4.18王玉波添加					
	virtual bool	IsMustCritHit()											{ return false;	}	//07.4.18王玉波添加		 


public:	// 为了修改魔法系统而增加的部分，等魔法系统修改完再做整理 -- zlong 2005-03-23
	virtual void	AwardSynWarScore(CNpc* pNpc, int nScore)				{}
	virtual bool	IsImmunity(IRole* pRole)								{ return false; }
	virtual DWORD	GetProfession()											{ return 1; }
	virtual bool	SpendEquipItem(int nSubType, int nNum, bool bSynchro)	{ return false; }
	virtual bool	MagicSpendItem(int nSubType, int nNum, bool bSynchro)	{ return false; }	
	virtual int		AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg=false)		{ return nRawExp; }
	virtual bool	DecEquipmentDurability(bool bBeAttack, bool bMagic, int bDurValue=1)	{ return false; }
	virtual bool	IsEmbedGemType(int nGemType)							{ return false; }
	
	virtual DWORD	GetMana()												{ return 0; }
	virtual DWORD	GetMaxMana()											{ return 0; }
	
	virtual int		GetEnergy()												{ return 0; }
	virtual bool	CheckWeaponSubType(int nSubType, int nNum = 0)			{ return false; }
	virtual DWORD	GetPotential()											{ return 0; }
	virtual CTransformation* QueryTransformation()							{ return NULL; }
	virtual bool	IsArrowPass(int x, int y, int nAlt=ARROW_FLY_ALT)		{ return true; }
	virtual CTeam*	GetTeam()												{ return NULL; }
	virtual CItem*	GetEquipItemByPos(int nPosition)						{ return NULL; }
	virtual bool	CheckCrime(IRole* pRole)								{ return false; }
	virtual void	SetDelay(int nDelay)									{}
	
	virtual CItem*	GetWeaponR()											{ return NULL; }
	virtual CItem*	GetWeaponL()											{ return NULL; }
	
	virtual bool	DropItem(OBJID idItem, int x, int y)					{ return false; }
	virtual DWORD	GetSoulSum()											{ return 0; }
	virtual bool	Synchro()												{ return true; }
	virtual CItem*	FindSpaceTransSpell()									{ return NULL; }
	virtual void	AddEquipmentDurability(int nPosition, int nAddValue)				{}
	virtual bool	SynPosition(int nPosX, int nPosY, int nMaxDislocation = 8)			{ return true; }
	virtual bool	Transform(DWORD dwType, int nKeepSecs, bool bSynchro = true)		{ return false; }
	virtual bool	AddItem (CItem* pItem, bool bSynchro, bool bUpdate = true)			{ return false; }
	virtual bool	IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posCenter);
	virtual int		AdjustData(int nData, int nAdjust, int nMaxData=0)	{ return CRole::AdjustDataEx(nData, nAdjust, nMaxData); }
	static int		AdjustDataEx(int nData, int nAdjust, int nMaxData=0);

	virtual	CMagic*	QueryMagic()											{ return NULL; }

	static int	CalcStatusInfluence( DWORD dwStatus,DWORD dwInitValue,int nPower );
protected:
	IStatusSet*		m_setStatus;
//	I64				m_i64Effect;

	PROCESS_ID	m_idProcess;

	//
	CGameMap*	m_pMap;
public:
	int			m_nPosX;
	int			m_nPosY;
	int			m_nDir;
	UINT        m_nNpcType;

	// 广播对象集
	typedef std::list<OBJID>	BROADCAST_SET;
	BROADCAST_SET	m_setBCRole;
	BROADCAST_SET	m_setBCMapItem;
	BROADCAST_SET	m_setAnotherMapThing;

public:
	virtual bool	UpdateBroadcastSet(bool bClearSet = false);
	virtual void	ClrBroadcastSet(bool bForce = true);

public:
	virtual void	AddToBCRoleSet			(OBJID idRole, bool bSendMsg);
	virtual void	RemoveFromBCRoleSet		(OBJID idRole,bool bClrBigBc = false);
	virtual void	AddToBCMapItemSet		(OBJID idMapItem);
	virtual void	RemoveFromBCMapItemSet	(OBJID idMapItem);
protected:
	void			DisappearMapThing		(OBJID idMapThing);
	void			SeeMapThing				(OBJID idMapThing);
		
public:
	void DetachAllDOT();
	static BOOL CheckDebuffLockTarget(IRole* pRole, OBJID idTarget);
	const SET_ENMITY& GetSortedEnmityListByMaxEnmity();
	//20080105修罗:仇恨列表降序排列.考虑到仇恨加权
	const SET_ENMITY& GetSortedEnmityListByMaxEnmity_enmAddi();
	//06.11.4王玉波修改
	static bool		AttachStatus( IRole* pRole, int nStatus, int nPower, int nSecs, int nTimes=0, int nParam1=0, int nParam2=0,int nParam3 = 0, OBJID idOwner=0 );		// =0 : status once
	static bool		DetachStatus(IRole* pRole, int nStatus);
	static void		DetachWellStatus(IRole* pRole);
	static void		DetachBadlyStatus(IRole* pRole);
	static void		DetachAllStatus(IRole* pRole);
	static void     DetachSomeBadStatus( IRole* pRole,int nCout );// 清除一些不利条件 [12/26/2007 陈剑飞]

	// 坐骑时清除其他的BUF [陈剑飞8/8/2007]
//	static void     DetachSpeedStatus( IRole* pRole );

	static bool		IsWellStatus(int nStatus);
	static bool		IsBadlyStatus(int nStatus);

	CUser*          pUser;

	//06.11.4王玉波添加
	static bool		IsControlStatus( int nStatus );//是不是控制系状态
protected:

	vector<ENMITY>	m_vEnmity;

	OBJID	GetMaxEnmityUserID();			//获得仇恨列表中最大值的玩家ID
	OBJID	GetMaxEnmityAndActiveUserID();
	void	ClsEnmityList();//20070428修罗:清空仇恨表
	//20080104修罗:	增加仇恨值.
	//可能新加,或能更新.返回值为此role在仇恨列表中的新值.
	int		AddEnmity(OBJID id,
		int nEnmity,
		int nEnmAddi = NULL);
	void	ClsEnmity(OBJID id);
	bool CheckTargetEnmity(OBJID id);
	//-------------------------------------------
	
};

//////////////////////////////////////////////////////////////////////////
//  [2007-8-30] 朱斌 仇恨列表按伤害值降序排列比较函数

bool greater_than(const ENMITY& info1, const ENMITY& info2);
//20080105修罗:仇恨表降序排列.考虑仇恨加成
bool greater_than_enmAddi(const ENMITY& info1, const ENMITY& info2);



#endif // !defined(AFX_ROLE_H__A27D6633_2132_4F63_AA77_4EB7740D2D67__INCLUDED_)
