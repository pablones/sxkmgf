// ItemType.cpp: implementation of the CItemType class.
//
//////////////////////////////////////////////////////////////////////

#include "winsock2.h"
#include "ItemType.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "ItemData.h"

MYHEAP_IMPLEMENTATION(CItemType,s_heap)
char	szItemTypeTable[] = _TBL_ITEMTYPE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItemType::CItemType()
{
	m_setType	= NULL;

}

//////////////////////////////////////////////////////////////////////
CItemType::~CItemType()
{
	if(m_setType)
		m_setType->Release();
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
bool CItemType::Create(IDatabase* pDb)
{
	m_setType	= CItemTypeSet::CreateNew(true);
	CHECKF(m_setType);

	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s", _TBL_ITEMTYPE);
	IF_NOT_(m_setType->Create(szSQL, pDb))
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////
CItemTypeData* CItemType::QueryItemType(OBJID idType)
{
	return m_setType->GetObj(idType);
}

int CItemType::GetMonoly(OBJID idType)
{
	CItemTypeData* pType = m_setType->GetObj(idType);

	IF_NOT(pType)
	{
		LOGERROR( "CItemType::GetInfo函数出错,idType:%d",idType );
		return 0;
	}
	else
	{
		return pType->GetInt(ITEMTYPEDATA_MONOPOLY);
	}
}

//////////////////////////////////////////////////////////////////////
//06.12.28
//王玉波
//读取ItemType表
bool CItemType::GetInfo(OBJID idType, ItemInfoStruct* pInfo)
{
	memset(pInfo, 0, sizeof(ItemInfoStruct));

	CItemTypeData* pType = m_setType->GetObj(idType);
	IF_NOT(pType)
	{
		LOGERROR( "CItemType::GetInfo函数出错,idType:%d",idType );
		return false;
	}

	pInfo->id				= ID_NONE;
	pInfo->idType			= pType->GetID();
	pInfo->idOwner			= ID_NONE;
	pInfo->idPlayer			= ID_NONE;
	pInfo->nAmount			= pType->GetInt( ITEMTYPEDATA_AMOUNT );
	pInfo->nAmountLimit		= pType->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
	pInfo->ucPosition		= ITEMPOSITION_NONE;
	pInfo->ucPackageIndex	= ITEMPOSITION_NONE;
	pInfo->nKidnap			= 0;
	pInfo->nNeedCheck		= 0;
	pInfo->uLifeTime		= 0;
	pInfo->dwLastCheckTime  = 0;

	pInfo->usCapability		= pType->GetInt(ITEMTYPEDATA_CAPABILITY);
	pInfo->usReqProff		= pType->GetInt(ITEMTYPEDATA_REQ_PROF);
	pInfo->usReqJobLev		= pType->GetInt(ITEMTYPEDATA_REQ_USERLEVEL);
	pInfo->usReqUserLev		= pType->GetInt(ITEMTYPEDATA_REQ_JOBLEVEL);
	pInfo->idSuit			= pType->GetInt(ITEMTYPEDATA_SUIT_ID);

// 	for(int i = 0 ; i < 3 ; i ++)
// 	{
// 		pInfo->GemTypeID[i] = _NOT_HOLE; //可开孔未打开
// 	}

	pInfo->dwLife			= pType->GetInt(ITEMTYPEDATA_LIFE);
	pInfo->dwMana			= pType->GetInt(ITEMTYPEDATA_MANA);
	pInfo->dwAction			= pType->GetInt(ITEMTYPEDATA_ACTION);
	pInfo->dwQual			= 0;
	pInfo->dwStr			= pType->GetInt(ITEMTYPEDATA_STAR_INIT);
	pInfo->dwColor			= pType->GetInt(ITEMTYPEDATA_COLOR_INIT);
	pInfo->dwAddLev         = 0;

	pInfo->BasicAddID[0]	=  pType->GetInt(ITEMTYPEDATA_BASE1_INIT);
	pInfo->BasicAddID[1]	=  pType->GetInt(ITEMTYPEDATA_BASE2_INIT);
	pInfo->AdditionAddID[0] =  pType->GetInt(ITEMTYPEDATA_BASIC_ADDITION1_INIT);
	pInfo->AdditionAddID[1] =  pType->GetInt(ITEMTYPEDATA_BASIC_ADDITION2_INIT);
	pInfo->AdditionAddID[2] =  pType->GetInt(ITEMTYPEDATA_BASIC_ADDITION3_INIT);
	pInfo->AdditionAddID[3] =  pType->GetInt(ITEMTYPEDATA_BASIC_ADDITION4_INIT);
	pInfo->AdditionAddID[4] =  pType->GetInt(ITEMTYPEDATA_BASIC_ADDITION5_INIT);
	pInfo->AdditionAddID[5] =  pType->GetInt(ITEMTYPEDATA_BASIC_ADDITION6_INIT);
	pInfo->AdditionAddID[6] =  pType->GetInt(ITEMTYPEDATA_BASIC_ADDITION7_INIT);
	pInfo->AdditionAddID[7] =  pType->GetInt(ITEMTYPEDATA_BASIC_ADDITION8_INIT);

	pInfo->GemTypeID[0]		=  pType->GetInt(ITEMTYPEDATA_GEM1_INIT);
	pInfo->GemTypeID[1]		=  pType->GetInt(ITEMTYPEDATA_GEM2_INIT);
	pInfo->GemTypeID[2]		=  pType->GetInt(ITEMTYPEDATA_GEM3_INIT);

	pInfo->usAddLevFailRate	=  0;//强化失败加成
	pInfo->usStarFailRate	=  0;//升星失败加成

	strcpy( pInfo->szName,pType->GetStr( ITEMTYPEDATA_NAME ) );
	return true;
}

