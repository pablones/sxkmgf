// BattleSystem.h: interface for the CBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLESYSTEM_H__291473E1_45B6_4A0A_8567_548852B93C33__INCLUDED_)
#define AFX_BATTLESYSTEM_H__291473E1_45B6_4A0A_8567_548852B93C33__INCLUDED_

#include "T_SingleObjSet2.h"

//////////////////////////////////////////////////////////////////////
enum LEVUPEXPDATA{
		LEVUPEXPDATA_LEVEL		=0,
		LEVUPEXPDATA_EXP,
		LEVUPEXPDATA_STU_EXP,	// 导师杀死怪，被带的学员这个等级最高可以得到的经验 --  zlong 2004.10.08
		LEVUPEXPDATA_EVOLUTION,//-----彭峰 2007/03/03--- 武器升级以后变换的样子
		LEVUPEXPDATA_REQLEVEL,//20070319修罗:需求等级加值
		LEVUPEXPDATA_UNKNOW,
		LEVUPEXPDATA_SYNSKILL_EXP,		//帮派研究度对应的等级
};
char	szZcxKey[];
char	szLevupexpTable[];
typedef	CGameData<LEVUPEXPDATA, szLevupexpTable, szZcxKey>	CLevupexpData;
typedef	ISingleObjSet2<CLevupexpData>	ILevupexpSet;
typedef	CSingleObjSet2<CLevupexpData>	CLevupexpSet;

const int	_EXP_TYPE	= 1000;
enum {
	EXP_TYPE_USER		= 0,	// 人物升级经验
	EXP_TYPE_USER_FIRJOB,
	EXP_TYPE_USER_SECJOB,
	EXP_TYPE_EUDEMON,
	EXP_TYPE_SKILLEXP,
	EXP_TYPE_SYNMONEY=5,
	EXP_TYPE_SYNSKILLCON = 7,
	EXP_TYPE_WEAPONSOUL = 8,

	EXP_TYPE_TUTOR=9,//师徒
	EXP_TYPE_SKILLMONEY,
};
//07.1.30王玉波修改
enum ATK_TYPE
{
	ATKTYPE_PHYSICAL_COMMON		=	0,	//物理普通攻击
	ATKTYPE_PHYSICAL_SKILL		=	1,	//物理技能攻击
	ATKTYPE_MAGIC_SKILL			=	2,	//魔法技能攻击
};
enum
{
	ATKTYPEMODE_NONE	=	0,//无效
	ATKTYPEMODE_COMMON	=	1,//外攻
	ATKTYPEMODE_INNNER	=	2,//内攻
	ATKTYPEMODE_ICE		=	3,//冰攻
	ATKTYPEMODE_FIRE	=	4,//火攻
	ATKTYPEMODE_NOXIOUS	=	5,//毒攻
	ATKTYPEMODE_MYSTERY	=	6,//玄攻
};
//07.1.20王玉波添加,偏斜类型,伤害程度
enum DAMAGE_PITCH
{
	DAMAGE_LOW	= 1,//轻伤
	DAMAGE_HALT	= 2,//中伤
	DAMAGE_HIGH	= 3,//重伤
};

enum {
	MAGICTYPE_NONE		= 0,	// 必须为0(可能由false转换而来)
	MAGICTYPE_NORMAL	= 1,	// 必须为1(可能由true转换而来)
};

class IRole;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBattleSystem  
{
public:
	CBattleSystem(PROCESS_ID idProcess, IRole* pOwner);
	virtual ~CBattleSystem();

public:
	// 一次交战终止或结束时调用
	void		ResetBattle				();
	BOOL		CreateBattle			(OBJID idTarget);
	IRole*		GetOwner				()					{return m_pOwner;}
	BOOL		checkMonsterTag			(int monsterID);

	BOOL		ProcAttack_Hand2Hand	();
	BOOL		IsBattleMaintain		();
	OBJID		GetTargetID				()					{return m_idTarget;}
	BOOL		IsActived				()					{return m_idTarget!=ID_NONE;}

public: // static
	static int	AdjustDrop( int nDrop,int nAtkLev,int nDefLev );
	static int	AdjustExp( int nExp,int nAtkLev,int nDefLev );
	//外部接口
	static int				CalcDamage( DWORD &dwBattleAddInfoRef,const ATK_TYPE& type,IRole* pAtker,IRole* pTargeter,int nPower = 0,int nSkillHitRate= 0, int nPower2=0, int nAtackType=100);

	//内部调用
	//等级差调整
	static float			GetLevelDifference( int nAtkLevel,int nTagLevel );
	//是否触发了致命一击,法术则是法术爆发,攻击力算法不同
	static bool				IsCriticalHit( IRole* pRole,bool bMagic ,IRole* pTarget = NULL);
	//----------------普通物理攻击相关--------------------------------------------------------------------------
	//是否命中
	static bool				IsHitRatePhysicalCommon( IRole* pAtker,IRole* pTargeter );
	//----------------物理技能攻击相关-------------------------------------------------------------------------
	//是否命中
	bool static				IsHitRatePhysicalSkill( IRole* pAtker,IRole* pTargeter,int nPercentValue );

	
	//----------------魔法技能攻击相关------------------------------------------------------------------------
	//最后结算
	static int				CalcDamage_PhysicalCommon( IRole* pAtker,IRole* pTargeter,DWORD& dwBattleAddInfoRef );//计算普通物理攻击伤害
	static int				CalcDamage_MagicSkill( IRole* pAtker,IRole* pTargeter,int nMagicSkillPower,DWORD& dwBattleAddInfoRef, int nPower2=0, int nAtackType=100);//计算魔法技能伤害

	static int				Adjust_Damage_MagicPassivity(IRole* pAtker,IRole* pTargeter,int nDamage );		//调整影响被动魔法伤害的函数，softworms-2007-09-05
	static float			CheckATKType(DWORD atkType,DWORD defType);


	OBJID			m_idTarget;
	IRole*			m_pOwner;

	PROCESS_ID		m_idProcess;

public: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_BATTLESYSTEM_H__291473E1_45B6_4A0A_8567_548852B93C33__INCLUDED_)
