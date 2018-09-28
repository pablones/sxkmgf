// UserList.cpp: implementation of the CUserList class.
//
//////////////////////////////////////////////////////////////////////
#include "MessagePort.h"
#include "common.h"
#include "NetMsg.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "protocol.h"
#include "inifile.h"
#include "MsgFee_A.h"
using namespace world_kernel;
#include "AllMsg.h"
#include "supermanlist.h"
#include "UserList.h"
#include "MessageBoard.h"
#include "WorldKernel.h"
#include "StrRes.h"//20080307修罗:



const int	SECS_PER_GAMEHOUR		= 300;			// send CMsgTime to all user
const int	NEWBIE_ATTR_POINT		= 10;			// 新建角色属性点总和

//20070125修罗:属性系统调整--------------------------
const int	_NEWCREATE_WARRIOR_STR			= 18;		//新建 战士 力量
const int	_NEWCREATE_WARRIOR_DEX			= 17;		//新建 战士 敏捷
const int	_NEWCREATE_WARRIOR_VIT			= 10;		//新建 战士 体力
const int	_NEWCREATE_WARRIOR_SOUL			= 5;		//新建 战士 智力
const int	_NEWCREATE_WARRIOR_LOVELI		= 10;		//新建 战士 魅力

const int   _NEWCREATE_WARRIOR_MAX_ATTACK		= 6;			//新建 战士 武器 最大物理攻击
const int   _NEWCREATE_WARRIOR_MIN_ATTACK		= 4;			//新建 战士 武器 最小物理攻击
const int   _NEWCREATE_WARRIOR_MAX_MAGIC_ATTACK = 0	;		//新建 战士 武器 最大魔法攻击
const int   _NEWCREATE_WARRIOR_MIN_MAGIC_ATTACK = 0	;		//新建 战士 武器 最小魔法攻击

//-------
const int	_NEWCREATE_ARCHER_STR			= 12;		//新建 弓手 力量
const int	_NEWCREATE_ARCHER_DEX			= 18;		//新建 弓手 敏捷
const int	_NEWCREATE_ARCHER_VIT			= 10;		//新建 弓手 体力
const int	_NEWCREATE_ARCHER_SOUL			= 10;		//新建 弓手 智力
const int	_NEWCREATE_ARCHER_LOVELI		= 10;		//新建 弓手 魅力

const int   _NEWCREATE_ARCHER_MAX_ATTACK	= 5;			//新建 弓手 武器 最大物理攻击
const int   _NEWCREATE_ARCHER_MIN_ATTACK	= 3;			//新建 弓手 武器 最小物理攻击
const int   _NEWCREATE_ARCHER_MAX_MAGIC_ATTACK = 0;			//新建 弓手 武器 最大魔法攻击
const int   _NEWCREATE_ARCHER_MIN_MAGIC_ATTACK = 0;			//新建 弓手 武器 最小魔法攻击

//------
const int	_NEWCREATE_MAGE_STR				= 5;		//新建 法师 力量
const int	_NEWCREATE_MAGE_DEX				= 15;		//新建 法师 敏捷
const int	_NEWCREATE_MAGE_VIT				= 10;		//新建 法师 体力
const int	_NEWCREATE_MAGE_SOUL			= 20;		//新建 法师 智力
const int	_NEWCREATE_MAGE_LOVELI			= 10;		//新建 法师 魅力

const int   _NEWCREATE_MAGE_MAX_ATTACK		= 	2;		//新建 法师 武器 最大物理攻击
const int   _NEWCREATE_MAGE_MIN_ATTACK		=	1;		//新建 法师 武器 最小物理攻击
const int   _NEWCREATE_MAGE_MAX_MAGIC_ATTACK =	6;		//新建 法师 武器 最大魔法攻击
const int   _NEWCREATE_MAGE_MIN_MAGIC_ATTACK =	4;		//新建 法师 武器 最小魔法攻击


//属性系统调整-------//彭峰2007/02/02--------------------------------
//////////////////////////////////////////////////////////////////////
void CPlayer::OnTimer(time_t tCurr)
{
	return;
}

bool CPlayer::SendMsg(CNetMsg* pMsg)
{
	return GameWorld()->SendClientMsg(idSocket, pMsg);
}

bool CPlayer:: fetchTest(OBJID id,OBJID synid,int type)
{
	if(fetchID == id && fetchType == type && fetchSynID == synid)
	{
		return true;
	}
	return false;
}

OBJID CPlayer:: fetchGet(int typeGet,int type)
{
	if(fetchType == type)
	{
		if(typeGet == 1)
			return fetchID;
		else if(typeGet == 2)
			return fetchSynID;
	}
	return 0;
}

void CPlayer:: fetchSet(OBJID id,OBJID synid,int type)
{
	fetchID = id;
	fetchType = type;
	fetchSynID = synid;
}


MYHEAP_IMPLEMENTATION(CUserList,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserList::CUserList()
{
	m_nPlayerAmount		= 0;
	m_nMaxPlayerAmount	= 0;
	m_nHour				= 6;		// start server in 8 am.
}

//////////////////////////////////////////////////////////////////////
CUserList::~CUserList()
{

}

//////////////////////////////////////////////////////////////////////
bool CUserList::Create(ISocket* pSocket, IDatabase* pDb)
{
	CHECKF(pSocket);
	m_pSocket	= pSocket;

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserList::LoginUser(LPCTSTR szAccount, LPCTSTR szPassword, SOCKET_ID idSocket, ULONG nIP)	// 仅用于非帐号服务器启动
{
#ifdef	ACCOUNT_ENABLE
	return false;
#endif
	bool	bRet = false;
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT id, name, account_id, recordmap_id FROM %s WHERE account='%s' && password='%s'", _TBL_USER, szAccount, szPassword);
	IRecordset*	pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);		//VVVVVVVVVVVVVVVVVVVVVVVV
	if(pRes)
	{
		OBJID	idUser	= pRes->LoadDWord("id");
		NAMESTR	szName = "无";
		pRes->LoadString(szName, "name", _MAX_NAMESIZE);
		OBJID	idAccount	= pRes->LoadDWord("account_id");
		OBJID	idMap	= pRes->LoadDWord("recordmap_id");
		PROCESS_ID	idProcess	= MapList()->GetMapProcessID(idMap);
		ASSERT (idProcess != PROCESS_NONE);
		int		nLevel	= 0;		// 未帐号服务器登录，不需要等级
		CreateUser(idProcess, idUser, szName, idAccount, idSocket, nLevel, szAccount, nIP);

		bRet = true;
		pRes->Release();		//AAAAAAAAAAAAAAAAAAAAAAAAAA

//		GameWorld()->SendFee(idAccount, CMsgFee_A::FEE_BEGIN);
	}

	{
		m_nPlayerAmount++;
		if(m_nMaxPlayerAmount < m_nPlayerAmount)
			m_nMaxPlayerAmount = m_nPlayerAmount;
		extern struct STAT_STRUCT	g_stat;
		InterlockedExchange(&g_stat.nAllPlayers, m_nPlayerAmount);
		InterlockedExchange(&g_stat.nMaxPlayers, m_nMaxPlayerAmount);
		InterlockedIncrement(&g_stat.nLoginPlayers);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////
// enum { LOGIN_OK, LOGIN_NEW, LOGIN_BAN };
int CUserList::LoginUser(OBJID idAccount, SOCKET_ID idSocket, LPCTSTR name, ULONG nIP)
{
	assert(name!=NULL);

	int	nRet = -1;
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT id, recordmap_id, level FROM %s WHERE account_id='%u'and name = '%s'", _TBL_USER, idAccount,name);
	IRecordset*	pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);		
	if(pRes)
	{
		OBJID	idUser	= pRes->LoadDWord("id");
		NAMESTR	szName;
		strcpy(szName,name);
		OBJID	idMap	= pRes->LoadDWord("recordmap_id");
		PROCESS_ID	idProcess	= MapList()->GetMapProcessID(idMap);


		//2010.11.28。登陆进普通地图组   以后有功能需求再改动
		if(idProcess != MSGPORT_MAPGROUP_FIRST)
		{
			idProcess = MSGPORT_MAPGROUP_FIRST;
			pRes->SetDWord("recordmap_id",1001);
			pRes->SetDWord("recordx",78);
			pRes->SetDWord("recordy",87);
			pRes->UpdateRecord();
		}

		int		nLevel	= pRes->LoadInt("level");

		if(CreateUser(idProcess, idUser, szName, idAccount, idSocket, nLevel, "", nIP))
		{
			nRet = LOGIN_OK;
			//GameWorld()->SendFee(idAccount, CMsgFee_A::FEE_BEGIN);
			///角色登录后服务器向关系服务通知一下
			if(GameWorld()->IsRelationServerOpen())
				GameWorld()->SendRelationMsg(raRoleOnline,idUser,szName);
		//	GameWorld()->SendRelationMsg(raQueryRole_Friend,idUser,"",idUser);
		}
		 else
		{
			nRet = LOGIN_BAN;
		//	GameWorld()->SendFee(idAccount, CMsgFee_A::FEE_OFFLINE);
		}/**/
		pRes->Release();		//AAAAAAAAAAAAAAAAAAAAAAAAAA

	}

	return nRet;
}
//06.5.5王玉波修改以下函数
int CUserList::LoginUserSnap(OBJID idAccount, SOCKET_ID idSocket,bool bCreate/* = false*/)
{
	SQLBUF	szSQL;
	//防止玩家超过三个限制的问题查询语句后加入 ORDER BY id LIMIT 3
	sprintf(szSQL, "SELECT  id,sex,name, Fir_Job,Fir_JobLv,Sec_Job,Sec_JobLv,lookface,level FROM %s WHERE account_id='%u' LIMIT 4", _TBL_USER, idAccount);
	IRecordset*	pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);

	CMsgConnect msg;
	msg.Create( 0,0,"GetUserSnap");

	int nRoleCount = 0;

	DWORD firCoat,firWapon,firMount;
	DWORD secCoat,secWapon,secMount;
	int id,level,photoIndex,firJob,firLev,secJob,secLev;	
	char name[16];

	
// 	if(!pRes && bCreate)
// 	{
// 		char szName[16];
// 		sprintf(szName,"访客%u",idAccount);
// 		g_UserList.CreateNewPlayer(idSocket, szName , "youke",10, 1, idAccount);
// 		pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
// 	}

	if(pRes)
	{
		int nRoleCount = pRes->RecordCount();
		if( nRoleCount <= 8 )
		{

			for( int i = 0; i < pRes->RecordCount(); i++)
			{	
				id	= pRes->LoadInt("id");
				pRes->LoadString(name, "name", _MAX_NAMESIZE);
				level	= pRes->LoadInt("level");
				photoIndex	= pRes->LoadInt("lookface");

				firJob	= pRes->LoadInt("Fir_Job");
				firLev	= pRes->LoadInt("Fir_JobLv");
				secJob	= pRes->LoadInt("Sec_Job");
				secLev	= pRes->LoadInt("Sec_JobLv");
				secCoat = pRes->LoadInt("sex");

				//POSITION 2 为衣服 15为时装  4为武器 10为坐骑
				firCoat = 0;
				firWapon = 0;
				firMount = 0;

				secMount=0;
				secWapon=0;
				msg.Add(id,level,photoIndex,name,firJob,firLev,secJob,secLev,firCoat,firWapon,firMount,secCoat,secWapon,secMount);				
				pRes->MoveNext();
			}
		}
		pRes->Release();
	}
	GameWorld()->SendClientMsg(idSocket,&msg);
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserList::LogoutUser(SOCKET_ID idSocket)		// return false: no this user
{
	if(!( idSocket != SOCKET_NONE && idSocket < m_setUser.size() && m_setUser[idSocket] ))
	{
		//王玉波添加，此时应该在m_clienconnecttUser集合里
		GameWorld()->EndConnectUserBySocket( idSocket );
		return false;
	}
	// account server
	CPlayer* pPlayer = m_setUser[idSocket];
	CHECKF(pPlayer);
	GameWorld()->SendFee(pPlayer->m_idAccount, CMsgFee_A::FEE_END);
	GameWorld()->SendFee(pPlayer->m_idAccount, CMsgFee_A::FEE_KICKASS);
	if(GameWorld()->IsRelationServerOpen())
		GameWorld()->SendRelationMsg(raRoleOffline,pPlayer->idUser);
	int	tCurr = time(NULL);
//	if(tCurr > pPlayer->m_tPoint + POINTFEE_SECS)
//	{
//		LOGWARNING("玩家[%s][%u]退出时有[%d]个点没有成功上传!", 
//				pPlayer->szName, pPlayer->GetID(), (tCurr - pPlayer->m_tPoint) / POINTFEE_SECS);
//	}

	//给npcserver发送玩家下线通知
// 	CMsgInfoNpcUserExit msg;
// 	msg.Create(pPlayer->GetID());
// 	GameWorld()->SendNpcMsg(ID_NONE,&msg);
	// gm ?
	if (pPlayer->IsGM())
	{
		vector<OBJID>::iterator iter;
		for (iter = m_setGm.begin(); iter != m_setGm.end(); iter++)
		{
			if (pPlayer->GetID() == *iter)
			{
				m_setGm.erase(iter);
				break;
			}
		}
	}
	
	///softworms清掉名字索引
//	m_setUserName.erase(pPlayer->szName);

	{ // stat
		m_nPlayerAmount--;
		extern struct STAT_STRUCT	g_stat;
		InterlockedExchange(&g_stat.nAllPlayers, m_nPlayerAmount);
		InterlockedIncrement(&g_stat.nLogoutPlayers);
	}
	{
		SAFE_DELETE(m_setUser[idSocket]);
		m_setUser[idSocket] = NULL;
		return true;
	}
}

//////////////////////////////////////////////////////////////////////
bool CUserList::KickOutAccount(OBJID idAccount, LPCTSTR szText)		// return false: 不在线
{
	CHECKF(szText);

	CPlayer* pPlayer = GetPlayerByAccountID(idAccount);
	SOCKET_ID idSocket = SOCKET_NONE;
	if(!pPlayer)
	{
		//在userlist中未找到,在connectUser中找
		idSocket = GameWorld()->GetSocketFromConnectUser( idAccount );
		if( SOCKET_NONE == idSocket ) // closesocket无效
		{
			return GameWorld()->EndConnectUserByAccount( idAccount );
		}
#ifdef WYB_TEST
		LOGMSG( "在connect中找到idSocket:%d",idSocket );
		
#endif
	}
	else
	{
		idSocket = pPlayer->idSocket;
		CMsgTalk	msg;
		if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, (char*)szText, NULL, _COLOR_WHITE, _TXTATR_REGISTER))
			pPlayer->SendMsg(&msg);

		//zeng添加
//		LogoutUser(idSocket);
	}
	LOGDEBUG("DEBUG：世界核心踢出帐号[%u]，SOCKET_ID[%d]", idAccount,idSocket);
	if( idSocket != SOCKET_NONE )
	{
		return GameWorld()->GetSocketInterface()->CloseSocket( idSocket );
	}
	else
	{
		LOGERROR( "UserList与Connect中都未找到帐号[%d]",idAccount );
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
bool CUserList::KickOutSocket(SOCKET_ID idSocket)		// return false: 不在线
{
	LOGDEBUG("DEBUG：世界核心踢出SOCKET_ID[%d]", idSocket);
	return GameWorld()->GetSocketInterface()->CloseSocket(idSocket);
}

//////////////////////////////////////////////////////////////////////
void CUserList::OnTimer(time_t tCurr)
{
	for(int i = 0; i < m_setUser.size(); i++)
	{
		if(m_setUser[i])
			m_setUser[i]->OnTimer(tCurr);
	}
}

//////////////////////////////////////////////////////////////////////
void CUserList::SendTimeToSocket(SOCKET_ID idSocket)
{
/*	CMsgTime	msg;
	IF_OK(msg.Create(m_nHour))
		GameWorld()->SendClientMsg(idSocket, &msg);
*/
}

//////////////////////////////////////////////////////////////////////
void CUserList::LogoutAllUser()
{
//06.11.21王玉波实现此方法(如果服务器主动关闭情况下,有内存泄露(new CPlayer对象都没有释放))
	for( int i = 0;i < m_setUser.size();i++ )
	{
		LogoutUser( i );
	}
//-------end-----------------------
}

//////////////////////////////////////////////////////////////////////
bool CUserList::CreateUser(PROCESS_ID idProcess, OBJID idUser, LPCTSTR szName, OBJID idAccount, SOCKET_ID idSocket, int nLevel, LPCTSTR szNotify, ULONG nIP)
{
	if(idSocket >= m_setUser.size())
		m_setUser.resize(idSocket + 1, NULL);		// +1 : 保证有m_setUser[idSocket]这个成员

	IF_NOT(!m_setUser[idSocket])
	{
		LOGERROR("玩家使用相同SOCKET登录，新玩家[%s][%u]，SOCKET[%d]，旧玩家[%s][%d]",
					szName, idUser, idSocket, m_setUser[idSocket]->szName, m_setUser[idSocket]->idUser);
		// gm ?
		if (m_setUser[idSocket]->IsGM())
		{
			vector<OBJID>::iterator iter;
			for (iter = m_setGm.begin(); iter != m_setGm.end(); iter++)
			{
				if (m_setUser[idSocket]->GetID() == *iter)
				{
					m_setGm.erase(iter);
					break;
				}
			}
		} 
		KickOutSocket(idSocket);
		return false;
	}

	m_setUser[idSocket] = new CPlayer;
	if(!m_setUser[idSocket])
		return false;

	m_setUser[idSocket]->idUser		= idUser;
	SafeCopy(m_setUser[idSocket]->szName, szName, _MAX_NAMESIZE);
	m_setUser[idSocket]->idProcess	= idProcess;
	SafeCopy(m_setUser[idSocket]->szAccount, szNotify, _MAX_NAMESIZE);
	m_setUser[idSocket]->idSocket	= idSocket;
	m_setUser[idSocket]->m_idAccount	= idAccount;
	m_setUser[idSocket]->m_nLevel		= nLevel;
	SafeCopy(m_setUser[idSocket]->szNotify, szNotify, _MAX_NAMESIZE);

	m_setUser[idSocket]->m_tPoint	= time(NULL);
	m_setUser[idSocket]->m_nIP		= nIP;

	///放入名字索引中
	m_setUserName[szName]=idUser;

	// gm ?
	if (m_setUser[idSocket]->IsGM())
		m_setGm.push_back(m_setUser[idSocket]->GetID());

	{ // stat
		m_nPlayerAmount++;
		if(m_nMaxPlayerAmount < m_nPlayerAmount)
			m_nMaxPlayerAmount = m_nPlayerAmount;
		extern struct STAT_STRUCT	g_stat;
		InterlockedExchange(&g_stat.nAllPlayers, m_nPlayerAmount);
		InterlockedExchange(&g_stat.nMaxPlayers, m_nMaxPlayerAmount);
		InterlockedIncrement(&g_stat.nLoginPlayers);
	}
	return true;
}

///softworms 优化后的新版本
CPlayer* CUserList::GetPlayer(LPCTSTR szName)
{
	ASSERT(szName);
	CPlayer* pUser=NULL;
	OBJID idUser=m_setUserName[szName];
	if(idUser)
		pUser=GetPlayer(idUser);
	return pUser;
}


//////////////////////////////////////////////////////////////////////
CPlayer* CUserList::GetPlayer(OBJID idUser)
{
	for(int i = 0; i < m_setUser.size(); i++)
	{
		if(m_setUser[i])
		{
			if(m_setUser[i]->idUser == idUser)
				return m_setUser[i];
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CUserList::GetPlayerByAccount(LPCTSTR szAccount)
{
	for(int i = 0; i < m_setUser.size(); i++)
	{
		if(m_setUser[i])
		{
			if(strcmp(m_setUser[i]->szAccount, szAccount) == 0)
				return m_setUser[i];
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
CPlayer* CUserList::GetPlayerByAccountID(OBJID idAccount)
{
	for(int i = 0; i < m_setUser.size(); i++)
	{
		if(m_setUser[i])
		{
			if(m_setUser[i]->m_idAccount == idAccount)
				return m_setUser[i];
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
//06.5.5王玉波修改..
bool CUserList::CreateNewPlayer(SOCKET_ID idSocket/*, LPCTSTR szAccount*/, LPCTSTR szName, LPCTSTR szPassword, 
						int unLook, int nData, OBJID idAccount/*, char cLength, char cFat, int nSex*/)
{
	//20070615修罗:验证角色名的合法性--------------
	SQLBUF szSQL={0};
	sprintf(szSQL,"SELECT * FROM %s WHERE NAME ='%s'",
			_TBL_USER,szName);
	IRecordset* pResTemp = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
	if(pResTemp)
	{
		//重复用户名
		CMsgTalk	msg;
		if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_DUPLICATE_NAME, NULL, _COLOR_WHITE, _TXTATR_REGISTER))
			GameWorld()->SendClientMsg(idSocket, &msg);
		pResTemp->Release();
		return false;
	}
	//---------------------------------------------


	// 缺省的登录点地图
	//int idMap = 1001;	
	int idMap = 0;//goto 在第一次登陆时根据新手城人数分配出生地图，这里不填地图
	int rebornX =79, rebornY = 79;
	int nChoosePos = RandGet(12);
	switch(nChoosePos)
	{
	case 0:
		rebornX =40;
		rebornY =112;
		break;
	case 1:
		rebornX =41;
		rebornY =115;
		break;
	case 2:
		rebornX =44;
		rebornY =115;
		break;
	case 3:
		rebornX =44;
		rebornY =112;
		break;
	case 4:
		rebornX =42;
		rebornY =113;
		break;
	case 5:
		rebornX =42;
		rebornY =111;
		break;
	case 6:
		rebornX =50;
		rebornY =123;
		break;
	case 7:
		rebornX =51;
		rebornY =125;
		break;
	case 8:
		rebornX =52;
		rebornY =123;
		break;
	case 9:
		rebornX =53;
		rebornY =121;
		break;
	case 10:
		rebornX =54;
		rebornY =124;
		break;
	case 11:
		rebornX =51;
		rebornY =121;
		break;
	}


	CMapSt* pMap = MapList()->GetMap(idMap);
// 	if(!pMap)
// 		return false;

	

	//基本属性
	int nStr	= 130;				
	int nInt	= 130;
	int nSta	= 130;
	int nSpi	= 130;
	int nAgi	= 130;
	int nPot	= 0;

	int nHp = 200;	//生命
	int nMp = 140;	//魔法
	int nSp = 0;	

	//钱
	int nDefaultMoney = 1000;

	//性别,职业
	int cSex = 0;
	int nProfession = 4096;
	if(nData == 1)
	{
		cSex = 1;
	}

	//检查是否已经到人物数量上限
	sprintf(szSQL, "SELECT  name FROM %s WHERE account_id='%u'", _TBL_USER, idAccount);
	IRecordset*	pRes = GameWorld()->GetDatabase()->CreateNewRecordset(szSQL);
	int nRoleCount = 0;////彭峰2007/01/29得到帐号人物个数
	if(pRes!=NULL )
	{
		nRoleCount = pRes->RecordCount();
		if( nRoleCount >= 1 )
		{
			CMsgTalk	msg2;
			if(msg2.Create(SYSTEM_NAME, ALLUSERS_NAME, "无法继续创建角色!", NULL, _COLOR_WHITE, _TXTATR_REGISTER))
			GameWorld()->SendClientMsg(idSocket, &msg2);

			pRes->Release();
			return false;
		}
		pRes->Release();
	}
	int nRad = ::RandGet(600);
	char szFeel[32];//最多7个汉字
	if (nRad<100)
		sprintf(szFeel,"浮沉江湖御剑情");
	else if (nRad<200)
		sprintf(szFeel,"有我才有真江湖");
	else if (nRad<300)
		sprintf(szFeel,"一人一剑江湖游");
	else if (nRad<400)
		sprintf(szFeel,"御剑至尊");
	else if (nRad<500)
		sprintf(szFeel,"御剑穹苍真好玩");
	else
		sprintf(szFeel,"感觉还不错");
	//记录创建人物的时间
	SYSTEMTIME ct;
	unsigned long date;
	GetLocalTime(&ct);
	date = (ct.wYear - 2000)*100000000 + ct.wMonth*1000000 + ct.wDay * 10000 + ct.wHour * 100 + ct.wMinute; 
	sprintf(szSQL, "INSERT INTO %s SET account_id=%u,creat_time=%u,Exit_Time=%u,name='%s',lookface=%d,sex =%d,protect_passwd='%s',Fir_Hp=%d,Fir_Mp=%d,Fir_Sp=%d,Gold2=%u,"
			       "recordmap_id=%u, recordx=%u, recordy=%u,Fir_Job=%d,Fir_Str=%d,Fir_Int=%d,Fir_Sta=%d,Fir_Spi=%d,Fir_Agi = %d,"
			       "Fir_Pot=%d,Info_Des='%s',Feel='%s'",
				   _TBL_USER,
				   idAccount,date,date, szName, unLook, cSex,szPassword,nHp,nMp,nSp,nDefaultMoney,
				   idMap,rebornX , rebornY,nProfession,nStr, nInt, nSta, nSpi,nAgi, 
				   nPot,g_objStrRes.GetStr(19055),szFeel);//这里填写您的自我介绍

	
	if(!GameWorld()->GetDatabase()->ExecuteSQL(szSQL))		// 不检查重名和重帐号，由UNI_KEY保证
	{
		CMsgTalk	msg;
		if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_DUPLICATE_NAME, NULL, _COLOR_WHITE, _TXTATR_REGISTER))
			GameWorld()->SendClientMsg(idSocket, &msg);
		return false;
	}

	OBJID idUser	= GameWorld()->GetDatabase()->GetInsertId();

	//武器
	sprintf(szSQL, "INSERT INTO %s SET TYPE=%u,owner_id=%u,player_id=%u,amount=%u,amount_limit=%u,quality=%u,star=%u,addlev=%u,kidnap=1,"
		"itemBase1=%u,itemBase2=%u,POSITION=%u",_TBL_ITEM,141000,idUser,idUser,10000,10000,0,0,0,10021,20021,4);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);

	//江湖指南
	sprintf(szSQL, "INSERT INTO %s SET TYPE=%u,owner_id=%u,player_id=%u,amount=%u,amount_limit=%u,quality=%u,star=%u,addlev=%u,kidnap=1,"
		"itemBase1=%u,itemBase2=%u,POSITION=%u",_TBL_ITEM,501002,idUser,idUser,1,1,0,0,0,0,0,47);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);

	//小礼包
	sprintf(szSQL, "INSERT INTO %s SET TYPE=%u,owner_id=%u,player_id=%u,amount=%u,amount_limit=%u,quality=%u,star=%u,addlev=%u,kidnap=1,color=3,"
		"itemBase1=%u,itemBase2=%u,POSITION=%u",_TBL_ITEM,500000,idUser,idUser,1,1,0,0,0,0,0,47);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);

	//大礼包
	sprintf(szSQL, "INSERT INTO %s SET TYPE=%u,owner_id=%u,player_id=%u,amount=%u,amount_limit=%u,quality=%u,star=%u,addlev=%u,kidnap=1,color=4,"
		"itemBase1=%u,itemBase2=%u,POSITION=%u",_TBL_ITEM,500100,idUser,idUser,1,1,0,0,0,0,0,47);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);

	//小飞鞋
	sprintf(szSQL, "INSERT INTO %s SET TYPE=%u,owner_id=%u,player_id=%u,amount=%u,amount_limit=%u,quality=%u,star=%u,addlev=%u,kidnap=1,color=3,"
		"itemBase1=%u,itemBase2=%u,POSITION=%u",_TBL_ITEM,630006,idUser,idUser,10,100,1,0,0,0,0,48);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);
	//HP
	sprintf(szSQL, "INSERT INTO %s SET TYPE=%u,owner_id=%u,player_id=%u,amount=%u,amount_limit=%u,quality=%u,star=%u,addlev=%u,kidnap=1,"
		"itemBase1=%u,itemBase2=%u,POSITION=%u",_TBL_ITEM,300001,idUser,idUser,10,20,0,0,0,0,0,47);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);

	//MP
	sprintf(szSQL, "INSERT INTO %s SET TYPE=%u,owner_id=%u,player_id=%u,amount=%u,amount_limit=%u,quality=%u,star=%u,addlev=%u,kidnap=1,"
		"itemBase1=%u,itemBase2=%u,POSITION=%u",_TBL_ITEM,310001,idUser,idUser,10,20,0,0,0,0,0,47);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);

	//任务
	sprintf(szSQL, "INSERT INTO %s SET userid=%u,taskid=%u,taskphase=%u,isRec=%u",_TBL_TASKDETAIL,idUser,1,1,1);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);


	//快捷键
	sprintf(szSQL, "INSERT INTO %s SET userid=%u, key1=%u, key2=%u, key3=%u, key7=%u, key8=%u",
		_TBL_USERKEY, idUser, 95090, 3000011, 3100011, 95080, 90000);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);

	//挂机
	sprintf(szSQL, "INSERT INTO %s SET userid=%u",
		_TBL_AUTOFIGHTSET, idUser);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);
//modify code by 林德渊 begin 2011.5.30-//充值元宝等级记录
	sprintf(szSQL, "INSERT INTO %s SET ACCOUNT_ID=%u,OBJ_ID=%u",_TBL_VASLV, idAccount,idUser);
	GameWorld()->GetDatabase()->ExecuteSQL(szSQL);
//modify code by 林德渊 end


	//给新人的东西可以放在脚本处理
	CMsgTalk	msg2;
	if(msg2.Create(SYSTEM_NAME, ALLUSERS_NAME, REPLAY_OK_STR, NULL, _COLOR_WHITE, _TXTATR_REGISTER))
		GameWorld()->SendClientMsg(idSocket, &msg2);

	CMsgRegister msg3;
	if (msg3.Create(szName, "", 1, unLook, cSex))
	{
		GameWorld()->BroadcastRegisterMsg(_MSG_REGISTER, msg3.GetBuf(), msg3.GetSize(), idSocket);
		GameWorld()->SendRelationRelayMsg(_MSG_REGISTER, msg3.GetBuf(), msg3.GetSize());
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
void CUserList::ChangeProcessID(SOCKET_ID idSocket, PROCESS_ID idProcess)
{
	CHECK(idProcess >= MSGPORT_MAPGROUP_FIRST && idProcess < GameWorld()->GetMsgPort()->GetSize());
	CHECK(idSocket >= 0 && idSocket < m_setUser.size());

	m_setUser[idSocket]->idProcess = idProcess;
}

//////////////////////////////////////////////////////////////////////
void CUserList::LoginToMapGroup(CPlayer* pPlayer)
{
	CHECK(pPlayer);

	// 转到地图组中进行登录。
	IMessagePort* pPort = GameWorld()->GetMsgPort();
	ST_LOGIN	cLogin;
	cLogin.idSocket	= pPlayer->idSocket;
	cLogin.idUser	= pPlayer->GetID();
	cLogin.nIP		= pPlayer->m_nIP;
	PROCESS_ID	idMapGroupProcess = pPlayer->GetProcessID();
	pPort->Send(idMapGroupProcess, MAPGROUP_LOGIN, STRUCT_TYPE(ST_LOGIN), &cLogin);
}

CPlayer* CUserList::GetRandomGM()
{
	int nGMCount=m_setGm.size();
	if (m_setGm.size())
	{
		nGMCount=rand()%nGMCount;
		return GetPlayer(m_setGm[nGMCount]); 
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
void CUserList::BroadcastMsg(CNetMsg* pMsg, CPlayer* pExcludeSender/*=NULL*/)
{
	CHECK(pMsg);

	for(int i = 0; i < m_setUser.size(); i++)
	{
		CPlayer* pPlayer = m_setUser[i];
		if(pPlayer && (!pExcludeSender || pExcludeSender->GetID() != pPlayer->GetID()))
			pPlayer->SendMsg(pMsg);
	}
}

//////////////////////////////////////////////////////////////////////
void CUserList::BroadcastMsg(LPCTSTR szMsg, CPlayer* pExcludeSender/*=NULL*/, LPCTSTR pszEmotion, DWORD dwColor, int nAttribute)
{
	CHECK(szMsg);

	CMsgTalk	msg;
	if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, (char*)szMsg, (char*)pszEmotion, dwColor, nAttribute))
		BroadcastMsg(&msg, pExcludeSender);
}



