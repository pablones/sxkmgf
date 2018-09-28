// PetEggData.h: interface for the CPetEggData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PETEGGDATA_H__9A2DE666_F2FE_462E_A2A3_8E3CA32EF3B5__INCLUDED_)
#define AFX_PETEGGDATA_H__9A2DE666_F2FE_462E_A2A3_8E3CA32EF3B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"

enum PETEGGDATA{
	PETEGGDATA_ID=0,
	PETEGGDATA_OWNERID,
	PETEGGDATA_ITEMID,//对应的物品ID
	PETEGGDATA_GETTYPE,//档次
	PETEGGDATA_GROW,//成长
	PETEGGDATA_STRCHG,//力量 资质=pow(1.06f, gettype) * chg * 悟性加成
	PETEGGDATA_INTCHG,//灵力
	PETEGGDATA_STACHG,//体力
	PETEGGDATA_SPICHG,//定力
	PETEGGDATA_AGICHG,//身法
	PETEGGDATA_NEXTFLUSH,//下次刷新时间
};
//切地图组用
struct EggInfoStruct
{
	OBJID	id;
	OBJID	idUser;
	OBJID	idItem;
	USHORT	usGetType;
	USHORT	usGrow;
	USHORT	usStrChg;
	USHORT	usIntChg;
	USHORT	usStaChg;
	USHORT	usSpiChg;
	USHORT	usAgiChg;
	DWORD	dwNextFlush;
};

char szPeteggTable[];
typedef	CGameData<PETEGGDATA, szPeteggTable, szID>	CGamePeteggData;

class CPetEggData  
{
public:
	CPetEggData();
	virtual ~CPetEggData();

public:
	static  CPetEggData*	CreateNew()		{ return new CPetEggData; }
	void	ReleaseByOwner()		{ delete this; }

	bool	Create(IRecordset* pRecordset);
	bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);

	OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
	bool	Update()			{ CHECKF(m_pData); return m_pData->Update(); }
	void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }

	int		GetInt(PETEGGDATA nDataName);
	void	SetInt(PETEGGDATA idx, int nData, bool bUpdate=false);

	bool	NeedFlush();
	LONG	GetNextFlushSeconds();
protected:
	CGamePeteggData*	m_pData;
};

#endif // !defined(AFX_PETEGGDATA_H__9A2DE666_F2FE_462E_A2A3_8E3CA32EF3B5__INCLUDED_)
