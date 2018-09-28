// Package.h: interface for the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGE_H__DBC395BA_5C4A_40B5_944A_329D547BFBE5__INCLUDED_)
#define AFX_PACKAGE_H__DBC395BA_5C4A_40B5_944A_329D547BFBE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"
#include "GameObjSet.h"
#include "Myheap.h"
#include <vector>
//////////////////////////////////////////////////////////////////////
enum {
		_STORE_NONE				=0, 
		_STORE_NPCITEM			=ITEMPOSITION_STORAGE,
		_STORE_TRUNKITEM		=ITEMPOSITION_TRUNK,
		_STORE_USERPACK_ITEM	=ITEMPOSITION_CHEST,				//	_ITEM_PACKAGE 百宝囊（放物品）
		_STORE_EUDEMONBROODER,		// NPC 身上的幻兽孵化器
		_STORE_EUDEMONITEM	,		// 幻兽仓库
		_STORE_EUD_BROOD_PACKAGE,	// 玩家身上的幻兽孵化器
};
enum{
	GOODS,
	MONEY,
};
const int	PACKAGE_LIMIT			= 36;			// 一个package最多能放多少个物品，注意：太大时CMsgPacket可能超过1024。
const int	PACKAGE_DEFAULTINDEX	= 60000;		//存入默认位置，物品存到第一个为空的格子
const int	PACKAGE_ORIGIN			= 24;			//初始仓库格子数

//////////////////////////////////////////////////////////////////////
typedef	IGameObjSet<CItem>	IItemSet;
typedef	CGameObjSet<CItem>	CItemSet;

class IPackage
{
public:
	virtual void	Release()	PURE_VIRTUAL_FUNCTION
	virtual bool	Create(int nType, OBJID idOwner, OBJID idPlayer, int nIndex, IDatabase* pDb)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	CheckIn(CUser* pUser, CItem* pItem, bool bUpdate = true)			PURE_VIRTUAL_FUNCTION_0
//	virtual bool    CheckIn(CItem* pItem)		PURE_VIRTUAL_FUNCTION_0
//	virtual CItem*	CheckOut(OBJID idItem)		PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	CheckOut(CUser* pUser, OBJID idItem, int nIndex, bool bUpdate)	PURE_VIRTUAL_FUNCTION_0					// 自动同步客户端
	virtual bool	SendInfo(CUser* pUser)	PURE_VIRTUAL_FUNCTION_0									// 自动同步客户端
	virtual bool	IsPlayerPackage()		PURE_VIRTUAL_FUNCTION_0	
	virtual int		GetAmount()				PURE_VIRTUAL_FUNCTION_0
	//virtual int	GetWeight(OBJID idItem)	PURE_VIRTUAL_FUNCTION_0
	//virtual OBJID	GetItemTypeByID(OBJID idItem)			PURE_VIRTUAL_FUNCTION_0
	//virtual DWORD	GetItemAmountLimitByID(OBJID idItem)	PURE_VIRTUAL_FUNCTION_0
	//LW拍买相关----------------------------------------
	virtual CItem*	GetItemByIndex(int index)	PURE_VIRTUAL_FUNCTION_0
	virtual CItem*  GetItemByID(OBJID idItem)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	DelItem(CUser* pUser, CItem* pItem)	PURE_VIRTUAL_FUNCTION_0
	//virtual bool    CAuctionChip(/*OBJID idNpc,*/OBJID idUser,DWORD value,IRecordset* pRecordset,IDatabase* pDb)	PURE_VIRTUAL_FUNCTION_0//创建一个物品ＬＷ
};


class CPackage  
{
protected:
	CPackage();
	virtual ~CPackage();
public:
	static CPackage* CreateNew()							{ return new CPackage; }
	ULONG	Release()										{ delete this; return 0; }

public:
	void CheckItemLifeTime(CUserPtr pOwner, BOOL bUpdateToClient, BOOL bSaveToDatabase);	//  [2007-9-14] 朱斌
	int GetItemType(int index);
    bool	Create(int nType, OBJID idOwner, OBJID idPlayer, int nIndex, IDatabase* pDb,bool bLogin);// 可重复调用
   
	virtual bool	CheckIn(CUser* pUser, CItem* pItem, int nIndex, bool bUpdate = false);// 自动同步客户端
//	virtual bool    CheckIn(CItem* pItem){CHECKF(pItem);if(m_setItem->AddObj(pItem)) { pItem->SaveInfo ;return true;}else return false;}
//	virtual CItem*	CheckOut(OBJID idItem);
	virtual CItem*	CheckOut(CUser* pUser, OBJID idItem, int nIndex, bool bUpdate = false);					// 自动同步客户端
	bool	SendInfo(CUser* pUser, int nIndex=1);									// 自动同步客户端
	bool	IsPlayerPackage()								{ return m_nType == ITEMPOSITION_STORAGE || m_nType == ITEMPOSITION_CHEST/* || m_nType == ITEMPOSITION_MAGICSTONEBOX*/;/*LW*/  }
	int		GetAmount()										{ return m_setItem->GetAmount(); }
	OBJID	GetItemTypeByID(OBJID idItem);
	DWORD	GetItemAmountLimitByID(OBJID idItem);
	//LW拍买相关----------------------------------------
	CItem*	GetItemByIndex(int index);//{CHECKF(index >= 0 && index < GetAmount());return(m_setItem->GetObjByIndex(index));}
	CItem*  GetItemByID(OBJID idItem){CHECKF(idItem > 0);CHECKF(m_setItem != NULL);return(m_setItem->GetObj(idItem));}
	//LW------------------------------------------------
	//	20070712	朱斌	按类型ID查找物品
	CItemPtr GetItemByTypeID(OBJID idType);
	bool	 CombineItem(CUser* pUser, CItem* pItem1, CItem* pItem2);	   //合并物品

	bool	ExChangeIndex(CUser* pUser, CItem* pItem1, CItem* pItem2);		//交换位置
	bool	IsEmpty(int nIndex);//判断格子是不是空的
	bool	MoveStorageItem(CUser* pUser, CItem* pItem, int nIndex);
	bool	DelItem(CUser* pUser, CItem* pItem);
	bool	AddItem( CItem* pItem);
	void	SendAllObjInfo	(CUserPtr pOwner,PROCESS_ID idProcess);
protected:
	IItemSet*	m_setItem;
	int			m_nType;
	OBJID		m_idOwner;
	OBJID		m_idPlayer;
	UCHAR		m_ucType;
protected:
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_PACKAGE_H__DBC395BA_5C4A_40B5_944A_329D547BFBE5__INCLUDED_)
