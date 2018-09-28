// UserTitleData.h: interface for the CUserTitleData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTITLEDATA_H__8C9E6A3C_D8B0_4AE3_AADB_541430242C34__INCLUDED_)
#define AFX_USERTITLEDATA_H__8C9E6A3C_D8B0_4AE3_AADB_541430242C34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include "MyHeap.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"

//////////////////////////////////////////////////////////////////////
enum TITLEDATA{
	TITLEDATA_ID_=0,
	TITLEDATA_OWNERID=1,
	TITLEDATA_TYPE,
	TITLEDATA_LEVEL,
};

char szTitleTable[];

struct ST_TITLE_DATA {
	OBJID	idTitle;
	OBJID	idOwner;
	USHORT	usType;
	USHORT	usLevel;
};
typedef	CGameData<TITLEDATA, szTitleTable, szID>	CTitleDataBase;

class CUserTitleData  
{
public:
	CUserTitleData();
	virtual ~CUserTitleData();

public:
	static CUserTitleData*	CreateNew()		{ return new CUserTitleData; }
	void	ReleaseByOwner()		{ delete this; }

	bool	Create(IRecordset* pRecordset);
	bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);
	bool	Create(ST_TITLE_DATA* pTitleData);
	OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
	bool	Update()			{ CHECKF(m_pData); return m_pData->Update(); }
	void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }
	int		GetInt(TITLEDATA nDataName);
	void	SetInt(TITLEDATA idx, int nData);

protected:
	CTitleDataBase*	m_pData;
	ST_TITLE_DATA	m_data;
};

#endif // !defined(AFX_USERTITLEDATA_H__8C9E6A3C_D8B0_4AE3_AADB_541430242C34__INCLUDED_)
