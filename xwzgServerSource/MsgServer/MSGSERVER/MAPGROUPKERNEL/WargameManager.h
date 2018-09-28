// WargameManager.h: interface for the CWargameManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WARGAMEMANAGER_H__6B300A42_8F2F_4260_A6E9_C79B6C3454F2__INCLUDED_)
#define AFX_WARGAMEMANAGER_H__6B300A42_8F2F_4260_A6E9_C79B6C3454F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "singleton.h"
#include "set"
#include "functional"
#include "MsgWargame.h"
#include "Wargame.h"
using namespace std;

//#define sWargamemgr CWargameManager::getSingleton()

class CWargame;
//class CMsgWargameSync;
typedef vector<CWargame*> CWargameList;
//typedef map<UINT,UINT> CRaceExchangeRateList;

///ս���������,��ͼ��ȫ��Ψһ����
class CWargameManager  ///: public Singleton<CWargameManager>
{
public:
	CWargameManager();
	virtual ~CWargameManager();

	///��ʼս���������
	BOOL Init(IDatabase* pDB,PROCESS_ID idProcess);
	//BOOL Release();///����ս���������
	void OnTimer(time_t tCurr);///��ʱ��������
//	void ProcessMsg(CMsgWargameSync* pMsg);///������Ϣ����

	bool AddWargame(CWargame* war);///���һ��ս���
	bool DelWargame(CWargame* war);///ɾ��һ��ս���
	CWargame* GetWargame(OBJID nWargameID);///ͨ��ս���typeid�õ�һ��ս�������ָ��
	UINT GetSize();///�õ������ڻ����
	bool CanSignup(OBJID idWarGame);///��Ƿ���Կ�ʼ������.
	bool Signup(OBJID idWarGame, CUser* pUser);///�����.
	bool Signup(OBJID idWarGame, WARGAME_PLAYERSCORE* pInfo);

	int  GetCurrentStatus(OBJID idWarGame);
	void OnPlayerKill(IRole* pAtker, IRole* pTarget, OBJID idWarGame);///��ұ�ɱ
	//void OnUserLogin(CUserPtr pUser);

	///�õ�ָ������Ķһ�����.
//	UINT GetRaceExchangeRate(UINT nUserRace){return m_RaceExchangeRateList[nUserRace];}

//	BOOL AddWargameResult(UINT nUserRace,UINT nScore);///��ӻ���ֽ��

//	BOOL ReplyWargameResult(CUserPtr pUser,UINT nPage);///ս�����������û���Ҫ�����ݷ�����

	bool IsMaxPlayer(OBJID idWarGame);
	bool IsMaxSigninPlayer(OBJID idWarGame);
	bool IsHasSignup(OBJID idWarGame, OBJID idUser);///�Ƿ��Ѳμӻ��.

	bool CanUserEnter(OBJID idWarGame);
	bool EnterWar(OBJID idWarGame, CUser* pUser);
	bool LeaveWar(OBJID idWarGame, CUser* pUser);

	bool GetRebornPos(OBJID idWarGame, OBJID idUser, int &idMap, int &nPosX, int &nPosY);
	int  SendScoreRank(OBJID idWarGame, CUser* pUser);//��ѯ���������ְ�
	void ProcessReborn(OBJID idWarGame, OBJID idUser);

	void SynchroPlayerAmount(OBJID idWarGame, WARGAME_PLAYERSCORE* pInfo, int nAmount);

	CGameMap* GetWarGameMap(OBJID idWarGame, CUser* pUser, int& nPosX, int& nPosY);
	void LoadMap();
	void BroadCaseEnterMsg(OBJID idWarGame);

	void PlayerLogout(OBJID idWarGame, CUser* pUser);

	void BroadCastCampMsg(OBJID idWarGame, OBJID idSender, CNetMsg* pMsg);//��Ӫ����

private:
	IDatabase* m_pDB;///�ڲ�����һ�����ݿ��ʵ��ָ��,���ڷ������ݿ��е��ʼ���
	PROCESS_ID m_idProcess;///��ͼ���߳�ID
	CWargameList m_WarGameList;///����ս����б�
	//CRaceExchangeRateList m_RaceExchangeRateList;
};

#endif // !defined(AFX_WARGAMEMANAGER_H__6B300A42_8F2F_4260_A6E9_C79B6C3454F2__INCLUDED_)
