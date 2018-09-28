// DropRuleGroup.h: interface for the CDropRuleGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPRULEGROUP_H__13989434_F1AB_4C95_B04C_C15619CFBBB8__INCLUDED_)
#define AFX_DROPRULEGROUP_H__13989434_F1AB_4C95_B04C_C15619CFBBB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "../share/define.h"
/*
#include "ConstGameData.h"
#include "GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleMap2.h"
*/
#include "../../GameBaseCodeMT/I_mydb.h"
#include "DropRule.h"
#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
class IDatabase;
class CDropRuleGroup  
{
public:
	CDropRuleGroup();
	virtual ~CDropRuleGroup();

public:
	static	CDropRuleGroup*	CreateNew()	{ return new CDropRuleGroup; }
	ULONG	Release()	{ delete this; return 0; }

public:
	bool	Create(OBJID	idRuleGroup, IDatabase*	pDb);
	void	Destroy();

	bool	LoadInfo(IRecordset*	pRes);
	bool	GetDropItem(vector<int> *pvRule);
	int		GetDropRuleCount(){return m_setRule.size();}
	bool	IsValid()	{ if (m_setRule.size() == 0) return false; return true; }
protected:
	typedef std::vector<CDropRule*> RULE_SET;

	OBJID		m_idRuleGroup;
	RULE_SET	m_setRule;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_DROPRULEGROUP_H__13989434_F1AB_4C95_B04C_C15619CFBBB8__INCLUDED_)
