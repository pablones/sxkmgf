// MyRecordset.cpp: implementation of the CMyRecordset class.
//
//////////////////////////////////////////////////////////////////////

#ifdef	MULTITHREAD_SAFE
#define	LOCKADD_TIMESUM
#else
extern	long	s_nDatabaseTimeSum;			//?? ����ͳ�����ݿ���������ĵ�ʱ��
#define	LOCKADD_TIMESUM		::InterlockedExchangeAdd(&s_nDatabaseTimeSum, clock() - tStart)
#endif

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
#include "PrcHeap.h"

#ifdef	NO_MUTEX
	#define		RELEASE_MUTEX(x)
#else
	#define		RELEASE_MUTEX(x)		::ReleaseMutex(x)
#endif


// static init
HANDLE	CMyRecordset::s_hHeap				=NULL;
UINT	CMyRecordset::s_uNumAllocsInHeap	=0;
#ifdef	MULTITHREAD_SAFE
	CCriticalSection	CMyRecordset::m_xCtrl;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ���� UpdateSQLs

///////////////////////////////////////////////////////////////////////
void CMyRecordset::BuildUpdateCondition()
{
	if (m_ulRowsCount == 0) 
		return; 

	sprintf(m_szUpdateCondition, "%s=%s", m_szKey, m_Row[m_uiKeyIndex]);
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
					if (strlen(m_objFields[i].m_szVal) == NULL) 
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
						::LogSave("ERROR: BuildUpdateOpration() ���!!!");
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

	sprintf(m_szUpdateSQL,psz,m_szTableName,			//paled - SQL_STMT_DELETE�ĵ�2������Ϊ�մ�
		m_szUpdateOpration,m_szUpdateCondition);		//paled - SQL_STMT_INSERTֻ��������������������3������
	if(strlen(m_szUpdateSQL) >= 1024)
		::LogSave("ERROR: BuildUpdateSQL() ���!!!");
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
void CMyRecordset::GetFieldValue(UINT uiIndex)		//paled - ��ROW�еĴ�ת��Ϊ����������objField��VAL�����С�
{
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
}	

// ��������

///////////////////////////////////////////////////////////////////////
CMyRecordset::CMyRecordset(CMyDatabase  * pMyDatabase )
{	
	m_bIsOpen = false;
	m_bEdit	  = false;

	m_pMyDatabase = pMyDatabase;
	if (pMyDatabase == NULL ) return;
	m_Recordset = NULL;
	
	if (m_pMyDatabase != NULL && m_pMyDatabase->IsOpen())
	{	m_pMyDatabase->m_nRefCount++;
	}
}			//�� δ��ʼ��������Ա����(������Ա������open()��ʼ��)

///////////////////////////////////////////////////////////////////////
CMyRecordset* CMyRecordset::CreateNew(CMyDatabase  * pMyDatabase)
{
	if (pMyDatabase == NULL ||
			!pMyDatabase->IsOpen())
		return NULL;

	return (new CMyRecordset(pMyDatabase));
}

///////////////////////////////////////////////////////////////////////
void* CMyRecordset::operator new(size_t size)
{
	LOCKOBJ;

	// got heap?
	if (!s_hHeap)
	{
		s_hHeap	=::HeapCreate(0, 0, 0);
		if (!s_hHeap)
			return NULL;
	}

	// alloc mem for new obj
	void* p	=::HeapAlloc(s_hHeap, HEAP_ZERO_MEMORY, size);

	if (p)
	{
		s_uNumAllocsInHeap++;
	}

	return p;
}

///////////////////////////////////////////////////////////////////////
void CMyRecordset::operator delete(void* p)
{
	LOCKOBJ;

	// check param
	if (!s_hHeap || s_uNumAllocsInHeap <= 0)
		return;

	// free it...
	if (::HeapFree(s_hHeap, 0, p))
	{
		s_uNumAllocsInHeap--;
	}

	// time to destroy heap?
	if (s_uNumAllocsInHeap <= 0)
	{
		if (::HeapDestroy(s_hHeap))
			s_hHeap	=NULL;
	}
}

///////////////////////////////////////////////////////////////////////
BOOL CMyRecordset::IsValidPt(void* p)
{
	LOCKOBJ;

	if (!s_hHeap || !p)
		return false;

	if ((DWORD)p < 0x00010000 || (DWORD)p >= 0x7FFEFFFF)	// user address range from 1M--2G-64k
		return false;

	if (::IsBadCodePtr((FARPROC)p))
		return false;

	return ::HeapValidate(s_hHeap, 0, p);
}

///////////////////////////////////////////////////////////////////////
bool CMyRecordset::Open(const char * szSQL)
{
	int nStep = 0;
	try {		
		time_t	tStart = clock();
		if(!m_pMyDatabase)
		{
			char szLog[2048];
			sprintf(szLog,"CMyRecordset::Open (!m_pMyDatabase):%s",szSQL);				
			::LogSave(szLog);

			g_nMyDBError = 2000;
			return false;
		}
		nStep = 1;

		int nLen = strlen(szSQL);
		char * p,* sz= strstr((char*)szSQL, "FROM");
		int nSize = 0;
		nStep = 2;

		if (!m_pMyDatabase->IsOpen())  
		{
			char szLog[2048];
			sprintf(szLog,"CMyRecordset::Open (!m_pMyDatabase->IsOpen()):%s",szSQL);				
			::LogSave(szLog);

			g_nMyDBError = 2001;
			return false;
		}
		nStep = 3;

		MYSQL * hdbc = m_pMyDatabase->m_hdbc;

		strcpy(m_szSQL,szSQL);

		if (m_bIsOpen) 
		{
			char szLog[2048];
			sprintf(szLog,"CMyRecordset::Open (m_bIsOpen):%s",szSQL);				
			::LogSave(szLog);

			g_nMyDBError = 2002;
			Close();			
			return false;
		}
		nStep = 4;

//#ifdef	NO_MUTEX
//		if(false)
//#else
//		if (WAIT_TIMEOUT ==::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME))
//#endif
//		{
//			::LogSave("*DATABASE* WARNING: Open() overtime");
//			return false;
//		}
////@@@int sec = (rand()%5)*100 + 0;printf("*[%d] --> ", sec/100); Sleep(sec); printf("|||\n");


		//20070523����
		DWORD dwWaitResult = ::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME);
		//if (WAIT_TIMEOUT ==::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME))
		if( WAIT_TIMEOUT == dwWaitResult)		
		{
			//::LogSave("*DATABASE* WARNING: Open() overtime");
			//20070523ashulanTest-----------
			g_nMyDBError = 2003;
			char szLog[2048];
			sprintf(szLog,"*DATABASE* WARNING: Open() overtime(%d):%s",
				dwWaitResult,szSQL);
			::LogSave(szLog);
			//------------------------------

			return false;
		}
		nStep = 5;
//@@@int sec = (rand()%5)*100 + 0;printf("*[%d] --> ", sec/100); Sleep(sec); printf("|||\n");

		//  [3/14/2008 ���]	����
		mysql_options(hdbc, MYSQL_OPT_RECONNECT,"1");	
		nStep = 6;
		time_t tQueryStart = clock();

		if (mysql_query(hdbc, m_szSQL)!=NULL)
		{	
			RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
			g_nMyDBError = 2004;
			//::MsgLogSave("CMyRecord::Query Error:%s-%s-%d", m_szSQL,mysql_error(hdbc),mysql_errno(hdbc));

			char szLog[2048];
			sprintf(szLog,"CMyRecordset::Open (mysql_query(hdbc, m_szSQL)!=NULL):%s-%s-%d",szSQL,mysql_error(hdbc),mysql_errno(hdbc));				
			::LogSave(szLog);
			return false;
		}
		else 
		{	
			nStep = 7;
			time_t tQueryUsed = clock() - tQueryStart;
			if(tQueryUsed>20)//���ݿ�ִ�г���20ms
			{
				char szLog[2048];
				sprintf(szLog,"CMyRecordset::Open overTime(%d):%s",tQueryUsed,szSQL);				
				::LogSave(szLog);
			}
			nStep = 8;


			m_Recordset=mysql_store_result(hdbc);

			RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
			if (m_Recordset == NULL)
			{	
				char szLog[2048];
				sprintf(szLog,"CMyRecordset::Open (m_Recordset == NULL):%s",szSQL);				
				::LogSave(szLog);

				g_nMyDBError = 2005;
				return false;
			}
			else 
			{	
				m_bBOF = false;
				m_bEOF = false;
				m_bIsOpen = true;
				nStep = 9;

				m_uiFieldsCount = mysql_num_fields(m_Recordset);
				m_ulRowsCount = mysql_num_rows(m_Recordset);
				m_Fields = mysql_fetch_field(m_Recordset);
				nStep = 10;
				
				m_objFields = new CMyField [m_uiFieldsCount];
				//m_pbDirty	= new bool [m_uiFieldsCount];
#ifdef	USE_NEW
				m_pbDirty	= new bool[m_uiFieldsCount];
#else
				m_pbDirty	= (bool *)g_heapSystem.New(sizeof(bool) * m_uiFieldsCount);		//�� ȫ�ֶѷ��䣬���ܲ����ڴ���Ƭ��
#endif
				ZeroMemory(m_pbDirty, sizeof(bool) * m_uiFieldsCount);
				nStep = 11;

				m_bCanUpdate = false;
				for (UINT i =0; i< m_uiFieldsCount; i++)
				{	m_objFields[i].m_Field = m_Fields[i];
					m_objFields[i].m_ulType = m_Fields[i].type;

					if (m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING 
							|| m_objFields[i].m_ulType == FIELD_TYPE_STRING)
#ifdef	USE_NEW
						m_objFields[i].m_szVal	= new char[m_Fields[i].length+1];
#else
						m_objFields[i].m_szVal = (char *) g_heapSystem.New(sizeof(char)*(m_Fields[i].length+1));
																		//�� ȫ�ֶѷ��䣬�����ڴ���Ƭ
#endif
					
					m_objFields[i].m_pMyField = &m_objFields[i];	//paled
					m_objFields[i].m_nIndex = i;
					if (
						!m_bCanUpdate && (
						m_Fields[i].flags & PRI_KEY_FLAG ||
						//paled ����ȫ		m_Fields[i].flags & UNIQUE_KEY_FLAG ||
						m_Fields[i].flags & AUTO_INCREMENT_FLAG ) )
					{	m_bCanUpdate = true;
						strcpy(m_szKey,m_Fields[i].name);
						m_uiKeyIndex = i;
					}
				}
				nStep = 12;
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
				nStep = 13;
				strncpy(m_szTableName,p,nSize);
				m_szTableName[nSize]='\0';
			}
		}
		LOCKADD_TIMESUM;
		return true;
	}
	catch(...)
	{
		char szLog[2048];
		sprintf(szLog,"CMyRecordset::Open (catch(...)):%s-%d-%d",szSQL,nStep,::GetLastError());	
		::LogSave(szLog);

		char szMsg[]	="Mysql RecordSet open error.";
		::OutputDebugString(szMsg);
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
			delete [] m_pbDirty;
#else
			g_heapSystem.Delete(m_pbDirty);
#endif
			for (unsigned int i=0;i<m_uiFieldsCount;i++)
			if (m_objFields[i].m_ulType == FIELD_TYPE_STRING 
				|| m_objFields[i].m_ulType == FIELD_TYPE_VAR_STRING)
#ifdef	USE_NEW
				delete [] m_objFields[i].m_szVal;
#else
				g_heapSystem.Delete(m_objFields[i].m_szVal);
#endif

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
		char szMsg[]	="Mysql RecordSet close error.";
		::OutputDebugString(szMsg);
	}
}

///////////////////////////////////////////////////////////////////////
CMyRecordset::~CMyRecordset()
{
	if (m_bIsOpen) Close();
}


///////////////////////////////////////////////////////////////////////
// ���ݼ�����
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
// ���ݼ��α��ƶ�
void CMyRecordset::MoveFirst()
{	
	if(m_bEdit)
		::OutputDebugString("*DATABASE* warning: MoveFirst() in edit state.");

	m_bEdit = false;				//�� ��û���ȴ���ѽ����ͬ
	if (! m_bIsOpen ) return;
	if (!m_bBOF) 
	{	m_ulCursor = 0;
		m_bBOF = true;
		assert( m_bIsOpen );
		mysql_data_seek(m_Recordset,0);
		m_Row=mysql_fetch_row( m_Recordset );
	}
	m_bEOF = false;		//paled - ���ݿ����������һ�����ݣ���ͬ
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
	}

	m_bBOF = (m_ulCursor == 0);
	m_bEOF = (m_ulCursor == m_ulRowsCount-1);
}


///////////////////////////////////////////////////////////////////////
// SQL
bool CMyRecordset::Query(char * strSQL, MYSQL_RES * * res)
{
	if (!CMyDatabase::CheckSQL(strSQL))
		return false;

	try {
		if (!m_bIsOpen) return false;

		time_t	tStart = clock();
#ifdef	NO_MUTEX
		if(true)
#else
		if (WAIT_OBJECT_0 ==::WaitForSingleObject(m_pMyDatabase->m_hDbMutex, _MAX_DBACCESSTIME))
#endif
		{
			MYSQL * hdbc = m_pMyDatabase->m_hdbc;
			if (mysql_query(hdbc, strSQL)!=NULL)
			{	
				LOCKADD_TIMESUM;
				//error
				RELEASE_MUTEX(m_pMyDatabase->m_hDbMutex);
				///-pengfeng��ǿ��־
				::MsgLogSave("CMyRecord::Query Error:%s-%s-%d", strSQL,mysql_error(hdbc),mysql_errno(hdbc));
				//error
				return false;
			}
			else 
			{	
				LOCKADD_TIMESUM;
				*res=mysql_store_result(hdbc);			//paled - ������INSERT��DELETE��UPDATE�Ȳ�ѯʱ����ʹ�� mysql_affected_rows() ����Ƿ�ɹ�
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
			::LogSave("*DATABASE* WARNING: Qurey() overtime");
			return false;
		}
	}
	catch(...)
	{
		char szMsg[1024];
		sprintf(szMsg, "Mysql Query error: %s", strSQL);
		::OutputDebugString(szMsg);

		return false;
	}
}

// ��������
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
	if (uiIndex > m_uiFieldsCount) 
	{
		::LogSave("*DATABASE* ERROR: Illegal Fields() parameter.");
		uiIndex=0;			//�� 0�Ŷ����ǷǷ�����ô?
	}
	
	if (m_bEdit)
	{	m_pbDirty[uiIndex] = true;
		CMyField MyField = m_objFields[uiIndex];
		MyField.m_pMyField = &m_objFields[uiIndex];	//paled
		MyField.m_nIndex = uiIndex;
		MyField.m_szVal = m_objFields[uiIndex].m_szVal;
		return MyField;				//�� BUG: û�г�ʼ�� m_Field ��Ա������û�г�ʼ����ֵ���Ա����
	}

	m_pbDirty[uiIndex] = false;
	GetFieldValue(uiIndex);
	return m_objFields[uiIndex];
}

///////////////////////////////////////////////////////////////////////
CMyField CMyRecordset::Fields(char * szField)
{
	int nIndex = -1 ;
	for (UINT i=0; i < m_uiFieldsCount; i++)
	{	
		if ( strcmp(szField, m_Fields[i].name) == 0)
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
		m_Row=mysql_fetch_row( m_Recordset );			//paled - ���ص��� mysql_data_seek()

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
IRecord* CMyRecordset::CreateNewRecord(DWORD idNewKey /*= 0*/) 
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
bool CMyRecordset::InsertRecord()
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
	else
		::LogSave("WARNING: CMyRecordset::InsertRecord() no found field.");

	// clear edit flag
	this->ClsEditFlag();

	return	ret;
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
//	if (this->UpdateFieldsCount() > 0)
	{
		// build sql
		this->BuildUpdateSQL();

		// need change the local row;
		MYSQL_RES * res;
		ret = Query(m_szUpdateSQL, &res);
		if(ret && res)
			mysql_free_result(res);
	}
//	else
//		::LogSave("WARNING: CMyRecordset::InsertRecord() no found field.");

	// clear edit flag
	this->ClsEditFlag();

	return	ret;
}














