// Rename.cpp: implementation of the CRename class.
//
//////////////////////////////////////////////////////////////////////

#include "Rename.h"
#include "MapGroup.h"
#include "UserManager.h"
#include "UserTutor.h"
#include "VipList.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CTutorList g_TutorListSystem;
extern CVipList  g_VipListMgr;
CRename::CRename(PROCESS_ID idProcess)
{
	m_idProcess = idProcess;
}

CRename::~CRename()
{
// 	if(m_setData)
// 		m_setData->ReleaseByOwner();
}
// login
//////////////////////////////////////////////////////////////////////
bool CRename::Create()
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	char szSQL[256]="";
	sprintf(szSQL,"SELECT * FROM %s",_TBL_CHANGENAME);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL);	
	if(pRes)
	{
		for (int i = 0; i < pRes->RecordCount(); i++,pRes->MoveNext())
		{
			m_Canrename.push_back(pRes->GetInt(0));
		}
		pRes->Release();
		return true;
	}
	return false;
}
bool CRename::ISCanRename(OBJID nUserId)
{
	if(nUserId==0)
		return false;

	list<OBJID>::iterator it = m_Canrename.begin(); 
	for(it;it != m_Canrename.end();it++)
	{
		OBJID temp=*it;
		if (*it==nUserId)
		{
			return true;
		}
	}
	return false;
}

bool CRename::Rename(OBJID nUserId,const char *szName,int nType)//0是个人，1是帮派
{
	if (ISCanRename(nUserId))
	{
		if (nType==0)
		{
			SQLBUF sql;
			sprintf(sql,"SELECT * FROM %s WHERE name = '%s' LIMIT 1",_TBL_USER,szName);
			IRecordset *pUerset=Database()->CreateNewRecordset(sql);
			if (!pUerset)
			{
				sprintf(sql,"UPDATE %s SET name = '%s' WHERE id = %d LIMIT 1",_TBL_USER,szName,nUserId);
				Database()->ExecuteSQL(sql);
				ReSetMapName(szName,nUserId);
				ReSetSynMemberName(szName,nUserId);
				ReSetVipListName(szName,nUserId);
				ReSetFriendName(szName,nUserId);
				DeleteRecord(nUserId);
				return true;
			}
			pUerset->Release();
			return false;
		}
		if (nType==1)
		{
			SQLBUF sql;
			sprintf(sql,"SELECT * FROM %s WHERE name = '%s' LIMIT 1",_TBL_SYNDICATE,szName);
			IRecordset *pUerset=Database()->CreateNewRecordset(sql);
			if (!pUerset)
			{
				sprintf(sql,"UPDATE %s SET name = '%s' WHERE id = %d LIMIT 1",_TBL_SYNDICATE,szName,nUserId);
				Database()->ExecuteSQL(sql);
				ReSetSynName(szName,nUserId);
				DeleteRecord(nUserId);
				return true;
			}
			pUerset->Release();
			return false;
		}
	}
	return false;
}
void CRename::DeleteRecord(OBJID nUserId,bool exSql)
{
	if (exSql)
	{
		SQLBUF sql;
		sprintf(sql,"DELETE FROM %s WHERE id = %d",_TBL_CHANGENAME,nUserId);
		Database()->ExecuteSQL(sql);
	}
	list<OBJID>::iterator it = m_Canrename.begin(); 
	for(it;it!= m_Canrename.end();it++)
	{
		if (*it==nUserId)
		{
			m_Canrename.erase(it);
			return;
		}
	}
}
void CRename::ReSetMapName(const char*szName,int nId)
{
	UserManager()->ReSetMapName(szName,nId);
}
void CRename::ReSetTutorListName(CUser* pUser,const char*szName,int nId)
{
	g_TutorListSystem.SetTutorListNameInfo(nId,szName);
	if (pUser)
	{
		if(pUser->GetTutorManager())
		{
			pUser->GetTutorManager()->ResetStudentName(szName);
			pUser->GetTutorManager()->ResetTutorName(szName);
		}
	}
}

void CRename::ReSetSynName(const char* szName,OBJID nSynid)
{
	CSyndicate *psyn =SynManager()->QuerySyndicate(nSynid);
	if (psyn)
		psyn->ReSetSynName(szName);
}
void CRename::ReSetSynMemberName(const char* szName,OBJID nUserid)
{
	CUserPtr pUser=UserManager()->GetUser(nUserid);
	if (pUser)
	{
		CSyndicate *psyn =pUser->GetSyndicate();
		if (psyn)
			psyn->ReSetSynMemberName(nUserid,szName);
	}
}
void CRename::ReSetFriendName(const char* szName,OBJID nUserid)
{
	SQLBUF	szSQL;
	sprintf(szSQL, "UPDATE %s SET friendname ='%s' WHERE friendid=%d LIMIT 100", _TBL_FRIEND,szName,nUserid);
	Database()->ExecuteSQL(szSQL);
}
void CRename::ReSetVipListName(const char* szName,OBJID nUserid)
{
	ST_VIPLIST *sViplist=g_VipListMgr.GetVipUser(nUserid);
	if (sViplist)
		SafeCopy(sViplist->szName,szName,_MAX_NAMESIZE);
}