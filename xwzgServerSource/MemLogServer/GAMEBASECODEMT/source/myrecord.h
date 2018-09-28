// myrecords.h 用于分解出myrecordset.h中的单个记录。
// 仙剑修，2002.9.12
//////////////////////////////////////////////////////////////////////

#ifndef	MYRECORD_H
#define	MYRECORD_H

class CCriticalSection;
class CMyDatabase;

class CMyRecord : public IRecord
{
protected: // interface
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
	virtual int		GetInt		(int idx);
	virtual int     GetInt		(LPCTSTR szField);
	virtual	LPCTSTR		GetStr(LPCTSTR szField);
	virtual LPCTSTR	GetStr		(int idx);
	virtual void	SetInt		(int idx, int nData);
	virtual void	SetStr		(int idx, LPCTSTR szData, int nSize);
	virtual	void	SetStr		(LPCTSTR szField,LPCTSTR szData, int nSize);
	virtual	_int64  GetInt64	(int idx);
	virtual _int64	GetInt64	(LPCTSTR szField);
	virtual void	SetInt64	(int nIdx,			_int64 i64Data);
	virtual void	SetInt64	(LPCTSTR szField,	_int64 i64Data);

	virtual void	ClearUpdateFlags	()							{ ClsEditFlag(); }
public: // Constructor
	CMyRecord(CMyDatabase  * pMyDatabase);
	virtual ~CMyRecord();

public:
#ifdef	MULTITHREAD_SAFE
	static CCriticalSection	m_xCtrl;
#endif
#ifdef	USE_NEW
	static CMyHeap	s_heapString;
#endif
	static CMyRecord* CreateNew(CMyDatabase  *pMyDatabase);

public:
	MYHEAP_DECLARATION(s_heap)

public:
	enum { modeEdit, modeAddNew, modeDelete };

	virtual bool Open( CMyRecordset* pRes, DWORD idNewKey = 0);	
	virtual void Close();

	int FieldsCount();
	bool IsOpen();

	bool CanUpdate() const;     // Can Edit/AddNew/Delete be called?
	void ClsEditFlag();

	CMyDatabase	*	m_pMyDatabase;
protected: // inside function
	bool			Spawn(CMyRecord  *pMyRecord, DWORD idNewKey = 0);
	int				UpdateFieldsCount();

	void			BuildUpdateSQL();
	void			BuildUpdateCondition();
	void			BuildUpdateOpration();

	bool			Query(char * szSQL, MYSQL_RES * * res);

	void			GetFieldValue(UINT uiIndex);
	bool			FormatField(char* szFormat, const CMyField& objField);

protected:
	MYSQL_FIELD*	m_Fields;
	unsigned int 	m_uiFieldsCount;

	char	  		m_szKey[16];
	unsigned int 	m_uiKeyIndex;
	char		 	m_szKeyData[256];

	bool	*		m_pbDirty;
	bool 			m_bEdit;
	bool 			m_bIsOpen;
	int				m_nEditMode;
	CMyField	*	m_objFields;

	char 			m_szTableName[32];
	char 			m_szUpdateSQL[1024];
	char			m_szUpdateOpration[1024];
	char			m_szUpdateCondition[128];

	bool			m_bCanUpdate;
};

#endif // MYRECORD_H