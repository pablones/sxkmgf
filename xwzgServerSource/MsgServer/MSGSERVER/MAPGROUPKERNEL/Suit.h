// Suit.h: interface for the CSuit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUIT_H__1DFAAB38_E65E_46DC_9D1F_E0F2D4325781__INCLUDED_)
#define AFX_SUIT_H__1DFAAB38_E65E_46DC_9D1F_E0F2D4325781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleMap2.h"

enum ISUITDATA
{
	ISUITDATA_ID = 0,
	ISUITDATA_SUIT_NUM,
	ISUITDATA_COLLECT_ADDITION1,
	ISUITDATA_COLLECT_ADDITION2,
	ISUITDATA_COLLECT_ADDITION3,
	ISUITDATA_COLLECT_ADDITION4,
	ISUITDATA_COLLECT_ADDITION5,
	ISUITDATA_COLLECT_ADDITION6,
	ISUITDATA_COLLECT_ADDITION7,
	ISUITDATA_COLLECT_ADDITION8,
	ISUITDATA_COLLECT_ADDITION9,
	ISUITDATA_COLLECT_ADDITION10,
	ISUITDATA_COLLECT_ADDITION11,
	ISUITDATA_STRENGTHAN_ADDITION11,
	ISUITDATA_STRENGTHAN_ADDITION12,
	ISUITDATA_STRENGTHAN_ADDITION13,
	ISUITDATA_STRENGTHAN_ADDITION14,
	ISUITDATA_STRENGTHAN_ADDITION15,
};

struct SUITDATA
{
	DWORD	dwNum;
	OBJID	idAddction[16];
};

struct SUITINFO
{
	OBJID id;
	DWORD dwNum;
//	DWORD dwStrengthan;
//	DWORD aStrengNum[15];

	SUITINFO()
	{
		memset(this, 0, sizeof(SUITINFO));
	}
};

struct PHASEINFO
{
	DWORD dwPhase;
	DWORD dwNum;
	DWORD dwStrengthan;
	DWORD aStrengNum[15];
	
	PHASEINFO()
	{
		memset(this, 0, sizeof(PHASEINFO));
	}
};

char szSuitTable[];
typedef CConstGameData<ISUITDATA, szSuitTable, ISUITDATA_ID> CSuitData;


typedef	ISingleMap2<CSuitData> ISuitSet;
typedef	CSingleMap2<CSuitData> CSuitSet;

class CSuit  
{
public:
	void GetDataByInfo(SUITINFO *pInfo, SUITDATA *pData);
	int GetDataByInfo(map<int, PHASEINFO*> mapPhaseInfo, SUITDATA *pData);
	CSuit();
	virtual ~CSuit();
	static CSuit* CreateNew() {return new CSuit;}
	bool Create(IDatabase* pDb);
	ULONG Release() {delete this; return NULL;}
protected:
	ISuitSet*	m_setSuit;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_SUIT_H__1DFAAB38_E65E_46DC_9D1F_E0F2D4325781__INCLUDED_)
