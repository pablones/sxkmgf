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

bool CAccountManager::IsAccountTimeout(OBJID idAccount)///判断此帐号是否已经超时
{
	///加锁,自动解锁
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

void CAccountManager::SetAccountTimeout(OBJID idAccount)///设置此帐号的超时时间
{
	///加锁,自动解锁
	CriticalLocker locker(m_cs);
	const nDefaultTimeWait=20000;///测试时先放个60秒吧
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			if(info.nTimeout==0)///只有在第一次时才设置超时时间
				info.nTimeout=GetTickCount()+nDefaultTimeWait;
		}
		else
			LOGCATCH("尝试对一个无效的帐号设置超时时间 account:%d!",idAccount); 
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::SetAccountTimeout crash!"); 
	}
}

void CAccountManager::ChangeServerAccountCount(LPCTSTR szServerName,bool nIncr)///改变服务器在线人数 nIncr为true时人数加1,false为减1
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
			LOGCATCH("CAccountManager::ChangeServerAccountCount warning!m_ServerAcountCount被减到负数了,强制归零,可能收到了重复离线消息 szServerName:%s",szServerName);
			m_ServerAcountCount[szServerName]=0;
		}
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::ChangeServerAccountCount crash!"); 
	}

}

void CAccountManager::RemoveAccount(OBJID idAccount)///删除此帐号
{
	///加锁,自动解锁
	CriticalLocker locker(m_cs);
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			ChangeServerAccountCount(info.szServerName,false);///删除帐号时
			m_accountContainer.erase(idAccount);
		}
		else
			LOGCATCH("该死!收到了一个重复的kickass消息,帐号%d已经不存在了",idAccount); 
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::IsAccountTimeout crash!"); 
	}
}

void CAccountManager::RemoveAllAccount(LPCTSTR szServerName)///删除此游戏服相关帐号,用于游戏服务器断开后重连上
{
	///加锁,自动解锁
	CriticalLocker locker(m_cs);
	try
	{
		LOGCATCH("该死!由于服务器(%s)重新连上了帐号服务器,先清除此服所有残余帐号",szServerName); 
		AccountIter it=m_accountContainer.begin();
		while (it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			///只有完全相等的服务器名字才处理
			if(strncmp(info.szServerName,szServerName,_MAX_NAMESIZE))
			{
				++it;
				continue;
			}
			it=m_accountContainer.erase(it);
		}
		m_ServerAcountCount[szServerName]=0;///此服务器人数直接清空
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::RemoveAllAccount crash! szServerName:%s",szServerName); 
	}
}

bool CAccountManager::IsOnlineAccount(OBJID idAccount)///判断此帐号是否在线
{
	///加锁,自动解锁
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
	///加锁,自动解锁
	CriticalLocker locker(m_cs);
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			///只有完全相等的服务器名字才处理
			if(strncmp(info.szServerName,pszServerName,_MAX_NAMESIZE)!=0)
				LOGCATCH("该死!收到了一个异常的UseTick消息,帐号%d在%s上,但用票是在%s",idAccount,info.szServerName,pszServerName); 
			else
				info.nAuthenID=0;///成功用掉这个票
		}
		else
			LOGCATCH("该死!收到了一个UseTick消息,帐号%d已经不存在了",idAccount); 
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::UseTick crash!idAccount:%d,pszServerName:%s",idAccount,pszServerName); 
	}
}

bool CAccountManager::GetAccount(OBJID idAccount,AccountInfo& info)
{
	bool hResult=false;
	///加锁,自动解锁
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

void CAccountManager::PureServerOnlineMonth()///清理掉上个月的历史数据
{
	///加锁,自动解锁
	CriticalLocker locker(m_cs);
	const char* sqlTemplate="DELETE FROM acc_linelog WHERE TIME<UNIX_TIMESTAMP(DATE_FORMAT(NOW(),'%Y-%m-01'))";///总是删除前月的所有数据
	try
	{
		if(g_db.ExecuteSQL((LPSTR)sqlTemplate)==false)
		{
			///数据库写入失败,要么表不存在了,要么与数据库失去连接了
			PrintText("CAccountManager::PureServerOnlineMonth 失败!检测acc_linelog是否存在!或者与数据库失去连接!");
		}
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::PureServerOnlineMonth crash!"); 
	}
}

void CAccountManager::WriteServerOnlinePlayerCount()///每隔十分钟写一次所有服在线玩家人数
{
	///加锁,自动解锁
	CriticalLocker locker(m_cs);
	///SELECT UNIX_TIMESTAMP(DATE_FORMAT(NOW(),'%Y-%m-01'));
	const UINT nWriteServerOnlinePlayerDelay=1000*60*10;///10分钟
	UINT nTickCount=GetTickCount();///得到操作系统启动秒数
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
					///数据库写入失败,要么表不存在了,要么与数据库失去连接了
					LOGCATCH("CAccountManager::WriteServerOnlinePlayerCount 失败!检测acc_linelog是否存在!或者与数据库失去连接!"); 
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

// void CAccountManager::PureServerOnlineMonth()///清理掉上个月的历史数据
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
	///加锁,自动解锁
	CriticalLocker locker(m_cs);
	UINT nServerPlayerCount=0;
	if(szServerName==NULL || strlen(szServerName)==0)
	{
		LOGCATCH("CAccountManager::GetServerPlayerCount szServerName空串无效!"); 
		return 0;
	}
	try
	{
// 		AccountIter it=m_accountContainer.begin();
// 		while (it!=m_accountContainer.end())
// 		{
// 			AccountInfo& info=it->second;
// 			///只有完全相等的服务器名字才处理
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

void CAccountManager::OfflineAccount(OBJID idAccount)///帐号下线;
{
	///加锁,自动解锁
	CriticalLocker locker(m_cs);
//	AccountInfo info={0};
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it!=m_accountContainer.end())
		{
			AccountInfo& info=it->second;
			ChangeServerAccountCount(info.szServerName,false);///删除帐号时
			LOGPOINT("帐户下线了!idAccount:%d,strAccountName:%s,szServerName:%s",idAccount,info.szAccountName,info.szServerName); 
			m_accountContainer.erase(idAccount);
		}
		else
			LOGCATCH("该死!收到了一个重复的kickass消息,帐号%d已经不存在了",idAccount); 
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
// 		if(info.nTimeout==0)///正常帐号不处理
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
// 	///删除所有超时的帐号
// 	const UINT nPureAllAccountDelay=6000;//1分钟清理一次
// 	static UINT nPureAllAccountTime=GetTickCount();
// 	if(GetTickCount()>(nPureAllAccountTime+nPureAllAccountDelay))
// 	{
// 		nPureAllAccountTime=GetTickCount();
// 		PureAllAccount();///清理所有超时帐号,超时帐号由重复登录踢人造成
// 	}
// 
// }

void CAccountManager::OnlineAccount(OBJID idAccount,DWORD nAuthenID,LPCTSTR strAccountName,LPCTSTR szServerName)///帐号上线
{
	///加锁,自动解锁
	CriticalLocker locker(m_cs);
	AccountInfo info={0};
	try
	{
		AccountIter it=m_accountContainer.find(idAccount);
		if(it==m_accountContainer.end())
		{
			info.idAccount=idAccount;
			info.nTimeout=0;///无超时帐号默认为0
			info.nAuthenID=nAuthenID;
			strncpy(info.szAccountName,strAccountName,_MAX_ACCOUNT_NAMESIZE);
			strncpy(info.szServerName,szServerName,_MAX_NAMESIZE);
			m_accountContainer[idAccount]=info;
			ChangeServerAccountCount(info.szServerName,true);///增加服务器人数
			LOGPOINT("帐户上线了!idAccount:%d,strAccountName:%s,szServerName:%s",idAccount,strAccountName,szServerName); 
		}
// 		else
// 			LOGCATCH("该死!补登时发现这个帐号已经存在,有bug吗?",idAccount); 
	}
	catch(...)
	{
		LOGCATCH("CAccountManager::OnlineAccount crash!"); 
	}
}