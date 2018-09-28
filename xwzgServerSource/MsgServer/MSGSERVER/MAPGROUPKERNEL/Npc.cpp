// TaskNpc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

//#include "AllMsg.h"
#include "afx.h"
#include "MsgPlayer.h"
#include "MsgNpcInfoEx.h"
#include "MsgNpcInfo.h"
#include "MsgNpc.h"
#include "MsgTalk.h"
#include "MsgUserAttrib.h"
#include "MsgMagicEffect.h"
#include "MsgAction.h"
//#include "windows.h"
#include "Gameobj.h"
#include "MapGroup.h"
#include "../share/define.h"
#include "Npc.h"
#include "NpcTask.h"
#include "NpcScene.h"
#include "NpcLink.h"
#include "NpcStorage.h"
//#include "BetOutside.h"


//////////////////////////////////////////////////////////////////////
const int	FIXSYNFLAG_SECS				= 10;
const int	MAX_FIXMONEY				= 1000;
const int	FIXLIFE_PER_MONEY			= 1;
const int	BROADCASTSCORE_SECS			= 30;				// 注意：太大可能错开战斗时间检查
const int	TRANSNPC_FEE				= 10000;			// 每天每个传送门收费

//////////////////////////////////////////////////////////////////////
// static 
char	szNpcTable[] = _TBL_NPC;
char	szDynTable[] = _TBL_DYNANPC;
MYHEAP_IMPLEMENTATION(CNpc,s_heap)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpc::CNpc()
{
	SetObjType(OBJ_NPC);
	m_id	= ID_NONE;
	m_pData	= NULL;
	m_bDynaNpc	= false;

	m_pShop		= NULL;
//	m_pTask		= NULL;
//	m_pStorage	= NULL;
//	m_pTrunk	= NULL;
//	m_pScene	= NULL;
//	m_pLinkMap	= NULL;
//	m_pBet		= NULL;
	m_i64Effect	= 0;
	m_beginTime   = 0;
	m_startPosX   = 0;  //用于记录第一次坐标
    m_startPosY   = 0;
	m_nNpcType    = 0;
	m_bTimeDuring = false;
	m_nToken = 0;
	m_bNPCShow = true;
	m_bSetShow = true;
	m_tUpdateShow.Clear();
}

//////////////////////////////////////////////////////////////////////
CNpc::~CNpc()
{
	DEBUG_TRY
	if(m_pMap)
	{
		//SendLeaveFromBlock();
		ClrBroadcastSet();
		
		if(m_pMap)
		{
			m_pMap->LeaveRoom(QueryMapThing(), WITH_BLOCK);
//			UpdateHoldFloor(false);
		}
	}

	SAFE_RELEASE (m_setStatus);

	if(m_pData)
	{
//		if(IsDynaNpc())
//		{
//			if(IsAlive())
//				SaveInfo();
//			else
//				DeleteRecord();
//		}
		m_pData->Release();
	}
	if(m_pShop)
	{
		m_pShop->Release();
	}
// 	if(m_pTask)
// 	{
// 		m_pTask->Release();
// 	}
	/*if(m_pStorage)
	{
		m_pStorage->Release();
	}*/
// 	if(m_pLinkMap)
// 	{
// 		m_pLinkMap->Release();
// 	}
// 	if(m_pScene)
// 	{
// 		m_pScene->Release();
// 	}
// 	if(m_pTrunk)
// 	{
// 		m_pTrunk->Release();
// 	}
// 	if(m_pBet)
// 	{
// 		m_pBet->Release();
// 	}

	//zeng添加 回收动态NPC的ID，主要是副本中的功能性NPC使用这些ID
	if(IsDynaNpc())
		MapManager()->RecycleDynaNpcID(m_id);


	DEBUG_CATCH("CNpc::~CNpc()");
}

bool CNpc::Create(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName,CGameMap* pGameMap)
{
	m_idProcess = idProcess;
	m_bDynaNpc	= true;

	ASSERT(!m_pData);
	CDynaNpcData* pData = CDynaNpcData::CreateNew();
	if(pData)
		m_pData	= pData->QueryInterface();
	IF_NOT(m_pData && m_pData->Create(GameDataDefault()->GetDynaNpc(), ID_NONE))			// idRecord))
		return false;


/*	m_pData->SetInt(NPCDATA_OWNERID,	pInfo->idOwner);
	m_pData->SetInt(NPCDATA_OWNERTYPE,	pInfo->nOwnerType);*/
	m_pData->SetInt(NPCDATA_TYPE,		pInfo->nType);
	m_pData->SetStr(NPCDATA_NAME,		szName, _MAX_NAMESIZE);
	m_pData->SetInt(NPCDATA_LOOKFACE,	pInfo->nLookFace);
	m_pData->SetInt(NPCDATA_MAPID,		pInfo->idMap);
	m_pData->SetInt(NPCDATA_POSX_,		pInfo->nPosX);
	m_pData->SetInt(NPCDATA_POSY_,		pInfo->nPosY);
	m_pData->SetInt(NPCDATA_SORT,		pInfo->nSort);
	m_pData->SetInt(NPCDATA_ACTION,		pInfo->nAction);
	//m_pData->InsertRecord();
	m_pData->SetInt(NPCDATA_LEVEL,pInfo->nLev);
	m_id	= MapManager()->SpawnNewNpcID();
	m_nPosX = m_pData->GetInt(NPCDATA_POSX_);
	m_nPosY = m_pData->GetInt(NPCDATA_POSY_);
    
	m_startPosX = m_nPosX;
	m_startPosY = m_nPosY;
	m_nNpcType = pInfo->nNpcType;
	
	if (this->IsShopNpc() || IsTaskShopNpc() || IsRandTaskShopNpc())
	{
		IF_NOT(CreateFuncObj(idProcess))
			return false;
	}

	IF_NOT(CheckSortMutex())
		return false;

	m_pMap = pGameMap;
	IF_NOT(m_pMap)
		return false;			//?? 非本进程组地图

	// trans npc exclusive
	//zeng 注释到该调用
	//MapGroup(PID)->QueryIntraMsg()->DelTransNpc(GetInt(NPCDATA_LINKID));	// del this mapgroup first

	m_pMap->EnterRoom(QueryMapThing(), WITH_BLOCK);
	UpdateHoldFloor(true);
	//SendSelfToBlock();
	UpdateBroadcastSet();

	// set map syn id
	if(IsSynFlag())
	{
		//GetMap()->SetSynID(GetSynID(), true);		// with all map npc
//		CheckFightTime();

		// temporary code
// 		OBJID idMoreMap = GetData(SYNFLAGNPC_MOREMAPID);
// 		if(idMoreMap != ID_NONE)
// 		{
// 			CGameMap* pMoreMap = MapManager()->QueryMap(idMoreMap);
// 			if(pMoreMap)
// 				pMoreMap->SetSynID(GetSynID(), true);		// with all map npc
// 		}
	}

	return true;
}

bool CNpc::CreateSynMapNpc(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName)
{
	m_idProcess = idProcess;
	m_bDynaNpc	= true;

	CHECKF(!m_pData);
	CDynaNpcData* pData = CDynaNpcData::CreateNew();
	CHECKF(pData);
	m_pData	= pData->QueryInterface();
	IF_NOT(m_pData && m_pData->Create(GameDataDefault()->GetDynaNpc(), ID_NONE))			// idRecord))
		return false;

	m_pData->SetInt(NPCDATA_TYPE,		pInfo->nType);
	m_pData->SetStr(NPCDATA_NAME,		szName, _MAX_NAMESIZE);
	m_pData->SetInt(NPCDATA_LOOKFACE,	pInfo->nLookFace);
	m_pData->SetInt(NPCDATA_MAPID,		pInfo->idMap);
	m_pData->SetInt(NPCDATA_POSX_,		pInfo->nPosX);
	m_pData->SetInt(NPCDATA_POSY_,		pInfo->nPosY);
	m_pData->SetInt(NPCDATA_SORT,		pInfo->nSort);
	m_pData->SetInt(NPCDATA_ACTION,		pInfo->nAction);

	m_pData->SetInt(NPCDATA_LEVEL,pInfo->nLev);
	m_id	= MapManager()->SpawnNewNpcID();
	m_nPosX = m_pData->GetInt(NPCDATA_POSX_);
	m_nPosY = m_pData->GetInt(NPCDATA_POSY_);
    
	m_startPosX = m_nPosX;
	m_startPosY = m_nPosY;
	m_nNpcType = pInfo->nNpcType;
	
	if (this->IsShopNpc() || IsTaskShopNpc() || IsRandTaskShopNpc())
	{
		IF_NOT(CreateFuncObj(idProcess))
			return false;
	}

	IF_NOT(CheckSortMutex())
		return false;
	
	m_setStatus = CStatusSet::CreateNew(true);
	CHECKF(m_setStatus);
	m_pMap = MapManager()->QueryMap(m_pData->GetInt(NPCDATA_MAPID));
	IF_NOT(m_pMap)
		return false;			//?? 非本进程组地图

	m_pMap->EnterRoom(QueryMapThing(), WITH_BLOCK);
	UpdateHoldFloor(true);
	UpdateBroadcastSet();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::Create(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName)
{
	m_idProcess = idProcess;
	m_bDynaNpc	= true;

	ASSERT(!m_pData);
	CDynaNpcData* pData = CDynaNpcData::CreateNew();
	if(pData)
		m_pData	= pData->QueryInterface();
	IF_NOT(m_pData && m_pData->Create(GameDataDefault()->GetDynaNpc(), ID_NONE))			// idRecord))
		return false;


/*	m_pData->SetInt(NPCDATA_OWNERID,	pInfo->idOwner);
	m_pData->SetInt(NPCDATA_OWNERTYPE,	pInfo->nOwnerType);*/
	m_pData->SetInt(NPCDATA_TYPE,		pInfo->nType);
	m_pData->SetStr(NPCDATA_NAME,		szName, _MAX_NAMESIZE);
	m_pData->SetInt(NPCDATA_LOOKFACE,	pInfo->nLookFace);
	m_pData->SetInt(NPCDATA_MAPID,		pInfo->idMap);
	m_pData->SetInt(NPCDATA_POSX_,		pInfo->nPosX);
	m_pData->SetInt(NPCDATA_POSY_,		pInfo->nPosY);
	m_pData->SetInt(NPCDATA_SORT,		pInfo->nSort);
	m_pData->SetInt(NPCDATA_ACTION,		pInfo->nAction);
	//m_pData->InsertRecord();
	m_pData->SetInt(NPCDATA_LEVEL,pInfo->nLev);
	m_id	= MapManager()->SpawnNewNpcID();
	m_nPosX = m_pData->GetInt(NPCDATA_POSX_);
	m_nPosY = m_pData->GetInt(NPCDATA_POSY_);
    
	m_startPosX = m_nPosX;
	m_startPosY = m_nPosY;
	m_nNpcType = pInfo->nNpcType;
	
	if (this->IsShopNpc() || IsTaskShopNpc() || IsRandTaskShopNpc())
	{
		IF_NOT(CreateFuncObj(idProcess))
			return false;
	}

	IF_NOT(CheckSortMutex())
		return false;

	m_setStatus = CStatusSet::CreateNew(true);
	CHECKF(m_setStatus);
	m_pMap = MapManager()->QueryMap(m_pData->GetInt(NPCDATA_MAPID));
	IF_NOT(m_pMap)
		return false;			//?? 非本进程组地图

	// trans npc exclusive
	//zeng 注释到该调用
	//MapGroup(PID)->QueryIntraMsg()->DelTransNpc(GetInt(NPCDATA_LINKID));	// del this mapgroup first

	m_pMap->EnterRoom(QueryMapThing(), WITH_BLOCK);
	UpdateHoldFloor(true);
	//SendSelfToBlock();
	UpdateBroadcastSet();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::Create(PROCESS_ID idProcess, IRecordset* pRecordset, bool bDynaNpc)
{
	m_idProcess = idProcess;
	m_bDynaNpc	= bDynaNpc;

	ASSERT(!m_pData);
	if(bDynaNpc)
	{
		CDynaNpcData* pData = CDynaNpcData::CreateNew();
		if(pData)
			m_pData	= pData->QueryInterface();
	}
	else
	{
		CNpcData* pData = CNpcData::CreateNew();
		if(pData)
			m_pData	= pData->QueryInterface();
	}
	IF_NOT(m_pData && m_pData->Create(pRecordset))
		return false;

	CGameMap* pMap = MapManager()->QueryMap(m_pData->GetInt(NPCDATA_MAPID), false);		// false: not load dynamic map
	if(!pMap)
		return false;			//?? 非本进程组地图，或者动态地图的NPC

	if(bDynaNpc)
		m_id	= MapManager()->SpawnNewNpcID();
	else
		m_id	= m_pData->GetID();					// fixed to database id
	m_nPosX = m_pData->GetInt(NPCDATA_POSX_);
	m_nPosY = m_pData->GetInt(NPCDATA_POSY_);

	m_startPosX = m_nPosX;
	m_startPosY = m_nPosY;

	if (this->IsShopNpc() || IsTaskShopNpc() || IsRandTaskShopNpc())
	{
 		IF_NOT(CreateFuncObj(idProcess))
			return false;
	}

// 	IF_NOT(CheckSortMutex())
// 		return false;

	m_setStatus = CStatusSet::CreateNew(true);
	CHECKF(m_setStatus);

	m_pMap = pMap;

	// 冗错非法NPC坐标
	if(!GetMap()->IsValidPoint(GetPosX(), GetPosY()))
	{
		LOGERROR("NPC数据错[%u]，坐标非法[%d,%d]", GetRecordID(), GetPosX(), GetPosY());
		m_pMap	= NULL;
		return false;
	}

	m_pMap->EnterRoom(QueryMapThing(), WITH_BLOCK);

//	UpdateHoldFloor(true);
	UpdateBroadcastSet();

	// set map syn id
// 	if(IsSynFlag())
// 	{
// 		GetMap()->SetSynID(GetSynID(), false);		// false: no set npc in map
// 		CheckFightTime();

		// temporary code
// 		OBJID idMoreMap = GetData(SYNFLAGNPC_MOREMAPID);
// 		if(idMoreMap != ID_NONE)
// 		{
// 			CGameMap* pMoreMap = MapManager()->QueryMap(idMoreMap);
// 			if(pMoreMap)
// 				pMoreMap->SetSynID(GetSynID(), false);		// false: no set npc in map
// 		}
//	}

//	if(!IsDynaNpc() && GetMaxLife() > 0)
//	{
//		LOGERROR("静态NPC不能设置最大血为非0：[%d]", GetID());
//	}

	return true;
}

/////////////////////////////////////////////////////////////////////
BOOL CNpc::	ChangePos (OBJID idMap, int nPosX, int nPosY)
{
	if (!MapManager()->QueryMap(idMap))
		return false;
	
//	this->SendLeaveFromBlock();
	m_pMap->LeaveRoom(QueryMapThing(), WITH_BLOCK);
	UpdateHoldFloor(false);

	m_pMap = MapManager()->QueryMap(idMap);
	m_nPosX = nPosX;
	m_nPosY = nPosY;

	m_pMap->EnterRoom(QueryMapThing(), WITH_BLOCK);
	UpdateHoldFloor(true);
	UpdateBroadcastSet();

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::CreateFuncTrunk(PROCESS_ID idProcess)
{
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::CreateFuncObj(PROCESS_ID idProcess)
{
	// shop
	if(IsShopNpc() || IsTaskShopNpc() || IsRandTaskShopNpc())
	{
		ASSERT(!m_pShop);
		m_pShop = CNpcShop::CreateNew();
		int id = GetID();
		if(id > 10000)
			id = GetNpcType();
		IF_NOT(m_pShop && m_pShop->Create(idProcess, this, id))
			return false;
	}
	return true;		//@
}

int CNpc::GetMountTypeID()
{ return /*IsShelfNpc() ? QueryTrunk()->QueryPackage()->GetItemType(0) : m_pData->GetInt(STATUARYDATA_HELMET)*/0; }
int CNpc::GetArmorTypeID()
{ return /*IsShelfNpc() ? QueryTrunk()->QueryPackage()->GetItemType(1) : m_pData->GetInt(STATUARYDATA_ARMOR)*/0; }
int CNpc::GetWeaponRTypeID()
{ return /*IsShelfNpc() ? QueryTrunk()->QueryPackage()->GetItemType(2) : m_pData->GetInt(STATUARYDATA_WEAPONR)*/0; }
int CNpc::GetWeaponLTypeID()
{ return /*IsShelfNpc() ? QueryTrunk()->QueryPackage()->GetItemType(3) : m_pData->GetInt(STATUARYDATA_WEAPONL)*/0; }
//////////////////////////////////////////////////////////////////////
void CNpc::SendShow(IRole* pRole)
{
	if(IsShow())
	{
		CMsgPlayer msg;
		IF_OK(msg.Create(QueryRole()))
			pRole->SendMsg(&msg);
	}
// 	else if(IsDynaNpc() && GetMaxLife() > 0)
// 	{
// 		CMsgNpcInfoEx	msg;
// 		IF_OK(msg.Create(this))
// 			pRole->SendMsg(&msg);
// 	}
// 	else
// 	{
// 		CMsgNpcInfo	msg;
// 		IF_OK(msg.Create(this))
// 			pRole->SendMsg(&msg);
// 	}
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
bool CNpc::SendSelfToBlock()
{
	CHECKF(GetMap());

	CMsgPlayer msg;
	IF_OK(msg.Create(QueryRole()))
		GetMap()->BroadcastBlockMsg(QueryMapThing(), &msg);
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::SendLeaveFromBlock()
{
	CHECKF(GetMap());

	CMsgAction msg;
	if (msg.Create(GetID(), 0, 0, 0, actionDisappear, 0))
		GetMap()->BroadcastBlockMsg(QueryMapThing(), &msg);

// 	CMsgNpc	msg;
// 	CHECKF(msg.Create(EVENT_LEAVEMAP, GetID()));
// 	GetMap()->BroadcastBlockMsg(QueryMapThing(), &msg);

	return true;
}

bool CNpc::ActivateShop(IRole* pRole)
{
	CUser* pUser = NULL;
	if(pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		if(IsTaskShopNpc() || IsRandTaskShopNpc())
		{
			DEBUG_TRY
			QueryShop()->SendInfo(pUser);
			DEBUG_CATCH2("QueryShop()->SendInfo(%u)",pUser->GetID())
			return true;
		}
	}
	return false;
}


bool CNpc::ActivateNpc(IRole* pRole, int nAction)
{
	CUser* pUser = NULL;
	if(pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		if(IsShopNpc())
		{
			QueryShop()->SendInfo(pUser);
		}
		else
		{
			GameAction()->ProcessAction(m_pData->GetInt(NPCDATA_ACTION), pUser,this->QueryRole());
		}
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
OBJID CNpc::GetMapID()
{
	CHECKF(m_pMap);
	int mapid = m_pMap->GetID();
	return m_pMap->GetID();
}

//////////////////////////////////////////////////////////////////////
void CNpc::OnTimer(time_t tCurr)
{
	if(!m_tUpdateShow.IsActive())
		m_tUpdateShow.Startup(60);
	else
	{
		if(!m_tUpdateShow.ToNextTime())
			return;
	}

	DEBUG_TRY
	int timeBe = GetInt(NPCDATA_SHOWTIME);
	int timeEn = GetInt(NPCDATA_HIDETIME);
	int showSpan = GetInt(NPCDATA_SHOWSPAN);
	int hideSpan = GetInt(NPCDATA_HIDEMSPAN);

	if((timeBe > 0) && (timeEn > timeBe))
	{
		CTime ThisTime(CTime::GetCurrentTime());
		int thisCheckTime = ThisTime.GetHour() * 100 + ThisTime.GetMinute();
		if(thisCheckTime < timeBe  )
		{
			if(!m_bSetShow)
				m_bSetShow = true;
			if(m_bNPCShow)
			{
				m_bNPCShow = false;
				ClrBroadcastSet();
			}
			return;
		}
		else if(thisCheckTime >= timeEn)
		{
			if(!m_bSetShow)
				m_bSetShow = true;
			if(m_bNPCShow)
			{
				m_bNPCShow = false;
				ClrBroadcastSet();
			}
			return;
		}
		else
		{
			int PassMin = (thisCheckTime / 100 - timeBe / 100) * 60 + thisCheckTime % 100 - timeBe % 100;
			PassMin %= showSpan + hideSpan;
			
			if(PassMin < showSpan)
			{
				if(!m_bSetShow)
				{
					if(m_bNPCShow)
					{
						m_bNPCShow = false;
						ClrBroadcastSet();
					}
				}
				else
				{
					if(!m_bNPCShow)
					{
						m_bNPCShow = true;
						ClrBroadcastSet();
						UpdateBroadcastSet();
					}
				}
			}
			else
			{
				if(!m_bSetShow)
					m_bSetShow = true;
				if(m_bNPCShow)
				{
					m_bNPCShow = false;
					ClrBroadcastSet();
					
				}
			}
			return;
		}
	}

	if(!m_bNPCShow)
	{
		m_bNPCShow = true;
		ClrBroadcastSet();
		UpdateBroadcastSet();
	}
	DEBUG_CATCH("CNpc::OnTimer(time_t tCurr)");

	DEBUG_TRY
	if(IsRandTaskShopNpc())
		m_pShop->DoRandPrice();
	DEBUG_CATCH3("IsRandTaskShopNpc npcid = %u,npctype = %u",GetID(),GetNpcType());
}

//////////////////////////////////////////////////////////////////////
void CNpc::BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf,EM_SENDNPCFLAG sendNpc/* = EM_SENDNPCFLAG_TRUE*/)
{
	CRole::BroadcastRoomMsg(pMsg, bSendSelf,sendNpc);
	CHECK(GetMap());

	GetMap()->BroadcastBlockMsg(QueryMapThing(), pMsg, bSendSelf);
}

//////////////////////////////////////////////////////////////////////
void CNpc::BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf)
{
	CMsgTalk	msg;
	if(msg.Create(GetName(), ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
		this->BroadcastRoomMsg(&msg, bSendSelf);
}

//////////////////////////////////////////////////////////////////////
void CNpc::BeKill(IRole* pTarget /*= NULL*/)
{
	CHECK(!"IsDynaNpc()");

	if(IsDeleted())
		return ;

	// die action
// 	if(IsDieAction())
// 	{
// 		OBJID idAction = m_pData->GetInt(NPCDATA_LINKID);
// 		CHECK(idAction);
// 
// 		CUser* pUser=NULL;
// 		if(pTarget)
// 			pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
// 
// 		GameAction()->ProcessAction(idAction, pUser, QueryRole());
// 	}
// 	else
// 	{
		this->SetStatus(STATUS_DIE);
		ASSERT(DelNpc());
//	}
}

//////////////////////////////////////////////////////////////////////
bool CNpc::DelNpc()			// call this mast !IsDeleted()
{
	if(!((IsDynaNpc() || (GetInt(NPCDATA_SORT) == _TASK_NPC_NEW)) && !IsDeleted()))
	{
		LogSave("npc del error! id = %u,typeid = %u",GetID(),GetNpcType());
		return false;
	}

	m_tDie.Startup(DYNANPCDIEDELAY_SECS);
	SendLeaveFromBlock();		//??? show leave map now!
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::BeAttack(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable/*=true*/)
{
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::AddAttrib(int idxAttr, __int64 i64Data, int nSynchro)
{
	LogSave("CNpc::AddAttrib %u %u",idxAttr,GetID());
	return true;
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::SaveInfo()
{
// 	if(m_pData && IsDynaNpc())
// 		m_pData->Update();
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::TransPos(int nPosX, int nPosY)
{
/*
	CHECK(IsDynaNpc());

	this->SendLeaveFromBlock();
	GetMap()->LeaveRoom(QueryMapThing(), WITH_BLOCK);

	m_nPosX	= nPosX;
	m_nPosY	= nPosY;
	m_pData->SetInt(NPCDATA_POSX, nPosX_);
	m_pData->SetInt(NPCDATA_POSY, nPosY_);
	if(IsDynaNpc())
		SaveInfo();

	GetMap()->EnterRoom(QueryMapThing(), WITH_BLOCK);
	SendSelfToBlock();
*/
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::SetStatus(int nStatus, bool bSynchro, int nPower)
{
//	I64	i64OldEffect = GetEffect();
//	switch (nStatus)
//	{
//	case	STATUS_NORMAL:
//		{
//			m_i64Effect	= KEEPEFFECT_NORMAL;
//		}
//		break;
//	}
//	if(bSynchro && i64OldEffect != GetEffect())
//	{
//		CMsgUserAttrib msg;
//		if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
//		{
//			if (STATUS_MAXLIFE == nStatus)
//			{
//				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
//				msg.Append(_USERATTRIB_LIFE, this->GetLife());
//			}
//			BroadcastRoomMsg(&msg, INCLUDE_SELF);
//		}
//	}
}


/////////////////////////////////////////////////////////////////////////////
void CNpc::ClsStatus(int nStatus, bool bSynchro)
{
//	if(bSynchro && i64OldEffect != GetEffect())
//	{
//		CMsgUserAttrib msg;
//		if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, GetEffect()))
//		{
//			if (STATUS_MAXLIFE == nStatus)
//			{
//				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
//				msg.Append(_USERATTRIB_LIFE, this->GetLife());
//			}
//			BroadcastRoomMsg(&msg, INCLUDE_SELF);
//		}
//	}
}

/////////////////////////////////////////////////////////////////////////////
CItem* CNpc::Recycle(OBJID idUser)
{
	//06.12.29王玉波暂时注释
/*	CHECKF(IsRecycleEnable());

	OBJID idType = GetInt(NPCDATA_ITEMTYPE);
	if(idType != 0)
	{
		CItem* pItem = CItem::CreateNew();
		IF_OK(pItem)
		{
			pItem->Create(GameDataDefault()->GetGameItemData(), idType, idUser, ITEMPOSITION_BACKPACK1, GameDataDefault()->GetEudemonData());
			if(IsDynaNpc() && GetMaxLife() > 0 && GetLife() < GetMaxLife())
				pItem->SetInt(ITEMDATA_DATA, GetLife());
			return pItem;
		}
		else
			pItem->ReleaseByOwner();
	}*/

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::IsOwnerOf(CUser* pUser, bool bMateEnable/*=false*/)
{
	CHECKF(pUser);

/*	if(IsSynNpc() && GetInt(NPCDATA_OWNERID) == pUser->GetSynID() && pUser->GetSynRankShow() == RANK_LEADER)
		return true;

	if(IsUserNpc() && GetInt(NPCDATA_OWNERID) == pUser->GetID())
		return true;

	if(bMateEnable && IsUserNpc() && GetInt(NPCDATA_OWNERID) == pUser->GetMateID())
		return true;*/

	return false;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::IsSynMoneyEmpty()
{
	CHECKF(IsSynFlag());

	//去掉帮派基金 satan 2009.03.06
//	CSyndicate* pSyn = SynManager()->QuerySyndicate(GetInt(NPCDATA_OWNERID));
//	if(pSyn)
//	{
//		if(pSyn->GetInt(SYNDATA_MONEY) > 0)
//			return false;
//	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::CheckSortMutex()
{
	DWORD	dwSort = GetSort() & NPCSORT_USELINKID;

	if(GetType() == _SYNTRANS_NPC && dwSort != 0)
		return false;

	for(int i = 0; i < 32; i++)
	{
		if(dwSort == 0)
			return true;

		if((dwSort & 1) != 0 && (dwSort >>= 1) != 0)
		{
			ASSERT(!"_TBL_NPC(_TBL_DYNANPC) link id mutex!!!");
			return false;
		}
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
#include <map>
using namespace std;
void CNpc::BroadcastScore()
{
	typedef	multimap<int, OBJID>	TOP_SET;
	TOP_SET	setTop;

	OBJID idNpc = GetID();
	for(int i = 0; i < SynManager()->QuerySynSet()->GetAmount(); i++)
	{
		CSyndicate* pSyn = SynManager()->QuerySynSet()->GetObjByIndex(i);
		if(pSyn)
			setTop.insert(TOP_SET::value_type(pSyn->GetSynWarScore(idNpc), pSyn->GetID()));
	}

	int	nChannel = _TXTATR_SYNWAR_FIRST;
	i=1;
	for(TOP_SET::reverse_iterator iter = setTop.rbegin(); iter != setTop.rend() && i <= 5; iter++, i++)
	{
		OBJID	idSyn	= iter->second;
		int		nScore	= iter->first;
		if(nScore == 0)
			break;
		CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
		if(!pSyn)
			continue;

		MSGBUF	szMsg;
		sprintf(szMsg, STR_TOP_LIST_dsd, i, pSyn->GetStr(SYNDATA_NAME), nScore);
		CMsgTalk	msg;
		IF_OK(msg.Create(nChannel, szMsg))
			UserManager()->BroadcastMapMsg(GetMap()->GetID(), &msg);

		nChannel = _TXTATR_SYNWAR_NEXT;
	}
}

/////////////////////////////////////////////////////////////////////////////
LPCTSTR	CNpc::GetName()
{
/*	if(IsSynFlag() && GetInt(NPCDATA_OWNERID) != ID_NONE)
	{
		CSyndicate* pSyn = SynManager()->QuerySyndicate(GetInt(NPCDATA_OWNERID));
		if(pSyn)
			return pSyn->GetStr(SYNDATA_NAME);
	}*/

	return m_pData->GetStr(NPCDATA_NAME);
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::IsBeAtkable()
{
//	if(IsSynFlag() && !GetMap()->IsWarTime())
//		return false;

	return false;//IsDynaNpc() && GetMaxLife() > 0;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::SetSynOwnerID(OBJID idSyn, BOOL bWithLinkMap)		// true: with link map
{
/*	if(GetInt(NPCDATA_OWNERTYPE) != OWNER_SYN)
		return false;

	SetInt(NPCDATA_OWNERID, idSyn, true);*/

// 	if(bWithLinkMap && IsLinkNpc())
// 	{
// 		CGameMap* pMap = QueryLinkMap()->GetLinkMap();
// 		IF_OK(pMap)
// 			pMap->SetSynID(idSyn, true);		// true: with all npc in map
// 	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::SetUserOwnerID(OBJID idUser, BOOL bWithLinkMap/*=false*/)		// true: with link map
{
/*	if(GetInt(NPCDATA_OWNERTYPE) != OWNER_USER)
		return false;

	SetInt(NPCDATA_OWNERID, idUser, true);*/

// 	if(bWithLinkMap && IsLinkNpc())
// 	{
// 		CGameMap* pMap = QueryLinkMap()->GetLinkMap();
// 		IF_OK(pMap)
// 			pMap->SetUserID(idUser, true);		// true: with all npc in map
// 	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CNpc::IsDelEnable()
{
	if( (GetSort() & NPCSORT_DELENABLE) == 0 )
		return false;

// 	if(IsTrunkNpc() && !QueryTrunk()->IsEmpty())
// 		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
void CNpc::UpdateHoldFloor(bool bEnter)
{
	CHECK(GetMap());

	int nDirFront	= GetDir();
	int nDirBack	= (GetDir() + 4) % 8;
	int nDirLeft	= (GetDir() + 8 - 2) % 8;
	int nDirRight	= (GetDir() +2) % 8;

// 	POINT posFront, posBack, posLeft, posRight;
// 	posFront.x = GetPosX() + _DELTA_X[nDirFront] * this->GetInt(NPCDATA_SIZE_FRONT);
// 	posFront.y = GetPosY() + _DELTA_Y[nDirFront] * this->GetInt(NPCDATA_SIZE_FRONT);
// 
// 	posBack.x = GetPosX() + _DELTA_X[nDirBack] * this->GetInt(NPCDATA_SIZE_BACK);
// 	posBack.y = GetPosY() + _DELTA_Y[nDirBack] * this->GetInt(NPCDATA_SIZE_BACK);
// 	
// 	posLeft.x = GetPosX() + _DELTA_X[nDirLeft] * this->GetInt(NPCDATA_SIZE_LEFT);
// 	posLeft.y = GetPosY() + _DELTA_Y[nDirLeft] * this->GetInt(NPCDATA_SIZE_LEFT);
// 	
// 	posRight.x = GetPosX() + _DELTA_X[nDirRight] * this->GetInt(NPCDATA_SIZE_RIGHT);
// 	posRight.y = GetPosY() + _DELTA_Y[nDirRight] * this->GetInt(NPCDATA_SIZE_RIGHT);

// 	int nMinX = __min(__min(posFront.x, posBack.x), __min(posLeft.x, posRight.x));
// 	int nMinY = __min(__min(posFront.y, posBack.y), __min(posLeft.y, posRight.y));
// 	
// 	int nMaxX = __max(__max(posFront.x, posBack.x), __max(posLeft.x, posRight.x));
// 	int nMaxY = __max(__max(posFront.y, posBack.y), __max(posLeft.y, posRight.y));
// 
// 	for (int i=nMinX; i<=nMaxX; i++)
// 	{
// 		for (int j=nMinY; j<=nMaxY; j++)
// 		{
// 			if (GetMap()->IsValidPoint(i, j)
// 				&& !(GetPosX() == i && GetPosY() == j))
// 			{
// 				if (bEnter)
// 					GetMap()->IncRole(i, j);
// 				else
// 					GetMap()->DecRole(i, j);
// 			}
// 		}
// 	}
}

bool CNpc::EnterMapGroup()
{
	CMapPtr pMap = this->GetMap();
	CHECKF(pMap);

	clock_t clDebugBegin1 = clock();
	UpdateBroadcastSet();
	clock_t clDebugUsed1 = clock() - clDebugBegin1;
#ifdef _DEBUG
	if(clDebugUsed1 > 30)
	{
		::LogSave("overTime(%d)::when UpdateBroadcastSet "
			"id:%d ",
			clDebugUsed1,
			this->GetID());
	}
#endif
	
//	MapGroup(PID)->QueryIntraMsg()->SetNpcProcessID(GetID(), true);			// 同步SOCKET_KERNEL

	
	pMap->EnterRoom(this->QueryMapThing(), WITH_BLOCK);

	clock_t clDebugBegin2 = clock();
	{
		CMsgAction	msg;
		IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionSoundEffect, GetType()))
		{
			_UserManager(PID)->BroadcastMapMsg(pMap->GetID(), &msg);
		}
	}
clock_t clDebugUsed2 = clock() - clDebugBegin2;
#ifdef _DEBUG
	if(clDebugUsed2 > 30)
	{
		::LogSave("overTime(%d)::when BroadcastMapMsg "
			"id:%d ",
			clDebugUsed2,
			this->GetID());
	}
#endif 
	//m_bLeaveMap = false;

	return true;
}

void CNpc::SetNpcOwner(OBJID idUser)
{
	m_nToken = idUser;
}

//本函数只能在星期之间通用
bool CNpc::IsTimeDuring(unsigned long nBegin, unsigned long nEnd, time_t tCurr/* =0 */)
{
	if(tCurr == 0)
		tCurr = time(NULL);
	
	//如果不相等这样判断的目的如下：
	//开始时间为当前星期2:12:30
	//下星期为星期1：6：30
	tm* pTm = localtime(&tCurr);
	CHECKF(pTm);

	int nBeginDay = nBegin / 10000; //开始星期
	int nEndDay = nEnd /10000; //结束星期
	int weekday = pTm->tm_wday; //当前星期

	int nCurr = pTm->tm_hour * 100 + pTm->tm_min; //当前时间
	int nBeginTime = nBegin % 10000; //开始时间
	int nEndTime = nEnd % 10000; //结束时间

	if(nEndDay > 6)  //没有结束时间
		return true;

	if(nBeginDay == nEndDay) //同天的情况
	{
		if(nBeginTime <= nEndTime) //开始时间小于结束时间,表示同一天
		{
			if(nBeginTime <= nCurr && nCurr <= nEndTime)
			{
				return true;
			}
		}
		else
			if(nBeginTime > nEndTime) //下个星期的同一天
			{
				if(weekday != nEndDay ||(weekday == nEndDay && nCurr < nEndTime))
					return true;
			}
	}
	else//nBeginTime>=nEndTime非同一天
	{
		//这里情况为两种 nBeginDay < nEndDay 和 nBeingDay > nEndDay
        //不管是那种情况，只需要判断是否当前星期和结束星期不相等或是相等同时
		//不是同一天的时候
		if(weekday != nEndDay || (weekday == nEndDay && nCurr < nEndTime)) //结束时间和当前时间不相等或者相等同时当前时间小于结束时间
			return true;
	}
	return false;
}

void CNpc::SetDelShow()
{
	m_bSetShow = false;
}

bool CNpc::IsShow()
{
	return m_bNPCShow;
}

//是否激活NPC移动控制
bool CNpc::IsActive(unsigned long nBegin)
{
	time_t tCurr = time(NULL);
	tm* pTm = localtime(&tCurr);
	CHECKF(pTm);
	
	int nBeginDay = nBegin / 10000; //开始星期
	int weekday = pTm->tm_wday; //当前星期
	
	int nCurr = pTm->tm_hour * 100 + pTm->tm_min; //当前时间
	int nBeginTime = nBegin % 10000; //开始时间

#ifdef _DEBUG
	int hour = nBeginTime /100;
	int minu = nBeginTime %100;
	int tX = GetPosX();
	int tY = GetPosY();
#endif
	//开始激活NPC
	if(nBeginDay == weekday && nCurr >= nBeginTime)
	{
		m_bTimeDuring = true;
	}
	return m_bTimeDuring;
}

bool  CNpc::MovePos(int posX,int poxY)
{
	if(!GetMap()->IsValidPoint(posX, poxY))
	{
		LOGERROR("NPC数据错[%u]，坐标非法[%d,%d]", GetRecordID(), posX, poxY);
		return false;
	}
	//是否能站立的判断
	CMapPtr pMap = GetMap();
	if (pMap)
	{
		//是否能站立的判断
		if(!pMap->IsStandEnable(posX, poxY))
		{
			return false;
		}
					
		IMapThing* pMapThing = NULL;
		//目标块上是否有物体
		CGameBlock* gameBlockTemp = pMap->QueryBlock(posX, poxY);
		CHECKF(gameBlockTemp);
		IThingSet* pSet = gameBlockTemp->QuerySet();
		//-------------------------------------
					
		for(int i = 0; i < pSet->GetAmount(); i++)
		{
			IMapThing* pTheMapThing = pSet->GetObjByIndex(i);
			if (pTheMapThing && 									
							pTheMapThing->GetObjType() == OBJ_NPC &&
							pTheMapThing->GetPosX() == posX && pTheMapThing->GetPosY() == poxY)
			{
				pMapThing = pTheMapThing;
				break;		
			}
		}
		//如果可以站立,飞坐标，否则返回
		if (!pMapThing)
		{
			ChangePos(GetMapID(),posX,poxY);
						
// 			char temp[300] = {0};
// 			sprintf(temp, g_objStrRes.GetStr(27006), pMap->GetName(), m_pData->GetInt((NPCDATA)NPCDATA_MOVEPOS)/1000, m_pData->GetInt((NPCDATA)NPCDATA_MOVEPOS)%1000);
// 						
// 			CMsgTalk msg;
// 			if (msg.Create(SYSTEM_NAME, GetName(), temp, NULL, 0xff0000, _TXTATR_SYSTEM))
// 							RoleManager()->BroadcastMsg(&msg);
		}
		else
			return false;
	}
    return true;
}
