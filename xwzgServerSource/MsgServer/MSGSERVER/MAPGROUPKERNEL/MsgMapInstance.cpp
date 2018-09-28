// MsgMapInstance.cpp: implementation of the CMsgMapInstance class.
//
//////////////////////////////////////////////////////////////////////
#include "afx.h"
#include "NetMsg.h"
#include "MsgMapInstance.h"
#include "mapgroup.h"
#include "MapInstanceMgr.h"
#include "InstanceNPC.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgMapInstance::CMsgMapInstance()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgType	=_MSG_INSTANCE;
	m_unMsgSize=sizeof(MSG_Info);
	m_idProcess=0;
//	m_StrPacker.SetBuf(m_pInfo->m_strBuffer,_MAX_MSGSIZE-sizeof(MSG_Info)+1);///定位到串包位置
}

CMsgMapInstance::CMsgMapInstance(PROCESS_ID idProcess)
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgType	=_MSG_INSTANCE;
	m_unMsgSize=sizeof(MSG_Info);
	m_idProcess=idProcess;
}

CMsgMapInstance::~CMsgMapInstance()
{

}

///重现marry消息对象
BOOL CMsgMapInstance::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_INSTANCE != this->GetType())
		return false;
	return true;
}

BOOL CMsgMapInstance::CreateRequestEnterMsg(PROCESS_ID idProcess,OBJID idUser,OBJID idOwn,UINT nInstanceType,UINT nInstanceScope,UINT nDomainMapID,UINT Cellx,UINT Celly,const char* FightingTeamName)///生成一个请求进行副本消息
{
	BOOL hr=true;
	m_pInfo->usAction=imaRequestEnterInstance;
	m_pInfo->idProcess=idProcess;
	m_pInfo->idUser=idUser;
	m_pInfo->idOwn=idOwn;
	m_pInfo->nParam=nInstanceType;
	m_pInfo->nDomainMapID = nDomainMapID;
	m_pInfo->nInstanceScope = nInstanceScope;
	m_pInfo->nCellx = Cellx;
	m_pInfo->nCelly = Celly;
 	if(FightingTeamName)
 		strcpy(m_pInfo->FightingTeamName,FightingTeamName);
	return hr;
}

BOOL CMsgMapInstance::CreateReplyEnterMsg(OBJID idUser,OBJID idOwn,OBJID idMap,UINT nMapType,UINT nInstanceScope,int Cellx,int Celly)///生成一个请求进行副本消息
{
	BOOL hr=true;
	m_pInfo->usAction=imaReplyEnterInstance;
	m_pInfo->idUser=idUser;
	m_pInfo->idOwn=idOwn;
	m_pInfo->nParam=nMapType;
	m_pInfo->nResult=idMap;
	m_pInfo->nInstanceScope = nInstanceScope;
	m_pInfo->nCellx = Cellx;
	m_pInfo->nCelly = Celly;
	return hr;
}

void CMsgMapInstance::CreateReplyCreateMsg(UINT nInstanceType,UINT nMapType)///生成一个回应副本消息
{
	BOOL hr=true;
	m_pInfo->usAction=imaReplyCreateInstance;
	m_pInfo->idOwn=nInstanceType;
	m_pInfo->nResult=nMapType;
}

void CMsgMapInstance::CreateNotifyInstanceCloseMsg(UINT nInstanceType,OBJID idOwn)///生成一个回应副本消息
{
	m_pInfo->usAction=imaNotifyInstanceClose;
	m_pInfo->idUser=idOwn;
	m_pInfo->nParam=nInstanceType;
}

void CMsgMapInstance::CreateRequestStartMsg(CInstanceNPC *pNpc)///生成一个回应副本消息
{
	CHECK(pNpc);
	m_pInfo->usAction=imaStartInsatance;
	m_pInfo->nInstanceType=pNpc->GetInstanceType();//副本类型
}

//void CMsgMapInstance::CreateRefreshClientMsg(CInstanceNPC *pNpc)///生成一个回应副本消息
//{
//	CHECK(pNpc);
//	m_pInfo->usAction=imaRefreshInsatance;
//	m_pInfo->nInstanceType=pNpc->GetInstanceType();//副本类型
//	m_pInfo->idOwn=pNpc->GetOwnID();//房主
//	m_pInfo->idNpc=pNpc->GetID();///NpcID用于返回时使用
//	m_pInfo->nMapType=pNpc->GetInstanceMapType();///地图类型
//	m_pInfo->nPlayerAmount=pNpc->GetInstanceUserSize();
//	strncpy(m_pInfo->szRoomName,pNpc->GetRoomTitle(),_MAX_STRING);
////	ZeroMemory(m_pInfo->setPlayer,sizeof(m_pInfo->setPlayer));
//	for(int i=0;i<pNpc->GetInstanceUserSize();++i)
//	{
//		///这里返回的指针肯定不会为空,所以不作判断
//		m_pInfo->setPlayer[i].id=pNpc->GetInstanceUser(i)->GetID();
//		m_pInfo->setPlayer[i].nTeamGroup=pNpc->GetInstanceUser(i)->GetTeamGroup();
//		m_pInfo->setPlayer[i].nLookFace=pNpc->GetInstanceUser(i)->GetTeamGroup();///默认头像,测试时不一样,正式时使用问号图标
//		strncpy(m_pInfo->setPlayer[i].szName,g_objStrRes.GetStr(11026),_MAX_NAMESIZE);///对方不在线。
//		CUserPtr pUser=UserManager()->GetUser(m_pInfo->setPlayer[i].id);
//		if(pUser) 
//		{
//			m_pInfo->setPlayer[i].nLookFace=pUser->GetLookFace();///头像
//			m_pInfo->setPlayer[i].nLevel=pUser->GetLev();///级别
//			m_pInfo->setPlayer[i].nProfession=pUser->GetProfession();///职业
//			strncpy(m_pInfo->setPlayer[i].szName,UserManager()->GetUser(m_pInfo->setPlayer[i].id)->GetName(),_MAX_NAMESIZE);///对方不在线。
//		}
//	}
//	///dummy
//	m_pInfo->nMapType=pNpc->GetInstanceMapType();///地图类型
//}

void CMsgMapInstance::CreateRequestCloseMsg(CInstanceNPC *pNpc)///生成一个关闭副本消息
{
	m_pInfo->usAction=imaCloseRoom;
	m_pInfo->nInstanceType=pNpc->GetInstanceType();//副本类型
	m_pInfo->idOwn=pNpc->GetID();//房主
}

void CMsgMapInstance::CreateRefreshClientMsg(CInstanceNPC *pNpc,UINT nCurrentPage)///生成一个回应副本消息
{
	UINT nBeginIndex=(nCurrentPage-1)*_MAX_INSTANCEPLAYER;
	if (nCurrentPage<0 || nBeginIndex>pNpc->GetInstanceUserSize())
		return;
	m_pInfo->usAction=imaRefreshInsatance;
	m_pInfo->nInstanceType=pNpc->GetInstanceType();//副本类型
	m_pInfo->idOwn=pNpc->GetOwnID();//房主
	m_pInfo->idNpc=pNpc->GetID();///NpcID用于返回时使用
	m_pInfo->nMapType=pNpc->GetInstanceMapType();///地图类型
	m_pInfo->nPlayerAmount=pNpc->GetInstanceUserSize();
	strncpy(m_pInfo->szRoomName,pNpc->GetRoomTitle(),_MAX_STRING);
	m_pInfo->nPlayerAmount=pNpc->GetInstanceUserSize();
	m_pInfo->nCurrentPage=nCurrentPage;
	for(int i=0;i<_MAX_INSTANCEPLAYER;++i)
	{
		CInstanceUser* UserPtr=pNpc->GetInstanceUser(nBeginIndex+i);
		if(UserPtr==NULL)
			break;
		else
		{
			m_pInfo->setPlayer[i].id=UserPtr->GetID();
			m_pInfo->setPlayer[i].nTeamGroup=UserPtr->GetTeamGroup();
			m_pInfo->setPlayer[i].nLookFace=UserPtr->GetTeamGroup();///默认头像,测试时不一样,正式时使用问号图标
			strncpy(m_pInfo->setPlayer[i].szName,g_objStrRes.GetStr(11026),_MAX_NAMESIZE);///对方不在线。
			CUserPtr pUser=UserManager()->GetUser(m_pInfo->setPlayer[i].id);
			if(pUser) 
			{
				m_pInfo->setPlayer[i].nLookFace=pUser->GetLookFace();///头像
				m_pInfo->setPlayer[i].nLevel=pUser->GetLev();///级别
				m_pInfo->setPlayer[i].nProfession=pUser->GetProfession();///职业
				strncpy(m_pInfo->setPlayer[i].szName,UserManager()->GetUser(m_pInfo->setPlayer[i].id)->GetName(),_MAX_NAMESIZE);///对方不在线。
			}
		}
	}
}

///这里将收到的底层消息做信息提取，然后交给User相应函数处理
void CMsgMapInstance::Process(void *pInfo)
{
//	char szLoverName[_MAX_NAMESIZE]={0};///配偶名字
//	char szLoveWords[_MAX_LOVEWORDS]={0};
	CUserPtr pUser = UserManager()->GetUser(m_pInfo->idUser);
	// CInstanceNPC* pNpc = (CInstanceNPC*)NpcManager()->GetNpc(m_pInfo->idUser);
	switch(m_pInfo->usAction)
	{
		case imaRequestEnterInstance://////请求进入副本消息
			{
				///请求副本管理器处理
				MapInstanceMgr()->RequestEnterInstance(m_pInfo->idProcess,m_pInfo->idUser,m_pInfo->idOwn,m_pInfo->nParam,m_pInfo->nDomainMapID,
					m_pInfo->nInstanceScope,m_pInfo->nCellx,m_pInfo->nCelly,m_pInfo->FightingTeamName);
//				m_StrPacker.GetString(0,szLoverName,_MAX_NAMESIZE);
//				m_StrPacker.GetString(1,szLoveWords,_MAX_LOVEWORDS);
//				pUser->Courtship(szLoverName,szLoveWords);///调用用户的求婚请求
			}
			break;
		case imaReplyEnterInstance:///处理副本管理器发的回应
			{
				if (pUser)
					pUser->EnterInstance(m_pInfo->nResult,m_pInfo->nParam,m_pInfo->nInstanceScope,m_pInfo->nDomainMapID,m_pInfo->nCellx,m_pInfo->nCelly);
// 				else
// 				{
// 					// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
// 					// pNpc->ReplyCreateInstance(m_pInfo->nResult,m_pInfo->nParam);
// 				}
			}
			break;
		case imaStartInsatance:///处理副本管理器发的回应
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->StartInstance(m_pInfo->idOwn);
			}
			break;
		case imaJoinInsatance:///处理副本管理器发的回应
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->RequestJoinRoom(m_pInfo->idOwn);
			}
			break;
		case imaRefreshInsatance:///处理客户端发来更新副本信息请求
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->RefreshClientInstanceInfo(m_pInfo->idOwn,m_pInfo->nParam);
			}
			break;
		case imaNotifyInstanceClose:///通知关闭副本
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// NpcManager()->RemoveSynNpc(pNpc->GetID());
			}
			break;
		case imaChangeTeamGroup:///改变副本玩家组号
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->ChangeTeamGroup(m_pInfo->idOwn,m_pInfo->nParam);
			}
			break;
		case imaKickOut:///房主请求踢掉某个参与者
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->KickPlayer(m_pInfo->idOwn,m_pInfo->nParam);
			}	
			break;
		case imaChangeOwn:///将房主移交给其它参与者
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->ChangeOwn(m_pInfo->idOwn,m_pInfo->nParam);
			}
			break;
		case imaChangeRoomTitle:///改变房间标题
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->ChangeRoomTitle(m_pInfo->idOwn,m_pInfo->szRoomName);
			}
			break;
		case imaChangeRoomPwd:///改变房间密码
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->ChangeRoomPassword(m_pInfo->idOwn,m_pInfo->szRoomName);
			}
			break;
		case imaCheckRoomPwd:///检查房间密码
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->CheckRoomPassword(m_pInfo->idOwn,m_pInfo->szRoomName);
			}
			break;
		case imaExitInsatance:
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->RequestExitRoom(m_pInfo->idOwn);
			}
			break;
		case imaCloseRoom:
			{
				MapInstanceMgr()->RequestCloseInstance(*this);
			}
			break;
		default:
			::LogSave("Get Unknown CMsgMapInstance Action,act:%u", m_pInfo->usAction);
			break;
	}
//	PostOffice()->ProcessMail(mail);///交给邮局处理
}
