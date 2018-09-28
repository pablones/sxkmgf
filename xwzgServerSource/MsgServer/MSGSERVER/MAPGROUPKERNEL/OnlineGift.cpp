// Treasure.cpp: implementation of the CTreasure class.
//
//////////////////////////////////////////////////////////////////////

#include "OnlineGift.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COnlineGift::COnlineGift(PROCESS_ID idProcess, CUser* pUser)
{
	ASSERT (pUser);
	m_idProcess = idProcess;
	m_pUser		= pUser;
	m_setData   = NULL;
}

COnlineGift::~COnlineGift()
{
	if(m_setData)
		m_setData->ReleaseByOwner();
}

COnlineGift* COnlineGift::CreateNew(PROCESS_ID idProcess, CUser* pUser)
{
	CHECKF (pUser);
	COnlineGift* pGift = new COnlineGift(idProcess, pUser);
	CHECKF(pGift);
	return pGift;
}

bool COnlineGift::AppendOnlinegiftInfo(const OnlineGiftInfoStruct* pInfo)
{
	CHECKF(!m_setData);
	m_setData = COnlineGiftData::CreateNew();
	CHECKF(m_setData);
	IF_NOT(m_setData->Create(GameDataDefault()->GetOnlineGiftData(), pInfo->id))
		return false;
	SetGift(pInfo->gift1,pInfo->gift2,pInfo->gift3,pInfo->gift4,pInfo->gift5,pInfo->gift6);
	SetTaskMask(pInfo->taskmask);
	SetDate(pInfo->date);
	SetDayAmount(pInfo->amount);
	return true;
}
bool COnlineGift::GetOnlinegiftInfo(OnlineGiftInfoStruct* pInfo)
{
	CHECKF(pInfo);
	CHECKF(m_setData);
	memset(pInfo, 0, sizeof(OnlineGiftInfoStruct));
	pInfo->id			= m_setData->GetID();
	if(m_setData)
	{
		pInfo->userid=m_pUser->GetID();
		pInfo->gift1=m_setData->GetGift1ID();
		pInfo->gift2=m_setData->GetGift2ID();
		pInfo->gift3=m_setData->GetGift3ID();
		pInfo->gift4=m_setData->GetGift4ID();
		pInfo->gift5=m_setData->GetGift5ID();
		pInfo->gift6=m_setData->GetGift6ID();
		pInfo->taskmask=GetTaskMask();
		pInfo->date=GetDate();
		pInfo->amount=GetDayAmount();
	}
	
	return true;
}

// login
//////////////////////////////////////////////////////////////////////
bool COnlineGift::CreateAll()
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	CHECKF(m_pUser);	
	char szSQL[256]="";
	sprintf(szSQL,"SELECT * FROM %s WHERE userid=%u", _TBL_ONLINEGIFT, m_pUser->GetID());
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL);	
	if(pRes)
	{
		if (!m_setData)
			m_setData	= COnlineGiftData::CreateNew();
		CHECKF(m_setData);
		CHECKF(m_setData->Create(pRes));
		pRes->Release();
		return true;
	}
	else
	{
     	AddGifts();
		return true;
	}
}
//////////////////////////////////////////////////////////////////////
bool COnlineGift::SaveInfo()
{
	//COnlineGiftData* pData = m_setData;
	IF_OK(m_setData)
	{
		m_setData->Update();
	}
	return true;
}

bool COnlineGift::AddGifts()
{
// 	COnlineGiftData* pData = NULL;
// 	pData = COnlineGiftData::CreateNew();
// 	CHECKF(pData);
	if (!m_setData)
	{
		m_setData = COnlineGiftData::CreateNew();
		CHECKF(m_setData);
	}
	IF_NOT(m_setData->Create(GameDataDefault()->GetOnlineGiftData(),ID_NONE))
		return false;
	m_setData->SetInt(GIFTDATA_USERID,m_pUser->GetID());
	m_setData->SetGift1ID(0);
	m_setData->SetGift2ID(0);
	m_setData->SetGift3ID(0);
	m_setData->SetGift4ID(0);
	m_setData->SetGift5ID(0);
	m_setData->SetGift6ID(0);
	m_setData->SetTaskMask(0);
	m_setData->SetDate(0);

	m_setData->InsertRecord();
	return true;
}
void COnlineGift::SetTaskMask(int nTaskMask)
{
	if (!m_setData)
	{
		AddGifts();
		SetTaskMask(nTaskMask);
	}
// 	COnlineGiftData* pData = NULL;
// 	pData=m_setData;
// 	CHECK(pData);
	m_setData->SetTaskMask(nTaskMask);
}
void COnlineGift::SetDate(int nDate)
{
	if (!m_setData)
	{
		AddGifts();
		SetDate(nDate);
	}
// 	COnlineGiftData* pData = NULL;
// 	pData=m_setData;
// 	CHECK(pData);
	m_setData->SetDate(nDate);
}
void COnlineGift::SetGift(OBJID Gift1ID, OBJID Gift2ID, OBJID Gift3ID, OBJID Gift4ID, OBJID Gift5ID,OBJID Gift6ID)
{
	if (!m_setData)
	{
		AddGifts();
	}
	ClnGifts();
// 	COnlineGiftData* pData = NULL;
// 	pData=m_setData;
// 	CHECK(pData);
	m_setData->SetGift1ID(Gift1ID);
	m_setData->SetGift2ID(Gift2ID);
	m_setData->SetGift3ID(Gift3ID);
	m_setData->SetGift4ID(Gift4ID);
	m_setData->SetGift5ID(Gift5ID);
	m_setData->SetGift6ID(Gift6ID);
}

void COnlineGift::ClnGifts()
{
	if(m_setData)
	{
// 		COnlineGiftData* pData = NULL;
// 		pData=m_setData;
// 		CHECK(pData);
		m_setData->SetGift2ID(0);
		m_setData->SetGift3ID(0);
		m_setData->SetGift4ID(0);
		m_setData->SetGift5ID(0);
		m_setData->SetGift6ID(0);
	}
}
bool COnlineGift::HasRecord()
{
	if (m_setData)
		return true;
	return false;
}

int COnlineGift::GetTaskMask()
{
	return m_setData->GetTaskMask();
}

OBJID COnlineGift::GetGiftID(int idx)
{
	return m_setData->GetInt(GIFTDATA(idx));
}
int COnlineGift::GetDate()
{
	return m_setData->GetDate();
}
void COnlineGift::AddDayAmount(int nData)
{
	m_setData->AddAmount(nData);
}
void COnlineGift::AddDayAmount()
{
	m_setData->AddAmount();
}
int COnlineGift::GetDayAmount()
{
	return m_setData->GetAmount();
}
void COnlineGift::SetDayAmount(int nData)
{
	m_setData->SetAmount(nData);
}