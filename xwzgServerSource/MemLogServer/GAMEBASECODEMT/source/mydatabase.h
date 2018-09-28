
#ifndef __MYDB_H__
#define __MYDB_H__

#include "I_mydb.h"
#include "MyHeap.h"
#include <iostream>
#include "map"
using namespace std;

typedef char * HSTMT; 
typedef bool ( * ENUMPROC)(char * ); 
typedef map<UINT,string> SQLDEBUGLIST;///�����������ݿ��ڴ�й¶����

static	int g_nMyDBError;
static	char m_szMyDBErrorMsg[256];
/////////////////////////////////////////////////////////////////////////////
class CMyDatabase : public IDatabase
{
	// Construct
public:
	CMyDatabase();
	virtual ~CMyDatabase();
public:
	static CMyDatabase*	CreateNew()	{ return new CMyDatabase; }		// { (CMyDatabase*)s_heap.Alloc(sizeof(CMyDatabase)); }
protected: // interface
	virtual	void	Release() { delete this; }				// { s_heap.Free(this); }
	virtual IRecordset*	CreateNewRecordset(LPCTSTR szSQL, bool bNoEmpty = true);
public: // interface
	virtual bool	ExecuteSQL(char* szSQL);
	virtual int		GetInsertId();
	virtual int		CountRecord	(const char* szSQL);

public:
	IDatabase*	GetInterface() { return (IDatabase*)this; }

	// Init & Final
	BOOL Init		(char* szType);
	void Destroy	(void);

	bool	ExecuteSQL(char * szSQL, MYSQL_RES ** pRes) { return ExecuteSQL(szSQL); }	//�� �����ڼ����ڻ������
//	int		GetInsertId();

	static	bool CheckSQL	(const char* pszSQL);

private:
	// Attributes
	bool	IsOpen()		{return m_bIsOpen;}

	bool	EnumDatabases	(ENUMPROC lpEnumFunc);
	bool	EnumTables		(ENUMPROC lpEnumFunc);
	void	GetDBErrorMsg	(char * szErrorMsg);

	// Operations
	bool Open(char * szHost, char * szUser, char * szPasswd,char * szDB, 
			unsigned int uiPort = MYSQL_PORT,char * charset = "gbk", char * szSocket = NULL, 
			unsigned int uiFlag = NULL);
	virtual void Close();

protected:
	MYSQL*	m_hdbc;			
	int		m_nRefCount;
	HANDLE	m_hDbMutex;

	// friend classes that call protected CMyDatabase overridables
	friend class CMyRecordset;
	friend class CMyRecord;

private:				
	bool 		m_bIsOpen;
	char		m_szDBName[32];

protected: // heap
	static CMyHeap	s_heap;

public:
	Reconnect();
	bool ErrorRecon();

protected:
	int		m_nMySQlErrorCode;	//  [2/22/2008 ���]	�������,ֻ�ڳ�OPEN����mysql_real_connect����ʱ����
	UINT	m_nOpenCount;///ִ��SQL���Ĵ���
	SQLDEBUGLIST m_sqlDebugList;///����ÿ��ִ�е�SQL���
};

// extern IDatabase*	CreateDatabase(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL);


/////////////////////////////////////////////////////////////////////////////
#endif //__MYDB_H__

