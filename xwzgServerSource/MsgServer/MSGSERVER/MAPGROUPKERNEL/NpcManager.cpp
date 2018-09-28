// NpcManager.cpp: implementation of the CNpcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MessagePort.h"
#include "AllMsg.h"
#include "protocol.h"
#include "MapGroup.h"
#include "MapTrap.h"
#include "InstanceNPC.h"
#include "SettledAction.h"	//  [2007-8-21] ���
#include "NpcManager.h"
#include "MapInstanceMgr.h"


MYHEAP_IMPLEMENTATION(CNpcManager,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcManager::CNpcManager(PROCESS_ID idProcess) : m_idProcess(idProcess)
{
	m_idLastNewNpc = MONSTERID_FIRST;
}

//////////////////////////////////////////////////////////////////////
CNpcManager::~CNpcManager()
{
	m_YabiaoRule.clear();
	m_ShopList.clear();
}

///idGenΪ0ʱ���ǹر�����GEN
void CNpcManager::CloseMonsterGenerat(OBJID idMap,OBJID idGen)
{
	GeneratMonster(idMap,idGen,MSGAINPCINFO_REMOVEGEN);
}

///ͨ��GEG�ڸ�����ˢ��
void CNpcManager::GeneratMonster(OBJID idMap,OBJID idGen,UINT nAction)
{
// 	ST_CREATENEWNPC	info;
// 	memset(&info, 0, sizeof(ST_CREATENEWNPC));
// 	info.id				= idGen;
// 	info.usAction		= nAction;
//	info.usType			= nMonsterType;
//	info.ucOwnerType	= m_pData->GetInt(PETDATA_OWNERTYPE);
//	info.idOwner		= m_pData->GetInt(PETDATA_OWNERID);
//	info.idMap			= idMap;
//	info.usPosX			= 100;
//	info.usPosY			= 200;
//	info.idData			= -1
//	info.nData			= GetMapType();

//	CMsgMonsterInfo	msg;
//	IF_OK(msg.Create(&info))
//		this->SendMsg(&msg);

// 	CMsgMonsterInfo	msg;
// 	IF_OK(msg.Create(&info))
// 	{
// 		MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 	}	
}

void CNpcManager::GeneratMonster(UINT nMapType,UINT nMonsterType,UINT nPosX,UINT nPosY)
{
//	ST_CREATENEWNPC	info;
//	memset(&info, 0, sizeof(ST_CREATENEWNPC));
//	//not in this			info.id				= MapManager()->SpawnNewPetID();
//	info.idMap			= nMapType;//pNpc->GetMapID();
//	info.idData			= 1;
//	info.idOwner		= 0;//pNpc->GetInt(NPCDATA_OWNERID);		// �ܰ�
//	info.usPosX			= nPosX;
//	info.usPosY			= nPosY;
//	//			info.usAction		= MSGAINPCINFO_CREATENEW;
//	info.usType			= nMonsterType;
//	info.ucOwnerType	= 0;//pNpc->GetInt(NPCDATA_OWNERTYPE);
//	info.nData			= 0;//nData;
//	NpcManager()->CreateSynPet(&info);

// 	ST_CREATENEWNPC	info;
// 	memset(&info, 0, sizeof(ST_CREATENEWNPC));
//	info.id				= GetID();
// 	info.usAction		= MSGAINPCINFO_CREATEGENNEW;
// 	info.usType			= nMonsterType;
//	info.ucOwnerType	= m_pData->GetInt(PETDATA_OWNERTYPE);
//	info.idOwner		= m_pData->GetInt(PETDATA_OWNERID);
//	info.idMap			= nMapType;
//	info.usPosX			= 100;
//	info.usPosY			= 200;
//	info.idData			= -1
//	info.nData			= GetMapType();

//	CMsgMonsterInfo	msg;
//	IF_OK(msg.Create(&info))
//		this->SendMsg(&msg);

// 	CMsgMonsterInfo	msg;
// 	IF_OK(msg.Create(&info))
// 	{
// 		MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 	}
}

bool CNpcManager::RemoveSynNpc(OBJID idSynNpc)
{
	bool hr=false;
	IRole* pRole =(CNpcPtr)RoleManager()->QuerySet()->GetObj(idSynNpc);
	if(!pRole)
		return false;

	CNpc* pNpc=NULL;
	pRole->QueryObj(OBJ_NPC,IPP_OF(pNpc));
	CHECKF(pNpc);
	pNpc->DelNpc();
	RoleManager()->QuerySet()->DelObj(idSynNpc);
	return hr;
}

OBJID CNpcManager::CreateInstanceNpc(UINT nNpcType,OBJID idOwn,UINT nInstanceType)///����Ϊָ��ӵ����һ������NPC
{
	OBJID idInstanceNpc=ID_NONE;
	IRole* pRole=RoleManager()->QueryRole(idOwn);
	CHECKF(pRole);
	CGameMap* pMap=pRole->GetMap();
	CHECKF(pMap);
	POINT pos={0};
	OBJID idMap=ID_NONE;
	pos.x	= pRole->GetPosX();
	pos.y	= pRole->GetPosY();
	if(pMap->FindDropItemCell(MONSTERDROPITEM_RANGE, &pos,OBJ_NPC))
		idInstanceNpc=CreateSynNpc(nNpcType,pMap->GetID(),pos.x,pos.y,1/*idOwn*/,0/*nInstanceType*/);
	else
		///����ʾ,�ݲ�֧��
		idInstanceNpc=ID_NONE;
	return idInstanceNpc;
}
void CNpcManager::CreateNewSynNpc(int nMapid,int nNpcID,int nPosx,int nPosy)
{
	CreateSynMapNpc(nNpcID,nMapid,nPosx,nPosy);
}
OBJID CNpcManager::CreateSynNpc(UINT nNpcType,OBJID idMap, int nPosX, int nPosY,int nParam1,int nParam2)
{
	OBJID hr=ID_NONE;
	
	SQLBUF szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE id=%u", _TBL_NPC,nNpcType);
	::CAutoPtr<IRecordset> pRes = Database()->CreateNewRecordset(szSQL, false);
	if(!pRes)
		return false;

	CNpcPtr pNpc = CreateNpc((NPCSORT)pRes->GetInt(NPCDATA_SORT));
	if(!pNpc)
	{
// 		pRes->Release();
		return false;
	}

	if(nParam1 == 0)
	{
		if(GetNpc(nNpcType))
		{
			LogSave("NPC:%u show error",nNpcType);
			return false;
		}
	}

	DEBUG_TRY
	DoBroadNpcMsg(nNpcType);
	DEBUG_CATCH("Syn DoBroadNpcMsg crash")

	///��̬����NPCλ��
	pRes->SetInt("mapid",idMap);
	pRes->SetInt("cellx",nPosX);
	pRes->SetInt("celly",nPosY);
	if(!pNpc->Create(m_idProcess, pRes, nParam1) || !pNpc->InitNpc(nParam1,nParam2))		// false: not dynamic
		pNpc->ReleaseByOwner();					//?? �Ǳ���ͼ��NPC
	else
	{
		RoleManager()->QuerySet()->AddObj(pNpc->QueryRole());
		pNpc->SendSelfToBlock();
		hr=pNpc->GetID();
	}
	return hr;
}
void	CNpcManager::AddShopNpcToList(NPCSHOP npcshop)
{
		m_ShopList.push_back(npcshop);
}
CNpc*	CNpcManager::QueryShopNpcByNPCID(OBJID idNpc)//--jay
{
	for (int i=0;i<m_ShopList.size();i++)
	{
		if (m_ShopList[i].idNpc==idNpc)
			return GetNpc(idNpc);
	}
	return NULL;
}
void CNpcManager::CreateSynMonster(OBJID idmap,int nPosx,int nPosy,OBJID idType,OBJID idsyn)
{
	ST_CREATENEWNPC	info;
	memset(&info, 0, sizeof(ST_CREATENEWNPC));
	CSynPtr pSyn=SynManager()->QuerySyndicate(idsyn);
	if (!pSyn)
		return;
	if (idmap==1048)
		idmap=pSyn->GetSynMapID();
	info.idMap			= idmap;//pNpc->GetMapID();
	info.idData			= 0;//1745;//20080131����:��id��֤��sk_generator�д���
	info.idOwner		= 0;//pNpc->GetInt(NPCDATA_OWNERID);		// �ܰ�
	info.usPosX			= nPosx;
	info.usPosY			= nPosy;
	
	info.usType			= idType;
	info.ucOwnerType	= 0;//pNpc->GetInt(NPCDATA_OWNERTYPE);
	info.nData			= 0;//nData;
	
	if(idmap>=SYN_MAP_DOWN && idmap < SYN_MAP_UP)
	{
		CMonsterPtr pMonster = NpcManager()->CreateNewSynBoss(&info);
		if (pMonster!=NULL)
		{
			pSyn->AddSynMonster(pMonster);
		}
	}
}
CNpc*	CNpcManager::QueryShopNpcByNPCType(OBJID idType)//--jay
{
	for (int i=0;i<m_ShopList.size();i++)
	{
		if (m_ShopList[i].idTypeNpc==idType)
			return GetNpc(m_ShopList[i].idNpc);
	}
	return NULL;
}
OBJID CNpcManager::QueryTypeIDByNpcID(OBJID idNpc)//--jay
{
	for (int i=0;i<m_ShopList.size();i++)
	{
		if (m_ShopList[i].idNpc==idNpc)
			return m_ShopList[i].idTypeNpc;
	}
	return 0;
}
OBJID CNpcManager::QueryNpcIDByTypeMapID(OBJID idTypeNpc,OBJID idmap)//--jay
{
	for (int i=0;i<m_ShopList.size();i++)
	{
		if (m_ShopList[i].idTypeNpc==idTypeNpc)
		{
			if (GetNpc(m_ShopList[i].idNpc)->GetMapID()==idmap)
				return m_ShopList[i].idNpc;
		}
	}
	return 0;
}
void CNpcManager::DoBroadNpcMsg(UINT nNpcType)
{
	char Strtemp[512];
	CMsgTalk	msg;
	if (2008 == nNpcType)
	{
		sprintf(Strtemp, "<3_�����_6>�⵽�������Ϯ����<3_����ӣ����_6>�ٻ����ŵ���ƽ�ң����Ե������ٴ�����ʿ����������Ԯ��");
	}
	else if (2108 == nNpcType)
	{
		sprintf(Strtemp, "<3_ȫ���_6>�⵽�������Ϯ����<3_����������_6>�ٻ����ŵ���ƽ�ң���ȫ��������ٴ�����ʿ����������Ԯ��");
	}
	else if (2208 == nNpcType)
	{
		sprintf(Strtemp, "<3_��ü��_6>�⵽�������Ϯ����<3_��������_6>�ٻ����ŵ���ƽ�ң����ü�������ٴ�����ʿ����������Ԯ��");
	}
	else if (2308 == nNpcType)
	{
		sprintf(Strtemp, "<3_����_6>�⵽�������Ϯ����<3_��������_6>�ٻ����ŵ���ƽ�ң������ŵ������ٴ�����ʿ����������Ԯ��");
	}
	else if (2408 == nNpcType)
	{
		sprintf(Strtemp, "<3_ؤ��_6>�⵽�������Ϯ����<3_�𲻻�����_6>�ٻ����ŵ���ƽ�ң���ؤ��������ٴ�����ʿ����������Ԯ��");
	}
	else if (2508 == nNpcType)
	{
		sprintf(Strtemp, "<3_������_6>�⵽�������Ϯ����<3_��������_6>�ٻ����ŵ���ƽ�ң������ֵ������ٴ�����ʿ����������Ԯ��");
	}
	else
		return;
	if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, Strtemp, NULL, 0x00a651, _TXTATR_TOPANNOUNCE))
		MapGroup(PID)->GetUserManager()->BroadcastMapGroupMsg(&msg);
	return;
}

CNpc* CNpcManager::GetNpc(OBJID idNpc)
{
	IRole* pRole=RoleManager()->QueryRole(idNpc);
	if(!pRole)
		return NULL;

	CNpc* pNpc=NULL;
	pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc));
	return pNpc;
}

CNpc* CNpcManager::CreateNpc(NPCSORT nsNpcSort)
{
	CNpc* pNpc=NULL;
	switch(nsNpcSort)
	{
	case NPCSORT_DYNANPC_INSTANCE:
		pNpc=CInstanceNPC::CreateNew();
		break;
	default:
		pNpc=CNpc::CreateNew();
		break;
	}
	return pNpc;
}
//////////////////////////////////////////////////////////////////////
bool CNpcManager::Create()
{
	// tasknpc set
	SQLBUF szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE sort < 3", _TBL_NPC);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
	if(!pRes)
		return false;
	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		CNpcPtr pNpc = CNpc::CreateNew();
		if(!pNpc)
		{
			pRes->Release();
			return false;
		}

		if(!pNpc->Create(m_idProcess, pRes, false))		// false: not dynamic
			pNpc->ReleaseByOwner();					//?? �Ǳ���ͼ��NPC
		else
			RoleManager()->QuerySet()->AddObj(pNpc->QueryRole());

		pRes->MoveNext();
	}
	pRes->Release();

	////softworms-�����˶�̬NPC����.
	// dyn npc set
//	sprintf(szSQL, "SELECT * FROM %s WHERE (idxserver=-1 || idxserver=%u)", _TBL_DYNANPC, GetServerID());
//	 pRes = Database()->CreateNewRecordset(szSQL, false);
//	if(!pRes)
//		return false;
//	for( i = 0; i < pRes->RecordCount(); i++)
//	{
//		CNpcPtr pNpc = CNpc::CreateNew();
//		if(!pNpc)
//		{
//			pRes->Release();
//			return false;
//		}
//
//		if(!pNpc->Create(m_idProcess, pRes, true))		// true: dynamic
//			pNpc->ReleaseByOwner();					//?? �Ǳ���ͼ��NPC
//		else
//			RoleManager()->QuerySet()->AddObj(pNpc->QueryRole());
//
//		pRes->MoveNext();
//	}
//	pRes->Release();

	// map trap set (�����)
// 	sprintf(szSQL, "SELECT * FROM %s", _TBL_TRAP);
// 	 pRes = Database()->CreateNewRecordset(szSQL, false);
// 	if(!pRes)
// 		return false;
// 	for( i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
// 	{
// 		CMapTrap* pTrap = CMapTrap::CreateNew(m_idProcess, pRes);
// 		if(pTrap)		//? may be other map group
// 			RoleManager()->QuerySet()->AddObj(pTrap->QueryRole());
// 	}
// 	pRes->Release();
// 
// 
// 	CreateYabiao();
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CNpcManager::CreateSpecilLineNpc()
{
	// tasknpc set
	SQLBUF szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE sort = 22", _TBL_NPC);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
	if(!pRes)
		return false;
	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		OBJID idMap = pRes->LoadInt("mapid");
		if(idMap==ID_NONE)
		{
			pRes->MoveNext();
			continue;
		}
// 		if(!CWargame::IsWarGameMap(idMap))
// 		{
// 			pRes->MoveNext();
// 			continue;
// 		}
		if (!MapManager()->QueryMap(idMap) || MapManager()->QueryMap(idMap)->GetType()!=3)
		{
			pRes->MoveNext();
			continue;
		}
		CNpcPtr pNpc = CNpc::CreateNew();
		if(!pNpc)
		{
			pRes->Release();
			return false;
		}
		
		if(!pNpc->Create(m_idProcess, pRes, false))		// false: not dynamic
			pNpc->ReleaseByOwner();					//?? �Ǳ���ͼ��NPC
		else
			RoleManager()->QuerySet()->AddObj(pNpc->QueryRole());
		
		pRes->MoveNext();
	}
	pRes->Release();
	
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CNpcManager::DelAllMonster()
{
	DEBUG_TRY		// VVVVVVVVVVVVVVV
	LOGMSG("DelAllMonster() start...");
	IRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
	while(pRole.Next())
	{
		CMonster* pMonster;
//		CUser* pUser;
		if(pRole) 
		{
			if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !pMonster->IsEudemon())
			{
				RoleManager()->QuerySet()->DelObj(pRole);
			}
		}
	}
	LOGMSG("DelAllMonster() ok.");
	DEBUG_CATCH("delete all npc")		// AAAAAAAAAAAAAAAAA

//	RoleManager()->LoadAllPet();

	return true;
}

///�½ӿ�֧���ڸ�����ˢ��
bool CNpcManager::CreatePowerMonster(const ST_CREATENEWNPC* pInfo)
{
	CHECKF(pInfo);
	switch(pInfo->usAction)
	{
		case MSGAINPCINFO_CREATENEW:
			return CreateMonster(pInfo);
			break;
		case MSGAINPCINFO_CREATEGENNEW:
			return MapInstanceMgr()->CreateMonster((ST_CREATENEWNPC*)pInfo);
		default:
			break;
	}
	return 0;
}

OBJID CNpcManager::CreateMonster(const ST_CREATENEWNPCNEW Info)
{
	//��ͨ��ͼ��
	if(m_idProcess != 3)
		return false;

	CMonsterPtr pNpc = CMonster::CreateNew();
	CHECKF(pNpc);
	CNpcType*	pType = MonsterType()->GetObj(Info.usType);
	if(pType)
	{
		ST_CREATENEWNPC	info;
 		info.id = SpawnNewGenMonsterID();
		info.idMap = Info.idMap;
		info.idData = Info.idGen;
		info.idOwner = 0;
		info.nData = 0;
		info.usPosX = Info.usPosX;
		info.usPosY = Info.usPosY;
		info.usAction = 0;
		info.usType = Info.usType;
		info.ucOwnerType = 0;
		info.unCitywarType = 0;
		CMapPtr pMap = MapManager()->GetGameMap(Info.idMap,FALSE);			//? need not ?
		if(pMap && pNpc->Create(m_idProcess, pType, &info))
		{
			RoleManager()->QuerySet()->AddObj(pNpc->QueryRole());
			pNpc->EnterMapGroup();
			return info.id;
		}
	}
	pNpc->ReleaseByOwner();
	return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
bool CNpcManager::CreateMonster(const ST_CREATENEWNPC* pInfo)
{	
	CMonsterPtr pNpc = CMonster::CreateNew();
	if(pNpc)
	{
		CNpcType*	pType = MonsterType()->GetObj(pInfo->usType);
		if(pType)
		{
			ST_CREATENEWNPC	info;
			memcpy(&info, pInfo, sizeof(ST_CREATENEWNPC));
			info.idOwner	= ID_NONE;
			info.ucOwnerType= OWNER_NONE;
			info.usAction	= MSGAINPCINFO_CREATENEW;

			CMapPtr pMap = MapManager()->GetGameMap(pInfo->idMap,FALSE);			//? need not ?
			if(pMap && pNpc->Create(m_idProcess, pType, &info))
			{
				//////////////////////////////////////////////////////////////////////////
				//  [2007-8-21] ��� C�ڿ�_20052007080301_�߾�սϵͳ�Ĳ߻���
// 				ST_SETTLEDACTIONINFO* pInfoTemp = g_settledAction.GetSettledActionInfoByType(EM_SETLDACTN_TYPE_NPC_CREATE, pInfo->usType);
// 				if(pInfoTemp && pInfoTemp->actionid)
// 				{
// 					GameAction()->ProcessAction(pInfoTemp->actionid, NULL, pNpc->QueryRole());
// 				}
				//////////////////////////////////////////////////////////////////////////
				RoleManager()->QuerySet()->AddObj(pNpc->QueryRole());
				clock_t clEnterMapBegin = clock();
				pNpc->EnterMapGroup();
				clock_t clEnterMapUsed = clock() - clEnterMapBegin;
#ifdef _DEBUG
				if(clEnterMapUsed > 30)
				{
					::LogSave("overTime(%d)::when enterMapGroup:"
						"id:%d "
						"idMap:%d "
						"idData:%d "
						"idOwner:%d "
						"nData:%d "
						"usPosX:%d "
						"usPosY:%d "
						"usAction:%d "
						"usType:%d "
						"ucOwnerType:%d ",
						clEnterMapUsed,
						pInfo->id,
						pInfo->idMap,
						pInfo->idData,
						pInfo->idOwner,
						pInfo->nData,
						pInfo->usPosX,
						pInfo->usPosY,
						pInfo->usAction,
						pInfo->usType,
						pInfo->ucOwnerType
						);
				}
#endif 

				return true;
			}
		}
		pNpc->ReleaseByOwner();
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CNpcManager::CreateSynPet(const ST_CREATENEWNPC* pInfo, LPCTSTR pszName/*=NULL*/)
{
	CHECKF(pInfo);

	CMonsterPtr pPet = CMonster::CreateNew();
	if(pPet)
	{
		CNpcType*	pType = MonsterType()->GetObj(pInfo->usType);
		if(pType)
		{
			ST_CREATENEWNPC	info;
			memcpy(&info, pInfo, sizeof(ST_CREATENEWNPC));
			info.id				= MapManager()->SpawnNewPetID();
			info.usAction		= MSGAINPCINFO_CREATENEW;
			CMapPtr pMap;
			if (pInfo->idMap==1048)
			{
				CUserPtr m_pUser=UserManager()->GetUser(pInfo->idOwner);
				CHECKF(m_pUser);
				pMap=m_pUser->GetSynMap();
			}
			else
				pMap= MapManager()->QueryMap(pInfo->idMap);			//??? need not? mast check not current mapgroup case!!!
			if(pMap && pPet->Create(m_idProcess, pType, &info, pszName))
			{
				//////////////////////////////////////////////////////////////////////////
				//  [2007-9-21] ��� C����_20062007092001_�����̶�action�ӿ�
// 				ST_SETTLEDACTIONINFO* pInfoTemp = g_settledAction.GetSettledActionInfoByType(EM_SETLDACTN_TYPE_NPC_CREATE, pInfo->usType);
// 				if(pInfoTemp && pInfoTemp->actionid)
// 				{
// 					GameAction()->ProcessAction(pInfoTemp->actionid, NULL, pPet->QueryRole());
// 				}
				//////////////////////////////////////////////////////////////////////////

				RoleManager()->QuerySet()->AddObj(pPet->QueryRole());
				pPet->EnterMapGroup();

				return true;
			}
		}
		pPet->ReleaseByOwner();
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
CMonsterPtr CNpcManager::CreateNewSynBoss(const  ST_CREATENEWNPC* pInfo, LPCTSTR pszName/*=NULL*/)
{
	CHECKF(pInfo);
	
	CMonsterPtr pPet = CMonster::CreateNew();
	if(pPet)
	{
		CNpcType*	pType = MonsterType()->GetObj(pInfo->usType);
		if(pType)
		{
			ST_CREATENEWNPC	info;
			memcpy(&info, pInfo, sizeof(ST_CREATENEWNPC));
			info.id				= MapManager()->SpawnNewPetID();
			info.usAction		= MSGAINPCINFO_CREATENEW;
			CMapPtr pMap;
			if (pInfo->idMap==1048)
			{
				CUserPtr m_pUser=UserManager()->GetUser(pInfo->idOwner);
				CHECKF(m_pUser);
				pMap=m_pUser->GetSynMap();
				
			}
			else
				pMap= MapManager()->QueryMap(pInfo->idMap);			//??? need not? mast check not current mapgroup case!!!
			if(pMap && pPet->Create(m_idProcess, pType, &info, pszName))
			{
				//////////////////////////////////////////////////////////////////////////
				//  [2007-9-21] ��� C����_20062007092001_�����̶�action�ӿ�
				// 				ST_SETTLEDACTIONINFO* pInfoTemp = g_settledAction.GetSettledActionInfoByType(EM_SETLDACTN_TYPE_NPC_CREATE, pInfo->usType);
				// 				if(pInfoTemp && pInfoTemp->actionid)
				// 				{
				// 					GameAction()->ProcessAction(pInfoTemp->actionid, NULL, pPet->QueryRole());
				// 				}
				//////////////////////////////////////////////////////////////////////////
				
				RoleManager()->QuerySet()->AddObj(pPet->QueryRole());
				pPet->EnterMapGroup();
				
				return pPet;
			}
		}
		pPet->ReleaseByOwner();
	}
	return NULL;
}
//satan 2009.08.07 �¼�һ��   ˢ�ֺ󷵻ع���ID
OBJID CNpcManager::CreateSynPetReturnID(const ST_CREATENEWNPC* pInfo, LPCTSTR pszName/*=NULL*/)
{
	CHECKF(pInfo);
	
	CMonsterPtr pPet = CMonster::CreateNew();
	if(pPet)
	{
		CNpcType*	pType = MonsterType()->GetObj(pInfo->usType);
		if(pType)
		{
			ST_CREATENEWNPC	info;
			memcpy(&info, pInfo, sizeof(ST_CREATENEWNPC));
			info.id				= MapManager()->SpawnNewPetID();
			info.usAction		= MSGAINPCINFO_CREATENEW;
			
			CMapPtr pMap = MapManager()->QueryMap(pInfo->idMap);			//??? need not? mast check not current mapgroup case!!!
			if(pMap && pPet->Create(m_idProcess, pType, &info, pszName))
			{
				//////////////////////////////////////////////////////////////////////////
				//  [2007-9-21] ��� C����_20062007092001_�����̶�action�ӿ�
// 				ST_SETTLEDACTIONINFO* pInfoTemp = g_settledAction.GetSettledActionInfoByType(EM_SETLDACTN_TYPE_NPC_CREATE, pInfo->usType);
// 				if(pInfoTemp && pInfoTemp->actionid)
// 				{
// 					GameAction()->ProcessAction(pInfoTemp->actionid, NULL, pPet->QueryRole());
// 				}
				//////////////////////////////////////////////////////////////////////////
				pPet->SetOwnerID(pInfo->idOwner);
				if(pszName)
					pPet->SetOwnerName(pszName);
				RoleManager()->QuerySet()->AddObj(pPet->QueryRole());
				pPet->EnterMapGroup();
				
				return info.id;
			}
		}
		pPet->ReleaseByOwner();
	}
	return 0;
}

CMonster* CNpcManager::CallEudemon(CUser* pUser,CUserPetData* pData,CEudemonTypeData* pTypeData,EudemonData eudemonData, int x, int y)
{
	if(pUser->GetMapID() == ID_NONE)
		return NULL;

	CHECKF(pUser);
	OBJID idPet = pData->GetInt(EUDEMONDATA_id);
	if(idPet <= EUDEMON_ID_FIRST || idPet >= EUDEMON_ID_LAST)
		return NULL;

// 	if(RoleManager()->QuerySet()->GetObj(pUser->m_idEudemonCur))
// 		RoleManager()->QuerySet()->DelObj(pUser->m_idEudemonCur);
 
	CMonster* pMonster;
	IRole* pRole = RoleManager()->QuerySet()->GetObj(idPet);
	if(pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && pMonster->IsEudemon())
		RoleManager()->QuerySet()->DelObj(idPet);


	CMonsterPtr pEudemon = CMonster::CreateNew();
	if(pEudemon)
	{
		ST_CREATENEWNPC	info;
		memset(&info, 0, sizeof(ST_CREATENEWNPC));
		info.id				= idPet;
		info.usAction		= MSGAINPCINFO_CREATENEW;
		info.usType			= pData->GetInt(EUDEMONDATA_typeid);
		info.ucOwnerType	= OWNER_USER;
		info.idOwner		= pUser->GetID();
		info.idMap			= pUser->GetMapID();
		info.usPosX			= x;
		info.usPosY			= y;
		info.idData			= 0;
		info.nData			= 0;
		
 		if(pEudemon->Create(m_idProcess, &info, pData,pTypeData, eudemonData,pUser))
 		{
			RoleManager()->QuerySet()->AddObj(pEudemon->QueryRole());
			pEudemon->EnterMapGroup();
			pUser->m_idEudemonCur = idPet;
			return pEudemon;
		}
		pEudemon->ReleaseByOwner();
	}
	return NULL;
}

CMonster* CNpcManager::CreateCallPet(CUser* pUser, OBJID idType, int x, int y)
{
	CHECKF(pUser);

	CMonsterPtr pPet = CMonster::CreateNew();
	if(pPet)
	{
		CNpcType*	pType = MonsterType()->GetObj(idType);
		if(pType)
		{
			ST_CREATENEWNPC	info;
			memset(&info, 0, sizeof(ST_CREATENEWNPC));
			info.id				= MapManager()->SpawnNewCallPetID();
			info.usAction		= MSGAINPCINFO_CREATENEW;
			info.usType			= idType;
			info.ucOwnerType	= OWNER_USER;
			info.idOwner		= pUser->GetID();
			info.idMap			= pUser->GetMapID();
			info.usPosX			= x;
			info.usPosY			= y;
			info.idData			= 0;
			info.nData			= 0;

			CMapPtr pMap = pUser->GetMap();			//??? need not? mast check not current mapgroup case!!!
			if(pMap && pPet->Create(m_idProcess, pType, &info, pUser))
			{
				//////////////////////////////////////////////////////////////////////////
				//  [2007-9-21] ��� C����_20062007092001_�����̶�action�ӿ�
// 				ST_SETTLEDACTIONINFO* pInfoTemp = g_settledAction.GetSettledActionInfoByType(EM_SETLDACTN_TYPE_NPC_CREATE, idType);
// 				if(pInfoTemp && pInfoTemp->actionid)
// 				{
// 					GameAction()->ProcessAction(pInfoTemp->actionid, NULL, pPet->QueryRole());
// 				}
				//////////////////////////////////////////////////////////////////////////
				
				RoleManager()->QuerySet()->AddObj(pPet->QueryRole());
				pPet->EnterMapGroup();

				return pPet;
			}
		}
		pPet->ReleaseByOwner();
	}
	return NULL;
}

bool CNpcManager::HasMonsterByType(int nMap ,int nType,int nCheckAmount)
{
	int amount  = 0;
	CRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
	while(pRole.Next())	
	{
		if(pRole  && pRole->GetMap() && (pRole->GetMap()->GetID() == nMap))
		{
			CMonster* pMonster = NULL;
			if(pRole->QueryObj(OBJ_MONSTER,IPP_OF(pMonster)))
			{
				if(pMonster && pMonster->IsAlive() && (pMonster->GetType() > nType && pMonster->GetType() <= nType + 30) )
					amount ++;
			}
		}
	}
	if(amount >= nCheckAmount)
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
//  [2007-8-28] ���
CMonster* CNpcManager::CreatePet(ST_CREATENEWNPC* pInfo)
{
	CHECKF(pInfo);

	CMonsterPtr pPet = CMonster::CreateNew();
	if(pPet)
	{
		CNpcType*	pType = MonsterType()->GetObj(pInfo->usType);
		if(pType)
		{
			ST_CREATENEWNPC	info;
			memcpy(&info, pInfo, sizeof(ST_CREATENEWNPC));
			info.id				= MapManager()->SpawnNewPetID();
			info.usAction		= MSGAINPCINFO_CREATENEW;
			info.idMap			= pInfo->idMap;
			info.idData			= 1745;
			info.usPosX			= pInfo->usPosX;
			info.usPosY			= pInfo->usPosY;
			info.idOwner		= 0;
			info.ucOwnerType	= 0;
			info.usType			= pInfo->usType;
			info.nData			= 0;

			CMapPtr pMap = MapManager()->QueryMap(pInfo->idMap);			//??? need not? mast check not current mapgroup case!!!

			if(pMap)
			{
				if(pPet->Create(m_idProcess, pType, &info))
				{
					//////////////////////////////////////////////////////////////////////////
					//  [2007-9-21] ��� C����_20062007092001_�����̶�action�ӿ�
// 					ST_SETTLEDACTIONINFO* pInfoTemp = g_settledAction.GetSettledActionInfoByType(EM_SETLDACTN_TYPE_NPC_CREATE, pInfo->usType);
// 					if(pInfoTemp && pInfoTemp->actionid)
// 					{
// 						GameAction()->ProcessAction(pInfoTemp->actionid, NULL, pPet->QueryRole());
// 					}
					//////////////////////////////////////////////////////////////////////////

					RoleManager()->QuerySet()->AddObj(pPet->QueryRole());
					pPet->EnterMapGroup();
					
					return pPet;
				}
				else
					LOGERROR("CNpcManager::CreatePet: Can not create Monster[%d] m_idProcess(%d)", pInfo->usType, m_idProcess);
			}
			else
			{
				LOGERROR("CNpcManager::CreatePet: Can not get map[%d] from MapManager m_idProcess", pInfo->idMap, m_idProcess);
			}	
		}
		else	//  [2007-9-20] ���	��ǿע��
		{
			LOGERROR("CNpcManager::CreatePet: Can not get CNpcType from MonsterType() type[%d]", pInfo->usType);
		}
		pPet->ReleaseByOwner();
	}
	return NULL;
}

void CNpcManager::DelMonster(CMonster *pMonster)
{
	CHECK(pMonster);
	RoleManager()->QuerySet()->DelObj(pMonster->GetID());
}

//satan 2009.08.07 �¼�һ��   ͨ������IDɾ������
void CNpcManager::DelMonsterByID(OBJID monsterID)
{	
	CMonster* pMonster;
	IRole* pRole = RoleManager()->QuerySet()->GetObj(monsterID);
	if(pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !pMonster->IsEudemon())
		RoleManager()->QuerySet()->DelObj(monsterID);
}

UINT CNpcManager::CreateSynMapNpc(UINT nNpcType,OBJID idSyn,int nPosX,int nPosY)
{
	CHECKF(idSyn);
	//����õ�ͼ
	CSynPtr psyn=SynManager()->QuerySyndicate(idSyn);
	CHECKF(psyn);

	UINT nNpcID = -1;
	NPCINFO_ST NpcInfo;
	memset(&NpcInfo,0,sizeof(NPCINFO_ST));

	INpcTypeSet* pSet = StaticNpcTypeSet();
	if(!pSet)
		return false;
	
	CStaticNpcType* pNpcType = pSet->GetObj(nNpcType);
	if(!pNpcType)
		return false;

	NpcInfo.idMap = psyn->GetSynMapID();
	NpcInfo.nPosX = nPosX;
	NpcInfo.nPosY = nPosY;
	NpcInfo.nType = pNpcType->GetInt(SATICNPCTYPEDATA_TYPE);
	NpcInfo.nLookFace = pNpcType->GetInt(SATICNPCTYPEDATA_LOOKFACE);
	NpcInfo.nAction = pNpcType->GetInt(SATICNPCTYPEDATA_ACTION);
	NpcInfo.nSort = pNpcType->GetInt(SATICNPCTYPEDATA_SORT);
	NpcInfo.nNpcType = nNpcType;
	NpcInfo.nLev =pNpcType->GetInt(SATICNPCTYPEDATA_LEVEL);

	CNpc* pDynaNpc = CNpc::CreateNew();
	if(!pDynaNpc->CreateSynMapNpc(m_idProcess,&NpcInfo,0,pNpcType->GetStr(SATICNPCTYPEDATA_NAME)))
	{
		LOGWARNING("SYNNPC CREATE ERROR!! NPCTYPE = %u SYNID = %u",nNpcType,idSyn);
		pDynaNpc->ReleaseByOwner();
	}

	MapGroup(m_idProcess)->GetRoleManager()->QuerySet()->AddObj(pDynaNpc->QueryRole());
	return pDynaNpc->GetID();
}

UINT CNpcManager::CreateDynaNpc(/*UINT idProcess,*/UINT nNpcType,UINT idMap,int nPosX,int nPosY)
{
	UINT nNpcID = -1;

	UINT idtest = m_idProcess;

	NPCINFO_ST NpcInfo;
	memset(&NpcInfo,0,sizeof(NPCINFO_ST));
	
	INpcTypeSet* pSet = StaticNpcTypeSet();
	if(!pSet)
		return false;

	 CStaticNpcType* pNpcType = pSet->GetObj(nNpcType);
	 if(!pNpcType)
		return false;



	NpcInfo.idMap = idMap;
	NpcInfo.nPosX = nPosX;
	NpcInfo.nPosY = nPosY;
	NpcInfo.nType = pNpcType->GetInt(SATICNPCTYPEDATA_TYPE);
	NpcInfo.nLookFace = pNpcType->GetInt(SATICNPCTYPEDATA_LOOKFACE);
	NpcInfo.nAction = pNpcType->GetInt(SATICNPCTYPEDATA_ACTION);
	NpcInfo.nSort = pNpcType->GetInt(SATICNPCTYPEDATA_SORT);
	NpcInfo.nNpcType = nNpcType;
	NpcInfo.nLev =pNpcType->GetInt(SATICNPCTYPEDATA_LEVEL);
	CNpc* pDynaNpc = CNpc::CreateNew();

// 	if (nNpcType>=9046 && nNpcType <=9050)
// 	{
// 		CSynPtr psyn=SynManager()->QuerySyndicate(idMap);
// 		if (psyn)
// 		{
// 			NpcInfo.idMap=psyn->GetSynMapID();
// 			pDynaNpc->Create(m_idProcess,&NpcInfo,0,pNpcType->GetStr(SATICNPCTYPEDATA_NAME));
// 			if(NpcManager()->QueryNpcIDByTypeMapID(nNpcType,pDynaNpc->GetMapID())==0)
// 			{	
// 				NPCSHOP npc;
// 				npc.idNpc=pDynaNpc->GetID();
// 				npc.idTypeNpc=nNpcType;
// 				NpcManager()->AddShopNpcToList(npc);
// 			}
// 		}	
// 	}
// 	else
	if(!pDynaNpc->Create(m_idProcess,&NpcInfo,0,pNpcType->GetStr(SATICNPCTYPEDATA_NAME)))
	{
		LOGWARNING("CreateDynaNpc CREATE ERROR!! NPCTYPE = %u idMap = %u",nNpcType,idMap);
		pDynaNpc->ReleaseByOwner();
	}

	MapGroup(m_idProcess)->GetRoleManager()->QuerySet()->AddObj(pDynaNpc->QueryRole());

	return pDynaNpc->GetID();

}

void CNpcManager::MoveMonsterDest(UINT idMap,UINT nType,int x,int y)
{
	
// 		CMsgMonsterInfo msg;
// 
// 	msg.CreateMoveDest(idMap,nType,x,y);
// 	MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(0,&msg);
}

OBJID CNpcManager::SpawnNewGenMonsterID()
{
	if(m_setRecycle.size() > 100)
	{
		return m_setRecycle.pop();
	}
	else if(m_idLastNewNpc < MONSTERID_LAST)
	{
		return ++m_idLastNewNpc;
	}
	else
	{
		m_idLastNewNpc = MONSTERID_FIRST;
		LOGERROR("NPC��ID�ռ䲻�㣬ǿ�лؾ�! NPC����ϵͳ���ֹ���!");
		return m_idLastNewNpc;
	}
}


