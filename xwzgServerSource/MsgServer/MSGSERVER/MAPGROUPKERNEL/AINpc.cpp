// AINpc.cpp: implementation of the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgAiNpcInfo.h"
#include "MsgCallPetInfo.h"
#include "MsgUserAttrib.h"
#include "MsgMagicEffect.h"
#include "MsgTalk.h"
#include "MsgPlayer.h"
#include "MsgAction.h"
#include "MsgEudemonInfo.h"
#include "MsgInteract.h"
#include "MsgItem.h"
#include "MsgNpc.h"//20080107����:
#include "MsgEudemonAttrib.h"

#include "GameMap.h"
#include "AINpc.h"
#include "MapGroup.h"
#include "MapItem.h"
#include "basefunc.h"
#include "ActionDefine.h"
#include "MapTrap.h"
#include "UserTable.h"
#include "Item.h"
#include "DeadLoop.h"
#include "ResourceWar.h"
#include "DropRuleMap.h"	//add by zlong 2003-11-15
#include "AdjustRate.h"
#include "NpcMagicManager.h"//20080118����:���＼�ܹ�����
#include "GiftManager.h"
#include "MsgBuff.h"
#include "MsgCoolDown.h"
#include "WargameManager.h"
#include "MsgWalkEx.h"
//#include "LuaScriptMachine.h"

//////////////////////////////////////////////////////////////////////
const int	RECRUITMAGIC_SECS					= 10;			// ÿ10��ʹ��һ�μ�Ѫħ��
const int	DEFAULT_MONSTER_MAGIC_LEVEL			= 0;
const int	UNIDENT_PERCENT						= 20;			// ����ͨ��Ʒʱ��δ�����ı���

const int	AUTO_INC_LIFE_SECS					= 10;			// ÿ10���Զ���������
const int	AUTO_INC_LIFE_PERCENT				= 10;			// ÿ���������������һ��

const int	SORB_REFLECT_SECS					= 6;			// ���շ���״̬ÿ��6������һ��

//////////////////////////////////////////////////////////////////////
//---jinggy---begin
#define EXPLODEMONEY_RATE	4	//��Ǯ���� 
#define EXPLODEMONEY_DOUBLE	15  //��Ǯʱ�ı���
//---jinggy---end
//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CMonster,s_heap)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMonster::CMonster()
{
	SetObjType(OBJ_MONSTER);
	m_pType			= NULL;
	m_idNpc			= ID_NONE;
	m_pBattleSystem	= NULL;

	m_tFootPrint	= 0;
	m_posFootPrint.x	= 0;
	m_posFootPrint.y	= 0;

	m_link.Init(this);
//	m_pMagicType	= NULL;
	m_pMagic		= NULL;

//	m_pEudemonItem	= NULL;
	m_pHelmet		= NULL;
	m_pNecklace		= NULL;
	m_pArmor		= NULL;
	m_pWeaponR		= NULL;
	m_pRingL		= NULL;
	m_pShoes		= NULL;
//	memset(&m_AddInfo, 0, sizeof(m_AddInfo));
	m_nPotential	= DEFAULT_EUDEMON_POTENTIAL;
	m_dwMaskData	= 0;

	m_bDecRole		= false;

	m_nKillNum		= 0;
	m_nKillNum4Potential	= 0;
	m_nKillNum4RelationShip	= 0;
	m_nNotice = 1;
	m_idOwner = 0;
	m_OwnerName[0] = 0;
	m_count1=0;
	m_count2=0;
	m_nAtkMode		= EATK_MODE_NONE;		// Ĭ�Ϲ�������Ϊ��	
	m_lastEudemonHappyCheck = clock();
	m_nSynId=ID_NONE;

	m_dwMoveInterTime = 1000;
	m_tMove.Clear();
	m_tWander.Clear();
	m_nAIStatus = EM_AISTATE_Initialize;
	m_nSeeUserNum = 0;
//modify code by �ֵ�Ԩ begin 2011.6.22
	m_idTargetBoss = ID_NONE;
//modify code by �ֵ�Ԩ end
}

//////////////////////////////////////////////////////////////////////
CMonster::~CMonster()
{
	int nStep = 0;
	DEBUG_TRY
	if(IsSynPet())
		MapManager()->RecyclePetID(GetID());

	if (GetID()>=MONSTERID_FIRST && GetID()<=MONSTERID_LAST)
	{
		NpcManager()->RecycleGenMonsterID(GetID());
		if(GetMap())
			GetMap()->GeneratorMonsterDel(GetGenID());
	}

	nStep = 1;
	if(m_idNpc != ID_NONE)
	{
		LeaveMapGroup();
		m_idNpc = ID_NONE;
	}

	nStep = 2;
	if (m_pBattleSystem)
	{
		delete m_pBattleSystem;
		m_pBattleSystem	= NULL;
	}
	
	nStep = 3;
	DEBUG_TRY
	if (m_pMagic)
		m_pMagic->ReleaseByOwner();
	DEBUG_CATCH("m_pMagic->ReleaseByOwner();");

	nStep = 4;
	SAFE_RELEASE(m_setStatus);

	DEBUG_CATCH2("CMonster::~CMonster() %u",nStep);
}

//////////////////////////////////////////////////////////////////////
bool CMonster::Create(PROCESS_ID idProcess, IRecordset* pRes)
{
	m_idProcess		= idProcess;

// 	ASSERT(!m_pData);
// 
// 	m_pData = CPetData::CreateNew();
// 	CHECKF(m_pData);
// 	IF_NOT(m_pData->Create(pRes))
// 		return false;
// 
// 	OBJID idType = m_pData->GetInt(PETDATA_TYPE);
// 	m_pType			= MonsterType()->GetObj(idType);
// 	CHECKF(m_pType);
// 
// 	m_nDir			= ::RandGet(8) + 1;
// 	if(m_nDir == 5)
// 		m_nDir ++;
// 	m_pMap			= MapManager()->GetGameMap(m_pData->GetInt(PETDATA_MAPID), false);		// false: not load dynamic map
// 	if(!m_pMap)
// 	{
// 		m_pData->Release();
// 		m_pData = NULL;
// 		return false;		// not this map group, or in dynamic map
// 	}
// 
// 	m_idNpc			= MapManager()->SpawnNewPetID();		//? mast follow m_pMap
// 	m_nPosX			= m_pData->GetInt(PETDATA_RECORD_X);
// 	m_nPosY			= m_pData->GetInt(PETDATA_RECORD_Y);
// //	m_dwStatus		= STATUS_NORMAL;
// 	m_i64Effect		= KEEPEFFECT_NORMAL;
// 	//[��;���� 2009.03.18]m_i64Effect����ʼ���ɴ�,�қ]��ʼ��m_i64Effect2.
// 	//m_i64Effect		= 0;
// 	m_i64Effect2	= KEEPEFFECT_NORMAL;
// 
// //	if((m_pType->GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_WING))
// //		m_i64Effect		= KEEPEFFECT_WING;
// 	m_nPose			= POSE_STAND;
// 	m_idGen			= m_pData->GetInt(PETDATA_GENID);
// 
// 	m_nCurrLife		= m_pData->GetInt(PETDATA_LIFE);
// 	m_nCurrMana		= m_pData->GetInt(PETDATA_MANA);
// 	m_tFight.SetInterval(m_pType->GetInt(NPCTYPEDATA_ATKSPEED));
// 	m_tFight.Update();
// 
// 	
// 	m_pBattleSystem	= new CBattleSystem(this->m_idProcess, this);
// 	CHECKF(m_pBattleSystem);
// 	m_setStatus = CStatusSet::CreateNew(true);
// 	CHECKF(m_setStatus);
// 
// 	// synchro
// 	ST_CREATENEWNPC	info;
// 	memset(&info, 0, sizeof(ST_CREATENEWNPC));
// 	info.id				= GetID();
// 	info.usAction		= MSGAINPCINFO_CREATENEW;
// 	info.usType			= GetType();
// 	info.ucOwnerType	= m_pData->GetInt(PETDATA_OWNERTYPE);
// 	info.idOwner		= m_pData->GetInt(PETDATA_OWNERID);
// 	info.idMap			= GetMap()->GetID();
// 	info.usPosX			= GetPosX();
// 	info.usPosY			= GetPosY();
// 	info.idData			= m_pData->GetInt(PETDATA_GENID);
// 	info.nData			= GetData();
// 	CMsgMonsterInfo	msg;
// 	IF_OK(msg.Create(&info))
// 		this->SendMsg(&msg);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::Create(PROCESS_ID idProcess, CNpcType* pType, const ST_CREATENEWNPC* pInfo, CUser* pUser, OBJID idItem)
{
	CHECKF(pType && pInfo && pUser);

	IF_OK(Create(idProcess, pType, pInfo, pUser->GetName()))
	{
		CHECKF(IsCallPet() || IsEudemon());

		m_pMagic = CMagic::CreateNew(idProcess, this->QueryRole());
		if (!m_pMagic || !m_pMagic->CreateAll())
			return false;
// 
// 		if (!LoadEquipment(Database()))
// 			return false;

		m_pOwner		= pUser->QueryLink();

		m_pPetData = NULL;

		if (IsEudemon())
		{
// 			CMsgUserAttrib msg;
// 			if (msg.Create(this->GetID(), _USERATTRIB_POTENTIAL, this->GetPotential()))
// 				this->SendMsg(&msg);
		}

		ST_CREATENEWNPC	info;
		memset(&info, 0, sizeof(ST_CREATENEWNPC));
		info.id				= GetID();
		info.usAction		= MSGAINPCINFO_CREATENEW;
		info.usType			= GetType();
		info.ucOwnerType	= /*m_pData->GetInt(PETDATA_OWNERTYPE)*/0;
		info.idOwner		= /*m_pData->GetInt(PETDATA_OWNERID)*/0;
		info.idMap			= GetMap()->GetID();
		info.usPosX			= GetPosX();
		info.usPosY			= GetPosY();
		info.idData			= /*m_pData->GetInt(PETDATA_GENID)*/0;
		info.nData			= /*GetData()*/0;
		CMsgMonsterInfo	msg;
		IF_OK(msg.Create(&info))
		this->SendMsg(&msg);

		return true;
	}
	return false;
}

bool CMonster::Create(PROCESS_ID idProcess,  const ST_CREATENEWNPC* pInfo,CUserPetData* pData,CEudemonTypeData* pTypeData,EudemonData eudemonData,CUser* pUser)
{
	CHECKF(pData && pInfo && pTypeData&& pUser);
	m_idProcess		= idProcess;
	m_idNpc			= pInfo->id;

	m_nDir			= /*::RandGet(8) + */1;
	if(m_nDir == 5)
		m_nDir ++;

	m_pMap			= MapManager()->GetGameMap(pInfo->idMap);
	m_nPosX			= pInfo->usPosX;
	m_nPosY			= pInfo->usPosY;

	m_nBornX		= m_nPosX;
	m_nBornY		= m_nPosY;

	m_i64Effect		= KEEPEFFECT_NORMAL;
	m_i64Effect2	= KEEPEFFECT_NORMAL;
	m_idGen         = 0;

	m_pType         = NULL;

	m_nCurrLife		= pData->GetInt(EUDEMONDATA_life);
	m_tFight.SetInterval(1200);
	m_tFight.Update();
	
	if(!m_pMap)
		return false;
	
	m_pBattleSystem	= new CBattleSystem(this->m_idProcess, this);
	CHECKF(m_pBattleSystem);

	m_setStatus = CStatusSet::CreateNew(true);
	CHECKF(m_setStatus);

	m_pMagic = CMagic::CreateNew(idProcess, this->QueryRole());
	CHECKF(m_pMagic);

	m_pPetData = pData;
	m_pPetTypeData = pTypeData;
	m_pOwner = pUser->QueryLink();
	memcpy(&m_eudemonData,&eudemonData,sizeof(EudemonData));
	InitEudemonMagic();
	InitGiftData();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::Create(PROCESS_ID idProcess, CNpcType* pType, const ST_CREATENEWNPC* pInfo, LPCTSTR pszName/*=NULL*/)
{
	m_idProcess		= idProcess;

	ClearPath();
	CHECKF(pType);

	m_idNpc			= pInfo->id;
	m_nDir			= /*::RandGet(8) +*/ 1;
	if(m_nDir == 5)
		m_nDir ++;
	if (pInfo->idMap==1048)
	{
		CUserPtr m_pUser=UserManager()->GetUser(pInfo->idOwner);
		CHECKF(m_pUser);
		m_pMap=m_pUser->GetSynMap();
	}
	else
		m_pMap			= MapManager()->GetGameMap(pInfo->idMap);
	m_nPosX			= pInfo->usPosX;
	m_nPosY			= pInfo->usPosY;

	m_nBornX		= m_nPosX;
	m_nBornY		= m_nPosY;

//	m_dwStatus		= STATUS_NORMAL;
	m_i64Effect		= KEEPEFFECT_NORMAL;
	m_i64Effect2	= KEEPEFFECT_NORMAL;

	m_idGen			= pInfo->idData;

	m_pType			= pType;
	m_nCityWarType  = pInfo->unCitywarType;

	m_nCurrLife		= m_pType->GetInt(NPCTYPEDATA_LIFE);
	m_nCurrMana		= m_pType->GetInt(NPCTYPEDATA_MANA);

	m_dwMoveInterTime = m_pType->GetInt( NPCTYPEDATA_RUNSPEED );
	m_tMove.SetInterval( m_dwMoveInterTime );	
	m_tMove.Update();

	m_tWander.SetInterval( m_dwMoveInterTime * 20 );	
	m_tWander.Update();

	m_tFight.SetInterval(1200);
	m_tFight.Update();

	if(!m_pMap)
		return false;

	m_pBattleSystem	= new CBattleSystem(this->m_idProcess, this);
	CHECKF(m_pBattleSystem);
	InitMagic();

	memset(&m_EudemonGifeData,0,sizeof(PETGIFTDATA));
	m_setStatus = CStatusSet::CreateNew(true);
	CHECKF(m_setStatus);

	m_pPetData = NULL;

	
// 	CMsgMonsterInfo	msg;
// 	IF_OK(msg.Create(pInfo))
// 		this->SendMsg(&msg);


// 	if(!IsCallPet() && !IsEudemon())	// synchro outside
// 	{
// 		CMsgMonsterInfo	msg;
// 		IF_OK(msg.Create(pInfo))
// 			this->SendMsg(&msg);
// 	}

// 	//add by xumiao 2009-12-14 ,���npc�����Ĺ���
// 	//������ɫ��0xfefeefef ���ֵ��ʾ���߿ͻ����յ������Ϣ���ù涨�ĺ�ɫ��ʾ�����ܾ���Ƶ��������Ӱ�죬��Ҫע�⡣
// 	int desType = m_pType->GetInt( NPCTYPEDATA_DESTYPE );
// 	
// 	if( desType > 0 )
// 	{
// 		char szParam[1024];
// 		sprintf(szParam, m_pType->GetStr( NPCTYPEDATA_DES ) , m_pMap->GetName() , m_nPosX , m_nPosY);
// 		
// 		CMsgTalk MapMsg;
// 		
// 		switch ( desType )
// 		{
// 		case _TXTATR_MAP: //���ͼƵ���з���
// 			MapMsg.Create( SYSTEM_NAME , ALLUSERS_NAME ,szParam,NULL,0xfefeefef,_TXTATR_MAP );
// 			UserManager()->BroadcastMapMsg( pInfo->idMap , &MapMsg,NULL);
// 			break;
// 		case _TXTATR_WORLD: //������Ƶ���з���
// 			UserManager()->BroadcastMsg(szParam, NULL, NULL, 0xfefeefef, _TXTATR_WORLD, NULL );
// 			break;
// 		}
// 	}
	return true;
}

void CMonster::MoveNext(int nMoveMode)
{
	if(!m_tMove.IsActive())
	{
		m_tMove.Startup(m_dwMoveInterTime);
	}
	else if( !m_tMove.ToNextTick( m_dwMoveInterTime ) )
	{
		return;
	}

	if(m_nPathLen <= 0)
		m_tMove.Clear();

	int nNewPosX = 0;
	int nNewPosY = 0;
	
	int nDir1	= 0;
	int nDir2	= 0;
	if(nMoveMode == MOVEMODE_WALK)
	{
		nDir1 = m_bufPath[0] % MAX_DIRSIZE;
		m_nPathLen--;
		if(m_nPathLen)
		{
			memcpy(m_bufPath, m_bufPath+1, m_nPathLen);
		}

		MoveToward(nDir1);

		CMsgWalkEx msg;
		IF_OK(msg.Create(GetID(), nDir1, 1, GetPosX(), GetPosY(),1234,1234))
		{
			BroadcastRoomMsg(&msg);
		}
		return;
	}

	nDir1 = m_bufPath[0] % MAX_DIRSIZE;
	m_nPathLen--;
	if(m_nPathLen)
	{
		memcpy(m_bufPath, m_bufPath+1, m_nPathLen);
	}
	
	MoveToward(nDir1);
	
	CMsgWalkEx msg;
	IF_OK(msg.Create(GetID(), nDir1, 2, GetPosX(), GetPosY(),1234,1234))
	{
		BroadcastRoomMsg(&msg);
	}

// 	if(m_nPathLen == 1)
// 	{
// 		nDir1 = m_bufPath[0] % MAX_DIRSIZE;
// 		m_nPathLen--;
// 		if(m_nPathLen)
// 		{
// 			memcpy(m_bufPath, m_bufPath+1, m_nPathLen);
// 		}
// 		MoveToward(nDir1);
// 	}
// 	else
// 	{
// 		nDir1 = m_bufPath[0] % MAX_DIRSIZE;
// 		nDir2 = m_bufPath[1] % MAX_DIRSIZE;
// 		m_nPathLen -= 2;
// 		if(m_nPathLen)
// 		{
// 			memcpy(m_bufPath, m_bufPath+2, m_nPathLen);
// 		}
// 		MoveToward(nDir1);
// 		MoveToward(nDir2);
// 	}
// 	
// 	if(m_nPathLen <= 0)
// 		m_tMove.Clear();
// 
// 	CMsgWalkEx msg;
// 	IF_OK(msg.Create(GetID(), nDir1, MOVEMODE_RUN_DIR0 + nDir2, GetPosX(), GetPosY(),1234,1234))
// 	{
// 		BroadcastRoomMsg(&msg);
// 	}
}

void CMonster::Run(int x,int y,bool CheckFloor)
{
	CHECK(GetMap());
	if(FindPath(x,y,CheckFloor))
	{
		//m_tMove.Startup(10);
		m_nMoveMode = MOVEMODE_RUN;
	}
}

void CMonster::Walk(int x,int y,bool CheckFloor)
{
	CHECK(GetMap());
	if(FindPath(x,y,CheckFloor))
	{
		//m_tMove.Startup(m_dwMoveInterTime);
		m_nMoveMode = MOVEMODE_WALK;
	}
}

void CMonster::Standby()
{
	CHECK(GetMap());
	ClearPath();
}

//////////////////////////////////////////////////////////////////////
void CMonster::OnTimer(time_t tCurr)
{
	//����AI
	if(!IsEudemon())
		UpdateAIStatus();

// 	if(!this->IsAlive() && !m_bLeaveMap)
// 	{
// 		if ((m_tDie.IsActive() && m_tDie.GetRemain() < NPCDIEDELAY_SECS/2)			// 
// 			&& !(m_i64Effect & KEEPEFFECT_DISAPPEARING)		// �Ѿ�������ʬ����ʧ״̬�������ظ�����
// 			)				
// 		{
// 			this->SetStatus(STATUS_DISAPPEARING);
// 		}
// 	}
	if(IsDeleted())
		return ;	// no timer

	// add life ------------------------------------------------------
// 	DEBUG_TRY
// 	if (IsAlive()  && m_tAddLife.IsActive() && m_tAddLife.ToNextTime(RECRUITMAGIC_SECS))
// 	{
// 		if(GetLife() < GetMaxLife())
// 		{
// 			int nAddLife = GetMaxLife() * GetRecruitMagicPercent() / 100;
// 			int nLoseLife = ::CutRange((int)GetMaxLife() - (int)GetLife(), (int)0, (int)GetMaxLife());
// 			nAddLife = ::CutOverflow(nAddLife, nLoseLife);
// 			AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
// 
// 			// synchro
// 			CMsgMagicEffect	msg;
// 			IF_OK(msg.Create(GetID(), SIMPLEMAGIC_RECRUIT, 0, GetID(), nAddLife, GetDir(),BATTLEADDINFO::INFO_INVALID))
// 				BroadcastRoomMsg(&msg, EXCLUDE_SELF);
// 
// 			m_nFightPause	+= 1000;		// delay
// 		}
// 		else
// 		{
// 			m_tAddLife.Clear();
// 		}
// 	}
// 	DEBUG_CATCH("CMonster add life");
	
	// ���������Զ��ظ� -----------------------------------------------
// 	DEBUG_TRY
// 	if (IsAlive() && IsEudemon() /*&& m_pEudemonItem*/)
// 	{
// 		if (!m_tIncLife.IsActive())
// 			m_tIncLife.Startup(AUTO_INC_LIFE_SECS);
// 		else
// 		{
// 			 if (m_tIncLife.ToNextTime() && GetLife() < GetMaxLife())
// 			 {
// 				 int nAddLife = 10/*GetMaxLife()*AUTO_INC_LIFE_PERCENT/100*/;
// 				 AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
// 			 }
// 		}
// 	}
// 	DEBUG_CATCH("CMonster increase life");

	// �Զ����� ------------------------------------------------------
	DEBUG_TRY
	if (m_pBattleSystem && m_pBattleSystem->IsActived() 
		&& m_tFight.ToNextTick(1200/* GetInterAtkRate()+ m_nFightPause*/)
		&& (!QueryMagic() || !QueryMagic()->IsActive()))
	{
		if(QueryStatus(STATUS_DREAMING) || QueryStatus(STATUS_PALSY) )
			return;

		m_pBattleSystem->ProcAttack_Hand2Hand();
		m_nFightPause	= 0;
	}
	DEBUG_CATCH("CMonster ProcAttack_Hand2Hand");

// 	if (m_pMagic)
// 	{
// 		DEBUG_TRY
// 		DEADLOOP_CHECK(PID, "QueryMagic()->OnTimer")
// 		m_pMagic->OnTimer(tCurr);
// 		DEBUG_CATCH("CMonster magic timer crash.")
// 	}

//06.9.27������ʱע�ʹ˴���,�˴���Ϊԭ����ҳ�����ش���---------begin-----------------------------
	// status --------------------------------------------------------
	DEBUG_TRY
/*	// �������Ӳ��ִ��������ж��Ƿ������ϵͳ״̬
	// bDetachTeamStatus = true ��ʾ��Ҫ�������������ص�״̬
	bool	bDetachTeamStatus = false;
	bool	bDetachAddExpStatus = false;		// �Ƿ���STATUS_ADD_EXP
	CTeam* pTeam = NULL;
	if (IsAlive() && m_pOwner && (pTeam = m_pOwner->GetTeam()))
	{
		if (pTeam->GetMemberAmount() < TEAM_STATUS_REQ_ROLES)
			bDetachTeamStatus = true;				

		CUser* pLeader = UserManager()->GetUser(pTeam->GetLeader());
		if (!pLeader || !pLeader->IsAlive()
			|| this->GetMap()->GetID() != pLeader->GetMap()->GetID()
			|| this->GetDistance(pLeader->QueryMapThing()) > _RANGE_TEAM_STATUS)
		{
			bDetachTeamStatus = true;
		}
		if (!pLeader || !pLeader->QueryStatus(STATUS_ADD_EXP))
			bDetachAddExpStatus = true;
	}
	else
	{
		bDetachTeamStatus	= true;
		bDetachAddExpStatus	= true;
	}*/
//-----------------------end-------------------------------------------------------------------------------	
	for(int i = QueryStatusSet()->GetAmount()-1; i >= 0; i--)
	{
		IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
		DEBUG_TRY
		if(pStatus)
		{
			if (!pStatus->OnTimer(tCurr))
				break;
			if(!pStatus->IsValid()
				/*|| (bDetachAddExpStatus && pStatus->GetID() == STATUS_ADD_EXP)*/)//������ʱע�ʹ˾�
			{
			//	QueryStatusSet()->DelObj(pStatus->GetID());
				DetachStatus( this->QueryRole(),pStatus->GetID() );
			}
		}
		DEBUG_CATCH2("QueryStatusSet()->GetObjByIndex(i) %u",pStatus->GetID());
	}
	DEBUG_CATCH("monster magic poison")


	// ���� -----------------------------------------------------
	DEBUG_TRY
	if((IsCallPet() || IsEudemon()) && m_pOwner == NULL)
	{
		if(!IsDeleted())
		{
			LOGERROR("Call pet don't find owner! del self.");
			DelMonster(DEL_NOW);
		}
	}

	//����δɾ
	if(IsMonster() && !IsAlive())
	{
		if(!IsDeleted())
		{
			DelMonster(DEL_NOW);
		}
	}

	if(IsEudemon()&& !IsDeleted())
	{
		DEBUG_TRY
		CheckEudeHappy();
		DEBUG_CATCH("CUser CheckEudeHappy();")
	}
	DEBUG_CATCH("RoleManager()->QuerySet()->DelObj")
}

//////////////////////////////////////////////////////////////////////
void CMonster::ClearAllStatus()
{
	for(int i = QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
		if(pStatus)
//			QueryStatusSet()->DelObj(pStatus->GetID());
		{
			CRole::DetachStatus( this->QueryRole(),pStatus->GetID() );
		}
	}
}

//////////////////////////////////////////////////////////////////////
/*
int	 CMonster::AdjustData(int nData, int nAdjust, int nMaxData/ *=0* /)
{
	if(nAdjust>=ADJUST_PERCENT)
		return MulDiv(nData, nAdjust-ADJUST_PERCENT, 100);

	if(nAdjust<=ADJUST_SET)
		return -1*nAdjust + ADJUST_SET;

	if(nAdjust==ADJUST_FULL)
	{
		ASSERT(nMaxData != 0);
		return nMaxData;
	}

	return nData + nAdjust;
}
*/

//////////////////////////////////////////////////////////////////////
bool CMonster::SendMsg(CNetMsg* pMsg)
{
	if ((this->IsEudemon() || this->IsCallPet()) && m_pOwner)
		return m_pOwner->SendMsg(pMsg);
	else
		return true;//MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(m_idNpc, pMsg);
}

//////////////////////////////////////////////////////////////////////
void CMonster::BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf /*= false*/,EM_SENDNPCFLAG sendNpc/* = EM_SENDNPCFLAG_TRUE*/)
{
	CRole::BroadcastRoomMsg(pMsg, bSendSelf,sendNpc);
}

//////////////////////////////////////////////////////////////////////
void CMonster::BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf /*= false*/)		
{
	CMsgTalk	msg;
	if(msg.Create(GetName(), ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
		this->BroadcastRoomMsg(&msg, bSendSelf);
}

//////////////////////////////////////////////////////////////////////
void CMonster::BroadcastMapMsg(CNetMsg* pMsg, bool bSendSelf /*= false*/)		
{ 
	CMapPtr pMap = this->GetMap();
	if(pMap)
		UserManager()->BroadcastMapMsg(pMap->GetID(), pMsg);
	if(bSendSelf)
	{
		if ((IsCallPet() || IsEudemon()) && m_pOwner)
			m_pOwner->SendMsg(pMsg);
		else
			return;
	//		MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, pMsg);
	}
}

//////////////////////////////////////////////////////////////////////
void CMonster::BroadcastMapMsg(LPCTSTR szMsg, bool bSendSelf /*= false*/)		
{
	CMsgTalk	msg;
	if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
		this->BroadcastMapMsg(&msg, bSendSelf);
}

//////////////////////////////////////////////////////////////////////
bool CMonster::SendSelfToBlock()
{
	// brocast my info
	CMsgPlayer msg;
	if (msg.Create((IRole*)this))
		this->BroadcastRoomMsg(&msg);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::SendLeaveFromBlock()
{
	if(!m_bLeaveMap)
	{
		m_bLeaveMap = true;

		CMsgAction	msg;
		if(msg.Create(GetID(), 0, 0, 0, actionLeaveMap))
			this->BroadcastRoomMsg(&msg, INCLUDE_SELF);

		if (this->IsEudemon() && m_pOwner)
			m_pOwner->SendMsg(&msg);
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// role
/////////////////////////////////////////////////////////////////////////////
void CMonster::SendShow(IRole* pRole)
{
	CHECK(pRole);

	if(IsDeleted())
		return;

// 	if(IsEudemon())
// 	{
// 		CMsgEudemonInfo msg;
// 		if(msg.Create(this))
// 			pRole->SendMsg(&msg);
// 	}
// 	else 
// 	{	
		CMsgPlayer	msg;
		if(msg.Create((IRole*)this))
			pRole->SendMsg(&msg);

		for(int i =  this->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
		{
			IStatus* pStatus =  this->QueryStatusSet()->GetObjByIndex(i);
			if( pStatus)
			{
				CMsgBuff msgBuff;
				if(msgBuff.Create( ACTION_ADDSTATUS,this->GetID(),pStatus->GetID(),pStatus->GetSecLeft(),0,0,0,0,0 ))
					pRole->SendMsg(&msgBuff);
			}
		}
//	}
}

//////////////////////////////////////////////////////////////////////
bool CMonster::SendSysMsg(const char* fmt, ...)
{ 
	MSGBUF	szMsg;
    vsprintf( szMsg, fmt, (char*) ((&fmt)+1) );

	CMsgTalk msg;
	if (msg.Create(SYSTEM_NAME, GetName(), szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
		return this->SendMsg(&msg);

	return false;
}
//////////////////////////////////////////////////////////////////////
bool CMonster::SendSysMsg(int nChannel, const char* fmt, ...)
{ 
	MSGBUF	szMsg;
    vsprintf( szMsg, fmt, (char*) ((&fmt)+1) );
	
	CMsgTalk msg;
	if (msg.Create(SYSTEM_NAME, GetName(), szMsg, NULL, 0x00ffff00, nChannel))
		return this->SendMsg(&msg);

	return false;
}
//////////////////////////////////////////////////////////////////////
void CMonster::AddLife(int nLifeAdd, int nSynchro/*=false*/)
{
	if(IsEudemon())
	{
		if(!m_pPetData)
			return;
		int nMaxLife = GetMaxLife();
		int nCurLife = GetLife() ;
		nCurLife += nLifeAdd;
		if(nCurLife > nMaxLife)
		{			
			nCurLife = nMaxLife;
		}

		if(nCurLife <= 0)
		{
			nCurLife = 0;
		}

		if(nCurLife != GetLife())
		{
			m_pPetData->SetInt(EUDEMONDATA_life,nCurLife);
		}
		return;
	}

	if (this->GetLife() + nLifeAdd <= 0)
	{
		m_nCurrLife	= 0;
	}
	else
		m_nCurrLife	= this->GetLife() + nLifeAdd;
}
//////////////////////////////////////////////////////////////////////
// modify attrib
//////////////////////////////////////////////////////////////////////
bool CMonster::AddAttrib(int idxAttr, __int64 i64Data, int nSynchro)
{
	CMsgUserAttrib	msg;
	if(!msg.Create(GetID(), _USERATTRIB_NONE, _USERATTRIB_NONE))			// _USERATTRIB_NONE : �����
		return false;	

	if(i64Data)
	{
		switch(idxAttr)
		{
		case	_USERATTRIB_LIFE:
			{
				AddLife((int)i64Data, nSynchro);
//				if (this->IsEudemon())
//				{
//					IF_OK (m_pEudemonItem)
//					{
//						int nCurrLife = ::CutTrail(0, m_pEudemonItem->GetInt(ITEMDATA_EUDEMON_LIFE)+(int)i64Data);
//						int nMaxLife = GetMaxLife();
//						nCurrLife = ::CutOverflow(nCurrLife, nMaxLife);
//						m_pEudemonItem->SetInt(ITEMDATA_EUDEMON_LIFE, nCurrLife);
//
//						CMsgItemAttrib	msgItemAttrib;
//						if (msgItemAttrib.Create(m_pEudemonItem->GetID(), _ITEMATTRIB_EUDEMON_LIFE, m_pEudemonItem->GetInt(ITEMDATA_EUDEMON_LIFE)))
//							m_pOwner->SendMsg(&msgItemAttrib);
//					}
//				}
//				else
//				{
//					if ((int)this->GetLife() + i64Data <= 0)
//					{
////						this->SetStatus(STATUS_DIE);
//						m_nCurrLife	= 0;
//					}
//					else
//						m_nCurrLife	= this->GetLife() + i64Data;
//				}
				// Ѫ����70%��ʱ������״̬
//				if (GetLife()*100/GetMaxLife() > MAX_FRENZY_LIFE_PERCENT)
//				{
//					CRole::DetachStatus(this->QueryRole(), STATUS_FRENZY);
//				}

				IF_NOT (msg.Append(_USERATTRIB_LIFE, GetLife()))
					return false;

				
				if(IsEudemon())
				{
					CMsgEudemonAttrib msgAttr;
					if(!msgAttr.Create(GetID(), EUDATTR_LIFE, GetLife()))		
						return false;
					this->SendMsg(&msgAttr);
				}
			}
			break;
		case	_USERATTRIB_POTENTIAL:
			{
				if (this->IsEudemon())
				{
					m_nPotential = __max(0, __min(m_nPotential + (int)i64Data, MAX_EUDEMON_POTENTIAL));
					IF_NOT (msg.Append(_USERATTRIB_POTENTIAL, m_nPotential))
						return false;
				}
			}
			break;
		case	_USERATTRIB_MANA:
			{
//				m_nCurrMana	= (__max(GetMana() + i64Data, 0));
//				IF_NOT (msg.Append(_USERATTRIB_MANA, GetMana()))
//					return false;
				if(IsEudemon())
				{
					int curMana = 0;
					if(GetMana() + (int)i64Data > 0 )
					{
						curMana = GetMana() + (int)i64Data;
					}
					if(curMana > GetMaxMana())
					{
						curMana = GetMaxMana();
					}
						
// 					CMsgEudemonAttrib msgAttr;
// 					if(!msgAttr.Create(GetID(), EUDATTR_MANA, GetMana()))		
// 						return false;
// 					this->SendMsg(&msgAttr);
				}
			}
			break;
		case	_USERATTRIB_EXP:
			{
//				IF_NOT (msg.Append(_USERATTRIB_EXP, GetExp()))
//					return false;
				if(IsEudemon())
				{
					AwardExp(i64Data);
				}
			}
			break;
		case	_USERATTRIB_MONEY:
			{
//				IF_NOT (msg.Append(_USERATTRIB_MONEY, GetMoney()))
					return false;
			}
			break;
		default:
			return false;
		}
	}

	//20070316����:����ͬ�����Ѫ���ı�--------------------------
	//ֻҪ�����Ѫ��.������nSynchro.ֱ�ӷ���㲥
	if(idxAttr == _USERATTRIB_LIFE && nSynchro != SYNCHRO_FALSE )// �½�����ӣ��㲥����Ѫ�������һ�����ܴ�ֲ���ѪBUG [6/21/2007]
	{
		if(IsEudemon())
			this->BroadcastRoomMsg(&msg, INCLUDE_SELF,EM_SENDNPCFLAG_FALSE);
		else
			this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		return false;
	}
	//-----------------------------------------------------------


	if(nSynchro != SYNCHRO_FALSE)
	{
		switch(nSynchro)
		{
		case SYNCHRO_TRUE:
			this->SendMsg(&msg);
			break;

		case SYNCHRO_BROADCAST:
			this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			break;

		default:
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMonster::ProcessAfterMove()
{
}

//////////////////////////////////////////////////////////////////////
void CMonster::ProcessOnMove(int nMoveMode)
{
	// stop fight
	if(nMoveMode != MOVEMODE_JUMPMAGICATTCK && nMoveMode != MOVEMODE_COLLIDE && nMoveMode != MOVEMODE_SYNCHRO)
		ClrAttackTarget();

	m_tFootPrint = ::TimeGet();
	m_posFootPrint.x = this->GetPosX();
	m_posFootPrint.y = this->GetPosY();
}

//////////////////////////////////////////////////////////////////////
void CMonster::GetFootPrint	(int& nPosX, int& nPosY)
{
	if (::TimeGet()-m_tFootPrint >= TIME_FOOTPRINT)
	{	// time out
		nPosX = this->GetPosX();
		nPosY = this->GetPosY();
	}
	else
	{
		nPosX = m_posFootPrint.x;
		nPosY = m_posFootPrint.y;
	}
}

bool CMonster::InitGiftData()
{
	memset(&m_EudemonGifeData,0,sizeof(PETGIFTDATA));
	if(!m_pPetData)
		return false;

	m_EudemonGifeData.m_dwGiftAddStr += ProcGift1001(DOTYPE_addstr,m_eudemonData.dwStr,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddSta += ProcGift1001(DOTYPE_addsta,m_eudemonData.dwSta,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddSpi += ProcGift1001(DOTYPE_addspi,m_eudemonData.dwSpi,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddInt += ProcGift1001(DOTYPE_addint,m_eudemonData.dwInt,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddAgi += ProcGift1001(DOTYPE_addagi,m_eudemonData.dwAgi,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddhp = ProcGift1001(DOTYPE_addhp,m_eudemonData.dwMaxLife,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddcrihit = ProcGift1001(DOTYPE_addcrihit,m_eudemonData.dwCrit,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddphyatt = ProcGift1001(DOTYPE_addphyatt,m_eudemonData.dwPhyAtt,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddmagatt = ProcGift1001(DOTYPE_addmagatt,m_eudemonData.dwMagAtt,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddphydef = ProcGift1001(DOTYPE_addphydef,m_eudemonData.dwPhyDef,valueMask_CONST) + ProcGift1001(DOTYPE_addPANDMDEF,0,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddmagdef = ProcGift1001(DOTYPE_addmagdef,m_eudemonData.dwMagDef,valueMask_CONST) + ProcGift1001(DOTYPE_addPANDMDEF,0,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddhit = ProcGift1001(DOTYPE_addhit,m_eudemonData.dwhit,valueMask_CONST) + ProcGift1001(DOTYPE_addHANDD,0,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAdddodge = ProcGift1001(DOTYPE_adddodge,m_eudemonData.dwDoge,valueMask_CONST) + ProcGift1001(DOTYPE_addHANDD,0,valueMask_CONST);
	m_EudemonGifeData.m_dwGiftAddTough = ProcGift1001(DOTYPE_addTough,m_eudemonData.dwTough,valueMask_CONST);
	m_EudemonGifeData.m_GiftAtt1 = ProcGift1001(DOTYPE_addAtt1,0,valueMask_CONST) + ProcGift1001(DOTYPE_addAllAtt,0,valueMask_CONST);
	m_EudemonGifeData.m_GiftAtt2 = ProcGift1001(DOTYPE_addAtt2,0,valueMask_CONST) + ProcGift1001(DOTYPE_addAllAtt,0,valueMask_CONST);
	m_EudemonGifeData.m_GiftAtt3 = ProcGift1001(DOTYPE_addAtt3,0,valueMask_CONST) + ProcGift1001(DOTYPE_addAllAtt,0,valueMask_CONST);
	m_EudemonGifeData.m_GiftAtt4 = ProcGift1001(DOTYPE_addAtt4,0,valueMask_CONST) + ProcGift1001(DOTYPE_addAllAtt,0,valueMask_CONST);
	m_EudemonGifeData.m_GiftDef1 = ProcGift1001(DOTYPE_addDef1,0,valueMask_CONST) + ProcGift1001(DOTYPE_addAllDef,0,valueMask_CONST);
	m_EudemonGifeData.m_GiftDef2 = ProcGift1001(DOTYPE_addDef2,0,valueMask_CONST) + ProcGift1001(DOTYPE_addAllDef,0,valueMask_CONST);
	m_EudemonGifeData.m_GiftDef3 = ProcGift1001(DOTYPE_addDef3,0,valueMask_CONST) + ProcGift1001(DOTYPE_addAllDef,0,valueMask_CONST);
	m_EudemonGifeData.m_GiftDef4 = ProcGift1001(DOTYPE_addDef4,0,valueMask_CONST) + ProcGift1001(DOTYPE_addAllDef,0,valueMask_CONST);


// 	DWORD thisGrow = m_eudemonData.initgrow + m_eudemonData.grow;
// 	m_eudemonData.dwStr = (m_eudemonData.firstr * (thisGrow / 100.0f * m_eudemonData.level * 0.1f + 1)) / 100 + m_eudemonData.stradd + m_EudemonGifeData.m_dwGiftAddStr;
// 	m_eudemonData.dwInt = (m_eudemonData.firint * (thisGrow / 100.0f * m_eudemonData.level * 0.1f + 1)) / 100 + m_eudemonData.intadd + m_EudemonGifeData.m_dwGiftAddInt;
// 	m_eudemonData.dwSta = (m_eudemonData.firsta * (thisGrow / 100.0f * m_eudemonData.level * 0.1f + 1)) / 100 + m_eudemonData.staadd + m_EudemonGifeData.m_dwGiftAddSta;
// 	m_eudemonData.dwSpi = (m_eudemonData.firspi * (thisGrow / 100.0f * m_eudemonData.level * 0.1f + 1)) / 100 + m_eudemonData.spiadd + m_EudemonGifeData.m_dwGiftAddSpi;
// 	m_eudemonData.dwAgi = (m_eudemonData.firagi * (thisGrow / 100.0f * m_eudemonData.level * 0.1f + 1)) / 100 + m_eudemonData.agiadd + m_EudemonGifeData.m_dwGiftAddAgi;
// 	
// 		
// 	m_eudemonData.dwMaxLife = (DWORD)((m_eudemonData.dwStr *  m_eudemonData.dwStrChg * 2.0f + m_eudemonData.dwSta *  m_eudemonData.dwStaChg * 6.0f) * 7.5f / 1000.0f) + m_EudemonGifeData.m_dwGiftAddhp;
// 	m_eudemonData.dwPhyAtt = (DWORD)(m_eudemonData.dwStr *  m_eudemonData.dwStrChg * 10.0f * 1.2f / 1000.0f);
// 	m_eudemonData.dwMagAtt = (DWORD)(m_eudemonData.dwInt *  m_eudemonData.dwIntChg * 10.0f * 1.2f / 1000.0f);
// 	m_eudemonData.dwPhyDef = (DWORD)((m_eudemonData.dwStr *  m_eudemonData.dwStrChg * 2.0f + m_eudemonData.dwSta *  m_eudemonData.dwStaChg * 3.0f + m_eudemonData.dwSpi *  m_eudemonData.dwSpiChg * 5.0f +m_eudemonData.dwSta *  m_eudemonData.dwStaChg * 2.0f) * 1.2f / 1000.0f);
// 	m_eudemonData.dwMagDef = (DWORD)((m_eudemonData.dwInt *  m_eudemonData.dwIntChg * 2.0f + m_eudemonData.dwSta *  m_eudemonData.dwStaChg * 3.0f + m_eudemonData.dwSpi *  m_eudemonData.dwSpiChg * 5.0f +m_eudemonData.dwSta *  m_eudemonData.dwStaChg * 2.0f) * 1.2f / 1000.0f);
// 	m_eudemonData.dwhit = (DWORD)(m_eudemonData.dwSta *  m_eudemonData.dwStaChg * 8.0f * 0.3f / 1000.0f);
// 	m_eudemonData.dwDoge = (DWORD)((m_eudemonData.dwSta *  m_eudemonData.dwStaChg * 2.0f + m_eudemonData.dwSpi *  m_eudemonData.dwSpiChg * 2.0f +m_eudemonData.dwSta *  m_eudemonData.dwStaChg * 4.0f) * 0.15f / 1000.0f);
// 	m_eudemonData.dwCrit = (DWORD)((m_eudemonData.dwAgi *  m_eudemonData.dwAgiChg * 0.05f + 10.0f) / 1000.0f);
// 	m_eudemonData.dwTough = (DWORD)(20 + m_eudemonData.level / 10);
	return true;
}

//////////////////////////////////////////////////////////////////////
// interactive
//////////////////////////////////////////////////////////////////////
bool CMonster::Freeze(int nPower)								// return false: ��Ѩ���ɹ�
{
	if(IsAlive())
	{
		if(nPower > 0)
		{
			return false;								// ����ʾ����״̬
		}
		else
		{
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// fight
//////////////////////////////////////////////////////////////////////
bool CMonster::SetAttackTarget(IRole* pTarget /*= NULL*/)
{
	if(pTarget == NULL)
	{
		m_pBattleSystem->ResetBattle();
		return true;
	}

	//-------���---2007/05/22-----�����Ժ�-�ùֺ������ڰ�ȫ���ﶼ����ɱ���---begin-----------
/*
	if(pTarget->QueryOwnerUser() && GetMap()->IsInRegionType(REGION_PK_PROTECTED, GetPosX(), GetPosY()))
		return false;
*/
	//---------------------end----------------------------
	
	if(this->GetDistance(pTarget->QueryMapThing()) > GetAttackRange(pTarget->GetSizeAdd()))
		return false;

	m_pBattleSystem->CreateBattle(pTarget->GetID());
	m_idTarget = pTarget->GetID();
	return true;
}

//////////////////////////////////////////////////////////////////////
//06.9.27
//�����޸�
//�߻�Ҫ�����ʹ�ü���,�˺�������֧��
bool CMonster::AutoSkillAttack(IRole* pTarget)
{
	CHECKF(pTarget);

	if (m_pMagic && m_pMagic->AutoAttackTarget(pTarget))
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////
int CMonster::GetDieMode()
{
// 	CHECKF(m_pMagicType);
// 
// 	if (m_pMagicType->GetInt(MAGICTYPEDATA_WEAPON_HIT) == CMagic::HIT_BY_MAGIC)
// 		return DIE_MAGIC;
// 	else if (IsBowman())
// 		return DIE_ARROWSHOOT;
// 	else
		return DIE_NORMAL;
}
///////////////////////////////////////////////////////////////////////
//Time	:06.9.27													///
//Author:����														///
//About	:����ĵ㼼�ܹ�������										///
///////////////////////////////////////////////////////////////////////
void CMonster::MonsterMagic_PointAttack( IRole* pTarget )
{
}
///////////////////////////////////////////////////////////////////////
//Time	:06.9.27													///
//Author:����														///
//About	:����ķ�Χ���ܹ�������(�Թ�������ΪԲ��,һ���뾶��Բ����)	///
///////////////////////////////////////////////////////////////////////	
void CMonster::MonsterMagic_CircleAttack()
{
}

//////////////////////////////////////////////////////////////////////
// precondition is dodge failed
int CMonster::Attack( IRole* pTarget,DWORD& dwBattleAddInfoRef )		// return : lose life
{
	CHECKF(pTarget);

	bool bNeedOverPVP = false;
	ATK_TYPE type = ATKTYPE_PHYSICAL_COMMON;
	int nDamage = CBattleSystem::CalcDamage( dwBattleAddInfoRef,type,this->QueryRole(),pTarget,0,0 );
	if (GetType() > 8000 && GetType() < 8011)
	{
		if (GetLife() < GetMaxLife()*0.1)
			nDamage*=2;
	}
	int nLoseLife = ::CutOverflow(nDamage, (int)pTarget->GetLife());

	CMonster* pMonster = NULL;
	if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
		if(pMonster->IsCityWarStrategicTarget())   //���﹥��ս��Ŀ����һ��Ѫ
		{
			nLoseLife = ::CutOverflow(nDamage, (int)(pTarget->GetLife()-1));
		}
	}

	if (IsEudemon() && pTarget->IsUser())
	{
		CUser* pTargetUser	= NULL;
		pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser));
		if (pTargetUser)
		{
			if(MapGroup(PID)->GetPVPManger()->findPVP(pTargetUser->GetID()))
			{
				if((pTargetUser->GetLife() - nLoseLife) <= 0)
					bNeedOverPVP = true;
				nLoseLife = pTargetUser->PVPEmendDamage(nLoseLife, m_pOwner->GetID(), false);
			}
			
		}
	}
	if (nLoseLife > 0)
		pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLoseLife, SYNCHRO_TRUE);
	
	pTarget->BeAttack(BY_WEAPON, QueryRole(), nLoseLife);

	if(IsEudemon() && m_pOwner)
		m_pOwner->CheckCrime(pTarget);

	if (bNeedOverPVP)
	{
		if (QueryOwnerUser())
			MapGroup(PID)->GetPVPManger()->EndPVP(pTarget->GetID(), QueryOwnerUser()->GetID(), true);
	}

// 	
// 	CGameMap* pMap = this->GetMap();
// 	if (pMap && m_pOwner)
// 	{
// 		// crime
// 		if (m_pOwner->CheckCrime(pTarget)
// 			&& !pTarget->IsEvil()				// ֻ��pk����������
// 			&& !pMap->IsDeadIsland())	// ������ɱ�˲�����
// 		{
// 			if (!m_pOwner->QueryStatus(STATUS_CRIME))
// 				m_pOwner->SetCrimeStatus();
// 		}
// 	}

	return nDamage;
}

int CMonster::AdjustGift(IRole* pTarget,int nType,int nOldData,int valueMask)
{
// 	if(!IsEudemon())
// 		return 0;
// 	int dataIndex = -1;
// 	switch(nType)
// 	{
// 	case EUDEMONGIFT_DAMAGE:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_addphydem;
// 		}
// 		break;
// 	case EUDEMONGIFT_REDAMAGE:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_redphyatt;
// 		}
// 		break;
// 	case EUDEMONGIFT_MAXLIFE:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_addhp;
// 		}
// 		break;
// 	case EUDEMONGIFT_MAXMANA:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_addmp;
// 		}
// 		break;
// 	case EUDEMONGIFT_ATT:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_addphyatt;
// 		}
// 		break;
// 	case EUDEMONGIFT_MAGATT:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_addmagatt;
// 		}
// 		break;
// 	case EUDEMONGIFT_DEF:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_addphydef;
// 		}
// 		break;
// 	case EUDEMONGIFT_MAGDEF:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_addmagdef;
// 		}
// 		break;
// 	case EUDEMONGIFT_CRIHIT:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_addcrihit;
// 		}
// 		break;
// 	case EUDEMONGIFT_HIT:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_addhit;
// 		}
// 		break;
// 	case EUDEMONGIFT_DODGE:
// 		{
// 			dataIndex = EM_GIFTTEMPLATE_1007_adddodge;
// 		}
// 		break;
// 	}
// 
// 	int data = 0;
// 	OBJID giftTmpType = 0 ;
// 	CGiftTreeData* pGiftTreeData = NULL;
// 
// 	OBJID nIdentity = GetIdentity1();
// 	CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(nIdentity);
// 	if(pMagicData)
// 	{
// 		int nTargetType = pMagicData->GetInt(EM_EUDEMONMAGIC_type);
// 		if(nTargetType & 0x00000040)//�츳
// 		{
// 			nIdentity = pMagicData->GetInt(EM_EUDEMONMAGIC_dataID);
// 			if(nIdentity)
// 			{
// 				giftTmpType = 0 ;
// 				CGiftTreeData* pGiftTreeData = g_pGiftManager->GetGiftData(CGiftManager::PeelType(nIdentity),CGiftManager::PeelLev(nIdentity));
// 				if(pGiftTreeData)
// 				{
// 					giftTmpType = pGiftTreeData->GetInt(GIFTTREEDATA_temType);
// 					if(giftTmpType == 1007)
// 					{
// 						IRecord* pIdentity = g_pGiftManager->GetGiftTemplete(nIdentity);
// 						if(pIdentity)
// 							data += ProcGift1007(pIdentity,pTarget,dataIndex,nOldData,valueMask_CONST);
// 					}
// 				}
// 			}
// 		}
// 	}
// 	
// 	
// 	nIdentity = GetIdentity2();	
// 	pMagicData = g_pNpcMagicManager->QueryEudemonData(nIdentity);
// 	if(pMagicData)
// 	{
// 		int nTargetType = pMagicData->GetInt(EM_EUDEMONMAGIC_type);
// 		if(nTargetType & 0x00000040)//�츳
// 		{
// 			nIdentity = pMagicData->GetInt(EM_EUDEMONMAGIC_dataID);
// 			if(nIdentity)
// 			{
// 				giftTmpType = 0 ;
// 				CGiftTreeData* pGiftTreeData = g_pGiftManager->GetGiftData(CGiftManager::PeelType(nIdentity),CGiftManager::PeelLev(nIdentity));
// 				if(pGiftTreeData)
// 				{
// 					giftTmpType = pGiftTreeData->GetInt(GIFTTREEDATA_temType);
// 					if(giftTmpType == 1007)
// 					{
// 						IRecord* pIdentity = g_pGiftManager->GetGiftTemplete(nIdentity);
// 						if(pIdentity)
// 							data += ProcGift1007(pIdentity,pTarget,dataIndex,nOldData,valueMask_CONST);
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	nIdentity = GetIdentity3();	
// 	pMagicData = g_pNpcMagicManager->QueryEudemonData(nIdentity);
// 	if(pMagicData)
// 	{
// 		int nTargetType = pMagicData->GetInt(EM_EUDEMONMAGIC_type);
// 		if(nTargetType & 0x00000040)//�츳
// 		{
// 			nIdentity = pMagicData->GetInt(EM_EUDEMONMAGIC_dataID);
// 			if(nIdentity)
// 			{
// 				giftTmpType = 0 ;
// 				CGiftTreeData* pGiftTreeData = g_pGiftManager->GetGiftData(CGiftManager::PeelType(nIdentity),CGiftManager::PeelLev(nIdentity));
// 				if(pGiftTreeData)
// 				{
// 					giftTmpType = pGiftTreeData->GetInt(GIFTTREEDATA_temType);
// 					if(giftTmpType == 1007)
// 					{
// 						IRecord* pIdentity = g_pGiftManager->GetGiftTemplete(nIdentity);
// 						if(pIdentity)
// 							data += ProcGift1007(pIdentity,pTarget,dataIndex,nOldData,valueMask_CONST);
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	nIdentity = GetIdentity4();	
// 	pMagicData = g_pNpcMagicManager->QueryEudemonData(nIdentity);
// 	if(pMagicData)
// 	{
// 		int nTargetType = pMagicData->GetInt(EM_EUDEMONMAGIC_type);
// 		if(nTargetType & 0x00000040)//�츳
// 		{
// 			nIdentity = pMagicData->GetInt(EM_EUDEMONMAGIC_dataID);
// 			if(nIdentity)
// 			{
// 				giftTmpType = 0 ;
// 				CGiftTreeData* pGiftTreeData = g_pGiftManager->GetGiftData(CGiftManager::PeelType(nIdentity),CGiftManager::PeelLev(nIdentity));
// 				if(pGiftTreeData)
// 				{
// 					giftTmpType = pGiftTreeData->GetInt(GIFTTREEDATA_temType);
// 					if(giftTmpType == 1007)
// 					{
// 						IRecord* pIdentity = g_pGiftManager->GetGiftTemplete(nIdentity);
// 						if(pIdentity)
// 							data += ProcGift1007(pIdentity,pTarget,dataIndex,nOldData,valueMask_CONST);
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	nIdentity = GetIdentity5();	
// 	pMagicData = g_pNpcMagicManager->QueryEudemonData(nIdentity);
// 	if(pMagicData)
// 	{
// 		int nTargetType = pMagicData->GetInt(EM_EUDEMONMAGIC_type);
// 		if(nTargetType & 0x00000040)//�츳
// 		{
// 			nIdentity = pMagicData->GetInt(EM_EUDEMONMAGIC_dataID);
// 			if(nIdentity)
// 			{
// 				giftTmpType = 0 ;
// 				CGiftTreeData* pGiftTreeData = g_pGiftManager->GetGiftData(CGiftManager::PeelType(nIdentity),CGiftManager::PeelLev(nIdentity));
// 				if(pGiftTreeData)
// 				{
// 					giftTmpType = pGiftTreeData->GetInt(GIFTTREEDATA_temType);
// 					if(giftTmpType == 1007)
// 					{
// 						IRecord* pIdentity = g_pGiftManager->GetGiftTemplete(nIdentity);
// 						if(pIdentity)
// 							data += ProcGift1007(pIdentity,pTarget,dataIndex,nOldData,valueMask_CONST);
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	nIdentity = GetIdentity6();	
// 	pMagicData = g_pNpcMagicManager->QueryEudemonData(nIdentity);
// 	if(pMagicData)
// 	{
// 		int nTargetType = pMagicData->GetInt(EM_EUDEMONMAGIC_type);
// 		if(nTargetType & 0x00000040)//�츳
// 		{
// 			nIdentity = pMagicData->GetInt(EM_EUDEMONMAGIC_dataID);
// 			if(nIdentity)
// 			{
// 				giftTmpType = 0 ;
// 				CGiftTreeData* pGiftTreeData = g_pGiftManager->GetGiftData(CGiftManager::PeelType(nIdentity),CGiftManager::PeelLev(nIdentity));
// 				if(pGiftTreeData)
// 				{
// 					giftTmpType = pGiftTreeData->GetInt(GIFTTREEDATA_temType);
// 					if(giftTmpType == 1007)
// 					{
// 						IRecord* pIdentity = g_pGiftManager->GetGiftTemplete(nIdentity);
// 						if(pIdentity)
// 							data += ProcGift1007(pIdentity,pTarget,dataIndex,nOldData,valueMask_CONST);
// 					}
// 				}
// 			}
// 		}
// 	}

	return nOldData;
}

int CMonster::ProcGift1001(int nType, int oldValue,int valueMask /*= valueMask_ALL*/)
{	
	if(nType > DOTYPE_petAddMaxLife || nType < DOTYPE_addhp)
		return 0;
	
	int nAdd = 0;
	for(int i = 0 ; i < m_pMagic->QuerySet()->GetAmount() ;  i++)
	{
		IMagicData* pData = m_pMagic->QuerySet()->GetObjByIndex(i);
		if(!pData)
			continue;
		CMagicTypeData* pType = CMagic::FindMagicType(pData->GetInt(MAGICDATA_TYPE)/*, pData->GetInt(MAGICDATA_LEVEL)*/);
		if(!pType)
			continue;
		if(pType->GetInt(MAGICTYPEDATA_SORT) != 99)
			continue;
		
		CGiftTreeData* pTreeData = g_pGiftManager->QueryGiftTreeSet()->GetObj(pData->GetInt(MAGICDATA_TYPE));
		if(!pTreeData)
			continue;
		if(pTreeData->GetInt(GIFTTREEDATA_temType) != 1001)
			continue;
		
		IRecord* pGiftTmpRecord = g_pGiftManager->GetGiftTemplete(pData->GetInt(MAGICDATA_TYPE));
		
		if(!pGiftTmpRecord)
			continue;
		
		//��ЩûӰ��
		if(pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dotype) != nType)
			continue;
		
		//���������ж�
		int doValue = pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dodata) + pGiftTmpRecord->GetInt(EM_NEWGIFTTEMPLATE_1001_dodatainc) *  (pData->GetInt(MAGICDATA_LEVEL) - 1) / 100;
		if(doValue >= 31000)
			doValue = 31000;
		
		if(doValue >= 30000)
		{
			nAdd += oldValue * ((doValue - 30000) / 100.0f);
		}
		else
		{
			nAdd += doValue;
		}
	}
	
	return nAdd;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::BeAttack(bool bMagic, IRole* pTarget, int nPower,int addi/* = 0*/, bool bReflectEnable/*=true*/)
{
	CUser* pTargetUser = NULL;
	
	if (!m_tAddLife.IsActive())
		m_tAddLife.Startup(RECRUITMAGIC_SECS);
	
	if (QueryStatus(STATUS_PALSY))
		CRole::DetachStatus(this->QueryRole(), STATUS_PALSY);
	if (GetType() > 8000 && GetType() < 8011)//����֣����⴦��
	{
		int lift=GetMaxLife();
		int nowlift=GetLife();
		if (m_count1==0)
		{
			if (GetLife() < GetMaxLife()*0.66)
			{
				RunScript("action/tiaozhuan.lua");//��ת����
				m_count1++;
			}
		}
		if (m_count2==0)
		{
			if (GetLife() < GetMaxLife()*0.33)
			{
				RunScript("action/tiaozhuan.lua");//��ת����
				m_count2++;
			}
		}
	}

	//goto սʿ����ʱ��޼�50%
	if (pTarget->IsUser() && (pTarget->GetProfession() & PROF_SOLDIER))
		nPower*=1.5;
	if (pTarget->IsUser() && pTarget->GetSynID()!=ID_NONE)
		m_nSynId=pTarget->GetSynID();
	//���ų�޵ĸı��ת������Ŀ��.��npc��AI���־���.
	//������msg����.
	AddEnmitySynNPC(pTarget->GetID(),nPower,0,addi);

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMonster::Kill(IRole* pTarget, DWORD dwDieWay)
{
	CHECK(pTarget);
	
	CMonster* pMonster = NULL;
	bool bRet = pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));

	if(IsEudemon() && m_pOwner && pMonster && bRet)
	{
// 		int type = pMonster->GetType();
// 		int add = type;
// 		if(add > 3000)
// 		{
// 			add = 3000 + ((type - 3001 ) / 30) * 30;
// 		}
// 		m_pOwner->UpdateMissionMsgByMonsterType(add );
//		m_pOwner->ProcessAction(20000 + add);

		CMsgInteract msg;
		if(msg.Create(INTERACT_KILL, this->GetID(), pTarget->GetID(), pTarget->GetPosX(), pTarget->GetPosY(), dwDieWay,0))
			pTarget->BroadcastRoomMsg(&msg, true,EM_SENDNPCFLAG_ENFORCE);
		CUser* pUserTemp;
		if(pTarget->IsUser() && this->QueryOwnerUser())
		{
			CUser* pUser= this->QueryOwnerUser();
			if(pTarget->QueryObj(OBJ_USER, IPP_OF(pUserTemp)))
			{
				if(pUserTemp->IsPVPStuts() && pUserTemp->GetPVPTargetID() != pUser->GetID())
				{
					if(MapGroup(PID)->GetPVPManger()->findPVP(pUser->GetID()))
						MapGroup(PID)->GetPVPManger()->EndPVP(pUser->GetID(),pUserTemp->GetPVPTargetID(), true);
// 					pUserTemp->SetAttrib(_USERATTRIB_LIFE,1,true);
// 					pUserTemp->DetachBadlyStatus(pUserTemp);
// 					pUser->DetachBadlyStatus(pUser);
// 					CRole::AttachStatus(pUserTemp->QueryRole(), STATUS_PK_PROTECT, 0, 3);	
// 					return;
				}
			}
		}

		//m_pOwner->Kill(pTarget,dwDieWay);
		pTarget->BeKill((IRole*)m_pOwner);

		return;
	}
	
	if (m_pOwner)
	{
		CUser* pUserTemp = NULL;
		CUser* pUser= this->QueryOwnerUser();
		if(pTarget->IsUser() && pUser)
		{
			IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
			IStatus* pStatusTarget = pTarget->QueryStatus(STATUS_WARGAME);

			if(pStatus && pStatusTarget && pStatus->GetPower()==pStatusTarget->GetPower())
				WargameManager()->OnPlayerKill(pUser, pTarget, pStatus->GetPower());
			else if(pTarget->QueryObj(OBJ_USER, IPP_OF(pUserTemp)))
			{
				if(pUser->IsPVPStuts() && pUser->GetPVPTargetID() == pUserTemp->GetID())
				{
					if(MapGroup(PID)->GetPVPManger()->findPVP(pUser->GetID()))
						MapGroup(PID)->GetPVPManger()->EndPVP(GetID(),pUserTemp->GetID(), true);
					pUserTemp->SetAttrib(_USERATTRIB_LIFE,1,true);
					pUserTemp->DetachBadlyStatus(pUserTemp);
					pUser->DetachBadlyStatus(pUser);
					CRole::AttachStatus(pUserTemp->QueryRole(), STATUS_PK_PROTECT, 0, 3);	
					return;
				}
			}
		}
	}
	CMsgInteract msg;
	if(msg.Create(INTERACT_KILL, this->GetID(), pTarget->GetID(), pTarget->GetPosX(), pTarget->GetPosY(), dwDieWay,0))
		pTarget->BroadcastRoomMsg(&msg, true,EM_SENDNPCFLAG_ENFORCE);

#ifdef	LOCAL_DEBUG
	//CMonster* pMonster;
	if(/*pTarget->QueryObj(OBJ_NPC, IPP_OF(*/pMonster/*))*/ && bRet)
		LOGERROR("����[%s][%d]��[%d,%d]ɱ����[%s][%d]", GetName(), GetType(), 
						GetPosX(), GetPosY(), pMonster->GetName(), pMonster->GetType());
#endif
	
	if(m_pOwner)
	{
		CUser*	pTargetUser=NULL;
		if(pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
			m_pOwner->ProcessPk(pTargetUser);
	}

//	if (QueryStatus(STATUS_FRENZY2))
//		++m_nKillNum;

	//// ���޼��ϵ��ע����  KXJ
//	if (this->IsEudemon())
//	{
//		++m_nKillNum4Potential;
//		if (m_nKillNum4Potential >= ADD_POTENTIAL_KILLNUM)
//		{
//			m_nKillNum4Potential = 0;
//			AddAttrib(_USERATTRIB_POTENTIAL, ADD_POTENTIAL_PER_KILLNUM, SYNCHRO_TRUE);
//		}
//
//		++m_nKillNum4RelationShip;
//		if (m_nKillNum4RelationShip >= ADD_RELATIONSHIP_KILLNUM)
//		{
//			// TODO: ����/���ٹ�ϵֵ
//			if (QueryOwnerUser())
//			{
//				UCHAR setDivine[10];
//				memset(setDivine, 0, sizeof(setDivine));
//				for (int i=0; i<m_pOwner->GetEudemonAmount(); i++)
//				{
//					CMonster* pEudemon = m_pOwner->QueryEudemonByIndex(i);
//					if (pEudemon && pEudemon->GetID() != this->GetID())
//					{
//						OBJID idDivine = pEudemon->GetDivineID();
//						++setDivine[idDivine];
//					}
//				}
//				for (i=1; i<=8; i++)
//				{
//					if (setDivine[i] > 0)
//					{
//						if (::RandGet(1000) < 10)
//							this->AddRelationShip(i, ADD_RELATIONSHIP_PER_TIME);
//					}
//					else
//					{
//						if (::RandGet(1000) < 5)
//							this->AddRelationShip(i, -1*ADD_RELATIONSHIP_PER_TIME);
//					}
//				}
//			}
//		}
//	}

	pTarget->BeKill((IRole*)this);
}

//////////////////////////////////////////////////////////////////////
void CMonster::BeKill(IRole* pRole /*= NULL*/)
{
	if(IsDeleted())
		return ;

	DEBUG_TRY
	slotOnMonsterBeKill(GetID());///֪ͨ�����¼���������ֻ������	
	if(IsEudemon())
	{
		if(!m_pPetData)
			return;
		int nHappy = m_pPetData->GetInt(EUDEMONDATA_happy);
		m_pPetData->SetInt(EUDEMONDATA_happy,max(nHappy - 5 , 0));
		m_pPetData->SetInt(EUDEMONDATA_life,0,true);
		m_pOwner->CallBackEudemon(this->GetID(),true,true,false);
		return;
	}
	DEBUG_CATCH("CMonster::BeKill 1");

	
	bool bAddBoon = true;
	DEBUG_TRY
	if( bAddBoon )//�Ӿ��飬�ӻ�ֵ
	{
		OBJID idMapItemOwner = GetMaxEnmityAndActiveUserID(); //ӵ����
		CUser* pActionUser = UserManager()->GetUser(idMapItemOwner);
		if(pActionUser)
		{
			DEBUG_TRY
			if(GetKilledAction() != ID_NONE)
			{
				pActionUser->ProcessAction(GetKilledAction());
			}
			DEBUG_CATCH("ProcessAction(GetKilledAction())");
// 			DEBUG_TRY
// 			int type = GetType();
// 			int add = type;
// 			if(add > 3000)
// 			{
// 				add = 3000 + ((type - 3001 ) / 30) * 30;
// 			}
// 			pActionUser->ProcessAction(20000 + add);
// 
// 			DEBUG_CATCH("ProcessAction(20000 + add)");

			this->AllotExp2User(pActionUser);
			this->AllotItem2User(pActionUser);
			DEBUG_TRY
			this->AllotTutorPreach(pActionUser);
			DEBUG_CATCH("CMonster::BeKill this->AllotTutorPreach(pActionUser)");
			
			DEBUG_TRY
			this->AllotKill2User(pActionUser);
			DEBUG_CATCH("this->AllotKill2User(pActionUser)");
		}
		this->m_vEnmity.clear();
	}
	DelMonster();
	DEBUG_CATCH("CMonster::BeKill 2");

	DEBUG_TRY
//modify code by �ֵ�Ԩ begin 2011.6.22
		int objid = this->GetType();
	if( objid > 6300 && objid <= 6330 && this->QueryMagic() )
		this->QueryMagic()->ProcessBuffBomb(this,4);
//modify code by �ֵ�Ԩ end
// 	if(IsInsteadMagic() && m_pOwner)
// 	{
// 		m_pOwner->BeKill();		// BeKill(pRole)
// 	}
	DEBUG_CATCH("instead kill!")

//	CityWar process Action delete All role
//		CUser* pCityWarUser = NULL;
//		if(pRole)
//		  pCityWarUser = pRole->QueryOwnerUser();	
//		if(bAddBoon)
//		{
//			int nAction = m_pType->GetInt(NPCTYPEDATA_ACTION);
//			if ( pCityWarUser && (nAction == 160||nAction==161))
//			{
//				GameAction()->ProcessAction(nAction, pCityWarUser, this);
//			}
//		}
}
//////////////////////////////////////////////////////////////////////
void CMonster::BeKillByDot(OBJID idAtker)//���ﱻdot�����Ĵ���,Ŀ����Ϊ�˽�����ﱻdot������������񲻼���������
{
	CUser* pAtker = UserManager()->GetUser(idAtker);
	if (pAtker)
		pAtker->Kill(this, DIE_NORMAL);
}

//�������������ҷ�����Ʒ��Ǯ
void CMonster::AllotItem2User(CUserPtr pGetUser)
{
	vector<CUserPtr> vTempUser;
	vector<CMapItemPtr> vTempMapItem;
	vTempUser.push_back(pGetUser);
	
	CTeam* pTempTeam = pGetUser->GetTeam();	
	if( pTempTeam )
	{
		int nTeamAmount = pTempTeam->GetMemberAmount();
		for(int i = 0; i < nTeamAmount; i++)
		{
			OBJID idMember = pTempTeam->GetMemberByIndex(i);
			CUser* pTempUser = NULL;
			this->FindAroundObj( OBJ_USER,idMember,( void** )&pTempUser );
			if(pTempUser && (pTempUser->GetID() != pGetUser->GetID()))
			{
				vTempUser.push_back(pTempUser);
			}
		}
	}

	
	if(m_pType->GetInt(NPCTYPEDATA_BOSSTYPE) > 0) //��Աȫ��
	{
		if (m_pType->GetInt(NPCTYPEDATA_BOSSTYPE) == 10)//����boss���⴦��
		{
			CSyndicate* pSyn;
			pSyn = pGetUser->GetSyndicate();
			if(!pSyn)
				return;
			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
				return;
			pSyn->GetMemberList(vTempUser);
			for(int  i = 0;i < vTempUser.size();i++ )
			{
				CUser* pTempUser = NULL;
				CGameMap *pSynMap = vTempUser[i]->GetSynMap();
				if (pSynMap!=NULL)
				{
					if (vTempUser[i]->GetMapID()==pSynMap->GetID())
					{
						pTempUser=vTempUser[i];
					}
					if(pTempUser)
					{
						CMapItem* pItem =  CMapItem::CreateNew();
						if(!pItem)
							return;
						if(!pItem->Create(MapManager()->SpawnMapItemID(),pTempUser->GetSynMap(),GetPosX(),GetPosY(),pTempUser,PID,m_pType->GetInt(NPCTYPEDATA_BOSSTYPE),GetName()))
							return;
						vTempMapItem.push_back(pItem);
					}
				}
				
			}
		}
		else
		{
			for(int  i = 0;i < vTempUser.size();i++ )
			{
				CUser* pTempUser = NULL;
				this->FindAroundObj( OBJ_USER,vTempUser[i]->GetID(),( void** )&pTempUser );
				if(pTempUser)
				{
					CMapItem* pItem =  CMapItem::CreateNew();
					if(!pItem)
						return;
					if(!pItem->Create(MapManager()->SpawnMapItemID(),m_pMap,GetPosX(),GetPosY(),pTempUser,PID,m_pType->GetInt(NPCTYPEDATA_BOSSTYPE),GetName()))
						return;
					vTempMapItem.push_back(pItem);
				}
			}
		}
	}
	else //���ѡһ��
	{
		int n = RandGet(vTempUser.size());
		CUser* pTempUser = NULL;
		this->FindAroundObj( OBJ_USER,vTempUser[n]->GetID(),( void** )&pTempUser );
		if(pTempUser)
		{
			CMapItem* pItem =  CMapItem::CreateNew();
			if(!pItem)
				return;
			if(!pItem->Create(MapManager()->SpawnMapItemID(),m_pMap,GetPosX(),GetPosY(),vTempUser[n],PID,m_pType->GetInt(NPCTYPEDATA_BOSSTYPE),GetName()))
				return;
			vTempMapItem.push_back(pItem);
		}
	}

	//bool bSend = false;
// 	if (::RandGet(10000) < m_pType->GetInt(NPCTYPEDATA_DROP_MONEY_CHANCE))
// 	{ 
// 		DWORD dwMoneyMin = m_pType->GetInt(NPCTYPEDATA_DROPMONEY_MIN);
// 		DWORD dwMoneyMax = m_pType->GetInt(NPCTYPEDATA_DROPMONEY_MAX);
// 		DWORD dwMoney = dwMoneyMin + ::RandGet(dwMoneyMax - dwMoneyMin + 1);
// 
// 		for(int  i = 0;i < vTempMapItem.size();i++ )
// 		{
// 			vTempMapItem[i]->AddItem(dwMoney);
// 		}
// 		bSend = true;
// 	}


	for(int  j = 0;j < vTempMapItem.size();j++ )
	{
		vector<int> vRule;
		CDropRuleGroup* pDropRuleGroup = NULL;
		if(m_pType->GetInt(NPCTYPEDATA_DROP_ITEM_RULE1) != ID_NONE)
			pDropRuleGroup = DropRuleMap()->GetObjByIndex(m_pType->GetInt(NPCTYPEDATA_DROP_ITEM_RULE1));
		if(pDropRuleGroup)
		{
			pDropRuleGroup->GetDropItem(&vRule);
			for (int i=0; i<vRule.size(); i++)
			{
				OBJID	idItemType = vRule[i];
				

				CItemTypeData* pItemType = ItemType()->QueryItemType( idItemType );
				if(!pItemType)
					continue;
					
				if(idItemType / 100000 == 7 && pItemType->GetInt(ITEMTYPEDATA_REQ_USERLEVEL) > vTempMapItem[j]->GetOwner()->GetLev())
					continue;
					
				vTempMapItem[j]->AddItem(idItemType);
			}
		}

		pDropRuleGroup = NULL;
		vRule.clear();
		if(m_pType->GetInt(NPCTYPEDATA_DROP_ITEM_RULE2) != ID_NONE)
			pDropRuleGroup = DropRuleMap()->GetObjByIndex(m_pType->GetInt(NPCTYPEDATA_DROP_ITEM_RULE2));
		if(pDropRuleGroup)
		{
			pDropRuleGroup->GetDropItem(&vRule);
			for (int i=0; i<vRule.size(); i++)
			{
				OBJID	idItemType = vRule[i];
				
				
				CItemTypeData* pItemType = ItemType()->QueryItemType( idItemType );
				if(!pItemType)
					continue;
				
				if(idItemType / 100000 == 7 && pItemType->GetInt(ITEMTYPEDATA_REQ_USERLEVEL) > vTempMapItem[j]->GetOwner()->GetLev())
					continue;
				
				vTempMapItem[j]->AddItem(idItemType);
			}
		}

		pDropRuleGroup = NULL;
		vRule.clear();
		if(m_pType->GetInt(NPCTYPEDATA_DROP_ITEM_RULE3) != ID_NONE)
			pDropRuleGroup = DropRuleMap()->GetObjByIndex(m_pType->GetInt(NPCTYPEDATA_DROP_ITEM_RULE3));
		if(pDropRuleGroup)
		{
			pDropRuleGroup->GetDropItem(&vRule);
			for (int i=0; i<vRule.size(); i++)
			{
				OBJID	idItemType = vRule[i];
				
				
				CItemTypeData* pItemType = ItemType()->QueryItemType( idItemType );
				if(!pItemType)
					continue;
				
				if(idItemType / 100000 == 7 && pItemType->GetInt(ITEMTYPEDATA_REQ_USERLEVEL) > vTempMapItem[j]->GetOwner()->GetLev())
					continue;
				
				vTempMapItem[j]->AddItem(idItemType);
			}
		}

		pDropRuleGroup = NULL;
		vRule.clear();
		if(m_pType->GetInt(NPCTYPEDATA_DROP_ITEM_RULE4) != ID_NONE)
			pDropRuleGroup = DropRuleMap()->GetObjByIndex(m_pType->GetInt(NPCTYPEDATA_DROP_ITEM_RULE4));
		if(pDropRuleGroup)
		{
			pDropRuleGroup->GetDropItem(&vRule);
			for (int i=0; i<vRule.size(); i++)
			{
				OBJID	idItemType = vRule[i];
				
				
				CItemTypeData* pItemType = ItemType()->QueryItemType( idItemType );
				if(!pItemType)
					continue;
				
				if(idItemType / 100000 == 7 && pItemType->GetInt(ITEMTYPEDATA_REQ_USERLEVEL) > vTempMapItem[j]->GetOwner()->GetLev())
					continue;
				
				vTempMapItem[j]->AddItem(idItemType);
			}
		}
	}
	
	for(int  i = 0;i < vTempMapItem.size();i++ )
	{
		if(vTempMapItem[i]->GetItemAmount() < 1)
		{
			vTempMapItem[i]->ReleaseByOwner();
			vTempMapItem[i] = NULL;
		}
		else
		{
			MapManager()->QueryMapItemSet()->AddObj(vTempMapItem[i]);
			vTempMapItem[i]->SendShowFirst();
		}
		
	}
	vTempUser.clear();
}

//�������������ҷ��侭��
void CMonster::AllotExp2User(CUserPtr pGetUser)
{
	DEBUG_TRY
	//�������
	int nMonsterExp = this->GetExp();
	if(nMonsterExp <= 0)
		return;
	vector<CUserPtr> vTempUser;
	vTempUser.push_back(pGetUser);

	CTeam* pTempTeam = pGetUser->GetTeam();	
	if( pTempTeam )
	{
		int nTeamAmount = pTempTeam->GetMemberAmount();
		for(int i = 0; i < nTeamAmount; i++)
		{
			OBJID idMember = pTempTeam->GetMemberByIndex(i);
			CUser* pTempUser = NULL;
			this->FindAroundObj( OBJ_USER,idMember,( void** )&pTempUser );
			if(pTempUser && pTempUser->IsAlive() && (pTempUser->GetID() != pGetUser->GetID()))
			{
				vTempUser.push_back(pTempUser);
			}
 		}

		nMonsterExp = pTempTeam->GetTeamAddExp(nMonsterExp, vTempUser.size());
		nMonsterExp /= vTempUser.size();
		if(nMonsterExp <= 0)
		{
			vTempUser.clear();
			return;
		}
	}

	for(int  i = 0;i < vTempUser.size();i++ )
	{
		int nExpUaer = 0;
		int nExpPet = 0;
		CUserPtr pTempUser = vTempUser[ i ];
		nExpUaer = pTempUser->AdjustExp(this->QueryRole(),nMonsterExp);

		

		nExpUaer = pTempUser->AwardBattleExp( nExpUaer,false,false );

		

		char szTemp1[128];
		char szTemp2[128];
		if(nExpUaer > 0)
		{
			sprintf(szTemp1,"������%u����",nExpUaer);
		}

		if(pTempUser->m_pWarPet)
		{
			nExpPet = pTempUser->AdjustEudemonExp(this->QueryRole(),pTempUser->m_pWarPet,nMonsterExp);
			nExpPet = pTempUser->AwardEudemonExp( nExpPet);

			if(nExpPet > 0)
			{
				if(nExpUaer > 0)
				{
					sprintf(szTemp2,"�����%s���%u����",pTempUser->m_pWarPet->GetName(),nExpPet);
					strcat(szTemp1,szTemp2);
					pTempUser->SendSysMsg(_TXTATR_HELP,szTemp1);
				}
				else
				{
					sprintf(szTemp2,"���%s���%u����",pTempUser->m_pWarPet->GetName(),nExpPet);
					pTempUser->SendSysMsg(_TXTATR_HELP,szTemp2);
				}
			}
		}

		if(nExpPet <= 0 && nExpUaer>0)
			pTempUser->SendSysMsg(_TXTATR_HELP,szTemp1);
	}

	vTempUser.clear();
	DEBUG_CATCH( "void CMonster::AllotExp2User() Catch!" )
}

/////////////////////////////////////////////////////////////////////
void CMonster::DelMonster(bool bNow/*=false*/)			// call this mast !IsDeleted()
{
	if(IsDeleted())		// ȥ���˶���
		return;
	if (!m_bDecRole)
	{
		m_bDecRole	= true;
		GetMap()->DecRole(this->GetPosX(), this->GetPosY());
	}

	if(m_pPetData)
	{
		m_pPetData->SaveData();
		m_pPetData = NULL;
		m_pPetTypeData = NULL;
	}

	if(bNow)
	{
		m_tDie.Startup(0);
		SendLeaveFromBlock();
		ClrBroadcastSet();
	}
	else
		m_tDie.Startup(NPCDIEDELAY_SECS);
}

//////////////////////////////////////////////////////////////////////
//  [7/10/2007] ���񲨵�һ���޸�
//	���Ӳ�����DWORD dwBattleAddInfo	ս��������Ϣ
void CMonster::SendDamageMsg( OBJID idTarget, int nDamage ,DWORD dwBattleAddInfo )
{
	if(IsSimpleMagicAtk())
	{
		CMsgMagicEffect	msg;
		IF_OK(msg.Create(GetID(), /*m_pType->GetInt(NPCTYPEDATA_MAGIC_TYPE)*/0, 0, idTarget, nDamage, GetDir(),dwBattleAddInfo))
			BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
	else if(IsBowman())
	{
		CMsgInteract msg;
		IF_OK(msg.Create2(INTERACT_SHOOT, GetID(), idTarget, GetPosX(), GetPosY(), nDamage,dwBattleAddInfo))
			BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
	else
	{
		CMsgInteract msg;		
		IF_OK(msg.Create2(INTERACT_ATTACK, GetID(), idTarget, GetPosX(), GetPosY(), nDamage,dwBattleAddInfo))
			BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
}

////////////////////////////////////////////////////////////////////////////////////
DWORD CMonster::GetDropMoneyAvg()
{
	DWORD dwMoneyMin = m_pType->GetInt(NPCTYPEDATA_DROPMONEY_MIN);
	DWORD dwMoneyMax = m_pType->GetInt(NPCTYPEDATA_DROPMONEY_MAX);
	return (dwMoneyMin+dwMoneyMax)/2;
}

////////////////////////////////////////////////////////////////////////////////////
// drop
////////////////////////////////////////////////////////////////////////////////////
bool CMonster::DropMoney(DWORD dwMoney, 
						 OBJID idOwner,
						 OBJID teamID/*=ID_NONE*/)//20070206�����¼Ӳ���
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////
bool CMonster::DropItem(OBJID idItemType, 
						OBJID idOwner, 
						int nMagic2, 
						int nMagic3, 
						int nUserLuck,
						bool bIsAllowDropUnident,
						OBJID teamID/*=ID_NONE*/)//20070206�����¼Ӳ���
{
	return true;
// 	ItemInfoStruct InfoType;
// 	if(!ItemType()->GetInfo(idItemType,&InfoType))
// 		return false;
// 	POINT	pos;
// 	pos.x	= GetPosX();
// 	pos.y	= GetPosY();
// 	
// 	if(GetMap()->FindDropItemCell(MONSTERDROPITEM_RANGE, &pos))
// 	{
// 		CMapItem* pMapItem = CMapItem::CreateNew();
// 		if(pMapItem)
// 		{
// 			char owName[_MAX_NAMESIZE] = "";
// 			CUser* pUser = UserManager()->GetUser(idOwner);
// 			if(pUser)
// 			{
// 				StrCpy(owName,pUser->GetName());
// 			}
// 			if(pMapItem->Create(MapManager()->SpawnMapItemID(),GetMap(), pos, &InfoType, idOwner,owName))
// 			{
// 				MapManager()->QueryMapItemSet()->AddObj(pMapItem);
// 				return true;
// 			}
// 			else
// 				pMapItem->ReleaseByOwner();
// 		}
// 	}
// 	return false;
}


////////////////////////////////////////////////////////////////////////////////////
//bool CMonster::ProcessAction(CUser* pUser, OBJID idAction, LPCTSTR pszAccept)
//{
//	if (idAction == ID_NONE)
//		return false;
//
//	try {
//		// process action...
//		const _MAX_ACTIONPRC	=32;
//		DWORD dwActionCount	=0;
//		while (idAction != ID_NONE)
//		{
//			CActionData* pAction	= ActionSet()->GetObj(idAction);
//			if (!pAction)
//			{
//				::LogSave("Error: game action %u not found.", idAction);
//				break;
//			}
//
//			// process action now!
//			idAction = pAction->GetInt(ACTIONDATA_IDNEXTFAIL);
//
//			int nActionType = pAction->GetInt(ACTIONDATA_TYPE);
//			switch(nActionType)
//			{
//			case ACTION_RAND:
//				{
//					// �������ʡ�"data1 data2"��"10 100"��ʾ��1/10�Ļ�����true��
//					DWORD dwChance = 0, dwMax = 0;
//					if (2 == sscanf(pAction->GetStr(ACTIONDATA_PARAM), "%u %u", &dwChance, &dwMax))
//					{
//						if (::RandGet(dwMax) < dwChance)
//							idAction	= pAction->GetInt(ACTIONDATA_IDNEXT);
//					}
//					else
//						LOGERROR("ACTION %u: ����Ĳ�������", pAction->GetID());
//				}
//				break;
//
//			case ACTION_RANDACTION:
//				{
//					// �������ʡ�"action0 .... action7"
//					OBJID idRndAction[8];
//					if (8 == sscanf(pAction->GetStr(ACTIONDATA_PARAM), "%u %u %u %u %u %u %u %u", 
//						&idRndAction[0], &idRndAction[1], &idRndAction[2], &idRndAction[3],
//						&idRndAction[4], &idRndAction[5], &idRndAction[6], &idRndAction[7]))
//					{
//						idAction	= idRndAction[::RandGet(8)];
//					}
//					else
//						LOGERROR("ACTION %u: ����Ĳ�������", pAction->GetID());
//				}
//				break;
//
//			case ACTION_MST_DROPITEM:
//				{
//					// monster��ɱ�������Ʒ����Ǯ, param "dropitem itemtype"����"dropmoney money [LowPercent]"
//					char szCmd[MAX_PARAMSIZE] = "";					
//					if (1 != sscanf(pAction->GetStr(ACTIONDATA_PARAM), "%s", szCmd))
//					{
//						LOGERROR("ACTION %u: ����Ĳ�������", pAction->GetID());
//						break;
//					}
//
//					if (0 == stricmp(szCmd, "dropitem"))
//					{
//						DWORD dwData = 0;
//						if (2 == sscanf(pAction->GetStr(ACTIONDATA_PARAM), "%s %u", szCmd, &dwData))
//						{
//							if (0 == stricmp(szCmd, "dropitem"))
//							{
//								OBJID idUser = ID_NONE;
//								if (pUser)
//									idUser = pUser->GetID();
//
//								if (this->DropItem(dwData, idUser))
//									idAction = pAction->GetInt(ACTIONDATA_IDNEXT);
//							}
//						}
//						else
//						{
//							LOGERROR("ACTION %u: ����Ĳ�������", pAction->GetID());
//							break;
//						}
//					}
//					else if (0 == stricmp(szCmd, "dropmoney"))
//					{
//						DWORD dwData=0, dwLowPercent=0;
//						int rval = sscanf(pAction->GetStr(ACTIONDATA_PARAM), "%s %u %u", szCmd, &dwData, &dwLowPercent);
//						if (rval == 2)
//							dwLowPercent = 100;
//						else if (rval == 3)
//							dwLowPercent %= 100;
//						else
//						{
//							LOGERROR("ACTION %u: ����Ĳ�������", pAction->GetID());
//							break;
//						}
//						
//						DWORD dwMoneyDrop = dwData*(dwLowPercent+::RandGet(100-dwLowPercent))/100;
//						if (dwMoneyDrop <= 0)
//							return true;
//
//						OBJID idUser = ID_NONE;
//						if (pUser)
//							idUser = pUser->GetID();
//
//						if (this->DropMoney(dwMoneyDrop, idUser))
//							idAction	= pAction->GetInt(ACTIONDATA_IDNEXT);
//					}
//				}
//				break;
//
//			default:
//				break;
//			}
//
//			if (dwActionCount++ >= _MAX_ACTIONPRC)
//			{
//				::LogSave("Error: too many game action, last action:%u.", idAction);
//				break;
//			}
//		} // while
//	}
//	catch(...)
//	{
//		ASSERT(!"catch");
//		::LogSave("Error: exception catched in CUser::ProcessAction(%u)", idAction);
//	}
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////
bool CMonster::IsAtkable(IRole* pTarget, bool bSendHint)
{
	if (!pTarget)
		return false;

	if (!this->IsAlive())
	{
		if (bSendHint)
			this->SendSysMsg(STR_DIE_FORGIVE);
		return false;
	}


	if (!pTarget->IsAlive())
	{
		//if (bSendHint)
		//	this->SendSysMsg(STR_DIE_STOP_ATTACK);
		return false;
	}	


	if(IsEudemon() && m_pOwner)
	{
		return m_pOwner->IsAtkable(pTarget,bSendHint);
	}

//modify code by �ֵ�Ԩ begin 2011.6.22
	int objid = GetType();
	if( objid > 6200 && objid <= 6230 )
		return true;
//modify code by �ֵ�Ԩ end

	if(pTarget->IsMonster())
	{
		return false;
	}
	CNpc* pNpc;
	pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));	
	if(pNpc)
		return false;
	return true;

	/*
	if (pTarget->IsGM())
	{
		if (bSendHint)
			this->SendSysMsg(STR_ATTACK_POLICE);
		return false;
	}
	*/

//	if(this->IsCat())
//		return false;

	if(this->IsNonAttack())
		return false;
	
	if(GetID() == pTarget->GetID())
		return false;

	CMonster* pMonster = NULL;
	if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
		if(GetType() == pMonster->GetType())
			return false;
//		if(pMonster->IsCat())
//			return false;
		if(pMonster->IsNonBeAttack())
			return false;
	}

	if(!pTarget->IsBeAtkable())
		return false;

// 	if (!pTarget->QueryOwnerUser())
// 	{		
// 		return false;
// 	}
// 
// 	if (!pTarget->QueryOwnerUser())
// 	{		
// 		return false;
// 	}

	// �ٻ��޲��ܹ����Լ�������
	if (this->QueryOwnerUser() && this->QueryOwnerUser()->GetID() == pTarget->GetID())
		return false;
	// ��ͨ���ﲻ����ͨ��
	if(this->IsMonster() && pTarget->IsMonster() /*&& !this->IsGuard()*/)
		return false;

// 	if (!(this->GetMap() && pTarget->GetMap() && this->GetMap() == pTarget->GetMap()))
// 		return false;
// 
// 	//  [2007-8-31] ��� �Ƿ�Ϊͬһս���
// 	IStatus* pStatus1 = this->QueryStatus(STATUS_WARGAME);
// 	IStatus* pStatus2 = pTarget->QueryStatus(STATUS_WARGAME);
// 
// 	if(pStatus1 != NULL && pStatus2 == NULL)	//	����û�вμӻ�����
// 		return false;
// 
// 	else if(pStatus1 != NULL && pStatus2 != NULL)
// 	{
// 		if(pStatus1->GetPower() != pStatus2->GetPower())	//	����μӻ����ͬ�����
// 			return false;
// 	}
// 
// 	CUser* pUser = NULL;
// 
// 	if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
// 	{
// 		if(pUser->GetCityWarDoMain() && CITYSYNWAR_CITYWAR == pUser->GetCitySynWarType())
// 		{
// 			if(pUser->IsCityWarAttack(pUser->GetCityWarDoMain()) && this->IsCityAttack())
// 			{
// 				return false;
// 			}
// 			else if(pUser->IsCityWarDefence(pUser->GetCityWarDoMain()) && this->IsCityDefence())
// 			{
// 				return false;
// 			}
// 		}
// 
// 		if(0 != pUser->GetSynWarJoinID() && CITYSYNWAR_SYNWAR == pUser->GetCitySynWarType())
// 		{
// 			if((SYNWARTYPE_ATTACK == pUser->GetSynWarJoinType() && this->IsCityAttack())
// 				|| (SYNWARTYPE_DEFENCE == pUser->GetSynWarJoinType() && this->IsCityDefence()))
// 			{
// 				return false;
// 			}
// 		}
// 	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::IsBeAtkable()
{
	return true;
}

void CMonster::SetEudemonData(CUserPetData* pData)
{

}

//////////////////////////////////////////////////////////////////////
bool CMonster::EnterMapGroup()
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
	
//	MapGroup(PID)->QueryIntraMsg()->SetNpcProcessID(GetID(), true);			// ͬ��SOCKET_KERNEL

	
	pMap->EnterRoom(this->QueryMapThing(), WITH_BLOCK);

	m_bLeaveMap = false;

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::LeaveMapGroup()
{
	int nStep = 1;
	DEBUG_TRY
	CMapPtr pMap = this->GetMap();
	CHECKF(pMap);


	nStep =2;
//	MapGroup(PID)->QueryIntraMsg()->SetNpcProcessID(GetID(), false);			// ͬ��SOCKET_KERNEL

	// BeKill��ʱ�������DecRole�����������Ҫ�жϣ�����LeaveRoom��ʱ����һ�ν�ɫ��
	bool bWithBlock = !m_bDecRole;
	
	nStep = 3;
	SendLeaveFromBlock();	// NPC��������Ҫ���֪ͨ��ɾ������

	nStep = 4;
	DEBUG_TRY
	ClrBroadcastSet();
	DEBUG_CATCH("CMonster::LeaveMapGroup")
	
	nStep = 5;
	pMap->LeaveRoom(this->QueryMapThing(), bWithBlock);
	m_bDecRole	= true;

	DEBUG_CATCH2("CMonster::LeaveMapGroup() %u",nStep);
	return true;
}

//////////////////////////////////////////////////////////////////////
DWORD CMonster::GetLife()
{
	if(IsEudemon())
	{
		if(!m_pPetData)
			return 0;
		m_nCurrLife = m_pPetData->GetInt(EUDEMONDATA_life);
	}

	int nMaxLife = this->GetMaxLife();
	if (m_nCurrLife > nMaxLife)
		m_nCurrLife = nMaxLife;
	return m_nCurrLife;
}

//////////////////////////////////////////////////////////////////////
//void CMonster::SetLife(int nLifeData, BOOL bUpdate)
//{// 2005.08.22 ���޸�����Ҫ���Ӵ˺���
//	int nMaxLife = this->GetMaxLife();
//	if (this->IsEudemon() && m_pEudemonItem)
//	{
//		m_nCurrLife	= nLifeData;
//		if (m_nCurrLife > nMaxLife)
//			m_nCurrLife	= nMaxLife;
//
//		m_pEudemonItem->SetInt(ITEMDATA_EUDEMON_LIFE, m_nCurrLife, bUpdate?true:false);
//	}
//}

//////////////////////////////////////////////////////////////////////
DWORD CMonster::GetMaxLife()
{
	int nData = 0;
	if(IsEudemon())
	{
		nData = m_eudemonData.dwMaxLife;
	}
	else
	{
		nData = m_pType->GetInt(NPCTYPEDATA_LIFE);
		nData = AdjustUserAttrib(nData,ADJUSTUSERATTRIB_MAXLIFE);
	}
	return nData;
}

DWORD	CMonster::GetAtk()
{
	int nData = 0;
	if(IsEudemon())
		nData = m_eudemonData.dwPhyAtt;
	else
	{
		nData = m_pType->GetInt(NPCTYPEDATA_ATK);
		nData = AdjustUserAttrib(nData,ADJUSTUSERATTRIB_PHYACK);
	}
	return nData;
}

//////////////////////////////////////////////////////////////////////
DWORD CMonster::GetMgcAtk()
{
	int nData = 0;
	if(IsEudemon())
		nData = m_eudemonData.dwMagAtt;
	else
	{
		nData = m_pType->GetInt(NPCTYPEDATA_MAGATK);
		nData = AdjustUserAttrib(nData,ADJUSTUSERATTRIB_MAGACK);
	}
	return nData;
}

DWORD CMonster::GetKilledAction()
{
	DWORD nData = 0;
	if(IsEudemon())
		nData = 0;
	else
		nData = m_pType->GetInt(NPCTYPEDATA_KILLEDACTION);
	return nData;
}

DWORD CMonster::GetAtkHitRate()
{
	int nData = 0;
	if(IsEudemon())
		nData = m_eudemonData.dwhit;
	else
	{
		nData = m_pType->GetInt(NPCTYPEDATA_HITRATE);
		nData = AdjustUserAttrib(nData,ADJUSTUSERATTRIB_HITRANGE);
	}
	return nData;
}

DWORD CMonster::GetAddAttByType(int nType)
{
	int nData = 0;
	switch(nType)
	{
	case 1:
		if(IsEudemon())
			nData = m_EudemonGifeData.m_GiftAtt1;
		else
			nData = m_pType->GetInt(NPCTYPEDATA_ATTADD1);
		break;
	case 2:
		if(IsEudemon())
			nData = m_EudemonGifeData.m_GiftAtt2;
		else
			nData = m_pType->GetInt(NPCTYPEDATA_ATTADD2);
		break;
	case 3:
		if(IsEudemon())
			nData = m_EudemonGifeData.m_GiftAtt3;
		else
			nData = m_pType->GetInt(NPCTYPEDATA_ATTADD3);
		break;
	case 4:
		if(IsEudemon())
			nData = m_EudemonGifeData.m_GiftAtt4;
		else
			nData = m_pType->GetInt(NPCTYPEDATA_ATTADD4);
		break;

	case 11:
		if(IsEudemon())
			nData = m_EudemonGifeData.m_GiftDef1;
		else
			nData = m_pType->GetInt(NPCTYPEDATA_DEFADD1);
		break;
	case 12:
		if(IsEudemon())
			nData = m_EudemonGifeData.m_GiftDef2;
		else
			nData = m_pType->GetInt(NPCTYPEDATA_DEFADD2);
		break;
	case 13:
		if(IsEudemon())
			nData = m_EudemonGifeData.m_GiftDef3;
		else
			nData = m_pType->GetInt(NPCTYPEDATA_DEFADD3);
		break;
	case 14:
		if(IsEudemon())
			nData = m_EudemonGifeData.m_GiftDef4;
		else
			nData = m_pType->GetInt(NPCTYPEDATA_DEFADD4);
		break;
	}
	return nData;
}


//////////////////////////////////////////////////////////////////////
DWORD CMonster::GetDef()
{
	int nData = 0;
	if(IsEudemon())
		nData = m_eudemonData.dwPhyDef;
	else
	{
		nData = m_pType->GetInt(NPCTYPEDATA_DEF);
		nData = AdjustUserAttrib(nData,ADJUSTUSERATTRIB_PHYDEF);
	}
	return nData;
}


DWORD CMonster::GetDodge()
{
	int nData = 0;
	if(IsEudemon())
		nData = m_eudemonData.dwDoge;
	else
	{
		nData = m_pType->GetInt(NPCTYPEDATA_DODGE);
		nData = AdjustUserAttrib(nData,ADJUSTUSERATTRIB_DOG);
	}
	return nData;
}

DWORD CMonster::GetCrit()
{
	int nData = 0;
	if(IsEudemon())
		nData = m_eudemonData.dwCrit;
	else
	{
		nData = m_pType->GetInt(NPCTYPEDATA_CRIT);
		nData = AdjustUserAttrib(nData,ADJUSTUSERATTRIB_Crit);
	}
	return nData;
}

DWORD CMonster::GetTough()
{
	int nData = 0;
	if(IsEudemon())
		nData = m_eudemonData.dwTough;
	else
	{
		nData = m_pType->GetInt(NPCTYPEDATA_THOUGH);
		nData = AdjustUserAttrib(nData,ADJUSTUSERATTRIB_TOUGH);
	}
	return nData;
}

//////////////////////////////////////////////////////////////////////
DWORD CMonster::GetMagicDef()
{
	int nData = 0;
	if(IsEudemon())
		nData = m_eudemonData.dwMagDef;
	else
	{
		nData = m_pType->GetInt(NPCTYPEDATA_MAGDEF);
		nData = AdjustUserAttrib(nData,ADJUSTUSERATTRIB_MAGDEF);
	}
	return nData;
}

//////////////////////////////////////////////////////////////////////
DWORD CMonster::GetInterAtkRate()
{
	int nRate = 1000;
	return nRate;
}

DWORD 	CMonster::AdjustDamage		(DWORD nDamage,int nAttribType)
{
	return nDamage;
}

int 	CMonster::AdjustUserAttrib	(DWORD nAttrib,int nAttribType)
{
	IStatus* pStatus = NULL;
	int nAttAddi = 0;
	switch(nAttribType)
	{
	case ADJUSTUSERATTRIB_PHYDEF:///���(���)
		{
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEUP_PERCENT );//�����������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEDOWN_PERCENT );//��������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEUP_NUMBER );//�������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALDEFENCEDOWN_NUMBER );//��������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALDEFENCEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DEFDOWN_ATKUP );//�Է�������������
			if (pStatus)
			{
				nAttAddi += (-2)*CRole::CalcStatusInfluence( STATUS_DEFDOWN_ATKUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFUP );//�������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFDOWN );//��������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFDOWN,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_OUTERDEF );//���������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_OUTERDEF,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAXMANA:///�����ֵ
		{
			pStatus = QueryStatus( STATUS_MAXMANAUP_PERCENT );//���ħ��ֵ����%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXMANAUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAXMANADOWN_PERCENT );//���ħ��ֵ����%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXMANADOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAXLIFE:///�������ֵ
		{
			pStatus = QueryStatus( STATUS_MAXLIFEUP_PERCENT );//�������ֵ����%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXLIFEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAXLIFEDOWN_PERCENT );//�������ֵ����%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAXLIFEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			if (IsEudemon())
			{
				if (QueryOwnerUser())
				{
					nAttAddi += QueryOwnerUser()->GetPetMaxLifeAdd();
				}
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAGDEF:///ħ��(�ڷ�)
		{
			pStatus = QueryStatus( STATUS_MAGICDEFENCEUP_PERCENT );//ħ����������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEDOWN_PERCENT );//ħ�������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEUP_NUMBER );//ħ������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEUP_NUMBER_1 );//ħ������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEUP_NUMBER_1,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICDEFENCEDOWN_NUMBER );//ħ�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICDEFENCEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFUP );//ħ������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_DEFDOWN );//ħ�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_DEFDOWN,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_INNERDEF );//ħ�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_INNERDEF,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_HITRANGE:///������
		{
			pStatus = QueryStatus( STATUS_HITRATEUP_PERCENT );//����������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_HITRATEDOWN_PERCENT );//�������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_HITRATEUP_NUMBER );//������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PET_HITRATE );//������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PET_HITRATE,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_HITRATEDOWN_NUMBER );//�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_HITRATEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SYN_HITRATEDOWN );//�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SYN_HITRATEDOWN,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_HITRATE );//�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_HITRATE,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_DOG:///����
		{
			pStatus = QueryStatus( STATUS_DODGERATEUP_PERCENT );//����������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DODGERATEDOWN_PERCENT );//�������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DODGERATEUP_NUMBER );//������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PET_DODGE );//������������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PET_DODGE,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DODGERATEDOWN_NUMBER );//�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DODGERATEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_DODGE );//�������½���������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_DODGE,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_PHYACK:///�﹥(�⹦)
		{
			pStatus = QueryStatus( STATUS_PHYSICALATTACKUP_PERCENT );//������������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTACKUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALATTACKDOWN_PERCENT );//���������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTACKDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALATTUP_NUMBER );//���������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_PHYSICALATTDOWN_NUMBER );//���������٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_PHYSICALATTDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_DEFDOWN_ATKUP );//�Է�������������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_DEFDOWN_ATKUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_OUTERATT );//�⹦��������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_OUTERATT,nAttrib,pStatus->GetPower() );
			}
			if (IsEudemon())
			{
				if (QueryOwnerUser())
				{
					nAttAddi += QueryOwnerUser()->GetPetAtkAdd();
				}
			}
		}
		break;
	case ADJUSTUSERATTRIB_MAGACK:///ħ��(�ڹ�)
		{
			pStatus = QueryStatus( STATUS_MAGICATTACKUP_PERCENT );//ħ������������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICATTACKUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_MAGICATTACKDOWN_PERCENT );//ħ���������½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_MAGICATTACKDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_INNERATT );//�ڹ�
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_INNERATT,nAttrib,pStatus->GetPower() );
			}
			if (IsEudemon())
			{
				if (QueryOwnerUser())
				{
					nAttAddi += QueryOwnerUser()->GetPetMagAtkAdd();
				}
			}
		}
		break;
	case ADJUSTUSERATTRIB_Crit:///����
		{
			pStatus = QueryStatus( STATUS_CRITRATEUP_PERCENT );//��������%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEUP_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_CRITRATEDOWN_PERCENT );//�����½�%
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEDOWN_PERCENT,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_CRITRATEUP_NUMBER );//�������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_CRITRATEDOWN_NUMBER );//�������٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_CRITRATEDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_EQUIP_CRIUP );//�������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_EQUIP_CRIUP,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SOUL_CRI );//�������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SOUL_CRI,nAttrib,pStatus->GetPower() );
			}
			if (IsEudemon())
			{
				if (QueryOwnerUser())
				{
					nAttAddi += QueryOwnerUser()->GetPetCrihitAdd();
				}
			}
		}
		break;
	case ADJUSTUSERATTRIB_STR://��������
		{
			pStatus = QueryStatus( STATUS_STRENGUP_NUMBER );//�������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STRENGDOWN_NUMBER );//�������٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STRENGUP_NUMBER_PET );//�������ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STRENGDOWN_NUMBER_PET );//�������٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STRENGDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_INT://����(����)
		{
			pStatus = QueryStatus( STATUS_INTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_INTDOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_INTUP_NUMBER_PET );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_INTDOWN_NUMBER_PET );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_INTDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_STA://����
		{
			pStatus = QueryStatus( STATUS_STAUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STAUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STADOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STADOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STAUP_NUMBER_PET );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STAUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_STADOWN_NUMBER_PET );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_STADOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_SPI://����
		{
			pStatus = QueryStatus( STATUS_SPIUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SPIDOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SPIUP_NUMBER_PET );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_SPIDOWN_NUMBER_PET );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_SPIDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_AGI://����
		{
			pStatus = QueryStatus( STATUS_AGIUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_AGIDOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_AGIUP_NUMBER_PET );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIUP_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_AGIDOWN_NUMBER_PET );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_AGIDOWN_NUMBER_PET,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_BASEATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_BASEATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_TOUGH://����
		{
			pStatus = QueryStatus( STATUS_TOUCHUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_TOUCHUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
			pStatus = QueryStatus( STATUS_TOUGHDOWN_NUMBER );//���٣�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_TOUGHDOWN_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_FIREATK://��
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_ICEATK://��
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MYSTICATK://��
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_NOXATK://��
		{
			pStatus = QueryStatus( STATUS_FOURATTUP_NUMBER );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FOURATTUP_NUMBER,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_FIREDEF://��
		{
			pStatus = QueryStatus( STATUS_FIREDEF_UP );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_ICEDEF://��
		{
			pStatus = QueryStatus( STATUS_ICEDEF_UP );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_MYSTICDEF://��
		{
			pStatus = QueryStatus( STATUS_MYSTICDEF_UP );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	case ADJUSTUSERATTRIB_NOXDEF://��
		{
			pStatus = QueryStatus( STATUS_NOXDEF_UP );//���ӣ�������
			if (pStatus)
			{
				nAttAddi += CRole::CalcStatusInfluence( STATUS_FIREDEF_UP,nAttrib,pStatus->GetPower() );
			}
		}
		break;
	default:
		break;
	}
	if (nAttAddi<0 && nAttrib<abs(nAttAddi))
		return 1;
	else
		return nAttrib+nAttAddi;
}
/////////////////////////////////////////////////////////////////////////////
void CMonster::SetStatus(int nStatus, bool bSynchro, int nPower)
{
	I64 i64OldEffect	= m_i64Effect;
	I64	i64OldEffect2	= m_i64Effect2;

	switch (nStatus)
	{
	case STATUS_NORMAL:
		{
			m_dwStatus	= nStatus;
			m_i64Effect	= KEEPEFFECT_NORMAL;
		}
		break;
	case STATUS_DIE:
		{
			m_dwStatus	= nStatus;
			m_i64Effect	= KEEPEFFECT_DIE;
			ClearAllStatus();
		}
		break;
	case STATUS_DISAPPEARING:
		{
			m_i64Effect	|= KEEPEFFECT_DISAPPEARING;
		}
		break;
	case STATUS_PK_PROTECT:
		{
			m_i64Effect |= KEEPEFFECT_PK_PROTECT;
		}
		break;
// 	case STATUS_SKZL:		//���ʱ��֮�ᣬ����
// 		{
// 			m_i64Effect |= KEEPEFFECT_SKZL;
// 		}
// 		break;
	
//-------------------------------------------------------------------------------
	case STATUS_FRENZY:							//��
		{
			m_i64Effect |= KEEPEFFECT_FRENZY;
		}
		break;
	case STATUS_SHIELD:							//һ��ʱ���ڿɵ���N������	����
	case STATUS_SHIELD_1:							//һ��ʱ���ڿɵ���N������	����
	case STATUS_SHIELD_2:
		{
			m_i64Effect |= KEEPEFFECT_SHIELD;
		}
		break;
	case STATUS_FIRMNESS_MAGIC:					//һ��ʱ�������Ӽ��ܲ�����ϵĻ���	�ᶨʩ��
		{
			m_i64Effect |= KEEPEFFECT_FIRMNESS_MAGIC;
		}
		break;
	case STATUS_HIDDEN:							//����
		{
			m_i64Effect |= KEEPEFFECT_HIDDEN;
		}
		break;
	case STATUS_RETORT:							//����
		{
			m_i64Effect |= KEEPEFFECT_RETORT;
		}
		break;
	case STATUS_LURK:							//NPC�������Լ����������Ч	Ǳ��
		{
			m_i64Effect |= KEEPEFFECT_LURK;
		}
		break;
	case STATUS_IMMUNE_ALL:						//����һ���˺�	�޵�
		{
			m_i64Effect |= KEEPEFFECT_IMMUNE_ALL;
		}
		break;
	case STATUS_SLEEP:							//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������ɽ����
		{
			m_i64Effect |= KEEPEFFECT_SLEEP;
		}
		break;
	case STATUS_DREAMING:						//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������������
		{
			m_i64Effect |= KEEPEFFECT_DREAMING;
		}
		break;
	case STATUS_PALSY:							//�����ƶ�	�������	
		{
			m_i64Effect |= KEEPEFFECT_PALSY;
		}
		break;
	case STATUS_SILENCE:						//����ʹ�ü���	��Ĭ
		{
			m_i64Effect |= KEEPEFFECT_SILENCE;
		}
		break;
	case STATUS_CONFUSION:						//�����ת����ʧĿ��	����
		{
			m_i64Effect |= KEEPEFFECT_CONFUSION;
		}
		break;
	case STATUS_STOPSAY:						//����˵�����ǿ���ϵ	����	
		{
			m_i64Effect |= KEEPEFFECT_STOPSAY;
		}
		break;
	case STATUS_PHYSICALATTACKUP_PERCENT:				//������������
	case STATUS_PHYSICALATTACKUP_PERCENT_WS://20070827����
	case STATUS_PHYATTACKUP_TUTOR_PERCENT:// ʦͽ�ӹ� [�½���10/26/2007]
	case STATUS_PHYSICALATTUP_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_PHYSICALATTACKUP;
		}
		break;
	case STATUS_PHYSICALATTACKDOWN_PERCENT:				//���������½�
	case STATUS_PHYSICALATTDOWN_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_PHYSICALATTACKDOWN;
		}
		break;
	case STATUS_MAGICATTACKUP_PERCENT:					//ħ������������
	case STATUS_MAGICATTACKUP_PERCENT_ED://2007827����:
	case STATUS_MAGATTACKUP_TUTOR_PERCENT://  [�½���10/27/2007]
		{
			m_i64Effect |= KEEPEFFECT_MAGICATTACKUP;
		}
		break;
	case STATUS_MAGICATTACKDOWN_PERCENT:				//ħ���������½�
		{
			m_i64Effect |= KEEPEFFECT_MAGICATTACKDOWN;
		}
		break;
	case STATUS_ATTACKUP_PERCENT:						//����������
		{
			m_i64Effect |= KEEPEFFECT_ATTACKUP;
		}
		break;
	case STATUS_ATTACKDOWN_PERCENT:						//�������½�
		{
			m_i64Effect |= KEEPEFFECT_ATTACKDOWN;
		}
		break;
	case STATUS_PHYSICALDEFENCEUP_PERCENT:				//�����������
	case STATUS_PHYSICALDEFENCEUP_NUMBER:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_SF://2007827����:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_L:
		{
			m_i64Effect |= KEEPEFFECT_PHYSICALDEFENCEUP;
		}
		break;
	case STATUS_PHYSICALDEFENCEDOWN_PERCENT:			//��������½�
	case STATUS_PHYSICALDEFENCEDOWN_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_PHYSICALDEFENCEDOWN;
		}
		break;
	case STATUS_MAGICDEFENCEUP_PERCENT:					//ħ����������
	case STATUS_MAGICDEFENCEUP_NUMBER:
	case STATUS_MAGICDEFENCEUP_NUMBER_EM://2007827����:
	case STATUS_MAGICDEFENCEUP_NUMBER_L:
	case STATUS_MAGICDEFENCEUP_NUMBER_1:
		{
			m_i64Effect |= 	KEEPEFFECT_MAGICDEFENCEUP;
		}
		break;
	case STATUS_MAGICDEFENCEDOWN_PERCENT:				//ħ�������½�
	case STATUS_MAGICDEFENCEDOWN_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_MAGICDEFENCEDOWN;
		}
		break;
	case STATUS_DEFENCEUP_PERCENT:						//����������
	case STATUS_DEFENCEUP_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_DEFENCEUP;
		}
		break;
	case STATUS_DEFENCEDOWN_PERCENT:					//�������½�
	case STATUS_DEFENCEDOWN_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_DEFENCEDOWN;
		}
		break;
	case STATUS_HITRATEUP_PERCENT:						//����������
	case STATUS_HITRATEUP_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_HITRATEUP;
		}
		break;
	case STATUS_HITRATEDOWN_PERCENT:					//�������½�
	case STATUS_HITRATEDOWN_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_HITRATEDOWN;
		}
		break;
	case STATUS_DODGERATEUP_PERCENT:					//���������
	case STATUS_DODGERATEUP_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_DODGERATEUP;
		}
		break;
	case STATUS_DODGERATEDOWN_PERCENT:					//������½�
	case STATUS_DODGERATEDOWN_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_DODGERATEDOWN;
		}
		break;
	case STATUS_CRITRATEUP_PERCENT:						//��������һ������
	case STATUS_CRITRATEUP_NUMBER:
	case STATUS_CRITRATEUP_NUMBER_SW://2007827����:
		{
			m_i64Effect |= KEEPEFFECT_CRITRATEUP;
		}
		break;
	case STATUS_CRITRATEDOWN_PERCENT:					//��������һ������
	case STATUS_CRITRATEDOWN_NUMBER:
		{
			m_i64Effect |= KEEPEFFECT_CRITRATEDOWN;
		}
		break;
	case STATUS_MAXLIFEUP_PERCENT:						//�������ֵ����
		{
			m_i64Effect |= KEEPEFFECT_MAXLIFEUP;
		}
		break;
	case STATUS_MAXLIFEDOWN_PERCENT:					//�������ֵ����
		{
			m_i64Effect |= KEEPEFFECT_MAXLIFEDOWN;
		}
		break;
	case STATUS_MAXMANAUP_PERCENT:						//���ħ��ֵ����
		{
			m_i64Effect |= KEEPEFFECT_MAXMANAUP;
		}
		break;
	case STATUS_MAXMANADOWN_PERCENT:					//���ħ��ֵ����
		{
			m_i64Effect |= KEEPEFFECT_MAXMANADOWN;
		}
		break;
	case STATUS_MAXSPUP_PERCENT:						//�������ֵ����
		{
			m_i64Effect |= KEEPEFFECT_MAXSPUP;
		}
		break;
	case STATUS_MAXSPDOWN_PERCENT:						//��������ֵ����
		{
			m_i64Effect |= KEEPEFFECT_MAXSPDOWN;
		}
		break;
	case STATUS_MOVESPEEDUP_PERCENT:					//�ƶ��ٶ�����
		{
			m_i64Effect |= KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOUNTSPEED_NUMBER:					//�ƶ��ٶ���������
		{
			m_i64Effect |= KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOVESPEEDDOWN_PERCENT:					//�ƶ��ٶȼ���
		{
			m_i64Effect |= KEEPEFFECT_MOVESPEEDDOWN;
		}
		break;
	case STATUS_ATKSPEEDUP_PERCENT:						//�����ٶ�����
		{
			m_i64Effect |= KEEPEFFECT_ATKSPEEDUP;
		}
		break;
	case STATUS_ATKSPEEDDOWN_PERCENT:					//�����ٶȼ���
		{
			m_i64Effect |= KEEPEFFECT_ATKSPEEDDOWN;
		}
		break;
	case STATUS_PHYSICALDAMAGEUP_PERCENT:				//�����˺�����
		{
			m_i64Effect |= KEEPEFFECT_PHYSICALDAMAGEUP;
		}
		break;
	case STATUS_PHYSICALDAMAGEDOWN_PERCENT:				//�����˺���С
		{
			m_i64Effect |= KEEPEFFECT_PHYSICALDAMAGEDOWN; 
		}
		break;
	case STATUS_MAGICDAMAGEUP_PERCENT:					//ħ���˺�����
		{
			m_i64Effect |= KEEPEFFECT_MAGICDAMAGEUP;
		}
		break;
	case STATUS_MAGICDAMAGEDOWN_PERCENT:				//ħ���˺���С
		{
			m_i64Effect |= KEEPEFFECT_MAGICDAMAGEDOWN;
		}
		break;
	case STATUS_DAMAGEUP_PERCENT:						//�˺�����
		{
			m_i64Effect |= KEEPEFFECT_DAMAGEUP;
		}
		break;
	case STATUS_DAMAGEDOWN_PERCENT:						//�˺���С
	case STATUS_DAMAGEDOWN_PERCENT_TN://2007827����:
		{
			m_i64Effect |= KEEPEFFECT_DAMAGEDOWN;

		}
		break;
	case STATUS_GETEXPUP_PERCENT:
		{
			m_i64Effect |= KEEPEFFECT_GETEXPUP;
		}
		break;
	case STATUS_GETEXPUP_PERCENT_SYNINS:
		{
			m_i64Effect |= KEEPEFFECT_GETEXPUP;
		}
		break;
	case STATUS_GETEXPUP_PERCENT_WT:
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_EUDEMONEXPUP;
		}
		break;
	case STATUS_GETEXPDOWN_PERCENT:						//�����ü���
		{
			m_i64Effect |= KEEPEFFECT_GETEXPDOWN;
		}
		break;
// 	case STATUS_ERUPTPOT_PERCENT:						//Ǳ�ܱ���
// 		{
// 			m_i64Effect |= KEEPEFFECT_ERUPTPOT;
// 		}
// 		break;
// 	case STATUS_FROSTSHIELD:							//��˪��
// 		{
// 			m_i64Effect |= KEEPEFFECT_FROSTSHIELD;
// 		}
		break;
//------------dot begin-------------------------------------
 	case STATUS_DOT_POISONING:				//�ж�DOT			,��hp
// 	case STATUS_DOT_POISON_ATK:            // ��Ϯ [12/13/2007 �½���]
		{
			m_i64Effect |= KEEPEFFECT_DOT_POISONING;
		}
		break;
// 	case STATUS_DOT_ENMITY:
	case STATUS_DOT_LIFEMISSING:			//��ѪDOT			,��hp
		{
			m_i64Effect |= KEEPEFFECT_DOT_LIFEMISSING;
		}
		break;
	
// 	case STATUS_DOT_MANAMISSING:			//������ʧDOT		,��mp
// 		{
// 			m_i64Effect |=	KEEPEFFECT_DOT_MANAMISSING;
// 		}
// 		break;
// 	case STATUS_DOT_STRENGTHMISSING:		//������ʧDOT		,��sp
// 		{
// 			m_i64Effect |= KEEPEFFECT_DOT_STRENGTHMISSING;
// 		}
// 		break;
 	case STATUS_DOT_BURNING:				//����DOT			,��hp
		{
			m_i64Effect |= KEEPEFFECT_DOT_BURNING;
		}
		break;
//-----------dot end------------------------------------
//SECOND STATUS BEGIN---------------------------------------------------
	case STATUS_CRAZE://����
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_CRAZE;
		}
		break;
 	case STATUS_DOT_SUFFERING://����DOT
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_SUFFERING;
		}
		break;
// 	case STATUS_DOT_BEATING://���DOT
// 	case STATUS_DOT_BEATINGPRE:// �������2 [12/12/2007 �½���]
// 		{
// 			m_i64Effect2 |= SECONDKEEPEFFECT_BEATING;
// 		}
// 		break;
	case STATUS_DETECTION://ʶ�ƣ�����
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_DETECTION;
		}
		break;
	case STATUS_DABAO://[��;���� 2009.03.18]��
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_DA_BAO;
		}
		break;
	case STATUS_ZHONGBAO://[��;���� 2009.03.18]�б�
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_ZHONG_BAO;
		}
		break;
	case STATUS_XIAOBAO://[��;���� 2009.03.18]С��
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_XIAOBAO;
		}
		break;

// 	case STATUS_DOT_ENMITY:
// 		{
// 			m_i64Effect2 |= SECONDKEEPEFFECT_SNEER;
// 		}
// 		break;
	case STATUS_PVPMODE: //����buff
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_PVP;
		}
		break;
	case STATUS_YABIAO:
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_YABIAO;
		}
		break;
	case STATUS_JIEBIAO:
		{
			m_i64Effect2 |= SECONDKEEPEFFECT_JIEBIAO;
		}
		break;
	case STATUS_BIGBLOOD_USERNOT:
		{
		}
		break;
	case STATUS_SELF_DAMAGEUP:
		{
		}
		break;
	case STATUS_STRENGUP_NUMBER:
		{
			
		}
		break;
	case STATUS_SPENDMP_UP://����������
	case STATUS_STAUP_NUMBER://��������
	case STATUS_STADOWN_NUMBER://��������
	case STATUS_SPIUP_NUMBER://���Ӿ���
	case STATUS_SPIDOWN_NUMBER://���پ���
	case STATUS_AGIUP_NUMBER://��������
	case STATUS_AGIDOWN_NUMBER://��������
	case STATUS_TOUCHUP_NUMBER://���Ӽ���
	case STATUS_TOUGHDOWN_NUMBER://���ټ���
	case STATUS_SELF_DAMAGEUP_NUMBER://����Ե��˺�����
	case STATUS_INTUP_NUMBER://��������
	case STATUS_MAGIC_SHIELD://ħ���ܣ��ܵ��˺�ʱ�������������˿�Ѫ
	case STATUS_FOURATTUP_NUMBER://����������
	case STATUS_FOURATTDOWN_NUMBER://�����Լ���
	case STATUS_BASEATTUP_NUMBER://����������
	case STATUS_SELF_DAMAGEUP_PERCENT://����Ե��˺�����
		{

		}
		break;
	case STATUS_EUDEMON_SOUL://���︽��
	case STATUS_STRENGUP_NUMBER_PET://��������
	case STATUS_STRENGDOWN_NUMBER_PET://��������
	case STATUS_SPIUP_NUMBER_PET://���Ӿ���
	case STATUS_SPIDOWN_NUMBER_PET://���پ���
	case STATUS_AGIUP_NUMBER_PET://��������
	case STATUS_AGIDOWN_NUMBER_PET://��������
	case STATUS_STAUP_NUMBER_PET://��������
	case STATUS_STADOWN_NUMBER_PET://��������
	case STATUS_INTUP_NUMBER_PET://��������
	case STATUS_INTDOWN_NUMBER_PET://��������
	case STATUS_CRIME://����״̬
	case STATUS_DOT_ADDLIFE:
	case STATUS_BOOTH_ADDEXP:
	case STATUS_ITEM_ADDEXP:
	case STATUS_ACTIVATE_ADDEXP:
	case STATUS_DOT_SYNEXP:
	case STATUS_DOT_TUTOREXP:
	case STATUS_DOT_WARGAMEEXP:
	case STATUS_DOT_LIFEMISS1:
	case STATUS_MONSTER_ENMITY:
	case STATUS_DEFDOWN_ATKUP:
	case STATUS_EQUIP_DEFUP://���������	������װ��
	case STATUS_EQUIP_DAMAGEUP://�Ե��˺�����  ������װ��
	case STATUS_EQUIP_CRIUP://��������		������װ��
	case STATUS_EQUIP_BEDAMAGEUP://�ܵ��˺�����	������װ��
	case STATUS_EQUIP_DEFDOWN://���������	������װ��
	case STATUS_DOT_REDMANA://����DOT
	case STATUS_DOT_REDSP://��ŭ��DOT
	case STATUS_BIZ_DEAD:// �����������buff
	case STATUS_FIREDEF_UP://���
	case STATUS_ICEDEF_UP://����
	case STATUS_MYSTICDEF_UP://����
	case STATUS_NOXDEF_UP://����
	case STATUS_SYN_HITRATEDOWN://���н���
	case STATUS_SYN_SHIELD://һ��ʱ���������˺�	����
//	case STATUS_SYNWORKTIME://���ɴ�buff
	case STATUS_SOUL_CRI://����			���
	case STATUS_SOUL_HITRATE://����			���
	case STATUS_SOUL_DODGE://����			���
	case STATUS_SOUL_INNERATT://�ڹ�			���
	case STATUS_SOUL_OUTERATT://�⹦			���
	case STATUS_SOUL_INNERDEF://�ڷ�			���
	case STATUS_SOUL_OUTERDEF://���			���
	case STATUS_SOUL_REBLOOD://��Ѫ			���
	case STATUS_SOUL_REBLOOD_PER:
	case STATUS_SOUL_REMP://��ħ
	case STATUS_TUTOR_BOSS://ʦͽ��������������Ҫ��Buff
	case STATUS_VERIFICATION_WAIT:
	case STATUS_VERIFICATION_RUNNING:
//modify code by �ֵ�Ԩ begin 2011.6.22
	case STATUS_RETORT_TENSEC:
	case STATUS_RETORT_LGWS:
	case STATUS_RETORT_ZHF:
	case STATUS_RETORT_MDSX:
	case STATUS_RETORT_MDSX_BUFF2:
	case STATUS_RETORT_MDSX_1:
//modify code by �ֵ�Ԩ end
	case STATUS_PET_DODGE://���︽�������
	case STATUS_PET_HITRATE://���︽�������
	case STATUS_DOT_ADDEXP:
		{

		}
		break;
//--------------END-----------------------------------------------------

//		{
//			IStatus* pStatus = QueryStatus( STATUS_DECSPEED );
//			IF_OK( pStatus )
//			{
//				CMsgAction	msg;
//				IF_OK( msg.Create( this->GetID(),this->GetPosX(),this->GetPosY(),this->GetDir(),actionMoveStop,pStatus->GetPower() ) )
//					this->SendMsg( &msg );
//			}
//		}

//--------------dot----------end--------------------------
	default:
		::LogSave("Invalid status: [%u]", nStatus);
	}
	if (STATUS_DREAMING == nStatus || STATUS_SLEEP == nStatus || STATUS_CONFUSION == nStatus )
	{
		//this->ClrAttackTarget();	// ֹͣ�Զ�ս��
	}

	if(bSynchro && (i64OldEffect != m_i64Effect || i64OldEffect2 != m_i64Effect2))
	{
		CMsgUserAttrib msg;
		if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, m_i64Effect))
		{
			if ( STATUS_MAXLIFEDOWN_PERCENT == nStatus || STATUS_MAXLIFEUP_PERCENT == nStatus )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}
			
		    if(STATUS_MOVESPEEDDOWN_PERCENT == nStatus)
			{
				msg.Append(_USERATTRIB_MONSTER_SPEEDDOWN, nPower);
			}
			//------------------SECOND STATUS PROCESS BEGIN--------------
			if( i64OldEffect2 != m_i64Effect2 )
			{
				msg.Append( _USERATTRIB_SECONDEFFECTLOW32,m_i64Effect2 );
			}
			//----------------------------------------END---------------
			BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}
	}
	//[��;���� 2009.03.18]�ڶ���Ϣλ
	else if (bSynchro && i64OldEffect2 != m_i64Effect2)
	{
		CMsgUserAttrib msg;
		if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, m_i64Effect))
		{
			msg.Append( _USERATTRIB_SECONDEFFECTLOW32,m_i64Effect2 );
			BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
void CMonster::ClsStatus(int nStatus, bool bSynchro)
{
	I64 i64OldEffect	= m_i64Effect;
	I64 i64OldEffect2	= m_i64Effect2;

	switch (nStatus)
	{
	case STATUS_NORMAL:
		{
			::LogSave( "CMonster::ClsStatus() Invalid status STATUS_NORMAL" );
		}
		break;
	case STATUS_DIE:
		{
			m_dwStatus	= nStatus;
			m_i64Effect	&= ~KEEPEFFECT_DIE;
		}
		break;
	case STATUS_DISAPPEARING:
		{
			m_i64Effect	&= ~KEEPEFFECT_DISAPPEARING;
		}
		break;
	case STATUS_PK_PROTECT:
		{
			m_i64Effect &= ~KEEPEFFECT_PK_PROTECT;
		}
		break;
// 	case STATUS_SKZL:		//���ʱ��֮�ᣬ����
// 		{
// 			m_i64Effect &= ~KEEPEFFECT_SKZL;
// 		}
// 		break;
	
//-------------------------------------------------------------------------------
	case STATUS_FRENZY:							//��
		{
			m_i64Effect &= ~KEEPEFFECT_FRENZY;
		}
		break;
	case STATUS_SHIELD:							//һ��ʱ���ڿɵ���N������	����
	case STATUS_SHIELD_1:						//һ��ʱ���ڿɵ���N������	����
	case STATUS_SHIELD_2:
		{
			m_i64Effect &= ~KEEPEFFECT_SHIELD;
		}
		break;
	case STATUS_FIRMNESS_MAGIC:					//һ��ʱ�������Ӽ��ܲ�����ϵĻ���	�ᶨʩ��
		{
			m_i64Effect &= ~KEEPEFFECT_FIRMNESS_MAGIC;
		}
		break;
	case STATUS_HIDDEN:							//����
		{
			m_i64Effect &= ~KEEPEFFECT_HIDDEN;
		}
		break;
	case STATUS_RETORT:							//����
		{
			m_i64Effect &= ~KEEPEFFECT_RETORT;
		}
		break;
	case STATUS_LURK:							//NPC�������Լ����������Ч	Ǳ��
		{
			m_i64Effect &= ~KEEPEFFECT_LURK;
		}
		break;
	case STATUS_IMMUNE_ALL:						//����һ���˺�	�޵�
		{
			m_i64Effect &= ~KEEPEFFECT_IMMUNE_ALL;
		}
		break;
	case STATUS_SLEEP:							//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������ɽ����
		{
			m_i64Effect &= ~KEEPEFFECT_SLEEP;
		}
		break;
	case STATUS_DREAMING:						//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������������
		{
			m_i64Effect &= ~KEEPEFFECT_DREAMING;
		}
		break;
	case STATUS_PALSY:							//�����ƶ�	�������	
		{
			m_i64Effect &= ~KEEPEFFECT_PALSY;
		}
		break;
	case STATUS_SILENCE:						//����ʹ�ü���	��Ĭ
		{
			m_i64Effect &= ~KEEPEFFECT_SILENCE;
		}
		break;
	case STATUS_CONFUSION:						//�����ת����ʧĿ��	����
		{
			m_i64Effect &= ~KEEPEFFECT_CONFUSION;
		}
		break;
	case STATUS_STOPSAY:						//����˵�����ǿ���ϵ	����	
		{
			m_i64Effect &= ~KEEPEFFECT_STOPSAY;
		}
		break;
	case STATUS_PHYSICALATTACKUP_PERCENT:				//������������
	case STATUS_PHYSICALATTACKUP_PERCENT_WS://2007827����:
	case STATUS_PHYATTACKUP_TUTOR_PERCENT://  [�½���10/26/2007]
	case STATUS_PHYSICALATTUP_NUMBER:
		{
			m_i64Effect &= ~KEEPEFFECT_PHYSICALATTACKUP;
		}
		break;
	case STATUS_PHYSICALATTACKDOWN_PERCENT:				//���������½�
	case STATUS_PHYSICALATTDOWN_NUMBER:
		{
			m_i64Effect &= ~KEEPEFFECT_PHYSICALATTACKDOWN;
		}
		break;
	case STATUS_MAGICATTACKUP_PERCENT:					//ħ������������
	case STATUS_MAGICATTACKUP_PERCENT_ED://2007827����:
	case STATUS_MAGATTACKUP_TUTOR_PERCENT://  [�½���10/27/2007]
		{
			m_i64Effect &= ~KEEPEFFECT_MAGICATTACKUP;
		}
		break;
	case STATUS_MAGICATTACKDOWN_PERCENT:				//ħ���������½�
		{
			m_i64Effect &= ~KEEPEFFECT_MAGICATTACKDOWN;
		}
		break;
	case STATUS_ATTACKUP_PERCENT:						//����������
		{
			m_i64Effect &= ~KEEPEFFECT_ATTACKUP;
		}
		break;
	case STATUS_ATTACKDOWN_PERCENT:						//�������½�
		{
			m_i64Effect &= ~KEEPEFFECT_ATTACKDOWN;
		}
		break;
	case STATUS_PHYSICALDEFENCEUP_PERCENT:				//�����������
	case STATUS_PHYSICALDEFENCEUP_NUMBER:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_SF://2007827����:
	case STATUS_PHYSICALDEFENCEUP_NUMBER_L:
		{
			m_i64Effect &= ~KEEPEFFECT_PHYSICALDEFENCEUP;
		}
		break;
	case STATUS_PHYSICALDEFENCEDOWN_PERCENT:			//��������½�
	case STATUS_PHYSICALDEFENCEDOWN_NUMBER:
		{
			m_i64Effect &= ~KEEPEFFECT_PHYSICALDEFENCEDOWN;
		}
		break;
	case STATUS_MAGICDEFENCEUP_PERCENT:					//ħ����������
	case STATUS_MAGICDEFENCEUP_NUMBER:
	case STATUS_MAGICDEFENCEUP_NUMBER_EM://2007827����:
	case STATUS_MAGICDEFENCEUP_NUMBER_L:
	case STATUS_MAGICDEFENCEUP_NUMBER_1:
		{
			m_i64Effect &= ~KEEPEFFECT_MAGICDEFENCEUP;
		}
		break;
	case STATUS_MAGICDEFENCEDOWN_PERCENT:				//ħ�������½�
	case STATUS_MAGICDEFENCEDOWN_NUMBER:
		{
			m_i64Effect &= ~KEEPEFFECT_MAGICDEFENCEDOWN;
		}
		break;
	case STATUS_DEFENCEUP_PERCENT:						//����������
	case STATUS_DEFENCEUP_NUMBER:
		{
			m_i64Effect &= ~KEEPEFFECT_DEFENCEUP;
		}
		break;
	case STATUS_DEFENCEDOWN_PERCENT:					//�������½�
	case STATUS_DEFENCEDOWN_NUMBER:
		{
			m_i64Effect &= ~KEEPEFFECT_DEFENCEDOWN;
		}
		break;
	case STATUS_HITRATEUP_PERCENT:						//����������
	case STATUS_HITRATEUP_NUMBER:
		{
			m_i64Effect &= ~KEEPEFFECT_HITRATEUP;
		}
		break;
	case STATUS_HITRATEDOWN_PERCENT:					//�������½�
	case STATUS_HITRATEDOWN_NUMBER:
		{
			m_i64Effect &= ~KEEPEFFECT_HITRATEDOWN;
		}
		break;
	case STATUS_DODGERATEUP_PERCENT:					//���������
		{
			m_i64Effect &= ~KEEPEFFECT_DODGERATEUP;
		}
		break;
	case STATUS_DODGERATEDOWN_PERCENT:					//������½�
		{
			m_i64Effect &= ~KEEPEFFECT_DODGERATEDOWN;
		}
		break;
	case STATUS_CRITRATEUP_PERCENT:						//��������һ������
	case STATUS_CRITRATEUP_NUMBER:
	case STATUS_CRITRATEUP_NUMBER_SW://2007827����:
		{
			m_i64Effect &= ~KEEPEFFECT_CRITRATEUP;
		}
		break;
	case STATUS_CRITRATEDOWN_PERCENT:					//��������һ������
	case STATUS_CRITRATEDOWN_NUMBER:
		{
			m_i64Effect &= ~KEEPEFFECT_CRITRATEDOWN;
		}
		break;
	case STATUS_MAXLIFEUP_PERCENT:						//�������ֵ����
		{
			m_i64Effect &= ~KEEPEFFECT_MAXLIFEUP;
		}
		break;
	case STATUS_MAXLIFEDOWN_PERCENT:					//�������ֵ����
		{
			m_i64Effect &= ~KEEPEFFECT_MAXLIFEDOWN;
		}
		break;
	case STATUS_MAXMANAUP_PERCENT:						//���ħ��ֵ����
		{
			m_i64Effect &= ~KEEPEFFECT_MAXMANAUP;
		}
		break;
	case STATUS_MAXMANADOWN_PERCENT:					//���ħ��ֵ����
		{
			m_i64Effect &= ~KEEPEFFECT_MAXMANADOWN;
		}
		break;
	case STATUS_MAXSPUP_PERCENT:						//�������ֵ����
		{
			m_i64Effect &= ~KEEPEFFECT_MAXSPUP;
		}
		break;
	case STATUS_MAXSPDOWN_PERCENT:						//��������ֵ����
		{
			m_i64Effect &= ~KEEPEFFECT_MAXSPDOWN;
		}
		break;
	case STATUS_MOVESPEEDUP_PERCENT:					//�ƶ��ٶ�����
		{
			m_i64Effect &= ~KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOUNTSPEED_NUMBER:					//�ƶ��ٶ����� ����
		{
			m_i64Effect &= ~KEEPEFFECT_MOVESPEEDUP;
		}
		break;
	case STATUS_MOVESPEEDDOWN_PERCENT:					//�ƶ��ٶȼ���
		{
			m_i64Effect &= ~KEEPEFFECT_MOVESPEEDDOWN;
		}
		break;
	case STATUS_ATKSPEEDUP_PERCENT:						//�����ٶ�����
		{
			m_i64Effect &= ~KEEPEFFECT_ATKSPEEDUP;
		}
		break;
	case STATUS_ATKSPEEDDOWN_PERCENT:					//�����ٶȼ���
		{
			m_i64Effect &= ~KEEPEFFECT_ATKSPEEDDOWN;
		}
		break;
	case STATUS_PHYSICALDAMAGEUP_PERCENT:				//�����˺�����
		{
			m_i64Effect &= ~KEEPEFFECT_PHYSICALDAMAGEUP;
		}
		break;
	case STATUS_PHYSICALDAMAGEDOWN_PERCENT:				//�����˺���С
		{
			m_i64Effect &= ~KEEPEFFECT_PHYSICALDAMAGEDOWN; 
		}
		break;
	case STATUS_MAGICDAMAGEUP_PERCENT:					//ħ���˺�����
		{
			m_i64Effect &= ~KEEPEFFECT_MAGICDAMAGEUP;
		}
		break;
	case STATUS_MAGICDAMAGEDOWN_PERCENT:				//ħ���˺���С
		{
			m_i64Effect &= ~KEEPEFFECT_MAGICDAMAGEDOWN;
		}
		break;
	case STATUS_DAMAGEUP_PERCENT:						//�˺�����
		{
			m_i64Effect &= ~KEEPEFFECT_DAMAGEUP;
		}
		break;
	case STATUS_DAMAGEDOWN_PERCENT:						//�˺���С
	case STATUS_DAMAGEDOWN_PERCENT_TN://2007827����:
		{
			m_i64Effect &= ~KEEPEFFECT_DAMAGEDOWN;

		}
		break;
	case STATUS_GETEXPUP_PERCENT:						//����������
		{
			m_i64Effect &= ~KEEPEFFECT_GETEXPUP;
		}
		break;
	case STATUS_GETEXPUP_PERCENT_SYNINS:
		{
			m_i64Effect &= ~KEEPEFFECT_GETEXPUP;
		}
		break;
	case STATUS_GETEXPUP_PERCENT_WT://2007827����:
		{
			m_i64Effect2 &= ~SECONDKEEPEFFECT_EUDEMONEXPUP;
		}
		break;
	case STATUS_GETEXPDOWN_PERCENT:						//�����ü���
		{
			m_i64Effect &= ~KEEPEFFECT_GETEXPDOWN;
		}
// 		break;
// 	case STATUS_ERUPTPOT_PERCENT:						//Ǳ�ܱ���
// 		{
// 			m_i64Effect &= ~KEEPEFFECT_ERUPTPOT;
// 		}
// 		break;
// 	case STATUS_FROSTSHIELD:							//��˪��
// 		{
// 			m_i64Effect &= ~KEEPEFFECT_FROSTSHIELD;
// 		}
// 		break;

//------------dot begin-------------------------------------
 	case STATUS_DOT_POISONING:				//�ж�DOT			,��hp
// 	case STATUS_DOT_POISON_ATK:            // ��Ϯ [12/13/2007 �½���]
		{
			m_i64Effect &= ~KEEPEFFECT_DOT_POISONING;
		}
		break;
// 	case STATUS_DOT_ENMITY:
	case STATUS_DOT_LIFEMISSING:			//��ѪDOT			,��hp
		{
			m_i64Effect &= ~KEEPEFFECT_DOT_LIFEMISSING;
		}
		break;
// 	case STATUS_DOT_MANAMISSING:			//������ʧDOT		,��mp
// 		{
// 			m_i64Effect &= ~KEEPEFFECT_DOT_MANAMISSING;
// 		}
// 		break;
// 	case STATUS_DOT_STRENGTHMISSING:		//������ʧDOT		,��sp
// 		{
// 			m_i64Effect &= ~KEEPEFFECT_DOT_STRENGTHMISSING;
// 		}
// 		break;
 	case STATUS_DOT_BURNING:				//����DOT			,��hp
		{
			m_i64Effect &= ~KEEPEFFECT_DOT_BURNING;
		}
		break;
//-----------dot end------------------------------------
//SECOND STATUS BEGIN---------------------------------------------------
	case STATUS_CRAZE://����
		{
			m_i64Effect2 &= ~SECONDKEEPEFFECT_CRAZE;
		}
		break;
	case STATUS_DOT_SUFFERING://����DOT
		{
			m_i64Effect2 &= ~SECONDKEEPEFFECT_SUFFERING;
		}
		break;
// 	case STATUS_DOT_BEATING://���DOT
// 	case STATUS_DOT_BEATINGPRE:// �������2 [12/12/2007 �½���]
// 		{
// 			m_i64Effect2 &= ~SECONDKEEPEFFECT_BEATING;
// 		}
// 		break;
	case STATUS_DETECTION://ʶ�ƣ�����
		{
			m_i64Effect2 &= ~SECONDKEEPEFFECT_DETECTION;
		}
		break;

// 	case STATUS_DOT_ENMITY:
// 		{
// 			m_i64Effect2 &= ~SECONDKEEPEFFECT_SNEER;
// 		}
// 		break;
	case STATUS_PVPMODE://����buff
		{
			m_i64Effect2 &= ~SECONDKEEPEFFECT_PVP;
		}
		break;
	case STATUS_YABIAO:
		{
			m_i64Effect2 &= ~SECONDKEEPEFFECT_YABIAO;
		}
		break;
	case STATUS_JIEBIAO:
		{
			m_i64Effect2 &= ~SECONDKEEPEFFECT_JIEBIAO;
		}
		break;
	case STATUS_BIGBLOOD_USERNOT:
		{
		}
		break;
	case STATUS_SELF_DAMAGEUP:
		{
		}
		break;
	case STATUS_STRENGUP_NUMBER:
		{
			
		}
		break;
	case STATUS_SPENDMP_UP://����������
	case STATUS_STAUP_NUMBER://��������
	case STATUS_STADOWN_NUMBER://��������
	case STATUS_SPIUP_NUMBER://���Ӿ���
	case STATUS_SPIDOWN_NUMBER://���پ���
	case STATUS_AGIUP_NUMBER://��������
	case STATUS_AGIDOWN_NUMBER://��������
	case STATUS_TOUCHUP_NUMBER://���Ӽ���
	case STATUS_TOUGHDOWN_NUMBER://���ټ���
	case STATUS_SELF_DAMAGEUP_NUMBER://����Ե��˺�����
	case STATUS_INTUP_NUMBER://��������
	case STATUS_MAGIC_SHIELD://ħ���ܣ��ܵ��˺�ʱ�������������˿�Ѫ
	case STATUS_FOURATTUP_NUMBER://����������
	case STATUS_FOURATTDOWN_NUMBER://�����Լ���
	case STATUS_BASEATTUP_NUMBER://����������
	case STATUS_SELF_DAMAGEUP_PERCENT://����Ե��˺�����
		{
			
		}
		break;

	case STATUS_EUDEMON_SOUL://���︽��
	case STATUS_STRENGUP_NUMBER_PET://��������

	case STATUS_STRENGDOWN_NUMBER_PET://��������
	case STATUS_SPIUP_NUMBER_PET://���Ӿ���
	case STATUS_SPIDOWN_NUMBER_PET://���پ���
	case STATUS_AGIUP_NUMBER_PET://��������
	case STATUS_AGIDOWN_NUMBER_PET://��������
	case STATUS_STAUP_NUMBER_PET://��������
	case STATUS_STADOWN_NUMBER_PET://��������
	case STATUS_INTUP_NUMBER_PET://��������
	case STATUS_INTDOWN_NUMBER_PET://��������
	case STATUS_CRIME://����״̬
	case STATUS_DOT_ADDLIFE:
	case STATUS_BOOTH_ADDEXP:
	case STATUS_ITEM_ADDEXP:
	case STATUS_ACTIVATE_ADDEXP:
	case STATUS_DOT_TUTOREXP:
	case STATUS_DOT_SYNEXP:
	case STATUS_DOT_WARGAMEEXP:
	case STATUS_DOT_LIFEMISS1:
	case STATUS_MONSTER_ENMITY:
	case STATUS_DEFDOWN_ATKUP:
	case STATUS_EQUIP_DEFUP://���������	������װ��
	case STATUS_EQUIP_DAMAGEUP://�Ե��˺�����  ������װ��
	case STATUS_EQUIP_CRIUP://��������		������װ��
	case STATUS_EQUIP_BEDAMAGEUP://�ܵ��˺�����	������װ��
	case STATUS_EQUIP_DEFDOWN://���������	������װ��
	case STATUS_DOT_REDMANA://����DOT
	case STATUS_DOT_REDSP://��ŭ��DOT
	case STATUS_BIZ_DEAD:// �����������buff
	case STATUS_FIREDEF_UP://���
	case STATUS_ICEDEF_UP://����
	case STATUS_MYSTICDEF_UP://����
	case STATUS_NOXDEF_UP://����
	case STATUS_SYN_HITRATEDOWN://���н���
	case STATUS_SYN_SHIELD://һ��ʱ���������˺�	����
//	case STATUS_SYNWORKTIME://���ɴ�buff
	case STATUS_SOUL_CRI://����			���
	case STATUS_SOUL_HITRATE://����			���
	case STATUS_SOUL_DODGE://����			���
	case STATUS_SOUL_INNERATT://�ڹ�			���
	case STATUS_SOUL_OUTERATT://�⹦			���
	case STATUS_SOUL_INNERDEF://�ڷ�			���
	case STATUS_SOUL_OUTERDEF://���			���
	case STATUS_SOUL_REBLOOD://��Ѫ			���
	case STATUS_SOUL_REBLOOD_PER:
	case STATUS_SOUL_REMP://��ħ
	case STATUS_TUTOR_BOSS://ʦͽ��������������Ҫ��Buff
	case STATUS_VERIFICATION_WAIT:
	case STATUS_VERIFICATION_RUNNING:
//modify code by �ֵ�Ԩ begin 2011.6.22
	case STATUS_RETORT_TENSEC:
	case STATUS_RETORT_LGWS:
	case STATUS_RETORT_ZHF:
	case STATUS_RETORT_MDSX:
	case STATUS_RETORT_MDSX_BUFF2:
	case STATUS_RETORT_MDSX_1:
//modify code by �ֵ�Ԩ end
	case STATUS_PET_DODGE://���︽�������
	case STATUS_PET_HITRATE://���︽�������
	case STATUS_DOT_ADDEXP:
		{
			
		}
		break;
//--------------END-----------------------------------------------------


//	case	STATUS_DECSPEED://���ᱦʯ�������ƶ��ٶȡ�
//		{
//			m_i64Effect &= ~KEEPEFFECT_DEC_SPEED;
//			//-----------------06.9.25�������-------------------------------------------------------
//			CMsgAction	msg;
//			IF_OK( msg.Create( this->GetID(),this->GetPosX(),this->GetPosY(),this->GetDir(),actionResumeMoveSpeed,0 ) )
//				this->SendMsg( &msg );
//			//----------------------END----------------------------------------------------------------
//		}
//		break;
	default:
		::LogSave("Invalid status: [%u]", nStatus);
	}

	if(bSynchro && (i64OldEffect != m_i64Effect || i64OldEffect2 != m_i64Effect2))
	{
		CMsgUserAttrib msg;
		if (msg.Create(GetID(), _USERATTRIB_KEEPEFFECT, m_i64Effect))
		{
			if ( STATUS_MAXLIFEUP_PERCENT == nStatus || STATUS_MAXLIFEDOWN_PERCENT )
			{
				msg.Append(_USERATTRIB_MAXLIFE, this->GetMaxLife());
				msg.Append(_USERATTRIB_LIFE, this->GetLife());
			}

			if(STATUS_MOVESPEEDDOWN_PERCENT == nStatus)
			{
				msg.Append(_USERATTRIB_MONSTER_SPEEDDOWN, 0);
			}


			//------------------SECOND STATUS PROCESS BEGIN--------------
			if( i64OldEffect2 != m_i64Effect2 )
			{
				msg.Append( _USERATTRIB_SECONDEFFECTLOW32,m_i64Effect2 );
			}
			//----------------------------------------END---------------

			BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
void CMonster::SaveInfo()
{
	if(m_pPetData)
	{
		m_pPetData->SaveData();
	}
}

/////////////////////////////////////////////////////////////////////////////
bool CMonster::IsAlive()
{
	if ((m_i64Effect & KEEPEFFECT_DIE) == KEEPEFFECT_DIE || this->GetLife() <= 0 || m_bLeaveMap)
		return false;

	return true;
}

/*////////////////////////////////////////////////////////////////////////////
BOOL CMonster::CheckStatus(int nStatus)
{
	if (nStatus == STATUS_NORMAL || STATUS_DIE == nStatus)
	{
		if (m_dwStatus == nStatus)
			return true;
		else
			return false;
	}
	else
	{
		if ((m_dwStatus & nStatus) == nStatus)
			return true;
		else
			return false;
	}
}*/

/////////////////////////////////////////////////////////////////////////////
int CMonster::AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg/*=false*/)
{
	CHECKF (pTarget);
	int nExp = nRawExp;

	// adjust
	nExp = CBattleSystem::AdjustExp(nExp, this->GetLev(), pTarget->GetLev());

	int nChangeData = 0;
	IStatus* pStatus;
	pStatus = this->QueryStatus( STATUS_GETEXPUP_PERCENT );
	if (pStatus)
		nChangeData += CRole::CalcStatusInfluence( STATUS_GETEXPUP_PERCENT,nExp,pStatus->GetPower() );
	pStatus = this->QueryStatus( STATUS_GETEXPUP_PERCENT_WT );
	if (pStatus)
		nChangeData += CRole::CalcStatusInfluence( STATUS_GETEXPUP_PERCENT_WT,nExp,pStatus->GetPower() );

// 	pStatus = this->QueryStatus( STATUS_GETEXPDOWN_PERCENT );
// 	if (pStatus)
// 		nChangeData += CRole::CalcStatusInfluence( STATUS_GETEXPDOWN_PERCENT,nExp,pStatus->GetPower() );

	return nExp + nChangeData;	
}

bool CMonster::AwardExp(__int64 i64Exp)
{
	bool bRet = false;
	CMsgEudemonAttrib msg;
	if(!msg.Create(GetID(), EUDATTR_NULL, 0))
		return false;
		
	if(IsEudemon())
	{
		if(!m_pPetData)
			return false;
		int nowLev = m_pPetData->GetInt(EUDEMONDATA_level);
		int nowExp = GetExp() + i64Exp;
		int addlev = 0;
		int ownerLev = QueryOwnerUser()->GetLev();
		if (nowLev >= ownerLev+5)
		{
			QueryOwnerUser()->SendSysMsg(_TXTATR_TOPSHOW, "���ĳ���ȼ��ѳ�������ȼ�5��,�޷���þ���");
			return false;
		}
		for(;;)
		{
			UINT nNeedUpExp	= CItem::GetEudemonNeedExp(nowLev + addlev);
			if(nNeedUpExp <= 0)
			{
				nowExp = 0;
				break;
			}

			if(nowExp >= nNeedUpExp)
			{
				addlev ++;
				nowExp -= nNeedUpExp;
			}
			else
			{
				break;
			}
		}
		
		if(m_pPetData->GetInt(EUDEMONDATA_level) + addlev >= ownerLev+5)
		{
			addlev = ownerLev + 5 - m_pPetData->GetInt(EUDEMONDATA_level);
			nowExp = 0;
		}
			
		if(addlev > 0)
		{
			m_pPetData->SetInt(EUDEMONDATA_level,m_pPetData->GetInt(EUDEMONDATA_level) + addlev);
			m_pPetData->SetInt(EUDEMONDATA_exp,nowExp);

			int nOldPoint = m_pPetData->GetInt(EUDEMONDATA_addpoint);
			m_pPetData->SetInt(EUDEMONDATA_addpoint,nOldPoint + addlev * 5);

			m_pPetData->SetInt(EUDEMONDATA_life,m_eudemonData.dwMaxLife);
			m_pPetData->SaveData();
			
			UpLevEudemonMagic();
			UpdateEudemonAll();			

			msg.Create(GetID(), EUDATTR_NULL, 0);
			msg.Append(EUDATTR_LEVEL,GetLev());
			msg.Append(EUDATTR_MAXLIFE,m_eudemonData.dwMaxLife);
			msg.Append(EUDATTR_LIFE,m_eudemonData.dwMaxLife);			
			BroadcastRoomMsg(&msg,INCLUDE_SELF);

 			
			msg.Append(EUDATTR_EXP,GetExp());			
			msg.Append(EUDATTR_ADDPOINT,m_pPetData->GetInt(EUDEMONDATA_addpoint));
			msg.Append(EUDATTR_STR,m_eudemonData.dwStr);
			msg.Append(EUDATTR_INT,m_eudemonData.dwInt);
			msg.Append(EUDATTR_STA,m_eudemonData.dwSta);
			msg.Append(EUDATTR_SPI,m_eudemonData.dwSpi);
			msg.Append(EUDATTR_AGI,m_eudemonData.dwAgi);
			msg.Append(EUDATTR_ATK,m_eudemonData.dwPhyAtt);
			msg.Append(EUDATTR_MAGICATK,m_eudemonData.dwMagAtt);
			msg.Append(EUDATTR_DEF,m_eudemonData.dwPhyDef);
			msg.Append(EUDATTR_MAGICDEF,m_eudemonData.dwMagDef);
			msg.Append(EUDATTR_HIT,m_eudemonData.dwhit);
			msg.Append(EUDATTR_DOGE,m_eudemonData.dwDoge);
			msg.Append(EUDATTR_CRIT,m_eudemonData.dwCrit);
			msg.Append(EUDATTR_TOUGH,m_eudemonData.dwTough);
			SendMsg(&msg);
//modify code by �ֵ�Ԩ begin 2011.5.24
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) pet %u(level up:%d) (exp:%d)", QueryOwnerUser()->GetID(), QueryOwnerUser()->GetName(), GetID(), m_pPetData->GetInt(EUDEMONDATA_level), nowExp);
//modify code by �ֵ�Ԩ end
			bRet = true;
		}
		else if(nowExp > 0)
		{
			m_pPetData->SetInt(EUDEMONDATA_exp,nowExp);
			msg.Append(EUDATTR_EXP,GetExp());
			SendMsg(&msg);
			bRet = true;
		}
	}
	return bRet;
}
/////////////////////////////////////////////////////////////////////////////
DWORD CMonster::AwardBattleExp		(int nExp, bool bGemEffect/*=true*/, bool bIncludeOwner/*=true*/)
{
	return 0;
//	if (IsEudemon() && m_pEudemonItem && m_pOwner)
//	{
//		if (GetLev() < m_pOwner->GetLev())
//			m_pOwner->AwardEudemonExp(m_pEudemonItem->GetID(), nExp, bGemEffect);
//		// ���˵õ���ͬ�ľ���
//		if (bIncludeOwner)
//			m_pOwner->AwardBattleExp(nExp, bGemEffect);
//	}
//	else
//	if(IsCallPet() && m_pOwner)
//	{
//		m_pOwner->AwardBattleExp(nExp, bGemEffect);
//	}
}

/////////////////////////////////////////////////////////////////////////////
bool CMonster::IsInsteadMagic()
{
	return false/* && m_pMagicType->GetInt(MAGICTYPEDATA_SORT) == MAGICSORT_INSTEAD*/;
}

/////////////////////////////////////////////////////////////////////////////
OBJID CMonster::GetMasterID()
{
	if((IsCallPet() || IsEudemon()) && m_pOwner)
		return m_pOwner->GetID();
	return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::IsImmunity(IRole* pRole)
{
	CHECKF(pRole);

	if((IsCallPet() || IsEudemon()) && m_pOwner)		// call pet of mine
		return m_pOwner->IsImmunity(pRole);

	const bool	ATK_IT = false;
	const bool	IMMUNITY = true;
	OBJID idRole = pRole->GetID();
	if(IsSceneID(idRole))		// is npc
	{
		if(pRole->IsBeAtkable())
			return ATK_IT;
		else
			return IMMUNITY;
	}
	else if(IsNpcID(idRole))	// is monster
	{
		if(IsEvil() && pRole->IsVirtuous() || IsVirtuous() && pRole->IsEvil())
			return ATK_IT;
		else
			return IMMUNITY;
	}
	else	// user
	{

		// �����޸�   08.6.12  ���

//		if( strstr( this->GetName(),"������" ) )
//		{
//			if( pRole->GetProfession() & PROF_HUMAN )
//			{
//				return IMMUNITY;
//			}
//			return ATK_IT;
//		}
//		else if( strstr( this->GetName(),"����" ) )
//		{
//			if( pRole->GetProfession() & PROF_DRAGON )
//			{
//				return IMMUNITY;
//			}
//			return ATK_IT;
//
//		}
//		else if( strstr( this->GetName(),"������" ) ) 
//		{
//			if( pRole->GetProfession() & PROF_GENE )
//			{
//				return IMMUNITY;
//			}
//			return ATK_IT;
//		}
		if(IsEvil())	// || IsGuard() && pUser->IsCrime() || IsPkKiller() && pUser->IsPker())
			return ATK_IT;
		else
			return IMMUNITY;
	}
}

//////////////////////////////////////////////////////////////////////
int CMonster::GetNameType(int nAtkerLev, int nMonsterLev)
{
	int nDeltaLev = nAtkerLev - nMonsterLev;

	if (nDeltaLev >= 3)
		return NAME_GREEN;
	else if (nDeltaLev >= 0)
		return NAME_WHITE;
	else if (nDeltaLev >= -5)
		return NAME_RED;
	else
		return NAME_BLACK;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::KickBack()
{
	// Monster����Ҫ�߻أ�CallPet��Eudemon����Ҫ
//	if (!((this->IsEudemon() || this->IsCallPet()) && m_pOwner))
//		return true;

	CMsgAction	msg;
	IF_OK (msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionKickBack, GetPosX(), GetPosY()))
		SendMsg(&msg);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::TryItem(CItem* pItem, int nPosition)
{
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::EquipItem(CItem* pItem, int nPosition, bool bSynchro)
{
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::UnEquipItem(int nPosition, bool bSynchro)
{
	return false;
}

void CMonster::UpdateEudemonAll()
{
	if(IsEudemon())
	{
		m_pOwner->LoadEudemon(this->GetID(),m_eudemonData,3);
		InitGiftData();
	}
}

//////////////////////////////////////////////////////////////////////
CItem* CMonster::UnEquipOnly(int nPosition)
{
	CHECKF (m_pOwner);

	CItem* pItem = NULL;
	CItemPtr& refpEquip = GetEquipItemRef(nPosition);
	if (refpEquip == NULL)
		return NULL;

	if (m_pOwner->QueryPackage()->IsPackFull())
		return NULL;

	if(refpEquip)
	{
		pItem = refpEquip;
		pItem->SetInt(ITEMDATA_OWNERID, m_pOwner->GetID(), UPDATE_TRUE);
		m_pOwner->QueryPackage()->AddItem(pItem);
		refpEquip = NULL;
	}
	return pItem;	
}

//////////////////////////////////////////////////////////////////////
CItem* CMonster::GetEquipItem(OBJID idItem)
{
	if(m_pHelmet && m_pHelmet->GetID() == idItem)
		return m_pHelmet;
	else if(m_pNecklace && m_pNecklace->GetID() == idItem)
		return m_pNecklace;
	else if(m_pArmor && m_pArmor->GetID() == idItem)
		return m_pArmor;
	else if(m_pWeaponR && m_pWeaponR->GetID() == idItem)
		return m_pWeaponR;
	else if(m_pRingL && m_pRingL->GetID() == idItem)
		return m_pRingL;
	else if(m_pShoes && m_pShoes->GetID() == idItem)
		return m_pShoes;
	
	return NULL;	
}

//////////////////////////////////////////////////////////////////////
CItemPtr* CMonster::GetEquipItemPtr(int nPosition)
{
	return NULL;
	switch(nPosition)
	{
	case	ITEMPOSITION_HELMET:
		return &m_pHelmet;
	case	ITEMPOSITION_NECKLACE:
		return &m_pNecklace;
	case	ITEMPOSITION_ARMOR:
		return &m_pArmor;
	case	ITEMPOSITION_WEAPONR:
		return &m_pWeaponR;
// 	case	ITEMPOSITION_RINGL:
// 		return &m_pRingL;
	case	ITEMPOSITION_SHOES:
		return &m_pShoes;
	}
	
	return NULL;	
}

//////////////////////////////////////////////////////////////////////
CItem* CMonster::GetEquipItemByPos(int nPosition)
{
	CItemPtr* ppEquip = this->GetEquipItemPtr(nPosition);
	if (!ppEquip)
		return NULL;
	
	return *ppEquip;
}

//////////////////////////////////////////////////////////////////////
CItemPtr& CMonster::GetEquipItemRef(int nPosition)
{
	CItemPtr* ppEquip = this->GetEquipItemPtr(nPosition);
	ASSERT (ppEquip);	
	return *ppEquip;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::LoadEquipment(IDatabase* pDb)
{
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMonster::DropItem(OBJID idItem, int x, int y)
{
	return true;
}


int CMonster::GetPhyle()
{
	return 0;
}

void CMonster::AddEnmitySynNPC(OBJID id,
								int nEnmity,
								USHORT nMax,
								int nEnmAddi/* = NULL*/,
								BOOL bSyn/* = true*/)
{
	//Satan 2009.07.29  ���ܼ��Լ������
	if(id == this->GetID())
		return;

	AddEnmity(id,nEnmity,nEnmAddi);

	if(bSyn)//ͬ����npc
	{

		CMsgInteract msg;
		IF_OK(msg.Create2(INTERACT_SynEnmity,
			GetID(),//idSender
			id,//idTarget
			0,//unPosX.����
			nMax,//unPosY.����
			nEnmity,//dwData
			nEnmAddi))//dwBattleAddInfo
		{
			this->SendMsg(&msg);
		}
	}
}

//------------------------------------------
//20080110����:��ճ�ޱ�.��ͬ����npc
void CMonster::ClsEnmityListSynNpc(BOOL bSyn/* = true*/)
{
	this->ClsEnmityList();

	if(bSyn)
	{
		CMsgAction msg;
		IF_OK(msg.Create(this->GetID(),//idPlayer
			0,//PosX
			0,//posY
			0,//usDir
			MSGACTION_CLSENMITYLIST))
		{
			this->SendMsg(&msg);
		}
	}
}

BOOL CMonster::InitEudemonMagic()
{
	CHECKF(m_pMagic);
	if(!IsEudemon())
		return false;

	int nDataTemp = 0;
	CEudemonMagicData* pMagicData[10] = {NULL};
	pMagicData[0] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill1()/1000);//���ܱ��޸ģ�����λ�Ǽ��ܵȼ�
	pMagicData[1] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill2()/1000);
	pMagicData[2] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill3()/1000);
	pMagicData[3] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill4()/1000);
	pMagicData[4] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill5()/1000);
	pMagicData[5] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill6()/1000);
	pMagicData[6] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill7()/1000);
	pMagicData[7] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill8()/1000);
	pMagicData[8] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill9()/1000);
	pMagicData[9] = g_pNpcMagicManager->QueryEudemonData(this->GetSkill10()/1000);
	int nMagicLevel = GetLev();
	for (int i=0; i<10; i++)
	{
		if(pMagicData[i])
		{
			MagicInfoStruct magicInfo1;
			memset(&magicInfo1,0,sizeof(MagicInfoStruct));
			nDataTemp = pMagicData[i]->GetInt(EM_EUDEMONMAGIC_dataID);
			//magicInfo1.nType = nDataTemp / 100;
			magicInfo1.nType = nDataTemp;
			magicInfo1.idOwner = this->GetType();
			magicInfo1.nLevel = nMagicLevel;
			magicInfo1.id = pMagicData[i]->GetInt(EM_EUDEMONMAGIC_id);
			magicInfo1.nHabit = pMagicData[i]->GetInt(EM_EUDEMONMAGIC_character);
			magicInfo1.nAffect = pMagicData[i]->GetInt(EM_EUDEMONMAGIC_affect);
			magicInfo1.usCDIndex = i;
			//if (pMagicData[i]->GetInt(EM_EUDEMONMAGIC_type)/100==1)//1��������2������
				m_pMagic->AppendMagic(&magicInfo1);
		}
	}
	CreateAllCD(false);//goto ����CD
	return true;
}
BOOL CMonster::UpLevEudemonMagic()
{
	CHECKF(m_pMagic);
	if(!IsEudemon())
		return FALSE;

	//���޸�
	CHECKF(m_pPetData);
	if(m_pPetData->GetInt(EUDEMONDATA_skill_1) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_1);
		m_pPetData->SetInt(EUDEMONDATA_skill_1,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}

	if(m_pPetData->GetInt(EUDEMONDATA_skill_2) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_2);
		m_pPetData->SetInt(EUDEMONDATA_skill_2,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}

	if(m_pPetData->GetInt(EUDEMONDATA_skill_3) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_3);
		m_pPetData->SetInt(EUDEMONDATA_skill_3,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}

	if(m_pPetData->GetInt(EUDEMONDATA_skill_4) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_4);
		m_pPetData->SetInt(EUDEMONDATA_skill_4,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}

	if(m_pPetData->GetInt(EUDEMONDATA_skill_5) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_5);
		m_pPetData->SetInt(EUDEMONDATA_skill_5,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}

	if(m_pPetData->GetInt(EUDEMONDATA_skill_6) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_6);
		m_pPetData->SetInt(EUDEMONDATA_skill_6,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}

	if(m_pPetData->GetInt(EUDEMONDATA_skill_7) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_7);
		m_pPetData->SetInt(EUDEMONDATA_skill_7,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}

	if(m_pPetData->GetInt(EUDEMONDATA_skill_8) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_8);
		m_pPetData->SetInt(EUDEMONDATA_skill_8,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}

	if(m_pPetData->GetInt(EUDEMONDATA_skill_9) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_9);
		m_pPetData->SetInt(EUDEMONDATA_skill_9,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}

	if(m_pPetData->GetInt(EUDEMONDATA_skill_10) != 0)
	{
		int nOldSkil = m_pPetData->GetInt(EUDEMONDATA_skill_10);
		m_pPetData->SetInt(EUDEMONDATA_skill_10,nOldSkil / 1000 * 1000 + m_pPetData->GetInt(EUDEMONDATA_level));
	}
	//�ڴ漼���޸�
	IMagicSet* pMagicSet = m_pMagic->QuerySet();
	if (!pMagicSet)
		return FALSE;
	int nMagicLevel = GetLev();
	for(int i = pMagicSet->GetAmount()-1; i >= 0; i--)
	{
		IMagicData* pData = pMagicSet->GetObjByIndex(i);
		if(pData)
		{
			pData->SetInt(MAGICDATA_LEVEL, nMagicLevel);
		}
	}
	return TRUE;
}
//--------------------------------------
BOOL CMonster::InitMagic()//���ܳ�ʼ��
{
	CHECKF(!m_pMagic);//δ��ʼ����

	//�õ������б�
	m_pMagic = CMagic::CreateNew(m_idProcess, this->QueryRole());
	CHECKF(m_pMagic);

	if(IsEudemon())
	{
		return true;
	}
	

	const VCT_NPCMAGICDATA* pVtcData = g_pNpcMagicManager->QueryDataByOwnerType(GetType());
	if(pVtcData)
	{
		for(int i=0;i<pVtcData->size();i++)
		{
			CNpcMagicData* pMagicData = (*pVtcData)[i];
			CHECKF(pMagicData);

			//�����ڴ�������sk_magic��copy
			MagicInfoStruct magicInfo;
			memset(&magicInfo,0,sizeof(MagicInfoStruct));
			magicInfo.nType = pMagicData->GetInt(EM_NPCMAGICDATA_magictype);
			magicInfo.idOwner = pMagicData->GetInt(EM_NPCMAGICDATA_ownertype);
			magicInfo.nLevel = pMagicData->GetInt(EM_NPCMAGICDATA_magiclev);
			//��sk_monstermagic��idαװsk_magic��id
			magicInfo.id = pMagicData->GetInt(EM_NPCMAGICDATA_id);

			m_pMagic->AppendMagic(&magicInfo);
			
		}
	}
	

	return true;
}

//satan ����������ӳ�޲�ͬ����NPC
bool CMonster::MyAddEnmitySynNPC(OBJID id,
								 int  nEnmAddi  = NULL,
								 BOOL bSyn      = true)
{
	//Satan 2009.07.29  �����ܶ��Լ���
	if(id == this->GetID())
		return true;
	this->AddEnmitySynNPC(id,10,1,nEnmAddi,bSyn);
	
	int amount = m_vEnmity.size();
	if(amount != 0)
	{
		for(int i = 0;i< amount;i++)		//�ڳ���б����������˺��������ˣ��ۼ�
		{
		    if(m_vEnmity[i].userID == id)	
			{
				m_vEnmity[i].nMax = 1;
			}
			else
				m_vEnmity[i].nMax = 0;
		}	
	}		

	IRole* pRole = RoleManager()->QueryRole(id);
	SetAttackTarget(pRole);
	
	return true;
}

//satan �������ܹ���
bool CMonster::BeEnmityAttack(bool bMagibec, IRole *pTarget,int addi/* = 0 */,bool bReflectEnable/* = true*/)
{
	CRole::DetachStatus(this->QueryRole(), STATUS_PALSY);
	
	MyAddEnmitySynNPC(pTarget->GetID(),addi);

	return true;
}

bool CMonster::ReleaseSleep()
{
	if(m_vEnmity.empty())
		return false;

	CUserPtr pTarget = NULL;
	this->FindAroundObj( OBJ_USER,m_vEnmity[ 0 ].userID,( void** )&pTarget );
	if( !pTarget )
		return false;

	this->SetAttackTarget(pTarget);

	return true;
}

//satan ���BUFFȥ����Ĵ���
bool CMonster::MyEnmityClr(void)
{
	OBJID id = 0;
	int amount = m_vEnmity.size();
	if(amount != 0)
	{
		for(int i = 0;i< amount;i++)		//�ڳ���б����������˺��������ˣ��ۼ�
		{
		    if(m_vEnmity[i].nMax == 1)	
			{
				id = m_vEnmity[i].userID;
				m_vEnmity[i].nMax = 0;
			}
		}	
	}		
	this->AddEnmitySynNPC(id,0,0,NULL,true);
	
	SET_ENMITY setEnmity = GetSortedEnmityListByMaxEnmity_enmAddi();
	if(0 != setEnmity.size())
	{
		IRole* pRole = RoleManager()->QueryRole(setEnmity[0].userID);
	    SetAttackTarget(pRole);
	}	
	return true;
}


void CMonster::MoveMonsterDest(UINT idMap,UINT nType,int x,int y)
{
	
}

// int CMonster::GetMaxLifeGrow()
// { 
// 	if(IsEudemon() && m_pData2)
// 	{
// 		return /*m_pData2->GetInt(EUDEMONDATA_maxlife_grow)*/0;
// 	}
// 	return 0;
// 
// }
DWORD CMonster::GetMana()
{
// 	if(IsEudemon() && m_pData2)
// 	{
// 		return m_pData2->GetInt(EUDEMONDATA_mana);
// 	}
	return 0;

}
DWORD CMonster::GetMaxMana()
{
// 	if(IsEudemon() && m_pData2)
// 	{
// 		return m_pData2->GetInt(EUDEMONDATA_maxmana) / 100 + m_EudemonGifeData.addmp;
// 	}
	return 0;
	
}
// int CMonster::GetMaxManaGrow()
// {
// 	if(IsEudemon() && m_pData2)
// 	{
// 		return /*m_pData2->GetInt(EUDEMONDATA_maxmana_grow)*/0;
// 	}
// 	return 0;
// 	
// }
// int CMonster::GetAttackGrow()
// {
// 	if(IsEudemon()/* && m_pData2*/)
// 	{
// 		return /*m_pData2->GetInt(EUDEMONDATA_attack_grow)*/0;
// 	}
// 	return 0;
// 	
// }
// int CMonster::GetAttackMagicGrow()
// {
// 	if(IsEudemon() && m_pData2)
// 	{
// 		return /*m_pData2->GetInt(EUDEMONDATA_attack_magic_grow)*/0;
// 	}
// 	return 0;
// }
// int CMonster::GetDefanceGrow()
// {
// 	if(IsEudemon()/* && m_pData2*/)
// 	{
// 		return /*m_pData2->GetInt(EUDEMONDATA_defence_grow)*/0;
// 	}
// 	return 0;
// 	
// }
// int CMonster::GetDefanceMagicGrow()
// {
// 	if(IsEudemon()/* && m_pData2*/)
// 	{
// 		return /*m_pData2->GetInt(EUDEMONDATA_defence_magic_grow)*/0;
// 	}
// 	return 0;
// 	
// }
int CMonster::GetSkill1()
{
	if(IsEudemon()/* && m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_1);
	}
	return 0;
	
}
int CMonster::GetSkill2()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_2);
	}
	return 0;
	
} 
int CMonster::GetSkill3()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_3);
	}
	return 0;
	
} 
int CMonster::GetSkill4()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_4);
	}
	return 0;
	
} 
int CMonster::GetSkill5()
{
	if(IsEudemon()/* && m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_5);
	}
	return 0;
	
} 
int CMonster::GetSkill6()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_6);
	}
	return 0;
}
int CMonster::GetSkill7()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_7);
	}
	return 0;	
}
int CMonster::GetSkill8()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_8);
	}
	return 0;	
}
int CMonster::GetSkill9()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_9);
	}
	return 0;	
}
int CMonster::GetSkill10()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_skill_10);
	}
	return 0;	
}
//��λ ��
int CMonster::GetSkillCD(int nCDIndex)
{
	if (!m_pPetData)
		return 0;

	int iType = 0;
	char thedata[24];
	if (nCDIndex==0)
	{
		sprintf(thedata, m_pPetData->GetStr(EUDEMONDATA_skill1_lastuse));
		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(this->GetSkill1()/1000);
		if (!pMagicData)
			return 0;
		iType  = pMagicData->GetInt(EM_EUDEMONMAGIC_dataID);
	}
	//ֻ��һ����������
// 	else if(nCDIndex==1)
// 	{
// 		sprintf(thedata, m_pPetData->GetStr(EUDEMONDATA_skill2_lastuse));
// 		CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonData(this->GetSkill2()/1000);
// 		if (!pMagicData)
// 			return 0;
// 		iType  = pMagicData->GetInt(EM_EUDEMONMAGIC_dataID);
// 	}
	else
		return 0;
	SYSTEMTIME thisTime;
	int dataCD,dataM,dataD,dataH,dataMo,dataS;
	if(6 == sscanf(thedata,"%d-%d-%d-%d-%d-%d",&dataCD,&dataM,&dataD,&dataH,&dataMo,&dataS))
	{
		::GetLocalTime(&thisTime);
		int iLevel	= GetLev();	
		
		int time = 0,temp1 =0,temp2 =0,temp3 =0;
		if(dataCD > 0)
		{
			if(dataM == thisTime.wMonth  &&  dataD == thisTime.wDay  &&  dataH <= thisTime.wHour)
			{
				temp1 = thisTime.wHour   - dataH;
				temp2 = thisTime.wMinute - dataMo;
				temp3 = thisTime.wSecond - dataS;
				//�������
				time = temp1 * 60 * 60 + temp2 * 60 + temp3 ;
				if(time > 0 && dataCD - time > 1)
				{
					return dataCD - time;
				}					
			}
		}
	}
	return 0;
}
void CMonster::CreateAllCD(bool bSynchro)
{
	if (!m_pMagic)
		return;
	if(!IsEudemon())
		return;
	IMagicSet* pMagicSet = m_pMagic->QuerySet();
	if (!pMagicSet)
		return;
	for(int i = pMagicSet->GetAmount()-1; i >= 0; i--)
	{
		IMagicData* pData = pMagicSet->GetObjByIndex(i);
		if(pData)
		{
			int nTime = GetSkillCD(pData->GetIndex());
			if (nTime>0)
			{
				CMsgCoolDown msg;
				if( msg.Create(GetID(),MAGICCOOLDOWN_BEGIN,MAGIC_INFO,pData->GetInt(MAGICDATA_TYPE),nTime * 1000,0))
				{
					if (bSynchro)
						m_pOwner->SendMsg( &msg );
					pData->QueryDelayObj().SetInterval(nTime*1000);
					pData->StartDelay();
				}
			}
		}
	}
}
void CMonster::SetMagicLastuse(EUDEMONDATA idx, LPCTSTR szData, int nSize, bool bUpdate)
{
	m_pPetData->SetStr(idx,szData,nSize,bUpdate);
}
int CMonster::GetIdentity1()
{
	if(IsEudemon()/* && m_pData2*/)
	{
		return /*m_pData2->GetInt(EUDEMONDATA_identity_1)*/0;
	}
	return 0;
	
}
int CMonster::GetIdentity2()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return /*m_pData2->GetInt(EUDEMONDATA_identity_2)*/0;
	}
	return 0;
	
}
int CMonster::GetIdentity3()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return /*m_pData2->GetInt(EUDEMONDATA_identity_3)*/0;
	}
	return 0;
	
}
int CMonster::GetIdentity4()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return /*m_pData2->GetInt(EUDEMONDATA_identity_4)*/0;
	}
	return 0;
	
}
int CMonster::GetIdentity5()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return /*m_pData2->GetInt(EUDEMONDATA_identity_5)*/0;
	}
	return 0;
	
}
int CMonster::GetIdentity6()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return /*m_pData2->GetInt(EUDEMONDATA_identity_6)*/0;
	}
	return 0;
	
}
int CMonster::GetHabit()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_habit);
//		CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(m_pData2->GetInt(EUDEMONDATA_typeid));
// 		if(pEudemonType)
// 			return pEudemonType->GetInt(EUDEMONTYPEDATA_habit);
	}
	return 0;
}

int CMonster::GetEudemonType()
{
	if(IsEudemon()/* && m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_typeid);
	}
	return 0;
}

int CMonster::GetSex()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_sex);
	}
	return 0;
	
}
int CMonster::GetPosition()
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		return /*m_pData2->GetInt(EUDEMONDATA_stutes)*/0;
	}
	return 0;
}

OBJID CMonster::GetHpTimes()
{
	if(IsEudemon() /*&& m_pData2*/)
		return /*m_pData2->GetInt(EUDEMONDATA_hp_times)*/0;
	return 0;
}

OBJID CMonster::GetMagTimes()
{
	if(IsEudemon() /*&& m_pData2*/)
		return /*m_pData2->GetInt(EUDEMONDATA_mag_times)*/0;
	return 0;
}

OBJID CMonster::GetPhyTimes()
{
	if(IsEudemon()/* && m_pData2*/)
		return /*m_pData2->GetInt(EUDEMONDATA_phy_times)*/0;
	return 0;
}

OBJID CMonster::GetDefTimes()
{
	if(IsEudemon() /*&& m_pData2*/)
		return /*m_pData2->GetInt(EUDEMONDATA_def_times)*/0;
	return 0;
}

OBJID CMonster::GetPlayTimes()
{
	if(IsEudemon()/* && m_pData2*/)
	{
		return m_pPetData->GetInt(EUDEMONDATA_playtime);
	}
	return 0;
}
CTeam* CMonster::GetTeam()
{
	if (IsEudemon())
	{
		CUser* pUser = QueryOwnerUser();
		if (pUser)
			return pUser->GetTeam();
	}
	return NULL;
}
//add by xumiao 2010-1-8
DWORD CMonster::GetMoveType()									
{ 
	if(IsEudemon() /*&& m_pData2*/)
	{
		return /*m_pData2->GetInt(EUDEMONDATA_movetype)*/0;
	}

	return 0;
}
DWORD CMonster::GetAttackType()
{ 
	if(IsEudemon()/* && m_pData2*/)
	{
		return /*m_pData2->GetInt(EUDEMONDATA_attacktype)*/0;
	}
	return 0;
}

void CMonster::SetMoveType( DWORD data , bool isRecord )
{
	if(IsEudemon() /*&& m_pData2*/)
	{
		//m_pData2->SetInt( EUDEMONDATA_movetype , data );

		if( isRecord )
			m_pPetData->SaveData();
	}

}

void CMonster::SetAttackType(DWORD data , bool isRecord)
{
	if(IsEudemon() /*&& m_pData2*/)
	{	
		//m_pData2->SetInt( EUDEMONDATA_attacktype , data );
		
		if( isRecord )
			m_pPetData->SaveData();
	}
}

void CMonster::CheckEudeHappy()
{
	if(IsEudemon())
	{
		if(!m_pPetData)
			return;

		if(!m_pOwner)
			return;

		DWORD dwSpan = clock() - m_lastEudemonHappyCheck;
		if(dwSpan >= 600000)
		{
 			m_lastEudemonHappyCheck += 600000;
			
			
			int nOldHappy = m_pPetData->GetInt(EUDEMONDATA_happy);
			int nNewHappy = max(nOldHappy - 1,0);
			if(nNewHappy != nOldHappy)
			{
				m_pPetData->SetInt(EUDEMONDATA_happy,nNewHappy,true);
						
				CMsgEudemonAttrib msg;
				if(msg.Create(m_pPetData->GetID(),EUDATTR_HAPPY,nNewHappy))
					m_pOwner->SendMsg(&msg);
						
				if(nNewHappy < 60)
				{
					m_pOwner->SendSysMsg("��ǰ��ս�������ֵ����");
					m_pOwner->CallBackEudemon(m_pPetData->GetID(),true,true,false);
				}
			}
		}
	}
}
//end

void CMonster::AddBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )
{
	if( nStatus != ID_NONE )
	{
		if( BIGTYPE_OTHER == nStatus / 10000 )//��BUFF��DEBUFF��DOT
		{
			return;
		}
		CMsgBuff msg;
		if( msg.Create( ACTION_ADDSTATUS,this->GetID(),nStatus,nTime,nTimes,nPower,nParam1,nParam2,nParam3 ) )
		{
			BroadcastRoomMsg( &msg,EXCLUDE_SELF );
		}
		if(IsEudemon())
		{
			UpdateMonsterProperty();
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------
//07.4.26
//�����޸�
//07.4.28���񲨵ڶ����޸ģ����ӷ�BUFF��DEBUFF��DOT�ж�				
void CMonster::UpdateBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )
{
	if( nStatus != ID_NONE )
	{
		if( BIGTYPE_OTHER == nStatus / 10000 )//��BUFF��DEBUFF��DOT
		{
			return;
		}
		CMsgBuff msg;
		if( msg.Create( ACTION_UPDATESTATUS,this->GetID(),nStatus,nTime,nTimes,nPower,nParam1,nParam2,nParam3 ) )
		{
			BroadcastRoomMsg( &msg,EXCLUDE_SELF );
		}
		if(IsEudemon())
		{
			UpdateMonsterProperty();
		}
	}
	
}
//-------------------------------------------------------------------------------------------------------------------------
//07.4.26
//�����޸�
//07.4.28���񲨵ڶ����޸ģ����ӷ�BUFF��DEBUFF��DOT�ж�				
void CMonster::ClsBuff( int nStatus ) 
{
	if( nStatus != ID_NONE )
	{
		if( BIGTYPE_OTHER == nStatus / 10000 )//��BUFF��DEBUFF��DOT
		{
			return;
		}
		CMsgBuff msg;
		if( msg.Create( ACTION_DELSTATUS,this->GetID(),nStatus,0,0,0,0,0,0 ) )
		{
			BroadcastRoomMsg( &msg,EXCLUDE_SELF );
		}
		if(IsEudemon())
		{
			UpdateMonsterProperty();
		}
	}
}

int CMonster::GetUnderstand()
{
	if(!m_pPetData)
		return NULL;
	
	if(!m_pOwner)
		return NULL;
	if (this->IsEudemon())
	{
		return m_pPetData->GetInt(EUDEMONDATA_understand);
	}
	return 0;
}

int CMonster::GetGetType()
{
	if(!m_pPetData)
		return NULL;
	
	if(!m_pOwner)
		return NULL;

	if (this->IsEudemon())
	{
		return m_pPetData->GetInt(EUDEMONDATA_gettype);
	}
	return 0;
}

//goto ���³��������������
bool CMonster::UpdateMonsterProperty(bool bIncludeBuff/*=true*/)
{
	if (!IsEudemon() || !QueryOwnerUser())
		return false;
	CMsgEudemonAttrib msg;
	if(!msg.Create(GetID(), EUDATTR_NULL, 0))
		return false;

	if (!m_pPetData)
		return false;
	CUserPet* pUserPetSet = QueryOwnerUser()->QueryUserPetSet();
	if (!pUserPetSet)
		return false;

	EudemonData data = {0};
	if (bIncludeBuff)
	{
		m_pPetData->GetData(data);
		pUserPetSet->fillType(data,3);
		pUserPetSet->fillAtt(data,3);
		memcpy(&m_eudemonData,&data,sizeof(EudemonData));
	}
	else
		memcpy(&data,&m_eudemonData,sizeof(EudemonData));

	msg.Append(EUDATTR_STR,data.dwStr);
	msg.Append(EUDATTR_INT,data.dwInt);
	msg.Append(EUDATTR_STA,data.dwSta);
	msg.Append(EUDATTR_SPI,data.dwSpi);
	msg.Append(EUDATTR_AGI,data.dwAgi);

	int nLife = min(data.dwMaxLife, GetLife());
	msg.Append(EUDATTR_LIFE, nLife);
	msg.Append(EUDATTR_MAXLIFE,data.dwMaxLife);
	msg.Append(EUDATTR_ATK,data.dwPhyAtt);
	msg.Append(EUDATTR_MAGICATK,data.dwMagAtt);
	msg.Append(EUDATTR_DEF,data.dwPhyDef);
	msg.Append(EUDATTR_MAGICDEF,data.dwMagDef);
	msg.Append(EUDATTR_HIT,data.dwhit);
	msg.Append(EUDATTR_DOGE,data.dwDoge);
	msg.Append(EUDATTR_CRIT,data.dwCrit);
	msg.Append(EUDATTR_TOUGH,data.dwTough);
	QueryOwnerUser()->SendMsg(&msg);

	return true;
}
//ʦͽ ��������
void CMonster::AllotTutorPreach(CUserPtr pGetUser)
{
	CHECK(pGetUser);

	int nStep = 0;
	DEBUG_TRY
	OBJID idAction = 1004;
	CTeam* pTempTeam = pGetUser->GetTeam();	
	nStep = 1;
	if( pTempTeam && pTempTeam->GetOnlineMemberAmount()>1)
	{
		nStep = 2;
		OBJID idLeader = pTempTeam->GetLeader();
		nStep = 3;
		int nTeamAmount = pTempTeam->GetMemberAmount();
		nStep = 4;
		if (idLeader==pGetUser->GetID() && pGetUser->GetStudentAmount(false)>0)//�Ƕӳ�������ͽ��
		{
			nStep = 5;
			LuaScriptMachine()->InitVar(GetLev());
			nStep = 6;
			pGetUser->ProcessAction(idAction);
		}
	}
	DEBUG_CATCH2("CMonster::AllotTutorPreach() nStep = %d",nStep)
}

void CMonster::AllotKill2User(CUserPtr pGetUser)
{
	int nStep = 0;
	CHECK(pGetUser);
	DEBUG_TRY	

	int type = GetType();
	int add = type;
	if(add > 3000)
	{
		add = 3000 + ((type - 3001 ) / 30) * 30;
		if(add == 5070)
			add = 3540;
	}

	nStep = 1;
	if(pGetUser->GetTeam())
	{
		nStep = 2;
		CUser* pMember =NULL;
		for(int i = 0 ;i<_MAX_TEAMAMOUNT ; i++)
		{
			nStep = 3;
			if(pGetUser->GetTeam()->m_TeamIn[i].id == ID_NONE)///�޶�Աid������
				continue;
			pMember = UserManager()->GetUser(pGetUser->GetTeam()->m_TeamIn[i].id);
			if(pMember==NULL)///��ԱΪ�ղ�����
				continue;
			
			nStep = 4;
			if(pGetUser->GetMapID() != pMember->GetMapID())///���������Ա����ͬһ��ͼ������
				continue;
				
			nStep = 5;
			pMember->UpdateMissionMsgByMonsterType(add);
			pMember->JudgeMonsterKill(1);
		}
	}
	else
	{
		nStep = 6;
		pGetUser->UpdateMissionMsgByMonsterType(add );
		pGetUser->JudgeMonsterKill(1);
	}

	//���˹���
// 	if (5161 <= type && type <= 5189)
// 	{
// 		if(pGetUser->GetInstenceType() == 2011)
// 		{
// 			char UserName[128] = "";
// 			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, pGetUser->GetName());
// 			char	szMsg[256] = "";
// 			sprintf(szMsg, "����Ӣ�۶�Ů<%s>����Э�����������ޣ�ӭ���˼�����������꣡��", UserName);
// 			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pGetUser, 0, 0, "" , ALLUSERS_NAME);
// 		}
// 	}

	DEBUG_TRY
	nStep = 7;
	if(!pGetUser->GetTeam())
		return;
	nStep = 8;
	char teamLeader[64]  = "";		
	if (type > 3030 && type < 3061)
	{
		nStep = 9;
		char UserName[128] = "";
		CUser* pLeader = UserManager()->GetUser(pGetUser->GetTeam()->GetLeader());
		if(pLeader)
		{
			strcpy(teamLeader, pLeader->GetName());
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, teamLeader);

			int flag = RandGet(2);
			char	szMsg[256] = "";
			if (flag == 1)
				sprintf(szMsg, "ʤ���������߿ܣ��ڶӳ�<%s>��ͳ���£�ɽ��ͷĿ��ŭն�䵶�£�<3_ɽ������_6>�������������֣�", UserName);
			else
				sprintf(szMsg, "����ɽ�����£�<%s>������飬��Ѹ�ײ����ڶ�֮�ƣ�ŭնɽ��ͷĿ���õ���<3_ɽ������_6>���ⷬΪ����������Ǵ�����ģ�", UserName);

			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pGetUser, 0, 0, "" , ALLUSERS_NAME);
		}
	}
	else if (3180 < type && 3361 > type)
	{
		nStep = 10;
		char UserName[128] = "";
		CUser* pLeader = UserManager()->GetUser(pGetUser->GetTeam()->GetLeader());
		if(pLeader)
		{
			strcpy(teamLeader, pLeader->GetName());
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, teamLeader);

			int flag = RandGet(3);
			char	szMsg[256] = "";
			if (flag == 0)
				sprintf(szMsg, "<%s>������飬�����˽�����Բ�����Ļ����ͽ��ͬʱ��<3_��ͽ�ı���_6>�չ鼺�С�", UserName);
			else if(flag == 1)
				sprintf(szMsg, "������������֮�ʣ�<%s>������Σ��֮ʱ��Я�����ͣ�������а֮�����ս�<3_��ͽ�ı���_6>��һʱ������ġ�", UserName);
			else 
				sprintf(szMsg, "����<%s>����Ⱥ������ս��а֮�����е������£��ս���<3_��ͽ�ı���_6>�������������Σ�ѣ���������˫�գ��ֲ������", UserName);

			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pGetUser, 0, 0, "" , ALLUSERS_NAME);
		}
	}
	else if ((3540 < type && 3571 > type) || (5101 > type && 5072 < type))
	{
		nStep = 11;
		char UserName[128] = "";
		CUser* pLeader = UserManager()->GetUser(pGetUser->GetTeam()->GetLeader());
		if(pLeader)
		{
			strcpy(teamLeader, pLeader->GetName());
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, teamLeader);
			char	szMsg[256] = "";
			sprintf(szMsg, "<%s>�ʶ��ں��ϳ������,������Ұ�Ĳ�����<3_�츣ħӰ_6>��ƽϢ�˺���֮�ң���ȥ����<3_�츣�ı���_6>�ж���ʲô�ö����ɣ�", UserName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pGetUser, 0, 0, "" , ALLUSERS_NAME);
		}
	}
	else if (3692 < type && 3721 > type)
	{
		nStep = 12;
		char UserName[128] = "";
		CUser* pLeader = UserManager()->GetUser(pGetUser->GetTeam()->GetLeader());
		if(pLeader)
		{
			strcpy(teamLeader, pLeader->GetName());
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, teamLeader);
			char	szMsg[256] = "";
			sprintf(szMsg, "<%s>�ʶ���Ͷ����ƣ����׹Ƽ�̳����<3_а�����ʦ_6>��<3_�����_6>��һʱ�����������˳��ޣ��������£�", UserName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pGetUser, 0, 0, "" , ALLUSERS_NAME);
		}
	}
	else if (3960 < type && 3991 > type)
	{
		nStep = 13;
		char UserName[128] = "";
		CUser* pLeader = UserManager()->GetUser(pGetUser->GetTeam()->GetLeader());
		if(pLeader)
		{
			strcpy(teamLeader, pLeader->GetName());
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, teamLeader);
			char	szMsg[256] = "";
			sprintf(szMsg, "<%s>���Ӵ���˻�Į����<3_��һ��_6>�������±����е��������ϣ����ڳ���Ӣ�ۺ��ܶ�ϲЦ�տ�����һ�˻�Į֮�ù�Ȼ�����ض��飡", UserName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pGetUser, 0, 0, "" , ALLUSERS_NAME);
		}
	}
//modify code by �ֵ�Ԩ begin 2011.6.22
	else if ( type > 6300 && type <= 6330 )
	{
		nStep = 11;
		char UserName[128] = "";
		CUser* pLeader = UserManager()->GetUser(pGetUser->GetTeam()->GetLeader());
		if(pLeader)
		{
			strcpy(teamLeader, pLeader->GetName());
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, teamLeader);
			char	szMsg[256] = "";
			sprintf(szMsg, "���鵺������������������<%s>�����С����ǧ�����գ����ڴ�ܾ��鵺�������ϱ��ﾡ�����£�", UserName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pGetUser, 0, 0, "" , ALLUSERS_NAME);
		}
	}
//modify code by �ֵ�Ԩ end
	DEBUG_CATCH("ProcessAction(20000 + add)")
	DEBUG_CATCH2("AllotKill2User nStep = %u",nStep)
}
BOOL CMonster::RunScript(const char *szFileName)
{
	bool hr=false;
	DEBUG_TRY
	RegisterLuaInterface();
	hr=LuaScriptMachine()->RunScriptFile(szFileName);
	///��ǰ�����ű�����
	if(LuaScriptMachine()->IsHasError())
		LOGERROR(LuaScriptMachine()->GetLastError());
	//		SendInstanceMsg(LuaScriptMachine()->GetLastError());
	DEBUG_CATCH("CMapInstance::RunScriptMonsterKill")
		return hr;
}
void CMonster::RegisterLuaInterface()
{
	LuaScriptMachine()->RegisterObjectDirect("Monster","CMonster","GetMonsterID",this,&CMonster::GetType);
	LuaScriptMachine()->RegisterObjectDirect("Monster","CMonster","GetMonsterPosx",this,&CMonster::GetPosX);
	LuaScriptMachine()->RegisterObjectDirect("Monster","CMonster","GetMonsterPosy",this,&CMonster::GetPosY);
	LuaScriptMachine()->RegisterObjectDirect("Monster","CMonster","GetMonsterSynId",this,&CMonster::GetMonsterInSynID);
}

float CMonster::GetQualityParam(int nQuality)
{
	float giftflag = 0;
	if(nQuality>=0 && nQuality<=1000)
	{
		giftflag=1.1f;
	}
	else if(nQuality>=1001 && nQuality<=1500)
	{
		giftflag=1.3f;
	}
	else if(nQuality>=1501 && nQuality<=2000)
	{
		giftflag=1.6f;
	}
	else if(nQuality>=2001 && nQuality<=2500)
	{
		giftflag=2.1f;
	}
	else if(nQuality>=2501 && nQuality<=3000)
	{
		giftflag=2.7f;
	}
	else if(nQuality>=3001 && nQuality<=3500)
	{
		giftflag=3.3f;
	}
	else if(nQuality>=3501)
	{
		giftflag=4.0f;
	}
	return giftflag;
}

float CMonster::GetFusionParam(int nFusion)
{
	float fsflag = 0;
	switch(nFusion)
	{
	case 1:
	case 2:
		fsflag=1.1f;
		break;
	case 3:
	case 4:
		fsflag=1.3f;
		break;
	case 5:
	case 6:
		fsflag=1.8f;
		break;
	case 7:
	case 8:
		fsflag=2.5f;
		break;
	case 9:
		fsflag=3.5f;
		break;
	case 10:
		fsflag=4.2f;
		break;	
	}
	return fsflag;
}

bool CMonster::FindPath(int x, int y,bool CheckFloor/* = true*/)
{
// 	if(!IsMoveEnable())
// 		return false;
	
	int x1 = GetPosX();
	int y1 = GetPosY();
	if(x == GetPosX() && y == GetPosY())
		return false;
	if(x == m_posTarget.x && y == m_posTarget.y)
		return false;
	if(!GetMap())
		return false;

	ClearPath();
	
	char	buf[_MAX_PATHSTEP + sizeof(PASSPATH0)];
	PASSPATH0* pPath = (PASSPATH0*)buf;
	POINT	posSource, posTarget;
	posSource.x	= GetPosX();
	posSource.y	= GetPosY();
	posTarget.x	= x;
	posTarget.y	= y;

	try{
		GetMap()->QueryMapData()->FindPath(pPath, sizeof(buf), posSource, posTarget, CheckFloor);
	}catch(...)
	{
		LOGWARNING("GetMap()->QueryMapData()->FindPath() docid = %u sx = %d sy = %d tx = %d ty = %d",GetMap()->GetDocID(),posSource.x,posSource.y,posTarget.x,posTarget.y);
		return false;
	}

	m_posTarget		= pPath->posStop;
	memcpy(m_bufPath, pPath->pPath, pPath->nSize);
	m_nPathLen		= pPath->nSize;
	
	return m_nPathLen > 0;
}


int	CMonster::CheckAIStatus(int nStatus)
{
	if(!IsAlive())
		return EM_AISTATE_Die;

	if(IsDeleted())
		return EM_AISTATE_Die;

	switch(nStatus)
	{
	case EM_AISTATE_NONE:
		return EM_AISTATE_Initialize;
		break;
	case EM_AISTATE_Initialize:
		if(m_nSeeUserNum > 0)
			return EM_AISTATE_Wander;
		break;
	case EM_AISTATE_Die:
		break;		
	case EM_AISTATE_Wander:
		{
			//����,��λ
			if(m_nSeeUserNum <= 0)
				return EM_AISTATE_SoonBack;
			if ((m_pType->GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_NOWANDER) != 0)		// ������ һֱ�й�
			{
				m_idTarget = ID_NONE;
				return EM_AISTATE_Wander;
			}
			//����,ҧ��
			SET_ENMITY setEnmity = GetSortedEnmityListByMaxEnmity_enmAddi();
			if(0 != setEnmity.size())
			{
				m_idTarget = setEnmity[0].userID;
				return EM_AISTATE_Chase;
			}			
			//������,���˾������ޣ�ҧ			
			if((m_pType->GetInt(NPCTYPEDATA_ATKUSER)& ATKUSER_ACTIVE) != 0) //����
			{
				m_idTarget = FindAttackUser(m_pType->GetInt(NPCTYPEDATA_VIEWRANGE));
				if(m_idTarget != ID_NONE)
				{
					AddEnmity(m_idTarget,10,0);
					return EM_AISTATE_Chase;
				}
			}
			//Զ��,��λ
			if(GetPosX() < m_nBornX - 5//x��С
				|| GetPosX() > m_nBornX + 5//x����
				|| GetPosY() < m_nBornY - 5//y��С
				|| GetPosY() > m_nBornY + 5)//y����
				return EM_AISTATE_SoonBack;
		}
		break;
	case EM_AISTATE_Chase:
		{
			int nStep = 0;
			DEBUG_TRY
			IRole* pRole = RoleManager()->QueryRole(m_idTarget);
			//û����,ѡ����Ŀ����
			if(!pRole)
			{
				ClsEnmity(m_idTarget);
				return EM_AISTATE_Choose;
			}
			nStep = 1;
			if(!pRole->IsAlive())
			{
				ClsEnmity(m_idTarget);
				return EM_AISTATE_Choose;
			}
			nStep = 2;
			if(!pRole->GetMap())
			{
				ClsEnmity(m_idTarget);
				return EM_AISTATE_Choose;
			}
			nStep = 3;
			if(pRole->GetMap()->GetID() != GetMap()->GetID())
			{
				ClsEnmity(m_idTarget);
				return EM_AISTATE_Choose;
			}
			nStep = 4;
			//�����룬��λ
			if(GetPosX() < m_nBornX - m_pType->GetInt(NPCTYPEDATA_CHASE_RANGE)//x��С
				|| GetPosX() > m_nBornX + m_pType->GetInt(NPCTYPEDATA_CHASE_RANGE)//x����
				|| GetPosY() < m_nBornY - m_pType->GetInt(NPCTYPEDATA_CHASE_RANGE)//y��С
				|| GetPosY() > m_nBornY + m_pType->GetInt(NPCTYPEDATA_CHASE_RANGE))//y����
				return EM_AISTATE_SoonBack;

			nStep = 5;
			if(!CheckTargetEnmity(m_idTarget)) //��ǰ�ǵ�һ��ޣ�ѡ��
				return EM_AISTATE_Choose;

			nStep = 6;

			//׷������
			if(pRole->GetPosX() >= GetPosX() - GetAttackRange(0)//x��С
				&& pRole->GetPosX() <= GetPosX() + GetAttackRange(0)//x����
				&& pRole->GetPosY() >= GetPosY() - GetAttackRange(0)//y��С
				&& pRole->GetPosY() <= GetPosY() + GetAttackRange(0))//y����
				return EM_AISTATE_Atk;
			DEBUG_CATCH2("CheckAIStatus EM_AISTATE_Chase nStep = %d",nStep);
		}
	case EM_AISTATE_Choose:
		{
			int nStep = 0;
			DEBUG_TRY
			//�г�ޣ�׷
			SET_ENMITY setEnmity = GetSortedEnmityListByMaxEnmity_enmAddi();
			nStep = 1;
			if(0 != setEnmity.size())
			{
//modify code by �ֵ�Ԩ begin 2011.6.22
//				m_idTarget = setEnmity[0].userID;
//				return EM_AISTATE_Chase;
				if( GetType() > 6100 && GetType() <= 6130 )
				{
					nStep = 2;
					IStatus* pStatus = this->QueryStatus(STATUS_RETORT_TENSEC);
					if( pStatus )
					{
						DetachStatus(this,STATUS_RETORT_TENSEC);
					}

					nStep = 3;
					typedef std::vector<IRole*>	ROLE_SET;
					ROLE_SET		setRole;
					ROLE_SET		setRole_one;
					ROLE_SET		setRole_two;
					ROLE_SET		setRole_three;
					POINT	pos;
					setRole.clear();
					setRole_one.clear();
					setRole_two.clear();
					setRole_three.clear();

					nStep = 4;
					if( this->QueryMagic() )
						this->QueryMagic()->CollectTargetSet_BuffBomb(setRole, pos, 2,11);// ����Ŀ�꼯


					nStep = 5;
					for (int i = 0; i < setRole.size(); i++)
					{
						if (!setRole[i] || !setRole[i]->IsAlive())
							continue;
							
						switch( setRole[i]->GetProfession() )
						{
						case 1://��
						case 4://��
							setRole_three.push_back(setRole[i]);
							break;
						case 2://ȫ
						case 32://��
							setRole_two.push_back(setRole[i]);
							break;
						case 8://ؤ
						case 16://��
							setRole_one.push_back(setRole[i]);
							break;
						default:
							setRole_three.push_back(setRole[i]);
						}
					}

					nStep = 6;
					if( this->GetLife() >= this->GetMaxLife()/50*33 )
					{
						nStep = 7;
						if( setRole_one.size() > 0 )
						{
							if( setRole_one.size() == 1 )
								m_idTarget = setRole_one[0]->GetID();
							else
							{
								if( setRole_one[0]->GetID() == m_idTargetBoss )
									m_idTarget = setRole_one[1]->GetID();
								else
									m_idTarget = setRole_one[0]->GetID();
							}
						}
						else if( setRole_two.size() > 0 )
						{
							if( setRole_two.size() == 1 )
								m_idTarget = setRole_two[0]->GetID();
							else
							{
								if( setRole_two[0]->GetID() == m_idTargetBoss )
									m_idTarget = setRole_two[1]->GetID();
								else
									m_idTarget = setRole_two[0]->GetID();
							}
						}
						else if( setRole_three.size() > 0 )
						{
							if( setRole_three.size() == 1 )
								m_idTarget = setRole_three[0]->GetID();
							else
							{
								if( setRole_three[0]->GetID() == m_idTargetBoss )
									m_idTarget = setRole_three[1]->GetID();
								else
									m_idTarget = setRole_three[0]->GetID();
							}
						}
					}
					else if( this->GetLife() < this->GetMaxLife()/100*33 )
					{
						nStep = 8;
						if( setRole_three.size() > 0 )
						{
							if( setRole_three.size() == 1 )
								m_idTarget = setRole_three[0]->GetID();
							else
							{
								if( setRole_three[0]->GetID() == m_idTargetBoss )
									m_idTarget = setRole_three[1]->GetID();
								else
									m_idTarget = setRole_three[0]->GetID();
							}
						}
						else if( setRole_one.size() > 0 )
						{
							if( setRole_one.size() == 1 )
								m_idTarget = setRole_one[0]->GetID();
							else
							{
								if( setRole_one[0]->GetID() == m_idTargetBoss )
									m_idTarget = setRole_one[1]->GetID();
								else
									m_idTarget = setRole_one[0]->GetID();
							}
						}
						else if( setRole_two.size() > 0 )
						{
							if( setRole_two.size() == 1 )
								m_idTarget = setRole_two[0]->GetID();
							else
							{
								if( setRole_two[0]->GetID() == m_idTargetBoss )
									m_idTarget = setRole_two[1]->GetID();
								else
									m_idTarget = setRole_two[0]->GetID();
							}
						}
					}
					else
					{
						nStep = 9;
						if( setRole_two.size() > 0 )
						{
							if( setRole_two.size() == 1 )
								m_idTarget = setRole_two[0]->GetID();
							else
							{
								if( setRole_two[0]->GetID() == m_idTargetBoss )
									m_idTarget = setRole_two[1]->GetID();
								else
									m_idTarget = setRole_two[0]->GetID();
							}
						}
						else if( setRole_three.size() > 0 )
						{
							if( setRole_three.size() == 1 )
								m_idTarget = setRole_three[0]->GetID();
							else
							{
								if( setRole_three[0]->GetID() == m_idTargetBoss )
									m_idTarget = setRole_three[1]->GetID();
								else
									m_idTarget = setRole_three[0]->GetID();
							}
						}
						else if( setRole_one.size() > 0 )
						{
							if( setRole_one.size() == 1 )
								m_idTarget = setRole_one[0]->GetID();
							else
							{
								if( setRole_one[0]->GetID() == m_idTargetBoss )
									m_idTarget = setRole_one[1]->GetID();
								else
									m_idTarget = setRole_one[0]->GetID();
							}
						}
					}
					nStep = 10;
					AddEnmity(m_idTarget,5,0);
					nStep = 11;
					AttachStatus(this,STATUS_RETORT_TENSEC, 0, 10, 0,0,0,0,GetID());
					return EM_AISTATE_Chase;
				}
				else
				{
					nStep = 12;
					m_idTarget = setEnmity[0].userID;
					return EM_AISTATE_Chase;
				}
//modify code by �ֵ�Ԩ end
			}
			//û��ޣ���λ
			return EM_AISTATE_SoonBack;
			DEBUG_CATCH2("CheckAIStatus EM_AISTATE_Choose nStep = %d",nStep);
		}
		break;
	case EM_AISTATE_SoonBack:
		{
			//������  ����
			if(GetPosX() >= m_nBornX - 1//x��С
				&& GetPosX() <= m_nBornX + 1//x����
				&& GetPosY() >= m_nBornY - 1//y��С
				&& GetPosY() <= m_nBornY + 1)//y����
				return EM_AISTATE_Initialize;
		}
		break;
	case EM_AISTATE_Atk:
		{
			IRole* pRole = RoleManager()->QueryRole(m_idTarget);
			//û���� ѡ����Ŀ����
			if(!pRole)
			{
				ClsEnmity(m_idTarget);
				return EM_AISTATE_Choose;
			}
			if(!pRole->IsAlive())
			{
				ClsEnmity(m_idTarget);
				return EM_AISTATE_Choose;
			}
			if(!pRole->GetMap())
			{
				ClsEnmity(m_idTarget);
				return EM_AISTATE_Choose;
			}
			if(pRole->GetMap()->GetID() != GetMap()->GetID())
			{
				ClsEnmity(m_idTarget);
				return EM_AISTATE_Choose;
			}

			if(!CheckTargetEnmity(m_idTarget)) //��ǰ�ǵ�һ��ޣ�ѡ��
				return EM_AISTATE_Choose;

			
			//�����룬׷
			if(pRole->GetPosX() < GetPosX() - GetAttackRange(0)//x��С
				|| pRole->GetPosX() > GetPosX() + GetAttackRange(0)//x����
				|| pRole->GetPosY() < GetPosY() - GetAttackRange(0)//y��С
				|| pRole->GetPosY() > GetPosY() + GetAttackRange(0))//y����
				return EM_AISTATE_Chase;
		}
		break;
	}
	return nStatus;
}

OBJID CMonster::FindAttackUser(int nRange)
{
	return CRole::FindAttackUser(nRange);
}

void CMonster::ProcessAIStatus(int nStatus)
{
	switch(nStatus)
	{
	case EM_AISTATE_NONE:
	case EM_AISTATE_Initialize:
	case EM_AISTATE_Die:
		break;

	case EM_AISTATE_Wander:
		ProcessWander();
		break;
	case EM_AISTATE_Chase:
		ProcessChase();
 		break;
	case EM_AISTATE_Atk:
		ProcessAtk();
		break;
	case EM_AISTATE_SoonBack:
		ProcessSoonBack();
		break;
	}
}

void CMonster::EnterAIStatus(int nStatus)
{
	ClearPath();
	
	switch(nStatus)
	{
	case EM_AISTATE_Choose:
		{
			m_idTarget = ID_NONE;
		}
		break;
	case EM_AISTATE_SoonBack:
		{
			m_dwMoveInterTime = 400;
		}
		break;
	case EM_AISTATE_Atk:
		{
			IRole* pRole = RoleManager()->QueryRole(m_idTarget);
			CHECK(pRole);
			SetAttackTarget(pRole);
		}
		break;
	case EM_AISTATE_Chase:
		{
			//modify code by �ֵ�Ԩ begin 2011.6.22
			m_idTargetBoss = m_idTarget;
			//modify code by �ֵ�Ԩ end
		}
		break;
	}
}

void CMonster::LeaveAIStatus(int nStatus)
{
	switch(nStatus)
	{
	 	case EM_AISTATE_SoonBack:
			{
				//˲���Ѫ
				int nLifeAdd = GetMaxLife() - GetLife();
				AddAttrib(_USERATTRIB_LIFE,	nLifeAdd,SYNCHRO_BROADCAST);//�㲥
				ClsEnmityListSynNpc(true);
				m_idTarget = ID_NONE;
				m_dwMoveInterTime = m_pType->GetInt( NPCTYPEDATA_RUNSPEED );
			}
			break;
	}
}

void CMonster::UpdateAIStatus()
{
	int nStep = 0;
	DEBUG_TRY
	int nNewStatus = CheckAIStatus(m_nAIStatus);
	nStep = 1;
	if(nNewStatus != m_nAIStatus)
	{
		LeaveAIStatus(m_nAIStatus);
		nStep = 2;
		m_nAIStatus = nNewStatus;
		EnterAIStatus(m_nAIStatus);
		nStep = 3;
		return;
	}
	
	ProcessAIStatus(m_nAIStatus);
	DEBUG_CATCH3("CMonster::UpdateAIStatus() nStep = %u status = %u",nStep,m_nAIStatus);
}

void CMonster::ProcessWander()
{
	if ((m_pType->GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_NOWANDER) != 0)		// ������ һֱ�й�
	{
		return;
	}

	//�ϴ�δ����
	if(m_nPathLen > 0)
		MoveNext(m_nMoveMode);
	else
	{
		//�Ƿ�Ҫ��һ��Ѱ·
		if(!m_tWander.IsActive())
		{
			m_tWander.SetInterval( m_dwMoveInterTime * 5 + RandGet(m_dwMoveInterTime * 20) );	
			m_tWander.Update();
		}
		else
		{
			//Ѱ·
			if(m_tWander.IsTimeOut())
			{
				Walk(GetPosX() + RandGet(5) - 2,GetPosY() + RandGet(5) - 2,true);
				m_tWander.Clear();
			}
		}
	}
}

void CMonster::ProcessChase()
{
	IRole* pRole = RoleManager()->QueryRole(m_idTarget);
	CHECK(pRole);
	Run(pRole->GetPosX(),pRole->GetPosY(),false);
	if(m_nPathLen > 0)
		MoveNext(m_nMoveMode);
}

void CMonster::ProcessSoonBack()
{
	if(m_nPathLen > 0)
		MoveNext(m_nMoveMode);
	else
	{
		Run(m_nBornX,m_nBornY,false);
		if(m_nPathLen > 0)
			MoveNext(m_nMoveMode);
	}
}

bool CMonster::TestMagicCastEnable(OBJID idMagicType,OBJID idTarget,int x,int y)
{
	return m_pMagic->TestMagicCastEnable(idMagicType,idTarget,x,y);
}

void CMonster::ProcessAtk()
{
//modify code by �ֵ�Ԩ begin 2011.6.22
	m_idTargetBoss = m_idTarget;
//modify code by �ֵ�Ԩ end
	IRole* pRole = RoleManager()->QueryRole(m_idTarget);
	CHECK(pRole);
	//���Ǽ���	
	const VCT_NPCMAGICDATA* pVtcData = g_pNpcMagicManager->QueryDataByOwnerType(GetType());
	if(pVtcData && pVtcData->size())
	{
		int magictradeid = 0;
		int magicbuffid = 0;
		int magicdbuffid = 0;
		int magicnid = 0;
		int magicfid = 0;
		
		for(int i = 0; i < pVtcData->size();i++)
		{
			CNpcMagicData* pMagicData = (*pVtcData)[i];
			CHECK(pMagicData);
			if(TestMagicCastEnable(pMagicData->GetInt(EM_NPCMAGICDATA_magictype),m_idTarget,pRole->GetPosX(),pRole->GetPosY()))
			{
				switch(pMagicData->GetInt(EM_NPCMAGICDATA_magictypekind))
				{
				case 1:
					magictradeid = pMagicData->GetInt(EM_NPCMAGICDATA_magictype);
					break;
				case 2:
					magicbuffid = pMagicData->GetInt(EM_NPCMAGICDATA_magictype);
					break;
				case 3:
					magicnid = pMagicData->GetInt(EM_NPCMAGICDATA_magictype);
					break;
				case 4:
					magicfid = pMagicData->GetInt(EM_NPCMAGICDATA_magictype);
					break;
				case 5:
					magicdbuffid = pMagicData->GetInt(EM_NPCMAGICDATA_magictype);
					break;
				}
			}
		}
		

// 		I64 MyEffect = GetEffect();
// 		I64 MyTargetEffect = pRole->GetEffect();
		
		if(magictradeid != 0 && (float)GetLife()/(float)GetMaxLife() <= 0.7)
		{			
			MagicAttack(magictradeid,GetID(),m_nPosX,m_nPosY);
		}
// 		else if(magicbuffid != 0 && !(MyEffect & GetEffectFromMagicType(magicbuffid)))
// 		{
// 			return MagicAttack(magicbuffid,GetID(),m_nPosX,m_nPosY);
// 		}
// 		else if(magicdbuffid != 0 && !(MyTargetEffect & GetEffectFromMagicType(magicdbuffid)))
// 		{
// 			return MagicAttack(magicdbuffid,idTarget,nTargetX,nTargetY);
// 		}
		else if(magicnid != 0)
		{
			MagicAttack(magicnid,m_idTarget,pRole->GetPosX(),pRole->GetPosY());
		}
		else if(magicfid != 0)
		{
			MagicAttack(magicfid,m_idTarget,pRole->GetPosX(),pRole->GetPosY());
		}
	}
}
