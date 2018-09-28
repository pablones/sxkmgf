// ConstGameData.h: interface for the CGameMapData class.
// paled, modify in 2003.8.26
//////////////////////////////////////////////////////////////////////

#if !defined(CONST_GAMEDATA_HEAD3)
#define CONST_GAMEDATA_HEAD3

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "I_mydb.h"

//////////////////////////////////////////////////////////////////////
template<TYPENAME T, char* TBL, T KEY>
class CConstGameData : IGameData<T>
{
protected:
	CConstGameData()										{ m_pRecord = NULL; m_idKey = ID_NONE; }
	virtual ~CConstGameData()								{ if(m_pRecord) m_pRecord->Release(); }
public: // construct
	static CConstGameData*	CreateNew()						{ return new CConstGameData; }
	virtual void		Release()						{ delete this; }
	IGameData*			QueryInterface()				{ return (IGameData*)this; }
public: // by set
	virtual void		ReleaseByOwner()				{ delete this; }
	OBJID		GetID()									{ return m_idKey; }
public:
	OBJID		GetKey()								{ return m_idKey; }
	virtual	int			GetInt(T nDataName)						{ return m_pRecord->GetInt(nDataName); }
	virtual LPCTSTR		GetStr(T nDataName)						{ return m_pRecord->GetStr(nDataName); }
public:
	bool		Create(OBJID id, IDatabase* pDb);				// 装入数据
	bool		Create(IRecordset* pRecordset/*, IDatabase* pDb = NULL*/)			{ m_pRecord = pRecordset->CreateNewConstRecord(); 
															if(m_pRecord) m_idKey	= m_pRecord->GetInt(KEY); 
															return m_pRecord != NULL; }
	IRecord*	GetRecord()								{ ASSERT(m_pRecord); return m_pRecord; }
protected:
	IRecord*	m_pRecord;
	OBJID		m_idKey;
};

template<TYPENAME T, char* TBL, T KEY>
bool CConstGameData<T,TBL,KEY>::Create(OBJID id, IDatabase* pDb)
{
	CHECKF(pDb);

	char	szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s WHERE %s=%u LIMIT 1", TBL, KEY, id);

	IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
	m_pRecord	= pRes->CreateNewRecord();
	pRes->Release();

	if(m_pRecord)
		m_idKey	= m_pRecord->GetInt(KEY);

	return m_pRecord != NULL;
}


#endif // !defined(CONST_GAMEDATA_HEAD3)
