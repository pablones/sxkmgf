// UserList.cpp: implementation of the CMapList class.
//
//////////////////////////////////////////////////////////////////////
#include "MessagePort.h"
#include "common.h"
#include "NetMsg.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "protocol.h"
using namespace world_kernel;
#include "MapList.h"
#include "WorldKernel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapList::CMapList()
{

}

CMapList::~CMapList()
{
	for(int i = 0; i < m_setMap.size(); i++)
	{
		delete m_setMap[i];
	}
}

//////////////////////////////////////////////////////////////////////
bool CMapList::Create(IDatabase* pDb)
{
	CHECKF(pDb);

	char szSQL[1024];
	sprintf(szSQL, "SELECT id,mapgroup FROM %s LIMIT 1234567890", _TBL_MAP);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
	CHECKF(pRes);
	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		CMapSt*	pMap = new CMapSt;
		if(pMap)
		{
			pMap->idMap		= pRes->LoadDWord("id");
			pMap->idxMapGroup	= pRes->LoadDWord("mapgroup");
			m_setMap.push_back(pMap);
		}
		pRes->MoveNext();
	}
	pRes->Release();

	return true;
}

//////////////////////////////////////////////////////////////////////
PROCESS_ID CMapList::GetMapProcessID(OBJID idMap)
{
	for(int i = 0; i < m_setMap.size(); i++)
	{
		if(m_setMap[i])
		{
			if(m_setMap[i]->idMap == idMap)
				return m_setMap[i]->idxMapGroup + MSGPORT_MAPGROUP_FIRST;
		}
	}
	//goto 新玩家地图分配处理
	if (idMap==ID_NONE)
		return MSGPORT_MAPGROUP_FIRST;
	return MSGPORT_MAPGROUP_INSTANCE;
}

//////////////////////////////////////////////////////////////////////
CMapSt* CMapList::GetMap(OBJID idMap)
{
	for(int i = 0; i < m_setMap.size(); i++)
	{
		if(m_setMap[i] && m_setMap[i]->idMap == idMap)
			return m_setMap[i];
	}

	return NULL;
}










