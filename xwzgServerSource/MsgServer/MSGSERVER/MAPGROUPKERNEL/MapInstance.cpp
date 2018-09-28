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
//	m_nInstanceType=0;///������չ
//	m_nInstanceStatus=0;///������չ
	m_idMap=0;///�������ڵ�ͼid
	m_idOwn=0;///�����߱��,�����Ƕ���ID����playerid
	m_idOwnProcess=0;///����ӵ���ߵ�ͼ��ID
	m_idProcess=0;
	m_pData=NULL;

	INSTANCE_EXPIRED_TIME=30;///��ʱʱ����
	INSTANCE_EXPIRED_TEAMTIME=30;///��ʱ�������ʱ����
	INSTANCE_EXPIRED_LIFETIME = 0;///��ʱ�������ʱ����
	INSTANCE_EXPIRED_RECYCLETIME=60;///��ʱ�������ʱ����
	
// 	INSTANCE_COUNTDOWN_TIME = 120;//����ʱ������Ĭ���趨Ϊ120��

	INSTANCE_USER_MAX_NUMBERS = 1500;//��������Ҹ�����������
	INSTANCE_USER_MIN_NUMBERS = 0;//��������Ҹ�����������

	INSTANCE_NEWGENERATE_TIME = 0; //����ˢ��ʱ���
//	m_bSetExpiredTime = FALSE;

	m_nLeaveSynTime = 30;//�˰�����ʱ�� default: 30��
	m_nCityWarVictoryTime = 30; //��սʤ������ʱ  default: 30��
	
	m_nDomainMapID = 0;				//����ռ�������
	m_nAttackSynID = 0;				//��������ID
	m_nDefenceSynID = 0;			//���ط�����ID
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

	m_nMonsterTypeEvent=0;///��������
	m_nMonsterCountEvent=0;///��������
	m_nMonsterEventContext=0;///�����¼�������

	m_bAllowIn = true;
	m_bNeedKickout = false;
	m_SetFreeUser.clear();
}

CMapInstance::~CMapInstance()
{
	BOOL hr=false;

	int nStep = 0;
	DEBUG_TRY
	//zengɾ��sk_dynamap���еĸ�����¼��Ϣ
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

BOOL CMapInstance::RemoveInstance()///�������,��InitMapInstance���
{
	BOOL hr=false;
	hr=RemoveInstanceMap();
	NpcManager()->CloseMonsterGenerat(GetMapID(),0);///0��ʾ�ر�����
	///�����˶����д����¼�����
	slotUserLeaveTeam.disconnect(this);
//	slotUserRejoinTeam.disconnect(this);
	slotLifeTimeout.disconnect(this);
	return hr;
}

BOOL CMapInstance::IsActive()///�жϸ����Ƿ����,Ŀǰ�򵥹���,�����Ͷ�ʱ��;
{
	return m_tLifeTimeInstance.IsActive();

	//return (!m_tLifeTimeInstance.IsTimeOut());
}

BOOL CMapInstance::IsTimeOut()///�жϸ����Ƿ�ʱ
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
	///��ǰ�����ű�����
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
	///��ǰ�����ű�����
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


//zeng������Ҫע��Ľӿڶ��ŵ�����ط�ע�ᣬͳһ���������������ط�����ע�ᣬ��Ϊ��ĺ�����Ҫ���ñ�����һ���Ե�ע�ắ��
void CMapInstance::RegisterLuaInterface()
{
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","NewGenerat",this,&CMapInstance::NewGenerat);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","ShutdownInstance",this,&CMapInstance::ShutdownInstance);
	LuaScriptMachine()->RegisterObjectDirect("Instance","CMapInstance","CloseMonsterGenerat",this,&CMapInstance::CloseMonsterGenerat);

	//zeng�������泬ʱ�Ľӿڸ��߻�
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
///������ʼ���¼�
void CMapInstance::OnInit()///�������������ڳ�ʱ�¼�
{
	///��NPC������������,Ҫ����ҽ���ˢ��
	// synchro
	RunScript(isInit);///ˢ��
}

BOOL CMapInstance::InitMapInstance(int idProcess,PROCESS_ID idOwnProcess,OBJID idOwn,UINT nInstanceType,UINT nInstanceScope,UINT nDoMain)///��ʼ������,����������ˢ�ֻ���һЩ׼������
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

	hr=CreateInstanceMap();///����������ͼ
	if(hr)
	{
		
		///���Ҷ��Ĵ����¼�����
 		slotUserLeaveTeam.connect(this,&CMapInstance::OnUserQuitTeam);
// 		slotUserRejoinTeam.connect(this,&CMapInstance::OnUserRejoinTeam);
		slotLifeTimeout.connect(this,&CMapInstance::OnLifeTimeout);
		slotInit.connect(this,&CMapInstance::OnInit);
		slotInit();///������ʼ���¼�

// 		m_tLifeTimeInstance.Startup(GetLifeTime());///��ʼ�����������ڼ�ʱ
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
// 		pUser->ProcessAction(9530); //������������action
// 	}
// 
// 	return true;
// }


void CMapInstance::EnterScript(OBJID idUser)
{
	CUser* pUser = UserManager()->GetUser(idUser);
	if(NULL == pUser)
		return;

	//�����￴������
	if(GetInstanceScope() == INSTANCETYPE_TEAM) //���鸱���������Ƿ�ʽ
	{		
		if(!pUser->GetTeam())
		{
			LOGWARNING("������鸱������ң�û�ж���,���ᱻ�߳�!!");
			return;
		}
		
		TeamMemberInfo stInfo;
		if(!pUser->GetTeam()->GetMemberInfo(stInfo,idUser))
		{
			LOGWARNING("������鸱������ң��ж���,��������û�Լ���Ϣ!!");
			return;
		}

		//�������˲��ò�����
		pUser->GetTeam()->SetLocation(idUser,0,0,0,true);
	}

	if(!IsFreeUser(pUser->GetID()) && !pUser->m_bInstanceReborn)///����ڸ�������ʱ�����ý��븱���Ľű�
	{
		//�Ϸ�
		if(InstanceUseMgr()->CheckUseInstance(pUser->GetID(),m_pData->GetID()) == 1)
		{
			//״̬2
			InstanceUseMgr()->UseInstance(pUser->GetID(),m_pData->GetID(),2);
			//�ܽű�
			LPCTSTR strScriptFile=m_pData->GetStr("join_script");
			if(strlen(strScriptFile))
				GameAction()->ProcessScriptAction(ACTION_SCRIPT_PRESENT,strScriptFile,0,pUser,NULL);
			//�Ժ�����������
			addFreeUser(pUser->GetID());
			return;
		}
		
		//LOGWARNING("timeû�Ӿ�Ҫ��������instancetype = %u",GetInstanceType());
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
	m_tExpiredInstance.Clear();///stop��ʱ��
//	m_bSetExpiredTime = FALSE;	//�������ó�ʱ��ʱ��

	TeamMemberInfo menberInfo;
	ZeroMemory(&menberInfo,sizeof(TeamMemberInfo));

	CHECKF(pUser->GetInfo());///�������Ϊ��
	pTeam=UserManager()->GetTeamById(pUser->GetInfo()->idTeam);
	
	if(pTeam)
		pTeam->GetMemberInfo(menberInfo,pUser->GetID());

	///�ڸ��������ߵ���������߲���ִ�н��븱���Ľű��������ظ��۵���
// 	if(!IsFreeUser(pUser->GetID()) && !pUser->m_bInstanceReborn)///����ڸ�������ʱ�����ý��븱���Ľű�
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
// 		pTeam->SetLocation(pUser->GetID(),0,0,0);///���
	DEBUG_CATCH2("CMapInstance::AddPlayer(OBJID idUser):%d",idUser)
// 	DWORD nActionJoin = m_pData->GetInt("join_script");
// 	pUser->ProcessAction(nActionJoin);

	///���ٿ��Ƿ��в�����������������Ҳ�����������
	if(GetInstanceScope()==INSTANCETYPE_USER && pUser->GetID()!=GetOwnID())
		LogSave("CMapInstance::AddPlayer error!����ҽ��˲��������ĸ���!userid:%d,instancetype:%d",pUser->GetID(),GetInstanceType());
	else if(GetInstanceScope()==INSTANCETYPE_TEAM && pTeam && pTeam->GetID()!=GetOwnID())
		LogSave("CMapInstance::AddPlayer error!����ҽ��˲�����������ĸ���!userid:%d,instancetype:%d",pUser->GetID(),GetInstanceType());

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
			break;///����break,�������
		}
	}
	return hr;
}

BOOL CMapInstance::IsRecycle()///�жϸ����Ƿ����,Ŀǰ�򵥹���,�����Ͷ�ʱ��;
{
	return m_tRecycleInstance.TimeOver();
}

BOOL CMapInstance::KickOutPlayer(OBJID idPlayer,OBJID idMap, int nPosX, int nPosY)///�߳�ָ�����
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
		else if (g_SpecilLine==1)//ר�߸���*ע��*
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


BOOL CMapInstance::KickOutAllPlayer()///�߳��������
{
	BOOL hr=false;
	for(InstanceUserList::iterator it=m_InstanceUserList.begin();it!=m_InstanceUserList.end();it++)
		hr=KickOutPlayer(it->second.GetID(),m_OutMapID,m_OutCellX,m_OutCellY);
	//m_InstanceUserList.clear();			//zeng���θ���
	m_vecAttackMember.clear();
	m_vecDefenceMember.clear();
	ClearFreeUser();

	//���鸱�����岹��
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


BOOL CMapInstance::ActiveKickAllOut()///�߳��������
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
		
	//m_InstanceUserList.clear();			//zeng���θ���
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

BOOL CMapInstance::RemoveInstanceMap()///ɾ��������������ӵ�еĵ�ͼ,���ͼ��������������
{
	BOOL hr=false;
	///�ڵ��õ�ͼ������֮ǰ�ж�һ�����Ƿ�������
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

void CMapInstance::ShutdownInstance(UINT nTimeOut)///�������������ڳ�ʱ�¼�
{
	m_tLifeTimeInstance.Startup(nTimeOut);
}

void CMapInstance::OnLifeTimeout()///�������������ڳ�ʱ�¼�
{
	CloseInstance();///�رո���,���������ڽ����򸱱�Ŀ����ʱ����
	//KickOutAllPlayer();///��NPC��������,Ҫ��ɾ��������
	
}

void CMapInstance::CloseMonsterGenerat(OBJID idGen)
{
	NpcManager()->CloseMonsterGenerat(GetMapID(),idGen);
}

BOOL CMapInstance::CloseSynWarInstance(bool bAttack)
{
	m_tLifeCityWarVictory.Startup(m_nCityWarVictoryTime);
	char msgbuf[100];
	sprintf(msgbuf,"ϵͳ����%d���رո���,����������˳�������׼��!",m_nCityWarVictoryTime);
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
	sprintf(msgbuf,"ϵͳ����%d���رո���,����������˳�������׼��!",m_nCityWarVictoryTime);
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

BOOL CMapInstance::CloseInstance()///�رո���,���������ڽ����򸱱�Ŀ����ʱ����
{
	BOOL hr=false;
	///�߳��������
	hr=KickOutAllPlayer();
	///�г�ʱ����,û�г�ʱ���������ϳ�ʱ
	//if (IsTimeOut()==false)
	//	m_tLifeTimeInstance.Startup(0);

	m_tLifeTimeInstance.Clear();

//	NotifyOwnDestory();///֪ͨ����ӵ����,�����ѹر�

	return hr;
}

BOOL CMapInstance::CreateInstanceMap()///Ϊ������������ӵ�еĵ�ͼ,���ͼ��������������
{
	///����ʵ��ʱ,ʵ�����;Ͷ�Ӧ��ͼ����,����Ժ�Ҫ��չ���Ǽ�һ��InstanceType��
	SetMapID(MapManager()->CreateDynaMap(GetMapType()));
	return GetMapID()!=ID_NONE;
}

void CMapInstance::OnUserQuitTeam(OBJID idUser)
{
	if(m_nInstanceScope==INSTANCETYPE_TEAM)
	{
		///���������,���������������Ӷ�ʱ��
// 		if (m_InstanceUserList[idUser].GetTeamLimitTimer().IsActive()==false)
// 		{
// 			m_InstanceUserList[idUser].GetTeamLimitTimer().Startup(GetLeaveTeamExpire());
// 			///debug
// 			//[��;���� 2009.03.09]�޸���ʾ��Ϣ
// 			CUser* pUser = UserManager()->GetUser(idUser);
// 			if(pUser)
// 		    	pUser->SendSysMsg("�����뿪����,%d��󽫱��߳�",GetLeaveTeamExpire());
// 		}

		///����,�߳������
//		if (m_InstanceUserList[idUser].GetTeamLimitTimer().TimeOver())
		KickOutPlayer(idUser,m_OutMapID,m_OutCellX,m_OutCellY);
	}

}


void CMapInstance::OnUserRejoinTeam(OBJID idUser)///������һض��¼�
{
}

///����Կ��Ÿ��ű�����
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
			LOGWARNING("���ָ������ַǱ��ӳ�Ա!");
			return true;
		}
		return false;
	}
	else
		return true;
// 	}
// 	else
// 		return FALSE;			//�����������Ͳ�������ӵ����
}

BOOL CMapInstance::ProcessPlayerEvent(CInstanceUser& userptr)//////����������¼�
{
	BOOL hr=false;
	CUserPtr UserPtr=UserManager()->GetUser(userptr.GetID());
	if (UserPtr==NULL)
	{
		///��ҿ��ܵ�����,�Ӹ�����ɾ�������
		RemovePlayer(userptr.GetID());
		return hr;
	}

	if(!UserPtr->CanBeSee())
		return hr;

	///��鸱���е������û�����,�����Ӿ�������������Ӷ�ʱ��
	if(INSTANCETYPE_TEAM == m_nInstanceScope)
	{		
		if(IsUserLevelTeam(UserPtr))
			slotUserLeaveTeam(userptr.GetID());
	}
// 	else if(INSTANCETYPE_CITYWAR == m_nInstanceScope)  //��ս��������
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
// 	else if(INSTANCETYPE_SYNWAR == m_nInstanceScope) //��ս����
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

BOOL CMapInstance::ProcessAllPlayerEvent()//////����������ұ��߳������
{
	BOOL hr=false;

	BOOL bDefence = false;
	BOOL bAttack = false;
	///���ΰ�,Ҫ������BOOST,�Ͳ���д����ô������.
	for(InstanceUserList::iterator it=m_InstanceUserList.begin();it!=m_InstanceUserList.end();++it)
	{
		ProcessPlayerEvent(it->second);
	}

	return hr;
}

BOOL CMapInstance::CanEnterInstance()
{
	//������Ԥ��һ����ʱ����и����Ļ���
	
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

///��������ض��¼�
void CMapInstance::ProcessMonsterEvent()
{
	const UINT nMonsterEventDelay=10000;//10��
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

BOOL CMapInstance::ProcessInstanceEvent()///�����������¼�
{
	BOOL hr=false;	

	//����������
//	ProcessArena();


	if(m_bNeedKickout)
	{
		if(!IsEmpty())
			ActiveKickAllOut();
	}

	///��������������ڵ��˾��߳��������,�������ն�ʱ��������Ϊtimeout
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
		m_tLifeTimeInstance.Startup(0);///������ʱ

		m_bNeedKickout = true;
		m_bAllowIn = false;
	}

	if(m_tLifeTimeInstance.TimeOver())
	{
		SendInstanceMsg("�����Ѿ��ر�");
		m_tLifeTimeInstance.Clear();
		slotLifeTimeout();

		m_bNeedKickout = true;
		m_bAllowIn = false;
	}
	///������û����,�Ϳ�����ʱ��ʱ��
	if(IsEmpty())
	{
		if(m_tLifeTimeInstance.IsActive())
		{
			if(m_tLifeTimeInstance.IsTimeOut(10) && m_tExpiredInstance.IsActive()==false)///��������10���ſ�ʼ�жϿո��������
			{
				m_tExpiredInstance.Startup(GetNoUserExpire());
				m_tLifeTimeInstance.Startup(5);
				m_bNeedKickout = true;
				m_bAllowIn = false;
			}
		}
	}


	if (m_tLifeTimeNewGenerage.TimeOver())//����ˢ�ּ�ʱ
	{
		NpcManager()->CloseMonsterGenerat(GetMapID(),0);///0��ʾ�ر�����
#ifdef _DEBUG 
		char msgbuf[100];
		sprintf(msgbuf,"%s","ˢ��ʱ�����˹ر�����newGen");
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
		///��������ض��¼�
		ProcessMonsterEvent();
	}

	return hr;
}

void CMapInstance::OnTimer(time_t tCurr)///��ʱ��������
{
	BOOL hr=false;

	DEBUG_TRY
	hr=ProcessAllPlayerEvent();///��ѭ������ҿ��Բ������¼����
	DEBUG_CATCH("ProcessAllPlayerEvent")
	///ʵʱ����

	DEBUG_TRY
	hr=ProcessInstanceEvent();///�����������¼�
	DEBUG_CATCH("ProcessInstanceEvent")
}

UINT  CMapInstance::SetRebronPos(UINT posX,UINT posY)
{
	m_posRebornX = posX;
	m_posRebornY = posY;
	return 0;
}

///����
UINT  CMapInstance::CreateChessMonsters(UINT nMonsterTypeA,int nMonsterACount,UINT nMonsterTypeB,int nMonsterBCount,int StartX,int StartY,int nMonsterWidth)
{
	DEBUG_TRY
	OBJID idMonsterType=0;
	UINT nTotalMonster=nMonsterACount+nMonsterBCount;///�õ���������
	int nCurrentWidth=0,nCurrentHeight=0;///�������ֲ������
	const int nOffset=3;///����ֲ�����ƫ��
	///���ݹ�������������,���ɹ���type�ֲ�����
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

		///����ֻ��ˢ����ͼ��
		if(nCurrentWidth>=nMonsterWidth)
		{
			nCurrentWidth=0;
			nCurrentHeight++;
		}
		// 		afxDump<<idMonsterType<<":"<<StartX+nCurrentWidth*nOffset<<":"<<StartY+nCurrentHeight*nOffset<<"\n";
		CreateMonsterInInstance(idMonsterType,StartX+nCurrentWidth*nOffset,StartY+nCurrentHeight*nOffset);
		nCurrentWidth++;
	}
	///ÿ��ˢ���̹��Զ���������
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
//modify code by �ֵ�Ԩ begin 2011.6.22
//	nMonsterType += m_nUserLev;
	if( !(nMonsterType > 6100 && nMonsterType < 6400) )
		nMonsterType += m_nUserLev;
//modify code by �ֵ�Ԩ end
	ST_CREATENEWNPC	info;

	memset(&info,0,sizeof(info));

	nStep = 1;
	info.idMap			= GetMapID();
	info.idData			= 0;//��id��֤��sk_generator�д���
	info.idOwner		= 0;
	info.usPosX			= cellx;
	info.usPosY			= celly;
	
	info.usType			= nMonsterType;
	info.ucOwnerType	= 0;
	info.nData			= 0;
	
	nStep = 2;
// 	NpcManager()->CreateSynPet(&info);
	OBJID idMonster=NpcManager()->CreateSynPetReturnID(&info,"");
	///��������ɹ��󽫹���idѹ��vector����
	if(idMonster!=ID_NONE)
	{
		nStep = 3;
		///������ֻ�ֵ������¼�
		CMonster *monsterptr=(CMonster*)RoleManager()->QueryRole(idMonster);
		if(monsterptr)
		{
			nStep = 4;
//			m_Monster.push_back(idMonster);
			///����������1
			m_MonsterCount[nMonsterType]++;
			monsterptr->slotOnMonsterBeKill.connect(this,&CMapInstance::OnMonsterBeKill);
		}
	}
	DEBUG_CATCH2("CMapInstance::CreateMonsterInInstance nStep = %d",nStep)
}

void CMapInstance::OnMonsterBeKillEvent(UINT nMonsterType)
{
	DEBUG_TRY
	///�ж����ֹ�ʣ�������Ƿ���������
	nMonsterType-=m_nUserLev;///�����ظ������
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
	///������Ȼ����ʱֻ���ɾ��������Ĺ���id
	CMonster *monsterptr=(CMonster*)RoleManager()->QueryRole(idMonster);
	if(monsterptr)
	{
		nMonsterType=monsterptr->GetType();
		if(m_MonsterCount[nMonsterType]>0)
		{
			m_MonsterCount[nMonsterType]--;
			///������������ָֹպ��Ǳ����ĵĹ��������������򴥷��ű�
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
	// 		pKiller = UserManager()->GetUser(m_vEnmity[ i ].userID);//�ڱ���ͼ��
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
// 		if(monsterptr->GetType() == nType)//����Ƿ���Ҫɾ��
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


//����ˢ��ʱ��ε���ʱ
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

	m_tLifeTimeInstance.Startup(GetLifeTime());///��ʼ�����������ڼ�ʱ
	
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
//		//��������ɱ��
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
// 	///������Ȼ����ʱֻ���ɾ��������Ĺ���id
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
// 	///������Ȼ����ʱֻ���ɾ��������Ĺ���id
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
	m_nMonsterTypeEvent=nMonsterType;///��������
	m_nMonsterCountEvent=nMonsterCount;///��������
	m_nMonsterEventContext=nContext;///�����¼�������	
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
