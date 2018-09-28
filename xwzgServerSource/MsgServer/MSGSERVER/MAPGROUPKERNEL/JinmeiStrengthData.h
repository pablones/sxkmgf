// JinmeiStrengthData.h: interface for the CJinmeiStrengthData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JINMEISTRENGTHDATA_H__E2EB58BF_A42A_4717_85A0_ED985FDEA256__INCLUDED_)
#define AFX_JINMEISTRENGTHDATA_H__E2EB58BF_A42A_4717_85A0_ED985FDEA256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "T_SingleObjSet2.h"
using namespace std;

enum JINMEISTRENGTHDATA
{
	JINMEISTRENGTHDATA_ID=0,
    JINMEISTRENGTHDATA_LEV,
	JINMEISTRENGTHDATA_FAILLEV,
    JINMEISTRENGTHDATA_SUCRATE,
	JINMEISTRENGTHDATA_ADDRATE,
	JINMEISTRENGTHDATA_ITEMTYPE,
	JINMEISTRENGTHDATA_REQMONEY,
	JINMEISTRENGTHDATA_PROTECITEMTYPE,
};

char	szJinmeiStengthTable[];
typedef	CGameData<JINMEISTRENGTHDATA, szJinmeiStengthTable, szID>	CGameJinmeiStrengthData;

class CJinmeiStrengthData
{
public:
	CJinmeiStrengthData()			{ m_pData = NULL; }
	virtual ~CJinmeiStrengthData()	{ if (m_pData) SAFE_RELEASE (m_pData); }
	
public:
	static CJinmeiStrengthData*	CreateNew	()	{ return new CJinmeiStrengthData; }
	ULONG	Release			()				{ delete this; return 0; }
	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	bool	Create			(IRecordset* pRes);
	
public: // get set
	OBJID		GetID()					{ CHECKF(m_pData); return m_pData->GetKey(); }
	
	int		GetInt(JINMEISTRENGTHDATA idx)	{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(JINMEISTRENGTHDATA idx)	{ CHECKF(m_pData); return m_pData->GetStr(idx); } 
	
protected:
	CGameJinmeiStrengthData*		m_pData;
};

//////////////////////////////////////////////////////////////////////

typedef	ISingleObjSet2<CJinmeiStrengthData>		IJinmeiStrengthTypeSet;
typedef	CSingleObjSet2<CJinmeiStrengthData>		CJinmeiStrengthTypeSet;

enum JINMEIEFFECTDATA
{
	JINMEIEFFECTDATA_ID = 0,
	JINMEIEFFECTDATA_TYPE,
	JINMEIEFFECTDATA_GRADE,
	JINMEIEFFECTDATA_MINLEV,
	JINMEIEFFECTDATA_MAXLEV,
	JINMEIEFFECTDATA_LEARNNUM,
	JINMEIEFFECTDATA_WAITNUM,
	JINMEIEFFECTDATA_EFFECT1,
	JINMEIEFFECTDATA_EFFECT2,
	JINMEIEFFECTDATA_EFFECT3,
	JINMEIEFFECTDATA_EFFECT4,
};

char	szJinmeiEffectTable[];
typedef	CGameData<JINMEIEFFECTDATA, szJinmeiEffectTable, szID>	CGameJinmeiEffectData;

class CJinmeiEffectData
{
public:
	CJinmeiEffectData()			{ m_pData = NULL; }
	virtual ~CJinmeiEffectData()	{ if (m_pData) SAFE_RELEASE (m_pData); }
	
public:
	static CJinmeiEffectData*	CreateNew	()	{ return new CJinmeiEffectData; }
	ULONG	Release			()				{ delete this; return 0; }
	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	bool	Create			(IRecordset* pRes);
	
public: // get set
	OBJID		GetID()					{ CHECKF(m_pData); return m_pData->GetKey(); }
	
	int		GetInt(JINMEIEFFECTDATA idx)	{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(JINMEIEFFECTDATA idx)	{ CHECKF(m_pData); return m_pData->GetStr(idx); } 
	
protected:
	CGameJinmeiEffectData*		m_pData;
};

//////////////////////////////////////////////////////////////////////

typedef	ISingleObjSet2<CJinmeiEffectData>		IJinmeiEffectTypeSet;
typedef	CSingleObjSet2<CJinmeiEffectData>		CJinmeiEffectTypeSet;

#endif // !defined(AFX_JINMEISTRENGTHDATA_H__E2EB58BF_A42A_4717_85A0_ED985FDEA256__INCLUDED_)
