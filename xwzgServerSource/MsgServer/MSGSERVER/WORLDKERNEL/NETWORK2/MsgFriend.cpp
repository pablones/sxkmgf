// MsgFriend.cpp: interface for the CMsgFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "WorldKernel.h"
using namespace world_kernel;
#include "userlist.h"
#include "I_MessagePort.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgFriend::CMsgFriend()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgFriend::~CMsgFriend()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFriend::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_FRIEND != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgFriend::Create	(int nAction, 
				 OBJID idFriend, 
				 LPCTSTR szName, 
				 short bOnline/* = false*/, 
				 DWORD nLookFace/*=0*/, 
				 int nLevel/*=0*/,
				 unsigned int profession/*职业=0*/,
				 unsigned int friendship/*友情度=0*/,
				 unsigned int nSex,
				 unsigned int nFriendType,
				 LPCTSTR szFeel,//[2010-06-05 goto]增加好友心情
				 int nRelation,//[2010-06-05 goto]关系,师徒、夫妻...
				 LPCTSTR szFaction,//帮派
				 LPCTSTR szTitle,//称号
				 LPCTSTR szMateName,//配偶
				 LPCTSTR szTeamName,//队伍
				 OBJID idMap//位置
				 )
{
	//20070528修罗:加强日志
	//CHECKF(idFriend && nAction && szName);
	//IF_NOT(idFriend && nAction && szName)
	IF_NOT(nAction && szName)
	{
		char szLog[512];
		sprintf(szLog,"nAction:%d,idFriend:%d.",nAction,idFriend);
		return false;
	}
	
	// init
	this->Init();
	// fill info now
	m_pInfo->idFriend		= idFriend;
	m_pInfo->dwLookFace		= nLookFace;
	m_pInfo->ucAction		= nAction;
	m_pInfo->ucOnlineFlag	= bOnline;
	m_pInfo->ucLevel		= nLevel;
	//20061012修罗.新增两个参数
	//------------------------------
	m_pInfo->ucProfession	= profession;
	m_pInfo->ucFriendship	= friendship;
	m_pInfo->ucSex          = nSex;
	//------------------------------
	m_pInfo->ucFriendType	= nFriendType;
	m_pInfo->ucRelation		= nRelation;
	SafeCopy(m_pInfo->szName, szName, _MAX_NAMESIZE);
	
	BOOL bSucMake	=true;
	if(szFeel)//[2010-06-05 goto]好友心情
		bSucMake &= m_StrPacker.AddString(szFeel);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(szFaction)//帮派
		bSucMake &= m_StrPacker.AddString(szFaction);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(szTitle)//称号
		bSucMake &= m_StrPacker.AddString(szTitle);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(szMateName)//配偶
		bSucMake &= m_StrPacker.AddString(szMateName);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(szTeamName)//队伍
		bSucMake &= m_StrPacker.AddString(szTeamName);
	else
		bSucMake &= m_StrPacker.AddString("无");
	if(idMap)//位置
	{
		char szMap[10];
		sprintf(szMap,"%u",idMap);
		bSucMake &= m_StrPacker.AddString(szMap);
	}
	else
		bSucMake &= m_StrPacker.AddString("未知");
	
	m_unMsgType	=_MSG_FRIEND;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	
	return bSucMake;
}

//////////////////////////////////////////////////////////////////////
void CMsgFriend::Process(void *pInfo)
{
#ifdef _MSGDEBUG
	::LogMsg("Process CMsgFriend, id:%u", m_pInfo->idFriend);
#endif

	DEBUG_TRY
	CPlayer* pUser = UserList()->GetPlayer(this->GetTransData());
// 	if (!pUser)
// 		return ;

	switch(m_pInfo->ucAction)
	{
	case _FRIEND_ADDSUCCESS_PASSIVE:
	case _FRIEND_DELFRIEND://[2010-06-08 goto]删除好友
		{
			if(GameWorld()->IsRelationServerOpen())///如果请求的好友在本线中不存在，则尝试向关系服务器求助
				GameWorld()->SendRelationRelayMsg(GetType(),(LPCTSTR)m_pInfo,GetSize(),GetTransData());
		}
		break;
	case _FRIEND_ONLINE:
		{
			if(pUser)
				pUser->SendMsg(this);
		}
		break;
	case _ENEMY_ONLINE:
		{
			if(pUser)
				pUser->SendMsg(this);
		}
		break;
	case _FRIEND_OFFLINE:
		{
			if(pUser)
				pUser->SendMsg(this);
			else if(GameWorld()->IsRelationServerOpen())///如果请求的好友在本线中不存在，则尝试向关系服务器求助
			{
				GameWorld()->SendRelationRelayMsg(GetType(),(LPCTSTR)m_pInfo,GetSize(),GetTransData());
			}
		}
		break;
	case _FRIEND_ADDSUCCESS:
		{
// 			CPlayer* pTarget = UserList()->GetPlayer(m_pInfo->idFriend);
// 			if(pTarget)
// 			{
// 				pTarget->SendMsg(this);
// 			}
// 			else if(GameWorld()->IsRelationServerOpen())///如果请求的好友在本线中不存在，则尝试向关系服务器求助
// 			{
// 			//	strncpy(m_pInfo->szName,pUser->szName,_MAX_NAMESIZE);
// 				GameWorld()->SendRelationRelayMsg(GetType(),(LPCTSTR)m_pInfo,GetSize(),pUser->GetID());
// 			}
			if(GameWorld()->IsRelationServerOpen() && pUser)///如果请求的好友在本线中不存在，则尝试向关系服务器求助
			{
				//	strncpy(m_pInfo->szName,pUser->szName,_MAX_NAMESIZE);
				GameWorld()->SendRelationRelayMsg(GetType(),(LPCTSTR)m_pInfo,GetSize(),pUser->GetID());
			}
		}
			break;
	case _FRIEND_GETINFO:
		{
			CPlayer* pTarget = UserList()->GetPlayer(m_pInfo->idFriend);
			if (pUser)
			{
				if(pTarget)
				{
					m_pInfo->ucOnlineFlag = GameWorld()->GetLineIndex();
					pUser->SendMsg(this);
				}
				else if(GameWorld()->IsRelationServerOpen())///如果请求的好友在本线中不存在，则尝试向关系服务器求助
				{
					GameWorld()->SendRelationRelayMsg(GetType(),(LPCTSTR)m_pInfo,GetSize(),pUser->GetID());
				}
				else
				pUser->SendMsg(this);
			}
		}
		break;
	case _FRIEND_HAVETEAM:
		{
// 			CPlayer* pTarget = UserList()->GetPlayer(m_pInfo->idFriend);
// 			if (pTarget)
// 				m_pInfo->ucOnlineFlag=ONLINE_STATUS;
// 			else
// 				m_pInfo->ucOnlineFlag=OFFLINE_STATUS;
// 
// 			MESSAGESTR	buf;
// 			CLIENTMSG_PACKET0* clientMsgPackage = (CLIENTMSG_PACKET0*)buf;
// 			clientMsgPackage->idSocket=GetSocketID();
// 			clientMsgPackage->idPacket=_MSG_FRIEND;
// 			clientMsgPackage->nSize=m_unMsgSize;
// 			m_pInfo->ucAction=_FRIEND_HAVETEAM_MT;
// 			this->SetTransData(pUser->GetID());
// 			this->m_idSocket=pUser->idSocket;
// 			memcpy(clientMsgPackage->buf, GetBuf(), GetSize());
// 			int aa = pUser->idProcess;
// 			IMessagePort* pMsgPort = GameWorld()->GetMsgPort();
// 			pMsgPort->Send(pTarget->idProcess,
// 				KERNEL_CLIENTMSG,
// 				STRUCT_TYPE(buf),
// 				clientMsgPackage);
		}
		break;
	}
	DEBUG_CATCH2("WorldKernel::CMsgFreind:Process action=%d",m_pInfo->ucAction);
}
