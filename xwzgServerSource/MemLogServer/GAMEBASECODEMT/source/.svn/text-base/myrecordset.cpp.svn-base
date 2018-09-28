// MyRecordset.cpp: implementation of the CMyRecordset class.


extern	long	s_nDatabaseTimeSum;			//?? 用于统计数据库操作所消耗的时间
#define	LOCKADD_TIMESUM		::InterlockedExchangeAdd(&s_nDatabaseTimeSum, clock() - tStart)


#ifdef	MULTITHREAD_SAFE
	#include <afxmt.h>
	#define	LOCKOBJ		CSingleLock xLock(&m_xCtrl, true)
	#include <winsock2.h>
#else
	#define	LOCKOBJ		
#endif

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "mydb.h"
#include "basefunc.h"
#include "common.h"

#define		RELEASE_MUTEX(x)		::ReleaseMutex(x)

// static init
CMyHeap	CMyRecordset::s_heapString("CMyRecordset::s_heapString");
#ifdef	MULTITHREAD_SAFE
	CCriticalSection	CMyRecordset::m_xCtrl;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 构建 UpdateSQLs
MYHEAP_IMPLEMENTATION(CMyRecordset,s_heap)
///////////////////////////////////////////////////////////////////////
void CMyRecordset::BuildUpdateCondition()
{
	switch ( m_nEditMode )
	{	
	case modeEdit:
		sprintf(m_szUpdateCondition, "%s=%s", m_szKey, m_Row[m_uiKeyIndex]);
		break;
	case modeAddNew:
	case modeDelete:
		sprintf(m_szUpdateCondition, "%s=%u", m_szKey, m_objFields[m_uiKeyIndex].m_ulVal);
		break;
	}
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::BuildUpdateOpration()
{
	char szFormat[1024];

	m_szUpdateOpration[0]='\0';

	BOOL bFirst = true;
	BOOL bFlag = true;
	
	switch ( m_nEditMode )
	{	
	case modeEdit:
	case modeAddNew:
		for (UINT i=0; i<m_uiFieldsCount; i++ )
		{	
			if ( m_pbDirty[i] )
			{
				switch (m_objFields[i].m_ulType)
				{
				case FIELD_TYPE_STRING:
				case FIELD_TYPE_VAR_STRING:
					if ((m_Fields[i].flags&NOT_NULL_FLAG)  && strlen(m_objFields[i].m_szVal) == NULL) 
						bFlag = false;
					else 
						sprintf(szFormat, "='%s'",m_objFields[i].m_szVal);
					break;

				case FIELD_TYPE_TINY:
				case FIELD_TYPE_SHORT:
					sprintf(szFormat, "=%d",m_objFields[i].m_iVal);
					break;

				case FIELD_TYPE_FLOAT:
					sprintf(szFormat, "=%.2f",m_objFields[i].m_fVal);
					break;

				case FIELD_TYPE_DOUBLE:
					sprintf(szFormat, "=%.2f",m_objFields[i].m_dVal);
					break;

				case FIELD_TYPE_LONG:
					if ((m_Fields[i].flags&UNSIGNED_FLAG) != 0)
						sprintf(szFormat, "=%u", m_objFields[i].m_ulVal);
					else
						sprintf(szFormat, "=%ld", m_objFields[i].m_lVal);
					break;

				default:
					::LogSave("Error: unknow type in CMyRecordset::BuildUpdateOpration()");
					break;
				}

				if (bFlag)
				{	
					if (!bFirst)
						strcat(m_szUpdateOpration,",");
					else 
						bFirst = false;

					strcat(m_szUpdateOpration, m_Fields[i].name);
					strcat(m_szUpdateOpration, szFormat);
					if(strlen(m_szUpdateOpration) >= 1024)
						::LogSave("ERROR: BuildUpdateOpration() 溢出!!!");
				}
				else
					bFlag	=true;

			} // if ( m_pbDirty[i] )
		} // for ( i=0; i<m_uiFieldsCount; i++ )
		break;
	}
}


///////////////////////////////////////////////////////////////////////
void CMyRecordset::BuildUpdateSQL()
{
	if (!m_bCanUpdate) 
		return;

	BuildUpdateOpration();
	BuildUpdateCondition();

	char * psz;

	switch (m_nEditMode)
	{
	case modeDelete:
		psz = (char * )SQL_STMT_DELETE;
		break;

	case modeEdit:
		psz = (char * )SQL_STMT_UPDATE;
		break;

	case modeAddNew:
		psz = (char * )SQL_STMT_INSERT;
		break;
	}

	sprintf(m_szUpdateSQL,psz,m_szTableName,			//paled - SQL_STMT_DELETE的第2个参数为空串
		m_szUpdateOpration,m_szUpdateCondition);		//paled - SQL_STMT_INSERT只有两个参数，将丢弃第3个参数
	if(strlen(m_szUpdateSQL) >= 1024)
		::LogSave("ERROR: BuildUpdateSQL() 溢出!!!");
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::GetAllField()		//paled - 将ROW中的串转换为数，并填入objField的VAL联合中。
{
	if(m_Row)
	{
		for(int i = 0; i < FieldsCount(); i++)
		{
			if ( m_ulRowsCount ==0) return;
			if (m_Row[i] == 0)
			{	m_objFields[i].m_lVal = 0;
				return;
			}
			switch (m_objFields[i].m_ulType)
			{	
			case	FIELD_TYPE_TINY:
			case	FIELD_TYPE_SHORT:
				m_objFields[i].m_iVal = atoi(m_Row[i]);
				break;
			case	FIELD_TYPE_LONG:
				m_objFields[i].m_lVal = atol(m_Row[i]);
				break;
			case	FIELD_TYPE_FLOAT:
				m_objFields[i].m_fVal = atof(m_Row[i]);
				break;
			case	FIELD_TYPE_DOUBLE:
				m_objFields[i].m_dVal = atof(m_Row[i]);
				break;
			case	FIELD_TYPE_LONGLONG:
				m_objFields[i].m_i64Val = _atoi64(m_Row[i]);
				break;
			case	FIELD_TYPE_DECIMAL:
			case	FIELD_TYPE_NEWDECIMAL:
				m_objFields[i].m_iVal = atoi(m_Row[i]);
				break;
			case	FIELD_TYPE_STRING:
			case	FIELD_TYPE_VAR_STRING:
				strcpy(m_objFields[i].m_szVal,m_Row[i]);
				break;
			}
		}
	}

	ClsEditFlag();
}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::GetDefauldField()		//取缺省域值，并填入objField的VAL联合中。
{
	MYSQL_RES *pRes = mysql_list_fields(m_pMyDatabase->m_hdbc, m_szTableName, NULL);
	if(!pRes)
		return false;

	for(int i = 0; i < FieldsCount(); i++)
	{
		MYSQL_FIELD * pField = mysql_fetch_field(pRes);
		if(!pField)
		{
			ASSERT(!"GetDefauldField()");
			mysql_free_result(pRes);
			return false;
		}

		switch (m_objFields[i].m_ulType)
		{
		case	FIELD_TYPE_TINY:
		case	FIELD_TYPE_SHORT:
			if(pField->def)
				m_objFields[i].m_iVal = atoi(pField->def);
			else
				m_objFields[i].m_iVal = 0;
			break;
		case	FIELD_TYPE_LONG:
			if(pField->def)
				m_objFields[i].m_lVal = atol(pField->def);
			else
				m_objFields[i].m_lVal = 0;
			break;
		case	FIELD_TYPE_FLOAT:
			if(pField->def)
				m_objFields[i].m_fVal = atof(pField->def);
			else
				m_objFields[i].m_fVal = 0;
			break;
		case	FIELD_TYPE_DOUBLE:
			if(pField->def)
				m_objFields[i].m_dVal = atof(pField->def);
			else
				m_objFields[i].m_dVal = 0;			
			break;
		case	FIELD_TYPE_LONGLONG:
			if(pField->def)
				m_objFields[i].m_i64Val = _atoi64(pField->def);
			else
				m_objFields[i].m_i64Val = 0;			
			break;
		case	FIELD_TYPE_DECIMAL:
		case	FIELD_TYPE_NEWDECIMAL:
			if(pField->def)
			{
				m_objFields[i].m_dVal = atof(pField->def);
			}
			else
			{
				m_objFields[i].m_dVal = 0;
			}
			break;
		case	FIELD_TYPE_STRING:
		case	FIELD_TYPE_VAR_STRING:
			if(pField->def)
				strcpy(m_objFields[i].m_szVal, pField->def);
			else
				strcpy(m_objFields[i].m_szVal, "");
			break;
		}
	}

	mysql_free_result(pRes);
	ClsEditFlag();
	m_bDefaultValue	= true;
	return true;
}	

// 基本操作
///////////////////////////////////////////////////////////////////////
CMyRecordset::CMyRecordset(CMyDatabase  * pMyDatabase )
{
	m_bCanUpdate=false;
	m_nEditMode=0;
	m_bEdit=false;
	m_Fields=NULL;
	m_objFields=NULL;
	m_pbDirty=NULL;
	m_pMyDatabase=NULL;
	strcpy(m_szKey,"");
	strcpy(m_szTableName,"");
	strcpy(m_szUpdateCondition,"");
	strcpy(m_szUpdateOpration,"");
	strcpy(m_szUpdateSQL,"");
	strcpy(m_szSQL,"");
	m_ulRowsCount=0;
	m_ulCursor=0;
	m_uiFieldsCount=0;
	m_uiKeyIndex=0;
	m_Recordset=NULL;
	m_Row=NULL;


	m_bDefaultValue=false;
	m_bEOF=false;
	m_bIsOpen = false;

	m_bEdit	  = true;

	m_pMyDatabase = pMyDatabase;
	if (pMyDatabase == NULL ) 
		return;

	m_Recordset = NULL;
	
	if (m_pMyDatabase != NULL && m_pMyDatabase->IsOpen())
	{	
		m_pMyDatabase->m_nRefCount++;
	}
}

///////////////////////////////////////////////////////////////////////
CMyRecordset* CMyRecordset::CreateNew(CMyDatabase  * pMyDatabase)
{
	if (pMyDatabase == NULL ||
			!pMyDatabase->IsOpen())
		return NULL;

	return (new CMyRecordset(pMyDatabase));
}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::Open(const char * szSQL)
{	
	TimeCostChk("CMyRecordset::Open",szSQL);
	int nstep = 0;
	try 
	{
		time_t	tStart = clock();

		if(!m_pMyDatabase)
			return false;
		if (!m_pMyDatabase->IsOpen())  
			return false;

		MYSQL * hdbc = m_pMyDatabase->m_hdbc;

		int nLen = strlen(szSQL);
		char * p,* sz= strstr((char*)szSQL, "FROM");
		int nSize = 0;
		if(sz == NULL)
			sz= strstr((char*)szSQL, "from");

		strcpy(m_szSQL,szSQL);

		if (m_bIsOpen) 
		{	
			Close();
			g_nMyDBError = 2000;
			return false;
		}

		InterlockedExchangeAdd(&g_DBDebug[4], 1);	//查询次数

		clock_t tDBDebug0 = clock();
		DWORD dwWaitResult = ::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME);
		nstep = 1;
		if( WAIT_TIMEOUT == dwWaitResult)		
		{
			char szLog[2048];
			sprintf(szLog,"*DATABASE* WARNING: Open() overtime(%d):%s",
				dwWaitResult,szSQL);
			::LogSave(szLog);
			return false;
		}

		InterlockedExchangeAdd(&g_DBDebug[0], clock() - tDBDebug0);
		clock_t tDBDebug1 = clock();

		char szLog[2048];
		sprintf(szLog,"CMyRecordset::Open:%s",szSQL);

		if (mysql_query(hdbc, m_szSQL)!=NULL)
		{	
			InterlockedExchangeAdd(&g_DBDebug[1], clock() - tDBDebug1);	//执行时间
			RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
			g_nMyDBError = 2001;
			
			if(mysql_errno(hdbc)==2013)//处理Lost connection to MySQL server during query
			{
				while(!(m_pMyDatabase->ErrorRecon()));
				
			}
			return false;
		}
		else 
		{	
			InterlockedExchangeAdd(&g_DBDebug[1], clock() - tDBDebug1);	//执行时间
			clock_t tDBDebug2 = clock();
			m_Recordset=mysql_store_result(hdbc);//把所有的结果纪律一次性地全部保存在一个MYSQL_RES结果里
			InterlockedExchangeAdd(&g_DBDebug[2], clock() - tDBDebug2);	//取结果用时
			clock_t tDBDebug3 = clock();	
			RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
			if (m_Recordset == NULL)
			{	
				g_nMyDBError = 2002;
				return false;
			}
			else 
			{	
				//一些初始化
				m_bEOF = false;
				m_bIsOpen = true;
				m_bDefaultValue	= false;
				m_bCanUpdate = false;


				m_uiFieldsCount = mysql_num_fields(m_Recordset);//返回查询结果中的纪录总数
				m_ulRowsCount = mysql_num_rows(m_Recordset);//返回查询结果里的纪录总数
				m_Fields = mysql_fetch_field(m_Recordset);//返回一个MYSQL_FIELD结构：结果字段（数据列）的数据类型。
				
				m_objFields = new CMyField [m_uiFieldsCount];

				//m_pbDirty	= new bool [m_uiFieldsCount];
#ifdef	USE_NEW
				m_pbDirty	= (bool *)s_heapString.Alloc(sizeof(bool) * m_uiFieldsCount);
#else
				m_pbDirty	= (bool *)g_heapSystem.New(sizeof(bool) * m_uiFieldsCount);		//★ 全局堆分配，可能产生内存碎片。
#endif
				ZeroMemory(m_pbDirty, sizeof(bool) * m_uiFieldsCount);
				

				
				for (UINT i =0; i< m_uiFieldsCount; i++)
				{
					m_objFields[i].m_Field = m_Fields[i];
					m_objFields[i].m_ulType = m_Fields[i].type;

					if (m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING 
							|| m_objFields[i].m_ulType == FIELD_TYPE_STRING )

#ifdef	USE_NEW
						m_objFields[i].m_szVal	= (char *) s_heapString.Alloc(sizeof(char)*(m_Fields[i].length+1));
#else
						m_objFields[i].m_szVal = (char *) g_heapSystem.New(sizeof(char)*(m_Fields[i].length+1));
																		//★ 全局堆分配，会有内存碎片
#endif
					
					m_objFields[i].m_pMyField = &m_objFields[i];	//paled
					m_objFields[i].m_nIndex = i;
					if (!m_bCanUpdate && (
						m_Fields[i].flags & PRI_KEY_FLAG ||
						m_Fields[i].flags & AUTO_INCREMENT_FLAG ) )
					{	
						m_bCanUpdate = true;
						strcpy(m_szKey,m_Fields[i].name);
						m_uiKeyIndex = i;
					}
				}

				MoveFirst();
				if (sz !=NULL) 
				{	sz = strstr(sz+1," ");
					if ( sz !=NULL )  
					{	p=sz+1;
						sz = strstr(sz+1," ");
						if ( sz == NULL) nSize = nLen -(p-szSQL);
						else nSize = sz - p;
					}
				}
				strncpy(m_szTableName,p,nSize);
				m_szTableName[nSize]='\0';
			}

			InterlockedExchangeAdd(&g_DBDebug[3], clock() - tDBDebug3);	//  [10/30/2007 朱斌]	处理结果用时
		}
		LOCKADD_TIMESUM;
		return true;
	}
	catch(...)
	{
		if(nstep==1)
			RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
		::LogSave("Mysql RecordSet Open Exception Error.");
		return false;
	}
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::Close()
{	
	try 
	{
		if (m_bIsOpen)
		{
			
#ifdef	USE_NEW
			s_heapString.Free(m_pbDirty);
#else
			g_heapSystem.Delete(m_pbDirty);
#endif
			for (unsigned int i=0;i<m_uiFieldsCount;i++)
			{
				if (m_objFields[i].m_ulType == FIELD_TYPE_STRING 
					|| m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING)
#ifdef	USE_NEW
					s_heapString.Free(m_objFields[i].m_szVal);
#else
					g_heapSystem.Delete(m_objFields[i].m_szVal);
#endif
			}
			delete [] m_objFields;
			m_pMyDatabase->m_nRefCount --;
		}

		m_bIsOpen = false;
		m_ulRowsCount = 0;
		if (m_Recordset !=NULL)
			mysql_free_result(m_Recordset);
		m_Recordset = NULL;
	}
	catch(...)
	{
		::LogSave("Mysql RecordSet Close Exception Error.");
	}
}

///////////////////////////////////////////////////////////////////////
CMyRecordset::~CMyRecordset()
{
	if (m_bIsOpen) 
		Close();
}




///////////////////////////////////////////////////////////////////////
// SQL
bool CMyRecordset::Query(char * strSQL, MYSQL_RES * * res)
{
	TimeCostChk("CMyRecordset::Query",strSQL);

	if (!CMyDatabase::CheckSQL(strSQL))
		return false;

	try 
	{
		if (!m_bIsOpen) 
			return false;

		time_t	tStart = clock();

		DWORD dwWaitResult = ::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME);
		if( WAIT_OBJECT_0 == dwWaitResult)
		{
			char szLog[2048];
			sprintf(szLog,"CMyRecordset::Query:%s",strSQL);
			MYSQL * hdbc = m_pMyDatabase->m_hdbc;
			if (mysql_query(hdbc, strSQL)!=NULL)
			{	
				LOCKADD_TIMESUM;
				RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);

				if(mysql_errno(hdbc)==2013)//处理Lost connection to MySQL server during query
				{
					while(!(m_pMyDatabase->ErrorRecon()));
					
				}

				return false;
			}
			else 
			{	
				LOCKADD_TIMESUM;
				*res=mysql_store_result(hdbc);			//paled - 当仅有INSERT、DELETE、UPDATE等查询时，可使用 mysql_affected_rows() 检查是否成功
				if (*res == NULL)
				{	
					if (mysql_field_count(hdbc) >0 )
					{
						//error
						RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
						return false;
					}
					else 
					{
						RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
						return true;		
					}
				}
				else
				{
					RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
					return true;
				}
			}
		}
		else
		{
			char szLog[2048];
			sprintf(szLog,"*DATABASE* WARNING: Qurey() overtime(%d):%s",
				dwWaitResult,strSQL);
			::LogSave(szLog);
			return false;
		}
	}
	catch(...)
	{
		::LogSave("MyRecordSet Query Exception error: %s", strSQL);
		RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
		return false;
	}
}

// 基本属性
///////////////////////////////////////////////////////////////////////
bool CMyRecordset::IsEOF()
{	
	return m_bEOF;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::IsOpen()
{
	return m_bIsOpen;
}

///////////////////////////////////////////////////////////////////////
int CMyRecordset::FieldsCount()
{
	if (m_bIsOpen)
		return m_uiFieldsCount;
	else 
		return -1;

}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::CanUpdate() const     // Can Edit/AddNew/Delete be called?
{	
	if (!m_bIsOpen ) 
		return false;
	
	return m_bCanUpdate; 
}



///////////////////////////////////////////////////////////////////////
void CMyRecordset::ClsEditFlag()
{
	for ( int i=0; i<(int)m_uiFieldsCount; i++ )
		m_pbDirty[i] = false;
}

///////////////////////////////////////////////////////////////////////
int CMyRecordset::UpdateFieldsCount()
{
	int nCount	=0;
	for ( int i=0; i<(int)m_uiFieldsCount; i++ )
	{
		if (m_pbDirty[i])
			nCount++;
	}

	return nCount;
}


///////////////////////////////////////////////////////////////////////
// recordset
///////////////////////////////////////////////////////////////////////
UINT CMyRecordset::RecordCount()		// return -1: error
{
	if (!m_bIsOpen)  return -1;
	return (UINT)m_ulRowsCount;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::MoveNext()
{
	m_bEdit = true;
	if(! m_bIsOpen )
		return false;

	if ( m_ulCursor < m_ulRowsCount-1 )
	{
		m_ulCursor ++ ;
		m_Row=mysql_fetch_row( m_Recordset );
		GetAllField();

		return true;
	}
	else 
	{
		m_bEOF = true;
		return false;
	}

}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::MoveFirst()
{
	m_bEdit = true;	
	if (! m_bIsOpen ) 
		return;

	m_ulCursor = 0;
	mysql_data_seek(m_Recordset,0);
	m_Row=mysql_fetch_row( m_Recordset );
	GetAllField();
	m_bEOF = false;
}
///////////////////////////////////////////////////////////////////////
// record
///////////////////////////////////////////////////////////////////////
IRecord* CMyRecordset::CreateNewRecord(DWORD idNewKey /*= NEWKEY_NONE*/) 
{
	CMyRecord* pRecord = CMyRecord::CreateNew(m_pMyDatabase);		//VVVVVVVVVVVVVV
	if(pRecord)
	{
		if(pRecord->Open(this, idNewKey))
			return (IRecord*)pRecord;

		delete pRecord;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////
DWORD CMyRecordset::InsertRecord()
{
	if (!m_bIsOpen || !m_bCanUpdate)
		return false;

	m_nEditMode	= modeAddNew;
	bool ret	=true;

	if (this->UpdateFieldsCount() > 0)
	{
		// build sql
		this->BuildUpdateSQL();
		MYSQL_RES * res;
		ret = Query(m_szUpdateSQL, &res);
		if(ret && res)
			mysql_free_result(res);
	}

	// clear edit flag
	this->ClsEditFlag();

	if(ret)
		return m_pMyDatabase->GetInsertId();
	else
		return 0;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::UpdateRecord()
{
	if (!m_bIsOpen || !m_bCanUpdate)
		return false;

	m_nEditMode	= modeEdit;

	bool ret	=true;
	if (this->UpdateFieldsCount() > 0)
	{
		// build sql
		this->BuildUpdateSQL();
		MYSQL_RES * res;
		ret = Query(m_szUpdateSQL, &res);
		if(ret && res)
			mysql_free_result(res);
	}

	// clear edit flag
	this->ClsEditFlag();

	return	ret;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::DeleteRecord()
{
	if (!m_bIsOpen || !m_bCanUpdate)
		return false;

	
	m_nEditMode	= modeDelete;

	bool ret	=true;

	// build sql
	this->BuildUpdateSQL();
	MYSQL_RES * res;
	ret = Query(m_szUpdateSQL, &res);
	if(ret && res)
		mysql_free_result(res);

	this->ClsEditFlag();
	m_bEdit		= false;
	return	ret;
}

///////////////////////////////////////////////////////////////////////
// new interface
///////////////////////////////////////////////////////////////////////
int CMyRecordset::LoadInt		(LPCTSTR szField)
{
	IF_NOT(szField)
		return INT_NONE;

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
			return GetInt(i);
		}
	}

	return INT_NONE;
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::SetInt		(LPCTSTR szField, int nData)
{
	IF_NOT(szField)
		return;

	CHECK(m_bEdit);

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
			if(GetInt(i) != nData)
				SetInt(i, nData);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::LoadString	(char* szData, LPCTSTR szField, int nSize)	
{
	IF_NOT(szField && szData)
		return;

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
			SafeCopy(szData, GetStr(i), nSize);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::SetString	(LPCTSTR szField, LPCTSTR szData, int nSize)
{
	IF_NOT(szField && szData)
		return;

	CHECK(m_bEdit);

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
			if(stricmp(szData,GetStr(i)) != 0)
				SetStr(i, szData, nSize);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////
int CMyRecordset::GetInt		(int idx)							
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount) 
		return INT_NONE;

	if (!m_objFields[idx].m_szVal)			// 非串类型
		return m_objFields[idx].m_iVal;
	else 
		return INT_NONE; 
}

double CMyRecordset::GetDouble		( int idx )
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount) 
		return INT_NONE;

	if (!m_objFields[idx].m_szVal)			// 非串类型
		return m_objFields[idx].m_dVal;
	else 
		return DOUBLE_NONE; 
}

_int64   CMyRecordset::GetInt64	(int idx)
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount) 
	return INT_NONE;

	if (!m_objFields[idx].m_szVal)			// 非串类型
		return m_objFields[idx].m_i64Val;
	else 
		return INT64_NONE; 
}
///////////////////////////////////////////////////////////////////////
LPCTSTR CMyRecordset::GetStr		(int idx)							
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount) 
		return STR_NONE;

	if (m_objFields[idx].m_szVal)			// 串类型
		return (m_objFields[idx].m_szVal);
	else 
		return STR_NONE;
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::SetInt		(int idx, int nData)				
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount) 
		return;

	CHECK(m_bEdit);

	m_pbDirty[idx] = true;
	if (!m_objFields[idx].m_szVal)			// 非串类型
		m_objFields[idx].m_iVal = nData;

	return;
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::SetStr		(int idx, LPCTSTR szData, int nSize)
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount) 
		return;
	IF_NOT(nSize <= (int)m_Fields[idx].length+1)
		return;

	CHECK(m_bEdit);

	m_pbDirty[idx] = true;
	if (m_objFields[idx].m_szVal)			// 串类型
		SafeCopy(m_objFields[idx].m_szVal, szData, nSize);

	return;
}




