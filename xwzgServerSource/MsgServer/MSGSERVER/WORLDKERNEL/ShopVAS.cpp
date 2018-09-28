// ShopVAS.cpp: implementation of the CShopVAS class.
//
//////////////////////////////////////////////////////////////////////

#include "ShopVAS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShopVAS::CShopVAS()
{
	m_pDb = NULL;
	maxID = 0;
	m_setVASShop.clear();
}

CShopVAS::~CShopVAS()
{
	m_setVASShop.clear();
}

bool CShopVAS::Create(IDatabase* pDb)
{
	m_pDb = pDb;
	char szSQL[1024];

	sprintf(szSQL, "SELECT * FROM %s order by id", _TBL_SHOPVAS);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
	CHECKF(pRes);

	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		vasItemInfo itemInfo;
		itemInfo.id = pRes->LoadDWord("Id");
		itemInfo.itemType = pRes->LoadDWord("itemtype");
		itemInfo.price = pRes->LoadDWord("price");
		itemInfo.time = pRes->LoadDWord("time");
		itemInfo.discount = pRes->LoadDWord("discount");
		itemInfo.payType = pRes->LoadDWord("paytype");
		itemInfo.state = pRes->LoadDWord("state");

		m_setVASShop.push_back(itemInfo);

		if(maxID < itemInfo.id)
			maxID = itemInfo.id;
		
		pRes->MoveNext();
	}
	pRes->Release();
	return true;
}

void CShopVAS::setItem(int n,const vasItemInfo& info,bool bSave)
{
	if(n < m_setVASShop.size())
	{
		m_setVASShop[n].doType = info.doType;
		m_setVASShop[n].id = info.id;
	    m_setVASShop[n].itemType = info.itemType;
		m_setVASShop[n].price = info.price;
		m_setVASShop[n].time = info.time;
		m_setVASShop[n].discount = info.discount;
		m_setVASShop[n].payType = info.payType;
		m_setVASShop[n].state = info.state;

		if(bSave)
		{
			char szSQL[1024];
			sprintf(szSQL, "UPDATE %s SET itemtype=%d,time=%d,discount=%d,paytype=%d,state=%d,price=%d WHERE Id=%d LIMIT 1"
				, _TBL_SHOPVAS,info.itemType,info.time,info.discount,info.payType,info.state,info.price,info.id);
			m_pDb->ExecuteSQL(szSQL);
		}
	}
}


vasItemInfo* CShopVAS::GetItemByType(OBJID itemType)
{
	for(int i=0; i<GetShopAmount(); i++)
	{
		vasItemInfo temp = GetItemByIndex(i);
		if (temp.itemType==itemType)
			return &GetItemByIndex(i);
	}
	return NULL;
}