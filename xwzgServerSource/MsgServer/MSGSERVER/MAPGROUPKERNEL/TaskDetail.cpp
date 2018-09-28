// TaskDetail.cpp: implementation of the CTaskDetail class.
//
//////////////////////////////////////////////////////////////////////

#include "TaskDetail.h"
#include "I_Role.h"
#include "MapGroup.h"
#include "User.h"
#include "AllMsg.h"


MYHEAP_IMPLEMENTATION(CTaskDetail, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction]
//////////////////////////////////////////////////////////////////////

CTaskDetail::CTaskDetail()
{

}

CTaskDetail::~CTaskDetail()
{
	TASKDETAILDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CTaskDetailData* pData = *it;
		SAFE_RELEASE (pData);
	}
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::Create(PROCESS_ID idProcess, CUser *pUser,bool bLogin/* = true*/)
{
	CHECKF(pUser);

	m_idProcess	= idProcess;

	if(!bLogin)
		return true;

	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE userid=%d ORDER BY taskid", _TBL_TASKDETAIL, pUser->GetID());
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
	if (pRes)
	{
		for (int i=0; i<pRes->RecordCount(); i++)
		{
			CTaskDetailData* pData = CTaskDetailData::CreateNew();
			if (pData && pData->Create(pRes, Database()))
			{
				m_setData.push_back(pData);
			}
			else
			{
				SAFE_RELEASE (pData);
				SAFE_RELEASE (pRes);
				return false;
			}

			pRes->MoveNext();
		}
		SAFE_RELEASE (pRes);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::CreateNewTaskDetailData(ST_TASKDETAILDATA *pInfo,bool bInsert)
{
	CTaskDetailData* pData = CTaskDetailData::CreateNew();
	if (pData)
	{
		if( pData->CreateRecord( GameDataDefault()->GetTaskDetailData(),pInfo,bInsert ) )
		{
			m_setData.push_back(pData);
			return true;
		}
		SAFE_RELEASE (pData);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
CTaskDetailData* CTaskDetail::QueryData(OBJID idData)
{
	TASKDETAILDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CTaskDetailData* pData = *it;
		if (pData && pData->GetID()==idData)
			return pData;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
CTaskDetailData* CTaskDetail::QueryData(OBJID idUser, OBJID idTask)
{
	TASKDETAILDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CTaskDetailData* pData = *it;
		if (pData && pData->GetInt(TASKDETAILDATA_USERID) == idUser	&& pData->GetInt(TASKDETAILDATA_TASKID) == idTask)
			return pData;
	}
	return NULL;

}

CTaskDetailData* CTaskDetail::QueryDataByChain(OBJID idUser,OBJID idChain)
{
	TASKDETAILDATA_SET::iterator it = m_setData.begin();
	for(;it!=m_setData.end();it++)
	{
		CTaskDetailData *pData = *it;
	//	if(pData && pData->GetInt(TASKDETAILDATA_USERID) == idUser && pData->GetInt(TASKDETAILDATA_TASKCHAIN) == idChain)
			return pData;
	}
	return NULL;
}

int CTaskDetail::QueryDataNumByChain(OBJID idUser,OBJID idChain)
{
	int nResult = 0;
	TASKDETAILDATA_SET::iterator it = m_setData.begin();
	for(;it!=m_setData.end();it++)
	{
		CTaskDetailData *pData = *it;
//		if(pData && pData->GetInt(TASKDETAILDATA_USERID) == idUser && pData->GetInt(TASKDETAILDATA_TASKCHAIN) == idChain)
			++nResult;
	}
	return nResult;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::DeleteData(OBJID idUser, OBJID idTask)
{
	TASKDETAILDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CTaskDetailData* pData = *it;
		if (pData && pData->GetInt(TASKDETAILDATA_USERID) == idUser	&& pData->GetInt(TASKDETAILDATA_TASKID) == idTask)
		{
			pData->DeleteRecord();
			SAFE_RELEASE (pData);
			m_setData.erase(it);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::FindNextData(OBJID idUser,int &nIter)
{
	OBJID	idLast	= nIter;
	OBJID	idNext	= ID_NONE;
	bool	nBreak		= false;

	TASKDETAILDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CTaskDetailData* pData = *it;
		if (pData && pData->GetInt(TASKDETAILDATA_USERID) == idUser)
		{
			if(idNext == ID_NONE)
			{
				idNext	= pData->GetID();

				if(nBreak)
					break;
				else
					nBreak = true;
			}
			if(pData->GetID() == idLast)
			{
				idNext	= ID_NONE;
				continue;
			}
		}			
	}

	if(idNext)
	{
		nIter = idNext;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::FindData(OBJID idTask, int nPhase, int nCompleteNum, int &nIter)
{
	TASKDETAILDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CTaskDetailData* pData = *it;
		if(nCompleteNum == -1 && nPhase == -1)
		{
			if (pData && pData->GetInt(TASKDETAILDATA_TASKID) == idTask)
			{
				nIter = pData->GetID();
				return true;
			}
		}
		else
		{
			if (pData && pData->GetInt(TASKDETAILDATA_TASKID) == idTask	&& pData->GetInt(TASKDETAILDATA_TASKPHASE) == nPhase
				/*&& pData->GetInt(TASKDETAILDATA_TASKCOMPLETENUM) == nCompleteNum*/)
			{
				nIter = pData->GetID();
				return true;
			}
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::FindNextData(int &nIter)
{
	OBJID	idLast	= nIter;
	OBJID	idNext	= ID_NONE;
	bool	nBreak		= false;

	TASKDETAILDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CTaskDetailData* pData = *it;
		{
			if(idNext == ID_NONE)
			{
				idNext	= pData->GetID();

				if(nBreak)
					break;
				else
					nBreak = true;
			}
			if(pData->GetID() == idLast)
			{
				idNext	= ID_NONE;
				continue;
			}
		}			
	}

	if(idNext)
	{
		nIter = idNext;
		return true;
	}
	return false;
}

CTaskSetRandData*  CTaskDetail::GetRandTask(int nChain,OBJID idNpc)
{
	CTaskSetRand* pTaskSet= NULL;
    pTaskSet = TaskSetRand();
	if(pTaskSet)
	{
   //    if(pTaskSet->IsValid(nChain))
	   {
		   CTaskSetRandChain* pTaskChain = pTaskSet->GetObjByIndex(nChain);
           if(pTaskChain)
		   {
			   return pTaskChain->GetRandTask(idNpc);
		   }
	   }
	}
	return NULL;
}

ULONG CTaskDetail::DeleteDataByChain(OBJID idUser,OBJID idChain)
{
	ULONG resultId = 0;
	TASKDETAILDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CTaskDetailData* pData = *it;
		if (pData && pData->GetInt(TASKDETAILDATA_USERID) == idUser/*	&& pData->GetInt(TASKDETAILDATA_TASKCHAIN) == idChain*/)
		{
			resultId = pData->GetInt(TASKDETAILDATA_TASKID);
			pData->DeleteRecord();
			SAFE_RELEASE (pData);
			m_setData.erase(it);
			return resultId;
		}
	}
	return resultId;
}

void CTaskDetail::SetTaskSpendEneType(OBJID idUser, OBJID idTask, USHORT nType)
{
	CTaskDetailData* pData = QueryData(idUser, idTask);
	CHECK(pData);
	pData->SetInt(TASKDETAILDATA_SPENDENETYPE, nType, true);
}

UINT CTaskDetail::GetTaskSpendEneType(OBJID idUser, OBJID idTask)
{
	CTaskDetailData* pData = QueryData(idUser, idTask);
	CHECKF(pData);
	return pData->GetInt(TASKDETAILDATA_SPENDENETYPE);
}

void  CTaskDetail::SendAllObjInfo(CUser* pUser,PROCESS_ID idProcess)
{
	TASKDETAILDATA_SET::iterator it=m_setData.begin();
	for (; it!=m_setData.end(); it++)
	{
		CTaskDetailData* pData = *it;
		if (pData)
		{
			ST_TASKDETAILDATA info;
			if(pData->GetInfo(info))
				pUser->SendObjInfo(idProcess,pUser->GetID(),INFO_TASKDETAIL,&info,sizeof(info));
		}
	}
}
