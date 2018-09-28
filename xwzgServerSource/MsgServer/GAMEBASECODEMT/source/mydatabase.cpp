
#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "mydb.h"
#include "basefunc.h"
#include "myrecordset.h"
#include "myrecord.h"
#include "IniFile.h"
#include "../MsgServer/share/define.h"
//#ifdef	MULTITHREAD_SAFE
//#define	LOCKADD_TIMESUM
//#else
extern	long	s_nDatabaseTimeSum;			//?? ����ͳ�����ݿ���������ĵ�ʱ��
#define	LOCKADD_TIMESUM		::InterlockedExchangeAdd(&s_nDatabaseTimeSum, clock() - tStart)
//#endif

//-------------------------------------
// static 
CMyHeap	CMyDatabase::s_heap("CMyDatabase");

long	g_DBDebug[5];

// gloable..
//CMyDatabase		g_db;   06.11.16����ע��

#define		RELEASE_MUTEX(x)		::ReleaseMutex(x)



//////////////////////////////////////////////////////////////////////
IRecordset* CMyDatabase::CreateNewRecordset(LPCTSTR szSQL, bool bNoEmpty /*= true*/) 
{ 
	CMyRecordset* pRes = (CMyRecordset::CreateNew(this)); 
	if(pRes)
	{
		///softworms-������Ϸ�����������ݿ�Ͽ����Ӻ��Զ�����-2007-10-31
		mysql_options(m_hdbc, MYSQL_OPT_RECONNECT,"1");	


		///����ڴ�й¶
		IncreaseRef((UINT)pRes,szSQL);

		if(pRes->Open(szSQL))
		{
			if(pRes->RecordCount())
			{
				return (IRecordset*)pRes;
			}
			else if(!bNoEmpty && pRes->GetDefauldField())
			{
				return (IRecordset*)pRes;
			}
		}
	
		delete pRes;

// 		DecreaseRef((UINT)pRes);///������䲢û��������ֱ��ɾ��֮
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
CMyDatabase::CMyDatabase()
{	
	m_nMySQlErrorCode = 0;
	m_hdbc = NULL;
	m_bIsOpen = FALSE;
	m_nRefCount = 0;
	m_szDBName[0]	= 0;	//paled debug
	m_nOpenCount=0;
}			//�� δ��ʼ�� m_szDBName��m_bIsConnect��m_hstmt

CMyDatabase::~CMyDatabase()
{	
	//[��;���� 2009.07.17]�ڴ�й©
	Destroy();
}

///////////////////////////////////////////////////////////////////////
bool CMyDatabase::CheckSQL(const char* pszSQL)
{
	if (!pszSQL)
		return false;

	if (strstr(pszSQL, "UPDATE"))	// update sql
	{
		if (!strstr(pszSQL, "WHERE") || !strstr(pszSQL, "LIMIT"))
		{	// invalid sql
			::LogSave("Invalid update SQL [%s], since not found WHERE or LIMIT.", pszSQL);
			return false;
		}
	}

	if (strstr(pszSQL, "DELETE"))	// delete sql
	{
		if (!strstr(pszSQL, "WHERE"))
		{	// invalid sql
			::LogSave("Invalid SQL delete [%s].", pszSQL);
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMyDatabase::Open(char * szHost, char * szUser, char * szPasswd,char * szDB, 
			unsigned int uiPort, char * szSocket, unsigned int uiFlag )
{	
	if (m_bIsOpen)
	{	if (m_nRefCount == 0 )
		{
			LOGWARNING("DATABASE close at open");
			Close();
		}
		else 
		{	// message:please close all recordset
			g_nMyDBError = 1000;
			return FALSE;
		}
	}

	m_hdbc = mysql_init(NULL);
	if (m_hdbc == NULL) 
	{	g_nMyDBError = 1001;
		return FALSE;
	}

	char	DB_CHARSET		[DBSTR_SIZE] = "";
	char* pCharSet=NULL;
	CIniFile	ini(CONFIG_FILENAME, "Database");
	ini.GetString(DB_CHARSET,	"DB_CHARSET",	DBSTR_SIZE);
	pCharSet=strupr(DB_CHARSET);
	if (!strcmp(pCharSet,"CHT"))
		strcpy(DB_CHARSET,"big5");
	else if (!strcmp(pCharSet,"ENU"))
		strcpy(DB_CHARSET,"latin1");
	else
		strcpy(DB_CHARSET,"gbk");
	mysql_options(m_hdbc, MYSQL_SET_CHARSET_NAME,DB_CHARSET);	


	if (szDB != NULL) 
		strcpy(m_szDBName ,szDB);
	
	//zeng�����������
	mysql_options(m_hdbc, MYSQL_OPT_RECONNECT,"1");
	if ( mysql_real_connect(m_hdbc, szHost, szUser, szPasswd, 
			szDB, uiPort, szSocket , uiFlag) == NULL )
	{
		g_nMyDBError = 1002;
		
	//  [2/22/2008 ���]
		m_nMySQlErrorCode = mysql_errno(m_hdbc);

		return FALSE;
	}
	m_nMySQlErrorCode = 0;
	m_bIsOpen = TRUE;
	return TRUE;
}



//////////////////////////////////////////////////////////////////////
void CMyDatabase::Close()
{	
	if ( m_hdbc != NULL )	
		mysql_close( m_hdbc );
	m_bIsOpen = false;
	m_hdbc = NULL;

	//paled debug
	if(m_nRefCount)
	{
	//	::OutputDebugString("*DATABASE* warning: Found unclosed CRecordset.");
		LogSave("*DATABASE* warning: Found unclosed CRecordset.m_nRefCount:%d",m_nRefCount);
		DumpRef();
	}
}					//�� δ��� m_nRefCount

//////////////////////////////////////////////////////////////////////
bool CMyDatabase::ExecuteSQL(char * szSQL)		//, MYSQL_RES ** pRes
{	
	TimeCostChk("CMyDatabase::ExecuteSQL",szSQL);

//	if (strstr(szSQL, "SELECT"))	// delete sql
//	{
//		::MsgLogSave("gmlog/DbError", "CMyDatabase::ExecuteSQL:���Ե�select!%s",szSQL);
//		return false;
//	}
//
	if (!CMyDatabase::CheckSQL(szSQL))
		return false;

	try {
		if (m_hdbc == NULL) 
		{	
			g_nMyDBError = 1003;
			return false;
		}

		///softworms-������Ϸ�����������ݿ�Ͽ����Ӻ��Զ�����-2007-10-31
		InterlockedExchangeAdd(&g_DBDebug[4], 1);	//  [10/31/2007 ���]	��ѯ����
		// mysql_options(m_hdbc, MYSQL_OPT_RECONNECT,"1");	

		time_t	tStart = clock();

		time_t	tDBDebug0 = tStart;
		//20070523����
		DWORD dwWaitResult = ::WaitForSingleObject(m_hDbMutex, _MAX_DBACCESSTIME);
		//if (WAIT_OBJECT_0 ==::WaitForSingleObject(m_hDbMutex, _MAX_DBACCESSTIME))
		if( WAIT_OBJECT_0 == dwWaitResult)	
		{
			InterlockedExchangeAdd(&g_DBDebug[0], clock() - tDBDebug0);	//  [10/30/2007 ���]	�ȴ�ʱ��
			//20070521ashulanTest-----------
			char szLog[2048];
			sprintf(szLog,"CMyDatabase::ExecuteSQL:%s",szSQL);
			//CTimeCostChk chk("CMyDatabase::ExecuteSQL", 1000);
// 			CTimeCostChk chk(szLog, 20);
			//------------------------------
			
			time_t	tDBDebug1 = clock();
			if (mysql_query(m_hdbc, szSQL) != NULL) 
			{	
				InterlockedExchangeAdd(&g_DBDebug[1], clock() - tDBDebug1);	//  [10/30/2007 ���]	ִ��ʱ��
				LOCKADD_TIMESUM;

				

				///softworms2007-07-02��ǿ��־
				::MsgLogSave("gmlog/DbError", "CMyDatabase::ExecuteSQL:%s-%s-%d", szSQL,mysql_error(m_hdbc),mysql_errno(m_hdbc));
				RELEASE_MUTEX(m_hDbMutex);
				g_nMyDBError = 1004;	
				
				if(mysql_errno(m_hdbc)==2013)//����Lost connection to MySQL server during query
				{
					LOGWARNING("DATABASE mysql_errno(m_hdbc)==2013 1 SQL = %s",szSQL);
					while(!ErrorRecon());
					
				}

				// log error sql
//				::MsgLogSave("gmlog/DbError", "%s", szSQL);
				return false;
			}
//			else if(NULL == pRes)
			else
			{
				InterlockedExchangeAdd(&g_DBDebug[1], clock() - tDBDebug1);	//  [10/30/2007 ���]	ִ��ʱ��

				clock_t tDBDebug2 = clock();

				///softworms���ExecuteSQL���ؼ�¼��,�ڴ��ͷŵ�.
				{
					MYSQL_RES * pRes=mysql_store_result(m_hdbc);
					if (pRes) 
					{
						mysql_free_result(pRes);	
						::MsgLogSave("gmlog/DbError", "CMyDatabase::ExecuteSQL-%s:���ڼ�¼��,�ڴ��ͷŵ�,debug!",szSQL);
					}
				}
				
				InterlockedExchangeAdd(&g_DBDebug[2], clock() - tDBDebug2);	//  [10/30/2007 ���]	ȡ�����ʱ
				LOCKADD_TIMESUM;
				RELEASE_MUTEX(m_hDbMutex);
				return true;
			}
/*			else 
			{	
				LOCKADD_TIMESUM;
				MYSQL_RES * res=mysql_store_result(m_hdbc);

				if (res == NULL)
				{	if (mysql_field_count(m_hdbc) >0 )
					{	g_nMyDBError = 1005;
						RELEASE_MUTEX(m_hDbMutex);
						return false;
					}
					else 
					{
						*pRes = NULL;
						RELEASE_MUTEX(m_hDbMutex);
						return true;
					}
				}
				else 
				{	*pRes = res;
					RELEASE_MUTEX(m_hDbMutex);
					return true;
				}
			}*/
		}
		else
		{
			//::LogSave("*DATABASE* WARNING: ExecuteSQL() overtime");
			//20070523ashulanTest-----------
			char szLog[2048];
			sprintf(szLog,"*DATABASE* WARNING: ExecuteSQL() overtime(%d):%s",
				dwWaitResult,szSQL);
			::LogSave(szLog);
			//------------------------------
			return false;
		}
	}
	catch (...)
	{
		RELEASE_MUTEX(m_hDbMutex);

		::LogSave("MyDataBase Exe Sql error: %s", szSQL);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
int	CMyDatabase::CountRecord (const char* szSQL)
{
	if (!szSQL)
		return -1;

	TimeCostChk("CMyDatabase::CountRecord",szSQL);

	if (NULL == strstr(szSQL, "SELECT"))
	{
		::LogSave("Error: invalid count record Sql: %s", szSQL);
		return -1;
	}

	if (m_hdbc == NULL) 
	{	
		g_nMyDBError = 1003;
		return -1;
	}

	
	//20070523����
	DWORD dwWaitResult = ::WaitForSingleObject(m_hDbMutex, _MAX_DBACCESSTIME);
	//if (WAIT_OBJECT_0 ==::WaitForSingleObject(m_hDbMutex, _MAX_DBACCESSTIME))
	if( WAIT_OBJECT_0 == dwWaitResult)	
	{
		
		//20070521ashulanTest-----------
		char szLog[2048];
		sprintf(szLog,"CMyDatabase::CountRecord:%s",szSQL);
		//CTimeCostChk chk("CMyDatabase::CountRecord", 1000);
// 		CTimeCostChk chk(szLog, 20);
		//------------------------------

		if (mysql_query(m_hdbc, szSQL) != NULL) 
		{	
			g_nMyDBError = 1004;				

			///softworms2007-07-02��ǿ��־
			::MsgLogSave("gmlog/DbError", "CMyDatabase::CountRecord Error:%s-%s-%d", szSQL,mysql_error(m_hdbc),mysql_errno(m_hdbc));

			// log error sql
//			::MsgLogSave("gmlog/DbError", "%s", szSQL);
			return -1;
		}

		MYSQL_RES* res = mysql_store_result(m_hdbc);
		RELEASE_MUTEX(m_hDbMutex);

		if (res == NULL)
			return -1;
		else
		{
			int rval = mysql_num_rows(res);
			mysql_free_result(res);
			return rval;
		}
	}
	else
	{
		//::LogSave("*DATABASE* WARNING: ExecuteSQL() overtime");
		//20070523ashulanTest-----------
		char szLog[2048];
		sprintf(szLog,"*DATABASE* WARNING: ExecuteSQL() overtime(%d):%s",
			dwWaitResult,szSQL);
		::LogSave(szLog);
		//------------------------------
		return -1;
	}
}


//////////////////////////////////////////////////////////////////////
int CMyDatabase::GetInsertId()
{	
	if (m_hdbc)
		return (int)mysql_insert_id(m_hdbc);
	else
		return 0;
}

/*/////////////////////////////////////////////////////////////////////
bool CMyDatabase::EnumDatabases( ENUMPROC lpEnumFunc)
{	
	MYSQL_RES * res;
	MYSQL_ROW	row;
	char * szSQL = (char *) &SQL_STMT_SHOWDATABASES[0];
	if (ExecuteSQL(szSQL, &res))
	{	for (int i=0;i<mysql_num_rows(res);i++)
		{	row=mysql_fetch_row(res) ;
			if (!row || !row[0] || !lpEnumFunc(row[0])) break;
		}
		mysql_free_result(res);
		return true;
	}
	else return false;
}*/

/*/////////////////////////////////////////////////////////////////////
bool CMyDatabase::EnumTables( ENUMPROC lpEnumFunc)
{	
	MYSQL_RES * res;
	MYSQL_ROW	row;
	char * szSQL = (char *) &SQL_STMT_SHOWTABLES[0];
	if (ExecuteSQL(szSQL, &res))
	{	for (int i=0;i<mysql_num_rows(res);i++)
		{	row=mysql_fetch_row(res) ;
			if (!row || !row[0] || !lpEnumFunc(row[0])) break;
		}
		mysql_free_result(res);
		return true;
	}
	else return false;
}*/


//////////////////////////////////////////////////////////////////////
void CMyDatabase::GetDBErrorMsg(char * szErrorMsg)
{	
	strcpy(szErrorMsg, mysql_error(m_hdbc));
}


//////////////////////////////////////////////////////////////////////
// globle init function
//////////////////////////////////////////////////////////////////////
//------------------------06.10.17����ע�Ͳ��޸�-----begin------------------------------------
//DWORD g_dwServerID	=1;
//DWORD g_dwMaxPlayer =800;
DWORD g_dwServerID	=4;
DWORD g_dwMaxPlayer =3000;
//-----------------------------------------------------end---------------------------------------
BOOL CMyDatabase::Init(const char* pszDBServer, const char* pszDBUser, const char* pszPassword, const char* pszDBName)
{
	// init db
	char szDBServer[256], szDBName[256], szDBUser[256], szPassword[256];

	if (pszDBServer)
		strcpy(szDBServer, pszDBServer);
	else
		strcpy(szDBServer,	"127.0.0.1");

	if (pszDBName)
		strcpy(szDBName, pszDBName);
	else
		strcpy(szDBName,	"sk");

	if (pszDBUser)
		strcpy(szDBUser, pszDBUser);
	else
		strcpy(szDBUser,	"wyb");

	if (pszPassword)
		strcpy(szPassword, pszPassword);
	else
		strcpy(szPassword,	"11password22");
//06.10.17����ע��

/*	// for local debug
	{
		FILE* fp	=fopen("gameserver.cfg", "r");
		if (fp)
		{
			fscanf(fp, "%u %u %s %s %s %s\n", &g_dwMaxPlayer, &g_dwServerID, szDBServer, szDBUser, szPassword, szDBName);
			fclose(fp);
		}
	}*/

	if(Open(szDBServer, szDBUser, szPassword, szDBName))
	{
		m_hDbMutex	=::CreateMutex(NULL, false, NULL);			// ���������� "GameDBMutex"
//@@@		m_hDbMutex	=::CreateMutex(NULL, false, "GameDBMutex");			// ���������� "GameDBMutex"
		if (m_hDbMutex)
			return true;
		else
		{
			::MessageBox(NULL, "m_hDbMutex����ʧ�ܡ�", "Error",MB_OK|MB_ICONERROR);
			return false;
		}
	}
	else
	{
		LOGWARNING("���ݿ����openʧ�ܡ�");
		//::MessageBox(NULL, "���ݿ����openʧ�ܡ�", "Error",MB_OK|MB_ICONERROR);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
void CMyDatabase::Destroy	(void)
{
	if (m_hDbMutex)
	{
		::CloseHandle(m_hDbMutex);
		m_hDbMutex	=NULL;
	}
	
	LOGWARNING("DATABASE close at Destroy");
	Close();
}

//////////////////////////////////////////////////////////////////////
IDatabase* CreateDatabase(const char* szDBServer /*= NULL*/, const char* szLoginName /*= NULL*/, const char* szPassword /*= NULL*/, const char* szDBName /*= NULL*/)
{
	CMyDatabase* pDb = CMyDatabase::CreateNew();
	if(!pDb)
		return NULL;;


	if(pDb->Init(szDBServer, szLoginName, szPassword, szDBName))
		return pDb->GetInterface();

	delete pDb;
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
//  [1/29/2008 ���]	����
BOOL ReconnectDatabase(IDatabase* pDb, const char* szDBServer /*= NULL*/, const char* szLoginName /*= NULL*/, const char* szPassword /*= NULL*/, const char* szDBName /*= NULL*/)
{
	if(!pDb)
		return NULL;

	if(((CMyDatabase*)pDb)->Reconnect(szDBServer, szLoginName, szPassword, szDBName))
		return TRUE;

//	delete ((CMyDatabase*)pDb);
//	delete pDb;
	return FALSE;
}

bool CMyDatabase::ErrorRecon()
{
	char	DB_IP	[DBSTR_SIZE] = "";
	char	DB_USER	[DBSTR_SIZE] = "";
	char	DB_PW	[DBSTR_SIZE] = "";
	char	DB_DB	[DBSTR_SIZE] = "";
	
	CIniFile	ini(CONFIG_FILENAME, "Database");
	ini.GetString(DB_IP,	"DB_IP",	DBSTR_SIZE);
	ini.GetString(DB_USER,	"DB_USER",	DBSTR_SIZE);
	ini.GetString(DB_PW,	"DB_PW",	DBSTR_SIZE);
	ini.GetString(DB_DB,	"DB_DB",	DBSTR_SIZE);
	//	m_pDb	= ::CreateDatabase(DB_IP, DB_USER, DB_PW, DB_DB);
	BOOL bSuccess	= ::ReconnectDatabase(this, DB_IP, DB_USER, DB_PW, DB_DB);
	
	
	if(!bSuccess)
	{
		LOGERROR("���ݿ���������ʧ��!");
		return false;
	}
	
	return true;
}

BOOL CMyDatabase::Reconnect(const char* pszDBServer, const char* pszDBUser, const char* pszPassword, const char* pszDBName)
{
	// init db
	char szDBServer[256], szDBName[256], szDBUser[256], szPassword[256];

	if (pszDBServer)
		strcpy(szDBServer, pszDBServer);
	else
		strcpy(szDBServer,	"127.0.0.1");

	if (pszDBName)
		strcpy(szDBName, pszDBName);
	else
		strcpy(szDBName,	"sk");

	if (pszDBUser)
		strcpy(szDBUser, pszDBUser);
	else
		strcpy(szDBUser,	"wyb");

	if (pszPassword)
		strcpy(szPassword, pszPassword);
	else
		strcpy(szPassword,	"11password22");
//06.10.17����ע��

/*	// for local debug
	{
		FILE* fp	=fopen("gameserver.cfg", "r");
		if (fp)
		{
			fscanf(fp, "%u %u %s %s %s %s\n", &g_dwMaxPlayer, &g_dwServerID, szDBServer, szDBUser, szPassword, szDBName);
			fclose(fp);
		}
	}*/

	LOGWARNING("DATABASE close at Reconnect");
	Close();

	if(Open(szDBServer, szDBUser, szPassword, szDBName))
	{
		m_hDbMutex	=::CreateMutex(NULL, false, NULL);			// ���������� "GameDBMutex"
//@@@		m_hDbMutex	=::CreateMutex(NULL, false, "GameDBMutex");			// ���������� "GameDBMutex"
		if (m_hDbMutex)
			return true;
		else
		{
			//  [2/22/2008 ���]	����ʧ�ܺ󲻵�����ʾ��
	//		::MessageBox(NULL, "m_hDbMutex����ʧ�ܡ�", "Error",MB_OK|MB_ICONERROR);
			LOGERROR("CMyDatabase::Reconnect:m_hDbMutex����ʧ�ܡ�");
			return false;
		}
	}
	else
	{
	//	::MessageBox(NULL, "���ݿ����openʧ�ܡ�", "Error",MB_OK|MB_ICONERROR);
		LOGERROR("CMyDatabase::Reconnect: ���ݿ����openʧ��! m_nMySQlErrorCode=%d, g_nMyDBError=%d", m_nMySQlErrorCode, g_nMyDBError);
		return false;
	}
}