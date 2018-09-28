// MsgMapItem.cpp: interface for the CMsgMapItem class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgMapItem.h"
#include "MapGroup.h"
#include "MapItem.h"
#include "MapTrap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgMapItem::CMsgMapItem()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgMapItem::~CMsgMapItem()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMapItem::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_MAPITEM != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMapItem::Create(int nAction, CMapItem* pMapItem)
{
	CHECKF(pMapItem && nAction);

	// init
	this->Init();

	m_pInfo->ReTime = pMapItem->GetReTime();
	m_pInfo->nAction	= nAction;
	m_pInfo->id			= pMapItem->GetID();
	m_pInfo->nPosX		= pMapItem->GetPosX();
	m_pInfo->nPosY		= pMapItem->GetPosY();
	m_pInfo->nItemAmount= pMapItem->m_nItemAmount;
	if(m_pInfo->nItemAmount > 8)
		m_pInfo->nItemAmount = 8;
	for(int i = 0 ; i < m_pInfo->nItemAmount ; i ++)
		m_pInfo->idType[i] = pMapItem->m_setItemType[i];

	m_unMsgType	=_MSG_MAPITEM;
	m_unMsgSize	=sizeof(MSG_Info);
	return true;
}

/*/////////////////////////////////////////////////////////////////////
BOOL CMsgMapItem::Create(int nAction, OBJID idUser)
{
	// init
	this->Init();

	// fill info now
	m_pInfo->nAction	= nAction;
	m_pInfo->id			= idUser;

	m_unMsgType	=_MSG_MAPITEM;
	m_unMsgSize	=sizeof(MSG_Info);

	return true;
}*/

//////////////////////////////////////////////////////////////////////
BOOL CMsgMapItem::Create(OBJID id, int nPosX, int nPosY, int nAction)
{
	if (id == ID_NONE)
		return false;

	// init
	this->Init();

	// fill info now
	m_pInfo->nAction	= nAction;
	m_pInfo->id			= id;
	m_pInfo->nPosX		= nPosX;
	m_pInfo->nPosY		= nPosY;

	m_unMsgType	= _MSG_MAPITEM;
	m_unMsgSize	= sizeof(MSG_Info);

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgMapItem::Process(void *pInfo)
{
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser || !pUser->CanBeSee())
		return;

	if(!pUser->IsAlive())
	{
		pUser->SendSysMsg(g_objStrRes.GetStr(12069));//STR_DIE
		return ;
	}

	switch(m_pInfo->nAction)
	{
// 	case MSGMAPITEM_PICK:			//06.6.13王玉波修改
// 		{
// 			if (pUser->SynPosition(m_pInfo->nPosX, m_pInfo->nPosY, 18/*0*/))
// 			{
// 				pUser->PickMapItem(m_pInfo->id,m_pInfo->idType[0]);
// 			}
// 			else
// 			{
// 			//	UserManager()->KickOutSocket(m_idSocket, "ITEM_PICK SynPosition");
// 			}
// 		}
// 		break;
		//20060919修罗:范围内拾取.无需判断hero坐标与item坐标之间的距离.
		//-----------------------------------------
	case MSGMAPITEM_PICK_IN_RANGE:
		{
			//判断是否摆摊
			if (pUser->QueryBooth())
			{
				return;
			}

			pUser->PickMapItem(m_pInfo->id,m_pInfo->idType[0],15);//3是写死的.与客户端一致即可.	
		}
		break;
		//-----------------------------------------
	default:
		{
			ASSERT(!"switch");
		}
	}
	return;

#ifdef _MSGDEBUG
	::LogMsg("Process CMsgMapItem, id:%u", m_pInfo->id);
#endif
}
