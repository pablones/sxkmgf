// Team.h: interface for the CTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEAM_H__096C275F_D6F9_40F1_8B39_471D69EBA7CC__INCLUDED_)
#define AFX_TEAM_H__096C275F_D6F9_40F1_8B39_471D69EBA7CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include <windows.h>
#include "Myheap.h"

#include <vector>
#include <algorithm>
#include "../share/protocol.h"
using namespace std;


const int   TUTOR_TEAM_ADD_EXP_PERSENT   	= 1;


const int	_MAX_TEAMAMOUNT			= 6;			//-------彭峰---2007/04/24-----修改成组队人数为5

const int	_MAX_TEAMAMOUNTOUT		= 8;// 申请的人数// By Minsky,2010-05-28

//[游途道標 2009.03.09]修改隊伍成員判定范圍為40格
//const int	_RANGE_EXPSHARE			= 32 + 360;		//  [2007-8-16] 朱斌 由32改为52,//  [2007-8-21] 朱斌 组队的有效范围在现有基础上增大20个屏（20*18=360格）。
const int	_RANGE_EXPSHARE			= 40;	
const int	_RANGE_TEAM_STATUS		= 12;			// 结界状态影响的范围半径
const int	MAX_TEAM_EXP_TIMES		= 360;			// 组队经验值的等级最大倍数
//---jinggy---帮派系统---begin
//const int	TEAMLEADER_WITHNEWBIE_REPUTEINC = 100;		//带低级的人练级1小时声望增加100
//---jinggy---帮派系统---end

enum {
	CHANGETEAM_ADD=1,
	CHANGETEAM_DEL,
	CHANGETEAM_DISMISS,
	CHANGETEAM_INFO,
	CHANGETEAM_FLY,
	CHANGETEAM_CHANGENAME,//20070202修罗:小队更名
	CHANGETEAM_CHANGLEADER,//20070205修罗:队长变更
	CHANGETEAM_CHANGLEADER_CLSEFFECT,//20070205修罗:队长变更.去掉原队长特效
	CHANGETEAM_CHANGLEADER_SETEFFECT,//20070205修罗:队长变更.添加新队长特效
	CHANGETEAM_CHANGCONFIG,//20070207修罗:小队设置变更.
	CHANGETEAM_UPDATE_BROADCAST_SET,	//  [2007-9-27] 朱斌	更新状态
	CHANGETEAM_QUERY_MEMINFO,      //查询成员信息
	CHANGETEAM_TEAMTEXT,           //更改队伍信息
	CHANGETEAM_PICKMONEY,          //分到队友捡的金钱
	CHANGETEAM_CREATE,
	CHANGETEAM_DES,
	CHANGETEAM_ONLINESTATUSCHANGE,
	CHANGETEAM_SETLOCATION,//队员在线状态改变
	CHANGETEAM_REQUESTCHANGLEADER,//请求自主决定队长变更,只有在队长所在地图组后继无人的情况下才发送
	CHANGETEAM_CLRLOCATION
};

//20070206修罗:加入结构体.传地图组
//int							m_nTeamMode;
//int							m_nExpMode;
//int							m_nItemMode;
//char						m_sTeamName[24];
//struct TeamInfoStruct{
//	OBJID	id;
//	OBJID	idLeader;
//	OBJID	setMember[_MAX_TEAMAMOUNT];
//	BOOL	bCloseMoney;
//	BOOL	bCloseItem;
//	int     nLeaderlev;
//	//------------------
//	int							nTeamMode;
//	int							nExpMode;
//	int							nItemMode;
//	char						sTeamName[24];
//	//------------------
//
//	//------------------
//	char                        m_sLeadName[16];
//	int                         m_nLev;
//	int                         m_nTarget;
//	char                        m_sTeamText[96];
//};

class CNetMsg;
class IRole;
class CTeam  
{
public:
	CTeam(PROCESS_ID idProcess);
	virtual ~CTeam();

public:
	static	CTeam*		CreateNew		(PROCESS_ID idProcess, OBJID m_idTeam, OBJID idLeader,bool bSend);
	static	CTeam*		CreateNew		(PROCESS_ID idProcess, TeamInfoStruct* pInfo);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	OBJID	GetID()				{ return m_id; }
	void	GetInfo(TeamInfoStruct* pInfo);
	void	Destroy				();

public:
	BOOL	AddMember			(OBJID idMember, bool bOwnerMap = true);
	BOOL	DelMember			(OBJID idMember);
	void	Dismiss				(OBJID idLeader);
	void    SetOnlineStatus(OBJID idMember,BOOL bStatus,BOOL bTransmitMsg=false);///改变队员在线状态,0为离线,1为在线
	void    SetLocation(OBJID idMember,OBJID idMap,int x,int y,BOOL bTransmitMsg=false);///下线时保存一下队员的位置
	void	ResetLocation(BOOL bTransmitMsg);
	void    ResetOfflineLocation(BOOL bTransmitMsg=false);///将所有离线队员的保存的副本地图位置重置为0
	//20070202修罗:小队更名
	void	ChangeTeamName(char *teamName);
	//20070203修罗:队长变更
	void	ChangeTeamLeader(OBJID leaderID);
	//20070205修罗:队长设置.仅设置队长.不处理别的
	void	SetTeamLeaderOnly(OBJID leaderID);
	void    SetTeamLeaderName(const char *leaderName);
	//20070207修罗:修改设置
	void	ChangeTeamInfo(int teamInfo);
	void	ChangeTeamInfoOnly(int teamInfo);//仅仅修改
	void    ChangeTeamText(char *pText);

	void	Open				();
	void	Close				();
	BOOL	IsClosed			();
	void	OpenMoney			();
	void	CloseMoney			();
	BOOL	IsCloseMoney		()			{ return m_bCloseMoney; }
	void	SetCloseMoney		(bool flag)	{ m_bCloseMoney = flag; }
	void	OpenItem			();
	void	CloseItem			();
	BOOL	IsCloseItem			()			{ return m_bCloseItem; }
	void	SetCloseItem		(bool flag)	{ m_bCloseItem = flag; }
	void	OpenGemAccess		();
	void	CloseGemAccess		();
	BOOL	IsCloseGemAccess	()			{ return m_bCloseGemAccess; }
	void	SetCloseGemAccess	(bool flag)	{ m_bCloseGemAccess = flag; }

	OBJID	GetMemberByIndex	(int nIndex);
	int		GetMemberAmount		();
	int		GetOnlineMemberAmount(int nSex = 2 /* 2是找所有人数，0找男，1找女*/);
	bool	GetMemberInfo		(TeamMemberInfo& menberInfo,OBJID idUser);
	bool    IsMemberOnline(OBJID idUser);
	bool	UpdateMemberInfo();

	void	DelBuffByTutor( OBJID UserID );// pengfeng 组队师徒加BUFF [10/26/2007]
	void	AddBuffByTutor( OBJID UserID );// pengfeng 组队师徒加BUFF [10/26/2007]
	void	DelBuffByTutorOnDisband();//队长把队伍解散删除特定BUFF// pengfeng [10/26/2007]
	OBJID   GetTeamMemberOnly();///仅得到成员非队长
	OBJID   GetOnlineTeamMemberOnly();///仅得到在线成员非队长
	OBJID   GetNextTeamLeader();///仅得到在线非队长成员,优先在副本内
	BOOL	IsTeamMember		(OBJID idMember);
	BOOL	IsTeamWithNewbie	(OBJID idKiller, IRole* pTarget);
	BOOL	IsTeamWithTutor		(OBJID idKiller, IRole* pTarget);
	BOOL	IsAccountTeamMember	(OBJID idAccount);///[2009.2.12--张冬]此账号的用户是否是队员

	void	SetLeader			(OBJID idLeader);
	OBJID	GetLeader			();

	// 用于结界系统的
	void	AttachMemberStatus	(int nStatus, int nPower, int nSecs, int nTimes, OBJID idExclude);
	void	AdjustMemberStatus	(int nOldAmount, int nNewAmount);
	int		ShareDamage			(int nDamage, OBJID idExclude);
	bool	CheckActive(CUser* pUser);
// 	void	BroadcastTeamMsg	(CNetMsg* pMsg, OBJID idExclude=ID_NONE);
	void	BroadcastTeamMsg	(CNetMsg* pMsg, CUser* pSender=NULL,bool bSendOtherLine = true);
	void	BroadcastMemberLife	(CUser* pMember, bool bMaxLife=false);
	void	BroadcastMemberMana(CUser* pMember, bool bMaxMana);
	void	BroadcastMemberLev(CUser* pMember, bool bLev=true);
	//void	BordcastTeamMsg		(int nChannel,const char* pMsg);
	OBJID	GetTeamMap() const { return m_idTeamMap; }
	void	SetTeamMap(OBJID val,BOOL bTransmitMsg=false); 
public: // call by transmit from other mapgroup
	int     GetTeamAddExp(int nExp, int nMemberCount);
	bool    DevolveOnLeader(int newLeaID);
//	void DealTeamLeaderInfo();
	void	AddMemberOnly		(TeamMemberInfo menberInfo);
	void	DelMemberOnly		(OBJID idMember);
	//061124hunter
	void    SetTeamTarget(int nTarget){ m_nTarget = nTarget;}
	int     GetTeamTarget(){ return m_nTarget;}
	void    SetTeamLev(int nLev){ m_nLev = nLev;}
	int     GetteamLev(){ return m_nLev;}
	void    SetTeamText(char *text){ strcpy(m_sTeamText, text);}
	char*   GetTeamText(){ return m_sTeamText;}
	void	SetTeamMode(int mode){	m_nTeamMode = mode; }
	int		GetTeamMode()	{	return	m_nTeamMode; }
	int		GetMemberLev();
	void	SetExpMode(int mode){	m_nExpMode = mode; }
	int		GetExpMode()	{	return	m_nExpMode; }
	void	SetItemMode(int mode){	m_nItemMode = mode; }
	int		GetItemMode()	{	return	m_nItemMode; }
	void	SetTeamName(char*name){strcpy(m_sTeamName,name);}
	char*	GetTeamName()	{return m_sTeamName;}
	char*   GetTeamLeaderName() {return m_sLeadName;}
	map<OBJID,bool>& GetShareStatusSet(){return m_setUserStatus;}
	void    AwardBufForTeacher();// 给组队的师徒加BUF [陈剑飞10/26/2007]
	TeamMemberInfo              m_TeamIn[6];
private:
	BOOL	Create				(OBJID idLeader,OBJID m_idTeam,bool bSend);
	void	Init				();
	CTeam*	CloneNew();

protected:
	typedef	vector<OBJID>		USERIDSET;
	USERIDSET					m_setIdUser;
	USERIDSET					m_setAccountIdUser;///[2009.2.12--张冬]储存队员的账号ID
	map<OBJID,bool>				m_setUserStatus;///指示该玩家是否在经验共享区

private:
	OBJID						m_id;
	OBJID						m_idLeader;
	PROCESS_ID					m_idProcess;
	BOOL						m_fClosed;
	BOOL						m_bCloseMoney;
	BOOL						m_bCloseItem;
	BOOL						m_bCloseGemAccess;
	int							m_nTeamMode;
	int							m_nExpMode;
	int							m_nItemMode;
	char						m_sTeamName[24];
	CTimeOut					m_tGuidingTime;  //带练时间
	char                        m_sLeadName[16];
	int                         m_nLev;
	int                         m_nTarget;
	char                        m_sTeamText[96];

	bool                        m_bEudemonBron;
	OBJID                       m_bronUser[2];
	OBJID                       m_bronEudemon[2];
	bool                        m_bEudemonBronLock[2];

	bool                        m_bEudemonPlay;
	OBJID                       m_playUser[2];
	OBJID                       m_playEudemon[2];
	bool                        m_bEudemonPlayLock[2];

	OBJID						m_idTeamMap;///队长所在地图id
//	TeamMemberInfo              m_TeamOut[6];

protected: // ctrl
	MYHEAP_DECLARATION(s_heap)

// By Minsky,2010-05-28，以下为新添加的组队的申请队员的管理
private:
	USERIDSET					m_setIdApply;// By Minsky,2010-05-30,申请中的队员的列表
public:
	void	AddMemberApply		(OBJID idMember);
	bool	DelMemberApply		(OBJID idMember);
	OBJID	GetMemberApplyByIndex(int nIndex);
	int		GetMemberApplyAmount		();

	bool    initEudemonBron();
	bool    initEudemonPlay();
	bool    EudemonBronChange(OBJID idUser,OBJID idEudmon);
	bool    EudemonBronChange(OBJID idUser,bool idLock);
	bool    EudemonBron();
	bool    EudemonCancel();
	bool	EudemonPlayChange(OBJID idUser,OBJID idEudmon);
	bool	SendEudemonBronInfo(OBJID idUser,OBJID idUserTarget = ID_NONE);
	bool	SendEudemonPlayInfo(OBJID idUser,OBJID idUserTarget = ID_NONE);
	bool	EudemonPlayChange(OBJID idUser,bool idLock);
	bool	EudemonPlayCancel();
	bool	EudemonPlay();
	bool	CheckTutorActive(CUser* pLeader);//查看队伍中徒弟是否都在附近
	bool    CheckIsSameSyn(OBJID idsyn);
};

#endif // !defined(AFX_TEAM_H__096C275F_D6F9_40F1_8B39_471D69EBA7CC__INCLUDED_)
