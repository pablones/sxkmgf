// PortableShop.cpp: implementation of the CPortableShop class.
//
//////////////////////////////////////////////////////////////////////

#include "PortableShop.h"
#include "windows.h"
#include "../share/define.h"
#include "MapGroup.h"
#include "npc.h"
#include "MsgNPCShop.h"
#include "MsgRandNPCShop.h"
#include "MsgAction.h"
//////////////////////////////////////////////////////////////////////
const int	SYNSHOP_REBATE_PERCENT				= 90;		// �����̵����

MYHEAP_IMPLEMENTATION(CPortableShop,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortableShop::CPortableShop()
{
	m_setGoods.clear();
}

CPortableShop::~CPortableShop()
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
bool CPortableShop::Create(PROCESS_ID idProcess,OBJID id)
{
	m_idProcess	= idProcess;

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
	sprintf(szSQL, "SELECT * FROM %s WHERE ownerid=%u order by id ASC LIMIT 60", _TBL_GOODS, id);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
	if(!pRes)
		return true;		// ��SHOPû�л�

	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		OBJID	idType = pRes->GetInt(SHOPDATA_ITEMTYPE);
		CItemTypeData* pType = ItemType()->QueryItemType(idType);
		if(!pType || m_setGoods[pType->GetID()])
		{
			LOGWARNING("�̵�[%u]�з��ַǷ��Ļ��ظ���itemtypeֵ[%u]", id, idType);
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
	m_idower=id;
	return true;
}
int	CPortableShop::QueryItemTypeSpend(OBJID idType)
{
	if(m_setGoods[idType])
	{
		return m_setGoods[idType]->needItemType;
	}
	else
		return -1;
}

int	CPortableShop::QueryItemAmountSpend(OBJID idType)
{
	if(m_setGoods[idType])
	{
		return m_setGoods[idType]->needItemAmount;
	}
	else
		return -1;
}
bool CPortableShop::SendInfo(CUser* pUser,OBJID id)
{
	CHECKF(pUser);
	CHECKF(id);
	int nStep = 1;
	DEBUG_TRY
	CMsgNPCShop msg;
 	if(!msg.Create(id,id,"�̵�"))
	return false;
	nStep = 2;
	GoodsIt it = m_setGoods.begin();
	for(; it != m_setGoods.end() ; it++)
	{
		nStep = 3;
		GoodInfo* pType = it->second;
		if(!pType)
			continue;
		nStep = 4;
		if(!msg.AddItem(pType->idType))
			return false;
	}
	nStep = 5;
	pUser->SendMsg(&msg);

	DEBUG_CATCH2("CPortableShop::SendInfo() %u",nStep)
	return true;
}