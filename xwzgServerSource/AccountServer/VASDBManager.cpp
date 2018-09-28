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
const char* sqlCheckAccount="select * from account where name='%s'";///���ʺ���ת��ID

const char* sqlWriteLifeTimeContinue="INSERT INTO ser_consumelog_table (type, account_name,ItemType_ID,ItemType_Amount,Spend_VAS_Point, OccurDate,Gift_User_ID,writeTime,Server_Name) VALUES (%d, '%s',%d,%d,%d,'%s',%d,'%s','%s')";
const char* sqlWriteVasTicketLog="INSERT INTO ser_vasticketlog (Account_Name,Oper_Type,ItemType_ID,VAS_Point,OccurDate,Gift_User_ID,Server_Name) VALUES ('%s',%d,%d,%d,'%s',%d,'%s')";

CVASDBManager::CVASDBManager()
{
	m_pRes=new CMyRecordset(&g_db);///�������ݿ��¼������
//	m_pRes=NULL;
}

CVASDBManager::~CVASDBManager()
{
	if(m_pRes)
	{
		m_pRes->Close();///�رռ�¼��
		delete m_pRes;///���ٶ���
		m_pRes=NULL;
	}
}

BOOL CVASDBManager::GetAccountID(const CString& strAccountName)
{
	CMyRecordset Res(&g_db);
	///�õ��ʺ�ID
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

	m_strSQL.Format(sqlGetServerByAcc,idAccount);///����SQL���
	m_strSQL.MakeUpper();

	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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
	m_strSQL.Format(sqlNewCardRecord,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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
	m_strSQL.Format(sqlNewCardRecord,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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
	m_strSQL.Format(sqlNewCardRecord2,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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
	m_strSQL.Format(sqlNewCardRecord2,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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

BOOL CVASDBManager::InitMananger(ULONG idAccount,const CString& strServerName)///��ʼ��VAS���ݿ������,��λ��¼λ��
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
	m_strSQL.Format(sqlLocatRecord,idAccount/*,strServerName*/);///����SQL���
	m_strSQL.MakeUpper();///��SQLͳͳ��д����Ϊ��¼��

	///softwormsΪ�˵���vasϵͳ���ӵĵ�����Ϣ
//	::LogSave("Info: CVASDBManager::InitMananger Open() database for [%s]", m_strSQL);///��־��¼
//	if (m_pRes)
//	{
//		delete m_pRes;
//		m_pRes=NULL;
//	}
//	m_pRes=(CMyRecordset*)((IDatabase*)&g_db)->CreateNewRecordset(m_strSQL);
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
	{
		///���û�и��ʺŵ�VAS��¼���½�һ�����Ϊ0�ļ�¼
		if(!m_pRes->RecordCount())
		{
			m_strSQL.Format(sqlNewVAS,idAccount,strServerName,0,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));///����SQL���
			m_strSQL.MakeUpper();
			bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
//			m_pRes->Close();
			InitMananger(idAccount,strServerName);///�ݹ��ٲ�һ�Σ���λ��¼��
		}
//		else
//			m_pRes->Close();
	}
	else
	{
		///SQL���ִ��ʧ�ܣ�������־
		::LogSave("ERROR: CVASDBManager::InitMananger can't Open() database for [%s],g_nMyDBError:%d", m_strSQL,g_nMyDBError);///��־��¼
	}

	return bReulst;
}

UINT CVASDBManager::GetBalance()///�õ��ʻ���ֵ�����
{
	//return (UINT)m_pRes->Fields("VAS_Point");
	return (long)m_pRes->Fields("VAS_Point");
}

UINT CVASDBManager::GetBalanceJuan()///�õ��ʻ���ֵ�����
{
	//return (UINT)m_pRes->Fields("VAS_Point");
	return (long)m_pRes->Fields("VAS_Point_juan");
}

BOOL CVASDBManager::WriteConsumeLog(const CString& strAccountName,OBJID UserID,OBJID ItemID,UINT ItemAmount,UINT nTradePoint,const CString& strServerName)///д���Ѽ�¼��,��ExecTrade����
{
	///�������ڿ����д�
	m_strSQL.Format(sqlWriteLog,strAccountName,ItemID,ItemAmount,nTradePoint,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),UserID,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),strServerName);///����SQL���
	m_strSQL.MakeUpper();
	BOOL bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	if(!bReulst)///ִ��SQL��ѯ
		::LogSave("ERROR: CVASDBManager::WriteConsumeLog can't Open() database for [%s]", m_strSQL);///��־��¼
	return bReulst; 
}

BOOL CVASDBManager::WriteSupplyLog(UINT idWebSite,const CString& strAccountName,const CString& strServerName,int nSupplyType,UINT nTradePoint,UINT nMoney)///д��ֵ��¼��,��ExecSupply����,����Ϸ�Ҷһ�
{
	///�������ڿ����д�
	m_strSQL.Format(sqlWriteSupplyLog,idWebSite,strAccountName,strServerName,nSupplyType,nTradePoint,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),nMoney);///����SQL���
	m_strSQL.MakeUpper();
	BOOL bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	if(!bReulst)///ִ��SQL��ѯ
		::LogSave("ERROR: CVASDBManager::WriteSupplyLog can't Open() database for [%s]", m_strSQL);///��־��¼
	return bReulst; 
}

BOOL CVASDBManager::WriteSupplyLog(UINT idWebSite,const CString& strAccountName,const CString& strServerName,int nSupplyType,UINT nTradePoint)///д���Ѽ�¼��,��ExecTrade����
{
	return WriteSupplyLog(idWebSite,strAccountName,strServerName,nSupplyType,nTradePoint,0);
}

BOOL CVASDBManager::WriteVasTicketLog(const CString& strAccountName,int nOperType,OBJID ItemTypeID,UINT nTradePoint,OBJID UserID,const CString& strServerName)///Ԫ��Ʊ��¼
{
	///�������ڿ����д�
	m_strSQL.Format(sqlWriteVasTicketLog,strAccountName,nOperType,ItemTypeID,nTradePoint,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),UserID,strServerName);///����SQL���
	m_strSQL.MakeUpper();
	BOOL bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	if(!bReulst)///ִ��SQL��ѯ
		::LogSave("ERROR: CVASDBManager::WriteVasTicketLog can't Open() database for [%s]", m_strSQL);///��־��¼
	return bReulst; 
}

UINT CVASDBManager::ExecTrade(UINT nTradePoint,OBJID UserID,OBJID ItemID,UINT ItemAmount,const CString& strServerName,int nJuan)///������ֵ����,�������ʧ�ܷ���-1,�ɹ����ؽ��׺����
{
//	BOOL bResult=-1;
	UINT nBalance= 0 ;
	if(nJuan == 0)
		nBalance = GetBalance();///�õ���ǰ���	
	else
		nBalance = GetBalanceJuan();///�õ���ǰ���	
	

	///softworms����VASϵͳ���
//	::LogSave("Info: CVASDBManager::ExecTrade �ʻ����:nTradePoint-%d/UserID-%d/ItemID-%d/ItemAmount-%d/nBalance-%d",nTradePoint,UserID,ItemID,ItemAmount,nBalance);///��־��¼

	UINT idAccount=(long)m_pRes->Fields("Account_ID");///�õ���ֵ��ID

	///softwormsΪ�����ݹ���,���ʺ�ID�ĳ��ʺ���.
	CString strAccountName="test";
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	{
		strAccountName	= g_cDatabase.GetName();
		g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
	else
	{
//		LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------


	if (nTradePoint > nBalance && nJuan != 5) ///�ʻ�����,����ʧ��
	{
		char temp[1024];
		sprintf(temp,"ERROR: CVASDBManager::ExecTrade �ʻ�����,����ʧ��(nTradePoint=%d,UserID=%d,ItemID=%d,nJuan=%d,nBalance=%d)",nTradePoint,UserID,ItemID,nJuan,nBalance);
		::LogSave(/*"ERROR: CVASDBManager::ExecTrade �ʻ�����,����ʧ��"*/temp);///��־��¼
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
			nBalance-=nTradePoint;///�۵�
		if(nTradePoint)///�������Ϊ��ѯ��ֻ�������,��������һ������
		{
			m_pRes->Edit();
			if(nJuan == 0)
				m_pRes->Fields("VAS_Point") = (long)nBalance;
			else
				m_pRes->Fields("VAS_Point_juan") = (long)nBalance;
			
			///ˢ����������ʱ��
			strcpy(m_pRes->Fields("LastConsumeDate"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
			m_pRes->Update();///д���ݿ�
//			m_pRes->Close();///�ر����ݿ�
			///д��־
			if(ItemID > 0 && ItemAmount > 0)
				WriteConsumeLog(strAccountName,UserID,ItemID,ItemAmount,nTradePoint,strServerName);
		}
	}
	m_pRes->Close();///�رռ�¼��
	return nBalance;///�������
}
//[2010-07-27 goto]���ش˺�����һ�ο��Խ��׶�����Ʒ
UINT CVASDBManager::ExecTrade(UINT nTradePoint,OBJID UserID,const CString& strServerName, CMsgVAS::MsgTraffic* pGoods, int nSize)///������ֵ����,�������ʧ�ܷ���-1,�ɹ����ؽ��׺����
{
	//	BOOL bResult=-1;
	UINT nBalance= 0 ;
	nBalance = GetBalance();///�õ���ǰ���	

	///softworms����VASϵͳ���
	//	::LogSave("Info: CVASDBManager::ExecTrade �ʻ����:nTradePoint-%d/UserID-%d/ItemID-%d/ItemAmount-%d/nBalance-%d",nTradePoint,UserID,ItemID,ItemAmount,nBalance);///��־��¼
	
	UINT idAccount=(long)m_pRes->Fields("Account_ID");///�õ���ֵ��ID
	
	///softwormsΪ�����ݹ���,���ʺ�ID�ĳ��ʺ���.
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
		//		LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------
	
	
	if (nTradePoint > nBalance) ///�ʻ�����,����ʧ��
	{
		char temp[1024];
		sprintf(temp,"ERROR: CVASDBManager::ExecTrade �ʻ�����,����ʧ��(nTradePoint=%d,UserID=%d,nBalance=%d)",nTradePoint,UserID,nBalance);
		::LogSave(/*"ERROR: CVASDBManager::ExecTrade �ʻ�����,����ʧ��"*/temp);///��־��¼
		nBalance=-1;
	}
	else
	{
		nBalance-=nTradePoint;///�۵�
		if(nTradePoint)///�������Ϊ��ѯ��ֻ�������,��������һ������
		{
			m_pRes->Edit();

			m_pRes->Fields("VAS_Point") = (long)nBalance;

			///ˢ����������ʱ��
			strcpy(m_pRes->Fields("LastConsumeDate"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
			m_pRes->Update();///д���ݿ�
			//			m_pRes->Close();///�ر����ݿ�
			///д��־
			if(nSize > 0)
			{
				for(int i=0; i<nSize; i++)
				{
					if (pGoods[i].dwData==1)//Ԫ��֧��
						WriteConsumeLog(strAccountName,UserID,pGoods[i].itemID,pGoods[i].itemAmount,nTradePoint,strServerName);
				}
			}
		}
	}
	m_pRes->Close();///�رռ�¼��
	return nBalance;///�������
}
long CVASDBManager::ExecChg(UINT nTradePoint,const CString& strServerName,int nTradeType)//����ʱ��ש 1Ϊ�� 2Ϊ��
{
	UINT nBalance=GetBalance();

	if(nTradeType == 1 && nBalance < nTradePoint)
	{
		return -1;
	}

	UINT AccountID=(long)m_pRes->Fields("Account_ID");

	///softwormsΪ�����ݹ���,���ʺ�ID�ĳ��ʺ���.
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
		nBalance -= nTradePoint;///�۵�
	}
	else if(nTradeType == 2)
		nBalance += nTradePoint;///�ӵ�

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

UINT CVASDBManager::ExecSupply(UINT nTradePoint,const CString& strServerName,int idWebSite,int nTradeType)///��ֵ
{
	return ExecSupply(nTradePoint,strServerName,idWebSite,nTradeType,0);
}

UINT CVASDBManager::ExecSupply(UINT nTradePoint,const CString& strServerName,int idWebSite,int nTradeType,UINT nMoney)///��ֵ,����Ϸ�Ҷһ�,�����ͬ��
{
	UINT nBalance=GetBalance();///�õ���ǰ���

	UINT AccountID=(long)m_pRes->Fields("Account_ID");///�õ���ֵ��ID

	///softwormsΪ�����ݹ���,���ʺ�ID�ĳ��ʺ���.
	CString strAccountName;
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(AccountID))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	{
		strAccountName	= g_cDatabase.GetName();
		g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
	else
	{
//		LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------

	nBalance+=nTradePoint;///�۵�
	if(nTradePoint)///�������Ϊ��ѯ��ֻ�������,��������һ������
	{
		m_pRes->Edit();
		m_pRes->Fields("VAS_Point") = (long)nBalance;
		///ˢ����������ʱ��
		strcpy(m_pRes->Fields("LastConsumeDate"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
		m_pRes->Update();///д���ݿ�
		m_pRes->Close();///�ر����ݿ�
		///д��ֵ��־
		WriteSupplyLog(idWebSite,strAccountName,strServerName,nTradeType,nTradePoint,nMoney);
	}
	m_pRes->Close();
	return nBalance;
}

//  [2007-9-15] ���	
UINT CVASDBManager::ExecItemLifeContinue(UINT nTradePoint, OBJID UserID, OBJID ItemID, UINT ItemAmount, const CString &strServerName, UINT uType)
{
//	BOOL bResult=-1;
	UINT nBalance=GetBalance();///�õ���ǰ���

	///softworms����VASϵͳ���
//	::LogSave("Info: CVASDBManager::ExecTrade �ʻ����:nTradePoint-%d/UserID-%d/ItemID-%d/ItemAmount-%d/nBalance-%d",nTradePoint,UserID,ItemID,ItemAmount,nBalance);///��־��¼

	UINT idAccount=(long)m_pRes->Fields("Account_ID");///�õ���ֵ��ID

	///softwormsΪ�����ݹ���,���ʺ�ID�ĳ��ʺ���.
	CString strAccountName;
	::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
	if(g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	{
		strAccountName	= g_cDatabase.GetName();
		g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
	else
	{
//		LOGERROR("��Ϸ������[%s]�����ѯһ��δ֪�ʺ�[%d]�Ʒ�����", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
	}
	::ReleaseMutex(g_xDatabase);	//------------------------------------------

	if (nTradePoint>nBalance) ///�ʻ�����,����ʧ��
	{
		::LogSave("ERROR: CVASDBManager::ExecTrade �ʻ�����,����ʧ��");///��־��¼
		nBalance=-1;
	}
	else
	{
		nBalance-=nTradePoint;///�۵�
		if(nTradePoint)///�������Ϊ��ѯ��ֻ�������,��������һ������
		{
			m_pRes->Edit();
			m_pRes->Fields("VAS_Point") = (long)nBalance;
			///ˢ����������ʱ��
			strcpy(m_pRes->Fields("LastConsumeDate"),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
			m_pRes->Update();///д���ݿ�
//			m_pRes->Close();///�ر����ݿ�
			///д��־
			WriteLifeTimeContinueLog(strAccountName,UserID,ItemID,1,nTradePoint,strServerName, uType);
		}
	}
	m_pRes->Close();///�رռ�¼��
	return nBalance;///�������
}


//////////////////////////////////////////////////////////////////////////
//  [2007-9-20] ���	���ѳ�ֵ
BOOL CVASDBManager::WriteLifeTimeContinueLog(const CString& strAccountName,OBJID UserID,OBJID ItemID,UINT ItemAmount,UINT nTradePoint,const CString& strServerName, UINT uType)///д���Ѽ�¼��,��ExecTrade����
{
	///�������ڿ����д�
	m_strSQL.Format(sqlWriteLifeTimeContinue,uType, strAccountName,ItemID,ItemAmount,nTradePoint,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),UserID,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),strServerName);///����SQL���
	m_strSQL.MakeUpper();
	BOOL bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	if(!bReulst)	///ִ��SQL��ѯ
		::LogSave("ERROR: CVASDBManager::WriteLifeTimeContinueLog can't Open() database for [%s]", m_strSQL);///��־��¼
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
	if(m_curTime.wYear != nYear || m_curTime.wMonth != nMonth || m_curTime.wDay != nDay) //����ͬһ�죬���
	{
		nChange = 2;
		data = 0;					
	}


	
	if(nChange > 0) //����
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
					return -1; //û�з��񷵻�-1
				}
				int value_server1=(long)m_pRes->Fields("Value_serve1");
				char checktime[32];
				strcpy(checktime,(char*)m_pRes->Fields("time_serve1"));
				int nYear,nMonth,nDay,nHour,nMinute;
				sscanf(checktime,"%d-%d-%d-%d-%d",&nYear,&nMonth,&nDay,&nHour,&nMinute);
				
				if((value_server1 == 0) || (nYear == 0 && nMonth == 0 && nDay == 0 && nHour == 0 && nMinute == 0) )
				{
					m_pRes->Close();
					return -1; //û�з��񷵻�-1
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
				timechk += span; //����ʱ�� + ʣ��ʱ�� = ����ʱ��
				if(timechk > m_curTime) //û����
				{
					CTimeSpan spanEnd = timechk - timecur; //ʣ��ʱ��
					m_pRes->Edit();
					m_pRes->Fields("Value_serve1") = (long)spanEnd.GetTotalMinutes();
					sprintf(checktime,"%d-%d-%d-%d-%d",m_curTime.wYear,m_curTime.wMonth,m_curTime.wDay,m_curTime.wHour,m_curTime.wMinute);
					strcpy(m_pRes->Fields("time_serve1"), checktime);
					m_pRes->Update();///д���ݿ�
					
					m_pRes->Close();
					return spanEnd.GetTotalMinutes(); //����ʵ��ʣ��ʱ��
				}
				else
				{
					m_pRes->Edit();
					m_pRes->Fields("status_serves") = status & (~0x00000001);
					m_pRes->Fields("Value_serve1") = 0;
					sprintf(checktime,"%d-%d-%d-%d-%d",0,0,0,0,0);
					strcpy(m_pRes->Fields("time_serve1"), checktime);
					m_pRes->Update();///д���ݿ�
					
					m_pRes->Close();
					return 0; //����ʵ��ʣ��ʱ��
					
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
					m_pRes->Update();///д���ݿ�

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
	__int64 nBalance = GetBalance();//�õ���ǰ���

	UINT AccountID=(long)m_pRes->Fields("Account_ID");///�õ���ֵ��ID

	nBalance += iTradePoint;

	if (0 > nBalance)
	{
		m_pRes->Close();
		return nBalance;
	}

	///softwormsΪ�����ݹ���,���ʺ�ID�ĳ��ʺ���.
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
		m_pRes->Update();///д���ݿ�
	}
	m_pRes->Close();
	return nBalance;
}

BOOL CVASDBManager::CheckVotCard(const char* pszCard)
{
	m_pRes->Close();
	m_strSQL.Format(sqlVotCardRecord,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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
	m_strSQL.Format(sqlVotCardRecord,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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
	m_strSQL.Format(sqlVotCardRecord2,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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
	m_strSQL.Format(sqlVotCardRecord2,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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
	m_strSQL.Format(sqlVotCardRecord3,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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
	m_strSQL.Format(sqlVotCardRecord3,pszCard);///����SQL���
	m_strSQL.MakeUpper();
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
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