// MsgTalk.cpp: implementation of the CMsgTalk class.
//
//////////////////////////////////////////////////////////////////////

//#define _WINSOCKAPI_
#include "AllMsg.h"
#ifdef	WORLD_KERNEL
#include "WorldKernel.h"
using namespace world_kernel;
#include "userlist.h"
#include "MessageBoard.h"
#else
#pragma warning(disable:4786)
#include "usermanager.h"
#endif
#include "I_MessagePort.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgTalk::CMsgTalk()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgTalk::~CMsgTalk()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTalk::Create(char* pszSpeaker, char* pszHearer, char* pszWords, 
					  char* pszEmotion, DWORD dwColor, unsigned short unAttribute, 
					  unsigned short unStyle,DWORD dwItemID,DWORD dwItemTypeID,char* pszItemName)
{
	if(!pszSpeaker || strlen(pszSpeaker) >= _MAX_NAMESIZE)
		return false;

	if(!pszHearer || strlen(pszHearer) >= _MAX_NAMESIZE)
		return false;

	if(!pszWords || strlen(pszWords) >= _MAX_WORDSSIZE)
		return false;

	// can be null
	if(pszEmotion && strlen(pszEmotion) >= _MAX_NAMESIZE)	
		return false;

	// fill
	this->Init();

	// fill structure
	m_pInfo->dwWordsColor	=dwColor;
	m_pInfo->unTxtAttribute	=unAttribute;
	m_pInfo->unTxtStyle		=unStyle;
	m_pInfo->dwTime			=::SysTimeGet();
	m_pInfo->dwItemID		=dwItemID;
	m_pInfo->dwItemTypeID   =dwItemTypeID;


	BOOL bSucMake	=true;
	bSucMake	&=m_StrPacker.AddString(pszSpeaker);
	bSucMake	&=m_StrPacker.AddString(pszHearer);
	bSucMake	&=m_StrPacker.AddString(pszEmotion);
	bSucMake	&=m_StrPacker.AddString(pszWords);
	bSucMake	&=m_StrPacker.AddString(pszItemName);

	m_unMsgType	=_MSG_TALK;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return bSucMake;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTalk::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

void CMsgTalk::Dump(char* pbufMsg, DWORD dwMsgSize)///当此消息出现错误时将消息全部内容写到日志中
{
	DEBUG_TRY 
		char szSender[_MAX_NAMESIZE]="";
	char szReceiver[_MAX_NAMESIZE]="";
	char szEmotion[_MAX_NAMESIZE]="";
	char szWords[_MAX_SPEAK_WORDSSIZE]="";
	char szItemName[_MAX_WORDSSIZE]="";
	CMsgTalk msg;
	if(dwMsgSize>_MAX_MSGSIZE)
	{
		LogSave("talk msg size is error!MsgSize:%d",dwMsgSize);
		dwMsgSize=_MAX_MSGSIZE;
	}
	msg.Create(pbufMsg,dwMsgSize);
	CMsgTalk::MSG_Info& info=msg.GetInfo();
	CNetStringPacker StrPacker;
	StrPacker.SeekBuf(info.szBuf,_MAX_MSGSIZE-sizeof(CMsgTalk::MSG_Info)+1);
	StrPacker.GetString(0, szSender, sizeof(szSender));
	StrPacker.GetString(1, szReceiver, sizeof(szReceiver));
	StrPacker.GetString(2, szEmotion, sizeof(szEmotion));
	StrPacker.GetString(3, szWords, sizeof(szWords));
	StrPacker.GetString(4, szItemName,sizeof(szItemName));
	LogSave("talk msg crash1!MsgSize:%d-WordsColor:%d-TxtAttribute:%d-TxtStyle:%d-Time:%d-UseGMToolPsw:%d-ItemID:%d-dwItemTypeID:%d",
		dwMsgSize,info.dwWordsColor,info.unTxtAttribute,info.unTxtStyle,info.dwTime,info.dwUseGMToolPsw,info.dwItemID,info.dwItemTypeID);
	LogSave("talk msg crash2!Sender:%s-Receiver:%s-Emotion:%s-Words:%s-ItemName:%s",
		szSender,szReceiver,szEmotion,szWords,szItemName);
	DEBUG_CATCH("CMsgTalk::Dump crash")
}

//////////////////////////////////////////////////////////////////////
void CMsgTalk::Process(void *pInfo)
{

#ifdef _MSGDEBUG
	::LogMsg("Process MsgTalk, Sender:%s, Receiver:%s, Words:%s", 
						szSender, 
						szReceiver, 
						szWords);
#endif

	char szSender[_MAX_NAMESIZE];
	char szReceiver[_MAX_NAMESIZE];
	char szEmotion[_MAX_NAMESIZE];
	char szWords[_MAX_WORDSSIZE];
	char szItemName[_MAX_WORDSSIZE];

	m_StrPacker.GetString(0, szSender, sizeof(szSender));
	m_StrPacker.GetString(1, szReceiver, sizeof(szReceiver));
	m_StrPacker.GetString(2, szEmotion, sizeof(szEmotion));
	m_StrPacker.GetString(3, szWords, sizeof(szWords));
	m_StrPacker.GetString(4, szItemName,sizeof(szItemName));

	CPlayer* pUser = UserList()->GetPlayerBySocket(GetSocketID());
	
	if (!pUser && m_pInfo->unTxtAttribute != _TXTATR_MSG_SYSTEM)
	{
		::LogSave("MsgTalkchanal:%s,User = %s, Receiver = %s, Words = %s", m_pInfo->unTxtAttribute, szSender, szReceiver, szWords);
	}

	CHECK(pUser || m_pInfo->unTxtAttribute==_TXTATR_MSG_SYSTEM);
	DEBUG_TRY
	switch(m_pInfo->unTxtAttribute)
	{
	case _TXTATR_WEBPAGE:
		break;

	case _TXTATR_PRIVATE:
		{
			/*/ 转发耳语消息
			CPlayer* pPlayer = g_UserList.GetPlayer(szReceiver);
			if(pPlayer)
			{
				pPlayer->SendMsg(this);
			}
			else
			{
				CPlayer* pSender = g_UserList.GetPlayerBySocket(GetSocketID());
				if(pSender)
				{
					if(this->Create(SYSTEM_NAME, pSender->szName, "对方不在线。"))
						pSender->SendMsg(this);
				}
			}//*/
		}
		break;
	case _TXTATR_MSG_TRADE:
		{
			GameWorld()->QueryTradeMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
		}
		break;
	case _TXTATR_MSG_FRIEND:
		{
			GameWorld()->QueryFriendMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
		}
		break;
	case _TXTATR_MSG_TEAM:
		{
			GameWorld()->QueryTeamMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
		}
		break;
	case _TXTATR_MSG_SYN:
		{
			OBJID idSyn = GetTransData();
			CSyndicateWorld* pSyn = SynWorldManager()->QuerySyndicate(idSyn);
			IF_OK(pSyn)
				pSyn->QueryMessageBoard()->AddMsg(pUser->GetID(), szSender, szWords);
		}
		break;
	case _TXTATR_MSG_OTHER:
		{
			GameWorld()->QueryOtherMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
		}
		break;
	case _TXTATR_MSG_SYSTEM:
		{
			OBJID idUser = GetTransData();
			GameWorld()->QuerySystemMsgBd()->AddMsg(idUser, szSender, szWords);
		}
		break;
	case _TXTATR_SERVE:
		{
			CPlayer* pPlayer = UserList()->GetGM();
			if (pPlayer)
			{
				pPlayer->SendMsg(this);
			}
			else
			{
				CPlayer* pSpeaker = UserList()->GetPlayerBySocket(GetSocketID());
				if (pSpeaker)
				{
					CMsgTalk	msg;
					if (msg.Create(SYSTEM_NAME, pSpeaker->szName, STR_TARGET_OFFLINE))
						pSpeaker->SendMsg(&msg);
				}
			}
		}
		break;
	case _TXTATR_SERVEEX:
		{
			UserList()->BroadcastMsg(this);
		}
		break;
	case _TXTATR_CHATWITH:
		{
			CPlayer* pReceiver = UserList()->GetPlayer(szReceiver);
			if (pReceiver)
				pReceiver->SendMsg(this);
			else//如果接受者不在副本里，则将消息发到关系服务器上
			{
				GameWorld()->SendRelationRelayMsg(GetType(),(LPCTSTR)m_pInfo,GetSize(),GetTransData());
				

// 				MESSAGESTR	buf;
// 				CLIENTMSG_PACKET0* clientMsgPackage = (CLIENTMSG_PACKET0*)buf;
// 				clientMsgPackage->idSocket=GetSocketID();
// 				clientMsgPackage->idPacket=_MSG_TALK;
// 				clientMsgPackage->nSize=m_unMsgSize;
// 				m_pInfo->unTxtAttribute=_TXTATR_LEAVEWORD;
// 				memcpy(clientMsgPackage->buf, GetBuf(), GetSize());
// 
// 
// 				IMessagePort* pMsgPort = GameWorld()->GetMsgPort();
// 				pMsgPort->Send(pUser->idProcess,
// 					KERNEL_CLIENTMSG,
// 					STRUCT_TYPE(buf),
// 					clientMsgPackage);
			}
		}
		break;
	default:
		break;
	}
	DEBUG_CATCH2("MsgTlak Channel:%d!", m_pInfo->unTxtAttribute);
}
