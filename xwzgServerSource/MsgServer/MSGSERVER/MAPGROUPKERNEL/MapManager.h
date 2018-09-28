// MapManager.h: interface for the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPMANAGER_H__4DC38FE0_1341_4D20_8C0A_47517229F518__INCLUDED_)
#define AFX_MAPMANAGER_H__4DC38FE0_1341_4D20_8C0A_47517229F518__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameMap.h"
#include "MapItem.h"
#include "Booth.h"
#include "T_SingleObjSet2.h"
#include "T_MyQueue.h"
#include <vector>
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////
const OBJID	MAPITEM_FIRSTID			= 1;
const OBJID	MAPITEM_LASTID			= 4012345678;

//////////////////////////////////////////////////////////////////////
typedef	IGameObjSet<CGameMap>	IMapSet;
typedef	CGameObjSet<CGameMap>	CMapSet;
typedef	IGameObjSet<CMapItem>	IMapItemSet;
typedef	CGameObjSet<CMapItem>	CMapItemSet;
typedef	ISingleObjSet2<CGameMapData>	IStaticMapSet;
typedef	CSingleObjSet2<CGameMapData>	CStaticMapSet;
typedef	IGameObjSet<CBooth>	IBoothSet;
typedef	CGameObjSet<CBooth>	CBoothSet;

//////////////////////////////////////////////////////////////////////
enum PASSWAYDATA{
	PASSWAYDATA_MAPID=1,
	PASSWAYDATA_INDEX,
	PASSWAYDATA_PASSWAYACTION,
};
char	szPasswayTable[];
typedef	CGameData<PASSWAYDATA, szPasswayTable, szID>	CPasswayData;
typedef	ISingleObjSet2<CPasswayData>		IPasswaySet;
typedef	CSingleObjSet2<CPasswayData>		CPasswaySet;

//////////////////////////////////////////////////////////////////////
class CMapItem;
class IMapManager
{
public:
	virtual ~IMapManager() {}
	virtual void		Release()							PURE_VIRTUAL_FUNCTION
	virtual CGameMap*	QueryMap(OBJID idMap, BOOL bLoad=false)				PURE_VIRTUAL_FUNCTION_0
	virtual IMapSet*	QuerySet()							PURE_VIRTUAL_FUNCTION_0
	virtual CGameMap*	GetGameMap(OBJID idMap, BOOL bLoad=false)				PURE_VIRTUAL_FUNCTION_0
	virtual int			GetGameMapAmount()					PURE_VIRTUAL_FUNCTION_0
	virtual CGameMap*	GetGameMapByIndex(int idx)			PURE_VIRTUAL_FUNCTION_0
	virtual PROCESS_ID	GetProcessID(OBJID idMap)			PURE_VIRTUAL_FUNCTION_0
	virtual	CGameMapData* GetMapData(OBJID idMap)			PURE_VIRTUAL_FUNCTION_0
	virtual void		OnTimer(time_t tCurr)				PURE_VIRTUAL_FUNCTION
	virtual CMapItem*	QueryMapItem(OBJID idMapItem)		PURE_VIRTUAL_FUNCTION_0
	virtual CMapItemSet*	QueryMapItemSet()				PURE_VIRTUAL_FUNCTION_0
	virtual CBooth*	QueryBooth(OBJID idBooth)			PURE_VIRTUAL_FUNCTION_0
	virtual CBoothSet*	QueryBoothSet()					PURE_VIRTUAL_FUNCTION_0
	virtual OBJID		SpawnMapItemID()					PURE_VIRTUAL_FUNCTION_0
	virtual bool		FindPortal(POINT* pPos, OBJID idMap, int idxPortal)		PURE_VIRTUAL_FUNCTION_0
	virtual DWORD		FindPassway(OBJID idMap, int idxPortal)		PURE_VIRTUAL_FUNCTION_0
	virtual OBJID		CreateDynaMap(const NewMapInfo* pInfo)	PURE_VIRTUAL_FUNCTION_0
	virtual OBJID		CreateDynaMap(OBJID idMapType) PURE_VIRTUAL_FUNCTION_0
	virtual	BOOL		DeleteDynaMap(OBJID idMap)			PURE_VIRTUAL_FUNCTION_0
	virtual BOOL		AddSynMap(CGameMap *pMap)			PURE_VIRTUAL_FUNCTION_0
	virtual IStaticMapSet*	QuerySystemMapSet()				PURE_VIRTUAL_FUNCTION_0
	virtual OBJID	SpawnNewPetID()								PURE_VIRTUAL_FUNCTION_0
	virtual void	RecyclePetID(OBJID id)						PURE_VIRTUAL_FUNCTION
	virtual OBJID	SpawnNewNpcID()								PURE_VIRTUAL_FUNCTION_0
	virtual void	RecycleDynaNpcID(OBJID id)					PURE_VIRTUAL_FUNCTION
	virtual OBJID	SpawnNewTrapID()							PURE_VIRTUAL_FUNCTION_0
	virtual void	RecycleTrapID(OBJID id)						PURE_VIRTUAL_FUNCTION
	virtual OBJID	SpawnNewCallPetID()							PURE_VIRTUAL_FUNCTION_0
	virtual void	RecycleCallPetID(OBJID id)					PURE_VIRTUAL_FUNCTION
	virtual OBJID	SpawnNewVItemID()							PURE_VIRTUAL_FUNCTION_0
	virtual void	RecycleVItemID(OBJID id)					PURE_VIRTUAL_FUNCTION
	virtual bool	IsMapDyna(OBJID idMap)						PURE_VIRTUAL_FUNCTION_0
	virtual IMapData* GetMapDataTemplate(UINT nMapDoc)			PURE_VIRTUAL_FUNCTION_0
	virtual bool AddMapData(UINT nMapDoc,IMapData* pMapData)	PURE_VIRTUAL_FUNCTION_0
	virtual	bool	CreateSpecilLineMap()						PURE_VIRTUAL_FUNCTION_0
};

class CMapManager : IMapManager
{
public:
	CMapManager(PROCESS_ID idProcess);
	virtual ~CMapManager();
	IMapManager*	GetInterface() { return (IMapManager*)this; }
	bool			Create();
	bool			CreateSpecilLineMap();
//	virtual CGameMap*	CreateGameMap();
	virtual void	OnTimer(time_t tCurr);

public:
	virtual void		Release();
	virtual CGameMap*	QueryMap(OBJID idMap, BOOL bLoad=false)			{ CHECKF(m_pMapSet); return GetGameMap(idMap, bLoad); }
	virtual IMapSet*	QuerySet()						{ CHECKF(m_pMapSet); return m_pMapSet; }
	virtual CGameMap*	GetGameMap(OBJID idMap, BOOL bLoad=false);
	virtual int			GetGameMapAmount()				{ return m_pMapSet->GetAmount(); }
	virtual CGameMap*	GetGameMapByIndex(int idx)		{ return m_pMapSet->GetObjByIndex(idx); }
	virtual DWORD		FindPassway(OBJID idMap, int idxPortal);
	virtual OBJID		CreateDynaMap(const NewMapInfo* pInfo);
	virtual OBJID		CreateDynaMap(OBJID idMapType);
	virtual	BOOL		DeleteDynaMap(OBJID idMap);
	virtual BOOL		AddSynMap(CGameMap *pMap);
	// 预定义地图集，不包含玩家地图。
	virtual IStaticMapSet*	QuerySystemMapSet()			{ extern IStaticMapSet* g_setStaticMap; CHECKF(g_setStaticMap); return g_setStaticMap; }
	virtual bool		IsMapDyna(OBJID idMap)			{ return idMap > DYNAMIC_MAP_ID; }
protected: // internal port
	bool	IsDynaMap(OBJID idMap)						{ return idMap > DYNAMIC_MAP_ID; }


public:
	virtual CBooth*	QueryBooth(OBJID idBooth)	{ CHECKF(m_pBoothSet); return m_pBoothSet->GetObj(idBooth); }
	virtual CBoothSet*	QueryBoothSet()			{ CHECKF(m_pBoothSet); return m_pBoothSet; }
public: // mapitem
	virtual CMapItem*	QueryMapItem(OBJID idMapItem)	{ CHECKF(m_pMapItemSet); return m_pMapItemSet->GetObj(idMapItem); }
	virtual CMapItemSet*	QueryMapItemSet()			{ CHECKF(m_pMapItemSet); return m_pMapItemSet; }
	virtual OBJID		SpawnMapItemID();

public:
	virtual PROCESS_ID	GetProcessID(OBJID idMap);
	virtual	CGameMapData* GetMapData(OBJID idMap);

	//zeng添加，地图模板操作
	IMapData* GetMapDataTemplate(UINT nMapDoc);
	bool AddMapData(UINT nMapDoc,IMapData* pMapData);
	void ReleaseMapData();

public: // spawn id
	OBJID	SpawnNewPetID();
	void	RecyclePetID(OBJID id)						{ if(id != ID_NONE) m_setPetRecycle.push(id); }
	OBJID	SpawnNewNpcID();
	void	RecycleDynaNpcID(OBJID id)					{ if(id != ID_NONE) m_setDynaNpcRecycle.push(id); }
	OBJID	SpawnNewTrapID();
	void	RecycleTrapID(OBJID id)						{ if(id != ID_NONE) m_setTrapRecycle.push(id); }
	OBJID	SpawnNewCallPetID();
	void	RecycleCallPetID(OBJID id)					{ if(id != ID_NONE) m_setCallPetRecycle.push(id); }
	OBJID	SpawnNewVItemID();
	void	RecycleVItemID(OBJID id)					{ if(id != ID_NONE) m_setVItemRecycle.push(id); }
protected:
	OBJID				m_idLastPet;
	CMyQueue<OBJID>		m_setPetRecycle;
	OBJID				m_idLastDynaNpc;
	CMyQueue<OBJID>		m_setDynaNpcRecycle;
	OBJID				m_idLastTrap;
	CMyQueue<OBJID>		m_setTrapRecycle;
	OBJID				m_idLastCallPet;
	CMyQueue<OBJID>		m_setCallPetRecycle;
	OBJID				m_idLastVItem;
	CMyQueue<OBJID>		m_setVItemRecycle;

protected: // ctrl
	PROCESS_ID	m_idProcess;

protected:
	IMapSet*		m_pMapSet;
	IPasswaySet*	m_setPassway;

protected: // mapitem
	CMapItemSet*	m_pMapItemSet;
	CBoothSet*		m_pBoothSet;
	OBJID			m_idLastMapItem;
	CTimeOut		m_tMapItem;

	//zeng添加,地图模板
	map<UINT,IMapData*>	m_MapTemplate;

public:
	MYHEAP_DECLARATION(s_heap)
};




#endif // !defined(AFX_MAPMANAGER_H__4DC38FE0_1341_4D20_8C0A_47517229F518__INCLUDED_)
