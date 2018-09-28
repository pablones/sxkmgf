// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Item.h"
#include "ItemType.h"
#include "MapGroup.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "User.h"
#include "MsgEudemonAttrib.h"
#include "MyArray.h"
//---jinggy---2004-11-12---Begin
#include "netmsg.h"
#include "AllMsg.h"
#include "Console.h"
//---jinggy---2004-11-12---End

MYHEAP_IMPLEMENTATION(CItem,s_heap)

//---tcp2005---9-17-add---begin
const _MAX_GEM_AMOUNT=13;
const unsigned int nGemType[_MAX_GEM_AMOUNT]= {0,
                                               1038000,
                                               1038010,
											   1038020,
											   1038030,
											   1038040,
											   1038050,
											   1038060,
											   1038070,
											   1038080,
											   1038090,
											   1038100,
											   1038110											   
};
const CMyArray<unsigned int,_MAX_GEM_AMOUNT>m_nGemTypeArray(nGemType,_MAX_GEM_AMOUNT);
//---tcp2005---9-17-add---end


//20070825修罗:外装附魔相关
const int _DRESSENCHANTMAXCOUNT = 3;//外装最大附魔数
const int _DRESSENCHANTGEMPOS[_DRESSENCHANTMAXCOUNT]={
	EM_GEMPOS_GEM1,
	EM_GEMPOS_GEM2,
	EM_GEMPOS_GEM3};//外装附魔GEM位

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItem::CItem()
{
	SetObjType(OBJ_ITEM);
	m_isLocked = false;
	m_pWpSlData = NULL;
}

//////////////////////////////////////////////////////////////////////
CItem::~CItem()
{
	DeletWpSlData();
}

//////////////////////////////////////////////////////////////////////
int	CItem::CalcRepairMoney()
{
	int nRecoverDurability = __max(0, GetInt(ITEMDATA_AMOUNTLIMIT) - GetInt(ITEMDATA_AMOUNT));
	if (nRecoverDurability == 0)
		return 0;

	// 修理价格=(持久上限-当前持久)/持久上限*商店出售价格
	double nRepairCost	= 0;
	if (GetInt(ITEMDATA_AMOUNTLIMIT) > 0)
		nRepairCost	= (double)GetInt(ITEMDATA_PRICE) * nRecoverDurability / GetInt(ITEMDATA_AMOUNTLIMIT)/* * 0.1*/;
	
	return __max(1, nRepairCost);
}

//////////////////////////////////////////////////////////////////////
DWORD CItem::GetSellPrice()
{
	if( GetInt(ITEMDATA_AMOUNTLIMIT) == 0 )
		return 0;
	int nAmount = this->GetInt(ITEMDATA_AMOUNT);
	if (nAmount > this->GetInt(ITEMDATA_AMOUNTLIMIT))
		nAmount = this->GetInt(ITEMDATA_AMOUNTLIMIT);

	DWORD dwPrice = GetInt(ITEMDATA_PRICEOUT);

	if( CItem::IsCountable( GetInt( ITEMDATA_TYPE ) ) )	//叠加物品
	{
		//dwPrice = dwPrice / this->GetInt( ITEMDATA_AMOUNT_INIT );
		DWORD dwTotal = dwPrice * nAmount;
		return dwTotal;
	}

	dwPrice = dwPrice;
	return dwPrice;
}

//////////////////////////////////////////////////////////////////////
CItem*	CItem::Split(int nNum, IDatabase* pDb)
{
	CHECKF(pDb);
	CHECKF(IsPileEnable());

	int nCurrNum = GetInt(ITEMDATA_AMOUNT);
	CHECKF(nNum > 0 && nNum < nCurrNum);

	int nStep = 0;
	DEBUG_TRY ////////
	CItemPtr pItem = Clone(pDb);
	CHECKF(pItem);
	nStep = 1;
	pItem->SetInt(ITEMDATA_BUYPRICE,this->GetInt(ITEMDATA_BUYPRICE));
	nStep = 2;
	pItem->SetInt(ITEMDATA_AMOUNT, nNum);
	nStep = 3;
	this->SetInt(ITEMDATA_AMOUNT, nCurrNum - nNum);

	return pItem;
	DEBUG_CATCH2("item split!! nstep = %u",nStep) //////////

	return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem*	CItem::Clone(IDatabase* pDb)		// 新物品存数据库
{
	CHECKF (pDb);
	CItemPtr pItem = CreateNew();
	IF_NOT(pItem)
		return NULL;

	ItemInfoStruct	info;
	memset(&info,0,sizeof(ItemInfoStruct));//20071115修罗:
	GetInfo(&info);
	info.id		= ID_NONE;		// new record


	IRecord* pWpSlRecord=NULL;
	if(IsWeaponSoul() && m_pWpSlData)
	{
		pWpSlRecord = m_pWpSlData->GetRecord();
	}

	if(pItem->Create(pDb, 
		m_pData->GetRecord(), 
		&info, 
		NULL,
		pWpSlRecord,
		true))
		return pItem;


	pItem->ReleaseByOwner();
	return NULL;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetItemSubType()
{
	// 取十万，万，千位
	return (GetInt(ITEMDATA_TYPE)%1000000)/1000;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetLevel(void)
{
	return /*GetInt( ITEMDATA_LEVEL )*/0;
}

//////////////////////////////////////////////////////////////////////
/*
 对表 cq_item 中的 type, 改变 type 的末尾就是改变装备的品质 ;
 通过 type 和 magic3 就可定位到  表 cq_itemaddition (不同品质及追加属性的装备对应的其他属性)
*/
bool CItem::ChangeType	(OBJID idNewType) //
{
	// type
	CItemTypeData *pType = ItemType()->QueryItemType(idNewType);
	if(!pType)
	{
		LOGERROR("ChangeType类型[%d]错误!", idNewType);

		return false;
	}
	
	m_pType = pType;
	this->SetInt(ITEMDATA_TYPE, idNewType);

//	m_pAddition = ItemAddition()->QueryItemAddition(idNewType, GetInt(ITEMDATA_ADDITION));

	return true;
}


//////////////////////////////////////////////////////////////////////
bool CItem::IsNonsuchItem(void)
{
	DWORD dwType = GetTypeID();
	// precious gem
	if (this->IsGem() && (dwType%10) >= 2)
		return true;

	// other type
	if (CItem::GetItemSort(dwType) == ITEMSORT_OTHER || CItem::GetItemSort(dwType) >= ITEMSORT_EXPEND)
		return false;

	// ....
	int nGem1 = this->GetInt(ITEMDATA_GEM1)%10;
	int nGem2 = this->GetInt(ITEMDATA_GEM2)%10;

	bool bIsNonsuch = false;
	if(IsWeapon())
	{
		if ((nGem1 != GEM_HOLE && nGem1 >= 2)
				|| (nGem2 != GEM_HOLE && nGem2 >= 2))
			bIsNonsuch = true;
	}
	else if(IsFinery1())
	{
		if (nGem1 != GEM_NONE || nGem2 != GEM_NONE)
			bIsNonsuch = true;
	}

	return bIsNonsuch;
}

//////////////////////////////////////////////////////////////////////
bool CItem::ChangeColor(int nColor)
{
	if (!(nColor >=3 && nColor <= 9))
		return false;

	if(IsHelmet() || IsArmor())
	{
		DWORD idType = this->GetInt(ITEMDATA_TYPE);
		DWORD idNewType = idType-((idType/100)%10)*100+nColor*100;
		return this->ChangeType(idNewType);
	}

	return false;
}



//武器打一个孔..
bool CItem::CreateHole()
{
	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
	{
		if (GetGemStatus(i)==_NOT_HOLE)
		{
			SetGemStatus( i,_HOLE ); 
			return true;
		}
	}
	return false;
}
//获得装备强化或升星后的属性加成系数
float CItem::GetEquipAddRate(int nLev)
{
	float nRate = 0;
	switch(nLev)
	{
	case 1:	nRate=1.1f;	break;
	case 2:	nRate=1.2f;	break;
	case 3:	nRate=1.3f;	break;
	case 4:	nRate=1.4f;	break;
	case 5:	nRate=1.55f;	break;
	case 6:	nRate=1.7f;	break;
	case 7:	nRate=1.85f;	break;
	case 8:	nRate=2.05f;	break;
	case 9:	nRate=2.25f;	break;
	case 10:nRate=2.6f;	break;
	}
	return nRate;
}
//[2010-07-22 goto]强化后更新装备基础属性
//goto 新加参数nType表示是强化还是魂印，0=强化，1=魂印
void CItem::UpdateEquipBaseProperty(int nOldLev, int nType/*=0*/)
{
	int nLev = GetEquipAddLev();
	if (nLev == nOldLev && nType==0)//强化等级无变化时不需要更新强化基础属性
		return;
	if (nLev==0 && !IsSoulMark())
		return;
	float nRate = GetEquipAddRate(nLev);
	float nLastRate=1;
	if (nLev>1)//如果是1级以上的要先还原成0级的属性再乘以系数
	{
		nLastRate = GetEquipAddRate(nOldLev);
	}
	if (IsSoulMark())//goto 魂印 +10%
	{
		nRate += 0.1f;
		if (nRate==0.1f)
			nRate=1.1f;
		if (nType!=1)//如果不是魂印后更新属性，必须还原成魂印前的属性
			nLastRate += 0.1f;
	}
	int nSort1 = GetInt( ITEMDATA_BASE1 ) / 10000;
	int nSort2 = GetInt( ITEMDATA_BASE2 ) / 10000;
	int nOldBase1 = GetInt( ITEMDATA_BASE1 )%10000*1.0/nLastRate+0.5;//四舍五入，减小误差
	int nOldBase2 = GetInt( ITEMDATA_BASE2 )%10000*1.0/nLastRate+0.5;

	SetInt(ITEMDATA_BASE1, nSort1*10000+nOldBase1*nRate+0.5);
	SetInt(ITEMDATA_BASE2, nSort2*10000+nOldBase2*nRate+0.5);
}
//[2010-07-22 goto]升星后更新装备附加属性
void CItem::UpdateEquipAdditionProperty(int nOldLev)
{
	int nLev = GetEquipStar();
	if (nLev == nOldLev)
		return;
	if (nLev==0)
		return;
	float nRate = GetEquipAddRate(nLev);
	float nLastRate=1;
	if (nLev>1)//如果是1级以上的要先还原成0级的属性再乘以系数
	{
		nLastRate = GetEquipAddRate(nOldLev);
	}
	int nSort1 = GetInt( ITEMDATA_BASIC_ADDITION1 ) / 10000;
	int nSort2 = GetInt( ITEMDATA_BASIC_ADDITION2 ) / 10000;
	int nSort3 = GetInt( ITEMDATA_BASIC_ADDITION3 ) / 10000;
	int nSort4 = GetInt( ITEMDATA_BASIC_ADDITION4 ) / 10000;
	int nSort5 = GetInt( ITEMDATA_BASIC_ADDITION5 ) / 10000;
	int nSort6 = GetInt( ITEMDATA_BASIC_ADDITION6 ) / 10000;
	int nSort7 = GetInt( ITEMDATA_BASIC_ADDITION7 ) / 10000;
	int nSort8 = GetInt( ITEMDATA_BASIC_ADDITION8 ) / 10000;
	//还原成0级时的属性
	int nOldAddition1 = GetInt( ITEMDATA_BASIC_ADDITION1 )%10000*1.0/nLastRate+0.5;//四舍五入，减小误差
	int nOldAddition2 = GetInt( ITEMDATA_BASIC_ADDITION2 )%10000*1.0/nLastRate+0.5;
	int nOldAddition3 = GetInt( ITEMDATA_BASIC_ADDITION3 )%10000*1.0/nLastRate+0.5;
	int nOldAddition4 = GetInt( ITEMDATA_BASIC_ADDITION4 )%10000*1.0/nLastRate+0.5;
	int nOldAddition5 = GetInt( ITEMDATA_BASIC_ADDITION5 )%10000*1.0/nLastRate+0.5;
	int nOldAddition6 = GetInt( ITEMDATA_BASIC_ADDITION6 )%10000*1.0/nLastRate+0.5;
	int nOldAddition7 = GetInt( ITEMDATA_BASIC_ADDITION7 )%10000*1.0/nLastRate+0.5;
	int nOldAddition8 = GetInt( ITEMDATA_BASIC_ADDITION8 )%10000*1.0/nLastRate+0.5;
	//更新为新等级属性
	SetInt(ITEMDATA_BASIC_ADDITION1, nSort1*10000+nOldAddition1*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION2, nSort2*10000+nOldAddition2*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION3, nSort3*10000+nOldAddition3*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION4, nSort4*10000+nOldAddition4*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION5, nSort5*10000+nOldAddition5*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION6, nSort6*10000+nOldAddition6*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION7, nSort7*10000+nOldAddition7*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION8, nSort8*10000+nOldAddition8*nRate+0.5);
}

OBJID CItem::GetGemStatus( int nPos )
{
	CHECKF( nPos >=EM_GEMPOS_BEGIN && nPos <= EM_GEMPOS_END );

	OBJID nResult = 0;
	switch( nPos )
	{
	case EM_GEMPOS_GEM1:
		nResult = GetInt( ITEMDATA_GEM1 );
		break;
	case EM_GEMPOS_GEM2:
		nResult = GetInt( ITEMDATA_GEM2 );
		break;
	case EM_GEMPOS_GEM3:
		nResult = GetInt( ITEMDATA_GEM3 );
		break;
	default:
		break;
	}
	return nResult;
}
OBJID CItem::GetAddStatus( int nPos )
{
	CHECKF( nPos >=EM_ADDPOS_BEGIN && nPos <= EM_ADDPOS_END );
	
	OBJID nResult = 0;
	switch( nPos )
	{
	case EM_ADDPOS_GEM1:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION1 );
		break;
	case EM_ADDPOS_GEM2:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION2 );
		break;
	case EM_ADDPOS_GEM3:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION3 );
		break;
	case EM_ADDPOS_GEM4:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION4 );
		break;
	case EM_ADDPOS_GEM5:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION5 );
		break;
	case EM_ADDPOS_GEM6:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION6 );
		break;
	case EM_ADDPOS_GEM7:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION7 );
		break;
	case EM_ADDPOS_GEM8:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION8 );
		break;
	default:
		break;
	}
	return nResult;
}
OBJID CItem::GetBasicStatus( int nPos )
{
	CHECKF( nPos >=EM_BASEPOS_BEGIN && nPos <= EM_BASEPOS_END );
	
	OBJID nResult = 0;
	switch( nPos )
	{
	case EM_BASEPOS_GEM1:
		nResult = GetInt( ITEMDATA_BASE1 );
		break;
	case EM_BASEPOS_GEM2:
		nResult = GetInt( ITEMDATA_BASE2 );
		break;
	default:
		break;
	}
	return nResult;
}

//设置武器的某一个孔的状态
//20070105修罗:新增两个宝石位.用7,8
//07.1.8王玉波修改
//20070825修罗:用enum
void CItem::SetGemStatus( unsigned char ucPos,OBJID idType,bool bUpdate )// bupdate默认值为false
{

	CHECK( ucPos >=EM_GEMPOS_BEGIN && ucPos <= EM_GEMPOS_END );

	switch( ucPos )
	{
	case EM_GEMPOS_GEM1:SetInt( ITEMDATA_GEM1,idType,bUpdate );break;
	case EM_GEMPOS_GEM2:SetInt( ITEMDATA_GEM2,idType,bUpdate );break;
	case EM_GEMPOS_GEM3:SetInt( ITEMDATA_GEM3,idType,bUpdate );break;
	default:
		{
			LogSave( "Invalid Pos :%d in SetGemStatus()",ucPos );
			return;
		}
		break;
	}

	//20070312修罗:原update忘写
	if(bUpdate)
	{
		this->SaveInfo();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//Time:06.8.17
//Author:王玉波修改
//静态方法
//return true:表示此物品可以叠加,false:不可以
////////////////////////////////////////////////////////////////////////////////////////
bool CItem::IsCountable( int nType )
{
	DWORD dwType = nType / 100000;
	if (dwType == 1 || dwType == 2)
	{
		return false;
	}

	if(nType / 1000 == 301 || nType / 1000 == 311 || nType / 1000 == 321 || nType == _SPCITEM_VASEXCHANGE || nType == _SPCITEM_EXPEXCHANGE || nType == _SPCITEM_MONEYFORSHOP)
	{
		return false;
	}
	
	CItemTypeData* pType =ItemType()->QueryItemType(nType);
	if (!pType) 
	{
		return false;
	}
	int iLimit = pType->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
	if (iLimit > 1 && iLimit <= 10000)
	{
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////
//06.11.7
//王玉波
//函数说明:静态方法通过物品的类型,得到这组物品CD在MAP中的索引
//06.11.17王玉波修改,为了在客户端区分技能的CD,为索引加一个偏移量
int CItem::GetDelayGroupIndex( OBJID idType )
{
	const int nRetOffset = 60000;
	int nFormatValue = idType / 10000;
	int nSort = idType/100000;
	if(IsActionItem(idType))//20070825修罗:action物品统一cd组
		return 1 + nRetOffset;
	else return -1;//无效值
}

//---------------------------------------------
bool CItem::IsCanEmbedEquip()
{
	if(!IsWeaponSoul())
	{
		return IsCanUpLevel();
	}
	return true;
}
//[2010-07-21 goto]获取装备上第一个空的孔位置
int	CItem::GetEmbedPos()
{
	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
	{
		int nStatus = GetGemStatus(i);
		if (nStatus==_HOLE)
			return i;
	}
	return 0;
}
//[2010-07-19 goto] 装备中是否已经镶嵌了此类宝石
bool CItem::IsHaveSameGemEmbed(OBJID idGemType)
{
// 	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
// 	{
// 		int nStatus = GetGemStatus(i);
// 		if (nStatus/100==idGemType/100)
// 			return true;
// 	}
	DEBUG_TRY
	int lastGem=ItemType()->QueryItemType(idGemType)->GetInt(ITEMTYPEDATA_BASE1_INIT)/10000;
	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
	{
		int nStatus = GetGemStatus(i);
		if(nStatus == 0 || nStatus==_NOT_HOLE || nStatus == _HOLE || CItem::GetItemSort(nStatus)!=4)
			continue;
		int firstGem=ItemType()->QueryItemType(nStatus)->GetInt(ITEMTYPEDATA_BASE1_INIT)/10000;

		if (firstGem==lastGem)
			return true;
		if ( (lastGem>=1 && lastGem<=2) && (firstGem>=1 && firstGem<=2) )
			return true;
		if ( (lastGem>=20 && lastGem<=23) && (firstGem>=20 && firstGem<=23) )
			return true;
		if ( (lastGem>=3 && lastGem<=4) && (firstGem>=3 && firstGem<=4) )
			return true;
		if ( (lastGem>=24 && lastGem<=27) && (firstGem>=24 && firstGem<=27) )
			return true;
		if ( (lastGem>=9 && lastGem<=13) && (firstGem>=9 && firstGem<=13) )
			return true;
	}
	DEBUG_CATCH("bool CItem::IsHaveSameGemEmbed");
	return false;
}
//要镶嵌的宝石类型是否与装备匹配
bool CItem::IsRightGemType(OBJID idGemType)
{
	int nType = GetItemType();
	if (nType==ITEMTYPE_NECKLACE
		|| nType==ITEMTYPE_WEAPONSOUL)//项链上两种宝石都可以镶嵌
		return true;
	
	if (idGemType/10000==41)//攻击类宝石
	{
		if (nType==ITEMTYPE_RING		//戒指
			|| nType==IETMTYPE_ADORN	//饰品
			|| nType==ITEMTYPE_BANGLE	//手镯
			|| nType==ITEMTYPE_WEAPONR)	//武器
			return true;
	}
	else if(idGemType/10000==42)//防御类宝石
	{
		if (nType==ITEMTYPE_SHOULDER	//护肩
			|| nType==ITEMTYPE_ARMOR	//盔甲
			|| nType==ITEMTYPE_HELMET	//头盔
			|| nType==ITEMTYPE_GLOVE	//手套
			|| nType==ITEMTYPE_CAESTUS	//腰带
			|| nType==ITEMTYPE_SHOES)	//鞋子
			return true;
	}
	return false;
}

int CItem::GetEudemonNeedExp(int nlevel)
{
	CLevupexpData* pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_EUDEMON * _EXP_TYPE + nlevel );
	if ( !pLevupexp )
	{
		return 0;
	}
	return pLevupexp->GetInt( LEVUPEXPDATA_EXP );//得到升级所需要经验
}


int	CItem::GetReqLev()//获得需求等级,可升级武器特殊判断
{	
	int reqLev = GetInt(ITEMDATA_REQ_USERLEVEL);
	return reqLev;
}

int	CItem::GetReqJobLev()//获得需求等级,可升级武器特殊判断
{	
	int reqLev = GetInt(ITEMDATA_REQ_JOBLEVEL);
	return reqLev;
}

//------------------------------------
DWORD CItem::GetWeaponLevExp(WORD lev)
{
	CHECKF(lev > 0 && lev <= 150);
	CLevupexpData* pLevupexp = LevupexpSet()->GetObj( EXP_TYPE_WEAPONSOUL * _EXP_TYPE + lev);
	if ( !pLevupexp )
	{
		return 0;
	}

	return pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
}

//--------------------------------
//20070704修罗:可修理
BOOL CItem::CanRepair(DWORD itemType)
{
	int nItemSort = GetItemSort(itemType);//前一位


	if(nItemSort == IETMSORT_FINERY1 || nItemSort == IETMSORT_FINERY2)
		return true;

	return false;
}


//	20070710 朱斌 是否为贵重物品
//goto 修改贵重物品判定规则
BOOL CItem::IsCostlyItem()
{
	if ((GetInt(ITEMDATA_MONOPOLY) & COSTLY_ITEM) != 0)
		return TRUE;
	else if (IsEquipment())
	{
		if (GetEquipAddLev()>4 || GetEquipStar()>4)//强化或星级+5
			return TRUE;
		if (GetEquipSoulLev()>2)
			return TRUE;
		for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)//镶有3级宝石
		{
			int nStatus = GetGemStatus(i);
			if(nStatus == 0 || nStatus==_NOT_HOLE || nStatus == _HOLE || CItem::GetItemSort(nStatus)!=4)
				continue;
			if (nStatus%100>2)
				return TRUE;
		}
	}
	return FALSE;
}

//	20070710	朱斌	金钱使用是否记录
BOOL CItem::IsLogSaveMoney(int nMoney)
{
	if(nMoney >= LOG_SAVE_MONEY)
		return TRUE;

	return FALSE;
}
//goto 屏蔽此函数,不以物品类型来判断
// BOOL CItem::IsCostlyItem(int nItemTypeID)
// {
// 	int nTypeID = nItemTypeID;
// 	int	nFirst = nTypeID / 100000;	//	前一位;
// 	int nFirstSecond = nTypeID / 10000;	//	前二位;
// 	int	nFirstToForth = nTypeID / 100;	//	前4位;
// 	int	nEnd = nTypeID - (nTypeID / 10) * 10;	//	个位
// 	
// 	//	1开头物品中，结尾是3的物品	完美装备
// 	if(nFirst == 1 && nEnd == 3)	
// 		return TRUE;
// 	
// 	//	4开头物品中，结尾是3的物品	完美武器
// 	else if(nFirst == 4 && nEnd == 3)
// 		return TRUE;
// 	
// 	//	500001	强化石,500002	精致强化石,511202	生命树叶 （增值道具要出售的材料）
// 	//	511301	金属齿轮（增值道具要出售的材料）,511302	能源石（增值道具要出售的材料）
// 	//	704006	包裹,704007	布匹,704008	名牌,704000	时空之泪碎片
// 	else if(nTypeID == 500001 || nTypeID == 500002 || nTypeID == 511202 || nTypeID == 511301 || 
// 		nTypeID == 511302 || nTypeID == 704006 || nTypeID == 704007 || nTypeID == 704008
// 		|| nTypeID == 704000)
// 		return TRUE;
// 
// 	//	5001开头的所有物品	升级宝石
// 	else if(nFirstToForth == 5001)
// 		return TRUE;
// 
// 	//	所有81-86开头的物品中，结尾是5的物品	5级星石
// 	else if(nFirstSecond >= 81 && nFirstSecond <= 86)
// 		return TRUE;
// 
// 	//	所有9开头物品	增值道具
// 	else if(nFirst == 9)
// 		return TRUE;
// 
// 	//20071121修罗:策划案----------
// 	else if(nFirst == 4 && nEnd == 4)//觉醒武器
// 		return TRUE;
// 
// 	else if(nFirstToForth == 5114)//晶原石
// 		return TRUE;
// 
// 	else if(nFirstSecond == 54)//武器觉醒需求的材料
// 		return TRUE;
// 	//------------------------------
// 	else if(nFirstSecond == 21)	//  [1/5/2008 朱斌] 泪灵
// 		return TRUE;
// 
// 	return FALSE;
// }

bool CItem::IsUserExclusive(unsigned int	uMonopoly)
{
	return (uMonopoly & USER_EXCLUSIVE_MASK ) != 0; 
}
//----------------------装备上孔的数量
int	CItem::GetEmbedCountLimit()
{
	int nHoleCount = 0;

	if (GetInt(ITEMDATA_GEM1)!=_NOT_HOLE && GetInt(ITEMDATA_GEM1)!=0)
		nHoleCount++;
	if (GetInt(ITEMDATA_GEM2)!=_NOT_HOLE && GetInt(ITEMDATA_GEM2)!=0)
		nHoleCount++;
	if (GetInt(ITEMDATA_GEM3)!=_NOT_HOLE && GetInt(ITEMDATA_GEM3)!=0)
		nHoleCount++;

	return nHoleCount;
}
//----------------------装备上已镶嵌宝石的数量
int	CItem::GetEmbedGemCount()
{
	int nGemCount = 0;
	
	for (int i=EM_GEMPOS_BEGIN; i<EM_GEMPOS_END; i++)
	{
		int nState = GetGemStatus(i);
		if (nState!=_NOT_HOLE && nState!=_HOLE && nState!=0)
			nGemCount++;
	}
	return nGemCount;
}
//----------------------
BOOL CItem::IsCanUpLevel(DWORD itemType)
{
	//[2010-07-19 goto]
	return  ( (GetItemSort(itemType) == IETMSORT_FINERY1 || GetItemSort(itemType) == IETMSORT_FINERY2)
			&& GetItemType(itemType)!=ITEMTYPE_MOUNT//坐骑不能强化
			&& GetItemType(itemType)!=ITEMTYPE_DRESS//时装不能强化
			&& GetItemType(itemType)!=ITEMTYPE_TREASURE
			&& GetItemType(itemType)!=ITEMTYPE_WEAPONSOUL);//武魂不能强化
// 	if( 4 == itemType / 100000 && itemType % 10 != 9 ) 
// 		return true; 
// 	else
//		return false;
}
//[2010-07-19 goto]获取装备强化或加星需要的强化宝石等级
int CItem::GetNeedGemLev(int nEquipLev)
{
	int nNeedLev = 0;
	switch(nEquipLev)
	{
	case 0:
	case 1:
	case 2:
		nNeedLev = 1;
		break;
	case 3:
	case 4:
	case 5:
		nNeedLev = 2;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
		nNeedLev = 3;
		break;
// 	case 9://策划要求去掉4级符，最高3级
// 		nNeedLev = 4;
// 		break;
	default:
		break;
	}
	return nNeedLev;
}
//[2010-08-24 goto]获取装备强化或加星单个符的成功率
float CItem::GetSucRate(short nType)
{
	int nLev = 0;
	float fSucRate = 0.0f;
	if (nType==1)//强化
		nLev = GetEquipAddLev();
	else if (nType==2)
		nLev = GetEquipStar();
	switch(nLev)
	{
	case 0:fSucRate=0.25f;break;
	case 1:fSucRate=0.21f;break;
	case 2:fSucRate=0.1725f;break;
	case 3:fSucRate=0.1375f;break;
	case 4:fSucRate=0.1f;break;
	case 5:fSucRate=0.075f;break;
	case 6:fSucRate=0.05f;break;
	case 7:fSucRate=0.025f;break;
	case 8:fSucRate=0.005f;break;
	case 9:fSucRate=0.0025f;break;
	default:
		break;
	}
	return fSucRate*100;
}
//[2010-07-19 goto]获取装备强化或加星需要的银两
int CItem::GetEquipAddLevCost(int nType)
{
	int nCost = 0;
	int nSwitchLev = 0;
	if (nType==1)
	{
		nSwitchLev = GetEquipAddLev();
		switch(nSwitchLev)
		{
		case 0:
		case 1:
		case 2:
			nCost=80;
			break;
		case 3:
		case 4:
		case 5:
			nCost=150;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
			nCost=250;
			break;
		default:
			break;
		}
	}
	else if (nType==2)
	{
		nSwitchLev = GetEquipStar();
		switch(nSwitchLev)
		{
		case 0:
		case 1:
		case 2:
			nCost=50;
			break;
		case 3:
		case 4:
		case 5:
			nCost=100;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
			nCost=150;
			break;
		default:
			break;
		}
	}
	return nCost;
}

//-----------------------------------------------
BOOL CItem::IsValidEquip(CUser* pUser)
{
	if(IsActivateEquip() || IsActivatePickup())
	{
		if(GetInt(ITEMDATA_NEEDCHECK) == 2)
			return FALSE;
	}
	if(IsEquipEnable())
	{
		if(IsWeaponSoul())
		{
			CWeaponSoulData* pData = QueryWeapensoulData();
			if(pData)
			{
				if(pData->GetInt(WEAPONSOULDATA_LIFESPAN) >= 5)
					return true;
			}
			return false;
		}

		//持久
		if(GetItemAmount()/100==0) 
			return false;

		if(pUser)
		{
			//人物等级
			if(GetReqLev() > pUser->GetLev())
				return false;
			//职业等级
			if(GetReqJobLev() > pUser->GetJobLev())
				return false;
			
// 			if (GetInt(ITEMDATA_REQ_SEX) != pUser->GetSex())
// 				return false;
			
			if (GetInt(ITEMDATA_REQ_PROF) && ((GetInt(ITEMDATA_REQ_PROF) & (pUser->GetProfession())) == 0))
				return false;			
		}

		return true;
	}
	else 
		return false;
}


//////////////////////////////////////////////////////////////////////////
//  [2007-9-14] 朱斌
bool CItem::IsActivatePickup(unsigned int uMonopoly)	//  [2007-9-14] 朱斌	获得激活
{
	return (uMonopoly & ACTIVATE_PICKUP ) != 0;
}

//  [2007-9-14] 朱斌	装备激活
bool CItem::IsActivateEquip(unsigned int uMonopoly)
{
	return (uMonopoly & ACTIVATE_EQUIP ) != 0;
}

//  [2007-9-14] 朱斌	可以续费
bool CItem::IsLifeTimeContinue(unsigned int uMonopoly)
{
	return (uMonopoly & LIFETIME_CONTINUE ) != 0;
}

//  [2007-9-14] 朱斌	不能续费
bool CItem::IsLifeTimeNonContinue(unsigned int uMonopoly)
{
	return (uMonopoly & LIFETIME_NON_CONTINUE ) != 0;
}

//  [2007-9-15] 朱斌 得上次检查时间,如果说为空则未激活
const char* CItem::GetLastCheckTime()
{
	return GetStr(ITEMDATA_LAST_CHECK_TIME);
}

//  [2007-9-15] 朱斌 得剩余生命周期,单位秒
unsigned int CItem::GetLifeTime()
{
	return GetInt(ITEMDATA_LIFE_TIME);
}


//-----------------------------------
//通过内存创建.并与数据库关联
bool CItem::Create(IDatabase* pDb, 
		IRecord* pDefault, 
		const ItemInfoStruct* pInfo, 
		IRecord* pDefaultEudemonData, 
		IRecord* pDefaultWpSl,//武魂 
		bool bInsert/*=true*/, 
		OBJID idNew/*=ID_NONE*/)
{
	CHECKF(CItemData::Create(pDb, 
		pDefault, 
		pInfo, 
		pDefaultEudemonData, 
		bInsert, 
		idNew));

	if(IsWeaponSoul())
	{
		ST_WEAPONSOULDATA data;
		CWeaponSoulData::GetInfoByDefault(data);
		data.idItem = GetID();
		CreateWpSlData(&data,pDefaultWpSl,pDb,bInsert);
	}

	return true;
}

bool CItem::CreateSend(IDatabase* pDb, 
				   IRecord* pDefault, 
				   const ItemInfoStructSend* pInfo, 
				   IRecord* pDefaultWpSl,//武魂 
				   bool bInsert/*=true*/, 
				   OBJID idNew/*=ID_NONE*/)
{
	CHECKF(CItemData::CreateSend(pDb, 
		pDefault, 
		pInfo, 
		NULL, 
		bInsert, 
		idNew));
	
	if(IsWeaponSoul())
	{
		CreateWpSlData(&(pInfo->stWeapenSoul),pDefaultWpSl,pDb,bInsert);
	}
	SetEquipSoulPoint(pInfo->nEquipSoulPoint);
	SetEquipSoulLev(pInfo->usEquipSoulLev);
	SetEquipSoulType(pInfo->usEquipSoulType);

	return true;
}

bool CItem::CreateSoul(IDatabase* pDb,
				   IRecord* pDefault,
				   const ItemInfoStruct* pInfo,
				   IRecord* pDefaultWpSl,
				   ST_WEAPONSOULDATA stData,
				   bool bInsert,
				   OBJID idNew)
{
	CHECKF(CItemData::Create(pDb, 
		pDefault, 
		pInfo, 
		NULL, 
		bInsert, 
		idNew));

	if(!IsWeaponSoul())
	{
		DeleteRecord(pDb);
		CHECKF(!"if(!IsWeaponSoul())");
	}

	stData.idItem = GetID();
	if(!CreateWpSlData(&stData,pDefaultWpSl,pDb,bInsert))
	{
		DeleteRecord(pDb);
		CHECKF(!"CreateWpSlData(&stData,pDefaultWpSl,pDb,bInsert)");
	}
	return true;
}

//通过数据库创建
bool CItem::Create(OBJID idItem, IDatabase* pDb)
{
	//调用CItemData的同名函数
	CHECKF(CItemData::Create(idItem,pDb));
	CreateWpSlData(pDb);
	return true;
}

bool CItem::Create(IRecordset* pRes, IDatabase* pDb)
{
	//调用CItemData的同名函数
	CHECKF(CItemData::Create(pRes,pDb));
	CHECKF(CreateWpSlData(pDb));
	return true;
}

//-------------------------------
BOOL CItem::CreateWpSlData(IDatabase* pDb)//加载武魂
{
	CHECKF(pDb);

	if( !IsWeaponSoul())//非武魂宿体或容器.不创建
		return true;

	OBJID idSoul = GetInt(ITEMDATA_BASE1);
	if(idSoul == ID_NONE)
	{
		LOGWARNING("CItem::CreateWpSlData(IDatabase* pDb) ITEMID = %u",GetID());
		return false;
	}

	//先清除原有的武魂数据
	DeletWpSlData();

	m_pWpSlData = new CWeaponSoulData;
	CHECKF(m_pWpSlData);
	IF_NOT(m_pWpSlData->Create(idSoul,pDb))
	{
		//创建失败
		SAFE_RELEASE(m_pWpSlData);
	}

	return true;
}

//-------------------------------
BOOL CItem::CreateWpSlData(const ST_WEAPONSOULDATA* pstWpSlData,IRecord* pDefaultWpSl,IDatabase* pDb,bool bInsert /*= true*/)
{
	CHECKF(pstWpSlData);
	CHECKF(pDefaultWpSl);
	CHECKF(pDb);
	CHECKF(IsWeaponSoul());//非武魂宿体或容器.不创建		

	//先清除以前的
	DeletWpSlData();

	m_pWpSlData = new CWeaponSoulData;
	CHECKF(m_pWpSlData);
	IF_NOT(m_pWpSlData->Create(pDb,
			pDefaultWpSl,
			pstWpSlData,
			bInsert,
			ID_NONE))
		{
			//创建失败
			SAFE_RELEASE(m_pWpSlData);
			return false;
		}

	this->SetInt(ITEMDATA_BASE1,m_pWpSlData->GetID(),true);//武魂ID
	return true;
}

//------------------------------------
//包装一层.内调用CItemData同名函数
bool CItem::GetInfo	(ItemInfoStructSend* pInfo)
{
	CItemData::GetInfo(pInfo); 
	if(IsWeaponSoul())
	{
		m_pWpSlData->GetInfo(pInfo->stWeapenSoul,false);
	}
	pInfo->usEquipSoulLev=GetEquipSoulLev();
	pInfo->usEquipSoulType=GetEquipSoulType();
	pInfo->nEquipSoulPoint=GetEquipSoulPoint();
	return true;
}


bool CItem::GetInfo	(ItemInfoStruct* pInfo)
{
	CItemData::GetInfo(pInfo); 
	pInfo->usEquipSoulLev=GetEquipSoulLev();
	pInfo->usEquipSoulType=GetEquipSoulType();
	pInfo->nEquipSoulPoint=GetEquipSoulPoint();
	return true;
}


//------------------------------------
void CItem::DeletWpSlData()//清除所有武魂数据.析构时调用
{
	SAFE_RELEASE(m_pWpSlData);
}

void CItem::EraseWpSl(IDatabase* pDb)
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		m_pWpSlData->DeleteRecord(pDb);
		DeletWpSlData();
	}
}

BOOL CItem::GetWpSlInfo(ST_WEAPONSOULDATA& stWpSl)
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		return m_pWpSlData->GetInfo(stWpSl);
	}
	return false;
}

DWORD CItem::GetWpSlLev()
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		return m_pWpSlData->GetInt(WEAPONSOULDATA_MIXLEV);
	}
	return 0;
}

DWORD CItem::GetWpSlType()
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		return m_pWpSlData->GetInt(WEAPONSOULDATA_TYPE) / 10;
	}
	return 0;
}


void CItem::OnSaveInfo()
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		m_pWpSlData->SaveInfo();
	}
}

bool CItem::IsChanceDropEquipWhenDead(unsigned int uMonopoly)
{
	return (uMonopoly & CHANCE_DROP_EQUIP_WHEN_DEAD ) != 0;
}

bool CItem::IsUnkidnap(unsigned int uMonopoly)
{
	return (uMonopoly & UNKIDNAP) != 0;
}

int	CItem::GetWeaponSpecialView()
{
	if (GetItemType() != ITEMTYPE_WEAPONR)
		return 0;

	int nViewLev = 0;
	if (GetEquipAddLev()>6)
		nViewLev += 2;
// 	if (GetEquipStar()>6)
// 		nViewLev++;
	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
	{
		int nStatus = GetGemStatus(i);
		if(nStatus == 0 || nStatus==_NOT_HOLE || nStatus == _HOLE || CItem::GetItemSort(nStatus)!=4)
			continue;
		if (nStatus%100 > 3)
		{
			nViewLev++;
			break;
		}
	}
	return nViewLev;
}

bool CItem::IsNewRemedy()
{
	return CItem::IsNewRemedyByItemType(GetTypeID());
}

bool CItem::IsNewRemedyByItemType( int typeID )
{
	if((301 == typeID/1000)
		|| (311 == typeID/1000)
		|| (321 == typeID/1000)
		|| (302 == typeID/1000)
		|| (312 == typeID/1000)
		|| (322 == typeID/1000))
		return true;
	else
		return false;
}

int	CItem::GetItemAdditionData(int nIndex)//获得物品某项附加属性的值,参数从1到8
{
	nIndex = nIndex + ITEMDATA_BASIC_ADDITION1 - 1;
	if (nIndex>=ITEMDATA_BASIC_ADDITION1 && nIndex<=ITEMDATA_BASIC_ADDITION8)
	{
		return GetInt((ITEMDATA)nIndex);
	}
	return 0;
}

bool CItem::CanCutWork()//宝石能否雕琢
{
	if(GetTypeID()/100 < 4105 || GetTypeID()/100 > 4108)//只有红水晶、绿水晶、黄水晶、蓝水晶可以雕琢
		return false;
	int nLev = GetTypeID()%100;
	if(nLev<=10)
		return true;

	return false;
}
//是不是宠物蛋
bool CItem::IsPetEgg()
{
	OBJID typeID = GetTypeID();
	if(typeID > 505000 && typeID < 506000)
		return true;

	return false;
}
//铸灵相关
short CItem::GetEquipSoulLev()//得到铸灵等级
{
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	if (nSoulState == 0 || nSoulState >= 1000000000)//大于10位为错误
		return 0;
	short nState=nSoulState/10000000;
	short nLev=(nSoulState-nState*10000000)/100000;
	return max(0,nLev);
}
short CItem::GetEquipSoulType()//得到铸灵属性
{
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	if (nSoulState == 0|| nSoulState >= 1000000000)//大于10位为错误
		return 0;
	short nState=nSoulState/10000000;
	return max(0,nState);
}
DWORD CItem::GetEquipSoulPoint()//得到魔灵数
{
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	if (nSoulState == 0|| nSoulState >= 1000000000)//大于10位为错误
		return 0;
	DWORD nPoint=nSoulState%100000;
	return max(0,nPoint);
}
void CItem::SetEquipSoulLev(short nLev)//得到铸灵等级
{
	if (nLev>99 || nLev <0)
		return;
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	short noldState=nSoulState/10000000;
	short noldLev=(nSoulState-noldState*10000000)/100000;
	int   noldPoint=nSoulState%100000;

	SetInt(ITEMDATA_SOULSTATE,noldState*10000000+nLev*100000+noldPoint);

}
void CItem::SetEquipSoulType(short nType)//得到铸灵属性
{
	if (nType>99 || nType <0)
		return;
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	short noldState=nSoulState/10000000;
	short noldLev=(nSoulState-noldState*10000000)/100000;
	int   noldPoint=nSoulState%100000;
	
	SetInt(ITEMDATA_SOULSTATE,nType*10000000+noldLev*100000+noldPoint);
}
void CItem::SetEquipSoulPoint(OBJID nPoint)//得到魔灵数
{
	if (nPoint>99999 || nPoint <0)
		return;
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	short noldState=nSoulState/10000000;
	short noldLev=(nSoulState-noldState*10000000)/100000;
	int   noldPoint=nSoulState%100000;
	
	SetInt(ITEMDATA_SOULSTATE,noldState*10000000+noldLev*100000+nPoint);
}
short CItem::GetSoulBookType()
{
	if (!IsSoulBook()) 
		return 0;
	else
	{
		int nType=(GetInt(ITEMDATA_TYPE)-(ITEMTYPE_SOULBOOK*10000))/100;
		if (nType<=0)
			return 0;
		return nType;
	}
}

bool CItem::IsActSoulBook()
{
	if (!IsSoulBook())
		return false;
	if(GetSoulBookType()==EM_SOULTYPE_BING || GetSoulBookType()==EM_SOULTYPE_HUO || 
	   GetSoulBookType()==EM_SOULTYPE_XUAN || GetSoulBookType()==EM_SOULTYPE_DU)
		return true;
	return false;
}
bool CItem::IsDefSoulBook()
{
	if (!IsSoulBook())
		return false;
	if(GetSoulBookType()==EM_SOULTYPE_LI || GetSoulBookType()==EM_SOULTYPE_LING || GetSoulBookType()==EM_SOULTYPE_DING || 
	   GetSoulBookType()==EM_SOULTYPE_SHEN || GetSoulBookType() == EM_SOULTYPE_TI)
		return true;
	return false;
}

