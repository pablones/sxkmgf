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

bool CWarGameArena::ScoreCompare(WARGAME_PLAYER player1, WARGAME_PLAYER player2)//���ڻ�������
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
		//���߷�ר�ߵ���·���Խ�����
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
	int nWinCamp = GetWinCamp(nMaxScore);//�п�����0��������Ӫƽ�֣�û�е�һ
// 	if(nMaxScore==0)
// 		return;

	sort(m_vetPlayerInfo.begin(), m_vetPlayerInfo.end(), CWarGameArena::ScoreCompare);//ǰ10��������
	CUser* pUser = NULL;

	for(int i=m_vetPlayerInfo.size()-1; i>=0; i--)
	{
		if(!m_vetPlayerInfo[i].bEntered)//ֻ����û����
			continue;

		pUser = UserManager()->GetUser(m_vetPlayerInfo[i].id);
		if(pUser)
		{
			pUser->DetachStatus(pUser, STATUS_DOT_WARGAMEEXP);

			if(i < 10)
			{
				int nAwardScore = 10*(11-i+1);//10*(11-����)
				pUser->AddAttrib(_USERATTRIB_ARENA_SCORE, nAwardScore, SYNCHRO_TRUE);
				m_vetPlayerInfo[i].nAwardScore += nAwardScore;
			}

			if(nMaxScore==0)
				continue;

			if(pUser->QueryStatus(STATUS_WARGAME))//�������ֳ�ȥ�Ĳ���
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

	CWargame::OnTimer(tCurr);//һ��Ҫ����ǰ�棬�ȼ��״̬�������㲥

	DEBUG_TRY
	if(GetCurrentStatus() == WARGAME_STATUS_WORK && !IsMaxPlayer())
	{
		if(tCurr - m_tLastBroadCast >= 300000 && m_nBroadTimes < 3)//ÿ5���ӹ㲥һ��,һ��3��
		{
			UserManager()->BroadcastMsgForWarGame(m_wargameInfo.nLevBegin, m_wargameInfo.nLevEnd, GetWarGameID());
			m_tLastBroadCast = tCurr;
			m_nBroadTimes++;
		}
// 		if(m_nBroadTimes>=3 && tCurr-m_tLastBroadCast >= 60000)//���ι㲥��û�˾͹ر�
// 		{
// 			int nPlayerInWar = GetPlayerInWar();
// 			if(nPlayerInWar >0 && nPlayerInWar < 2)
// 				Stop();
// 		}
	}
	//����5���ӻ�������߳�ȥ
// 	vector<WARGAME_PLAYER>::iterator it = m_vetPlayerInfo.begin();
// 	for(; it!=m_vetPlayerInfo.end(); it++)
// 	{
// 		if(it->tDieTime == 0)
// 			continue;
// 
// 		if(clock() - it->tDieTime > _BEKILLED_TIMESPAN)
// 		{
// 			CUser* pUser = UserManager()->GetUser(it->id);
// 			if(pUser && pUser->QueryStatus(STATUS_WARGAME))//���ߣ�ת��
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

void CWarGameArena::OnPlayerKill(IRole* pAtker, IRole* pTarget)//ɱ�˺�Ĵ���
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
	
	int nAwardScore = (m_wargameInfo.nBaseScore + pTargetInfo->nScore * m_wargameInfo.nScoreParam / 100);//��û���=���ֻ���+�Է�����*���ֲ���
	nAwardScore = __min(20, nAwardScore);//������20

	if(pAtkInfo->idLastKilled == pTarget->GetID() && pAtkInfo->nKillTime != 0)
	{
		if(!CheckLastKilled(pAtkInfo->nKillTime))
		{
			nAwardScore = 0;//3������ɱ��ͬһ����û����
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
		pAtker->SendSysMsg(_TXTATR_TOPSHOW, "���%d����ֵ", nAwardScore);
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
	
	if(spanTime.GetTotalSeconds() < _LASTKILLED_TIMESPAN)//��������ɱ��ͬһ��������
		return false;

	return true;
}
//���ĳ��Ӫ�ĵ�һ��
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
		return "̰��";
	else if(nCamp == 2)
		return "�ƾ�";
	else if(nCamp == 3)
		return "��ɱ";

	return NULL;
}

void CWarGameArena::BroadCaseResult(int nWinCamp, int nMaxScore)
{
	CHECK(nMaxScore);
	//������
	CHECK(m_vetMapInfo.size()>0);

	char szWarName[64] = "";
	OBJID idMap = m_vetMapInfo[0].idMap;//������Ӫ��ͬһ����ͼ�ϣ��ſ������ȡһ��
	CGameMap* pMap = MapManager()->QueryMap(idMap);
	CHECK(pMap);
	sprintf(szWarName, "3_%s_6", pMap->GetName());

	if(nWinCamp != 0)//�л�ʤ��Ӫ
	{
		WARGAME_PLAYER* pInfo = GetTheFirst(nWinCamp);
		CHECK(pInfo);
		
		char UserName[128] = "";
		sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, pInfo->szName);
		
		char szCamp[64] = "";
		sprintf(szCamp,"3_%s_6", GetCampName(nWinCamp));
		
		char szMsg[255] = "";
		sprintf(szMsg, "��<%s>�Ĵ����£�<%s>��Ӫ�²��ɵ���ȡ���˱���<%s>�İ�����λ��",UserName, szCamp, szWarName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, NULL, 0, 0, NULL, ALLUSERS_NAME);
	}
	else//û�л�ʤ��Ӫ��ƽ��
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
		
		if(setWinCamps.size()==2)//2��Ӫƽ��
		{
			char szCamp[64] = "";
			sprintf(szCamp,"3_%s_6", GetCampName(setWinCamps[0]));
			
			
			char szCamp2[64] = "";
			sprintf(szCamp2,"3_%s_6", GetCampName(setWinCamps[1]));
			
			char szMsg[255] = "";
			sprintf(szMsg, "<%s>��Ӫ��<%s>��Ӫ������ս������ƽ����ɫ������<%s>������λ��",szCamp, szCamp2, szWarName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, NULL, 0, 0, NULL, ALLUSERS_NAME);
		}
		else if(setWinCamps.size()==3)//3��Ӫƽ��
		{
			char szMsg[255] = "";
			sprintf(szMsg, "����<%s>�����У�������Ӫ�������ã�����ս������ƽ�ֽ���������ʮ��һ�����澰��", szWarName);
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
	//֪ͨ��ҽ���
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