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

	///生成虚假数据
//	MakeTestData();
}

CInstanceNPC::~CInstanceNPC()
{

}

void CInstanceNPC::RequestJoinRoom(OBJID idPlayer)
{
	CUser * pRequestUser=UserManager()->GetUser(idPlayer);
	if(pRequestUser && HasPassword())
		pRequestUser->ProcessActionUser(ACTION_USER_OPEN_DIALOG,"",44);///允许打开副本介面
	else
		JoinRoom(idPlayer);
}

BOOL CInstanceNPC::JoinRoom(OBJID idPlayer)
{
	BOOL hr=true;
	CUser * pUser=UserManager()->GetUser(idPlayer);
	CString strScriptFile=m_pData->GetStr("join_script");
	hr=GameAction()->ProcessScriptAction(ACTION_SCRIPT_PRESENT,strScriptFile,0,pUser,this);
	///触发副本的加入脚本
	if (strScriptFile.CompareNoCase("N/A")==0 || hr)
	{
		AddPlayer(idPlayer);
		RefreshClientInstanceInfo(idPlayer);
	}
	return hr;
}

void CInstanceNPC::MakeTestData()///生成虚假的测试数据
{
	for(int i=1000;i<1020;i++)
		AddPlayer(i,RandGet(8));
}

void CInstanceNPC::RefreshInstanceInfo(OBJID idPlayer)///刷新客户端副本信息
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


void CInstanceNPC::BroadcastMsg(CNetMsg& msg)///全部成员进行广播
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

void CInstanceNPC::CloseRoom()///关闭整个副本
{
	///更新信息
	CMsgMapInstance msg;
	msg.CreateRequestCloseMsg(this);
	BroadcastMsg(msg);
	///房主关闭了房间
	///更新客户端
	///删除NPC
//	CMsgMapInstance msg;
//	msg.CreateRequestEnterMsg(CurrentMapGroup()->GetID(),GetID(),GetID(),m_nInstanceType);///生成请求进入副本的消息
	CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_INSTANCE,&msg);
}

void CInstanceNPC::RequestExitRoom(OBJID idRequest)///刷新客户端副本信息
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
	///检查有效索引
	CHECKF(nIndex<GetInstanceUserSize());
	InstanceUserList::iterator it=m_InstanceUserList.begin();
	advance(it,nIndex);
	return &it->second;
}

void CInstanceNPC::TransmitPlayer(CInstanceUser& player)
{
	///将一个玩家丢到副本中去
	CUser* pUser=UserManager()->GetUser(player.GetID());
	if (pUser)
	{
		slotPlayerEnterInstance(player.GetID());
		///在线传送之
		pUser->EnterInstance(GetInstanceMapID(),GetInstanceMapType(),0,0,0,0);
	}
	else
	{
		///对方不在线或离开地图组了
		RemovePlayer(player.GetID());
	}
}

void CInstanceNPC::NotifyUserStartInstance(OBJID idPlayer)///通知客户端副本开始了
{
	CUser* pUser=UserManager()->GetUser(idPlayer);
	CHECK(pUser);
	CMsgMapInstance msg;
	msg.CreateRequestStartMsg(this);
	pUser->SendMsg(&msg);
}

BOOL CInstanceNPC::ChangeTeamGroup(OBJID idPlayer,UINT nTeamGroup)///改变玩家队伍
{
	CInstanceUser* pInstanceUser=FindInstanceUser(idPlayer);
	CHECKF(pInstanceUser);
	pInstanceUser->SetTeamGroup(nTeamGroup);
	RefreshClientInstanceInfo(idPlayer);
	return true;
}

void CInstanceNPC::KickPlayer(OBJID idRequest,OBJID idPlayer)///踢出玩家
{
	///检证请求者身份
	///删除信息
	///更新客户端信息.
	RemovePlayer(idPlayer);
	RefreshClientInstanceInfo(idRequest);
	RefreshClientInstanceInfo(idPlayer);
}

void CInstanceNPC::ChangeOwn(OBJID idRequest,OBJID idPlayer)///改变副本所有者信息
{
	///检证请求者身份
	///变更副本所有者信息
	///更新客户端信息.
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

void CInstanceNPC::OnPlayerEnterInstance(OBJID idPlayer)///处理自己产生的用户进入副本事件,准备关闭用户的副本对话框
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

void CInstanceNPC::StartInstance(OBJID idRequest)///处理客户端发过来的开始副本的请求
{
	///判断该请求者是否为房主
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
	///向5号地图组发请求,告之有一个User想去指定类型的副本
	CHECKF(JoinRoom(idPlayer));///将房主加到房间中
	CMsgMapInstance msg;
	msg.CreateRequestEnterMsg(CurrentMapGroup()->GetID(),GetID(),GetID(),m_nInstanceType,0,0,0,0,0);///生成请求进入副本的消息
	hr=CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_INSTANCE,&msg);
	return hr;
}

void CInstanceNPC::CheckRoomPassword(OBJID idRequest,const CString& strRoomPwd)///检查副本房间密码
{
//	CUserPtr pRequestUser=UserManager()->GetUser(idRequest);
	if (/*pRequestUser &&*/ strRoomPwd==m_strRoomPwd)
	{
		///允许打开副本介面
		JoinRoom(idRequest);
	}
	else
	{
		///发回错误提示
	}
}

void CInstanceNPC::ChangeRoomPassword(OBJID idRequest,const CString& strRoomPwd)///改变副本房间标题
{
	///检证请求者身份
	///更新信息
	///更新客户端信息.
	SetRoomPassword(strRoomPwd);
//	RefreshClientInstanceInfo(idRequest);
}

void CInstanceNPC::ChangeRoomTitle(OBJID idRequest,const CString& strRoomTitle)///改变副本房间标题
{
	///检证请求者身份
	///更新信息
	///更新客户端信息.
	SetRoomTitle(strRoomTitle);
	RefreshClientInstanceInfo(idRequest);
}

bool CInstanceNPC::InitNpc(int nParam1,int nParam2)
{
	///nParam1为创造者的ID,nParam2为请求创建的副本类型的ID
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