// WarGameArena.h: interface for the CWarGameArena class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WARGAMEARENA_H__38E27A3E_1CD1_46A4_BCE5_17A0E18246E5__INCLUDED_)
#define AFX_WARGAMEARENA_H__38E27A3E_1CD1_46A4_BCE5_17A0E18246E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Wargame.h"

const int _LASTKILLED_TIMESPAN = 180;//3分钟内杀死同一个玩家不计数
const int _BEKILLED_TIMESPAN = 300000;//死了5分钟不复活要踢出场

class CWarGameArena : public CWargame  
{
public:
	CWarGameArena();
	virtual ~CWarGameArena();

public:
	void OnTimer(time_t tCurr);
	bool EnterWar(CUser* pUser);
	void LeaveWar(CUser* pUser);
	void OnStart();
	void OnStop();

	void OnPlayerKill(IRole* pAtker, IRole* pTarget);//杀人后的处理

	bool CheckLastKilled(UINT nKillTime);

	WARGAME_PLAYER* GetTheFirst(int nCamp);
	char* GetCampName(int nCamp);
	void  BroadCaseResult(int nWinCamp, int nMaxScore);
	void ProcessReborn(OBJID idUser);
	void BroadCaseEnterMsg();

	static bool ScoreCompare(WARGAME_PLAYER player1, WARGAME_PLAYER player2);//用于积分排名

private:
	clock_t	m_tLastBroadCast;
	int		m_nBroadTimes;
};

#endif // !defined(AFX_WARGAMEARENA_H__38E27A3E_1CD1_46A4_BCE5_17A0E18246E5__INCLUDED_)
