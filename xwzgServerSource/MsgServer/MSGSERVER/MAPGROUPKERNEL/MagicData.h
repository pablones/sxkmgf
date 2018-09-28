// MagicData.h: interface for the CTempMagicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICDATA_H__59045F42_8A10_44CB_A75B_D6C519454845__INCLUDED_)
#define AFX_MAGICDATA_H__59045F42_8A10_44CB_A75B_D6C519454845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include "MagicType.h"
#include "MyHeap.h"
#include "TimeOut.h"

//////////////////////////////////////////////////////////////////////
enum MAGICDATA{
	MAGICDATA_ID_=0,				//? 仅用于武器上带魔法的非数据库对象和再次学习时的UPDATE

	MAGICDATA_OWNERID=1,
	MAGICDATA_TYPE,
	MAGICDATA_LEVEL,
 	MAGICDATA_EXP,
	MAGICDATA_LASTUSE,
// 	MAGICDATA_UNLEARN,
// 	MAGICDATA_OLD_LEVEL,
// 	MAGICDATA_GIFTFLAG,	//  [12/12/2007 朱斌]	天赋技能标志位
// 	MAGICDATA_ADDLEVEL,
// 	MAGICDATA_LEARN,

	MAGICTYPEDATA_OFFSET=100,		// 控制量，请勿使用
	MAGICDATA_TYPE_X=101,		// 不使用
// 	MAGICDATA_SORT,
// 	MAGICDATA_NAME,
// 	MAGICDATA_CRIME,
// 	MAGICDATA_GROUND,
// 	MAGICDATA_MULTI,
// 	MAGICDATA_TARGET,
// 	MAGICDATA_LEVEL_X,			// 不使用
// 	MAGICDATA_USE_MP,
// 	MAGICDATA_USE_POTENTIAL,	// 消耗潜能
// 	MAGICDATA_POWER,
// 	MAGICDATA_INTONE,			// 魔法吟唱时间(ms)
// 	MAGICDATA_PERCENT,
// 	MAGICDATA_STEP,
// 	MAGICDATA_RANGE,
// 	MAGICDATA_DISTANCE,
// 	MAGICDATA_STATUS_CHANCE,		// ATKSTATUS类型魔法附加状态的概率百分比
// 	MAGICDATA_STATUS,
// 	MAGICDATA_NEED_PROFMASK,
// 	MAGICDATA_NEED_EXP,
// 	MAGICDATA_NEED_LEVEL,
// 	MAGICDATA_NEED_GEMTYPE,		//---jinggy---需要的宝石类型
// 	MAGICDATA_USE_XP,				// == TYPE_XPSKILL: use xp
// 	MAGICDATA_WEAPON_SUBTYPE,
// 	MAGICDATA_TIMES,				// =0 : once status
// 	MAGICDATA_AUTO_ACTIVE,			// 0x01: active when kill target
// 	MAGICDATA_FLOOR_ATTR,			// 0: need not match.
// 	MAGICDATA_AUTO_LEARN,			// bool
// 	MAGICDATA_LEARN_LEVEL,			// auto learn need level, nn: equ level, nnmm: between mm - nn level.
// 	MAGICDATA_DROPWEAPON,			// 1: throw weapon_r to target
// 	MAGICDATA_USE_EP,				// use energy
// 	MAGICDATA_WEAPON_HIT,			// 1: hit by weapon
// 	MAGICDATA_USE_ITEM,				// use equip item
// 	MAGICDATA_NEXT_MAGIC,			// launch this magic when filished current magic, use same target and pos
// 	MAGICDATA_DELAY,				// milli-secs
// 	MAGICDATA_ITEM_NUM,				// use equip item number
// 	MAGICDATA_WIDTH,				// width of magic. 0-180 for degree of fan (sort 4), 0 for default.
// 	MAGICDATA_DURABILITY,
// 
// 	MAGICDATA_APPLY_MS,				// 魔法每次作用施放时间
// 	MAGICDATA_TRACK_ID,
// 	MAGICDATA_TRACK_ID2,
// 
// 	MAGICDATA_AUTO_LEARN_PROB,		// 自动学习的成功率(10000=100%)
// 	MAGICDATA_GROUP_TYPE,			// 合击类型
// 	MAGICDATA_GROUP_MEMBER1_POS,	// 合击幻兽1相对位置
// 	MAGICDATA_GROUP_MEMBER2_POS,	// 合击幻兽2相对位置
// 	MAGICDATA_GROUP_MEMBER3_POS,	// 合击幻兽3相对位置
// 	////////////////////////////////////////////////////
// 	//tcp2005.9.21 add  for union attack begin
// 	MAGICDATA_MAGIC1,               // 人的魔法 
// 	MAGICDATA_MAGIC2,               // 唤兽1的魔法
// 	MAGICDATA_MAGIC3,               // 唤兽2的魔法
// 	MAGICDATA_MAGIC4,	            // 唤兽3的魔法
// 	MAGICDATA_MASKBIT,				// 可豁免地图限制PK属性
// 	MAGICDATA_ENMITY,
// 	MAGICDATA_PUBLICDELAY,
// 	
// 	//tcp2005.9.21 add  for union attack end

	//[2010-08-02 goto]技能表修改如下
	MAGICDATA_SORT,					//技能类型(主效果)
	MAGICDATA_NAME,					//技能名称
	MAGICDATA_CRIME,				//善恶标识(友方敌方)
	MAGICDATA_MULTI,				//是否是群体技能
	MAGICDATA_GROUND,				//是否是地效技能
	MAGICDATA_TARGET,				//最多影响目标数
	MAGICDATA_TARGET_ADD,			//影响目标增量
	MAGICDATA_NEED_PROFMASK,		//需求职业
	MAGICDATA_NEED_LEVEL,			//初始学习等级
	MAGICDATA_NEED_COST_LEV,		//消耗等级
	MAGICDATA_USE_MP,				//消耗魔法值
	MAGICDATA_USE_MP_ADD,			//消耗魔法值增量
	MAGICDATA_USE_ANG,				//消耗怒气
	MAGICDATA_USE_ANG_ADD,			//消耗怒气增量
	MAGICDATA_POWER,				//威力
	MAGICDATA_POWER_ADD,			//威力增量
	MAGICDATA_POWER2,				//属性2威力
	MAGICDATA_POWER2_ADD,			//属性2威力增量
	MAGICDATA_DISTANCE,				//距离
	MAGICDATA_RANGE,				//范围
	MAGICDATA_STATUS,				//附加状态/BUFF类型ID
	MAGICDATA_STATUS_POWER,			//附加状态/BUFF威力
	MAGICDATA_STATUS_ADD,			//附加状态增量
	MAGICDATA_STATUS_CHANCE,		//附加状态成功率
	MAGICDATA_STEP,					//持续时间
	MAGICDATA_STEP_ADD,				//持续时间增量
	MAGICDATA_TIMES,				//作用次数
	MAGICDATA_TIMES_ADD,			//作用次数增量
	MAGICDATA_DELAY,				//私有CD
	MAGICDATA_DELAY_ADD,			//私有CD增量
	MAGICDATA_PUBLICDELAY,			//公共CD
	MAGICDATA_PUBLICDELAY_ADD,		//公共CD增量
	MAGICDATA_ATTACK_TYPE,			//攻击种类
	MAGICDATA_BOOK,					//技能书ID
};


struct ST_MAGIC_DATA {
	OBJID	idMagic;
	OBJID	idOwner;
	USHORT	usType;
	USHORT	usLevel;
	DWORD	dwExp;
	UCHAR	ucUnlearn;
	USHORT	usOldLevel;
	DWORD	dwGiftFlag;
	DWORD	dwAddLevel;
	DWORD	dwLearn;
	USHORT	usCDIndex;
};

//////////////////////////////////////////////////////////////////////
class IMagicData
{
protected:
	IMagicData()			{ m_pType = NULL; }
	virtual ~IMagicData()	{}
public:
	// 由于CSingleObjSet2需要CreateNew接口，不得不增加
	static IMagicData*	CreateNew();
	virtual int		GetMagicLevel()										PURE_VIRTUAL_FUNCTION_0
	virtual void	ReleaseByOwner()											PURE_VIRTUAL_FUNCTION
	virtual bool	Create(IRecordset* pRecordset/*,IDatabase* pDb = NULL*/)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	Create(ST_MAGIC_DATA* pMagicData)							PURE_VIRTUAL_FUNCTION_0
	virtual int		GetInt(MAGICDATA nDataName)									PURE_VIRTUAL_FUNCTION_0
	virtual void	SetInt(MAGICDATA idx, int nData)							PURE_VIRTUAL_FUNCTION
	virtual void	SetStr(MAGICDATA idx, char* szData,int nSize)				PURE_VIRTUAL_FUNCTION
	virtual bool	FindType()													PURE_VIRTUAL_FUNCTION_0

	virtual OBJID	GetID()														PURE_VIRTUAL_FUNCTION_0
	virtual bool	Update()													PURE_VIRTUAL_FUNCTION_0
	virtual void	ClearUpdateFlags()											PURE_VIRTUAL_FUNCTION
	virtual OBJID	InsertRecord()												PURE_VIRTUAL_FUNCTION_0
	virtual bool	DeleteRecord()												PURE_VIRTUAL_FUNCTION_0
	virtual int	NeedSend()											            PURE_VIRTUAL_FUNCTION_0
	virtual int		GetIndex()													PURE_VIRTUAL_FUNCTION_0
	virtual void	SetIndex(int nIndex)										PURE_VIRTUAL_FUNCTION

public:
	virtual const char*		GetName()		{ CHECKF(m_pType); return m_pType->GetStr(MAGICTYPEDATA_NAME); }

	virtual bool	IsReady()							{ return ( !m_tDelay.IsActive() || ( m_tDelay.IsActive() && m_tDelay.IsTimeOut() ) ); }
	virtual void	StartDelay()						{ m_tDelay.Update(); }
	virtual void	SetDelay();
	virtual int		LeaveTime()							{ return m_tDelay.LeaveTime(); }	
	virtual clock_t			GetDelayUpdateTime()		{ if( m_tDelay.IsActive() ) return m_tDelay.GetUpdateTime();return 0;}// 王玉波添加 [5/30/2007]
	virtual CTimeOutMS&		QueryDelayObj()				{ return m_tDelay;}													//	王玉波添加 [5/30/2007]
protected:
	CMagicTypeData*	m_pType;
	CTimeOutMS		m_tDelay;			// 施展结束延时		
};


//////////////////////////////////////////////////////////////////////
class CMagicTempData  : public IMagicData
{
protected:
	CMagicTempData();
	virtual ~CMagicTempData()	{}

public:
	static IMagicData*	CreateNew()			{ return new CMagicTempData; }
	virtual void	ReleaseByOwner()		{ delete this; }
	virtual int		GetMagicLevel();

	virtual bool	Create(IRecordset* pRecordset);
	virtual bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);
	virtual bool	Create(ST_MAGIC_DATA* pMagicData);
	virtual int		GetInt(MAGICDATA nDataName);
	virtual void	SetInt(MAGICDATA idx, int nData);
	virtual bool	FindType();

	virtual OBJID	GetID()					{ return m_data.idMagic; }
	virtual bool	Update()				{ return false; }
	virtual void	ClearUpdateFlags()		{}
	virtual OBJID	InsertRecord()			{ return m_data.idMagic; }
	virtual bool	DeleteRecord()			{ return false; }

protected:
	ST_MAGIC_DATA	m_data;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAGICDATA_H__59045F42_8A10_44CB_A75B_D6C519454845__INCLUDED_)
