// TaskSetRand.cpp: implementation of the CTaskSetRand class.
//
//////////////////////////////////////////////////////////////////////

#include "TaskSetRand.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CTaskSetRand, s_heap)

const int _TASKMAXNUM = 255;//最大任务数目，不可能超过255

#ifdef _DEBUG
#define new DEBUG_NEW
#undef  THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

#define TASKONTIME 4   //是否及时任务
CTaskSetRand::CTaskSetRand()
{

}

CTaskSetRand::~CTaskSetRand()
{
    Destroy();
}

bool CTaskSetRand::Create(IDatabase* pDb)
{
	CHECKF(pDb);

	char szSQL[1024];
	IRecordset* pRes = NULL;
	sprintf(szSQL, "SELECT DISTINCT taskid FROM %s", _TBL_RANDTASKSET);
    pRes = pDb->CreateNewRecordset(szSQL);
    int maxNum = pRes->RecordCount();
	for (int i=0;i<maxNum;i++)
	{
		if(pRes)
		{
			CTaskSetRandChain*	pTaskChain = NULL;
			OBJID	idTask = -1;

			DEBUG_TRY
			idTask = pRes->LoadDWord("taskid");
			if(idTask == -1)
			{
				SAFE_RELEASE(pRes);
				return false;
			}

			pTaskChain = CTaskSetRandChain::CreateNew();
			if (!pTaskChain)
			{
				SAFE_RELEASE(pRes);
				return false;
			}

			if (!pTaskChain->Create(idTask, pDb))
			{
				pTaskChain->Release();
				SAFE_RELEASE(pRes);
				return false;
			}

			m_map[idTask] = pTaskChain;
			pRes->MoveNext();

			DEBUG_CATCH("CTaskSetRand::Create()")
		}
	}
	ZeroMemory(szSQL,sizeof(char)*1024);
	pRes->Release();
	return true;
}

void CTaskSetRand::Destroy()
{
  DEBUG_TRY
  Iter  it = m_map.begin();
  for(;it!=m_map.end();it++)
  {
	  CTaskSetRandChain * taskSet = it->second;
	  if(taskSet)
	  {
            taskSet->Release();
	  }
  }
  DEBUG_CATCH("CTaskSetRand::Destroy()")
  m_map.clear();
}


DWORD CTaskSetRand::GetSetTime(DWORD idTask,DWORD idStep)
{
	Iter it = m_map.begin();
	for(;it !=m_map.end();it++)
	{
		CTaskSetRandChain *taskSet = it->second;
		if(taskSet)
		{
			DWORD idAction = taskSet->GetActionData(idTask);
			if(idAction)
				return idAction;
		}
	}
	return 0;
}
CTaskSetRandData* CTaskSetRand::GetDataStruct(OBJID	idChain,OBJID step)
{
	if (m_map[idChain]) 
		return m_map[idChain]->GetRandTask(idChain,step);
	else
		return NULL;
}

DWORD CTaskSetRand::GetActionId(DWORD idTask)
{
    Iter it = m_map.begin();
	for(;it !=m_map.end();it++)
	{
		CTaskSetRandChain *taskSet = it->second;
		if(taskSet)
		{
			DWORD idAction = taskSet->GetActionData(idTask);
			if(idAction)
				return idAction;
		}
	}
	return 0;
}

DWORD CTaskSetRand::GetActionGenerate(DWORD idTask,DWORD idNpc)
{
	Iter it = m_map.begin();
	for (;it != m_map.end();it++)
	{
		CTaskSetRandChain *taskSet = it->second;
		if(taskSet)
		{
			DWORD idAction = taskSet->GetActionDataGenerate(idTask,idNpc);
			if(idAction)
				return idAction;
		}
	}
	return 0;
}

DWORD CTaskSetRand::GetTargetNum(DWORD idTask)
{
	Iter it = m_map.begin();
	for (;it != m_map.end();it++)
	{
		CTaskSetRandChain *taskSet = it->second;
		if(taskSet)
		{
			DWORD idAction = taskSet->GetTargetAmount(idTask);
			if(idAction)
				return idAction;
		}
	}
	return 0;
}

BOOL CTaskSetRand::IsTaskOnTime(DWORD idTask)
{
    Iter it = m_map.begin();
	for(;it !=m_map.end();it++)
	{
		CTaskSetRandChain *taskSet = it->second;
		if(taskSet)
		{
			int ucType = taskSet->IsTaskOnTime(idTask);
			if(ucType != -1)
				return ucType == TASKONTIME;
		}
	}
	return FALSE;
}