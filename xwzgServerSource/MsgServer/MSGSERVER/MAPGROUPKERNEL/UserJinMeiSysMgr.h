// UserJinMeiSysMgr.h: interface for the CUserJinMeiSysMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERJINMEISYSMGR_H__6D2051B7_0592_4D65_BA9F_A2FB86874EB5__INCLUDED_)
#define AFX_USERJINMEISYSMGR_H__6D2051B7_0592_4D65_BA9F_A2FB86874EB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)
#include <map>
#include "UserJinMeiSys.h"
#include "Timeout.h"

class CUser;

class CUserJinMeiSysMgr  
{
public:
	CUserJinMeiSysMgr();
	virtual ~CUserJinMeiSysMgr();

public:
	bool Init(PROCESS_ID idProcess, IDatabase* pDataBase, IRecordset* pDefJinmeiRes);
	void OnTimer(time_t tCurr);
	void OnUserLogin(CUser* pUser,bool bLogin);
	void OnUserLogOut(OBJID idUser);
	void SendJinmeiSysInfo(OBJID idUser, CUser* pTargetUser);
	void SendLoginOKInfo(OBJID idUser);

	int  GetJinMeiValue(int nType, OBJID idUser);
	int  GetJinmeiIncDamageRate(OBJID idUser);
	int  GetJinmeiDecDamageRate(OBJID idUser);
	int  GetJinmeiDecManaRate(OBJID idUser);
	void StartLearnJinmei(int nType, OBJID idUser);
	void StopLearnJinmei(int nType, OBJID idUser);
	bool UpJinmeiLevByType(int nType, int nUpLevType, OBJID idUser);
	bool StrengthJinmeiSys(int nType, bool bProtect, OBJID idUser);
	void MoveWaitJinmeiType(int nType, OBJID idUser);
	void AddWaitJinmeiType(int nType, OBJID idUser);

	void SendAllObjInfo	(CUser* pUser,PROCESS_ID idProcess);
	bool AddJinmeiForChange(CUser* pUser,JinmeiInfoStruct* pInfo);
	bool UpdateChange(CUser* pUser);

private:
	PROCESS_ID m_idProcess;
	CTimeOut   m_tLearnJinmei;
	IDatabase* m_pDb;
	IRecordset* m_pDefRes;

	typedef std::map<OBJID, CUserJinMeiSys*> MAP_USERJINMEISYS;
	MAP_USERJINMEISYS m_mapUserJinmeiSys;

};

#endif // !defined(AFX_USERJINMEISYSMGR_H__6D2051B7_0592_4D65_BA9F_A2FB86874EB5__INCLUDED_)
