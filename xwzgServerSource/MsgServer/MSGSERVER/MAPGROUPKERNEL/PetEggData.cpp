// PetEggData.cpp: implementation of the CPetEggData class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdtctl.h>
#include "PetEggData.h"

char szPeteggTable[] = _TBL_PETEGG;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPetEggData::CPetEggData()
{
	m_pData = NULL;
}

CPetEggData::~CPetEggData()
{
	SAFE_RELEASE(m_pData);
}

bool CPetEggData::Create(IRecordset* pRecordset)
{
	m_pData	= CGamePeteggData::CreateNew();
	if (m_pData && m_pData->Create(pRecordset))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}

bool CPetEggData::Create(IRecordset* pDefaultRecord, OBJID idNewKey)
{
	m_pData	= CGamePeteggData::CreateNew();
	if (m_pData && m_pData->Create(pDefaultRecord, idNewKey))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}

int CPetEggData::GetInt(PETEGGDATA nDataName)
{
	CHECKF (m_pData);
	
	return m_pData->GetInt(nDataName);
}

void CPetEggData::SetInt(PETEGGDATA idx, int nData, bool bUpdate)
{
	if(GetInt(idx) == nData)
		return;

	m_pData->SetInt(idx, nData);
	if(bUpdate)
		m_pData->Update();
}

bool CPetEggData::NeedFlush()
{
	DEBUG_TRY
	int nNextTime = GetInt(PETEGGDATA_NEXTFLUSH);
	CHECKF(nNextTime);

	int nMonth	=  nNextTime / 100000000;
	int nDay	= (nNextTime / 1000000) % 100;
	int nHour	= (nNextTime / 10000) % 100;
	int nMinute = (nNextTime / 100) % 100;
	int nSecond =  nNextTime % 100;

	CTime timeCurr(CTime::GetCurrentTime());
	int nYear = timeCurr.GetYear();
	if(nMonth==12 && timeCurr.GetMonth()!=12)//认为跨年了，不一定对
		nYear--;

	CTime timeFlush(nYear, nMonth, nDay, nHour, nMinute, nSecond);
	CTimeSpan spanTime = timeCurr - timeFlush;
	
	if(spanTime.GetTotalSeconds() > 0)
		return true;
	DEBUG_CATCH("CPetEggData::NeedFlush()")
	return false;
}

LONG CPetEggData::GetNextFlushSeconds()
{
	LONG sec = 0;
	DEBUG_TRY
	int nNextTime = GetInt(PETEGGDATA_NEXTFLUSH);
	CHECKF(nNextTime);
	
	int nMonth	=  nNextTime / 100000000;
	int nDay	= (nNextTime / 1000000) % 100;
	int nHour	= (nNextTime / 10000) % 100;
	int nMinute = (nNextTime / 100) % 100;
	int nSecond =  nNextTime % 100;
	
	CTime timeCurr(CTime::GetCurrentTime());
	int nYear = timeCurr.GetYear();
	if(nMonth==12 && timeCurr.GetMonth()!=12)//认为跨年了，不一定对
		nYear--;
	
	CTime timeFlush(nYear, nMonth, nDay, nHour, nMinute, nSecond);
	CTimeSpan spanTime = timeFlush - timeCurr;
	
	sec = spanTime.GetTotalSeconds();
	if(sec > 0)
		return sec;
	DEBUG_CATCH("CPetEggData::GetFlushSeconds()")
	return 0;
}