// Suit.cpp: implementation of the CSuit class.
//
//////////////////////////////////////////////////////////////////////

#include "Suit.h"
#include "../../GameBaseCodeMT/I_mydb.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MYHEAP_IMPLEMENTATION(CSuit,s_heap)
char szSuitTable[] = _TBL_SUIT;

CSuit::CSuit()
{
	m_setSuit = NULL;
}

CSuit::~CSuit()
{
	if (m_setSuit)
	{
		m_setSuit->Release();
		m_setSuit = NULL;
	}
}

bool CSuit::Create(IDatabase* pDb)
{
	IF_NOT (pDb)
	{
		return false;
	}

	m_setSuit = CSuitSet::CreateNew(true);
	IF_NOT (m_setSuit)
	{
		return false;
	}

	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s", _TBL_SUIT);
	IF_NOT_ (m_setSuit->Create(szSQL, pDb))
	{
		return false;
	}
	return true;
}

void CSuit::GetDataByInfo(SUITINFO *pInfo, SUITDATA *pData)
{
	pData->dwNum = 0;
	if (!pInfo && !pData)
	{
		return;
	}
	//寻找套装类型
	CSuitData* pSuitData = m_setSuit->GetObj(pInfo->id);
	if (!pSuitData)
	{
		return;
	}
	
	//套装数量附加
	pData->dwNum = pInfo->dwNum;
	switch (pInfo->dwNum)
	{
	case 11:
		pData->idAddction[10] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION11);
	case 10:
		pData->idAddction[9] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION10);
	case 9:
		pData->idAddction[8] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION9);
	case 8:
		pData->idAddction[7] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION8);
	case 7:
		pData->idAddction[6] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION7);
	case 6:
		pData->idAddction[5] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION6);
	case 5:
		pData->idAddction[4] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION5);
	case 4:
		pData->idAddction[3] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION4);
	case 3:
		pData->idAddction[2] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION3);
	case 2:
		pData->idAddction[1] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION2);
	case 1:
		pData->idAddction[0] = pSuitData->GetInt(ISUITDATA_COLLECT_ADDITION1);
		break;
	}
}

int CSuit::GetDataByInfo(map<int, PHASEINFO*> mapPhaseInfo, SUITDATA *pData)
{
	return 0 ;
}
