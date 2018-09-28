// AccountManager.h: interface for the CAccountManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCOUNTMANAGER_H__2856258E_3B04_4BFE_93BD_60ECF1DB4D7C__INCLUDED_)
#define AFX_ACCOUNTMANAGER_H__2856258E_3B04_4BFE_93BD_60ECF1DB4D7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)

#include "AFXMT.H"
#include "string"
#include "Define.h"
#include "map"
using namespace std;

struct AccountInfo
{
	OBJID		idAccount;///帐号id
	OBJID		idServer;///服务器id
	char		szAccountName[_MAX_ACCOUNT_NAMESIZE];///帐号名
	char		szServerName[_MAX_NAMESIZE];///所在服务器名字,一线,二线
	UINT		nTimeout;///超时,一旦超时自动删除,小于GetTickCount,用于踢人时不能及时反馈
	DWORD		nAuthenID;///保存票
};

struct CriticalLocker
{
	CriticalLocker(CCriticalSection& locker):m_locker(locker)
	{
		m_locker.Lock();
	}
	~CriticalLocker()
	{
		m_locker.Unlock();
	}
	CCriticalSection& m_locker;
};

typedef map<OBJID,AccountInfo> AccountContainer;
typedef AccountContainer::iterator AccountIter;
typedef map<string,int> ServerAcountCount;

///在线帐号表的替代者,线程共享数据一定要加锁
class CAccountManager  
{
public:
	CAccountManager();
	virtual ~CAccountManager();

// 	Role* GetRole(LPCSTR strRoleName);///通过角色名查取角色信息
// 	Role* GetRole(OBJID idRole);///通过角色名查取角色信息
	void OnlineAccount(OBJID idAccount,DWORD nAuthenID,LPCTSTR strAccountName,LPCTSTR szServerName);///帐号上线
	void OfflineAccount(OBJID idAccount);///帐号下线;
	bool IsOnlineAccount(OBJID idAccount);///判断此帐号是否在线
	void RemoveAccount(OBJID idAccount);///删除此帐号
	void RemoveAllAccount(LPCTSTR szServerName);///删除此游戏服相关帐号,用于游戏服务器断开后重连上
	void SetAccountTimeout(OBJID idAccount);///设置此帐号的超时时间
	bool IsAccountTimeout(OBJID idAccount);///判断此帐号是否已经超时
	bool GetAccount(OBJID idAccount,AccountInfo& info);
	void UseTick(OBJID idAccount, const char* pszServerName);
	void Dump();///导出所有在线帐号列表,调试用
	void OnBreathe();
	void PureAllAccount();///清理所有无效超时的帐号
	UINT GetServerPlayerCount(LPCTSTR szServerName);
// 	void PureServerRoleList(OBJID idServer);///当服务器掉线重连后清空它拥有的玩家
// 	void PrintAllUser();
// 	UINT GetRoleCountAtServer(OBJID idServer);///查询指定服中的玩家数
// 	void QueryRole(int nServerIndex,MsgRelayAction msgAction,OBJID idDstRole,OBJID idSrcRole);///查询玩家在线情况
	void WriteServerOnlinePlayerCount();///每隔十分钟写一次所有服在线玩家人数
	void PureServerOnlineMonth();///清理掉上个月的历史数据
protected:
	void ChangeServerAccountCount(LPCTSTR szServerName,bool nIncr);///改变服务器在线人数 nIncr为true时人数加1,false为减1

private:
	AccountContainer m_accountContainer;
	ServerAcountCount m_ServerAcountCount;///保存所有线的人数,为了加快查询而生
	CCriticalSection   m_cs;
};

#endif // !defined(AFX_ACCOUNTMANAGER_H__2856258E_3B04_4BFE_93BD_60ECF1DB4D7C__INCLUDED_)
