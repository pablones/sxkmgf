// SynAttr.cpp: implementation of the CSynAttr class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "common.h"
#include "SynAttr.h"
#include "MapGroup.h"
#include "AutoPtr.h"

#include "SettledAction.h"//20070807修罗

//////////////////////////////////////////////////////////////////////
const int	MINI_SUB_SYN			= 20;				// 20人以下的子帮派，算迷你子帮派，堂主为“准堂主”

//军团成员贡献等级上限
const int _SYN_USER_PROFFERLEV_MAX = 5;

//军团贡献等级与贡献值的关联
//贡献级别	升级所需个人贡献度	发放工资额度
//贡献级别	升级所需个人贡献度	贡献对应级别值
//1级贡献度团员	141	0
//2级贡献度团员	200	141
//3级贡献度团员	325	341
//4级贡献度团员	516	666
//5级贡献度团员	0	1182
//const int _SYN_USER_PROFFERNEED[_SYN_USER_PROFFERLEV_MAX] = {0,141,341,666,1182};

//  [6/15/2007]---------彭峰-------
//军团策划案修改

//军团贡献等级与贡献值的关联
//贡献级别	升级所需个人贡献度	发放工资额度
//贡献级别	升级所需个人贡献度	贡献对应级别值
//1级贡献度团员	7	0
//2级贡献度团员	14	7
//3级贡献度团员	28	21
//4级贡献度团员	56	49
//5级贡献度团员	0	105
//const int _SYN_USER_PROFFERNEED[_SYN_USER_PROFFERLEV_MAX] = {0,7,21,49,105};

//20070807修罗:策划案
//团员进阶：
//团员级别	团员进阶所需个人贡献度
//1→2	5
//2→3	42
//3→4	56
//4→5	70
const int _SYN_USER_PROFFERNEED[_SYN_USER_PROFFERLEV_MAX] = {0,5,42,56,70};





MYHEAP_IMPLEMENTATION(CSynAttr,s_heap)
char	szSynAttrTable[]	= _TBL_SYNATTR;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSynAttr::CSynAttr()
{
	m_pData	= NULL;
	m_nProfferInc=0;
}

//////////////////////////////////////////////////////////////////////
CSynAttr::~CSynAttr()
{
// 	if(SynManager()->QuerySyndicate(GetSynID()))
// 	{
// 		SynManager()->QuerySyndicate(GetSynID())->SetOnline(m_pUser->GetID(),false);
// 		///离线同地图组广播
// 		SynManager()->QuerySyndicate(GetSynID())->BroadcastSynMemberInfoMsg(m_pUser->GetID());
// // 		SynManager()->QuerySyndicate(GetSynID())->DecOnline();
// 	}

	///帮派成员上线,将上线信息广播给其它线和其它地图组
	///有人入帮，通知其它线更新数据
// 	CMsgSynMemberInfo msg;
// 	if (msg.Create(SynMemberOnlineStatusChange,1,2))
// 	{
// 		msg.Append(GetSynID(),0,0,0,0,0,"",0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
// 		msg.Append(m_pUser->GetID(),SynMemberOffline,0,0,0,0,"",1);
//		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,0);
// 	}

	SaveInfo();
	if(m_pData)
		m_pData->Release();
}

//////////////////////////////////////////////////////////////////////
CSynAttr* CSynAttr::CreateNew(PROCESS_ID idProcess, CUser* pUser)
{
	CSynAttr* pSynAttr = new CSynAttr;
	CHECKF(pSynAttr);

	pSynAttr->m_idProcess	= idProcess;
	pSynAttr->m_pUser		= pUser;

	return pSynAttr;
}

//////////////////////////////////////////////////////////////////////
// login
//////////////////////////////////////////////////////////////////////
bool CSynAttr::Create()
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	CHECKF(!m_pData);
	CHECKF(m_pUser);

	m_pData	= CSynAttrData::CreateNew();
	CHECKF(m_pData);

	// 是否加入了帮派
	if(!m_pData->Create(m_pUser->GetID(), Database()))
	{
		SAFE_RELEASE (m_pData);			// 未加入帮派时，m_pData为NULL
		return true;
	}

	// 是否帮派删除了
	if(SynManager()->QuerySyndicate(GetSynID()) == NULL)
	{
		SAFE_RELEASE (m_pData);			// 未加入帮派时，m_pData为NULL
		return true;
	}
// 	if(SynManager()->QuerySyndicate(GetSynID()))
// 	{
// 		SynManager()->QuerySyndicate(GetSynID())->SetOnline(m_pUser->GetID(),true);
// 
// 		SynManager()->QuerySyndicate(GetSynID())->BroadcastSynMemberInfoMsg(m_pUser->GetID());
// 	}
// // 	SynManager()->QuerySyndicate(GetSynID())->incOnline();
// 
// 	///帮派成员上线,将上线信息广播给其它线
// 	///有人入帮，通知其它线更新数据
// 	CMsgSynMemberInfo msg;
// 	if (msg.Create(SynMemberOnlineStatusChange,1,2))
// 	{
// 		msg.Append(GetSynID(),0,0,0,0,0,"",0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
// 		msg.Append(m_pUser->GetID(),SynMemberOnline,0,0,0,0,"",1);
// 		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,0);
// 	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::SendInfoToClient()
{
	SynAttrInfoStruct	info;
	GetInfo(&info);

	CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());	// 可能为空
//----------06.10.25王玉波注释并修改以下------begn--------------------------------
//	if (pSyn)
//		pSyn = pSyn->GetMasterSyn();
	if( !pSyn )
		return false;
//--------------------------------------------end-------------------------------------

	CMsgSynAttrInfo msg;
	IF_OK(msg.Create(&info, pSyn))		// pSyn may be null
		m_pUser->SendMsg(&msg);

	return true;
}

//////////////////////////////////////////////////////////////////////
// MapGroup
//////////////////////////////////////////////////////////////////////
bool CSynAttr::SaveInfo()
{
	if(m_pData)
		return m_pData->Update();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::GetInfo(SynAttrInfoStruct* pInfo)
{
	CHECKF(pInfo);

	memset(pInfo, 0, sizeof(SynAttrInfoStruct));
	pInfo->id			= m_pUser->GetID();
	if(m_pData)
	{
		pInfo->idSyn		= m_pData->GetInt(SYNATTRDATA_SYNID);
		pInfo->ucRank		= m_pData->GetInt(SYNATTRDATA_RANK);
		pInfo->ucRankShow	= GetSynRankShow();
		pInfo->nProffer		= m_pData->GetInt(SYNATTRDATA_PROFFER);
		pInfo->nhisCon		=m_pData->GetInt(SYNATTRDATA_HISTORY_CON);
		pInfo->nhisGrow		=m_pData->GetInt(SYNATTRDATA_HISTORY_GROW);
		pInfo->nhisMoney	=m_pData->GetInt(SYNATTRDATA_HISTORY_MONEY);

// 		pInfo->nProfferInc	= m_pData->GetInt(SYNATTRDATA_PROFFERINC);
// 		pInfo->usMantle		= 0;
// 		pInfo->ucLevel		= m_pData->GetInt(SYNATTRDATA_MEMBERLEVEL);
// 		//20070414修罗:修正并增加--------
// 		pInfo->nTimeDetected= m_pData->GetInt(SYNATTRDATA_TIMEDETECTED);
// 		pInfo->nTimeOnline	= m_pData->GetInt(SYNATTRDATA_TIMEONLINE);
// 		pInfo->nTaskBeginTime	= m_pData->GetInt(SYNATTRDATA_TASKBEGINTIME);
// 		pInfo->dwTaskMaskWeek = m_pData->GetInt(SYNATTRDATA_TASKMASKWEEK);
// 		pInfo->dwTaskMaskDay = m_pData->GetInt(SYNATTRDATA_TASKMASKDAY);
		//-------------------------------

	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::AppendSynAttr(SynAttrInfoStruct* pInfo)
{
	CHECKF(!m_pData);
	if(pInfo->idSyn != ID_NONE)
	{
		m_pData = CSynAttrData::CreateNew();
		CHECKF(m_pData);
		IF_NOT(m_pData->Create(GameDataDefault()->GetSynAttrData(), pInfo->id))
			return false;

		m_pData->SetInt(SYNATTRDATA_SYNID, pInfo->idSyn);
		m_pData->SetInt(SYNATTRDATA_RANK, pInfo->ucRank);
		m_pData->SetInt(SYNATTRDATA_PROFFER, pInfo->nProffer);
		m_pData->SetInt(SYNATTRDATA_HISTORY_CON,pInfo->nhisCon);
		m_pData->SetInt(SYNATTRDATA_HISTORY_MONEY,pInfo->nhisMoney);
		m_pData->SetInt(SYNATTRDATA_HISTORY_GROW,pInfo->nhisGrow);
// 		m_pData->SetInt(SYNATTRDATA_PROFFERINC, pInfo->nProfferInc);
// 		m_pData->SetInt(SYNATTRDATA_MEMBERLEVEL, pInfo->ucLevel);
// 		//20070414修罗:修正并增加------
// 		m_pData->SetInt(SYNATTRDATA_TIMEDETECTED,pInfo->nTimeDetected);
// 		m_pData->SetInt(SYNATTRDATA_TIMEONLINE,pInfo->nTimeOnline);//没有此值时,会造成"两个月未上线xx时间,开除军团..."
// 		m_pData->SetInt(SYNATTRDATA_TASKBEGINTIME,pInfo->nTaskBeginTime);
// 		m_pData->SetInt(SYNATTRDATA_TASKMASKWEEK,pInfo->dwTaskMaskWeek);
// 		m_pData->SetInt(SYNATTRDATA_TASKMASKDAY,pInfo->dwTaskMaskDay);
		//-----------------------------
		m_pData->ClearUpdateFlags();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CSynAttr::CreateSyndicate(LPCTSTR szName, LPCTSTR szWord)
{
	CHECKF(!m_pData);
	
	if (!SynManager()->CheckSynName(szName))
	{
		m_pUser->SendSysMsg("帮派名字重名！");
		return false;
	}
	CreateSyndicateInfo	info;
	info.idSyn	= ID_NONE;
	SafeCopy(info.szName, szName, _MAX_NAMESIZE);
	info.idLeader	= m_pUser->GetID();
	SafeCopy(info.szLeader, m_pUser->GetName(), _MAX_NAMESIZE);
	SafeCopy(info.szWord, szWord, 256);
	info.nMoney		= 1000000;
	OBJID idSyn = SynManager()->CreateSyndicate(&info);
	if(idSyn == ID_NONE)
	{
		m_pUser->SendSysMsg("帮派创建失败");
		return false;
	}

	if(!CreateSyn(idSyn,  m_pUser->GetID()))
	{
		::GmLogSave("错误：玩家[%s]创帮成功，但无法创建帮主记录!", m_pUser->GetName());
		LOGERROR("错误：玩家[%s]创帮成功，但无法创建帮主记录!", m_pUser->GetName());
	}
//	CHECKF(m_pData);
//	m_pUser->SpendMoney(nMoney, SYNCHRO_TRUE);
//	m_pData->SetInt(SYNATTRDATA_RANK, RANK_LEADER);
//	m_pData->Update();
	m_pUser->AddHoldTitle(_TITLE_SYNDICATE, 14);//goto 添加帮主称号
	return idSyn;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::DestroySyndicate()
{
	CHECKF(m_pData);
	CHECKF(GetInt(SYNATTRDATA_RANK) == RANK_LEADER);

//	SynManager()->QueryModify()->DestroySyndicate(GetSynID());
	return true;
}
bool CSynAttr::CreateSyn(OBJID idSyn,OBJID idUser)
{
	CHECKF(!m_pData);
	
	SYSTEMTIME ct;
	unsigned long date;
	GetLocalTime(&ct);
	date = (ct.wYear - 2000)*100000000 + ct.wMonth*1000000 + ct.wDay * 10000 + ct.wHour * 100 + ct.wMinute;
	
	m_pData	= CSynAttrData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(GameDataDefault()->GetSynAttrData(), m_pUser->GetID()))
	{
		m_pData->Release();
		m_pData = NULL;
		return false;
	}
	
	m_pData->DeleteRecord();
	m_pData->SetInt(SYNATTRDATA_ID, m_pUser->GetID());
	m_pData->SetInt(SYNATTRDATA_SYNID, idSyn);
	m_pData->SetInt(SYNATTRDATA_RANK, RANK_LEADER);
	m_pData->SetInt(SYNATTRDATA_PROFFER, 0);
	m_pData->SetInt(SYNATTRDATA_jointime, date);
	
	if(m_pData->InsertRecord() != m_pUser->GetID())		//? 已有记录，改为UPDATE
	{
		m_pData->SetInt(SYNATTRDATA_SYNID, idSyn);
		m_pData->SetInt(SYNATTRDATA_RANK, RANK_LEADER);
		m_pData->SetInt(SYNATTRDATA_PROFFER, 0);
		m_pData->SetInt(SYNATTRDATA_jointime, date);
		m_pData->Update();
	}
	
	CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
	if(pSyn)
	{
		///如果已经存在这个成员则不重复添加
		if(pSyn->Hasmember(m_pUser->GetID())==true)
		{
			if(m_pUser)
				pSyn->SetOnline(m_pUser->GetID(),CVipList::GetLineByName(MapGroup(PID)->GetServerName()));
			SynchroInfo(true);
			return true;
		}
		
		MemberInfo info={0};
		info.id = m_pUser->GetID();
		info.rank = RANK_LEADER;
		info.hiscon = 0;
		info.level = m_pUser->GetLev();
		info.firJob = m_pUser->GetProfession(1);
		info.sex=m_pUser->GetSex();
		info.lastlogin = m_pUser->GetLastLogin();
		info.maxpower=m_pUser->GetMaxScore();
		info.line=CVipList::GetLineByName(MapGroup(PID)->GetServerName());
		if(m_pUser->IsVipActive())
			info.vip=m_pUser->GetVipLev();
		else
			info.vip=0;
		SafeCopy(info.szName,m_pUser->GetName(),16);
		// 		pSyn->incOnline();
		
		pSyn->AddMemberList(info);
	}
	SynchroInfo(true);
	int vip=0;
	if(m_pUser->IsVipActive())
		vip=m_pUser->GetVipLev();
	///有人入帮，通知其它线更新数据
	CMsgSynMemberInfo msg;
	if (msg.Create(SynMemberAdd,1,2))
	{
		msg.Append(idSyn,"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
		msg.Append(m_pUser->GetID(),m_pUser->GetName(),m_pUser->GetLev(),m_pUser->GetProfession(1),m_pUser->GetSex(),10,CVipList::GetLineByName(MapGroup(PID)->GetServerName()),
			vip,m_pUser->GetLastLogin(),0,0,0,m_pUser->GetMaxScore());
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::JoinSyn(OBJID idSyn,bool bLeaveLine , OBJID idUser,bool bSend)
{
	if(bLeaveLine)
	{
 		if(GetSynIDByUserID(idUser) != ID_NONE)
 			return false;

		AddSynAttData(idUser,10,0);
		
		CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
		if(pSyn)
		{
			MemberInfo info={0};
			info.id = idUser;
			info.rank = 10;
			info.hisgrow=0;
			info.hiscon=0;
			info.hismoney=0;
			GetSynInfoByID(info);
			if(!pSyn->Hasmember(idUser))
			{
				pSyn->AddMemberList(info);
				if (bSend)
				{
					char szWord[1024]="";
					sprintf(szWord,"%s加入帮派",info.szName);
					CMsgTalk	msgtalk;
					if (msgtalk.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
						pSyn->BroadcastSynMsg(&msgtalk);
				}
					///有人入帮，通知其它线更新数据
					CMsgSynMemberInfo msg;
					if (msg.Create(SynMemberAdd,1,2))
					{
						msg.Append(idSyn,"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
						msg.Append(info.id,info.szName,info.level,info.firJob,info.sex,info.rank,info.line,info.vip,info.lastlogin,info.hiscon,info.hismoney,info.hisgrow,info.maxpower);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
					}
				
			}
			else
				return false;
		}
		return true;
	}


	CHECKF(!m_pData);

	SYSTEMTIME ct;
	unsigned long date;
	GetLocalTime(&ct);
	date = (ct.wYear - 2000)*100000000 + ct.wMonth*1000000 + ct.wDay * 10000 + ct.wHour * 100 + ct.wMinute;

	m_pData	= CSynAttrData::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(GameDataDefault()->GetSynAttrData(), m_pUser->GetID()))
	{
		m_pData->Release();
		m_pData = NULL;
		return false;
	}

	m_pData->DeleteRecord();
	m_pData->SetInt(SYNATTRDATA_ID, m_pUser->GetID());
	m_pData->SetInt(SYNATTRDATA_SYNID, idSyn);
	m_pData->SetInt(SYNATTRDATA_RANK, 10);
	m_pData->SetInt(SYNATTRDATA_PROFFER, 0);
	m_pData->SetInt(SYNATTRDATA_jointime, date);
		
	if(m_pData->InsertRecord() != m_pUser->GetID())		//? 已有记录，改为UPDATE
	{
		m_pData->SetInt(SYNATTRDATA_SYNID, idSyn);
		m_pData->SetInt(SYNATTRDATA_RANK, 10);
		m_pData->SetInt(SYNATTRDATA_PROFFER, 0);
		m_pData->SetInt(SYNATTRDATA_jointime, date);
		m_pData->Update();
	}

	CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
	if(pSyn)
	{
		///如果已经存在这个成员则不重复添加
		if(pSyn->Hasmember(m_pUser->GetID())==true)
		{
			if(m_pUser)
				pSyn->SetOnline(m_pUser->GetID(),CVipList::GetLineByName(MapGroup(PID)->GetServerName()));
			SynchroInfo(true);
			return true;
		}

		MemberInfo info={0};
		info.id = m_pUser->GetID();
		info.rank = RANK_NORMAL;
		info.hiscon = 0;
		info.level = m_pUser->GetLev();
		info.firJob = m_pUser->GetProfession(1);
		info.sex=m_pUser->GetSex();
		info.lastlogin = m_pUser->GetLastLogin();
		info.maxpower=m_pUser->GetMaxScore();
		info.line=CVipList::GetLineByName(MapGroup(PID)->GetServerName());
		if(m_pUser->IsVipActive())
			info.vip=m_pUser->GetVipLev();
		else
			info.vip=0;
		SafeCopy(info.szName,m_pUser->GetName(),16);

		pSyn->AddMemberList(info);
		SynchroInfo(true);
		if (bSend)
		{
			char szWord[1024]="";
			sprintf(szWord,"%s加入帮派",info.szName);
			CMsgTalk	msgtalk;
			if (msgtalk.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
								pSyn->BroadcastSynMsg(&msgtalk);
		}
 			///有人入帮，通知其它线更新数据
			CMsgSynMemberInfo msg;
			if (msg.Create(SynMemberAdd,1,2))
			{
				msg.Append(idSyn,"",0,0,0,0,0,0,0,0,0,0,0);///无奈为了不修改协议，只有多加一个无效的人来传帮派ID
				msg.Append(info.id,info.szName,info.level,info.firJob,info.sex,info.rank,info.line,info.vip,info.lastlogin,0,0,0,info.maxpower);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
			}
			return true;
		
	}
 	return false;
}

//////////////////////////////////////////////////////////////////////
void CSynAttr::UpLevel(int nLevel)
{
	CHECK(GetSynID());

	MESSAGESTR	buf;
	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
	pInfo->idSyn		= GetSynID();
	pInfo->ucFuncType	= SYNFUNC_UP_LEVEL;
//	pInfo->IntParam[0]	= m_pUser->GetLev();//06.10.25王玉波注释并修改
	pInfo->IntParam[ 0 ] = nLevel;
	SafeCopy((char*)&pInfo->IntParam[1], m_pUser->GetName(), _MAX_NAMESIZE);
	pInfo->ucSaveFlag	= true;
	pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int) + _MAX_NAMESIZE;
	MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
}

//////////////////////////////////////////////////////////////////////
void CSynAttr::SynchroInfo(BOOL bAnnounce)		// 修改后，广播自己的属性
{
// 	SynAttrInfoStruct	info;
// 	GetInfo(&info);
// 	CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
// 	CMsgSynAttrInfo msg;
// 	IF_OK(msg.Create(&info, pSyn))		// pSyn may be null
// 		m_pUser->SendMsg(&msg);

	CMsgPlayer msgPlayer;
	IF_OK(msgPlayer.Create(m_pUser->QueryRole()))
		m_pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
	
// 	if(bAnnounce)
// 	{
// 		LPCTSTR pAnnounce = "";
// 		if(pSyn)
// 		{
// 			CSyndicate* pMaster = pSyn->GetMasterSyn();
// 			ASSERT(pMaster);
// 			pAnnounce = pMaster->GetStr(SYNDATA_ANNOUNCE);
// 		}
// 		m_pUser->SendSysMsg(_TXTATR_SYNANNOUNCE, pAnnounce);
// 	}
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::LeaveSyn(bool bKickout/*=false*/, bool bDeleteRecord/*=true*/, bool bSynchro/*=true*/)		// false: 用于帮派解散
{
	if(!m_pData)
		return false;
	char szWord[1024];
	if(bKickout)
	{
		sprintf(szWord, "%s被踢出帮派", m_pUser->GetName());
		CMsgTalk msg;
		if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
			QuerySyndicate()->BroadcastSynMsg(&msg);
	}
	else
	{
		if(QuerySyndicate() && QuerySyndicate()->GetInt(SYNDATA_STATE) != 1)
		{
			sprintf(szWord, "%s退出帮派", m_pUser->GetName());
			CMsgTalk msg;
			if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
				QuerySyndicate()->BroadcastSynMsg(&msg);
		}
	}

	if (m_pUser->GetSynMap()==m_pUser->GetMap() || m_pUser->GetMap()->GetDocID()==1055)
		m_pUser->LeaveSynMap();

	OBJID idOldSyn = GetSynID();
	if(bDeleteRecord)
		m_pData->DeleteRecord();
	m_pData->Release();
	m_pData	= NULL;
	
	if(bSynchro)
	{	
		DelMemberList(idOldSyn, m_pUser->GetID());
		SynchroInfo();
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::DelMemberList(OBJID idOldSyn, OBJID idUser)
{
	CSynPtr pSyn = SynManager()->QuerySyndicate(idOldSyn);
	if(pSyn)
	{
		pSyn->DelMemberList(idUser);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::DonateMoney(int nMoney)
{
	if(!m_pData)
		return false;

	if(nMoney <= 0)
		return false;

	// 检查帮派
	CSynPtr pSyn = SynManager()->QuerySyndicate(GetSynID());
	// 检查同步
	if(!pSyn)
		return false;
	pSyn = pSyn->GetMasterSyn();
	CHECKF(pSyn);

	if(m_pUser->GetMoney() < nMoney)
	{
		m_pUser->SendSysMsg(g_objStrRes.GetStr( 11086 ));//STR_NOT_SO_MUCH_MONEY
		return false;
	}

	if(nMoney<10000)
	{
		m_pUser->SendSysMsg(g_objStrRes.GetStr( 15035 ));//"你不应该看到这个错误.最少捐献金额为1W"
		return false;
	}

	if(nMoney>1000000000)
	{
		m_pUser->SendSysMsg(g_objStrRes.GetStr( 15036 ));//"你不应该看到这个错误.最多捐献金额为10Y"
		return false;
	}

	if(/*pSyn->QueryModify()->AddData(SYNDATA_MONEY, nMoney)*/true)
	{
		m_pUser->SpendMoney(nMoney, SYNCHRO_TRUE);
		//----------begin-------------------06.10.25王玉波修改-------------------
	//	int nAddPro = nMoney / 2000;//20070423修罗:2K对1点个人贡献度
		// 彭峰 军团策划案修改 [6/15/2007]
		int nAddPro = nMoney / 10000;//1W对1点个人贡献度
		if( nAddPro > 0 )
			AddProffer( nAddPro );
		//-----------end---------------------------------------------------------
	//	AddProffer(nMoney/10000);

		MSGBUF szMsg;
		sprintf(szMsg, STR_DONATE_ssd, m_pUser->GetName(), nMoney);
		CMsgTalk	msg;
		IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xFFFFFF, _TXTATR_SYNDICATE))
			pSyn->BroadcastSynMsg(&msg);

		SynchroInfo();
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
//给自己加贡献度
bool CSynAttr::AddProffer(int nData)
{
	CHECKF(m_pData && nData > 0);

	int	nProffer = ::AddToTop(m_pData->GetInt(SYNATTRDATA_PROFFER), nData, SYNPROFFER_LIMIT);
	m_pData->SetInt(SYNATTRDATA_PROFFER, nProffer);

	//---jinggy---添加贡献值的累加值---begin
// 	nProffer = ::AddToTop(m_pData->GetInt(SYNATTRDATA_PROFFERINC), nData, SYNPROFFER_LIMIT);
// 	m_pData->SetInt(SYNATTRDATA_PROFFERINC, nProffer);

	m_nProfferInc += nData;
	if(m_nProfferInc>=200)
	{
		AddProfferToLeader(m_nProfferInc);
		m_nProfferInc =0;
	}
	//---jinggy---添加贡献值的累加值---end

	//20070417修罗:report to world
	ReportProfferToWorld();

	// synchro
	CMsgUserAttrib	msg;
	IF_OK(msg.Create(m_pUser->GetID(), _USERATTRIB_PROFFER, nProffer))
		m_pUser->SendMsg(&msg);
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::DecProffer(int nData)
{
	CHECKF(m_pData && nData > 0);

	int	nProffer = __max(0, (int)m_pData->GetInt(SYNATTRDATA_PROFFER) - nData);
	m_pData->SetInt(SYNATTRDATA_PROFFER, nProffer);

	//20070417修罗:report to world
	ReportProfferToWorld();

	// synchro
	CMsgUserAttrib	msg;
	IF_OK(msg.Create(m_pUser->GetID(), _USERATTRIB_PROFFER, nProffer))
		m_pUser->SendMsg(&msg);

	return true;
}

//////////////////////////////////////////////////////////////////////
void CSynAttr::SetProffer(int nData)
{
	CHECK (m_pData);
	CHECK (nData >= 0);
	nData = __min(nData, SYNPROFFER_LIMIT);

	m_pData->SetInt(SYNATTRDATA_PROFFER, nData);

	//20070417修罗:report to world
	ReportProfferToWorld();

	// synchro
	CMsgUserAttrib	msg;
	IF_OK(msg.Create(m_pUser->GetID(), _USERATTRIB_PROFFER, nData))
		m_pUser->SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::SetRank(int nRank, bool bLeader /*= false*/)
{
	CHECKF(nRank != RANK_LEADER && m_pData->GetInt(SYNATTRDATA_RANK) != RANK_LEADER || bLeader);

	m_pData->SetInt(SYNATTRDATA_RANK, nRank);
	if(bLeader)
		m_pData->Update();

	MESSAGESTR	buf;
	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
	pInfo->idSyn		= GetSynID();
	pInfo->nRank		= this->GetSynRank();
	pInfo->ucFuncType	= SYNFUNC_SET_RANK;
	SafeCopy(pInfo->StrParam, m_pUser->GetName(), _MAX_NAMESIZE);
	pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
	MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
	

	SynchroInfo();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::SetIdRank(OBJID idUser,OBJID idSyn, int nRank, bool bForSave)
{
	if(bForSave)
	{

		SQLBUF	szSQL;
		sprintf(szSQL, "update %s set syn_id = %d , rank = %d where id =%d Limit 1", _TBL_SYNATTR, idSyn, nRank, idUser);
		Database()->ExecuteSQL(szSQL);
// 		QuerySyndicate()->ChgMemberRank(idUser,nRank,true,m_pUser);
// 		return true;
	}
	if(QuerySyndicate()->ChgMemberRank(idUser,nRank,true,m_pUser) == nRank)
	{	
		if(idUser == m_pData->GetID())
		{
			m_pData->SetInt(SYNATTRDATA_SYNID, idSyn);
			m_pData->SetInt(SYNATTRDATA_RANK, nRank);
		}
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::KickoutMember(OBJID idUser, bool IgnorErrorData)
{
	OBJID idSyn = GetSynID();
	if(idSyn == ID_NONE)
		return false;



	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE id=%u && syn_id=%u && rank!=%u LIMIT 1", _TBL_SYNATTR, idUser, idSyn,RANK_LEADER);
	::CAutoPtr<IRecordset> pStatRes = Database()->CreateNewRecordset(szSQL);
	if(!pStatRes && !IgnorErrorData)
	{
		return false;
	}
	char szName[16];
	GetUserNameByID(szName,idUser);
	if(strlen(szName) <= 0)
		return false;
	
	char szWord[1024];
	sprintf(szWord, "%s被踢出帮派", szName);
	
	CMsgTalk msg;
	if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
		QuerySyndicate()->BroadcastSynMsg(&msg);

	sprintf(szSQL, "DELETE FROM %s WHERE id=%u && syn_id=%u && rank!=%u LIMIT 1", _TBL_SYNATTR, idUser, idSyn, RANK_LEADER);
	Database()->ExecuteSQL(szSQL);

	
	DelMemberList(idSyn, idUser);
	return true;
}

//////////////////////////////////////////////////////////////////////
DWORD CSynAttr::GetSynRankShow		()
{
	if(m_pData)
	{
		return m_pData->GetInt(SYNATTRDATA_RANK);
	}
	else
		return RANK_NONE;
}

/*
//////////////////////////////////////////////////////////////////////
int CSynAttr::SumSynMember()
{
	int nSum = 0;
	CSynSet::Iterator pSyn = SynManager()->QuerySynSet()->NewEnum();
	while(pSyn.Next())
	{
		if(pSyn && pSyn->GetMasterSynID() == GetMasterSynID())
		{
			nSum += pSyn->GetInt(SYNDATA_AMOUNT);
		}
	}
	return nSum;
}
*/

//////////////////////////////////////////////////////////////////////
//06.4.21王玉波修改
LPCTSTR CSynAttr::GetRankTitle(int Rank)
{
	if(Rank == RANK_NONE)
		Rank = this->GetSynRank();
	switch( Rank )
	{
	case	RANK_LEADER:
		return "帮主";
		break;
	case	RANK_SUBLEADER:
		return "副帮主";
		break;
	case RANK_FEELLEADER:
		return "精英";
		break;
	case RANK_FEELLEADER1:
	case RANK_FEELLEADER2:
	case RANK_FEELLEADER3:
	case RANK_FEELLEADER4:
	case RANK_SUBFEELLEADER1:
	case RANK_SUBFEELLEADER2:
	case RANK_SUBFEELLEADER3:
	case RANK_SUBFEELLEADER4:
	case RANK_FEELMENBER1:
	case RANK_FEELMENBER2:
	case RANK_FEELMENBER3:
	case RANK_FEELMENBER4:
	case RANK_NORMAL1:			//分堂1帮众
	case RANK_NORMAL2:
	case RANK_NORMAL3:			//分堂3帮众
	case RANK_NORMAL4:			//分堂4帮众
	case RANK_SHOP:
	case RANK_NORMAL:
		return "帮众";
		
	}
	return STR_ERROR;
}

//////////////////////////////////////////////////////////////////////
bool CSynAttr::ChangeSyn(CSyndicate *pSyn, CSyndicate *pTarget)
{
	CHECKF(pSyn && pTarget);
	CHECKF(m_pData->GetInt(SYNATTRDATA_RANK) != RANK_LEADER);
	
	if(pSyn->GetMasterSynID() != pTarget->GetMasterSynID())
		return false;
	
	//	pSyn->QueryModify()->AddData(SYNDATA_AMOUNT, -1);
	DelMemberList(pSyn->GetID(), m_pUser->GetID());
	
	//	pTarget->QueryModify()->AddData(SYNDATA_AMOUNT, 1);
	//	AddMemberList(pTarget->GetID(), m_pUser->GetName(), m_pUser->GetLev(), m_pUser->GetID(), m_pUser->GetSex(), m_pUser->GetProfession());
	
	this->SetIdRank(0,pTarget->GetID(), RANK_NORMAL);
	
	
	//刚加入帮派时,在线时间设为零
	//	m_pData->SetInt(SYNATTRDATA_TIMEONLINE, 0);
	//加入帮派的时间
	time_t tm = time(NULL);
	//	m_pData->SetInt(SYNATTRDATA_TIMEDETECTED, (int)tm);
	
	return true;
}

//////////////////////////////////////////////////////////////////////
CSyndicate* CSynAttr::QuerySyndicate()
{
	if(GetSynID() == ID_NONE)
		return NULL;

	if(m_linkSyn && m_linkSyn->GetID() == GetSynID())
		return m_linkSyn;

	CSyndicate* pSyn = SynManager()->QuerySyndicate(GetSynID());
	if(pSyn)
	{
		m_linkSyn	= pSyn->QueryLink();
	}

	return m_linkSyn;
}

//---jinggy---2004-12-7---获得帮派成员的连续在线时间
unsigned int CSynAttr::GetTimeOnline()
{
	DEBUG_TRY
// 
// 	if(m_pData) 
// 		return m_pData->GetInt(SYNATTRDATA_TIMEONLINE); 

	DEBUG_CATCH(" CSynAttr::GetTimeOnline()")

	return ID_NONE; 
}
/*--------------------------函数IncTimeOnline(),DetectIsBetrySyn(),Betray为系统检测团员在线时间-------*/
//不足条件则开除或降级--------------begin--------------------------------------------------------------

//增加帮派成员的连续在线时间
void CSynAttr::IncTimeOnline(unsigned short usTimeInc)
{
	DEBUG_TRY

	unsigned int uiTimeOnline = GetTimeOnline(); //在线时间到5小时后就 不在增加

	if( uiTimeOnline >= TIMEONLINE_CHECK ) //5小时
		return ;

	unsigned int uiTimeSum = uiTimeOnline + usTimeInc;

//	m_pData->SetInt(SYNATTRDATA_TIMEONLINE,uiTimeSum);
	m_pData->Update();

	//检测是否背叛帮派
	DetectIsBetrySyn();

	DEBUG_CATCH("void CSynAttr::IncTimeOnline(UINT8 uiTimeInc)")

}

bool CSynAttr::CheckJoinSyndays(int nDays)
{	
	DEBUG_TRY

	int nTimeDetected = /*m_pData->GetInt(SYNATTRDATA_TIMEDETECTED)*/0;//may be ms
	time_t tDetected = (time_t) nTimeDetected;
	tm*	pTmDetected	= localtime(&tDetected);
	
	//将时间格式转换为nDays天后的值
	tm	tmE;	
	memset(&tmE, 0, sizeof(tm));	
	tmE =*pTmDetected;

	tmE.tm_mday += nDays;

	time_t	tE	= mktime(&tmE);

	//与当前时间进行比较
	time_t tCurTime = time(NULL);
	
	if(tCurTime >= tE) //若大于或等于1个星期
	{
		return true;
	}
	else
	{
		return false;
	}

	DEBUG_CATCH("bool CSynAttr::CheckJoinWeek()")
}

bool CSynAttr::CheckJoinWeek() //检测是否加入一周
{
	DEBUG_TRY

	int nTimeDetected = /*m_pData->GetInt(SYNATTRDATA_TIMEDETECTED)*/0;//may be ms
	time_t tDetected = (time_t) nTimeDetected;
	tm*	pTmDetected	= localtime(&tDetected);
	
	//将时间格式转换为7天后的值
	tm	tmE;	
	memset(&tmE, 0, sizeof(tm));	
	tmE =*pTmDetected;

	tmE.tm_mday += 7;

	time_t	tE	= mktime(&tmE);

	//与当前时间进行比较
	time_t tCurTime = time(NULL);
	
	if(tCurTime >= tE) //若大于或等于1个星期
	{
		return true;
	}
	else
	{
		return false;
	}

	DEBUG_CATCH("bool CSynAttr::CheckJoinWeek()")

	return true;
}
/*
派的正式成员必须每两个月必须上线10小时，如果没有达到要求，则视为背叛帮派处理。
*/
bool CSynAttr::DetectIsBetrySyn()
{   
	DEBUG_TRY

	//上次的记录时间,将现在的时间和记录的时间进行比较，看是否过了2个月，若过了2个月在线时间没有达到 10个小时，则
	//背叛帮派
	int nTimeDetected = /*m_pData->GetInt(SYNATTRDATA_TIMEDETECTED)*/0;//may be ms
	time_t tDetected = (time_t) nTimeDetected;
	tm*	pTmDetected	= localtime(&tDetected);
	
	//将时间格式转换为2个月后的值
	tm	tmE;	
	memset(&tmE, 0, sizeof(tm));	
	tmE =*pTmDetected;

	tmE.tm_mon += 2;

	time_t	tE	= mktime(&tmE);

	//与当前时间进行比较
	time_t tCurTime = time(NULL);
	
	if(tCurTime >= tE) //若大于或等于2个月
	{	
		unsigned int uiTimeOnline = /*m_pData->GetInt(SYNATTRDATA_TIMEONLINE)*/0;//单位分钟

		if(uiTimeOnline < TIMEONLINE_CHECK ) //连续2个月连续在线时间没有
		{
			this->Betray();
			return true;
		}
		else
		{
			//重新设置在线时间为0
// 			m_pData->SetInt(SYNATTRDATA_TIMEONLINE,0);
// 			m_pData->SetInt(SYNATTRDATA_TIMEDETECTED,tCurTime);
			m_pData->Update();
			return false;
		}
	}

	DEBUG_CATCH("bool CSynAttr::IsFullTimeOnline(int iMonth, int iDay)")

	return true;//未到检测时间 返回TRUE


}
//背叛帮派
void CSynAttr::Betray()
{
	DEBUG_TRY

	//
	char szMsg[ 128 ] ={ 0 };
	if (m_pUser->GetSynRank() <= 50 )	//普通团员
	{
		OBJID idOldSyn = GetSynID();
		//20061024修罗,拿上来
		//-------------------------
		sprintf( szMsg,g_objStrRes.GetStr(17014),m_pUser->GetName() );//"由于%s两个月上线不足5小时,现已被系统开除军团!"
		CMsgTalk	msg;
		IF_OK( msg.Create( SYSTEM_NAME, ALLUSERS_NAME, szMsg ) )
		{
			m_pUser->GetSyndicate()->BroadcastLocalSynMsg( &msg  );
		} 
		//---------------------------
		//---jinggy---从帮派中删除此成员---begin	
		if(m_pData)
		{
			m_pData->DeleteRecord();
			m_pData->Release();
			m_pData	= NULL;
		}
		else
			return;
		DelMemberList(idOldSyn, m_pUser->GetID());
		
	
		/*20061024修罗bug非法访问.放上面克
		sprintf( szMsg,"由于%s两个月上线不足5小时,现已被系统开除军团!",m_pUser->GetName() );
		CMsgTalk	msg;
		IF_OK( msg.Create( SYSTEM_NAME, ALLUSERS_NAME, szMsg ) )
		{
			m_pUser->GetSyndicate()->BroadcastLocalSynMsg( &msg );
		} 
		*/
		//---jinggy---从帮派中删除此成员---end
	}

	//若没有
	//if( 90 == m_pUser->GetSynRank() )//副团长
	else if( 90 == m_pUser->GetSynRank() )//副团长20070407修罗:else
	{
		this->DecProffer( 500 );//贡献度-500
		this->SetRank( RANK_NORMAL );//降为一级团员
		//更新下次检测的时间.
		time_t tCurTime = time(NULL);
// 		m_pData->SetInt( SYNATTRDATA_TIMEONLINE,0 );
// 		m_pData->SetInt( SYNATTRDATA_TIMEDETECTED,tCurTime );
		m_pData->Update();//"由于军团副团长%s两个月上线不足5小时,现已被系统降为一级团员,解除原副团长职务!"
		sprintf( szMsg,g_objStrRes.GetStr(17015),m_pUser->GetName() );
		CMsgTalk	msg;
		IF_OK( msg.Create( SYSTEM_NAME, ALLUSERS_NAME, szMsg ) )
		{
			m_pUser->GetSyndicate()->BroadcastLocalSynMsg( &msg  );
		}
	}
/*	switch(m_pUser->GetSynRank()) 
	{
	case RANK_LEV2:
		{
			nDecProffer = 200;
			this->DecProffer(nDecProffer);			
			this->SetRank(RANK_LEV2);

		}
		break;			
	case RANK_SUBLEADER:
		{
			nDecProffer = 200;
			this->DecProffer(nDecProffer);
			this->SetRank(RANK_LEV2);
		}
		break;
	case RANK_TEAMLEADER:
		{
			nDecProffer = 500;
			this->DecProffer(nDecProffer);
			this->SetRank(RANK_LEV2);
		}
		break;
	default:
		break;
	}*/

	//20070407修罗:没有处理者,也必须更新检查时间
	//---------------------------------
	else
	{
		time_t tCurTime = time(NULL);
// 		m_pData->SetInt( SYNATTRDATA_TIMEONLINE,0 );
// 		m_pData->SetInt( SYNATTRDATA_TIMEDETECTED,tCurTime );
		m_pData->Update();
	}
	//---------------------------------

	SynchroInfo();


	DEBUG_CATCH("void CSynAttr::Betray()")
}
/*---------------------------------------------------------系统检测处理-----------end--------------------*/
//当帮主分配了贡献值后，需要重新设置 成员的 贡献值的增量
void CSynAttr::ResetSynProfferInc()
{
	//m_pData->SetInt(SYNATTRDATA_PROFFERINC, 0);
}
/*
//添加帮派声望增量
void CSynAttr::AddReputeInc(int nReputeInc)
{
	//每帮军团增加10点军团声望，玩家军团贡献增加1点
	if(nReputeInc>=10)
	{
		int nProffer = nReputeInc/10;
		this->AddProffer(nProffer);		
		return;
	}	
}
*/
/*
//添加帮派基金增量
void CSynAttr::AddMoneyInc(int nMoneyInc)
{
	//每帮军团赚取100军团基金（不包括捐献军团基金），军团贡献增加1点。（这里包括招收新玩家）。
	DWORD dwMoneySum =  nMoneyInc;
	if(dwMoneySum/=100)
	{
		this->AddProffer(dwMoneySum);return;
	}	
}
*/
/*
CSyndicate* CSynAttr::GetMasterSyn()
{
	if(GetSynID() != ID_NONE)
	{
		CSynPtr pSyn = SynManager()->QuerySyndicate(GetSynID());
		IF_OK(pSyn)
		{
		}
	}
	
	return NULL;
}*/


static int	PROFFER_TIME_DIV[]	= {10, 20, 40};
// 给领导分贡献度
void CSynAttr::AddProfferToLeader(int nProffer)
{
	int nTime = 0;

	CSyndicate* pSyn = QuerySyndicate();
	if (pSyn)
	{
		CUser* pUser = NULL;

		if (pSyn->GetInt(SYNDATA_LEADERID) != ID_NONE
			&& pSyn->GetInt(SYNDATA_LEADERID) != m_pUser->GetID())
		{
			// 如果本人不是本帮派帮主，且存在帮主，直接找帮主
			CUser* pUser = UserManager()->GetUser(pSyn->GetInt(SYNDATA_LEADERID));
			if (pUser && pUser->QuerySynAttr())
			{
				CHECK(nTime<3);
				int nAddProffer = nProffer/PROFFER_TIME_DIV[nTime];
				pUser->QuerySynAttr()->AddProffer(nAddProffer);
			}
			++nTime;	// 即使帮主不在线，也当作分享了贡献度
		}

		// 向上寻找帮主
		while (!pSyn->IsMasterSyn())
		{
			
		}
	}
}

//改变帮派的成员的等级
bool CSynAttr::SetSynMemberLevel(int nLevel)
{
	if(m_pData)
	{
		//m_pData->SetInt(SYNATTRDATA_MEMBERLEVEL,nLevel); 
		return true;
	}
	return false;
}

int CSynAttr::GetSynMemberLevel()
{
// 	if(m_pData)
// 		return m_pData->GetInt(SYNATTRDATA_MEMBERLEVEL);
	return -1;
}

bool CSynAttr::CreateSubSubSyn(LPCTSTR szName, int nLevel,int nProffer, int nMoney, int nMoneyLeave)
{
/*	CHECKF(m_pData);

	if(m_pUser->GetLev() < nLevel)
	{
		m_pUser->SendSysMsg(STR_NOT_ENOUGH_LEV, nLevel);
		return false;
	}
	if (m_pUser->QuerySynProffer()<nProffer)
	{
		m_pUser->SendSysMsg(STR_SYN_SETSUBLEADER_NOPROFFER,nProffer);
		return false;
	}

	CSyndicate* pFealty = SynManager()->QuerySyndicate(GetSynID());
	CHECKF(pFealty);
	if(pFealty->GetInt(SYNDATA_MONEY) < nMoney)
	{
		m_pUser->SendSysMsg(STR_GUILD_FUND_NOT_ENOUGH, nMoney);
		return false;
	}

	CreateSyndicateInfo	info;
	info.idSyn	= ID_NONE;
	SafeCopy(info.szName, szName, _MAX_NAMESIZE);
	SafeCopy(info.szTenet, DEFAULT_TENET, MAX_TENETSIZE);
	SafeCopy(info.szLeaderTitle, DEFAULT_LEADERTITLE, _MAX_NAMESIZE);
	SafeCopy(info.szMemberTitle, DEFAULT_MEMBERTITLE, _MAX_NAMESIZE);
	info.idLeader	= m_pUser->GetID();
	SafeCopy(info.szLeader, m_pUser->GetName(), _MAX_NAMESIZE);
	info.nMoney		= nMoneyLeave;
	info.idFealty	= GetSynID();

	OBJID idNewSyn = SynManager()->QueryModify()->CreateSyndicate(&info);
	if(idNewSyn == ID_NONE)
	{
		m_pUser->SendSysMsg(STR_FORBIDDEN_GUILD_NAME);
		return false;
	}

	// update new leader
	DelMemberList(GetSynID(), m_pUser->GetName());
	AddMemberList(idNewSyn, m_pUser->GetName(), m_pUser->GetLev());
//	m_pData->SetInt(SYNATTRDATA_SYNID, idNewSyn);
//	m_pData->SetInt(SYNATTRDATA_RANK, RANK_LEADER);
//	m_pData->Update();
// update later	SynchroInfo();

	// update syndicate
	pFealty->QueryModify()->AddData(SYNDATA_MONEY, -1*nMoney);
	pFealty->QueryModify()->AddData(SYNDATA_AMOUNT, -1);

	// synchro relation
	CMsgSyndicate	msg;
	IF_OK(msg.Create(SET_SUBSUBSYN,pSyn->GetID(),pSynTeam->GetID()))
		pFealty->BroadcastSynMsg(&msg);
*/
	return true;
}


int CSynAttr::GetSynRank()
{
	if(GetSynID() != ID_NONE)
		return m_pData->GetInt(SYNATTRDATA_RANK);
	else
		return RANK_NONE;
}
//-------------------------------------------
BOOL CSynAttr::SetTaskBeginTime(DWORD curTime)//设置军团任务开始时间
{
	if(m_pData)
	{
//		m_pData->SetInt(SYNATTRDATA_TASKBEGINTIME,curTime); 
		m_pData->Update();
		return true;
	}
	return false;
}
//--------------------------------------------
int	CSynAttr::GetProfferLev()//20070412修罗:贡献等级.与经验奖励相关
{
	int curProffer = GetProffer();//当前贡献值

	int profferLev = 0;
	//贡献等级算法------------
	int nProffer = GetProffer();//贡献值
	for(int j=_SYN_USER_PROFFERLEV_MAX-1;j>=0;j--)
	{
		if(nProffer>=_SYN_USER_PROFFERNEED[j])
		{
			break;
		}
	}
	//j,即是贡献等级-1
	CHECKF(j>=0 && j<_SYN_USER_PROFFERLEV_MAX);
	profferLev=j+1;
	//------------------------

	return profferLev;
}
//------------------------------------------
BOOL CSynAttr::ReportProfferToWorld()//向世界线程汇报贡献值
{
	CSyndicate* pSyn = QuerySyndicate();
	if(!pSyn)
		return false;

	CHECKF(m_pUser);
	/////////////////////////////////通知其他线更新数据/////////////////////////////////////////
				MemberInfo info={0};
				info.hiscon = GetHistoryCon();
				info.hisgrow=GetHistoryGrow();
				info.hismoney=GetHistoryMoney();
				info.id=m_pUser->GetID();
				SafeCopy(info.szName,m_pUser->GetName());
				CMsgSynMemberInfo msg;
				if (msg.Create(SynProfferChange,1,1))
				{
					msg.Append(info.id,info.szName,0,0,0,0,0,0,pSyn->GetID(),info.hiscon,info.hisgrow,info.hismoney,0);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,0);
				}
//////////////////////////////////////////////////////////////////////////
	return true;
}
//------------------------------------------
BOOL CSynAttr::GetTaskMaskWeek(unsigned short bit32)//获取军团任务掩码.周
{
	CHECKF(bit32>=1 && bit32<32);

	if(m_pData)
	{
		DWORD taskMaskWeek = /*m_pData->GetInt(SYNATTRDATA_TASKMASKWEEK)*/0;
		DWORD taskMaskBit = 0x01;
		return taskMaskWeek&(taskMaskBit<<(bit32-1));
	}
	return false;
}
//------------------------------------------
BOOL CSynAttr::SetTaskMaskWeek(unsigned short bit32)//设置军团任务掩码.周
{
	CHECKF(bit32>=1 && bit32<32);

	if(m_pData)
	{
// 		DWORD taskMaskWeek = m_pData->GetInt(SYNATTRDATA_TASKMASKWEEK);
// 		DWORD taskMaskBit = 0x01;
// 		m_pData->SetInt(SYNATTRDATA_TASKMASKWEEK,taskMaskWeek|(taskMaskBit<<(bit32-1))); 
// 		m_pData->Update();
		return true;
	}
	return false;
}
//-------------------------------------------
BOOL CSynAttr::GetTaskMaskDay(unsigned short bit32)//获取军团任务掩码.天
{
	CHECKF(bit32>=1 && bit32<32);

	if(m_pData)
	{
		DWORD taskMaskWeek = /*m_pData->GetInt(SYNATTRDATA_TASKMASKDAY)*/0;
		DWORD taskMaskBit = 0x01;
		return taskMaskWeek&(taskMaskBit<<(bit32-1));
	}
	return false;
}
//------------------------------------------
BOOL CSynAttr::SetTaskMaskDay(unsigned short bit32)//设置军团任务掩码.天
{
	CHECKF(bit32>=1 && bit32<32);

	if(m_pData)
	{
// 		DWORD taskMaskWeek = m_pData->GetInt(SYNATTRDATA_TASKMASKDAY);
// 		DWORD taskMaskBit = 0x01;
// 		m_pData->SetInt(SYNATTRDATA_TASKMASKDAY,taskMaskWeek|(taskMaskBit<<(bit32-1))); 
// 		m_pData->Update();
		return true;
	}
	return false;
}

//[游途道标 2008/9/25]玩家登陆时记录其在线
void CSynAttr::UserLogin(OBJID idUser, OBJID idSyn)
{
	MESSAGESTR	buf;
	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
	//[游途道标 2008/9/24]增加对idUser,nSex,nProfession三个成员的赋值
	pInfo->idUser		= idUser;
	pInfo->bOnline		= true;
	pInfo->idSyn			= idSyn;
	pInfo->ucFuncType	= SYNFUNC_LOGIN;
	pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int) + _MAX_NAMESIZE;
	MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
	return;
}

//[游途道标 2008/9/25]玩家退出时记录其在线
void CSynAttr::UserLogout(OBJID idUser, OBJID idSyn)
{
	MESSAGESTR	buf;
	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
	//[游途道标 2008/9/24]增加对idUser,nSex,nProfession三个成员的赋值
	pInfo->idUser		= idUser;
	pInfo->bOnline		= false;
	pInfo->idSyn			= idSyn;
	pInfo->ucFuncType	= SYNFUNC_LOGOUT;
	pInfo->nSize		= sizeof(SynFuncInfo0) + sizeof(int) + _MAX_NAMESIZE;
	MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);
	return;
}

bool CSynAttr::AddSynAttData(OBJID idUser,int nRank,int nProffer)
{
	SYSTEMTIME ct;
	unsigned long date;
	GetLocalTime(&ct);
	date = (ct.wYear - 2000)*100000000 + ct.wMonth*1000000 + ct.wDay * 10000 + ct.wHour * 100 + ct.wMinute; 

	SQLBUF	szSQL;
	sprintf(szSQL, "INSERT INTO %s SET id=%d,syn_id=%d,rank=%d,proffer=%d,jointime=%u", _TBL_SYNATTR, idUser,GetSynID(),nRank,nProffer,date);
	return Database()->ExecuteSQL(szSQL); 
}

OBJID CSynAttr::GetSynIDByUserID(OBJID idUser)
{
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT syn_id FROM %s WHERE id ='%d'", _TBL_SYNATTR, idUser);
	::CAutoPtr<IRecordset> pStatRes = Database()->CreateNewRecordset(szSQL);
	if(pStatRes)
		return pStatRes->GetInt(0);
	else
		return 0;
}

void CSynAttr::GetMateNameByID(char *szPlayName , OBJID id)
{
	SQLBUF	szSQL;
	sprintf(szSQL, "select mate from %s where id = '%d'",
		_TBL_USER,
		id);
	::CAutoPtr<IRecordset> pStatRes = Database()->CreateNewRecordset(szSQL);
	if (!pStatRes)
	{
		strcpy(szPlayName,"无");
		return;
	}
	else
	{
		strcpy(szPlayName,pStatRes->GetStr(0));
		return;
	}
}

OBJID CSynAttr::GetAccountByID( OBJID id)
{
	SQLBUF	szSQL;
	sprintf(szSQL, "select account_id from %s where id = '%d'",
		_TBL_USER,
		id);
	::CAutoPtr<IRecordset> pStatRes = Database()->CreateNewRecordset(szSQL);
	if (!pStatRes)
	{	
		return ID_NONE;
	}
	else
	{		
		return pStatRes->GetInt(0);
	}
}

void CSynAttr::GetSynInfoByID(MemberInfo &info)
{
	SQLBUF	szSQL;
	sprintf(szSQL, "select level,Fir_Job,name,sex,last_login,vipstate,viplev from %s where id = '%d'", _TBL_USER, info.id);
	
	::CAutoPtr<IRecordset> pStatRes = Database()->CreateNewRecordset(szSQL);
	if (pStatRes)
	{
		info.level  = pStatRes->GetInt(0);
		info.firJob = pStatRes->GetInt(1);
		 info.sex= pStatRes->GetInt(3);
		strcpy(info.szName, pStatRes->GetStr(2));
		info.lastlogin=pStatRes->GetInt(4);
		if (pStatRes->GetInt(5) > 0)
			info.vip=pStatRes->GetInt(6);
		else
			info.vip=0;
	}
}

int CSynAttr::GetRankByID(OBJID idUser,OBJID idSyn)
{
	SQLBUF	szSQL;
	sprintf(szSQL, "select rank from %s where id = %u and syn_id = %u ", _TBL_SYNATTR, idUser,idSyn);
	::CAutoPtr<IRecordset> pStatRes = Database()->CreateNewRecordset(szSQL);
	if (pStatRes)
	{
		return pStatRes->GetInt(0);
	}
	return 0;
}
void CSynAttr::GetUserNameByID(char* name, OBJID idUser)
{
	SQLBUF	szSQL;
	sprintf(szSQL, "select name from %s where id = '%d'", _TBL_USER, idUser);
	::CAutoPtr<IRecordset> pStatRes = Database()->CreateNewRecordset(szSQL);
	if (pStatRes)
	{
		strcpy(name, pStatRes->GetStr(0));
	}
}

OBJID CSynAttr::GetUserIDByName(char *szPlayName)
{
	SQLBUF	szSQL;
	sprintf(szSQL, "select id from %s where name = '%s'",
		_TBL_USER,
		szPlayName);
	::CAutoPtr<IRecordset> pStatRes = Database()->CreateNewRecordset(szSQL);
	if (!pStatRes)
	{
		return NULL;
	}
	else
	{
		return pStatRes->GetInt(0);
	}
}

//[游途道标 2008/9/28]帮主转让
bool CSynAttr::Demise(OBJID idOldHand, char *szOldName, OBJID idNewHand, char *szNewName)
{
	m_pData->SetInt(SYNATTRDATA_RANK, 10);
	m_pData->Update();

	SQLBUF	szSQL;
	sprintf(szSQL, "UPDATE %s SET rank = 100  WHERE id = %u LIMIT 1",
		_TBL_SYNATTR,
		idNewHand);
	Database()->ExecuteSQL(szSQL);

	MESSAGESTR	buf;
	SynFuncInfo0*	pInfo = (SynFuncInfo0*)&buf;
	pInfo->idSyn		= GetSynID();
	pInfo->nRank		= 10;
	pInfo->ucFuncType	= SYNFUNC_SET_RANK;
	SafeCopy(pInfo->StrParam, szOldName, _MAX_NAMESIZE);
	pInfo->nSize		= sizeof(SynFuncInfo0) + _MAX_NAMESIZE;
	MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);

	pInfo->nRank		= 100;
	pInfo->ucFuncType	= SYNFUNC_SET_RANK;
	SafeCopy(pInfo->StrParam, szNewName, _MAX_NAMESIZE);
	MapGroup(PID)->QuerySynMessage()->ChangeSyndicate(pInfo);

	SynchroInfo();

	return true;
}

BOOL CSynAttr::CheckSynName(char *szName)
{
	return true;
}

//////--jay
void CSynAttr::SetMask(int nMask)
{
	SetInt(SYNATTRDATA_MASK,nMask);
}
int  CSynAttr::GetMask()
{
	CHECKF(m_pData);
	return GetInt(SYNATTRDATA_MASK);
}
void CSynAttr::SetGift1(OBJID nGift)
{
	SetInt(SYNATTRDATA_GIFT1,nGift);

}
void CSynAttr::SetGift2(OBJID nGift)
{
	SetInt(SYNATTRDATA_GIFT2,nGift);
}
OBJID CSynAttr::GetGift1()
{
	CHECKF(m_pData);
		return GetInt(SYNATTRDATA_GIFT1);
}
OBJID CSynAttr::GetGift2()
{
	CHECKF(m_pData);
	return GetInt(SYNATTRDATA_GIFT2);
}
int CSynAttr::GetHistoryCon()
{
	CHECKF(m_pData);
	return GetInt(SYNATTRDATA_HISTORY_CON);

}
void CSynAttr::SetHistoryCon(int nCon)
{
	if (nCon >= 0)
		SetInt(SYNATTRDATA_HISTORY_CON,nCon);
}
int CSynAttr::GetHistoryMoney()
{
	CHECKF(m_pData);
	return GetInt(SYNATTRDATA_HISTORY_MONEY);
}
void CSynAttr::SetHistoryMoney(int nMoney)
{
	if (nMoney >= 0)
		SetInt(SYNATTRDATA_HISTORY_MONEY,nMoney);
}
int CSynAttr::GetHistoryGrow()
{
	CHECKF(m_pData);
	return GetInt(SYNATTRDATA_HISTORY_GROW);
}
void CSynAttr::SetHistoryGrow(int nGrow)
{
	if (nGrow >= 0)
		SetInt(SYNATTRDATA_HISTORY_GROW,nGrow);
}
