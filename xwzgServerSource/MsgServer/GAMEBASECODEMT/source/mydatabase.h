
#ifndef __MYDB_H__
#define __MYDB_H__

#include "I_mydb.h"
#include "MyHeap.h"
#include <iostream>
#include "map"
using namespace std;

typedef char * HSTMT; 
typedef bool ( * ENUMPROC)(char * ); 
typedef map<UINT,string> SQLDEBUGLIST;///用来监视数据库内存泄露问题

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

	bool	ExecuteSQL(char * szSQL, MYSQL_RES ** pRes) { return ExecuteSQL(szSQL); }	//★ 仅用于兼容于幻灵代码
//	int		GetInsertId();

	static	bool CheckSQL	(const char* pszSQL);

// #define _DB_DEBUG
	///debug内存泄露接口
#ifdef _DB_DEBUG
// 	void	IncreaseRef(UINT nKey,LPCTSTR szSQL);
// 	void	DecreaseRef(UINT nKey);
// 	void    DumpRef();///在这里导出所有未正常关闭的记录集sql语句
	void IncreaseRef(UINT nKey,LPCTSTR szSQL)
	{
		///重复key的插入是不允许的,这个地方是绝对不会重复的，除非算法有问题
		SQLDEBUGLIST::iterator it=m_sqlDebugList.find(nKey);
		CHECK(it==m_sqlDebugList.end());
		
		m_sqlDebugList[nKey]=szSQL;
	}
	
	void DecreaseRef(UINT nKey)
	{
		///检查尝试删除一个不存在的记录,这里不应该出错
		SQLDEBUGLIST::iterator it=m_sqlDebugList.find(nKey);
		CHECK(it!=m_sqlDebugList.end());
		
		
		m_sqlDebugList.erase(nKey);
	}
	
	///在这里导出所有未正常关闭的记录集sql语句
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
	int		m_nMySQlErrorCode;	//  [2/22/2008 朱斌]	错误代码,只在出OPEN调用mysql_real_connect出错时有用
	UINT	m_nOpenCount;///执行SQL语句的次数
	SQLDEBUGLIST m_sqlDebugList;///保存每次执行的SQL语句
};

// extern IDatabase*	CreateDatabase(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL);


/////////////////////////////////////////////////////////////////////////////
#endif //__MYDB_H__

