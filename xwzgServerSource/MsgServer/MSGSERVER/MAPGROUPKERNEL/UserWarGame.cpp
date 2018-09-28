// UserWarGame.cpp: implementation of the CUserWarGame class.
//
//////////////////////////////////////////////////////////////////////

#include "UserWarGame.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserWarGame::CUserWarGame(PROCESS_ID idProcess, CUser* pUser)
{
	ASSERT(pUser);

	m_pUser = pUser;
	m_idProcess = idProcess;
	memset(&m_Info, 0, sizeof(UserWarGameInfo));
	m_pRes	= NULL;
	m_bNeedUpdate	= false;
}

CUserWarGame::~CUserWarGame()
{
	if(m_pRes)
		SAFE_RELEASE(m_pRes);
}

CUserWarGame* CUserWarGame::CreateNew(PROCESS_ID idProcess, CUser* pUser)
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess));
	CHECKF(pUser);
	
	CUserWarGame* pWarGame = new CUserWarGame(idProcess, pUser);
	CHECKF(pWarGame);
	
	return pWarGame;
}

bool CUserWarGame::Create(IDatabase* pDb)
{
	CHECKF(m_pUser);
	CHECKF(!m_pRes);
	CHECKF(pDb);

	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE id=%u LIMIT 1", _TBL_USERWARGAME, m_pUser->GetID());
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);	
	if(pRes)
	{
		if(pRes->RecordCount() < 1)
		{
			SAFE_RELEASE(pRes);
			return InsertNewRecord(pDb);
		}
		m_pRes = pRes->CreateNewRecord();
		SAFE_RELEASE(pRes);		
		
		return LoadInfo();
	}
	else
		return InsertNewRecord(pDb);
	return false;
}

bool CUserWarGame::InsertNewRecord(IDatabase* pDb)
{
	CHECKF(m_pUser);
	CHECKF(pDb);
	CHECKF(!m_pRes);

	SQLBUF szSQL;
	sprintf(szSQL, "INSERT INTO %s SET id=%u", _TBL_USERWARGAME, m_pUser->GetID());
	pDb->ExecuteSQL(szSQL);

	sprintf(szSQL, "SELECT * FROM %s WHERE id=%u LIMIT 1", _TBL_USERWARGAME, m_pUser->GetID());
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);	
	if(pRes)
	{
		m_pRes = pRes->CreateNewRecord();
		SAFE_RELEASE(pRes);
		
		return LoadInfo();
	}
	return false;
}

bool CUserWarGame::LoadInfo()
{
	if (!m_pRes) 
		return false;

	if (m_Info.idUser != ID_NONE)
		return true;

	m_Info.idUser		= m_pRes->LoadDWord( "id" );
	m_Info.nArenaScore	= m_pRes->LoadDWord( "arena_score" );
	m_Info.nArenaDayMask= m_pRes->LoadDWord( "arena_daymask" );

	return true;
}

bool CUserWarGame::SaveInfo()
{
	if (!m_pRes)
		return false;
	
	if(m_bNeedUpdate)
	{
		m_pRes->UpdateRecord();
		m_bNeedUpdate = false;
	}
	
	return true;
}

void CUserWarGame::SetArenaScore(UINT nScore, bool bUpdate)
{
	if(m_Info.nArenaScore == nScore)
		return;
	
	m_Info.nArenaScore = nScore;
	if (m_pRes)
	{
		m_pRes->SetDWord("arena_score", m_Info.nArenaScore);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserWarGame::SetWarGameMask(UINT nData, bool bUpdate)
{
	if(m_Info.nArenaDayMask == nData)
		return;
	
	m_Info.nArenaDayMask = nData;
	if (m_pRes)
	{
		m_pRes->SetDWord("arena_daymask", m_Info.nArenaDayMask);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserWarGame::AddWarGameMask(int nType)
{
	switch(nType)
	{
	case 1:
		SetWarGameMask(m_Info.nArenaDayMask | WARGAME_MASK_ARENA);
		break;
	}
}

void CUserWarGame::AppendInfo(UserWarGameInfo* pInfo)
{
	CHECK(pInfo);

	memcpy(&m_Info, pInfo, sizeof(UserWarGameInfo));
	m_pRes = GameDataDefault()->GetWarGameData()->CreateNewRecord(pInfo->idUser);
	CHECK(m_pRes);
}