// MsgVipList.cpp: implementation of the CMsgVipList class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgVipList.h"
#include "MapGroup.h"
#include "../VipList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgVipList::CMsgVipList()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgVipList::~CMsgVipList()
{

}

BOOL CMsgVipList::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_VIPLIST != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgVipList::Create(OBJID idUser, USHORT usAction, UCHAR ucPage)
{	
	CHECKF(usAction == 1 || usAction == 2);
	if (idUser==ID_NONE)
		return false;
	// init
	this->Init();
	
	// TODO: init data
	m_pInfo->idUser		= idUser;
	m_pInfo->usAction	= usAction;
	m_pInfo->ucPage		= ucPage;
	m_pInfo->ucAmount	= 0;
	
	m_unMsgType	=_MSG_VIPLIST;
	m_unMsgSize	=sizeof(MSG_Info);
	
	return true;
}

BOOL CMsgVipList::Append(OBJID idUser, OBJID idMap, USHORT usLev, UCHAR ucLine, UCHAR ucVipLev, LPCTSTR szName)
{
	CHECKF( (sizeof(MSG_Info) + (m_pInfo->ucAmount - 1)*sizeof(ST_VIPLIST)) < MAX_PACKETSIZE);

	m_unMsgSize += sizeof(ST_VIPLIST);
	
	ST_VIPLIST info = {0};

	info.idUser = idUser;
	info.idMap = idMap;
	info.usLev = usLev;
	info.ucLine = ucLine;
	info.ucVipLev = ucVipLev;
	SafeCopy(info.szName, szName, _MAX_NAMESIZE);

	memcpy(&m_pInfo->setMember[m_pInfo->ucAmount], &info, sizeof(ST_VIPLIST));
	m_pInfo->ucAmount++;

	return TRUE;
}

BOOL CMsgVipList::Append(ST_VIPLIST pInfo)
{
	CHECKF( (sizeof(MSG_Info) + (m_pInfo->ucAmount - 1)*sizeof(ST_VIPLIST)) < MAX_PACKETSIZE);
	
	memcpy(&m_pInfo->setMember[m_pInfo->ucAmount], &pInfo, sizeof(ST_VIPLIST));
	++m_pInfo->ucAmount;
	
	m_unMsgSize	+= sizeof(ST_VIPLIST);
	
	return TRUE;
}

void CMsgVipList::Process(void* pInfo)
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
	case VIPLIST_QUERYINFO:
		{
			CHECK(m_pInfo->ucPage);
			CHECK(m_pInfo->ucAmount);

			ST_VIPLIST arrData[20] = {0};
			int nDataSize = g_VipListMgr.QueryInfo(arrData, m_pInfo->ucPage, m_pInfo->ucAmount, pUser->GetID());

			int nTotalAmount = g_VipListMgr.GetVipAmount();
			if(pUser->IsVipActive())
				nTotalAmount--;//列表不包括自己

			UCHAR nPages = nTotalAmount%m_pInfo->ucAmount==0 ? nTotalAmount/m_pInfo->ucAmount : (nTotalAmount/m_pInfo->ucAmount) + 1;

			CMsgVipList msg;
			if(msg.Create(m_pInfo->idUser, VIPLIST_QUERYINFO, nPages))
			{
				for(int i=0; i<nDataSize; i++)
					msg.Append(arrData[i]);

				pUser->SendMsg(&msg);
			}
		}
		break;
	case 3://日志太多
		break;
	default:
		{
			char strTemp[128] = "";
			sprintf(strTemp,"CMsgVipList::Process switch(m_pInfo->usAction) default...m_pInfo->usAction=%d,userid:%u,username:%s",m_pInfo->usAction,pUser->GetID(), pUser->GetName());
			::LogSave(strTemp);
		}
		break;
	}
	DEBUG_CATCH2("CMsgVipList::Process action=%d", m_pInfo->usAction);
}

void CMsgVipList::ProcessLineMsg(void* pInfo)
{
	DEBUG_TRY
	switch(m_pInfo->usAction)
	{
	case VIPLIST_LINEBORAD:
		{
			switch(m_pInfo->ucPage)
			{
			case VIPLINEBORAD_ONLINE:
				{
					CHECK(m_pInfo->ucAmount);
					g_VipListMgr.AddVipUser(m_pInfo->setMember[0]);
				}
				break;
			case VIPLINEBORAD_OFFLINE:
				{
					CHECK(m_pInfo->idUser);
					g_VipListMgr.DelVipUser(m_pInfo->idUser);
				}
				break;
			case VIPLINEBORAD_VASLIMIT:
				{
					CHECK(m_pInfo->setMember[0].idUser);//这个字段里是物品type
					VASItem()->SetLimitItemAmount(m_pInfo->setMember[0].idUser, m_pInfo->setMember[0].idMap, false);
				}
				break;
			}
		}
		break;
	case 3://日志太多
		break;
	default:
		{
			char strTemp[128] = "";
			sprintf(strTemp,"CMsgVipList::ProcessLineMsg switch(m_pInfo->usAction) default...m_pInfo->usAction=%d",m_pInfo->ucPage);
			::LogSave(strTemp);
		}
		break;
	}
	DEBUG_CATCH2("CMsgVipList::ProcessLineMsg action=%d", m_pInfo->ucPage)
}