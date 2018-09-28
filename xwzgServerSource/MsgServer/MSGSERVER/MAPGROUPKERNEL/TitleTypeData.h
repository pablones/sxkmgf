// TitleTypeData.h: interface for the CTitleTypeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TITLETYPEDATA_H__3BBABE75_1D09_463B_AE83_5B60C09C238B__INCLUDED_)
#define AFX_TITLETYPEDATA_H__3BBABE75_1D09_463B_AE83_5B60C09C238B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleObjSet2.h"

enum TITLETYPEDATA{
	TITLETYPEDATA_ID_=0,
	TITLETYPEDATA_TYPE,		//称号ID
	TITLETYPEDATA_SORT,		//称号类别(师门,帮派等)
	TITLETYPEDATA_EXTEND,	//类型(冲突,继承)
	TITLETYPEDATA_MAXLEV,	//等级上限
};
char	szTitleTypeTable[];
typedef	CGameData<TITLETYPEDATA, szTitleTypeTable, szID>	CGameTitleTypeData;

class CTitleTypeData  
{
public:
	CTitleTypeData()			{ m_pData = NULL; }
	virtual ~CTitleTypeData()	{ if (m_pData) SAFE_RELEASE (m_pData); }

public:
	static  CTitleTypeData*	CreateNew	()	{ return new CTitleTypeData; }
	ULONG	Release			()				{ delete this; return 0; }
	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	bool	Create			(IRecordset* pRes/*, IDatabase* pDb = NULL*/);
public: // get set
	OBJID		GetID()					{ CHECKF(m_pData); return m_pData->GetKey(); }
	int		GetInt(TITLETYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(TITLETYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetStr(idx); }
	int		GetMaxLevel();
	int		GetExtend();
protected:
	CGameTitleTypeData*		m_pData;
public:
	MYHEAP_DECLARATION(s_heap)
};
//////////////////////////////////////////////////////////////////////
typedef	ISingleObjSet2<CTitleTypeData>		ITitleTypeSet;
typedef	CSingleObjSet2<CTitleTypeData>		CTitleTypeSet;

#endif // !defined(AFX_TITLETYPEDATA_H__3BBABE75_1D09_463B_AE83_5B60C09C238B__INCLUDED_)
