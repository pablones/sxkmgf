// NpcShop.cpp: implementation of the CNpcShop class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "../share/define.h"
#include "NpcShop.h"
#include "MapGroup.h"
#include "npc.h"
#include "MsgNPCShop.h"
#include "MsgRandNPCShop.h"
#include "MsgAction.h"

//////////////////////////////////////////////////////////////////////
const int	SYNSHOP_REBATE_PERCENT				= 90;		// 帮派商店打折

MYHEAP_IMPLEMENTATION(CNpcShop,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcShop::CNpcShop()
{
	m_setGoods.clear();
	m_tUpdate.Clear();
}

//////////////////////////////////////////////////////////////////////
CNpcShop::~CNpcShop()
{
	GoodsIt it = m_setGoods.begin();
	for(; it != m_setGoods.end() ; it++)
	{
		GoodInfo* pType = it->second;
		delete pType;
	}
	m_setGoods.clear();
}

//////////////////////////////////////////////////////////////////////
bool CNpcShop::Create(PROCESS_ID idProcess, CNpc* pOwner, OBJID idNpc)
{
	m_idProcess	= idProcess;
	m_pOwner	= pOwner;

	enum SHOPDATA 
	{ 
		SHOPDATA_OWNERID=1, 
		SHOPDATA_ITEMTYPE, 
		SHOPDATA_ITEMTYPESPEND,
		SHOPDATA_ITEMTYPESPENDMUN,
		SHOPDATA_BASEPRIC,
		SHOPDATA_PRICRAND,
		SHOPDATA_SELLTYPE,
	};

	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE ownerid=%u order by id ASC LIMIT 60", _TBL_GOODS, idNpc);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
	if(!pRes)
		return true;		// 该SHOP没有货

	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		OBJID	idType = pRes->GetInt(SHOPDATA_ITEMTYPE);
		CItemTypeData* pType = ItemType()->QueryItemType(idType);
		if(!pType || m_setGoods[pType->GetID()])
		{
			LOGWARNING("商店[%u]中发现非法的或重复的itemtype值[%u]", idNpc, idType);
			pRes->MoveNext();
			continue;
		}

		GoodInfo* pInfo = new GoodInfo;
		pInfo->idType = idType;
		pInfo->needItemType = pRes->GetInt(SHOPDATA_ITEMTYPESPEND);
		pInfo->needItemAmount = pRes->GetInt(SHOPDATA_ITEMTYPESPENDMUN);
		pInfo->nBasePrice = pRes->GetInt(SHOPDATA_BASEPRIC);
		pInfo->nPriceRand = pRes->GetInt(SHOPDATA_PRICRAND);
		pInfo->nSellType = pRes->GetInt(SHOPDATA_SELLTYPE);

		m_setGoods[pType->GetID()] = pInfo;
		pRes->MoveNext();
	}
	pRes->Release();


	if(pOwner->IsRandTaskShopNpc())
		DoRandPrice();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpcShop::IsOpen()	
{ 
	return /*!(m_pOwner->IsSynNpc() && m_pOwner->GetMap()->GetSynID() != ID_NONE)*/true; 
}

//////////////////////////////////////////////////////////////////////
int CNpcShop::Rebate(int nPrice, OBJID idSyn, int nRankShow)
{
// 	OBJID idOwnerSyn = m_pOwner->GetSynID();
// 	if(idOwnerSyn != ID_NONE && idSyn == idOwnerSyn)
// 	{
// 		int nRebate = /*m_pOwner->GetInt(NPCDATA_DATA0)*/100;
// 		
// 		return MulDiv(nPrice, nRebate, 100);
// 	}

	return nPrice;
}

void CNpcShop::DoRandPrice()
{
	bool bDo = false;
	if(!m_tUpdate.IsActive())
	{
		m_tUpdate.Startup(600);
		bDo = true;
	}
	else
	{
		if(m_tUpdate.ToNextTime())
			bDo = true;
	}

	if(!bDo)
		return;

	GoodsIt it = m_setGoods.begin();
	for(; it != m_setGoods.end() ; it++)
	{
		GoodInfo* pType = it->second;
		pType->nRnadPrice = pType->nBasePrice * (1 + RandGet(pType->nPriceRand) / 100.0f);
		pType->nLeaveAmount = 250;
	}
}

int CNpcShop::QueryRandShopLeave(OBJID idType)
{
	if(m_setGoods[idType])
	{
		return m_setGoods[idType]->nLeaveAmount;
	}
	else
		return -1;
}

int CNpcShop::QueryRandShopPrice(OBJID idType)
{
	if(m_setGoods[idType])
	{
		return m_setGoods[idType]->nRnadPrice;
	}
	else
		return -1;
}

int CNpcShop::QueryRandShopSellType(OBJID idType)
{
	if(m_setGoods[idType])
	{
		return m_setGoods[idType]->nSellType;
	}
	else
		return -1;
}

int	CNpcShop::QueryItemTypeSpend(OBJID idType)
{
	if(m_setGoods[idType])
	{
		return m_setGoods[idType]->needItemType;
	}
	else
		return -1;
}

int	CNpcShop::QueryItemAmountSpend(OBJID idType)
{
	if(m_setGoods[idType])
	{
		return m_setGoods[idType]->needItemAmount;
	}
	else
		return -1;
}

bool CNpcShop::SendInfo(CUser* pUser)
{
	CHECKF(m_pOwner);
	CHECKF(pUser);
	int nStep = 1;
	DEBUG_TRY
	if(m_pOwner->IsRandTaskShopNpc())
	{
		nStep = 2;
		CMsgRandNPCShop msg;
		if(!msg.Create(m_pOwner->GetID(),m_pOwner->GetInt(NPCDATA_TYPE),m_pOwner->GetName()))
			return false;
		
		GoodsIt it = m_setGoods.begin();
		for(; it != m_setGoods.end() ; it++)
		{
			nStep = 3;
			GoodInfo* pType = it->second;
			if(!msg.AddItem(pType->idType,pType->nSellType,pType->nLeaveAmount,pType->nRnadPrice))
				return false;
		}
		nStep = 4;
		pUser->SendMsg(&msg);
		return true;
	}

	nStep = 5;
	CMsgNPCShop msg;
	if (m_pOwner->GetID()>10000)
	{
		if(!msg.Create(m_pOwner->GetNpcType(),m_pOwner->GetInt(NPCDATA_TYPE),m_pOwner->GetName()))
		return false;
	}
	else
	{
 		if(!msg.Create(m_pOwner->GetID(),m_pOwner->GetInt(NPCDATA_TYPE),m_pOwner->GetName()))
		return false;
	}
	nStep = 6;
	GoodsIt it = m_setGoods.begin();
	for(; it != m_setGoods.end() ; it++)
	{
		nStep = 7;
		GoodInfo* pType = it->second;
		if(!pType)
			continue;
		nStep = 8;
		if(!msg.AddItem(pType->idType))
			return false;
	}
	nStep = 9;
	pUser->SendMsg(&msg);


// 	CMsgAction	msgAct;
// 	IF_OK(msgAct.Create(pUser->GetID(), pUser->GetPosX(), pUser->GetPosY(), pUser->GetDir(), actionOpenDialog, 3,0,0))
// 			pUser->SendMsg(&msgAct);
	DEBUG_CATCH2("CNpcShop::SendInfo() %u",nStep)
	return true;
}









