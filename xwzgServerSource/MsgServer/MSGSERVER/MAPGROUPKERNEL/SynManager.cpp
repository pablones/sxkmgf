// SynManager.cpp: implementation of the CSynManager class.
//
//////////////////////////////////////////////////////////////////////
#include "../share/define.h"
#include "AllMsg.h"
#include "Syndicate.h"
#include "SynManager.h"
#include "MapGroup.h"
#include "SKPostOffice.h"

//////////////////////////////////////////////////////////////////////
const int	MINI_SYN_SIZE			= 0;				// 10人以下的帮派，算迷你帮派。不显示列表。
extern int g_SpecilLine;
MYHEAP_IMPLEMENTATION(CSynManager,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSynManager::CSynManager(PROCESS_ID idProcess) : m_idProcess(idProcess)
{
	m_setSyn = CSynSet::CreateNew(true);
//	m_obj.m_pOwner=this;
}

//////////////////////////////////////////////////////////////////////
CSynManager::~CSynManager()
{
	if(m_setSyn)
		m_setSyn->Release();
}

// bool CSynManager::UpdateSyn(CMsgSynInfo& msg)
// {
// 	CSynPtr pSyn = QuerySyndicate(idSyn);
// 	if(pSyn==NULL)
// 	{
// 		pSyn = CSyndicate::CreateNew(m_idProcess);
// 		if(pSyn==NULL)
// 			return false;
// 	}
// 
// 	return false;
// }

//////////////////////////////////////////////////////////////////////
bool CSynManager::Create()
{
	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s ORDER BY amount DESC", _TBL_SYNDICATE);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
	CHECKF(pRes);
	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		CSynPtr pSyn = CSyndicate::CreateNew(m_idProcess);
		IF_OK(pSyn)
		{
			if(pSyn->Create(pRes))
			{
				m_setSyn->AddObj(pSyn);
			}
			else
				pSyn->ReleaseByOwner();
		}
		pRes->MoveNext();
	}
	pRes->Release();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynManager::Create(OBJID idSyn)
{
	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s where id=%d ORDER BY amount DESC limit 1", _TBL_SYNDICATE,idSyn);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL, false);
	CHECKF(pRes);
	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		CSynPtr pSyn = CSyndicate::CreateNew(m_idProcess);
		CGameMap* pMap = CGameMap::CreateNew();
		CHECKF(pMap);

		IF_OK(pSyn)
		{
			if(pSyn->Create(pRes))
			{
				m_setSyn->AddObj(pSyn);
				if (g_SpecilLine==1)
				{
					if (m_idProcess==MSGPORT_MAPGROUP_FIRST)
					{
						pSyn->CreateSynMap();
					}
				}	
				return true;
			}
			else
				pSyn->ReleaseByOwner();
		}
		pRes->MoveNext();
	}
	pRes->Release();
	return true;
}

//////////////////////////////////////////////////////////////////////
// interface
//////////////////////////////////////////////////////////////////////
OBJID CSynManager::CreateSyndicate(CreateSyndicateInfo* pInfo)
{
	OBJID idSyn = pInfo->idSyn;
	CGameMap* pMap=CGameMap::CreateNew();

	CHECKF(pMap);
	if (this->QuerySyndicate(idSyn)) 
	{
		return idSyn;
	}

	CSynPtr pSyn = CSyndicate::CreateNew(m_idProcess);
	IF_OK(pSyn)
	{
		IF_OK(pSyn->Create(pInfo, true))		// false: not save
		{
			m_setSyn->AddObj(pSyn);
			if (g_SpecilLine==1)
			{
				if (m_idProcess==MSGPORT_MAPGROUP_FIRST)
				{
					pSyn->CreateSynMap();
				}
			}
			return pInfo->idSyn;
		}
		else
			pSyn->ReleaseByOwner();
	}
	return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
bool CSynManager::DestroySyndicate(OBJID idSyn, OBJID idTarget)
{
	CSynPtr pSyn = QuerySyndicate(idSyn);
	if(pSyn)
	{
		SQLBUF	szSQL;//STR_SYNDICATE_DESTROY_s
		sprintf(szSQL, g_objStrRes.GetStr(11059), pSyn->GetStr(SYNDATA_LEADERNAME), pSyn->GetStr(SYNDATA_NAME));
		RoleManager()->BroadcastMapGroupMsg(szSQL);

		CMsgSyndicate	msg;
		IF_OK(msg.Create(DESTROY_SYN, idSyn))
			RoleManager()->BroadcastMapGroupMsg(&msg);

		CSyndicate* pTargetSyn = NULL;
		if (idTarget != ID_NONE)
			pTargetSyn = SynManager()->QuerySyndicate(idTarget);

		// 删除帮众
		CUserManager::Iterator pUser = UserManager()->NewEnum();
		while(pUser.Next())
		{
			if(pUser && pUser->QuerySynAttr() && pUser->QuerySynAttr()->GetSynID() == idSyn)
			{
				if (idTarget == ID_NONE)
				{
					pUser->QuerySynAttr()->LeaveSyn(false, SYNCHRO_FALSE, SYNCHRO_FALSE);		// false: 不删除记录，以防误操作。
					
					CMsgPlayer msgPlayer;
					IF_OK(msgPlayer.Create(pUser->QueryRole()))
					pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
				}
//				else
//				{
//					int nRank = pUser->QuerySynAttr()->GetSynRank();
//					if (nRank > RANK_NORMAL)
//						nRank = RANK_NORMAL;
//					pUser->QuerySynAttr()->SetIdRank(0, idSyn,  nRank, SYNCHRO_FALSE);
//				}
			}
		}
		m_setSyn->DelObj(idSyn);

//		DesSynRelateSynWar(idSyn);
	}
	return true;
}

void CSynManager::SendSynList(CUser* pUser,int Page)
{
	CMsgSyndicateList msgList;
	if (!msgList.Create(SYNLISTACTION_GETSYNLIST,0,"","",0,0))
		return;
	int aAmount = 0;
	int nNowSend=0;
	for(int i = 0; i < m_setSyn->GetAmount(); i++)
	{
		CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
		if(NULL == pSyn)
			continue;
		if (0 == pSyn->GetLev())			//解散的帮派不予理睬
			continue;	
		if(pSyn->GetInt(SYNDATA_STATE) == 2)
		{
			aAmount ++;
			msgList.Append(pSyn->GetID(),pSyn->GetStr(SYNDATA_NAME),pSyn->GetStr(SYNDATA_LEADERNAME),pSyn->GetLev(),pSyn->GetMemberAmount());
			nNowSend++;
			if (nNowSend>10)
			{
				msgList.SetMaxData(aAmount);
				pUser->SendMsg(&msgList);
				if (!msgList.Create(SYNLISTACTION_GETSYNLIST,0,"","",0,0))
					return;
				nNowSend=0;
			}
		}
	}
	if (nNowSend!=0)
	{
		msgList.SetMaxData(aAmount);
		pUser->SendMsg(&msgList);
	}
	

}

void CSynManager::SendCreateSynToUser(CUser* pUser,int Page)
{
	CMsgSyndicateList msgList;
	if (!msgList.Create(SYNLISTACTION_CREATE + Page * 100 ,0,"","",0,0))
		return;
	int aAmount = 0;
	for(int i = 0; i < m_setSyn->GetAmount(); i++)
	{
		CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
		if(NULL == pSyn)
			continue;			
		if(pSyn->GetInt(SYNDATA_STATE) == 1)
		{
			aAmount ++;
			if((Page - 1) * 10 < aAmount &&  aAmount <= Page * 10 )
				msgList.Append(pSyn->GetID(),pSyn->GetStr(SYNDATA_NAME),pSyn->GetStr(SYNDATA_LEADERNAME),pSyn->GetInt(SYNDATA_LEVEL),pSyn->GetInt(SYNDATA_AMOUNT));
		}
	}
	msgList.SetMaxData(aAmount);

	pUser->SendMsg(&msgList);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
CSyndicate* CSynManager::QuerySynByName(LPCTSTR szSyn)
{
	CHECKF(szSyn);

	for(int i = 0; i < m_setSyn->GetAmount(); i++)
	{
		CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
		if(pSyn && strcmp(pSyn->GetStr(SYNDATA_NAME), szSyn) == 0)
			return pSyn;
	}

	return NULL;
}


void CSynManager::SynchroAllLine(OBJID idSyn)///同步线上所有服务此帮派的信息
{
	///同步所有线的此帮派的信息
	CMsgSyndicate msg;
	if(msg.Create(SYN_CREATE_OK,idSyn))
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, ALLUSERS_NAME);
}

void CSynManager::ProcChackSynWar()
{
	for(int i = 0; i < m_setSyn->GetAmount(); i++)
	{
		CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
		if( !pSyn )
			continue;
		pSyn->ProcAutoSynWar();
	}
}
void CSynManager::ProcAutoDestoryOrFallSyn()
{
	for(int i = 0; i < m_setSyn->GetAmount(); i++)
	{
		CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
		if( !pSyn )
			continue;
		pSyn->ProcAutoDismissOrDestory();
	}
}
//////////////////////////////////////////////////////////////////////
// order by amount, return false: no more syn
bool CSynManager::FindNextSyn(int& nIter)
{
	OBJID	idLastSyn	= nIter;
	CSynPtr pTarget		= NULL;
	bool	bFindNext	= true;
	for(int i = 0; i < m_setSyn->GetAmount(); i++)
	{
		CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
		if(pSyn && pSyn->GetInt(SYNDATA_AMOUNT) > MINI_SYN_SIZE)
		{
			if(bFindNext)
			{
				pTarget = pSyn;
				bFindNext = false;
			}

			if(pSyn->GetID() == idLastSyn)
			{
				pTarget = NULL;
				bFindNext = true;
				continue;
			}
		}
	}

	if(pTarget)
	{
		nIter = pTarget->GetID();
		return true;
	}
	return false;
}


void CSynManager::OnTimer(time_t tCurr)
{
	int nStep = 0;
	DEBUG_TRY
// 	for(int j = 0; j < m_setSyn->GetAmount(); j++)
// 	{
// 		CSyndicate* pSyn = m_setSyn->GetObjByIndex(j);
// 		if( !pSyn || pSyn->GetInt(SYNDATA_STATE) < 2)
// 			continue;
// 		if (pSyn && pSyn->GetInt(SYNDATA_PAY) >= 25)
// 		{
// 			SynManager()->DestroySyndicate(pSyn->GetID());
// 		}
// 	}
	nStep = 1;
	for(int i = 0; i < m_setSyn->GetAmount(); i++)
	{
		nStep = 11;
		CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
		nStep = 12;
		if( !pSyn || pSyn->GetInt(SYNDATA_STATE) < 2)
			continue;
		nStep = 13;
		if (pSyn)
		{
			nStep = 14;
			pSyn->OnTimer(tCurr);
		}
	}
	DEBUG_CATCH2("CSynManager::OnTimer(time_t tCurr), step = %d", nStep);
}

BOOL CSynManager::CheckSynName(const char *szName)
{
	char szOldName[64] = "";
// 	for(int i = 0; i < m_setSyn->GetAmount(); i++)
// 	{
		SQLBUF	szSQL;
		sprintf(szSQL, "SELECT id FROM %s WHERE name='%s'", _TBL_SYNDICATE, szName );
		IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
		if (pRes)
		{
//			SafeCopy(szOldName, pRes->GetStr(2), _MAX_NAMESIZE);
			// 		CSyndicate* pSyn = m_setSyn->GetObjByIndex(i);
			// 		if( !pSyn || pSyn->GetInt(SYNDATA_STATE) < 2)
			// 			continue;
			// 		if (pSyn)
			// 		{
// 			if (strcmp(szName, szOldName/*pSyn->GetStr(SYNDATA_NAME)*/) == 0)
// 			{
			if (pRes->RecordCount())
			{
				SAFE_RELEASE(pRes);
				return false;
			}		
//			}
		}


//	}
	return true;
}

bool CSynManager::DestroySynSub(OBJID idSyn, OBJID idSub)
{
	CSynPtr pSyn = QuerySyndicate(idSyn);
	if(!pSyn)
		return false;
	// 删除帮众
	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while(pUser.Next())
	{
		if(pUser && pUser->QuerySynAttr()&& pUser->QuerySynAttr()->GetSynID() == idSyn)
		{
			int nRank = pUser->QuerySynAttr()->GetSynRank();
			if (nRank > RANK_NORMAL)
			{
				if (nRank % 10 == idSub)
				{
					nRank = RANK_NORMAL;
		                	pUser->QuerySynAttr()->SetIdRank(0, idSyn, nRank, SYNCHRO_FALSE);
				}
			}
		}
	}
	pSyn->SetStopsub(idSub);
	pSyn->SetPay(1);
	return true;
}
