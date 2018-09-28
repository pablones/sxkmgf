// MapItem.cpp: implementation of the CMapItem class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MapItem.h"
#include "MapGroup.h"
#include "ItemData.h"

MYHEAP_IMPLEMENTATION(CMapItem,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMapItem::CMapItem()
{
	SetObjType(OBJ_MAPITEM);
	m_pMap = NULL;
	m_bSee = false;
	m_nBossType = 0;
}

//////////////////////////////////////////////////////////////////////
CMapItem::~CMapItem()
{
	if (m_idMap!=1048)
	{
		if (m_bSee && m_pMap && MapGroup(m_idProcess)->GetMapManager()->GetGameMap(m_idMap,false))
		{
			m_pMap->LeaveRoom(QueryMapThing());
			DEBUG_TRY
			BroadcastSet(false);
			DEBUG_CATCH("BroadcastSet(false);")
		}
	}
	else
	{
		if (m_bSee && m_pMap)
		{
			m_pMap->LeaveRoom(QueryMapThing());
			DEBUG_TRY
				BroadcastSet(false);
			DEBUG_CATCH("BroadcastSet(false);")
		}
	}
	m_pMap = NULL;
	m_bSee = false;
}

bool CMapItem::Create(OBJID id,CGameMap* pMap,int posx,int posy,CUserPtr pOwner,PROCESS_ID idProcess,int nBossType/* = 0*/,const char* pDropMonster/* = NULL*/)
{
	CHECKF(pMap);
	CHECKF(pOwner);

	m_tAlive.SetInterval(MAPITEM_MONSTER_ALIVESECS);
	m_tAlive.Update();
	m_id		= id;
	m_pMap		= pMap;
	m_idMap		= pMap->GetID();
	m_pOwner	= pOwner;
	m_idOwner   = pOwner->GetID();
	m_nPosX		= posx;
	m_nPosY		= posy;
	m_nItemAmount = 0;
	m_idProcess = idProcess;
	m_bSee = false;
	m_nBossType = nBossType;
	if (pDropMonster!=NULL)
	{
		SafeCopy(m_pDropMonster,pDropMonster,_MAX_NAMESIZE);
	}
	return true;
}

void CMapItem::BroadcastSet(bool bEnter)
{
	CHECK (m_pMap);

	int x,y,z;
	IRole*	pRole = NULL;
	CUser* pTargetUser = NULL;
	FOR_9_BLOCKTHINGS(m_pMap, GetPosX(), GetPosY())
	{
		IMapThing* pTarget = m_pMap->QueryThingByIndex(x,y,z);
		if(pTarget && pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
		{
			if(abs(pTarget->GetPosX() - GetPosX()) <= CELLS_PER_VIEW
				&& abs(pTarget->GetPosY() - GetPosY()) <= CELLS_PER_VIEW 
				&& pTargetUser && pTargetUser == this->m_pOwner)
			{
				if (bEnter)
					pTargetUser->AddToBCMapItemSet(GetID());
				else
					pTargetUser->RemoveFromBCMapItemSet(GetID());
			}
		}
	}
}

bool CMapItem::AddItem(OBJID nItemType)
{
	CHECKF(m_pMap);	

	if(m_nItemAmount >= 10)
		return false;

	m_setBePick[m_nItemAmount] = false;
	m_setItemType[m_nItemAmount++] = nItemType;
	return true;
}

void CMapItem::SendShowFirst()
{
	if (m_idMap!=1048)
	{
		if(m_pMap && MapGroup(m_idProcess)->GetMapManager()->GetGameMap(m_idMap,false))
		{
			m_pMap->EnterRoom(QueryMapThing());
			m_bSee = true;
			BroadcastSet(true);
		}
	}
	else
	{
		if (m_pMap)
		{
			m_pMap->EnterRoom(QueryMapThing());
			m_bSee = true;
			BroadcastSet(true);
		}
	}
}

void CMapItem::SendShow(IRole* pRole)
{
	if(pRole && pRole->GetID() != m_idOwner)
		return;

	if(!m_bSee)
		return;

	CMsgMapItem	msg;
	if(msg.Create(MSGMAPITEM_CREATE, this))
		pRole->SendMsg(&msg);
}

int  CMapItem::GetReTime()
{
	return m_tAlive.GetRemain();
}


void CMapItem::SetLife(int nSec /* = 0 */)
{
	m_tAlive.SetInterval(nSec);
	m_tAlive.Update();
}
