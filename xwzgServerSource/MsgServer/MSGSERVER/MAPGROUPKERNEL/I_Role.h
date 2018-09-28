// RoleManager.h: interface for the CRoleManager class.
// 
//////////////////////////////////////////////////////////////////////

#if !defined(I_ROLE_HEADER)
#define I_ROLE_HEADER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "../share/define.h"
#include "GameObjSet.h"
#include "TimeOut.h"

enum { POSE_STAND=0, };

const int	ADJUST_PERCENT			= 30000;						// ADJUST时，>=30000 表示百分数
const int	ADJUST_SET				= -30000;						// ADJUST时，<=-30000 表示等于(-1*num - 30000)
const int	ADJUST_FULL				= -32768;						// ADJUST时，== -32768 表示填满
const int	DEFAULT_DEFENCE2		= 10000;						// 数据库缺省值

//帮派级别
//06.4.24王玉波添加
const int SYNRANK_SIX	= 6;
const int SYNRANK_FIVE	= 5;
const int SYNRANK_FOUR	= 4;
const int SYNRANK_THREE	= 3;
const int SYNRANK_TWO	= 2;
const int SYNRANK_ONE	= 1;

const DWORD TIME_FOOTPRINT	= 1000;

const int	TEAM_STATUS_REQ_ROLES	= 3;		// 结界状态生效需要的最少组队人数

const int	MAX_FRENZY_LIFE_PERCENT	= 70;		// 生命超过这个百分比就会取消狂暴状态


///////////////////////////////////////
// 修改魔法系统需要的
const int	ARROW_FLY_ALT					= 60;							// 箭支飞行高度


//---帮派---begin
//////////////////////////////////////////////////////////////////////
//06.4.21王玉波修改
enum {
		//////////////////////////
		//通过个位数来判断角色所属的堂
		/////////////////////////
		RANK_LEADER			= 100,			//军团团长		
		RANK_SUBLEADER		= 90,			//副团长
		RANK_FEELLEADER		= 85,			//精英
		RANK_NORMAL			= 10,			//普通成员		
	//////////////////////--jay以下废弃
		RANK_FEELLEADER1	= 81,			//堂主1
		RANK_FEELLEADER2	= 82,			//堂主2
		RANK_FEELLEADER3	= 83,			//堂主3
		RANK_FEELLEADER4	= 84,			//堂主4
		RANK_SUBFEELLEADER1	= 71,			//副堂主1
		RANK_SUBFEELLEADER2	= 72,			//副堂主2
		RANK_SUBFEELLEADER3	= 73,			//副堂主3
		RANK_SUBFEELLEADER4	= 74,			//副堂主4
		RANK_FEELMENBER1	= 61,			//分堂1精英
		RANK_FEELMENBER2	= 62,			//分堂2精英
		RANK_FEELMENBER3	= 63,			//分堂3精英
		RANK_FEELMENBER4	= 64,			//分堂4精英
		RANK_NORMAL1		= 51,			//分堂1帮众
		RANK_NORMAL2		= 52,			//分堂2帮众
		RANK_NORMAL3		= 53,			//分堂3帮众
		RANK_NORMAL4		= 54,			//分堂4帮众

		RANK_SHOP			= 20,			//果农

		RANK_NONE			= 0,
};

enum
{
	SYNMEM_LEVEL_ONE		= 1,
	SYNMEM_LEVEL_TWO		= 2,
	SYNMEM_LEVEL_THREE		= 3,
	SYNMEM_LEVEL_FOUR		= 4,
	SYNMEM_LEVEL_FIVE		= 5,		
};

//---帮派---end
//////////////////////////////////////////////////////////////////////
enum 
{
		DIE_NORMAL			= 1,
		DIE_CRITICALHIT		= 2,
		DIE_MAGIC			= 3,
		DIE_FLOAT			= 4,
		DIE_ARROWSHOOT		= 5,
};

//////////////////////////////////////////////////////////////////////
enum { OWNER_NONE, OWNER_USER, OWNER_SYN, OWNER_NPC,OWNER_RACE/*softworms-2007-08-11-种族拥用地图*/ };

//************************************************************************************************************************************************
// 外部状态，需要通过MsgPlayer通知其它玩家的状态。与客户端同步
const I64		KEEPEFFECT_NORMAL						=	0x0000000000000000; //	无特殊状态
const I64		KEEPEFFECT_DIE							=	0x0000000000000001;	//	死亡
const I64		KEEPEFFECT_CRIME						=	0x0000000000000002;	//	犯罪
const I64		KEEPEFFECT_ORANGE						=	0x0000000000000004;	//	得到“时空之泪”//橙名
const I64		KEEPEFFECT_TEAMLEADER					=	0x0000000000000008;	//	队长
const I64		KEEPEFFECT_PK_PROTECT					=	0x0000000000000010;	//	PK保护
const I64		KEEPEFFECT_DISAPPEARING					=	0x0000000000000020; // 尸体消失状态
const I64		KEEPEFFECT_ATTRACTMONSTER				=	0x0000000000000040;	//吸引怪物

const I64		KEEPEFFECT_DOT_POISONING				=	0x0000000000000080;	//中毒
const I64		KEEPEFFECT_DOT_LIFEMISSING				=	0x0000000000000100;	//流血
const I64		KEEPEFFECT_DOT_MANAMISSING				=	0x0000000000000200;	//精神流失
const I64		KEEPEFFECT_DOT_STRENGTHMISSING			=	0x0000000000000400;	//体力流失
const I64		KEEPEFFECT_DOT_BURNING					=	0x0000000000000800;	//灼伤

const I64		KEEPEFFECT_FRENZY						=	0x0000000000001000;	//狂暴	
const I64		KEEPEFFECT_SHIELD						=	0x0000000000002000;	//护盾
const I64		KEEPEFFECT_FIRMNESS_MAGIC				=	0x0000000000004000;	//坚定施法
const I64		KEEPEFFECT_HIDDEN						=	0x0000000000008000;	//隐身
const I64		KEEPEFFECT_RETORT						=	0x0000000000010000;	//反击
const I64		KEEPEFFECT_LURK							=	0x0000000000020000;	//潜行
const I64		KEEPEFFECT_IMMUNE_ALL					=	0x0000000000040000;	//无敌
const I64		KEEPEFFECT_SLEEP						=	0x0000000000080000;	//战斗不能（被攻击可打断）
const I64		KEEPEFFECT_DREAMING						=	0x0000000000100000;	//战斗不能（被攻击不可打断）
const I64		KEEPEFFECT_PALSY						=	0x0000000000200000;	//定身，麻痹(只是不能移动)
const I64		KEEPEFFECT_SILENCE						=	0x0000000000400000;	//沉默
const I64		KEEPEFFECT_CONFUSION					=	0x0000000000800000;	//混乱
const I64		KEEPEFFECT_STOPSAY						=	0x0000000001000000;	//禁言
const I64		KEEPEFFECT_RED							=	0x0000000002000000;	// 红名


const I64		KEEPEFFECT_PHYSICALATTACKUP				=	0x0000000004000000;						//物理攻击力上升
const I64		KEEPEFFECT_PHYSICALATTACKDOWN			=	0x0000000008000000;						//物理攻击力下降
const I64		KEEPEFFECT_MAGICATTACKUP				=	0x0000000010000000;						//魔法攻击力上升
const I64		KEEPEFFECT_MAGICATTACKDOWN				=	0x0000000020000000;						//魔法攻击力下降
const I64		KEEPEFFECT_ATTACKUP						=	0x0000000040000000;						//攻击力上升
const I64		KEEPEFFECT_ATTACKDOWN					=	0x0000000080000000;						//攻击力下降
const I64		KEEPEFFECT_PHYSICALDEFENCEUP			=	0x0000000100000000;						//物理防御上升
const I64		KEEPEFFECT_PHYSICALDEFENCEDOWN			=	0x0000000200000000;						//物理防御下降
const I64		KEEPEFFECT_MAGICDEFENCEUP				=	0x0000000400000000;						//魔法防御上升
const I64		KEEPEFFECT_MAGICDEFENCEDOWN				=	0x0000000800000000;						//魔法防御下降
const I64		KEEPEFFECT_DEFENCEUP					=	0x0000001000000000;						//防御力上升
const I64		KEEPEFFECT_DEFENCEDOWN					=	0x0000002000000000;						//防御力下降
const I64		KEEPEFFECT_HITRATEUP					=	0x0000004000000000;						//命中率上升
const I64		KEEPEFFECT_HITRATEDOWN					=	0x0000008000000000;						//命中率下降
const I64		KEEPEFFECT_DODGERATEUP					=	0x0000010000000000;						//躲避率上升
const I64		KEEPEFFECT_DODGERATEDOWN				=	0x0000020000000000;						//躲避率下降
const I64		KEEPEFFECT_CRITRATEUP					=	0x0000040000000000;						//增加致命一击机率
const I64		KEEPEFFECT_CRITRATEDOWN					=	0x0000080000000000;						//减少致命一击机率
const I64		KEEPEFFECT_MAXLIFEUP					=	0x0000100000000000;						//最大生命值增加
const I64		KEEPEFFECT_MAXLIFEDOWN					=	0x0000200000000000;						//最大生命值减少
const I64		KEEPEFFECT_MAXMANAUP					=	0x0000400000000000;						//最大魔法值增加
const I64		KEEPEFFECT_MAXMANADOWN					=	0x0000800000000000;						//最大魔法值减少

const I64		KEEPEFFECT_MAXSPUP						=	0x0001000000000000;		//最大体力值增加
const I64		KEEPEFFECT_MAXSPDOWN					=	0x0002000000000000;		//最在体力值减少
const I64		KEEPEFFECT_MOVESPEEDUP					=	0x0004000000000000;		//移动速度增加
const I64		KEEPEFFECT_MOVESPEEDDOWN				=	0x0008000000000000;		//移动速度减少
const I64		KEEPEFFECT_ATKSPEEDUP					=	0x0010000000000000;		//攻击速度增加
const I64		KEEPEFFECT_ATKSPEEDDOWN					=	0x0020000000000000;		//攻击速度减少
const I64		KEEPEFFECT_PHYSICALDAMAGEUP				=	0x0040000000000000;		//物理伤害增大
const I64		KEEPEFFECT_PHYSICALDAMAGEDOWN			=	0x0080000000000000;		//物理伤害减小
const I64		KEEPEFFECT_MAGICDAMAGEUP				=	0x0100000000000000;		//魔法伤害增大
const I64		KEEPEFFECT_MAGICDAMAGEDOWN				=	0x0200000000000000;		//魔法伤害减小
const I64		KEEPEFFECT_DAMAGEUP						=	0x0400000000000000;		//伤害增大
const I64		KEEPEFFECT_DAMAGEDOWN					=	0x0800000000000000;		//伤害减小
const I64		KEEPEFFECT_GETEXPUP						=	0x1000000000000000;		//经验获得增加
const I64		KEEPEFFECT_GETEXPDOWN					=	0x2000000000000000;		//经验获得减少
const I64		KEEPEFFECT_BROWN						=	0x4000000000000000;		//潜能爆发  //棕名
const I64		KEEPEFFECT_BLACK						=	0x8000000000000000;		//寒霜甲	//黑名


//////////////////////////////////////////////////////////////////////
// 综合状态
//enum {
const I64		KEEPEFFECT_NOT_CLEAR		=KEEPEFFECT_TEAMLEADER|KEEPEFFECT_RED|KEEPEFFECT_GETEXPUP;// 玩家死亡不清此状态


const I64		SECONDKEEPEFFECT_CRAZE							=	0x0000000000000001;	//	狂热
const I64		SECONDKEEPEFFECT_BEATING						=	0x0000000000000002;	//  打击DOT
const I64		SECONDKEEPEFFECT_SUFFERING						=	0x0000000000000004;	// 缠绕dot
const I64		SECONDKEEPEFFECT_DETECTION						=	0x0000000000000008;	//力擒，反击，识破
const I64		SECONDKEEPEFFECT_RESOURCEWAR_PROTECT			=	0x0000000000000010;	//资源战保护状态
const I64		SECONDKEEPEFFECT_DOT_ADDLIFE					=	0x0000000000000020;	//加血DOT
const I64		SECONDKEEPEFFECT_DOT_ADDMANA					=	0x0000000000000040;	//加蓝DOT
const I64		SECONDKEEPEFFECT_ENIMYFIRE						=	0x0000000000000080;	//仇视-softworms-2007-08-13
const I64       SECONDKEEPEFFECT_LIFEUP                         =   0x0000000000000100;// 新buf，HP上限提高 [1/30/2008 陈剑飞]
const I64       SECONDKEEPEFFECT_COMB_MAIN                      =   0x0000000000000200; //合体主buff
const I64       SECONDKEEPEFFECT_COMB_SECONDARY                 =   0x0000000000000400; //合体次buff
const I64       SECONDKEEPEFFECT_DA_BAO			                =   0x0000000000000800;//[游途道標 2009.03.18]大爆
const I64       SECONDKEEPEFFECT_ZHONG_BAO		                =   0x0000000000001000;//[游途道標 2009.03.18]中爆
const I64       SECONDKEEPEFFECT_XIAOBAO						=   0x0000000000002000;//[游途道標 2009.03.18]小爆
const I64       SECONDKEEPEFFECT_CITYWAR_WEAPON					=   0x0000000000004000; //城战武器
const I64       SECONDKEEPEFFECT_SNEER      					=   0x0000000000008000; //嘲讽特效
const I64       SECONDKEEPEFFECT_LIFESTONE      				=   0x0000000000010000; //血晶石特效
const I64       SECONDKEEPEFFECT_CITYATTACk      				=   0x0000000000020000; //城战攻方标志
const I64       SECONDKEEPEFFECT_CITYDEFENCE      				=   0x0000000000040000; //城战守方标志
const I64       SECONDKEEPEFFECT_BASEDOWN      					=   0x0000000000080000; //基本属性减少
const I64       SECONDKEEPEFFECT_PVP     						=   0x0000000000100000; //pvp
const I64       SECONDKEEPEFFECT_YABIAO     					=   0x0000000000200000; //押镖
const I64       SECONDKEEPEFFECT_JIEBIAO     					=   0x0000000000400000; //劫镖
const I64       SECONDKEEPEFFECT_LIANGONG     					=   0x0000000000800000;
const I64       SECONDKEEPEFFECT_ACTIONEND     					=   0x0000000001000000;
const I64       SECONDKEEPEFFECT_EUDEMONEXPUP   				=   0x0000000002000000;

//************************************************************************************************************************************************
//
//状态ID的法则
//ID共5位
//第1位：状态大的区分	buff	1;
//						debuff	2;
//						dot 	3;
//						其它	4.
//第2位：数值计算的标识	百分比				1,
//						数值				2,
//						控制系				3
//						其它				4
//第3,4,5无特殊意义，序号......
const int	BIGTYPE_BUFF	= 1;
const int	BIGTYPE_DEBUFF	= 2;
const int	BIGTYPE_DOT		= 3;
const int	BIGTYPE_OTHER	= 4;

const int	DATAIDENT_PERCENT	= 1;
const int	DATAIDENT_NUMBER	= 2;
const int	DATAIDENT_CONTROL	= 3;
const int	DATAIDENT_OTHER		= 4;
//************************************************************************************************************
//状态编号规则：ID共五位,
//第一位：BigType，细节buff 1,debuff 2,dot 3,其它 4
//第二位：数值标识,百分比：1 数值2 控制系 3 其它 4
//第三、四、五位：序号
//如果有特殊buff在 Cuser::IsBadlyStatus处理（第一位为 2和3的 BUFF 会在PK结束后给清掉，别坑爹~~）
//////////////////////////////////////////////////////////////////////
// 内部状态，不需要通过MsgPlayer通知其它玩家的状态。服务器内部使用(cq_magictype使用)
enum {
		STATUS_NORMAL		=	44001,								//普通状态	
		STATUS_DIE			=	44002,								//死亡
		STATUS_DABAO		=	44017,								//[游途道標 2009.03.18]大爆
		STATUS_ZHONGBAO		=	44018,								//[游途道標 2009.03.18]中爆
		STATUS_XIAOBAO		=	44019,								//[游途道標 2009.03.18]小爆
		STATUS_CRIME		=	54003,								//犯罪状态
//07.4.28王玉波，把STATUS_SKZL ID改到BUFF里，可以发到客户端
//		STATUS_SKZL			=	44004,								//获得时空之泪，防御，魔御各加100	
		STATUS_TEAMLEADER	=	44005,								//队长,
		STATUS_PKVALUE		=	44006,								//PK值
		STATUS_DETACH_BADLY	=	44007,								//去掉所有不良状态						//不能在SetStatus(),ClsStatus()中出现
		STATUS_DETACH_ALL	=	44008,								//去掉所有状态，不包括"犯罪状态"		//不能在SetStatus(),ClsStatus()中出现
		STATUS_DETACH_SOMEBAD = 44009,                             // 去掉一些不良状态 [12/26/2007 陈剑飞]
	
//07.4.28王玉波，把STATUS_PK_PROTECT ID改到BUFF里，可以发到客户端
//		STATUS_PK_PROTECT	=	44009,								//PK保护状态
		STATUS_DISAPPEARING	=	44010,								//尸体消失状态
		STATUS_ATTRACTMONSTER	= 44011,								//吸引怪物（只有玩家才有）
		STATUS_PARRY			= 44012,								//招架（减伤，降物理攻击力）			//不能在SetStatus(),ClsStatus()中出现
		STATUS_RECOVER			= 44013,								//恢复术（去掉中毒，流血）				//不能在SetStaus(),ClsStatus()中出现
		STATUS_GLARE			= 44014,								//眩目之刺(给对方加防御减少，命中减少)	//不能在SetStaus(),ClsStatus()中出现
		STATUS_AZRAEL_HAND		= 44015,								//死神之握								//不能在SetStaus(),ClsStatus()中出现
		STATUS_ATTACKMORE		= 44016,								//死神之握								//不能在SetStaus(),ClsStatus()中出现
		STATUS_FORTITUDE_WILL   = 41016,                             // 不屈意志 [12/25/2007 陈剑飞]
		STATUS_VERIFICATION_WAIT	=44100,								//跑商验证中
		STATUS_VERIFICATION_RUNNING=44101,								//跑商中
		STATUS_DETACH_STOPMOVE = 41017,								// 狂暴 [2/1/2008 陈剑飞]
		STATUS_BIZ_DEAD		   = 41018,								// goto 跑商死亡后的buff
		STATUS_TUTOR_BOSS	   = 35001,								// 师徒任务打特殊怪物需要的Buff

 		STATUS_DOT_POISONING		=	34001,							//中毒DOT			,掉hp
 		STATUS_DOT_LIFEMISSING		=	34002,							//流血DOT			,掉hp
// 		STATUS_DOT_MANAMISSING		=	34003,							//精神流失DOT		,掉mp
// 		STATUS_DOT_STRENGTHMISSING	=	34004,							//体力流失DOT		,掉sp
 		STATUS_DOT_BURNING			=	34005,							//灼伤DOT			,掉hp
 		STATUS_DOT_SUFFERING		=	34006,							//痛苦缠绕			,掉hp
// 		STATUS_DOT_BEATING			=	34007,							//打击，			 掉hp
 		STATUS_DOT_ADDLIFE			=	34008,							//加血DOT
 		STATUS_DOT_ADDMANA			=	34009,							//加蓝DOT
// 		STATUS_DOT_ADDLIFE_RB		=	34010,							//20070827修罗:加血DOT.复苏之风regeneration blow （RB）
// 		STATUS_DOT_ADDMANA_MC		=	34011,							//20070827修罗:加蓝DOT.精神集中mind concentrade （MC）
// 		STATUS_DOT_ENMITY           =   34012,                          //强制仇恨DOT
		STATUS_DOT_LIFEMISS1		=	34013,							//流血DOT，掉hp,为了可以叠加而新加的状态
// 		
// 		STATUS_DOT_BEATINGPRE       =   31012,                          //陈剑飞 致命打击2，以百分比掉血
// 		STATUS_DOT_SUFFERING_PRE    =   31013,                          // 痛苦缠绕2，百分比掉血 [12/13/2007 陈剑飞]
// 		STATUS_DOT_POISON_ATK       =   31014,                          // 毒袭,毒云喷射调用毒袭效果 [12/13/2007 陈剑飞]
// 		STATUS_DOT_QUICK_ADDLIFE    =   32015,                         // 快速恢复 [12/25/2007 陈剑飞]
// 		STATUS_DOT_ADDLIFE_L		=	34016,							//加血DOT
// 		STATUS_DOT_ADDMANA_L		=	34017,							//加蓝DOT
		STATUS_DOT_REDMANA			=	34018,					//减蓝DOT
		STATUS_DOT_REDSP			=	34019,					//减怒气DOT
		STATUS_DOT_WARGAMEEXP		=	34995,					//竞技场加经验buff
		STATUS_DOT_ADDEXP			=	34996,					//活动专用加经验buff( m_nParam2 决定在那副地图有经验 ,0 是全地图)
		STATUS_DOT_TUTOREXP			=	34997,					//师徒喝茶buf
		STATUS_ACTIVATE_ADDEXP		=	34998,					//奇迹盛典buff：每分钟获得一定经验
		STATUS_BOOTH_ADDEXP			=	34999,					//摆摊时自动获得经验BUFF
		STATUS_ITEM_ADDEXP			=	35000,					//物品给的加经验buff
		STATUS_DOT_SYNEXP			=	35002,					//帮派加经验buff
//		STATUS_SYNWORKTIME	        =	36000,					//帮派打工时间buff

		STATUS_FRENZY				=	11019,					//狂暴	
		STATUS_ERUPTPOT_PERCENT		=	11020,					//潜能爆发
		STATUS_SHIELD				=	12001,					//一定时间内可抵消N点消害	护盾
		STATUS_FIRMNESS_MAGIC		=	12002,					//一定时间内增加魔法不被打断的机率	坚定施法
		STATUS_HIDDEN				=	14001,					//隐身	
		STATUS_RETORT				=	14002,					//反击	
		STATUS_LURK					=	14003,					//NPC看不到自己，对玩家无效	潜行	
		STATUS_IMMUNE_ALL			=	14004,					//免疫一切伤害	无敌
		STATUS_CRAZE				=	14005,					//狂热，用MP抵消物理伤害
		STATUS_DETECTION			=	14006,					//识破（力擒）
		STATUS_RESOURCEWAR_PROTECT	=	14007,					//资源战保护状态，相当于无敌,不能让技能使用
		STATUS_PVPMODE				=	14008,					//决斗buff
		STATUS_YABIAO				=	14009,					//押镖buff
		STATUS_JIEBIAO				=	14013,					//劫镖buff
		STATUS_LIANGONG				=	14014,
		STATUS_ACTIONEND			=	14015,                  //同上在结束时能触发action
		STATUS_SLEEP				=	23001,					//不能移动、攻击、技能、使用物品	战斗不能（被攻击可解除）	
		STATUS_DREAMING				=	23002,					//不能移动、攻击、技能、使用物品	战斗不能（被攻击不解除）	
		STATUS_PALSY				=	23003,					//不能移动	定身，麻痹	
		STATUS_SILENCE				=	23004,					//不能使用需要吟唱的技能	沉默	
		STATUS_CONFUSION			=	23005,					//鼠标乱转，丢失目标	混乱	
		STATUS_STOPSAY				=	24001,					//不能说话，非控制系	禁言	
		STATUS_FROSTSHIELD			=	12009,					//寒霜甲，增加物理防御，被攻击时，降低对手移动速度

		STATUS_SKZL					=	22011,								//获得时空之泪，减速	
		STATUS_PK_PROTECT			=	14010,								//PK保护状态
		STATUS_STRUGGLE_RELIVE		=	14011,								//20070827修罗:死地后生struggle relive （SR）
		STATUS_NEW_PK_PROTECT		=	14012,					//  [2/22/2008 朱斌]	新PK保护,动了也不会掉
		STATUS_SHIELD_1				=	12039,					//一定时间内免疫伤害	护盾
		STATUS_SHIELD_2				=	12045,					//一定时间内免疫伤害	护盾

		//百分比系列------begin----------------------*******************************************************
		STATUS_PHYSICALATTACKUP_PERCENT		=	11001,				//物理攻击力上升
		STATUS_PHYSICALATTACKDOWN_PERCENT	=	21001,				//物理攻击力下降
		STATUS_MAGICATTACKUP_PERCENT		=	11002,				//魔法攻击力上升
		STATUS_MAGICATTACKDOWN_PERCENT		=	21002,				//魔法攻击力下降
		STATUS_ATTACKUP_PERCENT				=	11003,				//攻击力上升
		STATUS_ATTACKDOWN_PERCENT			=	21003,				//攻击力下降
		STATUS_PHYSICALDEFENCEUP_PERCENT	=	11004,				//物理防御上升
		STATUS_PHYSICALDEFENCEDOWN_PERCENT	=	21004,				//物理防御下降
		STATUS_MAGICDEFENCEUP_PERCENT		=	11005,				//魔法防御上升
		STATUS_MAGICDEFENCEDOWN_PERCENT		=	21005,				//魔法防御下降
		STATUS_DEFENCEUP_PERCENT			=	11006,				//防御力上升
		STATUS_DEFENCEDOWN_PERCENT			=	21006,				//防御力下降
		STATUS_HITRATEUP_PERCENT			=	11007,				//命中率上升
		STATUS_HITRATEDOWN_PERCENT			=	21007,				//命中率下降
		STATUS_DODGERATEUP_PERCENT			=	11008,				//躲避率上升
		STATUS_DODGERATEDOWN_PERCENT		=	21008,				//躲避率下降	
		STATUS_CRITRATEUP_PERCENT			=	11009,				//致命一击增加		
		STATUS_CRITRATEDOWN_PERCENT			=	21009,				//致命一击减少
		STATUS_MAXLIFEUP_PERCENT			=	11010,				//最大生命值增加
		STATUS_MAXLIFEDOWN_PERCENT			=	21010,				//最大生命值减少
		STATUS_MAXMANAUP_PERCENT			=	11011,				//最大魔法值增加
		STATUS_MAXMANADOWN_PERCENT			=	21011,				//最大魔法值减少
		STATUS_MAXSPUP_PERCENT				=	11012,				//最大体力值增加
		STATUS_MAXSPDOWN_PERCENT			=	21012,				//最在体力值减少
		STATUS_MOVESPEEDUP_PERCENT			=	11013,				//移动速度增加
		STATUS_MOVESPEEDDOWN_PERCENT		=	21013,				//移动速度减少
		STATUS_ATKSPEEDUP_PERCENT			=	11014,				//攻击速度增加
		STATUS_ATKSPEEDDOWN_PERCENT			=	21014,				//攻击速度减少
		STATUS_PHYSICALDAMAGEUP_PERCENT		=	21015,				//物理伤害增大
		STATUS_PHYSICALDAMAGEDOWN_PERCENT	=	11015,				//物理伤害减小
		STATUS_MAGICDAMAGEUP_PERCENT		=	21016,				//魔法伤害增大
		STATUS_MAGICDAMAGEDOWN_PERCENT		=	11016,				//魔法伤害减小
		STATUS_DAMAGEUP_PERCENT				=	21017,				//伤害增大
		STATUS_DAMAGEDOWN_PERCENT			=	11017,				//伤害减小
		STATUS_GETEXPUP_PERCENT				=	11018,				//经验获得增加
		STATUS_GETEXPDOWN_PERCENT			=	21018,				//经验获得减少
		STATUS_BASE_DOWN_PERSENT            =   21019,
		STATUS_PHYSICALATTACKUP_PERCENT_WS	=	11021,				//20070827修罗:物理攻击力上升.战地之歌war song （WS）
		STATUS_MAGICATTACKUP_PERCENT_ED		=	11022,				//20070827修罗:魔法攻击力上升.能量专注energy Determined （ED）
		STATUS_DAMAGEDOWN_PERCENT_TN		=	11023,				//20070827修罗:伤害减小.顽强似钉tough nail （TN）
		STATUS_GETEXPUP_PERCENT_WT			=	11024,				//20070827修罗:经验获得增加.天赋异禀well talent （WT）
		STATUS_PHYATTACKUP_TUTOR_PERCENT	=	11025,				//师徒组队加攻击
		STATUS_MAGATTACKUP_TUTOR_PERCENT    =   11026,              // 师徒组队加魔法攻击 [陈剑飞10/27/2007]
		STATUS_SELF_DAMAGEUP				=	11027,				//自身对敌伤害增加
		STATUS_SELF_DAMAGEUP_PERCENT		=	11028,				//自身对敌伤害增加,与上面可叠加

		STATUS_GETEXPUP_PERCENT_SYNINS		=	11090,				//帮派副本获得打怪双倍

		//点数系列--------begin------------------------****************************************************
		STATUS_MAGICDEFENCEUP_NUMBER		=	12003,				//魔法防御增加（点数）
		STATUS_MAGICDEFENCEUP_NUMBER_1		=	12040,				//魔法防御增加（点数）//为了可能叠加，多定义一个
		STATUS_MAGICDEFENCEDOWN_NUMBER		=	22003,				//魔法防御减少（点数）
		STATUS_PHYSICALDEFENCEUP_NUMBER		=	12004,				//物理防御增加（点数）
		STATUS_PHYSICALDEFENCEDOWN_NUMBER	=	22004,				//物理防御减少（点数）
		STATUS_PHYSICALATTUP_NUMBER	    	=	12019,				//物理攻击增加（点数） //张冬修改
		STATUS_PHYSICALATTDOWN_NUMBER   	=	22019,				//物理攻击减少（点数） //张冬修改
		STATUS_DEFENCEUP_NUMBER				=	12005,				//防御增加(点数)
		STATUS_DEFENCEDOWN_NUMBER			=	22005,				//防御减少(点数)
		STATUS_CRITRATEUP_NUMBER			=	12006,				//增加致命一击机率（点数）
		STATUS_CRITRATEDOWN_NUMBER			=	22006,				//减少致命一击机率（点数）
		STATUS_HITRATEUP_NUMBER				=	12007,				//命中率增加(点数)
		STATUS_HITRATEDOWN_NUMBER			=	22007,				//命中率减少
		STATUS_DODGERATEUP_NUMBER			=	12008,				//闪避率增加
		STATUS_DODGERATEDOWN_NUMBER			=	22008,				//闪避率减少
		STATUS_PHYSICALDEFENCEUP_NUMBER_SF	=	12010,				//20070827修罗:物理防御增加（点数）信仰之盾shield of faith （SF）
		STATUS_MAGICDEFENCEUP_NUMBER_EM		=	12011,				//20070827修罗:魔法防御增加（点数）洞察神秘expose the myth （EM）
		STATUS_CRITRATEUP_NUMBER_SW			=	12012,				//20070827修罗:增加致命一击机率（点数）寻找弱点seek weakpoints （SW）

		STATUS_LIFE_UP                      =   12013,              // 生命值提升 [1/30/2008 陈剑飞]

		STATUS_BIGBLOOD_USERNOT             =   24002,               // 不能使用增值药水 [1/15/2008 陈剑飞]

		STATUS_MOUNTSPEED_NUMBER            =   30000,              // 坐骑BUF [陈剑飞8/2/2007]     

		STATUS_ENEMYFIRE					=	22009,				//仇视,本族地图可被攻击.
		STATUS_INVALID						=	99999,				//无效状态,不处理AttachStatus

		STATUS_WARGAME						=	30001,				//  [2007-8-21] 朱斌 玩家参加游戏活动的BUF

        STATUS_COMB_MAIN                    =   12015,
		STATUS_COMB_SECONDARY               =   12016,

		STATUS_FISH_FOOD                    =   12017,             //鱼饵
		STATUS_FISH_WORKER                  =   12018,             //鱼工
		STATUS_FISH_TUTOR                   =   12031,             //钓鱼大师
		STATUS_CITYWAR_WEAPON               =   12032,             //城战武器
		STATUS_CITYWAR_SAFETIME             =   12033,             //城战安全时间
		STATUS_DEAD_ADDTEAMATTR             =   12034,             //死后给队友加血
		STATUS_CITYWAR_ATTACKMARK           =   12035,             //攻方标志
		STATUS_CITYWAR_DEFENCEMARK          =   12036,             //守方标志

		STATUS_LOCK_TARGET					=	22010,				//  [12/19/2007 朱斌]	只能攻击某一个目标,配合技能 挑战 6405
//		STATUS_BUFF_HPMP                    =   12014,              //同时加血和加蓝
		STATUS_MAGICDEFENCEUP_NUMBER_L		=	12020,				//魔法防御增加（点数）
		STATUS_PHYSICALDEFENCEUP_NUMBER_L	=	12021,				//物理防御增加(点数)
		STATUS_STRENGUP_NUMBER				=	12022,				//增加力量
		STATUS_STRENGDOWN_NUMBER			=	22022,				//减少力量
		STATUS_INTUP_NUMBER					=	12023,				//增加智力
		STATUS_INTDOWN_NUMBER				=	22024,				//减少智力
		STATUS_MAGIC_SHIELD					=	12024,				//魔法盾，受到伤害时扣蓝，蓝不够了扣血
		STATUS_SPENDMP_UP					=	21023,				//蓝消耗增加

		STATUS_STAUP_NUMBER					=	12025,				//增加体质
		STATUS_STADOWN_NUMBER				=	22025,				//减少体质
		STATUS_SPIUP_NUMBER					=	12026,				//增加精神
		STATUS_SPIDOWN_NUMBER				=	22026,				//减少精神
		STATUS_AGIUP_NUMBER					=	12027,				//增加敏捷
		STATUS_AGIDOWN_NUMBER				=	22027,				//减少敏捷
		STATUS_TOUCHUP_NUMBER				=	12028,				//增加坚韧
		STATUS_TOUGHDOWN_NUMBER				=	22028,				//减少坚韧
		STATUS_SELF_DAMAGEUP_NUMBER			=	12030,				//自身对敌伤害增加
		STATUS_FOURATTUP_NUMBER				=	12037,				//四属性增加
		STATUS_FOURATTDOWN_NUMBER			=	22037,				//四属性减少
		STATUS_BASEATTUP_NUMBER				=	12038,				//五属性增加
		STATUS_MONSTER_ENMITY				=	22038,				//嘲讽，强制怪物攻击施术者，需要改NPC，暂时不用,用仇恨值来做
		STATUS_DEFDOWN_ATKUP				=	12041,				//以防换攻，外防下降，外攻增加
		STATUS_FIREDEF_UP					=	12046,				//火防
		STATUS_ICEDEF_UP					=	12047,				//冰防
		STATUS_MYSTICDEF_UP					=	12048,				//玄防
		STATUS_NOXDEF_UP					=	12049,				//毒防
		//goto 定制装备附带的效果
		STATUS_EQUIP_DEFUP					=	12042,				//内外防增加	属定制装备
		STATUS_EQUIP_DAMAGEUP				=	12043,				//对敌伤害增加  属定制装备
		STATUS_EQUIP_CRIUP					=	12044,				//暴击增加		属定制装备
	
		STATUS_EQUIP_BEDAMAGEUP				=	22039,				//受到伤害增加	属定制装备
		STATUS_EQUIP_DEFDOWN				=	22040,				//内外防降低	属定制装备
		
		//武魂附带效果12051--12059

		STATUS_SOUL_CRI						=	12051,				//暴击			武魂
		STATUS_SOUL_HITRATE					=	12052,				//命中			武魂
		STATUS_SOUL_DODGE					=	12053,				//闪避			武魂
		STATUS_SOUL_OUTERATT				=	12054,				//外功			武魂
		STATUS_SOUL_INNERATT				=	12055,				//内功			武魂
		STATUS_SOUL_OUTERDEF				=	12056,				//外防			武魂
		STATUS_SOUL_INNERDEF				=	12057,				//内防			武魂
		STATUS_SOUL_REBLOOD					=	12058,				//回血			武魂
		STATUS_SOUL_REMP					=	12059,				//回魔			武魂


		STATUS_SOUL_REBLOOD_PER				=	12101,				//回血百分比
//modify code by 林德渊 begin 2011.6.22
		STATUS_RETORT_TENSEC				=	21024,				//10秒轮转一次
		STATUS_RETORT_LGWS					=	21025,				//烈骨无伤		A-BOSS
//		STATUS_RETORT_BACK2					=	21026,				//风卷残云		B-BOSS战斗开始后BOSS每10秒钟对以BOSS所在坐标点3*3格范围外14*14格范围内所有玩家造成一个群体AOE伤害，在范围内的玩家不受此技能影响
//		STATUS_RETORT_BACK4					=	21027,				//追魂斧		B-BOSS战斗开始后BOSS每40秒钟随机对一个玩家释放一个BUFF该BUFF会在5秒以后造成一个3*3码的范围伤害，该伤害较高
		STATUS_RETORT_ZHF					=	21028,				//追魂斧		B-BOSS5秒以后造成一个3*3码的范围伤害，该伤害较高
		STATUS_RETORT_MDSX					=	21029,				//迷毒噬心		C-BOSS前等待20秒
		STATUS_RETORT_MDSX_BUFF2			=	21030,				//迷毒噬心		C-BOSS BUFF2
		STATUS_RETORT_MDSX_1				=	21031,				//迷毒噬心		C-BOSS 定时减血BUFF
//modify code by 林德渊 end
		//帮派
		STATUS_SYN_HITRATEDOWN				=   22041,				//命中降低
		STATUS_SYN_SHIELD					=	12050,				//一定时间内免疫伤害	护盾

		//宠物附体
		STATUS_EUDEMON_SOUL                 =   12060,              //宠物附体，全属性加10

		//宠物给主人加的BUFF，因为要与人加的重叠，所以另外定义
		STATUS_STRENGUP_NUMBER_PET			=	12722,				//增加力量
		STATUS_STRENGDOWN_NUMBER_PET		=	22722,				//减少力量
		STATUS_SPIUP_NUMBER_PET				=	12726,				//增加精神
		STATUS_SPIDOWN_NUMBER_PET			=	22726,				//减少精神
		STATUS_AGIUP_NUMBER_PET				=	12727,				//增加敏捷
		STATUS_AGIDOWN_NUMBER_PET			=	22727,				//减少敏捷
		STATUS_STAUP_NUMBER_PET				=	12725,				//增加体质
		STATUS_STADOWN_NUMBER_PET			=	22725,				//减少体质
		STATUS_INTUP_NUMBER_PET				=	12723,				//增加智力
		STATUS_INTDOWN_NUMBER_PET			=	22724,				//减少智力

		STATUS_PET_DODGE					=	12724,//宠物附体加闪避
		STATUS_PET_HITRATE					=	12728,//宠物附体加命中
};

//////////////////////////////////////////////////////////////////////
struct StatusInfoStruct;
class IStatus
{
public:
	virtual OBJID	GetID()							PURE_VIRTUAL_FUNCTION_0
	virtual ULONG	ReleaseByOwner()				PURE_VIRTUAL_FUNCTION_0

	virtual bool		IsValid()							PURE_VIRTUAL_FUNCTION_0
	virtual void		SetPower(int data)					PURE_VIRTUAL_FUNCTION
	virtual int			GetPower()							PURE_VIRTUAL_FUNCTION_0
	virtual int			GetParam1		()					PURE_VIRTUAL_FUNCTION_0
	virtual int			GetParam2		()					PURE_VIRTUAL_FUNCTION_0
	virtual bool		GetInfo(StatusInfoStruct* pInfo)	PURE_VIRTUAL_FUNCTION_0
	virtual bool		OnTimer(DWORD tCurr)				PURE_VIRTUAL_FUNCTION_0
	virtual bool		ChangeData(int nPower,int nSecs,int nTimes=0,int nParam1=0,int nParam2=0,int nParam3=0,OBJID idOwner=0)		PURE_VIRTUAL_FUNCTION_0
	virtual bool		IncTime(int nMilliSecs, int nLimit)		PURE_VIRTUAL_FUNCTION_0
	virtual bool		ToFlash()							PURE_VIRTUAL_FUNCTION_0
	virtual int			GetParam3()							PURE_VIRTUAL_FUNCTION_0	//06.11.4王玉波添加	,06.11.13王玉波修改				
	virtual int			GetSec()							PURE_VIRTUAL_FUNCTION_0	//  [3/31/2008 朱斌]	得持续时间
	virtual int			GetSecLeft()						PURE_VIRTUAL_FUNCTION_0
	virtual void		SetFreeze()							PURE_VIRTUAL_FUNCTION
	virtual void		SetUnFreeze()						PURE_VIRTUAL_FUNCTION
};

typedef	IGameObjSet<IStatus>		IStatusSet;
typedef	CGameObjSet<IStatus>		CStatusSet;

////////////////////////////////////////////////////////////////
enum {	MOVEMODE_WALK=0,					// PathMove()的模式
		MOVEMODE_RUN,
		MOVEMODE_SHIFT,						// to server only
		MOVEMODE_JUMP,
		MOVEMODE_TRANS,
		MOVEMODE_CHGMAP,
		MOVEMODE_JUMPMAGICATTCK,
		MOVEMODE_COLLIDE,
		MOVEMODE_SYNCHRO,					// to server only
		MOVEMODE_TRACK,

		MOVEMODE_RUN_DIR0 = 20,

		MOVEMODE_RUN_DIR7 = 30,
};

////////////////////////////////////////////////////////////////
const int _DELTA_X[11]={ 0, 0, 1, 1, -1, 0, 1, -1, -1 , 0 , 0};
const int _DELTA_Y[11]={ 0, -1, -1, 0, -1, 0, 1, 0, 1 , 1 , 0};
const int MAX_DIRSIZE = 11;
inline int GetDirByPos(int nFromX, int nFromY, int nToX, int nToY)		// return MAX_DIRSIZE: error
{
	if(nFromX < nToX)
	{
		if(nFromY < nToY)
			return 6;
		else if(nFromY > nToY)
			return 2;
		else
			return 3;
	}
	else if(nFromX > nToX)
	{
		if(nFromY < nToY)
			return 8;
		else if(nFromY > nToY)
			return 4;
		else
			return 7;
	}
	else // if(nFromX == nToX)
	{
		if(nFromY < nToY)
			return 9;
		else if(nFromY > nToY)
			return 1;
	}
	return MAX_DIRSIZE;
}
inline int		MaskLook(int look)							{ return look/10; }
inline int		MaskDir(int look)							{ return look%10; }
inline DWORD	MakeLookFace(int nMaskLook, int nMaskDir)	{ return (nMaskLook*10) + (nMaskDir%10); }
inline float	GetRadian(float posSourX, float posSourY, float posTargetX, float posTargetY)
{
	CHECKF(posSourX != posTargetX || posSourY != posTargetY);

	float PI = static_cast<float>(3.1415926535);
	float fDeltaX = posTargetX - posSourX;
	float fDeltaY = posTargetY - posSourY;
	float fDistance	= sqrt(fDeltaX*fDeltaX + fDeltaY*fDeltaY);
	CHECKF(fDeltaX <= fDistance && fDistance > 0);
	float fRadian = asin(fDeltaX / fDistance);

	return fDeltaY > 0 ? (PI/2 - fRadian) : (PI + fRadian + PI/2);
}

//////////////////////////////////////////////////////////////////////
class IRole;
class IMapThing
{
public: // base
	virtual void	ReleaseByOwner()							PURE_VIRTUAL_FUNCTION
	virtual OBJID	GetID()										PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetObjType()								PURE_VIRTUAL_FUNCTION_0
	virtual int		GetPosX()									PURE_VIRTUAL_FUNCTION_0
	virtual int		GetPosY()									PURE_VIRTUAL_FUNCTION_0
	virtual void	SendShow(IRole* pRole)						PURE_VIRTUAL_FUNCTION

	// return true: ppv返回对象指针
	virtual bool	QueryObj(OBJID idObjType, void** ppv)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	QueryRole(void** ppv)						PURE_VIRTUAL_FUNCTION_0
};

//20080111修罗:发送npc标志------------
enum EM_SENDNPCFLAG
{
	EM_SENDNPCFLAG_FALSE	= 0,//不发送给npc
		EM_SENDNPCFLAG_TRUE,//正常发送.如果本房间有npc则发送
		EM_SENDNPCFLAG_ENFORCE,//强制.无论本房间有没有npc
};
//------------------------------------

/////////////////////////////////////////////////////////////////////
class IMapThing;
class IStatus;
class CTransformation;
class CTeam;
class CItem;
class CMagic;
class IRole
{
protected:
	IRole(){}
	~IRole(){}
public:
	virtual void	ReleaseByOwner		()						PURE_VIRTUAL_FUNCTION
public: // get attrib
	virtual OBJID	GetID				()						PURE_VIRTUAL_FUNCTION_0
	virtual LPCTSTR	GetName				()						PURE_VIRTUAL_FUNCTION_0
//	virtual LPCTSTR	GetMate				()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetLookFace			()						PURE_VIRTUAL_FUNCTION_0
	//以后要分解成下面两个接口
	//virtual int	GetLook				()						PURE_VIRTUAL_FUNCTION_0
	//virtual int	GetFace				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetHair				()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetPosX				()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetPosY				()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetDir				()						PURE_VIRTUAL_FUNCTION_0
//	virtual	BOOL	CheckStatus			(int nStatus)			PURE_VIRTUAL_FUNCTION_0
	virtual I64		GetEffect			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetLev				()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetLife				()						PURE_VIRTUAL_FUNCTION_0
//	virtual DWORD	GetMana				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMaxLife			()						PURE_VIRTUAL_FUNCTION_0
//	virtual DWORD	GetMaxMana			()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetAtk				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMgcAtk			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetAttack			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetAtkHitRate		()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetAddAttByType     (int nType)				PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetAtkType			()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetDef				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetDefence			()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetDex				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetDexterity		()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetDdg				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetDodge			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetCrit				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetTough			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD 	AdjustDamage(DWORD nDamage,int nAttribType) PURE_VIRTUAL_FUNCTION_0

//	virtual DWORD	GetMagicAtk			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMagicDef			()						PURE_VIRTUAL_FUNCTION_0

	virtual	DWORD	GetInterAtkRate		()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetIntervalAtkRate	()						PURE_VIRTUAL_FUNCTION_0

	virtual int		GetHelmetTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetArmorTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetWeaponRTypeID	()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetWeaponLTypeID	()						PURE_VIRTUAL_FUNCTION_0
	virtual int     GetDressTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int     GetWingTypeID		()						PURE_VIRTUAL_FUNCTION_0//翅膀
//	virtual int		GetNecklaceTypeID	()						PURE_VIRTUAL_FUNCTION_0
//	virtual int 	GetRingRTypeID		()						PURE_VIRTUAL_FUNCTION_0
//	virtual int 	GetRingLTypeID		()						PURE_VIRTUAL_FUNCTION_0
//	virtual int 	GetShoesTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetMountTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetMantleTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetWeaponSpecialView()						PURE_VIRTUAL_FUNCTION_0

//	virtual DWORD 	GetMedalSelect		()						PURE_VIRTUAL_FUNCTION_0
//	virtual DWORD	GetTitleSelect		()						PURE_VIRTUAL_FUNCTION_0

	virtual int		GetDistance			(int x, int y)			PURE_VIRTUAL_FUNCTION_0
	virtual int		GetDistance			(IMapThing* pTarget)	PURE_VIRTUAL_FUNCTION_0

	virtual CGameMap*	GetMap			()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetSynID			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetSynRank			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetSynRankShow		()						PURE_VIRTUAL_FUNCTION_0

	virtual bool	IsSimpleMagicAtk	()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetSimpleMagicHitRate()						PURE_VIRTUAL_FUNCTION_0

	virtual int		GetTutorExp			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetTutorLevel		()						PURE_VIRTUAL_FUNCTION_0
	virtual UCHAR	GetMercenaryRank	()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMercenaryExp		()						PURE_VIRTUAL_FUNCTION_0
	virtual UCHAR	GetNobilityRank		()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetExploit			()						PURE_VIRTUAL_FUNCTION_0
public: // const

//	virtual int		GetSex				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsAlive				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsTalkEnable		()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsGM				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsPM				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsMonster			()						PURE_VIRTUAL_FUNCTION_0
	//20070316修罗
	virtual BOOL	IsUser				()						PURE_VIRTUAL_FUNCTION_0
//	virtual bool	IsScene				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsDelThis			()						PURE_VIRTUAL_FUNCTION_1
	virtual bool	IsVirtuous()								PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsEvil()									PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsBowman()									PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsShieldEquip()								PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsWing()									PURE_VIRTUAL_FUNCTION_0
	// return true: ppv返回对象指针
	virtual bool	QueryObj			(OBJID idObjType, void** ppv)	PURE_VIRTUAL_FUNCTION_0
	virtual IMapThing*	QueryMapThing	()						PURE_VIRTUAL_FUNCTION_0
	virtual IRole*	FindAroundRole		(OBJID id)				PURE_VIRTUAL_FUNCTION_0
	virtual CUser*	QueryOwnerUser		()						PURE_VIRTUAL_FUNCTION_0

public:	// modify attrib ------------------------------
	virtual void	SetDir				(int nDir)							PURE_VIRTUAL_FUNCTION
	virtual	void	SetStatus			(int nStatus, bool bSynchro=true,int nPower=0)						PURE_VIRTUAL_FUNCTION
	virtual	void	ClsStatus			(int nStatus, bool bSynchro=true)						PURE_VIRTUAL_FUNCTION

	virtual	void	AddBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )		PURE_VIRTUAL_FUNCTION//07.4.26王玉波添加
	virtual void	UpdateBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )	PURE_VIRTUAL_FUNCTION//07.4.26王玉波添加

	virtual void    ClsBuff				(int nStatus)						PURE_VIRTUAL_FUNCTION
	virtual bool	AddAttrib			(int idxAttr, __int64 i64Data, int bSynchro)					PURE_VIRTUAL_FUNCTION_0
	virtual void	ProcessOnMove		(int nMoveMode)						PURE_VIRTUAL_FUNCTION
	virtual void	ProcessAfterMove	()									PURE_VIRTUAL_FUNCTION
	virtual void	GetFootPrint		(int& x, int& y)					PURE_VIRTUAL_FUNCTION

	virtual bool	SpendArrow			()									PURE_VIRTUAL_FUNCTION_0

	virtual	IStatus*	QueryStatus(OBJID idType)										PURE_VIRTUAL_FUNCTION_0
	virtual	IStatusSet*	QueryStatusSet()												PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	AwardBattleExp		(int nExp, bool bGemEffect=true, bool bIncludeOwner=true)		PURE_VIRTUAL_FUNCTION_0
	virtual void	SetFightPause		(int nInterval)									PURE_VIRTUAL_FUNCTION
	virtual void	SaveInfo			()												PURE_VIRTUAL_FUNCTION

public: // money --------------------------------
//	virtual bool	SpendMoney			(DWORD nMoney, bool bSynchro)			PURE_VIRTUAL_FUNCTION_0
//	virtual bool	GainMoney			(DWORD nMoney, bool bSynchro)			PURE_VIRTUAL_FUNCTION_0

public: // interactive
//	virtual bool	AddItem				(CItem* pItem)								PURE_VIRTUAL_FUNCTION_0
//	virtual bool	IsItemFull			()											PURE_VIRTUAL_FUNCTION_0

public: // system
	//20080111修罗:
	//-------------------
	//virtual void	BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
	virtual void	BroadcastRoomMsg(CNetMsg* pMsg, 
		bool bSendSelf = false,
		EM_SENDNPCFLAG sendNpc = EM_SENDNPCFLAG_TRUE)	PURE_VIRTUAL_FUNCTION
	//-------------------
	virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
//	virtual void	BroadcastMapMsg(CNetMsg* pMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
//	virtual void	BroadcastMapMsg(LPCTSTR szMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
	virtual bool	SendMsg(CNetMsg* pMsg)									PURE_VIRTUAL_FUNCTION_0
	virtual bool	SendSysMsg(const char* fmt, ...)						PURE_VIRTUAL_FUNCTION_0
	virtual bool	SendSysMsg(int nChannel, const char* fmt, ...)			PURE_VIRTUAL_FUNCTION_0
	virtual void	OnTimer(time_t tCurr)									PURE_VIRTUAL_FUNCTION

public: // map
	virtual int		MoveToward(int nDir, bool bSync = true)					PURE_VIRTUAL_FUNCTION_0
	virtual void	TransPos(int nPosX, int nPosY)							PURE_VIRTUAL_FUNCTION
	virtual void	JumpPos(int nPosX, int nPosY)							PURE_VIRTUAL_FUNCTION
	virtual bool	SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction)			PURE_VIRTUAL_FUNCTION_0
	virtual void	ActiveTransPos(int nPosX, int nPosY)					PURE_VIRTUAL_FUNCTION
	virtual bool	SendSelfToBlock()										PURE_VIRTUAL_FUNCTION_0

	virtual void	AddToBCRoleSet			(OBJID idRole, bool bSendMsg)	PURE_VIRTUAL_FUNCTION
	virtual void	RemoveFromBCRoleSet		(OBJID idRole,bool bClrBigBc = false)					PURE_VIRTUAL_FUNCTION
	virtual void	AddToBCMapItemSet		(OBJID idMapItem)				PURE_VIRTUAL_FUNCTION
	virtual void	RemoveFromBCMapItemSet	(OBJID idMapItem)				PURE_VIRTUAL_FUNCTION
	
public: // fight ------------------------------
	virtual void	ClrAttackTarget		()									PURE_VIRTUAL_FUNCTION
	virtual bool	SetAttackTarget(IRole* pTarget = NULL)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	AutoSkillAttack(IRole* pTarget)							PURE_VIRTUAL_FUNCTION_0
	virtual int		Attack( IRole* pTarget,DWORD& dwBattleAddInfoRef )		PURE_VIRTUAL_FUNCTION_0
	virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower,int addi = 0, bool bReflectEnable=true)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	BeEnmityAttack(bool bMagic, IRole* pTarget,int addi = 0, bool bReflectEnable=true)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	MyEnmityClr(void)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	ReleaseSleep(void)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y,bool needLearn = false,int learnLev = 0)	PURE_VIRTUAL_FUNCTION_0
	virtual void	Kill(IRole* pTarget, DWORD dwDieWay)									PURE_VIRTUAL_FUNCTION
	virtual void	BeKill(IRole* pTarget = NULL)							PURE_VIRTUAL_FUNCTION
	virtual bool	IsAtkable(IRole* pTarget, bool bSendHint=false)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsBeAtkable()											PURE_VIRTUAL_FUNCTION_0
	virtual int		GetAttackRange(int nTargetSizeAdd)						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetSizeAdd()											PURE_VIRTUAL_FUNCTION_0
	virtual void	SendDamageMsg( OBJID idTarget, int nDamage,DWORD dwBattleAddInfo )				PURE_VIRTUAL_FUNCTION
public:	// 为了修改魔法系统而增加的部分，等魔法系统修改完再做整理
	virtual void	AwardSynWarScore(CNpc* pNpc, int nScore)				PURE_VIRTUAL_FUNCTION
	virtual bool	IsImmunity(IRole* pRole)								PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetProfession()											PURE_VIRTUAL_FUNCTION_0
	virtual bool	SpendEquipItem(int nSubType, int nNum, bool bSynchro)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	MagicSpendItem(int nSubType, int nNum, bool bSynchro)	PURE_VIRTUAL_FUNCTION_0
	virtual int		AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg=false)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	DecEquipmentDurability(bool bBeAttack, bool bMagic, int bDurValue=1)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsEmbedGemType(int nGemType)							PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetMana()												PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMaxMana()											PURE_VIRTUAL_FUNCTION_0

	virtual int		GetEnergy()												PURE_VIRTUAL_FUNCTION_0
	virtual bool	CheckWeaponSubType(int nSubType, int nNum = 0)			PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetPotential()											PURE_VIRTUAL_FUNCTION_0
	virtual CTransformation* QueryTransformation()							PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsArrowPass(int x, int y, int nAlt=ARROW_FLY_ALT)		PURE_VIRTUAL_FUNCTION_0
	virtual CTeam*	GetTeam()												PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	GetEquipItemByPos(int nPosition)						PURE_VIRTUAL_FUNCTION_0
	virtual bool	CheckCrime(IRole* pRole)								PURE_VIRTUAL_FUNCTION_0
	virtual void	SetDelay(int nDelay)									PURE_VIRTUAL_FUNCTION

	virtual CItem*	GetWeaponR			()									PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	GetWeaponL			()									PURE_VIRTUAL_FUNCTION_0

	virtual bool	DropItem(OBJID idItem, int x, int y)					PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetSoulSum()											PURE_VIRTUAL_FUNCTION_0
	virtual void	AddEquipmentDurability(int nPosition, int nAddValue)	PURE_VIRTUAL_FUNCTION
	virtual bool	SynPosition(int nPosX, int nPosY, int nMaxDislocation = 8)						PURE_VIRTUAL_FUNCTION_0
	virtual bool	Transform(DWORD dwType, int nKeepSecs, bool bSynchro = true)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posCenter)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	AddItem (CItem* pItem, bool bSynchro, bool bUpdate = true)						PURE_VIRTUAL_FUNCTION_0
	virtual bool	Synchro()												PURE_VIRTUAL_FUNCTION_0
	virtual int		AdjustData(int nData, int nAdjust, int nMaxData=0)		PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	FindSpaceTransSpell()									PURE_VIRTUAL_FUNCTION_0

	virtual	CMagic*	QueryMagic()											PURE_VIRTUAL_FUNCTION_0
	virtual bool	IgnoreTargetDodge()										PURE_VIRTUAL_FUNCTION_0	
	virtual bool	IsMustCritHit()											PURE_VIRTUAL_FUNCTION_0
};




#endif // !defined(I_ROLE_HEADER)
