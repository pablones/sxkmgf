// PortableShop.h: interface for the CPortableShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTABLESHOP_H__7342E5AB_27FA_499F_A521_464D337FB4C1__INCLUDED_)
#define AFX_PORTABLESHOP_H__7342E5AB_27FA_499F_A521_464D337FB4C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "ItemType.h"
#include "Myheap.h"
#include "NpcShop.h"

#include <map>
using namespace std;
// 
// struct GoodInfo 
// {
// 	DWORD idType;
// 	DWORD needItemType;
// 	DWORD needItemAmount;
// 	DWORD nBasePrice;
// 	DWORD nPriceRand;
// 	DWORD nLeaveAmount;
// 	DWORD nRnadPrice;
// 	DWORD nSellType;
// };

typedef	map<OBJID,GoodInfo*>	GoodsSet;
typedef	map<OBJID,GoodInfo*>::iterator	GoodsIt;

class CUser;
class CPortableShop  
{
protected:
	CPortableShop();
	virtual ~CPortableShop();

public:
	static CPortableShop* CreateNew()						{ return new CPortableShop; }
	ULONG	Release()										{ delete this; return 0; }
	
public:
	bool	Create(PROCESS_ID idProcess, OBJID id);
	int		QueryItemTypeSpend(OBJID idType);
	int		QueryItemAmountSpend(OBJID idType);
	bool    SendInfo(CUser* pUser, OBJID id);
	OBJID	GetOwerID(){return m_idower;}
protected: // data
	GoodsSet		m_setGoods;
	OBJID			m_idower;
private: // ctrl
	PROCESS_ID		m_idProcess;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_PORTABLESHOP_H__7342E5AB_27FA_499F_A521_464D337FB4C1__INCLUDED_)
