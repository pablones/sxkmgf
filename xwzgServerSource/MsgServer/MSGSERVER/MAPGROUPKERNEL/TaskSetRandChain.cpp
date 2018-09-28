// TaskSetRandChain.cpp: implementation of the CTaskSetRandChain class.
//
//////////////////////////////////////////////////////////////////////
#include "common.h"
#include "basefunc.h"

#include "TaskSetRandChain.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MYHEAP_IMPLEMENTATION(CTaskSetRandChain,s_heap)

CTaskSetRandChain::CTaskSetRandChain()
{

}

CTaskSetRandChain::~CTaskSetRandChain()
{
     Destroy();
}

bool CTaskSetRandChain::Create(OBJID idChain,IDatabase * pdb)
{
	CHECKF(pdb);
	char szSql[1024];

	sprintf(szSql,"SELECT * FROM %s where taskid=%u", _TBL_RANDTASKSET, idChain);
	IRecordset* pRes = pdb->CreateNewRecordset(szSql);
	if(pRes)
	{
		bool ret = LoadInfo(pRes);
		pRes->Release();
		ZeroMemory(szSql,sizeof(char)*1024);
	}
	return true;
}

bool   CTaskSetRandChain::LoadInfo(IRecordset*	pRes)
{
	CHECKF(pRes);
	DEBUG_TRY
    for(int i=0;i<pRes->RecordCount();i++)
	{
		CTaskSetRandData * pData = NULL;

		if(!pData)
			pData = CTaskSetRandData::CreateNew();

		if(!pData)
			return false;

		if(!pData->Create(pRes))
		{
			pData->Release();
			return false;
		}
        m_setRule.push_back(pData);
		pRes->MoveNext();

	}
	DEBUG_CATCH("CTaskSetRandChain::LoadInfo()")
	return true;
}

void CTaskSetRandChain::Destroy()
{
	DEBUG_TRY
		for (int i=0; i<m_setRule.size(); i++)
		{
		   CTaskSetRandData*	pRule	= m_setRule[i];
		   if (pRule)
			  pRule->Release();
		}
	    m_setRule.clear();

	DEBUG_CATCH("CTaskSetRandChain::Destroy()")
}

CTaskSetRandData* CTaskSetRandChain::GetRandTask(OBJID idTask,OBJID step)
{
	int nSize = m_setRule.size();
	for(int i = 0;i<nSize;i++)
	{
		if(m_setRule[i]->TaskData.taskid == idTask)
		{
			if(m_setRule[i]->TaskData.ischain == 0 || m_setRule[i]->TaskData.taskchain == step)
				return m_setRule[i];
		}
	}
	return NULL;
}

CTaskSetRandData* CTaskSetRandChain::GetRandTask(OBJID idNpc)
{
	int nSize = m_setRule.size();
    RANDDATA_SET vContain;
	for(int i = 0;i<nSize;i++)
	{
		OBJID idReceive = m_setRule[i]->TaskData.Receive_NPC; 
		if (idReceive == 0 || idReceive == idNpc)
		{
			vContain.push_back(m_setRule[i]);
		}
	}
    nSize = vContain.size();
	int nIndex =RandGet(nSize);
	return vContain[nIndex];
}

DWORD CTaskSetRandChain::GetSetTime(DWORD idTask,DWORD idStep)
{
	for (int i=0; i<m_setRule.size(); i++)
	{
		CTaskSetRandData*	pRule	= m_setRule[i];
		if (pRule&&pRule->TaskData.taskid == idTask && pRule->TaskData.taskchain == idStep)
		{
			return pRule->TaskData.setTime;
		}
	}
	return 0;
}
DWORD CTaskSetRandChain::GetActionData(DWORD taskId)
{
	for (int i=0; i<m_setRule.size(); i++)
	{
		CTaskSetRandData*	pRule	= m_setRule[i];
		if (pRule&&pRule->TaskData.taskid == taskId)
		{
			return /*pRule->TaskData.ActionId*/0;
		}
	}
	return 0;
}

DWORD CTaskSetRandChain::GetActionDataGenerate(DWORD taskId,DWORD npcId)
{
	for (int i=0; i<m_setRule.size(); i++)
	{
		CTaskSetRandData*	pRule	= m_setRule[i];
		if (pRule&&pRule->TaskData.taskid == taskId &&(pRule->TaskData.Receive_NPC == 0 || pRule->TaskData.Receive_NPC == npcId))
		{
			return /*pRule->TaskData.GenerateAction*/0;
		}
	}
	return 0;
}

DWORD  CTaskSetRandChain::GetTargetAmount(DWORD taskId)
{
	for (int i=0; i<m_setRule.size(); i++)
	{
		CTaskSetRandData*	pRule	= m_setRule[i];
		if (pRule&&pRule->TaskData.taskid == taskId)
		{
			return /*pRule->TaskData.targetNum*/0;
		}
	}
	return 0;

}

int CTaskSetRandChain::IsTaskOnTime(DWORD taskId)
{
	for (int i=0; i<m_setRule.size(); i++)
	{
		CTaskSetRandData*	pRule	= m_setRule[i];
		if (pRule&&pRule->TaskData.taskid == taskId)
		{
			return pRule->TaskData.utype;
		}
	}
	return -1;
}