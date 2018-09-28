// MsgBetOutside.cpp: implementation of the CMsgAtk class.
// �ɽ��ޣ�2002.1.8
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
		REMOTE_CALL_SYNPAYOFF,//20070414����:���ŷ����ʵ�����
		REMOTE_CALL_GOMATE,///˲�Ƶ���ż����
		REMOTE_CALL_REPLYGOMATE,///��Ӧ˲�Ƶ���ż����	
		REMOTE_CALL_KICKOUTPLAYER,//�ߵ����
		REMOTE_CALL_WARGAME,		//	ս���ʹ��
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
// 				pUserTarget->fetchSet(pUser->idUser,m_pInfo->idTarget);//��������Է�
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
// 					IF_OK(msg.Create(SYSTEM_NAME, pUser->szName, "�Է��Ѿ��о��ţ�"))
// 					pUserTarget->SendMsg(&msg);
// 					return;
// 				}
// 
// 				CMsgTalk msg;
// 				if (msg.Create(SYSTEM_NAME, pUser->szName, "�������˲����ڻ����㲻��ͬһ��ͼ�����Ժ����ԡ�", NULL, 0xff0000, _TXTATR_SYSTEM))
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
	case SYN_ANNONCE_OTHERLINE:///���������ߴ����������
//		m_pInfo->usAction=SYN_ANNONCE;///�۸ĳ�һ�㹫������������
		GameWorld()->TransmitBroadcastMapGroupMsg(this);
		break;
	case SYN_BUILDER_OTHERLINE:///���������ߴ����������
	case SYN_UPLEV_OTHERLINE:
//	case SYN_UPSKILLSUB_OTHERLINE:
	case SYN_SETBUILDOTHERLINE:
	case SYN_INVITE_OTHERLINE:
	case SYN_SYNTALKMSG:
	case SYN_UPDATEOTHERNLINE:
		//		m_pInfo->usAction=SYN_ANNONCE;///�۸ĳ�һ�㹫������������
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
	case 	SYN_SETSYNMAPID://���°��ɵ�ͼid
	case SYN_CREATECOMPLETE:
	case SYN_CREATE_OK:///����߰��ɴ����ɹ���,Ҫ��������ת������ͼ���д������ɶ���
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





















