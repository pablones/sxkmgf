// MsgIssue.cpp: implementation of the CMsgIssue class.
//
//////////////////////////////////////////////////////////////////////
#include "AFXDISP.H"

#include "userlist.h"

// #include "stdafx.h"
#include "MsgIssue.h"
// #include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// #include "AllMsg.h"
// #ifdef	WORLD_KERNEL
// using namespace world_kernel;
// // #include "MessageBoard.h"
// #else
// #pragma warning(disable:4786)
//#include "usermanager.h"
#include "WorldKernel.h"
using namespace world_kernel;

#include "UserList.h"
CMsgIssue::CMsgIssue()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
}

CMsgIssue::~CMsgIssue()
{

}

BOOL CMsgIssue::SendIssue(LPCTSTR strPlayerName,UINT nPlayerID,LPCTSTR strTitle,IssueType itType,LPCTSTR strText)
{
	if(CreateIssue(strPlayerName,nPlayerID,strTitle,itType,strText))
	{
		//Send();
		return true;
	}
	return false;
}

BOOL CMsgIssue::CreateIssue(LPCTSTR strPlayerName,UINT nPlayerID,LPCTSTR strTitle,IssueType itType,LPCTSTR strText)
{
	m_pInfo->usAction=ISSUEACT_SENDISSUE;
	m_pInfo->usPlayerID=nPlayerID;
	m_pInfo->usType=itType;
	strncpy(m_pInfo->usTitle, strTitle,_MAX_ISSUETITLESIZE);
	strncpy(m_pInfo->usText, strText,_MAX_ISSUETEXTSIZE);
	strncpy(m_pInfo->usPlayerName, strPlayerName,_MAX_NAMESIZE);
	return true;
}

BOOL CMsgIssue::Create(char* pMsgBuf, DWORD dwSize)
{
// 	if (!CNetMsg::Create(pMsgBuf, dwSize))
// 		return false;
// 
// 	if(_MSG_ISSUE != this->GetType())
// 		return false;
	if(!pMsgBuf)
		return false;

	memcpy(this->m_bufMsg, pMsgBuf, dwSize);
	return true;
}

//////////////////////////////////////////////////////////////////////
// void CMsgIssue::Process(void* pInfo)
// {
// 	
// }

//////////////////////////////////////////////////////////////////////////
//	20070709 ��� ������Ʒ��¼
void	MyLogSaveIssue(const char* pszServerName/*��������,����*/,
							const char* CreateDate/*��������,����*/,
							const char* SolveDate/*��������,����*/,
							int	Type/*�˺�ID*/,
							int PlayerID/*��ɫID*/,
							const char* PlayerName/*��������,����*/,
							const char* Title/*��������,����*/,
							const char* Text/*��������,����*/)
{

	char	str[2048];
	sprintf(str, "0\t%s\t%s\t%s\t%d\t%s\t%s\t0\t%d\t%s\t0\t0\r\n",
			CreateDate,	SolveDate, pszServerName,PlayerID, PlayerName,Title,Type,Text);

// 	if(MY_LOG_SAVE_COSTLY_ITEM_TO_DISK == 1)
// 	{
// #ifdef	MUTEX_SYSLOG
// 		CSingleLock(&log_xSysLogCrit, true);
// #endif
		time_t ltime;
		time( &ltime );
		
		struct tm *pTime;
		pTime = localtime( &ltime ); /* Convert to local time. */
		
		char szLogName[256];
		sprintf(szLogName, "%s %d-%d-%d.csv", "gmlog/Issue", (pTime->tm_year + 1900),
			(pTime->tm_mon+1)%12, pTime->tm_mday);
		
		FILE* fp	=fopen(szLogName, "a+");
		if(!fp)
			return;
		
		fprintf(fp, "%s", str);
		fclose(fp);
}

// extern CWorldKernel::m_pWorld;
// extern CWorldKernel*	CWorldKernel::m_pWorld;

///���ｫ�յ��ĵײ���Ϣ����Ϣ��ȡ��Ȼ�󽻸�User��Ӧ��������
void CMsgIssue::Process(void *pInfo)
{
	CPlayer* pUser = UserList()->GetPlayerBySocket(GetSocketID());

	switch(m_pInfo->usAction)
	{
		case ISSUEACT_SENDISSUE:				// �ͻ��˷�������
			{
				CPlayer* pGM= UserList()->GetRandomGM();
				if (pGM)
					pGM->SendMsg(this);
  				else
				{
//					GameWorld();
					MyLogSaveIssue( GameWorld()->m_szServer,
									COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")),
								    COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")),
									m_pInfo->usType,
									m_pInfo->usPlayerID,
									m_pInfo->usPlayerName,
									m_pInfo->usTitle,
									m_pInfo->usText
									);
				}
					// 		///û���κ�PM���ߣ���������csv��ʽд�뵽��־�ļ���
			}
			break;
		case ISSUEACT_CHATTEXT:
		case ISSUEACT_REQUESTREPLYCHAT:		// GM�������1V1�������Ӧ��,��Result����ʾ���
			{

				CPlayer* pDstUser = UserList()->GetPlayer(m_pInfo->usPlayerID);
				if(pDstUser)
					pDstUser->SendMsg(this);
				else
				{
					///Ŀ����Ҳ�����
					if (pUser && pUser->IsGM())
					{
						///����gm�����Ҳ�����
						m_pInfo->usResult=2;
						pUser->SendMsg(this);
					}
				}
			}
			break;
// 		case ISSUEACT_SYSMAIL:
// // 			CHECK(pRever);
// 			PostOffice()->SendSysMail(usPlayerID,m_pInfo->usTitle,m_pInfo->usText);
// 			break;
		default:
			break;
	}
// 	if (pUser==NULL)
// 		return;
// 
// 	if (pGM==NULL)
// 	{
// 		///û���κ�PM���ߣ���������csv��ʽд�뵽��־�ļ���
// 		return;
// 	}
}
// 		{
// 			/*/ ת��������Ϣ
// 			CPlayer* pPlayer = g_UserList.GetPlayer(szReceiver);
// 			if(pPlayer)
// 			{
// 				pPlayer->SendMsg(this);
// 			}
// 			else
// 			{
// 				CPlayer* pSender = g_UserList.GetPlayerBySocket(GetSocketID());
// 				if(pSender)
// 				{
// 					if(this->Create(SYSTEM_NAME, pSender->szName, "�Է������ߡ�"))
// 						pSender->SendMsg(this);
// 				}
// 			}//*/
// 		}
// 		break;
// 	case _TXTATR_MSG_TRADE:
// 		{
// 			GameWorld()->QueryTradeMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
// 		}
// 		break;
// 	case _TXTATR_MSG_FRIEND:
// 		{
// 			GameWorld()->QueryFriendMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
// 		}
// 		break;
// 	case _TXTATR_MSG_TEAM:
// 		{
// 			GameWorld()->QueryTeamMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
// 		}
// 		break;
// 	case _TXTATR_MSG_SYN:
// 		{
// 			OBJID idSyn = GetTransData();
// 			CSyndicateWorld* pSyn = SynWorldManager()->QuerySyndicate(idSyn);
// 			IF_OK(pSyn)
// 				pSyn->QueryMessageBoard()->AddMsg(pUser->GetID(), szSender, szWords);
// 		}
// 		break;
// 	case _TXTATR_MSG_OTHER:
// 		{
// 			GameWorld()->QueryOtherMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
// 		}
// 		break;
// 	case _TXTATR_MSG_SYSTEM:
// 		{
// 			OBJID idUser = GetTransData();
// 			GameWorld()->QuerySystemMsgBd()->AddMsg(idUser, szSender, szWords);
// 		}
// 		break;
// 	case _TXTATR_SERVE:
// 		{
// 			CPlayer* pPlayer = UserList()->GetGM();
// 			if (pPlayer)
// 			{
// 				pPlayer->SendMsg(this);
// 			}
// 			else
// 			{
// 				CPlayer* pSpeaker = UserList()->GetPlayerBySocket(GetSocketID());
// 				if (pSpeaker)
// 				{
// 					CMsgTalk	msg;
// 					if (msg.Create(SYSTEM_NAME, pSpeaker->szName, STR_TARGET_OFFLINE))
// 						pSpeaker->SendMsg(&msg);
// 				}
// 			}
// 		}
// 		break;
// 
// 	default:
// 		break;
// 	}
// }

// 	char szLoverName[_MAX_NAMESIZE]={0};///��ż����
// 	char szLoveWords[_MAX_LOVEWORDS]={0};
// 	CUserPtr pUser = UserManager()->GetUser(this);
	///�յ���Ϣ��һת�������߳��д���
// 	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
// 	switch(m_pInfo->usAction)
// 	{
// 		case ISSUEACT_SENDISSUE:///��������
// 			{
// 				///��������������
// 			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
// 
// // 				m_StrPacker.GetString(0,szLoverName,_MAX_NAMESIZE);
// 				//m_StrPacker.GetString(1,szLoveWords,_MAX_LOVEWORDS);
// // 				pUser->Courtship(szLoverName/*,szLoveWords*/);///�����û����������
// 			}
// 			break;
// 		case ISSUEACT_REQUESTREPLYCHAT:///����Է�1v1����
// 			{
// // 				BOOL bResultCourtship=m_pInfo->nParam;///�õ������
// // 				UINT idBeger=m_pInfo->idTarget;
// // 				pUser->ReplyCourtship(idBeger,bResultCourtship);
// 			}
// 			break;
// // 		case mmaRepentMarry:///�ڻ�����
// // 			{
// // 				///UINT idMarry=m_pInfo->idTarget;///��ʹ��MarryID
// // 				pUser->RepentMarry();
// // 			}
// // 			break;
// // 		case mmaMarry:///�������
// // 			{
// // 				pUser->Marry();
// // 			}
// //  			break;
// // 		case mmaReplyMarry:///��Ӧ�������
// // 			{
// // 				BOOL bResultMarry=m_pInfo->nParam;///�õ������
// // 				UINT idBeger=m_pInfo->idTarget;
// // 				pUser->ReplyMarry(idBeger,bResultMarry);
// // 			}
// // 			break;
// // 		case mmaUnmarry:///�������
// // 			{
// // 				///��������������
// // 				m_StrPacker.GetString(0,szLoverName,_MAX_NAMESIZE);
// // 				//pUser->Unmarry(szLoverName);///��Ҫ���
// // 				pUser->RequestUnmarry(szLoverName);
// // 			}
// // 			break;
// // 		case mmaReplyUnmarry:///��Ӧ�������
// // 			{
// // 				BOOL bResultUnmarry=m_pInfo->nParam;///�õ������
// // 				UINT idBeger=m_pInfo->idTarget;
// // 				UINT nState=m_pInfo->unResult;///��ͨ�����������
// // 				pUser->ReplyUnmarry(idBeger,bResultUnmarry,nState);
// // 			}
// // 			break;
// // 		case mmaConditionUnmarry:
// // 			{
// // 				UINT nMoney=m_pInfo->nParam;///�õ������
// // 				UINT idBeger=m_pInfo->idTarget;
// // 				pUser->ConditionUnmarry(idBeger,nMoney);
// // 			}
// // 			break;
// // 		case mmaGotoMate:///˲���ƶ�����ż����
// // 			{
// // 				pUser->GotoMate();///��Ҫ˲��
// // 			}
// // 			break;
// // 		case mmaRefreshMarry:///�õ��ʼ��еĽ�Ǯ
// // 			{
// // 				pUser->RefreshMarryInfo();///�����ݿ��ж������µĻ�����Ϣ
// // 				pUser->SendSelfToBlock();///����㲥
// // 			}	
// // 			break;
// //		case mmaGetItem:///�õ��ʼ��еĵ���
// //			mail.Set_Type(CSKMail::mtCmdMailGetItem);///�ʼ�ȡ�ý�Ǯ��־
// //			mail.Set_MailID(m_pInfo->unResult);///�õ��ʼ�ID,���ڼ����ʼ�
// //			break;
// 		default:
// 			::LogSave("Get Unknown CMsgMarry Action,act:%u", m_pInfo->usAction);
// 			break;
// 	}
//	PostOffice()->ProcessMail(mail);///�����ʾִ���
// }
