// UserJinMeiSysMgr.cpp: implementation of the CUserJinMeiSysMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "UserJinMeiSysMgr.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserJinMeiSysMgr::CUserJinMeiSysMgr()
{
	m_tLearnJinmei.SetInterval(5);
	m_tLearnJinmei.Update();
}

CUserJinMeiSysMgr::~CUserJinMeiSysMgr()
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.begin();
	for (; it != m_mapUserJinmeiSys.end(); ++it)
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			SAFE_DELETE(pSys);
		}
	}

	m_mapUserJinmeiSys.clear();
}

bool CUserJinMeiSysMgr::Init( PROCESS_ID idProcess, IDatabase* pDataBase, IRecordset* pDefJinmeiRes )
{
	m_idProcess = idProcess;
	m_pDb = pDataBase;
	m_pDefRes = pDefJinmeiRes;
	return true;
}

void CUserJinMeiSysMgr::OnTimer(time_t tCurr)
{
	//5ÃëÂÖÑ¯Ò»´Î
	if (m_tLearnJinmei.ToNextTime())
	{
		MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.begin();
		for (; it != m_mapUserJinmeiSys.end(); ++it)
		{
			CUserJinMeiSys* pSys = it->second;
			if (NULL != pSys)
			{
				pSys->OnTimer(tCurr);
			}
		}
	}
}

bool CUserJinMeiSysMgr::AddJinmeiForChange(CUser* pUser,JinmeiInfoStruct* pInfo)
{
	CHECKF(pUser);
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(pUser->GetID());
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			return pSys->AddJinmeiForChange(GameDataDefault()->GetJinmeiData(),pInfo);
		}
	}
	return false;
}

bool CUserJinMeiSysMgr::UpdateChange(CUser* pUser)
{
	CHECKF(pUser);
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(pUser->GetID());
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			pSys->UpdateChange();
			return true;
		}
	}
	return false;
}

void CUserJinMeiSysMgr::SendAllObjInfo	(CUser* pUser,PROCESS_ID idProcess)
{
	CHECK(pUser);
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(pUser->GetID());
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			for (int i = USERJINMEITYPE_YANGQIAO ; i <= USERJINMEITYPE_RENMEI ; i ++)
			{
				CUserJinMeiSysData* pData = pSys->GetJinmeiDataByType(i);
				if(pData)
				{
					JinmeiInfoStruct szInfo;
					pData->GetInfo(szInfo);
					pUser->SendObjInfo(idProcess, pUser->GetID(), INFO_JINMEI, &szInfo, sizeof(szInfo));
				}
			}
		}
	}
}

void CUserJinMeiSysMgr::OnUserLogin(CUser* pUser,bool bLogin)
{
	DEBUG_TRY
    CHECK(pUser);    
	OBJID idUser = pUser->GetID();
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			LOGERROR("error at void CUserJinMeiSysMgr::OnUserLogin(%d) already exist", idUser);
			//pSys->OnLogin();
		}
	}
	else
	{
		CUserJinMeiSys* pSys = new CUserJinMeiSys(pUser, m_pDb, m_pDefRes);
		if (NULL != pSys)
		{
	    	m_mapUserJinmeiSys.insert(MAP_USERJINMEISYS::value_type(idUser, pSys));	
			if(bLogin)
				pSys->OnLogin();
		}
	}
	DEBUG_CATCH2("catch at CUserJinMeiSysMgr::OnUserLogin(idUser = %d) ", pUser->GetID())
}

void CUserJinMeiSysMgr::OnUserLogOut(OBJID idUser)
{
	DEBUG_TRY
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			pSys->OnLogOut();
			pSys->Release();
			pSys = NULL;
			m_mapUserJinmeiSys.erase(it);
		}
	}
	DEBUG_CATCH2("catch at CUserJinMeiSysMgr::OnUserLogOut(idUser = %d) ", idUser)
}

void CUserJinMeiSysMgr::SendJinmeiSysInfo(OBJID idUser, CUser* pTargetUser)
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			pSys->SendJinmeiSysInfo(pTargetUser);
		}
	}
}

void CUserJinMeiSysMgr::SendLoginOKInfo(OBJID idUser)
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			pSys->SendLoginOKInfo();
		}
	}
}

int CUserJinMeiSysMgr::GetJinMeiValue(int nType, OBJID idUser)
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			return pSys->GetJinmeiTypeValue(nType);
		}
	}

	return 0;
}

int CUserJinMeiSysMgr::GetJinmeiDecManaRate(OBJID idUser)
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			return pSys->GetJinmeiDecManaRate();
		}
	}
	
	return 0;
}

int CUserJinMeiSysMgr::GetJinmeiDecDamageRate(OBJID idUser)
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			return pSys->GetJinmeiDecDamageRate();
		}
	}
	
	return 0;
}

int CUserJinMeiSysMgr::GetJinmeiIncDamageRate(OBJID idUser)
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			return pSys->GetJinmeiIncDamageRate();
		}
	}
	
	return 0;
}

void CUserJinMeiSysMgr::StartLearnJinmei( int nType, OBJID idUser )
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			pSys->StartLearnJinmei(nType);
		}
	}
}

void CUserJinMeiSysMgr::StopLearnJinmei( int nType, OBJID idUser )
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			pSys->StopLearn(nType);
		}
	}	
}

void CUserJinMeiSysMgr::MoveWaitJinmeiType(int nType, OBJID idUser)
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			pSys->MoveWaitQueue(nType);
		}
	}
}

void CUserJinMeiSysMgr::AddWaitJinmeiType(int nType, OBJID idUser)
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			pSys->AddJinmeiWaitQueue(nType);
		}
	}
}

bool CUserJinMeiSysMgr::UpJinmeiLevByType( int nType, int nUpLevType, OBJID idUser )
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
	    	return pSys->UpJinmeiLev(nUpLevType, nType);
		}
	}

	return false;
}

bool CUserJinMeiSysMgr::StrengthJinmeiSys( int nType, bool bProtect, OBJID idUser )
{
	MAP_USERJINMEISYS::iterator it = m_mapUserJinmeiSys.find(idUser);
	if (it != m_mapUserJinmeiSys.end())
	{
		CUserJinMeiSys* pSys = it->second;
		if (NULL != pSys)
		{
			return pSys->StrengthUserJinmei(nType, bProtect);
		}
	}

	return false;
}
