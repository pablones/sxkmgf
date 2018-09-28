// UserJinMeiSysData.cpp: implementation of the CUserJinMeiSysData class.
//
//////////////////////////////////////////////////////////////////////

#include "UserJinMeiSysData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "UserJinMeiSysData.h"

char szUserJinmeidataTable[] = _TBL_JINMEIUSER;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserJinMeiSysData::CUserJinMeiSysData()
{
	m_pData = NULL;
}

CUserJinMeiSysData::~CUserJinMeiSysData()
{
	SAFE_RELEASE(m_pData);
}
//////////////////////////////////////////////////////////////////////
bool CUserJinMeiSysData::Create(IRecordset* pRecordset)
{
	m_pData	= CGameUserJinmeiData::CreateNew();
	if (m_pData && m_pData->Create(pRecordset))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}

bool CUserJinMeiSysData::Create(IRecordset* pDefaultRecord,JinmeiInfoStruct* pInfo,bool bInsert)
{
	m_pData	= CGameUserJinmeiData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pDefaultRecord, pInfo->id))
		return false;
	
	m_pData->SetInt(USERJINMEISYSDATA_IDUSER,pInfo->idUser);
	m_pData->SetInt(USERJINMEISYSDATA_TYPE,pInfo->nType);
	m_pData->SetInt(USERJINMEISYSDATA_LEV,pInfo->nLev); 
	m_pData->SetInt(USERJINMEISYSDATA_STENGTHLEV,pInfo->nStrLev);
	m_pData->SetInt(USERJINMEISYSDATA_STARTTIME,pInfo->dwStartTime);
	m_pData->SetInt(USERJINMEISYSDATA_USETIME,pInfo->dwUserTime);
	m_pData->SetInt(USERJINMEISYSDATA_LEARNSTATE,pInfo->nLeanState);
	m_pData->SetInt(USERJINMEISYSDATA_ADDQUEUETIME,pInfo->addQueueTime);
	
	if(bInsert)
	{
		IF_NOT(m_pData->InsertRecord())
			return false;
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUserJinMeiSysData::Create(IRecordset* pDefaultRecord, OBJID idNewKey)
{
	m_pData	= CGameUserJinmeiData::CreateNew();
	if (m_pData && m_pData->Create(pDefaultRecord, idNewKey))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}
//////////////////////////////////////////////////////////////////////
int CUserJinMeiSysData::GetInt(USERJINMEISYSDATA nDataName)
{
	CHECKF (m_pData);
	
	return m_pData->GetInt(nDataName);
}
//////////////////////////////////////////////////////////////////////
void CUserJinMeiSysData::SetInt(USERJINMEISYSDATA idx, int nData)
{
	m_pData->SetInt(idx, nData);
}

bool CUserJinMeiSysData::GetInfo(JinmeiInfoStruct& info)
{
	CHECKF(m_pData);
	info.id = m_pData->GetID();
	info.idUser			= m_pData->GetInt(USERJINMEISYSDATA_IDUSER);
	info.nType			= m_pData->GetInt(USERJINMEISYSDATA_TYPE);
	info.nLev			= m_pData->GetInt(USERJINMEISYSDATA_LEV);
	info.nStrLev		= m_pData->GetInt(USERJINMEISYSDATA_STENGTHLEV);
	info.dwStartTime	= m_pData->GetInt(USERJINMEISYSDATA_STARTTIME);
	info.dwUserTime		= m_pData->GetInt(USERJINMEISYSDATA_USETIME);
	info.nLeanState		= m_pData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
	info.addQueueTime	= m_pData->GetInt(USERJINMEISYSDATA_ADDQUEUETIME);
	return true;
}