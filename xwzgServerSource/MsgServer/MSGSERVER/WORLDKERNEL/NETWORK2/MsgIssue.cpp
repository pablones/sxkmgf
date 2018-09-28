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
//	20070709 朱斌 贵重物品记录
void	MyLogSaveIssue(const char* pszServerName/*服务器名,无用*/,
							const char* CreateDate/*服务器名,无用*/,
							const char* SolveDate/*服务器名,无用*/,
							int	Type/*账号ID*/,
							int PlayerID/*角色ID*/,
							const char* PlayerName/*服务器名,无用*/,
							const char* Title/*服务器名,无用*/,
							const char* Text/*服务器名,无用*/)
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

///这里将收到的底层消息做信息提取，然后交给User相应函数处理
void CMsgIssue::Process(void *pInfo)
{
	CPlayer* pUser = UserList()->GetPlayerBySocket(GetSocketID());

	switch(m_pInfo->usAction)
	{
		case ISSUEACT_SENDISSUE:				// 客户端发出问题
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
					// 		///没有任何PM在线，将问题以csv格式写入到日志文件中
			}
			break;
		case ISSUEACT_CHATTEXT:
		case ISSUEACT_REQUESTREPLYCHAT:		// GM请求玩家1V1聊天包括应答,在Result中提示结果
			{

				CPlayer* pDstUser = UserList()->GetPlayer(m_pInfo->usPlayerID);
				if(pDstUser)
					pDstUser->SendMsg(this);
				else
				{
					///目标玩家不在线
					if (pUser && pUser->IsGM())
					{
						///告诉gm这个玩家不在线
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
// 		///没有任何PM在线，将问题以csv格式写入到日志文件中
// 		return;
// 	}
}
// 		{
// 			/*/ 转发耳语消息
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
// 					if(this->Create(SYSTEM_NAME, pSender->szName, "对方不在线。"))
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

// 	char szLoverName[_MAX_NAMESIZE]={0};///配偶名字
// 	char szLoveWords[_MAX_LOVEWORDS]={0};
// 	CUserPtr pUser = UserManager()->GetUser(this);
	///收到消息后一转到世界线程中处理
// 	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
// 	switch(m_pInfo->usAction)
// 	{
// 		case ISSUEACT_SENDISSUE:///发送问题
// 			{
// 				///拆出求婚对象的名字
// 			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
// 
// // 				m_StrPacker.GetString(0,szLoverName,_MAX_NAMESIZE);
// 				//m_StrPacker.GetString(1,szLoveWords,_MAX_LOVEWORDS);
// // 				pUser->Courtship(szLoverName/*,szLoveWords*/);///调用用户的求婚请求
// 			}
// 			break;
// 		case ISSUEACT_REQUESTREPLYCHAT:///请求对方1v1聊天
// 			{
// // 				BOOL bResultCourtship=m_pInfo->nParam;///得到求婚结果
// // 				UINT idBeger=m_pInfo->idTarget;
// // 				pUser->ReplyCourtship(idBeger,bResultCourtship);
// 			}
// 			break;
// // 		case mmaRepentMarry:///悔婚请求
// // 			{
// // 				///UINT idMarry=m_pInfo->idTarget;///不使用MarryID
// // 				pUser->RepentMarry();
// // 			}
// // 			break;
// // 		case mmaMarry:///完婚请求
// // 			{
// // 				pUser->Marry();
// // 			}
// //  			break;
// // 		case mmaReplyMarry:///回应完婚请求
// // 			{
// // 				BOOL bResultMarry=m_pInfo->nParam;///得到求婚结果
// // 				UINT idBeger=m_pInfo->idTarget;
// // 				pUser->ReplyMarry(idBeger,bResultMarry);
// // 			}
// // 			break;
// // 		case mmaUnmarry:///离婚请求
// // 			{
// // 				///拆出离婚对象的名字
// // 				m_StrPacker.GetString(0,szLoverName,_MAX_NAMESIZE);
// // 				//pUser->Unmarry(szLoverName);///我要离婚
// // 				pUser->RequestUnmarry(szLoverName);
// // 			}
// // 			break;
// // 		case mmaReplyUnmarry:///回应离婚请求
// // 			{
// // 				BOOL bResultUnmarry=m_pInfo->nParam;///得到离婚结果
// // 				UINT idBeger=m_pInfo->idTarget;
// // 				UINT nState=m_pInfo->unResult;///普通离婚或条件离婚
// // 				pUser->ReplyUnmarry(idBeger,bResultUnmarry,nState);
// // 			}
// // 			break;
// // 		case mmaConditionUnmarry:
// // 			{
// // 				UINT nMoney=m_pInfo->nParam;///得到离婚结果
// // 				UINT idBeger=m_pInfo->idTarget;
// // 				pUser->ConditionUnmarry(idBeger,nMoney);
// // 			}
// // 			break;
// // 		case mmaGotoMate:///瞬间移动到配偶请求
// // 			{
// // 				pUser->GotoMate();///我要瞬移
// // 			}
// // 			break;
// // 		case mmaRefreshMarry:///得到邮件中的金钱
// // 			{
// // 				pUser->RefreshMarryInfo();///从数据库中读出最新的婚姻信息
// // 				pUser->SendSelfToBlock();///房间广播
// // 			}	
// // 			break;
// //		case mmaGetItem:///得到邮件中的道具
// //			mail.Set_Type(CSKMail::mtCmdMailGetItem);///邮件取得金钱标志
// //			mail.Set_MailID(m_pInfo->unResult);///得到邮件ID,用于检索邮件
// //			break;
// 		default:
// 			::LogSave("Get Unknown CMsgMarry Action,act:%u", m_pInfo->usAction);
// 			break;
// 	}
//	PostOffice()->ProcessMail(mail);///交给邮局处理
// }
