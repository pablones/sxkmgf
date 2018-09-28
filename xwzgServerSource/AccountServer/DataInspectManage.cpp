// DataInspectManage.cpp: implementation of the CDataInspectManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataInspectManage.h"
#include "Logfile.h"	//paled

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
//���ƣ����ֿ������ִ��ⶼ������һ����TYPE���֡�1�����ƣ�2�����ֿ���3�����ִ��� ��������ֻ��֪��������Ĵ���������Ҫ֪������
const DWORD	PRESENT_ONE				= 0x01;				//�͵�1�ε�װ��
const DWORD	PRESENT_TWO				= 0x02;				//�͵�2�ε�װ��
const DWORD	PRESENT_THREE			= 0x04;				//�͵�3�ε�װ��
const DWORD	PRESENT_FOUR			= 0x08;				//�͵�4�ε�װ��
const DWORD	PRESENT_FIVE			= 0x10;				//�͵�5�ε�װ��
const DWORD	PRESENT_SIX				= 0x20;				//�͵�6�ε�װ��
const DWORD	PRESENT_SEVEN			= 0x40;				//�͵�7�ε�װ��
const DWORD	PRESENT_EIGHT			= 0x80;				//�͵�8�ε�װ��
*/
CDataInspectManage::CDataInspectManage()
{
	m_pRes=new CMyRecordset(&g_db);///�������ݿ��¼������
}

CDataInspectManage::~CDataInspectManage()
{
	if( m_pRes )
	{
		m_pRes->Close();///�رռ�¼��
		delete m_pRes;///���ٶ���
		m_pRes = NULL;
	}
}

///�����ʺ�����ʱ��
BOOL CDataInspectManage::UpdateAccoutOnlineTime(OBJID idAccount,const CString& strServerName)
{
	//CString thisTime = COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	COleDateTime dtLoginTime;
	COleDateTimeSpan dtsPlayTime;
	m_strSQL.Format("select * from ser_loginlog where account_id=%d and remark='%s' order by landtime desc limit 1",idAccount, strServerName );
	m_strSQL.MakeUpper();///��SQLͳͳ��д����Ϊ��¼��
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
	{
		///���û�и��ʺŵ�ser_loginlog��¼���½�һ����½��¼
 		if( m_pRes->RecordCount() )
		{
			//UINT iTimes = m_pRes->Fields("times");
			CString strLandTime=(LPCSTR)m_pRes->Fields("landTime");
			if(dtLoginTime.ParseDateTime(strLandTime))
			{
				dtsPlayTime=COleDateTime::GetCurrentTime()-dtLoginTime;
				m_pRes->Edit();
	 			m_pRes->Fields("times") = (long)dtsPlayTime.GetTotalSeconds();
	 			m_pRes->Update();//�������ݿ�
			}
		}
	}
	m_pRes->Close();
	return true;
}

BOOL CDataInspectManage::WriteMananger(const CString& strServerName,const CString& strAccountName, UINT nAccountID,const CString& strServerIP,const CString& strMac)///֧�ּ�¼�ͻ���IP�����������ַ.
{
	BOOL hr=false;
	CString thisTime = COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
// 	m_strSQL.Format("select * from ser_loginlog where accountname='%s' and landtime='%s'",strAccountName, thisTime );
// 	m_strSQL.MakeUpper();///��SQLͳͳ��д����Ϊ��¼��
// 	BOOL bReulst=m_pRes->Open(m_strSQL);
// 	if(bReulst)///ִ��SQL��ѯ
// 	{
		///���û�и��ʺŵ�ser_loginlog��¼���½�һ����½��¼
// 		if( 0 == m_pRes->RecordCount() )
// 		{
			m_strSQL.Format("INSERT INTO ser_loginlog (remark,account_id, accountname,landtime,times,clientip,addressmac,writeTime) VALUES ('%s', %d, '%s', '%s', %d,'%s','%s','%s' )",strServerName,nAccountID,strAccountName,thisTime, 1,strServerIP,strMac,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );///����SQL���
			m_strSQL.MakeUpper();
			hr=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
// 		}
// 		else
// 		{
// 			UINT iTimes = m_pRes->Fields("times");
// 			m_pRes->Edit();
// 			m_pRes->Fields("times") = ( (long)iTimes + 1 );
// 			strcpy( m_pRes->Fields("writeTime"), COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M") );
// 			m_pRes->Update();//�������ݿ�
// 		}
// 	}
// 	else
// 	{
// 		///SQL���ִ��ʧ�ܣ�������־
// 		::LogSave("ERROR: CDataInspectManage::WriteMananger can't Open() database for [%s]", m_strSQL);///��־��¼
// 	}
// 	m_pRes->Close();
	return hr;
}

BOOL CDataInspectManage::WriteMananger( const CString& strAccountName, UINT nAccountID )///��ʼ��ser_loginlog���ݿ������,ֱ��д���ݡ�
{
	CString thisTime = COleDateTime::GetCurrentTime().Format("%Y-%m-%d");
	m_strSQL.Format("select * from ser_loginlog where accountname='%s' and landtime='%s'",strAccountName, thisTime );
	m_strSQL.MakeUpper();///��SQLͳͳ��д����Ϊ��¼��

	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
	{
		///���û�и��ʺŵ�ser_loginlog��¼���½�һ����½��¼
		if( 0 == m_pRes->RecordCount() )
		{
			m_strSQL.Format("INSERT INTO ser_loginlog ( account_id, accountname,landtime,times,writeTime) VALUES ( %d, '%s', '%s', %d, '%s' )",nAccountID,strAccountName,thisTime, 1, COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M") );///����SQL���
			m_strSQL.MakeUpper();
			bReulst=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
			m_pRes->Close();
		}
		else
		{
			UINT iTimes = m_pRes->Fields("times");
			m_pRes->Edit();
			m_pRes->Fields("times") = ( (long)iTimes + 1 );
			strcpy( m_pRes->Fields("writeTime"), COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M") );
			m_pRes->Update();//�������ݿ�
			m_pRes->Close();
		}
	}
	else
	{
		///SQL���ִ��ʧ�ܣ�������־
		::LogSave("ERROR: CDataInspectManage::WriteMananger can't Open() database for [%s]", m_strSQL);///��־��¼
	}
	m_pRes->Close();

	
	return bReulst;
}

BOOL CDataInspectManage::GetNewPlayerExamState( UINT nAccountID, char* state,int saveSize, USHORT& size )
{
	//2007928����:!��Ҫ.�������.���г�ʼ��.
	size = 0;

	m_strSQL.Format( "select * from sk_generalize where account_id=%d and type = 3", nAccountID);
	m_strSQL.MakeUpper();

	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
	{
		if( 0 == m_pRes->RecordCount() )
		{
			m_pRes->Close();
			return false;
		}
		else
		{
			for(int i = 0; i<__min(m_pRes->RecordCount(),saveSize); i++ )
			{
				int nState = m_pRes->Fields("state");
				state[i] = nState;
				m_pRes->MoveNext();
			}
			size = i;
			m_pRes->Close();
			return true;
		}
	}
	m_pRes->Close();
	return false;
}

BOOL CDataInspectManage::ReadAwardEx( UINT nAccountID, int nType, const char* szServerName, VECAWARD& vecAward)
{
	vecAward.clear();

	m_strSQL.Format( "select * from sk_generalize where account_id=%d and server_name='%s' and type>%d and type<%d", nAccountID, szServerName, AWARDEX_TYPE_BEGIN, AWARDEX_TYPE_END);
	m_strSQL.MakeUpper();
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
	{
		int nAmount = m_pRes->RecordCount();
		if( 0 == nAmount )
		{
			m_pRes->Close();
			return false;
		}
		else
		{
			for(int i = 0; i < nAmount; ++i)
			{
				GainAwardInfo info;
				info.nType = m_pRes->Fields("type");
				info.nValue = m_pRes->Fields("state");

				vecAward.push_back(info);

				m_pRes->MoveNext();
			}
			m_pRes->Close();
			
			return true;
		}
	}

	return false;
}

BOOL CDataInspectManage::WriteAwardeEx( UINT nAccountID, int nType, const char* szServerName, int nData)
{
	m_strSQL.Format( "select * from sk_generalize where account_id=%d and type=%d and server_name='%s' ", nAccountID, nType, szServerName);
	m_strSQL.MakeUpper();
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
	{
		if( 0 != m_pRes->RecordCount() )
		{
			int nState = m_pRes->Fields("state");
			/*nState += nData;*/
			if( nData > 0 && nState != nData)
			{
				m_pRes->Edit();
				m_pRes->Fields("state") = nData;
				m_pRes->Update();//�������ݿ�
				m_pRes->Close();
			}
			else
			{
				m_pRes->Close();
				m_strSQL.Format( "delete from sk_generalize where account_id=%d and type=%d and server_name='%s'", nAccountID, nType, szServerName);
				m_strSQL.MakeUpper();
				m_pRes->Open(m_strSQL);
				m_pRes->Close();
			}
			return true;
		}
	}
		
	return false;
}

int  CDataInspectManage::ManageGeneralize( UINT nAccountID, int nAction, int nType,int nData)///��ʼ��ser_loginlog���ݿ������,ֱ��д���ݡ�
{
	if ( 0 == nAction )
	{
		m_strSQL.Format( "select * from sk_generalize where account_id=%d and type=%d", nAccountID, nType);
		m_strSQL.MakeUpper();
		BOOL bReulst=m_pRes->Open(m_strSQL);
		if(bReulst)///ִ��SQL��ѯ
		{
			if( 0 == m_pRes->RecordCount() )
			{
				m_pRes->Close();
				return 1;
			}
			else
			{
				int nState = m_pRes->Fields("state");
				m_pRes->Close();
				return nState;
			}
		}
		else
			return 1;
	}
	else if (1 == nAction)
	{
		m_strSQL.Format( "select * from sk_generalize where account_id=%d and type=%d", nAccountID, nType);
		m_strSQL.MakeUpper();
		BOOL bReulst=m_pRes->Open(m_strSQL);
		if(bReulst)///ִ��SQL��ѯ
		{
			if( 0 == m_pRes->RecordCount() )
			{
				m_pRes->Close();
				return -1;
			}
			else
			{
				int nState = m_pRes->Fields("state");
				if( nState == nData )
				{
					m_pRes->Close();
					return -1;
				}

				m_pRes->Edit();
				m_pRes->Fields("state") = nData;
				m_pRes->Update();//�������ݿ�
				m_pRes->Close();
				return nData;
			}
		}
		else
			return -1;
	}
	return -1;

}
int CDataInspectManage::setMobChargeAward(UINT nAccountID,int nType,int nData)
{
	m_strSQL.Format( "select * from sk_generalize where account_id=%d and type=%d", nAccountID, nType);
	m_strSQL.MakeUpper();
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
	{
		if( 0 == m_pRes->RecordCount() )
		{
			m_pRes->Close();
			return -1;
		}
		else
		{
			int nState = m_pRes->Fields("state");
			if( nState==nData )
			{
				m_pRes->Close();
				return -1;
			}
			
			m_pRes->Edit();
			m_pRes->Fields("state") = nData;
			m_pRes->Update();//�������ݿ�
			m_pRes->Close();
			return nData;
		}
	}
	else
		return -1;
	
}

int CDataInspectManage::GetMobChargeAward(UINT nAccountID,int nType,int nData)
{		
		m_strSQL.Format( "select * from sk_generalize where account_id=%d and type=%d", nAccountID, nType);
		m_strSQL.MakeUpper();
		BOOL bReulst=m_pRes->Open(m_strSQL);
		if(bReulst)///ִ��SQL��ѯ
		{
			if( 0 == m_pRes->RecordCount() )
			{
				m_pRes->Close();
				return 0;
			}
			else
			{
				int nState = m_pRes->Fields("state");
				m_pRes->Close();
				return nState;
			}
		}
		else
			return 0;
}

int  CDataInspectManage::ManageTopListAward( UINT nAccountID, int nAction, int nType,int nData)///��ʼ��ser_loginlog���ݿ������,ֱ��д���ݡ�
{
	if ( 0 == nAction )
	{
		m_strSQL.Format( "select * from sk_generalize where account_id=%d and type=%d", nAccountID, nType);
		m_strSQL.MakeUpper();
		BOOL bReulst=m_pRes->Open(m_strSQL);
		if(bReulst)///ִ��SQL��ѯ
		{
			if( 0 == m_pRes->RecordCount() )
			{
				m_pRes->Close();
				return 0;
			}
			else
			{
				int nState = m_pRes->Fields("state");
				m_pRes->Close();
				return nState;
			}
		}
		else
			return 0;
	}
	else if (1 == nAction)
	{
		m_strSQL.Format( "select * from sk_generalize where account_id=%d and type=%d", nAccountID, nType);
		m_strSQL.MakeUpper();
		BOOL bReulst=m_pRes->Open(m_strSQL);
		if(bReulst)///ִ��SQL��ѯ
		{
			if( 0 == m_pRes->RecordCount() )
			{
				m_pRes->Close();
				return -1;
			}
			else
			{
				int nState = m_pRes->Fields("state");
				if( nState == nData )
				{
					m_pRes->Close();
					return -1;
				}
				
				m_pRes->Edit();
				m_pRes->Fields("state") = nData;
				m_pRes->Update();//�������ݿ�
				m_pRes->Close();
				return nData;
			}
		}
		else
			return -1;
	}
	return -1;
}

BOOL  CDataInspectManage::ActiveAccountByGMtool( UINT nAccountID )///ͨ��GM�����
{
	m_strSQL.Format( "select * from account where id=%d", nAccountID );
	m_strSQL.MakeUpper();///��SQLͳͳ��д����Ϊ��¼��
	
	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
	{
		m_pRes->Edit();
		m_pRes->Fields("accountstatus") = 0;//���״̬3
		m_pRes->Update();//�������ݿ�
		m_pRes->Close();
		return true;
	}
	m_pRes->Close();
	return false;
}

BOOL  CDataInspectManage::StopAccountByGMtool( UINT nAccountID )///ͨ��GM�����
{
	m_strSQL.Format( "select * from account where id=%d", nAccountID );
	m_strSQL.MakeUpper();///��SQLͳͳ��д����Ϊ��¼��

	BOOL bReulst=m_pRes->Open(m_strSQL);
	if(bReulst)///ִ��SQL��ѯ
	{
		m_pRes->Edit();
		m_pRes->Fields("accountstatus") = 3;//���״̬3
		m_pRes->Update();//�������ݿ�
		m_pRes->Close();
		return true;
	}
	m_pRes->Close();
	return false;
}

USHORT CDataInspectManage::GetAccountStatus( UINT nAccountID )
{
	m_strSQL.Format( "select * from account where id=%d", nAccountID );
	m_strSQL.MakeUpper();///��SQLͳͳ��д����Ϊ��¼��

	BOOL bReulst=m_pRes->Open(m_strSQL);

	if(bReulst)///ִ��SQL��ѯ
	{
		USHORT uAccountStatus=m_pRes->Fields("accountstatus");//���״̬
		m_pRes->Close();
		return uAccountStatus;
	}
	m_pRes->Close();
	return 0;
}

BOOL CDataInspectManage::WatchToolInfo(OBJID idAccount , const char * pGameServer,const short type)
{
	BOOL bResult = false;
	if (!type) 
	{
		return bResult;
	}
	UINT logindata=0; 
	CString thisTime=COleDateTime::GetCurrentTime().Format("%Y-%m-%d");

	if(m_pRes->IsOpen())
		m_pRes->Close();

	m_strSQL.Format("SELECT * from sk_tool_account where server_name='%s' and type = %d",pGameServer,type);
	m_strSQL.MakeUpper();///��SQLͳͳ��д����Ϊ��¼��
	bResult=m_pRes->Open(m_strSQL);	

	if(bResult)///ִ��SQL��ѯ
	{
		if( 0 == m_pRes->RecordCount() )
		{
			logindata = 1;
			m_strSQL.Format("INSERT sk_tool_account set server_name='%s',data=%d,type=%d,writetime ='%s'",pGameServer,logindata,type,thisTime);///����SQL���
			m_strSQL.MakeUpper();
			bResult=g_db.ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
			m_pRes->Close();
		}
		else
		{
			logindata=m_pRes->Fields("data");
			m_pRes->Edit();
			m_pRes->Fields("data") = ((long)logindata+1);
			strcpy( m_pRes->Fields("writetime"), thisTime );
			m_pRes->Update();//�������ݿ�
			m_pRes->Close();
		}
		return bResult;
	}
	m_pRes->Close();
	return bResult;
}
