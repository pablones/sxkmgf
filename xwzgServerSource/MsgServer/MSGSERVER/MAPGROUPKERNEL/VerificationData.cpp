// VerificationData.cpp: implementation of the CVerificationData class.
//
//////////////////////////////////////////////////////////////////////

#include "VerificationData.h"
#include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVerificationData::CVerificationData()
{
	m_idcodmap.clear();
}

CVerificationData::~CVerificationData()
{
	m_idcodmap.clear();
}

//////////////////////////////////////////////////////////////////////
bool CVerificationData::Create(IDatabase* pDb)
{
	char szSQL[256]="";
	sprintf(szSQL,"SELECT * FROM %s",_TBL_VERIFICATION_RANDSHOP);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);	
	if(pRes&&pRes->RecordCount()>0)
	{
		for (int i = 0; i < pRes->RecordCount(); i++,pRes->MoveNext())
		{
			m_idcodmap.insert(map<OBJID,string>::value_type(pRes->GetInt(0),pRes->GetStr(1)));
			m_idlist.push_back(pRes->GetInt64(0));
		}
		pRes->Release();
		return true;
	}
	return false;
}
