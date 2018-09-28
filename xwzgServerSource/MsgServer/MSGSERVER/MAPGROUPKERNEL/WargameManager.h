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

///战争活动管理器,地图组全局唯一对象
class CWargameManager  ///: public Singleton<CWargameManager>
{
public:
	CWargameManager();
	virtual ~CWargameManager();

	///初始战争活动管理器
	BOOL Init(IDatabase* pDB,PROCESS_ID idProcess);
	//BOOL Release();///析构战争活动管理器
	void OnTimer(time_t tCurr);///定时器处理函数
//	void ProcessMsg(CMsgWargameSync* pMsg);///处理消息函数

	bool AddWargame(CWargame* war);///添加一个战争活动
	bool DelWargame(CWargame* war);///删除一个战争活动
	CWargame* GetWargame(OBJID nWargameID);///通过战争活动typeid得到一个战争活动对象指针
	UINT GetSize();///得到容器内活动个数
	bool CanSignup(OBJID idWarGame);///活动是否可以开始报名了.
	bool Signup(OBJID idWarGame, CUser* pUser);///活动报名.
	bool Signup(OBJID idWarGame, WARGAME_PLAYERSCORE* pInfo);

	int  GetCurrentStatus(OBJID idWarGame);
	void OnPlayerKill(IRole* pAtker, IRole* pTarget, OBJID idWarGame);///玩家被杀
	//void OnUserLogin(CUserPtr pUser);

	///得到指定种族的兑换比率.
//	UINT GetRaceExchangeRate(UINT nUserRace){return m_RaceExchangeRateList[nUserRace];}

//	BOOL AddWargameResult(UINT nUserRace,UINT nScore);///添加活动积分结果

//	BOOL ReplyWargameResult(CUserPtr pUser,UINT nPage);///战争管理器将用户需要的数据发给他

	bool IsMaxPlayer(OBJID idWarGame);
	bool IsMaxSigninPlayer(OBJID idWarGame);
	bool IsHasSignup(OBJID idWarGame, OBJID idUser);///是否已参加活动了.

	bool CanUserEnter(OBJID idWarGame);
	bool EnterWar(OBJID idWarGame, CUser* pUser);
	bool LeaveWar(OBJID idWarGame, CUser* pUser);

	bool GetRebornPos(OBJID idWarGame, OBJID idUser, int &idMap, int &nPosX, int &nPosY);
	int  SendScoreRank(OBJID idWarGame, CUser* pUser);//查询竞技场积分榜
	void ProcessReborn(OBJID idWarGame, OBJID idUser);

	void SynchroPlayerAmount(OBJID idWarGame, WARGAME_PLAYERSCORE* pInfo, int nAmount);

	CGameMap* GetWarGameMap(OBJID idWarGame, CUser* pUser, int& nPosX, int& nPosY);
	void LoadMap();
	void BroadCaseEnterMsg(OBJID idWarGame);

	void PlayerLogout(OBJID idWarGame, CUser* pUser);

	void BroadCastCampMsg(OBJID idWarGame, OBJID idSender, CNetMsg* pMsg);//阵营聊天

private:
	IDatabase* m_pDB;///内部保存一个数据库的实例指针,用于访问数据库中的邮件表
	PROCESS_ID m_idProcess;///地图组线程ID
	CWargameList m_WarGameList;///保存战争活动列表
	//CRaceExchangeRateList m_RaceExchangeRateList;
};

#endif // !defined(AFX_WARGAMEMANAGER_H__6B300A42_8F2F_4260_A6E9_C79B6C3454F2__INCLUDED_)
