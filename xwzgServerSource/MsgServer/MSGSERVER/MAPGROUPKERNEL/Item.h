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
//	20070710	���	��Ǯʹ�ó������ٶ�Ⱦͼ�¼��־
const	int		LOG_SAVE_MONEY = 1000000;
enum	COSTLY_ITEM_OPER_TYPE	//	20070710	������Ʒ��������
{
	COSTLY_ITEM_OPER_TYPE_BUY			= 1,		//	NPC��
	COSTLY_ITEM_OPER_TYPE_SELL			= 2,		//	NPC��
	COSTLY_ITEM_OPER_TYPE_BOOTH_BUY		= 3,		//	��̯��
	COSTLY_ITEM_OPER_TYPE_BOOTH_SELL	= 4,		//	��̯��
	COSTLY_ITEM_OPER_TYPE_TRADE_GET		= 5,		//	����	��
	COSTLY_ITEM_OPER_TYPE_TRADE_LOST	= 6,		//	����	��
	COSTLY_ITEM_OPER_TYPE_PICK			= 7,		//	ʰȡ
	COSTLY_ITEM_OPER_TYPE_DROP			= 8,		//	����
	COSTLY_ITEM_OPER_TYPE_MAIL_POST		= 9,		//	�ʼķ�
	COSTLY_ITEM_OPER_TYPE_MAIL_RECV		= 10,		//	�ʼ���
	COSTLY_ITEM_OPER_TYPE_VAS_BUY		= 11,		//	ʱ��֮�깺��
	COSTLY_ITEM_OPER_TYPE_EQUIP_CREATE	= 12,		//	����װ��
	COSTLY_ITEM_OPER_TYPE_CREATEITEM	= 13,		//	�ϳɱ�ʯ
	COSTLY_ITEM_OPER_TYPE_ACTION_ITEM_ADD		= 14,	//	����������Ʒ
	COSTLY_ITEM_OPER_TYPE_ACTION_TEAM_ITEM_ADD	= 15,	//	��������С����Ʒ
	COSTLY_ITEM_OPER_TYPE_EQUIP_RECOVER	= 16,		//20071121����:װ������
};


const int _SPCITEM_MONEYFORSHOP	=626100;//������Ʊ

//GEMλ
enum EM_GEMPOS
{
	EM_GEMPOS_GEM1					= 1,
	EM_GEMPOS_GEM2					= 2,
	EM_GEMPOS_GEM3					= 3,
	EM_GEMPOS_BEGIN = EM_GEMPOS_GEM1,
	EM_GEMPOS_END = EM_GEMPOS_GEM3,
};

//BASEλ
enum EM_BASEPOS
{
	EM_BASEPOS_GEM1					= 1,
	EM_BASEPOS_GEM2					= 2,
	EM_BASEPOS_BEGIN = EM_GEMPOS_GEM1,
	EM_BASEPOS_END = EM_GEMPOS_GEM2,
};

//ADDλ
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
//��������
enum EM_SOULTYPE
{
	EM_SOULTYPE_BING    =20,//��
	EM_SOULTYPE_HUO		=21,//��
	EM_SOULTYPE_XUAN	=22,//��
	EM_SOULTYPE_DU		=23,//��
	EM_SOULTYPE_LI		=9,//����
	EM_SOULTYPE_LING    =10,//����
	EM_SOULTYPE_TI		=11,//����
	EM_SOULTYPE_DING	=12,//����
	EM_SOULTYPE_SHEN	=13,//��
};
class CItem : public CItemData  
{
protected:
	CItem();
	virtual ~CItem();
public://����
	static CItem* CreateNew()		{ return (new CItem); }
	UINT	ReleaseByOwner()		{ delete this; return 0; }
	int		GetKidnap()	{ return GetInt( ITEMDATA_KIDNAP )>0? 1 : 0; }
	bool	IsSoulMark()			{ return GetInt(ITEMDATA_KIDNAP)==2; }//goto �Ƿ��ӡ	
	void	SetKidnap( USHORT dat )		{ SetInt( ITEMDATA_KIDNAP, dat );	}

public: // ����
	bool	IsExpend()		{ return CItem::IsExpend(GetInt(ITEMDATA_TYPE)); } //����Ʒ
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
	static bool IsActionItem(DWORD itemType){return itemType/100000  == ITEMSORT_OTHER;}//20070825����
	bool	IsGem()			{ return GetItemSort() == ITEMSORT_GEM; }//20070111����
	bool	IsMedicine()	{ return CItem::GetItemSort() == ITEMSORT_EXPEND && GetItemType()!=35; }//goto �ų�Ԫ��Ʊ�;��鵤
	bool   IsPetEgg();//�ǲ��ǳ��ﵰ
public: // static
	static bool IsExpend(int nType)		{ return CItem::GetItemSort(nType) == ITEMSORT_EXPEND; }//����Ʒ
	//�ǲ��ǿɵ�����Ʒ
	static bool IsCountable(int nType);
	//06.11.7�������-------------------------------------------------
	static int GetDelayGroupIndex( OBJID idType );
	static bool	IsEquipment(int nType)	{ return  GetItemSort(nType) == IETMSORT_FINERY1 || GetItemSort(nType) == IETMSORT_FINERY2; }
public://����
	bool    IsEnableDress()			{return IsDress();}//  [4/5/2007] ���� ��װ
	bool    IsEnableMount()         {return IsMount(); }// ���� [�½���7/30/2007]
	bool	IsNewRemedy();
	static	bool    IsNewRemedyByItemType( int typeID );
	bool	IsEquipEnable()		{ return IsFinery1() || IsFinery2();}
	bool	IsEatEnable()		{ return IsMedicine(); }
	bool	IsPileEnable()		{ return IsCountable( GetInt( ITEMDATA_TYPE ) ) && GetInt(ITEMDATA_AMOUNTLIMIT) >= 1; }	//	20070627 BUG0000787 ��� ����Ϊamount_limit = 1 ʱ,����Ϊ�ɵ��ӵ���Ʒ,���Ե���
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

	bool IsActivatePickup()				{ return CItem::IsActivatePickup( GetInt( ITEMDATA_MONOPOLY ));		};			//  [2007-9-14] ���	��ü���
	bool IsActivateEquip()				{ return CItem::IsActivateEquip( GetInt( ITEMDATA_MONOPOLY ));		};//  [2007-9-14] ���	װ������
	bool IsLifeTimeContinue()			{ return CItem::IsLifeTimeContinue( GetInt( ITEMDATA_MONOPOLY ));	};//  [2007-9-14] ���	��������
	bool IsLifeTimeNonContinue()		{ return CItem::IsLifeTimeNonContinue( GetInt( ITEMDATA_MONOPOLY ));};//  [2007-9-14] ���	��������
	bool IsChanceDropEquipWhenDead()	{ return CItem::IsChanceDropEquipWhenDead(GetInt(ITEMDATA_MONOPOLY));}//  [1/27/2008 ���]	�����л��ʵ���װ��
	bool IsUnkidnap()					{ return CItem::IsUnkidnap(GetInt(ITEMDATA_MONOPOLY));}//  [1/27/2008 ���]	���ɰ�
	
	static	bool IsActivatePickup(unsigned int uMonopoly);	//  [2007-9-14] ���	��ü���
	static	bool IsActivateEquip(unsigned int uMonopoly);	//  [2007-9-14] ���	װ������
	static	bool IsLifeTimeContinue(unsigned int uMonopoly);		//  [2007-9-14] ���	��������
	static	bool IsLifeTimeNonContinue(unsigned int uMonopoly);		//  [2007-9-14] ���	��������
	static	bool IsChanceDropEquipWhenDead(unsigned int uMonopoly);	//  [1/27/2008 ���]	�����л��ʵ���װ��
	static	bool IsUnkidnap(unsigned int uMonopoly);	//  [1/27/2008 ���]	���ɰ�

	bool	IsNonsuchItem();
	int		GetEmbedCountLimit();
	int		GetEmbedGemCount();


	int		GetEmbedPos();
	bool	IsCanEmbedEquip();
	bool	IsHaveSameGemEmbed(OBJID idGemType);//[2010-07-19 goto] װ�����Ƿ��Ѿ���Ƕ�˴��౦ʯ
	bool	IsRightGemType(OBJID idGemType);//[2010-07-23 goto] Ҫ��Ƕ�ı�ʯ�����Ƿ���װ��ƥ��
public:	// get
	int		GetLife()			{ return GetInt( ITEMDATA_LIFE );		}
	OBJID	GetID()			{ return CItemData::GetID();}
	int		GetCapability()		{ return GetInt(ITEMDATA_CAPABILITY_INIT);}
	int		GetEquipQual()		{ return GetInt(ITEMDATA_QUALITY);}
	int		GetEquipStar()		{ return GetInt(ITEMDATA_STAR);}
	int		GetEquipAddLev()	{ return GetInt(ITEMDATA_ADDLEV);}
	//[2010-07-19 goto]װ��ǿ�������
	int		GetEquipAddLevFailRate()	{ return GetInt(ITEMDATA_ADDLEV_FAILRATE); }//��ȡװ��ǿ��ʧ�ܼӳɵĳɹ���,100%=10000
	void	SetEquipAddLevFailRate(int nData, bool bUpdate=false)	{ SetInt(ITEMDATA_ADDLEV_FAILRATE,nData,bUpdate); }

	int		GetEquipStarFailRate()	{ return GetInt(ITEMDATA_ADDSTAR_FAILRATE); }//��ȡװ������ʧ�ܼӳɵĳɹ���,100%=10000
	void	SetEquipStarFailRate(int nData, bool bUpdate=false)	{ SetInt(ITEMDATA_ADDSTAR_FAILRATE,nData,bUpdate); }

	void	SetEquipStar(int nData, bool bUpdate=false)		{ SetInt(ITEMDATA_STAR,nData,bUpdate); }//װ���ǵ�
	void	SetEquipAddLev(int nData, bool bUpdate=false)	{ SetInt(ITEMDATA_ADDLEV,nData,bUpdate); }//װ��ǿ���ȼ�
	int		GetNeedGemLev(int nEquipLev);//��ȡװ��ǿ���������Ҫ��ǿ����ʯ�ȼ�
	int		GetEquipAddLevCost(int nType);//��ȡװ��ǿ���������Ҫ������
	float	GetEquipAddRate(int nLev);//���װ��ǿ�������Ǻ�����Լӳ�ϵ��
	
	void	UpdateEquipBaseProperty(int nOldLev, int nType=0);//ǿ�������װ����������
	void	UpdateEquipAdditionProperty(int nOldLev);//���Ǻ����װ����������

	float	GetSucRate(short nType);//[2010-08-24 goto]��ȡװ��ǿ������ǵ������ĳɹ���
	int		GetWeaponSpecialView();

public: // const
	int		GetTypeID()			{ return GetInt( ITEMDATA_TYPE );}
	int		CalcRepairMoney();
	DWORD	GetSellPrice();
	int		GetItemAmount()					{	return GetInt( ITEMDATA_AMOUNT );		}// ��Ʒ������
	void	SetItemAmount( int nAmount )	{	SetInt( ITEMDATA_AMOUNT,nAmount );	}
	int		GetItemAmountLimit()			{	return GetInt( ITEMDATA_AMOUNTLIMIT );		}// ��Ʒ������

	int		GetChaosHoleState()				{ return GetInt(ITEMDATA_GEM4); }//���ĸ������״̬

public: // application
	CItem*	Split(int nNum, IDatabase* pDb);

public:
	int		GetItemSubType();

	int		GetLevel	(void);
	int		GetColour	(void)	{return GetInt(ITEMDATA_COLOR);}
	bool	ChangeType	(OBJID idNewType);
	bool	ChangeColor	(int nColor);

public: // ����
	int		GetItemSort()			{ return CItem::GetItemSort(GetInt(ITEMDATA_TYPE)); }
	int		GetItemType()			{ return CItem::GetItemType(GetInt(ITEMDATA_TYPE)); }
public:
	static int	GetItemSort(OBJID idType)			{ return idType/100000; } //ȡʮ��λ		
	static int	GetItemType(OBJID idType)			{ return idType/10000;	} //ȡʮ����λ		

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
	CItem*	Clone(IDatabase* pDb);		// ����Ʒ�����ݿ�
	bool m_isLocked;//[2010-06-28 goto]�Ƿ����������׵�ʱ��������Ʒ��������ʹ��

public:
	unsigned int GetLifeTime();
	const char* GetLastCheckTime();
	static BOOL IsLogSaveMoney(int nMoney);	//	20070710	���	��Ǯʹ���Ƿ��¼
	BOOL IsCostlyItem();	//	20070710 ��� �Ƿ�Ϊ������Ʒ
	int	GetReqLev();//�������ȼ�,���������������ж�
	int	GetReqJobLev();

	BOOL IsValidEquip(CUser* pUser = NULL);

	static BOOL CanRepair(DWORD itemType);
	BOOL CanRepair() {return CanRepair(GetTypeID());}//����


	static BOOL IsCanUpLevel(DWORD itemType);
	BOOL IsCanUpLevel() {return IsCanUpLevel(GetTypeID());}//����
	bool CanCutWork();//��ʯ�ܷ����
	static DWORD GetWeaponLevExp(WORD lev);
public:
	bool		Create			(IDatabase* pDb, 
		IRecord* pDefault, 
		const ItemInfoStruct* pInfo, 
		IRecord* pDefaultEudemonData, 
		IRecord* pDefaultWpSl,//���
		bool bInsert=true, 
		OBJID idNew=ID_NONE);
	bool		CreateSend			(IDatabase* pDb, 
		IRecord* pDefault, 
		const ItemInfoStructSend* pInfo, 
		IRecord* pDefaultWpSl,//���
		bool bInsert=true, 
		OBJID idNew=ID_NONE);
	bool		CreateSoul		(IDatabase* pDb,IRecord* pDefault,const ItemInfoStruct* pInfo,IRecord* pDefaultWpSl,ST_WEAPONSOULDATA stData,bool bInsert=true,OBJID idNew=ID_NONE);
	bool		Create			(OBJID idItem, IDatabase* pDb);
	bool		Create			(IRecordset* pRes, IDatabase* pDb);
	bool		GetInfo	(ItemInfoStructSend* pInfo);
	bool		GetInfo	(ItemInfoStruct* pInfo);
//WEAPENSOUL���
public:
	static BOOL	IsWeaponSoulExpItem(OBJID idType) { return idType / 10000 == 39;}
	BOOL		IsWeaponSoulExpItem(){ return IsWeaponSoulExpItem(GetTypeID());}//����
	int			GetItemAdditionData(int nIndex);//�����Ʒĳ������Ե�ֵ,������1��8

	BOOL		GetWpSlInfo(ST_WEAPONSOULDATA& stWpSl);
	void		EraseWpSl(IDatabase* pDb);//�������
	CWeaponSoulData* QueryWeapensoulData() { return m_pWpSlData;}
	DWORD		GetWpSlLev();
	DWORD		GetWpSlType();
//����&����ת�����--jay
public:
	short		GetEquipSoulLev();//�õ�����ȼ�
	short		GetEquipSoulType();//�õ���������
	DWORD		GetEquipSoulPoint();//�õ�ħ����
	void		SetEquipSoulLev(short nLev);//�õ�����ȼ�
	void		SetEquipSoulType(short nType);//�õ���������
	void		SetEquipSoulPoint(OBJID nPoint);//�õ�ħ����
	bool		IsActEquipment(){ return IsWeapon() || IsRing() || IsAdorn() || IsNecklace() || IsBangle();}//�ǹ����� --jay
	bool		IsDefEquipment(){ return IsArmor() || IsHelmet() || IsShoulder() || IsGlove() || IsCaestus() || IsShoes();}//�Ƿ�������Ʒ

	bool		IsSoulBook(){return GetItemType() == ITEMTYPE_SOULBOOK;}
	bool        IsActSoulBook();
	bool		IsDefSoulBook();
	short		GetSoulBookType();

protected:
	virtual void	OnSaveInfo();
	BOOL		CreateWpSlData(IDatabase* pDb);//�������
	BOOL		CreateWpSlData(const ST_WEAPONSOULDATA* pstWpSlData,IRecord* pDefaultWpSl,IDatabase* pDb,bool bInsert = true);
	void		DeletWpSlData();//��������������.����ʱ����
	CWeaponSoulData* m_pWpSlData;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_ITEM_H__CA7CE885_1DCF_4B96_B308_FB7F40CCF4BA__INCLUDED_)



