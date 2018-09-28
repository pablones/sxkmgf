// InstanceUseTime.h: interface for the CInstanceUseTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTANCEUSETIME_H__7166C7A8_CF99_4E0C_A1E7_9EEF5DC2E676__INCLUDED_)
#define AFX_INSTANCEUSETIME_H__7166C7A8_CF99_4E0C_A1E7_9EEF5DC2E676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../share/NetWorkDef.h"
#include "../share/protocol.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include <vector>

enum CINSTANCEUSETIMEDATA{
	CInstanceUseTime_ID = 0,
    CInstanceUseTime_USERID,
	CInstanceUseTime_INSTANCETYPE,
	CInstanceUseTime_USETIMES,
	CInstanceUseTime_CREATETIME,
	CInstanceUseTime_CANUSER,
	CInstanceUseTime_FINISHTIMES,
};

char	szInstanceUseTimeTable[];
typedef	CGameData<CINSTANCEUSETIMEDATA, szInstanceUseTimeTable, szID>	CInstanceUseTimeData;

class CInstanceUseTime  
{
public:
	CInstanceUseTime();
	virtual ~CInstanceUseTime();

	static CInstanceUseTime* CreateNew(IDatabase* pDB, PROCESS_ID idProcess);

	bool	Create(IRecordset* pRes);
	bool	Create( const InstanceUseTimeInfo* pInfo, bool bSave);
	ULONG	ReleaseByOwner()				{ delete this; return 0; }

	void    GetInstanceUseInfo(InstanceUseTimeInfo* pInfo);
	void    Destroy();

	void    SetUseTimes(UINT nTimes,char* szTime,bool bClearFinish = false, bool bUpdate=false);
	void    SetUse(int nUse,bool bUpdate =false );
	void    SetFinishTime(int nTimes,bool bUpdate =false );

	OBJID	GetID()	const						  {   return m_pInfo.id; }
	OBJID   GetUserID() const                     {   return m_pInfo.nUserId; }
    UINT    GetInstanceType() const               {   return m_pInfo.nInstanceType; }
	UINT    GetInstanceUseTimes() const           {   return m_pInfo.nUserTime; }
	UINT    GetCanUser() const					  {   return m_pInfo.nCanUser; }
	const char* GetCreateTime() const             {   return m_pInfo.szCreateTime; }
	UINT     GetFinishTime() const				  {   return m_pInfo.nFinishTimes; }

private:
	int		GetInt(CINSTANCEUSETIMEDATA idx)                  { return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(CINSTANCEUSETIMEDATA idx)		   	      { return m_pData->GetStr(idx); }

	IDatabase*               m_pDB;
	CInstanceUseTimeData*    m_pData;
	InstanceUseTimeInfo      m_pInfo;
	PROCESS_ID		         m_idProcess;
};

#endif // !defined(AFX_INSTANCEUSETIME_H__7166C7A8_CF99_4E0C_A1E7_9EEF5DC2E676__INCLUDED_)
