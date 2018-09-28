// ItemData.cpp: implementation of the CItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "ItemData.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "MapGroup.h"
#include "UserTable.h"
#include "ItemAddition.h"

char	szItemTable[] = _TBL_ITEM;
char	szEudemonTable[]	= _TBL_EUDEMON;
char	szPeifangTable[]	= _TBL_PEIFANG;
char	szWeapenSoulTable[]	= _TBL_WEAPONSOUL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItemData::CItemData()
{
	m_pData		= NULL;
	m_pType		= NULL;
	m_bNeedUpdate = false;
}

//////////////////////////////////////////////////////////////////////
CItemData::~CItemData()
{
	if(m_pData)
		m_pData->Release();
}

//////////////////////////////////////////////////////////////////////
bool CItemData::SaveInfo(void)
{
	CHECKF(m_pData);
	CHECKF(m_pType);

	if(GetInt(ITEMDATA_TYPE) == 144000)
		return true;

	if(m_bNeedUpdate)
	{
		m_pData->Update();
		m_bNeedUpdate = false;
	}

	OnSaveInfo();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CItemData::DeleteRecord(IDatabase* pDb, bool bDelEudemon/*=true*/)
{
	if(m_pData->GetInt(ITEMDATA_TYPE) == 144000)
		return true;

	return m_pData->DeleteRecord();
}

//////////////////////////////////////////////////////////////////////
int CItemData::GetInt(ITEMDATA idx)
{

	if(idx < ITEMTYPEDATA_OFFSET)
		return m_pData->GetInt(idx);
	
	return m_pType->GetInt((ITEMTYPEDATA)(idx-ITEMTYPEDATA_OFFSET));
}

///////////////////////////////////////
//  [11/14/2007 朱斌]
_int64 CItemData::GetInt64(ITEMDATA idx)
{

	if(idx < ITEMTYPEDATA_OFFSET)
		return m_pData->GetInt64(idx);
	
	return m_pType->GetInt64((ITEMTYPEDATA)(idx-ITEMTYPEDATA_OFFSET));
}

//////////////////////////////////////////////////////////////////////
void CItemData::SetInt(ITEMDATA idx, int nData, bool bUpdate/*=false*/)
{
	CHECK(idx < ITEMTYPEDATA_OFFSET);

	if(nData == GetInt(idx))
		return;

	m_pData->SetInt(idx, nData);

	if(GetInt(ITEMDATA_TYPE) == 144000)
		m_bNeedUpdate = false;
	else if(bUpdate)
		SaveInfo();
	else
		m_bNeedUpdate = true;
}

//////////////////////////////////////////////////////////////////////
void CItemData::SetInt64(ITEMDATA idx, __int64 i64Data, bool bUpdate/*=false*/)
{
	CHECK(idx < ITEMTYPEDATA_OFFSET);
	if(i64Data == GetInt64(idx))
		return;

	m_pData->SetInt64(idx, i64Data);

	if(GetInt(ITEMDATA_TYPE) == 144000)
		m_bNeedUpdate = false;
	else if(bUpdate)
		SaveInfo();
	else
		m_bNeedUpdate = true;
}

//////////////////////////////////////////////////////////////////////
LPCTSTR CItemData::GetStr(ITEMDATA idx)
{
	if(idx < ITEMTYPEDATA_OFFSET)
		return m_pData->GetStr(idx);
	
	return m_pType->GetStr((ITEMTYPEDATA)(idx-ITEMTYPEDATA_OFFSET));
}

//////////////////////////////////////////////////////////////////////
void CItemData::SetStr(ITEMDATA idx, LPCTSTR szData, int nSize, bool bUpdate /*= false*/)
{
	CHECK(idx < ITEMTYPEDATA_OFFSET);
	if(strcmp(szData,GetStr(idx)) == 0)
		return;

	m_pData->SetStr(idx, szData, nSize);

	if(m_pData->GetInt(ITEMDATA_TYPE) == 144000)
		m_bNeedUpdate = false;
	else if(bUpdate)
		SaveInfo();
	else
		m_bNeedUpdate = true;
}


//////////////////////////////////////////////////////////////////////
bool CItemData::Create(OBJID idItem, IDatabase* pDb)
{
	CHECKF(!m_pData);
	CHECKF(!m_pType);
	CHECKF(pDb);

	// data
	m_pData = CGameItemData::CreateNew();
	IF_NOT(m_pData)
		return false;
	IF_NOT(m_pData->Create(idItem, pDb))
		return false;

	// type
	int nType = GetInt(ITEMDATA_TYPE);// 加强日志 [陈剑飞11/3/2007]
	m_pType	= ItemType()->QueryItemType(nType);
	IF_NOT(m_pType)
	{
		char szMsg[256];
		sprintf(szMsg, "m_ptypeID= %d idItem = %d", nType,idItem);
		log_assert("IF_NOT_DEBUG", szMsg, __FILE__, __LINE__);
		//zeng堆栈打印
		//MyStackWalker sw;
		//sw.ShowCallstack();
		//
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CItemData::Create(IRecordset* pRes, IDatabase* pDb)
{
	CHECKF(!m_pData);
	CHECKF(!m_pType);
	CHECKF(pRes);
	CHECKF(pDb);

	// data
	m_pData = CGameItemData::CreateNew();
	IF_NOT(m_pData)
		return false;
	IF_NOT(m_pData->Create(pRes))
		return false;

	// type
	m_pType	= ItemType()->QueryItemType(GetInt(ITEMDATA_TYPE));
	if(!m_pType)
	{
		LOGERROR("数据库发现未知物品类型[%d]", GetInt(ITEMDATA_TYPE));
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
//通过ItemInfoStruct结构创建一个ITEM
//给玩家增加一个物品，最后会调用此函数
//王玉波增加默认参数bool bForChgMap,哪果为true,表示玩家切换地图组引起的物品CREATE,不会因为是升级武器而修改数据
//---彭峰-----2007/04/02------去掉默认参数，武器系统更改！---------
//
//√√√√√√√√√√√√√√√√√
bool CItemData::Create(IDatabase* pDb, IRecord* pDefault, const ItemInfoStruct* pInfo, IRecord* pDefaultEudemonData, bool bInsert/*=true*/, OBJID idNew/*=ID_NONE*/)			// false: 不存数据库
{
	CHECKF(!m_pData);
	CHECKF(!m_pType);
	CHECKF(pDb);
	CHECKF(pInfo);
	CHECKF(pDefault);
	
	OBJID idItem = pInfo->id;
	if(idNew)
		idItem = idNew;
	
	// data
	m_pData = CGameItemData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pDefault, idItem))
		return false;
	
	if(idNew)
		SetInt(ITEMDATA_ID_,		idNew);
	// item type
	m_pType	= ItemType()->QueryItemType( pInfo->idType );
	IF_NOT(m_pType)
		return false;
	
	
	m_pData->SetInt( ITEMDATA_TYPE,					pInfo->idType );
	m_pData->SetInt( ITEMDATA_OWNERID,				pInfo->idOwner );
	m_pData->SetInt( ITEMDATA_PLAYERID,				pInfo->idPlayer );
	m_pData->SetInt( ITEMDATA_AMOUNT,				pInfo->nAmount );
	m_pData->SetInt( ITEMDATA_AMOUNTLIMIT,			pInfo->nAmountLimit );
	m_pData->SetInt( ITEMDATA_POSITION,				pInfo->ucPosition );
	m_pData->SetInt( ITEMDATA_PACKAGE_INDEX,		pInfo->ucPackageIndex );
	m_pData->SetInt( ITEMDATA_KIDNAP,				pInfo->nKidnap );
	m_pData->SetInt( ITEMDATA_NEEDCHECK,			pInfo->nNeedCheck );
	
	m_pData->SetInt( ITEMDATA_LAST_CHECK_TIME,		pInfo->dwLastCheckTime );
	m_pData->SetInt( ITEMDATA_LIFE_TIME,			pInfo->uLifeTime );
	m_pData->SetInt( ITEMDATA_QUALITY,				pInfo->dwQual );
	m_pData->SetInt( ITEMDATA_STAR,					pInfo->dwStr );
	m_pData->SetInt( ITEMDATA_ADDLEV,				pInfo->dwAddLev );
	m_pData->SetInt( ITEMDATA_BASE1,				pInfo->BasicAddID[0] );
	m_pData->SetInt( ITEMDATA_BASE2,				pInfo->BasicAddID[1] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION1,		pInfo->AdditionAddID[0] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION2,		pInfo->AdditionAddID[1] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION3,		pInfo->AdditionAddID[2] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION4,		pInfo->AdditionAddID[3] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION5,		pInfo->AdditionAddID[4] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION6,		pInfo->AdditionAddID[5] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION7,		pInfo->AdditionAddID[6] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION8,		pInfo->AdditionAddID[7] );
	m_pData->SetInt( ITEMDATA_GEM1,					pInfo->GemTypeID[0] );
	m_pData->SetInt( ITEMDATA_GEM2,					pInfo->GemTypeID[1] );
	m_pData->SetInt( ITEMDATA_GEM3,					pInfo->GemTypeID[2] );
	m_pData->SetInt( ITEMDATA_GEM4,					pInfo->GemTypeID[3] );
	m_pData->SetInt( ITEMDATA_ADDLEV_FAILRATE,		pInfo->usAddLevFailRate );//[2010-07-19 goto]强化或加星失败后的加成成功率
	m_pData->SetInt( ITEMDATA_ADDSTAR_FAILRATE,		pInfo->usStarFailRate );//[2010-07-19 goto]强化或加星失败后的加成成功率
	m_pData->SetInt( ITEMDATA_COLOR,				pInfo->dwColor );
	m_pData->SetInt( ITEMDATA_BUYPRICE,				pInfo->dwPerPrice );
	m_pData->SetStr( ITEMDATA_MAKERNAME,			pInfo->szMakerName ,16);

	if(bInsert)
	{
		//湛卢剑 不写库
		if(pInfo->idType == 144000)
		{
			m_pData->ClearUpdateFlags();
			return true;
		}

		IF_NOT(m_pData->InsertRecord())
			return false;
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}
	
	return true;
}

bool CItemData::CreateSend(IDatabase* pDb, IRecord* pDefault, const ItemInfoStructSend* pInfo, IRecord* pDefaultEudemonData, bool bInsert/*=true*/, OBJID idNew/*=ID_NONE*/)			// false: 不存数据库
{
	CHECKF(!m_pData);
	CHECKF(!m_pType);
	CHECKF(pDb);
	CHECKF(pInfo);
	CHECKF(pDefault);
	
	OBJID idItem = pInfo->id;
	if(idNew)
		idItem = idNew;
	
	// data
	m_pData = CGameItemData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pDefault, idItem))
		return false;
	
	if(idNew)
		SetInt(ITEMDATA_ID_,		idNew);
	// item type
	m_pType	= ItemType()->QueryItemType( pInfo->idType );
	IF_NOT(m_pType)
		return false;
	
	
	m_pData->SetInt( ITEMDATA_TYPE,					pInfo->idType );
	m_pData->SetInt( ITEMDATA_OWNERID,				pInfo->idOwner );
	m_pData->SetInt( ITEMDATA_PLAYERID,				pInfo->idPlayer );
	m_pData->SetInt( ITEMDATA_AMOUNT,				pInfo->nAmount );
	m_pData->SetInt( ITEMDATA_AMOUNTLIMIT,			pInfo->nAmountLimit );
	m_pData->SetInt( ITEMDATA_POSITION,				pInfo->ucPosition );
	m_pData->SetInt( ITEMDATA_PACKAGE_INDEX,		pInfo->ucPackageIndex );
	m_pData->SetInt( ITEMDATA_KIDNAP,				pInfo->nKidnap );
	m_pData->SetInt( ITEMDATA_NEEDCHECK,			pInfo->nNeedCheck );
	
	m_pData->SetInt( ITEMDATA_LAST_CHECK_TIME,		pInfo->dwLastCheckTime );
	m_pData->SetInt( ITEMDATA_LIFE_TIME,			pInfo->uLifeTime );
	m_pData->SetInt( ITEMDATA_QUALITY,				pInfo->dwQual );
	m_pData->SetInt( ITEMDATA_STAR,					pInfo->dwStr );
	m_pData->SetInt( ITEMDATA_ADDLEV,				pInfo->dwAddLev );
	m_pData->SetInt( ITEMDATA_BASE1,				pInfo->BasicAddID[0] );
	m_pData->SetInt( ITEMDATA_BASE2,				pInfo->BasicAddID[1] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION1,		pInfo->AdditionAddID[0] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION2,		pInfo->AdditionAddID[1] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION3,		pInfo->AdditionAddID[2] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION4,		pInfo->AdditionAddID[3] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION5,		pInfo->AdditionAddID[4] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION6,		pInfo->AdditionAddID[5] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION7,		pInfo->AdditionAddID[6] );
	m_pData->SetInt( ITEMDATA_BASIC_ADDITION8,		pInfo->AdditionAddID[7] );
	m_pData->SetInt( ITEMDATA_GEM1,					pInfo->GemTypeID[0] );
	m_pData->SetInt( ITEMDATA_GEM2,					pInfo->GemTypeID[1] );
	m_pData->SetInt( ITEMDATA_GEM3,					pInfo->GemTypeID[2] );
	m_pData->SetInt( ITEMDATA_GEM4,					pInfo->GemTypeID[3] );
	m_pData->SetInt( ITEMDATA_ADDLEV_FAILRATE,		pInfo->usAddLevFailRate );//[2010-07-19 goto]强化或加星失败后的加成成功率
	m_pData->SetInt( ITEMDATA_ADDSTAR_FAILRATE,		pInfo->usStarFailRate );//[2010-07-19 goto]强化或加星失败后的加成成功率
	m_pData->SetInt( ITEMDATA_COLOR,				pInfo->dwColor );
	m_pData->SetInt( ITEMDATA_BUYPRICE,				pInfo->dwPerPrice );
	m_pData->SetStr( ITEMDATA_MAKERNAME,			pInfo->szMakerName ,16);
	
	if(bInsert)
	{
		IF_NOT(m_pData->InsertRecord())
			return false;
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}
	
	return true;
}
//调整数据..
void CItemData::AdjustItemData()
{
	
	float fAdjustData = 0.00;
	//得到耐久上限初始值
	int nAmountLimit = GetInt( ITEMDATA_AMOUNT_LIMIT_INIT );
	fAdjustData = ( float )( ::RandGet( 20 ) + 90 ) / 100.0;
	nAmountLimit *= fAdjustData;
	m_pData->SetInt( ITEMDATA_AMOUNT,nAmountLimit );
	m_pData->SetInt( ITEMDATA_AMOUNTLIMIT,nAmountLimit );
	//得到最大物理与最小物理攻击初始值
// 	int nMaxAttack = GetInt( ITEMDATA_ATTACK_MAX_INIT );
// 	int nMinAttack = GetInt( ITEMDATA_ATTACK_MIN_INIT );
	fAdjustData = ( float )( ::RandGet( 20 ) + 90 ) / 100.0;
// 	nMaxAttack *= fAdjustData;
// 	nMinAttack *= fAdjustData;
// 	m_pData->SetInt( ITEMDATA_MAX_ATTACK,nMaxAttack );
// 	m_pData->SetInt( ITEMDATA_MIN_ATTACK,nMinAttack );
	//得到最大最小魔法攻击初始值
// 	int nMaxMgcAttack = GetInt( ITEMDATA_MAGICATK_MAX_INIT );
// 	int nMinMgcAttack = GetInt( ITEMDATA_MAGICATK_MIN_INIT );
// 	fAdjustData = ( float )( ::RandGet( 20 ) + 90 ) / 100.0;
// 	nMaxMgcAttack *= fAdjustData;
// 	nMinMgcAttack *= fAdjustData;
// 	m_pData->SetInt( ITEMDATA_MAX_MAGICATTACK,nMaxMgcAttack );
// 	m_pData->SetInt( ITEMDATA_MIN_MAGICATTACK,nMinMgcAttack );
	//得到最大、最小物理攻击成长值初值
// 	int nMaxAttackGrow = GetInt( ITEMDATA_MAXATK_GROW_INIT );
// 	int nMinAttackGrow = GetInt( ITEMDATA_MINATK_GROW_INIT );
// 	fAdjustData = ( float )( ::RandGet( 20 ) + 90 ) / 100.0;
// 	nMaxAttackGrow *= fAdjustData;
// 	nMinAttackGrow *= fAdjustData;
// 	m_pData->SetInt( ITEMDATA_MAX_ATTACK_GROW,nMaxAttackGrow );
// 	m_pData->SetInt( ITEMDATA_MIN_ATTACK_GROW,nMinAttackGrow );
// 	//得到最大、最小魔法攻击成长初值
// 	int nMaxMgcAtkGrow = GetInt( ITEMDATA_MAXMAGICATK_GROW_INIT );
// 	int nMinMgcAtkGrow = GetInt( ITEMDATA_MINMAGICATK_GROW_INIT );
// 	fAdjustData = ( float )( ::RandGet( 20 ) + 90 ) / 100.0;
// 	nMaxMgcAtkGrow *= fAdjustData;
// 	nMinMgcAtkGrow *= fAdjustData;
// 	m_pData->SetInt( ITEMDATA_MAX_MAGICATTACK_GROW,nMaxMgcAtkGrow );
// 	m_pData->SetInt( ITEMDATA_MIN_MAGICATTACK_GROW,nMinMgcAtkGrow );
// 	//得到耐久成长初值
// 	int nAmountGrow = GetInt( ITEMEDATA_AMOUNT_GROW_INIT );
// 	fAdjustData = ( float )( ::RandGet( 20 ) + 90 ) / 100.0;
// 	nAmountGrow *= fAdjustData;
// 	m_pData->SetInt( ITEMDATA_AMOUNT_GROW,nAmountGrow );
	
}
//////////////////////////////////////////////////////////////////////
//06.12.29王玉波暂时注释此函数
//bool CItemData::Create (IRecord* pDefault, OBJID idType, OBJID idUser, int nPosition, IRecord* pDefaultEudemonData/*=NULL*/)
/*{
	CHECKF(!m_pData);
	CHECKF(!m_pType);
	CHECKF(idType != ID_NONE);
	CHECKF(pDefault);

	// type
	m_pType	= ItemType()->QueryItemType(idType);
	IF_NOT(m_pType)
		return false;

	// data
	m_pData = CGameItemData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pDefault, ID_NONE))
		return false;
	m_pData->SetInt( ITEMDATA_TYPE,		idType );
	m_pData->SetInt( ITEMDATA_OWNERID,	idUser );
	m_pData->SetInt( ITEMDATA_PLAYERID,	idUser );
	if( 4 == idType / 100000 && idType % 10 != 9 ) 
	{	
		AdjustItemData();
	}
	else
	{
		//itemtype值----------------------------begin-------------------------------------------
		m_pData->SetInt( ITEMDATA_AMOUNT,				GetInt( ITEMDATA_AMOUNT_INIT ) );
		m_pData->SetInt( ITEMDATA_AMOUNTLIMIT,			GetInt( ITEMDATA_AMOUNT_LIMIT_INIT ) );
		m_pData->SetInt( ITEMDATA_MAX_ATTACK,			GetInt( ITEMDATA_ATTACK_MAX_INIT ) );
		m_pData->SetInt( ITEMDATA_MIN_ATTACK,			GetInt( ITEMDATA_ATTACK_MIN_INIT ) );
		m_pData->SetInt( ITEMDATA_MAX_MAGICATTACK,		GetInt( ITEMDATA_MAGICATK_MAX_INIT ) );
		m_pData->SetInt( ITEMDATA_MIN_MAGICATTACK,		GetInt( ITEMDATA_MAGICATK_MIN_INIT ) );
		//---------------------------------------end--------------------------------------------
		//成长值为0
		m_pData->SetInt( ITEMDATA_MAX_ATTACK_GROW,		0 );
		m_pData->SetInt( ITEMDATA_MIN_ATTACK_GROW,		0 );
		m_pData->SetInt( ITEMDATA_MAX_MAGICATTACK_GROW, 0 );
		m_pData->SetInt( ITEMDATA_MIN_MAGICATTACK_GROW, 0 );
		m_pData->SetInt( ITEMDATA_AMOUNT_GROW,          0 );
	}
	m_pData->SetInt( ITEMDATA_IDENT,GetInt( ITEMDATA_IDENT_INIT ) );
	m_pData->SetInt( ITEMDATA_POSITION,nPosition );
//初始都没有孔....
	m_pData->SetInt( ITEMDATA_GEM1,0 );
	m_pData->SetInt( ITEMDATA_GEM2,0 );
	m_pData->SetInt( ITEMDATA_GEM3,0 );
	m_pData->SetInt( ITEMDATA_GEM4,0 );
	m_pData->SetInt( ITEMDATA_GEM5,0 );
	m_pData->SetInt( ITEMDATA_GEM6,0 );
	m_pData->SetInt( ITEMDATA_SPECIAL_EFFECT,0 );
	// db
	IF_NOT(m_pData->InsertRecord())
		return false;

	return true;
}*/


// info
//////////////////////////////////////////////////////////////////////
//发物品信息到客户端时会调用此函数
bool CItemData::GetInfo	(ItemInfoStructSend* pInfo)
{

	CHECKF(pInfo);
	CHECKF(GetID() != ID_NONE);

	pInfo->id					=	GetID();
	pInfo->idType				=	GetInt( ITEMDATA_TYPE );
	pInfo->idOwner				=	GetInt( ITEMDATA_OWNERID );
	pInfo->idPlayer				=	GetInt( ITEMDATA_PLAYERID );
	pInfo->nAmount				=	GetInt( ITEMDATA_AMOUNT );
	pInfo->nAmountLimit			=	GetInt( ITEMDATA_AMOUNTLIMIT );
	pInfo->ucPosition			=	GetInt( ITEMDATA_POSITION );
	pInfo->ucPackageIndex		=	GetInt( ITEMDATA_PACKAGE_INDEX );
	pInfo->nKidnap				=	GetInt( ITEMDATA_KIDNAP );
	pInfo->nNeedCheck			=	GetInt( ITEMDATA_NEEDCHECK );
	pInfo->dwLastCheckTime		=	GetInt( ITEMDATA_LAST_CHECK_TIME );
	pInfo->uLifeTime			=	GetInt( ITEMDATA_LIFE_TIME );
	pInfo->dwQual				=	GetInt( ITEMDATA_QUALITY );
	pInfo->dwStr				=	GetInt( ITEMDATA_STAR );
	pInfo->dwColor				=	GetInt( ITEMDATA_COLOR );
	pInfo->dwAddLev				=	GetInt( ITEMDATA_ADDLEV );
	pInfo->usCapability			=	GetInt( ITEMDATA_CAPABILITY_INIT );
	pInfo->usReqProff			=	GetInt( ITEMDATA_REQ_PROF );
	pInfo->usReqJobLev			=	GetInt( ITEMDATA_REQ_JOBLEVEL );
	pInfo->usReqUserLev			=	GetInt( ITEMDATA_REQ_USERLEVEL );
	pInfo->dwLife				=	GetInt( ITEMDATA_LIFE );
	pInfo->dwMana				=	GetInt( ITEMDATA_MANA );
	pInfo->dwAction				=	GetInt( ITEMDATA_ACTION );
	pInfo->idSuit				=	GetInt( ITEMDATA_SUIT_ID );


	pInfo->GemTypeID[ 0 ]		=	GetInt( ITEMDATA_GEM1 );
	pInfo->GemTypeID[ 1 ]		=	GetInt( ITEMDATA_GEM2 );
	pInfo->GemTypeID[ 2 ]		=	GetInt( ITEMDATA_GEM3 );
	pInfo->GemTypeID[ 3 ]		=	GetInt( ITEMDATA_GEM4 );

	pInfo->BasicAddID[ 0 ]		=	GetInt( ITEMDATA_BASE1 );
	pInfo->BasicAddID[ 1 ]		=	GetInt( ITEMDATA_BASE2 );
	
	pInfo->AdditionAddID[ 0 ]	=	GetInt( ITEMDATA_BASIC_ADDITION1 );
	pInfo->AdditionAddID[ 1 ]	=	GetInt( ITEMDATA_BASIC_ADDITION2 );
	pInfo->AdditionAddID[ 2 ]	=	GetInt( ITEMDATA_BASIC_ADDITION3 );
	pInfo->AdditionAddID[ 3 ]	=	GetInt( ITEMDATA_BASIC_ADDITION4 );
	pInfo->AdditionAddID[ 4 ]	=	GetInt( ITEMDATA_BASIC_ADDITION5 );
	pInfo->AdditionAddID[ 5 ]	=	GetInt( ITEMDATA_BASIC_ADDITION6 );
	pInfo->AdditionAddID[ 6 ]	=	GetInt( ITEMDATA_BASIC_ADDITION7 );
	pInfo->AdditionAddID[ 7 ]	=	GetInt( ITEMDATA_BASIC_ADDITION8 );
	pInfo->usAddLevFailRate			=	GetInt( ITEMDATA_ADDLEV_FAILRATE );//[2010-07-19 goto]强化或加星失败后的加成成功率
	pInfo->usStarFailRate			=	GetInt( ITEMDATA_ADDSTAR_FAILRATE );//[2010-07-19 goto]强化或加星失败后的加成成功率

	::SafeCopy(pInfo->szName,GetStr(ITEMDATA_NAME),16);
	::SafeCopy(pInfo->szMakerName,GetStr(ITEMDATA_MAKERNAME),16);
	return true;
}

bool CItemData::GetInfo	(ItemInfoStruct* pInfo)
{
	
	CHECKF(pInfo);
	CHECKF(GetID() != ID_NONE);
	
	pInfo->id					=	GetID();
	pInfo->idType				=	GetInt( ITEMDATA_TYPE );
	pInfo->idOwner				=	GetInt( ITEMDATA_OWNERID );
	pInfo->idPlayer				=	GetInt( ITEMDATA_PLAYERID );
	pInfo->nAmount				=	GetInt( ITEMDATA_AMOUNT );
	pInfo->nAmountLimit			=	GetInt( ITEMDATA_AMOUNTLIMIT );
	pInfo->ucPosition			=	GetInt( ITEMDATA_POSITION );
	pInfo->ucPackageIndex		=	GetInt( ITEMDATA_PACKAGE_INDEX );
	pInfo->nKidnap				=	GetInt( ITEMDATA_KIDNAP );
	pInfo->nNeedCheck			=	GetInt( ITEMDATA_NEEDCHECK );
	pInfo->dwLastCheckTime		=	GetInt( ITEMDATA_LAST_CHECK_TIME );
	pInfo->uLifeTime			=	GetInt( ITEMDATA_LIFE_TIME );
	pInfo->dwQual				=	GetInt( ITEMDATA_QUALITY );
	pInfo->dwStr				=	GetInt( ITEMDATA_STAR );
	pInfo->dwColor				=	GetInt( ITEMDATA_COLOR );
	pInfo->dwAddLev				=	GetInt( ITEMDATA_ADDLEV );
	pInfo->usCapability			=	GetInt( ITEMDATA_CAPABILITY_INIT );
	pInfo->usReqProff			=	GetInt( ITEMDATA_REQ_PROF );
	pInfo->usReqJobLev			=	GetInt( ITEMDATA_REQ_JOBLEVEL );
	pInfo->usReqUserLev			=	GetInt( ITEMDATA_REQ_USERLEVEL );
	pInfo->dwLife				=	GetInt( ITEMDATA_LIFE );
	pInfo->dwMana				=	GetInt( ITEMDATA_MANA );
	pInfo->dwAction				=	GetInt( ITEMDATA_ACTION );
	pInfo->idSuit				=	GetInt( ITEMDATA_SUIT_ID );
	
	
	pInfo->GemTypeID[ 0 ]		=	GetInt( ITEMDATA_GEM1 );
	pInfo->GemTypeID[ 1 ]		=	GetInt( ITEMDATA_GEM2 );
	pInfo->GemTypeID[ 2 ]		=	GetInt( ITEMDATA_GEM3 );
	pInfo->GemTypeID[ 3 ]		=	GetInt( ITEMDATA_GEM4 );
	
	pInfo->BasicAddID[ 0 ]		=	GetInt( ITEMDATA_BASE1 );
	pInfo->BasicAddID[ 1 ]		=	GetInt( ITEMDATA_BASE2 );
	
	pInfo->AdditionAddID[ 0 ]	=	GetInt( ITEMDATA_BASIC_ADDITION1 );
	pInfo->AdditionAddID[ 1 ]	=	GetInt( ITEMDATA_BASIC_ADDITION2 );
	pInfo->AdditionAddID[ 2 ]	=	GetInt( ITEMDATA_BASIC_ADDITION3 );
	pInfo->AdditionAddID[ 3 ]	=	GetInt( ITEMDATA_BASIC_ADDITION4 );
	pInfo->AdditionAddID[ 4 ]	=	GetInt( ITEMDATA_BASIC_ADDITION5 );
	pInfo->AdditionAddID[ 5 ]	=	GetInt( ITEMDATA_BASIC_ADDITION6 );
	pInfo->AdditionAddID[ 6 ]	=	GetInt( ITEMDATA_BASIC_ADDITION7 );
	pInfo->AdditionAddID[ 7 ]	=	GetInt( ITEMDATA_BASIC_ADDITION8 );
	pInfo->usAddLevFailRate			=	GetInt( ITEMDATA_ADDLEV_FAILRATE );//[2010-07-19 goto]强化或加星失败后的加成成功率
	pInfo->usStarFailRate			=	GetInt( ITEMDATA_ADDSTAR_FAILRATE );//[2010-07-19 goto]强化或加星失败后的加成成功率
	
	::SafeCopy(pInfo->szName,GetStr(ITEMDATA_NAME),16);
	::SafeCopy(pInfo->szMakerName,GetStr(ITEMDATA_MAKERNAME),16);
	return true;
}