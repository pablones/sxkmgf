// VASDBManager.cpp: implementation of the CVASDBManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VASDBManager.h"
#include "Logfile.h"	//paled
#include "AllHeads.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//const char* sqlLocatRecord="select * from sk_vas_table where account_id = %d and server_name='%s'";
const char* sqlLocatRecord="select * from sk_vas_table where account_id = %d";
const char* sqlNewCardRecord="select * from sk_newcard where newcard = '%s'";
const char* sqlNewCardRecord2="select * from sk_newcard2 where newcard = '%s'";
const char* sqlVotCardRecord="select * from sk_tpcard where newcard = '%s'";
const char* sqlVotCardRecord2="select * from sk_tpcard2 where newcard = '%s'";
const char* sqlVotCardRecord3="select * from sk_tpcard3 where newcard = '%s'";
const char* sqlGetServerByAcc="select * from account where id = %d";
const char* sqlWriteLog="INSERT INTO ser_consumelog_table (account_name,ItemType_ID,ItemType_Amount,Spend_VAS_Point, OccurDate,Gift_User_ID,writeTime,Server_Name) VALUES ('%s',%d,%d,%d,'%s',%d,'%s','%s')";
const char* sqlNewVAS="INSERT INTO sk_vas_table (account_id,Server_Name,VAS_Point,LastConsumeDate, FirstConsumeDate) VALUES (%d,'%s',%d,'%s','%s')";
const char* sqlWriteSupplyLog="INSERT INTO ser_supplyvaslog_table (WebServerID,Account_Name,Server_Name,Type, Supply_VAS_Point,OccurDate,writeTime,gameMoney) VALUES (%d,'%s','%s',%d,%d,'%s','%s',%d)";
const char* sqlCheckAccount="select * from account where name='%s'";///将帐号名转成ID

const char* sqlWriteLifeTimeContinue="INSERT INTO ser_consumelog_table (type, account_name,ItemType_ID,ItemType_Amount,Spend_VAS_Point, OccurDate,Gift_User_ID,writeTime,Server_Name) VALUES (%d, '%s',%d,%d,%d,'%s',%d,'%s','%s')";
const char* sqlWriteVasTicketLog="INSERT INTO ser_vasticketlog (Account_Name,Oper_Type,ItemType_ID,VAS_Point,OccurDate,Gift_User_ID,Server_Name) VALUES ('%s',%d,%d,%d,'%s',%d,'%s')";

CVASDBManager::CVASDBManager()
{
	m_pRes=new CMyRecordset(&g_db);///生成数据库记录集对象
//	m_pRes=NULL;
}

CVASDBManager::~CVASDBManager()
{
	if(m_pRes)
	{
		m_pRes->Close();///关闭记录集
		delete m_pRes;///销毁对象
		m_pRes=NULL;
	}
}

BOOL CVASDBManager::GetAccountID(const CString& strAccountName)
{
	CMyRecordset Res(&g_db);
	///得到帐号ID
	BOOL bReulst=0;
	m_strSQL.Format(sqlCheckAccount,strAccountName);
	m_strSQL.MakeUpper();
	bReulst=Res.Open(m_strSQL);
	if (bReulst && Res.RecordCount())
		bReulst=Res.Fields("id");
	else
		bReulst=-1;
	Res.Close();
	return bReulst;
}
BOOL CVASDBManager::InitServer(ULONG idAccount)
{
	m_pRes->Close();

	m_strSQL.Format(sqlGetServerByAcc,idAccount);///生成SQL语句
	m_strSQL.MakeUpper();

	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{
		if(m_pRes->RecordCount())
		{
			return true;
		}
		else
			m_pRes->Close();
	}
	return false;
}

BOOL CVASDBManager::SetNewCard(const char* pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlNewCardRecord,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{
		if(m_pRes->RecordCount())
		{
			int nState = m_pRes->Fields("picked");
			if(nState == 1)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 0;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}
	return false;
}

BOOL CVASDBManager::CheckNewCard(const char* pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlNewCardRecord,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{ 
		if(m_pRes->RecordCount())
		{
			int nState = m_pRes->Fields("picked");
			if(nState == 0)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 1;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}
	return false;
}

BOOL CVASDBManager::SetNewCard2(ULONG idAccount,const char* pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlNewCardRecord2,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{
		if(m_pRes->RecordCount())
		{
			int nID = m_pRes->Fields("id");
			if(idAccount != nID)
				return false;
			int nState = m_pRes->Fields("picked");
			if(nState == 1)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 0;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}
	return false;
}

BOOL CVASDBManager::CheckNewCard2(ULONG idAccount,const char* pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlNewCardRecord2,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{ 
		if(m_pRes->RecordCount())
		{
			int nID = m_pRes->Fields("id");
			if(idAccount != nID)
				return false;

			int nState = m_pRes->Fields("picked");
			if(nState == 0)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 1;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}
	return false;
}

BOOL CVASDBManager::InitMananger(ULONG idAccount,const CString& strServerName)///初始化VAS数据库管理器,定位记录位置
{
//	BOOL bReulst=0;
//	m_strSQL.Format(sqlCheckAccount,strAccount);
//	m_strSQL.MakeUpper();
	m_pRes->Close();
//	bReulst=m_pRes->Open(m_strSQL);
//	if (m_pRes->m_pRes->RecordCount())
//		m_idAccount=m_pRes->Fields("id");
//	else
//	{
//		return -1;
//	}
	m_strSQL.Format(sqlLocatRecord,idAccount/*,strServerName*/);///生成SQL语句
	m_strSQL.MakeUpper();///将SQL统统大写，因为记录集

	///softworms为了调试vas系统而加的调试信息
//	::LogSave("Info: CVASDBManager::InitMananger Open() database for [%s]", m_strSQL);///日志记录
//	if (m_pRes)
//	{
//		delete m_pRes;
//		m_pRes=NULL;
//	}
//	m_pRes=(CMyRecordset*)((IDatabase*)&g_db)->CreateNewRecordset(m_strSQL);
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{
		///如果没有该帐号的VAS记录则新建一个余额为0的记录
		if(!m_pRes->RecordCount())
		{
			m_strSQL.Format(sqlNewVAS,idAccount,strServerName,0,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));///生成SQL语句
			m_strSQL.MakeUpper();
			bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
//			m_pRes->Close();
			InitMananger(idAccount,strServerName);///递归再查一次，定位记录集
		}
//		else
//			m_pRes->Close();
	}
	else
	{
		///SQL语句执行失败，进行日志
		::LogSave("ERROR: CVASDBManager::InitMananger can't Open() database for [%s],g_nMyDBError:%d", m_strSQL,g_nMyDBError);///日志记录
	}

	return bReulst;
}

UINT CVASDBManager::GetBalance()///得到帐户增值点余额
{
	//return (UINT)m_pRes->Fields("VAS_Point");
	return (long)m_pRes->Fields("VAS_Point");
}

UINT CVASDBManager::GetBalanceJuan()///得到帐户增值点余额
{
	//return (UINT)m_pRes->Fields("VAS_Point");
	return (long)m_pRes->Fields("VAS_Point_juan");
}

BOOL CVASDBManager::WriteConsumeLog(const CString& strAccountName,OBJID UserID,OBJID ItemID,UINT ItemAmount,UINT nTradePoint,const CString& strServerName)///写消费记录表,由ExecTrade调用
{
	///生成日期可能有错
	m_strSQL.Format(sqlWriteLog,strAccountName,ItemID,ItemAmount,nTradePoint,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),UserID,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),strServerName);///生成SQL语句
	m_strSQL.MakeUpper();
	BOOL bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	if(!bReulst)///执行SQL查询
		::LogSave("ERROR: CVASDBManager::WriteConsumeLog can't Open() database for [%s]", m_strSQL);///日志记录
	return bReulst; 
}

BOOL CVASDBManager::WriteSupplyLog(UINT idWebSite,const CString& strAccountName,const CString& strServerName,int nSupplyType,UINT nTradePoint,UINT nMoney)///写充值记录表,由ExecSupply调用,带游戏币兑换
{
	///生成日期可能有错
	m_strSQL.Format(sqlWriteSupplyLog,idWebSite,strAccountName,strServerName,nSupplyType,nTradePoint,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),nMoney);///生成SQL语句
	m_strSQL.MakeUpper();
	BOOL bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	if(!bReulst)///执行SQL查询
		::LogSave("ERROR: CVASDBManager::WriteSupplyLog can't Open() database for [%s]", m_strSQL);///日志记录
	return bReulst; 
}

BOOL CVASDBManager::WriteSupplyLog(UINT idWebSite,const CString& strAccountName,const CString& strServerName,int nSupplyType,UINT nTradePoint)///写消费记录表,由ExecTrade调用
{
	return WriteSupplyLog(idWebSite,strAccountName,strServerName,nSupplyType,nTradePoint,0);
}

BOOL CVASDBManager::WriteVasTicketLog(const CString& strAccountName,int nOperType,OBJID ItemTypeID,UINT nTradePoint,OBJID UserID,const CString& strServerName)///元宝票记录
{
	///生成日期可能有错
	m_strSQL.Format(sqlWriteVasTicketLog,strAccountName,nOperType,ItemTypeID,nTradePoint,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),UserID,strServerName);///生成SQL语句
	m_strSQL.MakeUpper();
	BOOL bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	if(!bReulst)///执行SQL查询
		::LogSave("ERROR: CVASDBManager::WriteVasTicketLog can't Open() database for [%s]", m_strSQL);///日志记录
	return bReulst; 
}

UINT CVASDBManager::ExecTrade(UINT nTradePoint,OBJID UserID,OBJID ItemID,UINT ItemAmount,const CString& strServerName,int nJuan)///进行增值交易,余额不足或交易失败返回-1,成功返回交易后余额
{
//	BOOL bResult=-1;
	UINT nBalance= 0 ;
	if(nJuan == 0)
		nBalance = GetBalance();///得到当前余额	
	else
		nBalance = GetBalanceJuan();///得到当前余额	
	

	///softworms调试VAS系统添加
//	::LogSave("Info: CVASDBManager::ExecTrade 帐户余额:nTradePoint-%d/UserID-%d/ItemID-%d/ItemAmount-%d/nBalance-%d",nTradePoint,UserID,ItemID,ItemAmount,nBalance);///日志记录

	UINT idAccount=(long)m_pRes->Fields("Account_ID");///得到增值项ID

	///softworms为了数据工具,将帐号ID改成帐号名.
	CString strAccountName="test";
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	{
		strAccountName	= g_cDatabase.GetName();
		g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
	else
	{
//		LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------


	if (nTradePoint > nBalance && nJuan != 5) ///帐户余额不足,交易失败
	{
		char temp[1024];
		sprintf(temp,"ERROR: CVASDBManager::ExecTrade 帐户余额不足,交易失败(nTradePoint=%d,UserID=%d,ItemID=%d,nJuan=%d,nBalance=%d)",nTradePoint,UserID,ItemID,nJuan,nBalance);
		::LogSave(/*"ERROR: CVASDBManager::ExecTrade 帐户余额不足,交易失败"*/temp);///日志记录
		nBalance=-1;
	}
	else
	{
		if(nJuan > 0)
		{
			if(nJuan == 5)
				nBalance += nTradePoint;
			else
				nBalance -= nTradePoint;
		}
		else
			nBalance-=nTradePoint;///扣点
		if(nTradePoint)///如果交易为查询则只返回余额,不进行下一步操作
		{
			m_pRes->Edit();
			if(nJuan == 0)
				m_pRes->Fields("VAS_Point") = (long)nBalance;
			else
				m_pRes->Fields("VAS_Point_juan") = (long)nBalance;
			
			///刷入最新消费时间
			strcpy(m_pRes->Fields("LastConsumeDate"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
			m_pRes->Update();///写数据库
//			m_pRes->Close();///关闭数据库
			///写日志
			if(ItemID > 0 && ItemAmount > 0)
				WriteConsumeLog(strAccountName,UserID,ItemID,ItemAmount,nTradePoint,strServerName);
		}
	}
	m_pRes->Close();///关闭记录集
	return nBalance;///返回余额
}
//[2010-07-27 goto]重载此函数，一次可以交易多种物品
UINT CVASDBManager::ExecTrade(UINT nTradePoint,OBJID UserID,const CString& strServerName, CMsgVAS::MsgTraffic* pGoods, int nSize)///进行增值交易,余额不足或交易失败返回-1,成功返回交易后余额
{
	//	BOOL bResult=-1;
	UINT nBalance= 0 ;
	nBalance = GetBalance();///得到当前余额	

	///softworms调试VAS系统添加
	//	::LogSave("Info: CVASDBManager::ExecTrade 帐户余额:nTradePoint-%d/UserID-%d/ItemID-%d/ItemAmount-%d/nBalance-%d",nTradePoint,UserID,ItemID,ItemAmount,nBalance);///日志记录
	
	UINT idAccount=(long)m_pRes->Fields("Account_ID");///得到增值项ID
	
	///softworms为了数据工具,将帐号ID改成帐号名.
	CString strAccountName="test";
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	{
		strAccountName	= g_cDatabase.GetName();
		g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
	else
	{
		return -1;
		//		LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------
	
	
	if (nTradePoint > nBalance) ///帐户余额不足,交易失败
	{
		char temp[1024];
		sprintf(temp,"ERROR: CVASDBManager::ExecTrade 帐户余额不足,交易失败(nTradePoint=%d,UserID=%d,nBalance=%d)",nTradePoint,UserID,nBalance);
		::LogSave(/*"ERROR: CVASDBManager::ExecTrade 帐户余额不足,交易失败"*/temp);///日志记录
		nBalance=-1;
	}
	else
	{
		nBalance-=nTradePoint;///扣点
		if(nTradePoint)///如果交易为查询则只返回余额,不进行下一步操作
		{
			m_pRes->Edit();

			m_pRes->Fields("VAS_Point") = (long)nBalance;

			///刷入最新消费时间
			strcpy(m_pRes->Fields("LastConsumeDate"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
			m_pRes->Update();///写数据库
			//			m_pRes->Close();///关闭数据库
			///写日志
			if(nSize > 0)
			{
				for(int i=0; i<nSize; i++)
				{
					if (pGoods[i].dwData==1)//元宝支付
						WriteConsumeLog(strAccountName,UserID,pGoods[i].itemID,pGoods[i].itemAmount,nTradePoint,strServerName);
				}
			}
		}
	}
	m_pRes->Close();///关闭记录集
	return nBalance;///返回余额
}
long CVASDBManager::ExecChg(UINT nTradePoint,const CString& strServerName,int nTradeType)//交换时空砖 1为扣 2为加
{
	UINT nBalance=GetBalance();

	if(nTradeType == 1 && nBalance < nTradePoint)
	{
		return -1;
	}

	UINT AccountID=(long)m_pRes->Fields("Account_ID");

	///softworms为了数据工具,将帐号ID改成帐号名.
// 	CString strAccountName;
// 	::WaitForSingleObject(g_xDatabase, INFINITE);	
// 	if(g_cDatabase.Create(AccountID))
// 	{
// 		strAccountName	= g_cDatabase.GetName();
// 		g_cDatabase.Destroy();			
// 	}	
// 	::ReleaseMutex(g_xDatabase);	

	if(nTradeType == 1)
	{
		nBalance -= nTradePoint;///扣点
	}
	else if(nTradeType == 2)
		nBalance += nTradePoint;///加点

	if(nTradePoint)
	{
		m_pRes->Edit();
		m_pRes->Fields("VAS_Point") = (long)nBalance;
		strcpy(m_pRes->Fields("LastConsumeDate"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
		m_pRes->Update();
	}
	m_pRes->Close();

	return nBalance;
}

UINT CVASDBManager::ExecSupply(UINT nTradePoint,const CString& strServerName,int idWebSite,int nTradeType)///充值
{
	return ExecSupply(nTradePoint,strServerName,idWebSite,nTradeType,0);
}

UINT CVASDBManager::ExecSupply(UINT nTradePoint,const CString& strServerName,int idWebSite,int nTradeType,UINT nMoney)///充值,带游戏币兑换,结果和同上
{
	UINT nBalance=GetBalance();///得到当前余额

	UINT AccountID=(long)m_pRes->Fields("Account_ID");///得到增值项ID

	///softworms为了数据工具,将帐号ID改成帐号名.
	CString strAccountName;
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(AccountID))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	{
		strAccountName	= g_cDatabase.GetName();
		g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
	else
	{
//		LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------

	nBalance+=nTradePoint;///扣点
	if(nTradePoint)///如果交易为查询则只返回余额,不进行下一步操作
	{
		m_pRes->Edit();
		m_pRes->Fields("VAS_Point") = (long)nBalance;
		///刷入最新消费时间
		strcpy(m_pRes->Fields("LastConsumeDate"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
		m_pRes->Update();///写数据库
		m_pRes->Close();///关闭数据库
		///写充值日志
		WriteSupplyLog(idWebSite,strAccountName,strServerName,nTradeType,nTradePoint,nMoney);
	}
	m_pRes->Close();
	return nBalance;
}

//  [2007-9-15] 朱斌	
UINT CVASDBManager::ExecItemLifeContinue(UINT nTradePoint, OBJID UserID, OBJID ItemID, UINT ItemAmount, const CString &strServerName, UINT uType)
{
//	BOOL bResult=-1;
	UINT nBalance=GetBalance();///得到当前余额

	///softworms调试VAS系统添加
//	::LogSave("Info: CVASDBManager::ExecTrade 帐户余额:nTradePoint-%d/UserID-%d/ItemID-%d/ItemAmount-%d/nBalance-%d",nTradePoint,UserID,ItemID,ItemAmount,nBalance);///日志记录

	UINT idAccount=(long)m_pRes->Fields("Account_ID");///得到增值项ID

	///softworms为了数据工具,将帐号ID改成帐号名.
	CString strAccountName;
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	{
		strAccountName	= g_cDatabase.GetName();
		g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
	else
	{
//		LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------

	if (nTradePoint>nBalance) ///帐户余额不足,交易失败
	{
		::LogSave("ERROR: CVASDBManager::ExecTrade 帐户余额不足,交易失败");///日志记录
		nBalance=-1;
	}
	else
	{
		nBalance-=nTradePoint;///扣点
		if(nTradePoint)///如果交易为查询则只返回余额,不进行下一步操作
		{
			m_pRes->Edit();
			m_pRes->Fields("VAS_Point") = (long)nBalance;
			///刷入最新消费时间
			strcpy(m_pRes->Fields("LastConsumeDate"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
			m_pRes->Update();///写数据库
//			m_pRes->Close();///关闭数据库
			///写日志
			WriteLifeTimeContinueLog(strAccountName,UserID,ItemID,1,nTradePoint,strServerName, uType);
		}
	}
	m_pRes->Close();///关闭记录集
	return nBalance;///返回余额
}


//////////////////////////////////////////////////////////////////////////
//  [2007-9-20] 朱斌	续费冲值
BOOL CVASDBManager::WriteLifeTimeContinueLog(const CString& strAccountName,OBJID UserID,OBJID ItemID,UINT ItemAmount,UINT nTradePoint,const CString& strServerName, UINT uType)///写消费记录表,由ExecTrade调用
{
	///生成日期可能有错
	m_strSQL.Format(sqlWriteLifeTimeContinue,uType, strAccountName,ItemID,ItemAmount,nTradePoint,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),UserID,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),strServerName);///生成SQL语句
	m_strSQL.MakeUpper();
	BOOL bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	if(!bReulst)	///执行SQL查询
		::LogSave("ERROR: CVASDBManager::WriteLifeTimeContinueLog can't Open() database for [%s]", m_strSQL);///日志记录
	return bReulst; 
}

int CVASDBManager::ExecMaskDo(int data,int action ,int check)
{
	int nChange = 0;
	int data_save=(int)m_pRes->Fields("day_mask_data");
	switch(action)
	{
	case 1:
		{
		}
		break;
	case 2:
		{
			nChange = 1;
		}
		break;
	}

	char checktime[32];
	strcpy(checktime,m_pRes->Fields("day_mask_time"));
	int nYear,nMonth,nDay;
	sscanf(checktime,"%d-%d-%d",&nYear,&nMonth,&nDay);
	SYSTEMTIME m_curTime;
	::GetLocalTime(&m_curTime);
	CTime	timecur(m_curTime);
	CTimeSpan span(0,2,55,0);
	timecur = timecur - span;
	timecur.GetAsSystemTime(m_curTime);
	if(m_curTime.wYear != nYear || m_curTime.wMonth != nMonth || m_curTime.wDay != nDay) //不是同一天，清除
	{
		nChange = 2;
		data = 0;					
	}


	
	if(nChange > 0) //数字
	{
		m_pRes->Edit();
		m_pRes->Fields("day_mask_data") = data;
		if(nChange > 1)
		{
			sprintf(checktime,"%d-%d-%d",m_curTime.wYear,m_curTime.wMonth,m_curTime.wDay);
			strcpy(m_pRes->Fields("day_mask_time"), checktime);
		}
		m_pRes->Update();
		data_save=data;
	}

	
	m_pRes->Close();
	return data_save;
	
}

int CVASDBManager::ExecServerChk(int server,int action ,int data)
{
	int status=(long)m_pRes->Fields("status_serves");
	switch(server)
	{
	case 1:
		{
			if(action == 1)
			{
				if(status & 0x00000001 == 0)
				{
					m_pRes->Close();
					return -1; //没有服务返回-1
				}
				int value_server1=(long)m_pRes->Fields("Value_serve1");
				char checktime[32];
				strcpy(checktime,(char*)m_pRes->Fields("time_serve1"));
				int nYear,nMonth,nDay,nHour,nMinute;
				sscanf(checktime,"%d-%d-%d-%d-%d",&nYear,&nMonth,&nDay,&nHour,&nMinute);
				
				if((value_server1 == 0) || (nYear == 0 && nMonth == 0 && nDay == 0 && nHour == 0 && nMinute == 0) )
				{
					m_pRes->Close();
					return -1; //没有服务返回-1
				}
				
				SYSTEMTIME m_curTime,m_chkTime;
				::GetLocalTime(&m_curTime);
				m_chkTime.wYear = nYear;
				m_chkTime.wMonth = nMonth;
				m_chkTime.wDay = nDay;
				m_chkTime.wHour = nHour;
				m_chkTime.wMinute = nMinute;
				m_chkTime.wSecond = m_curTime.wSecond;
				m_chkTime.wMilliseconds = m_curTime.wMilliseconds;
				
				CTime	timechk(m_chkTime);
				CTime	timecur(m_curTime);	
				CTimeSpan span(0,0,value_server1,0);
				timechk += span; //检查的时间 + 剩余时间 = 到期时间
				if(timechk > m_curTime) //没到期
				{
					CTimeSpan spanEnd = timechk - timecur; //剩余时间
					m_pRes->Edit();
					m_pRes->Fields("Value_serve1") = (long)spanEnd.GetTotalMinutes();
					sprintf(checktime,"%d-%d-%d-%d-%d",m_curTime.wYear,m_curTime.wMonth,m_curTime.wDay,m_curTime.wHour,m_curTime.wMinute);
					strcpy(m_pRes->Fields("time_serve1"), checktime);
					m_pRes->Update();///写数据库
					
					m_pRes->Close();
					return spanEnd.GetTotalMinutes(); //返回实际剩余时间
				}
				else
				{
					m_pRes->Edit();
					m_pRes->Fields("status_serves") = status & (~0x00000001);
					m_pRes->Fields("Value_serve1") = 0;
					sprintf(checktime,"%d-%d-%d-%d-%d",0,0,0,0,0);
					strcpy(m_pRes->Fields("time_serve1"), checktime);
					m_pRes->Update();///写数据库
					
					m_pRes->Close();
					return 0; //返回实际剩余时间
					
				}
			}
			else if(action == 2)
			{
				if(data > 0)
				{
					SYSTEMTIME m_curTime;
					::GetLocalTime(&m_curTime);
					char checktime[32];
					sprintf(checktime,"%d-%d-%d-%d-%d",m_curTime.wYear,m_curTime.wMonth,m_curTime.wDay,m_curTime.wHour,m_curTime.wMinute);

					m_pRes->Edit();
					m_pRes->Fields("status_serves") = status | 0x00000001;
					m_pRes->Fields("Value_serve1") = data;					
					strcpy(m_pRes->Fields("time_serve1"), checktime);
					m_pRes->Update();///写数据库

					m_pRes->Close();
					return data;
				}				
			}
		}
		break;
	}
	m_pRes->Close();
	return -1;
}

__int64 CVASDBManager::ExecChg(const CString &cstrServerName, __int64 iTradePoint)
{
	__int64 nBalance = GetBalance();//得到当前余额

	UINT AccountID=(long)m_pRes->Fields("Account_ID");///得到增值项ID

	nBalance += iTradePoint;

	if (0 > nBalance)
	{
		m_pRes->Close();
		return nBalance;
	}

	///softworms为了数据工具,将帐号ID改成帐号名.
	CString strAccountName;
	::WaitForSingleObject(g_xDatabase, INFINITE);	
	if(g_cDatabase.Create(AccountID))
	{
		strAccountName	= g_cDatabase.GetName();
		g_cDatabase.Destroy();			
	}	
	::ReleaseMutex(g_xDatabase);	

	if (iTradePoint)
	{
		m_pRes->Edit();
		m_pRes->Fields("VAS_Point") = (long)nBalance;	
		strcpy(m_pRes->Fields("LastConsumeDate"), COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
		m_pRes->Update();///写数据库
	}
	m_pRes->Close();
	return nBalance;
}

BOOL CVASDBManager::CheckVotCard(const char* pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlVotCardRecord,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{ 
		if(m_pRes->RecordCount())
		{
			int nState = m_pRes->Fields("picked");
			if(nState == 0)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 1;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}         
	return false;
}

BOOL CVASDBManager::SetVotCard(const char *pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlVotCardRecord,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{
		if(m_pRes->RecordCount())
		{
			int nState = m_pRes->Fields("picked");
			if(nState == 1)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 0;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}
	return false;
}

BOOL CVASDBManager::CheckVotCard2(const char* pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlVotCardRecord2,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{ 
		if(m_pRes->RecordCount())
		{
			int nState = m_pRes->Fields("picked");
			if(nState == 0)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 1;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}         
	return false;
}

BOOL CVASDBManager::SetVotCard2(const char *pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlVotCardRecord2,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{
		if(m_pRes->RecordCount())
		{
			int nState = m_pRes->Fields("picked");
			if(nState == 1)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 0;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}
	return false;
}

BOOL CVASDBManager::CheckVotCard3(const char* pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlVotCardRecord3,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{ 
		if(m_pRes->RecordCount())
		{
			int nState = m_pRes->Fields("picked");
			if(nState == 0)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 1;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}         
	return false;
}

BOOL CVASDBManager::SetVotCard3(const char *pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlVotCardRecord3,pszCard);///生成SQL语句
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///执行SQL查询
	{
		if(m_pRes->RecordCount())
		{
			int nState = m_pRes->Fields("picked");
			if(nState == 1)
			{
				m_pRes->Edit();
				m_pRes->Fields("picked") = 0;
				m_pRes->Update();
				m_pRes->Close();
				return true;
			}
		}
		m_pRes->Close();
	}
	return false;
}