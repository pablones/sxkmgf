// MsgSuperWeaponSoul.cpp: implementation of the CMsgSuperWeaponSoul class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgSuperWeaponSoul.h"
#include "NetMsg.h"
#include "MsgSuperItem.h"
#include "MapGroup.h"
#include "ItemType.h"
#include "user.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSuperWeaponSoul::CMsgSuperWeaponSoul()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgSuperWeaponSoul::~CMsgSuperWeaponSoul()
{

}

CMsgSuperWeaponSoul::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_GMSUPERWEAPONSOULCREATE != this->GetType())
		return false;

	return true;
}

BOOL CMsgSuperWeaponSoul::Create(ST_WEAPONSOULDATA* WepeonSoulItemInfo, USHORT usAction,USHORT usSuc)
{
	this->Init();

	// fill info now
	m_unMsgSize		= sizeof(MSG_Info);
	m_unMsgType		= _MSG_GMSUPERWEAPONSOULCREATE;
	memcpy(&m_pInfo->unWeaponSoulItemInfo, WepeonSoulItemInfo, sizeof(ST_WEAPONSOULDATA));
	m_pInfo->usAction = usAction;
	m_pInfo->usSuc = usSuc;

	return true;
}


void CMsgSuperWeaponSoul::Process(void* pInfo)
{
	IRole* pRole = RoleManager()->QueryRole(this);

	CUser* pUser = NULL;
	pRole->QueryObj(OBJ_USER, IPP_OF(pUser));		//? pUser may be null

	switch(m_pInfo->usAction)
	{
	case ACTION_WEAPONSOULERROR:
		{
			
		}
		break;
	case ACTION_GETSUPERWEAPONSOULINFO:
		{
			ST_WEAPONSOULDATA tempSuperWeaponSoulInfo;
			ASSERT(CWeaponSoulData::GetInfo(m_pInfo->unWeaponSoulItemInfo.idtype, &tempSuperWeaponSoulInfo,"GM制造"));
			CMsgSuperWeaponSoul msg;
			if (msg.Create(&tempSuperWeaponSoulInfo,ACTION_GETITEMINFO,1))
			{
				SendMsg(&msg);
			}
		
		}
		break;
	case ACTION_CREATESUPERWEAPONSOUL:
		{	
			char szLog[1024] = {0};
			int nWeaponSoulItemType = m_pInfo->unWeaponSoulItemInfo.idtype+WEAPONSOULTYPEHEAD;
			if(pUser->SuperAwardWeaponSoul(nWeaponSoulItemType,&m_pInfo->unWeaponSoulItemInfo))
				pUser->SendSysMsg(g_objStrRes.GetStr( 15032 ));//"[您创造了自定义物品。]"
			sprintf(szLog,g_objStrRes.GetStr( 15033 ),pUser->GetName(),m_pInfo->unWeaponSoulItemInfo.ID,m_pInfo->unWeaponSoulItemInfo.idtype);
 			GmLogSave(szLog);//"%s创造了自定义物品。ID为:%d.IDType为:%d"

		}
		break;
	default:
		break;

	}

}