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

struct vasItem{
	int id;
	int itemType;
	int price;
	int time;
	int discount;
	int payType;
	int state;
	int doType;
	int aSaleCount;//特价商品数量
};
//限量商品
struct vasLimitItem{
	int id;
	int itemType;
	int aSaleCount;//商品数量
};

#include <vector>
using namespace std;

typedef	vector<vasItem> VECT_VASSHOP;

class CShopVASItem
{
public:
	static	CShopVASItem*	CreateNew()	{return new CShopVASItem;}
	int GetShopAmount() { return m_setVASShop.size(); }
	vasItem GetItemByIndex(int nIndex) { return m_setVASShop[nIndex]; }
	vasItem* GetItemByType(OBJID itemType);
	void setItem(int n,const vasItem& info,bool bSave=false);
	bool Create(IDatabase* pDb);
	int insertItem(vasItem& info,bool bSave = true);
	void deleteItem(const vasItem& info,bool bSave = true);
	
// 	int GetTimeSet(void);
// 	void SetTimeSet(int timeSet);
// 	
// 	int GetPriRate(void);
// 	void SetPriRate(int priRate);

	//以下是特价商品处理接口
	bool CreateASaleItem();//从数据库同步特价商品
	void InsertASaleItem(const vasItem& info);//特价商品表记录
	void ClearASaleItem();//清空特价商品表
	bool SetASaleItemAmount(OBJID itemType, int nAmount, bool bUpdate=false);//设置特价商品剩余数量
	int GetASaleItemAmount() { return m_setASaleItem.size(); }
	vasItem* GetASaleItemByType(OBJID itemType);
	vasItem* GetASaleItemByIndex(int nIndex);
	bool GenerateASaleItem(int nAmount);//随机生成特价商品，参数是特价商品的个数

	bool CreateLimitItem();//限量商品
	void InsertLimitItem(OBJID id, OBJID itemType, int nCount);
	vasLimitItem* GetLimitItemByType(OBJID itemType);
	bool IsSaledOut(OBJID itemType, int nNeed=1);
	bool IsLimitItem(OBJID itemType);
	bool SetLimitItemAmount(OBJID itemType, int nAmount, bool bUpdate);
	int  GetLimitItemAmount() { return m_setVasLimitItem.size(); }
	vasLimitItem* GetLimitItemByIndex(int nIndex);
	bool IsItemOnSale(int time);
	
	CShopVASItem();
	virtual ~CShopVASItem();
	
protected:
	VECT_VASSHOP m_setVASShop;
	VECT_VASSHOP m_setASaleShop;//商城特价商品可选项
	VECT_VASSHOP m_setASaleItem;//随机出来的特价商品
	vector<vasLimitItem>	m_setVasLimitItem;
	IDatabase* m_pDb;
	int maxID;
	
};
#endif // !defined(AFX_SHOPVAS_H__30F66C7E_D459_4E60_901C_43AD203555B3__INCLUDED_)
