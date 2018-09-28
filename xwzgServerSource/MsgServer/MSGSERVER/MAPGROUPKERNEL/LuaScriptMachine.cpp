// LuaScriptMachine.cpp: implementation of the CLuaScriptMachine class.
//
//////////////////////////////////////////////////////////////////////
#include "afx.h"
#include "MapGroup.h"
#include "LuaScriptMachine.h"
#include "TaskDetail.h"
#include "MapManager.h"
#include "MapInstanceMgr.h"
#include "InstanceNPC.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#pragma comment(lib, "LuaPlus.lib")

//template <typename Callee, typename Func>
//void RegisterObjectDirect(LuaStateOwner& state,const char* objname,const char* classname,const char* funname, Callee* callee, Func func)
//{
////		unsigned char* buffer = (unsigned char*)lua_newuserdata(GetCState(), sizeof(func));
////		memcpy(buffer, &func, sizeof(func));
////		Register(funcName, LPCD::DirectCallObjectMemberDispatcherHelper<Callee, Func, 2>::DirectCallMemberDispatcher, nupvalues + 1);
//	///生成元数据表
//	LuaObject mt=state->GetGlobals().GetByName(classname);
//	if(mt.IsNil())
//	{
//		mt= state->GetGlobals().CreateTable(classname);
//		mt.SetMetaTable(mt);
//		mt.SetObject("__index", mt);
//	}
//	mt.RegisterObjectDirect(funname, (Callee*)0, func);
//	///处理脚本中的对象
//	LuaObject luaObject=state->GetGlobals().GetByName(objname);
//	if(luaObject.IsNil())
//	{
//		luaObject.AssignNewTable(state);
//		luaObject.SetMetaTable(state->GetGlobal(classname));
//	}
//	luaObject.SetLightUserData("__object",callee);
//	state->GetGlobals().SetObject(objname, luaObject);
//}

//template <typename Callee, typename Func>
//void CLuaScriptMachine::RegisterObjectDirect(const char* objname,const char* classname,const char* funname, Callee* callee, Func func)
//{
//	LuaObject mt=m_LuaState->GetGlobals().GetByName(classname);
//	if(mt.IsNil())
//	{
//		mt= m_LuaState->GetGlobals().CreateTable(classname);
//		mt.SetMetaTable(mt);
//		mt.SetObject("__index", mt);
//	}
//	mt.RegisterObjectDirect(funname, (Callee*)0, func);
//	///处理脚本中的对象
//	LuaObject luaObject=m_LuaState->GetGlobals().GetByName(objname);
//	if(luaObject.IsNil())
//	{
//		luaObject.AssignNewTable(m_LuaState);
//		luaObject.SetMetaTable(m_LuaState->GetGlobal(classname));
//	}
//	luaObject.SetLightUserData("__object",callee);
//	m_LuaState->GetGlobals().SetObject(objname, luaObject);
//
////		::RegisterObjectDirect(m_LuaState,objname,classname,funname,callee,func);
//}

CLuaScriptMachine::CLuaScriptMachine()
{
	m_idProcess=0;
}

CLuaScriptMachine::~CLuaScriptMachine()
{

}

BOOL CLuaScriptMachine::HookMapMgr()
{
	BOOL hr=false;
	CHECKF(MapManager());
//	RegisterObjectDirect(m_LuaState,"MapMgr","CMapManager","urMsg",MapManager(),CUser::SendInformation);
	return hr;
}

BOOL CLuaScriptMachine::HookNpcMgr()
{
	BOOL hr=false;
	CHECKF(NpcManager());
	RegisterObjectDirect("NpcManager","INpcManager","NewSynNpc",NpcManager(),&INpcManager::CreateSynNpc);
	RegisterObjectDirect("NpcManager","INpcManager","DelSynNpc",NpcManager(),&INpcManager::RemoveSynNpc);
	RegisterObjectDirect("NpcManager","INpcManager","NewInstanceNpc",NpcManager(),&INpcManager::CreateInstanceNpc);
	RegisterObjectDirect("NpcManager","INpcManager","CreateSynNpc",NpcManager(),&INpcManager::CreateNewSynNpc);
	RegisterObjectDirect("NpcManager","INpcManager","CreateSynMonster",NpcManager(),&INpcManager::CreateSynMonster);
	return hr;
}


void CLuaScriptMachine::HookVarMgr()
{
//	CHECK(NpcManager());
	RegisterObjectDirect("VarMgr","CVariantManager","ResetVarSet",&g_VariantManager,&CVariantManager::ResetVariantSet);
}

CLuaScriptMachine::CLuaScriptMachine(PROCESS_ID idProcess)
{
	m_idProcess=idProcess;
	////一定要在所有管理器初始化之才能构造脚本机
	if (m_idProcess==MSGPORT_MAPGROUP_INSTANCE)
	{
		///挂接副本管理器接口
		CHECK(MapInstanceMgr());
//		RegisterObjectDirect(m_LuaState,"InstanceMgr","CMapInstanceMgr","urMsg",pScrUser,CUser::SendInformation);
	}
	///钩上地图管理器
	HookMapMgr();
	///钩上Npc管理器
	HookNpcMgr();
	///钩上变量管理器
	HookVarMgr();
}

//bool CLuaScriptMachine::DeInit()///解除注册
//{
//	bool hr=false;
//	m_LuaState->GetGlobals().Unregister("mnDrpItm");
//	m_LuaState->GetGlobals().Unregister("urMsg");
//	m_LuaState->GetGlobals().Unregister("urSysAct");
//	m_LuaState->GetGlobals().Unregister("urAction");
//	return hr;
//}

bool CLuaScriptMachine::InitMapOntimerScript(int nContext)
{
	bool hr=false;
	m_LuaState->GetGlobals().SetInteger("gContext",nContext);
	m_LuaState->GetGlobals().SetBoolean("result",false);
	return hr;
}

bool CLuaScriptMachine::InitScript(CUser *pScrUser,int nContext,IRole* pRole,CItem* pItem)///挂接对象中的函数
{
	bool hr=false;
	CMonster* pMonster=NULL;
	CUser*	  pDstUser=NULL;
	CNpc*	  pNpc=NULL;
	if(pRole)
	{
		pRole->QueryObj(OBJ_MONSTER,IPP_OF(pMonster));
		pRole->QueryObj(OBJ_USER,IPP_OF(pDstUser));
		pRole->QueryObj(OBJ_NPC,IPP_OF(pNpc));

		RegisterObjectDirect("Target","IRole","GetID",pRole,&IRole::GetID);
	}		

//	if (pScrUser) 
//	{
//		RegisterObjectDirect(m_LuaState,"Hero","CUser","Msg",pScrUser,CUser::SendInformation);
//		RegisterObjectDirect(m_LuaState,"Hero","CUser","ActFun",pScrUser,CUser::ProcessAction);
//		RegisterObjectDirect(m_LuaState,"Hero","CUser","Action",pScrUser,CUser::ProcessActionUser);
//		RegisterObjectDirect(m_LuaState,"Hero","CUser","SysMsg",pScrUser,CUser::SendSysInfo);
//		RegisterObjectDirect(m_LuaState,"Hero","CUser","SpendItem",pScrUser,CUser::SpendOneItem);
////		RegisterObjectDirect(m_LuaState,"Hero","CUser","urSysMsg",pScrUser,CUser::ProcessActionUser);
////		m_LuaState->GetGlobals().RegisterObjectDirect("urSysAct",pScrUser,CUser::ProcessAction);
////		m_LuaState->GetGlobals().RegisterDirect("urChk",*pScrUser,CUser::ChkTaskMask);
////		m_LuaState->GetGlobals().RegisterDirect("urID",*pScrUser,CUser::GetID);
////		m_LuaState->GetGlobals().RegisterObjectDirect("urAction",pScrUser,CUser::ProcessActionUser);
////		if (pScrUser->GetTaskDetail()) 
////		{
////			m_LuaState->GetGlobals().RegisterDirect("TaskIsExist",*(pScrUser->GetTaskDetail()),(pQueryData)CTaskDetail::QueryData);
////		}
//		
//	}
	return InitScript(pScrUser,nContext,pDstUser,pNpc,pMonster,pItem);
}

///初始化脚本,挂接对象中的函数
bool CLuaScriptMachine::InitScript(CUser *pScrUser,int nContext,CUser *pDstUser,CNpc *pNpc,CMonster* pMonster,CItem* pItem)
{
	bool hr=false;
	if (pMonster)
	{
//		unsigned char* buffer = (unsigned char*)lua_newuserdata(m_LuaState->GetGlobals().GetCState(), sizeof(*pMonster) + sizeof(pDropItem(CMonster::DropItem)));
//		memcpy(buffer, pMonster, sizeof(*pMonster));
//		memcpy(buffer + sizeof(*pMonster), &pDropItem(CMonster::DropItem), sizeof(pDropItem(CMonster::DropItem)));
//		m_LuaState->GetGlobals().RegisterDirect("some",pScrUser->ProcessActionUser);
//
//		m_LuaState->GetGlobals().RegisterObjectDirect("mnDrpItm",pMonster,(pDropItem)CMonster::DropItem);
//		m_LuaState->GetGlobals().RegisterDirect("mnGetPosX",*pMonster,CMonster::GetPosX);
//		m_LuaState->GetGlobals().RegisterDirect("mnGetPosY",*pMonster,CMonster::GetPosY);
//		m_LuaState->GetGlobals().RegisterDirect("PlayerSendInformation",*pUser,CUser::SendInformation);
//		m_LuaState->GetGlobals().RegisterDirect("PlayerGetLife",*pUser,CUser::GetLife);
//		m_LuaState->GetGlobals().RegisterDirect("PlayerProcessAction",*pUser,CUser::ProcessAction);
//		m_LuaState->GetGlobals().RegisterDirect("PlayerGainMoney",*pUser,CUser::GainMoney);
	}
	if (pScrUser) 
	{	

		RegisterObjectDirect("Hero","CUser","Msg",pScrUser,&CUser::SendInformation);
		RegisterObjectDirect("Hero","CUser","Action",pScrUser,&CUser::ProcessAction);
		RegisterObjectDirect("Hero","CUser","ActFun",pScrUser,&CUser::ProcessActionUser);
		RegisterObjectDirect("Hero","CUser","SysMsg",pScrUser,&CUser::SendSysInfo);
		RegisterObjectDirect("Hero","CUser","SpendItem",pScrUser,&CUser::SpendOneItem);
		RegisterObjectDirect("Hero","CUser","GetID",pScrUser,&CUser::GetID);
		RegisterObjectDirect("Hero","CUser","GetMoney",pScrUser,&CUser::GetMoney);
		RegisterObjectDirect("Hero","CUser","EnterInstance",pScrUser,&CUser::RequestEnterInstance);
		RegisterObjectDirect("Hero","CUser","AddExp",pScrUser,&CUser::AwardUserExp);
		RegisterObjectDirect("Hero","CUser","SendTeamMsg",pScrUser,&CUser::SendTeamMsg);
		RegisterObjectDirect("Hero","CUser","IsInTeam",pScrUser,&CUser::IsInTeam);
		RegisterObjectDirect("Hero","CUser","mapid",pScrUser,&CUser::IsMapID);
		RegisterObjectDirect("Hero","CUser","HasMount",pScrUser,&CUser::HasMount);
		RegisterObjectDirect("Hero","CUser","CheckJoinSynDays",pScrUser,&CUser::CheckJoinSynDays);
		RegisterObjectDirect("Hero","CUser","GetStudentAmount",pScrUser,&CUser::GetStudentAmount);
		RegisterObjectDirect("Hero","CUser","GetLev",pScrUser,&CUser::GetLev);
		RegisterObjectDirect("Hero","CUser","AddMoneyByType",pScrUser,&CUser::AddMoneyByType);
		RegisterObjectDirect("Hero","CUser","GetLife",pScrUser,&CUser::GetLife);
		RegisterObjectDirect("Hero","CUser","GetMaxLife",pScrUser,&CUser::GetMaxLife);
		RegisterObjectDirect("Hero","CUser","GetMana",pScrUser,&CUser::GetMana);
		RegisterObjectDirect("Hero","CUser","GetMaxMana",pScrUser,&CUser::GetMaxMana);
		RegisterObjectDirect("Hero","CUser","GetPetLife",pScrUser,&CUser::GetPetLife);
		RegisterObjectDirect("Hero","CUser","GetPetMaxLife",pScrUser,&CUser::GetPetMaxLife);
		RegisterObjectDirect("Hero","CUser","InitOnlineGift",pScrUser,&CUser::InitOnlineGift);
		RegisterObjectDirect("Hero","CUser","IsGive",pScrUser,&CUser::IsGive);
		//专线
		RegisterObjectDirect("Hero","CUser","ChangeLineMap",pScrUser,&CUser::ChangeLineMap);
		//帮派
		RegisterObjectDirect("Hero","CUser","EnterSynMap",pScrUser,&CUser::EnterSynMap);

		RegisterObjectDirect("Hero","CUser","LeaveSynMap",pScrUser,&CUser::LeaveSynMap);
		RegisterObjectDirect("Hero","CUser","IsMySynMap",pScrUser,&CUser::IsMySynMap);
		RegisterObjectDirect("Hero","CUser","LeaveSynInsten",pScrUser,&CUser::LeaveSynInsten);
		RegisterObjectDirect("Hero","CUser","SetSynTaskGift",pScrUser,&CUser::SetSynTaskGift);
		RegisterObjectDirect("Hero","CUser","GetGiftMask",pScrUser,&CUser::GetGiftMask);
		RegisterObjectDirect("Hero","CUser","GetSynSci",pScrUser,&CUser::GetSynSci);///
		RegisterObjectDirect("Hero","CUser","GetNowWeekDay",pScrUser,&CUser::GetNowWeekDay);
		RegisterObjectDirect("Hero","CUser","GetSynRank",pScrUser,&CUser::GetMySynRank);
		RegisterObjectDirect("Hero","CUser","BroadcastSyn",pScrUser,&CUser::BroadcastSyn);
		RegisterObjectDirect("Hero","CUser","GetSynID",pScrUser,&CUser::GetSynID);
		RegisterObjectDirect("Hero","CUser","CheckIsSameSyn",pScrUser,&CUser::CheckIsSameSyn);
		RegisterObjectDirect("Hero","CUser","SaveLog",pScrUser,&CUser::SaveLog);
		RegisterObjectDirect("Hero","CUser","CulData",pScrUser,&CUser::CulData);
		RegisterObjectDirect("Hero","CUser","GetEverydayOL",pScrUser,&CUser::GetEverydayOL);
		
		////////帮派end
		//跑商
		RegisterObjectDirect("Hero","CUser","SendVerification",pScrUser,&CUser::SendVerification);
		//跑商end
		RegisterObjectDirect("Hero","CUser","SetTaskSpendEneType",pScrUser,&CUser::SetTaskSpendEneType);//扣精力接任务的加成
		RegisterObjectDirect("Hero","CUser","GetTaskSpendEneType",pScrUser,&CUser::GetTaskSpendEneType);
		RegisterObjectDirect("Hero","CUser","GetPassedTimeFromCreate",pScrUser,&CUser::GetPassedTimeFromCreate);//获取玩家创建角色的时间到指定时间的分钟数
		RegisterObjectDirect("Hero","CUser","AddTutorPreach",pScrUser,&CUser::AddTutorPreach);//师徒传道任务，增加魂收集
		RegisterObjectDirect("Hero","CUser","GetTeamStuInLev",pScrUser,&CUser::GetTeamStuInLev);//获取队伍中指定等级范围的徒弟数量
		RegisterObjectDirect("Hero","CUser","HasSameIPStuInTeam",pScrUser,&CUser::HasSameIPStuInTeam);//队伍中是否有相同IP的徒弟
		RegisterObjectDirect("Hero","CUser","CheckItemCreater",pScrUser,&CUser::CheckItemCreater);//检查物品使用者是否是物品的制造者
		RegisterObjectDirect("Hero","CUser","AddMorAlity",pScrUser,&CUser::AddMorAlity);//增加师德
		RegisterObjectDirect("Hero","CUser","AddImpart",pScrUser,&CUser::AddImpart);//增加传授度
		RegisterObjectDirect("Hero","CUser","SpendMorAlity",pScrUser,&CUser::SpendMorAlity);//减少师德
		RegisterObjectDirect("Hero","CUser","HasTutorGift",pScrUser,&CUser::HasTutorGift);//判断师父奖励
		RegisterObjectDirect("Hero","CUser","SetTutorGiftMask",pScrUser,&CUser::SetTutorGiftMask);//设置师父奖励掩码
		RegisterObjectDirect("Hero","CUser","GetMorAlity",pScrUser,&CUser::GetMorality);//获取师德
		RegisterObjectDirect("Hero","CUser","GetPosX",pScrUser,&CUser::GetPosX);
		RegisterObjectDirect("Hero","CUser","GetPosY",pScrUser,&CUser::GetPosY);
		RegisterObjectDirect("Hero","CUser","GetTeamStuAvgLev",pScrUser,&CUser::GetTeamStuAvgLev);//队伍中徒弟的平均等级
		RegisterObjectDirect("Hero","CUser","GetStuLevAddedInTeam",pScrUser,&CUser::GetStuLevAddedInTeam);//队伍中的徒弟在师父门下升过的级数
		RegisterObjectDirect("Hero","CUser","GetInstanceTime",pScrUser,&CUser::GetInstanceTime);
		RegisterObjectDirect("Hero","CUser","GetInstanceFinishTime",pScrUser,&CUser::GetInstanceFinishTime);
		RegisterObjectDirect("Hero","CUser","FlushPetegg",pScrUser,&CUser::FlushPetegg);//刷新宠物蛋
		RegisterObjectDirect("Hero","CUser","SendPeteggData",pScrUser,&CUser::SendPeteggData);//宠物蛋数据
		RegisterObjectDirect("Hero","CUser","GetMapDoc",pScrUser,&CUser::GetMapDoc);
		RegisterObjectDirect("Hero","CUser","WarGameSignup",pScrUser,&CUser::WarGameSignup);//竞技场报名
		RegisterObjectDirect("Hero","CUser","EnterWarGame",pScrUser,&CUser::EnterWarGameFromNpc);//进入竞技场
		RegisterObjectDirect("Hero","CUser","LeaveWarGame",pScrUser,&CUser::LeaveWarGame);//进入竞技场
		RegisterObjectDirect("Hero","CUser","GetTaskStep",pScrUser,&CUser::GetTaskStep);
//modify code by 林德渊 begin 2011.5.30
		RegisterObjectDirect("Hero","CUser","GetVasLvQuestionAnswer",pScrUser,&CUser::GetVasLvQuestionAnswer);
		RegisterObjectDirect("Hero","CUser","GetVasLvQuestionAnswerId",pScrUser,&CUser::GetVasLvQuestionAnswerId);
		RegisterObjectDirect("Hero","CUser","GetVasLvid",pScrUser,&CUser::GetVasLvid);
//modify code by 林德渊 end
		RegisterObjectDirect("Hero","CUser","AddEudemonSoulView",pScrUser,&CUser::AddEudemonSoulView);
		//副本
		if(m_idProcess==MSGPORT_MAPGROUP_INSTANCE)
		{
			CMapInstance* pMapInstance = MapInstanceMgr()->FindInstance(pScrUser->GetMapID());
			if(pMapInstance)
			{
				pMapInstance->RegisterLuaInterface();
			}
		}
	}
	if (pNpc) 
	{
		///副本NPC
		if (pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE)
		{
			RegisterObjectDirect("InstanceNpc","CInstanceNPC","RefreshClient",(CInstanceNPC*)pNpc,&CInstanceNPC::RefreshInstanceInfo);
			RegisterObjectDirect("InstanceNpc","CInstanceNPC","HasPassword",(CInstanceNPC*)pNpc,&CInstanceNPC::HasPassword);
			RegisterObjectDirect("InstanceNpc","CInstanceNPC","IsRoomOwn",(CInstanceNPC*)pNpc,&CInstanceNPC::IsRoomOwn);

		}

	}
	if (pItem)
	{
		RegisterObjectDirect("Item","CItem","GetID",pItem,&CItem::GetID);
		RegisterObjectDirect("Item","CItem","GetLife",pItem,&CItem::GetLife);
		RegisterObjectDirect("Item","CItem","GetColour",pItem,&CItem::GetColour);
		RegisterObjectDirect("Item","CItem","GetReqLev",pItem,&CItem::GetReqLev);
		RegisterObjectDirect("Item","CItem","GetAddition",pItem,&CItem::GetItemAdditionData);
		RegisterObjectDirect("Item","CItem","GetKidnap",pItem,&CItem::GetKidnap);
	}
	///放入上下文信息
	m_LuaState->GetGlobals().SetInteger("gContext",nContext);
	m_LuaState->GetGlobals().SetBoolean("result",false);
	return hr;
}

//吗的，为了搞个随机奖品我还要在底层添加个函数
bool CLuaScriptMachine::InitVar(int nIndex)
{
	bool hr=false;
	m_LuaState->GetGlobals().SetInteger("gIndex",nIndex);
	return hr;
}

///执行脚本
bool CLuaScriptMachine::RunScript(const char* txtScript)
{
	bool hr=false;
//	hr=m_LuaState->DoString(txtScript)!=0;
	int ret=-1;
	ret=m_LuaState->LoadBuffer(txtScript,strlen(txtScript),txtScript);
	ret|=m_LuaState->PCall(0,0,0);
	if (ret)
	{
		m_strLastError=lua_tostring(m_LuaState->GetCState(), -1);
		m_LuaState->Pop(1);
	}
	return ret>0;
}

bool CLuaScriptMachine::LoadScriptFile(const char* szFileName)///运行脚本文件
{
	LuaObject result;
	CString strFileName=szFileName;
	///没有扩展名就加上
	if (strFileName.Find(".lua")==-1)
		strFileName+=".lua";
	///加路径
	if (strFileName.Find("script/")==-1)
		strFileName="script/"+strFileName;

	int ret=m_LuaState->DoFile(szFileName);
//	ret|=m_LuaState->PCall(0,0,0);
//	if (ret)
//	{
//		result=m_LuaState->Stack(-1);
//		if (result.IsNil()==false && result.IsString())
//			m_strLastError=result.GetString();
//		else
//			m_strLastError="unkown lua error!";
//	}
//	else
//	{
//		result = m_LuaState->GetGlobals()["result"];
//		if (result.IsNil()==false)
//			ret=result.GetBoolean();
//		m_strLastError="";///empty error info.
//	}
	return ret>0;
}

bool CLuaScriptMachine::RunScriptFun(const char* szFileName,const char* szFunName)///运行脚本文件中的函数
{
	CString strErrorInfo;
	LuaStackObject result;
	LuaObject funObj;
	int hr=0;
//	try
//	{
//		m_LuaState->
		if(!RunScriptFile(szFileName))
		{
			funObj = m_LuaState->GetGlobals()[szFunName];
//			if (funObj.IsFunction())
//			{
//				LuaFunction<bool> fun=funObj;
//				hr=fun();
//			}
			if(funObj.IsFunction())
			{
//				m_LuaState->GetGlobals()->
//				 lua_getglobal(m_LuaState->GetCState(), szFunName);
				 funObj.Push();
//				 int resultsStackPos = m_LuaState->GetTop();
				 hr = lua_pcall( m_LuaState->GetCState(),    //VMachine
							0,    //Argument Count
							1,    //Return Value Count
							0 );
				 result=m_LuaState->Stack(-1);
				 if(hr)
				 {
					if (result.IsString())
					{
						m_strLastError=result.GetString();//lua_tostring(m_LuaState->GetCState(), -1);
						hr=0;
					}
				 }
				 else if(lua_isboolean(m_LuaState->GetCState(), -1))
					 hr=lua_toboolean(m_LuaState->GetCState(), -1);
//				 m_LuaState->Pop(resultsStackPos);
			}
			else
			{
				strErrorInfo.Format("Could Not Find Function<%s> In Script<%s>!)",szFunName,szFileName);
				m_strLastError=strErrorInfo;
			}
//			funobj=m_LuaState->GetGlobal(szFunName);
//			if (funobj.IsNil()==false || funobj.IsFunction()==true)
//			{
//				LuaFunction<bool> CanUse =funobj;
////				bool hr=CanUse();
//				LuaCall fun=funobj;
//				LuaCall
//				result=fun<<LuaRun();
////				if(result.IsInteger())
////				{
////					hr=result.GetInteger();
////				}
////				else if(result.IsBoolean())
////				{
////					hr=result.GetBoolean();
////				}
			//}
		}
//	}
//	catch (LuaException& e)
//	{
////		m_strLastError=e.GetMessage();
//		m_strLastError=lua_tostring(m_LuaState->GetCState(), -1);
//	}
//	catch (...)
//	{
//		m_strLastError="unknown error!";
//	}
	if(IsHasError())///如果执行脚本有错误,则显示错误在客户端
	{
		strErrorInfo.Format("Hero:SysMsg(\"%s\")",LuaScriptMachine()->GetLastError());
		m_LuaState->DoString(strErrorInfo);
	}
//	LuaState::Destroy(m_LuaState);
	m_LuaState=LuaState::Create();
	return hr;
}

bool CLuaScriptMachine::RunScriptFile(const char* szFileName)///运行脚本文件
{
	LuaObject result;
	CString strFileName=szFileName;
	///没有扩展名就加上
	if (strFileName.Find(".lua")==-1)
		strFileName+=".lua";
	///加路径
	if (strFileName.Find("script/")==-1)
		strFileName="script/"+strFileName;

	int ret=m_LuaState->LoadFile(strFileName);
	ret|=m_LuaState->PCall(0,0,0);
	if (ret)
	{
		result=m_LuaState->Stack(-1);
		if (result.IsNil()==false && result.IsString())
			m_strLastError=result.GetString();
		else
			m_strLastError="unkown lua error!";
	}
	else
	{
		result = m_LuaState->GetGlobals()["result"];
		if (result.IsNil()==false)
			ret=result.GetBoolean();
		m_strLastError="";///empty error info.
	}
	return ret>0;
}
