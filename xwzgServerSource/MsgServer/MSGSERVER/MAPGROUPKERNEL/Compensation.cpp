// Compensation.cpp: implementation of the CCompensation class.
//
//////////////////////////////////////////////////////////////////////

#include "Compensation.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompensation::CCompensation()
{
	m_pData = NULL;
}

CCompensation::~CCompensation()
{
	if(m_pData)
	{
		m_pData->Release();
	}
}

bool CCompensation::Create(IRecordset* pRes)
{
	ASSERT(!m_pData);
	
	m_pData = CCompensationData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pRes))
	{
		return false;
	}

	
	return true;
}

void CCompensation::Destroy()
{
	if(m_pData)
		m_pData->DeleteRecord();
}
