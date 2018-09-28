// WeaponSoulType.h: interface for the CWeaponSoulType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONSOULTYPE_H__8C094750_53E7_4E67_9579_C1FE493DDD28__INCLUDED_)
#define AFX_WEAPONSOULTYPE_H__8C094750_53E7_4E67_9579_C1FE493DDD28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleMap2.h"


enum WEAPONSOULTYPEDATA
{
	WEAPONSOULTYPEDATA_ID = 0,//TYPEID
	WEAPONSOULTYPEDATA_SZNAME,//名称
	WEAPONSOULTYPEDATA_ABILITY_MARK,//主属性标识
//	WEAPONSOULTYPEDATA_MIN_ATTACK_FIRST,//初始_最小物理攻击力
// 	WEAPONSOULTYPEDATA_MAX_ATTACK_FIRST,//初始_最大物理攻击力
	WEAPONSOULTYPEDATA_MIN_ATTACK_FIRST_MAX,//极限_初始最小物理攻击力
	WEAPONSOULTYPEDATA_MAX_ATTACK_FIRST_MAX,//极限_初始最大物理攻击力
//	WEAPONSOULTYPEDATA_MIN_MAGIC_ATTACK_FIRST,//初始_最小魔法攻击力
// 	WEAPONSOULTYPEDATA_MAX_MAGIC_ATTACK_FIRST,//初始_最大魔法攻击力
	WEAPONSOULTYPEDATA_MIN_MAGIC_ATTACK_FIRST_MAX,//极限_初始最小魔法攻击力
	WEAPONSOULTYPEDATA_MAX_MAGIC_ATTACK_FIRST_MAX,//极限_初始最大魔法攻击力
//	WEAPONSOULTYPEDATA_DEFENSE_FIRST,//初始_物理防御
// 	WEAPONSOULTYPEDATA_MAGIC_DEF_FIRST,//初始_魔法防御
	WEAPONSOULTYPEDATA_DEFENSE_MAX,//极限_物理防御
	WEAPONSOULTYPEDATA_MAGIC_DEF_MAX,//极限_魔法防御
	WEAPONSOULTYPEDATA_MIN_ATTACK_GROW_FIRST,//初始_最小物理攻击力成长值
	WEAPONSOULTYPEDATA_MAX_ATTACK_GROW_FIRST,//初始_最大物理攻击力成长值
	WEAPONSOULTYPEDATA_MIN_ATTACK_GROW_MAX,//极限_最小物理攻击力成长值
	WEAPONSOULTYPEDATA_MAX_ATTACK_GROW_MAX,//极限_最大物理攻击力成长值
	WEAPONSOULTYPEDATA_MIN_MAGIC_ATTACK_GROW_FIRST,//初始_最小魔法攻击力成长值
	WEAPONSOULTYPEDATA_MAX_MAGIC_ATTACK_GROW_FIRST,//初始_最大魔法攻击力成长值
	WEAPONSOULTYPEDATA_MIN_MAGIC_ATTACK_GROW_MAX,//极限_最小魔法攻击力成长值
	WEAPONSOULTYPEDATA_MAX_MAGIC_ATTACK_GROW_MAX,//极限_最大魔法攻击力成长值
	WEAPONSOULTYPEDATA_DEFENSE_GROW_FIRST,//初始_物理防御成长值
	WEAPONSOULTYPEDATA_DEFENSE_GROW_MAX,//极限_物理防御成长值
	WEAPONSOULTYPEDATA_MAGIC_DEF_GROW_FIRST,//初始_魔法防御成长值
	WEAPONSOULTYPEDATA_MAGIC_DEF_GROW_MAX,//极限_魔法防御成长值
	WEAPONSOULTYPEDATA_SKILL_POINT_GROW,//修为值成长值
	WEAPONSOULTYPEDATA_SKILL_POINT_FIRST,//初始修为点
	WEAPONSOULTYPEDATA_TEAM_WORK,//合体贡献
};

struct ST_WeaponSoulInfo
{
	OBJID	id;//ID
	char	szName[_MAX_NAMESIZE];//名称
	UINT	ability_mark;//主属性标识
//	UINT	min_attack_first;//初始_最小物理攻击力
// 	UINT	max_attack_first;//初始_最大物理攻击力
	UINT	min_attack_first_max;//极限_初始最小物理攻击力
	UINT	max_attack_first_max;//极限_初始最大物理攻击力
//	UINT	min_magic_attack_first;//初始_最小魔法攻击力
// 	UINT	max_magic_attack_first;//初始_最大魔法攻击力
	UINT	min_magic_attack_first_max;//极限_初始最小魔法攻击力
	UINT	max_magic_attack_first_max;//极限_初始最大魔法攻击力
//	UINT	defense_first;//初始_物理防御
// 	UINT	magic_def_first;//初始_魔法防御
	UINT	defense_max;//极限_物理防御
	UINT	magic_def_max;//极限_魔法防御
	UINT	min_attack_grow_first;//初始_最小物理攻击力成长值
	UINT	max_attack_grow_first;//初始_最大物理攻击力成长值
	UINT	min_attack_grow_max;//极限_最小物理攻击力成长值
	UINT	max_attack_grow_max;//极限_最大物理攻击力成长值
	UINT	min_magic_attack_grow_first;//初始_最小魔法攻击力成长值
	UINT	max_magic_attack_grow_first;//初始_最大魔法攻击力成长值
	UINT	min_magic_attack_grow_max;//极限_最小魔法攻击力成长值
	UINT	max_magic_attack_grow_max;//极限_最大魔法攻击力成长值
	UINT	defense_grow_first;//初始_物理防御成长值
	UINT	defense_grow_max;//极限_物理防御成长值
	UINT	magic_def_grow_first;//初始_魔法防御成长值
	UINT	magic_def_grow_max;//极限_魔法防御成长值
	UINT	skill_point_grow;//修为值成长值
	UINT	skill_point_first;//初始修为点
	UINT	team_work;//合体贡献
};


char	szWeaponSoulTypeTable[];
typedef	CGameData<WEAPONSOULTYPEDATA,szWeaponSoulTypeTable,szID>	CGameDataWeaponSoulType;
typedef	ISingleMap2<CGameDataWeaponSoulType>	IWeaponSoulTypeSet;
typedef	CSingleMap2<CGameDataWeaponSoulType>	CWeaponSoulTypeSet;

enum EM_WPSL_ABILITY_MARK
{
	EM_WPSLABILITYMARK_max_attack_grow_first	=1,
	EM_WPSLABILITYMARK_min_attack_grow_first	=2,
	EM_WPSLABILITYMARK_max_magic_attack_grow_first	=4,
	EM_WPSLABILITYMARK_min_magic_attack_grow_first	=8,
	EM_WPSLABILITYMARK_defense_grow_first	=16,
	EM_WPSLABILITYMARK_magic_def_grow_first	=32,
};

class CWeaponSoulType  
{
public:
	CWeaponSoulType();
	virtual ~CWeaponSoulType();
public:
	static CWeaponSoulType*	CreateNew	()		{ return new CWeaponSoulType; }
	ULONG	Release			()				{ delete this; return 0; }
	bool	Create			(IDatabase* pDb);
	CGameDataWeaponSoulType*  QueryWeaponSoulType(OBJID idType);
	bool GetInfo(OBJID idType, ST_WeaponSoulInfo* pInfo);
	

protected:
	IWeaponSoulTypeSet*	m_setWeaponSoulType;
	MYHEAP_DECLARATION(s_heap)

};

//global
extern CWeaponSoulType* g_pWeaponSoulType;

#endif // !defined(AFX_WEAPONSOULTYPE_H__8C094750_53E7_4E67_9579_C1FE493DDD28__INCLUDED_)
