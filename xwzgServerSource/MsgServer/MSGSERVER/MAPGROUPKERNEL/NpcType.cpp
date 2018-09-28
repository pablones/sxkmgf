// NpcType.cpp: implementation of the CNpcType class.
//
//////////////////////////////////////////////////////////////////////

#include "NpcType.h"
#include "MapGroup.h"
#include "UserTable.h"

char	szNpcTypeTable[] = _TBL_NPC;


bool CStaticNpcType::Create(IRecordset* pRes)
{
	IF_NOT (pRes)
		return false;
	
	m_pDataNpc = CNpcTypeData::CreateNew();
	IF_NOT (m_pDataNpc)
		return false;
	
	if (!m_pDataNpc->Create(pRes))
	{
		m_pDataNpc->Release();
		return false;
	}
	
	return true;
}

MYHEAP_IMPLEMENTATION(CStaticNpcType, s_heap)

