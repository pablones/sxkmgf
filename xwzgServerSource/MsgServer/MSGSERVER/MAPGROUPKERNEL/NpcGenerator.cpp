// NpcGenerator.cpp: implementation of the CNpcGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "NpcGenerator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char	szGenerator[] = _TBL_GENERATOR;

bool CNpcGenerator::Create(IRecordset* pRes )
{
	IF_NOT (pRes)
		return false;
	
	m_pData = CNpcGenData::CreateNew();
	IF_NOT (m_pData)
		return false;
	
	if (!m_pData->Create(pRes))
	{
		m_pData->Release();
		return false;
	}
	

	if(GetInt(NPCGENDATA_INITSECS)==0)
		m_timeoutRest.SetInterval(GetInt(NPCGENDATA_RESTSECS) + ::RandGet(GetInt(NPCGENDATA_RESTSECS)));
	else
		m_timeoutRest.SetInterval(GetInt(NPCGENDATA_INITSECS));
	m_timeoutRest.Update();

	return true;
}

bool CNpcGenerator::IsTimeOut()
{
	if(m_timeoutRest.IsTimeOut())
	{
		m_timeoutRest.Clear();
		m_timeoutRest.SetInterval(GetInt(NPCGENDATA_RESTSECS) /*+ ::RandGet(GetInt(NPCGENDATA_RESTSECS))*/);
		return true;
	}
	return false;
}

void CNpcGenerator::UpdateTime()
{
	if(!m_timeoutRest.IsActive())
		m_timeoutRest.Update();
}
