// SynShell.h: interface for the CSyndicateWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNSHELL_H__8147FFBF_85B9_43E4_9D3A_B847BF31F088__INCLUDED_2)
#define AFX_SYNSHELL_H__8147FFBF_85B9_43E4_9D3A_B847BF31F088__INCLUDED_2

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0510 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0500	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif

#include "../share/define.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "protocol.h"
#include "AutoPtr.h"
#include "MessageBoard.h"
#include <vector>
#include <string>
using namespace std;

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
		SYNFUNC_UP_LEVEL,
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
enum SYNDATA{
		SYNDATA_ID=0,			// 仅用于INSERT
		SYNDATA_NAME=1,			// 帮派名
		SYNDATA_ANNOUNCE,		// 帮派公告
		SYNDATA_LEADERID,		// 帮主ID
		SYNDATA_LEADERNAME,		// 帮主名字
//		SYNDATA_SUBLEADERID,	// 副帮主ID
		SYNDATA_SUBLEADERNAME,	// 副帮主名字
		SYNDATA_CREATETIME,		//创建时间
		SYNDATA_STATE,			//状态1是响应，2是正常，3是解散
		SYNDATA_LEVEL,			//主堂等级
		SYNDATA_SUBLEV1,		
		SYNDATA_SUBLEV2,
		SYNDATA_SUBLEV3,
		SYNDATA_SUBLEV4,
		SYNDATA_GROW,			//主堂建设
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
};
char	szID[];
char	szSynTable[];
typedef	CGameData<SYNDATA, szSynTable, szID>	CSynWorldBase;

//////////////////////////////////////////////////////////////////////
class ISynModify
{
public:
	virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetTenet(LPCTSTR szTenet, bool bSave)													PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)										PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)										PURE_VIRTUAL_FUNCTION_0
	virtual bool	AddData(SYNDATA idx, int nData, bool bSave)												PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetData(SYNDATA idx, int nData, bool bSave)												PURE_VIRTUAL_FUNCTION_0
	virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave)										PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetSynName(LPCTSTR szSynName, bool bSave=true)											PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
// 所有成员函数为原子操作。
struct CPlayer;
class CSyndicateWorld : ISynModify
{
protected:
	CSyndicateWorld();
	virtual ~CSyndicateWorld();
public:
	static CSyndicateWorld* CreateNew()	{ return new CSyndicateWorld; }
	virtual ULONG	ReleaseByOwner()				{ delete this; return 0; }
	bool	Create(IRecordset* pRes);
	bool	Create(IRecordset* pRes, const CreateSyndicateInfo* pInfo, bool bSave);
	ISynModify*		QueryModify()					{ return (ISynModify*)this; }
	CMessageBoard*	QueryMessageBoard()				{ ASSERT(m_ptrMsgBd); return m_ptrMsgBd; }
	bool			ChangeSyndicate(/*const*/ SynFuncInfo0* pFuncInfo);
	BOOL			PayOff(DWORD dwMoney,BOOL bSave);//20070414修罗:发工资.在内修改数据库.且处理成员分发
	BOOL			SetMemberProffer(int dwProffer,OBJID idMember,BOOL bSave);//20070417修罗:成员贡献值
	void			WriteOfflineTime(OBJID userID);
public:
	OBJID	GetID()									{ return m_pData->GetKey(); }
	int		GetInt(SYNDATA idx)						{ return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(SYNDATA idx)						{ return m_pData->GetStr(idx); }

protected: // syn member list --------------------------
	typedef struct	MemberInfo{
		//[游途道标 2008/9/24]对帮会信息由原来四个扩充到七个
		//[游途道标 2008/9/25]增加对在线人员的等级
		MemberInfo(int id, LPCTSTR str, int nUserRank, int level, int proffer, int sex, int Profession, bool Online,int OfflineTime = 0) 
		{
			nId			=	id;
			szName		=	str;
			nRank		=	nUserRank;
			nLevel		=	level;
			nProffer	=	proffer;
			nSex		=	sex;
			nProfession	=	Profession;
			bOnline		=	Online;
			nOfflineTime = OfflineTime;
		}
		int		nId;		//玩家标识
		string	szName;		//玩家名
		int		nRank;		//玩家职位
		int		nLevel;		//玩家等级
		int     nProffer;	//玩家威望
		int		nSex;		//玩家性别
		int		nProfession;//玩家职业
		bool	bOnline;	//是否在线
		int     nOfflineTime;//离线时间 *玩家上线登录后数值大于1000000 超过部分记录在线真实时间
	}MEMBERINFO;
	typedef	vector<MemberInfo>	MEMBER_SET;
public:
	bool DelMember(int id);
	int GetMemberProffer(OBJID id);
	int		GetSynRankShow(int nRank);
    const char* GetMemberName(OBJID id);
//	void	SendMemberListSpecifySyn(CPlayer* pUser, int idxLast);
	CSyndicateWorld* GetMasterSyn();
	//[游途道标 2008/9/24]在AddMember函数后新加两个参数
	//[游途道标 2008/9/25]在AddMember函数后新加bOnline记录在线与否
	bool	AddMember(int id,LPCTSTR szName, int nUserRank,int level,int proffer, int nSex, int nProfession, bool bOnline);
	bool	DelMember(LPCTSTR szName);
	bool	SetMemberRank(const char* pszName, int nRank);
	int		GetMemberAmount()				{ return m_setMember.size(); }
	void	SendMemberList(CPlayer* pUser,DWORD dwTime );
	void	CombineMemberList(const MEMBER_SET& setMember);
	MEMBER_SET&	QueryMemberList()						{ return m_setMember; }

// 	OBJID   GetSynAttackID();
// 	OBJID   GetSynDefenceID();
// 	void    SetSynAttackID(OBJID idAttack, bool bUpdate=false);
// 	void    SetSynDefenceID(OBJID idDefence, bool bUpdate=false);
	bool    Update();

protected:
//------------06.10.26王玉波注释并修改-------------begin----------------------------------------
//	int		AppendMember(class CMsgName& msg, int idx, int idxFirst, int& nCount);
	void		AppendMember(class CMsgName& msg,int idxFirst,int idxTime );

//-------------------------------------------------end---------------------------------------
//	int		AppendOnlineMember(class CMsgName& msg, int idxFirst);
//	int		AppendOnlineMemberForAllMember(class CMsgName& msg, int idxFirst);
//	int		AppendOfflineMember(class CMsgName& msg, int idxFirst, int nCurrCount=0);
//	int		AppendOfflineMemberForAllMember(class CMsgName& msg, int idx, int nCount=0);
protected:
	MEMBER_SET	m_setMember;
//	MEMBER_SET	m_setMemberTmp;

protected: // modify, mast be atom operator. test & set
	// bSave 必须为true，否则不会存盘
	//[游途道标 2008/9/24]在Demise函数后面新加nOldSex和nOldProfession两个参数
	virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave, int nOldSex, int nOldProfession);
	virtual bool	SetTenet(LPCTSTR szTenet, bool bSave);
	virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave);
	virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave);
	virtual bool	AddData(SYNDATA idx, int nData, bool bSave);
	virtual bool	SetData(SYNDATA idx, int nData, bool bSave);
	virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave);
	virtual bool	SetSynName(LPCTSTR szSynName, bool bSave);
//06.10.25王玉波添加---------begin--------------------
	bool	SetMemberLevel( const char* pTargetName,int nLevel );//存盘也是更改synattr表,所以这里忽略
	DWORD	m_dwTimeLastUpdateMemberInfo;//服务端最后一次更新成员信息的时间,用它来判断是否要重发成员信息
	DWORD	m_dwTimeFirstUpdateMemberOffline;
//---------------------------end----------------------
private:
	CSynWorldBase*	m_pData;

protected: // msgbd
	typedef	CAutoPtr<CMessageBoard>	MSGBD_PTR;
	MSGBD_PTR		m_ptrMsgBd;

protected: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_SYNSHELL_H__8147FFBF_85B9_43E4_9D3A_B847BF31F088__INCLUDED_2)
