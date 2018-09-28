// ItemType.h: interface for the CItemType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ITEMTYPE_H)
#define ITEMTYPE_H

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
#include <vector>
#include "WeaponSoulData.h"//20071113修罗:
using namespace std;

//////////////////////////////////////////////////////////////////////
#pragma pack(4)//[醒目灯]在此进行4字节对齐。排除__I64对字节对齐的影响,否则此结构体用在消息中,会产生内存偏移.
struct ItemInfoStruct
{
	OBJID	id;	
	OBJID	idType;
	OBJID	idOwner;
	OBJID	idPlayer;
	DWORD	nAmount;
	DWORD	nAmountLimit;
	UCHAR	ucPosition;
	UCHAR	ucPackageIndex;
	UCHAR	nKidnap;
	UCHAR	nNeedCheck;
	DWORD	dwLastCheckTime;
	DWORD	uLifeTime;


	DWORD	BasicAddID[ 2 ];
	DWORD	AdditionAddID[ 8 ];
	DWORD	GemTypeID[ 4 ];
	USHORT	usAddLevFailRate;//[2010-07-19 goto]强化失败后的加成成功率
	USHORT	usStarFailRate;//[2010-07-19 goto]加星失败后的加成成功率

	USHORT	usCapability;
	USHORT	usReqProff;
	USHORT	usReqJobLev;
	USHORT	usReqUserLev;

	DWORD	dwLife;
	DWORD	dwMana;
	DWORD	dwAction;

	UCHAR	dwQual;
	UCHAR	dwStr;
	UCHAR	dwAddLev;
	UCHAR	dwColor;

	char	szName[_MAX_NAMESIZE];
	char	szMakerName[_MAX_NAMESIZE];
	OBJID	idSuit;
	DWORD	dwPerPrice;
	//铸灵
	USHORT	usEquipSoulLev;
	USHORT	usEquipSoulType;
	int		nEquipSoulPoint;

};

struct ItemInfoStructSend
{
	OBJID	id;	
	OBJID	idType;
	OBJID	idOwner;
	OBJID	idPlayer;
	DWORD	nAmount;
	DWORD	nAmountLimit;
	UCHAR	ucPosition;
	UCHAR	ucPackageIndex;
	UCHAR	nKidnap;
	UCHAR	nNeedCheck;
	DWORD	dwLastCheckTime;
	DWORD	uLifeTime;
	
	
	DWORD	BasicAddID[ 2 ];
	DWORD	AdditionAddID[ 8 ];
	DWORD	GemTypeID[ 4 ];
	USHORT	usAddLevFailRate;//[2010-07-19 goto]强化失败后的加成成功率
	USHORT	usStarFailRate;//[2010-07-19 goto]加星失败后的加成成功率
	
	USHORT	usCapability;
	USHORT	usReqProff;
	USHORT	usReqJobLev;
	USHORT	usReqUserLev;
	
	DWORD	dwLife;
	DWORD	dwMana;
	DWORD	dwAction;
	
	UCHAR	dwQual;
	UCHAR	dwStr;
	UCHAR	dwAddLev;
	UCHAR	dwColor;
	
	char	szName[_MAX_NAMESIZE];
	char	szMakerName[_MAX_NAMESIZE];
	OBJID	idSuit;
	DWORD	dwPerPrice;

	ST_WEAPONSOULDATA stWeapenSoul;
	//铸灵
	USHORT	usEquipSoulLev;
	USHORT	usEquipSoulType;
	int		nEquipSoulPoint;
};
#pragma pack()//[醒目灯]在此进行4字节对齐。排除__I64对字节对齐的影响,否则此结构体用在消息中,会产生内存偏移.

//////////////////////////////////////////////////////////////////////
enum ITEMTYPEDATA
{
	ITEMTYPEDATA_ID_=0,
	ITEMTYPEDATA_NAME,
	ITEMTYPEDATA_REQ_PROF,
	ITEMTYPEDATA_REQ_USERLEVEL,	
	ITEMTYPEDATA_REQ_JOBLEVEL,
	ITEMTYPEDATA_REQ_SEX,
	ITEMTYPEDATA_PRICE,
	ITEMTYPEDATA_PRICEOUT,
	ITEMTYPEDATA_ACTION,
	ITEMTYPEDATA_AMOUNT,
	ITEMTYPEDATA_AMOUNT_LIMIT,
	ITEMTYPEDATA_CAPABILITY,
	ITEMTYPEDATA_INIT_LIFE_TIME,
	ITEMTYPEDATA_LIFE,
	ITEMTYPEDATA_MANA,
	ITEMTYPEDATA_BASE1_INIT,
	ITEMTYPEDATA_BASE2_INIT,
	ITEMTYPEDATA_BASIC_ADDITION1_INIT,
	ITEMTYPEDATA_BASIC_ADDITION2_INIT,
	ITEMTYPEDATA_BASIC_ADDITION3_INIT,
	ITEMTYPEDATA_BASIC_ADDITION4_INIT,
	ITEMTYPEDATA_BASIC_ADDITION5_INIT,
	ITEMTYPEDATA_BASIC_ADDITION6_INIT,
	ITEMTYPEDATA_BASIC_ADDITION7_INIT,
	ITEMTYPEDATA_BASIC_ADDITION8_INIT,
	ITEMTYPEDATA_GEM1_INIT,
	ITEMTYPEDATA_GEM2_INIT,
	ITEMTYPEDATA_GEM3_INIT,
	ITEMTYPEDATA_SUIT_ID,
	ITEMTYPEDATA_MONOPOLY,
	ITEMTYPEDATA_DES,
	ITEMTYPEDATA_CD_INIT,
	ITEMTYPEDATA_COLOR_INIT,
	ITEMTYPEDATA_STAR_INIT,
};

char	szItemTypeTable[];
#if	defined(PALED_DEBUG)
typedef	CConstGameData<ITEMTYPEDATA,szItemTypeTable,ITEMTYPEDATA_ID_>	CItemTypeData;
#else
typedef	CGameData<ITEMTYPEDATA,szItemTypeTable,szID>	CItemTypeData;
#endif
typedef	ISingleMap2<CItemTypeData>	IItemTypeSet;
typedef	CSingleMap2<CItemTypeData>	CItemTypeSet;

class IDatabase;
class CItemType
{
protected:
	CItemType();
	virtual ~CItemType();
public:
	static CItemType*	CreateNew	()		{ return new CItemType; }
	ULONG	Release			()				{ delete this; return 0; }
	bool	Create			(IDatabase* pDb);
	CItemTypeData*	QueryItemType(OBJID idType);
	bool	GetInfo(OBJID idType, ItemInfoStruct* pInfo);
    int     GetMonoly(OBJID idType);
protected:
	IItemTypeSet*	m_setType;
	MYHEAP_DECLARATION(s_heap)
};



#endif // !defined(ITEMTYPE_H)
