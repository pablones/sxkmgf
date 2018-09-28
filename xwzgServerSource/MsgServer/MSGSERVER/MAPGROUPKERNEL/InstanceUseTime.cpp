// InstanceUseTime.cpp: implementation of the CInstanceUseTime class.
//
//////////////////////////////////////////////////////////////////////

#include "InstanceUseTime.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char szInstanceUseTimeTable[] = _TBL_INSTANCEUSETIME;
// char szSqlInstanceUseTime[128] = "";

CInstanceUseTime::CInstanceUseTime()
{
	m_pData = NULL;
	memset(&m_pInfo, 0, sizeof(InstanceUseTimeInfo));
}

CInstanceUseTime::~CInstanceUseTime()
{
	if(m_pData)
	{
		m_pData->Release();
	}

	memset(&m_pInfo, 0, sizeof(InstanceUseTimeInfo));
}

CInstanceUseTime* CInstanceUseTime::CreateNew(IDatabase* pDB, PROCESS_ID idProcess)
{ 
	CInstanceUseTime* ptr=new CInstanceUseTime; 
	if(ptr) 
	{
		ptr->m_pDB = pDB;
		ptr->m_idProcess = idProcess;
	}
	return ptr;
}

bool CInstanceUseTime::Create(IRecordset* pRes)
{
	ASSERT(!m_pData);

	m_pData = CInstanceUseTimeData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pRes))
	{
		return false;
	}
	else
	{
		GetInstanceUseInfo(&m_pInfo);
	}

	return true;
}

bool CInstanceUseTime::Create( const InstanceUseTimeInfo* pInfo, bool bSave)
{
	ASSERT(!m_pData);

	m_pData = CInstanceUseTimeData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(GameDataDefault()->GetInstanceUseData(), pInfo->id))
		return false;

	m_pData->SetInt(CInstanceUseTime_ID, pInfo->id);
	m_pData->SetInt(CInstanceUseTime_USERID, pInfo->nUserId);
	m_pData->SetInt(CInstanceUseTime_INSTANCETYPE, pInfo->nInstanceType);
	m_pData->SetInt(CInstanceUseTime_USETIMES, pInfo->nUserTime);
	m_pData->SetStr(CInstanceUseTime_CREATETIME, pInfo->szCreateTime, 32);
	m_pData->SetInt(CInstanceUseTime_CANUSER,pInfo->nCanUser);
	m_pData->SetInt(CInstanceUseTime_FINISHTIMES,pInfo->nFinishTimes);

	GetInstanceUseInfo(&m_pInfo);

	if(bSave)
	{
		return m_pData->InsertRecord() != ID_NONE;
	}
	else
	{
		m_pData->ClearUpdateFlags();
		return true;
	}
}

void CInstanceUseTime::GetInstanceUseInfo(InstanceUseTimeInfo* pInfo)
{
	if(NULL == m_pData)
		return ;
	
	pInfo->id = m_pData->GetInt(CInstanceUseTime_ID);
	pInfo->nUserId = m_pData->GetInt(CInstanceUseTime_USERID);
	pInfo->nInstanceType = m_pData->GetInt(CInstanceUseTime_INSTANCETYPE);
	pInfo->nUserTime = m_pData->GetInt(CInstanceUseTime_USETIMES);
	pInfo->nCanUser = m_pData->GetInt(CInstanceUseTime_CANUSER);
	pInfo->nFinishTimes = m_pData->GetInt(CInstanceUseTime_FINISHTIMES);
	
	strcpy(pInfo->szCreateTime, m_pData->GetStr(CInstanceUseTime_CREATETIME));
}

void CInstanceUseTime::Destroy()
{
	if(m_pData)
		m_pData->DeleteRecord();
}

void CInstanceUseTime::SetUseTimes(UINT nTimes,char* szTime,bool bClearFinish/* = false*/,bool bUpdate/* =false */)
{	
	CHECK(szTime);
	m_pData->SetInt(CInstanceUseTime_USETIMES, nTimes);
	m_pData->SetInt(CInstanceUseTime_CANUSER, 1);
	m_pData->SetStr(CInstanceUseTime_CREATETIME,szTime,strlen(szTime));
	m_pInfo.nUserTime = nTimes;
	m_pInfo.nCanUser = 1;	
	strcpy(m_pInfo.szCreateTime,szTime);

	if(bClearFinish)
	{
		m_pData->SetInt(CInstanceUseTime_FINISHTIMES,0);
		m_pInfo.nFinishTimes = 0;
	}

	if(bUpdate)
	{
		m_pData->Update();
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}
}

void CInstanceUseTime::SetUse(int nUse,bool bUpdate/* =false */)
{
	m_pData->SetInt(CInstanceUseTime_CANUSER, nUse);
	m_pInfo.nCanUser = nUse;

	if(bUpdate)
	{
		m_pData->Update();
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}
}

void CInstanceUseTime::SetFinishTime(int nTimes,bool bUpdate/* =false*/ )
{
	m_pData->SetInt(CInstanceUseTime_FINISHTIMES, nTimes);
	m_pInfo.nFinishTimes = nTimes;
	
	if(bUpdate)
	{
		m_pData->Update();
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}
}
