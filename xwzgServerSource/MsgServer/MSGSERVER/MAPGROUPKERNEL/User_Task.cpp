// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "User.h"
#include "MapGroup.h"
#include "netmsg.h"
#include "ItemType.h"
#include "MapItem.h"
#include "Package.h"
#include "ActionDefine.h"
#include "Syndicate.h"
#include "npc.h"
#include "WantedList.h"
#include "TaskDetail.h"//20070115修罗
//#include "UnitTestSelf.h"
//modify code by 林德渊 begin 2011.5.30
#include "..\Network\MsgVasLv.h"
//modify code by 林德渊 end
const VAL_SKIPTEST	=999;
const int	PRESENTMONEY_LIMIT		= 10000;			// 基金划拨不少于10000


//////////////////////////////////////////////////////////////////////
int CUser::PushTaskID(OBJID idTask, OBJID idIter)
{
	if(idTask != ID_NONE && m_setTaskID.size() < MAX_MENUTASKSIZE)
	{
		m_setTaskID.push_back(idTask);
		m_setIterID.push_back(idIter);
		return m_setTaskID.size()-1;
	}
	return INDEX_NONE;
}

///添加专为脚本使用的action处理接口,softworms-2007-12-7
bool CUser::ProcessAction(OBJID idAction)
{
	bool hr=false;
	hr=GameAction()->ProcessAction(idAction,this,NULL,NULL,NULL,true);
	return hr;
}
//goto 为了在脚本中操作物品，重载此函数
bool CUser::ProcessActionIncludeItem(OBJID idAction, CItem* pItem)
{
	CHECKF(pItem);
	bool hr=false;
	hr=GameAction()->ProcessAction(idAction,this,NULL,pItem,NULL,true);
	return hr;
}
///添加专为脚本使用的action处理接口,softworms-2007-12-11,idActionType中部分内容可能有问题.
bool CUser::ProcessActionUser(OBJID idActionType, LPCTSTR szParam, int data)
{
	return GameAction()->ProcessScriptAction(idActionType,szParam,data,this, m_pMonster,/*GetItem(m_idItem)*/NULL,NULL);//[游途道标 2009.04.03]m_pMonster
}

//////////////////////////////////////////////////////////////////////
bool CUser::ProcessTask(int idx)
{
 	// check npc
// 	if(m_idTaskNpc == ID_NONE)
// 		return false;

	CNpc* pNpc = NULL;
	IRole* pRole = RoleManager()->QuerySet()->GetObj(GetClickNpcID());
	if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && GetMapID() == pNpc->GetMapID())
	{
		if(!pNpc->IsShow())
		{
			SendSysMsg("NPC已经消失");
			return false;
		}
	}
	else
	{
		SendSysMsg("找不到NPC");
		return false;
	}

// 	if(!GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, m_idTaskNpc, IPP_OF(pNpc)))
// 	{
// 		return false;
// 	}
// 	CHECKF(pNpc);

	idx--;
	if(idx < 0)
	{
		ClearTaskID();
		return true;
	}
	DWORD idAction = this->GetTaskID(idx);
	ClearTaskID();
	if(idAction)
	{
		GameAction()->ProcessAction(idAction, this, pNpc->QueryRole(), NULL);
		return true;
	}
	return false;
}
void CUser::AddTaskMask		(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;

	_int64 i64Mask = m_data.GetTaskMask()|(1i64<<idx);
	m_data.SetTaskMask(i64Mask);

	CMsgUserAttrib msg;
	msg.Create(GetID(),_USERATTRIB_MASKLOW,i64Mask & 0x00000000FFFFFFFF);
	msg.Append(_USERATTRIB_MASKHI,i64Mask>>32);
 	SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
void CUser::ClrTaskMask		(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;

	_int64 i64Mask = m_data.GetTaskMask();
	i64Mask	&= ~(1i64<<idx);
	m_data.SetTaskMask(i64Mask);

	CMsgUserAttrib msg;
	msg.Create(GetID(),_USERATTRIB_MASKLOW,i64Mask & 0x00000000FFFFFFFF);
	msg.Append(_USERATTRIB_MASKHI,i64Mask>>32);
 	SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
bool CUser::ChkTaskMask		(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return false;

	return (m_data.GetTaskMask() & (1i64<<idx)) != 0;
}
//----------------------------------------------------------
_int64 CUser::GetTaskMask ()//20061121修罗:获得dwTaskMask
{
	return m_data.GetTaskMask();
}
//////////////////////////////////////////////////////////////////////

void CUser::PickupNewCard(char* szCard)
{
	if(m_tCardMsgTime.IsActive())
	{
		if(!m_tCardMsgTime.ToNextTime())
		{
			SendSysMsg("领取太快，请过会再来领取");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	第一次检查
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);

	if(ChkNewGiveMask(35))
	{
		SendSysMsg("你已经领取过新手卡，不能重复领取");
		return;
	}

	if(strlen(szCard) != 18)
	{
		SendSysMsg("错误的卡号");
		return;
	}

	if(strncmp(szCard,"YJJH-",5) != 0)
	{
		SendSysMsg("错误的卡号");
		return;
	}

	CMsgNewCard msg;
	if(msg.Create(NEWCARDACT_PICK,GetID(),GetAccountID(),szCard))
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
}

void CUser::SetPickupNewCard(char* szCard, int TXTATR)
{
	CHECK(szCard);
	CMsgNewCard msg;
	if (TXTATR == 0)
	{
		if(msg.Create(NEWCARDACT_SET,GetID(),GetAccountID(),szCard))
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
	}
	else	if (TXTATR == 1)
	{
		if(msg.Create(VOTCARD_SET,GetID(),GetAccountID(),szCard))
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
	}
	else	if (TXTATR == 2)
	{
		if(msg.Create(VOTCARD2_SET,GetID(),GetAccountID(),szCard))
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
	}
	else	if (TXTATR == 3)
	{
		if(msg.Create(VOTCARD3_SET,GetID(),GetAccountID(),szCard))
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
	}
}

void CUser::PickupNewCard2(char* szCard)
{
	if(m_tCardMsgTime.IsActive())
	{
		if(!m_tCardMsgTime.ToNextTime())
		{
			SendSysMsg("领取太快，请过会再来领取");
			return;
		}
	}
	else
	{
		m_tCardMsgTime.SetInterval(5);	//	第一次检查
		m_tCardMsgTime.Update();
	}

	CHECK(szCard);
	
	if(ChkNewGiveMask(35))
	{
		SendSysMsg("你已经领取过新手卡，不能重复领取");
		return;
	}
	
	if(strlen(szCard) != 32)
	{
		SendSysMsg("错误的卡号");
		return;
	}
	
	CMsgNewCard msg;
	if(msg.Create(NEWCARDACT2_PICK,GetID(),GetAccountID(),szCard))
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
}

void CUser::SetPickupNewCard2(char* szCard, int TXTATR)
{
	CHECK(szCard);
	CMsgNewCard msg;
	if(msg.Create(NEWCARDACT2_SET,GetID(),GetAccountID(),szCard))
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
}

bool CUser::AddNewGiveMask(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return false;
	
	_int64 i64NewGive = m_data.GetNewGiveMask()|(1i64<<idx);
	if (m_data.SetNewGiveMask(i64NewGive))
	{
		return true;
	}
	else
		return false;
}

void CUser::ClrNewGiveMask(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;
	
	_int64 i64Mask = m_data.GetNewGiveMask();
	i64Mask	&= ~(1i64<<idx);
	m_data.SetNewGiveMask(i64Mask);
}

bool CUser::ChkNewGiveMask(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return false;
	
	return (m_data.GetNewGiveMask() & (1i64<<idx)) != 0;
}

_int64 CUser::GetNewGiveMask()//20070916:获得dwDayTaskMask-softworms
{
	return m_data.GetNewGiveMask();
}

bool CUser::AddNewGiveMask2(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return false;
	
	_int64 i64NewGive = m_data.GetNewGiveMask2()|(1i64<<idx);
	if (m_data.SetNewGiveMask2(i64NewGive))
	{
		return true;
	}
	else
		return false;
}

void CUser::ClrNewGiveMask2(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;
	
	_int64 i64Mask = m_data.GetNewGiveMask2();
	i64Mask	&= ~(1i64<<idx);
	m_data.SetNewGiveMask2(i64Mask);
}

bool CUser::ChkNewGiveMask2(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return false;
	
	return (m_data.GetNewGiveMask2() & (1i64<<idx)) != 0;
}

_int64 CUser::GetNewGiveMask2()//20070916:获得dwDayTaskMask-softworms
{
	return m_data.GetNewGiveMask2();
}

void CUser::AddWeekTaskMask(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;
	
	_int64 i64WeekMask = m_data.GetWeekTaskMask()|(1i64<<idx);
	m_data.SetWeekTaskMask(i64WeekMask);
}

void CUser::ClrWeekTaskMask(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;
	
	_int64 i64Mask = m_data.GetWeekTaskMask();
	i64Mask	&= ~(1i64<<idx);
	m_data.SetWeekTaskMask(i64Mask);
}

bool CUser::ChkWeekTaskMask(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return false;
	
	return (m_data.GetWeekTaskMask() & (1i64<<idx)) != 0;
}

_int64 CUser::GetWeekTaskMask()//20070916:获得dwDayTaskMask-softworms
{
	return m_data.GetWeekTaskMask();
}

void CUser::AddDayTaskMask(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;

	_int64 i64DayMask = m_data.GetDayTaskMask()|(1i64<<idx);
	m_data.SetDayTaskMask(i64DayMask);

	CMsgUserAttrib msg;
	msg.Create(GetID(),_USERATTRIB_DAYMASKLOW,i64DayMask & 0x00000000FFFFFFFF);
	msg.Append(_USERATTRIB_DAYMASKHI,i64DayMask>>32);
 	SendMsg(&msg);
}

void CUser::AddDayTaskMask2(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;
	
	_int64 i64DayMask2 = m_data.GetDayTaskMask2()|(1i64<<idx);
	m_data.SetDayTaskMask2(i64DayMask2);
	
	CMsgUserAttrib msg;
	msg.Create(GetID(),_USERATTRIB_DAYMASKLOW2,i64DayMask2 & 0x00000000FFFFFFFF);
	msg.Append(_USERATTRIB_DAYMASKHI2,i64DayMask2>>32);
	SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
void CUser::ClrDayTaskMask(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;

	_int64 i64Mask = m_data.GetDayTaskMask();
	i64Mask	&= ~(1i64<<idx);
	m_data.SetDayTaskMask(i64Mask);

	CMsgUserAttrib msg;
	msg.Create(GetID(),_USERATTRIB_DAYMASKLOW,i64Mask & 0x00000000FFFFFFFF);
	msg.Append(_USERATTRIB_DAYMASKHI,i64Mask>>32);
 	SendMsg(&msg);
}
//////////////////////////////////////////////////////////////////////
void CUser::ClrDayTaskMask2(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return;
	
	_int64 i64Mask = m_data.GetDayTaskMask2();
	i64Mask	&= ~(1i64<<idx);
	m_data.SetDayTaskMask2(i64Mask);
	
	CMsgUserAttrib msg;
	msg.Create(GetID(),_USERATTRIB_DAYMASKLOW2,i64Mask & 0x00000000FFFFFFFF);
	msg.Append(_USERATTRIB_DAYMASKHI2,i64Mask>>32);
	SendMsg(&msg);
}
//////////////////////////////////////////////////////////////////////
bool CUser::ChkDayTaskMask(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return false;

	return (m_data.GetDayTaskMask() & (1i64<<idx)) != 0;
}

bool CUser::ChkDayTaskMask2(int idx)
{
	IF_NOT (idx >= 0 && idx < 64)
		return false;
	
	return (m_data.GetDayTaskMask2() & (1i64<<idx)) != 0;
}
//////////////////////////////////////////////////////////////////////
_int64 CUser::GetDayTaskMask()//20070916:获得dwDayTaskMask-softworms
{
	return m_data.GetDayTaskMask();
}


// void CUser::ReqDayTaskMask_acc()
// {
// 	CMsgVAS MsgVAS;
// 	CMsgVAS::MsgTraffic stTraffic;
// 	MsgVAS.Create(&stTraffic, 0, GetAccountID(), GetID(), _MSGVAS_DAYMASK_ACC_GET);
// 	MapGroup(GetProcessID())->QueryIntraMsg()->TransmitWorldMsg(&MsgVAS);
// }

// bool CUser::ChkDayTaskMask_acc(int index)
// {
// 	if (index >= 32 || index < 0)
// 		return false;
// 	
// 	return (m_dayMaskAcc & (1<<index)) != 0;	
// }
// 
// void CUser::AddDayTaskMask_acc(int index)
// {
// 	if (index >= 32 || index < 0)
// 		return;
// 	
// 	int Mask = m_dayMaskAcc|(1<<index);
// 
// 	CMsgVAS MsgVAS;
// 	CMsgVAS::MsgTraffic stTraffic;
// 	MsgVAS.Create(&stTraffic, Mask, GetAccountID(), GetID(), _MSGVAS_DAYMASK_ACC_SET);
// 	MapGroup(GetProcessID())->QueryIntraMsg()->TransmitWorldMsg(&MsgVAS);
// 
// }
// 
// void CUser::ClrDayTaskMask_acc(int index)
// {
// 	if (index >= 32 || index < 0)
// 		return;
// 	
// 	int Mask = m_dayMaskAcc & (~(1<<index));
// 	
// 	CMsgVAS MsgVAS;
// 	CMsgVAS::MsgTraffic stTraffic;
// 	MsgVAS.Create(&stTraffic, Mask, GetAccountID(), GetID(), _MSGVAS_DAYMASK_ACC_SET);
// 	MapGroup(GetProcessID())->QueryIntraMsg()->TransmitWorldMsg(&MsgVAS);
// 	
// }
// 
// void CUser::SetDayTaskMask_acc(int data)
// {
// 	m_dayMaskAcc = data;
// }
void CUser::ResetWeekTaskMask()
{
	for(int i=0;i<64;++i)
		ClrWeekTaskMask(i);
	//m_dayMaskAcc = 0;
}

void CUser::ResetDayTaskMask()///重置天任务掩码
{
	for(int i=0;i<64;++i)
		ClrDayTaskMask(i);
	//m_dayMaskAcc = 0;

	int nVit = m_data.GetVit();
	if(nVit < m_data.GetMaxVit())
	{
		m_data.SetVit(m_data.GetMaxVit());
		CMsgUserAttrib	msg;
		if(msg.Create(GetID(),_USERATTRIB_VIT,m_data.GetVit()))
			SendMsg(&msg);
	}
	int nEne = m_data.GetEne();//goto 体力
	if (nEne < m_data.GetMaxEne())
	{
		m_data.SetEne(m_data.GetMaxEne());
		CMsgUserAttrib	msg;
		if(msg.Create(GetID(),_USERATTRIB_ENE,m_data.GetEne()))
			SendMsg(&msg);
	}
	//清除玩家特价商品购买掩码
	if (GetASales())
		SetASales(0, false);
	//清楚上线祝愿掩码
	if (m_pOnlineGift)
	{
		m_pOnlineGift->SetTaskMask(0);
	}
//modify code by 林德渊 begin 2011.5.30-
	DEBUG_TRY
	if( m_pvaslvdata )
	{
		CVasLvData *pres = m_pvaslvdata->QueryRecord();
		if( pres )
		{
			pres->SetInt(VASLVDATA_VISCOU,0);
			pres->SetInt(VASLVDATA_VASCOUNT,0,true);
			CMsgVasLv2  MsgVASLv2;
			if( MsgVASLv2.Create(ACTION_LOGINUPDATE,GetID(),0,0,0,0,0,0,0,0,GetAccountID(),0) )
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVASLv2 );
		}
	}
	DEBUG_CATCH("CUser::ResetDayTaskMask m_pvaslvdata exit")
//modify code by 林德渊 end
}

void CUser::ResetDayTaskMask2()///重置天任务掩码
{
	for(int i=0;i<64;++i)
		ClrDayTaskMask2(i);
	//m_dayMaskAcc = 0;
	
// 	int nVit = m_data.GetVit();
// 	if(nVit < m_data.GetMaxVit())
// 	{
// 		m_data.SetVit(m_data.GetMaxVit());
// 		CMsgUserAttrib	msg;
// 		if(msg.Create(GetID(),_USERATTRIB_VIT,m_data.GetVit()))
// 			SendMsg(&msg);
// 	}
// 	int nEne = m_data.GetEne();//goto 体力
// 	if (nEne < m_data.GetMaxEne())
// 	{
// 		m_data.SetEne(m_data.GetMaxEne());
// 		CMsgUserAttrib	msg;
// 		if(msg.Create(GetID(),_USERATTRIB_ENE,m_data.GetEne()))
// 			SendMsg(&msg);
// 	}
}

bool CUser::CompleteMercenaryTask(OBJID idTask, OBJID idItem)
{
	// 提交完成的佣兵任务
	// 检查任务合法性
	return true;
}

const int BASECONDITON_INDEX = 30000;
const int CONDITON_OFF = 10;
void CUser::SendInsMemInfo(int nType)
{
	CTeam* pTeam = GetTeam();
	CHECK(pTeam);
	CHECK(nType < 100);

	bool bCondition[5];
	OBJID nBaseAction = BASECONDITON_INDEX + CONDITON_OFF * nType;

	CMsgInsMemInfo msg;
	bool bSet = false;
	for(int i = 0; i < GetTeam()->GetMemberAmount(); i++)
	{
		OBJID idMember = GetTeam()->GetMemberByIndex(i);
		CUser* pMember = UserManager()->GetUser(idMember);
		if(pMember)
		{
			for (int j = 0 ; j < 5 ; j ++)
			{
				bCondition[j] = pMember->ProcessAction(nBaseAction + j);
			}

			if(i == 0)
			{
				msg.Create(pMember->GetName(),bCondition[0],bCondition[1],bCondition[2],bCondition[3],bCondition[4]);
				bSet = true;
			}
			else
				msg.AddMem(pMember->GetName(),bCondition[0],bCondition[1],bCondition[2],bCondition[3],bCondition[4]);
		}
	}

	if(bSet)
	{
		SendMsg(&msg);
	}
}

//////////////////////////////////////////////////////////////////////
bool CUser::AcceptMercenaryTask(OBJID idTask)
{
	return true;
}


void CUser::InitItemScript(CItem* pItem)///初始化道具脚本环境,为后面运行脚本作准备
{
	CHECK(pItem);
	LuaScriptMachine()->RegisterObjectDirect("Item","CItem","GetID",pItem,&CItem::GetID);
	LuaScriptMachine()->RegisterObjectDirect("Item","CItem","GetLife",pItem,&CItem::GetLife);
}

void CUser::InitScript()///初始化脚本环境,为后面运行脚本作准备
{
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","Msg",this,&CUser::SendInformation);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","Action",this,&CUser::ProcessAction);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","ActFun",this,&CUser::ProcessActionUser);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","SysMsg",this,&CUser::SendSysInfo);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","SpendItem",this,&CUser::SpendOneItem);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","GetID",this,&CUser::GetID);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","GetMoney",this,&CUser::GetMoney);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","EnterInstance",this,&CUser::RequestEnterInstance);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","Save",this,&CUser::SaveInfo);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","AddExp",this,&CUser::AwardUserExp);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","mapid",this,&CUser::IsMapID);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","GetLev",this,&CUser::GetLev);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","AddMoneyByType",this,&CUser::AddMoneyByType);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","GetLife",this,&CUser::GetLife);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","GetMaxLife",this,&CUser::GetMaxLife);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","GetMana",this,&CUser::GetMana);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","GetMaxMana",this,&CUser::GetMaxMana);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","GetPetLife",this,&CUser::GetPetLife);
	LuaScriptMachine()->RegisterObjectDirect("Hero","CUser","GetPetMaxLife",this,&CUser::GetPetMaxLife);
}

///softworms
bool CUser::RunScript(const char* txtScript)
{
	bool hr=false;
//	CUnitTestSelf test;
//	hr=test.RunScript(this,txtScript);
	return hr;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CancelMercenaryTask(OBJID idTask)
{
	return true;
}
//-------------------------------------
//20070115修罗:放弃任务
BOOL CUser::TaskGiveUp(DWORD taskID,int step)
{	
	CTaskDetail * pTask = this->GetTaskDetail();
	CHECKF(pTask);

	CTaskDetailData* pTaskDetail = pTask->QueryData(pUser->GetID(), taskID);
	if (!pTaskDetail)
		return false;

	if(pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE) == step && pTaskDetail->GetInt(TASKDETAILDATA_ISREC) == 0)
		return true;

	pTaskDetail->SetInt(TASKDETAILDATA_TASKPHASE,step);
	pTaskDetail->SetInt(TASKDETAILDATA_TASKBEGINTIME,0);
	pTaskDetail->SetInt(TASKDETAILDATA_DATA1,0);
	pTaskDetail->SetInt(TASKDETAILDATA_DATA2,0);
	pTaskDetail->SetInt(TASKDETAILDATA_DATA3,0);
	pTaskDetail->SetInt(TASKDETAILDATA_DATA4,0);
	pTaskDetail->SetInt(TASKDETAILDATA_DATA5,0);
	pTaskDetail->SetInt(TASKDETAILDATA_DATA6,0);
	pTaskDetail->SetInt(TASKDETAILDATA_ISREC,0);
	
	TaskDetailInfoForMsg taskData;
	taskData.taskID = taskID;
	taskData.taskPhase = step;						
	taskData.data1 = 0;
	taskData.data2 = 0;
	taskData.data3 = 0;
	taskData.data4 = 0;
	taskData.data5 = 0;
	taskData.data6 = 0;
	taskData.isRec = 0;
	taskData.randStep = pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP);
	
	CMsgTaskDetail msg;
	if(msg.Create(_TASKHINT_UPDATE,&taskData,1))
		pUser->SendMsg(&msg);

	return true;
}

OBJID CUser::GetTaskIndex(OBJID idTask)
{
	OBJID idResult = 0;
	CTaskDetail* pTask = this->GetTaskDetail();
	if(!pTask)
		return idResult;
	CTaskDetailData* pTaskDetail = pTask->QueryData(this->GetID(),idTask);
	if (!pTaskDetail)
		return idResult;
	idResult = 1;
   return idResult;
}

bool CUser::isFiftyP()  
{
	if(GetFatigue())	
		{return NULL;} 
	else   
		{return (120 >= m_data.GetFatiTime());}
}
bool CUser::isZeroP()	 
{
	if(GetFatigue())	
		{return NULL;} 
	else   
		{return (m_data.GetFatiTime() == 0);}
}

void CUser::SetTaskSpendEneType(OBJID idTask, USHORT nType)
{
	if (m_pTaskDetail)
		m_pTaskDetail->SetTaskSpendEneType(GetID(), idTask, nType);
}

UINT CUser::GetTaskSpendEneType(OBJID idTask)
{
	if (m_pTaskDetail)
		return m_pTaskDetail->GetTaskSpendEneType(GetID(), idTask);

	return 0;
}

void CUser::SetSynCon(int nData, bool bSend /*= true*/)
{
	CHECK(QuerySynAttr());
	CSynPtr pSyn=SynManager()->QuerySyndicate(GetSynID());
	CHECK(pSyn);
	if (nData > GetSynCon())
	{
		SendSysMsg("你获得了%u帮贡",nData-GetSynCon());
		QuerySynAttr()->SetHistoryCon(GetHistorySynCon()+nData-GetSynCon());
	}
	else
	{
		SendSysMsg("你失去了%u帮贡",GetSynCon()-nData);
	}

	QuerySynAttr()->SetProffer(nData);
	if(!bSend)
		return;
	CMsgSynInfo msgupdate;//发送客户端更新
	IF_OK(msgupdate.CreateUpdate(103,GetSynCon(),GetHistorySynCon()))
		SendMsg(&msgupdate);
	CMsgSynMemberInfo msg;
	if(msg.Create(4,0,1))
	{
		int vip=0;
		if (IsVipActive())
			vip=GetVipLev();
		msg.Append(GetID(),GetName(),GetLev(),GetProfession(1),GetSex(),GetMySynRank(),CVipList::GetLineByName(MapGroup(PID)->GetServerName()),vip,GetLastLogin(),GetHistorySynCon(),GetHistorySynMoney(),GetHistorySynGrow(),0);
		pSyn->BroadcastSynMsg(&msg);
	}
}
int	CUser::GetMapDoc()
{
	CHECKF(GetMap());
	return GetMap()->GetDocID();
}

//玩家使用道具"唤神古玉"（使用"唤神古玉"后给人物一个标记）后与默契值为10的宠物附体后具有特殊光影效果
//保存玩家吃过古玉没有。。。
bool CUser::AddEudemonSoulView()
{
	if(m_data.GetHoldTitle() == 1)
		return false;
	
	m_data.SetHoldTitle(1);

	if(m_idSoulEudemon != ID_NONE)
	{
		CUserPetData* pData = m_pUserPet->QueryData(m_idSoulEudemon);
		if(pData)
		{
			if(pData->GetInt(EUDEMONDATA_fusion) == 10)
			{
				m_bSoulEudemonView = true;
				CMsgUserAttrib msgAtt;
				if(msgAtt.Create(GetID(), _USERATTRIB_EUDEMON_VIEW, m_bSoulEudemonView))
					BroadcastRoomMsg(&msgAtt, INCLUDE_SELF);
			}
		}
	}

	return true;
}