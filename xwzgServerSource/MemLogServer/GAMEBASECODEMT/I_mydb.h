// ���ݿ�ײ��ͨ�ýӿ�
//////////////////////////////////////////////////////////////////


#ifndef	I_MYDB_H
#define	I_MYDB_H

#include "..\COMMON\common.h"

#define PURE_VIRTUAL_DECLARATION_DB		PURE_VIRTUAL_FUNCTION
#define PURE_VIRTUAL_DECLARATION_DB_0	PURE_VIRTUAL_FUNCTION_0

/*
	˵����Ϊ���ݾ�ϵͳ�����������������ϵͳ��ͬ��
*/

class	CMyRecordset;
class	CMyRecord;
class	IRecordset;
class	IDatabase
{
protected:
	virtual ~IDatabase() {}
public:
	virtual IRecordset*	CreateNewRecordset(LPCTSTR szSQL, bool bNoEmpty = true)		PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB
public:
	virtual bool	ExecuteSQL	(char* szSQL)				PURE_VIRTUAL_DECLARATION_DB_0
	virtual int		GetInsertId	()						PURE_VIRTUAL_DECLARATION_DB_0
	virtual int		CountRecord	(const char* szSQL)		PURE_VIRTUAL_DECLARATION_DB_0
};

const int		NEWKEY_NONE	= (DWORD)-1;
class	IRecord
{
protected:
	virtual ~IRecord() {}
public:
	virtual IRecord*	CreateNewRecord(DWORD idNewKey = NEWKEY_NONE)	PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB

	// ע��Ŀǰֻ��CMyRecord֧����InsertRecord()�����UpdateRecord()��
	virtual DWORD	InsertRecord()								PURE_VIRTUAL_DECLARATION_DB_0
	virtual bool	UpdateRecord()								PURE_VIRTUAL_DECLARATION_DB_0
	virtual bool	DeleteRecord()								PURE_VIRTUAL_DECLARATION_DB_0

	virtual int		LoadInt		(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetInt		(LPCTSTR szField, int nData)		PURE_VIRTUAL_DECLARATION_DB
	virtual UINT	LoadUInt	(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetUInt		(LPCTSTR szField, UINT nData)		PURE_VIRTUAL_DECLARATION_DB
	virtual void	LoadString	(char* szData, LPCTSTR szField, int nSize)		PURE_VIRTUAL_DECLARATION_DB
	virtual void	SetString	(LPCTSTR szField, LPCTSTR szData, int nSize)	PURE_VIRTUAL_DECLARATION_DB
	virtual DWORD	LoadDWord	(LPCTSTR szField)					{ return LoadUInt(szField); }
	virtual void	SetDWord	(LPCTSTR szField, DWORD nData)		{ SetUInt(szField, nData); }
public:
	// ��ģʽ�Ľӿ�
	virtual	_int64   GetInt64	(int idx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual _int64	GetInt64	(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetInt64	(int nIdx,			_int64 i64Data)			PURE_VIRTUAL_DECLARATION_DB
	virtual void	SetInt64	(LPCTSTR szField,	_int64 i64Data)			PURE_VIRTUAL_DECLARATION_DB
	virtual double	GetDouble	(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual int		GetInt		(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual int     GetInt		(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0 ///������������չ,�����ǹ�
	virtual	LPCTSTR		GetStr(LPCTSTR szField)							PURE_VIRTUAL_DECLARATION_DB_0 ///ͬ��
	virtual LPCTSTR	GetStr		(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetInt		(int nIdx, int nData)				PURE_VIRTUAL_DECLARATION_DB
	virtual void	SetStr		(int nIdx, LPCTSTR szData, int nSize)			PURE_VIRTUAL_DECLARATION_DB
	virtual	void	SetStr		(LPCTSTR szField,LPCTSTR szData, int nSize)	PURE_VIRTUAL_DECLARATION_DB
	virtual void	ClearUpdateFlags	()							PURE_VIRTUAL_DECLARATION_DB
};

class	IRecordset : public IRecord
{
protected:
	virtual ~IRecordset() {}
public:
	virtual UINT		RecordCount()			PURE_VIRTUAL_DECLARATION_DB_0		// return -1: error
	virtual bool		MoveNext()				PURE_VIRTUAL_DECLARATION_DB_0		// return false: no more data
	virtual void		MoveFirst()				PURE_VIRTUAL_DECLARATION_DB			//�ƶ�����һ����¼
};

template<TYPENAME T>
class	IGameData
{
protected:
	virtual ~IGameData() {}
public:
	virtual	void		Release()											PURE_VIRTUAL_DECLARATION_DB
	virtual	DWORD		GetID()												PURE_VIRTUAL_DECLARATION_DB_0
public:
	virtual	int			GetInt(T nDataName)									PURE_VIRTUAL_DECLARATION_DB_0
	virtual	LPCTSTR		GetStr(T nDataName)									PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void		SetInt(T nDataName, int nData)						PURE_VIRTUAL_DECLARATION_DB
	virtual	void		SetStr(T nDataName, LPCTSTR szData, int nSize)		PURE_VIRTUAL_DECLARATION_DB
public:
	virtual	bool		Create(DWORD id, IDatabase* pDb)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual	bool		Create(IRecordset* pRecordset)						PURE_VIRTUAL_DECLARATION_DB_0
	virtual	bool		Create(IRecord* pDefaultRes, DWORD id)				PURE_VIRTUAL_DECLARATION_DB_0
	virtual	bool		Update()											PURE_VIRTUAL_DECLARATION_DB_0
	virtual	DWORD		InsertRecord()										PURE_VIRTUAL_DECLARATION_DB_0
	virtual	bool		DeleteRecord()										PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void		ClearUpdateFlags()									PURE_VIRTUAL_DECLARATION_DB

	virtual _int64			GetInt64(T nDataName)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void			SetInt64(T nDataName, __int64 i64Data)			PURE_VIRTUAL_DECLARATION_DB
};

extern IDatabase*	CreateDatabase(char* szType);
extern BOOL	ReconnectDatabase(IDatabase* pDb);

// ���ݿ��������
#define		_TBL_PREHEAD			"wb_"

#define		_TBL_USER				_TBL_PREHEAD "user"
#define		_TBL_CARD				_TBL_PREHEAD "card"
#define		_TBL_ACTIVE				_TBL_PREHEAD "active"
#endif // I_MYDB_H