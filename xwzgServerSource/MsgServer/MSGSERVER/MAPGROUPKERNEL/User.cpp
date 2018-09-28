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
#include "SKHonorMurderList.h"//20070213����
#include "SKPostOffice.h"
#include "UserMarryRing.h"
#include "DataInspectManage.h"
#include "AdjustRate.h"
#include "SettledAction.h"	//  [2007-8-15] ���
#include "MapGroup.h"
#include "WargameManager.h"///2007-08-25softworms
#include "MapInstanceMgr.h"///2008-01-12softworms-����������
#include "CombGod.h"		//����ϵͳ
#include "GiftManager.h"//20071123����:
#include "MsgGift.h"//20071204����:
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
const int	ADD_ENERGY_STAND_SECS			= 5;			// ÿ����������һ��
const int	ADD_ENERGY_STAND				= 50;			// վ����������
const int	DEL_ENERGY_PELT_SECS			= 3;			// ����״̬ÿ3���һ��
const int	DEL_ENERGY_PELT					= 2;			// ÿ�ο�2��
const int	KEEP_STAND_MS					= 1500;			// վ����Ҫ��ʱ�䣬վ�Ⱥ��ܵ�ס��ҡ�
const int	CRUSH_ENERGY_NEED				= 100;			// ������Ҫ���ٵ�
const int	CRUSH_ENERGY_EXPEND				= 100;			// �������Ķ��ٵ�
const int	SYNWAR_PROFFER_PERCENT			= 1;			// �����ӵþ���ֵ�İٷֱ�
const int	SYNWAR_MONEY_PERCENT			= 2;			// �����ӵ�Ǯ�İٷֱ�
const int	MIN_SUPERMAP_KILLS				= 25;			// ��˫�б����
//const int	NEWBIE_LEVEL					= 70;			// ���ֵĵȼ�
const int	VETERAN_DIFF_LEVEL				= 20;			// ���ֵĵȼ���
//const int	_MIN_TUTOR_LEVEL				= 50;							// ��ʦ��͵ȼ�
const int	HIGHEST_WATER_WIZARD_PROF		= 135;			// ˮ����
const int	MOUNT_ADD_INTIMACY_SECS			= 3600;			// 
const int	INTIMACY_DEAD_DEC				= 20;			// 
const int	SLOWHEALLIFE_MS					= 1000;							// ��ҩ��Ѫ��������ȥ�ĺ�������

const int	AUTOHEAL_TIME					= 600;
const int	AUTOHEAL_EACHLIFE				= 6;							// ÿ�β�Ѫ����
const int	AUTOHEAL_EACHMANA				= 3;							// ÿ�β�������
const int	AUTOHEAL_EACHSP					= 12;							// ÿ�λ�SP����
const int	AUTOHEAL_GENEEACHLIFE			= 9;							//�����������ӻ�Ѫ,����50%
const int	AUTOHEAL_GENEACHMANA			= 5;							//ͬ��
const int	AUTOHEAL_GENEEACHSP				= 24;							//ͬ��

const int	ITEM_LIFE_TIME_FIRST_CHECK_INTERVAL_SEC	= 30;							//  [2007-10-10] ���	��һ�����߼����
const int	ITEM_LIFE_TIME_CHECK_INTERVAL_SEC	= 60 /** 30*/;							//  [2007-9-14] ���	��Ʒ�������ڼ���� ��
const int	ITEM_LIFE_TIME_UPDATE_INTERVAL  = 1000 * 60 /** 30*/;						//  [2007-9-19] ���	��Ʒ�������ڸ��¿ͻ��˼��

const int	VIP_CHECK_SEC	= 60;							//  [2007-10-10] ���	��һ�����߼����
const int   SYN_WORK_SEC	= 15 * 60;
const int	VIP_FACE_SEC	= 60;
const int	ESEPCIAL_DRUG_SEC	=	6;		//����Ĵ������ָ�ʱ����
const int	EverdayOL_SEC	= 60;			//ÿ���Ӽ�һ��������ʱ

const int	UNDATEDATABASE_BASETIME = 300;
const int	UNDATEDATABASE_OFFTIME = 180;

const int	TICK_SECS						= 10;
const int	MAX_PKLIMIT						= 8;						// PKֵ�������С����(���ؾ�)  //change huang 2004.1.11
const int	PILEMONEY_CHANGE				= 5000;							// �����ֽ�䶯(��Ҫ��ʱ����)
const int	_ADDITIONALPOINT_NUM			= 0;							// ��1���Ӷ������Ե�//20070126���޸�Ϊ1��
const int	PK_DEC_TIME						= 30*60   /*20100730JoyCc��ÿ30���ӵ�һ��/*20*60*/;//[��;���� 2009.06.03]ÿ6���ӵ�һ��							//20070228����:20���� ÿ���60�뽵��pkֵ     //change huang 2004.1.11   
//const int	PK_DEC_TIME_PRISON				= 5;
const int	FATIGUE							= 60;
const int   PKVALUE_DEC_ONCE                = 1;                            // ��ʱ�����PKֵ��        

const int	PKVALUE_DEC_PERDEATH			= 30;							// ��PK��һ�μ��ٵ�PKֵ
const int	TIME_TEAMPRC					= 5;							// ������Ϣ�����ʱ��
const int	DURABILITY_DEC_TIME				= 12;							// ÿ12�뽵��һ�ΰ�ʱ�����ĵ�װ���;ö�
const int	USER_ATTACK_SPEED				= 1000/*800 */;							// ���ͽ�ֹ���Ƶ��
const int	POISONDAMAGE_INTERVAL			= 2;							// �ж�ÿ2����Ѫһ��
const int	WARGHOST_CHECK_INTERVAL			= 10;							// ÿ��10����һ��ս��ȼ�(��������)
const int	AUTO_REBORN_SECS				= 30;							// ���ʯ30����Զ�ʹ��
const int	INC_POTENTIAL_SECS				= 6*60;							// ÿ��6��������һ��Ǳ��
const int	INC_POTENTIAL_NUM				= 1;							// ÿ������1��Ǳ��
const int	ADD_POTENTIAL_RELATIONSHIP		= 6;							// ��ϵֵ֮��ÿ�ﵽ6����1��Ǳ��ֵ

// add by zlong 2003-11-28
const int	SPRITE_ADD_EXP_SECS				= 60;			//Ԫ�ؾ���ÿ��������һ�ξ���
const int	SPRITE_ADD_EXP					= 1;			//������������

const int	EUDEMON_ADD_FIDELITY_SECS		= 60*6;			// ����ÿ6��������һ�����ܶ�
const int	EUDEMON_ADD_FIDELITY			= 1;			// ÿ�����ӵ����ܶ�
const int	MAX_EUDEMON_FIDELITY			= 1000;			// ������ܶ�1000
const int	MAX_RETURN_BORN_POS_LEV			= 4;			// �����󷵻س�ʼ����������ȼ�

//const int	STATUS_NEVERDISAPPER			= 86400;		// ����������ʱ��״̬ʱ��

const int	SPIRITPERCENT2GRID				= 10000;			//����VALUEת����һ�����
const int	SPIRITGRID_NUMLIMIT				= 5;			//��һ��������������ֵ


const int	SKZL_ADD_PHYSICALDEFENCE		= 100;			//SKZL״̬���ӵ������������
const int	SKZL_ADD_MAGICDEFENCE			= 100;			//..............ħ����������

const int	FROSTSHIELD_DECSPEEDPERCENT		= 20;			//��˪�׽��Ͷ����ƶ��ٶ�20%		
const int	FROSTSHIELD_DECSPEEDTIME		= 3;			//����ʱ������
const int	EUDEMON_MIN_MIX_LEV				= 60;

//20070125����:����ϵͳ�ص���-------------------------------
const double	_GROW_WARRIOR_STR				= 4;			//սʿ ���� ÿ���ӵ�
const double	_GROW_WARRIOR_DEX				= 2;			//սʿ ���� ÿ���ӵ�
const double	_GROW_WARRIOR_VIT				= 3;			//սʿ ���� ÿ���ӵ�
const double	_GROW_WARRIOR_SOUL				= 1;			//սʿ ���� ÿ���ӵ�
const double	_GROW_WARRIOR_LOVELI			= 0;			//սʿ ���� ÿ���ӵ�

const double	_GROW_ARCHER_STR				= 2;			//���� ���� ÿ���ӵ�
const double	_GROW_ARCHER_DEX				= 4;			//���� ���� ÿ���ӵ�
const double	_GROW_ARCHER_VIT				= 3;			//���� ���� ÿ���ӵ�
const double	_GROW_ARCHER_SOUL				= 1;			//���� ���� ÿ���ӵ�
const double	_GROW_ARCHER_LOVELI				= 0;			//���� ���� ÿ���ӵ�

const double	_GROW_MAGE_STR					= 2;			//��ʦ ���� ÿ���ӵ�
const double	_GROW_MAGE_DEX					= 1;			//��ʦ ���� ÿ���ӵ�
const double	_GROW_MAGE_VIT					= 3;			//��ʦ ���� ÿ���ӵ�
const double	_GROW_MAGE_SOUL					= 4;			//��ʦ ���� ÿ���ӵ�
const double	_GROW_MAGE_LOVELI				= 0;			//��ʦ ���� ÿ���ӵ�
//����ϵͳ�ص���end---//���2007/02/02--------------------------------------

//20070314����:
//��߽�λ
const int _MAX_RANK_LEV = 10;
//��λ��Ӧ���׵�
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

////////////////////////06.3.23�������//////////////////////////////
enum
{
	FORCE,	//����
	DEX,	//����
	SOUL,	//����
	HEALTH,	//����
};

int warAttBase[]	=	{ 5,2,1,3 };
int bowAttBase[]	=	{ 1,2,6,2 };
int encAttBase[]	=	{ 2,5,2,2 };
/////////////////////////////////////////////////////////////////////
//��������/���ﻻƤ���
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
//�½�ɫ���������ͼ
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
		SendSysMsg("��CHEAT��");
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
	m_pMonster = NULL;//[��;���� 2009.04.03]
	m_idItem = NULL;//[��;���� 2009.05.22]
	SetObjType(OBJ_USER);

	m_link.Init(this);
	m_nStep			= STEP_LOGIN;		// ��ͼ�������ǽ����ͼ

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

	//���ɷ�װ��armor type;
	m_nSynDressArmorType = 0;
	m_pTaskDetail	= NULL;
	m_bIsEnableWarGhostExp = false;

	//06.3.21����
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
	//20070403����
	//applay
	m_nApply = 0;
	m_idApplyTarget = 0;

	//  [2007-9-29] ���	���ϳ�ʼ��
	m_nApplyByName = 0;
	memset(m_szApplyName, 0, _MAX_NAMESIZE);

	//ʶ��״̬���
	m_bIgnoreTargetDodge= false;
	m_bMustCritHit		= false;
	m_targetID			= NULL;
	//-----------------------end--------------
	m_bRebornFlag		=	false;
	m_nRecoverPercent	= 0;

	m_nFightRate = USER_ATTACK_SPEED;//20070425����:Ĭ��ֵ

	m_bFirstCheckItemLifeTime = TRUE;

	m_dwVas = 0;//��ʼ����ֵ��
	m_dwVasJuan = 0;

	//����Ŀ��
	m_IdLockTarget = 0;
// 	ClearHouseItem();

	//�����ɱ������
	m_KillMonsterNum = 0;
	
	m_killMonsterTargetNum = 0;       //ɱ�ֵ�Ŀ������(��ȡ���ݿ�)
	m_killMonsterTimeInterval = 0; //ɱ�ֵ�ʱ����(��ȡ���ݿ�)
	
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
	m_bHasTempFriend = false;//�Ƿ�����ʱ���ѣ����û�У�����ʱ�Ͳ���ִ��ɾ��
	m_GoldPoint = 0;

	m_doubletime=0;
	m_bInstanceReborn=false;

	//goto �µ������;���洢�����õ�������
	m_MoneyIncr = 0;
	m_ExpIncr = 0;

	//goto �����
	m_nActivityAward = 0;
	m_ChargePack1=0;
	m_ChargePack2=0;
	m_ChargePack3=0;
	m_pTitle = NULL;
	m_pTreasure = NULL;
	m_pOnlineGift = NULL;
	m_nSendLeaveWord = 0;//goto ��������ɾ��

	m_nConMoney = 0;
	m_nConBuild = 0;
	m_nLevExp = -1;

	m_nTutorQuestion = 0;//ʦͽ�������
	m_nTutorAnswer = 0;//ʦͽ�����

	m_pPetEgg = NULL;
	//ר�����
	m_pSpecilLineInfo = NULL;
	m_bChangeLine=false;
	
	m_pUserWarGame = NULL;
//modify code by �ֵ�Ԩ begin 2011.5.30-
	m_pvaslvdata = NULL;
//modify code by �ֵ�Ԩ end
	m_pVerification = NULL;
	m_Verification_pass=false;
	m_bLoaded = false;
	m_Verification_needChk=false;
	m_idSoulEudemon = ID_NONE;

	m_eudemonSoulphyatt = 0;//�⹥
	m_eudemonSoulmagatt = 0;//�ڹ�
	m_eudemonSoul1 = 0;//��
	m_eudemonSoul2 = 0;//��
	m_eudemonSoul3 = 0;//��
	m_eudemonSoul4 = 0;//��
	m_eudemonSoulAddHp = 0;//��Ѫ
	m_eudemonSoulAddMp = 0;//����
	m_eudemonSoulAddDodge = 0;//������buff
	m_eudemonSoulAddHitRate = 0;//������buff
	//���ڳ����Ѫ���ܵ�CD
	m_tLastAddHp = 0;
	m_tLastAddMp = 0;
	m_bSoulEudemonView = false;
//modify code by �ֵ�Ԩ begin 2011.6.22
	newvalue = 0;
	newvaluetrageid = 0;
//modify code by �ֵ�Ԩ end
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
//modify code by �ֵ�Ԩ begin 2011.5.30-
	DEBUG_TRY
	if(m_pvaslvdata)
		m_pvaslvdata->ReleaseByOwner();
	DEBUG_CATCH("~user error m_pvaslvData->ReleaseByOwner()");
//modify code by �ֵ�Ԩ end
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

///�ж���ҵ�ǰ�Ƿ���Ĭ�ϵ�¼����
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
			//goto ���µ�ͼ�������
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

//  [2007-8-15] ��� C����_20062007081101_�л���ͼ�鴥��action�߻���
// 	ST_SETTLEDACTIONINFO* pInfoTemp = g_settledAction.GetSettledActionInfoByType(EM_SETLDACTN_TYPE_ENTER_MAP);
// 	if(pInfoTemp && pInfoTemp->actionid)
// 	{
// 		GameAction()->ProcessAction(pInfoTemp->actionid, this);
// 	}
//	SendAllMagicInfo();//[2010-06-28 goto]ͬ�������б�
	
	nStep = 9;
	// satan����������������������������

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
		///��������ͼ���Ƕ�����ı���ڵ�ͼid
// 		if(GetTeam() && GetTeam()->GetLeader()==GetID())
// 		{
// 			GetTeam()->SetTeamMap(GetMap()->GetID(),true);
// 		}

		///�����¼�ĵ�ͼid��ͬ,���Էɵ�����
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
// 				m_bInstanceReborn=true;///���������ٵ��ó����ű�
// 				RequestEnterInstance(menberInfo.idMap,0,1,menberInfo.usPosX,menberInfo.usPosY);
// 				nStep = 13;
// 				GetTeam()->SetLocation(GetID(),0,0,0);///���
// //				menberInfo.idMap=menberInfo.usPosX=menberInfo.usPosY=0;
// 			}
// 		}
// 		DEBUG_CATCH2("CUser::EnterMap ���߽��������̱�����!nStep:%d",nStep);
		nStep = 14;

		///������ͼ�ڽ����ʱ�򲻱�������,ֻ��������ʱ�ű���
		TimeCostChk("Timecheck","SendPosToTeamMem()");
 		if(GetMap()->IsDynaMap()==false)//ר�߰���Ҳ��
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
		//goto ֪ͨ����
		SendPosToTeamMem(idMapDoc, GetPosX(), GetPosY());

		//VIP�б�
		nStep = 16;
		if(IsVipActive())
		{
			g_VipListMgr.AddVipUser(this);
			//�㲥��������·
			CMsgVipList msg;
			if (msg.Create(GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_ONLINE))
			{
				msg.Append(GetID(), GetMap()->GetDocID(), GetLev(), CVipList::GetLineByName(), GetVipLev(), GetName());
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
			}
		}

		//��������Ӫ
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
					//����һ������������ȴ���˾���������
					::LogSave("User enter wargame without signin up!userid=%u,name=%s", GetID(), GetName());
					//֪ͨ�ͻ�������
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
	IF_NOT (msg.Create(GetID(), _USERATTRIB_NONE, 0))			// _USERATTRIB_NONE : �����
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

//				// �ȼ���70�����·����仯����Ҫ��������ֵ�洢��ʽ
//				if (nNewLev > ADJUST_EXPFORMAT_LEVEL && nOldLev <= ADJUST_EXPFORMAT_LEVEL)
//					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()/10, nSynchro);
//				else if (nNewLev <= ADJUST_EXPFORMAT_LEVEL && nOldLev > ADJUST_EXPFORMAT_LEVEL)
//					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()*10, nSynchro);
			}
			break;
		//--------------------�������[08.9.27]���------------------//
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
// 				int nNewPackSize = __min(_MAX_PACK_TYPE - 1,nOldPackSize+i64Data);//_MAX_PACK_TYPE - 1,���4��......
// 				m_data.SetPackCount(nNewPackSize, true);
// 				IF_NOT (msg.Append(_USERATTRIB_PACKCOUNT, GetPackSize()))
// 					return false;

//				// �ȼ���70�����·����仯����Ҫ��������ֵ�洢��ʽ
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
				//20090612haocq��������ʱ��
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

				// �ȼ���70�����·����仯����Ҫ��������ֵ�洢��ʽ
				if (nNewLev > ADJUST_EXPFORMAT_LEVEL && nOldLev <= ADJUST_EXPFORMAT_LEVEL)
					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()/10, nSynchro);
				else if (nNewLev <= ADJUST_EXPFORMAT_LEVEL && nOldLev > ADJUST_EXPFORMAT_LEVEL)
					this->SetAttrib(_USERATTRIB_EXP, this->GetExp()*10, nSynchro);

				//������ҵ��츳����
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
				//������ʱ��Ӵ˴�,��Ϊ�Զ���Ѫ,û�й㲥Ѫ��
				if( this->GetTeam() )//�����
				{
					this->BroadcastTeamLife();	//����㲥Ѫ��
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
				if( this->GetTeam() )//�����
				{
					this->BroadcastTeamMana();	//����㲥Ѫ��
				}
			}
			break;
		case	_USERATTRIB_EXP:
			{
				//goto ��������
				if (!IsInMapInstance())//�����в��ƣ���Ϊ�����������ᱣ�������Ϣ�����Ҽ�̳����̫�࣬��ֹ��̳ʱ�����ݿ�ѹ������
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

				SendSysMsg("�������%u����",i64Data);
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
				
				SendSysMsg("�������%u����",i64Data);
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
					return false;	// ���Դ�
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
			//����ע�ͣ�Ǳ��..
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
			//����ע��Ӷ���ȼ��뾭��..
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
	//����ע�ͣ����ڵ�ʦ�����빦ѫ..
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
		case _USERATTRIB_STONELEV:   //ʯͷ��
            {
// 				int nOldStoneLev = GetStoneLev();
// 				int nNewStoneLev = nOldStoneLev+i64Data;
// 			//	int nNewStoneLev = __min(_MAX_STORAGE_TYPE, nOldStorageLimit+i64Data*18);
// 				m_data.SetStoneLev(nNewStoneLev, true);
// 				IF_NOT (msg.Append(_USERATTRIB_STONELEV, GetStoneLev()))
// 					return false;
			}
			break;
		case _USERATTRIB_GRASSLEV://            = 83,   //��ľ
			{
// 				int nOldGrassLev = GetGrassLev();
// 				int nNewGrassLev = nOldGrassLev+i64Data;
// 				//	int nNewStoneLev = __min(_MAX_STORAGE_TYPE, nOldStorageLimit+i64Data*18);
// 				m_data.SetGrassLev(nNewGrassLev, true);
// 				IF_NOT (msg.Append(_USERATTRIB_GRASSLEV, GetGrassLev()))
// 					return false;
			}
			break;
		case _USERATTRIB_BODYLEV://             = 84,   //Ƥ֬
			{
// 				int nOldBodyLev = GetBodyLev();
// 				int nNewBodyLev = nOldBodyLev+i64Data;
// 				//	int nNewStoneLev = __min(_MAX_STORAGE_TYPE, nOldStorageLimit+i64Data*18);
// 				m_data.SetBodyLev(nNewBodyLev, true);
// 				IF_NOT (msg.Append(_USERATTRIB_BODYLEV, GetBodyLev()))
// 					return false;
			}
			break;
		case _USERATTRIB_BOXLEV://              = 85,   //������
			{
// 				int nOldBoxLev = GetBoxLev();
// 				int nNewBoxLev = nOldBoxLev+i64Data;
// 				//	int nNewStoneLev = __min(_MAX_STORAGE_TYPE, nOldStorageLimit+i64Data*18);
// 				m_data.SetBoxLev(nNewBoxLev, true);
// 				IF_NOT (msg.Append(_USERATTRIB_BOXLEV, GetBoxLev()))
// 					return false;
			}
			break;
		case _USERATTRIB_ARENA_SCORE://����������
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

	//20070316����:����ͬ�����Ѫ���ı�--------------------------
	//ֻҪ�����Ѫ��.������nSynchro.ֱ�ӷ���㲥
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
	if(!msg.Create(GetID(), _USERATTRIB_NONE, 0))			// _USERATTRIB_NONE : �����
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
			//20090612haocq��������ʱ��
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

			// �ȼ���70�����·����仯����Ҫ��������ֵ�洢��ʽ
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
	case _USERATTRIB_GIFT_GIFTPOINT:		//�츳��
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
			//����ע�ͣ�����Ǳ��..
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
//------------------------06.12.13�������,���ڻ�----------begin--------------------------

	case	_USERATTRIB_EXP:
		{
			//goto ��������
			if (!IsInMapInstance())//�����в��ƣ���Ϊ�����������ᱣ�������Ϣ�����Ҽ�̳����̫�࣬��ֹ��̳ʱ�����ݿ�ѹ������
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
			// ��֧���޸�LOOKFACE����
			if(!msg.Append(idxAttr, i64Data))
				return;
		}
		break;
	case	_USERATTRIB_SIZEADD:
		{
			// ��֧���޸�SIZEADD����
			if(!msg.Append(idxAttr, i64Data))
				return;
		}
		break;
	case	_USERATTRIB_SUPERMAP_ORDER:
		{
			// ��֧���޸�SIZEADD����
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
		//����ע�����ϣ�����Ӷ��
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
	case _USERATTRIB_HIDESTATUS://�Ƿ�����
		{
			if( !msg.Append( _USERATTRIB_HIDESTATUS,i64Data ) )
				return;
		}
		break;
	case _USERATTRIB_USETITLE://[��;���� 2008/10/14]�ƺ�ʹ��״̬
		{
			if (!msg.Append(_USERATTRIB_USETITLE, i64Data))
			{
				return;
			}
		}
		break;
	case _USERATTRIB_ARENA_SCORE://����������
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

	//20070316����:����ͬ�����Ѫ���ı�--------------------------
	//ֻҪ�����Ѫ��.������nSynchro.ֱ�ӷ���㲥
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
//			if(!msg.Append(_USERATTRIB_EXPUPPERCENT, GetUserExpUpPre()))// �½����޸� [6/16/2007]
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
	return;//�޴˹��ܣ�����
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
// 			//����������������
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
	///softwormsΪ�˵��Զ�ȥ������PK����
	//CRole::DetachStatus(this->QueryRole(), STATUS_PK_PROTECT);

	//m_tLock.Clear();
}


//////////////////////////////////////////////////////////////////////
void CUser::ProcessOnAttack()
{
	///softwormsΪ�˵��Զ�ȥ������PK����
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
	if( prof & PROF_NEWHAND ) //����
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*0.8f;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*1.0f;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*1.2f;
		
	}
	else
	{
		LOGERROR( "CUser::GetAtkHitRate,�û�:%sְҵ:%d",this->GetName(),prof );
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*0.8f;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*1.2f;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*1.0f;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*1.4f;
		
	}
	else
	{
		LOGERROR( "CUser::GetAtkHitRateUserOnly,�û�:%sְҵ:%d",this->GetName(),prof );
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
		//[��;���� 2009.04.27]�޸ĺ���,ȡ�õ���Ʒ�������ϵ�װ��
		
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData1*45 + dwData2 * 150;
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData1*45 + dwData2 * 150;
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData1*60 + dwData2 * 150;
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else
	{
		LOGERROR( "CUser::GetMaxLife��������,�û�:%sְҵ:%d",this->GetName(),prof );
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

	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData1*45 + dwData2 * 150;
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData1*45 + dwData2 * 150;
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData1*60 + dwData2 * 150;
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData1*50 + dwData2 * 150;
	}
	else
	{
		LOGERROR( "CUser::GetMaxLifeUserOnly��������,�û�:%sְҵ:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}
DWORD CUser::GetMaxMana()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData1 =  GetSpi();
	DWORD dwData2 = GetLev();
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData1*38.5f + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData1*38.5f + dwData2 * 150 * 0.7f;	
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData1*28 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else
	{
		LOGERROR( "CUser::GetMaxManaUserOnly��������,�û�:%sְҵ:%d",this->GetName(),prof );
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData1*38.5f + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData1*38.5f + dwData2 * 150 * 0.7f;	
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData1*28 + dwData2 * 150 * 0.7f;
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData1*35 + dwData2 * 150 * 0.7f;
	}
	else
	{
		LOGERROR( "CUser::GetMaxManaUserOnly��������,�û�:%sְҵ:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}

DWORD CUser::GetCrit()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetAgi();
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*1;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*1.1f;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*0.9f;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*1.1f;
		
	}
	else
	{
		LOGERROR( "CUser::GetCrit,�û�:%sְҵ:%d",this->GetName(),prof );
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*1;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*1.1f;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*0.9f;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*1.1f;
		
	}
	else
	{
		LOGERROR( "CUser::GetDdgUserOnly,�û�:%sְҵ:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}
DWORD CUser::GetAtk()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetStr();
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*10;
		
	}
	else
	{
		LOGERROR( "CUser::GetAtk,�û�:%sְҵ:%d",this->GetName(),prof );
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*10;
		
	}
	else
	{
		LOGERROR( "CUser::GetMinAtkUserOnly��������,�û�:%sְҵ:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}
DWORD CUser::GetMgcAtk()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetIntex();
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*11;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*2;
		
	}
	else
	{
		LOGERROR( "CUser::GetMgcAtk,�û�:%sְҵ:%d",this->GetName(),prof );
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*11;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*2;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*2;
		
	}
	else
	{
		LOGERROR( "CUser::GetMgcAtkUserOnly��������,�û�:%sְҵ:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}
DWORD CUser::GetDef()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetSpi();

	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*6;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*5;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*5;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*10;
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*12;
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*8;
	}
	else
	{
		LOGERROR( "CUser::GetDef��������,�û�:%sְҵ:%d",this->GetName(),prof );
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*6;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*5;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*5;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*10;
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*12;
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*8;
	}
	else
	{
		LOGERROR( "CUser::GetDefUserOnly��������,�û�:%sְҵ:%d",this->GetName(),prof );
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

	//  [12/13/2007 ���]	�츳�ӳ�����
/*	nData += ProcGiftUserPropertyAdd(EM_GTTMP1005_TARGETATT_soul);*/

	return __max(0, nData);
}

DWORD CUser::GetDdg()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetAgi();
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*1;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*0.6f;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*0.6f;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*0.8f;
	}
	else
	{
		LOGERROR( "CUser::GetDdg,�û�:%sְҵ:%d",this->GetName(),prof );
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*1;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*0.6f;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*0.6f;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*1;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*0.8f;
		
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*0.8f;
	}
	else
	{
		LOGERROR( "CUser::GetDdgUserOnly,�û�:%sְҵ:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}


DWORD CUser::GetMagicDef()
{
	double value = 0.0; 
	DWORD prof = this->GetProfession();
	DWORD dwData = GetSpi();

	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*8;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*11;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*8;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*8;
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*7;
	}
	else
	{
		LOGERROR( "CUser::GetMagicDef��������,�û�:%sְҵ:%d",this->GetName(),prof );
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
	
	if( prof & PROF_NEWHAND ) //����
	{
		value =  dwData*10;
	}
	else if( prof & PROF_ASSIST )//����
	{
		value = dwData*8;
		
	}
	else if( prof & PROF_MASTER )//��ʦ
	{
		value = dwData*10;
		
	}
	else if( prof & PROF_PASTOR )//��ʦ
	{
		value = dwData*11;
		
	}
	else if( prof & PROF_ASSISSIN )//�̿�
	{
		value = dwData*8;
		
	}
	else if( prof & PROF_SOLDIER )//սʿ
	{
		value = dwData*8;
	}
	else if( prof & PROF_SWORD )//����
	{
		value = dwData*7;
	}
	else
	{
		LOGERROR( "CUser::GetMagicDefUserOnly��������,�û�:%sְҵ:%d",this->GetName(),prof );
	}
	return (DWORD)(value+0.5);
}

bool CUser::CheckLevUp(int nCurLev)
{
	//��ְҵ
	int nShowLev = -1;

	if(nCurLev >= 19)
	{
		int nCountLev = (nCurLev - 19) / 10;
		if(GetJobLev(1) < (nCountLev + 1) * 10)
			nShowLev = (nCountLev + 1) * 10;

		if(nShowLev != -1)
		{
			SendSysMsg("��ְҵ�ȼ��ﵽ%u���ſɼ�������",nShowLev);
			return false;
		}
	}
	return true;
}

void CUser::IncLev(int nLev,int nType)
{
	if(nType == 0) //��������
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
			if(dwPro & PROF_NEWHAND) //���ֲ��ܳ���10��
			{
				SendSysMsg(_TXTATR_TOPSHOW,"�������ɲ��ܼ�����������ȼ�");
				return;
			}
			CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
			if (!pLevupexp)
				return;
			__int64 i64LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
			
			if(this->GetExp() < i64LevupExp)
			{
				SendSysMsg(_TXTATR_TOPSHOW,"���ľ��鲻�㣬��������");
				return;
			}
			
			this->SetAttrib( _USERATTRIB_EXP,this->GetExp() - i64LevupExp,SYNCHRO_TRUE );
		}

		for(int i = 1 ; i < 3 ; i ++)
		{
			DWORD dwPro = m_data.GetJobByType(i);
			if(dwPro == 0 )
				continue;

			if( dwPro & PROF_NEWHAND) //����
			{
				m_data.AddStr( 36 * nLev,i );
				m_data.AddIntex( 36 * nLev,i);
				m_data.AddSta( 36 * nLev,i );
				m_data.AddSpi( 36 * nLev,i );
				m_data.AddAgi( 36 * nLev,i);

				//m_data.SetJobLev(GetLev() + nLev,i);
			}
			else if( dwPro & PROF_ASSIST) //����
			{
				m_data.AddStr( 42* nLev,i);
				m_data.AddIntex( 57* nLev,i );
				m_data.AddSta( 57* nLev,i );
				m_data.AddSpi( 68* nLev,i );
				m_data.AddAgi( 42* nLev,i );
			}
			else if( dwPro & PROF_MASTER) //��ʦ
			{
				m_data.AddStr( 37* nLev,i );
				m_data.AddIntex( 62* nLev,i);
				m_data.AddSta( 56* nLev,i );
				m_data.AddSpi( 68* nLev,i);
				m_data.AddAgi( 37* nLev,i);
			}
			else if( dwPro & PROF_PASTOR) //��ʦ
			{
				m_data.AddStr( 43* nLev,i );
				m_data.AddIntex( 62* nLev,i);
				m_data.AddSta( 50* nLev,i );
				m_data.AddSpi( 68* nLev,i);
				m_data.AddAgi( 43* nLev,i);
			}
			else if( dwPro & PROF_ASSISSIN) //�̿�
			{
				m_data.AddStr( 62* nLev,i );
				m_data.AddIntex( 50* nLev,i);
				m_data.AddSta( 62* nLev,i );
				m_data.AddSpi( 50* nLev,i);
				m_data.AddAgi( 62* nLev,i);
			}
			else if( dwPro & PROF_SOLDIER) //սʿ
			{
				m_data.AddStr( 56* nLev,i );
				m_data.AddIntex( 37* nLev,i);
				m_data.AddSta( 74* nLev,i );
				m_data.AddSpi( 68* nLev,i);
				m_data.AddAgi( 37* nLev,i);
			}
			else if( dwPro & PROF_SWORD) //����
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
		//goto ��������Ժ�Ͱ�������0������д���ݿ����
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
			if(GetLev()>10 && GetLev()%5==0)//ÿ����5�ı���ʱ������һ��ʦͽ��������
				m_pUserTutor->ProcessGiftMask(GetLev());
		}
		DEBUG_CATCH("ProcessGiftMask(GetLev())")
//modify code by �ֵ�Ԩ begin 2011.5.24
//		if (GetLev()>20)
//			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) level up to %d.", GetID(), GetName(), GetLev());
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) (level up:%d) (first job level:%d) (second job level:%d) (exp:%d)", GetID(), GetName(), GetLev(), m_data.GetJobLev(1), m_data.GetJobLev(2), GetExp());
//modify code by �ֵ�Ԩ end
	}
	else if(nType == 1)
	{
		if((GetProfession(1) & PROF_NEWHAND) != 0 || GetProfession(1) == 0)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"��ְҵ��������");
			return;
		}

		
// 		if(m_data.GetJobLev(nType) == 79)
// 		{
// 			//�����ж�
// 			IMagicData* pMagic = NULL;
// 			DWORD prof = this->GetProfession(1);
// 
// 			for(int skillIndex = 0; skillIndex < 18 ; skillIndex ++)
// 			{
// 				if( prof & PROF_ASSIST )//����
// 				{
// 					if(skillIndex == 8 ||skillIndex == 10 || skillIndex == 14 || skillIndex == 15 || skillIndex == 16)
// 						continue;
// 					pMagic = QueryMagic(1400 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_MASTER )//��ʦ
// 				{
// 					if(skillIndex == 8 || skillIndex == 10 || skillIndex == 13 || skillIndex == 14 || skillIndex == 15)
// 						continue;
// 					pMagic = QueryMagic(1120 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_PASTOR )//��ʦ
// 				{
// 					if(skillIndex == 8 || skillIndex == 11 || skillIndex == 14 || skillIndex == 16 || skillIndex == 17)
// 						continue;
// 					pMagic = QueryMagic(1138 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_ASSISSIN )//�̿�
// 				{
// 					if(skillIndex == 8 || skillIndex == 13 || skillIndex == 15 || skillIndex == 16 || skillIndex == 17)
// 						continue;
// 					pMagic = QueryMagic(1200 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_SOLDIER )//սʿ
// 				{
// 					if(skillIndex == 0 || skillIndex == 5 ||  skillIndex == 9 || skillIndex == 14 || skillIndex == 16 || skillIndex == 17)
// 						continue;
// 					pMagic = QueryMagic(1100 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 				else if( prof & PROF_SWORD )//����
// 				{
// 					if(skillIndex == 8 || skillIndex == 10 ||  skillIndex == 13 || skillIndex == 16 || skillIndex == 17)
// 						continue;
// 					pMagic = QueryMagic(1300 + skillIndex);
// 					if(!pMagic || pMagic->GetMagicLevel() < 80)
// 						return;
// 				}
// 			}
// 
// 			if( prof & PROF_SOLDIER )//սʿ
// 			{
// 				pMagic = QueryMagic(9003);
// 				if(!pMagic || pMagic->GetMagicLevel() < 80)
// 					return;
// 			}
// 		}

		int nNextLev = m_data.GetJobLev(nType) + nLev;
		if(nNextLev > GetLev()+5)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"ְҵ�ȼ����ܳ�������ȼ�5����������������ȼ�");
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
			sprintf(szText, "����%d�����ܴﵽ%d���ſɼ�������ְҵ�ȼ�", (nNextLev-20)/15*2+3, nNextLev-nNextLev%10-10);
			SendSysMsg(szText);
			return;
		}
		CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER_FIRJOB * _EXP_TYPE + GetJobLev(1));
		if (!pLevupexp)
			return;
		__int64 i64LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
		
		if(this->GetExp() < i64LevupExp)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"���ľ��鲻�㣬��������");
			return;
		}

		this->SetAttrib( _USERATTRIB_EXP,this->GetExp() - i64LevupExp,SYNCHRO_TRUE );

		if(nNextLev >= 25 && nNextLev <= 60)
			m_data.AddPot(2,nType);
		else if(nNextLev > 60 )
			m_data.AddPot(3,nType);

		m_data.SetJobLev(m_data.GetJobLev(nType) + nLev,nType);
//		m_data.SaveInfo();
		//goto ��������Ժ�Ͱ�������0������д���ݿ����
		m_MoneyIncr = 0;
		m_ExpIncr = 0;
		CMsgUserAttrib msg;
		msg.Create(GetID(),_USERATTRIB_ADDPOINT,m_data.GetPot(nType));
		msg.Append(_USERATTRIB_FIRJOBLEV,m_data.GetJobLev(nType));
		BroadcastRoomMsg(&msg,true);
//modify code by �ֵ�Ԩ begin 2011.5.24
//		if (m_data.GetJobLev(nType)>20)
//			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) first job level up to %d.", GetID(), GetName(), m_data.GetJobLev(nType));
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) (level up:%d) (first job level:%d) (second job level:%d) (exp:%d)", GetID(), GetName(), GetLev(), m_data.GetJobLev(1), m_data.GetJobLev(2), GetExp());
//modify code by �ֵ�Ԩ end
	}
	else if(nType == 2)
	{
		if((GetProfession(2) & PROF_NEWHAND) != 0 || GetProfession(2) == 0)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"ְҵ��������");
			return;
		}

		int nNextLev = m_data.GetJobLev(nType) + nLev;
		if(nNextLev > GetLev()+5)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"ְҵ�ȼ����ܳ�������ȼ�5����������������ȼ�");
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
			sprintf(szText, "����%d�����ܴﵽ%d���ſɼ�������ְҵ�ȼ�", (nNextLev-20)/15*2+3, nNextLev-nNextLev%10-10);
			SendSysMsg(szText);
			return;
		}
		CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER_SECJOB * _EXP_TYPE +  GetJobLev(2));
		if (!pLevupexp)
			return;
		__int64 i64LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
		
		if(this->GetExp() < i64LevupExp)
		{
			SendSysMsg(_TXTATR_TOPSHOW,"���ľ��鲻�㣬��������");
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
//modify code by �ֵ�Ԩ begin 2011.5.24
//		if (m_data.GetJobLev(nType)>20)
//			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) second job level up to %d.", GetID(), GetName(), m_data.GetJobLev(nType));
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) (level up:%d) (first job level:%d) (second job level:%d) (exp:%d)", GetID(), GetName(), GetLev(), m_data.GetJobLev(1), m_data.GetJobLev(2), GetExp());
//modify code by �ֵ�Ԩ end
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


// 	const int _thisLevel = this->GetLev();//�Ѿ����������¼���
// 	const int _lastLevel = _thisLevel-nLev;//��һ������;

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


	
	
// 	else if( dwPro & PROF_BOWMAN )//����
// 	{
// 		//����
// 		m_data.SetForce( m_data.GetForce() + _thisLevel*_GROW_ARCHER_STR - _lastLevel*_GROW_ARCHER_STR );
// 		//����
// 		m_data.SetDexterity( m_data.GetDexterity() + _thisLevel*_GROW_ARCHER_DEX - _lastLevel*_GROW_ARCHER_DEX );
// 		//����
// 		m_data.SetHealth( m_data.GetHealth() + _thisLevel*_GROW_ARCHER_VIT - _lastLevel*_GROW_ARCHER_VIT );
// 		//����
// 		m_data.SetSoul( m_data.GetSoul() + _thisLevel*_GROW_ARCHER_SOUL - _lastLevel*_GROW_ARCHER_SOUL );
// 	}
// 	else if( dwPro & PROF_ENCHANTER )//��ʦ
// 	{
// 		//����
// 		m_data.SetForce( m_data.GetForce() + _thisLevel*_GROW_MAGE_STR - _lastLevel*_GROW_MAGE_STR );
// 		//����
// 		m_data.SetDexterity( m_data.GetDexterity() + _thisLevel*_GROW_MAGE_DEX - _lastLevel*_GROW_MAGE_DEX );
// 		//����
// 		m_data.SetHealth( m_data.GetHealth() + _thisLevel*_GROW_MAGE_VIT - _lastLevel*_GROW_MAGE_VIT );
// 		//����
// 		m_data.SetSoul( m_data.GetSoul() + _thisLevel*_GROW_MAGE_SOUL - _lastLevel*_GROW_MAGE_SOUL );
// 	}
// 	//�ӵ�
// 	m_data.SetAddPoint( m_data.GetAddPoint()+_ADDITIONALPOINT_NUM * nLev );
// 
// 		
// 	CMsgUserAttrib	msg;
// 	if( msg.Create(GetID(), _USERATTRIB_FORCE, m_data.GetForce())
// 		&& msg.Append(_USERATTRIB_DEX, m_data.GetDexterity())
// 		&& msg.Append(_USERATTRIB_HEALTH, m_data.GetHealth())
// 		&& msg.Append(_USERATTRIB_SOUL, m_data.GetSoul())
// 		&& msg.Append( _USERATTRIB_ADDPOINT, m_data.GetAddPoint() )
// 	//	&& msg.Append( _USERATTRIB_MAXLIFE,this->GetMaxLife() )		//�ͻ����м���
// 		&& msg.Append( _USERATTRIB_MAXSP,this->GetMaxSp() )			//�ͻ���û�м���
// 		)//20070127����:���ͻ���������HP��SP
// 	{
// 		SendMsg( &msg );
// 	}
// 		*/
// 	// 2003.2.9 �����Զ�����Ѫ��
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
// 		this->BroadcastTeamLife(true); //�㲥����Ѫ��
// 		this->BroadcastTeamMana(true); //�㲥����ħ��
// 	}
// 	// save info
// 	m_data.SaveInfo();
// 	
// 
// 	//20070406����:����ʵʱ��UserListͬ��.
// 	//������ŵȼ���ʾ�����ĳЩbug
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
	// �޸ĳ�ʦ���� [�½���10/26/2007]
//	int tutorId = GetTutor()->GetTutorID();
//	CUser *pUser = UserManager()->GetUser(tutorId);
//	if(GetLev() >= 40 && GetTutor() != NULL && pUser->GetLev() >=65)//ͽ��40��������ʦ���ȼ���65������
//		Graduate();
    //----------------------------------------------------------------------------
	//20070711����:��������½�ɫ��
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
				//ɾ�����ֱ�̬��������
				bool bHasKind = false;
				DelItemByTypeMulti(144000,0,bHasKind, false, true);
				CItem* pItem = GetEquipItemByPos(ITEMPOSITION_WEAPONR);
				if(pItem && pItem->GetTypeID() == 144000)
				{
					EraseEquip(ITEMPOSITION_WEAPONR,true);
				}
			}
			DEBUG_CATCH("ɾ����̬����");
		}

		if(m_bSee)
		{
			pMap->LeaveRoom(QueryMapThing(), this->IsAlive());	//WITH_BLOCK);
			//goto ���µ�ͼ�������
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
	TutorLoginHint();//ʦͽ����
	return true;
}

//////////////////////////////////////////////////////////////////////
// fight
//////////////////////////////////////////////////////////////////////
bool CUser::SetAttackTarget(IRole* pTarget /*= NULL*/)
{
	//20070316����:�ѹ����ɹ��жϷŵ�IsAktable()ͳһ����

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

	//�ȿ���buff
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


	ATK_TYPE type = ATKTYPE_PHYSICAL_COMMON;//������ͨ����
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
//modify code by �ֵ�Ԩ begin 2011.6.22
			IStatus* pStatus = pTarget->QueryStatus(STATUS_RETORT_LGWS);
			if(pStatus)
			{
				this->AddAttrib(_USERATTRIB_LIFE, -1*nLoseLife, SYNCHRO_TRUE);
				this->BeAttack(false,pTarget,nLoseLife,0,true);
				if( !this->IsAlive() && this->QueryMagic() )
					pTarget->Kill(this, INTERACT_ATTACK);
			}
//modify code by �ֵ�Ԩ end
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
//06.3.6�����޸�
//�˺���Ϊ��ұ������Ĵ���
bool CUser::BeAttack(bool bMagic, IRole* pTarget, int nLifeLost,int addi/* = 0*/, bool bReflectEnable/*=true*/)
{
	CHECKF(pTarget);

	if (QueryStatus(STATUS_LURK))
		CRole::DetachStatus( QueryRole(),STATUS_LURK );		//���Ǳ��״̬
	if (QueryStatus(STATUS_SLEEP))
		CRole::DetachStatus( QueryRole(),STATUS_SLEEP );	//���ս������״̬

	StopMine();

	if (GetSp() < GetMaxSp())
	{
		this->AddAttrib( _USERATTRIB_SP,1,SYNCHRO_TRUE );
	}


	CUser *pUser = NULL;
	if( pTarget->QueryObj(OBJ_USER, IPP_OF( pUser ) ) )
	{
		if( !bMagic && !pUser->IsBowman()) //�ǽ�ս������.....
		{
			IStatus* pStatus = QueryStatus( STATUS_RETORT ); //����
			if (pStatus)			
				CRole::AttachStatus(pUser->QueryRole(), STATUS_SLEEP,0, pStatus->GetPower());
			pStatus = QueryStatus( STATUS_FROSTSHIELD );	//��˪��
			if( pStatus )
			{
				CRole::AttachStatus( pUser->QueryRole(),STATUS_MOVESPEEDDOWN_PERCENT,FROSTSHIELD_DECSPEEDPERCENT,FROSTSHIELD_DECSPEEDTIME );
			}
			// �������� �����˺� [1/31/2008 �½���]------------------------------------------------------------
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

	//goto ??����Ҽӳ��??�����
// 	CMonster* pKillerMonster= NULL;
// 	if(pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pKillerMonster)))
// 	{
// 		if(pKillerMonster->IsEudemon())
// 		{
// 			AddEnmity(pKillerMonster->GetID(),
// 				nLifeLost);
// 		}
// 	}
	
	//20070209����:��ҳ�ޱ�
	//ֻ��¼����˺�
	//����ʱ���
	if(pUser)//ǰ���Ѿ�ȡ�ù�.�������������Ҷ���¼
	{
		//20080104����:�Ժ�����װ����
		//goto ??����Ҽӳ��??�����
// 		AddEnmity(pUser->GetID(),
// 			nLifeLost);

//		DWORD curTime = ::TimeGet();
//		
//		int amount = m_vEnmity.size();
//		for(int i = 0;i< amount;i++)		//�ڳ���б����������˺��������ˣ��ۼ�
//		{
//			if(m_vEnmity[i].userID == pTarget->GetID())
//			{
//				m_vEnmity[i].enmity += nLifeLost;
//				m_vEnmity[i].lastUpdateTime = curTime;//ʱ���
//				break;
//			}	
//		}
//		if(i == amount)		//һ���µĵ��ˣ����
//		{
//			ENMITY enmity;
//			enmity.userID = pTarget->GetID();
//			enmity.enmity = nLifeLost;
//			enmity.lastUpdateTime = curTime;//ʱ���
//			m_vEnmity.push_back(enmity);
//#ifdef _DEBUG
//			//���ǵ�size��С
//			const int _warningSize = 20;
//			int enmitySize = m_vEnmity.size();
//			if(enmitySize > _warningSize)
//			{
//				LOGERROR( "��ҳ�ޱ�������,��������:%s enmitySize:%d",
//					this->GetName(),enmitySize );
//			}
//#endif
//		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
//  [7/10/2007] ���񲨵�һ���޸�
//	���Ӳ�����DWORD dwBattleAddInfo��ս��������Ϣ
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
//  [7/10/2007]------------end-----����ע��--------------------------------------------
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
	// 2003.2.13 ����Է�����Ҷ�������ˡ����������ȥѪֵҪ�ڶ��ڹ㲥��
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
	// 2006.12.08 ����Է�����Ҷ�������ˡ����������ȥѪֵҪ�ڶ��ڹ㲥��
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
	//[2009.08.13]�м��ʲ����;�
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
//				int nFundLost = ::MulDiv(nExpLost, nDecPercent, 100*MONEYCOST_PEREXP);	// 100�㾭��1��
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
//				int nFundLost = ::MulDiv(nExpLost, nDecPercent, 100*MONEYCOST_PEREXP);	// 100�㾭��1��
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
	//����������
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
	
	
	///������ɱ�¼�
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
			//m_KillMonsterNum++; //��ʼ��ʱ��ͬʱɱ��������1
// 		}
// 		else
// 		{
			
// 			if(!isQuestion &&
// 				IsInMapInstance()==false &&///���ڸ����в�����
// 				GetAutoFightFlag()==false)///�����Զ��һ��в�����
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
// 	   sprintf(szWords," ʱ����: %u ��Ҫɱ%dֻ�� Ҫɱ%d��",time(NULL) - m_dwAntiplugInterval,GetKillMonsterTargetNum()-m_KillMonsterNum,GetKillMonsterTargetNum());
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
		if (this->IsUser() && pTarget->IsUser())//���Ŀ��Ϊ���
		{
			//CMsgTalk Tmsg;
			//this->m_KillAccountId = pTarget->GetID();//  [4/11/2007] ���� ��¼ɱ�������ID
			DEBUG_TRY
				ProcessPk(pTargetUser);
			DEBUG_CATCH("ProcessPk(pTargetUser);");
			
			DEBUG_TRY
				pTarget->SendSysMsg("���Ѿ������<font color = '#ff0000'>[%s]</font>ɱ���ˣ��Է��Զ���ӵ���ĳ����б���",this->GetName());
			sprintf(szTalk, "<font color = '#ff0000'>���Ѿ������[%s]ɱ���ˣ��Է��Զ���ӵ���ĳ����б���, �����ͨ�����Ѳ˵��еĳ����б����鿴������Ϣ��</font>",this->GetName());
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
				pTarget->SendSysMsg("���Ѿ������<font color = '#ff0000'[%s]</font>�ĳ���<font color = '#ff0000'>{%s}</font>ɱ���ˣ��Է��Զ���ӵ���ĳ����б���",pMonsterTemp->QueryOwnerUser()->GetName(), pMonsterTemp->GetName());
				sprintf(szTalk, "<font color='#ff0000'>���Ѿ������<font color = '#ff0000'>[%s]</font>ɱ���ˣ��Է��Զ���ӵ���ĳ����б���, �����ͨ�����Ѳ˵��еĳ����б����鿴������Ϣ��</font>",this->GetName());
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
//	this->m_KillAccountId = pTarget->GetID();//  [4/11/2007] ���� ��¼ɱ�������ID
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
// 		else if (pTargetUser->GetMapID() == MAP_PRISON)//������ɱ
// 		{
// 			if (pTargetUser->GetPk() > 0)
// 			{
// 				pTargetUser->FlyMap(MAP_PRISON, 93, 94);
// 			}
// 			else
// 			{
// 				//ȥ���˼���
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
				int tempPK = this->AddPk(PKVALUE_ONEKILL * iAdd);//ɱ����������PKֵ

				DEBUG_TRY
				if (!(pTargetUser->GetFriend(GetID()) && pTargetUser->GetFriend(GetID())->GetFriendType()==ENEMY_GROUP))
				{
					if (pTargetUser->AddFriend(GetID(), GetName(), ENEMY_GROUP, GetMate()))//��������б�
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
				DEBUG_CATCH("processPk��������б����");
// 				CMsgUserAttrib msgUser;
// 				if(msgUser.Create(this->GetID(), _USERATTRIB_PK,this->GetPk()))
// 					BroadcastRoomMsg(&msgUser, INCLUDE_SELF);
				if (6 == tempPK)
				{
//					CMsgTalk msg;
//					if (msg.Create("ϵͳ", this->GetName(), "<font color='#ff0000'>���Ѿ�ɱ��̬�ˣ�����ɱ�˽��ᱻͶ�������</font>", NULL, 0xfefeefef, _TXTATR_CHATWITH))
//					this->SendMsg(&msg);
//					this->SendSysMsg("���Ѿ�ɱ��̬�ˣ�����ɱ�˽��ᱻͶ�������");
					this->SendEmail(GetName(), "<font color='#ff0000'>���Ѿ�ɱ��̬�ˣ�����ɱ�˽��ᱻͶ�������</font>", this);
				}
				else	if(8 == tempPK)
				{
					this->GotoJail(false);
				}
					//���ͼ�����ش���
			//����������г�ͻ
			//��AddPk�ﴦ��
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
//  					if (msg.Create("ϵͳ", this->GetName(), "<font color='#ff0000'>���Ѿ�ɱ��̬�ˣ�����ɱ�˽��ᱻͶ�������</font>", NULL, 0xfefeefef, _TXTATR_MAYBEMAIL))
//  					this->QueryOwnerUser()->SendMsg(&msg);
						this->SendEmail(this->GetName(), "<font color='#ff0000'>���Ѿ�ɱ��̬�ˣ�����ɱ�˽��ᱻͶ�������</font>", this);
					}
					if(8 == tempPK)//ɱ����������PKֵ
					{
						if (this->GetMapID() != MAP_PRISON)
						{
//						this->QueryOwnerUser()->SetRecordPos(MAP_PRISON, 93, 94, true);
//						//�Ƿ�ȥ��������
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
				}	//���ͼ�����ش���
				DEBUG_CATCH2("ProcessPK step = %d", nStep);
			}
		}
		else if(pTargetUser->IsRedName())
		{
			DEBUG_TRY
			nStep = 3;
			//��BEKILL�Ƶ��������ɱ������װ��
			//	pTargetUser->QueryPackage()->RandDropItem(1, true);//�������һ���ǰ���Ʒ
	
			
			/*****************�µĺ����ͷ�����*******************************************/		
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
				sprintf(szDropitem, "%d����Ʒ", nDropItem);
			if(nDropEqu)
				sprintf(szDropEqu, "%d��װ��", nDropEqu);
			if (nDropItem || nDropEqu)
			{
				sprintf(szMsg, "�㶪ʧ��%s%s", szDropitem, szDropEqu);
				pTargetUser->SendSysMsg(szMsg);
				pTargetUser->SendEmail(pTargetUser->GetName(), szMsg, pTargetUser);
//				sprintf(szTalk, "<font color='#ff0000'>�㱻���%sɱ���ˣ�����ɱ��ֵ�ĳͷ���%s</font>", this->GetName(), szMsg);
//				if (msg.Create("ϵͳ", this->GetName(), szTalk, NULL, 0xff0000, _TXTATR_CHATWITH))
//					pTargetUser->QueryOwnerUser()->SendMsg(&msg);
			}
			/****************************************************************************************/							
			
//			pTargetUser->QueryPackage()->RandDropItem(1, true);//�������һ���ǰ���Ʒ
			nStep = 5;
			if (!(pTargetUser->GetFriend(GetID()) && pTargetUser->GetFriend(GetID())->GetFriendType()==ENEMY_GROUP))
			{
				if (pTargetUser->AddFriend(GetID(), GetName(), ENEMY_GROUP, GetMate()))//��������б�
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
			// 		pTargetUser->AddPk(-PKVALUE_ONEKILL);//������ɱ����PKֵ
			// 		__int64	nLevExp = 0;	// �������辭��
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
			// 			int nLossExpPercent = 1;//��������ʧ�����1%
			// 			__int64 nExpGet = (nLevExp) * nLossExpPercent /100;
			// 			if (nExpGet > 0 && (this->GetExp() >nExpGet))
			// //				this->AwardBattleExp(-nExpGet);
			// 				this->AddAttrib(_USERATTRIB_EXP, -nExpGet, SYNCHRO_TRUE);
			// //				this->SendSysMsg("��Ϊ����ɱ�֣�������ľ��������1%");
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
// 				//	&& !g_ResourceWar.IsWarPkTime( GetMapID() )			//07.5.12����ע�ͣ������Դսʱɱ�Լ�������Ҳ�������BUG
// 				//&& pTargetUser->GetPhyle() == this->GetPhyle()
// 				//&& !pTargetUser->IsEvil()
// 				&& !pMap->IsPkFieldNoPkValue()	//  [2007-9-13] ��� ��MAPTYPE_PKFIELD_NO_PK_VALUE���Եĵ�ͼPK����PKֵ
// 				)


		}
		//[��;���� 2009.06.03]�˻ᱻԩ��,�����һ�γ�ɱֵ,�����ټ�һ��.���ε�����.
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

/*			// �°���û�м��������ʧ --- zlong 2004.10.28
			int nManaLost = pTargetUser->GetMana()/2;
			pTargetUser->AddAttrib(_USERATTRIB_MANA, -1*nManaLost, SYNCHRO_TRUE);		// , &dwDirty
*/

			// pk increase    // tudo 2004.1.11   // û��
//			if (!pTargetUser->IsEvil() 
//					&& pTargetUser->GetPk() < PKVALUE_REDNAME
//						&& !pMap->IsDeadIsland())	// ������ɱ�˲�����������PKֵ
						// && pTargetUser->GetExp() >= 0)	// ʥս��ȡ������趨

			// pk increase	���� PK����������Ҫ��PKֵ -- zlong 2005-04-28
			//20070228����:PKֵ���ӷ�ʽ�޸�
//			if (!pMap->IsDeadIsland()
//			//	&& !g_ResourceWar.IsWarPkTime( GetMapID() )			//07.5.12����ע�ͣ������Դսʱɱ�Լ�������Ҳ�������BUG
//			/*	&& pTargetUser->GetPhyle() == this->GetPhyle()*/
//				&& !pTargetUser->IsEvil()
//				&& !pMap->IsPkFieldNoPkValue()	//  [2007-9-13] ��� ��MAPTYPE_PKFIELD_NO_PK_VALUE���Եĵ�ͼPK����PKֵ
//				)
//			{
//				if(!(thisSyn && userSyn && thisSyn->GetState() == 2 && userSyn->GetState() == 2 && thisSyn->GetWarSynID() == userSyn->GetID()))
//					this->AddPk(PKVALUE_ONEKILL);
//				//����������г�ͻ
//				//��AddPk�ﴦ��
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
//						this->AddPk(60-GetPk());//PK��Ϊ60
//				}

				// syn effect
			//	if (this->IsHostileSynMember(pTargetUser))
			//		nPkValue /= 2;

				// crime
// 				if (!QueryStatus(STATUS_CRIME))
// 				{
// 					CONST CRIME_TIME_PERMURDER = 20;	//2*60;		// ����ʱ���Ϊ30��
// 					this->SetCrimeStatus(STATUS_CRIME, CRIME_TIME_PERMURDER);
// 				}

				// ��ʾ ���� zlong 2005-04-28
			/*	if (this->GetPk() < PKVALUE_REDNAME)
					this->SendSysMsg(STR_INCPK_REDNAME_PUNISH);
				else if (this->GetPk() < PKVALUE_BLACKNAME)
					this->SendSysMsg(STR_INCPK_BLACKNAME_PUNISH);
				else
					this->SendSysMsg(STR_INCPK_BLACKNAME_PUNISH2);*/
//			}

			// ��������
		/*	{
				// ����2004.10.28�İ������� ---- zlong
				__int64	nLevExp = 0;	// �������辭��
				int		nStuExp	= 0;	// ������߾���
				CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + pTargetUser->GetLev());
				if (!pLevupexp)
					pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + __max(0, pTargetUser->GetLev()-1));

				if (pLevupexp)
				{
					nLevExp = pLevupexp->GetInt(LEVUPEXPDATA_EXP);
					nStuExp	= pLevupexp->GetInt(LEVUPEXPDATA_STU_EXP);	//nStuExp������Ϊ��ĸ������Ϊ0
				}
				pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + this->GetLev());
				if (!pLevupexp)
					pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + __max(0, this->GetLev()-1));

				int nKillerStuExp	= 0;
				if (pLevupexp)
					nKillerStuExp	= pLevupexp->GetInt(LEVUPEXPDATA_STU_EXP);

				int nLossExpPercent = 0;
				if (pTargetUser->GetPk() < PKVALUE_REDNAME)					// ����
					nLossExpPercent	= 1;									// ��ʧ��������1%����
				else if (pTargetUser->GetPk() < PKVALUE_BLACKNAME)			// ����
					nLossExpPercent	= 2;									// ��ʧ��������2%����
				else														// ����
					nLossExpPercent	= 3;									// ��ʧ��������3%����

				__int64 nExpGet = (nLevExp) * nLossExpPercent * 70 / 10000;			// ɱ�˵õ��Է���ʧ�����70%
				// ����Ĺ�ʽ�޸���, 
				// ɱ�˵��˵õ�����=����ɱ����ʧ����/��ɱ�˱���1�οɵ���߾��飩*ɱ�˵��˱���1�οɵ���߾���*70%
				if (nStuExp == 0)
					nExpGet	= 0;
				else
					nExpGet	= nExpGet * nKillerStuExp / nStuExp;
				
				if (nExpGet > 0)
					this->AwardBattleExp(nExpGet);
	//������ʱע��..
	
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
//				__int64 i64CurExp = pTargetUser->GetExp();	//  [11/14/2007 ���]
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
				//20070320����:����.���뱾������.�����಻��
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
//						//20070320����:������һ��Ϊ0��int.ת��char *һ���Ƿ�.
//						//team Hunter���ܲ�ʹ��.ֱ������.
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
//modify code by �ֵ�Ԩ begin 2011.6.22
	EudemonDead();
	if(m_idSoulEudemon)//������︽��
		RemoveEudemonSoul();
//modify code by �ֵ�Ԩ end
	this->SetStatus(STATUS_DIE,true);

	CMsgInteract msg;
	if(msg.Create( INTERACT_KILL,this->GetID(),this->GetID(),this->GetPosX(),this->GetPosY(),DIE_NORMAL,0 ) )
		this->BroadcastRoomMsg(&msg, true,EM_SENDNPCFLAG_TRUE);

//	UpdateCalculatedProperty(UPDATECALTYPE_ALL);
	ClrAttackTarget();
	if (QueryMagic())
		QueryMagic()->AbortMagic(true);
//modify code by �ֵ�Ԩ begin 2011.6.22
	OBJID objmapid = this->GetRecordMapID();
	OBJID objx = this->GetRecordPosX();
	OBJID objy = this->GetRecordPosY();
//modify code by �ֵ�Ԩ end
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
		//[��;���� 2009.05.08]���������
		idRebornMap = 1001;
		pos.x = 78;
		pos.y = 87;
		this->SetRecordPos(idRebornMap, pos.x, pos.y);
	}
//modify code by �ֵ�Ԩ begin 2011.6.22
	if( pMap->GetDocID() == 2016 || pMap->GetDocID() == 2018 )//���鵺��ɱ���ؿ���
		this->SetRecordPos(objmapid, objx, objy);
//modify code by �ֵ�Ԩ end
	pMap->DecRole(this->GetPosX(), this->GetPosY());
}
//////////////////////////////////////////////////////////////////////
void CUser::BeKill(IRole* pRole /*= NULL*/)
{
 	EudemonDead();
	if(m_idSoulEudemon)//������︽��
		RemoveEudemonSoul();

	CGameMap* pMap = this->GetMap();
	CHECK(pMap);

	if(!pMap->IsPkGameMap())
	{
		//���������10��
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
// 		CMsgPlayer msgPlayer;// ������Ϣ���ͻ��˸��� [�½���8/25/2007]
// 		if (msgPlayer.Create(this->QueryRole()))
// 			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		
		SetAttrib(_USERATTRIB_SPEED, AdjustSpeed(GetSpeed()), SYNCHRO_TRUE);
	}
	if (GetItemByType(_SPCITEM_MONEYFORSHOP))//��������̣��򸴻��10�����ڲ��ܼ�����
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
	ClrAttackTarget();					//�����������Ŀ��
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
	if( this->IsEvil() && (this->GetMapID() == MAP_PRISON) )		//�����ͼ���߼���..�������ݵ�ͼ�����㸴��,
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
	// ��Ϊ����WITH_BLOCK���أ�Ϊ����������ʬ������͸����Ҫ����ͼ�ϵĽ�ɫ��
	pMap->DecRole(this->GetPosX(), this->GetPosY());

// 	if( IsOwnMap( pMap->GetID() ) || this->GetMapID() == MAP_PRISON )		//�����ͼ���߼���..�������ݵ�ͼ�����㸴��,
// 	{
// 		FlyMap(MAP_PRISON, 93,93);
// 	}
	return;

// 	if (pMap->IsPrisonMap())	//�ڼ�����ͼ����,û�гͷ�(��������,������Ǯ,������Ʒ)
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
	//1.�Ӿ���
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
			SendSysMsg(_TXTATR_TOPSHOW,"���ľ������������ֶ�����");
		return 0;
	}
	
	//2.11��ǰ��������
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
	if (!pTarget)//Ⱥ�����ܿ���û��Ŀ��
	{
// 		if (bSendHint)
// 			this->SendSysMsg(/*g_objStrRes.GetStr( 15037 )*/"����Ķ���:Ŀ��Ϊ��");//"����Ķ���:Ŀ��Ϊ��"
		return false;
	}

	//20070315����:��IsPkEnable()�е��ж��ù���
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
			if (pEudemon->GetType() == 9999)//���������ô���ģ�
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
// 			this->SendSysMsg(g_objStrRes.GetStr( 15045));//"����Ķ���:�㲻�ܹ����Լ�"
		return false;
	}
//����ΪPK�ж�
	if(pTarget->IsUser())		
	{		
		if(this->IsPVPStuts() && this->GetPVPTargetID() == pTarget->GetID()) 
			return true;
//		if ((this->GetMapID() == 1001) || (this->GetMapID() == 1006) ||(this->GetMapID() == MAP_PRISON))
		if (!this->GetMap())
			return false;
		if(this->GetMap()->isDisablePkill())
		{
//			this->SendSysMsg("���������͵����ؽ������");//STR_NO_PK_VILLAGE
			return false;
		}
		//1)��ӳ�Ա���ܱ����� 
		CTeam* tTeam = GetTeam();
		if(tTeam)
		{
			if(tTeam->IsTeamMember(pTarget->GetID()))
			{
			//	this->SendSysMsg("�㲻�ܹ�����Ķ���");
				return false;
			}
		}
		
		if (!this->GetMap())
		{
			return false;
		}
		if (this->GetMap()->IsPkGameMap())
		{
			//������
			IStatus* pStatus = QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);
			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
			{
				if(WargameManager()->GetCurrentStatus(pStatus->GetPower()) != WARGAME_STATUS_WORK)
					return false;

				if(pStatus->GetParam1() == pStatusTarget->GetParam1())//ͬ��Ӫ
					return false;
			}

			return true;
		}
		//2) ͬ�˻���ͬ���ɹ���
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
		
		
		
		//3)��������
		if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
		{	
#ifdef _DEBUG
			char szName[128];
			strcpy(szName,pUser->GetName());
#endif // _DEBUG
			
// 			int nDoMainID = this->GetCityWarDoMain();  //��սʱû��PK����
// 			if(nDoMainID && nDoMainID == pUser->GetCityWarDoMain())
// 			{
// 			}
// 			else if(0 != this->GetSynWarJoinID() && 0 != pUser->GetSynWarJoinID())//��սʱû��PK����
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
				else	//4)��������
				{
					if(this->IsWhileProcAble())
						return false;	
				}
			}
		}
//20��������Ҳ��ܱ�����
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
		// 			this->SendSysMsg(g_objStrRes.GetStr(28043));//("��ӵ�г�ս����״̬�����ܹ�����ͨ���");
		// 			return false;
		// 		}
	}

	
	// ��ʱ�������ﲻ�ܽ��й��� [�½���8/17/2007]
//	if( CUser::GetEquipItemByPos( ITEMPOSITION_MOUNT ) )
//	{
//		CItem* pItem = GetEquipItemByPos(ITEMPOSITION_MOUNT);
//		if(pItem != NULL)
//		{
//			int nMountID = pItem->GetTypeID();
//			CItemTypeData* pType = ItemType()->QueryItemType(nMountID);
//			if (pType != NULL && pType->GetInt(ITEMTYPEDATA_LUCK) == 1)
//			{
//				this->SendSysMsg(g_objStrRes.GetStr( 16020));//"�ù�����ʱ������!"
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
// 			this->SendSysMsg("�Է����ڳ�ս���밲ȫʱ��");
// 			return false;
// 		}
// 		int nDoMainID = this->GetCityWarDoMain();  //��սʱû�����ֱ���
// 		if(nDoMainID && nDoMainID == cityUser->GetCityWarDoMain())
// 		{
// 		}
// 		else if(SYNWARTYPE_NONE != GetSynWarJoinID() && SYNWARTYPE_NONE != cityUser->GetSynWarJoinID())
// 		{
// 		}
// 		else //������20����,���ɹ������
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
// 			//�������������,20������,�����Ա�����
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
	//Ŀ�������,��Ŀ���ڰ�ȫ��....��....�Լ��ڰ�ȫ��....���ֹPK��ͼ.


	//��ȫ������ satan******************************
// 	if(	((pTarget->IsUser() && pTarget->GetMap()->IsInRegionType(REGION_PK_PROTECTED, pTarget->GetPosX(), pTarget->GetPosY())) ||
// 		 ((!pTarget->IsEvil()) && pTarget->GetMap()->IsPkDisable())) // �½����޸�,�ж��ڰ�ȫ��ͼ����PK���� [6/11/2007]
// 		/*|| GetMap()->IsInRegionType(REGION_PK_PROTECTED, GetPosX(), GetPosY())*/)//  [5/16/2007]�½���ע�ͣ��жϰ�ȫ�����ڿ��Դ�֣�������PK
// 	{
// 		if (bSendHint)
// 			this->SendSysMsg(g_objStrRes.GetStr( 15041));//"���������:���Ŀ���������ȫ��"
// 		return false;
// 	}
	//  [5/12/2007] ��̯�����ж�
// 	int posX = this->GetPosX();
// 	int posY = this->GetPosY();
// 	CGameMap* pMap = MapManager()->QueryMap(this->GetMapID());	
// 	if(pMap->IsMarketRegion(posX,posY))
// 	{
// 		LOGERROR( "MsgAction::case actionCreateBooth����,��̯����Ƿ�����!.�û�:%s mapID:%d x:%d y:%d",
// 			this->GetName(),
// 			this->GetMapID(),
// 			posX,posY );
// 		this->SendSysMsg(g_objStrRes.GetStr( 15042));//"�㲻Ӧ�ÿ����������.�����Ѿ���¼.��̯����Ƿ�����!"
// 		return false;
/*	}*/
	//  [5/12/2007] ��̯�����ж�
	if (this && !this->IsAlive())
	{
//		if (bSendHint)
//			this->SendSysMsg(g_objStrRes.GetStr( 15043));//"�����״̬:���Ѿ�����"
		return false;
	}

	if (pTarget && !pTarget->IsAlive())
	{
	//	if (bSendHint)
//		this->SendSysMsg("����Ϊ�󣬽�ֹ��ʬ��");
 //		if (bSendHint)
 //			this->SendSysMsg(g_objStrRes.GetStr( 15044));//"����Ķ���:Ŀ���Ѿ�����"
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
				this->SendSysMsg("�����Ŀ��");
			return  false;
		}

//		else if(pMonster->IsGuard())
//		{
//			if (bSendHint)
//				this->SendSysMsg("�㲻�ܹ�������");
//			return  false;
//		}

// 		if(strstr(pMonster->GetName(),"����") != 0)//"����"
// 		{
// 			if (bSendHint)
// 				this->SendSysMsg(g_objStrRes.GetStr( 15049));//"����Ķ���:���ܹ�����������"
// 			return  false;
// 		}
// 
// 		if(GetCityWarDoMain() && CITYSYNWAR_CITYWAR == GetCitySynWarType())
// 		{
// 			if(IsCityWarAttack(GetCityWarDoMain()) && pMonster->IsCityAttack())
// 			{
// 				if (bSendHint)
// 			    	this->SendSysMsg("���ܹ�����������");
// 				return false;
// 			}
// 			else if(IsCityWarDefence(GetCityWarDoMain()) && pMonster->IsCityDefence())
// 			{
// 				if (bSendHint)
// 			    	this->SendSysMsg("���ܹ�����������");
// 				return false;
// 			}
// 		}
// // 		
// 
// 		IStatus* pStatus = QueryStatus(STATUS_CITYWAR_WEAPON);
// 		if( pStatus && !pMonster->IsCityWarStrategicTarget())
// 		{
// 			this->SendSysMsg(g_objStrRes.GetStr(28044));//("��ӵ�г�ս����״̬��ֻ�ܹ�����սս��Ŀ��");
// 			return false;
// 		}
// 
// 		if(0 != GetSynWarJoinID() && CITYSYNWAR_SYNWAR == GetCitySynWarType())
// 		{
// 			if((SYNWARTYPE_ATTACK == GetSynWarJoinType() && pMonster->IsCityAttack())
// 				|| (SYNWARTYPE_DEFENCE == GetSynWarJoinType() && pMonster->IsCityDefence()))
// 			{
// 				if (bSendHint)
// 		    		SendSysMsg("���ܹ�����������");
// 				return false;
// 			}
// 		}
			
//			if(this->GetPhyle() == PROF_DRAGON && strstr(pMonster->GetName(),g_objStrRes.GetStr( 15046)) != 0)//"����"
//			{
//				if (bSendHint)
//					this->SendSysMsg(g_objStrRes.GetStr( 15049));//"����Ķ���:���ܹ�����������"
//				return  false;
//			}
//			else if(this->GetPhyle() == PROF_HUMAN && strstr(pMonster->GetName(),g_objStrRes.GetStr( 15047)) != 0)//"������"
//			{
//				if (bSendHint)
//					this->SendSysMsg(g_objStrRes.GetStr( 15049));
//				return  false;
//			}
//			else if(this->GetPhyle() == PROF_GENE && strstr(pMonster->GetName(),g_objStrRes.GetStr( 15048)) != 0)//"������"
//			{
//				if (bSendHint)
//					this->SendSysMsg(g_objStrRes.GetStr( 15049));
//				return  false;
//			}
			
			//  [2007-8-31] ��� �Ƿ�Ϊͬһս���
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
// 				//	0002215: ��������б߾�սBUFF����ҹ�������ľ��죬����û�вμӱ߾�ս����ҾͲ��ܹ������� 	
// 				if(pMonster->GetType() == MONSTER_TYPE_DRAGON_FLAG 
// 					|| pMonster->GetType() == MONSTER_TYPE_HUMAN_FLAG 
// 					|| pMonster->GetType() == MONSTER_TYPE_GENE_FLAG )
// 				{
// 					this->SendSysMsg(g_objStrRes.GetStr(16037));
// 					return false;
// 				}  
// 			}

			//  [2007-9-22] ���	�������޵�BUF
			IStatus* pStatus3 = pTarget->QueryStatus(STATUS_IMMUNE_ALL);
			if(pStatus3 != NULL)
				return false;
			return true;
		}
//		if (pMonster->IsMonster() || pMonster->IsSynPet())//��������,���⹥��
		//07.5.12����ע������һ��,����Ҫ�жϣ����return true,ɱ����������ػ�����������ֱ�ӿ��Թ���
// 		return true;
// 	}


	//20070404����:�ᵽǰ����
	if(!pTarget->IsBeAtkable())
	{
		if (bSendHint)
			this->SendSysMsg("Ŀ�����޵е�");//"����Ķ���:���󲻿ɱ�����"
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
			//�κ�ģʽ�²μӻ�����֮����ܹ���
			IStatus* pKIllWarStatus1=QueryStatus(STATUS_WARGAME);///ɱ�ֻbuffer
			IStatus* pKIllWarStatus2=pTargetUser->QueryStatus(STATUS_WARGAME);///ɱ�ֻbuffer
			if ((pKIllWarStatus1 && !pKIllWarStatus2) || (!pKIllWarStatus1  && pKIllWarStatus2))
			{
				this->SendSysMsg(g_objStrRes.GetStr(16038));	//  [2007-9-3] ���
				return false;
			}
			///��Ȼ˫�����μ��˻,���μӻ�����Ͳ�һ��Ҳ���ܹ���
			if (pKIllWarStatus1 && pKIllWarStatus1 && pKIllWarStatus1->GetPower()!=pKIllWarStatus2->GetPower())
			{
				this->SendSysMsg(g_objStrRes.GetStr(16039));	//  [2007-9-3] ���
				return false;
			}

			//  [2007-9-28] ��� ��в��ܹ�������ͬ��
//			if(pKIllWarStatus1 && pKIllWarStatus2 && (pKIllWarStatus1->GetPower() == pKIllWarStatus2->GetPower()))
//			{
//				if(this->GetPhyle() == pTargetUser->GetPhyle())
//				{
//					this->SendSysMsg(g_objStrRes.GetStr(16040));
//					return false;
//				}
//				else	//  [11/9/2007 ���]	0002205: �ڱ߾�ս��.�б߾�ս�£գƣƵ����ֻҪ��һ���������Ӱ�ť.�Ͳ����໥����. 
//					return true;
//			}


			//�κ�ģʽ�¶����Թ�������,�������
// 			if(pTargetUser->IsEvil())
// 			{
// 				return true;
// 			}

			///�����ͼ��������ͼ����-softworms-2007-08-11-����ܻ���
			//if(pTarget->GetMap()->GetOwnerType()==OWNER_RACE ///�ܺ������ڵ�ͼΪ����ӵ�õ�ͼ
   			/*	&& pTarget->GetMap()->GetOwnerID()== pTargetUser->GetPhyle()*////�ܺ����ڱ����ͼ
			//	&& pTargetUser->QueryStatus(STATUS_ENEMYFIRE)==NULL ///�ܻ������ϴ���"���ӵ�buff" 
			/*	&& pTargetUser->GetPhyle()!=GetPhyle()*////˫������ͬ����
			//	)///�ܻ������Լ��������ڵĵ�ͼ
			//	return false;

			///�����ͼ��������ͼ����-softworms-2007-08-11-��Թ�����
			//if(GetMap()->GetOwnerType()==OWNER_RACE ///���������ڵ�ͼΪ����ӵ�õ�ͼ
			/*	&& GetMap()->GetOwnerID()== GetPhyle()*////�������ڱ����ͼ
			//	&& QueryStatus(STATUS_ENEMYFIRE)==NULL ///���������ϴ���"���ӵ�buff" 
			/*	&& pTargetUser->GetPhyle()!=GetPhyle()*////˫������ͬ����
			//	)///�ܻ������Լ��������ڵĵ�ͼ
			//{
			//	SendSysMsg(g_objStrRes.GetStr( 15051));//"��������ͷ���Ե�'PVP'��ť���������������PK!"
			//	return false;
			//}

			//�κ�ģʽ�¶����Թ����������,
//			if(this->GetPhyle() != pTargetUser->GetPhyle())
//				return true;
		}
	}
	
//     //1)��ӳ�Ա���ܱ����� 
//     CTeam* tTeam = GetTeam();
// 	if(tTeam)
// 	{
// 		if(tTeam->IsTeamMember(pTarget->GetID()))
// 		{
// 			SendSysMsg("�㲻�ܹ�����Ķ���");
// 			return false;
// 		}
// 	}
// 
// 	//2) ͬ�˻���ͬ���ɹ���
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
// 									  this->SendSysMsg("���ܹ��������Ա");//"����Ķ���:�ڵ�ǰPKģʽ��,���ܹ�ͬ��"
// 									  return false;
// 								  }
// 						  }
// 					  }
// 					  else
// 					  {
// 						  if (bSendHint)
// 							  this->SendSysMsg(g_objStrRes.GetStr( 15053));//"����Ķ���:�ڵ�ǰPKģʽ��,���ܹ�ͬ��"
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
//       //3)��������
// 	  if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
// 	  {	
// #ifdef _DEBUG
// 		  char szName[128];
// 		  strcpy(szName,pUser->GetName());
// #endif // _DEBUG
// 
// 		  int nDoMainID = this->GetCityWarDoMain();  //��սʱû��PK����
// 		  if(nDoMainID && nDoMainID == pUser->GetCityWarDoMain())
// 		  {
// 		  }
// 		  else if(0 != this->GetSynWarJoinID() && 0 != pUser->GetSynWarJoinID())//��սʱû��PK����
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
// 			  else	//4)��������
// 			  {
// 				  if(IsWhileProcAble())
// 					  return false;	
// 			  }
// 		  }
// 	  }

    


// 	switch(this->GetPkMode())
// 	{
// 	case	PKMODE_FREE://ȫ
// 		//return true;
// 		break;
// 
// 	case	PKMODE_TEAM://��
// 		{
// 			LOGERROR( "CUser::IsAtkable��������,"
// 				"�����PKģʽ.[��].�û�:%s",this->GetName());
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
// 	case    PKMODE_SYNDICATE://��
// 		{
// 			CUser* pUser = NULL;
// 			if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)) && this->GetSynID() != ID_NONE)
// 			{
// 				if (this->GetMasterSynID() == pUser->GetMasterSynID() )
// 				{
// 					if (bSendHint)
// 						this->SendSysMsg(g_objStrRes.GetStr( 15052));//"����Ķ���:�ڵ�ǰPKģʽ��,���ܹ������ų�Ա"
// 					return false;
// 				}
// //				else if(IsMate(pUser))
// //				{
// //					if (bSendHint)
// //						this->SendSysMsg("����Ķ���:�ڵ�ǰPKģʽ��,���ܹ�����ż");
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
// 								this->SendSysMsg(g_objStrRes.GetStr( 15053));//"����Ķ���:�ڵ�ǰPKģʽ��,���ܹ�ͬ��"
// 							return false;
// 						}
// 					}
// 				}
// 			}
// 		}
// 		break;
// 	case	PKMODE_ARRESTMENT://��
// 		{
// 			LOGERROR( "CUser::IsAtkable��������,"
// 				"�����PKģʽ.[��].�û�:%s",this->GetName());
// 			return false;
// //			if(pTarget->IsEvil())
// //				return true;
// 		}
// 		break;
// 
// 	case	PKMODE_SAFE://��
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
// 					this->SendSysMsg(g_objStrRes.GetStr( 15054));//"����Ķ���:�ڵ�ǰPKģʽ��,���ܹ���Ŀ��"
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


	




	//ǰ���жϹ�
//	if(GetID() == pTarget->GetID())
//		return false;
	
//20070315����.����
//	CNpc* pNpc;
//	if(pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
//	{
//		if(pNpc->IsSynFlag() && GetSynID() != ID_NONE && pNpc->GetSynID() != ID_NONE)
//		{
//			//---jinggy---���ǹ�����ɣ����������빥��İ��ɳ�Ա�ſ��Դ�---begin
//		/*	CGameMap * pMapSyn = this->GetMap();
//			if(pMapSyn->IsSynMap())
//			{
//				if(pMapSyn->IsWarTime())
//				{
//					//�������빥��İ��ɣ��Ͳ��ܴ�
//					CNpc * pWarLetterNpc = pMapSyn->GetNpcSynWarLetter();
//					if(!pWarLetterNpc)
//						return false;
//
//					if(this->GetSynID() != (pWarLetterNpc->GetApplyAttackerSynID()))
//						return false;
//					
//				}
//				else //��ս��ʱ�䲻������
//					return false;
//			}*/
//			//---jinggy---���ǹ�����ɣ����������빥��İ��ɳ�Ա�ſ��Դ�---end
//			CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
//			if(pSyn && pSyn->IsNearSyn(pNpc->GetSynID()))
//				return false;
//		}
//	}

	// map check
	if (!(this->GetMap() && pRealTarget->GetMap() && this->GetMap() == pRealTarget->GetMap()))
	{
		if (bSendHint)
			this->SendSysMsg(g_objStrRes.GetStr( 15055));//"����ĵ�ͼ:����Ŀ�겻��ͬһ����ͼ"
		return false;
	}

	CUser* pUserTarget = NULL;
	pTarget->QueryObj(OBJ_USER, IPP_OF(pUserTarget));


	//  [12/19/2007 ���]	��ս	6405
	//	ǿ��Ŀ��ֻ�ܹ����Լ�������3�롣���Ƕ�Ŀ��ʹ�õ�Debuff����Ҫ�µ�status��ʹ�Լ���Ϊ���˵�Ŀ�꣬�ҵ��˲��ܸı�Ŀ�ꡣֻ�������Ч��
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
	// װ��DATA
	if(!m_data.Create(idUser, Database()))
	{
		LOGWARNING("����m_dataʧ��");
		return false;
	}

	nStep = 2;
	// �����ͼ
// 	int nLife = GetLife();
// 	if( nLife <= 0 ) 
// 	{
// 		this->SetAttrib(_USERATTRIB_LIFE, GetMaxLife() / 10, SYNCHRO_TRUE);
// 		this->SetAttrib(_USERATTRIB_MANA, GetMaxLife() / 10, SYNCHRO_TRUE);
// 		this->SetAttrib(_USERATTRIB_SP, 0, SYNCHRO_TRUE);
// 	}
	InitSpeInfoOnly();//ֻ�����ж�ר���л������ݣ������Ҫ���ߣ��Ͳ��ټ�������������

	nStep = 3;
	CHECKF(m_pSpecilLineInfo);
	//���ж�һ���ǲ�����Ҫ���ߣ���Ҫ�Ļ��Ͳ��ټ���������
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
		//goto ���ֳ�����ͼ����
		if (idMap==ID_NONE)
		{
			idMap = GetBornMap();
		}
		m_pMap = MapManager()->GetGameMap(idMap);
	}

	nStep = 7;
	if(!m_pMap)		//? ���ͼ
	{
		LOGWARNING("�Ҳ���ԭ��ͼ idMap = %u",idMap);
		m_nPosX		= DEFAULT_LOGIN_POSX;
		m_nPosY		= DEFAULT_LOGIN_POSY;
		OBJID idMap	= DEFAULT_LOGIN_MAPID;
		m_pMap		= MapManager()->GetGameMap(idMap);
		CHECKF(m_pMap);
	}

	nStep = 8;
	if(!m_pMap->IsStandEnable(m_nPosX,m_nPosY))
	{
		LOGWARNING("ԭ����Ƿ� idMap = %u,m_nPosX = %u,m_nPosY = %u",idMap,m_nPosX,m_nPosY);
		m_nPosX		= DEFAULT_LOGIN_POSX;
		m_nPosY		= DEFAULT_LOGIN_POSY;
		OBJID idMap	= DEFAULT_LOGIN_MAPID;
		m_pMap		= MapManager()->GetGameMap(idMap);
		CHECKF(m_pMap);
	}

 	CHECKF(m_pMap);

	if( this->IsGM() )
		m_bHide = true;	//Ĭ��GM��ʼΪtrue;
	
#ifdef _DEBUG
	m_bHide = false;	//debugģʽ��GM�������������
#endif
	//�Զ��һ�����
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
	m_tFight.SetInterval(/*this->GetIntervalAtkRate()*/800);		// ͽ�ֹ�Ƶ��
	m_tFight.Update();

	m_tAutoHealRecover.SetInterval( AUTOHEAL_TIME );
	m_tAutoHealRecover.Update();

	m_tItemLifeTime.SetInterval(ITEM_LIFE_TIME_FIRST_CHECK_INTERVAL_SEC);	//	��һ�μ��
	m_tItemLifeTime.Update();

	m_tVipFaceTime.SetInterval(VIP_FACE_SEC);
	m_tVipFaceTime.Update();

	m_tVipTime.SetInterval(VIP_CHECK_SEC);
	m_tVipTime.Update();

	//[2010-08-27 goto]����������ļ�ʱ
	m_tHpMpAdd.SetInterval(ESEPCIAL_DRUG_SEC);
	m_tHpMpAdd.Update();

	m_EverydayOL.SetInterval(EverdayOL_SEC);
	m_EverydayOL.Update();

	//д���ݿ�ʱ�䣬���һ���Դ���ͬ�����
	m_tUpdateDatabase.SetInterval(UNDATEDATABASE_BASETIME + RandGet(UNDATEDATABASE_OFFTIME));
	m_tUpdateDatabase.Update();

	m_tPK.SetInterval(PK_DEC_TIME);// every 1 minutes reduce 2 pk value //change huang 2004.1.11
	m_tPK.Update();
	
	m_tFati.SetInterval(FATIGUE);		//�����ж�user�ķ�����
	m_tFati.Update();

	m_tTeam.SetInterval(TIME_TEAMPRC);
	m_tTeam.Update();

	m_pBattleSystem	= new CBattleSystem(this->m_idProcess, this);
	ASSERT(m_pBattleSystem);

	//��Ʒ
	IDatabase* pDb = Database();
	CreateItemFree(pDb);
	InitStorage(pDb,false);

	//����ϵͳ��ʼ��
	UserJinmeiSysMgr()->OnUserLogin(this,false);

	//key
	if(!IniUserKey(false,false))
		return false;

	//autofight
	if(!IniUserAutoFight(false))
		return false;

	// �䷽
	if(!m_pUserPeifang)
		m_pUserPeifang = CUserPeifang::CreateNew();
	if(!m_pUserPeifang)
		return false;
	m_pUserPeifang->Create(PID,this,false);

	//����
	m_pTaskDetail = CTaskDetail::CreateNew();
	IF_NOT(m_pTaskDetail && m_pTaskDetail->Create(m_idProcess, this,false))
		return false;	

	//����
	if(!InitEudemon(pDb,false))
		return false;
	m_idNowEudemon = ID_NONE;

	//���
	m_pSyn	= CSynAttr::CreateNew(m_idProcess, this);
	ASSERT(m_pSyn);
// 	if (!m_pSyn )
// 		return false;

	//����
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

	//�ƺ�
	m_pTitle = CUserTitle::CreateNew(m_idProcess, this);
	ASSERT(m_pTitle);
	
	//��Ʊ,������
	m_pTreasure = CTreasure::CreateNew(m_idProcess, this);
	ASSERT(m_pTreasure);

	//ʦͽ
	m_pUserTutor = CUserTutor::CreateNew(m_idProcess, this);
	ASSERT(m_pUserTutor);

	//���ﵰ
	m_pPetEgg = CPetEgg::CreateNew(m_idProcess, this);
	ASSERT(m_pPetEgg);

	//����ף��
	m_pOnlineGift = COnlineGift::CreateNew(m_idProcess, this);
	ASSERT(m_pOnlineGift);

	//��������
	InstanceUseMgr()->UserLogin(GetID());

	//���˾�������Ϣ
	m_pUserWarGame = CUserWarGame::CreateNew(m_idProcess, this);
	ASSERT(m_pUserWarGame);

	//ר�߼�¼
	m_pSpecilLineInfo = CSpecilLineInfo::CreateNew(this);
	ASSERT(m_pSpecilLineInfo);
//modify code by �ֵ�Ԩ begin 2011.5.30-
	m_pvaslvdata = CVasLvData::CreateNew();
	ASSERT(m_pvaslvdata);
//modify code by �ֵ�Ԩ end

	m_bLoaded = true;

	//������֤
	m_pVerification = CVerification::CreateNew(m_idProcess, this);
	ASSERT(m_pVerification);

	ClearCheckData();
	initCompensationManager();

	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUser::Init			()		// login �� change map_group��Ҫִ�еĲ���
{
	m_tFight.SetInterval(/*this->GetIntervalAtkRate()*/800);		// ͽ�ֹ�Ƶ��
	m_tFight.Update();

	m_tAutoHealRecover.SetInterval( AUTOHEAL_TIME );
	m_tAutoHealRecover.Update();

	m_tItemLifeTime.SetInterval(ITEM_LIFE_TIME_FIRST_CHECK_INTERVAL_SEC);	//	��һ�μ��
	m_tItemLifeTime.Update();

	m_tVipFaceTime.SetInterval(VIP_FACE_SEC);
	m_tVipFaceTime.Update();

	m_tVipTime.SetInterval(VIP_CHECK_SEC);
	m_tVipTime.Update();

	//[2010-08-27 goto]����������ļ�ʱ
	m_tHpMpAdd.SetInterval(ESEPCIAL_DRUG_SEC);
	m_tHpMpAdd.Update();

	m_EverydayOL.SetInterval(EverdayOL_SEC);
	m_EverydayOL.Update();

	//д���ݿ�ʱ�䣬���һ���Դ���ͬ�����
	m_tUpdateDatabase.SetInterval(UNDATEDATABASE_BASETIME + RandGet(UNDATEDATABASE_OFFTIME));
	m_tUpdateDatabase.Update();

	m_tPK.SetInterval(PK_DEC_TIME);// every 1 minutes reduce 2 pk value //change huang 2004.1.11
	m_tPK.Update();
	
	m_tFati.SetInterval(FATIGUE);		//�����ж�user�ķ�����
	m_tFati.Update();

	m_tTeam.SetInterval(TIME_TEAMPRC);
	m_tTeam.Update();

	m_pBattleSystem	= new CBattleSystem(this->m_idProcess, this);
	ASSERT(m_pBattleSystem);

	// װ��USER�������
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



	///�����������֪ͨ,�е�ͼ��ʱ����֪ͨ
	if(SynManager()->QuerySyndicate(GetSynID()) )
	{
		if(GetSyndicate()->GetInt(SYNDATA_STATE) == 2)
		{
			SynManager()->QuerySyndicate(GetSynID())->SetOnline(GetID(),CVipList::GetLineByName(MapGroup(PID)->GetServerName()));			
			SynManager()->QuerySyndicate(GetSynID())->BroadcastSynMemberInfoMsg(GetID());
		
			///���ɳ�Ա����,��������Ϣ�㲥��������
			///������֪ͨ�����߸�������
			CMsgSynMemberInfo msg;
			if (msg.Create(SynMemberOnlineStatusChange,1,2))
			{
				msg.Append(GetSynID(),"",0,0,0,0,0,0,0,0,0,0,0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
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

	//�ƺ�
	m_pTitle = CUserTitle::CreateNew(m_idProcess, this);
	ASSERT(m_pTitle);
	m_pTitle->CreateAll();
	
	//��Ʊ,������
	m_pTreasure = CTreasure::CreateNew(m_idProcess, this);
	ASSERT(m_pTreasure);
	m_pTreasure->CreateAll();
	//����ף��
	m_pOnlineGift = COnlineGift::CreateNew(m_idProcess, this);
	ASSERT(m_pOnlineGift);
	m_pOnlineGift->CreateAll();


	//����ϵͳ��ʼ��
	UserJinmeiSysMgr()->OnUserLogin(this,true);

	//��������
	InstanceUseMgr()->UserLogin(GetID());
	//ʦͽ
	m_pUserTutor = CUserTutor::CreateNew(m_idProcess, this);
	ASSERT(m_pUserTutor);
	m_pUserTutor->CreateAll();
	

	m_pUserWarGame = CUserWarGame::CreateNew(m_idProcess, this);
	ASSERT(m_pUserWarGame);
	m_pUserWarGame->Create(Database());
	
	//���ﵰ
	m_pPetEgg = CPetEgg::CreateNew(m_idProcess, this);
	ASSERT(m_pPetEgg);
	m_pPetEgg->CreateAll();
//modify code by �ֵ�Ԩ begin 2011.5.30-
	m_pvaslvdata= CVasLvData::CreateNew();
	ASSERT(m_pvaslvdata);
	m_pvaslvdata->Create(GetAccountID(),GetID(),Database());
//modify code by �ֵ�Ԩ end
	m_bLoaded = true;
	////������֤
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

	//ר��
	m_pSpecilLineInfo = CSpecilLineInfo::CreateNew(this);
	ASSERT(m_pSpecilLineInfo);
	m_pSpecilLineInfo->Create();
	//���߾���
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
		UpdateAllOnlineTime();// �������������ʱ�� [9/14/2007 %PENGFENG%]		
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
//modify code by �ֵ�Ԩ begin 2011.5.30-
		DEBUG_TRY
		m_pvaslvdata->SaveInfo();
		DEBUG_CATCH("m_pvaslvdata->Update() exit")
//modify code by �ֵ�Ԩ end
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
//modify code by �ֵ�Ԩ begin 2011.5.30-
			DEBUG_TRY
			m_pvaslvdata->SaveInfo();
			DEBUG_CATCH("m_pvaslvdata->Update()")
//modify code by �ֵ�Ԩ end
		}
	}
//	SaveEudemon();
}

//////////////////////////////////////////////////////////////////////
bool CUser::AddLeaveWord(LPCTSTR szRecv, LPCTSTR szWords, LPCTSTR szColor)//[2010-06-10 goto]�����ı���ɫ�ֶ�
{
	if (LeaveWord()->CountWords(GetName(), szRecv) >= MAX_LEAVEWORD_COUNTS)
	{
		SendSysMsg(_TXTATR_TOPSHOW,"���Թ��࣬�޷����͸��������");
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
					SendSysMsg(_TXTATR_TOPSHOW,"�Է������ߣ��ѳɹ�����Ϣ����Ϊ����");
					return true;
				}	
			}
			else
			{
				//SendSysMsg(g_objStrRes.GetStr( 12029), LEAVEWORD_INTERVAL_MINS);//STR_MSG_FAILED
				SendSysMsg(_TXTATR_TOPSHOW, "��Ϣ���͹���Ƶ�������Ժ��ٷ�");
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
///�������Ա��Ϣ�����û�
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
bool CUser::SendInformation(const char* strInfo)///softworms��չ-2007-12-7
{
	return SendSysMsg(strInfo);
}

bool CUser::SendSysInfo(const char* strInfo)///softworms��չ-2007-12-7
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
		//20070113����:ֱ��д��--------------------------------
		const int _posX = 30;
		const int _posY = 25;
		//06.11.17�����ƶ�������뵽��λ�ò��޸�
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
		sprintf(szMsg,"<%s>ɱ�����飬�������أ��ѱ��ٸ�ץ��Ͷ���˼���",Killer);//"holy shit!"
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
	//	UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
	//	AddLeaveWord(this->QueryOwnerUser(), "����Ϊɱ�˹��౻�����˼��������úø�����������!");
// 		CMsgTalk msg;
// 		if (msg.Create("ϵͳ", this->GetName(), "<font color='#ff0000'>����Ϊɱ�˹��౻�����˼��������úø����������ˣ�</font>", NULL, 0xfefeefef, _TXTATR_CHATWITH))
// 		this->SendMsg(&msg);
		this->SendEmail(this->GetName(), "<font color='#ff0000'>����Ϊɱ�˹��౻�����˼��������úø����������ˣ�</font>", this);
		
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
	ProcessAction(1027/*�ű�*/);
	//˽�˼���
}
//////////////////////////////////////////////////////////////////////
// return false: can't fly
// call - may be delete this;
int CUser::FlyMap(OBJID idMap, int nPosX, int nPosY)
{
	if(m_pMap == NULL)			// not in map
	{
		STACK_DUMP();
		LOGWARNING("��ҳ�������������");
		LOGWARNING("�������userName = %s��",GetName());
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
// 	if (m_bAutoFightFlag && !this->IsAlive())//�Զ��һ�ʱ
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
			if (pNewMap->GetType()!=2 && pNewMap->GetType()!=3)//ר��
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
			if (pNewMap->GetType()==2)//ר��
			{
				LeaveMap();
				if (pNewMap->IsSynMap())//����
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
// 						this->SendSysMsg("�����ڻ����ܼ�Ԧ�������������Լ��ĵȼ������ٻ���");
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
// 			//goto ֪ͨ����
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
		this->SendSysMsg("�����޷��뿪�˵�ͼ");
		return true;
	}
	return	false;
}

//////////////////////////////////////////////////////////////////////
void CUser::OnTimer(time_t tCurr)
{
	if(!m_pMap)
		return;

	if(IsClosing())//���ڹر���
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
//modify code by �ֵ�Ԩ begin 2011.6.22
	int retvalue = 0;
//modify code by �ֵ�Ԩ end
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
//modify code by �ֵ�Ԩ begin 2011.6.22
				DEBUG_TRY
				if( idBuff == STATUS_RETORT_ZHF )
					this->QueryMagic()->ProcessBuffBomb(this,1);
				if( idBuff == STATUS_RETORT_MDSX_1 )
				{
					if( this->GetNewValue() >= 3000 && this->GetNewValue() <= 35000 )
						retvalue = this->QueryMagic()->ProcessBuffBomb(this,3);//��Ѫ����buff1
				}
				DEBUG_CATCH2("DetachStatus(this,pStatus->GetID()) %u",pStatus->GetID());
//modify code by �ֵ�Ԩ end
			}
			DEBUG_CATCH2("DetachStatus(this,pStatus->GetID()) %u",pStatus->GetID());
		}
	}
//modify code by �ֵ�Ԩ begin 2011.6.22
	DEBUG_TRY
	if( 4 == retvalue || 5 == retvalue )
	{
		this->DetachStatus(this,STATUS_RETORT_MDSX);
		if( 4 == retvalue )
			this->BeKillBySys();
	}
	DEBUG_CATCH("if( 4 == retvalue || 5 == retvalue )");
//modify code by �ֵ�Ԩ end
	DEBUG_CATCH2("CUser status timer crash. buffid = %u",idBuff)

	DEBUG_TRY
	DEADLOOP_CHECK(PID, "ProcessPkTimer")
	if (this->GetMap() && !this->GetMap()->isDisableReducePk())
	{
		if (m_tPK.ToNextTick(PK_DEC_TIME))					//pkֵ�仯�Ĵ���
			this->ProcessPkTimer();
	}
	DEBUG_CATCH("CUser pk timer crash.")

	DEBUG_TRY
	DEADLOOP_CHECK(PID, "PorcessFatigue")
	if (this->isFatigue() &&  (m_data.GetFatiTime() > 0) && g_bEnableFatigue)
	{
		if (m_tFati.ToNextTick(FATIGUE))					//������
			this->ProcessFatigue();
	}
	DEBUG_CATCH("CUser Fatigue timer crash.")

	DEBUG_TRY
	if (m_tHpMpAdd.ToNextTick(ESEPCIAL_DRUG_SEC))					//�������������
		this->SpendNewRemedy();
	DEBUG_CATCH("CUser HpMpAdd timer crash.")

	DEBUG_TRY
	if (m_EverydayOL.ToNextTick(EverdayOL_SEC))					//���߼�ʱ
		this->ProcessEverydayOL();
	DEBUG_CATCH("CUser ProcessEverydayOL timer crash.")
	
// 	DEBUG_TRY
// 	DEADLOOP_CHECK(PID, "ProcessPkTimer")
// 	if (m_tPK_InPrison.ToNextTime()) 
// 		this->ProcessPkTimer_InPrison();
// 	DEBUG_CATCH("CUser pk inprison timer crash.")

// 	DEBUG_TRY
// 	if (m_tSynWork.TimeOver(SYN_WORK_SEC))					//��
// 	{
// 		this->ProcessWork();
// 	}
// 	DEBUG_CATCH("SYNWORK TIMER CRASH.")

	
	DEBUG_TRY
	SYSTEMTIME curTime;
	::GetLocalTime( &curTime );
	if(!m_bResetDarTimeSet) //��һ������
	{
		int nCheck = m_data.GetLastLogin();
		if(nCheck == 0)
		{
			m_data.SetLastLogin();
		}
		else
		{
			//m_tLastResetVasExchangeTime.wDay = m_data.GetLastLogin() % 100; //���Ǽ��ʱ�䵱���5��������ʱ��Ĳ�ֵ
			CTime timecount(nCheck/10000,(nCheck % 10000)/100,nCheck % 100,5,0,0);
			CTime timeCurr(CTime::GetCurrentTime());
			CTimeSpan spanTime = timeCurr - timecount;
			int nPassDay = spanTime.GetTotalHours() / 24;

			if(nPassDay > 0) //��һ����
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

				ResetDayTaskMask();/// ÿ��6���������������
				ResetDayTaskMask2();
				ResetMonsterKillAndEveryDayOL();
				SetVasTicketAmount(0);
				if(m_pUserWarGame)
					m_pUserWarGame->SetWarGameMask(0);

				if(curTime.wHour < 5)
				{
					CTimeSpan passDay(1,0,0,0); //һ��ǰ
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
		//�����º����ڲ���ͬһ��
		if(curTime.wHour == 5 /*&& curTime.wMinute == 00*/)//6:00
		{
			ResetDayTaskMask();///ÿ��5���������������
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

	//goto �µ������;���洢����
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
	if(lastUpdateTime / 500 != clock() / 500)//���ܼ�ʱ��ֿ�
	{
		if (m_tUpdateDatabase.ToNextTick(UNDATEDATABASE_BASETIME + UNDATEDATABASE_OFFTIME))					//д���ݿ�
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
//06.3.21�����޸�....
void CUser::ProcessAutoAttack		()
{
	// �ٶ��ͻ��޺�����߹����ٶ�
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

	if( prof & PROF_NEWHAND) //����
	{
		m_data.SetStr( 36* GetLev() + 94,nType);
		m_data.SetIntex( 36* GetLev() + 94,nType);
		m_data.SetSta( 36* GetLev() + 94,nType);
		m_data.SetSpi( 36* GetLev() + 94,nType);
		m_data.SetAgi( 36* GetLev() + 94,nType);
		m_data.SetPot(0 ,nType);
	}
	else if( prof & PROF_ASSIST) //����
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
	else if( prof & PROF_MASTER) //��ʦ
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
	else if( prof & PROF_PASTOR) //��ʦ
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
	else if( prof & PROF_ASSISSIN) //�̿�
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
	else if( prof & PROF_SOLDIER) //սʿ
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
	else if( prof & PROF_SWORD) //����
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
			sprintf(text, "���Ķ���%s�Ѿ���������������ǰΪ��������Ѫ��ħ��", GetName());
			pUser->SendSysMsg(text);
		
			CMsgTeam msg;
			if(msg.Create(pUser->GetID(), _MSGTEAM_MEMADDLIFE_EFFECT, SECONDKEEPEFFECT_LIFESTONE))
				pUser->BroadcastRoomMsg(&msg, SYNCHRO_TRUE);
		}
	}
}

void CUser::ClearManyStatusWhenDie()//-------���---2007/05/16-----
{
	for(int i = QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
		if(pStatus)
		{
			///������2007-07-13�ڴ˽������������״̬���в��.
//			m_data.m_Info.i64Effect	&= KEEPEFFECT_NOT_CLEAR;//�����������״̬��ԭ���о��У�ԭ���޾��ޣ���&
			//////////////////////////////////////////////////////////////////////////
			//	20070717 BUG0001013 ��� ������ȥ������״̬
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
			if( STATUS_PHYATTACKUP_TUTOR_PERCENT == pStatus->GetID() )//  [�½���10/26/2007]
				continue;
			if( STATUS_MAGATTACKUP_TUTOR_PERCENT == pStatus->GetID() )//  [�½���10/27/2007]
				continue;
			if( STATUS_FISH_WORKER == pStatus->GetID() )//  �湤���������
				continue;
			if( STATUS_FISH_TUTOR == pStatus->GetID() )//  �����ʦ���������
				continue;
			if( STATUS_CITYWAR_ATTACKMARK == pStatus->GetID() )
				continue;
			if( STATUS_CITYWAR_DEFENCEMARK == pStatus->GetID() )
				continue;
			if( STATUS_LIANGONG == pStatus->GetID() )
				continue;
			if(STATUS_ACTIONEND == pStatus->GetID())
				continue;
			if( STATUS_DEAD_ADDTEAMATTR == pStatus->GetID() ) //��ƷѪ��ʯ����
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
			if (STATUS_DOT_ADDEXP == pStatus->GetID())//���
				continue;
			//2007827����:��������buff���������--------------
			if( pStatus->GetID() == STATUS_PHYSICALATTACKUP_PERCENT_WS
				|| pStatus->GetID() == STATUS_MAGICATTACKUP_PERCENT_ED
				|| pStatus->GetID() == STATUS_DAMAGEDOWN_PERCENT_TN
				|| pStatus->GetID() == STATUS_GETEXPUP_PERCENT_WT
				|| pStatus->GetID() == STATUS_PHYSICALDEFENCEUP_NUMBER_SF
				|| pStatus->GetID() == STATUS_MAGICDEFENCEUP_NUMBER_EM
				|| pStatus->GetID() == STATUS_CRITRATEUP_NUMBER_SW
			//	|| pStatus->GetID() == STATUS_SYNWORKTIME//���ɴ�buff -- jay
// 				|| pStatus->GetID() == STATUS_DOT_ADDLIFE_RB
// 				|| pStatus->GetID() == STATUS_DOT_ADDMANA_MC
				|| pStatus->GetID() == STATUS_STRUGGLE_RELIVE
				|| pStatus->GetID() == STATUS_ENEMYFIRE ///����������ӵ�buff-softworms-2007-11-16
				|| pStatus->GetID() == STATUS_BIZ_DEAD
				|| pStatus->GetID() == STATUS_DOT_WARGAMEEXP
				|| pStatus->GetID() == STATUS_VERIFICATION_RUNNING
				|| pStatus->GetID() == STATUS_VERIFICATION_WAIT
				)
				continue;
			//------------------------------------------------

			//  [2007-9-25] ���	��������ս�����BUF
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
		//����û�м���,����
// 		CGameMap* pMap = MapManager()->QueryMap(this->GetMapID());
// 		IF_OK (pMap) 
// 		{
// 			if (pMap->IsPrisonMap())
// 			{
// 				this->AddPk( -1 * PKVALUE_DEC_ONCE * 2 );//�ڼ���2������
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
// // 	char szName[20] = "����С���";
// //	sprintf(szMsgxyz, "<font color='#e46d0a'>{%s}</font>�Ļ��ʴȣ��ڶ�üɽ�����˻�ʱ����ü������Σ���Ϊ��һ��<font color='#cc66ff'>�����ɵ���</font>!",this->GetName());
// // 	this->GetItemInfoByItemid(4091);
// // //	msg.Create(this->GetName(), ALLUSERS_NAME, szMsgxyz, 0, 0xffffff, 0, _TXTATR_ANNOUNCE, 300001, 300001, "ֹѪ��" );
// // /*	UserManager()->BroadcastMsg("����������Ѱ��·����������Ѱ��·����������Ѱ��·����������Ѱ��·", NULL/ *pUser* /, NULL, 0xffffff, _TXTATR_ANNOUNCE);*/
// // //	sprintf(szMsgxyz, "<font color='#e46d0a'>{%s}</font>�Ļ��ʴȣ��ڶ�üɽ�����˻�ʱ����ü������Σ���Ϊ��һ��<font color='#cc66ff'>�����ɵ���</font>!", this->GetName());
// // //	UserManager()->BroadcastMsg("<font color='#e46d0a'>321</font>�Ļ��ʴȣ��ڶ�üɽ�����˻�ʱ����ü������Σ���Ϊ��һ��<font color='#cc66ff'>�����ɵ���</font>!", NULL, NULL,0xffffff, _TXTATR_ANNOUNCE);
// //		UserManager()->BroadCastMsgAndReceiveName(szMsgxyz, NULL, NULL, 0xffffff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
// // //	SendSysMsg(szMsgxyz);
// //	UserManager()->BroadcastMsg(&msg, ALLUSERS_NAME)
// }
//////////////////////////////////////////////////////////////////////
bool CUser::IsPkEnable(IRole* pRole)
{
	//20070315����:�����ж��õ�CUser::IsAtkable()��ȥ
	if (!pRole || this->GetID() == pRole->GetID())
		return false;
	
	CMonster* pMonster = NULL;
	if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
		// �����޸�   08.6.12  ��� 
//		{
//			if(this->GetPhyle() == PROF_DRAGON && strstr(pMonster->GetName(),"����") != 0)
//				return  false;
//			else if(this->GetPhyle() == PROF_HUMAN && strstr(pMonster->GetName(),"������") != 0)
//				return  false;
//			else if(this->GetPhyle() == PROF_GENE && strstr(pMonster->GetName(),"������") != 0)
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
//2007828����:
//void CUser::Reborn(bool bChgPos/*=true*/)
void CUser::Reborn(bool bChgPos/*=true*/,
				   bool bRebornFlag/*=false*/,
				   int nRecoverPercent/*=0*/)
{
	if (this->IsAlive())
		return;

	DetachStatus( QueryRole(),STATUS_DIE );
	m_bGhost	= false;

	if( bRebornFlag )//����������ҡ�
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
    
	//06.10.13����:��Ϊ���︴���,�������ͬһ��ͼ��,�������,�����޸Ĵ���
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
// ��������ϵͳ
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
//06.11.7�����޸�
//�˺���ÿ10�����һ��,Ҫ���ǵ�������Ч������
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
	int nEne = m_data.GetEne();//goto ����
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
		//20070516����:bug.��������.
		//���¹�ʽ�������ϲ�����ȷ,���Ѿ����˱���ȷ��ʽ���������
		//��Ϊ���bug.���������������.
		//����������ֵ�ӽ�ԭ��Ϣ�ȴ���ʱTHREADDELAY_MS
		//----------------------------------------------
		nEchoTime += 400;//ms
		//----------------------------------------------
		if (nEchoTime < (int)dwClientTime-(int)m_dwLastClientTick-(int)dwTimeServerTickInterval)
		{
			::GmLogSave("������ң�%s��ʹ�ü�������", this->GetName());

			this->SendSysMsg( g_objStrRes.GetStr( 15059) );//"����:ʹ�ü�����,ϵͳ�Ѽ�¼!"
			UserManager()->KickOutSocket(m_idSocket, "������");
			//20070516��¼��ϸ��Ϣ---------------------
			LOGERROR( "��������ϸ��Ϣ:"
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
//��ȡȫ����������
int CUser::GetFriendAllAmount()
{
	return GetFriendGroupAmount(1) + GetFriendGroupAmount(2)  + GetFriendGroupAmount(3)  + GetFriendGroupAmount(4);
}

//��ȡָ������ĺ�������
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

//  [2007-9-10] ���
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
//goto ��������
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
bool CUser::AddBeFriend(OBJID id,OBJID idUserOwnmeAsFriend,USHORT nFriendType,LPCTSTR pszName )//��¼A��ĳ���˵�ĳ�����͵�����
{
	CFriend* pFriend = GetBeFriend(idUserOwnmeAsFriend);//����ظ����
	if (pFriend)
		return false;

	pFriend = CFriend::CreateNew();
	if(pFriend )//����Friendid���Լ���ID,szFriendNameҲ���Լ���NAME
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
	if (pFriend)//B�Ѿ���A�ĺ���
	{
		if (nFriendType==ENEMY_GROUP)//�������Ƶ����˷���
		{
			unsigned int uoldType = pFriend->GetFriendType();
			if( MoveFriend(pFriend,nFriendType) )//�޸ĺ��ѷ���
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
		return false;//goto �ƶ������ʱ�򷵻�false,����෢��һ����ӳɹ���ɳ���������ͬ����
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
//�������[2008/12/17]
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
//[2010-06-09 goto]ɾ���ѿ������û�����
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
//[2010-06-10 goto]ɾ����ʱ����
void CUser::DelTempFriend()
{
	OBJID dwUserId = this->GetID();
	char	szSQL[256]="";
	sprintf(szSQL, "DELETE FROM %s WHERE userid=%d AND friendtype=%d", _TBL_FRIEND, dwUserId, TEMP_FRIEND_GROUP );
	ASSERT(g_db.ExecuteSQL(szSQL));
}
//////////////////////////////////////////////////////////////////////////
//[2010-06-10 goto]��Ӳ����ߵ��û�Ϊ����
OBJID CUser::AddLeaveFriend(LPCTSTR szFriendName, unsigned int uFriendType, CMsgFriend* pFriendMsg)
{
	CFriend* pFriend= GetFriendByName((char*)szFriendName);
	if (pFriend)//B�Ѿ���A�ĺ���
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
	char szMateName[_MAX_NAMESIZE] = "��";
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
			return "δ֪";
		return pTemp->name;
	}
	else
	{
// 		char	szSQL[256];
// 		sprintf(szSQL, "SELECT name FROM %s WHERE id='%u' LIMIT 1", _TBL_USER, idUser);
// 		IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
// 		if(!pRes)
// 		{
// 			return "δ֪";
// 		}
// 		
// 		IRecord* pRecord = pRes->CreateNewRecord();
// 		pRes->Release();
// 		if(!pRecord)
// 			return "δ֪";
// 		
// 		static char szName[_MAX_NAMESIZE];
// 		pRecord->LoadString( szName, "name", _MAX_NAMESIZE );
// 		if (!szName || strlen(szName)==0)
// 			return "δ֪";
// 		
// 		pRecord->Release();
// 		return szName;
		LogSave("ERROR:�Ҳ������ iduser=%u",idUser);
		return "δ֪";
	}
}
//////////////////////////////////////////////////////////////////////////
void CUser::SendFriendInfo()
{
	int	nAmount = m_setFriend.size();
	CHECK(nAmount <= _MAX_USERFRIENDSIZE);
	//�������ʱ�����к��ѵ���Ϣ������
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
			//20061012����.CMsgFriend::Create()������������--------------------------
			unsigned int		profession	= pFriend->GetFriendFirJob();
			//------------------------------------------------------------------------
			int nFriendType		= pFriend->GetFriendType();//�������[2008/12/12]��������
            int nSex = pFriendUser ? pFriendUser->GetSex() : 0;
			unsigned int nFriendShip	= pFriend->GetFriendShip();

			CMsgFriend	msg;
			//  [11/3/2007 ���]
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
				pFriend->GetFriendFeel(),//[2010-06-05 goto]�����������
				0))//��ϵ,ʦͽ������...
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
				//MapGroup(PID)->QueryIntraMsg()->TransmitMsg(&msg, GetSocketID(), GetID());		// ֪ͨ������ͼ�飬�´����Һ������ߵ���Ϣ
				msg.SetTransData(GetID());
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg);
			}
		}
	}
	//ͬʱ���Լ�����Ϣ�������Լ����Լ�Ϊ���ѵ����
	// ͨ��WORLD����ת���������������
	nAmount = m_setBeFriend.size();
	for( i = 0; i < nAmount; i++)
	{
		CFriend* pFriend = m_setBeFriend[i];
		if(pFriend)//20061020����ע.�Է�û����,��Ӧ���跢��.���˴��޷���֪�Է��Ƿ�����
		{
			OBJID	idFriend	= pFriend->GetUserID();//��ȡ�����Լ�Ϊ���ѵ���ҵ�ID
			CMsgFriend	msg;
			//20061012����.�¼���������.
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
				GetFeel(),//[2010-06-05 goto]�����������
				0))//��ϵ,ʦͽ������...
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
// 		if(pFriend)//20061020����ע.�Է�û����,��Ӧ���跢��.���˴��޷���֪�Է��Ƿ�����
// 		{
// 			OBJID	idFriend	= pFriend->GetUserID();//��ȡ�����Լ�Ϊ���ѵ���ҵ�ID
// 			CMsgFriend	msg;
// 			//20061012����.�¼���������.
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
//20070329����:����һ������
//bool CUser::ChangeMapGroup(PROCESS_ID idProcess, OBJID idMap, int nPosX, int nPosY)					// call - delete this;
bool CUser::ChangeMapGroup(PROCESS_ID idProcess, 
						   OBJID idMap, 
						   int nPosX, 
						   int nPosY,
						   Enum_ChangeMapGroupMode mode/* = ENUM_CNGMAPGROUP_MODE_REQ*/)					// call - delete this;
{
	
	//����:��[����-��Ӧ]�ķ�ʽ�����ӽ�׳��
	switch(mode)
	{
	case ENUM_CNGMAPGROUP_MODE_REQ:
		{
			//����
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
			SendSysMsg("�ӵ�ͼ{%s}[%d][%d]������[%d][%d]...", GetMap()->GetName(), GetMapID(), m_idProcess-3, idMap, idProcess-3);
#endif

//  [3/27/2008 ���]	�����뿪��ͼACTION
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
			
			//  [2007-9-26] ��� BUG0001532
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
				
				ClrTeam(false,false);			//? LeaveMapGroup()ʱ���˳�����
			}

			// ����ѧ�� ���. [10/16/2007]
//			MapGroup(PID)->QueryIntraMsg()->SendNewPlayerState(idProcess, GetID(), nNewPlayerState, _MAX_NEWPLAYEREXAMSTATE);

//			MapGroup(PID)->QueryIntraMsg()->SendPresentState(idProcess, GetID(), nPresentNum, _MAX_PRESENT_NUM);


			// pengfeng ʦͽ [10/31/2007]
// 			MapGroup(PID)->QueryIntraMsg()->SendTutorState(idProcess, GetID(), m_setStudent,this->GetTutor(),this->GetExpTutor());
			
			nStep = 3;
			//װ��
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
			//������Ʒ
			m_pPackage->SendAllObjInfo(idProcess);

			nStep = 5;
			//�ֿ�
			m_pStorage->SendAllObjInfo(this,idProcess);

			nStep = 6;
			//����
			MapGroup(PID)->GetUserJinmeiSysMgr()->SendAllObjInfo(this,idProcess);

			nStep = 7;
			//KEY
			m_pUserKey.SendObjInfo(idProcess);

			nStep = 8;
			//AUTOFIGHT
			m_pAutoFightData.SendObjInfo(this,idProcess);

			nStep = 9;
			//�䷽
			if(m_pUserPeifang)
				m_pUserPeifang->SendAllObjInfo(idProcess);

			nStep = 10;
			//����
			if(m_pTaskDetail)
				m_pTaskDetail->SendAllObjInfo(this,idProcess);

			nStep = 11;
			//����
			if(m_pUserPet)
				m_pUserPet->SendAllObjInfo(idProcess);

			nStep = 12;
			//���
			if(QuerySynAttr())
			{
				SynAttrInfoStruct info;
				QuerySynAttr()->GetInfo(&info);
				MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_SYNATTR, &info, sizeof(info));
			}

			nStep = 13;
			//����
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
			//����
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
			//������
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
			//ʦͽ
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
			//ףԸ
			if(m_pOnlineGift)
			{
				OnlineGiftInfoStruct info = {0};
				if (m_pOnlineGift->GetOnlinegiftInfo(&info))
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_ONLINEGIFT, &info, sizeof(info));
			}

			nStep = 19;
			//����
			if (m_pVerification)
			{
				VerificationInfoStruct info = {0};
				if (m_pVerification->GetVerificationInfo(&info))
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_VERIFICATION, &info, sizeof(info));
			}

			nStep = 20;
			//�ƺ�
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
			//���ﵰ
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
			//�����˫��INFO_DOUBLE
			MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_DOUBLE, &m_doubletime, sizeof(DWORD));


			nStep = 23;
			//��������Ϣ
			if(m_pUserWarGame)
			{
				MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_WARGAME, m_pUserWarGame->GetInfo(), sizeof(UserWarGameInfo));
			}

			nStep = 24;
			//ר����Ϣ
			if(m_pSpecilLineInfo)
			{
				STSpecilLine info = {0};
				if (m_pSpecilLineInfo->GetInfo(&info))
					MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_SPECILLINE, &info, sizeof(info));
			}

			nStep = 25;
//modify code by �ֵ�Ԩ begin 2011.5.30-
			if(m_pvaslvdata)
			{
				MapGroup(PID)->QueryIntraMsg()->SendObjInfo(idProcess, GetID(), INFO_VASLV, m_pvaslvdata->GetInfo(), sizeof(UserVasLvInfo));
			}
//modify code by �ֵ�Ԩ end

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
			//goto �������ϵ�buff
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

			UserManager()->LogoutUser(GetSocketID(), false);	// ɾ�����󣬽�����һ��ͼ��, false: not offline
			
			
			// stat
			extern struct STAT_STRUCT	g_stat;
			InterlockedIncrement(&g_stat.setDebug[3]);		// debug
		}
		break;
	default:
		{
			//error!
			LOGERROR( "CUser::ChangeMapGroup��������,�û�:%s idMap=%d x=%d y=%d mode=%d",
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
	// leave map first(�����뿪��ͼ��������ظ����ʹ���Ϣ)
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
	DeleteAllBeFriend();//�������[2008/12/16]
//	DeleteAllFriendContact();//�������[2008/12/18]
//	DeleteAllBeFriendContact();//�������[2008/12/19]
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

	// ���ͻ�Ӧ��Ϣ
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
	//06.11.8�������-----begin---------
	case	INFO_COUNTTIMER:
		AppendCountTimer( ( CountTimerStruct* )pInfo );
		break;
	case	INFO_STATUS_MONSTER:
		{
			StatusInfoStruct info = {0};
			memcpy(&info, pInfo, sizeof(StatusInfoStruct));
			m_setEudemonStatus.push_back(info);//�ȴ�����,�ȳ����ٳ�������ܼ�
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
//modify code by �ֵ�Ԩ begin 2011.5.30-
	case INFO_VASLV:
		if(m_pvaslvdata)
			m_pvaslvdata->AppendInfo((UserVasLvInfo*)pInfo);
		break;
//modify code by �ֵ�Ԩ end
	//----------------------end-----------	
	default:
		ASSERT(!"switch INFO_TYPE");
	}
	return true;
}
//////////////////////////////////////////////////////////////////////
//06.11.8
//����
//��������:���ת����ͼ��,ԭ��ͼ���̷߳���������ƷCD��Ϣ,���ղ�����
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
			pItem = NULL;		// �����뱳��
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
				//07.5.12�����������һ��,���ڿͻ��˶��ԣ���������ӣ����Դ�ʱ��updatebuff
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
				//07.5.12�����������һ��
//				UpdateBuff( pInfo->nStatus,pInfo->nSecs,pInfo->nTimes,pInfo->nPower,pInfo->nParam1,pInfo->nParam2,pInfo->nParam3 );
				
			}
			else
				pStatus->ReleaseByOwner();
		}
	}
//07.5.12����ע������
//���´��룬���ܲ���ʵ��
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
		this->SendSysMsg(g_objStrRes.GetStr(28315));     //�����ս��Ʒ���������������
		return false;
	}

	int sourceItemSize = _MAX_USERITEMSIZE*4;
	OBJID sourceItemID[_MAX_USERITEMSIZE*4];

	int sourceItemAmount = this->GetItemByTypeMulti(CITYWAR_ITEM_TYPEID, sourceItemSize, sourceItemID);
	if(nAmount > sourceItemAmount)
	{
		this->SendSysMsg(g_objStrRes.GetStr(28322));     //�����ϵĳ�ս������������
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
//	g_EconomySystem.ChangeMoneyCount(dwMoney * -1,this->GetProfession());	//���0321 ������������ͳ��

	if(nMoney2 > 0)
		m_data.SetMoney(dwNewMoney2,2);
	if(nMoney1 > 0)
	{
		m_data.SetMoney(dwNewMoney1,1);
		//goto ��¼��������ֵ
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
	//g_EconomySystem.ChangeMoneyCount(dwMoney * -1,this->GetProfession());	//���0321 ������������ͳ��
	SetMoneyByType(dwNewMoney, nType, true);
	
	// update to client...
	int nAtt = 0;
	if (nType==1) nAtt = _USERATTRIB_MONEY;
	if (nType==2) nAtt = _USERATTRIB_MONEY2;
	if (nType==3) nAtt = _USERATTRIB_MONEY3;
	if (nType==4) nAtt = _USERATTRIB_MONEY4;
	if (nType==5) nAtt = _USERATTRIB_MONEY5;

	//goto ��¼��������ֵ
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
		this->SendSysMsg("Ǯ�ѳ�������");
		return false;
	}
//	g_EconomySystem.ChangeMoneyCount(dwMoney,this->GetProfession());	//���0321 ������������ͳ��
	m_data.SetMoney(dwNewMoney,nType,bUpdate);//goto ��Ϊʵʱд���ݿ⣬���⵱��ʱ��ʧ

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
			SendSysMsg("�������%u����",dwMoney);
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
			SendSysMsg("�������%u����",dwMoney);
		}
		else if(nType == 4)
		{
			SendSysMsg("�������%u�鱦",dwMoney);
		}
	}

	return true;
}

bool CUser::SaveMoney(DWORD dwMoney, bool bSynchro /*= true*/)
{
	if (this->GetMoneyByType(1) < dwMoney)
	{
		//this->SendSysMsg(g_objStrRes.GetStr( 11086) );//STR_NOT_SO_MUCH_MONEY
		this->SendSysMsg(_TXTATR_TOPSHOW, "������û����ô������");
		return false;
	}

	DWORD dwNewMoney = this->GetMoneyByType(1) - dwMoney;
	DWORD dwNewMoneySaved = this->GetMoneySaved() + dwMoney;
	if (dwNewMoneySaved > _MAX_MONEYLIMIT)
	{
		//this->SendSysMsg(g_objStrRes.GetStr( 14042) );//STR_TOOMUCH_MONEY_SAVED
		this->SendSysMsg(_TXTATR_TOPSHOW,"�ֿ������Ѵ�����");
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
		this->SendSysMsg(_TXTATR_TOPSHOW, "�ֿ���û����ô������");
		return false;
	}

	DWORD dwNewMoney = this->GetMoneyByType(1) + dwMoney;
	DWORD dwNewMoneySaved = this->GetMoneySaved() - dwMoney;
	if (dwNewMoney > _MAX_MONEYLIMIT)
	{
		//SendSysMsg(g_objStrRes.GetStr( 14044));//STR_TOOMUCH_MONEY_TO_DRAW
		SendSysMsg(_TXTATR_TOPSHOW,"����Я�������Ѵ�����");
		return false;
	}

	GainMoney(dwMoney, SYNCHRO_FALSE, 1);
	if (dwMoney>30000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::DrawMoney user:%u(%s)�������%d",GetID(),GetName(),dwMoney);
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
	//goto �ƺ�
	CMsgUserAttrib msgAttTitle;
	msgAttTitle.Create(GetID(), _USERATTRIB_USETITLE, GetUseTitle());
	pRole->SendMsg(&msgAttTitle);

	//��������Ӫ
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
	//����������Ч
	if(m_bSoulEudemonView)
	{
		CMsgUserAttrib msgAtt;
		if(msgAtt.Create(GetID(), _USERATTRIB_EUDEMON_VIEW, m_bSoulEudemonView))
			pRole->SendMsg(&msgAtt);
	}
}

bool CUser::CanUseItem(CItem* pItem)///�ж�����Ƿ���ʹ��ĳ������
{
	return true;
//	InitScript();///��ʼ���ű�����
//	InitItemScript(pItem);
////	LuaFunction<bool> CanUse = 	LuaScriptMachine()->GetScriptFun("instance\\Instance_Item_01.lua","CanUse");
//// LuaScriptMachine()->GetState()->GetGlobal("CanUse");
////	bool hr=CanUse();
////	return true;
//	return LuaScriptMachine()->RunScriptFun("instance\\Instance_Item_01.lua","CanUse");
}

/////////////////////////////////////////////////////////////////////////////
//06.3.3�����޸�
//�������ܣ�������ҵ�״̬
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
			//20070228����
			//��ʱ����ش���
//----------07.4.10����ע�ͣ�ǰ��һ���Ǹ�ֵ
//			m_data.m_Info.i64Effect &= ~KEEPEFFECT_RED;
			//�����
			if(IsRedName())
			{
				m_data.m_Info.i64Effect |= KEEPEFFECT_RED;
			}
			else if(IsGrayName())
			{
				//�ڱ𴦴���.
				//case	STATUS_CRIME: 
			}
			else if(IsWhiteName())
			{
				//�ڱ𴦴���
				//CUser::ClsStatus...case	STATUS_PKVALUE:
			}
			else
			{
				//warning
				LOGERROR( "CUser::SetStatus��������,�û�:%s PK:%d",this->GetName(),this->GetPk() );
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
			///�д�,���������θþ�2007-07-13
//			m_data.m_Info.i64Effect	&= KEEPEFFECT_NOT_CLEAR;//�����������״̬��ԭ���о��У�ԭ���޾��ޣ���&
			m_data.m_Info.i64Effect	|= KEEPEFFECT_DIE;

//			DebuffCityWarWeapon();
			
			ClearManyStatusWhenDie();//-------���---2007/05/16-----
			
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
	case STATUS_ATTRACTMONSTER://��������
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATTRACTMONSTER;
		}
		break;
	case	STATUS_PKVALUE:         //change huang 2004.1.11
		{
			//20070228����
			//�������pk״̬
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_RED;
			
			if(IsRedName())
			{
				if(QueryStatus(STATUS_CRIME))//����л���
				{
					//�������
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
				//�ڱ𴦴���.
				//case	STATUS_CRIME: 
			}
			else if(IsWhiteName())
			{
				//�ڱ𴦴���
				//CUser::ClsStatus...case	STATUS_PKVALUE:
			}
			else
			{
				//warning
				LOGERROR( "CUser::SetStatus��������,�û�:%s PK:%d",this->GetName(),this->GetPk() );
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
	case STATUS_ENEMYFIRE:///������Ч-softworms-2007-08-13
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_ENIMYFIRE;///��ʱ������ս����Ч
		}
		break;
// 	case STATUS_SKZL:		//���ʱ��֮�ᣬ����
// 		{
// 			m_data.m_Info.i64Effect |= KEEPEFFECT_SKZL;
// 		}
// 		break;
// 	
//-------------------------------------------------------------------------------
	case STATUS_FRENZY:							//��
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_FRENZY;
		}
		break;
	case STATUS_SHIELD:							//һ��ʱ���ڿɵ���N������	����
	case STATUS_SHIELD_1:						//һ��ʱ���ڿɵ���N������	����
	case STATUS_SHIELD_2:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_SHIELD;
		}
		break;
	case STATUS_FIRMNESS_MAGIC:					//һ��ʱ�������Ӽ��ܲ�����ϵĻ���	�ᶨʩ��
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_FIRMNESS_MAGIC;
		}
		break;
	case STATUS_HIDDEN:							//����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_HIDDEN;
		}
		break;
	case STATUS_RETORT:							//����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_RETORT;
		}
		break;
	case STATUS_LURK:							//NPC�������Լ����������Ч	Ǳ��
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_LURK;
		}
		break;
	case STATUS_IMMUNE_ALL:						//����һ���˺�	�޵�
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_IMMUNE_ALL;
		}
		break;
	case STATUS_SLEEP:							//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������ɽ����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_SLEEP;
		}
		break;
	case STATUS_DREAMING:						//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������������
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DREAMING;
		}
		break;
	case STATUS_PALSY:							//�����ƶ�	�������	
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PALSY;
		}
		break;
	case STATUS_SILENCE:						//����ʹ�ü���	��Ĭ
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_SILENCE;
		}
		break;
	case STATUS_CONFUSION:						//�����ת����ʧĿ��	����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_CONFUSION;
		}
		break;
	case STATUS_STOPSAY:						//����˵�����ǿ���ϵ	����	
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_STOPSAY;
		}
		break;
	case STATUS_PHYSICALATTACKUP_PERCENT:				//������������
	case STATUS_PHYSICALATTACKUP_PERCENT_WS://2007827����:
	case STATUS_PHYATTACKUP_TUTOR_PERCENT://  [�½���10/26/2007]
	case STATUS_PHYSICALATTUP_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALATTACKUP;
		}
		break;
	case STATUS_PHYSICALATTACKDOWN_PERCENT:				//���������½�
	case STATUS_PHYSICALATTDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALATTACKDOWN;
		}
		break;
	case STATUS_MAGICATTACKUP_PERCENT:					//ħ������������
	case STATUS_MAGICATTACKUP_PERCENT_ED:
	case STATUS_MAGATTACKUP_TUTOR_PERCENT://  [�½���10/27/2007]
//	case STATUS_COMB_MAIN:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICATTACKUP;
		}
		break;
	case STATUS_MAGICATTACKDOWN_PERCENT:				//ħ���������½�
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICATTACKDOWN;
		}
		break;
	case STATUS_ATTACKUP_PERCENT:						//����������
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATTACKUP;
		}
		break;
	case STATUS_ATTACKDOWN_PERCENT:						//�������½�
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATTACKDOWN;
		}
		break;
	case STATUS_PHYSICALDEFENCEUP_PERCENT:				//�����������
	case STATUS_PHYSICALDEFENCEUP_NUMBER:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_SF://2007827����:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_L:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALDEFENCEUP;
		}
		break;
	case STATUS_PHYSICALDEFENCEDOWN_PERCENT:			//��������½�
	case STATUS_PHYSICALDEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALDEFENCEDOWN;
		}
		break;
	case STATUS_MAGICDEFENCEUP_PERCENT:					//ħ����������
	case STATUS_MAGICDEFENCEUP_NUMBER:
	case STATUS_MAGICDEFENCEUP_NUMBER_EM://2007827����:
	case STATUS_MAGICDEFENCEUP_NUMBER_L:
	case STATUS_MAGICDEFENCEUP_NUMBER_1:
		{
			m_data.m_Info.i64Effect |= 	KEEPEFFECT_MAGICDEFENCEUP;
		}
		break;
	case STATUS_MAGICDEFENCEDOWN_PERCENT:				//ħ�������½�
	case STATUS_MAGICDEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICDEFENCEDOWN;
		}
		break;
	case STATUS_DEFENCEUP_PERCENT:						//����������
	case STATUS_DEFENCEUP_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DEFENCEUP;
		}
		break;
	case STATUS_DEFENCEDOWN_PERCENT:					//�������½�
	case STATUS_DEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DEFENCEDOWN;
		}
		break;
	case STATUS_HITRATEUP_PERCENT:						//����������
	case STATUS_HITRATEUP_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_HITRATEUP;
		}
		break;
	case STATUS_HITRATEDOWN_PERCENT:					//�������½�
	case STATUS_HITRATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_HITRATEDOWN;
		}
		break;
	case STATUS_DODGERATEUP_PERCENT:					//���������
	case STATUS_DODGERATEUP_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DODGERATEUP;
		}
		break;
	case STATUS_DODGERATEDOWN_PERCENT:					//������½�
	case STATUS_DODGERATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DODGERATEDOWN;
		}
		break;
	case STATUS_CRITRATEUP_PERCENT:						//��������һ������
	case STATUS_CRITRATEUP_NUMBER:
	case STATUS_CRITRATEUP_NUMBER_SW://2007827����:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_CRITRATEUP;
		}
		break;
	case STATUS_CRITRATEDOWN_PERCENT:					//��������һ������
	case STATUS_CRITRATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_CRITRATEDOWN;
		}
		break;
	case STATUS_MAXLIFEUP_PERCENT:						//�������ֵ����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXLIFEUP;
		}
		break;
	case STATUS_MAXLIFEDOWN_PERCENT:					//�������ֵ����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXLIFEDOWN;
		}
		break;
	case STATUS_MAXMANAUP_PERCENT:						//���ħ��ֵ����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXMANAUP;
		}
		break;
	case STATUS_MAXMANADOWN_PERCENT:					//���ħ��ֵ����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXMANADOWN;
		}
		break;
	case STATUS_MAXSPUP_PERCENT:						//�������ֵ����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXSPUP;
		}
		break;
	case STATUS_MAXSPDOWN_PERCENT:						//��������ֵ����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAXSPDOWN;
		}
		break;
	case STATUS_MOVESPEEDUP_PERCENT:					//�ƶ��ٶ�����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOUNTSPEED_NUMBER:					//�ƶ��ٶ�����  ����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOVESPEEDDOWN_PERCENT:					//�ƶ��ٶȼ���
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MOVESPEEDDOWN;
		}
		break;
	case STATUS_ATKSPEEDUP_PERCENT:						//�����ٶ�����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATKSPEEDUP;
		}
		break;
	case STATUS_ATKSPEEDDOWN_PERCENT:					//�����ٶȼ���
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_ATKSPEEDDOWN;
		}
		break;
	case STATUS_PHYSICALDAMAGEUP_PERCENT:				//�����˺�����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALDAMAGEUP;
		}
		break;
	case STATUS_PHYSICALDAMAGEDOWN_PERCENT:				//�����˺���С
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_PHYSICALDAMAGEDOWN; 
		}
		break;
	case STATUS_MAGICDAMAGEUP_PERCENT:					//ħ���˺�����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICDAMAGEUP;
		}
		break;
	case STATUS_MAGICDAMAGEDOWN_PERCENT:				//ħ���˺���С
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_MAGICDAMAGEDOWN;
		}
		break;
	case STATUS_DAMAGEUP_PERCENT:						//�˺�����
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DAMAGEUP;
		}
		break;
	case STATUS_DAMAGEDOWN_PERCENT:						//�˺���С
	case STATUS_DAMAGEDOWN_PERCENT_TN://2007827����:
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
	case STATUS_GETEXPDOWN_PERCENT:						//�����ü���
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_GETEXPDOWN;
		}
		break;
// 	case STATUS_ERUPTPOT_PERCENT:						//Ǳ�ܱ���
// 		{
// 			m_data.m_Info.i64Effect |= KEEPEFFECT_ERUPTPOT;
// 		}
// 		break;
// 	case STATUS_FROSTSHIELD:								//��˪��
// 		{
// 			m_data.m_Info.i64Effect |= KEEPEFFECT_FROSTSHIELD;
// 		}
// 		break;
//------------dot begin-------------------------------------
 	case STATUS_DOT_POISONING:				//�ж�DOT			,��hp
// 	case STATUS_DOT_POISON_ATK:            // ��Ϯ [12/13/2007 �½���]
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DOT_POISONING;
		}
		break;
// 	case STATUS_DOT_ENMITY:	
	case STATUS_DOT_LIFEMISSING:			//��ѪDOT			,��hp
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DOT_LIFEMISSING;
		}
		break;
// 	case STATUS_DOT_MANAMISSING:			//������ʧDOT		,��mp
// 		{
// 			m_data.m_Info.i64Effect |=	KEEPEFFECT_DOT_MANAMISSING;
// 		}
// 		break;
// 	case STATUS_DOT_STRENGTHMISSING:		//������ʧDOT		,��sp
// 		{
// 			m_data.m_Info.i64Effect |= KEEPEFFECT_DOT_STRENGTHMISSING;
// 		}
// 		break;
 	case STATUS_DOT_BURNING:				//����DOT			,��hp
		{
			m_data.m_Info.i64Effect |= KEEPEFFECT_DOT_BURNING;
		}
		break;
//-----------dot end------------------------------------
//SECOND STATUS BEGIN---------------------------------------------------
	case STATUS_CRAZE://����
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_CRAZE;
		}
		break;
	case STATUS_PVPMODE://����buff
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
	case STATUS_DOT_SUFFERING://����DOT
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_SUFFERING;
		}
		break;
// 	case STATUS_DOT_BEATING://���DOT
// 	case STATUS_DOT_BEATINGPRE:// �������2 [12/12/2007 �½���]
// 		{
// 			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_BEATING;
// 		}
// 		break;
	case STATUS_DETECTION://ʶ��
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_DETECTION;

		}
		break;
	case STATUS_RESOURCEWAR_PROTECT://��Դս����״̬
		{
			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_RESOURCEWAR_PROTECT;

		}
		break;
 	case STATUS_DOT_ADDLIFE://��Ѫdot
// 	case STATUS_DOT_ADDLIFE_RB://2007827����:
// 	case STATUS_DOT_QUICK_ADDLIFE:// ���ٻָ� [12/25/2007 �½���]
// 	case STATUS_DOT_ADDLIFE_L:
// 		{
// 			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_DOT_ADDLIFE;
// 		}
// 		break;
	case STATUS_DOT_ADDMANA://����DOT
// 	case STATUS_DOT_ADDMANA_MC://2007827����:
// 	case STATUS_DOT_ADDMANA_L:
// 		{
// 			m_data.m_Info.i64Effect2 |= SECONDKEEPEFFECT_DOT_ADDMANA;
// 
// 		}
// 		break;
		//  [1/30/2008 �½���]
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

	case STATUS_FISH_FOOD:        //��Щ״̬������Ч
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
	case STATUS_SPENDMP_UP://����������
	case STATUS_STAUP_NUMBER://��������
	case STATUS_STADOWN_NUMBER://��������
	case STATUS_SPIUP_NUMBER://���Ӿ���
	case STATUS_SPIDOWN_NUMBER://���پ���
	case STATUS_AGIUP_NUMBER://��������
	case STATUS_AGIDOWN_NUMBER://��������
	case STATUS_TOUCHUP_NUMBER://���Ӽ���
	case STATUS_TOUGHDOWN_NUMBER://���ټ���
	case STATUS_SELF_DAMAGEUP_NUMBER://����Ե��˺�����
	case STATUS_INTUP_NUMBER://��������
	case STATUS_MAGIC_SHIELD://ħ���ܣ��ܵ��˺�ʱ�������������˿�Ѫ
	case STATUS_FOURATTUP_NUMBER://����������
	case STATUS_FOURATTDOWN_NUMBER://�����Լ���
	case STATUS_BASEATTUP_NUMBER://����������
	case STATUS_SELF_DAMAGEUP_PERCENT://����Ե��˺�����
		{
			
		}
		break;
	case STATUS_EUDEMON_SOUL://���︽��
	case STATUS_STRENGUP_NUMBER_PET://��������
	case STATUS_STRENGDOWN_NUMBER_PET://��������
	case STATUS_SPIUP_NUMBER_PET://���Ӿ���
	case STATUS_SPIDOWN_NUMBER_PET://���پ���
	case STATUS_AGIUP_NUMBER_PET://��������
	case STATUS_AGIDOWN_NUMBER_PET://��������
	case STATUS_STAUP_NUMBER_PET://��������
	case STATUS_STADOWN_NUMBER_PET://��������
	case STATUS_INTUP_NUMBER_PET://��������
	case STATUS_INTDOWN_NUMBER_PET://��������
	case STATUS_BOOTH_ADDEXP:
	case STATUS_ACTIVATE_ADDEXP:
	case STATUS_DOT_SYNEXP:
	case STATUS_DOT_TUTOREXP:
	case STATUS_DOT_WARGAMEEXP:
	case STATUS_ITEM_ADDEXP:
	case STATUS_DOT_LIFEMISS1:
	case STATUS_MONSTER_ENMITY:
	case STATUS_DEFDOWN_ATKUP:
	case STATUS_EQUIP_DEFUP://���������	������װ��
	case STATUS_EQUIP_DAMAGEUP://�Ե��˺�����  ������װ��
	case STATUS_EQUIP_CRIUP://��������		������װ��
	case STATUS_EQUIP_BEDAMAGEUP://�ܵ��˺�����	������װ��
	case STATUS_EQUIP_DEFDOWN://���������	������װ��
	case STATUS_DOT_REDMANA://����DOT
	case STATUS_DOT_REDSP://��ŭ��DOT
	case STATUS_BIZ_DEAD:// �����������buff
	case STATUS_FIREDEF_UP://���
	case STATUS_ICEDEF_UP://����
	case STATUS_MYSTICDEF_UP://����
	case STATUS_NOXDEF_UP://����
	case STATUS_SYN_HITRATEDOWN://���н���
	case STATUS_SYN_SHIELD://һ��ʱ���������˺�	����
//	case STATUS_SYNWORKTIME://���ɴ�buff
	case STATUS_SOUL_CRI://����			���
	case STATUS_SOUL_HITRATE://����			���
	case STATUS_SOUL_DODGE://����			���
	case STATUS_SOUL_INNERATT://�ڹ�			���
	case STATUS_SOUL_OUTERATT://�⹦			���
	case STATUS_SOUL_INNERDEF://�ڷ�			���
	case STATUS_SOUL_OUTERDEF://���			���
	case STATUS_SOUL_REBLOOD://��Ѫ			���
	case STATUS_SOUL_REBLOOD_PER:
	case STATUS_SOUL_REMP://��ħ			���
	case STATUS_TUTOR_BOSS://ʦͽ��������������Ҫ��Buff
//modify code by �ֵ�Ԩ begin 2011.6.22
	case STATUS_RETORT_TENSEC:
	case STATUS_RETORT_LGWS:
	case STATUS_RETORT_ZHF:
	case STATUS_RETORT_MDSX:
	case STATUS_RETORT_MDSX_BUFF2:
	case STATUS_RETORT_MDSX_1:
//modify code by �ֵ�Ԩ end
	case STATUS_PET_DODGE://���︽�������
	case STATUS_PET_HITRATE://���︽�������
	case STATUS_DOT_ADDEXP://���

		{
			
		}
		break;
//--------------END-----------------------------------------------------
	default:
		::LogSave("Invalid status: [%u]", nStatus);
	}

	if (STATUS_DREAMING == nStatus || STATUS_SLEEP == nStatus || STATUS_CONFUSION == nStatus )
	{
		this->ClrAttackTarget();	// ֹͣ�Զ�ս��
	}
	if( STATUS_FRENZY == nStatus || STATUS_ATKSPEEDUP_PERCENT == nStatus || STATUS_ATKSPEEDDOWN_PERCENT == nStatus )
	{
		this->CalcFightRate();
	}
	if(bSynchro && (i64OldEffect != GetEffect() || i64OldEffect2 != GetSecondEffect()))///����i64OldEffect2 != GetSecondEffect()��չ�ж�,softworms-2007-08-14,���Ұ�.
	{
		CMsgUserAttrib msg;
		if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
		{

			if ( STATUS_MOVESPEEDUP_PERCENT == nStatus || STATUS_MOVESPEEDDOWN_PERCENT == nStatus || STATUS_FRENZY == nStatus || STATUS_MOUNTSPEED_NUMBER == nStatus || STATUS_SKZL == nStatus )
			{                                                                                                                        // �����ٶ� [�½���8/2/2007]
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
			//  [1/30/2008 �½���]
			else if( STATUS_LIFE_UP == nStatus )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			//----------------------------------------------
			// �¼��ܲ�����־׼�� [12/25/2007 �½���]
			else if( STATUS_FORTITUDE_WILL == nStatus )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			//----------------------------------------------


			//����second effect
			if( i64OldEffect2 != GetSecondEffect() )
			{
				msg.Append( _USERATTRIB_SECONDEFFECTLOW32,this->GetSecondEffect() );
			}

			BroadcastRoomMsg(&msg, INCLUDE_SELF,EM_SENDNPCFLAG_FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//06.3.3�����޸�
//�������ܣ������ҵ�״̬
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
			m_data.m_Info.dwStatus = STATUS_NORMAL;//20070825����:�������״̬ʱ,����ֵ��Ϊnormal
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
	case STATUS_ATTRACTMONSTER://��������
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATTRACTMONSTER;
		}
		break;
	case	STATUS_PKVALUE:        // change huang 2004.1.11  todo
		{
			//20070228����
			//��ʱ����ش���
			//�����
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_RED;
			if(IsRedName())
			{
				m_data.m_Info.i64Effect |= KEEPEFFECT_RED;
			}
			else if(IsGrayName())
			{
				//�ڱ𴦴���.
				//case	STATUS_CRIME: 
			}
			else if(IsWhiteName())
			{
				//�ڱ𴦴���
				//CUser::ClsStatus...case	STATUS_PKVALUE:
			}
			else
			{
				//warning
				LOGERROR( "CUser::SetStatus��������,�û�:%s PK:%d",this->GetName(),this->GetPk() );
			}


			// ȡ������״̬����������������
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
	case STATUS_ENEMYFIRE:///������Ч-softworms-2007-08-13
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_ENIMYFIRE;///��ʱ������ս����Ч
		}
		break;
// 	case STATUS_SKZL:		//���ʱ��֮�ᣬ��������
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_SKZL;
// 		}
// 		break;
	
//-------------------------------------------------------------------------------
	case STATUS_FRENZY:							//��
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_FRENZY;
		}
		break;
	case STATUS_SHIELD:							//һ��ʱ���ڿɵ���N������	����
	case STATUS_SHIELD_1:						//һ��ʱ���ڿɵ���N������	����
	case STATUS_SHIELD_2:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_SHIELD;
		}
		break;
	case STATUS_FIRMNESS_MAGIC:					//һ��ʱ�������Ӽ��ܲ�����ϵĻ���	�ᶨʩ��
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_FIRMNESS_MAGIC;
		}
		break;
	case STATUS_HIDDEN:							//����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_HIDDEN;
		}
		break;
	case STATUS_RETORT:							//����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_RETORT;
		}
		break;
	case STATUS_LURK:							//NPC�������Լ����������Ч	Ǳ��
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_LURK;
		}
		break;
	case STATUS_IMMUNE_ALL:						//����һ���˺�	�޵�
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_IMMUNE_ALL;
		}
		break;
	case STATUS_SLEEP:							//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������ɽ����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_SLEEP;
		}
		break;
	case STATUS_DREAMING:						//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������������
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DREAMING;
		}
		break;
	case STATUS_PALSY:							//�����ƶ�	�������	
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PALSY;
		}
		break;
	case STATUS_SILENCE:						//����ʹ�ü���	��Ĭ
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_SILENCE;
		}
		break;
	case STATUS_CONFUSION:						//�����ת����ʧĿ��	����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_CONFUSION;
		}
		break;
	case STATUS_STOPSAY:						//����˵�����ǿ���ϵ	����	
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_STOPSAY;
		}
		break;
	case STATUS_PHYSICALATTACKUP_PERCENT:				//������������
	case STATUS_PHYSICALATTACKUP_PERCENT_WS://2007827����:
	case STATUS_PHYATTACKUP_TUTOR_PERCENT://  [�½���10/26/2007]
	case STATUS_PHYSICALATTUP_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALATTACKUP;
		}
		break;
	case STATUS_PHYSICALATTACKDOWN_PERCENT:				//���������½�
	case STATUS_PHYSICALATTDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALATTACKDOWN;
		}
		break;
	case STATUS_MAGICATTACKUP_PERCENT:					//ħ������������
	case STATUS_MAGICATTACKUP_PERCENT_ED:
	case STATUS_MAGATTACKUP_TUTOR_PERCENT://  [�½���10/27/2007]
//	case STATUS_COMB_MAIN:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICATTACKUP;
		}
		break;
	case STATUS_MAGICATTACKDOWN_PERCENT:				//ħ���������½�
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICATTACKDOWN;
		}
		break;
	case STATUS_ATTACKUP_PERCENT:						//����������
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATTACKUP;
		}
		break;
	case STATUS_ATTACKDOWN_PERCENT:						//�������½�
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATTACKDOWN;
		}
		break;
	case STATUS_PHYSICALDEFENCEUP_PERCENT:				//�����������
	case STATUS_PHYSICALDEFENCEUP_NUMBER:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_SF://2007827����:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_L:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALDEFENCEUP;
		}
		break;
	case STATUS_PHYSICALDEFENCEDOWN_PERCENT:			//��������½�
	case STATUS_PHYSICALDEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALDEFENCEDOWN;
		}
		break;
	case STATUS_MAGICDEFENCEUP_PERCENT:					//ħ����������
	case STATUS_MAGICDEFENCEUP_NUMBER:
	case STATUS_MAGICDEFENCEUP_NUMBER_EM:
	case STATUS_MAGICDEFENCEUP_NUMBER_L:
	case STATUS_MAGICDEFENCEUP_NUMBER_1:
		{
			m_data.m_Info.i64Effect &= 	~KEEPEFFECT_MAGICDEFENCEUP;
		}
		break;
	case STATUS_MAGICDEFENCEDOWN_PERCENT:				//ħ�������½�
	case STATUS_MAGICDEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICDEFENCEDOWN;
		}
		break;
	case STATUS_DEFENCEUP_PERCENT:						//����������
	case STATUS_DEFENCEUP_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DEFENCEUP;
		}
		break;
	case STATUS_DEFENCEDOWN_PERCENT:					//�������½�
	case STATUS_DEFENCEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DEFENCEDOWN;
		}
		break;
	case STATUS_HITRATEUP_PERCENT:						//����������
	case STATUS_HITRATEUP_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_HITRATEUP;
		}
		break;
	case STATUS_HITRATEDOWN_PERCENT:					//�������½�
	case STATUS_HITRATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_HITRATEDOWN;
		}
		break;
	case STATUS_DODGERATEUP_PERCENT:					//���������
	case STATUS_DODGERATEUP_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DODGERATEUP;
		}
		break;
	case STATUS_DODGERATEDOWN_PERCENT:					//������½�
	case STATUS_DODGERATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DODGERATEDOWN;
		}
		break;
	case STATUS_CRITRATEUP_PERCENT:						//��������һ������
	case STATUS_CRITRATEUP_NUMBER:
	case STATUS_CRITRATEUP_NUMBER_SW://2007827����:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_CRITRATEUP;
		}
		break;
	case STATUS_CRITRATEDOWN_PERCENT:					//��������һ������
	case STATUS_CRITRATEDOWN_NUMBER:
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_CRITRATEDOWN;
		}
		break;
	case STATUS_MAXLIFEUP_PERCENT:						//�������ֵ����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXLIFEUP;
		}
		break;
	case STATUS_MAXLIFEDOWN_PERCENT:					//�������ֵ����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXLIFEDOWN;
		}
		break;
	case STATUS_MAXMANAUP_PERCENT:						//���ħ��ֵ����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXMANAUP;
		}
		break;
	case STATUS_MAXMANADOWN_PERCENT:					//���ħ��ֵ����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXMANADOWN;
		}
		break;
	case STATUS_MAXSPUP_PERCENT:						//�������ֵ����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXSPUP;
		}
		break;
	case STATUS_MAXSPDOWN_PERCENT:						//��������ֵ����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAXSPDOWN;
		}
		break;
	case STATUS_MOVESPEEDUP_PERCENT:					//�ƶ��ٶ�����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOUNTSPEED_NUMBER:					//�ƶ��ٶ����� ����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOVESPEEDDOWN_PERCENT:					//�ƶ��ٶȼ���
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MOVESPEEDDOWN;
		}
		break;
	case STATUS_ATKSPEEDUP_PERCENT:						//�����ٶ�����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATKSPEEDUP;
		}
		break;
	case STATUS_ATKSPEEDDOWN_PERCENT:					//�����ٶȼ���
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ATKSPEEDDOWN;
		}
		break;
	case STATUS_PHYSICALDAMAGEUP_PERCENT:				//�����˺�����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALDAMAGEUP;
		}
		break;
	case STATUS_PHYSICALDAMAGEDOWN_PERCENT:				//�����˺���С
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_PHYSICALDAMAGEDOWN; 
		}
		break;
	case STATUS_MAGICDAMAGEUP_PERCENT:					//ħ���˺�����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICDAMAGEUP;
		}
		break;
	case STATUS_MAGICDAMAGEDOWN_PERCENT:				//ħ���˺���С
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_MAGICDAMAGEDOWN;
		}
		break;
	case STATUS_DAMAGEUP_PERCENT:						//�˺�����
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DAMAGEUP;
		}
		break;
	case STATUS_DAMAGEDOWN_PERCENT:						//�˺���С
	case STATUS_DAMAGEDOWN_PERCENT_TN://2007827����:
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
	case STATUS_GETEXPDOWN_PERCENT:						//�����ü���
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_GETEXPDOWN;
		}
		break;
// 	case STATUS_ERUPTPOT_PERCENT:						//Ǳ�ܱ���
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_ERUPTPOT;
// 		}
// 		break;
// 	case STATUS_FROSTSHIELD:								//��˪��
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_FROSTSHIELD;
// 		}
// 		break;

//------------dot begin-------------------------------------
 	case STATUS_DOT_POISONING:				//�ж�DOT			,��hp
// 	case STATUS_DOT_POISON_ATK:            // ��Ϯ [12/13/2007 �½���]
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_POISONING;
		}
		break;
// 	case STATUS_DOT_ENMITY:
	case STATUS_DOT_LIFEMISSING:			//��ѪDOT			,��hp
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_LIFEMISSING;
		}
		break;
// 	case STATUS_DOT_MANAMISSING:			//������ʧDOT		,��mp
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_MANAMISSING;
// 		}
// 		break;
// 	case STATUS_DOT_STRENGTHMISSING:		//������ʧDOT		,��sp
// 		{
// 			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_STRENGTHMISSING;
// 		}
// 		break;
 	case STATUS_DOT_BURNING:				//����DOT			,��hp
		{
			m_data.m_Info.i64Effect &= ~KEEPEFFECT_DOT_BURNING;
		}
		break;
//-----------dot end------------------------------------
//SECOND STATUS BEGIN---------------------------------------------------
	case STATUS_CRAZE://����
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_CRAZE;
		}
		break;
	case STATUS_PVPMODE://����buff
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
	case STATUS_DOT_SUFFERING://����DOT
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_SUFFERING;
		}
		break;
// 	case STATUS_DOT_BEATING://���DOT
// 	case STATUS_DOT_BEATINGPRE:// �������2 [12/12/2007 �½���]
// 		{
// 			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_BEATING;
// 		}
// 		break;
	case STATUS_DETECTION://ʶ��
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_DETECTION;

		}
		break;
	case STATUS_RESOURCEWAR_PROTECT://��Դս����״̬
		{
			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_RESOURCEWAR_PROTECT;

		}
		break;
 	case STATUS_DOT_ADDLIFE://��Ѫdot
// 	case STATUS_DOT_ADDLIFE_RB://2007827����:
// 	case STATUS_DOT_QUICK_ADDLIFE:// ���ٻָ� [12/25/2007 �½���]
// 	case STATUS_DOT_ADDLIFE_L:
// 		{
// 			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_DOT_ADDLIFE;
// 		}
// 		break;
	case STATUS_DOT_ADDMANA://����DOT
// 	case STATUS_DOT_ADDMANA_MC://2007827����:
// 	case STATUS_DOT_ADDMANA_L:
// 		{
// 			m_data.m_Info.i64Effect2 &= ~SECONDKEEPEFFECT_DOT_ADDMANA;
// 
// 		}
// 		break;
		//  [1/30/2008 �½���]
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

	case STATUS_FISH_FOOD:        //��Щ״̬������Ч
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
	case STATUS_SPENDMP_UP://����������
	case STATUS_STAUP_NUMBER://��������
	case STATUS_STADOWN_NUMBER://��������
	case STATUS_SPIUP_NUMBER://���Ӿ���
	case STATUS_SPIDOWN_NUMBER://���پ���
	case STATUS_AGIUP_NUMBER://��������
	case STATUS_AGIDOWN_NUMBER://��������
	case STATUS_TOUCHUP_NUMBER://���Ӽ���
	case STATUS_TOUGHDOWN_NUMBER://���ټ���
	case STATUS_SELF_DAMAGEUP_NUMBER://����Ե��˺�����
	case STATUS_INTUP_NUMBER://��������
	case STATUS_MAGIC_SHIELD://ħ���ܣ��ܵ��˺�ʱ�������������˿�Ѫ
	case STATUS_FOURATTUP_NUMBER://����������
	case STATUS_FOURATTDOWN_NUMBER://�����Լ���
	case STATUS_BASEATTUP_NUMBER://����������
	case STATUS_SELF_DAMAGEUP_PERCENT://����Ե��˺�����
		{
			
		}
		break;
	case STATUS_EUDEMON_SOUL://���︽��
	case STATUS_STRENGUP_NUMBER_PET://��������
	case STATUS_STRENGDOWN_NUMBER_PET://��������
	case STATUS_SPIUP_NUMBER_PET://���Ӿ���
	case STATUS_SPIDOWN_NUMBER_PET://���پ���
	case STATUS_AGIUP_NUMBER_PET://��������
	case STATUS_AGIDOWN_NUMBER_PET://��������
	case STATUS_STAUP_NUMBER_PET://��������
	case STATUS_STADOWN_NUMBER_PET://��������
	case STATUS_INTUP_NUMBER_PET://��������
	case STATUS_INTDOWN_NUMBER_PET://��������
	case STATUS_BOOTH_ADDEXP:
	case STATUS_ACTIVATE_ADDEXP:
	case STATUS_DOT_TUTOREXP:
	case STATUS_DOT_SYNEXP:
	case STATUS_DOT_WARGAMEEXP:
	case STATUS_ITEM_ADDEXP:
	case STATUS_DOT_LIFEMISS1:
	case STATUS_MONSTER_ENMITY:
	case STATUS_DEFDOWN_ATKUP:
	case STATUS_EQUIP_DEFUP://���������	������װ��
	case STATUS_EQUIP_DAMAGEUP://�Ե��˺�����  ������װ��
	case STATUS_EQUIP_CRIUP://��������		������װ��
	case STATUS_EQUIP_BEDAMAGEUP://�ܵ��˺�����	������װ��
	case STATUS_EQUIP_DEFDOWN://���������	������װ��
	case STATUS_DOT_REDMANA://����DOT
	case STATUS_DOT_REDSP://��ŭ��DOT
	case STATUS_BIZ_DEAD:// �����������buff
	case STATUS_FIREDEF_UP://���
	case STATUS_ICEDEF_UP://����
	case STATUS_MYSTICDEF_UP://����
	case STATUS_NOXDEF_UP://����
	case STATUS_SYN_HITRATEDOWN://���н���
	case STATUS_SYN_SHIELD://һ��ʱ���������˺�	����
//	case STATUS_SYNWORKTIME://���ɴ�buff
	case STATUS_SOUL_CRI://����			���
	case STATUS_SOUL_HITRATE://����			���
	case STATUS_SOUL_DODGE://����			���
	case STATUS_SOUL_INNERATT://�ڹ�			���
	case STATUS_SOUL_OUTERATT://�⹦			���
	case STATUS_SOUL_INNERDEF://�ڷ�			���
	case STATUS_SOUL_OUTERDEF://���			���
	case STATUS_SOUL_REBLOOD://��Ѫ			���
	case STATUS_SOUL_REBLOOD_PER:
	case STATUS_SOUL_REMP://��ħ			���
	case STATUS_TUTOR_BOSS://ʦͽ��������������Ҫ��Buff
//modify code by �ֵ�Ԩ begin 2011.6.22
	case STATUS_RETORT_TENSEC:
	case STATUS_RETORT_LGWS:
	case STATUS_RETORT_ZHF:
	case STATUS_RETORT_MDSX:
	case STATUS_RETORT_MDSX_BUFF2:
	case STATUS_RETORT_MDSX_1:
//modify code by �ֵ�Ԩ end
	case STATUS_PET_DODGE://���︽�������
	case STATUS_PET_HITRATE://���︽�������
	case  STATUS_DOT_ADDEXP://���

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
	if(bSynchro && (i64OldEffect != GetEffect() || i64OldEffect2 != GetSecondEffect()))///����i64OldEffect2 != GetSecondEffect()��չ�ж�,softworms-2007-08-14,���Ұ�.
	{

		CMsgUserAttrib msg;
		if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
		{
			if ( STATUS_MOVESPEEDUP_PERCENT == nStatus || STATUS_MOVESPEEDDOWN_PERCENT == nStatus || STATUS_FRENZY == nStatus || STATUS_MOUNTSPEED_NUMBER == nStatus || STATUS_SKZL == nStatus )
			{                                                                                                                       // �����ٶ� [�½���8/2/2007]
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
			else if( STATUS_LIFE_UP == nStatus )//  [1/31/2008 �½���]
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			// �¼��ܲ�����־׼�� [12/25/2007 �½���]
			else if( STATUS_FORTITUDE_WILL == nStatus )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			//----------------------------------------------


			//����second effect
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
	//���¼��ؼ���//�Ż�������ÿ��תְ��ͬ��һ�Σ�ֻ������ʱ������ְҵ�ļ��ܶ�����ȥ
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
	if (GetMountState())//������
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
//[2010-06-08 goto]�޸����飬��֪ͨ����
void CUser::SetFeel(char* szFeel)
{
	m_data.SetFeel(szFeel);//Ϊ�˷�ֹ��������ʱ��������Ȼ���޸�ǰ�����飬��ͬ�����ݿ���Ϊtrue
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
	//���ʹ��� satan******************************
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
	return true;//[2010-06-26 goto]������赲
//����֮���Ƿ���Դ�͸......
//-------------------------------����ע�����²��޸�------------------------------------
/*	return true;		//? �ݲ���� /////////////////////////////////////////////////////////////

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
	//û�п��ǵ�nSize Ϊ��ʱ,nSize-1��������Խ��.ǰ����if( nSize <=2 ) retun
//--------------------------------------------end---------------------
	for(int i = 0; i < nSize; i++)
	{
		if( GetMap()->IsStandEnable( setLine[ i ].x,setLine[ i ].y ) )
			continue;
		this->SendSysMsg( g_objStrRes.GetStr( 15064) );//"����ȡ��,�赲!"
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
// 	if( nWeaponRate != 0 )//������������������Ĺ���Ƶ��
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
	//�ȿ���buff
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
int	CUser::AdjustUserAttrib(DWORD nAttrib,int nAttribType)///����������ħ��ֵ,softworms-2007-09-05
{
	IStatus* pStatus = NULL;
	int nAttAddi = 0;
	switch(nAttribType)
	{
	case ADJUSTUSERATTRIB_PHYDEF:///���(���)
		{
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEUP_PERCENT );//�����������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEDOWN_PERCENT );//��������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEUP_NUMBER );//�������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEDOWN_NUMBER );//��������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DEFDOWN_ATKUP );//�Է�������������
			if (pStatus)
			{
				nAttAddi += (-2)*CRole::CalcStatusInfluence( STATUS_DEFDOWN_ATKUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFUP );//�������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFDOWN );//��������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFDOWN,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_OUTERDEF );//���������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_OUTERDEF,nAttrib,pStatus->GetPower() );
			}

		}
		break;
	case ADJUSTUSERATTRIB_MAXMANA:///�����ֵ
		{
			pStatus = QueryStatus( STATUS_MAXMANAUP_PERCENT );//���ħ��ֵ����%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXMANAUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAXMANADOWN_PERCENT );//���ħ��ֵ����%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXMANADOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAXLIFE:///�������ֵ
		{
			pStatus = QueryStatus( STATUS_MAXLIFEUP_PERCENT );//�������ֵ����%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXLIFEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAXLIFEDOWN_PERCENT );//�������ֵ����%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXLIFEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAGDEF:///ħ��(�ڷ�)
		{
			pStatus = QueryStatus( STATUS_MAGICDEFENCEUP_PERCENT );//ħ����������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEDOWN_PERCENT );//ħ�������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEUP_NUMBER );//ħ������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEUP_NUMBER_1 );//ħ������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEUP_NUMBER_1,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEDOWN_NUMBER );//ħ�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFUP );//ħ������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFDOWN );//ħ�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFDOWN,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_INNERDEF );//�ڷ���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_INNERDEF,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_HITRANGE:///������
		{
			pStatus = QueryStatus( STATUS_HITRATEUP_PERCENT );//����������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_HITRATEDOWN_PERCENT );//�������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_HITRATEUP_NUMBER );//������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PET_HITRATE );//������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PET_HITRATE,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_HITRATEDOWN_NUMBER );//�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SYN_HITRATEDOWN );//�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SYN_HITRATEDOWN,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_HITRATE );//�����ʣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_HITRATE,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_DOG:///����
		{
			pStatus = QueryStatus( STATUS_DODGERATEUP_PERCENT );//����������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DODGERATEDOWN_PERCENT );//�������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DODGERATEUP_NUMBER );//������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PET_DODGE );//������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PET_DODGE,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DODGERATEDOWN_NUMBER );//�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_DODGE );//�����ʣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_DODGE,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_PHYACK:///�﹥(�⹦)
		{
			pStatus = QueryStatus( STATUS_PHYSICALATTACKUP_PERCENT );//������������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTACKUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALATTACKDOWN_PERCENT );//���������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTACKDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALATTUP_NUMBER );//���������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALATTDOWN_NUMBER );//���������٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DEFDOWN_ATKUP );//�Է�������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DEFDOWN_ATKUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_OUTERATT );//�⹦��������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_OUTERATT,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAGACK:///ħ��(�ڹ�)
		{
			pStatus = QueryStatus( STATUS_MAGICATTACKUP_PERCENT );//ħ������������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICATTACKUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICATTACKDOWN_PERCENT );//ħ���������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICATTACKDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_INNERATT );//�ڹ�
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_INNERATT,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_Crit:///����
		{
			pStatus = QueryStatus( STATUS_CRITRATEUP_PERCENT );//��������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_CRITRATEDOWN_PERCENT );//�����½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_CRITRATEUP_NUMBER );//�������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_CRITRATEDOWN_NUMBER );//�������٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_CRIUP );//�������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_CRIUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_CRI );//������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_CRI,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_STR://��������
		{
			pStatus = QueryStatus( STATUS_STRENGUP_NUMBER );//�������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STRENGDOWN_NUMBER );//�������٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STRENGUP_NUMBER_PET );//�������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STRENGDOWN_NUMBER_PET );//�������٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus(STATUS_EUDEMON_SOUL);//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_INT://����(����)
		{
			pStatus = QueryStatus( STATUS_INTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_INTDOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_INTUP_NUMBER_PET );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_INTDOWN_NUMBER_PET );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_STA://����
		{
			pStatus = QueryStatus( STATUS_STAUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STAUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STADOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STADOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STAUP_NUMBER_PET );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STAUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STADOWN_NUMBER_PET );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STADOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STAUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_SPI://����
		{
			pStatus = QueryStatus( STATUS_SPIUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SPIDOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SPIUP_NUMBER_PET );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SPIDOWN_NUMBER_PET );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}

			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_AGI://����
		{
			pStatus = QueryStatus( STATUS_AGIUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_AGIDOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_AGIUP_NUMBER_PET );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_AGIDOWN_NUMBER_PET );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_TOUGH://����
		{
			pStatus = QueryStatus( STATUS_TOUCHUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_TOUCHUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_TOUGHDOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_TOUGHDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EUDEMON_SOUL );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_TOUCHUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_FIREATK://��
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_ICEATK://��
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MYSTICATK://��
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_NOXATK://��
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_FIREDEF://��
		{
			pStatus = QueryStatus( STATUS_FIREDEF_UP );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_ICEDEF://��
		{
			pStatus = QueryStatus( STATUS_ICEDEF_UP );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MYSTICDEF://��
		{
			pStatus = QueryStatus( STATUS_MYSTICDEF_UP );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_NOXDEF://��
		{
			pStatus = QueryStatus( STATUS_NOXDEF_UP );//���ӣ�������
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
//������ҵ��ƶ��ٶ�
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
	//��״̬,�����ƶ��ٶ�
	pStatus = QueryStatus( STATUS_FRENZY );
	if( pStatus )//
	{
		nChangeData += CRole::CalcStatusInfluence( STATUS_FRENZY,nSpeed,pStatus->GetParam1() );	
	}
	// �����ٶ� [�½���8/2/2007]
	pStatus = QueryStatus( STATUS_MOUNTSPEED_NUMBER ); 
	if( pStatus )
	{
		nChangeData += CRole::CalcStatusInfluence( STATUS_MOUNTSPEED_NUMBER,nSpeed,pStatus->GetPower() );	
	}

	//��ս�����ٶ�
	pStatus = QueryStatus( STATUS_CITYWAR_WEAPON);
	if(pStatus)
	{
		return pStatus->GetPower();
	}

	nSpeed += nChangeData;

	//nSpeed = __min( 1000,__max( 1,nSpeed ) );
	nSpeed = __min( 150,__max( 1,nSpeed ) );//goto ���150
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
		//20070416����:�����ʵ�����
	case REMOTE_CALL_SYNPAYOFF:
		{
			int nMoney = pInfo->IntParam[0];//��������
			AddMoney(nMoney,true);//ͬ��,����
			this->SendSysMsg(_TXTATR_SYNDICATE,g_objStrRes.GetStr( 15065),nMoney);//�����˾��Ź���:%d"
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

			int MemberAmount[7]={0,40,80,120,160,200,240};//20070807����:�߻���
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
				sprintf(szMsg,"%s���������Ϊ����İ�����ż!", this->GetName());
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
			SendSysMsg("���������Ѿ�������");
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
		SendSysMsg("���������Ѿ�������");
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
		SendSysMsg("��ս����ĳ��ﲻ�ܽ��д˲���");
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
		SendSysMsg("�ϳɳɹ�");
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
		SendSysMsg("�ϳ�ʧ��");
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
		DetachStatusFromEudemon(idEudemon);//���ﱻ�ٻػ�����ʱȥ����������˼ӵ�״̬
	}

	CRole::DetachAllStatus(m_pWarPet->QueryRole());//UpdateMonsterProperty(false);//�����ջ�ʱҪ�����������buff,��Ҫ����һ�³����������
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
		//SendSysMsg("������У����ܳ�ս");
		return NULL;
	}

	CEudemonTypeData*	pTypeData = EudemonType()->GetObj(pData->GetInt(EUDEMONDATA_typeid));
	if(!pTypeData)
		return NULL;

	EudemonData eudemonData;
	LoadEudemon(idEudemon,eudemonData,3);

	if(pData->GetInt(EUDEMONDATA_lifespan) <= 0)
	{
		SendSysMsg("���������Ѿ����꣬���ܳ�ս");
		return NULL;
	}

	if(pData->GetInt(EUDEMONDATA_happy) < 60)
	{
		SendSysMsg("�������ֵ���㣬���ܳ�ս");
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

bool CUser::CreateEudemon(OBJID idEudemonType,bool bSaveData,EudemonData* pData, bool born,int nBornGrow,bool bCompensation/* = false*/)//bSaveData==1 ��ʾ��pData����  �����Ǵ����ݿ��ж�ȡ
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
//modify code by �ֵ�Ԩ begin 2011.5.24
	if( pData->grow > 20 || pData->understand > 0 )
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get pet %u(type:%u) create. grow(%u) understand(%u) level(%u) sex(%u) exp(%u)", GetID(), GetName(), pData->id, pData->typeId, pData->grow, pData->understand, pData->level, pData->sex, pData->exp);
//modify code by �ֵ�Ԩ end
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
			SendSysMsg("�������㣬�޷���ȡ");
			return false;
		}
		if(!SpendMoney(dwMoney,true))
		{
			SendSysMsg("�������㣬�޷���ȡ");
			return false;
		}
	}

	if(dwItemAmount > 0)
	{
		DWORD dwHasItem = GetItemNumByType(630014);
		if(dwHasItem < dwHasItem)
		{
			SendSysMsg("��ң�����㣬�޷���ȡ");
			return false;
		}

		bool bHasKind = false;
		if(!DelItemByTypeMulti(630014,dwItemAmount,bHasKind))
		{
			SendSysMsg("��ң�����㣬�޷���ȡ");
			return false;
		}
	}

	DWORD dwGetExp = AwardBattleExp(dwExp);
	if(dwGetExp > 0)
	{
		m_data.SetLeaveExp(/*dwHasTime % 60*/0);
		SendSysMsg("������%u����",dwGetExp);
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
			SendSysMsg("û�г���ҩƷ���벹��");
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
			SendSysMsg("û�г���ҩƷ���벹��");
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
			SendSysMsg("û�г�����ߣ��벹��");
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
//modify code by �ֵ�Ԩ begin 2011.5.24
		int datalevel = pData->GetInt(EUDEMONDATA_level);
		int datasex = pData->GetInt(EUDEMONDATA_sex);
		int dataexp = pData->GetInt(EUDEMONDATA_exp);
//modify code by �ֵ�Ԩ end
		OBJID idType = pData->GetInt(EUDEMONDATA_typeid);

		int nGrow = pData->GetInt(EUDEMONDATA_grow);
		int nUnderStand = pData->GetInt(EUDEMONDATA_understand);

		if(IsLockedEudemon(idEudemon))
			return false;

		if (!bIgnoreLock)//���԰�ȫ��,���罻�����ʱ��
		{
			if (IsItemLockedOpen())
			{
				SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
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
//modify code by �ֵ�Ԩ begin 2011.5.24
		if(nGrow>20 || nUnderStand>0)
		{
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost pet %u(type:%d) from giveup. grow(%u) understand(%u) level(%u) sex(%u) exp(%u)", GetID(), GetName(), idEudemon, idType, nGrow, nUnderStand, datalevel, datasex, dataexp);
		}
//modify code by �ֵ�Ԩ end
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
		SendSysMsg("���ַǷ�");
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
		SendSysMsg("�����ɹ�");


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

//���ﱻ�ٻػ�����ʱȥ����������˼ӵ�״̬
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
	//06.10.25����ע�ʹ˺���,��������:����ս���õ��ļ�ĳЩ��������
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
//��ʱע��.
//		IF_NOT(pMap->IsStandEnable(dwX, dwY))
//			return;
		if(CWargame::IsWarGameMap(idMap))//ר�ߵľ�����������
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
void CUser::SetSpecilSteate(SPECILLINESTATE nState ,bool bUpdate /* = false */)//ע����ȷ���м�¼
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

	//ͨ���ȼ�֮��������ﾭ��
	nHeroExp = CBattleSystem::AdjustExp( nHeroExp, this->GetLev(), pTarget->GetLev() );

	int nChangeData = 0;
	IStatus* pStatus;
	pStatus = this->QueryStatus( STATUS_GETEXPUP_PERCENT );
	if (pStatus)
		nChangeData += CRole::CalcStatusInfluence( STATUS_GETEXPUP_PERCENT,nHeroExp,pStatus->GetPower() );
	
	pStatus = this->QueryStatus(STATUS_GETEXPUP_PERCENT_SYNINS);//���ǰ��ɸ����ڵļӾ���buff
	if (pStatus)
	{
		if (GetMap()->GetDocID()== 2017)//ֻ�ڸõ�ͼ��˫��
			nChangeData += CRole::CalcStatusInfluence( STATUS_GETEXPUP_PERCENT_SYNINS,nHeroExp+nChangeData,pStatus->GetPower() );
	}
	//VIP�Ӿ���
	if(IsVipActive())
	{
		int nVipLev = GetVipLev()==4 ? 1 : GetVipLev();//�ܿ�Ҫ���¿�Ч��һ��
		nChangeData += nHeroExp * ( 0.10f + nVipLev *0.10f);
	}

	return nHeroExp + nChangeData;
}


int CUser::AdjustEudemonExp(IRole* pTarget,CMonster* pWarPet, int nRawExp, bool bNewbieBonusMsg)
{
	CHECKF(pTarget);
	int nHeroExp = 0;
	
	nHeroExp = nRawExp;
	
	//ͨ���ȼ�֮��������ﾭ��
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
		if (pRole->GetMap()->IsPkGameMap())	//����ڱ��䳡
		{
			return false;
		}
		if (pUser->GetPVPStuts())//�����PK
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
				if (pPet->QueryOwnerUser()->GetPVPStuts())//�����PK
				{
					return false;
				}
				if (pPet->QueryOwnerUser()->GetMap()->IsPkGameMap())	//����ڱ��䳡
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

	//20070316����:��ҹ����ж�ͳһ����.����IsAtkable()
	//������
	//�п���IsAtkable()������౻���ù�һ��.�˴��ǵڶ��α�����.
	return !IsAtkable(pRole);

	//����������������
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
//		if(this->GetPhyle() == PROF_DRAGON && strstr(pMonster->GetName(),"����") != 0)
//			return true;
//		else if(this->GetPhyle() == PROF_HUMAN && strstr(pMonster->GetName(),"������") != 0)
//			return true;
//		else if(this->GetPhyle() == PROF_GENE && strstr(pMonster->GetName(),"������") != 0)
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
//		case	PKMODE_TEAM:    //change huang 2004.1.11 (�����ʿ��ģʽ) 
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
		|| QueryStatus(STATUS_NEW_PK_PROTECT)	//  [2/22/2008 ���]	�µ�PK����
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
	//����ע�ͣ�����ѫ��..
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
/*-------------------------------����ע��---------end,����δ֪-------------------------------------*/
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
bool CUser::IsItemFull(int nWeight, OBJID idItemType, DWORD dwData)
{
	// �����Ǹ���

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
				if(pItem->GetInt(ITEMDATA_AMOUNT) + pInfo->nAmount <= pInfo->nAmountLimit)	// ���Ժϲ�
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
//ͨ��ͽ�����Ӵ��ڶ�
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
//ͨ��ʦ�����Ӵ��ڶ�
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
//ͽ�ܲ�ѯ���ڶ�
// int	CUser::GetImpartByStudent()
// {
// // 	if (!m_pTutor)
// // 		return 0;
// // 	return m_pTutor->GetImpart();
// 	CHECKF(m_pUserTutor);
// 	return m_pUserTutor->GetImpartByStudent();
// }
//ʦ����ѯ���ڶ�
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
// 		if(NULL != m_pTutor)     //��ʦ��
// 		{
// 			if(m_pTutor->GetTutorID() == idMember)
// 				nTutorStudentNum++;
// 		}
// 		
// 	    if(!m_setStudent.empty())  //��ͽ��
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
//goto ��ʦ ͽ�ܵ���
// bool CUser::AddTutor(OBJID idTutor, LPCTSTR szName, int nPro)
// {
// 	CHECKF(szName);
// 	//��Щ�ж϶��ŵ��ű���ȥ
// // 	if (m_pTutor && !m_pTutor->IsGraduated())
// // 	{
// // 		SendSysMsg(_TXTATR_TOPSHOW, "�㵱ǰ�Ѿ���ʦ���ˣ������ٰ�����Ϊʦ");
// // 		return false;
// // 	}
// 	CUser* pTarget = UserManager()->GetUser(idTutor);
// 	if (!pTarget)//��ʦ��ʱ��ʦ��һ������
// 		return false;
// // 	if (pTarget->GetTutor() && !pTarget->GetTutor()->IsGraduated())
// // 	{
// // 		SendSysMsg(_TXTATR_TOPSHOW, "�Է���δ��ʦ��������ͽ");
// // 		return false;
// // 	}
// // 	if(pTarget->IsStudentFull())
// // 	{
// // 		SendSysMsg(_TXTATR_TOPSHOW, "�Է���ͽ�������Ѵﵽ����");
// // 		pTarget->SendSysMsg(_TXTATR_TOPSHOW, "���ͽ�������Ѿ��ﵽ����");
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
// 				m_data.SetTutorMask(1);//�����Ƿ��һ�ΰ�ʦ��ʶ
// 			//֪ͨ˫��
// 			//CMsgSchoolMember msg;
// 			//if (msg.Create(GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0))//֪ͨͽ�����ʦ��
// 			{
// 				//msg.Append(RELATION_TEACHER, 0, pTarget);
// 				//SendMsg(&msg);
// 				char szTemp[128];
// 				sprintf(szTemp,"��ϲ���%sΪʦ",szName);
// 				SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 			}
// 			//if (msg.Create(idTutor, MESSAGEBOARD_ADDMEMBER, NULL, 0))//֪ͨʦ�����ͽ��
// 			{
// 				//msg.Append(RELATION_STUDENT, 0, this);
// 				//pTarget->SendMsg(&msg);
// 				char szTemp[128];
// 				sprintf(szTemp,"��ϲ����%sΪͽ",GetName());
// 				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 			}
// 			//if(!pTarget->IsStudentFull())
// 			pTarget->AddStudentRef(m_pTutor->GetInfo());
// // 			else
// // 			{
// // 				// TODO: ͨ���������������MapGroup��Ѱ�Ҳ�֪ͨ
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
//ͽ�ܵ������ʦͽ��ϵ��ֻ��ͽ�ܵ���
// bool CUser::DelTutor()
// {
// 	if (!m_pTutor)
// 		return false;
// 	if (m_pTutor->IsGraduated())
// 		return false;
// 	char szTemp[128];
// 	sprintf(szTemp, "�Ѿ��ɹ���%s���ʦͽ��ϵ", m_pTutor->GetTutorName());
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
// 	// ����ֻ��ɾ����ʦ�������ݿ��¼����Ӧ��֪ͨʦ��ɾ���ڴ����ݡ������ʦ�����ߵĻ�
// 	CUser* pUser = UserManager()->GetUser(idTutor);
// 	if (pUser)
// 	{
// 		pUser->DelStudentRef(GetID());//ͬʱ��ʦ������Ϣ
// 	}
// 	else
// 	{
// // 		// ֪ͨ����MapGroupѰ��ʦ����ɾ��ͽ��
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
// 	this->SendSysMsg(szTemp); //%s��%s��ʦͽ��ϵ�ָ�

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
// 		SendSysMsg(g_objStrRes.GetStr(16087));//"����ʦ���������ʦͽ��ϵ��������룡"
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
{//Ϊ�������ֹ��������򹫸����������ѧԱ�ĵȼ��͡�LW
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
//ʦ����ͽʱ����ͽ�ܵĳ�ʼ�������룬��ͽ�ܰ�ʦ֮ǰ�ĵȼ�����������Ϊ������
DWORD CUser::CalcTutorGiftMask(int nLev)
{
	//����ĵ�һλ��15���Ľ���,�ڶ�λ��20��...�Դ�����
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
//ʦͽ�������� �Ѽ���
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
	if (pItem->GetInt(ITEMDATA_BASE1) + nAmount >= pItem->GetInt(ITEMDATA_BASE2))//����
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
//��ȡ������ָ���ȼ���Χ��ͽ������
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
			if (pData)//��ͽ��
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
//�������Ƿ�����ͬIP��ͽ��
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
			if (pData)//��ͽ��
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
int	CUser::GetTeamStuAvgLev()//������ͽ�ܵ�ƽ���ȼ�
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
			if (pData)//��ͽ��
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
int	CUser::GetStuLevAddedInTeam()//�����е�ͽ����ʦ�����������ļ���
{
	if(!GetTeam())
		return 0;

	if (m_pTeam->GetMemberAmount()>2)//ֻ�����˶���
		return 0;

	OBJID idTarget = m_pTeam->GetTeamMemberOnly();
	
	CTutorData* pData = GetStudent(idTarget);
	if(!pData)
		return 0;

	return pData->GetInt(TUTORDATA_ADDEDLEV);
}
//goto type:0 δ��ʦͽ�ܣ�1 �ѳ�ʦͽ�ܣ�2 ȫ��
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
// 				SendSysMsg(g_objStrRes.GetStr(16088));//"����ͽ���������ʦͽ��ϵ��������룡"
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
// // 				this->SendSysMsg(g_objStrRes.GetStr(28312)); //����ָ�
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

//Э����ʦͽ��ϵ�������ɣ�ֻ��ʦ������
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
// // 			if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))//֪ͨʦ��
// // 			{
// // 				if (msg.Append(RELATION_STUDENT, 0, pStudent))
// // 					SendMsg(&msg);
// // 			}
// 
// 			if (pStudent->DelTutorRef())//ͬʱ��ͽ�ܷ���Ϣ
// 				return true;
// 		}
// 	}
// 	
// 	return false;
// }
//ʦ���������ʦͽ��ϵ��ֻ��ʦ������
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
// 			sprintf(szTemp, "���Ѿ��ɹ���%s���ʦ��", szName);
// 			SAFE_DELETE (pTutor);
// 			m_setStudent.erase(it);
// 			
// // 			CMsgSchoolMember	msg;
// // 			if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))//֪ͨʦ��
// // 			{
// // 				if (msg.Append(idStudent, RELATION_STUDENT, nImpart, szName, pTutor->GetStuLev()))
// // 					SendMsg(&msg);
// // 			}
// 			CUser* pStudent = UserManager()->GetUser(idStudent);
// 			if (pStudent)
// 			{
// 				pStudent->DelTutorRef();//ͬʱ��ͽ�ܷ���Ϣ
// 			}
// 			else
// 			{
// 				// ֪ͨ����MapGroupѰ��ͽ�ܲ�ɾ��ʦ��
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
// DWORD CUser::GetTutorMask(OBJID idStudent)//��ȡʦ����������
// {
// 	if (!m_pUserTutor)
// 		return 0;
// 	return m_pUserTutor->GetTutorGiftMask(idStudent);
// }
// void CUser::SetTutorMask(OBJID idStudent, DWORD dwMask)//����ʦ����������
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
// // 		this->SendSysMsg(g_objStrRes.GetStr(28312)); //����ָ�
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
// // 				this->SendSysMsg(g_objStrRes.GetStr(28312)); //����ָ�
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
// // 		this->SendSysMsg(g_objStrRes.GetStr(28312)); //����ָ�
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
// 	sprintf(szSQL, "SELECT a.*,b.level as 'stu_lev',%d as 'tutor_lev',0 as 'tutor_lv' FROM %s a,%s b WHERE tutor_id=%u AND a.user_id=b.id order by a.id", GetLev(), _TBL_TUTOR,_TBL_USER, GetID());//goto ͽ��������ʱû���ޣ���δ��ʦ��ͽ��ֻ��������
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
// 		sprintf(szMsg,"����δ��ȡ��ʦͽ�������뼰ʱ��ʦͽNPC����ȡ��");
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
// 	vector<ST_MEMBERINFO> vecMemUnGra;//δ��ʦ��
// 	vector<ST_MEMBERINFO> vecMemGra;//�ѳ�ʦ��
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
// 		if (pUser)//����Է����ߣ���ȡʵʱ����
// 		{
// 			infotutor.nLev = pUser->GetLev();
// 			infotutor.nTutorLev = pUser->GetTutorLevel();
// 		}
// 		else
// 		{
// 			infotutor.nLev = m_pTutor->GetTutorLev();//ʦ���ȼ�
// 			infotutor.nTutorLev = m_pTutor->GetTutorImpartLv();//ʦ��
// 		}
// 		infotutor.nGiftMask = m_pTutor->GetGiftMask();
// 		if (!m_pTutor->IsGraduated())//��ʦ��Ͳ�����
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
// 			if (pUser)//����Է����ߣ���ȡʵʱ����
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
// 	//goto ʦͽ�б��ҳ����
// 	if (m_setStudent.size()>0 && vecMemInfo.empty())//�����ͽ�ܣ�vecMemInfo��һ���ǿյ�
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
// 		int nPageBegin = (nPage-1)*4;//ҳ���Ǵ�1��ʼ��
// 		if (nStuAmount<nPageBegin)
// 			return;
// 
// 		it = vecMemInfoTemp.begin();
// 		it += nPageBegin;//ָ��ǰҳ�ĵ�һ��
// 		vector<ST_MEMBERINFO>::iterator itEnd = it+4;//ÿҳ���4��
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
// // 				msg.m_pInfo->usStuCount = 1;//�б��Ա�������ͻ��˷�ҳ��
// // 			else
// // 				msg.m_pInfo->usStuCount = m_setStudent.size();
// // 			this->SendMsg(&msg);
// // 		}
// 		if(msg.Create(GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0))
// 		{
// 			if (m_pTutor)
// 				msg.m_pInfo->usStuCount = 1;//�б��Ա�������ͻ��˷�ҳ��
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
// 		//SendSysMsg(_TXTATR_TOPSHOW, "��û��ʦ����ͽ��");
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
		// ���ͽ�ܹ��׵ľ���
		int nExp = pExpData->GetInt(TUTOREXPDATA_EXP);
		if (nExpTake > nExp)
			return false;

		// ���ݵ�ʦ�ȼ��Լ���ȡ�õĹ��׾���������ĵĵ�ʦ����
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
		this->AwardTutorExp(-1*nCostTutorExp);		// �ȿ۳���ʦ����
		pExpData->SetInt(TUTOREXPDATA_EXP, nExp - nExpTake);
		this->AwardBattleExp(nExpTake);			// �õ����׾���

		// ��ʦ���ݾ���
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
// 	// ֪ͨ�Լ����е�ʦͽ�Լ�Ŀǰ״̬
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

//�Ƿ��ǰ��ɰ���
bool CUser::IsSynLeader()
{
	return GetSynRank() == RANK_LEADER;

}
//20070416����:֧�ֿ�ѡ��ͬ��,����
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

//�õ�����½ʱ��
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

//�Ƿ���Ƕ��װ���ı�ʯ������
bool CUser::IsEmbedGemType(int nGemType)
{
	for(int i=ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		CItemPtr pItem = GetEquipItemByPos(i);
		if(pItem)
		{
//����ע��..
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
		if (QueryMagic()->IsInLaunch())	// ħ��ʩչ������
			return false;
		QueryMagic()->AbortMagic();
	}

	OBJID idMap = this->GetMapID();
	idMap = (idMap/1000)*1000; //����ͼID

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
//�õ������õİ��ɻ���
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
// 		//�жϾ��ŵȼ�
// 		int nRank = GetSynRank();
// 		if (nRank==SYNRANK_ONE || nRank==SYNRANK_TWO)
// 		{
// 			return false;
// 		}
// 		//����Ƿ��Ѿ���������ս��
// 		if(pNpc->GetData(SYNWARLETTER_ISATTACK)>0)
// 		{
// 			SendSysMsg(g_objStrRes.GetStr( 10030));//STR_SYN_APPLYATTACKSYN_HASBEENAPPLIED
// 			return false;
// 		}
// 
// 		if(pNpc->GetData(SYNWARLETTER_OCCUPYTIME)==0)	//û�б�ռ��
// 		{
// 			//��¼ռ���ʱ��---begin
// 			pNpc->GetMap()->SetSynOccupyTime();
// 			//��¼ռ���ʱ��---end
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
// 		//����˵�ͼ�ļ�ֵ
// 		int nSynMapValue = pSynMap->GetSynMapValue();
// 
// 		CSyndicate * pOwnerSyn = SynManager()->QuerySyndicate(pSynMap->GetSynID());
// 		
// 		//�ж��������Ƿ����Ҫ��
// 		CSyndicate * pSyn = GetSyndicate();
// 		if(pSyn->GetMoney() < nSynMapValue)//�ʽ𲻹�
// 		{
// 			SendSysMsg(g_objStrRes.GetStr( 10029));//STR_SYN_APPLYATTACKSYN_FAIL
// 			return false;
// 		}
// 		//д��ս�����Ϣ
// 		pNpc->SetData(SYNWARLETTER_ISATTACK,1,true);
// 		pNpc->SetData(SYNWARLETTER_ATTACK_SYNID,pSyn->GetID(),true);
// 		pNpc->SetData(SYNWARLETTER_DEPOSIT,nSynMapValue,true);
// 		//�۳�����Ѻ��
// 		pSyn->QueryModify()->AddData(SYNDATA_MONEY,-nSynMapValue);
// 
// 		//����ս��ʱ��
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

//��WorldKernel�������������Ϣ
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
		if (QueryMagic()->IsInLaunch())	// ħ��ʩչ������
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
	
	// �����޸�   08.6.12  ��� 
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
//�������ҽ���...
//�������,1��֮��ֻ���������
//2,���ܴ���127�ַ�....
void CUser::ChangeMyDesc( char* desc )
{
	if( !desc )
	{
		this->SendSysMsg( g_objStrRes.GetStr( 15066) );//"����д�������ҽ���!(60������)"
		return;
	}
	if( strlen( desc ) >= 128 )
	{
		this->SendSysMsg( g_objStrRes.GetStr( 10000 ) );
	//	this->SendSysMsg( "���������������,����ʧ��!" );
		return;
	}
	DWORD dwTimeLimit = 1000 * 60 * 60 * 8;//8Сʱ..
	//�ͻ����ȼ��ʱ����,�����ҲҪ���..
	DWORD dwLastTime = /*this->GetLastChangeDescTime()*/0; 
	if( 0 == dwLastTime )
	{
		//��ʾ��һ�θ��ģ����ж�ʱ��
	}
	else
	{
		if( ::GetTickCount() - dwLastTime <= dwTimeLimit )
		{
			this->SendSysMsg( g_objStrRes.GetStr( 10001 ) );
			//this->SendSysMsg( "���ҽ������ָ��ļ��Ϊ8��Сʱ!" );
			return;
		}
	}
	//���·�������ҽ����������
	DWORD dwNowTime = ::GetTickCount();
	this->SetDescription( desc );
//	this->SetLastChangeDescTime( dwNowTime );
	//ͬ���ͻ���..
	CMsgName msg;
	if( msg.Create( NAMEACT_CHANGE_DESC,this->GetName(),dwNowTime ) )
		this->SendMsg( &msg );

}

//��˱�   �����
//���˵�   �����
//��˻�   ������
//��1 ��2 ��3
//���������˺�.
int CUser::AdjustAttribDamage( int nPower, CMonster *pTarget )
{
	return nPower;
}


//////////////////////////////////////////////
//Ѫ�ⱦʯ	
//�ɷ���ʯ	
//������ʯ
//�����ֱ�ʯ������������ʱʹ�Է�����ĳ��״̬
void CUser::UseWeaponWithGem( IRole *pTarget )
{

//07.4.10������ע��	
//	int nDecHpTime,nDecMpTime,nDecSpTime,nDecHpPoint,nDecMpPoint,nDecSpPoint;			//ǰ���ִ����͵�����
//	nDecHpTime = nDecMpTime = nDecSpTime = nDecHpPoint = nDecMpPoint = nDecSpPoint = 0;
//	int nDecDefPower,nDecAtkPower,nDecSpeedPower,nDecDefTime,nDecAtkTime,nDecSpeedTime;	//�����������ͳ���ʱ��
//	nDecDefPower = nDecAtkPower = nDecSpeedPower = nDecDefTime = nDecAtkTime = nDecSpeedTime = 0;
//	if( m_pWeaponR && m_pWeaponR->IsCanUpLevel() )//�ֳֿ���������
//	{		
//		srand( ( unsigned int )time( NULL ) );
//		for( int i = 1;i <= 6;i++ )
//		{
//			int nResult = m_pWeaponR->GetGemStatus( i );
//			if( nResult <= 0 || nResult >= 255 )	//û����Ƕ��ʯ�������Ҳ������.
//				break;
//			//20070326����:���ǵ�
//			//CItemAdditionData* pAddition = ItemAddition()->QueryItemAddition( OBJID_GEM_BEGIN + nResult );
//			CItemAdditionData* pAddition = CItem::GetItemAddition( OBJID_GEM_BEGIN + nResult );
//			if( pAddition )
//			{
//				int nSort = pAddition->GetInt( ITEMADDITIONDATA_SORT );	
//				if( GEMSORT_TAG_DECHP == nSort )	//��Ѫ
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//����
//					if( rand() % 100 < nPossibility )
//					{
//						nDecHpTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); //����
//						nDecHpPoint += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//����
//					}
//				}
//				else if( GEMSORT_TAG_DECMP == nSort && ROLE_USER == pTarget->GetRoleType() ) //��ħ��
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//����
//					if( rand() % 100 < nPossibility )
//					{
//						nDecMpTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); //����
//						nDecMpPoint += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//����
//					}
//				}
//				else if( GEMSORT_TAG_DECSP == nSort && ROLE_USER == pTarget->GetRoleType() )//������
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//����
//					if( rand() % 100 < nPossibility )
//					{
//						nDecSpTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); //����
//						nDecSpPoint += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//����
//					}
//				}
//				//���������Ƕ��״̬CMoreStatus,������һ��״̬�����г���ʱ��
//				else if( GEMSORT_TAG_DECDEFENCE == nSort )//Ŀ�꽵��
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//����
//					if( rand() % 100 < nPossibility )
//					{
//						nDecDefTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); // ʱ��,��λ��
//						nDecDefPower += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//����
//					}
//				}
//				else if( GEMSORT_TAG_DECATTACK == nSort )//Ŀ�꽵��
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//����
//					if( rand() % 100 < nPossibility )
//					{
//						nDecAtkTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); // ʱ��,��λ��
//						nDecAtkPower += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//����
//					}
//					
//				}
//				else if( GEMSORT_TAG_DECSPEED == nSort )//���Ŀ�������,�����ܶ��ٶ�,����ǹ���,�����ƶ��ٶ�
//				{
//					int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//����
//					if( rand() % 100 < nPossibility )
//					{
//						nDecSpeedTime	+= pAddition->GetInt( ITEMADDITIONDATA_TIME ); // ʱ��,��λ��
//						nDecSpeedPower += pAddition->GetInt( ITEMADDITIONDATA_VALUE );//����
//					}
//				}
//
//			}	
//		}//end for
//		//�Խ�����д���
//		//���״̬����������
//		int interval = 4;//���ʱ��4��
//#ifdef _DEBUG
//		char szBuff[ 128 ] = { 0 };
//#endif
//		if( nDecHpTime > 0 && nDecHpPoint > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECHP, nDecHpPoint, interval, nDecHpTime );
//#ifdef _DEBUG
//			sprintf( szBuff,"Ŀ��:%s,����״̬:%d�ε�Ѫ,ÿ��%d��,���ʱ��%d��",pTarget->GetName(),nDecHpTime,nDecHpPoint,interval );
//			this->SendSysMsg( szBuff );
//#endif
//		}
//		if( nDecMpTime > 0 && nDecMpPoint > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECMP, nDecMpPoint, interval, nDecMpTime );	
//#ifdef _DEBUG
//			sprintf( szBuff,"Ŀ��:%s,����״̬:%d�ε�ħ,ÿ��%d��,���ʱ��%d��",pTarget->GetName(),nDecMpTime,nDecMpPoint,interval );
//			this->SendSysMsg( szBuff );
//#endif
//
//		}
//		if( nDecSpTime > 0 && nDecSpPoint > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECSP, nDecSpPoint, interval, nDecSpTime );	
//#ifdef _DEUBG
//			sprintf( szBuff,"Ŀ��:%s,����״̬:%d�ε�����,ÿ��%d��,���ʱ��%d��",pTarget->GetName(),nDecSpTime,nDecSpPoint,interval );
//			this->SendSysMsg( szBuff );
//#endif
//		}
//		///һ��״̬��������ʱ��
//		
//		if( nDecDefPower > 0 && nDecDefTime > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECDEFENCE,nDecDefPower,nDecDefTime,0,0,0 );
//#ifdef _DEBUG
//			sprintf( szBuff,"Ŀ��:%s,����״̬:%d���ڷ�������%d%%",pTarget->GetName(),nDecDefTime,nDecDefPower );
//			this->SendSysMsg( szBuff );
//#endif
//
//		}
//		if( nDecAtkPower > 0 && nDecAtkTime > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECATTACK,nDecAtkPower,nDecAtkTime,0,0,0 );
//#ifdef _DEBUG
//			sprintf( szBuff,"Ŀ��:%s,����״̬:%d���ڹ���������%d%%",pTarget->GetName(),nDecAtkTime,nDecAtkPower );
//			this->SendSysMsg( szBuff );
//#endif
//		}
//		if( nDecSpeedPower > 0  && nDecSpeedTime > 0 )
//		{
//			CRole::AttachStatus( pTarget,STATUS_DECSPEED,nDecSpeedPower,nDecSpeedTime,0,0,0 );
//#ifdef _DEBUG
//			sprintf( szBuff,"Ŀ��:%s,����״̬:%d�����ٶȼ���%d%%",pTarget->GetName(),nDecSpeedTime,nDecSpeedPower );
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
//goto ֻ��ʦ������
bool CUser::IsTutorTeam()//�����Ա�Ƿ���ʦͽ��ϵ
{
	if (!m_pUserTutor)
		return false;
	CTeam* pTeam = GetTeam();
	if (!pTeam)
		return false;
	if (pTeam->GetMemberAmount()>2)//ֻ��ʦͽ�������
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
//goto ͽ�ܵ���
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
// // 		if (!pTutor->IsTutorTeam())//�����ж�,���ɽű����ж�
// // 			return false;
// 		CTutor *pStudent = pTutor->GetStudent(GetID());
// 		if (!pStudent)
// 			return false;
// 		GetTutor()->SetGraduated(1,Database());//��Ϊ��ʦ״̬
// 		pStudent->SetGraduated(1,Database(), false);//ʦ�����ڴ��е�����ҲҪ�޸�һ��
// 		//ͬ���ͻ���
// // 		CMsgSchoolMember msg;
// // 		if (msg.Create(GetID(), MESSAGEBOARD_DELMEMBER, NULL, 0))//֪ͨͽ��
// // 		{
// // 			msg.Append(RELATION_TEACHER, 0, pTutor);
// // 			SendMsg(&msg);
// // 		}
// // 		if (msg.Create(pTutor->GetID(), MESSAGEBOARD_UPDATEMEMBER, NULL, 0))//֪ͨʦ��
// // 		{
// // 			msg.Append(RELATION_STUDENT_GRADUATE, 0, this);
// // 			pTutor->SendMsg(&msg);
// // 		}
// 		pTutor->SendSysMsg("���Ѿ��ɹ������˳�ʦ��ʽ�����Դ�ʦͽ������ȡ����");
// 		char GraduateMsg[255];
// 		sprintf(GraduateMsg, "��ϲ%s��%s�Ľ̵��³ɹ���ʦ��" ,this->GetName(),pTutor->GetName());//"��ϲ%s��%s��Ϥ�Ľ̵���˳����ʦ��"
// 		UserManager()->BroadcastMsg(GraduateMsg, NULL, NULL, 0xffffff, _TXTATR_SERVE);
// // 		this->DelTutor();
// 	}
// 	else
// 	{
// 		SendSysMsg(_TXTATR_TOPSHOW, "ʦͽ����̫Զ"); //�Է��Ѳ�����ĸ�����
// 		return false;
// 	}
// 	return true;
// }
//  [7/7/2007]
//	���񲨵�һ���޸ģ����Ӳ���
bool CUser::IsOwnMap( int nMapID )	//�ǲ����ڱ����ͼ
{
	int mapSort = nMapID / 1000;

//	if(mapSort == 4)
//		return true;
	// �����޸�   08.6.12  ��� 
//	if(GetPhyle() == PROF_DRAGON)
//		return mapSort == 3;
//	if(GetPhyle() == PROF_HUMAN)
//		return mapSort == 2;
//	if(GetPhyle() == PROF_GENE)
//		return mapSort == 1;

	return false;

}
//  [6/18/2007]
//	�������
//	�ǲ��ǹ�����ͼ
//  [7/7/2007],���񲨵�һ���޸ģ����Ӳ���	
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
		//���и�����
		SendSysMsg("���и�����");
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
//����
//����˵����nStatus ״̬��ʶ
//			nTime	����ʱ������Ǽ��ʱ��
//			nTimes	oneStatus��0,moreStatusʱ��ʾ����
//			nPower,nParam1---		����	
//07.4.28���񲨵�һ���޸ģ����ӷ�BUFF��DEBUFF��DOT�ж�				
void CUser::AddBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )
{
	if( nStatus != ID_NONE )
	{
		int nBigType = nStatus / 10000;
		if( BIGTYPE_OTHER == nBigType )//��BUFF��DEBUFF��DOT
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
			UpdateCalculatedProperty(UPDATECALTYPE_BUFF);//[2010-07-07 goto]����buff����������������
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
//�����޸�
//07.4.28���񲨵ڶ����޸ģ����ӷ�BUFF��DEBUFF��DOT�ж�				
void CUser::UpdateBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )
{
	if( nStatus != ID_NONE )
	{
		int nBigType = nStatus / 10000;
		if( BIGTYPE_OTHER == nBigType )//��BUFF��DEBUFF��DOT
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
			UpdateCalculatedProperty(UPDATECALTYPE_BUFF);//[2010-07-07 goto]����buff����������������
	}

}
//-------------------------------------------------------------------------------------------------------------------------
//07.4.26
//�����޸�
//07.4.28���񲨵ڶ����޸ģ����ӷ�BUFF��DEBUFF��DOT�ж�				
void CUser::ClsBuff( int nStatus ) 
{
	if( nStatus != ID_NONE )
	{
		int nBigType = nStatus / 10000;
		if( BIGTYPE_OTHER == nBigType )//��BUFF��DEBUFF��DOT
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
			UpdateCalculatedProperty(UPDATECALTYPE_BUFF);//[2010-07-07 goto]ɾ��buff����������������
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

BOOL CUser::GetStorageLimit()///�Ƿ�ͨ����ֵ���ѽ�������
{
	return /*m_data.GetStorageLimit()*/10;
}

void CUser::SetStorageLimit(int nStoragelimit)///������ֵ������ʶ
{
	//m_data.SetStorageLimit(nStoragelimit);
}

// BOOL CUser::GetSetMagicStoneBoxLimit()///�Ƿ�ͨ����ֵ���ѽ�������
// {
// 	return m_data.GetMagicStoneBoxLimit();
// }

// void CUser::SetMagicStoneBoxLimit(int nMagicStoneBoxLimit)///������ֵ������ʶ
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
//����
//������ҵ���������
//����:����,������:��������
//����ʱ����
//06.11.7�����޸Ļ���,��д�˺���
//void CUser::ProcessTalent()
//{
//
//	if( m_bDragonTalentSkill )//ÿ��������һ������
//	{
//		DWORD dwLevel	= GetLev();
//		const int ADDHEALTH_INTERVAL_LEVEL	= 3;//ÿ���ټ�����
//		const int ADDHEALTH_POINT			= 1;//��������
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
	if( dwProfess & PROF_DRAGON )//����
	{
		const int MAGIC_TYPE = 2001;//���꼼�ܵ�TYPE
		const int ADDHEALTH_INTERVAL_LEVEL = 3;//ÿ���ټ�����
		const int ADDHEALTH_POINT			= 1;//��������
		if( pMagic->FindMagic( MAGIC_TYPE ) )
		{
			if( 0 == dwLevel % ADDHEALTH_INTERVAL_LEVEL )
			{
				AddAttrib( _USERATTRIB_HEALTH,ADDHEALTH_POINT,SYNCHRO_TRUE );
			}
		}
		
	}
	else if( dwProfess & PROF_HUMAN )//������
	{
		const int MAGIC_TYPE = 2002;//�������ݵ�TYPE
		if( pMagic->FindMagic( MAGIC_TYPE ) )
		{
			if( dwLevel >= 20 )
			{
				if( 0 == GetAddHitRatePercent() )
				{
					SetAddHitRatePercent( 1 );//�������м�1%(100% full)
				}
			}
			if( dwLevel >= 40 )
			{
				if( 0 == GetAddDodgePercent() )
				{
					SetAddDodgePercent( 1 );//�������ܼ�1
				}	
			}
			if( dwLevel >= 60 )
			{
				if( 0 == GetAddCriticalHitPercent() )
				{
					SetAddCriticalHitPercent( 1 );	//��������һ�����ʼ�1
				}
			}

		}
		
	}
	else
	{}*/

//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.18
//����
//�ж��Ƿ��ܵõ�����Ļ�,���ʵ����ɷ��ڴ˴�
// bool CUser::CanGetSpirit()
// {
// 	//Ŀǰ����Ϊ30%
// #ifdef _DEBUG
// 	int probability = 100;
// #else
// 	int probability = 30;
// #endif
// 	//�л��ʵĸı���Coding here
// 	if( ::RandGet( 100 ) < probability )
// 		return true;
// 	return false;
// }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.13
//����
//���ﱻɱ����,��������Ҽӻ�
//δ����,Ҳδ�ж��Ƿ��ܵõ�
//06.12.18���񲨵�һ���޸�,���ӻ����жϺ͵ȼ�����
// bool CUser::AwardSpirit( IRole* pTarget,int nNum )
// {
// 		if( !pTarget )
// 		return false;
// 	//20070113����:��ұ���Ҫ��[�ǳ��ǻ�]������,�ܻ�������.------
// 	//�����ǳ��ǻ۵��ж�
// 	//if(!QueryMagic()->FindMagic(8889))//û��[�ǳ��ǻ�]����
// 	//	return false;
// 	//-----------------------------------------------------------
// 	if( !CanGetSpirit() )
// 		return false;//������ִ��
// 
// 	//��ʼ�ȼ�����
// 	//10000��ʾ100%
// 	// ���monsterType monsterSpirit�ֶ���400
// 	//	a)	����ҵȼ����ڹ���ȼ�10�����ϣ�����10��������ҽ����ܵõ����
// 	//	b)	����ҵȼ����ڹ���ȼ�7-9��(����7,9)����ҽ��õ�80���			//  20%
// 	//	c)	����ҵȼ����ڹ���ȼ�4-6��(����4,6)����ҽ��õ�240���			//	60%
// 	//	d)	����ҵȼ����ڹ���ȼ�0-3������ҽ��õ�400���,��400/10000*100%( ������ҵȼ�С�ڹ���ȼ�)//	100%
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
// 	//�����ȼ�����
// 	//������ЧCoding here------------
// // GM���ߵ�������ֵ���� [9/12/2007 Add By Halei ]
// //	if (::GetTickCount() < g_adjustTime[_ADJUSTRATE_SOUL_RANGE] )
// 	{
// 		float SpiritRange = g_adjustRate.Get_AdjustRate(_ADJUSTRATE_SOUL_RANGE);///g_adjustRate.m_mAdjustRateInfo[_ADJUSTRATE_SOUL_RANGE]->adjustRate;
// 		SpiritRange = SpiritRange/100.f;//����ɱ���
// 		if (SpiritRange>0.0f && SpiritRange<4.0f)//���ʲ��ɴ���4
// 		{
// 			nNum = nNum*SpiritRange;//���������ϱ���
// 		}		
// 	}
// 
// // GM���ߵ�������ֵ���� end [9/12/2007 Add By Halei ]
// 
// 		//20061222����:��Ч
// 	CMsgMagicEffect	msg;
// 	IF_OK(msg.Create(pTarget->GetID(), //����
// 		8888, //magicType
// 		0, //magicLev.Ϊ1����
// 		this->GetID(),//user.Ŀ��
// 		nNum,//dwData
// 		(this->GetDir()+4)%8,BATTLEADDINFO::INFO_INVALID))//���﷽��ķ�����
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
// 	///������2008-04-17-�������������
// 	//UINT nMaxSoulLimit=g_adjustRate.Get_AdjustRate(_ADJUSTRATE_SOUL_LIMIT,_ADJUSTRATE_ADJUSTRATE);
// 	
// 	UINT nMaxSoulLimit = GetMaxSoulAmount();//���������  guijia  2008.5 .5
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
// 	int nOldSpiritGridNum	= this->GetSpiritGridNum();		//������������
// 	int nOldSpiritPercent	= this->GetSpiritPercent();		//�������İٷֱ�
// 
// 	CHECKF( nOldSpiritPercent < SPIRITPERCENT2GRID );
// 	
// 	int nOldSpiritValue		= SPIRITPERCENT2GRID * nOldSpiritGridNum + nOldSpiritPercent;//�����ֵ
// 
// 	int nNowSpiritValue = nOldSpiritValue + nNum; //���ϱ���
// 	
// 	int nNowSpiritGridNum	=	nNowSpiritValue / SPIRITPERCENT2GRID;
// 	int nTemp = nNowSpiritGridNum;//����
// 
// 	int nNowSpiritPercent	=	nNowSpiritValue % SPIRITPERCENT2GRID;
// 
// 	int nGridCheck	= __min( this->GetSpiritGridMaxNum(),SPIRITGRID_NUMLIMIT );//���������ȡСֵ
// 
// 	nNowSpiritGridNum = __min( nNowSpiritGridNum,nGridCheck );
// 
// 	nNowSpiritPercent = nNowSpiritPercent + ( nTemp - nNowSpiritGridNum ) * SPIRITPERCENT2GRID;
// 	nNowSpiritPercent	= __min( nNowSpiritPercent,SPIRITPERCENT2GRID - 1 );
// 
// 		
// 	bool bSave = false;
// 	if( nOldSpiritPercent / 10 != nNowSpiritPercent / 10 )// ʮ��֮һ��һ��
// 	{
// 		bSave = true;
// 	}
// 	this->SetAttrib( _USERATTRIB_SPIRIT_PERCENT,nNowSpiritPercent,SYNCHRO_TRUE );//ͬ���ͻ���
// 
// 	if( nOldSpiritGridNum != nNowSpiritGridNum )//���ı䣬����棬�㲥
// 	{
// 		bSave = true;
// 		this->SetAttrib( _USERATTRIB_SPIRITGRID_NUM,nNowSpiritGridNum,SYNCHRO_BROADCAST );//��ROOM�й㲥
// 
// 	}
// 	if( bSave )
// 		m_data.SaveInfo();// CUser��CUserData����Ԫ��,���Կ��Բ����������� 
// 
// 	///�������������ۼ�.
// 	nNum=(nNowSpiritGridNum-nOldSpiritGridNum)*SPIRITPERCENT2GRID+(nNowSpiritPercent-nOldSpiritPercent);
// 	g_VariantManager.AddVariantInt(SPIRIT_LIMIT,GetID(),nNum);
// 	//���»�ֵ��ͬ���ͻ���
// 	SetAttrib(_USERATTRIB_CURRENTSOUL,0,SYNCHRO_TRUE);
// 	SetAttrib(_USERATTRIB_MAXSOUL,0,SYNCHRO_TRUE);
// 	return true;
// }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.20
//����
//�����ʯ�ɹ��Ļ��ʣ�����ʱRand(100)���С�ڴ˷���ֵ����ʾ�ɹ�
// int CUser::GetCongealSpiritStoneSuccedProbability()
// {
// 	int nTemp = 100;//ĿǰΪ100%�ɹ�
// 	return nTemp;
// }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.20
//����
//�õ������3��2����ʯ�Ļ��ʣ�����ʱRandGet( 1000 )���С�ڴ�ֵ��ʾ�ɹ�
//���������nProbabilityLev3Ҫ��֤С��nProbabilityLev2
// void CUser::GetCongealSpiritStoneLevelProbability( int& nProbabilityLev3,int& nProbabilityLev2 )
// {
// 	//���ĳ����Ʒ���ӻ���Ϊ�ٷ�֮����Ҫת��
// 	int p3 = 1;	//ǧ��֮һ
// 	int p2 = 10;//ǧ��֮ʮ
// 	nProbabilityLev3 = p3;
// 	nProbabilityLev2 = p2;
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.12.21
//����
//�꼼���Ļ�ֵ���⴦����Ϊ�洢�ṹ��ͬ
//07.4.20���񲨵�һ���޸�
// void CUser::SpendSpiritValue( int nValue )
// {
// //	int nCurSpiritPercentValue = GetSpiritPercent();//�ٷֱ�ֵ
// //	if( nValue <= nCurSpiritPercentValue )
// //	{
// //		SetAttrib( _USERATTRIB_SPIRIT_PERCENT,nCurSpiritPercentValue - nValue,SYNCHRO_TRUE );//ͬ���Լ�
// //	}
// //	else
// //	{
// //		int nValueConverted = GetSpiritGridNum() * SPIRITPERCENT2GRID + nCurSpiritPercentValue;
// //		nValueConverted = nValueConverted - nValue;
// //		SetAttrib( _USERATTRIB_SPIRIT_PERCENT,nValueConverted % SPIRITPERCENT2GRID,SYNCHRO_TRUE );//ͬ���Լ�
// //		SetAttrib( _USERATTRIB_SPIRITGRID_NUM,nValueConverted / SPIRITPERCENT2GRID,SYNCHRO_BROADCAST );//ͬ�����㲥
// //	}
// 
// 	int nOldSpiritGridNum	= this->GetSpiritGridNum();		//������������
// 	int nOldSpiritPercent	= this->GetSpiritPercent();		//�������İٷֱ�
// 
// 	CHECK( nOldSpiritPercent < SPIRITPERCENT2GRID );
// 	
// 	int nOldSpiritValue		= SPIRITPERCENT2GRID * nOldSpiritGridNum + nOldSpiritPercent;//�����ֵ
// 
// 	int nNowSpiritValue = nOldSpiritValue - nValue; //���ϱ���
// 
// 	nNowSpiritValue = __max( 0,nNowSpiritValue );
// 
// 	int nNowSpiritGridNum	=	nNowSpiritValue / SPIRITPERCENT2GRID;
// 	int nNowSpiritPercent	=	nNowSpiritValue % SPIRITPERCENT2GRID;
// 
// //	int nGridCheck	= __min( this->GetSpiritGridMaxNum(),SPIRITGRID_NUMLIMIT );//���������ȡСֵ
// //
// //	nNowSpiritGridNum = __min( nNowSpiritGridNum,nGridCheck );
// //	nNowSpiritPercent	= __min( nNowSpiritPercent,SPIRITPERCENT2GRID - 1 );
// 
// 		
// 	bool bSave = false;
// 	if( nOldSpiritPercent / 10 != nNowSpiritPercent / 10 )// ʮ��֮һ��һ��
// 	{
// 		bSave = true;
// 	}
// 	this->SetAttrib( _USERATTRIB_SPIRIT_PERCENT,nNowSpiritPercent,SYNCHRO_TRUE );//ͬ���ͻ���
// 	
// 	if( nOldSpiritGridNum != nNowSpiritGridNum )//���ı䣬����棬�㲥
// 	{
// 		bSave = true;
// 		this->SetAttrib( _USERATTRIB_SPIRITGRID_NUM,nNowSpiritGridNum,SYNCHRO_BROADCAST );//��ROOM�й㲥
// 
// 	}
// 	if( bSave )
// 		m_data.SaveInfo();// CUser��CUserData����Ԫ��,���Կ��Բ����������� 
// 
// 
// }

// int CUser::GetSpiritAllValue()
// {
// 	int nCurSpiritPercentValue = GetSpiritPercent();//�ٷֱ�ֵ
// 	return GetSpiritGridNum() * SPIRITPERCENT2GRID + nCurSpiritPercentValue;
// }

//-------------------------------------------
//20061229����:�µ����ݽṹ�漰���Ļ�������
void CUser::UpdateEquipBasicProperty()		//����װ����������.�������������
{
	//����װ��.��������������ֵ�۳���
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

	//�����㷨---------------------------
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
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_ATT1);//��
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAtt1 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_ATT2);//��
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAtt2 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_ATT3);//��
	if( itSerch != m_mOftenUseGemAddition.end())
	{
		m_equipAtt3 += itSerch->second->value;
	}
	itSerch=m_mOftenUseGemAddition.find(GENADD_SORT_ATT4);//��
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
	//goto ������ֵ����ȷ��
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
void CUser::UpdateOftenUseGemAddition()			//���³��ø�������
{
	ResetOftenUseGemAddition();
	OFTENUSE_GEMADDITION mapOftenUseGemPerEquipArrayTemp[ITEMPOSITION_EQUIPEND];

	//��������װ��
	map<OBJID, SUITINFO*> mapSuitInfo;
	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		DEBUG_TRY
		CItemPtr pEquip = GetEquipItemByPos(i);
		if(pEquip&& pEquip->IsValidEquip(this))//װ��ʧЧ
		{
			if(i != ITEMPOSITION_WEAPONSOUL)
			{
				//��װ
				ItemInfoStruct info;
				pEquip->GetInfo(&info);
				if (0 < info.idSuit)//0-100Ϊ��װ  ���ܶ�ȡ������ȥ
				{
					SUITINFO *pSuitInfo = NULL;
					if (mapSuitInfo[info.idSuit]) //��������װ
					{
						//��������
						pSuitInfo = mapSuitInfo[info.idSuit];
						++(pSuitInfo->dwNum);
					}
					else //��û��������װ
					{
						//����һ����1������װ
						pSuitInfo = new SUITINFO;
						pSuitInfo->dwNum = 1;
						pSuitInfo->id = info.idSuit;
						mapSuitInfo[info.idSuit] = pSuitInfo;
					}
				}

				//��������
				for( unsigned char ucPos = 1;ucPos <= EM_BASEPOS_END;ucPos++ )
				{
					int nResult = pEquip->GetBasicStatus( ucPos );
					int nSort = nResult / 10000;
					if(nSort <= 0 )
						continue;
					int nValue = nResult % 10000;
					if (nValue<0)//goto ��ֹ����ֵС��0
					{
						::LogSave("CUser::UpdateOftenUseGemAddition()  ��������ֵС��0");
						continue;
					}

					OFTENUSE_GEMADDITION::iterator it=mapOftenUseGemPerEquipArrayTemp[i].find(nSort);
					if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//û�ҵ�
					{		
						oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
						CHECK(pOftenUseGemAdditionTemp);
						pOftenUseGemAdditionTemp->sort = nSort;
						pOftenUseGemAdditionTemp->value = nValue;
						mapOftenUseGemPerEquipArrayTemp[i].insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
						continue;
					}
					//��ͬ�����
					oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
					CHECK(pOftenUseGemAdditionTemp);
					pOftenUseGemAdditionTemp->value += nValue;
				}

				//����
				for(ucPos = 1;ucPos <= EM_ADDPOS_END;ucPos++ )
				{
					int nResult = pEquip->GetAddStatus( ucPos );
					int nSort = nResult / 10000;
					if(nSort <= 0 )
						continue;
					int nValue = nResult % 10000;
					if (nValue<0)//goto ��ֹ����ֵС��0
					{
						::LogSave("CUser::UpdateOftenUseGemAddition()  ��������ֵС��0");
						continue;
					}
					
					OFTENUSE_GEMADDITION::iterator it=mapOftenUseGemPerEquipArrayTemp[i].find(nSort);
					if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//û�ҵ�
					{		
						oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
						CHECK(pOftenUseGemAdditionTemp);
						pOftenUseGemAdditionTemp->sort = nSort;
						pOftenUseGemAdditionTemp->value = nValue;
						mapOftenUseGemPerEquipArrayTemp[i].insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
						continue;
					}
					//��ͬ�����
					oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
					CHECK(pOftenUseGemAdditionTemp);
					pOftenUseGemAdditionTemp->value += nValue;
				}
				//����
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
					if (nValue<0)//goto ��ֹ����ֵС��0
					{
						::LogSave("CUser::UpdateOftenUseGemAddition() GetEquipSoulStatus ��������ֵС��0");
						continue;
					}

					OFTENUSE_GEMADDITION::iterator it=mapOftenUseGemPerEquipArrayTemp[i].find(nSort);
					if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//û�ҵ�
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
			

			//��ʯ//[2010-07-23 goto]�޸ı�ʯ�������Ի�ȡ��ʽ
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
				if (nValue<0)//goto ��ֹ����ֵС��0
				{
					::LogSave("CUser::UpdateOftenUseGemAddition()  ��ʯ����ֵС��0");
					continue;
				}

				OFTENUSE_GEMADDITION::iterator it=mapOftenUseGemPerEquipArrayTemp[i].find(nSort);
				if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//û�ҵ�
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
			//װ���ĵ��ĸ���
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
					if( it == mapOftenUseGemPerEquipArrayTemp[i].end() )//û�ҵ�
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

	//��װЧ��
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
			if (nValue<0)//goto ��ֹ����ֵС��0
			{
				::LogSave("CUser::UpdateOftenUseGemAddition()  ��װ����ֵС��0");
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

	
	//��ʯ
	for(i = 0/*ITEMPOSITION_EQUIPBEGIN*/; i < ITEMPOSITION_EQUIPEND; i++)
	{
		//��������װ���ı�ʯ��
		for(OFTENUSE_GEMADDITION::iterator iterPerEquip = mapOftenUseGemPerEquipArrayTemp[i].begin(); iterPerEquip!=mapOftenUseGemPerEquipArrayTemp[i].end(); iterPerEquip++)
		{
			oftenUseGemAddition *pGemAdditionPerEquip = iterPerEquip->second;
			CHECK(pGemAdditionPerEquip);
			int nSort = pGemAdditionPerEquip->sort;
			int nValue = pGemAdditionPerEquip->value;//�˺�
			if (nValue<0)//goto ��ֹ����ֵС��0
			{
				::LogSave("CUser::UpdateOftenUseGemAddition()  ��ʯ2����ֵС��0");
				continue;
			}

			//�����㷨---------------------------
			OFTENUSE_GEMADDITION::iterator itSerch=m_mOftenUseGemAddition.find(nSort);
			if( itSerch == m_mOftenUseGemAddition.end() )//û�ҵ�
			{
				//ֱ�Ӳ���
				m_mOftenUseGemAddition.insert(OFTENUSE_GEMADDITION::value_type(pGemAdditionPerEquip->sort,pGemAdditionPerEquip));
				continue;
			}
			else//�ҵ�.�����㷨
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
// 		CItemAdditionData* pAddition = CItem::GetItemAddition(additionID,true );//20070712����:���Ե�
// 		//CHECK(pAddition);
// 		if(pAddition)//������Ϊ�ǵ�,δ�õ���������
// 		{
// 		int nSort = pAddition->GetInt( ITEMADDITIONDATA_SORT );
// 		int nValue = pAddition->GetInt( ITEMADDITIONDATA_VALUE );//�˺�
// 		int nTime = pAddition->GetInt( ITEMADDITIONDATA_TIME );//����ʱ��
// 		int nPossibility = pAddition->GetInt( ITEMADDITIONDATA_POSSIBILITY );//����
// 		if(CItemAddition::IsOftenUseAddition(nSort))//����sort
// 		{
// 			//���δ����ӹ�
// 			//ֱ�Ӳ���
// 			OFTENUSE_GEMADDITION::iterator it=m_mOftenUseGemAddition.find(nSort);
// 			if( it == m_mOftenUseGemAddition.end() )//û�ҵ�
// 			{
// 				oftenUseGemAddition *pOftenUseGemAdditionTemp = new oftenUseGemAddition;
// 				CHECK(pOftenUseGemAdditionTemp);
// 				pOftenUseGemAdditionTemp->sort = nSort;
// 				pOftenUseGemAdditionTemp->value = nValue;
// 				pOftenUseGemAdditionTemp->possibility = nPossibility;
// 				pOftenUseGemAdditionTemp->time = nTime;
// 				pOftenUseGemAdditionTemp->amount =0;
// 				//����
// 				m_mOftenUseGemAddition.insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
// 			}else
// 			{
// 				oftenUseGemAddition *pOftenUseGemAdditionTemp = it->second;
// 				CHECK(pOftenUseGemAdditionTemp);
// 					
// 				//�����㷨-------------------------
// 				//����
// 				//[��;���� 2009.03.11]ȥ���b��˥�p
// 				//int lastTimePossible = pOftenUseGemAdditionTemp->possibility;//�ϴδ����Ļ���
// 				//int lastTimePossibleAdjust = lastTimePossible - lastTimePossible*nPossibility/100;//�ϴλ��ʵ�����.�۳�����������Ļ���
// 				//pOftenUseGemAdditionTemp->possibility = lastTimePossibleAdjust + nPossibility;//�����ܺ�.����100Ϊ100%
// 				pOftenUseGemAdditionTemp->possibility += nPossibility;
// 					
// 				//value
// 				//[��;���� 2009.03.11]ȥ���b��˥�p
// 				//int lastTimeValueTotal = (lastTimePossibleAdjust==0?100:lastTimePossibleAdjust)*pOftenUseGemAdditionTemp->value*(100-_weakenValue)/100;//�ϴ�100���˺������ֵ
// 				//int thisTimeValueTotal = (nPossibility==0?100:nPossibility)*nValue;//����100���˺������ֵ
// 				//pOftenUseGemAdditionTemp->value = (lastTimeValueTotal+thisTimeValueTotal)/(pOftenUseGemAdditionTemp->possibility==0?100:pOftenUseGemAdditionTemp->possibility);
// 				pOftenUseGemAdditionTemp->value += nValue;
// 					
// 				//time
// 				//�����һ��Ϊ׼
// 				pOftenUseGemAdditionTemp->time = nTime;
// 					
// 				//amount���Ӵ���
// 				pOftenUseGemAdditionTemp->amount++;
// 			}
// 		}
// 		}
// 	}
//	UpdateMagicAddition();
//	UpdateGifeAddition();
}
//------------------------------
//��ʼ��m_mOftenUseGemAddition
//void CUser::InitOftenUseGemAddition()//CUser ����ʱ����
//{
//	//���峣��sort
//	//����д�����������---------------
//	const int _maxOftenUseSortAmount = 5;//����sort����
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
//		pOftenUseGemAdditionTemp->amount = 0;//���Ӵ���,����˥��ʱ�õ�
//		
//		m_mOftenUseGemAddition.insert(OFTENUSE_GEMADDITION::value_type(pOftenUseGemAdditionTemp->sort,pOftenUseGemAdditionTemp));
//	}
//}
//------------------------------
//delete m_mOftenUseGemAddition
//void CUser::ClearOftenUseGemAddition()//CUser ����ʱ����
//{
//	//����,delete
//	for(OFTENUSE_GEMADDITION::iterator iter = m_mOftenUseGemAddition.begin(); iter!=m_mOftenUseGemAddition.end(); iter++)
//	{
//		delete iter->second;
//	}
//	m_mOftenUseGemAddition.clear();
//}
//-------------------------------
void CUser::ResetOftenUseGemAddition()//reset m_mOftenUseGemAddition.user����ʱ����,ÿ��update֮ʱ����
{
	//����,delete
	DEBUG_TRY
	for(OFTENUSE_GEMADDITION::iterator iter = m_mOftenUseGemAddition.begin(); iter!=m_mOftenUseGemAddition.end(); iter++)
	{
		SAFE_DELETE(iter->second);
		//delete iter->second;
	}
	DEBUG_CATCH("ResetOftenUseGemAddition()")
	m_mOftenUseGemAddition.clear();
	
}

void CUser::UpdateCalculatedProperty(UPDATECALTYPE nType)			//���¼���֮��Ļ�������.���л�������ϸ�����.
{
	if(UPDATECALTYPE_EQUIP == nType || UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)//װ���仯 �� ȫ�����¼����ʱ��Ÿ���װ����Ϣ
	{
		UpdateEquipBasicProperty();
	}
	if(UPDATECALTYPE_GIFT == nType || UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)//װ���仯 �� ȫ�����¼����ʱ��Ÿ���װ����Ϣ
	{
		UpdateGiftBaseProperty();
	}
	if(UPDATECALTYPE_BUFF == nType || UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)//װ���仯 �� ȫ�����¼����ʱ��Ÿ���װ����Ϣ
	{
		UpdateBuffBaseProperty();
	}
	if(UPDATECALTYPE_EUDSOUL == nType || UPDATECALTYPE_ALL == nType || UPDATECALTYPE_USER == nType)//װ���仯 �� ȫ�����¼����ʱ��Ÿ���װ����Ϣ
	{
		UpdateEudemonSoulProperty();
	}

	//ͬ��
	SynchroUserProperty(nType);
	//bc�㲥 ����
// 	int nMaxLife = this->GetMaxLife();
// 	int nMaxMana = this->GetMaxMana();
// 	int nMaxSp = this->GetMaxSp();
// 
// 	CMsgUserAttrib	msg;
// 	msg.Create(GetID(), _USERATTRIB_MAXLIFE, nMaxLife) ;
// 	msg.Append(_USERATTRIB_MAXMANA, nMaxMana);
// 	msg.Append(_USERATTRIB_MAXSP, nMaxSp);
// 	BroadcastRoomMsg(&msg, EXCLUDE_SELF);//�ų��Լ�
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
	// 				sprintf(szMsg, "<font color='#e46d0a'>{%s}</font>ǧ����ཫ<font color='#cc66ff'>������</font>��������<font color='#cc66ff'>%d��</font>�����ǿ�ϲ�ɺأ�����ʱ�գ����ܴ�չ��ͼ��", this->GetName(), Levbeibao);
	// 				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
	// 				
	// 			}
	// 			else if (502015 < pItem->GetTypeID() && 502024 > pItem->GetTypeID())
	// 			{
	// 				int LevMaterialBag = (pItem->GetTypeID()) % 502010 -1;
	// 				char szMsg[512] = "";
	// 				sprintf(szMsg, "<font color='#e46d0a'>{%s}</font>������յ�蹦��ͽ�<font color='#cc66ff'>������</font>������<font color='#cc66ff'>%d��</font>�����������ֺ䶯�������𾪣�", this->GetName(), LevMaterialBag);
	// 				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
	// 			}
	// 			else if (502028 <pItem->GetTypeID() && 502038 > pItem->GetTypeID())
	// 			{
	// 				int LevPetBag = (pItem->GetTypeID()) % 502020 -5;
	// 				char szMsg[512] = "";
	// 				sprintf(szMsg, "<font color='#e46d0a'>{%s}</font>�����Ľ�<font color='#cc66ff'>������</font>������<font color='#cc66ff'>%d��</font>�������������ţ���������", this->GetName(), LevPetBag);
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
			sprintf(UserPak, "%d_������_%d", SNAPSHOTNOTHING, COLORPINK);
			sprintf(PakLev, "%d_%d��_%d", SNAPSHOTNOTHING, nLev, COLORPINK);
			sprintf(szMsgx, "<%s>������յ�蹦��ͽ�<%s>������<%s>�����������ֺ䶯�������𾪣�", UserName, UserPak, PakLev);
			UserManager()->BroadCastMsgAndReceiveName(szMsgx, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
		}
		break;
	case 3:
		if(nLev <= nLev3)
			return false;
		nLev3 = nLev;
		if(nLev>4){
			sprintf(UserPak, "%d_������_%d", SNAPSHOTNOTHING, COLORPINK);
			sprintf(PakLev, "%d_%d��_%d", SNAPSHOTNOTHING, nLev, COLORPINK);
			sprintf(szMsgx, "<%s>ǧ����ཫ<%s>��������<%s>�����ǿ�ϲ�ɺأ�����ʱ�գ����ܴ�չ��ͼ��", UserName, UserPak, PakLev);
			UserManager()->BroadCastMsgAndReceiveName(szMsgx, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
		}
		break;
	case 4:
		if(nLev <= nLev4)
			return false;
		nLev4 = nLev;
		if(nLev>3){
		sprintf(UserPak, "%d_������_%d", SNAPSHOTNOTHING, COLORPINK);
		sprintf(PakLev, "%d_%d��_%d", SNAPSHOTNOTHING, nLev, COLORPINK);
		sprintf(szMsgx, "<%s>�����Ľ�<%s>������<%s>�������������ţ���������", UserName, UserPak, PakLev);
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
//����
inline BOOL CUser::IsWhiteName()
{
	return GetPk()==PKVALUE_WHITE;
}
//----------------------------------------------
//����
BOOL	CUser::IsGrayName()
{
	if(QueryStatus(STATUS_CRIME))
		return true;
	else
		return false;
}
//----------------------------------------------
//����
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

//  [4/2/2007] halei װ�������ʱ��ı����ĸ�������
void CUser::EquipChgAttribute( ItemInfoStruct* info,int nBossType /*= 0 */,int nAddType/* = 0*/)
{
	if (info->idType/100000 != IETMSORT_FINERY1 && info->idType/100000 != IETMSORT_FINERY2)
	{
		return;
	}
	if(info->idType == 144000)
	{
		//Ʒ��
		info->dwQual = 5;
		//��ɫ
		info->dwColor = 5;
		//�ȼ�
		info->dwAddLev = 10;
		//�Ǽ�
		info->dwStr = 10;
		return;
	}


	float nQualAdd = 1.0;
	float nColorAdd = 1.0;

	//Ʒ��
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
		//Ʒ��  ��ʱ��Ӱ��
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


		//��ɫ		
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
		//Ʒ��		
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

		//��ɫ		
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
	
	//�ȼ�
	info->dwAddLev = 0;

	//�Ǽ�
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

	//�������Ա仯
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

	//��������
	//���
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
	//δ���������
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
			//ȫ�ظ�����������
			nCount ++;
			if(nCount >= 10)
			{
				return;
			}

			if(j == i)
				bSetOK = true;
		}

		//�⹦�ڹ� ����ڷ�
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
		//���� ���� ����
		case 5:
		case 6:
		case 7:
			if(nLevNed > 10)
				nvalue = 0.5082f *  nLevNed - 0.9801f;
			else
				nvalue = 0.3388f * nLevNed + 1.1979f;
			break;
		//����
		case 8:
			if(nLevNed > 10)
				nvalue = 0.5566f*nLevNed - 1.0769f;
			else
				nvalue = 0.3751f*nLevNed + 1.3189f;
			break;
		//���� ����  ���� ����  ��
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
		//��������
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
		//���Թ���
		case 20:
		case 21:
		case 22:
		case 23:
		//���Է���
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
	//2007828����:ȥ����֪��ʱ������.--------
	//���µ�reborn
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

//	20070703 ���
//	�������ʱ�Ƿ������,��װ��,���;�
//	����TRUEʱΪ�������ͷ�,FALSEʱΪû�������ͷ�
BOOL CUser::DoesUserNeedDeathPunishment(IRole* pRole)
{
	//	20070705 PK��ͼ
	CGameMap* pMap = this->GetMap();
	if(pMap->IsPkFieldNoDeathPunish())
		return FALSE;

	if(GetLev() < VETERAN_DIFF_LEVEL)
		return FALSE;

	///softworms-07-08-28-�ж����вμ�ս��������,�������ͷ�
	if (QueryStatus(STATUS_WARGAME))
		return FALSE;
		
	return TRUE;
}

//	20070703 ���
//	��龭��ֵ����
//	���ؼ���������ľ���ֵ
//int CUser::CheckExp(__int64 &nAddExp/*���������ľ���ֵ,����ʵ������ľ���ֵ(����ֵ�������)*/)
__int64 CUser::CheckExp(__int64 &nAddExp/*���������ľ���ֵ,����ʵ������ľ���ֵ(����ֵ�������)*/)
{
	if(nAddExp > 0)	//	ֻ���۾���
		return GetExp() + nAddExp;

	if(nAddExp < USER_NEGATIVE_EXP_MIN_VALUE)
		nAddExp = USER_NEGATIVE_EXP_MIN_VALUE;

	__int64 i64CurrentExp = GetExp();
	__int64 newExp = i64CurrentExp + nAddExp;
	if(newExp < USER_NEGATIVE_EXP_MIN_VALUE)	//	���ܵ������ֵ
		newExp = USER_NEGATIVE_EXP_MIN_VALUE;
	
	//	���ܳ�����߰ٷֱ�
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
// 		if(nAction == 1) //�ı� ���Ҹ����ʺŷ��������ݸı�
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
// 		else //�����������ݿ� ����Ҫ֪ͨ�ʺŷ�����
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
// 			::GmLogSave("OK: ��ȡ%d��װ���ɹ���AccountID:%d, playerID: %d ",nAction,GetAccountID(), GetID() );
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
// // 	MsgVAS.Create( ( void* )&MsgVAS.m_pInfo->mtExtraInfo, nType, GetAccountID(), GetID(), 11,nAction );///����Ϣ˳��˵����Ϣ������������
// // 	MsgVAS.AppendInfo( m_idProcess,m_idSocket,ID_NONE );//����AppendInfo������07.1.20�޸�
// // 	//�������̷߳���Ϣ
// // 	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
// 
// 	if( nType >= 4  && nType != 6)
// 	{
// // 		CMsgVAS MsgVAS1;
// // 		MsgVAS1.Create( ( char* )&MsgVAS1.m_pInfo->mtExtraInfo, nType, GetAccountID(), GetID(),10 );///����Ϣ˳��˵����Ϣ������������
// // 		//�������̷߳���Ϣ
// // 		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS1 );
// 	}		
}

//	20070712	���	
//	������ID����ȫ��װ��,�ֿ�,����,ħ����,����
CItemPtr CUser::GetUserAllItemByType(OBJID idType)	
{
	CItemPtr	pItem = NULL;
	//	�ֿ�
	pItem = GetStorageItemByType(idType);
	if(pItem)
		return pItem;

	//	����,ħ����
	pItem = QueryPackage()->GetItemByType(idType);
	if(pItem)
		return pItem;

	//	����
	pItem = GetEquipItemByType(idType);
	if(pItem)
		return pItem;
	

	return NULL;
}

//	20070712	���	
//	����װ�������ϵ�
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
//  [2007-8-8] ��� ע���ǳ�������ħ��оƬ
// BOOL CUser::ImportSpiritStoneToChip(int *pMaterial, int nSize, char *szError)
// {
// 	CHECKF(pMaterial && nSize);
// 
// 	CItem*	pItemChip = this->GetItem(pMaterial[0]);	
// 	if(!pItemChip)
// 	{
// 		sprintf(szError,g_objStrRes.GetStr(17018) );//"û���ҵ�ħ��оƬ!"
// 		return FALSE;
// 	}
// 
// 	if(!pItemChip->IsSpiritChip() 
// 		&& !pItemChip->IsAdvancedSpiritChip())
// 	{
// 		sprintf(szError, g_objStrRes.GetStr(17019));//"û��ħ��оƬ!"
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
// 			sprintf(szError, g_objStrRes.GetStr(17020));//"�з�ħ����Ʒ����!"
// 			return FALSE;
// 		}
// 
// 		if(CItem::GetSpiritStoneLev(pItemSpirit[nSpiritCount]->GetTypeID()) > nMaxImportSpiritStoneLevel)
// 		{
// 			sprintf(szError, g_objStrRes.GetStr(17021));//"3�����ϵ�ħ���̺���������ǿ���޷�ע��!"
// 			return FALSE;
// 		}
// 		nSpiritCount++;
// 	}
// 
// 	if(nSpiritCount < 1)
// 	{
// 		sprintf(szError, g_objStrRes.GetStr(17022));//"ħ��������!"
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
//  [2007-8-10] ��� ��ħ��оƬ
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
// // 		int nSpendSpirit = __min(nCurrentSpirit, nMaxSpendSpirit);	//	�ñ���Ҫ�������
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
//  [2007-8-10] ��� ����
// BOOL CUser::AddSpirit(int nSpirit)
// {
// 	if(nSpirit < 0)
// 		return false;
// 	int nOldSpiritGridNum	= this->GetSpiritGridNum();		//	������������
// 	int nOldSpiritPercent	= this->GetSpiritPercent();		//	�������İٷֱ�
// 
// 	CHECKF( nOldSpiritPercent < SPIRITPERCENT2GRID );
// 	
// 	int nOldSpiritValue		= SPIRITPERCENT2GRID * nOldSpiritGridNum + nOldSpiritPercent;//�����ֵ
// 
// 	int nNowSpiritValue = nOldSpiritValue + nSpirit; //���ϱ���
// 	
// 	int nNowSpiritGridNum	=	nNowSpiritValue / SPIRITPERCENT2GRID;
// 	int nTemp = nNowSpiritGridNum;	//����
// 
// 	int nNowSpiritPercent	=	nNowSpiritValue % SPIRITPERCENT2GRID;
// 
// 	int nGridCheck	= __min( this->GetSpiritGridMaxNum(), SPIRITGRID_NUMLIMIT );	//	���������ȡСֵ
// 
// 	nNowSpiritGridNum = __min( nNowSpiritGridNum, nGridCheck );
// 
// 	nNowSpiritPercent = nNowSpiritPercent + ( nTemp - nNowSpiritGridNum ) * SPIRITPERCENT2GRID;
// 	nNowSpiritPercent	= __min( nNowSpiritPercent, SPIRITPERCENT2GRID - 1);
// 
// 		
// 
// 	bool bSave = false;
// 	if( nOldSpiritPercent / 10 != nNowSpiritPercent / 10 )// ʮ��֮һ��һ��
// 	{
// 		bSave = true;
// 	}
// 	this->SetAttrib( _USERATTRIB_SPIRIT_PERCENT, nNowSpiritPercent, SYNCHRO_TRUE );//ͬ���ͻ���
// 	
// 	if( nOldSpiritGridNum != nNowSpiritGridNum )//���ı䣬����棬�㲥
// 	{
// 		bSave = true;
// 		this->SetAttrib( _USERATTRIB_SPIRITGRID_NUM, nNowSpiritGridNum, SYNCHRO_BROADCAST );//��ROOM�й㲥
// 
// 	}
// 	if( bSave )
// 		m_data.SaveInfo();// CUser��CUserData����Ԫ��,���Կ��Բ����������� 
// 	return true;
// }

void CUser::AwardHonor(int nHonor, BOOL bUpdate, int nSynchro)
{
	return;
// 	//	��HONOR
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
// 		this->SendMsg(&msg);	//	�������Լ�	
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
// 	if (this->GetSynRank() != RANK_LEADER)		// ֻ�б���������������
// 	{
// 		this->SendSysMsg(g_objStrRes.GetStr(11097));//STR_NOT_AUTHORIZED
// 		return false;
// 	}
// 
// 	CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
// 	if( !pSyn)
// 		return false;
// 
// 	return pSyn->ReqSetSynName(name);//���Ÿ���
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
// 		if(pMember)	//	�ڵ�ͼ��
// 		{
// 			CTeam* pTeamTemp = pMember->GetTeam();
// 			if(pTeamTemp)
// 				pTeamTemp->AddMemberOnly(idAddMember);	//	ֻ�����³�ԱID
// 		}
// 		else	//	����, �㲥��������ͼ���¼�
// 		{
// 			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_INFO, pTeam->GetID(), idAddMember, idAddMemberSocket);
// 		}
//	}
	
	//	֪ͨ�³�Ա������Ա
//	ST_TEAM_INVITE st;
	
//	memcpy(&st, pTeamInvite, sizeof(ST_TEAM_INVITE));
	//	CUserPtr pTarget = UserManager()->GetUser(pTeamInvite->szTargetName);
	//	CHECK(pTarget);				
	
//	CTeam* pTeam = pUser->GetTeam();
// 	int nInMapGroupMemberCount = 0;
// 	BOOL	bNotInMapGroup = FALSE;	//	�ж�Ա���ڴ˵�ͼ��
// 	BOOL	bMemberHasUser = FALSE;
// 	for(i = 0; i < nMemberCount; i++)
// 	{
// 		OBJID idMember = pTeam->GetMemberByIndex(i);
// 		CUserPtr pMember = UserManager()->GetUser(idMember);
// 		if(pMember)	//	�ڵ�ͼ��
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
// 		else	//	����, �㲥��������ͼ���¼�
// 		{
// 			bNotInMapGroup = TRUE;
// 		}
// 
// 		if(idMember == GetID())
// 			bMemberHasUser = TRUE;
// 		
// 		st.teamInfo.setMember[i] = idMember;
// 	}

	//  [2007-9-27] ��� �����Ա��Ϣ��û���Լ�,����
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
	
//	CHECK(nInMapGroupMemberCount < 5);	//	�Ѿ����ڵĳ�Ա���ܳ���4��
	
	//  [2007-9-27] ���	�Լ����÷���
	//	�³�Ա�Լ�����Ϣ
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
	//	֪ͨ�³�Ա����ɹ�
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
// 	//	����TEAMINFO
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
// 	if(bNotInMapGroup)	//	֪ͨ���ڵĸ���
// 	{
// 		MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_ADD, pTeam->GetID(), idAddMember, idAddMemberSocket);
// 		MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_INFO, pTeam->GetID(), idAddMember, idAddMemberSocket);
// 	}
// 	
// 	//	С�ӹ㲥,�����³�Ա����Ϣ��ÿ����Ա
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

//  [2007-9-14] ���	���װ���Ƿ񼤻�,δ�����
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
			LOGWARNING("���װ����ʱ��װ�����Ծ���Ҫ��� itemid = %u itemtype = %u userid = %u oldPosition = %u checkPosition = %u",pItem->GetID(),pItem->GetTypeID(),GetID(),pItem->GetPosition(),i);
			DEBUG_TRY
			pItem->SetPosition(i);
			m_pPackage->PopItem(pItem->GetID());
			DEBUG_CATCH("���װ����ʱ��װ�����Ծ�����ԭ����");
		}


		CheckItemLifeTime(pItem, bUpdateToClient, true);

// 		CItemPtr pItemEnd = GetEquipItemByPos(i);
// 		if(pItemEnd)
// 		{
// 			if(pItem->GetPosition() != i)
// 			{
// 				LOGWARNING("�����װ����ʱ��װ�����Ծ���Ҫ��� itemid = %u userid = %u oldPosition = %u checkPosition = %u",pItem->GetID(),GetID(),pItem->GetPosition(),i);
// 				pItem->SetPosition(i);
// 			}
// 		}
// 		else
// 			LOGWARNING("װ�����ʱͨ��,װ����ʧ position = %u userid = %u",i,GetID());

		DEBUG_CATCH("���װ����ʱ��װ�����Ծ�")
	}

	DEBUG_TRY
	m_pPackage->CheckItemLifeTime(bUpdateToClient, true);
	DEBUG_CATCH("m_pPackage->CheckItemLifeTime()");
// 
// 	//  [3/5/2008 ���]	�ֿⲻ���,��Ϊ�ֿ������Ʒɾ��ʱɾ������(EraseItem��ɾ����������Ʒ)
// 	//	�ֿ������Ʒ���ó�ʱ��� ��CMsgPackage��
// 	//	�ֿ�
// // 	if(m_pStorage)
// // 		m_pStorage->CheckItemLifeTime(this, bUpdateToClient, bUpdateDB);
// 
// 	InterlockedExchangeAdd(&g_stat.setDebug[13], clock() - tStart);	//	����ʱ
}

float CUser::CalItemAddExpTeamFactor()
{
	float fResult=0.0;
	DEBUG_TRY
	CTeam* pTeam=GetTeam();
	if(IsVipActive())//������Ҳ��VIPʱҲ�ӳ�
	{
		int nVipLev = GetVipLev()==4 ? 1 : GetVipLev();//�ܿ�Ҫ���¿�Ч��һ��
		fResult+=nVipLev*0.1+0.1;///VIP��Ҹ���VIP�ȼ������ۼ�,��ͨ���Ϊ0.1���¿����0.2���������0.3�����꿨���0.4
	}
    else
        fResult+=0.1f;///��ͨ���Ҳ��0.1�ļӳ�
	if(pTeam!=NULL)///ֻ������ж�ʱ�Ž��д���
	{
		CUser* pMember=NULL;
		for(int i = 0 ;i<_MAX_TEAMAMOUNT ; i++)
		{
			///Ϊ�յĶ�Ա������
			if(pTeam->m_TeamIn[i].id ==ID_NONE)
				continue;
			if(pTeam->m_TeamIn[i].id ==GetID())///���Ե�buff������,��Ϊ���Ѿ��ӳɹ���
				continue;
			pMember=UserManager()->GetUser(pTeam->m_TeamIn[i].id);
			if(pMember==NULL)///�����Ա������ͬ��ͼ���򲻴���
				continue;
			if(pMember->GetMapID()!=GetMapID())///����buff��������ͬ��ͼ�Ķ�Ա������Ч��
				continue;
			if(pMember->QueryStatus(STATUS_ITEM_ADDEXP)==NULL)///����buff�Ķ�Ա������Ч��
				continue;

			if(pMember->IsVipActive()==false)///��ͨ���0.1
				fResult+=0.1f;
			else
			{
				int nVipLev = pMember->GetVipLev()==4 ? 1 : pMember->GetVipLev();//�ܿ�Ҫ���¿�Ч��һ��
				fResult+=nVipLev*0.1+0.1;///VIP��Ҹ���VIP�ȼ������ۼ�,��ͨ���Ϊ0.1���¿����0.2���������0.3�����꿨���0.4
			}
		}
	}
// 	char szWords[128] = "";
// 	sprintf(szWords, "����ӳ�ϵ��%f",fResult);
// 	UserManager()->BroadCastMsgAndReceiveName(szWords, /*pUser*/NULL, 0, 0xffffff, _TXTATR_WORLDSERVER,this,0,0,"",ALLUSERS_NAME);
	DEBUG_CATCH2("CalItemAddExpTeamFactor crash!userid:%d",GetID());
	return fResult;
}

void CUser::UpdateLastLogout()
{
	m_data.SetLastLogout();
}

void CUser::UpdateAllOnlineTime()//-����������ʱ��//���������洢����ʱ��
{
	// ------------------------begin-----------------���߸���������ʱ��-------------------------------- [9/12/2007 %PENGFENG%]
		int nAllOnlineTime = this->GetOnlineTime();
		this->SetAllOnlineTime(nAllOnlineTime);
	//---------------------------------end----------------------------------------------------------------
}

OBJID CUser::GetOnlineTime()//-�õ���������ʱ��//��λ����
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
	case 0://ȫϴ
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
	case 1://����1
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
			//goto ��������Ժ�Ͱ�������0������д���ݿ����
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
	case 2://����2
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
			//goto ��������Ժ�Ͱ�������0������д���ݿ����
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
	case 3://����3
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
			//goto ��������Ժ�Ͱ�������0������д���ݿ����
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
	case 4://����4
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
			//goto ��������Ժ�Ͱ�������0������д���ݿ����
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
	case 5://����5
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
			//goto ��������Ժ�Ͱ�������0������д���ݿ����
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
	if(IsVipActive()) //�Ѿ���
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
			sprintf(szMsg, "<%s>Զ���Ƿ����ؽ�����<%s>ʹ�ã�������ɷ���ˣ������½Ծ���", UserName,"3_VIP�ܿ�_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
		}
		else if (43200 == dwMin)
		{
			if(m_data.GetVipLev() < 1 || m_data.GetVipLev() > 3)
				m_data.SetVipLev(1);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>Զ���Ƿ����ؽ�����<%s>ʹ�ã�������ɷ���ˣ������½Ծ���", UserName,"3_VIP�¿�_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!4")
		}
		else if (129600 == dwMin)
		{
			if(m_data.GetVipLev() < 2 || m_data.GetVipLev() > 3)
				m_data.SetVipLev(2);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>Զ���Ƿ����ؽ�����<%s>ʹ�ã����Ҳ����԰���Ⱥ�ۣ��ھ�������", UserName,"3_VIP����_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!5")
		}
	    else if (259200 == dwMin)
		{
			if(m_data.GetVipLev() < 3 || m_data.GetVipLev() > 3)
				m_data.SetVipLev(3);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>Զ���Ƿ����ؽ�����<%s>ʹ�ã�׿Ȼ���������£��緶����ߣ�����֮�������м��ˣ�", UserName,"3_VIP���꿨_5");
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
			//�㲥��������·
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
			sprintf(szMsg, "<%s>Զ���Ƿ����ؽ�����<%s>ʹ�ã�������ɷ���ˣ������½Ծ���", UserName,"3_VIP�ܿ�_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!4")
		}
		else if (43200 == dwMin)
		 {
			 m_data.SetVipLev(1);
			 DEBUG_TRY
			 char szMsg[512] = "";
			 sprintf(szMsg, "<%s>Զ���Ƿ����ؽ�����<%s>ʹ�ã�������ɷ���ˣ������½Ծ���", UserName,"3_VIP�¿�_5");
			 UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			 DEBUG_CATCH("vip broad!!1")
		 }
		else if (129600 == dwMin)
		{
			m_data.SetVipLev(2);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>Զ���Ƿ����ؽ�����<%s>ʹ�ã����Ҳ����԰���Ⱥ�ۣ��ھ�������", UserName,"3_VIP����_5");
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, "" , ALLUSERS_NAME);
			DEBUG_CATCH("vip broad!!2")
		}
	   	else if (259200 == dwMin)
		{
			m_data.SetVipLev(3);

			DEBUG_TRY
			char szMsg[512] = "";
			sprintf(szMsg, "<%s>Զ���Ƿ����ؽ�����<%s>ʹ�ã�׿Ȼ���������£��緶����ߣ�����֮�������м��ˣ�", UserName,"3_VIP���꿨_5");
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
			//�㲥��������·
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
	SendSysMsg("���VIP�Ѿ�����");
	
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
	//����
	if(ChkNewGiveMask(nType - 1))
		return false;

	int nCheckLev = 0;
	DEBUG_TRY
	int nTotalHour = GetPassedTime(GetCreateTime(),2);

	//���
	switch(nType)
	{
	case 1:
		{
			//��1�� 1��	
			if(nTotalHour >= 24)
				return false;
			//15��
			if(GetLev() < 15)
				return false;
		}
		break;
	case 2:
		{
			//��2�� 2��	
			if(nTotalHour < 24 )
				return false;
			if(nTotalHour >= 24  + 24 * 2)
				return false;

			//26��
			if(GetLev() < 26)
				return false;
		}
		break;
	case 3:
		{
			//��3�� 3��	
			if(nTotalHour < 24 * 2)
				return false;
			if(nTotalHour >= 24  * 2 + 24 * 3)
				return false;
			
			//37��
			if(GetLev() < 37)
				return false;
		}
		break;
	case 4:
		{
			//��4�� 3��	
			if(nTotalHour < 24 * 3)
				return false;
			if(nTotalHour >= 24  * 3 + 24 * 6)
				return false;
			
			//��ְҵ
			if(GetProfession(1) == 0)
				return false;
		}
		break;
	case 5:
		{
			//��5�� 1��	
			if(nTotalHour < 24 * 4)
				return false;
			if(nTotalHour >= 24  * 4 + 24)
				return false;
			
			//����20��
			if(GetFriendAllAmount() < 20)
				return false;
		}
		break;
	case 6:
		{
			//��6�� 4��	
			if(nTotalHour < 24 * 5)
				return false;
			if(nTotalHour >= 24  * 5 + 24 * 10)
				return false;
			
			//���3��
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
			//��7�� 3��	
			if(nTotalHour < 24 * 6)
				return false;
			if(nTotalHour >= 24  * 6 + 24 * 3)
				return false;

			//�ڹ����⹦ 6000
			if((GetAtk() < 6000) && (GetMgcAtk() < 6000))
				return false;
		}
		break;
	case 8:
		{
			//��8�� 10��	
			if(nTotalHour < 24 * 7)
				return false;
			if(nTotalHour >= 24  * 7 + 24 * 10)
				return false;
			
			//50����װ3��
			int nHas = 0;
			DWORD prof = this->GetProfession();
			if( prof & PROF_ASSIST )//����
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
			else if( prof & PROF_MASTER )//��ʦ
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
			else if( prof & PROF_PASTOR )//��ʦ
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
			else if( prof & PROF_ASSISSIN )//�̿�
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
			else if( prof & PROF_SOLDIER )//սʿ
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
			else if( prof & PROF_SWORD )//����
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
			//��9�� 5��	
			if(nTotalHour < 24 * 8)
				return false;
			if(nTotalHour >= 24  * 8 + 24 * 5)
				return false;
			
			//��ս�� 50�� 6����
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
			//��10�� 5��	
			if(nTotalHour < 24 * 9)
				return false;
			if(nTotalHour >= 24  * 9 + 24 * 8)
				return false;
			
			//60��
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
	//SendSysMsg("��ȡ%u",nType);
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
						SendSysMsg("��ǰ��ս���������Ѿ�");
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
						SendSysMsg("��ǰ������������Ѿ�");
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
		SendSysMsg("���VIP�Ѿ�����");

		CMsgPlayer msgPlayer;
		if (msgPlayer.Create(this->QueryRole()))
			this->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

		DelHoldTitle(_TITLE_VIP);//goto ɾ��VIP�ƺ�
	}
	else if(nTotalMin <= 4320)
	{
		if(m_dwLastVipCheckSendTime  != ((timeNow.GetYear() - 2000) * 100000000 + timeNow.GetMonth() * 1000000  + timeNow.GetDay() * 10000))
		{
			m_dwLastVipCheckSendTime = (timeNow.GetYear() - 2000) * 100000000 + timeNow.GetMonth() * 1000000  + timeNow.GetDay() * 10000;
// 			char szTempTime[64];
// 			nHour = nTotalMin
			SendSysMsg("���VIP����ڻ���%u����",nTotalMin);
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
		SendSysMsg("����VIPͷ���ѹ���");
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

//  [2007-9-14] ���	�����Ʒ��������
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
			sprintf(szWords,"���%s�Ѿ�����",pItem->GetStr(ITEMDATA_NAME));
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

				if (bCostlyItem)//goto ������Ʒ��¼
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
// 		//  [2007-9-14] ���	δ����
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
// 		if(nNewLifeTime <= 0)	//	����
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
// 			InterlockedExchangeAdd(&g_stat.setDebug[14], clock() - tStart);	//	���ݿ������ʱ
// 			
// 			//  �Ѿ�����,���Ϸ���Ϣ
// 			tStart = clock();
// 			CMsgItemAttrib msg;
// 			if(msg.Create(pItem->GetID(), _ITEMATTRIB_IS_ACTIVATE, (DWORD)TRUE))
// 			{
// 				msg.Append(_ITEMATTRIB_LIFE_TIME, 0);
// 				this->SendMsg(&msg);
// 			}
// 			InterlockedExchangeAdd(&g_stat.setDebug[15], clock() - tStart);	//	ͬ���ͻ�����ʱ
// 
// 			//  [3/5/2008 ���]	��̯�쳣
// 			//	�ȴӰ�̯��ɾ��
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
// 				//	�ȷ��ʼ�
// 				sprintf(szMailTitle, g_objStrRes.GetStr(16022), pItem->GetStr(ITEMDATA_NAME));
// 				sprintf(szMailContent, g_objStrRes.GetStr(16023), pItem->GetStr(ITEMDATA_NAME));
// 				PostOffice()->SendSysMail(this->GetID(), szMailTitle, szMailContent, 0);
// 
// 				//	��������,ɾ��
// 				if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
// 					EraseEquip(nPos, true);
// 				else
// 					EraseItem(pItem->GetID(), true);
// 
// 				pItem = NULL;
// 			}
// 			else
// 			{
// 				//	��������
// 				//	ж��,����ʹ��,֪ͨ���,���ʼ�
// 		//		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
// 		//			UnEquipItem(nPos, true);	//	����п�λ�ͷ�����
// 
// 				//	����Ѿ�װ��,����
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
// 				//  [2007-9-18] ���	���ʼ��������
// 	 			PostOffice()->SendMail(this, this->GetName(), szMailTitle, szMailContent, CSKMail::mtNormal, pItem->GetID(), 0);	
// 				//PostOffice()->SendSysMail(GetID(), szMailTitle, szMailContent);
// 
// 				pItem = NULL;
// 			}	
// 			
// 			return;
// 		}
// 		else if(nNewLifeTime <= 60)	//	�뵽�ڻ���һ��Сʱ
// 		{
// 			if(uLifeTime > 60)
// 			{
// 				//	�ոյ����һ��Сʱ,��������
// 				//	����%s����%d���ӵ��ڣ�����Shift+����Ҽ������Ʒ���ѡ�
// 				if(pItem->IsLifeTimeContinue())	//  [11/8/2007 ���]	���������ٷ�
// 					this->SendSysMsg(_TXTATR_SYSTEM, g_objStrRes.GetStr(16021) , pItem->GetStr(ITEMDATA_NAME ), nNewLifeTime);
// 				
// 				clock_t tStart = clock();
// 				CMsgItemAttrib msg;
// 				if(msg.Create(pItem->GetID(), _ITEMATTRIB_LIFE_TIME, (DWORD)pItem->GetLifeTime()))
// 				{
// 					this->SendMsg(&msg);
// 				}
// 				InterlockedExchangeAdd(&g_stat.setDebug[15], clock() - tStart);	//	ͬ���ͻ�����ʱ
// 			}
// 			else
// 			{
// 				//	�򵥴���ÿ5��������һ��, ʣ�����β��Ϊ0,5ʱ����
// 				if(pItem->IsLifeTimeContinue())	//  [11/8/2007 ���]	���������ٷ�
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
// 		InterlockedExchangeAdd(&g_stat.setDebug[14], clock() - tStart);	//	���ݿ������ʱ
// 		
// 		tStart = clock();
// 		if(bUpdateToClient)	//	���¿ͻ���
// 		{
// 			CMsgItemAttrib msg;
// 			if(msg.Create(pItem->GetID(), _ITEMATTRIB_LIFE_TIME, (DWORD)pItem->GetLifeTime()))
// 			{
// 				this->SendMsg(&msg);
// 			}
// 		}
// 		InterlockedExchangeAdd(&g_stat.setDebug[15], clock() - tStart);	//	ͬ���ͻ�����ʱ
// 	}
}

//  [2007-9-21] ���	���µ�����������Ϣ
// void CUser::InsertVasItemPresendInfo(_VAS_ITEM_PRESENT_INFO& info)
// {
// 	m_setVasItemPresentInfo.push_back(info);	
// }

//  [2007-9-21] ���	��������������Ϣ
//	�ҵ�����TRUE, ������Ϣ��䵽info��ȥ,����ʧ�ܷ���FALSE
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

	TaskDetailInfoForMsg taskDetailArrayTemp[_MAX_TASK_DETAIL_STRUCT_NUM];//�ṹ������.
	int taskDetailNum=0;
	CTaskDetail * pTask = this->GetTaskDetail();
	IF_NOT(pTask)
		return ;
	
	int nSendCountAll = 0;//�ܹ�ʵ�ʷ�������
	int nCurSendCount = 0;//���η�������
	while((pTask->m_setData.size()-nSendCountAll)>0)//���з���
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
			DWORD timeSpan = TaskSetRand()->GetSetTime(taskDetailArrayTemp[nCurSendCount].taskID,taskDetailArrayTemp[nCurSendCount].taskPhase);//����涨ʱ��
			if(timeSpan == 0)
				taskDetailArrayTemp[nCurSendCount].lifeTime = 99999;
			else
			{
				DWORD now = TimeGet(TIME_SECOND);//����ʱ��
				DWORD frontTime = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKBEGINTIME);//����ʼʱ��
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
	//����β��
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

	TaskDetailInfoForMsg taskDetailArrayTemp[_MAX_TASK_DETAIL_STRUCT_NUM];//�ṹ������.
	int taskDetailNum=0;
	CTaskDetail * pTask = this->GetTaskDetail();
	IF_NOT(pTask)
		return ;
	
	int nSendCountAll = 0;//�ܹ�ʵ�ʷ�������
	int nCurSendCount = 0;//���η�������
	while((pTask->m_setData.size()-nSendCountAll)>0)//���з���
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
			DWORD timeSpan = TaskSetRand()->GetSetTime(taskDetailArrayTemp[nCurSendCount].taskID,taskDetailArrayTemp[nCurSendCount].taskPhase);//����涨ʱ��
			if(timeSpan == 0)
				taskDetailArrayTemp[nCurSendCount].lifeTime = 99999;
			else
			{
				DWORD now = TimeGet(TIME_SECOND);//����ʱ��
				DWORD frontTime = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKBEGINTIME);//����ʼʱ��
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
	//����β��
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
	TaskDetailInfoForMsg taskDetailArrayTemp[_MAX_TASK_DETAIL_STRUCT_NUM];//�ṹ������.
	int taskDetailNum=0;
	CTaskDetail * pTask = this->GetTaskDetail();
	IF_NOT(pTask)
		return 0;

	int nSendCountAll = 0;//�ܹ�ʵ�ʷ�������
	int nCurSendCount = 0;//���η�������
	while((pTask->m_setData.size()-nSendCountAll)>0)//���з���
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

		//ѹ��
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

		DWORD timeSpan = TaskSetRand()->GetSetTime(taskDetailArrayTemp[nCurSendCount].taskID,taskDetailArrayTemp[nCurSendCount].taskPhase);//����涨ʱ��
		if(timeSpan == 0)
			taskDetailArrayTemp[nCurSendCount].lifeTime = 99999;
		else
		{
			DWORD now = TimeGet(TIME_SECOND);//����ʱ��
			DWORD frontTime = pTask->m_setData[nSendCountAll]->GetInt(TASKDETAILDATA_TASKBEGINTIME);//����ʼʱ��
			DWORD span  = now - frontTime;
			if(span > timeSpan)
				taskDetailArrayTemp[nCurSendCount].lifeTime = 0;
			else
			taskDetailArrayTemp[nCurSendCount].lifeTime = timeSpan - span;  
		}
		nCurSendCount++;
		nSendCountAll++;
	}
	//����β��
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

// void CUser::LevaeResourceWarMap()// ����뿪��Դս��ͼ���� [9/15/2007 %PENGFENG%]
// {
// 	//----------������뿪��Դս��ͼ�������ȴ����������SAVEINFO-----begin-----------------//pengfeng
// 	this->DropItemBySys( g_ResourceWar.GetSKZLType(),this->GetPosX(),this->GetPosY() );
// 	
// 	CRole::DetachStatus( this->QueryRole(),STATUS_RESOURCEWAR_PROTECT );
// 
// 	//--------------------------����Ŷ�!--------------end------------------------------------------
// }

//-------------------------------------
BOOL CUser::SynchroBCMaxLife()//2007918����:ͬ�����Ѫ��
{
	int nMaxLife = GetMaxLife();

	CMsgUserAttrib	msg;
	IF_NOT (msg.Create(GetID(), _USERATTRIB_MAXLIFE, nMaxLife))
		return false;
	
	BroadcastRoomMsg(&msg, EXCLUDE_SELF);//�ų��Լ�
	return true;
}

//-----------------------------------------
BOOL CUser::SynchroBCMaxMana()//2007918����:ͬ���㲥���ħ��
{
	int nMaxMana = GetMaxMana();

	CMsgUserAttrib	msg;
	IF_NOT (msg.Create(GetID(), _USERATTRIB_MAXMANA, nMaxMana))
		return false;
	
	BroadcastRoomMsg(&msg, EXCLUDE_SELF);//�ų��Լ�
	return true;
}


BOOL CUser::SynchroBCMaxSP()// ͬ���㲥������� [�½���11/15/2007]
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
	
	BroadcastRoomMsg(&msg, true);// �㲥��Ϣ�������Լ� [�½���11/15/2007]
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
//20071214����:����ֱ��ʹ��CMsgItemInfo��ͬ��.��װһ��.
BOOL CUser::SynchroItem(CItem* pItem,
						int nItemInfoAction,//����CMsgItemInfo��action
						OBJID idUser/*=ID_NONE*/,//����CMsgItemInfo
						int nWpslAction/*=NULL*/)//����CMsgWeaponSoul��action
{
	//��Ʒ
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
// 	return true;//��ʱ��Ҫ����
// 	//����
// 	CMsgWeaponSoul msgWpsl;
// 	CHECKF(msgWpsl.Create(pItem,nWpslAction,idUser));
// 	this->SendMsg(&msgWpsl);
	return true;
}


//  [1/9/2008 ���]	ָ��ĳ�ظ���
void CUser::RebornInPos(OBJID nMapID, LONG nX, LONG nY, bool bRebornFlag, int nRecoverPercent)
{
	CHECK(nMapID);

	//  [1/27/2008 ���]	�ڼ���ɱ���Է�ʱ,����10����ж�BUFF����,Ȼ��Է�����,,,,�����,���ֶԷ�������Ȼ�������BUFF...
	ClearManyStatusWhenDie();

// 	if (this->IsAlive())
// 		return;

	// ������״̬��ͻ������ [�½���8/25/2007]
	//SetStatus(STATUS_NORMAL);
	DetachStatus( QueryRole(),STATUS_DIE );// ����������״̬ [�½���8/25/2007]
	m_bGhost	= false;

	//2007828����:�������ӿڸĳɲ���-------------
//	if( m_bRebornFlag )//����������ҡ�
//	{
//		SetAttrib( _USERATTRIB_LIFE,MulDiv( GetMaxLife(),m_nRecoverPercent,100 ),SYNCHRO_TRUE );
//		SetAttrib( _USERATTRIB_MANA,MulDiv( GetMaxMana(),m_nRecoverPercent,100 ),SYNCHRO_TRUE );
//		SetAttrib( _USERATTRIB_SP,MulDiv( GetMaxSp(),m_nRecoverPercent,100 ),SYNCHRO_TRUE );
//
//	}
	if( bRebornFlag )//����������ҡ�
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
	// [2007-7-21] ��� 
	//	��BUG0000841, BUG0000960ע��,�����������Ϣ
	//	SetAttrib( _USERATTRIB_LOOKFACE, GetLookFace(), SYNCHRO_BROADCAST );

	const bool bMaxLife = true;
	this->BroadcastTeamLife(bMaxLife);
    
	//06.10.13����:��Ϊ���︴���,�������ͬһ��ͼ��,�������,�����޸Ĵ���
	//----------begin------------------------------------------------------------------------
	int nFlyMapResult = -1;
	nFlyMapResult = this->FlyMap(nMapID, nX, nY);
//	StandTurnon();
	// lock
	//if( FLYMAP_NORMAL == nFlyMapResult ),��Ϊ��EnterMap����������PK����״̬,��������ע���������
	//FLYMAP_MAPGROUP == nFlyMapResultʱ���л���ͼ��,��ʱthis�ѱ�ɾ��,���µ������´���ͻ����
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
// 	if( nMask & 1<<nIndex)//����Ǽ���
// 	{
// 		nMask ^= 1<<nIndex;
// 		CHECKF(nMask>=0 && nMask<100);//ֻ������λ����
// 		int nWpSlState = pItem->GetStateWPSL();
// 		nWpSlState = nWpSlState - pItem->GetWpSlStateActive()*10;//ȥ��ԭactive
// 		nWpSlState = nWpSlState + nMask*10;//������active
// 		pItem->SetStateWPSL(nWpSlState);
// 	}
// 	pItem->SetWpSlAmount(pItem->GetWpSlAmount()-1);
// 	pItem->SetIdWPSLByIndex(nIndex,0);
// 	pItem->EraseWpSlByID(stWeaponSoulData.ID);
// 
// 	if(AwardItemByWpSlID( &stWeaponSoulData, bSendMail))
// 	{
// 		//20071214����:ͬ���޸�-----------
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

//  [1/27/2008 ���]
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

		if( pItem->GetKidnap() || pItem->IsKidnapWhenPickup())//-------���---2007/04/09---�жϰ�װ�����������--
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

///���ߺ����ؽ�����
void CUser::RebuildTeam()
{
	DEBUG_TRY
		///ͨ���Լ���id�ҵ������Ķ���,������ڵĻ�
	CTeam* pTeam	= NULL;
	UINT nTeamCount=UserManager()->GetTeamAmount();
	UINT nMemberCount=0;
	OBJID idMember=ID_NONE;
	for(int i=0;i<nTeamCount;++i)
	{
		pTeam=UserManager()->GetTeamByIndex(i);
		CHECK(pTeam);///���Բ������

		for(int i = 0 ;i<_MAX_TEAMAMOUNT ; i++)
		{
			if(pTeam->m_TeamIn[i].id ==GetID())
			{
 				pTeam->SetOnlineStatus(GetID(),true,true);///����״̬���ó�����
 				SetTeam(pTeam);///�ҵ���֯��,�ؽ�����ɳɹ�
 				break;
			}
		}
	}
	// 	if(GetTeam())
	// 	{
	// 		///���߼���ѭ������ȡ�����Ա�����
	// 		///������Ϣͬ�����Լ��Ͷ�����������
	// 		SendTeamMemberInfo();
	// 	}
	DEBUG_CATCH("CUser::RebuildTeam()")
}

int CUser::ProcGift1001(IRole* pTarget,int nType, int oldValue/*ԭʼֵ, �ڴ�ֵ�����ϼӳɻ��ֵ*/,int valueMask /*= valueMask_ALL*/)
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

		//��ЩûӰ��
		if(pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dotype) != nType)
			continue;
		
		//���������ж�
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
		::LogSave("int CUser::ProcGift1001  ������һ��С��0��ֵ");
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

	m_BuffAtt1 = 0;//��
	m_BuffAtt2 = 0;//��
	m_BuffAtt3 = 0;//��
	m_BuffAtt4 = 0;//��
	m_BuffDef1 = 0;//��
	m_BuffDef2 = 0;//��
	m_BuffDef3 = 0;//��
	m_BuffDef4 = 0;//��
//	return;
 	m_dwBuffAddStr += AdjustUserAttrib(GetStrUserOnly(),ADJUSTUSERATTRIB_STR);//����
 	m_dwBuffAddSta += AdjustUserAttrib(GetStaUserOnly(),ADJUSTUSERATTRIB_STA);//����
 	m_dwBuffAddSpi += AdjustUserAttrib(GetSpiUserOnly(),ADJUSTUSERATTRIB_SPI);//����
 	m_dwBuffAddInt += AdjustUserAttrib(GetIntUserOnly(),ADJUSTUSERATTRIB_INT);//����
 	m_dwBuffAddAgi += AdjustUserAttrib(GetAgiUserOnly(),ADJUSTUSERATTRIB_AGI);//����
 	m_dwBuffAddhp  += AdjustUserAttrib(GetMaxLifeUserOnly(),ADJUSTUSERATTRIB_MAXLIFE);//�������ֵ
 	m_dwBuffAddmp  += AdjustUserAttrib(GetMaxManaUserOnly(),ADJUSTUSERATTRIB_MAXMANA);//���ħ��
 	m_dwBuffAddcrihit += AdjustUserAttrib(GetCritUserOnly(),ADJUSTUSERATTRIB_Crit);//����
 	m_dwBuffAddphyatt += AdjustUserAttrib(GetAtkUserOnly(),ADJUSTUSERATTRIB_PHYACK);//�﹥
 	m_dwBuffAddmagatt += AdjustUserAttrib(GetMgcAtkUserOnly(),ADJUSTUSERATTRIB_MAGACK);//ħ��
 	m_dwBuffAddphydef += AdjustUserAttrib(GetDefUserOnly(),ADJUSTUSERATTRIB_PHYDEF);//���
 	m_dwBuffAddmagdef += AdjustUserAttrib(GetMagicDefUserOnly(),ADJUSTUSERATTRIB_MAGDEF);//ħ��
 	m_dwBuffAddhit	  += AdjustUserAttrib(GetAtkHitRateUserOnly(),ADJUSTUSERATTRIB_HITRANGE);//����
 	m_dwBuffAdddodge  += AdjustUserAttrib(GetDdgUserOnly(),ADJUSTUSERATTRIB_DOG);//����
 	m_dwBuffAddTough  += AdjustUserAttrib(GetToughUserOnly(),ADJUSTUSERATTRIB_TOUGH);//����

	m_BuffAtt1 += AdjustUserAttrib(m_equipAtt1+ m_GiftAtt1 + m_WPSLAtt1, ADJUSTUSERATTRIB_FIREATK);//��
	m_BuffAtt2 += AdjustUserAttrib(m_equipAtt2+ m_GiftAtt2 + m_WPSLAtt2, ADJUSTUSERATTRIB_ICEATK);//��
	m_BuffAtt3 += AdjustUserAttrib(m_equipAtt3+ m_GiftAtt3 + m_WPSLAtt3, ADJUSTUSERATTRIB_MYSTICATK);//��
	m_BuffAtt4 += AdjustUserAttrib(m_equipAtt4+ m_GiftAtt4 + m_WPSLAtt4, ADJUSTUSERATTRIB_NOXATK);//��
	m_BuffDef1 += AdjustUserAttrib(m_equipDef1+ m_GiftDef1,ADJUSTUSERATTRIB_FIREDEF);//��
	m_BuffDef2 += AdjustUserAttrib(m_equipDef2+ m_GiftDef2,ADJUSTUSERATTRIB_ICEDEF);//��
	m_BuffDef3 += AdjustUserAttrib(m_equipDef3+ m_GiftDef3,ADJUSTUSERATTRIB_MYSTICDEF);//��
	m_BuffDef4 += AdjustUserAttrib(m_equipDef4+ m_GiftDef4,ADJUSTUSERATTRIB_NOXDEF);//��
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
	//goto �Գ�����Ч�ı�������,����ɼ��ܵ�
	m_GiftPetAtk	= 0;//�⹦
	m_GiftPetMagAtk	= 0;//�ڹ�
	m_GiftPetCrihit	= 0;//����
	m_GiftPetMaxLife= 0;//Ѫ����

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
	//goto ���ɵ��Ǽ������ⱻ������,�߻�Ҫ�����а��ڶ�����ʹ�ô�����ɼ���
//	int nSynRank = QuerySynRank();
//	if (GetSynID() != ID_NONE)
//	{
//		if (nSynRank%10==1 || nSynRank>=90)//������
//		{
//			if (QueryMagic(2103))
				m_GiftPetAtk += ProcGift1001(this,DOTYPE_petAddAtk,0,valueMask_CONST);
//		}
//		if (nSynRank%10==2 || nSynRank>=90)//�׻���
//		{
//			if (QueryMagic(2203))
				m_GiftPetCrihit += ProcGift1001(this,DOTYPE_petAddCrihit,0,valueMask_CONST);
//		}
//		if (nSynRank%10==3 || nSynRank>=90)//������
//		{
//			if (QueryMagic(2303))
				m_GiftPetMagAtk += ProcGift1001(this,DOTYPE_petAddMagAtk,0,valueMask_CONST);
//		}
//		if (nSynRank%10==4 || nSynRank>=90)//��ȸ��
//		{
//			if (QueryMagic(2403))
				m_GiftPetMaxLife += ProcGift1001(this,DOTYPE_petAddMaxLife,0,valueMask_CONST);
//		}
//	}

	//goto ������ֵ����ȷ��
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
	if (m_dwBuffAddStr<0 && (GetStrUserOnly() + m_equipStrTotal+ m_dwGiftAddStr + m_WPSLStr)<abs(m_dwBuffAddStr))//��ֹ�޷�����������
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
		m_dwUserAtt1 = m_equipAtt1+ m_GiftAtt1 + m_BuffAtt1 + m_WPSLAtt1 + m_eudemonSoul1;//��

	if (m_BuffAtt2<0 && (m_equipAtt2+ m_GiftAtt2 + m_WPSLAtt2 + m_eudemonSoul2)<abs(m_BuffAtt2))
		m_dwUserAtt2 = 0;
	else
		m_dwUserAtt2 = m_equipAtt2+ m_GiftAtt2 + m_BuffAtt2 + m_WPSLAtt2 + m_eudemonSoul2;//��

	if (m_BuffAtt3<0 && (m_equipAtt3+ m_GiftAtt3 + m_WPSLAtt3 + m_eudemonSoul3)<abs(m_BuffAtt3))
		m_dwUserAtt3 = 0;
	else
		m_dwUserAtt3 = m_equipAtt3+ m_GiftAtt3 + m_BuffAtt3 + m_WPSLAtt3 + m_eudemonSoul3;//��

	if (m_BuffAtt4<0 && (m_equipAtt4+ m_GiftAtt4 + m_WPSLAtt4 + m_eudemonSoul4)<abs(m_BuffAtt4))
		m_dwUserAtt4 = 0;
	else
		m_dwUserAtt4 = m_equipAtt4+ m_GiftAtt4 + m_BuffAtt4 + m_WPSLAtt4 + m_eudemonSoul4;//��

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

	//goto ������ֵ����ȷ��
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

	//ͬ��Ѫ��ŭ
	int nMaxLife = this->GetMaxLife();
	int nMaxMana = this->GetMaxMana();
	int nMaxSp = this->GetMaxSp();
	
	CMsgUserAttrib	msgAtt;

	//�����ȷ�
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
	BroadcastRoomMsg(&msgAtt, EXCLUDE_SELF,EM_SENDNPCFLAG_FALSE);//�ų��Լ�
	
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


//[��;���� 2008/10/13]���õ�ǰʹ�õĳƺ�
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

//[��;���� 2008/10/13]���õ�ǰӵ�еĳƺ�
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


//[������ܡ�2008/12/4] ���ݿͻ��˴���������ƷID����ȡ����Ʒ,��������Ʒ��Ϣ���͵��ͻ���
bool CUser::GetItemInfoByItemid(OBJID idItem)
{
	vector<OBJID>::iterator it;

	it = find(NoItemId().begin(),NoItemId().end(),idItem);

	//�Ѿ��ҵ�
	if (it != NoItemId().end())  
	{
		this->SendSysMsg("�޷���ȡ�õ��ߵ���Ϣ��������Ϣ��������ϵ!");
		return true;
	}

	//���û�ҵ�
	CItem* pItem = NULL;
	pItem = CItem::CreateNew();
	if (!pItem)
		return false;
	if (!pItem->Create(idItem,Database()))
	{
		pItem->ReleaseByOwner();

		NoItemId().push_back(idItem); //����ʧ�ܱ�ʾ������Ʒ���������ݿ��У���Ȼ������ݿ��쳣����Ҳ�������������ɣ�����ûʲôӰ���
		this->SendSysMsg("�޷���ȡ�õ��ߵ���Ϣ��������Ϣ��������ϵ!");
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


bool CUser::CheckFreezeDoubleExp(UINT nBufID)//����˫������
{
	DEBUG_TRY
	if (!QueryStatusManager().CheckRestoreFreezeStatusDB(this, Database(), nBufID))
		return false;
	DEBUG_CATCH( "CUser::CheckFreezeDoubleExp" )	
	return true;
}

bool CUser::FreezeDoubleExp(UINT nBufID)//����˫������
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
bool CUser::RestoreDoubleExp(UINT nBufID)//�ָ������˫������
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
//���ֳ�����ͼ����
OBJID CUser::GetBornMap()
{
	OBJID idMap= 0;
	//���ֳ�����ͼ������,��ʼ����һ���ϴ����,����Ѳ����ڵĵ�ͼ�����������ٵĵ�ͼ
	const short nBornMapAmount = 5;//���ֵ�ͼ����
	DWORD dwUserAmount[nBornMapAmount] = {9999999, 9999999, 9999999, 9999999, 9999999};
	OBJID idMapArr[nBornMapAmount] = {1001, 1034, 1036, 1038, 1040};//����,����1,����2,����3,����4
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
	//�߻�Ҫ��û�е�ͼС��80��ʱ�ĳ����ѡ��,������ѡ�����ٵ�
	if (!idMap)
	{	//���ʱ����������0
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
				LOGWARNING("����Ʒ��ʱ����Ʒλ�����⣬��������itemid = %u userid = %u",pEquip->GetID(),GetID());
			setItems.push_back(i);
		}
	}
	nStep = 2;
	int nTotalItemCount = setItems.size();
	{
		//srand( ( unsigned )time( NULL ) );
		// ���ѡ���__min(nDropNum, nTotalItemCount)����Ʒ
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
				LOGWARNING("����Ʒ��ʱ����Ʒλ�����⣬��������itemid = %u userid = %u",pItem->GetID(),GetID());

			nStep = 6;
			if (pItem && pItem->IsSoulMark())//goto ��ӡ�Ĳ��ܵ�
				continue;
			nStep = 7;
			if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from red_killed", GetID(), GetName(), pItem->GetTypeID());

			nStep = 8;
			if (pItem)
			{
				if( /*pItem->GetKidnap() ||*/ pItem->IsNeverDropWhenDead())//-------���---2007/04/09---�жϰ�װ�����������--
					continue;
				if (this->EraseEquip(idItem, true))
				{
					++nRealDropNum;
					//						m_pOwner->SendSysMsg("��Ϊһ�������ɱ�����ﷸ���㽫����������ϵ�һ���ǰ󶨵���Ʒ");
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
	if (msg.Create("ϵͳ", this->GetName(), szWords, szDate, 0xff0000, _TXTATR_MAYBEMAIL, 0, 0, 0, " ", 0))
		pTarget->QueryOwnerUser()->SendMsg(&msg);
	DEBUG_CATCH("sendemail crash!");
}

bool CUser::IsAwardTimeOut()
{
// 	if(!m_awardcheck.IsActive())
// 		SendSysMsg("��ʱ��δ��ʼ");
// 	if(!m_awardcheck.IsTimeOut())
// 		SendSysMsg("ʣ��ʱ��%d��",m_awardcheck.GetRemain());
	return m_awardcheck.IsTimeOut();
}

void CUser::SetNextAwardTime(int nsecs)
{
     m_awardcheck.Clear();
	 m_awardcheck.Startup(nsecs);
}


void CUser::InitAwarTime(bool bLogin)
{

	m_data.setFati(1);			//��ʼ��������״̬������δ���˺ŷ�����ʱ����
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
		sprintf(szAwardMsg,"����%s����",szName);
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
			sprintf(szMsg, "<%s>���ȵĿ��Ŵ��̳��ﾫ����ѡ�ı�����������������������<%s>��������̫�ɰ��ˣ�", UserName ,itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
		}
		if (nEudemonType == 918)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
			SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>�����˵�̳����ڲ���ϡ������ϴ˷���Ȼ��<%s>����Ψһ�����γ������������Ľ��", UserName ,itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
		}
		if (nEudemonType == 919)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>��<3_���������_7>������������˺���<3_���������_7>ͷҲ���صľ����ˣ�����һ��<%s>��Ϊ��λӢ�۶�Ů�Ľ��㣡", UserName ,itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
		}
		if (925 < nEudemonType && 930 > nEudemonType)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>��ϧ�ؽ���̳ǹ��ý����о���������<%s>���������˳��ۣ�", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
		}
		if (nEudemonType == 930)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "���Ŵ󼪣�һ����ʵĹ�â��ҫ��<%s>���ϣ�����������ǰ�Ĳ����ǿɰ��ı���<%s>�����Ǻ��˵�ͷ�������ܵ���", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);
			
		}
		if (nEudemonType == 934)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, " ���������ԵĴ�������<%s>ѱ����<%s>��Ī�����������ж��������ĵ�һ�ˣ���", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);	
		}
		if (nEudemonType == 936 || nEudemonType == 937)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>��������̳����ڲ�ף���䱦���������ϴ˷����Ѱ��ֻ<%s>�����Ǻø�����", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE,this, pEudemonType->GetID(), nEudemonType, NULL, ALLUSERS_NAME);	
		}
		if (nEudemonType == 935)
		{
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTUSEREDU,  szName, EudId, nEudemonType, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, " <%s>�������������������ɾʹӾ۲������Ѱ��<%s>��������ɷ���ˣ�", UserName, itemName);
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
			this->SendSysMsg("ͷ����ɹ���");
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
		this->SendSysMsg("ͷ������ʱ��ɹ���");
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
//goto �����״̬
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
			SendSysMsg("��ȡ̫�죬�����������ȡ");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	��һ�μ��
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);
	
	if(ChkNewGiveMask(36))
	{
		SendSysMsg("���Ѿ���ȡ��ͶƱ���������ظ���ȡ");
		return;
	}
	
	if(strlen(szCard) != 18)
	{
		SendSysMsg("����Ŀ���");
		return;
	}
	
	if(strncmp(szCard,"YJTP-",5) != 0)
	{
		SendSysMsg("����Ŀ���");
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
			SendSysMsg("��ȡ̫�죬�����������ȡ");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	��һ�μ��
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);

	if(strlen(szCard) != 18)
	{
		SendSysMsg("����Ŀ���");
		return;
	}

	int nType = 0;
	if(strncmp(szCard,"LQIS-",5) != 0) //ͭ
	{
		SendSysMsg("����Ŀ���");
		return;
	}

	if(ChkNewGiveMask(37))
	{
		SendSysMsg("���Ѿ���ȡ�����Ῠ�������ظ���ȡ");
		return;
	}
//		nType = 1;
//	}
// 	else if(strncmp(szCard,"DWAN-",5) == 0) //��
// 	{
// 		if(ChkNewGiveMask(38))
// 		{
// 			SendSysMsg("���Ѿ���ȡ��������������ظ���ȡ");
// 			return;
// 		}
// 		nType = 2;
// 	}
// 	else if(strncmp(szCard,"ELWG-",5) == 0) //��
// 	{		
// 		if(ChkNewGiveMask(39))
// 		{
// 			SendSysMsg("���Ѿ���ȡ����𿨣������ظ���ȡ");
// 			return;
// 		}
// 		nType = 3;
// 	}
	
// 
// 	if(nType == 0)
// 	{
// 		SendSysMsg("����Ŀ���");
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
		SendSysMsg("����Ŀ���");
		return false;
	}

	int nType = 0;
	
	if(strncmp(szCard,"LQIS-",5) != 0) //ͭ
		return false;

// 	if(strncmp(szCard,"DWAN-",5) == 0) //��
// 	{
// 		nType = 2;
// 	}
// 	if(strncmp(szCard,"ELWG-",5) == 0) //��
// 	{
// 		nType = 3;
// 	}
// 	
// 	
// 	if(nType == 0)
// 	{
// 		SendSysMsg("����Ŀ���");
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
			SendSysMsg("��ȡ̫�죬�����������ȡ");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	��һ�μ��
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);

	if(strlen(szCard) != 18)
	{
		SendSysMsg("����Ŀ���");
		return;
	}

	if(ChkNewGiveMask(32))
	{
		SendSysMsg("���Ѿ���ȡ��ͬ���ͻ���������ظ���ȡ");
		return;
	}

	int nType = 0;
	if(strncmp(szCard,"HDWX-",5) == 0) //С
		nType = 1;
	else if(strncmp(szCard,"HDWZ-",5) == 0) //��
		nType = 2;
	else if(strncmp(szCard,"HDWD-",5) == 0) //��
		nType = 3;	

	if(nType == 0)
	{
		SendSysMsg("����Ŀ���");
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
		SendSysMsg("����Ŀ���");
		return false;
	}

	int nType = 0;
	
	if(strncmp(szCard,"HDWX-",5) == 0) //С
	{
		nType = 1;
	}
	else if(strncmp(szCard,"HDWZ-",5) == 0) //��
	{
		nType = 2;
	}
	else if(strncmp(szCard,"HDWD-",5) == 0) //��
	{
		nType = 3;
	}
	
	
	if(nType == 0)
	{
		SendSysMsg("����Ŀ���");
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
			SendSysMsg("ʱװ��ʾģʽ�Ѹı�");
		else
			SendSysMsg("ʱװ��ʾģʽ�ı�ʧ��");
	}
	else
	{
		if(m_pUserKey.SetDress(oldDress & nDress))
			SendSysMsg("ʱװ��ʾģʽ�Ѹı�");
		else
			SendSysMsg("ʱװ��ʾģʽ�ı�ʧ��");
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
		year=cardId.substr(6,4);//ȡ18λ���֤�е���
		month=cardId.substr(10,2); // ��
		day=cardId.substr(12,2);   //��
    }
    else
    {
		year=cardId.substr(6,2);//ȡ15λ���֤�е���
		month=cardId.substr(8,2); // ��
		day=cardId.substr(10,2);   //��
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
			error=false;  //�������з������ַ�
	}
    if(!BirthdayIsRight(card))
	{
		error=false; //�������´���
	}
	if(length==18)     //��֤18λ���֤����
	{
		int no[]={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
		char id[]={'1','0','X','9','8','7','6','5','4','3','2'};
		//1 0 X 9 8 7 6 5 4 3 2   У����
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
				error=false;//���֤���һλ�������
			}
		}
		wi=sum%11;
		if(*(p+17)=='x'||*(p+17)=='X')            //���һλΪ'x'��'X';
		{
			if(id[wi]!='x'&&id[wi]!='X')
				error=false;
		}
        else if(id[wi]!=*(p+17))     //�жϼ���������֤У�����������Ƿ����
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

UINT CUser::PVPEmendDamage(UINT nLoseLife, OBJID AttId, bool bOverPVP /*= true*/)//���������˺�
{
	UINT nRealLoseLife =0;
	DEBUG_TRY
	if (!IsPVPStuts())
		return nLoseLife;
	nRealLoseLife = nLoseLife;
	UINT nOldLife = GetLife();
	if(nOldLife > nLoseLife)	//��û����
		return nRealLoseLife;
	
	IRole* pRoleTemp = RoleManager()->QueryRole(AttId);
	if (!pRoleTemp)
	{
		::LogSave("������ȱʧ1, BeAttUserid = %u, AttUserid = %u", GetID(), AttId);
		return 0;
	}
	if(pRoleTemp->IsUser())				//�˳���
	{
		if(GetPVPTargetID() == AttId)
		{
			CUser* pAtt = NULL;
			if(pRoleTemp->QueryObj(OBJ_USER,IPP_OF(pAtt)))
			if (!pAtt)
			{
				::LogSave("������ȱʧ2(�����������), BeAttUserid = %u, AttUserid = %u", GetID(), AttId);
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
	else	//�������
	{
		CMonster* pAtt = NULL;
		if(pRoleTemp->QueryObj(OBJ_MONSTER,IPP_OF(pAtt)))
		if (!pAtt)
		{
			::LogSave("������ȱʧ3(�������ǳ���), BeAttUserid = %u, AttUserid = %u", GetID(), AttId);
			return 0;
		}
		if (!pAtt->IsEudemon())	//������ǳ���
		{
			return nRealLoseLife;
		}
		CUser* pEudOwner = pRoleTemp->QueryOwnerUser();
		if (!pEudOwner)
		{
			::LogSave("����ʱ����û�����ˣ�BeAttUserid = %u, AttEudid = %u", GetID(), AttId);
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
		::LogSave("CUser::GetPassedTimeFromCreate ��������(%s)", szTime);
		return 0;
	}

	DWORD nCreateDate = m_data.GetCreateTime();
	if (nCreateDate == 0)
	{
		::LogSave("CUser::GetPassedTimeFromCreate ��ɫ��������Ϊ0,userid=%u", GetID());
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
		::LogSave("CUser::GetPassedTimeFromCreate �������ڸ�ʽ����,userid=%u,createDate=%u", GetID(), nCreateDate);
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
			SendSysMsg("�ű����˲�������Ľ�Ǯ��");
		}
		return;
	}
	if (nMoney > 5000000)
	{
		::LogSave("AddMoneyByType�ӿڸ�Ǯ����500W����");
		if (IsGM())
		{
			SendSysMsg("AddMoneyByType�ӿڸ�Ǯ����500W����");
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
	m_data.SetMonsterkill(0);	//���ù������
	m_data.SetEverydayOL(0);	//����ÿ������
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
	
	m_data.SetEverydayOL((nOldTime + 1));//ÿ���Ӽ�һ��
	if ((nOldTime + 1) >= 4*60)//4Сʱ
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
		::LogSave("������䷽����");
		return -1;
		break;
	}

	if(!TypeID)
		return 0;
	CItem* pItem = GetItemByType(TypeID);
	if (!pItem)
	{
		::LogSave("��ҷ�������һ������û�еĵ���iduser=%u, typeid=%u", GetID(), TypeID);
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


	//�����ֵ�޸Ľ���
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
		LOGWARNING("�����Ʒʧ�ܣ��޷����뱳����userid = %u itemtype = %u",GetID(),pItem->GetTypeID());
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

	SendSysMsg(_TXTATR_TOPSHOW, "������%u����Ԫ", nData);
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

//////////////////////////����ף�����////////////////////////////////////////////
void CUser::InitOnlineGift(OBJID Gift1ID,OBJID Gift2ID,OBJID Gift3ID,OBJID Gift4ID,OBJID Gift5ID)//�ű��ӿڣ������Ʒid
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

void CUser::ProcessOnlineGift()//�ж�
{
	OBJID nDate = TimeGet(TIME_MINUTE);
	CHECK(m_pOnlineGift);
	if (m_pOnlineGift->GetDate() <= nDate)
	{
		if (m_pOnlineGift->GetDate() == nDate && m_pOnlineGift->GetTaskMask() == 2)
		{
	    	//SendSysMsg("��������ȡף����");
			return;
		}
		if (m_pOnlineGift->GetTaskMask() == 1)
		{
		//	SendSysMsg("�����ѳ�ȡף����");
			return;
		}
		if(m_pOnlineGift->GetTaskMask() == 0)
		{
			ProcessAction(500400);
		}
	}
}
bool CUser::GiveOnlineGift()//��������
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
				SendSysMsg("�����ռ䲻�㣡");
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
			pitem=AwardItem(m_pOnlineGift->GetGiftID(GIFTDATA_GIFT5ID), true, false, true, false, nGift,false, true, false, 1);//�߻�Ҫ��ÿ���������˶����������
			CHECKF(pitem);
		}
		
		for (int i=0;i<rounds;i++)
		{
			ObjID[i]=m_pOnlineGift->GetGiftID(GIFTDATA_GIFT1ID+i);
			int nType = ObjID[i]/1000000;//�õ����� 1�Ǿ��飬2����Ʒ
			int nData = ObjID[i]%1000000;//�õ�id�����Ǿ���
			switch(nType)
			{
			case 1:
				{
					AwardBattleExp(nData,false,false);
					SendSysMsg("������%d����",nData);
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

bool CUser::ChkItem(int nMustGive)//�����ж��ټ���Ʒ
{
	int nPak1=0, nPak2=0,nPak3=0;//,nPakPet=0;
	for (int i=0;i<4;i++)
	{
		OBJID gift = m_pOnlineGift->GetGiftID(GIFTDATA_GIFT1ID+i);
		if (!gift)
			continue;
		OBJID objid=gift%1000000;
		int nType = gift/1000000;//�õ����� 1�Ǿ��飬2����Ʒ
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
			SendSysMsg("IninOnlineGift �ű�����һ�������������ֵ��");
		}
		return true;
	}
	return false; 
}
bool CUser::IsGive(int nTask)//�ű��ӿڣ�
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
				//SendSysMsg("����1��");
				return false;
			}
			else
			{
				//SendSysMsg("С��1��");
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
//��ѯ�Ǽǵ�ʦ���б�
void CUser::QueryTutorList( USHORT nPage, int nPageSize )
{
	CHECK(nPageSize);
	TUTORLIST arrData[20] = {0};
	int nDataSize = g_TutorListSystem.GetInfo( arrData,nPageSize,nPage );
	if( -1 == nDataSize )
	{
		SendSysMsg("�б������");
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
//��Ҽ���ʦ���б�
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
//��ʦ���б���ɾ��
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
//���ɹ���

bool CUser::ChangeLineMap(OBJID MapID,int nPosx,int nPosy,int nLine)//��ָ������·����ͼ--jay
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
		LogSave("ChangeLineMap(OBJID MapID,int nPosx,int nPosy,int nLine)��������ֵ: MapID=%u,nPosx=%u,nPosy=%u,nLine=%u",MapID,nPosx,nPosy,nLine);
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
bool CUser::EnterSynMap(int nPosx,int nPosy)//������ɵ�ͼ--jay
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
void CUser::SetSynTaskGift(OBJID nGift1,OBJID nGift2)//���뽱������--jay
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
int CUser::GetGiftMask(bool bGive)//�жϽ�������
{
	if(bGive)
	{
		if (m_pSyn->GetMask() == 1)
		{
			int nPak1=0, nPak2=0,nPak3=0;
			OBJID gift = m_pSyn->GetGift1();
			if (!gift)
				return -1;//����
			int nPosition = QueryPackage()->GetAvailablePosition(gift);
			if (nPosition==ITEMPOSITION_BACKPACK1)
				nPak1++;
			else if (nPosition==ITEMPOSITION_BACKPACK2)
				nPak2++;
			else if (nPosition==ITEMPOSITION_BACKPACK3)
				nPak3++;
			else
			{
				return -1;//����
			}
			if ( (nPak1>0 && QueryPackage()->IsPackFull(nPak1, 0, ITEMPOSITION_BACKPACK1))
				|| (nPak2>0 && QueryPackage()->IsPackFull(nPak2, 0, ITEMPOSITION_BACKPACK2))
				|| (nPak3>0 && QueryPackage()->IsPackFull(nPak3, 0, ITEMPOSITION_BACKPACK3)))
			{
				return 1;//������
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
int CUser::GetSynSci(int nSub)//1.���� 2.�׻� 4.��ȸ 3.����
{
	CHECKF(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return -1;
	return psyn->GetLev(nSub);
}
int CUser::GetNowWeekDay()//--ע��1Ϊ������,7Ϊ������
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

}//��ʷ��Ǯ��
void CUser::SetHistorySynGrow(int nGrow)
{
	CHECK(nGrow>=0);
	CHECK(QuerySynAttr());
	CSynPtr psyn = SynManager()->QuerySyndicate(this->GetSynID());
	if(!psyn)
		return;
	QuerySynAttr()->SetHistoryGrow(nGrow);
	
}//��ʷ�ܽ���

int CUser::GetMySynRank()//�ű��ӿ�-��ȡ�������ְλ
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
		LogSave("���븺����Ǯ!");
		return;
	}
	int newMoney=0;
	if (nMoney>psyn->GetMoney())
	{
		newMoney=nMoney-psyn->GetMoney();
		if (newMoney<0)
		{
			LogSave("������Ǯ!");
			return;
		}
		SendSysMsg("��ð����ʽ�:%d��%d��%dͭ",newMoney/10000, (newMoney-((newMoney/10000)*10000))/100,newMoney % 100 );
		SetHistorySynMoney(GetHistorySynMoney()+nMoney-psyn->GetMoney());
	}
	else
	{
		newMoney=psyn->GetMoney()-nMoney;
		if (newMoney<0)
		{
			LogSave("������Ǯ!");
			return;
		}
		SendSysMsg("���İ����ʽ�:%d��%d��%dͭ",newMoney/10000, (newMoney-((newMoney/10000)*10000))/100,newMoney % 100);
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
		SendSysMsg("���%d���ɽ����",nData-psyn->GetGrow());
		SetHistorySynGrow(GetHistorySynGrow()+nData-psyn->GetGrow());
	}
	else
		SendSysMsg("����%d���ɽ����",psyn->GetGrow()-nData);
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
			SendSysMsg("���ɽ�Ǯ���ܳ��������޷����ף�");
			if (EndFlag)
			{
				Contribute();
			}
			return false;
		}
		else	if (!SpendMoneyByType(dwMoney, 1, true))
		{
			this->SendSysMsg("�����ϵĽ�Ǯ���㣡");
			return false;
		}
	}	

	if (EndFlag == 0)
	{
		OBJID OldMoney = pSyn->GetInt(SYNDATA_MONEY);
		OBJID NewMoney = OldMoney + m_nConMoney + dwMoney;
		if (NewMoney > (pSyn->GetDeadline(5)))
		{
			SendSysMsg("���ɽ�Ǯ���ܳ��������޷����ף�");
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
				this->SendSysMsg("ɾ����Ʒ����");
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
			SendSysMsg("���ɽ�Ǯ���ܳ��������޷����ף�");
			Contribute();
			return false;
		}
        if(idItem && !this->EraseItem(idItem, SYNCHRO_TRUE))
		{
			this->SendSysMsg("ɾ����Ʒ����");
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

void CUser::SaveLog(int nType,OBJID nValue,bool isGet,int nGetWay)//���Ÿ��ű�,�����־
{
	switch(nType)
	{
		case 4 ://�鱦��־
		{
			if (nValue>30)//����30�鱦�Ŵ���־
			{
				switch(nGetWay)
				{
					case 1://���ɻ��
					{
						if (isGet)//���
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
		this->SendSysMsg("�Ҳ�����Ʒ");
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

	if(nItemType==_SPCITEM_VASEXCHANGE || nItemType == 660000)//Ԫ��Ʊ���ܾ� //ħ�涾˿Ҳ��
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
			this->SendSysMsg("ɾ����Ʒ����");
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
			this->SendSysMsg("���ɽ�����Ѿ��ﵽ�����޷����ף�");
			Contribute();
			return false;
		}
		if(!this->EraseItem(idItem, SYNCHRO_TRUE))
		{
			this->SendSysMsg("ɾ����Ʒ����");
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
	int  nType=0;//1�ǽ���2���ʽ�
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
		::LogSave("�����Ľ����");
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
		::LogSave("�����Ľ�Ǯ");
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
				SendSysMsg("���ɽ���Ȳ��ܳ������ޣ�");
			if (nType==2)
				SendSysMsg("���ɽ�Ǯ���ܳ������ޣ�");
			SendSysMsg("����ʧ�ܣ�");
		}
		else
			SendSysMsg("���׳ɹ���");
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
			SendSysMsg("��ȡ̫�죬�����������ȡ");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	��һ�μ��
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);

	if(ChkNewGiveMask(35))
	{
		SendSysMsg("���Ѿ���ȡ�����ֿ��������ظ���ȡ");
		return;
	}



	if(strcmp(szCard,"70FF6E5E7AE772B6") != 0)
	{
		SendSysMsg("����Ŀ���");
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
			SendSysMsg("��ȡ�ɹ�");
		}
		else
		{
			SendSysMsg("��ȡʧ��");
		}
		DEBUG_CATCH("pUser->ProcessAction(2035)");
	}
}
bool CUser::WarGameSignup(OBJID idWarGame)
{
	CHECKF(idWarGame);
	CHECKF(m_pUserWarGame);

	if(m_pUserWarGame->HasArenaMask())//�Ѿ�ȥ����������
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����Ѿ�ȥ����������");
		return false;
	}
	if(!WargameManager()->CanSignup(idWarGame))//δ������ʱ��
	{
		SendSysMsg(_TXTATR_TOPSHOW, "δ������ʱ��");
		return false;
	}

	if(WargameManager()->IsHasSignup(idWarGame, GetID()))//�Ѿ���������
	{
		SendSysMsg(_TXTATR_TOPSHOW, "���Ѿ���������");
		return false;
	}

	if(WargameManager()->IsMaxSigninPlayer(idWarGame))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "������������");
		return false;
	}
	if(WargameManager()->Signup(idWarGame, this))
		SendSysMsg(_TXTATR_TOPSHOW, "�����ɹ�");
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
	//�Ѿ��ھ�������
	if(QueryStatus(STATUS_WARGAME))
		return false;
	nStep = 1;
	if(!WargameManager()->CanUserEnter(idWarGame))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�Ѿ����˽���ʱ��");
		return false;
	}
	nStep = 2;
	if(g_SpecilLine != 1)
	{
		if(m_pUserWarGame->HasArenaMask())//�Ѿ�ȥ����������
		{
			SendSysMsg(_TXTATR_TOPSHOW, "������Ѿ�ȥ����������");
			return false;
		}
		nStep = 3;
		ChangeLineMap(20000 + idWarGame, 0, 0, SPECILLINE);
		return true;
	}
	nStep = 4;
	if(WargameManager()->IsMaxPlayer(idWarGame))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "������������");
		return false;
	}
	nStep = 5;
	//�˳����飬�������������ǲ�ͬ��Ӫ
	if(CanBeSee() && GetTeam())
		QuitTeam();
	nStep = 6;
	bRet = WargameManager()->EnterWar(idWarGame, this);
	DEBUG_CATCH2("CUser::EnterWarGame nStep=%d", nStep)
	return bRet;
}

bool CUser::EnterWarGameFromNpc(OBJID idWarGame)
{
	if(!WargameManager()->IsHasSignup(idWarGame, GetID()))//�Ѿ���������
	{
		SendSysMsg(_TXTATR_TOPSHOW, "����û�б���");
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
		SendSysMsg("���ĵȼ�δ��18�����������ó��︽��");
		return false;
	}
	if(m_idSoulEudemon != ID_NONE)
	{
		SendSysMsg("�Ѿ���һ�����︽����");
		return false;
	}
	if(GetCallPetID() == idEudemon)
	{
		SendSysMsg("��ս�ĳ��ﲻ�ܸ���");
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
		SendSysMsg("����Ϊ0�ĳ��ﲻ�ܸ���");
		return false;
	}

	pData->SetLock(true);
	pData->SetInt(EUDEMONDATA_state,PETSTATE_ADDIN,false);//����Ϊ����״̬
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
	//��ʾ��Ч
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
	
	//ȥ����Ч
	if(m_bSoulEudemonView)
	{
		m_bSoulEudemonView = false;
		CMsgUserAttrib msgAtt;
		if(msgAtt.Create(GetID(), _USERATTRIB_EUDEMON_VIEW, m_bSoulEudemonView))
			BroadcastRoomMsg(&msgAtt, INCLUDE_SELF);
	}
	return true;
}
//���³��︽����õ�����
bool CUser::UpdateEudemonSoulProperty()
{
	m_eudemonSoulphyatt = 0;//�⹥
	m_eudemonSoulmagatt = 0;//�ڹ�
	m_eudemonSoul1 = 0;//��
	m_eudemonSoul2 = 0;//��
	m_eudemonSoul3 = 0;//��
	m_eudemonSoul4 = 0;//��
	m_eudemonSoulAddHp = 0;//��Ѫ
	m_eudemonSoulAddMp = 0;//����
	m_eudemonSoulAddDodge = 0;//������buff
	m_eudemonSoulAddHitRate = 0;//������buff

	if(m_idSoulEudemon == ID_NONE)//����
	{
		return true;
	}

	double nphyatt = 0;//�⹥
	double nmagatt = 0;//�ڹ�
	double nSoul1 = 0;//��
	double nSoul2 = 0;//��
	double nSoul3 = 0;//��
	double nSoul4 = 0;//��

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
			if(nMagicType % 2 == 1)//�ͼ�����
			{
				nphyatt *= 0.7f;//�⹥
			}
			break;
		case ATKTYPEMODE_INNNER:
			nmagatt = 7.35f * petData.level + 0.06f * nQuality * qualityParam + 23.34f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//�ͼ�����
			{
				nmagatt *= 0.7f;//�ڹ�
			}
			break;
		case ATKTYPEMODE_ICE:
			nSoul2 = 1.093f * petData.level + 0.013f * nQuality * qualityParam + 3.473f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//�ͼ�����
			{
				nSoul2 *= 0.7f;//��
			}
			break;
		case ATKTYPEMODE_FIRE:
			nSoul1 = 1.093f * petData.level + 0.013f * nQuality * qualityParam + 3.473f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//�ͼ�����
			{
				nSoul1 *= 0.7f;//��
			}
			break;
		case ATKTYPEMODE_NOXIOUS:
			nSoul4 = 1.093f * petData.level + 0.013f * nQuality * qualityParam + 3.473f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//�ͼ�����
			{
				nSoul4 *= 0.7f;//��
			}
			break;
		case ATKTYPEMODE_MYSTERY:
			nSoul3 = 1.093f * petData.level + 0.013f * nQuality * qualityParam + 3.473f * petData.fusion * fusionParam;
			if(nMagicType % 2 == 1)//�ͼ�����
			{
				nSoul3 *= 0.7f;//��
			}
			break;
		default://�������ܵĻ��޼���
			{
				if(idMagicType == 8501)
					m_eudemonSoulAddHp = nLifeParam * 0.3;//30%
				else if(idMagicType == 8502)
					m_eudemonSoulAddHp = nLifeParam * 0.5;
				else if(idMagicType == 8503)
					m_eudemonSoulAddMp = nLifeParam * 0.3;
				else if(idMagicType == 8504)
					m_eudemonSoulAddMp = nLifeParam * 0.5;
				else if(idMagicType == 8513)//�ͼ�����
					m_eudemonSoulAddDodge = 0.0665f * petData.level + 0.007f * nQuality * qualityParam + 2.11f * petData.fusion * fusionParam;
				else if(idMagicType == 8514)//�߼�����
					m_eudemonSoulAddDodge = 0.95f * petData.level + 0.01f * nQuality * qualityParam + 3.015f * petData.fusion * fusionParam;
				else if(idMagicType == 8515)//�ͼ�����
					m_eudemonSoulAddHitRate = 0.0665f * petData.level + 0.007f * nQuality * qualityParam + 2.11f * petData.fusion * fusionParam;
				else if(idMagicType == 8516)//�߼�����
					m_eudemonSoulAddHitRate = 0.95f * petData.level + 0.01f * nQuality * qualityParam + 3.015f * petData.fusion * fusionParam;
			}
			break;
		}
	}
	m_eudemonSoulphyatt += nphyatt;//�⹥
	m_eudemonSoulmagatt += nmagatt;//�ڹ�
	m_eudemonSoul1 += nSoul1;//��
	m_eudemonSoul2 += nSoul2;//��
	m_eudemonSoul3 += nSoul3;//��
	m_eudemonSoul4 += nSoul4;//��
	//���
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
