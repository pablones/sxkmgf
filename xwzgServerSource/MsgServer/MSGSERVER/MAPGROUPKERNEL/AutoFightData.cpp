// AutoFightData.cpp: implementation of the CAutoFightData class.
//
//////////////////////////////////////////////////////////////////////

#include "AutoFightData.h"
#include "User.h"
#include "MsgAutoFight.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char    szAutoTable[] = _TBL_AUTOFIGHTSET;
char    szUsersid[]   = "userid";
CAutoFightData::CAutoFightData()
{
// 	m_AutoFight = NULL;
// 	m_pAutoFightInfo = new CAutoFightInfo;
//  	memset(m_pAutoFightInfo, 0, sizeof(CAutoFightInfo));
	m_AutoFight = NULL;
	bAutof = false;
	m_bNeedUpdate = false;
}

CAutoFightData::~CAutoFightData()
{
	if(m_AutoFight)
		m_AutoFight->Release();
}

// CAutoFightData* CAutoFightData::CreateNew()
// {
// 	CAutoFightData* ptr=new CAutoFightData; 
// 	return ptr;
// }

ULONG CAutoFightData::ReleaseByOwner()
{
	delete this; 
	return 0; 
}

bool  CAutoFightData::Create(IDatabase* pDb,CUser* pUser,bool bLogin,IRecordset* pRec)
{
	CHECKF(!m_AutoFight);
	CHECKF(pDb);
	CHECKF(pUser);
	
	m_AutoFight = CAutoFight::CreateNew();
	IF_NOT(m_AutoFight)
		return false;

	if(!bLogin)
	{
		IF_NOT(m_AutoFight->Create(pRec, pUser->GetID()))
			return false;
		return true;
	}

	IF_NOT(m_AutoFight->Create(pUser->GetID(),pDb))
		return false;
	
    return true;
}

bool CAutoFightData::AutoFightReadDB(CUser* pUser, IDatabase* pDB, const char* pSkillName)
{
	if (!pUser)
	{
		return false;
	}
	if (!pUser->CanBeSee())
	{
		return false;
	}
	
	CHECKF(m_AutoFight);
	
	if(!m_AutoFight->GetID())  //不存在该用户的信息
	{
		m_pAutoFightInfo.idUser = pUser->GetID();
		m_AutoFight->SetInt(_AUTOFUGHTDATA_IDUSER, m_pAutoFightInfo.idUser);
		if(!m_AutoFight->InsertRecord())
			return false;
		
	}
	if (m_AutoFight)
	{
//		m_pAutoFightInfo->id                 = pNewRecordset->GetInt(_AUTOFUGHTDATA_ID);
		memset(&m_pAutoFightInfo,0L,sizeof(AUTOF));
		m_pAutoFightInfo.idUser             = m_AutoFight->GetInt(_AUTOFUGHTDATA_IDUSER);
		m_pAutoFightInfo.AutoSwitch			= m_AutoFight->GetInt(_AUTOFUGHTDATA_SWITCH);
		m_pAutoFightInfo.PercentHp			= m_AutoFight->GetInt(_AUTOFUGHTDATA_PERCENTHP);
		m_pAutoFightInfo.PercentMp			= m_AutoFight->GetInt(_AUTOFUGHTDATA_PERCENTMP);
		m_pAutoFightInfo.SkillHp			= m_AutoFight->GetInt(_AUTOFUGHTDATA_SKILLHP);
		m_pAutoFightInfo.SkillMp			= m_AutoFight->GetInt(_AUTOFUGHTDATA_SKILLMP);
		m_pAutoFightInfo.SkillTeamhp		= m_AutoFight->GetInt(_AUTOFUGHTDATA_SKILLTEAMHP);
		m_pAutoFightInfo.EudHp				= m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHP);
		m_pAutoFightInfo.EudHappy			= m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHAPPY);
		m_pAutoFightInfo.HitRange			= m_AutoFight->GetInt(_AUTOFUGHTDATA_HITRANGE);
		m_pAutoFightInfo.DoubleExp			= m_AutoFight->GetInt(_AUTOFUGHTDATA_DOUBLEEXP);
		m_pAutoFightInfo.DoubleEudExp		= m_AutoFight->GetInt(_AUTOFUGHTDATA_DOUBLEEUDEXP);
		m_pAutoFightInfo.LIYUCHUN			= m_AutoFight->GetInt(_AUTOFUGHTDATA_LIYUCHUN);
		m_pAutoFightInfo.Hpmed1				= m_AutoFight->GetInt(_AUTOFUGHTDATA_HPMED1);
		m_pAutoFightInfo.Hpmed2				= m_AutoFight->GetInt(_AUTOFUGHTDATA_HPMED2);
		m_pAutoFightInfo.Hpmed3				= m_AutoFight->GetInt(_AUTOFUGHTDATA_HPMED3);
		m_pAutoFightInfo.Mpmed1				= m_AutoFight->GetInt(_AUTOFUGHTDATA_MPMED1);
		m_pAutoFightInfo.Mpmed2				= m_AutoFight->GetInt(_AUTOFUGHTDATA_MPMED2);
		m_pAutoFightInfo.Mpmed3				= m_AutoFight->GetInt(_AUTOFUGHTDATA_MPMED3);
		m_pAutoFightInfo.EudHpmed1			= m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHPMED1);
		m_pAutoFightInfo.EudHpmed2			= m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHPMED2);
		m_pAutoFightInfo.EudHpmed3			= m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHPMED3);
		m_pAutoFightInfo.EudToy1			= m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDTOY1);
		m_pAutoFightInfo.EudToy2			= m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDTOY2);
		m_pAutoFightInfo.temp				= 0;
		m_pAutoFightInfo.id					= m_AutoFight->GetID();

		
		CMsgAutoFight msg;
		if(msg.Create(_AutoFight_READ,m_pAutoFightInfo))
			pUser->SendMsg(&msg);

		return true;
	}

	return false;
}

bool CAutoFightData::Update()
{
	CHECKF(m_AutoFight);
	
	if(m_bNeedUpdate)
	{
		m_bNeedUpdate = false;
		return m_AutoFight->Update();
	}
	return true;
}

bool CAutoFightData::AutoFightWriteDB(struct AutoFightInfo &pInfo,bool bUpdate,IDatabase* pDB/* = NULL*/,bool bInsert/* = true*/)
{
	if(bUpdate)
		CHECKF(pDB);
	bool bResult;
	if (!m_AutoFight)
	{
		return false;
	}

	if(m_AutoFight)
	{
		if(!bInsert)
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_IDUSER, pInfo.idUser);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_PERCENTHP, pInfo.PercentHp);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_PERCENTMP, pInfo.PercentMp);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_SWITCH, pInfo.AutoSwitch);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_SKILLHP, pInfo.SkillHp);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_SKILLMP, pInfo.SkillMp);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_SKILLTEAMHP, pInfo.SkillTeamhp);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHP, pInfo.EudHp);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHAPPY, pInfo.EudHappy);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_HITRANGE, pInfo.HitRange);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_DOUBLEEXP, pInfo.DoubleExp);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_DOUBLEEUDEXP, pInfo.DoubleEudExp);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_LIYUCHUN, pInfo.LIYUCHUN);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_HPMED1, pInfo.Hpmed1);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_HPMED2, pInfo.Hpmed2);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_HPMED3, pInfo.Hpmed3);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_MPMED1, pInfo.Mpmed1);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_MPMED2, pInfo.Mpmed2);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_MPMED3, pInfo.Mpmed3);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHPMED1, pInfo.EudHpmed1);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHPMED2, pInfo.EudHpmed2);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHPMED3, pInfo.EudHpmed3);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDTOY1, pInfo.EudToy1);
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDTOY2, pInfo.EudToy2);

			m_bNeedUpdate = false;
			m_AutoFight->ClearUpdateFlags();
		}


// 		pNewRecordset->SetInt(_AUTOFUGHTDATA_ID, pInfo->id);
		if(pInfo.idUser != m_AutoFight->GetInt(_AUTOFUGHTDATA_IDUSER))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_IDUSER, pInfo.idUser);
			m_bNeedUpdate = true;
		}
		if(pInfo.PercentHp != m_AutoFight->GetInt(_AUTOFUGHTDATA_PERCENTHP))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_PERCENTHP, pInfo.PercentHp);
			m_bNeedUpdate = true;
		}
		if(pInfo.PercentMp != m_AutoFight->GetInt(_AUTOFUGHTDATA_PERCENTMP))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_PERCENTMP, pInfo.PercentMp);
			m_bNeedUpdate = true;
		}
		if(pInfo.AutoSwitch != m_AutoFight->GetInt(_AUTOFUGHTDATA_SWITCH))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_SWITCH, pInfo.AutoSwitch);
			m_bNeedUpdate = true;
		}
		if(pInfo.SkillHp != m_AutoFight->GetInt(_AUTOFUGHTDATA_SKILLHP))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_SKILLHP, pInfo.SkillHp);
			m_bNeedUpdate = true;
		}
		if(pInfo.SkillMp != m_AutoFight->GetInt(_AUTOFUGHTDATA_SKILLMP))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_SKILLMP, pInfo.SkillMp);
			m_bNeedUpdate = true;
		}
		if(pInfo.SkillTeamhp != m_AutoFight->GetInt(_AUTOFUGHTDATA_SKILLTEAMHP))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_SKILLTEAMHP, pInfo.SkillTeamhp);
			m_bNeedUpdate = true;
		}
		if(pInfo.EudHp != m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHP))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHP, pInfo.EudHp);
			m_bNeedUpdate = true;
		}
		if(pInfo.EudHappy != m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHAPPY))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHAPPY, pInfo.EudHappy);
			m_bNeedUpdate = true;
		}
		if(pInfo.HitRange != m_AutoFight->GetInt(_AUTOFUGHTDATA_HITRANGE))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_HITRANGE, pInfo.HitRange);
			m_bNeedUpdate = true;
		}
		if(pInfo.DoubleExp != m_AutoFight->GetInt(_AUTOFUGHTDATA_DOUBLEEXP))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_DOUBLEEXP, pInfo.DoubleExp);
			m_bNeedUpdate = true;
		}
		if(pInfo.DoubleEudExp != m_AutoFight->GetInt(_AUTOFUGHTDATA_DOUBLEEUDEXP))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_DOUBLEEUDEXP, pInfo.DoubleEudExp);
			m_bNeedUpdate = true;
		}
		if(pInfo.LIYUCHUN != m_AutoFight->GetInt(_AUTOFUGHTDATA_LIYUCHUN))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_LIYUCHUN, pInfo.LIYUCHUN);
			m_bNeedUpdate = true;
		}
		if(pInfo.Hpmed1 != m_AutoFight->GetInt(_AUTOFUGHTDATA_HPMED1))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_HPMED1, pInfo.Hpmed1);
			m_bNeedUpdate = true;
		}
		if(pInfo.Hpmed2 != m_AutoFight->GetInt(_AUTOFUGHTDATA_HPMED2))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_HPMED2, pInfo.Hpmed2);
			m_bNeedUpdate = true;
		}
		if(pInfo.Hpmed3 != m_AutoFight->GetInt(_AUTOFUGHTDATA_HPMED3))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_HPMED3, pInfo.Hpmed3);
			m_bNeedUpdate = true;
		}
		if(pInfo.Mpmed1 != m_AutoFight->GetInt(_AUTOFUGHTDATA_MPMED1))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_MPMED1, pInfo.Mpmed1);
			m_bNeedUpdate = true;
		}
		if(pInfo.Mpmed2 != m_AutoFight->GetInt(_AUTOFUGHTDATA_MPMED2))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_MPMED2, pInfo.Mpmed2);
			m_bNeedUpdate = true;
		}
		if(pInfo.Mpmed3 != m_AutoFight->GetInt(_AUTOFUGHTDATA_MPMED3))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_MPMED3, pInfo.Mpmed3);
			m_bNeedUpdate = true;
		}
		if(pInfo.EudHpmed1 != m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHPMED1))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHPMED1, pInfo.EudHpmed1);
			m_bNeedUpdate = true;
		}
		if(pInfo.EudHpmed2 != m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHPMED2))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHPMED2, pInfo.EudHpmed2);
			m_bNeedUpdate = true;
		}
		if(pInfo.EudHpmed3 != m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDHPMED3))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDHPMED3, pInfo.EudHpmed3);
			m_bNeedUpdate = true;
		}
		if(pInfo.EudToy1 != m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDTOY1))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDTOY1, pInfo.EudToy1);
			m_bNeedUpdate = true;
		}
		if(pInfo.EudToy2 != m_AutoFight->GetInt(_AUTOFUGHTDATA_EUDTOY2))
		{
			m_AutoFight->SetInt(_AUTOFUGHTDATA_EUDTOY2, pInfo.EudToy2);
			m_bNeedUpdate = true;
		}

		if(bUpdate)
		{
			m_bNeedUpdate = false;
			m_AutoFight->Update();
		}

		bResult = true;
	}
	return bResult;
}

void CAutoFightData::SendObjInfo(CUser* pUser ,PROCESS_ID idProcess)
{
	CHECK(pUser);
	pUser->SendObjInfo(idProcess,pUser->GetID(),INFO_AUTOFIGHT,&m_pAutoFightInfo,sizeof(m_pAutoFightInfo));
}

void CAutoFightData::SetObjInfo(AUTOF* pStInfo)
{
	CHECK(pStInfo);
	AutoFightWriteDB(*pStInfo,false,NULL,false);
}
