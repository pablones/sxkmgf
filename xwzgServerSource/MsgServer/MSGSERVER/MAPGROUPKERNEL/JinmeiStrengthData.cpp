// JinmeiStrengthData.cpp: implementation of the CJinmeiStrengthData class.
//
//////////////////////////////////////////////////////////////////////

#include "JinmeiStrengthData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char	szJinmeiStengthTable[] = _TBL_JINMEISTRENGTHTYPE;
char    szJinmeiEffectTable[]  = _TBL_JINMEIEFFCTTYPE;

bool CJinmeiStrengthData::Create(IRecordset* pRes)
{
	IF_NOT (pRes)
		return false;
	
	m_pData = CGameJinmeiStrengthData::CreateNew();
	IF_NOT (m_pData)
		return false;
	
	if (!m_pData->Create(pRes))
	{
		SAFE_RELEASE(m_pData);
		return false;
	}
	
	return true;
}

bool CJinmeiEffectData::Create( IRecordset* pRes )
{
	IF_NOT (pRes)
		return false;
	
	m_pData = CGameJinmeiEffectData::CreateNew();
	IF_NOT (m_pData)
		return false;
	
	if (!m_pData->Create(pRes))
	{
		SAFE_RELEASE(m_pData);
		return false;
	}
	
	return true;
}