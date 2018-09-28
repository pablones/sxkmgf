// ShopVAS.h: interface for the CShopVAS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOPVAS_H__30F66C7E_D459_4E60_901C_43AD203555B3__INCLUDED_)
#define AFX_SHOPVAS_H__30F66C7E_D459_4E60_901C_43AD203555B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol.h"
#include "GameObjSet.h"
#include "../../GameBaseCodeMT/I_mydb.h"
// struct vasItemInfo{
// 	int id;
// 	int type;
// 	int index1;
// 	int index2;
// 	int pri;
// 	int time;
// 	int doType;
// 	int nMask;
// };
struct vasItemInfo{
	int id;
	int itemType;
	int price;
	int time;
	int discount;
	int payType;
	int state;
	int doType;
};
#include <vector>
using namespace std;

typedef	vector<vasItemInfo> VECT_VASSHOP;

class CShopVAS  
{
public:
	int GetShopAmount() { return m_setVASShop.size(); }
	vasItemInfo GetItemByIndex(int nIndex) { return m_setVASShop[nIndex]; }
	vasItemInfo* GetItemByType(OBJID itemType);
	void setItem(int n,const vasItemInfo& info,bool bSave=false);
	bool Create(IDatabase* pDb);

	CShopVAS();
	virtual ~CShopVAS();

protected:
	VECT_VASSHOP m_setVASShop;
	IDatabase* m_pDb;
	int maxID;
};
#endif // !defined(AFX_SHOPVAS_H__30F66C7E_D459_4E60_901C_43AD203555B3__INCLUDED_)
