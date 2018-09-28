// ItemData.h: interface for the CItemData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDATA_H__28794933_A926_4C9A_AACF_1A95340DA94B__INCLUDED_)
#define AFX_ITEMDATA_H__28794933_A926_4C9A_AACF_1A95340DA94B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "ItemType.h"

//////////////////////////////////////////////////////////////////////
const int MAX_INTIMACY		= 255;
//////////////////////////////////////////////////////////////////////
enum //宝石名定义
{
	_ERROR_GEM_TYPE=-1,
	_NOT_HOLE=99999,
	_HOLE=88888,
};


enum //特殊效果
{
	_SPECIAL_NONE=-1,
	_SPECIAL_LEVEL1=1,
	_SPECIAL_LEVEL2=2,
	_SPECIAL_LEVEL3=3,
	_SPECIAL_LEVEL4=4,
	_SPECIAL_LEVEL5=5,
	_SPECIAL_LEVEL6=6,
	_SPECIAL_LEVEL7=7,
	_SPECIAL_LEVEL8=8,
	_SPECIAL_LEVEL9=9,	
    _SPECIAL_MAX_LEVEL=_SPECIAL_LEVEL9,
};

//////////////////////////////////////////////////////////////////////
//物品MASK标识
const DWORD	EXCHANGE_DISABLE_MASK				= 0x01;				//不可交易
const DWORD	STORAGE_DISABLE_MASK				= 0x02;				//不可存仓库
const DWORD	DROP_HINT_MASK						= 0x04;				//丢弃有提示
const DWORD	SELL_HINT_MASK						= 0x08;				//卖出有提示
const DWORD	NEVER_DROP_WHEN_DEAD_MASK			= 0x10;				//死亡不掉
const DWORD	SELL_DISABLE_MASK					= 0x20;				//不能出售
const DWORD DROP_DISABLE_MASK					= 0x40;				//不能丢弃
//const DWORD POST_DISABLE_MASK					= 0x80;				//不可邮寄		//目前最大了,因为表中字段为unsigned 1字节
const DWORD COSTLY_ITEM							= 0x80;				//  贵重物品
const DWORD USER_EXCLUSIVE_MASK					= 0x0100;			//	20070712	用户唯一
const DWORD KIDNAP_EQUIP_MASK					= 0x0200;			// 装备后绑定 [9/5/2007 Add By Halei] 
const DWORD KIDNAP_PICKUP_MASK					= 0x0400;			// 拾取绑定 [9/5/2007 Add By Halei] 
const DWORD ACTIVATE_PICKUP						= 0x0800;			//  [2007-9-14] 朱斌	获得激活
const DWORD ACTIVATE_EQUIP						= 0x1000;			//  [2007-9-14] 朱斌	装备激活
const DWORD LIFETIME_CONTINUE					= 0x2000;			//  [2007-9-14] 朱斌	可以续费
const DWORD LIFETIME_NON_CONTINUE				= 0x4000;			//  [2007-9-14] 朱斌	不能续费
const DWORD DROP_WHEN_DEAD_MASK                 = 0x8000;           // 死亡后必掉落 [陈剑飞9/13/2007]
const DWORD CHANCE_DROP_EQUIP_WHEN_DEAD			= 0x00010000;		//  [1/27/2008 朱斌]	死亡后有一定机率掉的装备
const DWORD UNKIDNAP							= 0x00020000;	//  [1/27/2008 朱斌]	不可绑定

const int	MAGIC_NONE				= 0;
const int	GEM_NONE				= 99999;
const int	GEM_HOLE				= 255;

//////////////////////////////////////////////////////////////////////
// 物品大的分类
const int	ITEMSORT_INVALID				=	-1;
const int	IETMSORT_FINERY1				=	1;	// 装备
const int	IETMSORT_FINERY2				=	2;	// 装备
const int	ITEMSORT_EXPEND					=	3;	// 易耗品
const int	ITEMSORT_GEM					=	4;	// 宝石
const int	ITEMSORT_OTHER					=	5;	// action物品
const int	ITEMSORT_NORMAL					=	6;	// 材料
const int	IETMSORT_EUDEMON				=	7;	// 宠物

//////////////////////////////////////////////////////////////////////
// ITEMSORT_FINERY 类别物品
const int	ITEMTYPE_INVALID		=	-1;	  // 非法
const int	ITEMTYPE_SHOULDER		=	11;	// 护肩
const int	ITEMTYPE_ARMOR			=	12;	// 盔甲
const int	ITEMTYPE_HELMET			=	13;	// 头盔
const int	ITEMTYPE_WEAPONR		=	14;	// 武器
const int	ITEMTYPE_NECKLACE		=	15;	// 项链
const int	ITEMTYPE_BANGLE			=	16;	// 手镯
const int	ITEMTYPE_GLOVE			=	17;	// 手套
const int	ITEMTYPE_CAESTUS		=	18;	// 要带
const int	ITEMTYPE_SHOES			=	19;	// 鞋子
const int	ITEMTYPE_MOUNT			=	20;	// 坐骑
const int	ITEMTYPE_RING			=	21;	// 戒指
const int	IETMTYPE_ADORN			=	22;	// 饰品
const int   ITEMTYPE_DRESS			=   23; // 外装
const int   ITEMTYPE_TREASURE		=   24; // 法宝
const int   ITEMTYPE_WEAPONSOUL		=   25; // 武魂
const int   ITEMTYPE_SOULBOOK       =   43; // 铸灵书


//////////////////////////////////////////////////////////////////////
//const int	ITEMPOSITION_NONE		= 0;
const int	ITEMPOSITION_EQUIPBEGIN	=	1;
const int	ITEMPOSITION_SHOULDER	=	1;	// 护肩
const int	ITEMPOSITION_ARMOR		=	2;	// 盔甲
const int	ITEMPOSITION_HELMET		=	3;	// 头盔
const int	ITEMPOSITION_WEAPONR	=	4;	// 武器
const int	ITEMPOSITION_NECKLACE	=	5;	// 项链
const int	ITEMPOSITION_BANGLE		=	6;	// 手镯
const int	ITEMPOSITION_GLOVE		=	7;	// 手套
const int	ITEMPOSITION_CAESTUS	=	8;	// 要带
const int	ITEMPOSITION_SHOES		=	9;	// 鞋子
const int   ITEMPOSITION_MOUNT      =	10; // 坐骑
const int	ITEMPOSITION_RINGT		=	11;	// 上戒指	
const int	ITEMPOSITION_RINGB		=	12;	// 下戒指
const int	ITEMPOSITION_ADORNT		=	13;	// 上饰品	
const int	ITEMPOSITION_ADORNB		=	14;	// 下饰品
const int   ITEMPOSITION_DRESS		=	15; //外装
const int   ITEMPOSITION_WEAPONSOUL	=	16; //武魂
const int	ITEMPOSITION_EQUIPEND	=	17;//

const int	ITEMPOSITION_PACK_BEGIN			= 47;
const int	ITEMPOSITION_BACKPACK1			= 47;	// 普通物品背包1
const int	ITEMPOSITION_BACKPACK2			= 48;	// 普通物品背包2
const int	ITEMPOSITION_BACKPACK3			= 49;	// 普通物品背包3
const int	ITEMPOSITION_PACK_END			= 50;
const int	ITEMPOSITION_PETPACK			= 99;
const int	ITEMPOSITION_PACK_LIMIT			= 100;

//////////////////////////////////////////////////////////////////////
const int	ITEMPOSITION_STORAGE	= 201;			
const int	ITEMPOSITION_TRUNK		= 202;
const int	ITEMPOSITION_CHEST		= 203;
const int	ITEMPOSITION_PLAYERTASK	= 204;			// 佣兵任务奖品
const int   ITEMPOSITION_SENDBOX	= 205;			// 邮局发件箱
const int	ITEMPOSITION_GROUND		= 254;			// 不存数据库
const int	ITEMPOSITION_NONE		= 255;			// 非法位置
const int   ITEMPOSITION_MARRYSTORAGE = 208;        //婚姻仓库

const int   ITEMPOSITION_MARRY_BED_MATERIAL1 = 209;        //床材料1
const int   ITEMPOSITION_MARRY_BED_MATERIAL2 = 210;        //床材料2
const int   ITEMPOSITION_MARRY_BED_MATERIAL3 = 211;        //床材料3

const int   ITEMPOSITION_MARRY_DEPORT_MATERIAL1 = 212;        //柜子材料1
const int   ITEMPOSITION_MARRY_DEPORT_MATERIAL2 = 213;        //柜子材料2
const int   ITEMPOSITION_MARRY_DEPORT_MATERIAL3 = 214;        //柜子材料3

const int   ITEMPOSITION_MARRY_FLOWER_MATERIAL1 = 215;        //花材料1
const int   ITEMPOSITION_MARRY_FLOWER_MATERIAL2 = 216;        //花材料2
const int   ITEMPOSITION_MARRY_FLOWER_MATERIAL3 = 217;        //花材料3

const int   ITEMPOSITION_MARRY_CABINET_MATERIAL1 = 218;        //厨房材料1
const int   ITEMPOSITION_MARRY_CABINET_MATERIAL2 = 219;        //厨房材料2
const int   ITEMPOSITION_MARRY_CABINET_MATERIAL3 = 220;        //厨房材料3

//////////////////////////////////////////////////////////////////////
//06.12.29王玉波添加
//宝石可镶嵌在装备中的哪个位置
const USHORT	EMBEDPOS_INVALID	= 0x0000;//无效
const USHORT	EMBEDPOS_HELMET		= 0x0001;//头盔
const USHORT	EMBEDPOS_NECKLACE	= 0x0002;//项链
const USHORT	EMBEDPOS_ARMOR		= 0x0004;//衣服
const USHORT	EMBEDPOS_WEAPON		= 0x0008;//武器
const USHORT	EMBEDPOS_RING		= 0x0010;//戒指
const USHORT	EMBEDPOS_GLOVE		= 0x0020;//手套
const USHORT	EMBEDPOS_BANGLE		= 0x0040;//手镯
const USHORT	EMBEDPOS_CAESTUS	= 0x0080;//腰带
const USHORT	EMBEDPOS_SHOES		= 0x0100;//鞋子

const int CITYWAR_ITEM_TYPEID = 520000;      //城战宣战物品id
enum ITEMDATA
{
/*------------------item表-------begin--------------------------------------------------------------*/
		ITEMDATA_ID_=0,				// use for insert
		ITEMDATA_TYPE=1,			
		ITEMDATA_OWNERID,
		ITEMDATA_PLAYERID,
		ITEMDATA_AMOUNT,
		ITEMDATA_AMOUNTLIMIT,
		ITEMDATA_POSITION,
		ITEMDATA_PACKAGE_INDEX,
		ITEMDATA_KIDNAP,
		ITEMDATA_NEEDCHECK,
		ITEMDATA_LAST_CHECK_TIME,
		ITEMDATA_LIFE_TIME,
		ITEMDATA_QUALITY,
		ITEMDATA_STAR,
		ITEMDATA_ADDLEV,
		ITEMDATA_BASE1,
		ITEMDATA_BASE2,
		ITEMDATA_BASIC_ADDITION1,
		ITEMDATA_BASIC_ADDITION2,
		ITEMDATA_BASIC_ADDITION3,
		ITEMDATA_BASIC_ADDITION4,
		ITEMDATA_BASIC_ADDITION5,
		ITEMDATA_BASIC_ADDITION6,
		ITEMDATA_BASIC_ADDITION7,
		ITEMDATA_BASIC_ADDITION8,
		ITEMDATA_GEM1,
		ITEMDATA_GEM2,
		ITEMDATA_GEM3,
		ITEMDATA_MAKERNAME,
		ITEMDATA_ADDLEV_FAILRATE,//[2010-07-19 goto]强化失败后的加成成功率 100%=10000
		ITEMDATA_ADDSTAR_FAILRATE,//[2010-07-19 goto]加星失败后的加成成功率 100%=10000
		ITEMDATA_COLOR,
		ITEMDATA_BUYPRICE,
		ITEMDATA_GEM4,
		ITEMDATA_SOULSTATE,
/*---------------item表------end---------------------------------------------------------------------*/
/*---------------itemtype表-------begin--------------------------------------------------------------*/
		ITEMTYPEDATA_OFFSET=100,
		ITEMDATA_NAME=101,
		ITEMDATA_REQ_PROF,
		ITEMDATA_REQ_USERLEVEL,	
		ITEMDATA_REQ_JOBLEVEL,
		ITEMDATA_REQ_SEX,
		ITEMDATA_PRICE,
		ITEMDATA_PRICEOUT,
		ITEMDATA_ACTION,
		ITEMDATA_AMOUNT_INIT,
		ITEMDATA_AMOUNT_LIMIT_INIT,
		ITEMDATA_CAPABILITY_INIT,
		ITEMDATA_INIT_LIFE_TIME,
		ITEMDATA_LIFE,
		ITEMDATA_MANA,
		ITEMDATA_BASE1_INIT,
		ITEMDATA_BASE2_INIT,
		ITEMDATA_BASIC_ADDITION1_INIT,
		ITEMDATA_BASIC_ADDITION2_INIT,
		ITEMDATA_BASIC_ADDITION3_INIT,
		ITEMDATA_BASIC_ADDITION4_INIT,
		ITEMDATA_BASIC_ADDITION5_INIT,
		ITEMDATA_BASIC_ADDITION6_INIT,
		ITEMDATA_BASIC_ADDITION7_INIT,
		ITEMDATA_BASIC_ADDITION8_INIT,
		ITEMDATA_GEM1_INIT,
		ITEMDATA_GEM2_INIT,
		ITEMDATA_GEM3_INIT,
		ITEMDATA_SUIT_ID,
		ITEMDATA_MONOPOLY,
		ITEMDATA_DES,
		ITEMDATA_ITEMCD,
		ITEMDATA_COLOR_INIT,
		ITEMDATA_STAR_INIT,
/*-------------------itemtype表---------end------------------------------------------------------------*/
};
char	szItemTable[];
typedef	CGameData<ITEMDATA,szItemTable,szID>		CGameItemData;

//////////////////////////////////////////////////////////////////////
enum EUDEMONDATA{
	EUDEMONDATA_id = 0,			
	EUDEMONDATA_owner_id,
	EUDEMONDATA_typeid,
	EUDEMONDATA_playtime,//EUDEMONDATA_itemid,//用这个空闲字段作为玩耍次数
	EUDEMONDATA_name,
	EUDEMONDATA_life,
	EUDEMONDATA_level,					
	EUDEMONDATA_exp,				
	EUDEMONDATA_sex,	
	EUDEMONDATA_gettype,
	EUDEMONDATA_happy,
	EUDEMONDATA_understand,
	EUDEMONDATA_bone, 
	EUDEMONDATA_habit,
	EUDEMONDATA_lifespan,
	EUDEMONDATA_gift,
    EUDEMONDATA_borntime,
	EUDEMONDATA_grow,
	EUDEMONDATA_addpoint,
	EUDEMONDATA_firstr,
	EUDEMONDATA_stradd,
	EUDEMONDATA_strchg,
	EUDEMONDATA_firint,
	EUDEMONDATA_intadd,
	EUDEMONDATA_intchg,
	EUDEMONDATA_firsta,
	EUDEMONDATA_staadd,
	EUDEMONDATA_stachg,
	EUDEMONDATA_firspi,
	EUDEMONDATA_spiadd,
	EUDEMONDATA_spichg,
	EUDEMONDATA_firagi,
	EUDEMONDATA_agiadd,
	EUDEMONDATA_agichg,
	EUDEMONDATA_skill_1,
	EUDEMONDATA_skill_2,
	EUDEMONDATA_skill_3,
	EUDEMONDATA_skill_4,
	EUDEMONDATA_skill_5,
	EUDEMONDATA_skill_6,
	EUDEMONDATA_skill_7,
	EUDEMONDATA_skill_8,
	EUDEMONDATA_skill_9,
	EUDEMONDATA_skill_10,
	EUDEMONDATA_state,
	EUDEMONDATA_lastborn,
	EUDEMONDATA_failtime,
	EUDEMONDATA_MaxScore,
	EUDEMONDATA_skill1_lastuse,
	EUDEMONDATA_lastplay,//EUDEMONDATA_skill2_lastuse,//改成玩耍时间字段
	EUDEMONDATA_fusion,//默契
	EUDEMONDATA_intelligence,//灵性
};

struct EudemonData{
	//动态表
	OBJID id;				
	OBJID ownerId;
	DWORD typeId;
	DWORD playtimes;	
	char name[64];
	DWORD life;	
	DWORD level;					
	DWORD exp;			
	DWORD sex;
	DWORD fusion;//默契
	DWORD intelligence;//灵性
	DWORD gettype;
	DWORD happy;
	DWORD understand;
	DWORD bone;
	DWORD habit;
	DWORD lifespan;
	DWORD gift;
	DWORD borntime;
	DWORD grow;	
	DWORD addpoint;

	DWORD firstr;
	DWORD stradd;
	DWORD strchg;
	DWORD firint;
	DWORD intadd;
	DWORD intchg;
	DWORD firsta;
	DWORD staadd;
	DWORD stachg;
	DWORD firspi;
	DWORD spiadd;
	DWORD spichg;
	DWORD firagi;
	DWORD agiadd;
	DWORD agichg;
	DWORD skill[10];
	DWORD failtime;

	//静态表
	DWORD dwLookface;
	DWORD dwNeedlev;
	DWORD initgrow;
	DWORD maxstrchg;
	DWORD maxintchg;
	DWORD maxstachg;
	DWORD maxspichg;
	DWORD maxagichg;
	DWORD maxlifespan;

	//2级属性
	DWORD dwMaxLife;
	DWORD dwScore;
	DWORD dwState;
	DWORD dwfirStrChg;
	DWORD dwfirIntChg;
	DWORD dwfirStaChg;
	DWORD dwfirSpiChg;
	DWORD dwfirAgiChg;
	DWORD dwStrChg;
	DWORD dwIntChg;
	DWORD dwStaChg;
	DWORD dwSpiChg;
	DWORD dwAgiChg;
	DWORD dwStr;
	DWORD dwInt;
	DWORD dwSta;
	DWORD dwSpi;
	DWORD dwAgi;
	DWORD dwPhyAtt;
	DWORD dwMagAtt;
	DWORD dwPhyDef;
	DWORD dwMagDef;
	DWORD dwhit;
	DWORD dwDoge;
	DWORD dwCrit;
	DWORD dwTough;
};

char	szEudemonTable[];
typedef		CGameData<EUDEMONDATA, szEudemonTable, szID>	CEudemonData;


enum PEIFANGDATA{
	PEIFANGDATA_id = 0,			
	PEIFANGDATA_owner_id,
	PEIFANGDATA_peifangid,
};

struct ST_PEIFANGDATA
{
	OBJID id;
	OBJID owner_id;
	OBJID idPeifang;
};
char	szPeifangTable[];
typedef		CGameData<PEIFANGDATA, szPeifangTable, szID>	CPeifangData;

//////////////////////////////////////////////////////////////////////
#include "T_SingleObjSet2.h"
#include "T_SingleMap2.h"
#include "UserTable.h"
#include "ItemAddition.h"

//////////////////////////////////////////////////////////////////////
class IDatabase;
class IRecord;
class IRecordset;
class CItemData : public CGameObj  
{
protected:
	CItemData();
	virtual ~CItemData();

public:
 	bool		Create			(IDatabase* pDb, IRecord* pDefault, const ItemInfoStruct* pInfo, IRecord* pDefaultEudemonData, bool bInsert=true, OBJID idNew=ID_NONE );
	bool		CreateSend			(IDatabase* pDb, IRecord* pDefault, const ItemInfoStructSend* pInfo, IRecord* pDefaultEudemonData, bool bInsert=true, OBJID idNew=ID_NONE );	
//	bool		Create			(IRecord* pDefault, OBJID idType, OBJID idUser, int nPosition, IRecord* pDefaultEudemonData=NULL);//06.12.29王玉波注释
	bool		Create			(OBJID idItem, IDatabase* pDb);
	bool		Create			(IRecordset* pRes, IDatabase* pDb);
	bool		DeleteRecord	(IDatabase* pDb, bool bDelEudemon=true);
//	static bool		DeleteItemRecord	(OBJID id, IDatabase* pDb);

public: // info
	bool		GetInfo	(ItemInfoStruct* pInfo);
	bool		GetInfo	(ItemInfoStructSend* pInfo);
	bool		SaveInfo		(void);
	virtual void		OnSaveInfo(){};

public: // get set
	void		AdjustItemData();
	//bool		IsCanUpLevel();20070714修罗:拿到CItem
	OBJID		GetID()					{ return m_pData->GetKey(); }

	int			GetInt(ITEMDATA idx);
	LPCTSTR		GetStr(ITEMDATA idx);
	void		SetInt(ITEMDATA idx, int nData, bool bUpdate = false);
	void		SetStr(ITEMDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false);

	_int64		GetInt64(ITEMDATA idx);
	void		SetInt64(ITEMDATA idx, __int64 i64Data, bool bUpdate = false);
protected: // data & type
	CGameItemData*		m_pData;
	CItemTypeData*		m_pType;
	bool				m_bNeedUpdate;

};

#endif // !defined(AFX_ITEMDATA_H__28794933_A926_4C9A_AACF_1A95340DA94B__INCLUDED_)
