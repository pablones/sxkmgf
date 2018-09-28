// Verification.cpp: implementation of the CVerification class.
//
//////////////////////////////////////////////////////////////////////

#include "Verification.h"
#include "MapGroup.h"
#include "MsgAcclock.h"
#include "MsgFatigue.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CVerificationData* g_pVerificationData;

CVerification::CVerification(PROCESS_ID idProcess,CUser* pUser)
{
	CHECK(pUser);
	m_pUser=pUser;
	m_idProcess = idProcess;
	m_ID=0;
	m_coding=false;

}

CVerification::~CVerification()
{
	
}
CVerification* CVerification::CreateNew(PROCESS_ID idProcess,CUser* pUser)
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess));
	CHECKF(pUser);
	
	CVerification* pVerification = new CVerification(idProcess, pUser);
	CHECKF(pVerification);
	
	return pVerification;
}
bool CVerification::Verification(string szCode)
{
	CHECKF(g_pVerificationData);
	CHECKF(m_pUser);
	m_pUser->SetVerificationNeedChk(true);
	if(g_pVerificationData->GetCodeById(m_ID)==szCode)
	{
		m_pUser->SetVerificationTimes(0);
		m_pUser->SetVerificationPass(true);
		m_pUser->DetachStatus(m_pUser->QueryRole(),STATUS_VERIFICATION_WAIT);
		if(m_pUser->AwardBattleExp(m_pUser->GetLev()*50))
			m_pUser->SendSysMsg(_TXTATR_TOPSHOW, "你获得了%u经验", m_pUser->GetLev()*50);
		m_pUser->AddAttrib(101, m_pUser->GetLev() * 15, SYNCHRO_TRUE);//加碎银
		m_coding=false;
		m_ID=0;
		return true;
	}
	m_pUser->AddVerificationTimes();
	m_pUser->SetVerificationPass(false);
	m_coding=true;
	return false;
}

bool CVerification::SendVerification()
{
	CHECKF(m_pUser);
	int randindex=RandGet(g_pVerificationData->GetListSize());
	m_ID=g_pVerificationData->GetIdByIndex(randindex);
	m_coding=true;
	m_pUser->SetVerificationPass(false);
	m_pUser->SetVerificationNeedChk(true);
	m_pUser->AttachStatus(m_pUser->QueryRole(),STATUS_VERIFICATION_WAIT, 0, 62);//比客户端多两秒
	CMsgAccLock msg;
	if (!msg.Create(m_ID,_MSGACCLOCK_VERIFICATION_QUERY,m_pUser->GetVerificationTimes()))
		return false;
	m_pUser->SendMsg(&msg);
	return true;
}
bool CVerification::CheckVerificationState()
{
	CHECKF(m_pUser);
	if (m_pUser->GetVerificationState()==STATE_VERIFICATION_PASSTENMIN || m_pUser->GetVerificationState() == STATE_VERIFICATION_NOTALLOW)//方便以后添加在游戏服禁止玩家上线
	{
		m_pUser->SendVerification();
		m_pUser->SetVerificationState(STATE_VERIFICATION_NORMAL);
		return true;
	}
	return true;
}
void CVerification::KickOutUser()
{
	CHECK(m_pUser);
	m_pUser->SetVerificationNeedChk(false);
	m_pUser->SetVerificationState(STATE_VERIFICATION_NOTALLOW);	
	m_pUser->SetVerificationTimes(0);
	if (m_pUser->QueryStatus(STATUS_VERIFICATION_RUNNING))
		m_pUser->DetachStatus(m_pUser->QueryRole(),STATUS_VERIFICATION_RUNNING);
	if (m_pUser->QueryStatus(STATUS_VERIFICATION_WAIT))
		m_pUser->DetachStatus(m_pUser->QueryRole(),STATUS_VERIFICATION_WAIT);
	UserManager()->KickOutSocket(m_pUser->GetSocketID());
	LOGCHEAT("玩家因验证码不通过而被踢下线 useid=%u,usename=%s",m_pUser->GetID(),m_pUser->GetName());
}

int CVerification::GetTimes()
{
	CHECKF(m_pUser);
	return m_pUser->GetVerificationState();
}
bool CVerification::ReSendVerification()
{
	CHECKF(m_pUser);
	int randindex=RandGet(g_pVerificationData->GetListSize());
	m_ID=g_pVerificationData->GetIdByIndex(randindex);
	CMsgAccLock msg;
	if (!msg.Create(m_ID,_MSGACCLOCK_VERIFICATION_QUERY,m_pUser->GetVerificationTimes()))
		return false;
	m_pUser->SendMsg(&msg);
	m_coding=true;
	m_pUser->SetVerificationPass(false);
	m_pUser->SetVerificationNeedChk(true);
	return true;
}
bool CVerification::AppendVerificationInfo(const VerificationInfoStruct* pInfo)
{
	CHECKF(m_pUser);
	m_ID=pInfo->m_ID;
	m_coding=pInfo->m_coding;
	m_pUser->SetVerificationState(pInfo->m_State);
	m_pUser->SetVerificationTimes(pInfo->m_Times);
	m_pUser->SetVerificationNeedChk(pInfo->m_bChk);
	m_pUser->SetVerificationPass(pInfo->m_bPass);

	return true;
}
bool CVerification::GetVerificationInfo(VerificationInfoStruct* pInfo)
{
	CHECKF(m_pUser);
	CHECKF(pInfo);
	pInfo->m_ID=m_ID;
	pInfo->m_coding=m_coding;
	pInfo->m_bChk=m_pUser->GetVerificationNeedChk();
	pInfo->m_bPass=m_pUser->GetVerificationPass();
	pInfo->m_State=m_pUser->GetVerificationState();
	pInfo->m_Times=m_pUser->GetVerificationTimes();
	return true;
}
