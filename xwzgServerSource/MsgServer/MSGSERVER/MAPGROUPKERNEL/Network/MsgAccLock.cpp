// MsgAccLock.cpp: implementation of the CMsgAccLock class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgAccLock.h"
#include "MapGroup.h"
#include "Rename.h"
#include "MsgAward.h"
#include "MsgPlayer.h"
extern CRename *g_pRename;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAccLock::CMsgAccLock()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgAccLock::~CMsgAccLock()
{

}

BOOL CMsgAccLock::Create(OBJID idUser, USHORT usAction, USHORT usData)
{
	// reset
	this->Init();
	
	// fill
	m_unMsgType		= _MSG_ACCLOCK;
	m_unMsgSize		= sizeof(MSG_Info);
	
	m_pInfo->idUser		= idUser;
	m_pInfo->usAction	= usAction;
	m_pInfo->usData     = usData;
	return TRUE;
}

BOOL CMsgAccLock::Create(OBJID idUser, USHORT usAction, USHORT usData, char* szOldPwd, char* szNewPwd)
{
	// reset
	this->Init();
	
	// fill
	m_unMsgType		= _MSG_ACCLOCK;
	m_unMsgSize		= sizeof(MSG_Info);
	
	m_pInfo->idUser		= idUser;
	m_pInfo->usAction	= usAction;
	m_pInfo->usData     = usData;
	if (szOldPwd && strlen(szOldPwd)>0)
		SafeCopy(m_pInfo->szOldPwd, szOldPwd, 24);
	if (szNewPwd && strlen(szNewPwd)>0)
		SafeCopy(m_pInfo->szNewPwd, szNewPwd, 24);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
void CMsgAccLock::Process(void *pInfo)
{
	int nStep = 0;
	DEBUG_TRY
	if (m_idProcess == MSGPORT_MAPGROUP_FIRST)
	{
		if (m_pInfo->usAction == _MSGACCLOCK_RENAME_WORLD)
		{
			if (g_pRename->ISCanRename(m_pInfo->idUser))
			{
				nStep = 1;
				if (m_pInfo->usData == 0)
				{		
					nStep = 2;
					g_pRename->ReSetMapName(m_pInfo->szOldPwd,m_pInfo->idUser);
					nStep = 3;
					g_pRename->ReSetTutorListName(NULL,m_pInfo->szOldPwd,m_pInfo->idUser);
					nStep = 4;
					g_pRename->ReSetSynMemberName(m_pInfo->szOldPwd,m_pInfo->idUser);
					nStep = 5;
					g_pRename->ReSetVipListName(m_pInfo->szOldPwd,m_pInfo->idUser);
				}
				if (m_pInfo->usData==1)
				{
					nStep = 6;
					g_pRename->ReSetSynName(m_pInfo->szOldPwd,m_pInfo->idUser);
				}
				nStep = 7;
				g_pRename->DeleteRecord(m_pInfo->idUser,false);
			}
		}
	}

	nStep = 8;
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;

	switch(m_pInfo->usAction)
	{
	case _MSGACCLOCK_CHGPWD:
		{
			if (pUser->GetID()!=m_pInfo->idUser)
				return;
			if (0 != stricmp( m_pInfo->szOldPwd, pUser->GetItemLockPwd()))
			{
				//pUser->SendSysMsg(_TXTATR_TOPSHOW, "旧密码错误");
				CMsgAccLock msg;
				if (msg.Create(pUser->GetID(), _MSGACCLOCK_CHGPWD, 1))
					pUser->SendMsg(&msg);
				return;
			}
			else
			{
				pUser->SetItemLockPwd(m_pInfo->szNewPwd);
				CMsgAccLock msg;
				if (msg.Create(pUser->GetID(), _MSGACCLOCK_CHGPWD, 0))
					pUser->SendMsg(&msg);
			}
		}
		break;
	case _MSGACCLOCK_LOCK:
		{
			if (pUser->GetID()!=m_pInfo->idUser)
				return;
			if (!pUser->IsItemLockedOpen())
			{
				pUser->SetItemLock();
			}
			
			CMsgAccLock msg;
			if (msg.Create(pUser->GetID(), _MSGACCLOCK_LOCK, 0))
				pUser->SendMsg(&msg);
		}
		break;
	case _MSGACCLOCK_UNLOCK:
		{
			if (pUser->GetID()!=m_pInfo->idUser)
				return;
			
			if (0 != stricmp( m_pInfo->szNewPwd, pUser->GetItemLockPwd()))
			{
				//pUser->SendSysMsg(_TXTATR_TOPSHOW, "密码错误");
				CMsgAccLock msg;
				if (msg.Create(pUser->GetID(), _MSGACCLOCK_UNLOCK, 1))
					pUser->SendMsg(&msg);
				return;
			}
			else
			{
				if (pUser->IsItemLockedOpen())
				{
					pUser->SetItemUnLock();
				}
				
				CMsgAccLock msg;
				if (msg.Create(pUser->GetID(), _MSGACCLOCK_UNLOCK, 0))
					pUser->SendMsg(&msg);
			}
		}
		break;
	case _MSGACCLOCK_RENAME:
		{
			char szP[255];
			sprintf(szP,"%d",m_pInfo->usData);
			if (g_pRename->ISCanRename(m_pInfo->idUser))
			{
				if (g_pRename->Rename(m_pInfo->idUser,m_pInfo->szOldPwd,m_pInfo->usData))
				{
					if (m_pInfo->usData==0)
					{
						pUser->SetName(m_pInfo->szOldPwd);
						g_pRename->ReSetTutorListName(pUser,m_pInfo->szOldPwd,m_pInfo->idUser);
					}
					CMsgAccLock msg;
					if (msg.Create(m_pInfo->idUser, _MSGACCLOCK_RENAME, 1,szP,m_pInfo->szOldPwd))
						pUser->BroadcastRoomMsg(&msg,INCLUDE_SELF);
					CMsgAccLock msgworld;
					if (msgworld.Create(m_pInfo->idUser,_MSGACCLOCK_RENAME_WORLD,m_pInfo->usData,m_pInfo->szOldPwd,m_pInfo->szNewPwd))
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msgworld,OBJID(0));
					return;
				}
			}
			CMsgAccLock msg;
			if (msg.Create(m_pInfo->idUser, _MSGACCLOCK_RENAME, 0,szP,""))
				pUser->SendMsg(&msg);
		}
		break;
	case _MSGACCLOCK_VERIFICATION_CODE:
		{
			if (pUser->GetID()!=m_pInfo->idUser)
				return;
			if (!pUser->Verification(m_pInfo->szOldPwd))
			{
				if (pUser->GetVerificationTimes() >= 3)
					pUser->KickOutUser();
				else
					pUser->ReSendVerification();
				return;
			}
			CMsgAccLock msg;
			if (msg.Create(0, _MSGACCLOCK_VERIFICATION_CODE, 0))
				pUser->SendMsg(&msg);
		}
		break;
	case _MSGACCLOCK_VERIFICATION_QUERY:
		{
			if (pUser->IsVerificationCoding())
				pUser->ReSendVerification();
			else
				LOGWARNING("客户端在玩家非输入验证码时发送重新请求验证码消息.userid=%u",pUser->GetID());
		}
		break;
	default:
		break;
	}
	DEBUG_CATCH3("CMsgAccLock::Process action=%d nStep = %d", m_pInfo->usAction,nStep)
}
