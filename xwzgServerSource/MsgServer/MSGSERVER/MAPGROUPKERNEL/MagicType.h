// MagicType.h: interface for the CMagicType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICTYPE_H__344C2000_C8EA_4C2E_BDE7_A2772EB20976__INCLUDED_)
#define AFX_MAGICTYPE_H__344C2000_C8EA_4C2E_BDE7_A2772EB20976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleObjSet2.h"
#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////
//enum MAGICTYPEDATA{
// 		MAGICTYPEDATA_ID_=0,
// 		MAGICTYPEDATA_TYPE,
// 		MAGICTYPEDATA_SORT,
// 		MAGICTYPEDATA_NAME,
// 		MAGICTYPEDATA_CRIME,
// 		MAGICTYPEDATA_GROUND,
// 		MAGICTYPEDATA_MULTI,
// 		MAGICTYPEDATA_TARGET,
// 		MAGICTYPEDATA_LEVEL,
// 		MAGICTYPEDATA_USE_MP,
// 		MAGICTYPEDATA_USE_POTENTIAL,
// 		MAGICTYPEDATA_POWER,
// 		MAGICTYPEDATA_INTONE,
// 		MAGICTYPEDATA_PERCENT,
// 		MAGICTYPEDATA_STEP,
// 		MAGICTYPEDATA_RANGE,
// 		MAGICTYPEDATA_DISTANCE,
// 		MAGICTYPEDATA_STATUS_CHANCE,		// ATKSTATUS类型魔法攻击附加状态的概率百分比
// 		MAGICTYPEDATA_STATUS,
// 		MAGICTYPEDATA_NEED_PROFMASK,
// 		MAGICTYPEDATA_NEED_EXP,
// 		MAGICTYPEDATA_NEED_LEVEL,
// 		MAGICTYPEDATA_NEED_GEMTYPE,		//---jinggy---需要的宝石类型
// 		MAGICTYPEDATA_USE_XP,				// == TYPE_XPSKILL: use xp
// 		MAGICTYPEDATA_WEAPON_SUBTYPE,
// 		MAGICTYPEDATA_TIMES,				// =0 : once status
// 		MAGICTYPEDATA_AUTO_ACTIVE,			// 0x01: active when kill target
// 		MAGICTYPEDATA_FLOOR_ATTR,			// 0: need not match.
// 		MAGICTYPEDATA_AUTO_LEARN,			// bool
// 		MAGICTYPEDATA_LEARN_LEVEL,			// auto learn need level, nn: equ level, nnmm: between mm - nn level.
// 		MAGICTYPEDATA_DROPWEAPON,			// 1: throw weapon_r to target
// 		MAGICTYPEDATA_USE_EP,				// use energy
// 		MAGICTYPEDATA_WEAPON_HIT,			// 1: hit by weapon
// 		MAGICTYPEDATA_USE_ITEM,				// use equip item
// 		MAGICTYPEDATA_NEXT_MAGIC,			// launch this magic when filished current magic, use same target and pos
// 		MAGICTYPEDATA_DELAY,				// milli-secs
// 		MAGICTYPEDATA_ITEM_NUM,				// use equip item number
// 		MAGICTYPEDATA_WIDTH,
// 		MAGICTYPEDATA_DURABILITY,
// 		MAGICTYPEDATA_APPLY_MS,				// 魔法每次作用施展时间间隔
// 		MAGICTYPEDATA_TRACK_ID,
// 		MAGICTYPEDATA_TRACK_ID2,
// 		MAGICTYPEDATA_AUTO_LEARN_PROB,		// 自动学习的成功率(10000=100%)
// 	//	MAGICTYPEDATA_MAGIC1,               // 用来区分技能能否在坐骑上使用 [陈剑飞9/11/2007]
// 	
// 	    MAGICTYPEDATA_ENMITY_ADDI = 52,
	//[2010-08-02 goto]技能表修改如下
enum MAGICTYPEDATA{
	MAGICTYPEDATA_ID_=0,
	MAGICTYPEDATA_TYPE,
	MAGICTYPEDATA_SORT,					//技能类型(主效果)
	MAGICTYPEDATA_NAME,					//技能名称
	MAGICTYPEDATA_CRIME,				//善恶标识(友方敌方)
	MAGICTYPEDATA_MULTI,				//是否是群体技能
	MAGICTYPEDATA_GROUND,				//是否是地效技能
	MAGICTYPEDATA_TARGET,				//最多影响目标数
	MAGICTYPEDATA_TARGET_ADD,			//影响目标增量
	MAGICTYPEDATA_NEED_PROFMASK,		//需求职业
	MAGICTYPEDATA_NEED_LEVEL,			//初始学习等级
	MAGICTYPEDATA_NEED_COST_LEV,		//消耗等级
	MAGICTYPEDATA_USE_MP,				//消耗魔法值
	MAGICTYPEDATA_USE_MP_add,			//消耗魔法值增量
	MAGICTYPEDATA_USE_ANG,				//消耗怒气
	MAGICTYPEDATA_USE_ANG_ADD,			//消耗怒气增量
	MAGICTYPEDATA_POWER,				//威力
	MAGICTYPEDATA_POWER_ADD,			//威力增量
	MAGICTYPEDATA_POWER2,				//属性2威力
	MAGICTYPEDATA_POWER2_ADD,			//属性2威力增量
	MAGICTYPEDATA_DISTANCE,				//距离
	MAGICTYPEDATA_RANGE,				//范围
	MAGICTYPEDATA_STATUS,				//附加状态/BUFF类型ID
	MAGICTYPEDATA_STATUS_POWER,			//附加状态/BUFF威力
	MAGICTYPEDATA_STATUS_ADD,			//附加状态增量
	MAGICTYPEDATA_STATUS_CHANCE,		//附加状态成功率
	MAGICTYPEDATA_STEP,					//持续时间
	MAGICTYPEDATA_STEP_ADD,				//持续时间增量
	MAGICTYPEDATA_TIMES,				//作用次数
	MAGICTYPEDATA_TIMES_ADD,			//作用次数增量
	MAGICTYPEDATA_DELAY,				//私有CD
	MAGICTYPEDATA_DELAY_ADD,			//私有CD增量
	MAGICTYPEDATA_DELAY_PUBLIC,			//公共CD
	MAGICTYPEDATA_DELAY_PUBLIC_ADD,		//公共CD增量
	MAGICTYPEDATA_ATTACK_TYPE,			//攻击种类
	MAGICTYPEDATA_BOOK,					//技能书ID
};
char	szMagicTypeTable[];
typedef	CGameData<MAGICTYPEDATA, szMagicTypeTable, szID>	CGameMagicTypeData;


typedef struct {
	UCHAR	ucDir;
	UCHAR	ucStep;
	UCHAR	ucAlt;
	OBJID	idAction;
	int		nPower;
	int		nApplyMS;
} ST_TRACK;

class CMagicTypeData
{
public:
	CMagicTypeData()			{ m_pData = NULL; }
	virtual ~CMagicTypeData()	{ if (m_pData) SAFE_RELEASE (m_pData); }

public:
	static CMagicTypeData*	CreateNew	()	{ return new CMagicTypeData; }
	ULONG	Release			()				{ delete this; return 0; }
	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	bool	Create			(IRecordset* pRes/*, IDatabase* pDb = NULL*/);

public: // get set
	OBJID		GetID()					{ CHECKF(m_pData); return m_pData->GetKey(); }

	int		GetInt(MAGICTYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(MAGICTYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetStr(idx); } 

protected:
	CGameMagicTypeData*		m_pData;
public:
	MYHEAP_DECLARATION(s_heap)
};

//////////////////////////////////////////////////////////////////////

typedef	ISingleObjSet2<CMagicTypeData>		IMagicTypeSet;
typedef	CSingleObjSet2<CMagicTypeData>		CMagicTypeSet;


//class CMagicType  
//{
//public:
//	CMagicType();
//	virtual ~CMagicType();
//	
//public:
//	static CMagicType*	CreateNew	()		{ return new CMagicType; }
//	ULONG	Release			()				{ delete this; return 0; }
//	bool	Create			(IDatabase* pDb);
//	CMagicTypeData*	QueryMagicType(OBJID idType);
//
//protected:
//	IMagicTypeSet*	m_setType;
//public:
//	MYHEAP_DECLARATION(s_heap)
//};

#endif // !defined(AFX_MAGICTYPE_H__344C2000_C8EA_4C2E_BDE7_A2772EB20976__INCLUDED_)
