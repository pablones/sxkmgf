// MyRecord.cpp: implementation of the CMyRecord class.
// 仙剑修，2002.9.12
//////////////////////////////////////////////////////////////////////

#ifdef	MULTITHREAD_SAFE
	#include <afxmt.h>
	#define	LOCKOBJ		CSingleLock xLock(&m_xCtrl, true)
	#include <winsock2.h>
#else
	#define	LOCKOBJ		
#endif

extern	long	s_nDatabaseTimeSum;			//?? 用于统计数据库操作所消耗的时间
#define	LOCKADD_TIMESUM		::InterlockedExchangeAdd(&s_nDatabaseTimeSum, clock() - tStart)

#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "mydb.h"
#include "basefunc.h"
#include "MyRecord.h"
#include "common.h"

#define		RELEASE_MUTEX(x)		::ReleaseMutex(x)

// static init
CMyHeap	CMyRecord::s_heapString("CMyRecord::s_heapString");
#ifdef	MULTITHREAD_SAFE
	CCriticalSection	CMyRecord::m_xCtrl;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 构建 UpdateSQLs

MYHEAP_IMPLEMENTATION(CMyRecord,s_heap)
///////////////////////////////////////////////////////////////////////
bool CMyRecord::FormatField(char* szFormat, const CMyField& objField)
{
	switch (objField.m_ulType)
	{
	case FIELD_TYPE_STRING:
	case FIELD_TYPE_VAR_STRING:
		if ((objField.m_Field.flags&NOT_NULL_FLAG)  && strlen(objField.m_szVal) == NULL)			//paled 不能向数据库写入空串!!!
			return false;
		else 
			sprintf(szFormat, "'%s'",objField.m_szVal);
		break;

	case FIELD_TYPE_TINY:
	case FIELD_TYPE_SHORT:
		sprintf(szFormat, "%d",objField.m_iVal);
		break;

	case FIELD_TYPE_FLOAT:
		sprintf(szFormat, "%.2f",objField.m_fVal);
		break;

	case FIELD_TYPE_DOUBLE:
		sprintf(szFormat, "%.2f",objField.m_dVal);
		break;

	case FIELD_TYPE_LONG:
		if ((objField.m_Field.flags & UNSIGNED_FLAG) != 0)
			sprintf(szFormat, "%u", objField.m_ulVal);
		else
			sprintf(szFormat, "%ld", objField.m_lVal);
		break;
	case FIELD_TYPE_LONGLONG:
		sprintf(szFormat, "%I64d",objField.m_i64Val);
		break;

	default:
		::LogSave("Error: unknow type in CMyRecord::FieldToStr()");
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::BuildUpdateCondition()
{
	switch ( m_nEditMode )
	{	
	case modeEdit:
		sprintf(m_szUpdateCondition, "%s=%s", m_szKey, m_szKeyData);
		break;
	case modeAddNew:
	case modeDelete:
		sprintf(m_szUpdateCondition, "%s=%u", m_szKey, m_objFields[m_uiKeyIndex].m_ulVal);
		break;
	}
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::BuildUpdateOpration()
{
	char szFormat[1024];

	m_szUpdateOpration[0]='\0';

	BOOL bFirst = true;
	
	switch ( m_nEditMode )
	{	
	case modeEdit:
	case modeAddNew:
		for (UINT i=0; i<m_uiFieldsCount; i++ )
		{	
			if ( m_pbDirty[i] )
			{
				if (FormatField(szFormat, m_objFields[i]))
				{	
					if (!bFirst)
						strcat(m_szUpdateOpration,",");
					else 
						bFirst = false;

					strcat(m_szUpdateOpration, m_Fields[i].name);
					strcat(m_szUpdateOpration, "=");
					strcat(m_szUpdateOpration, szFormat);
					if(strlen(m_szUpdateOpration) >= 1024)
						::LogSave("ERROR: BuildUpdateOpration() 溢出!!!");
				}

			} // if ( m_pbDirty[i] )
		} // for ( i=0; i<m_uiFieldsCount; i++ )
		break;
	}
}


///////////////////////////////////////////////////////////////////////
void CMyRecord::BuildUpdateSQL()
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
CMyRecord::CMyRecord(CMyDatabase  * pMyDatabase )
{
	m_bCanUpdate=false;
	m_nEditMode=0;
	m_bEdit=false;
	m_Fields=NULL;
	m_objFields=NULL;
	m_pbDirty=NULL;
	m_pMyDatabase=NULL;
	strcpy(m_szKey,"");
	strcpy(m_szKeyData,"");
	strcpy(m_szTableName,"");
	strcpy(m_szUpdateCondition,"");
	strcpy(m_szUpdateOpration,"");
	strcpy(m_szUpdateSQL,"");
	m_uiFieldsCount=0;
	m_uiKeyIndex=0;


	m_bIsOpen = false;

	m_pMyDatabase = pMyDatabase;
	if (pMyDatabase == NULL ) 
		return;
	
	if (m_pMyDatabase != NULL && m_pMyDatabase->IsOpen())
	{	
		m_pMyDatabase->m_nRefCount++;
	}
}

///////////////////////////////////////////////////////////////////////
CMyRecord* CMyRecord::CreateNew(CMyDatabase  * pMyDatabase)
{
	if (pMyDatabase == NULL ||
			!pMyDatabase->IsOpen())
		return NULL;

	return (new CMyRecord(pMyDatabase));
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::Open(CMyRecordset  *pMyRecordset, DWORD idNewKey /*= NEWKEY_NONE*/)
{
	if (pMyRecordset == NULL ||	!pMyRecordset->IsOpen())
		return NULL;

	if (!m_pMyDatabase->IsOpen())
		return false;

	if (!( pMyRecordset->IsOpen() && (pMyRecordset->RecordCount() > 0 || pMyRecordset->m_bDefaultValue) ))	// m_bDefaultValue : add by paled
		return false;

	clock_t tDBDebug3 = clock();	//  [11/6/2007 朱斌]	处理用时

	m_uiFieldsCount	= pMyRecordset->m_uiFieldsCount;
	strcpy(m_szKey, pMyRecordset->m_szKey);
	m_uiKeyIndex	= pMyRecordset->m_uiKeyIndex;
	m_bEdit			= pMyRecordset->m_bEdit;
	m_bIsOpen			= pMyRecordset->m_bIsOpen;
	m_nEditMode		= pMyRecordset->m_nEditMode;
	strcpy(m_szTableName, pMyRecordset->m_szTableName); 
	m_bCanUpdate		= pMyRecordset->m_bCanUpdate;

#ifdef	USE_NEW
	m_pbDirty	= (bool*)s_heapString.Alloc(sizeof(bool) * pMyRecordset->m_uiFieldsCount);
#else
	m_pbDirty	= (bool *)g_heapSystem.New(sizeof(bool) * pMyRecordset->m_uiFieldsCount);		//★ 全局堆分配，可能产生内存碎片。
#endif

	memset(m_pbDirty, 0, (sizeof(bool) * pMyRecordset->m_uiFieldsCount));

#ifdef	USE_NEW
	m_Fields		= (MYSQL_FIELD*)s_heapString.Alloc(sizeof(MYSQL_FIELD) * m_uiFieldsCount);
#else
	m_Fields		= (MYSQL_FIELD*)g_heapSystem.New(sizeof(MYSQL_FIELD) * m_uiFieldsCount);
#endif

	for(int i = 0; i < (int)m_uiFieldsCount; i++)
	{
		m_Fields[i]	= pMyRecordset->m_Fields[i];

#ifdef	USE_NEW
		m_Fields[i].name = (char*)s_heapString.Alloc(sizeof(char) * (strlen(pMyRecordset->m_Fields[i].name)+1));
#else
		m_Fields[i].name = (char*)g_heapSystem.New(sizeof(char) * (strlen(pMyRecordset->m_Fields[i].name)+1));
#endif

		strcpy(m_Fields[i].name, pMyRecordset->m_Fields[i].name);		//VVVVV 注意删除。table、def不复制，从未用到过
	}

	m_objFields		= new CMyField [m_uiFieldsCount];
	for( i = 0; i < (int)m_uiFieldsCount; i++)
	{
		m_objFields[i]		= pMyRecordset->m_objFields[i];
		m_objFields[i].m_Field = m_Fields[i];					// 指向本地name串
		if (m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING 
				|| m_objFields[i].m_ulType == FIELD_TYPE_STRING)
		{

#ifdef	USE_NEW
			m_objFields[i].m_szVal = (char *) s_heapString.Alloc(sizeof(char)*(m_Fields[i].length+1));
#else
			m_objFields[i].m_szVal = (char *) g_heapSystem.New(sizeof(char)*(m_Fields[i].length+1));
#endif														//★ 全局堆分配，会有内存碎片
														
			strcpy(m_objFields[i].m_szVal, pMyRecordset->m_objFields[i].m_szVal);
		}
	}

	if(idNewKey != NEWKEY_NONE)
	{
		sprintf(m_szKeyData, "%u", idNewKey);
		m_objFields[m_uiKeyIndex].m_ulVal	= idNewKey;
	}
	else
		FormatField(m_szKeyData, pMyRecordset->m_objFields[pMyRecordset->m_uiKeyIndex]);

	InterlockedExchangeAdd(&g_DBDebug[3], clock() - tDBDebug3);	//  [11/06/2007 朱斌]	处理结果用时

	return true;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::Spawn(CMyRecord *pMyRecord, DWORD idNewKey /*= NEWKEY_NONE*/)
{
	*this	= *pMyRecord;

#ifdef	USE_NEW
	m_pbDirty	= (bool *)s_heapString.Alloc(sizeof(bool) * pMyRecord->m_uiFieldsCount);
#else
	m_pbDirty	= (bool *)g_heapSystem.New(sizeof(bool) * pMyRecord->m_uiFieldsCount);		//★ 全局堆分配，可能产生内存碎片。
#endif
	memset(m_pbDirty, 0, (sizeof(bool) * pMyRecord->m_uiFieldsCount));

#ifdef	USE_NEW
	m_Fields		= (MYSQL_FIELD*)s_heapString.Alloc(sizeof(MYSQL_FIELD) * m_uiFieldsCount);
#else
	m_Fields		= (MYSQL_FIELD*)g_heapSystem.New(sizeof(MYSQL_FIELD) * m_uiFieldsCount);
#endif

	for(int i = 0; i < (int)m_uiFieldsCount; i++)
	{
		m_Fields[i]	= pMyRecord->m_Fields[i];
#ifdef	USE_NEW
		m_Fields[i].name = (char*)s_heapString.Alloc(sizeof(char) * (strlen(pMyRecord->m_Fields[i].name)+1));
#else
		m_Fields[i].name	= (char*)g_heapSystem.New(sizeof(char) * (strlen(pMyRecord->m_Fields[i].name)+1));
#endif
		strcpy(m_Fields[i].name, pMyRecord->m_Fields[i].name);		//VVVVV 注意删除。table、def不复制，从未用到过
	}

	m_objFields		= new CMyField [m_uiFieldsCount];
	for( i = 0; i < (int)m_uiFieldsCount; i++)
	{
		m_objFields[i]		= pMyRecord->m_objFields[i];
		m_objFields[i].m_Field	= m_Fields[i];					// 指向本地name串
		if (m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING 
				|| m_objFields[i].m_ulType == FIELD_TYPE_STRING)
		{
#ifdef	USE_NEW
			m_objFields[i].m_szVal = (char *) s_heapString.Alloc(sizeof(char)*(m_Fields[i].length+1));
#else
			m_objFields[i].m_szVal = (char *) g_heapSystem.New(sizeof(char)*(m_Fields[i].length+1));
#endif
															//★ 全局堆分配，会有内存碎片
			strcpy(m_objFields[i].m_szVal, pMyRecord->m_objFields[i].m_szVal);
		}
	}

	if(idNewKey != NEWKEY_NONE)
	{
		sprintf(m_szKeyData, "%u", idNewKey);
		m_objFields[m_uiKeyIndex].m_ulVal	= idNewKey;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::Close()
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
			{
				if (m_objFields[i].m_ulType == FIELD_TYPE_STRING 
							|| m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING)
#ifdef	USE_NEW
					s_heapString.Free(m_objFields[i].m_szVal);
#else
					g_heapSystem.Delete(m_objFields[i].m_szVal);
#endif

#ifdef	USE_NEW
				s_heapString.Free(m_Fields[i].name);			// AAAAAAAAAAAAAAAAAA
#else
				g_heapSystem.Delete(m_Fields[i].name);
#endif
			}
			delete [] m_objFields;
#ifdef	USE_NEW
			s_heapString.Free(m_Fields);
#else
			g_heapSystem.Delete(m_Fields);
#endif
			m_pMyDatabase->m_nRefCount --;
		}
		m_bIsOpen = false;
	}
	catch(...)
	{
		::LogSave("Mysql RecordSet Close Exception error.");
	}
}

///////////////////////////////////////////////////////////////////////
CMyRecord::~CMyRecord()
{
	if (m_bIsOpen) 
		Close();
}

///////////////////////////////////////////////////////////////////////
// SQL
bool CMyRecord::Query(char * strSQL, MYSQL_RES * * res)
{
	TimeCostChk("CMyRecord::Query",strSQL);

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
			sprintf(szLog,"CMyRecord::Query:%s",strSQL);
			//CTimeCostChk chk("CMyRecord::Query", 1000);
// 			CTimeCostChk chk(szLog, 20);
			//------------------------------

			MYSQL * hdbc = m_pMyDatabase->m_hdbc;
			if (mysql_query(hdbc, strSQL)!=NULL)
			{
				LOCKADD_TIMESUM;
				///softworms2007-07-02加强日志
				//::MsgLogSave("gmlog/DbError", "CMyRecord::Query Error:%s-%s-%d", strSQL,mysql_error(hdbc),mysql_errno(hdbc));
				//error
				RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);

				if(mysql_errno(hdbc)==2013)//处理Lost connection to MySQL server during query
				{
					while(!(m_pMyDatabase->ErrorRecon()));
					
				}

				// log error sql
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
		RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
		::LogSave("Mysql Query Exception error: %s", strSQL);
		return false;
	}
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::IsOpen()
{
	//assert(this);
	return (m_bIsOpen);
}

///////////////////////////////////////////////////////////////////////
int CMyRecord::FieldsCount()
{
	if (m_bIsOpen)
		return m_uiFieldsCount;
	else 
		return -1;

}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::CanUpdate() const     // Can Edit/AddNew/Delete be called?
{	
	if (!m_bIsOpen ) return false; 
	return m_bCanUpdate; 
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::ClsEditFlag()
{
	for ( int i=0; i<(int)m_uiFieldsCount; i++ )
		m_pbDirty[i] = false;
}

///////////////////////////////////////////////////////////////////////
int CMyRecord::UpdateFieldsCount()
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
// record
///////////////////////////////////////////////////////////////////////
IRecord* CMyRecord::CreateNewRecord(DWORD idNewKey /*= NEWKEY_NONE*/) 
{
	CMyRecord* pRecord = CMyRecord::CreateNew(m_pMyDatabase);		//VVVVVVVVVVVVVV
	if(pRecord)
	{
		if(pRecord->Spawn(this, idNewKey))
			return (IRecord*)pRecord;

		delete pRecord;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////
DWORD CMyRecord::InsertRecord()
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
	{
		DWORD key = m_pMyDatabase->GetInsertId();
		if(key != 0)
		{
			sprintf(m_szKeyData, "%u", key);
			m_objFields[m_uiKeyIndex].m_ulVal	= key;
			return key;
		}
		else
			return m_objFields[m_uiKeyIndex].m_ulVal;
	}
	else
		return 0;
}

///////////////////////////////////////////////////////////////////////
bool CMyRecord::UpdateRecord()
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
#ifdef	PALED_DEBUG
		LOGWARNING("--- %s", m_szUpdateSQL);
#endif

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
bool CMyRecord::DeleteRecord()
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
int CMyRecord::LoadInt		(LPCTSTR szField)
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
void CMyRecord::SetInt		(LPCTSTR szField, int nData)
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
void CMyRecord::LoadString	(char* szData, LPCTSTR szField, int nSize)	
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
void CMyRecord::SetString	(LPCTSTR szField, LPCTSTR szData, int nSize)
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
int CMyRecord::GetInt		(int idx)							
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount)
	{
		LOGERROR("invalid index[%d] in table [%s].", idx, m_szTableName);
		return INT_NONE;
	}

	if (!m_objFields[idx].m_szVal)			// 非串类型
		return m_objFields[idx].m_iVal;
	else 
		return INT_NONE; 
}

int CMyRecord::GetInt(LPCTSTR szField)
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

LPCTSTR CMyRecord::GetStr(LPCTSTR szField)
{
	IF_NOT(szField)
		return INT_NONE;

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
			return GetStr(i);
		}
	}

	return INT_NONE;
}


void CMyRecord::SetStr(LPCTSTR szField,LPCTSTR szData, int nSize)
{
//	IF_NOT(szField)
//		return INT_NONE;

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
			SetStr(i,szData,nSize);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////
LPCTSTR CMyRecord::GetStr		(int idx)							
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount) 
	{
		LOGERROR("invalid index[%d] in table [%s].", idx, m_szTableName);
		return STR_NONE;
	}

	if (m_objFields[idx].m_szVal)			// 串类型
		return (m_objFields[idx].m_szVal);
	else 
		return STR_NONE;
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::SetInt		(int idx, int nData)				
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount) 
	{
		LOGERROR("Invalid index[%d] in table [%s]", idx, m_szTableName);
		return;
	}

	m_pbDirty[idx] = true;
	if (!m_objFields[idx].m_szVal)			// 非串类型
		m_objFields[idx].m_iVal = nData;

	return;
}

///////////////////////////////////////////////////////////////////////
void CMyRecord::SetStr		(int idx, LPCTSTR szData, int nSize)
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount) 
	{
		LOGERROR("Invalid index[%d] in table [%s]", idx, m_szTableName);
		return;
	}

	IF_NOT(nSize <= (int)m_Fields[idx].length+1)
	{
		LOGERROR("Invalid size[%d] in field[%s] of table [%s]", nSize, m_Fields[idx].name, m_szTableName);
		return;
	}

	m_pbDirty[idx] = true;
	if (m_objFields[idx].m_szVal)			// 串类型
		SafeCopy(m_objFields[idx].m_szVal, szData, nSize);

	return;
}

_int64  CMyRecord::GetInt64	(int idx)
{
	IF_NOT(idx>=0 && idx<(int)m_uiFieldsCount)
	{
		LOGERROR("invalid index[%d] in table [%s].", idx, m_szTableName);
		return INT_NONE;
	}

	if (!m_objFields[idx].m_szVal)			// 非串类型
		return m_objFields[idx].m_i64Val;
	else 
		return INT_NONE; 
}

_int64  CMyRecord::GetInt64	(LPCTSTR szField)
{
	IF_NOT(szField)
		return INT_NONE;

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
			return GetInt64(i);
		}
	}

	return INT_NONE;
}

void	CMyRecord::SetInt64	(int nIdx, _int64 i64Data)
{
	IF_NOT(nIdx>=0 && nIdx<(int)m_uiFieldsCount) 
	{
		LOGERROR("Invalid index[%d] in table [%s]", nIdx, m_szTableName);
		return;
	}

	m_pbDirty[nIdx] = true;
	if (!m_objFields[nIdx].m_szVal)			// 非串类型
		m_objFields[nIdx].m_i64Val = i64Data;

	return;
}

void	CMyRecord::SetInt64	(LPCTSTR szField, _int64 i64Data)
{
	IF_NOT(szField)
		return;

	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if (stricmp(szField, m_Fields[i].name) == 0)
		{
			SetInt64(i, i64Data);
			return;
		}
	}
}











