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
	OBJID		idAccount;///�ʺ�id
	OBJID		idServer;///������id
	char		szAccountName[_MAX_ACCOUNT_NAMESIZE];///�ʺ���
	char		szServerName[_MAX_NAMESIZE];///���ڷ���������,һ��,����
	UINT		nTimeout;///��ʱ,һ����ʱ�Զ�ɾ��,С��GetTickCount,��������ʱ���ܼ�ʱ����
	DWORD		nAuthenID;///����Ʊ
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

///�����ʺű�������,�̹߳�������һ��Ҫ����
class CAccountManager  
{
public:
	CAccountManager();
	virtual ~CAccountManager();

// 	Role* GetRole(LPCSTR strRoleName);///ͨ����ɫ����ȡ��ɫ��Ϣ
// 	Role* GetRole(OBJID idRole);///ͨ����ɫ����ȡ��ɫ��Ϣ
	void OnlineAccount(OBJID idAccount,DWORD nAuthenID,LPCTSTR strAccountName,LPCTSTR szServerName);///�ʺ�����
	void OfflineAccount(OBJID idAccount);///�ʺ�����;
	bool IsOnlineAccount(OBJID idAccount);///�жϴ��ʺ��Ƿ�����
	void RemoveAccount(OBJID idAccount);///ɾ�����ʺ�
	void RemoveAllAccount(LPCTSTR szServerName);///ɾ������Ϸ������ʺ�,������Ϸ�������Ͽ���������
	void SetAccountTimeout(OBJID idAccount);///���ô��ʺŵĳ�ʱʱ��
	bool IsAccountTimeout(OBJID idAccount);///�жϴ��ʺ��Ƿ��Ѿ���ʱ
	bool GetAccount(OBJID idAccount,AccountInfo& info);
	void UseTick(OBJID idAccount, const char* pszServerName);
	void Dump();///�������������ʺ��б�,������
	void OnBreathe();
	void PureAllAccount();///����������Ч��ʱ���ʺ�
	UINT GetServerPlayerCount(LPCTSTR szServerName);
// 	void PureServerRoleList(OBJID idServer);///�����������������������ӵ�е����
// 	void PrintAllUser();
// 	UINT GetRoleCountAtServer(OBJID idServer);///��ѯָ�����е������
// 	void QueryRole(int nServerIndex,MsgRelayAction msgAction,OBJID idDstRole,OBJID idSrcRole);///��ѯ����������
	void WriteServerOnlinePlayerCount();///ÿ��ʮ����дһ�����з������������
	void PureServerOnlineMonth();///������ϸ��µ���ʷ����
protected:
	void ChangeServerAccountCount(LPCTSTR szServerName,bool nIncr);///�ı�������������� nIncrΪtrueʱ������1,falseΪ��1

private:
	AccountContainer m_accountContainer;
	ServerAcountCount m_ServerAcountCount;///���������ߵ�����,Ϊ�˼ӿ��ѯ����
	CCriticalSection   m_cs;
};

#endif // !defined(AFX_ACCOUNTMANAGER_H__2856258E_3B04_4BFE_93BD_60ECF1DB4D7C__INCLUDED_)
