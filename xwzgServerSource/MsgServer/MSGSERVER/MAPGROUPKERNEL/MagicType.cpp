// MagicType.cpp: implementation of the CMagicType class.
//
//////////////////////////////////////////////////////////////////////

#include "MagicType.h"
#include "MapGroup.h"
#include "UserTable.h"

MYHEAP_IMPLEMENTATION(CMagicTypeData, s_heap)
//MYHEAP_IMPLEMENTATION(CMagicType, s_heap)


char	szMagicTypeTable[] = _TBL_MAGICTYPE;
//////////////////////////////////////////////////////////////////////
// MagicTypeData
bool CMagicTypeData::Create(IRecordset* pRes/*, IDatabase* pDb*/ )
{
	IF_NOT (pRes)
		return false;

	m_pData = CGameMagicTypeData::CreateNew();
	IF_NOT (m_pData)
		return false;

	if (!m_pData->Create(pRes))
	{
		m_pData->Release();
		return false;
	}

	return true;
}

