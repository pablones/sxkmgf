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
//	m_StrPacker.SetBuf(m_pInfo->m_strBuffer,_MAX_MSGSIZE-sizeof(MSG_Info)+1);///��λ������λ��
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

///����marry��Ϣ����
BOOL CMsgMapInstance::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_INSTANCE != this->GetType())
		return false;
	return true;
}

BOOL CMsgMapInstance::CreateRequestEnterMsg(PROCESS_ID idProcess,OBJID idUser,OBJID idOwn,UINT nInstanceType,UINT nInstanceScope,UINT nDomainMapID,UINT Cellx,UINT Celly,const char* FightingTeamName)///����һ��������и�����Ϣ
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

BOOL CMsgMapInstance::CreateReplyEnterMsg(OBJID idUser,OBJID idOwn,OBJID idMap,UINT nMapType,UINT nInstanceScope,int Cellx,int Celly)///����һ��������и�����Ϣ
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

void CMsgMapInstance::CreateReplyCreateMsg(UINT nInstanceType,UINT nMapType)///����һ����Ӧ������Ϣ
{
	BOOL hr=true;
	m_pInfo->usAction=imaReplyCreateInstance;
	m_pInfo->idOwn=nInstanceType;
	m_pInfo->nResult=nMapType;
}

void CMsgMapInstance::CreateNotifyInstanceCloseMsg(UINT nInstanceType,OBJID idOwn)///����һ����Ӧ������Ϣ
{
	m_pInfo->usAction=imaNotifyInstanceClose;
	m_pInfo->idUser=idOwn;
	m_pInfo->nParam=nInstanceType;
}

void CMsgMapInstance::CreateRequestStartMsg(CInstanceNPC *pNpc)///����һ����Ӧ������Ϣ
{
	CHECK(pNpc);
	m_pInfo->usAction=imaStartInsatance;
	m_pInfo->nInstanceType=pNpc->GetInstanceType();//��������
}

//void CMsgMapInstance::CreateRefreshClientMsg(CInstanceNPC *pNpc)///����һ����Ӧ������Ϣ
//{
//	CHECK(pNpc);
//	m_pInfo->usAction=imaRefreshInsatance;
//	m_pInfo->nInstanceType=pNpc->GetInstanceType();//��������
//	m_pInfo->idOwn=pNpc->GetOwnID();//����
//	m_pInfo->idNpc=pNpc->GetID();///NpcID���ڷ���ʱʹ��
//	m_pInfo->nMapType=pNpc->GetInstanceMapType();///��ͼ����
//	m_pInfo->nPlayerAmount=pNpc->GetInstanceUserSize();
//	strncpy(m_pInfo->szRoomName,pNpc->GetRoomTitle(),_MAX_STRING);
////	ZeroMemory(m_pInfo->setPlayer,sizeof(m_pInfo->setPlayer));
//	for(int i=0;i<pNpc->GetInstanceUserSize();++i)
//	{
//		///���ﷵ�ص�ָ��϶�����Ϊ��,���Բ����ж�
//		m_pInfo->setPlayer[i].id=pNpc->GetInstanceUser(i)->GetID();
//		m_pInfo->setPlayer[i].nTeamGroup=pNpc->GetInstanceUser(i)->GetTeamGroup();
//		m_pInfo->setPlayer[i].nLookFace=pNpc->GetInstanceUser(i)->GetTeamGroup();///Ĭ��ͷ��,����ʱ��һ��,��ʽʱʹ���ʺ�ͼ��
//		strncpy(m_pInfo->setPlayer[i].szName,g_objStrRes.GetStr(11026),_MAX_NAMESIZE);///�Է������ߡ�
//		CUserPtr pUser=UserManager()->GetUser(m_pInfo->setPlayer[i].id);
//		if(pUser) 
//		{
//			m_pInfo->setPlayer[i].nLookFace=pUser->GetLookFace();///ͷ��
//			m_pInfo->setPlayer[i].nLevel=pUser->GetLev();///����
//			m_pInfo->setPlayer[i].nProfession=pUser->GetProfession();///ְҵ
//			strncpy(m_pInfo->setPlayer[i].szName,UserManager()->GetUser(m_pInfo->setPlayer[i].id)->GetName(),_MAX_NAMESIZE);///�Է������ߡ�
//		}
//	}
//	///dummy
//	m_pInfo->nMapType=pNpc->GetInstanceMapType();///��ͼ����
//}

void CMsgMapInstance::CreateRequestCloseMsg(CInstanceNPC *pNpc)///����һ���رո�����Ϣ
{
	m_pInfo->usAction=imaCloseRoom;
	m_pInfo->nInstanceType=pNpc->GetInstanceType();//��������
	m_pInfo->idOwn=pNpc->GetID();//����
}

void CMsgMapInstance::CreateRefreshClientMsg(CInstanceNPC *pNpc,UINT nCurrentPage)///����һ����Ӧ������Ϣ
{
	UINT nBeginIndex=(nCurrentPage-1)*_MAX_INSTANCEPLAYER;
	if (nCurrentPage<0 || nBeginIndex>pNpc->GetInstanceUserSize())
		return;
	m_pInfo->usAction=imaRefreshInsatance;
	m_pInfo->nInstanceType=pNpc->GetInstanceType();//��������
	m_pInfo->idOwn=pNpc->GetOwnID();//����
	m_pInfo->idNpc=pNpc->GetID();///NpcID���ڷ���ʱʹ��
	m_pInfo->nMapType=pNpc->GetInstanceMapType();///��ͼ����
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
			m_pInfo->setPlayer[i].nLookFace=UserPtr->GetTeamGroup();///Ĭ��ͷ��,����ʱ��һ��,��ʽʱʹ���ʺ�ͼ��
			strncpy(m_pInfo->setPlayer[i].szName,g_objStrRes.GetStr(11026),_MAX_NAMESIZE);///�Է������ߡ�
			CUserPtr pUser=UserManager()->GetUser(m_pInfo->setPlayer[i].id);
			if(pUser) 
			{
				m_pInfo->setPlayer[i].nLookFace=pUser->GetLookFace();///ͷ��
				m_pInfo->setPlayer[i].nLevel=pUser->GetLev();///����
				m_pInfo->setPlayer[i].nProfession=pUser->GetProfession();///ְҵ
				strncpy(m_pInfo->setPlayer[i].szName,UserManager()->GetUser(m_pInfo->setPlayer[i].id)->GetName(),_MAX_NAMESIZE);///�Է������ߡ�
			}
		}
	}
}

///���ｫ�յ��ĵײ���Ϣ����Ϣ��ȡ��Ȼ�󽻸�User��Ӧ��������
void CMsgMapInstance::Process(void *pInfo)
{
//	char szLoverName[_MAX_NAMESIZE]={0};///��ż����
//	char szLoveWords[_MAX_LOVEWORDS]={0};
	CUserPtr pUser = UserManager()->GetUser(m_pInfo->idUser);
	// CInstanceNPC* pNpc = (CInstanceNPC*)NpcManager()->GetNpc(m_pInfo->idUser);
	switch(m_pInfo->usAction)
	{
		case imaRequestEnterInstance://////������븱����Ϣ
			{
				///���󸱱�����������
				MapInstanceMgr()->RequestEnterInstance(m_pInfo->idProcess,m_pInfo->idUser,m_pInfo->idOwn,m_pInfo->nParam,m_pInfo->nDomainMapID,
					m_pInfo->nInstanceScope,m_pInfo->nCellx,m_pInfo->nCelly,m_pInfo->FightingTeamName);
//				m_StrPacker.GetString(0,szLoverName,_MAX_NAMESIZE);
//				m_StrPacker.GetString(1,szLoveWords,_MAX_LOVEWORDS);
//				pUser->Courtship(szLoverName,szLoveWords);///�����û����������
			}
			break;
		case imaReplyEnterInstance:///���������������Ļ�Ӧ
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
		case imaStartInsatance:///���������������Ļ�Ӧ
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->StartInstance(m_pInfo->idOwn);
			}
			break;
		case imaJoinInsatance:///���������������Ļ�Ӧ
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->RequestJoinRoom(m_pInfo->idOwn);
			}
			break;
		case imaRefreshInsatance:///����ͻ��˷������¸�����Ϣ����
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->RefreshClientInstanceInfo(m_pInfo->idOwn,m_pInfo->nParam);
			}
			break;
		case imaNotifyInstanceClose:///֪ͨ�رո���
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// NpcManager()->RemoveSynNpc(pNpc->GetID());
			}
			break;
		case imaChangeTeamGroup:///�ı丱��������
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->ChangeTeamGroup(m_pInfo->idOwn,m_pInfo->nParam);
			}
			break;
		case imaKickOut:///���������ߵ�ĳ��������
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->KickPlayer(m_pInfo->idOwn,m_pInfo->nParam);
			}	
			break;
		case imaChangeOwn:///�������ƽ�������������
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->ChangeOwn(m_pInfo->idOwn,m_pInfo->nParam);
			}
			break;
		case imaChangeRoomTitle:///�ı䷿�����
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->ChangeRoomTitle(m_pInfo->idOwn,m_pInfo->szRoomName);
			}
			break;
		case imaChangeRoomPwd:///�ı䷿������
			{
				// CHECK(pNpc && pNpc->GetSort()==NPCSORT_DYNANPC_INSTANCE);
				// pNpc->ChangeRoomPassword(m_pInfo->idOwn,m_pInfo->szRoomName);
			}
			break;
		case imaCheckRoomPwd:///��鷿������
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
//	PostOffice()->ProcessMail(mail);///�����ʾִ���
}
