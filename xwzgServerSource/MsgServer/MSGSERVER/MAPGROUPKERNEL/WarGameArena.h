// WarGameArena.h: interface for the CWarGameArena class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WARGAMEARENA_H__38E27A3E_1CD1_46A4_BCE5_17A0E18246E5__INCLUDED_)
#define AFX_WARGAMEARENA_H__38E27A3E_1CD1_46A4_BCE5_17A0E18246E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Wargame.h"

const int _LASTKILLED_TIMESPAN = 180;//3������ɱ��ͬһ����Ҳ�����
const int _BEKILLED_TIMESPAN = 300000;//����5���Ӳ�����Ҫ�߳���

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

	void OnPlayerKill(IRole* pAtker, IRole* pTarget);//ɱ�˺�Ĵ���

	bool CheckLastKilled(UINT nKillTime);

	WARGAME_PLAYER* GetTheFirst(int nCamp);
	char* GetCampName(int nCamp);
	void  BroadCaseResult(int nWinCamp, int nMaxScore);
	void ProcessReborn(OBJID idUser);
	void BroadCaseEnterMsg();

	static bool ScoreCompare(WARGAME_PLAYER player1, WARGAME_PLAYER player2);//���ڻ�������

private:
	clock_t	m_tLastBroadCast;
	int		m_nBroadTimes;
};

#endif // !defined(AFX_WARGAMEARENA_H__38E27A3E_1CD1_46A4_BCE5_17A0E18246E5__INCLUDED_)
