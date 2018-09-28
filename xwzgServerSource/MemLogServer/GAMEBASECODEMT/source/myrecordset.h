// myrecordset.h

#ifndef	MYRECORDSET_H
#define	MYRECORDSET_H

class CCriticalSection;
class CMyDatabase;
class IRecordset;
class IRecord;
class CMyRecordset : public IRecordset
{
	friend class CMyRecord;
	friend class CMyConstRecord;
	friend class CMyField;
// Constructor
public:
	CMyRecordset(CMyDatabase  * pMyDatabase);
	virtual ~CMyRecordset();

public: // recordset
	virtual UINT		RecordCount();								// return -1: error
	virtual bool		MoveNext();
	virtual void		MoveFirst();
protected: // record
	virtual IRecord*	CreateNewRecord(DWORD idNewKey = NEWKEY_NONE);
	virtual	void	Release() { delete this; }

	virtual DWORD	InsertRecord();
	virtual bool	UpdateRecord();
	virtual bool	DeleteRecord();

	virtual int		LoadInt		(LPCTSTR szField);
	virtual void	SetInt		(LPCTSTR szField, int nData);
	virtual UINT	LoadUInt	(LPCTSTR szField)					{ return (UINT)LoadInt(szField); }
	virtual void	SetUInt		(LPCTSTR szField, UINT nData)		{ SetInt(szField, nData); }
	virtual void	LoadString	(char* szData, LPCTSTR szField, int nSize);
	virtual void	SetString	(LPCTSTR szField, LPCTSTR szData, int nSize);

	// 新模式的接口
	virtual	__int64   GetInt64	(int idx);
	virtual	double  GetDouble	(int idx);
	virtual int		GetInt		(int idx);
	virtual LPCTSTR	GetStr		(int idx);
	virtual void	SetInt		(int idx, int nData);
	virtual void	SetStr		(int idx, LPCTSTR szData, int nSize);
	virtual void	ClearUpdateFlags	()							{ ClsEditFlag(); }

public:
#ifdef	MULTITHREAD_SAFE
	static CCriticalSection	m_xCtrl;
#endif

#ifdef	USE_NEW
	static CMyHeap	s_heapString;
#endif
	static CMyRecordset* CreateNew(CMyDatabase  *pMyDatabase);

public:
	MYHEAP_DECLARATION(s_heap)
	
public:
	enum { modeEdit, modeAddNew, modeDelete };
	bool Open( const char * lpszSQL);
	void Close();
public:
	int	 FieldsCount();
	bool IsEOF();
	bool IsOpen();
	bool CanUpdate() const;
	void ClsEditFlag();
protected:
	int				UpdateFieldsCount();
	
	void			BuildUpdateSQL();
	void			BuildUpdateCondition();
	void			BuildUpdateOpration();

	bool			Query(char * szSQL, MYSQL_RES * * res);

	void			GetAllField();
public:
	bool			GetDefauldField();

protected:
	CMyDatabase	*	m_pMyDatabase; //数据库指针
	//内部使用
	MYSQL_RES	*	m_Recordset;
	MYSQL_FIELD *	m_Fields;
	MYSQL_ROW		m_Row;
	//数据信息
	_int64			m_ulRowsCount;      //总行数
	_int64			m_ulCursor;			//当前位置
	unsigned int 	m_uiFieldsCount;    //总列数
	char	  		m_szKey[16];
	unsigned int 	m_uiKeyIndex;
	CMyField	*	m_objFields;

	//状态信息
	bool	*		m_pbDirty;      //字段值改变标志
	bool 			m_bIsOpen;		//是否打开标志
	bool 			m_bEOF;			//尾标志
	bool 			m_bEdit;		//能否编辑 （删除的不能编辑）
	int				m_nEditMode;	//更改模式（用于生成Sql语句）
	bool			m_bCanUpdate;		//满足有key，并能INCREMENT
	bool			m_bDefaultValue;	//是否填充过默认值

	//用于存盘
	char  			m_szSQL[1024];
	char 			m_szTableName[32];
	char 			m_szUpdateSQL[1024];
	char			m_szUpdateOpration[1024];
	char			m_szUpdateCondition[128];

	
};

#endif // MYRECORDSET_H