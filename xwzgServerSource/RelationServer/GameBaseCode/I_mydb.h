// ���ݿ�ײ��ͨ�ýӿ�
// �ɽ���, 2002.9.27
//////////////////////////////////////////////////////////////////


#ifndef	I_MYDB_H
#define	I_MYDB_H

#define PURE_VIRTUAL_DECLARATION_DB	= 0;
#define PURE_VIRTUAL_DECLARATION_DB_0	= 0;

/*
	˵����Ϊ���ݾ�ϵͳ�����������������ϵͳ��ͬ��
*/

class	CMyRecordset;
class	CMyRecord;
class	IRecordset;
class	IDatabase
{
public:
	virtual ~IDatabase() {}
	virtual IRecordset*	CreateNewRecordset(LPCTSTR szSQL)		PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB
public:
	virtual bool	ExecuteSQL(char* szSQL)				PURE_VIRTUAL_DECLARATION_DB_0
	virtual int		GetInsertId()						PURE_VIRTUAL_DECLARATION_DB_0
};

class	IRecord
{
public:
	virtual ~IRecord() {}
	virtual IRecord*	CreateNewRecord(DWORD idNewKey = 0)		PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB

	virtual bool	InsertRecord()								PURE_VIRTUAL_DECLARATION_DB
	virtual bool	UpdateRecord()								PURE_VIRTUAL_DECLARATION_DB
	virtual bool	DeleteRecord()								PURE_VIRTUAL_DECLARATION_DB

	virtual int		LoadInt		(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetInt		(LPCTSTR szField, int nData)		PURE_VIRTUAL_DECLARATION_DB
	virtual UINT	LoadUInt	(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetUInt		(LPCTSTR szField, UINT nData)		PURE_VIRTUAL_DECLARATION_DB
	virtual void	LoadString	(char* szData, LPCTSTR szField, int nSize)		PURE_VIRTUAL_DECLARATION_DB
	virtual void	SetString	(LPCTSTR szField, LPCTSTR szData, int nSize)	PURE_VIRTUAL_DECLARATION_DB
	virtual DWORD	LoadDWord	(LPCTSTR szField)					{ return LoadUInt(szField); }
	virtual void	SetDWord	(LPCTSTR szField, DWORD nData)		{ SetUInt(szField, nData); }
	// ע����ʱ����Ҫ���ء���ȡ��д���Ĺ��򣬴�����ʵ�ָı��ɲ����˹���

	virtual bool	AddInt(LPCTSTR szField, int nData)			PURE_VIRTUAL_DECLARATION_DB		// ����д�⡣���޹����ͻ���ɲ��ṩ�ýӿ�
public:
//	virtual bool	IsOpen()									PURE_VIRTUAL_DECLARATION_DB_0
//	virtual bool	IsValidPt(void* p)							PURE_VIRTUAL_DECLARATION_DB_0
};

class	IRecordset : public IRecord
{
public:
	virtual ~IRecordset() {}
//	virtual IRecord*	GetRecordInterface()	PURE_VIRTUAL_DECLARATION_DB_0		// ȡIRecord
	virtual UINT		RecordCount()			PURE_VIRTUAL_DECLARATION_DB_0		// return -1: error
	virtual bool		MoveNext()				PURE_VIRTUAL_DECLARATION_DB			// return false: no more data
};


// SetDatabase()��SetTableName()�ݲ��ṩ�ӿڣ���ʱֱ�ӴӶ�����в�����

extern IDatabase*	CreateDatabase(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL);



#endif // I_MYDB_H