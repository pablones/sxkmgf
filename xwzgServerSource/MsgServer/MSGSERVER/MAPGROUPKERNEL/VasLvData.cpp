#include "VasLvData.h"
#include "MapGroup.h"

char	szVaslvTable[] = _TBL_VASLV;
//char    szobjId[]   = "obj_id";
CVasLvData::CVasLvData()
{
	m_pData		= NULL;
//	m_pRes		= NULL;
	m_bNeedUpdate = false;
}

CVasLvData::~CVasLvData()
{
	if(m_pData)
		m_pData->Release();
}

bool CVasLvData::SaveInfo(void)
{
	CHECKF(m_pData);

//	if(m_bNeedUpdate)
	{
		m_pData->Update();
//		m_bNeedUpdate = false;
	}

	OnSaveInfo();
	return true;
}

bool CVasLvData::DeleteRecord(void)
{
	return m_pData->DeleteRecord();
}

bool CVasLvData::Create(OBJID accountid,OBJID objid, IDatabase* pDb)
{
	CHECKF(!m_pData);
	CHECKF(pDb);
	
	m_pData = CGameVaslvData::CreateNew();
	IF_NOT(m_pData)
		return false;

	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE OBJ_ID = %u LIMIT 1", _TBL_VASLV, objid);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
	if(pRes)
	{
		if(pRes->RecordCount() < 1)
		{
			SAFE_RELEASE(pRes);
			SQLBUF szSQL_exe;
			sprintf(szSQL_exe, "INSERT INTO %s SET ACCOUNT_ID=%u,OBJ_ID=%u", _TBL_VASLV, accountid,objid);
			if( !pDb->ExecuteSQL(szSQL_exe) )
				return false;
			pRes = pDb->CreateNewRecordset(szSQL);
		}
	}
	else
	{
		SQLBUF szSQL_exe;
		sprintf(szSQL_exe, "INSERT INTO %s SET ACCOUNT_ID=%u,OBJ_ID=%u", _TBL_VASLV, accountid,objid);
		if( !pDb->ExecuteSQL(szSQL_exe) )
			return false;
		pRes = pDb->CreateNewRecordset(szSQL);
	}
	CHECKF(pRes && (pRes->RecordCount() >= 1));
	
	IF_NOT(m_pData->Create(pRes))
	{
		SAFE_RELEASE(pRes);
		return false;
	}
	SAFE_RELEASE(pRes);
	return true;
}

int	CVasLvData::GetInt(VASLVDATA idx)
{
	return m_pData->GetInt(idx);
}

void CVasLvData::SetInt(VASLVDATA idx, int nData, bool bUpdate)
{
	if(nData == GetInt(idx))
		return;
	
	m_pData->SetInt(idx, nData);
	
	if(bUpdate)
		SaveInfo();
}

bool CVasLvData::AppendInfo(UserVasLvInfo* pInfo)
{
	CHECKF(!m_pData);
	DEBUG_TRY

	m_pData = CGameVaslvData::CreateNew();
	IF_NOT(m_pData)
		return false;

	IF_NOT(m_pData->Create(::MapGroup(MSGPORT_MAPGROUP_FIRST)->QueryDataDefault()->GetVaslvData(),pInfo->id))
		return false;

	SetInt(VASLVDATA_ID_, pInfo->id);
	SetInt(VASLVDATA_ACCID, pInfo->account_id);
	SetInt(VASLVDATA_OBJID, pInfo->obj_id);
	SetInt(VASLVDATA_AWARDITEM, pInfo->awarditem);
	SetInt(VASLVDATA_VISCOU, pInfo->viscou);
	SetInt(VASLVDATA_MAXVIS, pInfo->maxvis);
	SetInt(VASLVDATA_VISTIME, pInfo->vistime);
	SetInt(VASLVDATA_VISFLAG, pInfo->visflag);
	SetInt(VASLVDATA_QUESTIONID, pInfo->questionid);
	SetInt(VASLVDATA_VASCOUNT, pInfo->vascount);
	
	m_pData->ClearUpdateFlags();
	DEBUG_CATCH("CVasLvData::AppendInfo")
	return true;
}

bool CVasLvData::LoadInfo(void)
{
	CHECKF(m_pData);
	DEBUG_TRY
	m_Info.id = m_pData->GetInt(VASLVDATA_ID_);
	m_Info.account_id = m_pData->GetInt(VASLVDATA_ACCID);
	m_Info.obj_id = m_pData->GetInt(VASLVDATA_OBJID);
	m_Info.awarditem = m_pData->GetInt(VASLVDATA_AWARDITEM);
	m_Info.viscou = m_pData->GetInt(VASLVDATA_VISCOU);
	m_Info.maxvis = m_pData->GetInt(VASLVDATA_MAXVIS);
	m_Info.vistime = m_pData->GetInt(VASLVDATA_VISTIME);
	m_Info.visflag = m_pData->GetInt(VASLVDATA_VISFLAG);
	m_Info.questionid = m_pData->GetInt(VASLVDATA_QUESTIONID);
	m_Info.vascount = m_pData->GetInt(VASLVDATA_VASCOUNT);
	DEBUG_CATCH("CVasLvData::LoadInfo")
	return true;
}
