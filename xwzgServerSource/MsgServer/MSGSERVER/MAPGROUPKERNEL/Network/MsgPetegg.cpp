// MsgPetegg.cpp: implementation of the CMsgPetegg class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgPetegg.h"
#include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgPetegg::CMsgPetegg()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgPetegg::~CMsgPetegg()
{

}

BOOL CMsgPetegg::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_PETEGG != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgPetegg::Create(OBJID idUser, OBJID idItem, UCHAR ucAction, USHORT usType, LONG nNeedSec, LPCTSTR szName)
{
	CHECKF(idUser);
	CHECKF(idItem);
	CHECKF(szName);

	Init();

	m_pInfo->idUser		= idUser;
	m_pInfo->idItem		= idItem;
	m_pInfo->nNeedSec	= nNeedSec;
	m_pInfo->usType		= usType;
	m_pInfo->ucAction	= ucAction;
	m_pInfo->ucAmount	= 0;
	SafeCopy(m_pInfo->szName, szName, _MAX_NAMESIZE);
	
	m_unMsgType	=_MSG_PETEGG;
	m_unMsgSize	=sizeof(MSG_Info);
	
	return true;
}

BOOL CMsgPetegg::Append(ST_PETEGG pInfo)
{
	CHECKF( (sizeof(MSG_Info) + (m_pInfo->ucAmount-1)*sizeof(ST_PETEGG)) < MAX_PACKETSIZE);
	
	memcpy(&m_pInfo->setPet[m_pInfo->ucAmount], &pInfo, sizeof(ST_PETEGG));
	++m_pInfo->ucAmount;
	
	m_unMsgSize	+= sizeof(ST_PETEGG);
	
	return TRUE;
}

void CMsgPetegg::Process(void* pInfo)
{
	DEBUG_TRY
	CUser* pUser = UserManager()->GetUser(this);
	
	if(!pUser)
		return;
	
	switch(m_pInfo->ucAction)
	{
	case PETEGG_QUERYINFO://查询可选宠物
		{
// 			CHECK(m_pInfo->idItem);
// 			
// 			CItem* pItemEgg = pUser->GetItem(m_pInfo->idItem, false, true);
// 			if(!pItemEgg)
// 				return;
// 
// 			if(pUser->QueryPetegg())
// 				pUser->QueryPetegg()->SendToClient(pItemEgg, PETEGG_QUERYINFO);
		}
		break;
	case PETEGG_QUICKFLUSH://使用道具刷新
		{
			CHECK(m_pInfo->idItem);

			CItem* pItemEgg = pUser->GetItem(m_pInfo->idItem, false, true);
			if(!pItemEgg)
				return;

			CItem* pItem = pUser->GetItemByType(630032);
			if(!pItem)
				return;

//modify code by 林德渊 begin 2011.5.23
			if( pItem->IsCostlyItem() )
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", pUser->GetID(), pUser->GetName(), 1, pItem->GetTypeID());
//modify code by 林德渊 end
			
			if(pUser->QueryPetegg() && pUser->SpendItem(pItem))
			{
				pUser->QueryPetegg()->FlushItemAllPetData(pItemEgg, true);
				pUser->QueryPetegg()->SendToClient(pItemEgg, PETEGG_QUERYINFO);
			}
		}
		break;
	case PETEGG_GETPET://获取宠物
		{
			CHECK(m_pInfo->idItem);
			CHECK(m_pInfo->setPet[0].idData);
			CHECK(m_pInfo->setPet[0].ucGetType);
			CHECK(m_pInfo->setPet[0].usGrow);

			CItem* pItemEgg = pUser->GetItem(m_pInfo->idItem, false, true);
			if(!pItemEgg)
				return;

			if(pUser->QueryPetegg())
				pUser->QueryPetegg()->CreatePetByEgg(m_pInfo->setPet[0].idData, pItemEgg, m_pInfo->setPet[0].ucGetType, m_pInfo->setPet[0].usGrow);
		}
		break;
	default:
		{
			char strTemp[128] = "";
			sprintf(strTemp,"CMsgPetegg::Process switch(m_pInfo->ucAction) default...m_pInfo->ucAction=%d,userid:%u,username:%s",m_pInfo->ucAction,pUser->GetID(), pUser->GetName());
			::LogSave(strTemp);
		}
		break;
	}
	DEBUG_CATCH2("CMsgPetegg::Process action=%d", m_pInfo->ucAction);
}