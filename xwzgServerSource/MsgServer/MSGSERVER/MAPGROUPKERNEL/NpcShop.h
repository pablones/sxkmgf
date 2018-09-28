// NpcShop.h: interface for the CNpcShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCSHOP_H__92EE4D73_A647_49A7_9F33_9168AA574529__INCLUDED_)
#define AFX_NPCSHOP_H__92EE4D73_A647_49A7_9F33_9168AA574529__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GameObjSet.h"
#include "ItemType.h"
#include "Myheap.h"
#include "TimeOut.h"

#include <map>
using namespace std;

struct GoodInfo 
{
	DWORD idType;
	DWORD needItemType;
	DWORD needItemAmount;
	DWORD nBasePrice;
	DWORD nPriceRand;
	DWORD nLeaveAmount;
	DWORD nRnadPrice;
	DWORD nSellType;
};

typedef	map<OBJID,GoodInfo*>	GoodsSet;
typedef	map<OBJID,GoodInfo*>::iterator	GoodsIt;

class CUser;
class CNpcShop  
{
protected:
	CNpcShop();
	virtual ~CNpcShop();
public:
	static CNpcShop* CreateNew()						{ return new CNpcShop; }
	ULONG	Release()									{ delete this; return 0; }

public:
	bool	Create(PROCESS_ID idProcess, CNpc* pOwner, OBJID idNpc);
	int		QueryItemTypeSpend(OBJID idType);
	int		QueryItemAmountSpend(OBJID idType);
	int		QueryRandShopLeave(OBJID idType);
	int		QueryRandShopPrice(OBJID idType);
	int		QueryRandShopSellType(OBJID idType);	
	bool	IsOpen();
	int		Rebate(int nPrice, OBJID idSyn, int nRankShow);
	bool    SendInfo(CUser* pUser);
	void    DoRandPrice();

protected: // data
	GoodsSet		m_setGoods;
	CTimeOut		m_tUpdate;

private: // ctrl
	CNpc*			m_pOwner;
	PROCESS_ID		m_idProcess;

	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_NPCSHOP_H__92EE4D73_A647_49A7_9F33_9168AA574529__INCLUDED_)
