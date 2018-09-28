// NpcManager.h: interface for the CNpcManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMANAGER_H__D2C21D0D_B629_4AD2_9CE4_0BB87EA93A8C__INCLUDED_)
#define AFX_NPCMANAGER_H__D2C21D0D_B629_4AD2_9CE4_0BB87EA93A8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "protocol.h"
#include "AiNpc.h"
#include "GameObjSet.h"
#include "T_MyQueue.h"
#include "Npc.h"
#include "NpcType.h"
//#include "MsgAiNpcInfo.h"
#include <vector>
using namespace std;

typedef	IGameObjSet<CNpc>			INpcSet;
typedef	CGameObjSet<CNpc>			CNpcSet;
struct NPCSHOP
{
	OBJID idNpc;
	OBJID idTypeNpc; 
};


struct MYPATHRULE
{
	int nRule;
	OBJID action[4];
	int monsterID;
};
typedef	vector<MYPATHRULE>	MYPATHRULESET;

class INpcManager
{
public:
	virtual ~INpcManager()	{}
	virtual void	Release()																			PURE_VIRTUAL_FUNCTION
	virtual bool	DelAllMonster()																		PURE_VIRTUAL_FUNCTION_0
	virtual bool	CreateMonster(const ST_CREATENEWNPC* pInfo)											PURE_VIRTUAL_FUNCTION_0
	virtual bool	CreateSynPet(const ST_CREATENEWNPC* pInfo, LPCTSTR pszName=NULL)					PURE_VIRTUAL_FUNCTION_0
	virtual OBJID	CreateSynPetReturnID(const ST_CREATENEWNPC* pInfo, LPCTSTR pszName=NULL)			PURE_VIRTUAL_FUNCTION_0
	virtual CMonster*	CreateCallPet(CUser* pUser, OBJID idType, int x, int y)								PURE_VIRTUAL_FUNCTION_0
	virtual CMonster*	CreatePet(ST_CREATENEWNPC* pInfo)										PURE_VIRTUAL_FUNCTION_0
	virtual	void		DelMonster(CMonster*	pMonster)											PURE_VIRTUAL_FUNCTION
	virtual	void DelMonsterByID(OBJID monsterID)													PURE_VIRTUAL_FUNCTION
	virtual OBJID	CreateSynNpc(UINT nNpcType,OBJID idMap, int nPosX, int nPosY,int nParam1,int nParam2)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	RemoveSynNpc(OBJID idSynNpc) PURE_VIRTUAL_FUNCTION_0
	virtual CNpc*	GetNpc(OBJID idNpc) PURE_VIRTUAL_FUNCTION_0
	virtual OBJID	CreateInstanceNpc(UINT nNpcType,OBJID idOwn,UINT nInstanceType) PURE_VIRTUAL_FUNCTION_0///创建为指定拥有者一个副本NPC
	virtual void    CreateNewSynNpc(int nMapid,int nNpcID,int nPosx,int nPosy) PURE_VIRTUAL_FUNCTION
	virtual void	GeneratMonster(UINT nMapType,UINT nMonsterType,UINT nPosX,UINT nPosY) PURE_VIRTUAL_FUNCTION
	virtual bool	CreatePowerMonster(const ST_CREATENEWNPC* pInfo) PURE_VIRTUAL_FUNCTION_0///新接口副本刷怪
	virtual	void	GeneratMonster(OBJID idMap,OBJID idGen,UINT nAction=2) PURE_VIRTUAL_FUNCTION
	virtual	void	CloseMonsterGenerat(OBJID idMap,OBJID idGen) PURE_VIRTUAL_FUNCTION
	virtual	UINT	CreateSynMapNpc(UINT nNpcType,OBJID idSyn,int nPosX,int nPosY) PURE_VIRTUAL_FUNCTION_0
	virtual UINT	CreateDynaNpc(/*UINT idProcess,*/UINT nNpcType,UINT idMap,int nPosX,int nPosY) PURE_VIRTUAL_FUNCTION_0
	virtual void	MoveMonsterDest(UINT idMap,UINT nType,int x,int y) PURE_VIRTUAL_FUNCTION;
	virtual OBJID	GetYabiaoMonsterID(int nRule)		PURE_VIRTUAL_FUNCTION_0
	virtual OBJID	GetYabiaoAction(int nRule,int nType)	PURE_VIRTUAL_FUNCTION_0
	virtual CMonster* CallEudemon(CUser* pUser,CUserPetData* pData,CEudemonTypeData* pTypeData,EudemonData eudemonData, int x, int y) PURE_VIRTUAL_FUNCTION_0
	virtual bool	HasMonsterByType(int nMap ,int nType,int nCheckAmount = 1)  	PURE_VIRTUAL_FUNCTION_0
	virtual CNpc*	QueryShopNpcByNPCID(OBJID idNpc)PURE_VIRTUAL_FUNCTION_0//--jay
	virtual OBJID QueryNpcIDByTypeMapID(OBJID idTypeNpc,OBJID idmap)PURE_VIRTUAL_FUNCTION_0//--jay
	virtual	OBJID QueryTypeIDByNpcID(OBJID idNpc)PURE_VIRTUAL_FUNCTION_0//--jay
	virtual void AddShopNpcToList(NPCSHOP npcshop)PURE_VIRTUAL_FUNCTION
	virtual CNpc* QueryShopNpcByNPCType(OBJID idType)PURE_VIRTUAL_FUNCTION_0//--jay
	virtual CMonsterPtr CreateNewSynBoss(const ST_CREATENEWNPC* pInfo, LPCTSTR pszName=NULL)PURE_VIRTUAL_FUNCTION_0
	virtual void CreateSynMonster(OBJID idmap,int nPosx,int nPosy,OBJID idType,OBJID idsyn)PURE_VIRTUAL_FUNCTION
	virtual bool		CreateSpecilLineNpc() PURE_VIRTUAL_FUNCTION_0
	virtual void	RecycleGenMonsterID(OBJID idMonster) PURE_VIRTUAL_FUNCTION
	virtual OBJID	CreateMonster(const ST_CREATENEWNPCNEW Info) PURE_VIRTUAL_FUNCTION_0
};



class CNpcManager : INpcManager
{
public:
	CNpcManager(PROCESS_ID idProcess);
	virtual ~CNpcManager();
	INpcManager*	GetInterface()			{ return (INpcManager*)this; }
	bool		Create();

	bool		CreateSpecilLineNpc();
	///softworms-NPC创建工厂模式
	CNpc*		CreateNpc(NPCSORT nsNpcSort);
	void		DoBroadNpcMsg(UINT nNpcType);//根据刷怪发公告   20100917  JoyCc


public:	
	virtual void CreateSynMonster(OBJID idmap,int nPosx,int nPosy,OBJID idType,OBJID idsyn);
	virtual CNpc*	QueryShopNpcByNPCID(OBJID idNpc);//--jay
	virtual CNpc* QueryShopNpcByNPCType(OBJID idType);//--jay
	virtual	OBJID QueryNpcIDByTypeMapID(OBJID idTypeNpc,OBJID idmap);
	virtual	OBJID QueryTypeIDByNpcID(OBJID idNpc);//--jay
	virtual void AddShopNpcToList(NPCSHOP npcshop);
	virtual void	Release()				{ delete this; }
	virtual bool	DelAllMonster();
	virtual OBJID	CreateMonster(const ST_CREATENEWNPCNEW Info);
	virtual bool	CreateMonster(const ST_CREATENEWNPC* pInfo);
	virtual bool	CreatePowerMonster(const ST_CREATENEWNPC* pInfo);
	virtual bool	CreateSynPet(const ST_CREATENEWNPC* pInfo, LPCTSTR pszName=NULL);
	virtual CMonsterPtr CreateNewSynBoss(const ST_CREATENEWNPC* pInfo, LPCTSTR pszName=NULL);
	virtual OBJID	CreateSynPetReturnID(const ST_CREATENEWNPC* pInfo, LPCTSTR pszName=NULL);
	virtual CMonster*	CreateCallPet(CUser* pUser, OBJID idType, int x, int y);	
	virtual CMonster*	CreatePet(ST_CREATENEWNPC* pInfo);	//  [2007-8-28] 朱斌 创建怪物，并得其指针
	virtual void DelMonster(CMonster* pMonster);	//  [2007-8-28] 朱斌 删除一个怪物，杀死他杀死。。。
	virtual	void DelMonsterByID(OBJID monsterID);
	virtual OBJID	CreateSynNpc(UINT nNpcType,OBJID idMap, int nPosX, int nPosY,int nParam1,int nParam2);
	virtual void    CreateNewSynNpc(int nMapid,int nNpcID,int nPosx,int nPosy);
	virtual bool	RemoveSynNpc(OBJID idSynNpc);
	virtual OBJID	CreateInstanceNpc(UINT nNpcType,OBJID idOwn,UINT nInstanceType);///创建为指定拥有者一个副本NPC
	virtual CNpc*	GetNpc(OBJID idNpc);
	virtual void	GeneratMonster(UINT nMapType,UINT nMonsterType,UINT nPosX,UINT nPosY);
	virtual	void	GeneratMonster(OBJID idMap,OBJID idGen,UINT nAction=2);///MSGAINPCINFO_CREATEGENNEW
	virtual	void	CloseMonsterGenerat(OBJID idMap,OBJID idGen);
	virtual	UINT	CreateSynMapNpc(UINT nNpcType,OBJID idSyn,int nPosX,int nPosY);
	virtual UINT	CreateDynaNpc(/*UINT idProcess,*/UINT nNpcType,UINT idMap,int nPosX,int nPosY);
	virtual void	MoveMonsterDest(UINT idMap,UINT nType,int x,int y);
	virtual OBJID	GetYabiaoMonsterID(int nRule) {return 0;}
	virtual OBJID	GetYabiaoAction(int nRule,int nType) {return 0;}
	virtual CMonster* CallEudemon(CUser* pUser,CUserPetData* pData,CEudemonTypeData* pTypeData,EudemonData eudemonData, int x, int y);
	virtual bool	HasMonsterByType(int nMap ,int nType,int nCheckAmount = 1);
	virtual void	RecycleGenMonsterID(OBJID idMonster) {m_setRecycle.push(idMonster);}

protected:
	OBJID SpawnNewGenMonsterID();
	typedef	CMyQueue<OBJID>	NPCID_RECYCLE;
	NPCID_RECYCLE	m_setRecycle;
	OBJID m_idLastNewNpc;
//	void	CreateYabiao();
protected: // ctrl
	PROCESS_ID			m_idProcess;
protected:
	MYPATHRULESET m_YabiaoRule;
	vector<NPCSHOP> m_ShopList;
public:
	
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_NPCMANAGER_H__D2C21D0D_B629_4AD2_9CE4_0BB87EA93A8C__INCLUDED_)
