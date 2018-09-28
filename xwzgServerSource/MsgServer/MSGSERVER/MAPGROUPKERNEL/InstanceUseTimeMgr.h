// InstanceUseTimeMgr.h: interface for the CInstanceUseTimeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTANCEUSETIMEMGR_H__ED36B899_0E9A_4A73_975F_ACFA2BD15C8E__INCLUDED_)
#define AFX_INSTANCEUSETIMEMGR_H__ED36B899_0E9A_4A73_975F_ACFA2BD15C8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObjSet.h"
#include "InstanceUseTime.h"

class CInstanceUseTimeMgr  
{
public:
	CInstanceUseTimeMgr();
	virtual ~CInstanceUseTimeMgr();

	static CInstanceUseTimeMgr* CreateNew() { return new CInstanceUseTimeMgr; }
	ULONG    	Release		    	()				{ delete this; return 0; }
	void		ClearInstanceUseSet();

	bool    	Create			    (IDatabase* pDb, PROCESS_ID idProcess);

	bool		UserLogin(OBJID idUser);
	bool		UserLogOut(OBJID idUser);

	bool        AddNewInstanceUse   (OBJID idUser, UINT nInstanceType,int nAddData = 1);
	int         CheckInstanceUse    (UINT userID, UINT nInstanceType);
	bool		UseInstance			(UINT userID, UINT nInstanceType,int nUse = 0);
	UINT		CheckUseInstance(UINT userID, UINT nInstanceType);
	void		ClearUsing(UINT userID, UINT nInstanceType = ID_NONE);
	bool		FinishInstance(UINT userID, UINT nInstanceType,int nTime);
	int			GetFinishInstanceTimes(OBJID idUser, UINT nInstanceType);
	bool		UseInstanceLeaveLine(UINT userID, UINT nInstanceType,int nUse);
	
	void        OnTimer(time_t tCurr);
private:
	CInstanceUseTime *	GetInstanceUseByUseAndType(UINT userID, UINT nInstanceType);
	int       GetCurrentTimeInterval(const char* pText);

	list<CInstanceUseTime*>     m_setInstanceUse;
	IDatabase*                    m_pDB;
	PROCESS_ID		              m_idProcess;
	bool                          m_bDayUp;
};

#endif // !defined(AFX_INSTANCEUSETIMEMGR_H__ED36B899_0E9A_4A73_975F_ACFA2BD15C8E__INCLUDED_)
