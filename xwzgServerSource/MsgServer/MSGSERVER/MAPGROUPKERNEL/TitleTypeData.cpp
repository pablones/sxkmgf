// TitleTypeData.cpp: implementation of the CTitleTypeData class.
//
//////////////////////////////////////////////////////////////////////

#include "TitleTypeData.h"

MYHEAP_IMPLEMENTATION(CTitleTypeData, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char	szTitleTypeTable[] = _TBL_TITLETYPE;

bool CTitleTypeData::Create(IRecordset* pRes/*, IDatabase* pDb*/ )
{
	IF_NOT (pRes)
		return false;
	
	m_pData = CGameTitleTypeData::CreateNew();
	IF_NOT (m_pData)
		return false;
	
	if (!m_pData->Create(pRes))
	{
		m_pData->Release();
		return false;
	}
	
	return true;
}

int CTitleTypeData::GetMaxLevel()
{
	return GetInt(TITLETYPEDATA_MAXLEV);
}

int CTitleTypeData::GetExtend()
{
	return GetInt(TITLETYPEDATA_EXTEND);
}