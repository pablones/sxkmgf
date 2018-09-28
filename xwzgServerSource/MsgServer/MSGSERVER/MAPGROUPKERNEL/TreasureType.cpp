// TreasureType.cpp: implementation of the CTreasureType class.
//
//////////////////////////////////////////////////////////////////////

#include "TreasureType.h"

MYHEAP_IMPLEMENTATION(CTreasureType, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char	szTreasureTypeTable[] = _TBL_TREASURETYPE;

bool CTreasureType::Create(IRecordset* pRes/*, IDatabase* pDb*/ )
{
	IF_NOT (pRes)
		return false;
	
	m_pData = CGameTreasureType::CreateNew();
	IF_NOT (m_pData)
		return false;
	
	if (!m_pData->Create(pRes))
	{
		//m_pData->Release();
		SAFE_RELEASE(m_pData);
		return false;
	}
	
	return true;
}