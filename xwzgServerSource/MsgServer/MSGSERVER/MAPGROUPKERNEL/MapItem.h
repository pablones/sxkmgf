// MapItem.h: interface for the CMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPITEM_H__9FB2A993_BFD6_47A9_B21A_4378262CCC0B__INCLUDED_)
#define AFX_MAPITEM_H__9FB2A993_BFD6_47A9_B21A_4378262CCC0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gameobj.h"
#include "ItemType.h"
#include "GameBlock.h"
#include "Timeout.h"
#include "Myheap.h"
#include "Item.h"

//////////////////////////////////////////////////////////////////////
const int	MAPITEM_ONTIMER_SECS		= 5;		// MAPMANAGER中的定时检查时间(太大不好-paled)
const int	MAPITEM_MONSTER_ALIVESECS	= 40;		// 怪物掉的地面物品的生存时间
const int	MAPITEM_USERMAX_ALIVESECS	= /*60*/10;//3*60;		// 玩家扔的地面物品的生存时间MAX
const int	MAPITEM_USERMIN_ALIVESECS	= /*60*/10;//1*60;		// 玩家扔的地面物品的生存时间MIN
const int	MAPITEM_PRIV_SECS			= /*30*/5;//30;		// 打怪玩家优先时间
const int	PICKMAPITEMDIST_LIMIT		= 3;		// 捡钱的限制距离

//////////////////////////////////////////////////////////////////////
class CMapItem : public CGameObj, IMapThing
{
protected:
	CMapItem();
	virtual ~CMapItem();
public:
	static	CMapItem* CreateNew()			{ return new CMapItem; }
	void	ReleaseByOwner()				{ delete this; }
	bool	Create(OBJID id,CGameMap* pMap,int posx,int posy,CUserPtr pOwner,PROCESS_ID idProcess,int nBossType = 0,const char* pDropMonster=NULL);
	void	BroadcastSet(bool bEnter);
	CUserPtr GetOwner()						{ return m_pOwner;}
	OBJID	 GetOwnerID()					{ return m_idOwner;}
	int 	 GetItemAmount()				{ return m_nItemAmount;}

	int			m_nItemAmount;
	OBJID		m_setItemType[8];
	bool		m_setBePick[8];
public: // interface
	virtual IMapThing*	QueryMapThing()		{ return (IMapThing*)this; }
public: // IMapThing
	virtual OBJID	GetID()									{ return m_id; }
	virtual int 	GetObjType()							{ return CGameObj::GetObjType(); }
	virtual int		GetPosX()								{ return m_nPosX; }
	virtual int		GetPosY()								{ return m_nPosY; }
	virtual CGameMap*	GetMap()							{ CHECKF(m_pMap); return m_pMap; }
	virtual void	SendShow(IRole* pRole);
	void	SendShowFirst();
protected:
	virtual bool	QueryObj(OBJID idObjType, void** ppv)			{ if(idObjType==OBJ_MAPITEM) return *ppv=this,true; return *ppv=NULL,false; }
	virtual bool	QueryRole(void** ppv)					{ return *ppv=NULL,false; }
public:
	int     GetReTime();
	void    SetLife(int nSec = 0);
	bool	IsAlive()										{ return !m_tAlive.IsTimeOut();}
	bool	AddItem(OBJID nItemType);
	int     m_nBossType;
	char   m_pDropMonster[_MAX_NAMESIZE];
protected:
	bool		m_bSee;
	OBJID		m_id;
	int			m_nPosX;
	int			m_nPosY;
	CGameMap*	m_pMap;
	OBJID		m_idMap;
	PROCESS_ID	 m_idProcess;
	CTimeOut	m_tAlive;
	CUserPtr	m_pOwner;
	OBJID		m_idOwner;	
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAPITEM_H__9FB2A993_BFD6_47A9_B21A_4378262CCC0B__INCLUDED_)
