// UserManager.h: interface for the CUserManager class.
//	20071019:修罗改
//	CUserManager拥有自己的set
//	不再公用CRoleManager的set,避免因为刷怪数量巨大影响CUserManager的遍历
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERMANAGER_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_)
#define AFX_USERMANAGER_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma	warning(disable:4786)
#include "../share/define.h"
#include "User.h"
#include "GameBlock.h"
#include "team.h"
#include "../share/NetworkDef.h"
#include "roleManager.h"
#include "index.h"
#include <vector>
using namespace std;

class IForMapGroup
{
public:
	virtual void	BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender=NULL)				PURE_VIRTUAL_FUNCTION
public: // user change mapgroup
	virtual void	JoinMapGroup(SOCKET_ID idSocket, UserInfoStruct* pInfo, ULONG nIP)				PURE_VIRTUAL_FUNCTION
};

typedef	IGameObjSet<CTeam>		ITeamSet;
typedef	CGameObjSet<CTeam>		CTeamSet;
typedef	map<string,OBJID> USERNAME_SET;

typedef	map<OBJID,stTempUser*> TEMPUSER_SET; //临时玩家

class CUserManager;


typedef	CSingleMap<CUser>		CUserSet;

class ISocket;
class IDatabase;
class CUserManager;
typedef CUserManager IUserManager;
class CUserManager : IForMapGroup
{
public://---------------------------------------------
//	20071019:修罗改
//	CUserManager拥有自己的set
//	不再公用CRoleManager的set,避免因为刷怪数量巨大影响CUserManager的遍历
//	class Iterator
//	{
//	private: // create by master
//		Iterator(IRoleSet* pSet) : m_pSet(pSet),m_pIter(pSet->NewEnum()),m_pUser(NULL) {}
//		friend class CUserManager;
//	public:
//		bool	Next()							{ while(m_pIter.Next()){ if(m_pIter->QueryObj(OBJ_USER, IPP_OF(m_pUser))) return true; } m_pUser=NULL; return false; }
//		operator CUser*()						{ return m_pUser; }
//		CUser* operator->()						{ ASSERT(m_pUser); return m_pUser; }
//	protected:
//		IRoleSet*			m_pSet;
//		CRoleSet::Iterator	m_pIter;
//		CUser*				m_pUser;
//	};
//	Iterator	NewEnum();
	class Iterator
	{
	private: // create by master
		Iterator(CUserSet* pSet) : m_pSet(pSet),m_pIter(pSet->NewEnum()),m_pUser(NULL) {}
		friend class CUserManager;
	public:
		bool Next()			{if(m_pIter.Next()) {m_pUser=(m_pIter);return true;} else return false;}
		operator CUser*()	{return m_pUser; }
		CUser* operator->()	{ASSERT(m_pUser); return m_pUser;}
	protected:
		CUserSet*			m_pSet;
		CUserSet::Iterator	m_pIter;
		CUser*				m_pUser;
	};
	Iterator	NewEnum()						{return Iterator(m_userSetEx);}
	protected:
		//此集合仅为RoleSet的一个copy.
		//仅做查询使用.提高查询效率.不提供delete等管理功能
		CUserSet* m_userSetEx;
	
public:
	//CUserManager(PROCESS_ID idProcess) : m_idProcess(idProcess), m_dwMaxUser(0) {}
	//virtual ~CUserManager() {}
	CUserManager(PROCESS_ID idProcess) : m_idProcess(idProcess), m_dwMaxUser(0) 
	{
		m_userSetEx = CUserSet::CreateNew(false);//false.不需要在delObj时releaseByOwner.否则与RoleSet重复delete
	}
	virtual ~CUserManager() 
	{
		if(m_userSetEx)
			m_userSetEx->Release();
	}
	CUserSet* QuerySet(){ASSERT(m_userSetEx); return m_userSetEx;}
	//20071019:修罗改end---------------------------------------------
	IUserManager*	GetInterface() { return (IUserManager*)this; }
	virtual IForMapGroup*	GetForMapGroup() { return (IForMapGroup*)this; }

public:
	virtual bool	Create();
	virtual void	Release();
	virtual void	OnTimer(time_t tCurr);
public: // login/logout
	virtual CUser*	CreateUser(SOCKET_ID idSocket, OBJID idUser, ULONG nIP);			// login第一步
	virtual bool	LoginUser(SOCKET_ID idSocket, CUser* pUser);			// login第二步
	virtual bool	LogoutUser(SOCKET_ID idSocket, BOOL bOffline=true);  
	virtual bool	KickOutSocket(SOCKET_ID idSocket, LPCTSTR szReason = NULL);
	virtual void	LogoutAllUser();
	virtual bool	IsLoginMapGroup(SOCKET_ID idSocket)			{ return (idSocket>=0 && idSocket<m_setUser.size() && m_setUser[idSocket]); }		// && m_setUser[m_setUser]->idProcess >= MSGPORT_MAPGROUP_FIRST
	virtual void	KickOutAllSocket();
public: // get user
	virtual CUser*	GetUser(OBJID idUser);
	virtual CUser*	GetUser(LPCTSTR szName);
	virtual CUser*	GetUser(CNetMsg* pMsg);
	virtual CUser*	GetUserBySocketID(SOCKET_ID idSocket)	{ if(idSocket>=0 && idSocket<m_setUser.size()) return m_setUser[idSocket]; return NULL; }
//can't use this func	virtual DWORD	GetSocketAmount()						{ return m_setUser.size(); }
	virtual DWORD	GetMapUserAmount(OBJID idMap);
	virtual DWORD	GetUserAmount();
	virtual DWORD	GetMaxUser()	{return m_dwMaxUser;}
	DWORD	m_dwMaxUser;
	virtual bool	FindNextSynMember(OBJID idSyn, int& nIter);
	virtual CUser*	GetGM()	{ if (m_setGm.size() <= 0) return NULL; else return this->GetUser(m_setGm.front()); }

public:
	virtual void	BroadcastMsg(CNetMsg* pMsg, CUser* pExcludeSender=NULL);			// 全世界广播
	virtual void	BroadcastMsg(LPCTSTR szMsg, CUser* pExcludeSender=NULL, LPCTSTR pszEmotion=NULL, DWORD dwColor=0xffffff, int nAttribute=_TXTATR_NORMAL,CUser* pUserName = NULL,DWORD dwItemID = 0,DWORD dwItemTypeID = 0, LPCTSTR pszItemName = NULL);			// 全世界广播
	virtual void	BroadcastMapMsg(OBJID idMap, CNetMsg* pMsg, CUser* pExcludeSender=NULL);			// 本地图广播
    //---------add by cz 广播消息并设置接受者名字主要是为了狮子吼与世界频道的判定
	void   BroadCastMsgAndReceiveName(LPCTSTR szMsg, CUser* pExcludeSender=NULL, LPCTSTR pszEmotion=NULL, DWORD dwColor=0xffffff, int nAttribute=_TXTATR_NORMAL,CUser* pUserName = NULL,DWORD dwItemID = 0,DWORD dwItemTypeID = 0, LPCTSTR pszItemName = NULL,LPCTSTR pReceiveName=NULL);	

	void   BroadcastMsgForWarGame(int nLevBegin, int nLevEnd, int nWarGameID);


public: // mapgroup
	virtual void	BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender=NULL);			// 本地图组广播
	void    BroadcastProMsg(int job,CNetMsg* pMsg, CUser* pExcludeSender=NULL);///广播门派消息,job职业
	virtual void	JoinMapGroup(SOCKET_ID idSocket, UserInfoStruct* pInfo, ULONG nIP);
	virtual void BroadcastMapGroupMsg(LPCTSTR szMsg, CUser* pExcludeSender = NULL, LPCTSTR pszEmotion=NULL, DWORD dwColor=0xffffff, int nAttribute=_TXTATR_NORMAL,CUser* pUserName = NULL);	//  [2007-9-3] 朱斌 广播地图组消息
public:	// --------- 队伍管理 ---------------
	CTeam*			GetTeamByLeaderId		(OBJID idLeader);
	CTeam*			GetTeamById				(OBJID id);
	CTeam*			GetTeamByIndex			(int nIndex);
	int				GetTeamAmount			();
	BOOL			IsValidTeam				(CTeam* pTeam);
	BOOL			AddTeam					(CTeam* pTeam);
	void			DelTeam					(CTeam* pTeam);
	OBJID			SpawnNewTeamID			();

public: // cheat ---------------------------------------------
	bool	AddCheat(OBJID idUser, int* pnAmount, int* pnTimes);
	int		GetCheatCount(OBJID idUser)				{ return m_setCheatIDTimes.IsIn(idUser) ? m_setCheatIDTimes.At(idUser) : 0; }
	int		GetCheatAmount()						{ return m_setCheatIDTimes.Size(); }

	
	void    AddTeamLeader(OBJID idLeader) ;
    OBJID   GetTeamLeaderByIndex(int i) ;
	void    ChangeTeamLeader(OBJID idLeader);
	void    DelTeamLeader(OBJID idLeader);
	int     GetTeamLeaderAmount();
	void    TeamSearchRole(CUser* pUserLeader);
	void    RoleSearchTeam(CUser* pMember);
	stTempUser* QueryTempUser(OBJID idUser,bool bFlush = false);
	stTempUser* QueryTempUser(char* szName,bool bFlush = false);
	bool	CreateTempUser(OBJID idUser,stTempUser* pUser);
	bool	CreateTempUser(char* szName,stTempUser* pUser);
	bool	CreateTempUser(CUser* pUser,stTempUser* pTempUser);
	void	UpdateTempUser(CUser* pUser);
	void    ClearTempUser();
protected:
	Index<OBJID,int>	m_setCheatIDTimes;

protected:
	typedef	vector<CUser*> USER_SET;
	USER_SET	m_setUser;					// 以SOCKET_ID为索引。reference point set

	USERNAME_SET	m_setUserName;					// 以UserName为索引。reference point set
	TEMPUSER_SET    m_setTempUser;

	ITeamSet*	m_pTeamSet;
	OBJID		m_idLastTeam;

	vector<OBJID>	m_setGm;

	vector<OBJID>   m_setTeamLeader;

	PROCESS_ID	m_idProcess;
	CTimeOut	m_StockTimeOut;
public:
	USERNAME_SET& GetUserNameSet()
	{
		return m_setUserName;
	}
	bool ReSetMapName(const char*szName,int nId);	//改名
	void BroadcastPhyleMapGroupMsg( CNetMsg* pMsg,int nPhyle );				//广播某一种族信息..
	void GetPhyleMemberCount(int mapid,int &dragon,int &human,int  &gene);
	MYHEAP_DECLARATION(s_heap)
};



#endif // !defined(AFX_USERMANAGER_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_)
