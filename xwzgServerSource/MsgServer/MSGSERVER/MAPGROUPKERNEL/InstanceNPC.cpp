// InstanceNPC.cpp: implementation of the CInstanceNPC class.
//
//////////////////////////////////////////////////////////////////////
#include "afx.h"
#pragma warning(disable:4786)
#include "Npc.h"
#include "InstanceNPC.h"
#include "MsgMapInstance.h"
#include "MapGroup.h"
#include "ActionDefine.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInstanceNPC::CInstanceNPC()
{
	m_nInstanceType=0;
	m_idOwn=0;
	m_strRoomPwd="abc";

	///�����������
//	MakeTestData();
}

CInstanceNPC::~CInstanceNPC()
{

}

void CInstanceNPC::RequestJoinRoom(OBJID idPlayer)
{
	CUser * pRequestUser=UserManager()->GetUser(idPlayer);
	if(pRequestUser && HasPassword())
		pRequestUser->ProcessActionUser(ACTION_USER_OPEN_DIALOG,"",44);///����򿪸�������
	else
		JoinRoom(idPlayer);
}

BOOL CInstanceNPC::JoinRoom(OBJID idPlayer)
{
	BOOL hr=true;
	CUser * pUser=UserManager()->GetUser(idPlayer);
	CString strScriptFile=m_pData->GetStr("join_script");
	hr=GameAction()->ProcessScriptAction(ACTION_SCRIPT_PRESENT,strScriptFile,0,pUser,this);
	///���������ļ���ű�
	if (strScriptFile.CompareNoCase("N/A")==0 || hr)
	{
		AddPlayer(idPlayer);
		RefreshClientInstanceInfo(idPlayer);
	}
	return hr;
}

void CInstanceNPC::MakeTestData()///������ٵĲ�������
{
	for(int i=1000;i<1020;i++)
		AddPlayer(i,RandGet(8));
}

void CInstanceNPC::RefreshInstanceInfo(OBJID idPlayer)///ˢ�¿ͻ��˸�����Ϣ
{
	RefreshClientInstanceInfo(idPlayer);
}

void CInstanceNPC::RefreshClientInstanceInfo(OBJID idPlayer,UINT nCurrentPage)
{
	CUser * pUser=UserManager()->GetUser(idPlayer);
	CHECK(pUser);
	CMsgMapInstance msg(m_idProcess);
	msg.CreateRefreshClientMsg(this,nCurrentPage);
	pUser->SendMsg(&msg);
}


void CInstanceNPC::BroadcastMsg(CNetMsg& msg)///ȫ����Ա���й㲥
{
	CUserPtr pUser=NULL;
	for (InstanceUserList::iterator it=m_InstanceUserList.begin();it!=m_InstanceUserList.end();++it)
	{
		pUser=UserManager()->GetUser(it->first);
		if (pUser)
		{
			pUser->SendMsg(&msg);
		}
	}
}

void CInstanceNPC::CloseRoom()///�ر���������
{
	///������Ϣ
	CMsgMapInstance msg;
	msg.CreateRequestCloseMsg(this);
	BroadcastMsg(msg);
	///�����ر��˷���
	///���¿ͻ���
	///ɾ��NPC
//	CMsgMapInstance msg;
//	msg.CreateRequestEnterMsg(CurrentMapGroup()->GetID(),GetID(),GetID(),m_nInstanceType);///����������븱������Ϣ
	CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_INSTANCE,&msg);
}

void CInstanceNPC::RequestExitRoom(OBJID idRequest)///ˢ�¿ͻ��˸�����Ϣ
{
	if(IsRoomOwn(idRequest))
		CloseRoom();
	else
		KickPlayer(GetOwnID(),idRequest);
}

CInstanceUser* CInstanceNPC::FindInstanceUser(OBJID idInstanceUser)
{
	if(m_InstanceUserList.find(idInstanceUser)!=m_InstanceUserList.end())
		return &m_InstanceUserList[idInstanceUser];
	else
		return NULL;
}

CInstanceUser* CInstanceNPC::GetInstanceUser(UINT nIndex)
{
	///�����Ч����
	CHECKF(nIndex<GetInstanceUserSize());
	InstanceUserList::iterator it=m_InstanceUserList.begin();
	advance(it,nIndex);
	return &it->second;
}

void CInstanceNPC::TransmitPlayer(CInstanceUser& player)
{
	///��һ����Ҷ���������ȥ
	CUser* pUser=UserManager()->GetUser(player.GetID());
	if (pUser)
	{
		slotPlayerEnterInstance(player.GetID());
		///���ߴ���֮
		pUser->EnterInstance(GetInstanceMapID(),GetInstanceMapType(),0,0,0,0);
	}
	else
	{
		///�Է������߻��뿪��ͼ����
		RemovePlayer(player.GetID());
	}
}

void CInstanceNPC::NotifyUserStartInstance(OBJID idPlayer)///֪ͨ�ͻ��˸�����ʼ��
{
	CUser* pUser=UserManager()->GetUser(idPlayer);
	CHECK(pUser);
	CMsgMapInstance msg;
	msg.CreateRequestStartMsg(this);
	pUser->SendMsg(&msg);
}

BOOL CInstanceNPC::ChangeTeamGroup(OBJID idPlayer,UINT nTeamGroup)///�ı���Ҷ���
{
	CInstanceUser* pInstanceUser=FindInstanceUser(idPlayer);
	CHECKF(pInstanceUser);
	pInstanceUser->SetTeamGroup(nTeamGroup);
	RefreshClientInstanceInfo(idPlayer);
	return true;
}

void CInstanceNPC::KickPlayer(OBJID idRequest,OBJID idPlayer)///�߳����
{
	///��֤���������
	///ɾ����Ϣ
	///���¿ͻ�����Ϣ.
	RemovePlayer(idPlayer);
	RefreshClientInstanceInfo(idRequest);
	RefreshClientInstanceInfo(idPlayer);
}

void CInstanceNPC::ChangeOwn(OBJID idRequest,OBJID idPlayer)///�ı丱����������Ϣ
{
	///��֤���������
	///���������������Ϣ
	///���¿ͻ�����Ϣ.
	SetOwnID(idPlayer);
	RefreshClientInstanceInfo(idRequest);
}

void CInstanceNPC::AutoCreateTeam()
{
	return;
	BOOL hr=false;
	CUser* pUser=NULL;
	CInstanceUser* pInstanceUser=NULL;
	map<UINT,CTeam*> TeamList;
	CTeam* pTeam=NULL;
	OBJID idLeader=ID_NONE;
	for(int i=0;i<GetInstanceUserSize();++i)
	{
		pInstanceUser=GetInstanceUser(i);
		CHECK(pInstanceUser);
		pUser=UserManager()->GetUser(pInstanceUser->GetID());
		CHECK(pUser);
		pTeam=TeamList[pInstanceUser->GetTeamGroup()];
		if(!pTeam)
		{
			pUser->CreateDefaultTeam();
			pTeam=pUser->GetTeam();
			TeamList[pInstanceUser->GetTeamGroup()]=pTeam;
		}
		else
		{
			CUser* pLeader=UserManager()->GetUser(pTeam->GetLeader());
			pUser->AcceptTeamInvit(pLeader,true);
		}
		//pInstanceUser->GetTeamLimitTimer()
		///TransmitPlayer(*);
	}	
}

void CInstanceNPC::OnPlayerEnterInstance(OBJID idPlayer)///�����Լ��������û����븱���¼�,׼���ر��û��ĸ����Ի���
{
//	CUser* pUser=UserManager()->GetUser(idPlayer);
//	CHECK(pUser);
	NotifyUserStartInstance(idPlayer);
}

void CInstanceNPC::TransmitAllPlayer()
{
	for(int i=0;i<GetInstanceUserSize();++i)
	{
		TransmitPlayer(*GetInstanceUser(i));
	}
}

void CInstanceNPC::StartInstance(OBJID idRequest)///����ͻ��˷������Ŀ�ʼ����������
{
	///�жϸ��������Ƿ�Ϊ����
	CHECK(idRequest==m_idOwn);
	AutoCreateTeam();
	TransmitAllPlayer();
}

void CInstanceNPC::ReplyCreateInstance(OBJID idInstanceMap,UINT nInstanceMapType)
{
	m_idInstanceMap=idInstanceMap;
	m_nInstanceMapType=nInstanceMapType;
//	RefreshClientInstanceInfo();
}

BOOL CInstanceNPC::CreateRoom(OBJID idPlayer)
{
	BOOL hr=false;
	///��5�ŵ�ͼ�鷢����,��֮��һ��User��ȥָ�����͵ĸ���
	CHECKF(JoinRoom(idPlayer));///�������ӵ�������
	CMsgMapInstance msg;
	msg.CreateRequestEnterMsg(CurrentMapGroup()->GetID(),GetID(),GetID(),m_nInstanceType,0,0,0,0,0);///����������븱������Ϣ
	hr=CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_INSTANCE,&msg);
	return hr;
}

void CInstanceNPC::CheckRoomPassword(OBJID idRequest,const CString& strRoomPwd)///��鸱����������
{
//	CUserPtr pRequestUser=UserManager()->GetUser(idRequest);
	if (/*pRequestUser &&*/ strRoomPwd==m_strRoomPwd)
	{
		///����򿪸�������
		JoinRoom(idRequest);
	}
	else
	{
		///���ش�����ʾ
	}
}

void CInstanceNPC::ChangeRoomPassword(OBJID idRequest,const CString& strRoomPwd)///�ı丱���������
{
	///��֤���������
	///������Ϣ
	///���¿ͻ�����Ϣ.
	SetRoomPassword(strRoomPwd);
//	RefreshClientInstanceInfo(idRequest);
}

void CInstanceNPC::ChangeRoomTitle(OBJID idRequest,const CString& strRoomTitle)///�ı丱���������
{
	///��֤���������
	///������Ϣ
	///���¿ͻ�����Ϣ.
	SetRoomTitle(strRoomTitle);
	RefreshClientInstanceInfo(idRequest);
}

bool CInstanceNPC::InitNpc(int nParam1,int nParam2)
{
	///nParam1Ϊ�����ߵ�ID,nParam2Ϊ���󴴽��ĸ������͵�ID
	bool hr=false;
	m_idOwn=nParam1;/////////////////////////////////////////////////////////////////////////////
	m_nInstanceType=nParam2;
	m_pData=InstanceTypeSet()->GetObj(m_nInstanceType);
	m_strRoomTitle=m_pData->GetStr("name");
	m_nInstanceMapType=m_pData->GetInt("maptype");
	CHECKF(m_pData);
	hr=CreateRoom(m_idOwn)>0;
	slotPlayerEnterInstance.connect(this,&CInstanceNPC::OnPlayerEnterInstance);
	return  hr;
}

void CInstanceNPC::AddPlayer(OBJID idPlayer,UINT nTeamGroup)
{
	BOOL hr=false;
	CInstanceUser iu;
	iu.SetID(idPlayer);
	iu.SetTeamGroup(nTeamGroup);
	AddPlayer(iu);
}

void CInstanceNPC::AddPlayer(CInstanceUser& instanceuser)
{
	m_InstanceUserList[instanceuser.GetID()]=instanceuser;
}

void CInstanceNPC::RemovePlayer(OBJID idPlayer)
{
	m_InstanceUserList.erase(idPlayer);
}