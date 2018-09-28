// UserManager.cpp: implementation of the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MessagePort.h"
#pragma warning(disable:4786)
#include "AllMsg.h"
#include "MapGroup.h"
#pragma warning(disable:4786)
#include <algorithm>
#include "netmsg.h"
#include "protocol.h"
#include "PoliceWanted.h"
#include "Agent.h"
#include "EconomySystem.h"	
#include "..\stdafx.h"
#include "ResourceWar.h"
#include "CombGod.h"
#include "StockOrder.h"
#include "MapInstanceMgr.h"
#include "TutorList.h"
#include "VipList.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define STOCK_TIMEOUT	10

MYHEAP_IMPLEMENTATION(CUserManager,s_heap)

extern int g_SpecilLine;

//////////////////////////////////////////////////////////////////////
bool CUserManager::Create()
{
	m_pTeamSet		= CTeamSet::CreateNew(true);
	m_idLastTeam	= m_idProcess;					//? 保证每个进程组的TEAM_ID不冲突。
	m_StockTimeOut.SetInterval(STOCK_TIMEOUT*m_idProcess);
	m_StockTimeOut.Update();
	return true;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::Release()
{
	LogoutAllUser();
 	if(m_pTeamSet)
 		m_pTeamSet->Release();
	delete this;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::OnTimer(time_t tCurr)
{
	//在这里进行金币交易，这样可以避免玩家指针失效问题
// 	if(m_StockTimeOut.IsTimeOut())
// 	{
// 		m_StockTimeOut.Update();
// 		CStockTrade::Instance()->OnUpdate(m_idProcess);
// 		
// 	}

}

//////////////////////////////////////////////////////////////////////
CUser* CUserManager::CreateUser(SOCKET_ID idSocket, OBJID idUser, ULONG nIP)		// login第一步
{
	if(idSocket == SOCKET_NONE || idUser == ID_NONE)
	{
		ASSERT(!"CUserManager::LoginUser");
		return NULL;
	}

	if(idSocket < m_setUser.size() && m_setUser[idSocket])
	{
		ASSERT(!"CUserManager::LoginUser 重复SOCKET_ID");
		DEBUG_TRY
		LogoutUser(idSocket);				//@ 同一SOCKET重复登录，前一个强行退出。
		DEBUG_CATCH("LogoutUser(idSocket)")	//@
		m_setUser[idSocket]	= NULL;
	}

	CUserPtr pOldUser = GetUser(idUser);
	if(pOldUser)
	{
		SOCKET_ID idOldSocket = pOldUser->GetSocketID();			// 前一玩家的SOCKET_ID

		DEBUG_TRY
		LOGMSG("DEBUG：踢出重复登录的USER_ID[%u], SOCKET_ID[%d]", idUser, idOldSocket);
		KickOutSocket(idOldSocket, "重复登录");				//@ 同一ID重复登录，前一个强行退出。
		DEBUG_CATCH("LogoutUser(id)")	//@
		return NULL;		// 不能继续
	}

	CUser* pUser = CUser::CreateNew();
	if(!pUser)
		return NULL;

	if(!pUser->Create(m_idProcess, idSocket, idUser, nIP))
	{
		pUser->ReleaseByOwner();
		LOGWARNING("创建玩家失败");
		return NULL;
	}
	
// 	if (DWORD dwID = pUser->GetSynID())//进行复制不是等于
// 	{
// 		pUser->QuerySynAttr()->UserLogin(idUser, dwID);
// 	}



	return pUser;
}

void CUserManager::UpdateTempUser(CUser* pUser)
{
	CHECK(pUser);
	TEMPUSER_SET::iterator it = m_setTempUser.find(pUser->GetID());
	if(it != m_setTempUser.end())
	{
		CreateTempUser(pUser,it->second);
	}
}

stTempUser* CUserManager::QueryTempUser(OBJID idUser,bool bFlush)
{
	CHECKF(idUser);
	TEMPUSER_SET::iterator it = m_setTempUser.find(idUser);
	if(it != m_setTempUser.end())
	{
		if(bFlush) //强制
			CHECKF(CreateTempUser(idUser,it->second));
// 		else if(::TimeGet(TIME_SECOND) - it->second->timeCheck > 20 * 60 )
// 			CHECKF(CreateTempUser(idUser,it->second));

		return it->second;
	}
	else
	{
		stTempUser* pUser = new stTempUser();
		if(!CreateTempUser(idUser,pUser))
		{
			LOGWARNING("CreateTempUser error user %u",idUser);
			delete pUser;
			return NULL;
		}

		if(m_setTempUser[idUser])
			delete m_setTempUser[idUser];

		m_setTempUser[idUser] = pUser;
		return pUser;
	}

	return NULL;
}

stTempUser* CUserManager::QueryTempUser(char* szName,bool bFlush)
{
	CHECKF(szName);
	OBJID idUser=m_setUserName[szName];
	if(idUser)
		return QueryTempUser(idUser,bFlush);

	stTempUser* pUser = new stTempUser();
	CHECKF(CreateTempUser(szName,pUser));

	if(m_setTempUser[idUser])
		delete m_setTempUser[idUser];

	m_setTempUser[pUser->idUser] = pUser;
	return pUser;
}

bool CUserManager::CreateTempUser(OBJID idUser,stTempUser* pUser)
{
	int nStep = 0;
	DEBUG_TRY
	CHECKF(idUser);
	CHECKF(pUser);
	pUser->timeCheck = ::TimeGet(TIME_SECOND);

	pUser->idUser = idUser;

	nStep = 1;

	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT account_id,name,lookface,level,Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv,Syn_ID,use_title,Feel,vipface,sex,Tutor_Lv,max_score,vipstate,viplev,Exit_Time,TutorMorality FROM %s WHERE id=%u LIMIT 1", _TBL_USER, idUser);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
	if(pRes)
	{
		nStep = 2;
		pUser->idAcc = pRes->LoadDWord("account_id");
		pUser->lookface = pRes->LoadDWord("lookface");
		pUser->level = pRes->LoadDWord("level");
		pUser->Fir_Job = pRes->LoadDWord("Fir_Job");
		pUser->Fir_JobLv = pRes->LoadDWord("Fir_JobLv");
		pUser->Sec_Job = pRes->LoadDWord("Sec_Job");
		pUser->Sec_JobLv = pRes->LoadDWord("Sec_JobLv");
		pUser->use_title = pRes->LoadDWord("use_title");
		pUser->vipface = pRes->LoadDWord("vipface");
		pUser->sex = pRes->LoadDWord("sex");
		pUser->Tutor_Lv = pRes->LoadDWord("Tutor_Lv");
		int nVipState = pRes->LoadDWord("vipstate");
		if(nVipState)
			pUser->viplev = pRes->LoadDWord("viplev");
		else
			pUser->viplev = 0;
		pUser->Exit_Time = pRes->LoadDWord("Exit_Time");
		pUser->max_score = pRes->LoadDWord("max_score");
		pUser->TutorMorality = pRes->LoadDWord("TutorMorality");
		pUser->Syn_ID = pRes->LoadDWord("Syn_ID");
		pRes->LoadString(pUser->name,	"name", _MAX_NAMESIZE);
		pRes->LoadString(pUser->Feel,	"Feel", _MAX_NAMESIZE);
		pRes->Release();

		nStep = 3;
		DEBUG_TRY
		m_setUserName[pUser->name]=pUser->idUser;
		DEBUG_CATCH3("m_setUserName[%s]=%d",pUser->name,pUser->idUser);
		return true;
	}
	DEBUG_CATCH2("CreateTempUser(OBJID idUser,stTempUser* pUser) step = %u",nStep);
	return false;
}

bool CUserManager::CreateTempUser(char* szName,stTempUser* pUser)
{
	int nStep = 0;
	DEBUG_TRY
	CHECKF(szName);
	CHECKF(pUser);
	pUser->timeCheck = ::TimeGet(TIME_SECOND);
	
	strcpy(pUser->name,	szName);
	
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT account_id,name,lookface,level,Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv,Syn_ID,use_title,Feel,vipface,sex,Tutor_Lv,max_score,vipstate,viplev,Exit_Time,TutorMorality FROM %s WHERE name='%s' LIMIT 1", _TBL_USER, szName);
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
	if(pRes)
	{
		nStep = 2;
		pUser->idAcc = pRes->LoadDWord("account_id");
		pUser->idUser = pRes->LoadDWord("id");
		pUser->lookface = pRes->LoadDWord("lookface");
		pUser->level = pRes->LoadDWord("level");
		pUser->Fir_Job = pRes->LoadDWord("Fir_Job");
		pUser->Fir_JobLv = pRes->LoadDWord("Fir_JobLv");
		pUser->Sec_Job = pRes->LoadDWord("Sec_Job");
		pUser->Sec_JobLv = pRes->LoadDWord("Sec_JobLv");
		pUser->use_title = pRes->LoadDWord("use_title");
		pUser->vipface = pRes->LoadDWord("vipface");
		pUser->sex = pRes->LoadDWord("sex");
		pUser->Tutor_Lv = pRes->LoadDWord("Tutor_Lv");
		int nVipState = pRes->LoadDWord("vipstate");
		if(nVipState)
			pUser->viplev = pRes->LoadDWord("viplev");
		else
			pUser->viplev = 0;
		pUser->Exit_Time = pRes->LoadDWord("Exit_Time");
		pUser->max_score = pRes->LoadDWord("max_score");
		pUser->TutorMorality = pRes->LoadDWord("TutorMorality");
		pUser->Syn_ID = pRes->LoadDWord("Syn_ID");
		pRes->LoadString(pUser->Feel,	"Feel", _MAX_NAMESIZE);
		pRes->Release();
		
		nStep = 3;
		m_setUserName[pUser->name]=pUser->idUser;
		return true;
	}
	DEBUG_CATCH2("CreateTempUser(char* szName,stTempUser* pUser) step = %u",nStep);
	return false;
}

bool CUserManager::CreateTempUser(CUser* pUser,stTempUser* pTempUser)
{
	CHECKF(pUser);
	CHECKF(pTempUser);
	pTempUser->timeCheck = ::TimeGet(TIME_SECOND);
	
	pTempUser->idAcc = pUser->GetAccountID();
	pTempUser->idUser = pUser->GetID();
	pTempUser->lookface = pUser->GetLookFace();
	pTempUser->level = pUser->GetLev();
	pTempUser->Fir_Job = pUser->GetProfession(1);
	pTempUser->Fir_JobLv = pUser->GetJobLev(1);
	pTempUser->Sec_Job = pUser->GetProfession(2);
	pTempUser->Sec_JobLv = pUser->GetJobLev(2);
	pTempUser->use_title = pUser->GetUseTitle();
	pTempUser->vipface = pUser->GetVipface();
	pTempUser->sex = pUser->GetSex();
	pTempUser->Tutor_Lv = pUser->GetImpart();
	pTempUser->viplev = pUser->IsVipActive() ? pUser->GetVipLev(): 0;
	pTempUser->Exit_Time = pUser->GetExitTime();
	pTempUser->max_score = pUser->GetMaxScore();
	pTempUser->TutorMorality = pUser->GetMorality();
	pTempUser->Syn_ID = pUser->GetSynID();
	strcpy(pTempUser->name,	pUser->GetName());
	strcpy(pTempUser->Feel,	pUser->GetFeel());
	return true;
}

void CUserManager::ClearTempUser()
{
	for (TEMPUSER_SET::iterator it = m_setTempUser.begin(); it != m_setTempUser.end() ; it ++ )
	{
		delete it->second;
	}

	m_setTempUser.clear();
}

//////////////////////////////////////////////////////////////////////
bool CUserManager::LoginUser(SOCKET_ID idSocket, CUser* pUser)		// login第二步
{
	CHECKF(pUser);

	if(idSocket >= m_setUser.size())
		m_setUser.resize(idSocket + 1, NULL);		//控制有效

	m_setUser[idSocket]	= pUser;
	QuerySet()->AddObj(pUser);
	RoleManager()->QuerySet()->AddObj(pUser->QueryRole());
	m_setUserName[pUser->GetName()]=pUser->GetID();


	//发给客户端，现在还没进入地图
	CMsgUserInfo	msg;	
	if(msg.Create(pUser))
	{
		MapGroup(PID)->QueryIntraMsg()->SendClientMsg(idSocket, &msg);
	}

	// max user record
	DWORD dwUserAmount = this->GetUserAmount();
	if (dwUserAmount >= m_dwMaxUser)
	{
		m_dwMaxUser = dwUserAmount;
	}

	if (pUser->IsGM())
		m_setGm.push_back(pUser->GetID());

	//-----师父列表维护
	DEBUG_TRY
	if ( pUser->IsInTutorList() )//登记了师父列表的
	{
		TUTORLIST info = {0};
		info.idUser = pUser->GetID();
		info.ucGender = pUser->GetSex();
		if (pUser->IsVipActive())
			info.ucVipLev = pUser->GetVipLev();
		else
			info.ucVipLev = 0;
		info.usLev = pUser->GetLev();
		info.dwProfession = pUser->GetProfession(1);
		info.dwBattleNum = pUser->GetMaxScore();
		info.dwImpart = pUser->GetImpart();
		info.dwLastLogoutTime = pUser->GetIPAddress();
		info.uIPAddress = pUser->GetIPAddress();
		SafeCopy(info.szName, pUser->GetName(), _MAX_NAMESIZE);

		if (!g_TutorListSystem.TutorLogin(&info))//设置为在线状态，返回false表示已过期
			pUser->DelTutorListMask();
		else//同步到其他线
		{
			CMsgSchoolMember msg;
			if (msg.Create(pUser->GetID(), MESSAGEBOARD_SYNLINE, NULL, 0, SYNTUTOR_ONLINE))
			{
				msg.Append(info);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, (OBJID)0);
			}
		}
	}
	DEBUG_CATCH2("CUserManager::LoginUser TutorList error!userid=%u", pUser->GetID())
	//-----
	return true;
}

//////////////////////////////////////////////////////////////////////     change huang 2004.1.5
bool CUserManager::LogoutUser(SOCKET_ID idSocket, BOOL bOffline/*=true*/)   
{
	OBJID idUser=ID_NONE;
	DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

	if(idSocket >= m_setUser.size() || !m_setUser[idSocket])
		return false;

	OBJID idMap = 0;
	

	CUserPtr pUser = GetUserBySocketID(idSocket);
	if(pUser)
	{
		int nCatchDebugStep = 0;
		if(pUser->GetMap())
		{
			idMap = pUser->GetMap()->GetID();
		}

		DEBUG_TRY
		UpdateTempUser(pUser);
		DEBUG_CATCH("UpdateTempUser(pUser)")
		
		DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
		idUser=pUser->GetID();

		if(bOffline)
		{
			nCatchDebugStep = 1;
			pUser->UpdateLastLogout();
			nCatchDebugStep = 12;
			// save record point
			if(pUser->IsAlive())
			{
				if (!pUser->IsChangeLine())//不是切线
				{
					if (g_SpecilLine==0)//不是专线下--jay
					{
						nCatchDebugStep = 13;
						if(pUser->GetMap())
						{
							if(!pUser->GetMap()->IsRecordDisable())
 							{
								pUser->SetSpecilSteate(STATE_SPECILLINE_OUT);
								pUser->SetRecordPos(pUser->GetMapID(), pUser->GetPosX(), pUser->GetPosY());
							}
						}
						else
						{
							//所有玩家，凡在含有"type 4"的地图中下线，再次登陆游戏的座标点为[4001 (74，113)]
							pUser->SetRecordPos(1001,78,87);
						}
					}
					else//是专线下
					{
						if(pUser->GetMap() && pUser->GetMap()->IsSynMap())
							pUser->SetSpecilPos(pUser->GetSynID(), pUser->GetPosX(),pUser->GetPosY(),STATE_SPECILLINE_REQUEST);//让他下次在专线上
						else if(pUser->QueryStatus(STATUS_WARGAME))//竞技场的
						{
							WargameManager()->PlayerLogout(pUser->QueryStatus(STATUS_WARGAME)->GetPower(), pUser);
							pUser->SetSpecilSteate(STATE_SPECILLINE_REQUEST);
						}
						else if (pUser->GetMap() && pUser->GetMap()->GetType() == 3)
						{
							pUser->SetSpecilPos(pUser->GetMapID(), pUser->GetPosX(),pUser->GetPosY(),STATE_SPECILLINE_REQUEST);//让他下次在专线上
						}
						else
							pUser->SetSpecilSteate(STATE_SPECILLINE_OUT);//让他下次不在专线上
					}
				}
			}
			else
			{
				nCatchDebugStep = 14;
				if(pUser->QueryStatus(STATUS_WARGAME))//竞技场的
				{
					pUser->SetLife( pUser->GetMaxLife() / 2 );
					pUser->SetMana( pUser->GetMaxMana() / 2 );
					pUser->SetSp( 0 );
					WargameManager()->PlayerLogout(pUser->QueryStatus(STATUS_WARGAME)->GetPower(), pUser);
					pUser->SetSpecilSteate(STATE_SPECILLINE_REQUEST);//让他下次在专线上
				}
				else
				{
					pUser->SetLife( pUser->GetMaxLife() / 10 );
					pUser->SetMana( pUser->GetMaxMana() / 10);
					pUser->SetSp( 0 );
					pUser->SetRecordPos(1026, 34, 31);
					pUser->SetSpecilSteate(STATE_SPECILLINE_OUT);//死了不再上专线了
				}
			}
			nCatchDebugStep = 11;
			DEBUG_TRY
			CTeam *pTeam = pUser->GetTeam();
			if(NULL != pTeam)
			{
				pTeam->SetOnlineStatus(pUser->GetID(),false,true);
				if(pTeam->GetOnlineMemberAmount()<=0)
				{
					pUser->DismissTeam();
				}
				else
				{
					if(pUser->GetMap() && pUser->GetMap()->IsDynaMap())
					{
						CMapInstance* pMapInstance = MapInstanceMgr()->FindInstance(pUser->GetMapID());///在副本外调用MapInstanceMgr会导致崩溃
						if(pMapInstance && pMapInstance->IsLeaveSave() /*&& pUser->CanBeSee()*/)///只有玩家在entermap之后下线才能保存在副本中的位置,在上线时可传回
							pTeam->SetLocation(pUser->GetID(),pMapInstance->GetInstanceType(), pUser->GetPosX(), pUser->GetPosY(),true);///队员离线时将坐标记入内存中
					}
					pUser->AbandonTeamLead();
				}
			}
			DEBUG_CATCH2("CUserManager::LogoutUser 玩家下线时处理队伍关系时crash!idUser:%d", pUser->GetID())


			//修改如下：如果我下线，把我下线的消息发给所有加了我为好友的玩家,而非发给我所有的好友
			//[2010-06-09 goto]
			nCatchDebugStep = 15;
			DEBUG_TRY
			for(int i = 0; i < pUser->GetBeFriendAmount(); i++)
			{
				CFriend* pFriend = pUser->GetBeFriendByIndex(i);
				if(pFriend)
				{
					OBJID	idFriend	= pFriend->GetUserID();
					CMsgFriend	msg;
					IF_OK(msg.Create(_FRIEND_OFFLINE, pUser->GetID(), pUser->GetName()))
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, idFriend);
				}
			}
			nCatchDebugStep = 16;
// 			bool bHasTempFriend = false;
// 			for(int j = 0; j < pUser->GetFriendAmount(); j++)
// 			{
// 				CFriend* pFriend = pUser->GetFriendByIndex(j);
// 				if(pFriend && pFriend->GetFriendType()==TEMP_FRIEND_GROUP)
// 				{
// 					bHasTempFriend = true;
// 					break;
// 				}
// 			}
//			if (bHasTempFriend)

			if (pUser->HasTempFriend())//goto 增加判断，不一定精确，可减小压力和出错机率
				pUser->DelTempFriend();////[2010-06-10 goto]删除临时好友
			DEBUG_CATCH2("CUserManager::LogoutUser Friend UserID=%d", pUser->GetID())



			DEBUG_TRY
			if(SynManager()->QuerySyndicate(pUser->GetSynID()))
			{
				///只有正式帮派才会通知成员上下线
				if(pUser->GetSyndicate()->GetInt(SYNDATA_STATE) == 2)
				{
					SynManager()->QuerySyndicate(pUser->GetSynID())->SetOnline(pUser->GetID(),0);
					///离线同地图组广播
					SynManager()->QuerySyndicate(pUser->GetSynID())->BroadcastSynMemberInfoMsg(pUser->GetID());
					// 		SynManager()->QuerySyndicate(GetSynID())->DecOnline();

					///帮派成员上线,将上线信息广播给其它线和其它地图组
					///有人入帮，通知其它线更新数据
					CMsgSynMemberInfo msg;
					if (msg.Create(SynMemberOnlineStatusChange,1,2))
					{
						msg.Append(pUser->GetSynID(),"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
						msg.Append(pUser->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,0);
					}
				}
			}
			DEBUG_CATCH2("CUserManager::LogoutUser SyncOffline UserID=%d", pUser->GetID())

			DEBUG_TRY
			if (pUser->IsInTutorList())
			{
				TUTORLIST info = {0};
				info.idUser = pUser->GetID();
				info.ucGender = pUser->GetSex();
				if (pUser->IsVipActive())
					info.ucVipLev = pUser->GetVipLev();
				else
					info.ucVipLev = 0;
				info.usLev = pUser->GetLev();
				info.dwProfession = pUser->GetProfession(1);
				info.dwBattleNum = pUser->GetMaxScore();
				info.dwImpart = pUser->GetImpart();
				info.dwLastLogoutTime = pUser->GetExitTime();
				info.uIPAddress = 0;
				SafeCopy(info.szName, pUser->GetName(), _MAX_NAMESIZE);

				if (!g_TutorListSystem.TutorLogout(&info))//返回false表示列表中无此人，不需同步
					pUser->DelTutorListMask();
				else
				{
					CMsgSchoolMember msgTutor;
					if (msgTutor.Create(pUser->GetID(), MESSAGEBOARD_SYNLINE, NULL, 0, SYNTUTOR_OFFLINE))
					{
						msgTutor.Append(info);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msgTutor, (OBJID)0);
					}
				}
			}
			DEBUG_CATCH2("CUserManager::LogoutUser TutorLogout UserID=%u", pUser->GetID())

			DEBUG_TRY
			pUser->TutorLogoutHint();
			DEBUG_CATCH2("CUserManager::LogoutUser TutorLogoutHint UserID=%u", pUser->GetID())

			DEBUG_TRY
			if(pUser->IsVipActive() && pUser->GetMap())
			{
				g_VipListMgr.DelVipUser(pUser->GetID());
				//广播到其他线路
				CMsgVipList msg;
				if (msg.Create(pUser->GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_OFFLINE))
				{
					msg.Append(pUser->GetID(), pUser->GetMap()->GetDocID(), pUser->GetLev(), CVipList::GetLineByName(), pUser->GetVipLev(), pUser->GetName());
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
			}
			DEBUG_CATCH2("CUserManager::LogoutUser g_VipListMgr.DelVipUser UserID=%u", pUser->GetID())

			DEBUG_TRY
			CMsgAward msgAward;
			GainAwardInfo info = {0};
			if(msgAward.Create(_GAINAWARD_SPECILLINE,pUser->GetAccountID(),pUser->GetID(),MapGroup(PID)->GetServerName(),&info))
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msgAward );
			DEBUG_CATCH2("CUserManager::LogoutUser _GAINAWARD_SPECILLINE UserID=%u", pUser->GetID())
		}

		nCatchDebugStep = 17;

		pUser->SaveInfo();

		nCatchDebugStep = 6;
			// gm?
		if (pUser->IsGM())
		{
			vector<OBJID>::iterator iter;
			for (iter = m_setGm.begin(); iter != m_setGm.end(); iter++)
			{
				if (pUser->GetID() == *iter)
				{
					m_setGm.erase(iter);
					break;
				}
			}
		}

		nCatchDebugStep = 7;
		IStatusSet* pStatusSet= pUser->QueryStatusSet();
		IDatabase* pDatabase=Database();
		if (pStatusSet && pDatabase)
		{
			for(int i=0;i<pStatusSet->GetAmount();++i)
			{
				IStatus* pStatus=pStatusSet->GetObjByIndex(i);
				if ( pUser->QueryStatusManager().IsValueStatus(pStatus))///状态保存逻辑判断
				{
					int nStatusID = pStatus->GetID();// 彭峰 [6/27/2007]
//modify code by 林德渊 begin 2011.6.22
					if( nStatusID == STATUS_RETORT_MDSX || nStatusID == STATUS_RETORT_MDSX_BUFF2 )
					{
						StatusInfoStruct pStatusInfo = {0};
						pStatus->GetInfo(&pStatusInfo);
						pStatus->ChangeData(pStatusInfo.nPower,pStatusInfo.nSecs,pStatusInfo.nTimes,pUser->GetNewValue(),pUser->GetNewValueTrageid(),pStatusInfo.nParam3,pUser->GetID());
					}
//modify code by 林德渊 end
					pUser->QueryStatusManager().WriteStatusDB( pUser,pDatabase,pStatus );
				}
			}
		}
		DEBUG_CATCH3("logout user [%d] nCatchDebugStep[%d]", idSocket, nCatchDebugStep)
				
		///在玩家对象析构时清除他身上所带的所有cd对像
		pUser->ClearAllCD();
		nCatchDebugStep = 9;
		DEBUG_TRY
		if(m_idProcess == MSGPORT_MAPGROUP_INSTANCE)
		{
			MapInstanceMgr()->OnPlayerExitMap(idMap,pUser->GetID());
		}
		DEBUG_CATCH("MapInstanceMgr()->OnPlayerExitMap(idMap,pUser->GetID()")
        
		UserJinmeiSysMgr()->OnUserLogOut(pUser->GetID());

		InstanceUseMgr()->UserLogOut(pUser->GetID());
		DEBUG_TRY
		nCatchDebugStep = 8;
		m_setUser[idSocket] = NULL;
//		m_setUserName.erase(pUser->GetName());
		QuerySet()->DelObj(pUser->GetID());//此时不会重复delete.因为构造时CreateNew(false)
		RoleManager()->QuerySet()->DelObj(pUser->GetID());

		

		DEBUG_CATCH3("User QuerySet()->DelObj删除时出错,估计是user析构函数肚子疼 idSocket:%d,nCatchDebugStep:%d", idSocket, nCatchDebugStep)
	}
	DEBUG_CATCH2("CUserManager::LogoutUser Friend UserID=%d",idUser)
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserManager::KickOutSocket(SOCKET_ID idSocket, LPCTSTR szReason/*=NULL*/)
{
	CHECKF(idSocket >= 0 && idSocket < m_setUser.size());

	CUserPtr pUser = GetUserBySocketID(idSocket);
	if (pUser)
	{
		if(pUser->GetTeam())
		{
			pUser->AbandonTeamLead();
			pUser->QuitTeam();
		}
		
		//ashulanTest---------
		clock_t clkDebug = clock();
		//--------------------
//		pUser->SaveInfo();		// SaveAllInfo
		//20070623修罗:暂时屏蔽.会有蝴蝶效应
//		if(szReason)
//			LOGWARNING("KickOutSocket()踢出SOCKET_ID[%d], 玩家[%s]。理由：%s", idSocket, pUser->GetName(), szReason);

		//--------------------
		clock_t clkUsed = clock() - clkDebug;
		if(clkUsed > 30)
		{
			LogSave("CUserManager::KickOutSocket pUser->SaveInfo();LOGWARNING overTime(%d) idSocket:%d",
				clkUsed,idSocket);
		}
		//--------------------
	}
	else
	{
		//20070623修罗:暂时屏蔽.会有蝴蝶效应
//		if(szReason)
//			LOGWARNING("KickOutSocket()踢出SOCKET_ID[%d]。理由：%s", idSocket, szReason);
	}

	//ashulanTest---------
	clock_t clkDebug = clock();
	//--------------------

	MapGroup(PID)->QueryIntraMsg()->CloseSocket(idSocket);

	//--------------------
	clock_t clkUsed = clock() - clkDebug;
	if(clkUsed > 30)
	{
		LogSave("CUserManager::KickOutSocket overTime(%d) idSocket:%d",
			clkUsed,idSocket);
	}
	//--------------------

	return true;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::LogoutAllUser()
{
	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while(pUser.Next())
	{
		LogoutUser(pUser->GetSocketID());
	}
}

void CUserManager::KickOutAllSocket()
{
	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while(pUser.Next())
	{
		
		KickOutSocket(pUser->GetSocketID(),"GM_KICKOUT");
		
	}
}

//////////////////////////////////////////////////////////////////////
CUser* CUserManager::GetUser(OBJID idUser)
{
	if(idUser == ID_NONE)
		return NULL;

	IRole* pRole = RoleManager()->QueryRole(idUser);
	CUser* pUser;
	if(pRole && pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
		return pUser;

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CUser* CUserManager::GetUser(LPCTSTR szName)
// {
// 	ASSERT(szName);
// 
// 	CUserManager::Iterator pUser = this->NewEnum();
// 	while(pUser.Next())
// 	{
// 		if(pUser && (strcmp(pUser->GetName(), szName) == 0))
// 			return pUser;
// 	}
// 
// 	return NULL;
// }
///softworms改进后的新版本
CUser* CUserManager::GetUser(LPCTSTR szName)
{
	ASSERT(szName);
	CUser* pUser=NULL;
	OBJID idUser=m_setUserName[szName];
	if(idUser)
		pUser=GetUser(idUser);
	return pUser;
}

//////////////////////////////////////////////////////////////////////
CUser* CUserManager::GetUser(CNetMsg* pMsg)			// for agent
{
	ASSERT(pMsg);

	if(pMsg->IsNpcMsg())
	{
		IRole* pRole = RoleManager()->QuerySet()->GetObj(pMsg->GetNpcID());
		if(!pRole)
			return NULL;
		CUser* pUser = NULL;
		pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
		return pUser;
	}
	else
		return GetUserBySocketID(pMsg->GetSocketID());
}

//////////////////////////////////////////////////////////////////////
bool CUserManager::FindNextSynMember(OBJID idSyn, int& nIter)
{
	CHECKF(idSyn != ID_NONE);

	OBJID	idLastUser	= nIter;
	CUser*	pTarget		= NULL;
	bool	bFindNext	= true;
	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while(pUser.Next())
	{
		if(pUser && pUser->GetSynID() == idSyn)
		{
			if(bFindNext)
			{
				pTarget		= pUser;
				bFindNext	= false;
			}

			if(pUser->GetID() == idLastUser)
			{
				pTarget		= NULL;
				bFindNext	= true;
				continue;
			}
		}
	}

	if(pTarget)
	{
		nIter = pTarget->GetID();
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::BroadcastMsg(CNetMsg* pMsg, CUser* pExcludeSender/*=NULL*/)
{
	CHECK(pMsg);

	BroadcastMapGroupMsg(pMsg, pExcludeSender);

	MapGroup(PID)->QueryIntraMsg()->SendBroadcastMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
void CUserManager::BroadcastMsg(LPCTSTR szMsg, CUser* pExcludeSender/*=NULL*/, LPCTSTR pszEmotion, DWORD dwColor, int nAttribute,CUser* pUserName/*NULL*/,DWORD dwItemID/* = 0*/,DWORD dwItemTypeID/* = 0*/, LPCTSTR pszItemName /*= NULL*/)
{
	CHECK(szMsg);
 	CMsgTalk	msg;
	//  [6/21/2007 Add By HaLei] 
	if (pUserName)
	{
		CMsgTalk	msg;
		if(msg.Create( pUserName->GetName(), ALLUSERS_NAME, szMsg, pszEmotion, dwColor, nAttribute,_TXT_NORMAL,dwItemID,dwItemTypeID,pszItemName))
			BroadcastMsg(&msg, pExcludeSender);		
	}
	else
	{
// 		CMsgTalk	msg;
		if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, pszEmotion, dwColor, nAttribute,_TXT_NORMAL,dwItemID,dwItemTypeID,pszItemName))
			BroadcastMsg(&msg, pExcludeSender);		
	}
// 	  	///单服不广播所有线
// 	if(nAttribute!=_TXTATR_ANNOUNCE)
// 
// 	///单服不广播所有线
 	if(nAttribute!=_TXTATR_ANNOUNCE)
 		///所有广播的talk消息均要上发到世界线程进行关系服务器转发
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, ALLUSERS_NAME);
}

//---------add by cz 广播消息并设置接受者名字主要是为了狮子吼与世界频道的判定
void    CUserManager::BroadCastMsgAndReceiveName\
(LPCTSTR szMsg, CUser* pExcludeSender, LPCTSTR pszEmotion, DWORD dwColor, \
 int nAttribute,CUser* pUserName ,DWORD dwItemID ,DWORD dwItemTypeID, \
 LPCTSTR pszItemName,LPCTSTR pReceiveName)
{

	CHECK(szMsg);
	//  [6/21/2007 Add By HaLei] 
	CMsgTalk	msg;
	if (pUserName)
	{
		DEBUG_TRY
		if(msg.Create( pUserName->GetName(), pReceiveName==NULL?ALLUSERS_NAME:pReceiveName, szMsg, pszEmotion, dwColor, nAttribute,_TXT_NORMAL,dwItemID,dwItemTypeID,pszItemName))
		{
			if(nAttribute != _TXTATR_FORGMTOOL)
				BroadcastMsg(&msg, pExcludeSender);	
		}
		DEBUG_CATCH3("%s SAY %d WorldSpeak When in if", pUserName->GetName(), nAttribute);
	}
	else
	{
		DEBUG_TRY
		if(msg.Create(SYSTEM_NAME, pReceiveName==NULL?ALLUSERS_NAME:pReceiveName, szMsg, pszEmotion, dwColor, nAttribute,_TXT_NORMAL,dwItemID,dwItemTypeID,pszItemName))
		{
			if(nAttribute != _TXTATR_FORGMTOOL)
				BroadcastMsg(&msg, pExcludeSender);
		}
		DEBUG_CATCH3("%s SAY WorldSpeak When in else", pUserName->GetName(), nAttribute);
	}
	DEBUG_TRY
	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, ALLUSERS_NAME);
	DEBUG_CATCH3("%s SAY WorldSpeak When SendMsg", pUserName->GetName(), nAttribute);
}

///广播本地图消息,idMap指定地图编号
void CUserManager::BroadcastMapMsg(OBJID idMap, CNetMsg* pMsg, CUser* pExcludeSender)
{
	UINT		usAction=0;
	UINT		nMsgType=0;
	int			nCatchValue=0;
	CUserPtr	pUser=NULL;
	OBJID idUser=ID_NONE;
	try
	{
		CHECK(pMsg);
		nMsgType=pMsg->GetType();
		memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
		for(USERNAME_SET::iterator it=m_setUserName.begin();it!=m_setUserName.end();++it)
		{
			idUser=it->second;
			
			nCatchValue=1;
			pUser=UserManager()->GetUser(idUser);
			if(pUser==NULL)
				continue;
			
			nCatchValue=2;
			if(pUser->CanBeSee()==false)
				continue;
			
			nCatchValue=3;
			if(pUser->GetMapID()!=idMap)
				continue;
			
			nCatchValue=4;
			if(pExcludeSender && pExcludeSender->GetID() == pUser->GetID())
				continue;
			
			nCatchValue=5;
			pUser->SendMsg(pMsg);
		}
	}
	catch(...)
	{
		LogSave("CUserManager::BroadcastProMsg crash()! msgtype:%d,action:%d,nCatchValue:%d",nMsgType,usAction,nCatchValue);
		if(nMsgType==_MSG_TALK)
		{
			///debug
			CMsgTalk::Dump((char*)pMsg->GetBuf(),pMsg->GetSize());
		}
	}
	
}

///广播门派消息,job1主职业,job2副职业
void CUserManager::BroadcastProMsg(int job,CNetMsg* pMsg, CUser* pExcludeSender)
{
	UINT		usAction=0;
	UINT		nMsgType=0;
	int			nCatchValue=0;
	CUserPtr	pUser=NULL;
	OBJID idUser=ID_NONE;
	try
	{
		CHECK(pMsg);
		nMsgType=pMsg->GetType();
		memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
		for(USERNAME_SET::iterator it=m_setUserName.begin();it!=m_setUserName.end();++it)
		{
			idUser=it->second;
			
			nCatchValue=1;
			pUser=UserManager()->GetUser(idUser);
			if(pUser==NULL)
				continue;
			
			nCatchValue=2;
			if(pUser->CanBeSee()==false)
				continue;
		
			nCatchValue=3;
			if(pUser->GetInfo()->FirJob!=job && pUser->GetInfo()->SecJob!=job)
				continue;

			nCatchValue=4;
			if(pExcludeSender && pExcludeSender->GetID() == pUser->GetID())
				continue;
			
			nCatchValue=5;
			pUser->SendMsg(pMsg);
		}
	}
	catch(...)
	{
		LogSave("CUserManager::BroadcastProMsg crash()! msgtype:%d,action:%d,nCatchValue:%d",nMsgType,usAction,nCatchValue);
		if(nMsgType==_MSG_TALK)
		{
			///debug
			CMsgTalk::Dump((char*)pMsg->GetBuf(),pMsg->GetSize());
		}
	}
	
}

void CUserManager::BroadcastMapGroupMsg(CNetMsg* pMsg, CUser* pExcludeSender)
{
	UINT		usAction=0;
	UINT		nMsgType=0;
	int			nCatchValue=0;
	CUserPtr	pUser=NULL;
	OBJID idUser=ID_NONE;
	try
	{
		CHECK(pMsg);
		nMsgType=pMsg->GetType();
		memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
		for(USERNAME_SET::iterator it=m_setUserName.begin();it!=m_setUserName.end();++it)
		{
			idUser=it->second;

			nCatchValue=1;
			pUser=UserManager()->GetUser(idUser);
			if(pUser==NULL)
				continue;

			nCatchValue=2;
			if(pUser->CanBeSee()==false)
				continue;

			nCatchValue=3;
			if(pExcludeSender && pExcludeSender->GetID() == pUser->GetID())
				continue;

			nCatchValue=4;
			pUser->SendMsg(pMsg);
		}
	}
	catch(...)
	{
		LogSave("CUserManager::BroadcastMapGroupMsg crash()! msgtype:%d,action:%d,nCatchValue:%d",nMsgType,usAction,nCatchValue);
		if(nMsgType==_MSG_TALK)
		{
			///debug
			CMsgTalk::Dump((char*)pMsg->GetBuf(),pMsg->GetSize());
		}
	}

}

void CUserManager::BroadcastPhyleMapGroupMsg( CNetMsg* pMsg,int nPhyle )
{
	ASSERT(pMsg);

	bool	bSendAiServer = true;
	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while( pUser.Next() )
	{
		if(/* nPhyle ==  pUser->GetPhyle() ||*/ pUser->IsGM() )
		{
			MapGroup(PID)->QueryIntraMsg()->SendClientMsg( pUser->GetSocketID(), pMsg );
		}
	}
}
//////////////////////////////////////////////////////////////////////
void CUserManager::JoinMapGroup(SOCKET_ID idSocket, UserInfoStruct* pInfo, ULONG nIP)
{
	CHECK(idSocket != SOCKET_NONE && pInfo);

	if(idSocket >= m_setUser.size())
		m_setUser.resize(idSocket + 1, NULL);			// 保证idSocket索引有效
	CHECK(m_setUser[idSocket] == NULL);

	CUser* pUser = CUser::CreateNewUser(m_idProcess, idSocket, pInfo, nIP);
	if(!pUser)
		return;

	//  [2007-9-26] 朱斌	BUG0001532
	pUser->SetIsAllowTeam(pInfo->bIsAllowTeam);

	ASSERT(!m_setUser[idSocket]);
	m_setUser[idSocket]	= pUser;
	//20071019修罗:---------
	m_setUserName[pUser->GetName()]=pUser->GetID();
	QuerySet()->AddObj(pUser);


	//进入副本，加入副本人员管理
	
	//----------------------
	RoleManager()->QuerySet()->AddObj(pUser->QueryRole());	
	

	DEBUG_TRY
	if(m_idProcess == MSGPORT_MAPGROUP_INSTANCE)
	{
		MapInstanceMgr()->OnPlayerEnterMap(pInfo->toMapid,pUser->GetID());
	}
	DEBUG_CATCH("MapInstanceMgr()->OnPlayerEnterMap(idMap,pUser->GetID());")
}
/*
void CUserManager::NewPlayerState(SOCKET_ID idSocket, char* data)
{
	CHECK(idSocket != SOCKET_NONE && pInfo);

	if(idSocket >= m_setUser.size())
		m_setUser.resize(idSocket + 1, NULL);			// 保证idSocket索引有效
	CHECK(m_setUser[idSocket] == NULL);
	

}
*/
//////////////////////////////////////////////////////////////////////
DWORD CUserManager::GetUserAmount()
{
	DWORD dwUsers = 0;
	for(int i = 0; i < m_setUser.size(); i++)
	{
		if(m_setUser[i])
			dwUsers++;
	}

	return dwUsers;
}

//////////////////////////////////////////////////////////////////////
DWORD CUserManager::GetMapUserAmount(OBJID idMap)
{
	DWORD dwUsers = 0;
	for(int i = 0; i < m_setUser.size(); i++)
	{
		CUser* pUser = m_setUser[i];
		if (pUser && pUser->GetMapID() == idMap)
			dwUsers++;
	}

	return dwUsers;
}

//////////////////////////////////////////////////////////////////////
int CUserManager::GetTeamAmount()
{
	CHECKF(m_pTeamSet);

	return m_pTeamSet->GetAmount();
}

//////////////////////////////////////////////////////////////////////
CTeam* CUserManager::GetTeamByIndex(int nIndex)
{
 	CHECKF(m_pTeamSet);
 
 	CTeam* pTeam	= m_pTeamSet->GetObjByIndex(nIndex);
	return pTeam;
}

//////////////////////////////////////////////////////////////////////
CTeam* CUserManager::GetTeamByLeaderId(OBJID idLeader)
{
	for (int i=0; i<this->GetTeamAmount(); i++)
	{
		CTeam* pTeam	= this->GetTeamByIndex(i);
		if (pTeam && pTeam->GetLeader() == idLeader)
			return pTeam;
	}

	return NULL;
}

CTeam* CUserManager::GetTeamById(OBJID id)
{
	for (int i=0; i<this->GetTeamAmount(); i++)
	{
		CTeam* pTeam	= this->GetTeamByIndex(i);
		if (pTeam && pTeam->GetID() == id)
			return pTeam;
	}
	
	return NULL;
}
//////////////////////////////////////////////////////////////////////
BOOL CUserManager::IsValidTeam(CTeam* pTeam)
{
	if (!pTeam)
		return false;

	for (int i=0; i<this->GetTeamAmount(); i++)
	{
		CTeam* pNowTeam	= this->GetTeamByIndex(i);
		if (pNowTeam && pNowTeam->GetLeader() == pTeam->GetLeader() &&
			pNowTeam->GetMemberAmount() == pTeam->GetMemberAmount())
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
BOOL CUserManager::AddTeam(CTeam* pTeam)
{
 	if (!pTeam)
 		return false;
 
 	m_pTeamSet->AddObj(pTeam);
	return true;
}

//////////////////////////////////////////////////////////////////////
void CUserManager::DelTeam(CTeam* pTeam)
{
	if (!pTeam)
		return;

 	for (int i=0; i<this->GetTeamAmount(); i++)
 	{
 		CTeam* pNowTeam	= this->GetTeamByIndex(i);
 		if (pNowTeam && pNowTeam->GetID() == pTeam->GetID())
 		{
 			m_pTeamSet->DelObj(pTeam->GetID());
 			return;
 		}
 	}
}

//////////////////////////////////////////////////////////////////////
OBJID CUserManager::SpawnNewTeamID()
{
	m_idLastTeam += MapGroup(PID)->GetMapGroupAmount();					//? 保证每个进程组的TEAM_ID不冲突。
	ASSERT(m_idLastTeam < 4012345678);
	return m_idLastTeam;
}

//CUserManager::Iterator CUserManager::NewEnum()						{ return Iterator(RoleManager()->QuerySet()); }


//////////////////////////////////////////////////////////////////////
bool CUserManager::AddCheat(OBJID idUser, int *pnAmount, int *pnTimes)
{
	CHECKF(pnAmount && pnTimes);

	int	nAmount = m_setCheatIDTimes.Size();
	int	nTimes = 1;
	if(m_setCheatIDTimes.IsIn(idUser))
		nTimes = m_setCheatIDTimes.At(idUser) + 1;
	else
		nAmount++;

	m_setCheatIDTimes.Add(idUser, nTimes);

	*pnAmount	= nAmount;
	*pnTimes	= nTimes;
	return true;
}
bool CUserManager::ReSetMapName(const char*szName,int nId)
{
	 	for(USERNAME_SET::iterator it=m_setUserName.begin();it!=m_setUserName.end();++it)
		{
			if (it->second==nId)
			{
				m_setUserName.erase(it);
				m_setUserName[szName]=nId;
			return true;
			}	
		}
		LogSave("bool CUserManager::ReSetMapName(const char*szName,int nId) 找不到玩家");
		return false;
}

void CUserManager::GetPhyleMemberCount(int mapid,int &dragon, int &human, int &gene)//统计某张地图上的玩家总数
{
	dragon = human = gene = 0;
	CUserManager::Iterator pUser = UserManager()->NewEnum();
	while(pUser.Next())
	{
		// 种族修改   08.6.12  桂佳 

//		if(pUser->GetMap()->GetID() == mapid )
//		{
//			if(pUser->GetProfession() & PROF_DRAGON)
//			{
//				dragon++;	
//			}
//			else if(pUser->GetProfession() & PROF_HUMAN)
//			{
//				human++;
//			}
//			else if(pUser->GetProfession() & PROF_GENE)
//			{
//				gene++;
//			}
//		}
		dragon++;
	}
}

void CUserManager::BroadcastMapGroupMsg(LPCTSTR szMsg, CUser *pExcludeSender, LPCTSTR pszEmotion, DWORD dwColor, int nAttribute,CUser* pUserName/*NULL*/)
{
	CHECK(szMsg);
	if (pUserName)
	{
		CMsgTalk	msg;
		if(msg.Create( pUserName->GetName(), ALLUSERS_NAME, szMsg, pszEmotion, dwColor, nAttribute))
			BroadcastMapGroupMsg(&msg, pExcludeSender);		
	}
	else
	{
		CMsgTalk	msg;
		if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, pszEmotion, dwColor, nAttribute))
			BroadcastMapGroupMsg(&msg, pExcludeSender);		
	}
}

void CUserManager::AddTeamLeader(OBJID idLeader)
{
	vector<OBJID>::iterator it = m_setTeamLeader.begin();

	for(; it != m_setTeamLeader.end(); it++)
	{
		if(*it == idLeader)
			return ;
	}

	m_setTeamLeader.push_back(idLeader);
}

void CUserManager::ChangeTeamLeader(OBJID idLeader)
{
}

void CUserManager::DelTeamLeader(OBJID idLeader)
{
	vector<OBJID>::iterator it = m_setTeamLeader.begin();

	for(; it != m_setTeamLeader.end(); it++)
	{
		if(*it == idLeader)
		{
			m_setTeamLeader.erase(it);
			return ;
		}
	}
}

OBJID CUserManager::GetTeamLeaderByIndex(int i)
{
	ASSERT(i>=0 && i<m_setTeamLeader.size());
	return m_setTeamLeader[i];
}

int  CUserManager::GetTeamLeaderAmount()
{
	return m_setTeamLeader.size();
}

void CUserManager::RoleSearchTeam(CUser* pMember)
{
	if(!pMember)
    	return ;
	for(int i = 0; i < GetTeamLeaderAmount(); i++)
	{
		CUser *pLeader = GetUser(m_setTeamLeader[i]);
		if(pLeader && /*pLeader->GetTeam() &&*/ pLeader->GetMapID()==pMember->GetMapID())
		{
			CTeam* pTeam = pLeader->GetTeam();
			if(pTeam)
			{
				CMsgTeam msg;
				if(msg.Create(_MSGTEAM_SEARCH_TEAM, 
					          pTeam->GetTeamTarget(), 
							  pTeam->GetteamLev(),
							  pLeader->GetID(),
							  pLeader->GetName(),
							  pLeader->GetLev(),
							  pTeam->GetMemberAmount()))
				{
					pMember->SendMsg(&msg);
				}
			}
		}
	}
}

void CUserManager::TeamSearchRole(CUser* pUserLeader)
{
	if(!pUserLeader)
		return ;
	CUserManager::Iterator pUser = this->NewEnum();
	while(pUser.Next())
	{
		if(pUser->GetID() != pUserLeader->GetID()
		  && (NULL == pUser->GetTeam())
		  && pUser->GetIsAllowTeam()
		  && pUserLeader->GetMapID() == pUser->GetMapID())
		{
			CMsgTeam msg;
			// By Minsky
			if(msg.Create(pUser->GetID(), 
			   pUser->GetName(), 
			   _MSGTEAM_SEARCH_ROLE))
			{
				pUserLeader->SendMsg(&msg);
			}
		}
	}
}

//竞技场用的，但不是给竞技场内玩家发送，而是发给竞技场外的玩家
void CUserManager::BroadcastMsgForWarGame(int nLevBegin, int nLevEnd, int nWarGameID)
{
	CHECK(nWarGameID);
	int			nCatchValue=0;
	CUserPtr	pUser=NULL;
	OBJID		idUser=ID_NONE;
	try
	{
// 		CHECK(pMsg);
// 		nMsgType=pMsg->GetType();
// 		memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
		CMsgWargame msg;
		IF_NOT(msg.Create(1, WARGAME_ENTER, 2))
			return;

		msg.m_pInfo->ucAmount = 1;
		msg.m_pInfo->stUserList[0].idPlayer = nWarGameID;
		for(USERNAME_SET::iterator it=m_setUserName.begin();it!=m_setUserName.end();++it)
		{
			idUser=it->second;
			
			nCatchValue=1;
			pUser=UserManager()->GetUser(idUser);
			if(pUser==NULL)
				continue;
			
			if(!pUser->IsAlive())
				continue;

			if(!pUser->GetMap())
				continue;

			if(!pUser->QueryWarGame())
				continue;

			nCatchValue=2;
			if(pUser->CanBeSee()==false)
				continue;
			
			nCatchValue=3;
			if(pUser->GetMapID()==MAP_PRISON || pUser->GetMapID()==1026)//监狱和地府不发
				continue;

			nCatchValue=4;
			if(pUser->GetMap()->IsDynaMap())//副本中不发
				continue;
			
			if(pUser->GetLev()<nLevBegin || pUser->GetLev()>nLevEnd)//等级不符
				continue;
			
			if(pUser->QueryWarGame()->HasArenaMask())//已经进过竞技场了
				continue;

			if(pUser->QueryStatus(STATUS_WARGAME))//已经在竞技场内的不发
				continue;

			nCatchValue=5;
			msg.m_pInfo->idUser = idUser;
			pUser->SendMsg(&msg);
		}
	}
	catch(...)
	{
		LogSave("CUserManager::BroadcastMsgForWarGame crash()! nCatchValue:%d",nCatchValue);
	}
	
}
