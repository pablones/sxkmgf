// UserManager.cpp: implementation of the CMapGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "MessagePort.h"
#pragma warning(disable:4786)
#include "AllMsg.h"
#include "WargameManager.h"
#include "MapInstanceMgr.h"
#include "MapGroup.h"
//#pragma warning(disable:4786)
#include "protocol.h"
#include "itemtype.h"
#include "MapManager.h"
#include "ActionDefine.h"
#include "WantedList.h"
#include "DeadLoop.h"
#include "Console.h"
#include "TopListSystem.h"
#include "GemCreate.h"//20061010修罗
#include "EquipCreate.h"//20061216修罗
#include "AwardExp.h"//20070418哈雷
#include "AwardHonor.h"
#include "FecthData.h"
#include "SKHonorMurderList.h"//20070213修罗
#include "Rename.h"
#include "CombGod.h"

#include "SKPostOffice.h"///softworms
#include "ResourceWar.h"//王玉波

#include "DataInspectManage.h"
#include "AdjustRate.h"
#include "SettledAction.h"//20070807修罗
#include "GiftManager.h"//20071110修罗:

#include "WargameManager.h"

#include "NpcMagicManager.h"//20080116修罗:怪物技能静态表
#include "Antiplug.h"
#include "CityWarMgr.h"
#include "../mapdata/source/T_CompactObjSet.h"
#include "../mapdata/source/Cell.h"
#include "CombGod.h"
#include "TaskSetRand.h"
#include "StockOrder.h"
#include "AwardMoney.h"

#include "StockOrder.h"
#include "Suit.h"
#include "TitleAttar.h"
const int UPDATASTOCKMARKETTIME = 30/*60*10*/;//定时清理指定时间内没有完成交易的定单

#include "ItemDropLimit.h"
#include "ShopVAS.h"
#include "MsgMainServer.h"
#include "TutorList.h"
#include "VerificationData.h"
//>>>>>>> .theirs
//#include "DropRuleMap.h"	//add by zlong 2003-11-15

//#include "UnitTestSelf.h"

//MYHEAP_IMPLEMENTATION(CMapGroup,s_heap)
//////////////////////////////////////////////////////////////////////
// 全局对象
CMapGroup::MAPGROUP_SET		CMapGroup::m_setMapGroup;
CItemType*		g_pItemType		= NULL;
CPortableShopManager* g_pPortableShopManager =NULL;
CDropRuleMap*	g_pDropRuleMap	= NULL;	
CTaskSetRand*   g_pTaskSetRand  = NULL;
CRename*		g_pRename		= NULL;
CVerificationData* g_pVerificationData=NULL;
IStaticMapSet*	g_setStaticMap	= NULL;
IActionSet*		g_setAction		= NULL;
ILevupexpSet*	g_setLevupexp	= NULL;
IMagicTypeSet*	g_setMagicType	= NULL;
INpcTypeSet*	g_setStaticNpcType	= NULL;
IMagicTypeSet*	g_setAutoMagicType	= NULL;
IMonsterTypeSet*	g_setMonsterType	= NULL;
IEudemonTypeSet*	g_setEudemonType	= NULL;
IPeifangTypeSet*	g_setPeifangType	= NULL;
CSuit			*g_pSuit = NULL;
IInstanceTypeSet*	g_setInstanceType	= NULL;///2008-01-24-softworms-副本type静态表
CShopVASItem*	g_pShopVas	= NULL;
ITitleTypeSet*  g_setTitleType = NULL;
ITreasureTypeSet* g_setTreasureType = NULL;
IJinmeiTypeSet* g_setJinmeiType = NULL;
IJinmeiStrengthTypeSet* g_setJinmeiStrengthType = NULL;
IJinmeiEffectTypeSet* g_setJinmeiEffectType = NULL;


int g_MainGameServer = 0;
int g_MainServerData = -1;
int g_SpecilLine	 = 0;
//modify code by 林德渊 begin 2011.5.6
char			 g_m_circle_list[LIN_CIRCLE_NUM][256];
int				 g_m_circleTime_list[LIN_CIRCLE_NUM];
CTimeOut		 g_m_circletime_list[LIN_CIRCLE_NUM];
bool			 g_circleflag = true;
//modify code by 林德渊 end
//modify code by 林德渊 begin 2011.5.19
CStatistical	 g_StatisticalData;
//modify code by 林德渊 end
///初始化战争活动管理器
//initialiseSingleton(CWargameManager);

//CTopListSystem	g_TopListSystem;			//排行榜系统,全局对象
//const int MSGPORT_MAPGROUP_LAST = 7;		//最后一个地图组ID
char	szDummyDataObjectTable[] = "id";
//////////////////////////////////////////////////////////////////////
// Construction/Destructiond
//////////////////////////////////////////////////////////////////////
CMapGroup::CMapGroup()
{
	m_idProcess		= NULL;
	m_pSocket		= NULL;
	m_pDb			= NULL;
	m_pMsgPort		= NULL;

	m_pRoleManager	= NULL;
	m_pUserManager	= NULL;
	m_pMapManager	= NULL;
	m_pNpcManager	= NULL;
	m_pSynManager	= NULL;
	m_pLeaveWord	= NULL;
	m_pGameAction	= NULL;
	m_ptrMapInstanceMgr = NULL;
	m_ptrWargameManager = NULL;
	m_ptrLuaScriptMachine=NULL;
	m_ptrVariantManager=NULL;///临时变量管理器
//modify code by 林德渊 begin 2011.5.6
// 	if( g_circleflag )
// 	{
// 		for( int i_num = 0; i_num < LIN_CIRCLE_NUM; i_num++ )
// 		{
// 			g_m_circleTime_list[i_num] = -1;
// 			g_m_circletime_list[i_num].SetInterval(0);
// 		}
// 		g_circleflag = false;
// 	}
	
//modify code by 林德渊 end

	m_pInstanceUseMgr    = NULL;
	m_pUserJinmeiSysMgr     = NULL;
	m_PVPManager         = NULL;
	strcpy(m_circle, "STOP");
	m_circleTime		 = -1;
	m_circleFlag		 = false;
	weekState			 = -1;
	nHourseState		 = -1;
	MartialAct			 = -1;
	AltarAct			 = -1;
        SynMaintain			 = -1;
	Allday				 = -1;
	Hunt				 = -1;
	m_circletime.SetInterval(0);
    ZeroMemory(m_szServerName,sizeof(m_szServerName));

	m_NoitemIdVec.clear(); //清空物品信息

	m_nDayState			= -1;
}



#include "IniFile.h"
//////////////////////////////////////////////////////////////////////
bool CMapGroup::Create(PROCESS_ID idProcess, ISocket* pSocket, IDatabase* pDb, IMessagePort* pMsgPort)
{
	CIniFile	ini(CONFIG_FILENAME, "AccountServer");
	ini.GetString(m_szServerName,"SERVERNAME",DBSTR_SIZE);

	m_idProcess	= idProcess;
	m_pSocket	= pSocket;
	m_pDb		= pDb;
	m_pMsgPort	= pMsgPort;

	
	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_ITEM);
	m_pGameItemDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pGameItemDataDefault)
	{
		LOGERROR("wb_item error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_USER);
	m_pUserDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pUserDataDefault)
	{
		LOGERROR("wb_user error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_MAP);
	m_pGameMapDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pGameMapDataDefault)
	{
		LOGERROR("wb_map error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_MAGIC);
	m_pMagicDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pMagicDataDefault)
	{
		LOGERROR("wb_magic error!!");
		return false;
	}
	



	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_SYNATTR);
	m_pSynAttrDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pSynAttrDataDefault)
	{
		LOGERROR("wb_synattr error!!");
		return false;
	}
	
	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_DYNANPC);
	m_pDynaNpcDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pDynaNpcDataDefault)
	{
		LOGERROR("wb_dynnpc error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_DYNAMAP);
	m_pDynaMapDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pDynaMapDataDefault)
	{
		LOGERROR("wb_dynmap error!!");
		return false;
	}
	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_SYNMAP);
	m_pSynMapDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pSynMapDataDefault)
	{
		LOGERROR("wb_synmap error!!");
		return false;
	}
	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_EUDEMON);
	m_pEudemonDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pEudemonDataDefault)
	{
		LOGERROR("wb_eudemon error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_CPSATIONMSG);
	m_pCPSationDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pCPSationDefault)
	{
		LOGERROR("wb_compensationmsg error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_TASKDETAIL);
	m_pTaskDetailDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pTaskDetailDataDefault)
	{
		LOGERROR("wb_taskdetail error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_WEAPONSOUL);
	m_pWeaponSoulDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pWeaponSoulDataDefault)
	{
		LOGERROR("wb_weapensoul error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_INSTANCEUSETIME);
	m_pInstanceUseDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pInstanceUseDataDefault)
	{
		LOGERROR("wb_instancetime error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_PEIFANG);
	m_pPeifangDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pPeifangDataDefault)
	{
		LOGERROR("wb_peifang error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_TITLE);
	m_pTitleDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pTitleDataDefault)
	{
		LOGERROR("wb_weapensoul error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_TREASURE);
	m_pTreasureDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pTreasureDataDefault)
	{
		LOGERROR("wb_treasure error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_ONLINEGIFT);
	m_pOnlineGiftDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pOnlineGiftDefault)
	{
		LOGERROR("wb_onlinegift error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_JINMEIUSER);
	m_pJinmeiDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pJinmeiDataDefault)
	{
		LOGERROR("wb_jinmeiuser error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_TUTOR);
	m_pTutorDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pTutorDataDefault)
	{
		LOGERROR("wb_tutor error!!");
		return false;
	}
	
	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_PETEGG);
	m_pPeteggDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pPeteggDataDefault)
	{
		LOGERROR("wb_petegg error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_USERKEY);
	m_pUserKeyDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pUserKeyDataDefault)
	{
		LOGERROR("_TBL_USERKEY error!!");
		return false;
	}
	if(!m_pUserKeyDataDefault->SetThisKey("userid"))
	{
		LOGERROR("_TBL_USERKEY key error!!");
		return false;
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_AUTOFIGHTSET);
	m_pAutoFightDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pAutoFightDataDefault)
	{
		LOGERROR("_TBL_AUTOFIGHTSET error!!");
		return false;
	}
	sprintf(szSQL,"SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_SPECILLINE_INFO);
	m_pSpecilLineDataDefault = Database()->CreateNewRecordset(szSQL, false);
	if(!m_pSpecilLineDataDefault)
	{
		LOGERROR("_TBL_SPECILLINE_INFO error!!");
		return false;
	}
	sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_SYNDICATE);
	m_pSynDataDefault	= Database()->CreateNewRecordset(szSQL, false);
	if(!m_pSynDataDefault)
	{
		LOGERROR("wb_syndicate error!!");
		return false;
	}

	sprintf(szSQL,"SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_USERWARGAME);
	m_pWarGameDataDefault = Database()->CreateNewRecordset(szSQL, false);
	if(!m_pWarGameDataDefault)
	{
		LOGERROR("_TBL_USERWARGAME error!!");
		return false;
	}
//modify code by 林德渊 begin 2011.5.30-
	sprintf(szSQL,"SELECT * FROM %s WHERE id=0 LIMIT 1", _TBL_VASLV);
	m_pVaslvDataDefault = Database()->CreateNewRecordset(szSQL, false);
	if(!m_pVaslvDataDefault)
	{
		LOGERROR("_TBL_VASLV error!!");
		return false;
	}
//modify code by 林德渊 end

	// 创建全局常量对象
	if(m_idProcess == MSGPORT_MAPGROUP_FIRST)
	{


		g_pItemType = CItemType::CreateNew();
		IF_NOT(g_pItemType && g_pItemType->Create(pDb))
			return false;

		//add by zlong 2003-11-15
		g_pDropRuleMap = CDropRuleMap::CreateNew();
		IF_NOT(g_pDropRuleMap && g_pDropRuleMap->Create(pDb))
			return false;

		g_pTaskSetRand = CTaskSetRand::CreateNew();
		IF_NOT(g_pTaskSetRand&&g_pTaskSetRand->Create(pDb))
			return false;

		char	szSQL[1024];
		sprintf(szSQL, "SELECT * FROM %s", _TBL_MAP);
		g_setStaticMap = CStaticMapSet::CreateNew(true);
		IF_NOT_(g_setStaticMap && g_setStaticMap->Create(szSQL, pDb))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_ACTION);
		g_setAction	= CActionSet::CreateNew(true);
		IF_NOT_(g_setAction && g_setAction->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_LEVEXP);
		g_setLevupexp	= CLevupexpSet::CreateNew(true);
		IF_NOT_(g_setLevupexp && g_setLevupexp->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_MAGICTYPE);
		g_setMagicType	= CMagicTypeSet::CreateNew(true);
		IF_NOT_(g_setMagicType && g_setMagicType->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s  WHERE  sort > 2", _TBL_NPC);
		g_setStaticNpcType	= CNpcTypeSet::CreateNew(true);
		IF_NOT_(g_setStaticNpcType && g_setStaticNpcType->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_INSTANCETYPE);
		g_setInstanceType	= CInstanceTypeSet::CreateNew(true);
		IF_NOT_(g_setInstanceType && g_setInstanceType->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_MAGICTYPE);
		g_setAutoMagicType	= CMagicTypeSet::CreateNew(true);
		IF_NOT_(g_setAutoMagicType && g_setAutoMagicType->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_MONSTERTYPE);
		g_setMonsterType	= CMonsterTypeSet::CreateNew(true);
		IF_NOT_(g_setMonsterType && g_setMonsterType->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_EUDEMONTYPE);
		g_setEudemonType	= CEudemonTypeSet::CreateNew(true);
		IF_NOT_(g_setEudemonType && g_setEudemonType->Create(szSQL, Database()))
			return false;
		
		sprintf(szSQL, "SELECT * FROM %s", _TBL_PEIFANGTYPE);
		g_setPeifangType	= CPeifangTypeSet::CreateNew(true);
		IF_NOT_(g_setPeifangType && g_setPeifangType->Create(szSQL, Database()))
			return false;


		sprintf(szSQL, "SELECT * FROM %s", _TBL_TITLETYPE);
		g_setTitleType	= CTitleTypeSet::CreateNew(true);
		IF_NOT_(g_setTitleType && g_setTitleType->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_TREASURETYPE);
		g_setTreasureType	= CTreasureTypeSet::CreateNew(true);
		IF_NOT_(g_setTreasureType && g_setTreasureType->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_JINMEITYPE);
		g_setJinmeiType = CJinmeiTypeSet::CreateNew(true);
		IF_NOT_(g_setJinmeiType && g_setJinmeiType->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_JINMEIEFFCTTYPE);
		g_setJinmeiEffectType = CJinmeiEffectTypeSet::CreateNew(true);
        IF_NOT_(g_setJinmeiEffectType && g_setJinmeiEffectType->Create(szSQL, Database()))
			return false;

		sprintf(szSQL, "SELECT * FROM %s", _TBL_JINMEISTRENGTHTYPE);
		g_setJinmeiStrengthType = CJinmeiStrengthTypeSet::CreateNew(true);
		IF_NOT_(g_setJinmeiStrengthType && g_setJinmeiStrengthType->Create(szSQL, Database()))
			return false;

		if( !g_TopListSystem.CreateData( pDb ) )
			return false;

		IF_NOT( g_TutorListSystem.CreateData( pDb ) )
			return false;

		g_pShopVas	= CShopVASItem::CreateNew();
		IF_NOT(g_pShopVas && g_pShopVas->Create(pDb))
			return false;

		g_pSuit = CSuit::CreateNew();
		IF_NOT(g_pSuit && g_pSuit->Create(pDb))
		{
			return false;
		}

		g_pNpcMagicManager = CNpcMagicManager::CreateNew();
		IF_NOT(g_pNpcMagicManager && g_pNpcMagicManager->CreateData(pDb))
		{
			//[2009.07.15]
			try
			{
				delete g_pNpcMagicManager;
				g_pNpcMagicManager = NULL;
			}
			catch (...)
			{
				g_pNpcMagicManager = NULL;
			}
			
			return false;
		}
		
		g_pGiftManager = CGiftManager::CreateNew();
		IF_NOT(g_pGiftManager && g_pGiftManager->Create(pDb))
			return false;
		//改名
		g_pRename = new CRename(m_idProcess);//
		g_pRename->Create();
		
		//跑商验证
		g_pVerificationData = CVerificationData::CreateNew();
		IF_NOT(g_pVerificationData && g_pVerificationData->Create(pDb))
			return false;

		g_pPortableShopManager = CPortableShopManager::CreateNew();//随身商店
		IF_NOT(g_pPortableShopManager && g_pPortableShopManager->CreateAll(m_idProcess))
			return false;

	}


	CMapManager* pMapManager = new CMapManager(m_idProcess);

	//zeng修改，把这条语句提前
	m_pMapManager = pMapManager->GetInterface();

	if (!pMapManager || !pMapManager->Create())
	{
		ASSERT(!"pMapManager->Create");
		return false;
	}
	// m_pMapManager = pMapManager->GetInterface();

	CRoleManager* pRoleManager = new CRoleManager(m_idProcess);
	if (!pRoleManager || !pRoleManager->Create())
	{
		ASSERT(!"pRoleManager->Create");
		return false;
	}
	m_pRoleManager = pRoleManager->GetInterface();

	CUserManager* pUserManager = new CUserManager(m_idProcess);
	if (!pUserManager || !pUserManager->Create())
	{
		ASSERT(!"pUserManager->Create");
		return false;
	}
	m_pUserManager = pUserManager->GetInterface();



	m_PVPManager = new CPVPManager(m_idProcess);

	CNpcManager* pNpcManager = new CNpcManager(m_idProcess);
	if (!pNpcManager || !pNpcManager->Create())
	{
		ASSERT(!"pNpcManager->Create");
		return false;
	}
	m_pNpcManager = pNpcManager->GetInterface();

	m_pLeaveWord = new CLeaveWord();
	if (!m_pLeaveWord || !m_pLeaveWord->Create(m_idProcess))
	{
		ASSERT(!"m_pLeaveWord->Create");
		return false;
	}

	m_pGameAction = CGameAction::CreateNew(m_idProcess);
	if (!m_pGameAction)
	{
		ASSERT(!"m_pGameAction->Create");
		return false;
	}
 
	if(m_idProcess == MSGPORT_MAPGROUP_FIRST)
	{
		m_ptrWargameManager=new CWargameManager;
		CHECKF(m_ptrWargameManager);
		m_ptrWargameManager->Init(m_pDb,m_idProcess);
	}
	if(m_idProcess == MSGPORT_MAPGROUP_INSTANCE)
	{
		m_ptrMapInstanceMgr=new CMapInstanceMgr;
		CHECKF(m_ptrMapInstanceMgr);
		m_ptrMapInstanceMgr->Init(m_pDb,m_idProcess);
	}

	m_ptrVariantManager= new CVariantManager(m_idProcess);///临时变量管理器
	CHECKF(m_ptrVariantManager);

	m_ptrLuaScriptMachine= new CLuaScriptMachine(m_idProcess);
	CHECKF(m_ptrLuaScriptMachine);



	m_pInstanceUseMgr = new CInstanceUseTimeMgr;
	if (!m_pInstanceUseMgr || !m_pInstanceUseMgr->Create(m_pDb, m_idProcess))
	{
		ASSERT(!"m_pInstanceUseMgr->Create");
		return false;
	}

	m_pUserJinmeiSysMgr = new CUserJinMeiSysMgr;
	if (!m_pUserJinmeiSysMgr || !m_pUserJinmeiSysMgr->Init(m_idProcess, m_pDb, m_pJinmeiDataDefault))
	{
		ASSERT(!"m_pUserJinmeiSysMgr->Create");
		return false;
	}
	m_pSynManager = new CSynManager(m_idProcess);
	if (!m_pSynManager || !m_pSynManager->Create())
	{
		ASSERT(!"m_pSynManager->Create");
		return false;
	}

	CONST INTERVAL_EVENT = 60;	// seconds
	m_tmEvent.Startup(INTERVAL_EVENT);

 	return true;
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::Destroy()
{
	//SAFE_RELEASE (m_pGameAction);
	SAFE_RELEASE (m_pLeaveWord);
	SAFE_RELEASE (m_pUserManager);
	SAFE_RELEASE (m_pRoleManager);
	SAFE_RELEASE (m_pNpcManager);
	SAFE_RELEASE (m_pMapManager);
	SAFE_RELEASE (m_pGameAction);
	SAFE_DELETE (m_pInstanceUseMgr);
	SAFE_DELETE (m_PVPManager);
	SAFE_DELETE (m_pUserJinmeiSysMgr);
	SAFE_RELEASE (m_pSynManager);

	m_NoitemIdVec.clear(); //清空物品信息，无需保存

	// 清除全局常量对象
	if(m_idProcess == MSGPORT_MAPGROUP_FIRST)		//@ 可能有问题
	{
		SAFE_RELEASE(g_pPortableShopManager);
		SAFE_DELETE (m_ptrWargameManager);
		SAFE_RELEASE (g_setMonsterType);
		SAFE_RELEASE (g_setEudemonType);
		SAFE_RELEASE (g_setPeifangType);
		SAFE_RELEASE (g_setAutoMagicType);
		SAFE_RELEASE (g_setMagicType);
		SAFE_RELEASE (g_setStaticNpcType);
		SAFE_RELEASE (g_setLevupexp);
		SAFE_RELEASE(g_setAction);
		SAFE_RELEASE (g_setStaticMap);
		SAFE_RELEASE (g_pItemType);
		SAFE_RELEASE (g_pDropRuleMap);
		SAFE_RELEASE (g_pSuit);
		SAFE_RELEASE (g_setInstanceType);
		SAFE_RELEASE (g_pGiftManager);
		SAFE_RELEASE (g_pTaskSetRand);

		SAFE_DELETE(g_pNpcMagicManager);
		SAFE_DELETE(g_pShopVas);
		SAFE_RELEASE(g_setTitleType);
		SAFE_RELEASE(g_setTreasureType);
		SAFE_RELEASE(g_setJinmeiType);
		SAFE_RELEASE(g_setJinmeiStrengthType);
		SAFE_RELEASE(g_setJinmeiEffectType);
	
	}
	SAFE_RELEASE (m_pSynDataDefault);
	SAFE_RELEASE (m_pEudemonDataDefault);
	SAFE_RELEASE (m_pCPSationDefault);
	SAFE_RELEASE (m_pPeifangDataDefault);
	SAFE_RELEASE (m_pDynaMapDataDefault);
	SAFE_RELEASE (m_pSynMapDataDefault);
	SAFE_RELEASE (m_pDynaNpcDataDefault);
	SAFE_RELEASE (m_pSynAttrDataDefault);
	
	SAFE_RELEASE (m_pMagicDataDefault);
	SAFE_RELEASE (m_pGameItemDataDefault);
	SAFE_RELEASE (m_pUserDataDefault);
	SAFE_RELEASE (m_pGameMapDataDefault);
	SAFE_RELEASE (m_pTaskDetailDataDefault );
	SAFE_RELEASE (m_pWeaponSoulDataDefault);
	SAFE_RELEASE (m_pInstanceUseDataDefault);

	SAFE_DELETE  (m_ptrMapInstanceMgr);	//  [1/5/2008 彭正帮]	清理
	SAFE_DELETE  (m_ptrLuaScriptMachine);

	SAFE_DELETE  (m_ptrVariantManager);
	SAFE_RELEASE (m_pTitleDataDefault);
	SAFE_RELEASE (m_pTreasureDataDefault);
	SAFE_RELEASE (m_pJinmeiDataDefault);
	SAFE_RELEASE(m_pOnlineGiftDefault);
	SAFE_RELEASE (m_pTutorDataDefault);
	SAFE_RELEASE (m_pPeteggDataDefault);
	SAFE_RELEASE(m_pWarGameDataDefault);
	SAFE_RELEASE(m_pSpecilLineDataDefault);
//modify code by 林德渊 begin 2011.5.30-
	SAFE_RELEASE(m_pVaslvDataDefault);
//modify code by 林德渊 end
	//CompactObjSet<CCell>::Release();
	
//	CWargameManager::dstrySingleton();///析构战争活动管理器
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::OnTimer(time_t tCurr)
{
	DEBUG_TRY
	DEADLOOP_CHECK(PID, "RoleManager()->OnTimer")
	RoleManager()->OnTimer(tCurr);
	DEBUG_CATCH("RoleManager()->OnTimer(tCurr);")

	DEBUG_TRY
	DEADLOOP_CHECK(PID, "MapManager()->OnTimer")
	MapManager()->OnTimer(tCurr);
	DEBUG_CATCH("MapManager()->OnTimer(tCurr);")
	
	DEBUG_TRY
	DEADLOOP_CHECK(PID, "UserManager()->OnTimer")
	UserManager()->OnTimer(tCurr);
	DEBUG_CATCH("UserManager()->OnTimer(tCurr);")


	DEBUG_TRY
	DEADLOOP_CHECK(PID, "SynManager()->OnTimer")
	GetSynManager()->OnTimer(tCurr);
	DEBUG_CATCH("SynManager()->OnTimer(tCurr);")

	DEBUG_TRY	
	GetPVPManger()->doPVP();
	DEBUG_CATCH("GetPVPManger()->doPVP();")
	
	DEBUG_TRY
	DEADLOOP_CHECK(PID, "LeaveWord()->OnTimer")
	LeaveWord()->OnTimer(tCurr);
	DEBUG_CATCH("LeaveWord()->OnTimer(tCurr);")

	DEBUG_TRY
	DEADLOOP_CHECK(PID, "UserJinmeiSysMgr()->OnTimer")
	UserJinmeiSysMgr()->OnTimer(tCurr);
	DEBUG_CATCH("UserJinmeiSysMgr()->OnTimer(tCurr);")

	if( m_idProcess == MSGPORT_MAPGROUP_FIRST)//第一个地图组
	{
		DEBUG_TRY
		DoBroadcastMsg();
		DEBUG_CATCH( "broad talk msg!!" )

		DEBUG_TRY
		g_TopListSystem.OnTimer( tCurr );
		DEBUG_CATCH( "CTopListSyste::OnTimer() catch exception!" )

		DEBUG_TRY
		g_TutorListSystem.OnTimer( tCurr );
		DEBUG_CATCH( "CTutorList::OnTimer() catch exception!")
		DEBUG_TRY
		m_ptrWargameManager->OnTimer(tCurr);
		DEBUG_CATCH("m_ptrWargameManager->OnTimer(tCurr);")
	}

	if(m_idProcess==MSGPORT_MAPGROUP_INSTANCE)
	{

		DEBUG_TRY
		DEADLOOP_CHECK(PID, "InstanceUseMgr()->OnTimer")
		InstanceUseMgr()->OnTimer(tCurr);
		DEBUG_CATCH("InstanceUseMgr()->OnTimer(tCurr);")


		DEBUG_TRY
		DEADLOOP_CHECK(PID, "MapInstanceMgr()->OnTimer)")
		MapInstanceMgr()->OnTimer(tCurr);
		DEBUG_CATCH("MapInstanceMgr()->OnTimmer(tCurr);")

	}
}

//////////////////////////////////////////////////////////////////////
BOOL CMapGroup::CheckTime(const DWORD dwType, const char* pszParam)
{
	if (!pszParam)
		return false;

	time_t ltime;
	time( &ltime );
	
	struct tm *pTime;
	pTime = localtime( &ltime ); /* Convert to local time. */
	
	int nCurYear, nCurMonth, nCurDay, nCurWeekDay, nCurYearDay, nCurHour, nCurMinute;
	nCurYear	=pTime->tm_year+1900;
	nCurMonth	=pTime->tm_mon+1;
	nCurDay		=pTime->tm_mday;
	nCurWeekDay	=pTime->tm_wday;
	nCurYearDay	=pTime->tm_yday;
	nCurHour	=pTime->tm_hour;
	nCurMinute	=pTime->tm_min;
	
	int nYear0=0, nMonth0=0, nDay0=0, nHour0=0, nMinute0=0;
	int nYear1=0, nMonth1=0, nDay1=0, nHour1=0, nMinute1=0;
	
	if (dwType == 0)			//精确时间
	{
		if (10 == sscanf(pszParam, "%d-%d-%d %d:%d %d-%d-%d %d:%d",
			&nYear0, &nMonth0, &nDay0, &nHour0, &nMinute0,
			&nYear1, &nMonth1, &nDay1, &nHour1, &nMinute1))
		{
			struct tm time0, time1;
			memset(&time0, 0L, sizeof(tm));
			memset(&time1, 0L, sizeof(tm));
			
			time0.tm_year	=nYear0-1900;
			time0.tm_mon	=nMonth0-1;	
			time0.tm_mday	=nDay0;
			time0.tm_hour	=nHour0;
			time0.tm_min	=nMinute0;
			time0.tm_isdst	=pTime->tm_isdst;
			
			time1.tm_year	=nYear1-1900;
			time1.tm_mon	=nMonth1-1;	
			time1.tm_mday	=nDay1;
			time1.tm_hour	=nHour1;
			time1.tm_min	=nMinute1;
			time1.tm_isdst	=pTime->tm_isdst;
			
			time_t t0	=mktime(&time0);
			time_t t1	=mktime(&time1);
			
			if (ltime >= t0 && ltime<= t1)
				return true;
		}
	}
	else if (dwType == 1)	//年时间
	{
		if (8 == sscanf(pszParam, "%d-%d %d:%d %d-%d %d:%d",
			&nMonth0, &nDay0, &nHour0, &nMinute0,
			&nMonth1, &nDay1, &nHour1, &nMinute1))
		{
			struct tm time0, time1;
			memset(&time0, 0L, sizeof(tm));
			memset(&time1, 0L, sizeof(tm));
			
			time0.tm_year	=pTime->tm_year;
			time0.tm_mon	=nMonth0-1;	
			time0.tm_mday	=nDay0;
			time0.tm_hour	=nHour0;
			time0.tm_min	=nMinute0;
			time0.tm_isdst	=pTime->tm_isdst;
			
			time1.tm_year	=pTime->tm_year;
			time1.tm_mon	=nMonth1-1;	
			time1.tm_mday	=nDay1;
			time1.tm_hour	=nHour1;
			time1.tm_min	=nMinute1;
			time1.tm_isdst	=pTime->tm_isdst;
			
			time_t t0	=mktime(&time0);
			time_t t1	=mktime(&time1);
			
			if (ltime >= t0 && ltime<= t1)
				return true;
		}
	}
	else if (dwType == 2)	//月时间
	{
		if (6 == sscanf(pszParam, "%d %d:%d %d %d:%d",
			&nDay0, &nHour0, &nMinute0,
			&nDay1, &nHour1, &nMinute1))
		{
			struct tm time0, time1;
			memset(&time0, 0L, sizeof(tm));
			memset(&time1, 0L, sizeof(tm));
			
			time0.tm_year	=pTime->tm_year;
			time0.tm_mon	=pTime->tm_mon;	
			time0.tm_mday	=nDay0;
			time0.tm_hour	=nHour0;
			time0.tm_min	=nMinute0;
			time0.tm_isdst	=pTime->tm_isdst;
			
			time1.tm_year	=pTime->tm_year;
			time1.tm_mon	=pTime->tm_mon;
			time1.tm_mday	=nDay1;
			time1.tm_hour	=nHour1;
			time1.tm_min	=nMinute1;
			time1.tm_isdst	=pTime->tm_isdst;
			
			time_t t0	=mktime(&time0);
			time_t t1	=mktime(&time1);
			
			if (ltime >= t0 && ltime<= t1)
				return true;
		}
	}
	else if (dwType == 3)	//周时间
	{
		if (6 == sscanf(pszParam, "%d %d:%d %d %d:%d",
			&nDay0, &nHour0, &nMinute0,
			&nDay1, &nHour1, &nMinute1))
		{
			DWORD timeNow	=nCurWeekDay*24*60+nCurHour*60+nCurMinute;
			if (timeNow >= nDay0*24*60+nHour0*60+nMinute0 && 
					timeNow <= nDay1*24*60+nHour1*60+nMinute1)
			{
				return true;
			}
		}
	}
	else if (dwType == 4)	//日时间
	{
		if (4 == sscanf(pszParam, "%d:%d %d:%d",
			&nHour0, &nMinute0,
			&nHour1, &nMinute1))
		{
			DWORD timeNow	=nCurHour*60+nCurMinute;
			if (timeNow >= nHour0*60+nMinute0 && 
					timeNow <= nHour1*60+nMinute1)
			{
				return true;
			}
		}
	}
	else if (dwType == 5)	//小时时间
	{
		if (2 == sscanf(pszParam, "%d %d",
			&nMinute0,
			&nMinute1))
		{
			if (nCurMinute >= nMinute0 && nCurMinute <= nMinute1)
			{
				return true;
			}
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::ProcessEvent(void)
{
	CONST OBJID ID_EVENT_BEGIN	= 2000000;
	CONST DWORD MAX_EVENT		= 100;

	OBJID idBegin = ID_EVENT_BEGIN+m_idProcess*10000;
	for (OBJID id=idBegin; id<idBegin+MAX_EVENT; id++)
	{
		//this->ProcessAction(id);
		if (ActionSet()->GetObj(id))
			GameAction()->ProcessAction(id);
	}
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::SendClientMsg(SOCKET_ID idSocket, CNetMsg* pMsg)
{
	pMsg->AppendInfo(m_idProcess, idSocket, ID_NONE);
 	return m_pSocket->SendMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
// bool CMapGroup::SendNpcMsg(OBJID idNpc, CNetMsg* pMsg)
// {
// 	pMsg->AppendInfo(m_idProcess, SOCKET_NONE, idNpc);
// 	return m_pSocket->SendMsg(pMsg);
// }

//////////////////////////////////////////////////////////////////////
bool CMapGroup::SendBroadcastMsg(CNetMsg* pMsg)
{
	for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
	{
		if(i != m_idProcess)		// 不回送
		{
			MESSAGESTR	buf;
			CLIENTMSG_PACKET0* pPacket = (CLIENTMSG_PACKET0*)buf;
			pPacket->idSocket	= pMsg->GetSocketID();
			pPacket->idPacket	= pMsg->GetType();
			memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
			pPacket->nSize		= pMsg->GetSize();
			m_pMsgPort->Send(i, MAPGROUP_BROADCASTMSG, STRUCT_TYPE(buf), pPacket);
		}
	}
	return true;
}
//06.9.1为种族聊天加的函数
//王玉波填加
bool CMapGroup::TransmitMsg( CNetMsg* pMsg,int nPhyle )
{
	//ASSERT( nPhyle != 0 );
	MESSAGESTR	buf;

	TRANSPHYMSG_PACK0* pPacket = ( TRANSPHYMSG_PACK0* )buf;
	pPacket->nPhyle		= nPhyle;
	pPacket->idPacket	= pMsg->GetType();
	memcpy( pPacket->buf, pMsg->GetBuf(), pMsg->GetSize() );
	pPacket->nSize		= pMsg->GetSize();

	for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
	{
		if(i != m_idProcess)		// 不回送
			m_pMsgPort->Send( i, MAPGROUP_PHYLEMSG, STRUCT_TYPE( buf ), pPacket );
	}
	return true;
}
bool	CMapGroup::TransmitMsgToWorld( CNetMsg* pMsg)//向世界发
{
	for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
	{
		m_pMsgPort->Send(MSGPORT_WORLD, WORLD_TRANSMITNETMSG, STRUCT_TYPE(pMsg), pMsg);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////
// 广播给其它地图组核心处理
bool CMapGroup::TransmitMsg(CNetMsg* pMsg, SOCKET_ID idSocket, OBJID idNpc)
{
	ASSERT(idSocket != SOCKET_NONE || idNpc != ID_NONE);

	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= idSocket;
	pPacket->idNpc		= idNpc;
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->nTrans		= pMsg->GetTransData();
	for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
	{
		if(i != m_idProcess)		// 不回送
			m_pMsgPort->Send(i, MAPGROUP_TRANSMITMSG, STRUCT_TYPE(buf), pPacket);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// 广播给其它地图组核心处理
void CMapGroup::RemoteCall(REMOTE_CALL0* pInfo,bool bSendToLocal)
{
	for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
	{
		if (i != m_idProcess || bSendToLocal)
			m_pMsgPort->Send(i, MAPGROUP_REMOTECALL, BUFFER_TYPE(pInfo->nSize), pInfo);
	}
}

//////////////////////////////////////////////////////////////////////
// 广播给指定地图组处理
bool CMapGroup::TransmitMapGroupMsg(PROCESS_ID idProcess,CNetMsg* pMsg)
{
	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= pMsg->GetSocketID();
	pPacket->idNpc		= pMsg->GetNpcID();
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->nTrans		= pMsg->GetTransData();

//	return m_pMsgPort->Send(idProcess, MAPGROUP_SENDMSG,BUFFER_TYPE(pMsg->GetSize()), pMsg->GetBuf());
	return m_pMsgPort->Send(idProcess, MAPGROUP_SENDMSG, STRUCT_TYPE(buf), pPacket);
}

//////////////////////////////////////////////////////////////////////
// 广播给世界核心处理
bool CMapGroup::TransmitWorldMsg(CNetMsg* pMsg)
{
	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= pMsg->GetSocketID();
	pPacket->idNpc		= pMsg->GetNpcID();
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->nTrans		= pMsg->GetTransData();

	m_pMsgPort->Send(MSGPORT_WORLD, WORLD_TRANSMITMSG, STRUCT_TYPE(buf), pPacket);
	return true;
}

//////////////////////////////////////////////////////////////////////
// 通过世界核心转发给玩家
bool CMapGroup::TransmitWorldMsg(CNetMsg* pMsg, LPCTSTR szName)
{
	MESSAGESTR	buf;
	USERNAMEMSG_PACKET0* pPacket = (USERNAMEMSG_PACKET0*)buf;
	pPacket->idSocket	= pMsg->GetSocketID();
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	SafeCopy(pPacket->szName, szName, _MAX_NAMESIZE);
	m_pMsgPort->Send(MSGPORT_WORLD, WORLD_USERNAMEMSG, STRUCT_TYPE(buf), pPacket);

	return true;
}

//////////////////////////////////////////////////////////////////////
// 通过世界核心转发给玩家
bool CMapGroup::TransmitWorldMsg(CNetMsg* pMsg, OBJID idUser)
{
	MESSAGESTR	buf;
	USERIDMSG_PACKET0* pPacket = (USERIDMSG_PACKET0*)buf;
	pPacket->idSocket	= pMsg->GetSocketID();
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->idUser		= idUser;
	m_pMsgPort->Send(MSGPORT_WORLD, WORLD_USERIDMSG, STRUCT_TYPE(buf), pPacket);

	return true;
}

//////////////////////////////////////////////////////////////////////
// 通过世界核心转发给关系服务器
bool CMapGroup::TransmitWorldMsg2(CNetMsg* pMsg, OBJID idUser)
{
	MESSAGESTR	buf;
	USERIDMSG_PACKET0* pPacket = (USERIDMSG_PACKET0*)buf;
	pPacket->idSocket	= m_idProcess;//pMsg->GetSocketID();
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->idUser		= idUser;
	m_pMsgPort->Send(MSGPORT_WORLD, WORLD_RELATIONMSG, STRUCT_TYPE(buf), pPacket);
	
	return true;
}

bool CMapGroup::SendObjInfo(PROCESS_ID idProcess, OBJID idUser, INFO_ID nInfoID, void* pInfo, int nSize)
{
	CHECKF(nInfoID > INFO_NONE && nInfoID < INFO_END); // 20 最多20种INFO
	MESSAGESTR	buf;
	CHANGEMAPGORUP_INFO0* pPacket		= (CHANGEMAPGORUP_INFO0*)buf;
	pPacket->idUser		= idUser;
	pPacket->nInfoType	= nInfoID;
	memcpy(&pPacket->info, pInfo, nSize);
	return m_pMsgPort->Send(idProcess, MAPGROUP_SENDOBJINFO, STRUCT_TYPE(buf), pPacket);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::SendSocketUserInfo(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo, ULONG nIP)
{
	// 通知另一地图组
	MESSAGESTR	buf;
	ST_USERCHANGEMAPGORUP* pPacket = (ST_USERCHANGEMAPGORUP*)buf;
	pPacket->idSocket	= idSocket;
	pPacket->nIP		= nIP;
	memcpy(&pPacket->info, pInfo, sizeof(UserInfoStruct));
	return m_pMsgPort->Send(idProcess, MAPGROUP_SOCKETUSERINFO, STRUCT_TYPE(ST_USERCHANGEMAPGORUP), &buf);
}

//////////////////////////////////////////////////////////////////////
// bool CMapGroup::SetNpcProcessID(OBJID idNpc, bool bAddNew)
// {
// 	CHANGE_NPCPROCESSID	buf;
// 	buf.idNpc		= idNpc;
// 	if(bAddNew)
// 		buf.idProcess	= m_idProcess;
// 	else
// 		buf.idProcess	= PROCESS_NONE;
// 	return m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SETNPCPROCESSID, STRUCT_TYPE(CHANGE_NPCPROCESSID), &buf);
// }

//////////////////////////////////////////////////////////////////////
//20070329修罗:新加一个参数
bool CMapGroup::ChangeMapGroup(PROCESS_ID idProcess,
							   OBJID idUser, 
							   OBJID idMap, 
							   int nPosX, 
							   int nPosY,
							   Enum_ChangeMapGroupMode mode/* = ENUM_CNGMAPGROUP_MODE_PROC*/,
							   void *lpBooth)//[游途道标 2009.05.12]摊位
{
	//修罗:区分[请求]和[响应处理]
	switch(mode)
	{
	case ENUM_CNGMAPGROUP_MODE_REQ://请求
		{
			ST_CHANGEMAPGROUP	st;
			st.idUser	= idUser;
			st.idMap	= idMap;
			st.nPosX	= nPosX;
			st.nPosY	= nPosY;
			st.idSourceProgress = m_idProcess;//用于反馈
			return m_pMsgPort->Send(idProcess, MAPGROUP_CHANGEMAPGROUP_REQ, STRUCT_TYPE(ST_CHANGEMAPGROUP), &st);
		}
		break;
	case ENUM_CNGMAPGROUP_MODE_PROC://响应处理
		{
			ST_CHANGEMAPGROUP	st;
			st.idUser	= idUser;
			st.idMap	= idMap;
			st.nPosX	= nPosX;
			st.nPosY	= nPosY;
			st.idSourceProgress = m_idProcess;//用于反馈
			st.lpBooth	= lpBooth;//[游途道标 2009.05.12]
			return m_pMsgPort->Send(idProcess, MAPGROUP_CHANGEMAPGROUP, STRUCT_TYPE(ST_CHANGEMAPGROUP), &st);
		}
		break;
	case ENUM_CNGMAPGROUP_MODE_ACK://同意
		{
			ST_CHANGEMAPGROUP	st;
			st.idUser	= idUser;
			st.idMap	= idMap;
			st.nPosX	= nPosX;
			st.nPosY	= nPosY;
			st.idSourceProgress = m_idProcess;//用于反馈
			return m_pMsgPort->Send(idProcess, MAPGROUP_CHANGEMAPGROUP_ACK, STRUCT_TYPE(ST_CHANGEMAPGROUP), &st);
		}
		break;
	case ENUM_CNGMAPGROUP_MODE_NACK://拒绝
		{
			ST_CHANGEMAPGROUP	st;
			st.idUser	= idUser;
			st.idMap	= idMap;
			st.nPosX	= nPosX;
			st.nPosY	= nPosY;
			st.idSourceProgress = m_idProcess;//用于反馈
			return m_pMsgPort->Send(idProcess, MAPGROUP_CHANGEMAPGROUP_NACK, STRUCT_TYPE(ST_CHANGEMAPGROUP), &st);
		}
		break;
	default:
		{
			//error!
			LOGERROR( "CMapGroup::ChangeMapGroup函数出错,idUser=%d idMap=%d nPosX=%d nPosY=%d mode=%d",
				idUser,idMap,nPosX,nPosY,mode );
			return false;
		}
		break;
	}

	return false;


	
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::PrintText			(LPCTSTR szText)
{
	return m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::CloseMapGroup(SOCKET_ID idGmSocket)		// 关闭所有socket(除GM)，禁止登录
{
	// TODO: 请在此添加关闭客户端SOCKET的代码
	return m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_BREAKALLCONNECT, VARTYPE_INT, &idGmSocket);

	RoleManager()->SaveAll();
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::ChangeTeam(int nAction, OBJID idTeam, OBJID idUser, int nData1/*=0*/, int nData2/*=0*/, int nData3/*=0*/, int nData4/*=0*/,char *teamName/* = NULL*/)			// 组队相关
{
	ST_CHANGETEAM	st;
	memset(&st,0,sizeof(ST_CHANGETEAM));//20070202修罗

	st.nAction	= nAction;
	st.idTeam	= idTeam;
	st.idUser	= idUser;
	st.nData1	= nData1;
	st.nData2	= nData2;
	st.nData3	= nData3;
	st.nData4	= nData4;
	if(teamName)
		strncpy(st.teamName,teamName,sizeof(char)*23);
	for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
	{
		if(i != m_idProcess)		// 不回送
			m_pMsgPort->Send(i, MAPGROUP_CHANGETEAM, STRUCT_TYPE(st), &st);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::ModifyNpc(OBJID idNpc, LPCTSTR szField, LPCTSTR szData)
{
	char	buf[1024];
	char*	ptr = buf;
	*(int*)ptr	= idNpc;
	ptr	+= sizeof(int);
	strcpy(ptr, szField);
	ptr	= ptr + strlen(szField) + 1;
	strcpy(ptr, szData);
	ptr = ptr + strlen(szData) + 1;

	for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
	{
		if(i != m_idProcess)		// 不回送
			m_pMsgPort->Send(i, MAPGROUP_CHANGENPC, BUFFER_TYPE(ptr - buf), buf);
	}
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::DelTransNpc(OBJID idMasterNpc, bool bBroadcast)
{
	for(IRoleSet::Iter i = RoleManager()->QuerySet()->Begin(); i != RoleManager()->QuerySet()->End(); i++)
	{
		IRole* pRole = RoleManager()->QuerySet()->GetObjByIter(i);
		if(pRole)
		{
			CNpc* pNpc;
			if(pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
			{
// 				if(pNpc->GetType() == _SYNTRANS_NPC && pNpc->GetInt(NPCDATA_LINKID) == idMasterNpc)
// 					ASSERT(pNpc->DelNpc());
			}
		}
	}

	if(bBroadcast)
	{
		for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
		{
			if(i != m_idProcess)		// 不回送
				m_pMsgPort->Send(i, MAPGROUP_DELTRANSNPC, VARTYPE_INT, &idMasterNpc);
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::ChangeCode (SOCKET_ID idSocket, DWORD dwData)
{
	CHANGE_USERDATA	st;
	st.idSocket		= idSocket;
	st.nData		= dwData;
	m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CHANGECODE, STRUCT_TYPE(CHANGE_USERDATA), &st);
}

//////////////////////////////////////////////////////////////////////
// syndicate
//////////////////////////////////////////////////////////////////////
bool CMapGroup::CreateSyndicate		(const CreateSyndicateInfo* pInfo)
{
	return m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_CREATESYNDICATE, STRUCT_TYPE(CreateSyndicateInfo), pInfo);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::DestroySyndicate(OBJID idSyn, OBJID idTarget)
{
	OBJID	setID[2];
	setID[0]	= idSyn;
	setID[1]	= idTarget;
	return m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_DESTROYSYNDICATE,	STRUCT_TYPE(setID), &setID);
}

//////////////////////////////////////////////////////////////////////
bool CMapGroup::ChangeSyndicate		(const SynFuncInfo0* pFuncInfo)
{
	return m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_CHANGESYNDICATE, BUFFER_TYPE(pFuncInfo->nSize), pFuncInfo);
}

bool CMapGroup::StopSayByGM(char* szname, int sec)//  [8/13/2007 Add By HaLei] 
{
	GM_STOPSAY st;
	SafeCopy(st.szname,szname,16);
	st.sec = sec;
	return m_pMsgPort->Send(MSGPORT_WORLD, WORLD_STOPSAY,STRUCT_TYPE(GM_STOPSAY),&st);
}

//////////////////////////////////////////////////////////////////////
int CMapGroup::GetMapGroupAmount()
{
	CHECKF(m_pMsgPort);
	return m_pMsgPort->GetSize() - MSGPORT_MAPGROUP_FIRST + 1;
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::LevelUp		(OBJID idUser, int nLevel)
{
	//20070406修罗:将idUser和nLevel全部转到世界线程
	//在另端获得得同步
	//m_pMsgPort->Send(MSGPORT_WORLD, WORLD_LEVELUP, VARTYPE_INT, &idUser);
	__int64 idUserAndLevel = idUser;
	idUserAndLevel = (idUserAndLevel<<32) | nLevel;
	m_pMsgPort->Send(MSGPORT_WORLD, WORLD_LEVELUP, STRUCT_TYPE(__int64), &idUserAndLevel);
}

//////////////////////////////////////////////////////////////////////
void CMapGroup::QueryFee		(OBJID idAccount)
{
	m_pMsgPort->Send(MSGPORT_WORLD, WORLD_QUERYFEE, VARTYPE_INT, &idAccount);
}


bool CMapGroup::TransmitWorldMsg(SOCKET_ID idSocket,CNetMsg *pMsg)
{
	MESSAGESTR	buf;
	TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
	pPacket->idSocket	= idSocket;
	pPacket->idNpc		= pMsg->GetNpcID();
	pPacket->idPacket	= pMsg->GetType();
	memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
	pPacket->nSize		= pMsg->GetSize();
	pPacket->nTrans		= pMsg->GetTransData();

	m_pMsgPort->Send(MSGPORT_WORLD, MAPGROUP_TRANSMITMSG, STRUCT_TYPE(buf), pPacket);

	return true;
}
//---------------------------------
//20070122修罗:开始更新角色卡
BOOL CMapGroup::NowUpdateCalculatedProperty(PROCESS_ID idProcess,OBJID userID)
{
	return m_pMsgPort->Send(idProcess, MAPGROUP_NOWUPDATE_CALCULATED_PROPERTY, STRUCT_TYPE(DWORD), &userID);
}



void CMapGroup::DoBroadcastMsg()
{
	CTime time(CTime::GetCurrentTime());
	int nhour = time.GetHour();
	if(nHourseState != nhour)
	{
		if (MainServerFlag()==1)//只有主服务器才可以生成随机数
		{
			int nRad = ::RandGet(10000);
			//通知关系服务器把随机数转发给其他游戏服务器
			CMsgMainServer msg;
			msg.Create(2,nRad*100+nhour,true);
			OBJID idUser = 0;
			TransmitWorldMsg(&msg, idUser);

			char szLuaName[64];
			if (nRad < 1666)//点苍
				sprintf(szLuaName, "ontimer/ontimer1.lua");
			else if (nRad < 3333)//全真
				sprintf(szLuaName, "ontimer/ontimer2.lua");
			else if (nRad < 5000)//峨嵋
				sprintf(szLuaName, "ontimer/ontimer3.lua");
			else if (nRad < 6666)//唐门
				sprintf(szLuaName, "ontimer/ontimer4.lua");
			else if (nRad < 8333)//丐帮
				sprintf(szLuaName, "ontimer/ontimer5.lua");
			else//少林
				sprintf(szLuaName, "ontimer/ontimer6.lua");
			LuaScriptMachine()->InitMapOntimerScript(nhour);
			if (strlen(szLuaName)>0)
				LuaScriptMachine()->RunScriptFile(szLuaName);
			nHourseState = nhour;

			//特价商品
			if (nhour == 5)
			{
				if (m_nDayState != time.GetDay())
				{
					if (VASItem()->GenerateASaleItem(ASALE_ITEM_AMOUNT))
					{
						CMsgMainServer msg2;
						msg2.Create(3,0,true);
						OBJID idUser = 0;
						TransmitWorldMsg(&msg2, idUser);
						m_nDayState = time.GetDay();
					}
				}
			}

		}
	}
	
	int nMin  = time.GetMinute();
	
	DEBUG_TRY

	if (m_circletime.IsActive())
	{
//modify code by 林德渊 begin 2011.5.17
// 		for( int i_num = 0 ; i_num < LIN_CIRCLE_NUM; i_num++ )
// 		{
// 			if( g_m_circleTime_list[i_num] > 0 && g_m_circletime_list[i_num].IsActive() )
// 			{
// 				if( g_m_circletime_list[i_num].ToNextTick(g_m_circleTime_list[i_num]) && strcmp(g_m_circle_list[i_num], "STOP") != 0 )
// 				{
// 					if( g_m_circle_list[i_num][0] == 'A' && g_m_circle_list[i_num][1] == 'L' )
// 					{
// 						CString szmsg = g_m_circle_list[i_num];
// 						if( szmsg.Find("ALAL") == 0 && szmsg.GetLength() > 5 )
// 						{
// 							char newschar[256];
// 							sscanf(g_m_circle_list[i_num],"ALAL%s",newschar);
// 							strcpy(g_m_circle_list[i_num], newschar);
// 							g_m_circleTime_list[i_num] = -1;
// 							g_m_circletime_list[i_num].SetInterval(g_m_circleTime_list[i_num]);
// 							g_m_circletime_list[i_num].Update();
// 							UserManager()->BroadcastMsg(g_m_circle_list[i_num], NULL, NULL, 0xff0000, _TXTATR_TOPANNOUNCE);
// 						}
// 						else
// 						{
// 							if( szmsg.Find("ALSI") == 0 && szmsg.GetLength() > 6 )
// 							{
// 								char newschar[256];
// 								int nnums = 0;
// 								sscanf(g_m_circle_list[i_num],"ALSI%d_%s",&nnums,newschar);
// 								strcpy(g_m_circle_list[i_num], newschar);
// 								g_m_circleTime_list[i_num] = nnums * 60;
// 								g_m_circletime_list[i_num].SetInterval(g_m_circleTime_list[i_num]);
// 								g_m_circletime_list[i_num].Update();
// 							}
// 							UserManager()->BroadCastMsgAndReceiveName(g_m_circle_list[i_num], NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, NULL, 0, 0, "" , ALLUSERS_NAME);
// 						}
// 					}
// 					else
// 						UserManager()->BroadCastMsgAndReceiveName(g_m_circle_list[i_num], NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, NULL, 0, 0, "" , ALLUSERS_NAME);
// 				}
// 			}
//		}
//modify code by 林德渊 end
/*
		if (m_circletime.ToNextTick(m_circleTime) && strcmp(m_circle, "STOP") != 0)
		{
// 			CMsgTalk msg;
// 			if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, m_circle, NULL, 0xff0000, _TXTATR_ANNOUNCE))
// 				UserManager()->BroadcastMapGroupMsg(&msg);
			UserManager()->BroadCastMsgAndReceiveName(m_circle, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, NULL, 0, 0, "" , ALLUSERS_NAME);
		}*/
	}
// 	if ((strlen(m_circle) != 0) && strcmp(m_circle, "") != 0 &&  strcmp(m_circle, "STOP") != 0)
// 	{
// 		CMsgTalk msg;
// 		if (m_circleFlag == true)
// 		{
// 			if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, m_circle, NULL, 0xff0000, _TXTATR_ANNOUNCE))
// 						UserManager()->BroadcastMapGroupMsg(&msg);
// 			m_circletime = m_circleTime;
// 			m_circleTime = (nMin + m_circletime) % 60;
// 			m_circleFlag = false;
// 		}
// 		else if(nMin == m_circleTime )
// 		{
// 			if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, m_circle, NULL, 0xff0000, _TXTATR_ANNOUNCE))
// 				UserManager()->BroadcastMapGroupMsg(&msg);
// 			m_circleTime = (nMin + m_circletime) % 60;
// 		}
// 	}
// 	else 	if( strcmp(m_circle, "STOP") == 0)
// 	{
// 		m_circleTime = -1;
// 		m_circleFlag = false;
// 	}
	DEBUG_CATCH("CIRCLE broad 4 gmtool")

// 	if (5 == nMin)
// 	{
// 		switch(nhour)
// 		{
// 		case 11:
// 		case 14:
// 		case 17:	
// 		case 20:
// 		case 23:
// 			{
// 				if(weekState == -1 || weekState == 0)
// 				{
// 					weekState = 1;
// 					CMsgTalk	msg;
// 					char		szTemp[256];
// 					char		szmsg[512];
// 					sprintf(szTemp, "%d_蝴蝶谷、云渡山、正气坡和西野郡郊_%d", SNAPSHOTNOTHING, COLORPINK);
// 					sprintf(szmsg, "在<%s>出现了大批山贼，玩家可以组队前往征讨！可获得大量经验和宝物！", szTemp);
// 					if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, szmsg, NULL, 0x00a651, _TXTATR_TOPANNOUNCE))
// 						UserManager()->BroadcastMapGroupMsg(&msg);
// 				}
// 			}
// 			break;
// 		case 12:
// 		case 15:
// 		case 18:
// 		case 21:
// 		case 0:
// 			{
// 				if(weekState == -1 || weekState == 1)
// 				{
// 					weekState = 0;
// 					CMsgTalk	msg;
// 					if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, "山贼在大家共同讨伐下已经退却，但他们放下话来说，还会再回来的", NULL, 0x00a651, _TXTATR_TOPANNOUNCE))
// 						UserManager()->BroadcastMapGroupMsg(&msg);
// 				}
// 			}
// 			break;
// 		}
// 	}
// 	if (nhour == 10 || nhour == 13 || nhour == 16 || nhour == 19 || nhour == 22)
// 	{
// 		if (-1 == MartialAct || 0 == MartialAct)
// 		{
// 			MartialAct = 1;
// 		}
// 	}
// 	if (nhour == 11 || nhour == 14 || nhour == 17 || nhour == 20 || nhour == 23)
// 	{
// 		if (-1 == MartialAct || 1 == MartialAct)
// 		{
// 			MartialAct = 0;
// 			CMsgTalk	msg;
// 			if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, "江湖恶霸连番兴风作浪，六大门派联手御敌，重创奸邪叛徒，终于解除了这场危机！", NULL, 0x00a651, _TXTATR_TOPANNOUNCE))
// 				UserManager()->BroadcastMapGroupMsg(&msg);		
// 		}
// 	}
// 	if(nMin == 45)
// 	{
// 		switch(nhour)
// 		{
// 		case	11:
// 		case	19:
// 			{
// 				if (-1 == Hunt || 1 == Hunt)
// 				{
// 					Hunt = 0;
// 					CMsgTalk	msg;
// 					if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, "<3_龙门荒漠_6>正式开放，请35级以上的玩家3人组队做好准备，在<3_江陵（26，74）贾镖头_9>或<3_开封（31，84）甄镖头_9>处进入大漠，挑战强悍的<3_胡一斧_7>！。", NULL, 0x00a651, _TXTATR_TOPANNOUNCE))
// 						UserManager()->BroadcastMapGroupMsg(&msg);	
// 				}
// 			}
// 			break;
// 		case	14:
// 		case	22:
// 			{	
// 				if (-1 == Hunt || 0 == Hunt)
// 				{
// 					Hunt = 1;
// 					CMsgTalk	msg;
// 					if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, "<3_胡一斧_7>终敌不过江湖中人的齐力一击，落荒退败，龙门荒漠又恢复了昔日的平静。不过他已经在悄悄的酝酿下一次的计划了。", NULL, 0x00a651, _TXTATR_TOPANNOUNCE))
// 						UserManager()->BroadcastMapGroupMsg(&msg);	
// 				}
// 			}
// 			break;
// 		}
// 	}
// 	if (30 ==nMin)
// 	{
// 		switch(nhour)
// 		{
// 			case 11:
// 			case 19:
// 			{
// 				if (-1 == AltarAct || 0 == AltarAct)
// 				{
// 					AltarAct = 1;
// 					CMsgTalk	msg;
// 					if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, "<3_巫蛊祭坛_6>活动已经开启，请各位玩家做好战前准备，<3_3人_8>以上组队，找<3_江陵（97，105）李双双_9>或<3_开封（119，109）程双双_9>参加。", NULL, 0x00a651, _TXTATR_TOPANNOUNCE))
// 						UserManager()->BroadcastMapGroupMsg(&msg);		
// 				}
// 			}
// 			break;
// 		
// 			case 14:
// 			case 21:
// 			{
// 				if (-1 == AltarAct || 1 == AltarAct)
// 				{
// 					AltarAct = 0;
// 					CMsgTalk	msg;
// 					if(msg.Create( SYSTEM_NAME, ALLUSERS_NAME, "<3_逆天苍_6>施展巫蛊之术逆天而行，被武林人士一举歼灭，真是脍炙人口的一桩喜讯！", NULL, 0x00a651, _TXTATR_TOPANNOUNCE))
// 						UserManager()->BroadcastMapGroupMsg(&msg);		
// 				}
// 			}
// 			break;
// 		}
// 	}


/////////////////////////////////////////////////////////////////帮派的维护放在这里处理
}

bool CMapGroup::SendTutorToWorld(const ChgTutorInfo * pInfo)
{
	return m_pMsgPort->Send(MSGPORT_WORLD, WORLD_TUTOR_INFO, STRUCT_TYPE(ChgTutorInfo), pInfo);
}

void CMapGroup::AddCircle(char *szWords, int CircleTime)
{
/*
	strcpy(m_circle, szWords);
	m_circleTime = CircleTime * 60;
	if (m_circleTime < 300)
		return;
	if (strcmp(szWords, "STOP") != 0)
	{
		m_circletime.SetInterval(m_circleTime);
		m_circletime.Update();
		m_circleFlag = true;
	}
	else
		m_circleFlag = false;
	return;*/
//modify code by 林德渊 begin 2011.5.6
// 	if( CircleTime * 60 < 300 )
// 		return;
// 	int b_fl = 0;//添加单条循环
// 	int total_num = 0;
// 	bool add_fl = true;
// 	if( strlen(szWords) >= strlen("STOP_") && szWords[0] == 'S' && szWords[1] == 'T' && szWords[2] == 'O' && szWords[3] == 'P' )
// 	{
// 		b_fl = 1;//删除单条循环
// 		if( strlen(szWords + strlen("STOP_")) == 0 )
// 			b_fl = 2;//删除所有循环
// 	}
// 
// 	for( int i_num = 0; i_num < LIN_CIRCLE_NUM; i_num++ )
// 	{
// 		if( 0 == b_fl )
// 		{
// 			if( g_m_circleTime_list[i_num] <= 0 )
// 			{
// 				if( add_fl )
// 				{
// 					strcpy(g_m_circle_list[i_num], szWords);
// 					g_m_circleTime_list[i_num] = CircleTime * 60;
// 					g_m_circletime_list[i_num].SetInterval(g_m_circleTime_list[i_num]);
// 					g_m_circletime_list[i_num].Update();
// 					m_circletime.SetInterval(CircleTime * 60);
// 					m_circletime.Update();
// 					m_circleFlag = true;
// 					add_fl = false;
// 					total_num++;
// 				}
// 			}
// 			else
// 				total_num++;
// 		}
// 		else if( 1 == b_fl && g_m_circleTime_list[i_num] > 0 )
// 		{
// 			if( strcmp(szWords + strlen("STOP_"), g_m_circle_list[i_num]) == 0 )
// 				g_m_circleTime_list[i_num] = -1;
// 			total_num++;
// 		}
// 		else if( 2 == b_fl && g_m_circleTime_list[i_num] > 0 )
// 		{
// 			g_m_circleTime_list[i_num] = -1;
// 		}
// 	}
// 	if( 2 == b_fl || 0 == total_num )
// 	{
// 		m_circletime.Clear();
// 		m_circleFlag = false;
// 	}
//modify code by 林德渊 end
}
