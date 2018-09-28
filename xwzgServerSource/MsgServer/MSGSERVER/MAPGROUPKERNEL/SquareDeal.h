// SquareDeal.h: interface for the CSquareDeal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQUAREDEAL_H__A5B68023_45DD_4048_8818_AD1778901164__INCLUDED_)
#define AFX_SQUAREDEAL_H__A5B68023_45DD_4048_8818_AD1778901164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Myheap.h"
#include "TaskDetailData.h"
#include <vector>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
class ISquareDeal;
class IUserSquareDeal			// CUser的接口类
{
public: // constructor
	virtual bool	Create(ISquareDeal* pSquareDeal)				PURE_VIRTUAL_FUNCTION_0
	virtual ULONG	Release()										PURE_VIRTUAL_FUNCTION_0
	virtual CUser*	QueryUser()										PURE_VIRTUAL_FUNCTION_0

public: // application
	// return false: too weight
	virtual bool	AddItem(CItem* pItem, int nAmount, int nAllWeight)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	DelItem(CItem* pItem, int nAmount, int nAllWeight)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	TradeLock()			                                        PURE_VIRTUAL_FUNCTION_0
	// return false: too more
	virtual bool	AddMoney(int nAllMoney)							PURE_VIRTUAL_FUNCTION_0
	virtual void	ClickOK()										PURE_VIRTUAL_FUNCTION
	virtual bool	AddTradeVAS(int nAllMoney)						PURE_VIRTUAL_FUNCTION_0
	virtual	bool	AddMonster(CUserPetData* pPetData, int nAmount)	PURE_VIRTUAL_FUNCTION_0
	virtual	bool	DelMonster(CUserPetData* pPetData, int nAmount)	PURE_VIRTUAL_FUNCTION_0
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class ISquareDeal
{
public: // constructor
	// return false: 对方正在交易中。注意：SQUARE DEAL 会自动将自己登记到USER中
	virtual bool	Create(IUserSquareDeal* pUser, IUserSquareDeal* pTarget)	PURE_VIRTUAL_FUNCTION_0
	virtual ULONG	Release()										PURE_VIRTUAL_FUNCTION_0
	
public: // application
	// return false: too weight
	virtual bool	AddItem(CItem* pItem)							PURE_VIRTUAL_FUNCTION_0
	virtual bool	DelItem(CItem* pItem)							PURE_VIRTUAL_FUNCTION_0
	virtual bool	TradeLock()         							PURE_VIRTUAL_FUNCTION_0
	// return all money
	virtual int 	AddMoney(int nMoney)							PURE_VIRTUAL_FUNCTION_0
	virtual int 	DelMoney(int nMoney)							PURE_VIRTUAL_FUNCTION_0
	virtual int 	AddVAS(int nMoney)								PURE_VIRTUAL_FUNCTION_0
	virtual int 	DelVAS(int nMoney)								PURE_VIRTUAL_FUNCTION_0
	// return true: trade ok, release this please!!!
	virtual bool	ClickOK()										PURE_VIRTUAL_FUNCTION_0
	virtual CUser*	GetTarget()										PURE_VIRTUAL_FUNCTION_0
	virtual void	CancelTrade()									PURE_VIRTUAL_FUNCTION
	virtual	bool	AddMonster(CUserPetData* pPetData)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	DelMonster(CUserPetData* pPetData)				PURE_VIRTUAL_FUNCTION_0
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class CItem;
class CSquareDeal  
{
protected:
	CSquareDeal();
	virtual ~CSquareDeal();
public:
	static ISquareDeal* CreateNew()		{ CSquareDeal* ptr = new CSquareDeal; if(ptr) return &ptr->m_iface1; else return NULL; }

protected:
	bool	TradeOK();
	bool	UserCancelTrade();

	//----------------------
	class XSquareDeal1 : public ISquareDeal
	{
	protected: // constructor
		CSquareDeal*	This()							{ return m_pParent; }
		virtual bool	Create(IUserSquareDeal* pUser, IUserSquareDeal* pTarget);
		virtual ULONG	Release		()					{ delete m_pParent; return 0; }
	protected: // application
		virtual bool	AddItem		(CItem* pItem);
		virtual bool	DelItem		(CItem* pItem);
		virtual int		AddMoney	(int nMoney);
		virtual bool	TradeLock   ();
		virtual int		DelMoney	(int nMoney);// return allmoney
		virtual bool	ClickOK		();						// return true: 成交
		virtual CUser*	GetTarget	();
		virtual int 	AddVAS(int nMoney);	
    	virtual int 	DelVAS(int nMoney);	
		virtual void	CancelTrade();
		virtual	bool	AddMonster(CUserPetData* pPetData);
		virtual bool	DelMonster(CUserPetData* pPetData);
	private:
		CSquareDeal*	m_pParent;
		friend class CSquareDeal;
	}			m_iface1;

	//----------------------
	class XSquareDeal2 : public ISquareDeal
	{
	protected: // constructor
		CSquareDeal*	This()							{ return m_pParent; }
		virtual bool	Create(IUserSquareDeal* pUser, IUserSquareDeal* pTarget);
		virtual ULONG	Release		()					{ delete m_pParent; return 0; }
	protected: // application
		virtual bool	AddItem		(CItem* pItem);
		virtual bool	DelItem		(CItem* pItem);
		virtual bool	TradeLock   ();
		virtual int		AddMoney	(int nMoney);		// return allmoney
		virtual int		DelMoney	(int nMoney);
		virtual bool	ClickOK		();						// return true: 成交
		virtual CUser*	GetTarget	();
		virtual int 	AddVAS(int nMoney);	
    	virtual int 	DelVAS(int nMoney);	
		virtual void	CancelTrade();
		virtual	bool	AddMonster(CUserPetData* pPetData);
		virtual bool	DelMonster(CUserPetData* pPetData);
	private:
		CSquareDeal*	m_pParent;
		friend class CSquareDeal;
	}			m_iface2;

protected:
	IUserSquareDeal*	m_pUser1;
	IUserSquareDeal*	m_pUser2;
	typedef vector<OBJID>	ITEM_SET;
	ITEM_SET		m_setItem1;
	ITEM_SET		m_setItem2;
	ITEM_SET		m_setPet1;
	ITEM_SET		m_setPet2;
	int				m_nMoney1;
	int				m_nMoney2;
	int				m_nVas1;
	int				m_nVas2;
	bool			m_bClickOK1;
	bool			m_bClickOK2;
	bool			m_bLocked1;//[2010-06-16 goto]判断交易时是否已经点击了锁定
	bool			m_bLocked2;//[2010-06-16 goto]判断交易时是否已经点击了锁定
	bool			m_bTradeSuccess;
	friend class CSquareDeal::XSquareDeal1;
	friend class CSquareDeal::XSquareDeal2;

	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_SQUAREDEAL_H__A5B68023_45DD_4048_8818_AD1778901164__INCLUDED_)
