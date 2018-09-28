// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__BF5C20F1_5DBE_4E65_9099_E96EFAD7ECBA__INCLUDED_1)
#define AFX_USER_H__BF5C20F1_5DBE_4E65_9099_E96EFAD7ECBA__INCLUDED_1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)
#include "GameBlock.h"
#include "UserData.h"
#include "Friend.h"
#include "Tutor.h"
#include "Item.h"
#include "Timeout.h"
#include "SquareDeal.h"
#include "gamemap.h"
#include "battleSystem.h"
#include "UserWeaponSkill.h"
#include "Myheap.h"
#include "T_SingleObjSet2.h"
#include "T_SingleMap2.h"
#include "team.h"
#include "magic.h"
#include "math.h"
#include "Package.h"
#include "SynAttr.h"
#include "../share/protocol.h"
#include "booth.h"
#include "autolink.h"
#include "Enemy.h"
#include "AutoPtr.h"
#include "UserPackage.h"
#include "Role.h"
#include "GemCreate.h"//20061010修罗
#include "TaskDetailData.h"//20061109修罗
#include "UserGift.h"//20071123修罗:
#include "StrRes.h"
#include "CityWarMgr.h"
#include "UserPet.h"
#include "CompensationManager.h"
#include "StatusManager.h"
#include "CDManager.h"//-----彭峰 2007/03/12--- 
#include "FriendContactHistory.h" //2008/12/17独孤求败
#include "UserKey.h"
#include "UserMaterialItem.h"
#include "MsgMarry.h"
#include "UserJinMeiSysData.h"
#include <vector>
#include <deque>
#include <set>
using namespace std;

#include "UserTable.h"		// 各种表定义
#include "IMarryObject.h"
#include "SKMail.h"
#include "WormSolt.h"
#include "UserSaveItem.h"
#include "MarryPackage.h"
#include "AutoFightData.h"
#include "MsgFriend.h"
#include "MsgTeamPos.h"
#include "..\SHARE\define.h"	// Added by ClassView
#include "UserTitle.h"
#include "Treasure.h"
#include "OnlineGift.h"
#include "TutorList.h"
#include "UserTutor.h"
#include "VipList.h"
#include "UserWarGame.h"
#include "PetEgg.h"
#include "WargameManager.h"
#include "SpecilLineInfo.h"
//modify code by 林德渊 begin 2011.5.30
#include "VasLvData.h"
//modify code by 林德渊 end
#include "Verification.h"
////////////////////////////////////////////////////////////////

const int	_MAX_USERFRIENDSIZE				= 50;
const int	_MAX_TUTORSIZE					= 15;
const int	_MAX_MONEYLIMIT					= 1000000000;
const int   _MAX_PKSAVE_VALUE               = 50000;                        //pk值对物品赎回影响界限
const int	USER_PACKAGE_SIZE				= 36;							// 仓库最多存多少东西
const int	USERDROPITEM_RANGE				= 5;							// 自动掉
const int	USERDROPITEM_RANGE2				= 9;							// 手动扔
//const int	ARROW_FLY_ALT					= 60;							// 箭支飞行高度
const int	JUMP_ALT						= 200;							// 跳的最高高度
const int	FLY_ALT							= 100;							// 飞的最高高度
const int	PERCENT_DECDUR					= 5;							// 装备升级失败后渐耐久的几率
const int	CRIME_KEEP_SECS					= /*60*2*/20;							// 20070226修罗:10分钟 犯罪维持时间
const int   SELF_DEFENCE_SECS               = 20;                           // 正当防卫持续时间
const int	SYNCRIME_KEEP_SECS				= 30;							// 帮派犯罪维持时间
const int	MAX_MENUTASKSIZE				= 20;							// 菜单系统中"回答TASK"的最大数量

const int	PKVALUE_HALFKILL				= 2;
const int	PKVALUE_ONEKILL					= 1/*10*/;//[游途道标 2009.06.03]                           //20070226修罗:10 每杀死一个白名/红名玩家，增加PK值   // add huang 2004.1.11
const int	PKVALUE_BLUE					= -120;
const int	PKVALUE_ORANGE					= PKVALUE_ONEKILL*6;
const int	PKVALUE_WHITE					= 0;	
const int	PKVALUE_BROWN					= PKVALUE_ONEKILL*16;			// 20070226修罗 原60
const int	PKVALUE_REDNAME					= PKVALUE_ONEKILL*11;							// 20070226修罗:同PKVALUE_ONEKILL 红名玩家最低PK值
const int	PKVALUE_BLACKNAME				= PKVALUE_ONEKILL*15;							// 20070226修罗:原240 黑名玩家最低PK值

const int	CHANCE_MAKEHOLE					= 300;

const int	SUPERMAN_KILLMONSTER_INCTIME		= 800;				// 无双状态每杀一个怪物的加时(MS)
const int	SUPERMAN_KILLMONSTER_TIMELIMIT		= 120*1000;			// 无双状态延长的时间限制(MS)
const int	TORNADO_KILLMONSTER_INCTIME			= 1000;				// 旋风状态每杀一个怪物的加时(MS)
const int	TORNADO_KILLMONSTER_TIMELIMIT		= 120*1000;			// 旋风状态延长的时间限制(MS)

const int	MASK_CHANGELOOK					= 10000000;						// 变身时LOCKFACE的掩码，face=face + new_face*10000000
const int	MAX_SIZEADD						= 2;							// 最大的SizeAdd，用于魔法区域的放大搜索(注意该数据不要太大)

const int	CHGMAP_LOCK_SECS				= 7;							// 进入地图后的保护时间
const int	PICK_MORE_MONEY					= 1000;							// 捡钱多时，发ACTION消息
const int	SYNCHRO_SECS					= 5;							// 同步消息5秒内同步

const int	IDENT_FEE						= 100;							// 鉴定的价格
const int	UNIDENT_SALE_FEE				= 1;							// 卖未鉴定的价格

const int	LOGOUTTIME						= 10;					//20070602修罗:退出读秒



const int	ADJUST_EXPFORMAT_LEVEL			= 70;			// 70级以上调整经验存储格式为/10

// 新开为100级 [陈剑飞11/12/2007]
//[游途道标 2009.05.04]等级限制,封顶改为45级
//[2009.5.13--张冬]等级限制,现在封顶改为50级
//[游途道标 2009.06.09]50=>60
const int	USERMAXLEVEL					= 100;			//20070808修罗:最高等级80.

const int	SAVE_POINT_RATE					= 20000;		// 每分钟 [9/18/2007 %PENGFENG%]
const int	SAVE_POINT_MAXTIME				= 300;			//分钟

/////////////////////////////////////////////////////////////////////////
// 导师系统相关
const int	ADDSTUDENT_TUTOREXP				= 10;			// 招收徒弟需要消耗的导师经验
const int	DELSTUDENT_TUTOREXP				= 1000;			// 开除学员扣的导师经验
const int	LEAVETUTOR_LEVEL				= 10;			// 10级以上玩家叛师要扣经验
const int	_MIN_GIVEEXP_LEV				= 30;			// 学员为导师提供奖励经验的最低等级
const int	TUTOREXP_PERCENT				= 40;			// 一级学员奖励给导师的经验比例
const int	TUTOREXP2_PERCENT				= 10;			// 二级学员奖励给导师的经验比例
const int	GETEXP_PER_TUTOREXP0			= 500;			// 初级导师消耗1点导师经验能够获得的学员贡献经验
const int	GETEXP_PER_TUTOREXP1			= 700;			// 中级导师消耗1点导师经验能够获得的学员贡献经验
const int	GETEXP_PER_TUTOREXP2			= 1000;			// 高级导师消耗1点导师经验能够获得的学员贡献经验


//---jinggy---帮派---begin
const int	SYNREPUTE_SECS					= 6*60;		// 玩家每在线6分钟，加10点军团 声望和贡献度。
const int	SYNREPUTE_VAL_PEREACH			= 10;		    
const int	SYNREPUTE_VAL_PROFFER			= 1;		// 玩家每在线6分钟，加一点军团贡献度。

#define		SYNMEM_TIMEONLINE_INTERVAL		15*60			//每15分钟更新一次帮派成员在线时间
#define		SYNMEM_TIMEONLINE_INCREPUTE		10 //③、	军团成员每人每在线6分钟，军团声望增加10点
//---jinggy---帮派---end
//---jinggy---铸造---begin
const int	EQUIPMENT_EXP_ADD_SECS			= 10*60;						// 每10分钟增加一次小于9级的武器经验
const int	EQUIPMENT_EXP_ADD_SECS_AFTER9	= 15*60;						// 每15分钟增加一次9级的武器经验
const int	WEAPON_UPEXP_MAXVALUE			= 8;
const int	ARMOR_UPEXP_MAXVALUE			= 10;
const int	NECKLACE_UPEXP_MAXVALUE			= 12;
const int	SHOE_UPEXP_MAXVALUE			= 14;
const int	BANGLE_UPEXP_MAXVALUE		= 16;
const int	MAX_LEVEL_WARLEVEL	= 9;
const int   DEC_WARGHOSTEXP_FORREPAIR = 3 ;
//---jinggy---铸造---end

const int	PROFFER_BEKILL					= 1;					// 每被敌对帮派玩家杀死一次减少的帮派贡献度
const int	SYNREPUTE_BEKILL				= 1;					// 每次被敌对帮派玩家杀死减少的帮派声望

const int	NEWBIE_LEVEL					= 40;			// 新手的等级
const int	_MIN_TUTOR_LEVEL				= 35;			// 导师最低等级
const int	GRAUDUATE_LEVEL					= 50;			// 最低出师等级
enum { FLYMAP_ERROR=-1, FLYMAP_NORMAL, FLYMAP_MAPGROUP };
enum { ATTACK_TIME=false, BEATTACK_TIME=true, BY_WEAPON=false, BY_MAGIC=true };
enum { valueMask_ALL=0, valueMask_CONST, valueMask_PERSENT };
enum { WEAPENSOULCAST_GROW=0, 
	   WEAPENSOULCAST_ADDTIONOPEN,
	   WEAPENSOULCAST_ADDTIONLEAN,
	   WEAPENSOULCAST_ADDTIONLEV,
	   WEAPENSOULCAST_MIXLEV,
	   WEAPENSOULCAST_SKILLLEAN,
	   WEAPENSOULCAST_SKILLRAND,
	   WEAPENSOULCAST_SKILLLEV,
	   WEAPENSOULCAST_TYPE,
};


/*
//地推，新手卡，新手答题都整和在一起用TYPE来分。1：地推，2：新手卡，3：新手答题 所以现在只用知道领礼物的次数，不需要知道种类
const DWORD	PRESENT_ONE				= 0x01;				//送第1次的装备
const DWORD	PRESENT_TWO				= 0x02;				//送第2次的装备
const DWORD	PRESENT_THREE			= 0x04;				//送第3次的装备
const DWORD	PRESENT_FOUR			= 0x08;				//送第4次的装备
const DWORD	PRESENT_FIVE			= 0x10;				//送第5次的装备
const DWORD	PRESENT_SIX				= 0x20;				//送第6次的装备
const DWORD	PRESENT_SEVEN			= 0x40;				//送第7次的装备
const DWORD	PRESENT_EIGHT			= 0x80;				//送第8次的装备
*///-pengfeng

enum Enum_ChangeMapGroupMode
	{
		ENUM_CNGMAPGROUP_MODE_REQ	=0,//转换地图组请求
		ENUM_CNGMAPGROUP_MODE_PROC	=1,//转换地图组响应后处理
		ENUM_CNGMAPGROUP_MODE_ACK	=2,//转换地图组同意
		ENUM_CNGMAPGROUP_MODE_NACK	=3,//转换地图组拒绝
	};

////////////////////////////////////////////////////////////////
// sex define
enum {SEX_NONE = 0, SEX_MAN, SEX_WOMAN, SEX_ASEXUAL, SEX_ALL};

enum
{
	SEX_MALE,SEX_FEMALE
};
// requre sex define
const unsigned char SEX_REQ_MAN			=	0x01;// 男的可用
const unsigned char SEX_REQ_WOMAN		=	0x02;// 女的可用
const unsigned char SEX_REQ_EUNUCH		=	0x04;// 太监可用


//////////////////////////////////////////////////////////////////////////
//	20070703 负经验使用
const __int64	USER_NEGATIVE_EXP_MIN_VALUE	=	-100000000000;	//	负经验最小值	//	[3/20/2008 朱斌]	负经验-1000亿
const int	USER_NEGATIVE_EXP_MAX_PERCENT =	-200;			//	负经验最大百分比
//////////////////////////////////////////////////////////////////////////

const int FISHTIRED_MAXVALUE = 500;                        //最大钓鱼疲劳度

const int  AWARDEX_TYPE_BEGIN    = 100; //领奖扩展起始类型
const int  AWARDEX_TYPE_END      = 200; //领奖扩展结束类型

////////////////////////////////////////////////////////////////
//const int	MASTER_USERLEV				= 120;
//const int	MAX_USERLEV					= 100;
const int	MAX_USERLEV					= 120;
//const int	MAX_EUDEMONLEV				= 130;

////////////////////////////////////////////////////////////////
enum 
{	
		RACE_NONE	= 0, // 未知
		RACE_HUMAN	= 1, // 人类
		RACE_SPRITE = 2, // 精灵
		RACE_DARKSPRITE = 3, // 暗精灵
};

////////////////////////////////////////////////////////////////
enum {	PROFSORT_MAGICIAN	=1, 
		PROFSORT_SOLDIER	=2, 
		PROFSORT_ARCHER		=3, 

		// no use
		PROFSORT_WIZARD		=4, 
		PROFSORT_DAOIST		=10, 
		PROFSORT_DAOISTGOLD	=11, 
		PROFSORT_DAOISTWOOD	=12, 
		PROFSORT_DAOISTWATER=13, 
		PROFSORT_DAOISTFIRE	=14, 
		PROFSORT_DAOISTEARTH=15, 
		};

////////////////////////////////////////////////////////////////
enum {
	NOBILITY_RANK_NONE		= 0,			// 无爵位
	NOBILITY_RANK_BARON		= 1,			// 爵士
	NOBILITY_RANK_VISCOUNT	= 2,			// 子爵
	NOBILITY_RANK_EARL		= 3,			// 伯爵
	NOBILITY_RANK_MARQUESS	= 4,			// 侯爵
	NOBILITY_RANK_DUKE		= 5,			// 公爵
};


enum {
	GENADD_SORT_PHYATT		= 1,//外功
	GENADD_SORT_MAGATT		= 2,//内功
	GENADD_SORT_PHYDEF		= 3,//外防
	GENADD_SORT_MAGDEF		= 4,//内防
	GENADD_SORT_HIT			= 5,//命中
	GENADD_SORT_DODGE		= 6,//闪躲
	GENADD_SORT_CRIT		= 7,//暴击
	GENADD_SORT_TOUGH		= 8,//坚韧
	GENADD_SORT_STR			= 9,//力量
	GENADD_SORT_INT			= 10,//灵力
	GENADD_SORT_STA			= 11,//体力
	GENADD_SORT_SPI			= 12,//定力
	GENADD_SORT_AGI			= 13,//身法
	GENADD_SORT_ALL			= 14,
	GENADD_SORT_HP			= 15,//HP
	GENADD_SORT_MP			= 16,//MP

	GENADD_SORT_ATT2		= 20,//冰
	GENADD_SORT_ATT1		= 21,//火
	GENADD_SORT_ATT3		= 22,//玄
	GENADD_SORT_ATT4		= 23,//毒
	GENADD_SORT_DEF2		= 24,
	GENADD_SORT_DEF1		= 25,
	GENADD_SORT_DEF3		= 26,
	GENADD_SORT_DEF4		= 27,

	GENADD_SORT_SPEED		= 28,//移动速度
};

////////////////////////////////////////////////////////////////
class IUserStorage
{
public:
	virtual bool	SendStorageInfo(OBJID idUser,int nIndex, int nPosition = ITEMPOSITION_STORAGE)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	CheckIn(OBJID idNpc, CItem* pItem, bool bUpdate = false, int nPosition = ITEMPOSITION_STORAGE)	PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	CheckOut(OBJID idNpc, OBJID idItem,  int nIndex, bool bUpdate = false, int nPosition = ITEMPOSITION_STORAGE)	PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetStorageCount()																		PURE_VIRTUAL_FUNCTION_0
	virtual OBJID	GetItemType(OBJID idItem)																PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetAmountLimit(OBJID idItem)															PURE_VIRTUAL_FUNCTION_0
	virtual bool    CombineItemInStorage(OBJID idNpc,CItem *pItem,bool bUpdate = true,int nPosition = ITEMPOSITION_STORAGE) PURE_VIRTUAL_FUNCTION_0
	virtual CItem*  GetItemByIdItem(OBJID idItem)															PURE_VIRTUAL_FUNCTION_0
	virtual CItem*  GetItemByIndex(int nIndex)																PURE_VIRTUAL_FUNCTION_0
	virtual bool	CombineItem(CUser* pUser, CItem* pItem1, CItem* pItem2)									PURE_VIRTUAL_FUNCTION_0
	virtual bool	ExChangeIndex(CUser* pUser, CItem* pItem1, CItem* pItem2)								PURE_VIRTUAL_FUNCTION_0
	virtual bool	MoveStorageItem(CUser* pUser, CItem* pItem, int nIndex)									PURE_VIRTUAL_FUNCTION_0
	virtual bool	DelItem(CUser* pUser, CItem* pItem)														PURE_VIRTUAL_FUNCTION_0
};

const int	MASTER_WEAPONSKILLLEVEL	= 12;
const int	MAX_WEAPONSKILLLEVEL	= 20;
const int	nWeaponSkillLeveUpNeedExp[20] = {
				0,
				1200,
				68000,
				250000,
				640000,
				1600000,
				4000000,
				10000000,
				22000000,
				40000000,
				90000000,
				190000000/2,
				285000000/2,
				427500000/2,
				641250000/2,
				961875000/2,
				1442812500/2,
				2164218750/2,
				3246328125/2,
				4200000000/2
};

//////////////////////////////////////////////////////////////////////
enum INFO_ID{
		INFO_NONE=0,
		INFO_ITEM,
		INFO_FRIEND,
		INFO_MAGIC,
		INFO_SYNATTR,
		INFO_STATUS,
		INFO_TEAM,
		INFO_COUNTTIMER,		//06.11.8王玉波添加
		INFO_AWARDINFOSEND,
		INFO_STATUS_MONSTER,//宠物身上的buff
		INFO_BEFRIEND,
		INFO_TREASURE,
		INFO_TUTOR,//师父
		INFO_STUDENT,//徒弟
		INFO_TITLE,//称号
		INFO_PETEGG,//宠物蛋
		INFO_JINMEI,
		INFO_DOUBLE,//冻结的双倍时间
		INFO_ONLINEGIFT,//许愿祝福
		INFO_KEY,
		INFO_AUTOFIGHT,
		INFO_PEIFANG,
		INFO_TASKDETAIL,
		INFO_EUDEMON,
		INFO_WARGAME,
		INFO_SPECILLINE,
//modify code by 林德渊 begin 2011.5.30-
		INFO_VASLV,
//modify code by 林德渊 end
		INFO_VERIFICATION,
		INFO_END,
};

////////////////////////////////////////////////////////////////
enum REMOTE_CALL{
		REMOTE_CALL_DROP_LEADER,
		REMOTE_CALL_LEAVE_SYN,
		REMOTE_CALL_TEAM_CLOSE_MONEY,
		REMOTE_CALL_TEAM_CLOSE_ITEM,
		REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS,
		REMOTE_CALL_TEAM_HUNTKILL,
		REMOTE_CALL_SYNPAYOFF,//20070414修罗:军团发工资到个人
		REMOTE_CALL_GOMATE,///瞬称到配偶请求
		REMOTE_CALL_REPLYGOMATE,///回应瞬称到配偶请求	
		REMOTE_CALL_KICKOUTPLAYER,//踢掉玩家
		REMOTE_CALL_WARGAME,		//	战争活动使用
		REMOTE_CALL_SYNADDMENBER,
		REMOTE_CALL_SYNADDHAVE,
		REMOTE_CALL_SYNADDOK,
};

enum ADJUSTDAMAGE
{
	ADJUSTDAMAGE_PHYDEF,///物防
};

enum ADJUSTUSERATTRIB
{
	ADJUSTUSERATTRIB_PHYDEF,///物防
	ADJUSTUSERATTRIB_MAXMANA,///最大法力值
	ADJUSTUSERATTRIB_MAXLIFE,///最大生命值
	ADJUSTUSERATTRIB_MAGDEF,///魔防
	ADJUSTUSERATTRIB_HITRANGE,///命中率
	ADJUSTUSERATTRIB_DOG,///闪避	
	ADJUSTUSERATTRIB_PHYACK,///物攻
	ADJUSTUSERATTRIB_MAGACK,///魔攻
	ADJUSTUSERATTRIB_Crit,///暴击
	ADJUSTUSERATTRIB_Drug,///吃药增加效果
	ADJUSTUSERATTRIB_STR,//力量
	ADJUSTUSERATTRIB_INT,//智力
	ADJUSTUSERATTRIB_STA,//体质
	ADJUSTUSERATTRIB_SPI,//精神
	ADJUSTUSERATTRIB_AGI,//敏捷
	ADJUSTUSERATTRIB_TOUGH,//坚韧

	ADJUSTUSERATTRIB_FIREATK,//火
	ADJUSTUSERATTRIB_FIREDEF,//火
	ADJUSTUSERATTRIB_ICEATK,//冰
	ADJUSTUSERATTRIB_ICEDEF,//冰
	ADJUSTUSERATTRIB_MYSTICATK,//玄
	ADJUSTUSERATTRIB_MYSTICDEF,//玄
	ADJUSTUSERATTRIB_NOXATK,//毒
	ADJUSTUSERATTRIB_NOXDEF,//毒
};

//enum BATTLEADDINFO //战斗系统附加信息
//{
namespace BATTLEADDINFO
{
const	DWORD INFO_INVALID			=	0X00000000;		//无效的，有些不参与战斗系统的计算的
const	DWORD INFO_BEGIN			=	0X00000001;		//开始,不使用此枚举
	
const	DWORD INFO_DODGE			=	0X00000001;		//闪避
const	DWORD INFO_CRITICALHIT		=	0X00000002;		//致命一击
const	DWORD INFO_LOWDAMAGE		=	0X00000004;		//轻伤 //改成吸收
const	DWORD INFO_HALTDAMAGE		=	0X00000008;		//中伤 //改成吸血
const	DWORD INFO_HIGHDAMAGE		=	0X00000010;		//重伤
const	DWORD INFO_BLOCK			=	0X00000020;		//格挡
const	DWORD INFO_IMMUNITY			=	0X00000040;		//免疫
const	DWORD INFO_RESIST			=	0X00000080;		//抵抗
const	DWORD INFO_ATK_INVALID		=	0X00000100;		//无效的攻击，用于类似攻击无效的目标，如守卫，会有特效
const	DWORD INFO_DODGE_WITHSHILD	=	0X00000200;		//闪避带护盾

const	DWORD INFO_END				=	0X80000000;		//结束,不使用此枚举
};

//};

//06.11.8王玉波添加--------------begin-------------------------------------------
struct CountTimerStruct{
	OBJID	idUser;				//玩家id
	int		nIndexInSet;		//在原来集合中的索引
	clock_t tUpdateTime;		//更新时间(上次开始计器时间)
	int		nIntervalTime;		//间隔时间
};

const _int64    _AWARD_VASPACK					=0x1;			///奖过VAS包了

typedef struct tag_GoMate_Info 
{
	int idMap;
	int posX;
	int posY;
}GoMate_Info;///配偶移动信息

//20061229修罗:宝石附加结构-------------------------------------
struct oftenUseGemAddition{
	int         sort;
	LONG		value;//goto 改成有符号类型,防止越界
	DWORD 		possibility;
};

enum UPDATECALTYPE
{
	UPDATECALTYPE_ALL,
	UPDATECALTYPE_EQUIP,
	UPDATECALTYPE_BUFF,
	UPDATECALTYPE_GIFT,
	UPDATECALTYPE_USER,
	UPDATECALTYPE_EUDSOUL,
};

enum
{
	SAVETYPE_OFFLINE,
	SAVETYPE_UPDATE,
};

typedef	map<unsigned short, oftenUseGemAddition*>		OFTENUSE_GEMADDITION;
//---------------------------------------------------------------
const DWORD		_MAX_CRIT						= 10000;		//20070130修罗:最大暴击.10000为100%
const float		_MAX_CRIT_F						= 100.0f;		//20070130修罗:最大暴击.10000为100%
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class	IChangeTeam
{
public:
	virtual bool AddTeamMember(OBJID idUser)				PURE_VIRTUAL_FUNCTION_0
	virtual bool DelTeamMember(OBJID idUser)				PURE_VIRTUAL_FUNCTION_0
	virtual bool DelTeam()									PURE_VIRTUAL_FUNCTION_0
	virtual bool SendInfo(SOCKET_ID idSocket, OBJID idAgent)				PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
// extern int RankOfIndex(int nPos);

////////////////////////////////////////////////////////////////

const	int		_MAX_VAS_ITEM_PRESENT_REGARD_LENGTH = 100;

const int       _MAX_SOUL_TYPE = 100000;            

typedef struct  
{
	int nType;	//	存放大类加小类207 sort = 2, type = 07
	int nPointA;
	int nPointB;
}ST_USER_LEARNED_GIFT_POINT_SUM;

//typedef vector<ST_USER_LEARNED_GIFT_POINT_SUM> SET_USER_GIFT_SUM;
typedef map<int, ST_USER_LEARNED_GIFT_POINT_SUM>	SET_USER_GIFT_SUM;

typedef vector<int>	SET_INT;

struct STINFO
{
	int nItemID;
	int nAmount;
};

enum CHECKDATA
{
	CHECKDATA_WALK,
	CHECKDATA_ATTACK,
	CHECKDATA_MAGICATT,
	CHECKDATA_WORLDTALK,
	CHECKDATA_SYNTALK,
	CHECKDATA_TEAMTALK,
	CHECKDATA_MAPTALK,
	CHECKDATA_LAST,
};

//------------------------------------------------------------------------------
class CNetMsg;
class CPackage;
class CTransformation;
class CTaskDetail;
class CUserPet;
class CUserPeifang;

class CUser : public CGameObj, public CRole, IUserStorage, IChangeTeam
{
protected:
	CUser();
	virtual ~CUser();
public:
	static		CUser*		CreateNew	()				{ return new CUser; }
	void		ReleaseByOwner			()				{ delete this; }
	void		OnTimer					(time_t tCurr);
	static		CUser*	CreateNewUser	(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo, ULONG nIP);
	bool		Create				(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idUser, ULONG nIP);
	bool		Init					();		// login 和 change map_group都要执行的操作
	bool		InitSpeInfoOnly			();		//只加载专线需要的信息
	bool		InitChangeMapGroup		();
	bool		InitEudemon				(IDatabase* pDb,bool bLogin = true);
	CStatusManager& GetStatusManager(){return m_statusmanager;}
	virtual		bool		FindAroundObj	(OBJID idObjType, OBJID idObj, void** pp)		{ return GetMap()->QueryObj(GetPosX(), GetPosY(), idObjType, idObj, pp); }
	virtual		IRole*		FindAroundRole	(OBJID id)		{ return GetMap()->QueryRole(GetPosX(), GetPosY(), id); }
	virtual		CUser*		FindAroundUser	(OBJID id);
	CUser*		FindUserInMap(OBJID id);
protected: // interface
	virtual		bool		QueryRole		(void** ppv)	{ return *ppv=(IRole*)this,true; }
	virtual		bool		QueryObj		(OBJID idObjType, void** ppv)			{ if(idObjType == OBJ_USER) return *ppv=this,true; return *ppv=NULL,false; }
	virtual		CUser*		QueryOwnerUser	()				{ return this; }

public:
	virtual		IMapThing*	QueryMapThing	()				{ return (IMapThing*)this; }
	IRole*					QueryRole		()				{ return (IRole*)this; }
	IChangeTeam*			QueryChangeTeam	()				{ return (IChangeTeam*)this; }
public:
	static BOOL IsFluctuate( UINT itemTypeID );// 判断装备生成时是否可以浮动
public:
	DWORD   GetPacklev(int nType = 0);
	void	SetPacklev( DWORD nLev )           { m_data.SetPackLev( nLev );}
	bool	SetPacklev( int nType,DWORD nLev);
	DWORD   GetBanklev(int nType = 0);
	bool	SetBanklev(int nType,DWORD nLev );
	void	UpdateMissionMsgByMonsterType(OBJID idType);
	void	UpdateMissionMsgByItemType(OBJID idType);
	int		sendMissionMsg();// 发送玩家任务信息
	void	SaveMissionInfo();
	//许愿祝福相关
public:
    void InitOnlineGift(OBJID Gift1ID,OBJID Gift2ID,OBJID Gift3ID,OBJID Gift4ID,OBJID Gift5ID);
	void ProcessOnlineGift();
	bool GiveOnlineGift();
	bool ChkItem(int nMustGive);
	bool ChkZero(int nIn);
	bool IsGive(int nTask);
	bool IsDayContune();

///副本相关接口
public:
	bool	RequestEnterInstance(UINT nInstanceType,UINT nDomainMapID=0,UINT nInstanceScope=0,UINT nCellx =0,UINT nCelly=0);///请求进入指定类型的副本
	BOOL	EnterInstance(OBJID idMapInstance,UINT nMapType,UINT nInstanceScope,UINT nDomainMapID,int x,int y);///进入指定类型的副本
	void	SendTeamMsg(int nChannel,const char* pMsg);
	BOOL	IsInTeam(){if(GetTeam()) return TRUE; else return FALSE;}
	void    AddTeamLeader(OBJID leaderId);
	DWORD	GetSynCon(){if(QuerySynAttr() && GetSynID()!=0) return QuerySynAttr()->GetProffer(); else return 0;}
	void	SetSynCon(int nData, bool bSend = true);
	int		GetMapDoc();
public:
	BOOL	ResetItemCD( UINT nDelayTimeMS, UINT TypeID, CTimeOutMS* pTimeOutMS  );//-------彭峰---2007/05/30-----
	void	UpdateAllOnlineTime();//-更新在线总时长
	void	UpdateLastLogout();
	OBJID	GetOnlineTime();//-得到本次在线时长//单位分钟
	BOOL	ReformPoint(int nType = 0,int data = 0);
	float	CalItemAddExpTeamFactor();
	BOOL	IsPickUpKidnap( int nItemType );
	BOOL	EquipMakeHole( OBJID equipID, OBJID stoneID);// 装备打洞 [2010-07-20 goto]
	BOOL	EquipMakeChaosHole(OBJID idEquip, OBJID idStone);//打特殊孔－－混沌之孔
public:
	virtual		int			GetObjType		()			{ return CGameObj::GetObjType(); }
	virtual		void		SendShow		(IRole* pRole);

public:
	virtual bool IgnoreTargetDodge()	{ return m_bIgnoreTargetDodge;	}
	virtual bool IsMustCritHit()		{ return m_bMustCritHit;			}

	void	SetDetectionFlag( OBJID targetID )			{ m_bIgnoreTargetDodge = m_bMustCritHit = true;m_targetID	= targetID;	}
	void	ResetDetectionFlag()						{ m_bIgnoreTargetDodge = m_bMustCritHit = false;m_targetID	= NULL;		}
protected:
		bool	m_bIgnoreTargetDodge;	//忽略对方躲闪
		bool	m_bMustCritHit;			//必然命中
public:
		OBJID	m_targetID;
//锁定目标状态相关
public:
	void setLockTarget( OBJID targetID) { m_IdLockTarget=targetID;}
	OBJID getLockTarget( ) { return m_IdLockTarget;}
private:
	OBJID	m_IdLockTarget;
	OBJID	m_idLastTaskEudemon;
public:
	void SetLastTaskEudemon(OBJID idEudemon) { m_idLastTaskEudemon = idEudemon; }
	OBJID GetLastTaskEudemon() { return m_idLastTaskEudemon; }
	bool IsRedProcAble(){return (m_data.GetPkProc() == 0);} //是否为白名保护
	bool IsWhileProcAble(){return (m_data.GetPkProc() == 0 || m_data.GetPkProc() == 1) ;} //是否为红白名保护
	bool IsSynProcAble(){return (m_data.GetPkProc() == 3);}//是否为帮派保护
	void SetPkProc(int nStatus,BOOL bUpdate = false)  
	{ 
		m_data.SetPkProc(nStatus,bUpdate);
	} //设置Pk保护,同时设置红名和白名
public: // IRole
	virtual OBJID	GetID			()				{ return m_data.m_Info.id; }
	virtual LPCTSTR	GetName			()				{ return m_data.m_Info.szName; }
	virtual LPCTSTR	GetMate			()				{ return "无"; }//婚姻系统未开发，暂时写无
	virtual LPCTSTR	GetNameByID		(OBJID idUser);
	virtual LPCTSTR	GetDepotCode	()				{ return m_data.m_Info.depotCode; }

	virtual DWORD	GetLookFace		()				{ return m_data.GetLookFace();}

	void		SetPosX			(int x)				{m_nPosX = x;}
	void		SetPosY			(int y)				{m_nPosX = y;}
	virtual int		GetPosX			()				
	{
		return m_nPosX;
	}
	virtual int		GetPosY			()				
	{
		return m_nPosY;
	}
	virtual int 	GetDir			();
	virtual void	SetDir			(int nDir)		{ m_nDir = nDir;/*m_nDir = nDir%8;*/ }
	virtual DWORD	GetMoney		()				{ return GetMoneyByType() + GetMoneyByType(2); }
	virtual DWORD	GetMoneySaved	()				{ return m_data.GetMoneySaved(); }


	DWORD	GetMoneyByType		(int nType =1)		{ return m_data.GetMoney(nType);}
	void	SetMoneyByType		(DWORD nMoney,int nType =1,BOOL bUpdate = false)		{  m_data.SetMoney(nMoney,nType,bUpdate);}
	void	SetFatigue			(int iFatigue, bool bUpdate = false)		{/*m_pUserKey.SetFatigue(iFatigue, bUpdate);*/m_data.SetFati(iFatigue);}
	void	SetFatigueInKey		(int iFatigue, bool bUpdate = false)				{m_pUserKey.SetFatigue(iFatigue, bUpdate);}	
	void	setFatiTime			(int iFatiTime)		{m_data.SetFatiTime(iFatiTime);}
	int		GetFatigue			()					{return m_data.getFati();}
	int		GetTreasureCost()	{ return m_pUserKey.GetTreasureCost(); }//记录玩家开箱子开了多少元宝仍然没有出麒麟之魂
	void	SetTreasureCost(int nCost, bool bUpdate=false)	{ m_pUserKey.SetTreasureCost(nCost, bUpdate); }//记录玩家开箱子开了多少元宝仍然没有出麒麟之魂

	int		GetVasTicketAmount()	{ return m_pUserKey.GetVasTicketAmount(); }
	void	SetVasTicketAmount(int nAmount, bool bUpdate=false)	{ m_pUserKey.SetVasTicketAmount(nAmount, bUpdate); }
	DWORD			GetWarSroce() {return m_dwWarSroce;}
	//一级属性
	//五项基本点数
	//考虑加成.
	DWORD			GetStr() {return m_dwUserStr;}//力量
	DWORD			GetIntex() {return m_dwUserInt;}//灵力
	DWORD			GetSta() {return m_dwUserSta;}//体力
	DWORD			GetSpi() {return m_dwUserSpi;}//定力
	DWORD			GetAgi() {return m_dwUserAgi;}//身法
	

	//五项基本点数的仅玩家点数
	DWORD			GetStrUserOnly(int type =0){ return m_data.GetStr(type);}//力量
	DWORD			GetIntUserOnly(int type =0){ return m_data.GetIntex(type);}//灵力
	DWORD			GetStaUserOnly(int type =0){ return m_data.GetSta(type);}//体力
	DWORD			GetSpiUserOnly(int type =0){ return m_data.GetSpi(type);}//定力
	DWORD			GetAgiUserOnly(int type =0){ return m_data.GetAgi(type);}//身法

	//2级属性
	//仅为玩家的基础属性
	DWORD 	GetAtkUserOnly(int nType =0);
	DWORD 	GetMgcAtkUserOnly(int nType =0);
	DWORD	GetDefUserOnly(int nType =0);
	DWORD	GetMagicDefUserOnly(int nType =0);
	DWORD	GetDdgUserOnly(int nType =0);
	DWORD	GetCritUserOnly(int nType =0);
	DWORD	GetToughUserOnly(int nType =0);
	DWORD	GetAtkHitRateUserOnly(int nType =0);
	DWORD	GetMaxLifeUserOnly(int nType =0);
	DWORD	GetMaxManaUserOnly(int nType =0);
	//考虑加成.
	virtual DWORD	GetAtk();//{return m_dwUserphyatt;}
	virtual DWORD	GetMgcAtk();//{return m_dwUsermagatt;}
	virtual DWORD	GetDef();// {return m_dwUserphydef;}
	virtual DWORD	GetMagicDef();//{return m_dwUsermagdef;}
	virtual DWORD	GetAtkHitRate();//{return m_dwUserhit;}
	virtual DWORD	GetAddAttByType(int nType);
	virtual DWORD	GetAtkType();
	virtual DWORD	GetDdg();//{return m_dwUserdodge;}
	virtual DWORD	GetCrit();//{return m_dwUsercrihit;}
	virtual DWORD	GetTough();//{return m_dwUserTough;}
	virtual DWORD	GetMaxLife();//{return m_dwUserhp;}
	virtual DWORD	GetMaxMana();//{return m_dwUsermp;}
	//特殊
	DWORD	GetRandPhyAtk(DWORD nOldAtk);
	DWORD	GetRandMagAtk(DWORD nOldAtk);
	DWORD	GetRandAtt1(DWORD nOldAtk);
	DWORD	GetRandAtt2(DWORD nOldAtk);
	DWORD	GetRandAtt3(DWORD nOldAtk);
	DWORD	GetRandAtt4(DWORD nOldAtk);
	DWORD	GetDecPhyDef(DWORD nOldDef);
	DWORD	GetDecMagDef(DWORD nOldDef);
	DWORD	GetDecDef1(DWORD nOldDef);
	DWORD	GetDecDef2(DWORD nOldDef);
	DWORD	GetDecDef3(DWORD nOldDef);
	DWORD	GetDecDef4(DWORD nOldDef);
	//goto 对宠物生效的被动技能,如帮派技能等
	LONG	GetPetAtkAdd()	{ return m_GiftPetAtk; }
	LONG	GetPetMagAtkAdd()	{ return m_GiftPetMagAtk; }
	LONG	GetPetCrihitAdd()	{ return m_GiftPetCrihit; }
	LONG	GetPetMaxLifeAdd()	{ return m_GiftPetMaxLife; }
	//动态
	void	SetLife( DWORD nLife, bool bUpdate = false )	{ m_data.SetLife( nLife,bUpdate);	}
	void	SetMana( DWORD nMana, bool bUpdate = false )	{ m_data.SetMana( nMana,bUpdate);	}
	void	SetSp(DWORD nSp, bool bUpdate = false )	{ m_data.SetSp( nSp,bUpdate );	}
	virtual DWORD	GetLife()			{ return m_data.GetLife(); }
	virtual DWORD	GetMana()			{ return m_data.GetMana(); }
	virtual DWORD	GetSp()				{ return m_data.GetSp();}
	//调整
	virtual DWORD 	AdjustDamage		(DWORD nDamage,int nAttribType);
	virtual int 	AdjustUserAttrib	(DWORD nAttrib,int nAttribType);
	//考虑
	virtual int		GetAttackRange		(int nTargetSizeAdd);
	virtual DWORD	GetMaxSp			();
	virtual DWORD	GetInterAtkRate		();
	virtual DWORD	GetIntervalAtkRate	();
	virtual	int		GetSizeAdd			();

	//	virtual void	AwardBattleExp		(int nExp, bool bGemEffect=true, bool bIncludeOwner=true);
	virtual DWORD	AwardBattleExp		(__int64 i64Exp, bool bGemEffect=true, bool bIncludeOwner=true, bool bTask=false);
	virtual void	AwardUserExp		(UINT nExp){AwardBattleExp(nExp);}///专供脚本系统测试之用的接口.

	virtual int		GetDistance			(IMapThing* pTarget)	{ return __max(abs(GetPosX() - pTarget->GetPosX()), abs(GetPosY() - pTarget->GetPosY())); }
	virtual int		GetDistance			(int x, int y)			{ return __max(abs(GetPosX() - x), abs(GetPosY() - y)); }

//	virtual int 	GetExp				()						{ return m_data.GetExp(); }
	virtual __int64		GetExp				()						{ return m_data.GetExp(); }
//	virtual int     SetExp              ()                      {  m_data.SetExp(); }//  [陈剑飞 7/10/2007]
	virtual void	SetStatus			(int nStatus, bool bSynchro=true, int nPower=0);
	virtual void	ClsStatus			(int nStatus, bool bSynchro=true);
	virtual DWORD	GetStatus			()						{ return /*m_data.m_Info.dwStatus*/0; }
	virtual void	ClearAllStatus		();
	virtual void    ClearManyStatusWhenDie();//-------彭峰---2007/05/16-----死亡时清除一些状态


	virtual void	SetAllOnlineTime( DWORD nAllOnlineTime, BOOL bUpdate = false ){m_data.SetAllOnlineTime(nAllOnlineTime,bUpdate);}// 设置总在线时长 [9/12/2007 %PENGFENG%]
	virtual DWORD	GetAllOnlineTime ()				{ return m_data.m_Info.dwAllOnlineTime; }// 得到总在线时长 [9/12/2007 %PENGFENG%]

	virtual void	SetAllAddExpTime( DWORD nAllAddExpTime, BOOL bUpdate = false ){ m_data.SetAllAddExpTime(nAllAddExpTime,bUpdate);}// 设置累积经验值[9/12/2007 %PENGFENG%]
	virtual DWORD	GetAllAddExpTime ()				{ return m_data.m_Info.dwAllDoubleTime; }// 得到累积经验值 [9/12/2007 %PENGFENG%]

	virtual DWORD	GetExitTime()	{ return m_data.m_Info.dwExitTime; }			// 得到开始下线累积经验时间 [9/12/2007 %PENGFENG%]
	virtual void	SetExitTime( DWORD exitTime,BOOL bUpdate = false ){m_data.SetExitTime(exitTime,bUpdate);}	// 设置开始下线累积经验时间 [9/12/2007 %PENGFENG%]			//很少使用，直接修改DB

//	virtual BOOL	CheckStatus			(int nStatus);
	virtual I64 	GetEffect			();
	I64				GetSecondEffect		()	{ return /*m_data.m_Info.i64Effect2*/0;}//07.4.17王玉波添加


	virtual DWORD	GetLev				()						{ return m_data.GetLev();		}
	virtual void	IncLev				(int nLev,int nType = 0); //0人物升级 1主职业升级 2副职业升级
	virtual void	AllotPoint			(int nData , int nType);
	void AllotEudemonPoint(OBJID idEudemon ,int nType , int nData );
	bool CheckLevUp(int nCurLev);
	virtual void	ClrAttackTarget		()						{ SetAttackTarget(NULL); QueryMagic()->BreakAutoAttack(); }
	virtual bool	SetAttackTarget		(IRole* pTarget);
	virtual OBJID	GetAttackTargetID	()						{ if (m_pBattleSystem) return m_pBattleSystem->GetTargetID(); return ID_NONE; }
	virtual bool	AutoSkillAttack		(IRole* pTarget)		{ return QueryMagic()->AutoAttackTarget(pTarget); }
	virtual int		Attack				( IRole* pTarget,DWORD& dwBattleAddInfoRef );
	virtual bool	BeAttack			(bool bMagic, IRole* pTarget, int nLifeLost,int addi = 0 , bool bReflectEnable=true);
	virtual bool	MagicAttack			(int nType, OBJID idTarget, int x, int y,bool needLearn = true,int learnLev = 0);
	virtual void	Kill				(IRole* pTarget, DWORD dwDieWay);
	virtual void	BeKill				(IRole* pRole = NULL);
	void	BeKillBySys();
	void	BeKillByDot(OBJID idAtker);
	virtual bool	BroadcastTeamLife	(bool bMaxLife=true);
	virtual bool	BroadcastTeamMana	(bool bMaxMana=true);
	virtual bool	BroadcastTeamLev	(bool bLev=true);
	virtual bool	IsAtkable			(IRole* pTarget, bool bSendHint=false);
	virtual bool	IsBeAtkable			();
	virtual void	SendDamageMsg		( OBJID idTarget, int nDamage,DWORD dwBattleAddInfo );
	virtual void	ProcessPk			(CUser* pTargetUser);
	virtual int		GetHelmetTypeID		()			{ if(m_pHelmet) return m_pHelmet->GetInt(ITEMDATA_TYPE); return ID_NONE; }
	virtual int		GetArmorTypeID		();//06.5.5王玉波修改
	virtual int		GetWeaponRTypeID	()			{ if(m_pWeaponR) return m_pWeaponR->GetInt(ITEMDATA_TYPE); return ID_NONE; }
	virtual int     GetDressTypeID		()			{ if(m_pDress) return m_pDress->GetInt(ITEMDATA_TYPE); return ID_NONE; }
	virtual int		GetNecklaceTypeID	()			{ if(m_pNecklace) return m_pNecklace->GetInt(ITEMDATA_TYPE); return ID_NONE; }
	virtual int		GetShoesTypeID		()			{ if(m_pShoes) return m_pShoes->GetInt(ITEMDATA_TYPE); return ID_NONE; }
	virtual int		GetMountTypeID		()			{ if(m_pMount) return m_pMount->GetInt(ITEMDATA_TYPE); return ID_NONE; }// 坐骑 [陈剑飞7/30/2007]
	virtual int		GetWeaponSpecialView()			{ if(m_pWeaponR) return m_pWeaponR->GetWeaponSpecialView(); return 0; }//goto 武器特效

	virtual CItem*	GetWeaponR			()			{ return m_pWeaponR; }

	virtual bool	IsSimpleMagicAtk	();
	virtual int 	GetSimpleMagicHitRate();

	virtual DWORD	GetTutorLevel		()			{ return m_data.GetTutorLevel(); }
	int		GetSex()								{ return m_data.GetSex(); }	

    virtual DWORD   GetAllTutorLevel    ();

	virtual bool	IsAlive				();
	virtual bool	IsTalkEnable		();
	virtual bool	IsGM				()			{ return (strstr(GetName(), "[GM]") || strstr(GetName(), "[PM]")); }
	virtual bool	IsPM				()			{ return (strstr(GetName(), "[PM]") != NULL); }
	virtual bool	IsMonster			()			{ return false; }
	virtual BOOL	IsUser				()			{ return true; }
	virtual bool	IsDelThis			()			{ return false; }
	virtual bool	IsEvil()						{ return IsRedName()||IsGrayName(); }
	//白名
	inline BOOL	IsWhiteName();
	//灰名
	BOOL	IsGrayName();
	//红名
	BOOL	IsRedName();

	int GetTaskStep(OBJID idTask);

	BOOL		GetStorageLimit();///是否通过增值消费奖过背包
	void		SetStorageLimit(int nStoragelimit);///设置增值奖包标识
	virtual bool	SendMsg				(CNetMsg* pMsg);
	void    SendTeamMemberInfo();
	virtual void	BroadcastRoomMsg(CNetMsg* pMsg, 
		bool bSendSelf = false,
		EM_SENDNPCFLAG sendNpc = EM_SENDNPCFLAG_TRUE);
	virtual void	BroadcastRoomMsg	(LPCTSTR szMsg, bool bSendSelf);
	virtual void	BroadcastMapMsg		(CNetMsg* pMsg, bool bSendSelf);
	virtual void	BroadcastMapMsg		(LPCTSTR szMsg, bool bSendSelf);
	virtual bool	SendSysMsg			(const char* fmt, ...);
	virtual bool	SendSysMsg	(int nChannel, const char* fmt, ...);
	virtual bool	SendInformation(const char* strInfo);///softworms扩展-2007-12-7
	virtual bool	SendSysInfo(const char* strInfo);///softworms扩展-2007-12-7
	virtual DWORD	GetSynID			()				{ if (QuerySynAttr()) return QuerySynAttr()->GetSynID(); return 0;}
 	virtual DWORD	GetMasterSynID		()				{ if(QuerySynAttr()->GetSynID()==ID_NONE) return ID_NONE; return this->GetSyndicate()->GetMasterSynID(); }
public:
	void ClearCheckData()											{ m_nLastWalkTime[0] = 400;m_nLastWalkTime[1] = 400;m_nThisWriteTime = 0;for(int i = 0 ; i < CHECKDATA_LAST ; i ++ ) m_dwCheckData[i] = 0;}
	void ClearWalkData()							{ m_dwCheckData[CHECKDATA_WALK] = 0;m_nLastWalkTime[0] = 400;m_nLastWalkTime[1] = 400;m_nThisWriteTime = 0;}	//防止切地图打回和打回后再打回
	bool CheckMsgData(CHECKDATA nData,DWORD dwMs)					
	{ 
		if(nData == CHECKDATA_WALK) //走路用3次平均值
		{
			DWORD nThisCheck = (m_nLastWalkTime[0] + m_nLastWalkTime[1] + clock() - m_dwCheckData[nData]) /3;
			if(nThisCheck > 450)
				nThisCheck = 450;
			if(nThisCheck > dwMs)
			{
				m_dwCheckData[nData] = clock() ;
				m_nLastWalkTime[m_nThisWriteTime] = nThisCheck;
				m_nThisWriteTime ++;
				m_nThisWriteTime %= 2;
				return true;
			}
		}
		else
		{
			if(clock() - m_dwCheckData[nData] > dwMs)
			{
				m_dwCheckData[nData] = clock() ;
				return true;
			} 
		}
		m_dwCheckData[nData] = clock(); 
		return false;
	}
protected:
	DWORD m_dwCheckData[CHECKDATA_LAST];
	USHORT m_nLastWalkTime[2];
	USHORT m_nThisWriteTime;
public:
	DWORD GetPasswayAction()				{ return m_dwPasswayAction;    }
	void  SetPasswayAction(DWORD idAction)  { m_dwPasswayAction = idAction;}
private:
	DWORD m_dwPasswayAction;
public:	// mining -------------------
	bool		Mine	();
	void		StopMine();
	void		ProcessMineTimer(void);
private:
	int			m_nMineType;
	CTimeOutMS	m_tMine;

	CStatusManager m_statusmanager;
	SYSTEMTIME m_tLastResetVasExchangeTime;
	bool m_bResetDarTimeSet;
public:
	CStatusManager& QueryStatusManager()	{  return m_statusmanager;	}
	
public:
	void		WeaponLevUp( OBJID idWeapon,short nGemCount,bool bUseLuckyCharm=false );
	void		EquipAddStar( OBJID idEquip,short nGemCount,bool bUseLuckyCharm=false );//[2010-07-20 goto]装备升星
	//镶入宝石
	bool		EmbedGem				( OBJID idEquip, OBJID idStone , OBJID idGem );
	bool		EmbedGemInChaos			( OBJID idEquip, OBJID idStone , OBJID idGem );
	//取出宝石
	bool		TakeOutGem				(OBJID idEquip,OBJID stoneID,int nPos );
	//[2010-08-04 goto]宝石合成
	bool		GemLevUp				(OBJID idCharm, set<OBJID> pDataSet,int nSize);
	bool		EquipSoulMark			(OBJID idEquip);//goto 魂印
	OBJID		GemCutwork				(OBJID idGem, OBJID idCharm);//宝石雕琢
//modify code by 林德渊 begin 2011.7.13
	bool		EquipArtific			(OBJID idEquip);//炼化
	bool		EquipMove				(OBJID idEquip,OBJID idCharm,int &firstaddlv,int &secondaddlv);//强化转移
//modify code by 林德渊 end
//////////////////////////////////////////////////////////////////////////
	//铸灵相关
	bool	EquipSoulAdd(OBJID idWeapon, OBJID nCount,bool bUseBook=false);
	bool    EquipSoulExchange(OBJID idWeapon, OBJID idExWeapon);
	int		GetEquipSoulDeadLine(int nType,int nLev);//1是所需魔灵数,2是冰火玄毒属性,3是力灵体定身属性
	int		GetEquipSoulLevByPoint(int nPoints,int nLev);//会计算剩余魔灵数
	bool	DelItemMulti(OBJID itemType,bool &bBind, int nCostNum,bool bKindnapFirst=true);
public: // move
	void		SetSynchro				(bool bSynchro)		{ if(bSynchro) m_tSynchro.Clear(); else m_tSynchro.Startup(SYNCHRO_SECS); }
	bool		IsSynchro				()					{ return !(m_tSynchro.IsActive() && !m_tSynchro.TimeOver()); }
	bool		SynPosition			(int nPosX, int nPosY, int nMaxDislocation = 8);
	void		ProcessOnAttack		();
	void		ProcessOnMove		(int nMoveMode);
	void		ProcessAfterMove	();
	void		GetFootPrint	(int& nPosX, int& nPosY);
	bool		KickBack();
	bool		Synchro();
protected:
	CTimeOut	m_tSynchro;			// invalid: wait for client actionSynchro.
	CTimeOut	m_tLogout;//20070601修罗:退出计时
	DWORD		m_tFootPrint;
	POINT		m_posFootPrint;
	auto_long	m_bRun;
	CTimeOutMS	m_tStand;
	auto_long	m_bStand;
	OBJID		m_nConMoney;
	OBJID		m_nConBuild;

//复活术相关--------------begin----------------------------
public:
	void	SetWaitRebornData( int nRecoverPercent,int nWaitTime ){ 
							m_bRebornFlag = true;
							m_nRecoverPercent = nRecoverPercent;
							m_tWaitReborn.Startup( nWaitTime );}
	void	ProcessRebornMe();
	void	ClearUsingInstance();
protected:
	int			m_nRecoverPercent;
	bool		m_bRebornFlag;
	CTimeOut	m_tWaitReborn;		//某人对你使用复活术,30秒内可以选择是或否. 
//------------------------end----------------------------
public:	// map ----------------------
	OBJID		GetMapID						();
	BOOL		IsMapID							(DWORD mapid); 
	OBJID		GetRecordMapID					();
	OBJID		GetRecordPosX					();
	OBJID		GetRecordPosY					();
	int 		FlyMap							(OBJID idMap, int nPosX, int nPosY);		// return false: can't fly
	bool		CanNot							(void);
	void		GotoJail						(BOOL GotoJail_Self = false);
	void		GotoSelfJail					(void);
	virtual bool		SendSelfToBlock			();
	
public: // main -------------------------------------
	SOCKET_ID	GetSocketID			()						{ return m_idSocket; }
	PROCESS_ID	GetProcessID		()						{ return m_idProcess; }
	void		SaveInfo			(int nSaveType = SAVETYPE_OFFLINE);		// save all info

	void		RemoteCall			( REMOTE_CALL0 * pInfo);
	OBJID		GetAccountID		()						{ return m_data.m_Info.idAccount; }

	ULONG		GetIPAddress		()						{ return m_nIPAddress; }//获得玩家IP地址

public: // const -------------------------------------

public: // leave word ---------------------------------------
	bool		IsMoreLeaveWord()							{ return m_bHaveLeaveWord; }
	void		SetLeaveWord(bool nFlag)					{ m_bHaveLeaveWord = nFlag; }
	bool		AddLeaveWord(LPCTSTR szRecv, LPCTSTR szWords, LPCTSTR szColor="0");//[2010-06-10 goto]增加文本颜色字段
	int			GetSendLeaveWordCount()						{ return m_nSendLeaveWord; }
	void		SetSendLeaveWordCount(int nCount)			{ m_nSendLeaveWord = nCount; }
protected:
	bool		m_bHaveLeaveWord;
	int			m_nSendLeaveWord;
public: // common -------------------------------------
//	static int	AdjustData			(int nData, int nAdjust, int nMaxData=0);
	static int	GetLook(DWORD dwLookFace)					{ return dwLookFace%1000; }
	static int	GetFace(DWORD dwLookFace)					{ return (dwLookFace/10000)%1000; }
//	static int	GetSex(DWORD dwLookFace)					{ return (m_pInfo->dwLookFace/1000)%10; }
	static int	GetTransFormLook(DWORD dwLookFace)			{ return dwLookFace/10000000; }

public: // IUserStorage ---------------------------------------------------
	IUserStorage*	QueryStorage()							{ return (IUserStorage*)this; }
protected: 
	virtual bool	SendStorageInfo(OBJID idUser, int nIndex, int nPosition = ITEMPOSITION_STORAGE);
	virtual bool	CheckIn(OBJID idNpc, CItem* pItem, bool bUpdate = false, int nPosition = ITEMPOSITION_STORAGE);
	virtual CItem*	CheckOut(OBJID idNpc, OBJID idItem, int nIndex, bool bUpdate = false, int nPosition = ITEMPOSITION_STORAGE);
	virtual int 	GetStorageCount()						{ IF_OK(m_pStorage) return m_pStorage->GetAmount(); return 1234567890; }
	virtual OBJID	GetItemType(OBJID idItem)				{ IF_OK(m_pStorage) return m_pStorage->GetItemTypeByID(idItem); return ID_NONE; }
	virtual DWORD	GetAmountLimit(OBJID idItem)			{ IF_OK(m_pStorage) return m_pStorage->GetItemAmountLimitByID(idItem); return 0; }
	virtual bool    CombineItemInStorage(OBJID idNpc,CItem *pItem,bool bUpdate = true,int nPosition = ITEMPOSITION_STORAGE); //用于仓库物品的合并
	virtual CItem*  GetItemByIdItem(OBJID idItem)            {IF_OK(m_pStorage) return m_pStorage->GetItemByID(idItem);return ID_NONE;}
	virtual CItem*	GetItemByIndex(int nIndex)				{ IF_OK(m_pStorage) return m_pStorage->GetItemByIndex(nIndex);return ID_NONE;}
	virtual bool	CombineItem(CUser* pUser, CItem* pItem1, CItem* pItem2);
	virtual bool	ExChangeIndex(CUser* pUser, CItem* pItem1, CItem* pItem2);
	virtual bool	MoveStorageItem(CUser* pUser, CItem* pItem, int nIndex);
	virtual bool	DelItem(CUser* pUser, CItem* pItem);
protected:
	CPackage*		m_pStorage;

public:	// modify attrib ------------------------------
	int			GetAddPoint(int nType = 0)				 {	return m_data.GetPot(nType); }

	//checked ok
	bool		AddAttrib(int idxAttr, __int64 i64Data, int nSynchro);
	void		SetAttrib(int idxAttr, __int64 i64Data, int nSynchro);

	bool CanUseItem(CItem* pItem);///判断玩家是否能使用某个道具
protected:
	CTimeOut	m_tMount;

public:	// sprite -- add by zlong 2003-11-28
protected:
	CTimeOut			m_tSprite;

public: // cheat --------------------------------------------------------
	enum { _TYPE_MAGIC=0, _TYPE_WS=1, _TYPE_FY=2, _TYPE_USE_LIFE=3, _TYPE_USE_MANA=4, }; // 无双// 风云	// 用红药// 用蓝药
	bool		SetCheatLevel(int nLevel);
	void		KickoutCheat(OBJID idKind);
	int			GetCheatLevel()						{ return m_nCheatLevel; }
	bool		IsCheater(OBJID idKind);
	bool		IsMagicAtkCheat	(OBJID idTarget, int nTargetX, int nTargetY, DWORD dwTimeStamp);
	void		SetCheatMark(OBJID idCheatKind, DWORD dwCheatMark);
	void		DoCheaterPunish	(int nData = 0);

	DWORD		m_dwLastJump;
	DWORD		m_dwMsgCount;

	bool		CheckTimeStamp(DWORD dwTimeStamp)				{ if(dwTimeStamp-m_dwLastTimeStamp>50) { m_dwLastTimeStamp=dwTimeStamp; return true; } return false; }

protected:
	void		SetCheat(int nType, int nLevel);
protected:
	auto_long	m_nCheatLevel;
	auto_long	m_nCheatCount;
	auto_long	m_maskCheat;

	deque<DWORD>	m_setActionTime;

	auto_long	m_dwLastTimeStamp;

protected:
//	virtual		I64		SetEffect			(I64 i64Effect)	{ m_data.m_Info.i64Effect |= i64Effect; return m_data.m_Info.i64Effect; }
//	virtual		I64		ResetEffect			(I64 i64Effect=0xFFFFFFFFFFFFFFFF)	{ m_data.m_Info.i64Effect &= (~i64Effect); return m_data.m_Info.i64Effect; }

public:
	UserInfoStruct*	GetInfo	(void)		{return &m_data.m_Info;}
//	virtual CGameMap*		GetMap			()			{ CHECKF(m_pMap); return m_pMap; }


public: // apply ---------------------------------------------
	enum { 
		APPLY_NONE=0,
		APPLY_FRIEND, 
		APPLY_MARRY, 
		APPLY_TRADE, 
		APPLY_TEAMAPPLY, 
		APPLY_TEAMINVIT,
		APPLY_JOIN_SYN, 
		APPLY_INVITE_JOIN_SYN, 
		APPLY_ALLY_SYN, 

		APPLY_STUDENTAPPLY,		// 申请收徒弟
		APPLY_TEACHERAPPLY,		// 申请拜师
		APPLY_GRADUATE,			// 申请出师
// 		APPLY_TEAM_NO,			//拒绝组队
		APPLY_SYNWAR,
		APPLY_PVP,
		APPLY_CANCELDISSTUDENT,		// 申请取消开除徒弟
		APPLY_CANCELLEVTEACHER,		// 申请取消离开师傅
		APPLY_LEAVETEACHER,			// 申请解除师徒关系
	};

	void	SetApply		(int nApply, OBJID idTarget)	{ m_nApply = nApply; m_idApplyTarget = idTarget; }
	OBJID	FetchApply		(int nApply)					{ if(nApply == m_nApply) { m_nApply = APPLY_NONE; return m_idApplyTarget; } return ID_NONE; }
	//bool	FetchApply		(int nApply, OBJID idTarget)	{ if(nApply == m_nApply && idTarget == m_idApplyTarget) { m_nApply = APPLY_NONE; return true; } return false; }
	bool	FetchApply		(int nApply, OBJID idTarget);//放到cpp里方便调试
	//////////////////////////////////////////////////////////////////////////
	//  [2007-9-7] 朱斌 按名字来
	void	SetApplyName	(int nApply, const char* pszName);
	bool	FetchApplyName	(int nApply, const char* pszName);
	//////////////////////////////////////////////////////////////////////////
protected:
	int		m_nApply;
	OBJID	m_idApplyTarget;

	int		m_nApplyByName;
	NAMESTR	m_szApplyName;
public:	//-------- 不知道的类型 -----------------------
//	void		InitWalkStep		()				{ m_nWalkStep = 0; }

	void		SetForce			(DWORD dwForce);
	

	void		SetDexterity		(DWORD dwDex);

	void		SetSpeed			(DWORD dwSpeed);
	DWORD		GetSpeed			()				{ return (m_data.GetSpeed()>150)?150:m_data.GetSpeed();} //强制同步为小于=150，防止玩家被tickOUt
	// 根据状态调整速度
	DWORD		AdjustSpeed			(int nSpeed);

	void		SetHealth			(DWORD dwHealth);
	

	void		SetLoveliness		(DWORD dwSoul);
	

	void		SetSoul				(DWORD dwSoul);
	
	DWORD		GetSoulSum			();

	void		SetFace				(DWORD dwFace);
	DWORD		GetFace				()				{ return m_data.GetLookFace(); }
	void		SetLook				(DWORD dwLook);

	int			GetRace				()				{ return 0; }
	//[2010-06-04 goto] 用户心情，在好友中显示
	void		SetFeel				(char* szFeel);
	char*		GetFeel				()				{ return m_data.GetFeel(); }

	void		SetItemLockPwd		(char* szPwd)   { m_data.SetItemLockPwd(szPwd); }
	char*		GetItemLockPwd		()			 { return m_data.GetItemLockPwd(); }

	void		ProcAutoHealRecover	();		//自动回血,蓝..	

	int         GetJinmeiValue()     { return m_data.GetJinmeiValue(); }
	void        SetJinmeiValue(int nData) { m_data.SetJinmeiValue(nData, true); }
	bool        IncJinmeiValue(int nData);
	bool        SpendJinmeiValue(int nData);
	void        SendJinmeiSysLogInOKInfo();

	int         GetJinmeiIncDamageRate();
	int         GetJinmeiDecDamageRate();


	void		SetName	(const char* pszName)	{ m_data.SetName(pszName); }

	void		SetDepotCode(const char* pszName)	{ m_data.SetDepotCode(pszName); }

public: // profession
	virtual DWORD		GetProfession()    { return GetProfession(0); }
	DWORD		GetProfession		(int nType)	{ if(nType == 0) return m_data.GetJobByType(m_data.GetCurJob()); else return m_data.GetJobByType(nType);}
	DWORD		GetJobLev			(int nType = 0)	{ if(nType == 0) return m_data.GetJobLev(m_data.GetCurJob()); else return m_data.GetJobLev(nType);}
	DWORD		GetJobCon			(int nType = 0)	{ if(nType == 0) return m_data.GetJobCon(m_data.GetCurJob()); else return m_data.GetJobCon(nType);}
	DWORD		GetCurJob			()				{ return m_data.GetCurJob();}
	bool		SetCurJob			(int nJob = 1);

	void		SetProfession		(int nType,DWORD dwProff,  BOOL bUpdate = false);
	void		ReSetUserPoint		(int nType);
	void		SetJobLev			(int nType,DWORD dwLev, bool bUpdate = false)	{ if(nType == 0)  m_data.SetJobLev(dwLev,m_data.GetCurJob(),bUpdate); else  m_data.SetJobLev(dwLev,nType,bUpdate);}
	void		SetJobCon			(int nType,DWORD dwLev, bool bUpdate = false)	{ if(nType == 0)  m_data.SetJobCon(dwLev,m_data.GetCurJob(),bUpdate); else  m_data.SetJobCon(dwLev,nType,bUpdate);}

public:	//---------- USER专有的战斗函数 ----------------------
	void		IncAddPoint			(int nAddPoint);
	void		SetExp				(__int64 i64Exp);
	void		CalcFightRate		()						{ m_nFightRate = /*GetInterAtkRate()*/1000; }
	void		ProcessAutoAttack	();
	int			AdjustExp			(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg=false);
	int			AdjustEudemonExp	(IRole* pTarget,CMonster* pWarPet, int nRawExp, bool bNewbieBonusMsg=false);
	bool		CheckCrime			(IRole* pRole);		
	bool		IsImmunity			(IRole* pRole);	
	
	bool		checkMonsterTag(int monsterID);

public:	//--------- 队伍管理 --------------------------------
	BOOL		AbandonTeamLead();///如果队长退出,则移交队长给下一人-softworms-08-01-16
	BOOL		CreateTeam			();
	void		RebuildTeam();///上线时重构队伍
	BOOL		CreateDefaultTeam	();///增强的创建组队方式
	BOOL		AcceptTeamMember	(CUser* pMemeber);
	BOOL		AcceptTeamInvit		(CUser* pLeader);
	BOOL		AcceptTeamInvit		(CUser* pLeader,BOOL bSynchro);
	BOOL		QuitTeam			();
	BOOL		DismissTeam			();
	BOOL		KickOutTeamMem		(OBJID idMember);
	void		SetTeam				(CTeam* pTeam);
	void		ClrTeam				(BOOL bKickOut = false,bool bLeave = true);
	CTeam*		GetTeam				();
	void		ProcessTeamTimer	();
	void		SendShow			(CUser* pNewMember);
	void		AppendTeam			(TeamInfoStruct* pInfo);
	void		AppendAwardInfo(AwardInfoStruct* pInfo);
	void		SendPosToTeamMem(OBJID idMap, int nPosX, int nPosY);//goto 通知队友自己所在的地图
	void		GetPosFromTeamMem();//goto 获取队友的位置
	//20070202修罗:小队更名.仅自己的CTeam更名.不考虑成员
	void		ChangeTeamNameOnly(char *teanName);
	bool        CheckSearchRole();
	bool        CheckSearchTeam();
	bool        CheckSearchHouse();
	BOOL        ChangeTeamText(char *pText);
	BOOL        DenyTeamInvite(OBJID idPlayer, char *pText);
	void		ClearAllCD();
	// house
	vector<UINT> m_setHouseStore;

// 	UINT        GetStoreHouseIDByIndex(int nIndex);
// 	int         GetStoreHouseAmount();
// //	void        InitHouseStore();
// 	void        GotoPublicHouse();
// 	void        HouseQueryBuy();
// 	void        BuyHouse(char* pHouseName);
// 	void        SaleHouse();
// 	void        StoreHouse(int nID);
// 	void        DeleteStoreHouse(int nID);
// 	void        EnterMyHouse();
// 	void        EnterOtherHouse(int nID);
// 	bool        IsInHouse();
// 	void        LeaveHouseByOwner(OBJID idOwner);
// 	void        LeaverHouseByID(int nID);
// 	void        ExitHouse();
// 	void        SetHouseRule(int id, int nRule);
// 	bool        CanEnterOtherHouse(int id);
// 	void        PickupHousePay(int nID);
// 	void        HouseGrass(int id);
// 	void        HouseChat(const char* pStr, DWORD color, bool bAll, bool onlySelf = false);
// 	void        ThrowFish(int id);
// 	bool        UseHouseItem(OBJID idType);
// 	void        ProcessFish(int nID, int nFishLev, bool bAuto = false);
// 	bool        UseNets(int nID, int nFishLev, OBJID itemType, bool bNet = true);
// 	int         GetFishLev();
// 	int         GetFishLev2();
// 	int         RandFish(int nLev);
// 	void        FishStart();
// 	void        WaterFlower();
// 	void        PickupHook();
// 	int         GetWaterIndex();
// 	void        FishEscape();
// 	void        ProcessFishWorker(bool bStart, bool bFirst = false);
// 	void        ProcessFishTutor(bool bStart, bool bFirst = false);
// 	void        AutoHook(int nMode);
// 	void        AllHouseRenew();
 	void        SetClickNpcID(DWORD idNpc) { m_dNpcID = idNpc; }
 	DWORD       GetClickNpcID() { return m_dNpcID; }
 	bool        IsNearNpc(DWORD idNpc);

	void		ActivateShopNpc(DWORD idNpc);

	void        SetAutoFightFlag(bool bFlag) { m_bAutoFightFlag = bFlag;}
	bool        GetAutoFightFlag()           { return m_bAutoFightFlag; }

private:
	DWORD   m_dNpcID;
	DWORD m_nSearchRoleTime;
	DWORD m_nSearchTeamTime;
	DWORD m_nSearchHouseTime;
	bool  m_bAutoFightFlag;

protected: // IChangeTeam
	virtual bool AddTeamMember(OBJID idUser);
	virtual bool DelTeamMember(OBJID idUser);
	virtual bool DelTeam();
	virtual bool SendInfo(SOCKET_ID idSocket, OBJID idAgent);		// agent overload

private:
	CTeam*		m_pTeam;
	CTimeOut	m_tTeam;
	bool		m_bIsAllowTeam;
	bool		m_bIsAllowBuy;
	bool		m_bIsAllowFriend;
	bool		m_bIsAllowLook;	
	int			m_nHasLock;

public: //--------- 玩家行为 --------------------------
	void	SetIsAllowTeam(bool flag){m_bIsAllowTeam = flag;}
	bool	GetIsAllowTeam(){return m_bIsAllowTeam;}

	void	SetIsAllowBuy(bool flag){m_bIsAllowBuy = flag;}
	bool	GetIsAllowBuy(){return m_bIsAllowBuy;}

	void	SetIsAllowFriend(bool flag){m_bIsAllowFriend = flag;}
	bool	GetIsAllowFriend(){return m_bIsAllowFriend;}

	void	SetIsAllowLook(bool flag){m_bIsAllowLook = flag;}
	bool	GetIsAllowLook(){return m_bIsAllowLook;}

	void	SetUseLock(int nHasLock) {m_nHasLock = nHasLock;}
	bool	IsSetUseLock() {return (1 == m_nHasLock);}

	int			GetPk				()				{ return m_data.GetPk(); }
	void        SetPk               (int nPk);       //  [陈剑飞 7/10/2007]
	int			IncPk				()				{ return AddPk(PKVALUE_ONEKILL); }
	int			GetPkMode			()				{ return m_data.m_Info.ucPkMode; }
	void		SetPkMode			(int nMode)		{ m_data.m_Info.ucPkMode = nMode; }
	bool		IsPkEnable			(IRole* pRole);
	bool		IsGhost				()				{ return m_bGhost; }
	//2007828修罗:新的参数,供复活术调用------
	//void		Reborn				(bool bChgPos=true);
	void		Reborn				(bool bChgPos=true,bool bRebornFlag=false,int nRecoverPercent=0);
	//---------------------------------------
	void		TransformGhost		();
	int			CalcExpLostOfDeath	(IRole* pKiller);

protected:
	bool		m_bGhost;
public: //大宋荣誉 江湖侠义
	int			AddNationCon		(int nAdd);
	int			SetNationCon		(int nAdd);
	int			GetNationCon		()				{ return m_data.GetNationCon(); }

	int			AddChivalrous		(int nAdd);
	int			SetChivalrous		(int nAdd);
	int			GetChivalrous		()				{ return m_data.GetGoodEvil(); }

public:	// 可用于任务系统
	int			AddPk				(int nAdd);
	void		ProcessPkTimer		(void);	// auto synchro
	void		ProcessPkTimer_InPrison(void);
	bool		m_bInstanceReborn;///保存玩家是否刚重生
public: // item ------------------------------------------
	//06.11.7王玉波添加------------------------------------
	//06.11.8王玉波修改
	//物品的CD相关
	typedef map< int,CTimeOutMS* > MAP_ITEMDELAY;
	typedef map< int,CTimeOutMS* >::iterator MAP_ITEMDELAY_Iter;
	MAP_ITEMDELAY m_setItemDelay;

	//-----------------------------------------------------
	///////////////////////////////////////////////////////////
	enum { IDENT_OK = true, NO_COMBINE=false, };
	//20070626修罗:改成静态
	static void    EquipChgAttribute( ItemInfoStruct* pinfo,int nBossType = 0 ,int nAddType = 0);		//  [4/2/2007] 哈雷用来改变装备的属性
	CItem*	AwardItem		(int nItemType,bool bSynchro=true, bool bIdentOK=false, bool bAutoCombine=true,bool bChgattribute=false,int nAmmount = 1 , BOOL bSaveCreater =true,BOOL bInsert=true,BOOL bMarry = false, DWORD nKidnap = NULL,int nBossType = 0,DWORD dwPerPrice = 0);//bChgattribute 是用来控制创造物品的时候是否变化属性 int nAmmount = 0 是为了控制生成物品时一次生成多个  add  by halei 070402		// 同时操作数据库
	BOOL	AwardItemByCreate(int nItemType, bool bSynchro, bool bAutoCombine,bool bChgattribute,int nAmmount,int nKindnap,int addType,bool bIgnoreType=false,LPCTSTR szCreater=NULL);
	///softworms-2007-11-08-添加系统奖励物品给玩家,如果背备满则发系统邮件
	BOOL    AwardItemBySys(int nItemType, bool bSynchro, bool bIdentOK=false, bool bAutoCombine=true,bool bChgattribute=false,int nAmmount=1, int nSynID = 0,int nJuan = 0,int nKindnap = 0,int nBossType = 0,DWORD dwPerPrice = 0,OBJID idAddItem = ID_NONE);
	BOOL	AwardItemCount	(OBJID idItemType,UINT nItemAmount=1, bool bChgattribute = true,int nJuan = 0,int nKindnap = 0,int nBossType = 0,int nPerPrice = 0);///给用户指定数量的物品.
	CItem*	AwardItem		(ItemInfoStruct* pInfo, 
		bool bSynchro, 
		bool bAutoCombine=true,
		bool bChgattribute=false,//bChgattribute 是用来控制创造物品的时候是否变化属性   add  by halei 070402		// 同时操作数据库
		BOOL bSaveCreater=true,//20070707修罗:记录创造者
		BOOL bInsert=true,
		BOOL bRoleCreate = false,
		BOOL bCreateEudemon = true,
		int	 nBossType = 0);//[2009.4.17--张冬]记录捡地图上物品创造者
	bool	EraseItem		(OBJID idItem, bool bSynchro, bool bDelEudemon=true,bool bIsPlayerKill = false, bool bComBin = false);	// 同时操作数据库
	bool	EraseEquip		(int nPosition, bool bSynchro);							// 同时操作数据库
	void    SendEudemonTo   (CUser* pUser,OBJID idEudemon);
	void    SendEudemonListTo   (CUser* pUser);
	int		SendEudemonList();
	int		SendPeifangList();
	int		SendAllCD();
	int		SendAllUserkey();
	void	SendAllBuff();
	CItem*	GetItem(OBJID idItem, bool bShen = true, bool bStorage = false);
	
	CItem*	GetItemByEudemon	(OBJID idItemType);
	CItem*	GetItemByType	(OBJID idItemType);
	int  	GetItemNumByType	(OBJID idItemType);
	CItem*  GetFristCarryItem();
	CItem*	GetEquipItem	(OBJID idItem);
	CItemPtr*	GetEquipItemPtr	(int nPosition);

	//警告：用下面的接口的时候请谨慎，如要用请找主管，否则后果自负
	CItemPtr&	GetEquipItemRef	(int nPosition)						{ CItemPtr* ppEquip = GetEquipItemPtr(nPosition); ASSERT(ppEquip); return *ppEquip; }

	CItem*	GetEquipItemByPos	(int nPosition);
	int		GetEudemonAmount() { return m_pUserPet->GetShowAmount();}
	bool	AddItem			(CItem* pItem, bool bSynchro, bool bUpdate = true);
	bool	DelItem			(OBJID idItem, bool bSynchro);
	bool	GetItemInfoByItemid(OBJID idItem);//独孤求败添加[2008/12/3]
	//20061209修罗.删除多个物品.
	//考虑到删除个数超过此物品的最大叠加数,所以需要删除多个item
	//在删除之前,先调用GetItemByTypeMulti得到user有此类物品个数,个数不合法时返回false
	BOOL	DelItemByTypeMulti (OBJID idItem,int nAmount,bool& bHasDelKidnapItem, bool isID = false, bool bIgnoreLock=false);
	//20061209修罗.删除多个物品.重载
	//考虑到效率问题,重载.
	//不需要在删除前调用GetItemByTypeMulti().由外部首先调用,判断通过之后再来调用此函数.
	BOOL	DelItemByTypeMulti (OBJID *itemIDArray,int size,int amountNeedToDel);
	bool	DelItemPt		(OBJID idItem);							// 只删除指针
//	CUserPetData*	PopEudemon (OBJID idEudemon, bool bSynchro, bool bUpdate);
	CItem*	PopItem			(OBJID idItem, bool bSynchro, bool bUpdate = false);			// 立即清除OWNER
	bool	TryItem			(OBJID idItem, int nPosition);
	bool	ChkUseItem		(CItem* pItem, IRole* pTarget);
	bool	UseItem			(OBJID idItem, int nPosition, bool bSynchro);
	bool	UseItemTo		(OBJID idTarget, OBJID idItem);			// 对别人使用物品 -- zlong 2004.09.23
	bool	UnEquipItem		(int nPosition, bool bSynchro ,int nWant2Position = -1,int nWant2PackageIndex = -1);
	bool	SplitItem		(OBJID idItem, int nNum);
	bool	OrderPackage	(int package);
	bool	CombineItem		(OBJID idItem, OBJID idOther);
	CItem*	FindCombineItem	(OBJID idType);
	CItem*	CombineNewItem	(CItem* pItem, ItemInfoStruct* pInfo, bool bSynchro);
	//20061023修罗.增加一个参数nDonotCareDistance,快速拾取时用
	//非0时,取代函数内PICKMAPITEMDIST_LIMIT常量.
	bool	PickMapItem		(OBJID id,OBJID idItem,int nDonotCareDistance=0);
	bool	DropItem		( OBJID idItem, bool bIsMustDrop = false);
	bool	DropItemBySys	( OBJID idType,int x,int y ); //系统强制丢物品到地上...	
	bool	DelTaskItemByType(int type);
	bool	DropMoney		(DWORD dwMoney, int x, int y);
	bool	RepairItem		( OBJID NpcID,OBJID itemID, bool bSynchro );
	bool	RepairAll		( OBJID NpcID,bool bSynchro);
	bool	RepairAllSpecial(bool bSynchro,bool bWithHammer = false,OBJID hammerID = 0);//独孤求败[08/11/18]全身特修
	//bool	RepairAllSpecialWithHammer(OBJID hammerID,bool bSynchro);//独孤求败[08/11/20]使用特修锤修理身上装备
	bool	RepairOneSpecial(OBJID nEquipPos,bool bSynchro);//独孤求败[08/11/18]单件特修身上装备
	void	ProcessFixHammer(OBJID itemID);
	bool	RepairSpecial   ( OBJID itemID, bool bSynchro );	//  [4/16/2007] 特殊修理，哈雷
	CItem*	GetTaskItem		(LPCTSTR szItemName);
	CItem*	GetTaskItemByType		(OBJID idItemType, int nAmount=0);
	bool	IsItemFull		(int nWeight, OBJID idItemType, DWORD dwData);
	bool	IsItemFull		(ItemInfoStruct* pInfo);
	bool	IsBackPackSpare	(int nNum, int nWeight, OBJID idItemType, DWORD dwData)		{ return m_pPackage->IsPackSpare(nNum, idItemType, dwData); }		//{ if(nWeight && GetWeight()+nWeight > GetWeightLimit() || nNum && m_setItem.size()+nNum > _MAX_USERITEMSIZE) return false; return true; }
	CItem*	FindSpaceTransSpell();
	void	AddEquipmentDurability		(int nPosition, int nAddValue);
	bool	MultiDelItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum);
	bool	MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum);
	bool	DecEquipmentDurability(bool bBeAttack, bool bMagic, int bDurValue=1);
	virtual bool	SpendArrow();
	bool	SpendItem(CItem* pItem, int nNum=1, int nPosition=ITEMPOSITION_BACKPACK1, bool bSynchro=true);	// may be erase
	BOOL	SpendNewRemedy( CItem* pItem, int nPosition=ITEMPOSITION_BACKPACK1, bool bSynchro=true,CUserPetData* pData = NULL,int nMaxLife = 0);	//---彭峰-----2007/03/26----针对喝不完的血瓶写的使用函数-----------
	bool	SpendNewRemedy();//重载
	bool	SpendOneItem(OBJID idItem);//消费一个道具,供脚本使用
	BOOL	SpendNewEdemonRemedy( CItem* pItem, int nPosition=ITEMPOSITION_BACKPACK1, bool bSynchro=true );

	bool	RebornUseScroll(BOOL bUseScroll = TRUE/*是否使用复活卷*/,bool bForce = false);// 王玉波 [6/15/2007]，使用复活卷轴//郝超庆[08.9.17]增加一个参数
	bool    RebornInstance(bool bInstance = true);
	bool	EquipItem		(CItem* pItem, int nPosition, bool bSynchro);
	bool	CheckPackageFree(USHORT packageType = 47,USHORT amount = 1);
protected:	
	CItem*	UnEquipOnly		(int nPosition,int nWant2Position = -1,int nWant2PackageIndex = -1);///[2009.4.7--张冬]添加nWant2Position，nWant2PackageIndex参数实现卸载装备到指定背包中位置
	void	UpdateEquipmentDurability	();
protected:
	bool	CreateAllItem	(IDatabase* pDb);
	bool	CreateItemFree(IDatabase* pDb);
	void	SaveItemInfo	();
	void	DeleteAllItem	();
	int		SendAllItemInfo	();
public: // msgitem
	void	SendAllEquipInfoTo(CUser* pTarget);
	void	SendSecAtt(int nType = 0);
	void	BuyItem			( OBJID idNpc, OBJID idType,int nAmount, int nBuyType ,int nPrice = 0);//06.9.15王玉波修改
	bool	CheckRandShopFull();
	int		GetRandShopMoney();
	bool	SellItem		(OBJID idNpc, OBJID idItem,int nPriceSell = 0);
	bool	BegainYabiao(int nType,int nRule);
	bool	EndYabiao(int nType);
	void	SetYabiaoState(int nState) { m_nYaBiaoState = nState;}
	int		GetYabiaoState() { return m_nYaBiaoState;}
	void	SetYabiaoID(OBJID nState) { m_nYaBiaoID = nState;}
	OBJID	GetYabiaoID() { return m_nYaBiaoID;}
	void	SetYabiaoRule(int nRule){ m_nYaBiaoRule = nRule;}
	int		GetYabiaoRule() { return m_nYaBiaoRule; }

protected: // member
	bool	m_bUpdateWeight;		// false: m_nAllWeight值无效，需要重新计算
	int		m_nAllWeight;			// 当前总负重量的CACHE
	int		m_idxLastArrow;
	int	    m_nYaBiaoState;
	OBJID	m_nYaBiaoID;
	int		m_nYaBiaoRule;

public: // task & action -------------------------------------------------------------------------------
	void	ClearTaskID		()									{ m_setTaskID.clear(); m_setIterID.clear(); }
	void	SetTaskNpcID	(OBJID idNpc)						{ m_idTaskNpc = idNpc; }
	void	SetTaskItemID	(OBJID idItem)						{ m_idTaskItem = idItem; }
	OBJID	GetTaskItemID	()									{ return m_idTaskItem; }
	OBJID	GetTaskItemActionID()								{ return m_idTaskItemAction; }
	void	SetTaskItemActionID(OBJID idAction)					{ m_idTaskItemAction = idAction; }
	bool	ProcessTask		(int idx);
	OBJID   GetTaskIndex(OBJID idTask);
	///添加专为脚本使用的action处理接口,softworms-2007-12-7
	bool	ProcessAction	(OBJID idAction);
	bool    ProcessActionUser(OBJID idActionType,LPCTSTR szParam,int data);	
	bool	ProcessActionIncludeItem(OBJID idAction, CItem* pItem);//goto 为了让脚本操作物品，添加此函数

	bool	DebugShowAction	()									{ m_bDebugAction = !m_bDebugAction; return m_bDebugAction; }
	int&	TaskIterator	()									{ return m_nTaskIterator; }
//	void	ReplaceAttrStr		(char* pszTarget, const char* pszSource, IRole* pRole = NULL);
	int		PushTaskID			(OBJID idTask)					{ if(idTask!=ID_NONE && m_setTaskID.size() < MAX_MENUTASKSIZE) return m_setTaskID.push_back(idTask), m_setTaskID.size()-1; return INDEX_NONE; }

	int		PushTaskID			(OBJID idTask, OBJID idIter);
	CUserData* QueryDataForAction()								{ return &m_data; }
	void 	SetRecordPos	(OBJID idMap, DWORD dwX, DWORD dwY, BOOL bUpdate = false);
	
	void	AddTaskMask		(int idx);
	void	ClrTaskMask		(int idx);
	bool	ChkTaskMask		(int idx);


	_int64	GetTaskMask		();//20061121修罗:获得dwTaskMask

	_int64	GetDayTaskMask	();//20070916:获得dwDayTaskMask
	void	AddDayTaskMask		(int idx);///2007-09-17-softworms天任务掩码扩展
	void	AddDayTaskMask2		(int idx);
	void	ClrDayTaskMask		(int idx);
	void	ClrDayTaskMask2		(int idx);
	bool	ChkDayTaskMask		(int idx);
	bool	ChkDayTaskMask2		(int idx);

	_int64	GetWeekTaskMask	();
	void	AddWeekTaskMask		(int idx);
	void	ClrWeekTaskMask		(int idx);
	bool	ChkWeekTaskMask		(int idx);

	_int64	GetNewGiveMask2	();
	bool	AddNewGiveMask2		(int idx);
	void	ClrNewGiveMask2		(int idx);
	bool	ChkNewGiveMask2		(int idx);

	_int64	GetNewGiveMask	();
	bool	AddNewGiveMask		(int idx);
	void	ClrNewGiveMask		(int idx);
	bool	ChkNewGiveMask		(int idx);
	bool    GetNewGive(int nType);
	int     GetRandForActive();
	
	void	PickupNewCard(char* szCard);
	void	SetPickupNewCard(char* szCard, int TXTATR = 0);

	void	PickupNewCard2(char* szCard);
	void	SetPickupNewCard2(char* szCard, int TXTATR = 0);

	// ========= 佣兵任务相关 ============
	bool	AcceptMercenaryTask(OBJID idTask);
	bool	CancelMercenaryTask(OBJID idTask);
	bool	CompleteMercenaryTask(OBJID idTask, OBJID idItem);
	// ===================================


	void SendInsMemInfo(int nType);
protected:
	OBJID	GetTaskID			(int idx)						{ if(idx >= 0 && idx < m_setTaskID.size()) return m_setTaskID[idx]; return ID_NONE; }
	void	SetIterID			(int idx)						{ if(idx >= 0 && idx < m_setIterID.size()) m_nTaskIterator = m_setIterID[idx]; }

protected:
	vector<OBJID>			m_setTaskID;
	vector<OBJID>			m_setIterID;
	vector<OBJID>           m_vecEquipMagicID;
	OBJID					m_idTaskNpc;
	OBJID					m_idTaskItemAction;		// for idTaskItem
	OBJID					m_idTaskItem;
	bool					m_bDebugAction;				// 是否给GM显示每条ACTION的执行细节
	int						m_nTaskIterator;			// 用于在任务系统中叠代
	
public:	// wanted
// 	WantedInfoStruct& WantedInfo(void)		{ return m_data.GetInfo().infoWanted; }
// 	void	ResetWantedInfo(void)	{ memset(&m_data.GetInfo().infoWanted, 0L, sizeof(WantedInfoStruct)); }
// 	OBJID&	PoliceWantedID()	{ return m_data.GetInfo().idPoliceWanted; }
// 	void	TeamHuntKill		(OBJID idTarget, int nTargetLev, const char* pszTargetName);
// 	bool	IsMyWantedTarget	(CUser* pTarget);
// 	bool	IsPoliceWantedTarget(CUser* pTarget);

public: // magic ------------------------------------------------------------------------------------
	int		SendAllMagicInfo()		{ return m_pMagic->SendInfoToClient(); }
	CMagic*		QueryMagic()			{ CHECKF(m_pMagic); return m_pMagic; }
	IMagicData* QueryMagic(int nMagicType){return QueryMagic()->FindMagic(nMagicType);}

	static bool	IsInCircle(POINT pos, POINT posCenter, int nRange)	{ return sqrt((double)(posCenter.x-pos.x)*(posCenter.x-pos.x) + (double)(posCenter.y-pos.y)*(posCenter.y-pos.y)) < nRange+.9; }
//	bool		IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posMidline);
	bool		IsArrowPass(int x, int y, int nAlt=ARROW_FLY_ALT);
	bool		IsJumpPass(int x, int y, int nAlt);
protected:
	CMagic*		m_pMagic;
	int			m_nCurrSupermanKills;
	int			m_nTopSupermanKills;
	int			m_nSupermanOrder;

public: // Transformation ----------------------------------------
	bool		Transform(DWORD dwType, int nKeepSecs, bool bSynchro=true);
	CTransformation* QueryTransformation()		{ return m_pTransformation; }
	void		AbortTransform();
	bool		SynchroTransform();
protected:
	CTransformation*		m_pTransformation;
	CTimeOut				m_tTransformation;

public: // call pet -----------------------------------------------
	bool		CallPet(OBJID idMonsterType, int x, int y, int nKeepSecs=0);
	CMonster*	QueryCallPet()			{ return m_pWarPet; }
	void		KillCallPet(bool bNow=false);
	OBJID		GetCallPetID();
	CUserPet*	QueryUserPetSet()		{ CHECKF(m_pUserPet); return m_pUserPet; }
protected:
	CAutoLink<CMonster>		m_pCallPet;			// may be null
	CTimeOut				m_tCallPet;			// no active for keep alive

	//---任务系统---begin
	CTaskDetail	*	m_pTaskDetail;
	//---任务系统---end
	
public: // ---------------军团---------------------------------------------------------------------------------
	bool		SendAllSynInfo()		{ return m_pSyn->SendInfoToClient(); }
	CSynAttr*	QuerySynAttr()			{ if(m_pSyn) return m_pSyn; else return NULL; }		//? 调用时注意检查是否有帮派
	void		AwardSynWarScore(CNpc* pNpc, int nScore);
	bool		IsHostileSynMember	(CUser* pUser);
	CSyndicate* GetSyndicate	(void);
	int			QuerySynProffer	(void)	{ if(m_pSyn) return m_pSyn->GetProffer(); return 0; }
	void		SetSynProffer	(int nData)		{ if(m_pSyn) m_pSyn->SetProffer(nData); }
	int			QuerySynRank	(void)	{ if(m_pSyn) return m_pSyn->GetSynRankShow(); return RANK_NONE; }
	bool		IsMemberOfSyn	(OBJID idSyn);

protected: // data
	CSynAttr*		m_pSyn;
	CTimeOut		m_tIncRepute;

public: // user status --------------------------------------------------------------------------------
//	IStatus*	QueryStatus(OBJID idType)		{ CHECKF(m_setStatus); return m_setStatus->GetObj(idType); }
//	IStatusSet*	QueryStatusSet()				{ CHECKF(m_setStatus); return m_setStatus; }
	void		SetCrimeStatus(int nStatus=STATUS_CRIME, int nTime=CRIME_KEEP_SECS);
	void        SetSelfDefStatus(int nTime=SELF_DEFENCE_SECS);      //add huang 2004.1.14
protected: // data
//	IStatusSet*	m_setStatus;	

public: // friend --------------------------------
	int			GetFriendAmount	()					{ return m_setFriend.size(); }
	int			GetBeFriendAmount()					{ return m_setBeFriend.size(); }//独孤求败[2008/12/17]加了我为好友的玩家的数量
	int			GetFriendGroupAmount(int groupid);//[2010-06-07 goto]获取某个好友分组的好友数量
	int			GetFriendAllAmount();
	CFriend*	GetBeFriendByIndex(int idx)			{ CHECKF(idx>=0 && idx<m_setBeFriend.size()); return m_setBeFriend[idx];}//独孤求败[2008/12/17]
	CFriend*	GetFriendByIndex(int idx)			{ CHECKF(idx>=0 && idx<m_setFriend.size()); return m_setFriend[idx]; }
	CFriend*	GetFriend		(OBJID idFriend);
	CFriend*	GetBeFriend		(OBJID idFriend);
	CFriend*	GetBeFriendByName(char* pszName);
	CFriend*	GetFriendByName(char* pszName);	//  [2007-9-10] 朱斌
	bool		AddFriend		(OBJID idFriend, LPCTSTR szFriendName,USHORT nFriendType = 1 ,LPCTSTR szFriendMate="无");		// 未检查重复添加
	bool		AddBeFriend		(OBJID id,OBJID idUserOwnmeAsFriend,USHORT nFriendType ,LPCTSTR pszName );
	bool		SetFriendShip(OBJID idFriend,int nFriendShip);

	bool		DelFriend		(OBJID idFriend);			// 同时删除记录
	bool		DelBeFriend		(OBJID idUserOwnmeAsFriend);//独孤求败[2008/12/17]	

	bool		MoveFriend		(CFriend* pFriend,int nFriendType);//独孤求败[2008/12/15]
	void		BroadcastFriendsMsg	(CNetMsg* pMsg);
	void		DelFriendLeaveWord(LPCTSTR szName, DWORD dwId);//[2010-06-09 goto]删除已看过的用户留言
	void		DelTempFriend();
	OBJID		AddLeaveFriend(LPCTSTR szFriendName, unsigned int uFriendType, CMsgFriend* pFriendMsg);
	bool		HasTempFriend	()	{ return m_bHasTempFriend; }
	void		SetTempFriend	(bool bFriend)	{ m_bHasTempFriend = bFriend; }
protected:
	bool		CreateAllFriend	(IDatabase* pDb);
	bool		CreateAllBeFriend(IDatabase* pDb);//独孤求败[2008/12/16]获取那些加了自己为好友的玩家的信息
	void		DeleteAllFriend	();
	void		DeleteAllBeFriend();//独孤求败[2008/12/16]
	void		SendFriendInfo	();
public:
//		void		SetName	(const char* pszName)	{ m_data.SetName(pszName); }
		
// 		void        setQq  (const char* qqValue)    { m_data.SetQq(qqValue);   }
// 		char*       getQq()                         { return m_data.GetQq();}
// 
// 		void        SetProv(const char* pszProv)    { m_data.SetProv(pszProv); }
// 		char*       getProv()                       { return  m_data.GetProv();}
// 
// 		void        SetCity(const char* pszCity)    { m_data.SetCity(pszCity); }
// 		char*       getCity()                       { return m_data.GetCity();}
// 
// 		void        SetStar(const char* pszStar)    { m_data.SetStar(pszStar); }
// 		char*       getStar()                       { return m_data.GetStar();}
// 		
// 		void        SetOld (const char* pasOld)     { m_data.SetOld(pasOld);   }
// 		char*       getOld()                        { return m_data.GetOld();}
// 
// 		void        SetBtSex(const char* pasBtSex)  { m_data.SetBtSex(pasBtSex);}
//         char*       getBtSex()                      { return m_data.GetBtSex();}
//////////////////////////////////////////////////////////////////
public:	// tutor&student ------------------------------------
	// tutor
	CUserTutor*		GetTutorManager()					{ CHECKF(m_pUserTutor); return m_pUserTutor; }
//	void		SetTutor(TutorInfoStruct * data);			
//	bool		AddTutor			(CUser* pTutor)		{ CHECKF(m_pUserTutor); m_pUserTutor->AddTutor(pTutor); }
//	bool		DelTutor			()					{ CHECKF(m_pUserTutor); m_pUserTutor->DelTutor(); }//徒弟单方解除师徒关系
//	bool		AddStudent			(CUser* pStudent)	{ CHECKF(m_pUserTutor); m_pUserTutor->AddStudent(pStudent); }
	//bool        RequestDelTutor();                      //请求删除师傅
	bool        ProcessTutorDelReq(int nApplyType);     //处理师傅删除请求
	//int         GetDensityWithTutor();                  //得到与师傅的亲密度
	bool        ProcessTutorRecover();                  //处理师傅恢复
	void		QueryTutorList( USHORT nPage, int nPageSize );//查询登记的师父列表 (页码，每页显示数量)
	bool		IsInTutorList()	{ return (m_data.GetTutorMask() & TUTOR_INLIST) != 0; }//玩家是否登记了师父列表
	void		JoinToTutorList();//玩家加入师父列表
	void		DelFromTutorList();//从师父列表中删除
	void		DelTutorListMask()	{ m_data.SetTutorMask( m_data.GetTutorMask() & (~TUTOR_INLIST) ); }//从师父列表中删除玩家
	void		AddTutorListMask()	{ m_data.SetTutorMask( m_data.GetTutorMask() | TUTOR_INLIST ); }

// 	void        RequestAddDensity(int nDensity);        //请求增加亲密度
//	void        AddImpartByStudent(int nImpart)	{ CHECK(m_pUserTutor); m_pUserTutor->AddImpartByStudent(nImpart); }//通过学生增加传授度
//	void        AddImpartByTutor(OBJID idStudent, int nImpart)	{ CHECK(m_pUserTutor); m_pUserTutor->AddImpartByTutor(idStudent, nImpart); }//通过学生增加传授度 //通过师傅增加传授度
//	int			GetImpartByStudent();//徒弟查询传授度
//	int			GetImpartByTutor(OBJID idStudent);//师父查询传授度

	//int         GetTutorTeamAddExp(int nExp);

	bool		TakeExpFromStudent	(int nExpTake);		// 取得学员贡献的经验
	bool		IsFirstTutor		()					{ return (m_data.GetTutorMask() & TUTOR_ISFIRST)==0; }//是否是第一次拜师
	void		SetNotFirstTutor	()					{ m_data.SetTutorMask(m_data.GetTutorMask() | TUTOR_ISFIRST); }//设为不是第一次拜师
	bool		HasGraduated		()					{ return (m_data.GetTutorMask() & TUTOR_HASGRADUARED)!=0; }//是否出过师
	void		SetGraduated		()					{ m_data.SetTutorMask(m_data.GetTutorMask() | TUTOR_HASGRADUARED); }//设为出过师
	bool		IsTutorTeam			();//队伍成员是否是师徒关系

	// student
	int			GetStudentAmount	(bool bIncludeGraduated)		{ CHECKF(m_pUserTutor); return m_pUserTutor->GetStudentAmount(bIncludeGraduated); }
//	CTutor*		GetStudentByIndex	(int idx)			{ CHECKF(idx>=0 && idx<m_setStudent.size()); return m_setStudent[idx]; }
	CTutorData*	GetStudent			(OBJID idStudent)	{ CHECKF(m_pUserTutor); return m_pUserTutor->FindStudentByID(idStudent); }
	CTutorData*	GetTutor			()					{ CHECKF(m_pUserTutor); return m_pUserTutor->GetTutor(); }
	//bool		DelStudent			(OBJID idStudent); //师徒协议解除关系
	bool		TutorDivorce		()					{ CHECKF(m_pUserTutor); return m_pUserTutor->TutorDivorce(); }//师徒协议解除关系
	bool		DelStudentByTutor	(OBJID idStudent,bool bDelDBRec=true)	{ CHECKF(m_pUserTutor); return m_pUserTutor->DelStudentByTutor(idStudent,bDelDBRec); }
	bool		DelTutorByStudent	(bool bDelDBRec=true)	{ CHECKF(m_pUserTutor); return m_pUserTutor->DelTutorByStudent(bDelDBRec); }
	//bool        RequestDelStudent   (OBJID idStudent);       //请求删除学生
	bool        ProcessStudentDelReq(OBJID idStudent, int nApplyType); //处理学生删除请求
	bool        ProcessStudentRecover(OBJID idStudent);      //处理学生恢复
	bool		IsStudentFull		();			

	//void		SendTutorInfo		(USHORT nPage);//goto 增加页码参数
	//void		NotifyTutorStatus	(int nStatus);
	bool		Graduate(OBJID idStudent)			{ CHECKF(m_pUserTutor); return m_pUserTutor->Graduate(idStudent); }//师父带徒弟出师
	//bool		DelTutorRef			();
	//bool		DelStudentRef		(OBJID idStudent);

	//bool        CancelLeaveTeach();                         //取消离开师傅
	//bool        CancelDelStudent(OBJID idStudent);          //取消删除学生
	//bool        RecoverTutor();                             //恢复师傅
	//bool        RecoverStudent(OBJID idStudent);            //恢复学生
	bool        RevQesCelLevTeach(OBJID idStudent);         //收到询问取消离开师傅
	bool        RevQesCelDelStudent();                      //收到询问取消删除学生

// 	DWORD		GetTutorMask(OBJID idStudent);//获取师父奖励掩码
// 	void		SetTutorMask(OBJID idStudent, DWORD dwMask);//设置师父奖励掩码

//	bool		AddStudentRef(CUser* pUser)	{ CHECKF(m_pUserTutor); return m_pUserTutor->AddStudentRef(pUser); }
//	bool		AddTutorRef(CUser* pUser)	{ CHECKF(m_pUserTutor); return m_pUserTutor->AddTutorRef(pUser); }
	int			GetMaxScore()	{ return m_data.GetMaxScore(); }
	DWORD		GetImpart()		{ return m_data.GetTutorLevel(); }
	int			GetMorality()	{ return m_data.GetMorality(); }//师德
	void		SetMorality(int nData)	{ m_data.SetMorality(nData); }
	void		AddMorAlity(int nData)	{ CHECK(nData>0); SetMorality(GetMorality()+nData); }
	bool		SpendMorAlity(int nData);
	void		AddImpart(int nData)	{ CHECK(nData>0); m_data.SetTutorLevel(GetImpart()+nData); }

	int			GetTutorApplyAmount()		{ CHECKF(m_pUserTutor); return m_pUserTutor->GetTutorApplyAmount(); }

	OBJID		ApplyAddTutor(ST_MEMBERINFO* pInfo, TUTORSTATE nState, ST_MEMBERINFO* pInfoExt)	{ CHECKF(m_pUserTutor); return m_pUserTutor->ApplyAddTutor(pInfo,nState,pInfoExt); }
	DWORD		CalcTutorGiftMask(int nLev);
	bool		AddTutorPreach(OBJID idItem, int nAmount);//师徒传道任务 搜集魂
	int			GetTeamStuInLev(int nMaxLev, int nMinLev, int nLowerThan=0);//获取队伍中指定等级范围的徒弟数量
	bool		HasSameIPStuInTeam();//队伍中是否有相同IP的徒弟
	bool		CheckItemCreater(OBJID idItem);
	bool		HasTutorGift(OBJID idStu, int nLev)	{ CHECKF(m_pUserTutor); return m_pUserTutor->CanGetGift(idStu, nLev); }//判断师父奖励
	bool		SetTutorGiftMask(OBJID idStu, int nLev)	{ CHECKF(m_pUserTutor); return m_pUserTutor->AddGiftMask(idStu, nLev); }//设置师父奖励掩码
	int			GetTeamStuAvgLev();	//队伍中徒弟的平均等级
	int			GetStuLevAddedInTeam();//队伍中的徒弟在师父门下升过的级数
	int			GetInstanceTime(int nInstanceType);
	int			GetInstanceFinishTime(int nInstanceType);
protected:
	//bool		AddStudentRef		(const TutorInfoStruct* pInfo);

	//bool		CreateAllTutor	(IDatabase* pDb);
	//void		DeleteAllTutor	();
	void        TutorLoginHint();             //师徒登陆提示
	
public: // enemy ------------------------------------
	void		TutorLogoutHint();			  //师徒下线
protected:
	OBJID		m_idCompLastCreat;
	bool		m_bCompLastStrEnd;
	bool		m_bCompLastMagicEnd; 
	OBJID		m_idCompLastDickQquEnd;
	OBJID		m_idCompLastGenEnd;
	OBJID		m_idCompLastGenQquEnd;
	OBJID		m_idliveLastResourse;
	OBJID		m_idliveLastMixEnd;

public: // money --------------------------------
	virtual bool	SpendMoney	(DWORD nMoney, bool bSynchro);
	virtual bool	SpendMoneyByType(DWORD dwMoney, int nType, bool bSynchro);
	virtual bool	GainMoney	(DWORD nMoney, bool bSynchro,int nType = 1,bool bUpdate=false);
	bool			SaveMoney	(DWORD nMoney, bool bSynchro = false);
	bool			DrawMoney	(DWORD nMoney, bool bSynchro = false);
	void			GainVas(DWORD dwMoney);//请求账号服务器加钱
	void			GainVasCPST(DWORD dwMoney);
	void			SpendVas(DWORD dwMoney);//扣除元宝，同步账号服务器

	bool			SaveDiamond( DWORD dwDiamond,bool bSynchro = false );
	bool			DrawDiamond( DWORD dwDiamond,bool bSynchro = false );
	bool            SpendCityWarItem(int nAmount);
public: // tick --------------------------------
	void			ProcessTick	(DWORD dwTime);
protected:
	deque<DWORD>	m_dequeServerTick;
	DWORD			m_dwFirstServerTick;
	DWORD			m_dwLastServerTick;

	DWORD			m_dwFirstClientTick;
	DWORD			m_dwLastClientTick;

	DWORD			m_dwLastRcvClientTick;

public: // weather ----------------------------------
	OBJID			GetWeatherID()					{ return m_idWeather; }
	void			SetWeatherID(OBJID id)			{ m_idWeather = id; }
	bool			SetWeather(int nType, int nIntensity, int nDir, DWORD nColor, DWORD nKeepSecs, DWORD nSpeedSecs);
protected:
	OBJID			m_idWeather;
//--------------07.4.10王玉波注释-----------------------------------------
/*
public: // energy -------------------------------------
	void		SetEnergy			(int nEnergy)	{ m_data.m_Info.usEnergy = nEnergy; }
	int			GetEnergy			()				{ return m_data.m_Info.usEnergy; }
	int			GetMaxEnergy		()				{ IStatus* pStatus = QueryStatus(STATUS_MAXENERGY); if (pStatus) return ::CutTrail(0, AdjustData(m_data.m_Info.usMaxEnergy, pStatus->GetPower(), 100)); return m_data.m_Info.usMaxEnergy; }
	void		AddEnergy			(int nEnergy)	{ m_data.m_Info.usEnergy = ::CutRange(m_data.m_Info.usEnergy+nEnergy, 0, GetMaxEnergy()); }
	void		ResetEnergyInc		()				{ m_tEnergy.Update(); }
protected:
	CTimeOut		m_tEnergy;*/
//---------------end-------------------------------------------------------

public: // light --------------------------------------
	void		SetLight			(int dwRGB, int nSecs=0);
	void		SendLight			();

public: // CBooth -------------------------------------
	CBooth*		QueryBooth()						{ return m_pBooth; }
	bool		CreateBooth(int nPosX, int nPosY, int nDir)	;
	void		DestroyBooth();
protected:
	CBooth*			m_pBooth;		// NULL: no in booth

protected:
	CTimeOut		m_tAttrackMonster;	// 吸引怪物持续时间 补丁:)

public: // login, return false: logout_user
	bool			IsDefaultLoginSite();
	bool			EnterMap		();
	bool			LeaveMap		();
	int				SendItemSet		();
	bool			SendGoodFriend	();

public: 
	void        DelCurNpc();

public: // mapgroup ---------------------------------
	//20070329修罗:新增一个默认参数.默认值0,表示请求.1表示响应处理
	//bool			ChangeMapGroup	(PROCESS_ID idProcess, OBJID idMap, int nPosX, int nPosY);	// out
	bool			ChangeMapGroup	(PROCESS_ID idProcess, 
		OBJID idMap, 
		int nPosX, 
		int nPosY,
		Enum_ChangeMapGroupMode mode = ENUM_CNGMAPGROUP_MODE_REQ);	// out
	bool			JoinMap			(OBJID idMap, int nPosX, int nPosY);		// in
	bool			AppendObjInfo	(INFO_ID nType, void* pInfo);
	bool			SendObjInfo(PROCESS_ID idProcess, OBJID idUser, INFO_ID nInfoMsgID, void* pInfo, int nSize);
protected:
	//06.11.8王玉波添加-------begin-----------------------------------------------------------
	//关于物品的CD

	bool			AppendCountTimer( CountTimerStruct* pInfo );//计时器信息
	//------------------------end-------------------------------------------------------------
	bool			AppendItem		(ItemInfoStructSend* pInfo);
	bool			AppendJinmei	(JinmeiInfoStruct* pInfo);
	bool			AppendKey		(SYSTEMUSERKEY* pInfo);
	bool			AppendAutoFight	(AUTOF* pInfo);
	bool			AppendFriend	(FriendInfoStruct* pInfo);
	bool			AppendBeFriend	(FriendInfoStruct* pInfo);
	bool			AppendStatus	(StatusInfoStruct* pInfo);
	bool			AppendPeifang	(ST_PEIFANGDATA* pInfo);
	bool			AppendTaskDetial(ST_TASKDETAILDATA* pInfo);
	bool			AppendEudemon	(EudemonData* pInfo);
	bool			LeaveMapGroup	();

public: // common ------------------------------------------
	CAutoLink<CUser>&	QueryLink	()					{ return m_link; }

public: // IUserSquareDeal ------------------------------------------
	bool	CreateSquareDeal		(CUser* pTarget);
	ISquareDeal*	QuerySquareDeal	()		{ m_ifaceSquareDeal.CheckDistance(); return m_ifaceSquareDeal.m_pSquareDeal; }

	class XUserSquareDeal : IUserSquareDeal			// SquareDeal 接口 
	{
	public: // constructor
		XUserSquareDeal()			{ m_pSquareDeal = NULL; }
		~XUserSquareDeal()			{ if(m_pSquareDeal) m_pSquareDeal->Release(); }
		void	CheckDistance();

	public:
		virtual bool	Create		(ISquareDeal* pSquareDeal);
		virtual ULONG	Release		();
		virtual CUser*	QueryUser	()					{ return This(); }

	public: // application
		// return false: too weight
		virtual bool	AddItem		(CItem* pItem, int nAmount, int nAllWeight);
		virtual bool	DelItem		(CItem* pItem, int nAmount, int nAllWeight);

		virtual bool	TradeLock		();
		// return false: too more
		virtual bool	AddMoney	(int nAllMoney);
		virtual void	ClickOK		();
		virtual bool	AddTradeVAS(int nAllMoney);
		virtual	bool	AddMonster(CUserPetData* pPetData, int nAmount);
		virtual	bool	DelMonster(CUserPetData* pPetData, int nAmount);
	protected:
		ISquareDeal*		m_pSquareDeal;

	private:
		CUser*		This()	{ return m_pParent; }
		CUser*		m_pParent;
		friend class CUser;
	}	m_ifaceSquareDeal;

	friend class CUser::XUserSquareDeal;

protected: // 登录控制量
	enum { STEP_NONE=0, STEP_LOGIN, STEP_ENTERMAP };
	int				m_nStep;
	
protected: // 玩家管理的对象
	typedef vector<CFriend*>	FRIEND_SET;
	FRIEND_SET	m_setFriend;//保存当前玩家的所有好友信息
	FRIEND_SET  m_setBeFriend;//独孤求败[2008/12/16]保存加了当前玩家为好友的所有玩家信息
	CUserTutor*		m_pUserTutor;

	IUserPackage*	m_pPackage;	// 背包
//	IUserPackage*	m_pMagicStoneBox;	// 魔晶盒
public:
	IUserPackage*	QueryPackage()		{ ASSERT(m_pPackage); return m_pPackage; }
//	IUserPackage*	QueryMagicStoneBox()	{ ASSERT(m_pMagicStoneBox); return m_pMagicStoneBox; }
protected:
	CItemPtr	m_pShoulder;
	CItemPtr	m_pArmor;
	CItemPtr	m_pHelmet;
	CItemPtr	m_pWeaponR;	
	CItemPtr	m_pNecklace;
	CItemPtr	m_pBangle;
	CItemPtr	m_pGlove;
	CItemPtr	m_pCaestus;
	CItemPtr	m_pShoes;
	CItemPtr    m_pMount;
	CItemPtr	m_pRingT;	
	CItemPtr	m_pRingB;
	CItemPtr	m_pAdornT;	
	CItemPtr	m_pAdornB;
	CItemPtr	m_pDress;
	CItemPtr	m_pWeaponSoul;

	//---jinggy---2004-11-19---装备经验值---begin
	CTimeOut    m_arrTimeOutForEquipExp[ITEMPOSITION_EQUIPEND-ITEMPOSITION_EQUIPBEGIN];
	int			m_arrEquipIncExp[ITEMPOSITION_EQUIPEND-ITEMPOSITION_EQUIPBEGIN];//设备战魂经验值的增量
	CTimeOut    m_tEquipWarGhostExp; //检测装备的战魂间隔时间
	bool		m_bIsEnableWarGhostExp;

//	int			m_Worktype;
//	int			m_WorkSub;
	CTimeOut	m_tTimeOnline; //帮派成员在线时间
	//---jinggy---2004-11-19---装备经验值---end

	CUserData	m_data;	
public:
//	DWORD		GetExpTutor(){return m_dwExpToTutor;}
//	void		SetExpTutor(DWORD exp){m_dwExpToTutor = exp;}
//	void		pushStudentVector(TutorInfoStruct * tempSetStudent);
//	void		ClearStudentVector(){m_setStudent.clear();}
//-----------------------------07.1.12王玉波添加，隐身-----------begin----------
	bool		IsHide()	{ if( IsGM() )	return m_bHide;return false;}
	bool		ChangeHideStatus();
protected: // 玩家状态
	bool		m_bHide;//是否隐身了
	int			m_nLevExp;
	int			m_nSynDressArmorType;
//	CGameMap*	m_pMap;

public:	
	void ClsBuff(int nStatus);
//	void AddBuff(int nStatus,int nTime,int nCount,int nParam,int nTimes=0,int nParam2=0,int nParam3=0);
	void	AddBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 );//07.4.26王玉波添加
	void	UpdateBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 );//07.4.26王玉波添加
	
	void	QueryTopList( USHORT usType, USHORT nPage );				//查询排行榜//彭峰2007/02/10修
	
	void UseWeaponWithGem( IRole* pTarget );				//有三种宝石嵌入武器处理
	void IncSP();
	int AdjustAttribDamage( int nPower,CMonster* pTarget); //调整属性攻击
	int  GetPhyle();

	bool	IsPublicMap( int nMapID );			//是不是公共地图
	bool	IsOwnMap( int nMapID );				//是不是本族地图

//-----------------------06.10.28背包机制新修改-------------------begin-----------------------------------------------
	void	SendUnlockItemMsg( OBJID id );
	
	CItem*	GetItemFromSpecificPack( OBJID idItem,int nPackArrayIndex )					{ return m_pPackage->GetItemFromSpecificPack( idItem,nPackArrayIndex );}
	void	ProcessChgItemPackIndex( OBJID idItem,int nWant2PackageIndex );
	void	ProcessChgItemPosAndIdx( OBJID idItem,int nWhichPackage,int nWant2Position,int nWant2PackageIndex );
	void	ProcessTradeItemPackageIndex( OBJID idItem1,OBJID idItem2 );
	void	ProcessTradeItemPosAndIdx( OBJID idItem1,int nWhichPackage,int nWant2Position,int nWant2PackageIndex , OBJID idItem2);


//----------------------------------------------------------------end-------------------------------------------------
	CTaskDetail* GetTaskDetail();
	void SetTaskSpendEneType(OBJID idTask, USHORT nType);
	UINT GetTaskSpendEneType(OBJID idTask);
	void SetSynDressArmorType(int nArmorType);	
//	void NotifyChatRoomOffline();
	bool ApplyAttackSyn(CNpc * pNpc);
	int GetAvailableSynFund();
	bool FlyToRandomPos();
	bool FlyToMainMap();
	bool IsEmbedGemType(int nGemType);
	DWORD GetLastLogin();
	DWORD GetLastLogout();
	DWORD GetCreateTime();
	DWORD GetPassedTime(DWORD curTime,int nType =1);
	//20040416修罗:支持可选的同步,更新
	//void AddMoney(DWORD dwMoney);
	void AddMoney(DWORD dwMoney,BOOL bSynchro=true,BOOL bUpdate=false);

	bool IsSynLeader();
	void ProcessMsgAction(int nAction, int nData, OBJID id);
	void ResourceWarVictory();
	DWORD		m_dwLastCoolShow;
//关于自我介绍
// 	DWORD	GetLastChangeDescTime()				{ return m_data.GetLastChangeDescTime();	}
// 	void	SetLastChangeDescTime( DWORD time )	{ m_data.SetLastChangeDescTime( time );		}
	char*	GetDescription()					{ return m_data.GetDescription();			}
	void	SetDescription( char* desc )		{ m_data.SetDescription( desc );			}
	void	ChangeMyDesc( char* desc );			//更改自我介绍,在这里控制条件..
	void    SendTragetDesc(CUser *pTraget);		//得到某人的自我介绍
//关于GM指令
	static long	GetTopUserCount();//-------彭峰---2007/05/17-----改为静态，可以直接用名字空间调用 最大在线人数
	
	static long	GetCurUserCount();//-------彭峰---2007/05/17-----改为静态，可以直接用名字空间调用 目前在线人数

public:
	int		GetNewKardState(int index = 0){return m_data.GetNewKardState(index);}
	void	SetNewKardState(int data,int index = 0){m_data.SetNewKardState(data,index);}
	void	SetCardState( int nIndex, int nData,bool bSend = false );
	int		GetCardState( int nIndex);
protected: // fight
	CBattleSystem*		m_pBattleSystem;
	CTimeOutMS			m_tFight;

	CTimeOut			m_tPK;
	CTimeOut			m_tFati;
//	CTimeOut			m_tPK_InPrison;	//监狱中减少PK值的速度
	CTimeOut			m_GotoPrison;
	CTimeOut			m_tSynWork;		//帮派打工
	CTimeOutMS			m_tSlowHealLife;
	CTimeOutMS			m_tSlowHealMana;
	CTimeOut			m_tAutoHealRecover;					// 每几秒自动回复生命,魔法.

	CTimeOut			m_tEquipmentDurability;				// 装备耐久度损耗

	int					m_nFightRate;						// 攻击的间隔毫秒数

	CTimeOut			m_tHpMpAdd;							// 特殊大红大蓝的自动回复速度
	CTimeOut			m_EverydayOL;
	CTimeOut			m_tUpdateDatabase;

protected: // 应用控制量
//	int			m_nWalkStep;
//	CTimeOut	m_tLock;
	CAutoLink<CUser>	m_link;

protected: // 控制量
	SOCKET_ID	m_idSocket;
//	PROCESS_ID	m_idProcess;
	ULONG		m_nIPAddress;
protected:
	//---jinggy---2004-11-17---升级装备经验值---已经穿在身上的装备的map---begin
	map<OBJID,CTimeOut*> m_mapEquipmentsUsed ;
	typedef pair <OBJID,CTimeOut*> PairID;	
	//---jinggy---2004-11-17---升级装备经验值---已经穿在身上的装备的map---end
public:
	//20061010修罗.复数查询by TypeID.由指针参数返回idItem[]
	//由引用参数返回实际的size.由函数返回amount
	//返回-1时表示idItem[]的实际size超出maxSize.但仍正常返回了maxSize之内的数据
	int	GetItemByTypeMulti(OBJID idType,int &maxSize,OBJID *idItem,bool isKidnap = true);
	int GetEudemonAmountByType(OBJID idType);
	int EquipCreateNew(OBJID idPeifang, OBJID idAdditem);
	DWORD	m_dwWarSroce;

	//equip
	//goto 全部改成有符号类型,防止数据越界
	LONG	m_equipPhysicAttackTotal;
	LONG	m_equipMagicAttackTotal;
	LONG	m_equipPhysicDefenseTotal;
	LONG	m_equipMagicDefenseTotal;
	LONG	m_equipHitarteTotal;
	LONG	m_equipDodgeTotal;
	LONG	m_equipCrihitTotal;
	LONG	m_equipToughTotal;
	LONG	m_equipAddhpTotal;
	LONG	m_equipAddmpTotal;
	LONG	m_equipStrTotal;
	LONG	m_equipIntTotal;
	LONG	m_equipStaTotal;
	LONG	m_equipSpiTotal;
	LONG	m_equipAgiTotal;

	LONG	m_equipAtt1;
	LONG	m_equipAtt2;
	LONG	m_equipAtt3;
	LONG	m_equipAtt4;
	LONG	m_equipDef1;
	LONG	m_equipDef2;
	LONG	m_equipDef3;
	LONG	m_equipDef4;

	//武魂特殊部分
	LONG	m_WPSLPhysicAttack;
	LONG	m_WPSLMagicAttack;
	LONG	m_WPSLPhysicDefense;
	LONG	m_WPSLMagicDefense;
	LONG	m_WPSLHitarte;
	LONG	m_WPSLDodge;
	LONG	m_WPSLCrihit;
	LONG	m_WPSLTough;
	LONG	m_WPSLStr;
	LONG	m_WPSLInt;
	LONG	m_WPSLSta;
	LONG	m_WPSLSpi;
	LONG	m_WPSLAgi;
	LONG	m_WPSLAtt1;
	LONG	m_WPSLAtt2;
	LONG	m_WPSLAtt3;
	LONG	m_WPSLAtt4;
	LONG	m_WPSLDecPhyDef;
	LONG	m_WPSLDecMagDef;
	LONG	m_WPSLDecDef1;
	LONG	m_WPSLDecDef2;
	LONG	m_WPSLDecDef3;
	LONG	m_WPSLDecDef4;
	stRandData m_WPSLRandAddPhyAtt;
	stRandData m_WPSLRandAddMagAtt;
	stRandData m_WPSLRandAddAtt1;
	stRandData m_WPSLRandAddAtt2;
	stRandData m_WPSLRandAddAtt3;
	stRandData m_WPSLRandAddAtt4;

	
	//goto 全部改成有符号类型,防止数据越界
	LONG	m_dwGiftAddphyatt;
	LONG	m_dwGiftAddmagatt;
	LONG	m_dwGiftAddphydef;
	LONG	m_dwGiftAddmagdef;
	LONG	m_dwGiftAddhit;
	LONG	m_dwGiftAdddodge;
	LONG	m_dwGiftAddcrihit;
	LONG	m_dwGiftAddTough;
	LONG	m_dwGiftAddhp;
	LONG	m_dwGiftAddmp;
	LONG	m_dwGiftAddStr;
	LONG	m_dwGiftAddInt;
	LONG	m_dwGiftAddSta;
	LONG	m_dwGiftAddSpi;
	LONG	m_dwGiftAddAgi;

	//gift
	LONG	m_GiftAtt1;
	LONG	m_GiftAtt2;
	LONG	m_GiftAtt3;
	LONG	m_GiftAtt4;
	LONG	m_GiftDef1;
	LONG	m_GiftDef2;
	LONG	m_GiftDef3;
	LONG	m_GiftDef4;
	//goto 对宠物生效的被动技能,如帮派技能等
	LONG	m_GiftPetAtk;//外功
	LONG	m_GiftPetMagAtk;//内功
	LONG	m_GiftPetCrihit;//暴击
	LONG	m_GiftPetMaxLife;//血上限

	//buff debuff有可能是减少属性的，所以必须是有符号类型
	LONG	m_dwBuffAddphyatt;
	LONG	m_dwBuffAddmagatt;
	LONG	m_dwBuffAddphydef;
	LONG	m_dwBuffAddmagdef;
	LONG	m_dwBuffAddhit;
	LONG	m_dwBuffAdddodge;
	LONG	m_dwBuffAddcrihit;
	LONG	m_dwBuffAddTough;
	LONG	m_dwBuffAddhp;
	LONG	m_dwBuffAddmp;
	LONG	m_dwBuffAddStr;
	LONG	m_dwBuffAddInt;
	LONG	m_dwBuffAddSta;
	LONG	m_dwBuffAddSpi;
	LONG	m_dwBuffAddAgi;

	LONG	m_BuffAtt1;//火
	LONG	m_BuffAtt2;//冰
	LONG	m_BuffAtt3;//玄
	LONG	m_BuffAtt4;//毒
	LONG	m_BuffDef1;//火
	LONG	m_BuffDef2;//冰
	LONG	m_BuffDef3;//玄
	LONG	m_BuffDef4;//毒

	//各项基本属性值
	//goto 全部改成有符号类型,防止数据越界
	LONG	m_dwUserphyatt;
	LONG	m_dwUsermagatt;
	LONG	m_dwUserphydef;
	LONG	m_dwUsermagdef;
	LONG	m_dwUserhit;
	LONG	m_dwUserdodge;
	LONG	m_dwUsercrihit;
	LONG	m_dwUserTough;
	LONG	m_dwUserhp;
	LONG	m_dwUsermp;
	LONG	m_dwUserStr;
	LONG	m_dwUserInt;
	LONG	m_dwUserSta;
	LONG	m_dwUserSpi;
	LONG	m_dwUserAgi;

	LONG	m_dwUserAtt1;
	LONG	m_dwUserAtt2;
	LONG	m_dwUserAtt3;
	LONG	m_dwUserAtt4;
	LONG	m_dwUserDef1;
	LONG	m_dwUserDef2;
	LONG	m_dwUserDef3;
	LONG	m_dwUserDef4;
	OFTENUSE_GEMADDITION m_mOftenUseGemAddition;		//常用宝石附加属性

	//宠物附体属性
	LONG	m_eudemonSoulphyatt;//外攻
	LONG	m_eudemonSoulmagatt;//内攻
	LONG	m_eudemonSoul1;//火
	LONG	m_eudemonSoul2;//冰
	LONG	m_eudemonSoul3;//玄
	LONG	m_eudemonSoul4;//毒
	LONG	m_eudemonSoulAddHp;//回血
	LONG	m_eudemonSoulAddMp;//回蓝
	LONG	m_eudemonSoulAddDodge;//闪避
	LONG	m_eudemonSoulAddHitRate;//命中

	clock_t m_tLastAddHp;
	clock_t m_tLastAddMp;
public:
//-------------------------------------------------------------------------------------------------------
//	CStrRes m_objStrRes;     // 整理服务端系统提示 [陈剑飞8/20/2007]  
//	const	char* GetStr(OBJID idRes){return m_objStrRes.GetStr(idRes);} 
//--------------------------------------------------------------------------------------------------
//	BOOL AddSpirit(int nSpirit);
//	BOOL SpendSpiritChip(CItem* pItem, int nPosition = ITEMPOSITION_BACKPACK1, bool bSynchro = true );
//	BOOL ImportSpiritStoneToChip(int* pMaterial, int nSize, char* szError);
//	int CheckExp(__int64& nAddExp);			//	20070703 朱斌 新增 检查经验值增减
	__int64 CheckExp(__int64& nAddExp);	//  [11/14/2007 朱斌]	全换成64位	//	20070703 朱斌 新增 检查经验值增减
	BOOL DoesUserNeedDeathPunishment(IRole* pRole = NULL);			//	// 20070702 朱斌 新增 判断玩家是否要死亡惩罚
	void UpdateEquipBasicProperty();			//更新装备基本属性.

	//void InitOftenUseGemAddition();				//CUser 构造时调用
	void ResetOftenUseGemAddition();			//reset m_mOftenUseGemAddition.每次update之时调用
	//void ClearOftenUseGemAddition();			//CUser	析构时调用
	void UpdateOftenUseGemAddition();			//更新常用宝石附加属性
	
	void UpdateCalculatedProperty(UPDATECALTYPE nType = UPDATECALTYPE_ALL);			//更新计算之后的基础属性.其中会调用以上各函数.

	bool UpdateEudemonSoulProperty();//宠物附体的属性
	//-----------------------------------------------------------------
													


	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//20070115修罗:放弃任务
	BOOL TaskGiveUp(DWORD taskID,int step = 0);
	///softworms-运行该死的脚本
	bool RunScript(const char* txtScript=NULL);
	void InitScript();///初始化脚本环境,为后面运行脚本作准备
	void InitItemScript(CItem* pItem);///初始化道具脚本环境,为后面运行脚本作准备
	//20070226修罗:背包内装备耐久损失一定百分比.返回实际损失耐久装备的个数
	int	PackageDurationLose(int durationLosePercent);

	//20070518修罗:
	BOOL IsClosing(){return m_data.m_Info.bClosing;}
	void SetClosing(){m_data.m_Info.bClosing = true;}

	//20070602修罗:修改登出机制---------------------
	//是否正在登出
	BOOL IsLogingout(){return m_tLogout.IsActive();}
	//是否登出超时
	BOOL IsLogoutTimeOut(){return m_tLogout.TimeOver();}
	//开始登出
	void BeginLogout(){m_tLogout.Startup(LOGOUTTIME);}
	//打断登出
	void BreakLogout(){m_tLogout.Clear();}
	//----------------------------------------------

	//20070614修罗:锁定攻击状态同步
	void SynchroLockAtkBegin();
	void SynchroLockAtkEnd();

	//	20070712	朱斌	
	CItemPtr GetUserAllItemByType(OBJID idType);	//	按类型ID查找全部装备,仓库,背包,魔晶盒,身上
	CItemPtr GetStorageItemByType(OBJID idType);	//	查找仓库
	CItemPtr GetEquipItemByType(OBJID idType);		//	查找装备在身上的
	//	20070712	朱斌	
	//	判断玩家有没有此类型物品,如果有,发送文件给玩家此物品只能有一个,并返回TRUE,否则返回FALSE
	static	BOOL	 IsUserHaveAnExclusiveItem(OBJID idType, const char* pszItemName, CUser* pUser);

	//20070716修罗:在自己脚边掉落物品.
	//物品非自身,而是系统创建的
	//mapItem创建时,任何人都可以拾取
	BOOL DropItemCreatedBySys(DWORD itemType,//itemtype or money
						  BOOL bIsMoney=false//true:money.false:item						  
						  );

	BOOL SynchroBCMaxLife();//2007918修罗:同步广播最大血量
	BOOL SynchroBCMaxMana();//2007918修罗:同步广播最大魔法
	BOOL SynchroBCMaxSP();// 同步广播最大体力 [陈剑飞11/15/2007]

	DWORD GetStengLevel()const {return m_LastStrengLevel;}
protected:
    bool m_NeedSynchroStrengLve;
	int m_LastStrengLevel;//--上次强化等级，只有与上次不同时
public:

	MYHEAP_DECLARATION(s_heap)
public:
	void CheckItemLifeTime(CItemPtr pItem, BOOL bUpdateToClient, BOOL bSaveToDatabase);	//  [2007-9-14] 朱斌
	void ActivateItem(CItemPtr pItem);
	void AddRemoteTeamMember(ST_TEAM_INVITE* pTeamInvite, BOOL bIsTargetTeam);
	void AwardHonor(int nHonor, BOOL bUpdate = TRUE, int nSynchro = TRUE);	//  [2007-8-30] 朱斌	加荣誉

	Singnal_v1 soltKillPlayer;///玩家被杀事件
	Singnal_v1 soltPlayerEnterMap;///玩家进入地图事件
	Singnal_v1 soltPlayerExitMap;///玩家进入地图事件
	Singnal_v1 soltPlayerTeamCreate;///玩家队伍信息改变事件
	
	void ResetDayTaskMask();///重置天任务掩码
	void ResetDayTaskMask2();
	void ResetWeekTaskMask();


	int ProcGift1001(IRole* pTarget,int nType, int oldValue,int valueMask = valueMask_ALL);

//PVP
	bool IsPVPStuts()	{return m_bPVPStuts;}	
	OBJID GetPVPTargetID()	{return m_idPVPTarget;}
	int GetPVPX()	{return nPVPX;}
	int GetPVPY()	{return nPVPY;}
	OBJID GetPVPMap()	{return m_idPVPMap;}
	bool IsPVPLong() { return m_bPVPLong;}

	void SetPVPLong(bool bStuts) {m_bPVPLong = bStuts;}
	void SetPVPStuts(bool bStuts)		{m_bPVPStuts = bStuts;}
	int	 GetPVPStuts()					{return m_bPVPStuts;}
	void SetPVPTargetID(OBJID idTarget)	{m_idPVPTarget = idTarget;}
	void SetPVPX(int x)	{nPVPX = x;}
	void SetPVPY(int y)	{nPVPY = y;}
	void SetPVPMap(OBJID idMap)	{m_idPVPMap = idMap;}
//task
	void SetLastGiveupTaskID(OBJID idTask)	{m_dwLastGiveupTaskID = idTask;}
	OBJID GetLastGiveupTaskID()	{ return m_dwLastGiveupTaskID;}
protected:
	DWORD m_dwLastGiveupTaskID;
	bool m_bPVPLong;
	bool m_bPVPStuts;
	OBJID m_idPVPTarget;
	int nPVPX,nPVPY;
	OBJID m_idPVPMap;
private:
	CTimeOut	m_tItemLifeTime;	//  [2007-9-19] 朱斌	生命周期检查间隔
	CTimeOut	m_tCardMsgTime;
	//	CTimeOut	m_tItemLifeTimeUpdate;	//  [2007-9-19] 朱斌	生命周期更新间隔
	
public:
	void CheckItemLifeTime(BOOL bUpdateToClient = FALSE);	//  [2007-9-17] 朱斌	检查物品生命周期
public:
	void ChanceDropItemAndEquip(int changeDrop);
//	BOOL WeaponSoulOutEx(CItem* pItem, int nIndex, BOOL bSendMail);
	void RebornInPos(OBJID nMapID, LONG nX, LONG nY, bool bRebornFlag=false, int nRecoverPercent=0);
	BOOL	m_bFirstCheckItemLifeTime;	//  [2007-10-10] 朱斌	第一次上线检查
	BOOL SynchroBoothItem(CItem* pItem,int nItemInfoAction,int nBoothIndex,int nMoney);
	BOOL SynchroItem(CItem* pItem,
		int nItemInfoAction,
		OBJID idUser=ID_NONE,//用于CMsgItemInfo
		int nWpslAction=NULL);//20071214修罗:不再直接使用CMsgItemInfo来同步.包装一下.
	BOOL SendDetailItem(CItem* pItem);
public:
	void	UpdateGiftBaseProperty();
	void	UpdateBuffBaseProperty();
protected:
	BOOL	SynchroUserProperty(UPDATECALTYPE nType);	//  [12/18/2007 朱斌]	同步角色基本属性

public:
	USHORT GetActivityAward(USHORT usType);
	void   SetActivityAward(USHORT usType, USHORT usState, bool bSend=false);
	void   SetMobChargeAward(USHORT usType, USHORT usState, bool bSend=false);//手机充值卡查询/操作

protected:
	DWORD m_doubletime;//用于记录玩家冻结的双倍时间，只在初始化时访问数据库一遍，避免频繁访问数据库
	bool IsInMapInstance();///softworms判断玩家是否在副本里

	char m_szCryOut[_MAX_WORDSSIZE];
	DWORD	m_dwVas;//记录增值币数量
	DWORD	m_dwVasJuan;
	int     m_hpState;
	int     m_mpState;
	int     m_KillMonsterNum;          //当前杀怪数量
	int     m_killMonsterTargetNum;       //杀怪的目标数量(读取数据库)
	int     m_killMonsterTimeInterval; //杀怪的时间间隔(读取数据库)

	long	m_MoneyIncr;			   //goto 银两增量,可为负
	long	m_ExpIncr;				   //goto 经验增量,可为负

	//goto 活动奖励
	USHORT	m_nActivityAward;
	USHORT  m_ChargePack1;
	USHORT  m_ChargePack2;
	USHORT  m_ChargePack3;
	CUserTitle* m_pTitle;//称号
	CTreasure* m_pTreasure;//goto 彩票,开箱子
	COnlineGift* m_pOnlineGift;//上线祝福
public:
	void SetUseTitle(int nType, int nLev, bool bUpdate = false);//[游途道标 2008/10/13]设置当前使用的称号
	int  GetUseTitle()	{ return m_data.GetUseTitle(); }//返回的值为称号ID*100+称号等级
	//void SetHoldTitle(UINT nPower, BOOL bUpdate = true);//[游途道标 2008/10/13]设置当前拥有的称号
	void AddHoldTitle(UINT nType, int nLev=1);
	void DelHoldTitle(UINT nType);
	CUserTitleData* FindTitleByType(UINT nType);
	void LevUpTitle(UINT nType);
	int  GetTitleLev(UINT nType);
	void SetTitleLevel(UINT nType, int nLev);
	//int CmpHoldTitle(UINT nPower);
	//_int64 GetHoldTitle() const {return m_data.GetHoldTitle();}//[游途道标 2008/10/13]获取当前拥有的称号
//	UINT GetUseTitle() const {return m_data.GetUseTitle();}//[游途道标 2008/10/13]获取当前使用的称号
	CUserTitle* QueryTitle()	{ CHECKF(m_pTitle); return m_pTitle; }
	void SendAllTitle();

	//goto 彩票,开箱子相关
	CTreasure* QueryTreasure()	{ CHECKF(m_pTreasure); return m_pTreasure; }
	void SendAllTreasure();
	bool IsLockedEudemon(OBJID idEudemon);
	void LockEudemon(OBJID idEudemon);
	void UnLockEudemon(OBJID idEudemon);
	bool CanEudemonBorn(OBJID idEudemon);
	bool CanEudemonPlay(OBJID idEudemon);
	bool CanEudemonBorn();
	bool CanEudemonPlay();
	bool EudemonSetBorn(OBJID idEudemon,bool bBron , int bronTime);
	bool EudemonSetPlay(OBJID idEudemon, int nPlayTimes);
	bool EudemonBronGet();
	bool EudemonPlayGet();
	bool EudemonBron(int nType,OBJID idEudemon1,OBJID idEudemon2,CUser* pOther = NULL);
	bool EudemonBaby(OBJID idEudemon);
	bool EudemonUnderUp(OBJID idEudemon);
	void SetInstanceRebornPos(int nPosX, int nPosY);
	void ProcessDeadAddTeamAttr();
	bool AwardEudemon(int nEudemonType,EudemonData& data , bool bSet = false, bool born = false,int nBornGrow = 0,bool bCompensation = false);
	CUserPetData* AwardEudemonByEgg(int nEudemonType,EudemonData& data);
	bool LoadEudemon(OBJID id,EudemonData& Data,int nType = 3);
	bool EudemonMix(OBJID idEudemon1,OBJID idEudemon2);
	bool EudemonSkill(OBJID eduemonItemId,OBJID idItem,int skill,bool bLean =true);
	void EudemonDead();
	CMonster*	CallEudemon(OBJID idEudemon,bool bSave = true,bool bSend = true);
	void CallBackEudemon(OBJID idEudemon,bool bSave = true,bool bSend = true,bool bHappy = true);
	bool CreateEudemon(OBJID idEudemonType,bool bSaveData,EudemonData* pData = NULL, bool born = false,int nBornGrow = 0,bool bCompensation = false);
	DWORD AwardEudemonExp(__int64 i64Exp);
	int	 GetEudemonLevOff(int index);
	bool EudemonChgName(OBJID idEudemon,char* name);
	void EudemonEat(OBJID idEudemon);
	DWORD QueryLeaveExp();
	void UpdateLeaveExp();
	bool GetLeaveExp(int nType);
	void EudemonLifeSpanadd(OBJID idEudemon);
	void EudemonTrian(OBJID idEudemon);
	bool EudemonGiveup(OBJID idEudemon, bool bIgnoreLock = false);
	void DetachStatusFromEudemon(OBJID idEudemon);//宠物被召回或死亡时去掉宠物给主人加的状态
	CUserPetData* PopEudemon(OBJID idEudemon, bool bSynchro, bool bUpdate=true);
	bool AddEudemon(CUserPetData* pEudemon, bool bSynchro, bool bUpdate=true);
	bool AddPeifang( OBJID idPeifang, bool bSynchro);
	bool DelPeifang( OBJID idPeifang, bool bSynchro);
	bool HasPeifang( OBJID idPeifang);
	OBJID m_idEudemonCur;
	OBJID m_idNowEudemon;
	OBJID m_idSoulEudemon;
	bool  m_bSoulEudemonView;
//	OBJID m_idEudemonCurForChangeMap;

	bool MapClean(UINT nID, UINT waitsec);

	void ReadAutoFightData();
	void WriteAutoFightData(struct AutoFightInfo &AutoInfo);
	void ReplyAutoFightData();
	void SetAutoFightPickUpMode(int nMode);
	OBJID GetSoulEudemon()	{ return m_idSoulEudemon; }
public:
	bool HasMount()	;
	bool HasEudemon()	;
public:
// 	const char * GetCryOut();
// 	void SetCryOut(const char *lpszCryOut);
// 	BOOL SendMail(CItemPtr pItem);//[游途道标 2008/12/24]将收购的东西发邮件给自己.
//	BOOL SendKillerMail(CItemPtr pItem);
	DWORD GetVas() {return m_dwVas;}//[游途道标 2008/12/16]获得增值币数量
	void SetVas(DWORD dwVas) {m_dwVas = dwVas;}//[游途道标 2008/12/16]设置增值币数量

	DWORD GetVasJuan() {return m_dwVasJuan;}
	void SetVasJuan(DWORD dwVas) {m_dwVasJuan = dwVas;}

// 	void ChangeVasToAcount(int dwVas);
// 	SAVE_ITEM   m_SaveItem;
// 	CUserSaveItem m_UserSaveItem;
// 	void GetSaveItem(bool bNpc = false);
// 	void GetUserSaveItem(bool bSend = false);//尝试从表中找被自己杀的玩家的item
// 	bool ProcessSaveItem(OBJID itemId,OBJID itemTypeId,bool IsMoney=true,bool IsWSA = false);
// 	bool SendItemByMail(OBJID nItemType, bool bSynchro, bool bIdentOK=false, bool bAutoCombine=true,bool bChgattribute=false,int nAmmount=1, int nSynID = 0);
// 	bool CheckSaveItemLifeTime();
// 	bool GetSaveInfo();
// 	bool FindItem(OBJID itemId);      //寻找物品
// 	bool GiveUpSaveItem(OBJID itemId); //放弃道具赎回
// 	bool GetWillSaveItem();    //获得站利品信息
public: //玩家按键信息
	void ClearBooth();
	bool IsLuxuryBooth();
	void SendNotice(char *pszTalk);
	int  GetDlgState() { return m_pUserKey.GetDlgStates(); }
	int  GetFeelSwitch() { return m_pUserKey.GetFeelSwitch(); }
	void SetFeelSwitch(int nSwitch)	{ m_pUserKey.SetInt(SYSKEY_FeelSwitch, nSwitch); }//此处不更新数据库，转场时更新
	void SetItemLock() { m_pUserKey.SetInt(SYSKEY_SYSTEMSETTING, m_pUserKey.GetUserSysSet() | USERCONFIG_ITEMLOCK); }
	void SetItemUnLock()	{ m_pUserKey.SetInt(SYSKEY_SYSTEMSETTING, m_pUserKey.GetUserSysSet() & ~USERCONFIG_ITEMLOCK); }
	bool IsAgreeTeamApply()		{ return m_pUserKey.IsAgreeTeamApply(); }
	bool IsAgreeTradeApply()	{ return m_pUserKey.IsAgreeTradeApply(); }
	bool IsAgreePKApply()		{ return m_pUserKey.IsAgreePKApply(); }
	bool IsAgreeFriendApply()	{ return m_pUserKey.IsAgreeFriendApply(); }
	bool IsItemLockedOpen()				{ return m_pUserKey.IsLocked(); }
	CMonster *m_pMonster; //[游途道标 2009.04.03]攻击杀死的怪物
	OBJID m_idItem; //[游途道标 2009.05.22]触发的物品
	CUserKey m_pUserKey;
	CAutoFightData m_pAutoFightData;
	CSpecilLineInfo* m_pSpecilLineInfo;
	bool   IniUserKey(bool bSend = true,bool bLogin = true);
	bool   SetUserKey(int nData,bool bUpdate = false);
	bool   SaveUserKey(struct UserKey &KeyInfo,bool bUpdate = false);

	USHORT GetWalkInt(void) {return m_walkInt;}
	void SetWalkInt(USHORT data) {m_walkInt = data;}
	void SaveEudemon();
	void ExchangePet(); 
	bool IsSelecetItem(OBJID idItemType) { return idSelectItemType == idItemType;}
	void SetTaskSelect(OBJID idItemType) { idSelectItemType = idItemType;}
	bool GetMountState()			{ return m_IsOnMount; }
	void SetMount()					{ m_IsOnMount = true; }
	void SetUnMount()				{ m_IsOnMount = false; }
	DWORD GetMountDelay()			{ return clock()-m_lastMountTime; }
	void UpdateMountDelay()			{ m_lastMountTime=clock(); }
	void IgotAnOrangeWeapen(int ItemTypeId, CItemPtr pItem,char* pMonsterName=NULL);									//公告：我捡到了XXXX极品 大家来杀我吧！
	void WeapensoulBroadCast(int nMsgtype,CItemPtr pItem,ST_WEAPONSOULDATA WPSLData,int nData);
	void JinmeiBroadCast(int nMsgtype, int nJinmeiIndex, int nData);
	CMonster* m_pWarPet;
	void SaveEudemonStatus();
	void RestoreEudemonStatus();

	BOOL CheckJoinSynDays(int nday);
private:
	OBJID idSelectItemType;
	bool bMateTeam;///[2009.2.12--张冬]队友中是否有配偶
	UINT m_nCityWarDomain;
	UINT m_nCityWarInstanceType;
	int  m_nCitySynWarType;
//	bool m_IsMarryTeam;///[2009.2.13--张冬]是否进行过IsAccountTeamMember()判断
	SYSTEMTIME m_createTime;
	OBJID m_nBabiID;
	bool bClear;
	//副本重生坐标
	UINT m_nInstanceRebornX;
	UINT m_nInstanceRebornY;
	USHORT m_walkInt;

	CUserPet* m_pUserPet;
	vector<StatusInfoStruct> m_setEudemonStatus;

	CUserPeifang* m_pUserPeifang;
	bool	m_IsOnMount;

// By Minsky,2010-05-28，添加现有邀请的组队数量的标志
private:
	OBJID m_idInviteTeamID[5];
public:
	bool SetInviteUserID(OBJID id);
	OBJID GetInviteUserID(int index);
	bool IsInviteUserID(OBJID id);
	void ClrInviteUserID(OBJID id = ID_NONE);

//vip
private:
	CTimeOut	m_tVipTime;
	CTimeOut	m_tVipFaceTime;
	DWORD       m_dwLastVipCheckSendTime;
public:
	void ClrVipTime();
	bool AddVipTime(DWORD dwMin);
	void CheckVipTime();
	void CheckVipFaceTime();
	bool IsVipActive();
	DWORD GetVipLev();

	//[2010-08-27 goto]特殊大红大蓝的增量，最后一位表示类型,1表示大瓶，2表示小瓶,大小表示的是总上限
	DWORD GetHpAdd	()	{ return m_data.GetHpAdd(); }
	DWORD GetMpAdd	()	{ return m_data.GetMpAdd(); }
	DWORD GetPetAdd	()	{ return m_data.GetPetAdd(); }
	void  SetHpAdd	(DWORD dwHpAdd, bool bUpdate=false)	{ m_data.SetHpAdd(dwHpAdd, bUpdate); }
	void  SetMpAdd	(DWORD dwMpAdd, bool bUpdate=false)	{ m_data.SetMpAdd(dwMpAdd, bUpdate); }
	void  SetPetAdd	(DWORD dwPetAdd, bool bUpdate=false) { m_data.SetPetAdd(dwPetAdd, bUpdate); }

	bool FreezeDoubleExp(UINT nBufID);//冻结双倍经验
	bool RestoreDoubleExp(UINT nBufID);//恢复冻结的双倍经验
	bool CheckFreezeDoubleExp(UINT nBufID);
	bool WriteStateToDB(IStatus* pStatus);
	bool DelStateFromDB(int nStatus);
	bool InitStorage(IDatabase* pDb,bool bLogin = true);

	//新手出生地图分配
	OBJID GetBornMap();


	int GetVit() { return m_data.GetVit(); }//活力	
	void AddVit(int nData);
	void SetVit(int nData);
	int GetMaxVit(){ return m_data.GetMaxVit(); }

	int GetEne(){ return m_data.GetEne(); }//精力
	void AddEne(int nData);
	void SetEne(int nData);
	int GetMaxEne(){ return m_data.GetMaxEne(); }	
//eudemon
private:
	CTimeOut  m_awardcheck;
	DWORD m_lastEudemonLifespanCheck;
	DWORD m_lastSoulEudemonLifeSpan;
	bool m_bSee;
	DWORD m_lastMountTime;//上次使用坐骑技能的时间，用于控制坐骑技能CD
	bool  m_bHasTempFriend;//是否有临时好友，如果没有，下线时就不用执行删除
	int	  m_GoldPoint;//goto 账号服务器的点券数量,因为用户表和账户表都有点券，只好加个变量来存了
	int	  m_nTutorQuestion;//师徒答题题号
	int	  m_nTutorAnswer;//师徒答题答案
	CUserWarGame* m_pUserWarGame;
	CPetEgg* m_pPetEgg;
//防沉迷相关
public:
	int  RDropEquip(int DropNum);		//随机掉指定数量的装备
	bool isFatigue() {return ((GetFatigue() != 1) && (m_pUserKey.GetFatigue() != 1));}
	bool isFiftyP(); 
	bool isZeroP();	 
	void ProcessFatigue();
	void CheckEudeLifespan();
	bool CanBeSee() { return m_bSee;}   
	int  GetIdentity() {return m_data.m_Info.Identity;} 
	void SetIdentity(int IdNum){m_data.SetIdentity(IdNum);}
	
public://用于加好友安全验证
	void PickupNewCard3(char *szCard);
	DWORD GetPetMaxLife();
	DWORD GetPetLife();
	void SetSynSkill(UINT uData, int nSub);
	int IsPEIFANGNeedKidnap(OBJID TypeID, CPeifangTypeData*	pPeifangType);
	void ProcessEverydayOL();
	int GetEverydayOL();
	void ResetMonsterKillAndEveryDayOL();
	void JudgeMonsterKill(int nAdd);
	void AddMoneyByType(DWORD nMoney, int nType);
	UINT PVPEmendDamage(UINT nLoseLife, OBJID AttId, bool bOverPVP = true);
	bool IsEighteen(int Birthday);
	bool IsRight(string card);
	bool BirthdayIsRight(string cardId);
	int GetDay(int year,int month);
	void SendFatigue();
	int GetFightPetUnderstand(CMonster* pMonster);
	DWORD GetDress();
	void SetDress(OBJID nDress);
	bool GetVotingGift(bool bignorAll = false);//参数用于测试
	void PickupVotCard(char* szCard);
	bool GetVotingGift2(char* szCard);
	void PickupVotCard2(char* szCard);
	bool GetVotingGift3(char* szCard);
	void PickupVotCard3(char* szCard);
	void AnounceNewFaceToFriend(int nNewFace);
	void SetVipfaceTime(OBJID nData);
	void ClrVipFace();
	OBJID GetVipFaceTime();
	OBJID GetVipface();
	void SetVipface(int nData, bool bActivNow = true, int dwDay = 7);
	bool IniUserAutoFight(bool bLogin = true);
	void IgotAnAmazingEudemon(int nEudemonType, CEudemonTypeData*	pEudemonType, char* szName, OBJID EudId);
	BOOL Contribute();
// 	void SetconSyn(int nData);
// 	DWORD GetconSyn();
	BOOL ConBuild(OBJID dwBuild, int EndFlag, OBJID idItem);
	bool ConItem(OBJID idItem, int EndFlag);
	bool ConMoney(DWORD dwMoney ,bool bMoney, int EndFlag, bool bSynchro = true, OBJID idItem = 0);
//	void ProcessWork();

/////////////////////////////////////////////////////////////////////////--jay
/*
RANDLINE=0,//随机
LINEONE=1,//1线
LINETWO=2,//2线
LINETHREE=3,//3线
LINEFOR=4,//4线
LINEFIVE=5,//5线
LINESIX=6,//6线
SPECILLINE=20,//专线
	*/	protected:
bool m_bChangeLine;
bool m_bLoaded;

public:
	bool	ChangeLineMap(OBJID MapID,int nPosx,int nPosy,int nLine);//转换线路和地图
	
	OBJID	GetSpecilMapID(){return m_pSpecilLineInfo->GetInt(SPECILLINEINFO_MAPID);}
	int		GetSpecilPosX(){return m_pSpecilLineInfo->GetInt(SPECILLINEINFO_POSX);}
	int		GetSpecilPosY(){return m_pSpecilLineInfo->GetInt(SPECILLINEINFO_POSY);}
	void	SetSpecilPos	(OBJID idMap, DWORD dwX, DWORD dwY, SPECILLINESTATE nState ,BOOL bUpdate = false);
	void	SetSpecilSteate(SPECILLINESTATE nState ,bool bUpdate = false);
	int		GetSpecilState(){return m_pSpecilLineInfo->GetInt(SPECILLINEINFO_STATE);} 
////////////////////////////帮派相关//////////////////////////////////////////////--jay
	bool LeaveSynMap();
	bool IsMySynMap();
	bool EnterSynMap(int nPosx,int nPosy);//进入帮派地图--jay
	void SetSynTaskGift(OBJID nGift1,OBJID nGift2);//放入奖励礼物--jay
	int GetGiftMask(bool bGift);//判断奖励掩码--jay
	int GetSynSci(int nSub);//获得技能等级
	int GetNowWeekDay();//获得当前是星期几
	int GetHistorySynCon();//获得历史总帮贡
	int GetHistorySynMoney();//获得历史总钱贡
	int GetHistorySynGrow();//获得历史总建设
	void SetHistorySynMoney(int nMoney);//历史总钱贡
	void SetHistorySynGrow(int nGrow);//历史总建设
	OBJID  GetSynMapID();
	int GetMySynRank();//获得人物职位
	int GetSynMoney();//获得帮派资金
	int GetSynGrow();//获得帮派建设
	void SetSynMoney(int nMoney);//
	void SetSynGrow(int nData);
	int GetMonsterType();
	int GetMonsterPosx();
	int GetMonsterPosy();
	CGameMap *GetSynMap();
	CMonster *GetAttackingMonster(); 
	bool IsChangeLine(){return m_bChangeLine;};
	void SetChangeLine(bool bChangeLine){ m_bChangeLine = bChangeLine; };
	void BroadcastSyn(LPCTSTR pPost,int type=1);
	bool EnterSynInstance(OBJID idmap,OBJID nPosx,OBJID nPosy);
	bool LeaveSynInsten(OBJID idmap,OBJID nPosx,OBJID nPosy);
	bool IsLoaded()	{ return m_bLoaded; }
	bool CheckIsSameSyn(int nType);//0判断全队,1判断个人
	void SaveLog(int nType,OBJID nValue,bool isGet,int nGetWay);//开放给脚本,添加日志
	int	 CulData(USHORT nType,int nData1,int nData2);

public:
	bool RemoveEudemonSoul();
	bool EudemonSoul(OBJID idEudemon);
	bool AddEudemonSoulView();
	//bool GoToWork(int nType, int nSub);
	OBJID GetInstenceType();
	void InitAwarTime(bool bLogin = true);
	void SetNextAwardTime(int nsecs);
	bool IsAwardTimeOut();
	void SetAward(DWORD awardtime,bool update = false);
	DWORD GetAward();
	void SendEmail(const char* szRecvUser,const char* szWords,  CUser* pTarget);
	void ClearAllDOT();
	DWORD GetFrozenDoubleTime();
	void InitDoubleTime();
	DWORD GetVipLeftMinites();

	int GetGoldPoint()	{ return m_GoldPoint; }
	void SetGoldPoint(int nPoint)	{ m_GoldPoint = nPoint; }

	double	GetPassedTimeFromCreate(LPCTSTR szTime);//获取玩家创建角色的时间到指定时间的分钟数

	_int64  GetASales() {return m_data.m_Info.i64BuyASales;} //购买的特价商品
	void	SetASales(_int64 nData, bool bUpdate=false)	{m_data.SetASales(nData, bUpdate);}
	void	SendASalesItem();//同步特价商品
	void	SendLimitItem();//同步限量商品

	bool AwardWeaponExp(DWORD dwExp);
	bool FullWeaponLifespan();
	CItem* CreateNewSoul(OBJID idItemType,ST_WEAPONSOULDATA wpslInfo,int nKindnap = 0,OBJID idGem1 = 99999,OBJID idGem2 = 99999,OBJID idGem3 = 99999);

	int	GetTutorAnswer()	{ return m_nTutorAnswer; }
	void SetTutorAnswer(int nAnswer)	{ m_nTutorAnswer = nAnswer; }

	int	GetTutorQuestion()	{ return m_nTutorQuestion; }
	void SetTutorQuestion(int nQuestion)	{ m_nTutorQuestion = nQuestion; }

	CUserWarGame* QueryWarGame()	{ CHECKF(m_pUserWarGame); return m_pUserWarGame; }
	void AddWarGameMask(int nType)	{ CHECK(m_pUserWarGame); m_pUserWarGame->AddWarGameMask(nType); }
	UINT GetArenaScore()	{ return m_pUserWarGame->GetArenaScore(); }

	bool WarGameSignup(OBJID idWarGame);//竞技场报名
	bool EnterWarGame(OBJID idWarGame);//进入竞技场
	bool EnterWarGameFromNpc(OBJID idWarGame);//从npc进场，脚本调用
	bool LeaveWarGame();

	CPetEgg* QueryPetegg()	{ CHECKF(m_pPetEgg); return m_pPetEgg; }
	bool FlushPetegg(OBJID idData, OBJID idItem, int nGrow, int nNextFlush);//提供给脚本的，由脚本决定机率和下次刷新需要等级的时间(秒)
	void SendPeteggData(OBJID idItem);//脚本调用，查询宠物蛋候选宠物
//modify code by 林德渊 begin 2011.5.30
private:
	int vaslv_question_answer;
	int vaslv_question_answerid;
	int vaslvid;
	CVasLvData* m_pvaslvdata;
public:
	void SetVasLvQuestionAnswer(int data,int dataid);
	int  GetVasLvQuestionAnswer(void);
	int  GetVasLvQuestionAnswerId(void);
	
	void SetVasLvid(int data);
	int  GetVasLvid(void);
	CVasLvData* QueryVaslvData()	{ CHECKF(m_pvaslvdata); return m_pvaslvdata; }
//modify code by 林德渊 end

private:
	CCompensationManager* m_pCompensationManager;
public:
	void initCompensationManager();
	void SendCompentationInfoMsg();
	void SendCompentationInfo();
	bool PickCompentation(OBJID id,bool& bOther);
	///跑商验证码相关 
public:
	//状态
	int  GetVerificationState();
	void SetVerificationState(int nState,bool bUpdate=false);
	//次数
	int  GetVerificationTimes();
	void SetVerificationTimes(int nState,bool bUpdate=false);
	void AddVerificationTimes();
	//buff结束是否需要判断
	void SetVerificationNeedChk(bool bChk){m_Verification_needChk=bChk;}
	bool GetVerificationNeedChk(){return m_Verification_needChk;}
	//是否通过验证
	void SetVerificationPass(bool bPass){m_Verification_pass=bPass;}
	bool GetVerificationPass(){return m_Verification_pass;}

	//执行任务相关
	void VerificationGivepupTask();
	//发送
	bool SendVerification(){return m_pVerification->SendVerification();}
	bool ReSendVerification(){return m_pVerification->ReSendVerification();}
	//踢出玩家
	void KickOutUser(){m_pVerification->KickOutUser();}
	//验证
	bool Verification(string szCode){return m_pVerification->Verification(szCode);}
	//状态查询
	bool CheckVerificationState();
	//是否正在
	bool IsVerificationCoding(){return m_pVerification->IsCoding();}
	void SetVerificationCoding(bool bCoding){m_pVerification->SetCoding(bCoding);}
private:
	CVerification *m_pVerification;///跑商验证
	bool  m_Verification_pass;
	bool  m_Verification_needChk;
	//////////////////////////////////////////////////////////////////////////
//modify code by 林德渊 begin 2011.6.22
public:
	void SetNewValue(int newvalue);
	int GetNewValue(void);
	void SetNewValueTrageid(OBJID newtrageid);
	OBJID GetNewValueTrageid(void);
private:
	int newvalue;
	OBJID newvaluetrageid;
//modify code by 林德渊 end
};
#endif // !defined(AFX_USER_H__BF5C20F1_5DBE_4E65_9099_E96EFAD7ECBA__INCLUDED_1)
