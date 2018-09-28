// MsgTalk.cpp: implementation of the CMsgTalk class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#ifdef	WORLD_KERNEL
#include "userlist.h"
#else
#pragma warning(disable:4786)
#include "mapgroup.h"
#endif

#include "SKPostOffice.h"
#include "user.h"
//#ifdef _DEBUG
//#define _GMTOOLDEBUG
//#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD WorldMsgTime = 0;


CMsgTalk::CMsgTalk()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgTalk::~CMsgTalk()
{

}

////////////////////////////////////////////////////////////////////
BOOL CMsgTalk::Create(LPCTSTR pszSpeaker, LPCTSTR pszHearer, LPCTSTR pszWords, 
					  LPCTSTR pszEmotion, DWORD dwColor, unsigned short unAttribute,
					  unsigned short unStyle,DWORD dwItemID,DWORD dwItemTypeID,LPCTSTR pszItemName,DWORD dwUseGMToolPsw)
{
	if(!pszSpeaker || strlen(pszSpeaker) >= _MAX_NAMESIZE)
		return false;

	if(!pszHearer || strlen(pszHearer) >= _MAX_NAMESIZE)
		return false;

	if(!pszWords || strlen(pszWords) >= _MAX_WORDSSIZE)
		return false;

	// can be null
	if(pszEmotion && strlen(pszEmotion) >= _MAX_NAMESIZE)	
		return false;

	// fill
	this->Init();

	// fill structure
	m_pInfo->dwWordsColor	=dwColor;
	m_pInfo->unTxtAttribute	=unAttribute;
	m_pInfo->unTxtStyle		=unStyle;
	m_pInfo->dwTime			=::SysTimeGet();
	m_pInfo->dwItemID		=dwItemID;
	m_pInfo->dwItemTypeID	=dwItemTypeID;
	m_pInfo->dwUseGMToolPsw	=dwUseGMToolPsw;

	BOOL bSucMake	=true;
	bSucMake	&=m_StrPacker.AddString(pszSpeaker);
	bSucMake	&=m_StrPacker.AddString(pszHearer);
	bSucMake	&=m_StrPacker.AddString(pszEmotion);
	bSucMake	&=m_StrPacker.AddString(pszWords);
	bSucMake	&=m_StrPacker.AddString(pszItemName);

	m_unMsgType	=_MSG_TALK;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return bSucMake;
}
////////////////////////////////////////////////////////////////////
//[2010-06-29 goto]重载此函数,好友留言时发送心情到客户端
BOOL CMsgTalk::Create(LPCTSTR pszSpeaker, LPCTSTR pszHearer, LPCTSTR pszWords, 
					  LPCTSTR pszEmotion, DWORD dwColor, unsigned short unAttribute,
					  unsigned short unStyle,DWORD dwItemID,DWORD dwItemTypeID,LPCTSTR pszItemName,DWORD dwUseGMToolPsw,LPCTSTR szFeel)
{
	if(!pszSpeaker || strlen(pszSpeaker) >= _MAX_NAMESIZE)
		return false;
	
	if(!pszHearer || strlen(pszHearer) >= _MAX_NAMESIZE)
		return false;
	
	if(!pszWords || strlen(pszWords) >= _MAX_WORDSSIZE)
		return false;
	
	// can be null
	if(pszEmotion && strlen(pszEmotion) >= _MAX_NAMESIZE)	
		return false;
	if (strlen(szFeel) > _MAX_FEELSIZE)
		return false;
	char szSenderFeel[_MAX_FEELSIZE] = "无";
	if(pszWords || strlen(szFeel) > 0)
		sprintf(szSenderFeel ,szFeel);
	
	// fill
	this->Init();
	
	// fill structure
	m_pInfo->dwWordsColor	=dwColor;
	m_pInfo->unTxtAttribute	=unAttribute;
	m_pInfo->unTxtStyle		=unStyle;
	m_pInfo->dwTime			=::SysTimeGet();
	m_pInfo->dwItemID		=dwItemID;
	m_pInfo->dwItemTypeID	=dwItemTypeID;
	m_pInfo->dwUseGMToolPsw	=dwUseGMToolPsw;
	
	BOOL bSucMake	=true;
	bSucMake	&=m_StrPacker.AddString(pszSpeaker);
	bSucMake	&=m_StrPacker.AddString(pszHearer);
	bSucMake	&=m_StrPacker.AddString(pszEmotion);
	bSucMake	&=m_StrPacker.AddString(pszWords);
	bSucMake	&=m_StrPacker.AddString(pszItemName);
	bSucMake	&=m_StrPacker.AddString(szSenderFeel);
	m_unMsgType	=_MSG_TALK;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	
	return bSucMake;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgTalk::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

void CMsgTalk::Dump(char* pbufMsg, DWORD dwMsgSize)///当此消息出现错误时将消息全部内容写到日志中
{
	DEBUG_TRY 
		char szSender[_MAX_NAMESIZE]="";
		char szReceiver[_MAX_NAMESIZE]="";
		char szEmotion[_MAX_NAMESIZE]="";
		char szWords[_MAX_SPEAK_WORDSSIZE]="";
		char szItemName[_MAX_WORDSSIZE]="";
		CMsgTalk msg;
		if(dwMsgSize>_MAX_MSGSIZE)
		{
			LogSave("talk msg size is error!MsgSize:%d",dwMsgSize);
			dwMsgSize=_MAX_MSGSIZE;
		}
		msg.Create(pbufMsg,dwMsgSize);
		CMsgTalk::MSG_Info& info=msg.GetInfo();
		CNetStringPacker StrPacker;
		StrPacker.SeekBuf(info.szBuf,_MAX_MSGSIZE-sizeof(CMsgTalk::MSG_Info)+1);
 		StrPacker.GetString(0, szSender, sizeof(szSender));
 		StrPacker.GetString(1, szReceiver, sizeof(szReceiver));
 		StrPacker.GetString(2, szEmotion, sizeof(szEmotion));
 		StrPacker.GetString(3, szWords, sizeof(szWords));
 		StrPacker.GetString(4, szItemName,sizeof(szItemName));
 		LogSave("talk msg crash1!MsgSize:%d-WordsColor:%d-TxtAttribute:%d-TxtStyle:%d-Time:%d-UseGMToolPsw:%d-ItemID:%d-dwItemTypeID:%d",
				dwMsgSize,info.dwWordsColor,info.unTxtAttribute,info.unTxtStyle,info.dwTime,info.dwUseGMToolPsw,info.dwItemID,info.dwItemTypeID);
 		LogSave("talk msg crash2!Sender:%s-Receiver:%s-Emotion:%s-Words:%s-ItemName:%s",
				szSender,szReceiver,szEmotion,szWords,szItemName);
	DEBUG_CATCH("CMsgTalk::Dump crash")
}

//////////////////////////////////////////////////////////////////////
void CMsgTalk::Process(void *pInfo)
{
	try
	{
#ifdef _MSGDEBUG
	::LogMsg("Process MsgTalk, Sender:%s, Receiver:%s, Words:%s", 
						szSender, 
						szReceiver, 
						szWords);
#endif

	char szSender[_MAX_NAMESIZE];
	char szReceiver[_MAX_NAMESIZE];
	char szEmotion[_MAX_NAMESIZE];
	char szWords[_MAX_SPEAK_WORDSSIZE];
	char szItemName[_MAX_WORDSSIZE];

#if defined(_DEBUG) || defined(_ASHRELEASE)
	const DWORD dwUseGMToolPsw = 0;		//GM使用GM命令密码。只有GMTOOL拥有.
	m_pInfo->dwUseGMToolPsw = dwUseGMToolPsw;
#else
	const DWORD dwUseGMToolPsw = 7788414;
#endif
	m_StrPacker.GetString(0, szSender, sizeof(szSender));
	m_StrPacker.GetString(1, szReceiver, sizeof(szReceiver));
	m_StrPacker.GetString(2, szEmotion, sizeof(szEmotion));
	m_StrPacker.GetString(3, szWords, sizeof(szWords));
 	m_StrPacker.GetString(4, szItemName,sizeof(szItemName));


	CHECK(strlen(szWords) <= 255);

	IRole* pRole = NULL;
	DEBUG_TRY		
	pRole = RoleManager()->QueryRole(this);
	DEBUG_CATCH("pRole = RoleManager()->QueryRole(this); in mapgroup Cmsgtalk")

// 	CUser* pRoleOtherLine = UserManager()->GetUser(szSender);
	if(!pRole)
	{
// 		if(IsNpcMsg())
// 			return;
		// TransmitMsg
		switch(m_pInfo->unTxtAttribute)
		{
		case	_TXTATR_SYNDICATE:
			{
				OBJID idSyn = m_pInfo->idTransmit;
				CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
				if (pSyn)
				{
					pSyn->BroadcastSynMsg(this);
				}

			}
			break;
		case	_TXTATR_PRO:
			{
// 				CUserManager::Iterator pListener = UserManager()->NewEnum();
// 				while(pListener.Next())
// 				{
// 					if(pListener && pListener->CanBeSee()==false)
// 						continue;
// 
// 					if(pListener && pListener->GetInfo()->FirJob == m_pInfo->dwItemTypeID || pListener->GetInfo()->SecJob == m_pInfo->dwItemTypeID)
// 					{
// 						pListener->SendMsg(this);
// 					}
// 				}
				UserManager()->BroadcastProMsg(m_pInfo->dwItemTypeID,this);
			}
			break;
		case _TXTATR_FORGMTOOL:
			{
				char szCmd[32];
				char szName[32];
//modify code by 林德渊 begin 2011.5.5
				int MaxStopTime = 0;
				int StopTime = 0;
				sscanf(szWords, "%s %s %d %d", szCmd, szName, &MaxStopTime, &StopTime);
//				sscanf(szWords, "%s %s", szCmd, szName);
//modify code by 林德渊 end
				if(strcmp(szCmd, "kickout") == 0)
				{
					CUserPtr pTarget = UserManager()->GetUser(szName);
					if (pTarget)
					{
						UserManager()->KickOutSocket(pTarget->GetSocketID(), "GM /kickout");
					}
				}
				if(strcmp(szCmd, "stopsay") == 0)
				{
					CUserPtr pTarget = UserManager()->GetUser(szName);
					if (pTarget)
					{
//modify code by 林德渊 begin 2011.5.5
						CRole::AttachStatus( pTarget->QueryRole(),STATUS_STOPSAY,MaxStopTime,StopTime );//power此时无意义
//						CRole::AttachStatus( pTarget->QueryRole(),STATUS_STOPSAY,6000,6000 );//power此时无意义
//modify code by 林德渊 end
					}
				}
			}
			break;
		default:
			break;
		}
		
		return;
	}
	

	CUser* pUser = NULL;
	DEBUG_TRY
	if(pRole)
		pRole->QueryObj(OBJ_USER, IPP_OF(pUser));		//? pUser may be null
	DEBUG_CATCH("pRole->QueryObj(OBJ_USER, IPP_OF(pUser)) in mapgroup Cmsgtalk");
// 	if (pRoleOtherLine)
// 	{
		if(pUser && strcmp(pUser->GetName(), szSender) != 0)
		{
			if (!pUser->IsGM())
			{
				::GmLogSave("玩家[%s]企图冒用他人名字[%s]发布Talk消息，请予封号。", pUser->GetName(), szSender);
				return ;
			}
		}
//	}
	if(pUser==NULL)
	{
		LogSave("CMsgTalk::Process pUser==NULL waring!!继续往往下走可能会crash!");
		CMsgTalk::Dump(GetBuf(),GetSize());
	}
	if(!pRole->IsTalkEnable())
	{
		pRole->SendSysMsg("您被禁言了,您说的话别人无法听见");
		return;
	}

	if(pUser && pUser->IsGM())
	{
		::GmLogSave("-TALK %s->%s: %s", szSender, szReceiver, szWords);
	}

	if(pUser && szWords[0] == '!')
	{
		BOOL bIsCmd = FALSE;
		char	szCmd[_MAX_WORDSSIZE]	= "NO_CMD";
		char	szParam[_MAX_WORDSSIZE] = "";
// 		DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVVVVVVV
		sscanf(szWords+1, "%s %s", szCmd, szParam);
		if( 0 == stricmp( szCmd,"在线人数") )//	"在线人数"	//在线人数为高级权利，需要GMTOOL密码验证
		{
			if( pUser->IsPM() /*|| pUser->IsPM()) && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				long nTopCount = pUser->GetTopUserCount();
				long nCurCount = pUser->GetCurUserCount();
				char szTemp[ 128 ] = { 0 };
				sprintf( szTemp,"@usercount %d %d",nCurCount,nTopCount );//@当前 最高
				pUser->SendSysMsg( szTemp );
				bIsCmd = TRUE;
			}

		}
		else if( 0 == stricmp( szCmd,"hideme" ) )
		{
			if( pUser->IsPM() /*|| pUser->IsPM()*/ )
			{
				if(pUser->ChangeHideStatus())
					pUser->SendSysMsg("现在你处于隐身状态");
				else
					pUser->SendSysMsg("现在你处于现身状态");
				bIsCmd = TRUE;
			}
		}
// 		else if(stricmp(szCmd, "pro") == 0)		//更换职业是高级权利，需要GMTOOL密码验证
// 		{
// 			if(pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
// 			{
// 				int	nData = atoi(szParam);
// 				pUser->SetAttrib(_USERATTRIB_PORFESSION, nData, SYNCHRO_TRUE);
// 				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
// 			}
// 		}
		else if (stricmp(szCmd, "kickoutcheat") == 0)		//踢掉作弊为高级权利，需要GMTOOL密码验证
		{
			if(pUser->IsPM() /* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw */)
			{
				int	nData = 0;
				if (2 == sscanf(szWords+1, "%s %d", szCmd, &nData))
				{
					extern long	g_sKickoutCheat;
					long nOld = InterlockedExchange(&g_sKickoutCheat, nData);
					pUser->SendSysMsg("set kickoutcheat OK! old value is %d", nOld);
					bIsCmd = TRUE;
				}
			}
		}
// 		else if (stricmp(szCmd, "anc") == 0)
// 		{
// 			//	0001_2000000147_兔子_10000263_0
// 			if(pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
// 			{
// 					char	szMsg[256] = "";
// 					sprintf(szMsg, "<0_sb_7>率队在海上除妖活动中,击败了野心勃勃的<3_徐福魔影_6>，平息了海上之乱，快去看看<3_徐福的宝箱_6>中都有什么好东西吧！");
// 					UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE, NULL, 0, 0, "" , ALLUSERS_NAME);
// 			}
// 		}
		else if(stricmp(szCmd, "kickout") == 0)			//踢掉玩家是高级权利,需要GMTOOL密码验证
		{
			if (pUser->IsPM() ||
				(strstr(szSender, "[PM]") != NULL)/* && strcmp(szEmotion , "JoyCc") == 0*//*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				CUserPtr pTarget = UserManager()->GetUser(szParam);
				if (pTarget)
				{
					UserManager()->KickOutSocket(pTarget->GetSocketID(), "GM /kickout");
				}
				else//不在本地图组
				{

					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, szReceiver);
//********************************************************************
// 					int nUserID = PostOffice()->GetReceiverID( szParam );
// 					CHECK( nUserID );
// 					
// 					char	buf[1024]={0};
// 					REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
// 					pInfo->idUser		= nUserID;
// 					pInfo->ucFuncType	= REMOTE_CALL_KICKOUTPLAYER;
// 					pInfo->nSize		= sizeof(REMOTE_CALL0) - sizeof(long);
// 					MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
				}
				bIsCmd = TRUE;
			}
		}
		else if (stricmp(szCmd, "ID") == 0)
		{
			if(pUser->IsPM()/* && strcmp(szEmotion , "JoyCc") == 0*//*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				CUserPtr pTarget = UserManager()->GetUser(szParam);
				MSGBUF  szTargetPlayer = "NONE";
				int	Identity;
				sscanf(szWords+1, "%s %s %d",szCmd,&szTargetPlayer,&Identity);
				if (pTarget)
				{
					pTarget->SetIdentity(Identity);
				}
				else
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, szReceiver);
				bIsCmd = TRUE;
			}
		}
		else if(stricmp(szCmd, "cleardrug") == 0)			//大红大蓝剩余量清0
		{
			if(pUser->IsGM())
			{
				pUser->SetHpAdd(0);
				pUser->SetMpAdd(0);
				pUser->SetPetAdd(0);
				CMsgUserAttrib	msg;//移动速度以及大红大蓝
				if(msg.Create(pUser->GetID(), _USERATTRIB_DRUG_ADDHP, pUser->GetHpAdd()))
				{
						msg.Append(_USERATTRIB_DRUG_ADDMP, pUser->GetMpAdd());
						msg.Append(_USERATTRIB_DRUG_ADDPET, pUser->GetPetAdd());
						pUser->SendMsg(&msg);
				}
			}
		}
		else if(stricmp(szCmd, "kickoutall") == 0)		//踢掉所有玩家是高级权利,需要GMTOOL密码验证
		{
			if(pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				LOGMSG("kickoutall process!");
				pUser->SendSysMsg(g_objStrRes.GetStr(12097));//STR_KICKOUT_ALL
				MapGroup(PID)->QueryIntraMsg()->PrintText("Server stop by GM, close server please!");
				MapGroup(PID)->QueryIntraMsg()->CloseMapGroup(GetSocketID());
				bIsCmd = TRUE; 
			}
		}
		else if(stricmp(szCmd, "find") == 0)			//飞到玩家身边是高级权利,需要GMTOOL密码验证
		{
			if(pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				CUserPtr pTarget = UserManager()->GetUser(szParam);
				if (pTarget)
				{
					int nPosX = pTarget->GetPosX();
					int nPosY = pTarget->GetPosY();
					
					pUser->FlyMap(pTarget->GetMapID(), nPosX, nPosY);
				}
				bIsCmd = TRUE;
			}
		}
		else if( 0 == stricmp( szCmd,"awardexp" ) )
		{
			if(pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				__int64 nData = _atoi64( szParam );
				if( nData > 0 )
				{
					pUser->AwardBattleExp( nData,false );
					bIsCmd = TRUE;	
				}
			}
		}
		else if( 0 == stricmp( szCmd,"vaslimit" ) )//限量商品数量调整
		{
			if(pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				OBJID itemType = ID_NONE;
				int nData = 0;
				if (3 == sscanf(szWords+1, "%s %d %d", szCmd, &itemType, &nData))
				{
					if(itemType == ID_NONE)
						return;
					if( nData >= 0 )
					{
						vasLimitItem* pLimitItem = VASItem()->GetLimitItemByType(itemType);
						if(pLimitItem)
						{
							int nAmount = max(0, nData);
							VASItem()->SetLimitItemAmount(itemType, nAmount, true);

							CMsgVipList msg;//借用这个协议广播到其他线
							if (msg.Create(pUser->GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_VASLIMIT))
							{
								msg.Append(itemType, nAmount, 0, 0, 0, "");
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
							}
						}
					}
					bIsCmd = TRUE;
				}
			}
		}
#if defined(_DEBUG) || defined(_ASHRELEASE)
		else if (stricmp(szCmd, "rename") == 0)
		{
			pUser->SetName(szParam);
			pUser->SendSysMsg("阁下已经更名为%s.", szParam);
			bIsCmd = TRUE;	
		}

		else if(stricmp(szCmd, "pk") == 0)
		{
			if(pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				int nData = 0;
				if (2 == sscanf(szWords+1, "%s %d", szCmd, &nData))
				{
					int nDeltaPk = nData - pUser->GetPk();
					pUser->AddPk(nDeltaPk);
					bIsCmd = TRUE;
				}
			}
		}

		else if(stricmp(szCmd, "awardmoney") == 0)		//增加金钱是高级权利,需要GMTOOL密码验证
		{
			if(pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				int	nMoney = atoi(szParam);
				if(pUser->GainMoney(nMoney, SYNCHRO_TRUE))
					pUser->SendSysMsg(g_objStrRes.GetStr(15096));//"[您的钱增加了。]"
				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
			}
		}
		else if(stricmp(szCmd, "awarditem") == 0)		//增加物品是高级权利,需要GMTOOL密码验证
		{
			if(pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw */)
			{
				int	nItemType = 0;
				int nAmount = 0;
				if (3 == sscanf(szWords+1, "%s %d %d", szCmd, &nItemType, &nAmount))
				{
					if(pUser->AwardItemCount(nItemType,nAmount))
						pUser->SendSysMsg("[您的物品增加了.]");
					bIsCmd = TRUE;
				}
			}
		}
// 		else if (stricmp(szCmd, "awardjinmeivalue") == 0)
// 		{
// 			if (pUser->IsGM())
// 			{
// 				int nJinmeiValue = atoi(szParam);
// 				pUser->IncJinmeiValue(nJinmeiValue);
// 				bIsCmd = true;
// 			}
// 		}
		else if(stricmp(szCmd, "awardEudemon") == 0)		//增加物品是高级权利,需要GMTOOL密码验证
		{
			if(pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw */)
			{
				int	nEudemonType = 0;
				if (2 == sscanf(szWords+1, "%s %d", szCmd, &nEudemonType))
				{
					EudemonData data;
					if(pUser->AwardEudemon(nEudemonType,data))
						pUser->SendSysMsg("[您的宠物增加了.]");
					bIsCmd = TRUE;
				}
			}
		}
		else if(stricmp(szCmd, "uplev") == 0)
		{
			if (pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/)
			{
				int	nLev = atoi(szParam);
				if (nLev > 0)
				{
					pUser->IncLev(nLev);
				}
				bIsCmd = TRUE;
			}
		}
		else if(stricmp(szCmd, "reducesyn") == 0)
		{
			if (pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/)
			{
				OBJID idSyn = pUser->GetSynID();
				CSyndicate *pSyn = SynManager()->QuerySyndicate(idSyn);
                                if (pSyn)
				    pSyn->ReduceLevel();
			}
			bIsCmd = TRUE;
		}
		else if(stricmp(szCmd, "syncon") == 0)
		{
			if (pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/)
			{
				int nCon = 0;
				int nAmount = 0;
				if (2 == sscanf(szWords+1, "%s %d", szCmd, &nCon))
				{
					pUser->SetSynCon(nCon);
				}
			}
			bIsCmd = TRUE;
		}
		else if(stricmp(szCmd, "synpaybasic") == 0)
		{
			if (pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/)
			{
				if(pUser->GetSyndicate())
				{
					pUser->GetSyndicate()->PayBasic();
				}
			}
			bIsCmd = TRUE;
		}
		else if(stricmp(szCmd, "synmoney") == 0)
		{
			if (pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/)
			{
				int nMoney = 0;
				int nAmount = 0;
				if (2 == sscanf(szWords+1, "%s %d", szCmd, &nMoney))
				{
					pUser->SetSynMoney(nMoney);
				}
			}
			bIsCmd = TRUE;
		}
		else if(stricmp(szCmd, "syngrow") == 0)
		{
			if (pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/)
			{
				int nGrow = 0;
				int nAmount = 0;
				if (2 == sscanf(szWords+1, "%s %d", szCmd, &nGrow))
				{
					pUser->SetSynGrow(nGrow);
				}
			}
			bIsCmd = TRUE;
		}
		else if(stricmp(szCmd, "setskill") == 0)
		{
			if (pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/)
			{
				int nSub = 0;
				int nAmount = 0;
				if (3 == sscanf(szWords+1, "%s %d %d", szCmd, &nSub, &nAmount))
				{
					OBJID idSyn = pUser->GetSynID();
					CSyndicate *pSyn = SynManager()->QuerySyndicate(idSyn);
					if (pSyn)
					{
						pSyn->SetSkill(nSub, nAmount);
					}
				}
			}
			bIsCmd = TRUE;
		}
		else if(stricmp(szCmd, "life") == 0)
		{
			if (pUser->IsGM())
			{
				int nData = atoi(szParam);
				if (nData<=0) 
					nData = pUser->GetMaxLife();
				pUser->SetAttrib(_USERATTRIB_LIFE, nData, SYNCHRO_TRUE);
				bIsCmd = TRUE;
			}
		}
		else if(stricmp(szCmd, "mana") == 0)
		{
			if (pUser->IsPM())
			{
				int nData = atoi(szParam);
				if (nData<=0) 
					nData = pUser->GetMaxMana();
				pUser->SetAttrib(_USERATTRIB_MANA, nData, SYNCHRO_TRUE);
				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
			}
		}
		else if(stricmp(szCmd, "sp") == 0)//加满怒气，goto添加，测试技能使用
		{
			if (pUser->IsPM())
			{
				int nData = pUser->GetMaxSp();
				pUser->SetAttrib(_USERATTRIB_SP, nData, SYNCHRO_TRUE);
				bIsCmd = TRUE; 
			}
		}
		else if(stricmp(szCmd, "ene") == 0)//加满精气，goto添加，测试技能使用
		{
			if (pUser->IsPM())
			{
				int nData = pUser->GetMaxEne();
				pUser->AddEne(nData);
				bIsCmd = TRUE; 
			}
		}
		else if(stricmp(szCmd, "magiccd") == 0)//清除CD，goto添加，测试技能使用
		{
			if (pUser->IsPM())
			{
				CMagic* pMagic = pUser->QueryMagic();
				if (!pMagic)
					return;
				pMagic->ClearMagicCD();
				pMagic->ClearDelay();

				CMsgMagicEffect msg;
				IF_OK(msg.Create(pUser->GetID(), 1134, 1, 0,0))
					pUser->SendMsg(&msg);
				bIsCmd = TRUE; 
			}
		}
		else if(stricmp(szCmd, "showaction") == 0)
		{
			if (pUser->IsPM())
			{
				if(pUser->DebugShowAction())
					pUser->SendSysMsg(g_objStrRes.GetStr(15098));//"可以显示ACTION了。"
				else
					pUser->SendSysMsg(g_objStrRes.GetStr(15099));//"不显示ACTION了。"
				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
			}
		}
		else if (stricmp(szCmd, "player") == 0)			//查询地图组玩家,是高级权利,需要GMTOOL密码验证
		{
			if(pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				//if (pUser->GetAccountID() <= 100)
				//{
				if(stricmp(szParam, "all") == 0)
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(11052), UserManager()->GetUserAmount(), UserManager()->GetMaxUser());//STR_SHOW_PLAYERS_uu
				}
				else if(stricmp(szParam, "map") == 0)
				{
					OBJID idMap = pUser->GetMapID();
					pUser->SendSysMsg("total %u player in this map.", UserManager()->GetMapUserAmount(idMap));
				}
				//}				

				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
			}
		}
		else if (stricmp(szCmd, "setmaxplayer") == 0)		//设定玩家上限是GM高级权利,需要GMTOOL密码验证
		{
			if (pUser && pUser->IsPM() /*&& strlen(szParam) > 0 && (dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw )*/)
			{
				int nMaxPlayers = atoi(szParam);
				extern DWORD g_dwMaxPlayer;
				InterlockedExchange((long*)&g_dwMaxPlayer, nMaxPlayers);
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(11051), nMaxPlayers);//STR_SET_MAX_PLAYERS_u
				}

				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
			}
		}
		else if (stricmp(szCmd, "fly") == 0)
		{
			if (pUser && pUser->IsPM())
			{
				MSGBUF	szCmd	= "NO_CMD";
				int nPosX = 0, nPosY = 0;
				if (3 == sscanf(szWords+1, "%s %d %d", szCmd, &nPosX, &nPosY))
				{
					CMapPtr pMap = pUser->GetMap();
					if (pMap)
					{
						pUser->FlyMap(ID_NONE, nPosX, nPosY);
					}
				}
				bIsCmd = TRUE;
			}
		}
		else if (stricmp(szCmd, "chgmap") == 0)
		{
			if (pUser && pUser->IsPM())
			{
				CUser* pObjectUser=NULL,*pPmUser=NULL;
				MSGBUF	szCmd	= "NO_CMD";
				MSGBUF  szTargetPlayer = "NONE";
				OBJID idMap = ID_NONE;
				int nPosX = 0, nPosY = 0;
				int nNum=sscanf(szWords+1, "%s %s %u %d %d", szCmd,szTargetPlayer, &idMap, &nPosX, &nPosY);
				if (nNum==4)
					nNum=sscanf(szWords+1, "%s %u %d %d", szCmd,&idMap, &nPosX, &nPosY);
				if(nNum==4 || nNum==5)///兼容旧chgmap协议,未指定TargetPlayer,则认为是自己
//				if (5 == sscanf(szWords+1, "%s %s %u %d %d", szCmd,szTargetPlayer, &idMap, &nPosX, &nPosY))
				{
					if (nNum==5)
					{
						CUser* pObjectUser=UserManager()->GetUser(szTargetPlayer);
						if (pObjectUser)
						{
							pUser->SendSysMsg(g_objStrRes.GetStr(16000),pObjectUser->GetName());//"尝试改变%s的位置!"
//							LPCSTR pPmName=pUser->GetName();
							pPmUser=pUser;
							pUser=pObjectUser;
							pUser->SendSysMsg(g_objStrRes.GetStr(16001),pPmUser->GetName());//"%s正尝试改变你的位置!"
						}
					}

					CMapPtr pMap = MapManager()->QueryMap(idMap);
					if (pMap)
					{
#ifdef	LOCAL_DEBUG
						if(!pMap->IsValidPoint(nPosX, nPosY))
#else
						if(!pMap->IsStandEnable(nPosX, nPosY))
#endif
						{
							if (pPmUser)
								pPmUser->SendSysMsg(g_objStrRes.GetStr(11050));//STR_CAN_STAND
							else
								pUser->SendSysMsg(g_objStrRes.GetStr(11050));//STR_CAN_STAND
							return;
						}
						IMapThing* pMapThing = NULL;

						//20070321修罗:返回值改为指针,检查安全性
						//IThingSet* pSet = pMap->QueryBlock(nPosX, nPosY).QuerySet();
						CGameBlock* gameBlockTemp = pMap->QueryBlock(nPosX, nPosY);
						CHECK(gameBlockTemp);
						IThingSet* pSet = gameBlockTemp->QuerySet();
						//-------------------------------------
						
						CHECK(pSet);
						for(int i = 0; i < pSet->GetAmount(); i++)
						{
							IMapThing* pTheMapThing = pSet->GetObjByIndex(i);
							if (pTheMapThing && 									
									pTheMapThing->GetObjType() == OBJ_NPC &&
										pTheMapThing->GetPosX() == nPosX && pTheMapThing->GetPosY() == nPosY)
							{
								pMapThing = pTheMapThing;
								break;		
							}
						}

						if (!pMapThing)
							pUser->FlyMap(idMap, nPosX, nPosY);
						else
							pUser->SendSysMsg(g_objStrRes.GetStr(11050));//STR_CAN_STAND
					}
					else
					{
						pUser->FlyMap(idMap, nPosX, nPosY);
					}
				}
				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
			}
		}
#endif
		else if (stricmp(szCmd, "服务") == 0)//  [6/8/2007] halei
		{
			if (pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw */)
			{
				char szMsg[256] = "";
				if (2 == sscanf(szWords+1, "%s %s", szCmd, szMsg))
				{
					char szBuf[300]	= "";
//modify code by 林德渊 begin 2011.5.17
/*					for (int k=0; k<256; k++)
					{
						if (szMsg[k] == '_')
						{                                                                                                                                                                                 
							szMsg[k] = ' ';
						}
					}*/
					sprintf(szBuf, "%s", szMsg);

					string newszmsg = szBuf;
					if( newszmsg.find("ALAL") == 0 )
					{
						char szstar[256] = {0};
						int nyear = -1,nmont = -1,ndays = -1,nhour = -1,nminn = -1;
						sscanf(szBuf,"ALAL%d-%d-%d-%d-%d_%s",&nyear,&nmont,&ndays,&nhour,&nminn,szstar);
						if( !(nyear < 2011 || nmont < 0 || nmont > 12 || ndays < 0 || ndays > 31 || nhour < 0 || nhour > 24 || nminn < 0 || nminn > 59) )
						{
							struct tm lat1 = {0};
							lat1.tm_year = nyear - 1900;
							lat1.tm_mon = nmont - 1;
							lat1.tm_mday = ndays;
							lat1.tm_hour = nhour;
							lat1.tm_min = nminn;
							if( (int)(mktime(&lat1) - time(NULL)) > 300 )
							{
								char newszMsg[256] = "";
								sprintf(newszMsg,"ALAL%s",szstar);
								MapGroup(PID)->AddCircle(newszMsg, (int)(mktime(&lat1) - time(NULL))/60);
							}
						}
					}
					else
						UserManager()->BroadcastMsg(szBuf, NULL, NULL, 0xff0000, _TXTATR_TOPANNOUNCE);//王玉波修改
//modify code by 林德渊 end
				}
				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
			}
		}
		else if (stricmp(szCmd, "JoyCc") == 0)//  单服公告，配合GM工具使用
		{
			if (pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw */)
			{
				char szMsg[256] = "";
				if (2 == sscanf(szWords+1, "%s %s", szCmd, szMsg))
				{
					char szBuf[300]	= "";
					sprintf(szBuf, "%s", szMsg);
					UserManager()->BroadcastMsg(szBuf, NULL, NULL, 0xff0000, _TXTATR_ANNOUNCE);//王玉波修改
				}
				bIsCmd = TRUE;	
			}
		}
		else if (stricmp(szCmd, "循环") == 0)//  循环公告，配合GM工具使用
		{
			if (pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw */)
			{
				char szMsg[256] = "";
				char sztime[32] = "";
				if (3 == sscanf(szWords+1, "%s %s %s", szCmd, sztime, szMsg ))
				{
//modify code by 林德渊 begin 2011.5.17
					string chstring = sztime;
					if( (int)chstring.find("-") > 0 )
					{
						int nyear = -1,nmont = -1,ndays = -1,nhour = -1,nminn = -1,nnums = -1;
						sscanf(sztime,"%d-%d-%d-%d-%d-%d",&nyear,&nmont,&ndays,&nhour,&nminn,&nnums);
						if( !(nyear < 2011 || nmont < 0 || nmont > 12 || ndays < 0 || ndays > 31 || nhour < 0 || nhour > 24 || nminn < 0 || nminn > 59 || nnums < 5) )
						{
							struct tm lat1 = {0};
							lat1.tm_year = nyear - 1900;
							lat1.tm_mon = nmont - 1;
							lat1.tm_mday = ndays;
							lat1.tm_hour = nhour;
							lat1.tm_min = nminn;
							if( (int)(mktime(&lat1) - time(NULL)) > 300 )
							{
								char newszMsg[256] = "";
								sprintf(newszMsg,"ALSI%d_%s",nnums,szMsg);
								MapGroup(PID)->AddCircle(newszMsg, (int)(mktime(&lat1) - time(NULL))/60);
							}
						}
					}
					else
						MapGroup(PID)->AddCircle(szMsg, atoi(sztime));
//modify code by 林德渊 end
				}
				bIsCmd = TRUE;	
			}
		}
#if defined(_DEBUG) || defined(_ASHRELEASE)
		else if((stricmp(szCmd,"status") == 0))
		{
			if (pUser->IsPM() /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				MSGBUF	szCmd	= "NO_CMD";
				int nStatusTemp = 0;//状态
				int nPowTemp = 0;//power
				int nSecTemp = 0;//持续时间
				int nTimesTemp = 0;//次数
				
				sscanf(szWords+1, "%s %d %d %d %d",
					szCmd,&nStatusTemp,&nPowTemp,&nSecTemp,&nTimesTemp);
				
				CRole::AttachStatus( pUser->QueryRole(),nStatusTemp,nPowTemp,nSecTemp,nTimesTemp);
				bIsCmd = TRUE;	// [2007-7-25] 朱斌 

			}
		}
#endif
		else if (stricmp(szCmd,"禁言") == 0)//  [7/11/2007 Add By HaLei] 
		{
			if (pUser->IsPM() ||
				(strstr(szSender, "[PM]") != NULL)/*&& strcmp(szEmotion, "JoyCc") == 0*/ /*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw */)
			{
				MSGBUF	szCmd	= "NO_CMD";
				CUser* pObjectUser = NULL;
				MSGBUF  szTargetPlayer = "NONE";
				int ntimesec = 0;
				sscanf(szWords+1, "%s %s %d",szCmd,&szTargetPlayer,&ntimesec);
				pObjectUser=UserManager()->GetUser(szTargetPlayer);
				if (pObjectUser)
				{
					CRole::AttachStatus( pObjectUser->QueryRole(),STATUS_STOPSAY,ntimesec,ntimesec );//power此时无意义
				}
				else
				{
					//通过名字得到ID
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, szReceiver);
				//	int nUserID = PostOffice()->GetReceiverID( szTargetPlayer );
				//	CHECK( nUserID );
				//	MapGroup(this->m_idProcess)->QuerySynMessage()->StopSayByGM( szTargetPlayer,ntimesec );
				}
				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
			}
		}
#if defined(_DEBUG) || defined(_ASHRELEASE)
		else if (!stricmp(szCmd, "sg") || !stricmp(szCmd, "刷怪"))//  [6/8/2007] halei
		{
			if (pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw */)
			{
				MSGBUF	szCmd	= "NO_CMD";
				OBJID idType = ID_NONE;
				OBJID idMap = ID_NONE;
				int nPosX = 0, nPosY = 0;
				sscanf(szWords+1, "%s %u %u %d %d",szCmd,&idType,&idMap, &nPosX, &nPosY);
				ST_CREATENEWNPC	info;
				memset(&info, 0, sizeof(ST_CREATENEWNPC));
				//[游途道標 2009.03.16]刷怪命令加強
				if (idMap == 0 && nPosX == 0 && nPosY == 0)
				{
					idMap = pUser->GetMapID();
					nPosX = pUser->GetPosX();
					nPosY = pUser->GetPosY();
				}
				//not in this			info.id				= MapManager()->SpawnNewPetID();
				info.idMap			= idMap;//pNpc->GetMapID();
				info.idData			= 0;//1745;//20080131修罗:此id保证在sk_generator中存在
				info.idOwner		= 0;//pNpc->GetInt(NPCDATA_OWNERID);		// 总帮
				info.usPosX			= nPosX;
				info.usPosY			= nPosY;
				//			info.usAction		= MSGAINPCINFO_CREATENEW;
				info.usType			= idType;
				info.ucOwnerType	= 0;//pNpc->GetInt(NPCDATA_OWNERTYPE);
				info.nData			= 0;//nData;
				
				NpcManager()->CreateSynPet(&info);

				bIsCmd = TRUE;	// [2007-7-25] 朱斌 
			}
			
		}
		//[游途道标 2009.03.28]action命令,只能完成部分功能,仅供程序测试用.
		else if (stricmp(szCmd, "chufa") == 0)
		{
			if (pUser->IsPM()/* && dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				MSGBUF szCmd = "NO_CMD";
				OBJID idActionType = NULL;
				MSGBUF szParam = "NO_CMD";
				int data = NULL;
				bool bState = false;
				for (int i = 0; i < strlen(szWords); ++i)
				{
					if (bState && szWords[i] == ' ')
					{
						szWords[i] = '_';
					}
					else if (szWords[i] == '\"' || szWords[i] == '\'')
					{
						bState = !bState;
					}
				}
				sscanf(szWords+1, "%s %u %d \"%s\"", szCmd, &idActionType, &data, szParam);
				for (i = 0; i < strlen(szParam); ++i)
				{
					if (szParam[i] == '_')
					{
						szParam[i] = ' ';
					}
				}
				if (strcmp("NO CMD", szParam) == 0)
				{
					strcpy(szParam, "");
				}
				pUser->ProcessActionUser(idActionType, szParam, data);///允许打开副本介面
			}
			bIsCmd = TRUE;
		}
#endif
		else if (stricmp(szCmd, "封号") == 0)
		{
//modify code by 林德渊 begin 2011.5.13
			//if (pUser->IsPM() && (strstr(szSender, "[PM]") != NULL)/*&&*/ /*strcmp(szEmotion, "JoyCc") == 0*//*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw */)
			if (pUser->IsPM())
//modify code by 林德渊 end
			{
				MSGBUF	szCmd	= "NO_CMD";
				CUser* pObjectUser = NULL;
				MSGBUF  szTargetPlayer = "NONE";
				int ntimesec = 0;
				sscanf(szWords+1, "%s %s",szCmd,&szTargetPlayer);
				stTempUser* pTempUser = UserManager()->QueryTempUser(szTargetPlayer);
				if(pTempUser)
				{
					int nUserID = pTempUser->idAcc;

					CMsgStopAccount msgStopAccount;
					if (msgStopAccount.Create(nUserID))
					{
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msgStopAccount );
					}
				}
				bIsCmd = TRUE;
			}
		}
		else if (stricmp(szCmd, "解封") == 0)
		{
			if (pUser->IsPM() /*&& strcmp(szEmotion, "JoyCc") == 0*//*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
			{
				MSGBUF	szCmd	= "NO_CMD";
				CUser* pObjectUser = NULL;
				MSGBUF  szTargetPlayer = "NONE";
				int ntimesec = 0;
				sscanf(szWords+1, "%s %s",szCmd,&szTargetPlayer);
				stTempUser* pTempUser = UserManager()->QueryTempUser(szTargetPlayer);
				if(pTempUser)
				{
					int nUserID =  pTempUser->idAcc;
				
					CMsgStopAccount msgStopAccount;
					if (msgStopAccount.Create(nUserID,0,2))
					{
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msgStopAccount );
					}
				}
				bIsCmd = TRUE;
			}
		}

// 		DEBUG_CATCH2("CMsgTalk CMD:[%s] error.", szCmd)

		if(bIsCmd)	
			return;
	}


// 	// 普通TALK
// 	/************************新的快照处理机制,玩家要显示性别跟门派**************************************/
	if (m_pInfo->unTxtAttribute != _TXTATR_CHATWITH)
	{
 		char	szSERVER[32]		=	""		;
	// 	char	szIdentity[32]		=	""		;
	// 	char	szTemp[512]			=	""		;
	// 	char	szSex[32]			=	""		;
	// 	char	szName[32]			=   ""		;
	// 	char	SnapShoot[2][128]				;
	// 	int		SnapShootTemp[4]				;
	// 	char	szRName[32]		    =	""		;
 		int		iLine				=	0		;
		int		nSex				=	0		;
		int		nIdentity			=	0		;
		int		nVip				=	0		;
		int		nALL				=	0		;

		int		iSex				=	8		;
		int		iId[5]				=	{0, 16, 32, 48, 64};
		int		iVip[5]				=	{0, 128, 256, 384, 512};
	// 	string	szNewWords						;
	// 	BOOL	Flag				= false		;
	// 		
	// 	//////////////////////////////////////////////////////////////////////添加新频道时都得在这个地方进行处理

 		strcpy(szSERVER, MapGroup(PID)->GetServerName());
		if (strcmp(szSERVER, "一线") == 0)
		{
			iLine = 1;
		}
		else	if (strcmp(szSERVER, "二线") == 0)
				{
					iLine = 2;
				}
		else	if (strcmp(szSERVER, "三线") == 0)
				{
					iLine = 3;
				}
		else	if (strcmp(szSERVER, "四线") == 0)
				{
					iLine = 4;
				}
		else	if (strcmp(szSERVER, "五线") == 0)
				{
					iLine = 5;
				}
		else	if (strcmp(szSERVER, "六线") == 0)
				{
					iLine = 6;
				}
// 		else	if (strcmp(szSERVER, "英雄之门") == 0)
// 				{
// 					iLine = 1;
// 				}
		nSex	=	pUser->GetSex();
		nIdentity = pUser->GetIdentity();
		if(pUser->IsVipActive())
			nVip	=	pUser->GetVipLev();
		nALL	=	iLine + (iSex * nSex) + iId[nIdentity] + iVip[nVip];
		
		sprintf(szEmotion, "%d", nALL);
		m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
		BOOL bSucMake	=true;
		bSucMake	&=m_StrPacker.AddString(szSender);
		bSucMake	&=m_StrPacker.AddString(szReceiver);
		bSucMake	&=m_StrPacker.AddString(szEmotion);
		bSucMake	&=m_StrPacker.AddString(szWords);
		bSucMake	&=m_StrPacker.AddString(szItemName);
		this->m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	}
// 	else if (pUser->GetIdentity() == 2)
// 	{
// 		sprintf(szIdentity, "<%d_[游戏管理员]_%d>", SNAPSHOTNOTHING, COLORLIGHTPURPLE);
// 	}
// 	if (pUser && strcmp(szSender, SYSTEM_NAME) != 0)
// 	{
// 		switch(m_pInfo->unTxtAttribute)
// 		{
// 			case	_TXTATR_WORLD:
// 				iColor = COLORLIGHTBLUE;
// 				break;
// 			case	_TXTATR_PRIVATE:
// 			case	_TXTATR_PRIVATE_OTHERLINE:
// 				iColor = COLORLIGHTPINK;
// 				break;
// 			case	_TXTATR_SYNDICATE:
// 				iColor = COLORLIGHGTYELLOW;
// 				break;
// 			case	_TXTATR_WORLDSERVER:
// 			case	_TXTATR_MAP:
// 				iColor = COLORYELLOW;
// 				break;
// 			case	_TXTATR_TEAM:
// 				iColor = COLORRICE;
// 				break;
// 			case	_TXTATR_BIGHORN:
// 				iColor = COLORRED;
// 				break;
// 			case	_TXTATR_PRO:
// 			case	_TXTATR_SECPRO:
// 				iColor = COLORYELLOW;
// 				break;
// 		}
// 		sprintf(szName, "%d_[%s]_%d_%s_%d_%d", SNAPSHOTUSER, szSender, pUser->GetSex(), szSERVER, pUser->GetCurJob(), iColor);
// 		sprintf(szRName, "%d_[%s]_%d_%s_%d_%d", SNAPSHOTUSER, szReceiver, pUser->GetSex(), szSERVER, pUser->GetCurJob(), iColor);
// 		if (0 == pUser->GetSex())
// 		{
// 			sprintf(szSex, "%d_%s_%d", SNAPSHOTNOTHING, "♂", COLORBLUE);
// 		}
// 		else if(1 == pUser->GetSex())
// 		{
// 			sprintf(szSex, "%d_%s_%d", SNAPSHOTNOTHING, "♀", COLORPINK);
// 		}
// 		else
// 		{
// 		//	::GmLogSave("%s->%s", szSender, "错误的性别!!");
// 			return;
// 		}
// 		//		int test	= 	strlen("♂");
// 	
// 		////////////////////////////////////////////////////////////////////////////////////////////////
// 		if (m_pInfo->unTxtAttribute != _TXTATR_PRIVATE && m_pInfo->unTxtAttribute != _TXTATR_BOOTHDES && m_pInfo->unTxtAttribute != _TXTATR_BOOTHNAME && m_pInfo->unTxtAttribute != _TXTATR_BOOTHLEAVEWORD
// 			&& m_pInfo->unTxtAttribute != _TXTATR_CHATWITH && m_pInfo->unTxtAttribute != _TXTATR_PRIVATE_OTHERLINE)
// 		{
// 			if (m_pInfo->unTxtAttribute == _TXTATR_WORLD || m_pInfo->unTxtAttribute == _TXTATR_SYNDICATE || m_pInfo->unTxtAttribute == _TXTATR_TEAM
// 				|| m_pInfo->unTxtAttribute == _TXTATR_WORLDSERVER || m_pInfo->unTxtAttribute == _TXTATR_BIGHORN || m_pInfo->unTxtAttribute == _TXTATR_PRO
// 				|| m_pInfo->unTxtAttribute == _TXTATR_SECPRO)
// 			{
// 				sprintf(szTemp, "<%s><%d_[%s]_%d>%s<%s><3_：_1>", szSex, SNAPSHOTNOTHING, szSERVER, iColor, szIdentity, szName);
// 			}
// 			else
// 				sprintf(szTemp, "<%s>%s<%s><3_：_1>", szSex, szIdentity, szName);//不用显示分线了
// 			strcat(szTemp, szWords);
// 		//	LogSave("talk check:%s %s",szTemp,szWords);
// 			SafeCopy(szWords, szTemp, (strlen(szTemp)+1));//safecopy不safe啊,他nia的还要加1
// 		
// 			/////////////////////////////////////////////////还要重新组包。。.幸亏不会牵扯到其他
// 			m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
// 			BOOL bSucMake	=true;
// 			bSucMake	&=m_StrPacker.AddString(szSender);
// 			bSucMake	&=m_StrPacker.AddString(szReceiver);
// 			bSucMake	&=m_StrPacker.AddString(szEmotion);
// 			bSucMake	&=m_StrPacker.AddString(szWords);
// 			bSucMake	&=m_StrPacker.AddString(szItemName);
// 			this->m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// 		}
// 	}	
	/****************************************************************************************************/
// 	DEBUG_TRY
	switch(m_pInfo->unTxtAttribute)
	{
	case	_TXTATR_GLOBAL:
		{
			const int nWorldMsgItemType = 930040;		//超级世界呼 
			if (pUser)
			{
				if(!::TalkStrCheck(szWords, true))		// true: enable new line character
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
					return ;
				}
				CItem* pTempItem = NULL;				
				pTempItem = pUser->GetItemByType(nWorldMsgItemType);
				if (pTempItem)
				{
					DWORD tempTime  = ::GetTickCount();
					if ( !WorldMsgTime || tempTime > (WorldMsgTime+35*1000) )
					{
						if (pUser->SpendItem(pTempItem))
						{
							WorldMsgTime = tempTime;

							UserManager()->BroadcastMsg(szWords, NULL/*pUser*/, NULL, 0xffffff, _TXTATR_GLOBAL,pUser);
						}
						else
						{
							pUser->SendSysMsg(g_objStrRes.GetStr(16003));//"使用超级频道失败!"
						}
					}
					else
					{
						pUser->SendSysMsg(g_objStrRes.GetStr(16004));//"上一条超级频道时间未过30秒!使用失败!"
					}
				
				}
				else
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(16003));//"使用超级频道失败!"
				}

			}
		
		}
		break;
	case	_TXTATR_PRIVATE:
		{
// 			char	szTempp[512];
// 			char	szTempWord[512];
// 			char	szTempq[512];
// // 			DEBUG_TRY
// 			strcpy(szTempWord, szWords);
			if(!::TalkStrCheck(szWords, true))		// true: enable new line character
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
				return ;
			}
// 			DEBUG_TRY
			if (!szReceiver)
			{
				return;
			}
			if (pUser->GetBeFriendByName(szReceiver) && pUser->GetBeFriendByName(szReceiver)->GetFriendType()==ENEMY_FRIEND_GROUP)
			{
				pUser->SendSysMsg("你在该玩家的黑名单中，无法进行私聊");
				return;
			}
// 			DEBUG_CATCH("MSGPRIVATE in mapgroup at step=1"); 		
// 			BOOL bSucMake	=true;
// 			
// 			sprintf(szTempq, "<%s><%d_[%s]_%d>%s<%s><3_对你说_1><3_：_1>", szSex, SNAPSHOTNOTHING, szSERVER, iColor, szIdentity, szName);
// 			strcat(szTempq, szWords);
// 			m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
// 			bSucMake	&=m_StrPacker.AddString(szSender);
// 			bSucMake	&=m_StrPacker.AddString(szReceiver);
// 			bSucMake	&=m_StrPacker.AddString(szEmotion);
// 			bSucMake	&=m_StrPacker.AddString(szTempq);
// 			bSucMake	&=m_StrPacker.AddString(szItemName);
// 			this->m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	
			CUserPtr pTarget = UserManager()->GetUser(szReceiver); 
			if(pTarget)
			{
// 				DEBUG_TRY
// 				if(pUser&&pUser->QueryEnemy()&&pUser->QueryEnemy()->GetName(pTarget->GetID()) != NULL)//发消息者的黑名单中有这个玩家,禁止私聊
// 				{
// 					pUser->SendSysMsg("该玩家在你的黑名单中，无法进行私聊");
// 					return;
// 				}
// 				
// 				if(pUser&&pTarget->QueryEnemy()&&pTarget->QueryEnemy()->GetName(pUser->GetID()) != NULL) //接收消息者放了你在自己的黑名单中，禁止私聊
// 				{
// 					pUser->SendSysMsg("你在该玩家的黑名单中，无法进行私聊");
// 					return;
// 				}
				pTarget->SendMsg(this);//sender对你说
// 				DEBUG_CATCH("MSGPRIVATE in mapgroup at step=2");
			}
			else
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, szReceiver);

			if(pUser)
			{
// 				DEBUG_TRY
// 				sprintf(szTempp, "<3_你对_1><%s><3_说_1><3_：_1>", szRName);
// 				strcat(szTempp, szTempWord );
// 				m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
// 				bSucMake	&=m_StrPacker.AddString(szSender);
// 				bSucMake	&=m_StrPacker.AddString(szReceiver);
// 				bSucMake	&=m_StrPacker.AddString(szEmotion);
// 				bSucMake	&=m_StrPacker.AddString(szTempp);
// 				bSucMake	&=m_StrPacker.AddString(szItemName);
// 				this->m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
				pUser->SendMsg(this);
// 				DEBUG_CATCH("私聊这里不应该出现错误吧。。。");
			}			
		}
		break;

// 	case _TXTATR_PRIVATE_OTHERLINE:
// 		{
// 			char szTemp[256] = "";
// 			char szTempp[256] = "";
// 			char szTempWord[256] = "";
// 			strcpy(szTempWord, szWords);
// 			bool bSucMake = true;
// 			CUserPtr pSender = UserManager()->GetUser(szSender);
// 			CUserPtr pReceiver = UserManager()->GetUser(szReceiver);
// 			if (pReceiver)
// 			{
// 				if (strcmp(szWords, "对方不在线，或在副本中。" )== 0)
// 				{
// 					pReceiver->SendSysMsg("对方不在线，或在副本中");
// 				}
// 				else
// 				{
// 					sprintf(szTemp, "<%s><%d_[%s]_%d>%s<%s><3_对你说_1><3_：_1>", szSex, SNAPSHOTNOTHING, szSERVER, iColor, szIdentity, szName);
// 					strcat(szTemp, szWords);
// 					m_pInfo->unTxtAttribute = _TXTATR_PRIVATE;
// 					m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
// 					bSucMake	&=m_StrPacker.AddString(szSender);
// 					bSucMake	&=m_StrPacker.AddString(szReceiver);
// 					bSucMake	&=m_StrPacker.AddString(szEmotion);
// 					bSucMake	&=m_StrPacker.AddString(szTemp);
// 					bSucMake	&=m_StrPacker.AddString(szItemName);
// 					this->m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// 					pReceiver->SendMsg(this);//sender对你说
// 				}
// 			}
// 			if (pSender)
// 			{
// 				sprintf(szTempp, "<3_你对_1><%s><3_说_1><3_：_1>", szRName);
// 				strcat(szTempp, szTempWord );
// 				m_pInfo->unTxtAttribute = _TXTATR_PRIVATE;
// 				m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
// 				bSucMake	&=m_StrPacker.AddString(szSender);
// 				bSucMake	&=m_StrPacker.AddString(szReceiver);
// 				bSucMake	&=m_StrPacker.AddString(szEmotion);
// 				bSucMake	&=m_StrPacker.AddString(szTempp);
// 				bSucMake	&=m_StrPacker.AddString(szItemName);
// 				this->m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
// 				pSender->SendMsg(this);
// 			}
// 		}
// 		break;
	case _TXTATR_CHATWITH://独孤求败[2008/12/23]1:1聊天窗口
		{
			int nStep = 0;
// 			DEBUG_TRY
			if(!::TalkStrCheck(szWords, true))		// true: enable new line character
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
				return ;
			}
			nStep = 1;
			CFriend* pSenderFriend = pUser->GetBeFriendByName(szReceiver);//用来获取接收方的ID，查询是否在对方黑名单中
			if (pSenderFriend && pSenderFriend->GetFriendType() == ENEMY_FRIEND_GROUP)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "你在对方的黑名单中，无法发送消息");
				return;
			}
// 			nStep = 2;
// 			if(pSenderFriend)
// 			{
// 				CFriend* pReceiverFriend = pUser->GetBeFriend(pSenderFriend->GetFriendID());
// 				if(pReceiverFriend)
// 				{
// 					if(pReceiverFriend->GetFriendType() == ENEMY_FRIEND_GROUP)//用户在对方的黑名单中
// 					{
// 						pUser->SendSysMsg(_TXTATR_TOPSHOW, "你在对方的黑名单中，无法发送消息");
// 						return;
// 					}
// 				}
// 			}
			nStep = 3;
			CUserPtr pTarget = UserManager()->GetUser(szReceiver);
			if(pTarget)
			{
				CFriend* pFriend = pTarget->GetFriendByName(szSender);
				if(pFriend)//用户是对方的好友
				{
					m_pInfo->dwItemID = pUser->GetLookFace();
					pTarget->SendMsg(this);
				}
				else
				{
					m_pInfo->dwUseGMToolPsw = 1;//未在好友列表中，通知客户端将用户加为临时好友
					m_pInfo->dwItemTypeID = pUser->GetID();
					m_pInfo->dwItemID = pUser->GetLookFace();
					pTarget->SendMsg(this);
					//pUser->AddBeFriend(0,pUser->GetID(), TEMP_FRIEND_GROUP);
				}
			}
			else//[2010-06-08 goto]不在线时改成留言
			{
				nStep = 4;
				//pUser->AddLeaveWord(szReceiver, szWords, szItemName);
				//MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, szReceiver);
				//pUser->SendSysMsg("对方不在线!");
				this->SetTransData(pUser->GetID());
				this->m_idSocket=pUser->GetSocketID();
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
			}
// 			DEBUG_CATCH2("CMsgTalk::_TXTATR_CHATWITH,nStep=%d", nStep);
		}
		break;
	case	_TXTATR_TEAM:
		{
			if (pUser)
			{
				if(!pUser->CheckMsgData(CHECKDATA_TEAMTALK,3000))
					return;
				if(!::TalkStrCheck(szWords, true))		// true: enable new line character
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
					return ;
				}
				CTeam* pTeam = pUser->GetTeam();
				if (pTeam)
					pTeam->BroadcastTeamMsg(this,pUser);
			}
		}
		break;
	case	_TXTATR_FRIEND:		//改为种族聊天
		{
			if (pUser)
			{
				if(!::TalkStrCheck(szWords, true))		// true: enable new line character
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
					return ;
				}
				//-------彭峰---2007/05/19-----种族聊天体力扣最大体力的30%----begin--------
				int nNeedSp = pUser->GetMaxSp() * 0.3 ;
				if( pUser->IsPM() )//GM单独处理
				{
					pUser->BroadcastFriendsMsg(this);
					//传递其它地图组
					MapGroup(PID)->TransmitMsg( this,pUser->GetPhyle() );
				}
				else if( pUser->GetSp() >= nNeedSp )
				{
					
					pUser->AddAttrib( _USERATTRIB_SP,( nNeedSp * -1 ), SYNCHRO_TRUE );//06.10.20王玉波添加,种族聊天扣体力			
					pUser->BroadcastFriendsMsg(this);
					//传递其它地图组
					MapGroup(PID)->TransmitMsg( this,pUser->GetPhyle() );
				}
				else
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(16005));//"体力不足以使用种族频道"
				}
				//-------------------------------end------------------------------------------
			}
		}
		break;
	case	_TXTATR_WORLDSERVER:
		{
			if (pUser)
			{
				CItem* pItem = pUser->GetItemByType(610024);
				if(pItem && pUser->SpendItem(pItem))
					UserManager()->BroadCastMsgAndReceiveName(szWords, /*pUser*/NULL, szEmotion, /*0xffffff*/m_pInfo->dwWordsColor, _TXTATR_WORLDSERVER,pUser,m_pInfo->dwItemID,m_pInfo->dwItemTypeID,szItemName,szReceiver);
				else
					pUser->SendSysMsg("你没有小喇叭，请去商城购买");
			}
		}
		break;

	case	_TXTATR_BIGHORN:
		{
			if (pUser)
			{
				CItem* pItem = pUser->GetItemByType(610040);
				if(pItem && pUser->SpendItem(pItem))
					UserManager()->BroadCastMsgAndReceiveName(szWords, /*pUser*/NULL, szEmotion, /*0xffffff*/m_pInfo->dwWordsColor, _TXTATR_BIGHORN,pUser,m_pInfo->dwItemID,m_pInfo->dwItemTypeID,szItemName,szReceiver);
				else
					pUser->SendSysMsg("你没有大喇叭，请去商城购买");
			}
		}
		break;
		
	case	_TXTATR_WORLD:
		{
			if (pUser)
			{
				if(!pUser->CheckMsgData(CHECKDATA_WORLDTALK,30000))
					return;

				UserManager()->BroadCastMsgAndReceiveName(szWords, /*pUser*/NULL, szEmotion, /*0xffffff*/m_pInfo->dwWordsColor, _TXTATR_WORLD,pUser,m_pInfo->dwItemID,m_pInfo->dwItemTypeID,szItemName,szReceiver);
				///所有广播的talk消息均要上发到世界线程进行关系服务器转发
//				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, ALLUSERS_NAME);
			}
		}
		break;
	case	_TXTATR_MAP:	//----------add by cz 加入地图说话功能--------------
		{
			if (pUser)
			{
				if(!pUser->CheckMsgData(CHECKDATA_MAPTALK,3000))
					return;

				if(!::TalkStrCheck(szWords, true))		// true: enable new line character
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
					return ;
				}

                CMsgTalk MapMsg;
			     MapMsg.Create(szSender,ALLUSERS_NAME,szWords,szEmotion,m_pInfo->dwWordsColor,_TXTATR_MAP,
					m_pInfo->unTxtStyle,m_pInfo->dwItemID,m_pInfo->dwItemTypeID,szItemName);
                 
				UserManager()->BroadcastMapMsg(pUser->GetMapID(), &MapMsg,/*pUser*/NULL);
		

			}


			break;
		}
	case	_TXTATR_SYNDICATE:
		{
			if (pUser)
			{
				if(!pUser->CheckMsgData(CHECKDATA_SYNTALK,3000))
					return;

				if(!::TalkStrCheck(szWords, true))		// true: enable new line character
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
					return ;
				}

				OBJID idSyn = pUser->GetSynID();
				CSyndicate* pSyn =SynManager()->QuerySyndicate(idSyn);
				if (pSyn)
				{
					//王玉波注释以下一行,军团基金不足时,不能发军团信息限制......
			/*		if(pSyn->GetInt(SYNDATA_MONEY) < SYN_MONEY_BASELINE)
					{
						pUser->SendSysMsg(STR_TOO_LOWER_SYN_MONEY);
						break;
					}*/
					pSyn->BroadcastSynMsg(this);

					m_pInfo->idTransmit	= pSyn->GetID();
//					MapGroup(PID)->QueryIntraMsg()->TransmitMsg(this, GetSocketID(), GetNpcID());

					///尝试转发到关系服务器处理
					m_pInfo->dwUseGMToolPsw=pSyn->GetID();///将帮派id存放在道具变量中发送
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(this,pUser->GetID());

				}
			}
		}
		break;
	case	_TXTATR_LEAVEWORD:		//留言
		{
			if (pUser)
			{
				if(!::TalkStrCheck(szWords, true))		// true: enable new line character
				{
					//pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
					return ;
				}
				pUser->AddLeaveWord(szReceiver, szWords, szItemName);
			}
		}
		break;
// 	case	_TXTATR_SYNANNOUNCE:
// 		{
// 			LPCTSTR pAnnounce = szWords;
// 			if(!pUser || pUser->GetSynID() == ID_NONE )
// 				return ;
// 			if(pUser->GetSynRankShow() != RANK_LEADER  && pUser->GetSynRankShow() != RANK_SUBLEADER)
// 			if(!::TalkStrCheck(szWords, true))		// true: enable new line character
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
// 				return ;
// 			}
// 
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(pUser->GetSynID());
// 			IF_OK(pSyn)
// 			{
// //				pSyn->QueryModify()->SetTenet(pAnnounce);
// //				pSyn->BroadcastSubSynMsg(GetSocketID(), GetNpcID(), pAnnounce, NULL, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE);
// 			}
// 		}
// 		break;
// 	case	_TXTATR_CRYOUT:
// 		{
// 			//[游途道标 2008/12/29]只设置不发送聊天信息
// 			/*if (pUser && pUser->QueryBooth())
// 			{
// 				if(!::TalkStrCheck(szWords, true))
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12016));//STR_INVALID_MSG
// 					return ;
// 				}
// 
// 				pUser->QueryBooth()->SetCryOut(szWords);
// 				pUser->BroadcastRoomMsg(this, false);// add by arhun
// 			}*/
// 			if (pUser)
// 			{
// 				if(!::TalkStrCheck(szWords, true))
// 				{
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12016));//STR_INVALID_MSG
// 					return;
// 				}
// 				pUser->SetCryOut(szWords);
// 			}
// 		}
// 		break;
	case	_TXTATR_MSG_TRADE:
	case	_TXTATR_MSG_FRIEND:
	case	_TXTATR_MSG_TEAM:
	case	_TXTATR_MSG_SYN:
	case	_TXTATR_MSG_OTHER:
		{
			CUser* pUser = UserManager()->GetUser(this);
			if(!pUser)
				return ;
			if(pUser->GetLev() < LOGIN_FREE_LEVEL)
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13006));//STR_LOW_LEVEL
				return ;
			}
			if(!::TalkStrCheck(szWords, true))		// true: enable new line character
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
				return ;
			}

				if(m_pInfo->unTxtAttribute == _TXTATR_MSG_SYN)
			{
				OBJID idSyn = pUser->GetSynID();
				if(idSyn == ID_NONE)
					return ;

				SetTransData(idSyn);
			}

			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);
		}
		break;
	case	_TXTATR_MSG_SYSTEM:
		{
			ASSERT(!"_TXTATR_MSG_OTHER");
		}
		break;

	case	_TXTATR_SERVE:
		{
			CUser* pUser = UserManager()->GetUser(this);
			if (!pUser)
				return;

			if(!::TalkStrCheck(szWords, true))		// true: enable new line character
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
				return ;
			}

			if (pUser->IsGM())	// game service
			{
				CUserPtr pTarget = UserManager()->GetUser(szReceiver);
				if (pTarget)
					pTarget->SendMsg(this);
				else
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, szReceiver);
			}
			else				// client ask service
			{
				// 发给服务频道GM的消息全部通过世界核心处理，以保证唯一性和正确性 -- zlong 2004.5.18
//				CUserPtr pTarget = UserManager()->GetGM();
//				if (pTarget)
//					pTarget->SendMsg(this);
//				else
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);	//, szReceiver);
			}
		}
		break;

	case	_TXTATR_REJECT:
		{
			if(!::TalkStrCheck(szWords, true))		// true: enable new line character
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
				return ;
			}
			CUserPtr pTarget = UserManager()->GetUser(szReceiver);
			if(pTarget)
			{
				pTarget->SendMsg(this);
				// 上面是从信仰继承过来的处理方式
				// 存在一个BUG，就是没有清除CUser的请求，需要以下修改：

				if (strcmp(szWords, "a") == 0)	// REJECT_FRIEND
					pTarget->FetchApply(CUser::APPLY_FRIEND, pUser->GetID());
				else if (strcmp(szWords, "b") == 0)	// REJECT_TRADE
					pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID());
				else if (strcmp(szWords, "c") == 0)	// REJECT_TEAM
					pTarget->FetchApply(CUser::APPLY_TEAMAPPLY, pUser->GetID());
				else if (strcmp(szWords, "d") == 0)	// REJECT_TEACHERAPPLY
					pTarget->FetchApply(CUser::APPLY_TEACHERAPPLY, pUser->GetID());
				else if (strcmp(szWords, "e") == 0)	// REJECT_STUDENGAPPLY
					pTarget->FetchApply(CUser::APPLY_STUDENTAPPLY, pUser->GetID());
				else if (strcmp(szWords, "f") == 0)//20070404修罗:拒绝加入军团.之前对方申情
					pTarget->FetchApply(CUser::APPLY_JOIN_SYN, pUser->GetID());
				else if (strcmp(szWords, "g") == 0)//20070404修罗:拒绝加入军团.之前对方邀请
					pTarget->FetchApply(CUser::APPLY_INVITE_JOIN_SYN, pUser->GetID());
//				else if (strcmp(szWords, "h") == 0)//-------彭峰---2007/05/31-----拒绝组队
// 					pTarget->FetchApply(CUser::APPLY_TEAM_NO, pUser->GetID() );
				
			}
			else
			{
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, szReceiver);
			}
		}
		break;
	case _TXTATR_MYSYSTEM: //用于消息的转发
		{
			//通过类型来改变属性
			if (pUser)
			{
				if(!::TalkStrCheck(szWords, true))		// true: enable new line character
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
					return ;
				}
				m_pInfo->unTxtAttribute = m_pInfo->unTxtStyle;
				CUserPtr pTarget = UserManager()->GetUser(szReceiver);
				if(pTarget)
					pTarget->SendMsg(this);
				else
				  MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, szReceiver);

			}
		}
		break;
	case	_TXTATR_BOOTHNAME:
		{
			OBJID idBooth = m_pInfo->dwWordsColor;
			CBooth* pBooth = MapManager()->QueryBooth(idBooth);
			if (pBooth)
			{
				pBooth->ChangeName(szWords,true);
			}
		}
		break;
	case	_TXTATR_BOOTHDES:
		{
			OBJID idBooth = m_pInfo->dwWordsColor;
			CBooth* pBooth = MapManager()->QueryBooth(idBooth);
			if (pBooth)
			{
				pBooth->SetCryOut(szWords,true);
			}
		}
		break;
	case	_TXTATR_BOOTHLEAVEWORD:
		{
			OBJID idBooth = m_pInfo->dwWordsColor;
			CBooth* pBooth = MapManager()->QueryBooth(idBooth);
			if (pBooth)
			{
				pBooth->AddLeaveWord(szSender,szWords,true,pUser);
			}
		}
		break;
	case	_TXTATR_PRO:
		{
// 			CUserManager::Iterator pListener = UserManager()->NewEnum();
// 			while(pListener.Next())
// 			{
// 				if(pListener && pListener->CanBeSee()==false)
// 					continue;
// 
// 				if(pListener && (pListener->GetInfo()->FirJob == m_pInfo->dwItemTypeID || pListener->GetInfo()->SecJob == m_pInfo->dwItemTypeID)
// 					/*&& pListener->GetID() != pUser->GetID()*/)
// 				{
// 					pListener->SendMsg(this);
// 				}
// 			}
			UserManager()->BroadcastProMsg(m_pInfo->dwItemTypeID,this);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(this, pUser->GetID());
		}
		break;
	case	_TXTATR_FATI:
		{
// 			for (int i=0; i<sizeof(szReceiver); i++)
// 			{
// 				if (szReceiver[i] > 0)
// 				{
// 					pUser->SendSysMsg("名字填写错误！");
// 					return;
// 				}
// 			}
			if(m_pInfo->unTxtStyle == 1)
			{
				pUser->SetFatigue(1, true);	//通过
				pUser->SendSysMsg(_TXTATR_FATI, "成功");
				pUser->SetFatigueInKey(1);
// 				string	idhigh, idlow, id;
// 				id = szWords;
// 				if(id.length() == 18)
// 				{
// 					idhigh = id.substr(0, 9);
// 					idlow  = id.substr(9, 9);
// 				}
// 				if (id.length() == 15)
// 				{
// 					idhigh = id.substr(0, 9);
// 					idlow  = id.substr(9, 6);
// 				}
// 
// 				OBJID Idhigh = atoi(idhigh.c_str());
// 				OBJID Idlow  = atoi(idlow.c_str());
//				pUser->m_pUserKey.SetStr(SYSKEY_IDHIGH, szWords, 20, true);
//				pUser->m_pUserKey.SetInt(SYSKEY_IDLOW, Idlow);
//				pUser->m_pUserKey.SetRealName(szReceiver);
			}
			else
				pUser->SendSysMsg(_TXTATR_FATI, "失败");
		}
		break;
	case	_TXTATR_FORGMTOOL:
		{
			if (!pUser->IsPM())
				return;
			char szCmd[32];
			char szName[32];
			UserManager()->BroadCastMsgAndReceiveName(szWords, /*pUser*/NULL, szEmotion, /*0xffffff*/m_pInfo->dwWordsColor, _TXTATR_FORGMTOOL,pUser,m_pInfo->dwItemID,m_pInfo->dwItemTypeID,szItemName,szReceiver);			
//modify code by 林德渊 begin 2011.5.5
			int MaxStopTime = 0;
			int StopTime = 0;
			sscanf(szWords, "%s %s %d %d", szCmd, szName, &MaxStopTime, &StopTime);
//modify code by 林德渊 end
			if(strcmp(szCmd, "kickout") == 0)
			{
				if (pUser->IsPM()/* && strcmp(szEmotion , "JoyCc") == 0*//*&& dwUseGMToolPsw == m_pInfo->dwUseGMToolPsw*/ )
				{
					CUserPtr pTarget = UserManager()->GetUser(szName);
					if (pTarget)
					{
						UserManager()->KickOutSocket(pTarget->GetSocketID(), "GM /kickout");
					}
				}
			}
			if(strcmp(szCmd, "stopsay") == 0)
			{
				CUserPtr pTarget = UserManager()->GetUser(szName);
				if (pTarget)
				{
					//					CRole::AttachStatus( pTarget->QueryRole(),STATUS_STOPSAY,6000,6000 );//power此时无意义
					//modify code by 林德渊 begin 2011.5.5
					CRole::AttachStatus( pTarget->QueryRole(),STATUS_STOPSAY,MaxStopTime,StopTime );//power此时无意义
					//modify code by 林德渊 end
				}
			}
//modify code by 林德渊 begin 2011.5.13
			if(strcmp(szCmd, "sendmail") == 0)
			{
				char mail[1024] = {0};
				sscanf(szWords, "%s %s %s", szCmd, szName, mail);
				CUserPtr pTarget = UserManager()->GetUser(szName);
				if (pTarget)
				{
					pTarget->SendEmail(szName,mail,pTarget);
				}
				else
				{
//					CUserPtr pTarget1 = UserManager()->GetUser(szSender);
/*					pTarget1->SendEmail(szName,mail,pTarget1);
//					PostOffice()->SendSysMail(pTarget->GetID(), szSender, mail,0, 0);
					CUserPtr pUser = g_UserManager.GetUser(szName);
					CMsgTalk msg;
					if (msg.Create("系统", szSender, szName, NULL, 0xFFFFFFFF, _TXTATR_MAYBEMAIL))
						pTarget1->SendMsg(&msg);*/
					LeaveWord()->InsertWords(szSender, szName, mail, "FCFCFC");
				}
			}
//modify code by 林德渊 end
		}
		break;
	case _TXTATR_WARGAME://竞技场阵营频道
		{
			if (pUser)
			{
				IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
				if(!pStatus)
					return;

				if(!::TalkStrCheck(szWords, true))		// true: enable new line character
				{
					pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
					return ;
				}
				
                CMsgTalk msg;
				if(msg.Create(szSender,ALLUSERS_NAME,szWords,szEmotion,m_pInfo->dwWordsColor,_TXTATR_WARGAME,
					m_pInfo->unTxtStyle,m_pInfo->dwItemID,m_pInfo->dwItemTypeID,szItemName))
					WargameManager()->BroadCastCampMsg(pStatus->GetPower(), pUser->GetID(), &msg);
			}
		}
		break;
	default:
		{
			if(!::TalkStrCheck(szWords, true))		// true: enable new line character
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13005));//STR_INVALID_CHARACTER
				return ;
			}
			//pRole->BroadcastRoomMsg()
			
			///softworms,普通消息不发给怪物.
			pRole->BroadcastRoomMsg(this, EXCLUDE_SELF,EM_SENDNPCFLAG_FALSE);
		}
		break;
	}
	}
//  	DEBUG_CATCH2("MsgChannel:%d ERROR!", m_pInfo->unTxtAttribute);
	catch(...)
	{
		LogSave("MAPGROUPKERNEL CMsgTalk::Process crash!");
		CMsgTalk::Dump(GetBuf(),GetSize());
	}
}
