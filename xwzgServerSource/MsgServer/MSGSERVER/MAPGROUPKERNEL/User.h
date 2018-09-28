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
#include "GemCreate.h"//20061010����
#include "TaskDetailData.h"//20061109����
#include "UserGift.h"//20071123����:
#include "StrRes.h"
#include "CityWarMgr.h"
#include "UserPet.h"
#include "CompensationManager.h"
#include "StatusManager.h"
#include "CDManager.h"//-----��� 2007/03/12--- 
#include "FriendContactHistory.h" //2008/12/17�������
#include "UserKey.h"
#include "UserMaterialItem.h"
#include "MsgMarry.h"
#include "UserJinMeiSysData.h"
#include <vector>
#include <deque>
#include <set>
using namespace std;

#include "UserTable.h"		// ���ֱ���
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
//modify code by �ֵ�Ԩ begin 2011.5.30
#include "VasLvData.h"
//modify code by �ֵ�Ԩ end
#include "Verification.h"
////////////////////////////////////////////////////////////////

const int	_MAX_USERFRIENDSIZE				= 50;
const int	_MAX_TUTORSIZE					= 15;
const int	_MAX_MONEYLIMIT					= 1000000000;
const int   _MAX_PKSAVE_VALUE               = 50000;                        //pkֵ����Ʒ���Ӱ�����
const int	USER_PACKAGE_SIZE				= 36;							// �ֿ�������ٶ���
const int	USERDROPITEM_RANGE				= 5;							// �Զ���
const int	USERDROPITEM_RANGE2				= 9;							// �ֶ���
//const int	ARROW_FLY_ALT					= 60;							// ��֧���и߶�
const int	JUMP_ALT						= 200;							// ������߸߶�
const int	FLY_ALT							= 100;							// �ɵ���߸߶�
const int	PERCENT_DECDUR					= 5;							// װ������ʧ�ܺ��;õļ���
const int	CRIME_KEEP_SECS					= /*60*2*/20;							// 20070226����:10���� ����ά��ʱ��
const int   SELF_DEFENCE_SECS               = 20;                           // ������������ʱ��
const int	SYNCRIME_KEEP_SECS				= 30;							// ���ɷ���ά��ʱ��
const int	MAX_MENUTASKSIZE				= 20;							// �˵�ϵͳ��"�ش�TASK"���������

const int	PKVALUE_HALFKILL				= 2;
const int	PKVALUE_ONEKILL					= 1/*10*/;//[��;���� 2009.06.03]                           //20070226����:10 ÿɱ��һ������/������ң�����PKֵ   // add huang 2004.1.11
const int	PKVALUE_BLUE					= -120;
const int	PKVALUE_ORANGE					= PKVALUE_ONEKILL*6;
const int	PKVALUE_WHITE					= 0;	
const int	PKVALUE_BROWN					= PKVALUE_ONEKILL*16;			// 20070226���� ԭ60
const int	PKVALUE_REDNAME					= PKVALUE_ONEKILL*11;							// 20070226����:ͬPKVALUE_ONEKILL ����������PKֵ
const int	PKVALUE_BLACKNAME				= PKVALUE_ONEKILL*15;							// 20070226����:ԭ240 ����������PKֵ

const int	CHANCE_MAKEHOLE					= 300;

const int	SUPERMAN_KILLMONSTER_INCTIME		= 800;				// ��˫״̬ÿɱһ������ļ�ʱ(MS)
const int	SUPERMAN_KILLMONSTER_TIMELIMIT		= 120*1000;			// ��˫״̬�ӳ���ʱ������(MS)
const int	TORNADO_KILLMONSTER_INCTIME			= 1000;				// ����״̬ÿɱһ������ļ�ʱ(MS)
const int	TORNADO_KILLMONSTER_TIMELIMIT		= 120*1000;			// ����״̬�ӳ���ʱ������(MS)

const int	MASK_CHANGELOOK					= 10000000;						// ����ʱLOCKFACE�����룬face=face + new_face*10000000
const int	MAX_SIZEADD						= 2;							// ����SizeAdd������ħ������ķŴ�����(ע������ݲ�Ҫ̫��)

const int	CHGMAP_LOCK_SECS				= 7;							// �����ͼ��ı���ʱ��
const int	PICK_MORE_MONEY					= 1000;							// ��Ǯ��ʱ����ACTION��Ϣ
const int	SYNCHRO_SECS					= 5;							// ͬ����Ϣ5����ͬ��

const int	IDENT_FEE						= 100;							// �����ļ۸�
const int	UNIDENT_SALE_FEE				= 1;							// ��δ�����ļ۸�

const int	LOGOUTTIME						= 10;					//20070602����:�˳�����



const int	ADJUST_EXPFORMAT_LEVEL			= 70;			// 70�����ϵ�������洢��ʽΪ/10

// �¿�Ϊ100�� [�½���11/12/2007]
//[��;���� 2009.05.04]�ȼ�����,�ⶥ��Ϊ45��
//[2009.5.13--�Ŷ�]�ȼ�����,���ڷⶥ��Ϊ50��
//[��;���� 2009.06.09]50=>60
const int	USERMAXLEVEL					= 100;			//20070808����:��ߵȼ�80.

const int	SAVE_POINT_RATE					= 20000;		// ÿ���� [9/18/2007 %PENGFENG%]
const int	SAVE_POINT_MAXTIME				= 300;			//����

/////////////////////////////////////////////////////////////////////////
// ��ʦϵͳ���
const int	ADDSTUDENT_TUTOREXP				= 10;			// ����ͽ����Ҫ���ĵĵ�ʦ����
const int	DELSTUDENT_TUTOREXP				= 1000;			// ����ѧԱ�۵ĵ�ʦ����
const int	LEAVETUTOR_LEVEL				= 10;			// 10�����������ʦҪ�۾���
const int	_MIN_GIVEEXP_LEV				= 30;			// ѧԱΪ��ʦ�ṩ�����������͵ȼ�
const int	TUTOREXP_PERCENT				= 40;			// һ��ѧԱ��������ʦ�ľ������
const int	TUTOREXP2_PERCENT				= 10;			// ����ѧԱ��������ʦ�ľ������
const int	GETEXP_PER_TUTOREXP0			= 500;			// ������ʦ����1�㵼ʦ�����ܹ���õ�ѧԱ���׾���
const int	GETEXP_PER_TUTOREXP1			= 700;			// �м���ʦ����1�㵼ʦ�����ܹ���õ�ѧԱ���׾���
const int	GETEXP_PER_TUTOREXP2			= 1000;			// �߼���ʦ����1�㵼ʦ�����ܹ���õ�ѧԱ���׾���


//---jinggy---����---begin
const int	SYNREPUTE_SECS					= 6*60;		// ���ÿ����6���ӣ���10����� �����͹��׶ȡ�
const int	SYNREPUTE_VAL_PEREACH			= 10;		    
const int	SYNREPUTE_VAL_PROFFER			= 1;		// ���ÿ����6���ӣ���һ����Ź��׶ȡ�

#define		SYNMEM_TIMEONLINE_INTERVAL		15*60			//ÿ15���Ӹ���һ�ΰ��ɳ�Ա����ʱ��
#define		SYNMEM_TIMEONLINE_INCREPUTE		10 //�ۡ�	���ų�Աÿ��ÿ����6���ӣ�������������10��
//---jinggy---����---end
//---jinggy---����---begin
const int	EQUIPMENT_EXP_ADD_SECS			= 10*60;						// ÿ10��������һ��С��9������������
const int	EQUIPMENT_EXP_ADD_SECS_AFTER9	= 15*60;						// ÿ15��������һ��9������������
const int	WEAPON_UPEXP_MAXVALUE			= 8;
const int	ARMOR_UPEXP_MAXVALUE			= 10;
const int	NECKLACE_UPEXP_MAXVALUE			= 12;
const int	SHOE_UPEXP_MAXVALUE			= 14;
const int	BANGLE_UPEXP_MAXVALUE		= 16;
const int	MAX_LEVEL_WARLEVEL	= 9;
const int   DEC_WARGHOSTEXP_FORREPAIR = 3 ;
//---jinggy---����---end

const int	PROFFER_BEKILL					= 1;					// ÿ���ж԰������ɱ��һ�μ��ٵİ��ɹ��׶�
const int	SYNREPUTE_BEKILL				= 1;					// ÿ�α��ж԰������ɱ�����ٵİ�������

const int	NEWBIE_LEVEL					= 40;			// ���ֵĵȼ�
const int	_MIN_TUTOR_LEVEL				= 35;			// ��ʦ��͵ȼ�
const int	GRAUDUATE_LEVEL					= 50;			// ��ͳ�ʦ�ȼ�
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
//���ƣ����ֿ������ִ��ⶼ������һ����TYPE���֡�1�����ƣ�2�����ֿ���3�����ִ��� ��������ֻ��֪��������Ĵ���������Ҫ֪������
const DWORD	PRESENT_ONE				= 0x01;				//�͵�1�ε�װ��
const DWORD	PRESENT_TWO				= 0x02;				//�͵�2�ε�װ��
const DWORD	PRESENT_THREE			= 0x04;				//�͵�3�ε�װ��
const DWORD	PRESENT_FOUR			= 0x08;				//�͵�4�ε�װ��
const DWORD	PRESENT_FIVE			= 0x10;				//�͵�5�ε�װ��
const DWORD	PRESENT_SIX				= 0x20;				//�͵�6�ε�װ��
const DWORD	PRESENT_SEVEN			= 0x40;				//�͵�7�ε�װ��
const DWORD	PRESENT_EIGHT			= 0x80;				//�͵�8�ε�װ��
*///-pengfeng

enum Enum_ChangeMapGroupMode
	{
		ENUM_CNGMAPGROUP_MODE_REQ	=0,//ת����ͼ������
		ENUM_CNGMAPGROUP_MODE_PROC	=1,//ת����ͼ����Ӧ����
		ENUM_CNGMAPGROUP_MODE_ACK	=2,//ת����ͼ��ͬ��
		ENUM_CNGMAPGROUP_MODE_NACK	=3,//ת����ͼ��ܾ�
	};

////////////////////////////////////////////////////////////////
// sex define
enum {SEX_NONE = 0, SEX_MAN, SEX_WOMAN, SEX_ASEXUAL, SEX_ALL};

enum
{
	SEX_MALE,SEX_FEMALE
};
// requre sex define
const unsigned char SEX_REQ_MAN			=	0x01;// �еĿ���
const unsigned char SEX_REQ_WOMAN		=	0x02;// Ů�Ŀ���
const unsigned char SEX_REQ_EUNUCH		=	0x04;// ̫�����


//////////////////////////////////////////////////////////////////////////
//	20070703 ������ʹ��
const __int64	USER_NEGATIVE_EXP_MIN_VALUE	=	-100000000000;	//	��������Сֵ	//	[3/20/2008 ���]	������-1000��
const int	USER_NEGATIVE_EXP_MAX_PERCENT =	-200;			//	���������ٷֱ�
//////////////////////////////////////////////////////////////////////////

const int FISHTIRED_MAXVALUE = 500;                        //������ƣ�Ͷ�

const int  AWARDEX_TYPE_BEGIN    = 100; //�콱��չ��ʼ����
const int  AWARDEX_TYPE_END      = 200; //�콱��չ��������

////////////////////////////////////////////////////////////////
//const int	MASTER_USERLEV				= 120;
//const int	MAX_USERLEV					= 100;
const int	MAX_USERLEV					= 120;
//const int	MAX_EUDEMONLEV				= 130;

////////////////////////////////////////////////////////////////
enum 
{	
		RACE_NONE	= 0, // δ֪
		RACE_HUMAN	= 1, // ����
		RACE_SPRITE = 2, // ����
		RACE_DARKSPRITE = 3, // ������
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
	NOBILITY_RANK_NONE		= 0,			// �޾�λ
	NOBILITY_RANK_BARON		= 1,			// ��ʿ
	NOBILITY_RANK_VISCOUNT	= 2,			// �Ӿ�
	NOBILITY_RANK_EARL		= 3,			// ����
	NOBILITY_RANK_MARQUESS	= 4,			// ���
	NOBILITY_RANK_DUKE		= 5,			// ����
};


enum {
	GENADD_SORT_PHYATT		= 1,//�⹦
	GENADD_SORT_MAGATT		= 2,//�ڹ�
	GENADD_SORT_PHYDEF		= 3,//���
	GENADD_SORT_MAGDEF		= 4,//�ڷ�
	GENADD_SORT_HIT			= 5,//����
	GENADD_SORT_DODGE		= 6,//����
	GENADD_SORT_CRIT		= 7,//����
	GENADD_SORT_TOUGH		= 8,//����
	GENADD_SORT_STR			= 9,//����
	GENADD_SORT_INT			= 10,//����
	GENADD_SORT_STA			= 11,//����
	GENADD_SORT_SPI			= 12,//����
	GENADD_SORT_AGI			= 13,//��
	GENADD_SORT_ALL			= 14,
	GENADD_SORT_HP			= 15,//HP
	GENADD_SORT_MP			= 16,//MP

	GENADD_SORT_ATT2		= 20,//��
	GENADD_SORT_ATT1		= 21,//��
	GENADD_SORT_ATT3		= 22,//��
	GENADD_SORT_ATT4		= 23,//��
	GENADD_SORT_DEF2		= 24,
	GENADD_SORT_DEF1		= 25,
	GENADD_SORT_DEF3		= 26,
	GENADD_SORT_DEF4		= 27,

	GENADD_SORT_SPEED		= 28,//�ƶ��ٶ�
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
		INFO_COUNTTIMER,		//06.11.8�������
		INFO_AWARDINFOSEND,
		INFO_STATUS_MONSTER,//�������ϵ�buff
		INFO_BEFRIEND,
		INFO_TREASURE,
		INFO_TUTOR,//ʦ��
		INFO_STUDENT,//ͽ��
		INFO_TITLE,//�ƺ�
		INFO_PETEGG,//���ﵰ
		INFO_JINMEI,
		INFO_DOUBLE,//�����˫��ʱ��
		INFO_ONLINEGIFT,//��Ըף��
		INFO_KEY,
		INFO_AUTOFIGHT,
		INFO_PEIFANG,
		INFO_TASKDETAIL,
		INFO_EUDEMON,
		INFO_WARGAME,
		INFO_SPECILLINE,
//modify code by �ֵ�Ԩ begin 2011.5.30-
		INFO_VASLV,
//modify code by �ֵ�Ԩ end
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
		REMOTE_CALL_SYNPAYOFF,//20070414����:���ŷ����ʵ�����
		REMOTE_CALL_GOMATE,///˲�Ƶ���ż����
		REMOTE_CALL_REPLYGOMATE,///��Ӧ˲�Ƶ���ż����	
		REMOTE_CALL_KICKOUTPLAYER,//�ߵ����
		REMOTE_CALL_WARGAME,		//	ս���ʹ��
		REMOTE_CALL_SYNADDMENBER,
		REMOTE_CALL_SYNADDHAVE,
		REMOTE_CALL_SYNADDOK,
};

enum ADJUSTDAMAGE
{
	ADJUSTDAMAGE_PHYDEF,///���
};

enum ADJUSTUSERATTRIB
{
	ADJUSTUSERATTRIB_PHYDEF,///���
	ADJUSTUSERATTRIB_MAXMANA,///�����ֵ
	ADJUSTUSERATTRIB_MAXLIFE,///�������ֵ
	ADJUSTUSERATTRIB_MAGDEF,///ħ��
	ADJUSTUSERATTRIB_HITRANGE,///������
	ADJUSTUSERATTRIB_DOG,///����	
	ADJUSTUSERATTRIB_PHYACK,///�﹥
	ADJUSTUSERATTRIB_MAGACK,///ħ��
	ADJUSTUSERATTRIB_Crit,///����
	ADJUSTUSERATTRIB_Drug,///��ҩ����Ч��
	ADJUSTUSERATTRIB_STR,//����
	ADJUSTUSERATTRIB_INT,//����
	ADJUSTUSERATTRIB_STA,//����
	ADJUSTUSERATTRIB_SPI,//����
	ADJUSTUSERATTRIB_AGI,//����
	ADJUSTUSERATTRIB_TOUGH,//����

	ADJUSTUSERATTRIB_FIREATK,//��
	ADJUSTUSERATTRIB_FIREDEF,//��
	ADJUSTUSERATTRIB_ICEATK,//��
	ADJUSTUSERATTRIB_ICEDEF,//��
	ADJUSTUSERATTRIB_MYSTICATK,//��
	ADJUSTUSERATTRIB_MYSTICDEF,//��
	ADJUSTUSERATTRIB_NOXATK,//��
	ADJUSTUSERATTRIB_NOXDEF,//��
};

//enum BATTLEADDINFO //ս��ϵͳ������Ϣ
//{
namespace BATTLEADDINFO
{
const	DWORD INFO_INVALID			=	0X00000000;		//��Ч�ģ���Щ������ս��ϵͳ�ļ����
const	DWORD INFO_BEGIN			=	0X00000001;		//��ʼ,��ʹ�ô�ö��
	
const	DWORD INFO_DODGE			=	0X00000001;		//����
const	DWORD INFO_CRITICALHIT		=	0X00000002;		//����һ��
const	DWORD INFO_LOWDAMAGE		=	0X00000004;		//���� //�ĳ�����
const	DWORD INFO_HALTDAMAGE		=	0X00000008;		//���� //�ĳ���Ѫ
const	DWORD INFO_HIGHDAMAGE		=	0X00000010;		//����
const	DWORD INFO_BLOCK			=	0X00000020;		//��
const	DWORD INFO_IMMUNITY			=	0X00000040;		//����
const	DWORD INFO_RESIST			=	0X00000080;		//�ֿ�
const	DWORD INFO_ATK_INVALID		=	0X00000100;		//��Ч�Ĺ������������ƹ�����Ч��Ŀ�꣬��������������Ч
const	DWORD INFO_DODGE_WITHSHILD	=	0X00000200;		//���ܴ�����

const	DWORD INFO_END				=	0X80000000;		//����,��ʹ�ô�ö��
};

//};

//06.11.8�������--------------begin-------------------------------------------
struct CountTimerStruct{
	OBJID	idUser;				//���id
	int		nIndexInSet;		//��ԭ�������е�����
	clock_t tUpdateTime;		//����ʱ��(�ϴο�ʼ����ʱ��)
	int		nIntervalTime;		//���ʱ��
};

const _int64    _AWARD_VASPACK					=0x1;			///����VAS����

typedef struct tag_GoMate_Info 
{
	int idMap;
	int posX;
	int posY;
}GoMate_Info;///��ż�ƶ���Ϣ

//20061229����:��ʯ���ӽṹ-------------------------------------
struct oftenUseGemAddition{
	int         sort;
	LONG		value;//goto �ĳ��з�������,��ֹԽ��
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
const DWORD		_MAX_CRIT						= 10000;		//20070130����:��󱩻�.10000Ϊ100%
const float		_MAX_CRIT_F						= 100.0f;		//20070130����:��󱩻�.10000Ϊ100%
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
	int nType;	//	��Ŵ����С��207 sort = 2, type = 07
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
	bool		Init					();		// login �� change map_group��Ҫִ�еĲ���
	bool		InitSpeInfoOnly			();		//ֻ����ר����Ҫ����Ϣ
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
	static BOOL IsFluctuate( UINT itemTypeID );// �ж�װ������ʱ�Ƿ���Ը���
public:
	DWORD   GetPacklev(int nType = 0);
	void	SetPacklev( DWORD nLev )           { m_data.SetPackLev( nLev );}
	bool	SetPacklev( int nType,DWORD nLev);
	DWORD   GetBanklev(int nType = 0);
	bool	SetBanklev(int nType,DWORD nLev );
	void	UpdateMissionMsgByMonsterType(OBJID idType);
	void	UpdateMissionMsgByItemType(OBJID idType);
	int		sendMissionMsg();// �������������Ϣ
	void	SaveMissionInfo();
	//��Ըף�����
public:
    void InitOnlineGift(OBJID Gift1ID,OBJID Gift2ID,OBJID Gift3ID,OBJID Gift4ID,OBJID Gift5ID);
	void ProcessOnlineGift();
	bool GiveOnlineGift();
	bool ChkItem(int nMustGive);
	bool ChkZero(int nIn);
	bool IsGive(int nTask);
	bool IsDayContune();

///������ؽӿ�
public:
	bool	RequestEnterInstance(UINT nInstanceType,UINT nDomainMapID=0,UINT nInstanceScope=0,UINT nCellx =0,UINT nCelly=0);///�������ָ�����͵ĸ���
	BOOL	EnterInstance(OBJID idMapInstance,UINT nMapType,UINT nInstanceScope,UINT nDomainMapID,int x,int y);///����ָ�����͵ĸ���
	void	SendTeamMsg(int nChannel,const char* pMsg);
	BOOL	IsInTeam(){if(GetTeam()) return TRUE; else return FALSE;}
	void    AddTeamLeader(OBJID leaderId);
	DWORD	GetSynCon(){if(QuerySynAttr() && GetSynID()!=0) return QuerySynAttr()->GetProffer(); else return 0;}
	void	SetSynCon(int nData, bool bSend = true);
	int		GetMapDoc();
public:
	BOOL	ResetItemCD( UINT nDelayTimeMS, UINT TypeID, CTimeOutMS* pTimeOutMS  );//-------���---2007/05/30-----
	void	UpdateAllOnlineTime();//-����������ʱ��
	void	UpdateLastLogout();
	OBJID	GetOnlineTime();//-�õ���������ʱ��//��λ����
	BOOL	ReformPoint(int nType = 0,int data = 0);
	float	CalItemAddExpTeamFactor();
	BOOL	IsPickUpKidnap( int nItemType );
	BOOL	EquipMakeHole( OBJID equipID, OBJID stoneID);// װ���� [2010-07-20 goto]
	BOOL	EquipMakeChaosHole(OBJID idEquip, OBJID idStone);//������ף�������֮��
public:
	virtual		int			GetObjType		()			{ return CGameObj::GetObjType(); }
	virtual		void		SendShow		(IRole* pRole);

public:
	virtual bool IgnoreTargetDodge()	{ return m_bIgnoreTargetDodge;	}
	virtual bool IsMustCritHit()		{ return m_bMustCritHit;			}

	void	SetDetectionFlag( OBJID targetID )			{ m_bIgnoreTargetDodge = m_bMustCritHit = true;m_targetID	= targetID;	}
	void	ResetDetectionFlag()						{ m_bIgnoreTargetDodge = m_bMustCritHit = false;m_targetID	= NULL;		}
protected:
		bool	m_bIgnoreTargetDodge;	//���ԶԷ�����
		bool	m_bMustCritHit;			//��Ȼ����
public:
		OBJID	m_targetID;
//����Ŀ��״̬���
public:
	void setLockTarget( OBJID targetID) { m_IdLockTarget=targetID;}
	OBJID getLockTarget( ) { return m_IdLockTarget;}
private:
	OBJID	m_IdLockTarget;
	OBJID	m_idLastTaskEudemon;
public:
	void SetLastTaskEudemon(OBJID idEudemon) { m_idLastTaskEudemon = idEudemon; }
	OBJID GetLastTaskEudemon() { return m_idLastTaskEudemon; }
	bool IsRedProcAble(){return (m_data.GetPkProc() == 0);} //�Ƿ�Ϊ��������
	bool IsWhileProcAble(){return (m_data.GetPkProc() == 0 || m_data.GetPkProc() == 1) ;} //�Ƿ�Ϊ���������
	bool IsSynProcAble(){return (m_data.GetPkProc() == 3);}//�Ƿ�Ϊ���ɱ���
	void SetPkProc(int nStatus,BOOL bUpdate = false)  
	{ 
		m_data.SetPkProc(nStatus,bUpdate);
	} //����Pk����,ͬʱ���ú����Ͱ���
public: // IRole
	virtual OBJID	GetID			()				{ return m_data.m_Info.id; }
	virtual LPCTSTR	GetName			()				{ return m_data.m_Info.szName; }
	virtual LPCTSTR	GetMate			()				{ return "��"; }//����ϵͳδ��������ʱд��
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
	int		GetTreasureCost()	{ return m_pUserKey.GetTreasureCost(); }//��¼��ҿ����ӿ��˶���Ԫ����Ȼû�г�����֮��
	void	SetTreasureCost(int nCost, bool bUpdate=false)	{ m_pUserKey.SetTreasureCost(nCost, bUpdate); }//��¼��ҿ����ӿ��˶���Ԫ����Ȼû�г�����֮��

	int		GetVasTicketAmount()	{ return m_pUserKey.GetVasTicketAmount(); }
	void	SetVasTicketAmount(int nAmount, bool bUpdate=false)	{ m_pUserKey.SetVasTicketAmount(nAmount, bUpdate); }
	DWORD			GetWarSroce() {return m_dwWarSroce;}
	//һ������
	//�����������
	//���Ǽӳ�.
	DWORD			GetStr() {return m_dwUserStr;}//����
	DWORD			GetIntex() {return m_dwUserInt;}//����
	DWORD			GetSta() {return m_dwUserSta;}//����
	DWORD			GetSpi() {return m_dwUserSpi;}//����
	DWORD			GetAgi() {return m_dwUserAgi;}//��
	

	//������������Ľ���ҵ���
	DWORD			GetStrUserOnly(int type =0){ return m_data.GetStr(type);}//����
	DWORD			GetIntUserOnly(int type =0){ return m_data.GetIntex(type);}//����
	DWORD			GetStaUserOnly(int type =0){ return m_data.GetSta(type);}//����
	DWORD			GetSpiUserOnly(int type =0){ return m_data.GetSpi(type);}//����
	DWORD			GetAgiUserOnly(int type =0){ return m_data.GetAgi(type);}//��

	//2������
	//��Ϊ��ҵĻ�������
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
	//���Ǽӳ�.
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
	//����
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
	//goto �Գ�����Ч�ı�������,����ɼ��ܵ�
	LONG	GetPetAtkAdd()	{ return m_GiftPetAtk; }
	LONG	GetPetMagAtkAdd()	{ return m_GiftPetMagAtk; }
	LONG	GetPetCrihitAdd()	{ return m_GiftPetCrihit; }
	LONG	GetPetMaxLifeAdd()	{ return m_GiftPetMaxLife; }
	//��̬
	void	SetLife( DWORD nLife, bool bUpdate = false )	{ m_data.SetLife( nLife,bUpdate);	}
	void	SetMana( DWORD nMana, bool bUpdate = false )	{ m_data.SetMana( nMana,bUpdate);	}
	void	SetSp(DWORD nSp, bool bUpdate = false )	{ m_data.SetSp( nSp,bUpdate );	}
	virtual DWORD	GetLife()			{ return m_data.GetLife(); }
	virtual DWORD	GetMana()			{ return m_data.GetMana(); }
	virtual DWORD	GetSp()				{ return m_data.GetSp();}
	//����
	virtual DWORD 	AdjustDamage		(DWORD nDamage,int nAttribType);
	virtual int 	AdjustUserAttrib	(DWORD nAttrib,int nAttribType);
	//����
	virtual int		GetAttackRange		(int nTargetSizeAdd);
	virtual DWORD	GetMaxSp			();
	virtual DWORD	GetInterAtkRate		();
	virtual DWORD	GetIntervalAtkRate	();
	virtual	int		GetSizeAdd			();

	//	virtual void	AwardBattleExp		(int nExp, bool bGemEffect=true, bool bIncludeOwner=true);
	virtual DWORD	AwardBattleExp		(__int64 i64Exp, bool bGemEffect=true, bool bIncludeOwner=true, bool bTask=false);
	virtual void	AwardUserExp		(UINT nExp){AwardBattleExp(nExp);}///ר���ű�ϵͳ����֮�õĽӿ�.

	virtual int		GetDistance			(IMapThing* pTarget)	{ return __max(abs(GetPosX() - pTarget->GetPosX()), abs(GetPosY() - pTarget->GetPosY())); }
	virtual int		GetDistance			(int x, int y)			{ return __max(abs(GetPosX() - x), abs(GetPosY() - y)); }

//	virtual int 	GetExp				()						{ return m_data.GetExp(); }
	virtual __int64		GetExp				()						{ return m_data.GetExp(); }
//	virtual int     SetExp              ()                      {  m_data.SetExp(); }//  [�½��� 7/10/2007]
	virtual void	SetStatus			(int nStatus, bool bSynchro=true, int nPower=0);
	virtual void	ClsStatus			(int nStatus, bool bSynchro=true);
	virtual DWORD	GetStatus			()						{ return /*m_data.m_Info.dwStatus*/0; }
	virtual void	ClearAllStatus		();
	virtual void    ClearManyStatusWhenDie();//-------���---2007/05/16-----����ʱ���һЩ״̬


	virtual void	SetAllOnlineTime( DWORD nAllOnlineTime, BOOL bUpdate = false ){m_data.SetAllOnlineTime(nAllOnlineTime,bUpdate);}// ����������ʱ�� [9/12/2007 %PENGFENG%]
	virtual DWORD	GetAllOnlineTime ()				{ return m_data.m_Info.dwAllOnlineTime; }// �õ�������ʱ�� [9/12/2007 %PENGFENG%]

	virtual void	SetAllAddExpTime( DWORD nAllAddExpTime, BOOL bUpdate = false ){ m_data.SetAllAddExpTime(nAllAddExpTime,bUpdate);}// �����ۻ�����ֵ[9/12/2007 %PENGFENG%]
	virtual DWORD	GetAllAddExpTime ()				{ return m_data.m_Info.dwAllDoubleTime; }// �õ��ۻ�����ֵ [9/12/2007 %PENGFENG%]

	virtual DWORD	GetExitTime()	{ return m_data.m_Info.dwExitTime; }			// �õ���ʼ�����ۻ�����ʱ�� [9/12/2007 %PENGFENG%]
	virtual void	SetExitTime( DWORD exitTime,BOOL bUpdate = false ){m_data.SetExitTime(exitTime,bUpdate);}	// ���ÿ�ʼ�����ۻ�����ʱ�� [9/12/2007 %PENGFENG%]			//����ʹ�ã�ֱ���޸�DB

//	virtual BOOL	CheckStatus			(int nStatus);
	virtual I64 	GetEffect			();
	I64				GetSecondEffect		()	{ return /*m_data.m_Info.i64Effect2*/0;}//07.4.17�������


	virtual DWORD	GetLev				()						{ return m_data.GetLev();		}
	virtual void	IncLev				(int nLev,int nType = 0); //0�������� 1��ְҵ���� 2��ְҵ����
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
	virtual int		GetArmorTypeID		();//06.5.5�����޸�
	virtual int		GetWeaponRTypeID	()			{ if(m_pWeaponR) return m_pWeaponR->GetInt(ITEMDATA_TYPE); return ID_NONE; }
	virtual int     GetDressTypeID		()			{ if(m_pDress) return m_pDress->GetInt(ITEMDATA_TYPE); return ID_NONE; }
	virtual int		GetNecklaceTypeID	()			{ if(m_pNecklace) return m_pNecklace->GetInt(ITEMDATA_TYPE); return ID_NONE; }
	virtual int		GetShoesTypeID		()			{ if(m_pShoes) return m_pShoes->GetInt(ITEMDATA_TYPE); return ID_NONE; }
	virtual int		GetMountTypeID		()			{ if(m_pMount) return m_pMount->GetInt(ITEMDATA_TYPE); return ID_NONE; }// ���� [�½���7/30/2007]
	virtual int		GetWeaponSpecialView()			{ if(m_pWeaponR) return m_pWeaponR->GetWeaponSpecialView(); return 0; }//goto ������Ч

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
	//����
	inline BOOL	IsWhiteName();
	//����
	BOOL	IsGrayName();
	//����
	BOOL	IsRedName();

	int GetTaskStep(OBJID idTask);

	BOOL		GetStorageLimit();///�Ƿ�ͨ����ֵ���ѽ�������
	void		SetStorageLimit(int nStoragelimit);///������ֵ������ʶ
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
	virtual bool	SendInformation(const char* strInfo);///softworms��չ-2007-12-7
	virtual bool	SendSysInfo(const char* strInfo);///softworms��չ-2007-12-7
	virtual DWORD	GetSynID			()				{ if (QuerySynAttr()) return QuerySynAttr()->GetSynID(); return 0;}
 	virtual DWORD	GetMasterSynID		()				{ if(QuerySynAttr()->GetSynID()==ID_NONE) return ID_NONE; return this->GetSyndicate()->GetMasterSynID(); }
public:
	void ClearCheckData()											{ m_nLastWalkTime[0] = 400;m_nLastWalkTime[1] = 400;m_nThisWriteTime = 0;for(int i = 0 ; i < CHECKDATA_LAST ; i ++ ) m_dwCheckData[i] = 0;}
	void ClearWalkData()							{ m_dwCheckData[CHECKDATA_WALK] = 0;m_nLastWalkTime[0] = 400;m_nLastWalkTime[1] = 400;m_nThisWriteTime = 0;}	//��ֹ�е�ͼ��غʹ�غ��ٴ��
	bool CheckMsgData(CHECKDATA nData,DWORD dwMs)					
	{ 
		if(nData == CHECKDATA_WALK) //��·��3��ƽ��ֵ
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
	void		EquipAddStar( OBJID idEquip,short nGemCount,bool bUseLuckyCharm=false );//[2010-07-20 goto]װ������
	//���뱦ʯ
	bool		EmbedGem				( OBJID idEquip, OBJID idStone , OBJID idGem );
	bool		EmbedGemInChaos			( OBJID idEquip, OBJID idStone , OBJID idGem );
	//ȡ����ʯ
	bool		TakeOutGem				(OBJID idEquip,OBJID stoneID,int nPos );
	//[2010-08-04 goto]��ʯ�ϳ�
	bool		GemLevUp				(OBJID idCharm, set<OBJID> pDataSet,int nSize);
	bool		EquipSoulMark			(OBJID idEquip);//goto ��ӡ
	OBJID		GemCutwork				(OBJID idGem, OBJID idCharm);//��ʯ����
//modify code by �ֵ�Ԩ begin 2011.7.13
	bool		EquipArtific			(OBJID idEquip);//����
	bool		EquipMove				(OBJID idEquip,OBJID idCharm,int &firstaddlv,int &secondaddlv);//ǿ��ת��
//modify code by �ֵ�Ԩ end
//////////////////////////////////////////////////////////////////////////
	//�������
	bool	EquipSoulAdd(OBJID idWeapon, OBJID nCount,bool bUseBook=false);
	bool    EquipSoulExchange(OBJID idWeapon, OBJID idExWeapon);
	int		GetEquipSoulDeadLine(int nType,int nLev);//1������ħ����,2�Ǳ�����������,3�������嶨������
	int		GetEquipSoulLevByPoint(int nPoints,int nLev);//�����ʣ��ħ����
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
	CTimeOut	m_tLogout;//20070601����:�˳���ʱ
	DWORD		m_tFootPrint;
	POINT		m_posFootPrint;
	auto_long	m_bRun;
	CTimeOutMS	m_tStand;
	auto_long	m_bStand;
	OBJID		m_nConMoney;
	OBJID		m_nConBuild;

//���������--------------begin----------------------------
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
	CTimeOut	m_tWaitReborn;		//ĳ�˶���ʹ�ø�����,30���ڿ���ѡ���ǻ��. 
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

	ULONG		GetIPAddress		()						{ return m_nIPAddress; }//������IP��ַ

public: // const -------------------------------------

public: // leave word ---------------------------------------
	bool		IsMoreLeaveWord()							{ return m_bHaveLeaveWord; }
	void		SetLeaveWord(bool nFlag)					{ m_bHaveLeaveWord = nFlag; }
	bool		AddLeaveWord(LPCTSTR szRecv, LPCTSTR szWords, LPCTSTR szColor="0");//[2010-06-10 goto]�����ı���ɫ�ֶ�
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
	virtual bool    CombineItemInStorage(OBJID idNpc,CItem *pItem,bool bUpdate = true,int nPosition = ITEMPOSITION_STORAGE); //���ڲֿ���Ʒ�ĺϲ�
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

	bool CanUseItem(CItem* pItem);///�ж�����Ƿ���ʹ��ĳ������
protected:
	CTimeOut	m_tMount;

public:	// sprite -- add by zlong 2003-11-28
protected:
	CTimeOut			m_tSprite;

public: // cheat --------------------------------------------------------
	enum { _TYPE_MAGIC=0, _TYPE_WS=1, _TYPE_FY=2, _TYPE_USE_LIFE=3, _TYPE_USE_MANA=4, }; // ��˫// ����	// �ú�ҩ// ����ҩ
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

		APPLY_STUDENTAPPLY,		// ������ͽ��
		APPLY_TEACHERAPPLY,		// �����ʦ
		APPLY_GRADUATE,			// �����ʦ
// 		APPLY_TEAM_NO,			//�ܾ����
		APPLY_SYNWAR,
		APPLY_PVP,
		APPLY_CANCELDISSTUDENT,		// ����ȡ������ͽ��
		APPLY_CANCELLEVTEACHER,		// ����ȡ���뿪ʦ��
		APPLY_LEAVETEACHER,			// ������ʦͽ��ϵ
	};

	void	SetApply		(int nApply, OBJID idTarget)	{ m_nApply = nApply; m_idApplyTarget = idTarget; }
	OBJID	FetchApply		(int nApply)					{ if(nApply == m_nApply) { m_nApply = APPLY_NONE; return m_idApplyTarget; } return ID_NONE; }
	//bool	FetchApply		(int nApply, OBJID idTarget)	{ if(nApply == m_nApply && idTarget == m_idApplyTarget) { m_nApply = APPLY_NONE; return true; } return false; }
	bool	FetchApply		(int nApply, OBJID idTarget);//�ŵ�cpp�﷽�����
	//////////////////////////////////////////////////////////////////////////
	//  [2007-9-7] ��� ��������
	void	SetApplyName	(int nApply, const char* pszName);
	bool	FetchApplyName	(int nApply, const char* pszName);
	//////////////////////////////////////////////////////////////////////////
protected:
	int		m_nApply;
	OBJID	m_idApplyTarget;

	int		m_nApplyByName;
	NAMESTR	m_szApplyName;
public:	//-------- ��֪�������� -----------------------
//	void		InitWalkStep		()				{ m_nWalkStep = 0; }

	void		SetForce			(DWORD dwForce);
	

	void		SetDexterity		(DWORD dwDex);

	void		SetSpeed			(DWORD dwSpeed);
	DWORD		GetSpeed			()				{ return (m_data.GetSpeed()>150)?150:m_data.GetSpeed();} //ǿ��ͬ��ΪС��=150����ֹ��ұ�tickOUt
	// ����״̬�����ٶ�
	DWORD		AdjustSpeed			(int nSpeed);

	void		SetHealth			(DWORD dwHealth);
	

	void		SetLoveliness		(DWORD dwSoul);
	

	void		SetSoul				(DWORD dwSoul);
	
	DWORD		GetSoulSum			();

	void		SetFace				(DWORD dwFace);
	DWORD		GetFace				()				{ return m_data.GetLookFace(); }
	void		SetLook				(DWORD dwLook);

	int			GetRace				()				{ return 0; }
	//[2010-06-04 goto] �û����飬�ں�������ʾ
	void		SetFeel				(char* szFeel);
	char*		GetFeel				()				{ return m_data.GetFeel(); }

	void		SetItemLockPwd		(char* szPwd)   { m_data.SetItemLockPwd(szPwd); }
	char*		GetItemLockPwd		()			 { return m_data.GetItemLockPwd(); }

	void		ProcAutoHealRecover	();		//�Զ���Ѫ,��..	

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

public:	//---------- USERר�е�ս������ ----------------------
	void		IncAddPoint			(int nAddPoint);
	void		SetExp				(__int64 i64Exp);
	void		CalcFightRate		()						{ m_nFightRate = /*GetInterAtkRate()*/1000; }
	void		ProcessAutoAttack	();
	int			AdjustExp			(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg=false);
	int			AdjustEudemonExp	(IRole* pTarget,CMonster* pWarPet, int nRawExp, bool bNewbieBonusMsg=false);
	bool		CheckCrime			(IRole* pRole);		
	bool		IsImmunity			(IRole* pRole);	
	
	bool		checkMonsterTag(int monsterID);

public:	//--------- ������� --------------------------------
	BOOL		AbandonTeamLead();///����ӳ��˳�,���ƽ��ӳ�����һ��-softworms-08-01-16
	BOOL		CreateTeam			();
	void		RebuildTeam();///����ʱ�ع�����
	BOOL		CreateDefaultTeam	();///��ǿ�Ĵ�����ӷ�ʽ
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
	void		SendPosToTeamMem(OBJID idMap, int nPosX, int nPosY);//goto ֪ͨ�����Լ����ڵĵ�ͼ
	void		GetPosFromTeamMem();//goto ��ȡ���ѵ�λ��
	//20070202����:С�Ӹ���.���Լ���CTeam����.�����ǳ�Ա
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

public: //--------- �����Ϊ --------------------------
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
	void        SetPk               (int nPk);       //  [�½��� 7/10/2007]
	int			IncPk				()				{ return AddPk(PKVALUE_ONEKILL); }
	int			GetPkMode			()				{ return m_data.m_Info.ucPkMode; }
	void		SetPkMode			(int nMode)		{ m_data.m_Info.ucPkMode = nMode; }
	bool		IsPkEnable			(IRole* pRole);
	bool		IsGhost				()				{ return m_bGhost; }
	//2007828����:�µĲ���,������������------
	//void		Reborn				(bool bChgPos=true);
	void		Reborn				(bool bChgPos=true,bool bRebornFlag=false,int nRecoverPercent=0);
	//---------------------------------------
	void		TransformGhost		();
	int			CalcExpLostOfDeath	(IRole* pKiller);

protected:
	bool		m_bGhost;
public: //�������� ��������
	int			AddNationCon		(int nAdd);
	int			SetNationCon		(int nAdd);
	int			GetNationCon		()				{ return m_data.GetNationCon(); }

	int			AddChivalrous		(int nAdd);
	int			SetChivalrous		(int nAdd);
	int			GetChivalrous		()				{ return m_data.GetGoodEvil(); }

public:	// ����������ϵͳ
	int			AddPk				(int nAdd);
	void		ProcessPkTimer		(void);	// auto synchro
	void		ProcessPkTimer_InPrison(void);
	bool		m_bInstanceReborn;///��������Ƿ������
public: // item ------------------------------------------
	//06.11.7�������------------------------------------
	//06.11.8�����޸�
	//��Ʒ��CD���
	typedef map< int,CTimeOutMS* > MAP_ITEMDELAY;
	typedef map< int,CTimeOutMS* >::iterator MAP_ITEMDELAY_Iter;
	MAP_ITEMDELAY m_setItemDelay;

	//-----------------------------------------------------
	///////////////////////////////////////////////////////////
	enum { IDENT_OK = true, NO_COMBINE=false, };
	//20070626����:�ĳɾ�̬
	static void    EquipChgAttribute( ItemInfoStruct* pinfo,int nBossType = 0 ,int nAddType = 0);		//  [4/2/2007] ���������ı�װ��������
	CItem*	AwardItem		(int nItemType,bool bSynchro=true, bool bIdentOK=false, bool bAutoCombine=true,bool bChgattribute=false,int nAmmount = 1 , BOOL bSaveCreater =true,BOOL bInsert=true,BOOL bMarry = false, DWORD nKidnap = NULL,int nBossType = 0,DWORD dwPerPrice = 0);//bChgattribute ���������ƴ�����Ʒ��ʱ���Ƿ�仯���� int nAmmount = 0 ��Ϊ�˿���������Ʒʱһ�����ɶ��  add  by halei 070402		// ͬʱ�������ݿ�
	BOOL	AwardItemByCreate(int nItemType, bool bSynchro, bool bAutoCombine,bool bChgattribute,int nAmmount,int nKindnap,int addType,bool bIgnoreType=false,LPCTSTR szCreater=NULL);
	///softworms-2007-11-08-���ϵͳ������Ʒ�����,�����������ϵͳ�ʼ�
	BOOL    AwardItemBySys(int nItemType, bool bSynchro, bool bIdentOK=false, bool bAutoCombine=true,bool bChgattribute=false,int nAmmount=1, int nSynID = 0,int nJuan = 0,int nKindnap = 0,int nBossType = 0,DWORD dwPerPrice = 0,OBJID idAddItem = ID_NONE);
	BOOL	AwardItemCount	(OBJID idItemType,UINT nItemAmount=1, bool bChgattribute = true,int nJuan = 0,int nKindnap = 0,int nBossType = 0,int nPerPrice = 0);///���û�ָ����������Ʒ.
	CItem*	AwardItem		(ItemInfoStruct* pInfo, 
		bool bSynchro, 
		bool bAutoCombine=true,
		bool bChgattribute=false,//bChgattribute ���������ƴ�����Ʒ��ʱ���Ƿ�仯����   add  by halei 070402		// ͬʱ�������ݿ�
		BOOL bSaveCreater=true,//20070707����:��¼������
		BOOL bInsert=true,
		BOOL bRoleCreate = false,
		BOOL bCreateEudemon = true,
		int	 nBossType = 0);//[2009.4.17--�Ŷ�]��¼���ͼ����Ʒ������
	bool	EraseItem		(OBJID idItem, bool bSynchro, bool bDelEudemon=true,bool bIsPlayerKill = false, bool bComBin = false);	// ͬʱ�������ݿ�
	bool	EraseEquip		(int nPosition, bool bSynchro);							// ͬʱ�������ݿ�
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

	//���棺������Ľӿڵ�ʱ�����������Ҫ���������ܣ��������Ը�
	CItemPtr&	GetEquipItemRef	(int nPosition)						{ CItemPtr* ppEquip = GetEquipItemPtr(nPosition); ASSERT(ppEquip); return *ppEquip; }

	CItem*	GetEquipItemByPos	(int nPosition);
	int		GetEudemonAmount() { return m_pUserPet->GetShowAmount();}
	bool	AddItem			(CItem* pItem, bool bSynchro, bool bUpdate = true);
	bool	DelItem			(OBJID idItem, bool bSynchro);
	bool	GetItemInfoByItemid(OBJID idItem);//����������[2008/12/3]
	//20061209����.ɾ�������Ʒ.
	//���ǵ�ɾ��������������Ʒ����������,������Ҫɾ�����item
	//��ɾ��֮ǰ,�ȵ���GetItemByTypeMulti�õ�user�д�����Ʒ����,�������Ϸ�ʱ����false
	BOOL	DelItemByTypeMulti (OBJID idItem,int nAmount,bool& bHasDelKidnapItem, bool isID = false, bool bIgnoreLock=false);
	//20061209����.ɾ�������Ʒ.����
	//���ǵ�Ч������,����.
	//����Ҫ��ɾ��ǰ����GetItemByTypeMulti().���ⲿ���ȵ���,�ж�ͨ��֮���������ô˺���.
	BOOL	DelItemByTypeMulti (OBJID *itemIDArray,int size,int amountNeedToDel);
	bool	DelItemPt		(OBJID idItem);							// ֻɾ��ָ��
//	CUserPetData*	PopEudemon (OBJID idEudemon, bool bSynchro, bool bUpdate);
	CItem*	PopItem			(OBJID idItem, bool bSynchro, bool bUpdate = false);			// �������OWNER
	bool	TryItem			(OBJID idItem, int nPosition);
	bool	ChkUseItem		(CItem* pItem, IRole* pTarget);
	bool	UseItem			(OBJID idItem, int nPosition, bool bSynchro);
	bool	UseItemTo		(OBJID idTarget, OBJID idItem);			// �Ա���ʹ����Ʒ -- zlong 2004.09.23
	bool	UnEquipItem		(int nPosition, bool bSynchro ,int nWant2Position = -1,int nWant2PackageIndex = -1);
	bool	SplitItem		(OBJID idItem, int nNum);
	bool	OrderPackage	(int package);
	bool	CombineItem		(OBJID idItem, OBJID idOther);
	CItem*	FindCombineItem	(OBJID idType);
	CItem*	CombineNewItem	(CItem* pItem, ItemInfoStruct* pInfo, bool bSynchro);
	//20061023����.����һ������nDonotCareDistance,����ʰȡʱ��
	//��0ʱ,ȡ��������PICKMAPITEMDIST_LIMIT����.
	bool	PickMapItem		(OBJID id,OBJID idItem,int nDonotCareDistance=0);
	bool	DropItem		( OBJID idItem, bool bIsMustDrop = false);
	bool	DropItemBySys	( OBJID idType,int x,int y ); //ϵͳǿ�ƶ���Ʒ������...	
	bool	DelTaskItemByType(int type);
	bool	DropMoney		(DWORD dwMoney, int x, int y);
	bool	RepairItem		( OBJID NpcID,OBJID itemID, bool bSynchro );
	bool	RepairAll		( OBJID NpcID,bool bSynchro);
	bool	RepairAllSpecial(bool bSynchro,bool bWithHammer = false,OBJID hammerID = 0);//�������[08/11/18]ȫ������
	//bool	RepairAllSpecialWithHammer(OBJID hammerID,bool bSynchro);//�������[08/11/20]ʹ�����޴���������װ��
	bool	RepairOneSpecial(OBJID nEquipPos,bool bSynchro);//�������[08/11/18]������������װ��
	void	ProcessFixHammer(OBJID itemID);
	bool	RepairSpecial   ( OBJID itemID, bool bSynchro );	//  [4/16/2007] ������������
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
	BOOL	SpendNewRemedy( CItem* pItem, int nPosition=ITEMPOSITION_BACKPACK1, bool bSynchro=true,CUserPetData* pData = NULL,int nMaxLife = 0);	//---���-----2007/03/26----��ԺȲ����Ѫƿд��ʹ�ú���-----------
	bool	SpendNewRemedy();//����
	bool	SpendOneItem(OBJID idItem);//����һ������,���ű�ʹ��
	BOOL	SpendNewEdemonRemedy( CItem* pItem, int nPosition=ITEMPOSITION_BACKPACK1, bool bSynchro=true );

	bool	RebornUseScroll(BOOL bUseScroll = TRUE/*�Ƿ�ʹ�ø����*/,bool bForce = false);// ���� [6/15/2007]��ʹ�ø������//�³���[08.9.17]����һ������
	bool    RebornInstance(bool bInstance = true);
	bool	EquipItem		(CItem* pItem, int nPosition, bool bSynchro);
	bool	CheckPackageFree(USHORT packageType = 47,USHORT amount = 1);
protected:	
	CItem*	UnEquipOnly		(int nPosition,int nWant2Position = -1,int nWant2PackageIndex = -1);///[2009.4.7--�Ŷ�]���nWant2Position��nWant2PackageIndex����ʵ��ж��װ����ָ��������λ��
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
	void	BuyItem			( OBJID idNpc, OBJID idType,int nAmount, int nBuyType ,int nPrice = 0);//06.9.15�����޸�
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
	bool	m_bUpdateWeight;		// false: m_nAllWeightֵ��Ч����Ҫ���¼���
	int		m_nAllWeight;			// ��ǰ�ܸ�������CACHE
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
	///���רΪ�ű�ʹ�õ�action����ӿ�,softworms-2007-12-7
	bool	ProcessAction	(OBJID idAction);
	bool    ProcessActionUser(OBJID idActionType,LPCTSTR szParam,int data);	
	bool	ProcessActionIncludeItem(OBJID idAction, CItem* pItem);//goto Ϊ���ýű�������Ʒ����Ӵ˺���

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


	_int64	GetTaskMask		();//20061121����:���dwTaskMask

	_int64	GetDayTaskMask	();//20070916:���dwDayTaskMask
	void	AddDayTaskMask		(int idx);///2007-09-17-softworms������������չ
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

	// ========= Ӷ��������� ============
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
	bool					m_bDebugAction;				// �Ƿ��GM��ʾÿ��ACTION��ִ��ϸ��
	int						m_nTaskIterator;			// ����������ϵͳ�е���
	
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

	//---����ϵͳ---begin
	CTaskDetail	*	m_pTaskDetail;
	//---����ϵͳ---end
	
public: // ---------------����---------------------------------------------------------------------------------
	bool		SendAllSynInfo()		{ return m_pSyn->SendInfoToClient(); }
	CSynAttr*	QuerySynAttr()			{ if(m_pSyn) return m_pSyn; else return NULL; }		//? ����ʱע�����Ƿ��а���
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
	int			GetBeFriendAmount()					{ return m_setBeFriend.size(); }//�������[2008/12/17]������Ϊ���ѵ���ҵ�����
	int			GetFriendGroupAmount(int groupid);//[2010-06-07 goto]��ȡĳ�����ѷ���ĺ�������
	int			GetFriendAllAmount();
	CFriend*	GetBeFriendByIndex(int idx)			{ CHECKF(idx>=0 && idx<m_setBeFriend.size()); return m_setBeFriend[idx];}//�������[2008/12/17]
	CFriend*	GetFriendByIndex(int idx)			{ CHECKF(idx>=0 && idx<m_setFriend.size()); return m_setFriend[idx]; }
	CFriend*	GetFriend		(OBJID idFriend);
	CFriend*	GetBeFriend		(OBJID idFriend);
	CFriend*	GetBeFriendByName(char* pszName);
	CFriend*	GetFriendByName(char* pszName);	//  [2007-9-10] ���
	bool		AddFriend		(OBJID idFriend, LPCTSTR szFriendName,USHORT nFriendType = 1 ,LPCTSTR szFriendMate="��");		// δ����ظ����
	bool		AddBeFriend		(OBJID id,OBJID idUserOwnmeAsFriend,USHORT nFriendType ,LPCTSTR pszName );
	bool		SetFriendShip(OBJID idFriend,int nFriendShip);

	bool		DelFriend		(OBJID idFriend);			// ͬʱɾ����¼
	bool		DelBeFriend		(OBJID idUserOwnmeAsFriend);//�������[2008/12/17]	

	bool		MoveFriend		(CFriend* pFriend,int nFriendType);//�������[2008/12/15]
	void		BroadcastFriendsMsg	(CNetMsg* pMsg);
	void		DelFriendLeaveWord(LPCTSTR szName, DWORD dwId);//[2010-06-09 goto]ɾ���ѿ������û�����
	void		DelTempFriend();
	OBJID		AddLeaveFriend(LPCTSTR szFriendName, unsigned int uFriendType, CMsgFriend* pFriendMsg);
	bool		HasTempFriend	()	{ return m_bHasTempFriend; }
	void		SetTempFriend	(bool bFriend)	{ m_bHasTempFriend = bFriend; }
protected:
	bool		CreateAllFriend	(IDatabase* pDb);
	bool		CreateAllBeFriend(IDatabase* pDb);//�������[2008/12/16]��ȡ��Щ�����Լ�Ϊ���ѵ���ҵ���Ϣ
	void		DeleteAllFriend	();
	void		DeleteAllBeFriend();//�������[2008/12/16]
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
//	bool		DelTutor			()					{ CHECKF(m_pUserTutor); m_pUserTutor->DelTutor(); }//ͽ�ܵ������ʦͽ��ϵ
//	bool		AddStudent			(CUser* pStudent)	{ CHECKF(m_pUserTutor); m_pUserTutor->AddStudent(pStudent); }
	//bool        RequestDelTutor();                      //����ɾ��ʦ��
	bool        ProcessTutorDelReq(int nApplyType);     //����ʦ��ɾ������
	//int         GetDensityWithTutor();                  //�õ���ʦ�������ܶ�
	bool        ProcessTutorRecover();                  //����ʦ���ָ�
	void		QueryTutorList( USHORT nPage, int nPageSize );//��ѯ�Ǽǵ�ʦ���б� (ҳ�룬ÿҳ��ʾ����)
	bool		IsInTutorList()	{ return (m_data.GetTutorMask() & TUTOR_INLIST) != 0; }//����Ƿ�Ǽ���ʦ���б�
	void		JoinToTutorList();//��Ҽ���ʦ���б�
	void		DelFromTutorList();//��ʦ���б���ɾ��
	void		DelTutorListMask()	{ m_data.SetTutorMask( m_data.GetTutorMask() & (~TUTOR_INLIST) ); }//��ʦ���б���ɾ�����
	void		AddTutorListMask()	{ m_data.SetTutorMask( m_data.GetTutorMask() | TUTOR_INLIST ); }

// 	void        RequestAddDensity(int nDensity);        //�����������ܶ�
//	void        AddImpartByStudent(int nImpart)	{ CHECK(m_pUserTutor); m_pUserTutor->AddImpartByStudent(nImpart); }//ͨ��ѧ�����Ӵ��ڶ�
//	void        AddImpartByTutor(OBJID idStudent, int nImpart)	{ CHECK(m_pUserTutor); m_pUserTutor->AddImpartByTutor(idStudent, nImpart); }//ͨ��ѧ�����Ӵ��ڶ� //ͨ��ʦ�����Ӵ��ڶ�
//	int			GetImpartByStudent();//ͽ�ܲ�ѯ���ڶ�
//	int			GetImpartByTutor(OBJID idStudent);//ʦ����ѯ���ڶ�

	//int         GetTutorTeamAddExp(int nExp);

	bool		TakeExpFromStudent	(int nExpTake);		// ȡ��ѧԱ���׵ľ���
	bool		IsFirstTutor		()					{ return (m_data.GetTutorMask() & TUTOR_ISFIRST)==0; }//�Ƿ��ǵ�һ�ΰ�ʦ
	void		SetNotFirstTutor	()					{ m_data.SetTutorMask(m_data.GetTutorMask() | TUTOR_ISFIRST); }//��Ϊ���ǵ�һ�ΰ�ʦ
	bool		HasGraduated		()					{ return (m_data.GetTutorMask() & TUTOR_HASGRADUARED)!=0; }//�Ƿ����ʦ
	void		SetGraduated		()					{ m_data.SetTutorMask(m_data.GetTutorMask() | TUTOR_HASGRADUARED); }//��Ϊ����ʦ
	bool		IsTutorTeam			();//�����Ա�Ƿ���ʦͽ��ϵ

	// student
	int			GetStudentAmount	(bool bIncludeGraduated)		{ CHECKF(m_pUserTutor); return m_pUserTutor->GetStudentAmount(bIncludeGraduated); }
//	CTutor*		GetStudentByIndex	(int idx)			{ CHECKF(idx>=0 && idx<m_setStudent.size()); return m_setStudent[idx]; }
	CTutorData*	GetStudent			(OBJID idStudent)	{ CHECKF(m_pUserTutor); return m_pUserTutor->FindStudentByID(idStudent); }
	CTutorData*	GetTutor			()					{ CHECKF(m_pUserTutor); return m_pUserTutor->GetTutor(); }
	//bool		DelStudent			(OBJID idStudent); //ʦͽЭ������ϵ
	bool		TutorDivorce		()					{ CHECKF(m_pUserTutor); return m_pUserTutor->TutorDivorce(); }//ʦͽЭ������ϵ
	bool		DelStudentByTutor	(OBJID idStudent,bool bDelDBRec=true)	{ CHECKF(m_pUserTutor); return m_pUserTutor->DelStudentByTutor(idStudent,bDelDBRec); }
	bool		DelTutorByStudent	(bool bDelDBRec=true)	{ CHECKF(m_pUserTutor); return m_pUserTutor->DelTutorByStudent(bDelDBRec); }
	//bool        RequestDelStudent   (OBJID idStudent);       //����ɾ��ѧ��
	bool        ProcessStudentDelReq(OBJID idStudent, int nApplyType); //����ѧ��ɾ������
	bool        ProcessStudentRecover(OBJID idStudent);      //����ѧ���ָ�
	bool		IsStudentFull		();			

	//void		SendTutorInfo		(USHORT nPage);//goto ����ҳ�����
	//void		NotifyTutorStatus	(int nStatus);
	bool		Graduate(OBJID idStudent)			{ CHECKF(m_pUserTutor); return m_pUserTutor->Graduate(idStudent); }//ʦ����ͽ�ܳ�ʦ
	//bool		DelTutorRef			();
	//bool		DelStudentRef		(OBJID idStudent);

	//bool        CancelLeaveTeach();                         //ȡ���뿪ʦ��
	//bool        CancelDelStudent(OBJID idStudent);          //ȡ��ɾ��ѧ��
	//bool        RecoverTutor();                             //�ָ�ʦ��
	//bool        RecoverStudent(OBJID idStudent);            //�ָ�ѧ��
	bool        RevQesCelLevTeach(OBJID idStudent);         //�յ�ѯ��ȡ���뿪ʦ��
	bool        RevQesCelDelStudent();                      //�յ�ѯ��ȡ��ɾ��ѧ��

// 	DWORD		GetTutorMask(OBJID idStudent);//��ȡʦ����������
// 	void		SetTutorMask(OBJID idStudent, DWORD dwMask);//����ʦ����������

//	bool		AddStudentRef(CUser* pUser)	{ CHECKF(m_pUserTutor); return m_pUserTutor->AddStudentRef(pUser); }
//	bool		AddTutorRef(CUser* pUser)	{ CHECKF(m_pUserTutor); return m_pUserTutor->AddTutorRef(pUser); }
	int			GetMaxScore()	{ return m_data.GetMaxScore(); }
	DWORD		GetImpart()		{ return m_data.GetTutorLevel(); }
	int			GetMorality()	{ return m_data.GetMorality(); }//ʦ��
	void		SetMorality(int nData)	{ m_data.SetMorality(nData); }
	void		AddMorAlity(int nData)	{ CHECK(nData>0); SetMorality(GetMorality()+nData); }
	bool		SpendMorAlity(int nData);
	void		AddImpart(int nData)	{ CHECK(nData>0); m_data.SetTutorLevel(GetImpart()+nData); }

	int			GetTutorApplyAmount()		{ CHECKF(m_pUserTutor); return m_pUserTutor->GetTutorApplyAmount(); }

	OBJID		ApplyAddTutor(ST_MEMBERINFO* pInfo, TUTORSTATE nState, ST_MEMBERINFO* pInfoExt)	{ CHECKF(m_pUserTutor); return m_pUserTutor->ApplyAddTutor(pInfo,nState,pInfoExt); }
	DWORD		CalcTutorGiftMask(int nLev);
	bool		AddTutorPreach(OBJID idItem, int nAmount);//ʦͽ�������� �Ѽ���
	int			GetTeamStuInLev(int nMaxLev, int nMinLev, int nLowerThan=0);//��ȡ������ָ���ȼ���Χ��ͽ������
	bool		HasSameIPStuInTeam();//�������Ƿ�����ͬIP��ͽ��
	bool		CheckItemCreater(OBJID idItem);
	bool		HasTutorGift(OBJID idStu, int nLev)	{ CHECKF(m_pUserTutor); return m_pUserTutor->CanGetGift(idStu, nLev); }//�ж�ʦ������
	bool		SetTutorGiftMask(OBJID idStu, int nLev)	{ CHECKF(m_pUserTutor); return m_pUserTutor->AddGiftMask(idStu, nLev); }//����ʦ����������
	int			GetTeamStuAvgLev();	//������ͽ�ܵ�ƽ���ȼ�
	int			GetStuLevAddedInTeam();//�����е�ͽ����ʦ�����������ļ���
	int			GetInstanceTime(int nInstanceType);
	int			GetInstanceFinishTime(int nInstanceType);
protected:
	//bool		AddStudentRef		(const TutorInfoStruct* pInfo);

	//bool		CreateAllTutor	(IDatabase* pDb);
	//void		DeleteAllTutor	();
	void        TutorLoginHint();             //ʦͽ��½��ʾ
	
public: // enemy ------------------------------------
	void		TutorLogoutHint();			  //ʦͽ����
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
	void			GainVas(DWORD dwMoney);//�����˺ŷ�������Ǯ
	void			GainVasCPST(DWORD dwMoney);
	void			SpendVas(DWORD dwMoney);//�۳�Ԫ����ͬ���˺ŷ�����

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
//--------------07.4.10����ע��-----------------------------------------
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
	CTimeOut		m_tAttrackMonster;	// �����������ʱ�� ����:)

public: // login, return false: logout_user
	bool			IsDefaultLoginSite();
	bool			EnterMap		();
	bool			LeaveMap		();
	int				SendItemSet		();
	bool			SendGoodFriend	();

public: 
	void        DelCurNpc();

public: // mapgroup ---------------------------------
	//20070329����:����һ��Ĭ�ϲ���.Ĭ��ֵ0,��ʾ����.1��ʾ��Ӧ����
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
	//06.11.8�������-------begin-----------------------------------------------------------
	//������Ʒ��CD

	bool			AppendCountTimer( CountTimerStruct* pInfo );//��ʱ����Ϣ
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

	class XUserSquareDeal : IUserSquareDeal			// SquareDeal �ӿ� 
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

protected: // ��¼������
	enum { STEP_NONE=0, STEP_LOGIN, STEP_ENTERMAP };
	int				m_nStep;
	
protected: // ��ҹ���Ķ���
	typedef vector<CFriend*>	FRIEND_SET;
	FRIEND_SET	m_setFriend;//���浱ǰ��ҵ����к�����Ϣ
	FRIEND_SET  m_setBeFriend;//�������[2008/12/16]������˵�ǰ���Ϊ���ѵ����������Ϣ
	CUserTutor*		m_pUserTutor;

	IUserPackage*	m_pPackage;	// ����
//	IUserPackage*	m_pMagicStoneBox;	// ħ����
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

	//---jinggy---2004-11-19---װ������ֵ---begin
	CTimeOut    m_arrTimeOutForEquipExp[ITEMPOSITION_EQUIPEND-ITEMPOSITION_EQUIPBEGIN];
	int			m_arrEquipIncExp[ITEMPOSITION_EQUIPEND-ITEMPOSITION_EQUIPBEGIN];//�豸ս�꾭��ֵ������
	CTimeOut    m_tEquipWarGhostExp; //���װ����ս����ʱ��
	bool		m_bIsEnableWarGhostExp;

//	int			m_Worktype;
//	int			m_WorkSub;
	CTimeOut	m_tTimeOnline; //���ɳ�Ա����ʱ��
	//---jinggy---2004-11-19---װ������ֵ---end

	CUserData	m_data;	
public:
//	DWORD		GetExpTutor(){return m_dwExpToTutor;}
//	void		SetExpTutor(DWORD exp){m_dwExpToTutor = exp;}
//	void		pushStudentVector(TutorInfoStruct * tempSetStudent);
//	void		ClearStudentVector(){m_setStudent.clear();}
//-----------------------------07.1.12������ӣ�����-----------begin----------
	bool		IsHide()	{ if( IsGM() )	return m_bHide;return false;}
	bool		ChangeHideStatus();
protected: // ���״̬
	bool		m_bHide;//�Ƿ�������
	int			m_nLevExp;
	int			m_nSynDressArmorType;
//	CGameMap*	m_pMap;

public:	
	void ClsBuff(int nStatus);
//	void AddBuff(int nStatus,int nTime,int nCount,int nParam,int nTimes=0,int nParam2=0,int nParam3=0);
	void	AddBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 );//07.4.26�������
	void	UpdateBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 );//07.4.26�������
	
	void	QueryTopList( USHORT usType, USHORT nPage );				//��ѯ���а�//���2007/02/10��
	
	void UseWeaponWithGem( IRole* pTarget );				//�����ֱ�ʯǶ����������
	void IncSP();
	int AdjustAttribDamage( int nPower,CMonster* pTarget); //�������Թ���
	int  GetPhyle();

	bool	IsPublicMap( int nMapID );			//�ǲ��ǹ�����ͼ
	bool	IsOwnMap( int nMapID );				//�ǲ��Ǳ����ͼ

//-----------------------06.10.28�����������޸�-------------------begin-----------------------------------------------
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
	//20040416����:֧�ֿ�ѡ��ͬ��,����
	//void AddMoney(DWORD dwMoney);
	void AddMoney(DWORD dwMoney,BOOL bSynchro=true,BOOL bUpdate=false);

	bool IsSynLeader();
	void ProcessMsgAction(int nAction, int nData, OBJID id);
	void ResourceWarVictory();
	DWORD		m_dwLastCoolShow;
//�������ҽ���
// 	DWORD	GetLastChangeDescTime()				{ return m_data.GetLastChangeDescTime();	}
// 	void	SetLastChangeDescTime( DWORD time )	{ m_data.SetLastChangeDescTime( time );		}
	char*	GetDescription()					{ return m_data.GetDescription();			}
	void	SetDescription( char* desc )		{ m_data.SetDescription( desc );			}
	void	ChangeMyDesc( char* desc );			//�������ҽ���,�������������..
	void    SendTragetDesc(CUser *pTraget);		//�õ�ĳ�˵����ҽ���
//����GMָ��
	static long	GetTopUserCount();//-------���---2007/05/17-----��Ϊ��̬������ֱ�������ֿռ���� �����������
	
	static long	GetCurUserCount();//-------���---2007/05/17-----��Ϊ��̬������ֱ�������ֿռ���� Ŀǰ��������

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
//	CTimeOut			m_tPK_InPrison;	//�����м���PKֵ���ٶ�
	CTimeOut			m_GotoPrison;
	CTimeOut			m_tSynWork;		//���ɴ�
	CTimeOutMS			m_tSlowHealLife;
	CTimeOutMS			m_tSlowHealMana;
	CTimeOut			m_tAutoHealRecover;					// ÿ�����Զ��ظ�����,ħ��.

	CTimeOut			m_tEquipmentDurability;				// װ���;ö����

	int					m_nFightRate;						// �����ļ��������

	CTimeOut			m_tHpMpAdd;							// ������������Զ��ظ��ٶ�
	CTimeOut			m_EverydayOL;
	CTimeOut			m_tUpdateDatabase;

protected: // Ӧ�ÿ�����
//	int			m_nWalkStep;
//	CTimeOut	m_tLock;
	CAutoLink<CUser>	m_link;

protected: // ������
	SOCKET_ID	m_idSocket;
//	PROCESS_ID	m_idProcess;
	ULONG		m_nIPAddress;
protected:
	//---jinggy---2004-11-17---����װ������ֵ---�Ѿ��������ϵ�װ����map---begin
	map<OBJID,CTimeOut*> m_mapEquipmentsUsed ;
	typedef pair <OBJID,CTimeOut*> PairID;	
	//---jinggy---2004-11-17---����װ������ֵ---�Ѿ��������ϵ�װ����map---end
public:
	//20061010����.������ѯby TypeID.��ָ���������idItem[]
	//�����ò�������ʵ�ʵ�size.�ɺ�������amount
	//����-1ʱ��ʾidItem[]��ʵ��size����maxSize.��������������maxSize֮�ڵ�����
	int	GetItemByTypeMulti(OBJID idType,int &maxSize,OBJID *idItem,bool isKidnap = true);
	int GetEudemonAmountByType(OBJID idType);
	int EquipCreateNew(OBJID idPeifang, OBJID idAdditem);
	DWORD	m_dwWarSroce;

	//equip
	//goto ȫ���ĳ��з�������,��ֹ����Խ��
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

	//������ⲿ��
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

	
	//goto ȫ���ĳ��з�������,��ֹ����Խ��
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
	//goto �Գ�����Ч�ı�������,����ɼ��ܵ�
	LONG	m_GiftPetAtk;//�⹦
	LONG	m_GiftPetMagAtk;//�ڹ�
	LONG	m_GiftPetCrihit;//����
	LONG	m_GiftPetMaxLife;//Ѫ����

	//buff debuff�п����Ǽ������Եģ����Ա������з�������
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

	LONG	m_BuffAtt1;//��
	LONG	m_BuffAtt2;//��
	LONG	m_BuffAtt3;//��
	LONG	m_BuffAtt4;//��
	LONG	m_BuffDef1;//��
	LONG	m_BuffDef2;//��
	LONG	m_BuffDef3;//��
	LONG	m_BuffDef4;//��

	//�����������ֵ
	//goto ȫ���ĳ��з�������,��ֹ����Խ��
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
	OFTENUSE_GEMADDITION m_mOftenUseGemAddition;		//���ñ�ʯ��������

	//���︽������
	LONG	m_eudemonSoulphyatt;//�⹥
	LONG	m_eudemonSoulmagatt;//�ڹ�
	LONG	m_eudemonSoul1;//��
	LONG	m_eudemonSoul2;//��
	LONG	m_eudemonSoul3;//��
	LONG	m_eudemonSoul4;//��
	LONG	m_eudemonSoulAddHp;//��Ѫ
	LONG	m_eudemonSoulAddMp;//����
	LONG	m_eudemonSoulAddDodge;//����
	LONG	m_eudemonSoulAddHitRate;//����

	clock_t m_tLastAddHp;
	clock_t m_tLastAddMp;
public:
//-------------------------------------------------------------------------------------------------------
//	CStrRes m_objStrRes;     // ��������ϵͳ��ʾ [�½���8/20/2007]  
//	const	char* GetStr(OBJID idRes){return m_objStrRes.GetStr(idRes);} 
//--------------------------------------------------------------------------------------------------
//	BOOL AddSpirit(int nSpirit);
//	BOOL SpendSpiritChip(CItem* pItem, int nPosition = ITEMPOSITION_BACKPACK1, bool bSynchro = true );
//	BOOL ImportSpiritStoneToChip(int* pMaterial, int nSize, char* szError);
//	int CheckExp(__int64& nAddExp);			//	20070703 ��� ���� ��龭��ֵ����
	__int64 CheckExp(__int64& nAddExp);	//  [11/14/2007 ���]	ȫ����64λ	//	20070703 ��� ���� ��龭��ֵ����
	BOOL DoesUserNeedDeathPunishment(IRole* pRole = NULL);			//	// 20070702 ��� ���� �ж�����Ƿ�Ҫ�����ͷ�
	void UpdateEquipBasicProperty();			//����װ����������.

	//void InitOftenUseGemAddition();				//CUser ����ʱ����
	void ResetOftenUseGemAddition();			//reset m_mOftenUseGemAddition.ÿ��update֮ʱ����
	//void ClearOftenUseGemAddition();			//CUser	����ʱ����
	void UpdateOftenUseGemAddition();			//���³��ñ�ʯ��������
	
	void UpdateCalculatedProperty(UPDATECALTYPE nType = UPDATECALTYPE_ALL);			//���¼���֮��Ļ�������.���л�������ϸ�����.

	bool UpdateEudemonSoulProperty();//���︽�������
	//-----------------------------------------------------------------
													


	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//20070115����:��������
	BOOL TaskGiveUp(DWORD taskID,int step = 0);
	///softworms-���и����Ľű�
	bool RunScript(const char* txtScript=NULL);
	void InitScript();///��ʼ���ű�����,Ϊ�������нű���׼��
	void InitItemScript(CItem* pItem);///��ʼ�����߽ű�����,Ϊ�������нű���׼��
	//20070226����:������װ���;���ʧһ���ٷֱ�.����ʵ����ʧ�;�װ���ĸ���
	int	PackageDurationLose(int durationLosePercent);

	//20070518����:
	BOOL IsClosing(){return m_data.m_Info.bClosing;}
	void SetClosing(){m_data.m_Info.bClosing = true;}

	//20070602����:�޸ĵǳ�����---------------------
	//�Ƿ����ڵǳ�
	BOOL IsLogingout(){return m_tLogout.IsActive();}
	//�Ƿ�ǳ���ʱ
	BOOL IsLogoutTimeOut(){return m_tLogout.TimeOver();}
	//��ʼ�ǳ�
	void BeginLogout(){m_tLogout.Startup(LOGOUTTIME);}
	//��ϵǳ�
	void BreakLogout(){m_tLogout.Clear();}
	//----------------------------------------------

	//20070614����:��������״̬ͬ��
	void SynchroLockAtkBegin();
	void SynchroLockAtkEnd();

	//	20070712	���	
	CItemPtr GetUserAllItemByType(OBJID idType);	//	������ID����ȫ��װ��,�ֿ�,����,ħ����,����
	CItemPtr GetStorageItemByType(OBJID idType);	//	���Ҳֿ�
	CItemPtr GetEquipItemByType(OBJID idType);		//	����װ�������ϵ�
	//	20070712	���	
	//	�ж������û�д�������Ʒ,�����,�����ļ�����Ҵ���Ʒֻ����һ��,������TRUE,���򷵻�FALSE
	static	BOOL	 IsUserHaveAnExclusiveItem(OBJID idType, const char* pszItemName, CUser* pUser);

	//20070716����:���Լ��űߵ�����Ʒ.
	//��Ʒ������,����ϵͳ������
	//mapItem����ʱ,�κ��˶�����ʰȡ
	BOOL DropItemCreatedBySys(DWORD itemType,//itemtype or money
						  BOOL bIsMoney=false//true:money.false:item						  
						  );

	BOOL SynchroBCMaxLife();//2007918����:ͬ���㲥���Ѫ��
	BOOL SynchroBCMaxMana();//2007918����:ͬ���㲥���ħ��
	BOOL SynchroBCMaxSP();// ͬ���㲥������� [�½���11/15/2007]

	DWORD GetStengLevel()const {return m_LastStrengLevel;}
protected:
    bool m_NeedSynchroStrengLve;
	int m_LastStrengLevel;//--�ϴ�ǿ���ȼ���ֻ�����ϴβ�ͬʱ
public:

	MYHEAP_DECLARATION(s_heap)
public:
	void CheckItemLifeTime(CItemPtr pItem, BOOL bUpdateToClient, BOOL bSaveToDatabase);	//  [2007-9-14] ���
	void ActivateItem(CItemPtr pItem);
	void AddRemoteTeamMember(ST_TEAM_INVITE* pTeamInvite, BOOL bIsTargetTeam);
	void AwardHonor(int nHonor, BOOL bUpdate = TRUE, int nSynchro = TRUE);	//  [2007-8-30] ���	������

	Singnal_v1 soltKillPlayer;///��ұ�ɱ�¼�
	Singnal_v1 soltPlayerEnterMap;///��ҽ����ͼ�¼�
	Singnal_v1 soltPlayerExitMap;///��ҽ����ͼ�¼�
	Singnal_v1 soltPlayerTeamCreate;///��Ҷ�����Ϣ�ı��¼�
	
	void ResetDayTaskMask();///��������������
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
	CTimeOut	m_tItemLifeTime;	//  [2007-9-19] ���	�������ڼ����
	CTimeOut	m_tCardMsgTime;
	//	CTimeOut	m_tItemLifeTimeUpdate;	//  [2007-9-19] ���	�������ڸ��¼��
	
public:
	void CheckItemLifeTime(BOOL bUpdateToClient = FALSE);	//  [2007-9-17] ���	�����Ʒ��������
public:
	void ChanceDropItemAndEquip(int changeDrop);
//	BOOL WeaponSoulOutEx(CItem* pItem, int nIndex, BOOL bSendMail);
	void RebornInPos(OBJID nMapID, LONG nX, LONG nY, bool bRebornFlag=false, int nRecoverPercent=0);
	BOOL	m_bFirstCheckItemLifeTime;	//  [2007-10-10] ���	��һ�����߼��
	BOOL SynchroBoothItem(CItem* pItem,int nItemInfoAction,int nBoothIndex,int nMoney);
	BOOL SynchroItem(CItem* pItem,
		int nItemInfoAction,
		OBJID idUser=ID_NONE,//����CMsgItemInfo
		int nWpslAction=NULL);//20071214����:����ֱ��ʹ��CMsgItemInfo��ͬ��.��װһ��.
	BOOL SendDetailItem(CItem* pItem);
public:
	void	UpdateGiftBaseProperty();
	void	UpdateBuffBaseProperty();
protected:
	BOOL	SynchroUserProperty(UPDATECALTYPE nType);	//  [12/18/2007 ���]	ͬ����ɫ��������

public:
	USHORT GetActivityAward(USHORT usType);
	void   SetActivityAward(USHORT usType, USHORT usState, bool bSend=false);
	void   SetMobChargeAward(USHORT usType, USHORT usState, bool bSend=false);//�ֻ���ֵ����ѯ/����

protected:
	DWORD m_doubletime;//���ڼ�¼��Ҷ����˫��ʱ�䣬ֻ�ڳ�ʼ��ʱ�������ݿ�һ�飬����Ƶ���������ݿ�
	bool IsInMapInstance();///softworms�ж�����Ƿ��ڸ�����

	char m_szCryOut[_MAX_WORDSSIZE];
	DWORD	m_dwVas;//��¼��ֵ������
	DWORD	m_dwVasJuan;
	int     m_hpState;
	int     m_mpState;
	int     m_KillMonsterNum;          //��ǰɱ������
	int     m_killMonsterTargetNum;       //ɱ�ֵ�Ŀ������(��ȡ���ݿ�)
	int     m_killMonsterTimeInterval; //ɱ�ֵ�ʱ����(��ȡ���ݿ�)

	long	m_MoneyIncr;			   //goto ��������,��Ϊ��
	long	m_ExpIncr;				   //goto ��������,��Ϊ��

	//goto �����
	USHORT	m_nActivityAward;
	USHORT  m_ChargePack1;
	USHORT  m_ChargePack2;
	USHORT  m_ChargePack3;
	CUserTitle* m_pTitle;//�ƺ�
	CTreasure* m_pTreasure;//goto ��Ʊ,������
	COnlineGift* m_pOnlineGift;//����ף��
public:
	void SetUseTitle(int nType, int nLev, bool bUpdate = false);//[��;���� 2008/10/13]���õ�ǰʹ�õĳƺ�
	int  GetUseTitle()	{ return m_data.GetUseTitle(); }//���ص�ֵΪ�ƺ�ID*100+�ƺŵȼ�
	//void SetHoldTitle(UINT nPower, BOOL bUpdate = true);//[��;���� 2008/10/13]���õ�ǰӵ�еĳƺ�
	void AddHoldTitle(UINT nType, int nLev=1);
	void DelHoldTitle(UINT nType);
	CUserTitleData* FindTitleByType(UINT nType);
	void LevUpTitle(UINT nType);
	int  GetTitleLev(UINT nType);
	void SetTitleLevel(UINT nType, int nLev);
	//int CmpHoldTitle(UINT nPower);
	//_int64 GetHoldTitle() const {return m_data.GetHoldTitle();}//[��;���� 2008/10/13]��ȡ��ǰӵ�еĳƺ�
//	UINT GetUseTitle() const {return m_data.GetUseTitle();}//[��;���� 2008/10/13]��ȡ��ǰʹ�õĳƺ�
	CUserTitle* QueryTitle()	{ CHECKF(m_pTitle); return m_pTitle; }
	void SendAllTitle();

	//goto ��Ʊ,���������
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
	void DetachStatusFromEudemon(OBJID idEudemon);//���ﱻ�ٻػ�����ʱȥ����������˼ӵ�״̬
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
// 	BOOL SendMail(CItemPtr pItem);//[��;���� 2008/12/24]���չ��Ķ������ʼ����Լ�.
//	BOOL SendKillerMail(CItemPtr pItem);
	DWORD GetVas() {return m_dwVas;}//[��;���� 2008/12/16]�����ֵ������
	void SetVas(DWORD dwVas) {m_dwVas = dwVas;}//[��;���� 2008/12/16]������ֵ������

	DWORD GetVasJuan() {return m_dwVasJuan;}
	void SetVasJuan(DWORD dwVas) {m_dwVasJuan = dwVas;}

// 	void ChangeVasToAcount(int dwVas);
// 	SAVE_ITEM   m_SaveItem;
// 	CUserSaveItem m_UserSaveItem;
// 	void GetSaveItem(bool bNpc = false);
// 	void GetUserSaveItem(bool bSend = false);//���Դӱ����ұ��Լ�ɱ����ҵ�item
// 	bool ProcessSaveItem(OBJID itemId,OBJID itemTypeId,bool IsMoney=true,bool IsWSA = false);
// 	bool SendItemByMail(OBJID nItemType, bool bSynchro, bool bIdentOK=false, bool bAutoCombine=true,bool bChgattribute=false,int nAmmount=1, int nSynID = 0);
// 	bool CheckSaveItemLifeTime();
// 	bool GetSaveInfo();
// 	bool FindItem(OBJID itemId);      //Ѱ����Ʒ
// 	bool GiveUpSaveItem(OBJID itemId); //�����������
// 	bool GetWillSaveItem();    //���վ��Ʒ��Ϣ
public: //��Ұ�����Ϣ
	void ClearBooth();
	bool IsLuxuryBooth();
	void SendNotice(char *pszTalk);
	int  GetDlgState() { return m_pUserKey.GetDlgStates(); }
	int  GetFeelSwitch() { return m_pUserKey.GetFeelSwitch(); }
	void SetFeelSwitch(int nSwitch)	{ m_pUserKey.SetInt(SYSKEY_FeelSwitch, nSwitch); }//�˴����������ݿ⣬ת��ʱ����
	void SetItemLock() { m_pUserKey.SetInt(SYSKEY_SYSTEMSETTING, m_pUserKey.GetUserSysSet() | USERCONFIG_ITEMLOCK); }
	void SetItemUnLock()	{ m_pUserKey.SetInt(SYSKEY_SYSTEMSETTING, m_pUserKey.GetUserSysSet() & ~USERCONFIG_ITEMLOCK); }
	bool IsAgreeTeamApply()		{ return m_pUserKey.IsAgreeTeamApply(); }
	bool IsAgreeTradeApply()	{ return m_pUserKey.IsAgreeTradeApply(); }
	bool IsAgreePKApply()		{ return m_pUserKey.IsAgreePKApply(); }
	bool IsAgreeFriendApply()	{ return m_pUserKey.IsAgreeFriendApply(); }
	bool IsItemLockedOpen()				{ return m_pUserKey.IsLocked(); }
	CMonster *m_pMonster; //[��;���� 2009.04.03]����ɱ���Ĺ���
	OBJID m_idItem; //[��;���� 2009.05.22]��������Ʒ
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
	void IgotAnOrangeWeapen(int ItemTypeId, CItemPtr pItem,char* pMonsterName=NULL);									//���棺�Ҽ���XXXX��Ʒ �����ɱ�Ұɣ�
	void WeapensoulBroadCast(int nMsgtype,CItemPtr pItem,ST_WEAPONSOULDATA WPSLData,int nData);
	void JinmeiBroadCast(int nMsgtype, int nJinmeiIndex, int nData);
	CMonster* m_pWarPet;
	void SaveEudemonStatus();
	void RestoreEudemonStatus();

	BOOL CheckJoinSynDays(int nday);
private:
	OBJID idSelectItemType;
	bool bMateTeam;///[2009.2.12--�Ŷ�]�������Ƿ�����ż
	UINT m_nCityWarDomain;
	UINT m_nCityWarInstanceType;
	int  m_nCitySynWarType;
//	bool m_IsMarryTeam;///[2009.2.13--�Ŷ�]�Ƿ���й�IsAccountTeamMember()�ж�
	SYSTEMTIME m_createTime;
	OBJID m_nBabiID;
	bool bClear;
	//������������
	UINT m_nInstanceRebornX;
	UINT m_nInstanceRebornY;
	USHORT m_walkInt;

	CUserPet* m_pUserPet;
	vector<StatusInfoStruct> m_setEudemonStatus;

	CUserPeifang* m_pUserPeifang;
	bool	m_IsOnMount;

// By Minsky,2010-05-28����������������������ı�־
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

	//[2010-08-27 goto]��������������������һλ��ʾ����,1��ʾ��ƿ��2��ʾСƿ,��С��ʾ����������
	DWORD GetHpAdd	()	{ return m_data.GetHpAdd(); }
	DWORD GetMpAdd	()	{ return m_data.GetMpAdd(); }
	DWORD GetPetAdd	()	{ return m_data.GetPetAdd(); }
	void  SetHpAdd	(DWORD dwHpAdd, bool bUpdate=false)	{ m_data.SetHpAdd(dwHpAdd, bUpdate); }
	void  SetMpAdd	(DWORD dwMpAdd, bool bUpdate=false)	{ m_data.SetMpAdd(dwMpAdd, bUpdate); }
	void  SetPetAdd	(DWORD dwPetAdd, bool bUpdate=false) { m_data.SetPetAdd(dwPetAdd, bUpdate); }

	bool FreezeDoubleExp(UINT nBufID);//����˫������
	bool RestoreDoubleExp(UINT nBufID);//�ָ������˫������
	bool CheckFreezeDoubleExp(UINT nBufID);
	bool WriteStateToDB(IStatus* pStatus);
	bool DelStateFromDB(int nStatus);
	bool InitStorage(IDatabase* pDb,bool bLogin = true);

	//���ֳ�����ͼ����
	OBJID GetBornMap();


	int GetVit() { return m_data.GetVit(); }//����	
	void AddVit(int nData);
	void SetVit(int nData);
	int GetMaxVit(){ return m_data.GetMaxVit(); }

	int GetEne(){ return m_data.GetEne(); }//����
	void AddEne(int nData);
	void SetEne(int nData);
	int GetMaxEne(){ return m_data.GetMaxEne(); }	
//eudemon
private:
	CTimeOut  m_awardcheck;
	DWORD m_lastEudemonLifespanCheck;
	DWORD m_lastSoulEudemonLifeSpan;
	bool m_bSee;
	DWORD m_lastMountTime;//�ϴ�ʹ�����＼�ܵ�ʱ�䣬���ڿ������＼��CD
	bool  m_bHasTempFriend;//�Ƿ�����ʱ���ѣ����û�У�����ʱ�Ͳ���ִ��ɾ��
	int	  m_GoldPoint;//goto �˺ŷ������ĵ�ȯ����,��Ϊ�û�����˻����е�ȯ��ֻ�üӸ�����������
	int	  m_nTutorQuestion;//ʦͽ�������
	int	  m_nTutorAnswer;//ʦͽ�����
	CUserWarGame* m_pUserWarGame;
	CPetEgg* m_pPetEgg;
//���������
public:
	int  RDropEquip(int DropNum);		//�����ָ��������װ��
	bool isFatigue() {return ((GetFatigue() != 1) && (m_pUserKey.GetFatigue() != 1));}
	bool isFiftyP(); 
	bool isZeroP();	 
	void ProcessFatigue();
	void CheckEudeLifespan();
	bool CanBeSee() { return m_bSee;}   
	int  GetIdentity() {return m_data.m_Info.Identity;} 
	void SetIdentity(int IdNum){m_data.SetIdentity(IdNum);}
	
public://���ڼӺ��Ѱ�ȫ��֤
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
	bool GetVotingGift(bool bignorAll = false);//�������ڲ���
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
RANDLINE=0,//���
LINEONE=1,//1��
LINETWO=2,//2��
LINETHREE=3,//3��
LINEFOR=4,//4��
LINEFIVE=5,//5��
LINESIX=6,//6��
SPECILLINE=20,//ר��
	*/	protected:
bool m_bChangeLine;
bool m_bLoaded;

public:
	bool	ChangeLineMap(OBJID MapID,int nPosx,int nPosy,int nLine);//ת����·�͵�ͼ
	
	OBJID	GetSpecilMapID(){return m_pSpecilLineInfo->GetInt(SPECILLINEINFO_MAPID);}
	int		GetSpecilPosX(){return m_pSpecilLineInfo->GetInt(SPECILLINEINFO_POSX);}
	int		GetSpecilPosY(){return m_pSpecilLineInfo->GetInt(SPECILLINEINFO_POSY);}
	void	SetSpecilPos	(OBJID idMap, DWORD dwX, DWORD dwY, SPECILLINESTATE nState ,BOOL bUpdate = false);
	void	SetSpecilSteate(SPECILLINESTATE nState ,bool bUpdate = false);
	int		GetSpecilState(){return m_pSpecilLineInfo->GetInt(SPECILLINEINFO_STATE);} 
////////////////////////////�������//////////////////////////////////////////////--jay
	bool LeaveSynMap();
	bool IsMySynMap();
	bool EnterSynMap(int nPosx,int nPosy);//������ɵ�ͼ--jay
	void SetSynTaskGift(OBJID nGift1,OBJID nGift2);//���뽱������--jay
	int GetGiftMask(bool bGift);//�жϽ�������--jay
	int GetSynSci(int nSub);//��ü��ܵȼ�
	int GetNowWeekDay();//��õ�ǰ�����ڼ�
	int GetHistorySynCon();//�����ʷ�ܰﹱ
	int GetHistorySynMoney();//�����ʷ��Ǯ��
	int GetHistorySynGrow();//�����ʷ�ܽ���
	void SetHistorySynMoney(int nMoney);//��ʷ��Ǯ��
	void SetHistorySynGrow(int nGrow);//��ʷ�ܽ���
	OBJID  GetSynMapID();
	int GetMySynRank();//�������ְλ
	int GetSynMoney();//��ð����ʽ�
	int GetSynGrow();//��ð��ɽ���
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
	bool CheckIsSameSyn(int nType);//0�ж�ȫ��,1�жϸ���
	void SaveLog(int nType,OBJID nValue,bool isGet,int nGetWay);//���Ÿ��ű�,�����־
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

	double	GetPassedTimeFromCreate(LPCTSTR szTime);//��ȡ��Ҵ�����ɫ��ʱ�䵽ָ��ʱ��ķ�����

	_int64  GetASales() {return m_data.m_Info.i64BuyASales;} //������ؼ���Ʒ
	void	SetASales(_int64 nData, bool bUpdate=false)	{m_data.SetASales(nData, bUpdate);}
	void	SendASalesItem();//ͬ���ؼ���Ʒ
	void	SendLimitItem();//ͬ��������Ʒ

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

	bool WarGameSignup(OBJID idWarGame);//����������
	bool EnterWarGame(OBJID idWarGame);//���뾺����
	bool EnterWarGameFromNpc(OBJID idWarGame);//��npc�������ű�����
	bool LeaveWarGame();

	CPetEgg* QueryPetegg()	{ CHECKF(m_pPetEgg); return m_pPetEgg; }
	bool FlushPetegg(OBJID idData, OBJID idItem, int nGrow, int nNextFlush);//�ṩ���ű��ģ��ɽű��������ʺ��´�ˢ����Ҫ�ȼ���ʱ��(��)
	void SendPeteggData(OBJID idItem);//�ű����ã���ѯ���ﵰ��ѡ����
//modify code by �ֵ�Ԩ begin 2011.5.30
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
//modify code by �ֵ�Ԩ end

private:
	CCompensationManager* m_pCompensationManager;
public:
	void initCompensationManager();
	void SendCompentationInfoMsg();
	void SendCompentationInfo();
	bool PickCompentation(OBJID id,bool& bOther);
	///������֤����� 
public:
	//״̬
	int  GetVerificationState();
	void SetVerificationState(int nState,bool bUpdate=false);
	//����
	int  GetVerificationTimes();
	void SetVerificationTimes(int nState,bool bUpdate=false);
	void AddVerificationTimes();
	//buff�����Ƿ���Ҫ�ж�
	void SetVerificationNeedChk(bool bChk){m_Verification_needChk=bChk;}
	bool GetVerificationNeedChk(){return m_Verification_needChk;}
	//�Ƿ�ͨ����֤
	void SetVerificationPass(bool bPass){m_Verification_pass=bPass;}
	bool GetVerificationPass(){return m_Verification_pass;}

	//ִ���������
	void VerificationGivepupTask();
	//����
	bool SendVerification(){return m_pVerification->SendVerification();}
	bool ReSendVerification(){return m_pVerification->ReSendVerification();}
	//�߳����
	void KickOutUser(){m_pVerification->KickOutUser();}
	//��֤
	bool Verification(string szCode){return m_pVerification->Verification(szCode);}
	//״̬��ѯ
	bool CheckVerificationState();
	//�Ƿ�����
	bool IsVerificationCoding(){return m_pVerification->IsCoding();}
	void SetVerificationCoding(bool bCoding){m_pVerification->SetCoding(bCoding);}
private:
	CVerification *m_pVerification;///������֤
	bool  m_Verification_pass;
	bool  m_Verification_needChk;
	//////////////////////////////////////////////////////////////////////////
//modify code by �ֵ�Ԩ begin 2011.6.22
public:
	void SetNewValue(int newvalue);
	int GetNewValue(void);
	void SetNewValueTrageid(OBJID newtrageid);
	OBJID GetNewValueTrageid(void);
private:
	int newvalue;
	OBJID newvaluetrageid;
//modify code by �ֵ�Ԩ end
};
#endif // !defined(AFX_USER_H__BF5C20F1_5DBE_4E65_9099_E96EFAD7ECBA__INCLUDED_1)
