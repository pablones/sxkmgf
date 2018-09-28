// ShopVAS.cpp: implementation of the CShopVAS class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdtctl.h>
#include "ShopVAS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShopVASItem::CShopVASItem()
{
	m_pDb = NULL;
	maxID = 0;
	m_setVASShop.clear();
	m_setASaleShop.clear();
	m_setASaleItem.clear();
	m_setVasLimitItem.clear();
}

CShopVASItem::~CShopVASItem()
{
	m_setVASShop.clear();
	m_setASaleShop.clear();
	m_setASaleItem.clear();
	m_setVasLimitItem.clear();
}

bool CShopVASItem::Create(IDatabase* pDb)
{
	m_pDb = pDb;
	char szSQL[1024];

	sprintf(szSQL, "SELECT * FROM %s order by id", _TBL_SHOPVAS);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
	CHECKF(pRes);

	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		vasItem itemInfo = {0};
		itemInfo.id = pRes->LoadDWord("Id");
		itemInfo.itemType = pRes->LoadDWord("itemtype");
		itemInfo.price = pRes->LoadDWord("price");
		itemInfo.time = pRes->LoadDWord("time");
		itemInfo.discount = pRes->LoadDWord("discount");
		itemInfo.payType = pRes->LoadDWord("paytype");
		itemInfo.state = pRes->LoadDWord("state");
		itemInfo.aSaleCount = pRes->LoadDWord("salecount");

		if (itemInfo.id < 10000)
			m_setVASShop.push_back(itemInfo);
		else if (itemInfo.id > 10000 && itemInfo.id < 19999)//特价商品
			m_setASaleShop.push_back(itemInfo);

		if(maxID < itemInfo.id)
			maxID = itemInfo.id;
		
		pRes->MoveNext();
	}
	pRes->Release();

	if (!CreateASaleItem())//特价商品,读取上次的，如果没有就重新生成
		GenerateASaleItem(ASALE_ITEM_AMOUNT);

	CreateLimitItem();//限量商品

	return true;
}

int CShopVASItem::insertItem(vasItem& info,bool bSave)
{
	maxID ++ ; //确保id不冲突
	char szSQL[1024];
	info.id = maxID;
	m_setVASShop.push_back(info); //添加本地记录

	sprintf(szSQL, "INSERT INTO %s SET Id=%d,itemtype=%d,time=%d,discount=%d,paytype=%d,state=%d,price=%d"
		, _TBL_SHOPVAS, info.id,info.itemType,info.time,info.discount,info.payType,info.state,info.price);
	m_pDb->ExecuteSQL(szSQL); 
	return info.id;
}

void CShopVASItem::setItem(int n,const vasItem& info,bool bSave)
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

void CShopVASItem::deleteItem(const vasItem& info,bool bSave)
{
	char szSQL[1024];
	sprintf(szSQL, "DELETE FROM %s WHERE Id=%d", _TBL_SHOPVAS,info.id);
	m_pDb->ExecuteSQL(szSQL);
}

vasItem* CShopVASItem::GetItemByType(OBJID itemType)
{
	for(int i=0; i<GetShopAmount(); i++)
	{
		vasItem temp = GetItemByIndex(i);
		if (temp.itemType==itemType)
			return &GetItemByIndex(i);
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////
bool CShopVASItem::CreateASaleItem()
{
	DEBUG_TRY
	m_setASaleItem.clear();
	char szSQL[1024];
	
	sprintf(szSQL, "SELECT * FROM %s order by id", _TBL_VASASALEITEM);
	IRecordset* pRes = m_pDb->CreateNewRecordset(szSQL, false);
	CHECKF(pRes);
	
	if(pRes->RecordCount() <= 0)
	{
		pRes->Release();
		return false;
	}
	else if (pRes->RecordCount() > ASALE_ITEM_AMOUNT)
		::LogSave("CShopVASItem::CreateASaleItem() 特价商品表里商品种类过多。amount=%d", pRes->RecordCount());
	
	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		vasItem itemInfo;
		itemInfo.id = pRes->LoadDWord("Id");
		itemInfo.itemType = pRes->LoadDWord("itemtype");
		itemInfo.price = pRes->LoadDWord("price");
		itemInfo.time = pRes->LoadDWord("time");
		itemInfo.discount = pRes->LoadDWord("discount");
		itemInfo.payType = pRes->LoadDWord("paytype");
		itemInfo.state = pRes->LoadDWord("state");
		itemInfo.aSaleCount = pRes->LoadDWord("salecount");

		CString strCreateTime;
		pRes->LoadString( strCreateTime.GetBuffer(0), "createtime", strCreateTime.GetLength() );//得到当时写的时间
		if( strCreateTime == "" || strCreateTime == "0")
		{
			SAFE_RELEASE(pRes);
			return false;
		}
		COleDateTime lastDate;
		if (!lastDate.ParseDateTime( strCreateTime ))
		{
			SAFE_RELEASE(pRes);
			return false;
		}
		COleDateTime nowDate = COleDateTime::GetCurrentTime();
		if (nowDate.GetHour() > 5 && lastDate.GetDay() != nowDate.GetDay())//过了一天了且错过了刷新时间
		{
			SAFE_RELEASE(pRes);
			return false;
		}
		else if (nowDate.GetHour() > 5 && lastDate.GetHour() < 5 && lastDate.GetDay() == nowDate.GetDay())//没过一天但错过了刷新时间
		{
			SAFE_RELEASE(pRes);
			return false;
		}

		m_setASaleItem.push_back(itemInfo);

		pRes->MoveNext();
	}
	pRes->Release();
	DEBUG_CATCH("CShopVASItem::CreateASaleItem()")
	return true;
}

void CShopVASItem::InsertASaleItem(const vasItem& info)
{
	char szSQL[1024];
	m_setASaleItem.push_back(info); //添加本地记录
	
	sprintf(szSQL, "INSERT INTO %s SET Id=%d,itemtype=%d,price=%d,salecount=%d,createtime='%s'"
		, _TBL_VASASALEITEM, info.id,info.itemType,info.price,info.aSaleCount, COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
	m_pDb->ExecuteSQL(szSQL);
}

void CShopVASItem::ClearASaleItem()
{
	m_setASaleItem.clear();
	char szSQL[1024];
	sprintf(szSQL, "truncate table %s" ,_TBL_VASASALEITEM);
	m_pDb->ExecuteSQL(szSQL);
}

vasItem* CShopVASItem::GetASaleItemByType(OBJID itemType)
{
	for(int i=0; i<m_setASaleItem.size(); i++)
	{
		vasItem temp = m_setASaleItem[i];
		if (temp.itemType==itemType)
			return &m_setASaleItem[i];
	}
	return NULL;
}
vasItem* CShopVASItem::GetASaleItemByIndex(int nIndex)
{
	if (nIndex < m_setASaleItem.size())
		return &m_setASaleItem[nIndex];

	return NULL;
}
bool CShopVASItem::SetASaleItemAmount(OBJID itemType, int nAmount, bool bUpdate)
{
	if (nAmount < 0)
		nAmount = 0;
	vasItem* pTemp = NULL;
	for(int i=0; i<m_setASaleItem.size(); i++)
	{
		if (m_setASaleItem[i].itemType==itemType)
			pTemp = &m_setASaleItem[i];
	}
	if (!pTemp)
		return false;

	if (pTemp->aSaleCount == nAmount)
		return true;
	pTemp->aSaleCount = nAmount;

	if (bUpdate)
	{
		char szSQL[1024];
		sprintf(szSQL, "UPDATE %s SET salecount=%d WHERE id=%d LIMIT 1" ,_TBL_VASASALEITEM,nAmount,pTemp->id);
		m_pDb->ExecuteSQL(szSQL);
	}
	
	return true;
}

bool CShopVASItem::GenerateASaleItem(int nAmount)
{
	DEBUG_TRY
	if (m_setASaleShop.size()<=0)
		::LogSave("CShopVASItem::GenerateASaleItem error! 缺少候选的特价商品!");

	CHECKF(nAmount);
	CHECKF(nAmount < 6);
	CHECKF(m_setASaleShop.size()>nAmount);

	ClearASaleItem();
	int nRand = 0;
	int nTemp[6] = {-1,-1,-1,-1,-1,-1};
	
	::LOGMSG("开始随机抽取商城特价商品");
	for (int i=0; i<nAmount; i++)
	{
		nRand = ::RandGet(m_setASaleShop.size()-i);
		//TRACE("rand1=%d", nRand);
		for (int j=0; j<i; j++)
		{
			if (nRand>=nTemp[j])
				nRand++;
		}
		
		nTemp[i] = nRand;
		//TRACE("rand2=%d\n", nRand);
		if (nRand < m_setASaleShop.size())
		{
			if (!GetASaleItemByType(m_setASaleShop[nRand].itemType))
				InsertASaleItem(m_setASaleShop[nRand]);
		}
	}
	::LOGMSG("抽取商城特价商品完成, 共抽出%d个商品", m_setASaleItem.size());
	DEBUG_CATCH("CShopVASItem::GenerateASaleItem")
	return m_setASaleItem.size() == nAmount;
}

bool CShopVASItem::CreateLimitItem()
{
	CHECKF(m_pDb);

	char szSQL[256] = "";
	sprintf(szSQL, "SELECT * FROM %s", _TBL_VASLIMITITEM);
	IRecordset* pRes = m_pDb->CreateNewRecordset(szSQL, false);
	CHECKF(pRes);
	
	vasLimitItem info = {0};
	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		info.id	= pRes->LoadDWord("id");
		info.itemType = pRes->LoadDWord("itemtype");
		info.aSaleCount = pRes->LoadDWord("salecount");

		m_setVasLimitItem.push_back(info);
		
		pRes->MoveNext();
	}
	pRes->Release();

	OBJID itemType = 0;
	for(i=0; i<GetShopAmount(); i++)
	{
		vasItem temp = GetItemByIndex(i);
		itemType = temp.itemType;
		if(itemType > 999999)//点卷商品
			itemType /= 10;

		if (temp.aSaleCount > 0 && IsItemOnSale(temp.time))
		{
			if(!GetLimitItemByType(itemType))
				InsertLimitItem(temp.id, itemType, temp.aSaleCount);
		}
	}
	return NULL;
}

vasLimitItem* CShopVASItem::GetLimitItemByType(OBJID itemType)
{
	for(int i = m_setVasLimitItem.size()-1; i>=0; i--)
	{
		if(m_setVasLimitItem[i].itemType == itemType)
			return &m_setVasLimitItem[i];
	}
	return NULL;
}

void CShopVASItem::InsertLimitItem(OBJID id, OBJID itemType, int nCount)
{
	char szSQL[1024] = "";

	vasLimitItem info = {0};
	info.id = id;
	info.itemType = itemType;
	info.aSaleCount = nCount;
	m_setVasLimitItem.push_back(info);

	sprintf(szSQL, "INSERT INTO %s SET Id=%d,itemtype=%d,salecount=%d" , _TBL_VASLIMITITEM, id, itemType, nCount);
	m_pDb->ExecuteSQL(szSQL);
}
//是否卖完
bool CShopVASItem::IsSaledOut(OBJID itemType, int nNeed/*=1*/)
{
	vasLimitItem* pTemp = GetLimitItemByType(itemType);
	if (!pTemp)
		return false;
// 	CHECKF(m_pDb);
// 	
// 	char szSQL[256] = "";
// 	sprintf(szSQL, "SELECT * FROM %s WHERE itemtype=%d LIMIT 1", _TBL_VASLIMITITEM, itemType);
// 	IRecordset* pRes = m_pDb->CreateNewRecordset(szSQL, false);
// 	CHECKF(pRes);
// 	if(pRes->RecordCount() < 1)
// 	{
// 		SAFE_RELEASE(pRes);
// 		return false;
// 	}
// 	int nAmount = pRes->LoadDWord("salecount");
// 	if(pTemp->aSaleCount != nAmount)
// 		pTemp->aSaleCount = nAmount;
// 
// 	SAFE_RELEASE(pRes);
	return nNeed > pTemp->aSaleCount;
}

bool CShopVASItem::SetLimitItemAmount(OBJID itemType, int nAmount, bool bUpdate)
{
	if (nAmount < 0)
		nAmount = 0;
	vasLimitItem* pTemp = GetLimitItemByType(itemType);
	if (!pTemp)
		return false;
	
	if (pTemp->aSaleCount == nAmount)
		return true;

	pTemp->aSaleCount = nAmount;
	
	if (bUpdate)
	{
		char szSQL[256] = "";
		sprintf(szSQL, "UPDATE %s SET salecount=%d WHERE id=%d LIMIT 1" , _TBL_VASLIMITITEM, nAmount, pTemp->id);
		m_pDb->ExecuteSQL(szSQL);
	}
	
	return true;
}

bool CShopVASItem::IsLimitItem(OBJID itemType)
{
	if(GetLimitItemByType(itemType))
		return true;

	return false;
}

vasLimitItem* CShopVASItem::GetLimitItemByIndex(int nIndex)
{
	if (nIndex < m_setVasLimitItem.size())
		return &m_setVasLimitItem[nIndex];
	
	return NULL;
}

bool CShopVASItem::IsItemOnSale(int time)
{
	DEBUG_TRY
	if(time == 0)//不限时的
		return true;

	CTime timecount(time/10000, (time % 10000)/100, time % 100,0,0,0);
	CTime timeCurr(CTime::GetCurrentTime());
	CTimeSpan spanTime = timeCurr - timecount;

	if(spanTime.GetTotalSeconds() > 0)//过了限时
		return false;
	DEBUG_CATCH("CShopVASItem::IsItemOnSale")
	
	return true;
}
