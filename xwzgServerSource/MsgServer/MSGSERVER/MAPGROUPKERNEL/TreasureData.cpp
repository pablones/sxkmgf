// TreasureData.cpp: implementation of the CTreasureData class.
//
//////////////////////////////////////////////////////////////////////

#include "TreasureData.h"

char szTreasureTable[] = _TBL_TREASURE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreasureData::CTreasureData()
{
	m_pData = NULL;
}

CTreasureData::~CTreasureData()
{
	SAFE_RELEASE(m_pData);
}
//////////////////////////////////////////////////////////////////////
bool CTreasureData::Create(IRecordset* pRecordset/*,IDatabase* pDb*/)
{
	m_pData	= CGameTreasureData::CreateNew();
	if (m_pData && m_pData->Create(pRecordset))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}
//////////////////////////////////////////////////////////////////////
bool CTreasureData::Create(IRecordset* pDefaultRecord, OBJID idNewKey)
{
	m_pData	= CGameTreasureData::CreateNew();
	if (m_pData && m_pData->Create(pDefaultRecord, idNewKey))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}
//////////////////////////////////////////////////////////////////////
int CTreasureData::GetInt(TREASUREDATA nDataName)
{
	CHECKF (m_pData);
	
	return m_pData->GetInt(nDataName);
}
//////////////////////////////////////////////////////////////////////
void CTreasureData::SetInt(TREASUREDATA idx, int nData)
{
	m_pData->SetInt(idx, nData);
}
