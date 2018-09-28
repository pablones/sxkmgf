// UserJinMeiTypeData.cpp: implementation of the CUserJinMeiTypeData class.
//
//////////////////////////////////////////////////////////////////////

#include "UserJinMeiTypeData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char	szJinmeiTypeTable[] = _TBL_JINMEITYPE;

bool CJinmeiTypeData::Create(IRecordset* pRes)
{
	IF_NOT (pRes)
		return false;
	
	m_pData = CGameJinmeiTypeData::CreateNew();
	IF_NOT (m_pData)
		return false;
	
	if (!m_pData->Create(pRes))
	{
		SAFE_RELEASE(m_pData);
		return false;
	}
	
	return true;
}