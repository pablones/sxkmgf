// SynWorldShell.cpp: implementation of the CSyndicateWorld class.
//
//////////////////////////////////////////////////////////////////////
// #undef _WINDOWS_
// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0510 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0500	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif
#define	_WINSOCKAPI_		// 阻止加载winsock.h
// #include "afx.h"
#include "afxdisp.h"

#include "AllMsg.h"

#include "I_MessagePort.h"

#include "../share/define.h"

#include "MessageBoard.h"

#include "UserList.h"

#include "WorldKernel.h"

#include "SynWorldShell.h"


//////////////////////////////////////////////////////////////////////
const int	SYNMONEY_LIMIT		= 2000000000;		// 基金的限制额
const int	MEMBERLIST_SIZE		= 10;				// 帮众列表的每次数量
const int	MEMBERLIST_LIMIT	= 1234567890;		// 帮众列表的限制额
//20070416修罗:需要以此处获得remoteCall相关.
//注意保持与user.h内同步
enum REMOTE_CALL{
		REMOTE_CALL_DROP_LEADER,
		REMOTE_CALL_LEAVE_SYN,
		REMOTE_CALL_TEAM_CLOSE_MONEY,
		REMOTE_CALL_TEAM_CLOSE_ITEM,
		REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS,
		REMOTE_CALL_TEAM_HUNTKILL,
		REMOTE_CALL_SYNPAYOFF,//20070414修罗:军团发工资到个人
		REMOTE_CALL_GOMATE,///瞬称到配偶请求
		REMOTE_CALL_REPLYGOMATE,///回应瞬称到配偶请求	
		REMOTE_CALL_KICKOUTPLAYER,//踢掉玩家
		REMOTE_CALL_WARGAME,		//	战争活动使用
		REMOTE_CALL_SYNADDMENBER,
		REMOTE_CALL_SYNADDHAVE,
		REMOTE_CALL_SYNADDOK,
};

//军团成员贡献等级上限
const int _SYN_USER_PROFFERLEV_MAX = 5;

//军团成员贡献等级与工资的关联
//团员贡献度等级	获得工资数	算法(系数m)
//1级贡献度团员	A	A
//2级贡献度团员	B	A(1+m%)
//3级贡献度团员	C	B(1+m%)
//4级贡献度团员	D	C(1+m%)
//5级贡献度团员	E	D(1+m%)
const int _SYN_USER_PAY_FACTOR = 10;//因子
const int _SYN_USER_PAY_PROFFERLEV_1 = 100;//1级贡献度
const int _SYN_USER_PAY_PROFFERLEV_2 = _SYN_USER_PAY_PROFFERLEV_1*(100+_SYN_USER_PAY_FACTOR)/100;//2级贡献度
const int _SYN_USER_PAY_PROFFERLEV_3 = _SYN_USER_PAY_PROFFERLEV_2*(100+_SYN_USER_PAY_FACTOR)/100;//3级贡献度
const int _SYN_USER_PAY_PROFFERLEV_4 = _SYN_USER_PAY_PROFFERLEV_3*(100+_SYN_USER_PAY_FACTOR)/100;//4级贡献度
const int _SYN_USER_PAY_PROFFERLEV_5 = _SYN_USER_PAY_PROFFERLEV_4*(100+_SYN_USER_PAY_FACTOR)/100;//5级贡献度

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
//1级贡献度团员	7	
//2级贡献度团员	14	
//3级贡献度团员	28	
//4级贡献度团员	56	
//5级贡献度团员	0	
//const int _SYN_USER_PROFFERNEED[_SYN_USER_PROFFERLEV_MAX] = {0,7,14,28,56};

//20070807修罗:策划案
//团员进阶：
//团员级别	团员进阶所需个人贡献度
//1→2	5
//2→3	42
//3→4	56
//4→5	70
const int _SYN_USER_PROFFERNEED[_SYN_USER_PROFFERLEV_MAX] = {0,5,42,56,70};



const int _SYN_PAYOFF_TEX = 30;//发工资税.百分比.100为100%



MYHEAP_IMPLEMENTATION(CSyndicateWorld,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSyndicateWorld::CSyndicateWorld()
{
	m_pData	= NULL;
	m_ptrMsgBd	= CMessageBoard::CreateNew(_TXTATR_MSG_SYN);
}

//////////////////////////////////////////////////////////////////////
CSyndicateWorld::~CSyndicateWorld()
{
	if(m_pData)
	{
		m_pData->Update();
		m_pData->Release();
	}

} 

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::Create(IRecordset* pRes)
{
	ASSERT(!m_pData);

	m_pData = CSynWorldBase::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pRes))
		return false;

	if(m_pData->GetInt(SYNDATA_STATE) == 3)		// 已删除的帮派
		return false;

	return true;

	// 统计帮派人数
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT COUNT(*) FROM %s as s LEFT JOIN %s AS u ON u.id=s.id WHERE s.syn_id=%u && u.id IS NOT NULL", _TBL_SYNATTR, _TBL_USER, m_pData->GetID());
	IRecordset* pStatRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
	if(pStatRes)
	{
		m_pData->SetInt(SYNDATA_AMOUNT, pStatRes->GetInt(0));	// 0: count
		m_pData->Update();
		pStatRes->Release();
	}

	// 载入帮众列表
	//[游途道标 2008/9/24]增加查询语句多返回性别和职业数据
	sprintf(szSQL, "SELECT u.id, u.name, s.rank, u.level, s.proffer, u.sex, u.profession,u.last_exit FROM %s as s LEFT JOIN %s AS u ON u.id=s.id WHERE s.syn_id=%u && u.id IS NOT NULL ORDER BY s.rank DESC, u.name LIMIT %d",
			_TBL_SYNATTR, _TBL_USER, m_pData->GetID(), MEMBERLIST_LIMIT);
	 pStatRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
	if(pStatRes)
	{
		for(int i = 0; i < pStatRes->RecordCount(); i++,pStatRes->MoveNext())
		{
			//[游途道标 2008/9/24]增加性别和职业数据的填入
			//[游途道标 2008/9/25]增加对在线人员的输入
			m_setMember.push_back(MemberInfo(pStatRes->GetInt(0),//玩家标识
											pStatRes->GetStr(1),//玩家名
											pStatRes->GetInt(2),//玩家职位
											pStatRes->GetInt(3),//玩家等级
											pStatRes->GetInt(4),//玩家威望
											pStatRes->GetInt(5),//玩家性别
											pStatRes->GetInt(6),
											false,
											0));//玩家职业
		}
		pStatRes->Release();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::Create(IRecordset* pDefault, const CreateSyndicateInfo* pInfo, bool bSave)
{
	ASSERT(!m_pData);

	OBJID idSyn = bSave ? ID_NONE : pInfo->idSyn;
	m_pData = CSynWorldBase::CreateNew();
	CHECKF(m_pData);
	IF_NOT(m_pData->Create(pDefault, idSyn))
		return false;

	m_pData->SetInt(SYNDATA_ID, idSyn);
	m_pData->SetStr(SYNDATA_NAME, pInfo->szName, _MAX_NAMESIZE);
//	m_pData->SetStr(SYNDATA_ANNOUNCE, pInfo->szLeaderTitle, MAX_TENETSIZE);
	m_pData->SetInt(SYNDATA_LEADERID, pInfo->idLeader);
	m_pData->SetStr(SYNDATA_LEADERNAME, pInfo->szLeader, _MAX_NAMESIZE);
	m_pData->SetInt(SYNDATA_MONEY, pInfo->nMoney);

	if(bSave)
	{
		return m_pData->InsertRecord() != ID_NONE;
	}
	else
	{
		m_pData->ClearUpdateFlags();
		return true;
	}
}

//////////////////////////////////////////////////////////////////////
// modify
//////////////////////////////////////////////////////////////////////
//[游途道标 2008/9/24]Demise后新加两个参数配合AddMember调用
bool CSyndicateWorld::Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave, int nOldSex, int nOldProfession)	
{
	//[游途道标 2008/9/28]玩家离线单独处理
	if (idOldLeader == 0 && idNewLeader != 0)
	{
		m_pData->SetInt(SYNDATA_LEADERID, idNewLeader);
		m_pData->SetStr(SYNDATA_LEADERNAME, szNewLeader, _MAX_NAMESIZE);
		m_pData->Update();
		return true;
	}
	if(!( idOldLeader==(OBJID)m_pData->GetInt(SYNDATA_LEADERID) || (OBJID)m_pData->GetInt(SYNDATA_LEADERID)==ID_NONE ))	//? 兼容于旧数据
		return false;
	NAMESTR	szOldLeader;
	SafeCopy(szOldLeader, m_pData->GetStr(SYNDATA_LEADERNAME), _MAX_NAMESIZE);
	m_pData->SetInt(SYNDATA_LEADERID, idNewLeader);
	m_pData->SetStr(SYNDATA_LEADERNAME, szNewLeader, _MAX_NAMESIZE);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();

	int	RANK_LEV2		= 10;
	int RANK_LEADER		=100;
	SQLBUF	szSQL;
	sprintf(szSQL, "UPDATE %s SET syn_id=%u,rank=%d WHERE id=%u LIMIT 1",
			_TBL_SYNATTR, idBackSyn, RANK_LEV2, idOldLeader);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);

	// synchro member list
//	if(GetID() != idBackSyn)
	{
		CSyndicateWorld * pBackSyn = SynWorldManager()->QuerySyndicate(idBackSyn);
		IF_OK(pBackSyn)
		{
			int proffer = GetMemberProffer(idOldLeader);
			CPlayer *player = UserList()->GetPlayer(szOldLeader);
			DelMember(szOldLeader);
			//[游途道标 2008/9/24]修正AddMember新增参数后调用错误.
			//[游途道标 2008/9/25]增加对是否在线的登记.
			pBackSyn->AddMember(idOldLeader,
				szOldLeader, 
				RANK_LEV2,
				player==NULL?0:player->m_nLevel,
				proffer,
				nOldSex,
				nOldProfession,
				UserList()->IsLoginMapGroup(player->idSocket));

			proffer = GetMemberProffer(idNewLeader);
			player = UserList()->GetPlayer(szNewLeader);
			pBackSyn->DelMember(szNewLeader);
			//[游途道标 2008/9/24]修正AddMember新增参数后调用错误.
			//[游途道标 2008/9/25]增加对是否在线的登记.
			AddMember(idNewLeader,
				szNewLeader,
				RANK_LEADER,
				player==NULL?0:player->m_nLevel,
				proffer,
				nOldSex,
				nOldProfession,
				UserList()->IsLoginMapGroup(player->idSocket));
		}
	}

	return true;
}

/////////////////////////////////////////////////////d/////////////////
bool CSyndicateWorld::SetTenet(LPCTSTR szTenet, bool bSave)		
{
// 	CHECKF(strlen(szTenet)<MAX_TENETSIZE);
// 	
// 	m_pData->SetStr(SYNDATA_TENET, szTenet, MAX_TENETSIZE);
// 	if(bSave)
// 		m_pData->Update();
// 	else
// 		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)		
{
	m_pData->SetStr(SYNDATA_ANNOUNCE, szLeaderTitle, MAX_TENETSIZE);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)		
{
	return true; 
}
//////////////////////////////////////////////////////////////////////
//06.10.25王玉波添加,某个军团成员等级改变了
bool CSyndicateWorld::SetMemberLevel( const char* pTargetName,int nLevel )
{
	CHECKF ( pTargetName );
	for( int i = 0;i < m_setMember.size();i++ )
	{
		if( 0 == strcmp( m_setMember[i].szName.c_str(),pTargetName ) )
		{
			m_setMember[ i ].nLevel = nLevel;
			//20070405修罗:移动到上层CSyndicateWorld::ChangeSyndicate统一处理
			//m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//更新时间
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::AddData(SYNDATA idx, int nData, bool bSave)
{
	int nAllData = ::AddToTop(m_pData->GetInt(idx), nData, SYNMONEY_LIMIT);		// 用于金钱和帮众数量
	if(nAllData < 0)
		nAllData = 0;
	m_pData->SetInt(idx, nAllData);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetData(SYNDATA idx, int nData, bool bSave)
{
	m_pData->SetInt(idx, nData);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::PresentMoney(OBJID idSyn, int nMoney, bool bSave)
{
	if(nMoney <= 0 || m_pData->GetInt(SYNDATA_MONEY) < nMoney)
		return false;
	CSyndicateWorld* pTarget = GameWorld()->QuerySynManager()->QuerySyndicate(idSyn);
	if(!pTarget)
		return false;

	m_pData->SetInt(SYNDATA_MONEY, m_pData->GetInt(SYNDATA_MONEY)-nMoney);
	pTarget->m_pData->SetInt(SYNDATA_MONEY, pTarget->m_pData->GetInt(SYNDATA_MONEY)+nMoney);
	if(bSave)
	{
		m_pData->Update();
		pTarget->m_pData->Update();
	}
	else
	{
		m_pData->ClearUpdateFlags();
		pTarget->m_pData->ClearUpdateFlags();
	}
	::GmLogSave("EVENT: 帮派[%s]向帮派[%s]划拨基金[%d]两银子。", GetStr(SYNDATA_NAME), pTarget->GetStr(SYNDATA_NAME), nMoney);
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetSynName(LPCTSTR szSynName, bool bSave)		
{
	m_pData->SetStr(SYNDATA_NAME, szSynName, _MAX_NAMESIZE);
	if(bSave)
		m_pData->Update();
	else
		m_pData->ClearUpdateFlags();
	return true; 
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::ChangeSyndicate(/*const*/ SynFuncInfo0* pFuncInfo)
{
	bool ret = false;
	switch(pFuncInfo->ucFuncType)
	{
	case	SYNFUNC_DEMISE:
		{
			//[游途道标 2008/9/24]修正Demise新加参数后的调用错误.
			int nOldID = m_pData->GetInt(SYNDATA_LEADERID);
			ret = Demise(pFuncInfo->IntParam[0],
				pFuncInfo->IntParam[3],
				pFuncInfo->IntParam[1],
				(LPCTSTR)&pFuncInfo->IntParam[5],
				pFuncInfo->IntParam[2],
				pFuncInfo->IntParam[4],
				pFuncInfo->ucSaveFlag!=0,
				pFuncInfo->nSex,
				pFuncInfo->nProfession);
			pFuncInfo->IntParam[0] = nOldID;
			//pFuncInfo->IntParam[3] = m_pData->GetInt(SYNATTRDATA_MEMBERLEVEL);
			
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		}		
		break;
// 	case	SYNFUNC_SETTENET:
// 		ret = (SetTenet(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0)
// 				&& SetData(SYNDATA_PUBLISHTIME, pFuncInfo->dwData[0], pFuncInfo->ucSaveFlag!=0));
// 		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
// 		break;
	case	SYNFUNC_SETLEADERTITLE:
		ret = SetLeaderTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		break;
	case	SYNFUNC_SETMEMBERTITLE:
		ret = SetMemberTitle(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		break;
	case	SYNFUNC_ADDDATA:
		ret = AddData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		break;
	case	SYNFUNC_SETDATA:
		ret = SetData((SYNDATA)pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		break;
	case	SYNFUNC_PRESENT:
		ret = PresentMoney(pFuncInfo->IntParam[0], pFuncInfo->IntParam[1], pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		break;
	case	SYNFUNC_SETSYNNAME:
		ret = SetSynName(pFuncInfo->StrParam, pFuncInfo->ucSaveFlag!=0);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		break;
	case	SYNFUNC_ADD_MEMBER:
		{
			//[游途道标 2008/9/24]将查找玩家从用名字改为用玩家标识来查找,避免改名出现的错误
			//CPlayer *player = UserList()->GetPlayer((char*)&pFuncInfo->IntParam[1]);
			CPlayer *player = UserList()->GetPlayer(pFuncInfo->idUser);
			if(player != NULL)
			{
				//[游途道标 2008/9/24]在AddMember函数后新加两个参数
				//[游途道标 2008/9/25]增加对是否在线的登记
				ret = AddMember(player->GetID(),
						(char*)&pFuncInfo->IntParam[1], 
						pFuncInfo->nRank,
						player==NULL?0:player->m_nLevel,
						pFuncInfo->IntParam[0],
						pFuncInfo->nSex,
						pFuncInfo->nProfession,
						UserList()->IsLoginMapGroup(player->idSocket));
				m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
			}
			return ret;		// not broadcast
		}
		break;
	case	SYNFUNC_DEL_MEMBER:
		ret = DelMember(pFuncInfo->StrParam);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		return ret;		// not broadcast
		break;
	case	SYNFUNC_SET_RANK:
		//return SetMemberRank(pFuncInfo->StrParam, pFuncInfo->nRank);
// 		CPlayer *player = UserList()->GetPlayer(pFuncInfo->StrParam);
// 		if(player)
// 		{
// 
// 		}
		ret = SetMemberRank(pFuncInfo->StrParam, pFuncInfo->nRank);
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		//return ret;
		break;
//---------06.10.25王玉波添加-------begin----------------------------------------
		//某玩家升级了,军团中要更新他的信息
	case SYNFUNC_UP_LEVEL:
		//return SetMemberLevel( ( char* )&pFuncInfo->IntParam[ 1 ],pFuncInfo->IntParam[ 0 ] );
		ret = SetMemberLevel( ( char* )&pFuncInfo->IntParam[ 1 ],pFuncInfo->IntParam[ 0 ] );
		m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//20070405修罗:更新时间
		return ret;
		//直接return 不下传
		break;
//----------------------------------end------------------------------------------
		//20070414修罗:发工资请求
	case SYNFUNC_PAYOFF:
		{
			//钱将在下面函数扣除.并处理分发.不在线的玩家直接修改数据库
			ret = PayOff(pFuncInfo->IntParam[0], pFuncInfo->ucSaveFlag!=0);
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//更新时间
			//接下来向各地图组广播.回调到地图组时仅处理内存
		}
		break;
		//20070417修罗:成员贡献度汇报
	case SYNFUNC_SET_MEMBERPROFFER:
		{
			ret = SetMemberProffer(pFuncInfo->IntParam[0],pFuncInfo->IntParam[1],false);
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();//更新时间
			return ret;//不再回调.单向汇报
		}
		break;
	//[游途道标 2008/9/25]玩家上线记录
	case	SYNFUNC_LOGIN:
	//[游途道标 2008/9/25]玩家下线记录
	case	SYNFUNC_LOGOUT:
		{			
			for (int i = 0; i < m_setMember.size(); i++)
			{
				if (m_setMember[i].nId == pFuncInfo->idUser)
				{
					m_setMember[i].bOnline = pFuncInfo->bOnline;
					m_setMember[i].nOfflineTime = 0;
					if(!pFuncInfo->bOnline)
					{
						m_setMember[i].nOfflineTime = GetTickCount() /1000 + 100000000;
						WriteOfflineTime(pFuncInfo->idUser);						
					}
					break;
				}
			}
			if (m_setMember.size() == i)
			{
				break;
			}
			char szMsg[64] = { 0 };
			sprintf(szMsg, "%s %d", 
				m_setMember[i].szName.c_str(), 
				m_setMember[i].bOnline);
			/*char szTemp[128];
			if (m_setMember[i].bOnline)
			{
				sprintf(szTemp, "本帮成员[%s]上线了.", m_setMember[i].szName.c_str());
			}
			else
			{
				sprintf(szTemp, "本帮成员[%s]下线了.", m_setMember[i].szName.c_str());
			}*/
			for (int n = 0; n < m_setMember.size(); n++)
			{
				if (i != n && m_setMember[n].bOnline)
				{
					CPlayer* pUser = UserList()->GetPlayer(m_setMember[n].nId);
					if (pUser)
					{
						CMsgName	msg;
						if (msg.Create(NAMEACT_SETLINE, szMsg))
						{
							pUser->SendMsg(&msg);
						}
						

						/*CMsgTalk msg;
						if (msg.Create(_TXTATR_SYSTEM, szTemp))
						{
							pUser->SendMsg(&msg);
						}*/
					}
				}
			}
		}
		break;
	case SYNFUNC_LEAVERANK:
		{
			ret = true;
		}
		break;

	default:
	//	ASSERT(!"switch CSynShell::ChangeSyndicate()");
		::LogSave( "CSyndicateWorld::ChangeSyndicate case Error,pFuncInfo->ucFuncType:%d",pFuncInfo->ucFuncType );
		return false;
		break;
	}

	if(ret)
	{
		// broadcast
		MESSAGESTR	buf;
		SynFuncInfo0*	pInfo = (SynFuncInfo0*)buf;
		memcpy(buf, pFuncInfo, MAX_MESSAGESIZE);
		pInfo->ucSaveFlag	= (unsigned char)false;
		GameWorld()->QuerySynMessage()->ChangeSyndicate(pInfo);
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////
//[游途道标 2008/9/25]增加bOnline参数,记录在线人员
bool CSyndicateWorld::AddMember(int id,LPCTSTR szName, int nUserRank,int level,int proffer, int nSex, int nProfession, bool bOnline)
{
	CHECKF(szName);
	ASSERT(!DelMember(szName));

	//[游途道标 2008/9/24]修正后的直接写入方式
	//[游途道标 2008/9/25]增加对是否在线的记录
	m_setMember.push_back(MemberInfo(id,szName, nUserRank,level,proffer, nSex, nProfession, bOnline));
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::DelMember(LPCTSTR szName)
{
	CHECKF(szName);

	for(int i = m_setMember.size()-1; i >= 0; i--)		// delete, reverse traverse
	{
		if(strcmp(m_setMember[i].szName.c_str(), szName) == 0)
		{
			m_setMember.erase(m_setMember.begin() + i);
			return true;
		}
	}

	return false;
}
bool CSyndicateWorld::DelMember(int id)
{
	if(id == 0)
		return false;

	for(int i = m_setMember.size()-1; i >= 0; i--)		// delete, reverse traverse
	{
		if(m_setMember[i].nId == id)
		{
			m_setMember.erase(m_setMember.begin() + i);
			return true;
		}
	}

	return false;
}
/////////////////////////////////////////////////////////////////////
bool CSyndicateWorld::SetMemberRank(const char* pszName, int nRank)
{
	CHECKF (pszName);
	for (int i=0; i<m_setMember.size(); i++)
	{
		if (strcmp(m_setMember[i].szName.c_str(), pszName) == 0)
		{
			m_setMember[i].nRank = nRank;
			//20070405修罗:移动到上层CSyndicateWorld::ChangeSyndicate统一处理
			//m_dwTimeLastUpdateMemberInfo = ::GetTickCount();
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////
//06.4.21王玉波修改
enum {
		RANK_LEADER			= 100,			//军团团长		
		RANK_SUBLEADER		= 90,			//副团长
		RANK_TEAMLEADER		= 60,			//队长
		RANK_LEV5			= 50,			//五级团员
		RANK_LEV4			= 40,			//四级团员
		RANK_LEV3			= 30,			//三级团员
		RANK_LEV2			= 20,			//二级团员
		RANK_LEV1			= 10,			//一级团员		
		RANK_NONE			= 0,
	
	RANK_SUB		= RANK_LEADER - RANK_SUBLEADER,
};

int CSyndicateWorld::GetSynRankShow(int nRank)
{
	int nRankShow = nRank;
	if (nRankShow > RANK_LEV2)
	{
	}
	return nRankShow;
}

//////////////////////////////////////////////////////////////////////
/*
int CSyndicateWorld::AppendOnlineMember(class CMsgName& msg, int idx)
{
	int	nAmount = m_setMember.size();
	int nCount	= 0;
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if(bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMember[i].szName.c_str(), GetSynRankShow(m_setMember[i].nRank), bOnline);

			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}
	if (nCount < MEMBERLIST_SIZE)
	{
	}

	return nCount;
}
//////////////////////////////////////////////////////////////////////
int CSyndicateWorld::AppendOnlineMemberForAllMember(class CMsgName& msg, int idx)
{
	int	nAmount = m_setMemberTmp.size();
	int nCount	= 0;
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if(bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMemberTmp[i].szName.c_str(), GetSynRankShow(m_setMemberTmp[i].nRank), bOnline);

			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}

	return nCount;
}
//////////////////////////////////////////////////////////////////////
int CSyndicateWorld::AppendOfflineMember(class CMsgName& msg, int idx, int nCount)
{
	int	nAmount = m_setMember.size();
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if(!bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMember[i].szName.c_str(), GetSynRankShow(m_setMember[i].nRank), bOnline);
			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}
	return nCount;
}
//////////////////////////////////////////////////////////////////////
int CSyndicateWorld::AppendOfflineMemberForAllMember(class CMsgName& msg, int idx, int nCount)
{
	int	nAmount = m_setMemberTmp.size();
	for(int i = idx; i < nAmount && nCount < MEMBERLIST_SIZE; i++)
	{
		bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);
		if(!bOnline)
		{
			char	szMsg[256];
			sprintf(szMsg, "%s %d %d", m_setMemberTmp[i].szName.c_str(), GetSynRankShow(m_setMemberTmp[i].nRank), bOnline);
			// msg append
			CHECKF(msg.AppendString(szMsg, i));
			nCount++;
		}
	}
	return nCount;
}
*/

//////////////////////////////////////////////////////////////////////
//06.10.26王玉波修改
//参数说明:idxLast = 0,为第一次查询
void CSyndicateWorld::SendMemberList(CPlayer* pUser,DWORD dwTime )
{
	/*
	try
	{
		if( dwTime < 0 )		
			return ;
		if( 0 == dwTime )//第一次发送,得系统时间
		{
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();
		}
		else //检查服务端有没有更新过数据
		{
			if( m_dwTimeLastUpdateMemberInfo <= dwTime )
				return;
		}
		int nTotalAmount = GetMemberAmount();//得到军团人数数量总数
		int nSendTime = nTotalAmount / MEMBERLIST_SIZE;
		if( nTotalAmount % MEMBERLIST_SIZE )
			nSendTime += 1;								//发送次数计算
		for( int i = 0;i < nSendTime;i++ )
		{
			CMsgName	msg;
			CHECK( msg.CreateHead( NAMEACT_MEMBERLIST,i,m_dwTimeLastUpdateMemberInfo ) );
			
			int nCount = 0;
			AppendMember( msg,i * MEMBERLIST_SIZE,i );
			pUser->SendMsg( &msg );
		}
		
	}catch(...)
	{
		::LogSave( "catch CSyndicateWorld::SendMemberLis(),UserID=%d,dwTime=%d\n",pUser->GetID(),dwTime );
	}
	*/
	//-------彭峰---2007/05/23-----
	DEBUG_TRY
		if( dwTime < 0 )		
			return ;
		if( 0 == dwTime )//第一次发送,得系统时间
		{
			m_dwTimeLastUpdateMemberInfo = ::GetTickCount();
		}
		else //检查服务端有没有更新过数据
		{
			if( m_dwTimeLastUpdateMemberInfo <= dwTime )
				return;
		}
		int nTotalAmount = GetMemberAmount();//得到军团人数数量总数
		int nSendTime = nTotalAmount / MEMBERLIST_SIZE;
		if( nTotalAmount % MEMBERLIST_SIZE )
			nSendTime += 1;								//发送次数计算
		for( int i = 0;i < nSendTime;i++ )
		{
			CMsgName	msg;
			CHECK( msg.CreateHead( NAMEACT_MEMBERLIST,i,m_dwTimeLastUpdateMemberInfo ) );
			
			int nCount = 0;
			AppendMember( msg,i * MEMBERLIST_SIZE,i );
			pUser->SendMsg( &msg );
		}
	DEBUG_CATCH3("catch CSyndicateWorld::SendMemberLis(),UserID=%d,dwTime=%d\n",pUser->GetID(),dwTime)

}


//////////////////////////////////////////////////////////////////////
void CSyndicateWorld::CombineMemberList(const MEMBER_SET& setMember)
{
	if(setMember.size())
		m_setMember.insert(m_setMember.end(), setMember.begin(), setMember.end());
}

//////////////////////////////////////////////////////////////////////
CSyndicateWorld* CSyndicateWorld::GetMasterSyn()
{
	return this;//pSyn->GetMasterSyn();
}

//////////////////////////////////////////////////////////////////////
/*
void CSyndicateWorld::SendMemberListSpecifySyn(CPlayer *pUser, int idxLast)
{
	if(idxLast < -1 || idxLast >= static_cast<int>(m_setMember.size()))
		return ;

	// msg head
	CMsgName	msg;
	CHECK(msg.CreateHead(NAMEACT_MEMBERLIST, idxLast));

	bool	bOnline = true;		//? -1 for first query
	int nCount = 0;

	if(idxLast != -1)
		bOnline = (UserList()->GetPlayer(m_setMember[idxLast].szName.c_str()) != NULL);

	int idx	= idxLast + 1;
	if(bOnline)
	{
		nCount	= AppendOnlineMember(msg, idx);
		if(nCount < MEMBERLIST_SIZE)
			nCount = AppendOfflineMember(msg, 0, nCount);
	}
	else
	{
		nCount = AppendOfflineMember(msg, idx);
	}

	pUser->SendMsg(&msg);
}
*/
//06.10.26王玉波修改此函数
//参数说明:	int idxFirst,此次发送数据开始的位置
//			int idxTime,标识发送次数,第一次为0
void CSyndicateWorld::AppendMember(class CMsgName& msg,int idxFirst,int idxTime )
{
	int nSizeLimt = m_setMember.size();
	int nNewIndex;

	DWORD nPassTime = 0;
	int nOfflineTimeTemp = 0;

	for ( int i = 0;i < MEMBERLIST_SIZE;i++ )
	{
		nNewIndex = idxFirst + i;
		if( nNewIndex >= nSizeLimt )
			break;

		//离线时间初始计算 satan 2009.7.30
		//m_dwTimeFirstUpdateMemberOffline记录初始计算时间
		//nOfflineTimeTemp 记录初始时间数（初始化时候计算好） 1000000以上表示服务器开启过程中角色退出时的时间计数 
		//其他表示小时差
		//nPassData记录启动后最后一次退出到现在的小时差
		if(m_setMember[nNewIndex].bOnline)
		{
			nOfflineTimeTemp = 0;
		}
		else
		{
			nOfflineTimeTemp = m_setMember[nNewIndex].nOfflineTime;
			if(m_setMember[nNewIndex].nOfflineTime >= 100000000)
			{
				nOfflineTimeTemp = 0;
				nPassTime = GetTickCount()/1000 + 100000000 - m_setMember[nNewIndex].nOfflineTime;
			}
			else
			{
				nPassTime = GetTickCount()/1000 - m_dwTimeFirstUpdateMemberOffline;
			}
			nOfflineTimeTemp += nPassTime / (60 * 60 );
		}			
		
		

	//	bool	bOnline = (UserList()->GetPlayer(m_setMember[i].szName.c_str()) != NULL);//玩家是否在线，暂时不用
		//20070417修罗:加入个人贡献值
//		char	szMsg[ 64 ] = { 0 };//enough
//		sprintf(szMsg, "%s %d %d", m_setMember[ nNewIndex ].szName.c_str(), GetSynRankShow(m_setMember[ nNewIndex ].nRank), m_setMember[ nNewIndex ].nLevel);
				
		char	szMsg[ 128 ] = { 0 };//enough
		//[游途道标 2008/9/24]新增性别和职业传入客户端
		//[游途道标 2008/9/25]新增是否在线信息传入客户端
		sprintf(szMsg, "%s %d %d %d %d %d %d %d", 
			m_setMember[ nNewIndex ].szName.c_str(), 
			GetSynRankShow(m_setMember[ nNewIndex ].nRank),//职位 
			m_setMember[ nNewIndex ].nLevel,
			m_setMember[nNewIndex].nProffer,
			m_setMember[nNewIndex].nSex,//性别
			m_setMember[nNewIndex].nProfession,//职业
			m_setMember[nNewIndex].bOnline,//是否在线
			nOfflineTimeTemp);
		
		msg.AppendString( szMsg,idxTime );
	}

}

int CSyndicateWorld::GetMemberProffer(OBJID id)
{
	for(int i=0;i<m_setMember.size();i++)
	{
		if(m_setMember[i].nId == id)
			return m_setMember[i].nProffer;
	
	}
	return NULL;
}

const char* CSyndicateWorld::GetMemberName(OBJID id)
{
	for(int i=0;i<m_setMember.size();i++)
	{
		if(m_setMember[i].nId == id)
			return m_setMember[i].szName.c_str();
	
	}
	return NULL;
}

//-------------------------------------
BOOL CSyndicateWorld::PayOff(DWORD dwMoney,BOOL bSave)//20070414修罗:发工资.在内修改数据库.且处理成员分发
{
//	CHECKF(dwMoney>0                     //  [5/18/2007]陈剑飞添加,发工资要判断税金
//		&& m_pData->GetInt(SYNDATA_MONEY) > dwMoney*(100+_SYN_PAYOFF_TEX)/100);//在地图组时已经检查过此处.此处直接认为是错误
	
// 	m_pData->SetInt(SYNDATA_MONEY, 
// 		m_pData->GetInt(SYNDATA_MONEY)-dwMoney*(100+_SYN_PAYOFF_TEX)/100);//dec money

	//  [3/19/2008 朱斌]	0002937: 军团发工资问题。详见说明。 
	CHECKF(dwMoney > 0);
	__int64 i64OldMoney = m_pData->GetInt(SYNDATA_MONEY);
	__int64 i64NewMoney = (__int64)dwMoney * (100 + _SYN_PAYOFF_TEX) / (__int64)100;
	i64NewMoney = i64OldMoney - i64NewMoney;

	CHECKF(i64NewMoney > 0);

	m_pData->SetInt(SYNDATA_MONEY, i64NewMoney);

	if(bSave)
	{
		m_pData->Update();
	}
	else
	{
		m_pData->ClearUpdateFlags();
	}

	//预处理.获得1级贡献度团员所得-----------------------
	//遍历所有成员,通过贡献等级统计得到基值
	int memberProfferLevAmount[_SYN_USER_PROFFERLEV_MAX] = {0};//成员等级统计
	for(int i=0;i<m_setMember.size();i++)
	{
		int nProffer = m_setMember[i].nProffer;//贡献值
		for(int j=_SYN_USER_PROFFERLEV_MAX-1;j>=0;j--)
		{
			if(nProffer>=_SYN_USER_PROFFERNEED[j])
			{
				break;
			}
		}
		//j,即是贡献等级-1
		CHECKF(j>=0 && j<_SYN_USER_PROFFERLEV_MAX);
		
		memberProfferLevAmount[j]++;
	}
	//通过统计值计算基值
	int baseValuePercent = 0;//基值倍数
	for(i=0;i<_SYN_USER_PROFFERLEV_MAX;i++)
	{
		switch(i)
		{
		case 0://1级贡献等级总人数
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_1*memberProfferLevAmount[i];
			break;
		case 1://2级贡献等级总人数
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_2*memberProfferLevAmount[i];
			break;
		case 2://3级贡献等级总人数
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_3*memberProfferLevAmount[i];
			break;
		case 3://4级贡献等级总人数
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_4*memberProfferLevAmount[i];
			break;
		case 4://5级贡献等级总人数
			baseValuePercent += _SYN_USER_PAY_PROFFERLEV_5*memberProfferLevAmount[i];
			break;
		default://error!
			break;
		}
	}
	//基值
	int baseValue = (__int64)dwMoney*100/baseValuePercent;
	//预处理end----------------------------------------
	


	//接下来处理成员分发
	//遍历成员,在线时转发地图组加钱,不在线时直接在此修改数据库
	int realPayAmount = 0;//已经发到手的成员的个数.无实际功能.仅统计
	for(i=0;i<m_setMember.size();i++)
	{
		OBJID playerID = m_setMember[i].nId;//成员id

		int nProffer = m_setMember[i].nProffer;
		for(int j=_SYN_USER_PROFFERLEV_MAX-1;j>=0;j--)
		{
			if(nProffer>=_SYN_USER_PROFFERNEED[j])
			{
				break;
			}
		}
		//j,即是贡献等级-1
		CHECKF(j>=0 && j<_SYN_USER_PROFFERLEV_MAX);
		
		int profferLev = j;//贡献等级
		int userPay = 0;//个人所得
		switch(profferLev)
		{
		case 0:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_1/100;break;
		case 1:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_2/100;break;
		case 2:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_3/100;break;
		case 3:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_4/100;break;
		case 4:userPay=(__int64)baseValue*_SYN_USER_PAY_PROFFERLEV_5/100;break;
		default:break;
		}
		
		CPlayer* pPlayer = g_UserList.GetPlayer(playerID);
		if(pPlayer && g_UserList.IsLoginMapGroup(pPlayer->idSocket))//玩家在线
		{
			//借用remoteCall的机制
			REMOTE_CALL0 remoteCallInfo;
			remoteCallInfo.idUser=playerID;
			remoteCallInfo.ucFuncType=REMOTE_CALL_SYNPAYOFF;
			remoteCallInfo.IntParam[0]=userPay;//个人所得的钱

			IMessagePort* pMsgPort = GameWorld()->GetMsgPort();
			pMsgPort->Send(pPlayer->idProcess, 
				MAPGROUP_REMOTECALL, 
				BUFFER_TYPE(sizeof(remoteCallInfo)), 
				&remoteCallInfo);

			realPayAmount++;
		}
		else//不在线.直接修改数据库
		{
			char szSQL[512];
			sprintf(szSQL, "select money from %s where id=%d",
				_TBL_USER, playerID);
			IRecordset* pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
			CHECKF(pRes);			
			
			DWORD dwCurMoney = pRes->GetInt(0);
			DWORD dwNewMoney = dwCurMoney+userPay;//所得之后的钱
			const int nMaxMoney = 2000000000;
			if(dwNewMoney >= nMaxMoney)
				dwNewMoney = nMaxMoney;
	
			sprintf(szSQL, "update %s set money=%d where id=%d",
				_TBL_USER,dwNewMoney,playerID);
			CHECKF(GameWorld()->GetDatabase()->ExecuteSQL(szSQL));
			pRes->Release();
			pRes->Release();
			realPayAmount++;
		}
	}


	::GmLogSave("EVENT: 军团[%s]发工资.总数[%d]。", GetStr(SYNDATA_NAME), dwMoney);
	return true; 
}
//-------------------------------------------
BOOL CSyndicateWorld::SetMemberProffer(int dwProffer,OBJID idMember,BOOL bSave)//20070417修罗:成员贡献值
{
	for(int i=0;i<m_setMember.size();i++)
	{
		if(m_setMember[i].nId == idMember)
		{
			m_setMember[i].nProffer = dwProffer;
			return true;
		}
	}
	return false;
}

void CSyndicateWorld::WriteOfflineTime(OBJID userID)
{
	//离线时间写入数据库 satan 2009.7.30
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT last_exit FROM %s WHERE id=%u", _TBL_USER, userID);
	IRecordset* pStatRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
	if(pStatRes)
	{
		pStatRes->Release();
		char szTemp[64];
		SYSTEMTIME time;
		::GetLocalTime(&time);
		sprintf(szTemp,"%d-%d-%d-%d",time.wYear,time.wMonth,time.wDay,time.wHour);
		sprintf(szSQL, "UPDATE %s SET last_exit='%s' WHERE id=%u LIMIT 1", _TBL_USER,szTemp,userID);
		GameWorld()->GetDatabase()->ExecuteSQL(szSQL);
	}
}

// void CSyndicateWorld::SetSynAttackID(OBJID idAttack, bool bUpdate)
// {
// 	m_pData->SetInt(SYNDATA_ATTACKID, idAttack);
// 	if(bUpdate)
// 		m_pData->Update();
// }
// 
// OBJID CSyndicateWorld::GetSynAttackID()
// {
// 	return m_pData->GetInt(SYNDATA_ATTACKID);
// }
// 
// OBJID CSyndicateWorld::GetSynDefenceID()
// {
// 	return m_pData->GetInt(SYNDATA_DEFENCEID);
// }
// 
// void CSyndicateWorld::SetSynDefenceID(OBJID idDefence, bool bUpdate)
// {
// 	m_pData->SetInt(SYNDATA_DEFENCEID, idDefence);
// 	if(bUpdate)
// 		m_pData->Update();
// }

bool CSyndicateWorld::Update()
{
	return m_pData->Update();
}




