// AccountManager.cpp: implementation of the CAccountManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "accountserver.h"
#include "AccountManager.h"
#include "logfile.h"
#include "GameBaseCode/mydb.h"
#include "typedef.h"
#include "AllHeads.h"
#include "PointThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccountManager::CAccountManager()
{

}

CAccountManager::~CAccountManager()
{

}

bool CAccountManager::IsAccountTimeout(OBJID idAccount)///�жϴ��ʺ��Ƿ��Ѿ���ʱ
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	bool hResult=false;
//	AccountInfo info={0};
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			hResult=(info.nTimeout>0 && info.nTimeout<GetTickCount());
		}
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::IsAccountTimeout crash!"); 
	}
	return hResult;
}

void CAccountManager::SetAccountTimeout(OBJID idAccount)///���ô��ʺŵĳ�ʱʱ��
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	const nDefaultTimeWait=20000;///����ʱ�ȷŸ�60���
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			if(info.nTimeout==0)///ֻ���ڵ�һ��ʱ�����ó�ʱʱ��
				info.nTimeout=GetTickCount()+nDefaultTimeWait;
		}
		else
			LOGCATCH("���Զ�һ����Ч���ʺ����ó�ʱʱ�� account:%d!",idAccount); 
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::SetAccountTimeout crash!"); 
	}
}

void CAccountManager::ChangeServerAccountCount(LPCTSTR szServerName,bool nIncr)///�ı�������������� nIncrΪtrueʱ������1,falseΪ��1
{
	UINT nServerAccountCount=0;
	try
	{
		if(nIncr)
			m_ServerAcountCount[szServerName]++;
		else
			m_ServerAcountCount[szServerName]--;
		
		if(m_ServerAcountCount[szServerName]<0)
		{
			LOGCATCH("CAccountManager::ChangeServerAccountCount warning!m_ServerAcountCount������������,ǿ�ƹ���,�����յ����ظ�������Ϣ szServerName:%s",szServerName);
			m_ServerAcountCount[szServerName]=0;
		}
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::ChangeServerAccountCount crash!"); 
	}

}

void CAccountManager::RemoveAccount(OBJID idAccount)///ɾ�����ʺ�
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			ChangeServerAccountCount(info.szServerName,false);///ɾ���ʺ�ʱ
			m_accountContainer.erase(idAccount);
		}
		else
			LOGCATCH("����!�յ���һ���ظ���kickass��Ϣ,�ʺ�%d�Ѿ���������",idAccount); 
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::IsAccountTimeout crash!"); 
	}
}

void CAccountManager::RemoveAllAccount(LPCTSTR szServerName)///ɾ������Ϸ������ʺ�,������Ϸ�������Ͽ���������
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	try
	{
		LOGCATCH("����!���ڷ�����(%s)�����������ʺŷ�����,������˷����в����ʺ�",szServerName); 
		AccountIter it=m_accountContainer.begin();
		while (it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			///ֻ����ȫ��ȵķ��������ֲŴ���
			if(strncmp(info.szServerName,szServerName,_MAX_NAMESIZE))
			{
				++it;
				continue;
			}
			it=m_accountContainer.erase(it);
		}
		m_ServerAcountCount[szServerName]=0;///�˷���������ֱ�����
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::RemoveAllAccount crash! szServerName:%s",szServerName); 
	}
}

bool CAccountManager::IsOnlineAccount(OBJID idAccount)///�жϴ��ʺ��Ƿ�����
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	bool hResult=false;
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		hResult=(it!=m_accountContainer.end());
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::IsOnlineAccount crash!"); 
	}
	return hResult;
}

void CAccountManager::UseTick(OBJID idAccount, const char* pszServerName)
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			///ֻ����ȫ��ȵķ��������ֲŴ���
			if(strncmp(info.szServerName,pszServerName,_MAX_NAMESIZE)!=0)
				LOGCATCH("����!�յ���һ���쳣��UseTick��Ϣ,�ʺ�%d��%s��,����Ʊ����%s",idAccount,info.szServerName,pszServerName); 
			else
				info.nAuthenID=0;///�ɹ��õ����Ʊ
		}
		else
			LOGCATCH("����!�յ���һ��UseTick��Ϣ,�ʺ�%d�Ѿ���������",idAccount); 
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::UseTick crash!idAccount:%d,pszServerName:%s",idAccount,pszServerName); 
	}
}

bool CAccountManager::GetAccount(OBJID idAccount,AccountInfo& info)
{
	bool hResult=false;
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		hResult=(it!=m_accountContainer.end());
		if(hResult)
			info=m_accountContainer[idAccount];
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::GetAccount crash!"); 
	}
	return hResult;
}

void CAccountManager::PureServerOnlineMonth()///������ϸ��µ���ʷ����
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	const char* sqlTemplate="DELETE FROM acc_linelog WHERE TIME<UNIX_TIMESTAMP(DATE_FORMAT(NOW(),'%Y-%m-01'))";///����ɾ��ǰ�µ���������
	try
	{
		if(g_db.ExecuteSQL((LPSTR)sqlTemplate)==false)
		{
			///���ݿ�д��ʧ��,Ҫô��������,Ҫô�����ݿ�ʧȥ������
			PrintText("CAccountManager::PureServerOnlineMonth ʧ��!���acc_linelog�Ƿ����!���������ݿ�ʧȥ����!");
		}
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::PureServerOnlineMonth crash!"); 
	}
}

void CAccountManager::WriteServerOnlinePlayerCount()///ÿ��ʮ����дһ�����з������������
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	///SELECT UNIX_TIMESTAMP(DATE_FORMAT(NOW(),'%Y-%m-01'));
	const UINT nWriteServerOnlinePlayerDelay=1000*60*10;///10����
	UINT nTickCount=GetTickCount();///�õ�����ϵͳ��������
	static UINT nWriteServerOnlinePlayerTime=nTickCount+nWriteServerOnlinePlayerDelay;
	CString strSQL;
	const char* sqlTemplate="insert acc_linelog SET TIME=UNIX_TIMESTAMP(),count_online=%d,line='%s'";
	try
	{
		if(nTickCount>nWriteServerOnlinePlayerTime)
		{
			ServerAcountCount::iterator it=m_ServerAcountCount.begin();
			while(it!=m_ServerAcountCount.end())
			{
				strSQL.Format(sqlTemplate,it->second,it->first.c_str());
				if(g_db.ExecuteSQL((LPSTR)(LPCSTR)strSQL)==false)
				{
					///���ݿ�д��ʧ��,Ҫô��������,Ҫô�����ݿ�ʧȥ������
					LOGCATCH("CAccountManager::WriteServerOnlinePlayerCount ʧ��!���acc_linelog�Ƿ����!���������ݿ�ʧȥ����!"); 
					break;
				}
				++it;
			}
			nWriteServerOnlinePlayerTime=nTickCount+nWriteServerOnlinePlayerDelay;
		}
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::WriteServerOnlinePlayerCount crash!"); 
	}
	
}

// void CAccountManager::PureServerOnlineMonth()///������ϸ��µ���ʷ����
// {
// 	CString strSQL;
// 	const char* sqlTemplate="delete from acc_linelog where time<(),count_online=%d,line='%s'";
// 	try
// 	{
// 
// 	}
// 	catch(...)
// 	{
// 		LOGCATCH("CAccountManager::WriteServerOnlinePlayerCount crash!"); 
// 	}
// }

UINT CAccountManager::GetServerPlayerCount(LPCTSTR szServerName)
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	UINT nServerPlayerCount=0;
	if(szServerName==NULL || strlen(szServerName)==0)
	{
		LOGCATCH("CAccountManager::GetServerPlayerCount szServerName�մ���Ч!"); 
		return 0;
	}
	try
	{
// 		AccountIter it=m_accountContainer.begin();
// 		while (it!=m_accountContainer.end())
// 		{
// 			AccountInfo& info=it->second;
// 			///ֻ����ȫ��ȵķ��������ֲŴ���
// 			if(strncmp(info.szServerName,szServerName,_MAX_NAMESIZE)==0)
// 			{
// 				nServerPlayerCount++;
// 			}
// 			++it;
// 		}
		nServerPlayerCount=m_ServerAcountCount[szServerName];
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::OfflineAccount crash!szServerName:%s",szServerName); 
	}
	return nServerPlayerCount;
}

void CAccountManager::OfflineAccount(OBJID idAccount)///�ʺ�����;
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
//	AccountInfo info={0};
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			ChangeServerAccountCount(info.szServerName,false);///ɾ���ʺ�ʱ
			LOGPOINT("�ʻ�������!idAccount:%d,strAccountName:%s,szServerName:%s",idAccount,info.szAccountName,info.szServerName); 
			m_accountContainer.erase(idAccount);
		}
		else
			LOGCATCH("����!�յ���һ���ظ���kickass��Ϣ,�ʺ�%d�Ѿ���������",idAccount); 
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::OfflineAccount crash!"); 
	}
}

// void CAccountManager::PureAllAccount()
// {
// 	AccountIter it=m_accountContainer.begin();
// 	while(it!=m_accountContainer.end())
// 	{
// 		AccountInfo& info=it->second;
// 		if(info.nTimeout==0)///�����ʺŲ�����
// 		{
// 			++it;
// 			continue;
// 		}
// 		if(info.nTimeout<GetTickCount())
// 			it=m_accountContainer.erase(it);
// 		else
// 			++it;
// 	}
// }

// void CAccountManager::OnBreathe()
// {
// 	///ɾ�����г�ʱ���ʺ�
// 	const UINT nPureAllAccountDelay=6000;//1��������һ��
// 	static UINT nPureAllAccountTime=GetTickCount();
// 	if(GetTickCount()>(nPureAllAccountTime+nPureAllAccountDelay))
// 	{
// 		nPureAllAccountTime=GetTickCount();
// 		PureAllAccount();///�������г�ʱ�ʺ�,��ʱ�ʺ����ظ���¼�������
// 	}
// 
// }

void CAccountManager::OnlineAccount(OBJID idAccount,DWORD nAuthenID,LPCTSTR strAccountName,LPCTSTR szServerName)///�ʺ�����
{
	///����,�Զ�����
	CriticalLocker locker(m_cs);
	AccountInfo info={0};
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it==m_accountContainer.end())
		{
			info.idAccount=idAccount;
			info.nTimeout=0;///�޳�ʱ�ʺ�Ĭ��Ϊ0
			info.nAuthenID=nAuthenID;
			strncpy(info.szAccountName,strAccountName,_MAX_ACCOUNT_NAMESIZE);
			strncpy(info.szServerName,szServerName,_MAX_NAMESIZE);
			m_accountContainer[idAccount]=info;
			ChangeServerAccountCount(info.szServerName,true);///���ӷ���������
			LOGPOINT("�ʻ�������!idAccount:%d,strAccountName:%s,szServerName:%s",idAccount,strAccountName,szServerName); 
		}
// 		else
// 			LOGCATCH("����!����ʱ��������ʺ��Ѿ�����,��bug��?",idAccount); 
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::OnlineAccount crash!"); 
	}
}