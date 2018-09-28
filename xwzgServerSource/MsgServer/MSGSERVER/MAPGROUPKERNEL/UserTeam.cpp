//
//
// UserTeam.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include "AllMsg.h"
#include "User.h"
#include "MapGroup.h"
#include "netmsg.h"
#include "CombGod.h"
#include "SKPostOffice.h"

const int RULE_FRIEND        = 1;
const int RULE_MARRY         = 2;
const int RULE_FACTION       = 4;
const int RULE_STRANGER      = 8;

const int TEAM_SEARCH_INTERVAL = 3;
const int HOUSE_ALLHOUSE_INTERVAL = 60;

BOOL CUser::CreateDefaultTeam()///增强的创建组队方式
{
	BOOL hr=false;
	int teamInfo = TEAMINFO_LOCK|TEAMINFO_EXP_PRIVATE|TEAMINFO_ITEM_PRIVATE;
	
	if(CreateTeam())
	{
		CMsgTeam msgCreateTeam;
		if (msgCreateTeam.Create(GetID(),_MSGTEAM_CREATE, GetMapID()))
			SendMsg(&msgCreateTeam);

		CTeam* pTeam = GetTeam();
		pTeam->SetTeamMode(teamInfo & 0x3);//取1，2位
		pTeam->SetExpMode((teamInfo & 0x0c)>>2);//取3，4位
		pTeam->SetItemMode((teamInfo & 0x70)>>4);//取5，6，7位
		pTeam->SetTeamName("team");
		
		CMsgTeam msgJoinTeam;
		if (msgJoinTeam.Create(GetID(),(const char*)NULL,_MSGTEAM_JOINTEAM))
			BroadcastRoomMsg(&msgJoinTeam,true);

		CMsgTeam msgLeaderApply;
		//20070205修罗:队长设置要发包.---------------------
		if (msgLeaderApply.Create(pTeam->GetLeader(), _MSGTEAM_CHG_LEADER_APPLY, pUser->GetMapID()))
		//包括自己
			pTeam->BroadcastTeamMsg(&msgLeaderApply);
	}
	return hr;
}

///上线后尝试重建队伍
// void CUser::RebuildTeam()
// {
// 	DEBUG_TRY
// 	///通过自己的id找到所属的队伍,如果存在的话
// 	CTeam* pTeam	= NULL;
// 	UINT nTeamCount=UserManager()->GetTeamAmount();
// 	UINT nMemberCount=0;
// 	OBJID idMember=ID_NONE;
// 	for(int i=0;i<nTeamCount;++i)
// 	{
// 		pTeam=UserManager()->GetTeamByIndex(i);
// 		CHECK(pTeam);///绝对不会出错
// 		nMemberCount=pTeam->GetMemberAmount();
// 		for(int n=0;n<nMemberCount;++n)
// 		{
// 			idMember=pTeam->GetMemberByIndex(n);
// 			if(GetID()==idMember)
// 			{
// 				pTeam->SetOnlineStatus(GetID(),true,true);///在线状态设置成上线
// 				SetTeam(pTeam);///找到组织了,重建队伍成成功
// 				return;
// 			}
// 		}
// 	}
// // 	if(GetTeam())
// // 	{
// // 		///将逻辑从循环中提取出来以便清晰
// // 		///队伍信息同步给自己和队伍中其它人
// // 		SendTeamMemberInfo();
// // 	}
// 	DEBUG_CATCH("CUser::RebuildTeam()")
// }

//////////////////////////////////////////////////////////////////////
BOOL CUser::CreateTeam()
{
	// 判断玩家是否已经有队伍了。
	if (this->GetTeam())
	{
		this->SendSysMsg(g_objStrRes.GetStr(13008));//STR_TEAMMATE_CANNOT_CREATE
		return false;
	}

	CTeam* pTeam	= CTeam::CreateNew(m_idProcess, UserManager()->SpawnNewTeamID(), this->GetID(),true);
	if (!pTeam)
	{
		this->SendSysMsg(g_objStrRes.GetStr(13009));//STR_CREATE_TEAM_FAILED
		return false;
	}

	this->SetTeam(pTeam);

//	UserManager()->AddTeamLeader(this->GetID());

 	UserManager()->AddTeam(pTeam);


//	if (GetMap() && GetMap()->IsDynaMap())
//	{
//		ParamList paramlist;
//		paramlist.push_back(CComVariant((long)GetID()));
//		paramlist.push_back(CComVariant((long)GetMap()->GetID()));
//		paramlist.push_back(CComVariant((long)pTeam->GetID()));
//		soltPlayerTeamCreate(paramlist);
//	}
	return true;
}

//////////////////////////////////////////////////////////////////////
CTeam* CUser::GetTeam()
{
	if (!m_pTeam)
		return NULL;

	return m_pTeam;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::AcceptTeamMember(CUser* pMember)
{
	if (!pMember)
		return false;

	if(m_pTeam->AddMember(pMember->GetID()))
	{
//		this->SendShow(pMember);


// 		CCombGod::GetCombGod()->AddNewUser(pMember,m_pTeam->GetID());

/*		pMember->SendSysMsg(_TXTATR_TEAM, STR_TEAM_MONEY_s, m_pTeam->IsCloseMoney() ? STR_CLOSE : STR_OPEN);
		pMember->SendSysMsg(_TXTATR_TEAM, STR_TEAM_ITEM_s, m_pTeam->IsCloseItem() ? STR_CLOSE : STR_OPEN);
		pMember->SendSysMsg(_TXTATR_TEAM, STR_TEAM_GEM_s, m_pTeam->IsCloseGemAccess()? STR_CLOSE : STR_OPEN);
*/		return true;
	}
	return false;
}

BOOL CUser::AcceptTeamInvit(CUser* pLeader,BOOL bSynchro)
{
	CHECKF(pLeader);
	CTeam* pTeam=pLeader->GetTeam();
	CHECKF(pTeam);
	if (AcceptTeamInvit(pLeader) && bSynchro==TRUE)
	{
		CMsgTeam msg;
		if (msg.Create(GetID(), (char*)GetName(), _MSGTEAM_JOINTEAM))
			BroadcastRoomMsg(&msg,true);
	}
	return true;
}

///清除并释放玩家身上的所有物品CD,防止内存泄露
void CUser::ClearAllCD()
{
	DEBUG_TRY
		for(MAP_ITEMDELAY_Iter it = m_setItemDelay.begin();it != m_setItemDelay.end();it++)
		{
			CTimeOutMS* pTemp = it->second;
			SAFE_DELETE(pTemp);
		}
		m_setItemDelay.clear();
	DEBUG_CATCH("CUser::ClearAllCD释放时出错了")
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::AcceptTeamInvit(CUser* pLeader)
{
	if (!pLeader)
		return false;

	if(!IsInviteUserID(pLeader->GetID()))
		return false;

	ClrInviteUserID(pLeader->GetID());

	CTeam* pTeam	= pLeader->GetTeam();
	if(pTeam->AddMember(this->GetID()))
	{
//		pLeader->SendShow(this);

/*		this->SendSysMsg(_TXTATR_TEAM, STR_TEAM_MONEY_s, pTeam->IsCloseMoney() ? STR_CLOSE : STR_OPEN);
		this->SendSysMsg(_TXTATR_TEAM, STR_TEAM_ITEM_s, pTeam->IsCloseItem() ? STR_CLOSE : STR_OPEN);
		this->SendSysMsg(_TXTATR_TEAM, STR_TEAM_GEM_s, pTeam->IsCloseGemAccess()? STR_CLOSE : STR_OPEN);
*/		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::QuitTeam()
{
	if (!m_pTeam)
		return false;

	if (this->GetID() == m_pTeam->GetLeader())
	{
		AbandonTeamLead();
		// By Minsky,2010-05-28，当队长退出时，转交队长
// 		int nAmount = m_pTeam->GetMemberAmount();
// 		int idMember = ID_NONE;
// 		for (int i=0; i<nAmount; i++)
// 		{
// 			idMember = m_pTeam->GetMemberByIndex(i);
// 			if (idMember != m_pTeam->GetLeader())
// 			{
// 				m_pTeam->ChangeTeamLeader(idMember);
// 				break;
// 			}
// 		}
	}
	ClrTeam();
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::KickOutTeamMem(OBJID idMember)
{
	if (!m_pTeam || idMember == ID_NONE || idMember == GetID())
		return false;

	m_pTeam->DelMember(idMember);
	CUser* pMember = UserManager()->GetUser(idMember);
	if(pMember)
	{
// 		CMsgTeam msg;
// 		if (msg.Create(idMember,pMember->GetName(), _MSGTEAM_KICKOUT))
// 			m_pTeam->BroadcastTeamMsg(&msg);

		pMember->ClrTeam(true);	
	}
	return true;
}

BOOL CUser::DenyTeamInvite(OBJID idPlayer, char *pText)
{  
	MSGBUF	szMsg;
    vsprintf( szMsg, pText, (char*) ((&pText)+1) );
	   
	CMsgTalk msg;
    if (msg.Create(SYSTEM_NAME, "", szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
	{
		CUser *pTarget = UserManager()->GetUser(idPlayer);
		if(pTarget)
		{
			pTarget->SendMsg(&msg);
		}
		else
		{
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idPlayer);
		}
	 }
	   
    return TRUE;
}

BOOL CUser::ChangeTeamText(char *pText)
{
	if (!m_pTeam )
		return false;

	if(m_pTeam->GetLeader() != GetID())
	{
		SendSysMsg("您不是队长，不能修改队伍描述");
		return false;
	}
	else
	{
		m_pTeam->ChangeTeamText(pText);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::AbandonTeamLead()///如果队长退出,则移交队长给下一人
{
	BOOL hr=false;
	DEBUG_TRY
	if (m_pTeam && GetID() == m_pTeam->GetLeader())
	{	 
		OBJID idLeader=m_pTeam->GetNextTeamLeader();//GetOnlineTeamMemberOnly();
		if (idLeader!=ID_NONE)
		{
			m_pTeam->ChangeTeamLeader(idLeader);
			CUserPtr pLeader = UserManager()->GetUser(idLeader);
			if(pLeader)
			{
				CMsgTeam msg;
				if(msg.Create(idLeader,pLeader->GetName(), _MSGTEAM_CHG_LEADER_APPLY))
				{
					m_pTeam->BroadcastTeamMsg(&msg);
				}
				
				CMsgPlayer msgPlayer;
				if (msgPlayer.Create(pLeader->QueryRole()))
					pLeader->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
			}
			else
			{
				TeamMemberInfo info;
				ZeroMemory(&info,sizeof(TeamMemberInfo));
				if(GetTeam())
				{
					GetTeam()->GetMemberInfo(info,idLeader);
					CMsgTeam msg;
					if(msg.Create(idLeader,info.szName, _MSGTEAM_CHG_LEADER_APPLY))
					{
						GetTeam()->BroadcastTeamMsg(&msg);
					}
				}
			}
			
			// 			if (msgPlayer.Create(QueryRole()))
			// 				BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
		else
			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_REQUESTCHANGLEADER, m_pTeam->GetID(), 0);///通知别的地图组自己选择队长
	}
	DEBUG_CATCH("CUser::AbandonTeamLead()")
	return hr;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::DismissTeam()
{
	if (!m_pTeam)
		return false;

	if(GetID() != m_pTeam->GetLeader())
	{
		SendSysMsg("[队伍]您不是队长，不能解散队伍。");
		return false;
	}

// 	CCombItem ci;
// 	BOOL bIsInComb = CCombGod::GetCombGod()->IsUserInComb(GetID(),ci);
// 	
// 	if(bIsInComb)
// 	{
// 		CCombGod::GetCombGod()->CancelComb(this);
// 	}

	CMsgTeam msg;
	if (msg.Create(this->GetID(), _MSGTEAM_DISMISS))
		m_pTeam->BroadcastTeamMsg(&msg);


	this->ClsStatus(STATUS_TEAMLEADER);

	m_pTeam->Dismiss(this->GetID());
	this->ClrTeam(false);
	UserManager()->DelTeamLeader(GetID());
	return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SetTeam(CTeam* pTeam)
{
	//ASSERT(m_pTeam == 0 && pTeam);
	m_pTeam	= pTeam;
}

//////////////////////////////////////////////////////////////////////
void CUser::ClrTeam(BOOL bKickOut,bool bLeave)
{
	if(m_pTeam )
	{
		if(bLeave)
		{
			CMsgTeam msg;
			CUser * pLeader=UserManager()->GetUser(m_pTeam->GetLeader());
			if(bKickOut)
			{
				m_pTeam->DelMember(this->GetID());
				if (!msg.Create(GetID(),GetName(),_MSGTEAM_KICKOUT ))
					return;
				SendMsg(&msg);
				if(pLeader)
					pLeader->SendMsg(&msg);
				if (!msg.Create(GetID(),GetName(),_MSGTEAM_LEAVE ))
					return;
				//if(pLeader)
				m_pTeam->BroadcastTeamMsg(&msg);			
			}
			else if(bLeave)
			{
				///解散队伍时不广播离队消息
				if(m_pTeam->GetOnlineMemberAmount()>1)
				{
					if (!msg.Create(GetID(),GetName(),_MSGTEAM_LEAVE ))
						return;
					m_pTeam->BroadcastTeamMsg(&msg);
				}
				m_pTeam->DelMember(this->GetID());
			}
			// 			else
			// 			{
			// 			}
			// 			if(bKickOut)
			// 				this->SendMsg(&msg);
			
			// 			if(m_pTeam->GetMemberAmount() == 0)
			if(m_pTeam->GetOnlineMemberAmount() <= 0)
			{
				MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_DES, m_pTeam->GetID(), 0);
				UserManager()->DelTeam(m_pTeam);
				
				//m_pTeam->ReleaseByOwner();
			}
			m_pTeam = NULL;
		}
		if (bKickOut)
			m_pTeam = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessTeamTimer()
{
	CTeam* pTeam = this->GetTeam();
	if (!pTeam)
		return;
	//-------彭峰---2007/06/01-----现在全部都要发，不仅仅可以看到队长，队长也可以看到队员！
/*
	OBJID idLeader = pTeam->GetLeader();
	if (this->GetID() != idLeader)
		return;
*/
	int nPosX = this->GetPosX();
	int nPosY = this->GetPosY();

	CMsgAction msg;
	IF_OK (msg.Create(this->GetID(), nPosX, nPosY, 0, actionTeamMemeberPos, this->GetMapID()))
	{
		int nTeamMembers = pTeam->GetMemberAmount();
		for (int i=0; i<nTeamMembers; i++)
		{
			OBJID idUser = pTeam->GetMemberByIndex(i);
			//-------彭峰---2007/06/01-----如果是自己就不发
			if( idUser == GetID() )
				continue;
			//--------------------------------------------
			CUser* pUser = UserManager()->GetUser(idUser);
			if (pUser && pUser->GetMapID() == this->GetMapID())
					pUser->SendMsg(&msg);
		}
	}
}

//////////////////////////////////////////////////////////////////////
//20070205修罗:注释一下
//此函数仅用来同步客户端
//User做为队长管理小队.没有任何特殊处理.一般而言,User一定为队长.
//target为要加入小队的玩家.
//这其实就是一个[介绍]的过程
//1.将所有小队现有成员介绍给target.循环单个发送
//2.将target介绍给小队现有所有成员.用BroadcastTeamMsg()
void CUser::SendShow(CUser* pTarget)
{
	CHECK(GetTeam());

	// msg
	TeamMemberInfo	info;
	info.id			= pTarget->GetID();
	info.dwLookFace	= pTarget->GetLookFace();
	info.dwLev      = pTarget->GetLev();
	info.usProfession = pTarget->GetProfession();
// 	info.usHp		= pTarget->GetLife();
// 	info.usMaxHp	= pTarget->GetMaxLife();
// 	info.usMp		= this->GetMana();
// 	info.usMaxMp	= this->GetMaxMana();
// 	info.usMapID    = pTarget->GetMapID();
	strcpy(info.szName, pTarget->GetName());

	for(int i = 0; i < GetTeam()->GetMemberAmount(); i++)
	{
		OBJID idMember = GetTeam()->GetMemberByIndex(i);
		CUser* pMember = UserManager()->GetUser(idMember);
		if(pMember)
		{
			info.id			= pMember->GetID();
			info.dwLookFace	= pMember->GetLookFace();
			info.dwLev      = pMember->GetLev();
			info.usProfession = pMember->GetProfession();
// 			info.usHp		= pMember->GetLife();
// 			info.usMaxHp	= pMember->GetMaxLife();
// 			info.usMp		= pMember->GetMana();
// 			info.usMaxMp	= pMember->GetMaxMana();
			strcpy(info.szName, pMember->GetName());
//			info.usMapID    = pMember->GetMapID();

			CMsgTeamMember msg;
			IF_OK(msg.Create(_MSG_TEAMMEMBER_ADDMEMBER, info))
				pTarget->SendMsg(&msg);

			for(int i =  pMember->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
			{
				IStatus* pStatus =  pMember->QueryStatusSet()->GetObjByIndex(i);
				if( pStatus)
				{
					CMsgBuff msgBuff;
					if(msgBuff.Create( ACTION_ADDSTATUS,pMember->GetID(),pStatus->GetID(),pStatus->GetSecLeft(),0,0,0,0,0 ))
						pTarget->SendMsg(&msgBuff);
				}
			}
			for(i =  pTarget->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
			{
				IStatus* pStatus =  pTarget->QueryStatusSet()->GetObjByIndex(i);
				if( pStatus)
				{
					CMsgBuff msgBuff;
					if(msgBuff.Create( ACTION_ADDSTATUS,pTarget->GetID(),pStatus->GetID(),pStatus->GetSecLeft(),0,0,0,0,0 ))
						pMember->SendMsg(&msgBuff);
				}
			}
		}
		else // transmit
		{
			MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_INFO, GetTeam()->GetID(), pTarget->GetID(), pTarget->GetSocketID());
		}
	}

	CMsgTeamMember msg;
	if (msg.Create(_MSG_TEAMMEMBER_ADDMEMBER, info))
		GetTeam()->BroadcastTeamMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
// IChangeTeam
//////////////////////////////////////////////////////////////////////
bool CUser::AddTeamMember(OBJID idUser)
{
// 	CHECKF(GetTeam());
// 
// 	TeamMemberInfo info;
// 	info.id = idUser;
// 	GetTeam()->AddMemberOnly(info);

	//  [2007-9-27] 朱斌	加入新队员后更新
//	UpdateBroadcastSet();
	

// 	GetTeam()->AddBuffByTutor(idUser);// pengfeng [10/26/2007]
	
	

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelTeamMember(OBJID idUser)
{
// 	CHECKF(GetTeam());
// 
// 	if(idUser == GetID())
// 		this->ClrTeam();
// 	else
// 	{
// // 		GetTeam()->DelBuffByTutor( idUser);// pengfeng [10/26/2007]
// 		
// 		GetTeam()->DelMemberOnly(idUser);
// 	}


//	if(idUser == GetID())
//	{
//		CMsgTeam msg;
//		if (msg.Create(GetID(), "", _MSGTEAM_CHG_NAME_APPLY, 0, ""))
//			BroadcastRoomMsg(&msg, true);
//	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelTeam()
{
	this->ClrTeam();

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendInfo(SOCKET_ID idSocket, OBJID idAgent)
{
	CHECKF(GetTeam());

	// msg
	TeamMemberInfo	info;
	info.id			= this->GetID();
	info.dwLookFace	= this->GetLookFace();
// 	info.usHp		= this->GetLife();
// 	info.usMaxHp	= this->GetMaxLife();
// 	info.usMp		= this->GetMana();
// 	info.usMaxMp	= this->GetMaxMana();
// 	info.dwLev      = this->GetLev();
// 	info.usMapID    = this->GetMapID();
	info.usProfession = this->GetProfession();
	strcpy(info.szName, this->GetName());

	CMsgTeamMember msg;
	if (msg.Create(_MSG_TEAMMEMBER_ADDMEMBER, info))
	{
// 		if(idSocket == SOCKET_NONE)
// 			MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(idAgent, &msg);
		if(idSocket != SOCKET_NONE)
			MapGroup(PID)->QueryIntraMsg()->SendClientMsg(idSocket, &msg);
	}
	return true;
}

void CUser::AppendAwardInfo(AwardInfoStruct* pInfo)
{
	if(GetAward()%10 == pInfo->nTime)
		SetNextAwardTime(pInfo->nLeave);
}
//////////////////////////////////////////////////////////////////////
void CUser::AppendTeam(TeamInfoStruct* pInfo)
{
	m_pTeam = UserManager()->GetTeamById(pInfo->id);
// 	if(!m_pTeam)
// 	{
// 		m_pTeam = CTeam::CreateNew(m_idProcess, pInfo);
// 		UserManager()->AddTeam(m_pTeam);
// 	}
	CHECK(m_pTeam);

	///查看自己是否在队伍中,如果不在则出日志警告
	if(m_pTeam->IsTeamMember(GetID())==false)
		LogSave("CUser::AppendTeam %s 尝试将自己加入到一个不属于自己的队伍中!",GetName());

	
	if(GetID() == m_pTeam->GetLeader())
	{
		m_pTeam->SetTeamLeaderName(GetName());
	}
	//UserManager()->AddTeamLeader(pInfo->idLeader);
}

bool CUser::CheckSearchRole()
{
	if(::TimeGet(TIME_SECOND) - m_nSearchRoleTime > TEAM_SEARCH_INTERVAL) //3秒钟查一次
	{
		m_nSearchRoleTime = ::TimeGet(TIME_SECOND);
		return true;
	}
	else
	{
		return false;
	}
}

bool CUser::CheckSearchTeam() //3秒钟查一次
{
	if(::TimeGet(TIME_SECOND) - m_nSearchTeamTime > TEAM_SEARCH_INTERVAL)
	{
		m_nSearchTeamTime = ::TimeGet(TIME_SECOND);
		return true;
	}
	else
	{
		return false;
	}
}

bool CUser::CheckSearchHouse()
{
	if(::TimeGet(TIME_SECOND) - m_nSearchHouseTime > HOUSE_ALLHOUSE_INTERVAL)
	{
		m_nSearchHouseTime = ::TimeGet(TIME_SECOND);
		return true;
	}
	else
	{
		return false;
	}
}

//20070202修罗:小队更名.仅自己的CTeam更名.不考虑成员
void CUser::ChangeTeamNameOnly(char *teanName)
{
	CHECK(teanName);

	CTeam *pTeam;
	pTeam=this->GetTeam();
	if(pTeam)
	{
		pTeam->SetTeamName(teanName);
	}

	CMsgTeam msg;
	// By Minsky
	if(msg.Create(GetID(), "", _MSGTEAM_CHG_NAME_APPLY))
	{
		BroadcastRoomMsg(&msg, true);
	}
}

void CUser::ActivateShopNpc(DWORD idNpc)
{
	if (idNpc>=PORTABLESHOP_DOWN && idNpc <= PORTABLESHOP_UP)
	{
		CPortableShop *pPShop=GetPPortableShopManager()->QueryShopByOwerid(idNpc);
		if (pPShop)
			pPShop->SendInfo(this,idNpc);
	}
	else
	{
		CNpc* pNpc;
		IRole* pRole = RoleManager()->QuerySet()->GetObj(GetClickNpcID());
		if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && GetMapID() == pNpc->GetMapID())
		{
			if(pNpc->IsDynaNpc())
			{
				if(pNpc->GetNpcType() != idNpc)
				{
					return;
				}
			}
			else
			{
				if(GetClickNpcID() != idNpc)
				{
					return;
				}
			}
			
			DEBUG_TRY
				pNpc->ActivateShop(this->QueryRole());
			DEBUG_CATCH2("pNpc->ActivateShop(%u)",GetID())
		}
		if (!pRole)
		{
			DEBUG_TRY
				pNpc=NpcManager()->QueryShopNpcByNPCType(idNpc);
			if (pNpc)
				pNpc->ActivateShop(this->QueryRole());
			DEBUG_CATCH2("NpcManager()->QueryShopNpcByNPCID(%u)->ActivateShop(this->QueryRole())",GetID())
		}
	}
	
}

bool CUser::IsNearNpc(DWORD idNpc)
{
	CNpc* pNpc;
	IRole* pRole = RoleManager()->QuerySet()->GetObj(idNpc);
	if(NULL == pRole)
		return false;
	if(!(pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc))))
		return false;
	if(GetMapID() != pNpc->GetMapID())
		return false;
	int x = GetPosX() - pRole->GetPosX();
	int y = GetPosY() - pRole->GetPosY();

	if(x < -12 || x > 12)
		return false;
	if(y < -12 || y > 12)
		return false;

	return true;
}

void CUser::UnLockEudemon(OBJID idEudemon)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(pData)
		pData->SetLock(false);
}

void CUser::LockEudemon(OBJID idEudemon)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(pData)
		pData->SetLock(true);
}

bool CUser::IsLockedEudemon(OBJID idEudemon)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(pData)
		return pData->isLocked();
	return true;
}

bool CUser::CanEudemonBorn(OBJID idEudemon)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
		return false;

	if(pData->isLocked())
		return false;

	return true;
}

bool CUser::CanEudemonPlay(OBJID idEudemon)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
		return false;
	
	if(pData->isLocked())
		return false;
	
	return true;
}

bool CUser::CanEudemonBorn()
{
	int amount = m_pUserPet->GetEudmonAmount();
	CUserPetData* pData = NULL;
	for(int i = 0 ; i < amount ; i ++)
	{
		pData = m_pUserPet->QueryDataByIndex(i);
		if(!pData)
			continue;
		if(pData->GetInt(EUDEMONDATA_state) == PETSTATE_BORN/* || pData->GetInt(EUDEMONDATA_state) == 10*/)
			return false;
	}
	return true;
}

bool CUser::CanEudemonPlay()
{
	int amount =  m_pUserPet->GetEudmonAmount();
	CUserPetData* pData = NULL;
	for(int i = 0 ; i < amount ; i ++)
	{
		pData =  m_pUserPet->QueryDataByIndex(i);
		if(!pData)
			continue;
		if(pData->GetInt(EUDEMONDATA_state) == PETSTATE_PLAY)
			return false;
	}
	return true;
}

bool CUser::EudemonSetBorn(OBJID idEudemon,bool bBron , int bronTime)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
		return false;

	if(bBron)
	{
		pData->SetInt(EUDEMONDATA_state,PETSTATE_BORN);
		pData->SetInt(EUDEMONDATA_borntime,bronTime);
		CTime timeNow = CTime::GetCurrentTime();
		pData->SetInt(EUDEMONDATA_lastborn,timeNow.GetMonth() * 1000000 + timeNow.GetDay() * 10000  + timeNow.GetHour() * 100 + timeNow.GetMinute(),true);

		EudemonData data;
		LoadEudemon(idEudemon,data,2);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_DelPet,&data,GetID()))
			SendMsg(&msg);
	}
	else
	{
		pData->SetInt(EUDEMONDATA_state,PETSTATE_REST,true);

		EudemonData data;
		LoadEudemon(idEudemon,data,2);

		char szAwardMsg[128]  = "";
		sprintf(szAwardMsg,"你获得%s宠物",data.name);
		SendSysMsg(szAwardMsg);

		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetInfo,&data,GetID()))
			SendMsg(&msg);
	}
	
	
	return true;
}

bool CUser::EudemonSetPlay(OBJID idEudemon, int nPlayTimes)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
		return false;
	
	pData->SetInt(EUDEMONDATA_state,PETSTATE_PLAY);
	pData->SetInt(EUDEMONDATA_playtime,nPlayTimes);
	CTime timeNow = CTime::GetCurrentTime();
	pData->SetInt(EUDEMONDATA_lastplay,timeNow.GetMonth() * 1000000 + timeNow.GetDay() * 10000  + timeNow.GetHour() * 100 + timeNow.GetMinute(),true);
	
	EudemonData data;
	LoadEudemon(idEudemon,data,2);
	CMsgEudemon msg;
	if(msg.Create(PETACTION_DelPet,&data,GetID()))
		SendMsg(&msg);
	
	return true;
}

bool CUser::EudemonBronGet()
{
	int amount = m_pUserPet->GetEudmonAmount();
	if(amount <= 0)
		return false;

	bool bOk = false;
	int nAmount = 0;
	OBJID idEudemon[4];
	int nBabyIndex1 = -1;
	int nBabyIndex2 = -1;
	CUserPetData* pData = NULL;
	bool bFirstUser = false; 
	for(int i = 0 ; i < amount ; i ++)
	{
		pData = m_pUserPet->QueryDataByIndex(i);
		if(pData->GetInt(EUDEMONDATA_state) == PETSTATE_BORN)
		{
			if(!bOk)
			{
				if(pData->GetInt(EUDEMONDATA_level) != 1)
				{
					idEudemon[nAmount++] = pData->GetInt(EUDEMONDATA_id);
					continue;
				}

				CTime timeNow = CTime::GetCurrentTime();
				DWORD dataLast = pData->GetInt(EUDEMONDATA_lastborn);
				CTimeSpan span;

				//没判断过年  悲剧
				if(dataLast/1000000 == 12 && timeNow.GetMonth() == 1)
				{
					CTime timeLastPassYear(timeNow.GetYear() -1,dataLast/1000000,(dataLast % 1000000) /10000,(dataLast % 10000)/100,dataLast % 100,0,0);
					span = timeNow - timeLastPassYear;
				}
				else
				{
					CTime timeLast(timeNow.GetYear(),dataLast/1000000,(dataLast % 1000000) /10000,(dataLast % 10000)/100,dataLast % 100,0,0);
					span = timeNow - timeLast;
				}
				
				if(span.GetTotalHours() < 2/*span.GetTotalSeconds() < 10*/)
				{
					SendSysMsg("你的宠物还没有繁殖好");
					return false;
				}
				else if(span.GetTotalHours() < 4)
				{
					if(bFirstUser)
						bOk = true;
					else
					{
						idEudemon[nAmount++] = pData->GetInt(EUDEMONDATA_id);
						bFirstUser = true;
					}
				}
				else
				{
					bOk = true;
				}
 			}

			if(bOk)
			{
				if(nBabyIndex1 == -1)
					nBabyIndex1 = nAmount;
				else
					nBabyIndex2 = nAmount;

				idEudemon[nAmount++] = pData->GetInt(EUDEMONDATA_id);
			}
		}
	}
	
	if(bOk)
	{
		if(m_pUserPet->GetShowAmount() + nAmount > 3 + GetPacklev(4))
		{
			SendSysMsg("宠物背包已满");
			return false;
		}

		for(int i = 0 ; i < nAmount ; i ++ )
		{
			EudemonSetBorn(idEudemon[i],false,0);
		}
		
		DEBUG_TRY
		if(idEudemon[0] != 0)
		{
			EudemonData dataCheck;
			LoadEudemon(idEudemon[0],dataCheck, 2);
			UpdateMissionMsgByItemType(dataCheck.typeId);
		}
		DEBUG_CATCH("pUser->UpdateMissionMsgByItemType eudemon")
//		char szTemp[256] = "";
		char szMsg[256] = "";
		char UserName[128] = "";
		char EudName[128] = "";
		char EudName0[128] = "";
		char NPC[128] = "";
		char ServerName[128];
		if (nAmount < 4)
		{
			if(nBabyIndex1 != -1 )
			{
				EudemonData data0;
				LoadEudemon(idEudemon[nBabyIndex1],data0, 2);
				sprintf(UserName, "%d_[%s]_%d", SNAPSHOTUSER, this->GetName(), COLORYELLOW);
				sprintf(EudName, "%d_[%s]_%d_%d_%d_%d_%d", 
				SNAPSHOTUSEREDU, data0.name, idEudemon[nBabyIndex1], data0.typeId, this->GetID(), 0, COLORPURPLE);
				sprintf(NPC, "%d_钟灵_%d", SNAPSHOTNOTHING, COLORPINK);
				sprintf(szMsg, "<%s>笑嘻嘻的说：“恭喜<%s>的宠物已经平安生产，看<%s>多可爱！”", NPC, UserName, EudName);
				//sprintf(szTemp, "%d_%d_%s_%d_%d_%d", 0, idEudemon[nBabyIndex1], data0.name, this->GetID(), 0, 3);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, NULL , ALLUSERS_NAME);
			}
		}
		else
		{
			if(nBabyIndex1 != -1 && nBabyIndex2 != -1)
			{

				EudemonData data1;
				EudemonData data2;
				strcpy(ServerName, MapGroup(PID)->GetServerName());
				LoadEudemon(idEudemon[nBabyIndex1],data1, 2);
				LoadEudemon(idEudemon[nBabyIndex2],data2, 2);
				sprintf(UserName, "%d_[%s]_%d", SNAPSHOTUSER, this->GetName(), COLORYELLOW);
				sprintf(EudName, "%d_[%s]_%d_%d_%d_%d_%d", 
					SNAPSHOTUSEREDU, data1.name, idEudemon[nBabyIndex1], data1.typeId, this->GetID(), 0, COLORPURPLE);
				sprintf(EudName0, "%d_[%s]_%d_%d_%d_%d_%d", 
					SNAPSHOTUSEREDU, data2.name, idEudemon[nBabyIndex2], data2.typeId, this->GetID(), 0, COLORPURPLE);
				sprintf(szMsg, "<%s>的宝宝通过繁殖，获得了<%s>和<%s>，真可爱！", UserName, EudName, EudName0);
//				sprintf(szTemp, "%d_%d_%s_%d_%d_%d&%d_%d_%s_%d_%d_%d", 0, idEudemon[nBabyIndex1], data1.name, this->GetID(), 0, 3, 0, idEudemon[nBabyIndex2], data2.name, this->GetID(), 0, 3);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, NULL , ALLUSERS_NAME);
			}
		}
	}
	else
	{
		SendSysMsg("你的宠物还没有繁殖好");
	}
	return bOk;
}

bool CUser::EudemonPlayGet()
{
	int nEudemonAmount = m_pUserPet->GetEudmonAmount();
	if(nEudemonAmount <= 0)
		return false;

	OBJID idGetEudemon = ID_NONE;
	CUserPetData* pData = NULL;
 
	CTime timeNow = CTime::GetCurrentTime();
	CTimeSpan span;

	for(int i = 0 ; i < nEudemonAmount ; i ++)
	{
		pData = m_pUserPet->QueryDataByIndex(i);
		if(!pData)
			continue;

		if(pData->GetInt(EUDEMONDATA_state) == PETSTATE_PLAY)
		{
			DWORD dataLast = pData->GetInt(EUDEMONDATA_lastplay);
			CHECKF(dataLast);
			//没判断过年  悲剧
			if(dataLast/1000000 == 12 && timeNow.GetMonth() == 1)
			{
				CTime timeLastPassYear(timeNow.GetYear() -1,dataLast/1000000,(dataLast % 1000000) /10000,(dataLast % 10000)/100,dataLast % 100,0,0);
				span = timeNow - timeLastPassYear;
			}
			else
			{
				CTime timeLast(timeNow.GetYear(),dataLast/1000000,(dataLast % 1000000) /10000,(dataLast % 10000)/100,dataLast % 100,0,0);
				span = timeNow - timeLast;
			}
				
			if(span.GetTotalHours() < 2)
			{
				SendSysMsg("你的宠物还在玩耍中");
				return false;
			}
			else
			{
				idGetEudemon = pData->GetID();
				break;
			}
		}
	}
	
	if(idGetEudemon != ID_NONE)
	{
		if(m_pUserPet->GetShowAmount() + 1 > 3 + GetPacklev(4))
		{
			SendSysMsg("宠物栏已满");
			return false;
		}

		CUserPetData* pData = m_pUserPet->QueryData(idGetEudemon);
		if(!pData)
			return false;
		
		LuaScriptMachine()->InitVar(pData->GetInt(EUDEMONDATA_level));
		if(!ProcessAction(2010780))
			return false;

		pData->SetInt(EUDEMONDATA_state,PETSTATE_REST,true);
		
		EudemonData data;
		LoadEudemon(idGetEudemon,data,2);
		
		char szAwardMsg[128]  = "";
		sprintf(szAwardMsg,"你获得%s宠物",data.name);
		SendSysMsg(szAwardMsg);
		
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetInfo,&data,GetID()))
			SendMsg(&msg);
		
		DEBUG_TRY
		UpdateMissionMsgByItemType(data.typeId);
		DEBUG_CATCH("pUser->UpdateMissionMsgByItemType eudemon")
	}
	else
	{
		SendSysMsg("你没有玩耍中的宠物");
		return false;
	}
	return true;
}

bool CUser::EudemonUnderUp(OBJID idEudemon)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
		return false;

	if(pData->isLocked())
		return false;
	
	if(pData->IsSoulState())
		return false;

	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(pData->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType)
		return false;

	OBJID idNeedItem = ID_NONE;

	DWORD nNeedMoney = 0;
	int nData = pData->GetInt(EUDEMONDATA_understand);

	int failto = -1;
	bool okClear = false;
	int change = 0;
	CItem* pProtect = NULL;
	switch(nData)
	{
	case 0:
		change = 10000;
		idNeedItem = 630011;
		nNeedMoney = 100;
		break;
	case 1:
		change = 8500;
		idNeedItem = 630011;
		nNeedMoney = 100;
		break;
	case 2:
		change = 7000;
		idNeedItem = 630011;
		nNeedMoney = 100;
		break;
	case 3:
		change = 5500;
		idNeedItem = 630012;
		nNeedMoney = 200;
		okClear = true;
		break;
	case 4:
		change = 4000;
		idNeedItem = 630012;
		nNeedMoney = 200;
		break;
	case 5:
		change = 2500;
		idNeedItem = 630012;
		nNeedMoney = 200;
		failto = 4;
		okClear = true;
		break;
	case 6:
		change = 1500;
		idNeedItem = 630013;
		nNeedMoney = 300;
		failto = 4;
		okClear = true;
		break;
	case 7:
		change = 1000;
		idNeedItem = 630013;
		nNeedMoney = 300;
		break;
	case 8:
		change = 500;
		idNeedItem = 630013;
		nNeedMoney = 300;
		failto = 7;
		okClear = true;
		break;
	case 9:
		change = 200;
		idNeedItem = 630013;
		nNeedMoney = 300;
		//有悟性保护丹的不降
		pProtect = GetItemByType(612002);
		if(!pProtect)
			failto = 7;
		break;
	}
	
	int nFailTime = pData->GetInt(EUDEMONDATA_failtime);
	change += nFailTime * 200;
	CItem* pItem = GetItemByType(idNeedItem);
	if(!pItem)
	{
		ItemInfoStruct info;
		CHECKF(ItemType()->GetInfo(idNeedItem, &info));
		SendSysMsg("%s不足，请补充",info.szName);
		return false;
	}

	if(GetMoney() < nNeedMoney)
		return false;

	if(!SpendMoney(nNeedMoney,true))
		return false;

	if(!SpendItem(pItem))
		return false;

	BOOL bValueItem = FALSE;
	if(pProtect)
	{
		bValueItem = pProtect->IsCostlyItem();
		if(!SpendItem(pProtect))
			return false;
	}
	if(RandGet(10000) < change)
	{
		SendSysMsg("提升成功");
		nData ++;
		pData->SetInt(EUDEMONDATA_understand,nData,true);

// 		if(okClear)
// 			pData->SetInt(EUDEMONDATA_failtime,0,true);

		EudemonData Data;
		LoadEudemon(idEudemon,Data,2);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetInfo,&Data,GetID()))
			SendMsg(&msg);

		if(Data.understand > 3)
		{
			char szMsg[256] = "";
			char szTemp[256] = "";
			char UserName[32] = "";
			char EudName[64] = "";
			sprintf(UserName, "%d_[%s]_%d", SNAPSHOTUSER, this->GetName(), COLORYELLOW);
			sprintf(EudName, "%d_[%s]_%d_%d_%d_%d_%d", 
					SNAPSHOTUSEREDU, Data.name, idEudemon, Data.typeId, this->GetID(), 0, COLORPURPLE);
			sprintf(szMsg, "<%s>潜心钻研，终于将<%s>的悟性提升到<3_%d_6>，宠物资质大幅提升！", UserName, EudName, Data.understand);
			sprintf(szTemp, "%d_%d_%s_%d_%d_%d", 0, idEudemon, Data.name, this->GetID(), 0, 3);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, szTemp , ALLUSERS_NAME);
		}

		
	}
	else
	{
		SendSysMsg("提升失败");
		if(failto != -1)
		{
			nData = failto;
		}

		//nFailTime++;

		//pData->SetInt(EUDEMONDATA_failtime,nFailTime);
		pData->SetInt(EUDEMONDATA_understand,nData,true);
		
		EudemonData Data;
		LoadEudemon(idEudemon,Data,2);
		CMsgEudemon msg;
		if(msg.Create(PETACTION_PetInfo,&Data,GetID()))
			SendMsg(&msg);
	}
	if ((ItemType()->GetMonoly(idNeedItem)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), idNeedItem);
	if(bValueItem)
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), 612002);

	return true;
}

bool CUser::EudemonBaby(OBJID idEudemon)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
		return false;

	if(pData->isLocked())
		return false;

	if(pData->IsSoulState())
		return false;

	if(pData->GetInt(EUDEMONDATA_sex) > 1)
		return false;

	if(pData->GetInt(EUDEMONDATA_borntime) != 0)
		return false;

	CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(pData->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType)
		return false;

	OBJID idNeedItem = ID_NONE;
	int nNeedMoney = 0;
	int nNeedLev = pEudemonType->GetInt(EUDEMONTYPEDATA_needLev);
	if(nNeedLev > 0 && nNeedLev <= 40)
	{
		idNeedItem = 630008;
		nNeedMoney = 100;
	}
	else if(nNeedLev > 40 && nNeedLev <= 70)
	{
		idNeedItem = 630009;
		nNeedMoney = 300;
	}
	else if(nNeedLev > 70)
	{
		idNeedItem = 630010;
		nNeedMoney = 400;
	}

	
	CItem* pItem = GetItemByType(idNeedItem);
	if(!pItem)
	{
		ItemInfoStruct info;
		CHECKF(ItemType()->GetInfo(idNeedItem, &info));
		SendSysMsg("%s不足，请补充",info.szName);
		return false;
	}


	if(!SpendMoney(nNeedMoney,true))
		return false;

	if(!SpendItem(pItem))
		return false;

	pData->SetInt(EUDEMONDATA_level, 1);
	pData->SetInt(EUDEMONDATA_exp, 0);
	pData->SetInt(EUDEMONDATA_understand, 0);
	pData->SetInt(EUDEMONDATA_gift, 0);
	pData->SetInt(EUDEMONDATA_borntime, 0);
	pData->SetInt(EUDEMONDATA_addpoint, 0);
	pData->SetInt(EUDEMONDATA_bone, RandGet(8000)%7);
	pData->SetInt(EUDEMONDATA_gettype, 1);
	pData->SetInt(EUDEMONDATA_happy, 100);
	pData->SetInt(EUDEMONDATA_stradd, 0);
	pData->SetInt(EUDEMONDATA_intadd, 0);
	pData->SetInt(EUDEMONDATA_staadd, 0);
	pData->SetInt(EUDEMONDATA_spiadd, 0);
	pData->SetInt(EUDEMONDATA_agiadd, 0);
    pData->SetInt(EUDEMONDATA_lifespan, pEudemonType->GetInt(EUDEMONTYPEDATA_maxlifespan),true);

	int nRand = RandGet(10000);
	int nAddData = 0;
	if(nRand < 200)
		nAddData = 80 + RandGet(20) + 1;
	else if(nRand < 800)
		nAddData = 60 + RandGet(20) + 1;
	else if(nRand < 4200)
		nAddData = 40 + RandGet(20) + 1;
	else
		nAddData = 20 + RandGet(20) + 1;

	char szTempName[64];
	sprintf(szTempName,"%s宝宝",pEudemonType->GetStr(EUDEMONTYPEDATA_name));
	pData->SetStr(EUDEMONDATA_name,szTempName,64);
    pData->SetInt(EUDEMONDATA_failtime,0);
	pData->SetInt(EUDEMONDATA_grow, nAddData,true);	

	int nTotal = 0;
	int nGet = RandGet(100);
	pData->SetInt(EUDEMONDATA_habit, 0);
	bool bSet = false;
	if(nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit1))
	{
		pData->SetInt(EUDEMONDATA_habit, 0);
		bSet = true;
	}
	nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit1);
				
	if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit2) + nTotal))
	{
		pData->SetInt(EUDEMONDATA_habit, 1);
		bSet = true;
	}
	nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit2);
	if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit3) + nTotal))
	{
		pData->SetInt(EUDEMONDATA_habit, 2);
		bSet = true;
	}
	nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit3);
	if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit4) + nTotal))
	{
		pData->SetInt(EUDEMONDATA_habit, 3);
		bSet = true;
	}
	nTotal+= pEudemonType->GetInt(EUDEMONTYPEDATA_habit4);
	if(!bSet && (nGet <  pEudemonType->GetInt(EUDEMONTYPEDATA_habit5) + nTotal))
	{
		pData->SetInt(EUDEMONDATA_habit, 4);
		bSet = true;
	}

	DWORD thisGrow = pEudemonType->GetInt(EUDEMONTYPEDATA_initgrow) + pData->GetInt(EUDEMONDATA_grow);
	DWORD thisstr = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initstr) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstr))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
	DWORD thisint = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initint) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randint))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
	DWORD thissta = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initsta) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randsta))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
	DWORD thisspi = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initspi) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randspi))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;
	DWORD thisagi = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initagi) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randagi))/*) * (thisGrow / 100.0f * 0.1f + 1)*/;

	pData->SetInt(EUDEMONDATA_firstr, thisstr);
	pData->SetInt(EUDEMONDATA_firint, thisint);
	pData->SetInt(EUDEMONDATA_firsta, thissta);
	pData->SetInt(EUDEMONDATA_firspi, thisspi);
	pData->SetInt(EUDEMONDATA_firagi, thisagi);

	float gettypeadd = 1.0f;
	DWORD thisstrchg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initstrChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstrChg))/*) * gettypeadd*/;
	DWORD thisintchg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initintChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randintChg))/*) * gettypeadd*/;
	DWORD thisstachg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initstaChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randstaChg))/*) * gettypeadd*/;
	DWORD thisspichg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initspiChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randspiChg))/*) * gettypeadd*/;
	DWORD thisagichg = /*(*/pEudemonType->GetInt(EUDEMONTYPEDATA_initagiChg) + RandGet(pEudemonType->GetInt(EUDEMONTYPEDATA_randagiChg))/*) * gettypeadd*/;

	pData->SetInt(EUDEMONDATA_strchg, thisstrchg);
	pData->SetInt(EUDEMONDATA_intchg, thisintchg);
	pData->SetInt(EUDEMONDATA_stachg, thisstachg);
	pData->SetInt(EUDEMONDATA_spichg, thisspichg);
	pData->SetInt(EUDEMONDATA_agichg, thisagichg,true);
					
	if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand1))
		pData->SetInt(EUDEMONDATA_skill_1, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_1) * 1000 + 1);
	else
		pData->SetInt(EUDEMONDATA_skill_1, 0);
	if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand2))
		pData->SetInt(EUDEMONDATA_skill_2, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_2)* 1000 + 1);
	else
		pData->SetInt(EUDEMONDATA_skill_2, 0);
	if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand3))
		pData->SetInt(EUDEMONDATA_skill_3, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_3)* 1000 + 1);
	else
		pData->SetInt(EUDEMONDATA_skill_3, 0);
	if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand4))
		pData->SetInt(EUDEMONDATA_skill_4, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_4)* 1000 + 1);
	else
		pData->SetInt(EUDEMONDATA_skill_4, 0);
	if(RandGet(10000) < pEudemonType->GetInt(EUDEMONTYPEDATA_skill_rand5))
		pData->SetInt(EUDEMONDATA_skill_5, pEudemonType->GetInt(EUDEMONTYPEDATA_skill_5)* 1000 + 1);
	else
		pData->SetInt(EUDEMONDATA_skill_5, 0);
	pData->SetInt(EUDEMONDATA_skill_6, 0);
	pData->SetInt(EUDEMONDATA_skill_7, 0);
	pData->SetInt(EUDEMONDATA_skill_8, 0);
	pData->SetInt(EUDEMONDATA_skill_9, 0);
	pData->SetInt(EUDEMONDATA_skill_10, 0);
	pData->SetInt(EUDEMONDATA_playtime, 0, true);

	EudemonData Data;
	LoadEudemon(idEudemon,Data,2);
	CMsgEudemon msg;
	if(msg.Create(PETACTION_PetInfo,&Data,GetID()))
	{	
		SendMsg(&msg);
		char szMsg[256] = "";
		char UserName[128] = "";
		char EudName[128] = "";
		char AddKnow[128] = "";
		char ServerName[128];
		strcpy(ServerName, MapGroup(PID)->GetServerName());
		sprintf(UserName, "%d_[%s]_%d_%s_%d_%d", SNAPSHOTUSER, this->GetName(), this->GetSex(), ServerName , this->GetCurJob(), COLORYELLOW);
		sprintf(EudName, "%d_[%s]_%d_%d_%d_%d_%d", 
				SNAPSHOTUSEREDU, Data.name, idEudemon, Data.typeId, this->GetID(), 0, COLORPURPLE);
// 		EudemonData Dataedu;
// 		LoadEudemon(idEudemon,Dataedu, 1);
		if (nAddData > 80)
		{
			sprintf(AddKnow, "%d_完美_%d", SNAPSHOTNOTHING, COLORPINK);
			sprintf(szMsg, "<%s>将<%s>成功还童成<%s>成长，期待这只宠物可以繁殖出一代神兽！", UserName, EudName, AddKnow);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, NULL , ALLUSERS_NAME);
		}
		else if (nAddData > 60)
		{
			sprintf(AddKnow, "%d_卓越_%d", SNAPSHOTNOTHING, COLORPINK);
			sprintf(szMsg, "<%s>将<%s>成功还童成<%s>成长，期待这只宠物可以繁殖出一代神兽！", UserName, EudName, AddKnow);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, NULL , ALLUSERS_NAME);
		}
		else if (nAddData > 40)
		{
			sprintf(AddKnow, "%d_杰出_%d", SNAPSHOTNOTHING, COLORPINK);
			sprintf(szMsg, "<%s>将<%s>成功还童成<%s>成长，期待这只宠物可以繁殖出一代神兽！", UserName, EudName, AddKnow);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, 0, 0, NULL , ALLUSERS_NAME);
		}

	}
	if ((ItemType()->GetMonoly(idNeedItem)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), idNeedItem);

	return true;
}

bool CUser::EudemonBron(int nType,OBJID idEudemon1,OBJID idEudemon2,CUser* pOther/* = NULL*/)
{
	if(!CanEudemonBorn())
	{
		SendSysMsg("你有宠物正在繁殖中，请在领取后再来繁殖");
		return false;
	}
	DWORD nMoney = 25000;
	CItem* pItem = NULL;
	if(nType == 1)
	{
		pOther = this;
		pItem = GetItemByType(630001);
		if(!pItem)
		{
			ItemInfoStruct info;
			CHECKF(ItemType()->GetInfo(630001, &info));
			SendSysMsg("%s不足，请补充",info.szName);
			return false;
		}

		if(GetMoney() < nMoney)
		{
			SendSysMsg("你没有足够的银两");
			return false;
		}
	}
	if(nType == 2)
	{
		nMoney = 20000;

		if(GetMoney() < nMoney)
		{
			SendSysMsg("你没有足够的银两");
			pOther->SendSysMsg("对方没有足够的银两");
			return false;
		}

		if(pOther->GetMoney() < nMoney)
		{
			pOther->SendSysMsg("你没有足够的银两");
			SendSysMsg("对方没有足够的银两");
			return false;
		}
	}

	EudemonData data1;
	if(!LoadEudemon(idEudemon1,data1,1))
		return false;
	EudemonData data2;
	if(!pOther->LoadEudemon(idEudemon2,data2,1))
		return false;

	if(idEudemon1 == m_idSoulEudemon)
		return false;
	if(idEudemon2 == pOther->GetSoulEudemon())
		return false;

	if(data1.sex > 1 || data2.sex > 1)
	{
		SendSysMsg("幻兽不能繁殖");
		pOther->SendSysMsg("幻兽不能繁殖");
		return false;
	}

	if(data1.level < 30 * (data1.borntime + 1))
		return false;
	if(data2.level < 30* (data2.borntime + 1))
		return false;

	if(data1.gettype != 1)
		return false;
	if(data2.gettype != 1)
		return false;

	if(data1.sex == data2.sex)
		return false;

	if(data1.typeId != data2.typeId)
		return false;

	if(data1.borntime >= 3)
		return false;

	if(data2.borntime >= 3)
		return false;
	
	if(!SpendMoney(nMoney,true))
		return false;

	if(nType == 2)
	{
		if(!pOther->SpendMoney(nMoney,true))
			return false;
	}
	else if(nType == 1)
	{
		if(pItem)
		{
			if(!SpendItem(pItem))
				return false;
			if ((ItemType()->GetMonoly(630001)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), 630001);
		}
	}
			

	int nAmount = 0;
	int nBorn1 = 0;
	int nBorn2 = 0;
	int nBorn3 = 0;
	int nBorn4 = 0;
	int nBorn5 = 0;
	int nBorn6 = 0;
	int nBorn7 = 0;
	int nBorn8 = 0;
	int nGetType1 = 0;
	int nGetType2 = 0;

	if(data1.dwNeedlev > 0 && data1.dwNeedlev <= 20)
	{
		nBorn1 = 20;
		nBorn2 = 20;
		nBorn3 = 15;
		nBorn4 = 15;
		nAmount= nBorn1+nBorn2+nBorn3+nBorn4;

		int nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType1 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType1 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType1 = 4;
		else
			nGetType1 = 5;

		nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType2 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType2 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType2 = 4;
		else
			nGetType2 = 5;
	}
	else if(data1.dwNeedlev > 20 && data1.dwNeedlev <= 40)
	{
		nBorn1 = 20;
		nBorn2 = 20;
		nBorn3 = 15;
		nBorn4 = 15;
		nBorn5 = 10;
		nAmount= nBorn1+nBorn2+nBorn3+nBorn4+nBorn5;
		
		int nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType1 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType1 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType1 = 4;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4)
			nGetType1 = 5;
		else
			nGetType1 = 6;
		
		nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType2 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType2 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType2 = 4;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4)
			nGetType2 = 5;
		else
			nGetType2 = 6;
	}
	else if(data1.dwNeedlev > 40 && data1.dwNeedlev <= 60)
	{
		nBorn1 = 20;
		nBorn2 = 20;
		nBorn3 = 15;
		nBorn4 = 15;
		nBorn5 = 10;
		nBorn6 = 10;
		nAmount= nBorn1+nBorn2+nBorn3+nBorn4+nBorn5 + nBorn6;
		
		int nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType1 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType1 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType1 = 4;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4)
			nGetType1 = 5;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5)
			nGetType1 = 6;
		else
			nGetType1 = 7;
		
		nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType2 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType2 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType2 = 4;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4)
			nGetType2 = 5;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5)
			nGetType2 = 6;
		else
			nGetType2 = 7;
	}
	else if(data1.dwNeedlev > 60 && data1.dwNeedlev <= 80)
	{
		nBorn1 = 20;
		nBorn2 = 20;
		nBorn3 = 15;
		nBorn4 = 15;
		nBorn5 = 10;
		nBorn6 = 10;
		nBorn7 = 5;
		nAmount= nBorn1+nBorn2+nBorn3+nBorn4+nBorn5+nBorn6+nBorn7;
		
		int nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType1 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType1 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType1 = 4;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4)
			nGetType1 = 5;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5)
			nGetType1 = 6;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5+ nBorn6)
			nGetType1 = 7;
		else
			nGetType1 = 8;
		
		nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType2 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType2 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType2 = 4;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4)
			nGetType2 = 5;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5)
			nGetType2 = 6;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5+ nBorn6)
			nGetType2 = 7;
		else
			nGetType2 = 8;
	}
	else
	{
		nBorn1 = 20;
		nBorn2 = 20;
		nBorn3 = 15;
		nBorn4 = 15;
		nBorn5 = 10;
		nBorn6 = 10;
		nBorn7 = 5;
		nBorn8 = 5;
		nAmount= nBorn1+nBorn2+nBorn3+nBorn4+nBorn5+nBorn6+nBorn7+nBorn8;
		
		int nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType1 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType1 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType1 = 4;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4)
			nGetType1 = 5;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5)
			nGetType1 = 6;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5+ nBorn6)
			nGetType1 = 7;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5+ nBorn6 + nBorn7)
			nGetType1 = 8;
		else
			nGetType1 = 9;
		
		nRand = RandGet(nAmount);
		if(nRand < nBorn1)
			nGetType2 = 2;
		else if(nRand < nBorn1 + nBorn2)
			nGetType2 = 3;
		else if(nRand < nBorn1 + nBorn2 + nBorn3)
			nGetType2 = 4;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4)
			nGetType2 = 5;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5)
			nGetType2 = 6;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5+ nBorn6)
			nGetType2 = 7;
		else if(nRand < nBorn1 + nBorn2 + nBorn3 + nBorn4 + nBorn5+ nBorn6 + nBorn7)
			nGetType2 = 8;
		else
			nGetType2 = 9;
	}
	EudemonData newPetData1;
	newPetData1.gettype = nGetType1;
 	AwardEudemon(data1.typeId,newPetData1,false,true,(data1.grow + data2.grow) / 2);

	EudemonData newPetData2;
	newPetData2.gettype = nGetType2;
 	pOther->AwardEudemon(data1.typeId,newPetData2,false,true,(data1.grow + data2.grow) / 2);

 	EudemonSetBorn(idEudemon1,true,data1.borntime + 1);
	EudemonSetBorn(newPetData1.id,true,0);

	pOther->EudemonSetBorn(idEudemon2,true,data2.borntime + 1);
	pOther->EudemonSetBorn(newPetData2.id,true,0);

	DEBUG_TRY
	UpdateMissionMsgByItemType(newPetData1.typeId);
	DEBUG_CATCH("pUser->UpdateMissionMsgByItemType eudemon")

	return true;
}

void CUser::SendPosToTeamMem(OBJID idMap, int nPosX, int nPosY)//goto 通知队友自己所在的地图
{
	if (GetTeam() && GetTeam()->GetMemberAmount()>1)
	{
		CMsgTeamPos msg;
		if (!msg.Create(_TEAMPOS_SENDSELFPOS, GetID()))
			return;
		msg.Append(GetID(),idMap,nPosX, nPosY,GetName());
		for(int i = 0; i < GetTeam()->GetMemberAmount(); i++)
		{
			OBJID idMember = GetTeam()->GetMemberByIndex(i);
			if (idMember==GetID())
				continue;
			CUser* pMember = UserManager()->GetUser(idMember);
			if(pMember)
			{
				pMember->SendMsg(&msg);
			}
			else
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idMember);
		}
	}
}

void CUser::GetPosFromTeamMem()//goto 获取队友的位置
{
	if (GetTeam() && GetTeam()->GetMemberAmount()>1)
	{
		CMsgTeamPos msg;
		if (!msg.Create(_TEAMPOS_SENDSELFPOS, GetID()))
			return;
		for(int i = 0; i < GetTeam()->GetMemberAmount(); i++)
		{
			OBJID idMember = GetTeam()->GetMemberByIndex(i);
			if (idMember==GetID())
				continue;
			CUser* pMember = UserManager()->GetUser(idMember);
			if(pMember)
			{
				msg.Append(pMember);
				SendMsg(&msg);
			}
			else
			{
				//到其他地图组去取队友位置
				CMsgTeamPos msg;
				if (!msg.Create(_TEAMPOS_SENDSELFPOS, GetID()))
					return;
				msg.SetTransData(idMember);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg);
			}
		}
	}
}
