// TutorData.cpp: implementation of the CTutorData class.
//
//////////////////////////////////////////////////////////////////////

#include "TutorData.h"
#include "UserManager.h"
char szTutorTable[] = _TBL_TUTOR;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTutorData::CTutorData()
{
	m_pData = NULL;
	memset(&m_stDetailInfo, 0L, sizeof(m_stDetailInfo));
	m_nLine = 0;
	m_nTeam = 0;
}

CTutorData::~CTutorData()
{
	SAFE_RELEASE(m_pData);
}
//////////////////////////////////////////////////////////////////////
bool CTutorData::Create(IRecordset* pRes)
{
	CHECKF(pRes);
	
	m_pData	= CGameTutorData::CreateNew();
	if (m_pData && m_pData->Create(pRes))
	{
		LoadDetailInfo(pRes);
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}


bool CTutorData::Create(IRecordset* pRes, CUser* pUser)
{
	CHECKF(pRes);
	
	m_pData	= CGameTutorData::CreateNew();
	if (m_pData && m_pData->Create(pRes))
	{
		LoadDetailInfo(pUser);
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}

bool CTutorData::Create(IRecordset* pRes, stTempUser* pUser)
{
	CHECKF(pRes);
	
	m_pData	= CGameTutorData::CreateNew();
	if (m_pData && m_pData->Create(pRes))
	{
		LoadDetailInfo(pUser);
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}


void CTutorData::LoadDetailInfo(IRecordset* pRes, int nOffset/*=0*/)
{
	CHECK(pRes);
	CHECK(TUTORDATA_SEX-nOffset>=0);

	m_stDetailInfo.nSex = pRes->GetInt(TUTORDATA_SEX-nOffset);
	m_stDetailInfo.nLookFace = pRes->GetInt(TUTORDATA_LOOKFACE-nOffset);
	m_stDetailInfo.nVipFace = pRes->GetInt(TUTORDATA_VIPFACE-nOffset);
	m_stDetailInfo.nLev = pRes->GetInt(TUTORDATA_LEV-nOffset);
	m_stDetailInfo.nMainJob = pRes->GetInt(TUTORDATA_MAINJOB-nOffset);
	m_stDetailInfo.nMainLev = pRes->GetInt(TUTORDATA_MAINLEV-nOffset);
	m_stDetailInfo.nSubJob = pRes->GetInt(TUTORDATA_SUBJOB-nOffset);
	m_stDetailInfo.nSubLev = pRes->GetInt(TUTORDATA_SUBLEV-nOffset);
	m_stDetailInfo.nImpart = pRes->GetInt(TUTORDATA_IMPART-nOffset);
	m_stDetailInfo.nMaxScore = pRes->GetInt(TUTORDATA_MAXSCORE-nOffset);
	m_stDetailInfo.nVipLev = pRes->GetInt(TUTORDATA_VIPLEV-nOffset);
	m_stDetailInfo.dwExitTime = pRes->GetInt(TUTORDATA_EXITTIME-nOffset);
	m_stDetailInfo.nMorality = pRes->GetInt(TUTORDATA_MORALITY-nOffset);
}

void CTutorData::LoadDetailInfo(CUser* pUser)
{
	CHECK(pUser);
	
	m_stDetailInfo.nSex			= pUser->GetSex();
	m_stDetailInfo.nLookFace	= pUser->GetLookFace();
	m_stDetailInfo.nVipFace		= pUser->GetVipface();
	m_stDetailInfo.nLev			= pUser->GetLev();
	m_stDetailInfo.nMainJob		= pUser->GetProfession(1);
	m_stDetailInfo.nMainLev		= pUser->GetJobLev(1);
	m_stDetailInfo.nSubJob		= pUser->GetProfession(2);
	m_stDetailInfo.nSubLev		= pUser->GetJobLev(2);
	m_stDetailInfo.nImpart		= pUser->GetImpart();
	m_stDetailInfo.nMaxScore	= pUser->GetMaxScore();
	m_stDetailInfo.nVipLev		= pUser->GetVipLev();
	m_stDetailInfo.dwExitTime	= pUser->GetExitTime();
	m_stDetailInfo.nMorality	= pUser->GetMorality();
}

void CTutorData::LoadDetailInfo(stTempUser* pUser)
{
	CHECK(pUser);
	
	m_stDetailInfo.nSex			= pUser->sex;
	m_stDetailInfo.nLookFace	= pUser->lookface;
	m_stDetailInfo.nVipFace		= pUser->vipface;
	m_stDetailInfo.nLev			= pUser->level;
	m_stDetailInfo.nMainJob		= pUser->Fir_Job;
	m_stDetailInfo.nMainLev		= pUser->Fir_JobLv;
	m_stDetailInfo.nSubJob		= pUser->Sec_Job;
	m_stDetailInfo.nSubLev		= pUser->Sec_JobLv;
	m_stDetailInfo.nImpart		= pUser->Tutor_Lv;
	m_stDetailInfo.nMaxScore	= pUser->max_score;
	m_stDetailInfo.nVipLev		= pUser->viplev;
	m_stDetailInfo.dwExitTime	= pUser->Exit_Time;
	m_stDetailInfo.nMorality	= pUser->TutorMorality;
}

//////////////////////////////////////////////////////////////////////
bool CTutorData::Create(IRecordset* pRes, OBJID idKey)
{
	CHECKF(pRes);

	m_pData	= CGameTutorData::CreateNew();
	if (m_pData && m_pData->Create(pRes, idKey))
	{
		return true;
	}
	
	SAFE_RELEASE (m_pData);
	return false;
}
//////////////////////////////////////////////////////////////////////
int CTutorData::GetInt(TUTORDATA nDataName)
{
	CHECKF (m_pData);
	
	if (nDataName>TUTORDATA_ADDEDLEV)
	{
		switch(nDataName)
		{
		case TUTORDATA_SEX:			//性别
			return m_stDetailInfo.nSex;
			break;
		case TUTORDATA_LOOKFACE:	//头像
			return m_stDetailInfo.nLookFace;
			break;
		case TUTORDATA_VIPFACE:	//VIP头像
			return m_stDetailInfo.nVipFace;
			break;
		case TUTORDATA_LEV:		//等级
			return m_stDetailInfo.nLev;
			break;
		case TUTORDATA_MAINJOB:	//主职业
			return m_stDetailInfo.nMainJob;
			break;
		case TUTORDATA_MAINLEV:	//主职业等级
			return m_stDetailInfo.nMainLev;
			break;
		case TUTORDATA_SUBJOB:	//副职业
			return m_stDetailInfo.nSubJob;
			break;
		case TUTORDATA_SUBLEV:	//副职业等级
			return m_stDetailInfo.nSubLev;
			break;
		case TUTORDATA_IMPART:	//传授度
			return m_stDetailInfo.nImpart;
			break;
		case TUTORDATA_MAXSCORE:	//战斗力
			return m_stDetailInfo.nMaxScore;
			break;
		case TUTORDATA_VIPLEV:	//VIP等级
			return m_stDetailInfo.nVipLev;
			break;
		case TUTORDATA_EXITTIME:	//离线时间
			return m_stDetailInfo.dwExitTime;
			break;
		case TUTORDATA_MORALITY:	//师德
			return m_stDetailInfo.nMorality;
			break;
		default:
			break;
		}
	}

	return m_pData->GetInt(nDataName);
}

void CTutorData::SetInt(TUTORDATA idx, int nData, bool bUpdate)
{
	CHECK(m_pData);
	if (GetInt(idx)==nData)
		return;
	
	if (idx>TUTORDATA_ADDEDLEV)
	{
		switch(idx)
		{
		case TUTORDATA_SEX:			//性别
			m_stDetailInfo.nSex = nData;
			break;
		case TUTORDATA_LOOKFACE:	//头像
			m_stDetailInfo.nLookFace = nData;
			break;
		case TUTORDATA_VIPFACE:	//VIP头像
			m_stDetailInfo.nVipFace = nData;
			break;
		case TUTORDATA_LEV:		//等级
			m_stDetailInfo.nLev = nData;
			break;
		case TUTORDATA_MAINJOB:	//主职业
			m_stDetailInfo.nMainJob = nData;
			break;
		case TUTORDATA_MAINLEV:	//主职业等级
			m_stDetailInfo.nMainLev = nData;
			break;
		case TUTORDATA_SUBJOB:	//副职业
			m_stDetailInfo.nSubJob = nData;
			break;
		case TUTORDATA_SUBLEV:	//副职业等级
			m_stDetailInfo.nSubLev = nData;
			break;
		case TUTORDATA_IMPART:	//传授度
			m_stDetailInfo.nImpart = nData;
			break;
		case TUTORDATA_MAXSCORE:	//战斗力
			m_stDetailInfo.nMaxScore = nData;
			break;
		case TUTORDATA_VIPLEV:	//VIP等级
			m_stDetailInfo.nVipLev = nData;
			break;
		case TUTORDATA_EXITTIME:	//离线时间
			m_stDetailInfo.dwExitTime = nData;
			break;
		case TUTORDATA_MORALITY:	//师德
			m_stDetailInfo.nMorality = nData;
			break;
		default:
			break;
		}
	}
	else
		m_pData->SetInt(idx, nData);
	
	if (bUpdate)
		m_pData->Update();
}
//////////////////////////////////////////////////////////////////////
LPCTSTR CTutorData::GetStr(TUTORDATA nDataName)
{
	CHECKF (m_pData);
	
	return m_pData->GetStr(nDataName);
}

void CTutorData::SetStr(TUTORDATA idx, LPCTSTR szData, int nSize, bool bUpdate)
{
	CHECK(m_pData);
	if (strcmp(GetStr(idx),szData)==0)
		return;
	m_pData->SetStr(idx, szData, nSize);

	if (bUpdate)
		m_pData->Update();
}
int	CTutorData::GetLookFace()
{
	CHECKF(m_pData);

	if (GetInt(TUTORDATA_VIPFACE)!=0)
		return GetInt(TUTORDATA_VIPFACE);
	else
		return GetInt(TUTORDATA_LOOKFACE);
}
void CTutorData::SetLine(LPCTSTR szLine)
{
	if (!szLine)//不在线上
		m_nLine = 0;
	else if (strcmp(szLine, "一线") == 0)
		m_nLine = 1;
	else if (strcmp(szLine, "二线") == 0)
		m_nLine = 2;
	else if (strcmp(szLine, "三线") == 0)
		m_nLine = 3;
	else if (strcmp(szLine, "四线") == 0)
		m_nLine = 4;
	else if (strcmp(szLine, "五线") == 0)
		m_nLine = 5;
	else if (strcmp(szLine, "六线") == 0)
		m_nLine = 6;
}

void CTutorData::SetTeam(OBJID idTeam)
{
	m_nTeam = idTeam;
}

void CTutorData::SetDetailInfo(const stDetailInfo* pInfo)
{
	CHECK(pInfo);

	memcpy(&m_stDetailInfo, pInfo, sizeof(m_stDetailInfo));
}
//////////////////////////////////////////////////////////////////////
// bool CTutorData::LoadInfo(IRecord* pRes)
// {
// 	if (!pRes) 
// 		return false;
// 
// 	// load again?
// 	if (m_Info.id != ID_NONE)
// 		return true;
// 
// 	m_Info.id				= pRes->LoadDWord("id");
// 	m_Info.idUser			= pRes->LoadDWord("user_id");
// 	m_Info.idTutor			= pRes->LoadDWord("tutor_id");
// 	pRes->LoadString(m_Info.szUserName,	"user_name", _MAX_NAMESIZE);
// 	pRes->LoadString(m_Info.szTutorName, "tutor_name", _MAX_NAMESIZE);
// 	m_Info.nImpart			= pRes->LoadInt("impart");
// 	m_Info.nIsGraduated		= pRes->LoadInt("is_graduated");
// 	m_Info.nHasGift			= pRes->LoadInt("has_gift");
// 	m_Info.nGiftMask		= pRes->LoadDWord("gift_mask");
// 	m_Info.nTutorMask		= pRes->LoadDWord("tutor_mask");
// 	m_Info.nStuLev			= pRes->LoadDWord("stu_lev");
// 	m_Info.nTutorLev		= pRes->LoadDWord("tutor_lev");//师父等级
// 	m_Info.nTutorImpartLv	= pRes->LoadDWord("tutor_lv");//师父师德
// 
// 	return true;
// }

//////////////////////////////////////////////////////////////////////
// OBJID CTutorData::CreateRecord(const TutorInfoStruct* info, IDatabase* pDb)
// {
// 	CHECKF( pDb);
// 	m_Info.idUser		= info->idUser;
// 	m_Info.idTutor		= info->idTutor;
// 	m_Info.nStuLev		= info->nStuLev;
// 	m_Info.nTutorLev	= info->nTutorLev;
// 	m_Info.nTutorImpartLv = info->nTutorImpartLv;
// 	SafeCopy(m_Info.szUserName, info->szUserName, _MAX_NAMESIZE);
// 	SafeCopy(m_Info.szTutorName, info->szTutorName, _MAX_NAMESIZE);
// 
// 	SQLBUF	szSQL;
// 	sprintf(szSQL, "INSERT %s SET user_id='%u', tutor_id='%u', user_name='%s', tutor_name='%s'",
// 		_TBL_TUTOR, info->idUser, info->idTutor, info->szUserName, info->szTutorName);
// 	pDb->ExecuteSQL(szSQL);
// 	m_Info.id	= pDb->GetInsertId();
// 
// 	return m_Info.id;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CTutorData::DeleteRecord(IDatabase* pDb)
// {
// 	if(NULL == pDb)
// 		return false;
// 
// 	SQLBUF	szSQL;
// 	sprintf(szSQL, "DELETE FROM %s WHERE id='%u'", _TBL_TUTOR, m_Info.id);
// 
// 	return pDb->ExecuteSQL(szSQL);
// }
// bool CTutorData::AddImpart(int nImpart, IDatabase* pDb, bool bUpdate)
// {
// 	if(NULL == pDb)
// 		return false;
// 	if (nImpart==0)
// 		return true;
// 	m_Info.nImpart += nImpart;
// 	m_Info.nImpart = max(0,m_Info.nImpart);
// 	if (bUpdate)
// 	{
// 		SQLBUF	szSQL;
// 		sprintf(szSQL, "update %s set impart='%u' where id='%u'", _TBL_TUTOR, m_Info.nImpart, m_Info.id);
// 		
// 		return pDb->ExecuteSQL(szSQL);
// 	}
// 
// 	return true;
// }
// bool CTutorData::SetGiftMask(int nGiftMask, IDatabase* pDb, bool bUpdate)
// {
// 	if(NULL == pDb)
// 		return false;
// 	
// 	m_Info.nGiftMask = nGiftMask;
// 	if (bUpdate)
// 	{
// 		SQLBUF	szSQL;
// 		sprintf(szSQL, "update %s set gift_mask='%u' where id='%u'", _TBL_TUTOR, m_Info.nGiftMask, m_Info.id);
// 		
// 		return pDb->ExecuteSQL(szSQL);
// 	}
// 	
// 	return true;
// }
// bool CTutorData::SetTutorMask(int nGiftMask, IDatabase* pDb, bool bUpdate)
// {
// 	if(NULL == pDb)
// 		return false;
// 	
// 	m_Info.nTutorMask = nGiftMask;
// 	if (bUpdate)
// 	{
// 		SQLBUF	szSQL;
// 		sprintf(szSQL, "update %s set tutor_mask='%u' where id='%u'", _TBL_TUTOR, m_Info.nTutorMask, m_Info.id);
// 		
// 		return pDb->ExecuteSQL(szSQL);
// 	}
// 	
// 	return true;
// }
// bool CTutorData::SetGraduated(int nGraduated, IDatabase* pDb, bool bUpdate)
// {
// 	if(NULL == pDb)
// 		return false;
// 	
// 	m_Info.nIsGraduated = nGraduated;
// 	if (bUpdate)
// 	{
// 		SQLBUF	szSQL;
// 		sprintf(szSQL, "update %s set is_graduated='%u' where id='%u'", _TBL_TUTOR, m_Info.nIsGraduated, m_Info.id);
// 		
// 		return pDb->ExecuteSQL(szSQL);
// 	}
// 	
// 	return true;
// }
// bool CTutorData::SetHasGiftFlag(int nFlag, IDatabase* pDb, bool bUpdate)//设置师父奖励标识
// {
// 	if(NULL == pDb)
// 		return false;
// 	
// 	m_Info.nHasGift = nFlag;
// 	if (bUpdate)
// 	{
// 		SQLBUF	szSQL;
// 		sprintf(szSQL, "update %s set has_gift='%d' where id='%u'", _TBL_TUTOR, m_Info.nHasGift, m_Info.id);
// 		
// 		return pDb->ExecuteSQL(szSQL);
// 	}
// 	
// 	return true;
// }
// bool CTutorData::AddDensity( int nDensity)
// {
// 	m_Info.nDensity += nDensity;
// 	m_Info.nDensity = max(0, m_Info.nDensity);
// 	m_Info.nDensity = min(MAX_DENSITY, m_Info.nDensity);
// 
// 	return true;
// }

