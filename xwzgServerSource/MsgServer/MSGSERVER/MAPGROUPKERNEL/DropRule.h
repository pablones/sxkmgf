// DropRule.h: interface for the CDropRule class.
//
// Àä×ÏÁú 2003-11-15
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPRULE_H__1A26CA92_A4A3_42C6_81F9_DD1A70FA5E38__INCLUDED_)
#define AFX_DROPRULE_H__1A26CA92_A4A3_42C6_81F9_DD1A70FA5E38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "../share/define.h"
/*
#include "ConstGameData.h"
#include "GameData.h"
#include "GameObj.h"
#include "T_SingleMap2.h"
*/
#include "Myheap.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////
/*
const int _MAX_DROPRULE_ITEMTYPE_NUM	= 15;

struct RuleInfoStruct
{
	OBJID	id;
	int		nChance;
	OBJID	idItemType[_MAX_DROPRULE_ITEMTYPE_NUM];
};
*/
//////////////////////////////////////////////////////////////////////

enum DROPRULEDATA
{
	DROPRULEDATA_ID_	= 0,
	DROPRULEDATA_RULEID	= 1,
	DROPRULEDATA_MAXMUN,
	DROPRULEDATA_CHANCE0,
	DROPRULEDATA_ITEM0,
	DROPRULEDATA_CHANCE1,
	DROPRULEDATA_ITEM1,
	DROPRULEDATA_CHANCE2,
	DROPRULEDATA_ITEM2,
	DROPRULEDATA_CHANCE3,
	DROPRULEDATA_ITEM3,
	DROPRULEDATA_CHANCE4,
	DROPRULEDATA_ITEM4,
	DROPRULEDATA_CHANCE5,
	DROPRULEDATA_ITEM5,
	DROPRULEDATA_CHANCE6,
	DROPRULEDATA_ITEM6,
	DROPRULEDATA_CHANCE7,
	DROPRULEDATA_ITEM7,
};
struct DORPITEMDATA
{
	int nChance;
	OBJID idItemType;
};
//////////////////////////////////////////////////////////////////////
class CDropRule  
{
public:
	CDropRule();
	virtual ~CDropRule();

protected:
	typedef	std::vector<DORPITEMDATA> DROPITEMTYPE_SET;

	OBJID			m_idRule;
	int				m_nMaxItem;
	DROPITEMTYPE_SET	m_setItemType;
	OBJID			m_id;
public:
	static	CDropRule*	CreateNew()	{ return new CDropRule; }
	ULONG	Release()	{ delete this; return 0; }

	bool	Create(IRecordset*	pRes);
	bool	LoadInfo(IRecord*	pRes);
	DWORD	GetDropItem(vector<int> *pvRule,DWORD nValue,DWORD nAmountMax);
	int		GetDropItemMaxAmount() { return m_nMaxItem;}
	int		GetDropItemAmount() { return m_setItemType.size();}

	bool	IsValid()	{ return true; }
protected:
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_DROPRULE_H__1A26CA92_A4A3_42C6_81F9_DD1A70FA5E38__INCLUDED_)
