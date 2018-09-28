// PortableShopManager.cpp: implementation of the CPortableShopManager class.
//
//////////////////////////////////////////////////////////////////////

#include "PortableShopManager.h"
#include "windows.h"
#include "../share/define.h"
#include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortableShopManager::CPortableShopManager()
{
	m_ShopList.clear();
}

CPortableShopManager::~CPortableShopManager()
{
	for (int i=0;i < m_ShopList.size();i++)
	{
		m_ShopList[i]->Release();
	}
	m_ShopList.clear();
}

bool	CPortableShopManager::CreateAll(PROCESS_ID idProcess)
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess));
	m_idProcess = idProcess;
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT ownerid FROM %s WHERE ownerid >= %u && ownerid <= %u order by id ASC LIMIT 60", _TBL_GOODS, PORTABLESHOP_DOWN,PORTABLESHOP_UP);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
	if(!pRes)
		return true;		// 该SHOP没有货
	
	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		CPortableShop *pShop=CPortableShop::CreateNew();
		if(!pShop)
		{
			pRes->Release();
			return false;
		}
		if(pShop->Create(idProcess,pRes->GetInt(0)))
			m_ShopList.push_back(pShop);
		pRes->MoveNext();
	}
	pRes->Release();
	return true;

}
CPortableShop* CPortableShopManager::QueryShopByOwerid(OBJID Owerid)
{
	for (int i=0;i < m_ShopList.size();i++)
	{
		CPortableShop* pShop = m_ShopList[i];
		if(pShop && pShop->GetOwerID()==Owerid)
			return m_ShopList[i];
	}

	return NULL;
}
