// MapInstance.cpp: implementation of the CMapInstance class.
#include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
#include "MapInstanceMgr.h"
//#include "DataObject.h"
#include "MapInstance.h"
#include "MsgMapInstance.h"
#include "allmsg.h"
#include "MapTrap.h"
#include "CombGod.h"
#include "SKPostOffice.h"
#include "ActionDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int CMapInstance::InstanceTeam = 0;
const int CMapInstance::InstanceSyn = 1;
const int CMapInstance::InstanceCity =2;
extern int g_SpecilLine;


CMapInstance::CMapInstance()
{
//	m_nInstanceType=0;///留做扩展
//	m_nInstanceStatus=0;///留做扩展
	m_idMap=0;///副本所在地图id
	m_idOwn=0;///所有者编号,可能是队伍ID或是playerid
	m_idOwnProcess=0;///副本拥有者地图组ID
	m_idProcess=0;
	m_pData=NULL;

	INSTANCE_EXPIRED_TIME=30;///超时时限秒
	INSTANCE_EXPIRED_TEAMTIME=30;///超时离队容忍时限秒
	INSTANCE_EXPIRED_LIFETIME = 0;///超时离队容忍时限秒
	INSTANCE_EXPIRED_RECYCLETIME=60;///超时离队容忍时限秒
	
// 	INSTANCE_COUNTDOWN_TIME = 120;//倒计时秒数，默认设定为120秒

	INSTANCE_USER_MAX_NUMBERS = 1500;//副本中玩家个数限制上限
	INSTANCE_USER_MIN_NUMBERS = 0;//副本中玩家个数限制下限

	INSTANCE_NEWGENERATE_TIME = 0; //副本刷怪时间段
//	m_bSetExpiredTime = FALSE;

	m_nLeaveSynTime = 30;//退帮容忍时限 default: 30秒
	m_nCityWarVictoryTime = 30; //城战胜利倒计时  default: 30秒
	
	m_nDomainMapID = 0;				//可以占领的区域
	m_nAttackSynID = 0;				//攻方军团ID
	m_nDefenceSynID = 0;			//防守方军团ID
	m_nUserLev = 0;

	m_nAttackBossID = 0;
	m_nDefenceBossID = 0;

	m_CityWarClose = false;
	m_CityMsg = false;

	m_nMapType = 0;

	m_nRebornAttackX = 0;
	m_nRebornAttackY = 0;

	m_nRebornDefenceX = 0;
	m_nRebornDefenceY = 0;

	m_posRebornX = 0;
	m_posRebornY = 0;

	m_nLimitCityWar = 60;
	m_dwKickPlayerInterval = 0;

	m_bArenaEnd = false;

	m_nSWABCount = 0;
	m_nSWDBCount = 0;
	m_nSWBeKillMark = 0;

	instanceData[0] = 0;
	instanceData[1] = 0;
	m_nActionForTimer = 0;

	m_nMonsterTypeEvent=0;///怪物类型
	m_nMonsterCountEvent=0;///怪物数量
	m_nMonsterEventContext=0;///怪物事件上下文

	m_bAllowIn = true;
	m_bNeedKickout = false;
	m_SetFreeUser.clear();
}

CMapInstance::~CMapInstance()
{
	BOOL hr=false;

	int nStep = 0;
	DEBUG_TRY
	//zeng删除sk_dynamap表中的副本记录信息
	char sqlstr[128];
	sprintf(sqlstr,"delete from %s where id = %d " ,_TBL_DYNAMAP,GetMapID());
	nStep = 1;
	IDatabase* pDb = Database();
	nStep = 2;
	if(pDb)
		pDb->ExecuteSQL((char*)(const char*)sqlstr);

	nStep = 3;
	hr=RemoveInstance();
	nStep = 4;
	m_SetFreeUser.clear();
	DEBUG_CATCH2("CMapInstance::~CMapInstance() nStep = %u",nStep);
}

void CMapInstance::NotifyOwnDestory()
{
	CMsgMapInstance msg(m_idOwnProcess);
	msg.CreateNotifyInstanceCloseMsg(GetInstanceType(),m_idOwn);
	CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(GetOwnProcessID(),&msg);
}

BOOL CMapInstance::RemoveInstance()///清除副本,和InitMapInstance相对
{
	BOOL hr=false;
	hr=RemoveInstanceMap();
	NpcManager()->CloseMonsterGenerat(GetMapID(),0);///0表示关闭所有
	///自我退订所有处理事件对象
	slotUserLeaveTeam.disconnect(this);
//	slotUserRejoinTeam.disconnect(this);
	slotLifeTimeout.disconnect(this);
	return hr;
}

BOOL CMapInstance::IsActive()///判断副本是否过期,目前简单规则,人数和定时器;
{
	return m_tLifeTimeInstance.IsActive();

	//return (!m_tLifeTimeInstance.IsTimeOut());
}

BOOL CMapInstance::IsTimeOut()///判断副本是否超时
{
	return m_tLifeTimeInstance.IsTimeOut();
}

void CMapInstance::SetLeaveSynExpire(UINT nExpire)
{
	m_nLeaveSynTime = nExpire;
}

UINT CMapInstance::GetLeaveSynExpire()
{
	return m_nLeaveSynTime;
}


void CMapInstance::NewGenerat(OBJID idGen)
{
	NpcManager()->GeneratMonster(GetMapID(),idGen);
}

void CMapInstance::GeneratMonster(UINT nMonsterType,UINT nPosX,UINT nPosY)
{
	NpcManager()->GeneratMonster(GetMapID(),nMonsterType,nPosX,nPosY);
}

BOOL CMapInstance::RunScriptMonsterKill(int nAction)
{
	bool hr=false;
	DEBUG_TRY
	RegisterLuaInterfaceOntime(nAction);
	hr=LuaScriptMachine()->RunScriptFile("instanceMonsterKill.lua");
	///提前副本脚本错误
	if(LuaScriptMachine()->IsHasError())
		LOGERROR(LuaScriptMachine()->GetLastError());
	//		SendInstanceMsg(LuaScriptMachine()->GetLastError());
	DEBUG_CATCH("CMapInstance::RunScriptMonsterKill")
	return hr;
}

BOOL CMapInstance::RunScriptOntime(int nAction)
{	
	bool hr=false;
	DEBUG_TRY
	RegisterLuaInterfaceOntime(nAction);
	hr=LuaScriptMachine()->RunScriptFile("instanceOntime.lua");
	///提前副本脚本错误
	if(LuaScriptMachine()->IsHasError())
		LOGERROR(LuaScriptMachine()->GetLastError());
//		SendInstanceMsg(LuaScriptMachine()->GetLastError());
	DEBUG_CATCH("CMapInstance::RunScript")
	return hr;
}
BOOL CMapInstance::RunScript(InstaceStatus isStatus)
{
	BOOL hr=false;
	DEBUG_TRY
	RegisterLuaInterface();

	switch(isStatus)
	{
		case isInit:
			{
				hr=LuaScriptMachine()->RunScriptFile(m_pData->GetStr("init_script"));
				if(LuaScriptMachine()->IsHasError())
					LOGERROR(LuaScriptMachine()->GetLastError());
			}
			break;
// 		case isJoin:
// 			hr=LuaScriptMachine()->RunScriptFile(m_pData->GetStr("join_script"));
// 			break;
		default:
			break;
	}
	DEBUG_CATCH("CMapInstance::RunScript")
	return hr;
}

void CMapInstance::RegisterLuaInterfaceOntime(int nAction)
{
	RegisterLuaInterface();
	LuaScriptMachine()->InitMapOntimerScript(nAction);
}


//zeng所有需要注册的接口都放到这个地方注册，统一管理，不能在其他地方进行注册，因为别的函数需要调用本函数一次性的注册函数
void CMapInstance::RegisterLuaInterface()
{
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","NewGenerat",this,&CMapInstance::NewGenerat);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","ShutdownInstance",this,&CMapInstance::ShutdownInstance);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CloseMonsterGenerat",this,&CMapInstance::CloseMonsterGenerat);

	//zeng开放下面超时的接口给策划
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetNoUserExpire",this,&CMapInstance::SetNoUserExpire);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetRecycleExpire",this,&CMapInstance::SetRecycleExpire);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetLeaveTeamExpire",this,&CMapInstance::SetLeaveTeamExpire);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetInstanceLife",this,&CMapInstance::SetInstanceLife);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetUserMaxNumbers",this,&CMapInstance::SetUserMaxNumbers);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetUserMinNumbers",this,&CMapInstance::SetUserMinNumbers);
 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetCountDownInstance",this,&CMapInstance::SetCountDownInstance);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetOutInstancePos",this,&CMapInstance::SetOutInstancePos);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetInstanceActiveOut",this,&CMapInstance::SetInstanceActiveOut);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetInstanceLev",this,&CMapInstance::SetUserLev);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","OperateInstanceData",this,&CMapInstance::OperateData);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetInstanceTimer",this,&CMapInstance::SetInstanceTimer);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","GetOperateData",this,&CMapInstance::GetOperateData);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetMonsterKillEvent",this,&CMapInstance::SetMonsterKillEvent);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CreateInstanceNpc",this,&CMapInstance::CreateInstanceNpc);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CloseInstance",this,&CMapInstance::CloseInstance);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","DeleteNpc",this,&CMapInstance::DeleteNpc);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CreateMonsterInInstance",this,&CMapInstance::CreateMonsterInInstance);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SendInstanceMsg",this,&CMapInstance::SendInstanceMsg);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","MoveMonsterDest",this,&CMapInstance::MoveMonsterDest);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CreateInstanceGameTrap",this,&CMapInstance::CreateInstanceGameTrap);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetBufferUserID",this,&CMapInstance::SetBufferUserID);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetAttackBufferNPC",this,&CMapInstance::SetAttackBufferNPC);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetDefenceBufferNPC",this,&CMapInstance::SetDefenceBufferNPC);
 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","DeleteMonster",this,&CMapInstance::DeleteMonster);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","StartUpNewGenTime",this,&CMapInstance::StartUpNewGen);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","IsAttackFull",this,&CMapInstance::IsAttackFull);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","IsDefenceFull",this,&CMapInstance::IsDefenceFull);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetLeaveSynExpire",this,&CMapInstance::SetLeaveSynExpire);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CreateCityWarAttackBoss",this,&CMapInstance::CreateCityWarAttackBoss);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CreateCityWarDefenceBoss",this,&CMapInstance::CreateCityWarDefenceBoss);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetCityWarVictoryExpire",this,&CMapInstance::SetCityWarVictoryExpire);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetInstanceRebornPos",this,&CMapInstance::SetInstanceRebornPos);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","HasCityWarDefence",this,&CMapInstance::HasCityWarDefence);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CreateCityWarTransMonster",this,&CMapInstance::CreateCityWarTransMonster);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","GetInstanceType",this,&CMapInstance::GetInstanceType);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","HasMonster",this,&CMapInstance::HasMonster);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CreateChessMonsters",this,&CMapInstance::CreateChessMonsters);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","SetRebronPos",this,&CMapInstance::SetRebronPos);	
	
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","BuyBeDestroy",this,&CMapInstance::BuyBeDestroy);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","ChgMapBeDestroy",this,&CMapInstance::ChgMapBeDestroy);
// 	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","TaskBeDestroy",this,&CMapInstance::TaskBeDestroy);
}
///触发初始化事件
void CMapInstance::OnInit()///处理副本生命周期超时事件
{
	///向NPC服务器发请求,要求对我进行刷怪
	// synchro
	RunScript(isInit);///刷怪
}

BOOL CMapInstance::InitMapInstance(int idProcess,PROCESS_ID idOwnProcess,OBJID idOwn,UINT nInstanceType,UINT nInstanceScope,UINT nDoMain)///初始化副本,可在这里面刷怪或做一些准备工作
{
	BOOL hr=false;
	SetOwnID(idOwn);

	m_pData=InstanceTypeSet()->GetObj(nInstanceType);
	CHECKF(m_pData);

	m_idProcess=idProcess;
	m_idOwnProcess=idOwnProcess;
	m_InstanceType = nInstanceType;
	m_nInstanceScope = nInstanceScope;
	m_nDomainMapID = nDoMain;
	m_nUserLev = min(nDoMain,150);

	m_OutMapID = m_pData->GetInt("returnmap");
	m_OutCellX = m_pData->GetInt("return_x");
	m_OutCellY = m_pData->GetInt("return_y");

	m_nMapType = m_pData->GetInt("maptype");

	hr=CreateInstanceMap();///创建副本地图
	if(hr)
	{
		
		///自我订阅处理事件对象
 		slotUserLeaveTeam.connect(this,&CMapInstance::OnUserQuitTeam);
// 		slotUserRejoinTeam.connect(this,&CMapInstance::OnUserRejoinTeam);
		slotLifeTimeout.connect(this,&CMapInstance::OnLifeTimeout);
		slotInit.connect(this,&CMapInstance::OnInit);
		slotInit();///触发初始化事件

// 		m_tLifeTimeInstance.Startup(GetLifeTime());///开始副本生命周期计时
// 		
// 		if(GetLifeTime()>INSTANCE_COUNTDOWN_TIME)
// 		{
// 			m_tCountDownInstance.Startup(GetLifeTime()-INSTANCE_COUNTDOWN_TIME);
// 		}
// 
// 		if(m_nInstanceScope==INSTANCETYPE_ARENA)
// 		{
// 			m_ArenaBegin.Startup(30);
// 		}
	}
	return hr;
}

void CMapInstance::AddAttackPlayer(OBJID idPlayer)
{
	if(0 == idPlayer)
		return ;

	ITMEM it = find(m_vecAttackMember.begin(), m_vecAttackMember.end(), idPlayer);

	if(it == m_vecAttackMember.end())
	{
		m_vecAttackMember.push_back(idPlayer);
	}
}

void CMapInstance::AddDefencePlayer(OBJID idPlayer)
{
	if(0 == idPlayer)
		return ;

	ITMEM it = find(m_vecDefenceMember.begin(), m_vecDefenceMember.end(), idPlayer);

	if(it == m_vecDefenceMember.end())
	{
		m_vecDefenceMember.push_back(idPlayer);
	}
}

// bool CMapInstance::CitySynWarJoin(OBJID idUser, int nType)
// {
// 	CUser* pUser = UserManager()->GetUser(idUser);
// 	if(NULL == pUser)
// 		return false;
// 
// 	if( nType == INSTANCETYPE_CITYWAR)
// 	{
// 		pUser->SetCitySynWarType(CITYSYNWAR_CITYWAR);
// 		pUser->SetCityWarDoMain(m_nDomainMapID);
// 		pUser->SetCityWarInstanceType(m_InstanceType);
// 		pUser->AttachStatus(pUser, STATUS_CITYWAR_SAFETIME, 50, 7);
// 		CMsgUserAttrib msg;
// 		if(pUser->IsCityWarAttack(m_nDomainMapID))
// 		{
// 			if(msg.Create(idUser, _USERATTRIB_CITYWARE_ATTACKDEFENCE, 1))
// 			{
// 				msg.Append(_USERATTRIB_CITYWARE_DOMAIN, m_nDomainMapID);
// 		//		msg.Append(_USERATTRIB_CITYSYNWAR_TYPE, CITYSYNWAR_CITYWAR);
// 				MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 				pUser->BroadcastRoomMsg(&msg, true);
// 			}
// 	 		ProcessSynCityMemberPos(pUser, true);
// 			AddAttackPlayer(idUser);
// 			
// 			pUser->SetInstanceRebornPos(m_nRebornAttackX, m_nRebornAttackY);
// 
// 			CRole::AttachStatus(pUser, STATUS_CITYWAR_ATTACKMARK, 50, -1);
// 			
// 		}
// 		else if(pUser->IsCityWarDefence(m_nDomainMapID))
// 		{
// 			if(msg.Create(idUser, _USERATTRIB_CITYWARE_ATTACKDEFENCE, 2))
// 			{
// 				msg.Append(_USERATTRIB_CITYWARE_DOMAIN, m_nDomainMapID);
// //				msg.Append(_USERATTRIB_CITYSYNWAR_TYPE, CITYSYNWAR_CITYWAR);
// 				MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 				pUser->BroadcastRoomMsg(&msg, true);
// 			}
//  			ProcessSynCityMemberPos(pUser, false);
// 			AddDefencePlayer(idUser);
// 			
// 			pUser->SetInstanceRebornPos(m_nRebornDefenceX, m_nRebornDefenceY);
// 
// 			CRole::AttachStatus(pUser, STATUS_CITYWAR_DEFENCEMARK, 50, -1);
// 		}
// 		else
// 		{
// 			if(msg.Create(idUser, _USERATTRIB_CITYWARE_ATTACKDEFENCE, 0))
// 			{
// 				msg.Append(_USERATTRIB_CITYWARE_DOMAIN, m_nDomainMapID);
// //				msg.Append(_USERATTRIB_CITYSYNWAR_TYPE, CITYSYNWAR_CITYWAR);
// 				MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 				pUser->BroadcastRoomMsg(&msg, true);
// 			}
// 		}
// 	}
// 	else if(nType == INSTANCETYPE_SYNWAR)
// 	{
// 		pUser->SetCitySynWarType(CITYSYNWAR_SYNWAR);
// 	
// 		pUser->SetCityWarInstanceType(m_InstanceType);
// 		pUser->AttachStatus(pUser, STATUS_CITYWAR_SAFETIME, 50, 7);
// 		
// 		CMsgUserAttrib msg;
// 		if(SYNWARTYPE_ATTACK == pUser->GetSynWarJoinType())
// 		{
// 			if(msg.Create(idUser, _USERATTRIB_CITYWARE_ATTACKDEFENCE, 1))
// 			{
// 				msg.Append(_USERATTRIB_CITYWARE_DOMAIN, pUser->GetSynWarJoinID());
// //				msg.Append(_USERATTRIB_CITYSYNWAR_TYPE, CITYSYNWAR_SYNWAR);
// 				MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 				pUser->BroadcastRoomMsg(&msg, true);
// 			}
// 	 
// 			AddAttackPlayer(idUser);
// 			
// 			pUser->SetInstanceRebornPos(m_nRebornAttackX, m_nRebornAttackY);
// 
// 			CRole::AttachStatus(pUser, STATUS_CITYWAR_ATTACKMARK, 50, -1);
// 			
// 		}
// 		else if(SYNWARTYPE_DEFENCE == pUser->GetSynWarJoinType())
// 		{
// 			if(msg.Create(idUser, _USERATTRIB_CITYWARE_ATTACKDEFENCE, 2))
// 			{
// 				msg.Append(_USERATTRIB_CITYWARE_DOMAIN, pUser->GetSynWarJoinID());
// //				msg.Append(_USERATTRIB_CITYSYNWAR_TYPE, CITYSYNWAR_SYNWAR);
// 				MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 				pUser->BroadcastRoomMsg(&msg, true);
// 			}
// 
//  	    	AddDefencePlayer(idUser);
// 			
// 			pUser->SetInstanceRebornPos(m_nRebornDefenceX, m_nRebornDefenceY);
// 
// 			CRole::AttachStatus(pUser, STATUS_CITYWAR_DEFENCEMARK, 50, -1);
// 		}
// 		else
// 		{
// 			if(msg.Create(idUser, _USERATTRIB_CITYWARE_ATTACKDEFENCE, 0))
// 			{
// 				msg.Append(_USERATTRIB_CITYWARE_DOMAIN, pUser->GetSynWarJoinID());
// //				msg.Append(_USERATTRIB_CITYSYNWAR_TYPE, CITYSYNWAR_SYNWAR);
// 				MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 				pUser->BroadcastRoomMsg(&msg, true);
// 			}
// 		}
// 	}
// 	else if(nType == INSTANCETYPE_TEAM)
// 	{
// 		pUser->ProcessAction(9530); //副本计数限制action
// 	}
// 
// 	return true;
// }


void CMapInstance::EnterScript(OBJID idUser)
{
	CUser* pUser = UserManager()->GetUser(idUser);
	if(NULL == pUser)
		return;

	//在这里看看队伍
	if(GetInstanceScope() == INSTANCETYPE_TEAM) //队伍副本看看补登方式
	{		
		if(!pUser->GetTeam())
		{
			LOGWARNING("进入队伍副本的玩家，没有队伍,将会被踢出!!");
			return;
		}
		
		TeamMemberInfo stInfo;
		if(!pUser->GetTeam()->GetMemberInfo(stInfo,idUser))
		{
			LOGWARNING("进入队伍副本的玩家，有队伍,队伍里面没自己信息!!");
			return;
		}

		//都进来了不用补登了
		pUser->GetTeam()->SetLocation(idUser,0,0,0,true);
	}

	if(!IsFreeUser(pUser->GetID()) && !pUser->m_bInstanceReborn)///玩家在副本重生时不调用进入副本的脚本
	{
		//合法
		if(InstanceUseMgr()->CheckUseInstance(pUser->GetID(),m_pData->GetID()) == 1)
		{
			//状态2
			InstanceUseMgr()->UseInstance(pUser->GetID(),m_pData->GetID(),2);
			//跑脚本
			LPCTSTR strScriptFile=m_pData->GetStr("join_script");
			if(strlen(strScriptFile))
				GameAction()->ProcessScriptAction(ACTION_SCRIPT_PRESENT,strScriptFile,0,pUser,NULL);
			//以后进本副本免费
			addFreeUser(pUser->GetID());
			return;
		}
		
		//LOGWARNING("time没加就要进副本？instancetype = %u",GetInstanceType());
		return;
	}


	pUser->m_bInstanceReborn=false;
}

BOOL CMapInstance::AddPlayer(OBJID idUser)
{
	
	CUser* pUser = UserManager()->GetUser(idUser);
	if(NULL == pUser)
		return false;

	BOOL hr=true;
	CTeam *pTeam=NULL;
	DEBUG_TRY
	m_InstanceUserList[idUser].SetID(idUser);
	m_InstanceUserList[idUser].SetSynMemTime(::TimeGet(TIME_MILLISECOND));
	m_tExpiredInstance.Clear();///stop定时器
//	m_bSetExpiredTime = FALSE;	//重新设置超时定时器

	TeamMemberInfo menberInfo;
	ZeroMemory(&menberInfo,sizeof(TeamMemberInfo));

	CHECKF(pUser->GetInfo());///这个不能为空
	pTeam=UserManager()->GetTeamById(pUser->GetInfo()->idTeam);
	
	if(pTeam)
		pTeam->GetMemberInfo(menberInfo,pUser->GetID());

	///在副本中下线的玩家重上线不再执行进入副本的脚本，避免重复扣道具
// 	if(!IsFreeUser(pUser->GetID()) && !pUser->m_bInstanceReborn)///玩家在副本重生时不调用进入副本的脚本
// 	{
// 		if(!pUser->m_bInstanceReborn)
// 		{
// 			if(InstanceUseMgr()->CheckUseInstance(pUser->GetID(),m_pData->GetID()) == 1)
// 			{
// 				InstanceUseMgr()->UseInstance(pUser->GetID(),m_pData->GetID(),2);
// 				InstanceUseMgr()->ClearUsing(pUser->GetID(),m_pData->GetID());
// 				LPCTSTR strScriptFile=m_pData->GetStr("join_script");
// 				if(strlen(strScriptFile))
// 					hr=GameAction()->ProcessScriptAction(ACTION_SCRIPT_PRESENT,strScriptFile,0,pUser,NULL);
// 
// 				addFreeUser(pUser->GetID());
// 			}
// 			
// // 		}
// 	}
// 	pUser->m_bInstanceReborn=false;
// 	if(pTeam)
// 		pTeam->SetLocation(pUser->GetID(),0,0,0);///清空
	DEBUG_CATCH2("CMapInstance::AddPlayer(OBJID idUser):%d",idUser)
// 	DWORD nActionJoin = m_pData->GetInt("join_script");
// 	pUser->ProcessAction(nActionJoin);

	///跟踪看是否有不属于这个副本的玩家也进了这个副本
	if(GetInstanceScope()==INSTANCETYPE_USER && pUser->GetID()!=GetOwnID())
		LogSave("CMapInstance::AddPlayer error!有玩家进了不属于他的副本!userid:%d,instancetype:%d",pUser->GetID(),GetInstanceType());
	else if(GetInstanceScope()==INSTANCETYPE_TEAM && pTeam && pTeam->GetID()!=GetOwnID())
		LogSave("CMapInstance::AddPlayer error!有玩家进了不属于他队伍的副本!userid:%d,instancetype:%d",pUser->GetID(),GetInstanceType());

	return hr;
	
}

BOOL CMapInstance::IsEmpty()
{
	return m_InstanceUserList.size()==0;
}

BOOL CMapInstance::CityWarBeKill(CUser* pUser)
{
	return TRUE;
}

BOOL CMapInstance::RemovePlayer(OBJID idUser)
{
	BOOL hr=true;

	for(InstanceUserList::iterator it=m_InstanceUserList.begin();it!=m_InstanceUserList.end();++it)
	{
		if (it->second.GetID()==idUser)
		{
			m_InstanceUserList.erase(it);
			break;///必须break,否则出错
		}
	}
	return hr;
}

BOOL CMapInstance::IsRecycle()///判断副本是否回收,目前简单规则,人数和定时器;
{
	return m_tRecycleInstance.TimeOver();
}

BOOL CMapInstance::KickOutPlayer(OBJID idPlayer,OBJID idMap, int nPosX, int nPosY)///踢出指定玩家
{
	BOOL hr=false;
	CUserPtr pUser=UserManager()->GetUser(idPlayer);
	if (pUser)
	{
		pUser->ClearUsingInstance();
		if (g_SpecilLine==0)
		{
			if(idMap == 0 && nPosX == 0 && nPosY == 0)
				hr=pUser->FlyMap(pUser->GetRecordMapID(),pUser->GetRecordPosX(),pUser->GetRecordPosY());
			else
				hr=pUser->FlyMap(idMap,nPosX,nPosY);
		}
		else if (g_SpecilLine==1)//专线副本*注意*
		{
			if(idMap == 0 && nPosX == 0 && nPosY == 0)
			{
				if (pUser->GetSpecilMapID()==pUser->GetSynID())
					hr=pUser->FlyMap(pUser->GetSynMap()->GetID(),pUser->GetSpecilPosX(),pUser->GetSpecilPosY());
				else
					hr=pUser->FlyMap(pUser->GetSpecilMapID(),pUser->GetSpecilPosX(),pUser->GetSpecilPosY());
			}
			else
				hr=pUser->FlyMap(idMap,nPosX,nPosY);
		}
		if(hr == FLYMAP_ERROR)
		{
			hr=pUser->RebornUseScroll(false,true);
		}
	}
	return hr;
}


BOOL CMapInstance::KickOutAllPlayer()///踢出所有玩家
{
	BOOL hr=false;
	for(InstanceUserList::iterator it=m_InstanceUserList.begin();it!=m_InstanceUserList.end();it++)
		hr=KickOutPlayer(it->second.GetID(),m_OutMapID,m_OutCellX,m_OutCellY);
	//m_InstanceUserList.clear();			//zeng屏蔽该行
	m_vecAttackMember.clear();
	m_vecDefenceMember.clear();
	ClearFreeUser();

	//队伍副本，清补登
	if(GetInstanceScope() == INSTANCETYPE_TEAM)
	{
		OBJID idTeam = GetOwnID();
		CTeam* pTeam = UserManager()->GetTeamById(idTeam);
		if(pTeam)
			pTeam->ResetLocation(true);
	}
	m_dwKickPlayerInterval = ::TimeGet(TIME_MILLISECOND);
	return hr;
}


BOOL CMapInstance::ActiveKickAllOut()///踢出所有玩家
{
	BOOL hr=false;
	for(InstanceUserList::iterator it=m_InstanceUserList.begin();it!=m_InstanceUserList.end();it++)
	{
		if(it->second.GetBeKickOut())
			continue;

		CUser* pUser = UserManager()->GetUser(it->second.GetID());
		hr=KickOutPlayer(it->second.GetID(),pUser->GetRecordMapID(),pUser->GetRecordPosX(),pUser->GetRecordPosY());
		it->second.SetBeKickout();
	}
		
	//m_InstanceUserList.clear();			//zeng屏蔽该行
	m_vecAttackMember.clear();
	m_vecDefenceMember.clear();
	m_dwKickPlayerInterval = ::TimeGet(TIME_MILLISECOND);
	return hr;
}

void CMapInstance::ClearUser()
{
	InstanceUserList::iterator it = m_InstanceUserList.begin();
	for (;it!=m_InstanceUserList.end();)
	{
		CUser* pUser = UserManager()->GetUser(it->first);
		if(!pUser)
		{
			it = m_InstanceUserList.erase(it);
		}
		else
		{
			if(::TimeGet(TIME_MILLISECOND) - m_dwKickPlayerInterval > MUSTKICKOUT_INTERVAL)
			{
				KickOutPlayer(it->second.GetID(),m_OutMapID,m_OutCellX,m_OutCellY);
				m_dwKickPlayerInterval = ::TimeGet(TIME_MILLISECOND);
			}
			++it;
		}
	}
}

BOOL CMapInstance::RemoveInstanceMap()///删除副本创建它所拥有的地图,向地图管理器发出请求
{
	BOOL hr=false;
	///在调用地图管理器之前判断一下它是否已析构
	if (MapManager()) 
	{
		//hr=MapManager()->DeleteDynaMap(GetMapID());
		CGameMap* pMap=MapManager()->GetGameMap(GetMapID(),false);
		if(!pMap)
		{
			LOGWARNING("RemoveInstanceMap error instancetype = %u",GetInstanceType());
			return true;
			//CHECKF(pMap);
		}
		//pMap->EraseMap();
		pMap->EraseInstanceMap();
	}
	return hr;
}

void CMapInstance::ShutdownInstance(UINT nTimeOut)///处理副本生命周期超时事件
{
	m_tLifeTimeInstance.Startup(nTimeOut);
}

void CMapInstance::OnLifeTimeout()///处理副本生命周期超时事件
{
	CloseInstance();///关闭副本,当生命周期结束或副本目标达成时调用
	//KickOutAllPlayer();///向NPC发出请求,要求删除生成器
	
}

void CMapInstance::CloseMonsterGenerat(OBJID idGen)
{
	NpcManager()->CloseMonsterGenerat(GetMapID(),idGen);
}

BOOL CMapInstance::CloseSynWarInstance(bool bAttack)
{
	m_tLifeCityWarVictory.Startup(m_nCityWarVictoryTime);
	char msgbuf[100];
	sprintf(msgbuf,"系统将在%d秒后关闭副本,请玩家做好退出副本的准备!",m_nCityWarVictoryTime);
	SendInstanceMsg(msgbuf);

	SynWarAward(bAttack);

	if(bAttack)
	{
		DeleteMonster(m_nAttackBossID);
	}
	else
	{
		DeleteMonster(m_nDefenceBossID);
	}

	return TRUE;
}

BOOL CMapInstance::CloseCityWarInstance(bool bAttack)
{
	m_tLifeCityWarVictory.Startup(m_nCityWarVictoryTime);
	char msgbuf[100];
	sprintf(msgbuf,"系统将在%d秒后关闭副本,请玩家做好退出副本的准备!",m_nCityWarVictoryTime);
	SendInstanceMsg(msgbuf);

	if(bAttack)
	{
		DeleteMonster(m_nDefenceBossID);
	}
	else
	{
		DeleteMonster(m_nAttackBossID);
	}

	return TRUE;
}

BOOL CMapInstance::CloseInstance()///关闭副本,当生命周期结束或副本目标达成时调用
{
	BOOL hr=false;
	///踢出所有玩家
	hr=KickOutAllPlayer();
	///有超时则不理,没有超时则让它马上超时
	//if (IsTimeOut()==false)
	//	m_tLifeTimeInstance.Startup(0);

	m_tLifeTimeInstance.Clear();

//	NotifyOwnDestory();///通知副本拥有者,副本已关闭

	return hr;
}

BOOL CMapInstance::CreateInstanceMap()///为副本创建它所拥有的地图,向地图管理器发出请求
{
	///初期实现时,实例类型就对应地图类型,如果以后要扩展就是加一个InstanceType表
	SetMapID(MapManager()->CreateDynaMap(GetMapType()));
	return GetMapID()!=ID_NONE;
}

void CMapInstance::OnUserQuitTeam(OBJID idUser)
{
	if(m_nInstanceScope==INSTANCETYPE_TEAM)
	{
		///如果玩家离队,则马上启动玩家离队定时器
// 		if (m_InstanceUserList[idUser].GetTeamLimitTimer().IsActive()==false)
// 		{
// 			m_InstanceUserList[idUser].GetTeamLimitTimer().Startup(GetLeaveTeamExpire());
// 			///debug
// 			//[游途道標 2009.03.09]修改提示信息
// 			CUser* pUser = UserManager()->GetUser(idUser);
// 			if(pUser)
// 		    	pUser->SendSysMsg("你已离开队伍,%d秒后将被踢出",GetLeaveTeamExpire());
// 		}

		///到点,踢出该玩家
//		if (m_InstanceUserList[idUser].GetTeamLimitTimer().TimeOver())
		KickOutPlayer(idUser,m_OutMapID,m_OutCellX,m_OutCellY);
	}

}


void CMapInstance::OnUserRejoinTeam(OBJID idUser)///处理玩家回队事件
{
}

///这可以开放给脚本处理
BOOL CMapInstance::IsUserLevelTeam(CUserPtr UserPtr)
{
//	if (GetType()==itRoomInstance)
//		return false;
//	else if (UserPtr==NULL)
//		return false;

// 	if(m_nInstanceScope==0)
// 	{
	if(!UserPtr)
		return true;
		
	else if (UserPtr->GetTeam())
	{
		if(UserPtr->GetTeam()->GetID()!=GetOwnID())
		{
			LOGWARNING("发现副本出现非本队成员!");
			return true;
		}
		return false;
	}
	else
		return true;
// 	}
// 	else
// 		return FALSE;			//其他副本类型不存在离队的情况
}

BOOL CMapInstance::ProcessPlayerEvent(CInstanceUser& userptr)//////处理单个玩家事件
{
	BOOL hr=false;
	CUserPtr UserPtr=UserManager()->GetUser(userptr.GetID());
	if (UserPtr==NULL)
	{
		///玩家可能掉线了,从副本中删除该玩家
		RemovePlayer(userptr.GetID());
		return hr;
	}

	if(!UserPtr->CanBeSee())
		return hr;

	///检查副本中的玩家有没有离队,如果离队就起启动他的离队定时器
	if(INSTANCETYPE_TEAM == m_nInstanceScope)
	{		
		if(IsUserLevelTeam(UserPtr))
			slotUserLeaveTeam(userptr.GetID());
	}
// 	else if(INSTANCETYPE_CITYWAR == m_nInstanceScope)  //城战副本处理
// 	{	
// 		if(!(UserPtr->IsAlive()))
// 		{
// 			CityWarBeKill(UserPtr);
// 		}
// 		else if(IsUserLevelSyn(UserPtr))
// 		{
// 			UserQuitSyn(userptr.GetID());
// 		}
// 		else
// 		{
// 			UserRejoinSyn(userptr.GetID());
// 		}
// 
// 		DWORD nTime = ::TimeGet(TIME_MILLISECOND);
// 		if(::TimeGet(TIME_MILLISECOND) - userptr.GetSynMemTime() > SYNMEMPOSUPDATE_INTERVAL)
// 		{
// 			userptr.SetSynMemTime(::TimeGet(TIME_MILLISECOND));
// 			if(UserPtr->IsCityWarAttack(m_nDomainMapID))
// 			{
// 				ProcessSynCityMemberPos(UserPtr, true);
// 			}
// 			else
// 			{
// 				ProcessSynCityMemberPos(UserPtr, false);
// 			}
// 		}
// 	}
// 	else if(INSTANCETYPE_SYNWAR == m_nInstanceScope) //帮战副本
// 	{
// 		if(IsUserLevelSyn(UserPtr))
// 		{
// 			UserQuitSyn(userptr.GetID());
// 		}
// 		else
// 		{
// 			UserRejoinSyn(userptr.GetID());
// 		}
// 	}
	return hr;
}

BOOL CMapInstance::ProcessAllPlayerEvent()//////处理所有玩家被踢出的情况
{
	BOOL hr=false;

	BOOL bDefence = false;
	BOOL bAttack = false;
	///无奈啊,要是我有BOOST,就不用写得这么啰嗦了.
	for(InstanceUserList::iterator it=m_InstanceUserList.begin();it!=m_InstanceUserList.end();++it)
	{
		ProcessPlayerEvent(it->second);
	}

	return hr;
}

BOOL CMapInstance::CanEnterInstance()
{
	//在这里预留一定的时间进行副本的回收
	
//	bool bok = false;

// 	if(m_bSetExpiredTime && m_tExpiredInstance.IsTimeOut(m_tExpiredInstance.GetInterval()))
//  	if(m_tExpiredInstance.IsActive())
//  		return bok;
// 	if(!m_tLifeTimeInstance.IsActive())
// 		return bok;
// 	if(m_tLifeTimeInstance.IsTimeOut(m_tLifeTimeInstance.GetInterval()-10))
// 		return bok;
// 	
// 	bok = true;


	return m_bAllowIn;
}

///处理怪物特定事件
void CMapInstance::ProcessMonsterEvent()
{
	const UINT nMonsterEventDelay=10000;//10秒
	static UINT nUpdateMonsterTime=GetTickCount();
	if(m_nMonsterTypeEvent==ID_NONE)
		return;
	if(GetTickCount()>(nUpdateMonsterTime+nMonsterEventDelay))
	{
		nUpdateMonsterTime=GetTickCount();
		bool bCheck=HasMonster2(m_nMonsterTypeEvent,m_nMonsterCountEvent);
		if(bCheck==false)
		{
			RunScriptMonsterKill(m_nMonsterEventContext);
			m_nMonsterEventContext=0;
			m_nMonsterTypeEvent=0;
			m_nMonsterCountEvent=0;
		}
	}
}

bool CMapInstance::IsLeaveSave()
{
	if(m_bNeedKickout)
	{
		return false;
	}
	return true;
}

BOOL CMapInstance::ProcessInstanceEvent()///处理副本自身事件
{
	BOOL hr=false;	

	//竞技场处理
//	ProcessArena();


	if(m_bNeedKickout)
	{
		if(!IsEmpty())
			ActiveKickAllOut();
	}

	///如果副本生命周期到了就踢出所有玩家,并将回收定时器立即置为timeout
	if(m_tTimeActiveOut.IsActive())
	{
		if(m_tTimeActiveOut.IsTimeOut())
		{
			m_tTimeActiveOut.Clear();
			ActiveKickAllOut();

			m_bNeedKickout = true;
			m_bAllowIn = false;
		}
	}
	if (m_tExpiredInstance.TimeOver())
	{
		m_tLifeTimeInstance.Startup(0);///立即超时

		m_bNeedKickout = true;
		m_bAllowIn = false;
	}

	if(m_tLifeTimeInstance.TimeOver())
	{
		SendInstanceMsg("副本已经关闭");
		m_tLifeTimeInstance.Clear();
		slotLifeTimeout();

		m_bNeedKickout = true;
		m_bAllowIn = false;
	}
	///当副本没人了,就开动超时定时器
	if(IsEmpty())
	{
		if(m_tLifeTimeInstance.IsActive())
		{
			if(m_tLifeTimeInstance.IsTimeOut(10) && m_tExpiredInstance.IsActive()==false)///副本开启10秒后才开始判断空副本的情况
			{
				m_tExpiredInstance.Startup(GetNoUserExpire());
				m_tLifeTimeInstance.Startup(5);
				m_bNeedKickout = true;
				m_bAllowIn = false;
			}
		}
	}


	if (m_tLifeTimeNewGenerage.TimeOver())//副本刷怪计时
	{
		NpcManager()->CloseMonsterGenerat(GetMapID(),0);///0表示关闭所有
#ifdef _DEBUG 
		char msgbuf[100];
		sprintf(msgbuf,"%s","刷怪时间完了关闭所有newGen");
		SendInstanceMsg(msgbuf);
#endif
	}


	if(m_bAllowIn)
	{
		if(m_tLifeTimeInstance.IsTimeOut(m_tLifeTimeInstance.GetInterval()-10))
			m_bAllowIn = false;
	}

	if(m_bNeedKickout)
	{
		if(m_tForDelay.IsActive())
		{
			if(m_nActionForTimer == ID_NONE)
			{
				m_tForDelay.Clear();
			}
			else if(m_tForDelay.TimeOver())
			{
				RunScriptOntime(m_nActionForTimer);
				// 			m_tForDelay.Clear();
			}
		}
		///处理怪物特定事件
		ProcessMonsterEvent();
	}

	return hr;
}

void CMapInstance::OnTimer(time_t tCurr)///定时器处理函数
{
	BOOL hr=false;

	DEBUG_TRY
	hr=ProcessAllPlayerEvent();///轮循所有玩家可以产生的事件情况
	DEBUG_CATCH("ProcessAllPlayerEvent")
	///实时处理

	DEBUG_TRY
	hr=ProcessInstanceEvent();///处理副本自身事件
	DEBUG_CATCH("ProcessInstanceEvent")
}

UINT  CMapInstance::SetRebronPos(UINT posX,UINT posY)
{
	m_posRebornX = posX;
	m_posRebornY = posY;
	return 0;
}

///生成
UINT  CMapInstance::CreateChessMonsters(UINT nMonsterTypeA,int nMonsterACount,UINT nMonsterTypeB,int nMonsterBCount,int StartX,int StartY,int nMonsterWidth)
{
	DEBUG_TRY
	OBJID idMonsterType=0;
	UINT nTotalMonster=nMonsterACount+nMonsterBCount;///得到怪物总数
	int nCurrentWidth=0,nCurrentHeight=0;///怪物矩阵分布高与宽
	const int nOffset=3;///怪物分布坐标偏移
	///根据怪物数据与种类,生成怪物type分布数列
	for(int i=0;i<nTotalMonster;++i)
	{
		if(nMonsterACount>0 && rand()%2)
		{
			idMonsterType=nMonsterTypeA;
			nMonsterACount--;
		}
		else if(nMonsterBCount>0)
		{
			idMonsterType=nMonsterTypeB;
			nMonsterBCount--;
		}
		else if(nMonsterACount>0)
		{
			idMonsterType=nMonsterTypeA;
			nMonsterACount--;
		}

		///把这只怪刷到地图上
		if(nCurrentWidth>=nMonsterWidth)
		{
			nCurrentWidth=0;
			nCurrentHeight++;
		}
		// 		afxDump<<idMonsterType<<":"<<StartX+nCurrentWidth*nOffset<<":"<<StartY+nCurrentHeight*nOffset<<"\n";
		CreateMonsterInInstance(idMonsterType,StartX+nCurrentWidth*nOffset,StartY+nCurrentHeight*nOffset);
		nCurrentWidth++;
	}
	///每次刷棋盘怪自动订阅事情
	SetMonsterKillEvent(nMonsterTypeA,1,10);
	DEBUG_CATCH("CMapInstance::CreateChessMonsters")
	return true;
}

UINT CMapInstance::CreateInstanceNpc(UINT nNpcType,int cellx,int celly)
{
	UINT NpcID = MapGroup(m_idProcess)->GetNpcManager()->CreateDynaNpc(nNpcType,m_idMap,cellx,celly);
	CNpc* pNpc = NpcManager()->GetNpc(NpcID);
	if(pNpc)
	{
		m_InstanceNpcs.insert(make_pair(NpcID,NPCINFO(nNpcType,pNpc)));
		pNpc->SendSelfToBlock();
	}
	return NpcID;
}

bool CMapInstance::DeleteNpc(UINT nNpcType)
{
	map<UINT,NPCINFO>::iterator iter;

	for(iter=m_InstanceNpcs.begin();iter!=m_InstanceNpcs.end();++iter)
	{
		CNpc* pNpc = (iter->second).pNpc;
		if((iter->second).NpcType==nNpcType)
		{
			pNpc->DelNpc();		
			m_InstanceNpcs.erase(iter);
			return true;
		}
	}

	return false;
}

void CMapInstance::CreateMonsterInInstance(UINT nMonsterType,int cellx,int celly)
{
	int nStep = 0;
	DEBUG_TRY
//modify code by 林德渊 begin 2011.6.22
//	nMonsterType += m_nUserLev;
	if( !(nMonsterType > 6100 && nMonsterType < 6400) )
		nMonsterType += m_nUserLev;
//modify code by 林德渊 end
	ST_CREATENEWNPC	info;

	memset(&info,0,sizeof(info));

	nStep = 1;
	info.idMap			= GetMapID();
	info.idData			= 0;//此id保证在sk_generator中存在
	info.idOwner		= 0;
	info.usPosX			= cellx;
	info.usPosY			= celly;
	
	info.usType			= nMonsterType;
	info.ucOwnerType	= 0;
	info.nData			= 0;
	
	nStep = 2;
// 	NpcManager()->CreateSynPet(&info);
	OBJID idMonster=NpcManager()->CreateSynPetReturnID(&info,"");
	///创建怪物成功后将怪物id压入vector保存
	if(idMonster!=ID_NONE)
	{
		nStep = 3;
		///订阅这只怪的死亡事件
		CMonster *monsterptr=(CMonster*)RoleManager()->QueryRole(idMonster);
		if(monsterptr)
		{
			nStep = 4;
//			m_Monster.push_back(idMonster);
			///怪物数量加1
			m_MonsterCount[nMonsterType]++;
			monsterptr->slotOnMonsterBeKill.connect(this,&CMapInstance::OnMonsterBeKill);
		}
	}
	DEBUG_CATCH2("CMapInstance::CreateMonsterInInstance nStep = %d",nStep)
}

void CMapInstance::OnMonsterBeKillEvent(UINT nMonsterType)
{
	DEBUG_TRY
	///判断这种怪剩下数量是否满足条件
	nMonsterType-=m_nUserLev;///修正重复的添加
	bool bCheck=HasMonster(nMonsterType,m_nMonsterCountEvent);
	if(bCheck==false)
	{
		RunScriptMonsterKill(m_nMonsterEventContext);
 		m_nMonsterEventContext=0;
 		m_nMonsterTypeEvent=0;
 		m_nMonsterCountEvent=0;
	}
	DEBUG_CATCH("CMapInstance::OnMonsterBeKillEvent")
}

void CMapInstance::OnMonsterBeKill(OBJID idMonster)
{
	DEBUG_TRY
	UINT nMonsterType=0;
	///怪物自然死亡时只需简单删除所保存的怪物id
	CMonster *monsterptr=(CMonster*)RoleManager()->QueryRole(idMonster);
	if(monsterptr)
	{
		nMonsterType=monsterptr->GetType();
		if(m_MonsterCount[nMonsterType]>0)
		{
			m_MonsterCount[nMonsterType]--;
			///如果被死的这种怪刚好是被订阅的怪物类型与数量则触发脚本
			if(m_nMonsterTypeEvent>0 && m_nMonsterTypeEvent==nMonsterType)
				OnMonsterBeKillEvent(nMonsterType);
		}
// 	for(vector<OBJID>::iterator it = m_Monster.begin();it!=m_Monster.end();++it)
// 	{
// 		if(*it==idMonster)
// 		{
// 			m_Monster.erase(it);
// 			break;
// 		}
// 	}
	}
	DEBUG_CATCH("CMapInstance::OnMonsterBeKill")
}

BOOL CMapInstance::DeleteMonster(int nType)
{
	// 		CMonster* pKillerMonster= NULL;
	// 		pKiller = UserManager()->GetUser(m_vEnmity[ i ].userID);//在本地图组
	// 		if(!pKiller)
	// 		{
	// 			IRole* pRoleEudemon = RoleManager()->QueryRole(m_vEnmity[ i ].userID);
	// 			if()

// 	DEBUG_TRY
// 	nType += m_nUserLev;
// 	OBJID idMonster=ID_NONE;
// 	CMonster *monsterptr=NULL;
// 	vector<OBJID>::iterator it = m_Monster.begin();
// 	while (it != m_Monster.end())
// 	{
// 		idMonster=*it;
// 		monsterptr=(CMonster*)RoleManager()->QueryRole(idMonster);
// 		if(monsterptr==NULL)
// 		{
// 			it=m_Monster.erase(it);
// 			continue;
// 		}
// 		if(monsterptr->IsDeleted())
// 		{
// 			it=m_Monster.erase(it);
// 			continue;
// 		}
// 		if(monsterptr->GetType() == nType)//检查是否需要删除
// 		{
// 			RoleManager()->QuerySet()->DelObj(idMonster);
// 			it=m_Monster.erase(it);
// 			continue;
// 		}
// 		++it;
// 	}
// 	DEBUG_CATCH("CMapInstance::DeleteMonster")

	DEBUG_TRY
	nType += m_nUserLev;

	IRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
	while(pRole.Next())
	{
		CMonster* pMonster;
		if(pRole) 
		{
			if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
			{
				CGameMap *pMap = pMonster->GetMap();
				if(pMonster->IsDeleted())
					continue;

				if(pMap && pMonster->GetType() == nType && pMap->GetID() == GetMapID())
				{

					if(m_MonsterCount[pMonster->GetType()]>0)
						m_MonsterCount[pMonster->GetType()]--;

					RoleManager()->QuerySet()->DelObj(pRole);
				}
			}
		}
	}
	DEBUG_CATCH("CMapInstance::DeleteMonster(int nType)")

	return true;
}

BOOL CMapInstance::IsInstanceUserFull()
{
	return m_InstanceUserList.size() >= GetUserMaxNumbers();
}

void CMapInstance::SendInstanceMsg(const char* pMsg)
{
	InstanceUserList::iterator iter;
	
	for(iter = m_InstanceUserList.begin();iter!=m_InstanceUserList.end();++iter)
	{
		CUser* pUser = UserManager()->GetUser(iter->first);
		if(pUser)
			pUser->SendSysMsg(/*_TXTATR_NORMAL,*/pMsg);
	}
}

void CMapInstance::MoveMonsterDest(UINT ntype,int x,int y)
{
	NpcManager()->MoveMonsterDest(m_idMap,ntype,x,y);
}

void CMapInstance::SetOutInstancePos(UINT idMap,int cellx,int celly)
{
	m_OutMapID = idMap;
	m_OutCellX = cellx;
	m_OutCellY = celly;
}

void CMapInstance::SetInstanceRebornPos(int attackPosX, int attackPosY, int defencePosX, int defencePosY, int nType)
{
	m_nRebornAttackX = attackPosX;
	m_nRebornAttackY = attackPosY;

	m_nRebornDefenceX = defencePosX;
	m_nRebornDefenceY = defencePosY;
}


//开启刷怪时间段到计时
void CMapInstance::StartUpNewGen(UINT timeVal)
{
	m_tLifeTimeNewGenerage.Startup(timeVal);
}

bool CMapInstance::IsAttackFull(int nAmount)
{
	if(m_nInstanceScope != 2)
		return true;
	if(m_vecAttackMember.size() >= nAmount)
		return true;
	return false;
}

bool CMapInstance::IsDefenceFull(int nAmount)
{
	if(m_nInstanceScope != 2)
		return true;
	if(m_vecDefenceMember.size() >= nAmount)
		return true;
	return false;
}


void CMapInstance::ResetInstanceLife(UINT nLiftTime)
{
	INSTANCE_EXPIRED_LIFETIME = nLiftTime;
	//SetInstanceLife(nLiftTime);

	m_tLifeTimeInstance.Startup(GetLifeTime());///开始副本生命周期计时
	
// 	if(GetLifeTime()>INSTANCE_COUNTDOWN_TIME)
// 	{
// 		m_tCountDownInstance.Startup(GetLifeTime()-INSTANCE_COUNTDOWN_TIME);
// 	}
}


void CMapInstance::RequestEnterArena(const string& TeamName,UINT UserID)
{
	m_RequstUserMap[TeamName].insert(UserID);
}

void CMapInstance::EnterArena(const string& TeamName,UINT UserID)
{
	m_EnterArenaUserMap[TeamName].push_back(UserID);
}
bool CMapInstance::SynWarAward(bool bAttack)
{
	return true;
}

//void CMapInstance::SynWarTargetBekill(bool bBoss, bool bAttack)
//{
//	if(!bBoss)
//	{
//		//被攻击方杀死
//		if(bAttack)
//		{
//			m_nSWDBCount++;
//			if(m_nSWDBCount >= 3)
//			{
//				this->CloseCityWarInstance(bAttack);
//				SynManager()->SynWarVictory(m_nAttackSynID, m_nDefenceSynID, SYNWARTYPE_ATTACK);
//			}
//		}
//		else
//		{
//			m_nSWABCount++;
//			if(m_nSWABCount >= 3)
//			{
//				this->CloseCityWarInstance(bAttack);
//				SynManager()->SynWarVictory(m_nAttackSynID, m_nDefenceSynID, SYNWARTYPE_DEFENCE);
//			}
//		}
//	}
//}

void CMapInstance::ProcessSWTBeKill(bool bAttack, UINT type)
{
	switch(type)
	{
	case  CITYSYNMONSTER_BEKILL_ZONGTAN :
		{
		}
		break;
		
	case  CITYSYNMONSTER_BEKILL_BUY:
		{
			m_nSWBeKillMark |= SYNWAR_BEKILLMARK_BUY;
		}
		break;
		
	case CITYSYNMONSTER_BEKILL_CHGMAP:
		{
			m_nSWBeKillMark |= SYNWAR_BEKILLMARK_CHGMAP;
		}
		break;
		
	case CITYSYNMONSTER_BEKILL_TASK :
		{
			m_nSWBeKillMark |= SYNWAR_BEKILLMARK_TASK;
		}
		break;
	}
}

bool CMapInstance::BuyBeDestroy()
{
	return m_nSWBeKillMark&SYNWAR_BEKILLMARK_BUY;
}

bool CMapInstance::ChgMapBeDestroy()
{
	return m_nSWBeKillMark&SYNWAR_BEKILLMARK_CHGMAP;
}

bool CMapInstance::TaskBeDestroy()
{
	return m_nSWBeKillMark&SYNWAR_BEKILLMARK_TASK;
}

bool CMapInstance::HasMonster2(int nType,int nCheckAmount)
{
	DWORD dwHasMonster = 0;
	DEBUG_TRY
// 	nType += m_nUserLev;
	
	IRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
	while(pRole.Next())
	{
		CMonster* pMonster;
		if(pRole) 
		{
			if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
			{
				CGameMap *pMap = pMonster->GetMap();
				if(pMonster->IsDeleted())
					continue;
				
				if(!pMonster->IsAlive())
					continue;

				if(pMap && pMonster->GetType() == nType && pMap->GetID() == GetMapID())
				{
					dwHasMonster ++;
				}
			}
		}
	}

	DEBUG_CATCH("CMapInstance::HasMonster(int nType,int nCheckAmount)")
	return (dwHasMonster >= nCheckAmount);
}

bool CMapInstance::HasMonster(int nType,int nCheckAmount)
{
	nType += m_nUserLev;
	UINT nMonsterCount=m_MonsterCount[nType];
	return nMonsterCount>=nCheckAmount;
// 	DEBUG_TRY
// 	nType += m_nUserLev;
// 	///怪物自然死亡时只需简单删除所保存的怪物id
// 	CMonster *monsterptr=NULL;
// 	for(vector<OBJID>::iterator it = m_Monster.begin();it!=m_Monster.end();++it)
// 	{
// 		if(nCheckAmount==0)
// 			break;
// 		monsterptr=(CMonster*)RoleManager()->QueryRole(*it);
// 		if(monsterptr==NULL)
// 			continue;
// 		if(monsterptr->IsDeleted())
// 			continue; 				
// 		if(!monsterptr->IsAlive())
// 			continue;
// 		if(monsterptr->GetType()==nType)
// 			nCheckAmount--;
// 	}
// 	DEBUG_CATCH("CMapInstance::HasMonster")
// 	return nCheckAmount==0;

// 	DEBUG_TRY
// 	nType += m_nUserLev;
// 	///怪物自然死亡时只需简单删除所保存的怪物id
// 	UINT nMonster=0;
// 	CMonster *monsterptr=NULL;
// 	for(vector<OBJID>::iterator it = m_Monster.begin();it!=m_Monster.end();++it)
// 	{
// 		monsterptr=(CMonster*)RoleManager()->QueryRole(*it);
// 		if(monsterptr && monsterptr->GetType()==nType)
// 		{
// 			nCheckAmount--;
// 		}
// 	}
// 	DEBUG_CATCH("CMapInstance::HasMonster")
// 	return nCheckAmount==0;




// 	DEBUG_TRY
// 	nType += m_nUserLev;
// 	
// 	DWORD dwHasMonster = 0;
// 	IRoleSet::Iterator pRole = RoleManager()->QuerySet()->NewEnum();
// 	while(pRole.Next())
// 	{
// 		CMonster* pMonster;
// 		if(pRole) 
// 		{
// 			if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
// 			{
// 				CGameMap *pMap = pMonster->GetMap();
// 				if(pMonster->IsDeleted())
// 					continue;
// 				
// 				if(!pMonster->IsAlive())
// 					continue;
// 
// 				if(pMap && pMonster->GetType() == nType && pMap->GetID() == GetMapID())
// 				{
// 					dwHasMonster ++;
// 				}
// 			}
// 		}
// 	}
// 
// 	return (dwHasMonster >= nCheckAmount);
// 	DEBUG_CATCH("CMapInstance::HasMonster(int nType,int nCheckAmount)")
// 	return true;
}

bool CMapInstance::SetInstanceTimer(int nAction,UINT nPassSec)
{
	m_nActionForTimer = nAction;
// 	if(m_tForDelay.IsActive())
// 		return false;
// 	else
// 	{
		m_tForDelay.Startup(nPassSec);
		return true;
// 	}
}

void CMapInstance::SetMonsterKillEvent(OBJID nMonsterType,UINT nMonsterCount,UINT nContext)
{
	nMonsterType += m_nUserLev;
	m_nMonsterTypeEvent=nMonsterType;///怪物类型
	m_nMonsterCountEvent=nMonsterCount;///怪物数量
	m_nMonsterEventContext=nContext;///怪物事件上下文	
}


int CMapInstance::GetOperateData(int nIndex)
{
	if(nIndex < 0 || nIndex > 1)
		return 0;
	return instanceData[nIndex];
}

bool CMapInstance::OperateData(int nIndex,const char* szParam)
{
	if(nIndex < 0 || nIndex > 1)
		return false;
	if(!szParam)
		return false;

	char szOp[64];
	int nData = 0;
	if(sscanf(szParam,"%s %d",szOp,&nData) != 2)
		return false;

	if(strcmp(szOp,"=") == 0)
	{
		instanceData[nIndex] = nData;
		return true;
	}
	else if(strcmp(szOp,"+=") == 0)
	{
		instanceData[nIndex] += nData;
		return true;
	}
	else if(strcmp(szOp,"-=") == 0)
	{
		instanceData[nIndex] -= nData;
		return true;
	}
	else if(strcmp(szOp,"==") == 0)
	{
		return (instanceData[nIndex] == nData);
	}
	else if(strcmp(szOp,"<") == 0)
	{
		return (instanceData[nIndex] < nData);
	}
	else if(strcmp(szOp,">") == 0)
	{
		return (instanceData[nIndex] > nData);
	}

	return false;
}

void CMapInstance::addFreeUser(OBJID idUser)
{
	FreeUserIt it = m_SetFreeUser.find(idUser);
	if(it == m_SetFreeUser.end())
		m_SetFreeUser.insert(idUser);
}

bool CMapInstance::IsFreeUser(OBJID idUser)
{
	FreeUserIt it = m_SetFreeUser.find(idUser);
	if(it == m_SetFreeUser.end())
		return false;

	return true;
}

void CMapInstance::ClearFreeUser()
{
	CHECK(m_pData);

	for(FreeUserIt it = m_SetFreeUser.begin(); it != m_SetFreeUser.end(); it++)
	{
 		OBJID idUser = *it;
 		InstanceUseMgr()->UseInstance(idUser,m_pData->GetID(),0);
	}
	m_SetFreeUser.clear();
}
