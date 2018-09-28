// User.cpp: implementation of the CUser class.
// 
//////////////////////////////////////////////////////////////////////
#include <afx.h>
#include <afxdisp.h>

#include <math.h>
#include "network/AllMsg.h"
#include "Package.h"
#include "AwardHonor.h"
#include "User.h"
#include "MapGroup.h"
#include "NetWork/NetMsg.h"
#include "ItemType.h"
#include "team.h"
#include "StatusOnce.h"
#include "Transformation.h"
#include "WantedList.h"
#include "PoliceWanted.h"
#include "MapTrap.h"
#include "I_MessagePort.h"
#include "DeadLoop.h"
#include "AutoPtr.h"
#include "../share/I_Shell.h"
#include "math.h"
#include "TaskDetail.h"
#include "EconomySystem.h"
#include "ResourceWar.h"
#include "TopListSystem.h"
#include "SKHonorMurderList.h"//20070213修罗
#include "SKPostOffice.h"
#include "UserMarryRing.h"
#include "DataInspectManage.h"
#include "AdjustRate.h"
#include "SettledAction.h"	//  [2007-8-15] 朱斌
#include "MapGroup.h"
#include "WargameManager.h"///2007-08-25softworms
#include "MapInstanceMgr.h"///2008-01-12softworms-副本管理器
#include "CombGod.h"		//合体系统
#include "GiftManager.h"//20071123修罗:
#include "MsgGift.h"//20071204修罗:
#include "MsgHelpAction.h"
#include "Antiplug.h"
#include "MsgSaveItemView.h"

#include "StockOrder.h"
#include "TitleAttar.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern int g_SpecilLine;
#define TIME_CITYWAR_INTERVAL   3
#define TIME_SAVEITEM_INTERVAL  3*24*3600

//extern long	g_sKickoutCheat;
long	g_sKickoutCheat = 0;

//////////////////////////////////////////////////////////////////////
const int	ADD_ENERGY_STAND_SECS			= 5;			// 每多少秒增加一次
const int	ADD_ENERGY_STAND				= 50;			// 站着增加数量
const int	DEL_ENERGY_PELT_SECS			= 3;			// 疾行状态每3秒减一次
const int	DEL_ENERGY_PELT					= 2;			// 每次扣2点
const int	KEEP_STAND_MS					= 1500;			// 站稳需要的时间，站稳后能挡住玩家。
const int	CRUSH_ENERGY_NEED				= 100;			// 挤开需要多少点
const int	CRUSH_ENERGY_EXPEND				= 100;			// 挤开消耗多少点
const int	SYNWAR_PROFFER_PERCENT			= 1;			// 打柱子得经验值的百分比
const int	SYNWAR_MONEY_PERCENT			= 2;			// 打柱子得钱的百分比
const int	MIN_SUPERMAP_KILLS				= 25;			// 无双列表低限
//const int	NEWBIE_LEVEL					= 70;			// 新手的等级
const int	VETERAN_DIFF_LEVEL				= 20;			// 老手的等级差
//const int	_MIN_TUTOR_LEVEL				= 50;							// 导师最低等级
const int	HIGHEST_WATER_WIZARD_PROF		= 135;			// 水真人
const int	MOUNT_ADD_INTIMACY_SECS			= 3600;			// 
const int	INTIMACY_DEAD_DEC				= 20;			// 
const int	SLOWHEALLIFE_MS					= 1000;							// 吃药补血慢慢补上去的豪秒数。

const int	AUTOHEAL_TIME					= 600;
const int	AUTOHEAL_EACHLIFE				= 6;							// 每次补血点数
const int	AUTOHEAL_EACHMANA				= 3;							// 每次补蓝点数
const int	AUTOHEAL_EACHSP					= 12;							// 每次回SP点数
const int	AUTOHEAL_GENEEACHLIFE			= 9;							//基因体质增加回血,回蓝50%
const int	AUTOHEAL_GENEACHMANA			= 5;							//同上
const int	AUTOHEAL_GENEEACHSP				= 24;							//同上

const int	ITEM_LIFE_TIME_FIRST_CHECK_INTERVAL_SEC	= 30;							//  [2007-10-10] 朱斌	第一次上线检查间隔
const int	ITEM_LIFE_TIME_CHECK_INTERVAL_SEC	= 60 /** 30*/;							//  [2007-9-14] 朱斌	物品生命周期检查间隔 秒
const int	ITEM_LIFE_TIME_UPDATE_INTERVAL  = 1000 * 60 /** 30*/;						//  [2007-9-19] 朱斌	物品生命周期更新客户端间隔

const int	VIP_CHECK_SEC	= 60;							//  [2007-10-10] 朱斌	第一次上线检查间隔
const int   SYN_WORK_SEC	= 15 * 60;
const int	VIP_FACE_SEC	= 60;
const int	ESEPCIAL_DRUG_SEC	=	6;		//特殊的大红大蓝恢复时间间隔
const int	EverdayOL_SEC	= 60;			//每分钟计一次在线总时

const int	UNDATEDATABASE_BASETIME = 300;
const int	UNDATEDATABASE_OFFTIME = 180;

const int	TICK_SECS						= 10;
const int	MAX_PKLIMIT						= 8;						// PK值的最大最小限制(防回卷)  //change huang 2004.1.11
const int	PILEMONEY_CHANGE				= 5000;							// 大量现金变动(需要即时存盘)
const int	_ADDITIONALPOINT_NUM			= 0;							// 升1级加多少属性点//20070126修罗改为1点
const int	PK_DEC_TIME						= 30*60   /*20100730JoyCc：每30分钟掉一点/*20*60*/;//[游途道标 2009.06.03]每6分钟掉一点							//20070228修罗:20分钟 每间隔60秒降低pk值     //change huang 2004.1.11   
//const int	PK_DEC_TIME_PRISON				= 5;
const int	FATIGUE							= 60;
const int   PKVALUE_DEC_ONCE                = 1;                            // 按时间减少PK值。        

const int	PKVALUE_DEC_PERDEATH			= 30;							// 被PK死一次减少的PK值
const int	TIME_TEAMPRC					= 5;							// 队伍信息处理的时间
const int	DURABILITY_DEC_TIME				= 12;							// 每12秒降低一次按时间消耗的装备耐久度
const int	USER_ATTACK_SPEED				= 1000/*800 */;							// 玩家徒手攻击频率
const int	POISONDAMAGE_INTERVAL			= 2;							// 中毒每2秒伤血一次
const int	WARGHOST_CHECK_INTERVAL			= 10;							// 每隔10秒检查一次战魂等级(武器经验)
const int	AUTO_REBORN_SECS				= 30;							// 复活宝石30秒后自动使用
const int	INC_POTENTIAL_SECS				= 6*60;							// 每隔6分钟增加一次潜能
const int	INC_POTENTIAL_NUM				= 1;							// 每次增加1点潜能
const int	ADD_POTENTIAL_RELATIONSHIP		= 6;							// 关系值之和每达到6增加1点潜力值

// add by zlong 2003-11-28
const int	SPRITE_ADD_EXP_SECS				= 60;			//元素精灵每分钟增加一次经验
const int	SPRITE_ADD_EXP					= 1;			//经验增加数量

const int	EUDEMON_ADD_FIDELITY_SECS		= 60*6;			// 幻兽每6分钟增加一次亲密度
const int	EUDEMON_ADD_FIDELITY			= 1;			// 每次增加的亲密度
const int	MAX_EUDEMON_FIDELITY			= 1000;			// 最大亲密度1000
const int	MAX_RETURN_BORN_POS_LEV			= 4;			// 死亡后返回初始出生点的最大等级

//const int	STATUS_NEVERDISAPPER			= 86400;		// 代表永不过时的状态时间

const int	SPIRITPERCENT2GRID				= 10000;			//多少VALUE转换成一个魂格
const int	SPIRITGRID_NUMLIMIT				= 5;			//玩家魂格数量不超过此值


const int	SKZL_ADD_PHYSICALDEFENCE		= 100;			//SKZL状态增加的物理防御点数
const int	SKZL_ADD_MAGICDEFENCE			= 100;			//..............魔法防御点数

const int	FROSTSHIELD_DECSPEEDPERCENT		= 20;			//寒霜甲降低对手移动速度20%		
const int	FROSTSHIELD_DECSPEEDTIME		= 3;			//持续时间秒数
const int	EUDEMON_MIN_MIX_LEV				= 60;

//20070125修罗:属性系统重调整-------------------------------
const double	_GROW_WARRIOR_STR				= 4;			//战士 力量 每级加点
const double	_GROW_WARRIOR_DEX				= 2;			//战士 敏捷 每级加点
const double	_GROW_WARRIOR_VIT				= 3;			//战士 体力 每级加点
const double	_GROW_WARRIOR_SOUL				= 1;			//战士 智力 每级加点
const double	_GROW_WARRIOR_LOVELI			= 0;			//战士 魅力 每级加点

const double	_GROW_ARCHER_STR				= 2;			//弓手 力量 每级加点
const double	_GROW_ARCHER_DEX				= 4;			//弓手 敏捷 每级加点
const double	_GROW_ARCHER_VIT				= 3;			//弓手 体力 每级加点
const double	_GROW_ARCHER_SOUL				= 1;			//弓手 智力 每级加点
const double	_GROW_ARCHER_LOVELI				= 0;			//弓手 魅力 每级加点

const double	_GROW_MAGE_STR					= 2;			//法师 力量 每级加点
const double	_GROW_MAGE_DEX					= 1;			//法师 敏捷 每级加点
const double	_GROW_MAGE_VIT					= 3;			//法师 体力 每级加点
const double	_GROW_MAGE_SOUL					= 4;			//法师 智力 每级加点
const double	_GROW_MAGE_LOVELI				= 0;			//法师 魅力 每级加点
//属性系统重调整end---//彭峰2007/02/02--------------------------------------

//20070314修罗:
//最高阶位
const int _MAX_RANK_LEV = 10;
//阶位对应军阶点
const int _RANK_BREAKPT[_MAX_RANK_LEV]={750,2250,3000,7500,15000,30000,45000,60000,75000,90000};

//const int _ANTIPLUG_QUESTION_NUM = 5;
const int _ANTIPLUG_INTERVAL_TIME = 100;
const int _ANTIPLUG_DEFAULT_REPLY = 60;
const int _ANTIPLUG_MAX_TIME = 25*60;
const int _ANTIPLUG_MIN_TIME = 5*60;
const int _ANTIPLUG_MIN_MONSTERNUM = 50;
const int _ANTIPLUG_MAX_MONSTERNUM = 150;


//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CUser,s_heap)
char	szActionTable[]			= _TBL_ACTION;
char	szMonsterTypeTable[]	= _TBL_MONSTERTYPE;
char	szEudemonTypeTable[]	= _TBL_EUDEMONTYPE;
char	szPeifangTypeTable[]	= _TBL_PEIFANGTYPE;

////////////////////////06.3.23王玉波添加//////////////////////////////
enum
{
	FORCE,	//力量
	DEX,	//敏捷
	SOUL,	//智力
	HEALTH,	//体质
};

int warAttBase[]	=	{ 5,2,1,3 };
int bowAttBase[]	=	{ 1,2,6,2 };
int encAttBase[]	=	{ 2,5,2,2 };
/////////////////////////////////////////////////////////////////////
//这里人物/宠物换皮相关
const int DressNewCloth		= 1;
const int EudemonNewCloth	= 2;


/////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction CHEAT
//////////////////////////////////////////////////////////////////////
enum
{
	EUDEMONMIX_id = 0,
	EUDEMONMIX_firstid,
	EUDEMONMIX_nexthabit,
	EUDEMONMIX_hptime,
	EUDEMONMIX_phytime,
	EUDEMONMIX_magtime,
	EUDEMONMIX_deftime,
	EUDEMONMIX_resulthp,
	EUDEMONMIX_resultphy,
	EUDEMONMIX_resultmag,
	EUDEMONMIX_resultdef,
	EUDEMONMIX_resultno,
};
//新角色分配出生地图
bool MoreUserThan(MapUserAmount stmap1, MapUserAmount stmap2)
{
	return stmap1.dwAmount < stmap2.dwAmount;
}
bool CUser::SetCheatLevel(int nLevel)
{
#if	!defined(_DEBUG)
	if(IsGM() || GetLev() < LOGIN_FREE_LEVEL)
		return false;
#endif

	// init count
	if(m_nCheatLevel == 0)
		m_nCheatCount = UserManager()->GetCheatCount(GetID());


	// is max?
	if(nLevel <= m_nCheatLevel)
		return false;

	if(!IsCheater(_TYPE_MAGIC))
	{
		m_nCheatLevel	= nLevel;
		if(m_nCheatLevel + m_nCheatCount*10 >= 30)
		{
			SetCheatMark(_TYPE_MAGIC, 0);

			// add to manager
			int nAmount, nTimes;
			UserManager()->AddCheat(GetID(), &nAmount, &nTimes);
			LOGCHEAT("magic: user{%s}, account[%u], type[zffy], level[%d], amount[%d], times[%d]", GetName(), GetAccountID(), nLevel, nAmount, nTimes);
			return true;
		}
	}
	else
	{
		m_nCheatLevel	= nLevel;
		LOGCHEAT("magic: user{%s}, account[%u], type[zffy], level[%d]", GetName(), GetAccountID(), nLevel);
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsCheater(OBJID idKind)
{
	CHECKF(idKind < 32);

	return (m_maskCheat & (1<<idKind)) != 0;
}

char* g_setName[] = { "_TYPE_MAGIC", "_TYPE_WS", "_TYPE_FY", "_TYPE_USE_LIFE", "_TYPE_USE_MANA", 
		"null", "null", "null", "null", "null", "null", "null", "null", "null", "null", 
		"null", "null", "null", "null", "null", "null", "null", "null", "null", "null", 
		"null", "null", "null", "null", "null", "null", "null", "null", "null", "null",  };
//////////////////////////////////////////////////////////////////////
void CUser::SetCheatMark(OBJID idCheatKind, DWORD dwCheatMark)
{
	CHECK(idCheatKind < 32);

	LOGCHEAT("client: user{%s}, account[%u], type{%s}, mark[%d]", GetName(), GetAccountID(), g_setName[idCheatKind], dwCheatMark);

	m_maskCheat = m_maskCheat | (1<<idCheatKind);
}

//////////////////////////////////////////////////////////////////////
void CUser::KickoutCheat(OBJID idKind)
{
	CHECK(idKind < 32);
	CHECK(IsCheater(idKind));

#if	!defined(_DEBUG)
//	if(UserManager()->GetCheatAmount() < 5)
//		return ;
#endif

	// kickout
	if(GetLev() >= LOGIN_FREE_LEVEL && (g_sKickoutCheat&(1<<idKind)) != 0 )
	{
#if	defined(_DEBUG)
		SendSysMsg("★CHEAT★");
#endif
		UserManager()->KickOutSocket(m_idSocket, "cheat");
	}
	else
	{
		LOGCHEAT("need kick: user{%s}, account[%u], type{%s}", GetName(), GetAccountID(), g_setName[idKind]);
	}
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsMagicAtkCheat (OBJID idTarget, int nTargetX, int nTargetY, DWORD dwTimeStamp)
{
	IRole* pRole = RoleManager()->QueryRole(idTarget);
	if (!pRole)
		return false;

	// action mode test
	{
		if (dwTimeStamp-m_dwLastJump < 100)
		{
			//::MyLogSave("gmlog/test", "---------suspect action found!");
			return true;
		}
	}

	// interval test
	CONST TEST_SIZE = 8;
	m_setActionTime.push_back(dwTimeStamp);

	if (m_setActionTime.size() > TEST_SIZE)
		m_setActionTime.pop_front();

	if (m_setActionTime.size() != TEST_SIZE)
		return false;

	{
		deque<int> setDeltaTime;
		for (int i=1; i<TEST_SIZE; i++)
			setDeltaTime.push_back(m_setActionTime[i]-m_setActionTime[i-1]);		

		CONST SUSPECT_TIME = 20;

		int nSuspectCount = 0;
		for (i=1; i<TEST_SIZE-1; i++)
		{
			if (abs(setDeltaTime[0] - setDeltaTime[i]) <= SUSPECT_TIME)
				nSuspectCount++;
		}

		if (nSuspectCount >= TEST_SIZE-2)
		{
			//::MyLogSave("gmlog/test", "---------repeat action found!");
			return true;
		}
	}

	// target pos test
	if (nTargetX == 0 && nTargetY == 0)
		return true;

	// dislocation test
	// ....
	return false;	
}

//////////////////////////////////////////////////////////////////////
void CUser::DoCheaterPunish	(int nData/* = 0*/)
{
	CONST OBJID idMapCheaterPrison = 6010;
	CONST int nPosX = 34, nPosY = 74;

	if (this->GetMapID() == idMapCheaterPrison)
		return;

	this->SetRecordPos(idMapCheaterPrison, nPosX, nPosY);
	this->FlyMap(idMapCheaterPrison, nPosX, nPosY);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUser::CUser()
{
	m_pCompensationManager = NULL;
	m_pMonster = NULL;//[游途道标 2009.04.03]
	m_idItem = NULL;//[游途道标 2009.05.22]
	SetObjType(OBJ_USER);

	m_link.Init(this);
	m_nStep			= STEP_LOGIN;		// 地图组首先是进入地图

	m_pShoulder = NULL;
	m_pArmor = NULL;
	m_pHelmet = NULL;
	m_pWeaponR = NULL;
	m_pNecklace = NULL;
	m_pBangle = NULL;
	m_pGlove = NULL;
	m_pCaestus = NULL;
	m_pShoes = NULL;
	m_pMount = NULL;
	m_pRingT = NULL;	
	m_pRingB = NULL;
	m_pAdornT = NULL;	
	m_pAdornB = NULL;
	m_pDress = NULL;
	m_pWeaponSoul = NULL;

	m_hpState		 = -1;
	m_mpState		 = -1;
	

	m_bUpdateWeight	= true;		

	m_pTeam			= NULL;

	// tick -----------
	m_dwFirstServerTick	= 0;
	m_dwLastServerTick	= 0;

	m_dwLastClientTick	= 0;
	m_dwFirstClientTick	= 0;
	m_dwLastCoolShow	= 0;	

	m_dwLastRcvClientTick = 0;

	// interface
	m_ifaceSquareDeal.m_pParent	= this;

	m_pBattleSystem	= NULL;
	m_pStorage		= NULL;
	m_idTaskNpc		= ID_NONE;
	m_idTaskItem	= ID_NONE;
	m_idTaskItemAction	= ID_NONE;
	m_bDebugAction	= false;
	m_pPackage		= CUserPackage::CreateNew();
//	m_pMagicStoneBox = CUserPackage::CreateNew();

	m_pMagic		= NULL;
	m_pSyn			= NULL;
	m_bGhost		= false;
	
	m_bHaveLeaveWord	= true;
	m_pTransformation	= NULL;

//	m_bSynchro		= true;
	m_pBooth		= NULL;
	m_idxLastArrow	= -1;		// error

	// foot print
	m_tFootPrint	= 0;
	m_posFootPrint.x	= 0;
	m_posFootPrint.y	= 0;

	// superman list
	m_nCurrSupermanKills	= 0;
	m_nTopSupermanKills		= 0;
	m_nSupermanOrder		= 0;

	// cheat test
	m_dwLastJump = 0;
	m_dwMsgCount = 0;

	// tutor
	m_pUserTutor = NULL;
// 	m_pTutor	= NULL;
// 	m_dwExpToTutor	= 0;
	
	m_nHasLock = 0;
	// mine
//	m_pMineTarget	= NULL;

	//帮派服装的armor type;
	m_nSynDressArmorType = 0;
	m_pTaskDetail	= NULL;
	m_bIsEnableWarGhostExp = false;

	//06.3.21王玉波
// 	m_bWarSkill = false;
// 	m_WarSkillID = 0;

	m_bIsAllowTeam	=	true;
	m_bIsAllowBuy	=	true;
	m_bIsAllowFriend=	true;
	m_bIsAllowLook	=	true;
	m_bResetDarTimeSet = false;

	//--------------------------------------
	m_bHide = false;
	m_setItemDelay.clear();
	//20070403修罗
	//applay
	m_nApply = 0;
	m_idApplyTarget = 0;

	//  [2007-9-29] 朱斌	补上初始化
	m_nApplyByName = 0;
	memset(m_szApplyName, 0, _MAX_NAMESIZE);

	//识破状态相关
	m_bIgnoreTargetDodge= false;
	m_bMustCritHit		= false;
	m_targetID			= NULL;
	//-----------------------end--------------
	m_bRebornFlag		=	false;
	m_nRecoverPercent	= 0;

	m_nFightRate = USER_ATTACK_SPEED;//20070425修罗:默认值

	m_bFirstCheckItemLifeTime = TRUE;

	m_dwVas = 0;//初始化增值币
	m_dwVasJuan = 0;

	//锁定目标
	m_IdLockTarget = 0;
// 	ClearHouseItem();

	//反外挂杀怪数量
	m_KillMonsterNum = 0;
	
	m_killMonsterTargetNum = 0;       //杀怪的目标数量(读取数据库)
	m_killMonsterTimeInterval = 0; //杀怪的时间间隔(读取数据库)
	
	bMateTeam=false;

	m_nBabiID = 0;

    m_nSearchRoleTime = 0;
	m_nSearchTeamTime = 0;
	m_nSearchHouseTime = 0;

	m_dNpcID = 0;

	m_bAutoFightFlag = false;
	m_pUserPet = NULL;
	m_pUserPeifang = NULL;
	m_pWarPet = NULL;
	m_IsOnMount = false;
	m_dwLastVipCheckSendTime = 0;
	m_lastEudemonLifespanCheck = clock();
	m_lastSoulEudemonLifeSpan = clock();
	m_bSee = false;
	m_lastMountTime = 0;
	m_bHasTempFriend = false;//是否有临时好友，如果没有，下线时就不用执行删除
	m_GoldPoint = 0;

	m_doubletime=0;
	m_bInstanceReborn=false;

	//goto 新的银两和经验存储机制用到的增量
	m_MoneyIncr = 0;
	m_ExpIncr = 0;

	//goto 活动奖励
	m_nActivityAward = 0;
	m_ChargePack1=0;
	m_ChargePack2=0;
	m_ChargePack3=0;
	m_pTitle = NULL;
	m_pTreasure = NULL;
	m_pOnlineGift = NULL;
	m_nSendLeaveWord = 0;//goto 用于留言删除

	m_nConMoney = 0;
	m_nConBuild = 0;
	m_nLevExp = -1;

	m_nTutorQuestion = 0;//师徒答题题号
	m_nTutorAnswer = 0;//师徒答题答案

	m_pPetEgg = NULL;
	//专线相关
	m_pSpecilLineInfo = NULL;
	m_bChangeLine=false;
	
	m_pUserWarGame = NULL;
//modify code by 林德渊 begin 2011.5.30-
	m_pvaslvdata = NULL;
//modify code by 林德渊 end
	m_pVerification = NULL;
	m_Verification_pass=false;
	m_bLoaded = false;
	m_Verification_needChk=false;
	m_idSoulEudemon = ID_NONE;

	m_eudemonSoulphyatt = 0;//外攻
	m_eudemonSoulmagatt = 0;//内攻
	m_eudemonSoul1 = 0;//火
	m_eudemonSoul2 = 0;//冰
	m_eudemonSoul3 = 0;//玄
	m_eudemonSoul4 = 0;//毒
	m_eudemonSoulAddHp = 0;//回血
	m_eudemonSoulAddMp = 0;//回蓝
	m_eudemonSoulAddDodge = 0;//加闪避buff
	m_eudemonSoulAddHitRate = 0;//加命中buff
	//用于宠物回血技能的CD
	m_tLastAddHp = 0;
	m_tLastAddMp = 0;
	m_bSoulEudemonView = false;
//modify code by 林德渊 begin 2011.6.22
	newvalue = 0;
	newvaluetrageid = 0;
//modify code by 林德渊 end
}	
//////////////////////////////////////////////////////////////////////
CUser::~CUser()
{
	DEBUG_TRY
	LeaveMapGroup();
	DEBUG_CATCH("~user error LeaveMapGroup();");

	DEBUG_TRY
	if(QueryBooth())
		this->DestroyBooth();
	DEBUG_CATCH("~user error DestroyBooth();");

	DEBUG_TRY
	if (m_pBattleSystem)
	{
		delete m_pBattleSystem;
		m_pBattleSystem	= NULL;
	}
	DEBUG_CATCH("delete m_pBattleSystem");

	DEBUG_TRY
	if(m_pStorage)
		m_pStorage->Release();
	DEBUG_CATCH("~user error m_pStorage->Release()");

	DEBUG_TRY
	if (m_pPackage)
		m_pPackage->Release();
	DEBUG_CATCH("m_pPackage->Release()");

	DEBUG_TRY
	if (m_pMagic)
		m_pMagic->ReleaseByOwner();
	DEBUG_CATCH("m_pMagic->ReleaseByOwner()");

	DEBUG_TRY
	if (m_pSyn)
		m_pSyn->ReleaseByOwner();
	DEBUG_CATCH("~user error m_pSyn->ReleaseByOwner();");
 
	SAFE_RELEASE (m_setStatus);	

	DEBUG_TRY
 	if(m_pTaskDetail)
 		m_pTaskDetail->Release();
	DEBUG_CATCH("m_pTaskDetail->Release()");

	DEBUG_TRY
	ResetOftenUseGemAddition();
	DEBUG_CATCH("~user error ResetOftenUseGemAddition();");

	DEBUG_TRY
	if(m_pUserPet)
	{
		m_pUserPet->Release();
		m_pUserPet = NULL;
	}

	if(m_pWarPet)
	{
		m_pWarPet->DelMonster(true);
		m_pWarPet = NULL;
	}

	if(m_pUserPeifang)
	{
		m_pUserPeifang->Release();
		m_pUserPeifang = NULL;
	}
	DEBUG_CATCH("~user error m_pWarPet->DelMonster(true)");

	DEBUG_TRY
	if (m_pTitle)
		m_pTitle->ReleaseByOwner();
	DEBUG_CATCH("m_pTitle->ReleaseByOwner()")

	DEBUG_TRY
	if (m_pTreasure)
		m_pTreasure->ReleaseByOwner();
	DEBUG_CATCH("m_pTreasure->ReleaseByOwner()")

	DEBUG_TRY
	if (m_pOnlineGift)
		m_pOnlineGift->ReleaseByOwner();
	DEBUG_CATCH("m_pOnlineGift->ReleaseByOwner()")

	DEBUG_TRY
	if (m_pUserTutor)
		m_pUserTutor->ReleaseByOwner();
	DEBUG_CATCH("m_pUserTutor->ReleaseByOwner()")
// 	if(m_pAutoFightData)
// 		m_pAutoFightData->ReleaseByOwner();
	DEBUG_TRY
	if(m_pPetEgg)
		m_pPetEgg->ReleaseByOwner();
	DEBUG_CATCH("~user error m_pPetEgg->ReleaseByOwner()");
	DEBUG_TRY
	if(m_pUserWarGame)
		m_pUserWarGame->ReleaseByOwner();
	DEBUG_CATCH("~user error m_pUserWarGame->ReleaseByOwner()");
	DEBUG_TRY
	if(m_pCompensationManager)
		m_pCompensationManager->Release();
	DEBUG_CATCH("~user error m_pCompensationManager->ReleaseByOwner()");
	DEBUG_TRY
	if(m_pSpecilLineInfo)
		m_pSpecilLineInfo->ReleaseByOwner();
	DEBUG_CATCH("~user error m_pSpecilLineInfo->ReleaseByOwner()");
//modify code by 林德渊 begin 2011.5.30-
	DEBUG_TRY
	if(m_pvaslvdata)
		m_pvaslvdata->ReleaseByOwner();
	DEBUG_CATCH("~user error m_pvaslvData->ReleaseByOwner()");
//modify code by 林德渊 end
	DEBUG_TRY
		if(m_pVerification)
			m_pVerification->ReleaseByOwner();
	DEBUG_CATCH("~user error m_pVerification->ReleaseByOwner()");
	
}

//////////////////////////////////////////////////////////////////////
CUser* CUser::CreateNewUser(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo, ULONG nIP)	// join from other map_group
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess));
	CHECKF(pInfo);

	CUserPtr pUser = CUser::CreateNew();
	CHECKF(pUser);
	if(pUser)
	{
		pUser->m_idProcess	= idProcess;
		pUser->m_idSocket	= idSocket;
		pUser->m_nIPAddress = nIP;
		memcpy(&pUser->m_data.m_Info, pInfo, sizeof(UserInfoStruct));
		pUser->m_data.m_pRes = MapGroup(idProcess)->QueryDataDefault()->GetUserData()->CreateNewRecord(pInfo->id);
		if(!pUser->m_data.m_pRes)
		{
			pUser->ReleaseByOwner();
			return NULL;
		}
	}

	pUser->InitChangeMapGroup();

	return pUser;
}

///判断玩家当前是否在默认登录点上
// bool CUser::IsDefaultLoginSite()
// {
// 	return GetMapID()==DEFAULT_LOGIN_MAPID && GetPosX()==DEFAULT_LOGIN_POSX && GetPosY()==DEFAULT_LOGIN_POSY;
// }

//////////////////////////////////////////////////////////////////////
// login, return false: logout_user
bool CUser::EnterMap()
{
	ClearWalkData();

	int nStep = 0;
	DEBUG_TRY
	if(m_nStep != STEP_LOGIN)
		return false;

	nStep = 1;
	CMapPtr pMap = GetMap();
	if(pMap)
	{
		m_bSee = true;
		
		{
			TimeCostChk("Timecheck","ClrBroadcastSet()");
			nStep = 2;
			ClrBroadcastSet();
			nStep = 3;
			UpdateBroadcastSet();
		}

		{
			TimeCostChk("Timecheck","EnterRoom()");
			nStep = 4;
			pMap->EnterRoom(this->QueryMapThing(), WITH_BLOCK);
			nStep = 5;
			//goto 更新地图玩家人数
			pMap->AddUserAmount(1);
		}

		{
			TimeCostChk("Timecheck","GetTeam()");
			nStep = 6;
			//		pMap->SendMapInfo(this);
			if(GetTeam())
			{
				CMsgPlayer msgPlayer;
				IF_OK(msgPlayer.Create(QueryRole()))
					SendMsg(&msgPlayer);
				// 			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
			}
		}
// 		if(!pMap->IsRecordDisable())
// 		{
// 			SetRecordPos(GetMapID(), GetPosX(), GetPosY(),true);	
// 		}
		nStep = 7;
	}

	{
		TimeCostChk("Timecheck","AttachStatus()");
		CRole::AttachStatus(this->QueryRole(), STATUS_PK_PROTECT, 0, CHGMAP_LOCK_SECS);
	}

	nStep = 8;

	m_nStep = STEP_ENTERMAP;

// 	if(m_idEudemonCurForChangeMap != ID_NONE)
// 		CallEudemon(m_idEudemonCurForChangeMap,false,false);

//  [2007-8-15] 朱斌 C冯翔_20062007081101_切换地图组触发action策划案
// 	ST_SETTLEDACTIONINFO* pInfoTemp = g_settledAction.GetSettledActionInfoByType(EM_SETLDACTN_TYPE_ENTER_MAP);
// 	if(pInfoTemp && pInfoTemp->actionid)
// 	{
// 		GameAction()->ProcessAction(pInfoTemp->actionid, this);
// 	}
//	SendAllMagicInfo();//[2010-06-28 goto]同步技能列表
	
	nStep = 9;
	// satan。。。。。。。。。。。。。。

	if(m_idNowEudemon != ID_NONE)
	{
		TimeCostChk("Timecheck","CallEudemon()");
		nStep = 10;
		CallEudemon(m_idNowEudemon,false,false);
		m_idNowEudemon = ID_NONE;
		RestoreEudemonStatus();
	}

	if(m_idSoulEudemon != ID_NONE)
	{
		if(!QueryStatus(STATUS_EUDEMON_SOUL))
		{
			OBJID idEudemon = m_idSoulEudemon;
			m_idSoulEudemon = ID_NONE;
			EudemonSoul(idEudemon);
		}
	}

	if(GetMap())
	{
		///如果进入地图的是队主则改变队内地图id
// 		if(GetTeam() && GetTeam()->GetLeader()==GetID())
// 		{
// 			GetTeam()->SetTeamMap(GetMap()->GetID(),true);
// 		}

		///如果记录的地图id不同,则尝试飞到副本
// 		if(GetTeam() && GetRecordMapID()>1000000 && GetRecordMapID()==GetTeam()->GetTeamMap() && IsDefaultLoginSite())
// 		{
// 			FlyMap(GetRecordMapID(),GetRecordPosX(),GetRecordPosY());
// 		}
// 		DEBUG_TRY
// 		TeamMemberInfo menberInfo;
// 		if(GetTeam())
// 		{	
// 			nStep = 11;
// 			GetTeam()->GetMemberInfo(menberInfo,GetID());
// 			if(menberInfo.idMap && GetMap()->IsDynaMap()==false)
// 			{
// //				EnterInstance(5,0,1,62,24)
// //				FlyMap(menberInfo.idMap,menberInfo.usPosX,menberInfo.usPosY);
// 				nStep = 12;
// 				m_bInstanceReborn=true;///进副本后不再调用初进脚本
// 				RequestEnterInstance(menberInfo.idMap,0,1,menberInfo.usPosX,menberInfo.usPosY);
// 				nStep = 13;
// 				GetTeam()->SetLocation(GetID(),0,0,0);///清空
// //				menberInfo.idMap=menberInfo.usPosX=menberInfo.usPosY=0;
// 			}
// 		}
// 		DEBUG_CATCH2("CUser::EnterMap 上线进副本流程崩溃了!nStep:%d",nStep);
		nStep = 14;

		///副本地图在进入的时候不保存坐标,只有在离线时才保存
		TimeCostChk("Timecheck","SendPosToTeamMem()");
 		if(GetMap()->IsDynaMap()==false)//专线帮派也是
			SetRecordPos(GetMapID(), GetPosX(), GetPosY());
		else
		{
			if(m_idProcess == MSGPORT_MAPGROUP_INSTANCE)
			{
				MapInstanceMgr()->EnterScript(GetMap()->GetID(),GetID());
			}
		}

		nStep = 15;
		OBJID idMapDoc = GetMap()->GetDocID();
		//goto 通知队友
		SendPosToTeamMem(idMapDoc, GetPosX(), GetPosY());

		//VIP列表
		nStep = 16;
		if(IsVipActive())
		{
			g_VipListMgr.AddVipUser(this);
			//广播到其他线路
			CMsgVipList msg;
			if (msg.Create(GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_ONLINE))
			{
				msg.Append(GetID(), GetMap()->GetDocID(), GetLev(), CVipList::GetLineByName(), GetVipLev(), GetName());
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
			}
		}

		//竞技场阵营
		if(g_SpecilLine==1)
		{
			if(GetMap()->IsPkGameMap())
			{
				IStatus* pStatus = QueryStatus(STATUS_WARGAME);
				if(pStatus)
				{
					CMsgUserAttrib msg;
					if(msg.Create(GetID(), _USERATTRIB_WARGAME_CAMP, pStatus->GetParam1()))
						SendMsg(&msg);
				}
				else if(CWargame::IsWarGameMap(GetMapID()))
				{
					//发现一个不在名单内却进了竞技场的人
					::LogSave("User enter wargame without signin up!userid=%u,name=%s", GetID(), GetName());
					//通知客户端切线
					SetSpecilSteate(STATE_SPECILLINE_OUT);
					ChangeLineMap(GetRecordMapID(), GetRecordPosX(), GetRecordPosY(), RANDLINE);
				}
			}
		}
	}
	SendFatigue();
	DEBUG_CATCH2("CUser::EnterMap() error at %u",nStep);
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendSelfToBlock()
{
	// brocast my info
	CMsgPlayer msg;
	if (msg.Create((IRole*)this))
		this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);

	return true;
}

//////////////////////////////////////////////////////////////////////
// modify attrib
//////////////////////////////////////////////////////////////////////
bool CUser::AddAttrib(int idxAttr, __int64 i64Data, int nSynchro )
{
	CMsgUserAttrib	msg;
	IF_NOT (msg.Create(GetID(), _USERATTRIB_NONE, 0))			// _USERATTRIB_NONE : 不添加
		return false;

	if(i64Data)
	{
		switch(idxAttr)
		{

		case	_USERATTRIB_STORAGELIMIT:
			{
				IF_NOT (i64Data >= 0)
					return false;
				int nOldStorageLimit = GetStorageLimit();
				int nNewStorageLimit = __min(_MAX_STORAGE_TYPE, nOldStorageLimit+i64Data*18);
// 				m_data.SetStorageLimit(nNewStorageLimit, true);
// 				IF_NOT (msg.Append(_USERATTRIB_STORAGELIMIT, GetStorageLimit()))
// 					return false;

//				// 等级在70级上下发生变化，需要调整经验值存储格式
//				if (nNewLev > ADJUST_EXPFORMAT_LEVEL && nOldLev <= ADJUST_EXPFORMAT_LEVEL)
//					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()/10, nSynchro);
//				else if (nNewLev <= ADJUST_EXPFORMAT_LEVEL && nOldLev > ADJUST_EXPFORMAT_LEVEL)
//					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()*10, nSynchro);
			}
			break;
		//--------------------独孤求败[08.9.27]添加------------------//
		case _USERATTRIB_PACK_OPENEDROW:
			{
// 				IF_NOT (i64Data >= 0)
// 					return false;
// 
// 				int nOldPackOpenRow = GetPackOpenRow();
// 				int nNewPackOpenRow = __min(18,nOldPackOpenRow+i64Data);
// 				m_data.SetPackRow(nNewPackOpenRow,true);
// 				IF_NOT (msg.Append(_USERATTRIB_PACK_OPENEDROW, GetPackOpenRow()))
// 					return false;
// 				if (nNewPackOpenRow > 6 && nNewPackOpenRow <= 12)
// 					m_data.SetPackCount(2);
// 				if (nNewPackOpenRow > 12)
// 					m_data.SetPackCount(3);
// 				IF_NOT (msg.Append(_USERATTRIB_PACKCOUNT, GetPackSize()))
// 					return false;
			}
			break;
		//-------------------------------------------------------//
		case	_USERATTRIB_PACKCOUNT:
			{
// 				IF_NOT (i64Data >= 0)
// 					return false;
// 
// 				int nOldPackSize = GetPackSize();
// 				int nNewPackSize = __min(_MAX_PACK_TYPE - 1,nOldPackSize+i64Data);//_MAX_PACK_TYPE - 1,最多4个......
// 				m_data.SetPackCount(nNewPackSize, true);
// 				IF_NOT (msg.Append(_USERATTRIB_PACKCOUNT, GetPackSize()))
// 					return false;

//				// 等级在70级上下发生变化，需要调整经验值存储格式
//				if (nNewLev > ADJUST_EXPFORMAT_LEVEL && nOldLev <= ADJUST_EXPFORMAT_LEVEL)
//					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()/10, nSynchro);
//				else if (nNewLev <= ADJUST_EXPFORMAT_LEVEL && nOldLev > ADJUST_EXPFORMAT_LEVEL)
//					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()*10, nSynchro);
			}
			break;

		case	_USERATTRIB_LEV:
			{
				IF_NOT (i64Data >= 0)
					return false;

				int nOldLev = this->GetLev();
				//20090612haocq更新升级时间
// 				if (nOldLev < MAX_USERLEV && i64Data > 0)
// 				{
// 					m_data.SetUpLevelTime(time(NULL));
// 					IF_NOT (msg.Append(_USERATTRIB_UPLEVEL_TIME, GetUpLevelTime()))
// 						return false;
// 				}
				int nNewLev = __min(MAX_USERLEV, nOldLev+i64Data);
				m_data.SetLev(nNewLev);
				IF_NOT (msg.Append(_USERATTRIB_LEV, this->GetLev()))
					return false;

				// 等级在70级上下发生变化，需要调整经验值存储格式
				if (nNewLev > ADJUST_EXPFORMAT_LEVEL && nOldLev <= ADJUST_EXPFORMAT_LEVEL)
					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()/10, nSynchro);
				else if (nNewLev <= ADJUST_EXPFORMAT_LEVEL && nOldLev > ADJUST_EXPFORMAT_LEVEL)
					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()*10, nSynchro);

				//调整玩家的天赋点数
				/*
				int nOldGiftPoint = m_data.GetGiftPoint();
				int nNewGiftPoint = nOldGiftPoint+i64Data;
				m_data.SetGiftPoint(nNewGiftPoint,true);
				if(!msg.Append(_USERATTRIB_GIFT_GIFTPOINT,m_data.GetGiftPoint()))
					return false;
					*/
			}
			break;

		case	_USERATTRIB_LIFE:
			{
				if ((int)this->GetLife() + i64Data <= 0)
				{
					//this->SetStatus(STATUS_DIE);
					SetLife(0);
				}
				else
				{
					SetLife(__min(this->GetMaxLife(), this->GetLife() + i64Data));
				}

				IF_NOT (msg.Append(_USERATTRIB_LIFE, this->GetLife()))
					return false;
				//王玉波暂时添加此处,因为自动加血,没有广播血量
				if( this->GetTeam() )//有组队
				{
					this->BroadcastTeamLife();	//队伍广播血量
				}
			}
			break;
		case	_USERATTRIB_MAXLIFEPERCENT:
			{
			}
			break;
		case	_USERATTRIB_MANA:
			{

				if ((int)this->GetMana() + i64Data <= 0)
					m_data.SetMana(0);
				else
					m_data.SetMana(__min(this->GetMaxMana(), this->GetMana() + i64Data));

				IF_NOT (msg.Append(_USERATTRIB_MANA, this->GetMana()))
					return false;
				if( this->GetTeam() )//有组队
				{
					this->BroadcastTeamMana();	//队伍广播血量
				}
			}
			break;
		case	_USERATTRIB_EXP:
			{
				//goto 经验增量
				if (!IsInMapInstance())//副本中不计，因为进出副本都会保存玩家信息，而且祭坛经验太多，防止祭坛时间数据库压力过大
					m_ExpIncr += i64Data;
				_int64 i64Exp=this->GetExp() + i64Data;
				if(i64Data < 0)
				{
					__int64	i64NewExp = CheckExp(i64Data);
					m_data.SetExp(i64NewExp);
				}
				else
				{
					m_data.SetExp(i64Exp);
				}
				IF_NOT (msg.Append(_USERATTRIB_EXP, m_data.GetExp()))
					return false;
			}
			break;
		case	_USERATTRIB_PK:
			{
				// pk allow negative
				m_data.SetPk(this->GetPk() + i64Data);
				IF_NOT (msg.Append(_USERATTRIB_PK, this->GetPk()))
					return false;
			}
			break;			
		case	_USERATTRIB_MONEY:
			{
				if (isFiftyP())
				{
					i64Data = i64Data/2;
				}
				else if(isZeroP())
				{
					i64Data = 0;
					break;
				}
				if (i64Data < 0 && (int)this->GetMoneyByType(1) + i64Data < 0)
				{
					return false;
				}
				else
					m_data.SetMoney(GetMoneyByType(1) + i64Data,1);

				SendSysMsg("您获得了%u银两",i64Data);
				IF_NOT (msg.Append(_USERATTRIB_MONEY, this->GetMoneyByType(1)))
					return false;
			}
			break;
		case	_USERATTRIB_MONEY2:
			{

				if (isFiftyP())
				{
					i64Data = i64Data/2;
				}
				else if(isZeroP())
				{
					i64Data = 0;
					break;
				}

				
				if (i64Data < 0 && (int)this->GetMoneyByType(2) + i64Data < 0)
				{
					return false;
				}
				else
					m_data.SetMoney(GetMoneyByType(2) + i64Data,2);
				
				SendSysMsg("您获得了%u碎银",i64Data);
				IF_NOT (msg.Append(_USERATTRIB_MONEY2, this->GetMoneyByType(2)))
					return false;
			}
			break;
		case	_USERATTRIB_MONEY3:
			{
				if (i64Data < 0 && (int)this->GetMoneyByType(3) + i64Data < 0)
				{
					return false;
				}
				else
					m_data.SetMoney(GetMoneyByType(3) + i64Data,3);
				
				IF_NOT (msg.Append(_USERATTRIB_MONEY3, this->GetMoneyByType(3)))
					return false;
			}
			break;
		case	_USERATTRIB_MONEY4:
			{
				if (i64Data < 0 && (int)this->GetMoneyByType(4) + i64Data < 0)
				{
					return false;
				}
				else
					m_data.SetMoney(GetMoneyByType(4) + i64Data,4);
				
				IF_NOT (msg.Append(_USERATTRIB_MONEY4, this->GetMoneyByType(4)))
					return false;
			}
			break;
		case _USERATTRIB_ADDPOINT:
			{
				if (i64Data < 0)
				{
					return false;	// 断言错。
				}
// 				m_data.SetAddPoint(m_data.GetAddPoint()+i64Data);
// 				IF_NOT (msg.Append(_USERATTRIB_ADDPOINT, this->GetAddPoint()))
// 					return false;
			}
			break;

		case _USERATTRIB_SPEED:
			{
				m_data.SetSpeed(__max(0, m_data.GetSpeed()+i64Data));
				IF_NOT (msg.Append(_USERATTRIB_SPEED, AdjustSpeed(this->GetSpeed())))
					return false;
			}
			break;
	
		case _USERATTRIB_POTENTIAL:
			//王玉波注释，潜能..
		/*	{
				m_data.SetPotential(__max(0, (int)this->GetPotential()+i64Data));
				IF_NOT (msg.Append(_USERATTRIB_POTENTIAL, this->GetPotential()))
					return false;
			}*/
			break;
		case _USERATTRIB_SP:
			{
				if((int)i64Data != 0)
				{
					int sp = GetSp() + i64Data ;
					int nMaxSp = this->GetMaxSp();
					if( sp < 0 )
						sp = 0;
					else if( sp > nMaxSp )
						sp = nMaxSp;
					SetSp( sp );
					IF_NOT (msg.Append(_USERATTRIB_SP, sp))
						return false;
				}
			}
			break;
//		case _USERATTRIB_ENERGY:
//			{
//				AddEnergy(i64Data);
//				IF_NOT (msg.Append(_USERATTRIB_ENERGY, this->GetEnergy()))
//					return false;
//#ifdef	PALED_DEBUG
//				SendSysMsg("ENERGY: [%d]", GetEnergy());
//#endif
//			}
//			break;
//		case _USERATTRIB_MAXENERGY:
//			{
//				// no operation
//				IF_NOT (msg.Append(_USERATTRIB_MAXENERGY, this->GetMaxEnergy()))
//					return false;
//			}
//			break;
		case _USERATTRIB_MERCENARYEXP:
			//王玉波注释佣兵等级与经验..
/*			{
				m_data.SetMercenaryExp(__max(0, (int)m_data.m_Info.dwMercenaryExp+i64Data));
				IF_NOT (msg.Append(_USERATTRIB_MERCENARYEXP, this->GetMercenaryExp()))
					return false;
			}
			break;
		case _USERATTRIB_MERCENARYRANK:
			{
				m_data.SetMercenaryRank(__max(0, (int)m_data.m_Info.ucMercenaryRank + i64Data));
				IF_NOT (msg.Append(_USERATTRIB_MERCENARYRANK, this->GetMercenaryRank()))
					return false;
			}
			break;*/
		case _USERATTRIB_NOBILITYRANK:
			{
// 				m_data.SetNobilityRank(__max(0, (int)m_data.GetNobilityRank() + i64Data));
// 				IF_NOT (msg.Append(_USERATTRIB_NOBILITYRANK, this->GetNobilityRank()))
// 					return false;
			}
			break;
	//王玉波注释，关于导师经验与功勋..
/*		case _USERATTRIB_TUTOR_EXP:
			{
				m_data.SetTutorExp(this->GetTutorExp()+i64Data);
				IF_NOT (msg.Append(_USERATTRIB_TUTOR_EXP, this->GetTutorExp()))
					return false;
			}
			break;*/
		case _USERATTRIB_TUTOR_LEVEL:
			{
				m_data.SetTutorLevel(__max(0, (int)this->GetTutorLevel()+i64Data));
				IF_NOT (msg.Append(_USERATTRIB_TUTOR_LEVEL, this->GetTutorLevel()))
					return false;
			}
			break;
		case _USERATTRIB_MAXEUDEMON:
			{
			}
			break;
		case _USERATTRIB_STONELEV:   //石头矿
            {
// 				int nOldStoneLev = GetStoneLev();
// 				int nNewStoneLev = nOldStoneLev+i64Data;
// 			//	int nNewStoneLev = __min(_MAX_STORAGE_TYPE, nOldStorageLimit+i64Data*18);
// 				m_data.SetStoneLev(nNewStoneLev, true);
// 				IF_NOT (msg.Append(_USERATTRIB_STONELEV, GetStoneLev()))
// 					return false;
			}
			break;
		case _USERATTRIB_GRASSLEV://            = 83,   //伐木
			{
// 				int nOldGrassLev = GetGrassLev();
// 				int nNewGrassLev = nOldGrassLev+i64Data;
// 				//	int nNewStoneLev = __min(_MAX_STORAGE_TYPE, nOldStorageLimit+i64Data*18);
// 				m_data.SetGrassLev(nNewGrassLev, true);
// 				IF_NOT (msg.Append(_USERATTRIB_GRASSLEV, GetGrassLev()))
// 					return false;
			}
			break;
		case _USERATTRIB_BODYLEV://             = 84,   //皮脂
			{
// 				int nOldBodyLev = GetBodyLev();
// 				int nNewBodyLev = nOldBodyLev+i64Data;
// 				//	int nNewStoneLev = __min(_MAX_STORAGE_TYPE, nOldStorageLimit+i64Data*18);
// 				m_data.SetBodyLev(nNewBodyLev, true);
// 				IF_NOT (msg.Append(_USERATTRIB_BODYLEV, GetBodyLev()))
// 					return false;
			}
			break;
		case _USERATTRIB_BOXLEV://              = 85,   //开宝箱
			{
// 				int nOldBoxLev = GetBoxLev();
// 				int nNewBoxLev = nOldBoxLev+i64Data;
// 				//	int nNewStoneLev = __min(_MAX_STORAGE_TYPE, nOldStorageLimit+i64Data*18);
// 				m_data.SetBoxLev(nNewBoxLev, true);
// 				IF_NOT (msg.Append(_USERATTRIB_BOXLEV, GetBoxLev()))
// 					return false;
			}
			break;
		case _USERATTRIB_ARENA_SCORE://竞技场积分
			{
				if (GetArenaScore() + i64Data < 0)
					return false;

				CHECKF(m_pUserWarGame);
				m_pUserWarGame->SetArenaScore(GetArenaScore() + i64Data);
				
				IF_NOT (msg.Append(_USERATTRIB_ARENA_SCORE, this->GetArenaScore()))
					return false;
			}
			break;
		default:
			return false;
		}
	}

	//20070316修罗:发送同屏玩家血量改变--------------------------
	//只要是玩家血量.不考虑nSynchro.直接房间广播
	if((idxAttr == _USERATTRIB_LIFE || idxAttr == _USERATTRIB_MANA) && nSynchro != SYNCHRO_FALSE )
	{
		this->BroadcastRoomMsg(&msg, INCLUDE_SELF,EM_SENDNPCFLAG_FALSE);
		return true;
	}
	//-----------------------------------------------------------

	if(nSynchro != SYNCHRO_FALSE)
	{
		switch(nSynchro)
		{
		case SYNCHRO_TRUE:
			this->SendMsg(&msg);
			break;

		case SYNCHRO_BROADCAST:
			this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			break;

		default:
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SetAttrib(int idxAttr, __int64 i64Data, int nSynchro)
{
	CMsgUserAttrib	msg;
	if(!msg.Create(GetID(), _USERATTRIB_NONE, 0))			// _USERATTRIB_NONE : 不添加
		return;
	
	switch(idxAttr)
	{
//	case	_USERATTRIB_ENERGY:
//		{
//			this->SetEnergy(i64Data);
//			if(!msg.Append(_USERATTRIB_ENERGY, this->GetEnergy()))
//				return;
//		}
//		break;
//	case	_USERATTRIB_MAXENERGY:
//		{
//			// no operation
//			if (!msg.Append(_USERATTRIB_MAXENERGY, this->GetMaxEnergy()))
//				return;
//		}
//		break;

	case	_USERATTRIB_PK:
		{
// 			char JudgeUpdate = 0;
// 			if (0 == i64Data || 1 == i64Data || 6 == i64Data ||11 == i64Data ||16 == i64Data|| 20 == i64Data)
// 			{
// 				JudgeUpdate = 1;
// 			}
			m_data.SetPk(i64Data/* JudgeUpdate*/);
			if(!msg.Append(_USERATTRIB_PK, this->GetPk()))
				return;
		}
		break;
	case	_USERATTRIB_HONOR:
		{
// 			m_data.SetHonor(i64Data, true);
// 			if(!msg.Append(_USERATTRIB_HONOR, this->GetHonor()))
// 				return;
		}
		break;
	case	_USERATTRIB_LIFE:
		{
			i64Data = __max(0, __min(this->GetMaxLife(), i64Data));

			SetLife(i64Data);
			if(!msg.Append(_USERATTRIB_LIFE, GetLife()))
				return;
		}
		break;
	case _USERATTRIB_CURRENTSOUL:
		{
			UINT nUserSoul=g_VariantManager.GetVariantInt(SPIRIT_LIMIT,GetID());

		//	if(nUserSoul > GetMaxSoulAmount())
			//	nUserSoul = GetMaxSoulAmount();
			
			if(!msg.Append(_USERATTRIB_CURRENTSOUL, nUserSoul))
				return;
		}
		break;

	case _USERATTRIB_LEV:
		{
			int nOldLev = this->GetLev();
			//20090612haocq更新升级时间
// 			if (nOldLev < MAX_USERLEV && i64Data > 0)
// 			{
// 				m_data.SetUpLevelTime(time(NULL));
// 				IF_NOT (msg.Append(_USERATTRIB_UPLEVEL_TIME, GetUpLevelTime()))
// 					return ;
// 			}
			int nNewLev = __max(0, __min(MAX_USERLEV, i64Data));

			m_data.SetLev(nNewLev);
			if(!msg.Append(_USERATTRIB_LEV, GetLev()))
				return;

			// 等级在70级上下发生变化，需要调整经验值存储格式
			// 70------>71
			if (nNewLev > ADJUST_EXPFORMAT_LEVEL && nOldLev <= ADJUST_EXPFORMAT_LEVEL)
				this->SetAttrib(_USERATTRIB_EXP, this->GetExp()/10, nSynchro);
			// 71------>70
			else if (nNewLev <= ADJUST_EXPFORMAT_LEVEL && nOldLev > ADJUST_EXPFORMAT_LEVEL)
				this->SetAttrib(_USERATTRIB_EXP, this->GetExp()*10, nSynchro);
			/*
			int nOldGiftPoint = m_data.GetGiftPoint();
			int nNewGiftPoint = nOldGiftPoint+1;
			m_data.SetGiftPoint(nNewGiftPoint);
			if(!msg.Append(_USERATTRIB_GIFT_GIFTPOINT,m_data.GetGiftPoint()))
				return;
			*/
		}
		break;
	case _USERATTRIB_GIFT_GIFTPOINT:		//天赋点
		{
// 			m_data.SetGiftPoint(i64Data,true);
// 			if(!msg.Append(_USERATTRIB_GIFT_GIFTPOINT,m_data.GetGiftPoint()))
// 				return;
		}
		break;
	case _USERATTRIB_ADDPOINT:
		{
			i64Data = __max(0, i64Data);

// 			m_data.SetAddPoint(i64Data);
// 			if(!msg.Append(_USERATTRIB_ADDPOINT, GetAddPoint()))
// 				return;
		}
		break;
	case	_USERATTRIB_MANA:
		{
			i64Data = __max(0, __min(this->GetMaxMana(), i64Data));

			m_data.SetMana(i64Data);
			if(!msg.Append(_USERATTRIB_MANA, GetMana()))
				return;
		}
		break;
	case _USERATTRIB_SPEED:
		{
			m_data.SetSpeed(__max(0, i64Data));
			if(!msg.Append(_USERATTRIB_SPEED, AdjustSpeed(GetSpeed())))
				return;
		}
		break;
	case _USERATTRIB_POTENTIAL:
		{
			//王玉波注释，关于潜能..
/*			m_data.SetPotential(__max(0, i64Data));
			if (!msg.Append(_USERATTRIB_POTENTIAL, GetPotential()))
				return;*/
		}
		break;
	case _USERATTRIB_SP:
		{
			SetSp( i64Data );
			IF_NOT( msg.Append( _USERATTRIB_SP, GetSp() ) )
				return;
		}
		break;
//------------------------06.12.13王玉波添加,关于魂----------begin--------------------------

	case	_USERATTRIB_EXP:
		{
			//goto 经验增量
			if (!IsInMapInstance())//副本中不计，因为进出副本都会保存玩家信息，而且祭坛经验太多，防止祭坛时间数据库压力过大
				m_ExpIncr += (i64Data-GetExp());
			m_data.SetExp(i64Data < 0 ? 0 :i64Data);
			if(!msg.Append(_USERATTRIB_EXP, GetExp()))
				return;
		}
		break;
	case _USEREXIT_ALLTIME:
		{
			SetAllAddExpTime(i64Data);
			if(!msg.Append(_USEREXIT_ALLTIME, GetAllAddExpTime()))//  [9/15/2007 %PENGFENG%]
				return;
		}
		break;
	case	_USERATTRIB_MONEY:
		{
			m_data.SetMoney(__max(0, i64Data),1);
			if(!msg.Append(_USERATTRIB_MONEY, GetMoneyByType(1)))
				return;
		}
		break;
	case	_USERATTRIB_MONEY2:
		{
			m_data.SetMoney(__max(0, i64Data),2);
			if(!msg.Append(_USERATTRIB_MONEY2, GetMoneyByType(2)))
				return;
		}
		break;
	case	_USERATTRIB_MONEY3:
		{
			m_data.SetMoney(__max(0, i64Data),3);
			if(!msg.Append(_USERATTRIB_MONEY3, GetMoneyByType(3)))
				return;
		}
		break;
	case	_USERATTRIB_MONEY4:
		{
			m_data.SetMoney(__max(0, i64Data),4);
			if(!msg.Append(_USERATTRIB_MONEY4, GetMoneyByType(4)))
				return;
		}
		break;
	case	_USERATTRIB_KEEPEFFECT:
		{
//			ResetEffect();		// reset all
//			this->SetEffect(i64Data);
//			if(!msg.Append(idxAttr, this->GetEffect()))
				return;
		}
		break;
	case	_USERATTRIB_HAIR:
		{
		}
		break;
	case	_USERATTRIB_PORFESSION:
		{
// 			m_data.SetProfession(i64Data, true);
// 			if(!msg.Append(idxAttr, m_data.GetProfession()))
// 				return;
		}
		break;
	case	_USERATTRIB_LOOKFACE:
		{
			// 不支持修改LOOKFACE属性
			if(!msg.Append(idxAttr, i64Data))
				return;
		}
		break;
	case	_USERATTRIB_SIZEADD:
		{
			// 不支持修改SIZEADD属性
			if(!msg.Append(idxAttr, i64Data))
				return;
		}
		break;
	case	_USERATTRIB_SUPERMAP_ORDER:
		{
			// 不支持修改SIZEADD属性
			if(!msg.Append(idxAttr, i64Data))
				return;
		}
		break;
/*	case	_USERATTRIB_MERCENARYEXP:
		{
			m_data.SetMercenaryExp(i64Data, true);
			if(!msg.Append(idxAttr, m_data.m_Info.dwMercenaryExp))
				return;
		}
		break;
	case	_USERATTRIB_MERCENARYRANK:
		{
			m_data.SetMercenaryRank(i64Data, true);
			if (!msg.Append(idxAttr, m_data.m_Info.ucMercenaryRank))
				return;
		}
		break;*/
		//王玉波注释以上，关于佣兵
	case _USERATTRIB_NOBILITYRANK:
		{
// 			m_data.SetNobilityRank(__max(0, i64Data));
// 			if (!msg.Append(idxAttr, m_data.GetNobilityRank()))
// 				return;
 		}
		break;
//	case	_USERATTRIB_ACTIONSPEED:
//		{
//			// TODO: 
//		}
//		break;
	case	_USERATTRIB_TUTOR_EXP:
		{
		}
		break;
	case	_USERATTRIB_TUTOR_LEVEL:
		{
			m_data.SetTutorLevel(__max(0, i64Data));
			IF_NOT (msg.Append(_USERATTRIB_TUTOR_LEVEL, this->GetTutorLevel()))
				return ;
		}
		break;
	case _USERATTRIB_MAXEUDEMON:
		{
		}
		break;
	case	_USERATTRIB_MAXLIFEPERCENT:
		{
		}
		break;
	case _USERATTRIB_EUDBROODPACKSIZE:
		{

		}
		break;
	case _USERATTRIB_HIDESTATUS://是否隐身
		{
			if( !msg.Append( _USERATTRIB_HIDESTATUS,i64Data ) )
				return;
		}
		break;
	case _USERATTRIB_USETITLE://[游途道标 2008/10/14]称号使用状态
		{
			if (!msg.Append(_USERATTRIB_USETITLE, i64Data))
			{
				return;
			}
		}
		break;
	case _USERATTRIB_ARENA_SCORE://竞技场积分
		{
			if(i64Data < 0)
				i64Data = 0;

			CHECK(m_pUserWarGame);
			m_pUserWarGame->SetArenaScore(i64Data);

			if (!msg.Append(_USERATTRIB_ARENA_SCORE, i64Data))
			{
				return;
			}
		}
	default:
		return;
	}

	//20070316修罗:发送同屏玩家血量改变--------------------------
	//只要是玩家血量.不考虑nSynchro.直接房间广播
	if((idxAttr == _USERATTRIB_LIFE || idxAttr == _USERATTRIB_MANA) && nSynchro != SYNCHRO_FALSE )
	{
		this->BroadcastRoomMsg(&msg, INCLUDE_SELF,EM_SENDNPCFLAG_FALSE);
		return;
	}
	//-----------------------------------------------------------
	switch(nSynchro)
	{
	case SYNCHRO_TRUE:
		this->SendMsg(&msg);
		break;

	case SYNCHRO_BROADCAST:
		this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		break;
//	case _USERATTRIB_EXPUPPERCENT:
//		{
//			if(!msg.Append(_USERATTRIB_EXPUPPERCENT, GetUserExpUpPre()))// 陈剑飞修改 [6/16/2007]
//				return;
//		}
//		break;

	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// return false mean fatal syn error.
bool CUser::SynPosition(int nPosX, int nPosY, int nMaxDislocation/* = 8*/)
{
	if (nMaxDislocation <= 0 || (nPosX == 0 && nPosY == 0))	// ignore in this condition
		return true;

	int nDislocation = this->GetDistance(nPosX, nPosY);
	if (nDislocation >= nMaxDislocation)
		return false;

	if (nDislocation <= 0)
		return true;


/*	if (this->IsGM())
		this->SendSysMsg("syn move: (%u, %u)->(%u, %u)",
									this->GetPosX(), this->GetPosY(),
									nPosX, nPosY);

	CMapPtr pMap = this->GetMap();
	IF_NOT(pMap && pMap->IsValidPoint(nPosX, nPosY))
		return false;
	
	// correct pos
	CMsgAction msg;
	if (msg.Create(this->GetID(), nPosX, nPosY, this->GetDir(), actionSynchro, nPosX, nPosY))
		this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);

	// process move
	this->ProcessOnMove(MOVEMODE_SYNCHRO);
	this->JumpPos(nPosX, nPosY);*/
	return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessAfterMove()
{
	return;//无此功能，屏蔽
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessOnMove(int nMoveMode)
{
	// stop fight
	if(nMoveMode != MOVEMODE_TRACK && nMoveMode != MOVEMODE_JUMPMAGICATTCK 
		&& nMoveMode != MOVEMODE_COLLIDE && nMoveMode != MOVEMODE_SYNCHRO)
	{
		ClrAttackTarget();
	}

	if(nMoveMode == MOVEMODE_RUN || nMoveMode == MOVEMODE_JUMP || nMoveMode == MOVEMODE_WALK
		|| (nMoveMode >= MOVEMODE_RUN_DIR0 && nMoveMode <= MOVEMODE_RUN_DIR7))		// && QueryMagic()->IsKeepBow()
	{
		if (QueryMagic()){
			//QueryMagic()->setMagicState(MAGICSTATE_INTONE);
			QueryMagic()->AbortMagic(true);
		}

		// run
		if(nMoveMode == MOVEMODE_RUN || (nMoveMode >= MOVEMODE_RUN_DIR0 && nMoveMode <= MOVEMODE_RUN_DIR7))
		{
	#ifdef	PALED_DEBUG
			if(!m_bRun)
				SendSysMsg("RUN");
	#endif
			m_bRun	= true;
// 			//这里是设置体力的
// 			if (GetPkMode() != PKMODE_SAFE)
// 			{
// 				AddAttrib(_USERATTRIB_SP, -1, SYNCHRO_TRUE);
// 			}
		}
//		else
//			ResetEnergyInc();
	}

//	if(nMoveMode != MOVEMODE_SHIFT && nMoveMode != MOVEMODE_SYNCHRO)
//		QueryStatusSet()->DelObj(STATUS_WEAPONDAMAGE);

//	if(nMoveMode != MOVEMODE_SHIFT && nMoveMode != MOVEMODE_SYNCHRO)
//		QueryStatusSet()->DelObj(STATUS_KEEPBOW);

	// stop mine
	this->StopMine();

	// foot print
	m_tFootPrint = ::TimeGet();
	m_posFootPrint.x = this->GetPosX();
	m_posFootPrint.y = this->GetPosY();

	// unlock
	///softworms为了调试而去掉所有PK保护
	//CRole::DetachStatus(this->QueryRole(), STATUS_PK_PROTECT);

	//m_tLock.Clear();
}


//////////////////////////////////////////////////////////////////////
void CUser::ProcessOnAttack()
{
	///softworms为了调试而去掉所有PK保护
	//CRole::DetachStatus(this->QueryRole(), STATUS_PK_PROTECT);

	//m_tLock.Clear();
}
//////////////////////////////////////////////////////////////////////
void CUser::GetFootPrint	(int& nPosX, int& nPosY)
{
	if (::TimeGet()-m_tFootPrint >= TIME_FOOTPRINT)
	{	// time out
		nPosX = this->GetPosX();
		nPosY = this->GetPosY();
	}
	else
	{
		nPosX = m_posFootPrint.x;
		nPosY = m_posFootPrint.y;
	}
}


DWORD CUser::GetMaxSp()
{
	DWORD prof = this->GetProfession();
	if( prof & PROF_NEWHAND ) //新手
	{
		return 0;
	}
	return 99;
}
DWORD CUser::GetAtkHitRate()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetAgi();
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*0.8f;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*1.0f;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*1.2f;
		
	}
	else
	{
		LOGERROR( "CUser::GetAtkHitRate,用户:%s职业:%d",this->GetName(),prof );
	}

	int nHitRateAdd = UserJinmeiSysMgr()->GetJinMeiValue(USERJINMEITYPE_DAIMEI, this->GetID());

	if (m_dwBuffAddhit<0 && ((DWORD)(value+0.5) + m_equipHitarteTotal+ m_dwGiftAddhit + m_WPSLHitarte + nHitRateAdd + m_eudemonSoulAddHitRate)<=abs(m_dwBuffAddhit))
		return 1;
	else
		return (DWORD)(value+0.5) + m_equipHitarteTotal+ m_dwGiftAddhit + m_dwBuffAddhit + m_WPSLHitarte + nHitRateAdd + m_eudemonSoulAddHitRate;
}
DWORD CUser::GetAtkHitRateUserOnly(int nType)
{
	double value = 0.0; 
	DWORD prof = this->GetProfession(nType);
	DWORD dwData = GetAgiUserOnly(nType);
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*0.8f;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*1.0f;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*1.4f;
		
	}
	else
	{
		LOGERROR( "CUser::GetAtkHitRateUserOnly,用户:%s职业:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}

CItem*	CUser::GetItem(OBJID idItem, bool bShen, bool bStorage)							
{
	if(idItem==ID_NONE)
	{
		STACK_DUMP();
	}
	
	CHECKF(idItem);
	CHECKF(m_pPackage);
	int nStep = 1;
	CItem *pItem =NULL;
	DEBUG_TRY
		//[游途道标 2009.04.27]修改函数,取得的物品包括身上的装备
		
	pItem = m_pPackage->GetItem( idItem );
// 	nStep = 2;
// 	if (!pItem && bShen)
// 	{
// 		for (int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; ++i)
// 		{
// 			pItem = *(GetEquipItemPtr(i));
// 			if (pItem && idItem == pItem->GetID())
// 			{
// 				break;
// 			}
// 		}
// 		if(i == ITEMPOSITION_EQUIPEND)
// 			pItem = NULL;
// 	}
// 	nStep = 3;
// 	if (!pItem && bStorage && m_pStorage)
// 	{
// 		pItem = m_pStorage->GetItemByID( idItem );
// 	}
	DEBUG_CATCH2("CItem*	GetItem %u",nStep);
	return pItem;
}

DWORD CUser::GetTough()
{
	int nToughAdd = UserJinmeiSysMgr()->GetJinMeiValue(USERJINMEITYPE_YINGWEI, this->GetID());

	if (m_dwBuffAddTough<0 && (GetLev() * 10 + m_equipToughTotal+ m_dwGiftAddTough + m_WPSLTough + nToughAdd)<=abs(m_dwBuffAddTough))
		return 1;
	else
		return	GetLev() * 10 + m_equipToughTotal+ m_dwGiftAddTough + m_dwBuffAddTough + m_WPSLTough + nToughAdd;
}
DWORD CUser::GetToughUserOnly(int nType)
{
	return	GetLev() * 10;
}
DWORD CUser::GetMaxLife()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData1 = GetSta();
	DWORD dwData2 = GetLev();
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData1*45 + dwData2 * 150;
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData1*45 + dwData2 * 150;
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData1*60 + dwData2 * 150;
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else
	{
		LOGERROR( "CUser::GetMaxLife函数出错,用户:%s职业:%d",this->GetName(),prof );
	}
	int nLifeAdd = UserJinmeiSysMgr()->GetJinMeiValue(USERJINMEITYPE_YANGQIAO, this->GetID());

	if (m_dwBuffAddhp<0 && ((DWORD)(value+0.5) + m_equipAddhpTotal+ m_dwGiftAddhp + nLifeAdd)<=abs(m_dwBuffAddhp))
		return 1;
	else
		return (DWORD)(value+0.5) + m_equipAddhpTotal+ m_dwGiftAddhp + m_dwBuffAddhp + nLifeAdd;
}
DWORD CUser::GetMaxLifeUserOnly(int nType)
{
	double value = 0.0; 
	DWORD prof = this->GetProfession(nType);
	DWORD dwData1 = GetStaUserOnly(nType);
	DWORD dwData2 = GetLev();

	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData1*45 + dwData2 * 150;
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData1*45 + dwData2 * 150;
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData1*60 + dwData2 * 150;
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else
	{
		LOGERROR( "CUser::GetMaxLifeUserOnly函数出错,用户:%s职业:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}
DWORD CUser::GetMaxMana()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData1 =  GetSpi();
	DWORD dwData2 = GetLev();
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData1*38.5f + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData1*38.5f + dwData2 * 150 * 0.7f;	
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData1*28 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else
	{
		LOGERROR( "CUser::GetMaxManaUserOnly函数出错,用户:%s职业:%d",this->GetName(),prof );
	}

	int nManaAdd = UserJinmeiSysMgr()->GetJinMeiValue(USERJINMEITYPE_YINGQIAO, this->GetID());

	if (m_dwBuffAddmp<0 && ((DWORD)(value+0.5) + m_equipAddmpTotal+ m_dwGiftAddmp + nManaAdd)<=abs(m_dwBuffAddmp))
		return 1;
	else
		return (DWORD)(value+0.5) + m_equipAddmpTotal+ m_dwGiftAddmp + m_dwBuffAddmp + nManaAdd;
}

DWORD CUser::GetMaxManaUserOnly(int nType)
{
	double value = 0.0; 
	DWORD prof = this->GetProfession(nType);
	DWORD dwData1 =  GetSpiUserOnly(nType);
	DWORD dwData2 =	 GetLev();
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData1*38.5f + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData1*38.5f + dwData2 * 150 * 0.7f;	
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData1*28 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else
	{
		LOGERROR( "CUser::GetMaxManaUserOnly函数出错,用户:%s职业:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}

DWORD CUser::GetCrit()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetAgi();
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*1;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*1.1f;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*0.9f;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*1.1f;
		
	}
	else
	{
		LOGERROR( "CUser::GetCrit,用户:%s职业:%d",this->GetName(),prof );
	}

	int nAddCriHit = UserJinmeiSysMgr()->GetJinMeiValue(USERJINMEITYPE_CHONGMEI, this->GetID());
	if (m_dwBuffAddcrihit<0 && ((DWORD)(value+0.5) + m_equipCrihitTotal+ m_dwGiftAddcrihit + m_WPSLCrihit + nAddCriHit)<=abs(m_dwBuffAddcrihit))
		return 1;
	else
		return (DWORD)(value+0.5) + m_equipCrihitTotal+ m_dwGiftAddcrihit + m_dwBuffAddcrihit + m_WPSLCrihit + nAddCriHit;
}
DWORD CUser::GetCritUserOnly(int nType)
{
	double value = 0.0; 
	DWORD prof = this->GetProfession(nType);
	DWORD dwData = GetAgiUserOnly(nType);
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*1;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*1.1f;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*0.9f;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*1.1f;
		
	}
	else
	{
		LOGERROR( "CUser::GetDdgUserOnly,用户:%s职业:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}
DWORD CUser::GetAtk()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetStr();
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*10;
		
	}
	else
	{
		LOGERROR( "CUser::GetAtk,用户:%s职业:%d",this->GetName(),prof );
	}

	int nAddAtk = UserJinmeiSysMgr()->GetJinMeiValue(USERJINMEITYPE_RENMEI, this->GetID());

	if (m_dwBuffAddphyatt<0 && ((DWORD)(value+0.5) + m_equipPhysicAttackTotal + m_dwGiftAddphyatt + m_WPSLPhysicAttack + nAddAtk + m_eudemonSoulphyatt)<=abs(m_dwBuffAddphyatt))
		return 1;
	else
		return (DWORD)(value+0.5) + m_equipPhysicAttackTotal + m_dwGiftAddphyatt+ m_dwBuffAddphyatt + m_WPSLPhysicAttack + nAddAtk + m_eudemonSoulphyatt;
}

DWORD CUser::GetAtkUserOnly(int nType)
{
	double value = 0.0; 
	DWORD prof = this->GetProfession(nType);
	DWORD dwData = GetStrUserOnly(nType);
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*10;
		
	}
	else
	{
		LOGERROR( "CUser::GetMinAtkUserOnly函数出错,用户:%s职业:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}
DWORD CUser::GetMgcAtk()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetIntex();
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*11;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*2;
		
	}
	else
	{
		LOGERROR( "CUser::GetMgcAtk,用户:%s职业:%d",this->GetName(),prof );
	}

	int nMgcAddAtk = UserJinmeiSysMgr()->GetJinMeiValue(USERJINMEITYPE_DUMEI, this->GetID());

	if (m_dwBuffAddmagatt<0 && ((DWORD)(value+0.5) + m_equipMagicAttackTotal+ m_dwGiftAddmagatt + m_WPSLMagicAttack + nMgcAddAtk + m_eudemonSoulmagatt)<=abs(m_dwBuffAddmagatt))
		return 1;
	else
		return (DWORD)(value+0.5) + m_equipMagicAttackTotal+ m_dwGiftAddmagatt + m_dwBuffAddmagatt + m_WPSLMagicAttack + nMgcAddAtk + m_eudemonSoulmagatt;
}
DWORD CUser::GetMgcAtkUserOnly(int nType)
{
	double value = 0.0; 
	DWORD prof = this->GetProfession(nType);
	DWORD dwData = GetIntUserOnly(nType);
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*11;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*2;
		
	}
	else
	{
		LOGERROR( "CUser::GetMgcAtkUserOnly函数出错,用户:%s职业:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}
DWORD CUser::GetDef()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetSpi();

	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*6;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*5;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*5;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*10;
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*12;
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*8;
	}
	else
	{
		LOGERROR( "CUser::GetDef函数出错,用户:%s职业:%d",this->GetName(),prof );
	}
	if (m_dwBuffAddphydef<0 && ((DWORD)(value+0.5) + m_equipPhysicDefenseTotal+ m_dwGiftAddphydef)<=abs(m_dwBuffAddphydef))
		return 1;
	else
		return (DWORD)(value+0.5) + m_equipPhysicDefenseTotal+ m_dwGiftAddphydef + m_dwBuffAddphydef;
}
DWORD CUser::GetDefUserOnly(int nType)
{
	double value = 0.0; 
	DWORD prof = this->GetProfession(nType);
	DWORD dwData = GetSpiUserOnly(nType);
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*6;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*5;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*5;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*10;
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*12;
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*8;
	}
	else
	{
		LOGERROR( "CUser::GetDefUserOnly函数出错,用户:%s职业:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}

//////////////////////////////////////////////////////////////////////
DWORD CUser::GetSoulSum()
{
	int nData = /*this->GetSoul()*/100;

/*
	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		CItem* pEquip = this->GetEquipItemByPos(i);
		if(pEquip)
			nData += pEquip->GetInt(ITEMDATA_ADD_SOUL);
	}
*/

	//  [12/13/2007 朱斌]	天赋加成属性
/*	nData += ProcGiftUserPropertyAdd(EM_GTTMP1005_TARGETATT_soul);*/

	return __max(0, nData);
}

DWORD CUser::GetDdg()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetAgi();
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*1;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*0.6f;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*0.6f;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*0.8f;
	}
	else
	{
		LOGERROR( "CUser::GetDdg,用户:%s职业:%d",this->GetName(),prof );
	}

	int nDbgAdd = UserJinmeiSysMgr()->GetJinMeiValue(USERJINMEITYPE_YANGWEI, this->GetID());

	if (m_dwBuffAdddodge<0 && ((DWORD)(value+0.5) + m_equipDodgeTotal+ m_dwGiftAdddodge + m_WPSLDodge + nDbgAdd + m_eudemonSoulAddDodge)<=abs(m_dwBuffAdddodge))
		return 1;
	else
		return (DWORD)(value+0.5) + m_equipDodgeTotal+ m_dwGiftAdddodge + m_dwBuffAdddodge + m_WPSLDodge + nDbgAdd + m_eudemonSoulAddDodge;
}
DWORD CUser::GetDdgUserOnly(int nType)
{
	double value = 0.0; 
	DWORD prof = this->GetProfession(nType);
	DWORD dwData = GetAgiUserOnly(nType);
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*1;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*0.6f;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*0.6f;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*0.8f;
	}
	else
	{
		LOGERROR( "CUser::GetDdgUserOnly,用户:%s职业:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}


DWORD CUser::GetMagicDef()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetSpi();

	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*8;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*11;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*8;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*8;
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*7;
	}
	else
	{
		LOGERROR( "CUser::GetMagicDef函数出错,用户:%s职业:%d",this->GetName(),prof );
	}
	if (m_dwBuffAddmagdef<0 && ((DWORD)(value+0.5) + m_equipMagicDefenseTotal+ m_dwGiftAddmagdef + m_WPSLMagicDefense)<=abs(m_dwBuffAddmagdef))
		return 1;
	else
		return (DWORD)(value+0.5) + m_equipMagicDefenseTotal+ m_dwGiftAddmagdef + m_dwBuffAddmagdef + m_WPSLMagicDefense;
}
DWORD CUser::GetMagicDefUserOnly(int nType)
{
	double value = 0.0; 
	DWORD prof = this->GetProfession(nType);
	DWORD dwData = GetSpiUserOnly(nType);
	
	if( prof & PROF_NEWHAND ) //新手
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//辅助
	{
		value = dwData*8;
		
	}
	else if( prof & PROF_MASTER )//法师
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_PASTOR )//牧师
	{
		value = dwData*11;
		
	}
	else if( prof & PROF_ASSISSIN )//刺客
	{
		value = dwData*8;
		
	}
	else if( prof & PROF_SOLDIER )//战士
	{
		value = dwData*8;
	}
	else if( prof & PROF_SWORD )//弓手
	{
		value = dwData*7;
	}
	else
	{
		LOGERROR( "CUser::GetMagicDefUserOnly函数出错,用户:%s职业:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}

bool CUser::CheckLevUp(int nCurLev)
{
	//主职业
	int nShowLev = -1;

	if(nCurLev >= 19)
	{
		int nCountLev = (nCurLev - 19) / 10;
		if(GetJobLev(1) < (nCountLev + 1) * 10)
			nShowLev = (nCountLev + 1) * 10;

		if(nShowLev != -1)
		{
			SendSysMsg("主职业等级达到%u级才可继续升级",nShowLev);
			return false;
		}
	}
	return true;
}

void CUser::IncLev(int nLev,int nType)
{
	if(nType == 0) //人物升级
	{
		if(GetLev() >= USERMAXLEVEL)
			return;
		if(GetLev() + nLev >= USERMAXLEVEL)
			nLev = USERMAXLEVEL - GetLev();

		if(!CheckLevUp(GetLev()))
			return;

		if(GetLev() >= 10)
		{
			DWORD dwPro = m_data.GetJobByType(1);
			if(dwPro & PROF_NEWHAND) //新手不能超过10级
			{
				SendSysMsg(_TXTATR_TOPSHOW,"加入门派才能继续提升人物等级");
				return;
			}
			CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
			if (!pLevupexp)
				return;
			__int64 i64LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
			
			if(this->GetExp() < i64LevupExp)
			{
				SendSysMsg(_TXTATR_TOPSHOW,"您的经验不足，不能升级");
				return;
			}
			
			this->SetAttrib( _USERATTRIB_EXP,this->GetExp() - i64LevupExp,SYNCHRO_TRUE );
		}

		for(int i = 1 ; i < 3 ; i ++)
		{
			DWORD dwPro = m_data.GetJobByType(i);
			if(dwPro == 0 )
				continue;

			if( dwPro & PROF_NEWHAND) //新手
			{
				m_data.AddStr( 36 * nLev,i );
				m_data.AddIntex( 36 * nLev,i);
				m_data.AddSta( 36 * nLev,i );
				m_data.AddSpi( 36 * nLev,i );
				m_data.AddAgi( 36 * nLev,i);

				//m_data.SetJobLev(GetLev() + nLev,i);
			}
			else if( dwPro & PROF_ASSIST) //辅助
			{
				m_data.AddStr( 42* nLev,i);
				m_data.AddIntex( 57* nLev,i );
				m_data.AddSta( 57* nLev,i );
				m_data.AddSpi( 68* nLev,i );
				m_data.AddAgi( 42* nLev,i );
			}
			else if( dwPro & PROF_MASTER) //法师
			{
				m_data.AddStr( 37* nLev,i );
				m_data.AddIntex( 62* nLev,i);
				m_data.AddSta( 56* nLev,i );
				m_data.AddSpi( 68* nLev,i);
				m_data.AddAgi( 37* nLev,i);
			}
			else if( dwPro & PROF_PASTOR) //牧师
			{
				m_data.AddStr( 43* nLev,i );
				m_data.AddIntex( 62* nLev,i);
				m_data.AddSta( 50* nLev,i );
				m_data.AddSpi( 68* nLev,i);
				m_data.AddAgi( 43* nLev,i);
			}
			else if( dwPro & PROF_ASSISSIN) //刺客
			{
				m_data.AddStr( 62* nLev,i );
				m_data.AddIntex( 50* nLev,i);
				m_data.AddSta( 62* nLev,i );
				m_data.AddSpi( 50* nLev,i);
				m_data.AddAgi( 62* nLev,i);
			}
			else if( dwPro & PROF_SOLDIER) //战士
			{
				m_data.AddStr( 56* nLev,i );
				m_data.AddIntex( 37* nLev,i);
				m_data.AddSta( 74* nLev,i );
				m_data.AddSpi( 68* nLev,i);
				m_data.AddAgi( 37* nLev,i);
			}
			else if( dwPro & PROF_SWORD) //弓手
			{
				m_data.AddStr( 62* nLev,i );
				m_data.AddIntex( 50* nLev,i);
				m_data.AddSta( 62* nLev,i );
				m_data.AddSpi( 50* nLev,i);
				m_data.AddAgi( 62* nLev,i);
			}
		}
		m_data.SetLev(GetLev() + nLev);
//		m_data.SaveInfo();
		//goto 存过数据以后就把增量清0，减少写数据库次数
		m_MoneyIncr = 0;
		m_ExpIncr = 0;
	
		UpdateCalculatedProperty(UPDATECALTYPE_USER);

		DEBUG_TRY
		UpdateMissionMsgByMonsterType(100002);
		UpdateMissionMsgByItemType(_SPCITEM_MONEYFORSHOP);
		DEBUG_CATCH("UpdateMissionMsgByItemType(100002)")


		DEBUG_TRY
		if(GetLev() == 15 || GetLev() == 16 || GetLev() == 17 || GetLev() == 25 || GetLev() == 40)
			ProcessAction(1100 + GetLev());
		DEBUG_CATCH("ProcessAction(1100 + GetLev())")

		this->SetAttrib(_USERATTRIB_LIFE,this->GetMaxLife(),SYNCHRO_TRUE);
		this->SetAttrib(_USERATTRIB_MANA,this->GetMaxMana(),SYNCHRO_TRUE);

		CMsgUserAttrib msg;
		msg.Create(GetID(),_USERATTRIB_LEV,m_data.GetLev());
		BroadcastRoomMsg(&msg,true,EM_SENDNPCFLAG_FALSE);

		DEBUG_TRY
		if (m_pUserTutor)
		{
			m_pUserTutor->AddTutorAddedLev();
			if(GetLev()>10 && GetLev()%5==0)//每升到5的倍数时，处理一下师徒奖励掩码
				m_pUserTutor->ProcessGiftMask(GetLev());
		}
		DEBUG_CATCH("ProcessGiftMask(GetLev())")
//modify code by 林德渊 begin 2011.5.24
//		if (GetLev()>20)
//			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) level up to %d.", GetID(), GetName(), GetLev());
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) (level up:%d) (first job level:%d) (second job level:%d) (exp:%d)", GetID(), GetName(), GetLev(), m_data.GetJobLev(1), m_data.GetJobLev(2), GetExp());
//modify code by 林德渊 end
	}
	else if(nType == 1)
	{
		if((GetProfession(1) & PROF_NEWHAND) != 0 || GetProfession(1) == 0)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"本职业不能升级");
			return;
		}

		
// 		if(m_data.GetJobLev(nType) == 79)
// 		{
// 			//技能判断
// 			IMagicData* pMagic = NULL;
// 			DWORD prof = this->GetProfession(1);
// 
// 			for(int skillIndex = 0; skillIndex < 18 ; skillIndex ++)
// 			{
// 				if( prof & PROF_ASSIST )//辅助
// 				{
// 					if(skillIndex == 8 ||skillIndex == 10 || skillIndex == 14 || skillIndex == 15 || skillIndex == 16)
// 						continue;
// 					pMagic = QueryMagic(1400 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_MASTER )//法师
// 				{
// 					if(skillIndex == 8 || skillIndex == 10 || skillIndex == 13 || skillIndex == 14 || skillIndex == 15)
// 						continue;
// 					pMagic = QueryMagic(1120 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_PASTOR )//牧师
// 				{
// 					if(skillIndex == 8 || skillIndex == 11 || skillIndex == 14 || skillIndex == 16 || skillIndex == 17)
// 						continue;
// 					pMagic = QueryMagic(1138 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_ASSISSIN )//刺客
// 				{
// 					if(skillIndex == 8 || skillIndex == 13 || skillIndex == 15 || skillIndex == 16 || skillIndex == 17)
// 						continue;
// 					pMagic = QueryMagic(1200 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_SOLDIER )//战士
// 				{
// 					if(skillIndex == 0 || skillIndex == 5 ||  skillIndex == 9 || skillIndex == 14 || skillIndex == 16 || skillIndex == 17)
// 						continue;
// 					pMagic = QueryMagic(1100 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_SWORD )//弓手
// 				{
// 					if(skillIndex == 8 || skillIndex == 10 ||  skillIndex == 13 || skillIndex == 16 || skillIndex == 17)
// 						continue;
// 					pMagic = QueryMagic(1300 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 			}
// 
// 			if( prof & PROF_SOLDIER )//战士
// 			{
// 				pMagic = QueryMagic(9003);
// 				if(!pMagic || pMagic->GetMagicLevel() < 80)
// 					return;
// 			}
// 		}

		int nNextLev = m_data.GetJobLev(nType) + nLev;
		if(nNextLev > GetLev()+5)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"职业等级不能超过人物等级5级，请先升级人物等级");
			return;
		}
		if ( (nNextLev>=80 && QueryMagic()->CheckMagicLevAmount(70, 13, GetProfession(1))==false)
		    //|| (nNextLev>=65 && QueryMagic()->CheckMagicLevAmount(55, 9, GetProfession(1))==false)
			|| (nNextLev>=50 && QueryMagic()->CheckMagicLevAmount(40, 7, GetProfession(1))==false)
			//|| (nNextLev>=35 && QueryMagic()->CheckMagicLevAmount(25, 5, GetProfession(1))==false)
			//|| (nNextLev>=20 && QueryMagic()->CheckMagicLevAmount(10, 3, GetProfession(1))==false)
			)
		{
			char szText[128];
			sprintf(szText, "至少%d个技能达到%d级才可继续提升职业等级", (nNextLev-20)/15*2+3, nNextLev-nNextLev%10-10);
			SendSysMsg(szText);
			return;
		}
		CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER_FIRJOB * _EXP_TYPE + GetJobLev(1));
		if (!pLevupexp)
			return;
		__int64 i64LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
		
		if(this->GetExp() < i64LevupExp)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"您的经验不足，不能升级");
			return;
		}

		this->SetAttrib( _USERATTRIB_EXP,this->GetExp() - i64LevupExp,SYNCHRO_TRUE );

		if(nNextLev >= 25 && nNextLev <= 60)
			m_data.AddPot(2,nType);
		else if(nNextLev > 60 )
			m_data.AddPot(3,nType);

		m_data.SetJobLev(m_data.GetJobLev(nType) + nLev,nType);
//		m_data.SaveInfo();
		//goto 存过数据以后就把增量清0，减少写数据库次数
		m_MoneyIncr = 0;
		m_ExpIncr = 0;
		CMsgUserAttrib msg;
		msg.Create(GetID(),_USERATTRIB_ADDPOINT,m_data.GetPot(nType));
		msg.Append(_USERATTRIB_FIRJOBLEV,m_data.GetJobLev(nType));
		BroadcastRoomMsg(&msg,true);
//modify code by 林德渊 begin 2011.5.24
//		if (m_data.GetJobLev(nType)>20)
//			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) first job level up to %d.", GetID(), GetName(), m_data.GetJobLev(nType));
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) (level up:%d) (first job level:%d) (second job level:%d) (exp:%d)", GetID(), GetName(), GetLev(), m_data.GetJobLev(1), m_data.GetJobLev(2), GetExp());
//modify code by 林德渊 end
	}
	else if(nType == 2)
	{
		if((GetProfession(2) & PROF_NEWHAND) != 0 || GetProfession(2) == 0)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"职业不能升级");
			return;
		}

		int nNextLev = m_data.GetJobLev(nType) + nLev;
		if(nNextLev > GetLev()+5)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"职业等级不能超过人物等级5级，请先升级人物等级");
			return;
		}
		if ( (nNextLev>=80 && QueryMagic()->CheckMagicLevAmount(70, 13, GetProfession(2))==false)
			//|| (nNextLev>=65 && QueryMagic()->CheckMagicLevAmount(55, 9, GetProfession(2))==false)
			|| (nNextLev>=50 && QueryMagic()->CheckMagicLevAmount(40, 7, GetProfession(2))==false)
			//|| (nNextLev>=35 && QueryMagic()->CheckMagicLevAmount(25, 5, GetProfession(2))==false)
			//|| (nNextLev>=20 && QueryMagic()->CheckMagicLevAmount(10, 3, GetProfession(2))==false)
			)
		{
			char szText[128];
			sprintf(szText, "至少%d个技能达到%d级才可继续提升职业等级", (nNextLev-20)/15*2+3, nNextLev-nNextLev%10-10);
			SendSysMsg(szText);
			return;
		}
		CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER_SECJOB * _EXP_TYPE +  GetJobLev(2));
		if (!pLevupexp)
			return;
		__int64 i64LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
		
		if(this->GetExp() < i64LevupExp)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"您的经验不足，不能升级");
			return;
		}
		
		this->SetAttrib( _USERATTRIB_EXP,this->GetExp() - i64LevupExp,SYNCHRO_TRUE );

		if(nNextLev >= 25 && nNextLev <= 60)
			m_data.AddPot(2,nType);
		else if(nNextLev > 60 )
			m_data.AddPot(3,nType);
		
		m_data.SetJobLev(m_data.GetJobLev(nType) + nLev,nType);
		
		CMsgUserAttrib msg;
		msg.Create(GetID(),_USERATTRIB_ADDPOINT_SEC,m_data.GetPot(nType));		
		msg.Append(_USERATTRIB_SECJOBLEV,m_data.GetJobLev(nType));
		BroadcastRoomMsg(&msg,true);
//modify code by 林德渊 begin 2011.5.24
//		if (m_data.GetJobLev(nType)>20)
//			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) second job level up to %d.", GetID(), GetName(), m_data.GetJobLev(nType));
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) (level up:%d) (first job level:%d) (second job level:%d) (exp:%d)", GetID(), GetName(), GetLev(), m_data.GetJobLev(1), m_data.GetJobLev(2), GetExp());
//modify code by 林德渊 end
	}
	
// 	int nOldLev = this->GetLev();
// 	int nNewLev	= nOldLev + nLev;
// 
// 	if(nOldLev>=USERMAXLEVEL)
// 		return;
// 	else
// 		nNewLev = __min(nNewLev,USERMAXLEVEL);
// 
// 	this->SetAttrib(_USERATTRIB_LEV, nNewLev, SYNCHRO_TRUE);


// 	const int _thisLevel = this->GetLev();//已经升过级的新级别
// 	const int _lastLevel = _thisLevel-nLev;//上一级级别;

    //	g_objStrRes.GetStr(19101);
// 	int nFirstLev = atoi(g_objStrRes.GetStr(27001));
// 	int nFirstGift = atoi(g_objStrRes.GetStr(27002));
// 	int nIntervalLev = atoi(g_objStrRes.GetStr(27003));
// 
// 	ASSERT(nIntervalLev > 0);
// 	
// 	if(this->GetLev() >= nFirstLev)
// 	{
// 		int nAddGift  = 0;
// 		if( _lastLevel < nFirstLev)
// 		{
// 			nAddGift = nFirstGift + (_thisLevel - nFirstLev)/nIntervalLev;
// 		}
// 		else
// 		{
// 			int lastGetGift = nFirstGift + (_lastLevel - nFirstLev)/nIntervalLev;
// 			int thisGetGift = nFirstGift + (_thisLevel - nFirstLev)/nIntervalLev;
// 			nAddGift = thisGetGift - lastGetGift;
// 		}
// 		int nOldGiftPoint = /*m_data.GetGiftPoint()*/0;
// 		int nNewGiftPoint = nOldGiftPoint+nAddGift;
// 
// 		if(nAddGift > 0)
// 		{
// 			CMsgAction msgAction;
// 			IF_OK(msgAction.Create(GetID(), 0, 0, 0, actionLeanMagic,2))
// 				SendMsg(&msgAction);
// 		}
// 
// 		SetAttrib(_USERATTRIB_GIFT_GIFTPOINT,nNewGiftPoint,SYNCHRO_TRUE);
//     }


	
	
// 	else if( dwPro & PROF_BOWMAN )//弓手
// 	{
// 		//力量
// 		m_data.SetForce( m_data.GetForce() + _thisLevel*_GROW_ARCHER_STR - _lastLevel*_GROW_ARCHER_STR );
// 		//敏捷
// 		m_data.SetDexterity( m_data.GetDexterity() + _thisLevel*_GROW_ARCHER_DEX - _lastLevel*_GROW_ARCHER_DEX );
// 		//体力
// 		m_data.SetHealth( m_data.GetHealth() + _thisLevel*_GROW_ARCHER_VIT - _lastLevel*_GROW_ARCHER_VIT );
// 		//智力
// 		m_data.SetSoul( m_data.GetSoul() + _thisLevel*_GROW_ARCHER_SOUL - _lastLevel*_GROW_ARCHER_SOUL );
// 	}
// 	else if( dwPro & PROF_ENCHANTER )//法师
// 	{
// 		//力量
// 		m_data.SetForce( m_data.GetForce() + _thisLevel*_GROW_MAGE_STR - _lastLevel*_GROW_MAGE_STR );
// 		//敏捷
// 		m_data.SetDexterity( m_data.GetDexterity() + _thisLevel*_GROW_MAGE_DEX - _lastLevel*_GROW_MAGE_DEX );
// 		//体力
// 		m_data.SetHealth( m_data.GetHealth() + _thisLevel*_GROW_MAGE_VIT - _lastLevel*_GROW_MAGE_VIT );
// 		//智力
// 		m_data.SetSoul( m_data.GetSoul() + _thisLevel*_GROW_MAGE_SOUL - _lastLevel*_GROW_MAGE_SOUL );
// 	}
// 	//加点
// 	m_data.SetAddPoint( m_data.GetAddPoint()+_ADDITIONALPOINT_NUM * nLev );
// 
// 		
// 	CMsgUserAttrib	msg;
// 	if( msg.Create(GetID(), _USERATTRIB_FORCE, m_data.GetForce())
// 		&& msg.Append(_USERATTRIB_DEX, m_data.GetDexterity())
// 		&& msg.Append(_USERATTRIB_HEALTH, m_data.GetHealth())
// 		&& msg.Append(_USERATTRIB_SOUL, m_data.GetSoul())
// 		&& msg.Append( _USERATTRIB_ADDPOINT, m_data.GetAddPoint() )
// 	//	&& msg.Append( _USERATTRIB_MAXLIFE,this->GetMaxLife() )		//客户端有计算
// 		&& msg.Append( _USERATTRIB_MAXSP,this->GetMaxSp() )			//客户端没有计算
// 		)//20070127修罗:发送基本点数与HP，SP
// 	{
// 		SendMsg( &msg );
// 	}
// 		*/
// 	// 2003.2.9 升级自动补满血。
// 	this->SetAttrib(_USERATTRIB_LIFE,this->GetMaxLife(),SYNCHRO_TRUE);
// 	this->SetAttrib(_USERATTRIB_MANA,this->GetMaxMana(),SYNCHRO_TRUE);
// 	this->SetAttrib(_USERATTRIB_SP,this->GetMaxSp(),SYNCHRO_TRUE);//  [4/30/2007]
// 
// 	if(GetTeam())
// 	{
// 		CMsgTeamMember msg;
// 		if (msg.Create(_MSG_TEAMMEMBER_LEV, 1, GetID(), nNewLev))
// 			GetTeam()->BroadcastTeamMsg(&msg);
// 	}
// 	
// 	CMsgAction msgAction;
// 	if (msgAction.Create(this->GetID(), 0, 0, 0, actionUplev, 0, 0))
// 		this->BroadcastRoomMsg(&msgAction, INCLUDE_SELF);
// 	// team life broacast
// 	if (this->GetTeam())
// 	{
// 		this->BroadcastTeamLife(true); //广播队伍血量
// 		this->BroadcastTeamMana(true); //广播队伍魔法
// 	}
// 	// save info
// 	m_data.SaveInfo();
// 	
// 
// 	//20070406修罗:升级实时与UserList同步.
// 	//解决军团等级显示错误的某些bug
// 	
// 	MapGroup(PID)->QueryIntraMsg()->LevelUp(GetID(), nNewLev);
// 	
// 	if (QueryMagic())
// 		QueryMagic()->OwnerUpLevel(nNewLev);
// 	
// 	if(GetSynID() != ID_NONE)
// 		QuerySynAttr()->UpLevel(nNewLev);
// 	
// 	// tutor exp record
// 	if (this->GetLev() >= _MIN_TUTOR_LEVEL)
// 	{
// 		CTutorexpData*	pExpData;
// 		pExpData = CTutorexpData::CreateNew();
// 		if (pExpData && pExpData->Create(GetID(), Database()))
// 		{
// 			if (pExpData->GetID() == ID_NONE)
// 			{
// 				pExpData->SetInt(TUTOREXPDATA_ID, this->GetID());
// 				pExpData->SetInt(TUTOREXPDATA_TUTOR_LEV, this->GetLev());
// 				pExpData->InsertRecord();
// 			}
// 			else
// 			{
// 				if (pExpData->GetInt(TUTOREXPDATA_TUTOR_LEV) != this->GetLev())
// 				{
// 					pExpData->SetInt(TUTOREXPDATA_TUTOR_LEV, this->GetLev());
// 					pExpData->Update();
// 				}
// 			}
// 		}
// 		SAFE_RELEASE (pExpData);
// 	}
	// 修改出师条件 [陈剑飞10/26/2007]
//	int tutorId = GetTutor()->GetTutorID();
//	CUser *pUser = UserManager()->GetUser(tutorId);
//	if(GetLev() >= 40 && GetTutor() != NULL && pUser->GetLev() >=65)//徒弟40级，并且师傅等级在65级以上
//		Graduate();
    //----------------------------------------------------------------------------
	//20070711修罗:升级后更新角色卡
//	UpdateCalculatedProperty(UPDATECALTYPE_USER);
}

void CUser::AllotEudemonPoint(OBJID idEudemon ,int nType , int nData )
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	EudemonData data;
	pData->GetData(data);
	if(pData)
	{
		switch(nType)
		{
		case 1:
			{
				if(nData < 0)
					return;

				if(data.addpoint >= nData)
				{
					data.stradd += nData;
					data.addpoint -= nData;
					pData->SetInt(EUDEMONDATA_stradd,data.stradd);
					pData->SetInt(EUDEMONDATA_addpoint,data.addpoint,true);
				}
			}
			break;
		case 2:
			{
				if(nData < 0)
					return;

				if(data.addpoint >= nData)
				{
					data.intadd += nData;
					data.addpoint -= nData;
					pData->SetInt(EUDEMONDATA_intadd,data.intadd);
					pData->SetInt(EUDEMONDATA_addpoint,data.addpoint,true);
				}
			}
			break;
		case 3:
			{
				if(nData < 0)
					return;

				if(data.addpoint >= nData)
				{
					data.staadd += nData;
					data.addpoint -= nData;
					pData->SetInt(EUDEMONDATA_staadd,data.staadd);
					pData->SetInt(EUDEMONDATA_addpoint,data.addpoint,true);
				}
			}
			break;
		case 4:
			{
				if(nData < 0)
					return;

				if(data.addpoint >= nData)
				{
					data.spiadd += nData;
					data.addpoint -= nData;
					pData->SetInt(EUDEMONDATA_spiadd,data.spiadd);
					pData->SetInt(EUDEMONDATA_addpoint,data.addpoint,true);
				}
			}
			break;
		case 5:
			{
				if(nData < 0)
					return;

				if(data.addpoint >= nData)
				{
					data.agiadd += nData;
					data.addpoint -= nData;
					pData->SetInt(EUDEMONDATA_agiadd,data.agiadd);
					pData->SetInt(EUDEMONDATA_addpoint,data.addpoint,true);
				}
			}
			break;
		case 6:
			{
				m_pUserPet->fillType(data,3);
				m_pUserPet->fillAtt(data,3);
				if(data.life > data.dwMaxLife)
				{
					data.life = data.dwMaxLife;
					pData->SetInt(EUDEMONDATA_life,data.life,true );
				}

				if(m_pWarPet && (idEudemon == m_pWarPet->GetID()))
				{
					m_pWarPet->UpdateEudemonAll();
				}

				CMsgEudemonAttrib msgAttr;
				if(!msgAttr.Create(idEudemon, EUDATTR_LIFE, data.life))
					return;

				msgAttr.Append(EUDATTR_ADDPOINT,data.addpoint);
				msgAttr.Append(EUDATTR_STR,data.dwStr);
				msgAttr.Append(EUDATTR_INT,data.dwInt);
				msgAttr.Append(EUDATTR_STA,data.dwSta);
				msgAttr.Append(EUDATTR_SPI,data.dwSpi);
				msgAttr.Append(EUDATTR_AGI,data.dwAgi);
				msgAttr.Append(EUDATTR_MAXLIFE,data.dwMaxLife);
				msgAttr.Append(EUDATTR_ATK,data.dwPhyAtt);
				msgAttr.Append(EUDATTR_MAGICATK,data.dwMagAtt);
				msgAttr.Append(EUDATTR_DEF,data.dwPhyDef);
				msgAttr.Append(EUDATTR_MAGICDEF,data.dwMagDef);
				msgAttr.Append(EUDATTR_HIT,data.dwhit);
				msgAttr.Append(EUDATTR_DOGE,data.dwDoge);
				msgAttr.Append(EUDATTR_CRIT,data.dwCrit);
				msgAttr.Append(EUDATTR_TOUGH,data.dwTough);
				this->SendMsg(&msgAttr);
			}
		break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CUser::AllotPoint(int nData , int nType)
{	
	switch(nType)
	{
	case 1:
		{
			if(nData < 0)
				return;
			if(GetAddPoint(1) >= nData)
			{
				m_data.AddStr(nData * 100,1);
				m_data.AddPot(-1 * nData,1);
			}
		}
		break;
	case 2:
		{
			if(nData < 0)
				return;

			if(GetAddPoint(1) >= nData)
			{
				m_data.AddIntex(nData * 100,1);
				m_data.AddPot(-1 * nData,1);
			}
		}
		break;
	case 3:
		{
			if(nData < 0)
				return;

			if(GetAddPoint(1) >= nData)
			{
				m_data.AddSta(nData * 100,1);
				m_data.AddPot(-1 * nData,1);
			}
		}
		break;
	case 4:
		{
			if(nData < 0)
				return;

			if(GetAddPoint(1) >= nData)
			{
				m_data.AddSpi(nData * 100,1);
				m_data.AddPot(-1 * nData,1);
			}
		}
		break;
	case 5:
		{
			if(nData < 0)
				return;

			if(GetAddPoint(1) >= nData)
			{
				m_data.AddAgi(nData * 100,1);
				m_data.AddPot(-1 * nData,1);
			}
		}
		break;
	case 6:
		{
			if(nData < 0)
				return;

			if(GetAddPoint(2) >= nData)
			{
				m_data.AddStr(nData * 100,2);
				m_data.AddPot(-1 * nData,2);
			}
		}
		break;
	case 7:
		{
			if(nData < 0)
				return;

			if(GetAddPoint(2) >= nData)
			{
				m_data.AddIntex(nData * 100,2);
				m_data.AddPot(-1 * nData,2);
			}
		}
		break;
	case 8:
		{
			if(nData < 0)
				return;

			if(GetAddPoint(2) >= nData)
			{
				m_data.AddSta(nData * 100,2);
				m_data.AddPot(-1 * nData,2);
			}
		}
		break;
	case 9:
		{
			if(nData < 0)
				return;

			if(GetAddPoint(2) >= nData)
			{
				m_data.AddSpi(nData * 100,2);
				m_data.AddPot(-1 * nData,2);
			}
		}
		break;
	case 10:
		{
			if(nData < 0)
				return;

			if(GetAddPoint(2) >= nData)
			{
				m_data.AddAgi(nData * 100,2);
				m_data.AddPot(-1 * nData,2);
			}
		}
		break;
	case 11:
		{
			if(GetCurJob() == 1)
			{
				UpdateCalculatedProperty(UPDATECALTYPE_USER);
			}
			else
			{
				SendSecAtt(1);
			}
		}
		break;
	case 12:
		{
			if(GetCurJob() == 2)
			{
				UpdateCalculatedProperty(UPDATECALTYPE_USER);
			}
			else
			{
				SendSecAtt(2);
			}
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////
I64 CUser::GetEffect()
{
	return m_data.m_Info.i64Effect;
}

//////////////////////////////////////////////////////////////////////
int CUser::GetDir()
{
	return m_nDir;
}

//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool CUser::LeaveMap()
{
	m_idNowEudemon = 0;
	DEBUG_TRY
	if(m_pWarPet)
	{
		m_idNowEudemon = m_pWarPet->GetID();
		SaveEudemonStatus();
		CallBackEudemon(m_idNowEudemon,false,false,false);
	}
	DEBUG_CATCH("CUser::LeaveMap() CallBackEudemon");
	CMapPtr pMap = GetMap();

	if(pMap)
	{
		if (pMap->IsPkGameMap())
		{
			DetachBadlyStatus(this);
		}
		if(m_idProcess == MSGPORT_MAPGROUP_INSTANCE)
		{
			DEBUG_TRY
			if(pMap->GetDocID() == 2002)
			{
				//删除新手变态副本武器
				bool bHasKind = false;
				DelItemByTypeMulti(144000,0,bHasKind, false, true);
				CItem* pItem = GetEquipItemByPos(ITEMPOSITION_WEAPONR);
				if(pItem && pItem->GetTypeID() == 144000)
				{
					EraseEquip(ITEMPOSITION_WEAPONR,true);
				}
			}
			DEBUG_CATCH("删除变态武器");
		}

		if(m_bSee)
		{
			pMap->LeaveRoom(QueryMapThing(), this->IsAlive());	//WITH_BLOCK);
			//goto 更新地图玩家人数
			pMap->AddUserAmount(-1);
			m_bSee = false;
		}

// 		CMsgAction msg;
// 		if (msg.Create(GetID(), 0,0,0, actionLeaveMap))
// 			this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);

//		TeamMemberLeaveMap();
		ClrBroadcastSet();
	}

	m_nStep = STEP_LOGIN;
	return true;
}

//////////////////////////////////////////////////////////////////////
int CUser::SendItemSet()
{
	return SendAllItemInfo();
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendGoodFriend()
{
	SendFriendInfo();
	TutorLoginHint();//师徒数据
	return true;
}

//////////////////////////////////////////////////////////////////////
// fight
//////////////////////////////////////////////////////////////////////
bool CUser::SetAttackTarget(IRole* pTarget /*= NULL*/)
{
	//20070316修罗:把攻击成功判断放到IsAktable()统一管理

	if (!m_pBattleSystem)
		return false;

	if(pTarget == NULL)
	{
		m_pBattleSystem->ResetBattle();
		return true;
	}

//	if(pTarget->QueryOwnerUser() && GetMap()->IsInRegionType(REGION_PK_PROTECTED, GetPosX(), GetPosY()))
//		return false;

	if (!this->IsAtkable(pTarget, true))
		return false;

//	if(pTarget->IsWing() && !this->IsWing() && !(this->IsBowman() || this->IsSimpleMagicAtk()))
//		return false;

	// goal
//	CNpc* pNpc;
//	pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
//	if(pNpc && pNpc->IsGoal() && (GetLev() < pNpc->GetLev() || pNpc->GetType()!=_WEAPONGOAL_NPC))
//		return false;
//
//	if(this->GetDistance(pTarget->QueryMapThing()) > GetAttackRange(pTarget->GetSizeAdd()))
//	{
//		m_pBattleSystem->ResetBattle();
//		return false;
//	}

//	CheckCrime(pTarget);
//
//	if(IsBowman() && !IsArrowPass(pTarget->GetPosX(), pTarget->GetPosY()))
//		return false;

	m_pBattleSystem->CreateBattle(pTarget->GetID());
	CalcFightRate();

	if (QueryMagic())
		QueryMagic()->AbortMagic(true);

	// unequip magic arrow, hard code@@@

	return true;
}

//////////////////////////////////////////////////////////////////////
// precondition is dodge failed
int CUser::Attack( IRole* pTarget,DWORD& dwBattleAddInfoRef )		// return : lose life
{
 	CHECKF(pTarget);
//	CUser* pUserTemp = NULL;
    int nDamage = 0;

	CNpc* pNpc = NULL;
	pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
	if(pNpc)
	{
		LogSave("***%u ATT NPC(%u)***",GetID(),pTarget->GetID());
		return 0;
	}

	//先考虑buff
	CItem* pWeapenSoul = GetEquipItemByPos(ITEMPOSITION_WEAPONSOUL);
	if(pWeapenSoul && pWeapenSoul->IsWeaponSoul() && pWeapenSoul->IsValidEquip())
	{
		CWeaponSoulData* pData = pWeapenSoul->QueryWeapensoulData();
		if(pData)
		{
			stRandBuffData stData;
			pData->ProcGift1003(stData);
			if(stData.nBuff > 0)
			{
				CRole::AttachStatus(QueryRole(), stData.nBuff, stData.nPow, stData.nSec,stData.nTime);
			}
		}
	}


	ATK_TYPE type = ATKTYPE_PHYSICAL_COMMON;//物理普通攻击
	nDamage = CBattleSystem::CalcDamage( dwBattleAddInfoRef,type,QueryRole(),pTarget );
	CUser *pUser;
	pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
	if ( nDamage > 0 )
	{
		if (GetSp() < GetMaxSp())
		{
			this->AddAttrib( _USERATTRIB_SP,2,SYNCHRO_TRUE );
		}

		int nLoseLife = ::CutOverflow(nDamage, (int)pTarget->GetLife());
		
		if (pUser)
		{
			nLoseLife = pUser->PVPEmendDamage(nLoseLife, GetID());
		}
		if (nLoseLife > 0 /*&& pTarget->IsMonster()*/)
		{
			pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLoseLife, SYNCHRO_TRUE);
//modify code by 林德渊 begin 2011.6.22
			IStatus* pStatus = pTarget->QueryStatus(STATUS_RETORT_LGWS);
			if(pStatus)
			{
				this->AddAttrib(_USERATTRIB_LIFE, -1*nLoseLife, SYNCHRO_TRUE);
				this->BeAttack(false,pTarget,nLoseLife,0,true);
				if( !this->IsAlive() && this->QueryMagic() )
					pTarget->Kill(this, INTERACT_ATTACK);
			}
//modify code by 林德渊 end
		}
	
		pTarget->BeAttack( false,this,nLoseLife );
		
	}	
	CGameMap* pMap = this->GetMap();
	if (pMap)
	{	


// 		if (pUser!=NULL&& !pUser->IsEvil() && !this->IsEvil()&& !pMap->IsPkFieldNoPkValue())	
// 		{
// 			CSyndicate * thisSyn = this->GetSyndicate();
// 			CSyndicate * userSyn = pUser->GetSyndicate();
// 			if(!thisSyn || !userSyn || thisSyn->GetState() != 2 || userSyn->GetState() != 2 || thisSyn->GetWarSynID() != userSyn->GetID())
// 			{
// 				if (!QueryStatus(STATUS_CRIME))
// 					this->SetCrimeStatus();
// 			}			
// 		}

//		this->DecEquipmentDurability(ATTACK_TIME, IsSimpleMagicAtk(),/*10 */2);
	}
	return nDamage;
}

//////////////////////////////////////////////////////////////////////
//06.3.6王玉波修改
//此函数为玩家被攻击的处理
bool CUser::BeAttack(bool bMagic, IRole* pTarget, int nLifeLost,int addi/* = 0*/, bool bReflectEnable/*=true*/)
{
	CHECKF(pTarget);

	if (QueryStatus(STATUS_LURK))
		CRole::DetachStatus( QueryRole(),STATUS_LURK );		//解除潜行状态
	if (QueryStatus(STATUS_SLEEP))
		CRole::DetachStatus( QueryRole(),STATUS_SLEEP );	//解除战斗不能状态

	StopMine();

	if (GetSp() < GetMaxSp())
	{
		this->AddAttrib( _USERATTRIB_SP,1,SYNCHRO_TRUE );
	}


	CUser *pUser = NULL;
	if( pTarget->QueryObj(OBJ_USER, IPP_OF( pUser ) ) )
	{
		if( !bMagic && !pUser->IsBowman()) //非近战物理攻击.....
		{
			IStatus* pStatus = QueryStatus( STATUS_RETORT ); //反击
			if (pStatus)			
				CRole::AttachStatus(pUser->QueryRole(), STATUS_SLEEP,0, pStatus->GetPower());
			pStatus = QueryStatus( STATUS_FROSTSHIELD );	//寒霜甲
			if( pStatus )
			{
				CRole::AttachStatus( pUser->QueryRole(),STATUS_MOVESPEEDDOWN_PERCENT,FROSTSHIELD_DECSPEEDPERCENT,FROSTSHIELD_DECSPEEDTIME );
			}
			// 被动技能 反弹伤害 [1/31/2008 陈剑飞]------------------------------------------------------------
// 			IMagicData* pData=QueryMagic(MAGIC_SHFT);
// 			if( pData )
// 			{
// 				int nPercent = pData->GetInt( MAGICDATA_PERCENT );
// 				if( RandGet(100) <= nPercent )
// 				{
// 					CRole::AttachStatus( pUser->QueryRole(),STATUS_DOT_LIFEMISSING,nLifeLost,1,1 );
// 				}
// 			}
			//------------------------------------------------------------------
		}
	}
	// equipment durability cost
// 	if (!this->GetMap()->IsTrainMap())
// 	{
// 		this->DecEquipmentDurability( BEATTACK_TIME, bMagic,( nLifeLost > this->GetMaxLife() / 4 ) ? /*10*/2 : 1 );
// 	}
	
	if( nLifeLost > 0 )
	{
		BroadcastTeamLife();
	}

	//goto ??给玩家加仇恨??不理解
// 	CMonster* pKillerMonster= NULL;
// 	if(pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pKillerMonster)))
// 	{
// 		if(pKillerMonster->IsEudemon())
// 		{
// 			AddEnmity(pKillerMonster->GetID(),
// 				nLifeLost);
// 		}
// 	}
	
	//20070209修罗:玩家仇恨表
	//只记录玩家伤害
	//加入时间戳
	if(pUser)//前面已经取得过.如果攻击者是玩家都记录
	{
		//20080104修罗:以函数封装以下
		//goto ??给玩家加仇恨??不理解
// 		AddEnmity(pUser->GetID(),
// 			nLifeLost);

//		DWORD curTime = ::TimeGet();
//		
//		int amount = m_vEnmity.size();
//		for(int i = 0;i< amount;i++)		//在仇恨列表里找曾经伤害过他的人，累计
//		{
//			if(m_vEnmity[i].userID == pTarget->GetID())
//			{
//				m_vEnmity[i].enmity += nLifeLost;
//				m_vEnmity[i].lastUpdateTime = curTime;//时间戳
//				break;
//			}	
//		}
//		if(i == amount)		//一个新的敌人，添加
//		{
//			ENMITY enmity;
//			enmity.userID = pTarget->GetID();
//			enmity.enmity = nLifeLost;
//			enmity.lastUpdateTime = curTime;//时间戳
//			m_vEnmity.push_back(enmity);
//#ifdef _DEBUG
//			//考虑到size大小
//			const int _warningSize = 20;
//			int enmitySize = m_vEnmity.size();
//			if(enmitySize > _warningSize)
//			{
//				LOGERROR( "玩家仇恨表超长警告,被攻击者:%s enmitySize:%d",
//					this->GetName(),enmitySize );
//			}
//#endif
//		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
//  [7/10/2007] 王玉波第一次修改
//	增加参数：DWORD dwBattleAddInfo，战斗附加信息
void CUser::SendDamageMsg( OBJID idTarget, int nDamage,DWORD dwBattleAddInfo )
{
//  [7/10/2007]------------begin---------------------------------------------------------
//	OBJID idMagicType = ID_NONE;
//	if(QueryTransformation())
//		idMagicType = QueryTransformation()->GetMagicType();
//
//	if(idMagicType != ID_NONE)
//	{
//		CMsgMagicEffect	msg;
//		IF_OK(msg.Create(GetID(), idMagicType, 0, idTarget, nDamage, GetDir()))
//			BroadcastRoomMsg(&msg, INCLUDE_SELF);
//	}
/*	else*/ 
//  [7/10/2007]------------end-----王玉波注释--------------------------------------------
	if(IsBowman())
	{
		CMsgInteract msg;
		IF_OK(msg.Create2(INTERACT_SHOOT, GetID(), idTarget, GetPosX(), GetPosY(), nDamage,dwBattleAddInfo))
			BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
	else
	{
		CMsgInteract msg;		
		IF_OK(msg.Create2(INTERACT_ATTACK, GetID(), idTarget, GetPosX(), GetPosY(), nDamage,dwBattleAddInfo))
			BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
}

//////////////////////////////////////////////////////////////////////
bool CUser::BroadcastTeamLife(bool bMaxLife)
{
	// 2003.2.13 如果对方是玩家而且组队了。被攻击玩家去血值要在队内广播。
	CTeam* pTeam	= GetTeam();
	if (pTeam && pTeam->GetMemberAmount() > 1)
	{
		pTeam->BroadcastMemberLife(this, bMaxLife);
		return true;
	}
	return false;
}
bool CUser::BroadcastTeamMana(bool bMaxMana)
{
	// 2006.12.08 如果对方是玩家而且组队了。被攻击玩家去血值要在队内广播。
	CTeam* pTeam	= GetTeam();
	if (pTeam && pTeam->GetMemberAmount() > 1)
	{
		pTeam->BroadcastMemberMana(this, bMaxMana);
		return true;
	}
	return false;
}

bool CUser::BroadcastTeamLev(bool bLev)
{
	CTeam* pTeam	= GetTeam();
	if (pTeam && pTeam->GetMemberAmount() > 1)
	{
		pTeam->BroadcastMemberLev(this, bLev);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DecEquipmentDurability(bool bBeAttack, bool bMagic, int bDurValue/*=1*/)
{
	//[2009.08.13]有几率不掉耐久
	if (::RandGet(100) >= 15)
	{
		return true;
	}
	int nInc = -1 * bDurValue;
	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{		
		if (!bMagic)
		{
			if (i == ITEMPOSITION_RINGT || 
					i == ITEMPOSITION_RINGB ||
						i == ITEMPOSITION_SHOES ||
							i == ITEMPOSITION_WEAPONR )
			{
				if(!bBeAttack)
					AddEquipmentDurability(i, nInc);
			}
			else
			{
				if(bBeAttack)
					AddEquipmentDurability(i, nInc);
			}
		}
		else
		{
			if (i == ITEMPOSITION_RINGT || 
					i == ITEMPOSITION_RINGB ||
						i == ITEMPOSITION_SHOES ||
							i == ITEMPOSITION_WEAPONR )
			{
				if(!bBeAttack)
					AddEquipmentDurability(i, nInc);
			}
			else
			{
				if(bBeAttack)
					AddEquipmentDurability(i, nInc);
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::MagicAttack(int nType, OBJID idTarget, int x, int y,bool needLearn,int learnLev)
{
	if (QueryMagic())
		return QueryMagic()->MagicAttack(nType, idTarget, x, y,0,needLearn,learnLev);
	return false;
}

//////////////////////////////////////////////////////////////////////
int	CUser::CalcExpLostOfDeath (IRole* pKiller)
{
	return 0;
//	CUser* pUserKiller = NULL;
//	if (!pKiller->QueryObj(OBJ_USER, IPP_OF(pUserKiller)))
//		return 0;
//
//	// normal condition
//	int nParam = 50;
//	if (this->GetEffect()&KEEPEFFECT_RED)
//		nParam = 10;
//	else if (this->GetEffect()&KEEPEFFECT_BLACK)
//		nParam = 5;
//	else
//		nParam = 50;
//
//	int nExpLost = __max(0, this->GetExp()/nParam);
//
//	//if (true)
//	//	return nExpLost;
//
//	// syndicate member && not the killer wanted guy
//	if (this->GetSynID() != ID_NONE && !pUserKiller->IsMyWantedTarget(this))
//	{
//		int nDecPercent = 0;
//		CSyndicate* pSyn = this->GetSyndicate();
//		IF_OK (pSyn)
//		{
//			int nExpPayBySyn = 0;
//			int nProffer = this->QuerySynProffer();
//			if (nProffer > 0)
//			{			
//				int nRank = this->QuerySynRank();
////				switch(nRank)
////				{
////				case RANK_LEADER:
////					nDecPercent = 80;
////					break;
////
////				case RANK_SUBLEADER:
////					nDecPercent = 60;
////					break;
////				case RANK_TEAMLEADER:
////					nDecPercent = 40;
////					break;
////				default:
////					nDecPercent = 25;
////					break;
////				}
//	
//
//				CONST MONEYCOST_PEREXP = 100;
//				int nFundSyn  = pSyn->GetInt(SYNDATA_MONEY);
//				int nFundLost = ::MulDiv(nExpLost, nDecPercent, 100*MONEYCOST_PEREXP);	// 100点经验1两
//				if (nFundLost > nFundSyn)
//					nFundLost = nFundSyn;
//
//				pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1*nFundLost, true);
//
//				if (nRank != RANK_LEADER)
//					this->SetSynProffer(nProffer-nFundLost);
//
//				nExpPayBySyn = nFundLost*MONEYCOST_PEREXP;
//				nExpLost -= nExpPayBySyn;//nFundLost*MONEYCOST_PEREXP;
//			}
//			else
//			{
//				if (pSyn->IsWhiteSynMapOwner() || pSyn->IsBlackSynMapOwner())
//				{
//					nDecPercent = 20;
//				}
//				else
//				{
//					nDecPercent = 10;
//				}
//
//				CONST MONEYCOST_PEREXP = 100;
//				int nFundSyn  = pSyn->GetInt(SYNDATA_MONEY);
//				int nFundLost = ::MulDiv(nExpLost, nDecPercent, 100*MONEYCOST_PEREXP);	// 100点经验1两
//				if (nFundLost > nFundSyn)
//					nFundLost = nFundSyn;
//
//				pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1*nFundLost, true);
//
//				if (this->QuerySynRank() != RANK_LEADER)
//					this->SetSynProffer(nProffer-nFundLost);
//
//				nExpPayBySyn = nFundLost*MONEYCOST_PEREXP;
//				nExpLost -= nExpPayBySyn;//nFundLost*MONEYCOST_PEREXP;
//			}
//
//			this->SendSysMsg(_TXTATR_NORMAL, EXPLOST_PAYBY_SYNFUND, nExpPayBySyn);
//		}
//	}
//
//	return __max(0, nExpLost);
}

//////////////////////////////////////////////////////////////////////
void CUser::Kill(IRole* pTarget, DWORD dwDieWay)
{
	CHECK(pTarget);

	USHORT usRemainTime = 0;
	CUser* pUserTemp	= NULL;
	CMonster* pMonsterTemp	= NULL;	
	//竞技场处理
// 	if(IsInArena() && pTarget->QueryObj(OBJ_USER, IPP_OF(pUserTemp)) && m_strFightingTeamName!=pUserTemp->GetFightingName())
// 	{
// 		m_ArenaKillNum++;
// 	}

	if(pTarget->QueryObj(OBJ_USER, IPP_OF(pUserTemp)))
	{
// 		if(pUserTemp->GetYabiaoState() == 1 && GetYabiaoState() == 2)
// 		{
// 			OBJID nAction1 = NpcManager()->GetYabiaoAction(pUserTemp->GetYabiaoRule(),2);
// 			OBJID nAction2 = NpcManager()->GetYabiaoAction(pUserTemp->GetYabiaoRule(),3);
// 			GameAction()->ProcessAction(nAction1, pUserTemp);			
// 			GameAction()->ProcessAction(nAction2, this);
// 			pUserTemp->BeKillBySys();
// 			return;
// 		}
// 		else if(pUserTemp->GetYabiaoState() == 2 && GetYabiaoState() == 1)
// 		{
// 			OBJID nAction = NpcManager()->GetYabiaoAction(pUserTemp->GetYabiaoRule(),4);
// 			GameAction()->ProcessAction(nAction, pUserTemp);
// 			pUserTemp->BeKillBySys();
// 			return;
// 		}
		if(pUserTemp->IsPVPStuts() && pUserTemp->GetPVPTargetID() != GetID())
		{
			if(MapGroup(PID)->GetPVPManger()->findPVP(pUserTemp->GetID()))
				MapGroup(PID)->GetPVPManger()->EndPVP(pUserTemp->GetID(),pUserTemp->GetPVPTargetID(), true);
// 			pUserTemp->SetAttrib(_USERATTRIB_LIFE,1,true);
// 			pUserTemp->DetachBadlyStatus(pUserTemp);
// 			this->DetachBadlyStatus(this);
// 			CRole::AttachStatus(pUserTemp->QueryRole(), STATUS_PK_PROTECT, 0, 3);	
// 			return;
		}
	}

	CMsgInteract msg;
	if(msg.Create( INTERACT_KILL,this->GetID(),pTarget->GetID(),pTarget->GetPosX(),pTarget->GetPosY(),dwDieWay,usRemainTime ) )
		pTarget->BroadcastRoomMsg(&msg, true,EM_SENDNPCFLAG_ENFORCE);
	
	
	///触发被杀事件
// 	ParamList pl;
// 	pl.push_back(CComVariant((long)this));
// 	pl.push_back(CComVariant((long)pTarget));
// 	soltKillPlayer(pl);

	CUser* pTargetUser	= NULL;
	if(!pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
	{		
// 		if(0 == m_dwAntiplugInterval)
// 		{
//			m_dwAntiplugInterval = time(NULL);
			//m_KillMonsterNum++; //开始计时，同时杀怪数量加1
// 		}
// 		else
// 		{
			
// 			if(!isQuestion &&
// 				IsInMapInstance()==false &&///人在副本中不答题
// 				GetAutoFightFlag()==false)///人在自动挂机中不答题
// 			{
// 				int interval = time(NULL)-m_dwAntiplugInterval;
// 				if(interval > GetKillMonsterTimeInterval()/*_ANTIPLUG_INTERVAL_TIME*/)
// 				{
// 					if (m_KillMonsterNum > GetKillMonsterTargetNum())
// 					{	
// 						AntiplugQuestion(1);
// 					}
// 				}
// 			}
//	}
//	m_KillMonsterNum++;
		pTarget->BeKill(this);		// NPC return
//    if (this->IsGM())
//    {
// 	   char szWords[256];
// 	   sprintf(szWords," 时间间隔: %u 还要杀%d只怪 要杀%d怪",time(NULL) - m_dwAntiplugInterval,GetKillMonsterTargetNum()-m_KillMonsterNum,GetKillMonsterTargetNum());
// 	   CMsgTalk msg;
// 	   if (msg.Create(SYSTEM_NAME,this->GetName(),szWords))
// 	   {
// 		   this->SendMsg(&msg);
// 	   }
//   }

		return;
	}
	char  szTalk[512] = "";
	if (!this->GetMap())
	{
		return;
	}
	if(!this->GetMap()->IsPkGameMap() && (this->GetID() != pTarget->GetID()) )
	{
		if (this->IsUser() && pTarget->IsUser())//如果目标为玩家
		{
			//CMsgTalk Tmsg;
			//this->m_KillAccountId = pTarget->GetID();//  [4/11/2007] 哈雷 记录杀死的玩家ID
			DEBUG_TRY
				ProcessPk(pTargetUser);
			DEBUG_CATCH("ProcessPk(pTargetUser);");
			
			DEBUG_TRY
				pTarget->SendSysMsg("你已经被玩家<font color = '#ff0000'>[%s]</font>杀死了，对方自动添加到你的仇人列表中",this->GetName());
			sprintf(szTalk, "<font color = '#ff0000'>你已经被玩家[%s]杀死了，对方自动添加到你的仇人列表中, 你可以通过好友菜单中的仇人列表来查看更多信息。</font>",this->GetName());
			this->SendEmail(pTargetUser->GetName(), szTalk, pTargetUser);
			DEBUG_CATCH("ProcessPk(pTargetUser)-->SendEmail;");
		}
		if ((!this->IsUser()) && pTarget->IsUser())
		{
			CMsgTalk Tmsg;
			DEBUG_TRY
				this->QueryObj(OBJ_MONSTER, IPP_OF(pMonsterTemp));
			if (pMonsterTemp->IsEudemon())
			{
				ProcessPk(pTargetUser);
				pTarget->SendSysMsg("你已经被玩家<font color = '#ff0000'[%s]</font>的宠物<font color = '#ff0000'>{%s}</font>杀死了，对方自动添加到你的仇人列表中",pMonsterTemp->QueryOwnerUser()->GetName(), pMonsterTemp->GetName());
				sprintf(szTalk, "<font color='#ff0000'>你已经被玩家<font color = '#ff0000'>[%s]</font>杀死了，对方自动添加到你的仇人列表中, 你可以通过好友菜单中的仇人列表来查看更多信息。</font>",this->GetName());
				this->SendEmail(pTargetUser->GetName(), szTalk, pTargetUser);
			}
			DEBUG_CATCH("ProcessPk(pTargetUser);2");
		}
	}
	else
	{
		if (pTarget->IsUser())
		{
			IStatus* pStatus = QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);
			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
			{
				if(GetMap() && GetMap()->IsPkGameMap() && GetMap()==pTarget->GetMap())
					WargameManager()->OnPlayerKill(this, pTarget, pStatus->GetPower());
			}
		}
	}
//	this->m_KillAccountId = pTarget->GetID();//  [4/11/2007] 哈雷 记录杀死的玩家ID
//	ProcessPk(pTargetUser);
	
	// target...
	pTarget->BeKill((IRole*)this);


}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessPk (CUser* pTargetUser)
{
	ASSERT(pTargetUser);
	int nStep = 0;
	CSyndicate * thisSyn = this->GetSyndicate();
	CSyndicate * userSyn = pTargetUser->GetSyndicate();

	// map...
	CGameMap* pMap = MapManager()->QueryMap(this->GetMapID());
	IF_OK (pMap) 
	{
		if (pMap->IsPkField())
		{
		}
		else if (pMap->IsPkGameMap())
		{

		}
		else if (pMap->IsSynMap())
		{
		}
// 		else if (pTargetUser->GetMapID() == MAP_PRISON)//监狱被杀
// 		{
// 			if (pTargetUser->GetPk() > 0)
// 			{
// 				pTargetUser->FlyMap(MAP_PRISON, 93, 94);
// 			}
// 			else
// 			{
// 				//去单人监狱
// 			}
// 		}
		else if(pTargetUser->IsGrayName())
		{
		}
		else if(pTargetUser->IsWhiteName())
		{	
			if(this->IsUser())
			{
				DEBUG_TRY
				nStep = 1;
				int iAdd = 1;
				int difference = (this->GetLev()) - (pTargetUser->GetLev());
				if (difference < 11)
					iAdd = 1;
				else if (difference < 21)
					iAdd = 2;
				else
					iAdd = 3;					
				int tempPK = this->AddPk(PKVALUE_ONEKILL * iAdd);//杀死白名增加PK值

				DEBUG_TRY
				if (!(pTargetUser->GetFriend(GetID()) && pTargetUser->GetFriend(GetID())->GetFriendType()==ENEMY_GROUP))
				{
					if (pTargetUser->AddFriend(GetID(), GetName(), ENEMY_GROUP, GetMate()))//加入仇人列表
					{
						CMsgFriend pMsg;
						pMsg.Create(_FRIEND_ADDSUCCESS,
							GetID(),
							GetName(),
							CMsgFriend::ONLINE_STATUS,
							GetLookFace(),
							GetLev(),
							GetProfession(),
							0,//friendship
							GetSex(),
							ENEMY_GROUP,//frinedgroup
							GetFeel(),
							0);//relation
						pTargetUser->SendMsg(&pMsg);
					}
				}
				DEBUG_CATCH("processPk加入仇人列表出错！");
// 				CMsgUserAttrib msgUser;
// 				if(msgUser.Create(this->GetID(), _USERATTRIB_PK,this->GetPk()))
// 					BroadcastRoomMsg(&msgUser, INCLUDE_SELF);
				if (6 == tempPK)
				{
//					CMsgTalk msg;
//					if (msg.Create("系统", this->GetName(), "<font color='#ff0000'>你已经杀变态了，继续杀人将会被投入监狱！</font>", NULL, 0xfefeefef, _TXTATR_CHATWITH))
//					this->SendMsg(&msg);
//					this->SendSysMsg("你已经杀变态了，继续杀人将会被投入监狱！");
					this->SendEmail(GetName(), "<font color='#ff0000'>你已经杀变态了，继续杀人将会被投入监狱！</font>", this);
				}
				else	if(8 == tempPK)
				{
					this->GotoJail(false);
				}
					//传送监狱相关处理
			//灰名与红名有冲突
			//在AddPk里处理
			DEBUG_CATCH2("ProcessPK step = %d", nStep);
			}
			else if (this->IsMonster())
			{
				DEBUG_TRY
				nStep = 2;
				CMonster *pMonsterTemp = NULL;
				if(this->QueryObj(OBJ_EUDEMON, IPP_OF(pMonsterTemp)) && pMonsterTemp->IsEudemon() && pMonsterTemp->QueryOwnerUser() )
				{
					int iAdd = 1;
					int difference = (this->QueryOwnerUser()->GetLev()) - (pTargetUser->GetLev());
					if (difference < 11)
						iAdd = 1;
					else if (difference < 21)
							iAdd = 2;
					else
						iAdd = 3;					
					int tempPK = this->QueryOwnerUser()->AddPk(PKVALUE_ONEKILL * iAdd);
					if (6 == tempPK)
					{
// 						CMsgTalk msg;
//  					if (msg.Create("系统", this->GetName(), "<font color='#ff0000'>你已经杀变态了，继续杀人将会被投入监狱！</font>", NULL, 0xfefeefef, _TXTATR_MAYBEMAIL))
//  					this->QueryOwnerUser()->SendMsg(&msg);
						this->SendEmail(this->GetName(), "<font color='#ff0000'>你已经杀变态了，继续杀人将会被投入监狱！</font>", this);
					}
					if(8 == tempPK)//杀死白名增加PK值
					{
						if (this->GetMapID() != MAP_PRISON)
						{
//						this->QueryOwnerUser()->SetRecordPos(MAP_PRISON, 93, 94, true);
//						//是否去公共监狱
// 						CMsgAction msg;
// 						msg.Create(0, actionGotojial_Self, 0);
// 						this->QueryOwnerUser()->SendMsg(&msg);
// 						this->QueryOwnerUser()->m_GotoPrison.Startup(30);
// 						if (this->QueryOwnerUser()->m_GotoPrison.TimeOver())
// 						{
							this->QueryOwnerUser()->GotoJail(false);
			/*			}*/
						
						//Sleep(30*1000);
						
						}
					}
				}	//传送监狱相关处理
				DEBUG_CATCH2("ProcessPK step = %d", nStep);
			}
		}
		else if(pTargetUser->IsRedName())
		{
			DEBUG_TRY
			nStep = 3;
			//从BEKILL移到这里，被怪杀死不掉装备
			//	pTargetUser->QueryPackage()->RandDropItem(1, true);//随机掉落一件非绑定物品
	
			
			/*****************新的红名惩罚规则*******************************************/		
			int		nRandItem = 0;
			int		nRandEqu  = 0;
			int		nDropItem = 0;
			int		nDropEqu  = 0;
			char	szDropitem[128] = "";
			char	szDropEqu[128]  = "";
			char	szMsg[256]	= "";
			char	szTalk[512] = "";
			CMsgTalk msg;
			if (pTargetUser->GetPk() > 5)
			{
				nRandItem = RandGet(4);
				nRandEqu  = RandGet(2);
				nDropItem = pTargetUser->QueryPackage()->RandDropItem(nRandItem+1, true);
				nDropEqu  = pTargetUser->RDropEquip(nRandEqu+1);
			}
			else if (pTargetUser->GetPk() >3)
			{
				nRandItem = RandGet(2);
				nRandEqu  = RandGet(2);
				nDropItem = pTargetUser->QueryPackage()->RandDropItem(nRandItem+1, true);
				if (nRandEqu)
				{
					nDropEqu  = pTargetUser->RDropEquip(nRandEqu);
				}
			}
			else
			{
				nRandItem = RandGet(2);
				if (nRandItem)
				{
					nDropItem = pTargetUser->QueryPackage()->RandDropItem(nRandItem, true);
				}
			}
			nStep = 4;
			if(nDropItem)
				sprintf(szDropitem, "%d件物品", nDropItem);
			if(nDropEqu)
				sprintf(szDropEqu, "%d件装备", nDropEqu);
			if (nDropItem || nDropEqu)
			{
				sprintf(szMsg, "你丢失了%s%s", szDropitem, szDropEqu);
				pTargetUser->SendSysMsg(szMsg);
				pTargetUser->SendEmail(pTargetUser->GetName(), szMsg, pTargetUser);
//				sprintf(szTalk, "<font color='#ff0000'>你被玩家%s杀死了，由于杀气值的惩罚，%s</font>", this->GetName(), szMsg);
//				if (msg.Create("系统", this->GetName(), szTalk, NULL, 0xff0000, _TXTATR_CHATWITH))
//					pTargetUser->QueryOwnerUser()->SendMsg(&msg);
			}
			/****************************************************************************************/							
			
//			pTargetUser->QueryPackage()->RandDropItem(1, true);//随机掉落一件非绑定物品
			nStep = 5;
			if (!(pTargetUser->GetFriend(GetID()) && pTargetUser->GetFriend(GetID())->GetFriendType()==ENEMY_GROUP))
			{
				if (pTargetUser->AddFriend(GetID(), GetName(), ENEMY_GROUP, GetMate()))//加入仇人列表
				{
					CMsgFriend pMsg;
					pMsg.Create(_FRIEND_ADDSUCCESS,
						GetID(),
						GetName(),
						CMsgFriend::ONLINE_STATUS,
						GetLookFace(),
						GetLev(),
						GetProfession(),
						0,//friendship
						GetSex(),
						ENEMY_GROUP,//frinedgroup
						GetFeel(),
						0);//relation
					pTargetUser->SendMsg(&pMsg);
				}
			}
			DEBUG_CATCH2("ProcessPK step = %d", nStep);
			// 		pTargetUser->AddPk(-PKVALUE_ONEKILL);//红名被杀减少PK值
			// 		__int64	nLevExp = 0;	// 升级所需经验
			// 		CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
			// 		if (pLevupexp)
			// 		{
			// 			nLevExp = pLevupexp->GetInt(LEVUPEXPDATA_EXP);
			// 		}
			// 		pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
			// 		int nKillerStuExp	= 0;
			// 		if (pLevupexp)
			// 		{
			// 			//	nKillerStuExp	= pLevupexp->GetInt(LEVUPEXPDATA_STU_EXP);			
			// 			int nLossExpPercent = 1;//红名，损失经验的1%
			// 			__int64 nExpGet = (nLevExp) * nLossExpPercent /100;
			// 			if (nExpGet > 0 && (this->GetExp() >nExpGet))
			// //				this->AwardBattleExp(-nExpGet);
			// 				this->AddAttrib(_USERATTRIB_EXP, -nExpGet, SYNCHRO_TRUE);
			// //				this->SendSysMsg("因为你是杀手，所以你的经验减少了1%");
			// 		}
			// 		
			// 	}


		}
	/*	else if(thisSyn && userSyn && thisSyn->GetState() == 2 && userSyn->GetState() == 2 / *&& thisSyn->GetWarSynID() == userSyn->GetID()* /)*/
		{

		}
	/*	else*/
		{
// 			if (!pMap->IsDeadIsland()
// 				//	&& !g_ResourceWar.IsWarPkTime( GetMapID() )			//07.5.12王玉波注释，解决资源战时杀自己种族玩家不红名的BUG
// 				//&& pTargetUser->GetPhyle() == this->GetPhyle()
// 				//&& !pTargetUser->IsEvil()
// 				&& !pMap->IsPkFieldNoPkValue()	//  [2007-9-13] 朱斌 有MAPTYPE_PKFIELD_NO_PK_VALUE属性的地图PK不加PK值
// 				)


		}
		//[游途道标 2009.06.03]人会被冤死,这里加一次仇杀值,下面再加一次.屏蔽掉这里.
		/*else
		{	
			if( !pMap->IsPkFieldNoPkValue())
			{
			    this->AddPk(PKVALUE_ONEKILL);
			}
		}*/
			// innocent kill      //change huang 2004.1.11
//			if (!(pTargetUser->GetEffect()&KEEPEFFECT_RED) 
//					&& !(pTargetUser->GetEffect()&KEEPEFFECT_BLACK))
//			{
//				if (this->GetEffect()&(KEEPEFFECT_BLACK | KEEPEFFECT_RED))   
//					PoliceWanted().AddWanted(this);
//			}

/*			// 新案子没有计算这个损失 --- zlong 2004.10.28
			int nManaLost = pTargetUser->GetMana()/2;
			pTargetUser->AddAttrib(_USERATTRIB_MANA, -1*nManaLost, SYNCHRO_TRUE);		// , &dwDirty
*/

			// pk increase    // tudo 2004.1.11   // 没改
//			if (!pTargetUser->IsEvil() 
//					&& pTargetUser->GetPk() < PKVALUE_REDNAME
//						&& !pMap->IsDeadIsland())	// 死亡岛杀人不闪蓝，不加PK值
						// && pTargetUser->GetExp() >= 0)	// 圣战中取消这个设定

			// pk increase	—— PK白名红名都要加PK值 -- zlong 2005-04-28
			//20070228修罗:PK值增加方式修改
//			if (!pMap->IsDeadIsland()
//			//	&& !g_ResourceWar.IsWarPkTime( GetMapID() )			//07.5.12王玉波注释，解决资源战时杀自己种族玩家不红名的BUG
//			/*	&& pTargetUser->GetPhyle() == this->GetPhyle()*/
//				&& !pTargetUser->IsEvil()
//				&& !pMap->IsPkFieldNoPkValue()	//  [2007-9-13] 朱斌 有MAPTYPE_PKFIELD_NO_PK_VALUE属性的地图PK不加PK值
//				)
//			{
//				if(!(thisSyn && userSyn && thisSyn->GetState() == 2 && userSyn->GetState() == 2 && thisSyn->GetWarSynID() == userSyn->GetID()))
//					this->AddPk(PKVALUE_ONEKILL);
//				//灰名与红名有冲突
//				//在AddPk里处理
//			}
			
//			if (!pMap->IsDeadIsland()
//				&& !g_ResourceWar.IsWarPk(GetMapID())
//				&&	pTargetUser->GetPhyle() == this->GetPhyle())
//			{
//				int nPkValue = PKVALUE_ONEKILL;
//				if(GetPk() > 0)
//				{
//					this->AddPk(PKVALUE_ONEKILL);
//				}
//				else
//				{
//					if(pTargetUser->GetPk() > 0 || pTargetUser->QueryStatus(STATUS_CRIME))
//						this->AddPk(-1 * PKVALUE_ONEKILL);
//					else 
//						this->AddPk(60-GetPk());//PK变为60
//				}

				// syn effect
			//	if (this->IsHostileSynMember(pTargetUser))
			//		nPkValue /= 2;

				// crime
// 				if (!QueryStatus(STATUS_CRIME))
// 				{
// 					CONST CRIME_TIME_PERMURDER = 20;	//2*60;		// 闪蓝时间改为30秒
// 					this->SetCrimeStatus(STATUS_CRIME, CRIME_TIME_PERMURDER);
// 				}

				// 提示 —— zlong 2005-04-28
			/*	if (this->GetPk() < PKVALUE_REDNAME)
					this->SendSysMsg(STR_INCPK_REDNAME_PUNISH);
				else if (this->GetPk() < PKVALUE_BLACKNAME)
					this->SendSysMsg(STR_INCPK_BLACKNAME_PUNISH);
				else
					this->SendSysMsg(STR_INCPK_BLACKNAME_PUNISH2);*/
//			}

			// 奖励经验
		/*	{
				// 根据2004.10.28的案子增加 ---- zlong
				__int64	nLevExp = 0;	// 升级所需经验
				int		nStuExp	= 0;	// 被带最高经验
				CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + pTargetUser->GetLev());
				if (!pLevupexp)
					pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + __max(0, pTargetUser->GetLev()-1));

				if (pLevupexp)
				{
					nLevExp = pLevupexp->GetInt(LEVUPEXPDATA_EXP);
					nStuExp	= pLevupexp->GetInt(LEVUPEXPDATA_STU_EXP);	//nStuExp将被作为分母，不能为0
				}
				pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
				if (!pLevupexp)
					pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + __max(0, this->GetLev()-1));

				int nKillerStuExp	= 0;
				if (pLevupexp)
					nKillerStuExp	= pLevupexp->GetInt(LEVUPEXPDATA_STU_EXP);

				int nLossExpPercent = 0;
				if (pTargetUser->GetPk() < PKVALUE_REDNAME)					// 白名
					nLossExpPercent	= 1;									// 损失升级所需1%经验
				else if (pTargetUser->GetPk() < PKVALUE_BLACKNAME)			// 红名
					nLossExpPercent	= 2;									// 损失升级所需2%经验
				else														// 黑名
					nLossExpPercent	= 3;									// 损失升级所需3%经验

				__int64 nExpGet = (nLevExp) * nLossExpPercent * 70 / 10000;			// 杀人得到对方损失经验的70%
				// 上面的公式修改了, 
				// 杀人的人得到经验=（被杀人损失经验/被杀人被带1次可得最高经验）*杀人的人被带1次可得最高经验*70%
				if (nStuExp == 0)
					nExpGet	= 0;
				else
					nExpGet	= nExpGet * nKillerStuExp / nStuExp;
				
				if (nExpGet > 0)
					this->AwardBattleExp(nExpGet);
	//王玉波暂时注释..
	
			}*/

			// wanted kill
//			if (this->IsMyWantedTarget(pTargetUser))
//			{
//				OBJID idWanted = this->WantedInfo().idWanted;
//				CWantedData* pData = CWantedList::s_WantedList.GetWanted(idWanted);
//
//				// award bonuty!
//				int nBonuty = 0;
//				if (pData)
//					nBonuty = pData->GetInt(DATA_BOUNTY);
//
//				this->AddAttrib(_USERATTRIB_MONEY, nBonuty*(100-_BONUTY_TAX)/100, SYNCHRO_TRUE);
//				
//				// del wanted from list
//				CWantedList::s_WantedList.DelWanted(idWanted);
//				
//				// reset my wanted info
//				this->ResetWantedInfo();
//				
//				// inform client
//				CMsgName msg;
//				IF_OK_ (msg.Create(NAMEACT_WANTED, ""))
//					this->SendMsg(&msg);
//				
//				// lost of wanted target
//			//	int nLevExp = 0;
//				__int64	i64LevExp = 0;
//				CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
//				if (pLevupexp)
//				//	i64LevExp = pLevupexp->GetInt(LEVUPEXPDATA_EXP);
//					i64LevExp = pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
//				
//			//	int nCurExp = pTargetUser->GetExp();
//				__int64 i64CurExp = pTargetUser->GetExp();	//  [11/14/2007 朱斌]
//			//	int nExpLost = __min(nBonuty, i64LevExp);
//				__int64 i64ExpLost = __min(nBonuty, i64LevExp);
//				
//				if (i64CurExp >= 0)				
//				{
//					if (i64CurExp - i64ExpLost <= 0)
//						i64ExpLost = i64CurExp + (i64ExpLost-i64CurExp)/2;
//				}
//				else
//				{
//					int nFactor = (-1*i64CurExp/i64LevExp + 1)*2;
//					i64ExpLost = i64ExpLost/nFactor;
//				}
//				
//				pTargetUser->AddAttrib(_USERATTRIB_EXP, -1*i64ExpLost, SYNCHRO_TRUE);
//			}
//
//			// police wanted kill
//			if (pTargetUser->GetID() == this->PoliceWantedID())
//			{
//				this->PoliceWantedID() = ID_NONE;
//
//				int nKillAward = __min(-1, -1*(int(pTargetUser->GetLev())-40)/10);
//				this->AddAttrib(_USERATTRIB_PK, nKillAward, SYNCHRO_TRUE);
//			}
//			else
//			{
				//20070320修罗:屏蔽.代码本身有误.功能亦不明
				// team hunter
//				CTeam* pTeam = this->GetTeam();
//				if (pTeam)
//				{
//					CUser* pLeader = UserManager()->GetUser(pTeam->GetLeader());
//					if (pLeader)
//					{
//						if (pLeader->PoliceWantedID() == pTargetUser->GetID())
//							pLeader->TeamHuntKill(pTargetUser->GetID(), pTargetUser->GetLev(), pTargetUser->GetName());
//					}
//					else	// in other thread?
//					{
//						char buf[1024] = {0};
//						REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
//						pInfo->ucFuncType	= REMOTE_CALL_TEAM_HUNTKILL;
//						pInfo->idUser		= pTeam->GetLeader();
//						pInfo->IntParam[0]	= pTargetUser->GetID();
//						pInfo->IntParam[1]	= pTargetUser->GetLev();
//						//20070320修罗:以下是一个为0的int.转成char *一定非法.
//						//team Hunter功能不使用.直接屏蔽.
//						strcpy((char*)(pInfo->IntParam[2]), pTargetUser->GetName());
//						pInfo->nSize		= sizeof(REMOTE_CALL0) + 2*sizeof(int)+strlen(pTargetUser->GetName())+1;
//						MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
//					}
//				}
//			}
//		}
	}
}

//////////////////////////////////////////////////////////////////////
void CUser::BeKillByDot(OBJID idAtker)
{		
	CUser* pAtker = UserManager()->GetUser(idAtker);
	if (pAtker)
		pAtker->Kill(this, DIE_NORMAL);
	else
	{
		BeKill(NULL);
		CMsgInteract msg;
		if(msg.Create( INTERACT_KILL,idAtker,this->GetID(),this->GetPosX(),this->GetPosY(),DIE_NORMAL,0 ) )
			this->BroadcastRoomMsg(&msg, true,EM_SENDNPCFLAG_ENFORCE);
	}
}
void CUser::BeKillBySys()
{
//	EudemonDead(3);
	int nLife = this->GetLife();
	this->AddAttrib(_USERATTRIB_LIFE, -1*nLife, SYNCHRO_FALSE);
//modify code by 林德渊 begin 2011.6.22
	EudemonDead();
	if(m_idSoulEudemon)//解除宠物附体
		RemoveEudemonSoul();
//modify code by 林德渊 end
	this->SetStatus(STATUS_DIE,true);

	CMsgInteract msg;
	if(msg.Create( INTERACT_KILL,this->GetID(),this->GetID(),this->GetPosX(),this->GetPosY(),DIE_NORMAL,0 ) )
		this->BroadcastRoomMsg(&msg, true,EM_SENDNPCFLAG_TRUE);

//	UpdateCalculatedProperty(UPDATECALTYPE_ALL);
	ClrAttackTarget();
	if (QueryMagic())
		QueryMagic()->AbortMagic(true);
//modify code by 林德渊 begin 2011.6.22
	OBJID objmapid = this->GetRecordMapID();
	OBJID objx = this->GetRecordPosX();
	OBJID objy = this->GetRecordPosY();
//modify code by 林德渊 end
	CGameMap* pMap = this->GetMap();
	if( !pMap )
		return;
	OBJID	idRebornMap = ID_NONE;
	POINT	pos;

	if( IsOwnMap( pMap->GetID() ) || pMap->IsPrisonMap() )		
	{
		IF_OK( pMap->GetRebornMap( &idRebornMap, &pos ) )	
			this->SetRecordPos(idRebornMap, pos.x, pos.y);
	}
	else				
	{
		//[游途道标 2009.05.08]复活点坐标
		idRebornMap = 1001;
		pos.x = 78;
		pos.y = 87;
		this->SetRecordPos(idRebornMap, pos.x, pos.y);
	}
//modify code by 林德渊 begin 2011.6.22
	if( pMap->GetDocID() == 2016 || pMap->GetDocID() == 2018 )//绝归岛自杀死回开封
		this->SetRecordPos(objmapid, objx, objy);
//modify code by 林德渊 end
	pMap->DecRole(this->GetPosX(), this->GetPosY());
}
//////////////////////////////////////////////////////////////////////
void CUser::BeKill(IRole* pRole /*= NULL*/)
{
 	EudemonDead();
	if(m_idSoulEudemon)//解除宠物附体
		RemoveEudemonSoul();

	CGameMap* pMap = this->GetMap();
	CHECK(pMap);

	if(!pMap->IsPkGameMap())
	{
		//武魂生命扣10点
		CItem* pEquip = GetEquipItemByPos(ITEMPOSITION_WEAPONSOUL);
		if(pEquip && pEquip->IsValidEquip(this))
		{
			CWeaponSoulData* pData = pEquip->QueryWeapensoulData();
			if(pData)
			{
				int nOldLifespan = pData->GetInt(WEAPONSOULDATA_LIFESPAN);
				if(nOldLifespan >= 5)
					pData->SetInt(WEAPONSOULDATA_LIFESPAN,nOldLifespan - 5);
			}
		}
	}

	if( GetMountState() )
	{
		CRole::DetachStatus((IRole*)this,STATUS_MOUNTSPEED_NUMBER);
		SetUnMount();
// 		CMsgPlayer msgPlayer;// 发送消息到客户端更新 [陈剑飞8/25/2007]
// 		if (msgPlayer.Create(this->QueryRole()))
// 			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		
		SetAttrib(_USERATTRIB_SPEED, AdjustSpeed(GetSpeed()), SYNCHRO_TRUE);
	}
	if (GetItemByType(_SPCITEM_MONEYFORSHOP))//如果在跑商，则复活后10分钟内不能继续跑
	{
		ProcessAction(10032);
	}
// 
// 	if(MapGroup(PID)->GetPVPManger()->findPVP(GetID()))
// 		MapGroup(PID)->GetPVPManger()->EndPVP(GetID());
// 
// 	if (GetYabiaoState() == 1)
// 	{
// 		OBJID nAction = NpcManager()->GetYabiaoAction(GetYabiaoRule(),2);
// 		GameAction()->ProcessAction(nAction, this);
// 	}
// 	if (GetYabiaoState() == 2)
// 	{
// 		OBJID nAction = NpcManager()->GetYabiaoAction(GetYabiaoRule(),4);
// 		GameAction()->ProcessAction(nAction, this);
// 	}

	bool bDropItem	= true;
	bool bDropExp	= true;

	this->SetStatus(STATUS_DIE,true);

	UpdateCalculatedProperty(UPDATECALTYPE_ALL);
	ClrAttackTarget();					//清除锁定攻击目标
	if (QueryMagic())
		QueryMagic()->AbortMagic(true);

	m_bGhost	= false;

	OBJID	idRebornMap = ID_NONE;
	POINT	pos;

	if (pMap->IsPkGameMap() && !CWargame::IsWarGameMap(GetMapID()))
	{
		idRebornMap = 1006;
		pos.x = 153;
		pos.y = 108;
		this->SetRecordPos(idRebornMap, pos.x, pos.y);
	}
	if( this->IsEvil() && (this->GetMapID() == MAP_PRISON) )		//本族地图或者监狱..死亡根据地图出生点复活,
	{
		idRebornMap = MAP_PRISON;
		pos.x = 30;
		pos.y = 25;
		this->SetRecordPos(idRebornMap, pos.x, pos.y);
	}
	else
	{
// 		idRebornMap = 1026;
// 	    pos.x = 34;
// 	    pos.y = 31;
// 		this->SetRecordPos(idRebornMap, pos.x, pos.y);
	}
	// 因为打开了WITH_BLOCK开关，为了在死亡后尸体允许穿透，需要减地图上的角色数
	pMap->DecRole(this->GetPosX(), this->GetPosY());

// 	if( IsOwnMap( pMap->GetID() ) || this->GetMapID() == MAP_PRISON )		//本族地图或者监狱..死亡根据地图出生点复活,
// 	{
// 		FlyMap(MAP_PRISON, 93,93);
// 	}
	return;

// 	if (pMap->IsPrisonMap())	//在监狱地图死亡,没有惩罚(不掉经验,不掉物钱,不掉物品)
// 	{
// 		return;
// 	}

	if(!pRole)	
		return ;
}

/////////////////////////////////////////////////////////////////////////////
//	void CUser::AwardBattleExp(int nExp, bool bGemEffect/*=true*/, bool bIncludeOwner/*=true*/)
DWORD CUser::AwardBattleExp(__int64 i64Exp, bool bGemEffect/*=true*/, bool bIncludeOwner/*=true*/, bool bTask)
{
 	//AwardEudemonExp(i64Exp);

	if (this->isFatigue())
	{
		if (this->isFiftyP())
		{
			i64Exp = i64Exp / 2;
		}
		if (this->isZeroP())
			return 0;
	}
	//1.加经验
	if (i64Exp == 0)
		return 0;
	
	if (i64Exp < 0)
	{
		this->AddAttrib(_USERATTRIB_EXP, i64Exp, SYNCHRO_TRUE);
		return 0;
	}
	
	CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
	if (!pLevupexp)
		return 0;
	__int64 i64LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);

	if(this->GetExp() >= i64LevupExp * 4)
	{
		if(this->GetLev() < USERMAXLEVEL)
			SendSysMsg(_TXTATR_TOPSHOW,"您的经验已满，请手动升级");
		return 0;
	}
	
	//2.11级前人物升级
	if(this->GetLev() < 10)
	{
		__int64 i64NewExp = i64Exp + this->GetExp();
		if (i64NewExp >= i64LevupExp)
		{
			int nAddLevel		= 0;
			__int64 i64TotalExp		= i64NewExp;
			__int64 i64NextLevNeedExp = i64LevupExp;	
			while( i64TotalExp >= i64NextLevNeedExp )
			{
				nAddLevel++;
				i64TotalExp -= i64NextLevNeedExp;
				CLevupexpData* pNextLevData= LevupexpSet()->GetObj( EXP_TYPE_USER * _EXP_TYPE + this->GetLev() + nAddLevel );
				IF_NOT(pNextLevData)
					break;

				if(this->GetLev() + nAddLevel >= 10)
					break;
				
				i64NextLevNeedExp	= pNextLevData->GetInt64( LEVUPEXPDATA_EXP );	
			}
			CHECKF( nAddLevel >= 1 );
			this->IncLev( nAddLevel );

			pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
			if (!pLevupexp)
			{
				this->SetAttrib( _USERATTRIB_EXP,0,SYNCHRO_TRUE );
				return i64Exp;
			}
			i64LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);

			if(i64TotalExp >= i64LevupExp * 4)
			{
				this->SetAttrib( _USERATTRIB_EXP,i64LevupExp * 4,SYNCHRO_TRUE );
			}
			else
			{
				this->SetAttrib( _USERATTRIB_EXP,i64TotalExp,SYNCHRO_TRUE );
			}
			
			return i64Exp;
		}	
	}

	if(this->GetExp() + i64Exp >= i64LevupExp * 4)
	{
		i64Exp = i64LevupExp * 4 - this->GetExp();
	}

	this->AddAttrib(_USERATTRIB_EXP,i64Exp,SYNCHRO_TRUE);
	return i64Exp;
}

////////////////////////////////////////////////////////////////////// 
bool CUser::IsAtkable(IRole* pTarget, bool bSendHint)
{
	//return true;
	if (!pTarget)//群攻技能可能没有目标
	{
// 		if (bSendHint)
// 			this->SendSysMsg(/*g_objStrRes.GetStr( 15037 )*/"错误的对象:目标为空");//"错误的对象:目标为空"
		return false;
	}

	//20070315修罗:将IsPkEnable()中的判断拿过来
	CMonster*	pEudemon = NULL;
	IRole* pRealTarget = pTarget;
	if(pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pEudemon)))
	{
		if(pEudemon && pEudemon->IsEudemon())
		{
			pTarget = pEudemon->QueryOwnerUser();
		}
		if(pEudemon && pEudemon->IsMonster())
		{
			if (pEudemon->GetType() == 9999)//打决斗旗这么无聊？
			{
				return false;
			}
		}
	}
	if(this->IsMonster())
	{
		if (this->QueryOwnerUser()==pTarget)
			return false;
	}
	if(!pRealTarget || !pTarget)
		return false;
	if (this->GetID() == pRealTarget->GetID())
	{
		return false;
	}
	if (this->GetID() == pTarget->GetID())
	{
// 		if (bSendHint)
// 			this->SendSysMsg(g_objStrRes.GetStr( 15045));//"错误的对象:你不能攻击自己"
		return false;
	}
//以下为PK判定
	if(pTarget->IsUser())		
	{		
		if(this->IsPVPStuts() && this->GetPVPTargetID() == pTarget->GetID()) 
			return true;
//		if ((this->GetMapID() == 1001) || (this->GetMapID() == 1006) ||(this->GetMapID() == MAP_PRISON))
		if (!this->GetMap())
			return false;
		if(this->GetMap()->isDisablePkill())
		{
//			this->SendSysMsg("身处江湖，就得遵守江湖规矩");//STR_NO_PK_VILLAGE
			return false;
		}
		//1)组队成员不能被攻击 
		CTeam* tTeam = GetTeam();
		if(tTeam)
		{
			if(tTeam->IsTeamMember(pTarget->GetID()))
			{
			//	this->SendSysMsg("你不能攻击你的队友");
				return false;
			}
		}
		
		if (!this->GetMap())
		{
			return false;
		}
		if (this->GetMap()->IsPkGameMap())
		{
			//竞技场
			IStatus* pStatus = QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);
			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
			{
				if(WargameManager()->GetCurrentStatus(pStatus->GetPower()) != WARGAME_STATUS_WORK)
					return false;

				if(pStatus->GetParam1() == pStatusTarget->GetParam1())//同阵营
					return false;
			}

			return true;
		}
		//2) 同盟或是同帮派攻击
		CUser* pUser = NULL;
		if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)) && this->GetSynID() != ID_NONE)
		{				
			if(this->IsSynProcAble()){
				if(this->GetSynID() == pTarget->GetSynID())
					return false;
			}
		}
// 			CSyndicate * thisSyn = this->GetSyndicate();
// 			if(thisSyn && thisSyn->GetState() == 2 /*&& thisSyn->GetWarSynID() == pUser->GetSynID()*/)
// 			{
// 				return true;
// 			}
// 		}
		
		
		
		//3)红名保护
		if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
		{	
#ifdef _DEBUG
			char szName[128];
			strcpy(szName,pUser->GetName());
#endif // _DEBUG
			
// 			int nDoMainID = this->GetCityWarDoMain();  //城战时没有PK保护
// 			if(nDoMainID && nDoMainID == pUser->GetCityWarDoMain())
// 			{
// 			}
// 			else if(0 != this->GetSynWarJoinID() && 0 != pUser->GetSynWarJoinID())//帮战时没有PK保护
// 			{
// 			}
// 			else
			{
				if(pUser->IsEvil())
				{
					if(this->IsRedProcAble())
					{
						return false;
					}
				}
				else	//4)白名保护
				{
					if(this->IsWhileProcAble())
						return false;	
				}
			}
		}
//20级以下玩家不能被攻击
		if ( pTarget->GetLev() < 20)
		{
			if (this->IsUser() || this->QueryOwnerUser())
				return false;
		}

		// 		CUser *YabiaoUser = NULL;
		// 		if(pTarget->QueryObj(OBJ_USER, IPP_OF(YabiaoUser)))
		// 		{
		// 			int satate1 = this->GetYabiaoState();
		// 			int satate2 = YabiaoUser->GetYabiaoState();
		// 			if(satate1 == 1 && satate2 == 2)
		// 			{
		// 				return true;
		// 			}
		// 			if(satate1 == 2 && satate2 == 1)
		// 			{
		// 				return true;
		// 			}
		// 		}
		
		// 		if(IsInArena())
		// 		{
		// 			CUser* pTempUser;
		// 			if(pTarget->QueryObj(OBJ_USER, IPP_OF(pTempUser)))
		// 			{
		// 				if(pTempUser->m_strFightingTeamName==m_strFightingTeamName)
		// 					return false;
		// 				IStatus* pStatus = pTempUser->QueryStatus(STATUS_PK_PROTECT);
		// 				if(pStatus)
		// 					return false;
		// 				if(!pTempUser->IsAlive())
		// 					return false;
		// 				else
		// 					return true;
		// 			}
		// 		}
		
		// 		IStatus* pStatus = QueryStatus(STATUS_CITYWAR_WEAPON);
		// 		if( pStatus )
		// 		{
		// 			this->SendSysMsg(g_objStrRes.GetStr(28043));//("您拥有城战武器状态，不能攻击普通玩家");
		// 			return false;
		// 		}
	}

	
	// 暂时限制坐骑不能进行攻击 [陈剑飞8/17/2007]
//	if( CUser::GetEquipItemByPos( ITEMPOSITION_MOUNT ) )
//	{
//		CItem* pItem = GetEquipItemByPos(ITEMPOSITION_MOUNT);
//		if(pItem != NULL)
//		{
//			int nMountID = pItem->GetTypeID();
//			CItemTypeData* pType = ItemType()->QueryItemType(nMountID);
//			if (pType != NULL && pType->GetInt(ITEMTYPEDATA_LUCK) == 1)
//			{
//				this->SendSysMsg(g_objStrRes.GetStr( 16020));//"该功能暂时不开放!"
//				return false;
//			}
//		}
//
//	
//	}
// 	CUser *cityUser = NULL;
// 	if(pTarget->QueryObj(OBJ_USER, IPP_OF(cityUser)))
// 	{
// 		IStatus* pStatus = pTarget->QueryStatus( STATUS_CITYWAR_SAFETIME );
//         if(pStatus)
// 		{
// 			this->SendSysMsg("对方处在城战进入安全时间");
// 			return false;
// 		}
// 		int nDoMainID = this->GetCityWarDoMain();  //城战时没有新手保护
// 		if(nDoMainID && nDoMainID == cityUser->GetCityWarDoMain())
// 		{
// 		}
// 		else if(SYNWARTYPE_NONE != GetSynWarJoinID() && SYNWARTYPE_NONE != cityUser->GetSynWarJoinID())
// 		{
// 		}
// 		else //攻击方20以下,不可攻击玩家
// 		{	
// 			const int PKLEVEL_LITMIT = 30;
// 			if(/*pTarget->IsUser() && */this->GetLev()<PKLEVEL_LITMIT)
// 			{
// 				if (bSendHint)
// 				{
// 					this->SendSysMsg(g_objStrRes.GetStr( 15039),PKLEVEL_LITMIT);
// 				}
// 				return false;
// 			}
// 			
// 			//被攻击方是玩家,20级以下,不可以被攻击
// 			if(/*pTarget->IsUser() && */cityUser->GetLev()<PKLEVEL_LITMIT)
// 			{
// 				if (bSendHint)
// 				{
// 					this->SendSysMsg(g_objStrRes.GetStr( 15040),PKLEVEL_LITMIT);
// 				}
// 				return false;
// 			}
// 		}
/*	}*/

	//if(pTarget->QueryOwnerUser() && GetMap()->IsInRegionType(REGION_PK_PROTECTED, GetPosX(), GetPosY()))
	//目标是玩家,且目标在安全区....或....自己在安全区....或禁止PK地图.


	//安全区域处理 satan******************************
// 	if(	((pTarget->IsUser() && pTarget->GetMap()->IsInRegionType(REGION_PK_PROTECTED, pTarget->GetPosX(), pTarget->GetPosY())) ||
// 		 ((!pTarget->IsEvil()) && pTarget->GetMap()->IsPkDisable())) // 陈剑飞修改,判断在安全地图可以PK红名 [6/11/2007]
// 		/*|| GetMap()->IsInRegionType(REGION_PK_PROTECTED, GetPosX(), GetPosY())*/)//  [5/16/2007]陈剑飞注释，判断安全区域内可以打怪，但不能PK
// 	{
// 		if (bSendHint)
// 			this->SendSysMsg(g_objStrRes.GetStr( 15041));//"错误的区域:你或目标玩家身处安全区"
// 		return false;
// 	}
	//  [5/12/2007] 摆摊区域判定
// 	int posX = this->GetPosX();
// 	int posY = this->GetPosY();
// 	CGameMap* pMap = MapManager()->QueryMap(this->GetMapID());	
// 	if(pMap->IsMarketRegion(posX,posY))
// 	{
// 		LOGERROR( "MsgAction::case actionCreateBooth出错,摆摊区域非法攻击!.用户:%s mapID:%d x:%d y:%d",
// 			this->GetName(),
// 			this->GetMapID(),
// 			posX,posY );
// 		this->SendSysMsg(g_objStrRes.GetStr( 15042));//"你不应该看到这个错误.错误已经记录.摆摊区域非法攻击!"
// 		return false;
/*	}*/
	//  [5/12/2007] 摆摊区域判定
	if (this && !this->IsAlive())
	{
//		if (bSendHint)
//			this->SendSysMsg(g_objStrRes.GetStr( 15043));//"错误的状态:你已经死亡"
		return false;
	}

	if (pTarget && !pTarget->IsAlive())
	{
	//	if (bSendHint)
//		this->SendSysMsg("死者为大，禁止鞭尸。");
 //		if (bSendHint)
 //			this->SendSysMsg(g_objStrRes.GetStr( 15044));//"错误的对象:目标已经死亡"
		return false;
	}

//	if (!this->IsPkEnable(pTarget))
//	{
//		if (bSendHint)
//			this->SendSysMsg(STR_NOT_PK_MODE);
//		return false;
//	}
	
// 	if(IsInArena())
// 	{
// 		CUser* pTempUser;
// 		if(pTarget->QueryObj(OBJ_USER, IPP_OF(pTempUser)))
// 		{
// 			if(pTempUser->m_strFightingTeamName==m_strFightingTeamName)
// 				return false;
// 			IStatus* pStatus = pTempUser->QueryStatus(STATUS_PK_PROTECT);
// 			if(pStatus)
// 				return false;
// 			if(!pTempUser->IsAlive())
// 				return false;
// 			else
// 				return true;
// 		}
// 	}
	
	CMonster* pMonster = NULL;
	if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
	/*	if(pMonster->IsCat())*/		
		if(pMonster->IsNonBeAttack())
		{
			if (bSendHint)
				this->SendSysMsg("错误的目标");
			return  false;
		}

//		else if(pMonster->IsGuard())
//		{
//			if (bSendHint)
//				this->SendSysMsg("你不能攻击守卫");
//			return  false;
//		}

// 		if(strstr(pMonster->GetName(),"守卫") != 0)//"龙族"
// 		{
// 			if (bSendHint)
// 				this->SendSysMsg(g_objStrRes.GetStr( 15049));//"错误的对象:不能攻击本族守卫"
// 			return  false;
// 		}
// 
// 		if(GetCityWarDoMain() && CITYSYNWAR_CITYWAR == GetCitySynWarType())
// 		{
// 			if(IsCityWarAttack(GetCityWarDoMain()) && pMonster->IsCityAttack())
// 			{
// 				if (bSendHint)
// 			    	this->SendSysMsg("不能攻击本帮守卫");
// 				return false;
// 			}
// 			else if(IsCityWarDefence(GetCityWarDoMain()) && pMonster->IsCityDefence())
// 			{
// 				if (bSendHint)
// 			    	this->SendSysMsg("不能攻击本帮守卫");
// 				return false;
// 			}
// 		}
// // 		
// 
// 		IStatus* pStatus = QueryStatus(STATUS_CITYWAR_WEAPON);
// 		if( pStatus && !pMonster->IsCityWarStrategicTarget())
// 		{
// 			this->SendSysMsg(g_objStrRes.GetStr(28044));//("您拥有城战武器状态，只能攻击城战战略目标");
// 			return false;
// 		}
// 
// 		if(0 != GetSynWarJoinID() && CITYSYNWAR_SYNWAR == GetCitySynWarType())
// 		{
// 			if((SYNWARTYPE_ATTACK == GetSynWarJoinType() && pMonster->IsCityAttack())
// 				|| (SYNWARTYPE_DEFENCE == GetSynWarJoinType() && pMonster->IsCityDefence()))
// 			{
// 				if (bSendHint)
// 		    		SendSysMsg("不能攻击本帮守卫");
// 				return false;
// 			}
// 		}
			
//			if(this->GetPhyle() == PROF_DRAGON && strstr(pMonster->GetName(),g_objStrRes.GetStr( 15046)) != 0)//"龙族"
//			{
//				if (bSendHint)
//					this->SendSysMsg(g_objStrRes.GetStr( 15049));//"错误的对象:不能攻击本族守卫"
//				return  false;
//			}
//			else if(this->GetPhyle() == PROF_HUMAN && strstr(pMonster->GetName(),g_objStrRes.GetStr( 15047)) != 0)//"天人族"
//			{
//				if (bSendHint)
//					this->SendSysMsg(g_objStrRes.GetStr( 15049));
//				return  false;
//			}
//			else if(this->GetPhyle() == PROF_GENE && strstr(pMonster->GetName(),g_objStrRes.GetStr( 15048)) != 0)//"基因族"
//			{
//				if (bSendHint)
//					this->SendSysMsg(g_objStrRes.GetStr( 15049));
//				return  false;
//			}
			
			//  [2007-8-31] 朱斌 是否为同一战争活动
// 			IStatus* pStatus1 = this->QueryStatus(STATUS_WARGAME);
// 			IStatus* pStatus2 = pTarget->QueryStatus(STATUS_WARGAME);
// 			if(pStatus1 == NULL && pStatus2 != NULL)
// 			{
// 				this->SendSysMsg(g_objStrRes.GetStr(16036));
// 				return false;
// 			}
// 						
// 			else if(pStatus1 != NULL && pStatus2 != NULL)
// 			{
// 				if(pStatus1->GetPower() != pStatus2->GetPower())
// 				{
// 					this->SendSysMsg(g_objStrRes.GetStr(16037));
// 					return false;
// 				}
// 			}
// 
// 			else if(pStatus1 != NULL && pStatus2 == NULL)
// 			{
// 				//	0002215: 如果身上有边境战BUFF的玩家攻击别族的军旗，别族没有参加边境战的玩家就不能攻击他。 	
// 				if(pMonster->GetType() == MONSTER_TYPE_DRAGON_FLAG 
// 					|| pMonster->GetType() == MONSTER_TYPE_HUMAN_FLAG 
// 					|| pMonster->GetType() == MONSTER_TYPE_GENE_FLAG )
// 				{
// 					this->SendSysMsg(g_objStrRes.GetStr(16037));
// 					return false;
// 				}  
// 			}

			//  [2007-9-22] 朱斌	怪物有无敌BUF
			IStatus* pStatus3 = pTarget->QueryStatus(STATUS_IMMUNE_ALL);
			if(pStatus3 != NULL)
				return false;
			return true;
		}
//		if (pMonster->IsMonster() || pMonster->IsSynPet())//怪物或宠物,随意攻击
		//07.5.12王玉波注释以上一句,不需要判断，真接return true,杀其它种族的守护兽与守卫，直接可以攻击
// 		return true;
// 	}


	//20070404修罗:提到前面来
	if(!pTarget->IsBeAtkable())
	{
		if (bSendHint)
			this->SendSysMsg("目标是无敌的");//"错误的对象:对象不可被攻击"
		return false;
	}

	CNpc* pNpc = NULL;
	pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));	
	if(pNpc)
		return false;

	{
		CUser* pTargetUser = NULL;
		if (pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
		{
			//任何模式下参加活动的玩家之间才能攻击
			IStatus* pKIllWarStatus1=QueryStatus(STATUS_WARGAME);///杀手活动buffer
			IStatus* pKIllWarStatus2=pTargetUser->QueryStatus(STATUS_WARGAME);///杀手活动buffer
			if ((pKIllWarStatus1 && !pKIllWarStatus2) || (!pKIllWarStatus1  && pKIllWarStatus2))
			{
				this->SendSysMsg(g_objStrRes.GetStr(16038));	//  [2007-9-3] 朱斌
				return false;
			}
			///虽然双方都参加了活动,但参加活动的类型不一样也不能攻击
			if (pKIllWarStatus1 && pKIllWarStatus1 && pKIllWarStatus1->GetPower()!=pKIllWarStatus2->GetPower())
			{
				this->SendSysMsg(g_objStrRes.GetStr(16039));	//  [2007-9-3] 朱斌
				return false;
			}

			//  [2007-9-28] 朱斌 活动中不能攻击本族同胞
//			if(pKIllWarStatus1 && pKIllWarStatus2 && (pKIllWarStatus1->GetPower() == pKIllWarStatus2->GetPower()))
//			{
//				if(this->GetPhyle() == pTargetUser->GetPhyle())
//				{
//					this->SendSysMsg(g_objStrRes.GetStr(16040));
//					return false;
//				}
//				else	//  [11/9/2007 朱斌]	0002205: 在边境战中.有边境战ＢＵＦＦ的玩家只要有一方不开仇视按钮.就不能相互攻击. 
//					return true;
//			}


			//任何模式下都可以攻击红名,灰名玩家
// 			if(pTargetUser->IsEvil())
// 			{
// 				return true;
// 			}

			///种族地图保护及地图归属-softworms-2007-08-11-针对受击者
			//if(pTarget->GetMap()->GetOwnerType()==OWNER_RACE ///受害者所在地图为种族拥用地图
   			/*	&& pTarget->GetMap()->GetOwnerID()== pTargetUser->GetPhyle()*////受害者在本族地图
			//	&& pTargetUser->QueryStatus(STATUS_ENEMYFIRE)==NULL ///受击者身上带了"仇视的buff" 
			/*	&& pTargetUser->GetPhyle()!=GetPhyle()*////双方不是同种族
			//	)///受击者在自己种族所在的地图
			//	return false;

			///种族地图保护及地图归属-softworms-2007-08-11-针对攻击者
			//if(GetMap()->GetOwnerType()==OWNER_RACE ///攻击者所在地图为种族拥用地图
			/*	&& GetMap()->GetOwnerID()== GetPhyle()*////攻击者在本族地图
			//	&& QueryStatus(STATUS_ENEMYFIRE)==NULL ///攻击者身上带了"仇视的buff" 
			/*	&& pTargetUser->GetPhyle()!=GetPhyle()*////双方不是同种族
			//	)///受击者在自己种族所在的地图
			//{
			//	SendSysMsg(g_objStrRes.GetStr( 15051));//"请点击人物头像旁的'PVP'按钮，来开启对异族的PK!"
			//	return false;
			//}

			//任何模式下都可以攻击异族玩家,
//			if(this->GetPhyle() != pTargetUser->GetPhyle())
//				return true;
		}
	}
	
//     //1)组队成员不能被攻击 
//     CTeam* tTeam = GetTeam();
// 	if(tTeam)
// 	{
// 		if(tTeam->IsTeamMember(pTarget->GetID()))
// 		{
// 			SendSysMsg("你不能攻击你的队友");
// 			return false;
// 		}
// 	}
// 
// 	//2) 同盟或是同帮派攻击
// 	CUser* pUser = NULL;
// 	if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)) && this->GetSynID() != ID_NONE)
// 	  {
// 		  {				
// 			  CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetMasterSynID());
// 			  if (pSyn)
// 			  {
// 				  if ( pSyn->IsFriendly(pUser->GetSynID()))
// 				  {
// 					  if(CITYSYNWAR_SYNWAR == GetCitySynWarType())
// 					  {
// 						  if(SYNWARTYPE_NONE != GetSynWarJoinType() && GetSynWarJoinType()==pUser->GetSynWarJoinType())
// 						  {
// 							  if (bSendHint)
// 								  if(IsSynProcAble())
// 								  {
// 									  this->SendSysMsg("不能攻击本帮成员");//"错误的对象:在当前PK模式下,不能攻同盟"
// 									  return false;
// 								  }
// 						  }
// 					  }
// 					  else
// 					  {
// 						  if (bSendHint)
// 							  this->SendSysMsg(g_objStrRes.GetStr( 15053));//"错误的对象:在当前PK模式下,不能攻同盟"
// 						  return false;
// 					  }
// 				  }
// 			  }
// 		  }
// 
// 		  CSyndicate * thisSyn = this->GetSyndicate();
// 		  if(thisSyn && thisSyn->GetState() == 2 /*&& thisSyn->GetWarSynID() == pUser->GetSynID()*/)
// 		  {
// 			  return true;
// 		  }
// 	  }
// 
// 	  
// 
//       //3)红名保护
// 	  if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
// 	  {	
// #ifdef _DEBUG
// 		  char szName[128];
// 		  strcpy(szName,pUser->GetName());
// #endif // _DEBUG
// 
// 		  int nDoMainID = this->GetCityWarDoMain();  //城战时没有PK保护
// 		  if(nDoMainID && nDoMainID == pUser->GetCityWarDoMain())
// 		  {
// 		  }
// 		  else if(0 != this->GetSynWarJoinID() && 0 != pUser->GetSynWarJoinID())//帮战时没有PK保护
// 		  {
// 		  }
// 		  else
// 		  {
// 			  if(pUser->IsEvil())
// 			  {
// 				  if(IsRedProcAble())
// 				  {
// 					  return false;
// 				  }
// 			  }
// 			  else	//4)白名保护
// 			  {
// 				  if(IsWhileProcAble())
// 					  return false;	
// 			  }
// 		  }
// 	  }

    


// 	switch(this->GetPkMode())
// 	{
// 	case	PKMODE_FREE://全
// 		//return true;
// 		break;
// 
// 	case	PKMODE_TEAM://队
// 		{
// 			LOGERROR( "CUser::IsAtkable函数出错,"
// 				"错误的PK模式.[队].用户:%s",this->GetName());
// 			return false;
// //			CTeam* pTeam = this->GetTeam();
// //			if(pTeam && pTeam->IsTeamMember(pTarget->GetID()))
// //				return false;
// //			
// //			if (this->GetFriend(pTarget->GetID()))
// //				return false;
// //			
// //			CUser* pUser = NULL; 
// //			if(pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
// //			{
// //				if(this->IsMate(pUser))
// //					return false;
// //			}	
// 		}
// 		break;
// 		
// 	case    PKMODE_SYNDICATE://团
// 		{
// 			CUser* pUser = NULL;
// 			if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)) && this->GetSynID() != ID_NONE)
// 			{
// 				if (this->GetMasterSynID() == pUser->GetMasterSynID() )
// 				{
// 					if (bSendHint)
// 						this->SendSysMsg(g_objStrRes.GetStr( 15052));//"错误的对象:在当前PK模式下,不能攻击本团成员"
// 					return false;
// 				}
// //				else if(IsMate(pUser))
// //				{
// //					if (bSendHint)
// //						this->SendSysMsg("错误的对象:在当前PK模式下,不能攻击配偶");
// //					return false;
// //				}
// 				else
// 				{				
// 					CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetMasterSynID());
// 					if (pSyn)
// 					{
// 						if ( pSyn->IsFriendly(pUser->GetSynID()))
// 						{
// 							if (bSendHint)
// 								this->SendSysMsg(g_objStrRes.GetStr( 15053));//"错误的对象:在当前PK模式下,不能攻同盟"
// 							return false;
// 						}
// 					}
// 				}
// 			}
// 		}
// 		break;
// 	case	PKMODE_ARRESTMENT://罪
// 		{
// 			LOGERROR( "CUser::IsAtkable函数出错,"
// 				"错误的PK模式.[罪].用户:%s",this->GetName());
// 			return false;
// //			if(pTarget->IsEvil())
// //				return true;
// 		}
// 		break;
// 
// 	case	PKMODE_SAFE://和
// 		{
// //			CUser* pUser = NULL;
// //			if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
// //			{
// //				if(this->GetPhyle() != pUser->GetPhyle())
// //					return true;
// //			}
// 			if(!pTarget->IsMonster())
// 			{
// 				if (bSendHint)
// 					this->SendSysMsg(g_objStrRes.GetStr( 15054));//"错误的对象:在当前PK模式下,不能攻击目标"
// 				return false;
// 			}
// 		}
// 		break;
// 
// 	default:
// 		ASSERT(!"switch pk mode");
// 		break;
// 	}
	//---------------------------------------------------------------------


	




	//前面判断过
//	if(GetID() == pTarget->GetID())
//		return false;
	
//20070315修罗.屏蔽
//	CNpc* pNpc;
//	if(pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
//	{
//		if(pNpc->IsSynFlag() && GetSynID() != ID_NONE && pNpc->GetSynID() != ID_NONE)
//		{
//			//---jinggy---若是攻打帮派，必须是申请攻打的帮派成员才可以打---begin
//		/*	CGameMap * pMapSyn = this->GetMap();
//			if(pMapSyn->IsSynMap())
//			{
//				if(pMapSyn->IsWarTime())
//				{
//					//不是申请攻打的帮派，就不能打
//					CNpc * pWarLetterNpc = pMapSyn->GetNpcSynWarLetter();
//					if(!pWarLetterNpc)
//						return false;
//
//					if(this->GetSynID() != (pWarLetterNpc->GetApplyAttackerSynID()))
//						return false;
//					
//				}
//				else //非战争时间不允许攻打
//					return false;
//			}*/
//			//---jinggy---若是攻打帮派，必须是申请攻打的帮派成员才可以打---end
//			CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
//			if(pSyn && pSyn->IsNearSyn(pNpc->GetSynID()))
//				return false;
//		}
//	}

	// map check
	if (!(this->GetMap() && pRealTarget->GetMap() && this->GetMap() == pRealTarget->GetMap()))
	{
		if (bSendHint)
			this->SendSysMsg(g_objStrRes.GetStr( 15055));//"错误的地图:你与目标不在同一个地图"
		return false;
	}

	CUser* pUserTarget = NULL;
	pTarget->QueryObj(OBJ_USER, IPP_OF(pUserTarget));


	//  [12/19/2007 朱斌]	挑战	6405
	//	强迫目标只能攻击自己，持续3秒。这是对目标使用的Debuff，需要新的status。使自己成为敌人的目标，且敌人不能改变目标。只对玩家有效。
	if(!CRole::CheckDebuffLockTarget(QueryRole(), pRealTarget->GetID()))
		return false;

	

	return true;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
bool CUser::Create(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idUser, ULONG nIP)
{
	int nStep = 0;
	DEBUG_TRY
	CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess));
	m_idProcess	= idProcess;
	m_idSocket	= idSocket;
	m_nIPAddress = nIP;

	nStep = 1;
	m_nDir		= 2;	//DEFAULT_LOGIN_DIR;
	// 装入DATA
	if(!m_data.Create(idUser, Database()))
	{
		LOGWARNING("加载m_data失败");
		return false;
	}

	nStep = 2;
	// 进入地图
// 	int nLife = GetLife();
// 	if( nLife <= 0 ) 
// 	{
// 		this->SetAttrib(_USERATTRIB_LIFE, GetMaxLife() / 10, SYNCHRO_TRUE);
// 		this->SetAttrib(_USERATTRIB_MANA, GetMaxLife() / 10, SYNCHRO_TRUE);
// 		this->SetAttrib(_USERATTRIB_SP, 0, SYNCHRO_TRUE);
// 	}
	InitSpeInfoOnly();//只加载判断专线切换的数据，如果需要切线，就不再加载其他数据了

	nStep = 3;
	CHECKF(m_pSpecilLineInfo);
	//简单判断一下是不是需要切线，需要的话就不再加载数据了
	if(!m_pSpecilLineInfo->NeedChangeLine())
	{
		Init();
	}

	nStep = 4;
	OBJID idMap = ID_NONE;
	bool bNeedChange = m_pSpecilLineInfo->ProcessLogin(idMap, m_nPosX, m_nPosY);
	if(!bNeedChange && !m_bLoaded)
		Init();


	nStep = 5;
	if(idMap!=ID_NONE)
		m_pMap = MapManager()->GetGameMap(idMap);
	if(m_pMap && CWargame::IsWarGameMap(idMap))
		EnterWarGame(GetSpecilMapID()%10);

	nStep = 6;
	if(!m_pMap)
	{
		m_nPosX		= this->m_data.m_Info.usRecordX;
		m_nPosY		= this->m_data.m_Info.usRecordY;
		OBJID idMap	= this->m_data.m_Info.idRecordmap;
		//goto 新手出生地图分配
		if (idMap==ID_NONE)
		{
			idMap = GetBornMap();
		}
		m_pMap = MapManager()->GetGameMap(idMap);
	}

	nStep = 7;
	if(!m_pMap)		//? 活动地图
	{
		LOGWARNING("找不到原地图 idMap = %u",idMap);
		m_nPosX		= DEFAULT_LOGIN_POSX;
		m_nPosY		= DEFAULT_LOGIN_POSY;
		OBJID idMap	= DEFAULT_LOGIN_MAPID;
		m_pMap		= MapManager()->GetGameMap(idMap);
		CHECKF(m_pMap);
	}

	nStep = 8;
	if(!m_pMap->IsStandEnable(m_nPosX,m_nPosY))
	{
		LOGWARNING("原坐标非法 idMap = %u,m_nPosX = %u,m_nPosY = %u",idMap,m_nPosX,m_nPosY);
		m_nPosX		= DEFAULT_LOGIN_POSX;
		m_nPosY		= DEFAULT_LOGIN_POSY;
		OBJID idMap	= DEFAULT_LOGIN_MAPID;
		m_pMap		= MapManager()->GetGameMap(idMap);
		CHECKF(m_pMap);
	}

 	CHECKF(m_pMap);

	if( this->IsGM() )
		m_bHide = true;	//默认GM初始为true;
	
#ifdef _DEBUG
	m_bHide = false;	//debug模式下GM不隐身，方便调试
#endif
	//自动挂机数据
//	this->ReplyAutoFightData();
	return true;
	DEBUG_CATCH2("CUser::Create() nStep = %d",nStep)
	return false;
}
bool CUser::InitStorage(IDatabase* pDb,bool bLogin/* = true*/)
{
	if(!m_pStorage)
		m_pStorage	= CPackage::CreateNew();
	CHECKF(m_pStorage);	
	if (!m_pStorage->Create(ITEMPOSITION_STORAGE, GetID(), GetID(), 0, Database(),bLogin))
		return false;
	return true;
}
bool CUser::InitEudemon(IDatabase* pDb,bool bLogin/* = true*/)		
{
	if(m_pUserPet)
		return true;

	m_idEudemonCur = 0;
	m_idSoulEudemon = 0;
	if(!m_pUserPet)
		m_pUserPet = CUserPet::CreateNew();
	if(!m_pUserPet)
		return false;

	m_idEudemonCur = ID_NONE;
	m_idSoulEudemon =ID_NONE;
	m_pUserPet->Create(PID,this,bLogin);
	
	return true;
}

bool CUser::InitChangeMapGroup()
{
	m_tFight.SetInterval(/*this->GetIntervalAtkRate()*/800);		// 徒手攻频率
	m_tFight.Update();

	m_tAutoHealRecover.SetInterval( AUTOHEAL_TIME );
	m_tAutoHealRecover.Update();

	m_tItemLifeTime.SetInterval(ITEM_LIFE_TIME_FIRST_CHECK_INTERVAL_SEC);	//	第一次检查
	m_tItemLifeTime.Update();

	m_tVipFaceTime.SetInterval(VIP_FACE_SEC);
	m_tVipFaceTime.Update();

	m_tVipTime.SetInterval(VIP_CHECK_SEC);
	m_tVipTime.Update();

	//[2010-08-27 goto]特殊大红大蓝的计时
	m_tHpMpAdd.SetInterval(ESEPCIAL_DRUG_SEC);
	m_tHpMpAdd.Update();

	m_EverydayOL.SetInterval(EverdayOL_SEC);
	m_EverydayOL.Update();

	//写数据库时间，随机一下以错开不同的玩家
	m_tUpdateDatabase.SetInterval(UNDATEDATABASE_BASETIME + RandGet(UNDATEDATABASE_OFFTIME));
	m_tUpdateDatabase.Update();

	m_tPK.SetInterval(PK_DEC_TIME);// every 1 minutes reduce 2 pk value //change huang 2004.1.11
	m_tPK.Update();
	
	m_tFati.SetInterval(FATIGUE);		//用来判断user的防沉迷
	m_tFati.Update();

	m_tTeam.SetInterval(TIME_TEAMPRC);
	m_tTeam.Update();

	m_pBattleSystem	= new CBattleSystem(this->m_idProcess, this);
	ASSERT(m_pBattleSystem);

	//物品
	IDatabase* pDb = Database();
	CreateItemFree(pDb);
	InitStorage(pDb,false);

	//经脉系统初始化
	UserJinmeiSysMgr()->OnUserLogin(this,false);

	//key
	if(!IniUserKey(false,false))
		return false;

	//autofight
	if(!IniUserAutoFight(false))
		return false;

	// 配方
	if(!m_pUserPeifang)
		m_pUserPeifang = CUserPeifang::CreateNew();
	if(!m_pUserPeifang)
		return false;
	m_pUserPeifang->Create(PID,this,false);

	//任务
	m_pTaskDetail = CTaskDetail::CreateNew();
	IF_NOT(m_pTaskDetail && m_pTaskDetail->Create(m_idProcess, this,false))
		return false;	

	//宠物
	if(!InitEudemon(pDb,false))
		return false;
	m_idNowEudemon = ID_NONE;

	//帮会
	m_pSyn	= CSynAttr::CreateNew(m_idProcess, this);
	ASSERT(m_pSyn);
// 	if (!m_pSyn )
// 		return false;

	//技能
	m_pMagic = CMagic::CreateNew(m_idProcess, this->QueryRole());
	ASSERT(m_pMagic);

	m_setStatus	= CStatusSet::CreateNew(true);
	ASSERT(m_setStatus);

	m_bPVPStuts = false;
	m_dwPasswayAction = ID_NONE;
	idSelectItemType = ID_NONE;
	m_idLastTaskEudemon = ID_NONE;

	ClrInviteUserID();
	InitAwarTime(false);

	//称号
	m_pTitle = CUserTitle::CreateNew(m_idProcess, this);
	ASSERT(m_pTitle);
	
	//彩票,开箱子
	m_pTreasure = CTreasure::CreateNew(m_idProcess, this);
	ASSERT(m_pTreasure);

	//师徒
	m_pUserTutor = CUserTutor::CreateNew(m_idProcess, this);
	ASSERT(m_pUserTutor);

	//宠物蛋
	m_pPetEgg = CPetEgg::CreateNew(m_idProcess, this);
	ASSERT(m_pPetEgg);

	//上线祝福
	m_pOnlineGift = COnlineGift::CreateNew(m_idProcess, this);
	ASSERT(m_pOnlineGift);

	//副本计数
	InstanceUseMgr()->UserLogin(GetID());

	//个人竞技场信息
	m_pUserWarGame = CUserWarGame::CreateNew(m_idProcess, this);
	ASSERT(m_pUserWarGame);

	//专线记录
	m_pSpecilLineInfo = CSpecilLineInfo::CreateNew(this);
	ASSERT(m_pSpecilLineInfo);
//modify code by 林德渊 begin 2011.5.30-
	m_pvaslvdata = CVasLvData::CreateNew();
	ASSERT(m_pvaslvdata);
//modify code by 林德渊 end

	m_bLoaded = true;

	//跑商验证
	m_pVerification = CVerification::CreateNew(m_idProcess, this);
	ASSERT(m_pVerification);

	ClearCheckData();
	initCompensationManager();

	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUser::Init			()		// login 和 change map_group都要执行的操作
{
	m_tFight.SetInterval(/*this->GetIntervalAtkRate()*/800);		// 徒手攻频率
	m_tFight.Update();

	m_tAutoHealRecover.SetInterval( AUTOHEAL_TIME );
	m_tAutoHealRecover.Update();

	m_tItemLifeTime.SetInterval(ITEM_LIFE_TIME_FIRST_CHECK_INTERVAL_SEC);	//	第一次检查
	m_tItemLifeTime.Update();

	m_tVipFaceTime.SetInterval(VIP_FACE_SEC);
	m_tVipFaceTime.Update();

	m_tVipTime.SetInterval(VIP_CHECK_SEC);
	m_tVipTime.Update();

	//[2010-08-27 goto]特殊大红大蓝的计时
	m_tHpMpAdd.SetInterval(ESEPCIAL_DRUG_SEC);
	m_tHpMpAdd.Update();

	m_EverydayOL.SetInterval(EverdayOL_SEC);
	m_EverydayOL.Update();

	//写数据库时间，随机一下以错开不同的玩家
	m_tUpdateDatabase.SetInterval(UNDATEDATABASE_BASETIME + RandGet(UNDATEDATABASE_OFFTIME));
	m_tUpdateDatabase.Update();

	m_tPK.SetInterval(PK_DEC_TIME);// every 1 minutes reduce 2 pk value //change huang 2004.1.11
	m_tPK.Update();
	
	m_tFati.SetInterval(FATIGUE);		//用来判断user的防沉迷
	m_tFati.Update();

	m_tTeam.SetInterval(TIME_TEAMPRC);
	m_tTeam.Update();

	m_pBattleSystem	= new CBattleSystem(this->m_idProcess, this);
	ASSERT(m_pBattleSystem);

	// 装入USER管理对象
	IDatabase* pDb = Database();
	if(!CreateAllFriend(pDb))
		return false;
	if(!CreateAllBeFriend(pDb))
		return false;
	if(!CreateAllItem(pDb))
		return false;
	if(!IniUserKey(false))
		return false;
	if(!IniUserAutoFight())
		return false;
	if(!InitEudemon(pDb))
		return false;
	m_idNowEudemon = m_idEudemonCur;


	if(!m_pUserPeifang)
		m_pUserPeifang = CUserPeifang::CreateNew();
	if(!m_pUserPeifang)
		return false;
	m_pUserPeifang->Create(PID,this);

	InitStorage(pDb);

	m_pTaskDetail = CTaskDetail::CreateNew();
	IF_NOT(m_pTaskDetail && m_pTaskDetail->Create(m_idProcess, this))
		return false;
	
	m_pSyn	= CSynAttr::CreateNew(m_idProcess, this);
	ASSERT(m_pSyn);
	if (!m_pSyn || !m_pSyn->Create())
		return false;

	m_pMagic = CMagic::CreateNew(m_idProcess, this->QueryRole());
	m_pMagic->CreateAll();
	ASSERT(m_pMagic);



	///处理帮派上线通知,切地图组时不发通知
	if(SynManager()->QuerySyndicate(GetSynID()) )
	{
		if(GetSyndicate()->GetInt(SYNDATA_STATE) == 2)
		{
			SynManager()->QuerySyndicate(GetSynID())->SetOnline(GetID(),CVipList::GetLineByName(MapGroup(PID)->GetServerName()));			
			SynManager()->QuerySyndicate(GetSynID())->BroadcastSynMemberInfoMsg(GetID());
		
			///帮派成员上线,将上线信息广播给其它线
			///有人入帮，通知其它线更新数据
			CMsgSynMemberInfo msg;
			if (msg.Create(SynMemberOnlineStatusChange,1,2))
			{
				msg.Append(GetSynID(),"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
				msg.Append(GetID(),"",0,0,0,0,CVipList::GetLineByName(MapGroup(PID)->GetServerName()),0,0,0,0,0,0);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,0);
			}
		}
	}

	m_bPVPStuts = false;

	m_dwPasswayAction = ID_NONE;
	idSelectItemType = ID_NONE;
	m_idLastTaskEudemon = ID_NONE;

	
	ClrInviteUserID();
	InitDoubleTime();
	InitAwarTime(true);
	RebuildTeam();

	//称号
	m_pTitle = CUserTitle::CreateNew(m_idProcess, this);
	ASSERT(m_pTitle);
	m_pTitle->CreateAll();
	
	//彩票,开箱子
	m_pTreasure = CTreasure::CreateNew(m_idProcess, this);
	ASSERT(m_pTreasure);
	m_pTreasure->CreateAll();
	//上线祝福
	m_pOnlineGift = COnlineGift::CreateNew(m_idProcess, this);
	ASSERT(m_pOnlineGift);
	m_pOnlineGift->CreateAll();


	//经脉系统初始化
	UserJinmeiSysMgr()->OnUserLogin(this,true);

	//副本计数
	InstanceUseMgr()->UserLogin(GetID());
	//师徒
	m_pUserTutor = CUserTutor::CreateNew(m_idProcess, this);
	ASSERT(m_pUserTutor);
	m_pUserTutor->CreateAll();
	

	m_pUserWarGame = CUserWarGame::CreateNew(m_idProcess, this);
	ASSERT(m_pUserWarGame);
	m_pUserWarGame->Create(Database());
	
	//宠物蛋
	m_pPetEgg = CPetEgg::CreateNew(m_idProcess, this);
	ASSERT(m_pPetEgg);
	m_pPetEgg->CreateAll();
//modify code by 林德渊 begin 2011.5.30-
	m_pvaslvdata= CVasLvData::CreateNew();
	ASSERT(m_pvaslvdata);
	m_pvaslvdata->Create(GetAccountID(),GetID(),Database());
//modify code by 林德渊 end
	m_bLoaded = true;
	////跑商验证
	m_pVerification = CVerification::CreateNew(m_idProcess, this);///
 	ASSERT(m_pVerification);
////
	ClearCheckData();
	initCompensationManager();
	return true;
}

bool CUser::InitSpeInfoOnly()
{
	m_setStatus	= CStatusSet::CreateNew(true);
	ASSERT(m_setStatus);

	//专线
	m_pSpecilLineInfo = CSpecilLineInfo::CreateNew(this);
	ASSERT(m_pSpecilLineInfo);
	m_pSpecilLineInfo->Create();
	//离线经验
	UpdateLeaveExp();

	return true;
}
//////////////////////////////////////////////////////////////////////
void CUser::SaveInfo(int nSaveType)
{
	if(!m_bLoaded && m_pSpecilLineInfo)
	{
		DEBUG_TRY
		m_data.SaveInfo();
		m_pSpecilLineInfo->Update();
		DEBUG_CATCH("CUser::SaveInfo savetype changeline!")
		return;
	}
	if(nSaveType == SAVETYPE_OFFLINE)
	{
		DEBUG_TRY
		if(IsAwardTimeOut()&&GetAward()%10!=8)
			SetAward(GetAward()%10+10);
		DEBUG_CATCH("SetAward(GetAward()%10+10,false)")

		DEBUG_TRY
		UpdateAllOnlineTime();// 更新玩家总在线时长 [9/14/2007 %PENGFENG%]		
		DEBUG_CATCH("logout user in CUSER::SaveInfo When Save logout time")

		DEBUG_TRY
		m_data.SaveInfo();
		DEBUG_CATCH("m_data.SaveInfo() exit")
		DEBUG_TRY
		SaveMissionInfo();	
		DEBUG_CATCH("SaveMissionInfo() exit")
		DEBUG_TRY
		SaveItemInfo();
		DEBUG_CATCH("SaveItemInfo() exit")
		DEBUG_TRY
		m_pMagic->SaveInfo();
		DEBUG_CATCH("m_pMagic->SaveInfo() exit")
		DEBUG_TRY
		m_pSyn->SaveInfo();
		DEBUG_CATCH("m_pSyn->SaveInfo() exit")
		DEBUG_TRY
		m_pUserPet->SaveData();
		DEBUG_CATCH("m_pUserPet->SaveInfo() exit")
		DEBUG_TRY
		m_pTitle->SaveInfo();
		DEBUG_CATCH("m_pTitle->SaveInfo() exit")
		DEBUG_TRY
		m_pTreasure->SaveInfo();
		DEBUG_CATCH("m_pTreasure->SaveInfo() exit")
		DEBUG_TRY
		m_pOnlineGift->SaveInfo();
		DEBUG_CATCH("m_pOnlineGift->SaveInfo() exit")
		DEBUG_TRY
		m_pUserKey.SaveInfo();
		DEBUG_CATCH("m_pUserKey.SaveInfo()");
		DEBUG_TRY
		m_pUserTutor->SaveInfo();
		DEBUG_CATCH("m_pUserTutor->SaveInfo() exit")
		DEBUG_TRY
		m_pUserWarGame->SaveInfo();
		DEBUG_CATCH("m_pUserWarGame->SaveInfo() exit")
		DEBUG_TRY
		m_pPetEgg->SaveInfo();
		DEBUG_CATCH("m_pPetEgg->SaveInfo() exit")
		DEBUG_TRY
		m_pAutoFightData.Update();
		DEBUG_CATCH("m_pAutoFightData->Update() exit")
		DEBUG_TRY
		if(m_pSpecilLineInfo)
			m_pSpecilLineInfo->Update();
		DEBUG_CATCH("m_pSpecilLineInfo->Update() exit")
//modify code by 林德渊 begin 2011.5.30-
		DEBUG_TRY
		m_pvaslvdata->SaveInfo();
		DEBUG_CATCH("m_pvaslvdata->Update() exit")
//modify code by 林德渊 end
	}
	else if(nSaveType == SAVETYPE_UPDATE)
	{
		if(CanBeSee())
		{
			DEBUG_TRY
			m_data.SaveInfo();
			DEBUG_CATCH("m_data.SaveInfo()");
			DEBUG_TRY
			SaveItemInfo();
			DEBUG_CATCH("SaveItemInfo()");
			DEBUG_TRY
			SaveMissionInfo();		
			DEBUG_CATCH("SaveMissionInfo");
			DEBUG_TRY
			m_pSyn->SaveInfo();
			DEBUG_CATCH("m_pSyn->SaveInfo()")
			DEBUG_TRY
			m_pUserKey.SaveInfo();
			DEBUG_CATCH("m_pUserKey.SaveInfo()");
			DEBUG_TRY
			m_pMagic->SaveInfo();
			DEBUG_CATCH("m_pMagic->SaveInfo()")
			DEBUG_TRY
			m_pUserKey.SaveInfo();
			DEBUG_CATCH("m_pUserKey.SaveInfo()");
			DEBUG_TRY
			m_pAutoFightData.Update();
			DEBUG_CATCH("m_pAutoFightData->Update()")
//modify code by 林德渊 begin 2011.5.30-
			DEBUG_TRY
			m_pvaslvdata->SaveInfo();
			DEBUG_CATCH("m_pvaslvdata->Update()")
//modify code by 林德渊 end
		}
	}
//	SaveEudemon();
}

//////////////////////////////////////////////////////////////////////
bool CUser::AddLeaveWord(LPCTSTR szRecv, LPCTSTR szWords, LPCTSTR szColor)//[2010-06-10 goto]增加文本颜色字段
{
	if (LeaveWord()->CountWords(GetName(), szRecv) >= MAX_LEAVEWORD_COUNTS)
	{
		SendSysMsg(_TXTATR_TOPSHOW,"留言过多，无法发送更多的留言");
		return false;
	}
	for(int i = 0; i < m_setFriend.size(); i++)
	{
		CFriend* pFriend = m_setFriend[i];
		if(pFriend && strcmp(pFriend->GetFriendName(), szRecv) == 0)
		{
//			ASSERT(UserManager()->GetUser(pFriend->GetFriendID()) == NULL);
			if(pFriend->IsLeaveWordEnable())
			{
				if (LeaveWord()->InsertWords(GetName(), szRecv, szWords, szColor))
				{
					//SendSysMsg(g_objStrRes.GetStr( 12028));//STR_MSG_SENT
					SendSysMsg(_TXTATR_TOPSHOW,"对方不在线，已成功将消息保存为留言");
					return true;
				}	
			}
			else
			{
				//SendSysMsg(g_objStrRes.GetStr( 12029), LEAVEWORD_INTERVAL_MINS);//STR_MSG_FAILED
				SendSysMsg(_TXTATR_TOPSHOW, "消息发送过于频繁，请稍候再发");
			}
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
/*
int	 CUser::AdjustData(int nData, int nAdjust, int nMaxData/ *=0* /)
{
	if(nAdjust>=ADJUST_PERCENT)
		return MulDiv(nData, nAdjust-ADJUST_PERCENT, 100);

	if(nAdjust<=ADJUST_SET)
		return -1*nAdjust + ADJUST_SET;

	if(nAdjust==ADJUST_FULL)
	{
		ASSERT(nMaxData != 0);
		return nMaxData;
	}

	return nData + nAdjust;
}
*/

//////////////////////////////////////////////////////////////////////
///将队伍成员信息发给用户
// void CUser::SendTeamMemberInfo()
// {
// 	DEBUG_TRY
// 	CMsgTeamMember msgTeamMem;
// 	TeamMemberInfo sendInfo;
// 	sendInfo.id = ID_NONE;
// 	if (!msgTeamMem.Create(_MSG_TEAMMEMBER_ANSWERINFO,sendInfo))
// 		return;
// 				
// 	CTeam* pTeam = GetTeam();
// 	if (NULL == pTeam)
// 	{
// 		return;
// // 		if(m_pInfo->idPlayer == ID_NONE)
// // 			return;
// // 		if(m_pInfo->idPlayer != pUser->GetID())
// // 		{
// // 			if(pUser->IsInviteUserID(m_pInfo->idPlayer))
// // 			{
// // 				CUser *pInviteUser = UserManager()->GetUser(m_pInfo->idPlayer);
// // 				if(pInviteUser)
// // 					pTeam = pInviteUser->GetTeam();
// // 			}
// // 		}
// 	}	
// 	pTeam->UpdateMemberInfo();
// 	
// 	pTeam->GetMemberInfo(sendInfo,pTeam->GetLeader());
// 	if(sendInfo.id != pTeam->GetLeader())
// 		return;
// 	
// 	msgTeamMem.Append(1,sendInfo);
// 	
// 	
// 	for(int k = 0 ; k < 6 ; k ++)
// 	{
// 		if(pTeam->m_TeamIn[k].id != ID_NONE   && pTeam->m_TeamIn[k].id != pTeam->GetLeader())
// 		{
// 			pTeam->GetMemberInfo(sendInfo,pTeam->m_TeamIn[k].id);
// 			if(sendInfo.id != pTeam->m_TeamIn[k].id)
// 				return;					
// 			msgTeamMem.Append(1,sendInfo);
// 		}
// 	}
// 	
// 	int nAmountApply = pTeam->GetMemberApplyAmount();
// 	TeamApplyInfo *pInfoOut = NULL;
// 	if (nAmountApply > 0)
// 	{
// 		pInfoOut = new TeamApplyInfo[nAmountApply];
// 	}
// 	
// 	if ( pInfoOut)
// 	{
// 		int nCur = 0;
// 		for (int j=0; j<nAmountApply; j++)
// 		{
// 			OBJID idMember = pTeam->GetMemberApplyByIndex(j);
// 			CUser* pMember = UserManager()->GetUser(idMember);
// 			
// 			if(pMember && !pMember->GetTeam())
// 			{
// 				pInfoOut[nCur].idPlayer			= pMember->GetID();
// 				pInfoOut[nCur].dwLookFace	= pMember->GetLookFace();
// 				pInfoOut[nCur].usLev      = pMember->GetLev();
// 				pInfoOut[nCur].usPro = pMember->GetProfession();
// 				pInfoOut[nCur].usProLev = pMember->GetJobLev();
// 				strcpy(pInfoOut[nCur].szPlayerName, pMember->GetName());
// 				//						pInfoOut[nCur].idMap    = pMember->GetMapID();
// 				
// 				nCur++;
// 			}
// 		}
// 		
// 		nAmountApply = nCur;
// 	}
// 	
// 	
// 	for(int n = 0 ; n < nAmountApply ; n ++)
// 	{
// 		sendInfo.id = pInfoOut[n].idPlayer;
// 		sendInfo.dwLookFace = pInfoOut[n].dwLookFace;
// 		sendInfo.dwLev = pInfoOut[n].usLev;
// 		sendInfo.usProfession = pInfoOut[n].usPro;
// 		sendInfo.usProLev = pInfoOut[n].usProLev;
// 		SafeCopy(sendInfo.szName, pInfoOut[n].szPlayerName,_MAX_NAMESIZE);
// 		msgTeamMem.Append(2,sendInfo);
// 	}
// 	
// 	if(pInfoOut)
// 	{
// 		delete[] pInfoOut;
// 	}
// 	
// 	SendMsg(&msgTeamMem);
// 	DEBUG_CATCH("CUser::SendTeamMemberInfo()")
// }

bool CUser::SendMsg(CNetMsg* pMsg)
{
	if(false
//		||pMsg->GetType() == _MSG_ACTION 
		||pMsg->GetType() == _MSG_INTERACT
//		||pMsg->GetType() == _MSG_PLAYER
//		||pMsg->GetType() == _MSG_TRADE
		)
	{
		DEBUG_SENDMSG(pMsg->GetType(), m_idSocket)
	}

	return MapGroup(PID)->QueryIntraMsg()->SendClientMsg(m_idSocket, pMsg);
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf /*= false*/,EM_SENDNPCFLAG sendNpc/* = EM_SENDNPCFLAG_TRUE*/)		
{ 
	CRole::BroadcastRoomMsg(pMsg, bSendSelf,sendNpc);
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf /*= false*/)		
{
	CMsgTalk	msg;
	if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_TOPSHOW))
		this->BroadcastRoomMsg(&msg, bSendSelf);
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastMapMsg(CNetMsg* pMsg, bool bSendSelf /*= false*/)		
{ 
	//CMapPtr pMap = MapManager()->GetGameMap(GetMapID());
	CMapPtr pMap = this->GetMap();
	if(pMap)
		UserManager()->BroadcastMapMsg(pMap->GetID(), pMsg, bSendSelf?NULL:this);
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastMapMsg(LPCTSTR szMsg, bool bSendSelf /*= false*/)		
{
	CMsgTalk	msg;
	if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg/*, NULL, 0xff0000, _TXTATR_SYSTEM*/))
		this->BroadcastMapMsg(&msg, bSendSelf);
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendInformation(const char* strInfo)///softworms扩展-2007-12-7
{
	return SendSysMsg(strInfo);
}

bool CUser::SendSysInfo(const char* strInfo)///softworms扩展-2007-12-7
{
	return SendSysMsg(_TXTATR_TOPSHOW,strInfo);
}

bool CUser::SendSysMsg(const char* fmt, ...)
{ 
	MSGBUF	szMsg;
    vsprintf( szMsg, fmt, (char*) ((&fmt)+1) );
	
	CMsgTalk msg;
	if (msg.Create(SYSTEM_NAME, GetName(), szMsg, NULL, 0xff0000, _TXTATR_TOPSHOW))
		return this->SendMsg(&msg);

	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendSysMsg(int nChannel, const char* fmt, ...)
{ 
	MSGBUF	szMsg;
    vsprintf( szMsg, fmt, (char*) ((&fmt)+1) );
	
	CMsgTalk msg;
	if (msg.Create(SYSTEM_NAME, GetName(), szMsg, NULL, 0x00ffff00, nChannel))
		return this->SendMsg(&msg);

	return false;
}

//////////////////////////////////////////////////////////////////////
OBJID CUser::GetMapID()
{
	if(m_pMap)
		 
	{
		//for test
		//int mapid = m_pMap->GetID();
		return m_pMap->GetID();
	}
	return ID_NONE;
}

BOOL CUser::IsMapID(DWORD mapid)
{
	return (mapid == GetMapID());
}

//////////////////////////////////////////////////////////////////////
OBJID CUser::GetRecordMapID()
{
	return m_data.GetRecordMap();
}

OBJID CUser::GetRecordPosX()
{
	return m_data.GetRecordX();
}

OBJID CUser::GetRecordPosY()
{
	return m_data.GetRecordY();
}

//////////////////////////////////////////////////////////////////////
void CUser::GotoJail(BOOL GotoJail_Self)
{
 // 	CGameMap* pMap = MapManager()->QueryMap(MAP_PRISON);
//	IF_OK (pMap)
	DEBUG_TRY
	{
		//20070113修罗:直接写死--------------------------------
		const int _posX = 30;
		const int _posY = 25;
		//06.11.17王玉波移动下面代码到此位置并修改
		// reset reborn pos
		{
//			OBJID	idRebornMap = ID_NONE;
//			POINT	pos;
//			IF_OK( pMap->GetRebornMap( &idRebornMap, &pos ) )
//				this->SetRecordPos( idRebornMap, pos.x, pos.y );
//			this->SetRecordPos( MAP_PRISON, _posX, _posX );
		}
		char szMsg[1024] = "";
		char Killer[128];
		sprintf(Killer, "%d_[%s]_7", SNAPSHOTUSER, this->GetName());
		sprintf(szMsg,"<%s>杀人如麻，罪孽深重，已被官府抓获并投入了监狱",Killer);//"holy shit!"
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
	//	UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
	//	AddLeaveWord(this->QueryOwnerUser(), "你因为杀人过多被送入了监狱，望好好改造重新做人!");
// 		CMsgTalk msg;
// 		if (msg.Create("系统", this->GetName(), "<font color='#ff0000'>你因为杀人过多被送入了监狱，望好好改造重新做人！</font>", NULL, 0xfefeefef, _TXTATR_CHATWITH))
// 		this->SendMsg(&msg);
		this->SendEmail(this->GetName(), "<font color='#ff0000'>你因为杀人过多被送入了监狱，望好好改造重新做人！</font>", this);
		
		this->QuitTeam();
		
//		POINT pos = pMap->GetPortal();
//		this->FlyMap(MAP_PRISON, pos.x, pos.y);
		
// 		if (GotoJail_Self)
// 		{
// 			this->GotoSelfJail();
// 		}
// 			
// 		else
			this->FlyMap(MAP_PRISON, _posX, _posY);
		//---------------------------------------------------------




		
/*		// reset reborn pos
		{
			OBJID	idRebornMap = ID_NONE;
			POINT	pos;
			IF_OK(GetMap()->GetRebornMap(&idRebornMap, &pos))
				this->SetRecordPos(idRebornMap, pos.x, pos.y);
		}*/
	}
	DEBUG_CATCH("GotoJail Crash!!!!!!!!!!");			
}
void CUser::GotoSelfJail(void)
{
	ProcessAction(1027/*脚本*/);
	//私人监狱
}
//////////////////////////////////////////////////////////////////////
// return false: can't fly
// call - may be delete this;
int CUser::FlyMap(OBJID idMap, int nPosX, int nPosY)
{
	if(m_pMap == NULL)			// not in map
	{
		STACK_DUMP();
		LOGWARNING("玩家出现连续切屏。");
		LOGWARNING("切屏玩家userName = %s。",GetName());
		return FLYMAP_ERROR;
	}
	if(!this->IsAlive())
	{
 		return FLYMAP_ERROR;
	}

	if(QueryBooth())
	{
		return FLYMAP_ERROR;
	}

	if(!CanBeSee())
	{
		return FLYMAP_ERROR;
	}
	//20090531haocq
// 	if (m_bAutoFightFlag && !this->IsAlive())//自动挂机时
// 	{
// 		return FLYMAP_ERROR;
// 	}

	if (idMap == ID_NONE)
		idMap = this->GetMapID();
	if (CanNot() && !CWargame::IsWarGameMap(GetMapID()))
	{
		return FLYMAP_ERROR;
	}

	PROCESS_ID idProcess = MapManager()->GetProcessID(idMap);

// 	if (idProcess==PROCESS_NONE && MapManager()->IsMapDyna(idMap))///
// 		idProcess=MSGPORT_MAPGROUP_INSTANCE;
	CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess));
	if(idProcess == m_idProcess)
	{
		if (idMap==1048)
		{
			if (g_SpecilLine==0)
			{
				LeaveMap();
				EnterSynMap(nPosX,nPosY);
				return FLYMAP_NORMAL;
			}
			else
				idMap=GetSynMapID();
		}
		CGameMap* pNewMap  = MapManager()->GetGameMap(idMap);
		if(!pNewMap || !pNewMap->IsValidPoint(nPosX, nPosY))
		{
				return FLYMAP_ERROR;
		}
		if(g_SpecilLine==1)
		{
			DEBUG_TRY	// VVVVVVVVVVVVVV
			if (pNewMap->GetType()!=2 && pNewMap->GetType()!=3)//专线
			{
				LeaveMap();
				ChangeLineMap(idMap,nPosX,nPosY,RANDLINE);
				return FLYMAP_NORMAL;
			}
			DEBUG_CATCH("FlyMap")
		}
		if (g_SpecilLine==0)
		{
			DEBUG_TRY	// VVVVVVVVVVVVVV
			if (pNewMap->GetType()==2)//专线
			{
				LeaveMap();
				if (pNewMap->IsSynMap())//帮派
					EnterSynMap(nPosX,nPosY);
				return FLYMAP_NORMAL;
			}
			DEBUG_CATCH("FlyMap")
		}
		DEBUG_TRY	// VVVVVVVVVVVVVV
		LeaveMap();
		m_pMap		= pNewMap;
		m_nPosX		= nPosX;
		m_nPosY		= nPosY;

		int x = GetPosX();
		int y = GetPosY();

		CMsgAction	msg;
		if(msg.Create(GetID(), nPosX, nPosY, GetDir(), actionFlyMap, pNewMap->GetDocID(),m_pMap->GetID(),true))
			SendMsg(&msg);

//			EnterMap();

// 		CItem* pEquip = GetEquipItemByPos(/*ITEMPOSITION_EUEDMONL*/20);
// 		if(pEquip)
// 		{
// 			if(pEquip->GetStateWPSL())
// 			{
// 				KillEudemon(pEquip->GetStateWPSL(),false);					
// 				if(LoadEudemon(pEquip->GetStateWPSL(),&m_eudemonData[1]) && m_eudemonData[1].id != ID_NONE)
// 				{
// 					if(GetLev() < m_eudemonData[1].level)
// 					{
// 						this->SendSysMsg("你现在还不能驾驭这个宠物，请提升自己的等级后再召唤！");
// 					}
// 					else
// 					{
// 						m_eudemonData[1].ownerId = this->GetID();
// 						m_eudemonData[1].stutes = 2;
// 						if(m_eudemonData[1].life > 0)
// 						{							
// 							CallEudemon(m_eudemonData[1], GetPosX(), GetPosY(),false,true);
// 						}
// 						else
// 						{
// 							CallEudemon(m_eudemonData[1], GetPosX(), GetPosY(),false,false);
// 						}
// 						
// 						CMsgEudemonInfo msgEudemon;
// 						if(msgEudemon.Create(m_eudemonData[1]))
// 							SendMsg(&msgEudemon);
// 					}
// 				}					
// 			}
// 		}
		DEBUG_CATCH("FlyMap")		// AAAAAAAAAAA		
		return FLYMAP_NORMAL;
	}
	else
	{

		ChangeMapGroup(idProcess, idMap, nPosX, nPosY);					// call - delete this;
// 		CGameMap* pMap = ::MapGroup(idProcess)->GetMapManager()->GetGameMap(idMap);
// 		if (pMap)
// 		{
// 			//goto 通知队友
// 			SendPosToTeamMem(pMap->GetDocID(), nPosX, nPosY);
// 		}
		return FLYMAP_MAPGROUP;
	}
}

bool CUser::CanNot()
{
	if (!this->GetMap())
		return true;

	if (this->GetMap()->isDisableLeave() && this->GetPk()>0)
	{
		this->SendSysMsg("红名无法离开此地图");
		return true;
	}
	return	false;
}

//////////////////////////////////////////////////////////////////////
void CUser::OnTimer(time_t tCurr)
{
	if(!m_pMap)
		return;

	if(IsClosing())//正在关闭中
		return;

	if(!CanBeSee())
		return;

	DEBUG_TRY
	DEADLOOP_CHECK(PID, "ProcessAutoAttack")
	ProcessAutoAttack();
	DEBUG_CATCH("CUser ProcAttack_Hand2Hand carsh")



	DEBUG_TRY
	CheckItemLifeTime(true);
	DEBUG_CATCH("CUser CheckItemLifeTime() carsh")

	DEBUG_TRY
	CheckVipTime();
	DEBUG_CATCH("CUser CheckVipTime();")

	DEBUG_TRY
	CheckVipFaceTime();
	DEBUG_CATCH("CUser CheckVipFaceTime();")

	DEBUG_TRY
	CheckEudeLifespan();
	DEBUG_CATCH("CUser CheckEudeLifespan();")

	DEBUG_TRY
	ProcessMineTimer();
	DEBUG_CATCH("CUser ProcessMineTimer();")

	DEBUG_TRY
	if( m_tAutoHealRecover.ToNextTime() )
	{
		ProcAutoHealRecover();				
	}
	DEBUG_CATCH("CUser ProcAutoHealRecover() carsh")
	
	DEBUG_TRY
	DEADLOOP_CHECK(PID, "QueryMagic()->OnTimer")
	if (QueryMagic())
		QueryMagic()->OnTimer(tCurr);
	DEBUG_CATCH("CUser magic timer crash.")

	OBJID idBuff = 0;
	DEBUG_TRY
	DEADLOOP_CHECK(PID, "status")
//modify code by 林德渊 begin 2011.6.22
	int retvalue = 0;
//modify code by 林德渊 end
	for(int i = QueryStatusSet()->GetAmount()-1; i >= 0; i--)
	{
		IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
		if(pStatus)
		{
			DEBUG_TRY
			idBuff = pStatus->GetID();
			if (!pStatus->OnTimer(tCurr))
				break;
			if (this->IsAlive() && !pStatus->IsValid())
			{
				DetachStatus(this,pStatus->GetID());
//modify code by 林德渊 begin 2011.6.22
				DEBUG_TRY
				if( idBuff == STATUS_RETORT_ZHF )
					this->QueryMagic()->ProcessBuffBomb(this,1);
				if( idBuff == STATUS_RETORT_MDSX_1 )
				{
					if( this->GetNewValue() >= 3000 && this->GetNewValue() <= 35000 )
						retvalue = this->QueryMagic()->ProcessBuffBomb(this,3);//减血并减buff1
				}
				DEBUG_CATCH2("DetachStatus(this,pStatus->GetID()) %u",pStatus->GetID());
//modify code by 林德渊 end
			}
			DEBUG_CATCH2("DetachStatus(this,pStatus->GetID()) %u",pStatus->GetID());
		}
	}
//modify code by 林德渊 begin 2011.6.22
	DEBUG_TRY
	if( 4 == retvalue || 5 == retvalue )
	{
		this->DetachStatus(this,STATUS_RETORT_MDSX);
		if( 4 == retvalue )
			this->BeKillBySys();
	}
	DEBUG_CATCH("if( 4 == retvalue || 5 == retvalue )");
//modify code by 林德渊 end
	DEBUG_CATCH2("CUser status timer crash. buffid = %u",idBuff)

	DEBUG_TRY
	DEADLOOP_CHECK(PID, "ProcessPkTimer")
	if (this->GetMap() && !this->GetMap()->isDisableReducePk())
	{
		if (m_tPK.ToNextTick(PK_DEC_TIME))					//pk值变化的处理
			this->ProcessPkTimer();
	}
	DEBUG_CATCH("CUser pk timer crash.")

	DEBUG_TRY
	DEADLOOP_CHECK(PID, "PorcessFatigue")
	if (this->isFatigue() &&  (m_data.GetFatiTime() > 0) && g_bEnableFatigue)
	{
		if (m_tFati.ToNextTick(FATIGUE))					//防沉迷
			this->ProcessFatigue();
	}
	DEBUG_CATCH("CUser Fatigue timer crash.")

	DEBUG_TRY
	if (m_tHpMpAdd.ToNextTick(ESEPCIAL_DRUG_SEC))					//特殊大红大蓝处理
		this->SpendNewRemedy();
	DEBUG_CATCH("CUser HpMpAdd timer crash.")

	DEBUG_TRY
	if (m_EverydayOL.ToNextTick(EverdayOL_SEC))					//在线计时
		this->ProcessEverydayOL();
	DEBUG_CATCH("CUser ProcessEverydayOL timer crash.")
	
// 	DEBUG_TRY
// 	DEADLOOP_CHECK(PID, "ProcessPkTimer")
// 	if (m_tPK_InPrison.ToNextTime()) 
// 		this->ProcessPkTimer_InPrison();
// 	DEBUG_CATCH("CUser pk inprison timer crash.")

// 	DEBUG_TRY
// 	if (m_tSynWork.TimeOver(SYN_WORK_SEC))					//打工
// 	{
// 		this->ProcessWork();
// 	}
// 	DEBUG_CATCH("SYNWORK TIMER CRASH.")

	
	DEBUG_TRY
	SYSTEMTIME curTime;
	::GetLocalTime( &curTime );
	if(!m_bResetDarTimeSet) //第一次重置
	{
		int nCheck = m_data.GetLastLogin();
		if(nCheck == 0)
		{
			m_data.SetLastLogin();
		}
		else
		{
			//m_tLastResetVasExchangeTime.wDay = m_data.GetLastLogin() % 100; //考虑检查时间当天的5点与现在时间的差值
			CTime timecount(nCheck/10000,(nCheck % 10000)/100,nCheck % 100,5,0,0);
			CTime timeCurr(CTime::GetCurrentTime());
			CTimeSpan spanTime = timeCurr - timecount;
			int nPassDay = spanTime.GetTotalHours() / 24;

			if(nPassDay > 0) //过一天了
			{
				if(nPassDay > 6)
				{
					ResetWeekTaskMask();
				}
				else
				{
					int nCurWeekDay = timeCurr.GetDayOfWeek();
					int nLastWeekDay = nCurWeekDay - nPassDay;
					if(nLastWeekDay <= 0)
					{
						ResetWeekTaskMask();
					}
				}

				ResetDayTaskMask();/// 每天6点清除天任务掩码
				ResetDayTaskMask2();
				ResetMonsterKillAndEveryDayOL();
				SetVasTicketAmount(0);
				if(m_pUserWarGame)
					m_pUserWarGame->SetWarGameMask(0);

				if(curTime.wHour < 5)
				{
					CTimeSpan passDay(1,0,0,0); //一天前
					CTime timeWrite = timeCurr - passDay;
					m_tLastResetVasExchangeTime.wYear = timeWrite.GetYear();
					m_tLastResetVasExchangeTime.wMonth = timeWrite.GetMonth();
					m_tLastResetVasExchangeTime.wDay = timeWrite.GetDay();
					m_data.SetLastLogin(m_tLastResetVasExchangeTime.wYear * 10000 + m_tLastResetVasExchangeTime.wMonth * 100 + m_tLastResetVasExchangeTime.wDay);
				}
				else
				{
					::GetLocalTime( &m_tLastResetVasExchangeTime );
					m_data.SetLastLogin();
				}
			}
			else
			{
				m_tLastResetVasExchangeTime.wYear = nCheck/10000;
				m_tLastResetVasExchangeTime.wMonth = (nCheck % 10000)/100;
				m_tLastResetVasExchangeTime.wDay = nCheck % 100;
			}
			m_bResetDarTimeSet = true;
		}
	}
	else if(m_tLastResetVasExchangeTime.wDay != curTime.wDay)
	{
		//最后更新和现在不是同一天
		if(curTime.wHour == 5 /*&& curTime.wMinute == 00*/)//6:00
		{
			ResetDayTaskMask();///每天5点清除天任务掩码
			ResetDayTaskMask2();
			ResetMonsterKillAndEveryDayOL();
			SetVasTicketAmount(0);
			if(curTime.wDayOfWeek == 1)
			{
				ResetWeekTaskMask();
			}

			::GetLocalTime( &m_tLastResetVasExchangeTime );
			m_data.SetLastLogin();

			if(m_pUserWarGame)
				m_pUserWarGame->SetWarGameMask(0);
		}
	}
	DEBUG_CATCH("ResetDayTaskMask")

	//goto 新的银两和经验存储机制
// 	DEBUG_TRY
// 	if (abs(m_MoneyIncr)>30000 || abs(m_ExpIncr)>(750*GetLev()+10500))
// 	{
// 		if (!m_data.SaveInfo())
// 			::LogSave("MoneyAndExpIncCheckSave failed userid=%u", GetID());
// 		m_MoneyIncr = 0;
// 		m_ExpIncr = 0;
// 	}
// 	DEBUG_CATCH("MoneyAndExpIncCheckSave")

	DEBUG_TRY
	static int lastUpdateTime = clock();
	if(lastUpdateTime / 500 != clock() / 500)//把密集时间分开
	{
		if (m_tUpdateDatabase.ToNextTick(UNDATEDATABASE_BASETIME + UNDATEDATABASE_OFFTIME))					//写数据库
		{
			SaveInfo(SAVETYPE_UPDATE);
			lastUpdateTime = clock();
		}
	}
	DEBUG_CATCH("CUser ProcessEverydayOL timer crash.")

	DEBUG_TRY
	if (m_pPetEgg)
		m_pPetEgg->OnTimer(tCurr);
	DEBUG_CATCH("CUser OnTimer m_pPetEgg->OnTimer(tCurr) crash.")
}

//////////////////////////////////////////////////////////////////////
//06.3.21王玉波修改....
void CUser::ProcessAutoAttack		()
{
	// 速度型幻兽合体提高攻击速度
//	int nFightPause = m_nFightRate/* + m_nFightPause*/;

	if (m_pBattleSystem && m_pBattleSystem->IsActived() && m_tFight.ToNextTick(800)/*ToNextTick(nFightPause)*/ )
	{
// 		if( !QueryMagic()/*|| !QueryMagic()->IsActive() || QueryMagic()->IsCanPhysicalAttack() */)
// 		{
// 			if (QueryMagic() && ( ::TimeGet(TIME_MILLISECOND)-QueryMagic()->GetPublicCDTime()<= QueryMagic()->GetPublicCDInterval()))
// 			{
// 				return;
// 			}
// 			if (QueryMagic())
// 			{
// 				QueryMagic()->setMagicState(4);
// 			}	
//			CheckCrime(this);
			m_pBattleSystem->ProcAttack_Hand2Hand();
//			m_nFightPause	= 0;
//			if(m_pBattleSystem->GetTargetID() != ID_NONE)
//				ResetEnergyInc();
//		}
// 		else if(QueryMagic() && QueryMagic()->getMagicState() == 4)
// 		{
// 			QueryMagic()->setMagicState(0);
// 		}
	}
}

void CUser::ReSetUserPoint(int nType)
{
	int nAddPoint = 0;
	DWORD prof = this->GetProfession(nType);

	if( prof & PROF_NEWHAND) //新手
	{
		m_data.SetStr( 36* GetLev() + 94,nType);
		m_data.SetIntex( 36* GetLev() + 94,nType);
		m_data.SetSta( 36* GetLev() + 94,nType);
		m_data.SetSpi( 36* GetLev() + 94,nType);
		m_data.SetAgi( 36* GetLev() + 94,nType);
		m_data.SetPot(0 ,nType);
	}
	else if( prof & PROF_ASSIST) //辅助
	{
		m_data.SetStr( 42* GetLev() + 27,nType);
		m_data.SetIntex( 57* GetLev() - 116,nType);
		m_data.SetSta( 57* GetLev() - 116,nType);
		m_data.SetSpi( 68* GetLev() - 149,nType);
		m_data.SetAgi( 42* GetLev() - 73,nType);

		nAddPoint = 0;
		if(GetJobLev(nType) > 24)
			nAddPoint += (GetJobLev(nType) - 24) * 2;
		if(GetJobLev(nType) > 60)
			nAddPoint += (GetJobLev(nType) - 60) * 1;

		m_data.SetPot(max(0,nAddPoint),nType);
	}
	else if( prof & PROF_MASTER) //法师
	{
		m_data.SetStr( 37* GetLev() + 42,nType);
		m_data.SetIntex( 62* GetLev() - 131,nType);
		m_data.SetSta( 56* GetLev() - 113,nType);
		m_data.SetSpi( 68* GetLev() - 149,nType);
		m_data.SetAgi( 37* GetLev() - 58,nType);

		nAddPoint = 0;
		if(GetJobLev(nType) > 24)
			nAddPoint += (GetJobLev(nType) - 24) * 2;
		if(GetJobLev(nType) > 60)
			nAddPoint += (GetJobLev(nType) - 60) * 1;

		m_data.SetPot(max(0,nAddPoint),nType);
	}
	else if( prof & PROF_PASTOR) //牧师
	{
		m_data.SetStr( 43* GetLev() - 75,nType);
		m_data.SetIntex( 62* GetLev() - 131,nType);
		m_data.SetSta( 50* GetLev() - 95,nType);
		m_data.SetSpi( 68* GetLev() - 149,nType);
		m_data.SetAgi( 43* GetLev() - 76,nType);
		
		nAddPoint = 0;
		if(GetJobLev(nType) > 24)
			nAddPoint += (GetJobLev(nType) - 24) * 2;
		if(GetJobLev(nType) > 60)
			nAddPoint += (GetJobLev(nType) - 60) * 1;

		m_data.SetPot(max(0,nAddPoint),nType);
	}
	else if( prof & PROF_ASSISSIN) //刺客
	{
		m_data.SetStr( 62* GetLev() - 131,nType);
		m_data.SetIntex( 50* GetLev() - 95,nType);
		m_data.SetSta( 62* GetLev() - 131,nType);
		m_data.SetSpi( 50* GetLev() - 95,nType);
		m_data.SetAgi( 62* GetLev() - 131,nType);
		
		nAddPoint = 0;
		if(GetJobLev(nType) > 24)
			nAddPoint += (GetJobLev(nType) - 24) * 2;
		if(GetJobLev(nType) > 60)
			nAddPoint += (GetJobLev(nType) - 60) * 1;

		m_data.SetPot(max(0,nAddPoint),nType);
	}
	else if( prof & PROF_SOLDIER) //战士
	{
		m_data.SetStr( 56* GetLev() - 113,nType);
		m_data.SetIntex( 37* GetLev() + 42,nType);
		m_data.SetSta( 74* GetLev() - 167,nType);
		m_data.SetSpi( 68* GetLev() - 149,nType);
		m_data.SetAgi( 37* GetLev() - 58,nType);
		
		nAddPoint = 0;
		if(GetJobLev(nType) > 24)
			nAddPoint += (GetJobLev(nType) - 24) * 2;
		if(GetJobLev(nType) > 60)
			nAddPoint += (GetJobLev(nType) - 60) * 1;

		m_data.SetPot(max(0,nAddPoint),nType);
	}
	else if( prof & PROF_SWORD) //弓手
	{
		m_data.SetStr( 62* GetLev() - 131,nType);
		m_data.SetIntex( 50* GetLev() - 95,nType);
		m_data.SetSta( 62* GetLev() - 131,nType);
		m_data.SetSpi( 50* GetLev() - 95,nType);
		m_data.SetAgi( 62* GetLev() - 131,nType);
		
		nAddPoint = 0;
		if(GetJobLev(nType) > 24)
			nAddPoint += (GetJobLev(nType) - 24) * 2;
		if(GetJobLev(nType) > 60)
			nAddPoint += (GetJobLev(nType) - 60) * 1;

		m_data.SetPot(max(0,nAddPoint),nType);
	}
}

void CUser::SetProfession(int nType,DWORD dwProff,  BOOL bUpdate)
{ 
	CUserKey keyInfo;
	if(nType == 0)
	{
		m_data.SetProfession(dwProff,m_data.GetCurJob());
		m_data.SetJobLev(1,m_data.GetCurJob(),bUpdate);
		ReSetUserPoint(m_data.GetCurJob());
	}
	else  
	{
		m_data.SetProfession(dwProff,nType);
		m_data.SetJobLev(1,nType,bUpdate);
		ReSetUserPoint(nType);
	}

	CMsgUserAttrib	msg;
	if(nType == 1)
	{
		msg.Create(GetID(),_USERATTRIB_PORFESSION,GetProfession(nType));
		msg.Append(_USERATTRIB_FIRJOBLEV,GetJobLev(nType));
	}	
	else
	{
		msg.Create(GetID(),_USERATTRIB_PORFESSION_SEC,GetProfession(nType));
		msg.Append(_USERATTRIB_SECJOBLEV,GetJobLev(nType));
	
	}

	if(GetCurJob() == nType)
	{
		BroadcastRoomMsg(&msg,true,EM_SENDNPCFLAG_FALSE);
		UpdateCalculatedProperty(UPDATECALTYPE_ALL);
	}
	else
	{
		SendMsg(&msg);
	}
}
//////////////////////////////////////////////////////////////////////
void CUser::ClearAllStatus		()
{

	for(int i = QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
		if(pStatus)
		{
		//	QueryStatusSet()->DelObj(pStatus->GetID());
			CRole::DetachStatus( this->QueryRole(),pStatus->GetID() );
		}
	}
}

void CUser::ProcessDeadAddTeamAttr()
{
	CTeam *pTeam = this->GetTeam();

	if(NULL == pTeam)
		return ;

	for ( int i = 0; i < pTeam->GetMemberAmount(); i++ )
	{
		CUser* pUser	= UserManager()->GetUser( pTeam->GetMemberByIndex( i ) );
		if ( !pUser )		
			continue;
		
		if ( pUser->GetMapID() != this->GetMapID() )
			continue;

		if( !(pUser->IsAlive()))
			continue;

		if( abs( pUser->GetPosX() - this->GetPosX() ) > _RANGE_EXPSHARE ||
			abs( pUser->GetPosY() - this->GetPosY() ) > _RANGE_EXPSHARE )
			continue; 

		int nAddLife  = pUser->GetMaxLife()-pUser->GetLife();
		int nAddMana  = pUser->GetMaxMana()-pUser->GetMana();
		if(nAddLife || nAddMana)
		{
			if(nAddLife)
		    	pUser->AddAttrib(_USERATTRIB_LIFE,  nAddLife, SYNCHRO_TRUE);
			if(nAddMana)
				pUser->AddAttrib( _USERATTRIB_MANA, nAddMana,  SYNCHRO_TRUE );

			char text[128];
			sprintf(text, "您的队友%s已经死亡，他在临死前为您加满了血和魔法", GetName());
			pUser->SendSysMsg(text);
		
			CMsgTeam msg;
			if(msg.Create(pUser->GetID(), _MSGTEAM_MEMADDLIFE_EFFECT, SECONDKEEPEFFECT_LIFESTONE))
				pUser->BroadcastRoomMsg(&msg, SYNCHRO_TRUE);
		}
	}
}

void CUser::ClearManyStatusWhenDie()//-------彭峰---2007/05/16-----
{
	for(int i = QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
		if(pStatus)
		{
			///彭正帮2007-07-13在此将死亡不清除的状态进行拆分.
//			m_data.m_Info.i64Effect	&= KEEPEFFECT_NOT_CLEAR;//死亡不清除的状态，原来有就有，原来无就无，用&
			//////////////////////////////////////////////////////////////////////////
			//	20070717 BUG0001013 朱斌 死亡后去掉灰名状态
			//	if (STATUS_CRIME == pStatus->GetID()) 
			//	 continue;
			if (STATUS_TEAMLEADER == pStatus->GetID()) 
				 continue;
			if (STATUS_GETEXPUP_PERCENT == pStatus->GetID()) 
				 continue;
			if (STATUS_GETEXPUP_PERCENT_SYNINS == pStatus->GetID())
				 continue;
			if( STATUS_MOUNTSPEED_NUMBER == pStatus->GetID() )
				continue;
			if( STATUS_PHYATTACKUP_TUTOR_PERCENT == pStatus->GetID() )//  [陈剑飞10/26/2007]
				continue;
			if( STATUS_MAGATTACKUP_TUTOR_PERCENT == pStatus->GetID() )//  [陈剑飞10/27/2007]
				continue;
			if( STATUS_FISH_WORKER == pStatus->GetID() )//  渔工死亡不清除
				continue;
			if( STATUS_FISH_TUTOR == pStatus->GetID() )//  钓鱼大师死亡不清除
				continue;
			if( STATUS_CITYWAR_ATTACKMARK == pStatus->GetID() )
				continue;
			if( STATUS_CITYWAR_DEFENCEMARK == pStatus->GetID() )
				continue;
			if( STATUS_LIANGONG == pStatus->GetID() )
				continue;
			if(STATUS_ACTIONEND == pStatus->GetID())
				continue;
			if( STATUS_DEAD_ADDTEAMATTR == pStatus->GetID() ) //物品血晶石功能
				ProcessDeadAddTeamAttr();
			if (STATUS_ACTIVATE_ADDEXP == pStatus->GetID())
				continue;
			if (STATUS_DOT_TUTOREXP == pStatus->GetID())
				continue;
			if (STATUS_ITEM_ADDEXP == pStatus->GetID())
				continue;
			if (STATUS_NOXDEF_UP == pStatus->GetID())
				continue;
			if (STATUS_TUTOR_BOSS == pStatus->GetID())
				continue;
			if (STATUS_DOT_ADDEXP == pStatus->GetID())//活动用
				continue;
			//2007827修罗:以下荣誉buff死亡不清除--------------
			if( pStatus->GetID() == STATUS_PHYSICALATTACKUP_PERCENT_WS
				|| pStatus->GetID() == STATUS_MAGICATTACKUP_PERCENT_ED
				|| pStatus->GetID() == STATUS_DAMAGEDOWN_PERCENT_TN
				|| pStatus->GetID() == STATUS_GETEXPUP_PERCENT_WT
				|| pStatus->GetID() == STATUS_PHYSICALDEFENCEUP_NUMBER_SF
				|| pStatus->GetID() == STATUS_MAGICDEFENCEUP_NUMBER_EM
				|| pStatus->GetID() == STATUS_CRITRATEUP_NUMBER_SW
			//	|| pStatus->GetID() == STATUS_SYNWORKTIME//帮派打工buff -- jay
// 				|| pStatus->GetID() == STATUS_DOT_ADDLIFE_RB
// 				|| pStatus->GetID() == STATUS_DOT_ADDMANA_MC
				|| pStatus->GetID() == STATUS_STRUGGLE_RELIVE
				|| pStatus->GetID() == STATUS_ENEMYFIRE ///死亡不清仇视的buff-softworms-2007-11-16
				|| pStatus->GetID() == STATUS_BIZ_DEAD
				|| pStatus->GetID() == STATUS_DOT_WARGAMEEXP
				|| pStatus->GetID() == STATUS_VERIFICATION_RUNNING
				|| pStatus->GetID() == STATUS_VERIFICATION_WAIT
				)
				continue;
			//------------------------------------------------

			//  [2007-9-25] 朱斌	死亡不清战争活动的BUF
			if(pStatus->GetID() == STATUS_WARGAME)
				continue;
//			 if( STATUS_GETEXPUP_PERCENT == pStatus->GetID() )
//			 {
//				 continue;
//			 }
//			 else
//			 {
			//	ClsBuff( pStatus->GetID() );
			//	QueryStatusSet()->DelObj(pStatus->GetID());
				 CRole::DetachStatus( this->QueryRole(),pStatus->GetID() );
//			 }
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessPkTimer()
{
	if (!this->GetMap())
		return;
	if (this->GetPk() > 0)
	{
		if (!this->GetMap()->isDisableReducePk())
		{
		//问天没有监狱,屏蔽
// 		CGameMap* pMap = MapManager()->QueryMap(this->GetMapID());
// 		IF_OK (pMap) 
// 		{
// 			if (pMap->IsPrisonMap())
// 			{
// 				this->AddPk( -1 * PKVALUE_DEC_ONCE * 2 );//在监狱2倍减少
// 				return;
// 			}
// 		}
			this->AddPk( -1*PKVALUE_DEC_ONCE );
		}
	}
}
// void CUser::ProcessPkTimer_InPrison()
// {
// 	if (this->GetPk() > 0)
// 	{
// 		 		CGameMap* pMap = MapManager()->QueryMap(this->GetMapID());
// 		 		IF_OK (pMap) 
// 				{
// 		 			if (this->GetMapID() == MAP_PRISON)
// 		 			{
// 		 				this->AddPk( -1 * PKVALUE_DEC_ONCE);
// 						return;
// 					}
// 		 		}
// 	}
// 	//    >
//  	char  szMsgxyz[256] = "";
// // 	CMsgTalk msg;
// // 	char szName[20] = "新手小礼包";
// //	sprintf(szMsgxyz, "<font color='#e46d0a'>{%s}</font>心怀仁慈，在峨眉山救助伤患时经峨眉掌门提拔，成为了一名<font color='#cc66ff'>峨嵋派弟子</font>!",this->GetName());
// // 	this->GetItemInfoByItemid(4091);
// // //	msg.Create(this->GetName(), ALLUSERS_NAME, szMsgxyz, 0, 0xffffff, 0, _TXTATR_ANNOUNCE, 300001, 300001, "止血丹" );
// // /*	UserManager()->BroadcastMsg("你们这是自寻死路你们这是自寻死路你们这是自寻死路你们这是自寻死路", NULL/ *pUser* /, NULL, 0xffffff, _TXTATR_ANNOUNCE);*/
// // //	sprintf(szMsgxyz, "<font color='#e46d0a'>{%s}</font>心怀仁慈，在峨眉山救助伤患时经峨眉掌门提拔，成为了一名<font color='#cc66ff'>峨嵋派弟子</font>!", this->GetName());
// // //	UserManager()->BroadcastMsg("<font color='#e46d0a'>321</font>心怀仁慈，在峨眉山救助伤患时经峨眉掌门提拔，成为了一名<font color='#cc66ff'>峨嵋派弟子</font>!", NULL, NULL,0xffffff, _TXTATR_ANNOUNCE);
// //		UserManager()->BroadCastMsgAndReceiveName(szMsgxyz, NULL, NULL, 0xffffff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
// // //	SendSysMsg(szMsgxyz);
// //	UserManager()->BroadcastMsg(&msg, ALLUSERS_NAME)
// }
//////////////////////////////////////////////////////////////////////
bool CUser::IsPkEnable(IRole* pRole)
{
	//20070315修罗:所有判断拿到CUser::IsAtkable()里去
	if (!pRole || this->GetID() == pRole->GetID())
		return false;
	
	CMonster* pMonster = NULL;
	if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
		// 种族修改   08.6.12  桂佳 
//		{
//			if(this->GetPhyle() == PROF_DRAGON && strstr(pMonster->GetName(),"龙族") != 0)
//				return  false;
//			else if(this->GetPhyle() == PROF_HUMAN && strstr(pMonster->GetName(),"天人族") != 0)
//				return  false;
//			else if(this->GetPhyle() == PROF_GENE && strstr(pMonster->GetName(),"基因族") != 0)
//				return  false;
//		}
		if (pMonster->IsMonster() || pMonster->IsSynPet())
			return true;
	}

	CNpc* pNpc = NULL;
	pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc));
	
	if (pNpc)
		return true;

 	bool bCanPk = false;
// 	switch(this->GetPkMode())
// 	{
// 	case	PKMODE_FREE:
// 		bCanPk = true;
// 		break;
// 
// 	case	PKMODE_TEAM:               //change huang 2004.1.15
// 		{
// 			bCanPk = true;
// 
// 			CTeam* pTeam = this->GetTeam();
// 			if(pTeam && pTeam->IsTeamMember(pRole->GetID()))
// 				bCanPk = false;
// 			
// 			if (this->GetFriend(pRole->GetID()))
// 				bCanPk = false;
// 			
// 			CUser* pUser = NULL; 
// 			if(pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
// 			{
// //				if(this->IsMate(pUser))
// //					bCanPk = false;
// 			}	
// 			return bCanPk;
// 		}
// 		break;
// 		
// 	case    PKMODE_SYNDICATE:     //add huang 2004.1.15
// 		{
// 			bool bCanPk = true;
// 
// 			CUser* pUser = NULL;
// 			if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)) && this->GetSynID() != ID_NONE)
// 			{
// 				if (this->GetMasterSynID() == pUser->GetMasterSynID() /*|| this->IsMate(pUser)*/)
// 					bCanPk = false;
// 				else
// 				{				
// 					CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetMasterSynID());
// 					if (pSyn)
// 					{
// 						if ( pSyn->IsFriendly(pUser->GetSynID()))
// 							bCanPk = false;
// 					}
// 				}
// 			}
// 			return bCanPk;
// 		}
// 		break;
// 	case	PKMODE_ARRESTMENT:
// 		{
// 			if(pRole->IsEvil())
// 				bCanPk = true;
// 		}
// 		break;
// 
// 	case	PKMODE_SAFE:
// 		{
// 			CUser* pUser = NULL;
// 			if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
// 			{
// 			//	if(this->GetPhyle() != pUser->GetPhyle())
// 					return true;
// 			}
// 		}
// 		bCanPk = false;
// 		break;
// 
// 	default:
// 		ASSERT(!"switch pk mode");
// 		bCanPk = false;
// 		break;
// 	}

	return bCanPk;
}

//////////////////////////////////////////////////////////////////////
//2007828修罗:
//void CUser::Reborn(bool bChgPos/*=true*/)
void CUser::Reborn(bool bChgPos/*=true*/,
				   bool bRebornFlag/*=false*/,
				   int nRecoverPercent/*=0*/)
{
	if (this->IsAlive())
		return;

	DetachStatus( QueryRole(),STATUS_DIE );
	m_bGhost	= false;

	if( bRebornFlag )//复活术复活“我”
	{
		SetAttrib( _USERATTRIB_LIFE,MulDiv( GetMaxLife(),nRecoverPercent,100 ),SYNCHRO_TRUE );
		SetAttrib( _USERATTRIB_MANA,MulDiv( GetMaxMana(),nRecoverPercent,100 ),SYNCHRO_TRUE );
		SetAttrib( _USERATTRIB_SP,MulDiv( GetMaxSp(),nRecoverPercent,100 ),SYNCHRO_TRUE );

	}
	else
	{
		SetAttrib(_USERATTRIB_LIFE, GetMaxLife() / 10/*2*/, SYNCHRO_TRUE);
		SetAttrib(_USERATTRIB_MANA, GetMaxMana() / 10/*2*/, SYNCHRO_TRUE);
		SetAttrib( _USERATTRIB_SP,GetMaxSp() / 2,SYNCHRO_TRUE );
		
	}
	SetAttrib( _USERATTRIB_KEEPEFFECT, GetEffect(), SYNCHRO_BROADCAST );

	const bool bMaxLife = true;
	this->BroadcastTeamLife(bMaxLife);
    
	//06.10.13王玉波:因为人物复活后,如果不是同一地图组,会出问题,所以修改代码
	//----------begin------------------------------------------------------------------------
	int nFlyMapResult = -1;
	if (bChgPos)
	{
		if (ID_NONE != m_data.GetRecordMap())
			nFlyMapResult = this->FlyMap(m_data.GetRecordMap(), m_data.GetRecordX(), m_data.GetRecordY());
	}

	CMsgAction msgAction;
	if(msgAction.Create(this->GetID(), this->GetPosX(), this->GetPosY(), this->GetDir(), actionStandBy))
	{
		this->BroadcastRoomMsg(&msgAction, true);
	}
}

//////////////////////////////////////////////////////////////////////
void CUser::TransformGhost()
{
	m_bGhost = true;

//	SetAttrib(_USERATTRIB_KEEPEFFECT, SetEffect(KEEPEFFECT_GHOST), SYNCHRO_BROADCAST);
	SetAttrib(_USERATTRIB_LOOKFACE, GetLookFace(), SYNCHRO_BROADCAST);
}

int CUser::AddNationCon(int nAdd)
{
	DEBUG_TRY
	ASSERT(nAdd != 0);
	int nData = m_data.GetNationCon();
	nData += nAdd;
	m_data.SetNationCon(nData);
	DEBUG_CATCH("AddNationCon Crash!!!!");
	return 0;
}

int CUser::SetNationCon(int nData)
{
	DEBUG_TRY
	m_data.SetNationCon(nData);
	DEBUG_CATCH("SetNationCon Crash!!!!");
	return 0;
}

int CUser::AddChivalrous(int nAdd)
{
	DEBUG_TRY
	ASSERT(nAdd != 0);
	int nData = m_data.GetGoodEvil();
	nData += nAdd;
	m_data.SetGoodEvil(nData);
	DEBUG_CATCH("AddChivalrous Crash!!!!");
	return 0;
}

int CUser::SetChivalrous(int nData)
{
	DEBUG_TRY
	m_data.SetGoodEvil(nData);
	DEBUG_CATCH("SetChivalrous Crash!!!!");
	return 0;
}

//////////////////////////////////////////////////////////////////////
// 用于任务系统
//////////////////////////////////////////////////////////////////////
int CUser::AddPk(int nAdd)			// auto synchro
{
	DEBUG_TRY
	ASSERT(nAdd != 0);

	if (this->GetPk() == 0)
	{
		m_tPK.SetInterval(PK_DEC_TIME);// every 1 minutes reduce 2 pk value //change huang 2004.1.11
	}

	if(abs(m_data.GetPk() + nAdd) < MAX_PKLIMIT)
		m_data.SetPk(__max(0, m_data.GetPk() + nAdd));
	else
	{
		m_data.SetPk(MAX_PKLIMIT);
	}

// 	if(nAdd > 0)
// 	{
// 		SetStatus(STATUS_PKVALUE, SYNCHRO_TRUE);
	this->SetAttrib(_USERATTRIB_PK, this->GetPk(), SYNCHRO_BROADCAST);
	// 	else
	// 		ClsStatus(STATUS_PKVALUE, SYNCHRO_TRUE);
	
/*	this->SetAttrib(_USERATTRIB_PK, this->GetPk(), SYNCHRO_TRUE);*/
	DEBUG_CATCH("ADDPK Crash!!!!");
	return m_data.GetPk();
}

//////////////////////////////////////////////////////////////////////
//06.11.7王玉波修改
//此函数每10秒调用一次,要考虑到服务器效率问题
void CUser::ProcAutoHealRecover()
{
	int nVit = m_data.GetVit();
	if(nVit < m_data.GetMaxVit())
	{
		m_data.SetVit(nVit + 1);
		CMsgUserAttrib	msg;
		if(msg.Create(GetID(),_USERATTRIB_VIT,m_data.GetVit()))
			SendMsg(&msg);
	}
	int nEne = m_data.GetEne();//goto 体力
	if (nEne < m_data.GetMaxEne())
	{
		m_data.SetEne(nEne + 1);
		CMsgUserAttrib	msg;
		if(msg.Create(GetID(),_USERATTRIB_ENE,m_data.GetEne()))
			SendMsg(&msg);
	}
}
	
//////////////////////////////////////////////////////////////////////
void CUser::ProcessTick(DWORD dwClientTime)
{
//#ifdef _DEBUG
//	return;
//#endif

	if (m_dwFirstClientTick == 0)
		m_dwFirstClientTick = dwClientTime;

	if (dwClientTime < m_dwLastClientTick)
	{	// ridiculous time stamp
		this->SendSysMsg(g_objStrRes.GetStr( 12033));//STR_CONNECTION_OFF
		UserManager()->KickOutSocket(m_idSocket, "ProcessTick");
		return;
	}

	const int CRITICAL_TICK_PERCENT = 5;

	int nServerTicks = m_dequeServerTick.size();
	if (m_dwLastClientTick != 0 && 
			nServerTicks >= 2 &&
				dwClientTime > m_dwLastClientTick+TICK_SECS*(100+CRITICAL_TICK_PERCENT)*10)	// *10 means 1000ms/100percent
	{	// suspicious time stamp		
		DWORD dwTimeServer = ::TimeGet();
		DWORD dwTimeServerTickInterval = m_dequeServerTick[1]-m_dequeServerTick[0];

		int nEchoTime = dwTimeServer-m_dequeServerTick[1];
		//20070516修罗:bug.加速误判.
		//以下公式在理论上并不正确,但已经做了比正确公式更大的容忍
		//现为解决bug.调整做更大的容忍.
		//调整的容忍值接近原消息等待延时THREADDELAY_MS
		//----------------------------------------------
		nEchoTime += 400;//ms
		//----------------------------------------------
		if (nEchoTime < (int)dwClientTime-(int)m_dwLastClientTick-(int)dwTimeServerTickInterval)
		{
			::GmLogSave("发现玩家（%s）使用加速器。", this->GetName());

			this->SendSysMsg( g_objStrRes.GetStr( 15059) );//"警告:使用加速器,系统已记录!"
			UserManager()->KickOutSocket(m_idSocket, "加速器");
			//20070516记录详细信息---------------------
			LOGERROR( "加速器详细信息:"
				"dwClientTime=%d,m_dwLastClientTick=%d,dwTimeServer=%d,m_dequeServerTick[1]=%d,dwTimeServer=%d,"
				"nEchoTime=%d,dwTimeServerTickInterval=%d",
				dwClientTime,m_dwLastClientTick,dwTimeServer,m_dequeServerTick[1],dwTimeServer,
				nEchoTime,dwTimeServerTickInterval);
			//-----------------------------------------
			return;
		}
	}

	m_dwLastClientTick = dwClientTime;

	if (m_dequeServerTick.size() >= 2)
		m_dequeServerTick.pop_front();

	m_dwLastRcvClientTick = ::TimeGet();
}


// friend
//////////////////////////////////////////////////////////////////////
//获取全部好友数量
int CUser::GetFriendAllAmount()
{
	return GetFriendGroupAmount(1) + GetFriendGroupAmount(2)  + GetFriendGroupAmount(3)  + GetFriendGroupAmount(4);
}

//获取指定分组的好友人数
int CUser::GetFriendGroupAmount(int groupid)
{
	int amount=0;
	for( int i=0; i<m_setFriend.size(); i++ )
	{
		CFriend* pFriend = m_setFriend[i];
		if( pFriend->GetFriendType() == groupid )
			amount++;
	}
	return amount;
}
CFriend* CUser::GetFriend(OBJID idFriend)
{
	for(int i = 0; i < m_setFriend.size(); i++)
	{
		CFriend* pFriend = m_setFriend[i];
		if(pFriend && pFriend->GetFriendID() == idFriend)
			return m_setFriend[i];
	}

	return NULL;
}

//  [2007-9-10] 朱斌
CFriend* CUser::GetFriendByName(char* pszName)
{
	for(int i = 0; i < m_setFriend.size(); i++)
	{
		CFriend* pFriend = m_setFriend[i];
		if(pFriend && strcmp(pFriend->GetFriendName(), pszName) == 0)
			return m_setFriend[i];
	}

	return NULL;
}
CFriend* CUser::GetBeFriend(OBJID idFriend)
{
	for(int i = 0; i < m_setBeFriend.size(); i++)
	{
		CFriend* pFriend = m_setBeFriend[i];
		if(pFriend && pFriend->GetUserID() == idFriend)
			return m_setBeFriend[i];
	}
	
	return NULL;
}
//goto 按名字找
CFriend* CUser::GetBeFriendByName(char* pszName)
{
	for(int i = 0; i < m_setBeFriend.size(); i++)
	{
		CFriend* pFriend = m_setBeFriend[i];
		if(pFriend && strcmp(pFriend->GetUserName(), pszName)==0)
			return m_setBeFriend[i];
	}
	
	return NULL;
}
//////////////////////////////////////////////////////////////////////////
bool CUser::MoveFriend(CFriend* pFriend,int nFriendType)
{
	if (!pFriend)
		return false;
	if (pFriend->GetFriendType() == nFriendType)
		return false;
	SQLBUF	szSQL;
	sprintf(szSQL, "UPDATE %s SET friendtype = %u WHERE friendid = %u AND userid = %u LIMIT 1", _TBL_FRIEND, nFriendType,pFriend->GetFriendID(),pFriend->GetUserID());
	if (Database()->ExecuteSQL(szSQL))
	{
		pFriend->SetFriendType(nFriendType);
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool CUser::AddBeFriend(OBJID id,OBJID idUserOwnmeAsFriend,USHORT nFriendType,LPCTSTR pszName )//记录A是某个人的某种类型的朋友
{
	CFriend* pFriend = GetBeFriend(idUserOwnmeAsFriend);//检查重复添加
	if (pFriend)
		return false;

	pFriend = CFriend::CreateNew();
	if(pFriend )//这里Friendid是自己的ID,szFriendName也是自己的NAME
	{
		pFriend->SetRecord(id,idUserOwnmeAsFriend,this->GetID(),  this->GetName(),nFriendType, pszName);
		m_setBeFriend.push_back(pFriend);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CUser::SetFriendShip(OBJID idFriend,int nFriendShip)
{
	for (int i = 0;i<m_setBeFriend.size();i++)
	{
		CFriend* pFriend = m_setBeFriend[i];
		if (pFriend && pFriend->GetUserID() == idFriend)
		{
			return pFriend->SetFriendShip(GetID(),idFriend,Database(),nFriendShip);
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
bool CUser::AddFriend(OBJID idFriend, LPCTSTR szFriendName, USHORT nFriendType ,LPCTSTR szFriendMate)
{
	CFriend* pFriend= GetFriendByName((char*)szFriendName);
	if (pFriend)//B已经是A的好友
	{
		if (nFriendType==ENEMY_GROUP)//将好友移到仇人分组
		{
			unsigned int uoldType = pFriend->GetFriendType();
			if( MoveFriend(pFriend,nFriendType) )//修改好友分组
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_CHANGEGROUP,idFriend,szFriendName,0,0,0,0,uoldType,0,nFriendType,NULL,0))
					SendMsg(&pMsg);
			}
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(szFriendName);
			if(pTarget)
			{
				CFriend* pTargetFriend = pTarget->GetBeFriend(GetID());
				if (pTargetFriend)
					pTargetFriend->SetFriendType(nFriendType);
			}

			DEBUG_TRY
			UpdateMissionMsgByMonsterType(100001);
			DEBUG_CATCH("UpdateMissionMsgByItemType(100001)")

			//return true;
		}
		return false;//goto 移动分组的时候返回false,以免多发送一次添加成功造成出现两个相同好友
	}
	
	pFriend = CFriend::CreateNew();

	if(pFriend && pFriend->CreateRecord(GetID(), idFriend, szFriendName, Database(),nFriendType,0,szFriendMate))
	{
		CUser* pFriendUser = UserManager()->GetUser(idFriend);
		if(pFriendUser)
		{
			if (pFriend->Create(GetID(),pFriendUser,GetName()))
			{
				m_setFriend.push_back(pFriend);
				//ProcessAction(8009);
				
				DEBUG_TRY
				UpdateMissionMsgByMonsterType(100001);
				DEBUG_CATCH("UpdateMissionMsgByItemType(100001)")
					
				return true;
			}
		}
		else
		{
			stTempUser* pFriendTempUser = UserManager()->QueryTempUser(idFriend);
			if(pFriendTempUser)
			{
				if (pFriend->Create(GetID(),pFriendTempUser,GetName()))
				{
					m_setFriend.push_back(pFriend);
					//ProcessAction(8009);
					
					DEBUG_TRY
					UpdateMissionMsgByMonsterType(100001);
					DEBUG_CATCH("UpdateMissionMsgByItemType(100001)")
						
					return true;
				}
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
//独孤求败[2008/12/17]
bool CUser::DelBeFriend(OBJID idUserOwnmeAsFriend)
{
	for (int i = 0;i<m_setBeFriend.size();i++)
	{
		CFriend* pFriend = m_setBeFriend[i];
		if (pFriend && pFriend->GetUserID() == idUserOwnmeAsFriend)
		{
			delete pFriend;
			m_setBeFriend.erase(m_setBeFriend.begin() + i);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelFriend(OBJID idFriend)
{
	for(int i = 0; i < m_setFriend.size(); i++)
	{
		CFriend* pFriend = m_setFriend[i];
		if(pFriend && pFriend->GetFriendID() == idFriend)
		{
			pFriend->DeleteRecord(Database());
			delete pFriend;
			m_setFriend.erase(m_setFriend.begin() + i);


			DEBUG_TRY
			UpdateMissionMsgByMonsterType(100001);
			DEBUG_CATCH("UpdateMissionMsgByItemType(100001)")

			return true;
		}
	}

	return false;
}

bool CUser::CreateAllBeFriend(IDatabase* pDb)
{
	SQLBUF		szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE friendid = %u", _TBL_FRIEND, GetID());
	IRecordset*	pRes = pDb->CreateNewRecordset(szSQL);
	if(pRes)
	{
		OBJID idBeFriend = 0;
		IRecordset*	pRes2 = NULL;
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			idBeFriend = pRes->LoadDWord("userid");
			if(!idBeFriend)
			{
				::LogSave("CUser::CreateAllBeFriend friend id error!sql='%s'", szSQL);
				pRes->MoveNext();
				continue;
			}

			CUser* pUser = UserManager()->GetUser(idBeFriend);
			if(pUser)
			{
				CFriendPtr	pFriend = CFriend::CreateNew();
				if(pFriend)
				{
					if(pFriend->Create(pRes, pUser, GetName()))
						m_setBeFriend.push_back(pFriend);
					else
						delete pFriend;
				}

				pRes->MoveNext();
				continue;
			}

			stTempUser* pTempData = UserManager()->QueryTempUser(idBeFriend);
			if(pTempData)
			{
				CFriendPtr	pFriend = CFriend::CreateNew();
				if(pFriend)
				{
					if(pFriend->Create(pRes, pTempData, GetName()))
						m_setBeFriend.push_back(pFriend);
					else
						delete pFriend;
				}
				
				pRes->MoveNext();
				continue;
			}
			
			pRes->MoveNext();
		}
		pRes->Release();
	}
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUser::CreateAllFriend(IDatabase* pDb)
{
	SQLBUF		szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE userid = %u LIMIT %d", _TBL_FRIEND, GetID(),_MAX_USERFRIENDSIZE);
	IRecordset*	pRes = pDb->CreateNewRecordset(szSQL);
	if(pRes)
	{
		OBJID idFriend = 0;
		IRecordset*	pRes2 = NULL;
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			idFriend = pRes->LoadDWord("friendid");
			if(!idFriend)
			{
				::LogSave("CUser::CreateAllFriend friend id error!sql='%s'", szSQL);
				pRes->MoveNext();
				continue;
			}
			
			CUser* pUser = UserManager()->GetUser(idFriend);
			if(pUser)
			{
				CFriendPtr	pFriend = CFriend::CreateNew();
				if(pFriend)
				{
					if(pFriend->Create(pRes, pUser, GetName()))
						m_setFriend.push_back(pFriend);
					else
						delete pFriend;
				}
				
				pRes->MoveNext();
				continue;
			}
			
			stTempUser* pTempData = UserManager()->QueryTempUser(idFriend);
			if(pTempData)
			{
				CFriendPtr	pFriend = CFriend::CreateNew();
				if(pFriend)
				{
					if(pFriend->Create(pRes, pTempData, GetName()))
						m_setFriend.push_back(pFriend);
					else
						delete pFriend;
				}
				
				pRes->MoveNext();
				continue;
			}
			
			pRes->MoveNext();
		}
		pRes->Release();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CUser::DeleteAllBeFriend()
{
	for(int i = 0; i < m_setBeFriend.size(); i++)
	{
		SAFE_DELETE(m_setBeFriend[i]);
	}
	m_setBeFriend.clear();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void CUser::DeleteAllFriend()
{
	for(int i = 0; i < m_setFriend.size(); i++)
	{
		SAFE_DELETE(m_setFriend[i]);
	}
	m_setFriend.clear();
}

//////////////////////////////////////////////////////////////////////////
//[2010-06-09 goto]删除已看过的用户留言
void CUser::DelFriendLeaveWord(LPCTSTR szName, DWORD dwId)
{
	if (GetSendLeaveWordCount()<=0)
		return;
	char	szSQL[256]="";
	sprintf(szSQL, "DELETE FROM %s WHERE send_name='%s' AND user_name='%s' ORDER BY id LIMIT %d", _TBL_LEAVEWORD, szName, GetName(), GetSendLeaveWordCount()/*PLAYER_WORDS_PER_TIME*/);
	ASSERT(g_db.ExecuteSQL(szSQL));
	SetLeaveWord(false);
	SetSendLeaveWordCount(0);
}
//////////////////////////////////////////////////////////////////////////
//[2010-06-10 goto]删除临时好友
void CUser::DelTempFriend()
{
	OBJID dwUserId = this->GetID();
	char	szSQL[256]="";
	sprintf(szSQL, "DELETE FROM %s WHERE userid=%d AND friendtype=%d", _TBL_FRIEND, dwUserId, TEMP_FRIEND_GROUP );
	ASSERT(g_db.ExecuteSQL(szSQL));
}
//////////////////////////////////////////////////////////////////////////
//[2010-06-10 goto]添加不在线的用户为好友
OBJID CUser::AddLeaveFriend(LPCTSTR szFriendName, unsigned int uFriendType, CMsgFriend* pFriendMsg)
{
	CFriend* pFriend= GetFriendByName((char*)szFriendName);
	if (pFriend)//B已经是A的好友
		return ID_NONE;

	char	szSQL[256];
	sprintf(szSQL, "SELECT id,lookface,Feel,mate FROM %s WHERE name='%s' LIMIT 1", _TBL_USER, szFriendName);
	IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
	if(!pRes)
	{
		return 0;
	}

	IRecord* pRecord = pRes->CreateNewRecord();
	pRes->Release();
	if(!pRecord)
		return 0;

	char szFriendFeel[_MAX_FEELSIZE];
	OBJID dwFriendID = pRecord->LoadDWord( "id" );
	DWORD dwLookface = pRecord->LoadDWord("lookface");
	OBJID dwMateID = pRecord->LoadDWord( "mate" );
	pRecord->LoadString( szFriendFeel, "Feel", _MAX_FEELSIZE );
	pRecord->Release();
	char szMateName[_MAX_NAMESIZE] = "无";
	if (dwMateID!=0)
		sprintf(szMateName, GetNameByID(dwMateID));
	if (AddFriend(dwFriendID, szFriendName, uFriendType,szMateName))
	{
		if( !pFriendMsg->Create(_FRIEND_ADDSUCCESS,dwFriendID,szFriendName,CMsgFriend::OFFLINE_STATUS,dwLookface,0,0,0,0,uFriendType,szFriendFeel,0) )
			return NULL;
	}

	return dwFriendID;
}
//////////////////////////////////////////////////////////////////////////
LPCTSTR	CUser::GetNameByID(OBJID idUser)
{
	stTempUser* pTemp = UserManager()->QueryTempUser(idUser);
	if (pTemp)
	{
		if (!pTemp->name || strlen(pTemp->name)==0)
			return "未知";
		return pTemp->name;
	}
	else
	{
// 		char	szSQL[256];
// 		sprintf(szSQL, "SELECT name FROM %s WHERE id='%u' LIMIT 1", _TBL_USER, idUser);
// 		IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// 		if(!pRes)
// 		{
// 			return "未知";
// 		}
// 		
// 		IRecord* pRecord = pRes->CreateNewRecord();
// 		pRes->Release();
// 		if(!pRecord)
// 			return "未知";
// 		
// 		static char szName[_MAX_NAMESIZE];
// 		pRecord->LoadString( szName, "name", _MAX_NAMESIZE );
// 		if (!szName || strlen(szName)==0)
// 			return "未知";
// 		
// 		pRecord->Release();
// 		return szName;
		LogSave("ERROR:找不到玩家 iduser=%u",idUser);
		return "未知";
	}
}
//////////////////////////////////////////////////////////////////////////
void CUser::SendFriendInfo()
{
	int	nAmount = m_setFriend.size();
	CHECK(nAmount <= _MAX_USERFRIENDSIZE);
	//玩家上线时把所有好友的信息发过来
	for(int i = 0; i < nAmount; i++)
	{
		CFriend* pFriend = m_setFriend[i];
		if(pFriend)
		{
			OBJID	idFriend	= pFriend->GetFriendID();
			CUser*  pFriendUser	= UserManager()->GetUser(idFriend);
			int		bOnline		= 0;
			if(pFriendUser)
				bOnline			= CVipList::GetLineByName(MapGroup(PID)->GetServerName());
			DWORD	dwLookFace	= pFriend->GetFriendLookFace();
			int		nLevel		= pFriend->GetFriendLevel();
			//20061012修罗.CMsgFriend::Create()新增两个参数--------------------------
			unsigned int		profession	= pFriend->GetFriendFirJob();
			//------------------------------------------------------------------------
			int nFriendType		= pFriend->GetFriendType();//独孤求败[2008/12/12]好友类型
            int nSex = pFriendUser ? pFriendUser->GetSex() : 0;
			unsigned int nFriendShip	= pFriend->GetFriendShip();

			CMsgFriend	msg;
			//  [11/3/2007 朱斌]
			if(!msg.Create(_FRIEND_GETINFO, 
				idFriend, 
				pFriend->GetFriendName(), 
				bOnline, 
				dwLookFace, 
				nLevel,
				profession,
				nFriendShip,
				nSex,
				nFriendType,
				pFriend->GetFriendFeel(),//[2010-06-05 goto]增加玩家心情
				0))//关系,师徒、夫妻...
			{
				char szMsg[255];
				if(pFriend)
				{
					sprintf(szMsg, "CUser.ID = %d idFriend = %d friend name = %s ", GetID(), idFriend, pFriend->GetFriendName());
				}
				else
				{
					sprintf(szMsg, "CUser.ID = %d idFriend = %d pFriend = NULL ", GetID(), idFriend);
				}
				
				log_assert("IF_NOT_DEBUG", szMsg, __FILE__, __LINE__);
			}
			if (bOnline)
				SendMsg(&msg);
			else
			{
				//MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, GetSocketID(), GetID());		// 通知其它地图组，下传给我好友在线的消息
				msg.SetTransData(GetID());
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg);
			}
		}
	}
	//同时把自己的信息发给所以加了自己为好友的玩家
	// 通过WORLD核心转发给其它在线玩家
	nAmount = m_setBeFriend.size();
	for( i = 0; i < nAmount; i++)
	{
		CFriend* pFriend = m_setBeFriend[i];
		if(pFriend)//20061020修罗注.对方没上线,理应无需发送.但此处无法得知对方是否在线
		{
			OBJID	idFriend	= pFriend->GetUserID();//获取加了自己为好友的玩家的ID
			CMsgFriend	msg;
			//20061012修罗.新加两个参数.
			IF_OK(msg.Create(_FRIEND_ONLINE, 
				GetID(), 
				GetName(), 
				CVipList::GetLineByName(MapGroup(PID)->GetServerName()),//CMsgFriend::ONLINE_STATUS, 
				GetLookFace(), 
				GetLev(),
				GetProfession(),
				0,
				GetSex(),
				pFriend->GetFriendType(),
				GetFeel(),//[2010-06-05 goto]增加玩家心情
				0))//关系,师徒、夫妻...
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idFriend);
		}
	}

// 	nAmount = m_setFriendContact.size();
// 	for (i = 0;i<nAmount;i++)
// 	{
// 		CFriendContactHistory* pFriend = m_setFriendContact[i];
// 		if(pFriend)
// 		{
// 			OBJID	idFriend	= pFriend->GetFriendID();
// 			CUser* pFriendUser	= UserManager()->GetUser(idFriend);
// 			bool	bOnline		= pFriendUser!=0;
// 			DWORD	dwLookFace	= pFriendUser ? pFriendUser->GetLookFace() : 0;
// 			int		nLevel		= pFriendUser ? pFriendUser->GetLev() : 0;
// 			unsigned int		profession	= pFriendUser ? pFriendUser->GetProfession() : 0;
// 			int nFriendType		= pFriend->GetFriendType();
//             int nSex = pFriendUser ? pFriendUser->GetSex() : 0;
// 			CMsgFriend	msg;
// 			if(msg.Create(_FRIEND_CONTACTHISTORY_GETINFO, 
// 				idFriend, 
// 				pFriend->GetFriendName(), 
// 				bOnline, 
// 				dwLookFace, 
// 				nLevel,
// 				profession,
// 				0,
// 				nSex,
// 				nFriendType))
// 				SendMsg(&msg);
// 		}
// 	}
// 
// 	nAmount = m_setBeFriendContact.size();
// 	for (i = 0;i<nAmount;i++)
// 	{
// 		CFriendContactHistory* pFriend = m_setBeFriendContact[i];
// 		if(pFriend)//20061020修罗注.对方没上线,理应无需发送.但此处无法得知对方是否在线
// 		{
// 			OBJID	idFriend	= pFriend->GetUserID();//获取加了自己为好友的玩家的ID
// 			CMsgFriend	msg;
// 			//20061012修罗.新加两个参数.
// 			IF_OK(msg.Create(_FRIEND_CONTACTHISTORY_ONLINE, 
// 				GetID(), 
// 				GetName(), 
// 				CMsgFriend::ONLINE_STATUS, 
// 				GetLookFace(), 
// 				GetLev(),
// 				GetProfession(),
// 				0,
// 				GetSex(),
// 				pFriend->GetFriendType()))
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idFriend);
// 		}
// 	}


	return;
}

//////////////////////////////////////////////////////////////////////
void CUser::BroadcastFriendsMsg	(CNetMsg* pMsg)
{
	if (!pMsg)
		return;
	
	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while(pUser.Next())
	{
		if(pUser && pUser->CanBeSee()==false)
			continue;

		if((/*pUser->GetPhyle() == this->GetPhyle() || */pUser && pUser->IsGM()) && pUser->GetID() != this->GetID())
		{
			pUser->SendMsg(pMsg);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// mapgroup
//////////////////////////////////////////////////////////////////////
//20070329修罗:新增一个参数
//bool CUser::ChangeMapGroup(PROCESS_ID idProcess, OBJID idMap, int nPosX, int nPosY)					// call - delete this;
bool CUser::ChangeMapGroup(PROCESS_ID idProcess, 
						   OBJID idMap, 
						   int nPosX, 
						   int nPosY,
						   Enum_ChangeMapGroupMode mode/* = ENUM_CNGMAPGROUP_MODE_REQ*/)					// call - delete this;
{
	
	//修罗:用[请求-响应]的方式来增加健壮性
	switch(mode)
	{
	case ENUM_CNGMAPGROUP_MODE_REQ:
		{
			//请求
			MapGroup(PID)->QueryIntraMsg()->ChangeMapGroup(idProcess, 
				GetID(), 
				idMap, 
				nPosX, 
				nPosY,
				ENUM_CNGMAPGROUP_MODE_REQ);
		}
		break;
	case ENUM_CNGMAPGROUP_MODE_PROC:
		{
			int nStep = 0;
			DEBUG_TRY
//			SaveInfo();		// SaveAllInfo
#ifdef	PALED_DEBUG
			SendSysMsg("从地图{%s}[%d][%d]切屏到[%d][%d]...", GetMap()->GetName(), GetMapID(), m_idProcess-3, idMap, idProcess-3);
#endif

//  [3/27/2008 朱斌]	处理离开地图ACTION
// 			ST_SETTLEDACTIONINFO* pInfoTemp = g_settledAction.GetSettledActionInfoByType(EM_SETLDACTN_TYPE_LEAVE_MAP);
// 			if(pInfoTemp && pInfoTemp->actionid)
// 			{
// 				GameAction()->ProcessAction(pInfoTemp->actionid, this);
// 			}

			// transformation
// 			m_data.m_Info.dwMonsterType	= 0;
// 			m_data.m_Info.dwMonsterSecs = m_tTransformation.GetRemain();
// 			if(QueryTransformation() && m_data.m_Info.dwMonsterSecs > 0)
// 			{
// 				m_data.m_Info.dwMonsterType	= QueryTransformation()->GetMonsterType();
// 			}
			
			//  [2007-9-26] 朱斌 BUG0001532
			m_data.m_Info.bIsAllowTeam = m_bIsAllowTeam;
			m_data.m_Info.toMapid = idMap;
			m_data.m_Info.idTeam=ID_NONE;
			if(GetTeam())
				m_data.m_Info.idTeam=GetTeam()->GetID();

			nStep = 1;
			MapGroup(PID)->QueryIntraMsg()->SendSocketUserInfo(idProcess, m_idSocket, GetInfo(), m_nIPAddress);

			nStep = 2;
			if(GetTeam())
			{
				TeamInfoStruct info;
				GetTeam()->GetInfo(&info);
				MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_TEAM, &info, sizeof(info));
				
				ClrTeam(false,false);			//? LeaveMapGroup()时不退出队伍
			}

			// 新手学堂 彭峰. [10/16/2007]
//			MapGroup(PID)->QueryIntraMsg()->SendNewPlayerState(idProcess, GetID(), nNewPlayerState, _MAX_NEWPLAYEREXAMSTATE);

//			MapGroup(PID)->QueryIntraMsg()->SendPresentState(idProcess, GetID(), nPresentNum, _MAX_PRESENT_NUM);


			// pengfeng 师徒 [10/31/2007]
// 			MapGroup(PID)->QueryIntraMsg()->SendTutorState(idProcess, GetID(), m_setStudent,this->GetTutor(),this->GetExpTutor());
			
			nStep = 3;
			//装备
			for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
			{
				CItem* pEquip = GetEquipItemByPos(i);
				if(pEquip)
				{
					ItemInfoStructSend info;
					pEquip->GetInfo(&info);
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_ITEM, &info, sizeof(info));
				}
			}
			
			nStep = 4;
			//背包物品
			m_pPackage->SendAllObjInfo(idProcess);

			nStep = 5;
			//仓库
			m_pStorage->SendAllObjInfo(this,idProcess);

			nStep = 6;
			//筋脉
			MapGroup(PID)->GetUserJinmeiSysMgr()->SendAllObjInfo(this,idProcess);

			nStep = 7;
			//KEY
			m_pUserKey.SendObjInfo(idProcess);

			nStep = 8;
			//AUTOFIGHT
			m_pAutoFightData.SendObjInfo(this,idProcess);

			nStep = 9;
			//配方
			if(m_pUserPeifang)
				m_pUserPeifang->SendAllObjInfo(idProcess);

			nStep = 10;
			//任务
			if(m_pTaskDetail)
				m_pTaskDetail->SendAllObjInfo(this,idProcess);

			nStep = 11;
			//宠物
			if(m_pUserPet)
				m_pUserPet->SendAllObjInfo(idProcess);

			nStep = 12;
			//帮会
			if(QuerySynAttr())
			{
				SynAttrInfoStruct info;
				QuerySynAttr()->GetInfo(&info);
				MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_SYNATTR, &info, sizeof(info));
			}

			nStep = 13;
			//好友
			for(i = 0; i < m_setFriend.size(); i++)
			{
				CFriendPtr pFriend = m_setFriend[i];
				if(pFriend)
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_FRIEND, pFriend->GetInfo(), sizeof(FriendInfoStruct));
			}
			nStep = 14;
			for( i = 0; i < m_setBeFriend.size(); i++)
			{
				CFriendPtr pFriend = m_setBeFriend[i];
				if(pFriend)
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_BEFRIEND, pFriend->GetInfo(), sizeof(FriendInfoStruct));
			}

			nStep = 15;
			//技能
			if (QueryMagic())
			{
				for( i = 0; i < QueryMagic()->QuerySet()->GetAmount(); i++)
				{
					MagicInfoStruct	info = {0};
					if(QueryMagic()->GetInfoByIndex(&info, i))
						MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_MAGIC, &info, sizeof(info));
				}
			}

			nStep = 16;
			//开箱子
			if(m_pTreasure)
			{
				for(i=0; i<m_pTreasure->GetAmount(); i++)
				{
					TreasureInfoStruct info={0};
					if(m_pTreasure->GetInfoByIndex(&info, i))
						MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_TREASURE, &info, sizeof(info));
				}
			}

			nStep = 17;
			//师徒
			if(m_pUserTutor)
			{
				for(i=0; i<m_pUserTutor->GetSetTutorSize(); i++)
				{
					TutorInfoStruct	info = {0};
					if(m_pUserTutor->GetTutorInfoByIndex(&info, i))
						MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_TUTOR, &info, sizeof(info));
				}
				for(i=0; i<m_pUserTutor->GetSetStuSize(); i++)
				{
					TutorInfoStruct	info = {0};
					if(m_pUserTutor->GetStuInfoByIndex(&info, i))
						MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_STUDENT, &info, sizeof(info));
				}
			}

			nStep = 18;
			//祝愿
			if(m_pOnlineGift)
			{
				OnlineGiftInfoStruct info = {0};
				if (m_pOnlineGift->GetOnlinegiftInfo(&info))
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_ONLINEGIFT, &info, sizeof(info));
			}

			nStep = 19;
			//跑商
			if (m_pVerification)
			{
				VerificationInfoStruct info = {0};
				if (m_pVerification->GetVerificationInfo(&info))
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_VERIFICATION, &info, sizeof(info));
			}

			nStep = 20;
			//称号
			if(m_pTitle)
			{
				for(i=0; i<m_pTitle->GetAmount(); i++)
				{
					ST_TITLE_DATA info = {0};
					if(m_pTitle->GetInfoByIndex(&info, i))
						MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_TITLE, &info, sizeof(info));
				}
			}

			nStep = 21;
			//宠物蛋
			if(m_pPetEgg)
			{
				for(i=0; i<m_pPetEgg->GetAmount(); i++)
				{
					EggInfoStruct info = {0};
					if(m_pPetEgg->GetInfoByIndex(&info, i))
						MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_PETEGG, &info, sizeof(info));
				}
			}

			nStep = 22;
			//冻结的双倍INFO_DOUBLE
			MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_DOUBLE, &m_doubletime, sizeof(DWORD));


			nStep = 23;
			//竞技场信息
			if(m_pUserWarGame)
			{
				MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_WARGAME, m_pUserWarGame->GetInfo(), sizeof(UserWarGameInfo));
			}

			nStep = 24;
			//专线信息
			if(m_pSpecilLineInfo)
			{
				STSpecilLine info = {0};
				if (m_pSpecilLineInfo->GetInfo(&info))
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_SPECILLINE, &info, sizeof(info));
			}

			nStep = 25;
//modify code by 林德渊 begin 2011.5.30-
			if(m_pvaslvdata)
			{
				MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_VASLV, m_pvaslvdata->GetInfo(), sizeof(UserVasLvInfo));
			}
//modify code by 林德渊 end

			nStep = 26;
			// status info
			for(i = 0; i < QueryStatusSet()->GetAmount(); i++)
			{
				IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
				if(pStatus)
				{
					StatusInfoStruct	info;
					if(pStatus->GetInfo(&info))	
					{
// 						if(info.nStatus % 1000 > 700  && info.nStatus % 1000 < 900)
// 							continue;

						MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_STATUS, &info, sizeof(info));
					}
				}
			}

			nStep = 27;
			//goto 宠物身上的buff
			SaveEudemonStatus();
			for (int j=0; j<m_setEudemonStatus.size(); j++)
			{
				StatusInfoStruct	info = m_setEudemonStatus[j];
				MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_STATUS_MONSTER, &info, sizeof(info));
			}			


			nStep = 28;
			if(GetAward()%10 < 8)
			{
				AwardInfoStruct info;
				if(GetAward() > 9)
				{
					info.nTime = GetAward() % 10;
					info.nLeave = 1;
				}
				else
				{
					if(m_awardcheck.IsTimeOut())
					{
						info.nTime = GetAward() % 10;
						info.nLeave = 1;
					}
					else
					{
						info.nTime = GetAward() % 10;
						info.nLeave = m_awardcheck.GetRemain() - 1;
					}
				}
				MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_AWARDINFOSEND, &info, sizeof(info));
			}
		

			nStep = 29;
			for( MAP_ITEMDELAY_Iter it = m_setItemDelay.begin();it != m_setItemDelay.end();it++ )
			{
				CTimeOutMS* pTemp = it->second;
				if( pTemp )
				{
					CountTimerStruct info;
					info.nIndexInSet	= it->first;
					info.idUser			= GetID();
					info.tUpdateTime	= pTemp->GetUpdateTime();
					info.nIntervalTime	= pTemp->GetInterval();
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo( idProcess,GetID(),INFO_COUNTTIMER,&info,sizeof( info ) );	
				}
			}
			DEBUG_CATCH2("ENUM_CNGMAPGROUP_MODE_PROC error nStep = %d",nStep)
			
			MapGroup(PID)->QueryIntraMsg()->ChangeMapGroup(idProcess, GetID(), idMap, nPosX, nPosY, ENUM_CNGMAPGROUP_MODE_PROC, this->m_pBooth);
			this->m_pBooth = NULL;
			
			MapGroup(PID)->QueryIntraMsg()->NowUpdateCalculatedProperty(idProcess, GetID());

			UserManager()->LogoutUser(GetSocketID(), false);	// 删除对象，进入另一地图组, false: not offline
			
			
			// stat
			extern struct STAT_STRUCT	g_stat;
			InterlockedIncrement(&g_stat.setDebug[3]);		// debug
		}
		break;
	default:
		{
			//error!
			LOGERROR( "CUser::ChangeMapGroup函数出错,用户:%s idMap=%d x=%d y=%d mode=%d",
				this->GetName(),idMap,nPosX,nPosY,mode );
			return false;
		}
		break;
	}
	
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::LeaveMapGroup()
{
// 	if(m_eudemonData[0].id)
// 	{
// 		KillEudemon(m_eudemonData[0].id,false,true);
// 	}
	// leave map first(加速离开地图，后面会重复发送此消息)
	CMsgAction msg;
	if (msg.Create(GetID(), 0,0,0, actionLeaveMap))
		this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);
	
//	DestroyBooth();

// 	if (m_pTeam)
// 	{
// 		this->AbandonTeamLead();
//  		this->QuitTeam();
// 	}
//	SaveInfo();		

	LeaveMap();
	
//	DeleteAllTutor();
	DeleteAllFriend();
	DeleteAllBeFriend();//独孤求败[2008/12/16]
//	DeleteAllFriendContact();//独孤求败[2008/12/18]
//	DeleteAllBeFriendContact();//独孤求败[2008/12/19]
	DeleteAllItem();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::JoinMap(OBJID idMap, int nPosX, int nPosY)
{
	DEBUG_TRY
	int nprocessid = m_idProcess;

	m_pMap		= MapManager()->GetGameMap(idMap);
	if(m_pMap==NULL)
	{
		ChangeMapGroup(MSGPORT_MAPGROUP_FIRST, 1001, 360, 360);	
		return true;
	}
	m_nPosX		= nPosX;
	m_nPosY		= nPosY;

	// 发送回应消息
	CMsgAction	msg;
	if(msg.Create(GetID(), nPosX, nPosY, GetDir(), actionFlyMap, m_pMap->GetDocID(),m_pMap->GetID(),true))
		SendMsg(&msg);

	//EnterMap();
	//InitEudemon();


	DEBUG_CATCH("JoinMap(OBJID idMap, int nPosX, int nPosY)")
	return true;
}

bool CUser::SendObjInfo(PROCESS_ID idProcess, OBJID idUser, INFO_ID nInfoMsgID, void* pInfo, int nSize)
{
	return MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, idUser, nInfoMsgID, pInfo, nSize);
}

//////////////////////////////////////////////////////////////////////
bool CUser::AppendObjInfo(INFO_ID nType, void* pInfo)
{
	switch(nType)
	{
// 	case INFO_GOMATE:
// 		{
// 			GoMate_Info* pGoMateInfo=(GoMate_Info*)pInfo;
// 			FlyMap(pGoMateInfo->idMap,pGoMateInfo->posX,pGoMateInfo->posY);
// 		}
// 		break;
	case	INFO_ITEM:
		AppendItem((ItemInfoStructSend*)pInfo);
		break;
	case	INFO_JINMEI:
		AppendJinmei((JinmeiInfoStruct*)pInfo);
		break;
	case	INFO_KEY:
		AppendKey((SYSTEMUSERKEY*)pInfo);
		break;
	case	INFO_AUTOFIGHT:
		AppendAutoFight((AUTOF*)pInfo);
		break;
	case	INFO_PEIFANG:
		AppendPeifang((ST_PEIFANGDATA*)pInfo);
		break;
	case	INFO_TASKDETAIL:
		AppendTaskDetial((ST_TASKDETAILDATA*)pInfo);
		break;
	case	INFO_EUDEMON:
		AppendEudemon((EudemonData*)pInfo);
		break;
	case	INFO_FRIEND:
		AppendFriend((FriendInfoStruct*)pInfo);
		break;
	case	INFO_BEFRIEND:
		AppendBeFriend((FriendInfoStruct*)pInfo);
		break;
	case	INFO_MAGIC:
		if (QueryMagic())
			QueryMagic()->AppendMagic((MagicInfoStruct*)pInfo);
		break;
	case	INFO_TREASURE:
		if(m_pTreasure)
			m_pTreasure->AppendTreasure((TreasureInfoStruct*)pInfo);
		break;
	case	INFO_TUTOR:
		if(m_pUserTutor)
			m_pUserTutor->AppendTutor((TutorInfoStruct*)pInfo);
		break;
	case	INFO_STUDENT:
		if(m_pUserTutor)
			m_pUserTutor->AppendStu((TutorInfoStruct*)pInfo);
		break;
	case	INFO_TITLE:
		if(m_pTitle)
			m_pTitle->AppendTitle((ST_TITLE_DATA*)pInfo);
		break;
	case	INFO_PETEGG:
		if(m_pPetEgg)
			m_pPetEgg->AppendPetData((EggInfoStruct*)pInfo);
		break;
	case	INFO_DOUBLE:
		m_doubletime = *(int*)pInfo;
		break;
	case	INFO_SYNATTR:
		if(QuerySynAttr())
			QuerySynAttr()->AppendSynAttr((SynAttrInfoStruct*)pInfo);
		break;
	case	INFO_STATUS:
		AppendStatus((StatusInfoStruct*)pInfo);
		break;
	case	INFO_TEAM:
		AppendTeam((TeamInfoStruct*)pInfo);
		break;
	case	INFO_AWARDINFOSEND:
		AppendAwardInfo((AwardInfoStruct*)pInfo);
		break;
	case INFO_ONLINEGIFT:
		if (m_pOnlineGift)
			m_pOnlineGift->AppendOnlinegiftInfo((OnlineGiftInfoStruct*)pInfo);
		break;
	case INFO_VERIFICATION:
		if (m_pVerification)
			m_pVerification->AppendVerificationInfo((VerificationInfoStruct*)pInfo);
		break;
	//06.11.8王玉波添加-----begin---------
	case	INFO_COUNTTIMER:
		AppendCountTimer( ( CountTimerStruct* )pInfo );
		break;
	case	INFO_STATUS_MONSTER:
		{
			StatusInfoStruct info = {0};
			memcpy(&info, pInfo, sizeof(StatusInfoStruct));
			m_setEudemonStatus.push_back(info);//先存起来,等宠物召出来后才能加
		}
		break;
	case	INFO_WARGAME:
		if(m_pUserWarGame)
			m_pUserWarGame->AppendInfo((UserWarGameInfo*)pInfo);
		break;
	case INFO_SPECILLINE:
		if(m_pSpecilLineInfo)
			m_pSpecilLineInfo->AppendInfo((STSpecilLine*)pInfo);
		break;
//modify code by 林德渊 begin 2011.5.30-
	case INFO_VASLV:
		if(m_pvaslvdata)
			m_pvaslvdata->AppendInfo((UserVasLvInfo*)pInfo);
		break;
//modify code by 林德渊 end
	//----------------------end-----------	
	default:
		ASSERT(!"switch INFO_TYPE");
	}
	return true;
}
//////////////////////////////////////////////////////////////////////
//06.11.8
//王玉波
//函数功能:玩家转换地图组,原地图组线程发过来的物品CD信息,接收并处理
bool CUser::AppendCountTimer( CountTimerStruct* pInfo )
{
	CHECKF( pInfo->idUser == GetID() );
	CTimeOutMS* pTimeOutMS = new CTimeOutMS;
	if( !pTimeOutMS )
		return false;
	pTimeOutMS->SetUpdateTime( pInfo->tUpdateTime );
	pTimeOutMS->SetInterval( pInfo->nIntervalTime );
	this->m_setItemDelay[ pInfo->nIndexInSet ] = pTimeOutMS;
	return true;
}

bool CUser::AppendKey(SYSTEMUSERKEY* pInfo)
{
	CHECKF(pInfo);
	m_pUserKey.SetObjInfo(pInfo);

	return true;
}

bool CUser::AppendAutoFight(AUTOF* pInfo)
{
	CHECKF(pInfo);
	m_pAutoFightData.SetObjInfo(pInfo);

	return true;
}

bool CUser::AppendPeifang	(ST_PEIFANGDATA* pInfo)
{
	CHECKF(pInfo);
	CHECKF(m_pUserPeifang);
	m_pUserPeifang->AddPeifangBySend(*pInfo);

	return true;
}

bool CUser::AppendTaskDetial(ST_TASKDETAILDATA* pInfo)
{
	CHECKF(pInfo);
	CHECKF(m_pTaskDetail);
	m_pTaskDetail->CreateNewTaskDetailData(pInfo,false);

	return true;
}

bool CUser::AppendEudemon	(EudemonData* pInfo)
{
	CHECKF(pInfo);
	CHECKF(m_pUserPet);
	m_pUserPet->AddPeTBySend(*pInfo);

	return true;
}

bool CUser::AppendJinmei	(JinmeiInfoStruct* pInfo)
{
	CUserJinMeiSysMgr* pMgr = MapGroup(PID)->GetUserJinmeiSysMgr();
	CHECKF(pMgr);
	pMgr->AddJinmeiForChange(this,pInfo);

	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUser::AppendItem(ItemInfoStructSend* pInfo)
{
	if(pInfo->idType == 144000)
		return true;

	CHECKF(pInfo->idOwner == GetID());

	CItemPtr pItem = CItem::CreateNew();
	IF_NOT(pItem)
		return false;


	IF_NOT( pItem->CreateSend( Database(), 
		GameDataDefault()->GetGameItemData(), 
		pInfo,
		GameDataDefault()->GetWeaponSoulData(),
		false,
		ID_NONE ) )
	{
		pItem->ReleaseByOwner();
		return false;
	}

	if(pInfo->ucPosition >= ITEMPOSITION_EQUIPBEGIN && pInfo->ucPosition < ITEMPOSITION_EQUIPEND)
	{
		CItemPtr& refpEquip = this->GetEquipItemRef(pInfo->ucPosition);
		IF_OK(!refpEquip)
		{
			refpEquip = pItem;
			pItem = NULL;		// 不放入背包
		}
	}
	else if(pInfo->ucPosition == 201)
	{
		m_pStorage->AddItem(pItem);
	}
	else
		m_pPackage->AddItemByChangeMap(pItem);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::AppendFriend(FriendInfoStruct* pInfo)
{
	CHECKF(pInfo->idUser == GetID());

	CFriendPtr pFriend = NULL;
	pFriend = CFriend::CreateNewFriend(pInfo);
	if(!pFriend)
		return false;

	m_setFriend.push_back(pFriend);

	return true;
}

bool CUser::AppendBeFriend(FriendInfoStruct* pInfo)
{
	CHECKF(pInfo->idFriend == GetID());
	
	CFriendPtr pFriend = NULL;
	pFriend = CFriend::CreateNewFriend(pInfo);
	if(!pFriend)
		return false;
	
	m_setBeFriend.push_back(pFriend);
	
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUser::AppendStatus(StatusInfoStruct* pInfo)
{
// 	if(pInfo->nStatus % 1000 > 700  && pInfo->nStatus % 1000 < 900)
// 		return true;
	if(pInfo->nTimes > 0)		// more status
	{
		CStatusMore* pStatus = CStatusMore::CreateNew();
		if(pStatus)
		{
			if( pStatus->Create( QueryRole(), pInfo->nStatus, pInfo->nPower, pInfo->nSecs, pInfo->nTimes, pInfo->nParam1, pInfo->nParam2,pInfo->nParam3 ) )
			{
				QueryStatusSet()->AddObj(pStatus->QueryInterface());
				//07.5.12王玉波添加以下一句,对于客户端而言，并不是添加，所以此时用updatebuff
//				UpdateBuff( pInfo->nStatus,pInfo->nSecs,pInfo->nTimes,pInfo->nPower,pInfo->nParam1,pInfo->nParam2,pInfo->nParam3 );
			}
			else
				pStatus->ReleaseByOwner();
		}
	}
	else
	{
		CStatusOnce* pStatus = CStatusOnce::CreateNew();
		if(pStatus)
		{
			if( pStatus->Create( QueryRole(), pInfo->nStatus, pInfo->nPower, pInfo->nSecs, pInfo->nParam1, pInfo->nParam2,pInfo->nParam3 ) )
			{
				QueryStatusSet()->AddObj(pStatus->QueryInterface());
				//07.5.12王玉波添加以下一句
//				UpdateBuff( pInfo->nStatus,pInfo->nSecs,pInfo->nTimes,pInfo->nPower,pInfo->nParam1,pInfo->nParam2,pInfo->nParam3 );
				
			}
			else
				pStatus->ReleaseByOwner();
		}
	}
//07.5.12王玉波注释以上
//以下代码，功能不能实现
//	if( !pInfo )
//	{
//		return false;
//	}
//	return CRole::AttachStatus( this->QueryRole(),pInfo->nStatus,pInfo->nPower,pInfo->nSecs,pInfo->nTimes,pInfo->nParam1,pInfo->nParam2,pInfo->nParam3 );
	return true;
}

bool CUser::SpendCityWarItem(int nAmount)
{
	if(nAmount <= 0)
		return false;

	ItemInfoStruct info;
	if(!(ItemType()->GetInfo(CITYWAR_ITEM_TYPEID, &info)))
	{
		return 0;
	}
	if(nAmount > info.nAmountLimit)
	{
		this->SendSysMsg(g_objStrRes.GetStr(28315));     //输入城战物品数量超过最大数量
		return false;
	}

	int sourceItemSize = _MAX_USERITEMSIZE*4;
	OBJID sourceItemID[_MAX_USERITEMSIZE*4];

	int sourceItemAmount = this->GetItemByTypeMulti(CITYWAR_ITEM_TYPEID, sourceItemSize, sourceItemID);
	if(nAmount > sourceItemAmount)
	{
		this->SendSysMsg(g_objStrRes.GetStr(28322));     //您身上的城战宝箱数量不够
		return false;
	}
	
	return DelItemByTypeMulti(sourceItemID, sourceItemSize, nAmount);
}

//////////////////////////////////////////////////////////////////////
// money
//////////////////////////////////////////////////////////////////////
bool CUser::SpendMoney(DWORD dwMoney, bool bSynchro)
{
	if(dwMoney == 0)
		return true;

	int nMoney1 = 0;
	int nMoney2 = 0;
	if (this->GetMoneyByType(2) < dwMoney)
	{
		nMoney2 = this->GetMoneyByType(2);
		if(this->GetMoneyByType(1) < dwMoney - nMoney2)
			return false;
		else
			nMoney1 = dwMoney - nMoney2;

	}
	else
	{
		nMoney2 = dwMoney;
	}

	DWORD dwNewMoney2 = this->GetMoneyByType(2) - nMoney2;
	DWORD dwNewMoney1 = this->GetMoneyByType(1) - nMoney1;
//	g_EconomySystem.ChangeMoneyCount(dwMoney * -1,this->GetProfession());	//李京哲0321 增加种族收入统计

	if(nMoney2 > 0)
		m_data.SetMoney(dwNewMoney2,2);
	if(nMoney1 > 0)
	{
		m_data.SetMoney(dwNewMoney1,1);
		//goto 记录银两增量值
		m_MoneyIncr -= nMoney1;
	}

	// update to client...
	if(bSynchro)
	{
		CMsgUserAttrib	msg;
		msg.Create(this->GetID(),_USERATTRIB_NONE,0);
		if(nMoney2 > 0)
			msg.Append( _USERATTRIB_MONEY2, dwNewMoney2);
		if(nMoney1 > 0)
			msg.Append( _USERATTRIB_MONEY, dwNewMoney1);
		SendMsg(&msg);
	}

	return true;
}
bool CUser::SpendMoneyByType(DWORD dwMoney, int nType, bool bSynchro)
{
	if(dwMoney == 0)
		return true;
	
	if (this->GetMoneyByType(nType) < dwMoney)
		return false;
	
	DWORD dwNewMoney = this->GetMoneyByType(nType) - dwMoney;
	//g_EconomySystem.ChangeMoneyCount(dwMoney * -1,this->GetProfession());	//李京哲0321 增加种族收入统计
	SetMoneyByType(dwNewMoney, nType, true);
	
	// update to client...
	int nAtt = 0;
	if (nType==1) nAtt = _USERATTRIB_MONEY;
	if (nType==2) nAtt = _USERATTRIB_MONEY2;
	if (nType==3) nAtt = _USERATTRIB_MONEY3;
	if (nType==4) nAtt = _USERATTRIB_MONEY4;
	if (nType==5) nAtt = _USERATTRIB_MONEY5;

	//goto 记录银两增量值
	if (nType==1)
		m_MoneyIncr -= dwMoney;

	if(bSynchro)
	{
		CMsgUserAttrib	msg;
		if(msg.Create(this->GetID(), nAtt, dwNewMoney))
			SendMsg(&msg);
	}
	
	return true;
}

void CUser::GainVasCPST(DWORD dwMoney)
{
	CMsgVAS msg;
	msg.Create(dwMoney, GetAccountID(), GetID(), _MSGVAS_ADD_VAS_CPST);
	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
}

void CUser::GainVas(DWORD dwMoney)
{
	CMsgVAS msg;
	msg.Create(dwMoney, GetAccountID(), GetID(), _MSGVAS_ADD_VAS);
 	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
}

void CUser::SpendVas(DWORD dwMoney)
{
	CMsgVAS msg;
	msg.Create(dwMoney, GetAccountID(), GetID(), _MSGVAS_SPEND_VAS);
	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
}

bool CUser::GainMoney(DWORD dwMoney, bool bSynchro /*= true*/,int nType/* =1*/,bool bUpdate/*=false*/)
{
	if(dwMoney == 0)
		return true;

	if(dwMoney < 0)
	{
		LOGWARNING("GainMoney: dwMoney < 0");
	}

	DWORD dwNewMoney = this->GetMoneyByType(nType) + dwMoney;
	if (dwNewMoney > _MAX_MONEYLIMIT)
	{
		this->SendSysMsg("钱已超过数量");
		return false;
	}
//	g_EconomySystem.ChangeMoneyCount(dwMoney,this->GetProfession());	//李京哲0321 增加种族收入统计
	m_data.SetMoney(dwNewMoney,nType,bUpdate);//goto 改为实时写数据库，避免当机时损失

	// update to client...
	if(bSynchro)
	{
		CMsgUserAttrib	msg;
		if(msg.Create(this->GetID(), _USERATTRIB_MONEY + nType - 1, dwNewMoney))
			SendMsg(&msg);

		if(nType == 1)
		{
			if (isFiftyP())
			{
				dwMoney = dwMoney/2;
			}
			else if(isZeroP())
			{
				dwMoney = 0;
			}
			m_MoneyIncr += dwMoney;
			SendSysMsg("您获得了%u银两",dwMoney);
		}
		else if(nType == 2)
		{
			if (isFiftyP())
			{
				dwMoney = dwMoney/2;
			}
			else if(isZeroP())
			{
				dwMoney = 0;
			}
			SendSysMsg("您获得了%u碎银",dwMoney);
		}
		else if(nType == 4)
		{
			SendSysMsg("您获得了%u珠宝",dwMoney);
		}
	}

	return true;
}

bool CUser::SaveMoney(DWORD dwMoney, bool bSynchro /*= true*/)
{
	if (this->GetMoneyByType(1) < dwMoney)
	{
		//this->SendSysMsg(g_objStrRes.GetStr( 11086) );//STR_NOT_SO_MUCH_MONEY
		this->SendSysMsg(_TXTATR_TOPSHOW, "包裹中没有那么多银两");
		return false;
	}

	DWORD dwNewMoney = this->GetMoneyByType(1) - dwMoney;
	DWORD dwNewMoneySaved = this->GetMoneySaved() + dwMoney;
	if (dwNewMoneySaved > _MAX_MONEYLIMIT)
	{
		//this->SendSysMsg(g_objStrRes.GetStr( 14042) );//STR_TOOMUCH_MONEY_SAVED
		this->SendSysMsg(_TXTATR_TOPSHOW,"仓库银两已达上限");
		return false;
	}

	SpendMoneyByType(dwMoney, 1, SYNCHRO_FALSE);
	m_data.SetMoneySaved(dwNewMoneySaved);
	m_MoneyIncr -= dwMoney;

	// update to client...
	if(bSynchro)
	{
		CMsgUserAttrib	msg;
		if(msg.Create(this->GetID(), _USERATTRIB_MONEY, dwNewMoney))
		{
			msg.Append(_USERATTRIB_MONEYSAVED, this->GetMoneySaved());
			SendMsg(&msg);
		}
	}

	return true;
}

bool CUser::DrawMoney(DWORD dwMoney, bool bSynchro )
{
	if (this->GetMoneySaved() < dwMoney)
	{
		//SendSysMsg(g_objStrRes.GetStr( 11086) );//STR_NOT_SO_MUCH_MONEY
		this->SendSysMsg(_TXTATR_TOPSHOW, "仓库中没有那么多银两");
		return false;
	}

	DWORD dwNewMoney = this->GetMoneyByType(1) + dwMoney;
	DWORD dwNewMoneySaved = this->GetMoneySaved() - dwMoney;
	if (dwNewMoney > _MAX_MONEYLIMIT)
	{
		//SendSysMsg(g_objStrRes.GetStr( 14044));//STR_TOOMUCH_MONEY_TO_DRAW
		SendSysMsg(_TXTATR_TOPSHOW,"随身携带银两已达上限");
		return false;
	}

	GainMoney(dwMoney, SYNCHRO_FALSE, 1);
	if (dwMoney>30000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::DrawMoney user:%u(%s)获得银两%d",GetID(),GetName(),dwMoney);
	m_data.SetMoneySaved(dwNewMoneySaved);
	m_MoneyIncr += dwMoney;

	// update to client...
	if(bSynchro)
	{
		CMsgUserAttrib	msg;
		if(msg.Create(this->GetID(), _USERATTRIB_MONEY, dwNewMoney))
		{
			msg.Append(_USERATTRIB_MONEYSAVED, this->GetMoneySaved());
			SendMsg(&msg);
		}
	}

	return true;
}


void CUser::SendShow(IRole* pRole)
{
	CMsgPlayer	msg;
	if(msg.Create((IRole*)this,false))
		pRole->SendMsg(&msg);

	if(pRole->IsMonster())
		return;

	CItem* pEquip = GetEquipItemByPos(ITEMPOSITION_WEAPONSOUL);
	if(pEquip && pEquip->IsValidEquip(this))
	{
		ST_WEAPONSOULDATA stWpSl;
		if(pEquip->GetWpSlInfo(stWpSl))
		{
			CMsgWeapensoulShow msgSoul;
			msgSoul.Create(stWpSl,EM_MSGWPSLSHOW_IN,GetID(),pEquip->GetID());
			pRole->SendMsg(&msgSoul);
		}
	}

	for(int i =  this->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus =  this->QueryStatusSet()->GetObjByIndex(i);
		if( pStatus)
		{
			CMsgBuff msgBuff;
			if(msgBuff.Create( ACTION_ADDSTATUS,this->GetID(),pStatus->GetID(),pStatus->GetSecLeft(),0,0,0,0,0 ))
				pRole->SendMsg(&msgBuff);
		}
	}

	if (m_pUserKey.bInit)
	{
		CMsgUserAttrib msgAtt;
		msgAtt.Create(GetID(), _USERATTRIB_FEEL_SWITH, GetFeelSwitch());
		pRole->SendMsg(&msgAtt);
	}
	//goto 称号
	CMsgUserAttrib msgAttTitle;
	msgAttTitle.Create(GetID(), _USERATTRIB_USETITLE, GetUseTitle());
	pRole->SendMsg(&msgAttTitle);

	//竞技场阵营
	if(GetMap() && GetMap()->IsPkGameMap())
	{
		IStatus* pStatus = QueryStatus(STATUS_WARGAME);
		if(pStatus)
		{
			CMsgUserAttrib msg;
			if(msg.Create(GetID(), _USERATTRIB_WARGAME_CAMP, pStatus->GetParam1()))
				pRole->SendMsg(&msg);
		}
	}
	//附体宠物的特效
	if(m_bSoulEudemonView)
	{
		CMsgUserAttrib msgAtt;
		if(msgAtt.Create(GetID(), _USERATTRIB_EUDEMON_VIEW, m_bSoulEudemonView))
			pRole->SendMsg(&msgAtt);
	}
}

bool CUser::CanUseItem(CItem* pItem)///判断玩家是否能使用某个道具
{
	return true;
//	InitScript();///初始化脚本环境
//	InitItemScript(pItem);
////	LuaFunction<bool> CanUse = 	LuaScriptMachine()->GetScriptFun("instance\\Instance_Item_01.lua","CanUse");
//// LuaScriptMachine()->GetState()->GetGlobal("CanUse");
////	bool hr=CanUse();
////	return true;
//	return LuaScriptMachine()->RunScriptFun("instance\\Instance_Item_01.lua","CanUse");
}

/////////////////////////////////////////////////////////////////////////////
//06.3.3王玉波修改
//函数功能：设置玩家的状态
void CUser::SetStatus( int nStatus,bool bSynchro , int nPower)
{
	I64 i64OldEffect = GetEffect();
	I64 i64OldEffect2 = GetSecondEffect();

	switch (nStatus)
	{
	case STATUS_NORMAL:           //change huang 2004.1.11
		{
			m_data.m_Info.dwStatus	= nStatus;
			m_data.m_Info.i64Effect	= KEEPEFFECT_NORMAL;
			m_data.m_Info.i64Effect2=KEEPEFFECT_NORMAL;
			//20070228修罗
			//暂时做最保守处理
//----------07.4.10王玉波注释，前面一句是赋值
//			m_data.m_Info.i64Effect &= ~KEEPEFFECT_RED;
			//先清除
			if(IsRedName())
			{
				m_data.m_Info.i64Effect |= KEEPEFFECT_RED;
			}
			else if(IsGrayName())
			{
				//在别处处理.
				//case	STATUS_CRIME: 
			}
			else if(IsWhiteName())
			{
				//在别处处理
				//CUser::ClsStatus...case	STATUS_PKVALUE:
			}
			else
			{
				//warning
				LOGERROR( "CUser::SetStatus函数出错,用户:%s PK:%d",this->GetName(),this->GetPk() );
			}
			

//			if (m_data.GetPk() <= PKVALUE_BLUE)
//				m_data.m_Info.i64Effect	|= KEEPEFFECT_BLUE;
//			else if (m_data.GetPk() < PKVALUE_GREEN)
//				m_data.m_Info.i64Effect	|= KEEPEFFECT_GREEN;
//			else if(m_data.GetPk() < PKVALUE_YELLOW && m_data.GetPk() > 0)
//				m_data.m_Info.i64Effect |= KEEPEFFECT_YELLOW;
//			else if(m_data.GetPk() < PKVALUE_REDNAME && m_data.GetPk() > 0)
//				m_data.m_Info.i64Effect |= KEEPEFFECT_RED;
//			else if(m_data.GetPk() >= PKVALUE_BLACKNAME && m_data.GetPk() > 0)
//				m_data.m_Info.i64Effect |= KEEPEFFECT_BLACK;

			CTeam* pTeam = GetTeam();
			if(pTeam && pTeam->GetLeader() == GetID())
				m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAMLEADER;
		}
		break;
	case STATUS_DIE:
		{
			m_data.m_Info.dwStatus	= nStatus;
			///有错,彭正帮屏蔽该句2007-07-13
//			m_data.m_Info.i64Effect	&= KEEPEFFECT_NOT_CLEAR;//死亡不清除的状态，原来有就有，原来无就无，用&
			m_data.m_Info.i64Effect	|= KEEPEFFECT_DIE;

//			DebuffCityWarWeapon();
			
			ClearManyStatusWhenDie();//-------彭峰---2007/05/16-----
			
		}
		break;
	case STATUS_TEAMLEADER:
		{
		     m_data.m_Info.i64Effect	|= KEEPEFFECT_TEAMLEADER;
		}
		break;
	case STATUS_LIANGONG:       
		{
			m_data.m_Info.i64Effect2	|= SECONDKEEPEFFECT_LIANGONG;
		}
		break;
	case STATUS_ACTIONEND:       
		{
			m_data.m_Info.i64Effect2	|= SECONDKEEPEFFECT_ACTIONEND;
		}
		break;		
	case STATUS_CRIME:       
		{
			m_data.m_Info.i64Effect	|= KEEPEFFECT_CRIME;
		}
		break;
	case STATUS_ATTRACTMONSTER://吸引怪物
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATTRACTMONSTER;
		}
		break;
	case	STATUS_PKVALUE:         //change huang 2004.1.11
		{
			//20070228修罗
			//清除所有pk状态
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_RED;
			
			if(IsRedName())
			{
				if(QueryStatus(STATUS_CRIME))//如果有灰名
				{
					//清除灰名
					DetachStatus(this->QueryRole(),STATUS_CRIME);
					ClsBuff(STATUS_CRIME);
				}
				if (m_data.GetPk() < PKVALUE_ORANGE && m_data.GetPk() > 0)
					m_data.m_Info.i64Effect	|= KEEPEFFECT_ORANGE;
				else if(m_data.GetPk() < PKVALUE_REDNAME && m_data.GetPk() > 0)
					m_data.m_Info.i64Effect |= KEEPEFFECT_RED;
				else if(m_data.GetPk() < PKVALUE_BROWN && m_data.GetPk() > 0)
					m_data.m_Info.i64Effect |= KEEPEFFECT_BROWN;
				else if(m_data.GetPk() >= PKVALUE_BLACKNAME && m_data.GetPk() > 0)
					m_data.m_Info.i64Effect |= KEEPEFFECT_BLACK;
			}
			else if(IsGrayName())
			{
				//在别处处理.
				//case	STATUS_CRIME: 
			}
			else if(IsWhiteName())
			{
				//在别处处理
				//CUser::ClsStatus...case	STATUS_PKVALUE:
			}
			else
			{
				//warning
				LOGERROR( "CUser::SetStatus函数出错,用户:%s PK:%d",this->GetName(),this->GetPk() );
			}

//			if (m_data.GetPk() <= PKVALUE_BLUE)
//				m_data.m_Info.i64Effect	|= KEEPEFFECT_BLUE;
//			else if (m_data.GetPk() < PKVALUE_GREEN)
//				m_data.m_Info.i64Effect	|= KEEPEFFECT_GREEN;
//			else if(m_data.GetPk() < PKVALUE_YELLOW && m_data.GetPk() > 0)
//				m_data.m_Info.i64Effect |= KEEPEFFECT_YELLOW;
//			else if(m_data.GetPk() < PKVALUE_REDNAME)
//				m_data.m_Info.i64Effect |= KEEPEFFECT_RED;
//			else if(m_data.GetPk() >= PKVALUE_BLACKNAME)
//				m_data.m_Info.i64Effect |= KEEPEFFECT_BLACK;

		}
		break;
	case STATUS_PK_PROTECT:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PK_PROTECT;
		}
		break;
	case STATUS_ENEMYFIRE:///仇视特效-softworms-2007-08-13
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_ENIMYFIRE;///暂时用资料战的特效
		}
		break;
// 	case STATUS_SKZL:		//获得时空之泪，减速
// 		{
// 			m_data.m_Info.i64Effect |= KEEPEFFECT_SKZL;
// 		}
// 		break;
// 	
//-------------------------------------------------------------------------------
	case STATUS_FRENZY:							//狂暴
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_FRENZY;
		}
		break;
	case STATUS_SHIELD:							//一定时间内可抵消N点消害	护盾
	case STATUS_SHIELD_1:						//一定时间内可抵消N点消害	护盾
	case STATUS_SHIELD_2:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_SHIELD;
		}
		break;
	case STATUS_FIRMNESS_MAGIC:					//一定时间内增加技能不被打断的机率	坚定施法
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_FIRMNESS_MAGIC;
		}
		break;
	case STATUS_HIDDEN:							//隐身
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_HIDDEN;
		}
		break;
	case STATUS_RETORT:							//反击
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_RETORT;
		}
		break;
	case STATUS_LURK:							//NPC看不到自己，对玩家无效	潜行
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_LURK;
		}
		break;
	case STATUS_IMMUNE_ALL:						//免疫一切伤害	无敌
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_IMMUNE_ALL;
		}
		break;
	case STATUS_SLEEP:							//不能移动、攻击、技能、使用物品	战斗不能（被攻击可解除）
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_SLEEP;
		}
		break;
	case STATUS_DREAMING:						//不能移动、攻击、技能、使用物品	战斗不能（被攻击不解除）
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DREAMING;
		}
		break;
	case STATUS_PALSY:							//不能移动	定身，麻痹	
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PALSY;
		}
		break;
	case STATUS_SILENCE:						//不能使用技能	沉默
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_SILENCE;
		}
		break;
	case STATUS_CONFUSION:						//鼠标乱转，丢失目标	混乱
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_CONFUSION;
		}
		break;
	case STATUS_STOPSAY:						//不能说话，非控制系	禁言	
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_STOPSAY;
		}
		break;
	case STATUS_PHYSICALATTACKUP_PERCENT:				//物理攻击力上升
	case STATUS_PHYSICALATTACKUP_PERCENT_WS://2007827修罗:
	case STATUS_PHYATTACKUP_TUTOR_PERCENT://  [陈剑飞10/26/2007]
	case STATUS_PHYSICALATTUP_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALATTACKUP;
		}
		break;
	case STATUS_PHYSICALATTACKDOWN_PERCENT:				//物理攻击力下降
	case STATUS_PHYSICALATTDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALATTACKDOWN;
		}
		break;
	case STATUS_MAGICATTACKUP_PERCENT:					//魔法攻击力上升
	case STATUS_MAGICATTACKUP_PERCENT_ED:
	case STATUS_MAGATTACKUP_TUTOR_PERCENT://  [陈剑飞10/27/2007]
//	case STATUS_COMB_MAIN:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICATTACKUP;
		}
		break;
	case STATUS_MAGICATTACKDOWN_PERCENT:				//魔法攻击力下降
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICATTACKDOWN;
		}
		break;
	case STATUS_ATTACKUP_PERCENT:						//攻击力上升
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATTACKUP;
		}
		break;
	case STATUS_ATTACKDOWN_PERCENT:						//攻击力下降
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATTACKDOWN;
		}
		break;
	case STATUS_PHYSICALDEFENCEUP_PERCENT:				//物理防御上升
	case STATUS_PHYSICALDEFENCEUP_NUMBER:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_SF://2007827修罗:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_L:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALDEFENCEUP;
		}
		break;
	case STATUS_PHYSICALDEFENCEDOWN_PERCENT:			//物理防御下降
	case STATUS_PHYSICALDEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALDEFENCEDOWN;
		}
		break;
	case STATUS_MAGICDEFENCEUP_PERCENT:					//魔法防御上升
	case STATUS_MAGICDEFENCEUP_NUMBER:
	case STATUS_MAGICDEFENCEUP_NUMBER_EM://2007827修罗:
	case STATUS_MAGICDEFENCEUP_NUMBER_L:
	case STATUS_MAGICDEFENCEUP_NUMBER_1:
		{
			m_data.m_Info.i64Effect |= 	KEEPEFFECT_MAGICDEFENCEUP;
		}
		break;
	case STATUS_MAGICDEFENCEDOWN_PERCENT:				//魔法防御下降
	case STATUS_MAGICDEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICDEFENCEDOWN;
		}
		break;
	case STATUS_DEFENCEUP_PERCENT:						//防御力上升
	case STATUS_DEFENCEUP_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DEFENCEUP;
		}
		break;
	case STATUS_DEFENCEDOWN_PERCENT:					//防御力下降
	case STATUS_DEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DEFENCEDOWN;
		}
		break;
	case STATUS_HITRATEUP_PERCENT:						//命中率上升
	case STATUS_HITRATEUP_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_HITRATEUP;
		}
		break;
	case STATUS_HITRATEDOWN_PERCENT:					//命中率下降
	case STATUS_HITRATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_HITRATEDOWN;
		}
		break;
	case STATUS_DODGERATEUP_PERCENT:					//躲避率上升
	case STATUS_DODGERATEUP_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DODGERATEUP;
		}
		break;
	case STATUS_DODGERATEDOWN_PERCENT:					//躲避率下降
	case STATUS_DODGERATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DODGERATEDOWN;
		}
		break;
	case STATUS_CRITRATEUP_PERCENT:						//增加致命一击机率
	case STATUS_CRITRATEUP_NUMBER:
	case STATUS_CRITRATEUP_NUMBER_SW://2007827修罗:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_CRITRATEUP;
		}
		break;
	case STATUS_CRITRATEDOWN_PERCENT:					//减少致命一击机率
	case STATUS_CRITRATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_CRITRATEDOWN;
		}
		break;
	case STATUS_MAXLIFEUP_PERCENT:						//最大生命值增加
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXLIFEUP;
		}
		break;
	case STATUS_MAXLIFEDOWN_PERCENT:					//最大生命值减少
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXLIFEDOWN;
		}
		break;
	case STATUS_MAXMANAUP_PERCENT:						//最大魔法值增加
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXMANAUP;
		}
		break;
	case STATUS_MAXMANADOWN_PERCENT:					//最大魔法值减少
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXMANADOWN;
		}
		break;
	case STATUS_MAXSPUP_PERCENT:						//最大体力值增加
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXSPUP;
		}
		break;
	case STATUS_MAXSPDOWN_PERCENT:						//最在体力值减少
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXSPDOWN;
		}
		break;
	case STATUS_MOVESPEEDUP_PERCENT:					//移动速度增加
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOUNTSPEED_NUMBER:					//移动速度增加  坐骑
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOVESPEEDDOWN_PERCENT:					//移动速度减少
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MOVESPEEDDOWN;
		}
		break;
	case STATUS_ATKSPEEDUP_PERCENT:						//攻击速度增加
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATKSPEEDUP;
		}
		break;
	case STATUS_ATKSPEEDDOWN_PERCENT:					//攻击速度减少
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATKSPEEDDOWN;
		}
		break;
	case STATUS_PHYSICALDAMAGEUP_PERCENT:				//物理伤害增大
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALDAMAGEUP;
		}
		break;
	case STATUS_PHYSICALDAMAGEDOWN_PERCENT:				//物理伤害减小
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALDAMAGEDOWN; 
		}
		break;
	case STATUS_MAGICDAMAGEUP_PERCENT:					//魔法伤害增大
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICDAMAGEUP;
		}
		break;
	case STATUS_MAGICDAMAGEDOWN_PERCENT:				//魔法伤害减小
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICDAMAGEDOWN;
		}
		break;
	case STATUS_DAMAGEUP_PERCENT:						//伤害增大
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DAMAGEUP;
		}
		break;
	case STATUS_DAMAGEDOWN_PERCENT:						//伤害减小
	case STATUS_DAMAGEDOWN_PERCENT_TN://2007827修罗:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DAMAGEDOWN;

		}
		break;
	case STATUS_GETEXPUP_PERCENT:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_GETEXPUP;
		}
		break;
	case STATUS_GETEXPUP_PERCENT_SYNINS:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_GETEXPUP;
		}
		break;
	case STATUS_GETEXPUP_PERCENT_WT:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_EUDEMONEXPUP;
		}
		break;
	case STATUS_GETEXPDOWN_PERCENT:						//经验获得减少
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_GETEXPDOWN;
		}
		break;
// 	case STATUS_ERUPTPOT_PERCENT:						//潜能爆发
// 		{
// 			m_data.m_Info.i64Effect |= KEEPEFFECT_ERUPTPOT;
// 		}
// 		break;
// 	case STATUS_FROSTSHIELD:								//寒霜甲
// 		{
// 			m_data.m_Info.i64Effect |= KEEPEFFECT_FROSTSHIELD;
// 		}
// 		break;
//------------dot begin-------------------------------------
 	case STATUS_DOT_POISONING:				//中毒DOT			,掉hp
// 	case STATUS_DOT_POISON_ATK:            // 毒袭 [12/13/2007 陈剑飞]
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DOT_POISONING;
		}
		break;
// 	case STATUS_DOT_ENMITY:	
	case STATUS_DOT_LIFEMISSING:			//流血DOT			,掉hp
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DOT_LIFEMISSING;
		}
		break;
// 	case STATUS_DOT_MANAMISSING:			//精神流失DOT		,掉mp
// 		{
// 			m_data.m_Info.i64Effect |=	KEEPEFFECT_DOT_MANAMISSING;
// 		}
// 		break;
// 	case STATUS_DOT_STRENGTHMISSING:		//体力流失DOT		,掉sp
// 		{
// 			m_data.m_Info.i64Effect |= KEEPEFFECT_DOT_STRENGTHMISSING;
// 		}
// 		break;
 	case STATUS_DOT_BURNING:				//灼伤DOT			,掉hp
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DOT_BURNING;
		}
		break;
//-----------dot end------------------------------------
//SECOND STATUS BEGIN---------------------------------------------------
	case STATUS_CRAZE://狂热
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_CRAZE;
		}
		break;
	case STATUS_PVPMODE://决斗buff
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_PVP;
		}
		break;
	case STATUS_YABIAO:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_YABIAO;
		}
		break;
	case STATUS_JIEBIAO:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_JIEBIAO;
		}
		break;
	case STATUS_DOT_SUFFERING://缠绕DOT
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_SUFFERING;
		}
		break;
// 	case STATUS_DOT_BEATING://打击DOT
// 	case STATUS_DOT_BEATINGPRE:// 致命打击2 [12/12/2007 陈剑飞]
// 		{
// 			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_BEATING;
// 		}
// 		break;
	case STATUS_DETECTION://识破
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_DETECTION;

		}
		break;
	case STATUS_RESOURCEWAR_PROTECT://资源战保护状态
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_RESOURCEWAR_PROTECT;

		}
		break;
 	case STATUS_DOT_ADDLIFE://加血dot
// 	case STATUS_DOT_ADDLIFE_RB://2007827修罗:
// 	case STATUS_DOT_QUICK_ADDLIFE:// 快速恢复 [12/25/2007 陈剑飞]
// 	case STATUS_DOT_ADDLIFE_L:
// 		{
// 			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_DOT_ADDLIFE;
// 		}
// 		break;
	case STATUS_DOT_ADDMANA://加蓝DOT
// 	case STATUS_DOT_ADDMANA_MC://2007827修罗:
// 	case STATUS_DOT_ADDMANA_L:
// 		{
// 			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_DOT_ADDMANA;
// 
// 		}
// 		break;
		//  [1/30/2008 陈剑飞]
	case STATUS_LIFE_UP:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_LIFEUP;
		}
		break;
	case STATUS_COMB_MAIN:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_COMB_MAIN;
		}
		break;
	case STATUS_COMB_SECONDARY:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_COMB_SECONDARY;
		}
		break;
// 	case STATUS_BUFF_HPMP:
// 		{
// 			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_DOT_ADDLIFE;
//             m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_DOT_ADDMANA;
// 		}
// 		break;
	case STATUS_CITYWAR_WEAPON:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_CITYWAR_WEAPON;
		}
		break;
// 	case STATUS_DOT_ENMITY:	
// 		{
// 			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_SNEER;
// 		}
// 		break;
	case STATUS_DEAD_ADDTEAMATTR:
		{
		// 	m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_LIFESTONE;
		}
		break;
	case STATUS_CITYWAR_ATTACKMARK:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_CITYATTACk;
		}
		break;
	case STATUS_CITYWAR_DEFENCEMARK:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_CITYDEFENCE;
		}
		break;
	case STATUS_BASE_DOWN_PERSENT:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_BASEDOWN;
		}
		break;

	case STATUS_FISH_FOOD:        //这些状态不加特效
	case STATUS_FISH_WORKER:
	case STATUS_FISH_TUTOR:
	case STATUS_CITYWAR_SAFETIME:
	case STATUS_VERIFICATION_WAIT:
	case STATUS_VERIFICATION_RUNNING:
		{
		}
		break;
	case STATUS_BIGBLOOD_USERNOT:
		{
		}
		break;
	case STATUS_WARGAME:
		{

		}
		break;
	case STATUS_SELF_DAMAGEUP:
		{
		}
		break;
	case STATUS_STRENGUP_NUMBER:
		{
			
		}
		break;
	case STATUS_SPENDMP_UP://蓝消耗增加
	case STATUS_STAUP_NUMBER://增加体质
	case STATUS_STADOWN_NUMBER://减少体质
	case STATUS_SPIUP_NUMBER://增加精神
	case STATUS_SPIDOWN_NUMBER://减少精神
	case STATUS_AGIUP_NUMBER://增加敏捷
	case STATUS_AGIDOWN_NUMBER://减少敏捷
	case STATUS_TOUCHUP_NUMBER://增加坚韧
	case STATUS_TOUGHDOWN_NUMBER://减少坚韧
	case STATUS_SELF_DAMAGEUP_NUMBER://自身对敌伤害增加
	case STATUS_INTUP_NUMBER://增加智力
	case STATUS_MAGIC_SHIELD://魔法盾，受到伤害时扣蓝，蓝不够了扣血
	case STATUS_FOURATTUP_NUMBER://四属性增加
	case STATUS_FOURATTDOWN_NUMBER://四属性减少
	case STATUS_BASEATTUP_NUMBER://五属性增加
	case STATUS_SELF_DAMAGEUP_PERCENT://自身对敌伤害增加
		{
			
		}
		break;
	case STATUS_EUDEMON_SOUL://宠物附体
	case STATUS_STRENGUP_NUMBER_PET://增加力量
	case STATUS_STRENGDOWN_NUMBER_PET://减少力量
	case STATUS_SPIUP_NUMBER_PET://增加精神
	case STATUS_SPIDOWN_NUMBER_PET://减少精神
	case STATUS_AGIUP_NUMBER_PET://增加敏捷
	case STATUS_AGIDOWN_NUMBER_PET://减少敏捷
	case STATUS_STAUP_NUMBER_PET://增加体质
	case STATUS_STADOWN_NUMBER_PET://减少体质
	case STATUS_INTUP_NUMBER_PET://增加智力
	case STATUS_INTDOWN_NUMBER_PET://减少智力
	case STATUS_BOOTH_ADDEXP:
	case STATUS_ACTIVATE_ADDEXP:
	case STATUS_DOT_SYNEXP:
	case STATUS_DOT_TUTOREXP:
	case STATUS_DOT_WARGAMEEXP:
	case STATUS_ITEM_ADDEXP:
	case STATUS_DOT_LIFEMISS1:
	case STATUS_MONSTER_ENMITY:
	case STATUS_DEFDOWN_ATKUP:
	case STATUS_EQUIP_DEFUP://内外防增加	属定制装备
	case STATUS_EQUIP_DAMAGEUP://对敌伤害增加  属定制装备
	case STATUS_EQUIP_CRIUP://暴击增加		属定制装备
	case STATUS_EQUIP_BEDAMAGEUP://受到伤害增加	属定制装备
	case STATUS_EQUIP_DEFDOWN://内外防降低	属定制装备
	case STATUS_DOT_REDMANA://减蓝DOT
	case STATUS_DOT_REDSP://减怒气DOT
	case STATUS_BIZ_DEAD:// 跑商死亡后的buff
	case STATUS_FIREDEF_UP://火防
	case STATUS_ICEDEF_UP://冰防
	case STATUS_MYSTICDEF_UP://玄防
	case STATUS_NOXDEF_UP://毒防
	case STATUS_SYN_HITRATEDOWN://命中降低
	case STATUS_SYN_SHIELD://一定时间内免疫伤害	护盾
//	case STATUS_SYNWORKTIME://帮派打工buff
	case STATUS_SOUL_CRI://暴击			武魂
	case STATUS_SOUL_HITRATE://命中			武魂
	case STATUS_SOUL_DODGE://闪避			武魂
	case STATUS_SOUL_INNERATT://内功			武魂
	case STATUS_SOUL_OUTERATT://外功			武魂
	case STATUS_SOUL_INNERDEF://内防			武魂
	case STATUS_SOUL_OUTERDEF://外防			武魂
	case STATUS_SOUL_REBLOOD://回血			武魂
	case STATUS_SOUL_REBLOOD_PER:
	case STATUS_SOUL_REMP://回魔			武魂
	case STATUS_TUTOR_BOSS://师徒任务打特殊怪物需要的Buff
//modify code by 林德渊 begin 2011.6.22
	case STATUS_RETORT_TENSEC:
	case STATUS_RETORT_LGWS:
	case STATUS_RETORT_ZHF:
	case STATUS_RETORT_MDSX:
	case STATUS_RETORT_MDSX_BUFF2:
	case STATUS_RETORT_MDSX_1:
//modify code by 林德渊 end
	case STATUS_PET_DODGE://宠物附体加闪避
	case STATUS_PET_HITRATE://宠物附体加命中
	case STATUS_DOT_ADDEXP://活动用

		{
			
		}
		break;
//--------------END-----------------------------------------------------
	default:
		::LogSave("Invalid status: [%u]", nStatus);
	}

	if (STATUS_DREAMING == nStatus || STATUS_SLEEP == nStatus || STATUS_CONFUSION == nStatus )
	{
		this->ClrAttackTarget();	// 停止自动战斗
	}
	if( STATUS_FRENZY == nStatus || STATUS_ATKSPEEDUP_PERCENT == nStatus || STATUS_ATKSPEEDDOWN_PERCENT == nStatus )
	{
		this->CalcFightRate();
	}
	if(bSynchro && (i64OldEffect != GetEffect() || i64OldEffect2 != GetSecondEffect()))///加了i64OldEffect2 != GetSecondEffect()扩展判断,softworms-2007-08-14,混乱啊.
	{
		CMsgUserAttrib msg;
		if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
		{

			if ( STATUS_MOVESPEEDUP_PERCENT == nStatus || STATUS_MOVESPEEDDOWN_PERCENT == nStatus || STATUS_FRENZY == nStatus || STATUS_MOUNTSPEED_NUMBER == nStatus || STATUS_SKZL == nStatus )
			{                                                                                                                        // 坐骑速度 [陈剑飞8/2/2007]
				msg.Append( _USERATTRIB_SPEED, AdjustSpeed( GetSpeed() ) );
			}
			else if ( STATUS_MAXLIFEUP_PERCENT == nStatus || STATUS_MAXLIFEDOWN_PERCENT == nStatus )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			else if( STATUS_MAXMANAUP_PERCENT == nStatus || STATUS_MAXMANADOWN_PERCENT == nStatus  )
			{
				msg.Append( _USERATTRIB_MAXMANA,this->GetMaxMana() );
				msg.Append( _USERATTRIB_MANA,this->GetMana() );
			}
			else if( STATUS_MAXSPDOWN_PERCENT == nStatus || STATUS_MAXSPDOWN_PERCENT == nStatus )
			{
				msg.Append( _USERATTRIB_MAXSP,this->GetMaxSp() );
				msg.Append( _USERATTRIB_SP,this->GetSp() );
			}
			//  [1/30/2008 陈剑飞]
			else if( STATUS_LIFE_UP == nStatus )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			//----------------------------------------------
			// 新技能不屈意志准备 [12/25/2007 陈剑飞]
			else if( STATUS_FORTITUDE_WILL == nStatus )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			//----------------------------------------------


			//处理second effect
			if( i64OldEffect2 != GetSecondEffect() )
			{
				msg.Append( _USERATTRIB_SECONDEFFECTLOW32,this->GetSecondEffect() );
			}

			BroadcastRoomMsg(&msg, INCLUDE_SELF,EM_SENDNPCFLAG_FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//06.3.3王玉波修改
//函数功能：清除玩家的状态
void CUser::ClsStatus( int nStatus, bool bSynchro )
{
	I64 i64OldEffect = GetEffect();
	I64 i64OldEffect2 = GetSecondEffect();

	switch (nStatus)
	{		
	case	STATUS_NORMAL:
		{
			::LogSave( "CUser::ClsStatus nStatus=STATUS_NORMAL Invalid" );
		}
		break;
	case	STATUS_DIE:
		{
			m_data.m_Info.dwStatus = STATUS_NORMAL;//20070825修罗:清除死亡状态时,将此值赋为normal
			m_data.m_Info.i64Effect	&= ~KEEPEFFECT_DIE;
		}
		break;
	case	STATUS_TEAMLEADER:
		{
		     m_data.m_Info.i64Effect	&= ~KEEPEFFECT_TEAMLEADER;
		}
		break;
	case	STATUS_CRIME:           
		{
			m_data.m_Info.i64Effect	&= ~KEEPEFFECT_CRIME;
			ClsBuff(STATUS_CRIME);
		}
		break;
	case STATUS_LIANGONG:       
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_LIANGONG;
		}
		break;
	case STATUS_ACTIONEND:       
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_ACTIONEND;
		}
		break;	
	case STATUS_ATTRACTMONSTER://吸引怪物
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATTRACTMONSTER;
		}
		break;
	case	STATUS_PKVALUE:        // change huang 2004.1.11  todo
		{
			//20070228修罗
			//暂时做最保守处理
			//先清除
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_RED;
			if(IsRedName())
			{
				m_data.m_Info.i64Effect |= KEEPEFFECT_RED;
			}
			else if(IsGrayName())
			{
				//在别处处理.
				//case	STATUS_CRIME: 
			}
			else if(IsWhiteName())
			{
				//在别处处理
				//CUser::ClsStatus...case	STATUS_PKVALUE:
			}
			else
			{
				//warning
				LOGERROR( "CUser::SetStatus函数出错,用户:%s PK:%d",this->GetName(),this->GetPk() );
			}


			// 取消黄名状态，保留黑名、红名
//			m_data.m_Info.i64Effect	&= 0xFFFFF0F;//(KEEPEFFECT_BLUE|KEEPEFFECT_GREEN|PKVALUE_YELLOW|PKVALUE_REDNAME|KEEPEFFECT_BLACK);
//			if (m_data.GetPk() <= PKVALUE_BLUE)
//				m_data.m_Info.i64Effect	|= KEEPEFFECT_BLUE;
//			else if (m_data.GetPk() < PKVALUE_GREEN)
//				m_data.m_Info.i64Effect	|= KEEPEFFECT_GREEN;
//			else if(m_data.GetPk() < PKVALUE_YELLOW && m_data.GetPk() > 0)
//				m_data.m_Info.i64Effect |= KEEPEFFECT_YELLOW;
//			else if(m_data.GetPk() < PKVALUE_REDNAME && m_data.GetPk() > 0)
//				m_data.m_Info.i64Effect |= KEEPEFFECT_RED;
//			else if(m_data.GetPk() >= PKVALUE_BLACKNAME && m_data.GetPk() > 0)
//				m_data.m_Info.i64Effect |= KEEPEFFECT_BLACK;
		}
		break;
	case STATUS_PK_PROTECT:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PK_PROTECT;
		}
		break;
	case STATUS_ENEMYFIRE:///仇视特效-softworms-2007-08-13
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_ENIMYFIRE;///暂时用资料战的特效
		}
		break;
// 	case STATUS_SKZL:		//获得时空之泪，减速作用
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_SKZL;
// 		}
// 		break;
	
//-------------------------------------------------------------------------------
	case STATUS_FRENZY:							//狂暴
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_FRENZY;
		}
		break;
	case STATUS_SHIELD:							//一定时间内可抵消N点消害	护盾
	case STATUS_SHIELD_1:						//一定时间内可抵消N点消害	护盾
	case STATUS_SHIELD_2:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_SHIELD;
		}
		break;
	case STATUS_FIRMNESS_MAGIC:					//一定时间内增加技能不被打断的机率	坚定施法
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_FIRMNESS_MAGIC;
		}
		break;
	case STATUS_HIDDEN:							//隐身
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_HIDDEN;
		}
		break;
	case STATUS_RETORT:							//反击
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_RETORT;
		}
		break;
	case STATUS_LURK:							//NPC看不到自己，对玩家无效	潜行
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_LURK;
		}
		break;
	case STATUS_IMMUNE_ALL:						//免疫一切伤害	无敌
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_IMMUNE_ALL;
		}
		break;
	case STATUS_SLEEP:							//不能移动、攻击、技能、使用物品	战斗不能（被攻击可解除）
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_SLEEP;
		}
		break;
	case STATUS_DREAMING:						//不能移动、攻击、技能、使用物品	战斗不能（被攻击不解除）
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DREAMING;
		}
		break;
	case STATUS_PALSY:							//不能移动	定身，麻痹	
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PALSY;
		}
		break;
	case STATUS_SILENCE:						//不能使用技能	沉默
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_SILENCE;
		}
		break;
	case STATUS_CONFUSION:						//鼠标乱转，丢失目标	混乱
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_CONFUSION;
		}
		break;
	case STATUS_STOPSAY:						//不能说话，非控制系	禁言	
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_STOPSAY;
		}
		break;
	case STATUS_PHYSICALATTACKUP_PERCENT:				//物理攻击力上升
	case STATUS_PHYSICALATTACKUP_PERCENT_WS://2007827修罗:
	case STATUS_PHYATTACKUP_TUTOR_PERCENT://  [陈剑飞10/26/2007]
	case STATUS_PHYSICALATTUP_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALATTACKUP;
		}
		break;
	case STATUS_PHYSICALATTACKDOWN_PERCENT:				//物理攻击力下降
	case STATUS_PHYSICALATTDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALATTACKDOWN;
		}
		break;
	case STATUS_MAGICATTACKUP_PERCENT:					//魔法攻击力上升
	case STATUS_MAGICATTACKUP_PERCENT_ED:
	case STATUS_MAGATTACKUP_TUTOR_PERCENT://  [陈剑飞10/27/2007]
//	case STATUS_COMB_MAIN:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICATTACKUP;
		}
		break;
	case STATUS_MAGICATTACKDOWN_PERCENT:				//魔法攻击力下降
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICATTACKDOWN;
		}
		break;
	case STATUS_ATTACKUP_PERCENT:						//攻击力上升
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATTACKUP;
		}
		break;
	case STATUS_ATTACKDOWN_PERCENT:						//攻击力下降
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATTACKDOWN;
		}
		break;
	case STATUS_PHYSICALDEFENCEUP_PERCENT:				//物理防御上升
	case STATUS_PHYSICALDEFENCEUP_NUMBER:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_SF://2007827修罗:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_L:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALDEFENCEUP;
		}
		break;
	case STATUS_PHYSICALDEFENCEDOWN_PERCENT:			//物理防御下降
	case STATUS_PHYSICALDEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALDEFENCEDOWN;
		}
		break;
	case STATUS_MAGICDEFENCEUP_PERCENT:					//魔法防御上升
	case STATUS_MAGICDEFENCEUP_NUMBER:
	case STATUS_MAGICDEFENCEUP_NUMBER_EM:
	case STATUS_MAGICDEFENCEUP_NUMBER_L:
	case STATUS_MAGICDEFENCEUP_NUMBER_1:
		{
			m_data.m_Info.i64Effect &= 	~KEEPEFFECT_MAGICDEFENCEUP;
		}
		break;
	case STATUS_MAGICDEFENCEDOWN_PERCENT:				//魔法防御下降
	case STATUS_MAGICDEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICDEFENCEDOWN;
		}
		break;
	case STATUS_DEFENCEUP_PERCENT:						//防御力上升
	case STATUS_DEFENCEUP_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DEFENCEUP;
		}
		break;
	case STATUS_DEFENCEDOWN_PERCENT:					//防御力下降
	case STATUS_DEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DEFENCEDOWN;
		}
		break;
	case STATUS_HITRATEUP_PERCENT:						//命中率上升
	case STATUS_HITRATEUP_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_HITRATEUP;
		}
		break;
	case STATUS_HITRATEDOWN_PERCENT:					//命中率下降
	case STATUS_HITRATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_HITRATEDOWN;
		}
		break;
	case STATUS_DODGERATEUP_PERCENT:					//躲避率上升
	case STATUS_DODGERATEUP_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DODGERATEUP;
		}
		break;
	case STATUS_DODGERATEDOWN_PERCENT:					//躲避率下降
	case STATUS_DODGERATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DODGERATEDOWN;
		}
		break;
	case STATUS_CRITRATEUP_PERCENT:						//增加致命一击机率
	case STATUS_CRITRATEUP_NUMBER:
	case STATUS_CRITRATEUP_NUMBER_SW://2007827修罗:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_CRITRATEUP;
		}
		break;
	case STATUS_CRITRATEDOWN_PERCENT:					//减少致命一击机率
	case STATUS_CRITRATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_CRITRATEDOWN;
		}
		break;
	case STATUS_MAXLIFEUP_PERCENT:						//最大生命值增加
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXLIFEUP;
		}
		break;
	case STATUS_MAXLIFEDOWN_PERCENT:					//最大生命值减少
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXLIFEDOWN;
		}
		break;
	case STATUS_MAXMANAUP_PERCENT:						//最大魔法值增加
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXMANAUP;
		}
		break;
	case STATUS_MAXMANADOWN_PERCENT:					//最大魔法值减少
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXMANADOWN;
		}
		break;
	case STATUS_MAXSPUP_PERCENT:						//最大体力值增加
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXSPUP;
		}
		break;
	case STATUS_MAXSPDOWN_PERCENT:						//最在体力值减少
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXSPDOWN;
		}
		break;
	case STATUS_MOVESPEEDUP_PERCENT:					//移动速度增加
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOUNTSPEED_NUMBER:					//移动速度增加 坐骑
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOVESPEEDDOWN_PERCENT:					//移动速度减少
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MOVESPEEDDOWN;
		}
		break;
	case STATUS_ATKSPEEDUP_PERCENT:						//攻击速度增加
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATKSPEEDUP;
		}
		break;
	case STATUS_ATKSPEEDDOWN_PERCENT:					//攻击速度减少
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATKSPEEDDOWN;
		}
		break;
	case STATUS_PHYSICALDAMAGEUP_PERCENT:				//物理伤害增大
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALDAMAGEUP;
		}
		break;
	case STATUS_PHYSICALDAMAGEDOWN_PERCENT:				//物理伤害减小
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALDAMAGEDOWN; 
		}
		break;
	case STATUS_MAGICDAMAGEUP_PERCENT:					//魔法伤害增大
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICDAMAGEUP;
		}
		break;
	case STATUS_MAGICDAMAGEDOWN_PERCENT:				//魔法伤害减小
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICDAMAGEDOWN;
		}
		break;
	case STATUS_DAMAGEUP_PERCENT:						//伤害增大
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DAMAGEUP;
		}
		break;
	case STATUS_DAMAGEDOWN_PERCENT:						//伤害减小
	case STATUS_DAMAGEDOWN_PERCENT_TN://2007827修罗:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DAMAGEDOWN;

		}
		break;
	case STATUS_GETEXPUP_PERCENT:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_GETEXPUP;
		}
		break;
	case STATUS_GETEXPUP_PERCENT_SYNINS:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_GETEXPUP;
		}
		break;
	case STATUS_GETEXPUP_PERCENT_WT:
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_EUDEMONEXPUP;
		}
		break;
	case STATUS_GETEXPDOWN_PERCENT:						//经验获得减少
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_GETEXPDOWN;
		}
		break;
// 	case STATUS_ERUPTPOT_PERCENT:						//潜能爆发
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ERUPTPOT;
// 		}
// 		break;
// 	case STATUS_FROSTSHIELD:								//寒霜甲
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_FROSTSHIELD;
// 		}
// 		break;

//------------dot begin-------------------------------------
 	case STATUS_DOT_POISONING:				//中毒DOT			,掉hp
// 	case STATUS_DOT_POISON_ATK:            // 毒袭 [12/13/2007 陈剑飞]
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_POISONING;
		}
		break;
// 	case STATUS_DOT_ENMITY:
	case STATUS_DOT_LIFEMISSING:			//流血DOT			,掉hp
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_LIFEMISSING;
		}
		break;
// 	case STATUS_DOT_MANAMISSING:			//精神流失DOT		,掉mp
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_MANAMISSING;
// 		}
// 		break;
// 	case STATUS_DOT_STRENGTHMISSING:		//体力流失DOT		,掉sp
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_STRENGTHMISSING;
// 		}
// 		break;
 	case STATUS_DOT_BURNING:				//灼伤DOT			,掉hp
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_BURNING;
		}
		break;
//-----------dot end------------------------------------
//SECOND STATUS BEGIN---------------------------------------------------
	case STATUS_CRAZE://狂热
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_CRAZE;
		}
		break;
	case STATUS_PVPMODE://决斗buff
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_PVP;
		}
		break;
	case STATUS_YABIAO:
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_YABIAO;
		}
		break;
	case STATUS_JIEBIAO:
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_JIEBIAO;
		}
		break;
	case STATUS_DOT_SUFFERING://缠绕DOT
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_SUFFERING;
		}
		break;
// 	case STATUS_DOT_BEATING://打击DOT
// 	case STATUS_DOT_BEATINGPRE:// 致命打击2 [12/12/2007 陈剑飞]
// 		{
// 			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_BEATING;
// 		}
// 		break;
	case STATUS_DETECTION://识破
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_DETECTION;

		}
		break;
	case STATUS_RESOURCEWAR_PROTECT://资源战保护状态
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_RESOURCEWAR_PROTECT;

		}
		break;
 	case STATUS_DOT_ADDLIFE://加血dot
// 	case STATUS_DOT_ADDLIFE_RB://2007827修罗:
// 	case STATUS_DOT_QUICK_ADDLIFE:// 快速恢复 [12/25/2007 陈剑飞]
// 	case STATUS_DOT_ADDLIFE_L:
// 		{
// 			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_DOT_ADDLIFE;
// 		}
// 		break;
	case STATUS_DOT_ADDMANA://加蓝DOT
// 	case STATUS_DOT_ADDMANA_MC://2007827修罗:
// 	case STATUS_DOT_ADDMANA_L:
// 		{
// 			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_DOT_ADDMANA;
// 
// 		}
// 		break;
		//  [1/30/2008 陈剑飞]
	case STATUS_LIFE_UP:
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_LIFEUP;
		}
		break;
	case STATUS_COMB_MAIN:
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_COMB_MAIN;
		}
		break;
	case STATUS_COMB_SECONDARY:
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_COMB_SECONDARY;
		}
		break;
// 	case STATUS_BUFF_HPMP:
// 		{
// 			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_DOT_ADDLIFE;
// 			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_DOT_ADDMANA;
// 		}
// 		break;
	case STATUS_CITYWAR_WEAPON:
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_CITYWAR_WEAPON;
		}
		break;

// 	case STATUS_DOT_ENMITY:
// 		{
// 			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_SNEER;
// 		}
// 		break;

	case STATUS_DEAD_ADDTEAMATTR:
		{
		// 	m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_LIFESTONE;
		}
		break;

	case STATUS_CITYWAR_ATTACKMARK:
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_CITYATTACk;
		}
		break;
	case STATUS_CITYWAR_DEFENCEMARK:
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_CITYDEFENCE;
		}
		break;
	case STATUS_BASE_DOWN_PERSENT:
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_BASEDOWN;
		}
		break;

	case STATUS_FISH_FOOD:        //这些状态不加特效
	case STATUS_FISH_WORKER:
	case STATUS_FISH_TUTOR:
	case STATUS_CITYWAR_SAFETIME:
	case STATUS_VERIFICATION_WAIT:
	case STATUS_VERIFICATION_RUNNING:
		{
		}
		break;
	case STATUS_BIGBLOOD_USERNOT:
		{
		}
		break;
	case STATUS_WARGAME:
		{

		}
		break;
	case STATUS_SELF_DAMAGEUP:
		{
		}
		break;
	case STATUS_STRENGUP_NUMBER:
		{

		}
		break;
	case STATUS_SPENDMP_UP://蓝消耗增加
	case STATUS_STAUP_NUMBER://增加体质
	case STATUS_STADOWN_NUMBER://减少体质
	case STATUS_SPIUP_NUMBER://增加精神
	case STATUS_SPIDOWN_NUMBER://减少精神
	case STATUS_AGIUP_NUMBER://增加敏捷
	case STATUS_AGIDOWN_NUMBER://减少敏捷
	case STATUS_TOUCHUP_NUMBER://增加坚韧
	case STATUS_TOUGHDOWN_NUMBER://减少坚韧
	case STATUS_SELF_DAMAGEUP_NUMBER://自身对敌伤害增加
	case STATUS_INTUP_NUMBER://增加智力
	case STATUS_MAGIC_SHIELD://魔法盾，受到伤害时扣蓝，蓝不够了扣血
	case STATUS_FOURATTUP_NUMBER://四属性增加
	case STATUS_FOURATTDOWN_NUMBER://四属性减少
	case STATUS_BASEATTUP_NUMBER://五属性增加
	case STATUS_SELF_DAMAGEUP_PERCENT://自身对敌伤害增加
		{
			
		}
		break;
	case STATUS_EUDEMON_SOUL://宠物附体
	case STATUS_STRENGUP_NUMBER_PET://增加力量
	case STATUS_STRENGDOWN_NUMBER_PET://减少力量
	case STATUS_SPIUP_NUMBER_PET://增加精神
	case STATUS_SPIDOWN_NUMBER_PET://减少精神
	case STATUS_AGIUP_NUMBER_PET://增加敏捷
	case STATUS_AGIDOWN_NUMBER_PET://减少敏捷
	case STATUS_STAUP_NUMBER_PET://增加体质
	case STATUS_STADOWN_NUMBER_PET://减少体质
	case STATUS_INTUP_NUMBER_PET://增加智力
	case STATUS_INTDOWN_NUMBER_PET://减少智力
	case STATUS_BOOTH_ADDEXP:
	case STATUS_ACTIVATE_ADDEXP:
	case STATUS_DOT_TUTOREXP:
	case STATUS_DOT_SYNEXP:
	case STATUS_DOT_WARGAMEEXP:
	case STATUS_ITEM_ADDEXP:
	case STATUS_DOT_LIFEMISS1:
	case STATUS_MONSTER_ENMITY:
	case STATUS_DEFDOWN_ATKUP:
	case STATUS_EQUIP_DEFUP://内外防增加	属定制装备
	case STATUS_EQUIP_DAMAGEUP://对敌伤害增加  属定制装备
	case STATUS_EQUIP_CRIUP://暴击增加		属定制装备
	case STATUS_EQUIP_BEDAMAGEUP://受到伤害增加	属定制装备
	case STATUS_EQUIP_DEFDOWN://内外防降低	属定制装备
	case STATUS_DOT_REDMANA://减蓝DOT
	case STATUS_DOT_REDSP://减怒气DOT
	case STATUS_BIZ_DEAD:// 跑商死亡后的buff
	case STATUS_FIREDEF_UP://火防
	case STATUS_ICEDEF_UP://冰防
	case STATUS_MYSTICDEF_UP://玄防
	case STATUS_NOXDEF_UP://毒防
	case STATUS_SYN_HITRATEDOWN://命中降低
	case STATUS_SYN_SHIELD://一定时间内免疫伤害	护盾
//	case STATUS_SYNWORKTIME://帮派打工buff
	case STATUS_SOUL_CRI://暴击			武魂
	case STATUS_SOUL_HITRATE://命中			武魂
	case STATUS_SOUL_DODGE://闪避			武魂
	case STATUS_SOUL_INNERATT://内功			武魂
	case STATUS_SOUL_OUTERATT://外功			武魂
	case STATUS_SOUL_INNERDEF://内防			武魂
	case STATUS_SOUL_OUTERDEF://外防			武魂
	case STATUS_SOUL_REBLOOD://回血			武魂
	case STATUS_SOUL_REBLOOD_PER:
	case STATUS_SOUL_REMP://回魔			武魂
	case STATUS_TUTOR_BOSS://师徒任务打特殊怪物需要的Buff
//modify code by 林德渊 begin 2011.6.22
	case STATUS_RETORT_TENSEC:
	case STATUS_RETORT_LGWS:
	case STATUS_RETORT_ZHF:
	case STATUS_RETORT_MDSX:
	case STATUS_RETORT_MDSX_BUFF2:
	case STATUS_RETORT_MDSX_1:
//modify code by 林德渊 end
	case STATUS_PET_DODGE://宠物附体加闪避
	case STATUS_PET_HITRATE://宠物附体加命中
	case  STATUS_DOT_ADDEXP://活动用

		{
			
		}
		break;
//--------------END-----------------------------------------------------


	default:
		::LogSave("Invalid status: [%u]", nStatus);
	}

	if( STATUS_FRENZY == nStatus || STATUS_ATKSPEEDUP_PERCENT == nStatus || STATUS_ATKSPEEDDOWN_PERCENT == nStatus )
	{
		this->CalcFightRate();
	}
	if(bSynchro && (i64OldEffect != GetEffect() || i64OldEffect2 != GetSecondEffect()))///加了i64OldEffect2 != GetSecondEffect()扩展判断,softworms-2007-08-14,混乱啊.
	{

		CMsgUserAttrib msg;
		if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
		{
			if ( STATUS_MOVESPEEDUP_PERCENT == nStatus || STATUS_MOVESPEEDDOWN_PERCENT == nStatus || STATUS_FRENZY == nStatus || STATUS_MOUNTSPEED_NUMBER == nStatus || STATUS_SKZL == nStatus )
			{                                                                                                                       // 坐骑速度 [陈剑飞8/2/2007]
				msg.Append( _USERATTRIB_SPEED, AdjustSpeed( GetSpeed() ) );
			}
			else if ( STATUS_MAXLIFEUP_PERCENT == nStatus || STATUS_MAXLIFEDOWN_PERCENT == nStatus )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			else if( STATUS_MAXMANAUP_PERCENT == nStatus || STATUS_MAXMANADOWN_PERCENT == nStatus  )
			{
				msg.Append( _USERATTRIB_MAXMANA,this->GetMaxMana() );
				msg.Append( _USERATTRIB_MANA,this->GetMana() );
			}
			else if( STATUS_MAXSPDOWN_PERCENT == nStatus || STATUS_MAXSPDOWN_PERCENT == nStatus )
			{
				msg.Append( _USERATTRIB_MAXSP,this->GetMaxSp() );
				msg.Append( _USERATTRIB_SP,this->GetSp() );
			}
			else if( STATUS_LIFE_UP == nStatus )//  [1/31/2008 陈剑飞]
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			// 新技能不屈意志准备 [12/25/2007 陈剑飞]
			else if( STATUS_FORTITUDE_WILL == nStatus )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			//----------------------------------------------


			//处理second effect
			if( i64OldEffect2 != GetSecondEffect() )
			{
				msg.Append( _USERATTRIB_SECONDEFFECTLOW32,this->GetSecondEffect() );
			}

			BroadcastRoomMsg(&msg, INCLUDE_SELF,EM_SENDNPCFLAG_FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
void CUser::IncAddPoint(int nAddPoint)
{
	int nNewPoint	= nAddPoint + m_data.GetPot();

	if (nNewPoint < 0)
		nNewPoint	= 0;

	m_data.SetPot(nNewPoint);
}


bool CUser::SetCurJob(int nJob)
{
	if(nJob !=  1 && nJob !=  2)
		return false;
	if(m_data.GetCurJob() ==  nJob )
		return false;

	m_data.SetCurJob(nJob);
	//重新加载技能//优化，不用每次转职都同步一次，只在上线时把两个职业的技能都发过去
// 	if (m_pMagic)
// 		m_pMagic->ReleaseByOwner();
// 	m_pMagic = CMagic::CreateNew(m_idProcess, this->QueryRole());
// 	ASSERT(m_pMagic);
// 	if (!m_pMagic || !m_pMagic->CreateAll())
// 		return false;
// 	SendAllMagicInfo();

	CMsgUserAttrib	msg;
	msg.Create(GetID(),_USERATTRIB_PORFESSION,GetProfession(1) );
	msg.Append(_USERATTRIB_PORFESSION_SEC,GetProfession(2));
	msg.Append(_USERATTRIB_CURJOB, nJob);
	BroadcastRoomMsg(&msg,true);	

	DEBUG_TRY
	if (GetMountState())//下坐骑
	{
		SetUnMount();
		CMsgPlayer msgPlayer;
		if (msgPlayer.Create(QueryRole()))
			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		
		CRole::DetachStatus(this,STATUS_MOUNTSPEED_NUMBER);
	}
	UpdateCalculatedProperty(UPDATECALTYPE_ALL);
	DEBUG_CATCH("CUser::SetCurJob(int nJob)");
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::IsAlive()
{
	if ((GetStatus() & STATUS_DIE) == STATUS_DIE || this->GetLife() <= 0)
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetForce(DWORD dwForce)
{
	//m_data.SetForce(dwForce);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetDexterity(DWORD dwDex)
{
	//m_data.SetDexterity(dwDex);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetSpeed(DWORD dwSpeed)
{
	m_data.SetSpeed(dwSpeed);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetSoul(DWORD dwSoul)
{
	//m_data.SetSoul(dwSoul);
}
void CUser::SetLoveliness(DWORD dwLoveli)
{
//	m_data.SetLoveliness(dwLoveli);
}
/////////////////////////////////////////////////////////////////////////////
void CUser::SetHealth(DWORD dwHealth)
{
//	m_data.SetHealth(dwHealth);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetFace(DWORD dwFace)
{
	
	DWORD dwLookFace = m_data.GetLookFace();
	dwLookFace = (dwLookFace-this->GetFace()*10000)+(dwFace%1000)*10000;

	m_data.SetLookFace(dwLookFace);
}

/////////////////////////////////////////////////////////////////////////////
void CUser::SetLook(DWORD dwLook)
{
	DWORD dwLookFace = m_data.GetLookFace();
	dwLookFace = (dwLookFace/1000)*1000 + (dwLook%1000);

	m_data.SetLookFace(dwLookFace);
}
//[2010-06-08 goto]修改心情，并通知好友
void CUser::SetFeel(char* szFeel)
{
	m_data.SetFeel(szFeel);//为了防止好友上线时看到的仍然是修改前的心情，将同步数据库设为true
	for(int i = 0; i < m_setBeFriend.size(); i++)
	{
		CFriend* pFriend = m_setBeFriend[i];
		CUserPtr pTarget = NULL;
		pTarget = UserManager()->GetUser(pFriend->GetUserID());
		if(pTarget)
		{
			CFriend* pBeFriend=pTarget->GetFriend(this->GetID());
			if(pBeFriend)
				pBeFriend->SetFriendFeel(szFeel);
			CMsgFriend pMsg;
			if (pMsg.Create(_FRIEND_BESETFEEL,this->GetID(),this->GetName(),CMsgFriend::ONLINE_STATUS,0,0,0,0,0,0,szFeel,0))
				pTarget->SendMsg(&pMsg);
		}
	}
}
/*////////////////////////////////////////////////////////////////////////////
BOOL CUser::CheckStatus(int nStatus)
{
	if (nStatus == STATUS_NORMAL || STATUS_DIE == nStatus)
	{
		if (m_infoStatus.dwStatus == nStatus)
			return true;
		else
			return false;
	}
	else
	{
		if ((m_infoStatus.dwStatus & nStatus) == nStatus)
			return true;
		else
			return false;
	}
}*/

/////////////////////////////////////////////////////////////////////////////
bool CUser::IsJumpPass(int x, int y, int nAlt)
{
	vector<POINT>	setLine;
	DDALineEx(GetPosX(), GetPosY(), x, y, setLine);

	int nSize = setLine.size();
	if(nSize <= 2)
		return true;
	ASSERT(x == setLine[nSize-1].x);
	ASSERT(y == setLine[nSize-1].y);
	//传送处理 satan******************************
// 	float	fAlt	= (float)(GetMap()->GetFloorAlt(GetPosX(), GetPosY()) + nAlt) + 0.5;
// 	float	fDelta	= (float)(GetMap()->GetFloorAlt(x, y) - fAlt) / nSize;
// 	for(int i = 0; i < nSize; i++)
// 	{
// 		if(GetMap()->IsAltOver(setLine[i], (int)fAlt))
// 			return false;
// //		fAlt	+= fDelta;
// 	}

	return true;
}
/////////////////////////////////////////////////////////////////////////////
bool CUser::IsArrowPass(int x, int y, int nAlt/*=ARROW_FLY_ALT*/)
{
	return true;//[2010-06-26 goto]不检查阻挡
//两点之间是否可以穿透......
//-------------------------------王玉波注释以下并修改------------------------------------
/*	return true;		//? 暂不检查 /////////////////////////////////////////////////////////////

	vector<POINT>	setLine;
	DDALineEx(GetPosX(), GetPosY(), x, y, setLine);

	int nSize = setLine.size();
	if(nSize <= 2)
		return true;
	ASSERT(x == setLine[nSize-1].x);
	ASSERT(y == setLine[nSize-1].y);
	float	fAlt	= (float)(GetMap()->GetFloorAlt(GetPosX(), GetPosY()) + nAlt) + 0.5;
	float	fDelta	= (float)(GetMap()->GetFloorAlt(x, y) - fAlt) / nSize;
	for(int i = 0; i < nSize; i++)
	{
		if(GetMap()->IsAltOver(setLine[i], (int)fAlt))
			return false;
		fAlt	+= fDelta;
	
	}

	return true;*/
//----------------------------------end---------------------------------------------------------
	vector<POINT>	setLine;
	DDALineEx(GetPosX(), GetPosY(), x, y, setLine);

	int nSize = setLine.size();
//----------------------------------06.10.23--begin------------------
//	ASSERT(x == setLine[nSize-1].x);
//	ASSERT(y == setLine[nSize-1].y);
	//没有考虑掉nSize 为零时,nSize-1访问数组越界.前面是if( nSize <=2 ) retun
//--------------------------------------------end---------------------
	for(int i = 0; i < nSize; i++)
	{
		if( GetMap()->IsStandEnable( setLine[ i ].x,setLine[ i ].y ) )
			continue;
		this->SendSysMsg( g_objStrRes.GetStr( 15064) );//"操作取消,阻挡!"
		return false;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
CUser*	CUser::FindAroundUser(OBJID id)
{
	IRole* pRole = FindAroundRole(id);
	CUser* pUser = NULL;
	if(pRole)
		pRole->QueryObj(OBJ_USER, IPP_OF(pUser));

	return pUser;
}

CUser* CUser::FindUserInMap(OBJID id)
{
	CUser * pMember = UserManager()->GetUser(id);
	if( pMember )
	{
		// distance
		if (pMember->GetMap()->GetID() != this->GetMap()->GetID())
			return NULL;
		else
			return pMember;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
DWORD	CUser::GetInterAtkRate()			
{
// 	int nRate = GetIntervalAtkRate();
// 
// 	int nWeaponRate = 0;
// 	if ( m_pWeaponR && m_pWeaponR->IsWeapon() )	
// 		nWeaponRate = /*m_pWeaponR->GetInt(ITEMDATA_ATKSPEED)*/800;
// 	if( nWeaponRate != 0 )//如果有武器，用武器的攻击频率
// 	{
// 		nRate = nWeaponRate;
// 	}
// 
// 	int nChangeData = 0;
// 
// 	IStatus* pStatus = this->QueryStatus( STATUS_FRENZY );
// 	if( pStatus )
// 	{
// 		nChangeData += ( - 1 ) * CRole::CalcStatusInfluence( STATUS_FRENZY,nRate,pStatus->GetPower() );
// 	}
// 	nRate += nChangeData;

	return /*nRate*/1000;
}

/////////////////////////////////////////////////////////////////////////////
DWORD	CUser::GetIntervalAtkRate()			
{
	return USER_ATTACK_SPEED;
}

/////////////////////////////////////////////////////////////////////////////
int	CUser::GetAttackRange(int nTargetSizeAdd)			
{
	int nRange = 1;
	DWORD prof = this->GetProfession();
	
	if( prof & PROF_MASTER )
	{
		nRange = 11;
		
	}
	else if( prof & PROF_PASTOR )
	{
		nRange = 11;
		
	}
	else if( prof & PROF_SWORD )
	{
		nRange = 11;
		
	}
	else
	{
		nRange = 3;
	}
	return nRange;
}

/////////////////////////////////////////////////////////////////////////////
int	CUser::GetSizeAdd()			
{
	if(QueryTransformation())
		return QueryTransformation()->GetSizeAdd();

	return 0;
}

DWORD	CUser::GetAddAttByType(int nType)
{
	int nData = 0;
	switch(nType)
	{
	case 1:
		nData = m_dwUserAtt1;
		break;
	case 2:
		nData = m_dwUserAtt2;
		break;
	case 3:
		nData = m_dwUserAtt3;
		break;
	case 4:
		nData = m_dwUserAtt4;
		break;

	case 11:
		nData = m_dwUserDef1;
		break;
	case 12:
		nData = m_dwUserDef2;
		break;
	case 13:
		nData = m_dwUserDef3;
		break;
	case 14:
		nData = m_dwUserDef4;
		break;
	}
	return nData;
}

DWORD	CUser::GetAtkType()
{ 
	//先考虑buff
	CItem* pWeapenSoul = GetEquipItemByPos(ITEMPOSITION_WEAPONSOUL);
	if(pWeapenSoul && pWeapenSoul->IsWeaponSoul())
	{
		CWeaponSoulData* pData = pWeapenSoul->QueryWeapensoulData();
		if(pData)
			return pData->GetInt(WEAPONSOULDATA_TYPE) % 10;
	}
	return 0;
}

DWORD 	CUser::AdjustDamage		(DWORD nDamage,int nAttribType)
{
	return nDamage;
}
int	CUser::AdjustUserAttrib(DWORD nAttrib,int nAttribType)///调整玩家最大魔法值,softworms-2007-09-05
{
	IStatus* pStatus = NULL;
	int nAttAddi = 0;
	switch(nAttribType)
	{
	case ADJUSTUSERATTRIB_PHYDEF:///物防(外防)
		{
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEUP_PERCENT );//物理防御上升%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEDOWN_PERCENT );//物理防御下降%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEUP_NUMBER );//物理防御上升（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEDOWN_NUMBER );//物理防御下降（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DEFDOWN_ATKUP );//以防换攻（点数）
			if (pStatus)
			{
				nAttAddi += (-2)*CRole::CalcStatusInfluence( STATUS_DEFDOWN_ATKUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFUP );//物理防御上升（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFDOWN );//物理防御下降（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFDOWN,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_OUTERDEF );//物理防御（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_OUTERDEF,nAttrib,pStatus->GetPower() );
			}

		}
		break;
	case ADJUSTUSERATTRIB_MAXMANA:///最大法力值
		{
			pStatus = QueryStatus( STATUS_MAXMANAUP_PERCENT );//最大魔法值增加%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXMANAUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAXMANADOWN_PERCENT );//最大魔法值减少%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXMANADOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAXLIFE:///最大生命值
		{
			pStatus = QueryStatus( STATUS_MAXLIFEUP_PERCENT );//最大生命值增加%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXLIFEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAXLIFEDOWN_PERCENT );//最大生命值减少%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXLIFEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAGDEF:///魔防(内防)
		{
			pStatus = QueryStatus( STATUS_MAGICDEFENCEUP_PERCENT );//魔法防御上升%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEDOWN_PERCENT );//魔法防御下降%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEUP_NUMBER );//魔法防御上升（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEUP_NUMBER_1 );//魔法防御上升（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEUP_NUMBER_1,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEDOWN_NUMBER );//魔法防御下降（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFUP );//魔法防御上升（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFDOWN );//魔法防御下降（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFDOWN,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_INNERDEF );//内防（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_INNERDEF,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_HITRANGE:///命中率
		{
			pStatus = QueryStatus( STATUS_HITRATEUP_PERCENT );//命中率上升%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_HITRATEDOWN_PERCENT );//命中率下降%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_HITRATEUP_NUMBER );//命中率上升（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PET_HITRATE );//命中率上升（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PET_HITRATE,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_HITRATEDOWN_NUMBER );//命中率下降（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SYN_HITRATEDOWN );//命中率下降（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SYN_HITRATEDOWN,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_HITRATE );//命中率（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_HITRATE,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_DOG:///闪避
		{
			pStatus = QueryStatus( STATUS_DODGERATEUP_PERCENT );//闪避率上升%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DODGERATEDOWN_PERCENT );//闪避率下降%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DODGERATEUP_NUMBER );//闪避率上升（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PET_DODGE );//闪避率上升（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PET_DODGE,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DODGERATEDOWN_NUMBER );//闪避率下降（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_DODGE );//闪避率（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_DODGE,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_PHYACK:///物攻(外功)
		{
			pStatus = QueryStatus( STATUS_PHYSICALATTACKUP_PERCENT );//物理攻击力上升%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTACKUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALATTACKDOWN_PERCENT );//物理攻击力下降%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTACKDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALATTUP_NUMBER );//物理攻击增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALATTDOWN_NUMBER );//物理攻击减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DEFDOWN_ATKUP );//以防换攻（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DEFDOWN_ATKUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_OUTERATT );//外功（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_OUTERATT,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAGACK:///魔攻(内功)
		{
			pStatus = QueryStatus( STATUS_MAGICATTACKUP_PERCENT );//魔法攻击力上升%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICATTACKUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICATTACKDOWN_PERCENT );//魔法攻击力下降%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICATTACKDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_INNERATT );//内功
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_INNERATT,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_Crit:///暴击
		{
			pStatus = QueryStatus( STATUS_CRITRATEUP_PERCENT );//暴击上升%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_CRITRATEDOWN_PERCENT );//暴击下降%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_CRITRATEUP_NUMBER );//暴击增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_CRITRATEDOWN_NUMBER );//暴击减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_CRIUP );//暴击增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_CRIUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_CRI );//暴击（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_CRI,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_STR://增加力量
		{
			pStatus = QueryStatus( STATUS_STRENGUP_NUMBER );//力量增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STRENGDOWN_NUMBER );//力量减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STRENGUP_NUMBER_PET );//力量增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STRENGDOWN_NUMBER_PET );//力量减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus(STATUS_EUDEMON_SOUL);//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_INT://智力(灵力)
		{
			pStatus = QueryStatus( STATUS_INTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_INTDOWN_NUMBER );//减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_INTUP_NUMBER_PET );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_INTDOWN_NUMBER_PET );//减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_STA://体质
		{
			pStatus = QueryStatus( STATUS_STAUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STAUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STADOWN_NUMBER );//减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STADOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STAUP_NUMBER_PET );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STAUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STADOWN_NUMBER_PET );//减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STADOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STAUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_SPI://精神
		{
			pStatus = QueryStatus( STATUS_SPIUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SPIDOWN_NUMBER );//减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SPIUP_NUMBER_PET );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SPIDOWN_NUMBER_PET );//减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}

			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_AGI://敏捷
		{
			pStatus = QueryStatus( STATUS_AGIUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_AGIDOWN_NUMBER );//减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_AGIUP_NUMBER_PET );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_AGIDOWN_NUMBER_PET );//减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_TOUGH://坚韧
		{
			pStatus = QueryStatus( STATUS_TOUCHUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_TOUCHUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_TOUGHDOWN_NUMBER );//减少（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_TOUGHDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_TOUCHUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_FIREATK://火
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_ICEATK://冰
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MYSTICATK://玄
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_NOXATK://毒
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_FIREDEF://火
		{
			pStatus = QueryStatus( STATUS_FIREDEF_UP );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_ICEDEF://冰
		{
			pStatus = QueryStatus( STATUS_ICEDEF_UP );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MYSTICDEF://玄
		{
			pStatus = QueryStatus( STATUS_MYSTICDEF_UP );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_NOXDEF://毒
		{
			pStatus = QueryStatus( STATUS_NOXDEF_UP );//增加（点数）
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	default:
		break;
	}
	return nAttAddi;
}
//////////////////////////////////////////////////////////////////////
//调整玩家的移动速度
DWORD CUser::AdjustSpeed(int nSpeed)
{
	int nChangeData = 0;

	IStatus* pStatus = QueryStatus( STATUS_MOVESPEEDUP_PERCENT );
	if ( pStatus )
	{
		nChangeData += CRole::CalcStatusInfluence( STATUS_MOVESPEEDUP_PERCENT,nSpeed,pStatus->GetPower() );
	}
	pStatus = QueryStatus( STATUS_MOVESPEEDDOWN_PERCENT );
	if ( pStatus )
	{
		nChangeData += CRole::CalcStatusInfluence( STATUS_MOVESPEEDDOWN_PERCENT,nSpeed,pStatus->GetPower() );
	}
	pStatus = QueryStatus( STATUS_SKZL );
	if ( pStatus )
	{
		nChangeData += CRole::CalcStatusInfluence( STATUS_SKZL,nSpeed,pStatus->GetPower() );
	}
	//狂暴状态,增加移动速度
	pStatus = QueryStatus( STATUS_FRENZY );
	if( pStatus )//
	{
		nChangeData += CRole::CalcStatusInfluence( STATUS_FRENZY,nSpeed,pStatus->GetParam1() );	
	}
	// 坐骑速度 [陈剑飞8/2/2007]
	pStatus = QueryStatus( STATUS_MOUNTSPEED_NUMBER ); 
	if( pStatus )
	{
		nChangeData += CRole::CalcStatusInfluence( STATUS_MOUNTSPEED_NUMBER,nSpeed,pStatus->GetPower() );	
	}

	//城战武器速度
	pStatus = QueryStatus( STATUS_CITYWAR_WEAPON);
	if(pStatus)
	{
		return pStatus->GetPower();
	}

	nSpeed += nChangeData;

	//nSpeed = __min( 1000,__max( 1,nSpeed ) );
	nSpeed = __min( 150,__max( 1,nSpeed ) );//goto 最多150
	return nSpeed;
}

//////////////////////////////////////////////////////////////////////
void CUser::RemoteCall(REMOTE_CALL0* pInfo)
{
	switch(pInfo->ucFuncType)
	{
	case	REMOTE_CALL_TEAM_HUNTKILL:
		{
//			this->TeamHuntKill((OBJID)pInfo->IntParam[0], pInfo->IntParam[1], (const char*)pInfo->IntParam[2]);
		}
		break;
	case	REMOTE_CALL_DROP_LEADER:
		{
			CHECK(GetSynRank() == RANK_LEADER);

			// update new leader
			QuerySynAttr()->SetIdRank(0,pInfo->IntParam[0], pInfo->IntParam[1]);
		}
		break;
	case	REMOTE_CALL_LEAVE_SYN:
		{
			if(GetSynID() == pInfo->IntParam[0])
			{
				QuerySynAttr()->LeaveSyn(true, DELRECORD_FALSE, SYNCHRO_TRUE);		// false: need not delete recorder again
				SendSysMsg(g_objStrRes.GetStr( 11042), (char*)&pInfo->IntParam[1]);			//	STR_KICKOUT_ME_s
			}
		}
		break;
	case	REMOTE_CALL_TEAM_CLOSE_MONEY:
		{
			CTeam* pTeam = GetTeam();
			if(pTeam)
			{
				if(pInfo->IntParam[0])
					pTeam->SetCloseMoney(true);
				else
					pTeam->SetCloseMoney(false);
				this->SendSysMsg(_TXTATR_TEAM, g_objStrRes.GetStr( 11002), pInfo->IntParam[0] ? STR_CLOSE : STR_OPEN);//STR_TEAM_MONEY_s
			}
		}
		break;
	case	REMOTE_CALL_TEAM_CLOSE_ITEM:
		{
			CTeam* pTeam = GetTeam();
			if(pTeam)
			{
				if(pInfo->IntParam[0])
					pTeam->SetCloseItem(true);
				else
					pTeam->SetCloseItem(false);
				this->SendSysMsg(_TXTATR_TEAM, g_objStrRes.GetStr( 11003), pInfo->IntParam[0] ? STR_CLOSE : STR_OPEN);//STR_TEAM_ITEM_s
			}
		}
		break;
	case	REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS:
		{
			CTeam* pTeam = GetTeam();
			if(pTeam)
			{
				if(pInfo->IntParam[0])
					pTeam->SetCloseGemAccess(true);
				else
					pTeam->SetCloseGemAccess(false);
				this->SendSysMsg(_TXTATR_TEAM,g_objStrRes.GetStr( 11004) , pInfo->IntParam[0] ? STR_CLOSE : STR_OPEN);//STR_TEAM_GEM_s
			}
		}
		break;
		//20070416修罗:发工资到个人
	case REMOTE_CALL_SYNPAYOFF:
		{
			int nMoney = pInfo->IntParam[0];//个人所得
			AddMoney(nMoney,true);//同步,更新
			this->SendSysMsg(_TXTATR_SYNDICATE,g_objStrRes.GetStr( 15065),nMoney);//你获得了军团工资:%d"
		}
		break;
	case REMOTE_CALL_SYNADDMENBER:
		{
			CSyndicate* pSyn = this->GetSyndicate();
			if(pSyn)
			{
				CMsgSyndicate	msg;
				IF_OK(msg.Create(INVITE_JOIN_NAME, pInfo->IntParam[0],this->GetID()))
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg);
			}
			else
			{
				CSynPtr pSyn = SynManager()->QuerySyndicate(pInfo->IntParam[1]);
				if(pSyn)
				{
					CMsgSyndicate	msg;
					IF_OK_(msg.Create(INVITE_JOIN, pInfo->IntParam[0],pSyn->GetStr(SYNDATA_NAME)))
						SendMsg(&msg);
				}
			}
		}
		break;
	case REMOTE_CALL_SYNADDOK:
		{
			int nRank = RANK_NORMAL;
			CSynPtr pSyn = SynManager()->QuerySyndicate(pInfo->IntParam[1]);
			if(!pSyn)
				return;

			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807修罗:策划案
			if(pSyn->GetInt(SYNDATA_AMOUNT) >= MemberAmount[pSyn->GetInt(SYNDATA_LEVEL)])
			{
				this->SendSysMsg(g_objStrRes.GetStr(11036));//STR_SYN_FULL
				return;
			}

			OBJID idMate = 0;
//			this->QuerySynAttr()->GetMateIDByID(idMate, this->GetID());
			if(/*this->Get_MateID()*/idMate == pSyn->GetLeaderID())
			{
//				nRank = RANK_LEADERWIFE;
// 				pSyn->SetLeaderWifeID(this->GetID());
//				pSyn->QueryModify()->SetData(SYNDATA_LEADERWIFE, this->GetID(), true);
			}			
			
			this->QuerySynAttr()->JoinSyn(pInfo->IntParam[1], nRank);
			this->SendSysMsg(g_objStrRes.GetStr(11039), pSyn->GetMasterSyn()->GetStr(SYNDATA_NAME));//STR_JOIN_SYN_s
			MSGBUF szMsg;
			sprintf(szMsg,g_objStrRes.GetStr(11040), this->GetName());//STR_SYN_NOTIFY_JOIN_s
			if(/*this->Get_MateID()*/idMate == pSyn->GetLeaderID())
			{
				sprintf(szMsg,"%s来到本帮，作为本帮的帮主配偶!", this->GetName());
			}
			pSyn->BroadcastSubSynMsg(/*GetSocketID(), GetNpcID(),*/0,0, szMsg);
		}
		break;
	case REMOTE_CALL_KICKOUTPLAYER:
		{
			UserManager()->KickOutSocket(m_idSocket);
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////
bool CUser::Transform(DWORD dwType, int nKeepSecs, bool bSynchro/*=true*/)
{
	bool bBack = false;

	if(m_pTransformation)
	{
		m_pTransformation->Release();
		m_pTransformation = NULL;
		bBack = true;
	}

	CNpcType* pType = MonsterType()->GetObj(dwType);
	CHECKF(pType);

	CTransformation* pTransform = CTransformation::CreateNew();		// VVVVVVVVVVVVVVVVVVVVVVVVvv
	IF_OK(pTransform)
	{
		IF_OK(pTransform->Create(this, pType))
		{
			m_pTransformation = pTransform;
			m_tTransformation.Startup(nKeepSecs);
			if(bSynchro)
				SynchroTransform();

			this->StopMine();
			return true;
		}
		else
			pTransform->Release();		// AAAAAAAAAAAAAAAAAAAAAAAA
	}

	if(bBack)
		SynchroTransform();

	return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::AbortTransform()
				{ SAFE_RELEASE(m_pTransformation); SynchroTransform(); }

//////////////////////////////////////////////////////////////////////
bool CUser::SynchroTransform()
{
	CMsgUserAttrib	msg;
	IF_OK( msg.Create(GetID(), _USERATTRIB_LOOKFACE, GetLookFace())
			&& msg.Append(_USERATTRIB_MAXLIFE, GetMaxLife())
			&& msg.Append(_USERATTRIB_LIFE, GetLife())
			&& msg.Append(_USERATTRIB_SIZEADD, GetSizeAdd()) )
		BroadcastRoomMsg(&msg, INCLUDE_SELF);

	this->BroadcastTeamLife(true);		// true: with max life

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CallPet(OBJID idMonsterType, int x, int y, int nKeepSecs/*=0*/)
{
	KillCallPet();

	CMonster* pMonster = NpcManager()->CreateCallPet(this, idMonsterType, x, y);
	CHECKF(pMonster);
	m_pCallPet = pMonster->QueryLink();

	if(nKeepSecs)
		m_tCallPet.Startup(nKeepSecs);
	else
		m_tCallPet.Clear();

	return true;
}

DWORD CUser::AwardEudemonExp(__int64 i64Exp)
{
	if(i64Exp <= 0)
		return 0;
	
	if(m_pWarPet)
	{
		if (m_pWarPet->AwardExp(i64Exp))
			return i64Exp;
	}
	return 0;
}

int CUser::GetEudemonLevOff(int index)
{
	int nTemp = 100;
// 	if(m_eudemonData[0].id && m_eudemonData[0].pMonster)
// 	{
// 		nTemp = m_eudemonData[0].pMonster->GetLev() - this->GetLev();
// 	}
	return nTemp;
}

bool CUser::AwardEudemon(int nEudemonType,EudemonData& data , bool bSet, bool born,int nBornGrow,bool bCompensation/* = false*/)		
{
	CHECKF(m_pUserPet);
	if(!born)
	{
		if(m_pUserPet->GetShowAmount() >= 3 + GetPacklev(4))
		{
			SendSysMsg("宠物数量已经到上限");
			return false;
		}
	}
	
	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(nEudemonType);
	CHECKF(pEudemonType);
	if(pEudemonType)
	{
		data.ownerId = this->GetID();
		data.playtimes = 0;
		data.id = ID_NONE;
		if(CreateEudemon(nEudemonType,bSet,&data,born,nBornGrow,bCompensation))
		{
			if (!born && !bCompensation)
			{
				IgotAnAmazingEudemon(nEudemonType, pEudemonType, data.name, data.id);
			}
			return data.id != ID_NONE;
		}
	}
	return false;
}

CUserPetData* CUser::AwardEudemonByEgg(int nEudemonType,EudemonData& data)
{
	CHECKF(m_pUserPet);

	if(m_pUserPet->GetShowAmount() >= 3 + GetPacklev(4))
	{
		SendSysMsg("宠物数量已经到上限");
		return NULL;
	}
	
	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(nEudemonType);
	CHECKF(pEudemonType);
	if(pEudemonType)
	{
		data.ownerId = this->GetID();
		data.playtimes = 0;
		data.id = ID_NONE;

		CUserPetData* pTemp = NULL;
		DEBUG_TRY
		pTemp = m_pUserPet->CreateNewPetByEgg(nEudemonType, &data);	
		if(pTemp)
		{
			UpdateMissionMsgByItemType(nEudemonType);
			CMsgEudemon msg;
			if(msg.Create(PETACTION_PetInfo, &data, GetID()))
				SendMsg(&msg);
		}
		DEBUG_CATCH2("CUser::CreateEudemon %u",nEudemonType);
		return pTemp;
// 		if(CreateEudemon(nEudemonType,bSet,&data,born,nBornGrow))
// 		{
// 			if (!born)
// 			{
// 				IgotAnAmazingEudemon(nEudemonType, pEudemonType, data.name, data.id);
// 			}
// 			return data.id != ID_NONE;
// 		}
	}
	return NULL;
}

bool CUser::LoadEudemon(OBJID id,EudemonData& Data,int nType/* = 3*/)
{
	CUserPetData* pPetData = m_pUserPet->QueryData(id);
	if(!pPetData)
		return false;

	pPetData->GetData(Data);
	m_pUserPet->fillType(Data,nType);
	m_pUserPet->fillAtt(Data,nType);

	
	if(nType == 3 && (Data.dwScore > pPetData->GetInt(EUDEMONDATA_MaxScore)))
		pPetData->SetInt(EUDEMONDATA_MaxScore,Data.dwScore,true);

	return true;
}

void CUser::EudemonDead()
{
	if(m_pWarPet)
		m_pWarPet->BeKill(NULL);
}

bool CUser::EudemonSkill(OBJID eduemonItemId,OBJID idItem,int skill,bool bLean/* =true*/)
{
	CHECKF(eduemonItemId);
	CHECKF(m_pUserPet);

	if(GetCallPetID()==eduemonItemId || m_idSoulEudemon==eduemonItemId)
	{
		SendSysMsg("出战或附体的宠物不能进行此操作");
		return false;
	}
	if(bLean)
	{
		if(!m_pUserPet->LearnSkill(eduemonItemId, idItem, skill))
		{
			CMsgAction msgError;
			if(msgError.Create(eduemonItemId,0,0,0,actionEudemonLean,0))
				SendMsg(&msgError);
			return false;
		}
	}
	else
	{
		if(!m_pUserPet->UplevSkill(eduemonItemId, skill))
		{
			CMsgAction msgError;
			if(msgError.Create(eduemonItemId,0,0,0,actionEudemonLean,0))
				SendMsg(&msgError);
			return false;
		}
	}
	return true;
}

bool CUser::EudemonMix(OBJID idEudemon1,OBJID idEudemon2)
{
	CUserPetData* pData1 = m_pUserPet->QueryData(idEudemon1);
	CUserPetData* pData2 = m_pUserPet->QueryData(idEudemon2);
	if(!pData1 || !pData2)
		return false;

	if(pData1->IsSoulState() || pData2->IsSoulState())
		return false;

	if(pData1->isLocked())
		return false;

	if(pData2->isLocked())
		return false;

	CEudemonTypeData*	pEudemonType1 = EudemonType()->GetObj(pData1->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType1)
		return false;
	CEudemonTypeData*	pEudemonType2 = EudemonType()->GetObj(pData2->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType2)
		return false;

	if(pEudemonType2->GetInt(EUDEMONTYPEDATA_needLev) < pEudemonType1->GetInt(EUDEMONTYPEDATA_needLev))
		return false;
	
	int nData = pData1->GetInt(EUDEMONDATA_understand);
	if(pData2->GetInt(EUDEMONDATA_bone) <= nData)
		return false;


	int failto = -1;
	bool okClear = false;
	int change = 0;
	int nNeedMoney = 10000;
	switch(nData)
	{
	case 0:
		change = 10000;
		break;
	case 1:
		change = 8500;
		break;
	case 2:
		change = 7000;
		break;
	case 3:
		change = 5500;
		okClear = true;
		break;
	case 4:
		change = 4000;
		break;
	case 5:
		change = 2500;
		failto = 4;
		okClear = true;
		break;
	case 6:
		change =1500;
		failto = 4;
		okClear = true;
		break;
	case 7:
		change = 1000;
		break;
	case 8:
		change = 500;
		failto = 7;
		okClear = true;
		break;
	case 9:
		change = 200;
		failto = 7;
		break;
	}
	
	int nFailTime = pData1->GetInt(EUDEMONDATA_failtime);
	change += nFailTime * 200;
	
	if(!SpendMoney(nNeedMoney,true))
		return false;

	if(!m_pUserPet->DeleteData(idEudemon2))
		return false;
	

	if(RandGet(10000) < change)
	{
		SendSysMsg("合成成功");
		nData ++;
		pData1->SetInt(EUDEMONDATA_understand,nData,true);

		EudemonData Data;
		LoadEudemon(idEudemon1,Data,2);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetInfo,&Data,GetID()))
			SendMsg(&msg);

// 		if(okClear)
// 			pData1->SetInt(EUDEMONDATA_failtime,0,true);
	}
	else
	{
		SendSysMsg("合成失败");
		if(failto != -1)
		{
			nData = failto;
		}

// 		nFailTime++;
// 
// 		pData1->SetInt(EUDEMONDATA_failtime,nFailTime);
		pData1->SetInt(EUDEMONDATA_understand,nData,true);
		
		EudemonData Data;
		LoadEudemon(idEudemon1,Data,2);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetInfo,&Data,GetID()))
			SendMsg(&msg);
	}
	return true;
}

void CUser::CallBackEudemon(OBJID idEudemon,bool bSave,bool bSend,bool bHappy)
{
	if(!m_pWarPet)
		return;

	if(m_pWarPet->GetID() != idEudemon)
		return;

	m_pWarPet->SaveInfo();

	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
		return;
	
	UnLockEudemon(idEudemon);

	if(bSave)
		pData->SetInt(EUDEMONDATA_state,PETSTATE_REST,true);

	if(bHappy)
	{
		int nHappy = pData->GetInt(EUDEMONDATA_happy);
		if(nHappy > 0)
		{
			nHappy -= 1;
			pData->SetInt(EUDEMONDATA_happy,nHappy,true);

			CMsgEudemonAttrib msg;
			if(msg.Create(idEudemon,EUDATTR_HAPPY,nHappy))
				SendMsg(&msg);
		}
	}

	if(bSend)
	{
		CMsgEudemonAttrib msg;
		if(msg.Create(idEudemon,EUDATTR_STATE,0))
		{
			msg.Append(EUDATTR_HAPPY,pData->GetInt(EUDEMONDATA_happy));
			SendMsg(&msg);
		}
		DetachStatusFromEudemon(idEudemon);//宠物被召回或死亡时去掉宠物给主人加的状态
	}

	CRole::DetachAllStatus(m_pWarPet->QueryRole());//UpdateMonsterProperty(false);//宠物收回时要清除身上所有buff,需要更新一下宠物面板数据
	m_pWarPet->DelMonster(true);
	//NpcManager()->DelMonsterByID(idEudemon);
	m_pWarPet = NULL;
	m_idEudemonCur = ID_NONE;
}

CMonster* CUser::CallEudemon(OBJID idEudemon,bool bSave,bool bSend)
{
	if(!IsAlive())
		return NULL;
	//m_idNowEudemon = 0;
	if (idEudemon==ID_NONE)
		return NULL;
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
		return NULL;

	if(IsLockedEudemon(idEudemon))
	{
		//SendSysMsg("宠物绑定中，不能出战");
		return NULL;
	}

	CEudemonTypeData*	pTypeData = EudemonType()->GetObj(pData->GetInt(EUDEMONDATA_typeid));
	if(!pTypeData)
		return NULL;

	EudemonData eudemonData;
	LoadEudemon(idEudemon,eudemonData,3);

	if(pData->GetInt(EUDEMONDATA_lifespan) <= 0)
	{
		SendSysMsg("宠物寿命已经用完，不能出战");
		return NULL;
	}

	if(pData->GetInt(EUDEMONDATA_happy) < 60)
	{
		SendSysMsg("宠物快乐值不足，不能出战");
		return NULL;
	}

	if(pData->GetInt(EUDEMONDATA_life) <= 0)
	{
		pData->SetInt(EUDEMONDATA_life,eudemonData.dwMaxLife/*1*/);
		CMsgEudemonAttrib msgAtt;
		if(msgAtt.Create(pData->GetID(),EUDATTR_LIFE,pData->GetInt(EUDEMONDATA_life)))
			SendMsg(&msgAtt);
	}
	
	if(m_pWarPet)
		CallBackEudemon(m_pWarPet->GetID(),true,true,false);

	for(int i = 1 ; i < 7 ; i ++)
	{
		if(i == 5)
			continue;

		if(GetMap()->IsValidPoint(GetPosX()+_DELTA_X[i] * 3,GetPosY()+_DELTA_Y[i] * 3))
			break;
	}

	if(i == 7)
		m_pWarPet = NpcManager()->CallEudemon(this, pData,pTypeData,eudemonData, GetPosX(), GetPosY());
	else
		m_pWarPet = NpcManager()->CallEudemon(this, pData,pTypeData,eudemonData, GetPosX()+_DELTA_X[i] * 3, GetPosY()+_DELTA_Y[i] * 3);

	if(m_pWarPet)
	{
		LockEudemon(idEudemon);

		if(bSave)
			pData->SetInt(EUDEMONDATA_state,PETSTATE_INWAR,true);

		if(bSend)
		{
			CMsgEudemonAttrib msg;
			if(msg.Create(idEudemon,EUDATTR_STATE,1))
				SendMsg(&msg);
		}

//		LockEudemon(idEudemon);

		m_lastEudemonLifespanCheck = clock();

		return m_pWarPet;
	}
	else
		return NULL;
}

bool CUser::CreateEudemon(OBJID idEudemonType,bool bSaveData,EudemonData* pData, bool born,int nBornGrow,bool bCompensation/* = false*/)//bSaveData==1 表示按pData创建  否则是从数据库中读取
{
	bool bRet =0;
	DEBUG_TRY
	bRet = m_pUserPet->CreateNewPetData(idEudemonType,pData,bSaveData,born,nBornGrow,bCompensation);	
	if(bRet && !born)
	{
		UpdateMissionMsgByItemType(idEudemonType);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetInfo,pData,GetID()))
			SendMsg(&msg);
	}
//modify code by 林德渊 begin 2011.5.24
	if( pData->grow > 20 || pData->understand > 0 )
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get pet %u(type:%u) create. grow(%u) understand(%u) level(%u) sex(%u) exp(%u)", GetID(), GetName(), pData->id, pData->typeId, pData->grow, pData->understand, pData->level, pData->sex, pData->exp);
//modify code by 林德渊 end
	DEBUG_CATCH2("CUser::CreateEudemon %u",idEudemonType);
	return bRet;
}

bool CUser::GetLeaveExp(int nType)
{
	DWORD dwHasTime = (m_data.GetLeaveExp()) / 60;
	if(dwHasTime < 1)
		return false;

	//-1.06*0.01*x^3+3.26*x^2+32.7*x+384
	DWORD dwPerExp = 384.5f + GetLev() * 32.7f + GetLev() * GetLev() * 3.26f - GetLev() * GetLev() * GetLev() * 0.0106f;
	DWORD dwPerMoney = 70 + GetLev() * 140;
	DWORD dwExp = dwPerExp * dwHasTime;
	DWORD dwMoney = 0;
	DWORD dwItemAmount = 0;

	dwExp *= nType;

	switch(nType)
	{
	case 1:
		break;
	case 2:
		{
			dwMoney = dwPerMoney * dwHasTime;
		}
		break;
	case 4:
		{
			dwItemAmount = 1;
		}
		break;
	case 8:
		{
			dwItemAmount = 2;
		}
		break;
	default:
		return false;
	}

	if(dwMoney > 0)
	{
		if(GetMoney() < dwMoney)
		{
			SendSysMsg("银两不足，无法提取");
			return false;
		}
		if(!SpendMoney(dwMoney,true))
		{
			SendSysMsg("银两不足，无法提取");
			return false;
		}
	}

	if(dwItemAmount > 0)
	{
		DWORD dwHasItem = GetItemNumByType(630014);
		if(dwHasItem < dwHasItem)
		{
			SendSysMsg("逍遥丹不足，无法提取");
			return false;
		}

		bool bHasKind = false;
		if(!DelItemByTypeMulti(630014,dwItemAmount,bHasKind))
		{
			SendSysMsg("逍遥丹不足，无法提取");
			return false;
		}
	}

	DWORD dwGetExp = AwardBattleExp(dwExp);
	if(dwGetExp > 0)
	{
		m_data.SetLeaveExp(/*dwHasTime % 60*/0);
		SendSysMsg("你获得了%u经验",dwGetExp);
		return true;
	}

	return false;
}

void CUser::UpdateLeaveExp()
{
	if(m_nLevExp != -1)
		return;

	DWORD dwLastLogout = m_data.GetLastLogout();
	if(dwLastLogout == 0)
	{
		m_nLevExp = 0;
		return;
	}
	
	CTime timeCur(CTime::GetCurrentTime());
	CTime timeLastCheck(2000 + dwLastLogout / 100000000, (dwLastLogout % 100000000) / 1000000, (dwLastLogout % 1000000) / 10000, (dwLastLogout % 10000) / 100, dwLastLogout % 100,0);
	CTimeSpan timePass = timeCur - timeLastCheck;
	
	DWORD dwHasTime = m_data.GetLeaveExp();
	LONG dwPassMin = timePass.GetTotalMinutes();
	m_nLevExp = dwHasTime;
	
	if(dwPassMin < 1)
	{
		return;
	}

	if(m_data.GetLev() < 15)
	{
		return;
	}
	
	if(dwHasTime >= 8 * 60)
	{
		return;
	}
	else if(dwHasTime + dwPassMin > 8 * 60)
	{
		m_nLevExp = 8 * 60;
		m_data.SetLeaveExp(8 * 60);
		return;
	}
	else
	{
		m_nLevExp = dwHasTime + dwPassMin;
		m_data.SetLeaveExp(dwHasTime + dwPassMin);
		return;
	}
}

DWORD CUser::QueryLeaveExp()
{
	if(m_nLevExp == -1)
		return 0;
	
	return m_nLevExp / 60;
}

void CUser::EudemonEat(OBJID idEudemon)
{
	CHECK(m_pUserPet);
	CHECK(m_pPackage);
	int nStep = 0;
	DEBUG_TRY
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	nStep = 1;
	if(pData)
	{
// 		if(IsLockedEudemon(idEudemon))
// 			return;

		CItem* pItem = NULL;
		nStep = 2;
		DEBUG_TRY
		pItem = m_pPackage->GetPetEatItem();
		DEBUG_CATCH("m_pPackage->GetPetEatItem()");
		if(!pItem)
		{
			SendSysMsg("没有宠物药品，请补充");
			return;
		}
		nStep = 3;
		int nItemType = pItem->GetID();
		if(UseItemTo(idEudemon,pItem->GetID()))
		{
			nStep = 4;
			CMsgItem msg;
			IF_OK(msg.Create(ITEMACT_USE_OK ,0,0,0,0,nItemType,0,0,0))
			{
				SendMsg(&msg);
			}
		}
	}
	DEBUG_CATCH3("CUser::EudemonEat %u %u",idEudemon,nStep);
}

void CUser::EudemonLifeSpanadd(OBJID idEudemon)
{
	CHECK(m_pUserPet);
	CHECK(m_pPackage);
	int nStep = 0;
	DEBUG_TRY
		CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	nStep = 1;
	if(pData)
	{
		// 		if(IsLockedEudemon(idEudemon))
		// 			return;
		
		CItem* pItem = NULL;
		nStep = 2;
		DEBUG_TRY
			pItem = m_pPackage->GetPetLifeSpanItem();
		DEBUG_CATCH("m_pPackage->GetPetLifeSpanItem()");
		if(!pItem)
		{
			SendSysMsg("没有宠物药品，请补充");
			return;
		}
		nStep = 3;
		int nItemType = pItem->GetID();
		if(UseItemTo(idEudemon,pItem->GetID()))
		{
			nStep = 4;
			CMsgItem msg;
			IF_OK(msg.Create(ITEMACT_USE_OK ,0,0,0,0,nItemType,0,0,0))
			{
				SendMsg(&msg);
			}
		}
	}
	DEBUG_CATCH3("CUser::EudemonEat %u %u",idEudemon,nStep);
}

void CUser::EudemonTrian(OBJID idEudemon)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(pData)
	{
		CItem* pItem = m_pPackage->GetPetTrainItem();
		if(!pItem)
		{
			SendSysMsg("没有宠物玩具，请补充");
			return;
		}
		UseItemTo(idEudemon,pItem->GetID());
	}
}
bool CUser::EudemonGiveup(OBJID idEudemon, bool bIgnoreLock/* = false*/)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(pData)
	{	
//modify code by 林德渊 begin 2011.5.24
		int datalevel = pData->GetInt(EUDEMONDATA_level);
		int datasex = pData->GetInt(EUDEMONDATA_sex);
		int dataexp = pData->GetInt(EUDEMONDATA_exp);
//modify code by 林德渊 end
		OBJID idType = pData->GetInt(EUDEMONDATA_typeid);

		int nGrow = pData->GetInt(EUDEMONDATA_grow);
		int nUnderStand = pData->GetInt(EUDEMONDATA_understand);

		if(IsLockedEudemon(idEudemon))
			return false;

		if (!bIgnoreLock)//忽略安全锁,比如交任务的时候
		{
			if (IsItemLockedOpen())
			{
				SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return false;
			}
		}
		bool bRet = false;
//		CItem* pItem = GetItem(pData->GetInt(EUDEMONDATA_itemid));
// 		if(pItem)
// 		{
// 			EraseItem(pItem->GetID(),true);
// 		}
// 		else
// 		{
			if(m_pUserPet->DeleteData(idEudemon))
			{
				EudemonData data;
				CMsgEudemon msg;
				data.id = idEudemon;
				if(msg.Create(PETACTION_DelPet,&data,GetID()))
					SendMsg(&msg);
				bRet = true;
			}
//		}

		DEBUG_TRY
		UpdateMissionMsgByItemType(idType);
		DEBUG_CATCH("pUser->UpdateMissionMsgByItemType eudemon")
//modify code by 林德渊 begin 2011.5.24
		if(nGrow>20 || nUnderStand>0)
		{
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost pet %u(type:%d) from giveup. grow(%u) understand(%u) level(%u) sex(%u) exp(%u)", GetID(), GetName(), idEudemon, idType, nGrow, nUnderStand, datalevel, datasex, dataexp);
		}
//modify code by 林德渊 end
		return bRet;
	}
	return false;
}

bool CUser::EudemonChgName(OBJID idEudemon,char* name)
{
	if(strlen(name) < 1 || strlen(name) > 14)
		return false;
	if(!::NameStrCheck(name))
	{
		SendSysMsg("名字非法");
		return false;
	}
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(pData)
	{
// 		if(IsLockedEudemon(idEudemon))
// 			return false;

		pData->SetStr(EUDEMONDATA_name,name,16,true);
		EudemonData data;
		if(pData->GetData(data))
		{
			m_pUserPet->fillType(data,1);
			CMsgEudemon msg;
			if(msg.Create(PETACTION_PetInfo,&data,GetID()))
				SendMsg(&msg);
		}
		SendSysMsg("改名成功");


		if(m_pWarPet && (pData->GetID() == m_pWarPet->GetID()) )
		{																	
			CMsgPlayer msgPlayer;
			if (msgPlayer.Create(m_pWarPet->QueryRole()))
				m_pWarPet->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}

		return true;
	}
	return false;
}

//宠物被召回或死亡时去掉宠物给主人加的状态
void CUser::DetachStatusFromEudemon(OBJID idEudemon)
{
	IRole* pRole = RoleManager()->QueryRole(idEudemon);
	if(!pRole)
		return;
	CMagic* pMagicEndemon = pRole->QueryMagic();
	if (!pMagicEndemon)
		return;
	IMagicSet* pMagicSet = pMagicEndemon->QuerySet();
	if (!pMagicSet)
		return;
	
	for (int i=0; i<pMagicSet->GetAmount(); i++)
	{
		IMagicData* pMagicData = pMagicSet->GetObjByIndex(i);
		if (!pMagicData)
			continue;
		if (pMagicData->GetInt(MAGICDATA_SORT)==MAGICSORT_ATTACHSTATUS && pMagicData->GetInt(MAGICDATA_CRIME)==2 && pMagicData->GetInt(MAGICDATA_STEP)==-1)
			DetachStatus(this, pMagicData->GetInt(MAGICDATA_STATUS));
	}
}
//////////////////////////////////////////////////////////////////////
void CUser::KillCallPet(bool bNow/*=false*/)
{
//	m_idEudemonCurForChangeMap = 0;
	if(m_pWarPet)
	{
	//	m_idEudemonCurForChangeMap = m_pWarPet->GetID();
		CallBackEudemon(m_pWarPet->GetID(),false,false,false);
	}
// 	if(!m_pCallPet)
// 		return ;
// 
// 	if(!m_pCallPet->IsDeleted())
// 		m_pCallPet->DelMonster(bNow);
}

//////////////////////////////////////////////////////////////////////
OBJID CUser::GetCallPetID()			{ if(m_pWarPet) return QueryCallPet()->GetID(); return ID_NONE; }

//////////////////////////////////////////////////////////////////////
bool CUser::IsSimpleMagicAtk	()
{ return QueryTransformation() && QueryTransformation()->GetMagicType() != ID_NONE; }

//////////////////////////////////////////////////////////////////////
int CUser::GetSimpleMagicHitRate()
{ IF_OK(QueryTransformation()) return QueryTransformation()->GetMagicHitRate(); else return 100; }

//////////////////////////////////////////////////////////////////////
bool CUser::SetWeather(int nType, int nIntensity, int nDir, DWORD nColor, DWORD nKeepSecs, DWORD nSpeedSecs)
{
// 	if(m_idWeather != ID_NONE)
// 	{
// 		CWeatherRegion* pRegion = GetMap()->QueryWeatherRegion(m_idWeather);
// 		IF_OK(pRegion)
// 			return pRegion->QueryWeather()->SetNewWeather(nType, nIntensity, nDir, nColor, nKeepSecs, nSpeedSecs);
// 	}

	return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::SetLight(int dwRGB, int nSecs/*=0*/)
{
// 	if(nSecs == 0)		// keep light
// 		GetMap()->SetLight(dwRGB);
// 
// 	CMsgAction	msg;
// 	IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), nSecs, actionMapARGB, dwRGB))
// 		UserManager()->BroadcastMapMsg(GetMapID(), &msg, NULL);
}

bool CUser::CreateBooth(int nPosX, int nPosY, int nDir)
{ 
	if(m_pBooth) 
		return false; 
	m_pBooth = CBooth::CreateNew(m_idProcess, this, nPosX, nPosY, nDir);
	if(!m_pBooth) 
		return false; 
	MapManager()->QueryBoothSet()->AddObj(m_pBooth);

	CMsgPlayer msgPlayer;
	if (msgPlayer.Create(QueryRole()))
		BroadcastRoomMsg(&msgPlayer, EXCLUDE_SELF);
	return true; 
}


void CUser::DestroyBooth()						
{ 
	if(m_pBooth)
	{
		MapManager()->QueryBoothSet()->DelObj(m_pBooth->GetID());
	}
	m_pBooth = NULL; 

	CMsgPlayer msgPlayer;
	if (msgPlayer.Create(QueryRole()))
		BroadcastRoomMsg(&msgPlayer, EXCLUDE_SELF);
	CRole::DetachStatus(this, STATUS_BOOTH_ADDEXP);
}
//////////////////////////////////////////////////////////////////////
void CUser::SendLight()
{
// 	int	nKeepSecs = 0;		// keep light
// 
// 	CMsgAction	msg;
// 	IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), nKeepSecs, actionMapARGB, GetMap()->GetLight()))
// 		SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
void CUser::AwardSynWarScore(CNpc* pNpc, int nScore)
{
	//06.10.25王玉波注释此函数,函数功能:帮派战争用到的加某些帮派数据
/*	CHECK(pNpc);
	CHECK(nScore > 0);

	if(GetSynID() == ID_NONE || GetSynID() == pNpc->GetSynID())
		return ;

	int nAddProffer = MulDiv(nScore, SYNWAR_PROFFER_PERCENT, 100);
	if(nAddProffer)
		QuerySynAttr()->AddProffer(nAddProffer);

	int nAddMoney	= MulDiv(nScore, SYNWAR_MONEY_PERCENT, 100);
	if(nAddMoney > 0)
	{
		CSyndicate* pSyn = SynManager()->QuerySyndicate(pNpc->GetSynID());
		if(pSyn)
		{
			nAddMoney	= ::CutOverflow(nAddMoney, pSyn->GetInt(SYNDATA_MONEY));
			pSyn->QueryModify()->AddData(SYNDATA_MONEY, -1*nAddMoney, true);
			this->GainMoney(nAddMoney, SYNCHRO_TRUE);
			//
		}
	}

	CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
	if(pSyn)
	{
		pSyn = pSyn->GetMasterSyn();
		pSyn->AddSynWarScore(pNpc->GetID(), nScore);
		
	}*/
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsHostileSynMember	(CUser* pTarget)
{
	IF_NOT (pTarget)
		return false;

	OBJID idSynTarget	= pTarget->GetSynID();
	OBJID idSyn			= this->GetSynID();
	if (ID_NONE != idSynTarget && ID_NONE != idSyn)
	{
		CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
		if (pSyn && pSyn->IsHostile(idSynTarget))
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
CSyndicate* CUser::GetSyndicate(void)
{
	if(this->GetSynID() != ID_NONE)
		return SynManager()->QuerySyndicate(this->GetSynID());

	return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsMemberOfSyn(OBJID idSyn)
{
	if (idSyn == ID_NONE)
		return false;
	CSyndicate* pSyn = this->GetSyndicate();
	while (pSyn)
	{
		if (pSyn->GetID() == idSyn)
			return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////
void CUser::SetRecordPos(OBJID idMap, DWORD dwX, DWORD dwY, BOOL bUpdate/* = false*/)
{
	PROCESS_ID idProcess = MapManager()->GetProcessID(idMap);
 	if(idProcess == MSGPORT_MAPGROUP_INSTANCE)
 		return;

	CHECK(idProcess != PROCESS_NONE);
	if(idProcess == m_idProcess)
	{
		CGameMap* pMap = MapManager()->GetGameMap(idMap);
		IF_NOT (pMap)
			return;
//暂时注释.
//		IF_NOT(pMap->IsStandEnable(dwX, dwY))
//			return;
		if(CWargame::IsWarGameMap(idMap))//专线的竞技场不保存
			return;
		if (pMap->GetType()==3)
			return;
		if (pMap->IsSynMap())
			return;
	}

	m_data.SetRecordPos(idMap, dwX, dwY, bUpdate);
}
//////////////////////////////////////////////////////////////////////
void CUser::SetSpecilPos(OBJID idMap, DWORD dwX, DWORD dwY,SPECILLINESTATE nState,BOOL bUpdate)
{
	m_pSpecilLineInfo->SetState(nState);
	m_pSpecilLineInfo->SetRecordPos(idMap,dwX,dwY);
}
//////////////////////////////////////////////////////////////////////
void CUser::SetSpecilSteate(SPECILLINESTATE nState ,bool bUpdate /* = false */)//注意先确定有记录
{
	m_pSpecilLineInfo->SetState(nState);
}
bool CUser::KickBack()
{
	ClearWalkData();
	if(!this->IsSynchro())
		return false;

	CMsgAction	msg;
	IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionKickBack, GetPosX(), GetPosY()))
		SendMsg(&msg);

//#define  KICKBACK_DEBUG
#ifdef KICKBACK_DEBUG
	::LogSave("kickback id= %u,mapid= %u,x= %u,y= %u",GetID(),GetMapID(),GetPosX(),GetPosY());
#endif

	this->SetSynchro(false);
	return true;
}

//////////////////////////////////////////////////////////////////////
 bool CUser::Synchro()
{
	CMsgAction	msg;
	IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionSynchro, GetPosX(), GetPosY()))
		SendMsg(&msg);

	this->SetSynchro(false);
	return true;
}

//////////////////////////////////////////////////////////////////////
//OBJID CUser::GetMateID()
//{
//	if(!IsMarried())
//		return ID_NONE;
//
//	CUser* pMate = UserManager()->GetUser(this->GetMate());
//	if(pMate)
//		return pMate->GetID();
//
//	SQLBUF	szSQL;
//	sprintf(szSQL, "SELECT id FROM %s where name='%s' LIMIT 1", _TBL_USER, GetMate());
//	CAutoPtr<IRecordset> pRes = _Database(PID)->CreateNewRecordset(szSQL);
//	if(pRes)
//		return pRes->GetInt(0);
//
//	return ID_NONE;
//}


int CUser::AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg)
{
	CHECKF(pTarget);
	int nHeroExp = 0;

	nHeroExp = nRawExp;

	//通过等级之差调整人物经验
	nHeroExp = CBattleSystem::AdjustExp( nHeroExp, this->GetLev(), pTarget->GetLev() );

	int nChangeData = 0;
	IStatus* pStatus;
	pStatus = this->QueryStatus( STATUS_GETEXPUP_PERCENT );
	if (pStatus)
		nChangeData += CRole::CalcStatusInfluence( STATUS_GETEXPUP_PERCENT,nHeroExp,pStatus->GetPower() );
	
	pStatus = this->QueryStatus(STATUS_GETEXPUP_PERCENT_SYNINS);//这是帮派副本内的加经验buff
	if (pStatus)
	{
		if (GetMap()->GetDocID()== 2017)//只在该地图有双倍
			nChangeData += CRole::CalcStatusInfluence( STATUS_GETEXPUP_PERCENT_SYNINS,nHeroExp+nChangeData,pStatus->GetPower() );
	}
	//VIP加经验
	if(IsVipActive())
	{
		int nVipLev = GetVipLev()==4 ? 1 : GetVipLev();//周卡要和月卡效果一样
		nChangeData += nHeroExp * ( 0.10f + nVipLev *0.10f);
	}

	return nHeroExp + nChangeData;
}


int CUser::AdjustEudemonExp(IRole* pTarget,CMonster* pWarPet, int nRawExp, bool bNewbieBonusMsg)
{
	CHECKF(pTarget);
	int nHeroExp = 0;
	
	nHeroExp = nRawExp;
	
	//通过等级之差调整人物经验
	nHeroExp = CBattleSystem::AdjustExp( nHeroExp, pWarPet->GetLev(), pTarget->GetLev() );
	
	int nChangeData = 0;
	IStatus* pStatus;
	pStatus = this->QueryStatus( STATUS_GETEXPUP_PERCENT );
	if (pStatus)
		nChangeData += CRole::CalcStatusInfluence( STATUS_GETEXPUP_PERCENT,nHeroExp,pStatus->GetPower() );

	pStatus = this->QueryStatus( STATUS_GETEXPUP_PERCENT_WT );
	if (pStatus)
		nChangeData += CRole::CalcStatusInfluence( STATUS_GETEXPUP_PERCENT_WT,nHeroExp + nChangeData,pStatus->GetPower() );
	
	return nHeroExp + nChangeData;
}


//////////////////////////////////////////////////////////////////////
bool CUser::checkMonsterTag(int monsterID)
{
	return m_pBattleSystem->checkMonsterTag(monsterID);
}
bool CUser::CheckCrime(IRole* pRole)
{
	CHECKF(pRole);

	if (QueryStatus(STATUS_CRIME))
	{
		if(QueryStatus(STATUS_CRIME)->GetSecLeft() > 10)
			return false;
	}
	if(pRole->IsUser())
	{
		CUser *pUser = NULL;
		pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
		if(NULL != pUser && this->GetAutoFightFlag())
			return false;
		if (!pRole->GetMap())
		{
			return false;
		}
		if (pRole->GetMap()->IsPkGameMap())	//如果在比武场
		{
			return false;
		}
		if (pUser->GetPVPStuts())//如果在PK
		{
			return false;
		}
		if (!pUser->IsEvil() && (this->GetID() != pUser->GetID()))
		{
			SetCrimeStatus();	
			return true;
		}
	}
	else
	{
		CMonster *pPet = NULL;
		pRole->QueryObj(OBJ_MONSTER, IPP_OF(pPet));
		if (NULL != pPet && this->GetAutoFightFlag())
			return false;
		if (pPet!=NULL&& pPet->IsEudemon())
		{
			if (pPet->QueryOwnerUser())
			{
				if (pPet->QueryOwnerUser()->GetPVPStuts())//如果在PK
				{
					return false;
				}
				if (pPet->QueryOwnerUser()->GetMap()->IsPkGameMap())	//如果在比武场
				{
					return false;
				}
				if (!pPet->QueryOwnerUser()->IsEvil() && (pPet->QueryOwnerUser()->GetID() != this->GetID()))
				{
					SetCrimeStatus();	
					return true;
				}
			}
		}
	}

	CGameMap* pMap = this->GetMap();

// 		if(pRole->IsUser())
// 		{
// 			
// 			if (pTarget->IsUser())
// 			{
// 				CUser* pTargetUser = NULL;
// 				pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser));
// 				//			if (pUser->IsWhiteName() && (!pTargetUser->IsEvil()))
// 				if(pTargetUser && !pTargetUser->IsEvil())
// 				{
// 					pUser->SetCrimeStatus();
// 				}
// 			}
// 			else
// 			{
// 				CMonster* pTargetPet = NULL;
// 				pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pTargetPet));
// 				if (pTargetPet && pTargetPet->IsEudemon() && pTargetPet->QueryOwnerUser())
// 				{
// 					//				if (!(pTargetPet->QueryOwnerUser()->IsEvil()) && pUser->IsWhiteName())
// 					if(!pTargetPet->QueryOwnerUser()->IsEvil() && pUser->IsAtkable(pTargetPet))
// 					{
// 						pUser->SetCrimeStatus();
// 					}
// 				}
// 			}
// 			
// 		}
// 		if (!pRole->IsUser())
// 		{
// 			CMonster* pUserPet = NULL;
// 			pRole->QueryObj(OBJ_MONSTER, IPP_OF(pUserPet));	
// 			if(pUserPet && pUserPet->IsEudemon())
// 			{
// 				if( pTarget->IsUser())
// 				{
// 					
// 					if (pUserPet->IsEudemon() && pUserPet->QueryOwnerUser())
// 					{
// 						CUser* pTargetUser = NULL;
// 						pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser));
// 						//				if (!(pTargetUser->IsEvil()) && (pUserPet->QueryOwnerUser()->IsWhiteName()))
// 						if(!pTargetUser->IsEvil())
// 						{
// 							pUserPet->QueryOwnerUser()->SetCrimeStatus();
// 						}
// 					}
// 				}
// 			}
// 			
// 			else
// 			{	
// 				CMonster* pTargetPet = NULL;
// 				pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pTargetPet));
// 				
// 				if (pTargetPet && pUserPet->IsEudemon() && pTargetPet->IsEudemon())
// 				{
// 					//				if ((pUserPet->QueryOwnerUser()->IsWhiteName()) && (!pTargetPet->QueryOwnerUser()->IsEvil()) )
// 					if(!pTargetPet->QueryOwnerUser()->IsEvil())
// 					{
// 						pUserPet->QueryOwnerUser()->SetCrimeStatus();
// 					}
// 				}
// 				
// 			}
// 			
// 		}
// 	}
// 	if (pUser!=NULL&& !pUser->IsEvil())
// 	{
// // 		CSyndicate * thisSyn = this->GetSyndicate();
// // 		CSyndicate * userSyn = pUser->GetSyndicate();
// // 		if(!thisSyn || !userSyn || thisSyn->GetState() != 2 || userSyn->GetState() != 2 || thisSyn->GetWarSynID() != userSyn->GetID())
// 		SetCrimeStatus();	
// 		return true;
// // 		else
// // 			return false;
// 	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsImmunity(IRole* pRole)
{
	CHECKF(pRole);

	//20070316修罗:玩家攻击判定统一处理.交给IsAtkable()
	//大刀阔斧
	//有可能IsAtkable()在外层亦被掉用过一次.此处是第二次被掉用.
	return !IsAtkable(pRole);

	//修罗屏蔽以下所有
//	const bool	ATK_IT = false;
//	const bool	IMMUNITY = true;
//	OBJID idRole = pRole->GetID();
//	if(IsSceneID(idRole))		// is npc
//	{
//		CNpc* pNpc;
//		pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc));
//		if(pNpc && !pNpc->IsBeAtkable())
//			return true;
//		return false;
//	}
//	else if(IsNpcID(idRole))	// is monster
//	{
//		CMonster* pMonster = NULL;
//		CHECKF(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)));
//		if(pMonster->GetMasterID() == GetID())		// call pet of mine
//			return true;
//		if(this->GetPhyle() == PROF_DRAGON && strstr(pMonster->GetName(),"龙族") != 0)
//			return true;
//		else if(this->GetPhyle() == PROF_HUMAN && strstr(pMonster->GetName(),"天人族") != 0)
//			return true;
//		else if(this->GetPhyle() == PROF_GENE && strstr(pMonster->GetName(),"基因族") != 0)
//			return true;
//		switch(this->GetPkMode())
//		{
//		case PKMODE_FREE:
//			return ATK_IT;
//			break;
//		case PKMODE_TEAM:
//			if (pMonster->IsCallPet() || pMonster->IsEudemon())
//			{
//				CTeam* pTeam = GetTeam();
//				if(pTeam && pTeam->IsTeamMember(pMonster->GetMasterID()))
//					return IMMUNITY;
//
//				CUser* pUser = this->QueryOwnerUser();
//				if(this->IsMate(pUser))
//					return IMMUNITY;
//			}
//			else
//				return ATK_IT;
//			break;
//		case PKMODE_SYNDICATE:          //add huang 2004.1.15  todo.
//			if(pMonster->IsCallPet() || pMonster->IsEudemon())
//			{
//				CUser* pOwnerUser = pRole->QueryOwnerUser();
//				if (this->GetSynID() == pOwnerUser->GetSynID() || this->IsMate(pOwnerUser))
//					return IMMUNITY;
//				else
//				{				
//					CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetSynID());
//					if (pSyn)
//					{
//						if ( pSyn->IsFriendly(pOwnerUser->GetSynID())
//							    || pSyn->IsNearSyn(pOwnerUser->GetSynID()))
//
//							return IMMUNITY;
//					}
//				}
//			}
//			else if(pMonster->IsSynPet())
//			{
//				CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetSynID());
//				if (pSyn)
//				{
//					if ( pSyn->IsFriendly(pMonster->GetSynID())
//							|| pSyn->IsNearSyn(pMonster->GetSynID()))
//						
//						return IMMUNITY;
//				}	
//			}
//			
//			return ATK_IT;
//			break;
//		case PKMODE_ARRESTMENT:
//			if(pMonster->IsMonster() || pMonster->IsSynMonster() || pMonster->IsEvil())
//				return ATK_IT;
//			else
//				return IMMUNITY;
//			break;
//			
//		case PKMODE_SAFE:
//			if(pMonster->IsMonster() || pMonster->IsSynMonster())
//				return ATK_IT;
//			else
//				return IMMUNITY;
//			break;
//		}
//		return false;
//	}
//	else	// user
//	{
//	
//		switch(this->GetPkMode())
//		{
//		case	PKMODE_FREE:
//			return false;
//			break;
//			
//		case	PKMODE_TEAM:    //change huang 2004.1.11 (添加骑士团模式) 
//			{
//				bool bImmunity = false;
//				
//				CTeam* pTeam = this->GetTeam();
//				if(pTeam && pTeam->IsTeamMember(idRole))
//					bImmunity = true;
//				
//				if (this->GetFriend(pRole->GetID()))
//					bImmunity = true;
//				
//				CUser* pUser = NULL;
//				if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
//				{
//					if(this->IsMate(pUser))
//						bImmunity = true;
//				}
//				
//				return bImmunity;
//			}
//			break;
//		case	PKMODE_SYNDICATE:     //add huang 2004.1.15
//			{
//				bool bImmunity = false;
//				CUser* pUser = NULL;
//				if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)) && this->GetSynID() != ID_NONE)
//				{
//					if (this->GetSynID() == pUser->GetSynID())
//						bImmunity = true;
//					else
//					{
//						CSyndicate* pSyn = SynManager()->QuerySyndicate(this->GetSynID());
//						if (pSyn)
//						{
//							if (pSyn->IsNearSyn(pUser->GetSynID()) 
//								|| pSyn->IsFriendly(pUser->GetSynID()))
//								
//								bImmunity = true;
//						}
//					}
//					if (this->IsMate(pUser))
//						bImmunity = true;
//				}
//				return bImmunity;
//			}
//			break;
//		case	PKMODE_ARRESTMENT:
//			{
//				if(pRole->IsEvil())
//					return false;
//				return true;
//			}
//			break;
//
//		case	PKMODE_SAFE:
//			{	
//				CUser* pUser = NULL;
//				if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
//				{
//					if(this->GetPhyle() != pUser->GetPhyle())
//						return  false;
//				}
//			}
//			return true;
//			break;
//
//		default:
//			ASSERT(!"switch pk mode");
//			break;
//		}
//
//		return false;
//	}
}

//////////////////////////////////////////////////////////////////////
void CUser::SetCrimeStatus(int nStatus/*=STATUS_CRIME*/, int nTime/*=CRIME_KEEP_SECS*/)
{
	ASSERT(nStatus == STATUS_CRIME);

	int nPower = 1;		// 0: decrement secs
	CRole::AttachStatus(QueryRole(), nStatus, nPower, nTime);
	if(QueryCallPet())
		CRole::AttachStatus(QueryCallPet()->QueryRole(), nStatus, nPower, nTime);

}
//////////////////////////////////////////////////////////////////////
void CUser::SetSelfDefStatus(int nTime)      //add huang 2004.1.14
{
//	int nPower = 1;
//	CRole::AttachStatus(QueryRole(),STATUS_SELFDEFENCE,nPower,nTime);
}
//////////////////////////////////////////////////////////////////////
bool CUser::IsBeAtkable			()
{ 
	return !(QueryStatus(STATUS_PK_PROTECT)	//m_tLock.IsActive() && !m_tLock.IsTimeOut() 
		|| QueryStatus(STATUS_NEW_PK_PROTECT)	//  [2/22/2008 朱斌]	新的PK保护
		|| m_pCallPet&&m_pCallPet->IsInsteadMagic() 
		|| QueryStatus(STATUS_HIDDEN));
}


void CUser::DelCurNpc()
{
	CNpc* pNpc;
	IRole* pRole = RoleManager()->QuerySet()->GetObj(GetClickNpcID());
	if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && GetMapID() == pNpc->GetMapID())
	{
		pNpc->SetDelShow();
	}
}


//////////////////////////////////////////////////////////////////////
void CUser::ProcessMsgAction(int nAction, int nData, OBJID id)
{
	//王玉波注释，关于勋章..
/*	switch(nAction)
	{
	case actionQueryMedal:
		{
			CMsgDataArray	msg;
			IF_OK(msg.Create(_DATAARRAY_ACTION_MEDAL, strlen(this->GetMedal()), this->GetMedal()))
				SendMsg(&msg);
		}
		break;
	case actionDelMedal:
		{
			char* ptr = strchr(this->GetMedal(), nData);
			if(ptr)
			{
				memcpy(ptr, ptr+1, (ptr - this->GetMedal()));
			}
		}
		break;
	case actionQueryHonorTitle:
		{
			CMsgDataArray	msg;
			IF_OK(msg.Create(_DATAARRAY_ACTION_HONORTITLE, strlen(this->GetTitle()), this->GetTitle()))
				SendMsg(&msg);
		}
		break;
	case actionDelHonorTitle:
		{
			char* ptr = strchr(this->GetTitle(), nData);
			if(ptr)
			{
				memcpy(ptr, ptr+1, (ptr - this->GetTitle()));
			}
		}
		break;
	case actionSelectHonorTitle:
		{
		}
		break;
	default:
		ASSERT(!"CUser::ProcessMsgAction");
	}*/
}
/*------------------------------------begin------------------------------------------------*/
//////////////////////////////////////////////////////////////////////
// lock key
//////////////////////////////////////////////////////////////////////
/*
bool CUser::SetLock(DWORD dwKey)
{
	// check key
	if(m_data.GetRealLockKey())
		return false;
	if(dwKey <= 99999)		// || dwKey >= 1000000000
		return false;
	if(dwKey == 123456 || dwKey == 654321)
		return false;
	if(dwKey == 111111 || dwKey == 222222 || dwKey == 333333 || dwKey == 666666 || dwKey == 888888 || dwKey == 999999)
		return false;

	m_data.SetLockKey(dwKey);
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::OpenLock(DWORD dwKey)
{
	if(!m_data.GetRealLockKey() || m_data.GetRealLockKey() != dwKey)
		return false;

	m_data.SetCurrLockKey(0, UPDATE_FALSE);
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::ClearLock(DWORD dwKey)
{
	if(m_data.GetRealLockKey() && m_data.GetRealLockKey() != dwKey)
		return false;

	m_data.SetLockKey(0, UPDATE_FALSE);
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CheckLock(bool bSynchro)
{
	if(m_data.GetCurrLockKey())
	{
		if(bSynchro)
			SendSysMsg(STR_LOCKKEY_DENY);
		return false;
	}

	return true;
}*/
/*-------------------------------王玉波注释---------end,功能未知-------------------------------------*/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
bool CUser::IsItemFull(int nWeight, OBJID idItemType, DWORD dwData)
{
	// 不考虑负重

	return m_pPackage->IsPackFull(dwData,idItemType);
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsItemFull(ItemInfoStruct* pInfo)
{
	IF_NOT (pInfo)
		return true;
	if (!m_pPackage->IsPackFull(1,pInfo->idType))
		return false;

	if (CItem::IsCountable(pInfo->idType))
	{
		if (/*!CItem::IsNeedIdent(pInfo->ucIdent) && */pInfo->nAmount < pInfo->nAmountLimit)
		{
			CItem* pItem = FindCombineItem(pInfo->idType);
			if(pItem)
			{
				if(pItem->GetInt(ITEMDATA_AMOUNT) + pInfo->nAmount <= pInfo->nAmountLimit)	// 可以合并
					return false;
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// tutor 
// void CUser::SetTutor(TutorInfoStruct * data)
// {
// 	m_pTutor = CTutor::CreateNewTutor(data);
// }	

// int CUser::GetDensityWithTutor()
// {
// 	if(!m_pTutor)
// 		return -1;
// 	else
// 		return m_pTutor->GetDensity();
// }

// void CUser::RequestAddDensity(int nDensity)
// {
// 	if(!m_pTutor)
// 		return ;
// 
// 	ChgTutorInfo info;
// 	memset(&info, 0, sizeof(ChgTutorInfo));
// 	info.nAction = CHGTUTORDATA_REQADDDENSITY;
// 	info.idUser  = GetID();
// 	info.idTutor = m_pTutor->GetTutorID();
// 	info.idStudent = m_pTutor->GetUserID();
// 	info.id      = m_pTutor->GetID();
// 	
// 	MapGroup(PID)->QueryIntraMsg()->SendTutorToWorld(&info);
// }
//通过徒弟增加传授度
// void CUser::AddImpartByStudent(int nImpart)
// {
// 	if(!m_pTutor)
// 		return ;
// 	else
// 	{
// 		m_pTutor->AddImpart( nImpart, Database(), true);
// 
// // 		ST_MEMBERINFO info;
// // 		memset(&info, 0, sizeof(ST_MEMBERINFO));
// // 		
// // 		info.idMember = m_pTutor->GetTutorID();
// // 		info.idMember = m_pTutor->GetUserID();
// // 		info.nImpart = m_pTutor->GetImpart();
// 		
// 		CMsgSchoolMember msg;
// 		if (msg.Create(GetID(), MESSAGEBOARD_ADDDENSITY, NULL, 0))
// 		{
// 			msg.Append(m_pTutor->GetTutorID(), RELATION_TEACHER, m_pTutor->GetImpart(), m_pTutor->GetTutorName(), m_pTutor->GetTutorLev());
// 			this->SendMsg(&msg);
// 		}
// 
// 		if (!msg.Create(m_pTutor->GetTutorID(), MESSAGEBOARD_ADDDENSITY, NULL, 0))
// 			return;
// 		msg.Append(GetID(), RELATION_STUDENT, m_pTutor->GetImpart(), GetName(), GetLev());
// 
// 		CUser *pTutor = UserManager()->GetUser(m_pTutor->GetTutorID());
// 		if (pTutor)
// 			pTutor->SendMsg(&msg);
// 		else
// 			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pTutor->GetTutorID());
// 	}
// }
//通过师父增加传授度
// void CUser::AddImpartByTutor(OBJID idStudent, int nImpart)
// {
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == this->GetID())
// 		{
// 			pTutor->AddImpart(nImpart, Database(), true);
// // 			ST_MEMBERINFO info;
// // 			memset(&info, 0, sizeof(ST_MEMBERINFO));
// // 			
// // 			info.idTutor = pTutor->GetTutorID();
// // 			info.idMember = pTutor->GetUserID();
// // 			info.nDensity = nDensity;
// 			
// 			CMsgSchoolMember msg;
// 			if(msg.Create(GetID(), MESSAGEBOARD_ADDDENSITY, NULL, 0))
// 			{
// 				msg.Append(pTutor->GetUserID(), RELATION_STUDENT, pTutor->GetImpart(), pTutor->GetUserName(), pTutor->GetStuLev());
// 				this->SendMsg(&msg);
// 			}
// 			if (!msg.Create(pTutor->GetUserID(), MESSAGEBOARD_ADDDENSITY, NULL, 0))
// 				return;
// 			msg.Append(GetID(), RELATION_TEACHER, pTutor->GetImpart(), GetName(), GetLev());
// 			CUser* pStudent = UserManager()->GetUser(pTutor->GetUserID());
// 			if (pStudent)
// 				pStudent->SendMsg(&msg);
// 			else
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pTutor->GetUserID());
// 			break;
// 		}
// 	}
// }
//徒弟查询传授度
// int	CUser::GetImpartByStudent()
// {
// // 	if (!m_pTutor)
// // 		return 0;
// // 	return m_pTutor->GetImpart();
// 	CHECKF(m_pUserTutor);
// 	return m_pUserTutor->GetImpartByStudent();
// }
//师父查询传授度
// int	CUser::GetImpartByTutor(OBJID idStudent)
// {
// // 	CTutor* pStudent = GetStudent(idStudent);
// // 	if (!pStudent)
// // 		return 0;
// // 	return pStudent->GetImpart();
// 	CHECKF(m_pUserTutor);
// 	return m_pUserTutor->GetImpartByTutor(idStudent);
// }
// int CUser::GetTutorTeamAddExp(int nExp)
// {
// 	CHECKF(m_pTeam);
// 	if(NULL == m_pTeam || 1 == m_pTeam->GetMemberAmount())
// 		return 0;
// 
// 	if(NULL == m_pTutor && m_setStudent.empty())
// 		return 0;
// 
// 	int nTutorStudentNum = 0;
// 
// 	for(int i = 0; i < m_pTeam->GetMemberAmount(); i++)
// 	{
// 		OBJID idMember = m_pTeam->GetMemberByIndex(i);
// 		if(idMember == this->GetID())
// 			continue;
// 	
// 		if(NULL != m_pTutor)     //有师傅
// 		{
// 			if(m_pTutor->GetTutorID() == idMember)
// 				nTutorStudentNum++;
// 		}
// 		
// 	    if(!m_setStudent.empty())  //有徒弟
// 		{
// 			TUTOR_SET::iterator it = m_setStudent.begin();
// 			for (; it != m_setStudent.end(); it++)
// 			{
// 				CTutor* pTutor = *it;
// 				if (pTutor->GetUserID() == idMember && pTutor->GetTutorID() == GetID())
// 				{
// 					nTutorStudentNum++;
// 				}
// 			}
// 		}
// 	}
// 
// 	if(0 != nTutorStudentNum)
// 		nTutorStudentNum++;
// 
// 	return nExp*TUTOR_TEAM_ADD_EXP_PERSENT*nTutorStudentNum/100;
// }
//goto 拜师 徒弟调用
// bool CUser::AddTutor(OBJID idTutor, LPCTSTR szName, int nPro)
// {
// 	CHECKF(szName);
// 	//这些判断都放到脚本中去
// // 	if (m_pTutor && !m_pTutor->IsGraduated())
// // 	{
// // 		SendSysMsg(_TXTATR_TOPSHOW, "你当前已经有师父了，不能再拜他人为师");
// // 		return false;
// // 	}
// 	CUser* pTarget = UserManager()->GetUser(idTutor);
// 	if (!pTarget)//拜师的时候师父一定在线
// 		return false;
// // 	if (pTarget->GetTutor() && !pTarget->GetTutor()->IsGraduated())
// // 	{
// // 		SendSysMsg(_TXTATR_TOPSHOW, "对方还未出师，不能收徒");
// // 		return false;
// // 	}
// // 	if(pTarget->IsStudentFull())
// // 	{
// // 		SendSysMsg(_TXTATR_TOPSHOW, "对方的徒弟数量已达到上限");
// // 		pTarget->SendSysMsg(_TXTATR_TOPSHOW, "你的徒弟数量已经达到上限");
// // 		return false;
// // 	}
// 	m_pTutor	= CTutor::CreateNew();
// 	if (m_pTutor)
// 	{
// 		TutorInfoStruct info;
// 		memset(&info, 0, sizeof(TutorInfoStruct));
// 		info.idUser = GetID();
// 		info.idTutor = idTutor;
// 		info.nStuLev = GetLev();
// 		info.nTutorLev = pTarget->GetLev();
// 		info.nTutorImpartLv = pTarget->GetTutorLevel();
// 		strcpy(info.szUserName, this->GetName());
// 		strcpy(info.szTutorName, szName);
// // 		info.nStudentPro = GetProfession();
// // 		info.nTutorPro = nPro;
// 		if (m_pTutor->CreateRecord(&info, Database()))
// 		{
// 			if (IsFirstTutor())
// 				m_data.SetTutorMask(1);//设置是否第一次拜师标识
// 			//通知双方
// 			//CMsgSchoolMember msg;
// 			//if (msg.Create(GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0))//通知徒弟添加师父
// 			{
// 				//msg.Append(RELATION_TEACHER, 0, pTarget);
// 				//SendMsg(&msg);
// 				char szTemp[128];
// 				sprintf(szTemp,"恭喜你拜%s为师",szName);
// 				SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 			}
// 			//if (msg.Create(idTutor, MESSAGEBOARD_ADDMEMBER, NULL, 0))//通知师父添加徒弟
// 			{
// 				//msg.Append(RELATION_STUDENT, 0, this);
// 				//pTarget->SendMsg(&msg);
// 				char szTemp[128];
// 				sprintf(szTemp,"恭喜你收%s为徒",GetName());
// 				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 			}
// 			//if(!pTarget->IsStudentFull())
// 			pTarget->AddStudentRef(m_pTutor->GetInfo());
// // 			else
// // 			{
// // 				// TODO: 通过世界核心在其它MapGroup中寻找并通知
// // 				CMsgSchoolMember msg;
// // 				if (msg.Create(GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0))
// // 				{
// // 					if (msg.Append(RELATION_TEACHER, MEMBERSTATUS_OFFLINE, idTutor, szName))
// // 					{
// // 						SendMsg(&msg);
// // 						MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, this->GetSocketID(), this->GetID());
// // 					}
// // 				}
// // 			}
// 			return true;
// 		}
// 		else
// 			SAFE_DELETE (m_pTutor);
// 	}
// 	return false;
// }
//徒弟单方解除师徒关系，只能徒弟调用
// bool CUser::DelTutor()
// {
// 	if (!m_pTutor)
// 		return false;
// 	if (m_pTutor->IsGraduated())
// 		return false;
// 	char szTemp[128];
// 	sprintf(szTemp, "已经成功与%s解除师徒关系", m_pTutor->GetTutorName());
// 	OBJID idTutor = m_pTutor->GetTutorID();
// 
// 	m_pTutor->DeleteRecord(Database());
// 	SAFE_DELETE (m_pTutor);
// 	
// // 	CMsgSchoolMember	msg;
// // 	if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))
// // 	{
// // 		if (msg.Append(idTutor, RELATION_TEACHER, m_pTutor->GetImpart(), m_pTutor->GetTutorName(), m_pTutor->GetTutorLev()))
// // 			SendMsg(&msg);
// // 	}
// 	
// 	// 这里只是删除了师傅的数据库记录，还应该通知师傅删除内存数据——如果师傅在线的话
// 	CUser* pUser = UserManager()->GetUser(idTutor);
// 	if (pUser)
// 	{
// 		pUser->DelStudentRef(GetID());//同时给师父发消息
// 	}
// 	else
// 	{
// // 		// 通知其它MapGroup寻找师傅并删除徒弟
// // 		if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))
// // 		{
// // 			if (msg.Append(GetID(), RELATION_STUDENT, m_pTutor->GetImpart(), GetName(), GetLev()))
// // 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idTutor);
// // 		}
// 	}
// 	SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 	return true;
// }

bool CUser::ProcessTutorRecover()
{
// 	if(!m_pTutor)
// 		return false;
// //	m_pTutor->SetApplyType(0);
// 
// 	CTime time(CTime::GetCurrentTime());
// 	CString ss = time.Format("%Y-%m-%d %H:%M:%S");
// 	char szTime[32];
// 	strcpy(szTime, ss);
// 
// //	m_pTutor->SetDelTime(szTime);
// 
// 	ST_MEMBERINFO info;
// 	memset(&info, 0, sizeof(ST_MEMBERINFO));
// 
// 	info.idTutor = m_pTutor->GetTutorID();
// 	info.idMember = m_pTutor->GetUserID();
// 	info.uApplyType = 0;
// 	strcpy(info.delTime, szTime);
// 
// 	CMsgSchoolMember msg;
// 	if(msg.Create(MESSAGEBOARD_RECOVER, &info, 1))
// 	{
// 		this->SendMsg(&msg);
// 	}
// 
// 	char szTemp[256];
// 	sprintf(szTemp, g_objStrRes.GetStr(28311), m_pTutor->GetUserName(), m_pTutor->GetTutorName());
// 	ProcessActionUser(126, szTemp, 0);
// 	this->SendSysMsg(szTemp); //%s与%s的师徒关系恢复

	return true;
}

bool CUser::ProcessTutorDelReq(int nApplyType)
{
// 	if(!m_pTutor)
// 		return false;
// //	m_pTutor->SetApplyType(nApplyType);
// 
// 	CTime time(CTime::GetCurrentTime());
// 	CString ss = time.Format("%Y-%m-%d %H:%M:%S");
// 	char szTime[32];
// 	strcpy(szTime, ss);
// 
// //	m_pTutor->SetDelTime(szTime);
// 
// 	ST_MEMBERINFO info;
// 	memset(&info, 0, sizeof(ST_MEMBERINFO));
// 
// 	info.idTutor = m_pTutor->GetTutorID();
// 	info.idMember = m_pTutor->GetUserID();
// 	info.uApplyType = nApplyType;
// 	strcpy(info.delTime, szTime);
// 
// 	CMsgSchoolMember msg;
// 	if(msg.Create(MESSAGEBOARD_APPLYDEL, &info, 1))
// 	{
// 		this->SendMsg(&msg);
// 	}
// 
// 	if(APPLYDEL_STUDENT == nApplyType)
// 	{
// 		ProcessActionUser(126, g_objStrRes.GetStr(16087), 0);
// 		SendSysMsg(g_objStrRes.GetStr(16087));//"您的师傅向您提出师徒关系解除的申请！"
// 	}

	return true;
}

// bool CUser::RequestDelTutor()
// {
// 	if (!m_pTutor)
// 		return false;
// 	
// 	ChgTutorInfo info;
// 	memset(&info, 0, sizeof(ChgTutorInfo));
// 	info.nAction = CHGTUTORDATA_REQDELTUTOR;
// 	info.idUser  = GetID();
// 	info.idTutor = m_pTutor->GetTutorID();
// 	info.idStudent = m_pTutor->GetUserID();
// 	info.id      = m_pTutor->GetID();
// 	MapGroup(PID)->QueryIntraMsg()->SendTutorToWorld(&info);
// 
// 	return true;
// }

// bool CUser::DelTutorRef()
// {
// 	if (!m_pTutor)
// 		return false;
// 
// 	//SendSysMsg(g_objStrRes.GetStr( 13085), m_pTutor->GetTutorName());//STR_LEAVE_TEACHER4
// 
// 	OBJID idTutor = m_pTutor->GetTutorID();
// 	int nImpart = m_pTutor->GetImpart();
// 	const char *szName = m_pTutor->GetTutorName();
// 	SAFE_DELETE (m_pTutor);
// 
// // 	CMsgSchoolMember	msg;
// // 	if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))
// // 	{
// // 		if (msg.Append(idTutor, RELATION_TEACHER, nImpart, szName, m_pTutor->GetTutorLev()))
// // 			SendMsg(&msg);
// // 	}
// 
// 	return true;
// }

// CTutor* CUser::GetStudent(OBJID idStudent)
// {
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent)
// 			return pTutor;
// 	}
// 	return NULL;
// }

// bool CUser::AddStudentRef(const TutorInfoStruct* pInfo)
// {
// 	CHECKF (pInfo);
// 
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == pInfo->idUser && pTutor->GetTutorID() == this->GetID())
// 			return false;
// 	}
// 
// 	CTutor* pTutor = CTutor::CreateNewTutor(pInfo);
// 	if (pTutor)
// 	{
// 		m_setStudent.push_back(pTutor);
// 		CMsgSchoolMember	msg;
// 		if (msg.Create(GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0))
// 		{
// 			CUser* pUser = UserManager()->GetUser(pTutor->GetUserID());
// 			if (pUser)
// 			{
// 				if (msg.Append(RELATION_STUDENT, 0, pUser))
// 					SendMsg(&msg);
// 			}
// 			else
// 			{
// 				if (msg.Append(pTutor->GetUserID(), RELATION_STUDENT, pTutor->GetImpart(), pTutor->GetUserName(), pTutor->GetStuLev()))
// 				{
// 					SendMsg(&msg);
// 					MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, this->GetSocketID(), this->GetID());
// 				}
// 			}
// 		}
// 		return true;
// 	}
// 	return false;
// }
DWORD   CUser::GetAllTutorLevel()
{//为了在新手工会中排序公告而查找所有学员的等级和。LW
/*	DWORD dLevel = 0;
	TUTOR_SET::iterator it = m_setStudent.begin();
	for (; it != m_setStudent.end(); it++)
	{
		CTutor* pTutor = *it;
		OBJID idStudent = pTutor->GetUserID(); 
		dLevel += UserManager()->GetUser(idStudent)->GetLev();
	}*/
	return 0; 
	
}

bool CUser::IsStudentFull()
{
	int nMaxStudenet = CUserTutor::GetMaxStuAmount(GetImpart());

	return (GetStudentAmount(false) >= nMaxStudenet); 
}
//师父收徒时计算徒弟的初始奖励掩码，将徒弟拜师之前的等级奖励掩码设为不可领
DWORD CUser::CalcTutorGiftMask(int nLev)
{
	//掩码的第一位是15级的奖励,第二位是20级...以此类推
	if(nLev<10)
		return 0;
	int nIndex = (nLev-10)/5;
	DWORD dwRet = 0;
	for (int i=0; i<nIndex; i++)
	{
		dwRet |= (1<<i);
	}
	return dwRet;
}
//师徒传道任务 搜集魂
bool CUser::AddTutorPreach(OBJID idItem, int nAmount)
{
	CHECKF(idItem);
	if (nAmount<0)
		return false;
	
	CItem* pItem = GetItemByType(idItem);
	if (!pItem)
		return false;

	if (nAmount==0)
		return pItem->GetInt(ITEMDATA_BASE1) >= pItem->GetInt(ITEMDATA_BASE2);
	if (pItem->GetInt(ITEMDATA_BASE1) + nAmount >= pItem->GetInt(ITEMDATA_BASE2))//满了
	{
		pItem->SetInt(ITEMDATA_BASE1, pItem->GetInt(ITEMDATA_BASE2));
		CMsgItemInfo msg;
		if (msg.Create(pItem,ITEMACT_VIEWCHATITEMINFO))
		{
			SendMsg(&msg);
		}
		return true;
	}
	else
	{
		pItem->SetInt(ITEMDATA_BASE1, pItem->GetInt(ITEMDATA_BASE1)+nAmount);
		CMsgItemInfo msg;
		if (msg.Create(pItem,ITEMACT_VIEWCHATITEMINFO))
		{
			SendMsg(&msg);
		}
		return false;
	}
}
//获取队伍中指定等级范围的徒弟数量
int	CUser::GetTeamStuInLev(int nMaxLev, int nMinLev, int nLowerThan/*=0*/)
{
	if (!GetTeam())
		return 0;
	if (m_pTeam->GetLeader()!=GetID())
		return 0;

	int nAmount = 0;
	int nTeamAmount = m_pTeam->GetMemberAmount();
	CTutorData* pData = NULL;
	for(int i = 0; i < nTeamAmount; i++)
	{
		OBJID idMember = m_pTeam->GetMemberByIndex(i);
		CUser* pTempUser = NULL;
		FindAroundObj( OBJ_USER,idMember,( void** )&pTempUser );
		if(pTempUser && pTempUser->IsAlive() && (pTempUser->GetID() != GetID()))
		{
			pData = GetStudent(pTempUser->GetID());
			if (pData)//是徒弟
			{
				if(pData->GetState()==TUTORSTATE_NORMAL ||  pData->GetState()==TUTORSTATE_AGREED)
				{
					if (pTempUser->GetLev() >= nMinLev && pTempUser->GetLev() <= nMaxLev && GetLev()-pTempUser->GetLev()>=nLowerThan)
						nAmount++;
				}
			}
		}
 	}
	return nAmount;
}
//队伍中是否有相同IP的徒弟
bool CUser::HasSameIPStuInTeam()
{
#if defined(_DEBUG) || defined(_ASHRELEASE)
	return false;
#endif

	if (!GetTeam())
		return false;
	CHECKF(m_pTeam->GetLeader()==GetID());
	
	int nTeamAmount = m_pTeam->GetMemberAmount();
	CTutorData* pData = NULL;
	for(int i = 0; i < nTeamAmount; i++)
	{
		OBJID idMember = m_pTeam->GetMemberByIndex(i);
		CUser* pTempUser = NULL;
		FindAroundObj( OBJ_USER,idMember,( void** )&pTempUser );
		if(pTempUser && pTempUser->IsAlive() && (pTempUser->GetID() != GetID()))
		{
			pData = GetStudent(pTempUser->GetID());
			if (pData)//是徒弟
			{
				if(pData->GetState()==TUTORSTATE_NORMAL ||  pData->GetState()==TUTORSTATE_AGREED)
				{
					if(GetIPAddress()==pTempUser->GetIPAddress())
						return true;
				}
			}
		}
	}
	return false;
}
bool CUser::CheckItemCreater(OBJID idItem)
{
	CHECKF(idItem);
	CItem* pItem = GetItem(idItem, false, false);
	if (!pItem)
		return false;
	
	if (strcmp(pItem->GetStr(ITEMDATA_MAKERNAME), GetName())==0)
		return true;
	else
		return false;
}
bool CUser::SpendMorAlity(int nData)
{
	if (nData<=0)
		return true;

	if(GetMorality()<nData)
		return false;
	else
		SetMorality(GetMorality()-nData);

	return true;
}
int	CUser::GetTeamStuAvgLev()//队伍中徒弟的平均等级
{
	if (!GetTeam())
		return 0;
	CHECKF(m_pTeam->GetLeader()==GetID());
	
	int nTotalLev = 0, nStuAmount = 0;
	int nTeamAmount = m_pTeam->GetMemberAmount();
	CTutorData* pData = NULL;
	for(int i = 0; i < nTeamAmount; i++)
	{
		OBJID idMember = m_pTeam->GetMemberByIndex(i);
		CUser* pTempUser = UserManager()->GetUser(idMember);
		if(pTempUser && (pTempUser->GetID() != GetID()))
		{
			pData = GetStudent(pTempUser->GetID());
			if (pData)//是徒弟
			{
				if(pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_AGREED)
				{
					nTotalLev += pTempUser->GetLev();
					nStuAmount++;
				}
			}
		}
	}
	if(nStuAmount>0)
		return nTotalLev / nStuAmount + 0.5;
	else
		return 0;
}
int	CUser::GetStuLevAddedInTeam()//队伍中的徒弟在师父门下升过的级数
{
	if(!GetTeam())
		return 0;

	if (m_pTeam->GetMemberAmount()>2)//只限两人队伍
		return 0;

	OBJID idTarget = m_pTeam->GetTeamMemberOnly();
	
	CTutorData* pData = GetStudent(idTarget);
	if(!pData)
		return 0;

	return pData->GetInt(TUTORDATA_ADDEDLEV);
}
//goto type:0 未出师徒弟，1 已出师徒弟，2 全部
// int CUser::GetStudentAmount(short nType)
// {
// 	int nGraduated = 0;
// 	int nUnGraduated = 0;
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->IsGraduated())
// 			nGraduated++;
// 		else
// 			nUnGraduated++;
// 	}
// 	if (nType==0) return nUnGraduated;
// 	else if (nType==1) return nGraduated;
// 	else if (nType==2) return nGraduated+nUnGraduated;
// 	return 0;
// }
bool CUser::ProcessStudentRecover(OBJID idStudent)
{
// 	if(m_setStudent.empty())
// 		return false;
// 	
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
// 		{
// //			pTutor->SetApplyType(0);
// 			
// 			CTime time(CTime::GetCurrentTime());
// 			CString ss = time.Format("%Y-%m-%d %H:%M:%S");
// 			char szTime[32];
// 			strcpy(szTime, ss);
// 			
// //			pTutor->SetDelTime(szTime);
// 
// 			ST_MEMBERINFO info;
// 			memset(&info, 0, sizeof(ST_MEMBERINFO));
// 			
// 			info.idTutor = pTutor->GetTutorID();
// 			info.idMember = pTutor->GetUserID();
// 			info.uApplyType = 0;
// 			strcpy(info.delTime, szTime);
// 			
// 			CMsgSchoolMember msg;
// 			if(msg.Create(MESSAGEBOARD_RECOVER, &info, 1))
// 			{
// 				this->SendMsg(&msg);
// 			}
// 
// 			char szTemp[256];
//         	sprintf(szTemp, g_objStrRes.GetStr(28311), m_pTutor->GetUserName(), m_pTutor->GetTutorName());
// 		    ProcessActionUser(126, szTemp, 0);
// 			this->SendSysMsg(szTemp);
// 			
// 			return true;
// 		}
// 	}
	
	return true;
}

bool CUser::ProcessStudentDelReq(OBJID idStudent, int nApplyType)
{
// 	if(m_setStudent.empty())
// 		return false;
// 	
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
// 		{
// //			pTutor->SetApplyType(nApplyType);
// 			
// 			CTime time(CTime::GetCurrentTime());
// 			CString ss = time.Format("%Y-%m-%d %H:%M:%S");
// 			char szTime[32];
// 			strcpy(szTime, ss);
// 			
// //			pTutor->SetDelTime(szTime);
// 
// 			ST_MEMBERINFO info;
// 			memset(&info, 0, sizeof(ST_MEMBERINFO));
// 			
// 			info.idTutor = pTutor->GetTutorID();
// 			info.idMember = pTutor->GetUserID();
// 			info.uApplyType = nApplyType;
// 			strcpy(info.delTime, szTime);
// 			
// 			CMsgSchoolMember msg;
// 			if(msg.Create(MESSAGEBOARD_APPLYDEL, &info, 1))
// 			{
// 				this->SendMsg(&msg);
// 			}
// 			
// 			if(APPLYDEL_TUTOR == nApplyType)
// 			{
// 				ProcessActionUser(126, g_objStrRes.GetStr(16088), 0);
// 				SendSysMsg(g_objStrRes.GetStr(16088));//"您的徒弟向您提出师徒关系解除的申请！"
//             }
// 			return true;
// 		}
// 	}
	
	return true;
}

// bool CUser::RequestDelStudent(OBJID idStudent)
// {
// 	if(m_setStudent.empty())
// 		return false;
// 
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
// 		{
// 			ChgTutorInfo info;
// 			memset(&info, 0, sizeof(ChgTutorInfo));
// 			info.nAction = CHGTUTORDATA_REQDELSTUDENT;
// 			info.idUser  = GetID();
// 			info.idTutor = pTutor->GetTutorID();
// 			info.idStudent = pTutor->GetUserID();
// 			info.id      = pTutor->GetID();
// 			MapGroup(PID)->QueryIntraMsg()->SendTutorToWorld(&info);
// 			
// 			break;
// 		}
// 	}
// 
// 	return true;
// }

// bool CUser::RecoverStudent(OBJID idStudent)
// {
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
// 		{
// // 			if(0 == pTutor->GetDelApplyType())
// // 			{
// // 				this->SendSysMsg(g_objStrRes.GetStr(28312)); //无需恢复
// // 				return false;
// // 			}
// 
// 			ChgTutorInfo info;
// 			memset(&info, 0, sizeof(ChgTutorInfo));
// 			info.nAction = CHGTUTORDATA_RECOVERSTUDENT;
// 			info.idUser  = GetID();
// 			info.idTutor = pTutor->GetTutorID();
// 			info.idStudent = pTutor->GetUserID();
// 			info.id      = pTutor->GetID();
// 			MapGroup(PID)->QueryIntraMsg()->SendTutorToWorld(&info);
// 			break;
// 		}
// 	}
// 
// 	return true;
// }

//协议解除师徒关系，组队完成，只能师父调用
// bool CUser::DelStudent(OBJID idStudent)
// {
// 	if (!IsTutorTeam())
// 		return false;
// 	CUser* pStudent = UserManager()->GetUser(idStudent);
// 	if (!pStudent)
// 		return false;
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
// 		{
// 			pTutor->DeleteRecord(Database());
// 			OBJID idStudent = pTutor->GetUserID();
// 			SAFE_DELETE (pTutor);
// 			m_setStudent.erase(it);
// 			
// // 			CMsgSchoolMember	msg;
// // 			if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))//通知师父
// // 			{
// // 				if (msg.Append(RELATION_STUDENT, 0, pStudent))
// // 					SendMsg(&msg);
// // 			}
// 
// 			if (pStudent->DelTutorRef())//同时给徒弟发消息
// 				return true;
// 		}
// 	}
// 	
// 	return false;
// }
//师父单方解除师徒关系，只能师父调用
// bool CUser::DelStudentByTutor(OBJID idStudent)
// {
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
// 		{
// 			pTutor->DeleteRecord(Database());
// 			OBJID idStudent = pTutor->GetUserID();
// 			int nImpart = pTutor->GetImpart();
// 			const char* szName = pTutor->GetUserName();
// 			char szTemp[128];
// 			sprintf(szTemp, "你已经成功将%s逐出师门", szName);
// 			SAFE_DELETE (pTutor);
// 			m_setStudent.erase(it);
// 			
// // 			CMsgSchoolMember	msg;
// // 			if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))//通知师父
// // 			{
// // 				if (msg.Append(idStudent, RELATION_STUDENT, nImpart, szName, pTutor->GetStuLev()))
// // 					SendMsg(&msg);
// // 			}
// 			CUser* pStudent = UserManager()->GetUser(idStudent);
// 			if (pStudent)
// 			{
// 				pStudent->DelTutorRef();//同时给徒弟发消息
// 			}
// 			else
// 			{
// 				// 通知其它MapGroup寻找徒弟并删除师傅
// // 				if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))
// // 				{
// // 					if (msg.Append(GetID(), RELATION_TEACHER, nImpart, GetName(), GetLev()))
// // 						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idStudent);
// // 				}
// 			}
// 			SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 			return true;
// 		}
// 	}
// 	
// 	return false;
// }
// bool CUser::DelStudentRef(OBJID idStudent)
// {
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
// 		{
// 			//this->SendSysMsg(g_objStrRes.GetStr( 13084), pTutor->GetUserName());//STR_LEAVE_TEACHER3
// 			int nImpart = pTutor->GetImpart();
// 			const char *szName = pTutor->GetUserName();
// 			SAFE_DELETE (pTutor);
// 			m_setStudent.erase(it);
// 
// // 			CMsgSchoolMember	msg;
// // 			if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))
// // 			{
// // 				if (msg.Append(idStudent, RELATION_STUDENT, nImpart,  szName, pTutor->GetStuLev()))
// // 					SendMsg(&msg);
// // 			}
// 
// 			return true;
// 		}
// 	}
// 	return false;
// }
// DWORD CUser::GetTutorMask(OBJID idStudent)//获取师父奖励掩码
// {
// 	if (!m_pUserTutor)
// 		return 0;
// 	return m_pUserTutor->GetTutorGiftMask(idStudent);
// }
// void CUser::SetTutorMask(OBJID idStudent, DWORD dwMask)//设置师父奖励掩码
// {
// 	if (!m_pUserTutor)
// 		return;
// 	m_pUserTutor->SetTutorGiftMask(idStudent, dwMask);
// }
// bool CUser::CancelLeaveTeach()
// {
// 	if(NULL == m_pTutor)
// 		return false;
// 
// // 	if(0 == m_pTutor->GetDelApplyType())
// // 	{
// // 		this->SendSysMsg(g_objStrRes.GetStr(28312)); //无需恢复
// // 		return false;
// // 	}
// 
// 	ChgTutorInfo info;
// 	memset(&info, 0, sizeof(ChgTutorInfo));
// 	info.nAction = CHGTUTORDATA_CANCELLEAVETUTOR;
// 	info.idUser  = GetID();
// 	info.idTutor = m_pTutor->GetTutorID();
// 	info.idStudent = m_pTutor->GetUserID();
// 	info.id      = m_pTutor->GetID();
// 	MapGroup(PID)->QueryIntraMsg()->SendTutorToWorld(&info);
// 
// 	return true;
// }

// bool CUser::CancelDelStudent(OBJID idStudent)
// {
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor->GetUserID() == idStudent && pTutor->GetTutorID() == GetID())
// 		{
// // 			if(0 == pTutor->GetDelApplyType())
// // 			{
// // 				this->SendSysMsg(g_objStrRes.GetStr(28312)); //无需恢复
// // 				return false;
// // 			}
// 
// 			ChgTutorInfo info;
// 			memset(&info, 0, sizeof(ChgTutorInfo));
// 			info.nAction = CHGTUTORDATA_CANCELDELSTUDENT;
// 			info.idUser  = GetID();
// 			info.idTutor = pTutor->GetTutorID();
// 			info.idStudent = pTutor->GetUserID();
// 			info.id      = pTutor->GetID();
// 			MapGroup(PID)->QueryIntraMsg()->SendTutorToWorld(&info);
// 			break;
// 		}
// 	}
// 
// 	return true;
// }

bool CUser::RevQesCelDelStudent()
{
// 	if(NULL == m_pTutor)
// 		return false;
// 
// 	CMsgAction msg;
// 	if(msg.Create(m_pTutor->GetTutorID(), 0, 0, 0, actionCancelDisStudent))
// 		this->SendMsg(&msg);
	return true;
}

bool CUser::RevQesCelLevTeach(OBJID idStudent)
{
// 	CMsgAction msg;
// 	if(msg.Create(idStudent, 0, 0, 0, actionCancelLevTeacher))
// 		this->SendMsg(&msg);
	return true;
}

// bool CUser::RecoverTutor()
// {
// 	if(NULL == m_pTutor)
// 		return false;
// 
// // 	if(0 == m_pTutor->GetDelApplyType())
// // 	{
// // 		this->SendSysMsg(g_objStrRes.GetStr(28312)); //无需恢复
// // 		return false;
// // 	}
// 
// 	ChgTutorInfo info;
// 	memset(&info, 0, sizeof(ChgTutorInfo));
// 	info.nAction = CHGTUTORDATA_RECOVERTUTOR;
// 	info.idUser  = GetID();
// 	info.idTutor = m_pTutor->GetTutorID();
// 	info.idStudent = m_pTutor->GetUserID();
// 	info.id      = m_pTutor->GetID();
// 	MapGroup(PID)->QueryIntraMsg()->SendTutorToWorld(&info);
// 
// 	return true;
// }

void CUser::TutorLoginHint()
{
	if(m_pUserTutor)
	{
		m_pUserTutor->SendLeaveMsg();
	}
}
void CUser::TutorLogoutHint()
{
	if(m_pUserTutor)
	{
		m_pUserTutor->SendTutorOffLineInfo();
	}
}
// bool CUser::CreateAllTutor(IDatabase* pDb)
// {
// 	// create tutor
// 	SQLBUF		szSQL;
// 	sprintf(szSQL, "SELECT a.*,b.level as 'tutor_lev',%d as 'stu_lev',b.tutor_lv FROM %s a,%s b WHERE a.user_id=%u AND a.tutor_id=b.id LIMIT 1",GetLev(), _TBL_TUTOR,_TBL_USER, GetID());
// 	IRecordset*	pRes = pDb->CreateNewRecordset(szSQL);
// 	if(pRes)
// 	{
// 		m_pTutor = CTutor::CreateNew();
// 		if(m_pTutor)
// 		{
// 			if(!m_pTutor->Create(pRes))
// 				SAFE_DELETE (m_pTutor);
// 			TutorInfoStruct *pInfo = m_pTutor->GetInfo();
// 		}
// 		SAFE_RELEASE (pRes);
// 	}
// 
// 	// create student
// 	//sprintf(szSQL, "SELECT * FROM %s WHERE tutor_id=%u LIMIT %d", _TBL_TUTOR, GetID(), _MAX_TUTORSIZE);
// 	bool bHasGift = false;
// 	sprintf(szSQL, "SELECT a.*,b.level as 'stu_lev',%d as 'tutor_lev',0 as 'tutor_lv' FROM %s a,%s b WHERE tutor_id=%u AND a.user_id=b.id order by a.id", GetLev(), _TBL_TUTOR,_TBL_USER, GetID());//goto 徒弟数量暂时没上限，但未出师的徒弟只能有两个
// 	pRes = pDb->CreateNewRecordset(szSQL);
// 	if(pRes)
// 	{
// 		for(int i = 0; i < pRes->RecordCount(); i++)
// 		{
// 			CTutor*	pTutor = CTutor::CreateNew();
// 			if(pTutor)
// 			{
// 				if(pTutor->Create(pRes))
// 				{
// 					if (pTutor->HasGift())
// 						bHasGift = true;
// 					m_setStudent.push_back(pTutor);
// 				}
// 				else
// 				{
// 					SAFE_DELETE (pTutor);
// 				}
// 			}
// 			pRes->MoveNext();
// 		}
// 		SAFE_RELEASE (pRes);
// 	}
// 	if (bHasGift)
// 	{
// 		char szMsg[128] = "";
// 		sprintf(szMsg,"你有未领取的师徒奖励，请及时到师徒NPC处领取！");
// 		//SendSysFriendMsg(szMsg);
// 	}
// 	return true;
// }

// void CUser::DeleteAllTutor()
// {
// 	SAFE_DELETE (m_pTutor);
// 
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		SAFE_DELETE (pTutor);
// 	}
// 	m_setStudent.clear();
// }

// void CUser::SendTutorInfo(USHORT nPage)
// {
// 	if (!m_pTutor && m_setStudent.empty())
// 	{
// 		CMsgSchoolMember msg;
// 		if(msg.Create(GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0))
// 			this->SendMsg(&msg);
// 		return;
// 	}
// 
// 	vector<ST_MEMBERINFO> vecMemInfo;
// 	vector<ST_MEMBERINFO> vecMemUnGra;//未出师的
// 	vector<ST_MEMBERINFO> vecMemGra;//已出师的
// 
// 	CUser* pUser = NULL;
// 	if (m_pTutor)
// 	{
// 		pUser = UserManager()->GetUser(m_pTutor->GetTutorID());
// 		ST_MEMBERINFO infotutor;
// 		memset(&infotutor, 0, sizeof(ST_MEMBERINFO));
// 		infotutor.idMember = m_pTutor->GetTutorID() ;
// 		infotutor.nImpart = m_pTutor->GetImpart();
// 		strcpy(infotutor.szName, m_pTutor->GetTutorName());
// 		infotutor.ucRelation  = RELATION_TEACHER;
// 		if (pUser)//如果对方在线，就取实时数据
// 		{
// 			infotutor.nLev = pUser->GetLev();
// 			infotutor.nTutorLev = pUser->GetTutorLevel();
// 		}
// 		else
// 		{
// 			infotutor.nLev = m_pTutor->GetTutorLev();//师父等级
// 			infotutor.nTutorLev = m_pTutor->GetTutorImpartLv();//师德
// 		}
// 		infotutor.nGiftMask = m_pTutor->GetGiftMask();
// 		if (!m_pTutor->IsGraduated())//出师后就不发了
// 			vecMemInfo.push_back(infotutor);
// 		
// // 		if(APPLYDEL_STUDENT == m_pTutor->GetDelApplyType())
// // 		{
// // 			TutorLoginHint(m_pTutor);
// // 		}
// 	}
// 
// 	TUTOR_SET::iterator it = m_setStudent.begin();
// 	for (; it != m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor)
// 		{
// 			pUser = UserManager()->GetUser(pTutor->GetUserID());
// 			ST_MEMBERINFO infoStudent;
// 
// 			memset(&infoStudent, 0, sizeof(ST_MEMBERINFO));
// 			infoStudent.idMember = pTutor->GetUserID();
// 			infoStudent.nImpart = pTutor->GetImpart();
// 			strcpy(infoStudent.szName, pTutor->GetUserName());
// 			if (pTutor->IsGraduated())
// 				infoStudent.ucRelation  = RELATION_STUDENT_GRADUATE;
// 			else
// 				infoStudent.ucRelation	= RELATION_STUDENT;
// 			if (pUser)//如果对方在线，就取实时数据
// 				infoStudent.nLev = pUser->GetLev();
// 			else
// 				infoStudent.nLev = pTutor->GetStuLev();
// 			infoStudent.nTutorLev = 0;
// 			infoStudent.nGiftMask = pTutor->GetTutorMask();
// 			if (pTutor->IsGraduated())
// 				vecMemGra.push_back(infoStudent);
// 			else
// 				vecMemUnGra.push_back(infoStudent);
// 
// // 			if(APPLYDEL_TUTOR == pTutor->GetDelApplyType())
// // 			{
// // 				TutorLoginHint(pTutor);
// // 			}
// 		}
// 	}
// 	//goto 师徒列表分页发送
// 	if (m_setStudent.size()>0 && vecMemInfo.empty())//如果有徒弟，vecMemInfo中一定是空的
// 	{
// 		vector<ST_MEMBERINFO> vecMemInfoTemp;
// 		vector<ST_MEMBERINFO>::iterator it = vecMemUnGra.begin();
// 		for (; it != vecMemUnGra.end(); it++)
// 			vecMemInfoTemp.push_back(*it);
// 		it = vecMemGra.begin();
// 		for (; it != vecMemGra.end(); it++)
// 			vecMemInfoTemp.push_back(*it);
// 
// 		int nStuAmount = vecMemInfoTemp.size();
// 		int nPageBegin = (nPage-1)*4;//页码是从1开始的
// 		if (nStuAmount<nPageBegin)
// 			return;
// 
// 		it = vecMemInfoTemp.begin();
// 		it += nPageBegin;//指向当前页的第一个
// 		vector<ST_MEMBERINFO>::iterator itEnd = it+4;//每页最多4个
// 
// 		for (; it!=vecMemInfoTemp.end()&&it!=itEnd; it++)
// 			vecMemInfo.push_back(*it);
// 	}
// 	if(!vecMemInfo.empty())
// 	{
// 		CMsgSchoolMember	msg;
// // 		if(msg.Create(GetID(), MESSAGEBOARD_ADDMEMBER, &vecMemInfo[0], vecMemInfo.size()))
// // 		{
// // 			if (m_pTutor)
// // 				msg.m_pInfo->usStuCount = 1;//列表成员总数，客户端翻页用
// // 			else
// // 				msg.m_pInfo->usStuCount = m_setStudent.size();
// // 			this->SendMsg(&msg);
// // 		}
// 		if(msg.Create(GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0))
// 		{
// 			if (m_pTutor)
// 				msg.m_pInfo->usStuCount = 1;//列表成员总数，客户端翻页用
// 			else
// 				msg.m_pInfo->usStuCount = m_setStudent.size();
// 
// 			vector<ST_MEMBERINFO>::iterator it=vecMemInfo.begin();
// 			for (; it!=vecMemInfo.end(); it++)
// 			{
// 				msg.Append(it->idMember,it->ucRelation,it->nImpart, it->szName, it->nLev, it->nTutorLev, it->nGiftMask);
// 			}
// 			this->SendMsg(&msg);
// 		}
// 	}
// 	else
// 	{
// 		CMsgSchoolMember msg;
// 		if(msg.Create(GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0))
// 			this->SendMsg(&msg);
// 		//SendSysMsg(_TXTATR_TOPSHOW, "你没有师父或徒弟");
// 	}
// }


bool CUser::TakeExpFromStudent(int nExpTake)
{
/*	if (GetLev() < _MIN_TUTOR_LEVEL)
		return false;

	CTutorexpData*	pExpData;
	pExpData = CTutorexpData::CreateNew();
	if (!pExpData)
		return false;

	if (pExpData->Create(GetID(), Database()))
	{
		// 获得徒弟贡献的经验
		int nExp = pExpData->GetInt(TUTOREXPDATA_EXP);
		if (nExpTake > nExp)
			return false;

		// 根据导师等级以及所取得的贡献经验计算消耗的导师经验
		int nGetExpPerTutorexp = GETEXP_PER_TUTOREXP0;
		if (this->GetTutorLevel() == 1)
			nGetExpPerTutorexp = GETEXP_PER_TUTOREXP1;
		else if (this->GetTutorLevel() == 2)
			nGetExpPerTutorexp = GETEXP_PER_TUTOREXP2;
		int nCostTutorExp = nExpTake/nGetExpPerTutorexp + ((nExpTake%nGetExpPerTutorexp > 0)?1:0);
		if (this->GetTutorExp() < nCostTutorExp)
		{
			this->SendSysMsg(STR_NOT_ENOUGH_TUTOREXP);
			return false;
		}
		this->AwardTutorExp(-1*nCostTutorExp);		// 先扣除导师经验
		pExpData->SetInt(TUTOREXPDATA_EXP, nExp - nExpTake);
		this->AwardBattleExp(nExpTake);			// 得到贡献经验

		// 向导师传递经验
		int nTutorExp = pExpData->GetInt(TUTOREXPDATA_TUTOREXP);
		pExpData->SetInt(TUTOREXPDATA_TUTOREXP, 0);
		pExpData->Update();
		if (GetTutor() && nTutorExp>0)
		{
			SQLBUF szSQL;
			sprintf(szSQL, "UPDATE %s SET exp=exp+%u WHERE tutor_id=%u AND tutor_lev>%u LIMIT 1",
				_TBL_TUTOREXP, nTutorExp, m_pTutor->GetTutorID(), GetLev());
			Database()->ExecuteSQL(szSQL);
		}
	}
	SAFE_RELEASE (pExpData);*/
	return true;
}

// void CUser::NotifyTutorStatus(int nStatus)
// {
// 	if (!m_pTutor && m_setStudent.size() <= 0)
// 		return;
// 
// 	// 通知自己所有的师徒自己目前状态
// 	CMsgSchoolMember	msg;
// 	if (m_pTutor)
// 	{
// 		msg.Create(GetID(), MESSAGEBOARD_UPDATEMEMBER, NULL, 0);
// 		msg.Append(RELATION_STUDENT, nStatus, this);
// 		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pTutor->GetTutorID());
// 	}
// 
// 
// 	char szMsg[_MAX_WORDSSIZE];
// 	if (nStatus == MEMBERSTATUS_ONLINE)
// 		sprintf(szMsg,g_objStrRes.GetStr(14047) , this->GetName());//STR_TUTOR_LOGIN
// 	else
// 		sprintf(szMsg, g_objStrRes.GetStr(14048), this->GetName());//STR_TUTOR_LOGOUT
// 	
// 	msg.Create(GetID(), MESSAGEBOARD_UPDATEMEMBER, NULL, 0);
// 	msg.Append(RELATION_TEACHER, nStatus, this);
// 	TUTOR_SET::iterator it=m_setStudent.begin();
// 	for (; it!=m_setStudent.end(); it++)
// 	{
// 		CTutor* pTutor = *it;
// 		if (pTutor)
// 		{
// 			CUser* pUser = UserManager()->GetUser(pTutor->GetUserID());
// 			if (pUser)
// 				pUser->SendMsg(&msg);
// 			else
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, pTutor->GetUserID());
// 
// 			CMsgTalk	msgNotify;
// 			if (msgNotify.Create(SYSTEM_NAME, pTutor->GetUserName(), szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
// 			{
// 				if (pUser)
// 					pUser->SendMsg(&msgNotify);
// 				else
// 					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msgNotify, pTutor->GetUserID());
// 			}
// 		}
// 	}
// }

//是否是帮派帮主
bool CUser::IsSynLeader()
{
	return GetSynRank() == RANK_LEADER;

}
//20070416修罗:支持可选的同步,更新
void CUser::AddMoney(DWORD dwMoney,BOOL bSynchro/*=true*/,BOOL bUpdate/*=false*/)
{
	//AddAttrib(_USERATTRIB_MONEY,dwMoney,true);

	if (isFiftyP())
	{
		dwMoney = dwMoney / 2;
	}
	if (isZeroP())
	{
		dwMoney = 0;
	}
	int nNewMoney = GetMoney()+dwMoney;
	m_data.SetMoney(nNewMoney,bUpdate);

	// update to client...
	if(bSynchro)
	{
		CMsgUserAttrib	msg;
		if(msg.Create(this->GetID(), _USERATTRIB_MONEY, nNewMoney))
			SendMsg(&msg);
	}
}

//得到最后登陆时间
DWORD CUser::GetLastLogin()
{
	return this->m_data.GetLastLogin();
}
DWORD CUser::GetLastLogout()
{
	return this->m_data.GetLastLogout();
}
DWORD CUser::GetPassedTime(DWORD curTime,int nType/* = 1*/)
{
	int nYear = 2000 + curTime / 100000000;
	int nMounth =  (curTime % 100000000) / 1000000;
	int nDay =  (curTime % 1000000) / 10000;
	int nHour =  (curTime % 10000) / 100;
	int nMin =  curTime % 100;
	CTime timeEnd(nYear,nMounth,nDay,nHour,nMin,0);
	CTime timeNow(CTime::GetCurrentTime());
	CTimeSpan span = timeNow - timeEnd;

	switch(nType)
	{
	case 1:
		return max(0,span.GetTotalMinutes());
	case 2:
		return max(0,span.GetTotalHours());
	case 3:
		return max(0,span.GetDays());
	}
	return 0;
}

DWORD CUser::GetCreateTime()
{
	return this->m_data.GetCreateTime();
}

//是否是嵌入装备的宝石的类型
bool CUser::IsEmbedGemType(int nGemType)
{
	for(int i=ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		CItemPtr pItem = GetEquipItemByPos(i);
		if(pItem)
		{
//王玉波注释..
//			if(pItem->GetInt(ITEMDATA_GEMTYPE)==nGemType)
				return true;
		}
	}
	return false;
}

bool CUser::FlyToMainMap()
{
	if (QueryMagic())
	{
		if (QueryMagic()->IsInLaunch())	// 魔法施展过程中
			return false;
		QueryMagic()->AbortMagic();
	}

	OBJID idMap = this->GetMapID();
	idMap = (idMap/1000)*1000; //主地图ID

	CGameMap * pMap=MapManager()->QueryMap(idMap);
	
	if(pMap)
	{
		OBJID idRebornMap ;
		POINT pt;
		pMap->GetRebornMap(&idRebornMap,&pt);

		ProcessOnMove(MOVEMODE_CHGMAP);

		if(this->FlyMap(idRebornMap,pt.x,pt.y) == FLYMAP_NORMAL)
		{
			ProcessAfterMove();
			return true;
		}
		else
			return false;
	}
	else
	{
		SendSysMsg(g_objStrRes.GetStr( 10036));//STR_ACTION_USER_FLYTOMAINMAP_ERROR
		return false;
	}
	
}
//得到可以用的帮派基金
int CUser::GetAvailableSynFund()
{
	CSyndicate * pSyn = this->GetSyndicate();
	if (pSyn)
	{
		__int64 money = pSyn->GetMoney() / 2 - 1;
		return money;
	}
	return 0;
}

bool CUser::ApplyAttackSyn(CNpc * pNpc)
{
// 	if(!IsSynLeader())
// 		return false;
// 	if (!pNpc->IsSynWarLetter())
// 	{
// 		return false;
// 	}	
// 	
// 	{		
// 		CGameMap* pSynMap = MapManager()->GetGameMap(pNpc->GetMapID());
// 		if(!pSynMap->IsSynMap())
// 			return false;
// 		//判断军团等级
// 		int nRank = GetSynRank();
// 		if (nRank==SYNRANK_ONE || nRank==SYNRANK_TWO)
// 		{
// 			return false;
// 		}
// 		//检查是否已经有人下了战书
// 		if(pNpc->GetData(SYNWARLETTER_ISATTACK)>0)
// 		{
// 			SendSysMsg(g_objStrRes.GetStr( 10030));//STR_SYN_APPLYATTACKSYN_HASBEENAPPLIED
// 			return false;
// 		}
// 
// 		if(pNpc->GetData(SYNWARLETTER_OCCUPYTIME)==0)	//没有被占领
// 		{
// 			//记录占领的时间---begin
// 			pNpc->GetMap()->SetSynOccupyTime();
// 			//记录占领的时间---end
// 			CNpc* pSynFlagNpc = pSynMap->GetNpcSynFlag();
// 
// 			IF_NOT(pSynFlagNpc)
// 				return false;
// 								
// 			pSynFlagNpc->SetSynOwnerID(GetSyndicate()->GetID(), false);		// true: with link map
// 			pSynFlagNpc->GetMap()->SetSynID(GetSyndicate()->GetID(), true);		// with all map npc
// 			
// //			pSynFlagNpc->SendSelfToBlock();
// 			
// 			MSGBUF	szMsg;
// 			sprintf(szMsg,g_objStrRes.GetStr(11065) , GetSyndicate()->GetStr(SYNDATA_NAME));//STR_GOT_WIN_s				
// 			CMsgTalk msg;
// 			IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, _COLOR_WHITE, _TXTATR_IMPORTINFO))
// 				UserManager()->BroadcastMsg(&msg);
// 			
// 			// owner flag
// 			CGameMap* pMap = pNpc->GetMap();
// // 			if(pMap->GetID() == WHITE_SYN_MAP_ID)
// // 			{
// // 				CMsgSyndicate	msg;
// // 				IF_OK(msg.Create(SET_WHITE_SYN, pMap->GetOwnerID()))
// // 					UserManager()->BroadcastMsg(&msg);
// // 			}
// // 			else if(pMap->GetID() == BLACK_SYN_MAP_ID)
// // 			{
// // 				CMsgSyndicate	msg;
// // 				IF_OK(msg.Create(SET_BLACK_SYN, pMap->GetOwnerID()))
// // 					UserManager()->BroadcastMsg(&msg);
// // 			}
// 			return false;
// 		}
// 
// 		
// 		//计算此地图的价值
// 		int nSynMapValue = pSynMap->GetSynMapValue();
// 
// 		CSyndicate * pOwnerSyn = SynManager()->QuerySyndicate(pSynMap->GetSynID());
// 		
// 		//判断申请者是否符合要求
// 		CSyndicate * pSyn = GetSyndicate();
// 		if(pSyn->GetMoney() < nSynMapValue)//资金不够
// 		{
// 			SendSysMsg(g_objStrRes.GetStr( 10029));//STR_SYN_APPLYATTACKSYN_FAIL
// 			return false;
// 		}
// 		//写下战书的信息
// 		pNpc->SetData(SYNWARLETTER_ISATTACK,1,true);
// 		pNpc->SetData(SYNWARLETTER_ATTACK_SYNID,pSyn->GetID(),true);
// 		pNpc->SetData(SYNWARLETTER_DEPOSIT,nSynMapValue,true);
// 		//扣除帮派押金
// 		pSyn->QueryModify()->AddData(SYNDATA_MONEY,-nSynMapValue);
// 
// 		//计算战争时间
// 		CNpc * pNpcSynFlag=pSynMap->GetNpcSynFlag();
// 		char szAnnounce[255],szAnnounce2[255];
// 		if(pNpcSynFlag)
// 		{
// 			time_t	tCur;
// 			tCur = time(NULL);
// 			tm * pTm = localtime(&tCur);
// 			
// 			tm	tmS;	
// 			memset(&tmS, 0, sizeof(tm));
// 			
// 			tmS =*pTm;
// 			tmS.tm_mday	+= 3;			
// 			tmS.tm_hour = 20;
// 			tmS.tm_min = 0;			
// 			tmS.tm_sec = 0;
// 			time_t	tS	= mktime(&tmS);//STR_SYNWAR_ANNOUNCE STR_SYNWAR_ANNOUNCE2
// 			sprintf(szAnnounce,g_objStrRes.GetStr(14063) , tmS.tm_year+1900, tmS.tm_mon+1, tmS.tm_mday,pOwnerSyn->GetStr(SYNDATA_NAME));
// 			sprintf(szAnnounce2, g_objStrRes.GetStr(14064), tmS.tm_year+1900, tmS.tm_mon+1, tmS.tm_mday,pSyn->GetStr(SYNDATA_NAME));
// 			
// 			tmS.tm_hour = 23;
// 			time_t	tE	= mktime(&tmS);
// 			
// 			pNpcSynFlag->SetData(SYNFLAGNPC_FIGHTBEGIN,(int)tS,true);
// 			pNpcSynFlag->SetData(SYNFLAGNPC_FIGHTEND,(int)tE,true);
// 			
// 		}
// 		
// 		GetSyndicate()->QueryModify()->SetTenet(szAnnounce);
// 		GetSyndicate()->BroadcastSubSynMsg(GetSocketID(), ID_NONE, szAnnounce, NULL, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE);
// 
// 		pOwnerSyn->QueryModify()->SetTenet(szAnnounce2);
// 		pOwnerSyn->BroadcastSubSynMsg(GetSocketID(), ID_NONE, szAnnounce2, NULL, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE);		
// 	}
	//	SendSysMsg(_TXTATR_MSG_SYN,STR_SYN_APPLYATTACKSYN_SUCCESS);
	return true;	
}

//向WorldKernel发送玩家离线消息
//void CUser::NotifyChatRoomOffline()
//{
//	CMsgChatRoom	msg;
//	if(msg.Create(ACTION_OFFLINE))
//	{
//		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this->GetSocketID(),&msg);
//	}
//
//}

void CUser::SetSynDressArmorType(int nArmorType)
{
	if(nArmorType<=0)
		return ;
	m_nSynDressArmorType = nArmorType;
}


bool CUser::FlyToRandomPos()
{
	if (QueryMagic())
	{
		if (QueryMagic()->IsInLaunch())	// 魔法施展过程中
			return false;
		QueryMagic()->AbortMagic();
	}
	int nPosX = 0;
	int nPosY = 0;

	if (GetMap()->GetRandomPos(nPosX,nPosY))
	{
		ActiveTransPos(nPosX, nPosY);
		return true;
	}
	return false;
}

CTaskDetail* CUser::GetTaskDetail()
{
	return m_pTaskDetail;
}

int CUser::GetPhyle()
{
	
	// 种族修改   08.6.12  桂佳 
//	if(this->GetProfession() & PROF_DRAGON)
//		return PROF_DRAGON;
//	if(this->GetProfession() & PROF_HUMAN)
//		return PROF_HUMAN;
//	if(this->GetProfession() & PROF_GENE)
//		return PROF_GENE;

	return 0;
}
void CUser::SendTragetDesc(CUser *pTraget)
{
	if(pTraget == NULL)
		return ;
	
	CMsgName msg;
	if( msg.Create( NAMEACT_LOOK_DESC,pTraget->GetDescription()) )
		this->SendMsg( &msg );

	
}
//更改自我介绍...
//检查条件,1天之内只允许改三次
//2,不能大于127字符....
void CUser::ChangeMyDesc( char* desc )
{
	if( !desc )
	{
		this->SendSysMsg( g_objStrRes.GetStr( 15066) );//"请填写您的自我介绍!(60字以内)"
		return;
	}
	if( strlen( desc ) >= 128 )
	{
		this->SendSysMsg( g_objStrRes.GetStr( 10000 ) );
	//	this->SendSysMsg( "字数超过最大限制,更改失败!" );
		return;
	}
	DWORD dwTimeLimit = 1000 * 60 * 60 * 8;//8小时..
	//客户端先检测时间间隔,服务端也要检测..
	DWORD dwLastTime = /*this->GetLastChangeDescTime()*/0; 
	if( 0 == dwLastTime )
	{
		//表示第一次更改，不判断时间
	}
	else
	{
		if( ::GetTickCount() - dwLastTime <= dwTimeLimit )
		{
			this->SendSysMsg( g_objStrRes.GetStr( 10001 ) );
			//this->SendSysMsg( "自我介绍文字更改间隔为8个小时!" );
			return;
		}
	}
	//更新服务端自我介绍相关数据
	DWORD dwNowTime = ::GetTickCount();
	this->SetDescription( desc );
//	this->SetLastChangeDescTime( dwNowTime );
	//同步客户端..
	CMsgName msg;
	if( msg.Create( NAMEACT_CHANGE_DESC,this->GetName(),dwNowTime ) )
		this->SendMsg( &msg );

}

//火克冰   被电克
//冰克电   被火克
//电克火   被冰克
//火1 冰2 电3
//调整属性伤害.
int CUser::AdjustAttribDamage( int nPower, CMonster *pTarget )
{
	return nPower;
}


//////////////////////////////////////////////
//血光宝石	
//噬法宝石	
//乏力宝石
//这三种宝石镶入武器攻击时使对方处于某种状态
void CUser::UseWeaponWithGem( IRole *pTarget )
{

//07.4.10王玉波暂注释	
//	int nDecHpTime,nDecMpTime,nDecSpTime,nDecHpPoint,nDecMpPoint,nDecSpPoint;			//前三种次数和点数和
//	nDecHpTime = nDecMpTime = nDecSpTime = nDecHpPoint = nDecMpPoint = nDecSpPoint = 0;
//	int nDecDefPower,nDecAtkPower,nDecSpeedPower,nDecDefTime,nDecAtkTime,nDecSpeedTime;	//后三种威力和持续时间
//	nDecDefPower = nDecAtkPower = nDecSpeedPower = nDecDefTime = nDecAtkTime = nDecSpeedTime = 0;
//	if( m_pWeaponR && m_pWeaponR->IsCanUpLevel() )//手持可升级武器
//	{		
//		srand( ( unsigned int )time( NULL ) );
//		for( int i = 1;i <= 6;i++ )
//		{
//			int nResult = m_pWeaponR->GetGemStatus( i );
//			if( nResult <= 0 || nResult >= 255 )	//没有镶嵌宝石，后面的也不会有.
//				break;
//			//20070326修罗:考虑蛋
//			//CItemAdditionData* pAddition = ItemAddition()->QueryItemAddition( OBJID_GEM_BEGIN + nResult );
//			CItemAdditionData* pAddition = CItem::GetItemAddition( OBJID_GEM_BEGIN + nResult );
//			if( pAddition )
//			{
//				int nSort = pAddition->GetInt( ITEMADDITIONDATA_SORT );	
//				if( GEMSORT_TAG_DECHP == nSort )	//掉血
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//机率
//					if( rand() % 100 < nPossibility )
//					{
//						nDecHpTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); //次数
//						nDecHpPoint += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//点数
//					}
//				}
//				else if( GEMSORT_TAG_DECMP == nSort && ROLE_USER == pTarget->GetRoleType() ) //掉魔法
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//机率
//					if( rand() % 100 < nPossibility )
//					{
//						nDecMpTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); //次数
//						nDecMpPoint += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//点数
//					}
//				}
//				else if( GEMSORT_TAG_DECSP == nSort && ROLE_USER == pTarget->GetRoleType() )//掉体力
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//机率
//					if( rand() % 100 < nPossibility )
//					{
//						nDecSpTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); //次数
//						nDecSpPoint += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//点数
//					}
//				}
//				//以上三种是多次状态CMoreStatus,下面是一次状态，但有持续时间
//				else if( GEMSORT_TAG_DECDEFENCE == nSort )//目标降防
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//机率
//					if( rand() % 100 < nPossibility )
//					{
//						nDecDefTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); // 时间,单位秒
//						nDecDefPower += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//威力
//					}
//				}
//				else if( GEMSORT_TAG_DECATTACK == nSort )//目标降攻
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//机率
//					if( rand() % 100 < nPossibility )
//					{
//						nDecAtkTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); // 时间,单位秒
//						nDecAtkPower += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//威力
//					}
//					
//				}
//				else if( GEMSORT_TAG_DECSPEED == nSort )//如果目标是玩家,降低跑动速度,如果是怪物,降低移动速度
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//机率
//					if( rand() % 100 < nPossibility )
//					{
//						nDecSpeedTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); // 时间,单位秒
//						nDecSpeedPower += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//威力
//					}
//				}
//
//			}	
//		}//end for
//		//对结果进行处理
//		//多次状态，次数控制
//		int interval = 4;//间隔时间4秒
//#ifdef _DEBUG
//		char szBuff[ 128 ] = { 0 };
//#endif
//		if( nDecHpTime > 0 && nDecHpPoint > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECHP, nDecHpPoint, interval, nDecHpTime );
//#ifdef _DEBUG
//			sprintf( szBuff,"目标:%s,增加状态:%d次掉血,每次%d点,间隔时间%d秒",pTarget->GetName(),nDecHpTime,nDecHpPoint,interval );
//			this->SendSysMsg( szBuff );
//#endif
//		}
//		if( nDecMpTime > 0 && nDecMpPoint > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECMP, nDecMpPoint, interval, nDecMpTime );	
//#ifdef _DEBUG
//			sprintf( szBuff,"目标:%s,增加状态:%d次掉魔,每次%d点,间隔时间%d秒",pTarget->GetName(),nDecMpTime,nDecMpPoint,interval );
//			this->SendSysMsg( szBuff );
//#endif
//
//		}
//		if( nDecSpTime > 0 && nDecSpPoint > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECSP, nDecSpPoint, interval, nDecSpTime );	
//#ifdef _DEUBG
//			sprintf( szBuff,"目标:%s,增加状态:%d次掉体力,每次%d点,间隔时间%d秒",pTarget->GetName(),nDecSpTime,nDecSpPoint,interval );
//			this->SendSysMsg( szBuff );
//#endif
//		}
//		///一次状态，带持续时间
//		
//		if( nDecDefPower > 0 && nDecDefTime > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECDEFENCE,nDecDefPower,nDecDefTime,0,0,0 );
//#ifdef _DEBUG
//			sprintf( szBuff,"目标:%s,增加状态:%d秒内防御减少%d%%",pTarget->GetName(),nDecDefTime,nDecDefPower );
//			this->SendSysMsg( szBuff );
//#endif
//
//		}
//		if( nDecAtkPower > 0 && nDecAtkTime > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECATTACK,nDecAtkPower,nDecAtkTime,0,0,0 );
//#ifdef _DEBUG
//			sprintf( szBuff,"目标:%s,增加状态:%d秒内攻击力减少%d%%",pTarget->GetName(),nDecAtkTime,nDecAtkPower );
//			this->SendSysMsg( szBuff );
//#endif
//		}
//		if( nDecSpeedPower > 0  && nDecSpeedTime > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECSPEED,nDecSpeedPower,nDecSpeedTime,0,0,0 );
//#ifdef _DEBUG
//			sprintf( szBuff,"目标:%s,增加状态:%d秒内速度减少%d%%",pTarget->GetName(),nDecSpeedTime,nDecSpeedPower );
//			this->SendSysMsg( szBuff );
//#endif
//		}
//	}	
}
extern struct STAT_STRUCT	g_stat;
long CUser::GetTopUserCount()
{
	return g_stat.nMaxPlayers;
}
long CUser::GetCurUserCount()
{
	return g_stat.nAllPlayers;
}
//goto 只能师父调用
bool CUser::IsTutorTeam()//队伍成员是否是师徒关系
{
	if (!m_pUserTutor)
		return false;
	CTeam* pTeam = GetTeam();
	if (!pTeam)
		return false;
	if (pTeam->GetMemberAmount()>2)//只能师徒两人组队
		return false;
	OBJID idStu = pTeam->GetTeamMemberOnly();
	CTutorData* pData = GetStudent(idStu);
	if (pData)
	{
		if(pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_AGREED)
			return true;
	}
	return false;
}
//goto 徒弟调用
// bool CUser::Graduate()
// {
// 
// //	OBJID idTutor = pUser->GetTutor()->GetTutorID();
// //	if(idTutor == ID_NONE)
// //		return false;
// //	CUser *pTutor = UserManager()->GetUser(idTutor);
// 	if (!GetTutor())
// 		return false;
// 	int tutorId = GetTutor()->GetTutorID();
// 
// 	CUser* pTutor = NULL;
// 	if(this->GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_USER, tutorId, IPP_OF(pTutor)))
// 	{
// // 		if (!pTutor->IsTutorTeam())//条件判断,改由脚本中判断
// // 			return false;
// 		CTutor *pStudent = pTutor->GetStudent(GetID());
// 		if (!pStudent)
// 			return false;
// 		GetTutor()->SetGraduated(1,Database());//改为出师状态
// 		pStudent->SetGraduated(1,Database(), false);//师父的内存中的数据也要修改一下
// 		//同步客户端
// // 		CMsgSchoolMember msg;
// // 		if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))//通知徒弟
// // 		{
// // 			msg.Append(RELATION_TEACHER, 0, pTutor);
// // 			SendMsg(&msg);
// // 		}
// // 		if (msg.Create(pTutor->GetID(), MESSAGEBOARD_UPDATEMEMBER, NULL, 0))//通知师父
// // 		{
// // 			msg.Append(RELATION_STUDENT_GRADUATE, 0, this);
// // 			pTutor->SendMsg(&msg);
// // 		}
// 		pTutor->SendSysMsg("你已经成功举行了出师仪式，可以打开师徒界面领取奖励");
// 		char GraduateMsg[255];
// 		sprintf(GraduateMsg, "恭喜%s在%s的教导下成功出师！" ,this->GetName(),pTutor->GetName());//"恭喜%s在%s的悉心教导下顺利出师！"
// 		UserManager()->BroadcastMsg(GraduateMsg, NULL, NULL, 0xffffff, _TXTATR_SERVE);
// // 		this->DelTutor();
// 	}
// 	else
// 	{
// 		SendSysMsg(_TXTATR_TOPSHOW, "师徒距离太远"); //对方已不在你的附近！
// 		return false;
// 	}
// 	return true;
// }
//  [7/7/2007]
//	王玉波第一次修改，增加参数
bool CUser::IsOwnMap( int nMapID )	//是不是在本族地图
{
	int mapSort = nMapID / 1000;

//	if(mapSort == 4)
//		return true;
	// 种族修改   08.6.12  桂佳 
//	if(GetPhyle() == PROF_DRAGON)
//		return mapSort == 3;
//	if(GetPhyle() == PROF_HUMAN)
//		return mapSort == 2;
//	if(GetPhyle() == PROF_GENE)
//		return mapSort == 1;

	return false;

}
//  [6/18/2007]
//	王玉波添加
//	是不是公共地图
//  [7/7/2007],王玉波第一次修改，增加参数	
bool CUser::IsPublicMap( int nMapID )
{
	int nMapSort = nMapID / 1000;
	if( nMapSort > 3 )
	{
		return true;
	}
	return false;
}
void CUser::QueryTopList( USHORT usAction, USHORT nPage )
{
	toplistMemData arrData[20];
	int nDataSize = 0;
	if( false == g_TopListSystem.GetInfo( usAction,arrData,&nDataSize,nPage ) )
	{
		//排行更新中
		SendSysMsg("排行更新中");
		CMsgTopList msg;
		if( msg.Create( usAction,NULL,0,nPage,9999 ) )
		this->SendMsg( &msg );

		return;
	}
	int myPos = 9999;
	if(usAction < MSGTOPLIST_SYNMEM || usAction == MSGTOPLIST_EUDEMONSCORE)
		myPos = g_TopListSystem.GetTopListPlace(usAction,GetID(),GetName());
	else
	{
		if(GetSyndicate())
		{
			myPos = g_TopListSystem.GetTopListPlace(usAction,GetID(),GetSyndicate()->GetStr(SYNDATA_NAME));
		}
	}
	CMsgTopList msg;
	if( msg.Create( usAction,arrData,nDataSize,nPage,myPos ) )
		this->SendMsg( &msg );	
}
//------------------------------------------------------------------------------------------------------------------------
//07.4.26
//王玉波
//参数说明：nStatus 状态标识
//			nTime	持续时间或者是间隔时间
//			nTimes	oneStatus是0,moreStatus时表示次数
//			nPower,nParam1---		参数	
//07.4.28王玉波第一次修改，增加非BUFF、DEBUFF、DOT判断				
void CUser::AddBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )
{
	if( nStatus != ID_NONE )
	{
		int nBigType = nStatus / 10000;
		if( BIGTYPE_OTHER == nBigType )//非BUFF、DEBUFF、DOT
		{
			return;
		}
		CMsgBuff msg;
		if( msg.Create( ACTION_ADDSTATUS,this->GetID(),nStatus,nTime,nTimes,nPower,nParam1,nParam2,nParam3 ) )
		{
			if( m_pTeam  && nStatus!=STATUS_CRIME)
			{
				m_pTeam->BroadcastTeamMsg( &msg );
			}
// 			else
// 			{
				//SendMsg( &msg );
				BroadcastRoomMsg( &msg,INCLUDE_SELF,EM_SENDNPCFLAG_FALSE );
//			}
		}
		if(BIGTYPE_BUFF == nBigType || BIGTYPE_DEBUFF == nBigType)
			UpdateCalculatedProperty(UPDATECALTYPE_BUFF);//[2010-07-07 goto]增加buff后更新人物基础属性
// 		if(12018 == nStatus)
// 		{
// 			ProcessFishWorker(true, false);
// 		}
// 		else if(12031 == nStatus)
// 		{
// 			ProcessFishTutor(true, false);
// 		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------
//07.4.26
//王玉波修改
//07.4.28王玉波第二次修改，增加非BUFF、DEBUFF、DOT判断				
void CUser::UpdateBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )
{
	if( nStatus != ID_NONE )
	{
		int nBigType = nStatus / 10000;
		if( BIGTYPE_OTHER == nBigType )//非BUFF、DEBUFF、DOT
		{
			return;
		}
		CMsgBuff msg;
		if( msg.Create( ACTION_UPDATESTATUS,this->GetID(),nStatus,nTime,nTimes,nPower,nParam1,nParam2,nParam3 ) )
		{
			if( m_pTeam && nStatus!=STATUS_CRIME)
			{
				m_pTeam->BroadcastTeamMsg( &msg );
			}
//  			else
//  			{
				//SendMsg( &msg );
				BroadcastRoomMsg( &msg,INCLUDE_SELF );
//			}
		}
		if(BIGTYPE_BUFF == nBigType || BIGTYPE_DEBUFF == nBigType)
			UpdateCalculatedProperty(UPDATECALTYPE_BUFF);//[2010-07-07 goto]增加buff后更新人物基础属性
	}

}
//-------------------------------------------------------------------------------------------------------------------------
//07.4.26
//王玉波修改
//07.4.28王玉波第二次修改，增加非BUFF、DEBUFF、DOT判断				
void CUser::ClsBuff( int nStatus ) 
{
	if( nStatus != ID_NONE )
	{
		int nBigType = nStatus / 10000;
		if( BIGTYPE_OTHER == nBigType )//非BUFF、DEBUFF、DOT
		{
			return;
		}
		CMsgBuff msg;
		if( msg.Create( ACTION_DELSTATUS,this->GetID(),nStatus,0,0,0,0,0,0 ) )
		{
			if( m_pTeam  && nStatus!=STATUS_CRIME)
			{
				m_pTeam->BroadcastTeamMsg( &msg );
			}
// 			else
// 			{
				//SendMsg( &msg );
				BroadcastRoomMsg( &msg,INCLUDE_SELF,EM_SENDNPCFLAG_FALSE );
/*			}*/
		}
		if(BIGTYPE_BUFF == nBigType || BIGTYPE_DEBUFF == nBigType)
			UpdateCalculatedProperty(UPDATECALTYPE_BUFF);//[2010-07-07 goto]删除buff后更新人物基础属性
// 		if(12018 == nStatus)
// 		{
// 			ProcessFishWorker(false, false);
// 		}
// 		else if(12031 == nStatus)
// 		{
// 			ProcessFishTutor(false, false);
// 		}
	}
}

BOOL CUser::GetStorageLimit()///是否通过增值消费奖过背包
{
	return /*m_data.GetStorageLimit()*/10;
}

void CUser::SetStorageLimit(int nStoragelimit)///设置增值奖包标识
{
	//m_data.SetStorageLimit(nStoragelimit);
}

// BOOL CUser::GetSetMagicStoneBoxLimit()///是否通过增值消费奖过背包
// {
// 	return m_data.GetMagicStoneBoxLimit();
// }

// void CUser::SetMagicStoneBoxLimit(int nMagicStoneBoxLimit)///设置增值奖包标识
// {
// 	m_data.SetMagicStoneBoxLimit(nMagicStoneBoxLimit);
// }

void CUser::SendUnlockItemMsg( OBJID id )
{
	CMsgItem msg;
	IF_OK( msg.Create(ITEMACT_SYNCHRO_UNLOCK,0,0,0,0, id))
		SendMsg( &msg );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.11.1
//王玉波
//处理玩家的天生技能
//龙族:龙魂,天人族:天生敏捷
//升级时调用
//06.11.7王玉波修改机制,重写此函数
//void CUser::ProcessTalent()
//{
//
//	if( m_bDragonTalentSkill )//每三级奖励一点体质
//	{
//		DWORD dwLevel	= GetLev();
//		const int ADDHEALTH_INTERVAL_LEVEL	= 3;//每多少级奖励
//		const int ADDHEALTH_POINT			= 1;//奖励点数
//		if( 0 == dwLevel % ADDHEALTH_INTERVAL_LEVEL )
//		{
//			AddAttrib( _USERATTRIB_HEALTH,ADDHEALTH_POINT,SYNCHRO_TRUE );
//		}
//
//	}
//////////////////////////////////////////////////////////////////////////////////////
/*	DWORD dwProfess = GetProfession();
	DWORD dwLevel	= GetLev();
	CMagic* pMagic	= QueryMagic();
	if( !pMagic )
		return;
	if( dwProfess & PROF_DRAGON )//龙族
	{
		const int MAGIC_TYPE = 2001;//龙魂技能的TYPE
		const int ADDHEALTH_INTERVAL_LEVEL = 3;//每多少级奖励
		const int ADDHEALTH_POINT			= 1;//奖励点数
		if( pMagic->FindMagic( MAGIC_TYPE ) )
		{
			if( 0 == dwLevel % ADDHEALTH_INTERVAL_LEVEL )
			{
				AddAttrib( _USERATTRIB_HEALTH,ADDHEALTH_POINT,SYNCHRO_TRUE );
			}
		}
		
	}
	else if( dwProfess & PROF_HUMAN )//天人族
	{
		const int MAGIC_TYPE = 2002;//天生敏捷的TYPE
		if( pMagic->FindMagic( MAGIC_TYPE ) )
		{
			if( dwLevel >= 20 )
			{
				if( 0 == GetAddHitRatePercent() )
				{
					SetAddHitRatePercent( 1 );//增加命中加1%(100% full)
				}
			}
			if( dwLevel >= 40 )
			{
				if( 0 == GetAddDodgePercent() )
				{
					SetAddDodgePercent( 1 );//增加闪避加1
				}	
			}
			if( dwLevel >= 60 )
			{
				if( 0 == GetAddCriticalHitPercent() )
				{
					SetAddCriticalHitPercent( 1 );	//增加致命一击机率加1
				}
			}

		}
		
	}
	else
	{}*/

//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.18
//王玉波
//判断是否能得到怪物的魂,机率调整可放在此处
// bool CUser::CanGetSpirit()
// {
// 	//目前机率为30%
// #ifdef _DEBUG
// 	int probability = 100;
// #else
// 	int probability = 30;
// #endif
// 	//有机率的改变再Coding here
// 	if( ::RandGet( 100 ) < probability )
// 		return true;
// 	return false;
// }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.13
//王玉波
//怪物被杀死后,给所属玩家加魂
//未调整,也未判断是否能得到
//06.12.18王玉波第一次修改,增加机率判断和等级调整
// bool CUser::AwardSpirit( IRole* pTarget,int nNum )
// {
// 		if( !pTarget )
// 		return false;
// 	//20070113修罗:玩家必须要有[星晨智慧]被动技,能会吸到魂.------
// 	//屏蔽星辰智慧的判断
// 	//if(!QueryMagic()->FindMagic(8889))//没有[星晨智慧]技能
// 	//	return false;
// 	//-----------------------------------------------------------
// 	if( !CanGetSpirit() )
// 		return false;//不继续执行
// 
// 	//开始等级调整
// 	//10000表示100%
// 	// 如果monsterType monsterSpirit字段填400
// 	//	a)	若玩家等级大于怪物等级10级以上（包括10级），玩家将不能得到其魂
// 	//	b)	若玩家等级大于怪物等级7-9级(包括7,9)，玩家将得到80点魂			//  20%
// 	//	c)	若玩家等级大于怪物等级4-6级(包括4,6)，玩家将得到240点魂			//	60%
// 	//	d)	若玩家等级大于怪物等级0-3级，玩家将得到400点魂,即400/10000*100%( 或者玩家等级小于怪物等级)//	100%
// 
// 	int nLevelDifference = (int)this->GetLev() - (int)pTarget->GetLev(); 
// 	if( nLevelDifference >= 10 )
// 		return false;
// 	else if( nLevelDifference >= 7 && nLevelDifference <= 9 )
// 	{
// 		nNum = ( float )nNum * 0.2; 
// 	}
// 	else if( nLevelDifference >=4 && nLevelDifference <= 6 )
// 	{
// 		nNum = ( float )nNum * 0.6; 
// 	}
// 	else //if( nLevelDifference <= 3 )
// 	{
// 		nNum = ( float )nNum * 1.0;
// 	}
// 	if( nNum <=0 )
// 		return false;
// 	//结束等级调整
// 	//表现特效Coding here------------
// // GM工具调整吸魂值比率 [9/12/2007 Add By Halei ]
// //	if (::GetTickCount() < g_adjustTime[_ADJUSTRATE_SOUL_RANGE] )
// 	{
// 		float SpiritRange = g_adjustRate.Get_AdjustRate(_ADJUSTRATE_SOUL_RANGE);///g_adjustRate.m_mAdjustRateInfo[_ADJUSTRATE_SOUL_RANGE]->adjustRate;
// 		SpiritRange = SpiritRange/100.f;//换算成比率
// 		if (SpiritRange>0.0f && SpiritRange<4.0f)//比率不可大于4
// 		{
// 			nNum = nNum*SpiritRange;//吸魂数乘上比率
// 		}		
// 	}
// 
// // GM工具调整吸魂值比率 end [9/12/2007 Add By Halei ]
// 
// 		//20061222修罗:特效
// 	CMsgMagicEffect	msg;
// 	IF_OK(msg.Create(pTarget->GetID(), //怪物
// 		8888, //magicType
// 		0, //magicLev.为1即可
// 		this->GetID(),//user.目标
// 		nNum,//dwData
// 		(this->GetDir()+4)%8,BATTLEADDINFO::INFO_INVALID))//人物方向的反方向
// 	{
// 		msg.AppendRole(this->GetID(),0,BATTLEADDINFO::INFO_INVALID);
// 		this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 	}
// 
// 
// 
// 	//-------------------------------
// 
// 
// 
// 	if( nNum <= 0 )
// 		return false;
// 
// 	//----------------------end--------------------------------
// 
// 	///彭正帮2008-04-17-对日吸魂的限制
// 	//UINT nMaxSoulLimit=g_adjustRate.Get_AdjustRate(_ADJUSTRATE_SOUL_LIMIT,_ADJUSTRATE_ADJUSTRATE);
// 	
// 	UINT nMaxSoulLimit = GetMaxSoulAmount();//最大吸魂量  guijia  2008.5 .5
// 
// 	UINT nUserSoul=g_VariantManager.GetVariantInt(SPIRIT_LIMIT,GetID());
// 
// 	if (nUserSoul>=nMaxSoulLimit)
// 		return false;
// 
// 	if (nUserSoul+nNum>nMaxSoulLimit)
// 		nNum=nMaxSoulLimit-nUserSoul;
// 
// 
// 
// 
// 	int nOldSpiritGridNum	= this->GetSpiritGridNum();		//本来魂格的数量
// 	int nOldSpiritPercent	= this->GetSpiritPercent();		//本来魂格的百分比
// 
// 	CHECKF( nOldSpiritPercent < SPIRITPERCENT2GRID );
// 	
// 	int nOldSpiritValue		= SPIRITPERCENT2GRID * nOldSpiritGridNum + nOldSpiritPercent;//换算成值
// 
// 	int nNowSpiritValue = nOldSpiritValue + nNum; //加上本次
// 	
// 	int nNowSpiritGridNum	=	nNowSpiritValue / SPIRITPERCENT2GRID;
// 	int nTemp = nNowSpiritGridNum;//缓存
// 
// 	int nNowSpiritPercent	=	nNowSpiritValue % SPIRITPERCENT2GRID;
// 
// 	int nGridCheck	= __min( this->GetSpiritGridMaxNum(),SPIRITGRID_NUMLIMIT );//检查条件，取小值
// 
// 	nNowSpiritGridNum = __min( nNowSpiritGridNum,nGridCheck );
// 
// 	nNowSpiritPercent = nNowSpiritPercent + ( nTemp - nNowSpiritGridNum ) * SPIRITPERCENT2GRID;
// 	nNowSpiritPercent	= __min( nNowSpiritPercent,SPIRITPERCENT2GRID - 1 );
// 
// 		
// 	bool bSave = false;
// 	if( nOldSpiritPercent / 10 != nNowSpiritPercent / 10 )// 十分之一存一次
// 	{
// 		bSave = true;
// 	}
// 	this->SetAttrib( _USERATTRIB_SPIRIT_PERCENT,nNowSpiritPercent,SYNCHRO_TRUE );//同步客户端
// 
// 	if( nOldSpiritGridNum != nNowSpiritGridNum )//魂格改变，必须存，广播
// 	{
// 		bSave = true;
// 		this->SetAttrib( _USERATTRIB_SPIRITGRID_NUM,nNowSpiritGridNum,SYNCHRO_BROADCAST );//在ROOM中广播
// 
// 	}
// 	if( bSave )
// 		m_data.SaveInfo();// CUser是CUserData的友元类,所以可以操作保护函数 
// 
// 	///对吸魂量进行累加.
// 	nNum=(nNowSpiritGridNum-nOldSpiritGridNum)*SPIRITPERCENT2GRID+(nNowSpiritPercent-nOldSpiritPercent);
// 	g_VariantManager.AddVariantInt(SPIRIT_LIMIT,GetID(),nNum);
// 	//更新魂值，同步客户端
// 	SetAttrib(_USERATTRIB_CURRENTSOUL,0,SYNCHRO_TRUE);
// 	SetAttrib(_USERATTRIB_MAXSOUL,0,SYNCHRO_TRUE);
// 	return true;
// }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.20
//王玉波
//凝结魂石成功的机率，调用时Rand(100)如果小于此返回值，表示成功
// int CUser::GetCongealSpiritStoneSuccedProbability()
// {
// 	int nTemp = 100;//目前为100%成功
// 	return nTemp;
// }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.20
//王玉波
//得到凝结出3、2级魂石的机率，调用时RandGet( 1000 )如果小于此值表示成功
//函数处理后nProbabilityLev3要保证小于nProbabilityLev2
// void CUser::GetCongealSpiritStoneLevelProbability( int& nProbabilityLev3,int& nProbabilityLev2 )
// {
// 	//如果某个物品增加机率为百分之几，要转换
// 	int p3 = 1;	//千分之一
// 	int p2 = 10;//千分之十
// 	nProbabilityLev3 = p3;
// 	nProbabilityLev2 = p2;
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.21
//王玉波
//魂技消耗魂值特殊处理，因为存储结构不同
//07.4.20王玉波第一次修改
// void CUser::SpendSpiritValue( int nValue )
// {
// //	int nCurSpiritPercentValue = GetSpiritPercent();//百分比值
// //	if( nValue <= nCurSpiritPercentValue )
// //	{
// //		SetAttrib( _USERATTRIB_SPIRIT_PERCENT,nCurSpiritPercentValue - nValue,SYNCHRO_TRUE );//同步自己
// //	}
// //	else
// //	{
// //		int nValueConverted = GetSpiritGridNum() * SPIRITPERCENT2GRID + nCurSpiritPercentValue;
// //		nValueConverted = nValueConverted - nValue;
// //		SetAttrib( _USERATTRIB_SPIRIT_PERCENT,nValueConverted % SPIRITPERCENT2GRID,SYNCHRO_TRUE );//同步自己
// //		SetAttrib( _USERATTRIB_SPIRITGRID_NUM,nValueConverted / SPIRITPERCENT2GRID,SYNCHRO_BROADCAST );//同步并广播
// //	}
// 
// 	int nOldSpiritGridNum	= this->GetSpiritGridNum();		//本来魂格的数量
// 	int nOldSpiritPercent	= this->GetSpiritPercent();		//本来魂格的百分比
// 
// 	CHECK( nOldSpiritPercent < SPIRITPERCENT2GRID );
// 	
// 	int nOldSpiritValue		= SPIRITPERCENT2GRID * nOldSpiritGridNum + nOldSpiritPercent;//换算成值
// 
// 	int nNowSpiritValue = nOldSpiritValue - nValue; //加上本次
// 
// 	nNowSpiritValue = __max( 0,nNowSpiritValue );
// 
// 	int nNowSpiritGridNum	=	nNowSpiritValue / SPIRITPERCENT2GRID;
// 	int nNowSpiritPercent	=	nNowSpiritValue % SPIRITPERCENT2GRID;
// 
// //	int nGridCheck	= __min( this->GetSpiritGridMaxNum(),SPIRITGRID_NUMLIMIT );//检查条件，取小值
// //
// //	nNowSpiritGridNum = __min( nNowSpiritGridNum,nGridCheck );
// //	nNowSpiritPercent	= __min( nNowSpiritPercent,SPIRITPERCENT2GRID - 1 );
// 
// 		
// 	bool bSave = false;
// 	if( nOldSpiritPercent / 10 != nNowSpiritPercent / 10 )// 十分之一存一次
// 	{
// 		bSave = true;
// 	}
// 	this->SetAttrib( _USERATTRIB_SPIRIT_PERCENT,nNowSpiritPercent,SYNCHRO_TRUE );//同步客户端
// 	
// 	if( nOldSpiritGridNum != nNowSpiritGridNum )//魂格改变，必须存，广播
// 	{
// 		bSave = true;
// 		this->SetAttrib( _USERATTRIB_SPIRITGRID_NUM,nNowSpiritGridNum,SYNCHRO_BROADCAST );//在ROOM中广播
// 
// 	}
// 	if( bSave )
// 		m_data.SaveInfo();// CUser是CUserData的友元类,所以可以操作保护函数 
// 
// 
// }

// int CUser::GetSpiritAllValue()
// {
// 	int nCurSpiritPercentValue = GetSpiritPercent();//百分比值
// 	return GetSpiritGridNum() * SPIRITPERCENT2GRID + nCurSpiritPercentValue;
// }

//-------------------------------------------
//20061229修罗:新的数据结构涉及到的基础函数
void CUser::UpdateEquipBasicProperty()		//更新装备基本属性.调用上面各函数
{
	//遍历装备.将各个基础属性值扣出来
	m_equipPhysicAttackTotal = 0;
	m_equipMagicAttackTotal = 0;
	m_equipPhysicDefenseTotal = 0;
	m_equipMagicDefenseTotal = 0;
	m_equipHitarteTotal = 0;
	m_equipDodgeTotal = 0;
	m_equipCrihitTotal = 0;
	m_equipToughTotal = 0;
	m_equipAddhpTotal = 0;
	m_equipAddmpTotal = 0;
	m_equipStrTotal = 0;
	m_equipIntTotal = 0;
	m_equipStaTotal = 0;
	m_equipSpiTotal = 0;
	m_equipAgiTotal = 0;
	m_equipAtt1 = 0;
	m_equipAtt2 = 0;
	m_equipAtt3 = 0;
	m_equipAtt4 = 0;
	m_equipDef1 = 0;
	m_equipDef2 = 0;
	m_equipDef3 = 0;
	m_equipDef4 = 0;


	m_WPSLPhysicAttack = 0;
	m_WPSLMagicAttack = 0;
	m_WPSLPhysicDefense = 0;
	m_WPSLMagicDefense = 0;
	m_WPSLHitarte = 0;
	m_WPSLDodge = 0;
	m_WPSLCrihit = 0;
	m_WPSLTough = 0;
	m_WPSLStr = 0;
	m_WPSLInt = 0;
	m_WPSLSta = 0;
	m_WPSLSpi = 0;
	m_WPSLAgi = 0;
	m_WPSLAtt1 = 0;
	m_WPSLAtt2 = 0;
	m_WPSLAtt3 = 0;
	m_WPSLAtt4 = 0;
	m_WPSLDecPhyDef = 0;
	m_WPSLDecMagDef = 0;
	m_WPSLDecDef1 = 0;
	m_WPSLDecDef2 = 0;
	m_WPSLDecDef3 = 0;
	m_WPSLDecDef4 = 0;
	m_WPSLRandAddPhyAtt.nChange = 0;
	m_WPSLRandAddPhyAtt.nData= 0;
	m_WPSLRandAddMagAtt.nChange = 0;
	m_WPSLRandAddMagAtt.nData = 0;
	m_WPSLRandAddAtt1.nChange = 0;
	m_WPSLRandAddAtt1.nData = 0;
	m_WPSLRandAddAtt2.nChange = 0;
	m_WPSLRandAddAtt2.nData = 0;
	m_WPSLRandAddAtt3.nChange = 0;
	m_WPSLRandAddAtt3.nData = 0;
	m_WPSLRandAddAtt4.nChange = 0;
	m_WPSLRandAddAtt4.nData = 0;
	
	UpdateOftenUseGemAddition();

	//汇总算法---------------------------
	OFTENUSE_GEMADDITION::iterator itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_PHYATT);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipPhysicAttackTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_MAGATT);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipMagicAttackTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_PHYDEF);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipPhysicDefenseTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_MAGDEF);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipMagicDefenseTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_HIT);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipHitarteTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_DODGE);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipDodgeTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_CRIT);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipCrihitTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_TOUGH);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipToughTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_HP);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAddhpTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_MP);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAddmpTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_STR);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipStrTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_INT);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipIntTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_STA);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipStaTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_SPI);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipSpiTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_AGI);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAgiTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_ALL);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipStrTotal += itSerch->second->value;
		m_equipIntTotal += itSerch->second->value;
		m_equipStaTotal += itSerch->second->value;
		m_equipSpiTotal += itSerch->second->value;
		m_equipAgiTotal += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_ATT1);//火
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAtt1 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_ATT2);//冰
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAtt2 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_ATT3);//玄
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAtt3 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_ATT4);//毒
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAtt4 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_DEF1);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipDef1 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_DEF2);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipDef2 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_DEF3);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipDef3 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_DEF4);
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipDef4 += itSerch->second->value;
	}
	//goto 检查各数值的正确性
	if (m_equipPhysicAttackTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipPhysicAttackTotal=%d", m_equipPhysicAttackTotal);
		m_equipPhysicAttackTotal = 0;
	}
	if (m_equipMagicAttackTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipMagicAttackTotal=%d", m_equipMagicAttackTotal);
		m_equipMagicAttackTotal = 0;
	}
	if (m_equipPhysicDefenseTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipPhysicDefenseTotal=%d", m_equipPhysicDefenseTotal);
		m_equipPhysicDefenseTotal = 0;
	}
	if (m_equipMagicDefenseTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipMagicDefenseTotal=%d", m_equipMagicDefenseTotal);
		m_equipMagicDefenseTotal = 0;
	}
	if (m_equipHitarteTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipHitarteTotal=%d", m_equipHitarteTotal);
		m_equipHitarteTotal = 0;
	}
	if (m_equipDodgeTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipDodgeTotal=%d", m_equipDodgeTotal);
		m_equipDodgeTotal = 0;
	}
	if (m_equipCrihitTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipCrihitTotal=%d", m_equipCrihitTotal);
		m_equipCrihitTotal = 0;
	}
	if (m_equipToughTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipToughTotal=%d", m_equipToughTotal);
		m_equipToughTotal = 0;
	}
	if (m_equipAddhpTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipAddhpTotal=%d", m_equipAddhpTotal);
		m_equipAddhpTotal = 0;
	}
	if (m_equipAddmpTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipAddmpTotal=%d", m_equipAddmpTotal);
		m_equipAddmpTotal = 0;
	}
	if (m_equipStrTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipStrTotal=%d", m_equipStrTotal);
		m_equipStrTotal = 0;
	}
	if (m_equipIntTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipIntTotal=%d", m_equipIntTotal);
		m_equipIntTotal = 0;
	}
	if (m_equipStaTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipStaTotal=%d", m_equipStaTotal);
		m_equipStaTotal = 0;
	}
	if (m_equipSpiTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipSpiTotal=%d", m_equipSpiTotal);
		m_equipSpiTotal = 0;
	}
	if (m_equipAgiTotal < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipAgiTotal=%d", m_equipAgiTotal);
		m_equipAgiTotal = 0;
	}
	if (m_equipAtt1 < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipAtt1=%d", m_equipAtt1);
		m_equipAtt1 = 0;
	}
	if (m_equipAtt2 < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipAtt2=%d", m_equipAtt2);
		m_equipAtt2 = 0;
	}
	if (m_equipAtt3 < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipAtt3=%d", m_equipAtt3);
		m_equipAtt3 = 0;
	}
	if (m_equipAtt4 < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipAtt4=%d", m_equipAtt4);
		m_equipAtt4 = 0;
	}
	if (m_equipDef1 < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipDef1=%d", m_equipDef1);
		m_equipDef1 = 0;
	}
	if (m_equipDef2 < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipDef2=%d", m_equipDef2);
		m_equipDef2 = 0;
	}
	if (m_equipDef3 < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipDef3=%d", m_equipDef3);
		m_equipDef3 = 0;
	}
	if (m_equipDef4 < 0)
	{
		::LogSave("CUser::UpdateEquipBasicProperty unexpected value m_equipDef4=%d", m_equipDef4);
		m_equipDef4 = 0;
	}

	CItem* pWeapenSoul = GetEquipItemByPos(ITEMPOSITION_WEAPONSOUL);
	if(pWeapenSoul && pWeapenSoul->IsWeaponSoul() && pWeapenSoul->IsValidEquip())
	{
		CWeaponSoulData* pData = pWeapenSoul->QueryWeapensoulData();
		if(pData)
		{
			ST_WEAPONSOULDATA info;
			if(pData->GetInfo(info))
			{
				m_WPSLPhysicAttack = info.phyattlev + pData->ProcGift1001(DOTYPE_addphyatt,0);
				m_WPSLMagicAttack = info.magattlev + pData->ProcGift1001(DOTYPE_addmagatt,0);
				m_WPSLPhysicDefense = pData->ProcGift1001(DOTYPE_addphydef,0);
				m_WPSLMagicDefense = pData->ProcGift1001(DOTYPE_addmagdef,0);
				m_WPSLHitarte = pData->ProcGift1001(DOTYPE_addhit,0);
				m_WPSLDodge = pData->ProcGift1001(DOTYPE_adddodge,0);
				m_WPSLCrihit = pData->ProcGift1001(DOTYPE_addcrihit,0);
				m_WPSLTough = pData->ProcGift1001(DOTYPE_addTough,0);
				m_WPSLStr = info.basestr + pData->ProcGift1001(DOTYPE_addstr,0);
				m_WPSLInt = info.baseint + pData->ProcGift1001(DOTYPE_addint,0);
				m_WPSLSta = info.basesta + pData->ProcGift1001(DOTYPE_addsta,0);
				m_WPSLSpi = info.basespi + pData->ProcGift1001(DOTYPE_addspi,0);
				m_WPSLAgi = info.baseagi + pData->ProcGift1001(DOTYPE_addagi,0);
				m_WPSLAtt1 = pData->ProcGift1001(DOTYPE_addAtt1,0);
				m_WPSLAtt2 = pData->ProcGift1001(DOTYPE_addAtt2,0);
				m_WPSLAtt3 = pData->ProcGift1001(DOTYPE_addAtt3,0);
				m_WPSLAtt4 = pData->ProcGift1001(DOTYPE_addAtt4,0);
				m_WPSLDecPhyDef = pData->ProcGift1002(DOTYPE1002_decphydef,0);
				m_WPSLDecMagDef = pData->ProcGift1002(DOTYPE1002_decmagdef,0);
				m_WPSLDecDef1 = pData->ProcGift1002(DOTYPE1002_decDef1,0);
				m_WPSLDecDef2 = pData->ProcGift1002(DOTYPE1002_decDef2,0);
				m_WPSLDecDef3 = pData->ProcGift1002(DOTYPE1002_decDef3,0);
				m_WPSLDecDef4 = pData->ProcGift1002(DOTYPE1002_decDef4,0);
				pData->ProcGift1004(DOTYPE1004_phgdem,0,m_WPSLRandAddPhyAtt);
				pData->ProcGift1004(DOTYPE1004_magdem,0,m_WPSLRandAddMagAtt);
				pData->ProcGift1004(DOTYPE1004_Att1,0,m_WPSLRandAddAtt1);
				pData->ProcGift1004(DOTYPE1004_Att2,0,m_WPSLRandAddAtt2);
				pData->ProcGift1004(DOTYPE1004_Att3,0,m_WPSLRandAddAtt3);
				pData->ProcGift1004(DOTYPE1004_Att4,0,m_WPSLRandAddAtt4);
			}
		}
	}
}

//-------------------------------------------
void CUser::UpdateOftenUseGemAddition()			//更新常用附加属性
{
	ResetOftenUseGemAddition();
	OFTENUSE_GEMADDITION mapOftenUseGemPerEquipArrayTemp[ITEMPOSITION_EQUIPEND];

	//遍历所有装备
	map<OBJID, SUITINFO*> mapSuitInfo;
	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		DEBUG_TRY
		CItemPtr pEquip = GetEquipItemByPos(i);
		if(pEquip&& pEquip->IsValidEquip(this))//装备失效
		{
			if(i != ITEMPOSITION_WEAPONSOUL)
			{
				//套装
				ItemInfoStruct info;
				pEquip->GetInfo(&info);
				if (0 < info.idSuit)//0-100为套装  不能读取到后面去
				{
					SUITINFO *pSuitInfo = NULL;
					if (mapSuitInfo[info.idSuit]) //有这种套装
					{
						//更新数量
						pSuitInfo = mapSuitInfo[info.idSuit];
						++(pSuitInfo->dwNum);
					}
					else //还没有这种套装
					{
						//插入一个有1件的套装
						pSuitInfo = new SUITINFO;
						pSuitInfo->dwNum = 1;
						pSuitInfo->id = info.idSuit;
						mapSuitInfo[info.idSuit] = pSuitInfo;
					}
				}

				//基本属性
				for( unsigned char ucPos = 1;ucPos <= EM_BASEPOS_END;ucPos++ )
				{
					int nResult = pEquip->GetBasicStatus( ucPos );
					int nSort = nResult / 10000;
					if(nSort <= 0 )
						continue;
					int nValue = nResult % 10000;
					if (nValue<0)//goto 防止属性值小于0
					{
						::LogSave("CUser::UpdateOftenUseGemAddition()  基本属性值小于0");
						continue;
					}

					OFTENUSE_GEMADDITION::iterator it=mapOftenUseGemPerEquipArrayTemp[i].find(nSort);
					if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//没找到
					{		
						oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
						CHECK(pOftenUseGemAdditionTemp);
						pOftenUseGemAdditionTemp->sort = nSort;
						pOftenUseGemAdditionTemp->value = nValue;
						mapOftenUseGemPerEquipArrayTemp[i].insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
						continue;
					}
					//有同类更新
					oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
					CHECK(pOftenUseGemAdditionTemp);
					pOftenUseGemAdditionTemp->value += nValue;
				}

				//附加
				for(ucPos = 1;ucPos <= EM_ADDPOS_END;ucPos++ )
				{
					int nResult = pEquip->GetAddStatus( ucPos );
					int nSort = nResult / 10000;
					if(nSort <= 0 )
						continue;
					int nValue = nResult % 10000;
					if (nValue<0)//goto 防止属性值小于0
					{
						::LogSave("CUser::UpdateOftenUseGemAddition()  附加属性值小于0");
						continue;
					}
					
					OFTENUSE_GEMADDITION::iterator it=mapOftenUseGemPerEquipArrayTemp[i].find(nSort);
					if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//没找到
					{		
						oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
						CHECK(pOftenUseGemAdditionTemp);
						pOftenUseGemAdditionTemp->sort = nSort;
						pOftenUseGemAdditionTemp->value = nValue;
						mapOftenUseGemPerEquipArrayTemp[i].insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
						continue;
					}
					//有同类更新
					oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
					CHECK(pOftenUseGemAdditionTemp);
					pOftenUseGemAdditionTemp->value += nValue;
				}
				//铸灵
				if (pEquip->GetEquipSoulLev()>0)
				{
					int nSort = pEquip->GetEquipSoulType();
					int nLev  = pEquip->GetEquipSoulLev();
					CHECK(nSort > 0);
					CHECK(nLev <= 10);
					int nValue = 0;
					if (pEquip->IsActEquipment())
						nValue = GetEquipSoulDeadLine(2,nLev);
					else
						nValue = GetEquipSoulDeadLine(3,nLev);
					if (nValue<0)//goto 防止属性值小于0
					{
						::LogSave("CUser::UpdateOftenUseGemAddition() GetEquipSoulStatus 附加属性值小于0");
						continue;
					}

					OFTENUSE_GEMADDITION::iterator it=mapOftenUseGemPerEquipArrayTemp[i].find(nSort);
					if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//没找到
					{		
						oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
						CHECK(pOftenUseGemAdditionTemp);
						pOftenUseGemAdditionTemp->sort = nSort;
						pOftenUseGemAdditionTemp->value = nValue;
						mapOftenUseGemPerEquipArrayTemp[i].insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
					}
					else
					{
						oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
						CHECK(pOftenUseGemAdditionTemp);
						pOftenUseGemAdditionTemp->value += nValue;
					}
				}
				////end

			}
			

			//宝石//[2010-07-23 goto]修改宝石附加属性获取方式
			for( unsigned char  ucPos = 1;ucPos <= EM_GEMPOS_END;ucPos++ )
			{
				int nGemTypeID = pEquip->GetGemStatus( ucPos );
				if(nGemTypeID == 0 || nGemTypeID==_NOT_HOLE || nGemTypeID == _HOLE || CItem::GetItemSort(nGemTypeID)!=4)
					continue;
				//CItemAdditionData* pAddition = CItem::GetItemAddition( nGemTypeID,true );
				
				ItemInfoStruct info;
				memset(&info,0,sizeof(ItemInfoStruct));
				ASSERT(ItemType()->GetInfo(nGemTypeID, &info));
				int nAddition = info.BasicAddID[0];
				if(nAddition == 0)
					continue;

				int nSort = nAddition/10000;
				int nValue = nAddition%10000;
				if (nValue<0)//goto 防止属性值小于0
				{
					::LogSave("CUser::UpdateOftenUseGemAddition()  宝石属性值小于0");
					continue;
				}

				OFTENUSE_GEMADDITION::iterator it=mapOftenUseGemPerEquipArrayTemp[i].find(nSort);
				if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//没找到
				{		
					oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
					CHECK(pOftenUseGemAdditionTemp);
					pOftenUseGemAdditionTemp->sort = nSort;
					pOftenUseGemAdditionTemp->value = nValue;
					mapOftenUseGemPerEquipArrayTemp[i].insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
					continue;
				}
				oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
				CHECK(pOftenUseGemAdditionTemp);
				pOftenUseGemAdditionTemp->value += nValue;
			}
			//装备的第四个孔
			int nGemTypeID4 = pEquip->GetInt(ITEMDATA_GEM4);
			if(nGemTypeID4 != 0 && nGemTypeID4!=_NOT_HOLE && nGemTypeID4 != _HOLE && CItem::GetItemSort(nGemTypeID4)==4)
			{
				ItemInfoStruct info4 = {0};
				ASSERT(ItemType()->GetInfo(nGemTypeID4, &info4));
				int nAddition = info4.BasicAddID[0];
				if(nAddition != 0)
				{
					int nSort = nAddition/10000;
					int nValue = nAddition%10000;
					CHECK(nValue>=0);

					OFTENUSE_GEMADDITION::iterator it=mapOftenUseGemPerEquipArrayTemp[i].find(nSort);
					if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//没找到
					{		
						oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
						CHECK(pOftenUseGemAdditionTemp);
						pOftenUseGemAdditionTemp->sort = nSort;
						pOftenUseGemAdditionTemp->value = nValue;
						mapOftenUseGemPerEquipArrayTemp[i].insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
					}
					else
					{
						oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
						CHECK(pOftenUseGemAdditionTemp);
						pOftenUseGemAdditionTemp->value += nValue;
					}

				}
			}
		}
		DEBUG_CATCH3("count equipment error! position = %u userid = %u",i,GetID())
	}

	//套装效果
	map<OBJID, SUITINFO*>::iterator it = mapSuitInfo.begin();
	while (it != mapSuitInfo.end())
	{
		SUITINFO* pSuitInfo = it->second;
		SUITDATA suitData;
		memset(&suitData, 0, sizeof(SUITDATA));
		Suit()->GetDataByInfo(pSuitInfo, &suitData);
		delete pSuitInfo;
		it->second = NULL;
		++it;
		for (int i = 0; i < suitData.dwNum; ++i)
		{
			int nAddition = suitData.idAddction[i];
			if(nAddition == 0)
				continue;
			
			int nSort = nAddition/10000;
			int nValue = nAddition%10000;
			if (nValue<0)//goto 防止属性值小于0
			{
				::LogSave("CUser::UpdateOftenUseGemAddition()  套装属性值小于0");
				continue;
			}
			
			OFTENUSE_GEMADDITION::iterator it = mapOftenUseGemPerEquipArrayTemp[0].find(nSort);
			if(it == mapOftenUseGemPerEquipArrayTemp[0].end())
			{		
				oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
				CHECK(pOftenUseGemAdditionTemp);
				pOftenUseGemAdditionTemp->sort = nSort;
				pOftenUseGemAdditionTemp->value = nValue;
				mapOftenUseGemPerEquipArrayTemp[0].insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
				continue;
			}
			oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
			CHECK(pOftenUseGemAdditionTemp);
			pOftenUseGemAdditionTemp->value += nValue;
		}
	}

	
	//宝石
	for(i = 0/*ITEMPOSITION_EQUIPBEGIN*/; i < ITEMPOSITION_EQUIPEND; i++)
	{
		//遍历单个装备的宝石表
		for(OFTENUSE_GEMADDITION::iterator iterPerEquip = mapOftenUseGemPerEquipArrayTemp[i].begin(); iterPerEquip!=mapOftenUseGemPerEquipArrayTemp[i].end(); iterPerEquip++)
		{
			oftenUseGemAddition *pGemAdditionPerEquip = iterPerEquip->second;
			CHECK(pGemAdditionPerEquip);
			int nSort = pGemAdditionPerEquip->sort;
			int nValue = pGemAdditionPerEquip->value;//伤害
			if (nValue<0)//goto 防止属性值小于0
			{
				::LogSave("CUser::UpdateOftenUseGemAddition()  宝石2属性值小于0");
				continue;
			}

			//汇总算法---------------------------
			OFTENUSE_GEMADDITION::iterator itSerch=m_mOftenUseGemAddition.find(nSort);
			if( itSerch == m_mOftenUseGemAddition.end() )//没找到
			{
				//直接插入
				m_mOftenUseGemAddition.insert(OFTENUSE_GEMADDITION::value_type(pGemAdditionPerEquip->sort,pGemAdditionPerEquip));
				continue;
			}
			else//找到.汇总算法
			{
				oftenUseGemAddition *pOftenUseGemAdditionTemp = itSerch->second;
				CHECK(pOftenUseGemAdditionTemp);
				pOftenUseGemAdditionTemp->value += nValue;
				delete pGemAdditionPerEquip;
			}
		}
	}

// 	if (this->GetUseTitle())
// 	{
// 		OBJID additionID = g_TitleAttar.GetTitleAdditionId(this->GetUseTitle());
// 		CItemAdditionData* pAddition = CItem::GetItemAddition(additionID,true );//20070712修罗:忽略蛋
// 		//CHECK(pAddition);
// 		if(pAddition)//可能因为是蛋,未得到附加属性
// 		{
// 		int nSort = pAddition->GetInt( ITEMADDITIONDATA_SORT );
// 		int nValue = pAddition->GetInt( ITEMADDITIONDATA_VALUE );//伤害
// 		int nTime = pAddition->GetInt( ITEMADDITIONDATA_TIME );//持续时间
// 		int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//机率
// 		if(CItemAddition::IsOftenUseAddition(nSort))//常用sort
// 		{
// 			//如果未增添加过
// 			//直接插入
// 			OFTENUSE_GEMADDITION::iterator it=m_mOftenUseGemAddition.find(nSort);
// 			if( it == m_mOftenUseGemAddition.end() )//没找到
// 			{
// 				oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
// 				CHECK(pOftenUseGemAdditionTemp);
// 				pOftenUseGemAdditionTemp->sort = nSort;
// 				pOftenUseGemAdditionTemp->value = nValue;
// 				pOftenUseGemAdditionTemp->possibility = nPossibility;
// 				pOftenUseGemAdditionTemp->time = nTime;
// 				pOftenUseGemAdditionTemp->amount =0;
// 				//插入
// 				m_mOftenUseGemAddition.insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
// 			}else
// 			{
// 				oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
// 				CHECK(pOftenUseGemAdditionTemp);
// 					
// 				//叠加算法-------------------------
// 				//机率
// 				//[游途道標 2009.03.11]去掉裝備衰減
// 				//int lastTimePossible = pOftenUseGemAdditionTemp->possibility;//上次触发的机率
// 				//int lastTimePossibleAdjust = lastTimePossible - lastTimePossible*nPossibility/100;//上次机率调整后.扣除被本次替代的机率
// 				//pOftenUseGemAdditionTemp->possibility = lastTimePossibleAdjust + nPossibility;//机率总合.机率100为100%
// 				pOftenUseGemAdditionTemp->possibility += nPossibility;
// 					
// 				//value
// 				//[游途道標 2009.03.11]去掉裝備衰減
// 				//int lastTimeValueTotal = (lastTimePossibleAdjust==0?100:lastTimePossibleAdjust)*pOftenUseGemAdditionTemp->value*(100-_weakenValue)/100;//上次100次伤害输出总值
// 				//int thisTimeValueTotal = (nPossibility==0?100:nPossibility)*nValue;//本次100次伤害输出总值
// 				//pOftenUseGemAdditionTemp->value = (lastTimeValueTotal+thisTimeValueTotal)/(pOftenUseGemAdditionTemp->possibility==0?100:pOftenUseGemAdditionTemp->possibility);
// 				pOftenUseGemAdditionTemp->value += nValue;
// 					
// 				//time
// 				//以最后一次为准
// 				pOftenUseGemAdditionTemp->time = nTime;
// 					
// 				//amount叠加次数
// 				pOftenUseGemAdditionTemp->amount++;
// 			}
// 		}
// 		}
// 	}
//	UpdateMagicAddition();
//	UpdateGifeAddition();
}
//------------------------------
//初始化m_mOftenUseGemAddition
//void CUser::InitOftenUseGemAddition()//CUser 构造时调用
//{
//	//定义常用sort
//	//这样写方便下面调用---------------
//	const int _maxOftenUseSortAmount = 5;//常用sort总数
//	int oftenUseSortAmount = 0;
//	unsigned short oftenUseSort[_maxOftenUseSortAmount];
//	oftenUseSort[oftenUseSortAmount++] = 1;
//	oftenUseSort[oftenUseSortAmount++] = 2;
//	oftenUseSort[oftenUseSortAmount++] = 3;
//	//-----------------------------------
//	for(int i=0;i<oftenUseSortAmount;i++)
//	{
//		oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
//		pOftenUseGemAdditionTemp->sort = oftenUseSort[i];
//		pOftenUseGemAdditionTemp->value = 0;
//		pOftenUseGemAdditionTemp->possibility = 0;
//		pOftenUseGemAdditionTemp->time = 0;
//		pOftenUseGemAdditionTemp->amount = 0;//叠加次数,计算衰减时用到
//		
//		m_mOftenUseGemAddition.insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
//	}
//}
//------------------------------
//delete m_mOftenUseGemAddition
//void CUser::ClearOftenUseGemAddition()//CUser 析构时调用
//{
//	//遍历,delete
//	for(OFTENUSE_GEMADDITION::iterator iter = m_mOftenUseGemAddition.begin(); iter!=m_mOftenUseGemAddition.end(); iter++)
//	{
//		delete iter->second;
//	}
//	m_mOftenUseGemAddition.clear();
//}
//-------------------------------
void CUser::ResetOftenUseGemAddition()//reset m_mOftenUseGemAddition.user构造时调用,每次update之时调用
{
	//遍历,delete
	DEBUG_TRY
	for(OFTENUSE_GEMADDITION::iterator iter = m_mOftenUseGemAddition.begin(); iter!=m_mOftenUseGemAddition.end(); iter++)
	{
		SAFE_DELETE(iter->second);
		//delete iter->second;
	}
	DEBUG_CATCH("ResetOftenUseGemAddition()")
	m_mOftenUseGemAddition.clear();
	
}

void CUser::UpdateCalculatedProperty(UPDATECALTYPE nType)			//更新计算之后的基础属性.其中会调用以上各函数.
{
	if(UPDATECALTYPE_EQUIP == nType || UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)//装备变化 和 全部重新计算的时候才更新装备信息
	{
		UpdateEquipBasicProperty();
	}
	if(UPDATECALTYPE_GIFT == nType || UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)//装备变化 和 全部重新计算的时候才更新装备信息
	{
		UpdateGiftBaseProperty();
	}
	if(UPDATECALTYPE_BUFF == nType || UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)//装备变化 和 全部重新计算的时候才更新装备信息
	{
		UpdateBuffBaseProperty();
	}
	if(UPDATECALTYPE_EUDSOUL == nType || UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)//装备变化 和 全部重新计算的时候才更新装备信息
	{
		UpdateEudemonSoulProperty();
	}

	//同步
	SynchroUserProperty(nType);
	//bc广播 蓝量
// 	int nMaxLife = this->GetMaxLife();
// 	int nMaxMana = this->GetMaxMana();
// 	int nMaxSp = this->GetMaxSp();
// 
// 	CMsgUserAttrib	msg;
// 	msg.Create(GetID(), _USERATTRIB_MAXLIFE, nMaxLife) ;
// 	msg.Append(_USERATTRIB_MAXMANA, nMaxMana);
// 	msg.Append(_USERATTRIB_MAXSP, nMaxSp);
// 	BroadcastRoomMsg(&msg, EXCLUDE_SELF);//排除自己
// 
// 	if (m_data.GetLife() > nMaxLife)
// 	{
// 		SetAttrib(_USERATTRIB_LIFE, nMaxLife, SYNCHRO_TRUE);
// 	}
// 	
// 	if(m_data.GetMana() > nMaxMana)
// 	{
// 		SetAttrib(_USERATTRIB_MANA, nMaxMana, SYNCHRO_TRUE);
// 	}
// 
// 	if(m_data.GetSp() > nMaxSp)
// 	{
// 		SetAttrib(_USERATTRIB_SP, nMaxSp, SYNCHRO_TRUE);
// 	}
}

// void CUser::pushStudentVector( TutorInfoStruct* data)
// {
// 	CTutor* pTutor = 	CTutor::CreateNewTutor(data);
// 	if(pTutor)
// 		m_setStudent.push_back(pTutor);
// }


//--------------------------------------------------------------------

bool CUser::ChangeHideStatus()
{
	m_bHide = !m_bHide;
	SetAttrib( _USERATTRIB_HIDESTATUS,m_bHide,SYNCHRO_BROADCAST );
	return m_bHide;
}

DWORD CUser::GetPacklev(int nType)					   
{ 
	if(nType == 0)
		return m_data.GetPackLev(); 
	else if(nType > 0 && nType < 5 )
	{
		int nLev1 = m_data.GetPackLev() % 100;
		int nLev2 = (m_data.GetPackLev() / 100) % 100;
		int nLev3 = (m_data.GetPackLev() / 10000) % 100;
		int nLev4 = (m_data.GetPackLev() / 1000000) % 100;

		int nPetAdd = 0;
		int nLev = GetLev();
		if(nLev >= 30 && nLev < 50)
			nPetAdd = 1;
		else if(nLev >= 50 && nLev < 70)
			nPetAdd = 2;
		else if(nLev >= 70 && nLev < 90)
			nPetAdd = 3;
		else if(nLev >= 90)
			nPetAdd = 4;

		switch(nType)
		{
		case 1:
			return nLev1;
			break;
		case 2:
			return nLev2;
			break;
		case 3:
			return nLev3;
			break;
		case 4:
			return nLev4 + nPetAdd;
			break;
		}
	}
	return 0;
}

bool CUser::SetPacklev(int nType,DWORD nLev)		      
{ 
	if(nLev > 12 || nLev <= 0)
		return false;

	int nLev1 = m_data.GetPackLev() % 100;
	int nLev2 = (m_data.GetPackLev() / 100) % 100;
	int nLev3 = (m_data.GetPackLev() / 10000) % 100;
	int nLev4 = (m_data.GetPackLev() / 1000000) % 100;

	// 		if (502002 < pItem->GetTypeID() && 502038 > pItem->GetTypeID())
	// 		{
	// 			if (pItem->GetTypeID()>502003 && pItem->GetTypeID()<502012)
	// 			{
	// 				int Levbeibao = (pItem->GetTypeID()) % 502000 + 1;
	// 				char szMsg[512] = "";
	// 				sprintf(szMsg, "<font color='#e46d0a'>{%s}</font>千辛万苦将<font color='#cc66ff'>背包栏</font>提升到了<font color='#cc66ff'>%d级</font>，真是可喜可贺！假以时日，定能大展宏图！", this->GetName(), Levbeibao);
	// 				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
	// 				
	// 			}
	// 			else if (502015 < pItem->GetTypeID() && 502024 > pItem->GetTypeID())
	// 			{
	// 				int LevMaterialBag = (pItem->GetTypeID()) % 502010 -1;
	// 				char szMsg[512] = "";
	// 				sprintf(szMsg, "<font color='#e46d0a'>{%s}</font>不出半盏茶功夫就将<font color='#cc66ff'>材料栏</font>提升到<font color='#cc66ff'>%d级</font>，真是让武林轰动，江湖震惊！", this->GetName(), LevMaterialBag);
	// 				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
	// 			}
	// 			else if (502028 <pItem->GetTypeID() && 502038 > pItem->GetTypeID())
	// 			{
	// 				int LevPetBag = (pItem->GetTypeID()) % 502020 -5;
	// 				char szMsg[512] = "";
	// 				sprintf(szMsg, "<font color='#e46d0a'>{%s}</font>潇洒的将<font color='#cc66ff'>宠物栏</font>提升到<font color='#cc66ff'>%d级</font>，真是难以置信！人生啊！", this->GetName(), LevPetBag);
	// 				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
	// 			}
	// 		}	
	char szMsgx[512] = "";
	char UserName[256] = "";
	char UserPak[128] = "";
	char PakLev[32] = "";
	sprintf(UserName, "%d_[%s]_%d", SNAPSHOTUSER, this->GetName(), COLORYELLOW);
	switch(nType)
	{
	case 1:
		if(nLev <= nLev1)
			return false;
		nLev1 = nLev;
		break;
	case 2:
		if(nLev <= nLev2)
			return false;
		nLev2 = nLev;
		if (nLev>4){
			char szMsg[256] = "";
			sprintf(UserPak, "%d_材料栏_%d", SNAPSHOTNOTHING, COLORPINK);
			sprintf(PakLev, "%d_%d级_%d", SNAPSHOTNOTHING, nLev, COLORPINK);
			sprintf(szMsgx, "<%s>不出半盏茶功夫就将<%s>提升到<%s>，真是让武林轰动，江湖震惊！", UserName, UserPak, PakLev);
			UserManager()->BroadCastMsgAndReceiveName(szMsgx, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
		}
		break;
	case 3:
		if(nLev <= nLev3)
			return false;
		nLev3 = nLev;
		if(nLev>4){
			sprintf(UserPak, "%d_背包栏_%d", SNAPSHOTNOTHING, COLORPINK);
			sprintf(PakLev, "%d_%d级_%d", SNAPSHOTNOTHING, nLev, COLORPINK);
			sprintf(szMsgx, "<%s>千辛万苦将<%s>提升到了<%s>，真是可喜可贺！假以时日，定能大展宏图！", UserName, UserPak, PakLev);
			UserManager()->BroadCastMsgAndReceiveName(szMsgx, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
		}
		break;
	case 4:
		if(nLev <= nLev4)
			return false;
		nLev4 = nLev;
		if(nLev>3){
		sprintf(UserPak, "%d_宠物栏_%d", SNAPSHOTNOTHING, COLORPINK);
		sprintf(PakLev, "%d_%d级_%d", SNAPSHOTNOTHING, nLev, COLORPINK);
		sprintf(szMsgx, "<%s>潇洒的将<%s>提升到<%s>，真是难以置信！人生啊！", UserName, UserPak, PakLev);
		UserManager()->BroadCastMsgAndReceiveName(szMsgx, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
		}
		break;
	}

	m_data.SetPackLev( (nLev4 % 100) * 1000000 + (nLev3 % 100) * 10000 + (nLev2 % 100) * 100 +  (nLev1 % 100));

	CMsgUserAttrib msg;
	if(msg.Create(this->GetID(),_USERATTRIB_PACKAGELEV,m_data.GetPackLev()))
		this->SendMsg(&msg);
	return true;
}


DWORD   CUser::GetBanklev(int nType/* = 0*/)
{ 
	if(nType == 0)
		return m_data.GetBankLev(); 
	else if(nType > 0 && nType < 3 )
	{
		int nLev1 = m_data.GetBankLev() % 100;
		int nLev2 = (m_data.GetBankLev() / 100) % 100;
		
		switch(nType)
		{
		case 1:
			return nLev1;
			break;
		case 2:
			return nLev2;
			break;
		}
	}
	return 0;
}

bool CUser::SetBanklev(int nType,DWORD nLev)
{
	if(nLev > 14 || nLev <= 0)
		return false;
	
	int nLev1 = m_data.GetBankLev() % 100;
	int nLev2 = (m_data.GetBankLev() / 100) % 100;
	
	switch(nType)
	{
	case 1:
		if(nLev <= nLev1)
			return false;
		nLev1 = nLev;
		break;
	case 2:
		if(nLev <= nLev2)
			return false;
		nLev2 = nLev;
		break;
	}
	
	m_data.SetBankLev( (nLev2 % 100) * 100 +  (nLev1 % 100));

	CMsgUserAttrib msg;
	if(msg.Create(this->GetID(),_USERATTRIB_BANKLEV, m_data.GetBankLev()))
		this->SendMsg(&msg);

	return true;
}
//-------------------------------------------------
//白名
inline BOOL CUser::IsWhiteName()
{
	return GetPk()==PKVALUE_WHITE;
}
//----------------------------------------------
//灰名
BOOL	CUser::IsGrayName()
{
	if(QueryStatus(STATUS_CRIME))
		return true;
	else
		return false;
}
//----------------------------------------------
//红名
BOOL	CUser::IsRedName()
{
	return GetPk()>PKVALUE_WHITE;
}

int CUser::GetTaskStep(OBJID idTask)
{
	CTaskDetail * pTask = GetTaskDetail();
	IF_NOT(pTask)
		return 0;

	CTaskDetailData* pTaskDetail = pTask->QueryData(GetID(), idTask);
	if (!pTaskDetail)
		return 0;

	return pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE);
}

//  [4/2/2007] halei 装备创造的时候改变它的附加属性
void CUser::EquipChgAttribute( ItemInfoStruct* info,int nBossType /*= 0 */,int nAddType/* = 0*/)
{
	if (info->idType/100000 != IETMSORT_FINERY1 && info->idType/100000 != IETMSORT_FINERY2)
	{
		return;
	}
	if(info->idType == 144000)
	{
		//品质
		info->dwQual = 5;
		//颜色
		info->dwColor = 5;
		//等级
		info->dwAddLev = 10;
		//星级
		info->dwStr = 10;
		return;
	}


	float nQualAdd = 1.0;
	float nColorAdd = 1.0;

	//品质
	int nCheckData = nBossType;
	if(nBossType == 10000)
		nCheckData = 2;

	int nQual = RandGet(18);
	if(nQual >= 9 && nQual < 12)
	{
		info->dwQual = 1;
		nQualAdd = 1.05f;
	}
	else if(nQual >= 6 && nQual < 9)
	{
		info->dwQual = 2;
		nQualAdd = 1.1f;
	}
	else if(nQual >= 4 && nQual < 6)
	{
		info->dwQual = 3;
		nQualAdd = 1.15f;
	}
	else if(nQual >= 2 && nQual < 4)
	{
		info->dwQual = 4;
		nQualAdd = 1.2f;
	}
	else if(nQual >= 0 && nQual < 2)
	{
		info->dwQual = 5;
		nQualAdd = 1.25f;
	}
	
	if(info->dwQual >= max(4,nCheckData + 3))
	{
		info->dwQual = 1;
		nQualAdd = 1.05f;
	}

	if(nBossType == 10000)
	{
		CHECK(nAddType >= 0 && nAddType < 6);
		//品质  暂时不影响
 		int nAllCount = 0;
// 		int nQual = RandGet(10000);
// 
// 		int QualData[6][6] = {	{8500	,1000	,500	,0		,0		,0},
// 								{6500	,2000	,1000	,500	,0		,0},
// 								{3500	,3000	,2000	,1000	,500	,0},
// 								{1500	,2000	,3000	,2000	,1000	,500},
// 								{500	,1000	,2000	,3000	,2000	,1500},
// 								{0		,500	,1000	,2000	,3000	,3500}};
// 
// 		for(int i = 0 ; i < 6 ; i ++)
// 		{
// 			nAllCount += QualData[nAddType][i];
// 			if(nQual < nAllCount)
// 			{
// 				switch(i)
// 				{
// 				case 1:
// 					info->dwQual = 1;
// 					nQualAdd = 1.05;
// 					break;
// 				case 2:
// 					info->dwQual = 2;
// 					nQualAdd = 1.1;
// 					break;
// 				case 3:
// 					info->dwQual = 3;
// 					nQualAdd = 1.15;
// 					break;
// 				case 4:
// 					info->dwQual = 4;
// 					nQualAdd = 1.2;
// 					break;
// 				case 5:
// 					info->dwQual = 5;
// 					nQualAdd = 1.25;
// 					break;
// 				}
// 				break;
// 			}
// 		}


		//颜色		
		if(info->dwColor != 0 )
		{
			if(info->dwColor == 2 )
			{
				nColorAdd = 1.05f;
			}
			else if(info->dwColor == 3 )
			{
				nColorAdd = 1.1f;
			}
			else if(info->dwColor == 4 )
			{
				nColorAdd = 1.2f;
			}
			else if(info->dwColor == 5 )
			{
				nColorAdd = 1.3f;
			}
		}
		else
		{
			nAllCount = 0;
			int nColor = RandGet(10000);
			
			int ColorData[6][5] = {	{3500	,4000	,2000	,500	,0},
									{1500	,2000	,4000	,2000	,500},
									{0		,2000	,3000	,3500	,1500},
									{0		,1000	,2000	,4500	,2500},
									{0		,0		,1500	,4000	,4500},
									{0		,0		,0		,1500	,8500}};
			
			for(int i = 0 ; i < 5 ; i ++)
			{
				nAllCount += ColorData[nAddType][i];
				if(nColor < nAllCount)
				{
					switch(i)
					{
					case 0:
						info->dwColor = 1;
						nColorAdd = 1.05f;
						break;
					case 1:
						info->dwColor = 2;
						nColorAdd = 1.1f;
						break;
					case 2:
						info->dwColor = 3;
						nColorAdd = 1.15f;
						break;
					case 3:
						info->dwColor = 4;
						nColorAdd = 1.2f;
						break;
					case 4:
						info->dwColor = 5;
						nColorAdd = 1.25f;
						break;
					}
					break;
				}
			}
		}
	}
	else
	{
		//品质		
// 		int nQual = RandGet(18);
// 		if(nQual >= 9 && nQual < 12)
// 		{
// 			info->dwQual = 1;
// 			nQualAdd = 1.05;
// 		}
// 		else if(nQual >= 6 && nQual < 9)
// 		{
// 			info->dwQual = 2;
// 			nQualAdd = 1.1;
// 		}
// 		else if(nQual >= 4 && nQual < 6)
// 		{
// 			info->dwQual = 3;
// 			nQualAdd = 1.15;
// 		}
// 		else if(nQual >= 2 && nQual < 4)
// 		{
// 			info->dwQual = 4;
// 			nQualAdd = 1.2;
// 		}
// 		else if(nQual >= 0 && nQual < 2)
// 		{
// 			info->dwQual = 5;
// 			nQualAdd = 1.25;
// 		}
// 
// 		if(info->dwQual >= max(4,nBossType + 3))
// 		{
// 			info->dwQual = 1;
// 			nQualAdd = 1.05;
// 		}

		//颜色		
		if(info->dwColor != 0 )
		{
			if(info->dwColor == 2 )
			{
				nColorAdd = 1.05f;
			}
			else if(info->dwColor == 3 )
			{
				nColorAdd = 1.1f;
			}
			else if(info->dwColor == 4 )
			{
				nColorAdd = 1.2f;
			}
			else if(info->dwColor == 5 )
			{
				nColorAdd = 1.3f;
			}
		}
		else
		{
			info->dwColor = 1;
			int nColor = RandGet(16);
			if(nColor >= 4 && nColor < 8)
			{
				info->dwColor = 2;
				nColorAdd = 1.05f;
			}
			else if(nColor >= 2 && nColor < 4)
			{
				info->dwColor = 3;
				nColorAdd = 1.1f;
			}
			else if(nColor == 1 )
			{
				info->dwColor = 4;
				nColorAdd = 1.2f;
			}
			else if(nColor == 0)
			{
				info->dwColor = 5;
				nColorAdd = 1.3f;
			}

			if(info->dwColor >= max(4,nBossType + 3))
			{
				info->dwColor = 1;
				nColorAdd = 1.0f;
			}
		}
	}
	
	//等级
	info->dwAddLev = 0;

	//星级
	float nStarAdd = 0.0f;
	if(info->dwStr == 1 )
	{
		nStarAdd += 0.1f;
	}
	else if(info->dwStr == 2 )
	{
		nStarAdd += 0.2f;
	}
	else if(info->dwStr == 3 )
	{
		nStarAdd += 0.3f;
	}
	else if(info->dwStr == 4 )
	{
		nStarAdd += 0.4f;
	}
	else if(info->dwStr == 5 )
	{
		nStarAdd += 0.55f;
	}
	else if(info->dwStr == 6)
	{
		nStarAdd += 0.7f;
	}
	else if(info->dwStr == 7)
	{
		nStarAdd += 0.87f;
	}
	else if(info->dwStr == 8)
	{
		nStarAdd += 1.05f;
	}
	else if(info->dwStr == 9)
	{
		nStarAdd += 1.25;
	}
	else if(info->dwStr == 10)
	{
		nStarAdd += 1.6f;
	}

	//基本属性变化
	int nType = 0;
	int nvalue = 0;
	for(int i = 0 ; i < 2 ; i ++)
	{
		if(info->BasicAddID[i] != 0 )
		{
			nType = info->BasicAddID[i]/10000;
			if(nType < 0)
				continue;
			nvalue = info->BasicAddID[i]%10000;

			int nRand = RandGet(51) + 975;
			info->BasicAddID[i] = nType * 10000 + (DWORD)(nvalue * nColorAdd * nQualAdd * ((float)(nRand / 1000.0f)));
		}
	}

	//附加属性
	//填的
	if(info->AdditionAddID[0] != 0 )
	{
		for(i = 0 ; i < 7 ; i ++)
		{
			nType = 0;
			nvalue = 0;
			if(info->AdditionAddID[i] == 0 )
			{
				break;
			}
			else
			{
				nType = info->AdditionAddID[i] / 10000;
				nvalue = info->AdditionAddID[i] % 10000;
			}

			if(nvalue <= 0)
				continue;
			
			int nRand = RandGet(51) + 975;
			info->AdditionAddID[i] = nType * 10000 + (DWORD)(nvalue * nColorAdd * (nQualAdd + nStarAdd) * ((float)(nRand / 1000.0f)));
		}
		return;
	}
	//未填随机数量
	int nMax = 0;
	int nMin = 0;
	int nLevNed = info->usReqJobLev/10;
	switch(nLevNed)
	{
	case 0:
		nMin = 0;
		nMax = 1;
		break;
	case 1:
		nMin = 1;
		nMax = 2;
		break;
	case 2:
		nMin = 1;
		nMax = 3;
		break;
	case 3:	
		nMin = 2;
		nMax = 3;
		break;
	case 4:
	case 5:	
		nMin = 2;
		nMax = 4;
		break;
	case 6:	
		nMin = 2;
		nMax = 5;
	break;
	case 7:
	case 8:
	case 9:
	case 10:
		nMin = 3;
		nMax = 5;
		break;	
	case 11:
	case 12:
	case 13:
		nMin = 3;
		nMax = 6;
		break;
	case 14:
		nMin = 3;
		nMax = 7;
		break;
	case 15:
		nMin = 4;
		nMax = 7;
		break;
	}

	int nMun = nMin;
	if(nMax > nMin)
	{
		nMun = nMin + RandGet(nMax - nMin + 1);
	}
	
	nLevNed = info->usReqJobLev;

	for(i = 0 ; i < nMun ; i ++)
	{
		nType = 0;
		nvalue = 0;

		int nCount = 0;
		bool bSetOK = false;
		while(!bSetOK)
		{
			if(nBossType == 10000)
			{
				nType = RandGet(24) + 1;
				if(nType > 16)
					nType += 3;
			}
			else
			{
				nType = RandGet(20) + 1;
				if(nType > 16)
					nType += 7;
			}
			
			for(int j = 0 ; j < i ; j ++)
			{
				if(info->AdditionAddID[j] / 10000 == nType)
					break;
			}
			//全重复，不继续了
			nCount ++;
			if(nCount >= 10)
			{
				return;
			}

			if(j == i)
				bSetOK = true;
		}

		//外功内功 外防内防
		switch(nType)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			if(nLevNed > 10)
				nvalue = 4.6222f *  nLevNed - 8.9419f;
			else
				nvalue = 3.0976f * nLevNed + 10.7569f;
			break;
		//命中 闪避 致命
		case 5:
		case 6:
		case 7:
			if(nLevNed > 10)
				nvalue = 0.5082f *  nLevNed - 0.9801f;
			else
				nvalue = 0.3388f * nLevNed + 1.1979f;
			break;
		//韧性
		case 8:
			if(nLevNed > 10)
				nvalue = 0.5566f*nLevNed - 1.0769f;
			else
				nvalue = 0.3751f*nLevNed + 1.3189f;
			break;
		//力量 灵力  体力 定力  身法
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
			if(nLevNed > 10)
				nvalue = 0.46222f * nLevNed - 0.89419f;
			else
				nvalue = 0.30976f * nLevNed + 1.08658f;
			break;
		//所有属性
		case 14:
			if(nLevNed > 10)
				nvalue = 0.30734f *  nLevNed - 0.59532f;
			else
				nvalue = 0.2057f * nLevNed + 0.726f;
			break;
		//hp mp
		case 15:
		case 16:
			if(nLevNed > 10)
				nvalue = 23.0989f *  nLevNed - 44.7095f;
			else
				nvalue = 15.488f *  nLevNed + 54.329f;
			break;
		//属性攻击
		case 20:
		case 21:
		case 22:
		case 23:
		//属性防御
		case 24:
		case 25:
		case 26:
		case 27:
			if(nLevNed > 10)
				nvalue = 0.308147f *  nLevNed - 0.09613f;
			else
				nvalue = 0.206507f *  nLevNed + 1.217127f;
			break;
		}
		
		if(nvalue <= 0)
			continue;

		int nRand = RandGet(20) + 90;
		info->AdditionAddID[i] = nType * 10000 + (DWORD)(nvalue * nColorAdd * (nQualAdd + nStarAdd) * ((float)(nRand / 100.0f)));
	}
}

//---------------------------------------------------
bool CUser::FetchApply(int nApply, OBJID idTarget)
{ 
	if(nApply == m_nApply && idTarget == m_idApplyTarget) 
	{ 
		m_nApply = APPLY_NONE; 
		return true; 
	} 
	return false; 
}


void CUser::ProcessRebornMe()
{
	//2007828修罗:去掉得知活时间限制.--------
	//及新的reborn
	Reborn(false,m_bRebornFlag,m_nRecoverPercent);
	m_nRecoverPercent = 0;
	m_bRebornFlag = false;

//	if( !m_tWaitReborn.IsTimeOut() )
//	{
//		Reborn( false );
//
//		m_nRecoverPercent = 0;
//		m_bRebornFlag = false;
//	}
	//-----------------------------------------
}

//------------------------------------
void CUser::SynchroLockAtkBegin()
{
	CMsgAction msg;
	IF_OK(msg.Create(this->GetID(),//idPlayer
		0,//nPosX
		0,//nPosY
		0,//usDir
		actionLockAtkBegin))
		this->SendMsg(&msg);
}

//------------------------------------
void CUser::SynchroLockAtkEnd()
{

	CMsgAction msg;
	IF_OK(msg.Create(this->GetID(),//idPlayer
		0,//nPosX
		0,//nPosY
		0,//usDir
		actionLockAtkEnd))
		this->SendMsg(&msg);
}

BOOL CUser::IsFluctuate( UINT itemTypeID )
{
	if ((itemTypeID/10000) == ITEMTYPE_SHOULDER
		||(itemTypeID/10000) ==ITEMTYPE_ARMOR
		||(itemTypeID/10000) ==ITEMTYPE_HELMET
		||(itemTypeID/10000) == ITEMTYPE_WEAPONR
		||(itemTypeID/10000) ==ITEMTYPE_NECKLACE
		||(itemTypeID/10000) ==ITEMTYPE_BANGLE
		||(itemTypeID/10000) ==ITEMTYPE_GLOVE
		||(itemTypeID/10000) ==ITEMTYPE_CAESTUS
		||(itemTypeID/10000) ==ITEMTYPE_SHOES
//		||(itemTypeID/10000) ==ITEMTYPE_MOUNT
		||(itemTypeID/10000) ==ITEMTYPE_RING
		||(itemTypeID/10000) ==IETMTYPE_ADORN
//		||(itemTypeID/10000) ==ITEMTYPE_DRESS
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//	20070703 朱斌
//	玩家死亡时是否掉经验,掉装备,掉耐久
//	返回TRUE时为有死亡惩罚,FALSE时为没有死亡惩罚
BOOL CUser::DoesUserNeedDeathPunishment(IRole* pRole)
{
	//	20070705 PK地图
	CGameMap* pMap = this->GetMap();
	if(pMap->IsPkFieldNoDeathPunish())
		return FALSE;

	if(GetLev() < VETERAN_DIFF_LEVEL)
		return FALSE;

	///softworms-07-08-28-判定所有参加战争活动的玩家,死亡不惩罚
	if (QueryStatus(STATUS_WARGAME))
		return FALSE;
		
	return TRUE;
}

//	20070703 朱斌
//	检查经验值增减
//	返回计算增减后的经验值
//int CUser::CheckExp(__int64 &nAddExp/*传入增减的经验值,传出实际允许的经验值(避免值过大溢出)*/)
__int64 CUser::CheckExp(__int64 &nAddExp/*传入增减的经验值,传出实际允许的经验值(避免值过大溢出)*/)
{
	if(nAddExp > 0)	//	只检查扣经验
		return GetExp() + nAddExp;

	if(nAddExp < USER_NEGATIVE_EXP_MIN_VALUE)
		nAddExp = USER_NEGATIVE_EXP_MIN_VALUE;

	__int64 i64CurrentExp = GetExp();
	__int64 newExp = i64CurrentExp + nAddExp;
	if(newExp < USER_NEGATIVE_EXP_MIN_VALUE)	//	不能低于最低值
		newExp = USER_NEGATIVE_EXP_MIN_VALUE;
	
	//	不能超过最高百分比
	__int64 i64LevExp = 0;
	CLevupexpData* pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
	if (!pLevupexp)
		pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + __max(0, this->GetLev()-1));
	if (pLevupexp)
		i64LevExp = pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
	
	if(newExp < 0 && i64LevExp != 0)
	{
		if((abs((long double)newExp / i64LevExp) * 100 ) >= abs(USER_NEGATIVE_EXP_MAX_PERCENT))
		{
			double scale = (USER_NEGATIVE_EXP_MAX_PERCENT / 100);
		//	newExp = scale * i64LevExp ;
			newExp = (__int64)scale * i64LevExp ;
			if(newExp < USER_NEGATIVE_EXP_MIN_VALUE)
				newExp = USER_NEGATIVE_EXP_MIN_VALUE;
		}
	}

	nAddExp = newExp - GetExp();
	return newExp;			
}

void CUser::SetPk(int nPk)
{
	 m_data.SetPk( nPk );
	 this->SetAttrib(_USERATTRIB_PK, this->GetPk(), SYNCHRO_BROADCAST);

// 	 if(GetPk()> 0)
// 		 SetStatus(STATUS_PKVALUE, SYNCHRO_TRUE);
// 	 else
// 		 ClsStatus(STATUS_PKVALUE, SYNCHRO_TRUE);

}

// void CUser::SetExp(int nExp)
// {
// 	m_data.SetExp(nExp);
// 	this->SetAttrib(_USERATTRIB_EXP, this->GetExp(), SYNCHRO_TRUE);
// 
// 
// }

void CUser::SetExp(__int64 i64Exp)
{
	m_data.SetExp(i64Exp);
	this->SetAttrib(_USERATTRIB_EXP, this->GetExp(), SYNCHRO_TRUE);


}


bool CUser::IsTalkEnable()
{
	IStatus* pStatus = QueryStatus( STATUS_STOPSAY );	
	if (pStatus)
		return false;
	
	return true;
}
/*
PRESENT_ONE	
PRESENT_TWO	
PRESENT_THREE
PRESENT_FOUR
PRESENT_FIVE
PRESENT_SIX	
PRESENT_SEVEN
PRESENT_EIGHT
*/
int CUser::GetCardState( int nIndex)
{
	if(nIndex < 0 || nIndex > 10)
		return 1;

	return GetNewKardState(nIndex);
}
void CUser::SetCardState( int nIndex, int nData ,bool bSend)
{
	int data = GetNewKardState(nIndex);
	if(data == nData)
		return;
	else
		SetNewKardState(nData,nIndex);

	if(bSend && nData != 0)
	{
		CMsgVAS MsgVAS;
		if (MsgVAS.Create(nIndex, GetAccountID(), GetID(), _MSGVAS_OPRATE_CARD,nData))
		{
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
		}
	}
// 	int data = -1;
// 	if( nType >= 4 && nType < 6)
// 	{
// 		data = /*GetPresentByIndex(nType)*/0;
// 	}
// 	switch( nType )
// 	{
// 	case 1:
// 		{
// 			data = GetGeneralizeState();
// 		}
// 		break;
// 	case 2:
// 		{
// 			data = GetNewPlayerState();
// 		}
// 		break;
// 	case 3:
// 		{
// //			SetExamState(nAction);
// 		}
// 		break;	
// 	case 6:
// 	case 7:
// 	case 8:
// 	case 9:
// 	case 10:
// 		{
// 			data = GetNewKardState(nType - 6);
// 		}
// 		break;	
// 	}
// 	if(nType > 5 && nType < 11 )
// 	{
// 		if(nAction == 1) //改变 并且告诉帐号服务器数据改变
// 		{
// 			if(nData != data)
// 			{
// 				SetNewKardState(nData,nType - 6);
// 
// // 				CMsgVAS MsgVAS1;
// // 				
// // 				MsgVAS1.Create( ( char* )&MsgVAS1.m_pInfo->mtExtraInfo, nType, GetAccountID(), GetID(),12 );
// // 				MsgVAS1.m_pInfo->mtExtraInfo.itemID = nData;
// // 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS1 );
// 			}
// 		}
// 		else //数据来自数据库 不需要通知帐号服务器
// 		{
// 			if(nData != data)
// 			{
// 				SetNewKardState(nData,nType - 6);
// 			}
// 		}
// 		return;
// 	}
// 
// 	if( !nAction )
// 	{
// 		data = nData;
// 		switch(nType)
// 		{
// 		case 1:
// 			{
// 				SetGeneralizeState(data);
// 			}
// 			break;
// 		case 2:
// 			{
// 				SetNewPlayerState(data);
// 			}
// 			break;
// 		}
// 		if(nType >= 4)
// 		{
// 			//SetPresentByIndex( nType, data );
// 		}
// 		
// 		return;
// 	}
// 	else
// 	{
// 		if( 5 == nType )
// 			data = nAction;
// 		else
// 		{
// 			data |= 1<<nAction;
// 			::GmLogSave("OK: 领取%d次装备成功！AccountID:%d, playerID: %d ",nAction,GetAccountID(), GetID() );
// 		}
// 	}
// 
// 	switch(nType)
// 	{
// 	case 1:
// 		{
// 			SetGeneralizeState(data);
// 		}
// 		break;
// 	case 2:
// 		{
// 			SetNewPlayerState(data);
// 		}
// 		break;
// 	}
// 	if(nType >= 4)
// 		;
// 		//SetPresentByIndex( nType, data );
// 
// // 	CMsgVAS MsgVAS;
// // 	MsgVAS.Create( ( void* )&MsgVAS.m_pInfo->mtExtraInfo, nType, GetAccountID(), GetID(), 11,nAction );///发消息顺带说明消息的来历或类型
// // 	MsgVAS.AppendInfo( m_idProcess,m_idSocket,ID_NONE );//借用AppendInfo函数，07.1.20修改
// // 	//给世界线程发消息
// // 	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
// 
// 	if( nType >= 4  && nType != 6)
// 	{
// // 		CMsgVAS MsgVAS1;
// // 		MsgVAS1.Create( ( char* )&MsgVAS1.m_pInfo->mtExtraInfo, nType, GetAccountID(), GetID(),10 );///发消息顺带说明消息的来历或类型
// // 		//给世界线程发消息
// // 		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS1 );
// 	}		
}

//	20070712	朱斌	
//	按类型ID查找全部装备,仓库,背包,魔晶盒,身上
CItemPtr CUser::GetUserAllItemByType(OBJID idType)	
{
	CItemPtr	pItem = NULL;
	//	仓库
	pItem = GetStorageItemByType(idType);
	if(pItem)
		return pItem;

	//	背包,魔晶盒
	pItem = QueryPackage()->GetItemByType(idType);
	if(pItem)
		return pItem;

	//	身上
	pItem = GetEquipItemByType(idType);
	if(pItem)
		return pItem;
	

	return NULL;
}

//	20070712	朱斌	
//	查找装备在身上的
CItemPtr CUser::GetEquipItemByType(OBJID idType)
{
	if(m_pShoulder && m_pShoulder->GetTypeID() == idType)
		return m_pShoulder;
	if(m_pArmor && m_pArmor->GetTypeID() == idType)
		return m_pArmor;
	if(m_pHelmet && m_pHelmet->GetTypeID() == idType)
		return m_pHelmet;
	if(m_pWeaponR && m_pWeaponR->GetTypeID() == idType)
		return m_pWeaponR;
	if(m_pNecklace && m_pNecklace->GetTypeID() == idType)
		return m_pNecklace;
	if(m_pBangle  && m_pBangle ->GetTypeID() == idType)
		return m_pBangle;
	if(m_pGlove  && m_pGlove ->GetTypeID() == idType)
		return m_pGlove;
	if(m_pCaestus  && m_pCaestus ->GetTypeID() == idType)
		return m_pCaestus ;
	if(m_pShoes && m_pShoes->GetTypeID() == idType)
		return m_pShoes;
	if(m_pMount && m_pMount->GetTypeID() == idType)
		return m_pMount;
	if(m_pRingT && m_pRingT->GetTypeID() == idType)
		return m_pRingT;
	if(m_pRingB && m_pRingB->GetTypeID() == idType)
		return m_pRingB;
	if(m_pAdornT && m_pAdornT->GetTypeID() == idType)
		return m_pAdornT;
	if(m_pAdornB && m_pAdornB->GetTypeID() == idType)
		return m_pAdornB;
	if(m_pDress && m_pDress->GetTypeID() == idType)
		return m_pDress;
	if(m_pWeaponSoul && m_pWeaponSoul->GetTypeID() == idType)
		return m_pWeaponSoul;
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//  [2007-8-8] 朱斌 注入星辰力量到魔导芯片
// BOOL CUser::ImportSpiritStoneToChip(int *pMaterial, int nSize, char *szError)
// {
// 	CHECKF(pMaterial && nSize);
// 
// 	CItem*	pItemChip = this->GetItem(pMaterial[0]);	
// 	if(!pItemChip)
// 	{
// 		sprintf(szError,g_objStrRes.GetStr(17018) );//"没有找到魔导芯片!"
// 		return FALSE;
// 	}
// 
// 	if(!pItemChip->IsSpiritChip() 
// 		&& !pItemChip->IsAdvancedSpiritChip())
// 	{
// 		sprintf(szError, g_objStrRes.GetStr(17019));//"没有魔导芯片!"
// 		return FALSE;
// 	}
// 
// 	int nCurrentChipSpirit = pItemChip->GetInt(ITEMDATA_AMOUNT);
// 	ItemInfoStruct info;
// 	ItemType()->GetInfo(pItemChip->GetTypeID(), &info);
// 	int nMaxChipSpirit = info.nLife;
// 		
// 	int	nLife	= pItemChip->GetInt(ITEMDATA_LIFE);
// 
// 
// 	int	nMaxImportSpiritStoneLevel	= 3;
// 
// 	CItem* pItemSpirit[5];
// 	int nSpiritCount = 0;
// 	for(int i = 1; i < nSize; i++)
// 	{
// 		pItemSpirit[nSpiritCount] = NULL;
// 		pItemSpirit[nSpiritCount] = this->GetItem(pMaterial[i]);
// 		if(!pItemSpirit[nSpiritCount])
// 		{
// 			continue;
// 		}
// 
// 		if(!pItemSpirit[nSpiritCount]->IsSpiritStone())
// 		{
// 			sprintf(szError, g_objStrRes.GetStr(17020));//"有非魔晶物品存在!"
// 			return FALSE;
// 		}
// 
// 		if(CItem::GetSpiritStoneLev(pItemSpirit[nSpiritCount]->GetTypeID()) > nMaxImportSpiritStoneLevel)
// 		{
// 			sprintf(szError, g_objStrRes.GetStr(17021));//"3级以上的魔晶蕴含能量过于强大，无法注入!"
// 			return FALSE;
// 		}
// 		nSpiritCount++;
// 	}
// 
// 	if(nSpiritCount < 1)
// 	{
// 		sprintf(szError, g_objStrRes.GetStr(17022));//"魔晶不存在!"
// 		return FALSE;
// 	}		
// 
// 	int nImportSpirit = 0;
// 	
// 	int	nLev1Spirit			=	10000;
// 	int	nLev2Spirit			=	nLev1Spirit * 6;
// 	int	nLev3Spirit			=	nLev2Spirit * 6;	
// 	/*
// 	int nNormalChipConsume		=	40;
// 	int nAdvChipConsume			=	20;
// 	int nChipConsume				=	nNormalChipConsume;
// 
// 	
// 	if(pItemChip->IsAdvancedSpiritChip())
// 		nChipConsume = nAdvChipConsume;
// 	*/
// 	int nChipConsume;
// 	if(info.usEmbedPos > 100 || info.usEmbedPos < 0)
// 		nChipConsume = 40;
// 	else
// 		nChipConsume = info.usEmbedPos;
// 	
// 
// 	int nLev1SpiritReduseConsume	=	0;
// 	int nLev2SpiritReduseConsume	=	10;
// 	int nLev3SpiritReduseConsume	=	20;
// 
// 	for(i = 0; i <nSpiritCount; i++)
// 	{
// 		int nLev = CItem::GetSpiritStoneLev(pItemSpirit[i]->GetTypeID());
// 
// 		int nFinalConsume = nChipConsume;
// 		int	nSpirit = 0;
// 		switch(nLev)
// 		{
// 		case 1:
// 			nFinalConsume = nChipConsume;
// 			nSpirit = nLev1Spirit;
// 			break;
// 		case 2:
// 			nFinalConsume = __max(0, nChipConsume - nLev2SpiritReduseConsume);
// 			nSpirit = nLev2Spirit;
// 			break;
// 		case 3:
// 			nFinalConsume = __max(0, nChipConsume - nLev3SpiritReduseConsume);
// 			nSpirit = nLev3Spirit;
// 			break;
// 		default :
// 			break;
// 		}
//  
// 		EraseItem(pItemSpirit[i]->GetID(), TRUE);
// 		int nTemp = nSpirit * (100 - nFinalConsume) / 100;
// 		nImportSpirit += nTemp;
// 	}
// 
// 	if(nImportSpirit > 0)
// 	{
// 		int nNewChipSpirit = nImportSpirit + nCurrentChipSpirit;
// 		if(nNewChipSpirit > nMaxChipSpirit)
// 			nNewChipSpirit = nMaxChipSpirit;
// 
// 		pItemChip->SetInt( ITEMDATA_AMOUNT, nNewChipSpirit, TRUE);
// 		
// 		CMsgItem	msg;
// 		IF_OK(msg.Create(pItemChip->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItemChip->GetInt(ITEMDATA_AMOUNT)))
// 		SendMsg(&msg);
// 	}
// 	
// 	return TRUE;
// }

//////////////////////////////////////////////////////////////////////////
//  [2007-8-10] 朱斌 吃魔导芯片
// BOOL CUser::SpendSpiritChip(CItem *pItem, int nPosition/* = ITEMPOSITION_BACKPACK1*/, bool bSynchro/* = true*/)
// {
// 	CHECKF(pItem);
// 
// 	if(!pItem->IsAdvancedSpiritChip() && !pItem->IsSpiritChip())
// 		return FALSE;
// 	
// 	int nCurrentSpirit  = pItem->GetInt( ITEMDATA_AMOUNT );
// 	if( 0 == nCurrentSpirit)
// 		return FALSE;
// 
// 		
// 	int	nLife	= pItem->GetInt(ITEMDATA_LIFE);
// 	if( 0 == nLife )
// 		return FALSE;
// 	int nTypeID		= pItem->GetInt( ITEMDATA_TYPE );
// 	
// 
// // 	if(nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition <= ITEMPOSITION_PACK_END)
// // 	{
// // 		int nMaxSpendSpirit = 10000;
// // 		int nSpendSpirit = __min(nCurrentSpirit, nMaxSpendSpirit);	//	得本次要充入的魄
// // 		if(!AddSpirit(nSpendSpirit))
// // 			return FALSE;
// // 		int nLeftSpirit = __max(0, nCurrentSpirit - nSpendSpirit);
// // 		pItem->SetInt(ITEMDATA_AMOUNT, nLeftSpirit);
// // 	}
// 	CMsgItem	msg;
// 	IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
// 		SendMsg(&msg);
// 
// 	UpdateWeight();
// 	return TRUE;
// }

//////////////////////////////////////////////////////////////////////////
//  [2007-8-10] 朱斌 加魄
// BOOL CUser::AddSpirit(int nSpirit)
// {
// 	if(nSpirit < 0)
// 		return false;
// 	int nOldSpiritGridNum	= this->GetSpiritGridNum();		//	本来魂格的数量
// 	int nOldSpiritPercent	= this->GetSpiritPercent();		//	本来魂格的百分比
// 
// 	CHECKF( nOldSpiritPercent < SPIRITPERCENT2GRID );
// 	
// 	int nOldSpiritValue		= SPIRITPERCENT2GRID * nOldSpiritGridNum + nOldSpiritPercent;//换算成值
// 
// 	int nNowSpiritValue = nOldSpiritValue + nSpirit; //加上本次
// 	
// 	int nNowSpiritGridNum	=	nNowSpiritValue / SPIRITPERCENT2GRID;
// 	int nTemp = nNowSpiritGridNum;	//缓存
// 
// 	int nNowSpiritPercent	=	nNowSpiritValue % SPIRITPERCENT2GRID;
// 
// 	int nGridCheck	= __min( this->GetSpiritGridMaxNum(), SPIRITGRID_NUMLIMIT );	//	检查条件，取小值
// 
// 	nNowSpiritGridNum = __min( nNowSpiritGridNum, nGridCheck );
// 
// 	nNowSpiritPercent = nNowSpiritPercent + ( nTemp - nNowSpiritGridNum ) * SPIRITPERCENT2GRID;
// 	nNowSpiritPercent	= __min( nNowSpiritPercent, SPIRITPERCENT2GRID - 1);
// 
// 		
// 
// 	bool bSave = false;
// 	if( nOldSpiritPercent / 10 != nNowSpiritPercent / 10 )// 十分之一存一次
// 	{
// 		bSave = true;
// 	}
// 	this->SetAttrib( _USERATTRIB_SPIRIT_PERCENT, nNowSpiritPercent, SYNCHRO_TRUE );//同步客户端
// 	
// 	if( nOldSpiritGridNum != nNowSpiritGridNum )//魂格改变，必须存，广播
// 	{
// 		bSave = true;
// 		this->SetAttrib( _USERATTRIB_SPIRITGRID_NUM, nNowSpiritGridNum, SYNCHRO_BROADCAST );//在ROOM中广播
// 
// 	}
// 	if( bSave )
// 		m_data.SaveInfo();// CUser是CUserData的友元类,所以可以操作保护函数 
// 	return true;
// }

void CUser::AwardHonor(int nHonor, BOOL bUpdate, int nSynchro)
{
	return;
// 	//	加HONOR
// 	int nNewHonor = m_data.GetHonor() + nHonor;
// 	nNewHonor = __max(0, nNewHonor);
// 
// 	m_data.SetHonor(nNewHonor, bUpdate);
// 
// 	if(nSynchro)
// 	{
// 		CMsgUserAttrib	msg;
// 		if(!msg.Create(GetID(), _USERATTRIB_HONOR, m_data.GetHonor()))
// 			return;
// 		this->SendMsg(&msg);	//	仅发给自己	
// 		
// 	}
// 
// 	if(this->GetHonor()	>= 50000 && this->GetSynRank() < RANK_OLD)
// 		this->QuerySynAttr()->SetRank(RANK_OLD, true);
// 	else if(this->GetHonor() < 50000 && this->GetSynRank() == RANK_OLD)
// 		this->QuerySynAttr()->SetRank(RANK_SPERNORMAL, true);
// 
// 	if(this->GetHonor()	>= 10000 && this->GetSynRank() < RANK_SPERNORMAL)
// 		this->QuerySynAttr()->SetRank(RANK_SPERNORMAL, true);
// 	else if(this->GetHonor() < 10000 && this->GetSynRank() == RANK_SPERNORMAL)
// 		this->QuerySynAttr()->SetRank(RANK_NORMAL, true);
}

// BOOL CUser::IsChangeUserName()
// {
// 	IDatabase* pDatabase= Database();
// 	CDataInspectManage DataInspectManager;
// 	return DataInspectManager.IsChangeUserName(  pDatabase,this->GetID() );
// }
// 
// BOOL CUser::IsChangeSynName()
// {
// 	IDatabase* pDatabase= Database();
// 	CDataInspectManage DataInspectManager;
// 	return DataInspectManager.IsChangeSynName(  pDatabase,this->GetID() );
// }
// 
// BOOL CUser::IsChangeHouseName()
// {
// 	IDatabase* pDatabase= Database();
// 	CDataInspectManage DataInspectManager;
// 	return DataInspectManager.IsChangeHouseName(  pDatabase,this->GetID() );
// }
// 
// void CUser::SetChangeUserNameOK()
// {
// 	IDatabase* pDatabase= Database();
// 	CDataInspectManage DataInspectManager;
// 	DataInspectManager.SetChangeUserNameOK(  pDatabase,this->GetID() );
// }
// 
// void CUser::SetChangeSynNameOK()
// {
// 	IDatabase* pDatabase= Database();
// 	CDataInspectManage DataInspectManager;
// 	DataInspectManager.SetChangeSynNameOK(  pDatabase,this->GetID() );
// }
// 
// void CUser::SetChangeHouseNameOK()
// {
// 	IDatabase* pDatabase= Database();
// 	CDataInspectManage DataInspectManager;
// 	DataInspectManager.SetChangeHouseNameOK(  pDatabase,this->GetID() );
// }

// bool CUser::FlushCityWarSynName(OBJID idSyn, const char* pName)
// {
// 	return CityWarMgr()->ProcessChangeSynName(idSyn, pName);
// }
// 
// void CUser::ChangHouseOwnerName(const char* pName)
// {
// 	CHouse* pHouse = HouseManager()->QueryHouseByOwnerID(GetID());
// 	if(pHouse)
// 	{
// 		HouseManager()->ProcessChgOwnerName(pHouse->GetID(), pName);
// 	}
// }
// 
// BOOL CUser::RequestChangeUserName(char * name )
// {
// 	IDatabase* pDatabase= Database();
// 	CDataInspectManage DataInspectManager;
// 	return DataInspectManager.RequestChangeUserName(  pDatabase,this->GetID(), name );
// }
// 
// BOOL CUser::RequestChangeSynName(char * name )
// {
// 	if(!name)
// 		return false;
// 
// 	if( SynManager()->QuerySynByName(name) )
// 		return false;
// 	if (strlen(name) < 2 || strlen(name) > 15 || !::NameStrCheck((char*)name))
// 	{
// 		this->SendSysMsg(g_objStrRes.GetStr(11095));//STR_INVALID_GUILD_NAME
// 		return false;
// 	}
// 	OBJID idSyn = this->GetSynID();
// 	if (idSyn == ID_NONE)
// 		return false;
// 
// 	if (this->GetSynRank() != RANK_LEADER)		// 只有本帮帮主才允许改名
// 	{
// 		this->SendSysMsg(g_objStrRes.GetStr(11097));//STR_NOT_AUTHORIZED
// 		return false;
// 	}
// 
// 	CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// 	if( !pSyn)
// 		return false;
// 
// 	return pSyn->ReqSetSynName(name);//军团改名
// }
// 
// BOOL CUser::RequestChangeHouseName(const char * name )
// {
// 	if(!name)
// 		return false;
// 
// 	CHouse *pHouse = HouseManager()->QueryHouseByOwnerID(GetID());
// 	
// 	if(!pHouse)
// 		return false;
// 
// 	if(HouseManager()->QueryHouseByName(name))
// 		return false;
// 
// 	if (strlen(name) < 2 || strlen(name) > 15 || !::NameStrCheck((char*)name))
// 	{
// 		this->SendSysMsg(g_objStrRes.GetStr(11095));//STR_INVALID_GUILD_NAME
// 		return false;
// 	}
// 
// 	CreateHouseInfo info;
// 	memset(&info, 0, sizeof(CreateHouseInfo));
// 	pHouse->GetHouseInfo(&info);
// 	strcpy(info.szName, name);
// 	HouseManager()->ProcessHouseUpdate(&info, true);
// 
// 	return true;
// }

void CUser::SetApplyName(int nApply, const char* pszName)
{ 
	m_nApplyByName = nApply; 
	if(pszName == NULL)
		memset(m_szApplyName, 0, _MAX_NAMESIZE);
	else
		strcpy(m_szApplyName, pszName); 
}

bool CUser::FetchApplyName(int nApply, const char* pszName)
{
	if(m_nApplyByName == nApply && strcmp(pszName, m_szApplyName) == 0) 
	{ 
		m_nApplyByName = APPLY_NONE; 
		return true; 
	} 
	return false; 
}

void CUser::AddRemoteTeamMember(ST_TEAM_INVITE* pTeamInvite, BOOL bIsJoin)
{
	return;

// 	OBJID idAddMember;
// 	char* pszAddMemberName;
// 	SOCKET_ID	idAddMemberSocket;
// 	if(bIsJoin)
// 	{
// 		idAddMember = pTeamInvite->idPlayer;
// 		pszAddMemberName = pTeamInvite->szPlayerName;
// 		idAddMemberSocket = pTeamInvite->idSocket;
// 	}
// 	else
// 	{
// 		idAddMember = pTeamInvite->idTarget;
// 		pszAddMemberName = pTeamInvite->szTargetName;
// 		idAddMemberSocket = pTeamInvite->idSocketTarget;
// 	}
// 
// 	CTeam* pTeam = GetTeam();
// 	CHECK(pTeam);
// 
// 	int nMemberCount = pTeam->GetMemberAmount();
// 	for(int i = 0; i < nMemberCount; i++)
// 	{
// 		OBJID idMember = pTeam->GetMemberByIndex(i);
// 		CUserPtr pMember = UserManager()->GetUser(idMember);
// 		if(pMember)	//	在地图组
// 		{
// 			CTeam* pTeamTemp = pMember->GetTeam();
// 			if(pTeamTemp)
// 				pTeamTemp->AddMemberOnly(idAddMember);	//	只加入新成员ID
// 		}
// 		else	//	不在, 广播组其它地图组新加
// 		{
// 			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_INFO, pTeam->GetID(), idAddMember, idAddMemberSocket);
// 		}
//	}
	
	//	通知新成员创建队员
//	ST_TEAM_INVITE st;
	
//	memcpy(&st, pTeamInvite, sizeof(ST_TEAM_INVITE));
	//	CUserPtr pTarget = UserManager()->GetUser(pTeamInvite->szTargetName);
	//	CHECK(pTarget);				
	
//	CTeam* pTeam = pUser->GetTeam();
// 	int nInMapGroupMemberCount = 0;
// 	BOOL	bNotInMapGroup = FALSE;	//	有队员不在此地图组
// 	BOOL	bMemberHasUser = FALSE;
// 	for(i = 0; i < nMemberCount; i++)
// 	{
// 		OBJID idMember = pTeam->GetMemberByIndex(i);
// 		CUserPtr pMember = UserManager()->GetUser(idMember);
// 		if(pMember)	//	在地图组
// 		{
// 			st.teamMemberInfo[nInMapGroupMemberCount].id			= pMember->GetID();
// 			st.teamMemberInfo[nInMapGroupMemberCount].dwLookFace	= pMember->GetLookFace();
// 			st.teamMemberInfo[nInMapGroupMemberCount].dwLev         = pMember->GetLev();
// 			st.teamMemberInfo[nInMapGroupMemberCount].usHp			= pMember->GetLife();
// 			st.teamMemberInfo[nInMapGroupMemberCount].usMaxHp		= pMember->GetMaxLife();
// 			st.teamMemberInfo[nInMapGroupMemberCount].usMp			= pMember->GetMana();
// 			st.teamMemberInfo[nInMapGroupMemberCount].usMaxMp		= pMember->GetMaxMana();
// 			st.teamMemberInfo[nInMapGroupMemberCount].usProfession  = pMember->GetProfession();
// 			st.teamMemberInfo[nInMapGroupMemberCount].usPosX		= pMember->GetPosX();
// 			st.teamMemberInfo[nInMapGroupMemberCount].usPosY		= pMember->GetPosX();
// 			strcpy(st.teamMemberInfo[nInMapGroupMemberCount].szName, pMember->GetName());
// 			nInMapGroupMemberCount++;
// 		}
// 		else	//	不在, 广播组其它地图组新加
// 		{
// 			bNotInMapGroup = TRUE;
// 		}
// 
// 		if(idMember == GetID())
// 			bMemberHasUser = TRUE;
// 		
// 		st.teamInfo.setMember[i] = idMember;
// 	}

	//  [2007-9-27] 朱斌 如果队员信息里没有自己,加入
	/*
	if(!bMemberHasUser && nInMapGroupMemberCount < 4)
	{
		st.teamMemberInfo[nInMapGroupMemberCount].id			= GetID();
		st.teamMemberInfo[nInMapGroupMemberCount].dwLookFace	= GetLookFace();
		st.teamMemberInfo[nInMapGroupMemberCount].usHp			= GetLife();
		st.teamMemberInfo[nInMapGroupMemberCount].usMaxHp		= GetMaxLife();
		st.teamMemberInfo[nInMapGroupMemberCount].usMp			= GetMana();
		st.teamMemberInfo[nInMapGroupMemberCount].usMaxMp		= GetMaxMana();
		st.teamMemberInfo[nInMapGroupMemberCount].usPosX		= GetPosX();
		st.teamMemberInfo[nInMapGroupMemberCount].usPosY		= GetPosX();
		strcpy(st.teamMemberInfo[nInMapGroupMemberCount].szName, GetName());
		nInMapGroupMemberCount++;
	}
	*/
	
//	CHECK(nInMapGroupMemberCount < 5);	//	已经存在的成员不能超过4个
	
	//  [2007-9-27] 朱斌	自己不用发送
	//	新成员自己的信息
	/*
	st.teamMemberInfo[nInMapGroupMemberCount].id			= pTeamInvite->teamMemberInfo[0].id;
	st.teamMemberInfo[nInMapGroupMemberCount].dwLookFace	= pTeamInvite->teamMemberInfo[0].dwLookFace;
	st.teamMemberInfo[nInMapGroupMemberCount].usHp			= pTeamInvite->teamMemberInfo[0].usHp;
	st.teamMemberInfo[nInMapGroupMemberCount].usMaxHp		= pTeamInvite->teamMemberInfo[0].usMaxHp;
	st.teamMemberInfo[nInMapGroupMemberCount].usMp			= pTeamInvite->teamMemberInfo[0].usMp;
	st.teamMemberInfo[nInMapGroupMemberCount].usMaxMp		= pTeamInvite->teamMemberInfo[0].usMaxMp;
	st.teamMemberInfo[nInMapGroupMemberCount].usPosX		= pTeamInvite->teamMemberInfo[0].usPosX;
	st.teamMemberInfo[nInMapGroupMemberCount].usPosY		= pTeamInvite->teamMemberInfo[0].usPosY;
	strcpy(st.teamMemberInfo[nInMapGroupMemberCount].szName, pTeamInvite->teamMemberInfo[0].szName);
	nInMapGroupMemberCount++;
	*/
	//	通知新成员加入成功
// 	if(bIsJoin)
// 	{
// 		st.nMemberInfoCount = nInMapGroupMemberCount;
// 		st.unAction = TEAM_INVITE_ACTION_TYPE_JOIN_FINAL;		
// 	}
// 	else
// 	{
// 		st.nMemberInfoCount = nInMapGroupMemberCount;
// 		st.unAction = TEAM_INVITE_ACTION_TYPE_INVITE_ANK_RETURN;
// 	}
// 	
// 	//	更新TEAMINFO
// 	TeamInfoStruct	teamInfo;
// 	pTeam->GetInfo(&teamInfo);
// 	st.teamInfo.id = teamInfo.id;
// 	st.teamInfo.idLeader = teamInfo.idLeader;
// 	st.teamInfo.bCloseMoney = teamInfo.bCloseMoney;
// 	st.teamInfo.bCloseItem = teamInfo.bCloseItem;
// 	st.teamInfo.nTeamMode = teamInfo.nTeamMode;
// 	st.teamInfo.nExpMode = teamInfo.nExpMode;
// 	st.teamInfo.nItemMode = teamInfo.nItemMode;
// 	memcpy(st.teamInfo.setMember, teamInfo.setMember, sizeof(teamInfo.setMember));
// 	strcpy(st.teamInfo.sTeamName, teamInfo.sTeamName);
// 	st.teamInfo.m_nLev = teamInfo.m_nLev;
// 	st.teamInfo.m_nTarget = teamInfo.m_nTarget;
// 	strcpy(st.teamInfo.m_sLeadName, teamInfo.m_sLeadName);
// 	strcpy(st.teamInfo.m_sTeamText, teamInfo.m_sTeamText);
// 	MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);	
// 	
// 	
// 	if(bNotInMapGroup)	//	通知不在的更新
// 	{
// 		MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_ADD, pTeam->GetID(), idAddMember, idAddMemberSocket);
// 		MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_INFO, pTeam->GetID(), idAddMember, idAddMemberSocket);
// 	}
// 	
// 	//	小队广播,发送新成员的信息到每个成员
// 	CMsgTeamMember msg;
// 	TeamMemberInfo teamMemberInfo;
// 	teamMemberInfo.id = pTeamInvite->teamMemberInfo[0].id;
// 	teamMemberInfo.dwLookFace	= pTeamInvite->teamMemberInfo[0].dwLookFace;
// 	teamMemberInfo.dwLev        = pTeamInvite->teamMemberInfo[0].dwLev;
// 	teamMemberInfo.usHp			= pTeamInvite->teamMemberInfo[0].usHp;
// 	teamMemberInfo.usMaxHp		= pTeamInvite->teamMemberInfo[0].usMaxHp;
// 	teamMemberInfo.usMp			= pTeamInvite->teamMemberInfo[0].usMp;
// 	teamMemberInfo.usMaxMp		= pTeamInvite->teamMemberInfo[0].usMaxMp;
// 	teamMemberInfo.usPosX		= pTeamInvite->teamMemberInfo[0].usPosX;
// 	teamMemberInfo.usPosY		= pTeamInvite->teamMemberInfo[0].usPosY;
//     teamMemberInfo.usMapID      = pTeamInvite->teamMemberInfo[0].usMapID;
// 	teamMemberInfo.usProfession = pTeamInvite->teamMemberInfo[0].usProfession;
// 	strcpy(teamMemberInfo.szName, pTeamInvite->teamMemberInfo[0].szName);
// 	IF_NOT(msg.Create(_MSG_TEAMMEMBER_ADDMEMBER, teamMemberInfo))
// 		return;
// 	
// 	pTeam->BroadcastTeamMsg(&msg);
}

//  [2007-9-14] 朱斌	检查装备是否激活,未激活激活
void CUser::ActivateItem(CItemPtr pItem)
{
	CHECK(pItem);
	
	if(pItem->IsActivatePickup() || pItem->IsActivateEquip())
	{
		if(pItem->GetInt(ITEMDATA_NEEDCHECK) == 1)
			return;

		CTime time = CTime::GetCurrentTime();
		CTimeSpan timeSpan(0,0,pItem->GetInt(ITEMDATA_INIT_LIFE_TIME),0);
		CTime timeEnd = time + timeSpan;

 		pItem->SetInt(ITEMDATA_LIFE_TIME, (timeEnd.GetYear() - 2000) * 100000000 + timeEnd.GetMonth() * 1000000  + timeEnd.GetDay() * 10000+ timeEnd.GetHour() * 100+ timeEnd.GetMinute() );
		pItem->SetInt(ITEMDATA_NEEDCHECK,1);

// 		CMsgItemAttrib msg;
// 		if(msg.Create(pItem->GetID(), _ITEMATTRIB_IS_ACTIVATE, (DWORD)TRUE))
// 		{
// 			msg.Append(_ITEMATTRIB_LIFE_TIME, (DWORD)pItem->GetLifeTime());
// 			this->SendMsg(&msg);
// 		}
	}
}

void CUser::CheckItemLifeTime(BOOL bUpdateToClient)
{
	if(!CanBeSee())
		return;

	if( !m_tItemLifeTime.ToNextTime() )
	{
		return;				
	}
//	clock_t tStart = clock();

	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		CItemPtr pItem = GetEquipItemByPos(i);
		if(pItem == NULL)
			continue;

		DEBUG_TRY
		if(pItem->GetPosition() != i)
		{
			LOGWARNING("检查装备的时候装备不对劲，要检查 itemid = %u itemtype = %u userid = %u oldPosition = %u checkPosition = %u",pItem->GetID(),pItem->GetTypeID(),GetID(),pItem->GetPosition(),i);
			DEBUG_TRY
			pItem->SetPosition(i);
			m_pPackage->PopItem(pItem->GetID());
			DEBUG_CATCH("检查装备的时候装备不对劲，还原出错");
		}


		CheckItemLifeTime(pItem, bUpdateToClient, true);

// 		CItemPtr pItemEnd = GetEquipItemByPos(i);
// 		if(pItemEnd)
// 		{
// 			if(pItem->GetPosition() != i)
// 			{
// 				LOGWARNING("检查完装备的时候装备不对劲，要检查 itemid = %u userid = %u oldPosition = %u checkPosition = %u",pItem->GetID(),GetID(),pItem->GetPosition(),i);
// 				pItem->SetPosition(i);
// 			}
// 		}
// 		else
// 			LOGWARNING("装备检查时通过,装备消失 position = %u userid = %u",i,GetID());

		DEBUG_CATCH("检查装备的时候装备不对劲")
	}

	DEBUG_TRY
	m_pPackage->CheckItemLifeTime(bUpdateToClient, true);
	DEBUG_CATCH("m_pPackage->CheckItemLifeTime()");
// 
// 	//  [3/5/2008 朱斌]	仓库不检查,因为仓库里的物品删除时删除不掉(EraseItem是删除背包里物品)
// 	//	仓库里的物品在拿出时检查 在CMsgPackage里
// 	//	仓库
// // 	if(m_pStorage)
// // 		m_pStorage->CheckItemLifeTime(this, bUpdateToClient, bUpdateDB);
// 
// 	InterlockedExchangeAdd(&g_stat.setDebug[13], clock() - tStart);	//	总用时
}

float CUser::CalItemAddExpTeamFactor()
{
	float fResult=0.0;
	DEBUG_TRY
	CTeam* pTeam=GetTeam();
	if(IsVipActive())//如果玩家也是VIP时也加成
	{
		int nVipLev = GetVipLev()==4 ? 1 : GetVipLev();//周卡要和月卡效果一样
		fResult+=nVipLev*0.1+0.1;///VIP玩家根据VIP等级进行累加,普通玩家为0.1，月卡玩家0.2，季卡玩家0.3，半年卡玩家0.4
	}
    else
        fResult+=0.1f;///普通玩家也有0.1的加成
	if(pTeam!=NULL)///只有玩家有队时才进行处理
	{
		CUser* pMember=NULL;
		for(int i = 0 ;i<_MAX_TEAMAMOUNT ; i++)
		{
			///为空的队员不处理
			if(pTeam->m_TeamIn[i].id ==ID_NONE)
				continue;
			if(pTeam->m_TeamIn[i].id ==GetID())///忽略掉buff所有者,因为他已经加成过了
				continue;
			pMember=UserManager()->GetUser(pTeam->m_TeamIn[i].id);
			if(pMember==NULL)///如果队员不在相同地图组则不处理
				continue;
			if(pMember->GetMapID()!=GetMapID())///不在buff所有者相同地图的队员不计算效果
				continue;
			if(pMember->QueryStatus(STATUS_ITEM_ADDEXP)==NULL)///不带buff的队员不计算效果
				continue;

			if(pMember->IsVipActive()==false)///普通玩家0.1
				fResult+=0.1f;
			else
			{
				int nVipLev = pMember->GetVipLev()==4 ? 1 : pMember->GetVipLev();//周卡要和月卡效果一样
				fResult+=nVipLev*0.1+0.1;///VIP玩家根据VIP等级进行累加,普通玩家为0.1，月卡玩家0.2，季卡玩家0.3，半年卡玩家0.4
			}
		}
	}
// 	char szWords[128] = "";
// 	sprintf(szWords, "经验加成系数%f",fResult);
// 	UserManager()->BroadCastMsgAndReceiveName(szWords, /*pUser*/NULL, 0, 0xffffff, _TXTATR_WORLDSERVER,this,0,0,"",ALLUSERS_NAME);
	DEBUG_CATCH2("CalItemAddExpTeamFactor crash!userid:%d",GetID());
	return fResult;
}

void CUser::UpdateLastLogout()
{
	m_data.SetLastLogout();
}

void CUser::UpdateAllOnlineTime()//-更新在线总时长//现在用作存储下线时间
{
	// ------------------------begin-----------------下线更新总在线时长-------------------------------- [9/12/2007 %PENGFENG%]
		int nAllOnlineTime = this->GetOnlineTime();
		this->SetAllOnlineTime(nAllOnlineTime);
	//---------------------------------end----------------------------------------------------------------
}

OBJID CUser::GetOnlineTime()//-得到本次在线时长//单位分钟
{
		time_t timeEnd;
		time(&timeEnd);
		OBJID nOnlineTimeMinutes = timeEnd;
		return nOnlineTimeMinutes;
}

BOOL CUser::ReformPoint(int nType,int data)
{
	switch(nType)
	{
	case 0://全洗
		{
			int nData1 = GetStrUserOnly();
			int nData2 = GetIntUserOnly();
			int nData3 = GetStaUserOnly();
			int nData4 = GetSpiUserOnly();
			int nData5 = GetAgiUserOnly();
			ReSetUserPoint(GetCurJob());

			int nAddData1 = nData1 - GetStrUserOnly();
			int nAddData2 = nData2 - GetIntUserOnly();
			int nAddData3 = nData3 - GetStaUserOnly();
			int nAddData4 = nData4 - GetSpiUserOnly();
			int nAddData5 = nData5 - GetAgiUserOnly();

			if(nAddData1 == 0 
				&&nAddData2 == 0
				&&nAddData3 == 0
				&&nAddData4 == 0
				&&nAddData5 == 0)
				return false;
// 			m_data.AddPot(5 * GetJobLev(GetCurJob()),GetCurJob());
// 			m_data.SaveInfo();

			CMsgUserAttrib msg;
			if(GetCurJob() == 1)
				msg.Create(GetID(),_USERATTRIB_ADDPOINT,m_data.GetPot(GetCurJob()));
			else
				msg.Create(GetID(),_USERATTRIB_ADDPOINT_SEC,m_data.GetPot(GetCurJob()));
			SendMsg(&msg);
		}
		break;
	case 1://属性1
		{
			int nData1 = GetStrUserOnly();
			int nData2 = GetIntUserOnly();
			int nData3 = GetStaUserOnly();
			int nData4 = GetSpiUserOnly();
			int nData5 = GetAgiUserOnly();
			int nOldAdd = GetAddPoint(GetCurJob());
			ReSetUserPoint(GetCurJob());
			int nDataNow = GetStrUserOnly();

			int nChgData = min(nData1 - nDataNow,data);

			int nAddData1 = nData1 - GetStrUserOnly() - nChgData;
			int nAddData2 = nData2 - GetIntUserOnly();
			int nAddData3 = nData3 - GetStaUserOnly();
			int nAddData4 = nData4 - GetSpiUserOnly();
			int nAddData5 = nData5 - GetAgiUserOnly();

			m_data.AddStr(nAddData1 * 100);
			m_data.AddIntex(nAddData2 * 100);
			m_data.AddSta(nAddData3 * 100);
			m_data.AddSpi(nAddData4 * 100);
			m_data.AddAgi(nAddData5 * 100);
			m_data.SetPot(nOldAdd + nChgData,GetCurJob());
//			m_data.SaveInfo();
			//goto 存过数据以后就把增量清0，减少写数据库次数
			m_MoneyIncr = 0;
			m_ExpIncr = 0;

			if(nChgData <= 0)
				return false;
			
			CMsgUserAttrib msg;
			if(GetCurJob() == 1)
				msg.Create(GetID(),_USERATTRIB_ADDPOINT,m_data.GetPot(GetCurJob()));
			else
				msg.Create(GetID(),_USERATTRIB_ADDPOINT_SEC,m_data.GetPot(GetCurJob()));
			SendMsg(&msg);
		}
		break;
	case 2://属性2
		{
			int nData1 = GetStrUserOnly();
			int nData2 = GetIntUserOnly();
			int nData3 = GetStaUserOnly();
			int nData4 = GetSpiUserOnly();
			int nData5 = GetAgiUserOnly();
			int nOldAdd = GetAddPoint(GetCurJob());
			ReSetUserPoint(GetCurJob());
			int nDataNow = GetIntUserOnly();
			
			int nChgData = min(nData2 - nDataNow,data);
			
			int nAddData1 = nData1 - GetStrUserOnly();
			int nAddData2 = nData2 - GetIntUserOnly() - nChgData;
			int nAddData3 = nData3 - GetStaUserOnly();
			int nAddData4 = nData4 - GetSpiUserOnly();
			int nAddData5 = nData5 - GetAgiUserOnly();
			
			m_data.AddStr(nAddData1 * 100);
			m_data.AddIntex(nAddData2 * 100);
			m_data.AddSta(nAddData3 * 100);
			m_data.AddSpi(nAddData4 * 100);
			m_data.AddAgi(nAddData5 * 100);
			m_data.SetPot(nOldAdd + nChgData,GetCurJob());
//			m_data.SaveInfo();
			//goto 存过数据以后就把增量清0，减少写数据库次数
			m_MoneyIncr = 0;
			m_ExpIncr = 0;

			if(nChgData <= 0)
				return false;
			
			CMsgUserAttrib msg;
			if(GetCurJob() == 1)
				msg.Create(GetID(),_USERATTRIB_ADDPOINT,m_data.GetPot(GetCurJob()));
			else
				msg.Create(GetID(),_USERATTRIB_ADDPOINT_SEC,m_data.GetPot(GetCurJob()));
			SendMsg(&msg);
		}
		break;
	case 3://属性3
		{
			int nData1 = GetStrUserOnly();
			int nData2 = GetIntUserOnly();
			int nData3 = GetStaUserOnly();
			int nData4 = GetSpiUserOnly();
			int nData5 = GetAgiUserOnly();
			int nOldAdd = GetAddPoint(GetCurJob());
			ReSetUserPoint(GetCurJob());
			int nDataNow = GetStaUserOnly();
			
			int nChgData = min(nData3 - nDataNow,data);
			
			int nAddData1 = nData1 - GetStrUserOnly();
			int nAddData2 = nData2 - GetIntUserOnly();
			int nAddData3 = nData3 - GetStaUserOnly() - nChgData;
			int nAddData4 = nData4 - GetSpiUserOnly();
			int nAddData5 = nData5 - GetAgiUserOnly();
			
			m_data.AddStr(nAddData1 * 100);
			m_data.AddIntex(nAddData2 * 100);
			m_data.AddSta(nAddData3 * 100);
			m_data.AddSpi(nAddData4 * 100);
			m_data.AddAgi(nAddData5 * 100);
			m_data.SetPot(nOldAdd + nChgData,GetCurJob());
//			m_data.SaveInfo();
			//goto 存过数据以后就把增量清0，减少写数据库次数
			m_MoneyIncr = 0;
			m_ExpIncr = 0;

			if(nChgData <= 0)
				return false;
			
			CMsgUserAttrib msg;
			if(GetCurJob() == 1)
				msg.Create(GetID(),_USERATTRIB_ADDPOINT,m_data.GetPot(GetCurJob()));
			else
				msg.Create(GetID(),_USERATTRIB_ADDPOINT_SEC,m_data.GetPot(GetCurJob()));
			SendMsg(&msg);
		}
		break;
	case 4://属性4
		{
			int nData1 = GetStrUserOnly();
			int nData2 = GetIntUserOnly();
			int nData3 = GetStaUserOnly();
			int nData4 = GetSpiUserOnly();
			int nData5 = GetAgiUserOnly();
			int nOldAdd = GetAddPoint(GetCurJob());
			ReSetUserPoint(GetCurJob());
			int nDataNow = GetSpiUserOnly();
			
			int nChgData = min(nData4 - nDataNow,data);
			
			int nAddData1 = nData1 - GetStrUserOnly();
			int nAddData2 = nData2 - GetIntUserOnly();
			int nAddData3 = nData3 - GetStaUserOnly();
			int nAddData4 = nData4 - GetSpiUserOnly() - nChgData;
			int nAddData5 = nData5 - GetAgiUserOnly();
			
			m_data.AddStr(nAddData1 * 100);
			m_data.AddIntex(nAddData2 * 100);
			m_data.AddSta(nAddData3 * 100);
			m_data.AddSpi(nAddData4 * 100);
			m_data.AddAgi(nAddData5 * 100);
			m_data.SetPot(nOldAdd + nChgData,GetCurJob());
//			m_data.SaveInfo();
			//goto 存过数据以后就把增量清0，减少写数据库次数
			m_MoneyIncr = 0;
			m_ExpIncr = 0;

			if(nChgData <= 0)
				return false;
			
			CMsgUserAttrib msg;
			if(GetCurJob() == 1)
				msg.Create(GetID(),_USERATTRIB_ADDPOINT,m_data.GetPot(GetCurJob()));
			else
				msg.Create(GetID(),_USERATTRIB_ADDPOINT_SEC,m_data.GetPot(GetCurJob()));
			SendMsg(&msg);
		}
		break;
	case 5://属性5
		{
			int nData1 = GetStrUserOnly();
			int nData2 = GetIntUserOnly();
			int nData3 = GetStaUserOnly();
			int nData4 = GetSpiUserOnly();
			int nData5 = GetAgiUserOnly();
			int nOldAdd = GetAddPoint(GetCurJob());
			ReSetUserPoint(GetCurJob());
			int nDataNow = GetAgiUserOnly();
			
			int nChgData = min(nData5 - nDataNow,data);
			
			int nAddData1 = nData1 - GetStrUserOnly();
			int nAddData2 = nData2 - GetIntUserOnly();
			int nAddData3 = nData3 - GetStaUserOnly();
			int nAddData4 = nData4 - GetSpiUserOnly();
			int nAddData5 = nData5 - GetAgiUserOnly() - nChgData;
			
			m_data.AddStr(nAddData1 * 100);
			m_data.AddIntex(nAddData2 * 100);
			m_data.AddSta(nAddData3 * 100);
			m_data.AddSpi(nAddData4 * 100);
			m_data.AddAgi(nAddData5 * 100);
			m_data.SetPot(nOldAdd + nChgData,GetCurJob());
//			m_data.SaveInfo();
			//goto 存过数据以后就把增量清0，减少写数据库次数
			m_MoneyIncr = 0;
			m_ExpIncr = 0;

			if(nChgData <= 0)
				return false;
			
			CMsgUserAttrib msg;
			if(GetCurJob() == 1)
				msg.Create(GetID(),_USERATTRIB_ADDPOINT,m_data.GetPot(GetCurJob()));
			else
				msg.Create(GetID(),_USERATTRIB_ADDPOINT_SEC,m_data.GetPot(GetCurJob()));
			SendMsg(&msg);
		}
		break;
	}
 	UpdateCalculatedProperty(UPDATECALTYPE_USER);
 	return true;
}

bool CUser::AddVipTime(DWORD dwMin)
{
	if(dwMin <= 0)
		return false;

	char UserName[256] = "";
	sprintf(UserName, "%d_[%s]_%d", SNAPSHOTUSER,this->GetName(), COLORYELLOW);
	if(IsVipActive()) //已经有
	{
		int nEndDate = m_data.GetVipTime();
		int nYear = 2000 + nEndDate / 100000000;
		int nMounth =  (nEndDate % 100000000) / 1000000;
		int nDay =  (nEndDate % 1000000) / 10000;
		int nHour =  (nEndDate % 10000) / 100;
		int nMin =  nEndDate % 100;
		CTime timeEndOld(nYear,nMounth,nDay,nHour,nMin,0);
		CTimeSpan timeSpan(0,0,dwMin,0);
		CTime timeThisEnd = timeEndOld + timeSpan;
		m_data.SetVipTime((timeThisEnd.GetYear() - 2000) * 100000000 + timeThisEnd.GetMonth() * 1000000  + timeThisEnd.GetDay() * 10000+ timeThisEnd.GetHour() * 100+ timeThisEnd.GetMinute() );
		m_data.ActiveVipTime();

		if(1440 == dwMin)
		{
			if(m_data.GetVipLev() < 1 || m_data.GetVipLev() > 4)
				m_data.SetVipLev(4);
		}
		if (10080 == dwMin)
		{
			if(m_data.GetVipLev() < 1 || m_data.GetVipLev() > 4)
				m_data.SetVipLev(4);
			
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>远见非凡，重金购买了<%s>使用，真是羡煞众人，令天下皆惊！", UserName,"3_VIP周卡_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
		}
		else if (43200 == dwMin)
		{
			if(m_data.GetVipLev() < 1 || m_data.GetVipLev() > 3)
				m_data.SetVipLev(1);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>远见非凡，重金购买了<%s>使用，真是羡煞众人，令天下皆惊！", UserName,"3_VIP月卡_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!4")
		}
		else if (129600 == dwMin)
		{
			if(m_data.GetVipLev() < 2 || m_data.GetVipLev() > 3)
				m_data.SetVipLev(2);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>远见非凡，重金购买了<%s>使用，万贯家财足以傲视群雄，冠绝江湖！", UserName,"3_VIP季卡_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!5")
		}
	    else if (259200 == dwMin)
		{
			if(m_data.GetVipLev() < 3 || m_data.GetVipLev() > 3)
				m_data.SetVipLev(3);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>远见非凡，重金购买了<%s>使用，卓然独立于天下，风范如君者，当今之世，能有几人？", UserName,"3_VIP半年卡_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!6")
		}

		CMsgPlayer msgPlayer;
		if (msgPlayer.Create(this->QueryRole()))
			this->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

		DEBUG_TRY
		if (IsVipActive())
		{
			int nVipLev = GetVipLev()==4 ? 1 : GetVipLev();
			if (FindTitleByType(_TITLE_VIP))
			{
				int nLev = GetTitleLev(_TITLE_VIP);
				if (nLev != nVipLev)
					SetTitleLevel(_TITLE_VIP, nVipLev);
			}
			else
			{
				AddHoldTitle(_TITLE_VIP, nVipLev);
			}
		}
		DEBUG_CATCH("set vip title1")

		DEBUG_TRY
		if(IsVipActive() && GetMap())
		{
			g_VipListMgr.AddVipUser(this);
			//广播到其他线路
			CMsgVipList msg;
			if (msg.Create(GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_ONLINE))
			{
				msg.Append(GetID(), GetMap()->GetDocID(), GetLev(), CVipList::GetLineByName(), GetVipLev(), GetName());
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
			}
		}
		DEBUG_CATCH("CUser::AddVipTime.Update Vip List 1")

		return true;
	}
	else
	{
		CTime timeEndOld(CTime::GetCurrentTime());
		CTimeSpan timeSpan(0,0,dwMin,0);
		CTime timeThisEnd = timeEndOld + timeSpan;
		m_data.SetVipTime((timeThisEnd.GetYear() - 2000) * 100000000 + timeThisEnd.GetMonth() * 1000000  + timeThisEnd.GetDay() * 10000+ timeThisEnd.GetHour() * 100+ timeThisEnd.GetMinute() );
		m_data.ActiveVipTime();
		
		if(1440 == dwMin)
		{
			m_data.SetVipLev(4);
		}
		if (10080 == dwMin)
		{
			//if(m_data.GetVipLev() < 1 || m_data.GetVipLev() > 4)
			m_data.SetVipLev(4);
			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>远见非凡，重金购买了<%s>使用，真是羡煞众人，令天下皆惊！", UserName,"3_VIP周卡_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!4")
		}
		else if (43200 == dwMin)
		 {
			 m_data.SetVipLev(1);
			 DEBUG_TRY
			 char szMsg[512] = "";
			 sprintf(szMsg, "<%s>远见非凡，重金购买了<%s>使用，真是羡煞众人，令天下皆惊！", UserName,"3_VIP月卡_5");
			 UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			 DEBUG_CATCH("vip broad!!1")
		 }
		else if (129600 == dwMin)
		{
			m_data.SetVipLev(2);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>远见非凡，重金购买了<%s>使用，万贯家财足以傲视群雄，冠绝江湖！", UserName,"3_VIP季卡_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!2")
		}
	   	else if (259200 == dwMin)
		{
			m_data.SetVipLev(3);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>远见非凡，重金购买了<%s>使用，卓然独立于天下，风范如君者，当今之世，能有几人？", UserName,"3_VIP半年卡_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!3")
		}

		CMsgPlayer msgPlayer;
		if (msgPlayer.Create(this->QueryRole()))
			this->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		DEBUG_TRY
		if (IsVipActive())
		{
			int nVipLev = GetVipLev()==4 ? 1 : GetVipLev();
			if (FindTitleByType(_TITLE_VIP))
			{
				int nLev = GetTitleLev(_TITLE_VIP);
				if (nLev != nVipLev && m_pTitle)
					m_pTitle->ChangeTitleLevel(_TITLE_VIP, nVipLev);
			}
			else
			{
				AddHoldTitle(_TITLE_VIP, nVipLev);
			}
		}
		DEBUG_CATCH("set vip title2")

		DEBUG_TRY
		if(IsVipActive() && GetMap())
		{
			g_VipListMgr.AddVipUser(this);
			//广播到其他线路
			CMsgVipList msg;
			if (msg.Create(GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_ONLINE))
			{
				msg.Append(GetID(), GetMap()->GetDocID(), GetLev(), CVipList::GetLineByName(), GetVipLev(), GetName());
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
			}
		}
		DEBUG_CATCH("CUser::AddVipTime.Update Vip List 2")

		return true;
	}

}

void CUser::AddVit(int nData)
{
	int nVit = m_data.GetVit();
	nVit += nData;
	if(nVit < 0)
		nVit = 0;

	if(nVit > m_data.GetMaxVit())
		nVit = m_data.GetMaxVit();

	m_data.SetVit(nVit);

	CMsgUserAttrib msg;
	if(msg.Create(GetID(),_USERATTRIB_VIT,m_data.GetVit()))
		SendMsg(&msg);
}

void CUser::SetVit(int nData)
{
	if(nData < 0)
		nData = 0;
	
	if(nData > m_data.GetMaxVit())
		nData = m_data.GetMaxVit();

	m_data.SetVit(nData);

	CMsgUserAttrib msg;
	if(msg.Create(GetID(),_USERATTRIB_VIT,m_data.GetVit()))
		SendMsg(&msg);
}

void CUser::AddEne(int nData)
{
	int nEne = m_data.GetEne();
	nEne += nData;
	if(nEne < 0)
		nEne = 0;
	
	if(nEne > m_data.GetMaxEne())
		nEne = m_data.GetMaxEne();
	
	m_data.SetEne(nEne);

	CMsgUserAttrib msg;
	if(msg.Create(GetID(),_USERATTRIB_ENE,m_data.GetEne()))
		SendMsg(&msg);
}

void CUser::SetEne(int nData)
{
	if(nData < 0)
		nData = 0;
	
	if(nData > m_data.GetMaxEne())
		nData = m_data.GetMaxEne();
	
	m_data.SetEne(nData);

	CMsgUserAttrib msg;
	if(msg.Create(GetID(),_USERATTRIB_ENE,m_data.GetEne()))
		SendMsg(&msg);
}

void CUser::ClrVipTime() 
{ 
	m_data.ClrVipTime();
	SendSysMsg("你的VIP已经过期");
	
	CMsgPlayer msgPlayer;
	if (msgPlayer.Create(this->QueryRole()))
		this->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
}

int  CUser::GetRandForActive()
{
	CTime timecount(2010,1,1,0,0,0);
	CTime timeCurr(CTime::GetCurrentTime());
	CTimeSpan spanTime = timeCurr - timecount;
	int nPassDay = (spanTime.GetTotalHours() / 24) % 12;
	return nPassDay;
}

bool CUser::GetNewGive(int nType)
{
	//掩码
	if(ChkNewGiveMask(nType - 1))
		return false;

	int nCheckLev = 0;
	DEBUG_TRY
	int nTotalHour = GetPassedTime(GetCreateTime(),2);

	//检测
	switch(nType)
	{
	case 1:
		{
			//第1天 1天	
			if(nTotalHour >= 24)
				return false;
			//15级
			if(GetLev() < 15)
				return false;
		}
		break;
	case 2:
		{
			//第2天 2天	
			if(nTotalHour < 24 )
				return false;
			if(nTotalHour >= 24  + 24 * 2)
				return false;

			//26级
			if(GetLev() < 26)
				return false;
		}
		break;
	case 3:
		{
			//第3天 3天	
			if(nTotalHour < 24 * 2)
				return false;
			if(nTotalHour >= 24  * 2 + 24 * 3)
				return false;
			
			//37级
			if(GetLev() < 37)
				return false;
		}
		break;
	case 4:
		{
			//第4天 3天	
			if(nTotalHour < 24 * 3)
				return false;
			if(nTotalHour >= 24  * 3 + 24 * 6)
				return false;
			
			//副职业
			if(GetProfession(1) == 0)
				return false;
		}
		break;
	case 5:
		{
			//第5天 1天	
			if(nTotalHour < 24 * 4)
				return false;
			if(nTotalHour >= 24  * 4 + 24)
				return false;
			
			//好友20个
			if(GetFriendAllAmount() < 20)
				return false;
		}
		break;
	case 6:
		{
			//第6天 4天	
			if(nTotalHour < 24 * 5)
				return false;
			if(nTotalHour >= 24  * 5 + 24 * 10)
				return false;
			
			//入帮3天
			CSynAttr* pSynAttr = QuerySynAttr();
			if(!pSynAttr)
				return false;

			int nTotalJoinHour = GetPassedTime(pSynAttr->GetInt(SYNATTRDATA_jointime),2);
			if(nTotalJoinHour < 24 * 3)
				return false;
		}
		break;
	case 7:
		{
			//第7天 3天	
			if(nTotalHour < 24 * 6)
				return false;
			if(nTotalHour >= 24  * 6 + 24 * 3)
				return false;

			//内功或外功 6000
			if((GetAtk() < 6000) && (GetMgcAtk() < 6000))
				return false;
		}
		break;
	case 8:
		{
			//第8天 10天	
			if(nTotalHour < 24 * 7)
				return false;
			if(nTotalHour >= 24  * 7 + 24 * 10)
				return false;
			
			//50级套装3件
			int nHas = 0;
			DWORD prof = this->GetProfession();
			if( prof & PROF_ASSIST )//辅助
			{
				CItem* refpEquip = GetEquipItemByPos(ITEMPOSITION_ARMOR);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 126921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_HELMET);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 136921))
					nHas ++;

				refpEquip = GetEquipItemByPos(ITEMPOSITION_NECKLACE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 156921))
					nHas ++;

				refpEquip = GetEquipItemByPos(ITEMPOSITION_BANGLE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 166921))
					nHas ++;

				refpEquip = GetEquipItemByPos(ITEMPOSITION_CAESTUS);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 186921))
					nHas ++;

				refpEquip = GetEquipItemByPos(ITEMPOSITION_SHOES);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 196921))
					nHas ++;				
			}
			else if( prof & PROF_MASTER )//法师
			{
				CItem* refpEquip = GetEquipItemByPos(ITEMPOSITION_ARMOR);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 122921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_HELMET);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 132921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_NECKLACE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 152921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_BANGLE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 162921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_CAESTUS);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 182921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_SHOES);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 192921))
					nHas ++;
			}
			else if( prof & PROF_PASTOR )//牧师
			{
				CItem* refpEquip = GetEquipItemByPos(ITEMPOSITION_ARMOR);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 121921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_HELMET);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 131921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_NECKLACE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 151921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_BANGLE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 161921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_CAESTUS);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 181921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_SHOES);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 191921))
					nHas ++;
			}
			else if( prof & PROF_ASSISSIN )//刺客
			{
				CItem* refpEquip = GetEquipItemByPos(ITEMPOSITION_ARMOR);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 123921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_HELMET);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 133921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_NECKLACE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 153921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_BANGLE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 163921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_CAESTUS);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 183921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_SHOES);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 193921))
					nHas ++;
			}
			else if( prof & PROF_SOLDIER )//战士
			{
				CItem* refpEquip = GetEquipItemByPos(ITEMPOSITION_ARMOR);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 125921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_HELMET);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 135921))
					nHas ++;
			
				refpEquip = GetEquipItemByPos(ITEMPOSITION_NECKLACE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 155921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_BANGLE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 165921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_CAESTUS);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 185921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_SHOES);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 195921))
					nHas ++;
			}
			else if( prof & PROF_SWORD )//弓手
			{
				CItem* refpEquip = GetEquipItemByPos(ITEMPOSITION_ARMOR);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 124921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_HELMET);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 134921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_NECKLACE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 154921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_BANGLE);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 164921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_CAESTUS);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 184921))
					nHas ++;
				
				refpEquip = GetEquipItemByPos(ITEMPOSITION_SHOES);
				if (refpEquip && (refpEquip->GetInt(ITEMDATA_TYPE) == 194921))
					nHas ++;
			}
			else
				return false;

			if(nHas < 3)
				return false;
		}
		break;
	case 9:
		{
			//第9天 5天	
			if(nTotalHour < 24 * 8)
				return false;
			if(nTotalHour >= 24  * 8 + 24 * 5)
				return false;
			
			//出战宠 50级 6技能
			if(!m_pWarPet)
				return false;

			EudemonData eData;
			LoadEudemon(m_pWarPet->GetID(),eData,1);

			if(eData.level < 50)
				return false;

			int nSkillAmount = 0;
			for (int n = 0 ; n < 10 ; n ++)
			{
				if(eData.skill[n] > 0)
					nSkillAmount ++;
			}
			if(nSkillAmount < 6)
				return false;
		}
		break;
	case 10:
		{
			//第10天 5天	
			if(nTotalHour < 24 * 9)
				return false;
			if(nTotalHour >= 24  * 9 + 24 * 8)
				return false;
			
			//60级
			if(GetLev() < 55)
				return false;
		}
		break;
	case 11:
		nCheckLev = 18;
		break;
	case 12:
		nCheckLev = 25;
		break;
	case 13:
		nCheckLev = 26;
		break;
	case 14:
		nCheckLev = 28;
		break;
	case 15:
		nCheckLev = 29;
		break;
	case 16:
		nCheckLev = 30;
		break;
	case 17:
		nCheckLev = 32;
		break;
	case 18:
		nCheckLev = 33;
		break;
	case 19:
		nCheckLev = 35;
		break;
	case 20:
		nCheckLev = 36;
		break;
	case 21:
		nCheckLev = 38;		
		break;
	default:
		return false;
	}
 

	if(nCheckLev > 0)
	{
		if(GetLev() < nCheckLev)
			return false;		
		
		if(!ChkNewGiveMask2(nType))
			return false;
	}

	DEBUG_CATCH("GetNewGive");
	DEBUG_TRY
	//SendSysMsg("领取%u",nType);
	if(ProcessAction(2000 + nType))
	{
		AddNewGiveMask(nType - 1);
		return true;
	}
	DEBUG_CATCH("ProcessAction(2000 + nType)");
	return false;
}

void CUser::CheckEudeLifespan()
{
	if(!m_pUserPet)
		return;
	if(!m_pWarPet && m_idSoulEudemon==0)
		return;

	DWORD dwSpan = clock() - m_lastEudemonLifespanCheck;
	DWORD dwSoulSpan = clock() - m_lastSoulEudemonLifeSpan;
	if(dwSpan >= 600000)
	{
		m_lastEudemonLifespanCheck += 600000;

		int amount = m_pUserPet->GetEudmonAmount();
		CUserPetData* pData = NULL;
		for(int i = 0 ; i < amount ; i ++)
		{
			pData = m_pUserPet->QueryDataByIndex(i);
			if(!pData)
				continue;
			if(pData->GetInt(EUDEMONDATA_state)==PETSTATE_INWAR)
			{

				if(m_pWarPet->GetID() != pData->GetID())
				continue;
				int nOldLifespan = pData->GetInt(EUDEMONDATA_lifespan);
				if(nOldLifespan == 0)
					continue;
				int nNewLifespan = max(nOldLifespan - 1,0);
				if(nNewLifespan != nOldLifespan)
				{
					pData->SetInt(EUDEMONDATA_lifespan,nNewLifespan,true);

					CMsgEudemonAttrib msg;
					if(msg.Create(pData->GetID(),EUDATTR_LIFESPAN,nNewLifespan))
						SendMsg(&msg);

					if(nNewLifespan == 0)
					{
						SendSysMsg("当前出战宠物寿命已尽");
						DEBUG_TRY
						CallBackEudemon(m_pWarPet->GetID(),true,true,false);
						DEBUG_CATCH2("CheckEudeLifespan - CallBackEudemon %u",m_pWarPet->GetID());
					}
				}
			}
		}
	}
	if(dwSoulSpan > 60000)
	{
		m_lastSoulEudemonLifeSpan += 60000;

		int amount = m_pUserPet->GetEudmonAmount();
		CUserPetData* pData = NULL;
		for(int i = 0 ; i < amount ; i ++)
		{
			pData = m_pUserPet->QueryDataByIndex(i);
			if(!pData)
				continue;
			if(pData->GetInt(EUDEMONDATA_state)==PETSTATE_ADDIN)
			{
				if(m_idSoulEudemon != pData->GetID())
					continue;
				int nOldLifespan = pData->GetInt(EUDEMONDATA_lifespan);
				if(nOldLifespan == 0)
					continue;
				int nNewLifespan = max(nOldLifespan - 1,0);
				if(nNewLifespan != nOldLifespan)
				{
					pData->SetInt(EUDEMONDATA_lifespan,nNewLifespan,false);
					
					CMsgEudemonAttrib msg;
					if(msg.Create(pData->GetID(),EUDATTR_LIFESPAN,nNewLifespan))
						SendMsg(&msg);
					
					if(nNewLifespan == 0)
					{
						SendSysMsg("当前附体宠物寿命已尽");
						DEBUG_TRY
						RemoveEudemonSoul();
						DEBUG_CATCH2("CheckEudeLifespan - RemoveEudemonSoul %u",m_idSoulEudemon);
					}
				}
			}
		}
	}
	
}

void CUser::CheckVipTime()
{
	if(!IsVipActive())
		return;
	if( !m_tVipTime.ToNextTime() )
	{
		return;				
	}

	int nEndDate = m_data.GetVipTime();
	int nYear = 2000 + nEndDate / 100000000;
	int nMounth =  (nEndDate % 100000000) / 1000000;
	int nDay =  (nEndDate % 1000000) / 10000;
	int nHour =  (nEndDate % 10000) / 100;
	int nMin =  nEndDate % 100;
	CTime timeEnd(nYear,nMounth,nDay,nHour,nMin,0);
	CTime timeNow(CTime::GetCurrentTime());
	CTimeSpan span = timeEnd - timeNow;
	int nTotalMin = span.GetTotalMinutes();
	if(nTotalMin <= 0)
	{
		m_data.ClrVipTime();
		SendSysMsg("你的VIP已经过期");

		CMsgPlayer msgPlayer;
		if (msgPlayer.Create(this->QueryRole()))
			this->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

		DelHoldTitle(_TITLE_VIP);//goto 删除VIP称号
	}
	else if(nTotalMin <= 4320)
	{
		if(m_dwLastVipCheckSendTime  != ((timeNow.GetYear() - 2000) * 100000000 + timeNow.GetMonth() * 1000000  + timeNow.GetDay() * 10000))
		{
			m_dwLastVipCheckSendTime = (timeNow.GetYear() - 2000) * 100000000 + timeNow.GetMonth() * 1000000  + timeNow.GetDay() * 10000;
// 			char szTempTime[64];
// 			nHour = nTotalMin
			SendSysMsg("你的VIP离过期还有%u分钟",nTotalMin);
		}
	}

}

void CUser::CheckVipFaceTime()
{
	if (!this->GetVipface())
	{
		return;
	}
	if( !m_tVipFaceTime.ToNextTime() )
	{
		return;				
	}
	CTime timeNow(CTime::GetCurrentTime());
	OBJID TimeNow = (timeNow.GetYear() -2000) * 1000000 + timeNow.GetMonth() * 10000 + timeNow.GetDay() * 100 + timeNow.GetHour();
	OBJID TimeEnd = this->GetVipFaceTime();
	if (TimeEnd > TimeNow)
		return;
	else
	{
		this->ClrVipFace();
		CMsgUserAttrib	msg;
		IF_OK(msg.Create(GetID(), _USERATTRIB_LOOKFACE, GetFace()))
			this->BroadcastRoomMsg(&msg, true);
		SendSysMsg("您的VIP头像已过期");
	}

}

bool CUser::IsVipActive()
{
	return m_data.IsVipActive();
}

DWORD  CUser::GetVipLev()
{
	int nLev = m_data.GetVipLev();
	if(nLev < 1 || nLev > 4)
	{
		m_data.SetVipLev(1);
		nLev = 1;
	}
	return nLev;
}

//  [2007-9-14] 朱斌	检查物品生命周期
void CUser::CheckItemLifeTime(CItemPtr pItem, BOOL bUpdateToClient, BOOL bSaveToDatabase)
{
	CHECK(pItem);

	int nPos = pItem->GetPosition();

	if(pItem->GetInt(ITEMDATA_NEEDCHECK) == 0)
		return;

	if(pItem->IsActivateEquip() || pItem->IsActivatePickup())
	{
		int nEndDate = pItem->GetInt(ITEMDATA_LIFE_TIME);
		int nYear = 2000 + nEndDate / 100000000;
		int nMounth =  (nEndDate % 100000000) / 1000000;
		int nDay =  (nEndDate % 1000000) / 10000;
		int nHour =  (nEndDate % 10000) / 100;
		int nMin =  nEndDate % 100;
		CTime timeEnd(nYear,nMounth,nDay,nHour,nMin,0);
		CTime timeNow(CTime::GetCurrentTime());
		CTimeSpan span = timeEnd - timeNow;
		int nTotalMin = span.GetTotalMinutes();
		if(nTotalMin <= 0)
		{
			char szWords[128];
			sprintf(szWords,"你的%s已经到期",pItem->GetStr(ITEMDATA_NAME));
			SendSysMsg(szWords);
			//LeaveWord()->InsertWords(SYSTEM_NAME, this->GetName(), szWords, "0");

			if(pItem->IsLifeTimeNonContinue())
			{
				bool bCostlyItem = pItem->IsCostlyItem();
				OBJID itemtype = pItem->GetTypeID();
				DEBUG_TRY
				if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
					EraseEquip(nPos, true);
				else
					EraseItem(pItem->GetID(), true);

				if (bCostlyItem)//goto 贵重物品记录
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from time_over", GetID(), GetName(), itemtype);
				DEBUG_CATCH("EraseItem(pItem->GetID(), true);");
			}
			else
			{
				pItem->SetInt(ITEMDATA_NEEDCHECK,2);
				if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
					UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
			}

		}
	}
// 		OBJID idItem = pItem->GetID();
// 
// 		OBJID idType = pItem->GetTypeID();
// 		
// 		LPCSTR pszLastCheckTime = pItem->GetStr(ITEMDATA_LAST_CHECK_TIME);
// 		
// 		//  [2007-9-14] 朱斌	未激活
// 		if(strlen(pszLastCheckTime) == 0)
// 			return;
// 
// 		if(stricmp(pszLastCheckTime, "NULL") == 0)
// 			return;
// 		
// 		unsigned	int	uLifeTime = pItem->GetInt(ITEMDATA_LIFE_TIME);
// 		
// 		if(uLifeTime == 0)
// 			return;
// 		
// 		COleDateTime timeLastCheckTime;
// 		COleDateTime timeNow = COleDateTime::GetCurrentTime();
// 		timeLastCheckTime.ParseDateTime(pszLastCheckTime);
// 		COleDateTimeSpan timeSpan = timeNow - timeLastCheckTime;
// 		unsigned int uTempLifeTime = timeSpan.GetTotalMinutes();
// 		int nNewLifeTime = uLifeTime - uTempLifeTime;
// 		
// 		if(nNewLifeTime <= 0)	//	到期
// 		{
// 			char szMailTitle[255];
// 			char szMailContent[255];
// 			
// 			nNewLifeTime = 0;
// 			{
// 			}
// 			
// // 			bool	bSaveToDatabase = FALSE;
// // 			if(timeNow.GetMinute() % 20 == 0)
// // 				bSaveToDatabase = TRUE;
// 
// 			clock_t tStart = clock();
// 			
// 			pItem->SetInt(ITEMDATA_LIFE_TIME, nNewLifeTime,TRUE);
// 			
// 			char szTimeString[25];
// 			sprintf(szTimeString, "%s", timeNow.Format(_T("%Y-%m-%d %H:%M:%S")).GetBuffer(0));
// 			
// 			if(uTempLifeTime > 0)
// 		    	pItem->SetStr(ITEMDATA_LAST_CHECK_TIME, szTimeString, 21, TRUE);
// 
// 			InterlockedExchangeAdd(&g_stat.setDebug[14], clock() - tStart);	//	数据库更新用时
// 			
// 			//  已经到期,马上发消息
// 			tStart = clock();
// 			CMsgItemAttrib msg;
// 			if(msg.Create(pItem->GetID(), _ITEMATTRIB_IS_ACTIVATE, (DWORD)TRUE))
// 			{
// 				msg.Append(_ITEMATTRIB_LIFE_TIME, 0);
// 				this->SendMsg(&msg);
// 			}
// 			InterlockedExchangeAdd(&g_stat.setDebug[15], clock() - tStart);	//	同步客户端用时
// 
// 			//  [3/5/2008 朱斌]	摆摊异常
// 			//	先从摆摊中删除
// 			CBooth* pBooth = QueryBooth();
// 			if(pBooth)
// 			{
// 				int nCount = pBooth->GetCost(idItem);
// 				if(nCount > 0)
// 				{
// 					CMsgItem	msg;
// 					IF_OK(msg.Create(idItem, ITEMACT_BOOTH_DEL, pBooth->GetID()))
// 						BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 				}
// 			}			
// 			
// 			if(pItem->IsLifeTimeNonContinue())
// 			{
// 				//	先发邮件
// 				sprintf(szMailTitle, g_objStrRes.GetStr(16022), pItem->GetStr(ITEMDATA_NAME));
// 				sprintf(szMailContent, g_objStrRes.GetStr(16023), pItem->GetStr(ITEMDATA_NAME));
// 				PostOffice()->SendSysMail(this->GetID(), szMailTitle, szMailContent, 0);
// 
// 				//	不可续费,删除
// 				if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
// 					EraseEquip(nPos, true);
// 				else
// 					EraseItem(pItem->GetID(), true);
// 
// 				pItem = NULL;
// 			}
// 			else
// 			{
// 				//	可以续费
// 				//	卸下,不能使用,通知玩家,发邮件
// 		//		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
// 		//			UnEquipItem(nPos, true);	//	如果有空位就放下来
// 
// 				//	如果已经装备,放下
// 				if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
// 				{
// 					CMsgItem msg;
// 					if(msg.Create(pItem->GetID(), ITEMACT_DROPEQUIPMENT, nPos))
// 						this->SendMsg(&msg);
// 				}
// 
// 				sprintf(szMailTitle, g_objStrRes.GetStr(16024), pItem->GetStr(ITEMDATA_NAME));
// 				sprintf(szMailContent, g_objStrRes.GetStr(16025), pItem->GetStr(ITEMDATA_NAME));
// 
// 				//  [2007-9-18] 朱斌	用邮件发给玩家
// 	 			PostOffice()->SendMail(this, this->GetName(), szMailTitle, szMailContent, CSKMail::mtNormal, pItem->GetID(), 0);	
// 				//PostOffice()->SendSysMail(GetID(), szMailTitle, szMailContent);
// 
// 				pItem = NULL;
// 			}	
// 			
// 			return;
// 		}
// 		else if(nNewLifeTime <= 60)	//	离到期还有一个小时
// 		{
// 			if(uLifeTime > 60)
// 			{
// 				//	刚刚到最后一个小时,马上提醒
// 				//	您的%s还有%d分钟到期，请用Shift+鼠标右键点击物品续费。
// 				if(pItem->IsLifeTimeContinue())	//  [11/8/2007 朱斌]	可以续费再发
// 					this->SendSysMsg(_TXTATR_SYSTEM, g_objStrRes.GetStr(16021) , pItem->GetStr(ITEMDATA_NAME ), nNewLifeTime);
// 				
// 				clock_t tStart = clock();
// 				CMsgItemAttrib msg;
// 				if(msg.Create(pItem->GetID(), _ITEMATTRIB_LIFE_TIME, (DWORD)pItem->GetLifeTime()))
// 				{
// 					this->SendMsg(&msg);
// 				}
// 				InterlockedExchangeAdd(&g_stat.setDebug[15], clock() - tStart);	//	同步客户端用时
// 			}
// 			else
// 			{
// 				//	简单处理每5分钟提醒一次, 剩余分钟尾数为0,5时提醒
// 				if(pItem->IsLifeTimeContinue())	//  [11/8/2007 朱斌]	可以续费再发
// 					if(nNewLifeTime % 5 == 0 || nNewLifeTime % 10 == 0)
// 						this->SendSysMsg(_TXTATR_SYSTEM, g_objStrRes.GetStr(16021) , pItem->GetStr(ITEMDATA_NAME ), nNewLifeTime);
// 			}
// 		}
// 		
// 		clock_t tStart = clock();
// 		pItem->SetInt(ITEMDATA_LIFE_TIME, nNewLifeTime, bSaveToDatabase);
// 		
// 		char szTimeString[25];
// 		sprintf(szTimeString, "%s", timeNow.Format(_T("%Y-%m-%d %H:%M:%S")).GetBuffer(0));
// 		
// 		if(uTempLifeTime > 0)
// 	    	pItem->SetStr(ITEMDATA_LAST_CHECK_TIME, szTimeString, 21, bSaveToDatabase);
// 		
// 		InterlockedExchangeAdd(&g_stat.setDebug[14], clock() - tStart);	//	数据库更新用时
// 		
// 		tStart = clock();
// 		if(bUpdateToClient)	//	更新客户端
// 		{
// 			CMsgItemAttrib msg;
// 			if(msg.Create(pItem->GetID(), _ITEMATTRIB_LIFE_TIME, (DWORD)pItem->GetLifeTime()))
// 			{
// 				this->SendMsg(&msg);
// 			}
// 		}
// 		InterlockedExchangeAdd(&g_stat.setDebug[15], clock() - tStart);	//	同步客户端用时
// 	}
}

//  [2007-9-21] 朱斌	加新的赠送请求信息
// void CUser::InsertVasItemPresendInfo(_VAS_ITEM_PRESENT_INFO& info)
// {
// 	m_setVasItemPresentInfo.push_back(info);	
// }

//  [2007-9-21] 朱斌	检索赠送请求信息
//	找到返回TRUE, 并将信息填充到info中去,否则失败返回FALSE
// BOOL CUser::FetchVasItemPresendInfo(DWORD dwTick, _VAS_ITEM_PRESENT_INFO& info)
// {
// 	memset(&info, 0, sizeof(info));
// 	for(int i = 0; i < m_setVasItemPresentInfo.size(); i++)
// 	{
// 		if(m_setVasItemPresentInfo[i].dwTick == dwTick)
// 		{
// 			info.dwTick = m_setVasItemPresentInfo[i].dwTick;
// 			info.idTarget = m_setVasItemPresentInfo[i].idTarget;
// 			info.idItemType = m_setVasItemPresentInfo[i].idItemType;
// 			info.uAmount = m_setVasItemPresentInfo[i].uAmount;
// 			strcpy(info.szTargetName, m_setVasItemPresentInfo[i].szTargetName);
// 			strcpy(info.szRegard, m_setVasItemPresentInfo[i].szRegard);
// 			m_setVasItemPresentInfo.erase(m_setVasItemPresentInfo.begin() + i);
// 			return TRUE;
// 		}
// 	}
// 
// 	return FALSE;
// }
void CUser::SaveMissionInfo()
{
	CTaskDetail * pTask = this->GetTaskDetail();
	IF_NOT(pTask)
		return;

	for(int i = 0 ; i < pTask->m_setData.size() ; i ++)
	{
		if(pTask->m_setData[i])
			pTask->m_setData[i]->UpdateData();
	}
}

void CUser::UpdateMissionMsgByMonsterType(OBJID idType)
{
	if(idType == ID_NONE)
		return;

	TaskDetailInfoForMsg taskDetailArrayTemp[_MAX_TASK_DETAIL_STRUCT_NUM];//结构体数组.
	int taskDetailNum=0;
	CTaskDetail * pTask = this->GetTaskDetail();
	IF_NOT(pTask)
		return ;
	
	int nSendCountAll = 0;//总共实际发送条数
	int nCurSendCount = 0;//本次发送条数
	while((pTask->m_setData.size()-nSendCountAll)>0)//还有发的
	{
		if(nCurSendCount >= _MAX_TASK_DETAIL_STRUCT_NUM)
		{
			CMsgTaskDetail msg;
			IF_OK(msg.Create(_TASKHINT_UPDATE,
				taskDetailArrayTemp,
				nCurSendCount))
				this->SendMsg(&msg);
			
			memset(taskDetailArrayTemp,0,sizeof(taskDetailArrayTemp));
			nCurSendCount = 0;
		}
		
		
		OBJID taskID = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKID);
		OBJID taskPhase = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKPHASE);
		int isRec = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_ISREC);
		if(isRec != 1)
		{
			nSendCountAll++;
			continue;
		}
		
		CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(taskID, taskPhase);
		if(!pSetData)
		{
			nSendCountAll++;
			continue;
		}
		
		bool bSend = false;
		if(pSetData->TaskData.monster1 == idType)
		{
			if(pSetData->TaskData.monster1 > 100000)
			{
				if(pSetData->TaskData.monster1 == 100001)
				{
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA1,GetFriendAllAmount());
					taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
					taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
					taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
					bSend = true;
				}
				else if(pSetData->TaskData.monster1 == 100002)
				{
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA1,GetLev());
					taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
					taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
					taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
					bSend = true;
				}
				else if(pSetData->TaskData.monster1 > 200000 && pSetData->TaskData.monster1 < 300000)
				{
					OBJID idSkill = pSetData->TaskData.monster1 - 200000;
					if(QueryMagic(idSkill))
						pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA1,QueryMagic(idSkill)->GetMagicLevel());
					else
						pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA1,0);
					taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
					taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
					taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
					bSend = true;
				}
			}
			else
			{
				if(pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1) >= pSetData->TaskData.monsterNum1)
				{
					nSendCountAll++;
					continue;
				}

				pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA1,pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1) + 1);
				taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
				taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
				taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
				bSend = true;
			}
		}
		if(pSetData->TaskData.monster2 == idType)
		{	
			if(pSetData->TaskData.monster2 > 100000)
			{
				if(pSetData->TaskData.monster2 == 100001)
				{
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA2,GetFriendAllAmount());
					taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
					taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
					taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
					bSend = true;
				}
				else if(pSetData->TaskData.monster2 == 100002)
				{
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA2,GetLev());
					taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
					taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
					taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
					bSend = true;
				}
				else if(pSetData->TaskData.monster2 > 200000 && pSetData->TaskData.monster2 < 300000)
				{
					OBJID idSkill = pSetData->TaskData.monster2 - 200000;
					if(QueryMagic(idSkill))
						pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA2,QueryMagic(idSkill)->GetMagicLevel());
					else
						pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA2,0);
					taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
					taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
					taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
					bSend = true;
				}
			}
			else
			{
				if(pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2) >= pSetData->TaskData.monsterNum2)
				{
					nSendCountAll++;
					continue;
				}
				pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA2,pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2) + 1);
				taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
				taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
				taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
				bSend = true;
			}
		}
		if(pSetData->TaskData.monster3 == idType)
		{
			if(pSetData->TaskData.monster3 > 100000)
			{
				if(pSetData->TaskData.monster3 == 100001)
				{
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA3,GetFriendAllAmount());
					taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
					taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
					taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
					bSend = true;
				}
				else if(pSetData->TaskData.monster3 == 100002)
				{
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA3,GetLev());
					taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
					taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
					taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
					bSend = true;
				}
				else if(pSetData->TaskData.monster3 > 200000 && pSetData->TaskData.monster3 < 300000)
				{
					OBJID idSkill = pSetData->TaskData.monster3 - 200000;
					if(QueryMagic(idSkill))
						pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA3,QueryMagic(idSkill)->GetMagicLevel());
					else
						pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA3,0);
					taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
					taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
					taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
					bSend = true;
				}
			}
			else
			{
				if(pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3) >= pSetData->TaskData.monsterNum3)
				{
					nSendCountAll++;
					continue;
				}
				pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA3,pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3) + 1);
				taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
				taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
				taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
				bSend = true;
			}
		}

		if(bSend)
		{
			taskDetailArrayTemp[nCurSendCount].taskID = taskID;
			taskDetailArrayTemp[nCurSendCount].taskPhase = taskPhase;
			taskDetailArrayTemp[nCurSendCount].data4 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA4);
			taskDetailArrayTemp[nCurSendCount].data5 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA5);
			taskDetailArrayTemp[nCurSendCount].data6 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA6);
			taskDetailArrayTemp[nCurSendCount].isRec = isRec;
			DWORD timeSpan = TaskSetRand()->GetSetTime(taskDetailArrayTemp[nCurSendCount].taskID,taskDetailArrayTemp[nCurSendCount].taskPhase);//任务规定时间
			if(timeSpan == 0)
				taskDetailArrayTemp[nCurSendCount].lifeTime = 99999;
			else
			{
				DWORD now = TimeGet(TIME_SECOND);//现在时间
				DWORD frontTime = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKBEGINTIME);//任务开始时间
				DWORD span  = now - frontTime;
				if(span > timeSpan)
					taskDetailArrayTemp[nCurSendCount].lifeTime = 0;
				else
					taskDetailArrayTemp[nCurSendCount].lifeTime = timeSpan - span;  
			}
			taskDetailArrayTemp[nCurSendCount].randStep = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_RANDSTEP);
			nCurSendCount++;
		}
		nSendCountAll++;
	}
	//处理尾巴
	if(nCurSendCount>0)
	{
		CMsgTaskDetail msg;
		IF_OK(msg.Create(_TASKHINT_UPDATE,
			taskDetailArrayTemp,
			nCurSendCount))
			this->SendMsg(&msg);
	}
}

void CUser::UpdateMissionMsgByItemType(OBJID idType)
{
	if(idType == ID_NONE)
		return;

	TaskDetailInfoForMsg taskDetailArrayTemp[_MAX_TASK_DETAIL_STRUCT_NUM];//结构体数组.
	int taskDetailNum=0;
	CTaskDetail * pTask = this->GetTaskDetail();
	IF_NOT(pTask)
		return ;
	
	int nSendCountAll = 0;//总共实际发送条数
	int nCurSendCount = 0;//本次发送条数
	while((pTask->m_setData.size()-nSendCountAll)>0)//还有发的
	{
		if(nCurSendCount >= _MAX_TASK_DETAIL_STRUCT_NUM)
		{
			CMsgTaskDetail msg;
			IF_OK(msg.Create(_TASKHINT_UPDATE,
				taskDetailArrayTemp,
				nCurSendCount))
				this->SendMsg(&msg);
			
			memset(taskDetailArrayTemp,0,sizeof(taskDetailArrayTemp));
			nCurSendCount = 0;
		}
		
		OBJID taskID = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKID);
		OBJID taskPhase = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKPHASE);
		int isRec = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_ISREC);
		if(isRec != 1)
		{
			nSendCountAll++;
			continue;
		}

		CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(taskID, taskPhase);
		if(!pSetData)
		{
			nSendCountAll++;
			continue;
		}

		bool bSend = false;
		int nMaxAmount = 0;
		if(pSetData->TaskData.item1 == idType)
		{
			if(idType == _SPCITEM_MONEYFORSHOP)
			{
				if(CheckRandShopFull())
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA4,1);
				else
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA4,0);
			}
			else if(idType > 99999)
			{
				if(idType >= SPECIALITEMBASE)
				{
					idType%=SPECIALITEMBASE;
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA4,pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA4)+1);
				}
				else
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA4,GetItemByTypeMulti(idType,nMaxAmount,NULL));
			}
			else
				pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA4,GetEudemonAmountByType(idType));

			taskDetailArrayTemp[nCurSendCount].data4 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA4);
			taskDetailArrayTemp[nCurSendCount].data5 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA5);
			taskDetailArrayTemp[nCurSendCount].data6 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA6);
			bSend = true;
		}
		if(pSetData->TaskData.item2 == idType)
		{
			if(idType == _SPCITEM_MONEYFORSHOP)
			{
				if(CheckRandShopFull())
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA5,1);
				else
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA5,0);
			}
			if(idType > 99999)
			{
				if(idType >= SPECIALITEMBASE)
				{
					idType%=SPECIALITEMBASE;
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA5,pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA5)+1);
				}
				else pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA5,GetItemByTypeMulti(idType,nMaxAmount,NULL));
			}
			else
				pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA5,GetEudemonAmountByType(idType));

			taskDetailArrayTemp[nCurSendCount].data4 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA4);
			taskDetailArrayTemp[nCurSendCount].data5 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA5);
			taskDetailArrayTemp[nCurSendCount].data6 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA6);
			bSend = true;
		}
		if(pSetData->TaskData.item3 == idType)
		{
			if(idType == _SPCITEM_MONEYFORSHOP)
			{
				if(CheckRandShopFull())
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA6,1);
				else
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA6,0);
			}
			else if(idType > 99999)
			{
				if(idType >= SPECIALITEMBASE)
				{
					idType%=SPECIALITEMBASE;
					pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA6,pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA6)+1);
				}
				else pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA6,GetItemByTypeMulti(idType,nMaxAmount,NULL));
			}
			else
				pTask->m_setData[nSendCountAll]->SetInt(TASKDETAILDATA_DATA6,GetEudemonAmountByType(idType));

			taskDetailArrayTemp[nCurSendCount].data4 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA4);
			taskDetailArrayTemp[nCurSendCount].data5 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA5);
			taskDetailArrayTemp[nCurSendCount].data6 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA6);
			bSend = true;
		}
		if(bSend)
		{
			taskDetailArrayTemp[nCurSendCount].taskID = taskID;
			taskDetailArrayTemp[nCurSendCount].taskPhase = taskPhase;
			taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
			taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
			taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
			taskDetailArrayTemp[nCurSendCount].isRec = isRec;
			DWORD timeSpan = TaskSetRand()->GetSetTime(taskDetailArrayTemp[nCurSendCount].taskID,taskDetailArrayTemp[nCurSendCount].taskPhase);//任务规定时间
			if(timeSpan == 0)
				taskDetailArrayTemp[nCurSendCount].lifeTime = 99999;
			else
			{
				DWORD now = TimeGet(TIME_SECOND);//现在时间
				DWORD frontTime = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKBEGINTIME);//任务开始时间
				DWORD span  = now - frontTime;
				if(span > timeSpan)
					taskDetailArrayTemp[nCurSendCount].lifeTime = 0;
				else
					taskDetailArrayTemp[nCurSendCount].lifeTime = timeSpan - span;  
			}
			taskDetailArrayTemp[nCurSendCount].randStep = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_RANDSTEP);
			nCurSendCount++;
		}
		nSendCountAll++;
	}
	//处理尾巴
	if(nCurSendCount>0)
	{
		CMsgTaskDetail msg;
		IF_OK(msg.Create(_TASKHINT_UPDATE,
			taskDetailArrayTemp,
			nCurSendCount))
			this->SendMsg(&msg);
	}
}

int CUser::sendMissionMsg()
{
	int nAmont = 0;
	TaskDetailInfoForMsg taskDetailArrayTemp[_MAX_TASK_DETAIL_STRUCT_NUM];//结构体数组.
	int taskDetailNum=0;
	CTaskDetail * pTask = this->GetTaskDetail();
	IF_NOT(pTask)
		return 0;

	int nSendCountAll = 0;//总共实际发送条数
	int nCurSendCount = 0;//本次发送条数
	while((pTask->m_setData.size()-nSendCountAll)>0)//还有发的
	{
		if(nCurSendCount >= _MAX_TASK_DETAIL_STRUCT_NUM)
		{
			CMsgTaskDetail msg;
			IF_OK(msg.Create(_TASKHINT_UPDATE,
				taskDetailArrayTemp,
				nCurSendCount))
				this->SendMsg(&msg);
			nAmont += nCurSendCount;

			memset(taskDetailArrayTemp,0,sizeof(taskDetailArrayTemp));
			nCurSendCount = 0;
		}

		//压入
		taskDetailArrayTemp[nCurSendCount].taskID = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKID);
		taskDetailArrayTemp[nCurSendCount].taskPhase = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKPHASE);
		taskDetailArrayTemp[nCurSendCount].data1 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA1);
		taskDetailArrayTemp[nCurSendCount].data2 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA2);
		taskDetailArrayTemp[nCurSendCount].data3 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA3);
		taskDetailArrayTemp[nCurSendCount].data4 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA4);
		taskDetailArrayTemp[nCurSendCount].data5 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA5);
		taskDetailArrayTemp[nCurSendCount].data6 = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_DATA6);
		taskDetailArrayTemp[nCurSendCount].isRec = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_ISREC);
		taskDetailArrayTemp[nCurSendCount].isLink = 1;

		taskDetailArrayTemp[nCurSendCount].randStep = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_RANDSTEP);

		DWORD timeSpan = TaskSetRand()->GetSetTime(taskDetailArrayTemp[nCurSendCount].taskID,taskDetailArrayTemp[nCurSendCount].taskPhase);//任务规定时间
		if(timeSpan == 0)
			taskDetailArrayTemp[nCurSendCount].lifeTime = 99999;
		else
		{
			DWORD now = TimeGet(TIME_SECOND);//现在时间
			DWORD frontTime = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKBEGINTIME);//任务开始时间
			DWORD span  = now - frontTime;
			if(span > timeSpan)
				taskDetailArrayTemp[nCurSendCount].lifeTime = 0;
			else
			taskDetailArrayTemp[nCurSendCount].lifeTime = timeSpan - span;  
		}
		nCurSendCount++;
		nSendCountAll++;
	}
	//处理尾巴
	if(nCurSendCount>0)
	{
		CMsgTaskDetail msg;
		IF_OK(msg.Create(_TASKHINT_UPDATE,
			taskDetailArrayTemp,
			nCurSendCount))
			this->SendMsg(&msg);

		nAmont += nCurSendCount;
	}
	return nAmont;
}

// void CUser::DebuffCityWarWeapon()
// {
// 	IStatus* pStatus = QueryStatus(STATUS_CITYWAR_WEAPON);
// 	if( pStatus )
// 	{
// 		CRole::DetachStatus( QueryRole(),STATUS_CITYWAR_WEAPON );
// 
// 	// 	AddAttrib(_USERATTRIB_SPEED, 0, SYNCHRO_BROADCAST);
// 		CMsgUserAttrib	msg;
// 		if (msg.Create(this->GetID(), _USERATTRIB_SPEED, AdjustSpeed(this->GetSpeed())))
// 		{
// 			this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 		}
// 	}
// }

// void CUser::LevaeResourceWarMap()// 玩家离开资源战地图处理 [9/15/2007 %PENGFENG%]
// {
// 	//----------如果是离开资源战地图，必须先处理下面的再SAVEINFO-----begin-----------------//pengfeng
// 	this->DropItemBySys( g_ResourceWar.GetSKZLType(),this->GetPosX(),this->GetPosY() );
// 	
// 	CRole::DetachStatus( this->QueryRole(),STATUS_RESOURCEWAR_PROTECT );
// 
// 	//--------------------------这里才对!--------------end------------------------------------------
// }

//-------------------------------------
BOOL CUser::SynchroBCMaxLife()//2007918修罗:同步最大血量
{
	int nMaxLife = GetMaxLife();

	CMsgUserAttrib	msg;
	IF_NOT (msg.Create(GetID(), _USERATTRIB_MAXLIFE, nMaxLife))
		return false;
	
	BroadcastRoomMsg(&msg, EXCLUDE_SELF);//排除自己
	return true;
}

//-----------------------------------------
BOOL CUser::SynchroBCMaxMana()//2007918修罗:同步广播最大魔法
{
	int nMaxMana = GetMaxMana();

	CMsgUserAttrib	msg;
	IF_NOT (msg.Create(GetID(), _USERATTRIB_MAXMANA, nMaxMana))
		return false;
	
	BroadcastRoomMsg(&msg, EXCLUDE_SELF);//排除自己
	return true;
}


BOOL CUser::SynchroBCMaxSP()// 同步广播最大体力 [陈剑飞11/15/2007]
{
	int nMaxSP = GetMaxSp();
	CMsgUserAttrib	msg;

	if(GetCurJob() == 1)
	{
		IF_NOT (msg.Create(GetID(), _USERATTRIB_MAXSP, nMaxSP))
			return false;
	}
	else
	{
		IF_NOT (msg.Create(GetID(), _USERATTRIB_MAXSP_SEC, nMaxSP))
			return false;
	}
	
	BroadcastRoomMsg(&msg, true);// 广播消息，包含自己 [陈剑飞11/15/2007]
	return true;
}


BOOL CUser::SynchroBoothItem(CItem* pItem,int nItemInfoAction,int nBoothIndex,int nMoney)
{
	CMsgItem msg;
	if (msg.Create(nItemInfoAction,0)
		&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), /*pItem->GetPosition()*/200,pItem->GetKidnap(),nBoothIndex,nMoney,pItem->GetColour()))
		this->SendMsg(&msg);
	return true;
}

BOOL CUser::SendDetailItem(CItem* pItem)
{
	if(!pItem)
		return false;

// 	if(pItem->GetItemSort() == IETMSORT_EUDEMON)
// 	{
// 		CUserPetData* pPetData = m_pUserPet->QueryData(pItem->GetInt(ITEMDATA_BASE1));
// 		if(pPetData)
// 		{
// 			CMsgEudemonInfo msg;
// 			EudemonData data;
// 			if(pPetData->GetData(data))
// 			{
// 				if(msg.Create(data))
// 					this->SendMsg(&msg);
// 			}
// 		}
// 	}
// 	else
// 	{
		CMsgItemInfo msg;
		if(msg.Create(pItem))
			this->SendMsg(&msg);
//	}
	return true;
}
//-------------------------------------
//20071214修罗:不再直接使用CMsgItemInfo来同步.包装一下.
BOOL CUser::SynchroItem(CItem* pItem,
						int nItemInfoAction,//用于CMsgItemInfo的action
						OBJID idUser/*=ID_NONE*/,//用于CMsgItemInfo
						int nWpslAction/*=NULL*/)//用于CMsgWeaponSoul的action
{
	//物品
// 	if(IETMTYPE_EUDEMON == CItem::GetItemType(pItem->GetTypeID()))
// 	{
// 		if(pItem->GetStateWPSL())
// 		{
// 			m_eudemonData[2].itemid = 0;
// 			if(LoadEudemon(pItem->GetStateWPSL(),&m_eudemonData[2]) && m_eudemonData[2].id != ID_NONE)
// 			{
// 				if(m_eudemonData[2].stutes == 1)
// 				{
// 					CMsgEudemonInfo msg;
// 					if(msg.Create(m_eudemonData[2]))
// 						SendMsg(&msg);
// 				}
// 			}
// 		}
// 	}

	CMsgItem msg;
	if (msg.Create(ITEMACT_ADD,0)
		&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour()))
	this->SendMsg(&msg);

// 	CMsgItemInfo msg;
// 	CHECKF(msg.Create(pItem, nItemInfoAction,idUser));
// 	this->SendMsg(&msg);
// 
// 	return true;//暂时不要泪灵
// 	//泪灵
// 	CMsgWeaponSoul msgWpsl;
// 	CHECKF(msgWpsl.Create(pItem,nWpslAction,idUser));
// 	this->SendMsg(&msgWpsl);
	return true;
}


//  [1/9/2008 朱斌]	指定某地复活
void CUser::RebornInPos(OBJID nMapID, LONG nX, LONG nY, bool bRebornFlag, int nRecoverPercent)
{
	CHECK(nMapID);

	//  [1/27/2008 朱斌]	在即将杀死对方时,丢个10秒的中毒BUFF给他,然后对方倒地,,,,复活后,发现对方身上依然保留这个BUFF...
	ClearManyStatusWhenDie();

// 	if (this->IsAlive())
// 		return;

	// 与死亡状态冲突，屏蔽 [陈剑飞8/25/2007]
	//SetStatus(STATUS_NORMAL);
	DetachStatus( QueryRole(),STATUS_DIE );// 添加清除死亡状态 [陈剑飞8/25/2007]
	m_bGhost	= false;

	//2007828修罗:复活术接口改成参数-------------
//	if( m_bRebornFlag )//复活术复活“我”
//	{
//		SetAttrib( _USERATTRIB_LIFE,MulDiv( GetMaxLife(),m_nRecoverPercent,100 ),SYNCHRO_TRUE );
//		SetAttrib( _USERATTRIB_MANA,MulDiv( GetMaxMana(),m_nRecoverPercent,100 ),SYNCHRO_TRUE );
//		SetAttrib( _USERATTRIB_SP,MulDiv( GetMaxSp(),m_nRecoverPercent,100 ),SYNCHRO_TRUE );
//
//	}
	if( bRebornFlag )//复活术复活“我”
	{
		SetAttrib( _USERATTRIB_LIFE,MulDiv( GetMaxLife(),nRecoverPercent,100 ),SYNCHRO_TRUE );
		SetAttrib( _USERATTRIB_MANA,MulDiv( GetMaxMana(),nRecoverPercent,100 ),SYNCHRO_TRUE );
		SetAttrib( _USERATTRIB_SP,MulDiv( GetMaxSp(),nRecoverPercent,100 ),SYNCHRO_TRUE );

	}
	//------------------------------------------
	else
	{
		SetAttrib(_USERATTRIB_LIFE, GetMaxLife() / 2, SYNCHRO_TRUE);
		SetAttrib(_USERATTRIB_MANA, GetMaxMana() / 2, SYNCHRO_TRUE);
		SetAttrib( _USERATTRIB_SP,GetMaxSp() / 2,SYNCHRO_TRUE );
		
	}
	SetAttrib( _USERATTRIB_KEEPEFFECT, GetEffect(), SYNCHRO_BROADCAST );
	// [2007-7-21] 朱斌 
	//	因BUG0000841, BUG0000960注掉,复活不发送脸消息
	//	SetAttrib( _USERATTRIB_LOOKFACE, GetLookFace(), SYNCHRO_BROADCAST );

	const bool bMaxLife = true;
	this->BroadcastTeamLife(bMaxLife);
    
	//06.10.13王玉波:因为人物复活后,如果不是同一地图组,会出问题,所以修改代码
	//----------begin------------------------------------------------------------------------
	int nFlyMapResult = -1;
	nFlyMapResult = this->FlyMap(nMapID, nX, nY);
//	StandTurnon();
	// lock
	//if( FLYMAP_NORMAL == nFlyMapResult ),因为在EnterMap里加入了添加PK保护状态,所以现在注释以下这句
	//FLYMAP_MAPGROUP == nFlyMapResult时是切换地图组,此时this已被删除,导致调用以下代码就会出错
	//	CRole::AttachStatus(this->QueryRole(), STATUS_PK_PROTECT, 0, CHGMAP_LOCK_SECS);
	//----------end--------------------------------------------------------------------------
//	m_tLock.Startup(CHGMAP_LOCK_SECS);

	CMsgAction msg;
	if(msg.Create(this->GetID(), this->GetPosX(), this->GetPosY(), this->GetDir(), actionStandBy))
	{
		this->SendMsg(&msg);
	}
}

// BOOL CUser::WeaponSoulOutEx(CItem *pItem, int nIndex, BOOL bSendMail)
// {
// 	CHECKF(nIndex >=0 && nIndex <_MAX_WEAPONSOUL_SIZE );
// 	CHECKF(pItem);
// 	ST_WEAPONSOULDATA stWeaponSoulData;
// 	pItem->GetWpSlInfoByIdx(nIndex, &stWeaponSoulData);
// 	if(!stWeaponSoulData.idtype)
// 		return false;
// 
// 
// 	int nMask = pItem->GetWpSlStateActive();
// 	if( nMask & 1<<nIndex)//如果是激活
// 	{
// 		nMask ^= 1<<nIndex;
// 		CHECKF(nMask>=0 && nMask<100);//只允许两位正数
// 		int nWpSlState = pItem->GetStateWPSL();
// 		nWpSlState = nWpSlState - pItem->GetWpSlStateActive()*10;//去掉原active
// 		nWpSlState = nWpSlState + nMask*10;//加上新active
// 		pItem->SetStateWPSL(nWpSlState);
// 	}
// 	pItem->SetWpSlAmount(pItem->GetWpSlAmount()-1);
// 	pItem->SetIdWPSLByIndex(nIndex,0);
// 	pItem->EraseWpSlByID(stWeaponSoulData.ID);
// 
// 	if(AwardItemByWpSlID( &stWeaponSoulData, bSendMail))
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		IF_OK (msg.Create(pItem, ITEMINFO_UPDATE))
// //			this->SendMsg(&msg);		
// 		this->SynchroItem(pItem,ITEMINFO_UPDATE);
// 		//--------------------------------
// 		return true;
// 	}
// 	
// 	return false;
// }

//  [1/27/2008 朱斌]
void CUser::ChanceDropItemAndEquip(int changeDrop)
{
	return;
	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		CItemPtr pItem = GetEquipItemByPos(i);
		if(pItem == NULL)
			continue;

		if(!pItem->IsChanceDropEquipWhenDead())
			continue;

		if( pItem->GetKidnap() || pItem->IsKidnapWhenPickup())//-------彭峰---2007/04/09---判断绑定装备随机不掉落--
			continue;

		const	DWORD	DROP_CHANCE = changeDrop;
		if(RandGet(100) > DROP_CHANCE)
			continue;
		
		POINT	pos;
		pos.x	= GetPosX();
		pos.y	= GetPosY();
		if (GetMap()->FindDropItemCell(USERDROPITEM_RANGE, &pos))
		{
			DropItem(pItem->GetID());
		}
		else
			break;
		
	}
}

///上线后尝试重建队伍
void CUser::RebuildTeam()
{
	DEBUG_TRY
		///通过自己的id找到所属的队伍,如果存在的话
	CTeam* pTeam	= NULL;
	UINT nTeamCount=UserManager()->GetTeamAmount();
	UINT nMemberCount=0;
	OBJID idMember=ID_NONE;
	for(int i=0;i<nTeamCount;++i)
	{
		pTeam=UserManager()->GetTeamByIndex(i);
		CHECK(pTeam);///绝对不会出错

		for(int i = 0 ;i<_MAX_TEAMAMOUNT ; i++)
		{
			if(pTeam->m_TeamIn[i].id ==GetID())
			{
 				pTeam->SetOnlineStatus(GetID(),true,true);///在线状态设置成上线
 				SetTeam(pTeam);///找到组织了,重建队伍成成功
 				break;
			}
		}
	}
	// 	if(GetTeam())
	// 	{
	// 		///将逻辑从循环中提取出来以便清晰
	// 		///队伍信息同步给自己和队伍中其它人
	// 		SendTeamMemberInfo();
	// 	}
	DEBUG_CATCH("CUser::RebuildTeam()")
}

int CUser::ProcGift1001(IRole* pTarget,int nType, int oldValue/*原始值, 在此值基础上加成或加值*/,int valueMask /*= valueMask_ALL*/)
{	
	if(nType > DOTYPE_petAddMaxLife || nType < DOTYPE_addhp)
		return 0;

	int nAdd = 0;
	for(int i = 0 ; i < m_pMagic->QuerySet()->GetAmount() ;  i++)
	{
		IMagicData* pData = m_pMagic->QuerySet()->GetObjByIndex(i);
		if(!pData)
			continue;
		CMagicTypeData* pType = CMagic::FindMagicType(pData->GetInt(MAGICDATA_TYPE)/*, pData->GetInt(MAGICDATA_LEVEL)*/);
		if(!pType)
			continue;
		if(pType->GetInt(MAGICTYPEDATA_SORT) != 99)
			continue;

		CGiftTreeData* pTreeData = g_pGiftManager->QueryGiftTreeSet()->GetObj(pData->GetInt(MAGICDATA_TYPE));
		if(!pTreeData)
			continue;
		if(pTreeData->GetInt(GIFTTREEDATA_temType) != 1001)
			continue;

		IRecord* pGiftTmpRecord = g_pGiftManager->GetGiftTemplete(pData->GetInt(MAGICDATA_TYPE));

		if(!pGiftTmpRecord)
			continue;

		//有些没影响
		if(pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dotype) != nType)
			continue;
		
		//触发条件判断
		int doValue = pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dodata) + pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dodatainc) *  (pData->GetInt(MAGICDATA_LEVEL) - 1) / 100;
		if(doValue >= 31000)
			doValue = 31000;

		if(doValue >= 30000)
		{
			nAdd += oldValue * ((doValue - 30000) / 100.0f);
		}
		else
		{
			nAdd += doValue;
		}
	}
	if (nAdd<0)
	{
		::LogSave("int CUser::ProcGift1001  返回了一个小于0的值");
		return 0;
	}
	else
		return nAdd;
}

void CUser::UpdateBuffBaseProperty()
{
	DEBUG_TRY
	m_dwBuffAddphyatt = 0;
	m_dwBuffAddmagatt = 0;
	m_dwBuffAddphydef = 0;
	m_dwBuffAddmagdef = 0;
	m_dwBuffAddhit = 0;
	m_dwBuffAdddodge = 0;
	m_dwBuffAddcrihit = 0;
	m_dwBuffAddTough = 0;
	m_dwBuffAddhp = 0;
	m_dwBuffAddmp = 0;
	m_dwBuffAddStr = 0;
	m_dwBuffAddInt = 0;
	m_dwBuffAddSta = 0;
	m_dwBuffAddSpi = 0;
	m_dwBuffAddAgi = 0;

	m_BuffAtt1 = 0;//火
	m_BuffAtt2 = 0;//冰
	m_BuffAtt3 = 0;//玄
	m_BuffAtt4 = 0;//毒
	m_BuffDef1 = 0;//火
	m_BuffDef2 = 0;//冰
	m_BuffDef3 = 0;//玄
	m_BuffDef4 = 0;//毒
//	return;
 	m_dwBuffAddStr += AdjustUserAttrib(GetStrUserOnly(),ADJUSTUSERATTRIB_STR);//力量
 	m_dwBuffAddSta += AdjustUserAttrib(GetStaUserOnly(),ADJUSTUSERATTRIB_STA);//体质
 	m_dwBuffAddSpi += AdjustUserAttrib(GetSpiUserOnly(),ADJUSTUSERATTRIB_SPI);//精神
 	m_dwBuffAddInt += AdjustUserAttrib(GetIntUserOnly(),ADJUSTUSERATTRIB_INT);//智力
 	m_dwBuffAddAgi += AdjustUserAttrib(GetAgiUserOnly(),ADJUSTUSERATTRIB_AGI);//敏捷
 	m_dwBuffAddhp  += AdjustUserAttrib(GetMaxLifeUserOnly(),ADJUSTUSERATTRIB_MAXLIFE);//最大生命值
 	m_dwBuffAddmp  += AdjustUserAttrib(GetMaxManaUserOnly(),ADJUSTUSERATTRIB_MAXMANA);//最大魔法
 	m_dwBuffAddcrihit += AdjustUserAttrib(GetCritUserOnly(),ADJUSTUSERATTRIB_Crit);//暴击
 	m_dwBuffAddphyatt += AdjustUserAttrib(GetAtkUserOnly(),ADJUSTUSERATTRIB_PHYACK);//物攻
 	m_dwBuffAddmagatt += AdjustUserAttrib(GetMgcAtkUserOnly(),ADJUSTUSERATTRIB_MAGACK);//魔攻
 	m_dwBuffAddphydef += AdjustUserAttrib(GetDefUserOnly(),ADJUSTUSERATTRIB_PHYDEF);//物防
 	m_dwBuffAddmagdef += AdjustUserAttrib(GetMagicDefUserOnly(),ADJUSTUSERATTRIB_MAGDEF);//魔防
 	m_dwBuffAddhit	  += AdjustUserAttrib(GetAtkHitRateUserOnly(),ADJUSTUSERATTRIB_HITRANGE);//命中
 	m_dwBuffAdddodge  += AdjustUserAttrib(GetDdgUserOnly(),ADJUSTUSERATTRIB_DOG);//闪避
 	m_dwBuffAddTough  += AdjustUserAttrib(GetToughUserOnly(),ADJUSTUSERATTRIB_TOUGH);//坚韧

	m_BuffAtt1 += AdjustUserAttrib(m_equipAtt1+ m_GiftAtt1 + m_WPSLAtt1, ADJUSTUSERATTRIB_FIREATK);//火
	m_BuffAtt2 += AdjustUserAttrib(m_equipAtt2+ m_GiftAtt2 + m_WPSLAtt2, ADJUSTUSERATTRIB_ICEATK);//冰
	m_BuffAtt3 += AdjustUserAttrib(m_equipAtt3+ m_GiftAtt3 + m_WPSLAtt3, ADJUSTUSERATTRIB_MYSTICATK);//玄
	m_BuffAtt4 += AdjustUserAttrib(m_equipAtt4+ m_GiftAtt4 + m_WPSLAtt4, ADJUSTUSERATTRIB_NOXATK);//毒
	m_BuffDef1 += AdjustUserAttrib(m_equipDef1+ m_GiftDef1,ADJUSTUSERATTRIB_FIREDEF);//火
	m_BuffDef2 += AdjustUserAttrib(m_equipDef2+ m_GiftDef2,ADJUSTUSERATTRIB_ICEDEF);//冰
	m_BuffDef3 += AdjustUserAttrib(m_equipDef3+ m_GiftDef3,ADJUSTUSERATTRIB_MYSTICDEF);//玄
	m_BuffDef4 += AdjustUserAttrib(m_equipDef4+ m_GiftDef4,ADJUSTUSERATTRIB_NOXDEF);//毒
 	DEBUG_CATCH("CUser::UpdateBuffBaseProperty");
}

void CUser::UpdateGiftBaseProperty()
{
	DEBUG_TRY
	m_dwGiftAddStr = 0;
	m_dwGiftAddInt = 0;
	m_dwGiftAddSta = 0;
	m_dwGiftAddSpi = 0;
	m_dwGiftAddAgi = 0;
	m_dwGiftAddhp = 0;
	m_dwGiftAddmp = 0;
	m_dwGiftAddcrihit  = 0;
	m_dwGiftAddphyatt  = 0;
	m_dwGiftAddmagatt  = 0;
	m_dwGiftAddphydef  = 0;
	m_dwGiftAddmagdef  = 0;
	m_dwGiftAddhit     = 0;
    m_dwGiftAdddodge   = 0;
	m_dwGiftAddTough   = 0;
	m_GiftAtt1   = 0;
	m_GiftAtt2   = 0;
	m_GiftAtt3   = 0;
	m_GiftAtt4   = 0;
	m_GiftDef1   = 0;
	m_GiftDef2   = 0;
	m_GiftDef3   = 0;
	m_GiftDef4   = 0;
	//goto 对宠物生效的被动技能,如帮派技能等
	m_GiftPetAtk	= 0;//外功
	m_GiftPetMagAtk	= 0;//内功
	m_GiftPetCrihit	= 0;//暴击
	m_GiftPetMaxLife= 0;//血上限

//	return;
	m_dwGiftAddStr += ProcGift1001(this,DOTYPE_addstr,GetStrUserOnly(),valueMask_CONST);
	m_dwGiftAddSta += ProcGift1001(this,DOTYPE_addsta,GetStaUserOnly(),valueMask_CONST);
	m_dwGiftAddSpi += ProcGift1001(this,DOTYPE_addspi,GetSpiUserOnly(),valueMask_CONST);
	m_dwGiftAddInt += ProcGift1001(this,DOTYPE_addint,GetIntUserOnly(),valueMask_CONST);
	m_dwGiftAddAgi += ProcGift1001(this,DOTYPE_addagi,GetAgiUserOnly(),valueMask_CONST);
	m_dwGiftAddhp = ProcGift1001(this,DOTYPE_addhp,GetMaxLifeUserOnly(),valueMask_CONST);
	m_dwGiftAddmp = ProcGift1001(this,DOTYPE_addmp,GetMaxManaUserOnly(),valueMask_CONST);
	m_dwGiftAddcrihit = ProcGift1001(this,DOTYPE_addcrihit,GetCritUserOnly(),valueMask_CONST);
	m_dwGiftAddphyatt = ProcGift1001(this,DOTYPE_addphyatt,GetAtkUserOnly(),valueMask_CONST);
	m_dwGiftAddmagatt = ProcGift1001(this,DOTYPE_addmagatt,GetMgcAtkUserOnly(),valueMask_CONST);
	m_dwGiftAddphydef = ProcGift1001(this,DOTYPE_addphydef,GetDefUserOnly(),valueMask_CONST) + ProcGift1001(this,DOTYPE_addPANDMDEF,0,valueMask_CONST);
	m_dwGiftAddmagdef = ProcGift1001(this,DOTYPE_addmagdef,GetMagicDefUserOnly(),valueMask_CONST) + ProcGift1001(this,DOTYPE_addPANDMDEF,0,valueMask_CONST);
	m_dwGiftAddhit = ProcGift1001(this,DOTYPE_addhit,GetAtkHitRateUserOnly(),valueMask_CONST) + ProcGift1001(this,DOTYPE_addHANDD,0,valueMask_CONST);
	m_dwGiftAdddodge = ProcGift1001(this,DOTYPE_adddodge,GetDdgUserOnly(),valueMask_CONST) + ProcGift1001(this,DOTYPE_addHANDD,0,valueMask_CONST);
	m_dwGiftAddTough = ProcGift1001(this,DOTYPE_addTough,GetToughUserOnly(),valueMask_CONST);
	m_GiftAtt1 = ProcGift1001(this,DOTYPE_addAtt1,0,valueMask_CONST) + ProcGift1001(this,DOTYPE_addAllAtt,0,valueMask_CONST);
	m_GiftAtt2 = ProcGift1001(this,DOTYPE_addAtt2,0,valueMask_CONST) + ProcGift1001(this,DOTYPE_addAllAtt,0,valueMask_CONST);
	m_GiftAtt3 = ProcGift1001(this,DOTYPE_addAtt3,0,valueMask_CONST) + ProcGift1001(this,DOTYPE_addAllAtt,0,valueMask_CONST);
	m_GiftAtt4 = ProcGift1001(this,DOTYPE_addAtt4,0,valueMask_CONST) + ProcGift1001(this,DOTYPE_addAllAtt,0,valueMask_CONST);
	m_GiftDef1 = ProcGift1001(this,DOTYPE_addDef1,0,valueMask_CONST) + ProcGift1001(this,DOTYPE_addAllDef,0,valueMask_CONST);
	m_GiftDef2 = ProcGift1001(this,DOTYPE_addDef2,0,valueMask_CONST) + ProcGift1001(this,DOTYPE_addAllDef,0,valueMask_CONST);
	m_GiftDef3 = ProcGift1001(this,DOTYPE_addDef3,0,valueMask_CONST) + ProcGift1001(this,DOTYPE_addAllDef,0,valueMask_CONST);
	m_GiftDef4 = ProcGift1001(this,DOTYPE_addDef4,0,valueMask_CONST) + ProcGift1001(this,DOTYPE_addAllDef,0,valueMask_CONST);
	//goto 帮派的那几个特殊被动技能,策划要求所有帮众都可以使用此类帮派技能
//	int nSynRank = QuerySynRank();
//	if (GetSynID() != ID_NONE)
//	{
//		if (nSynRank%10==1 || nSynRank>=90)//青龙堂
//		{
//			if (QueryMagic(2103))
				m_GiftPetAtk += ProcGift1001(this,DOTYPE_petAddAtk,0,valueMask_CONST);
//		}
//		if (nSynRank%10==2 || nSynRank>=90)//白虎堂
//		{
//			if (QueryMagic(2203))
				m_GiftPetCrihit += ProcGift1001(this,DOTYPE_petAddCrihit,0,valueMask_CONST);
//		}
//		if (nSynRank%10==3 || nSynRank>=90)//玄武堂
//		{
//			if (QueryMagic(2303))
				m_GiftPetMagAtk += ProcGift1001(this,DOTYPE_petAddMagAtk,0,valueMask_CONST);
//		}
//		if (nSynRank%10==4 || nSynRank>=90)//朱雀堂
//		{
//			if (QueryMagic(2403))
				m_GiftPetMaxLife += ProcGift1001(this,DOTYPE_petAddMaxLife,0,valueMask_CONST);
//		}
//	}

	//goto 检查各数值的正确性
	if (m_dwGiftAddStr < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddStr=%d", m_dwGiftAddStr);
		m_dwGiftAddStr = 0;
	}
	if (m_dwGiftAddInt < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddInt=%d", m_dwGiftAddInt);
		m_dwGiftAddInt = 0;
	}
	if (m_dwGiftAddSta < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddSta=%d", m_dwGiftAddSta);
		m_dwGiftAddSta = 0;
	}
	if (m_dwGiftAddSpi < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddSpi=%d", m_dwGiftAddSpi);
		m_dwGiftAddSpi = 0;
	}
	if (m_dwGiftAddAgi < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddAgi=%d", m_dwGiftAddAgi);
		m_dwGiftAddAgi = 0;
	}
	if (m_dwGiftAddhp < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddhp=%d", m_dwGiftAddhp);
		m_dwGiftAddhp = 0;
	}
	if (m_dwGiftAddmp < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddmp=%d", m_dwGiftAddmp);
		m_dwGiftAddmp = 0;
	}
	if (m_dwGiftAddcrihit < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddcrihit=%d", m_dwGiftAddcrihit);
		m_dwGiftAddcrihit = 0;
	}
	if (m_dwGiftAddphyatt < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddphyatt=%d", m_dwGiftAddphyatt);
		m_dwGiftAddphyatt = 0;
	}
	if (m_dwGiftAddmagatt < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddmagatt=%d", m_dwGiftAddmagatt);
		m_dwGiftAddmagatt = 0;
	}
	if (m_dwGiftAddphydef < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddphydef=%d", m_dwGiftAddphydef);
		m_dwGiftAddphydef = 0;
	}
	if (m_dwGiftAddmagdef < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddmagdef=%d", m_dwGiftAddmagdef);
		m_dwGiftAddmagdef = 0;
	}
	if (m_dwGiftAddhit < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddhit=%d", m_dwGiftAddhit);
		m_dwGiftAddhit = 0;
	}
	if (m_dwGiftAdddodge < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAdddodge=%d", m_dwGiftAdddodge);
		m_dwGiftAdddodge = 0;
	}
	if (m_dwGiftAddTough < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_dwGiftAddTough=%d", m_dwGiftAddTough);
		m_dwGiftAddTough = 0;
	}
	if (m_GiftAtt1 < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_GiftAtt1=%d", m_GiftAtt1);
		m_GiftAtt1 = 0;
	}
	if (m_GiftAtt2 < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_GiftAtt2=%d", m_GiftAtt2);
		m_GiftAtt2 = 0;
	}
	if (m_GiftAtt3 < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_GiftAtt3=%d", m_GiftAtt3);
		m_GiftAtt3 = 0;
	}
	if (m_GiftAtt4 < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_GiftAtt4=%d", m_GiftAtt4);
		m_GiftAtt4 = 0;
	}
	if (m_GiftDef1 < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_GiftDef1=%d", m_GiftDef1);
		m_GiftDef1 = 0;
	}
	if (m_GiftDef2 < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_GiftDef2=%d", m_GiftDef2);
		m_GiftDef2 = 0;
	}
	if (m_GiftDef3 < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_GiftDef3=%d", m_GiftDef3);
		m_GiftDef3 = 0;
	}
	if (m_GiftDef4 < 0)
	{
		::LogSave("CUser::UpdateGiftBaseProperty unexpected value m_GiftDef4=%d", m_GiftDef4);
		m_GiftDef4 = 0;
	}
 	DEBUG_CATCH("CUser::UpdateGiftBaseProperty");
}

BOOL CUser::SynchroUserProperty(UPDATECALTYPE nType)
{
	m_dwUserphyatt = 0;
	m_dwUsermagatt = 0;
	m_dwUserphydef = 0;
	m_dwUsermagdef = 0;
	m_dwUserhit = 0;
	m_dwUserdodge = 0;
	m_dwUsercrihit = 0;
	m_dwUserTough = 0;
	m_dwUserhp = 0;
	m_dwUsermp = 0;
	m_dwUserStr = 0;
	m_dwUserInt = 0;
	m_dwUserSta = 0;
	m_dwUserSpi = 0;
	m_dwUserAgi = 0;
	
	m_dwUserAtt1 = 0;
	m_dwUserAtt2 = 0;
	m_dwUserAtt3 = 0;
	m_dwUserAtt4 = 0;
	m_dwUserDef1 = 0;
	m_dwUserDef2 = 0;
	m_dwUserDef3 = 0;
	m_dwUserDef4 = 0;
// 	m_dwUserphyatt	= GetAtkUserOnly()  + m_equipPhysicAttackTotal + m_dwGiftAddphyatt+ m_dwBuffAddphyatt;
// 	m_dwUsermagatt	= GetMgcAtkUserOnly() + m_equipMagicAttackTotal+ m_dwGiftAddmagatt + m_dwBuffAddmagatt;
// 	m_dwUserphydef	= GetDefUserOnly() + m_equipPhysicDefenseTotal+ m_dwGiftAddphydef + m_dwBuffAddphydef;
// 	m_dwUsermagdef	= GetMagicDefUserOnly() + m_equipMagicDefenseTotal+ m_dwGiftAddmagdef + m_dwBuffAddmagdef;
// 	m_dwUserhit		= GetAtkHitRateUserOnly() + m_equipHitarteTotal+ m_dwGiftAddhit + m_dwBuffAddhit;
// 	m_dwUserdodge	= GetDdgUserOnly() + m_equipDodgeTotal+ m_dwGiftAdddodge + m_dwBuffAdddodge;
// 	m_dwUsercrihit	= GetCritUserOnly() + m_equipCrihitTotal+ m_dwGiftAddcrihit + m_dwBuffAddcrihit;
// 	m_dwUserTough	= GetToughUserOnly() + m_equipToughTotal+ m_dwGiftAddTough + m_dwBuffAddTough;
// 	m_dwUserhp		= GetMaxLifeUserOnly() + m_equipAddhpTotal+ m_dwGiftAddhp + m_dwBuffAddhp;
// 	m_dwUsermp		= GetMaxManaUserOnly() + m_equipAddmpTotal+ m_dwGiftAddmp + m_dwBuffAddmp;
	if (m_dwBuffAddStr<0 && (GetStrUserOnly() + m_equipStrTotal+ m_dwGiftAddStr + m_WPSLStr)<abs(m_dwBuffAddStr))//防止无符号相减后溢出
		m_dwUserStr = 0;
	else
		m_dwUserStr	= GetStrUserOnly() + m_equipStrTotal+ m_dwGiftAddStr + m_dwBuffAddStr + m_WPSLStr;

	if (m_dwBuffAddInt<0 && (GetIntUserOnly() + m_equipIntTotal+ m_dwGiftAddInt + m_WPSLInt)<abs(m_dwBuffAddInt))
		m_dwUserInt = 0;
	else
		m_dwUserInt	= GetIntUserOnly() + m_equipIntTotal+ m_dwGiftAddInt + m_dwBuffAddInt + m_WPSLInt;

	if (m_dwBuffAddSta<0 && (GetStaUserOnly() + m_equipStaTotal+ m_dwGiftAddSta + m_WPSLSta)<abs(m_dwBuffAddSta))
		m_dwUserSta = 0;
	else
		m_dwUserSta	= GetStaUserOnly() + m_equipStaTotal+ m_dwGiftAddSta + m_dwBuffAddSta + m_WPSLSta;

	if (m_dwBuffAddSpi<0 && (GetSpiUserOnly() + m_equipSpiTotal+ m_dwGiftAddSpi + m_WPSLSpi)<abs(m_dwBuffAddSpi))
		m_dwUserSpi = 0;
	else
		m_dwUserSpi	= GetSpiUserOnly() + m_equipSpiTotal+ m_dwGiftAddSpi + m_dwBuffAddSpi + m_WPSLSpi;

	if (m_dwBuffAddAgi<0 && (GetAgiUserOnly() + m_equipAgiTotal+ m_dwGiftAddAgi + m_WPSLAgi)<abs(m_dwBuffAddAgi))
		m_dwUserAgi = 0;
	else
		m_dwUserAgi	= GetAgiUserOnly() + m_equipAgiTotal+ m_dwGiftAddAgi + m_dwBuffAddAgi + m_WPSLAgi;

	if (m_BuffAtt1<0 && (m_equipAtt1+ m_GiftAtt1 + m_WPSLAtt1 + m_eudemonSoul1)<abs(m_BuffAtt1))
		m_dwUserAtt1 = 0;
	else
		m_dwUserAtt1 = m_equipAtt1+ m_GiftAtt1 + m_BuffAtt1 + m_WPSLAtt1 + m_eudemonSoul1;//火

	if (m_BuffAtt2<0 && (m_equipAtt2+ m_GiftAtt2 + m_WPSLAtt2 + m_eudemonSoul2)<abs(m_BuffAtt2))
		m_dwUserAtt2 = 0;
	else
		m_dwUserAtt2 = m_equipAtt2+ m_GiftAtt2 + m_BuffAtt2 + m_WPSLAtt2 + m_eudemonSoul2;//冰

	if (m_BuffAtt3<0 && (m_equipAtt3+ m_GiftAtt3 + m_WPSLAtt3 + m_eudemonSoul3)<abs(m_BuffAtt3))
		m_dwUserAtt3 = 0;
	else
		m_dwUserAtt3 = m_equipAtt3+ m_GiftAtt3 + m_BuffAtt3 + m_WPSLAtt3 + m_eudemonSoul3;//玄

	if (m_BuffAtt4<0 && (m_equipAtt4+ m_GiftAtt4 + m_WPSLAtt4 + m_eudemonSoul4)<abs(m_BuffAtt4))
		m_dwUserAtt4 = 0;
	else
		m_dwUserAtt4 = m_equipAtt4+ m_GiftAtt4 + m_BuffAtt4 + m_WPSLAtt4 + m_eudemonSoul4;//毒

	if (m_BuffDef1<0 && (m_equipDef1+ m_GiftDef1)<abs(m_BuffDef1))
		m_dwUserDef1 = 0;
	else
		m_dwUserDef1 = m_equipDef1+ m_GiftDef1 + m_BuffDef1;

	if (m_BuffDef2<0 && (m_equipDef2+ m_GiftDef2)<abs(m_BuffDef2))
		m_dwUserDef2 = 0;
	else
		m_dwUserDef2 = m_equipDef2+ m_GiftDef2 + m_BuffDef2;

	if (m_BuffDef3<0 && (m_equipDef3+ m_GiftDef3)<abs(m_BuffDef3))
		m_dwUserDef3 = 0;
	else
		m_dwUserDef3 = m_equipDef3+ m_GiftDef3 + m_BuffDef3;

	if (m_BuffDef4<0 && (m_equipDef4+ m_GiftDef4)<abs(m_BuffDef4))
		m_dwUserDef4 = 0;
	else
		m_dwUserDef4 = m_equipDef4+ m_GiftDef4 + m_BuffDef4;

	//goto 检查各数值的正确性
	if (m_dwUserStr<0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserStr=%d", m_dwUserStr);
		m_dwUserStr = 0;
	}
	if (m_dwUserInt < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserInt=%d", m_dwUserInt);
		m_dwUserInt = 0;
	}
	if (m_dwUserSta < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserSta=%d", m_dwUserSta);
		m_dwUserSta = 0;
	}
	if (m_dwUserSpi < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserSpi=%d", m_dwUserSpi);
		m_dwUserSpi = 0;
	}
	if (m_dwUserAgi < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserAgi=%d", m_dwUserAgi);
		m_dwUserAgi = 0;
	}
	if (m_dwUserAtt1 < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserAtt1=%d", m_dwUserAtt1);
		m_dwUserAtt1 = 0;
	}
	if (m_dwUserAtt2 < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserAtt2=%d", m_dwUserAtt2);
		m_dwUserAtt2 = 0;
	}
	if (m_dwUserAtt3 < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserAtt3=%d", m_dwUserAtt3);
		m_dwUserAtt3 = 0;
	}
	if (m_dwUserAtt4 < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserAtt4=%d", m_dwUserAtt4);
		m_dwUserAtt4 = 0;
	}
	if (m_dwUserDef1 < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserDef1=%d", m_dwUserDef1);
		m_dwUserDef1 = 0;
	}
	if (m_dwUserDef2 < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserDef2=%d", m_dwUserDef2);
		m_dwUserDef2 = 0;
	}
	if (m_dwUserDef3 < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserDef3=%d", m_dwUserDef3);
		m_dwUserDef3 = 0;
	}
	if (m_dwUserDef4 < 0)
	{
		::LogSave("CUser::SynchroUserProperty  unexpected value m_dwUserDef4=%d", m_dwUserDef4);
		m_dwUserDef4 = 0;
	}

	//同步血蓝怒
	int nMaxLife = this->GetMaxLife();
	int nMaxMana = this->GetMaxMana();
	int nMaxSp = this->GetMaxSp();
	
	CMsgUserAttrib	msgAtt;

	//属性先发
	msgAtt.Create(GetID(),_USERATTRIB_ADD_ATT1,m_dwUserAtt1);
	msgAtt.Append(_USERATTRIB_ADD_ATT2,m_dwUserAtt2);
	msgAtt.Append(_USERATTRIB_ADD_ATT3,m_dwUserAtt3);
	msgAtt.Append(_USERATTRIB_ADD_ATT4,m_dwUserAtt4);
	msgAtt.Append(_USERATTRIB_ADD_DEF1,m_dwUserDef1);
	msgAtt.Append(_USERATTRIB_ADD_DEF2,m_dwUserDef2);
	msgAtt.Append(_USERATTRIB_ADD_DEF3,m_dwUserDef3);
	msgAtt.Append(_USERATTRIB_ADD_DEF4,m_dwUserDef4);
	this->SendMsg(&msgAtt);


	msgAtt.Create(GetID(), _USERATTRIB_MAXLIFE, nMaxLife) ;
	msgAtt.Append(_USERATTRIB_MAXMANA, nMaxMana);
	msgAtt.Append(_USERATTRIB_MAXSP, nMaxSp);
	BroadcastRoomMsg(&msgAtt, EXCLUDE_SELF,EM_SENDNPCFLAG_FALSE);//排除自己
	
	if (m_data.GetLife() > nMaxLife)
	{
		SetAttrib(_USERATTRIB_LIFE, nMaxLife, SYNCHRO_TRUE);
	}
	
	if(m_data.GetMana() > nMaxMana)
	{
		SetAttrib(_USERATTRIB_MANA, nMaxMana, SYNCHRO_TRUE);
	}
	
	if(m_data.GetSp() > nMaxSp)
	{
		SetAttrib(_USERATTRIB_SP, nMaxSp, SYNCHRO_TRUE);
	}

	m_dwWarSroce    = (GetAtk() + GetMgcAtk()) * 10;
	m_dwWarSroce   += (GetDef() + GetMagicDef()) * 3.33f;
	m_dwWarSroce   += (GetCrit() + GetAtkHitRate() + GetDdg()) * 33.3f;
	m_dwWarSroce   += GetTough() * 30;
	m_dwWarSroce   += GetMaxLife() * 2;
	m_dwWarSroce   += GetMaxMana();
	m_dwWarSroce   += (GetJobLev(1) + GetJobLev(2)) * 200;
	m_dwWarSroce   += m_pMagic->GetMagicScore() * 16;
	m_dwWarSroce   += (GetAddAttByType(1) + GetAddAttByType(2) + GetAddAttByType(3) + GetAddAttByType(4)) * 60;
	m_dwWarSroce   += (GetAddAttByType(11) + GetAddAttByType(12) + GetAddAttByType(13) + GetAddAttByType(14)) * 12;
	m_dwWarSroce   /= 300;
	

	if(GetWeaponR() && GetWeaponR()->GetTypeID() != 144000)
	{
		if(m_data.GetMaxScore() < m_dwWarSroce)
			m_data.SetMaxScore(m_dwWarSroce);
		if(m_data.GetMaxPhyatt() < GetAtk()/* - m_dwBuffAddphyatt*/)//GetAtkUserOnly()  + m_equipPhysicAttackTotal + m_dwGiftAddphyatt)
			m_data.SetMaxPhyatt(GetAtk()/* - m_dwBuffAddphyatt*/);//GetAtkUserOnly()  + m_equipPhysicAttackTotal + m_dwGiftAddphyatt);
		if(m_data.GetMaxMagatt() < GetMgcAtk()/* - m_dwBuffAddmagatt*/)//GetMgcAtkUserOnly() + m_equipMagicAttackTotal+ m_dwGiftAddmagatt)
			m_data.SetMaxMagatt(GetMgcAtk()/* - m_dwBuffAddmagatt*/);//GetMgcAtkUserOnly() + m_equipMagicAttackTotal+ m_dwGiftAddmagatt);
		if(m_data.GetMaxMaxlife() < GetMaxLife()/* - m_dwBuffAddhp*/)//GetMaxLifeUserOnly() + m_equipAddhpTotal+ m_dwGiftAddhp)
			m_data.SetMaxMaxlife(GetMaxLife()/* - m_dwBuffAddhp*/);//GetMaxLifeUserOnly() + m_equipAddhpTotal+ m_dwGiftAddhp);
		if(m_data.GetMaxCrit() < GetCrit()/* - m_dwBuffAddcrihit*/)//GetCritUserOnly() + m_equipCrihitTotal+ m_dwGiftAddcrihit)
			m_data.SetMaxCrit(GetCrit()/* - m_dwBuffAddcrihit*/);//GetCritUserOnly() + m_equipCrihitTotal+ m_dwGiftAddcrihit);
		if(m_data.GetMaxHitrate() < GetAtkHitRate()/* - m_dwBuffAddhit*/)//GetAtkHitRateUserOnly() + m_equipHitarteTotal+ m_dwGiftAddhit)
			m_data.SetMaxHitrate(GetAtkHitRate()/* - m_dwBuffAddhit*/);//GetAtkHitRateUserOnly() + m_equipHitarteTotal+ m_dwGiftAddhit);
	}


	CMsgUserAttrib	msg;	
// 	if(UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)
// 	{
		if(GetCurJob() == 1)
		{
			msg.Create(GetID(),_USERATTRIB_PHYATT,GetAtk());//GetAtkUserOnly());
			msg.Append(_USERATTRIB_MAGATT,GetMgcAtk());//GetMgcAtkUserOnly());
			msg.Append(_USERATTRIB_PHYDEF,GetDef());//GetDefUserOnly());
			msg.Append(_USERATTRIB_MAGDEF,GetMagicDef());//GetMagicDefUserOnly());
			msg.Append(_USERATTRIB_HIT,GetAtkHitRate());//GetAtkHitRateUserOnly());
			msg.Append(_USERATTRIB_DODGE,GetDdg());//GetDdgUserOnly());
			msg.Append(_USERATTRIB_CRIT,GetCrit());//GetCritUserOnly());
			msg.Append(_USERATTRIB_TOUGHT,GetTough());//GetToughUserOnly());
			msg.Append(_USERATTRIB_MAXLIFE,GetMaxLife());//GetMaxLifeUserOnly());
			msg.Append(_USERATTRIB_MAXMANA,GetMaxMana());//GetMaxManaUserOnly());
			msg.Append(_USERATTRIB_MAXSP,GetMaxSp());
			msg.Append(_USERATTRIB_STR,GetStr());//GetStrUserOnly());
			msg.Append(_USERATTRIB_INT,GetIntex());//GetIntUserOnly());
			msg.Append(_USERATTRIB_STA,GetSta());//GetStaUserOnly());
			msg.Append(_USERATTRIB_SPI,GetSpi());//GetSpiUserOnly());
			msg.Append(_USERATTRIB_AGI,GetAgi());//GetAgiUserOnly());
			msg.Append(_USERATTRIB_ADDPOINT,GetAddPoint());
		}
		else
		{
			msg.Create(GetID(),_USERATTRIB_PHYATT_SEC,GetAtk());
			msg.Append(_USERATTRIB_MAGATT_SEC,GetMgcAtk());
			msg.Append(_USERATTRIB_PHYDEF_SEC,GetDef());
			msg.Append(_USERATTRIB_MAGDEF_SEC,GetMagicDef());
			msg.Append(_USERATTRIB_HIT_SEC,GetAtkHitRate());
			msg.Append(_USERATTRIB_DODGE_SEC,GetDdg());
			msg.Append(_USERATTRIB_CRIT_SEC,GetCrit());
			msg.Append(_USERATTRIB_TOUGHT_SEC,GetTough());
			msg.Append(_USERATTRIB_MAXLIFE_SEC,GetMaxLife());
			msg.Append(_USERATTRIB_MAXMANA_SEC,GetMaxMana());
			msg.Append(_USERATTRIB_MAXSP_SEC,GetMaxSp());
			msg.Append(_USERATTRIB_STR_SEC,GetStr());
			msg.Append(_USERATTRIB_INT_SEC,GetIntex());
			msg.Append(_USERATTRIB_STA_SEC,GetSta());
			msg.Append(_USERATTRIB_SPI_SEC,GetSpi());
			msg.Append(_USERATTRIB_AGI_SEC,GetAgi());
			msg.Append(_USERATTRIB_ADDPOINT_SEC,GetAddPoint());
		}
		
		msg.Append(_USERATTRIB_WARSOOCE,m_dwWarSroce);
		this->SendMsg(&msg);
//	}
// 	if(UPDATECALTYPE_EQUIP == nType || UPDATECALTYPE_GIFT == nType ||UPDATECALTYPE_BUFF == nType || UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)
// 	{
// 		msg.Create(GetID(),_USERATTRIB_ADD_PHYATT,m_equipPhysicAttackTotal + m_dwGiftAddphyatt+ m_dwBuffAddphyatt);
// 		msg.Append(_USERATTRIB_ADD_MAGATT,m_equipMagicAttackTotal+ m_dwGiftAddmagatt + m_dwBuffAddmagatt);
// 		msg.Append(_USERATTRIB_ADD_PHYDEF,m_equipPhysicDefenseTotal+ m_dwGiftAddphydef + m_dwBuffAddphydef);
// 		msg.Append(_USERATTRIB_ADD_MAGDEF,m_equipMagicDefenseTotal+ m_dwGiftAddmagdef + m_dwBuffAddmagdef);
// 		msg.Append(_USERATTRIB_ADD_HIT,m_equipHitarteTotal+ m_dwGiftAddhit + m_dwBuffAddhit);
// 		msg.Append(_USERATTRIB_ADD_DODGE,m_equipDodgeTotal+ m_dwGiftAdddodge + m_dwBuffAdddodge);
// 		msg.Append(_USERATTRIB_ADD_CRIT,m_equipCrihitTotal+ m_dwGiftAddcrihit + m_dwBuffAddcrihit);
// 		msg.Append(_USERATTRIB_ADD_TOUGHT,m_equipToughTotal+ m_dwGiftAddTough + m_dwBuffAddTough);
// 		msg.Append(_USERATTRIB_ADD_MAXLIFE,m_equipAddhpTotal+ m_dwGiftAddhp + m_dwBuffAddhp);
// 		msg.Append(_USERATTRIB_ADD_MAXMANA,m_equipAddmpTotal+ m_dwGiftAddmp + m_dwBuffAddmp);
// 		msg.Append(_USERATTRIB_ADD_STR,m_equipStrTotal+ m_dwGiftAddStr + m_dwBuffAddStr);
// 		msg.Append(_USERATTRIB_ADD_INT,m_equipIntTotal+ m_dwGiftAddInt + m_dwBuffAddInt);
// 		msg.Append(_USERATTRIB_ADD_STA,m_equipStaTotal+ m_dwGiftAddSta + m_dwBuffAddSta);
// 		msg.Append(_USERATTRIB_ADD_SPI,m_equipSpiTotal+ m_dwGiftAddSpi + m_dwBuffAddSpi);
// 		msg.Append(_USERATTRIB_ADD_AGI,m_equipAgiTotal+ m_dwGiftAddAgi + m_dwBuffAddAgi);
// 		msg.Append(_USERATTRIB_WARSOOCE,m_dwWarSroce);
// 		this->SendMsg(&msg);
// 
// 		msg.Create(GetID(),_USERATTRIB_ADD_ATT1,m_dwUserAtt1);
// 		msg.Append(_USERATTRIB_ADD_ATT2,m_dwUserAtt2);
// 		msg.Append(_USERATTRIB_ADD_ATT3,m_dwUserAtt3);
// 		msg.Append(_USERATTRIB_ADD_ATT4,m_dwUserAtt4);
// 		msg.Append(_USERATTRIB_ADD_DEF1,m_dwUserDef1);
// 		msg.Append(_USERATTRIB_ADD_DEF2,m_dwUserDef2);
// 		msg.Append(_USERATTRIB_ADD_DEF3,m_dwUserDef3);
// 		msg.Append(_USERATTRIB_ADD_DEF4,m_dwUserDef4);
// 		this->SendMsg(&msg);
// 	}
	return true;
}



bool CUser::IsInMapInstance()
{
	CHECKF(GetMap());
	return GetMap()->IsDynaMap();
}


//[游途道标 2008/10/13]设置当前使用的称号
void CUser::SetUseTitle(int nType, int nLev, bool bUpdate)
{
	if (!m_pTitle)
		return;
	if (!m_pTitle->IsValidTitle(nType, nLev))
		return;

	if (GetUseTitle()==nType*100+nLev)
		return;
	m_data.SetUseTitle(nType*100+nLev, bUpdate);
	SetAttrib(_USERATTRIB_USETITLE, m_data.GetUseTitle(), SYNCHRO_BROADCAST);
	//UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
}

//[游途道标 2008/10/13]设置当前拥有的称号
// void CUser::SetHoldTitle(UINT nPower, BOOL bUpdate)
// {
// 	m_data.SetHoldTitle(nPower, bUpdate, Database());
// 	CMsgName msg;
// 	if (msg.Create(NAMEACT_CHANGE_HOLDTITLEOK, GetName(), m_data.GetHoldTitle()))
// 	{
// 		SendMsg( &msg );
// 	}
// }

void CUser::AddHoldTitle(UINT nType, int nLev/*=1*/)
{
	if (!m_pTitle)
		return;
	m_pTitle->AddTitle(nType, nLev);
// 	m_data.AddHoldTitle(nPower, bUpdate, Database());
// 	CMsgName msg;
// 	if (msg.Create(NAMEACT_CHANGE_HOLDTITLEOK, GetName(), m_data.GetHoldTitle()))
// 	{
// 		SendMsg( &msg );
// 	}
}

void CUser::DelHoldTitle(UINT nType)
{
	if (!m_pTitle)
		return;
	m_pTitle->DelTitleByType(nType);
// 	m_data.SubHoldTitle(nPower, bUpdate, Database());
// 	CMsgName msg;
// 	if (msg.Create(NAMEACT_CHANGE_HOLDTITLEOK, GetName(), m_data.GetHoldTitle()))
// 	{
// 		SendMsg( &msg );
// 	}
}	

// int CUser::CmpHoldTitle(UINT nPower)
// {
// 	return m_data.CmpHoldTitle(nPower);
// }
void CUser::LevUpTitle(UINT nType)
{
	if (!m_pTitle)
		return;
	m_pTitle->TitleUpLevel(nType);
}
int CUser::GetTitleLev(UINT nType)
{
	if (!m_pTitle)
		return 0;
	return m_pTitle->GetTitleLev(nType);
}
void CUser::SetTitleLevel(UINT nType, int nLev)
{
	if (!m_pTitle)
		return;
	if (nLev == 0)
		return;
	m_pTitle->ChangeTitleLevel(nType, nLev);
}
CUserTitleData* CUser::FindTitleByType(UINT nType)
{
	if (!m_pTitle)
		return NULL;
	CUserTitleData* pData = m_pTitle->FindTitle(nType);
	return pData;
}
void CUser::SendAllTitle()
{
	if (!m_pTitle)
		return;
	m_pTitle->SendInfoToClient();
}


//[独孤求败　2008/12/4] 根据客户端传过来的物品ID来获取该物品,并将该物品信息发送到客户端
bool CUser::GetItemInfoByItemid(OBJID idItem)
{
	vector<OBJID>::iterator it;

	it = find(NoItemId().begin(),NoItemId().end(),idItem);

	//已经找到
	if (it != NoItemId().end())  
	{
		this->SendSysMsg("无法获取该道具的信息，请与信息发布者联系!");
		return true;
	}

	//如果没找到
	CItem* pItem = NULL;
	pItem = CItem::CreateNew();
	if (!pItem)
		return false;
	if (!pItem->Create(idItem,Database()))
	{
		pItem->ReleaseByOwner();

		NoItemId().push_back(idItem); //创建失败表示，该物品不存在数据库中，当然如果数据库异常可能也会出现这种情况吧，好像没什么影响的
		this->SendSysMsg("无法获取该道具的信息，请与信息发布者联系!");
		return false;
	}
	CMsgItemInfo msg;
	if (msg.Create(pItem,ITEMACT_VIEWCHATITEMINFO))
	{
		SendMsg(&msg);
		pItem->ReleaseByOwner();
		return true;
	}
	pItem->ReleaseByOwner();
	return false;

}

void CUser::SendNotice(char *pszTalk)
{
	CMsgTalk msg;
	if(msg.Create(_TXTATR_SERVEEX, pszTalk))
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(GetSocketID(), &msg);
}

bool CUser::IsLuxuryBooth()
{
	if (m_pBooth)
	{
		return m_pBooth->GetIDLock() != BOOTH_LOOK;
	}
	return false;
}

void CUser::ClearBooth()
{
	if (m_pBooth)
	{
		m_pBooth->Clear();
	}
}


bool CUser::CheckFreezeDoubleExp(UINT nBufID)//冻结双倍经验
{
	DEBUG_TRY
	if (!QueryStatusManager().CheckRestoreFreezeStatusDB(this, Database(), nBufID))
		return false;
	DEBUG_CATCH( "CUser::CheckFreezeDoubleExp" )	
	return true;
}

bool CUser::FreezeDoubleExp(UINT nBufID)//冻结双倍经验
{
	IStatus* pStates = QueryStatus(nBufID);
	if (!pStates)
		return false;
	pStates->SetFreeze();
	DEBUG_TRY
	if (!QueryStatusManager().WriteStatusDB(this, Database(), pStates))
	{
		return false;
	}

	StatusInfoStruct statusinfo;
	pStates->GetInfo(&statusinfo);
	m_doubletime=statusinfo.nSecs;

	if (!CRole::DetachStatus(this, nBufID))
	{
		pStates->SetUnFreeze();
		return false;
	}
    	
	DEBUG_CATCH( "CUser::FreezeDoubleExp" )	
	return true;
}
bool CUser::RestoreDoubleExp(UINT nBufID)//恢复冻结的双倍经验
{
	DEBUG_TRY
	if (!QueryStatusManager().RestoreFreezeStatusDB(this, Database(), nBufID))
		return false;
	m_doubletime=0;
	DEBUG_CATCH( "CUser::RestoreDoubleExp" )	
	return true;
}
bool CUser::WriteStateToDB(IStatus* pStatus)
{
	bool hr=false;
	DEBUG_TRY
	hr=QueryStatusManager().WriteStatusDB(this,Database(),pStatus);
	DEBUG_CATCH( "CUser::WriteStateToDB" )
	return hr;
}
bool CUser::DelStateFromDB(int nStatus)
{
	DEBUG_TRY
	CString m_strSQL;
	m_strSQL.Format("delete from %s where USER_ID=%d and Status_ID=%d and Param3!=1",_TBL_USERSTATUS,GetID(),nStatus);	
	m_strSQL.MakeUpper();
	Database()->ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	DEBUG_CATCH( "CUser::DelStateFromDB" )
	return true;
}
//新手出生地图分配
OBJID CUser::GetBornMap()
{
	OBJID idMap= 0;
	//新手出生地图的人数,初始化成一个较大的数,以免把不存在的地图当成了人最少的地图
	const short nBornMapAmount = 5;//新手地图数量
	DWORD dwUserAmount[nBornMapAmount] = {9999999, 9999999, 9999999, 9999999, 9999999};
	OBJID idMapArr[nBornMapAmount] = {1001, 1034, 1036, 1038, 1040};//江陵,江陵1,江陵2,江陵3,江陵4
	CGameMap* pMapTemp = NULL;
	const int nAmountlimit = 80;
	for (int i=0; i<nBornMapAmount; i++)
	{
		pMapTemp = MapManager()->GetGameMap(idMapArr[i]);
		if (pMapTemp)
			dwUserAmount[i] = pMapTemp->GetUserAmount();

		if (dwUserAmount[i] < nAmountlimit)
		{
			idMap = idMapArr[i];
			break;
		}
	}
	//策划要求没有地图小于80人时改成随机选择,而不是选人最少的
	if (!idMap)
	{	//随机时不包括江陵0
		int nRan = ::RandGet(400);
		if (nRan < 100)
			idMap = idMapArr[1];
		else if (nRan < 200)
			idMap = idMapArr[2];
		else if (nRan < 300)
			idMap = idMapArr[3];
		else if (nRan < 400)
			idMap = idMapArr[4];
	}
// 	{
// 		vector<MapUserAmount> vMapUserAmount;
// 		MapUserAmount stMap;
// 		stMap.idMap = 1001;
// 		stMap.dwAmount = dwUserAmount1;
// 		vMapUserAmount.push_back(stMap);
// 		stMap.idMap = 1034;
// 		stMap.dwAmount = dwUserAmount2;
// 		vMapUserAmount.push_back(stMap);
// 		stMap.idMap = 1036;
// 		stMap.dwAmount = dwUserAmount3;
// 		vMapUserAmount.push_back(stMap);
// 		sort(vMapUserAmount.begin(), vMapUserAmount.end(), MoreUserThan);
// 		if (vMapUserAmount.size()>0)
// 			idMap = vMapUserAmount[0].idMap;
// 	}
	return idMap;
}

int CUser::RDropEquip(int DropNum)
{
	int nStep = 0;
	DEBUG_TRY
	CHECKF(DropNum > 0);
	int nRealDropNum = 0;
	const int DROP_EQUIP = ITEMPOSITION_EQUIPEND - ITEMPOSITION_EQUIPBEGIN/* + 1*/;
	nStep = 1;
	std::vector<OBJID>	setItems;
	for (int i=1; i<DROP_EQUIP; i++)
	{
		CItem* pEquip =  GetEquipItemByPos(i);

		if (pEquip)
		{
			if(pEquip->GetPosition() != i)
				LOGWARNING("掉物品的时候，物品位置问题，混乱啦？itemid = %u userid = %u",pEquip->GetID(),GetID());
			setItems.push_back(i);
		}
	}
	nStep = 2;
	int nTotalItemCount = setItems.size();
	{
		//srand( ( unsigned )time( NULL ) );
		// 随机选择掉__min(nDropNum, nTotalItemCount)个物品
		for (int i=0; i<nTotalItemCount/*__min(DropNum, nTotalItemCount)*/; i++)
		{
			nStep = 3;
			if( setItems.empty() )
				break;
			if (nRealDropNum >= DropNum)
			{
				break;
			}
			int nIdx = rand() % setItems.size();
			OBJID idItem = setItems[nIdx];
			setItems.erase(setItems.begin()+nIdx);
			nStep = 4;
			CItem* pItem = this->GetEquipItemByPos(idItem);

			nStep = 5;
			if(pItem && pItem->GetPosition() != i)
				LOGWARNING("掉物品的时候，物品位置问题，混乱啦？itemid = %u userid = %u",pItem->GetID(),GetID());

			nStep = 6;
			if (pItem && pItem->IsSoulMark())//goto 魂印的不能掉
				continue;
			nStep = 7;
			if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from red_killed", GetID(), GetName(), pItem->GetTypeID());

			nStep = 8;
			if (pItem)
			{
				if( /*pItem->GetKidnap() ||*/ pItem->IsNeverDropWhenDead())//-------彭峰---2007/04/09---判断绑定装备随机不掉落--
					continue;
				if (this->EraseEquip(idItem, true))
				{
					++nRealDropNum;
					//						m_pOwner->SendSysMsg("作为一个被玩家杀死的罪犯，你将随机掉落身上的一件非绑定的物品");
				}
				else
					break;
			}
			
		}	// end of for loop
	}
	
	return nRealDropNum;
	DEBUG_CATCH3("CUser::RDropEquip(int DropNum) error iduser = %u nStep = %u",GetID(),nStep)
	return 0;
}



DWORD CUser::GetVipLeftMinites()
{
	if(!IsVipActive())
		return 0;
	int nEndDate = m_data.GetVipTime();
	int nYear = 2000 + nEndDate / 100000000;
	int nMounth =  (nEndDate % 100000000) / 1000000;
	int nDay =  (nEndDate % 1000000) / 10000;
	int nHour =  (nEndDate % 10000) / 100;
	int nMin =  nEndDate % 100;
	CTime timeEnd(nYear,nMounth,nDay,nHour,nMin,0);
	CTime timeNow(CTime::GetCurrentTime());
	CTimeSpan span = timeEnd - timeNow;
	int nTotalMin = span.GetTotalMinutes();
	return nTotalMin;

}

void CUser::InitDoubleTime()
{
	CString m_strSQL;
	m_strSQL.Format("select * from %s where USER_ID=%d and Param3=1 and Status_ID=%u",_TBL_USERSTATUS,GetID(),STATUS_GETEXPUP_PERCENT);
	m_strSQL.MakeUpper();
	IRecordset* pNewRecordset=Database()->CreateNewRecordset(m_strSQL);

	if(!pNewRecordset)
	{
		m_doubletime=0;
		return;
	}

    if(pNewRecordset->RecordCount()>1)
	{
		m_doubletime=0;
		SAFE_RELEASE(pNewRecordset);
		return;
	}
	
	
	m_doubletime=pNewRecordset->LoadInt("RestTime");
	pNewRecordset->Release();
	return;

}

DWORD CUser::GetFrozenDoubleTime()
{
    if(m_doubletime<0)
		return 0;
	return m_doubletime;
}

DWORD CUser::GetAward()
{
	return m_data.GetAwardTime();
}



void CUser::SetAward(DWORD awardtime,bool update)
{
	m_data.SetOnlineAward(awardtime,update);
	CMsgUserAttrib msg_att;
	msg_att.Create(GetID(),_USERATTRIB_UserOnLine_Award,GetAward());
	SendMsg(&msg_att);
}

void CUser::ClearAllDOT()
{
	for(int i =  this->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus =  this->QueryStatusSet()->GetObjByIndex(i);
		if((pStatus->GetID() / 100000 == 2) ||  (pStatus->GetID() / 100000 == 3))
			DetachStatus(this,pStatus->GetID());
	}
	return;
}

void CUser::SendEmail(const char* szRecvUser,const char* szWords, CUser* pTarget)
{
	DEBUG_TRY
	CHECK(this);
	CMsgTalk Msg;
	char szDate[32] = "";
	
	SYSTEMTIME ct = {0};
	unsigned long date = 0;
	GetLocalTime(&ct);
	date = (ct.wYear - 2000)*100000000 + ct.wMonth*1000000 + ct.wDay * 10000 + ct.wHour * 100 + ct.wMinute;
	int Sec = ct.wSecond;
	sprintf(szDate, "20%d%2d", date, Sec);

	CMsgTalk	msg;
	if (msg.Create("系统", this->GetName(), szWords, szDate, 0xff0000, _TXTATR_MAYBEMAIL, 0, 0, 0, " ", 0))
		pTarget->QueryOwnerUser()->SendMsg(&msg);
	DEBUG_CATCH("sendemail crash!");
}

bool CUser::IsAwardTimeOut()
{
// 	if(!m_awardcheck.IsActive())
// 		SendSysMsg("定时器未开始");
// 	if(!m_awardcheck.IsTimeOut())
// 		SendSysMsg("剩余时间%d秒",m_awardcheck.GetRemain());
	return m_awardcheck.IsTimeOut();
}

void CUser::SetNextAwardTime(int nsecs)
{
     m_awardcheck.Clear();
	 m_awardcheck.Startup(nsecs);
}


void CUser::InitAwarTime(bool bLogin)
{

	m_data.setFati(1);			//初始化防沉迷状态，避免未开账号服务器时出错
	if(GetAward()%10==8)
		return;
    if(GetAward()>9)
	{
		SetNextAwardTime(1);
		return;
	}
	switch(GetAward())
	{
	case 0:
		SetNextAwardTime(30);
		break;
	case 1:
		SetNextAwardTime(120);
		break;
	case 2:
		SetNextAwardTime(180);
		break;
	case 3:
		SetNextAwardTime(300);
		break;
	case 4:
		SetNextAwardTime(600);
		break;
	case 5:
		SetNextAwardTime(900);
		break;
	case 6:
		SetNextAwardTime(1200);
		break;
	case 7:
		SetNextAwardTime(1800);
		break;
	default :
		break;
	}
}

void CUser::ProcessFatigue()
{
	int FatiTime;
	FatiTime = m_data.GetFatiTime();
	FatiTime -=1;
	if (FatiTime % 60 == 0)
	{
		this->SendSysMsg(_TXTATR_FATI, "%d", (5-FatiTime/60));
	}
	m_data.SetFatiTime(FatiTime);

	if(FatiTime % 10 == 0)
		m_data.SetFatiTime(FatiTime);
}


OBJID CUser::GetInstenceType()
{
	if(m_pMap)
		
	{
		int mapid = m_pMap->GetID();
		if (m_pMap->IsDynaMap())
		{
			CMapInstance* pInstance = MapInstanceMgr()->FindInstance(mapid);
			//4 TEST
			int iType = pInstance->GetMapType();
			return pInstance->GetMapType();
		}
	}
	return ID_NONE;
}


void CUser::IgotAnAmazingEudemon(int nEudemonType, CEudemonTypeData*	pEudemonType, char* szName, OBJID EudId)
{	
	DEBUG_TRY
	{
		char szAwardMsg[128]  = "";
		sprintf(szAwardMsg,"你获得%s宠物",szName);
		SendSysMsg(szAwardMsg);

		if (!nEudemonType)
		{
			return;
		}
		char UserName[64] = "";
		char itemName[64] = "";
		char szMsg[128]  = "";
		sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());

		if (nEudemonType == 917)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
			SNAPSHOTUSEREDU,  szName ,EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>惊讶的看着从商城里精心挑选的宝贝出了神，这就是梦寐以求的<%s>啊，真是太可爱了！", UserName ,itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
		}
		if (nEudemonType == 918)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
			SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>早就听说商城里内藏珍稀宝物，不料此番竟然买到<%s>这样唯一的人形宠物，真是令人羡慕！", UserName ,itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
		}
		if (nEudemonType == 919)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>给<3_草泥马大王_7>造成了致命的伤害，<3_草泥马大王_7>头也不回的就跑了，留下一个<%s>成为众位英雄儿女的焦点！", UserName ,itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
		}
		if (925 < nEudemonType && 930 > nEudemonType)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>不惜重金从商城购得江湖中绝世罕见的<%s>，真是令人称羡！", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
		}
		if (nEudemonType == 930)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "开门大吉！一道五彩的光芒照耀在<%s>身上，出现在他眼前的不正是可爱的宝宝<%s>吗？真是鸿运当头，无人能挡！", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
			
		}
		if (nEudemonType == 934)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, " 来自御剑穹苍的传奇王者<%s>驯服了<%s>，莫非他是武林中独步当代的第一人？！", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);	
		}
		if (nEudemonType == 936 || nEudemonType == 937)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>早就听闻商城中内藏祝福珍宝无数，不料此番真的寻得只<%s>，真是好福气！", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);	
		}
		if (nEudemonType == 935)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, " <%s>今日手气不错，轻轻松松就从聚财礼包中寻得<%s>，真是羡煞众人！", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);	

		}

	}
	DEBUG_CATCH2("IgotAn_Amazing_Eudemon, %d", nEudemonType);
}

void CUser::SetVipface(int nData, bool bActivNow /*= true*/, int dwDay/* = 7*/)
{
	DWORD dwHour = dwDay * 24;
	CTimeSpan timeSpan(0,dwHour,0,0);
	if(GetVipface() != nData)
	{
		m_data.SetVipface(nData);
		CTime timeNow(CTime::GetCurrentTime());
		CTime timeThisEnd = timeNow + timeSpan;	
		OBJID TimeNow = (timeThisEnd.GetYear() -2000) * 1000000 + timeThisEnd.GetMonth() * 10000 + timeThisEnd.GetDay() * 100 + timeThisEnd.GetHour();
		SetVipfaceTime(TimeNow);

		if (bActivNow)
		{
			CMsgUserAttrib	msg;
			IF_OK(msg.Create(GetID(), _USERATTRIB_LOOKFACE, nData))
				this->BroadcastRoomMsg(&msg, true);
			this->SendSysMsg("头像购买成功！");
			AnounceNewFaceToFriend(nData);
		}	
	}
	else
	{
		int nEndDate = m_data.GetVipfaceTime();
		int nYear = 2000 + nEndDate / 1000000;
		int nMounth =  (nEndDate % 1000000) / 10000;
		int nDay =  (nEndDate % 10000) / 100;
		int nHour =  (nEndDate % 100);
		CTime timeEndOld(nYear,nMounth,nDay,nHour,0,0);
		CTime timeThisEnd = timeEndOld + timeSpan;
		OBJID TimeNow = (timeThisEnd.GetYear() -2000) * 1000000 + timeThisEnd.GetMonth() * 10000 + timeThisEnd.GetDay() * 100 + timeThisEnd.GetHour();
		SetVipfaceTime(TimeNow);
		this->SendSysMsg("头像增加时间成功！");
	}
}

OBJID CUser::GetVipface()
{
	int nFace = m_data.GetVipface();
	return nFace;
}

OBJID CUser::GetVipFaceTime()
{
	int nTime = m_data.GetVipfaceTime();
	return nTime;
}

void CUser::ClrVipFace()
{
	m_data.SetVipface(0);
	AnounceNewFaceToFriend(GetFace());
}

void CUser::SetVipfaceTime(OBJID nData)
{

	m_data.SetVipfaceTime(nData);
}

void CUser::AnounceNewFaceToFriend(int nNewFace)
{
	for(int i = 0; i < m_setBeFriend.size(); i++)
	{
		CFriend* pFriend = m_setBeFriend[i];
		CUserPtr pTarget = NULL;
		pTarget = UserManager()->GetUser(pFriend->GetUserID());
		if(pTarget)
		{
			CFriend* pBeFriend=pTarget->GetFriend(this->GetID());
			if(pBeFriend)
				pBeFriend->SetFriendFace(nNewFace);
			CMsgFriend pMsg;
			if (pMsg.Create(_FRIEND_SETFACE,this->GetID(),this->GetName(),CMsgFriend::ONLINE_STATUS,nNewFace,0,0,0,0,0,"",0))
				pTarget->SendMsg(&pMsg);
		}
	}
}
//goto 活动奖励状态
USHORT CUser::GetActivityAward(USHORT usType)
{
	if (usType==3)
		return m_nActivityAward;
	else if(usType==4)
		return m_ChargePack1;
	else if(usType==5)
		return m_ChargePack2;
	else if(usType==6)
		return m_ChargePack3;


	return 0;
}

void CUser::SetMobChargeAward(USHORT usType, USHORT usState, bool bSend)
{
	if(!bSend)
	{
		if (usType==4)
		{
			if (usState == m_ChargePack1)
				return;
			m_ChargePack1 = usState;
		}
		
		if (usType==5)
		{
			if (usState == m_ChargePack2)
				return;
			m_ChargePack2 = usState;
		}
		
		if (usType==6)
		{
			if (usState == m_ChargePack3)
				return;
			m_ChargePack3 =  usState;
		}
	}
	if (bSend)
	{
		if (usType==4)
		{
			if (usState == 0||usState>m_ChargePack1)
				return;
		
			m_ChargePack1 = m_ChargePack1-usState;
			CMsgVAS MsgVAS;
			if (MsgVAS.Create(usType, GetAccountID(), GetID(), _MSGVAS_GET_MOBCHARGE,1,m_ChargePack1))
			{
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
			}
		}
		
		if (usType==5)
		{
			if (usState == 0||usState>m_ChargePack2)
				return;
			m_ChargePack2 = m_ChargePack2-usState;
			CMsgVAS MsgVAS;
			if (MsgVAS.Create(usType, GetAccountID(), GetID(), _MSGVAS_GET_MOBCHARGE,1,m_ChargePack2))
			{
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
			}
		}
		
		if (usType==6)
		{
			if (usState == 0||usState>m_ChargePack3)
				return;
			m_ChargePack3 =m_ChargePack3- usState;
			CMsgVAS MsgVAS;
			if (MsgVAS.Create(usType, GetAccountID(), GetID(), _MSGVAS_GET_MOBCHARGE,1,m_ChargePack3))
			{
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
			}
		}

	}
	


}

void CUser::SetActivityAward(USHORT usType, USHORT usState, bool bSend)
{
	if (usType==3)
	{
		if (usState == m_nActivityAward)
			return;
		m_nActivityAward = usState;
	}
	if (bSend)
	{
		CMsgVAS MsgVAS;
		if (MsgVAS.Create(usType, GetAccountID(), GetID(), _MSGVAS_OPRATE_ACTIVITY,1,usState))
		{
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
		}
	}
}

bool CUser::IniUserAutoFight(bool bLogin)
{
	if(m_pAutoFightData.bAutof)
		return true;
	
	IF_NOT(m_pAutoFightData.Create(Database(),this,bLogin,GameDataDefault()->GetAutoFightData()))
		return false;
	
	m_pAutoFightData.bAutof = true;

	return true;
}

void CUser::PickupVotCard(char *szCard)
{
	if(m_tCardMsgTime.IsActive())
	{
		if(!m_tCardMsgTime.ToNextTime())
		{
			SendSysMsg("领取太快，请过会再来领取");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	第一次检查
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);
	
	if(ChkNewGiveMask(36))
	{
		SendSysMsg("你已经领取过投票卡，不能重复领取");
		return;
	}
	
	if(strlen(szCard) != 18)
	{
		SendSysMsg("错误的卡号");
		return;
	}
	
	if(strncmp(szCard,"YJTP-",5) != 0)
	{
		SendSysMsg("错误的卡号");
		return;
	}
	
	CMsgNewCard msg;
	if(msg.Create(VOTCARD_PICK,GetID(),GetAccountID(),szCard))
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
}

bool CUser::GetVotingGift(bool bignorAll/* = false*/)
{
	int nRand = RandGet(100);
	OBJID nAction = 0;
	
	
	if (nRand == 99)
	{
		nAction = 2038;
	}
	else	if (nRand > 93)
	{
		nAction = 2037;
	}
	else
	{
		nAction = 2036;
	}               
	
	if(ProcessAction(nAction))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CUser::PickupVotCard2(char *szCard)
{
	if(m_tCardMsgTime.IsActive())
	{
		if(!m_tCardMsgTime.ToNextTime())
		{
			SendSysMsg("领取太快，请过会再来领取");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	第一次检查
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);

	if(strlen(szCard) != 18)
	{
		SendSysMsg("错误的卡号");
		return;
	}

	int nType = 0;
	if(strncmp(szCard,"LQIS-",5) != 0) //铜
	{
		SendSysMsg("错误的卡号");
		return;
	}

	if(ChkNewGiveMask(37))
	{
		SendSysMsg("你已经领取过工会卡，不能重复领取");
		return;
	}
//		nType = 1;
//	}
// 	else if(strncmp(szCard,"DWAN-",5) == 0) //银
// 	{
// 		if(ChkNewGiveMask(38))
// 		{
// 			SendSysMsg("你已经领取过活动银卡，不能重复领取");
// 			return;
// 		}
// 		nType = 2;
// 	}
// 	else if(strncmp(szCard,"ELWG-",5) == 0) //金
// 	{		
// 		if(ChkNewGiveMask(39))
// 		{
// 			SendSysMsg("你已经领取过活动金卡，不能重复领取");
// 			return;
// 		}
// 		nType = 3;
// 	}
	
// 
// 	if(nType == 0)
// 	{
// 		SendSysMsg("错误的卡号");
// 		return;
// 	}

	CMsgNewCard msg;
	if(msg.Create(VOTCARD2_PICK,GetID(),GetAccountID(),szCard))
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
}

bool CUser::GetVotingGift2(char *szCard)
{
	CHECKF(szCard);
	
	if(strlen(szCard) != 18)
	{
		SendSysMsg("错误的卡号");
		return false;
	}

	int nType = 0;
	
	if(strncmp(szCard,"LQIS-",5) != 0) //铜
		return false;

// 	if(strncmp(szCard,"DWAN-",5) == 0) //银
// 	{
// 		nType = 2;
// 	}
// 	if(strncmp(szCard,"ELWG-",5) == 0) //金
// 	{
// 		nType = 3;
// 	}
// 	
// 	
// 	if(nType == 0)
// 	{
// 		SendSysMsg("错误的卡号");
// 		return false;
// 	}

	
	OBJID nAction = 2039;//2038 + nType;
	if(ProcessAction(nAction))
	{
		AddNewGiveMask(37);
		return true;
	}
	else
	{
		return false;
	}
}

void CUser::PickupVotCard3(char *szCard)
{
	if(m_tCardMsgTime.IsActive())
	{
		if(!m_tCardMsgTime.ToNextTime())
		{
			SendSysMsg("领取太快，请过会再来领取");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	第一次检查
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);

	if(strlen(szCard) != 18)
	{
		SendSysMsg("错误的卡号");
		return;
	}

	if(ChkNewGiveMask(32))
	{
		SendSysMsg("你已经领取过同类型活动卡，不能重复领取");
		return;
	}

	int nType = 0;
	if(strncmp(szCard,"HDWX-",5) == 0) //小
		nType = 1;
	else if(strncmp(szCard,"HDWZ-",5) == 0) //中
		nType = 2;
	else if(strncmp(szCard,"HDWD-",5) == 0) //大
		nType = 3;	

	if(nType == 0)
	{
		SendSysMsg("错误的卡号");
		return;
	}

	CMsgNewCard msg;
	if(msg.Create(VOTCARD3_PICK,GetID(),GetAccountID(),szCard))
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
}

bool CUser::GetVotingGift3(char *szCard)
{
	CHECKF(szCard);
	
	if(strlen(szCard) != 18)
	{
		SendSysMsg("错误的卡号");
		return false;
	}

	int nType = 0;
	
	if(strncmp(szCard,"HDWX-",5) == 0) //小
	{
		nType = 1;
	}
	else if(strncmp(szCard,"HDWZ-",5) == 0) //中
	{
		nType = 2;
	}
	else if(strncmp(szCard,"HDWD-",5) == 0) //大
	{
		nType = 3;
	}
	
	
	if(nType == 0)
	{
		SendSysMsg("错误的卡号");
		return false;
	}

	
	OBJID nAction = 2031 + nType;
	if(ProcessAction(nAction))
	{
		AddNewGiveMask(32);
		return true;
	}
	else
	{
		return false;
	}
}

void CUser::SendFatigue()
{
	if (!isFatigue())
		return;
	if (!g_bEnableFatigue)
	{
		return;
	}
	if (m_data.GetFatiTime())
	{
		SendSysMsg(_TXTATR_FATI, "%d", (5 - m_data.GetFatiTime()/60));
	}
	else
		SendSysMsg(_TXTATR_FATI, "5");
}

void CUser::SetDress(OBJID nDress)
{
	OBJID oldDress =  GetDress();
	if (nDress)
	{
		if(m_pUserKey.SetDress(oldDress | nDress))
			SendSysMsg("时装显示模式已改变");
		else
			SendSysMsg("时装显示模式改变失败");
	}
	else
	{
		if(m_pUserKey.SetDress(oldDress & nDress))
			SendSysMsg("时装显示模式已改变");
		else
			SendSysMsg("时装显示模式改变失败");
	}
}

DWORD CUser::GetDress()
{
	return (m_pUserKey.GetDress());
}

int CUser::GetFightPetUnderstand(CMonster* pMonster)
{
	int Understand = 0;
	DEBUG_TRY
	if(!pMonster)
		return NULL;
	OBJID idEudemon = pMonster->GetID();
	EudemonData data;
	LoadEudemon(idEudemon, data, 2);
	Understand = data.understand;
	DEBUG_CATCH("GetFightPetUnderstand");
	return Understand;
}

int CUser::GetDay(int year, int month)
{
	int day=0;
	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		{
			day=31;
			break;
		}
	case 4:
	case 6:
	case 9:
	case 11:
		{
			day=30;
			break;
		}
	case 2:
        {
			if(year%4==0&&year%100!=0||year%400==0)
				day=29;
			else
				day=28;
			break;
        }
	default :  {
		day=-1;
        break;
			   }
	}
    return day;
}

bool CUser::BirthdayIsRight(string cardId)
{
	string year,month,day;
    bool flag=false;
	int length=cardId.length();
	if(length==18){
		year=cardId.substr(6,4);//取18位身份证中的年
		month=cardId.substr(10,2); // 月
		day=cardId.substr(12,2);   //日
    }
    else
    {
		year=cardId.substr(6,2);//取15位身份证中的年
		month=cardId.substr(8,2); // 月
		day=cardId.substr(10,2);   //日
		year="19"+year;
    }
    if(atoi(year.c_str())==0||atoi(month.c_str())==0||atoi(day.c_str())==0)
	{
		flag=false;
	}
	else if(GetDay(atoi(year.c_str()),atoi(month.c_str()))>=atoi(day.c_str()))
    {
		if(IsEighteen(atoi(year.c_str())*10000 + atoi(month.c_str())*100 + atoi(day.c_str())))
			flag=true;
    }
    return flag;

}

bool CUser::IsRight(string card)
{
	bool error=true;
	int length=(int)card.length();
	int index=card.find_first_not_of("1234567890");
	if(index!=card.npos)
	{
		if (index != 17)
			error=false;  //号码中有非数字字符
	}
    if(!BirthdayIsRight(card))
	{
		error=false; //出生年月错误
	}
	if(length==18)     //验证18位身份证号码
	{
		int no[]={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
		char id[]={'1','0','X','9','8','7','6','5','4','3','2'};
		//1 0 X 9 8 7 6 5 4 3 2   校验码
		const char *p=card.c_str();
		int i=0,wi=0,sum=0;
		for(;i<length-1;i++)
		{
			wi=(*(p+i)-'0')*no[i];
			sum+=wi;
		}
		if(*(p+i)<'0'||*(p+i)>'9')
		{
			if (*(p+i)!='X'&&*(p+i)!='x')
			{
				error=false;//身份证最后一位输入错误
			}
		}
		wi=sum%11;
		if(*(p+17)=='x'||*(p+17)=='X')            //最后一位为'x'或'X';
		{
			if(id[wi]!='x'&&id[wi]!='X')
				error=false;
		}
        else if(id[wi]!=*(p+17))     //判断计算出的身份证校验码与输入是否相符
		{
			error=false;
		}
	}
 return error;
}

bool CUser::IsEighteen(int Birthday)
{
	SYSTEMTIME ct;
	unsigned long date;
	GetLocalTime(&ct);
	date = ct.wYear*10000 + ct.wMonth*100 + ct.wDay;
	if ((date - Birthday) > 180000)
	{
		return true;
	}
	else
	{
		return false;
	}
}

UINT CUser::PVPEmendDamage(UINT nLoseLife, OBJID AttId, bool bOverPVP /*= true*/)//决斗修正伤害
{
	UINT nRealLoseLife =0;
	DEBUG_TRY
	if (!IsPVPStuts())
		return nLoseLife;
	nRealLoseLife = nLoseLife;
	UINT nOldLife = GetLife();
	if(nOldLife > nLoseLife)	//还没打死
		return nRealLoseLife;
	
	IRole* pRoleTemp = RoleManager()->QueryRole(AttId);
	if (!pRoleTemp)
	{
		::LogSave("攻击者缺失1, BeAttUserid = %u, AttUserid = %u", GetID(), AttId);
		return 0;
	}
	if(pRoleTemp->IsUser())				//人吃人
	{
		if(GetPVPTargetID() == AttId)
		{
			CUser* pAtt = NULL;
			if(pRoleTemp->QueryObj(OBJ_USER,IPP_OF(pAtt)))
			if (!pAtt)
			{
				::LogSave("攻击者缺失2(攻击者是玩家), BeAttUserid = %u, AttUserid = %u", GetID(), AttId);
				return 0;
			}
			if (bOverPVP)
			{
				if(MapGroup(PID)->GetPVPManger()->findPVP(GetID()))
					MapGroup(PID)->GetPVPManger()->EndPVP(GetID(),AttId, true);
			}
			//			this->SetAttrib(_USERATTRIB_LIFE,1,true);
			pAtt->DetachBadlyStatus(pAtt);
			this->DetachBadlyStatus(this);
			CRole::AttachStatus(QueryRole(), STATUS_PK_PROTECT, 0, 3);
			nRealLoseLife = nOldLife - 1;
		}
		
	}
	else	//宠物打人
	{
		CMonster* pAtt = NULL;
		if(pRoleTemp->QueryObj(OBJ_MONSTER,IPP_OF(pAtt)))
		if (!pAtt)
		{
			::LogSave("攻击者缺失3(攻击者是宠物), BeAttUserid = %u, AttUserid = %u", GetID(), AttId);
			return 0;
		}
		if (!pAtt->IsEudemon())	//如果不是宠物
		{
			return nRealLoseLife;
		}
		CUser* pEudOwner = pRoleTemp->QueryOwnerUser();
		if (!pEudOwner)
		{
			::LogSave("攻击时宠物没有主人？BeAttUserid = %u, AttEudid = %u", GetID(), AttId);
			return nRealLoseLife;
		}
		if (GetPVPTargetID() == (pEudOwner->GetID()))
		{
			if (bOverPVP)
			{
				if(MapGroup(PID)->GetPVPManger()->findPVP(GetID()))
					MapGroup(PID)->GetPVPManger()->EndPVP(GetID(), pEudOwner->GetID(), true);
			}
		}
		pEudOwner->DetachBadlyStatus(pEudOwner);
		this->DetachBadlyStatus(this);
		CRole::AttachStatus(QueryRole(), STATUS_PK_PROTECT, 0, 3);
		nRealLoseLife = nOldLife - 1;
	}
	DEBUG_CATCH3("PVPEmendDamage Error BeAttUserid = %u, AttUserid = %u", GetID(), AttId)
	return nRealLoseLife;
}

void CUser::SendAllTreasure()
{
	if (!m_pTreasure)
		return;
	m_pTreasure->SendInfoToClient();
}

double CUser::GetPassedTimeFromCreate(LPCTSTR szTime)
{
	CHECKF(szTime);
	CHECKF(strlen(szTime));

	double nPassTime = 0;
	DEBUG_TRY
	COleDateTime compDate;
	if (!compDate.ParseDateTime(szTime))
	{
		::LogSave("CUser::GetPassedTimeFromCreate 参数错误(%s)", szTime);
		return 0;
	}

	DWORD nCreateDate = m_data.GetCreateTime();
	if (nCreateDate == 0)
	{
		::LogSave("CUser::GetPassedTimeFromCreate 角色创建日期为0,userid=%u", GetID());
		return 0;
	}
	int nYear = 2000 + nCreateDate/100000000;
	int nMonth = (nCreateDate/1000000) % 100;
	int nDay = (nCreateDate/10000) % 100;
	int nHour = (nCreateDate/100) % 100;
	int nMin = nCreateDate % 100;

	char szCreateTime[32]="";
	sprintf(szCreateTime, "%d-%d-%d %d:%d:00", nYear, nMonth, nDay, nHour, nMin);
	COleDateTime createDate;
	if (!createDate.ParseDateTime(szCreateTime))
	{
		::LogSave("CUser::GetPassedTimeFromCreate 创建日期格式错误,userid=%u,createDate=%u", GetID(), nCreateDate);
		return 0;
	}
	COleDateTimeSpan passDate = compDate - createDate;
	nPassTime = passDate.GetTotalMinutes();
	DEBUG_CATCH3("CUser::GetPassedTimeFromCreate error! user=%u,szTime=%s", GetID(), szTime);
	return nPassTime;
}

void CUser::AddMoneyByType(DWORD nMoney, int nType)
{
	if(nType != 1 && nType != 2)
	{
		if (IsGM())
		{
			SendSysMsg("脚本给了不允许给的金钱！");
		}
		return;
	}
	if (nMoney > 5000000)
	{
		::LogSave("AddMoneyByType接口给钱超过500W上限");
		if (IsGM())
		{
			SendSysMsg("AddMoneyByType接口给钱超过500W上限");
		}
		nMoney = 5000000;
	}
	DWORD nNewMoney = GetMoneyByType(nType)+nMoney;
	if(nType == 1)
		SetAttrib( _USERATTRIB_MONEY, nNewMoney, SYNCHRO_TRUE);
	else
		SetAttrib(_USERATTRIB_MONEY2, nNewMoney, SYNCHRO_TRUE);
}

void CUser::JudgeMonsterKill(int nAdd)
{
	bool bUpdate = false;
	DWORD oldKill = m_data.GetMonsterkill();
	m_data.SetMonsterkill((nAdd + oldKill), bUpdate);
	CMsgUserAttrib msg;
	msg.Create(GetID(),_USERATTRIB_UserInfo_MonsterKill, nAdd + oldKill);
	SendMsg(&msg);
	if(!ChkDayTaskMask2(11))
	{
		if ((nAdd + oldKill)>= 1000)
		{
			AddDayTaskMask2(11);
		}
	}	
}

void CUser::ResetMonsterKillAndEveryDayOL()
{
	m_data.SetMonsterkill(0);	//重置怪物计数
	m_data.SetEverydayOL(0);	//重置每日在线
}

int CUser::GetEverydayOL()
{
	return m_data.GetEverydayOL();
}

void CUser::ProcessEverydayOL()
{	
	if (ChkDayTaskMask2(12))
		return;
	int nOldTime = m_data.GetEverydayOL();
	
	m_data.SetEverydayOL((nOldTime + 1));//每分钟计一次
	if ((nOldTime + 1) >= 4*60)//4小时
	{
		AddDayTaskMask2(12);
	}
}

void CUser::SaveEudemonStatus()
{
	if (!m_pWarPet)
		return;
	if (!m_pWarPet->QueryStatusSet())
		return;
	m_setEudemonStatus.clear();
	StatusInfoStruct	info = {0};
	for(int i = 0; i < m_pWarPet->QueryStatusSet()->GetAmount(); i++)
	{
		IStatus* pStatus = m_pWarPet->QueryStatusSet()->GetObjByIndex(i);
		if(pStatus)
		{
			if(pStatus->GetInfo(&info))	
			{
				m_setEudemonStatus.push_back(info);
			}
		}
	}
}

BOOL CUser::CheckJoinSynDays(int nday)
{
	if(0 == GetSynID() || !QuerySynAttr())
	{
		return false;
	}
	else
	{
		return QuerySynAttr()->CheckJoinSyndays(nday);
	}
}

void CUser::RestoreEudemonStatus()
{
	if (!m_pWarPet)
		return;
	StatusInfoStruct	info = {0};
	for(int i = 0; i < m_setEudemonStatus.size(); i++)
	{
		info = m_setEudemonStatus[i];
		CRole::AttachStatus(m_pWarPet->QueryRole(), info.nStatus, info.nPower, info.nSecs, info.nTimes, info.nParam1, info.nParam2, info.nParam3);
	}
	m_setEudemonStatus.clear();
}

int CUser::IsPEIFANGNeedKidnap(OBJID ObjType, CPeifangTypeData*	pPeifangType)
{
	OBJID TypeID;
	switch(ObjType)
	{
	case	1:
		TypeID = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM1);
		break;
	case	2:
		TypeID = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM2);
		break;
	case	3:
		TypeID = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM3);
		break;
	case	4:
		TypeID = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM4);
		break;
	default:
		::LogSave("诡异的配方个数");
		return -1;
		break;
	}

	if(!TypeID)
		return 0;
	CItem* pItem = GetItemByType(TypeID);
	if (!pItem)
	{
		::LogSave("玩家发过来了一个身上没有的道具iduser=%u, typeid=%u", GetID(), TypeID);
			return -1;
	}
	return pItem->GetKidnap();
}

DWORD CUser::GetPetLife()
{
	if(!m_pWarPet)
		return 0;
	return m_pWarPet->GetLife();
}

DWORD CUser::GetPetMaxLife()
{
	if(!m_pWarPet)
		return 0;
	return m_pWarPet->GetMaxLife();
}

// DWORD CUser::GetconSyn()
// {
// 	return m_data.GetSynCon();
// }
// 
// void CUser::SetconSyn(int nData)
// {
//  	m_data.SetSynCon(nData);
// }


DWORD	CUser::GetRandPhyAtk(DWORD nOldAtk)
{
	if(m_WPSLRandAddPhyAtt.nChange <= 0)
		return 0;
	if(m_WPSLRandAddPhyAtt.nData <= 0)
		return 0;

	if(RandGet(10000) < m_WPSLRandAddPhyAtt.nChange)
		return m_WPSLRandAddPhyAtt.nData;

	return 0;
}

DWORD	CUser::GetRandMagAtk(DWORD nOldAtk)
{
	if(m_WPSLRandAddMagAtt.nChange <= 0)
		return 0;
	if(m_WPSLRandAddMagAtt.nData <= 0)
		return 0;
	
	if(RandGet(10000) < m_WPSLRandAddMagAtt.nChange)
		return m_WPSLRandAddMagAtt.nData;
	
	return 0;
}

DWORD	CUser::GetRandAtt1(DWORD nOldAtk)
{
	if(m_WPSLRandAddAtt1.nChange <= 0)
		return 0;
	if(m_WPSLRandAddAtt1.nData <= 0)
		return 0;
	
	if(RandGet(10000) < m_WPSLRandAddAtt1.nChange)
		return m_WPSLRandAddAtt1.nData;
	
	return 0;
}

DWORD	CUser::GetRandAtt2(DWORD nOldAtk)
{
	if(m_WPSLRandAddAtt2.nChange <= 0)
		return 0;
	if(m_WPSLRandAddAtt2.nData <= 0)
		return 0;
	
	if(RandGet(10000) < m_WPSLRandAddAtt2.nChange)
		return m_WPSLRandAddAtt2.nData;
	
	return 0;
}

DWORD	CUser::GetRandAtt3(DWORD nOldAtk)
{
	if(m_WPSLRandAddAtt3.nChange <= 0)
		return 0;
	if(m_WPSLRandAddAtt3.nData <= 0)
		return 0;
	
	if(RandGet(10000) < m_WPSLRandAddAtt3.nChange)
		return m_WPSLRandAddAtt3.nData;
	
	return 0;
}

DWORD	CUser::GetRandAtt4(DWORD nOldAtk)
{
	if(m_WPSLRandAddAtt4.nChange <= 0)
		return 0;
	if(m_WPSLRandAddAtt4.nData <= 0)
		return 0;
	
	if(RandGet(10000) < m_WPSLRandAddAtt4.nChange)
		return m_WPSLRandAddAtt4.nData;
	
	return 0;
}

DWORD	CUser::GetDecPhyDef(DWORD nOldDef)
{
	return min(nOldDef,m_WPSLDecPhyDef);
}

DWORD	CUser::GetDecMagDef(DWORD nOldDef)
{
	return min(nOldDef,m_WPSLDecMagDef);
}

DWORD	CUser::GetDecDef1(DWORD nOldDef)
{
	return min(nOldDef,m_WPSLDecDef1);
}

DWORD	CUser::GetDecDef2(DWORD nOldDef)
{
	return min(nOldDef,m_WPSLDecDef2);
}

DWORD	CUser::GetDecDef3(DWORD nOldDef)
{
	return min(nOldDef,m_WPSLDecDef3);
}

DWORD	CUser::GetDecDef4(DWORD nOldDef)
{
	return min(nOldDef,m_WPSLDecDef4);
}

CItem* CUser::CreateNewSoul(OBJID idItemType,ST_WEAPONSOULDATA wpslInfo,int nKindnap,OBJID idGem1,OBJID idGem2,OBJID idGem3)
{
	CHECKF(idItemType / 10000 == 25);

	ItemInfoStruct info;
	memset(&info,0,sizeof(ItemInfoStruct));
	CHECKF(ItemType()->GetInfo(idItemType, &info));
	info.idOwner	= GetID();
	info.idPlayer	= GetID();
	info.nKidnap	= nKindnap;
	info.dwPerPrice = 0;
	info.GemTypeID[0] = idGem1;
	info.GemTypeID[1] = idGem2;
	info.GemTypeID[2] = idGem3;


	//填入空值修改结束
	CItemPtr pItem = CItem::CreateNew();
	CHECKF(pItem);
	if(!pItem->CreateSoul(Database(), 
		GameDataDefault()->GetGameItemData(), 
		&info, 
		GameDataDefault()->GetWeaponSoulData(),
		wpslInfo,
		true, 
		info.id))
	{
		pItem->ReleaseByOwner();
					
		ASSERT(!"Create");
		this->SendSysMsg(g_objStrRes.GetStr(12049));//STR_FAILED_GENERATE_ITEM
		return NULL;
	}

	if (!AddItem(pItem, true))
	{
		LOGWARNING("获得物品失败，无法加入背包，userid = %u itemtype = %u",GetID(),pItem->GetTypeID());
		pItem->EraseWpSl(Database());
		pItem->DeleteRecord(Database());
		pItem->ReleaseByOwner();
		return NULL;
	}
	return pItem;
}

void CUser::SendJinmeiSysLogInOKInfo()
{
	if(m_pCompensationManager)
		m_pCompensationManager->UpdateAll();
	UserJinmeiSysMgr()->SendLoginOKInfo(this->GetID());
}

bool CUser::SpendJinmeiValue( int nData )
{
	if (nData <= 0)
	{
		return false;
	}

    int nValue = GetJinmeiValue();
	if (nData > nValue)
	{
		return false;
	}

	SetJinmeiValue(nValue - nData);

	CMsgUserAttrib msg_att;
	if (msg_att.Create(this->GetID(), _USERATTRIB_UserInfo_JimeiValue, nValue - nData))
	{
		this->SendMsg(&msg_att);
	}

	return true;
}

bool CUser::IncJinmeiValue( int nData )
{
	if (nData <= 0)
	{
		return false;
	}

    const int MAX_JINMEI_VALUE = 1000000;
	int nTmpData = GetJinmeiValue();
    if (nTmpData + nData > MAX_JINMEI_VALUE)
	{
		nTmpData = MAX_JINMEI_VALUE;
	}
	else
	{
		nTmpData += nData;
	}

	SetJinmeiValue(nTmpData);

	CMsgUserAttrib msg_att;
	if (msg_att.Create(this->GetID(), _USERATTRIB_UserInfo_JimeiValue, nTmpData))
	{
		this->SendMsg(&msg_att);
	}

	SendSysMsg(_TXTATR_TOPSHOW, "你获得了%u点真元", nData);
	return true;
}

int CUser::GetJinmeiIncDamageRate()
{
	return UserJinmeiSysMgr()->GetJinmeiIncDamageRate(this->GetID());
}

int CUser::GetJinmeiDecDamageRate()
{
	return UserJinmeiSysMgr()->GetJinmeiDecDamageRate(this->GetID());
}

//////////////////////////上线祝福相关////////////////////////////////////////////
void CUser::InitOnlineGift(OBJID Gift1ID,OBJID Gift2ID,OBJID Gift3ID,OBJID Gift4ID,OBJID Gift5ID)//脚本接口，获得物品id
{
	MSG_GIFT cGift;
	if(ChkZero(Gift1ID) || ChkZero(Gift2ID) || ChkZero(Gift3ID))
	{
		CMsgAction msg;
		cGift.Gift1 = 0;
		cGift.Gift2 = 0;
		cGift.Gift3 = 0;
		cGift.Gift4 = 0;
		cGift.usAction=actionGift;
		cGift.DayAmount=0;
		cGift.Num=0;
		if (msg.Create(cGift))
		{
			SendMsg(&msg);
		}
			return;
	}
	OBJID nDate = TimeGet(TIME_MINUTE);
	CHECK(m_pOnlineGift);
	if (!m_pOnlineGift->HasRecord())
	{
		m_pOnlineGift->AddGifts();
	}
	if (!IsVipActive() || GetVipLev() != 3)
	{
		Gift4ID=0;
	}
	if (IsDayContune())
	{
		m_pOnlineGift->AddDayAmount();
	}
	else
	{
		m_pOnlineGift->SetDayAmount(1);
	}
	if (m_pOnlineGift->GetDayAmount() > 10)
	{
		m_pOnlineGift->SetDayAmount(1);
	}
	m_pOnlineGift->SetGift(Gift1ID,Gift2ID,Gift3ID,Gift4ID,Gift5ID,0);
	m_pOnlineGift->SetDate(nDate);
	CMsgAction msg;
	cGift.Gift1 = Gift1ID;
	cGift.Gift2 = Gift2ID;
	cGift.Gift3 = Gift3ID;
	cGift.Gift4 = Gift4ID;
	cGift.usAction=actionGift;
	cGift.DayAmount=(USHORT)0;
	cGift.Num=(USHORT)0;
	if (!msg.Create(cGift))
	{
		::LogSave("ERROR!!! bool CUser::InitOnlineGift() SendMsg");
		return;
	}
	else			
	{
		SendMsg(&msg);		
		m_pOnlineGift->SetTaskMask(1);
	}
}

void CUser::ProcessOnlineGift()//判断
{
	OBJID nDate = TimeGet(TIME_MINUTE);
	CHECK(m_pOnlineGift);
	if (m_pOnlineGift->GetDate() <= nDate)
	{
		if (m_pOnlineGift->GetDate() == nDate && m_pOnlineGift->GetTaskMask() == 2)
		{
	    	//SendSysMsg("今天已领取祝福！");
			return;
		}
		if (m_pOnlineGift->GetTaskMask() == 1)
		{
		//	SendSysMsg("今天已抽取祝福！");
			return;
		}
		if(m_pOnlineGift->GetTaskMask() == 0)
		{
			ProcessAction(500400);
		}
	}
}
bool CUser::GiveOnlineGift()//发放礼物
{
	CHECKF(m_pOnlineGift);
	MSG_GIFT cGift;
	if (m_pOnlineGift->GetTaskMask() == 1)
	{
		int rounds=3;
		CMsgAction msg;
		OBJID ObjID[4]={0};
		CItem* pitem;
		int		DayAmount = m_pOnlineGift->GetDayAmount();
		int nGift=1;
		if(IsVipActive() && GetVipLev() == 3)
		{
			rounds=4;
		}
		
		if(DayAmount == 5)
		{
			nGift=2;
		}
		else if(DayAmount == 10)
		{
			nGift=3;
		}
		if(!ChkItem(nGift))
		{
				SendSysMsg("背包空间不足！");
				cGift.Gift1 = 0;
				cGift.Gift2 = 0;
				cGift.Gift3 = 0;
				cGift.Gift4 = 0;
				cGift.usAction=actionAcceptGift;
				cGift.DayAmount=0;
				cGift.Num=0;
				if (msg.Create(cGift))
					SendMsg(&msg);
				else
					::LogSave("ERROR!!! bool CUser::GiveOnlineGift() SendMsg");
				return false;
		}
		ItemInfoStruct info;
		if (ItemType()->GetInfo(m_pOnlineGift->GetGiftID(GIFTDATA_GIFT5ID), &info))
		{
			pitem=AwardItem(m_pOnlineGift->GetGiftID(GIFTDATA_GIFT5ID), true, false, true, false, nGift,false, true, false, 1);//策划要求每个参与活动的人都送这份礼物
			CHECKF(pitem);
		}
		
		for (int i=0;i<rounds;i++)
		{
			ObjID[i]=m_pOnlineGift->GetGiftID(GIFTDATA_GIFT1ID+i);
			int nType = ObjID[i]/1000000;//得到类型 1是经验，2是物品
			int nData = ObjID[i]%1000000;//得到id或者是经验
			switch(nType)
			{
			case 1:
				{
					AwardBattleExp(nData,false,false);
					SendSysMsg("你获得了%d经验",nData);
				}
				break;
			case 2:
				{
					if (ItemType()->GetInfo(nData, &info))
					{
						pitem=AwardItem(nData, true, false, true, false, 1, false, true, false, 1);
						if (!pitem)
						{
							LogSave("CUser::GiveOnlineGift() AwardItem()  nData=%d error!!!! ",nData);
							return false;
						}
						IgotAnOrangeWeapen(ObjID[i],pitem);	
					}
				}
				break;
			default:
				LOGWARNING("ERROR!!! bool CUser::GiveOnlineGift()");
			}
		}
		cGift.Gift1 = ObjID[0];
		cGift.Gift2 = ObjID[1];
		cGift.Gift3 = ObjID[2];
		cGift.Gift4 = ObjID[3];
		cGift.usAction=actionAcceptGift;
		cGift.DayAmount=(USHORT)m_pOnlineGift->GetDayAmount();
		cGift.Num=(USHORT)nGift;
		if(msg.Create(cGift))
		{
			SendMsg(&msg);
			m_pOnlineGift->SetTaskMask(2);
			return true;
		}
		::LogSave("ERROR!!! bool CUser::GiveOnlineGift() SendMsg");
	}
	return false;
}

bool CUser::ChkItem(int nMustGive)//看看有多少件物品
{
	int nPak1=0, nPak2=0,nPak3=0;//,nPakPet=0;
	for (int i=0;i<4;i++)
	{
		OBJID gift = m_pOnlineGift->GetGiftID(GIFTDATA_GIFT1ID+i);
		if (!gift)
			continue;
		OBJID objid=gift%1000000;
		int nType = gift/1000000;//得到类型 1是经验，2是物品
		if (nType == 1)
		{
			continue;
		}
		int nPosition = QueryPackage()->GetAvailablePosition(objid);
		if (nPosition==ITEMPOSITION_BACKPACK1)
				nPak1++;
		else if (nPosition==ITEMPOSITION_BACKPACK2)
				nPak2++;
		else if (nPosition==ITEMPOSITION_BACKPACK3)
				nPak3++;
		else
		{
			return false;
		}
	}
	
		int nPosition = QueryPackage()->GetAvailablePosition(m_pOnlineGift->GetGiftID(GIFTDATA_GIFT5ID));
		if (nPosition==ITEMPOSITION_BACKPACK1)
			nPak1+=nMustGive;
		else if (nPosition==ITEMPOSITION_BACKPACK2)
			nPak2+=nMustGive;
		else if (nPosition==ITEMPOSITION_BACKPACK3)
			nPak3+=nMustGive;
		else
		{
			return false;
		}
	if ( (nPak1>0 && QueryPackage()->IsPackFull(nPak1, 0, ITEMPOSITION_BACKPACK1))
		|| (nPak2>0 && QueryPackage()->IsPackFull(nPak2, 0, ITEMPOSITION_BACKPACK2))
		|| (nPak3>0 && QueryPackage()->IsPackFull(nPak3, 0, ITEMPOSITION_BACKPACK3)))
	{
		return false;
	}

	return true;
}
bool CUser::ChkZero(int nIn)
{
	if (nIn == 0)
	{
		if(IsGM())
		{
			SendSysMsg("IninOnlineGift 脚本给了一个不允许的类型值！");
		}
		return true;
	}
	return false; 
}
bool CUser::IsGive(int nTask)//脚本接口，
{

	OBJID nDate = TimeGet(TIME_MINUTE);
	CHECKF(m_pOnlineGift);
	if (!m_pOnlineGift->HasRecord())
	{
		m_pOnlineGift->AddGifts();
	}
	if(m_pOnlineGift->GetTaskMask() == nTask)
	{
		return true;
	}
	return false;
}

bool CUser::IsDayContune()
{
	int nLastGetDate=m_pOnlineGift->GetDate();
	if (nLastGetDate == 0)
	{
		return false;
	}
	int nYear = 2000 + nLastGetDate/100000000;
	int nMonth = (nLastGetDate/1000000) % 100;
	int nDay = (nLastGetDate/10000) % 100;
	int nHour = (nLastGetDate/100) % 100;
	int nMin = nLastGetDate % 100;
	DEBUG_TRY
	char szCreateTime[32]="";
	sprintf(szCreateTime, "%d-%d-%d 00:00:00", nYear, nMonth, nDay);

	COleDateTime createDate;
	COleDateTime NowDate(COleDateTime::GetCurrentTime());
	if (createDate.ParseDateTime(szCreateTime))
	{
		if(NowDate.GetHour() < 5)
		{
			COleDateTimeSpan passDate(1,0,0,0);
			NowDate -= passDate;
		}

		COleDateTime NowCount;
		sprintf(szCreateTime, "%d-%d-%d 00:00:00", NowDate.GetYear(), NowDate.GetMonth(), NowDate.GetDay());
		if (NowCount.ParseDateTime(szCreateTime))
		{
			COleDateTimeSpan realPass = NowCount - createDate;
			if(realPass.GetTotalDays() > 1)
			{
				//SendSysMsg("大于1天");
				return false;
			}
			else
			{
				//SendSysMsg("小于1天");
				return true;
			}
		}
		else
		{
			::LogSave("CUser::IsDayContune() NowCount.ParseDateTime(szCreateTime) error");
			return false;
		}		
	}
	else
	{
		::LogSave("CUser::IsDayContune() createDate.ParseDateTime(szCreateTime) error");
		return false;
	}
	DEBUG_CATCH("CUser::IsDayContune() error");
	return false;
}
//查询登记的师父列表
void CUser::QueryTutorList( USHORT nPage, int nPageSize )
{
	CHECK(nPageSize);
	TUTORLIST arrData[20] = {0};
	int nDataSize = g_TutorListSystem.GetInfo( arrData,nPageSize,nPage );
	if( -1 == nDataSize )
	{
		SendSysMsg("列表更新中");
		return;
	}

	int nTotalAmount = g_TutorListSystem.GetTutorAmount();
	UCHAR nPages = nTotalAmount%nPageSize==0 ? nTotalAmount/nPageSize : (nTotalAmount/nPageSize) + 1;
	CMsgSchoolMember msg;
	if (msg.Create(GetID(), MESSAGEBOARD_QUERYTOTURLIST, NULL, 0, nPages))
	{
		for (int i=0; i<nDataSize; i++)
		{
			msg.Append(arrData[i]);
		}
	}
	SendMsg(&msg);
}
//玩家加入师父列表
void CUser::JoinToTutorList()
{
	TUTORLIST info = {0};
	info.idUser = GetID();
	info.ucGender = GetSex();
	if(IsVipActive())
		info.ucVipLev = GetVipLev();
	else
		info.ucVipLev = 0;
	info.usLev = GetLev();
	info.dwProfession = GetProfession(1);
	info.dwBattleNum = m_data.GetMaxScore();
	info.dwImpart = GetTutorLevel();
	info.dwLastLogoutTime = 0;
//	info.dwMorality = GetMorality();
	SafeCopy(info.szName, GetName(), _MAX_NAMESIZE);
	g_TutorListSystem.AddTutorToList(&info);

	AddTutorListMask();

	CMsgSchoolMember msgTutor;
	if (msgTutor.Create(GetIPAddress(), MESSAGEBOARD_SYNLINE, NULL, 0, SYNTUTOR_ADD))
	{
		msgTutor.Append(info);
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msgTutor, (OBJID)0);
	}
}
//从师父列表中删除
void CUser::DelFromTutorList()
{
	int nStep = 0;
	DEBUG_TRY
	g_TutorListSystem.DeleteTutorFromList(GetID());
	nStep = 1;
	DelTutorListMask();
	nStep = 2;
	TUTORLIST info = {0};
	info.idUser = GetID();
	info.ucGender = GetSex();
	if(IsVipActive())
		info.ucVipLev = GetVipLev();
	else
		info.ucVipLev = 0;
	info.usLev = GetLev();
	info.dwProfession = GetProfession(1);
	info.dwBattleNum = m_data.GetMaxScore();
	info.dwImpart = GetImpart();
	info.dwLastLogoutTime = 0;
	//	info.dwMorality = GetMorality();
	SafeCopy(info.szName, GetName(), _MAX_NAMESIZE);
	nStep = 3;
	CMsgSchoolMember msgTutor;
	if (msgTutor.Create(GetID(), MESSAGEBOARD_SYNLINE, NULL, 0, SYNTUTOR_DEL))
	{
		msgTutor.Append(info);
		nStep = 4;
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msgTutor, (OBJID)0);
	}
	DEBUG_CATCH2("CUser::DelFromTutorList() error!nStep=%d",nStep)
}
//帮派功能

bool CUser::ChangeLineMap(OBJID MapID,int nPosx,int nPosy,int nLine)//向指定的线路换地图--jay
{
	if (nLine < 0)
		return false;
	if ((nLine>=CHANGELINE_BEGIN && nLine<CHANGELINE_END)|| nLine==SPECILLINE)
	{	
		CHECKF(m_pSpecilLineInfo);
			return m_pSpecilLineInfo->ChangeLineMap(MapID, nPosx, nPosy,(CHANGELINE) nLine);
	}
	else
	{
		LogSave("ChangeLineMap(OBJID MapID,int nPosx,int nPosy,int nLine)传入错误的值: MapID=%u,nPosx=%u,nPosy=%u,nLine=%u",MapID,nPosx,nPosy,nLine);
	}
	return false;
}
bool CUser::LeaveSynMap()
{
	return ChangeLineMap(GetRecordMapID(),GetRecordPosX(),GetRecordPosY(),RANDLINE);
}
bool CUser::IsMySynMap()
{
	CHECKF(GetSynMap());
	return GetSynMap()->GetID()==GetMapID();
}
bool CUser::EnterSynMap(int nPosx,int nPosy)//进入帮派地图--jay
{
	CHECKF(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	CHECKF(psyn);
	if (g_SpecilLine==1)
		return FlyMap(psyn->GetSynMapID(),nPosx,nPosy);
	else
		return ChangeLineMap(psyn->GetID(),nPosx,nPosy,SPECILLINE);
}
bool CUser::LeaveSynInsten(OBJID idmap,OBJID nPosx,OBJID nPosy)
{
	if (idmap==1048)
	{
		idmap=GetSynMapID();
	}
	return FlyMap(idmap, nPosx, nPosy);
}

CGameMap *CUser::GetSynMap()
{
	CHECKF(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(psyn)
		return MapManager()->QueryMap(psyn->GetSynMapID());
	else
		return NULL;
}
void CUser::SetSynTaskGift(OBJID nGift1,OBJID nGift2)//放入奖励礼物--jay
{
	if (!GetSyndicate())
	return;
	if(nGift1 < 0 || nGift2 <0)
		return;
	if (nGift1 < 100000 || nGift1 > 999999)
		return;
	m_pSyn->SetGift1(nGift1);
	if (nGift2!=0)
	m_pSyn->SetGift2(nGift2);
	
	m_pSyn->SetMask(1);
}
int CUser::GetGiftMask(bool bGive)//判断奖励掩码
{
	if(bGive)
	{
		if (m_pSyn->GetMask() == 1)
		{
			int nPak1=0, nPak2=0,nPak3=0;
			OBJID gift = m_pSyn->GetGift1();
			if (!gift)
				return -1;//出错
			int nPosition = QueryPackage()->GetAvailablePosition(gift);
			if (nPosition==ITEMPOSITION_BACKPACK1)
				nPak1++;
			else if (nPosition==ITEMPOSITION_BACKPACK2)
				nPak2++;
			else if (nPosition==ITEMPOSITION_BACKPACK3)
				nPak3++;
			else
			{
				return -1;//包满
			}
			if ( (nPak1>0 && QueryPackage()->IsPackFull(nPak1, 0, ITEMPOSITION_BACKPACK1))
				|| (nPak2>0 && QueryPackage()->IsPackFull(nPak2, 0, ITEMPOSITION_BACKPACK2))
				|| (nPak3>0 && QueryPackage()->IsPackFull(nPak3, 0, ITEMPOSITION_BACKPACK3)))
			{
				return 1;//背包满
			}
			CItem* pitem;
			pitem=AwardItem(gift, true, false, true, false, 1,false, true, false, 1);
			CHECKF(pitem);
			m_pSyn->SetMask(0);
			return 0;
		}
		else
			return -1;
	}
	return m_pSyn->GetMask();
}
OBJID CUser::GetSynMapID()
{
	CHECKF(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return -1;
	return psyn->GetSynMapID();
}
int CUser::GetSynSci(int nSub)//1.青龙 2.白虎 4.朱雀 3.玄武
{
	CHECKF(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return -1;
	return psyn->GetLev(nSub);
}
int CUser::GetNowWeekDay()//--注意1为星期天,7为星期六
{
	CTime now(CTime::GetCurrentTime());
	return now.GetDayOfWeek();
} 
int CUser::GetHistorySynCon()
{
	CHECKF(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return 0;
	return QuerySynAttr()->GetHistoryCon();
}
int CUser::GetHistorySynMoney()
{
	CHECKF(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return 0;
	return QuerySynAttr()->GetHistoryMoney();
}
int CUser::GetHistorySynGrow()
{
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return 0;
	return QuerySynAttr()->GetHistoryGrow();
}
void CUser::SetHistorySynMoney(int nMoney)
{
	CHECK(nMoney>=0);
	CHECK(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return;
	QuerySynAttr()->SetHistoryMoney(nMoney);

}//历史总钱贡
void CUser::SetHistorySynGrow(int nGrow)
{
	CHECK(nGrow>=0);
	CHECK(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return;
	QuerySynAttr()->SetHistoryGrow(nGrow);
	
}//历史总建设

int CUser::GetMySynRank()//脚本接口-获取人物帮派职位
{
	CHECKF(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return 0;
	return QuerySynAttr()->GetSynRank();
}
int CUser::GetSynMoney()
{
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if (psyn)
		return psyn->GetMoney();
	else
		return 0;
}
void CUser::SetSynMoney(int nMoney)
{
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if (!psyn)
		return;
	if (nMoney<0)
	{
		LogSave("传入负数金钱!");
		return;
	}
	int newMoney=0;
	if (nMoney>psyn->GetMoney())
	{
		newMoney=nMoney-psyn->GetMoney();
		if (newMoney<0)
		{
			LogSave("负数金钱!");
			return;
		}
		SendSysMsg("获得帮派资金:%d金%d银%d铜",newMoney/10000, (newMoney-((newMoney/10000)*10000))/100,newMoney % 100 );
		SetHistorySynMoney(GetHistorySynMoney()+nMoney-psyn->GetMoney());
	}
	else
	{
		newMoney=psyn->GetMoney()-nMoney;
		if (newMoney<0)
		{
			LogSave("负数金钱!");
			return;
		}
		SendSysMsg("消耗帮派资金:%d金%d银%d铜",newMoney/10000, (newMoney-((newMoney/10000)*10000))/100,newMoney % 100);
	}
	psyn->SetGrow(SPENDMONEY,nMoney);

}
int CUser::GetSynGrow()
{
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if (psyn)
		return psyn->GetGrow();
	else
		return 0;
}
void CUser::SetSynGrow(int nData)
{
	CHECK(nData>=0);
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if (!psyn)
	return;
	if (nData>psyn->GetGrow())
	{
		SendSysMsg("获得%d帮派建设度",nData-psyn->GetGrow());
		SetHistorySynGrow(GetHistorySynGrow()+nData-psyn->GetGrow());
	}
	else
		SendSysMsg("消耗%d帮派建设度",psyn->GetGrow()-nData);
	psyn->SetGrow(SPENDBUILD,nData);		
}
CMonster* CUser::GetAttackingMonster()
{
	return (CMonster*)RoleManager()->QueryRole(GetAttackTargetID());
}
int CUser::GetMonsterType()
{
	CHECKF(GetAttackingMonster());
	return GetAttackingMonster()->GetType();
}
int CUser::GetMonsterPosx()
{
	CHECKF(GetAttackingMonster());
	return GetAttackingMonster()->GetPosX();
}
int CUser::GetMonsterPosy()
{
	CHECKF(GetAttackingMonster());
	return GetAttackingMonster()->GetPosY();
}
void CUser::BroadcastSyn(LPCTSTR pPost,int type)
{
	CMsgTalk	msg;
	char poststr[_MAX_STRING];
	switch(type)
	{
	case 0:
		{
			sprintf(poststr,"%s",pPost);	
		}
		break;
	case 1:
		{
			sprintf(poststr,"<%d_[%s]_7>%s",SNAPSHOTUSER,GetName(),pPost);
		}
		break;
	default:
			sprintf(poststr,"");
	}
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if (psyn)
	{
		if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , poststr, NULL, 0xff0000, _TXTATR_SYNDICATE))
			psyn->BroadcastSynMsg(&msg );
	}
	CMsgSyndicate msgSyndicate;
			msgSyndicate.Create(SYN_SYNTALKMSG ,0 ,GetName() ,psyn->GetID() ,poststr);
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,(OBJID)0);
}
bool CUser::ConMoney(DWORD dwMoney, bool bMoney, int EndFlag, bool bSynchro, OBJID idItem)
{
	if(dwMoney == 0)
		return true;
	
	if(dwMoney < 0)
	{
		LOGWARNING("GainMoney: dwMoney < 0");
                return false;
	}
	OBJID idSyn = this->GetSynID();
	CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);

	if (!pSyn)
	{
		return false;
	}
	if (bMoney)
	{
		OBJID OldMoney = pSyn->GetInt(SYNDATA_MONEY);
		OBJID NewMoney = OldMoney + dwMoney;
		if (NewMoney > (pSyn->GetDeadline(5)))
		{
			SendSysMsg("帮派金钱不能超过上限无法捐献！");
			if (EndFlag)
			{
				Contribute();
			}
			return false;
		}
		else	if (!SpendMoneyByType(dwMoney, 1, true))
		{
			this->SendSysMsg("你身上的金钱不足！");
			return false;
		}
	}	

	if (EndFlag == 0)
	{
		OBJID OldMoney = pSyn->GetInt(SYNDATA_MONEY);
		OBJID NewMoney = OldMoney + m_nConMoney + dwMoney;
		if (NewMoney > (pSyn->GetDeadline(5)))
		{
			SendSysMsg("帮派金钱不能超过上限无法捐献！");
			return false;
		}
		if (!bMoney)
		{
			if (!idItem)
				return false;
			CItem* pItem = GetItem( idItem );
			if (!pItem)
			{
				return false;
			}
            if(pItem->IsLocked())
				return false;

	        if(!pItem->IsSellEnable())
			  return false;
			if(!this->EraseItem(idItem, SYNCHRO_TRUE))
			{
				this->SendSysMsg("删除物品出错");
				return false;
			}
		}
		m_nConMoney += dwMoney;
		return true;
	}
	else	if(EndFlag == 1)
	{
		OBJID OldMoney = pSyn->GetInt(SYNDATA_MONEY);
		OBJID NewMoney = OldMoney + m_nConMoney + dwMoney;
        if (idItem)
		{
			 CItem* pItem = GetItem( idItem );
			 if (!pItem)
			 {
				 return false;
			 }
			 if(pItem->IsLocked())
				 return false;

			if(!pItem->IsSellEnable())
				return false;

		}
		if (NewMoney > (pSyn->GetDeadline(5)))
		{
			SendSysMsg("帮派金钱不能超过上限无法捐献！");
			Contribute();
			return false;
		}
        if(idItem && !this->EraseItem(idItem, SYNCHRO_TRUE))
		{
			this->SendSysMsg("删除物品出错");
			return false;
		}
		m_nConMoney += dwMoney;
		return Contribute();			
	}	
	else 
		return false;
}
bool CUser::CheckIsSameSyn(int nType)
{
	CTeam *pTeam=GetTeam();
	if (pTeam)
	{
		CSynPtr pSyn = SynManager()->QuerySyndicate(GetSynID());
		if (pSyn)
		{
			if(nType==0)
				return pTeam->CheckIsSameSyn(GetSynID());
			else if (nType==1)
			{
				CUserPtr pLeader=UserManager()->GetUser(pTeam->GetLeader());
				if (pLeader)
					return GetSynID()==pLeader->GetSynID();
			}
		}
	}
		return false;
}

int CUser::CulData(USHORT nType,int nData1,int nData2)
{
	switch(nType)
	{
	case 1:
		if(nData2 == 0)
			return 0;
		return nData1 / nData2;
		break;
	case 2:
		if(nData2 == 0)
			return 0;
		return nData1 % nData2;
		break;
	}
	return 0;
}

void CUser::SaveLog(int nType,OBJID nValue,bool isGet,int nGetWay)//开放给脚本,添加日志
{
	switch(nType)
	{
		case 4 ://珠宝日志
		{
			if (nValue>30)//高于30珠宝才打日志
			{
				switch(nGetWay)
				{
					case 1://帮派获得
					{
						if (isGet)//获得
							MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get  %u jewel from syndicate ",GetID(), GetName(), nValue);
						else
							MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %u jewel from syndicate ",GetID(), GetName(), nValue);
					}
					break;
				}
			}
		}
		break;
	}

}

bool CUser::ConItem(OBJID idItem, int EndFlag)
{	
	CItem* pItem = GetItem( idItem );
	if(!pItem)
	{
		this->SendSysMsg("找不到物品");
		return false;
	}
	
	OBJID idSyn = GetSynID();
	CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
	
	if (!pSyn)
	{
		return false;
	}
	if(pItem->IsLocked())
		return false;

	if(!pItem->IsSellEnable())
		return false;

	if(pItem->GetWpSlLev() >= 3)
		return false;

	DEBUG_TRY	// VVVVVVVVVVVVVV

	BOOL bIsCostlyItem = pItem->IsCostlyItem();
	int nItemTypeID = 0, nItemID = 0, nItemAmount = 0;
//	ItemInfoStruct info;
	//NAMESTR szItemName;
	BOOL bIsItemCountable = FALSE;

	DWORD	dwMoney = pItem->GetSellPrice();
	DWORD nItemType = pItem->GetTypeID();

	if(nItemType==_SPCITEM_VASEXCHANGE || nItemType == 660000)//元宝票不能捐 //魔虫毒丝也是
		return false;

	char valuablelog[_MAX_STRING];
	if (bIsCostlyItem)
	{
		int nAmount = pItem->IsCountable(nItemType) ? pItem->GetItemAmount() : 1;
		sprintf(valuablelog,"%u(%s) lose %d %u from syndicate contribute", GetID(), GetName(), nAmount, pItem->GetTypeID());
	}
	BOOL bRet = FALSE;
	if(nItemType / 10000 != 66)
	{
		if (dwMoney*2 + m_nConMoney > pSyn->GetDeadline(5))
			bRet = this->ConMoney(dwMoney*2, false, 1, SYNCHRO_TRUE, idItem);
		else
			bRet = this->ConMoney(dwMoney*2, false, EndFlag, SYNCHRO_TRUE, idItem);
	}
	else
	{
		if(dwMoney/10 +m_nConBuild  > pSyn->GetDeadline(1))
			bRet = this->ConBuild(dwMoney/10, 1, idItem);
		else
			bRet = this->ConBuild(dwMoney/10, EndFlag, idItem);
	}
	if(bIsCostlyItem && bRet)
		::MyLogSave(VALUABLESLOG_FILENAME, valuablelog);
	DEBUG_CATCH("ConItem") // AAAAAAAAAAAAAAAAA
	return true;
}

BOOL CUser::ConBuild(OBJID dwBuild, int EndFlag, OBJID idItem)
{
	DEBUG_TRY
	if(dwBuild == 0)
		return true;
	
	if(dwBuild < 0)
	{
		LOGWARNING("CONBUILD: dwBuild < 0");
		return false;
	}
	OBJID idSyn = this->GetSynID();
	CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
	
	if (!pSyn)
	{
		return false;
	}
	
	if (EndFlag == 0)
	{
		OBJID OldBuild = pSyn->GetInt(SYNDATA_GROW);
		OBJID NewBuild = OldBuild + dwBuild+ m_nConBuild;
		if (NewBuild > pSyn->GetDeadline(1))
		{
			Contribute();
			return false;
		}
		if(!this->EraseItem(idItem, SYNCHRO_TRUE))
		{
			this->SendSysMsg("删除物品出错");
			return false;
		}
                m_nConBuild += dwBuild;
		return true;
	}
	else	if(EndFlag == 1)
	{
		OBJID OldBuild = pSyn->GetInt(SYNDATA_GROW);
		OBJID NewBuild = OldBuild + dwBuild + m_nConBuild;
		if (NewBuild > pSyn->GetDeadline(1))
		{
			this->SendSysMsg("帮派建设度已经达到上限无法捐献！");
			Contribute();
			return false;
		}
		if(!this->EraseItem(idItem, SYNCHRO_TRUE))
		{
			this->SendSysMsg("删除物品出错");
			return false;
		}
                m_nConBuild += dwBuild;
		return Contribute();
	}
	else
		return false;
	DEBUG_CATCH("CUser::ConBuild(OBJID dwBuild, int EndFlag, OBJID idItem)");
	return false;
}

BOOL CUser::Contribute()
{
	bool bResult = true;
	int  nType=0;//1是建设2是资金
    DEBUG_TRY 
		OBJID idSyn = this->GetSynID();
	CSynPtr pSyn = SynManager()->QuerySyndicate(idSyn);
	if (!pSyn)
		return FALSE;
	int OldBuild = pSyn->GetInt(SYNDATA_GROW);
	int NewBuild = OldBuild + m_nConBuild;
	if (NewBuild > pSyn->GetDeadline(1) && m_nConBuild != 0)
	{
		nType=1;
		m_nConMoney = 0;
		m_nConBuild = 0;
		bResult = false;
	}
	if (NewBuild < 0)
	{
		::LogSave("负数的建设度");
		NewBuild = 0;
	}
	
	
	
	int OldMoney = pSyn->GetInt(SYNDATA_MONEY);
	int NewMoney = OldMoney + m_nConMoney;
	if (NewMoney > (pSyn->GetDeadline(5)) && m_nConMoney != 0)
	{
		nType=2;
		m_nConMoney = 0;
		m_nConBuild = 0;
		bResult = false;
	}
	if (NewMoney < 0)
	{
		::LogSave("负数的金钱");
	}
	if (bResult)
	{
		if(m_nConMoney)
		{
			pSyn->SetGrow(SPENDMONEY, NewMoney);
			SetHistorySynMoney(GetHistorySynMoney()+m_nConMoney);
		}
		if (m_nConBuild)
		{
			pSyn->SetGrow(SPENDBUILD, NewBuild);
			SetHistorySynGrow(GetHistorySynGrow()+m_nConBuild);
		}
	}
	
	int nRate = 0;
	nRate = m_nConBuild / 1000 + m_nConMoney / 1000;
	CMsgTalk	msg;
	IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, this->GetName(), NULL, GetID(), _TXTATR_SYNAWARDMONEY, 0, m_nConBuild, m_nConMoney, NULL, nRate))
	{
		pSyn->BroadcastSynMsg(&msg);
		if (m_nConBuild == 0 && m_nConMoney == 0)
		{
			if (nType==1)
				SendSysMsg("帮派建设度不能超过上限！");
			if (nType==2)
				SendSysMsg("帮派金钱不能超过上限！");
			SendSysMsg("捐献失败！");
		}
		else
			SendSysMsg("捐献成功！");
	}
	DEBUG_CATCH2("CUser::Contribute(), iduser = %u", GetID())
	m_nConMoney = 0;
	m_nConBuild = 0;
	return true;
}
//////////////////////////////////////////////////////////////////////////
void CUser::ClearUsingInstance()
{
	InstanceUseMgr()->ClearUsing(GetID());
}

int CUser::GetInstanceTime(int nInstanceType)
{
	int nTime = InstanceUseMgr()->CheckInstanceUse(GetID(), nInstanceType);
	if(nTime == -1)
	{
		return 0;
	}

	return nTime;
}

int CUser::GetInstanceFinishTime(int nInstanceType)
{
	return InstanceUseMgr()->GetFinishInstanceTimes(GetID(), nInstanceType);
}


void CUser::PickupNewCard3(char *szCard)
{
	if(m_tCardMsgTime.IsActive())
	{
		if(!m_tCardMsgTime.ToNextTime())
		{
			SendSysMsg("领取太快，请过会再来领取");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	第一次检查
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);

	if(ChkNewGiveMask(35))
	{
		SendSysMsg("你已经领取过新手卡，不能重复领取");
		return;
	}



	if(strcmp(szCard,"70FF6E5E7AE772B6") != 0)
	{
		SendSysMsg("错误的卡号");
		return;
	}
	else
	{
		DEBUG_TRY
		if(ChkNewGiveMask(35))
			return;

		if(ProcessAction(2035))
		{
			AddNewGiveMask(35);
			SendSysMsg("领取成功");
		}
		else
		{
			SendSysMsg("领取失败");
		}
		DEBUG_CATCH("pUser->ProcessAction(2035)");
	}
}
bool CUser::WarGameSignup(OBJID idWarGame)
{
	CHECKF(idWarGame);
	CHECKF(m_pUserWarGame);

	if(m_pUserWarGame->HasArenaMask())//已经去过竞技场了
	{
		SendSysMsg(_TXTATR_TOPSHOW, "今天已经去过竞技场了");
		return false;
	}
	if(!WargameManager()->CanSignup(idWarGame))//未到报名时间
	{
		SendSysMsg(_TXTATR_TOPSHOW, "未到报名时间");
		return false;
	}

	if(WargameManager()->IsHasSignup(idWarGame, GetID()))//已经报过名了
	{
		SendSysMsg(_TXTATR_TOPSHOW, "您已经报过名了");
		return false;
	}

	if(WargameManager()->IsMaxSigninPlayer(idWarGame))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "报名人数已满");
		return false;
	}
	if(WargameManager()->Signup(idWarGame, this))
		SendSysMsg(_TXTATR_TOPSHOW, "报名成功");
	else
		return false;

	return true;
}

bool CUser::EnterWarGame(OBJID idWarGame)
{
	CHECKF(idWarGame);
	CHECKF(m_pUserWarGame);

	bool bRet = false;
	int nStep = 0;
	DEBUG_TRY
	//已经在竞技场内
	if(QueryStatus(STATUS_WARGAME))
		return false;
	nStep = 1;
	if(!WargameManager()->CanUserEnter(idWarGame))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "已经过了进场时间");
		return false;
	}
	nStep = 2;
	if(g_SpecilLine != 1)
	{
		if(m_pUserWarGame->HasArenaMask())//已经去过竞技场了
		{
			SendSysMsg(_TXTATR_TOPSHOW, "你今天已经去过竞技场了");
			return false;
		}
		nStep = 3;
		ChangeLineMap(20000 + idWarGame, 0, 0, SPECILLINE);
		return true;
	}
	nStep = 4;
	if(WargameManager()->IsMaxPlayer(idWarGame))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "进入人数已满");
		return false;
	}
	nStep = 5;
	//退出队伍，以免进场后队友是不同阵营
	if(CanBeSee() && GetTeam())
		QuitTeam();
	nStep = 6;
	bRet = WargameManager()->EnterWar(idWarGame, this);
	DEBUG_CATCH2("CUser::EnterWarGame nStep=%d", nStep)
	return bRet;
}

bool CUser::EnterWarGameFromNpc(OBJID idWarGame)
{
	if(!WargameManager()->IsHasSignup(idWarGame, GetID()))//已经报过名了
	{
		SendSysMsg(_TXTATR_TOPSHOW, "您还没有报名");
		return false;
	}

	return EnterWarGame(idWarGame);
}

bool CUser::LeaveWarGame()
{
	if(g_SpecilLine != 1)
		return false;

	IStatus* pStatus = QueryStatus(STATUS_WARGAME);
	if(!pStatus)
		return false;

	return WargameManager()->LeaveWar(pStatus->GetPower(), this);
}

bool CUser::EudemonSoul(OBJID idEudemon)
{
	CHECKF(idEudemon);

	if(!IsAlive())
		return false;

	if(GetLev() < 18)
	{
		SendSysMsg("您的等级未满18级，还不能让宠物附体");
		return false;
	}
	if(m_idSoulEudemon != ID_NONE)
	{
		SendSysMsg("已经有一个宠物附体了");
		return false;
	}
	if(GetCallPetID() == idEudemon)
	{
		SendSysMsg("出战的宠物不能附体");
		return false;
	}

	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
		return false;
	
	if(pData->isLocked())
		return false;

	int nLifespan = pData->GetInt(EUDEMONDATA_lifespan);
	if(nLifespan<=0)
	{
		SendSysMsg("寿命为0的宠物不能附体");
		return false;
	}

	pData->SetLock(true);
	pData->SetInt(EUDEMONDATA_state,PETSTATE_ADDIN,false);//设置为附体状态
	m_idSoulEudemon = idEudemon;
	m_tLastAddHp = 0;
	m_tLastAddMp = 0;

	if(!QueryStatus(STATUS_EUDEMON_SOUL))
		CRole::AttachStatus(this->QueryRole(), STATUS_EUDEMON_SOUL,10,-1);

	CMsgEudemonAttrib msg;
	if(msg.Create(idEudemon, EUDATTR_STATE, PETSTATE_ADDIN))
		SendMsg(&msg);
	UpdateCalculatedProperty(UPDATECALTYPE_EUDSOUL);
	m_lastSoulEudemonLifeSpan = clock();
	//显示特效
	if(m_data.GetHoldTitle() == 1 && pData->GetInt(EUDEMONDATA_fusion) == 10)
	{
		m_bSoulEudemonView = true;
		CMsgUserAttrib msgAtt;
		if(msgAtt.Create(GetID(), _USERATTRIB_EUDEMON_VIEW, m_bSoulEudemonView))
			BroadcastRoomMsg(&msgAtt, INCLUDE_SELF);
	}

	return true;
}

bool CUser::RemoveEudemonSoul()
{
	if(m_idSoulEudemon == ID_NONE)
		return false;

	CUserPetData* pData = m_pUserPet->QueryData(m_idSoulEudemon);
	if(!pData)
		return false;

	pData->SetLock(false);
	pData->SetInt(EUDEMONDATA_state,PETSTATE_REST,false);

	CRole::DetachStatus(this->QueryRole(), STATUS_EUDEMON_SOUL);
	m_idSoulEudemon = ID_NONE;
	CMsgEudemonAttrib msg;
	if(msg.Create(pData->GetID(), EUDATTR_STATE, PETSTATE_DELIN))
		SendMsg(&msg);
	UpdateCalculatedProperty(UPDATECALTYPE_EUDSOUL);	
	
	//去掉特效
	if(m_bSoulEudemonView)
	{
		m_bSoulEudemonView = false;
		CMsgUserAttrib msgAtt;
		if(msgAtt.Create(GetID(), _USERATTRIB_EUDEMON_VIEW, m_bSoulEudemonView))
			BroadcastRoomMsg(&msgAtt, INCLUDE_SELF);
	}
	return true;
}
//更新宠物附体后获得的属性
bool CUser::UpdateEudemonSoulProperty()
{
	m_eudemonSoulphyatt = 0;//外攻
	m_eudemonSoulmagatt = 0;//内攻
	m_eudemonSoul1 = 0;//火
	m_eudemonSoul2 = 0;//冰
	m_eudemonSoul3 = 0;//玄
	m_eudemonSoul4 = 0;//毒
	m_eudemonSoulAddHp = 0;//回血
	m_eudemonSoulAddMp = 0;//回蓝
	m_eudemonSoulAddDodge = 0;//加闪避buff
	m_eudemonSoulAddHitRate = 0;//加命中buff

	if(m_idSoulEudemon == ID_NONE)//分离
	{
		return true;
	}

	double nphyatt = 0;//外攻
	double nmagatt = 0;//内攻
	double nSoul1 = 0;//火
	double nSoul2 = 0;//冰
	double nSoul3 = 0;//玄
	double nSoul4 = 0;//毒

	EudemonData petData = {0};
	LoadEudemon(m_idSoulEudemon,petData,3);
	int nQuality = __max(petData.dwStrChg, petData.dwIntChg);
	float qualityParam = CMonster::GetQualityParam(nQuality);
	float fusionParam = CMonster::GetFusionParam(petData.fusion);
	int nLifeParam = petData.dwMaxLife;

	CEudemonMagicData* pThisMagicData = NULL;
	CMagicTypeData * pTypeData = NULL;
	for(int i = 0 ; i < 10 ; i ++)
	{
		if(petData.skill[i] / 1000 == 0)
			continue;
		
		pThisMagicData = g_pNpcMagicManager->QueryEudemonData(petData.skill[i] / 1000);
		if(!pThisMagicData)
			continue;
		
		int nMagicType = pThisMagicData->GetInt(EM_EUDEMONMAGIC_dataID);

		pTypeData = CMagic::FindMagicType(nMagicType);
		if(!pTypeData)
			continue;

		if(pTypeData->GetInt(MAGICTYPEDATA_SORT) != MAGICSORT_EUDEMON_SOUL)
			continue;

		int nAtkType = pTypeData->GetInt(MAGICTYPEDATA_ATTACK_TYPE);
		OBJID idMagicType = pTypeData->GetInt(MAGICTYPEDATA_TYPE);
		switch(nAtkType)
		{
		case ATKTYPEMODE_COMMON:
			nphyatt = 7.35f * petData.level + 0.06f * nQuality * qualityParam + 23.34f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//低级技能
			{
				nphyatt *= 0.7f;//外攻
			}
			break;
		case ATKTYPEMODE_INNNER:
			nmagatt = 7.35f * petData.level + 0.06f * nQuality * qualityParam + 23.34f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//低级技能
			{
				nmagatt *= 0.7f;//内攻
			}
			break;
		case ATKTYPEMODE_ICE:
			nSoul2 = 1.093f * petData.level + 0.013f * nQuality * qualityParam + 3.473f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//低级技能
			{
				nSoul2 *= 0.7f;//冰
			}
			break;
		case ATKTYPEMODE_FIRE:
			nSoul1 = 1.093f * petData.level + 0.013f * nQuality * qualityParam + 3.473f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//低级技能
			{
				nSoul1 *= 0.7f;//火
			}
			break;
		case ATKTYPEMODE_NOXIOUS:
			nSoul4 = 1.093f * petData.level + 0.013f * nQuality * qualityParam + 3.473f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//低级技能
			{
				nSoul4 *= 0.7f;//毒
			}
			break;
		case ATKTYPEMODE_MYSTERY:
			nSoul3 = 1.093f * petData.level + 0.013f * nQuality * qualityParam + 3.473f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//低级技能
			{
				nSoul3 *= 0.7f;//玄
			}
			break;
		default://其他功能的幻兽技能
			{
				if(idMagicType == 8501)
					m_eudemonSoulAddHp = nLifeParam * 0.3;//30%
				else if(idMagicType == 8502)
					m_eudemonSoulAddHp = nLifeParam * 0.5;
				else if(idMagicType == 8503)
					m_eudemonSoulAddMp = nLifeParam * 0.3;
				else if(idMagicType == 8504)
					m_eudemonSoulAddMp = nLifeParam * 0.5;
				else if(idMagicType == 8513)//低级闪避
					m_eudemonSoulAddDodge = 0.0665f * petData.level + 0.007f * nQuality * qualityParam + 2.11f * petData.fusion * fusionParam;
				else if(idMagicType == 8514)//高级闪避
					m_eudemonSoulAddDodge = 0.95f * petData.level + 0.01f * nQuality * qualityParam + 3.015f * petData.fusion * fusionParam;
				else if(idMagicType == 8515)//低级命中
					m_eudemonSoulAddHitRate = 0.0665f * petData.level + 0.007f * nQuality * qualityParam + 2.11f * petData.fusion * fusionParam;
				else if(idMagicType == 8516)//高级命中
					m_eudemonSoulAddHitRate = 0.95f * petData.level + 0.01f * nQuality * qualityParam + 3.015f * petData.fusion * fusionParam;
			}
			break;
		}
	}
	m_eudemonSoulphyatt += nphyatt;//外攻
	m_eudemonSoulmagatt += nmagatt;//内攻
	m_eudemonSoul1 += nSoul1;//火
	m_eudemonSoul2 += nSoul2;//冰
	m_eudemonSoul3 += nSoul3;//玄
	m_eudemonSoul4 += nSoul4;//毒
	//检查
	if(m_eudemonSoulphyatt < 0)
		m_eudemonSoulphyatt = 0;
	if(m_eudemonSoulmagatt < 0)
		m_eudemonSoulmagatt = 0;
	if(m_eudemonSoul1 < 0)
		m_eudemonSoul1 = 0;
	if(m_eudemonSoul2 < 0)
		m_eudemonSoul2 = 0;
	if(m_eudemonSoul3 < 0)
		m_eudemonSoul3 = 0;
	if(m_eudemonSoul4 < 0)
		m_eudemonSoul4 = 0;

	return true;
}
