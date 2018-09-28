// GameMap.cpp: implementation of the CGameMap class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4786)

#include "GameObj.h"
#include "GameMap.h"
#include <algorithm>
#include "AllMsg.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "I_MapData.h"
#include "MapGroup.h"
#include "BaseFunc.h"
#include "WeatherRegion.h"
#include "RoleManager.h"
#include "Array.h"

//////////////////////////////////////////////////////////////////////
#define		MAPDATA_VERSION		((LPCTSTR)true)					// DDVersion
const DWORD	DEFAULT_LIGHTRGB		= 0xFFFFFFFF;
extern int g_SpecilLine;
//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CGameMap,s_heap)
char	szMapTable[]		= _TBL_MAP;
char	szPasswayTable[]	= _TBL_PASSWAY;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameMap::CGameMap()
{
	SetObjType(OBJ_MAP);

	m_idProcess = PROCESS_NONE;

	m_pData		= NULL;
	m_pMapData	= NULL;
	m_pGenData  = NULL;
	m_nUserAmount = 0;
}

//////////////////////////////////////////////////////////////////////
CGameMap::~CGameMap()
{
	if(m_pData)
	{
		if(IsDynaMap())
			m_pData->Update();
		m_pData->Release();
	}

	if(m_pGenData)
		m_pGenData->Release();
	m_pMapData = NULL;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::Create(PROCESS_ID idProcess, IRecordset* pRes)
{
	m_idProcess = idProcess;

	m_pData	= CGameMapData::CreateNew();
	if(!m_pData || !m_pData->Create(pRes))
		return false;
//*
		LOGMSG("加载地图文件[%d]...", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
		//zeng添加，查询地图模板是否存在，如果存在，直接使用 不用创建
		m_pMapData = MapManager()->GetMapDataTemplate(m_pData->GetInt(GAMEMAPDATA_MAPDOC));
		if(!m_pMapData)
		{
			m_pMapData	= IMapData::CreateNew(m_pData->GetInt(GAMEMAPDATA_MAPDOC), MAPDATA_VERSION);
			if(m_pMapData)
				MapManager()->AddMapData(m_pData->GetInt(GAMEMAPDATA_MAPDOC),m_pMapData);
		}
		
		if(!m_pMapData)
		{
			LOGERROR("地图文件[%d]加载失败！", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
			return false;
		}

		// 装载地图
		m_setBlock.resize(GetWidthOfBlock());
		for(int i = 0; i < m_setBlock.size(); i++)
		{
			m_setBlock[i].resize(GetHeightOfBlock());
			for(int j = 0; j < m_setBlock[i].size(); j++)
				m_setBlock[i][j].Create();
		}
		

	DEBUG_TRY
	CreateGenData();
	DEBUG_CATCH2("CreateGenData() mapid = %u",GetID())
	return true;
}

void CGameMap::CreateGenData()
{
	if(IsDynaMap())
		return;

	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s WHERE  mapid=%u", _TBL_GENERATOR, GetID());
	::CAutoPtr<IRecordset> pRes = Database()->CreateNewRecordset(szSQL, false);
	if(!pRes)
		return;
	if(pRes->RecordCount() < 1)
		return;

	m_pGenData	= CNpcGenSet::CreateNew(true);
	CHECK(m_pGenData);
	for(int i = 0; i < pRes->RecordCount(); i++,pRes->MoveNext())
	{
		CNpcGenerator* pGen = CNpcGenerator::CreateNew();		// VVVVVVVVV
		CHECK(pGen);

		if(!pGen->Create(pRes))
		{
			pGen->ReleaseByOwner();
			return;
		}
		m_pGenData->AddObj(pGen);
	}
}

void CGameMap::GeneratorMonsterDel(OBJID idGen)
{
	CHECK(m_pGenData);
	INpcGenSet::Iterator pGenSet = m_pGenData->NewEnum();
	while(pGenSet.Next())
	{
		if(pGenSet->GetID() == idGen)
		{
			pGenSet->DecGenNum();
			break;
		}
	}
}

void CGameMap::GeneratorMonster()
{
	CHECK(m_pGenData);
	INpcGenSet::Iterator pGenSet = m_pGenData->NewEnum();
	while(pGenSet.Next())
	{
		CNpcGenerator* pGen = pGenSet;
		if(pGen)
		{
			if(pGen->GetGenNum() >= pGen->GetInt(NPCGENDATA_MAXNPC))
				continue;

			if(!pGen->IsTimeOut())
				continue;
			for(int i = 0 ; i < 10 ; i ++)
			{
				if(pGen->GetGenNum() < pGen->GetInt(NPCGENDATA_MAXNPC))
				{
					if(CreateMonster(GetID()
						,pGen->GetInt(NPCGENDATA_BOUNDX) + ::RandGet(pGen->GetInt(NPCGENDATA_BOUNDCX))
						,pGen->GetInt(NPCGENDATA_BOUNDY) + ::RandGet(pGen->GetInt(NPCGENDATA_BOUNDCY))
						,pGen->GetInt(NPCGENDATA_NPCTYPE)
						,pGen->GetID()) != ID_NONE)
					{
						pGen->AddGenNum();
					}
				}
				else
					break;
			}
		}
	}
}

OBJID CGameMap::CreateMonster(OBJID idMap,int x,int y,int type,OBJID idGen)
{
	ST_CREATENEWNPCNEW stData;
	stData.idMap = idMap;
	stData.usPosX = x;
	stData.usPosY = y;
	stData.usType = type;
	stData.idGen = idGen;
	return NpcManager()->CreateMonster(stData);
}

//////////////////////////////////////////////////////////////////////////
OBJID CGameMap::CreateSpecilMap(PROCESS_ID idProcess,const NewMapInfo* pInfo)
{
	
	m_idProcess		= idProcess;
	CHECKF(pInfo);
	
	m_pData	= CGameMapData::CreateNew();
	if(!m_pData || !m_pData->Create(GameDataDefault()->GetSynMap(), ID_NONE))
		return false;
	m_pData->SetStr(GAMEMAPDATA_NAME, pInfo->szName, _MAX_NAMESIZE);
	m_pData->SetInt(GAMEMAPDATA_MAPDOC,			pInfo->nMapDoc);
	m_pData->SetInt(GAMEMAPDATA_TYPE,		pInfo->nType);
	m_pData->SetInt(GAMEMAPDATA_MAPGROUP,		pInfo->nMapGroup);
	m_pData->SetInt(GAMEMAPDATA_OWNERTYPE,pInfo->nOwnerType);
	m_pData->SetInt(GAMEMAPDATA_OWNERID,pInfo->idOwner);
	m_pData->SetInt(GAMEMAPDATA_DISABLE,pInfo->nDisable);
	OBJID idMap = m_pData->InsertRecord();
	CHECKF(idMap != ID_NONE);
//*
	LOGMSG("加载地图文件[%d]...", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
	if(m_pData->GetInt(GAMEMAPDATA_TYPE)==2)//定义地图表Type=2是专线
	{
		//zeng添加，查询地图模板是否存在，如果存在，直接使用 不用创建
		m_pMapData = MapManager()->GetMapDataTemplate(m_pData->GetInt(GAMEMAPDATA_MAPDOC));
		if(!m_pMapData)
		{
			m_pMapData	= IMapData::CreateNew(m_pData->GetInt(GAMEMAPDATA_MAPDOC), MAPDATA_VERSION);
			if(m_pMapData)
				MapManager()->AddMapData(m_pData->GetInt(GAMEMAPDATA_MAPDOC),m_pMapData);
		}
		
		if(!m_pMapData)
		{
			LOGERROR("特殊地图[%d]加载失败！", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
			return false;
		}
		
		// 装载地图
		m_setBlock.resize(GetWidthOfBlock());
		for(int i = 0; i < m_setBlock.size(); i++)
		{
			m_setBlock[i].resize(GetHeightOfBlock());
			for(int j = 0; j < m_setBlock[i].size(); j++)
				m_setBlock[i][j].Create();
		}	
		
		return idMap;
	}
	return ID_NONE;
}
//////////////////////////////////////////////////////////////////////////
OBJID CGameMap::CreateSpecilMap(PROCESS_ID idProcess,IRecordset* pRes)
{
	
	m_idProcess = idProcess;
	
	m_pData	= CGameMapData::CreateNew();
	if(!m_pData || !m_pData->Create(pRes))
		return 0;
//*
	LOGMSG("加载地图文件[%d]...", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
	if(m_pData->GetInt(GAMEMAPDATA_TYPE)==2)//定义地图表Type=2是专线
	{
		//zeng添加，查询地图模板是否存在，如果存在，直接使用 不用创建
		m_pMapData = MapManager()->GetMapDataTemplate(m_pData->GetInt(GAMEMAPDATA_MAPDOC));
		if(!m_pMapData)
		{
			m_pMapData	= IMapData::CreateNew(m_pData->GetInt(GAMEMAPDATA_MAPDOC), MAPDATA_VERSION);
			if(m_pMapData)
				MapManager()->AddMapData(m_pData->GetInt(GAMEMAPDATA_MAPDOC),m_pMapData);
		}
		
		if(!m_pMapData)
		{
			LOGERROR("特殊地图[%d]加载失败！", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
			return 0;
		}
		
		// 装载地图
		m_setBlock.resize(GetWidthOfBlock());
		for(int i = 0; i < m_setBlock.size(); i++)
		{
			m_setBlock[i].resize(GetHeightOfBlock());
			for(int j = 0; j < m_setBlock[i].size(); j++)
				m_setBlock[i][j].Create();
		}	
		
		return m_pData->GetID();
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
void CGameMap::OnTimer(DWORD tCurr)
{
	if(IsDynaMap())
		return;
	if(!m_pGenData)
		return;
	DEBUG_TRY
	GeneratorMonster();
	DEBUG_CATCH2("GeneratorMonster,mapid = %u",GetID())

	//普通地图刷怪

	NpcManager()->CreateSynMonster(0,0,0,0,0);
// 	for(int i = 0; i < m_setWeather->GetAmount(); i++)
// 	{
// 		CWeatherRegion* pWeather = m_setWeather->GetObjByIndex(i);
// 		if(pWeather)
// 			pWeather->QueryWeather()->OnTimer();
// 	}
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::QueryObjInPos(int nPosX, int nPosY, OBJID idObjType, void** ppObj)
{
	*ppObj = NULL;
	CHECKF(idObjType != ID_NONE);

	//20070320修罗:返回值改为指针,检查安全性
	//IThingSet* pSet = QueryBlock(nPosX, nPosY).QuerySet();
	CGameBlock* gameBlockTemp = QueryBlock(nPosX, nPosY);
	CHECKF(gameBlockTemp);
	IThingSet* pSet = gameBlockTemp->QuerySet();
	//--------------------------------------
	

	for(int i = 0; i < pSet->GetAmount(); i++)
	{
		IMapThing* pTarget = pSet->GetObjByIndex(i);
		void* pTemp;
		if(pTarget && pTarget->QueryObj(idObjType, &pTemp) && pTarget->GetPosX() == nPosX && pTarget->GetPosY() == nPosY)
		{
			*ppObj = pTemp;
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
//只为摆摊服务
bool CGameMap::CheckBooth( int nPosX,int nPosY,OBJID idObjType,void** ppObj )
{

	*ppObj = NULL;
	int x,y,z;
	FOR_9_BLOCKTHINGS( this,nPosX,nPosY )
	{
		IMapThing* pTarget = this->QueryThingByIndex(x,y,z);
		if(!pTarget)
			continue;
	
		if (abs(pTarget->GetPosX() - nPosX) <= 3
			&& abs(pTarget->GetPosY() - nPosY ) <= 3)
		{
			if( pTarget->QueryObj(idObjType,ppObj) )//摊位
			{
				return true;
			}
		}
	}
	return false;

}
//////////////////////////////////////////////////////////////////////
bool CGameMap::QueryObj(int nCenterX, int nCenterY, OBJID idObjType, OBJID idObj, void** ppObj)
{
	*ppObj = NULL;
	CHECKF(idObjType != ID_NONE);

	void* pTemp = NULL;
	int x,y,z;
//----------------------------------------------------------------------
	FOR_9_BLOCKTHINGS(this, nCenterX, nCenterY)
	{
		IMapThing* pTarget = QueryThingByIndex(x,y,z);
		if(pTarget && pTarget->QueryObj(idObjType, &pTemp))
		{
			if (idObj != ID_NONE)
			{
				if (pTarget->GetID() == idObj)
				{
					*ppObj = pTemp;
					return true;
				}
			}
			else
			{
				*ppObj = pTemp;
				return true;
			}
		}
	}
	return false;

}

//////////////////////////////////////////////////////////////////////
IRole* CGameMap::QueryRole(int nCenterX, int nCenterY, OBJID idObj)
{
	//CHECKF(idObj != ID_NONE);

	IRole*	pRole	= NULL;
	int x,y,z;
	FOR_9_BLOCKTHINGS(this, nCenterX, nCenterY)
	{
		IMapThing* pTarget = QueryThingByIndex(x,y,z);
		if (pTarget && pTarget->QueryRole(IPP_OF(pRole)))
		{
			if (idObj == ID_NONE
					&& pTarget->GetPosX() == nCenterX
					&& pTarget->GetPosY() == nCenterY)
			{
				return pRole;
			}

			if (pTarget->GetID() == idObj)
				return pRole;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
IRole* CGameMap::QueryRoleByPos(int nPosX, int nPosY)
{
	if(!IsValidPoint(nPosX, nPosY))
		return NULL;

	IRole*	pRole	= NULL;

	
	//20070320修罗:返回值改为指针,检查安全性
	//IThingSet* pSet = QueryBlock(nPosX, nPosY).QuerySet();
	CGameBlock* gameBlockTemp = QueryBlock(nPosX, nPosY);
	CHECKF(gameBlockTemp);
	IThingSet* pSet = gameBlockTemp->QuerySet();
	//--------------------------------------


	for(int i = 0; i < pSet->GetAmount(); i++)
	{
		IMapThing* pTarget = pSet->GetObjByIndex(i);
		if (pTarget && pTarget->QueryRole(IPP_OF(pRole))
					&& pTarget->GetPosX() == nPosX && pTarget->GetPosY() == nPosY)
		{
			return pRole;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
void CGameMap::SendBlockInfo(IRole* pRole)
{
	CHECK(pRole);

	// 优化
	void* pTemp = NULL;
	if(pRole->QueryObj(OBJ_USER, IPP_OF(pTemp)))
	{
		int x,y;
		FOR_9_BLOCKS(this, pRole->GetPosX(), pRole->GetPosY())
		{
			IThingSet* pSet = BlockByIndex(x,y).QuerySet();
			for(int i = 0; i < pSet->GetAmount(); i++)
			{
				IMapThing* pMapThing = pSet->GetObjByIndex(i);
				if(pMapThing && !(pMapThing->GetObjType() == OBJ_USER && pMapThing->GetID() == pRole->GetID()) )
				{
					pMapThing->SendShow(pRole);
				}
			}
		}
	}
	else // if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pTemp)))
	{
		CUser* pUser = NULL;
		int x,y;
		FOR_9_BLOCKS(this, pRole->GetPosX(), pRole->GetPosY())
		{
			IThingSet* pSet = BlockByIndex(x,y).QuerySet();
			for(int i = 0; i < pSet->GetAmount(); i++)
			{
				IMapThing* pMapThing = pSet->GetObjByIndex(i);
				if(pMapThing && pMapThing->GetObjType() == OBJ_USER)		//@ 只发USER的信息给NPC服务器
				{
					pMapThing->SendShow(pRole);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CGameMap::BroadcastBlockMsg(IMapThing* pThing, CNetMsg* pMsg, bool bSendSelf /*= false*/)
{
	ASSERT(pMsg);

	//? 这个函数的逻辑有点乱：(
	void*	pObj	= NULL;
	IRole*	pRole	= NULL;
	CMonster* pMonster = NULL;
//@不发非玩家消息到NPC服务器	bool	bSendToNpc = (!pThing->QueryObj(OBJ_MONSTER, IPP_OF(pObj)) || bSendSelf);
	bool	bSendToNpc = (pThing->QueryObj(OBJ_USER, IPP_OF(pObj)) 
							|| (pThing->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && (pMonster->IsCallPet() || pMonster->IsEudemon()))
							|| bSendSelf);

	int x,y,z;
	FOR_9_BLOCKTHINGS(this, pThing->GetPosX(), pThing->GetPosY())
	{
		DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVV
		IMapThing* pTarget = QueryThingByIndex(x,y,z);
		if(pTarget && pTarget->QueryRole(IPP_OF(pRole)) && pRole->GetDistance(pThing) <= CELLS_PER_VIEW)
		{
			if(bSendSelf || !(pTarget->GetObjType() == pThing->GetObjType() && pTarget->GetID() == pThing->GetID()))
			{
				// monster
				if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && bSendToNpc)
				{
					if (pMonster->IsCallPet() || pMonster->IsEudemon())
					{
					}
					else
					{
						if(bSendToNpc)
						{
							pRole->SendMsg(pMsg);
							bSendToNpc = false;
						}
					}
				}
				
				// user
				CUser* pUser;
				if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
				{
					pRole->SendMsg(pMsg);
				}				
			}
		}
		DEBUG_CATCH("BroadcastBlockMsg")		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
}

//////////////////////////////////////////////////////////////////////
void CGameMap::BroadcastBlockMsg(int nPosX, int nPosY, CNetMsg* pMsg)
{
	ASSERT(pMsg);

//	bool bSendNpcAlready = false;
	void*	pObj	= NULL;
	IRole*	pRole	= NULL;

	int x,y,z;
	FOR_9_BLOCKTHINGS(this, nPosX, nPosY)
	{
		DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVV
		IMapThing* pTarget = QueryThingByIndex(x,y,z);
		if(pTarget && pTarget->QueryRole(IPP_OF(pRole)) && pRole->GetDistance(nPosX, nPosY) <= CELLS_PER_VIEW)
		{
			// monster
			CMonster* pMonster = NULL;			
			if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) /*&& !bSendNpcAlready*/)
			{
				if (pMonster->IsCallPet() || pMonster->IsEudemon())
				{
				}
				else
				{
					pRole->SendMsg(pMsg);
//					bSendNpcAlready = true;
				}
			}
			
			// user
			CUser* pUser;
			if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
			{
// 				if(!bSendNpcAlready)
// 				{
					// agent
					pRole->SendMsg(pMsg);
// 					bSendNpcAlready = true;
// 				}
			}
		}
		DEBUG_CATCH("BroadcastBlockMsg")		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	}
}

//////////////////////////////////////////////////////////////////////
// role
//////////////////////////////////////////////////////////////////////
void CGameMap::EnterRoom(IMapThing* pThing, BOOL bWithBlock/*=false*/)
{
	CHECK(pThing);

	
	
	//20070320修罗:返回值改为指针,检查安全性
	//QueryBlock(pThing->GetPosX(), pThing->GetPosY()).QuerySet()->AddObj(pThing);
	CGameBlock* gameBlockTemp = QueryBlock(pThing->GetPosX(), pThing->GetPosY());
	CHECK(gameBlockTemp);
	gameBlockTemp->QuerySet()->AddObj(pThing);
	//--------------------------------------


	if(bWithBlock)
	{
		if(!m_pMapData->IncRole(pThing->GetPosX(), pThing->GetPosY()))
		{
			::LogSave("m_pMapData->IncRole %u %u %u %u",pThing->GetID(),GetID(),pThing->GetPosX(),pThing->GetPosY());
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CGameMap::LeaveRoom(IMapThing* pThing, BOOL bWithBlock/*=false*/)
{
	CHECK(pThing);

	
	//20070320修罗:返回值改为指针,检查安全性
	//QueryBlock(pThing->GetPosX(), pThing->GetPosY()).QuerySet()->DelObj(pThing->GetID(), pThing->GetObjType());
	CGameBlock* gameBlockTemp = QueryBlock(pThing->GetPosX(), pThing->GetPosY());
	CHECK(gameBlockTemp);
	if(!(gameBlockTemp->QuerySet()->DelObj(pThing->GetID(), pThing->GetObjType())))
	{
		LogSave("CGameMap::LeaveRoom(IMapThing* pThing, BOOL bWithBlock/*=false*/) %u %u %u %u",pThing->GetID(),GetID(),pThing->GetPosX(),pThing->GetPosY());
	}
	//--------------------------------------


	if(bWithBlock)
		m_pMapData->DecRole(pThing->GetPosX(), pThing->GetPosY());
}

//////////////////////////////////////////////////////////////////////
void CGameMap::MoveTo(IRole* pRole, int nNewPosX, int nNewPosY, BOOL bLeaveBlock/*=false*/, BOOL bEnterBlock/*=false*/)		// 移动到相邻的BLOCK
{
	CHECK(pRole);

	if(bLeaveBlock)
	{
		if(!m_pMapData->DecRole(pRole->GetPosX(), pRole->GetPosY()))
		{
			::LogSave("m_pMapData->IncRole %u %u %u %u",pRole->GetID(),GetID(),pRole->GetPosX(),pRole->GetPosY());
		}
	}
	if(bEnterBlock)
	{
		if(!m_pMapData->IncRole(nNewPosX, nNewPosY))
		{
			::LogSave("m_pMapData->IncRole %u %u %u %u",pRole->GetID(),GetID(),pRole->GetPosX(),pRole->GetPosY());
		}
	}

	if(Block(pRole->GetPosX()) == Block(nNewPosX) && Block(pRole->GetPosY()) == Block(nNewPosY))
		return;

	IMapThing* pThing = pRole->QueryMapThing();
	CHECK(pThing);


	
	//20070320修罗:返回值改为指针,检查安全性
	//QueryBlock(pThing->GetPosX(), pThing->GetPosY()).QuerySet()->DelObj(pThing->GetID(), pThing->GetObjType());
	CGameBlock* gameBlockTemp = QueryBlock(pThing->GetPosX(), pThing->GetPosY());
	CHECK(gameBlockTemp);
	if(!(gameBlockTemp->QuerySet()->DelObj(pThing->GetID(), pThing->GetObjType())))
	{
		LogSave("CGameMap::MoveTo(IRole* pRole, int nNewPosX, int nNewPosY %u %u %u %u %u",pThing->GetID(),pThing->GetObjType(),GetID(),nNewPosX,nNewPosY);
	}
	//--------------------------------------

	//20070320修罗:返回值改为指针,检查安全性
	//QueryBlock(nNewPosX, nNewPosY).QuerySet()->AddObj(pThing);
	gameBlockTemp = QueryBlock(nNewPosX, nNewPosY);
	CHECK(gameBlockTemp);
	gameBlockTemp->QuerySet()->AddObj(pThing);
	//--------------------------------------
}


bool CGameMap::ChangeUserMapData(CUser* pUser, int nNewPosX, int nNewPosY)
{
	if(!IsValidPoint(nNewPosX, nNewPosY))
		return false;

	POINT point;
	int nX[9]={0,1,-1,1,0,1,-1,-1,0};
	int nY[9]={0,1,-1,0,1,-1,1,0,-1};

	DWORD dwAction = ID_NONE;
	DWORD dwActionUser = pUser->GetPasswayAction();
	for (int i=0; i<9; i++)
	{
		point.x = nNewPosX + nX[i];
		point.y = nNewPosY + nY[i];
		dwAction = GetPassageAction(point);
		if(dwAction != 0)
		{
			if(dwActionUser != dwAction)
			{
				GameAction()->ProcessAction(dwAction, pUser, NULL);
				pUser->SetPasswayAction(dwAction);
				return true;
			}
			break;
		}
	}
	
	if(dwAction == ID_NONE && dwActionUser != ID_NONE)
		pUser->SetPasswayAction(ID_NONE);

	return false;
}

//////////////////////////////////////////////////////////////////////
// const
//////////////////////////////////////////////////////////////////////
bool CGameMap::IsStandEnable(int nPosX, int nPosY)
{
	if(!IsValidPoint(nPosX, nPosY))
		return false;
	return (m_pMapData->GetFloorMask(nPosX, nPosY) != 1);
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsMoveEnable(int x, int y)
{
	if(!IsValidPoint(x, y))
		return false;
	return (m_pMapData->GetFloorMask(x, y) != 1);
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::IsMoveEnable(int xSour, int ySour, int x, int y, int nClimbCap)
{
	if(!IsValidPoint(x, y))
		return false;
	if(m_pMapData->GetFloorMask(x, y) == 1)
		return false;
	if(m_pMapData->GetFloorMask(xSour, ySour) == 1)
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
void CGameMap::CollectMapThing(MAPTHING_SET& setMapThing, const POINT pos, int nRange, OBJID idObjTypeUnion)
{
//	CHECK(psetMapThing);

	int	nSize		= nRange*2 + 1;
	int	nBufSize	= nSize * nSize;
	setMapThing.resize(nBufSize);

	int x,y,z;
	FOR_9_BLOCKTHINGS(this, pos.x, pos.y)
	{
		IMapThing* pThing = QueryThingByIndex(x,y,z);
		if(pThing && ::IsObjType(pThing->GetObjType(), idObjTypeUnion)
				&& Distance(pThing->GetPosX(), pThing->GetPosY(), pos.x, pos.y) <= nRange)
		{
			int idx = POS2INDEX(pThing->GetPosX() - (pos.x - nRange), pThing->GetPosY() - (pos.y - nRange), nSize, nSize);
			IF_OK(idx >= 0 && idx < nBufSize)
			{
				//20080112修罗:从2维变为3维------
				//setMapThing[idx]	= pThing;
				setMapThing[idx].push_back(pThing);
				//-------------------------------
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::FindDropItemCell(int nRange, POINT* pPos,MAP_OJBTYPE motType)		// pos: in/out
{
	int	nSize		= nRange*2 + 1;
	int	nBufSize	= nSize * nSize;
	MAPTHING_SET	setItem;

	CollectMapThing(setItem, *pPos, nRange, motType);

	// 先随机
	int		nIndex = ::RandGet(nBufSize);
	//20080112修罗:从2维变为3维------
	//if(!setItem[nIndex])
	if(!(setItem[nIndex].size()))
	//-------------------------------
	{
		POINT	posTest;
		int		nLeft	= pPos->x - nRange;
		int		nTop	= pPos->y - nRange;
		posTest.x	= nLeft + INDEX2X(nIndex, nSize, nSize);
		posTest.y	= nTop + INDEX2Y(nIndex, nSize, nSize);
		if(/*IsLayItemEnable(posTest.x, posTest.y)*/true)
		{
			*pPos = posTest;
			return true;
		}
	}

	// 顺序找
	int		nMinRange = nRange + 1;
	int		ret	= false;
	POINT	posFree;
	for(int i = __max(pPos->x-nRange, 0); i <= pPos->x+nRange && i < GetWidth(); i++)
	{
		for(int j = __max(pPos->y-nRange, 0); j <= pPos->y+nRange && j < GetHeight(); j++)
		{
			int idx = POS2INDEX(i - (pPos->x - nRange), j - (pPos->y - nRange), nSize, nSize);
			IF_OK(idx >= 0 && idx < nBufSize)
				//if(setItem[idx])
				if(setItem[idx].size())//20080112修罗:从2维变为3维
					continue;

			if(/*IsLayItemEnable(i, j)*/true)
			{
				int	nDistance = Distance(i, j, pPos->x, pPos->y);
				if(nDistance < nMinRange)
				{
					// 找到了!
					nMinRange = nDistance;
					posFree.x	= i;
					posFree.y	= j;
					ret = true;
				}
			}
		}
	}

	if(ret)
	{
		*pPos = posFree;
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
DWORD CGameMap::GetPassageAction(const POINT& pos)
{
	CHECKF(pos.x >= 0 && pos.x < GetWidth() && pos.y >=0 && pos.y < GetHeight());

	int	idxPassage = m_pMapData->GetPassage(pos.x, pos.y);
	if(idxPassage == PASSAGE_NONE)
		return ID_NONE;

	return MapManager()->FindPassway(GetDocID(), idxPassage);
}
//////////////////////////////////////////////////////////////////////
bool CGameMap::GetRebornMap(OBJID* pidMap, POINT* pposTarget)
{
	DEBUG_TRY
		LOGERROR("CGameMap::GetRebornMap 这里不应该进来,检查调用的地方!");
		CHECKF(pidMap && pposTarget);
		*pidMap=1001;
		pposTarget->x=78;
		pposTarget->y=87;
	DEBUG_CATCH("CGameMap::GetRebornMap")

	return true;

	//重生地点处理 satan******************************
	OBJID	idTargetMap	= 1001/*m_pData->GetInt(GAMEMAPDATA_REBORN_MAPID)*/;
	int		idxPortal	= 1/*m_pData->GetInt(GAMEMAPDATA_REBORN_PORTAL)*/;
	CHECKF(idTargetMap != ID_NONE);

	/*
	if(!MapManager()->FindPortal(&posNew, idTargetMap, idxPortal))
	{
		LOGWARNING("没有找到地图入口点[%u][%d], 使用缺省入口点.", idTargetMap, idxPortal);
	}
	*/
	
	CGameMapData* pTargetMap = MapManager()->QuerySystemMapSet()->GetObj(idTargetMap);
	IF_NOT(pTargetMap)
	{
		LOGERROR("没有找到切屏地图[%u] !", idTargetMap);
		return false;
	}
	idTargetMap=1001;
	POINT	posNew={78,87};
// 	posNew.x	= pTargetMap->GetInt(GAMEMAPDATA(GAMEMAPDATA_PORTAL0_X));
// 	posNew.y	= pTargetMap->GetInt(GAMEMAPDATA(GAMEMAPDATA_PORTAL0_Y));

// 	IF_NOT (posNew.x != 0 || posNew.y != 0)
// 		return false;

	*pidMap	= idTargetMap;
	*pposTarget	= posNew;

	return true;
}


//////////////////////////////////////////////////////////////////////
OBJID CGameMap::CreateDynaMap(PROCESS_ID idProcess, const NewMapInfo* pInfo)
{
	m_idProcess		= idProcess;
	CHECKF(pInfo);

	m_pData	= CGameMapData::CreateNew();
	if(!m_pData || !m_pData->Create(GameDataDefault()->GetDynaMap(), ID_NONE))
		return false;
	m_pData->SetStr(GAMEMAPDATA_NAME, pInfo->szName, _MAX_NAMESIZE);
	m_pData->SetInt(GAMEMAPDATA_OWNERTYPE,		pInfo->nOwnerType);
	m_pData->SetInt(GAMEMAPDATA_OWNERID,		pInfo->idOwner);
	m_pData->SetInt(GAMEMAPDATA_MAPDOC,			pInfo->nMapDoc);
	m_pData->SetInt(GAMEMAPDATA_TYPE,			pInfo->nType);
	m_pData->SetInt(GAMEMAPDATA_MAPGROUP,		pInfo->nMapGroup);
	OBJID idMap = m_pData->InsertRecord();
	CHECKF(idMap != ID_NONE);

//*
//	LOGMSG("加载动态地图文件[%d]...", m_pData->GetInt(GAMEMAPDATA_MAPDOC));

	//zeng,查询地图模板
	m_pMapData = MapManager()->GetMapDataTemplate(m_pData->GetInt(GAMEMAPDATA_MAPDOC));
	
	if(!m_pMapData)
	{
		m_pMapData	= IMapData::CreateNew(m_pData->GetInt(GAMEMAPDATA_MAPDOC), MAPDATA_VERSION);
		if(m_pMapData)
			MapManager()->AddMapData(m_pData->GetInt(GAMEMAPDATA_MAPDOC),m_pMapData);
	}
	if(!m_pMapData)
	{
		LOGERROR("动态地图文件[%d]加载失败！", m_pData->GetInt(GAMEMAPDATA_MAPDOC));
		return ID_NONE;
	}

	// 装载地图
	m_setBlock.resize(GetWidthOfBlock());
	for(int i = 0; i < m_setBlock.size(); i++)
	{
		m_setBlock[i].resize(GetHeightOfBlock());
		for(int j = 0; j < m_setBlock[i].size(); j++)
			m_setBlock[i][j].Create();
	}
//*/

	return idMap;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::EraseMap()
{
	CHECKF(IsDynaMap());

	CRoleSet::Iterator	pRole = RoleManager()->QuerySet()->NewEnum();
	while(pRole.Next())
	{
		if(pRole && pRole->GetMap() && pRole->GetMap()->GetID() == GetID())			//zeng必须判断角色的地图是否为NULL
		{
			CUser* pUser;
//			CNpc* pNpc;
			if(pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
				return false;
// 			else if(pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsLinkNpc())
// 				return false;
		}
	}

	pRole.Reset();
	while(pRole.Next())
	{
		DEBUG_TRY	// VVVVVVVVVVVVVV
		if(pRole && pRole->GetMap()&& pRole->GetMap()->GetID() == GetID())		//zeng必须判断角色的地图是否为NULL
		{
			RoleManager()->QuerySet()->DelObj(pRole->GetID());
		}
		DEBUG_CATCH("EraseMap()")	// AAAAAAAAAAAAAAA
	}

	//??? 未处理Booth，估计不会在动态地图中摆摊

	CMapItemSet::Iterator pMapItem = MapManager()->QueryMapItemSet()->NewEnum();
	while(pMapItem.Next())
	{
		if(pMapItem && pMapItem->GetMap()->GetID() == GetID())					//zeng物品的地图不会为NULL，所以不需要判断地图是否为NULL
		{
			MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());
		}
	}

//	SQLBUF	szSQL;
//	sprintf(szSQL, "UPDATE %s SET del_flag=1 WHERE id=%u LIMIT 1", _TBL_DYNAMAP, GetID());
//	Database()->ExecuteSQL(szSQL);

	MapManager()->QuerySet()->DelObj(GetID());

	return true;
}

bool CGameMap::EraseInstanceMap()
{
	CHECKF(IsDynaMap());
	
	CRoleSet::Iterator	pRole = RoleManager()->QuerySet()->NewEnum();
	
	
// 	while(pRole.Next())
// 	{
// 		if(pRole && pRole->GetMap() && pRole->GetMap()->GetID() == GetID())			//zeng必须判断角色的地图是否为NULL
// 		{
// 			CUser* pUser;
// 			CNpc* pNpc;
// 			if(pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
// 			{
// 				//UserManager()->KickOutSocket(pUser->GetSocketID());
// 				::LOGERROR("玩家没有删除完全，副本回收失败");
// 				return false;
// 			}
// 			//return false;
// 			// 			else if(pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) /*&& pNpc->IsLinkNpc()*/)
// 			// 				return false;
// 		}
// 	}
// 	
// 	pRole.Reset();
	

	while(pRole.Next())
	{
		DEBUG_TRY	// VVVVVVVVVVVVVV
		if(pRole && pRole->GetMap()&& pRole->GetMap()->GetID() == GetID())		//zeng必须判断角色的地图是否为NULL
		{
			if(pRole->IsUser())
				LogSave("EraseMap() error has user!!   userid = %u",pRole->GetID());
			RoleManager()->QuerySet()->DelObj(pRole->GetID());
		}
		DEBUG_CATCH("EraseMap()")	// AAAAAAAAAAAAAAA
	}
	
	//??? 未处理Booth，估计不会在动态地图中摆摊
	
	CMapItemSet::Iterator pMapItem = MapManager()->QueryMapItemSet()->NewEnum();
	while(pMapItem.Next())
	{
		if(pMapItem && pMapItem->GetMap()->GetID() == GetID())					//zeng物品的地图不会为NULL，所以不需要判断地图是否为NULL
		{
//			LogSave("EraseMap() error has mapitem!!   mapitemid = %u",pMapItem->GetID());
			MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());
		}
	}
	
	//	SQLBUF	szSQL;
	//	sprintf(szSQL, "UPDATE %s SET del_flag=1 WHERE id=%u LIMIT 1", _TBL_DYNAMAP, GetID());
	//	Database()->ExecuteSQL(szSQL);
	
	MapManager()->QuerySet()->DelObj(GetID());
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CGameMap::ChangeMapDoc(OBJID idDoc)						// false: user in map, can't change
{
/*	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while(pUser.Next())
	{
		if(pUser->GetMapID() == GetID())
			return false;
	}

	IMapData* pMapData	= IMapData::CreateNew(idDoc, MAPDATA_VERSION);	// VVVVVVVVVVVVVVVVVVVVVVVVVV
	if(!pMapData)
	{
		LOGERROR("地图文件[%d]加载失败!", idDoc);
		return false;
	}
	CHECKF(pMapData->GetMapWidth() == m_pMapData->GetMapWidth());
	CHECKF(pMapData->GetMapHeight() == m_pMapData->GetMapHeight());
*/
	int nOldDoc = m_pData->GetInt(GAMEMAPDATA_MAPDOC);
	m_pData->SetInt(GAMEMAPDATA_MAPDOC, idDoc);
	m_pData->Update();
	m_pData->SetInt(GAMEMAPDATA_MAPDOC, nOldDoc);
	m_pData->ClearUpdateFlags();
/*
	IF_OK(m_pMapData)
		m_pMapData->Release();
	m_pMapData	= pMapData;		// AAAAAAAAAAAAAAAAA

	// synchro ai server
	CMsgAction	msg;
	IF_OK(msg.Create(GetID(), 0, 0, 0, actionChangeMapDoc, idDoc))
		MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
*/
	return true;
}

//得到地图的随机位置
bool CGameMap::GetRandomPos(int &nX,int &nY)
{
	int nWidth = this->GetWidth();
	int nHeight = this->GetHeight();

	int nNum=0;

	while(nNum<20)
	{
		nX = ::RandGet(nWidth);
		nY = ::RandGet(nHeight);
		if(this->IsStandEnable(nX,nY))
			return true;
		else
		{
			nNum++;
			continue;
		}
	}

	return false;
}
//-----------------------------------
BOOL CGameMap::IsMarketRegion(int x,int y)//20070123修罗:市场用区域实现
{
	//摆摊处理 satan******************************
// 	for( int i = 0; i < m_setRegion->GetAmount(); i++)
// 	{
// 		CRegionData*	pData = m_setRegion->GetObjByIndex(i);
// 		if(pData && pData->GetInt(REGIONDATA_TYPE) == REGION_MARKET)
// 		{
// 			int regionX = pData->GetInt(REGIONDATA_BOUND_X);
// 			int regionY = pData->GetInt(REGIONDATA_BOUND_Y);
// 			int regionCX = pData->GetInt(REGIONDATA_BOUND_CX);
// 			int regionCY = pData->GetInt(REGIONDATA_BOUND_CY);
// 
// 			if(x >= regionX && 
// 				y >= regionY && 
// 				x <= regionX + regionCX && 
// 				y <= regionY + regionCY)
// 			return true;
// 		}
// 	}
	return false;
}
//----------------------------------------
CGameBlock*	CGameMap::QueryBlock(int nPosX, int nPosY)
{ 
	int blockX = Block(nPosX);
	int blockY = Block(nPosY);
	//检查X,Y越界
	CHECKF(m_setBlock.size()>blockX
		&& m_setBlock[blockX].size()>blockY);

	return &(m_setBlock[blockX][blockY]); 
}
