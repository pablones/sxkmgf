
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
//	virtual bool	CheckSQL(const char* pszSQL);
	virtual int		GetInsertId();
	virtual int		CountRecord	(const char* szSQL);

public:
	IDatabase*	GetInterface() { return (IDatabase*)this; }

	// Init & Final
	BOOL Init		(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL);
	void Destroy	(void);

	bool	ExecuteSQL(char * szSQL, MYSQL_RES ** pRes) { return ExecuteSQL(szSQL); }	//�� �����ڼ����ڻ������
//	int		GetInsertId();

	static	bool CheckSQL	(const char* pszSQL);

// #define _DB_DEBUG
	///debug�ڴ�й¶�ӿ�
#ifdef _DB_DEBUG
// 	void	IncreaseRef(UINT nKey,LPCTSTR szSQL);
// 	void	DecreaseRef(UINT nKey);
// 	void    DumpRef();///�����ﵼ������δ�����رյļ�¼��sql���
	void IncreaseRef(UINT nKey,LPCTSTR szSQL)
	{
		///�ظ�key�Ĳ����ǲ������,����ط��Ǿ��Բ����ظ��ģ������㷨������
		SQLDEBUGLIST::iterator it=m_sqlDebugList.find(nKey);
		CHECK(it==m_sqlDebugList.end());
		
		m_sqlDebugList[nKey]=szSQL;
	}
	
	void DecreaseRef(UINT nKey)
	{
		///��鳢��ɾ��һ�������ڵļ�¼,���ﲻӦ�ó���
		SQLDEBUGLIST::iterator it=m_sqlDebugList.find(nKey);
		CHECK(it!=m_sqlDebugList.end());
		
		
		m_sqlDebugList.erase(nKey);
	}
	
	///�����ﵼ������δ�����رյļ�¼��sql���
	void DumpRef()
	{
		UINT nDumpCount=0;
		SQLDEBUGLIST::iterator it=m_sqlDebugList.begin();
		LogSave("CMyDatabase::DumpRef size:%d begin!!!!",m_sqlDebugList.size());
		while(it!=m_sqlDebugList.end())
		{
			LogSave("CMyDatabase::DumpRef(%d):%s",++nDumpCount,it->second.c_str());
		}
		m_sqlDebugList.clear();
		LogSave("CMyDatabase::DumpRef end!!!!!");
	}
#else
	inline void IncreaseRef(UINT nKey,LPCTSTR szSQL){}
	inline void DecreaseRef(UINT nKey){}
	inline void DumpRef(){}
#endif


private:
	// Attributes
	bool	IsOpen()		{return m_bIsOpen;}

	bool	EnumDatabases	(ENUMPROC lpEnumFunc);
	bool	EnumTables		(ENUMPROC lpEnumFunc);
	void	GetDBErrorMsg	(char * szErrorMsg);

	// Operations
	bool Open(char * szHost, char * szUser, char * szPasswd,char * szDB, 
			unsigned int uiPort = MYSQL_PORT, char * szSocket = NULL, 
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
	Reconnect(const char* pszDBServer, const char* pszDBUser, const char* pszPassword, const char* pszDBName);
	bool ErrorRecon();

protected:
	int		m_nMySQlErrorCode;	//  [2/22/2008 ���]	�������,ֻ�ڳ�OPEN����mysql_real_connect����ʱ����
	UINT	m_nOpenCount;///ִ��SQL���Ĵ���
	SQLDEBUGLIST m_sqlDebugList;///����ÿ��ִ�е�SQL���
};

// extern IDatabase*	CreateDatabase(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL);


/////////////////////////////////////////////////////////////////////////////
#endif //__MYDB_H__

