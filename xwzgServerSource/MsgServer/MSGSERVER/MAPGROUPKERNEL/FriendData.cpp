// FriendData.cpp: implementation of the CFriendData class.
//
//////////////////////////////////////////////////////////////////////

#include "FriendData.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFriendData::CFriendData()
{
	memset(&m_Info, 0, sizeof(FriendInfoStruct));
	SetObjType(OBJ_FRIEND);
}

//////////////////////////////////////////////////////////////////////
CFriendData::~CFriendData()
{
}

bool CFriendData::SetFriendShip(OBJID id, OBJID idFriend, IDatabase* pDb,int nFriendShip)
{
	CHECKF(id);
	CHECKF(idFriend);
	
	m_Info.nFriendShip = nFriendShip;

	SQLBUF	szSQL;
	sprintf(szSQL, "UPDATE %s SET friendship=%d WHERE userid=%u AND friendid=%u LIMIT 1", _TBL_FRIEND,m_Info.nFriendShip, id, idFriend);
	if (pDb->ExecuteSQL(szSQL))
		return true;
	else
		::LogSave("CFriendData::SetFriendShip faled!sql=%s",szSQL);
	return false;
}

bool CFriendData::Create(OBJID id, CUser* pTarget, LPCTSTR szMyName)
{
	CHECKF(id);
	CHECKF(pTarget);
	CHECKF(szMyName);
	
	m_Info.dwLookface		= pTarget->GetLookFace();
	m_Info.usLevel			= pTarget->GetLev();
	m_Info.FirJob			= pTarget->GetProfession(1);
	m_Info.FirJobLev		= pTarget->GetJobLev(1);
	m_Info.SecJob			= pTarget->GetProfession(2);
	m_Info.SecJobLev		= pTarget->GetJobLev(2);
	m_Info.SynCon			= pTarget->GetSynID();
	m_Info.iUseTitle		= pTarget->GetUseTitle();
	m_Info.iRelation		= 0;
	SafeCopy(m_Info.szFeel, pTarget->GetFeel(), _MAX_NAMESIZE);
	SafeCopy(m_Info.szUserName, szMyName, _MAX_NAMESIZE);
	
	return true;
}

bool CFriendData::Create(OBJID id, stTempUser* pTarget, LPCTSTR szMyName)
{
	CHECKF(id);
	CHECKF(pTarget);
	CHECKF(szMyName);
	
	m_Info.dwLookface		= pTarget->lookface;
	m_Info.usLevel			= pTarget->level;
	m_Info.FirJob			= pTarget->Fir_Job;
	m_Info.FirJobLev		= pTarget->Fir_JobLv;
	m_Info.SecJob			= pTarget->Sec_Job;
	m_Info.SecJobLev		= pTarget->Sec_JobLv;
	m_Info.SynCon			= pTarget->Syn_ID;
	m_Info.iUseTitle		= pTarget->use_title;
	m_Info.iRelation		= 0;
	strcpy(m_Info.szFeel,	pTarget->Feel);
	strcpy(m_Info.szUserName, szMyName);
	
	return true;
}

bool CFriendData::Create(IRecordset* pRes, CUser* pUser, LPCTSTR szMyName)
{
	CHECKF(pRes);
	CHECKF(pUser);
	CHECKF(szMyName);

	m_Info.dwLookface		= pUser->GetLookFace();
	m_Info.usLevel			= pUser->GetLev();
	m_Info.FirJob			= pUser->GetProfession(1);
	m_Info.FirJobLev		= pUser->GetJobLev(1);
	m_Info.SecJob			= pUser->GetProfession(2);
	m_Info.SecJobLev		= pUser->GetJobLev(2);
	m_Info.SynCon			= pUser->GetSynID();
	m_Info.iUseTitle		= pUser->GetUseTitle();

	strcpy(m_Info.szFeel,	pUser->GetFeel());
	strcpy(m_Info.szUserName, szMyName);
	IRecord* pRecord = pRes->CreateNewRecord();
	bool ret = LoadInfo(pRecord);
	pRecord->Release();
	return ret;
}

bool CFriendData::Create(IRecordset* pRes, stTempUser* pUser, LPCTSTR szMyName)
{
	CHECKF(pRes);
	CHECKF(pUser);
	CHECKF(szMyName);
	
	m_Info.dwLookface		= pUser->lookface;
	m_Info.usLevel			= pUser->level;
	m_Info.FirJob			= pUser->Fir_Job;
	m_Info.FirJobLev		= pUser->Fir_JobLv;
	m_Info.SecJob			= pUser->Sec_Job;
	m_Info.SecJobLev		= pUser->Sec_JobLv;
	m_Info.SynCon			= pUser->Syn_ID;
	m_Info.iUseTitle		= pUser->use_title;
	
	strcpy(m_Info.szFeel,	pUser->Feel);
	strcpy(m_Info.szUserName, szMyName);
	IRecord* pRecord = pRes->CreateNewRecord();
	bool ret = LoadInfo(pRecord);
	pRecord->Release();
	return ret;
}

//////////////////////////////////////////////////////////////////////
bool CFriendData::LoadInfo(IRecord* pRes)
{
	if (!pRes) 
		return false;

	// load again? 允许load again,新加的好友需要load
// 	if (m_Info.id != ID_NONE)
// 		return true;

	m_Info.id				= pRes->LoadDWord("id");
	m_Info.idUser			= pRes->LoadDWord("userid");
	m_Info.idFriend			= pRes->LoadDWord("friendid");
	m_Info.nFriendType		= pRes->LoadUInt("friendtype");
	m_Info.nFriendShip		= pRes->LoadDWord("friendship");//[2010-06-07 goto]友好度
	pRes->LoadString(m_Info.szFriendName,	"friendname", _MAX_NAMESIZE);
	pRes->LoadString(m_Info.szMateName,	"friendmatename", _MAX_NAMESIZE);
	m_Info.iRelation		= pRes->LoadUInt("friendrelation");
	

	return true;
}

//////////////////////////////////////////////////////////////////////////
//[2010-06-07 goto]增加参数友好度
void CFriendData::SetRecord(OBJID id, OBJID idUser, OBJID idFriend, LPCTSTR szFriendName,
							USHORT nFriendType,LPCTSTR szUserName, UINT nFriendShip , LPCTSTR szFriendMateName)
{
	if (!szFriendName || strlen(szFriendName)==0 || !szFriendMateName || strlen(szFriendMateName)==0)
		return;
	m_Info.id			= id;
	m_Info.idUser		= idUser;
	m_Info.idFriend		= idFriend;
	m_Info.nFriendType	= nFriendType;
	m_Info.nFriendShip	= nFriendShip;
	SafeCopy(m_Info.szFriendName, szFriendName, _MAX_NAMESIZE);
	SafeCopy(m_Info.szMateName, szFriendMateName, _MAX_NAMESIZE);
	SafeCopy(m_Info.szUserName, szUserName, _MAX_NAMESIZE);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//[2010-06-07 goto]增加参数友好度
OBJID CFriendData::CreateRecord(OBJID idUser, OBJID idFriend, LPCTSTR szFriendName, IDatabase* pDb,USHORT nFriendType, UINT nFriendShip ,LPCTSTR szFriendMateName)
{
	if (!szFriendName || strlen(szFriendName)==0)
		return 0;

	char szMateName[_MAX_NAMESIZE] = "无";
	if (szFriendMateName && strlen(szFriendMateName)>0)
		sprintf(szMateName,szFriendMateName);

	m_Info.idUser		= idUser;
	m_Info.idFriend		= idFriend;
	m_Info.nFriendType	= nFriendType;
	m_Info.nFriendShip	= nFriendShip;
	SafeCopy(m_Info.szFriendName, szFriendName, _MAX_NAMESIZE);
	SafeCopy(m_Info.szMateName, szFriendMateName, _MAX_NAMESIZE);

	SQLBUF	szSQL;
	sprintf(szSQL, "INSERT %s SET userid='%u',friendid='%u',friendtype='%u',friendname='%s',friendship='%u',friendmatename='%s'",
			_TBL_FRIEND, idUser, idFriend, nFriendType,szFriendName,nFriendShip,szMateName);
	pDb->ExecuteSQL(szSQL);
	m_Info.id = pDb->GetInsertId();

	return m_Info.id;
}

//////////////////////////////////////////////////////////////////////
bool CFriendData::DeleteRecord(IDatabase* pDb)
{
	SQLBUF	szSQL;
	//sprintf(szSQL, "DELETE FROM %s WHERE id='%u'", _TBL_FRIEND, m_Info.id);//新加的好友可能不知道数据库自动生成的ID是多少
	sprintf(szSQL, "DELETE FROM %s WHERE userid='%u' AND friendid='%u' AND friendname='%s'", _TBL_FRIEND, m_Info.idUser, m_Info.idFriend, m_Info.szFriendName);
	pDb->ExecuteSQL(szSQL);

	return false;
}

void CFriendData::SetFriendFace(int nFace)
{
	m_Info.dwLookface = nFace;
	return;
}
