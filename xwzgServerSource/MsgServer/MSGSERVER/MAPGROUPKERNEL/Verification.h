// Verification.h: interface for the CVerification class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERIFICATION_H__74333764_25BA_462F_9716_F85AA41C43F9__INCLUDED_)
#define AFX_VERIFICATION_H__74333764_25BA_462F_9716_F85AA41C43F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "T_SingleObjSet2.h"
#include "VerificationData.h"
enum
{ 
	STATE_VERIFICATION_BEGIN=1,
	STATE_VERIFICATION_NORMAL=1,
	STATE_VERIFICATION_NOTALLOW=2,
	STATE_VERIFICATION_PASSTENMIN=3,
	STATE_VERIFICATION_ACCEPTTASK=4,
	STATE_VERIFICATION_ENDTASK=5,
	STATE_VERIFICATION_RUNNING=6,
	STATE_VERIFICATION_END,
};
struct VerificationInfoStruct
{
	int m_ID;
	int m_State;
	int m_Times;
	bool m_coding;
	bool m_bChk;
	bool m_bPass;
};
class CVerification  
{
public:
	CVerification(PROCESS_ID idProcess,CUser* pUser);
	virtual ~CVerification();
	static	CVerification* CreateNew(PROCESS_ID idProcess,CUser* pUser);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
public:
	bool Verification(string szCode);
	bool SendVerification();
	bool CheckVerificationState();
	void KickOutUser();
	int  GetTimes();
	bool ReSendVerification();
	bool IsCoding(){return m_coding;}
	void SetCoding(bool b_coding){m_coding=b_coding;}
	//用于地图组传送
	bool AppendVerificationInfo(const VerificationInfoStruct* pInfo);
	bool GetVerificationInfo(VerificationInfoStruct* pInfo);
protected:
	CUser* m_pUser;
	PROCESS_ID m_idProcess;
	int m_ID;
	bool m_coding;
};

#endif // !defined(AFX_VERIFICATION_H__74333764_25BA_462F_9716_F85AA41C43F9__INCLUDED_)
