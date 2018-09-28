// NpcGenerator.h: interface for the CNpcGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCGENERATOR_H__4DC34AE2_8F6B_41C0_9499_D4207B8163DC__INCLUDED_)
#define AFX_NPCGENERATOR_H__4DC34AE2_8F6B_41C0_9499_D4207B8163DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameData.h"
#include "GameObj.h"
#include "GameObjSet.h"
#include "TimeOut.h"

enum NPCGENDATA
{
	NPCGENDATA_ID=0,
	NPCGENDATA_MAPID=1,
	NPCGENDATA_BOUNDX, 
	NPCGENDATA_BOUNDY, 
	NPCGENDATA_BOUNDCX, 
	NPCGENDATA_BOUNDCY,
	NPCGENDATA_RESTTYPE,
	NPCGENDATA_RESTSECS,
	NPCGENDATA_MAXNPC,
	NPCGENDATA_MAXPERGEN,
	NPCGENDATA_NPCTYPE, 
	NPCGENDATA_BEGIN,
	NPCGENDATA_END,
	NPCGENDATA_INITSECS,
	NPCGENDATA_GENTYPE,
};

char	szGenerator[];
typedef CGameData<NPCGENDATA,szGenerator,szID>	CNpcGenData;

class CNpcGenerator
{
public:
	CNpcGenerator()	{ m_pData = NULL;m_nGenNum = 0; }
	virtual ~CNpcGenerator()	{ if (m_pData) SAFE_RELEASE (m_pData); }

public:
	static CNpcGenerator*	CreateNew	()	{ return new CNpcGenerator; }
	ULONG	Release			()				{ delete this; return 0; }
	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	bool	Create			(IRecordset* pRes);
	int     GetGenNum()		{ return m_nGenNum;}
	void	AddGenNum()		{ m_nGenNum++; if(GetInt(NPCGENDATA_RESTTYPE) == 1) UpdateTime();}
	void	DecGenNum()		{ if(m_nGenNum > 0) m_nGenNum--; if(GetInt(NPCGENDATA_RESTTYPE) == 2) UpdateTime();}
	bool	IsTimeOut();
	void	UpdateTime();
public: // get set
	OBJID		GetID()					{ CHECKF(m_pData); return m_pData->GetKey(); }	
	int		GetInt(NPCGENDATA idx)		{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(NPCGENDATA idx)		{ CHECKF(m_pData); return m_pData->GetStr(idx); } 

protected:
	CNpcGenData* m_pData;
	int m_nGenNum;
	CTimeOut	m_timeoutRest;
// public:
// 	MYHEAP_DECLARATION(s_heap)
};

typedef	CGameObjSet<CNpcGenerator>	INpcGenSet;
typedef	CGameObjSet<CNpcGenerator>	CNpcGenSet;
#endif // !defined(AFX_NPCGENERATOR_H__4DC34AE2_8F6B_41C0_9499_D4207B8163DC__INCLUDED_)
