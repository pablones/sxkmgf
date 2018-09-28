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
		_STORE_USERPACK_ITEM	=ITEMPOSITION_CHEST,				//	_ITEM_PACKAGE �ٱ��ң�����Ʒ��
		_STORE_EUDEMONBROODER,		// NPC ���ϵĻ��޷�����
		_STORE_EUDEMONITEM	,		// ���޲ֿ�
		_STORE_EUD_BROOD_PACKAGE,	// ������ϵĻ��޷�����
};
enum{
	GOODS,
	MONEY,
};
const int	PACKAGE_LIMIT			= 36;			// һ��package����ܷŶ��ٸ���Ʒ��ע�⣺̫��ʱCMsgPacket���ܳ���1024��
const int	PACKAGE_DEFAULTINDEX	= 60000;		//����Ĭ��λ�ã���Ʒ�浽��һ��Ϊ�յĸ���
const int	PACKAGE_ORIGIN			= 24;			//��ʼ�ֿ������

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
	virtual CItem*	CheckOut(CUser* pUser, OBJID idItem, int nIndex, bool bUpdate)	PURE_VIRTUAL_FUNCTION_0					// �Զ�ͬ���ͻ���
	virtual bool	SendInfo(CUser* pUser)	PURE_VIRTUAL_FUNCTION_0									// �Զ�ͬ���ͻ���
	virtual bool	IsPlayerPackage()		PURE_VIRTUAL_FUNCTION_0	
	virtual int		GetAmount()				PURE_VIRTUAL_FUNCTION_0
	//virtual int	GetWeight(OBJID idItem)	PURE_VIRTUAL_FUNCTION_0
	//virtual OBJID	GetItemTypeByID(OBJID idItem)			PURE_VIRTUAL_FUNCTION_0
	//virtual DWORD	GetItemAmountLimitByID(OBJID idItem)	PURE_VIRTUAL_FUNCTION_0
	//LW�������----------------------------------------
	virtual CItem*	GetItemByIndex(int index)	PURE_VIRTUAL_FUNCTION_0
	virtual CItem*  GetItemByID(OBJID idItem)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	DelItem(CUser* pUser, CItem* pItem)	PURE_VIRTUAL_FUNCTION_0
	//virtual bool    CAuctionChip(/*OBJID idNpc,*/OBJID idUser,DWORD value,IRecordset* pRecordset,IDatabase* pDb)	PURE_VIRTUAL_FUNCTION_0//����һ����Ʒ�̣�
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
	void CheckItemLifeTime(CUserPtr pOwner, BOOL bUpdateToClient, BOOL bSaveToDatabase);	//  [2007-9-14] ���
	int GetItemType(int index);
    bool	Create(int nType, OBJID idOwner, OBJID idPlayer, int nIndex, IDatabase* pDb,bool bLogin);// ���ظ�����
   
	virtual bool	CheckIn(CUser* pUser, CItem* pItem, int nIndex, bool bUpdate = false);// �Զ�ͬ���ͻ���
//	virtual bool    CheckIn(CItem* pItem){CHECKF(pItem);if(m_setItem->AddObj(pItem)) { pItem->SaveInfo ;return true;}else return false;}
//	virtual CItem*	CheckOut(OBJID idItem);
	virtual CItem*	CheckOut(CUser* pUser, OBJID idItem, int nIndex, bool bUpdate = false);					// �Զ�ͬ���ͻ���
	bool	SendInfo(CUser* pUser, int nIndex=1);									// �Զ�ͬ���ͻ���
	bool	IsPlayerPackage()								{ return m_nType == ITEMPOSITION_STORAGE || m_nType == ITEMPOSITION_CHEST/* || m_nType == ITEMPOSITION_MAGICSTONEBOX*/;/*LW*/  }
	int		GetAmount()										{ return m_setItem->GetAmount(); }
	OBJID	GetItemTypeByID(OBJID idItem);
	DWORD	GetItemAmountLimitByID(OBJID idItem);
	//LW�������----------------------------------------
	CItem*	GetItemByIndex(int index);//{CHECKF(index >= 0 && index < GetAmount());return(m_setItem->GetObjByIndex(index));}
	CItem*  GetItemByID(OBJID idItem){CHECKF(idItem > 0);CHECKF(m_setItem != NULL);return(m_setItem->GetObj(idItem));}
	//LW------------------------------------------------
	//	20070712	���	������ID������Ʒ
	CItemPtr GetItemByTypeID(OBJID idType);
	bool	 CombineItem(CUser* pUser, CItem* pItem1, CItem* pItem2);	   //�ϲ���Ʒ

	bool	ExChangeIndex(CUser* pUser, CItem* pItem1, CItem* pItem2);		//����λ��
	bool	IsEmpty(int nIndex);//�жϸ����ǲ��ǿյ�
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
