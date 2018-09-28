// NpcType.h: interface for the CNpcType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCTYPE_H__2989E1DC_47E1_48E6_95B2_93C1B9C1DDAF__INCLUDED_)
#define AFX_NPCTYPE_H__2989E1DC_47E1_48E6_95B2_93C1B9C1DDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleObjSet2.h"
#include <vector>
using namespace std;

enum SATICNPCTYPEDATA
{
	SATICNPCTYPEDATA_ID,
	SATICNPCTYPEDATA_NAME = 1,
	SATICNPCTYPEDATA_LOOKFACE,
	SATICNPCTYPEDATA_MAPID,
	SATICNPCTYPEDATA_POSX_,
	SATICNPCTYPEDATA_POSY_,
	SATICNPCTYPEDATA_SORT,
	SATICNPCTYPEDATA_TYPE,
	SATICNPCTYPEDATA_ACTION,
	SATICNPCTYPEDATA_LEVEL,
	SATICNPCTYPEDATA_SHOWTIME,
	SATICNPCTYPEDATA_HIDETIME,
	SATICNPCTYPEDATA_SHOWSPAN,
	SATICNPCTYPEDATA_HIDEMSPAN,
};

char	szNpcTypeTable[];
typedef	CGameData<SATICNPCTYPEDATA, szNpcTypeTable, szID>	CNpcTypeData;

class CStaticNpcType  
{
public:
	CStaticNpcType()				{ m_pDataNpc = NULL; }
	virtual ~CStaticNpcType() 	{ if (m_pDataNpc) SAFE_RELEASE (m_pDataNpc); }

public:
	static CStaticNpcType*	CreateNew	()	{ return new CStaticNpcType; }
	ULONG	Release			()				{ delete this; return 0; }
	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	bool	Create(IRecordset* pRes);
	
public: // get set
	OBJID		GetID()					{ CHECKF(m_pDataNpc); return m_pDataNpc->GetKey(); }
	
	int		GetInt(SATICNPCTYPEDATA idx)	{ CHECKF(m_pDataNpc); return m_pDataNpc->GetInt(idx); }
	LPCTSTR	GetStr(SATICNPCTYPEDATA idx)	{ CHECKF(m_pDataNpc); return m_pDataNpc->GetStr(idx); } 
	
protected:
	CNpcTypeData* 		m_pDataNpc;
public:
	MYHEAP_DECLARATION(s_heap)
};

//////////////////////////////////////////////////////////////////////

typedef	ISingleObjSet2<CStaticNpcType>		INpcTypeSet;
typedef	CSingleObjSet2<CStaticNpcType>		CNpcTypeSet;

#endif // !defined(AFX_NPCTYPE_H__2989E1DC_47E1_48E6_95B2_93C1B9C1DDAF__INCLUDED_)
