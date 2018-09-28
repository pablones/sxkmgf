// Syndicate.h: interface for the CSyndicate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNDICATE_H__78C1D98F_7DA8_482D_9819_22AD24402CB6__INCLUDED_)
#define AFX_SYNDICATE_H__78C1D98F_7DA8_482D_9819_22AD24402CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../share/NetWorkDef.h"
#include "../share/protocol.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include <vector>
#include "MsgSynInfo.h"
#include "TimeOut.h"
#include "math.h"
#include "..\SHARE\define.h"	// Added by ClassView
using namespace std;

const int	SYN_MONEY_BASELINE		= 0;					// 帮派基金少于该值，会限制帮派功能

//////////////////////////////////////////////////////////////////////
enum {
		SYNFUNC_DEMISE=1,
		SYNFUNC_SETTENET,
		SYNFUNC_SETLEADERTITLE,
		SYNFUNC_SETMEMBERTITLE,
		SYNFUNC_ADDDATA,
		SYNFUNC_SETDATA,
		SYNFUNC_PRESENT,
		SYNFUNC_ADD_MEMBER,
		SYNFUNC_DEL_MEMBER,
		SYNFUNC_UP_LEVEL,			//军团成员人物等级提升
		SYNFUNC_SETSYNNAME,
		SYNFUNC_SET_RANK,
		SYNFUNC_PAYOFF,//20070414修罗:发工资
		SYNFUNC_SET_MEMBERPROFFER,//20070417修罗:同步世界线程成员贡献值
		SYNFUNC_LOGIN,//[游途道标 2008/9/25]玩家登陆通报
		SYNFUNC_LOGOUT,//[游途道标 2008/9/25]玩家推出通报
		SYNFUNC_LEAVERANK,
		SYNFUNC_SYNATTACKID,
		SYNFUNC_SYNDEFENCEID,
		SYNWAR_DEC,
		SYNWAR_JOIN,
		SYNWAR_RELULT,
		SYNWAR_CLEAR,
};

//////////////////////////////////////////////////////////////////////
const int	MAX_SYNENEMYSIZE		= 5;		// 一共5个敌对帮派空间
const int	MAX_SYNALLYSIZE			= 5;		// 一共5个结盟帮派空间
enum SYNDATA{
		SYNDATA_ID=0,			// 仅用于INSERT
		SYNDATA_NAME=1,			// 帮派名
		SYNDATA_ANNOUNCE,		// 帮派公告
		SYNDATA_LEADERID,		// 帮主ID
		SYNDATA_LEADERNAME,		// 帮主名字
		SYNDATA_CREATELEADERNAME,	// 创建帮主名字
		SYNDATA_CREATETIME,
		SYNDATA_STATE,
		SYNDATA_LEVEL,			//主堂等级
		SYNDATA_SUBLEV1,//分堂一等级
		SYNDATA_SUBLEV2,//分堂二等级
		SYNDATA_SUBLEV3,//分堂三等级
		SYNDATA_SUBLEV4,//分堂四等级
		SYNDATA_GROW,
		SYNDATA_SUBGROW1,
		SYNDATA_SUBGROW2,
		SYNDATA_SUBGROW3,
		SYNDATA_SUBGROW4,
		SYNDATA_MONEY,
		SYNDATA_AMOUNT,
 		SYNDATA_ACTIVEAMOUNT,
		SYNDATA_DATA1,
		SYNDATA_SUB1_SKILL1,
		SYNDATA_SUB1_SKILL2,
		SYNDATA_SUB1_SKILL3,
		SYNDATA_SUB2_SKILL1,
		SYNDATA_SUB2_SKILL2,
		SYNDATA_SUB2_SKILL3,
		SYNDATA_SUB3_SKILL1,
		SYNDATA_SUB3_SKILL2,
		SYNDATA_SUB3_SKILL3,
		SYNDATA_SUB4_SKILL1,
		SYNDATA_SUB4_SKILL2,
		SYNDATA_SUB4_SKILL3,
		SYNDATA_SUB1_BUILDER,
		SYNDATA_SUB2_BUILDER,
		SYNDATA_SUB3_BUILDER,
		SYNDATA_SUB4_BUILDER,
		SYNDATA_SUB1_KONGFU,
		SYNDATA_SUB2_KONGFU,
		SYNDATA_SUB3_KONGFU,
		SYNDATA_SUB4_KONGFU,
		SYNDATA_SUB1_BUSMAN,
		SYNDATA_SUB2_BUSMAN,
		SYNDATA_SUB3_BUSMAN,
		SYNDATA_SUB4_BUSMAN,
		SYNDATA_SUB1_START,
		SYNDATA_SUB2_START,
		SYNDATA_SUB3_START,
		SYNDATA_SUB4_START,
		SYNDATA_PAY,
		SYNDATA_MAPID,
};
char	szSynTable[];
typedef	CGameData<SYNDATA, szSynTable, szID>	CSynBase;

const int   DISMISSSYN_MONEY_LIMIT  = 100000;           // 金钱少于此数，自动解散。//add huang 2004.1.5
const int	DISMISSSYN_AMOUNT_LIMIT	= 20;				// 帮众少于此数，自动解散  
const int   DISMISSSYN_TIME_LIMIT   = 24*60*60;         // 自动解散帮派时限 24小时

const int	AUTOCHECK_TIME			= 30;	//检测降级或解散的时间周期,30天

const int   SYNWARTYPE_NONE         = 0;    //无
const int   SYNWARTYPE_ATTACK       = 1;    //帮战攻击
const int   SYNWARTYPE_DEFENCE      = 2;    //帮战防守

const int   CITYSYNWAR_NONE         = 0;    //无类型
const int   CITYSYNWAR_CITYWAR      = 1;    //城战类型
const int   CITYSYNWAR_SYNWAR       = 2;    //帮战类型

const int SPENDBUILD = 1;
const int SPENDACTIV = 2;
const int SPENDMONEY = 3;

//////////////////////////////////////////////////////////////////////
// class ISynModify
// {
// public:
// 	virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave=true)		PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetTenet(LPCTSTR szTenet, bool bSave=true)													PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave=true)										PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave=true)										PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	AddData(SYNDATA idx, int nData, bool bSave=true)											PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetData(SYNDATA idx, int nData, bool bSave=true)											PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave=true)										PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetSynName(LPCTSTR szSynName, bool bSave=true)												PURE_VIRTUAL_FUNCTION_0
// 	//20070414修罗:发工资dwMoney为总钱数
// 	virtual bool	PayOff(DWORD dwMoney,BOOL bSave=true)														PURE_VIRTUAL_FUNCTION_0
// 	//20070417修罗:同步世界线程内成员贡献值
// 	virtual bool	SetMemberProffer(int dwProffer,OBJID idMember,BOOL bSave=true)								PURE_VIRTUAL_FUNCTION_0
// };
typedef struct	MemberInfo{
	OBJID	id;
	char	szName[16];	
	USHORT	level;
	USHORT	firJob;
	USHORT	sex;
	USHORT	rank;
	USHORT  line;
	USHORT	vip;
	OBJID   lastlogin;
	OBJID   hiscon;
	OBJID   hismoney;
	OBJID   hisgrow;
	OBJID	maxpower;

}MEMBERINFO;
enum 
{
	MemberList=5,
		ConList,
		QueryList,
		AddMember=8,
		DelMember=9,
		AddQuery=10,
		DelQuery=11,
		SetRank= 12, 
};
//////////////////////////////////////////////////////////////////////
// 所有成员函数为原子操作。
class CSyndicate : public CGameObj/*, ISynModify*/
{
protected:
	CSyndicate();
	virtual ~CSyndicate();
	vector<OBJID> m_SynMonsterList;
public:
	static CSyndicate* CreateNew() { ASSERT(!"CSyndicate::CreateNew()"); return NULL; }				// 用于CSynPtr
	static CSyndicate* CreateNew(PROCESS_ID idProcess)	{ CSyndicate* ptr=new CSyndicate; if(ptr) ptr->m_idProcess=idProcess; return ptr; }
	virtual ULONG	ReleaseByOwner()				{ delete this; return 0; }
	bool	Create(IRecordset* pRes);
	bool    Create(CMsgSynInfo& msg);
	bool	Create(CreateSyndicateInfo* pInfo,bool bSave);
	void	CreateSynMap();
	void	SetInSynMapID();

//	ISynModify*		QueryModify()	{ return (ISynModify*)&m_obj; }
	bool			ChangeSyndicate( const SynFuncInfo0 * pFuncInfo);
	bool			ReloadData();

	int				GetKill() { return m_warKill; }
	void			SetKill(int kill) { m_warKill = kill; }
	void	DelSynMonster();
	void	AddSynMonster(CMonsterPtr pMonster);
	void CSyndicate::Lock()
	{
		///未锁定状态时则
		m_bLock=true;
	}
	
	bool CSyndicate::HasLock()
	{
		///未锁定状态时则
		return m_bLock;
	}
	void CSyndicate::Unlock()
	{
		m_bLock=false;
	}
	bool CanUpLevel(int nSub,CUserPtr pUser=NULL);
	int				GetState() { return warState; }
	void			SetState(int state) { warState = state; }
	LPCTSTR			GetRankTitle(int Rank);
public: // const
	OBJID	GetID()							{ return m_pData->GetKey(); }
	int		GetInt(SYNDATA idx);
	LPCTSTR	GetStr(SYNDATA idx)				{ return m_pData->GetStr(idx); }
	void	SetInt(SYNDATA idx,int nData)	{ CHECK(m_pData);if(nData == GetInt(idx) )return;m_pData->SetInt(idx,nData);}
	void	BroadcastSynMsg(CNetMsg* pMsg, CUser* pExcludeSender=NULL);
	void	BroadcastSubSynMsg(SOCKET_ID idSocket, OBJID idNpc, LPCTSTR szMsg, CUser* pExcludeSender=NULL, LPCTSTR pszEmotion=NULL, DWORD dwColor=0xffffff, int nAttribute=_TXTATR_SYNDICATE);
	void	BroadcastLocalSynMsg(CNetMsg* pMsg, CUser* pExcludeSender=NULL);
	LPCTSTR	GetFealtyName();
	bool	IsNearSyn(OBJID idSyn);
	CSyndicate*		GetMasterSyn();
	bool	IsFriendly(OBJID idSyn, BOOL bNest=true);		// mast be true
	bool	IsHostile(OBJID idSyn, BOOL bNest=true);		// mast be true
	bool	IsNeutral(OBJID idSyn);
	bool	IsWhiteSynMapOwner();
	bool	IsBlackSynMapOwner();
	int		GetMemberAmount(bool bIncludeSub = true);
	int		GetShopMemberAmount(bool bIncludeSub = true);
	int		GetQueryMemberAmount(bool bIncludeSub = true);
	int		GetSynAmount();
	bool    ProcAutoDismissOrDestory();
	void	ProcAutoSynWar();
	//////新功能--jay
	void	SetSynMapID(OBJID idmap){CHECK(idmap); m_nMapID=idmap;}
	OBJID	GetSynMapID(){return m_nMapID;}
	void	TransmitWorldMsg(CNetMsg* msg);
	void	SendMenberListToClient(CUser* pUser);
	void    SendConListToClient(CUser* pUser,int nPage = 1);
	void    SendQueryListToClient(CUser* pUser,int nPage = 1);
	void	GetMemberList(vector<CUserPtr> &User);
	void	ReSetSynName(const char* szName);
	void	ReSetSynMemberName(OBJID nUserid,const char* szName);
	void	SetCallTask(bool Task){m_bCallTask=Task;}
	bool	GetCallTask(){return m_bCallTask;}
private:
	OBJID m_nMapID;
protected:
	bool    CheckDismissTime();    // add huang 2004.1.7
	bool    CheckDismissSyn();  
	int		AmontStatue;
	int		MaintainSta;


public: // application
//	void	SendQueryMenberToClient(CUser* pUser,int nPage = 1);
	void	SendInfoToClient(CUser* pUser);
	void	SendAllInfoToClient(CUser* pUser);
	void	SendSynInfoToClient(CUser* pUser);
//	void	SendMenberToClient(CUser* pUser,int nPage = 1);
	void    BroadcastSynMemberInfoMsg(OBJID idMember);
	void    BroadcastSynMemberStatusChangeMsg(OBJID idMember);

	void	SetNewSubName(LPCTSTR szName);		// 临时代码
	
	bool	ClearAllySyn(CSyndicate* pTargetSyn);
	bool	ClearEnemySyn(CSyndicate* pTargetSyn);

public: // link -------------------------------------
	CAutoLink<CSyndicate>&	QueryLink	()					{ return m_link; }
protected:
	CAutoLink<CSyndicate>	m_link;

public: // syn war -------------------------------------
	void	AddSynWarScore(OBJID idNpc, int nScore);
	int		GetSynWarScore(OBJID idNpc);
	void	ClearSynWarScore(OBJID idNpc);
protected:
	struct	SynWarInfo{
		OBJID	idNpc;
		int		nScore;
	};
	typedef	vector<SynWarInfo>	SYNWAR_SET;
	SYNWAR_SET		m_setSynWarScore;
	int             warState,warTurn;
	int             m_warKill;
protected: // modify, mast be atom operator. test & set
	// bSave 必须为true，否则不会存盘
	virtual bool	AddData(SYNDATA idx, int nData, bool bSave);
	virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave);
	virtual bool	SetTenet(LPCTSTR szTenet, bool bSave);
	virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave);
	virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave);
	virtual bool	SetData(SYNDATA idx, int nData, bool bSave);
	virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave);
	virtual bool	SetSynName(LPCTSTR szSynName, bool bSave);
	virtual bool	PayOff(DWORD dwMoney,BOOL bSave/* =true */);//20070414修罗:发工资.仅处理内存.


public:
	int GetMoney();
	void  SetSynWar(OBJID warID, bool bSave = true);
//	void  SetRepute(int data, bool bSave = true) {SetData(SYNDATA_REPUTE,data,bSave);}
	void  SetMoney(int data, bool bSave = true) {SetGrow(SPENDMONEY,data,bSave);}
	bool			SetLastBonusTime( LPCTSTR szTime );
//	LPCTSTR			GetLastBonusTime()					{ return GetStr( SYNDATA_LASTBONUSTIME ); }
	//20070331修罗:军团任务相关-------------------------
	//所有Set...()仅由世界线程回调时调用.不允许update.所有update操作在世界线程完成
	//需要同步到各地图组时,调用ReqSet...()函数
	BOOL			ReqSetTaskPublishTime(DWORD curTime);//请求世界线程,回调时方可修改且仅修改本地图组内存数据
//	BOOL			SetTaskPublishTime(DWORD curTime,BOOL bUpdate=UPDATE_NEVER);//世界线程回调,设置军团任务发布时间
//	DWORD			GetTaskPublishTime(){return GetInt(SYNDATA_TASKPUBLISHTIME);}//获得军团任务发布时间

	BOOL			ReqSetActionID(DWORD actionID);//请求世界线程,回调时方可修改且仅修改本地图组内存数据
//	BOOL			SetActionID(DWORD actionID,BOOL bUpdate=UPDATE_NEVER);//世界线程回调,设置军团任务action
//	DWORD			GetActionID(){return GetInt(SYNDATA_ACTIONID);}//获得军团action

	//20070412修罗:军团功能扩展
	BOOL			ReqSetSynProffer(int synProffer);//请求世界线程设置军团贡献值
//	int				GetSynProffer(){return GetInt(SYNDATA_SYNPROFFER);}//获得军团贡献值

	BOOL			ReqPayOff(DWORD dwMoney);//请求世界线程发工资
	BOOL			ReportMemberProfferToWolrd(int dwProffer,OBJID idMember);//汇报成员贡献值到世界线程.
	//--------------------------------------------------

	BOOL			ReqSetSynName(LPCTSTR szNewSynName);//20070825修罗:请求世界线程军团改名
private:
	CSynBase*	m_pData;
	bool		m_bLock;///1表示锁定,0表示未锁
protected: // ctrl
	PROCESS_ID		m_idProcess;
	CTimeOut t_Maintain;
	CTimeOut t_Update;
	CTimeOut t_PayEmploy;

	MYHEAP_DECLARATION(s_heap)
//protected:
// 	class	XSynModify : public ISynModify
// 	{
// 	public:
// 		virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave);
// 		virtual bool	SetTenet(LPCTSTR szTenet, bool bSave);
// 		virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave);
// 		virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave);
// 		virtual bool	AddData(SYNDATA idx, int nData, bool bSave);
// 		virtual bool	SetData(SYNDATA idx, int nData, bool bSave);
// 		virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave);
// 		virtual bool	SetSynName(LPCTSTR szSynName, bool bSave);
// 		//20070414修罗:向世界组请求发工资
// 		virtual bool	PayOff(DWORD dwMoney,BOOL bSave);//bSave=true
// 		virtual bool	SetMemberProffer(int dwProffer,OBJID idMember,BOOL bSave/* =true */);//20070417修罗:同步世界线程内成员贡献值
// 	protected:
// 		CSyndicate*		This()		{ return m_pOwner; }
// 		CSyndicate*		m_pOwner;
// 		friend class CSyndicate;
// 	} m_obj;
public:
	int GetLeaderID();
	int GetSubLeaderID();
	int GetSubLeader2ID();
	int GetLeaderWifeID();
	void SetSubLeaderID(OBJID id);	
	void SetSubLeaderID2(OBJID id);
	void SetLeaderWifeID(OBJID id);
	void	SynchroInfo();

	OBJID	GetMasterSynID();
	bool	FallLevel();
	bool	IsCanFallLevel();
	bool	IsSubSubSyn();
	bool	IsSubSyn();
	int		GetSubSubSynNum();
	bool	IfCanCreateSubSubSyn();
	bool	IsMasterSyn()				{ return false; }
	//帮派每周会在统一的地方发放本周军团利润的一部分作为成员所得
	bool	AllotBonusForMember(CUserPtr pUser,DWORD dwBonus);
	bool	ApplyKickoutMember(CUserPtr pLeader,CUserPtr pTarget);
	int		GetSubSynNum();
//	friend class CSyndicate::XSynModify;
public:
	typedef	vector<MemberInfo>	MEMBER_SET;
private:
	MEMBER_SET m_setMember;
	MEMBER_SET m_setQueryMember;
	OBJID idLeader;
	OBJID idSubLeader;
	OBJID idSubLeader2;
	OBJID idFeelLeader;
	bool  m_bCallTask;
// 	OBJID idFeelLeader1;
// 	OBJID idFeelLeader2;
// 	OBJID idFeelLeader3;
// 	OBJID idFeelLeader4;
// 	OBJID idSubFeelLeader1;
// 	OBJID idSubFeelLeader2;
// 	OBJID idSubFeelLeader3;
// 	OBJID idSubFeelLeader4;
// 	int nAmount;
//	int nShpAmount;
// 	int nOnlineAmount;
	DWORD dwLastUpdate;

public:

	DWORD GetSkillExpDeadline(int nSub);
	bool IsStopSub(int nSub);
	int GetSkillLev(int nSub, int nSkill);
	void SetSkill(int nSub, int nData, bool bSendtoOtherline = true);
	UINT SetGrow(int nType, UINT nData, bool bSendtoOtherline = true);//(包括资金,建设)会发送客户端即时更新
	bool UpSubSkill(int nSub, int nSkill, char* szUserName = "");
	bool CheckSkillDeadline(int nSub);
	int GetSubAmount();
	UINT GetSkillExp(int nSub, int nSkill = 0);
	bool SpendMoney(int Syndata, UINT nData);
	bool CheckCondition(/*CUserPtr pUser, */int nSub);
	bool UpLevel(CUserPtr pUser, int nSub);
	void SetPay(int nPay);
	void SetDeadline();	
	void UpdateInfo(int nThing,OBJID nData,USHORT nSub=0);//0是建设，1是资金，2是等级
	void UpdateLevel(int sub=0);//
	const __int64 GetDeadline(int nThing, int Sub = 0);//1是繁荣建设资金 2是技能 3是帮派等级 4是各种工人上限 5是资金
	void ReSetStopsub(int nSub);
	void SetStopsub(int nSub);
	void SetLev(int nSub = 0, int nLev = 0);
	UINT GetLev(int nSub = 0);
	void ReduceLevel();
	void OnTimer(time_t tCur);
	bool PayBasic();
	bool NoPay();
//	UINT GetBusMan();
//	UINT GetBuilder();
//	bool PayEmployer();
	bool MaintainSynData();
	UINT GetGrow();
	LPCTSTR GetSubNameByID(OBJID sub);
//	bool SetBusMan(OBJID SubId, int Data1, int Data2, int Data3, const char* szName, int Money);//工匠武士商人
//	UINT GetBusMan(OBJID SubId);
//	void SetKONGFU(OBJID SubId, int Data);
//	UINT GetKONGFU(OBJID SubId);
//	void SetBuider(OBJID SubId, int Data);
//	UINT GetBuilder(OBJID SubId);
	void SendSubToC(CUserPtr pUser);
	void SendSciToC(CUserPtr pUser);

	MemberInfo* GetMemberFromList(OBJID idUser,int nType);

	bool Hasmember(OBJID idUser);
	bool HasQuerymember(OBJID idUser);
	void SetOnline(OBJID idUser,UCHAR cLine);
// 	void incOnline() { nOnlineAmount ++ ;}
// 	void DecOnline() { nOnlineAmount -- ; if(nOnlineAmount < 0) nOnlineAmount = 0;}
 	int GetOnlineAmount();
	int AddMemberList(MEMBERINFO info,int nType = 1); //1 帮会成员 2.申请列表
	int ChgMemberRank(OBJID idUser,int nRank = 10,bool bSend = true,CUser* pUser = NULL);
	bool DelMemberList(OBJID idUser,int nType = 1);
	bool AllowMenber(OBJID idUser,CUser* pUser);
	void SetStr(SYNDATA idx, LPCTSTR szData,int nSize,bool bSave = false)				{ m_pData->SetStr(idx,szData,nSize); if(bSave)  m_pData->Update();}
};

#endif // !defined(AFX_SYNDICATE_H__78C1D98F_7DA8_482D_9819_22AD24402CB6__INCLUDED_)
