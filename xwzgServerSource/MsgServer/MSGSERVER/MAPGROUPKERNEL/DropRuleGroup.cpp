// DropRuleGroup.cpp: implementation of the CDropRuleGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "basefunc.h"

#include "DropRuleGroup.h"

#include <algorithm>

MYHEAP_IMPLEMENTATION(CDropRuleGroup,s_heap)

const int _DROP_CHANCE_PRECISION	= 1000000;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropRuleGroup::CDropRuleGroup()
{
	m_idRuleGroup	= ID_NONE;
}

CDropRuleGroup::~CDropRuleGroup()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////

bool	CDropRuleGroup::Create(OBJID	idRuleGroup, IDatabase*	pDb)
{
	CHECKF(pDb);

	m_idRuleGroup = idRuleGroup;

	//选择该组规则，并按照概率从低到高排序
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s where id=%u order by ruleid", _TBL_DROPITEMRULE, idRuleGroup);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);
	if(pRes)
	{
		bool ret = LoadInfo(pRes);
		pRes->Release();
		return ret;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////

void	CDropRuleGroup::Destroy()
{
	DEBUG_TRY
	for (int i=0; i<m_setRule.size(); i++)
	{
		CDropRule*	pRule	= m_setRule[i];
		if (pRule)
			pRule->Release();
	}
//	m_setRule.clear();
	DEBUG_CATCH("CDropRuleGroup::Destroy()")
}
//////////////////////////////////////////////////////////////////////

bool	CDropRuleGroup::LoadInfo(IRecordset*	pRes)
{
	CHECKF(pRes);
	DEBUG_TRY	
	for (int i=0; i<pRes->RecordCount(); i++)
	{
		CDropRule*	pRule = NULL;
		if (!pRule)
		{
			pRule = CDropRule::CreateNew();
		}

		if (!pRule)
		{
			return false;
		}
		if (!pRule->Create(pRes))
		{
			pRule->Release();
			return false;
		}

		m_setRule.push_back(pRule);
		pRes->MoveNext();
	}
	DEBUG_CATCH("CDropRuleGroup::LoadInfo()")
	return true;
}

bool	CDropRuleGroup::GetDropItem(vector<int> *pvRule)
{
	OBJID idType = ID_NONE;
	int nStep = 0;
	//DEBUG_TRY
	CHECKF(pvRule);
	int nSize = m_setRule.size();
	for (int i=0; i<nSize; i++)
	{
		nStep = 1;
		CDropRule*	pRule	= m_setRule[i];
		IF_NOT (pRule)
			continue;

		//nAmountMax 1:几率掉落  2:权重掉落  100:接上一条
		int nAmountMax = pRule->GetDropItemMaxAmount();

		if(nAmountMax >= 100)
			continue;

		nStep = 2;
		vector<int> vRuleThisDrop;
		DWORD dwValue = RandGet(1000000);
		
		DWORD dwThisCountValue = pRule->GetDropItem(&vRuleThisDrop,dwValue,nAmountMax); //为权重是返回考虑完的权重 为几率时返回1
		if(nAmountMax == 2  && dwThisCountValue >= 0 && dwThisCountValue < 1000000 )//权重下一步
		{
			nStep = 3;
			int nData = 100;
			int nNext = i;
			while(nData == 100)
			{
				nNext += 1;
				if(nNext >= nSize)
					break;
				
				nStep = 4;
				CDropRule*	pNextRule	= m_setRule[nNext];
				if(!pNextRule)
					break;
				
				nData = pNextRule->GetDropItemMaxAmount();
				if(nData != 100)
					break;
				
				dwThisCountValue = pNextRule->GetDropItem(&vRuleThisDrop,dwThisCountValue,nAmountMax);
				if(dwThisCountValue >= 1000000)
					break;
				nStep = 5;
			}
		}

		nStep = 6;
		for (int j= 0 ; j < vRuleThisDrop.size() ; j ++)
		{
			pvRule->push_back(vRuleThisDrop[j]);
		}
		vRuleThisDrop.clear();
	}
	//DEBUG_CATCH("CDropRuleGroup::GetDropItem() rule:%d,index:%u,nstep:%u",m_idRuleGroup,i,nStep);
	return true;
}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
