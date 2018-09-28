// TreasureType.h: interface for the CTreasureType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREASURETYPE_H__CC0B7245_1C69_44B9_A98F_DF0E2EFDAECE__INCLUDED_)
#define AFX_TREASURETYPE_H__CC0B7245_1C69_44B9_A98F_DF0E2EFDAECE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleObjSet2.h"

enum TREASURETYPE{
	TREASURETYPE_ID_=0,
	TREASURETYPE_ITEMTYPE,		//物品类型
	TREASURETYPE_DIVIDE,		//物品分类 1:药品,2:装备,3:宝石,4:宠物,5:杂货
	TREASURETYPE_AMOUNT,		//物品数量
	TREASURETYPE_GROUP,			//物品集ID
	TREASURETYPE_START,			//随机数上区间
	TREASURETYPE_END,			//随机数下区间
	TREASURETYPE_BROADCAST,		//是否需要广播
};
char	szTreasureTypeTable[];
typedef	CGameData<TREASURETYPE, szTreasureTypeTable, szID>	CGameTreasureType;

class CTreasureType  
{
public:
	CTreasureType()				{ m_pData = NULL; }
	virtual ~CTreasureType()	{ if (m_pData) SAFE_RELEASE (m_pData); }

public:
	static  CTreasureType*	CreateNew	()	{ return new CTreasureType; }
	ULONG	Release			()				{ delete this; return 0; }
	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	bool	Create			(IRecordset* pRes/*, IDatabase* pDb = NULL*/);
public: // get set
	OBJID	GetID()						{ CHECKF(m_pData); return m_pData->GetKey(); }
	int		GetInt(TREASURETYPE idx)	{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(TREASURETYPE idx)	{ CHECKF(m_pData); return m_pData->GetStr(idx); }
	OBJID	GetTypeID()					{ CHECKF(m_pData); return m_pData->GetInt(TREASURETYPE_ITEMTYPE); }
	int		GetDivide()					{ CHECKF(m_pData); return m_pData->GetInt(TREASURETYPE_DIVIDE); }
	int		GetAmount()					{ CHECKF(m_pData); return m_pData->GetInt(TREASURETYPE_AMOUNT); }
	bool	isBroadcast()				{ CHECKF(m_pData); return m_pData->GetInt(TREASURETYPE_BROADCAST)>0; }
	int		GetMaxLevel();
	int		GetExtend();
protected:
	CGameTreasureType*		m_pData;
public:
	MYHEAP_DECLARATION(s_heap)
};
//////////////////////////////////////////////////////////////////////
typedef	ISingleObjSet2<CTreasureType>		ITreasureTypeSet;
typedef	CSingleObjSet2<CTreasureType>		CTreasureTypeSet;

#endif // !defined(AFX_TREASURETYPE_H__CC0B7245_1C69_44B9_A98F_DF0E2EFDAECE__INCLUDED_)
