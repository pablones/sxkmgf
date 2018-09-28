// UserJinMeiSys.cpp: implementation of the CUserJinMeiSys class.
//
//////////////////////////////////////////////////////////////////////
#include "UserJinMeiSys.h"
#include "MapGroup.h"
#include "User.h"
#include "MsgUserAttrib.h"
#include "MsgJinmei.h"
#include "MsgAction.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserJinMeiSys::CUserJinMeiSys(CUser* pUser, IDatabase* pDataBase, IRecordset* pDefJinmeiRes)
{
	m_pUser = pUser;
	m_nLimitJinmeiLev = 0;
	m_nLimitJinmeiStrengthLev = 0;
	m_nCompleteNum = 0;
	m_nMaxLearnNum = 1;
	m_nMaxWaitNum = 2;
	m_bNeedUpdate = false;
	m_bFirstLogin = true;
	m_pDb = pDataBase;
	m_pDefRes = pDefJinmeiRes;
	m_nLeaveTime = 0;
}

CUserJinMeiSys::~CUserJinMeiSys()
{
	VEC_USERJINMEIDATA::iterator it = m_vecUserJinmeiData.begin();
	for (; it != m_vecUserJinmeiData.end(); ++it)
	{
		CUserJinMeiSysData* pData = (*it);
		if (NULL != pData)
		{
			pData->ReleaseByOwner();
			pData = NULL;
		}
	}

	m_vecUserJinmeiData.clear();
}

CJinmeiTypeData* CUserJinMeiSys::GetDataByTypeAndLev( int nType, int nLev )
{
	if (nType <= USERJINMEITYPE_UNKOWN || nType > USERJINMEITYPE_RENMEI)
	{
		return NULL;
	}
	
	for (int i = 0; i < JinmeiTypeSet()->GetAmount(); i++)
	{
		CJinmeiTypeData* pData = JinmeiTypeSet()->GetObjByIndex(i);
		if (NULL != pData && pData->GetInt(JINMEITYPEDATA_TYPE) == nType && pData->GetInt(JINMEITYPEDATA_LEV) == nLev)
		{
			return pData;
		}
	}
	
	return NULL;
}

CUserJinMeiSysData* CUserJinMeiSys::GetJinmeiDataByType( int nType )
{
	VEC_USERJINMEIDATA::iterator it = m_vecUserJinmeiData.begin();
	for (; it != m_vecUserJinmeiData.end(); ++it)
	{
		CUserJinMeiSysData* pData = (*it);
		if (NULL != pData && pData->GetInt(USERJINMEISYSDATA_TYPE) == nType)
		{
			return pData;				
		}
	}

	return NULL;
}

CJinmeiStrengthData* CUserJinMeiSys::GetJinmeiStrengthDataByLev(int nLev)
{
	if (nLev <= 0 || nLev > MAX_JINMEI_STRENGTHLEV)
	{
		return NULL;
	}

	int nAmount = JinmeiStrengthTypeSet()->GetAmount();
	for (int i = 0; i < nAmount; i++)
	{
		CJinmeiStrengthData* pData = JinmeiStrengthTypeSet()->GetObjByIndex(i);
		if (NULL != pData && pData->GetInt(JINMEISTRENGTHDATA_LEV) == nLev)
		{
			return pData;
		}
	}

	return NULL;
}

CJinmeiEffectData* CUserJinMeiSys::GetJinmeiEffectData( int nEffectType )
{
	int nCompareLev = 0;
	if (nEffectType == JINMEIACHIEVE_LEV)
	{
		nCompareLev = m_nLimitJinmeiLev;
	}
	else if (nEffectType == JINMEISTENGTHACHIEVE)
	{
		nCompareLev  = m_nLimitJinmeiStrengthLev;
	}

	for (int i = 0; i < JinmeiEffectTypeSet()->GetAmount(); i++)
	{
		CJinmeiEffectData* pData = JinmeiEffectTypeSet()->GetObjByIndex(i);
		if (NULL != pData && pData->GetInt(JINMEIEFFECTDATA_TYPE) == nEffectType)
		{
			int nMinLev  = pData->GetInt(JINMEIEFFECTDATA_MINLEV);
			int nMaxLev  = pData->GetInt(JINMEIEFFECTDATA_MAXLEV);
            if (nMinLev <= nCompareLev && nMaxLev >= nCompareLev)
			{
				return pData;
			}
		}
	}
	
	return NULL;
}

void CUserJinMeiSys::SaveInfo()
{
	VEC_USERJINMEIDATA::iterator it = m_vecUserJinmeiData.begin();
	for (; it != m_vecUserJinmeiData.end(); ++it)
	{
		CUserJinMeiSysData* pData = (*it);
		if (NULL != pData)
		{
			pData->Update();
		}
	}
}

void CUserJinMeiSys::SendLoginOKInfo()
{
	CHECK(m_pUser);
	VEC_JINMEITYPE::iterator it = m_vecCompleteType.begin();
	for (; it != m_vecCompleteType.end(); ++it)
	{
    	char szMsg[1024];
 		memset(szMsg, 0, sizeof(szMsg));
 		int nType = (*it);
		if (nType <= USERJINMEITYPE_UNKOWN || nType > USERJINMEITYPE_RENMEI)
		{
			continue;
		}
 		_snprintf(szMsg, sizeof(szMsg), "你的[%s]已经完成修炼，请点开人物属性界面(快捷键 C)内的经脉分页进行升级。", szJinmeiName[nType - 1]);
 		m_pUser->SendEmail(m_pUser->GetName(), szMsg, m_pUser);
	}

	m_vecCompleteType.clear();
}

void CUserJinMeiSys::SendJinmeiSysInfo(CUser* pTargetUser)
{
	CHECK(m_pUser);
	if (m_vecUserJinmeiData.empty())
	{
		if (m_bFirstLogin)
			m_bFirstLogin = false;

		return;
	}

	int nCurrTime = time(NULL);
    CMsgJinmei msg;
	bool bFirst = true;    
	VEC_USERJINMEIDATA::iterator itBegin = m_vecUserJinmeiData.begin();
	VEC_USERJINMEIDATA::iterator itEnd   = m_vecUserJinmeiData.end();
	for (; itBegin != itEnd; ++itBegin)
	{
		CUserJinMeiSysData* pSysData = (*itBegin);
		if (NULL != pSysData)
		{
			int nLev = pSysData->GetInt(USERJINMEISYSDATA_LEV);
			int nType = pSysData->GetInt(USERJINMEISYSDATA_TYPE);
			int nAddLev = pSysData->GetInt(USERJINMEISYSDATA_STENGTHLEV);
			int nStartTime = pSysData->GetInt(USERJINMEISYSDATA_STARTTIME);
			int nUseTime = pSysData->GetInt(USERJINMEISYSDATA_USETIME);
			int nStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
			int nAddQueueTime = pSysData->GetInt(USERJINMEISYSDATA_ADDQUEUETIME);
			if (nStartTime != 0 && nStatu == LEARNSTATU_PROCESS)
			{
				nUseTime += nCurrTime - nStartTime;
			}
			
			int nAddRate = 0;
			int nValue = 0;		
			int nNeedTime = 0;
			CJinmeiTypeData* pTypeData = this->GetDataByTypeAndLev(nType, nLev);
			if (NULL != pTypeData)
			{
				nValue = pTypeData->GetInt(JINMEITYPEDATA_DATA);
			}

			pTypeData = this->GetDataByTypeAndLev(nType, nLev + 1);
			if (NULL != pTypeData)
			{
				nNeedTime = pTypeData->GetInt(JINMEITYPEDATA_NEEDTIME);
			}
			nNeedTime = nNeedTime - nUseTime;
			if (nNeedTime < 0 || nStatu == LEARNSTATU_COMPELETE)
			{
				nNeedTime = 0;
			}

			CJinmeiStrengthData* pData = this->GetJinmeiStrengthDataByLev(nAddLev);
			if (NULL != pData)
			{
				nAddRate = pData->GetInt(JINMEISTRENGTHDATA_ADDRATE);
			}
			
			int nJinmeiGrade = 0;
			int nJinmeiStrengthGrade = 0;
			if (bFirst)
			{
				CJinmeiEffectData* pEffectData = this->GetJinmeiEffectData(JINMEIACHIEVE_LEV);
				if (NULL != pEffectData)
				{
					nJinmeiGrade = pEffectData->GetInt(JINMEIEFFECTDATA_GRADE);
				}
				pEffectData = this->GetJinmeiEffectData(JINMEISTENGTHACHIEVE);
				if (NULL != pEffectData)
				{
					nJinmeiStrengthGrade = pEffectData->GetInt(JINMEIEFFECTDATA_GRADE);
				}
			}

			if (bFirst)
			{
				bFirst = false;
		    	msg.Create(m_pUser->GetID(), nType, nLev, nValue, nNeedTime, nStatu, nAddQueueTime, nAddLev, nAddRate, nJinmeiGrade, nJinmeiStrengthGrade);
			}
			else
			{
				msg.Append(nType, nLev, nValue, nNeedTime, nAddLev, nAddRate, nStatu, nAddQueueTime);
			}
		}
	}
     
	CUser* pUser = NULL;
    if (NULL != pTargetUser)
	{
		pUser = pTargetUser;
	}
	else
	{
		pUser = m_pUser;
	}

	pUser->SendMsg(&msg);

	if (m_bFirstLogin)
	{
		m_bFirstLogin = false;
		CMsgAction msgAction;
		if (msgAction.Create(m_pUser->GetID(), m_nMaxLearnNum, m_nMaxWaitNum, 0, actionJinmeiWaitLearnNum))
		{
			m_pUser->SendMsg(&msgAction);
		}

		VEC_JINMEITYPE::iterator it = m_vecStartType.begin();
		for(; it != m_vecStartType.end(); ++it)
		{
			int nType = (*it);
			CUserJinMeiSysData* pData = this->GetJinmeiDataByType(nType);
			if (NULL != pData)
			{
				int nLev = pData->GetInt(USERJINMEISYSDATA_LEV);
				int nStartTime = pData->GetInt(USERJINMEISYSDATA_STARTTIME);
				int nUseTime = pData->GetInt(USERJINMEISYSDATA_USETIME);
				int nNeedTime = 0;
				CJinmeiTypeData* pTypeData = this->GetDataByTypeAndLev(nType, nLev + 1);
				if (NULL != pTypeData)
				{
					int nTime = pTypeData->GetInt(JINMEITYPEDATA_NEEDTIME);
					if (0 != nStartTime)
					{
						nUseTime += nCurrTime - nStartTime + nUseTime;
					}
					nNeedTime = nTime - nUseTime;
				}

				CMsgAction msgAction;
				if (msgAction.Create(m_pUser->GetID(), nType, nNeedTime, 0, actionJinmeiAutoLearn))
				{
					m_pUser->SendMsg(&msgAction);
				}
			}
		}
		m_vecStartType.clear();
	}
}

void CUserJinMeiSys::UpdateChange()
{
	int nStep = 0;
	DEBUG_TRY
	UpdateJinmeiLimitLev();
	nStep = 1;
	UpdateJinmeiStrengthLev();
	nStep = 2;
	int nWaitSize = m_vecWaitUserJinmeiData.size();
	if (nWaitSize > 1)
	{
		nStep = 3;
		std::sort(m_vecWaitUserJinmeiData.begin(), m_vecWaitUserJinmeiData.end(), UserJinmeiSysDataSort());
		
		if(m_bNeedUpdate)
		{
			nStep = 4;
			for (VEC_USERJINMEIDATA::iterator itWait = m_vecWaitUserJinmeiData.begin();itWait != m_vecWaitUserJinmeiData.end() ;)
			{
				if(m_nLeaveTime <= 0)
					break;
				
				nStep = 5;
				CUserJinMeiSysData* pSysData = *itWait;
				int nLearnStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
				if (nLearnStatu != LEARNSTATU_WAIT)
					break;
				nStep = 6;
				if (UPJINMEILEV_SUC == this->CheckUpJMLevForLogin(pSysData,m_nLeaveTime,2))
				{
					nStep = 7;
					pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_COMPELETE);
					int nType = pSysData->GetInt(USERJINMEISYSDATA_TYPE);
					m_vecCompleteType.push_back(nType);
					
					m_vecProcessUserJinmeiData.push_back(pSysData);
					
					// 					VEC_USERJINMEIDATA::iterator itDelete = itWait;
					// 					itWait ++;
					nStep = 8;
					m_vecWaitUserJinmeiData.erase(itWait);
				}
				else
					break;
			}
		}
	}
    
	nStep = 9;
	m_nCompleteNum = m_vecCompleteType.size();
	if (m_nCompleteNum <= 0)
	{
		return;
	}
	
	nStep = 10;
	ProcessWaitToLearn();
	DEBUG_CATCH2("UpdateChange step = %u ",nStep)
}

bool CUserJinMeiSys::AddJinmeiForChange(IRecordset* pDefaultRecord,JinmeiInfoStruct* pInfo)
{
	CHECKF(m_pUser);
    CHECKF(m_pDb);
	
	CUserJinMeiSysData* pSysData = CUserJinMeiSysData::CreateNew();
	CHECKF(pSysData);
	if (pSysData->Create(pDefaultRecord,pInfo,false))
	{
		int nType = pSysData->GetInt(USERJINMEISYSDATA_TYPE);
		int nLearnStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
		if (nLearnStatu == LEARNSTATU_PROCESS)
		{
			if (UPJINMEILEV_SUC == this->CheckUpJMLevForLogin(pSysData,m_nLeaveTime))
			{
				pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_COMPELETE);
				m_bNeedUpdate = true;
				m_vecCompleteType.push_back(nType);
			}
			
			m_vecProcessUserJinmeiData.push_back(pSysData);
		}
		else if (nLearnStatu == LEARNSTATU_WAIT)
		{
			m_vecWaitUserJinmeiData.push_back(pSysData);
		}
		else if (nLearnStatu == LEARNSTATU_COMPELETE)
		{
			m_vecCompleteType.push_back(nType);
			m_vecProcessUserJinmeiData.push_back(pSysData);
		}
		
		m_vecUserJinmeiData.push_back(pSysData);
		return true;
	}
	else
		pSysData->ReleaseByOwner();
	return false;
}

void CUserJinMeiSys::OnLogin()
{
    DEBUG_TRY 
    CHECK(m_pUser);
    CHECK(m_pDb);

	m_nLeaveTime = 0;
    SQLBUF szSQL;
	_snprintf(szSQL, sizeof(szSQL), "SELECT * FROM %s WHERE idUser = %d LIMIT %d", _TBL_JINMEIUSER, m_pUser->GetID(), MAX_JINMEI_NUM);
	IRecordset* pRes = m_pDb->CreateNewRecordset(szSQL);
	if (NULL != pRes)
	{
     	for (int i = 0; i < pRes->RecordCount(); ++i)
		{
			CUserJinMeiSysData* pSysData = CUserJinMeiSysData::CreateNew();
			if (NULL != pSysData && pSysData->Create(pRes))
			{
				int nType = pSysData->GetInt(USERJINMEISYSDATA_TYPE);
				int nLearnStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
				if (nLearnStatu == LEARNSTATU_PROCESS)
				{
					if (UPJINMEILEV_SUC == this->CheckUpJMLevForLogin(pSysData,m_nLeaveTime))
					{
						pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_COMPELETE);
						m_bNeedUpdate = true;
						m_vecCompleteType.push_back(nType);
					}
					
					m_vecProcessUserJinmeiData.push_back(pSysData);
				}
				else if (nLearnStatu == LEARNSTATU_WAIT)
				{
					m_vecWaitUserJinmeiData.push_back(pSysData);
				}
				else if (nLearnStatu == LEARNSTATU_COMPELETE)
				{
					m_vecCompleteType.push_back(nType);
					m_vecProcessUserJinmeiData.push_back(pSysData);
				}

				m_vecUserJinmeiData.push_back(pSysData);
			}
			else
			{
				if (NULL != pSysData)
				{
					pSysData->ReleaseByOwner();
				}
			}

			pRes->MoveNext();
		}

		pRes->Release();
	}

	UpdateJinmeiLimitLev();
	UpdateJinmeiStrengthLev();

	int nWaitSize = m_vecWaitUserJinmeiData.size();
	if (nWaitSize > 1)
	{
		std::sort(m_vecWaitUserJinmeiData.begin(), m_vecWaitUserJinmeiData.end(), UserJinmeiSysDataSort());

		if(m_bNeedUpdate)
		{
			VEC_USERJINMEIDATA::iterator itWait;
			for ( itWait = m_vecWaitUserJinmeiData.begin();itWait != m_vecWaitUserJinmeiData.end() ;)
			{
				if(m_nLeaveTime <= 0)
					break;

				CUserJinMeiSysData* pSysData = *itWait;
				int nLearnStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
				if (nLearnStatu != LEARNSTATU_WAIT)
					break;
				if (UPJINMEILEV_SUC == this->CheckUpJMLevForLogin(pSysData,m_nLeaveTime,2))
				{
					pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_COMPELETE);
					int nType = pSysData->GetInt(USERJINMEISYSDATA_TYPE);
					m_vecCompleteType.push_back(nType);

					m_vecProcessUserJinmeiData.push_back(pSysData);

// 					VEC_USERJINMEIDATA::iterator itDelete = itWait;
// 					itWait ++;
					m_vecWaitUserJinmeiData.erase(itWait);
				}
				else
					break;
			}
		}
	}
    
	m_nCompleteNum = m_vecCompleteType.size();
	if (m_nCompleteNum <= 0)
	{
		return;
	}

	ProcessWaitToLearn();
	
	DEBUG_CATCH("catch at CUserJinMeiSys::OnLogin()")
}

void CUserJinMeiSys::ProcessWaitToLearn()
{
	VEC_JINMEITYPE vecStartType;
	int nCanLearNum = m_nMaxLearnNum - m_vecProcessUserJinmeiData.size() + m_nCompleteNum;
	VEC_USERJINMEIDATA::iterator itor = m_vecWaitUserJinmeiData.begin();
	for (; itor != m_vecWaitUserJinmeiData.end() && nCanLearNum > 0; ++itor, --nCanLearNum )
	{
		CUserJinMeiSysData* pData = (*itor);
		if (NULL != pData)
		{
			int nType = pData->GetInt(USERJINMEISYSDATA_TYPE);
			int nNeedTime = 0;
			int nAddTime = 0;
			bool bRet = this->StartLearn(nType, nNeedTime, nAddTime);
			if (bRet)
			{
				vecStartType.push_back(nType);
				if (m_bFirstLogin)
				{
					m_vecStartType.push_back(nType);
				}
				else
				{
					CMsgAction msg;
					if (msg.Create(m_pUser->GetID(), nType, 0, 0, actionJinmeiAutoLearn, nNeedTime, nAddTime, false))
					{
						m_pUser->SendMsg(&msg);
					}
				}
			}
		}
	}
	
	VEC_JINMEITYPE::iterator itStart = vecStartType.begin();
	for (; itStart != vecStartType.end(); ++itStart)
	{
		int nType = (*itStart);
        VEC_USERJINMEIDATA::iterator itWait = m_vecWaitUserJinmeiData.begin();
		for (; itWait != m_vecWaitUserJinmeiData.end(); ++itWait)
		{
			CUserJinMeiSysData* pTmpData = (*itWait);
			if (NULL != pTmpData && pTmpData->GetInt(USERJINMEISYSDATA_TYPE) == nType)
			{
				m_vecWaitUserJinmeiData.erase(itWait);
				break;
			}
		}
	}
}

void CUserJinMeiSys::OnLogOut()
{
	DEBUG_TRY		
	this->SaveInfo();		
	DEBUG_CATCH("catch at CUserJinMeiSys::OnLogOut()")
}

int CUserJinMeiSys::CheckUpJMLevForLogin( CUserJinMeiSysData* pSysData, int& nLeaveTime ,int nType/* = 1*/)
{
	DEBUG_TRY
	CHECKF(pSysData);
	CHECKF(m_pUser);	
	
    int nCurrType = pSysData->GetInt(USERJINMEISYSDATA_TYPE);
	int nStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
	if (nCurrType <= USERJINMEITYPE_UNKOWN || nCurrType > USERJINMEITYPE_RENMEI)
	{
		return UPJINMEILEV_FAIL;
	}

	
	int nLev = pSysData->GetInt(USERJINMEISYSDATA_LEV);
    if (nLev + 1 > MAX_JINMEI_LEV)
	{
		return UPJINMEILEV_FAIL;
	}

	if(nType == 1)
	{
		if (nStatu != LEARNSTATU_PROCESS)
			return UPJINMEILEV_FAIL;

		int nCurrTime = time(NULL);
		int nUseTime = pSysData->GetInt(USERJINMEISYSDATA_USETIME);
		int nStartTime = pSysData->GetInt(USERJINMEISYSDATA_STARTTIME);
		int nLearnTime = nCurrTime - nStartTime + nUseTime;
		CJinmeiTypeData* pData = this->GetDataByTypeAndLev(nCurrType, nLev + 1);
		if (NULL != pData)
		{
			int nNeedTime = pData->GetInt(JINMEITYPEDATA_NEEDTIME);
			if (nNeedTime <= nLearnTime)
			{
				nLeaveTime = nLearnTime - nNeedTime;
				return UPJINMEILEV_SUC;
			}
		}
	}
	else if(nType == 2)
	{
		CHECKF(nLeaveTime > 0);
		int nUseTime = pSysData->GetInt(USERJINMEISYSDATA_USETIME);
		int nLearnTime = nLeaveTime + nUseTime;
		CJinmeiTypeData* pData = this->GetDataByTypeAndLev(nCurrType, nLev + 1);
		int nNeedTime = pData->GetInt(JINMEITYPEDATA_NEEDTIME);
		
		if (NULL != pData)
		{
			if (nNeedTime <= nLearnTime)
			{
				nLeaveTime = nLearnTime - nNeedTime;
				return UPJINMEILEV_SUC;
			}
			else
			{
				pSysData->SetInt(USERJINMEISYSDATA_USETIME,nLearnTime);
			}
		}
	}
	
	return UPJINMEILEV_FAIL;
    DEBUG_CATCH("catch at bool CUserJinMeiSys::CheckUpJMLevForLogin()")
	return UPJINMEILEV_FAIL;
}

int CUserJinMeiSys::CheckUpJinmeiLevConditon( CUserJinMeiSysData* pSysData, int nUpLevType )
{
	DEBUG_TRY
    CHECKF(pSysData);
	CHECKF(m_pUser);

    int nCurrType = pSysData->GetInt(USERJINMEISYSDATA_TYPE);
	int nStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
	if (nCurrType <= USERJINMEITYPE_UNKOWN || nCurrType > USERJINMEITYPE_RENMEI)
	{
		return UPJINMEILEV_FAIL;
	}

	if (nStatu == LEARNSTATU_WAIT || nStatu == LEARNSTATU_STOP)
	{
		return UPJINMEILEV_FAIL;
	}

	int nLev = pSysData->GetInt(USERJINMEISYSDATA_LEV);
    if (nLev + 1 > MAX_JINMEI_LEV)
	{
		return UPJINMEILEV_FAIL;
	}

	int nCurrTime = time(NULL);
	int nUseTime = pSysData->GetInt(USERJINMEISYSDATA_USETIME);
	int nStartTime = pSysData->GetInt(USERJINMEISYSDATA_STARTTIME);
	int nLearnTime = nCurrTime - nStartTime + nUseTime;
	CJinmeiTypeData* pData = this->GetDataByTypeAndLev(nCurrType, nLev + 1);

	if (NULL != pData)
	{
		int nNeedTime = pData->GetInt(JINMEITYPEDATA_NEEDTIME);
		if (UPJINMEILEV_TIME == nUpLevType)
		{
			if (nStartTime != 0)
			{
				if (nNeedTime <= nLearnTime)
				{
					return UPJINMEILEV_SUC;
				}
			}
		}
		else if (UPJINMEILEV_EMONEY == nUpLevType)
		{
			nNeedTime = nNeedTime - nLearnTime;
			int nJinmeiValue= m_pUser->GetJinmeiValue();
			int nSec = nNeedTime / 60;
			if (nNeedTime % 60 != 0)
			{
				nSec++;
			}
			
			int nNeedValue = nSec * 2;
			//经脉部分加速 一分钟需要两点经脉值
			if (nJinmeiValue < nNeedValue)
			{
				nUseTime += nJinmeiValue * 30;
				if (m_pUser->SpendJinmeiValue(nJinmeiValue))
				{
					pSysData->SetInt(USERJINMEISYSDATA_USETIME, nUseTime);
					m_bNeedUpdate = true;
					return UPJINMEILEV_PART;
				}
				
				return UPJINMEILEV_FAIL;
			}

			if (m_pUser->SpendJinmeiValue(nNeedValue))
			{
				return UPJINMEILEV_SUC;
			}
		}
	}

	return UPJINMEILEV_FAIL;
    DEBUG_CATCH("catch at bool CUserJinMeiSys::CheckUpJinmeiLevConditon()")
	return UPJINMEILEV_FAIL;
}

void CUserJinMeiSys::OnTimer(time_t tCurr)
{
	DEBUG_TRY
    CHECK(m_pUser);
	std::vector<int> vecCompType;
	VEC_USERJINMEIDATA::iterator it = m_vecProcessUserJinmeiData.begin();
	for (; it != m_vecProcessUserJinmeiData.end(); ++it)
	{
		CUserJinMeiSysData* pSysData = (*it);
		if (NULL != pSysData)
		{
			int nStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
			int nType= pSysData->GetInt(USERJINMEISYSDATA_TYPE);
			if (nStatu == LEARNSTATU_PROCESS)
			{
				int nRet = this->CheckUpJinmeiLevConditon(pSysData);
				if (UPJINMEILEV_SUC == nRet)
				{
					pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_COMPELETE);
					vecCompType.push_back(nType);
					CMsgAction msg;
					if (msg.Create(m_pUser->GetID(), nType, 0, 0, actionJinmeiNeedUpLev))
					{
						m_pUser->SendMsg(&msg);
					}
					m_bNeedUpdate = true;
					m_nCompleteNum++;
				}
			}
			else if (nStatu == LEARNSTATU_COMPELETE)
			{
				vecCompType.push_back(nType);
			}
		}
	}

	if (m_bNeedUpdate)
	{
		this->SaveInfo();
		m_bNeedUpdate = false;
	}

	if (m_nCompleteNum <= 0)
	{
		return;
	}

    this->ProcessWaitToLearn();
	
	DEBUG_CATCH("catch at CUserJinMeiSys::OnTimer()")
}

bool CUserJinMeiSys::UpJinmeiLev( CUserJinMeiSysData* pSysData )
{
	DEBUG_TRY
    CHECKF(pSysData);
	CHECKF(m_pUser);
    int nType = pSysData->GetInt(USERJINMEISYSDATA_TYPE);
	int nCurrLev = pSysData->GetInt(USERJINMEISYSDATA_LEV);
	int nData = 0;
	int nNeedTime = 0;

	//修炼完当前级别给予玩家的经验奖励
    CJinmeiTypeData* pTypeData = this->GetDataByTypeAndLev(nType, nCurrLev + 1);
	if (NULL != pTypeData)
	{
		int nExp = pTypeData->GetInt(JINMEITYPEDATA_REQEXP);
		m_pUser->SetAttrib(_USERATTRIB_EXP, m_pUser->GetExp() + nExp, SYNCHRO_TRUE);
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW, "你获得了%u经验", nExp);
	}

	pSysData->SetInt(USERJINMEISYSDATA_LEV, nCurrLev + 1);
	pSysData->SetInt(USERJINMEISYSDATA_USETIME, 0);
	pSysData->SetInt(USERJINMEISYSDATA_STARTTIME, 0);
	pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_STOP);
	pSysData->SetInt(USERJINMEISYSDATA_ADDQUEUETIME, 0);   //完成后 直接移出队列
    m_bNeedUpdate = true;
	
	VEC_USERJINMEIDATA::iterator it = m_vecProcessUserJinmeiData.begin();
	for (; it != m_vecProcessUserJinmeiData.end(); ++it)
	{
		CUserJinMeiSysData* pData = (*it);
		if (NULL != pData && pData->GetInt(USERJINMEISYSDATA_TYPE) == nType )
		{
			m_vecProcessUserJinmeiData.erase(it);
			break;
		}
	}

	int nOldLev = m_nLimitJinmeiLev;
	UpdateJinmeiLimitLev();
	if((m_nLimitJinmeiLev > nOldLev) && ((m_nLimitJinmeiLev % 5) == 0))
		m_pUser->JinmeiBroadCast(1,0,m_nLimitJinmeiLev);

	CMsgAction msgAction;
	if (msgAction.Create(m_pUser->GetID(), m_nMaxLearnNum, m_nMaxWaitNum, 0, actionJinmeiWaitLearnNum))
	{
		m_pUser->SendMsg(&msgAction);
	}

//    MyLogSave("syslog/jinmeisys", "玩家(%d)经脉(%d)修炼等级(%d)成功", m_pUser->GetID(), pSysData->GetInt(USERJINMEISYSDATA_TYPE), nCurrLev + 1);
	return true;

	DEBUG_CATCH("catch at CUserJinMeiSys::UpJinmeiLev(1)")

	return false;
}

bool CUserJinMeiSys::UpJinmeiLev( int nUpLevType, int nType )
{
	DEBUG_TRY
	CUserJinMeiSysData* pSysData = this->GetJinmeiDataByType(nType);
	if (NULL == pSysData || NULL == m_pUser)
	{
		return false;
	}
    
	int nRet = UPJINMEILEV_FAIL;
	if (nUpLevType == UPJINMEILEV_EMONEY)
	{
		nRet = this->CheckUpJinmeiLevConditon(pSysData, UPJINMEILEV_EMONEY);
		if (UPJINMEILEV_SUC == nRet)
		{
			this->UpJinmeiLev(pSysData);			
			int nWaitSize = m_vecWaitUserJinmeiData.size();
			int nCanLearNum = m_nMaxLearnNum - m_vecProcessUserJinmeiData.size() + m_nCompleteNum;
			if (nCanLearNum > 0 && nWaitSize > 0)
			{
				VEC_USERJINMEIDATA::iterator itor = m_vecWaitUserJinmeiData.begin();
				CUserJinMeiSysData* pData = (*itor);
				if (NULL != pData)
				{
					int nType = pData->GetInt(USERJINMEISYSDATA_TYPE);
					int nNeedTime = 0;
					int nAddTime = 0;
					if (this->StartLearn(nType, nNeedTime, nAddTime))
					{
						m_vecWaitUserJinmeiData.erase(itor);
						CMsgAction msg;
						if (msg.Create(m_pUser->GetID(), nType, 0, 0, actionJinmeiAutoLearn, nNeedTime, nAddTime, false))
						{
							m_pUser->SendMsg(&msg);
						}
					}
				}	
			}
		}
	}
	else
	{
		if (pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE) == LEARNSTATU_COMPELETE)
		{
			nRet = UPJINMEILEV_SUC;
		}
		else
		{
			nRet = this->CheckUpJinmeiLevConditon(pSysData);
		}

		if (UPJINMEILEV_SUC == nRet)
		{
			this->UpJinmeiLev(pSysData);
			m_nCompleteNum--;
		}
	}
    
	bool bRet = false;
	int nUpResult = UPLEV_FAIL;	
	int nLeaveTime= 0;
	if (nRet == UPJINMEILEV_SUC)
	{
		nUpResult = UPLEV_SUC;
		bRet = true;
	}
    else if (nRet == UPJINMEILEV_PART)
	{
		int nUseTime = pSysData->GetInt(USERJINMEISYSDATA_USETIME);
		int nLev     = pSysData->GetInt(USERJINMEISYSDATA_LEV);
        CJinmeiTypeData* pTypeData = this->GetDataByTypeAndLev(nType, nLev + 1);
		if (NULL != pTypeData)
		{
			int nCurrTime = time(NULL);
			int nStartTime = pSysData->GetInt(USERJINMEISYSDATA_STARTTIME);
			int nLearnTime = nCurrTime - nStartTime + nUseTime;

			nLeaveTime = pTypeData->GetInt(JINMEITYPEDATA_NEEDTIME) - nLearnTime;
		}
		nUpResult = UPLEV_PART;
		bRet = true;
	}

	CMsgAction msg;
	if (msg.Create(m_pUser->GetID(), nType, 0, nUpResult, actionJinmeiOpratRet, nLeaveTime))
	{
		m_pUser->SendMsg(&msg);
	}
	return bRet;
	DEBUG_CATCH("catch at CUserJinMeiSys::UpJinmeiLev(2)")
	return false;
}

bool CUserJinMeiSys::DelJinmeiType( int nType )
{
	DEBUG_TRY		
		
	return false;
	DEBUG_CATCH("catch at CUserJinMeiSys::OnTimer()")
}

void CUserJinMeiSys::StartLearnJinmei(int nType)
{
    CHECK(m_pUser);
	int nLearnNum = m_vecProcessUserJinmeiData.size() - m_nCompleteNum;
	if (nLearnNum >= m_nMaxLearnNum)
	{
		this->AddJinmeiWaitQueue(nType);
		return;
	}

	int nResult = STARTLEARN_FAIL;
	int nNeedTime = 0;
	int nAddTime = 0;
	bool bRet = this->StartLearn(nType, nNeedTime, nAddTime);
	if (bRet)
	{
		nResult = STARTLEARN_SUC;
	}

	CMsgAction msg;
	if (msg.Create(m_pUser->GetID(), nType, 0, nResult, actionJinmeiOpratRet, nNeedTime, nAddTime, false))
	{
		m_pUser->SendMsg(&msg);
	}
}

bool CUserJinMeiSys::StartLearn(int nType, int& nNeedTime, int& nAddTime)
{
	DEBUG_TRY
	if (nType <= USERJINMEITYPE_UNKOWN || nType > USERJINMEITYPE_RENMEI)
	{
		return false;
	}
    
	nNeedTime = 0;
	int nLearnNum = m_vecProcessUserJinmeiData.size() - m_nCompleteNum;
	if (nLearnNum >= m_nMaxLearnNum)
	{
		return false;
	}

	int nCurrTime = time(NULL);	
	CUserJinMeiSysData* pSysData = this->GetJinmeiDataByType(nType);
    if (NULL != pSysData)
	{
		int nStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
		if (nStatu == LEARNSTATU_PROCESS || nStatu == LEARNSTATU_COMPELETE)
		{
			return false;
		}

		int nUseTime = pSysData->GetInt(USERJINMEISYSDATA_USETIME);
		if (0 == nUseTime && nStatu == LEARNSTATU_STOP)
		{
			if (!CheckLearnCondition(pSysData))
			{
				return false;
			}
		}
		
		if (nStatu != LEARNSTATU_WAIT)
		{
			pSysData->SetInt(USERJINMEISYSDATA_ADDQUEUETIME, nCurrTime);
		}

		pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_PROCESS);
		pSysData->SetInt(USERJINMEISYSDATA_STARTTIME, nCurrTime);
		int nLev = pSysData->GetInt(USERJINMEISYSDATA_LEV);
		CJinmeiTypeData* pData = this->GetDataByTypeAndLev(nType, nLev + 1);
		if (NULL != pData)
		{
			int nTime = pData->GetInt(JINMEITYPEDATA_NEEDTIME);
			nNeedTime = nTime - nUseTime;
		}
        
		nAddTime = nCurrTime;
		m_bNeedUpdate = true;
		m_vecProcessUserJinmeiData.push_back(pSysData);
	}
	else
	{
        if (!CheckLearnNewConditon(nType))
		{
			return false;
		}

		pSysData = CUserJinMeiSysData::CreateNew();
		if (pSysData && pSysData->Create(m_pDefRes))
		{
			pSysData->SetInt(USERJINMEISYSDATA_TYPE, nType);
			pSysData->SetInt(USERJINMEISYSDATA_STARTTIME, nCurrTime);
			pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_PROCESS);
			pSysData->SetInt(USERJINMEISYSDATA_IDUSER, m_pUser->GetID());
			pSysData->SetInt(USERJINMEISYSDATA_ADDQUEUETIME, nCurrTime);
			m_vecUserJinmeiData.push_back(pSysData);
			pSysData->InsertRecord();
			CJinmeiTypeData* pData = this->GetDataByTypeAndLev(nType, 1);
			if (NULL != pData)
			{
				nNeedTime = pData->GetInt(JINMEITYPEDATA_NEEDTIME);
			}

			nAddTime = nCurrTime;
			m_vecProcessUserJinmeiData.push_back(pSysData);
		}
	}

//	MyLogSave("syslog/jinmeisys", "玩家(%d)经脉(%d)修炼开始", m_pUser->GetID(), pSysData->GetInt(USERJINMEISYSDATA_TYPE));

	return true;
	DEBUG_CATCH2("catch at CUserJinMeiSys::StartLearn(%d)", nType)
	return false;
}

void CUserJinMeiSys::AddJinmeiWaitQueue(int nType)
{
	if (NULL == m_pUser)
	{
		return;
	}
    
	int nAddTime = 0;
	int nNeedTime = 0;
	int nResult = ADDWAITQUEUE_FAIL;
	bool bRet = this->AddWaitLearnQueue(nType, nAddTime, nNeedTime);
	if (bRet)
	{
		nResult = ADDWAITQUEUE_SUC;
	}

	CMsgAction msg;
	if (msg.Create(m_pUser->GetID(), nType, 0, nResult, actionJinmeiOpratRet, nNeedTime, nAddTime, false))
	{
		m_pUser->SendMsg(&msg);
	}
}

bool CUserJinMeiSys::AddWaitLearnQueue(int nType, int& nAddTime, int& nNeedTime)
{
	DEBUG_TRY
	if (nType <= USERJINMEITYPE_UNKOWN || nType > USERJINMEITYPE_RENMEI)
	{
		return false;
	}
		
	int nWaitNum = m_vecWaitUserJinmeiData.size();
	if (nWaitNum >= m_nMaxWaitNum)
	{
		return false;
	}

	int nLearnNum = m_vecProcessUserJinmeiData.size();
	if (nLearnNum + nWaitNum >= m_nMaxWaitNum + m_nMaxLearnNum)
	{
		return false;
	}

	int nTime = time(NULL);
	CUserJinMeiSysData* pSysData = this->GetJinmeiDataByType(nType);
    if (NULL != pSysData)
	{
		int nStatu = pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE);
		if (nStatu != LEARNSTATU_STOP)
		{
			return false;
		}

		int nUseTime = pSysData->GetInt(USERJINMEISYSDATA_USETIME);
		if (0 == nUseTime && nStatu == LEARNSTATU_STOP)
		{
			if (!CheckLearnCondition(pSysData))
			{
				return false;
			}
		}

		int nLev = pSysData->GetInt(USERJINMEISYSDATA_LEV);
		CJinmeiTypeData* pTypeData = this->GetDataByTypeAndLev(nType, nLev + 1);
		if (NULL != pTypeData)
		{
			nNeedTime = pTypeData->GetInt(JINMEITYPEDATA_NEEDTIME);
			nNeedTime = nNeedTime - nUseTime;
		}
		
		nAddTime = nTime;
		pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_WAIT);
		pSysData->SetInt(USERJINMEISYSDATA_ADDQUEUETIME, nTime);
		m_bNeedUpdate = true;
		m_vecWaitUserJinmeiData.push_back(pSysData);
	}
	else
	{
        if (!CheckLearnNewConditon(nType))
		{
			return false;
		}
		
		pSysData = CUserJinMeiSysData::CreateNew();
		if (pSysData && pSysData->Create(m_pDefRes))
		{
			nAddTime = nTime;
			pSysData->SetInt(USERJINMEISYSDATA_TYPE, nType);
			pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_WAIT);
			pSysData->SetInt(USERJINMEISYSDATA_IDUSER, m_pUser->GetID());
			pSysData->SetInt(USERJINMEISYSDATA_ADDQUEUETIME, nTime);
			CJinmeiTypeData* pTypeData = this->GetDataByTypeAndLev(nType, 1);
			if (NULL != pTypeData)
			{
				nNeedTime = pTypeData->GetInt(JINMEITYPEDATA_NEEDTIME);
			}

			m_vecUserJinmeiData.push_back(pSysData);
			pSysData->InsertRecord();
			m_vecWaitUserJinmeiData.push_back(pSysData);
		}
	}
	return true;
	DEBUG_CATCH("catch at void CUserJinMeiSys::AddWaitLearnQueue")
	return false;
}

void CUserJinMeiSys::MoveWaitQueue(int nType)
{
	DEBUG_TRY
	if (nType <= USERJINMEITYPE_UNKOWN || nType > USERJINMEITYPE_RENMEI)
	{
		return;
	}

	bool bRet = false;
	CUserJinMeiSysData* pSysData = this->GetJinmeiDataByType(nType);
    if (NULL != pSysData && pSysData->GetInt(USERJINMEISYSDATA_LEARNSTATE) == LEARNSTATU_WAIT)
	{
		pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_STOP);
		pSysData->SetInt(USERJINMEISYSDATA_ADDQUEUETIME, 0);
		int nUseTime = pSysData->GetInt(USERJINMEISYSDATA_USETIME);
		if (0 == nUseTime)
		{
			pSysData->SetInt(USERJINMEISYSDATA_USETIME, 1);
		}

		m_bNeedUpdate = true;
		bRet = true;

		VEC_USERJINMEIDATA::iterator it = m_vecWaitUserJinmeiData.begin();
		for (; it != m_vecProcessUserJinmeiData.end(); ++it)
		{
			CUserJinMeiSysData* pData = (*it);
			if (NULL != pData && pData->GetInt(USERJINMEISYSDATA_TYPE) == nType )
			{
				m_vecWaitUserJinmeiData.erase(it);
				break;
			}
		}
	}

	int nResult = MOVEWAITQUEUE_FAIL;
	if (bRet)
	{
		nResult = MOVEWAITQUEUE_SUC;
	}
	
	CMsgAction msg;
	if (msg.Create(m_pUser->GetID(), nType, 0, nResult, actionJinmeiOpratRet))
	{
		m_pUser->SendMsg(&msg);
	}

	DEBUG_CATCH("catch at void CUserJinMeiSys::MoveWaitQueue")
}

bool CUserJinMeiSys::CheckLearnCondition(CUserJinMeiSysData* pSysData)
{
    CHECKF(pSysData);
	CHECKF(m_pUser);
	int nCurrType = pSysData->GetInt(USERJINMEISYSDATA_TYPE);
	if (nCurrType <= USERJINMEITYPE_UNKOWN || nCurrType > USERJINMEITYPE_RENMEI)
	{
		return false;
	}
	
	int nLev = pSysData->GetInt(USERJINMEISYSDATA_LEV);
    if (nLev + 1 > MAX_JINMEI_LEV)
	{
		return false;
	}

	CJinmeiTypeData* pData = this->GetDataByTypeAndLev(nCurrType, nLev + 1);
	int nReqLearnLev = pData->GetInt(JINMEITYPEDATA_REQMAGICLEV);
    if (m_nLimitJinmeiLev < nReqLearnLev)
	{
		return false;
	}

	int nNeedMoney = pData->GetInt(JINMEITYPEDATA_REQMONEY);
	int nReqLev    = pData->GetInt(JINMEITYPEDATA_REQLEV);
	
	if (m_pUser->GetLev() < nReqLev)
	{
		return false;
	}
	
	if (m_pUser->GetMoney() < nNeedMoney || !m_pUser->SpendMoney(nNeedMoney, SYNCHRO_TRUE))
	{
		return false;
	}
	
	return true;
}

bool CUserJinMeiSys::CheckLearnNewConditon(int nType)
{
	CHECKF(m_pUser);
	CJinmeiTypeData* pData = this->GetDataByTypeAndLev(nType,  1);
	if (NULL == pData)
	{
		return false;
	}
	
	int nNeedMoney = pData->GetInt(JINMEITYPEDATA_REQMONEY);
	int nReqLev    = pData->GetInt(JINMEITYPEDATA_REQLEV);
	
	if (m_pUser->GetLev() < nReqLev)
	{
		return false;
	}
	
	if (m_pUser->GetMoney() < nNeedMoney || !m_pUser->SpendMoney(nNeedMoney, SYNCHRO_TRUE))
	{
		return false;
	}

	return true;
}

void CUserJinMeiSys::StopLearn( int nType )
{
	DEBUG_TRY
	if (nType <= USERJINMEITYPE_UNKOWN || nType > USERJINMEITYPE_RENMEI)
	{
		return;
	}	
	
	CUserJinMeiSysData* pSysData = this->GetJinmeiDataByType(nType);
	if (NULL != pSysData)
	{
		int nTime = time(NULL);
		int nStartTime = pSysData->GetInt(USERJINMEISYSDATA_STARTTIME);
		int nUseTime = pSysData->GetInt(USERJINMEISYSDATA_USETIME);
		if (nStartTime != 0)
		{
			pSysData->SetInt(USERJINMEISYSDATA_USETIME, nUseTime + nTime - nStartTime);
		}

		pSysData->SetInt(USERJINMEISYSDATA_STARTTIME, 0);
		pSysData->SetInt(USERJINMEISYSDATA_LEARNSTATE, LEARNSTATU_STOP);
		pSysData->SetInt(USERJINMEISYSDATA_ADDQUEUETIME, 0);
		m_bNeedUpdate = true;

		VEC_USERJINMEIDATA::iterator it = m_vecProcessUserJinmeiData.begin();
		for (; it != m_vecProcessUserJinmeiData.end(); ++it)
		{
			CUserJinMeiSysData* pData = (*it);
			if (NULL != pData && pData->GetInt(USERJINMEISYSDATA_TYPE) == nType )
			{
				m_vecProcessUserJinmeiData.erase(it);
				break;
			}
		}
	}

	DEBUG_CATCH2("catch at CUserJinMeiSys::StopLearn(%d)", nType)
}

int CUserJinMeiSys::GetJinmeiTypeValue( int nType )
{
	int nData = 0;
	CUserJinMeiSysData* pSysData = this->GetJinmeiDataByType(nType);
	if (NULL != pSysData)
	{
		int nLev = pSysData->GetInt(USERJINMEISYSDATA_LEV);
        CJinmeiTypeData* pData = this->GetDataByTypeAndLev(nType, nLev);
		if (NULL != pData)
		{
			nData += pData->GetInt(JINMEITYPEDATA_DATA);
		}
        
		int nStrLev = pSysData->GetInt(USERJINMEISYSDATA_STENGTHLEV);
		CJinmeiStrengthData* pStengData = this->GetJinmeiStrengthDataByLev(nStrLev);
		if (NULL != pStengData)
		{
			int nAddRate = pStengData->GetInt(JINMEISTRENGTHDATA_ADDRATE);
			nData += pData->GetInt(JINMEITYPEDATA_DATA) * nAddRate / 100;
		}

		CJinmeiEffectData* pEffectData = this->GetJinmeiEffectData(JINMEIACHIEVE_LEV);
		if (NULL != pEffectData)
		{
			nData += pEffectData->GetInt(JINMEIEFFECTDATA_EFFECT1);
		}

		pEffectData = this->GetJinmeiEffectData(JINMEISTENGTHACHIEVE);
		if (NULL != pEffectData)
		{
			int nRate = pEffectData->GetInt(JINMEIEFFECTDATA_EFFECT1);
			nData += pData->GetInt(JINMEITYPEDATA_DATA) * nRate / 1000;
		}
	}    

	return nData;
}

int CUserJinMeiSys::GetJinmeiDecManaRate()
{
	CJinmeiEffectData* pEffectData = this->GetJinmeiEffectData(JINMEIACHIEVE_LEV);
	if (NULL != pEffectData)
	{
		return pEffectData->GetInt(JINMEIEFFECTDATA_EFFECT2);
	}
	
	return 0;
}

int CUserJinMeiSys::GetJinmeiDecDamageRate()
{
	CJinmeiEffectData* pEffectData = this->GetJinmeiEffectData(JINMEIACHIEVE_LEV);
	if (NULL != pEffectData)
	{
		return pEffectData->GetInt(JINMEIEFFECTDATA_EFFECT3);
	}
	
	return 0;
}

int CUserJinMeiSys::GetJinmeiIncDamageRate()
{
	CJinmeiEffectData* pEffectData = this->GetJinmeiEffectData(JINMEIACHIEVE_LEV);
	if (NULL != pEffectData)
	{
		return pEffectData->GetInt(JINMEIEFFECTDATA_EFFECT4);
	}

	return 0;
}

bool CUserJinMeiSys::StrengthUserJinmei(int nType, bool bProtect)
{
	if (NULL == m_pUser)
	{
		return false;
	}

	int nResult = STRENGTHJINMEI_FAIL;
	int nFailLev = 0;
	bool bRet = this->StrengthJinmei(nType, bProtect, nFailLev);
	if (bRet)
	{
		nResult = STRENGTHJINMEI_SUC;
	}

	CMsgAction msg;
	if (msg.Create(m_pUser->GetID(), nType, nFailLev, nResult, actionJinmeiOpratRet))
	{
		m_pUser->SendMsg(&msg);
	}

	return bRet;
}

bool CUserJinMeiSys::StrengthJinmei( int nType, bool bProtect, int& nFailStrengLev)
{
	DEBUG_TRY
	CUserJinMeiSysData* pSysData = this->GetJinmeiDataByType(nType);
	if (NULL == pSysData || NULL == m_pUser)
	{
		return false;
	}
    
	int nCurLev = pSysData->GetInt(USERJINMEISYSDATA_STENGTHLEV);
	nFailStrengLev = nCurLev;
	if (nCurLev + 1 > MAX_JINMEI_STRENGTHLEV)
	{
		return false;
	}

	CJinmeiStrengthData* pData = this->GetJinmeiStrengthDataByLev(nCurLev + 1);
    if (NULL == pData)
	{
		return false;
	}

    int nRate = pData->GetInt(JINMEISTRENGTHDATA_SUCRATE);
	int nProcItemType = pData->GetInt(JINMEISTRENGTHDATA_PROTECITEMTYPE);
	int nReqMoney = pData->GetInt(JINMEISTRENGTHDATA_REQMONEY);
	int nItemType = pData->GetInt(JINMEISTRENGTHDATA_ITEMTYPE);
    
	CItem* pProtectItem = NULL;
	if (bProtect)
	{
		pProtectItem = m_pUser->GetItemByType(nProcItemType);
		if (!pProtectItem)
			return false;
	}

	CItem* pStrengthItem = m_pUser->GetItemByType(nItemType);
    if (!pStrengthItem)
	{
		return false;
	}

	if (m_pUser->GetMoney() < nReqMoney || !m_pUser->SpendMoney(nReqMoney, SYNCHRO_TRUE))
	{
		return false;
	}
	
	if (bProtect)
	{
		if (!m_pUser->SpendItem(pProtectItem))
			return false;
	}
    
	if (!m_pUser->SpendItem(pStrengthItem))
	{
		return false;
	}
    
	bool bRet = false;
	USHORT usStrengthRet = STRENGTHJINMEI_FAIL;
	int nCurRate = RandGet(100);
	if (nCurRate <= nRate)
	{
		pSysData->SetInt(USERJINMEISYSDATA_STENGTHLEV, nCurLev + 1);
		m_bNeedUpdate = true;
		usStrengthRet = STRENGTHJINMEI_SUC;

		if(nCurLev >= 4)
			m_pUser->JinmeiBroadCast(2,nType,nCurLev + 1);

//		MyLogSave("syslog/jinmeisys", "玩家(%d)经脉(%d)强化成功(LEV=%d->%d)。", m_pUser->GetID(), nType, nCurLev, nCurLev + 1);
		bRet = true;
	}
	else
	{
		if (!bProtect)
		{
			pData = this->GetJinmeiStrengthDataByLev(nCurLev);
			if (NULL != pData)
			{
				int nFailLev = pData->GetInt(JINMEISTRENGTHDATA_FAILLEV);
				nFailStrengLev = nFailLev;
				pSysData->SetInt(USERJINMEISYSDATA_STENGTHLEV, nFailLev);
				m_bNeedUpdate = true;
//				MyLogSave("syslog/jinmeisys", "玩家(%d)经脉(%d)强化失败且未保护降级为(%d)。", m_pUser->GetID(), nType, nFailLev);
			}
			else
			{
//				MyLogSave("syslog/jinmeisys", "玩家(%d)经脉(%d)强化失败且未保护但未降级，数据配置异常。", m_pUser->GetID(), nType);
			}
		}
		else
		{
//			MyLogSave("syslog/jinmeisys", "玩家(%d)经脉(%d)强化失败且使用了保护。", m_pUser->GetID(), nType);
		}
	}

	UpdateJinmeiStrengthLev();

    return bRet;
	DEBUG_CATCH("catch at bool CUserJinMeiSys::StrengthJinmei")
	return false;
}

void CUserJinMeiSys::UpdateJinmeiLimitLev()
{
	m_nLimitJinmeiLev = 0;
	if (m_vecUserJinmeiData.size() < MAX_JINMEI_NUM)
	{
		return;
	}

	VEC_USERJINMEIDATA::iterator it = m_vecUserJinmeiData.begin();
	for (; it != m_vecUserJinmeiData.end(); ++it)
	{
		CUserJinMeiSysData* pData = (*it);
		if (NULL != pData)
		{
			int nLev = pData->GetInt(USERJINMEISYSDATA_LEV);
			if (m_nLimitJinmeiLev == 0)
			{
				m_nLimitJinmeiLev = nLev;
			}
			else
			{
				if (m_nLimitJinmeiLev > nLev)
				{
					m_nLimitJinmeiLev = nLev;
				}
			}
		}
	}

	CJinmeiEffectData* pEffectData = this->GetJinmeiEffectData(JINMEIACHIEVE_LEV);
	if (NULL != pEffectData)
	{
		m_nMaxLearnNum = pEffectData->GetInt(JINMEIEFFECTDATA_LEARNNUM);
		m_nMaxWaitNum  = pEffectData->GetInt(JINMEIEFFECTDATA_WAITNUM);
	}
}

void CUserJinMeiSys::UpdateJinmeiStrengthLev()
{
	if (m_vecUserJinmeiData.size() < MAX_JINMEI_NUM)
	{
		return;
	}

	VEC_USERJINMEIDATA::iterator it = m_vecUserJinmeiData.begin();
	for (; it != m_vecUserJinmeiData.end(); ++it)
	{
		CUserJinMeiSysData* pData = (*it);
		if (NULL != pData)
		{
			int nLev = pData->GetInt(USERJINMEISYSDATA_STENGTHLEV);
			if (m_nLimitJinmeiStrengthLev == 0)
			{
				m_nLimitJinmeiStrengthLev = nLev;
			}
			else
			{
				if (m_nLimitJinmeiStrengthLev > nLev)
				{
					m_nLimitJinmeiStrengthLev = nLev;
				}
			}
		}
	}
}
