// TutorList.cpp: implementation of the CTutorList class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdtctl.h>
#include "TutorList.h"
#include "MapGroup.h"
#include "Network/MsgSchoolMember.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTutorList	g_TutorListSystem;

CTutorList::CTutorList()
{
	m_pDb = NULL;
	m_TutorListUpMin = RandGet(60,true);
	
	m_TutorUpdateCompleteEvent = CreateEvent( NULL,true,true,NULL );//句柄不被继承,手动复位,初始为置位状态,不指定事件名
	
	Init();
}

CTutorList::~CTutorList()
{
	CloseHandle(m_TutorUpdateCompleteEvent);
	m_vTutorList.clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTutorList::TimeFromExit(TUTORLIST tutor1, TUTORLIST tutor2)
{
	if (tutor1.dwLastLogoutTime == 0 && tutor2.dwLastLogoutTime != 0)
		return true;
	else if (tutor1.dwLastLogoutTime != 0 && tutor2.dwLastLogoutTime == 0)
		return false;
	else if (tutor1.dwLastLogoutTime == 0 && tutor2.dwLastLogoutTime == 0)
		return tutor1.dwImpart > tutor2.dwImpart;
	else if (tutor1.dwLastLogoutTime != 0 && tutor2.dwLastLogoutTime != 0)
		return tutor1.dwLastLogoutTime > tutor2.dwLastLogoutTime;
	else
		return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTutorList::Init()			//清空数据
{
	m_vTutorList.clear();
	memset( &m_LastUpdateTime,0L,sizeof( SYSTEMTIME ) );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTutorList::CreateData( IDatabase* pDb )
{
	if( !pDb )
		return false;
	m_pDb = pDb;

	char szSQL[1024] = "";

	//Tutor_Mask 字段的二进制第1位标识玩家是否登记了师父列表
	sprintf( szSQL, "SELECT id,name,sex,level,Fir_job,max_score,Tutor_Lv,vipstate,viplev,Exit_Time FROM %s WHERE Tutor_Mask&1>0 ORDER BY Exit_Time DESC LIMIT %d",_TBL_USER, _TUTOR_LIST_NUM);
	IRecordset* pRes = pDb->CreateNewRecordset( szSQL, false );
	if( pRes )
	{
		TUTORLIST tutorList = {0};
		for( int i = 0; i < pRes->RecordCount(); i++ )
		{
			LoadInfo(pRes, &tutorList, true);
			if (tutorList.dwLastLogoutTime!=0 && GetTutorLeaveHours(tutorList.dwLastLogoutTime) > _TUTOR_CLEAR_HOURS)//长时间不上线的，从列表里清除
			{
				pRes->MoveNext();
				continue;
			}
			m_vTutorList.push_back(tutorList);
			pRes->MoveNext();
		}
		pRes->Release();  
	}

	::GetLocalTime( &m_LastUpdateTime );//启动服务器时更新数据
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CTutorList::LoadInfo(IRecordset* pRes, TUTORLIST* listInfo, bool bCreate)//bCreate=false表示列表刷新，反之是创建
{
	CHECK(pRes);

	listInfo->idUser = pRes->LoadDWord("id");
	listInfo->ucGender = pRes->LoadInt("sex");
	int nVipState = pRes->LoadInt("vipstate");
	if(nVipState>0)
		listInfo->ucVipLev = pRes->LoadInt("viplev");
	else
		listInfo->ucVipLev = 0;
	listInfo->usLev    = pRes->LoadInt("level");
	listInfo->dwProfession = pRes->LoadDWord("Fir_job");
	listInfo->dwBattleNum  = pRes->LoadDWord("max_score");
	listInfo->dwImpart	   = pRes->LoadDWord("Tutor_Lv");
	//listInfo->dwMorality   = pRes->LoadDWord("Tutor_Morality");
	if (bCreate==false && listInfo->dwLastLogoutTime==0)//刷新列表，要保留玩家在线状态
		listInfo->dwLastLogoutTime = 0;
	else
		listInfo->dwLastLogoutTime = pRes->LoadDWord("Exit_Time");
	pRes->LoadString( listInfo->szName,"name", _MAX_NAMESIZE );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
TUTORLIST* CTutorList::FindTutorByID(OBJID idUser)
{
	CHECKF(idUser);

	for (int i=0; i<m_vTutorList.size(); i++)
	{
		if (m_vTutorList[i].idUser==idUser)
			return &m_vTutorList[i];
	}
	return NULL;
}
TUTORLIST* CTutorList::FindTutorByName(NAMESTR szName)
{
	CHECKF(szName);

	for (int i=0; i<m_vTutorList.size(); i++)
	{
		if(strcmp(m_vTutorList[i].szName, szName) == 0)
			return &m_vTutorList[i];
	}
	return NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//每天6点更新师徒列表数据
void CTutorList::OnTimer( time_t tCurr )
{	
	SYSTEMTIME curTime;
	::GetLocalTime( &curTime );
	if (curTime.wHour!=6)
		return;
	if( curTime.wDay != m_LastUpdateTime.wDay)//今天还没有更新过
	{
		if(m_TutorListUpMin != curTime.wMinute)
			return;
		
		ResetEvent( m_TutorUpdateCompleteEvent );//正在更新,事件为复位状态
		//Init();不能清空原来的列表，要保留玩家的在线状态
		memset( &m_LastUpdateTime,0L,sizeof( SYSTEMTIME ) );
		UpdateListData();
		SetEvent( m_TutorUpdateCompleteEvent );
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CTutorList::UpdateListData()
{
	if( !m_pDb )
		return;
	
// 	char szSQL[1024] = "";
// 	
// 	//等级排行
// 	sprintf( szSQL, "SELECT id,name,sex,level,Fir_job,max_score,Tutor_Lv,vipstate,viplev,Exit_Time FROM %s WHERE Tutor_Mask&1>0 ORDER BY Exit_Time DESC LIMIT %d",_TBL_USER, _TUTOR_LIST_NUM);
// 	IRecordset* pRes = m_pDb->CreateNewRecordset( szSQL, false );
// 	if( pRes )
// 	{
// 		OBJID idTemp = 0;
// 		TUTORLIST* pTutorList = NULL;
// 		TUTORLIST tutorList = {0};
// 		DWORD dwLastExitTime = 0;
// 		for( int i = 0; i < pRes->RecordCount(); i++ )
// 		{
// 			idTemp = pRes->LoadDWord("id");
// 			pTutorList = FindTutorByID(idTemp);
// 			if (pTutorList)//已经在列表中的玩家,load的时候要看是不是在线
// 			{
// 				LoadInfo(pRes, pTutorList, false);
// 				dwLastExitTime = pTutorList->dwLastLogoutTime;
// 			}
// 			else
// 			{
// 				LoadInfo(pRes, &tutorList, true);
// 				dwLastExitTime = tutorList.dwLastLogoutTime;
// 			}
// 
// 			if (dwLastExitTime!=0 && GetTutorLeaveHours(dwLastExitTime) > _TUTOR_CLEAR_HOURS)//长时间不上线的，从列表里清除
// 			{
// 				if (pTutorList)
// 					DeleteTutorFromList(pTutorList->idUser);
// 				pRes->MoveNext();
// 				continue;
// 			}
// 			if (!pTutorList)//已经在列表中的不用再加入
// 				m_vTutorList.push_back(tutorList);
// 			pRes->MoveNext();
// 		}
// 		pRes->Release();  
// 	}
	int nStep = 0;
	DEBUG_TRY
//	CUser* pUser = NULL;
	nStep = 1;
	for (int i=m_vTutorList.size()-1; i>=0; i--)
	{
		nStep = 2;
		//不更新信息，只在用户玩家上下线时更新，以免线路之间信息不一致
//		pUser = ::MapGroup(MSGPORT_MAPGROUP_FIRST)->GetUserManager()->GetUser(m_vTutorList[i].idUser);
		nStep = 3;
// 		if(pUser)
// 		{
// 			nStep = 4;
// 			m_vTutorList[i].ucVipLev = pUser->IsVipActive() ? pUser->GetVipLev() : 0;
// 			m_vTutorList[i].usLev = pUser->GetLev();
// 			m_vTutorList[i].dwBattleNum = pUser->GetMaxScore();
// 			m_vTutorList[i].dwImpart = pUser->GetImpart();
// 			m_vTutorList[i].dwLastLogoutTime = 0;
// 		}
// 		else
		{
			nStep = 5;
			if (m_vTutorList[i].dwLastLogoutTime!=0 && GetTutorLeaveHours(m_vTutorList[i].dwLastLogoutTime) > _TUTOR_CLEAR_HOURS)
			{
				nStep = 6;
				DeleteTutorFromList(m_vTutorList[i].idUser);
			}
		}
	}
	DEBUG_CATCH2("CTutorList::UpdateListData() nStep = %u", nStep)
	::GetLocalTime( &m_LastUpdateTime );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int CTutorList::GetInfo( TUTORLIST* pData,int nPageSize, USHORT nPage )
{
	CHECKF(pData);
	CHECKF(nPageSize>0 && nPageSize<=20);//每页最多只允许20个
	int nRetAmount = 0;
	DEBUG_TRY
	int nRet = ::WaitForSingleObject( m_TutorUpdateCompleteEvent,1000 );
	if( WAIT_TIMEOUT == nRet )
	{
		return -1;
	}

	int nIndex = 0;
	for(int i = 0; i < nPageSize; i++)
	{
		nIndex = (nPage-1)*nPageSize + i;
		if (nIndex >= m_vTutorList.size())
			break;

		pData[i].idUser = m_vTutorList[nIndex].idUser;
		pData[i].ucGender = m_vTutorList[nIndex].ucGender;
		pData[i].ucVipLev = m_vTutorList[nIndex].ucVipLev;
		pData[i].usLev = m_vTutorList[nIndex].usLev;
		pData[i].dwProfession = m_vTutorList[nIndex].dwProfession;
		pData[i].dwBattleNum = m_vTutorList[nIndex].dwBattleNum;
		pData[i].dwImpart = m_vTutorList[nIndex].dwImpart;
		//pData[i].dwMorality = m_vTutorList[nIndex].dwMorality;
		pData[i].dwLastLogoutTime = m_vTutorList[nIndex].dwLastLogoutTime;
		SafeCopy(pData[i].szName, m_vTutorList[nIndex].szName);
		nRetAmount++;
	}
	DEBUG_CATCH("CTutorList::GetInfo");
	return nRetAmount;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//列表中的某个玩家上线了
bool CTutorList::TutorLogin( TUTORLIST* info )
{
	CHECKF(info);
	TUTORLIST* pList = FindTutorByID(info->idUser);
	if (!pList)
		return false;

	pList->ucVipLev = info->ucVipLev;
	pList->usLev = info->usLev;
	pList->dwBattleNum = info->dwBattleNum;
	pList->dwImpart = info->dwImpart;
	pList->dwLastLogoutTime = 0;
	pList->uIPAddress = info->uIPAddress;
	sort(m_vTutorList.begin(), m_vTutorList.end(), CTutorList::TimeFromExit);

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//列表中的某个玩家下线了
bool CTutorList::TutorLogout( TUTORLIST* info )
{
	CHECKF(info);
	TUTORLIST* pList = FindTutorByID(info->idUser);
	if (!pList)
		return false;

	pList->ucVipLev = info->ucVipLev;
	pList->usLev = info->usLev;
	pList->dwBattleNum = info->dwBattleNum;
	pList->dwImpart = info->dwImpart;
	pList->dwLastLogoutTime = info->dwLastLogoutTime;
	pList->uIPAddress = 0;
	sort(m_vTutorList.begin(), m_vTutorList.end(), CTutorList::TimeFromExit);

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CTutorList::AddTutorToList(TUTORLIST* info)
{
	CHECK(info);
	if (FindTutorByID(info->idUser))
		return;
	if (info->dwProfession != MALE_ASSIST
		&& info->dwProfession != MALE_MASTER
		&& info->dwProfession != MALE_PASTOR
		&& info->dwProfession != MALE_ASSISSIN
		&& info->dwProfession != MALE_SOLDIER
		&& info->dwProfession != MALE_SWORD
		&& info->dwProfession != MALE_NEWHAND)//检查职业是否有问题
	{
		::LogSave("CTutorList::AddTutorToList job error!job=%d", info->dwProfession);
		return;
	}
	if (info->ucGender != 0
		&& info->ucGender != 1)//检查性别
	{
		::LogSave("CTutorList::AddTutorToList sex error!sex=%d", info->ucGender);
		return;
	}
	TUTORLIST temp = {0};
	memcpy(&temp, info, sizeof(TUTORLIST));
	m_vTutorList.push_back(temp);
	sort(m_vTutorList.begin(), m_vTutorList.end(), CTutorList::TimeFromExit);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CTutorList::DeleteTutorFromList(OBJID idUser)
{
	vector<TUTORLIST>::iterator it = m_vTutorList.begin();
	for (; it!=m_vTutorList.end(); it++)
	{
		if (it->idUser==idUser)
		{
			m_vTutorList.erase(it);
			return;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//获得玩家有多少个小时没有登陆过了
int CTutorList::GetTutorLeaveHours( DWORD dwExitTime )
{
	CHECKF(dwExitTime);
	int nYear = 2000 + dwExitTime/100000000;
	int nMonth = (dwExitTime/1000000) % 100;
	int nDay = (dwExitTime/10000) % 100;
	int nHour = (dwExitTime/100) % 100;
	int nMin = dwExitTime % 100;

	char szExitTime[32]="";
	sprintf(szExitTime, "%d-%d-%d %d:%d:00", nYear, nMonth, nDay, nHour, nMin);
	COleDateTime exitDate;
	if (!exitDate.ParseDateTime(szExitTime))
	{
		return 0;
	}
	COleDateTimeSpan passDate = COleDateTime::GetCurrentTime() - exitDate;
	double nPassTime = passDate.GetTotalHours();
	return nPassTime;
}
//查询查询玩家的徒弟以及徒弟申请列表是否已满
int CTutorList::GetTutorApplyAmount(OBJID idTutor, int nImpart)
{
	if (idTutor==ID_NONE)
		return 0;
	char szSQL[1024] = "";
	int nApplyAmount = 0, nStuAmount = 0;

	sprintf( szSQL, "SELECT state FROM %s WHERE tutor_id=%u",_TBL_TUTOR,idTutor);
	IRecordset* pRes = m_pDb->CreateNewRecordset( szSQL, false );
	if( pRes )
	{
		int nState = 0;
		for( int i = 0; i < pRes->RecordCount(); i++ )
		{
			nState = pRes->GetInt(0);
			if (nState==TUTORSTATE_NORMAL || nState==TUTORSTATE_AGREED)
				nStuAmount++;
			else if (nState==TUTORSTATE_APPLY || nState==TUTORSTATE_APPLYREF)
				nApplyAmount++;
			pRes->MoveNext();
		}
		pRes->Release();  
	}
	if (nApplyAmount>=20)
		return 2;
	else if (nStuAmount>=CUserTutor::GetMaxStuAmount(nImpart))
		return 1;
	return 0;
}
bool CTutorList::HasTutor(OBJID idUser)
{
	if (idUser==ID_NONE)
		return 0;
	char szSQL[1024] = "";

	sprintf( szSQL, "SELECT state FROM %s WHERE stu_id=%u",_TBL_TUTOR,idUser);
	IRecordset* pRes = m_pDb->CreateNewRecordset( szSQL, false );
	if( pRes )
	{
		int nState = 0;
		for( int i = 0; i < pRes->RecordCount(); i++ )
		{
			nState = pRes->GetInt(0);
			if (nState==TUTORSTATE_NORMAL || nState==TUTORSTATE_AGREED)
			{
				pRes->Release();
				return true;
			}
			pRes->MoveNext();
		}
		pRes->Release();
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
//改名
void CTutorList::SetTutorListNameInfo(OBJID idUser,LPCTSTR szName)//改名
{
	TUTORLIST* pInfo= FindTutorByID(idUser);
	if(!pInfo)
		return;
	SafeCopy(pInfo->szName,szName,_MAX_NAMESIZE);
}