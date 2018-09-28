// TreasureData.cpp: implementation of the CTreasureData class.
//
//////////////////////////////////////////////////////////////////////

#include "OnlineGiftData.h"

char szGiftTable[] = _TBL_ONLINEGIFT;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COnlineGiftData::COnlineGiftData()
{
	m_pData = NULL;
}

COnlineGiftData::~COnlineGiftData()
{
	SAFE_RELEASE(m_pData);
}
//////////////////////////////////////////////////////////////////////
bool COnlineGiftData::Create(IRecordset* pRecordset/*,IDatabase* pDb*/)
{
	m_pData	= CGameGiftData::CreateNew();
	if (m_pData && m_pData->Create(pRecordset))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}
//////////////////////////////////////////////////////////////////////
bool COnlineGiftData::Create(IRecordset* pDefaultRecord, OBJID idNewKey)
{
	m_pData	= CGameGiftData::CreateNew();
	if (m_pData && m_pData->Create(pDefaultRecord, idNewKey))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}
//////////////////////////////////////////////////////////////////////
int COnlineGiftData::GetInt(GIFTDATA nDataName)
{
	CHECKF (m_pData);
	return m_pData->GetInt(nDataName);
}
//////////////////////////////////////////////////////////////////////
void COnlineGiftData::SetInt(GIFTDATA idx, int nData)
{
	if (nData == GetInt(idx))
	{
		return;
	}
	CHECK (m_pData);
	m_pData->SetInt(idx, nData);
}
