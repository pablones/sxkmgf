// UserTitleData.cpp: implementation of the CUserTitleData class.
//
//////////////////////////////////////////////////////////////////////

#include "UserTitleData.h"

char szTitleTable[] = _TBL_TITLE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserTitleData::CUserTitleData()
{
	m_pData = NULL;
}

CUserTitleData::~CUserTitleData()
{
	SAFE_RELEASE (m_pData);
}

//////////////////////////////////////////////////////////////////////
bool CUserTitleData::Create(IRecordset* pRecordset/*,IDatabase* pDb*/)
{
	m_pData	= CTitleDataBase::CreateNew();
	if (m_pData && m_pData->Create(pRecordset))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUserTitleData::Create(IRecordset* pDefaultRecord, OBJID idNewKey)
{
	m_pData	= CTitleDataBase::CreateNew();
	if (m_pData && m_pData->Create(pDefaultRecord, idNewKey))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUserTitleData::Create(ST_TITLE_DATA* pTitleData)
{
	CHECKF (pTitleData);
	if (pTitleData->idTitle	== ID_NONE)
		return false;
	
	memcpy(&m_data, pTitleData, sizeof(ST_TITLE_DATA));
	return true;
}

//////////////////////////////////////////////////////////////////////
int CUserTitleData::GetInt(TITLEDATA nDataName)
{
	CHECKF (m_pData);

	return m_pData->GetInt(nDataName);
}

//////////////////////////////////////////////////////////////////////
void CUserTitleData::SetInt(TITLEDATA idx, int nData)
{
	switch (idx)
	{
	case TITLEDATA_OWNERID:
		m_data.idOwner	= nData;
		break;
	case TITLEDATA_TYPE:
		m_data.usType	= nData;
		break;
	case TITLEDATA_LEVEL:
		m_data.usLevel	= nData;
		break;
	default:
		::LogSave("Invalid TITLEDATA [%d]", idx);
		break;
	}
	m_pData->SetInt(idx, nData);
}