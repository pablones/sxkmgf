// Magic.h: interface for the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGIC_H__B2689767_419D_487F_B0F9_FBB04C4619AA__INCLUDED_)
#define AFX_MAGIC_H__B2689767_419D_487F_B0F9_FBB04C4619AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "UserTable.h"
#include "MagicType.h"
#include "T_SingleObjSet2.h"
#include "Myheap.h"
#include "MagicData.h"
#include "StrRes.h"


#pragma warning(disable:4786)
#include <vector>


enum EM_GIFT_MAGIC_FLAG
{
	EM_NOT_GIFT_MAGIC = 0,	//	普通技能
	EM_GIFT_MAGIC,			//	天赋技能
	EM_GIFT_MAGIC_INVALID,	//  持能无效
	EM_GIFT_MAGIC_REMOVE,	//	技能删除(不用)
};

char	szMagicTable[];

enum MAGIC_TYPE
{
	MAGIC_PASSIVITY_AVENGER				 =2410,///被动-天人族-复仇者
	MAGIC_PASSIVITY_NATURALAFFINITY		 =2411,///被动-天人族-自然亲和
	MAGIC_PASSIVITY_NATURETHINK			 =2210,///被动-天人族-天人合一
	MAGIC_PASSIVITY_EASYTAKE			 =2110,///被动-天人族-手到擒来
	MAGIC_PASSIVITY_BEASTRECOVERY		 =2310,///被动-天人族-野兽恢复
	MAGIC_PASSIVITY_PAINSUPPRESS 		 =2211,///被动-天人族-痛苦压制
	MAGIC_PASSIVITY_INNATENESSWARRIOR  	 =2111,///被动-天人族-天生战士
	MAGIC_PASSIVITY_MANNAAGILITY  		 =2311,///被动-天人族-神赐敏捷

	MAGIC_PASSIVITY_DCZ  = 1401,///被动-龙族--独裁者
	MAGIC_PASSIVITY_SLBN = 1402,///被动-龙族--杀戮本能
    MAGIC_PASSIVITY_YSLL = 1111,///被动-龙族--战士-原始力量-
	MAGIC_PASSIVITY_ZLFK = 1112,///被动-龙族--战士--振聋发聩
	MAGIC_PASSIVITY_LLGR = 1311,///被动-龙族--弓手--膂力过人
	MAGIC_PASSIVITY_XLSH = 1312,///被动-龙族--弓手--心领神会
	MAGIC_PASSIVITY_BMLH = 1211,///被动-龙族--法师--不灭龙魂
	MAGIC_PASSIVITY_GGZH = 1212,///被动-龙族--法师--亘古智慧

	MAGIC_PASSIVITY_ZLZ  = 3401,///被动-基因--中立者
	MAGIC_PASSIVITY_DXJH = 3402,///被动-基因--代谢进化
    MAGIC_PASSIVITY_JYTZ = 3111,///被动-基因--战士--基因体质-
	MAGIC_PASSIVITY_TZBD = 3112,///被动-基因--战士--特种部队
	MAGIC_PASSIVITY_GCMB = 3311,///被动-基因--枪手--贯穿目标
	MAGIC_PASSIVITY_DWXF = 3312,///被动-基因--枪手--弹无虚发
	MAGIC_PASSIVITY_TNQH = 3211,///被动-基因--法师--头脑强化
	MAGIC_PASSIVITY_JSMX = 3212,///被动-基因--法师--静思冥想

//----------------新添被动技能--------------------------------------------------------------------------
	MAGIC_MZWQ = 4111,//  被动技能--魔铸武器[12/14/2007 陈剑飞]warriorWARRIOR
	MAGIC_JSZD = 4112,//  被动技能--坚守阵地[12/14/2007 陈剑飞]
	MAGIC_SZSJ = 4113,// 被动技能--速战速决 [12/14/2007 陈剑飞]
	MAGIC_CSZJ = 4114,//乘胜追击
	MAGIC_WG   = 4115,// 被动技能--稳固 [12/14/2007 陈剑飞]
	MAGIC_KZ   = 4116,//抗争
	MAGIC_GMXN = 4117,//光明信念
	MAGIC_JSLX = 4118,//精神领袖
	MAGIC_DMKJ = 4119,//打磨铠甲	4119	被动	获得等于所有基本属性之和的物防、魔防。
	MAGIC_MZWQ2 = 4311,//魔铸武器	4311	被动	你的普通攻击有10％的几率附带等于敏捷值的魔法伤害。
	MAGIC_CTSJ = 4312,//穿透射击	4312	被动	你的物理攻击有5％的几率无视目标物理防御。
	MAGIC_SDFY = 4313,//闪电反应	4313	被动	你有5％的几率忽略物理伤害。
	MAGIC_DKMF = 4314,//抵抗魔法	4314	被动	你有5％的几率忽略魔法伤害。
	MAGIC_FSCT = 4209,//法术穿透	4209	被动	你的法术攻击有10％的几率无视目标魔防
	MAGIC_BLFS = 4210,//暴力法师	4210	被动	暴击几率提高10％
	MAGIC_XS   = 4211,//吸收	4211	被动	有15％的几率在受到物理伤害时回复等于该伤害20％的HP。
	MAGIC_QHXS = 4212,//强化吸收	4212	被动	吸收还可以有20％的几率在受到魔法伤害时回复等于该伤害20％的MP。
	MAGIC_SHFT = 4402,//伤害反弹	4402	被动	你有5％的几率反弹受到的伤害。
	MAGIC_HB   = 4403,//回避	4403	被动	降低你受到的范围攻击伤害10％。
	MAGIC_JR   = 4404,//坚韧	4404	被动	降低你受到暴击的几率10％。
	MAGIC_SL   = 4405,//狩猎	4405	被动	提高你对怪物造成的伤害，最多300点
	MAGIC_JH   = 4120,//击晕	4120	被动	龙斩/狼牙/雷刃有10％的几率使目标眩晕1秒（战斗不能，伤害不打断）。
	MAGCI_MZTB = 4315,//瞄准腿部	4315	被动	强力弩/急速箭/速射有10％的几率使目标定身2秒。
	MAGIC_HM   = 4213,//毁灭	4213	被动	天崩地裂有10％的几率使目标不能战斗（受到伤害就消失），持续3秒。
	MAGIC_ZQ   = 4214,//榨取	4214	被动	异界洪流有15％的几率为你回复HP，效果相当于造成伤害的10％。
	MAGIC_QHJHS = 4316,//强化净化术	4316	被动	净化术可以对敌人使用，驱除目标最多两个有利状态。有利状态＝Buff。
	MAGIC_QHQTJHS = 4317,//强化群体净化术	4317	被动	强化群体净化可以对敌人使用，驱除目标周围多人的1个有利状态，范围最大为20。有利状态＝Buff。
	MAGIC_JTMS = 4215,//精通抹煞Ⅱ	4215	被动	抹煞Ⅱ可以使你回复HP，效果最多为造成伤害的25％。
	MAGIC_SXWB = 4216,//瞬息万变	4216	被动	受到暴击后结束狂热灵感Ⅱ的冷却时间。
	MAGIC_QHSY = 4217,//强化瞬间移动	4217	被动	瞬间移动可以解除战斗不能状态。

//---------------------------------------------------------------------------------------------------------------

	//龙斩，狼牙，雷斩
	MAGIC_LONGZ = 1102 ,
	MAGIC_LANGY = 2102,
	MAGIC_LEIZ = 3102,

	//天崩地裂，异界洪流,狂热灵感,抹煞2
	MAGIC_TBDL = 9022,
	MAGIC_YJHL = 10000,
	MAGIC_KRLG2 = 4208,
	MAGIC_MOS2 = 6432,

	//强力弩/急速箭/速射
	MAGIC_QLN = 1302,
	MAGIC_JSJ = 2302,
	MAGIC_SUS = 3302,
	
	//顺势斩、风卷残云
	MAGIC_SSZ  = 4105,
	MAGIC_FJCY = 6701,


	//速射、强力射击、瞄准射击
	MAGIC_SS   = 3302,
	MAGIC_QLSJ = 4307,
	MAGIC_MZSJ = 6452,


	//充能弹、聚能突袭、精神攻击
	MAGIC_CND  = 3203,
	MAGIC_JNTX = 3204,
	MAGIC_JSGJ = 6726,




	//魔力增幅、法术崩坏、乏力、潜能爆发
	MAGIC_MLZF = 4201,
	MAGIC_FSBH = 6827,
	MAGIC_FL   = 6427,
	MAGIC_QNBF = 6829,

	//斗志昂扬、恐怖吼叫、刺耳尖啸、勇气光环、招架、铜墙铁壁
	MAGIC_DZAY = 4101,
	MAGIC_KBHJ = 4102,
	MAGIC_CEJX = 6803,
	MAGIC_YQGH = 6804,
	MAGIC_ZJ   = 4107,
	MAGIC_TQTB = 6811,
	//预见危险、挑战女巫、坚岩皮肤

	MAGIC_YJWX = 1301,
	MAGIC_TZLW = 6551,
	MAGIC_JYPF = 6552,
	

	MAGIC_BARB			 =2101,///穿刺
	MAGIC_DEATHCHAINS	 =4104,///死亡枷锁
	MAGIC_DEATHHANDSHAKE =6807,///死亡之握
	MAGIC_HEALMAGIC =4301,///治愈术
	MAGIC_COLONYHEAL =6851,///群体治愈术
	MAGIC_ENERGYSHIELD =2202,///自然之盾
	MAGIC_CRAZEAFFLATUS =6826,///狂热灵感

	//satan 
	MAGIC_SSQY = 1119,//殉身取义
	//goto 技能名字有变动,请以编号为准,勿以注释中的名字为准
	MAGIC_JGBH	=9001,//金刚不坏
	MAGIC_JGBRZ	=9002,//金刚般若掌
	MAGIC_TBS	=9003,//铁布衫
	MAGIC_LY	=1124,//灵蕴
	MAGIC_ZYHT	=9004,//真元护体
	MAGIC_WJJ	=9008,//万剑决,击退目标
	MAGIC_BACK	=9000,//回城
	MAGIC_BACKSYN=2501,//回帮
//modify code by 林德渊 begin 2011.6.22
	MAGIC_LGWS  =4010,//烈骨无伤
	MAGIC_FJCY_2=4011,//风卷残云
	MAGIC_ZHF   =4012,//追魂斧
	MAGIC_MDSX  =4013,//迷毒噬心
//modify code by 林德渊 end
	//宠物技能
	MAGIC_XX	=8001,//吸血
	MAGIC_GJXX	=8002,//高级吸血
	MAGIC_XQ	=8003,//吸气
	MAGIC_GJXQ	=8004,//高级吸气
	MAGIC_DN	=8005,//打怒
	MAGIC_GJDN	=8006,//高级打怒
	MAGIC_LJ	=8007,//连击
	MAGIC_GJLJ	=8008,//高级连击
};

enum MAGIC_GROUP_TYPE
{
	MAGICGROUP_NONE	=0,
	MAGICGROUP_ATTACK_NOMOVE,	     // 合击1，幻兽原地攻击
	MAGICGROUP_ATTACK_MOVE,		     // 合击2，幻兽跑位攻击(跑位吟唱)
	MAGICGROUP_ATTACK_RAND,		     // 幻兽攻击玩家合法范围内的某一随机对象
	MAGICGROUP_ATTACK_SAME,		     // 幻兽攻击与玩家相同的目标
	MAGICGROUP_STANDAR_UINON_ATTACK, // 玩家用固定组合技进攻 tcp2005.9.21 add
	MAGICGROUP_FREE_UINON_ATTACK,    // 玩家用自由组合技进攻 tcp2005.9.21 add	
};
//////////////////////////////////////////////////////////////////////
enum {
		MAGICSORT_ATTACK			= 1,
		MAGICSORT_RECRUIT			= 2,			// support auto active.
		MAGICSORT_CROSS				= 3,
		MAGICSORT_FAN				= 4,			// support auto active(random).
		MAGICSORT_BOMB				= 5,
		MAGICSORT_ATTACHSTATUS		= 6,
		MAGICSORT_DETACHSTATUS		= 7,
		MAGICSORT_SQUARE			= 8,
		MAGICSORT_FLYSTRIKE		= 9,				// move, a-lock
		MAGICSORT_RANDOMTRANS		= 10,			// move, a-lock
		MAGICSORT_DISPATCHXP		= 11,
		MAGICSORT_COLLIDE			= 12,			// move, a-lock & b-synchro	
		MAGICSORT_SERIALCUT			= 13,			// auto active only.
		MAGICSORT_LINE				= 14,			// support auto active(random).
		MAGICSORT_ATKRANGE			= 15,			// auto active only, forever active.
		MAGICSORT_ATKSTATUS			= 16,			// support auto active, random active.
		MAGICSORT_CALLTEAMMEMBER	= 17,
		MAGICSORT_RECORDTRANSSPELL	= 18,
		MAGICSORT_TRANSFORM			= 19,
		MAGICSORT_ADDMANA			= 20,			// support self target only.
		MAGICSORT_LAYTRAP			= 21,
		MAGICSORT_DANCE				= 22,			// 跳舞(only use for client)
		MAGICSORT_CALLPET			= 23,			// 召唤兽
		MAGICSORT_VAMPIRE			= 24,			// 吸血，power is percent award. use for call pet
		MAGICSORT_INSTEAD			= 25,			// 替身. use for call pet
		MAGICSORT_DECLIFE			= 26,			// 扣血(当前血的比例)
		MAGICSORT_GROUNDSTING		= 27,			// 地刺
		MAGICSORT_REBORN			= 28,			// 复活 -- zlong 2004.5.14
		MAGICSORT_TEAM_MAGIC		= 29,			// 界结魔法—— 与MAGICSORT_ATTACHSTATUS相同处理，
													//				这里独立分类只是为了方便客户端识别
		MAGICSORT_BOMB_LOCKALL		= 30,			// 与MAGICSORT_BOMB处理相同，只是锁定全部目标
		MAGICSORT_SORB_SOUL			= 31,			// 吸魂魔法
		MAGICSORT_STEAL				= 32,			// 偷盗，随机从目标身上偷取power个物品
		MAGICSORT_LINE_PENETRABLE	= 33,			// 攻击者轨迹可以穿人的线性攻击

		//////////////////////////////////////////////
		// 新增幻兽魔法类型
		MAGICSORT_BLAST_THUNDER			= 34,			// 魔雷
		MAGICSORT_MULTI_ATTACHSTATUS	= 35,			// 群体施加状态
		MAGICSORT_MULTI_DETACHSTATUS	= 36,			// 群体解除状态
		MAGICSORT_MULTI_CURE			= 37,			// 群体补血
		MAGICSORT_STEAL_MONEY			= 38,			// 偷钱
		MAGICSORT_KO					= 39,			// 必杀技，目标血小于15%自动触发
		MAGICSORT_ESCAPE				= 40,			// 逃跑/救助
//		MAGICSORT_FLASH_ATTACK			= 41,			// 移形换位	
//		MAGICSORT_ATTRACK_MONSTER		= 42,			// 吸引怪物
		MAGICSORT_PASS_THROUGH			= 43,			// 冲刺

		MAGICSORT_PRECONDITION			= 44,
		MAGICSORT_SPACETIMEDOOR			= 55,	//时空门

		MAGICSORT_MONSTER_POINTATTACK	= 56,	//怪物的点攻击技能
		MAGICSORT_MONSTER_CIRCLEATTACK	= 57,	//怪物的范围技能攻击,以怪自身为圆点,一定半径的圆范围

		MAGICSORT_STOPTRAP				= 58,	//静止陷阱
		MAGICSORT_DRAGONHEART			= 59,	//龙之心
		MAGICSORT_BELLOW				= 60,	//强化嘲讽
		MAGICSORT_CONGEAL_SPRITGRID		= 61,	//原魂凝结
		MAGICSORT_DIMENSION_DOOR		= 62,	//瞬间移动 Dimension Door: Teleports you short distance.
		MAGICSORT_SWITCHSTATUS		    = 63,	///softworms-2007-08-13开关状态类型,如果状态存在,就去掉该状态,反之就添加该状态.
		MAGICSORT_CLEARSKILLCD          = 64,   // 清除技能CD [12/21/2007 陈剑飞]
		MAGICSORT_SUMMON				= 65,	//20080202修罗:召唤.
		MAGICSORT_TARGET_BACK			= 66,	//击退目标
		MAGICSORT_RANDOM_MULTI			= 67,	//随机攻击多个目标
		MAGICSORT_TIMESATK				= 68,	//连击
		MAGICSORT_WITHDRAWATK			= 69,	//后退并攻击
		MAGICSORT_BACK_CITY				= 90,	//回城
		MAGICSORT_EUDEMON_SOUL			= 98,	//宠物附体被动技能
		MAGICSORT_BACK_SYN				= 91,	//回帮
};
enum MAGICLEVADDTYPE
{
	MAGICLEVADDTYPE_POW		=	0,//技能威力加成
	MAGICLEVADDTYPE_MP		=	1,//耗蓝加成
	MAGICLEVADDTYPE_PRIVATECD	=	2,//私有CD加成
	MAGICLEVADDTYPE_PUBLICCD	=	3,//公共CD加成
	MAGICLEVADDTYPE_STEPSEC		=	4,//持续时间加成
	MAGICLEVADDTYPE_TIMES		=	5,//作用次数
	MAGICLEVADDTYPE_TARGET		=	6,//攻击目标上限
	MAGICLEVADDTYPE_POW2		=	7,//技能威力2加成
	MAGICLEVADDTYPE_STATUS_POW	=	8,//附加状态威力加成
};
//////////////////////////////////////////////////////////////////////
const int	WEAPONMAGIC_OFFSET				= 10000;	// 武器上带的魔法的最低ID值，同时是ITEMTYPE中MAGICx字段加此偏移等于MAGICTYPE的ID
const int	WEAPONMAGIC_MIN					= WEAPONMAGIC_OFFSET;
const int	WEAPONMAGIC_MAX					= WEAPONMAGIC_OFFSET+256;
const int	WEAPONMAGIC_LEVEL	= 0;					// 缺省的武器魔法等级
const int	WEAPONR_MAGICID		= 4200000001;			// 右手武器的魔法ID
const int	WEAPONL_MAGICID		= 4200000002;			// 左手武器的魔法ID

//////////////////////////////////////////////////////////////////////
const int	MAGICDAMAGE_ALT					= 26;		// 法术效果的高差限制
const int	AUTOLEVELUP_EXP					= -1;		// 自动升级的标志
const int	DISABLELEVELUP_EXP				= 0;		// 不升级的标志
const int	AUTOMAGICLEVEL_PER_USERLEVEL	= 10;		// 每10级，法术等级加一级
const int	USERLEVELS_PER_MAGICLEVEL		= 10;		// 玩家等级必须是法术等级的10倍

const int	KILLBONUS_PERCENT				= 5;		// 杀死怪物的额外EXP奖励
const int	KILLBONUS_PERCENT_TEAM			= 20;		//20060905修罗:小队杀死怪物的额外exp奖励
const int	HAVETUTORBONUS_PERCENT			= 10;		// 有导师的情况下额外EXP奖励
const int	WITHTUTORBONUS_PERCENT			= 20;		// 和导师一起练的情况下EXP奖励

const int	MAGIC_DELAY						= 1000;		// 魔法DELAY
const int	MAGIC_DECDELAY_PER_LEVEL		= 100;		// 每个“法术等级”减少的魔法DELAY
const int	RANDOMTRANS_TRY_TIMES			= 10;		// 随机瞬移的尝试次数
const int	DISPATCHXP_NUMBER				= 20;		// 加XP的数量
const int	COLLIDE_POWER_PERCENT			= 80;		// 冲撞增加攻击力的百分比
const int	COLLIDE_SHIELD_DURABILITY		= 3;		// 冲撞要减少的盾牌寿命
const int	LINE_WEAPON_DURABILITY			= 2;		// 直线攻击要减少的武器寿命
const int	MAX_SERIALCUTSIZE				= 10;		// 顺势斩的数量
const int	AWARDEXP_BY_TIMES				= 1;		// 按次数加经验值
const int	AUTO_MAGIC_DELAY_PERCENT		= 150;		// 连续魔法攻击时增加的DELAY
const int	BOW_SUBTYPE						= 500;		// 弓的SUBTYPE
const int	POISON_MAGIC_TYPE				= 1501;		// use for more status
const int	DEFAULT_MAGIC_FAN				= 120;		// 
//const int	STUDENTBONUS_PERCENT			= 5;		// 导师杀死一只怪物徒弟得到的经验百分比

const int	MAGIC_KO_LIFE_PERCENT			= 15;		// 必杀技能去血上限
const int	MAGIC_ESCAPE_LIFE_PERCENT		= 15;		// 逃跑技能有效的生命上限

		
const int	PERISH_TAGLIFE_PERCENT		= 20;		//技能不从者死，目标血量小于此百分比，才能使用
//魂技
const int	SPECIALSKILL_TYPE_PERISH1	= 6005;		//技能不从者死，
const int	SPECIALSKILL_TYPE_PERISH2	= 6102;		//不敬者亡
const int	SPECIALSKILL_TYPE_PERISH3	= 6302;		//不顺者夷

const int	SPECIALSKILL_TYPE_BURNSOUL	= 6802;		//技能焚魂射击

const int	SPECIALSILL_TYPE_BURNMANA	= 8001;		//技能燃魔射击，非魂技

const int	SPECIALSKILL_TYPE_BURNSOUL_MAX_POWER = 29999;// 焚魂射击最大伤害 [11/5/2007]
const int	SPECIALSILL_TYPE_BURNMANA_MAX_POWER = 10000;// 燃魔射击最大伤害

const int	SPECIALSKILL_TYPE_THROWICEBALL = 8002;

const int   OPPOSE_TAGLIFE_PERCENT = 50;            // 技能抗争，血量小于此百分比 [12/14/2007 陈剑飞]
const int   SPECIALSKILL_TYPE_OPPOSE = 4116;       // 抗争，血量小于百分比时降低伤害 [12/14/2007 陈剑飞]

//VIP学习技能折扣
const int	MAGIC_VIP_LEARN1	= 95;//1级VIP
const int	MAGIC_VIP_LEARN2	= 90;//2级VIP
const int	MAGIC_VIP_LEARN3	= 85;//3级VIP
//////////////////////////////////////////////////////////////////////
typedef	CGameData<MAGICDATA, szMagicTable, szID>	CMagicDataBase;
class	CMagicData : public IMagicData
{
protected:
	CMagicData()			{ m_pData = NULL;needsend = 0; m_nAddLev = 0;m_nLearn = 0;m_nIndex=-1;m_bNeedUpdate = false;}
	virtual ~CMagicData();
public: // overload
	static IMagicData* CreateNew()		{ return new CMagicData; }
	virtual int		GetMagicLevel();
	virtual void	ReleaseByOwner()	{ delete this; }
	virtual bool	Create(IRecordset* pRecordset/*,IDatabase* pDb = NULL*/);
	virtual bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);
	virtual bool	Create(ST_MAGIC_DATA* pMagicData);
	virtual int		GetInt(MAGICDATA nDataName);
	virtual void	SetInt(MAGICDATA idx, int nData);
	virtual void	SetStr(MAGICDATA idx, char* szData,int nSize);
	virtual bool	FindType();		//? 创建后，需要调用此函数手工同步

	virtual OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
	virtual bool	Update();
	virtual void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	virtual OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	virtual bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }
	virtual int		NeedSend()	;
	virtual int		GetIndex()			{ return m_nIndex; }
	virtual void	SetIndex(int nIndex){ m_nIndex = nIndex; }
		
protected:
	CMagicDataBase*	m_pData;
	bool m_bNeedUpdate;
	int  needsend;
	int m_nAddLev;
	int m_nLearn;
	int m_nIndex;//技能下标，宠物专用

protected: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

//////////////////////////////////////////////////////////////////////
typedef	ISingleObjSet2<IMagicData>		IMagicSet;
typedef	CSingleObjSet2<IMagicData>		CMagicSet;

//////////////////////////////////////////////////////////////////////
struct MagicInfoStruct
{
	OBJID	id;
	OBJID	idOwner;
	int		nType;
	int		nLevel;
	DWORD	dwExp;
	clock_t	updateClock;	// 王玉波 [5/30/2007]，转换地图组用到.此技能CD的更新时间
	int		nHabit;//影响CD的性格
	int		nAffect;//影响的数值,负数表示减少，正数表示增加
	USHORT	usCDIndex;//CD下标，宠物专用
};
//[2010-08-03 goto]目标数量超过技能攻击上限时，按距离排序，取距离小的目标
struct BeAttackSort 
{
	IRole* pRole;
	int	   nDistance;
};
////////////////////////////////////////////////////////////////
enum
{
	TYPE_MAGIC		= 0, // 魔法
	TYPE_XPSKILL	= 1, // xp技
	TYPE_KONGFU		= 2, // 武功招式
};

//////////////////////////////////////////////////////////////////////
enum
{
	MAGICSTATE_NONE		= 0,		// 无状态
	MAGICSTATE_INTONE	= 1,		// 吟唱状态
	MAGICSTATE_LAUNCH	= 2,		// 施展状态
	MAGICSTATE_DELAY	= 3,		// 延迟状态
	PHYSICAL_ATT        = 4,        // 物理攻击
};

//////////////////////////////////////////////////////////////////////
enum AUTO_ACTIVE{
	AUTOACTIVE_KILL			= 0x01,
	AUTOACTIVE_FOREVER		= 0x02,
	AUTOACTIVE_RANDOM		= 0x04,
	AUTOACTIVE_HIDDEN		= 0x08,		// 包含此掩码的魔法不显示在客户端
	AUTOACTIVE_EXPDISABLE	= 0x10,		// 包含此掩码的魔法使用过程中不能得到经验
};
enum MASK_BIT{
	MAGICMASKBIT_IGNOREPKDIABLE = 0x01,//可豁免地图PK限制
};
//////////////////////////////////////////////////////////////////////
enum THROW_WEAPON{
	THROW_WERPONR_TARGET			= 1,
};

//////////////////////////////////////////////////////////////////////
inline bool		IsWeaponMagic(int nType)			{ return nType >= WEAPONMAGIC_MIN && nType < WEAPONMAGIC_MAX; }
inline int		MagicType(int idx)					{ int nType = idx + WEAPONMAGIC_OFFSET; CHECKF(IsWeaponMagic(nType)); return nType; }
inline OBJID	POSITION2MAGIC_ID(int nPosition)	{ if(nPosition == ITEMPOSITION_WEAPONR) return WEAPONR_MAGICID; else return WEAPONL_MAGICID; }
void	DDALine(int x0, int y0, int x1, int y1, int nRange, vector<POINT>& vctPoint);

//////////////////////////////////////////////////////////////////////
typedef struct				// 合击时,幻兽相对位置
{
	DWORD	idEudemon;	
	USHORT	usOffsetX, usOffsetY;
} EudemonPos_ST;

// dwData [0][0][5][1][0][6] 代表(+05, -06)
//////////////////////////////////////////////////////////////////////
class CMagic  
{
protected:
	CMagic(PROCESS_ID idProcess, IRole* pRole);
	virtual ~CMagic();

private:
	CMagic() { m_bAutoAttack = false;}

public:
	static CMagic* CreateNew(PROCESS_ID idProcess, IRole* pRole);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	void	OnTimer(DWORD tCurr);
	void	ClearMagicCD();//清除技能CD 彭峰
public: // login
	bool	CreateAll();
	int		CreateAllCd();
	int		SendInfoToClient();

public: // MapGroup
	bool	SaveInfo();
	bool	GetInfoByIndex(MagicInfoStruct* pInfo, int idx);
	bool	AppendMagic(const MagicInfoStruct* pInfo);

public: // application
	IMagicSet*	QuerySet()		{ CHECKF(m_setData); return m_setData; }
	bool		MagicAttack(int nType, OBJID idTarget, int x, int y, UCHAR ucAutoActive = 0,bool needLearn = false,int learnLev = 0);		// return false: NO BROADCAST
	bool		AbortMagic(bool bSynchro=true/*false*/);// halei [5/25/2007] 更改终止魔法应该和客户端立即同步
	bool		TestMagicCastEnable(OBJID idMagicType,OBJID idTarget,int x,int y);

	bool		UserKillTarget(IRole* pTarget);
	bool		AutoAttackTarget(IRole* pTarget);
	IMagicData*	FindMagic(int nType);
	int         GetAtkRange();
	bool		AwardExp(int nType, int nExp = AWARDEXP_BY_TIMES);
	bool		AwardExp(int nType, int nBattleExp, int nExp);
//	bool		AwardExp(int nBattleExp, int nExp = AWARDEXP_BY_TIMES, bool bIgnoreFlag=false);		// 同时支持外调用，但bAddBattleExp应该为0。用于变身法术
	void		BreakAutoAttack();
	void		DebugReloadAll(){
					for(int i = 0; i < m_setData->GetAmount(); i++)
						m_setData->GetObjByIndex(i)->FindType();
				}

	bool		IsIntone()		{ return m_nMagicState == MAGICSTATE_INTONE; }	//return (m_tIntone.IsActive() && !m_tIntone.IsTimeOut()); }
	bool		IsInLaunch()	{ return m_nMagicState == MAGICSTATE_LAUNCH; }	//return (m_tApply.IsActive() && !m_tApply.IsTimeOut()); }
	bool		IsActive()		{ return (m_nMagicState != MAGICSTATE_NONE); }
	
	//-----彭峰---2007/06/08----在User魔法CD时或没有使用魔法时可以使用物理攻击
	BOOL		IsCanPhysicalAttack()		{ if( m_nMagicState == MAGICSTATE_DELAY || m_nMagicState == MAGICSTATE_NONE) return true; return false;}
	
	///[2009.5.19--张冬]获取公共CD
	DWORD GetPublicCDTime() { return m_nPublicCDTime;}
	DWORD GetPublicCDInterval() { return m_nPublicCDInterval;}
	int	  GetMagicScore();

public: // static
	static		CMagicTypeData*		FindMagicType	(int nType/*, int nLevel*/);
	static		int					CalcMagicPower	(IRole* pAtker, IRole* pTarget, int* pAdjustAtk=0);

protected:
	void		ShowMiss();
	int			Attack(int nType, IRole* pRole);		// return lose life
	int			Attack(int nType, int x, int y);		// return lose life
	int			Launch();
	bool		UpLevel(IMagicData* pMagic,int nLev, bool bSynchro = true);
	bool		CheckCondition(IMagicData* pData, 
		OBJID idTarget, 
		int x, 
		int y, 
		bool bSpiritSkill,
		char *szError=NULL);//20070614修罗:参数带入错误信息
	bool		CheckCrime(const vector<IRole*>* pSet);
	bool		CheckCrime(IRole* pRole);

	bool		LockTarget(bool bLock);
public: // const
	enum { HIT_BY_MAGIC, HIT_BY_WEAPON, HIT_BY_ITEM };
	bool		IsImmunity(IRole* pRole, bool bSendMsg=true);
	bool		HitByMagic();
	bool		HitByWeapon();
	int			GetDieMode();
	bool		IsKeepBow()						{ return false; }	//m_pMagic && m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_ATTACHSTATUS && m_pMagic->GetInt(MAGICDATA_STATUS) == STATUS_KEEPBOW; }
	int			GetPower(int nStepIndex = -1);
	OBJID		GetType();
//	int			GetApplyMS(int nStepIndex = -1);
	ATK_TYPE	GetAtkType() const;
public: // task
	bool		CheckType(int nType);
	bool		CheckLevel(int nType, int nLevel);

public: // create
	void		ResetDelay();
//	OBJID		LearnMagic(int nType, int nLevel=0, bool bSaveMagic=true);
	OBJID		LearnMagic(int nType, 
		                   int nLevel=0,
						   bool bSaveMagic=true,
						   int nGiftFlag = EM_NOT_GIFT_MAGIC,
						   DWORD dwAddLev = NULL,
						   DWORD dwLearn = NULL);
	void        ClearDelay();// 清除技能CD [12/21/2007 陈剑飞]

public:
//	bool		IsGroupMagic(IMagicData* pData)	{ return pData->GetInt(MAGICDATA_GROUP_TYPE) != MAGICGROUP_NONE; }
protected: // process
	bool		ProcessAttack();
	bool		ProcessAtkStatus();
	bool		ProcessRecruit();
	bool		ProcessCross();
	bool		ProcessLine();
	bool		ProcessFan();
	bool		ProcessBomb();
	bool		ProcessBombLockAll();
	bool		ProcessSquare();
	bool		ProcessAttach();
	bool		ProcessSwitchStatus();///状态开关处理函数,softworms2007-08-13
	bool		ProcessDetach();
	bool		ProcessJumpAttackFaild(int x, int y);
	bool		ProcessFlyStrike();								//天人闪击技能
	bool		ProcessDragonHeart();							//龙之心
	bool		ProcessRandomTrans();
	bool		ProcessDispatchXP();
	bool		ProcessCollideFail(int x, int y, int nDir);
	bool		ProcessCollide();
	bool		ProcessSerialCut();
	bool		ProcessCallTeamMember();
	bool		ProcessRecordTransSpell();
	bool		ProcessTransform();
	bool		ProcessAddMana();
	bool		ProcessLayTrap();
	bool		ProcessCallPet();
	bool		ProcessDecLife();
	bool		ProcessGroundSting();
	bool		ProcessReborn();
	bool		ProcessSorbSoul();
	bool		ProcessSteal();

	bool		ProcessDimensionDoor();
	bool		ProcessSummon();//20080202修罗:召唤
	bool		ProcessPreCondition();
	bool        ProcessClearCD();//  [12/21/2007 陈剑飞]
	////////////////////////////////////////////////////////
	// 新增幻兽魔法类型
	bool		ProcessBlastThunder();
	bool		ProcessMultiAttach();
	bool		ProcessMultiDetach();
	bool		ProcessMultiCure();
	bool		ProcessStealMoney();
	bool		ProcessKO();
	bool		ProcessEscape();
//	bool		ProcessFlashAttack();
//	bool		ProcessAttrackMonster();
	bool		ProcessPassThrough();
	bool		Process_StopTrap();
	bool		Process_Bellow();


	int			AdjustMagicPower(int nPower,int nMagicType, int nlev=1, MAGICLEVADDTYPE nAddType=MAGICLEVADDTYPE_POW);///调整玩家魔法值强度值,softworms-2007-09-05
//	int			AdditionPower(int nPower1,int nPower2);//对Power值进行加成.softworms-2007-09-05

	typedef std::vector<IRole*>	ROLE_SET;
	enum { LOCK_NONE, LOCK_ONE, LOCK_ALL };
	bool		CollectTargetSet_Bomb(ROLE_SET& setRole, POINT& pos, int nLockType = LOCK_NONE );

	bool		CollectTargetSet_Team(ROLE_SET& setRole);
	bool		CollectTargetSetByPos(ROLE_SET& setRole, POINT& pos);
	
protected: // spawn id
	static OBJID	SpawnMagicID()						{ return InterlockedIncrement(&m_lockNextID); }

protected: // data
	IMagicSet*		m_setData;

protected: // intone
	IMagicData*		m_pMagic;
	OBJID			m_idTarget;
	POINT			m_pos;
	int				m_nData;
	bool			m_bAutoAttack;
// 王玉波注释 [5/30/2007]---------begin-----------------------------------------------
//	int				m_nRawDelay;		// fast delay for trans map
//	int				m_nDelay;			// normal delay
//--------------------------------end-----------------------------------------------

	int				m_nMagicState;		// 魔法状态	-- zlong 2004-11-30

	CTimeOutMS		m_tIntone;			// 吟唱延时
	CTimeOutMS		m_tApply;			// 每次效果延时
	int				m_nApplyTimes;		// 作用效果次数
	CTimeOutMS		m_tDelay;			// 施展结束延时

	vector<OBJID>	m_setTargetLocked;	// 魔法锁定，有轨迹的目标集
	bool			m_bTargetLocked;
	vector<EudemonPos_ST>	m_setEudemonInfo;		// 合击2时幻兽相对位置
	int						m_nAttackSameAmount;	// 合击4 参与人数
private: // cheat --------
	OBJID			m_cheatTargetID;
	int				m_cheatCount;
	bool			m_bIsSpendMp;
	int				nSpendMp;
	DWORD           m_nPublicCDTime;     //上次公共CD时间
	DWORD           m_nPublicCDInterval; //公共CD时间
	int				m_nSpendAng;		 //消耗怒气
	bool			m_bIsSpendAng;
// XP add by arhun
public:
//	int GetMagicLevel(IMagicData* pData);
	bool MyProcessDo();
	BOOL HasSameWargameStatus(IRole* pRole);
	BOOL CanRecurit(IRole* pRole);	//  [2007-9-29] 朱斌	可否加血

	bool Process_SpaceTimeDoor();
//	bool Process_CongealSpiritStone();
	bool ProcessAtkBack();
	bool ProcessBackCity();
	bool ProcessBackSyn();
	bool ProcessRandomMultiAtk();
	bool ProcessTimesAtk();
	bool ProcessWithDrawAtk();
	int  ProcessSynMagic(CUser* pUser, IRole* pTargeter, int nSynRank=100);//帮派技能,职位默认为100，因为帮众也能学习所有帮派技能
	int	 ProcessSoulEudemonMagic(CUser* pUser, IRole* pTargeter);

//modify code by 林德渊 begin 2011.6.22
	int ProcessBuffBomb(IRole *m_pOwner,int flag);
	bool CollectTargetSet_BuffBomb(ROLE_SET& setRole, POINT& pos, int nLockType,int irange);
//modify code by 林德渊 end
	void        setMagicState(int state) { m_nMagicState = state;	}
	int         getMagicState(void) { return m_nMagicState;	}
	bool		CheckMagicLevAmount(int nLev, int nAmount, int nProf);
protected: // ctrl
	PROCESS_ID		m_idProcess;
	IRole*			m_pOwner;
	static long		m_lockNextID;
	CUser*          m_pUser;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAGIC_H__B2689767_419D_487F_B0F9_FBB04C4619AA__INCLUDED_)
