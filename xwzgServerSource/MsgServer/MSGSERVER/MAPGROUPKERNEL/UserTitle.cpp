// UserTitle.cpp: implementation of the CUserTitle class.
//
//////////////////////////////////////////////////////////////////////

#include "UserTitle.h"
#include "MapGroup.h"
#include "../Network/MsgTitle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserTitle::CUserTitle(PROCESS_ID idProcess, CUser* pUser)
{
	ASSERT (pUser);
	
	m_idProcess = idProcess;
	m_pUser		= pUser;
	
	m_setData	= CTitleSet::CreateNew(true);
}

CUserTitle::~CUserTitle()
{
	if(m_setData)
		m_setData->Release();
}

CUserTitle* CUserTitle::CreateNew(PROCESS_ID idProcess, CUser* pUser)
{
	CHECKF (pUser);
	
	CUserTitle* pUserTitle = new CUserTitle(idProcess, pUser);
	CHECKF(pUserTitle);
	
	pUserTitle->m_idProcess = idProcess;
	pUserTitle->m_pUser		= pUser;
	
	return pUserTitle;
}

// login
//////////////////////////////////////////////////////////////////////
bool CUserTitle::CreateAll()
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	CHECKF(m_setData);
	CHECKF(m_pUser);

	char szSQL[256]="";
	sprintf(szSQL,"SELECT * FROM %s WHERE ownerid=%u", _TBL_TITLE, m_pUser->GetID());

	CHECKF(m_setData->Create(szSQL, Database()));

	return true;
}

////////////////////////////////////////////////////////////////////
int CUserTitle::SendInfoToClient()
{
	int nAmount = 0;
	CMsgTitle msg;
	if (!msg.Create(_TITLE_INIT, m_pUser->GetID()))
		return 0;
	for(int i = m_setData->GetAmount()-1; i >= 0; i--)
	{
		CUserTitleData* pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetInt(TITLEDATA_TYPE))
		{
			//send to client
			msg.Append(pData->GetInt(TITLEDATA_TYPE), pData->GetInt(TITLEDATA_LEVEL));
			nAmount ++;
		}
	}
	m_pUser->SendMsg(&msg);
	return nAmount;
}

bool CUserTitle::SaveInfo()
{
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		CUserTitleData* pData = m_setData->GetObjByIndex(i);
		IF_OK(pData)
		{
			pData->Update();
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////
CUserTitleData* CUserTitle::FindTitle(int nType)
{
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		CUserTitleData* pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetInt(TITLEDATA_TYPE) == nType)
			return pData;
	}
	return NULL;
}

OBJID CUserTitle::AddTitle(int nType, int nLev)
{
	if (!m_pUser)
		return ID_NONE;
	CTitleTypeData* pType = CUserTitle::FindTitleType(nType);
	if (!pType)
		return ID_NONE;
	if (nLev==0)
		return ID_NONE;
	CUserTitleData* pTitleData = FindTitle(nType);
	if(pTitleData)
	{
		return ID_NONE;
	}
	OBJID idTItle = ID_NONE;

	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT id FROM %s WHERE ownerid=%u && type=%d", _TBL_TITLE, m_pUser->GetID(), nType);
	::CAutoPtr<IRecordset> pRes = _Database(PID)->CreateNewRecordset(szSQL);
	if(pRes)
	{
		return ID_NONE;
	}
	CUserTitleData* pData = NULL;
	pData = CUserTitleData::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GameDataDefault()->GetTitleData(), idTItle))
		return ID_NONE;

	pData->SetInt(TITLEDATA_OWNERID, m_pUser->GetID());
	pData->SetInt(TITLEDATA_TYPE, nType);
	pData->SetInt(TITLEDATA_LEVEL, nLev);
	

	if(idTItle != ID_NONE)
	{
		pData->Update();
	}
	else
	{
		idTItle = pData->InsertRecord();
	}
	
	m_setData->AddObj(pData);
	CMsgTitle msg;
	if (msg.Create(_TITLE_ADD, m_pUser->GetID()))
		msg.Append(nType, nLev);
	m_pUser->SendMsg(&msg);
	return idTItle;
}
//////////////////////////////////////////////////////////////////////
bool CUserTitle::TitleUpLevel(int nType)
{
	if (!m_pUser)
		return false;
	CTitleTypeData* pType = CUserTitle::FindTitleType(nType);
	if (!pType)
		return false;
	CUserTitleData* pTitleData = FindTitle(nType);
	if(!pTitleData)
	{
		return false;
	}

	int	nNewLevel = pTitleData->GetInt(TITLEDATA_LEVEL) + 1;
	if (nNewLevel>pType->GetMaxLevel())
		return false;
	pTitleData->SetInt(TITLEDATA_LEVEL, nNewLevel);
	pTitleData->Update();
	
	CMsgTitle msg;
	if (msg.Create(_TITLE_CHGLEV, m_pUser->GetID()))
		msg.Append(nType, nNewLevel);
	m_pUser->SendMsg(&msg);
	if (m_pUser->GetUseTitle()/100 == nType)
		m_pUser->SetUseTitle(0, 0);
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUserTitle::ChangeTitleLevel(int nType, int nLev)
{
	if (!m_pUser)
		return false;
	CTitleTypeData* pType = CUserTitle::FindTitleType(nType);
	if (!pType)
		return false;
	CUserTitleData* pTitleData = FindTitle(nType);
	if(!pTitleData)
	{
		return false;
	}
	
	if (nLev > pType->GetMaxLevel())
		return false;
	pTitleData->SetInt(TITLEDATA_LEVEL, nLev);
	pTitleData->Update();
	
	CMsgTitle msg;
	if (msg.Create(_TITLE_CHGLEV, m_pUser->GetID()))
		msg.Append(nType, nLev);
	m_pUser->SendMsg(&msg);
	if (m_pUser->GetUseTitle()/100 == nType)
		m_pUser->SetUseTitle(0, 0);
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUserTitle::DelTitleByType(int nType)
{
	CUserTitleData* pTitleData = FindTitle(nType);
	if(!pTitleData)
		return false;

	ASSERT(pTitleData->DeleteRecord());

	m_setData->DelObj(pTitleData->GetID());

	CMsgTitle msg;
	if (msg.Create(_TITLE_DELTE, m_pUser->GetID()))
		msg.Append(nType, 0);
	m_pUser->SendMsg(&msg);
	if (m_pUser->GetUseTitle()/100 == nType)
		m_pUser->SetUseTitle(0, 0);
	return true;
}
//////////////////////////////////////////////////////////////////////
int CUserTitle::GetTitleLev(int nType)
{
	CUserTitleData* pTitleData = FindTitle(nType);
	if(!pTitleData)
		return 0;
	return pTitleData->GetInt(TITLEDATA_LEVEL);
}

//////////////////////////////////////////////////////////////////////
CTitleTypeData* CUserTitle::FindTitleType(int nType)
{
	for(int i = 0; i < TitleTypeSet()->GetAmount(); i++)
	{
		CTitleTypeData* pType = TitleTypeSet()->GetObjByIndex(i);
		if(pType && pType->GetInt(TITLETYPEDATA_TYPE) == nType)
			return pType;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////
//goto 检查是否可以设为使用称号
bool CUserTitle::IsValidTitle(int nType, int nLev)
{
	if (nType==0 && nLev==0)//称号设为隐藏
		return true;
	if (!FindTitle(nType))
		return false;
	CTitleTypeData* pType = FindTitleType(nType);
	if (!pType)
		return false;
	if (nLev > pType->GetMaxLevel())
		return false;
	if (GetTitleLev(nType)<nLev)
		return false;
	if (GetTitleLev(nType)>nLev && pType->GetExtend()==0)//冲突类称号只能使用最大等级
		return false;
	return true;
}

bool CUserTitle::GetInfoByIndex(ST_TITLE_DATA* pInfo, int idx)
{
	CHECKF(pInfo);
	CHECKF(idx >= 0 && idx < m_setData->GetAmount());
	
	CUserTitleData* pData = m_setData->GetObjByIndex(idx);
	IF_OK(pData)
	{
		pInfo->idTitle = pData->GetInt(TITLEDATA_ID_);
		pInfo->idOwner = pData->GetInt(TITLEDATA_OWNERID);
		pInfo->usType  = pData->GetInt(TITLEDATA_TYPE);
		pInfo->usLevel = pData->GetInt(TITLEDATA_LEVEL);
		return true;
	}
	return false;
}

bool CUserTitle::AppendTitle(const ST_TITLE_DATA* pInfo)
{
	CHECKF(pInfo);

	CUserTitleData* pData = CUserTitleData::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GameDataDefault()->GetTitleData(), pInfo->idTitle))
		return false;
	
	pData->SetInt(TITLEDATA_OWNERID, pInfo->idOwner);
	pData->SetInt(TITLEDATA_TYPE, pInfo->usType);
	pData->SetInt(TITLEDATA_LEVEL, pInfo->usLevel);
	pData->ClearUpdateFlags();
	m_setData->AddObj(pData);
	return true;
}
//////////////////////////////////////////////////////////////////////
//goto 操作离线用户的称号,用于帮派等
bool CUserTitle::AddOrSetTitle(OBJID idUser, int nType, int nLev, IDatabase* pDb)
{
	if (idUser==ID_NONE || nLev==0 || nType==0)
		return false;
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE ownerid=%u AND type=%d", _TBL_TITLE, idUser, nType);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
	if (pRes)
	{
		int nLevel = pRes->LoadInt("level");
		pRes->Release();
		if (nLevel == nLev)
			return false;
		else
		{
			sprintf(szSQL, "UPDATE %s SET level=%d WHERE ownerid=%u AND type=%d LIMIT 1", _TBL_TITLE, nLev, idUser, nType);
			if (pDb->ExecuteSQL(szSQL))
			{
				sprintf(szSQL, "UPDATE %s SET use_title=%d WHERE id=%u LIMIT 1", _TBL_USER, 0, idUser);
				if (pDb->ExecuteSQL(szSQL))
					return true;
			}
			else
				return false;
		}
	}
	else
	{
		sprintf(szSQL, "INSERT %s SET ownerid=%u,type=%d,level=%d", _TBL_TITLE, idUser, nType, nLev);
		if (pDb->ExecuteSQL(szSQL))
			return true;
		else
			return false;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
bool CUserTitle::DelOffLineUserTitle(OBJID idUser, int nType, IDatabase* pDb)
{
	if (idUser==ID_NONE || nType==0)
		return false;
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE ownerid=%u AND type=%d", _TBL_TITLE, idUser, nType);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
	if (pRes)
	{
		if (pRes->DeleteRecord())
		{
			pRes->Release();
			sprintf(szSQL, "UPDATE %s SET use_title=%d WHERE id=%u LIMIT 1", _TBL_USER, 0, idUser);
			if (pDb->ExecuteSQL(szSQL))
				return true;
		}
		else
			pRes->Release();
	}
	return false;
}