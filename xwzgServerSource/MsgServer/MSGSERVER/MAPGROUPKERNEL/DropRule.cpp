// DropRule.cpp: implementation of the CDropRule class.
//
//////////////////////////////////////////////////////////////////////

#include "DropRule.h"
#include "basefunc.h"


MYHEAP_IMPLEMENTATION(CDropRule,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropRule::CDropRule()
{
	m_idRule	= ID_NONE;
}

CDropRule::~CDropRule()
{
	m_setItemType.clear();
}

bool	CDropRule::Create(IRecordset*	pRes)
{
	if (!pRes)
		return false;

	IRecord* pRecord = pRes->CreateNewRecord();

	bool ret = LoadInfo(pRecord);
	SAFE_RELEASE (pRecord);
	return ret;
}

bool	CDropRule::LoadInfo(IRecord*	pRes)
{
	if (!pRes)
		return false;

	if (m_idRule == ID_NONE)
	{
		m_idRule	= pRes->LoadDWord("ruleid");
		m_id		= pRes->LoadDWord("id");
		m_nMaxItem	= pRes->LoadDWord("maxmun");
	}

	DEBUG_TRY
	for (int i=DROPRULEDATA_ITEM0; i<=DROPRULEDATA_ITEM7; i += 2)
	{
		DORPITEMDATA data;
		data.idItemType = (OBJID)pRes->GetInt(i);
		data.nChance = (OBJID)pRes->GetInt(i -1);
		if (data.idItemType != ID_NONE && data.nChance > 0)
			m_setItemType.push_back(data);
	}
	DEBUG_CATCH("CDropRule::LoadInfo()")
	return true;
}

DWORD	CDropRule::GetDropItem(vector<int> *pvRule,DWORD nValue,DWORD nAmountMax)
{
	DEBUG_TRY
	if(nAmountMax == 1)//¼¸ÂÊ
	{
		DWORD nData = 1000000;
		for(int i = 0 ; i < m_setItemType.size() ; i ++)
		{
			nData = RandGet(1000000);
			if(m_setItemType[i].nChance > nData)
			{
				pvRule->push_back(m_setItemType[i].idItemType);
			}
		}
		return 1;
	}
	else if(nAmountMax == 2)
	{
		for(int i = 0 ; i < m_setItemType.size() ; i ++)
		{
			if(m_setItemType[i].nChance > nValue)
			{
				pvRule->push_back(m_setItemType[i].idItemType);
				return 1000000;
			}
			else
			{
				nValue -= m_setItemType[i].nChance;
			}
		}
		return nValue;
	}
	
	DEBUG_CATCH("CDropRule::GetDropItem()")
	return 0;
}
