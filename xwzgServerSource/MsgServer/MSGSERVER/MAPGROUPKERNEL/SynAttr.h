// SynAttr.h: interface for the CSynAttr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNATTR_H__7205A42E_F451_4A87_9BBD_37397D6578E2__INCLUDED_)
#define AFX_SYNATTR_H__7205A42E_F451_4A87_9BBD_37397D6578E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "user.h"
#include "Myheap.h"
#include "AutoLink.h"
#include "Syndicate.h"
#include "..\SHARE\define.h"	// Added by ClassView

#define	DEFAULT_TENET				""
#define	DEFAULT_LEADERTITLE			STR_SYNDICATE_LEADER
#define	DEFAULT_MEMBERTITLE			STR_SYNDICATE_MEMBER
//---jinggy---2004-12-7---define---begin
//两个月在线时间统计,10小时
#define TIMEONLINE_CHECK		5*60	
//---jinggy---2004-12-7---define---end
//////////////////////////////////////////////////////////////////////
const int	DEFAULT_PROFFER			= 0;				// 新人的贡献
const int	SYNMONEY_LIMIT			= 2000000000;		// 基金的限制额
const int	SYNPROFFER_LIMIT		= 1000000000;		// 贡献的限制额
const int	MAX_ASSISTANTSIZE		= 1;//5;				// 最多有多少个副帮主，暂定//20070822修罗:策划需求.1
const int	SYN_MEMBER_LIMIT		= 800;				// 限制最多招收多少个帮众

//---jinggy---2004-12-7---基金---begin
const int	MONEY_PER_MEMBER		= 10000;			// 招人增加的基金
const int	MONEY_PER_MEMBER_OUT	= 20000;			// 主动脱离团队减少的基金
const int	MONEY_KICKOUT			= 10000;			// 开除人员扣除的基金			//王玉波修改
//---jinggy---2004-12-7---基金---end

//---jinggy---2004-12-7---声望---begin
//王玉波修改参数
const int	REPUTE_PER_MEMBER		= 50;			// 招人增加的声望50
const int	REPUTE_PER_MEMBER_OUT	= 100;			// 主动退出减少军团声望100
const int	REPUTE_KICKOUT			= 50;			// 开除人员扣除的声望50
//---jinggy---2004-12-7---声望---end
//---jinggy---贡献度---begin

//---jinggy---贡献度---end

//////////////////////////////////////////////////////////////////////
struct SynAttrInfoStruct
{
	OBJID		id;
	OBJID		idSyn;
	USHORT		usMantle;
	UCHAR		ucRank;
	UCHAR		ucRankShow;
	int 		nProffer;
	int			nProfferInc;
	int			nTitle;
	UCHAR		ucLevel;
	OBJID		nhisGrow;
	OBJID		nhisMoney;
	OBJID		nhisCon;
	//20070414修罗:修正并增加
	int			nTimeDetected;
	int			nTimeOnline;
	int			nTaskBeginTime;
	DWORD		dwTaskMaskWeek;//20070418任务掩码.周
	DWORD		dwTaskMaskDay;//20070418任务掩码.天
};

enum KICKOUTDATA
{
	KICKOUTDATA_IN_BYUSER = 1,
	KICKOUTDATA_IN_BYKICK,

	KICKOUTDATA_OUTCHECK_BYUSER = 101,
	KICKOUTDATA_OUTCHECK_BYKICK,

	KICKOUTDATA_OUTCHECK_BYUSER_END = 201,
	KICKOUTDATA_OUTCHECK_BYKICK_END,

	KICKOUTDATA_OUTDO_TRUE = 1001,
	KICKOUTDATA_OUTDO_FALSE,
};
//////////////////////////////////////////////////////////////////////
enum SYNATTRDATA
{
	SYNATTRDATA_ID=0,		     // 用于INSERT
	SYNATTRDATA_SYNID=1,
	SYNATTRDATA_RANK,
	SYNATTRDATA_PROFFER,
	SYNATTRDATA_jointime,
	SYNATTRDATA_GIFT1,
	SYNATTRDATA_GIFT2,
	SYNATTRDATA_MASK,
	SYNATTRDATA_HISTORY_CON,
	SYNATTRDATA_HISTORY_MONEY,
	SYNATTRDATA_HISTORY_GROW,
};
char	szSynAttrTable[];
typedef	CGameData<SYNATTRDATA, szSynAttrTable, szID>		CSynAttrData;
//---------------------begin-----------------------------------------------------
//06.10.26王玉波添加,军团成员信息变更用到
enum SYNMEMBERINDEX
{
	MEMBERINDEX_NAME = 0,
	MEMBERINDEX_RANK,
	MEMBERINDEX_LEVEL,

};
//---------------------end-------------------------------------------------------

//////////////////////////////////////////////////////////////////////
class CSynAttr  
{
protected:
	virtual ~CSynAttr();

public:
	CSynAttr();
	static CSynAttr* CreateNew(PROCESS_ID idProcess, CUser* pUser);
	ULONG	ReleaseByOwner()						{ delete this; return 0; }
//	void	OnTimer(DWORD tCurr);

public: // login
	bool	Create();
	bool	SendInfoToClient();

public: // MapGroup
	bool	SaveInfo();
	bool	GetInfo(SynAttrInfoStruct* pInfo);
	bool	AppendSynAttr(SynAttrInfoStruct* pInfo);

public: // const
	OBJID	GetSynID()								{ if(m_pData) return m_pData->GetInt(SYNATTRDATA_SYNID); return ID_NONE; }
	
	int		GetInt(SYNATTRDATA idx)					{ CHECKF(m_pData); ASSERT(idx!=SYNATTRDATA_SYNID); return m_pData->GetInt(idx); }
	void	SetInt(SYNATTRDATA idx,int nData)       { CHECK(m_pData); if (nData == GetInt(idx)) return; m_pData->SetInt(idx,nData);}

	DWORD	GetSynRankShow		();
	LPCTSTR	GetRankTitle(int Rank = RANK_NORMAL);
//	int		SumSynMember();
	int		GetProffer(void)	const				{ if(m_pData) return m_pData->GetInt(SYNATTRDATA_PROFFER); return 0; }
	
	int		GetProfferLev();//20070412修罗:贡献等级.与经验奖励相关

	//---jinggy---得到贡献值的增量
//	int		GetProfferInc(void)	const				{ if(m_pData) return m_pData->GetInt(SYNATTRDATA_PROFFERINC); return 0; }
	
public: // modify
	int		GetBuilder(OBJID SubId);
	void	SetBuider(OBJID SubId, int Data);
	int		GetKONGFU(OBJID SubId);
	void	SetKONGFU(OBJID SubId, int Data);
	int		GetBusMan(OBJID SubId);
	void	SetBusMan(OBJID SubId, int Data);
	BOOL	CreateSyndicate(LPCTSTR szName, LPCTSTR szWord);
//	bool	CreateSubSyn(LPCTSTR szName, int nLevel,int nProffer, int nMoney, int nMoneyLeave);
	OBJID	CreateSubSyn(OBJID idFealty,const char* pszName);
	bool	DestroySyndicate();
	bool	JoinSyn(OBJID idSyn,bool bLeaveLine = false , OBJID idUser = ID_NONE,bool bSend=true);
	bool	CreateSyn(OBJID idSyn,OBJID idUser);
	bool	LeaveSyn(bool bKickout=false, bool bDeleteRecord=true, bool bSynchro=true);
	bool	DelMemberList(OBJID idOldSyn, OBJID idUser);
	bool	DonateMoney(int nMoney);
	bool	SetRank(int nRank, bool bLeader = false);
	bool	SetIdRank(OBJID idUser,OBJID idSyn, int nRank,  bool bForSave = false);
	bool	AddProffer(int nData);
	bool	DecProffer(int nData);
	void	UpLevel(int nLevel);			// synchro world kernel member list
	void	SetProffer(int nProffer);
	//20070331修罗:军团任务相关----------------------
	BOOL	SetTaskBeginTime(DWORD curTime);//设置军团任务开始时间
//	DWORD	GetTaskBeginTime(){return GetInt(SYNATTRDATA_TASKBEGINTIME);}//获得军团任务开始时间
	BOOL	ReportProfferToWorld();//向世界线程汇报贡献值
	//20070418修罗:军团任务掩码
	BOOL	SetTaskMaskWeek(unsigned short bit32);//设置军团任务掩码.周
	BOOL	GetTaskMaskWeek(unsigned short bit32);//获取军团任务掩码.周

	BOOL	SetTaskMaskDay(unsigned short bit32);//设置军团任务掩码.天
	BOOL	GetTaskMaskDay(unsigned short bit32);//获取军团任务掩码.天
	//----------------------------------------------
	

public:
	void	SynchroInfo(BOOL bAnnounce=false);				// 修改后，广播自己的属性
protected:
	void	AddProfferToLeader(int nProffer);

public: // static
	BOOL CheckSynName(char* szName);
	bool Demise(OBJID idOldHand, char *szOldName, OBJID idNewHand, char *szNewName);//[游途道标 2008/9/28]帮主转让
	OBJID GetUserIDByName(char *szPlayName);
	int GetRankByID(OBJID idUser,OBJID idSyn);
	void  GetUserNameByID(char* name, OBJID idUser);
	OBJID GetSynIDByUserID(OBJID idUser);
	bool AddSynAttData(OBJID idUser,int nRank,int nProffer);
	void UserLogout(OBJID idUser, OBJID idSyn);
	void UserLogin(OBJID idUser, OBJID idObj);
	void GetMateNameByID(char *szPlayName , OBJID id);
	OBJID GetAccountByID( OBJID id);
	int		GetSynRank();
	OBJID	CreateSubSubSyn(OBJID idFealty,const char  szName[]);
	bool	CreateSubSubSyn(LPCTSTR szName, int nLevel,int nProffer, int nMoney, int nMoneyLeave);
	int		GetSynMemberLevel();
	bool	SetSynMemberLevel(int nLevel);
	void	ResetSynProfferInc();
	void	Betray();
	bool	DetectIsBetrySyn();
	bool    CheckJoinWeek();
	bool    CheckJoinSyndays(int nDays);
	void	IncTimeOnline(unsigned short usTimeInc);
	void	GetSynInfoByID(MemberInfo &info);

	//---jinggy---2004-12-7---获得帮派成员的连续在线时间
	unsigned int GetTimeOnline();

	CSyndicate* QuerySyndicate();
	bool	ChangeSyn(CSyndicate* pSyn, CSyndicate* pTarget);
	bool	KickoutMember(OBJID idUser, bool IgnorErrorData = false);
protected: // data
	CSynAttrData*	m_pData;			//? 可能为0
	int m_nProfferInc;
protected: // ctrl
	PROCESS_ID		m_idProcess;
	CUser*			m_pUser;
	CAutoLink<CSyndicate>	m_linkSyn;
//////////////////////////////////////////////////////////////////////////--jay
public:
	void SetMask(int nMask);
	int  GetMask();
	void SetGift1(OBJID nGift);
	void SetGift2(OBJID nGift);
	OBJID GetGift1();
	OBJID GetGift2();
	int GetHistoryCon();
	void SetHistoryCon(int nCon);
	int GetHistoryMoney();
	void SetHistoryMoney(int nMoney);
	int GetHistoryGrow();
	void SetHistoryGrow(int nGrow);

protected: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_SYNATTR_H__7205A42E_F451_4A87_9BBD_37397D6578E2__INCLUDED_)
