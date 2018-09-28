// MapManager.cpp: implementation of the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#include "GameObj.h"
#include "MapManager.h"
#include "MapGroup.h"
#include "MapItem.h"
#include "Npc.h"
//#include "DynaGameMap.h"

MYHEAP_IMPLEMENTATION(CMapManager,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMapManager::CMapManager(PROCESS_ID idProcess) : m_idProcess(idProcess)
{
	m_pMapSet		= CMapSet::CreateNew(true);
	m_setPassway	= CPasswaySet::CreateNew(true);

	// mapitem
	m_pMapItemSet	= CMapItemSet::CreateNew(true);
	m_pBoothSet		= CBoothSet::CreateNew(true);
	m_idLastMapItem	= MAPITEM_FIRSTID - 1;
	m_tMapItem.SetInterval(MAPITEM_ONTIMER_SECS);
	m_tMapItem.Update();

	// spawn
	m_idLastPet		= (PETID_FIRST-1) + MapGroup(PID)->GetID();					//? ��֤ÿ���������PET_ID����ͻ��
	m_idLastDynaNpc		= (DYNANPCID_FIRST-1) + MapGroup(PID)->GetID();					//? ��֤ÿ���������UNMOVENPC_ID����ͻ��
	m_idLastTrap		= (MAGICTRAPID_FIRST-1) + MapGroup(PID)->GetID();					//? ��֤ÿ���������UNMOVENPC_ID����ͻ��
	m_idLastCallPet		= (CALLPETID_FIRST-1) + MapGroup(PID)->GetID();					//? ��֤ÿ���������CALLPET_ID����ͻ��
	m_idLastVItem		= (VITEM_ID_FIRST-1) + MapGroup(PID)->GetID();	
}

//////////////////////////////////////////////////////////////////////
CMapManager::~CMapManager()
{
	if(m_pMapItemSet)
		m_pMapItemSet->Release();
	if(m_pBoothSet)
		m_pBoothSet->Release();
	if(m_setPassway)
		m_setPassway->Release();
	if(m_pMapSet)
		m_pMapSet->Release();

	//zeng���ͷŵ�ͼģ����Դ
	ReleaseMapData();
}

//////////////////////////////////////////////////////////////////////
void	CMapManager::Release()
{
	delete this;
}

//////////////////////////////////////////////////////////////////////
void	CMapManager::OnTimer(time_t tCurr)
{
	// game map
	for(int i = 0; i < m_pMapSet->GetAmount(); i++)
	{
		CGameMap* pMap = m_pMapSet->GetObjByIndex(i);
		if(pMap)
			pMap->OnTimer(tCurr);
	}

	// map item
	if(m_tMapItem.ToNextTime())
	{
		int nCount = 0;
		for(int i = QueryMapItemSet()->GetAmount()-1; i >= 0; i--)		// ɾ��������
		{
			CMapItem* pMapItem = QueryMapItemSet()->GetObjByIndex(i);
			if(pMapItem)
			{
				if(!pMapItem->IsAlive())
				{
					//pMapItem->ReleaseByOwner();
					QueryMapItemSet()->DelObj(pMapItem->GetID());	// �ͻ�����Ϣͬ����CMapItem��������ʵ��
				}
			}
			DEAD_LOOP_BREAK(nCount, 5000);
		}
	}
}

//////////////////////////////////////////////////////////////////////
bool CMapManager::Create()
{
	CHECKF(m_pMapSet);
	CHECKF(m_setPassway);
	CHECKF(m_pMapItemSet);
	CHECKF(m_pBoothSet);

	// map set
	SQLBUF	szSQL;
#ifdef _DEBUGx
	sprintf(szSQL, "SELECT * FROM %s WHERE  mapgroup=%u and type = 0 ORDER BY id limit 10", _TBL_MAP, GetServerID(), MapGroup(PID)->GetMapGroup());
#else
	sprintf(szSQL, "SELECT * FROM %s WHERE mapgroup=%u and  type = 0 ORDER BY id", _TBL_MAP, MapGroup(PID)->GetMapGroup());
#endif
	::CAutoPtr<IRecordset> pRes = Database()->CreateNewRecordset(szSQL, false);		// false: ����ÿ�������鶼�е�ͼ
	if(!pRes)
		return false;
	int nCount = 0;
	for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
	{
		CMapPtr pMap = CGameMap::CreateNew();
		if(!pMap)
		{
// 			pRes->Release();
			return false;
		}
#ifdef	PALED_DEBUG
//------------------07.5.25����ע��-------------------begin---------------
//		if(pRes->GetInt(0) == 1041 || pRes->GetInt(0)>=1060 && pRes->GetInt(0)<=1062)		//? �����ء������Թ���������(
//		{
//			pMap->ReleaseByOwner();
//			continue;	// szSQL[0]=0;//
//		}
//------------------------------------------------------end-----------------------
#endif
		
		if(!pMap->Create(m_idProcess, pRes))
		{
			pMap->ReleaseByOwner();
#ifdef	LOCAL_DEBUG
			MapGroup(PID)->QueryIntraMsg()->PrintText("Can't init a map data!");
			continue;
#endif
//  			pRes->Release();
// 			return false;
			continue;
		}

		m_pMapSet->AddObj(pMap);
		nCount++;
	}
// 	pRes->Release();
	LOGMSG("Load Map %d in mapgroup %d.", nCount, MapGroup(PID)->GetMapGroup());

	sprintf(szSQL, "SELECT * FROM %s", _TBL_PASSWAY);
	IF_NOT_(m_setPassway->Create(szSQL, Database()))
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMapManager::CreateSpecilLineMap()
{
	CHECKF(m_pMapSet);
	CHECKF(m_setPassway);
	CHECKF(m_pMapItemSet);
	CHECKF(m_pBoothSet);
	
	// map set
	SQLBUF	szSQL;
#ifdef _DEBUGx
	sprintf(szSQL, "SELECT * FROM %s WHERE  mapgroup=%u and type = 3 ORDER BY id limit 10", _TBL_MAP, GetServerID(), MapGroup(PID)->GetMapGroup());
#else
	sprintf(szSQL, "SELECT * FROM %s WHERE mapgroup=%u and  type = 3 ORDER BY id", _TBL_MAP, MapGroup(PID)->GetMapGroup());
#endif
	::CAutoPtr<IRecordset> pRes = Database()->CreateNewRecordset(szSQL, false);		// false: ����ÿ�������鶼�е�ͼ
	if(!pRes)
		return false;
	int nCount = 0;
	for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
	{
		CMapPtr pMap = CGameMap::CreateNew();
		if(!pMap)
		{
			// 			pRes->Release();
			return false;
		}
#ifdef	PALED_DEBUG
		//------------------07.5.25����ע��-------------------begin---------------
		//		if(pRes->GetInt(0) == 1041 || pRes->GetInt(0)>=1060 && pRes->GetInt(0)<=1062)		//? �����ء������Թ���������(
		//		{
		//			pMap->ReleaseByOwner();
		//			continue;	// szSQL[0]=0;//
		//		}
		//------------------------------------------------------end-----------------------
#endif
		
		if(!pMap->Create(m_idProcess, pRes))
		{
			pMap->ReleaseByOwner();
#ifdef	LOCAL_DEBUG
			MapGroup(PID)->QueryIntraMsg()->PrintText("Can't init a map data!");
			continue;
#endif
			//  			pRes->Release();
			// 			return false;
			continue;
		}
		
		m_pMapSet->AddObj(pMap);
		nCount++;
	}
	// 	pRes->Release();
	LOGMSG("Load Map %d in mapgroup %d.", nCount, MapGroup(PID)->GetMapGroup());
	
// 	sprintf(szSQL, "SELECT * FROM %s", _TBL_PASSWAY);
// 	IF_NOT_(m_setPassway->Create(szSQL, Database()))
// 		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////
OBJID CMapManager::SpawnMapItemID()
{
	m_idLastMapItem++;

	if(m_idLastMapItem > MAPITEM_LASTID)
	{
		LOGERROR("���� MapItemID �ؾ�MAPITEMϵͳ���ܻ����! ����");
		m_idLastMapItem	= MAPITEM_FIRSTID;
	}

	return m_idLastMapItem;
}

//////////////////////////////////////////////////////////////////////
PROCESS_ID CMapManager::GetProcessID(OBJID idMap)
{
	extern IStaticMapSet* g_setStaticMap;
	for(int i = 0; i < g_setStaticMap->GetAmount(); i++)
	{
		CGameMapData* pData = g_setStaticMap->GetObjByIndex(i);
		if(pData && pData->GetID() == idMap)
			return pData->GetInt(GAMEMAPDATA_MAPGROUP) + MSGPORT_MAPGROUP_FIRST;
	}
	if(idMap >= SYN_MAP_DOWN && idMap < SYN_MAP_UP)
		return MSGPORT_MAPGROUP_FIRST;

	return MSGPORT_MAPGROUP_INSTANCE;
}

CGameMapData* CMapManager::GetMapData(OBJID idMap)
{
	extern IStaticMapSet* g_setStaticMap;
	for(int i = 0; i < g_setStaticMap->GetAmount(); i++)
	{
		CGameMapData* pData = g_setStaticMap->GetObjByIndex(i);
		if(pData && pData->GetID() == idMap)
			return pData;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
DWORD CMapManager::FindPassway(OBJID idMap, int idxPortal)
{
	CHECKF(m_setPassway);
	CHECKF(idMap != ID_NONE);

	for(int i = 0; i < m_setPassway->GetAmount(); i++)
	{
		CPasswayData* pData = m_setPassway->GetObjByIndex(i);
		if(pData && pData->GetInt(PASSWAYDATA_MAPID) == idMap && pData->GetInt(PASSWAYDATA_INDEX) == idxPortal)
		{
			return  pData->GetInt(PASSWAYDATA_PASSWAYACTION);
		}
	}
	return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
// spawn
//////////////////////////////////////////////////////////////////////
OBJID CMapManager::SpawnNewPetID()
{
	int nNextID = m_idLastPet + MapGroup(PID)->GetMapGroupAmount();					//? ��֤ÿ���������PET_ID����ͻ��
	if(nNextID < PETID_LAST)
	{
		m_idLastPet = nNextID;
		return nNextID;
	}
	else if(m_setPetRecycle.size())
	{
		return m_setPetRecycle.pop();
	}
	else
	{
		m_idLastPet = (PETID_FIRST-1) + MapGroup(PID)->GetID();					//? ��֤ÿ���������PET_ID����ͻ��
		LOGERROR("PET��ID�ռ䲻�㣬ǿ�лؾ�! NPC����ϵͳ���ֹ���!");
		return m_idLastPet;
	}
}

//////////////////////////////////////////////////////////////////////
OBJID CMapManager::SpawnNewNpcID()
{
	int nNextID = m_idLastDynaNpc + MapGroup(PID)->GetMapGroupAmount();					//? ��֤ÿ���������PET_ID����ͻ��
	if(nNextID < DYNANPCID_LAST)
	{
		m_idLastDynaNpc = nNextID;
		return nNextID;
	}
	else if(m_setDynaNpcRecycle.size())
	{
		return m_setDynaNpcRecycle.pop();
	}
	else
	{
		m_idLastDynaNpc = (DYNANPCID_FIRST-1) + MapGroup(PID)->GetID();					//? ��֤ÿ���������DYNA_NPC_ID����ͻ��
		LOGERROR("NPC��ID�ռ䲻�㣬ǿ�лؾ�! NPC����ϵͳ���ֹ���!");
		return m_idLastDynaNpc;
	}
}

//////////////////////////////////////////////////////////////////////
OBJID CMapManager::SpawnNewTrapID()
{
	int nNextID = m_idLastTrap + MapGroup(PID)->GetMapGroupAmount();					//? ��֤ÿ���������PET_ID����ͻ��
	if(nNextID < MAGICTRAPID_LAST)
	{
		m_idLastTrap = nNextID;
		return nNextID;
	}
	else if(m_setTrapRecycle.size())
	{
		return m_setTrapRecycle.pop();
	}
	else
	{
		m_idLastTrap = (MAGICTRAPID_FIRST-1) + MapGroup(PID)->GetID();					//? ��֤ÿ���������DYNA_NPC_ID����ͻ��
		LOGERROR("NPC��ID�ռ䲻�㣬ǿ�лؾ�! NPC����ϵͳ���ֹ���!");
		return m_idLastTrap;
	}
}

OBJID	CMapManager::SpawnNewVItemID()
{
	int nNextID = m_idLastVItem + MapGroup(PID)->GetMapGroupAmount();					//? ��֤ÿ���������PET_ID����ͻ��
	if(nNextID < VITEM_ID_LAST)
	{
		m_idLastVItem = nNextID;
		return nNextID;
	}
	else if(m_setVItemRecycle.size())
	{
		return m_setVItemRecycle.pop();
	}
	else
	{
		m_idLastVItem = (VITEM_ID_FIRST-1) + MapGroup(PID)->GetID();					//? ��֤ÿ���������CALLPET_ID����ͻ��
		LOGERROR("VITEM��ID�ռ䲻�㣬ǿ�лؾ�!");
		return m_idLastVItem;
	}
}

//////////////////////////////////////////////////////////////////////
OBJID CMapManager::SpawnNewCallPetID()
{
	int nNextID = m_idLastCallPet + MapGroup(PID)->GetMapGroupAmount();					//? ��֤ÿ���������PET_ID����ͻ��
	if(nNextID < CALLPETID_LAST)
	{
		m_idLastCallPet = nNextID;
		return nNextID;
	}
	else if(m_setCallPetRecycle.size())
	{
		return m_setCallPetRecycle.pop();
	}
	else
	{
		m_idLastCallPet = (CALLPETID_FIRST-1) + MapGroup(PID)->GetID();					//? ��֤ÿ���������CALLPET_ID����ͻ��
		LOGERROR("CALLPET��ID�ռ䲻�㣬ǿ�лؾ�! NPC����ϵͳ���ֹ���!");
		return m_idLastCallPet;
	}
}

//////////////////////////////////////////////////////////////////////
CGameMap* CMapManager::GetGameMap(OBJID idMap, BOOL bLoad/*=true*/)
{
	CGameMap* pMap = m_pMapSet->GetObj(idMap);
	if(pMap)
		return pMap;

	if(!bLoad)
		return NULL;

	if(!IsDynaMap(idMap))
		return NULL;

	return pMap;
}

BOOL CMapManager::DeleteDynaMap(OBJID idMap)
{
	BOOL hr=false;
	CGameMap* pMap=GetGameMap(idMap);
	if (pMap && pMap->IsDynaMap())
	{
		hr=m_pMapSet->DelObj(idMap);
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////
OBJID CMapManager::CreateDynaMap(const NewMapInfo* pInfo)
{
	CGameMap* pMap = CGameMap::CreateNew();
	CHECKF(pMap);

	OBJID	idMap = pMap->CreateDynaMap(m_idProcess, pInfo);
	if(idMap)
	{
		m_pMapSet->AddObj(pMap);
		return idMap;
	}
	else
		pMap->ReleaseByOwner();

	return ID_NONE;
}
BOOL CMapManager::AddSynMap(CGameMap *pMap)
{
	CHECKF(pMap);
	if(pMap->GetID())
	{
		m_pMapSet->AddObj(pMap);
		return true;
	}
	else
		pMap->ReleaseByOwner();
	
	return false;
}
OBJID CMapManager::CreateDynaMap(OBJID idMapType)
{
	NewMapInfo	info;
	ZeroMemory(&info,sizeof(info));
	CGameMapData* pMapData=GetMapData(idMapType);
	CHECKF(pMapData);
	SafeCopy(info.szName, "instance", _MAX_NAMESIZE);
	info.nMapDoc		= pMapData->GetKey();
	info.nType			= pMapData->GetInt(GAMEMAPDATA_TYPE);
	info.nOwnerType		= pMapData->GetInt(GAMEMAPDATA_OWNERTYPE);
	info.idOwner		= pMapData->GetInt(GAMEMAPDATA_OWNERID);
	info.nMapGroup		= pMapData->GetInt(GAMEMAPDATA_MAPGROUP);
	info.nDisable		= pMapData->GetInt(GAMEMAPDATA_DISABLE);
	return CreateDynaMap(&info);
}


IMapData* CMapManager::GetMapDataTemplate(UINT nMapDoc)
{
	map<UINT,IMapData*>::iterator it = m_MapTemplate.find(nMapDoc);
	if(it!=m_MapTemplate.end())
		return it->second;
	
	return NULL;
}

bool CMapManager::AddMapData(UINT nMapDoc,IMapData* pMapData)
{
	map<UINT,IMapData*>::iterator it = m_MapTemplate.find(nMapDoc);
	if(it!=m_MapTemplate.end())
	{
		return false;
	}
	
	m_MapTemplate.insert(make_pair(nMapDoc,pMapData));
	
	return true;
}

void CMapManager::ReleaseMapData()
{
	map<UINT,IMapData*>::iterator it ;
	
	for (it = m_MapTemplate.begin();it!=m_MapTemplate.end();++it)
	{
		IMapData* pMapData = it->second;
		pMapData->Release();
	}
}













