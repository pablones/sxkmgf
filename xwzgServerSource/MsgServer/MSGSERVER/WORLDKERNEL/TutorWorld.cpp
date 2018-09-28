// TutorWorld.cpp: implementation of the CTutorWorld class.
//
//////////////////////////////////////////////////////////////////////
#define	_WINSOCKAPI_

#include <afx.h>
#include "TutorWorld.h"
#include "WorldKernel.h"
#include "StrRes.h"

MYHEAP_IMPLEMENTATION(CTutorWorld, s_heap)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTutorWorld::CTutorWorld()
{
	m_bUpdate = false;
	m_pDb = NULL;
}

CTutorWorld::~CTutorWorld()
{

}

bool CTutorWorld::Create(IDatabase* pDb)
{
	m_pDb = pDb;
	return true;
}

bool CTutorWorld::ProcessTutorInfo(const ChgTutorInfo *info)
{
	switch(info->nAction)
	{
	case CHGTUTORDATA_REQDELTUTOR:
		{
			return RequestDelTutor(info, true);
		}
		break;

	case CHGTUTORDATA_REQDELSTUDENT:
		{
			return RequestDelTutor(info, false);
		}
		break;

	case CHGTUTORDATA_RECOVERTUTOR:
		{
			return RecoverTutor(info, true);
		}
		break;

	case CHGTUTORDATA_RECOVERSTUDENT:
		{
			return RecoverTutor(info, false);
		}
		break;

	case CHGTUTORDATA_REQADDDENSITY:
		{
			return AddDensity(info);
		}
		break;

	case CHGTUTORDATA_CANCELLEAVETUTOR:
		{
			return CancelLeaveTutor(info);
		}
		break;

	case CHGTUTORDATA_CANCELDELSTUDENT:
		{
			return CancelDelStudent(info);
		}
		break;

	default:
		break;
	}

	return false;
}

bool CTutorWorld::CancelLeaveTutor(const ChgTutorInfo *info)
{
	if(NULL == info)
		return false;

	CPlayer *pTutor   = UserList()->GetPlayer(info->idTutor);
	CPlayer *pStudent = UserList()->GetPlayer(info->idStudent);

	if(pTutor && UserList()->IsLoginMapGroup(pTutor->idSocket))
	{
		return true;
	}
	else
	{
		if(pStudent)
		{	
			CMsgTalk	msg;
			IF_OK(msg.Create(SYSTEM_NAME, pStudent->szName, (char*)g_objStrRes.GetStr(11026), NULL, 0xff0000, _TXTATR_SYSTEM))    //"对方不在线"		
				pStudent->SendMsg(&msg);
		}
		return false;
	}

	return false;
}

bool CTutorWorld::CancelDelStudent(const ChgTutorInfo *info)
{
	if(NULL == info)
		return false;

	CPlayer *pTutor   = UserList()->GetPlayer(info->idTutor);
	CPlayer *pStudent = UserList()->GetPlayer(info->idStudent);

	if(pStudent && UserList()->IsLoginMapGroup(pStudent->idSocket))
	{
		return true;
	}
	else
	{
		if(pTutor)
		{	
			CMsgTalk	msg;
			IF_OK(msg.Create(SYSTEM_NAME, pTutor->szName, (char*)g_objStrRes.GetStr(11026), NULL, 0xff0000, _TXTATR_SYSTEM))    //"对方不在线"		
				pTutor->SendMsg(&msg);
		}
		return false;
	}

	return false;
}

bool CTutorWorld::AddDensity(const ChgTutorInfo *info)
{
// 	if(NULL == info || NULL == m_pDb)
// 		return false;
// 
// 	SQLBUF	szSQL;
// 	sprintf(szSQL, "SELECT * FROM %s WHERE id='%d' LIMIT 1", _TBL_TUTOR, info->id);
// 	IRecordset* pRes =m_pDb->CreateNewRecordset(szSQL);
// 	if(NULL == pRes)
// 		return false;
// 
// 	int nDensity = pRes->GetInt(TUTORDATA_DENSITY);
// 	nDensity += info->nTutorImpart;
// 	nDensity = max(0, nDensity);
// 	nDensity = min(MAX_DENSITY, nDensity);
// 
// 	pRes->SetInt(TUTORDATA_DENSITY, nDensity);
// 	pRes->UpdateRecord();
// 
//    SAFE_RELEASE(pRes);

   return true;
}

bool CTutorWorld::RequestDelTutor(const ChgTutorInfo *info, bool bTutor)
{
// 	if(NULL == info || NULL == m_pDb)
// 		return false;
// 
// 	SQLBUF	szSQL;
// 	sprintf(szSQL, "SELECT * FROM %s WHERE id='%d' LIMIT 1", _TBL_TUTOR, info->id);
// 	IRecordset* pRes =m_pDb->CreateNewRecordset(szSQL);
// 	if(NULL == pRes)
// 		return false;
// 
//    int nApplyType = 0;
//    nApplyType = pRes->GetInt(TUTORDATA_APPLYTYPE);
//    if(0 != nApplyType)
//    {
// 	   CPlayer *pPlayer = UserList()->GetPlayer(info->idUser);
// 	   if(pPlayer)
// 	   {
// 		   CMsgTalk	msg;
// 		   IF_OK(msg.Create(SYSTEM_NAME, pPlayer->szName, (char*)g_objStrRes.GetStr(28309), NULL, 0xff0000, _TXTATR_SYSTEM))    //"对方已经先提出申请"		
// 			   pPlayer->SendMsg(&msg);
// 		   return false;
// 	   }
//    }
// 
//    CTime time(CTime::GetCurrentTime());
//    
//    CString ss = time.Format("%Y-%m-%d %H:%M:%S");
//    char szTime[32];
//    strcpy(szTime, ss);
//    
//    if(bTutor)   //徒弟请求删除师傅
//    {
// 	   nApplyType = 1;
//    }
//    else
//    {
// 	   nApplyType = 2;
//    }
//    pRes->SetInt(TUTORDATA_APPLYTYPE, nApplyType);
//    pRes->SetStr(TUTORDATA_DELTIME, szTime, 32);
//    pRes->UpdateRecord();
// 
//    SAFE_RELEASE(pRes);

   return true;
}

bool CTutorWorld::RecoverTutor(const ChgTutorInfo *info, bool bTutor)
{
// 	if(NULL == info || NULL == m_pDb)
// 		return false;
// 
// 	SQLBUF	szSQL;
// 	sprintf(szSQL, "SELECT * FROM %s WHERE id='%d' LIMIT 1", _TBL_TUTOR, info->id);
// 	IRecordset* pRes =m_pDb->CreateNewRecordset(szSQL);
// 	if(NULL == pRes)
// 		return false;
// 
//    int nApplyType = 0;
//    nApplyType = pRes->GetInt(TUTORDATA_APPLYTYPE);
//    if(0 == nApplyType)
//    {
// 	   CPlayer *pPlayer = UserList()->GetPlayer(info->idUser);
// 	   if(pPlayer)
// 	   {
// 		   CMsgTalk	msg;
// 		   IF_OK(msg.Create(SYSTEM_NAME, pPlayer->szName, (char*)g_objStrRes.GetStr(28310), NULL, 0xff0000, _TXTATR_SYSTEM))    //师徒关系已经恢复	
// 			   pPlayer->SendMsg(&msg);
// 		   return false;
// 	   }
//    }
// 
//    CTime time(CTime::GetCurrentTime());
//    CString ss = time.Format("%Y-%m-%d %H:%M:%S");
//    char szTime[32];
//    strcpy(szTime, ss);
//    
//  
//    pRes->SetInt(TUTORDATA_APPLYTYPE, 0);
//    pRes->SetStr(TUTORDATA_DELTIME, szTime, 32);
//    pRes->UpdateRecord();
// 
//    SAFE_RELEASE(pRes);

   return true;
}

void CTutorWorld::UpdateOnTime()
{
}

void CTutorWorld::OnTimer()
{
	SYSTEMTIME sysTime;
	::GetLocalTime( &sysTime );	
	if (0  == sysTime.wHour && !m_bUpdate)
	{
		UpdateOnTime();
		m_bUpdate = true;
	}
	
	//1点钟恢复更新标志
	if (1  == sysTime.wHour && m_bUpdate)
	{
		m_bUpdate = false;
	}
}
