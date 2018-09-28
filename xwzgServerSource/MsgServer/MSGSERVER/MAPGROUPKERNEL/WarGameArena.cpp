// WarGameArena.cpp: implementation of the CWarGameArena class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdtctl.h>
#include "WarGameArena.h"
#include "MapGroup.h"
#include "MsgUserAttrib.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWarGameArena::CWarGameArena()
{
	m_nBroadTimes = 0;
}

CWarGameArena::~CWarGameArena()
{

}

bool CWarGameArena::ScoreCompare(WARGAME_PLAYER player1, WARGAME_PLAYER player2)//用于积分排名
{
	if (player1.nScore == player2.nScore)
		return player1.usLev > player2.usLev;
	return player1.nScore > player2.nScore;
}

void CWarGameArena::OnStart()
{
	m_tLastBroadCast = clock();
	m_nBroadTimes = 0;
	
	if(g_SpecilLine==1)
	{
		BroadCaseEnterMsg();
		//告诉非专线的线路可以进场了
		CMsgWargame msg;
		if (msg.Create(GetWarGameID(), WARGAME_LINEBORAD, WARGAMELINEBORAD_START))
		{
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
		}
	}
	CWargame::OnStart();
}

void CWarGameArena::OnStop()
{
	DEBUG_TRY
	CWargame::OnStop();

	m_nBroadTimes = 0;
	if(m_vetPlayerInfo.size()<=0)
		return;

	int nMaxScore = 0;
	int nWinCamp = GetWinCamp(nMaxScore);//有可能是0，两个阵营平手，没有第一
// 	if(nMaxScore==0)
// 		return;

	sort(m_vetPlayerInfo.begin(), m_vetPlayerInfo.end(), CWarGameArena::ScoreCompare);//前10名给奖励
	CUser* pUser = NULL;

	for(int i=m_vetPlayerInfo.size()-1; i>=0; i--)
	{
		if(!m_vetPlayerInfo[i].bEntered)//只报名没进场
			continue;

		pUser = UserManager()->GetUser(m_vetPlayerInfo[i].id);
		if(pUser)
		{
			pUser->DetachStatus(pUser, STATUS_DOT_WARGAMEEXP);

			if(i < 10)
			{
				int nAwardScore = 10*(11-i+1);//10*(11-名次)
				pUser->AddAttrib(_USERATTRIB_ARENA_SCORE, nAwardScore, SYNCHRO_TRUE);
				m_vetPlayerInfo[i].nAwardScore += nAwardScore;
			}

			if(nMaxScore==0)
				continue;

			if(pUser->QueryStatus(STATUS_WARGAME))//进来了又出去的不发
			{
				if(m_vetPlayerInfo[i].ucCamp == nWinCamp)
				{
					pUser->AddAttrib(_USERATTRIB_ARENA_SCORE, 500, SYNCHRO_TRUE);
					m_vetPlayerInfo[i].nAwardScore += 500;
				}
				else
				{
					pUser->AddAttrib(_USERATTRIB_ARENA_SCORE, 200, SYNCHRO_TRUE);
					m_vetPlayerInfo[i].nAwardScore += 200;
				}
			}
		}

		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %u arena score from wargame", m_vetPlayerInfo[i].id, m_vetPlayerInfo[i].szName, m_vetPlayerInfo[i].nScore + m_vetPlayerInfo[i].nAwardScore);
	}

	if(nMaxScore!=0)
	{
		BroadCaseResult(nWinCamp, nMaxScore);
		SendScoreRank(NULL);
	}
	
	DEBUG_CATCH("CWarGameArena::OnStop()")
}

void CWarGameArena::OnTimer(time_t tCurr)
{
	if(clock() - m_tLastOnTimer < WARGAME_ONTIMER_INTEVAL)
		return;

	CWargame::OnTimer(tCurr);//一定要放在前面，先检查状态，再来广播

	DEBUG_TRY
	if(GetCurrentStatus() == WARGAME_STATUS_WORK && !IsMaxPlayer())
	{
		if(tCurr - m_tLastBroadCast >= 300000 && m_nBroadTimes < 3)//每5分钟广播一次,一共3次
		{
			UserManager()->BroadcastMsgForWarGame(m_wargameInfo.nLevBegin, m_wargameInfo.nLevEnd, GetWarGameID());
			m_tLastBroadCast = tCurr;
			m_nBroadTimes++;
		}
// 		if(m_nBroadTimes>=3 && tCurr-m_tLastBroadCast >= 60000)//三次广播后还没人就关闭
// 		{
// 			int nPlayerInWar = GetPlayerInWar();
// 			if(nPlayerInWar >0 && nPlayerInWar < 2)
// 				Stop();
// 		}
	}
	//死了5分钟还不复活，踢出去
// 	vector<WARGAME_PLAYER>::iterator it = m_vetPlayerInfo.begin();
// 	for(; it!=m_vetPlayerInfo.end(); it++)
// 	{
// 		if(it->tDieTime == 0)
// 			continue;
// 
// 		if(clock() - it->tDieTime > _BEKILLED_TIMESPAN)
// 		{
// 			CUser* pUser = UserManager()->GetUser(it->id);
// 			if(pUser && pUser->QueryStatus(STATUS_WARGAME))//切线，转场
// 			{
// 				if(pUser->IsAlive())
// 					continue;
// 
// 				LeaveWar(pUser);
// 				//pUser->RebornUseScroll(false);
// 				it->tDieTime = 0;
// 				//m_vetPlayerInfo.erase(it);
// 			}
// 		}
// 	}
	DEBUG_CATCH("CWarGameArena::OnTimer")
}

bool CWarGameArena::EnterWar(CUser* pUser)
{
	CHECKF(pUser);

	if(!CWargame::EnterWar(pUser))
		return false;

	CTime timeCurr(CTime::GetCurrentTime());
	CTime timeEnd(timeCurr.GetYear(), timeCurr.GetMonth(), timeCurr.GetDay(), m_wargameInfo.nEndTime/100, m_wargameInfo.nEndTime%100, 0);
	CTimeSpan spanTime = timeEnd - timeCurr;
	
	int nLeftSec = spanTime.GetTotalSeconds();
	if(nLeftSec > 6)
	{
		int nAddExp = (125 + (pUser->GetLev() - 11) * 5) * 3 / 4;
		CHECKF(nAddExp > 0);
		CUser::AttachStatus(pUser, STATUS_DOT_WARGAMEEXP, nAddExp, 6, (nLeftSec/6)+1);
	}

	return true;
}

void CWarGameArena::LeaveWar(CUser* pUser)
{
	CHECK(pUser);

	if(pUser->QueryStatus(STATUS_DOT_WARGAMEEXP))
		pUser->DetachStatus(pUser, STATUS_DOT_WARGAMEEXP);

	CWargame::LeaveWar(pUser);
}

void CWarGameArena::OnPlayerKill(IRole* pAtker, IRole* pTarget)//杀人后的处理
{
	CHECK(pAtker);
	CHECK(pTarget);

	DEBUG_TRY
	WARGAME_PLAYER* pAtkInfo = GetWarGamePlayer(pAtker->GetID());
	WARGAME_PLAYER* pTargetInfo = GetWarGamePlayer(pTarget->GetID());

	CHECK(pAtkInfo);
	CHECK(pTargetInfo);

	pAtkInfo->nKilled++;
	pTargetInfo->nBeKilled++;
	pTargetInfo->tDieTime = clock();
	
	int nAwardScore = (m_wargameInfo.nBaseScore + pTargetInfo->nScore * m_wargameInfo.nScoreParam / 100);//获得积分=积分基数+对方积分*积分参数
	nAwardScore = __min(20, nAwardScore);//上限是20

	if(pAtkInfo->idLastKilled == pTarget->GetID() && pAtkInfo->nKillTime != 0)
	{
		if(!CheckLastKilled(pAtkInfo->nKillTime))
		{
			nAwardScore = 0;//3分钟内杀死同一个人没积分
		}
	}

	pAtkInfo->nScore += nAwardScore;
	
	WARGAME_CAMPS* pCampInfo = GetCampsInfo(pAtkInfo->ucCamp);
	CHECK(pCampInfo);
	pCampInfo->nScore += nAwardScore;
	
	CUser* pUser = NULL;
	if(pAtker->QueryObj(OBJ_USER, (void**)&pUser))
		pUser->AddAttrib(_USERATTRIB_ARENA_SCORE, nAwardScore, SYNCHRO_FALSE);

	pAtkInfo->idLastKilled = pTarget->GetID();
	CTime timeCurr(CTime::GetCurrentTime());
	pAtkInfo->nKillTime = timeCurr.GetHour()*10000 + timeCurr.GetMinute()*100 + timeCurr.GetSecond();

	if(nAwardScore > 0)
	{
		pAtker->SendSysMsg(_TXTATR_TOPSHOW, "获得%d荣誉值", nAwardScore);
	}

	if(nAwardScore > 0)
		SendCampScore(NULL);

	CWargame::OnPlayerKill(pAtker, pTarget);
	DEBUG_CATCH("CWarGameArena::OnPlayerKill")
}

bool CWarGameArena::CheckLastKilled(UINT nKillTime)
{
	if(nKillTime==0)
		return true;

	int nHour = nKillTime/10000;
	int nMin = (nKillTime/100) % 100;
	int nSec = nKillTime % 100;
	
	CTime timeCurr(CTime::GetCurrentTime());
	CTime timeLast(timeCurr.GetYear(), timeCurr.GetMonth(), timeCurr.GetDay(), nHour, nMin, nSec);
	
	CTimeSpan spanTime = timeCurr - timeLast;
	
	if(spanTime.GetTotalSeconds() < _LASTKILLED_TIMESPAN)//三分钟内杀死同一个不计数
		return false;

	return true;
}
//获得某阵营的第一名
WARGAME_PLAYER* CWarGameArena::GetTheFirst(int nCamp)
{
	WARGAME_PLAYER* pInfo = NULL;

	for(int i=m_vetPlayerInfo.size()-1; i>=0; i--)
	{
		if(m_vetPlayerInfo[i].ucCamp == nCamp)
		{
			if(!pInfo)
				pInfo = &m_vetPlayerInfo[i];
			else if(pInfo && m_vetPlayerInfo[i].nScore > pInfo->nScore)
				pInfo = &m_vetPlayerInfo[i];
		}
	}

	return pInfo;
}

char* CWarGameArena::GetCampName(int nCamp)
{
	char szCampName[16] = "";
	if(nCamp == 1)
		return "贪狼";
	else if(nCamp == 2)
		return "破军";
	else if(nCamp == 3)
		return "七杀";

	return NULL;
}

void CWarGameArena::BroadCaseResult(int nWinCamp, int nMaxScore)
{
	CHECK(nMaxScore);
	//发公告
	CHECK(m_vetMapInfo.size()>0);

	char szWarName[64] = "";
	OBJID idMap = m_vetMapInfo[0].idMap;//三个阵营在同一个地图上，才可以随便取一个
	CGameMap* pMap = MapManager()->QueryMap(idMap);
	CHECK(pMap);
	sprintf(szWarName, "3_%s_6", pMap->GetName());

	if(nWinCamp != 0)//有获胜阵营
	{
		WARGAME_PLAYER* pInfo = GetTheFirst(nWinCamp);
		CHECK(pInfo);
		
		char UserName[128] = "";
		sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, pInfo->szName);
		
		char szCamp[64] = "";
		sprintf(szCamp,"3_%s_6", GetCampName(nWinCamp));
		
		char szMsg[255] = "";
		sprintf(szMsg, "在<%s>的带领下，<%s>阵营勇不可挡，取得了本次<%s>的霸主地位！",UserName, szCamp, szWarName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, NULL, 0, 0, NULL, ALLUSERS_NAME);
	}
	else//没有获胜阵营，平手
	{
		map<int, WARGAME_CAMPS>::iterator it = m_mapCampScore.begin();
		vector<int> setWinCamps;
		for(; it!=m_mapCampScore.end(); it++)
		{
			if(it->second.nScore == nMaxScore)
			{
				setWinCamps.push_back(it->first);
			}
		}
		
		if(setWinCamps.size()==2)//2阵营平手
		{
			char szCamp[64] = "";
			sprintf(szCamp,"3_%s_6", GetCampName(setWinCamps[0]));
			
			
			char szCamp2[64] = "";
			sprintf(szCamp2,"3_%s_6", GetCampName(setWinCamps[1]));
			
			char szMsg[255] = "";
			sprintf(szMsg, "<%s>阵营与<%s>阵营骁勇善战，最终平分秋色，共享<%s>霸主地位！",szCamp, szCamp2, szWarName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, NULL, 0, 0, NULL, ALLUSERS_NAME);
		}
		else if(setWinCamps.size()==3)//3阵营平手
		{
			char szMsg[255] = "";
			sprintf(szMsg, "本次<%s>竞技中，三大阵营各不相让，激烈战斗后以平局结束，真是十年一见的奇景！", szWarName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, NULL, 0, 0, NULL, ALLUSERS_NAME);
		}
	}
}

void CWarGameArena::ProcessReborn(OBJID idUser)
{
	WARGAME_PLAYER* pInfo = GetWarGamePlayer(idUser);
	if(pInfo)
		pInfo->tDieTime = 0;

	CWargame::ProcessReborn(idUser);
}

void CWarGameArena::BroadCaseEnterMsg()
{
	//通知玩家进场
	DEBUG_TRY
	CUser* pUser = NULL;
	CMsgWargame msg;
	IF_NOT(msg.Create(1, WARGAME_ENTER, 1))
		return;
	
	for(int i=m_vetPlayerInfo.size()-1; i>=0; i--)
	{
		pUser = UserManager()->GetUser(m_vetPlayerInfo[i].id);
		if(pUser)
		{
			if(pUser->QueryStatus(STATUS_WARGAME) || CWargame::IsWarGameMap(pUser->GetMapID()))
				continue;
			msg.m_pInfo->idUser = m_vetPlayerInfo[i].id;
			msg.m_pInfo->ucAmount = 1;
			msg.m_pInfo->stUserList[0].idPlayer = GetWarGameID();
			pUser->SendMsg(&msg);
		}
	}
	DEBUG_CATCH("CWarGameArena::BroadCaseEnterMsg()")
}