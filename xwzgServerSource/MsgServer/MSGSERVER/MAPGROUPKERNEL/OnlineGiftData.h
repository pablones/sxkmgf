// TreasureData.h: interface for the CTreasureData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREASUREDATA_H__374A4CC1_64CD_460E_AFCA_24C298B4B24B__INCLUDED_)
#define AFX_TREASUREDATA_H__374A4CC1_64CD_460E_AFCA_24C298B4B24B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include "MyHeap.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"

//////////////////////////////////////////////////////////////////////
enum GIFTDATA{
	GIFTDATA_ID=0,
	GIFTDATA_USERID,
	GIFTDATA_GIFT1ID,
	GIFTDATA_GIFT2ID,
	GIFTDATA_GIFT3ID,
	GIFTDATA_GIFT4ID,
	GIFTDATA_GIFT5ID,
	GIFTDATA_GIFT6ID,
	GIFTDATA_DATE,
	GIFTDATA_TASKMASK,
	GIFTDATA_AMOUNT
};

char szGiftTable[];
typedef	CGameData<GIFTDATA, szGiftTable, szID>	CGameGiftData;

class COnlineGiftData  
{
public:
	COnlineGiftData();
	virtual ~COnlineGiftData();
	
public:
	static COnlineGiftData*	CreateNew()		{ return new COnlineGiftData; }
	void	ReleaseByOwner()		{ delete this; }
	
	bool	Create(IRecordset* pRecordset);
	bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);

	OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
	bool	Update()			{ CHECKF(m_pData); return m_pData->Update(); }

	void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }

	int		GetInt(GIFTDATA nDataName);
	void	SetInt(GIFTDATA idx, int nData);
	
	OBJID	GetGift1ID()			{ return GetInt(GIFTDATA_GIFT1ID); }
	OBJID	GetGift2ID()			{ return GetInt(GIFTDATA_GIFT2ID); }
	OBJID	GetGift3ID()			{ return GetInt(GIFTDATA_GIFT3ID); }
	OBJID	GetGift4ID()			{ return GetInt(GIFTDATA_GIFT4ID); }
	OBJID	GetGift5ID()			{ return GetInt(GIFTDATA_GIFT5ID); }
	OBJID	GetGift6ID()			{ return GetInt(GIFTDATA_GIFT6ID); }
	int		GetDate()				{ return GetInt(GIFTDATA_DATE); }
	int		GetTaskMask()			{ return GetInt(GIFTDATA_TASKMASK);}
	int		GetAmount()				{ return GetInt(GIFTDATA_AMOUNT);}

	
	void	SetGift1ID(int nData)			{ SetInt(GIFTDATA_GIFT1ID,nData); }
	void	SetGift2ID(int nData)			{ SetInt(GIFTDATA_GIFT2ID,nData); }
	void	SetGift3ID(int nData)			{ SetInt(GIFTDATA_GIFT3ID,nData); }
	void	SetGift4ID(int nData)			{ SetInt(GIFTDATA_GIFT4ID,nData); }
	void	SetGift5ID(int nData)			{ SetInt(GIFTDATA_GIFT5ID,nData); }
	void	SetGift6ID(int nData)			{ SetInt(GIFTDATA_GIFT6ID,nData); }
	void	SetDate(int nData)				{ SetInt(GIFTDATA_DATE,nData); }
	void	SetTaskMask(int nData)			{ SetInt(GIFTDATA_TASKMASK,nData);}
	void	SetAmount(int nData)			{SetInt(GIFTDATA_AMOUNT,nData);}
	
	void	AddAmount()				{SetAmount(GetAmount()+1);}
	void	AddAmount(int nData)				{SetAmount(GetAmount()+nData);}
protected:
	CGameGiftData*	m_pData;
};

#endif // !defined(AFX_TREASUREDATA_H__374A4CC1_64CD_460E_AFCA_24C298B4B24B__INCLUDED_)
