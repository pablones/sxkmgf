// UserJinMeiSys.h: interface for the CUserJinMeiSys class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERJINMEISYS_H__32FD53AC_2C4F_424C_BE21_294532E8A106__INCLUDED_)
#define AFX_USERJINMEISYS_H__32FD53AC_2C4F_424C_BE21_294532E8A106__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)
#include <vector>
#include "UserJinMeiSysData.h"
#include "UserJinMeiTypeData.h"
#include "JinmeiStrengthData.h"
#include "TimeOut.h"

class CUserJinMeiSys  
{
public:
	CUserJinMeiSys(CUser* pUser, IDatabase* pDataBase, IRecordset* pDefJinmeiRes);
	~CUserJinMeiSys();
	void Release() { delete this; }

public:
	void OnLogin();
	void OnLogOut();
	void OnTimer(time_t tCurr);
    
	bool UpJinmeiLev(int nUpLevType, int nType);
	bool DelJinmeiType(int nType);
    
	bool StrengthUserJinmei(int nType, bool bProtect);
	bool StrengthJinmei(int nType, bool bProtect, int& nFailStrengLev);    
	void StartLearnJinmei(int nType);
	bool StartLearn(int nType, int& nNeedTime, int& nAddTime);
	void AddJinmeiWaitQueue(int nType);
	bool AddWaitLearnQueue(int nType, int& nAddtime, int& nNeedTime);
	void StopLearn(int nType);
	void MoveWaitQueue(int nType);

	void SaveInfo();
	void SendJinmeiSysInfo(CUser* pTargetUser);
	void SendLoginOKInfo();

	int GetJinmeiTypeValue(int nType);
	int GetJinmeiIncDamageRate();
	int GetJinmeiDecDamageRate();
	int GetJinmeiDecManaRate();

	void UpdateChange();
	bool AddJinmeiForChange(IRecordset* pDefaultRecord,JinmeiInfoStruct* pInfo);
	CUserJinMeiSysData* GetJinmeiDataByType(int nType);
protected:
	CJinmeiTypeData* GetDataByTypeAndLev(int nType, int nLev);
	
	CJinmeiStrengthData* GetJinmeiStrengthDataByLev(int nLev);
	CJinmeiEffectData*  GetJinmeiEffectData(int nEffectType);

	void UpdateJinmeiLimitLev();
	void UpdateJinmeiStrengthLev();
	void ProcessWaitToLearn();
	bool CheckLearnCondition(CUserJinMeiSysData* pSysData);
	bool CheckLearnNewConditon(int nType);
	int CheckUpJMLevForLogin( CUserJinMeiSysData* pSysData, int& nLeaveTime ,int nType = 1);
	int CheckUpJinmeiLevConditon(CUserJinMeiSysData* pSysData , int nUpLevType = UPJINMEILEV_TIME);
	bool UpJinmeiLev(CUserJinMeiSysData* pSysData);

private:
	typedef std::vector<CUserJinMeiSysData*> VEC_USERJINMEIDATA;
	VEC_USERJINMEIDATA m_vecUserJinmeiData;

	typedef std::vector<int> VEC_JINMEITYPE;
	VEC_JINMEITYPE m_vecCompleteType;
	VEC_JINMEITYPE m_vecStartType;

	VEC_USERJINMEIDATA m_vecWaitUserJinmeiData;    // 等待修炼队列
	VEC_USERJINMEIDATA m_vecProcessUserJinmeiData; // 进行中的修炼队列 

	int   m_nLimitJinmeiLev; //所有经脉的最低等级
	int   m_nLimitJinmeiStrengthLev;//所有经脉强化最低等级
	int   m_nMaxLearnNum;    //同时允许修炼的数目
	int   m_nMaxWaitNum;     //同时允许等待的数目
	int   m_nCompleteNum;
	int   m_nLeaveTime;
	bool  m_bNeedUpdate;
	bool  m_bFirstLogin;
	CUser* m_pUser;
	IDatabase* m_pDb;
	IRecordset* m_pDefRes;
};

class UserJinmeiSysDataSort
{
public:
	bool operator() (CUserJinMeiSysData* data1, CUserJinMeiSysData* data2)
	{
		return data1->GetInt(USERJINMEISYSDATA_ADDQUEUETIME) < data2->GetInt(USERJINMEISYSDATA_ADDQUEUETIME);
	}
};

#endif // !defined(AFX_USERJINMEISYS_H__32FD53AC_2C4F_424C_BE21_294532E8A106__INCLUDED_)
