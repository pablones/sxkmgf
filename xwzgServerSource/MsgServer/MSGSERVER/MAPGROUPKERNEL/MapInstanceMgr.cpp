// MapInstanceMgr.cpp: implementation of the CMapInstanceMgr class.
//
//////////////////////////////////////////////////////////////////////
#include <afx.h>
#include "MapGroup.h"
#include "MapInstance.h"
#include "MapInstanceMgr.h"
#include "MsgMapInstance.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//char	szDummyDataObjectTable[] = "id";


CMapInstanceMgr::CMapInstanceMgr()
{
	m_pDB=NULL;///内部保存一个数据库的实例指针,用于访问数据库中的邮件表
	m_idProcess=0;///地图组线程ID
    m_nActiveTime = 0;
}

CMapInstanceMgr::~CMapInstanceMgr()
{
	Clear();
}

BOOL CMapInstanceMgr::Clear()
{
	BOOL hr=true;
	for (CMapInstanceList::iterator it=m_instancelist.begin();it!=m_instancelist.end();++it)
		SAFE_DELETE(it->second)
	m_instancelist.clear();
	PureInstanceMgr();
	return hr;
}

BOOL CMapInstanceMgr::Init(IDatabase* pDB,PROCESS_ID idProcess)
{
	BOOL hr=false;
	m_idProcess=idProcess;
	m_pDB=pDB;

	//InitPvpControlState();
	///将数据库中的所有战争活动读到内存中

	return hr;
}

///判断该用户是否已在该类型的
BOOL CMapInstanceMgr::IsExistUserInstance(OBJID idUser,UINT nInstanceType)
{
	BOOL hr=false;
	InstanceID id=make_pair(idUser,nInstanceType);
	return IsExistUserInstance(id);
}

///判断该用户是否已在该类型的
BOOL CMapInstanceMgr::IsExistUserInstance(const InstanceID& id)
{
	return GetInstance(id)!=NULL;
}

///创建一个副本实例
CMapInstance* CMapInstanceMgr::CreateInstance()
{
	return new CMapInstance;
}


//OBJID CMapInstanceMgr::CreateInstanceMap(const InstanceID& id)///为副本创建它所拥有的地图,向地图管理器发出请求
//{
//	///初期实现时,实例类型就对应地图类型,如果以后要扩展就是加一个InstanceType表
//	OBJID idOwnd=id.first;
//	UINT nInstanceType=id.second;
//	NewMapInfo	info;
//	ZeroMemory(&info,sizeof(info));
//	SafeCopy(info.szName, "test", _MAX_NAMESIZE);
//	info.nMapDoc		= nInstanceType; 
//	info.nType			= 0;
//	info.nOwnerType		= 0;
//	info.idOwner		= idOwnd;
//	info.nMapGroup		= MapGroup(PID)->GetMapGroup();
//	info.idLikeMap		= 0;//pUser->GetMapID();
//	info.nLinkX			= 0;//pUser->GetPosX();
//	info.nLinkY			= 0;//pUser->GetPosY();
//	info.nPortalX		= 0;
//	info.nPortalY		= 0;
//	info.idRebornMap	= 0;
//	info.nRebornPortal	= 0;
//	info.nResLev		= 0;
//	return  MapManager()->CreateDynaMap(&info);
//}

BOOL CMapInstanceMgr::AddInstance(CMapInstance* ptrInstance)
{
	CHECKF(ptrInstance);
	InstanceID id=make_pair(ptrInstance->GetOwnID(),ptrInstance->GetInstanceType());
	if(m_instancelist.count(id))
	{
		//id存在
		SAFE_DELETE(ptrInstance);

		return true;
	}
	m_instancelist[id]=ptrInstance;
//	ptrInstance->slotLifeTimeout.connect(this,&CMapInstanceMgr::OnLifeTimeout);
	return true;
}

BOOL CMapInstanceMgr::CreateInstance(const InstanceID& id,PROCESS_ID idOwnProcess,UINT nInstanceScope, UINT nDoMainID)///处理用户创建指定类型的副本
{
	BOOL hr=false;
	CMapInstance* ptrInstance=CreateInstance();
	CHECKF(ptrInstance);
//	OBJID idMap=CreateInstanceMap(id);
//	if (idMap!=ID_NONE)
//	{
		if(ptrInstance->InitMapInstance(m_idProcess,idOwnProcess,id.first,id.second,nInstanceScope,nDoMainID))
			hr=AddInstance(ptrInstance);///将实例添加到管理容器中
		else
			SAFE_DELETE(ptrInstance);///如果初始化失败则删除该副本
//	}
	return hr;
}

UINT CMapInstanceMgr::GetInstanceMapType(const InstanceID& id)
{
	CMapInstance* ptrInstance=GetInstance(id);
	CHECKF(ptrInstance);
	return ptrInstance->GetMapType();
}

///得到副本地图id
OBJID CMapInstanceMgr::GetInstanceMap(const InstanceID& id)
{
	CMapInstance* ptrInstance=GetInstance(id);
	CHECKF(ptrInstance);
	return ptrInstance->GetMapID();
}

BOOL CMapInstanceMgr::SendReplyEnterMsg(OBJID idUser,const InstanceID& id,UINT nInstanceScope,int Cellx,int Celly,UINT nIdProcess)
{
	BOOL hr=TRUE;
	CMapInstance* pInstance=GetInstance(id);
	CHECKF(pInstance);
	CMsgMapInstance msg;
	hr=msg.CreateReplyEnterMsg(idUser,id.first,pInstance->GetMapID(),pInstance->GetMapType(),nInstanceScope,Cellx,Celly);
	if( 0 != nIdProcess )
		CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(nIdProcess,&msg);
	else
    	CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(pInstance->GetOwnProcessID(),&msg);
	return hr;
}



//BOOL CMapInstanceMgr::SendReplyMsg(PROCESS_ID pid,OBJID idUser,const InstanceID& id) 
//{
//	BOOL hr=false;
//	OBJID idMap=GetInstanceMap(id);
//	if (idMap!=ID_NONE)
//	{
//		CMsgMapInstance msg;
//		hr=msg.CreateReplyEnterMsg(idUser,id.first,idMap,id.second);
//		CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(pid,&msg);
//	}
//	return hr;
//}

BOOL CMapInstanceMgr::RequestEnterInstance(PROCESS_ID idOwnProcess,OBJID idUser,OBJID idOwn,UINT nInstanceType,UINT nDomainMapID,UINT nInstanceScope,UINT Cellx,UINT Celly,const char* TeameName)///处理用户请求进入指定类型的副本
{
	//通过副本规模来确定如何处理副本

	//普通副本（组队副本，军团副本）
	if(nInstanceScope==INSTANCETYPE_USER || nInstanceScope==INSTANCETYPE_TEAM/* || nInstanceScope==INSTANCETYPE_SYN*/)
	{
		InstanceID id=make_pair(idOwn,nInstanceType);
		CMapInstance* pMapInstance ;
		CMapInstanceList::iterator iter;
		
		if (IsExistUserInstance(id)==false)
		{
			//不管什么补登方式，对应副本不存在都应该放弃掉
			if(nDomainMapID > 1000000)
			{
				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
				if(pUser)
					pUser->ClearUsingInstance();
				return false;
			}

			//zeng 同时在线的副本数量太多，会出现创建副本地图失败的问题
			if (m_instancelist.size()>=100)
			{
				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
				if(pUser)
					pUser->SendSysMsg(_TXTATR_NORMAL,"系统中副本数量达到上线，无法创建更多副本，请稍候重试");
				return FALSE;
			}

			CreateInstance(id,idOwnProcess,nInstanceScope,nDomainMapID);	///用户还没有该类型的副本,为他创建一个

			iter = m_instancelist.find(id);
			if(iter!=m_instancelist.end())
			{
				pMapInstance = iter->second;
				pMapInstance->SetInstanceScope(nInstanceScope);
// 				pMapInstance->SetAttackSynID(0);
// 				pMapInstance->SetDefenceSynID(0);
			}
		}

		//判断副本人数限制,判断时间限制	
		iter = m_instancelist.find(id);
		if(iter!=m_instancelist.end())
		{
			pMapInstance = iter->second;
			if(!pMapInstance->CanEnterInstance())
			{
				//向玩家发消息通知无法进入到副本
				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
				if(pUser)
					pUser->SendSysMsg("当前副本进入回收阶段，无法进入");
				return FALSE;
			}

			//未进入的补登
			if(nDomainMapID > 1000000 && nDomainMapID < 2000000)
			{
				if(!pMapInstance->IsFreeUser(idUser))//不是这个副本的，代表掉线的时候这个副本已经完成了
				{
					CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
					if(pUser)
						pUser->ClearUsingInstance();
					return FALSE;
				}
			}

// 			if(pMapInstance->IsInstanceUserFull())
// 			{
// 				//向玩家发消息通知无法进入到副本
// 				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 				if(pUser)
// 					pUser->SendSysMsg("当前副本人数已满，无法进入");
// 				return FALSE;
// 			}
// 			else
			return SendReplyEnterMsg(idUser,id,nInstanceScope,Cellx,Celly);
		}
	}
	return true;
// 	else if(nInstanceScope==INSTANCETYPE_CITYWAR)//城战副本
// 	{
// 		InstanceID id = make_pair(nDomainMapID,nInstanceType);	//把城战副本的所有者定为DomainMapID
// 
// 		//判断是否能进入城战副本
// 		
// 		CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 		if(NULL == pUser)
// 			return FALSE;
// 
// 		//创建或进入副本
// 
// 		CMapInstance* pMapInstance = NULL ;
// 		CMapInstanceList::iterator iter;
// 
// 		if (IsExistUserInstance(id)==false)
// 		{
// 			CreateInstance(id,idOwnProcess,nInstanceScope,nDomainMapID);	///用户还没有该类型的副本,为他创建一个
// 			iter = m_instancelist.find(id);
// 			if(iter!=m_instancelist.end())
// 			{
// 				pMapInstance = iter->second;
// 				
// 				//设置城战副本相关内容
// 				pMapInstance->SetInstanceScope(nInstanceScope);
// 				pMapInstance->SetDomainMapID(nDomainMapID);
// 				pMapInstance->SetAttackSynID(pUser->GetAttackSyndicateID(nDomainMapID));
// 				pMapInstance->SetDefenceSynID(pUser->GetDefenceSyndicateID(nDomainMapID));
// 
// 				//根据城战开始时间计算副本剩余时间
// 				int nLife = pMapInstance->GetInstanceLife();
// 				CCityWare *pWar = CityWarMgr()->GetCityWarByDoMain(nDomainMapID);
// 				if(pWar)
// 				{
// 					int beginTime = pWar->GetWarBeginTime();
// 
// 					// 	int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat
// 					CTime time(CTime::GetCurrentTime());
// 					int weekDay = time.GetDayOfWeek();
// 					int hour = time.GetHour();
// 					int minute = time.GetMinute();
// 
// 					int nPassTime = 0;
// 					if(hour == beginTime%10000/100)
// 					{
// 						nPassTime = (minute - beginTime%10000%100)*60;
// 					}
// 					else if(hour > beginTime%10000/100)
// 					{
// 						nPassTime = (hour - beginTime%10000/100)*60*60 + (minute - beginTime%10000%100)*60;
// 					}
// 
// 					int nNewTime = nLife - nPassTime;
// 			// 		pMapInstance->SetInstanceLife(nNewTime);
// 					pMapInstance->ResetInstanceLife(nNewTime);
// 				}
// 			}
// 		}
// 		
// 		iter = m_instancelist.find(id);
// 		if(iter!=m_instancelist.end())
// 		{
// 			pMapInstance = iter->second;
// 			
// 			if(pMapInstance->IsInstanceUserFull())
// 			{
// 			// 	pUser->SendSysMsg(_TXTATR_NORMAL,"当前副本人数已满，无法进入！");
// 				pUser->ProcessActionUser(126, "当前副本人数已满，无法进入！", 0);
// 				return FALSE;
// 			}
// 			else if(pUser->IsCityWarAttack(nDomainMapID) && pMapInstance->IsAttackFull(60))
// 			{
// 			// 	pUser->SendSysMsg(_TXTATR_NORMAL,"当前副本攻击方人数已满！");
// 				pUser->ProcessActionUser(126, "当前副本攻击方人数已满！", 0);
// 				return FALSE;
// 			}
// 			else if(pUser->IsCityWarDefence(nDomainMapID) && pMapInstance->IsDefenceFull(60))
// 			{
// 			// 	pUser->SendSysMsg(_TXTATR_NORMAL,"当前副本防守方人数已满！");
// 				pUser->ProcessActionUser(126, "当前副本防守方人数已满！", 0);
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return SendReplyEnterMsg(idUser,id,nInstanceScope,Cellx,Celly);
// 			}
// 		}
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_ACTIVE)//active
// 	{
// 		m_nActiveTime = nDomainMapID;
// 		CTime time(CTime::GetCurrentTime());
// 	    int weekDay = time.GetDayOfWeek();
//     	int hour = time.GetHour();
//     	int minute = time.GetMinute();
// 
// 		if((hour < m_nActiveTime/10000/100) || ( hour==m_nActiveTime/10000/100 && minute < m_nActiveTime/10000%100))
// 		{
// 			CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 			if(pUser)
// 				pUser->SendSysMsg(_TXTATR_NORMAL,"活动时间没到，无法进入！");
// 			return FALSE;
// 		}
// 		InstanceID id = make_pair(nDomainMapID,nInstanceType);
// 
// 		CMapInstance* pMapInstance ;
// 		CMapInstanceList::iterator iter;
// 		
// 		if (IsExistUserInstance(id)==false)
// 		{
// 			CreateInstance(id,idOwnProcess,nInstanceScope);	///用户还没有该类型的副本,为他创建一个
// 			iter = m_instancelist.find(id);
// 			if(iter!=m_instancelist.end())
// 			{
// 				pMapInstance = iter->second;
// 				
// 				pMapInstance->SetInstanceScope(nInstanceScope);
// 				pMapInstance->SetDomainMapID(nDomainMapID);
// 				pMapInstance->SetAttackSynID(0);
// 				pMapInstance->SetDefenceSynID(0);
// 			}
// 		}
// 
// // 		m_nActiveID = id;
// 		
// 		iter = m_instancelist.find(id);
// 		if(iter!=m_instancelist.end())
// 		{
// 			pMapInstance = iter->second;
// 			if(pMapInstance->IsInstanceUserFull())
// 			{
// 				//向玩家发消息通知无法进入到副本
// 				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 				if(pUser)
// 					pUser->SendSysMsg(_TXTATR_NORMAL,"当前副本人数已满，无法进入！");
// 				return FALSE;
// 			}
// 			else
// 				return SendReplyEnterMsg(idUser,id,nInstanceScope,Cellx,Celly);
// 		}
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_MARRY)
// 	{
// 		InstanceID id=make_pair(idOwn,nInstanceType);
// 		CMapInstance* pMapInstance ;
// 		CMapInstanceList::iterator iter;
// 		
// 		if (IsExistUserInstance(id)==false)
// 		{
// 			CreateInstance(id,idOwnProcess,nInstanceScope);	///用户还没有该类型的副本,为他创建一个
// 			iter = m_instancelist.find(id);
// 			if(iter!=m_instancelist.end())
// 			{
// 				pMapInstance = iter->second;
// 				
// 				pMapInstance->SetInstanceScope(nInstanceScope);
// 				pMapInstance->SetDomainMapID(nDomainMapID);
// 				pMapInstance->SetAttackSynID(0);
// 				pMapInstance->SetDefenceSynID(0);
// 			}
// 		}
// 		//判断副本人数限制	
// 		iter = m_instancelist.find(id);
// 		if(iter!=m_instancelist.end())
// 		{
// 			pMapInstance = iter->second;
// 			if(pMapInstance->IsInstanceUserFull())
// 			{
// 				//向玩家发消息通知无法进入到副本
// 				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 				if(pUser)
// 					pUser->SendSysMsg(_TXTATR_NORMAL,"当前副本人数已满，无法进入！");
// 				return FALSE;
// 			}
// 			else
// 				return SendReplyEnterMsg(idUser,id,nInstanceScope,Cellx,Celly);
// 		}
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_SYNWAR)//帮战副本
// 	{	
// 		CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 		if(NULL == pUser)
// 			return FALSE;
// 
// 		InstanceID id = make_pair(idOwn, nInstanceType);	
// 
// 		//创建或进入副本
// 
// 		CMapInstance* pMapInstance = NULL ;
// 		CMapInstanceList::iterator iter;
// 
// 		if (IsExistUserInstance(id)==false)
// 		{
// 			CreateInstance(id,idOwnProcess,nInstanceScope,nDomainMapID);	///用户还没有该类型的副本,为他创建一个
// 			iter = m_instancelist.find(id);
// 			if(iter!=m_instancelist.end())
// 			{
// 				pMapInstance = iter->second;
// 				
// 				//设置帮战副本相关内容
// 			// 	pMapInstance->SetOwnID(idOwn);
// 				pMapInstance->SetInstanceScope(nInstanceScope);
// 			// 	pMapInstance->SetDomainMapID(nDomainMapID);
// 				pMapInstance->SetAttackSynID(pUser->GetSynWarInstanceAttackID());
// 				pMapInstance->SetDefenceSynID(pUser->GetSynWarInstanceDefenceID());
// 
// 				//根据帮战结束时间计算副本剩余时间
// 
// 				UINT nBeginTime = 0;
// 				UINT nEndTime   = 0;
// 				g_objStrRes.GetSynWarTime(nBeginTime, nEndTime);
// 			
// 				CTime time(CTime::GetCurrentTime());	
// 				CTime tEnd(time.GetYear(),time.GetMonth(),time.GetDay(),nEndTime%1000000/10000,nEndTime%10000/100,nEndTime%100,0);
// 				CTimeSpan nLeav = tEnd - time;
// 
// 				pMapInstance->ResetInstanceLife(nLeav.GetTotalSeconds());
// 			}
// 		}
// 		
// 		iter = m_instancelist.find(id);
// 		if(iter!=m_instancelist.end())
// 		{
// 			pMapInstance = iter->second;
// 			
// 			if(pMapInstance->IsInstanceUserFull())
// 			{
//  				pUser->ProcessActionUser(126, "当前副本人数已满，无法进入！", 0);
// 				return FALSE;
// 			}
// 			else
// 			{
// 				return SendReplyEnterMsg(idUser,id,nInstanceScope,Cellx,Celly,idOwnProcess);
// 			}
// 		}
// 	}
// 	else if (nInstanceScope==INSTANCETYPE_ARENA)
// 	{
// 		InstanceID id=make_pair(idOwn,nInstanceType);
// 		CMapInstance* pMapInstance ;
// 		CMapInstanceList::iterator iter;
// 		
// 		if (IsExistUserInstance(id)==false)
// 		{
// 			
// 			CreateInstance(id,idOwnProcess,nInstanceScope,nDomainMapID);	///用户还没有该类型的副本,为他创建一个
// 			
// 		}
// 		
// 		//判断副本人数限制
// 		iter = m_instancelist.find(id);
// 		if(iter!=m_instancelist.end())
// 		{
// 			pMapInstance = iter->second;
// 			//竞技场是否已经结束了
// 			if (pMapInstance->IsEndArena())
// 			{
// 				return false;
// 			}
// 			pMapInstance->RequestEnterArena(TeameName,idUser);
// 			
// 			return SendReplyEnterMsg(idUser,id,nInstanceScope,Cellx,Celly,idOwnProcess);
// 		}
// 	}
}

void CMapInstanceMgr::RequestCloseInstance(OBJID idOwn, UINT nInstanceType)
{
	InstanceID id=make_pair(idOwn, nInstanceType);
	CMapInstance* pInstance=GetInstance(id);
	CHECK(pInstance);
	pInstance->CloseInstance();
}

void CMapInstanceMgr::RequestCloseInstance(CMsgMapInstance& msg)///用户请求进入指定类型的副本
{
	InstanceID id=make_pair(msg.GetInfo()->idOwn,msg.GetInfo()->nInstanceType);
	CMapInstance* pInstance=GetInstance(id);
	CHECK(pInstance);
	pInstance->CloseInstance();
//	RecycleInstance(id);
//	RemoveInstance(id);
}

BOOL CMapInstanceMgr::PureInstanceMgr()
{
	BOOL hr=false;
//	for(CMapInstanceList::iterator it=m_instancerecyclelist.begin();it!=m_instancerecyclelist.end();)
//	{
//		if(it->second->IsRecycle())
//		{
//			it=RemoveInstance(it);
//		}
//		else
//			++it;
//	}

	CMapInstanceList::iterator it = m_instancerecyclelist.begin();
	CMapInstance* pInstance = NULL;
	for (;it!=m_instancerecyclelist.end();)
	{
		pInstance = it->second;
		if (pInstance && pInstance->IsEmpty())
		{
			delete pInstance;
			it = m_instancerecyclelist.erase(it);
		}
		else
		{
			//副本里面还有玩家
			pInstance->ClearUser();
			++it;
		}
	}

	return hr;
}

BOOL CMapInstanceMgr::ProcessAllInstanceEvent(time_t tCurr)///处理所有副本事件
{
	BOOL hr=true;
	for(CMapInstanceList::iterator it=m_instancelist.begin();it!=m_instancelist.end();)
	{
		///如果副本激活则运行副本OnTimer,否则删除该副本
		if (it->second->IsActive())
		{
			it->second->OnTimer(tCurr);
			++it;
		}
		else ///副本生命周期结束回收之
			it=RecycleInstance(it);
	}
	return hr;
}

void CMapInstanceMgr::OnTimer(time_t tCurr)///定时器处理函数
{
	ProcessAllInstanceEvent(tCurr);///处理所有副本事件

// 	ProcessPvpActive();           //处理pvp活动
	
	////删除无效的副本
	DEBUG_TRY
	PureInstanceMgr();
	DEBUG_CATCH("PureInstanceMgr();")
}

CMapInstance* CMapInstanceMgr::FindInstance(OBJID idMap)///寻找用户所在副本
{
	if(m_instancelist.size()==0)
		return NULL;
	for(CMapInstanceList::iterator it=m_instancelist.begin();it!=m_instancelist.end();++it)
	{
		if (it->second->GetMapID()==idMap)
			return it->second;
	}
	return NULL;
}

void CMapInstanceMgr::EnterScript(OBJID idMap,OBJID idUser)
{
	CMapInstance* ptrMapInstance=FindInstance(idMap);
	if(ptrMapInstance)
		ptrMapInstance->EnterScript(idUser);
}

void CMapInstanceMgr::OnPlayerEnterMap(OBJID idMap,OBJID idUser)///用户进入地图
{
	AddPlayer(idMap,idUser);
}

bool CMapInstanceMgr::CreateMonster(ST_CREATENEWNPC* pInfo)
{
	CHECKF(pInfo);
	UINT nMapType=pInfo->idMap;
	CMapInstanceList::iterator it;
	for( it=m_instancelist.begin();it!=m_instancelist.end();++it)
	{
		if (it->second->GetMapType()==nMapType)
		{
			pInfo->idMap=it->second->GetMapID();
			NpcManager()->CreateSynPet(pInfo);
		}
	}
	return it!=m_instancelist.end();
}

BOOL CMapInstanceMgr::AddPlayer(OBJID idMap,OBJID idUser)///往副本地图里
{
	BOOL hr=false;
	CMapInstance* ptrMapInstance=FindInstance(idMap);
	if(ptrMapInstance)
		hr=ptrMapInstance->AddPlayer(idUser);
	return hr;
}

//CMapInstanceList::iterator CMapInstanceMgr::RemoveInstance(const InstanceID& id)
//{
//	CMapInstance* ptrMapInstance=GetInstance(id);
//	CHECKF(ptrMapInstance);
////	ptrMapInstance->PureInstance();///清除副本,和InitMapInstance相对
//	SAFE_DELETE(ptrMapInstance);///释放内存
//	return m_instancelist.erase(id);
////	return ptrMapInstance!=NULL;
//}
CMapInstanceList::iterator CMapInstanceMgr::RecycleInstance(const CMapInstanceList::iterator& it)
{
//	CMapInstance* ptrMapInstance=it->second;
//	CHECKF(ptrMapInstance);
	//m_instancerecyclelist[it->first]=it->second;

	//zeng不要在这个地方删除副本
	//delete (it->second);
	//it->second->KickOutAllPlayer();
	m_instancerecyclelist.insert(make_pair(it->first,it->second));

	return m_instancelist.erase(it);
}

// void CMapInstanceMgr::RemoveInstance(const InstanceID& id)///删除副本
// {
// 	CMapInstanceList::iterator it=m_instancelist.find(id);
// 	if (it!=m_instancelist.end())
// 		RemoveInstance(it);
// }

// CMapInstanceList::iterator	CMapInstanceMgr::RemoveInstance(const CMapInstanceList::iterator& it)
// {
// 	CMapInstance* ptrMapInstance=it->second;
// 	CHECKF(ptrMapInstance);
// 	SAFE_DELETE(ptrMapInstance);///释放内存
// 	return /*m_instancerecyclelist.erase(it)*/ NULL;
// }

//void CMapInstanceMgr::OnLifeTimeout(OBJID idOwn,UINT nInstanceType)///处理副本生命周期超时事件
//{
////	InstanceID id=make_pair(idOwn,nInstanceType);
////	RemoveInstance(id);
////	CHECK(ptrMapInstance);
////	ptrMapInstance->PureInstance();///释放副本
//		//		if(it->second->IsExpired())
////		{
////			it->second->PureInstance();///释放副本
////			SAFE_DELETE(it->second);
////			it=m_instancelist.erase(it);
////		}
////		else
////			++it;
//
//}

BOOL CMapInstanceMgr::AddPlayer(CMapInstance* ptrInstance,OBJID idUser)///往副本里添加一个用户ID
{
	BOOL hr=false;

	return hr;
}

CMapInstance* CMapInstanceMgr::GetInstance(const InstanceID& id)///寻找用户所在副本
{
	CMapInstanceList::iterator it=m_instancelist.find(id);
	if (it!=m_instancelist.end())
		return it->second;
	else
		return NULL;
}

//BOOL CMapInstanceMgr::RefreshInstanceOwn(const InstanceID& id,OBJID idNewOwn)///更新副本拥有者ID
//{
//	CMapInstance* ptrInstance=GetInstance(id);
//	if (ptrInstance)
//		ptrInstance->SetOwnID(idNewOwn);
//	return ptrInstance!=NULL;
//}

//void CMapInstanceMgr::OnPlayerTeamCreate(const ParamList& paramlist)///用户离开地图
//{
//	OBJID idUser=paramlist[0].lVal;///用户id
//	OBJID idMap=paramlist[1].lVal;///进入的地图id	
//	OBJID idTeam=paramlist[2].lVal;///进入的地图id	
//
//	InstanceID id=make_pair(idUser,idMap);
//	RefreshInstanceOwn(id,idTeam);
////	///在用户进入地图时,添加该用户到副本中
////	RemovePlayer(idMap,idUser);
//}

void CMapInstanceMgr::OnPlayerExitMap (OBJID idMap,OBJID idUser)///用户离开地图
{
	DEBUG_TRY
	RemovePlayer(idMap,idUser);
	///当副本为空时通知所有队内离线队员更新下线地图位置
	CMapInstance* ptrMapInstance=FindInstance(idMap);
	if(ptrMapInstance && ptrMapInstance->IsEmpty())
	{
		CUser *pUser=UserManager()->GetUser(idUser);
		if(pUser && pUser->GetTeam()) 
		{
			pUser->GetTeam()->ResetOfflineLocation(true);
		}
	}
	DEBUG_CATCH3("OnPlayerExitMap %u,%u",idMap,idUser)
}
//BOOL CMapInstanceMgr::RefreshInstanceOwn(const InstanceID& id,OBJID idNewOwn)///更新副本拥有者ID
//{
//	CMapInstance* ptrMapInstance=FindInstance(idMap);
//	if(ptrMapInstance)
//		hr=ptrMapInstance->RemovePlayer(idUser);	
//}

BOOL CMapInstanceMgr::RemovePlayer(OBJID idMap,OBJID idUser)///删除副本地图里一个用户
{
	BOOL hr=false;
	CMapInstance* ptrMapInstance=FindInstance(idMap);
	if(ptrMapInstance)
		hr=ptrMapInstance->RemovePlayer(idUser);
	return hr;
}

BOOL CMapInstanceMgr::CheckCurrentTime(UINT nTime)
{
	if(nTime < 0)
		return FALSE;
	
	// 	int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat
	CTime time(CTime::GetCurrentTime());
	int nWeekDay  = time.GetDayOfWeek();
	int nHour     = time.GetHour();
	int nMinute   = time.GetMinute();
	int nSecond   = time.GetSecond();

	//例如6151236      周六 15时12分36秒
	if(nTime >= 8000000)
	{
		return FALSE;
	}
	else if(nTime >= 1000000)
	{
		if(nWeekDay-1 == nTime/1000000%7
			&& nHour == nTime/10000%100
			&& nMinute == nTime/100%100
			&& nSecond == nTime%100)
		{
			return TRUE;
		}
	}
	else if(nTime >= 10000)
	{
		if(nHour == nTime/10000
			&& nMinute == nTime/100%100
			&& nSecond == nTime%100)
		{
			return TRUE;
		}
	}
	else if(nTime >= 100)
	{
		if(nMinute == nTime/100
			&& nSecond == nTime%100)
		{
			return TRUE;
		}
	}
	else
	{
		if(nSecond == nTime)
		{
			return TRUE;
		}
	}

	return FALSE;
}

