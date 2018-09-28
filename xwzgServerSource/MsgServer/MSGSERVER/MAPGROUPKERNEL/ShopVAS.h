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
	int aSaleCount;//�ؼ���Ʒ����
};
//������Ʒ
struct vasLimitItem{
	int id;
	int itemType;
	int aSaleCount;//��Ʒ����
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

	//�������ؼ���Ʒ����ӿ�
	bool CreateASaleItem();//�����ݿ�ͬ���ؼ���Ʒ
	void InsertASaleItem(const vasItem& info);//�ؼ���Ʒ���¼
	void ClearASaleItem();//����ؼ���Ʒ��
	bool SetASaleItemAmount(OBJID itemType, int nAmount, bool bUpdate=false);//�����ؼ���Ʒʣ������
	int GetASaleItemAmount() { return m_setASaleItem.size(); }
	vasItem* GetASaleItemByType(OBJID itemType);
	vasItem* GetASaleItemByIndex(int nIndex);
	bool GenerateASaleItem(int nAmount);//��������ؼ���Ʒ���������ؼ���Ʒ�ĸ���

	bool CreateLimitItem();//������Ʒ
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
	VECT_VASSHOP m_setASaleShop;//�̳��ؼ���Ʒ��ѡ��
	VECT_VASSHOP m_setASaleItem;//����������ؼ���Ʒ
	vector<vasLimitItem>	m_setVasLimitItem;
	IDatabase* m_pDb;
	int maxID;
	
};
#endif // !defined(AFX_SHOPVAS_H__30F66C7E_D459_4E60_901C_43AD203555B3__INCLUDED_)
