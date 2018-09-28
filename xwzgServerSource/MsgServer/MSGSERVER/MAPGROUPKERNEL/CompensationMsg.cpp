// CompensationMsg.cpp: implementation of the CCompensationMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "CompensationMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompensationMsg::CCompensationMsg()
{
	m_pData = NULL;
}

CCompensationMsg::~CCompensationMsg()
{
	if(m_pData)
	{
		m_pData->Release();
	}
}

bool CCompensationMsg::Create(IRecordset* pRes)
{
	ASSERT(!m_pData);
	
	m_pData = CCompensationMsgData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pRes))
	{
		return false;
	}
	
	
	return true;
}

bool CCompensationMsg::Create(IRecord* pDefault,OBJID idOwner, DWORD dwMsgType,const char* szMsg,const char* szTime, bool bInsert/*=true*/)			// false: ²»´æÊý¾Ý¿â
{
	CHECKF(!m_pData);
	CHECKF(idOwner);
	CHECKF(szMsg);
	CHECKF(szTime);
	CHECKF(pDefault);
	

	// data
	m_pData = CCompensationMsgData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pDefault, ID_NONE))
		return false;
	
	
	m_pData->SetInt( COMPENSATIONMSGDATA_USERID,idOwner );
	m_pData->SetInt( COMPENSATIONMSGDATA_TYPE,dwMsgType);
	m_pData->SetStr( COMPENSATIONMSGDATA_WRITETIME,szTime,32);
	m_pData->SetStr( COMPENSATIONMSGDATA_MSG,szMsg,_MAX_STRING);

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

void CCompensationMsg::Destroy()
{
	if(m_pData)
		m_pData->DeleteRecord();
}

