// MsgWargame.cpp: implementation of the CMsgWargame class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgWargame.h"
#include "wargame.h"
#include "MapGroup.h"
#include "WargameManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern int g_SpecilLine;

CMsgWargame::CMsgWargame()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_unMsgType = _MSG_WARGAME;
	m_unMsgSize=sizeof(MSG_Info);
}

CMsgWargame::~CMsgWargame()
{

}

BOOL CMsgWargame::Create(char *pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return FALSE;

	if(_MSG_WARGAME != this->GetType())
		return FALSE;

	return TRUE;
}

BOOL CMsgWargame::Create(OBJID idUser, USHORT usAction, UCHAR ucPage)
{
	if (idUser==ID_NONE)
		return FALSE;

	if(usAction == WARGAME_LINEBORAD)
		CHECKF(ucPage);

	// init
	this->Init();
	
	// TODO: init data
	m_pInfo->idUser		= idUser;
	m_pInfo->usAction	= usAction;
	m_pInfo->ucPage		= ucPage;
	m_pInfo->ucAmount	= 0;
	
	m_unMsgType	=_MSG_WARGAME;
	m_unMsgSize	=sizeof(MSG_Info);
	
	return TRUE;
}

BOOL CMsgWargame::Append(WARGAME_PLAYERSCORE info)
{
	CHECKF( (sizeof(MSG_Info) + (m_pInfo->ucAmount - 1)*sizeof(WARGAME_PLAYERSCORE)) < MAX_PACKETSIZE);
	
	memcpy(&m_pInfo->stUserList[m_pInfo->ucAmount], &info, sizeof(WARGAME_PLAYERSCORE));
	++m_pInfo->ucAmount;
	
	m_unMsgSize	+= sizeof(WARGAME_PLAYERSCORE);
	
	return TRUE;
}

BOOL CMsgWargame::Append(OBJID idUser, USHORT usLev, int nScore, UCHAR ucCamp, UCHAR ucVipLev, DWORD dwProfession, LPCTSTR szName)
{
	CHECKF( (sizeof(MSG_Info) + (m_pInfo->ucAmount - 1)*sizeof(WARGAME_PLAYERSCORE)) < MAX_PACKETSIZE);
	
	m_unMsgSize += sizeof(WARGAME_PLAYERSCORE);

	WARGAME_PLAYERSCORE info = {0};
	info.idPlayer = idUser;
	info.usLev = usLev;
	info.nScore = nScore;
	info.ucCamp = ucCamp;
	info.ucVipLev = ucVipLev;
	info.dwProfession = dwProfession;
	SafeCopy(info.szName, szName, _MAX_NAMESIZE);

	memcpy(&m_pInfo->stUserList[m_pInfo->ucAmount], &info, sizeof(WARGAME_PLAYERSCORE));
	m_pInfo->ucAmount++;
	
	return TRUE;
}

void CMsgWargame::Process(void *pInfo)
{
	DEBUG_TRY
	CUser* pUser = UserManager()->GetUser(this);
	
	if(!pUser)//其他线路过来的消息
	{
		ProcessLineMsg(pInfo);
		return;
	}
	
	switch(m_pInfo->usAction)
	{
// 	case WARGAME_SIGNIN://报名
// 		{
// 
// 		}
// 		break;
	case WARGAME_SCORERANK://积分榜
		{
			IStatus* pStatus = pUser->QueryStatus(STATUS_WARGAME);
			if(!pStatus)
				return;

			WargameManager()->SendScoreRank(pStatus->GetPower(), pUser);
		}
		break;
	case WARGAME_ENTER://进入竞技场
		{
			CHECK(m_pInfo->stUserList[0].idPlayer);
			if(pUser->QueryWarGame() && pUser->QueryWarGame()->HasArenaMask())
				return;

			//已经在竞技场内
			if(pUser->QueryStatus(STATUS_WARGAME) || CWargame::IsWarGameMap(pUser->GetMapID()))
				return;

			if(WargameManager()->IsMaxPlayer(m_pInfo->stUserList[0].idPlayer))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "进入人数已满");
				return;
			}
			//切换到专线

			pUser->EnterWarGame(m_pInfo->stUserList[0].idPlayer);
		}
		break;
	default:
		{
			char strTemp[128] = "";
			sprintf(strTemp,"CMsgWargame::Process switch(m_pInfo->usAction) default...m_pInfo->usAction=%d,userid:%u,username:%s",m_pInfo->usAction,pUser->GetID(), pUser->GetName());
			::LogSave(strTemp);
		}
		break;
	}
	DEBUG_CATCH2("CMsgWargame::Process action=%d", m_pInfo->usAction);
}

void CMsgWargame::ProcessLineMsg(void *pInfo)
{
	DEBUG_TRY
	switch(m_pInfo->usAction)
	{
	case WARGAME_LINEBORAD:
		{
			switch(m_pInfo->ucPage)
			{
			case WARGAMELINEBORAD_LOADMAP:
				{
					if(g_SpecilLine!=1)
					{
						if(m_pInfo->idUser == 6)
							WargameManager()->LoadMap();
					}
				}
				break;
			case WARGAMELINEBORAD_SIGNIN:
				{
					CHECK(m_pInfo->stUserList[0].usRank);

					WargameManager()->Signup(m_pInfo->stUserList[0].usRank, &m_pInfo->stUserList[0]);
				}
				break;
			case WARGAMELINEBORAD_ENTER:
				{
					CHECK(m_pInfo->stUserList[0].usRank);
					
					WargameManager()->SynchroPlayerAmount(m_pInfo->stUserList[0].usRank, &m_pInfo->stUserList[0], 1);
				}
				break;
			case WARGAMELINEBORAD_LEAVE:
				{
					CHECK(m_pInfo->stUserList[0].usRank);
					
					WargameManager()->SynchroPlayerAmount(m_pInfo->stUserList[0].usRank, &m_pInfo->stUserList[0], -1);
				}
				break;
			case WARGAMELINEBORAD_START:
				{
					CHECK(m_pInfo->idUser);

					WargameManager()->BroadCaseEnterMsg(m_pInfo->idUser);
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		{
			char strTemp[128] = "";
			sprintf(strTemp,"CMsgWargame::ProcessLineMsg switch(m_pInfo->usAction) default...m_pInfo->usAction=%d",m_pInfo->ucPage);
			::LogSave(strTemp);
		}
		break;
	}
	DEBUG_CATCH2("CMsgWargame::ProcessLineMsg action=%d", m_pInfo->ucPage)
}
