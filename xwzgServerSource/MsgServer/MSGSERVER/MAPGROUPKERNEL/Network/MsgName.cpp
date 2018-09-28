// MsgName.cpp: implementation of the CMsgName class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgName.h"
#include "MsgEudemonInfo.h"
#include "MsgItemInfo.h"
#include "mapgroup.h"
#include "WantedList.h"
#include "PoliceWanted.h"
//#include "BetOutside.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgName::CMsgName()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgName::~CMsgName()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_NAME != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::Create(int nType, const char* pszName, __int64 dwData /*= 0*/)
{
	// param check
	if (nType == NAMEACT_NONE || !pszName)
		return false;

	// init
	this->Init();

	// fill info now

	m_pInfo->ucType	= nType;
	m_pInfo->dwData	= dwData;
	m_pInfo->dwHData = dwData >> 32;

	m_StrPacker.AddString(pszName);

	m_unMsgType	=_MSG_NAME;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgName::Create(int nType, const char* pszName, USHORT usPosX, USHORT usPosY)
{
	// param check
	if (nType == NAMEACT_NONE || !pszName)
		return false;
	// init
	this->Init();
	// fill info now
	m_pInfo->ucType	= nType;
	m_pInfo->usPosX	= usPosX;
	m_pInfo->usPosY	= usPosY;

	m_StrPacker.AddString(pszName);

	m_unMsgType	=_MSG_NAME;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return true;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgName::	AppendStr(const char* pszName)
{
	CHECKF(pszName);

	m_StrPacker.AddString(pszName);
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgName::Process(void *pInfo)
{
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;

	char szName[_MAX_WORDSSIZE];
	m_StrPacker.GetString(0, szName, sizeof(szName));

	switch(m_pInfo->ucType)
	{
// 	case	NAMEACT_QUERY_WANTED:
// 		{
// 			CWantedData* pWanted = CWantedList::s_WantedList.GetWanted(pUser->WantedInfo().idWanted);
// 			if (pWanted)
// 			{
// 				CUser* pTarget = UserManager()->GetUser(pWanted->GetStr(DATA_TARGET_NAME));
// 				if (pTarget && pUser->FindAroundUser(pTarget->GetID()))
// 					pUser->SendSysMsg(_TXTATR_NORMAL,g_objStrRes.GetStr(11033) , pTarget->GetName());//STR_FOND_WANTED
// 			}
// 		}
// 		break;
// 
// 	case	NAMEACT_QUERY_POLICEWANTED:
// 		{
// 			OBJID idWanted = pUser->PoliceWantedID();
// 			PoliceWantedStruct* pInfo = PoliceWanted().GetWanted(idWanted);
// 
// 			if (pUser->FindAroundUser(idWanted) && pInfo)
// 				pUser->SendSysMsg(_TXTATR_NORMAL, g_objStrRes.GetStr(11034), pInfo->strName.c_str());//STR_FOND_POLICEWANTED
// 		}
// 		break;

	case	NAMEACT_FIREWORKS:
		{
			pUser->BroadcastRoomMsg(this, EXCLUDE_SELF);
		}
		break;
	case	NAMEACT_CREATE_SYN: 				// 改到TALK中
		{
			ASSERT(!"NAMEACT_CREATE_SYN");
		}
		break;
	case	NAMEACT_SYNDICATE: 				// 无法实现修改帮派名，仅下传
		{
			ASSERT(!"NAMEACT_SYNDICATE");
		}
		break;
	case	NAMEACT_CHANGE_SYNTITLE: 
		{
			ASSERT(!"NAMEACT_CHANGE_SYNTITLE");
		}
		break;
	case	NAMEACT_DELROLE:						// 无意义
		{
			ASSERT(!"NAMEACT_DELROLE");
		}
		break;
	case	NAMEACT_MATE:
		{
			ASSERT(!"NAMEACT_MATE");
		}
		break;
	case	NAMEACT_QUERY_NPC:
		{
			OBJID idNpc = m_pInfo->idTarget;
			if(idNpc == ID_NONE)
				return ;
			CNpc* pNpc;
			IRole* pRole = RoleManager()->QuerySet()->GetObj(idNpc);
			if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
			{
				Create(NAMEACT_QUERY_NPC, pNpc->GetName(), idNpc);
				pUser->SendMsg(this);
			}
		}
		break;
	case NAMEACT_MEMBERLIST_SPECIFYSYN:
		{
 			CSyndicate * pSyn = SynManager()->QuerySynByName(szName);
			CHECK(pSyn);
			OBJID idSyn = pSyn->GetID();
			CHECK(idSyn != ID_NONE);
			SetTransData(idSyn);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
		}
		break;
	case	NAMEACT_MEMBERLIST:
		{
			OBJID idSyn = pUser->GetSynID();
			CHECK(idSyn != ID_NONE);
			SetTransData(idSyn);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
		}
		break;
	case	NAMEACT_QUERY_MATE:
		{
			CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
			if(pTarget)
			{
				IF_OK(Create(NAMEACT_QUERY_MATE, /*pTarget->GetMate()*/"111", m_pInfo->idTarget))
					SendMsg(this);
			}
		}
		break;
// 	case	NAMEACT_ADDDICE_PLAYER:
// 		{
// 			IRole* pRole = pUser->FindAroundRole(m_pInfo->idTarget);
// 			CNpc* pNpc;
// 			if(pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsBetNpc())
// 			{
// 				pNpc->QueryBet()->JoinBet(pUser);
// 			}
// 		}
// 		break;
// 	case	NAMEACT_DELDICE_PLAYER:
// 		{
// 			CMsgName	msg;
// 			IF_OK(msg.Create(NAMEACT_DELDICE_PLAYER, pUser->GetName(), m_pInfo->idTarget))
// 				pUser->SendMsg(&msg);
// 
// 			IRole* pRole = RoleManager()->QueryRole(m_pInfo->idTarget);
// 			CNpc* pNpc;
// 			if(pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsBetNpc())
// 			{
// 				pNpc->QueryBet()->LeaveBet(pUser);
// 			}
// 		}
// 		break;
	case NAMEACT_CHANGE_EUDEMON_NAME://幻兽改名,去掉..
		{

		}
		break;
		//更改自我介绍...
	case NAMEACT_CHANGE_DESC:
		{
			char szDescBuffer[ 128 ] = { 0 }; 
			m_StrPacker.GetString( 1,szDescBuffer,sizeof( szDescBuffer ) );	
			pUser->ChangeMyDesc( szDescBuffer );
		}
		break;
// 	case NAMEACT_CHANGE_QQ:       //qq
// 		{
// 			char szQqBuffer[ 128 ] = { 0 }; 
// 			m_StrPacker.GetString( 1,szQqBuffer,sizeof( szQqBuffer ) );	
// 			pUser->setQq( szQqBuffer );
// 		}
// 		break;
//     case NAMEACT_CHANGE_PROV:     //省
// 		{
// 			char szProvBuffer[ 128 ] = { 0 }; 
// 			m_StrPacker.GetString( 1,szProvBuffer,sizeof( szProvBuffer ) );	
// 			pUser->SetProv( szProvBuffer );	
// 		}
// 		break;
// 	case NAMEACT_CHANGE_CITY:     //市
// 		{
// 			char szCityBuffer[ 128 ] = { 0 }; 
// 			m_StrPacker.GetString( 1,szCityBuffer,sizeof( szCityBuffer ) );	
// 			pUser->SetCity( szCityBuffer );
// 		}
// 		break;
//     case NAMEACT_CHANGE_OLD:      //年龄
// 		{
// 			char szOldBuffer[ 128 ] = { 0 }; 
// 			m_StrPacker.GetString( 1,szOldBuffer,sizeof( szOldBuffer ) );	
// 			pUser->SetOld( szOldBuffer );
// 		}
// 		break;
// 	case NAMEACT_CHANGE_STAR:     //星座
// 		{
// 			char szStarBuffer[ 128 ] = { 0 }; 
// 			m_StrPacker.GetString( 1,szStarBuffer,sizeof( szStarBuffer ) );	
// 			pUser->SetStar( szStarBuffer );
// 		}
// 		break;
// 	case NAMEACT_CHANGE_BTSEX:   //性变态
// 		{
// 			char szBtSexBuffer[ 128 ] = { 0 }; 
// 			m_StrPacker.GetString( 1,szBtSexBuffer,sizeof( szBtSexBuffer ) );	
// 			pUser->SetBtSex( szBtSexBuffer );
// 		}
// 		break;
	case NAMEACT_REBORN_ACCEPT://同意复活
		{
			pUser->ProcessRebornMe();
		}
		break;
// 	case NAMEACT_CHANGE_EXPSCALE://更改武器经验比例请求
// 		{
// 			//如果钱不够5万,retun,不提示了，客户端有提示
// 			//	20070714	朱斌 调整为10000
// 			//	int nNeedMoney = 50000;
// 			int nNeedMoney = 10000;
// 			DWORD dwMoney = pUser->GetMoney();
// 			if( dwMoney < nNeedMoney )
// 				return;
// 			if( pUser->SpendMoney( nNeedMoney,true ) )
// 			{
// 				//成功
// 				pUser->SetExpScale( m_pInfo->dwData );
// 				CMsgName msg;
// 				if( msg.Create( NAMEACT_CHANGE_EXPSCALEOK,"111",m_pInfo->dwData ) )
// 					pUser->SendMsg( &msg );
// 
// 			}
// 		}
// 		break;
// 	case NAMEACT_CHANGE_WPSLEXPSCALE://更改武器经验比例请求
// 		{
// 			//如果钱不够5万,retun,不提示了，客户端有提示
// 			//	20070714	朱斌 调整为10000
// 			//	int nNeedMoney = 50000;
// 			int nNeedMoney = 10000;
// 			DWORD dwMoney = pUser->GetMoney();
// 			if( dwMoney < nNeedMoney )
// 				return;
// 			if( pUser->SpendMoney( nNeedMoney,true ) )
// 			{
// 				//成功
// 				pUser->SetWeaponSoulExpScale( m_pInfo->dwData );
// 				CMsgName msg;
// 				if( msg.Create( NAMEACT_CHANGE_WPSLEXPSCALEOK,"111",m_pInfo->dwData ) )
// 					pUser->SendMsg( &msg );
// 
// 			}
// 		}
// 		break;
	case NAMEACT_CHANGE_TITLE://[游途道标 2008/10/13]更改称号
		{
// 			if (m_pInfo->dwData)
// 			{
// 				_int64 i64HoldTitle = pUser->GetHoldTitle();
// 				int iKind = m_pInfo->dwData / 100;
// 				int iLevel = m_pInfo->dwData % 100;
// 				i64HoldTitle = i64HoldTitle >> (_MAX(iKind - 1, 0) * 4);
// 				i64HoldTitle &= 0x000000000000000F;
// 				if (i64HoldTitle < iLevel)
// 				{
// 					return;
// 				}
// 			}
// 			pUser->SetUseTitle(m_pInfo->dwData);
// 			
// 			CMsgName msg;
// 			if( msg.Create(NAMEACT_CHANGE_TITLEOK, pUser->GetName(), m_pInfo->dwData))
// 			{
// 				pUser->SendMsg( &msg );
// 			}
		}
		break;
	case NAMEACT_CHANGE_RECORDKEY:   //系统快捷键的保存
		{

		}
		break;
	case NAMEACT_HANDUP_BUG:
		{
			char Param[1024]={0};
			char szName[128]={0};
			char szDes[512] ={0};
			char szAction[16] ={0};
			
			m_StrPacker.GetString( 1,Param,1024 );
			sscanf(Param,"%s %s %s",szDes,szName,szAction);
			if(szName[0] !='\0' && szDes[0] != '\0')
				::MsgLogSave("Antilog/handupbug", "user[%s],account[%u],handupPlayer[%s],action[%s],descript[%s]",
				pUser->GetName(),pUser->GetAccountID(),szName,szAction,szDes); 
			//角色名 账号名 举报玩家名字 行为 描述
		}
		break;
	}
}
