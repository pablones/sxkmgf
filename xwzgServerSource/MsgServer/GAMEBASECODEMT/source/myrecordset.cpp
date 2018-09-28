// MyRecordset.cpp: implementation of the CMyRecordset class.
//
//////////////////////////////////////////////////////////////////////

//#ifdef	MULTITHREAD_SAFE
//#define	LOCKADD_TIMESUM
//#else
extern	long	s_nDatabaseTimeSum;			//?? 用于统计数据库操作所消耗的时间
#define	LOCKADD_TIMESUM		::InterlockedExchangeAdd(&s_nDatabaseTimeSum, clock() - tStart)
//#endif

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
	if (!m_bCanUpdate) return;

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
bool CMyRecordset::Update()
{
	if (!m_bIsOpen) 
		return false;

	m_bEdit = false;

	bool ret	=true;
	if (this->UpdateFieldsCount() > 0)
	{
		// build sql
		this->BuildUpdateSQL();

		// need change the local row;
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
CMyField * CMyRecordset::GetFieldRef(UINT uiIndex)
{
	return m_objFields+uiIndex;
}


///////////////////////////////////////////////////////////////////////
void CMyRecordset::GetFieldValue(UINT uiIndex)		//paled - 将ROW中的串转换为数，并填入objField的VAL联合中。
{
/*
	if ( m_ulRowsCount ==0) return;
	if (m_Row[uiIndex] == 0)
	{	m_objFields[uiIndex].m_lVal = 0;
		return;
	}
	switch (m_objFields[uiIndex].m_ulType)
	{	
	case	FIELD_TYPE_TINY:
	case	FIELD_TYPE_SHORT:
		m_objFields[uiIndex].m_iVal = atoi(m_Row[uiIndex]);
		break;
	case	FIELD_TYPE_LONG:
		m_objFields[uiIndex].m_lVal = atol(m_Row[uiIndex]);
		break;
	case	FIELD_TYPE_DECIMAL:
	case	FIELD_TYPE_FLOAT:
	case	FIELD_TYPE_DOUBLE:
		m_objFields[uiIndex].m_dVal = atof(m_Row[uiIndex]);
		break;
	case	FIELD_TYPE_STRING:
	case	FIELD_TYPE_VAR_STRING:
		strcpy(m_objFields[uiIndex].m_szVal,m_Row[uiIndex]);
		break;
	}
*/
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
				/*
			case	FIELD_TYPE_DECIMAL:
			case	FIELD_TYPE_FLOAT:
			case	FIELD_TYPE_DOUBLE:
// 彭峰    添加新的两个类型支持mysql5.0 [7/23/2007]
			case	FIELD_TYPE_LONGLONG:// 支持BIGINT字段 [7/23/2007]
			case	FIELD_TYPE_NEWDECIMAL:// 支持精度数学DECIMAL或NUMERIC字段 [7/23/2007]
				m_objFields[i].m_dVal = atof(m_Row[i]);
				m_objFields[i].m_iVal = atoi(m_Row[i]);//让int也支持新的类型
				break;
				*/
				//////////////////////////////////////////////////////////////////////////
				//  [11/16/2007 朱斌]	union 类型只能用一个
			case	FIELD_TYPE_FLOAT:
				{
					m_objFields[i].m_fVal = atof(m_Row[i]);
				}
				break;
			case	FIELD_TYPE_DOUBLE:
				{
					m_objFields[i].m_dVal = atof(m_Row[i]);
				}
				break;
			case	FIELD_TYPE_LONGLONG:
				{
					m_objFields[i].m_i64Val = _atoi64(m_Row[i]);
				}
				break;
			case	FIELD_TYPE_DECIMAL:
			case	FIELD_TYPE_NEWDECIMAL:
				{
					m_objFields[i].m_iVal = atoi(m_Row[i]);
				}
				break;
				//////////////////////////////////////////////////////////////////////////
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
bool CMyRecordset::GetDefauldField()		//paled - 取缺省域值，并填入objField的VAL联合中。
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
// 彭峰    添加新的两个类型支持mysql5.0 [7/23/2007]

		case	FIELD_TYPE_LONGLONG:// 支持BIGINT字段 [7/23/2007]
			if(pField->def)
				m_objFields[i].m_i64Val = _atoi64(pField->def);
			else
				m_objFields[i].m_i64Val = 0;			
			break;
		case	FIELD_TYPE_DECIMAL:
		case	FIELD_TYPE_NEWDECIMAL:// 支持精度数学DECIMAL或NUMERIC字段 [7/23/2007]
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
	///softwors拼死改底层,将所有没有初始化的成员变量初始化,有锅我来背吧-2007-11-12
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
	m_bBOF=false;
	m_bDefaultValue=0;
	m_bEOF=0;
	m_Recordset=NULL;
	m_Row=NULL;
	///-----------------------------------

	m_bIsOpen = false;
	m_bEdit	  = false;

	m_pMyDatabase = pMyDatabase;
	if (pMyDatabase == NULL ) return;
	m_Recordset = NULL;
	
	if (m_pMyDatabase != NULL && m_pMyDatabase->IsOpen())
	{	m_pMyDatabase->m_nRefCount++;
	}
}			//★ 未初始化其它成员变量(其它成员变量由open()初始化)

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
	try {

		time_t	tStart = clock();
		if(!m_pMyDatabase)
			return false;
		
		nstep = 101;
		int nLen = strlen(szSQL);
		char * p,* sz= strstr((char*)szSQL, "FROM");
		int nSize = 0;
		if(sz == NULL)
			sz= strstr((char*)szSQL, "from");
		
		nstep = 102;
		if (!m_pMyDatabase->IsOpen())  return false;

		nstep = 103;
		MYSQL * hdbc = m_pMyDatabase->m_hdbc;

		nstep = 104;
		strcpy(m_szSQL,szSQL);

		nstep = 105;
		if (m_bIsOpen) 
		{	
			//权宜之计 可能会存在泄漏  具体问题待定
			DEBUG_TRY
			Close();
			DEBUG_CATCH("CMyRecordset::Open() close()")
			m_bIsOpen = false;

			g_nMyDBError = 2000;
			return false;
		}
		nstep = 106;

		InterlockedExchangeAdd(&g_DBDebug[4], 1);	//  [10/31/2007 朱斌]	查询次数

		clock_t tDBDebug0 = clock();

		//20070523修罗
		DWORD dwWaitResult = ::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME);
		nstep = 1;
		//if (WAIT_TIMEOUT ==::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME))
		if( WAIT_TIMEOUT == dwWaitResult)		
		{
			DEBUG_TRY
			//::LogSave("*DATABASE* WARNING: Open() overtime");
			//20070523ashulanTest-----------
			nstep = 101;
			char szLog[2048];
			nstep = 102;
			sprintf(szLog,"*DATABASE* WARNING: Open() overtime(%d):%s",
				dwWaitResult,szSQL);
			nstep = 103;
			::LogSave(szLog);
			//------------------------------

			DEBUG_CATCH2("WAIT_TIMEOUT == dwWaitResult nStep = %u",nstep)
			return false;
		}
//@@@int sec = (rand()%5)*100 + 0;printf("*[%d] --> ", sec/100); Sleep(sec); printf("|||\n");
		
		nstep = 2;
		InterlockedExchangeAdd(&g_DBDebug[0], clock() - tDBDebug0);
		clock_t tDBDebug1 = clock();
		//20070521ashulanTest-----------
		char szLog[2048];
		sprintf(szLog,"CMyRecordset::Open:%s",szSQL);
		//CTimeCostChk chk("CMyRecordset::Open", 1000);
// 		CTimeCostChk chk(szLog, 20);
		//------------------------------

		nstep = 3;
		if (mysql_query(hdbc, m_szSQL)!=NULL)
		{	
			nstep = 4;
			InterlockedExchangeAdd(&g_DBDebug[1], clock() - tDBDebug1);	//  [10/30/2007 朱斌]	执行时间
			RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
			g_nMyDBError = 2001;
			::MsgLogSave("gmlog/DbError", "CMyRecordset::Open Error:%s-%s-%d", m_szSQL,mysql_error(hdbc),mysql_errno(hdbc));
			//unsigned int err =  mysql_errno(hdbc);
			if(mysql_errno(hdbc)==2013)//处理Lost connection to MySQL server during query
			{
				LOGWARNING("DATABASE mysql_errno(m_hdbc)==2013 2 SQL = %s",szSQL);
				while(!(m_pMyDatabase->ErrorRecon()));
				
			}

			///softworms2007-07-02加强日志
//			UINT error=(UINT)mysql_error(hdbc);
			
			// log error sql
//			::MsgLogSave("gmlog/DbError", "%s", szSQL);
			return false;
		}
		else 
		{	
			nstep = 5;
			InterlockedExchangeAdd(&g_DBDebug[1], clock() - tDBDebug1);	//  [10/30/2007 朱斌]	执行时间
			clock_t tDBDebug2 = clock();
			m_Recordset=mysql_store_result(hdbc);//把所有的结果纪律一次性地全部保存在一个MYSQL_RES结果里// 彭峰 [7/23/2007]
			InterlockedExchangeAdd(&g_DBDebug[2], clock() - tDBDebug2);	//  [10/30/2007 朱斌]	取结果用时
			clock_t tDBDebug3 = clock();	
			RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
			if (m_Recordset == NULL)
			{	g_nMyDBError = 2002;
				return false;
			}
			else 
			{	
				nstep = 6;
				m_bBOF = false;
				m_bEOF = false;
				m_bIsOpen = true;
				m_bDefaultValue	= false;		// paled

				m_uiFieldsCount = mysql_num_fields(m_Recordset);//返回查询结果中的纪录总数// 彭峰 [7/23/2007]
				m_ulRowsCount = mysql_num_rows(m_Recordset);//返回查询结果里的纪录总数// 彭峰 [7/23/2007]
				m_Fields = mysql_fetch_field(m_Recordset);//返回一个MYSQL_FIELD结构：结果字段（数据列）的数据类型。
				
				nstep = 7;
				m_objFields = new CMyField [m_uiFieldsCount];
				//m_pbDirty	= new bool [m_uiFieldsCount];
#ifdef	USE_NEW
				m_pbDirty	= (bool *)s_heapString.Alloc(sizeof(bool) * m_uiFieldsCount);
#else
				m_pbDirty	= (bool *)g_heapSystem.New(sizeof(bool) * m_uiFieldsCount);		//★ 全局堆分配，可能产生内存碎片。
#endif
				ZeroMemory(m_pbDirty, sizeof(bool) * m_uiFieldsCount);
				

				nstep = 8;
				m_bCanUpdate = false;
				for (UINT i =0; i< m_uiFieldsCount; i++)
				{
					nstep = 9;
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
					
					nstep = 10;
					m_objFields[i].m_pMyField = &m_objFields[i];	//paled
					m_objFields[i].m_nIndex = i;
					if (
						!m_bCanUpdate && (
						m_Fields[i].flags & PRI_KEY_FLAG ||
						//paled 不安全		m_Fields[i].flags & UNIQUE_KEY_FLAG ||
						m_Fields[i].flags & AUTO_INCREMENT_FLAG ) )
					{	
						m_bCanUpdate = true;
						strcpy(m_szKey,m_Fields[i].name);
						m_uiKeyIndex = i;
					}
				}
				nstep = 11;
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
				nstep = 12;
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
		if(nstep>=1)
			RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
		::LogSave("Mysql RecordSet Open Exception Error.sql= %s,nstep = %u",szSQL,nstep);
		return false;
	}
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::Close()
{	
	try {
		if (m_bIsOpen)
		{	
#ifdef	USE_NEW
			s_heapString.Free(m_pbDirty);
#else
			g_heapSystem.Delete(m_pbDirty);
#endif
			for (unsigned int i=0;i<m_uiFieldsCount;i++)
			if (m_objFields[i].m_ulType == FIELD_TYPE_STRING 
				|| m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING)
#ifdef	USE_NEW
				s_heapString.Free(m_objFields[i].m_szVal);
#else
				g_heapSystem.Delete(m_objFields[i].m_szVal);
#endif

			delete [] m_objFields;
			m_pMyDatabase->m_nRefCount --;
			m_pMyDatabase->DecreaseRef((UINT)this);
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
	if (m_bIsOpen) Close();
}


///////////////////////////////////////////////////////////////////////
// 数据集操作
void CMyRecordset::Edit()
{	m_bEdit = true;
	if (! m_bIsOpen ) return;
	m_nEditMode = modeEdit;

}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::AddNew()
{	m_bEdit = true;
	if (! m_bIsOpen ) return;
	m_nEditMode = modeAddNew;
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::Delete()
{	m_bEdit = true;
	if (! m_bIsOpen ) return;
	m_nEditMode = modeDelete;
}

///////////////////////////////////////////////////////////////////////
// 数据集游标移动
void CMyRecordset::MoveFirst()
{	
	if(m_bEdit)
		::OutputDebugString("*DATABASE* warning: MoveFirst() in edit state.");

	m_bEdit = false;				//★ 有没有先存盘呀？下同
	if (! m_bIsOpen ) return;
	if (!m_bBOF) 
	{	m_ulCursor = 0;
		m_bBOF = true;
		assert( m_bIsOpen );
		mysql_data_seek(m_Recordset,0);
		m_Row=mysql_fetch_row( m_Recordset );
		GetAllField();
	}
	m_bEOF = false;		//paled - 数据库必须至少有一行数据，下同
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::MovePrev()
{
	if(m_bEdit)
		::OutputDebugString("*DATABASE* warning: MovePrev() in edit state.");

	m_bEdit = false;
	if (! m_bIsOpen ) return;
	m_bEOF=false;
	if (m_ulCursor > 0)
	{	m_ulCursor -- ;
		mysql_data_seek(m_Recordset,m_ulCursor);
		m_Row=mysql_fetch_row( m_Recordset );
		GetAllField();
	}
	else 
		m_bBOF = true;
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::MoveLast()
{	
	if(m_bEdit)
		::OutputDebugString("*DATABASE* warning: MoveLast() in edit state.");

	m_bEdit = false;
	if (! m_bIsOpen ) return;
	if (!m_bEOF)
	{	m_ulCursor = m_ulRowsCount-1;
		mysql_data_seek(m_Recordset,m_ulCursor);
		m_Row=mysql_fetch_row( m_Recordset );
		GetAllField();
		m_bBOF = false;
		m_bEOF = true;
	}
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::Move(ULONG ulIndex)
{	
	if(m_bEdit)
		::OutputDebugString("*DATABASE* warning: Move() in edit state.");

	m_bEdit = false;
	if (! m_bIsOpen ) return;
	if (ulIndex < m_ulRowsCount ) 
	{	mysql_data_seek(m_Recordset,ulIndex);
		m_ulCursor = ulIndex;
		m_Row=mysql_fetch_row( m_Recordset );
		GetAllField();
	}

	m_bBOF = (m_ulCursor == 0);
	m_bEOF = (m_ulCursor == m_ulRowsCount-1);
}


///////////////////////////////////////////////////////////////////////
// SQL
bool CMyRecordset::Query(char * strSQL, MYSQL_RES * * res)
{
	TimeCostChk("CMyRecordset::Query",strSQL);

	if (!CMyDatabase::CheckSQL(strSQL))
		return false;


	try {
		if (!m_bIsOpen) return false;

		time_t	tStart = clock();


		//20070523修罗
		DWORD dwWaitResult = ::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME);
		//if (WAIT_OBJECT_0 ==::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME))
		if( WAIT_OBJECT_0 == dwWaitResult)
		{
			//20070521ashulanTest-----------
			char szLog[2048];
			sprintf(szLog,"CMyRecordset::Query:%s",strSQL);
			//CTimeCostChk chk("CMyRecordset::Query", 1000);
// 			CTimeCostChk chk(szLog, 20);
			//------------------------------

			MYSQL * hdbc = m_pMyDatabase->m_hdbc;
			if (mysql_query(hdbc, strSQL)!=NULL)
			{	
				LOCKADD_TIMESUM;
				///softworms2007-07-02加强日志
				::MsgLogSave("gmlog/DbError", "CMyRecordset::Query Error:%s-%s-%d", strSQL,mysql_error(hdbc),mysql_errno(hdbc));
				//error
				RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);

				if(mysql_errno(hdbc)==2013)//处理Lost connection to MySQL server during query
				{
					LOGWARNING("DATABASE mysql_errno(m_hdbc)==2013 3 SQL = %s",strSQL);
					while(!(m_pMyDatabase->ErrorRecon()));
					
				}

				// log error sql
//				::MsgLogSave("gmlog/DbError", "%s", strSQL);
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
			//::LogSave("*DATABASE* WARNING: Qurey() overtime");
			//20070523ashulanTest-----------
			char szLog[2048];
			sprintf(szLog,"*DATABASE* WARNING: Qurey() overtime(%d):%s",
				dwWaitResult,strSQL);
			::LogSave(szLog);
			//------------------------------
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
bool CMyRecordset::IsBOF()
{	return m_bBOF;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::IsEOF()
{	return m_bEOF;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::IsOpen()
{
	//assert(this);
	return (m_bIsOpen);
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
	if (!m_bIsOpen ) return false; 
	return m_bCanUpdate; 
}

///////////////////////////////////////////////////////////////////////
// 
CMyField CMyRecordset::Fields(UINT uiIndex)
{	
	if (uiIndex >= m_uiFieldsCount) 
	{
		::LogSave("*DATABASE* ERROR: Illegal Fields() parameter[%u].", uiIndex);
		uiIndex=0;			//★ 0号对象是非法对象么?
	}
	
	if (m_bEdit)
	{	m_pbDirty[uiIndex] = true;
		CMyField MyField = m_objFields[uiIndex];
		MyField.m_pMyField = &m_objFields[uiIndex];	//paled
		MyField.m_nIndex = uiIndex;
		MyField.m_szVal = m_objFields[uiIndex].m_szVal;
		return MyField;				//★ BUG: 没有初始化 m_Field 成员变量。没有初始化数值类成员变量
	}

//@	m_pbDirty[uiIndex] = false;
	GetFieldValue(uiIndex);
	return m_objFields[uiIndex];
}

///////////////////////////////////////////////////////////////////////
CMyField CMyRecordset::Fields(char * szField)
{
	int nIndex = -1 ;
	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if ( stricmp(szField, m_Fields[i].name) == 0)
		{	
			nIndex = i;
			break;
		}
	}

	if (nIndex != -1)	
		return Fields(nIndex);
	else 
	{
		::LogSave("Error: field %s not found in CMyRecordset::Fields", szField);
		return Fields((unsigned int)0);
	}
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
bool CMyRecordset::AddField(LPCTSTR szField, int nData)
{	
	sprintf(m_szUpdateSQL, "UPDATE %s SET %s=%s+%d WHERE %s=%s LIMIT 1", 
				m_szTableName, szField, szField, nData, m_szKey, m_Row[m_uiKeyIndex]);

	MYSQL_RES * res;
	bool ret = Query(m_szUpdateSQL, &res);
	if(ret && res)
		mysql_free_result(res);
	return	ret;
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
	m_bEdit = false;
	if(! m_bIsOpen )
		return false;

	if ( m_ulCursor < m_ulRowsCount-1 )
	{
		m_bBOF = false;

		m_ulCursor ++ ;
		m_Row=mysql_fetch_row( m_Recordset );			//paled - 不必调用 mysql_data_seek()
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
IRecord* CMyRecordset::CreateNewConstRecord(DWORD idNewKey /*= NEWKEY_NONE*/) 
{
	CMyConstRecord* pRecord = CMyConstRecord::CreateNew(m_pMyDatabase);		//VVVVVVVVVVVVVV
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

	m_bEdit		= false;
	m_nEditMode	= modeAddNew;

	bool ret	=true;
	if (this->UpdateFieldsCount() > 0)
	{
		// build sql
		this->BuildUpdateSQL();

		// need change the local row;
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

	m_bEdit		= false;
	m_nEditMode	= modeEdit;

	bool ret	=true;
	if (this->UpdateFieldsCount() > 0)
	{
		// build sql
		this->BuildUpdateSQL();

		// need change the local row;
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

	m_bEdit		= false;
	m_nEditMode	= modeDelete;

	bool ret	=true;

	// build sql
	this->BuildUpdateSQL();

	// need change the local row;
	MYSQL_RES * res;
	ret = Query(m_szUpdateSQL, &res);
	if(ret && res)
		mysql_free_result(res);

	// clear edit flag
	this->ClsEditFlag();

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

BOOL CMyRecordset::SetThisKey(LPCTSTR szField)
{
	IF_NOT(szField)
		return FALSE;

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
			m_uiKeyIndex = i;
			strcpy(m_szKey, szField);
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::SetInt		(LPCTSTR szField, int nData)
{
	IF_NOT(szField)
		return;

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
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

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
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


//-------彭峰---2007/05/15-----得到DOUBLE型的数据
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

	m_pbDirty[idx] = true;
	if (m_objFields[idx].m_szVal)			// 串类型
		SafeCopy(m_objFields[idx].m_szVal, szData, nSize);

	return;
}




