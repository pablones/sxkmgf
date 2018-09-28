// MsgBetOutside.cpp: implementation of the CMsgAtk class.
// 仙剑修，2002.1.8
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "WorldKernel.h"
#include "../share/define.h"
#include "MsgSyndicate.h"
#include "I_MessagePort.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

enum REMOTE_CALL{
		REMOTE_CALL_DROP_LEADER,
		REMOTE_CALL_LEAVE_SYN,
		REMOTE_CALL_TEAM_CLOSE_MONEY,
		REMOTE_CALL_TEAM_CLOSE_ITEM,
		REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS,
		REMOTE_CALL_TEAM_HUNTKILL,
		REMOTE_CALL_SYNPAYOFF,//20070414修罗:军团发工资到个人
		REMOTE_CALL_GOMATE,///瞬称到配偶请求
		REMOTE_CALL_REPLYGOMATE,///回应瞬称到配偶请求	
		REMOTE_CALL_KICKOUTPLAYER,//踢掉玩家
		REMOTE_CALL_WARGAME,		//	战争活动使用
		REMOTE_CALL_SYNADDMENBER,
		REMOTE_CALL_SYNADDHAVE,
		REMOTE_CALL_SYNADDOK,
};

CMsgSyndicate::CMsgSyndicate()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgSyndicate::~CMsgSyndicate()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create		(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_SYNDICATE != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void	CMsgSyndicate::Process		(void *pInfo)
{
	char szName[_MAX_NAMESIZE] = "";
	if (m_StrPacker.GetStrAmount() > 0)
		m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
	switch(m_pInfo->usAction)
	{
// 	case INVITE_JOIN_NAME:
// 		{
// 			CPlayer* pUser = UserList()->GetPlayer(m_pInfo->idFealty);//GetPlayerBySocket(GetSocketID());
// 			if(!pUser)
// 				return;
// 			CPlayer* pUserTarget = UserList()->GetPlayer(szName);
// 			if(pUserTarget)
// 			{
// 				pUserTarget->fetchSet(pUser->idUser,m_pInfo->idTarget);//设置邀请对方
// 
// 				REMOTE_CALL0 remoteCallInfo;
// 				remoteCallInfo.idUser=pUserTarget->idUser;
// 				remoteCallInfo.ucFuncType=REMOTE_CALL_SYNADDMENBER;
// 				remoteCallInfo.IntParam[0]=pUser->idUser;
// 				remoteCallInfo.IntParam[1]=m_pInfo->idTarget;
// 					
// 				IMessagePort* pMsgPort = GameWorld()->GetMsgPort();
// 				pMsgPort->Send(pUserTarget->idProcess, 
// 					MAPGROUP_REMOTECALL, 
// 					BUFFER_TYPE(sizeof(remoteCallInfo)), 
// 					&remoteCallInfo);
// 			}
// 			else
// 			{
// 				if(pUser->fetchGet(1) != 0)
// 				{
// 					pUserTarget = UserList()->GetPlayer(pUser->fetchGet(1));
// 					pUser->fetchSet(0,0,0);
// 
// 					CMsgTalk	msg;
// 					IF_OK(msg.Create(SYSTEM_NAME, pUser->szName, "对方已经有军团！"))
// 					pUserTarget->SendMsg(&msg);
// 					return;
// 				}
// 
// 				CMsgTalk msg;
// 				if (msg.Create(SYSTEM_NAME, pUser->szName, "被邀请人不存在或与你不在同一地图，请稍后再试。", NULL, 0xff0000, _TXTATR_SYSTEM))
// 					pUser->SendMsg(&msg);
// 
// 				pUser->SendSysMsg();
// 				pUser->SendSysMsg()
// 				CMsgTalk	msg; 
// 				IF_OK(msg.Create(SYSTEM_NAME, pUser->szName, ))
// 					pUser->SendMsg(&msg);
//			}

			
//		}

//		break;
// 	case SYN_SETRANK:
// 		{
// 			CPlayer* pPlayer = UserList()->GetPlayer(szName);
// 			if(pPlayer)
// 			{
// 				AppendInfo(-1,pPlayer->idSocket,-1);
// 				GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,this);
// 			}
// 		}
// 		break;
	case SYN_ANNONCE_OTHERLINE:///处理其它线传来公告更新
//		m_pInfo->usAction=SYN_ANNONCE;///篡改成一般公告走正常流程
		GameWorld()->TransmitBroadcastMapGroupMsg(this);
		break;
	case SYN_BUILDER_OTHERLINE:///处理其它线传来公告更新
	case SYN_UPLEV_OTHERLINE:
//	case SYN_UPSKILLSUB_OTHERLINE:
	case SYN_SETBUILDOTHERLINE:
	case SYN_INVITE_OTHERLINE:
	case SYN_SYNTALKMSG:
	case SYN_UPDATEOTHERNLINE:
		//		m_pInfo->usAction=SYN_ANNONCE;///篡改成一般公告走正常流程
				GameWorld()->TransmitBroadcastMapGroupMsg(this);
	//	GameWorld()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST,this);
		break;
	case SYN_KICKOUT:
	case SYN_BUILDER:
	case SYN_UPLEV:
	case SYN_LEVELUP:
	case SYN_IMPACH:
	case SYN_UPSKILLSUB:
	case SYN_SETRANK:
	case SYN_DEMISE:
	case SYN_ANNONCE:
	case SYN_LEAVER:
	case SYN_ANSERR_NO:
	case SYN_ANSERR_OK:
	case SYN_SETLEADER:
		{
			CPlayer* pPlayer = UserList()->GetPlayer(szName);
			if(pPlayer)
			{
				AppendInfo(-1,pPlayer->idSocket,-1);
				GameWorld()->TransmitMapGroupMsg(pPlayer->idProcess,this);
			}
		}
		break;
	case 	SYN_SETSYNMAPID://更新帮派地图id
	case SYN_CREATECOMPLETE:
	case SYN_CREATE_OK:///别的线帮派创建成功了,要求在这里转发到地图组中创建帮派对象
		{
			//GameWorld()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_FIRST,this);
						GameWorld()->TransmitBroadcastMapGroupMsg(this);
		}
		break;
	case INVITE_JOIN:
		{
		}
	case APPLY_JOIN:
		{
			CPlayer* pUser = UserList()->GetPlayerBySocket(GetSocketID());
			if(!pUser)
				return;
			if(pUser->fetchGet(1) != 0)
			{
				CPlayer* pUserTarget = UserList()->GetPlayer(pUser->fetchGet(1));
				
				if(pUserTarget)
				{
					REMOTE_CALL0 remoteCallInfo;
					remoteCallInfo.idUser=pUser->idUser;
					remoteCallInfo.ucFuncType=REMOTE_CALL_SYNADDOK;
					remoteCallInfo.IntParam[0]=pUser->fetchGet(1);
					remoteCallInfo.IntParam[1]=pUser->fetchGet(2);

					IMessagePort* pMsgPort = GameWorld()->GetMsgPort();
					pMsgPort->Send(pUser->idProcess, 
						MAPGROUP_REMOTECALL, 
						BUFFER_TYPE(sizeof(remoteCallInfo)), 
						&remoteCallInfo);
				}

				pUser->fetchSet(0,0,0);
			}
		}
		break;
	}
}





















