// InstanceUseTimeMgr.cpp: implementation of the CInstanceUseTimeMgr class.
//
//////////////////////////////////////////////////////////////////////
#include <afx.h>
#include "MapGroup.h"
#include "InstanceUseTimeMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInstanceUseTimeMgr::CInstanceUseTimeMgr()
{
	m_bDayUp = false;
}

CInstanceUseTimeMgr::~CInstanceUseTimeMgr()
{
	ClearInstanceUseSet();
}

void CInstanceUseTimeMgr::ClearInstanceUseSet()
{
	for(list<CInstanceUseTime*>::iterator it = m_setInstanceUse.begin(); it != m_setInstanceUse.end(); it ++)
	{
		CInstanceUseTime* pUserData = *it;
		delete pUserData;
	}
	m_setInstanceUse.clear();
}

bool CInstanceUseTimeMgr::Create (IDatabase* pDb,  PROCESS_ID idProcess)
{
	m_pDB = pDb;
	m_idProcess = idProcess;

	return true;
}

bool CInstanceUseTimeMgr::UserLogin(OBJID idUser)
{
	CHECKF(m_pDB);

	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s WHERE user_id = %u", _TBL_INSTANCEUSETIME, idUser);
	IRecordset* pRes = m_pDB->CreateNewRecordset(szSQL, false);
	if(pRes)
	{
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			CInstanceUseTime *pInstanceUse = CInstanceUseTime::CreateNew(m_pDB, m_idProcess);
			IF_OK(pInstanceUse)
			{
				if(pInstanceUse->Create(pRes))
				{
					m_setInstanceUse.push_back(pInstanceUse);
				}
				else
					pInstanceUse->ReleaseByOwner();
			}
			pRes->MoveNext();
		}
		pRes->Release();
	}

	return true;
}

bool CInstanceUseTimeMgr::UserLogOut(OBJID idUser)
{
	CHECKF(m_pDB);

	for(list<CInstanceUseTime*>::iterator it = m_setInstanceUse.begin(); it != m_setInstanceUse.end(); )
	{
		CInstanceUseTime* pUserData = *it;
		if(pUserData->GetUserID() == idUser)
		{
			list<CInstanceUseTime*>::iterator itDel = it;
			CInstanceUseTime* pUserData = *itDel;
			it ++;
			delete pUserData;
			m_setInstanceUse.erase(itDel);
		}
		else
			it ++;
	}
	
	return true;
}

bool CInstanceUseTimeMgr::AddNewInstanceUse(OBJID idUser, UINT nInstanceType,int nAddData/* = 1*/)
{
	CHECKF(m_pDB);
	CInstanceUseTime *pInstanceUse = this->GetInstanceUseByUseAndType(idUser, nInstanceType);
	if(NULL == pInstanceUse)
	{
		InstanceUseTimeInfo pInfo;
		memset(&pInfo, 0, sizeof(InstanceUseTimeInfo));
		
		pInfo.nInstanceType = nInstanceType;
		pInfo.nUserId = idUser;
		pInfo.nUserTime = nAddData;
		pInfo.nCanUser = 1;
		pInfo.nFinishTimes = 0;
		
		CTime time(CTime::GetCurrentTime());
		CString ss = time.Format("%Y-%m-%d %H:%M:%S");
		strcpy(pInfo.szCreateTime, ss);

		pInstanceUse = CInstanceUseTime::CreateNew(m_pDB,m_idProcess);
		CHECKF(pInstanceUse);
		if(!pInstanceUse->Create(&pInfo,true))
		{
			pInstanceUse->ReleaseByOwner();
			CHECKF(!"pInstanceUse->Create(&pInfo,true)");
		}

		m_setInstanceUse.push_back(pInstanceUse);
		return true;
	}
	
	int nTime = GetCurrentTimeInterval(pInstanceUse->GetCreateTime());
	if(nTime > 0)
	{
		//有效，累加
		char szTimeNow[64] = "";
		CTime time(CTime::GetCurrentTime());
		CString ss = time.Format("%Y-%m-%d %H:%M:%S");
		strcpy(szTimeNow, ss);
		pInstanceUse->SetUseTimes(pInstanceUse->GetInstanceUseTimes() + nAddData,szTimeNow,false,true);
		return true;
	}
	else
	{
		//无效覆盖
		char szTimeNow[64] = "";
		CTime time(CTime::GetCurrentTime());
		CString ss = time.Format("%Y-%m-%d %H:%M:%S");
		strcpy(szTimeNow, ss);
		pInstanceUse->SetUseTimes(nAddData,szTimeNow,true,true);
		return true;

	}
}

int CInstanceUseTimeMgr::GetCurrentTimeInterval(const char* pText)
{
	CString nCreateTime = pText;
	CString Gpstime;
	CString szTemp = "2009-01-01 18:18:18";

	if(szTemp.GetAt(4) == nCreateTime.GetAt(4)
		&& szTemp.GetAt(7) == nCreateTime.GetAt(7)
		&& szTemp.GetAt(10) == nCreateTime.GetAt(10)
		&& szTemp.GetAt(13) == nCreateTime.GetAt(13)
		&& szTemp.GetAt(16) == nCreateTime.GetAt(16))
	{
	}
	else
	{
		LOGWARNING("非法 CInstanceUseTimeMgr time=%s", pText);
		return 0;
	}
	
	int y;	
    Gpstime=nCreateTime.Mid(0,4);
	y=atoi(Gpstime);
	
	int m;
    Gpstime=nCreateTime.Mid(5,2);
	m=atoi(Gpstime);

	int d;
    Gpstime=nCreateTime.Mid(8,2);
	d=atoi(Gpstime);

	int hh;
    Gpstime=nCreateTime.Mid(11,2);
	hh=atoi(Gpstime);

	int mm;
    Gpstime=nCreateTime.Mid(14,2);
	mm=atoi(Gpstime);

	int ss;
    Gpstime=nCreateTime.Mid(17,2);
	ss=atoi(Gpstime);
	
	int ss2=0;
	
	CTime tCreate(y,m,d,hh,mm,ss,ss2);	
	CTime tNowCheck(CTime::GetCurrentTime());
	if(tNowCheck.GetHour() < 5)//5点前算前一天
	{
		CTimeSpan tPassDay(1,0,0,0);
		tNowCheck -= tPassDay;
	}
	
	CTime tNow(tNowCheck.GetYear(),tNowCheck.GetMonth(),tNowCheck.GetDay(),5,0,0);
	if(tNow >= tCreate)
		return 0;

//	CTimeSpan tInterval = tCreate - tNow;
	return 1;//tInterval.GetTotalHours();
}

int CInstanceUseTimeMgr::CheckInstanceUse(UINT userID, UINT nInstanceType)
{
	CHECKF(m_pDB);
	CInstanceUseTime *pInstanceUse = this->GetInstanceUseByUseAndType(userID, nInstanceType);
	if(NULL == pInstanceUse)
		return -1;

	int nTime = GetCurrentTimeInterval(pInstanceUse->GetCreateTime());
	if(nTime > 0)
	{
		return pInstanceUse->GetInstanceUseTimes();
	}

	return 0;
}


void CInstanceUseTimeMgr::OnTimer(time_t tCurr)
{
	return;
}

CInstanceUseTime* CInstanceUseTimeMgr::GetInstanceUseByUseAndType(UINT userID, UINT nInstanceType)
{
	for(list<CInstanceUseTime*>::iterator it = m_setInstanceUse.begin(); it != m_setInstanceUse.end();it ++ )
	{
		CInstanceUseTime* pUserData = *it;
		if(pUserData && pUserData->GetUserID() == userID && pUserData->GetInstanceType() == nInstanceType)
		{
			return pUserData;
		}
	}
	
	return NULL;
}

UINT CInstanceUseTimeMgr::CheckUseInstance(UINT userID, UINT nInstanceType)
{
	CHECKF(m_pDB);
	CInstanceUseTime *pInstanceUse = this->GetInstanceUseByUseAndType(userID, nInstanceType);
	if(NULL == pInstanceUse)
		return 0;
	
	return pInstanceUse->GetCanUser();
}

void CInstanceUseTimeMgr::ClearUsing(UINT userID, UINT nInstanceType/* = ID_NONE*/)
{
	for(list<CInstanceUseTime*>::iterator it = m_setInstanceUse.begin(); it != m_setInstanceUse.end();it ++ )
	{
		CInstanceUseTime* pUserData = *it;
		if(pUserData && pUserData->GetUserID() == userID && pUserData->GetInstanceType() != nInstanceType && pUserData->GetCanUser() == 2)
		{
			UseInstance(userID,pUserData->GetInstanceType(),0);
		}
	}
}

bool CInstanceUseTimeMgr::UseInstance(UINT userID, UINT nInstanceType,int nUse/* = 0*/)
{
	CHECKF(nUse == 0 || nUse == 2);
	CHECKF(m_pDB);
	CInstanceUseTime *pInstanceUse = this->GetInstanceUseByUseAndType(userID, nInstanceType);
	if(NULL == pInstanceUse)
		return UseInstanceLeaveLine(userID, nInstanceType,nUse);

	if(pInstanceUse->GetCanUser() == nUse)
		return false;

	pInstanceUse->SetUse(nUse,true);
	return true;
}

bool CInstanceUseTimeMgr::UseInstanceLeaveLine(UINT userID, UINT nInstanceType,int nUse/* = 0*/)
{
	CHECKF(m_pDB);
	
	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s WHERE user_id = %u", _TBL_INSTANCEUSETIME, userID);
	IRecordset* pRes = m_pDB->CreateNewRecordset(szSQL, false);
	if(pRes)
	{
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			int nInsType = pRes->GetInt(CInstanceUseTime_INSTANCETYPE);
			if(nInsType == nInstanceType)
			{
				if(pRes->GetInt(CInstanceUseTime_CANUSER) != nUse)
				{
					pRes->SetInt(CInstanceUseTime_CANUSER,nUse);
					pRes->UpdateRecord();
					break;
				}
			}
			pRes->MoveNext();
		}
		pRes->Release();
	}
	
	return true;
}


bool CInstanceUseTimeMgr::FinishInstance(UINT userID, UINT nInstanceType,int nTime)
{
	CHECKF(m_pDB);
	CInstanceUseTime *pInstanceUse = this->GetInstanceUseByUseAndType(userID, nInstanceType);
	if(NULL == pInstanceUse)
		return false;
	
	int nFinishTime = pInstanceUse->GetFinishTime() + nTime;
	pInstanceUse->SetFinishTime(nFinishTime,true);
	return true;
}

int CInstanceUseTimeMgr::GetFinishInstanceTimes(OBJID idUser, UINT nInstanceType)
{
	CHECKF(m_pDB);
	CInstanceUseTime *pInstanceUse = this->GetInstanceUseByUseAndType(idUser, nInstanceType);
	if(NULL == pInstanceUse)
	{
		return 0;
	}
	
	int nTime = GetCurrentTimeInterval(pInstanceUse->GetCreateTime());
	if(nTime > 0)
	{
		return pInstanceUse->GetFinishTime();
	}

	return 0;
}