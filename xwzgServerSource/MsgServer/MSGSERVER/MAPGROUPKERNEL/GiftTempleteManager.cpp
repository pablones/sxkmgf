// GiftTempleteManager.cpp: implementation of the CGiftTempleteManager class.
//
//////////////////////////////////////////////////////////////////////

#include "GiftTempleteManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//char	szGiftTemp001Table[] = "sk_gift_templet_1001";
//char	szGiftTemp002Table[] = "sk_gift_templet_1002";
//char	szGiftTemp003Table[] = "sk_gift_templet_1003";
//char	szGiftTemp005Table[] = "sk_gift_templet_1005";
//char	szGiftTemp006Table[] = "sk_gift_templet_1006";
//char	szGiftTemp007Table[] = "sk_gift_templet_1007";
//char	szGiftTemp008Table[] = "sk_gift_templet_1008";

CGiftTempleteManager::CGiftTempleteManager()
{

}

CGiftTempleteManager::~CGiftTempleteManager()
{

	for(MAP_GIFTTEMPLETE::iterator it=m_mGiftTemplete.begin();it!=m_mGiftTemplete.end();it++)
	{
		ST_GIFTTEMPLETEDATA* pstGiftTmpData = it->second;
		if(!pstGiftTmpData)
			continue;

		//SAFE_RELEASE(pstGiftTmpData->iRes);
		MAP_GIFTTEMP_TYPE::iterator iter;
		MAP_GIFTTEMP_TYPE* pGiftTempMap = pstGiftTmpData->m_pGiftTempMap;
		for(iter = pGiftTempMap->begin();iter != pGiftTempMap->end();++iter)
		{
			IRecord* pRecord = iter->second;
			pRecord->Release();
		}
		delete pstGiftTmpData->m_pGiftTempMap;
		SAFE_DELETE(pstGiftTmpData);
	}
	m_mGiftTemplete.clear();
}

//------------------------------
bool CGiftTempleteManager::Create(IDatabase* pDb)
{
	CHECKF(pDb);
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s ",_TBL_GIFTTEMPLETMANAGER);

	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
	CHECKF(pRes);
	for(int i = 0; i < pRes->RecordCount(); i++,pRes->MoveNext())
	{
		OBJID giftTmpTypeID = pRes->GetInt(EM_GIFTTEMPLETEMNGDATA_id);
		LPCTSTR szTableName = pRes->GetStr(EM_GIFTTEMPLETEMNGDATA_tableName);
		LPCTSTR szDesc = pRes->GetStr(EM_GIFTTEMPLETEMNGDATA_desc);


		sprintf(szSQL,"SELECT * FROM %s ",szTableName);
		IRecordset* pResTemp = pDb->CreateNewRecordset(szSQL);
		IF_NOT(pResTemp)//空表,或未查询成功
		{
			LOGERROR("can't find giftTempleteTable:[%s]!",szTableName);
			continue;
		}
		MAP_GIFTTEMP_TYPE* pGiftTempMap = new MAP_GIFTTEMP_TYPE;
		OBJID id ;
		for(int j =0;j<pResTemp->RecordCount();j++)
		{
			//创建record
			IRecord* pRecord = pResTemp->CreateNewRecord();
			id = pRecord->LoadInt("id");
			pGiftTempMap->insert(make_pair(id,pRecord));
			pResTemp->MoveNext();
		}
		pResTemp->Release();
		//map
		ST_GIFTTEMPLETEDATA* pstGiftTmpData = new ST_GIFTTEMPLETEDATA;
		pstGiftTmpData->id = giftTmpTypeID;
		pstGiftTmpData->m_pGiftTempMap = pGiftTempMap;

		m_mGiftTemplete.insert(MAP_GIFTTEMPLETE::value_type(giftTmpTypeID,pstGiftTmpData));
	}
	pRes->Release();
	return true;
}

//----------------------------------
IRecord* CGiftTempleteManager::GetInfo(OBJID tmpType,OBJID tmpIdx)//通过模板type和idx找到单行记录
{
	//CHECKF(tmpType && tmpIdx);
	MAP_GIFTTEMPLETE::iterator it = m_mGiftTemplete.find(tmpType);
	if(it != m_mGiftTemplete.end())//有此模板
	{
		ST_GIFTTEMPLETEDATA* pGiftTmpDataTemp = it->second;
		CHECKF(pGiftTmpDataTemp);

		MAP_GIFTTEMP_TYPE* pGiftTempMap = pGiftTmpDataTemp->m_pGiftTempMap;
		CHECKF(pGiftTempMap);
		
		MAP_GIFTTEMP_TYPE::iterator iter = pGiftTempMap->find(tmpIdx);
		if(iter!=pGiftTempMap->end())
		{
			//找到了
			return iter->second;
		}
		
//		int nRecordCount = iResTemp->RecordCount();
//
//		if(nRecordCount > 0)
//			iResTemp->MoveFirst();
//
//		for(int i = 0; i < nRecordCount; i++)
//		{        
//			int nID = iResTemp->LoadInt("id");
////			if(nID == tmpIdx)
////				return iResTemp->CreateNewConstRecord(tmpIdx);
//			//修改天赋日志问题
//			if(nID == tmpIdx)
//				return iResTemp->CreateNewRecord(tmpIdx);
//
//			iResTemp->MoveNext();
// 		}

//		return iResTemp->CreateNewConstRecord(tmpIdx);
		
	}
	LOGERROR("gift error tmpType =%d tmpIdx=%d ", tmpType, tmpIdx);

	return NULL;
}