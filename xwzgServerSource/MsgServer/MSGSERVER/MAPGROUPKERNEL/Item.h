// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__CA7CE885_1DCF_4B96_B308_FB7F40CCF4BA__INCLUDED_)
#define AFX_ITEM_H__CA7CE885_1DCF_4B96_B308_FB7F40CCF4BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemData.h"
#include "MyHeap.h"
#include "T_SingleObjSet2.h"
#include "T_SingleMap2.h"
#include "UserTable.h"
#include "WeaponSoulData.h"


enum PEIFANG_SORT
{
	PEIFANG_SORT_JINGONG1  = 0,
	PEIFANG_SORT_JINGONG2,
	PEIFANG_SORT_DUANZAO1,
	PEIFANG_SORT_DUANZAO2,
	PEIFANG_SORT_ZHIPI1,
	PEIFANG_SORT_ZHIPI2,
};
//////////////////////////////////////////////////////////////////////////
//	20070710	朱斌	金钱使用超过多少额度就记录日志
const	int		LOG_SAVE_MONEY = 1000000;
enum	COSTLY_ITEM_OPER_TYPE	//	20070710	贵重物品操作类型
{
	COSTLY_ITEM_OPER_TYPE_BUY			= 1,		//	NPC买
	COSTLY_ITEM_OPER_TYPE_SELL			= 2,		//	NPC卖
	COSTLY_ITEM_OPER_TYPE_BOOTH_BUY		= 3,		//	摆摊买
	COSTLY_ITEM_OPER_TYPE_BOOTH_SELL	= 4,		//	摆摊卖
	COSTLY_ITEM_OPER_TYPE_TRADE_GET		= 5,		//	交易	买
	COSTLY_ITEM_OPER_TYPE_TRADE_LOST	= 6,		//	交易	卖
	COSTLY_ITEM_OPER_TYPE_PICK			= 7,		//	拾取
	COSTLY_ITEM_OPER_TYPE_DROP			= 8,		//	丢弃
	COSTLY_ITEM_OPER_TYPE_MAIL_POST		= 9,		//	邮寄发
	COSTLY_ITEM_OPER_TYPE_MAIL_RECV		= 10,		//	邮寄收
	COSTLY_ITEM_OPER_TYPE_VAS_BUY		= 11,		//	时空之店购买
	COSTLY_ITEM_OPER_TYPE_EQUIP_CREATE	= 12,		//	打造装备
	COSTLY_ITEM_OPER_TYPE_CREATEITEM	= 13,		//	合成宝石
	COSTLY_ITEM_OPER_TYPE_ACTION_ITEM_ADD		= 14,	//	任务增加物品
	COSTLY_ITEM_OPER_TYPE_ACTION_TEAM_ITEM_ADD	= 15,	//	任务增加小队物品
	COSTLY_ITEM_OPER_TYPE_EQUIP_RECOVER	= 16,		//20071121修罗:装备回收
};


const int _SPCITEM_MONEYFORSHOP	=626100;//跑商银票

//GEM位
enum EM_GEMPOS
{
	EM_GEMPOS_GEM1					= 1,
	EM_GEMPOS_GEM2					= 2,
	EM_GEMPOS_GEM3					= 3,
	EM_GEMPOS_BEGIN = EM_GEMPOS_GEM1,
	EM_GEMPOS_END = EM_GEMPOS_GEM3,
};

//BASE位
enum EM_BASEPOS
{
	EM_BASEPOS_GEM1					= 1,
	EM_BASEPOS_GEM2					= 2,
	EM_BASEPOS_BEGIN = EM_GEMPOS_GEM1,
	EM_BASEPOS_END = EM_GEMPOS_GEM2,
};

//ADD位
enum EM_ADDPOS
{
	EM_ADDPOS_GEM1					= 1,
	EM_ADDPOS_GEM2					= 2,
	EM_ADDPOS_GEM3					= 3,
	EM_ADDPOS_GEM4					= 4,
	EM_ADDPOS_GEM5					= 5,
	EM_ADDPOS_GEM6					= 6,
	EM_ADDPOS_GEM7					= 7,
	EM_ADDPOS_GEM8					= 8,
	EM_ADDPOS_BEGIN = EM_ADDPOS_GEM1,
	EM_ADDPOS_END = EM_ADDPOS_GEM8,
};
//铸灵属性
enum EM_SOULTYPE
{
	EM_SOULTYPE_BING    =20,//冰
	EM_SOULTYPE_HUO		=21,//火
	EM_SOULTYPE_XUAN	=22,//玄
	EM_SOULTYPE_DU		=23,//毒
	EM_SOULTYPE_LI		=9,//力量
	EM_SOULTYPE_LING    =10,//灵力
	EM_SOULTYPE_TI		=11,//体力
	EM_SOULTYPE_DING	=12,//定力
	EM_SOULTYPE_SHEN	=13,//身法
};
class CItem : public CItemData  
{
protected:
	CItem();
	virtual ~CItem();
public://基础
	static CItem* CreateNew()		{ return (new CItem); }
	UINT	ReleaseByOwner()		{ delete this; return 0; }
	int		GetKidnap()	{ return GetInt( ITEMDATA_KIDNAP )>0? 1 : 0; }
	bool	IsSoulMark()			{ return GetInt(ITEMDATA_KIDNAP)==2; }//goto 是否魂印	
	void	SetKidnap( USHORT dat )		{ SetInt( ITEMDATA_KIDNAP, dat );	}

public: // 分类
	bool	IsExpend()		{ return CItem::IsExpend(GetInt(ITEMDATA_TYPE)); } //消耗品
	bool	IsNormal()		{ return GetItemSort() == ITEMSORT_NORMAL; }
	bool	IsOther()		{ return GetItemSort() == ITEMSORT_OTHER; }
	bool	IsFinery1()		{ return GetItemSort() == IETMSORT_FINERY1; }
	bool	IsFinery2()		{ return GetItemSort() == IETMSORT_FINERY2; }
	
	
	bool	IsShoulder()	{ return IsFinery1() && GetItemType() == ITEMTYPE_SHOULDER; }
	bool	IsWeapon()		{ return IsFinery1() && GetItemType() == ITEMTYPE_WEAPONR; }
	bool	IsHelmet()		{ return IsFinery1() && GetItemType() == ITEMTYPE_HELMET;  }
	bool	IsNecklace()	{ return IsFinery1() && GetItemType() == ITEMTYPE_NECKLACE;}
	bool	IsArmor()		{ return IsFinery1() && GetItemType() == ITEMTYPE_ARMOR;   }
	bool	IsRing()		{ return IsFinery2() && GetItemType() == ITEMTYPE_RING;    }
	bool	IsBangle()		{ return IsFinery1() && GetItemType() == ITEMTYPE_BANGLE;  }
	bool	IsGlove()		{ return IsFinery1() && GetItemType() == ITEMTYPE_GLOVE;   }
	bool	IsCaestus()		{ return IsFinery1() && GetItemType() == ITEMTYPE_CAESTUS; }
	bool	IsShoes()		{ return IsFinery1() && GetItemType() == ITEMTYPE_SHOES;   }
	bool	IsAdorn()		{ return IsFinery2() && GetItemType() == IETMTYPE_ADORN;   }
	bool	IsMount()		{ return IsFinery2() && GetItemType() == ITEMTYPE_MOUNT;   }
	bool	IsDress()		{ return IsFinery2() && GetItemType() == ITEMTYPE_DRESS;   }
	bool	IsWeaponSoul()	{ return IsFinery2() && GetItemType() == ITEMTYPE_WEAPONSOUL;   }

	bool	IsEquipment()	{ return CItem::IsEquipment(GetInt(ITEMDATA_TYPE)); }
	bool	IsActionItem()	{ return IsOther(); }
	BOOL	IsMakeHoleStone()			{ if( GetTypeID()>=610000 && GetTypeID()<=610011 ) return true; return false; }//  [6/22/2007]
	static bool IsActionItem(DWORD itemType){return itemType/100000  == ITEMSORT_OTHER;}//20070825修罗
	bool	IsGem()			{ return GetItemSort() == ITEMSORT_GEM; }//20070111修罗
	bool	IsMedicine()	{ return CItem::GetItemSort() == ITEMSORT_EXPEND && GetItemType()!=35; }//goto 排除元宝票和经验丹
	bool   IsPetEgg();//是不是宠物蛋
public: // static
	static bool IsExpend(int nType)		{ return CItem::GetItemSort(nType) == ITEMSORT_EXPEND; }//消耗品
	//是不是可叠加物品
	static bool IsCountable(int nType);
	//06.11.7王玉波添加-------------------------------------------------
	static int GetDelayGroupIndex( OBJID idType );
	static bool	IsEquipment(int nType)	{ return  GetItemSort(nType) == IETMSORT_FINERY1 || GetItemSort(nType) == IETMSORT_FINERY2; }
public://操作
	bool    IsEnableDress()			{return IsDress();}//  [4/5/2007] 哈雷 外装
	bool    IsEnableMount()         {return IsMount(); }// 坐骑 [陈剑飞7/30/2007]
	bool	IsNewRemedy();
	static	bool    IsNewRemedyByItemType( int typeID );
	bool	IsEquipEnable()		{ return IsFinery1() || IsFinery2();}
	bool	IsEatEnable()		{ return IsMedicine(); }
	bool	IsPileEnable()		{ return IsCountable( GetInt( ITEMDATA_TYPE ) ) && GetInt(ITEMDATA_AMOUNTLIMIT) >= 1; }	//	20070627 BUG0000787 朱斌 修正为amount_limit = 1 时,属性为可叠加的物品,可以叠加
	bool	IsRepairEnable()	{ return IsEquipEnable(); }
	// modified by zlong 2003-11-24 ---- chest item can not Exchange and store
	
	bool	IsExchangeEnable()			{ return ( GetInt( ITEMDATA_MONOPOLY ) & EXCHANGE_DISABLE_MASK ) == 0;		}
	bool	IsStorageEnable()			{ return ( GetInt( ITEMDATA_MONOPOLY ) & STORAGE_DISABLE_MASK ) == 0;		}
	bool	IsSellEnable()				{ return ( GetInt( ITEMDATA_MONOPOLY ) & SELL_DISABLE_MASK ) == 0;			}
	bool	IsDropEnable()				{ return ( GetInt( ITEMDATA_MONOPOLY ) & DROP_DISABLE_MASK ) == 0;			}
	bool	IsNeverDropWhenDead()		{ return ( GetInt( ITEMDATA_MONOPOLY ) & NEVER_DROP_WHEN_DEAD_MASK ) != 0;	}
	bool	IsKidnapWhenPickup()		{ return ( GetInt( ITEMDATA_MONOPOLY ) & KIDNAP_PICKUP_MASK ) != 0;			}
	bool	IsKidnapWhenEquip()			{ return ( GetInt( ITEMDATA_MONOPOLY ) & KIDNAP_EQUIP_MASK ) != 0;			}
	bool    IsDropWhenDead()            { return ( GetInt( ITEMDATA_MONOPOLY ) & DROP_WHEN_DEAD_MASK ) !=0;         }
	bool	IsUserExclusive()			{ return CItem::IsUserExclusive( GetInt( ITEMDATA_MONOPOLY ));				}
	static	bool IsUserExclusive(unsigned int uMonopoly);

	bool IsActivatePickup()				{ return CItem::IsActivatePickup( GetInt( ITEMDATA_MONOPOLY ));		};			//  [2007-9-14] 朱斌	获得激活
	bool IsActivateEquip()				{ return CItem::IsActivateEquip( GetInt( ITEMDATA_MONOPOLY ));		};//  [2007-9-14] 朱斌	装备激活
	bool IsLifeTimeContinue()			{ return CItem::IsLifeTimeContinue( GetInt( ITEMDATA_MONOPOLY ));	};//  [2007-9-14] 朱斌	可以续费
	bool IsLifeTimeNonContinue()		{ return CItem::IsLifeTimeNonContinue( GetInt( ITEMDATA_MONOPOLY ));};//  [2007-9-14] 朱斌	不能续费
	bool IsChanceDropEquipWhenDead()	{ return CItem::IsChanceDropEquipWhenDead(GetInt(ITEMDATA_MONOPOLY));}//  [1/27/2008 朱斌]	死后有机率掉的装备
	bool IsUnkidnap()					{ return CItem::IsUnkidnap(GetInt(ITEMDATA_MONOPOLY));}//  [1/27/2008 朱斌]	不可绑定
	
	static	bool IsActivatePickup(unsigned int uMonopoly);	//  [2007-9-14] 朱斌	获得激活
	static	bool IsActivateEquip(unsigned int uMonopoly);	//  [2007-9-14] 朱斌	装备激活
	static	bool IsLifeTimeContinue(unsigned int uMonopoly);		//  [2007-9-14] 朱斌	可以续费
	static	bool IsLifeTimeNonContinue(unsigned int uMonopoly);		//  [2007-9-14] 朱斌	不能续费
	static	bool IsChanceDropEquipWhenDead(unsigned int uMonopoly);	//  [1/27/2008 朱斌]	死后有机率掉的装备
	static	bool IsUnkidnap(unsigned int uMonopoly);	//  [1/27/2008 朱斌]	不可绑定

	bool	IsNonsuchItem();
	int		GetEmbedCountLimit();
	int		GetEmbedGemCount();


	int		GetEmbedPos();
	bool	IsCanEmbedEquip();
	bool	IsHaveSameGemEmbed(OBJID idGemType);//[2010-07-19 goto] 装备中是否已经镶嵌了此类宝石
	bool	IsRightGemType(OBJID idGemType);//[2010-07-23 goto] 要镶嵌的宝石类型是否与装备匹配
public:	// get
	int		GetLife()			{ return GetInt( ITEMDATA_LIFE );		}
	OBJID	GetID()			{ return CItemData::GetID();}
	int		GetCapability()		{ return GetInt(ITEMDATA_CAPABILITY_INIT);}
	int		GetEquipQual()		{ return GetInt(ITEMDATA_QUALITY);}
	int		GetEquipStar()		{ return GetInt(ITEMDATA_STAR);}
	int		GetEquipAddLev()	{ return GetInt(ITEMDATA_ADDLEV);}
	//[2010-07-19 goto]装备强化与加星
	int		GetEquipAddLevFailRate()	{ return GetInt(ITEMDATA_ADDLEV_FAILRATE); }//获取装备强化失败加成的成功率,100%=10000
	void	SetEquipAddLevFailRate(int nData, bool bUpdate=false)	{ SetInt(ITEMDATA_ADDLEV_FAILRATE,nData,bUpdate); }

	int		GetEquipStarFailRate()	{ return GetInt(ITEMDATA_ADDSTAR_FAILRATE); }//获取装备升星失败加成的成功率,100%=10000
	void	SetEquipStarFailRate(int nData, bool bUpdate=false)	{ SetInt(ITEMDATA_ADDSTAR_FAILRATE,nData,bUpdate); }

	void	SetEquipStar(int nData, bool bUpdate=false)		{ SetInt(ITEMDATA_STAR,nData,bUpdate); }//装备星等
	void	SetEquipAddLev(int nData, bool bUpdate=false)	{ SetInt(ITEMDATA_ADDLEV,nData,bUpdate); }//装备强化等级
	int		GetNeedGemLev(int nEquipLev);//获取装备强化或加星需要的强化宝石等级
	int		GetEquipAddLevCost(int nType);//获取装备强化或加星需要的银两
	float	GetEquipAddRate(int nLev);//获得装备强化或升星后的属性加成系数
	
	void	UpdateEquipBaseProperty(int nOldLev, int nType=0);//强化后更新装备基础属性
	void	UpdateEquipAdditionProperty(int nOldLev);//升星后更新装备附加属性

	float	GetSucRate(short nType);//[2010-08-24 goto]获取装备强化或加星单个符的成功率
	int		GetWeaponSpecialView();

public: // const
	int		GetTypeID()			{ return GetInt( ITEMDATA_TYPE );}
	int		CalcRepairMoney();
	DWORD	GetSellPrice();
	int		GetItemAmount()					{	return GetInt( ITEMDATA_AMOUNT );		}// 物品的数量
	void	SetItemAmount( int nAmount )	{	SetInt( ITEMDATA_AMOUNT,nAmount );	}
	int		GetItemAmountLimit()			{	return GetInt( ITEMDATA_AMOUNTLIMIT );		}// 物品的数量

	int		GetChaosHoleState()				{ return GetInt(ITEMDATA_GEM4); }//第四个特殊孔状态

public: // application
	CItem*	Split(int nNum, IDatabase* pDb);

public:
	int		GetItemSubType();

	int		GetLevel	(void);
	int		GetColour	(void)	{return GetInt(ITEMDATA_COLOR);}
	bool	ChangeType	(OBJID idNewType);
	bool	ChangeColor	(int nColor);

public: // 类型
	int		GetItemSort()			{ return CItem::GetItemSort(GetInt(ITEMDATA_TYPE)); }
	int		GetItemType()			{ return CItem::GetItemType(GetInt(ITEMDATA_TYPE)); }
public:
	static int	GetItemSort(OBJID idType)			{ return idType/100000; } //取十万位		
	static int	GetItemType(OBJID idType)			{ return idType/10000;	} //取十万、万位		

public:
	void	SetGemStatus( unsigned char ucPos,OBJID idType,bool bUpdate = false );
	OBJID	GetGemStatus( int nPos );
	OBJID	GetAddStatus( int nPos );
	OBJID	GetBasicStatus( int nPos );

	static int GetEudemonNeedExp(int nlevel);
	UCHAR	GetPackageIndex()								{ return	GetInt( ITEMDATA_PACKAGE_INDEX );		}
	int		GetPosition()									{ return	GetInt( ITEMDATA_POSITION );			}
	void	SetPosition( int position,bool update = false )			{ SetInt( ITEMDATA_POSITION,position,update );		}
	void	SetPackageIndex( UCHAR ucIndex,bool bUpdate = false )	{ SetInt( ITEMDATA_PACKAGE_INDEX,ucIndex,bUpdate );	}

  
	bool    CreateHole();	
	bool	IsLocked()	{ return m_isLocked; }
	void	setLocked(bool bLocked)	{ m_isLocked=bLocked; }
protected: // 
	CItem*	Clone(IDatabase* pDb);		// 新物品存数据库
	bool m_isLocked;//[2010-06-28 goto]是否锁定，交易的时候锁定物品，不允许使用

public:
	unsigned int GetLifeTime();
	const char* GetLastCheckTime();
	static BOOL IsLogSaveMoney(int nMoney);	//	20070710	朱斌	金钱使用是否记录
	BOOL IsCostlyItem();	//	20070710 朱斌 是否为贵重物品
	int	GetReqLev();//获得需求等级,可升级武器特殊判断
	int	GetReqJobLev();

	BOOL IsValidEquip(CUser* pUser = NULL);

	static BOOL CanRepair(DWORD itemType);
	BOOL CanRepair() {return CanRepair(GetTypeID());}//重载


	static BOOL IsCanUpLevel(DWORD itemType);
	BOOL IsCanUpLevel() {return IsCanUpLevel(GetTypeID());}//重载
	bool CanCutWork();//宝石能否雕琢
	static DWORD GetWeaponLevExp(WORD lev);
public:
	bool		Create			(IDatabase* pDb, 
		IRecord* pDefault, 
		const ItemInfoStruct* pInfo, 
		IRecord* pDefaultEudemonData, 
		IRecord* pDefaultWpSl,//武魂
		bool bInsert=true, 
		OBJID idNew=ID_NONE);
	bool		CreateSend			(IDatabase* pDb, 
		IRecord* pDefault, 
		const ItemInfoStructSend* pInfo, 
		IRecord* pDefaultWpSl,//武魂
		bool bInsert=true, 
		OBJID idNew=ID_NONE);
	bool		CreateSoul		(IDatabase* pDb,IRecord* pDefault,const ItemInfoStruct* pInfo,IRecord* pDefaultWpSl,ST_WEAPONSOULDATA stData,bool bInsert=true,OBJID idNew=ID_NONE);
	bool		Create			(OBJID idItem, IDatabase* pDb);
	bool		Create			(IRecordset* pRes, IDatabase* pDb);
	bool		GetInfo	(ItemInfoStructSend* pInfo);
	bool		GetInfo	(ItemInfoStruct* pInfo);
//WEAPENSOUL相关
public:
	static BOOL	IsWeaponSoulExpItem(OBJID idType) { return idType / 10000 == 39;}
	BOOL		IsWeaponSoulExpItem(){ return IsWeaponSoulExpItem(GetTypeID());}//重载
	int			GetItemAdditionData(int nIndex);//获得物品某项附加属性的值,参数从1到8

	BOOL		GetWpSlInfo(ST_WEAPONSOULDATA& stWpSl);
	void		EraseWpSl(IDatabase* pDb);//丢弃武魂
	CWeaponSoulData* QueryWeapensoulData() { return m_pWpSlData;}
	DWORD		GetWpSlLev();
	DWORD		GetWpSlType();
//铸灵&铸灵转移相关--jay
public:
	short		GetEquipSoulLev();//得到铸灵等级
	short		GetEquipSoulType();//得到铸灵属性
	DWORD		GetEquipSoulPoint();//得到魔灵数
	void		SetEquipSoulLev(short nLev);//得到铸灵等级
	void		SetEquipSoulType(short nType);//得到铸灵属性
	void		SetEquipSoulPoint(OBJID nPoint);//得到魔灵数
	bool		IsActEquipment(){ return IsWeapon() || IsRing() || IsAdorn() || IsNecklace() || IsBangle();}//是攻击类 --jay
	bool		IsDefEquipment(){ return IsArmor() || IsHelmet() || IsShoulder() || IsGlove() || IsCaestus() || IsShoes();}//是防御类物品

	bool		IsSoulBook(){return GetItemType() == ITEMTYPE_SOULBOOK;}
	bool        IsActSoulBook();
	bool		IsDefSoulBook();
	short		GetSoulBookType();

protected:
	virtual void	OnSaveInfo();
	BOOL		CreateWpSlData(IDatabase* pDb);//创建武魂
	BOOL		CreateWpSlData(const ST_WEAPONSOULDATA* pstWpSlData,IRecord* pDefaultWpSl,IDatabase* pDb,bool bInsert = true);
	void		DeletWpSlData();//清除所有武魂数据.析构时调用
	CWeaponSoulData* m_pWpSlData;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_ITEM_H__CA7CE885_1DCF_4B96_B308_FB7F40CCF4BA__INCLUDED_)



