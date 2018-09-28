// CompensationManager.h: interface for the CCompensationManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPENSATIONMANAGER_H__A79A5B52_5442_4ECA_B127_D6A521BA3B20__INCLUDED_)
#define AFX_COMPENSATIONMANAGER_H__A79A5B52_5442_4ECA_B127_D6A521BA3B20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Compensation.h"
#include "CompensationMsg.h"
#include "GameObjSet.h"


class CCompensationManager  
{
public:
	CCompensationManager();
	virtual ~CCompensationManager();

	static CCompensationManager* CreateNew() { return new CCompensationManager; }
	ULONG    	Release		    	()				{ delete this; return 0; }
	bool    	Create			    (IDatabase* pDb, PROCESS_ID idProcess,CUser* pUser);
	void		UpdateAll();
	void		AddMsg(OBJID idUser,int nType,const char* szTime,const char* szMsg,bool bSendMail = false);
	void		ClearAllThing();
	void		SendCompentationInfo();
	void		SendCompentationInfoMsg();
	bool		PickCompentation(OBJID id,bool& bOther);

private:
	int       GetCurrentTimePass(const char* pText);
	
	list<CCompensation*>		  m_setCompensation;
	list<CCompensationMsg*>		  m_setCompensationMsg;
	IDatabase*                    m_pDB;
	PROCESS_ID		              m_idProcess;
	CUser*						  m_pOwner;
};

#endif // !defined(AFX_COMPENSATIONMANAGER_H__A79A5B52_5442_4ECA_B127_D6A521BA3B20__INCLUDED_)
