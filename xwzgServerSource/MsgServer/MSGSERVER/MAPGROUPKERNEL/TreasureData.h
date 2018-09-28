// TreasureData.h: interface for the CTreasureData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREASUREDATA_H__374A4CC1_64CD_460E_AFCA_24C298B4B24A__INCLUDED_)
#define AFX_TREASUREDATA_H__374A4CC1_64CD_460E_AFCA_24C298B4B24A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include "MyHeap.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"

//////////////////////////////////////////////////////////////////////
enum TREASUREDATA{
	TREASUREDATA_ID=0,
	TREASUREDATA_OWNERID=1,
	TREASUREDATA_ITEMTYPE,
	TREASUREDATA_AMOUNT,
	TREASUREDATA_DIVIDE,
};

char szTreasureTable[];
typedef	CGameData<TREASUREDATA, szTreasureTable, szID>	CGameTreasureData;

class CTreasureData  
{
public:
	CTreasureData();
	virtual ~CTreasureData();

public:
	static CTreasureData*	CreateNew()		{ return new CTreasureData; }
	void	ReleaseByOwner()		{ delete this; }
	
	bool	Create(IRecordset* pRecordset);
	bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);
	OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
	bool	Update()			{ CHECKF(m_pData); return m_pData->Update(); }
	void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }
	int		GetInt(TREASUREDATA nDataName);
	void	SetInt(TREASUREDATA idx, int nData);

	OBJID	GetTypeID()			{ CHECKF(m_pData); return m_pData->GetInt(TREASUREDATA_ITEMTYPE); }
	int		GetDivide()			{ CHECKF(m_pData); return m_pData->GetInt(TREASUREDATA_DIVIDE); }
	int		GetAmount()			{ CHECKF(m_pData); return m_pData->GetInt(TREASUREDATA_AMOUNT); }
	void	SetAmount(int nAmount)	{ CHECK(m_pData); m_pData->SetInt(TREASUREDATA_AMOUNT, nAmount); }
	
protected:
	CGameTreasureData*	m_pData;
};

#endif // !defined(AFX_TREASUREDATA_H__374A4CC1_64CD_460E_AFCA_24C298B4B24A__INCLUDED_)
