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
	m_pDB=NULL;///�ڲ�����һ�����ݿ��ʵ��ָ��,���ڷ������ݿ��е��ʼ���
	m_idProcess=0;///��ͼ���߳�ID
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
	///�����ݿ��е�����ս��������ڴ���

	return hr;
}

///�жϸ��û��Ƿ����ڸ����͵�
BOOL CMapInstanceMgr::IsExistUserInstance(OBJID idUser,UINT nInstanceType)
{
	BOOL hr=false;
	InstanceID id=make_pair(idUser,nInstanceType);
	return IsExistUserInstance(id);
}

///�жϸ��û��Ƿ����ڸ����͵�
BOOL CMapInstanceMgr::IsExistUserInstance(const InstanceID& id)
{
	return GetInstance(id)!=NULL;
}

///����һ������ʵ��
CMapInstance* CMapInstanceMgr::CreateInstance()
{
	return new CMapInstance;
}


//OBJID CMapInstanceMgr::CreateInstanceMap(const InstanceID& id)///Ϊ������������ӵ�еĵ�ͼ,���ͼ��������������
//{
//	///����ʵ��ʱ,ʵ�����;Ͷ�Ӧ��ͼ����,����Ժ�Ҫ��չ���Ǽ�һ��InstanceType��
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
		//id����
		SAFE_DELETE(ptrInstance);

		return true;
	}
	m_instancelist[id]=ptrInstance;
//	ptrInstance->slotLifeTimeout.connect(this,&CMapInstanceMgr::OnLifeTimeout);
	return true;
}

BOOL CMapInstanceMgr::CreateInstance(const InstanceID& id,PROCESS_ID idOwnProcess,UINT nInstanceScope, UINT nDoMainID)///�����û�����ָ�����͵ĸ���
{
	BOOL hr=false;
	CMapInstance* ptrInstance=CreateInstance();
	CHECKF(ptrInstance);
//	OBJID idMap=CreateInstanceMap(id);
//	if (idMap!=ID_NONE)
//	{
		if(ptrInstance->InitMapInstance(m_idProcess,idOwnProcess,id.first,id.second,nInstanceScope,nDoMainID))
			hr=AddInstance(ptrInstance);///��ʵ����ӵ�����������
		else
			SAFE_DELETE(ptrInstance);///�����ʼ��ʧ����ɾ���ø���
//	}
	return hr;
}

UINT CMapInstanceMgr::GetInstanceMapType(const InstanceID& id)
{
	CMapInstance* ptrInstance=GetInstance(id);
	CHECKF(ptrInstance);
	return ptrInstance->GetMapType();
}

///�õ�������ͼid
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

BOOL CMapInstanceMgr::RequestEnterInstance(PROCESS_ID idOwnProcess,OBJID idUser,OBJID idOwn,UINT nInstanceType,UINT nDomainMapID,UINT nInstanceScope,UINT Cellx,UINT Celly,const char* TeameName)///�����û��������ָ�����͵ĸ���
{
	//ͨ��������ģ��ȷ����δ�����

	//��ͨ��������Ӹ��������Ÿ�����
	if(nInstanceScope==INSTANCETYPE_USER || nInstanceScope==INSTANCETYPE_TEAM/* || nInstanceScope==INSTANCETYPE_SYN*/)
	{
		InstanceID id=make_pair(idOwn,nInstanceType);
		CMapInstance* pMapInstance ;
		CMapInstanceList::iterator iter;
		
		if (IsExistUserInstance(id)==false)
		{
			//����ʲô���Ƿ�ʽ����Ӧ���������ڶ�Ӧ�÷�����
			if(nDomainMapID > 1000000)
			{
				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
				if(pUser)
					pUser->ClearUsingInstance();
				return false;
			}

			//zeng ͬʱ���ߵĸ�������̫�࣬����ִ���������ͼʧ�ܵ�����
			if (m_instancelist.size()>=100)
			{
				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
				if(pUser)
					pUser->SendSysMsg(_TXTATR_NORMAL,"ϵͳ�и��������ﵽ���ߣ��޷��������ั�������Ժ�����");
				return FALSE;
			}

			CreateInstance(id,idOwnProcess,nInstanceScope,nDomainMapID);	///�û���û�и����͵ĸ���,Ϊ������һ��

			iter = m_instancelist.find(id);
			if(iter!=m_instancelist.end())
			{
				pMapInstance = iter->second;
				pMapInstance->SetInstanceScope(nInstanceScope);
// 				pMapInstance->SetAttackSynID(0);
// 				pMapInstance->SetDefenceSynID(0);
			}
		}

		//�жϸ�����������,�ж�ʱ������	
		iter = m_instancelist.find(id);
		if(iter!=m_instancelist.end())
		{
			pMapInstance = iter->second;
			if(!pMapInstance->CanEnterInstance())
			{
				//����ҷ���Ϣ֪ͨ�޷����뵽����
				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
				if(pUser)
					pUser->SendSysMsg("��ǰ����������ս׶Σ��޷�����");
				return FALSE;
			}

			//δ����Ĳ���
			if(nDomainMapID > 1000000 && nDomainMapID < 2000000)
			{
				if(!pMapInstance->IsFreeUser(idUser))//������������ģ�������ߵ�ʱ����������Ѿ������
				{
					CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
					if(pUser)
						pUser->ClearUsingInstance();
					return FALSE;
				}
			}

// 			if(pMapInstance->IsInstanceUserFull())
// 			{
// 				//����ҷ���Ϣ֪ͨ�޷����뵽����
// 				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 				if(pUser)
// 					pUser->SendSysMsg("��ǰ���������������޷�����");
// 				return FALSE;
// 			}
// 			else
			return SendReplyEnterMsg(idUser,id,nInstanceScope,Cellx,Celly);
		}
	}
	return true;
// 	else if(nInstanceScope==INSTANCETYPE_CITYWAR)//��ս����
// 	{
// 		InstanceID id = make_pair(nDomainMapID,nInstanceType);	//�ѳ�ս�����������߶�ΪDomainMapID
// 
// 		//�ж��Ƿ��ܽ����ս����
// 		
// 		CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 		if(NULL == pUser)
// 			return FALSE;
// 
// 		//��������븱��
// 
// 		CMapInstance* pMapInstance = NULL ;
// 		CMapInstanceList::iterator iter;
// 
// 		if (IsExistUserInstance(id)==false)
// 		{
// 			CreateInstance(id,idOwnProcess,nInstanceScope,nDomainMapID);	///�û���û�и����͵ĸ���,Ϊ������һ��
// 			iter = m_instancelist.find(id);
// 			if(iter!=m_instancelist.end())
// 			{
// 				pMapInstance = iter->second;
// 				
// 				//���ó�ս�����������
// 				pMapInstance->SetInstanceScope(nInstanceScope);
// 				pMapInstance->SetDomainMapID(nDomainMapID);
// 				pMapInstance->SetAttackSynID(pUser->GetAttackSyndicateID(nDomainMapID));
// 				pMapInstance->SetDefenceSynID(pUser->GetDefenceSyndicateID(nDomainMapID));
// 
// 				//���ݳ�ս��ʼʱ����㸱��ʣ��ʱ��
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
// 			// 	pUser->SendSysMsg(_TXTATR_NORMAL,"��ǰ���������������޷����룡");
// 				pUser->ProcessActionUser(126, "��ǰ���������������޷����룡", 0);
// 				return FALSE;
// 			}
// 			else if(pUser->IsCityWarAttack(nDomainMapID) && pMapInstance->IsAttackFull(60))
// 			{
// 			// 	pUser->SendSysMsg(_TXTATR_NORMAL,"��ǰ��������������������");
// 				pUser->ProcessActionUser(126, "��ǰ��������������������", 0);
// 				return FALSE;
// 			}
// 			else if(pUser->IsCityWarDefence(nDomainMapID) && pMapInstance->IsDefenceFull(60))
// 			{
// 			// 	pUser->SendSysMsg(_TXTATR_NORMAL,"��ǰ�������ط�����������");
// 				pUser->ProcessActionUser(126, "��ǰ�������ط�����������", 0);
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
// 				pUser->SendSysMsg(_TXTATR_NORMAL,"�ʱ��û�����޷����룡");
// 			return FALSE;
// 		}
// 		InstanceID id = make_pair(nDomainMapID,nInstanceType);
// 
// 		CMapInstance* pMapInstance ;
// 		CMapInstanceList::iterator iter;
// 		
// 		if (IsExistUserInstance(id)==false)
// 		{
// 			CreateInstance(id,idOwnProcess,nInstanceScope);	///�û���û�и����͵ĸ���,Ϊ������һ��
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
// 				//����ҷ���Ϣ֪ͨ�޷����뵽����
// 				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 				if(pUser)
// 					pUser->SendSysMsg(_TXTATR_NORMAL,"��ǰ���������������޷����룡");
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
// 			CreateInstance(id,idOwnProcess,nInstanceScope);	///�û���û�и����͵ĸ���,Ϊ������һ��
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
// 		//�жϸ�����������	
// 		iter = m_instancelist.find(id);
// 		if(iter!=m_instancelist.end())
// 		{
// 			pMapInstance = iter->second;
// 			if(pMapInstance->IsInstanceUserFull())
// 			{
// 				//����ҷ���Ϣ֪ͨ�޷����뵽����
// 				CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 				if(pUser)
// 					pUser->SendSysMsg(_TXTATR_NORMAL,"��ǰ���������������޷����룡");
// 				return FALSE;
// 			}
// 			else
// 				return SendReplyEnterMsg(idUser,id,nInstanceScope,Cellx,Celly);
// 		}
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_SYNWAR)//��ս����
// 	{	
// 		CUser* pUser = MapGroup(idOwnProcess)->GetUserManager()->GetUser(idUser);
// 		if(NULL == pUser)
// 			return FALSE;
// 
// 		InstanceID id = make_pair(idOwn, nInstanceType);	
// 
// 		//��������븱��
// 
// 		CMapInstance* pMapInstance = NULL ;
// 		CMapInstanceList::iterator iter;
// 
// 		if (IsExistUserInstance(id)==false)
// 		{
// 			CreateInstance(id,idOwnProcess,nInstanceScope,nDomainMapID);	///�û���û�и����͵ĸ���,Ϊ������һ��
// 			iter = m_instancelist.find(id);
// 			if(iter!=m_instancelist.end())
// 			{
// 				pMapInstance = iter->second;
// 				
// 				//���ð�ս�����������
// 			// 	pMapInstance->SetOwnID(idOwn);
// 				pMapInstance->SetInstanceScope(nInstanceScope);
// 			// 	pMapInstance->SetDomainMapID(nDomainMapID);
// 				pMapInstance->SetAttackSynID(pUser->GetSynWarInstanceAttackID());
// 				pMapInstance->SetDefenceSynID(pUser->GetSynWarInstanceDefenceID());
// 
// 				//���ݰ�ս����ʱ����㸱��ʣ��ʱ��
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
//  				pUser->ProcessActionUser(126, "��ǰ���������������޷����룡", 0);
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
// 			CreateInstance(id,idOwnProcess,nInstanceScope,nDomainMapID);	///�û���û�и����͵ĸ���,Ϊ������һ��
// 			
// 		}
// 		
// 		//�жϸ�����������
// 		iter = m_instancelist.find(id);
// 		if(iter!=m_instancelist.end())
// 		{
// 			pMapInstance = iter->second;
// 			//�������Ƿ��Ѿ�������
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

void CMapInstanceMgr::RequestCloseInstance(CMsgMapInstance& msg)///�û��������ָ�����͵ĸ���
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
			//�������滹�����
			pInstance->ClearUser();
			++it;
		}
	}

	return hr;
}

BOOL CMapInstanceMgr::ProcessAllInstanceEvent(time_t tCurr)///�������и����¼�
{
	BOOL hr=true;
	for(CMapInstanceList::iterator it=m_instancelist.begin();it!=m_instancelist.end();)
	{
		///����������������и���OnTimer,����ɾ���ø���
		if (it->second->IsActive())
		{
			it->second->OnTimer(tCurr);
			++it;
		}
		else ///�����������ڽ�������֮
			it=RecycleInstance(it);
	}
	return hr;
}

void CMapInstanceMgr::OnTimer(time_t tCurr)///��ʱ��������
{
	ProcessAllInstanceEvent(tCurr);///�������и����¼�

// 	ProcessPvpActive();           //����pvp�
	
	////ɾ����Ч�ĸ���
	DEBUG_TRY
	PureInstanceMgr();
	DEBUG_CATCH("PureInstanceMgr();")
}

CMapInstance* CMapInstanceMgr::FindInstance(OBJID idMap)///Ѱ���û����ڸ���
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

void CMapInstanceMgr::OnPlayerEnterMap(OBJID idMap,OBJID idUser)///�û������ͼ
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

BOOL CMapInstanceMgr::AddPlayer(OBJID idMap,OBJID idUser)///��������ͼ��
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
////	ptrMapInstance->PureInstance();///�������,��InitMapInstance���
//	SAFE_DELETE(ptrMapInstance);///�ͷ��ڴ�
//	return m_instancelist.erase(id);
////	return ptrMapInstance!=NULL;
//}
CMapInstanceList::iterator CMapInstanceMgr::RecycleInstance(const CMapInstanceList::iterator& it)
{
//	CMapInstance* ptrMapInstance=it->second;
//	CHECKF(ptrMapInstance);
	//m_instancerecyclelist[it->first]=it->second;

	//zeng��Ҫ������ط�ɾ������
	//delete (it->second);
	//it->second->KickOutAllPlayer();
	m_instancerecyclelist.insert(make_pair(it->first,it->second));

	return m_instancelist.erase(it);
}

// void CMapInstanceMgr::RemoveInstance(const InstanceID& id)///ɾ������
// {
// 	CMapInstanceList::iterator it=m_instancelist.find(id);
// 	if (it!=m_instancelist.end())
// 		RemoveInstance(it);
// }

// CMapInstanceList::iterator	CMapInstanceMgr::RemoveInstance(const CMapInstanceList::iterator& it)
// {
// 	CMapInstance* ptrMapInstance=it->second;
// 	CHECKF(ptrMapInstance);
// 	SAFE_DELETE(ptrMapInstance);///�ͷ��ڴ�
// 	return /*m_instancerecyclelist.erase(it)*/ NULL;
// }

//void CMapInstanceMgr::OnLifeTimeout(OBJID idOwn,UINT nInstanceType)///�������������ڳ�ʱ�¼�
//{
////	InstanceID id=make_pair(idOwn,nInstanceType);
////	RemoveInstance(id);
////	CHECK(ptrMapInstance);
////	ptrMapInstance->PureInstance();///�ͷŸ���
//		//		if(it->second->IsExpired())
////		{
////			it->second->PureInstance();///�ͷŸ���
////			SAFE_DELETE(it->second);
////			it=m_instancelist.erase(it);
////		}
////		else
////			++it;
//
//}

BOOL CMapInstanceMgr::AddPlayer(CMapInstance* ptrInstance,OBJID idUser)///�����������һ���û�ID
{
	BOOL hr=false;

	return hr;
}

CMapInstance* CMapInstanceMgr::GetInstance(const InstanceID& id)///Ѱ���û����ڸ���
{
	CMapInstanceList::iterator it=m_instancelist.find(id);
	if (it!=m_instancelist.end())
		return it->second;
	else
		return NULL;
}

//BOOL CMapInstanceMgr::RefreshInstanceOwn(const InstanceID& id,OBJID idNewOwn)///���¸���ӵ����ID
//{
//	CMapInstance* ptrInstance=GetInstance(id);
//	if (ptrInstance)
//		ptrInstance->SetOwnID(idNewOwn);
//	return ptrInstance!=NULL;
//}

//void CMapInstanceMgr::OnPlayerTeamCreate(const ParamList& paramlist)///�û��뿪��ͼ
//{
//	OBJID idUser=paramlist[0].lVal;///�û�id
//	OBJID idMap=paramlist[1].lVal;///����ĵ�ͼid	
//	OBJID idTeam=paramlist[2].lVal;///����ĵ�ͼid	
//
//	InstanceID id=make_pair(idUser,idMap);
//	RefreshInstanceOwn(id,idTeam);
////	///���û������ͼʱ,��Ӹ��û���������
////	RemovePlayer(idMap,idUser);
//}

void CMapInstanceMgr::OnPlayerExitMap (OBJID idMap,OBJID idUser)///�û��뿪��ͼ
{
	DEBUG_TRY
	RemovePlayer(idMap,idUser);
	///������Ϊ��ʱ֪ͨ���ж������߶�Ա�������ߵ�ͼλ��
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
//BOOL CMapInstanceMgr::RefreshInstanceOwn(const InstanceID& id,OBJID idNewOwn)///���¸���ӵ����ID
//{
//	CMapInstance* ptrMapInstance=FindInstance(idMap);
//	if(ptrMapInstance)
//		hr=ptrMapInstance->RemovePlayer(idUser);	
//}

BOOL CMapInstanceMgr::RemovePlayer(OBJID idMap,OBJID idUser)///ɾ��������ͼ��һ���û�
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

	//����6151236      ���� 15ʱ12��36��
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

