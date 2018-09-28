// WargameManager.cpp: implementation of the CWargameManager class.
// 
//////////////////////////////////////////////////////////////////////
#include "afxdisp.h"
#include "MAPGROUP.h"
#include "Wargame.h"
#include "MsgWargameSync.h"
#include "WargameManager.h"
#include "MsgWargame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWargameManager::CWargameManager()
{
	m_WarGameList.clear();
}

CWargameManager::~CWargameManager()
{
	for(int i=GetSize()-1; i>=0; i--)
		SAFE_RELEASE(m_WarGameList[i]);
	m_WarGameList.clear();
}

UINT CWargameManager::GetSize()///�õ������ڻ����
{
	return m_WarGameList.size();
}

BOOL CWargameManager::Init(IDatabase* pDB,PROCESS_ID idProcess)
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess));

	m_idProcess=idProcess;
	m_pDB=pDB;

	//��ʼ��ս���
	WARGAME_INFO warinfo = {0};
	CWargame* pWargame = NULL;
	char szSql[256] = "";
	sprintf(szSql, "select * from %s", _TBL_WARGAME);
	IRecordset* pNewRecordset=m_pDB->CreateNewRecordset(szSql);

	if (pNewRecordset)
	{
		int aaa = pNewRecordset->RecordCount();
		for(int i=0; i<pNewRecordset->RecordCount(); ++i, pNewRecordset->MoveNext())
		{
			warinfo.id=pNewRecordset->LoadInt("id");///�õ�ID
			warinfo.nType=pNewRecordset->LoadInt("type");///�õ������
			warinfo.nCamps=pNewRecordset->LoadInt("camps");
			warinfo.nSinginBegin=pNewRecordset->LoadInt("singin_begin");
			warinfo.nSinginEnd=pNewRecordset->LoadInt("singin_end");
			warinfo.nStartTime=pNewRecordset->LoadInt("start_time");
			warinfo.nEndTime=pNewRecordset->LoadInt("end_time");
			warinfo.nLevBegin=pNewRecordset->LoadInt("lev_begin");
			warinfo.nLevEnd=pNewRecordset->LoadInt("lev_end");
			warinfo.nMaxPlayer=pNewRecordset->LoadInt("max_player");
			warinfo.nBaseScore=pNewRecordset->LoadInt("base_score");
			warinfo.nScoreParam=pNewRecordset->LoadInt("score_param");
			
			pWargame=CWargame::CreateNew(warinfo.nType);
			CHECKF(pWargame);
			CHECKF(pWargame->CreateWargame(warinfo, m_idProcess));
			if(pWargame)
				AddWargame(pWargame);
		}
		SAFE_RELEASE(pNewRecordset);
	}
	return GetSize();
}

void CWargameManager::LoadMap()
{
	for(int i=GetSize()-1; i>=0; i--)
	{
		if(m_WarGameList[i])
		{
			DEBUG_TRY
			m_WarGameList[i]->LoadMap();
			DEBUG_CATCH2("CWargameManager::LoadMap()!id=%d", m_WarGameList[i]->GetWarGameID())
		}
	}
}

void CWargameManager::OnTimer(time_t tCurr)///��ʱ��������
{
	///��OnTimer�н�����������ս�������ץ�����������ǵ�OnTimer
	for(int i=GetSize()-1; i>=0; i--)
	{
		if(m_WarGameList[i])
		{
			DEBUG_TRY
			m_WarGameList[i]->OnTimer(tCurr);
			DEBUG_CATCH2("CWargameManager::OnTimer!id=%d", m_WarGameList[i]->GetWarGameID())
		}
	}

}

// void CWargameManager::ProcessMsg(CMsgWargameSync* pMsg)///������Ϣ����
// {
// 	CHECK(pMsg);
// 	CWargame* pWargame=GetWargame(pMsg->GetWargameType());
// //	CHECK(pWargame);
// // 	if(pWargame)
// // 		pWargame->ProcessMsg(pMsg);
// }

bool CWargameManager::AddWargame(CWargame* war)///���һ��ս���
{
	CHECKF(war);

	m_WarGameList.push_back(war);
	return true;
}

CWargame* CWargameManager::GetWargame(OBJID nWargameID)///ͨ��ս���id�õ�һ��ս�������ָ��
{
	CHECKF(nWargameID);

	for(int i=GetSize()-1; i>=0; i--)
	{
		if(m_WarGameList[i] && m_WarGameList[i]->GetWarGameID()== nWargameID)
			return m_WarGameList[i];
	}
	return NULL;
}

bool CWargameManager::DelWargame(CWargame* war)///ɾ��һ��ս���
{
	CHECKF(war);

	for(CWargameList::iterator it=m_WarGameList.begin();it!=m_WarGameList.end();++it)
	{
		if( (*it) == war )
		{
			SAFE_RELEASE(war);
			m_WarGameList.erase(it);
			return true;
		}
	}
	return false;
}

// BOOL CWargameManager::ReplyWargameResult(CUserPtr pUser,UINT nPage)///ս�����������û���Ҫ�����ݷ�����
// {
// 	return TRUE;
// 	CHECKF(pUser);
// 	BOOL hr=false;
// 	char tmpStr[256]={0};
// 	CMsgWargame msg;
// 	msg.m_pInfo->nWargameType=WARGAME_TYPE_RESULT;
// 	msg.m_pInfo->nWargameStatus=WARGAME_TYPE_RESULT_UPDATERESULT;
// 	CString strSql;///��ϲ�ѯ��SQL���
// 	///�õ���ǰ����
// 	strSql.Format(sqlQueryWarGameResult,nPage*21,21);///�ȷ�7����¼��ȥ����
// 	IRecordset* pNewRecordset=m_pDB->CreateNewRecordset(strSql);
// 	if (pNewRecordset)
// 	{
// 		for(int i=0;i<pNewRecordset->RecordCount();++i)
// 		{
// 			///�м�¼����ȡֵȻ��ɾ��
// 			COleDateTime date;
// 			pNewRecordset->LoadString(tmpStr,"date",256);
// 			date.ParseDateTime(tmpStr);
// 			msg.m_pInfo->result[i].dwTime=date.GetYear()*10000+date.GetMonth()*100+date.GetDay();
// 			msg.m_pInfo->result[i].dwRace=pNewRecordset->LoadInt("race");
// 			msg.m_pInfo->result[i].dwScore=pNewRecordset->LoadInt("score");	
// 			pNewRecordset->MoveNext();
// 		}
// 		msg.m_pInfo->nPararm=MAKELONG(nPage,i);
// 		if (i>0)
// 			hr=pUser->SendMsg(&msg);
// 		SAFE_RELEASE(pNewRecordset);
// 	}
// 	return hr;
//}

// BOOL CWargameManager::AddWargameResult(UINT nUserRace,UINT nScore)///��ӻ���ֽ��
// {
// 	BOOL hr=false;
// 	CString strSql;///��ϲ�ѯ��SQL���
// 	///�õ���ǰ����
// 	const COleDateTime& objCurrentDate=COleDateTime::GetCurrentTime();
// 	strSql.Format(sqlGetWarGameResult,nUserRace,objCurrentDate.Format("%Y-%m-%d"));
// 	IRecordset* pNewRecordset=m_pDB->CreateNewRecordset(strSql);
// 	if (pNewRecordset)
// 	{
// 		///�м�¼����ȡֵȻ��ɾ��
// 		nScore+=pNewRecordset->LoadInt("score");
// 		pNewRecordset->DeleteRecord();///ɾ���ѻָ���״̬
// //		pNewRecordset->UpdateRecord();
// 		SAFE_RELEASE(pNewRecordset);
// 	}
// 	strSql.Format(sqlAddWarGameResult,nUserRace,objCurrentDate.Format("%Y-%m-%d"),nScore);
// 	hr=m_pDB->ExecuteSQL((LPSTR)(LPCSTR)strSql);
// 	return hr;
// }

int CWargameManager::GetCurrentStatus(OBJID idWarGame)
{
	CHECKF(idWarGame);
	
	CWargame* pWargame=GetWargame(idWarGame);
	CHECKF(pWargame);
	
	return pWargame->GetCurrentStatus();
}

bool CWargameManager::IsHasSignup(OBJID idWarGame, OBJID idUser)///�Ƿ��ѲμӾ�����.
{
	CHECKF(idWarGame);
	CHECKF(idUser);

	CWargame* pWargame=GetWargame(idWarGame);
	CHECKF(pWargame);

	if(pWargame->GetWarGamePlayer(idUser))
		return true;

	return false;
}

bool CWargameManager::Signup(OBJID idWarGame, CUser* pUser)///�����.
{
	CHECKF(pUser);

	CWargame* pWargame=GetWargame(idWarGame);
	CHECKF(pWargame);

	return pWargame->Signup(pUser);
}

bool CWargameManager::Signup(OBJID idWarGame, WARGAME_PLAYERSCORE* pInfo)
{
	CHECKF(idWarGame);
	CHECKF(pInfo);

	CWargame* pWargame=GetWargame(idWarGame);
	CHECKF(pWargame);
	
	return pWargame->Signup(pInfo);
}

bool CWargameManager::CanSignup(OBJID idWarGame)///��Ƿ���Կ�ʼ������.
{
	CHECKF(idWarGame);

	CWargame* pWargame=GetWargame(idWarGame);
	CHECKF(pWargame);

	return pWargame->CanSignup();
}

bool CWargameManager::EnterWar(OBJID idWarGame, CUser* pUser)
{
	CHECKF(pUser);
	CHECKF(idWarGame);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECKF(pWargame);
	
	if(!pWargame->CanUserEnter())
		return false;

	return pWargame->EnterWar(pUser);
}

bool CWargameManager::LeaveWar(OBJID idWarGame, CUser* pUser)
{
	CHECKF(pUser);
	CHECKF(idWarGame);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECKF(pWargame);
	
// 	if(pWargame->GetCurrentStatus() != WARGAME_STATUS_WORK)
// 		return false;
	
	pWargame->LeaveWar(pUser);
	return true;
}

bool CWargameManager::CanUserEnter(OBJID idWarGame)
{
	CHECKF(idWarGame);

	CWargame* pWargame = GetWargame(idWarGame);
	CHECKF(pWargame);

	return pWargame->CanUserEnter();
}

void CWargameManager::OnPlayerKill(IRole* pAtker, IRole* pTarget, OBJID idWarGame)///��ұ�ɱ
{
	CHECK(pAtker);
	CHECK(pTarget);
	CHECK(idWarGame);

	CWargame* pWargame = GetWargame(idWarGame);
	CHECK(pWargame);

	pWargame->OnPlayerKill(pAtker, pTarget);
}

bool CWargameManager::GetRebornPos(OBJID idWarGame, OBJID idUser, int &idMap, int &nPosX, int &nPosY)
{
	CHECKF(idWarGame);
	CHECKF(idUser);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECKF(pWargame);

	return pWargame->GetRebornPos(idUser, idMap, nPosX, nPosY);
}

int CWargameManager::SendScoreRank(OBJID idWarGame, CUser* pUser)
{
	CHECKF(idWarGame);
	CHECKF(pUser);

	CWargame* pWargame = GetWargame(idWarGame);
	CHECKF(pWargame);

	return pWargame->SendScoreRank(pUser);
}

void CWargameManager::ProcessReborn(OBJID idWarGame, OBJID idUser)
{
	CHECK(idWarGame);
	CHECK(idUser);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECK(pWargame);
	
	pWargame->ProcessReborn(idUser);
}

void CWargameManager::SynchroPlayerAmount(OBJID idWarGame, WARGAME_PLAYERSCORE* pInfo, int nAmount)
{
	CHECK(idWarGame);
	CHECK(pInfo);

	CWargame* pWargame = GetWargame(idWarGame);
	CHECK(pWargame);

	pWargame->SynchroPlayerAmount(pInfo, nAmount);
}

bool CWargameManager::IsMaxPlayer(OBJID idWarGame)
{
	CHECKF(idWarGame);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECKF(pWargame);

	return pWargame->IsMaxPlayer();
}

bool CWargameManager::IsMaxSigninPlayer(OBJID idWarGame)
{
	CHECKF(idWarGame);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECKF(pWargame);
	
	return pWargame->IsMaxSigninPlayer();
}

CGameMap* CWargameManager::GetWarGameMap(OBJID idWarGame, CUser* pUser, int& nPosX, int& nPosY)
{
	CHECKF(idWarGame);
	CHECKF(pUser);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECKF(pWargame);
	
	return pWargame->GetWarGameMap(pUser, nPosX, nPosY);
}

void CWargameManager::PlayerLogout(OBJID idWarGame, CUser* pUser)
{
	CHECK(idWarGame);
	CHECK(pUser);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECK(pWargame);
	
	pWargame->PlayerLogout(pUser);
}

void CWargameManager::BroadCaseEnterMsg(OBJID idWarGame)
{
	CHECK(idWarGame);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECK(pWargame);
	
	pWargame->BroadCaseEnterMsg();
}

void CWargameManager::BroadCastCampMsg(OBJID idWarGame, OBJID idSender, CNetMsg* pMsg)
{
	CHECK(idWarGame);
	CHECK(idSender);
	CHECK(pMsg);
	
	CWargame* pWargame = GetWargame(idWarGame);
	CHECK(pWargame);
	
	pWargame->BroadCastCampMsg(idSender, pMsg);
}
// void CWargameManager::OnUserLogin(CUserPtr pUser)///��������ύ����Ȩ
// {
// 	UINT nWargameType=0;
// 	IStatus* pStatus=pUser->QueryStatus(STATUS_WARGAME);
// 	if (pStatus)
// 		nWargameType=pStatus->GetPower();
// 	CWargame* pWargame=GetWargame(nWargameType);
// 	if (pWargame)
// 		pWargame->OnUserLogin(pUser);
// }
