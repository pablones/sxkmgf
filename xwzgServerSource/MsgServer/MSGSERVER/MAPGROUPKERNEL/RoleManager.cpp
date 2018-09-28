// RoleManager.cpp: implementation of the CRoleManager class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "../share/define.h"
#include "RoleManager.h"
#include "MapGroup.h"
#include "AllMsg.h"

MYHEAP_IMPLEMENTATION(CRoleManager,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
CRoleManager::CRoleManager(PROCESS_ID idProcess) : m_idProcess(idProcess)
{
	m_setRole	= CRoleSet::CreateNew(true);
	
}

//////////////////////////////////////////////////////////////////////
CRoleManager::~CRoleManager()
{
	if(m_setRole)
		m_setRole->Release();

}

//////////////////////////////////////////////////////////////////////
bool CRoleManager::Create()
{
	CHECKF(m_setRole);

// need not	LoadAllPet();

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CRoleManager::LoadAllPet()		//??? 最好由AI服务器分段创建
{
	return true;
}

//////////////////////////////////////////////////////////////////////
IRole* CRoleManager::QueryRole(CNetMsg* pMsg, OBJID idRole)
{
	if(!pMsg->IsNpcMsg())
	{
		CUserPtr pUser = UserManager()->GetUserBySocketID(pMsg->GetSocketID());
		if(pUser)
		{
			if (idRole && idRole == pUser->GetCallPetID())
				return pUser->QueryCallPet()->QueryRole();
			else
			{
				if (idRole >= CALLPETID_FIRST && idRole <= CALLPETID_LAST)
				{
					return NULL;
				}
				else if(idRole >= EUDEMON_ID_FIRST && idRole <= EUDEMON_ID_LAST)
				{
					return  QuerySet()->GetObj(idRole);
				}
				else
					return pUser->QueryRole();
			}
		}
	}
	else
	{
		return  QuerySet()->GetObj(pMsg->GetNpcID());
	}
	return NULL;
}

bool CRoleManager::CheckValidPt(IRole* pRole)
{
	CHECKF (pRole);
	
	try {
		BOOL bIsValid = true;
		int nObjType = 0;
		DEBUG_TRY
		CUser* pUser = NULL;
		if(pRole->QueryObj(OBJ_USER,( void** )&pUser))
		{
			nObjType = OBJ_USER;
#ifndef _DEBUG
			bIsValid = CUser::IsValidPt(pUser);
#endif
		}
		DEBUG_CATCH("CheckValidPt OBJ_USER")
			
	
		if (!bIsValid)
		{
			LOGWARNING("CheckValidPt !bIsValid nObjType = %d",nObjType);
			return false;
		}
	} 
	catch (...) 
	{
		LOGWARNING("CheckValidPt try");
		return false;
	}
	return true;

}

//////////////////////////////////////////////////////////////////////
void CRoleManager::OnTimer(time_t tCurr)
{
	//函数说明:
	//注释掉以前的i++,改放在新的位置;
	//i++在之前断言USER存在,所以在下一次执行时就会出错
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); )		// 删除
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
	//	i++;		//??? 提前移动，下面可能会删除当前角色,06.11.3王玉波注释
		if(pRole)
		{
			if(!CheckValidPt(pRole))
			{
				i++;
				LOGWARNING("CheckValidPt(pRole)");
				continue;
			}
			DEBUG_TRY
			i++;							//06.11.3王玉波添加
			if(pRole->IsDelThis())
			{
				m_setRole->DelObj(pRole->GetID());
				continue;
			}
			pRole->OnTimer(tCurr);// 可能删除MONSTER
			DEBUG_CATCH2("@CRoleManager::OnTimer@ %u",pRole->GetID());
		}
	}
	
/*	for(int i = m_setRole->GetAmount()-1; i >= 0; i--)		// 删除
	{
		IRole* pRole = m_setRole->GetObjByIndex(i);
		if(pRole)
			pRole->OnTimer(tCurr);
	}*/
}

//////////////////////////////////////////////////////////////////////
void CRoleManager::SaveAll()
{
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole)
			pRole->SaveInfo();
	}
}

//////////////////////////////////////////////////////////////////////
void CRoleManager::BroadcastMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
	CHECK(pMsg);

	BroadcastMapGroupMsg(pMsg, pExcludeSender);

	MapGroup(PID)->QueryIntraMsg()->SendBroadcastMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
void CRoleManager::BroadcastMsg(LPCTSTR szMsg, CUser* pExcludeSender)
{
	CHECK(szMsg);

	CMsgTalk	msg;
	if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg))
		BroadcastMsg(&msg, pExcludeSender);
}

//////////////////////////////////////////////////////////////////////
// for mapgroup
//////////////////////////////////////////////////////////////////////
void CRoleManager::BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
	ASSERT(pMsg);

	UserManager()->GetForMapGroup()->BroadcastMapGroupMsg(pMsg, pExcludeSender);
}

//////////////////////////////////////////////////////////////////////
void CRoleManager::BroadcastMapGroupMsg(LPCTSTR szMsg, CUser* pExcludeSender)
{
	CHECK(szMsg);

	CMsgTalk	msg;
	if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg))
		UserManager()->GetForMapGroup()->BroadcastMapGroupMsg(&msg, pExcludeSender);
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountAllNpc(OBJID idMap)
{
	int		nCount = 0;
	CNpc*	pNpc;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == idMap)
			nCount++;
	}
	return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountFurnitureNpc(OBJID idMap)
{
	int		nCount = 0;
	CNpc*	pNpc;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == idMap && (pNpc->GetType()==_ROLE_FURNITURE_NPC || pNpc->GetType()==_ROLE_3DFURNITURE_NPC))
			nCount++;
	}
	return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountNpcByName(OBJID idMap, LPCTSTR szName)
{
	int		nCount = 0;
	CNpc*	pNpc;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == idMap && strcmp(pNpc->GetName(), szName)==0)
			nCount++;
	}
	return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountNpcByType(OBJID idMap, int nType)
{
	int		nCount = 0;
	CNpc*	pNpc;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetMapID() == idMap && pNpc->GetType() ==  nType)
			nCount++;
	}
	return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountMonsterByName(OBJID idMap, LPCTSTR szName)
{
	int		nCount = 0;
	CMonster*	pMonster;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && pMonster->GetMap()->GetID() == idMap && strcmp(pMonster->GetName(), szName)==0)
			nCount++;
	}
	return nCount;
}

//////////////////////////////////////////////////////////////////////
int CRoleManager::CountMonsterByGenID(OBJID idMap, OBJID idGen)
{
	int		nCount = 0;
	CMonster*	pMonster;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && pMonster->GetMap()->GetID() == idMap && pMonster->GetGenID() ==  idGen)
			nCount++;
	}
	return nCount;
}

//////////////////////////////////////////////////////////////////////
CNpc* CRoleManager::QueryDynNpcByRecordID(OBJID idNpcRecord)
{
	CNpc*	pNpc;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsDynaNpc() && pNpc->GetRecordID() == idNpcRecord)
			return pNpc;
	}
	return NULL;
}

int CRoleManager::CountAliveMonsterByName(OBJID idMap, LPCTSTR szName)
{
	int		nCount = 0;
	CMonster*	pMonster;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) 
			&& pMonster->GetMap()->GetID() == idMap 
			&& strcmp(pMonster->GetName(), szName)==0
			&& pRole->IsAlive())
			nCount++;
	}
	return nCount;
}

int CRoleManager::CountAliveMonsterByGenID(OBJID idMap, OBJID idGen)
{
	int		nCount = 0;
	CMonster*	pMonster;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) 
			&& pMonster->GetMap()->GetID() == idMap 
			&& pMonster->GetGenID() ==  idGen
			&& pRole->IsAlive())
			nCount++;
	}
	return nCount;
}

//  [2007-8-27] 朱斌
int CRoleManager::CountAliveMonsterByType(OBJID idMap, OBJID idType)
{
	int		nCount = 0;
	CMonster*	pMonster;
	for(IRoleSet::Iter i = m_setRole->Begin(); i != m_setRole->End(); i++)
	{
		IRole* pRole = m_setRole->GetObjByIter(i);
		if(pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) 
			&& pMonster->GetMap()->GetID() == idMap 
			&& pMonster->GetType() ==  idType
			&& pMonster->IsAlive())
			nCount++;
	}
	return nCount;
}

void CRoleManager::AddTrap(OBJID idMap,IRole* pTrap)
{
	//如果该idmap不存在，会自动的创建一个空的vector
	m_setTrap[idMap].push_back(pTrap);
}

void CRoleManager::DelTrap(OBJID idMap,IRole* pTrap)
{
	map<OBJID,vector<IRole*> >::iterator it = m_setTrap.find(idMap);
	if(it!=m_setTrap.end())
	{
		vector<IRole*>::iterator iter;
		for(iter = (it->second).begin();iter!= (it->second).end();++iter)
		{
			if((*iter)==pTrap)
			{
				(it->second).erase(iter);
				break;
			}
		}

		if((it->second).empty())
			m_setTrap.erase(it);
	}
}