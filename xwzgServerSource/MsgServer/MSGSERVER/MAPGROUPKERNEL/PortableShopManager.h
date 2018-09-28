// PortableShopManager.h: interface for the CPortableShopManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTABLESHOPMANAGER_H__ED693FB8_C279_46FE_9669_570E102DE90C__INCLUDED_)
#define AFX_PORTABLESHOPMANAGER_H__ED693FB8_C279_46FE_9669_570E102DE90C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PortableShop.h"

#include <vector>
#define PORTABLESHOP_DOWN 1000000
#define PORTABLESHOP_UP	  2000000
class CPortableShopManager  
{
public:
	CPortableShopManager();
	virtual ~CPortableShopManager();
public:
	static CPortableShopManager* CreateNew()						{ return new CPortableShopManager; }
	ULONG    Release(){delete this; return 0;}
	bool	CreateAll(PROCESS_ID idProcess);
	CPortableShop* QueryShopByOwerid(OBJID Owerid);
		
protected: // data
	//CPortableShop*		m_pShop;
	vector<CPortableShop*> m_ShopList;

private: // ctrl
	PROCESS_ID		m_idProcess;
};

#endif // !defined(AFX_PORTABLESHOPMANAGER_H__ED693FB8_C279_46FE_9669_570E102DE90C__INCLUDED_)
