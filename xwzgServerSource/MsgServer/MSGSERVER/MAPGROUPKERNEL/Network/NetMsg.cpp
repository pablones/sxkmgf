// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "basefunc.h"
#include "common.h"
#include "MapGroup.h"
#include "MsgIssue.h"
//modify code by 林德渊 begin 2011.5.30
#include "MsgVasLv.h"
//modify code by 林德渊 end
//@暂屏蔽	MYHEAP_IMPLEMENTATION(CNetMsg,s_heap)
MYHEAP_IMPLEMENTATION(CNetMsg,s_heap)		//06.10.7王玉波添加
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNetMsg::CNetMsg()
{
	Init();
}

CNetMsg::~CNetMsg()
{

}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Init()
{
	memset(m_bufMsg, 0L, sizeof(m_bufMsg));
	m_unMsgSize	=0;
	m_unMsgType	=_MSG_NONE;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::IsValid(void)
{
	if(_MSG_NONE == this->GetType())
		return false;

	return true;
}

void CNetMsg::DumpMsg(LPCTSTR szReasion,...)
{
	DEBUG_TRY 
    char buffer[2048]="";
    vsprintf( buffer, szReasion, (char*) ((&szReasion)+1) );
	::LogSave("CNetMsg::DumpMsg szReasion:%s,msgtype:%d-msgsize:%d,idProcess:%d,idSocket:%d,idNpc:%d",buffer,m_unMsgType,m_unMsgSize,m_idProcess,m_idSocket,m_idNpc);///日志记录
	DEBUG_CATCH("CNetMsg::DumpMsg crash")
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Process(void *pInfo)
{
	//--------------------
	char szMsg[1024];
	sprintf(szMsg, "Process Msg:%d, Size:%d", m_unMsgType, m_unMsgSize);
	::LogSave(szMsg);	
}

//////////////////////////////////////////////////////////////////////
bool CNetMsg::SendMsg(CNetMsg* pMsg)
{
#ifdef _MYDEBUG
	::LogMsg("SendMsg:%d, size:%d", m_unMsgType, m_unMsgSize);

#endif
	if(!IsNpcMsg())
		return MapGroup(PID)->QueryIntraMsg()->SendClientMsg(m_idSocket, pMsg);
// 	else
// 		return MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(m_idNpc, pMsg);
}

//////////////////////////////////////////////////////////////////////
// static functions;
//////////////////////////////////////////////////////////////////////
CNetMsg* CNetMsg::CreateClientMsg(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idMsg,
										const char* pbufMsg, DWORD dwMsgSize, int nTrans)
{
	// check it...
	if(idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
		return NULL;

	CNetMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
	if(!pMsg)
		return NULL;

	pMsg->m_idProcess	= idProcess;
	pMsg->m_idSocket	= idSocket;
	pMsg->m_idNpc		= ID_NONE;
	pMsg->m_unMsgType	= idMsg;
	pMsg->m_unMsgSize	= dwMsgSize;
	pMsg->m_nTransData	= nTrans;

	if(!pMsg->CNetMsg::Create((char*)pbufMsg, dwMsgSize))
	{
		SAFE_DELETE(pMsg);
		return NULL;
	}
	else
	{
	#ifdef _MYDEBUG
		::LogMsg("\n--Receive Msg:%u, Size:%u", pMsg->GetType(), pMsg->GetSize());
	#endif
		return pMsg;
	}
}

//////////////////////////////////////////////////////////////////////
CNetMsg* CNetMsg::CreateNpcMsg(PROCESS_ID idProcess, OBJID idNpc, OBJID idMsg, 
										const char* pbufMsg, DWORD dwMsgSize, int nTrans)
{
	// check it...
	if(idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
		return NULL;

	CNetMsg* pMsg = CreateMsg(idMsg, pbufMsg, dwMsgSize);
	if(!pMsg)
		return NULL;

	pMsg->m_idProcess	= idProcess;
	pMsg->m_idSocket	= SOCKET_NONE;
	pMsg->m_idNpc		= idNpc;
	pMsg->m_unMsgType	= idMsg;
	pMsg->m_unMsgSize	= dwMsgSize;
	pMsg->m_nTransData	= nTrans;

	if(!pMsg->CNetMsg::Create((char*)pbufMsg, dwMsgSize))
	{
		SAFE_DELETE(pMsg);
		return NULL;
	}
	else
	{
	#ifdef _MYDEBUG
		::LogMsg("\n--Receive Msg:%u, Size:%u", pMsg->GetType(), pMsg->GetSize());
	#endif
		return pMsg;
	}
}

CNetMsg* CNetMsg::CreateMsg(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize)
{
	// make it...
	CNetMsg* pMsg	=NULL;
	switch(idMsg)
	{
	case _MSG_GMMAC:
		{
			pMsg	=new CMsgGmMac;		//  [6/19/2007] 哈雷 GM MAC地址
		}
		break;
	case _MSG_TALK:
		pMsg	=new CMsgTalk;
		break;
	case _MSG_ACTION:
		pMsg	=new CMsgAction;
		break;
	case _MSG_USERINFO:
		pMsg	=new CMsgUserInfo;
		break;
	case _MSG_PLAYER:
		pMsg	=new CMsgPlayer;
		break;
	case _MSG_PLAYERDETIL:
		pMsg	=new CMsgPlayDetil;
		break;
	case _MSG_NPCSHOP:
		pMsg	=new CMsgNPCShop;
		break;
	case _MSG_RANDNPCSHOP:
		pMsg	=new CMsgRandNPCShop;
		break;
	case _MSG_ITEMINFO:
		pMsg	=new CMsgItemInfo;
		break;
	case _MSG_ITEM:
		pMsg	=new CMsgItem;
		break;
	case _MSG_FRIEND:
		pMsg	=new CMsgFriend;
		break;
	case _MSG_INTERACT:
		pMsg	=new CMsgInteract;
		break;
	case _MSG_USERATTRIB:
		pMsg	=new CMsgUserAttrib;
		break;
	case _MSG_NAME:
		pMsg	=new CMsgName;
		break;
	case _MSG_AINPCINFO:
		pMsg	=new CMsgMonsterInfo;
		break;
	case _MSG_WALKEX:
		pMsg	=new CMsgWalkEx;
		break;
	case _MSG_MAPITEM:
		pMsg	=new CMsgMapItem;
		break;
	case _MSG_TRADE:
		pMsg	=new CMsgTrade;
		break;
	case _MSG_PACKAGE:
		pMsg	= new CMsgPackage;
		break;
	case _MSG_DIALOG:
		pMsg	= new CMsgDialog;
		break;
	case _MSG_TEAM:
		pMsg	= new CMsgTeam;
		break;
	case _MSG_TEAMMEMBER:
		pMsg	= new CMsgTeamMember;
		break;
	case _MSG_MAGICINFO:
		pMsg	= new CMsgMagicInfo;
		break;
	case _MSG_MAGICEFFECT:
		pMsg	= new CMsgMagicEffect;
		break;
	case _MSG_SYNATTRINFO:
		pMsg	= new CMsgSynAttrInfo;
		break;
	case _MSG_SYNDICATE:
		pMsg	= new CMsgSyndicate;
		break;
	case _MSG_FRIENDINFO:
		pMsg	= new CMsgFriendInfo;
		break;
	case _MSG_SYNMEMBERINFO:
		pMsg	= new CMsgSynMemberInfo;
		break;
	case _MSG_SYNINFO:
		pMsg	= new CMsgSynInfo;
		break;
	case _MSG_EUDEMONATTRIB:
		pMsg	= new CMsgEudemonAttrib;
		break;
	case _MSG_SCHOOLMEMBER:
		pMsg	= new CMsgSchoolMember;
		break;
	case _MSG_ITEMATTRIB:
		pMsg	= new CMsgItemAttrib;
		break;
	case _MSG_TOPLIST:
		pMsg	= new CMsgTopList;
		break;
	case _MSG_TASKDETAIL:
		pMsg	= new CMsgTaskDetail;
		break;
	case _MSG_COMPOUND:
		pMsg    = new CMsgCompound;
		break;
	case _MSG_VAS:
		pMsg    = new CMsgVAS;
		break;
	case _MSG_NEWCARD:
		pMsg    = new CMsgNewCard;
		break;
	case _MSG_GMSTOPACCOUNT:
		pMsg	= new CMsgStopAccount;
		break;
	case _MSG_WEAPONSOUL:
		pMsg = new CMsgWeaponSoul;
		break;
	case _MSG_INSTANCE:
		pMsg = new CMsgMapInstance;
		break;
	case _MSG_USERKEY:
		pMsg = new CMsgUserKey;
		break;
	case _MSG_INFONPCUSEREXIT:
		pMsg = new CMsgInfoNpcUserExit;
		break;
	case _MSG_USERPOS:
		pMsg = new CMsgUserPos;
		break;
	case _MSG_AUTOFIGHT:
		pMsg = new CMsgAutoFight;
		break;
	case _MSG_Fatigue:
		pMsg = new CMsgFatigue;
		break;
	case _MSG_ACCLOCK:
		pMsg = new CMsgAccLock;
		break;
	case _MSG_TEAMPOS:
		pMsg = new CMsgTeamPos;
	case _MSG_VIPLIST:
		pMsg = new CMsgVipList;
		break;
	case _MSG_PETEGG:
		pMsg = new CMsgPetegg;
		break;
//modify code by 林德渊 begin 2011.5.30
	case _MSG_VASLV:
		pMsg = new CMsgVasLv;
		break;
//modify code by 林德渊 end
	case _MSG_WARGAME:
		pMsg = new CMsgWargame;
		break;
	case _MSG_COMPSITION:
		pMsg = new CMsgCompensation;
		break;
//#endif
/////////
	default:
		break;
	}
	return pMsg;
}
