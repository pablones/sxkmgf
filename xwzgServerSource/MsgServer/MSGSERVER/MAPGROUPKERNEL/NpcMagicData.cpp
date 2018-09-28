// NpcMagicData.cpp: implementation of the CNpcMagicData class.
//
//////////////////////////////////////////////////////////////////////

#include "NpcMagicData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


char	szNpcMagic[] = _TBL_MONTERMAGIC;
char	szEudemonMagic[] = _TBL_EUDEMONMAGIC;
char	szSoulMagic[] = _TBL_SOULMAGIC;

//////////////////////////
CNpcMagicData::CNpcMagicData()
{
	m_pData=NULL;
}

CNpcMagicData::~CNpcMagicData()
{
	if(m_pData)
		m_pData->Release();
}

//-----------------------------
bool CNpcMagicData::Create(OBJID id, IDatabase* pDb)
{
	CHECKF(!m_pData);
	CHECKF(pDb);

	// data
	m_pData = CGameDataNpcMagic::CreateNew();
	IF_NOT(m_pData)
		return false;
	IF_NOT(m_pData->Create(id, pDb))
		return false;

	return true;
}
//-----------------------------
bool CNpcMagicData::Create(IRecordset* pRes)
{
	CHECKF(!m_pData);

	// data
	m_pData = CGameDataNpcMagic::CreateNew();
	IF_NOT(m_pData)
		return false;

	IF_NOT(m_pData->Create(pRes))
		return false;

	return true;
}
//-----------------------------
bool CNpcMagicData::DeleteRecord(IDatabase* pDb)
{
	return m_pData->DeleteRecord();
}
//-----------------------------
bool CNpcMagicData::SaveInfo(void)
{
	CHECKF(m_pData);

	m_pData->Update();

	return true;
}
//-----------------------------
int	CNpcMagicData::GetInt(EM_NPCMAGICDATA idx)
{
	return m_pData->GetInt(idx);
}

//-----------------------------
LPCTSTR	CNpcMagicData::GetStr(EM_NPCMAGICDATA idx)
{
	return m_pData->GetStr(idx);
}

//------------------------------
void CNpcMagicData::SetInt(EM_NPCMAGICDATA idx, int nData, bool bUpdate/* = false*/)
{
	m_pData->SetInt(idx, nData);
	if(bUpdate)
		m_pData->Update();
}

//------------------------------
void CNpcMagicData::SetStr(EM_NPCMAGICDATA idx, LPCTSTR szData, int nSize, bool bUpdate/* = false*/)
{
	m_pData->SetStr(idx, szData, nSize);
	if(bUpdate)
		m_pData->Update();
}


//////////////////////////
CEudemonMagicData::CEudemonMagicData()
{
	m_pData=NULL;
}

CEudemonMagicData::~CEudemonMagicData()
{
	if(m_pData)
		m_pData->Release();
}

//-----------------------------
bool CEudemonMagicData::Create(OBJID id, IDatabase* pDb)
{
	CHECKF(!m_pData);
	CHECKF(pDb);
	
	// data
	m_pData = CGameDataEudemonMagic::CreateNew();
	IF_NOT(m_pData)
		return false;
	IF_NOT(m_pData->Create(id, pDb))
		return false;
	
	return true;
}
//-----------------------------
bool CEudemonMagicData::Create(IRecordset* pRes)
{
	CHECKF(!m_pData);
	
	// data
	m_pData = CGameDataEudemonMagic::CreateNew();
	IF_NOT(m_pData)
		return false;
	
	IF_NOT(m_pData->Create(pRes))
		return false;
	
	return true;
}
//-----------------------------
bool CEudemonMagicData::DeleteRecord(IDatabase* pDb)
{
	return m_pData->DeleteRecord();
}
//-----------------------------
bool CEudemonMagicData::SaveInfo(void)
{
	CHECKF(m_pData);
	
	m_pData->Update();
	
	return true;
}
//-----------------------------
int	CEudemonMagicData::GetInt(EM_EUDEMONMAGIC idx)
{
	return m_pData->GetInt(idx);
}

//-----------------------------
LPCTSTR	CEudemonMagicData::GetStr(EM_EUDEMONMAGIC idx)
{
	return m_pData->GetStr(idx);
}

//------------------------------
void CEudemonMagicData::SetInt(EM_EUDEMONMAGIC idx, int nData, bool bUpdate/* = false*/)
{
	m_pData->SetInt(idx, nData);
	if(bUpdate)
		m_pData->Update();
}

//------------------------------
void CEudemonMagicData::SetStr(EM_EUDEMONMAGIC idx, LPCTSTR szData, int nSize, bool bUpdate/* = false*/)
{
	m_pData->SetStr(idx, szData, nSize);
	if(bUpdate)
		m_pData->Update();
}

//////////////////////////
CSoulMagicData::CSoulMagicData()
{
	m_pData=NULL;
}

CSoulMagicData::~CSoulMagicData()
{
	if(m_pData)
		m_pData->Release();
}

//-----------------------------
bool CSoulMagicData::Create(OBJID id, IDatabase* pDb)
{
	CHECKF(!m_pData);
	CHECKF(pDb);
	
	// data
	m_pData = CGameDataSoulMagic::CreateNew();
	IF_NOT(m_pData)
		return false;
	IF_NOT(m_pData->Create(id, pDb))
		return false;
	
	return true;
}
//-----------------------------
bool CSoulMagicData::Create(IRecordset* pRes)
{
	CHECKF(!m_pData);
	
	// data
	m_pData = CGameDataSoulMagic::CreateNew();
	IF_NOT(m_pData)
		return false;
	
	IF_NOT(m_pData->Create(pRes))
		return false;
	
	return true;
}
//-----------------------------
bool CSoulMagicData::DeleteRecord(IDatabase* pDb)
{
	return m_pData->DeleteRecord();
}
//-----------------------------
bool CSoulMagicData::SaveInfo(void)
{
	CHECKF(m_pData);
	
	m_pData->Update();
	
	return true;
}
//-----------------------------
int	CSoulMagicData::GetInt(EM_SOULMAGIC idx)
{
	return m_pData->GetInt(idx);
}

//-----------------------------
LPCTSTR	CSoulMagicData::GetStr(EM_SOULMAGIC idx)
{
	return m_pData->GetStr(idx);
}

//------------------------------
void CSoulMagicData::SetInt(EM_SOULMAGIC idx, int nData, bool bUpdate/* = false*/)
{
	m_pData->SetInt(idx, nData);
	if(bUpdate)
		m_pData->Update();
}

//------------------------------
void CSoulMagicData::SetStr(EM_SOULMAGIC idx, LPCTSTR szData, int nSize, bool bUpdate/* = false*/)
{
	m_pData->SetStr(idx, szData, nSize);
	if(bUpdate)
		m_pData->Update();
}