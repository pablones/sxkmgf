// Wargame.cpp: implementation of the CWargame class.
//
//////////////////////////////////////////////////////////////////////
#include "afx.h"
#include "Wargame.h"
#include "MapGroup.h"
#include "WargameBorder.h"
#include "BaseFunc.h"
#include "SettledAction.h"
#include "WarGameArena.h"
#include "MsgPlayer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWargame::CWargame()
{
	m_tLastOnTimer = clock();

	memset(&m_wargameInfo, 0, sizeof(m_wargameInfo));
	m_idProcess = 0;
	m_nStatus = WARGAME_STATUS_STOP;
	m_vetMapInfo.clear();
	m_vetPlayerInfo.clear();
	m_mapCampScore.clear();
}

CWargame::~CWargame()
{

}
CWargame* CWargame::CreateNew(int nType)
{
	switch(nType)
	{
	case WARGAME_TYPE_ARENA:
		return new CWarGameArena;
		break;
	default:
		ASSERT(!"CWargame::CreateWargame unsupport wargame type");
		return NULL;
		break;
	}
}
bool CWargame::CreateWargame(WARGAME_INFO &info, int idProcess)
{
	m_idProcess = idProcess;

	memcpy(&m_wargameInfo, &info, sizeof(WARGAME_INFO));
	
	return true;
}

int CWargame::LoadMap()
{
	//重复加载
	if(m_vetMapInfo.size() > 0)
		return m_vetMapInfo.size();
	//初始化地图
	char szSql[256] = "";
	sprintf(szSql, "select * from %s where type = %d", _TBL_WARGAMEMAP, m_wargameInfo.id);
	IRecordset* pRes = Database()->CreateNewRecordset(szSql);
	if(pRes == NULL)
	{
		ASSERT(!"CWargame::CreateWargame query maps failed!");
		return 0;
	}
	if(pRes->RecordCount() < m_wargameInfo.nCamps)	//有阵营没有分配地图坐标
	{
		::LogSave("CWargame::CreateWargame not enough maps! wargameid=%d", m_wargameInfo.id);
		SAFE_RELEASE(pRes);
		return 0;
	}
	
	WARGAMEMAP_INFO mapInfo = {0};
	CGameMap *pMap = NULL;
	for(int i = 0; i < pRes->RecordCount(); i++,pRes->MoveNext())
	{
		mapInfo.id = pRes->LoadInt("id");
		mapInfo.nType = pRes->LoadInt("type");
		mapInfo.idMap = pRes->LoadInt("map_id");
		mapInfo.nBorn_X = pRes->LoadInt("born_X");
		mapInfo.nBorn_Y = pRes->LoadInt("born_Y");
		mapInfo.nReBorn_X = pRes->LoadInt("reborn_X");
		mapInfo.nReBorn_Y = pRes->LoadInt("reborn_Y");
		
		if(g_SpecilLine != 1)
			m_vetMapInfo.push_back(mapInfo);
		else
		{
			pMap = MapManager()->QueryMap(mapInfo.idMap, TRUE);
			CHECKF(pMap);
			m_vetMapInfo.push_back(mapInfo);
		}
	}
	SAFE_RELEASE(pRes);
	
	if(m_vetMapInfo.size() < m_wargameInfo.nCamps)
	{
		::LogSave("CWargame::CreateWargame can not found maps!wargameid=%d", m_wargameInfo.id);
		return 0;
	}

	Init();

	if(g_SpecilLine==1)
	{
		//广播到其他线路
		CMsgWargame msg;
		if (msg.Create(6, WARGAME_LINEBORAD, WARGAMELINEBORAD_LOADMAP))
		{
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
		}
	}
	return m_vetMapInfo.size();
}

void CWargame::Init()
{
	if(m_mapCampScore.size()>0)
		return;

	WARGAME_CAMPS campInfo = {0};
	for(int i=0; i<m_wargameInfo.nCamps; i++)
	{
		campInfo.nScore = 0;
		campInfo.nPlayerAmount = 0;
		memcpy(campInfo.mapInfo, &m_vetMapInfo[i], sizeof(WARGAMEMAP_INFO));
		m_mapCampScore.insert(make_pair(i+1, campInfo));
	}
}

int CWargame::GetExpectStatusByTime()
{
	CTime curTime = CTime::GetCurrentTime();
	int nCurHour = curTime.GetHour();
	int nCurMinute = curTime.GetMinute();
	DWORD timeNow	=nCurHour*60+nCurMinute;

	int nSinginBeginHour = m_wargameInfo.nSinginBegin/100;
	int nSinginBeginMin = m_wargameInfo.nSinginBegin%100;
	int nSinginEndHour = m_wargameInfo.nSinginEnd/100;
	int nSinginEndMin = m_wargameInfo.nSinginEnd%100;
	
	if (timeNow >= nSinginBeginHour*60+nSinginBeginMin && timeNow < nSinginEndHour*60+nSinginEndMin)
	{
		return WARGAME_STATUS_PREPARE;
	}

	int nStartHour = m_wargameInfo.nStartTime/100;
	int nStartMin = m_wargameInfo.nStartTime%100;
	int nEndHour = m_wargameInfo.nEndTime/100;
	int nEndMin = m_wargameInfo.nEndTime%100;

	if(timeNow >= nStartHour*60+nStartMin && timeNow < nEndHour*60+nEndMin)
	{
		return WARGAME_STATUS_WORK;
	}

	return WARGAME_STATUS_STOP;
}

int CWargame::GetCurrentStatus()
{
	return m_nStatus;
}

// int CWargame::GetType()
// {
// 	return m_wargameInfo.nType;
// }

OBJID CWargame::GetWarGameID()
{
	return m_wargameInfo.id;
}

bool CWargame::CanSignup()
{
	return GetCurrentStatus() == WARGAME_STATUS_PREPARE;
}

bool CWargame::CanUserEnter()
{
	if(GetCurrentStatus() == WARGAME_STATUS_STOP)
		return false;
	//return GetCurrentStatus() == WARGAME_STATUS_WORK || GetCurrentStatus() == WARGAME_STATUS_PREPARE;
	CTime curTime = CTime::GetCurrentTime();
	int nCurHour = curTime.GetHour();
	int nCurMinute = curTime.GetMinute();
	DWORD timeNow	=nCurHour*60+nCurMinute;
	
	int nStartHour = m_wargameInfo.nStartTime/100;
	int nStartMin = m_wargameInfo.nStartTime%100;
	int nEndHour = m_wargameInfo.nEndTime/100;
	int nEndMin = m_wargameInfo.nEndTime%100;
	
	if(nStartHour*60+nStartMin > timeNow)//未到开场时间，可提前5分钟进场
	{
		if((nStartHour*60+nStartMin) - timeNow <= 5)
			return true;
	}
	else
	{
		if(timeNow - (nStartHour*60+nStartMin) <= 17)//开场后16分钟内可以进场,留1分钟反应时间
			return true;
	}
	return false;
}

//仅包含目前仍在场内的玩家，离开了的不算
bool CWargame::IsMaxPlayer()
{
// 	int nPlayerAmount = 0;
// 
// 	map<int, WARGAME_CAMPS>::iterator it = m_mapCampScore.begin();
// 	for(; it!=m_mapCampScore.end(); it++)
// 	{
// 		nPlayerAmount += it->second.nPlayerAmount;
// 	}
//	return nPlayerAmount >= m_wargameInfo.nMaxPlayer;
	return GetPlayerInWar() >= m_wargameInfo.nMaxPlayer;
}

bool CWargame::IsMaxSigninPlayer()
{
	return m_vetPlayerInfo.size() >= m_wargameInfo.nMaxPlayer;
}

bool CWargame::Signup(CUser* pUser, bool bSynchro/*=true*/)
{
	CHECKF(pUser);

	if(!CanSignup() && bSynchro)
		return false;

	if(pUser->GetLev() < m_wargameInfo.nLevBegin || pUser->GetLev() > m_wargameInfo.nLevEnd)//等级不符合条件
		return false;

	if(IsMaxPlayer())//名额已满
		return false;

	//已经在名单里了
	if(GetWarGamePlayer(pUser->GetID()))
		return false;

	WARGAME_PLAYER info = {0};
	info.id = pUser->GetID();

	if(pUser->IsVipActive())
		info.ucVipLev = pUser->GetVipLev();
	else
		info.ucVipLev = 0;
	info.usLev = pUser->GetLev();
	info.dwPro = pUser->GetProfession(1);

	SafeCopy(info.szName, pUser->GetName(), _MAX_NAMESIZE);

	AddWarGamePlayer(info);

	if(bSynchro)
	{
		//广播到其他线路
		CMsgWargame msg;
		if (msg.Create(pUser->GetID(), WARGAME_LINEBORAD, WARGAMELINEBORAD_SIGNIN))
		{
			msg.Append(info.id, info.usLev, info.nScore, info.ucCamp, info.ucVipLev, info.dwPro, info.szName);
			msg.m_pInfo->stUserList[0].usRank = GetWarGameID();
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
		}
	}
	return true;
}
//其他线路过来的报名信息
bool CWargame::Signup(WARGAME_PLAYERSCORE* pInfo)
{
	CHECKF(pInfo);
	
	if(pInfo->usLev < m_wargameInfo.nLevBegin || pInfo->usLev > m_wargameInfo.nLevEnd)//等级不符合条件
		return false;
	
	//已经在名单里了
	if(GetWarGamePlayer(pInfo->idPlayer))
		return false;

	WARGAME_PLAYER info = {0};
	info.id = pInfo->idPlayer;
	info.ucCamp = pInfo->ucCamp;
	info.ucVipLev = pInfo->ucVipLev;

	info.usLev = pInfo->usLev;
	info.dwPro = pInfo->dwProfession;

	SafeCopy(info.szName, pInfo->szName, _MAX_NAMESIZE);

	AddWarGamePlayer(info);
	
	return true;
}

bool CWargame::AddWarGamePlayer(WARGAME_PLAYER info)
{
	CHECKF(info.id);

	m_vetPlayerInfo.push_back(info);
	return true;
}
//分配阵营
int CWargame::AllocCamp()
{
	map<int, WARGAME_CAMPS>::iterator it = m_mapCampScore.begin();
	map<int, WARGAME_CAMPS>::iterator itMin = m_mapCampScore.begin();

	for(; it!=m_mapCampScore.end(); it++)
	{
		if(it->second.nPlayerAmount < itMin->second.nPlayerAmount)
			itMin = it;
	}

	itMin->second.nPlayerAmount++;
	return itMin->first;
}

bool CWargame::EnterWar(CUser* pUser)
{
	CHECKF(pUser);
	
	if(pUser->GetLev() < m_wargameInfo.nLevBegin || pUser->GetLev() > m_wargameInfo.nLevEnd)//等级不符合条件
		return false;

	WARGAME_PLAYER* pInfo = GetWarGamePlayer(pUser->GetID());
	if(!pInfo)
	{
		if(!Signup(pUser, false))
			return false;
		pInfo = GetWarGamePlayer(pUser->GetID());
	}

	CHECKF(pInfo);

	pInfo->bEntered = true;
	if(pInfo->ucCamp==0)
		pInfo->ucCamp = AllocCamp();

	WARGAME_CAMPS* pGampInfo = GetCampsInfo(pInfo->ucCamp);
	CHECKF(pGampInfo);

	//在专线的玩家需要让他飞一下地图
	if(g_SpecilLine==1)
	{
		if(!IsWarGameMap(pUser->GetMapID()))
		{
			pUser->FlyMap(pGampInfo->mapInfo->idMap, pGampInfo->mapInfo->nBorn_X, pGampInfo->mapInfo->nBorn_Y);
		}
	}
	
	CUser::AttachStatus(pUser, STATUS_WARGAME, GetWarGameID(), -1, 0, pInfo->ucCamp);
	pUser->AddWarGameMask(m_wargameInfo.nType);
	SendCampScore(pUser);

	if (pUser->GetMountState())//下坐骑
	{
		pUser->SetUnMount();
		CMsgPlayer msgPlayer;
		if (msgPlayer.Create(pUser->QueryRole()))
			pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		
		CRole::DetachStatus(pUser,STATUS_MOUNTSPEED_NUMBER);
	}

	//广播到其他线路
	CMsgWargame msg;
	if (msg.Create(pUser->GetID(), WARGAME_LINEBORAD, WARGAMELINEBORAD_ENTER))
	{
		msg.Append(pInfo->id, pInfo->usLev, pInfo->nScore, pInfo->ucCamp, pInfo->ucVipLev, pInfo->dwPro, pInfo->szName);
		msg.m_pInfo->stUserList[0].usRank = GetWarGameID();
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
	}

	return true;
}

WARGAME_PLAYER* CWargame::GetWarGamePlayer(OBJID idUser)
{
	CHECKF(idUser);

	for(int i=m_vetPlayerInfo.size()-1; i>=0; i--)
	{
		if(m_vetPlayerInfo[i].id == idUser)
			return &m_vetPlayerInfo[i];
	}
	return NULL;
}
void CWargame::OnPrepare()
{
	if(m_vetMapInfo.size()==0)
		LoadMap();
}
void CWargame::OnStart()
{
	
}
void CWargame::OnStop()
{
	m_tStopTime = clock();
}

WARGAME_CAMPS* CWargame::GetCampsInfo(int nCamps)
{
	map<int, WARGAME_CAMPS>::iterator it = m_mapCampScore.begin();
	
	for(; it!=m_mapCampScore.end(); it++)
	{
		if(it->first == nCamps)
			return &(it->second);
	}
	
	return NULL;
}

void CWargame::OnPlayerKill(IRole* pAtker, IRole* pTarget)
{
	//计算积分，奖励等，由派生类处理
}

bool CWargame::GetRebornPos(OBJID idUser, int &idMap, int &nPosX, int &nPosY)
{
	WARGAME_PLAYER* pInfo = GetWarGamePlayer(idUser);
	CHECKF(pInfo);

	WARGAME_CAMPS* pCampInfo = GetCampsInfo(pInfo->ucCamp);
	CHECKF(pCampInfo);

	idMap = pCampInfo->mapInfo->idMap;

	int nRand = RandGet(100);
	if(nRand < 50)
	{
		nPosX = pCampInfo->mapInfo->nReBorn_X;
		nPosY = pCampInfo->mapInfo->nReBorn_Y;
	}
	else
	{
		nPosX = pCampInfo->mapInfo->nBorn_X;
		nPosY = pCampInfo->mapInfo->nBorn_Y;
	}

	return true;
}

int CWargame::SendScoreRank(CUser* pUser)
{
	int nAmount = 0;

	DEBUG_TRY
	CMsgWargame msg;
	IF_NOT(msg.Create(1, WARGAME_SCORERANK, 0))
		return 0;

	if(pUser)
	{
		if(!pUser->QueryStatus(STATUS_WARGAME))
			return 0;
		msg.m_pInfo->idUser = pUser->GetID();
	}

	//各阵营积分
	WARGAME_PLAYERSCORE info = {0};
	info.idPlayer = 1;
	map<int, WARGAME_CAMPS>::iterator it = m_mapCampScore.begin();
	for(; it!=m_mapCampScore.end(); it++)
	{
		if(it->first == 1)
			info.nScore = it->second.nScore;
		else if(it->first == 2)
			info.nAwardScore = it->second.nScore;
		else if(it->first == 3)
			info.dwProfession = it->second.nScore;

		nAmount++;
	}
	msg.Append(info);

	//玩家积分
	for(int i=m_vetPlayerInfo.size()-1; i>=0; i--)
	{
		if(!m_vetPlayerInfo[i].bEntered)//只报名没进场
			continue;

		info.idPlayer = m_vetPlayerInfo[i].id;
		info.usRank = 0;
		info.usLev = m_vetPlayerInfo[i].usLev;
		info.nScore = m_vetPlayerInfo[i].nScore;
		info.nAwardScore = m_vetPlayerInfo[i].nAwardScore;
		info.usKilled = m_vetPlayerInfo[i].nKilled;
		info.ucCamp = m_vetPlayerInfo[i].ucCamp;
		info.ucVipLev = m_vetPlayerInfo[i].ucVipLev;
		info.dwProfession = m_vetPlayerInfo[i].dwPro;
		SafeCopy(info.szName, m_vetPlayerInfo[i].szName, _MAX_NAMESIZE);
		msg.Append(info);
		if(msg.GetAmount() >= 10)//以免包过大
		{
			if(pUser)
				pUser->SendMsg(&msg);
			else
				BroadCaseWarGameMsg(&msg);

			msg.Init();
			IF_NOT(msg.Create(1, WARGAME_SCORERANK, 0))
				return 0;
			if(pUser)
				msg.m_pInfo->idUser = pUser->GetID();
		}
		if(i==0)//告诉客户端数据发完了
		{
			if(pUser)
				msg.Append(2, 0, 0, 0, 0, 0, "");
			else
				msg.Append(3, 0, 0, 0, 0, 0, "");
		}

		nAmount++;
	}

	if(msg.GetAmount()>0)
	{
		if(pUser)
			pUser->SendMsg(&msg);
		else
			BroadCaseWarGameMsg(&msg);
	}

	DEBUG_CATCH("CWargame::QueryScoreRank")
	return nAmount;
}
//竞技场内广播消息
void CWargame::BroadCaseWarGameMsg(CMsgWargame* pMsg)
{
	CHECK(pMsg);

	CUser* pUser = NULL;
	for(int i=m_vetPlayerInfo.size()-1; i>=0; i--)
	{
		pUser = UserManager()->GetUser(m_vetPlayerInfo[i].id);
		if(pUser && pUser->QueryStatus(STATUS_WARGAME))
		{
			pMsg->m_pInfo->idUser = m_vetPlayerInfo[i].id;
			pUser->SendMsg(pMsg);
		}
	}
}

void CWargame::OnTimer(time_t tCurr)
{
	DEBUG_TRY
	if(clock() - m_tLastOnTimer < WARGAME_ONTIMER_INTEVAL)
		return;

	m_tLastOnTimer = clock();

	int nExpectStatus = GetExpectStatusByTime();
	if(GetCurrentStatus() != nExpectStatus)
	{
		m_nStatus = nExpectStatus;

		switch(m_nStatus)
		{
		case WARGAME_STATUS_PREPARE:
			OnPrepare();
			break;
		case WARGAME_STATUS_WORK:
			OnStart();
			break;
		case WARGAME_STATUS_STOP:
			OnStop();
			break;
		default:
			::LogSave("CWargame::OnTimer unknow wargame status %d!", m_nStatus);
			break;
		}
	}

	OnProcess();
	DEBUG_CATCH("CWargame::OnTimer")
}

void CWargame::OnProcess()
{
	if(GetCurrentStatus() == WARGAME_STATUS_STOP)
	{
		clock_t tPassed = clock() - m_tStopTime;
		if(tPassed >= 900000 && tPassed%3==0 && m_vetPlayerInfo.size()>0)//结束900秒后清理竞技场
			Stop();
	}
}

void CWargame::Stop()
{
	CUser* pUser = NULL;
	for(int i=m_vetPlayerInfo.size()-1; i>=0; i--)
	{
		pUser = UserManager()->GetUser(m_vetPlayerInfo[i].id);// == idUser)
		if(pUser && IsWarGameMap(pUser->GetMapID()) && m_vetPlayerInfo[i].bEntered)
		{
			m_vetPlayerInfo[i].bEntered = false;
			LeaveWar(pUser);
			return;
		}
	}

	map<int, WARGAME_CAMPS>::iterator it = m_mapCampScore.begin();
	for(; it!=m_mapCampScore.end(); it++)
	{
		it->second.nPlayerAmount = 0;
		it->second.nScore = 0;
	}
	m_vetPlayerInfo.clear();
}

void CWargame::Release()
{
	m_vetMapInfo.clear();
	m_vetPlayerInfo.clear();
	m_mapCampScore.clear();

	delete this;
}

int CWargame::GetWinCamp(int &nMaxScore)
{
	int nWinCamp = 0;
	nMaxScore = 0;

	map<int, WARGAME_CAMPS>::iterator it = m_mapCampScore.begin();
	for(; it!=m_mapCampScore.end(); it++)
	{
		if(it->second.nScore > nMaxScore)
		{
			nMaxScore = it->second.nScore;
			nWinCamp = it->first;
		}
		else if(it->second.nScore == nMaxScore)//平手
			nWinCamp = 0;
	}
	return nWinCamp;
}

void CWargame::ProcessReborn(OBJID idUser)
{
}

void CWargame::LeaveWar(CUser* pUser)
{
	CHECK(pUser);
	
	pUser->DetachStatus(pUser, STATUS_WARGAME);

	OBJID idUser = pUser->GetID();
	//通知客户端切线
	pUser->SetSpecilSteate(STATE_SPECILLINE_OUT);
	pUser->ChangeLineMap(pUser->GetRecordMapID(), pUser->GetRecordPosX(),pUser->GetRecordPosY(), RANDLINE);

	WARGAME_PLAYER* pInfo = GetWarGamePlayer(idUser);
	CHECK(pInfo);

	WARGAME_CAMPS* pCampInfo = GetCampsInfo(pInfo->ucCamp);
	CHECK(pCampInfo);

	CHECK(pCampInfo->nPlayerAmount>0);
	pCampInfo->nPlayerAmount--;

	//广播到其他线路
	CMsgWargame msg;
	if (msg.Create(idUser, WARGAME_LINEBORAD, WARGAMELINEBORAD_LEAVE))
	{
		msg.Append(pInfo->id, pInfo->usLev, pInfo->nScore, pInfo->ucCamp, pInfo->ucVipLev, pInfo->dwPro, pInfo->szName);
		msg.m_pInfo->stUserList[0].usRank = GetWarGameID();
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
	}
}

void CWargame::SynchroPlayerAmount(WARGAME_PLAYERSCORE* pInfo, int nAmount)
{
	CHECK(pInfo);

	WARGAME_CAMPS* pCampInfo = GetCampsInfo(pInfo->ucCamp);
	if(!pCampInfo)
		return;

	pCampInfo->nPlayerAmount+=nAmount;
}

CGameMap* CWargame::GetWarGameMap(CUser* pUser, int& nPosX, int& nPosY)
{
	CHECKF(pUser);
	
	if(pUser->GetLev() < m_wargameInfo.nLevBegin || pUser->GetLev() > m_wargameInfo.nLevEnd)//等级不符合条件
		return NULL;
	
	if(!GetWarGamePlayer(pUser->GetID()))
	{
		if(!Signup(pUser, false))
			return NULL;
	}
	
	WARGAME_PLAYER* pInfo = GetWarGamePlayer(pUser->GetID());
	CHECKF(pInfo);
	if(pInfo->ucCamp==0)
		pInfo->ucCamp = AllocCamp();
	
	WARGAME_CAMPS* pCampInfo = GetCampsInfo(pInfo->ucCamp);
	CHECKF(pCampInfo);

	CGameMap* pMap = MapManager()->QueryMap(pCampInfo->mapInfo->idMap, TRUE);
	nPosX = pCampInfo->mapInfo->nBorn_X;
	nPosY = pCampInfo->mapInfo->nBorn_Y;

	return pMap;
}

void CWargame::BroadCaseEnterMsg()
{

}

int CWargame::GetPlayerInWar()
{
	int nPlayerAmount = 0;
	
	map<int, WARGAME_CAMPS>::iterator it = m_mapCampScore.begin();
	for(; it!=m_mapCampScore.end(); it++)
	{
		nPlayerAmount += it->second.nPlayerAmount;
	}
	return nPlayerAmount;
}

bool CWargame::SendCampScore(CUser* pUser/*=NULL*/)
{
	//各阵营积分
	CMsgWargame msg;

	if(!msg.Create(1, WARGAME_CAMPSCORE, 0))
		return false;

	WARGAME_PLAYERSCORE info = {0};
	info.idPlayer = 1;
	map<int, WARGAME_CAMPS>::iterator it = m_mapCampScore.begin();
	for(; it!=m_mapCampScore.end(); it++)
	{
		if(it->first == 1)
			info.nScore = it->second.nScore;
		else if(it->first == 2)
			info.nAwardScore = it->second.nScore;
		else if(it->first == 3)
			info.dwProfession = it->second.nScore;
	}

	msg.Append(info);

	if(pUser)
	{
		msg.m_pInfo->idUser = pUser->GetID();
		pUser->SendMsg(&msg);
	}
	else
		BroadCaseWarGameMsg(&msg);

	return true;
}

bool CWargame::IsWarGameMap(OBJID idMap)
{
	if(idMap >= 1049 && idMap <= 1051)
		return true;

	return false;
}

void CWargame::PlayerLogout(CUser* pUser)
{
	CHECK(pUser);

	WARGAME_PLAYER* pInfo = GetWarGamePlayer(pUser->GetID());
	CHECK(pInfo);

	WARGAME_CAMPS* pCampInfo = GetCampsInfo(pInfo->ucCamp);
	CHECK(pCampInfo);
	
	CHECK(pCampInfo->nPlayerAmount>0);
	pCampInfo->nPlayerAmount--;
	
	//广播到其他线路
	CMsgWargame msg;
	if (msg.Create(pUser->GetID(), WARGAME_LINEBORAD, WARGAMELINEBORAD_LEAVE))
	{
		msg.Append(pInfo->id, pInfo->usLev, pInfo->nScore, pInfo->ucCamp, pInfo->ucVipLev, pInfo->dwPro, pInfo->szName);
		msg.m_pInfo->stUserList[0].usRank = GetWarGameID();
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
	}
}

void CWargame::BroadCastCampMsg(OBJID idSender, CNetMsg* pMsg)//阵营聊天
{
	CHECK(idSender);
	CHECK(pMsg);

	WARGAME_PLAYER* pInfo = GetWarGamePlayer(idSender);
	CHECK(pInfo);
	int nCamps = pInfo->ucCamp;

	CUser* pUser = NULL;
	for(int i=m_vetPlayerInfo.size()-1; i>=0; i--)
	{
		if(m_vetPlayerInfo[i].ucCamp != nCamps)
			continue;

		pUser = UserManager()->GetUser(m_vetPlayerInfo[i].id);
		if(pUser && pUser->QueryStatus(STATUS_WARGAME))
		{
			pUser->SendMsg(pMsg);
		}
	}
}