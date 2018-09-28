// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////
#include "..\stdafx.h"
#include "Network/allmsg.h"
#include "User.h"
#include "MapGroup.h"
#include "Network/netmsg.h"
#include "ItemType.h"
#include "MapItem.h"
#include "Package.h"
#include "EconomySystem.h"
#include "ResourceWar.h"
#include "EquipCreate.h"		//20061218修罗
#include "SKPostOffice.h"
#include "DataInspectManage.h"
#include "SettledAction.h"// 王萌 [1/29/2008]
#include "MapInstanceMgr.h"
#include "LoverHome.h"
#include "PortableShopManager.h"
//modify code by 林德渊 begin 2011.5.30
#include ".\\..\\msgserver\\mapgroupkernel\\Network\\MsgVasLv.h"
//modify code by 林德渊 end
const int	MINI_ADDLIFE_DRAG				= 100;
//const int	ADDLIFE_FIRST_PERCENT			= 40;
//const int	ADDLIFE_NEXT_PERCENT			= 20;
const int	STORAGE_NPC_ID					= 1005;		//? 信仰仓库互通
const int	_MINE_RANGE						= 1;		// 挖矿距离

const int   STORAGE_NPC_ID_BED_MAT1         = 1004;
const int   STORAGE_NPC_ID_BED_MAT2         = 1003;
const int   STORAGE_NPC_ID_BED_MAT3         = 1002;

const int   STORAGE_NPC_ID_STORAGE_MAT1         = 1001;
const int   STORAGE_NPC_ID_STORAGE_MAT2         = 1000;
const int   STORAGE_NPC_ID_STORAGE_MAT3         = 999;

const int   STORAGE_NPC_ID_FLOWER_MAT1         = 998;
const int   STORAGE_NPC_ID_FLOWER_MAT2         = 997;
const int   STORAGE_NPC_ID_FLOWER_MAT3         = 996;

const int   STORAGE_NPC_ID_CABINET_MAT1         = 995;
const int   STORAGE_NPC_ID_CABINET_MAT2         = 994;
const int   STORAGE_NPC_ID_CABINET_MAT3         = 993;
const int   STORAGE_NPC_ID_MARRYHOME            = 992;

const OBJID REBORN_SCROLL = 630000;//原地复活卷轴TYPEID
extern int g_SpecilLine;

int EquipSoulPoints[] = {0,10,30,90,270,810,2430,7290,21870,65610};//铸灵所需魔灵数
int EquipSoulAdds1[] = {0,19,34,66,101,216,325,518,645,766,896};//冰火玄毒叠加属性
int EquipSoulAdds2[] = {0,12,22,43,67,144,216,346,430,509,640};//力灵体定身

//////////////////////////////////////////////////////////////////////
// item
//////////////////////////////////////////////////////////////////////
enum{
	StarPoint1 = 1,
	StarPoint2 = 2,
	StarPoint3 = 3,
	StarPoint4 = 4
};
bool CUser::CreateItemFree(IDatabase* pDb)
{
	ASSERT(m_pPackage);
	m_pPackage->Create(this, pDb,false);

	//下面的保留请求
	DEBUG_TRY
	//从账号服务器获取用户的元宝余额
	CMsgVAS MsgVAS;
	if (MsgVAS.Create(0, GetAccountID(), GetID(), _MSGVAS_QUERY_BALANCE))
	{
		//MsgVAS.m_pInfo->mtExtraInfo.usAction = 3;
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	//从账号服务器获取用户的防沉迷信息  
	CMsgFatigue MsgFati;
	if (MsgFati.Create( GetAccountID(), GetID(), 0))
	{
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgFati );
	}

	if (MsgVAS.Create(1, GetAccountID(), GetID(), _MSGVAS_QUERY_CARD))
	{
		//MsgVAS.m_pInfo->mtExtraInfo.usAction = 3;
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}

	if (MsgVAS.Create(2, GetAccountID(), GetID(), _MSGVAS_QUERY_CARD))
	{
		//MsgVAS.m_pInfo->mtExtraInfo.usAction = 3;
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	//goto 查询活动奖励状态
	if (MsgVAS.Create(3, GetAccountID(), GetID(), _MSGVAS_QUERY_ACTIVITY))
	{
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	DEBUG_CATCH("请求玩家账号信息")
//	UpdateWeight();
	//物品来了  可以更新了   以后的天赋初始化请做在这一步以前
// 	UpdateCalculatedProperty();
	return true;
}

bool CUser::CreateAllItem(IDatabase* pDb)
{
	ASSERT(m_pPackage);

	int nStep = 0;
	DEBUG_TRY
	// 载入背包物品
	if (!m_pPackage->Create(this, pDb,true))
	{
		LOGWARNING("载入物品失败！");
		return false;
	}

	nStep = 1;
	// 载入装备物品,防止重复位置可能吞掉物品
	SQLBUF		szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && position >= %u && position<%u LIMIT %d", _TBL_ITEM, GetID(), ITEMPOSITION_EQUIPBEGIN, ITEMPOSITION_EQUIPEND, ITEMPOSITION_EQUIPEND - ITEMPOSITION_EQUIPBEGIN);
	IRecordset*	pRes = pDb->CreateNewRecordset(szSQL);
	if(pRes)
	{
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			CItemPtr	pItem = CItem::CreateNew();
			if(pItem)
			{
				nStep = 2;
				if(pItem->Create(pRes, pDb))
				{
					int nPosition = pItem->GetInt(ITEMDATA_POSITION);
					if(GetEquipItem(pItem->GetID()))
					{
						pItem->ReleaseByOwner();
						LOGWARNING("加载装备的时候,已经有对应id物品在身上 userid = %u itemid = %u inPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());
						pRes->MoveNext();
						continue;
					}
					if(GetItem(pItem->GetID()))
					{
						pItem->ReleaseByOwner();
						LOGWARNING("加载装备的时候,已经有对应id物品在背包 userid = %u itemid = %u inPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());
						pRes->MoveNext();
						continue;
					}

					nStep = 3;					
					CItemPtr& refpEquip = GetEquipItemRef(nPosition);
					if(!refpEquip)				//? 防止两个物品的POSITION相同 paled
					{					
						refpEquip = pItem;
					}
					else
					{
						nStep = 5;
						char err[256];
						int nPos = 	m_pPackage->GetAvailablePosition(pItem->GetTypeID());
						if( nPos != -1 )
						{
							nStep = 6;
							sprintf(err,"Found same position of user equip.The Item ID: %d,pos改为%d",pItem->GetID(),nPos );
							pItem->SetPosition( nPos );
							if(!m_pPackage->AddItem( pItem ))
							{
								DEBUG_TRY
								m_pPackage->PopItem(pItem->GetID());
								pItem->ReleaseByOwner();
								DEBUG_CATCH("重复的装备加入背包失败,试图卸载的时候出错")

								LOGWARNING("加载装备的时候,已经有对应位置有装备，背包未满放入失败 userid = %u itemid = %u inPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());
								pRes->MoveNext();
								continue;
							}
						}
						else
						{
							pItem->ReleaseByOwner();
							LOGWARNING("加载装备的时候,已经有对应位置有装备，背包已满 userid = %u itemid = %u inPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());
							pRes->MoveNext();
							continue;
						}
					}
				}
				else
				{
					nStep = 8;
					pItem->ReleaseByOwner();
				}
			}

			pRes->MoveNext();
		}
		pRes->Release();
	}
	DEBUG_CATCH3("玩家加载物品，userid = %u,step = %u",GetID(),nStep);
	
	DEBUG_TRY
	//从账号服务器获取用户的元宝余额
	CMsgVAS MsgVAS;
	if (MsgVAS.Create(0, GetAccountID(), GetID(), _MSGVAS_QUERY_BALANCE))
	{
		//MsgVAS.m_pInfo->mtExtraInfo.usAction = 3;
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	//从账号服务器获取用户的防沉迷信息  
	CMsgFatigue MsgFati;
	if (MsgFati.Create( GetAccountID(), GetID(), 0))
	{
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgFati );
	}

	if (MsgVAS.Create(1, GetAccountID(), GetID(), _MSGVAS_QUERY_CARD))
	{
		//MsgVAS.m_pInfo->mtExtraInfo.usAction = 3;
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}

	if (MsgVAS.Create(2, GetAccountID(), GetID(), _MSGVAS_QUERY_CARD))
	{
		//MsgVAS.m_pInfo->mtExtraInfo.usAction = 3;
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	//goto 查询活动奖励状态
	if (MsgVAS.Create(3, GetAccountID(), GetID(), _MSGVAS_QUERY_ACTIVITY))
	{
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	//查看手机充值礼包状态
	if (MsgVAS.Create(4, GetAccountID(), GetID(), _MSGVAS_QUERY_MOBCHARGE))
	{
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	if (MsgVAS.Create(5, GetAccountID(), GetID(), _MSGVAS_QUERY_MOBCHARGE))
	{
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	if (MsgVAS.Create(6, GetAccountID(), GetID(), _MSGVAS_QUERY_MOBCHARGE))
	{
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
//modify code by 林德渊 begin 2011.5.30
	CMsgVasLv2  MsgVASLv2;
	if( MsgVASLv2.Create(ACTION_LOGINUPDATE,GetID(),0,0,0,0,0,0,0,0,GetAccountID(),0) )
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVASLv2 );
//modify code by 林德渊 end

	DEBUG_CATCH("请求玩家账号信息")
//	UpdateWeight();
	//物品来了  可以更新了   以后的天赋初始化请做在这一步以前
// 	UpdateCalculatedProperty();
	return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SaveItemInfo()
{
	if(m_pShoulder)
		m_pShoulder->SaveInfo();
	if(m_pArmor)
		m_pArmor->SaveInfo();
	if(m_pHelmet)
		m_pHelmet->SaveInfo();
	if(m_pWeaponR )
		m_pWeaponR ->SaveInfo();
	if(m_pNecklace)
		m_pNecklace->SaveInfo();
	if(m_pBangle)
		m_pBangle->SaveInfo();
	if(m_pGlove)
		m_pGlove->SaveInfo();
	if(m_pCaestus)
		m_pCaestus->SaveInfo();
	if(m_pShoes)
		m_pShoes->SaveInfo();
	if(m_pMount)
		m_pMount->SaveInfo();
	if(m_pRingT)
		m_pRingT->SaveInfo();
	if(m_pRingB)
		m_pRingB->SaveInfo();
	if(m_pAdornT)
		m_pAdornT->SaveInfo();
	if(m_pAdornB)
		m_pAdornB->SaveInfo();
	if(m_pDress)
		m_pDress->SaveInfo();
	if(m_pWeaponSoul)
		m_pWeaponSoul->SaveInfo();
	m_pPackage->SaveAllInfo();
}

//////////////////////////////////////////////////////////////////////
void CUser::DeleteAllItem()
{
	SAFE_RELEASEBYOWNER(m_pShoulder );
	SAFE_RELEASEBYOWNER(m_pArmor );
	SAFE_RELEASEBYOWNER(m_pHelmet );
	SAFE_RELEASEBYOWNER(m_pWeaponR);
	SAFE_RELEASEBYOWNER(m_pNecklace );
	SAFE_RELEASEBYOWNER(m_pBangle );
	SAFE_RELEASEBYOWNER(m_pGlove);
	SAFE_RELEASEBYOWNER(m_pCaestus );
	SAFE_RELEASEBYOWNER(m_pShoes);
	SAFE_RELEASEBYOWNER(m_pMount );
	SAFE_RELEASEBYOWNER(m_pRingT );
	SAFE_RELEASEBYOWNER(m_pRingB  );
	SAFE_RELEASEBYOWNER(m_pAdornT );
	SAFE_RELEASEBYOWNER(m_pAdornB  );
	SAFE_RELEASEBYOWNER(m_pDress);
	SAFE_RELEASEBYOWNER(m_pWeaponSoul);
	m_pPackage->ClearAll();
//	UpdateWeight();
}

//////////////////////////////////////////////////////////////////////
int CUser::SendAllItemInfo()
{
	int amount = 0;

	CMsgItem msg;
	msg.Create(ITEMACT_ADD,1);
	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND ; i ++)
	{
		CItem* pItem = GetEquipItemByPos(i);
		if(pItem)
		{
			amount ++;
			msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour());
		}
	}
	SendMsg(&msg);


	//武魂
	CItem* pItemSoul = GetEquipItemByPos(ITEMPOSITION_WEAPONSOUL);
	if(pItemSoul)
	{	
		ST_WEAPONSOULDATA wpslInfo;
		CWeaponSoulData* pData = pItemSoul->QueryWeapensoulData();
		if(pData)
		{
			if(pData->GetInfo(wpslInfo))
			{
				CMsgWeapensoulShow msgwpsl;
				if(msgwpsl.Create(wpslInfo,EM_MSGWPSLSHOW_IN,GetID(),pItemSoul->GetID()))
					this->SendMsg(&msgwpsl);
			}
		}
	}

	amount +=m_pPackage->SendAllItemInfo();
	return amount;
// 		if(bSend)
// 			this->SendMsg(&msg);
// 
// 		DEBUG_TRY
// 		//发宠物 出战宠物
// 		CHECK(m_pUserPet);
// 		m_pUserPet->sendInfo();		
// 		if(m_idEudemonCur !=ID_NONE)
// 		{
// 			CallEudemon(m_idEudemonCur);
// 		}
// 		//心情开关
// 		CMsgUserAttrib msgAtt;
// 		msgAtt.Create(this->GetID(), _USERATTRIB_FEEL_SWITH, GetFeelSwitch());
// 		this->BroadcastRoomMsg(&msgAtt, INCLUDE_SELF);
// 
// 		//发快捷键
// 		m_pUserKey.SendKey(this);
// 
// 		//发技能相关
// 		CHECK(m_pMagic);
// 		m_pMagic->CreateAllCd();
// 
// 		//buff
// 		IDatabase* pDatabase= Database();//MapGroup(idProcess)->GetDatabase();
// 		m_statusmanager.RestoreStatusDB(this,pDatabase);
// 
// 		DEBUG_CATCH("IniUserKey();")
// 	}
// 	else
// 	{
// 		DEBUG_TRY
// 		//物品
// 		m_pPackage->SendAllItemInfo();
// 		//技能列表
// 		SendAllMagicInfo();//技能列表
// //		InitEudemon(Database());
// 		//任务
// 		sendMissionMsg();
// 		//大药
// 		CMsgUserAttrib	msg;//移动速度以及大红大蓝
// 		if(msg.Create(this->GetID(), _USERATTRIB_SPEED, this->AdjustSpeed(this->GetSpeed())))
// 		{
// 			if (GetHpAdd()>0 || GetMpAdd()>0 || GetPetAdd()>0)
// 			{
// 				msg.Append(_USERATTRIB_DRUG_ADDHP, GetHpAdd());
// 				msg.Append(_USERATTRIB_DRUG_ADDMP, GetMpAdd());
// 				msg.Append(_USERATTRIB_DRUG_ADDPET, GetPetAdd());
// 			}
// 			SendMsg(&msg);
// 		}
// //		this->SetPkProc(0, true);
// 		DEBUG_CATCH("m_pPackage->SendAllItemInfo();")
// 	}
}

void CUser::SendSecAtt(int nType)
{
	CMsgUserAttrib	msg;
	if(nType == 0)
		nType = GetCurJob();
	if(nType == 1)
	{
		msg.Create(GetID(),_USERATTRIB_PHYATT,GetAtkUserOnly(nType));
		msg.Append(_USERATTRIB_PORFESSION,GetProfession(nType));
		msg.Append(_USERATTRIB_MAGATT,GetMgcAtkUserOnly(nType));
		msg.Append(_USERATTRIB_PHYDEF,GetDefUserOnly(nType));
		msg.Append(_USERATTRIB_MAGDEF,GetMagicDefUserOnly(nType));
		msg.Append(_USERATTRIB_HIT,GetAtkHitRateUserOnly(nType));
		msg.Append(_USERATTRIB_DODGE,GetDdgUserOnly(nType));
		msg.Append(_USERATTRIB_CRIT,GetCritUserOnly(nType));
		msg.Append(_USERATTRIB_TOUGHT,GetToughUserOnly(nType));
		msg.Append(_USERATTRIB_MAXLIFE,GetMaxLifeUserOnly(nType));
		msg.Append(_USERATTRIB_MAXMANA,GetMaxManaUserOnly(nType));
		msg.Append(_USERATTRIB_MAXSP,GetMaxSp());
		msg.Append(_USERATTRIB_STR,GetStrUserOnly(nType));
		msg.Append(_USERATTRIB_INT,GetIntUserOnly(nType));
		msg.Append(_USERATTRIB_STA,GetStaUserOnly(nType));
		msg.Append(_USERATTRIB_SPI,GetSpiUserOnly(nType));
		msg.Append(_USERATTRIB_AGI,GetAgiUserOnly(nType));
		msg.Append(_USERATTRIB_ADDPOINT,GetAddPoint(nType));
	}
	else
	{
		msg.Create(GetID(),_USERATTRIB_PHYATT_SEC,GetAtkUserOnly(nType));
		msg.Append(_USERATTRIB_PORFESSION_SEC,GetProfession(nType));
		msg.Append(_USERATTRIB_MAGATT_SEC,GetMgcAtkUserOnly(nType));
		msg.Append(_USERATTRIB_PHYDEF_SEC,GetDefUserOnly(nType));
		msg.Append(_USERATTRIB_MAGDEF_SEC,GetMagicDefUserOnly(nType));
		msg.Append(_USERATTRIB_HIT_SEC,GetAtkHitRateUserOnly(nType));
		msg.Append(_USERATTRIB_DODGE_SEC,GetDdgUserOnly(nType));
		msg.Append(_USERATTRIB_CRIT_SEC,GetCritUserOnly(nType));
		msg.Append(_USERATTRIB_TOUGHT_SEC,GetToughUserOnly(nType));
		msg.Append(_USERATTRIB_MAXLIFE_SEC,GetMaxLifeUserOnly(nType));
		msg.Append(_USERATTRIB_MAXMANA_SEC,GetMaxManaUserOnly(nType));
		msg.Append(_USERATTRIB_MAXSP_SEC,GetMaxSp());
		msg.Append(_USERATTRIB_STR_SEC,GetStrUserOnly(nType));
		msg.Append(_USERATTRIB_INT_SEC,GetIntUserOnly(nType));
		msg.Append(_USERATTRIB_STA_SEC,GetStaUserOnly(nType));
		msg.Append(_USERATTRIB_SPI_SEC,GetSpiUserOnly(nType));
		msg.Append(_USERATTRIB_AGI_SEC,GetAgiUserOnly(nType));
		msg.Append(_USERATTRIB_ADDPOINT_SEC,GetAddPoint(nType));
	}
	this->SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
void CUser::SendAllEquipInfoTo(CUser* pRecv)
{
	return;
// 	CHECK(pRecv);
// 
// 	if(GetIsAllowLook() == false)
// 	{
// 		CMsgAction msg;
// 		if(msg.Create(GetID(), 0, 0, 0, actionNotAllowLook,GetID()))
// 		{
// 			pRecv->SendMsg(&msg);
// 		}
// 		return;
// 	}
// 	if(m_pHelmet)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pHelmet, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pHelmet,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pNecklace)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pNecklace, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pNecklace,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pArmor)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pArmor, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pArmor,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pWeaponR)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pWeaponR, ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pWeaponR,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 
// 	if(m_pRingR)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pRingR, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pRingR,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pRingL)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pRingL, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pRingL,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 
// 	if(m_pGlove)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pGlove, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pGlove,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pBangle)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pBangle, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pBangle,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pCaestus)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pCaestus, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pCaestus,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pShoes)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pShoes, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pShoes,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pAmmo1)
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pAmmo1, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pAmmo1,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if (m_pDress)//  [4/5/2007] 哈雷 外装
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pDress, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pDress,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if (m_pWing)//  [4/5/2007] 哈雷 翅膀
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pWing, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pWing,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if( m_pMount )// 坐骑  [陈剑飞 7/30/2007]
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo  msg;
// //		if(msg.Create(m_pMount,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pMount,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if( m_pTearSprite )// 坐骑  [陈剑飞 7/30/2007]
// 	{
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo  msg;
// //		if(msg.Create(m_pMount,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pTearSprite,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 
// 	if(m_pEudemon1)
// 		pRecv->SynchroItem(m_pEudemon1,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 	if(m_pEudemon2)
// 		pRecv->SynchroItem(m_pEudemon2,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
}

//20061010修罗.复数查询by TypeID.由指针参数返回idItem[]
//由引用参数返回实际的size.由函数返回amount
//返回-1时表示idItem[]的实际size超出maxSize.但仍正常返回了maxSize之内的数据
int	CUser::GetItemByTypeMulti(OBJID idType,int &maxSize,OBJID *idItem, bool isKidnap)
{
	return m_pPackage->GetItemByTypeMulti(idType,maxSize,idItem,isKidnap);
}

int CUser::GetEudemonAmountByType(OBJID idType)
{
	CHECKF(m_pUserPet);
	return m_pUserPet->GetShowAmountByType(idType);
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetItemByType(OBJID idType)
{
	return m_pPackage->GetItemByType(idType);
}

CItem* CUser::GetItemByEudemon(OBJID idEudemon)
{
// 	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
// 	if(pData)
// 	{
// 		return GetItem(pData->GetInt(EUDEMONDATA_itemid));
// 	}
	return NULL;
}

int CUser::GetItemNumByType(OBJID idItemType)
{
	CHECKF(m_pPackage);
	return m_pPackage->GetItemNumByType(idItemType);
}

CItem* CUser::GetFristCarryItem()
{
	return m_pPackage->GetFirstCarryItem();
}
//////////////////////////////////////////////////////////////////////
CItem* CUser::GetTaskItem(LPCTSTR szItemName)
{
	return m_pPackage->GetTaskItem(szItemName);
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetTaskItemByType(OBJID idType, int nAmount/*=0*/)
{
	return m_pPackage->GetTaskItemByType(idType, nAmount);
}

bool CUser::CheckPackageFree(USHORT packageType,USHORT amount)
{
	return m_pPackage->GetFreeSpaceCount(packageType) >= amount;
}

/////////////////////////////////////////////////////////////////////////////
CItem* CUser::FindSpaceTransSpell()
{
	return m_pPackage->FindSpaceTransSpell();
}

bool CUser::HasMount()
{
	return m_pMount != NULL;
}

bool CUser::HasEudemon()
{
	return m_pMount != NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetEquipItem(OBJID idItem)
{
	if(m_pShoulder  && m_pShoulder ->GetID() == idItem)
		return m_pShoulder ;
	else if(m_pArmor  && m_pArmor ->GetID() == idItem)
		return m_pArmor ;
	else if(m_pHelmet  && m_pHelmet ->GetID() == idItem)
		return m_pHelmet ;
	else if(m_pWeaponR && m_pWeaponR->GetID() == idItem)
		return m_pWeaponR;
	else if(m_pNecklace  && m_pNecklace ->GetID() == idItem)
		return m_pNecklace ;
	else if(m_pBangle  && m_pBangle ->GetID() == idItem)
		return m_pBangle ;
	else if(m_pGlove && m_pGlove->GetID() == idItem)
		return m_pGlove;
	else if(m_pCaestus  && m_pCaestus ->GetID() == idItem)
		return m_pCaestus ;
	else if(m_pShoes  && m_pShoes ->GetID() == idItem)
		return m_pShoes ;
	else if(m_pMount  && m_pMount ->GetID() == idItem)
		return m_pMount ;
	else if(m_pRingT  && m_pRingT ->GetID() == idItem)
		return m_pRingT ;
	else if (m_pRingB  && m_pRingB ->GetID() == idItem)
		return m_pRingB ;
	else if( m_pAdornT  && m_pAdornT ->GetID() == idItem )// 坐骑 [陈剑飞7/31/2007]
		return m_pAdornT ;
	else if( m_pAdornB  && m_pAdornB ->GetID() == idItem )// 坐骑 [陈剑飞7/31/2007]
		return m_pAdornB ;
	else if (m_pDress && m_pDress->GetID() == idItem)
		return m_pDress;
	else if (m_pWeaponSoul && m_pWeaponSoul->GetID() == idItem)
		return m_pWeaponSoul;
	
	return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetEquipItemByPos(int nPosition)
{
	CItemPtr* ppEquip = this->GetEquipItemPtr(nPosition);
	if (!ppEquip)
		return NULL;
	
	return *ppEquip;
}

//////////////////////////////////////////////////////////////////////
CItemPtr* CUser::GetEquipItemPtr(int nPosition)
{
	switch(nPosition)
	{
	case	ITEMPOSITION_SHOULDER:
		return &m_pShoulder;
	case	ITEMPOSITION_ARMOR:
		return &m_pArmor ;
	case	ITEMPOSITION_HELMET:
		return &m_pHelmet ;
	case	ITEMPOSITION_WEAPONR:
		return &m_pWeaponR;
	case	ITEMPOSITION_NECKLACE:
		return &m_pNecklace ;
	case	ITEMPOSITION_BANGLE:
		return &m_pBangle ;
	case	ITEMPOSITION_GLOVE:
		return &m_pGlove;
	case	ITEMPOSITION_CAESTUS:
		return &m_pCaestus ;
	case	ITEMPOSITION_SHOES:
		return &m_pShoes ;
	case	ITEMPOSITION_MOUNT:
		return &m_pMount ;
	case ITEMPOSITION_RINGT:
		return &m_pRingT ;
	case ITEMPOSITION_RINGB:
		return &m_pRingB ;
	case ITEMPOSITION_ADORNT:
		return &m_pAdornT ;
	case ITEMPOSITION_ADORNB:
		return &m_pAdornB ;
	case ITEMPOSITION_DRESS:
		return &m_pDress;
	case ITEMPOSITION_WEAPONSOUL:
		return &m_pWeaponSoul;
	default:		
		LOGWARNING("ERROR POSITION!!!");
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelItem	(OBJID idItem, bool bSynchro)
{
	if (m_pPackage->DelItem(idItem))
	{
		if(bSynchro)
		{
			CMsgItem msg;
			if(msg.Create(idItem, ITEMACT_DROP))
				this->SendMsg(&msg);
		}
		return true;
	}
	else
		return false;
}
//-------------------------------------------
//20061209修罗.删除多个物品.
//考虑到删除个数超过此物品的最大叠加数,所以需要删除多个item
//在删除之前,先调用GetItemByTypeMulti得到user有此类物品个数,个数不合法时返回false
BOOL	CUser::DelItemByTypeMulti (OBJID idItem,int nAmount,bool& bHasDelKidnapItem, bool isID, bool bIgnoreLock/*=false*/)//新加忽略安全锁参数
{
	CHECKF(idItem);

	//先检查是否够数
	const int _maxItemSize = 120;
	int itemSize = _maxItemSize;//item的个数,叠加算1个
	int itemAmount = 0;
	OBJID itemId[_maxItemSize];//目前背包内最多120格
	if (isID)
	{
		CItem* pItem = GetItem(idItem,false);

		if(!pItem)
			return false;

		itemAmount = pItem->GetItemAmount();
		itemSize = 1;
		itemId[0] = idItem;
		idItem = pItem->GetTypeID();
	}
	else
	{
		itemAmount = GetItemByTypeMulti(idItem,itemSize,itemId);
	}

	if(itemAmount == -1)
		return FALSE;

	if(itemAmount < nAmount/* && CItem::IsCountable(idItem)*/)//不够数
	{
		return FALSE;
	}

	//开始删除///////////////////
	int needDelAmount = nAmount;
	for(int j=0;j<itemSize;j++)
	{
		CItem *pItemTemp = GetItem(itemId[j],false);
		CHECKF(pItemTemp);

		//goto 跳过锁定的物品
		if (pItemTemp->IsLocked())
			continue;

		if(pItemTemp->GetTypeID() != idItem)
		{
			LOGWARNING("删除不正确的物品？");
			continue;
		}
		if (!bIgnoreLock)
		{
			//打了孔的、镶了宝石的、强过化的、升过星的、完美及完美以上的、蓝色以及以上的,需要判断安全锁
			if (IsItemLockedOpen())
			{
				if (pItemTemp->GetEmbedCountLimit()>0 || pItemTemp->GetEmbedGemCount()>0 || pItemTemp->GetEquipAddLev()>0 || pItemTemp->GetEquipStar()>0
					|| pItemTemp->GetEquipQual()>3 || pItemTemp->GetColour()>2 || pItemTemp->IsSoulMark())
				{
					SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return false;
				}
			}
		}

		if(pItemTemp->GetKidnap())
			bHasDelKidnapItem = true;
		
		if(nAmount == 0)
		{
			EraseItem(itemId[j],true);
		}
		else if(CItem::IsCountable(idItem))//可叠加
		{
			int itemAmountTemp= pItemTemp->GetInt(ITEMDATA_AMOUNT);//缓存
			if(needDelAmount>0
				&& needDelAmount >= itemAmountTemp )
			{//当前物品叠加数少于 需删除数量.整个物品删除
				EraseItem(itemId[j],true);//同时给客户端发消息
				needDelAmount-=itemAmountTemp;
			}
			else if(needDelAmount>0
				&& needDelAmount < itemAmountTemp)
			{//当前物品叠加数大于 需删除数量.改变当前物品amount
				SpendItem(pItemTemp, needDelAmount );
				needDelAmount=0;//已经删除够数
			}
		}
		else//不可叠加
		{
			if(needDelAmount>0)
			{
				EraseItem(itemId[j],true);//同时给客户端发消息
				needDelAmount--;
			}
		}
		
		if(nAmount != 0)
		{
			if(needDelAmount == 0)//删除够数了
				break;
			else if(needDelAmount < 0)
			{
				return false;
			}
		}
	}
	return TRUE;
}
//-------------------------------------------
//20061209修罗.删除多个物品.重载
//考虑到效率问题,重载.
//不需要在删除前调用GetItemByTypeMulti().由外部首先调用,判断通过之后再来调用此函数.
BOOL CUser::DelItemByTypeMulti (OBJID *itemIDArray,int size,int amountNeedToDel)
{
	CHECKF(itemIDArray && size>0 && amountNeedToDel>0);

	
	//开始删除///////////////////
	int needDelAmount = amountNeedToDel;
	for(int j=0;j<size;j++)
	{
		CItem *pItemTemp = GetItem(itemIDArray[j],false);
		CHECKF(pItemTemp);

		if(CItem::IsCountable(pItemTemp->GetTypeID()))//可叠加
		{
			int itemAmountTemp= pItemTemp->GetInt(ITEMDATA_AMOUNT);//缓存
			if(needDelAmount>0
				&& needDelAmount >= itemAmountTemp )
			{//当前物品叠加数少于 需删除数量.整个物品删除
				EraseItem(itemIDArray[j],true);//同时给客户端发消息
				needDelAmount-=itemAmountTemp;
			}
			else if(needDelAmount>0
				&& needDelAmount < itemAmountTemp)
			{//当前物品叠加数大于 需删除数量.改变当前物品amount
				SpendItem(pItemTemp, needDelAmount );
				needDelAmount=0;//已经删除够数
			}
		}
		else//不可叠加
		{
			if(needDelAmount>0)
			{
				EraseItem(itemIDArray[j],true);//同时给客户端发消息
				needDelAmount--;
			}
		}
		
		if(needDelAmount == 0)//删除够数了
			break;
		else if(needDelAmount < 0)
		{
			//删成负数.错误
#ifdef _DEBUG
			SendSysMsg("删除物品过量");
#endif
			return false;
		}
	}
	/////////////////////////////
	return TRUE;
}

// bool CUser::AddEudemon (CUserPetData* pEudemon, bool bSynchro, bool bUpdate /*= true*/)
// {
// 	CHECKF(pEudemon);
// 	
// 	if (pEudemon->GetInt(EUDEMONDATA_owner_id) != GetID())
// 		pEudemon->SetInt(EUDEMONDATA_owner_id, GetID(),bUpdate);
// 	
// 	m_pUserPet->AddData(pEudemon);
// 	
// 	if(bSynchro)
// 	{
// 		CMsgEudemon msg;
// 		EudemonData stData;
// 		LoadEudemon(pEudemon->GetID(),stData,2);
// 		msg.Create(1,&stData,GetID());
// 		SendMsg(&msg);
// 	}
// 	return true;
// }
//////////////////////////////////////////////////////////////////////
bool CUser::AddItem (CItem* pItem, bool bSynchro, bool bUpdate /*= true*/)
{
	int nStep = 0;
	DEBUG_TRY
	CHECKF(pItem);
	
	if (pItem->GetInt(ITEMDATA_OWNERID) != GetID())
		pItem->SetInt(ITEMDATA_OWNERID, GetID());

	nStep = 1;

	if (pItem->GetInt(ITEMDATA_PLAYERID) != GetID())
		pItem->SetInt(ITEMDATA_PLAYERID, GetID());

	nStep = 2;

	if (!m_pPackage->AddItem(pItem, bUpdate))
		return false;

	if(bSynchro)
	{
		//20071214修罗:同步修改-----------
//		CMsgItemInfo msg;
//		if(msg.Create(pItem, ITEMINFO_ADDITEM))
//			this->SendMsg(&msg);	
		this->SynchroItem(pItem,ITEMINFO_ADDITEM);
		//--------------------------------
	}

	nStep = 3;

	int  amountShow = 1;
	if(pItem->IsCountable(pItem->GetTypeID()))
	{
		amountShow = pItem->GetItemAmount();
	}

	nStep = 4;

	if (pItem->GetTypeID()==_SPCITEM_VASEXCHANGE || pItem->GetTypeID()==_SPCITEM_EXPEXCHANGE || pItem->GetTypeID()==_SPCITEM_MONEYFORSHOP)
		SendSysMsg(_TXTATR_HELP,"你获得了[%s]",pItem->GetStr(ITEMDATA_NAME));
	else
		SendSysMsg(_TXTATR_HELP,"你获得了[%s] x %u",pItem->GetStr(ITEMDATA_NAME),amountShow);

//	UpdateMissionMsgByItemType(pItem->GetTypeID());
//	UpdateWeight();

	return true;
	DEBUG_CATCH3("CUser::AddItem (CItem* pItem) userid = %u,nStep = %u",GetID(),nStep)
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelItemPt (OBJID idItem)
{
	if (m_pPackage->PopItem(idItem))
	{
//		UpdateWeight();
		return true;
	}
	return false;
}


CUserPetData* CUser::PopEudemon (OBJID idEudemon, bool bSynchro, bool bUpdate)			// 立即清除OWNER
{
	CUserPetData* pEudemon = m_pUserPet->QueryData(idEudemon);
	if (pEudemon)
	{
		OBJID idType = pEudemon->GetInt(EUDEMONDATA_typeid);
		pEudemon->SetInt(EUDEMONDATA_owner_id, ID_NONE, bUpdate);
		if(bSynchro)
		{
			CMsgEudemon msg;
			EudemonData eudemonData;
			if(LoadEudemon(idEudemon,eudemonData,2))
				if(msg.Create( PETACTION_DelPet,&eudemonData,this->GetID()))
					this->SendMsg(&msg);
			CUserPetData* pEudemon = m_pUserPet->QueryData(idEudemon,true);
		}

		DEBUG_TRY
		UpdateMissionMsgByItemType(idType);
		DEBUG_CATCH("pUser->UpdateMissionMsgByItemType eudemon")

		return pEudemon;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::PopItem (OBJID idItem, bool bSynchro, bool bUpdate)			// 立即清除OWNER
{
	CItemPtr pItem = m_pPackage->PopItem(idItem);
	if (pItem)
	{
		pItem->SetInt(ITEMDATA_PLAYERID, ID_NONE);
		pItem->SetInt(ITEMDATA_OWNERID, ID_NONE);			// 立即清除OWNER
		if(bSynchro)
		{
			CMsgItem msg;
			if(msg.Create( ITEMACT_DROP,0,0,0,0,idItem ))
				this->SendMsg(&msg);
		}

		if(bUpdate)
			pItem->SaveInfo();
		return pItem;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////
// CUserPetData* CUser::PopEudemon(OBJID idEudemon, bool bSynchro, bool bUpdate)			// 立即清除OWNER
// {
// 	CUserPetData* pEudemon = m_pUserPet->PopEudemon(idEudemon);
// 	if (pEudemon)
// 	{
// 		pEudemon->SetInt(EUDEMONDATA_owner_id, ID_NONE, bUpdate);
// 		if(bSynchro)
// 		{
// 			EudemonData data;
// 			pEudemon->GetData(data);
// 			//m_pUserPet->fillType(data,1);
// 			CMsgEudemon msg;
// 			msg.Create(2,&data);
// 			SendMsg(&msg);
// 		}
// 		
// 		return pEudemon;
// 	}
// 	return NULL;
// }
//////////////////////////////////////////////////////////////////////
bool CUser::AddEudemon(CUserPetData* pEudemon, bool bSynchro, bool bUpdate)
{
	CHECKF(pEudemon);
	if (pEudemon->GetInt(EUDEMONDATA_owner_id)!=GetID())
		pEudemon->SetInt(EUDEMONDATA_owner_id, GetID(), bUpdate);

	if (!m_pUserPet->AddEudemon(pEudemon, bUpdate))
	{
		return false;
	}

	DEBUG_TRY
	UpdateMissionMsgByItemType(pEudemon->GetInt(EUDEMONDATA_typeid));
	DEBUG_CATCH("pUser->UpdateMissionMsgByItemType eudemon")

	if(bSynchro)
	{
		EudemonData data;
		pEudemon->GetData(data);
		//m_pUserPet->fillType(data,1);
		CMsgEudemon msg;
		msg.Create(PETACTION_PetInfo,&data,GetID());
			SendMsg(&msg);
	}
	return true;
}

bool CUser::HasPeifang( OBJID idPeifang)
{
	CHECKF(m_pUserPeifang);
	if(m_pUserPeifang->QueryData(idPeifang))
		return true;
	return false;
}

bool CUser::DelPeifang( OBJID idPeifang, bool bSynchro)
{
	CHECKF(m_pUserPeifang);
	if(!m_pUserPeifang->QueryData(idPeifang))
		return false;
	if (!m_pUserPeifang->DeleteData(idPeifang))
	{
		return false;
	}
	if(bSynchro)
	{
		CMsgCompound msg;
		if(msg.Create(MSGCOMPOUND_PEIFANGDEL,ID_NONE))
		{
			msg.AppendPeifang(idPeifang);
			SendMsg(&msg);
		}
	}
	return true;
}


bool CUser::AddPeifang( OBJID idPeifang, bool bSynchro)
{
	CHECKF(m_pUserPeifang);
	if(m_pUserPeifang->QueryData(idPeifang))
		return false;
	if (!m_pUserPeifang->CreateNewPeifangData(GetID(), idPeifang))
	{
		return false;
	}
	if(bSynchro)
	{
		CMsgCompound msg;
		if(msg.Create(MSGCOMPOUND_PEIFANGADD,ID_NONE))
		{
			msg.AppendPeifang(idPeifang);
			SendMsg(&msg);
		}
	}
	return true;
}

BOOL CUser::AwardItemCount(OBJID idItemType,UINT nItemAmount, bool bChgattribute,int nJuan,int nKindnap,int nBossType,int nPerPrice)///softworms-给用户指定数量的物品.2007-11-10
{
	BOOL hr=false;
	UINT nItemCountLimit=0;
	if(CItem::IsCountable(idItemType)==true)
	{
		///可以叠加
		CItemTypeData* p = ItemType()->QueryItemType( idItemType );
		CHECKF(p);
		nItemCountLimit=p->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);///得到叠加数
	}
	else
	{
		///不可叠加
		nItemCountLimit=1;
	}
// 	if(IsBackPackSpare(nItemAmount/nItemCountLimit+((nItemAmount%nItemCountLimit)>0), 0, 131010, 0)==false)	//	131010无意义，只是指定一个TYPEID就可以
// 		return false;

	if(idItemType==_SPCITEM_VASEXCHANGE)//元宝票
		nItemCountLimit = nItemAmount;

	while(nItemAmount)
	{
		if(nItemAmount<nItemCountLimit)
			nItemCountLimit=nItemAmount;
		AwardItemBySys(idItemType,true,false,true,bChgattribute/*false*//*true*/,nItemCountLimit,0,nJuan,nKindnap,nBossType,nPerPrice);
		nItemAmount-=nItemCountLimit;
	}
	return !nItemAmount;
}

BOOL CUser::AwardItemByCreate(int nItemType, bool bSynchro, bool bAutoCombine,bool bChgattribute,int nAmmount,int nKindnap,int addType,bool bIgnoreType/*=false*/,LPCTSTR szCreater/*=NULL*/)
{
	ItemInfoStruct info;
	memset(&info,0,sizeof(ItemInfoStruct));
	CHECKF(ItemType()->GetInfo(nItemType, &info));

	BOOL hr=false;
	CItemPtr pItem=NULL;
	if (m_pPackage->IsPackFull(1,nItemType,m_pPackage->GetPositionByItemType(nItemType)))
	{
		SendSysMsg("背包已满");
		return false;
	}
	else
	{
		info.idOwner	= GetID();
		info.idPlayer	= GetID();
		if (szCreater && strlen(szCreater)>0)
			strcpy(info.szMakerName,szCreater);
		else
			strcpy(info.szMakerName,GetName());

		if( IsPickUpKidnap( nItemType ) )
		{
			info.nKidnap = 1;
		}
		else
			info.nKidnap	= nKindnap;
		info.idType = nItemType;
		info.dwPerPrice = 0;
		
		
		
		if(CItem::IsCountable(nItemType))
		{
			if(nAmmount < 1)
				return false;
			if(info.nAmountLimit < nAmmount )
			{
				nAmmount = info.nAmountLimit;
			}
			info.nAmount = nAmmount;
		}

		if(!bIgnoreType)
		{
			if(ITEMSORT_GEM == CItem::GetItemSort(nItemType)
				||ITEMSORT_OTHER == CItem::GetItemSort(nItemType)
				||IETMSORT_EUDEMON == CItem::GetItemSort(nItemType))
				return false;
		}

		if(bAutoCombine && CItem::IsCountable(nItemType)) 
		{
			if(info.nAmount < info.nAmountLimit)
			{
				const int _maxSize=120;
				int sourceItemAmount = 0;
				int sourceItemSize=_maxSize;
				OBJID sourceItemID[_maxSize];
				sourceItemAmount = this->GetItemByTypeMulti(nItemType, sourceItemSize, sourceItemID);

				for (int i = 0; i < sourceItemSize; ++i)
				{
					CItem* pItemGet = GetItem(sourceItemID[i],false);
					if(pItemGet && info.nKidnap ==  pItemGet->GetKidnap())
					{
						if(pItemGet->GetInt(ITEMDATA_AMOUNT) + info.nAmount > info.nAmountLimit)
						{
							if( i == sourceItemSize - 1 && m_pPackage->IsPackFull())
							{
								this->SendSysMsg(g_objStrRes.GetStr(12036));//STR_ITEMBAG_FULL
								return NULL;
							}
							continue;
						}
						
						CItem* pNewItem = CombineNewItem(pItemGet, &info, bSynchro);
						if(pNewItem)
						{
							SendSysMsg(_TXTATR_HELP,"你获得了[%s] x %u",pNewItem->GetStr(ITEMDATA_NAME),info.nAmount);
						}
						return true;
					}
				}
			}
		}

		if (bChgattribute)
		{
			if ( IsFluctuate( nItemType ) )
			{
				EquipChgAttribute(&info,10000,addType);
			}
		}


		pItem = CItem::CreateNew();
		CHECKF(pItem);
		if(!pItem->Create(Database(), 
			GameDataDefault()->GetGameItemData(), 
			&info, 
			GameDataDefault()->GetEudemonData(),
			GameDataDefault()->GetWeaponSoulData(), 
			true, 
			info.id))
		{
			pItem->ReleaseByOwner();
			
			ASSERT(!"Create");
			this->SendSysMsg(g_objStrRes.GetStr(12049));//STR_FAILED_GENERATE_ITEM
			return NULL;
		}

	
		if(pItem)
		{
			pItem->SetInt(ITEMDATA_BUYPRICE,0);
			//  [2007-9-14] 朱斌	获得后激活
			if(pItem->IsActivatePickup())
			{
				//	激活
				ActivateItem(pItem);
			}

			//------拾取绑定---------//-------哈雷
			if (pItem->IsKidnapWhenPickup())
			{
				pItem->SetKidnap(1);
			}
			//------拾取绑定---------//-------哈雷
		}


		if (!AddItem(pItem, bSynchro))
		{
			pItem->DeleteRecord(Database());
			pItem->ReleaseByOwner();
			return false;
		}

	}
	return true;
}
///softworms-2007-11-08-添加系统奖励物品给玩家,如果背备满则发系统邮件
BOOL CUser::AwardItemBySys(int nItemType, bool bSynchro, bool bIdentOK/*=*/, bool bAutoCombine/*=true*/,bool bChgattribute/*=false*/,
						   int nAmmount/*=1*/,int nSynID /*= 0*/,int nJuan,int nKindnap,int nBossType,DWORD dwPerPrice/* = 0*/,OBJID idAddItem)		// 同时操作数据库
{
	ItemInfoStruct info;
	memset(&info,0,sizeof(ItemInfoStruct));
	CHECKF(ItemType()->GetInfo(nItemType, &info));

	//  [1/17/2008 朱斌]	新增int nSynID = 0,PK联赛用,给玩家的入场卷
	BOOL hr=false;
	CItemPtr pItem=NULL;
	if (m_pPackage->IsPackFull(1,nItemType,m_pPackage->GetPositionByItemType(nItemType)))
	{
		///背包满了,尝试用系统邮件发给他
// 		ItemInfoStruct info;
// 		ASSERT(ItemType()->GetInfo(nItemType, &info));
// 		info.idOwner	= GetID();
// 		info.idPlayer	= GetID();
// 		if( CItem::IsNewRemedyByItemType( nItemType ) || CItem::IsNewEudemonRemedyByItemType(nItemType))//这个是对应的IsNewRemedy()的判断！如果IsNewRemedy()修改，这个照改！
// 		{
// 			//int nLife = 0;
// 			//int nMana = 0;
// 			int nAmount = 0;
// 			CItemTypeData* p = ItemType()->QueryItemType( nItemType );
// 			CHECKF( p );
// 			if( p )
// 			{	
// 				if (9200 == nItemType/100 || 3390 == nItemType/100)
// 				{
// 					nAmount = p->GetInt( ITEMTYPEDATA_LIFE );
// 				}else if (9201 == nItemType/100 || 3391 == nItemType/100 ) 
// 				{
// 					nAmount = p->GetInt( ITEMTYPEDATA_MANA );
// 				}			
// 			}
// 		if( nAmount )
// 			info.nAmount = nAmount;
// 		}
// 		//  [2007-8-10] 朱斌 如果是魔导芯片 nAmount 初始为0
// 		if(CItem::IsAdvancedSpiritChip(nItemType) || CItem::IsSpiritChip(nItemType))
// 		{
// 			info.nAmount = 0; 
// 		}
// 		if (bChgattribute)
// 		{
// 			if ( IsFluctuate( nItemType ) )
// 			{
// 				EquipChgAttribute(&info);
// 			}
// 			else if( IsMountFluctuate( nItemType ) )// 改变坐骑属性 [陈剑飞8/17/2007]
// 			{
// 				MountChgAttribute( &info );
// 			}
// 		}
// 		//------拾取绑定---------//-------彭峰---2007/05/09-----	
// 		if( IsPickUpKidnap( nItemType )  || nJuan)
// 		{
// 			info.nKidnap = 1;
// 		}
// 	//------------------------------------------------------
// 		//[游途道标 2008/9/18]如果不急了制造者名字改为把相应字段填空.
// 		if(bChgattribute)//20070707修罗:记录创造者
// 		{
// 			SafeCopy(info.szName,GetName(),_MAX_NAMESIZE);		//存入玩家名字
// 		}
// 		else
// 		{
// 			SafeCopy(info.szName, "", _MAX_NAMESIZE);//[游途道标 2008/9/18]制造者字段设置为空
// 		}
// 
// 		if(bAutoCombine && CItem::IsCountable(nItemType)) ////[游途道标 2009.07.16]如果可以合并
// 		{
// 			if(/*!CItem::IsNeedIdent(info.ucIdent) && */nAmmount < info.nAmountLimit)
// 			{
// 				
// 				//[游途道标 2009.04.25]
// 				//////////////////////////////////////////////////////////////////////////
// 				const int _maxSize=120;
// 				int sourceItemAmount = 0;
// 				int sourceItemSize=_maxSize;
// 				OBJID sourceItemID[_maxSize];
// 				sourceItemAmount = this->GetItemByTypeMulti(nItemType, sourceItemSize, sourceItemID);
// 				
// 				for (int i = 0; i < sourceItemSize; ++i)
// 				{
// 					CItem* pItem = GetItem(sourceItemID[i]);
// 					//[游途道标 2009.04.25]想合并必须绑定条件一样
// 					if(pItem && info.nKidnap ==  pItem->GetKidnap())
// 					{
// 						// check size
// 						if(pItem->GetInt(ITEMDATA_AMOUNT) + nAmmount > info.nAmountLimit)
// 						{
// 							continue;
// 						}
// 						info.nAmount = nAmmount;
// 						return reinterpret_cast<bool>(CombineNewItem(pItem, &info, bSynchro));
// 					}
// 				}
// 			}
// 		}
// 
// 		CItemPtr pItem = CItem::CreateNew();
// 		CHECKF(pItem);
// 		if(!pItem->Create(Database(), 
// 			GameDataDefault()->GetGameItemData(), 
// 			&info, 
// 			GameDataDefault()->GetEudemonData(), 
// 			GameDataDefault()->GetWeaponSoulData(),//20071113修罗:
// 			true,
// 			info.id))
// 		{
// 			pItem->ReleaseByOwner();
// 			ASSERT(!"Create");
// 			this->SendSysMsg(g_objStrRes.GetStr(12049));//STR_FAILED_GENERATE_ITEM
// 			return NULL;
// 		}
// 		//////////////////////////////////////////////////////////////////////////
// 		//  [2007-9-14] 朱斌	生命周期
// 		if(pItem)
// 		{
// 			//  [2007-9-14] 朱斌	获得后激活
// 			if(pItem->IsActivatePickup())
// 			{
// 				//	激活
// 				ActivateItem(pItem);
// 			}	
// 		}
// 		if(CItem::IsCountable(pItem->GetTypeID()))
// 		{
// 			if(nAmmount > 1)//如果大于1,那么是复数物品产生
// 			{
// //				if(pItem->GetItemAmountL()nAmountLimit < nAmmount )
// //				{
// //					nAmmount = info.nAmountLimit;
// //				}
// //				info.nAmount = nAmmount;
// 			CItemTypeData* p = ItemType()->QueryItemType( pItem->GetTypeID() );
// 			CHECKF( p );
// 			if( p )
// 			{
// 				int nAmmountLimt = 	p->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);///得到叠加数
// 				if (nAmmount > nAmmountLimt)
// 				{
// 					nAmmount = nAmmountLimt;
// 				}
// 				pItem->SetItemAmount(nAmmount);
// 			}
// 
// 			}		
// 		}
// 
// 		//  [1/17/2008 朱斌]
// // 		if(nSynID != ID_NONE)
// // 		{
// // 			pItem->SetInt64(ITEMDATA_EXP, nSynID, true);
// // 		}
// 	
// //		hr=PostOffice()->SendMail(this, GetName(), info.szName,info.szName, CSKMail::mtNormal, pItem, 0);	
// 	

	
		return false;
		//pItem->ReleaseByOwner();
	}
	else
	{
		///背包没满,走正常流程		
		pItem=AwardItem(nItemType,bSynchro,bIdentOK,bAutoCombine,bChgattribute,nAmmount,bChgattribute,true,false,nKindnap,nBossType,dwPerPrice);
// 		if(pItem)
// 			UpdateMissionMsgByItemType(pItem->GetTypeID());
		//  [1/17/2008 朱斌]
//		if(pItem != NULL /*&& nSynID != ID_NONE*/)
//		{
			//IgotAnOrangeWeapen(nItemType, pItem); 放在这里不恰当 移走
			//pItem->SetInt64(ITEMDATA_EXP, nSynID, true);
//		}		
		
		hr=pItem!=NULL;
	}
	return hr;
}
////////////////////////////////////////////////////玩家获得装备时发公告
void CUser::IgotAnOrangeWeapen(int ItemTypeId, CItemPtr pItem, char* pMonsterName)
{
	CHECK(pItem);
	int RandNum		   = 0 ;
	char UserName[128] = "";
	char itemName[128] = "";
	char gemName[128]  = "";
	sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
	sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
	SNAPSHOTOBJ,  ItemType()->QueryItemType(pItem->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());

	if (640009 < ItemTypeId && 640413 > ItemTypeId && GetInstenceType() >= 2020 && GetInstenceType() <= 2025)//除判掉落
	{
		if (pItem->GetColour() > 2)
		{
			RandNum = RandGet(3);
			char szMsg[255]		   ;
			if (1 == RandNum)
				sprintf(szMsg, "<%s>经过一番激战后，在门派除叛中居然得到了<%s>这种世间罕有的宝物，真是令人惊叹！",UserName, itemName);
			else if(2== RandNum)
				sprintf(szMsg, "<%s>使出绝技，一举歼灭了门派叛徒，面前一个金光灿灿的宝箱，打开一看竟然是<%s>！",UserName, itemName);
			else 
				sprintf(szMsg, "<%s>怔怔的看着面前门派叛徒的宝箱出了神，心情久久不能平复，这就是梦寐已久的<%s>？真是难以置信！",UserName, itemName);
				
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		}
	}

	if ((((ItemTypeId /100) % 10) == 9) && (pItem->GetColour() == 4) &&  (2002<GetInstenceType()) && (2006>GetInstenceType()))//海上副本公告
	{
		char szMsg[255] = "";
		sprintf(szMsg, " <%s>经过一番苦战，终于在海上副本打败海上魔妖。那金灿灿的宝箱中，竟然是世间罕有的装备<%s>！", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
	}
	if (ItemTypeId == 250002)//海上副本掉落魂魄
	{
		DEBUG_TRY
			char szMsg[255] = "";
		CHECK(pMonsterName);
		if(pItem->GetWpSlType() == 1)
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阳之魄", pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());
		else
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阴之魂", pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());		
		
		sprintf(szMsg, "海上副本的<3_%s_6>已经被我们的英雄<%s>干掉了，搬开尸体发现了一个<%s>！下一个送死的会是谁？",pMonsterName,UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 250002");		
	}

	if (610000 <= ItemTypeId && ItemTypeId < 610012 && GetInstenceType() == 2001 || (ItemTypeId > 630007 && ItemTypeId < 630011 && GetInstenceType() == 2001))//山贼掉落
	{
		char szMsg[255] = "";
		if (1 == RandNum)
			sprintf(szMsg, "<%s>一招打在山贼头目脸上，山贼头目落荒而逃，遗落了一个<%s>。",UserName, itemName);
		else if(2== RandNum)
			sprintf(szMsg, "<%s>一腿踢在山贼头目的小肚子上，山贼头目立刻掏出一个<%s>跪地求饶。",UserName, itemName);
		else 
			 sprintf(szMsg, "<%s>将山贼头目砍翻在地，竟然从其尸体上搜出一个<%s>。",UserName, itemName);
		
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
	}
	if (ItemTypeId == 591003)//山贼头目掉落
	{
		DEBUG_TRY
			char szMsg[255] = "";
		CHECK(pMonsterName);
		sprintf(szMsg, "<%s>率队一举荡平<3_山贼头目_6>的老巢，最终在山贼宝库中发现了一个<%s>。",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 591003");		
	}

// 	if(ItemTypeId == 500204)
// 	{
// 		DEBUG_TRY
// 		char szMsg[255] = "";		
// 		sprintf(szMsg, "<%s>勤劳致富，在江陵<3_礼物仙子(69,98)_6>处领取了每日<%s>。内含有<3_中量经验_6>和各种珍宝。",UserName, itemName);
// 		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
// 		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 2038");
// 	}
	if(ItemTypeId == 500205)
	{
		DEBUG_TRY
		char szMsg[255] = "";		
		sprintf(szMsg, "<%s>幸运不凡，在江陵<3_礼物仙子(69,98)_6>处领取了每日<%s>。内含有<3_大量经验_6>和各种珍宝。",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 2038");
	}
	if(ItemTypeId == 500206)
	{
		DEBUG_TRY
		char szMsg[255] = "";		
		sprintf(szMsg, "<%s>吉星高照，在江陵<3_礼物仙子(69,98)_6>处领取了每日<%s>。内含有<3_巨量经验_6>和各种珍宝。",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 2038");		
	}
	if (ItemTypeId >= 2000000 && ItemTypeId <=2999999)//因为许愿祝福的物品id都加了2000000所以判断大于20W为许愿祝福的物品
	{
		DEBUG_TRY
			char szMsg[255] = "";		
		sprintf(szMsg, "<%s>在开封<3_梦缘天使(43,137)_6>处进行许愿，竟然意外获得了期待已久的<%s>，真是羡煞旁人。",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName()2000000");		
	}
	if (ItemTypeId == 591002)//草泥马大王掉落
	{
		DEBUG_TRY
			char szMsg[255] = "";
		CHECK(pMonsterName);
		sprintf(szMsg, "皇家围场中的<3_草泥马大王_6>傻里傻气的带着<%s>炫耀，却被<%s>征服，只好将其奉上求饶。", itemName,UserName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 591002");		
	}

	else if( ItemTypeId == 500203)
	{
		DEBUG_TRY
		char szMsg[255] = "";		
		sprintf(szMsg, "<%s>完成了首次充值，从江陵<3_礼物仙子(69,98)_6>处领取了<%s>一份。",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 500203");
	}
//modify code by 林德渊 begin 2011.5.30
	else if( ItemTypeId == 500112 ||
			 ItemTypeId == 500113 ||
			 ItemTypeId == 500114 ||
			 ItemTypeId == 500115 ||
			 ItemTypeId == 500116 ||
			 ItemTypeId == 500117 ||
			 ItemTypeId == 500118 ||
			 ItemTypeId == 500119 )
	{
		DEBUG_TRY
		char szMsg[255] = "";
		sprintf(szMsg, "<%s>虽生于豪门，仍不畏艰险踏入江湖，武林前辈赠予其<%s>一个。",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH2("UserManager()->BroadCastMsgAndReceiveName() %d",ItemTypeId);
	}
//modify code by 林德渊 end
//modify code by 林德渊 begin 2011.6.22
	else if( ItemTypeId == 630035 && (GetInstenceType() == 2016 || GetInstenceType() == 2018) )
	{
		DEBUG_TRY
			char szMsg[255] = "";
		sprintf(szMsg, "传闻绝归岛有去无回，<%s>却率队奋勇击败了吴开疆，从吴开疆身上缴获的<%s>，璀璨夺目，令人啧啧称叹！",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH2("UserManager()->BroadCastMsgAndReceiveName() %d",ItemTypeId);
	}
	else if( (ItemTypeId == 630034 ||
		ItemTypeId == 610019 ||
		ItemTypeId == 591101 ||
		ItemTypeId == 591201 ||
		ItemTypeId == 500116 ||
		ItemTypeId == 500319 ||
		ItemTypeId == 500321) && (GetInstenceType() == 2016 || GetInstenceType() == 2018) )
	{
		DEBUG_TRY
			char szMsg[255] = "";
		sprintf(szMsg, "绝归岛主神通广大，却终究敌不过<%s>队伍的英勇善战，<%s>绝归岛的宝藏，尽归英雄之手！",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH2("UserManager()->BroadCastMsgAndReceiveName() %d",ItemTypeId);
	}
//modify code by 林德渊 end
}

bool GetGBKchar(int n,char* szTemp)
{
	CHECKF(szTemp);
	szTemp[0] = 0;
	switch(n)
	{
	case 1:
		strcpy(szTemp,"一");
		break;
	case 2:
		strcpy(szTemp,"二");
		break;
	case 3:
		strcpy(szTemp,"三");
		break;
	case 4:
		strcpy(szTemp,"四");
		break;
	case 5:
		strcpy(szTemp,"五");
		break;
	case 6:
		strcpy(szTemp,"六");
		break;
	case 7:
		strcpy(szTemp,"七");
		break;
	case 8:
		strcpy(szTemp,"八");
		break;
	case 9:
		strcpy(szTemp,"九");
		break;
	case 10:
		strcpy(szTemp,"十");
		break;
	}

	return true;
}

void CUser::JinmeiBroadCast(int nMsgtype, int nJinmeiIndex, int nData)
{
	char UserName[128] = "";
	char szTempName[128] = "";
	char szMsg[255] = "";
	sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());

	
	switch(nMsgtype)
	{
	case 1: //所有等级
		{
			CHECK(nData > 0 && nData <= 90);
			DEBUG_TRY
			char szTop1[128] = "";
			switch((nData - 1) / 30)
			{
			case 1:
				strcpy(szTop1,"真·");
				break;
			case 2:
				strcpy(szTop1,"无上·");
				break;
			}
			char szTopTemp1[128] = "";
			char szTopTemp2[128] = "";
			int nEnd = (nData - 1) % 30 + 1;
			if(nEnd < 11)
			{
				GetGBKchar(nEnd,szTopTemp1);
				sprintf(szTempName,"3_%s%s层_6",szTop1,szTopTemp1);
			}
			else if(nEnd % 10 == 0)
			{
				GetGBKchar(nEnd / 10,szTopTemp1);
				sprintf(szTempName,"3_%s%s十层_6",szTop1,szTopTemp1);
			}
			else
			{
				GetGBKchar(nEnd / 10,szTopTemp1);
				GetGBKchar(nEnd % 10,szTopTemp2);
				sprintf(szTempName,"3_%s%s十%s层_6",szTop1,szTopTemp1,szTopTemp2);
			}
			
			sprintf(szMsg, "<%s>将上乘绝学融会贯通，运转五脏六腑继而汇聚丹田，终于将全部经脉修炼至<%s>。",UserName, szTempName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, ID_NONE, ID_NONE, NULL, ALLUSERS_NAME);
			DEBUG_CATCH("JinmeiBroadCast 1");

		}
		break;
	case 2:
		{
			CHECK (nJinmeiIndex > USERJINMEITYPE_UNKOWN && nJinmeiIndex <= USERJINMEITYPE_RENMEI);
			CHECK(nData > 0 && nData <= 10);
			DEBUG_TRY
			switch(nJinmeiIndex)
			{
			case USERJINMEITYPE_YANGQIAO:
				strcpy(szTempName,"3_阳跷_6");
				break;
			case USERJINMEITYPE_YINGQIAO:
				strcpy(szTempName,"3_阴跷_6");
				break;
			case USERJINMEITYPE_YANGWEI:
				strcpy(szTempName,"3_阳维_6");
				break;
			case USERJINMEITYPE_YINGWEI:
				strcpy(szTempName,"3_阴维_6");
				break;
			case USERJINMEITYPE_DAIMEI:
				strcpy(szTempName,"3_带脉_6");
				break;
			case USERJINMEITYPE_CHONGMEI:
				strcpy(szTempName,"3_冲脉_6");
				break;
			case USERJINMEITYPE_DUMEI:
				strcpy(szTempName,"3_督脉_6");
				break;
			case USERJINMEITYPE_RENMEI:
				strcpy(szTempName,"3_任脉_6");
				break;
			}
			
			sprintf(szMsg, "<%s>闭关强化经脉，终于将<%s>强化至<3_%d_6>。",UserName, szTempName,nData);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, ID_NONE, ID_NONE, NULL, ALLUSERS_NAME);
			DEBUG_CATCH("JinmeiBroadCast 2");
		}
		break;
	}
}

void CUser::WeapensoulBroadCast(int nMsgtype,CItemPtr pItem,ST_WEAPONSOULDATA WPSLData,int nData)
{
	CHECK(pItem);
	char UserName[128] = "";
	char itemName[128] = "";
	char szMsg[255] = "";
	bool bSet = false;
	sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
	if(WPSLData.nType / 10 == 1)
		sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阳之魄", pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());
	else
		sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阴之魂", pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());
	switch(nMsgtype)
	{
	case WEAPENSOULCAST_GROW:
		{
			char szGrow[64] = "";
			if(WPSLData.grow >= 500 && WPSLData.grow < 650)
				strcpy(szGrow,"3_普通_6");
			else if(WPSLData.grow >= 650 && WPSLData.grow < 750)
				strcpy(szGrow,"3_优秀_6");
			else if(WPSLData.grow >= 750 && WPSLData.grow < 850)
				strcpy(szGrow,"3_杰出_6");
			else if(WPSLData.grow >= 850 && WPSLData.grow < 950)
				strcpy(szGrow,"3_卓越_6");
			else if(WPSLData.grow >= 950 && WPSLData.grow < 1000)
				strcpy(szGrow,"3_完美_6");
			else
				return;
			
			int nRand = RandGet(2);
			if(nRand == 0)
				sprintf(szMsg, "<%s>惊天一声咆哮，成功将<%s>的成长率提升至<%s>，魂魄综合能力大幅提升。",UserName, itemName,szGrow);
			else
				sprintf(szMsg, "<%s>仔细把玩着手中的凝炼珠，一不小心竟然将随身携带的<%s>提升至<%s>成长率。",UserName, itemName,szGrow);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_ADDTIONOPEN:
		{
			sprintf(szMsg, "<%s>使用<3_天心箭_6>将<%s>的扩展属性槽增加到<3_%d个_6>。",UserName, itemName,nData);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_ADDTIONLEAN:
		{
			CHECK(nData >= 0 && nData < 10);
			OBJID idSkill = WPSLData.addition[nData];
			if(idSkill == 0 || idSkill == 88888)
				return;
			
			CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(idSkill / 1000);
			if(!pMagicData)
				return;
			int skillType = pMagicData->GetInt(EM_SOULMAGIC_dataID);
			CHECK(skillType);
			CMagicTypeData* pType = CMagic::FindMagicType(skillType);
			if(!pType)
				return;
			if(pType->GetInt(MAGICTYPEDATA_SORT) != 99)
				return;
			
			char szAddtion[64] = "";
			sprintf(szAddtion,"3_%s(%u级)_6",pType->GetStr(MAGICTYPEDATA_NAME),idSkill % 1000);
			sprintf(szMsg, "<%s>通过努力使<%s>学到了扩展属性<%s>，逐渐摆脱了菜鸟行列。",UserName, itemName,szAddtion);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_ADDTIONLEV:
		{
			CHECK(nData >= 0 && nData < 10);
			OBJID idSkill = WPSLData.addition[nData];
			if(idSkill == 0 || idSkill == 88888)
				return;
			
			CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(idSkill / 1000);
			if(!pMagicData)
				return;
			int skillType = pMagicData->GetInt(EM_SOULMAGIC_dataID);
			CHECK(skillType);
			CMagicTypeData* pType = CMagic::FindMagicType(skillType);
			if(!pType)
				return;
			if(pType->GetInt(MAGICTYPEDATA_SORT) != 99)
				return;
			
			char szAddtion[64] = "";
			sprintf(szAddtion,"3_%s_6",pType->GetStr(MAGICTYPEDATA_NAME));

			int nRand = RandGet(2);
			if(nRand == 0)
				sprintf(szMsg, "<%s>经过不懈的努力将<%s>的<%s>提升至<3_%u级_6>，这是怎样的人生啊！",UserName, itemName,szAddtion,idSkill % 1000);
			else
				sprintf(szMsg, "叮！<%s>成功将<%s>的<%s>提升至<3_%u级_6>，看来这是一个极品魂魄。",UserName, itemName,szAddtion,idSkill % 1000);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_MIXLEV:
		{
			CHECK(nData > 0 && nData <= 10);
			if(nData == 5)
			{
				char szType[64] = "";
				switch(WPSLData.nType % 10)
				{
				case 1:
					strcpy(szType,"地");
					break;
				case 2:
					strcpy(szType,"水");
					break;
				case 3:
					strcpy(szType,"火");
					break;
				case 4:
					strcpy(szType,"风");
					break;
				default:
					return;
				}
				sprintf(szMsg, "<%s>已等了几千年，今日终于将<%s>的合成等级提升到<3_%u级_6>，并获得了<3_%s属相_6>！",UserName, itemName,nData,szType);
			}
			else
				sprintf(szMsg, "<%s>历经千辛万苦，终于将<%s>的合成等级提升至<3_%u级_6>，从此不再提心吊胆的生活了。",UserName, itemName,nData);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_SKILLLEAN:
		{
			CHECK(nData >= 0 && nData < 3);
			OBJID idSkill = WPSLData.skill[nData];
			if(idSkill == 0 || idSkill == 88888)
				return;
			
			CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(idSkill / 1000);
			if(!pMagicData)
				return;
			int skillType = pMagicData->GetInt(EM_SOULMAGIC_dataID);
			CHECK(skillType);
			CMagicTypeData* pType = CMagic::FindMagicType(skillType);
			if(!pType)
				return;
			if(pType->GetInt(MAGICTYPEDATA_SORT) != 99)
				return;
			
			char szSkill[64] = "";
			sprintf(szSkill,"3_%s(%u级)_6",pType->GetStr(MAGICTYPEDATA_NAME),idSkill % 1000);
			sprintf(szMsg, "<%s>的<%s>竟然平白无故领悟了<%s>，震惊了所有的父老乡亲。",UserName, itemName,szSkill);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_SKILLRAND:
		{
			sprintf(szMsg, "<%s>使用<3_洗魂符_6>重洗了<%s>的所有技能。",UserName, itemName);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_SKILLLEV:
		{
			CHECK(nData >= 0 && nData < 3);
			OBJID idSkill = WPSLData.skill[nData];
			if(idSkill == 0 || idSkill == 88888)
				return;
			
			CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(idSkill / 1000);
			if(!pMagicData)
				return;
			int skillType = pMagicData->GetInt(EM_SOULMAGIC_dataID);
			CHECK(skillType);
			CMagicTypeData* pType = CMagic::FindMagicType(skillType);
			if(!pType)
				return;
			if(pType->GetInt(MAGICTYPEDATA_SORT) != 99)
				return;
			
			char szSkill[64] = "";
			sprintf(szSkill,"3_%s_6",pType->GetStr(MAGICTYPEDATA_NAME));
			
			int nRand = RandGet(2);
			if(nRand == 0)
				sprintf(szMsg, "<%s>历尽千辛万苦，头发都掉了一半，终于将<%s>的<%s>技能提升至<3_%u级_6>。",UserName, itemName,szSkill,idSkill % 1000);
			else
				sprintf(szMsg, "漫漫江湖路，<%s>近来最幸运的事，就是将<%s>的<%s>技能提升至<3_%u级_6>。",UserName, itemName,szSkill,idSkill % 1000);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_TYPE:
		{
			char szType[64] = "";
			switch(WPSLData.nType % 10)
			{
			case 1:
				strcpy(szType,"地");
				break;
			case 2:
				strcpy(szType,"水");
				break;
			case 3:
				strcpy(szType,"火");
				break;
			case 4:
				strcpy(szType,"风");
				break;
			default:
				return;
			}
			sprintf(szMsg, "<%s>使用<3_魂魄易相丹_6>将<%s>的属相改变为<3_%s属相_6>。",UserName, itemName,szType);
			bSet = true;
		}
		break;
	default:
		break;
	}

	if(bSet)
	{
		DEBUG_TRY
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("WeapensoulBroadCast()");
	}
}

///////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
//[游途道标 2008/9/18]增加BOOL bSaveCreater =true参数,用来选择是否要记录创造者名字.
CItem* CUser::AwardItem(int nItemType, bool bSynchro, bool bIdentOK/*=false*/, bool bAutoCombine/*=true*/,bool bChgattribute/*=false*/,int nAmmount/*=1*/, BOOL bSaveCreater/*=true*/,BOOL bInsert/*=true*/,BOOL bMarry/* = false*/, DWORD nKidnap,int nBossType/* = 0*/,DWORD dwPerPrice/* = 0*/)		// 同时操作数据库
{

	ItemInfoStruct info;
	memset(&info,0,sizeof(ItemInfoStruct));

	CHECKF(ItemType()->GetInfo(nItemType, &info));
	info.idOwner	= GetID();
	info.idPlayer	= GetID();
	info.nKidnap	= nKidnap;
	info.dwPerPrice = dwPerPrice;

	
	if( CItem::IsNewRemedyByItemType( nItemType ))
	{
		int nAmount = 0;
		CItemTypeData* p = ItemType()->QueryItemType( nItemType );
		CHECKF( p );
		if( p )
		{
			if (301 == nItemType/1000 || 321 == nItemType/1000)
			{
				nAmount = p->GetInt( ITEMTYPEDATA_LIFE );
			}else if (311 == nItemType/1000 ) 
			{
				nAmount = p->GetInt( ITEMTYPEDATA_MANA );
			}
			//新加的可叠加的大红大蓝
			if (302 == nItemType/1000 || 322 == nItemType/1000)
			{
				info.BasicAddID[0] = info.BasicAddID[1] = p->GetInt( ITEMTYPEDATA_LIFE );
			}else if (312 == nItemType/1000 ) 
			{
				info.BasicAddID[0] = info.BasicAddID[1] = p->GetInt( ITEMTYPEDATA_MANA );
			}
		}

		if( nAmount )
		{
			info.nAmountLimit = info.nAmount = nAmount;
		}
 	}
	if (nItemType==_SPCITEM_VASEXCHANGE || nItemType==_SPCITEM_EXPEXCHANGE)
		info.nAmountLimit = info.nAmount = nAmmount;

	if (nItemType== _SPCITEM_MONEYFORSHOP)
	{
		info.nAmountLimit = 3000000;
		if(GetLev() < 50)
			info.nAmount = 20000;
		else if(GetLev() < 70)
			info.nAmount = 40000;
		else if(GetLev() < 90)
			info.nAmount = 50000;
		else
			info.nAmount = 60000;
	}
	

	//info.idType = nItemType;
	if(CItem::IsCountable(nItemType))
	{
		if(nAmmount < 1)
			return false;
		if(info.nAmountLimit < nAmmount )
		{
			nAmmount = info.nAmountLimit;
		}
		info.nAmount = nAmmount;
	}
//---------------------end------------------------------------------
	if (bChgattribute)//如果需要改变属性
	{
		return AwardItem(&info, bSynchro, bAutoCombine, true, bSaveCreater,bInsert,false,true,nBossType);
	}
	else
	{
		return AwardItem(&info, bSynchro, bAutoCombine, false, bSaveCreater,bInsert);
	}

}

//////////////////////////////////////////////////////////////////////
CItem* CUser::AwardItem(ItemInfoStruct* pInfo, 
						bool bSynchro, 
						bool bAutoCombine/*=true*/,
						bool bChgattribute/*=false*/,
						BOOL bSaveCreater/*=true*/,BOOL bInsert/*=true*/,BOOL bRoleCreate /*= false*/,BOOL bCreateEudemon/* = false*/,int nBossType/* = 0*/)		// 同时操作数据库
{
	CHECKF (pInfo);
	int nItemType = pInfo->idType;

	if(IETMSORT_EUDEMON == CItem::GetItemSort(nItemType))
		return NULL;

	if( IsPickUpKidnap( nItemType ) )
	{
		pInfo->nKidnap = 1;
	}

	pInfo->ucPosition	= this->QueryPackage()->GetPositionByItemType(nItemType);
	if (pInfo->ucPosition == -1)
	{
		::LogSave("CUser::AwardItem GetAvailablePosition return -1");
		return NULL;
	}

	if(bAutoCombine && CItem::IsCountable(nItemType)) 
	{
		if(pInfo->nAmount < pInfo->nAmountLimit)
		{
			const int _maxSize=120;
			int sourceItemAmount = 0;
			int sourceItemSize=_maxSize;
			OBJID sourceItemID[_maxSize];
			sourceItemAmount = this->GetItemByTypeMulti(nItemType, sourceItemSize, sourceItemID);

			for (int i = 0; i < sourceItemSize; ++i)
			{
				CItem* pItem = GetItem(sourceItemID[i],false);
				if(pItem && pInfo->nKidnap ==  pItem->GetKidnap())
				{
					if(pItem->GetInt(ITEMDATA_AMOUNT) + pInfo->nAmount > pInfo->nAmountLimit)
					{
						if(( i == sourceItemSize - 1 ) && m_pPackage->IsPackFull(1,pItem->GetTypeID()))
						{
							this->SendSysMsg(g_objStrRes.GetStr(12036));//STR_ITEMBAG_FULL
							return NULL;
						}
						continue;
					}
					
					CItem* pNewItem = CombineNewItem(pItem, pInfo, bSynchro);
					if(pNewItem)
					{
						if (pNewItem->GetTypeID()==_SPCITEM_VASEXCHANGE || pNewItem->GetTypeID()==_SPCITEM_EXPEXCHANGE|| pNewItem->GetTypeID()==_SPCITEM_MONEYFORSHOP)
							SendSysMsg(_TXTATR_HELP,"你获得了[%s]",pNewItem->GetStr(ITEMDATA_NAME));
						else
							SendSysMsg(_TXTATR_HELP,"你获得了[%s] x %u",pNewItem->GetStr(ITEMDATA_NAME),pInfo->nAmount);
					}
					return pNewItem;
				}
			}
		}
	}

	//持久加满
	if(CItem::IsEquipment( nItemType ))
	{
		pInfo->nAmount = pInfo->nAmountLimit;
	}

	if (bChgattribute)
	{
		if ( IsFluctuate( nItemType ) )
		{
			EquipChgAttribute(pInfo,nBossType);
		}
	}

	if(pInfo->idType == 144000)
	{
		pInfo->id = MapManager()->SpawnNewVItemID();
	}


	//[游途道标 2009.04.25]绑定条件上移
	//------拾取绑定---------//-------彭峰---2007/05/09-----	
	//if( IsPickUpKidnap( nItemType ) )
	//{
	//	pInfo->nKidnap = 1;
	//}
//------------------------------------------------------
	//[游途道标 2008/9/18]不记录创造者姓名时把相应字段设置为空
	//填入空值修改开始
	if(bSaveCreater)//20070707修罗:记录创造者
	{
		SafeCopy(pInfo->szName,GetName(),_MAX_NAMESIZE);//存入玩家名字
	}	
	//[2009.4.17--张冬]记录捡地图上物品创造者
	else if (bRoleCreate)
	{
		
	}
	else
	{
		SafeCopy(pInfo->szName,"",_MAX_NAMESIZE);
	}

	//填入空值修改结束
	CItemPtr pItem = CItem::CreateNew();
	CHECKF(pItem);
//	bool INSERT_TRUE = true;
	if(!pItem->Create(Database(), 
		GameDataDefault()->GetGameItemData(), 
		pInfo, 
		GameDataDefault()->GetEudemonData(), 
		GameDataDefault()->GetWeaponSoulData(),
		bInsert, 
		pInfo->id))
	{
		pItem->ReleaseByOwner();
		
		ASSERT(!"Create");
		this->SendSysMsg(g_objStrRes.GetStr(12049));//STR_FAILED_GENERATE_ITEM
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	//  [2007-9-14] 朱斌	生命周期
	if(pItem)
	{
		pItem->SetInt(ITEMDATA_BUYPRICE,pInfo->dwPerPrice);
		//  [2007-9-14] 朱斌	获得后激活
		if(pItem->IsActivatePickup())
		{
			//	激活
			ActivateItem(pItem);
		}

		//------拾取绑定---------//-------哈雷
		if (pItem->IsKidnapWhenPickup())
		{
			pItem->SetKidnap(1);
		}
		//------拾取绑定---------//-------哈雷
	}


	// end  [2007-9-14] 朱斌 生命周期
	//////////////////////////////////////////////////////////////////////////
	
	if (!AddItem(pItem, bSynchro))
	{
// 		if(IETMSORT_EUDEMON == CItem::GetItemSort(pInfo->idType))
// 		{
// 			m_pUserPet->DeleteData(pItem->GetInt(ITEMDATA_BASE1));
// 		}
		LOGWARNING("获得物品失败，无法加入背包，userid = %u itemtype = %u",GetID(),pItem->GetTypeID());
		pItem->DeleteRecord(Database());
		pItem->ReleaseByOwner();
		return NULL;
	}

	int  amountShow = 1;
	if(pItem->IsCountable(pItem->GetTypeID()))
	{
		amountShow = pItem->GetItemAmount();
	}
// 	SendSysMsg(_TXTATR_SYSTEM,"你获得了[%s] x %u",pItem->GetStr(ITEMDATA_NAME),amountShow);
// 	UpdateMissionMsgByItemType(pItem->GetTypeID());

	if(pItem->IsPetEgg())//宠物蛋
	{
		if(m_pPetEgg)
			m_pPetEgg->FlushItemAllPetData(pItem);
	}

	return pItem;
}

//////////////////////////////////////////////////////////////////////
bool CUser::EraseItem(OBJID idItem, bool bSynchro, bool bDelEudemon/*=true*/,bool bIsPlayerKill /*= false*/,bool bComBin)		// 同时操作数据库
{
	CItemPtr pItem = m_pPackage->PopItem(idItem);
	if (pItem)
	{
		if(pItem->GetID() > VITEM_ID_FIRST)
			MapManager()->RecycleVItemID(pItem->GetID());

		if(pItem->IsWeaponSoul())
		{
			pItem->EraseWpSl(Database());
		}

		pItem->DeleteRecord(Database(), bDelEudemon);

		pItem->ReleaseByOwner();

		if(bSynchro)
		{
			CMsgItem msg;
			if(msg.Create(ITEMACT_DROP,0,0,0,0,idItem))
				this->SendMsg(&msg);
		}
		return true;
	}
	else
		return false;
}

void	CUser::SendAllBuff()
{
	IDatabase* pDatabase= Database();
	m_statusmanager.RestoreStatusDB(this,pDatabase);
}

int	CUser::SendAllCD()
{
	CHECKF(m_pMagic);
	return m_pMagic->CreateAllCd();
}

int	CUser::SendAllUserkey()
{
	CMsgUserAttrib msgAtt;
	m_pUserKey.SendKey(this);
	msgAtt.Create(GetID(), _USERATTRIB_FEEL_SWITH, GetFeelSwitch());
	BroadcastRoomMsg(&msgAtt, EXCLUDE_SELF);
	SendMsg(&msgAtt);//这里玩家还没有canbesee，所以改成不广播自己，单独给自己发
	return 2;
}

int CUser::SendEudemonList()
{
	CHECKF(m_pUserPet);
	int nAmount = m_pUserPet->sendInfo();

	if(!m_pWarPet && m_idEudemonCur !=ID_NONE)
	{
		CallEudemon(m_idEudemonCur);
	}
	return nAmount;
}

int CUser::SendPeifangList()
{
	CHECKF(m_pUserPeifang);
	return m_pUserPeifang->sendInfo();
}

void CUser::SendEudemonListTo   (CUser* pUser)
{
	CHECK(m_pUserPet);
	m_pUserPet->sendInfo(pUser);
}

void CUser::SendEudemonTo   (CUser* pUser,OBJID idEudemon)
{
	CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
	if(!pData)
	{
		pUser->SendSysMsg("宠物被卖出或销毁");
		return;
	}

	EudemonData data;
	pData->GetData(data);
	m_pUserPet->fillType(data,3);
	m_pUserPet->fillAtt(data,3);
	if(data.life > data.dwMaxLife)
	{
		pData->SetInt(EUDEMONDATA_life,data.dwMaxLife,true);
		data.life = data.dwMaxLife;
	}

	CMsgEudemonInfo msg;
	if(msg.Create(data))
		pUser->SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
bool CUser::EraseEquip(int nPosition, bool bSynchro)		// 同时操作数据库
{
	int nStep = 0;	
	UINT nItemType=0;
	CItemPtr& refpEquip = GetEquipItemRef(nPosition);
	CHECKF(refpEquip);
	DEBUG_TRY
	nItemType=refpEquip->GetTypeID();
	nStep = 1;
// 	DEBUG_TRY
// 	UpdateMissionMsgByItemType(refpEquip->GetTypeID());
// 	DEBUG_CATCH("m_pOwner->UpdateMissionMsgByItemType3")

	OBJID idItem = refpEquip->GetID();
	OBJID idItemType = refpEquip->GetTypeID();
	ST_WEAPONSOULDATA wpslInfo;

	if(idItem > VITEM_ID_FIRST)
		MapManager()->RecycleVItemID(idItem);

	if(refpEquip->IsWeaponSoul())
	{
		CWeaponSoulData* pData = refpEquip->QueryWeapensoulData();
		if(pData)
			pData->GetInfo(wpslInfo);
		refpEquip->EraseWpSl(Database());
	}

	refpEquip->DeleteRecord(Database());
	refpEquip->ReleaseByOwner();
	refpEquip = NULL;

	nStep = 2;

	if(bSynchro)
	{		
		CMsgItem msg;
		if(msg.Create(ITEMACT_DROPEQUIPMENT,0,0,0,0,idItem,0,0,0,nPosition))
			this->SendMsg(&msg);

// 		CMsgItem msg;
// 		if(msg.Create(idItem, ITEMACT_DROPEQUIPMENT, nPosition))
// 			this->SendMsg(&msg);

		if(		nPosition == ITEMPOSITION_WEAPONR
				|| nPosition == ITEMPOSITION_DRESS //2007829修罗:外装
				|| nPosition == ITEMPOSITION_MOUNT //2007829修罗:坐骑
				)
		{
			CMsgPlayer msgPlayer;
			if (msgPlayer.Create(this->QueryRole()))
				this->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
		else if(nPosition == ITEMPOSITION_WEAPONSOUL)
		{
			//武魂
			CMsgWeapensoulShow msgwpsl;
			if(msgwpsl.Create(wpslInfo,EM_MSGWPSLSHOW_OUT,GetID(),idItem))
				this->BroadcastRoomMsg(&msgwpsl, INCLUDE_SELF);

		}

		//  [2007-10-10] 朱斌 从CUser::UnEquipItem中拷贝出来,清坐骑BUF
// 		if( nPosition == ITEMPOSITION_MOUNT )
// 		{
// 			CRole::DetachStatus((IRole*)this,STATUS_MOUNTSPEED_NUMBER);
// 			CMsgPlayer msgPlayer;// 发送消息到客户端更新 [陈剑飞8/25/2007]
// 			if (msgPlayer.Create(this->QueryRole()))
// 				this->BroadcastRoomMsg(&msgPlayer, EXCLUDE_SELF);
// 		}
	}

	nStep = 3;
//	UpdateWeight();
	UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
	return true;
	DEBUG_CATCH4("bool CUser::EraseEquip(int nPosition, bool bSynchro) error step = %u,nPosition = %u,itemtype = %u",nStep,nPosition,nItemType)
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::EquipItem(CItem* pItem, int nPosition, bool bSynchro)
{	
	int nstep = 0;
	DEBUG_TRY
	CHECKF(pItem);

	if(!( nPosition >= ITEMPOSITION_EQUIPBEGIN && nPosition < ITEMPOSITION_EQUIPEND ))
		return false;

	nstep = 1;
	if(pItem->GetPosition() >= ITEMPOSITION_EQUIPBEGIN && pItem->GetPosition()< ITEMPOSITION_EQUIPEND )
	{
		LOGWARNING("装备错误的装备！");
		return false;
	}

	if (QueryMagic())
		QueryMagic()->AbortMagic();

	nstep = 2;
	OBJID idItem = pItem->GetID();
	if (!this->TryItem(idItem, nPosition))
		return false;

	CHECKF(DelItemPt(idItem));
	CHECKF(!GetItem(idItem));

	bool	bRet	= false;
	CItem* pItemAddToBag = NULL;

	nstep = 3;
	if (pItem->IsKidnapWhenEquip() && !pItem->GetKidnap())
	{
		pItem->SetKidnap(1);
		SynchroItem(pItem,ITEMINFO_ADDITEM);
// 		CMsgItemAttrib msg;
// 		if (msg.Create( pItem->GetID(),_ITEMATTRIB_KIDNAP,1 ))
// 			this->SendMsg( &msg );
// 		
	}

	nstep = 4;
	switch(nPosition)
	{
	case	ITEMPOSITION_SHOULDER:
		{
			nstep = 5;
			if(pItem->IsShoulder())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pShoulder = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_SHOULDER);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_ARMOR:
		{
			nstep = 6;
			if(pItem->IsArmor())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pArmor = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_ARMOR);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_HELMET:
		{
			nstep = 7;
			if(pItem->IsHelmet())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pHelmet = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_HELMET);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_WEAPONR:
		{
			nstep = 8;
			if(pItem->IsWeapon())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pWeaponR = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_WEAPONR);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_NECKLACE:
		{
			nstep = 9;
			if(pItem->IsNecklace())
			{	 
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pNecklace = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_NECKLACE);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_BANGLE:
		{
			nstep = 10;
			if(pItem->IsBangle())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pBangle = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_BANGLE);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_GLOVE:
		{
			nstep = 11;
			if(pItem->IsGlove())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pGlove = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_GLOVE);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_CAESTUS:
		{
			nstep = 12;
			if(pItem->IsCaestus())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pCaestus = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_CAESTUS);
				bRet	= true;
			}			
		}
		break;
	case	ITEMPOSITION_SHOES:
		{
			nstep = 13;
			if(pItem->IsShoes())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pShoes = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_SHOES);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_MOUNT:
		{
			nstep = 14;
			if(pItem->IsMount())
			{
				if( GetMountState() )
				{
					CRole::DetachStatus((IRole*)this,STATUS_MOUNTSPEED_NUMBER);
					SetUnMount();
// 					CMsgPlayer msgPlayer;// 发送消息到客户端更新 [陈剑飞8/25/2007]
// 					if (msgPlayer.Create(this->QueryRole()))
// 						BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
					
					SetAttrib(_USERATTRIB_SPEED, AdjustSpeed(GetSpeed()), SYNCHRO_TRUE);
				}
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pMount = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_MOUNT);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_RINGT:
		{
			nstep = 15;
			if(pItem->IsRing())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pRingT = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_RINGT);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_RINGB:
		{
			nstep = 16;
			if(pItem->IsRing())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pRingB = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_RINGB);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_ADORNT:
		{
			nstep = 17;
			if(pItem->IsAdorn())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pAdornT = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_ADORNT);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_ADORNB:
		{
			nstep = 18;
			if(pItem->IsAdorn())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pAdornB = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_ADORNB);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_DRESS:	//外装
		{
			nstep = 19;
			if (pItem->IsDress())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pDress = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_DRESS);
				bRet	= true;
			}
		}
		break;
	case	ITEMPOSITION_WEAPONSOUL:
		{
			nstep = 19;
			if (pItem->IsWeaponSoul())
			{
				pItemAddToBag = UnEquipOnly(nPosition);
				m_pWeaponSoul = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_WEAPONSOUL);
				bRet	= true;
			}
		}
		break;
	default:
		ASSERT(!"switch");
	} // switch
	if(bRet)
	{
		nstep = 191;
		if(pItem->GetPosition() != nPosition)
		{
			LOGWARNING("穿上去的装备位置不对position = %u userid = %u itemid = %u",nPosition,GetID(),pItem->GetID());
		}
		nstep = 20;
		pItem->SetInt( ITEMDATA_PACKAGE_INDEX,0 );
		//pItem->SaveInfo();
		if(bSynchro)
		{
			CMsgItem msg;
			int bagPosition = 0;
			int bagIndex = 0;
			if(pItemAddToBag)
			{
				DEBUG_TRY
				bagPosition = pItemAddToBag->GetPosition();
				bagIndex = pItemAddToBag->GetPackageIndex();

				if(GetEquipItem(pItemAddToBag->GetID()))
					LOGWARNING("脱下来的装备,身上还有position = %u userid = %u itemid = %u",bagPosition,GetID(),pItemAddToBag->GetTypeID());

				if ((bagPosition < ITEMPOSITION_PACK_BEGIN) && (bagPosition >= ITEMPOSITION_PACK_END))
				{
					LOGWARNING("脱下来的装备位置不对position = %u userid = %u itemid = %u",bagPosition,GetID(),pItemAddToBag->GetID());
				}
				DEBUG_CATCH3("对被脱下来的装备操作出问题，position = %u userid = %u",nPosition,GetID())
			}
			nPosition = nPosition * 1000 + bagPosition;
			if (msg.Create(ITEMACT_EQUIP,0, 0, nPosition,bagIndex,pItem->GetID()))
				this->SendMsg(&msg);
			//-------------------------end----------------------------------------------------
			if(/* nPosition / 1000 == ITEMPOSITION_ARMOR ||*/ nPosition / 1000 == ITEMPOSITION_WEAPONR || nPosition / 1000 == ITEMPOSITION_DRESS || nPosition / 1000 == ITEMPOSITION_MOUNT )
			{																	
				CMsgPlayer msgPlayer;
				if (msgPlayer.Create(this->QueryRole()))
					this->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
			}
			else if(nPosition / 1000 == ITEMPOSITION_WEAPONSOUL)
			{
				//武魂
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(pData)
				{
					if(pData->GetInfo(wpslInfo))
					{
						CMsgWeapensoulShow msgwpsl;
						if(msgwpsl.Create(wpslInfo,EM_MSGWPSLSHOW_IN,GetID(),pItem->GetID()))
							this->BroadcastRoomMsg(&msgwpsl, INCLUDE_SELF);
					}
				}
			}
		}

		nstep = 21;
		ActivateItem(pItem);
		nstep = 22;
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);

// 		vector<ST_SETTLEDACTIONINFO*> vecTemp;
// 		g_settledAction.GetAllSettledActionInfoByType(EM_SETLDACTN_TYPE_EQUIPITEM,vecTemp);
// 		vector<ST_SETTLEDACTIONINFO*>::iterator iter;
// 		for(iter = vecTemp.begin();iter != vecTemp.end();iter++)
// 		{
// 			if((*iter) && (*iter)->data == pItem->GetTypeID())
// 			{
// 				GameAction()->ProcessAction((*iter)->actionid, this);
// 			}  // 策划：C冯翔_20062008012802_增加sk_settledaction的type 王萌 [1/29/2008]
// 		}
		return true;
	}
	else
	{	
		nstep = 23;
		LOGWARNING("将装备装向不正确的位置 itemid=%u,position=%u,userid=%u",pItem->GetID(),nPosition,GetID());
		AddItem(pItem, SYNCHRO_FALSE);		
		return false;
	}
	DEBUG_CATCH3("equipitem error!! userid = %u nstep = %u",GetID(),nstep)
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::TryItem(OBJID idItem, int nPosition)
{
	CItemPtr pItem = GetItem(idItem,false);
	if(!pItem)
		return false;

	if(GetEquipItem(idItem))
	{
		LogSave("使用前，发现装备了同样物品userid = %u , itemid = %u itemtype = %u",GetID(),pItem->GetID(),pItem->GetTypeID());
		return false;
	}

	if (pItem->IsNormal())
		return false;

	if(pItem->GetReqLev() > GetLev())
		return false;

	if(pItem->GetReqJobLev() > GetJobLev())
		return false;


	if (pItem->GetInt(ITEMDATA_REQ_SEX) && (pItem->GetInt(ITEMDATA_REQ_SEX) != (GetSex() + 1)))
		return false;

	if (pItem->GetInt(ITEMDATA_REQ_PROF) != 0)
	{
		int nRequireProf = pItem->GetInt(ITEMDATA_REQ_PROF);
		int nProfession  = this->GetProfession();

		if ( 0 ==  ( nProfession & nRequireProf ) )	
			return false;
	} false;

	if(pItem)
	{
		if(pItem->IsActivateEquip() || pItem->IsActivatePickup())
		{
			if(pItem->GetInt(ITEMDATA_NEEDCHECK) == 2)
				return FALSE;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::ChkUseItem(CItem* pItem, IRole* pTarget)
{
	CHECKF (pItem);
	CHECKF (pTarget);

// 	USHORT	usTarget = pItem->GetInt(ITEMDATA_TARGET);
// 	if (usTarget == TARGET_NONE)
// 	{
// 		if (pTarget->GetID() == this->GetID())
// 			return true;
// 		if (pTarget->GetID() == m_eudemonData[0].id)
// 			return true;
// 		else
// 			return false;
// 	}
// 
// 	if (usTarget&TARGET_FORBIDDEN)
// 		return false;
// 
// 	// PK模式由判断提示由客户端完成
// 
// 	CUser* pUser = NULL;
// 	if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
// 	{
// 		if (usTarget&TARGET_ONLY)
// 			return false;
// 		// 限制使用对象
// 		if ((usTarget&TARGET_SELF) && pUser->GetID() != this->GetID()
// 			|| (usTarget&TARGET_OTHERS) && pUser->GetID() == this->GetID())
// 			return false;
// 
// 		//竞技场中不能使用回魂灯
// // 		if(IsInArena() && pItem->GetTypeID()==750040)
// // 		{
// // 			return false;
// // 		}
// 
// 		// 是否对尸体使用的限制
// 		if (((usTarget&TARGET_BODY) && pUser->IsAlive())
// 			|| (!(usTarget&TARGET_BODY) && !pUser->IsAlive()))
// 		{
// 			CMsgItem msg;
// 			IF_OK( msg.Create( pItem->GetID(),ITEMACT_SYNCHRO_UNLOCK ) )
// 				SendMsg( &msg );// 陈剑飞添加,死后对物品解锁 [5/25/2007]
// 			return false;
// 		}	
// 	}
// 	else
// 	{
// 		CMonster* pMonster = NULL;
// 		if (!pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
// 			return false;
// 
// 		int	targetType = pItem->GetInt(ITEMDATA_MONSTERTYPE);
// 	
// 		if (usTarget&TARGET_ONLY && pMonster->GetType() != targetType)
// 			return false;
// 		// 判断目标类型
// 		if (!((usTarget&TARGET_MONSTER) && pMonster->IsMonster())
// 			&& !((usTarget&TARGET_EUDEMON) && pMonster->IsEudemon()))
// 			return false;
// 
// 		// 判断限制条件
// 		CUser* pOwner = pMonster->QueryOwnerUser();
// 		if (pOwner)
// 		{
// 			if ((usTarget&TARGET_SELF) && pOwner->GetID() != this->GetID()
// 				|| (usTarget&TARGET_OTHERS) && pOwner->GetID() == this->GetID())
// 				return false;
// 		}
// 
// 		if (((usTarget&TARGET_BODY) && pMonster->IsAlive())
// 			|| (!(usTarget&TARGET_BODY) && !pMonster->IsAlive()))
// 			return false;
// 	
// 	}

	return true;
}

//////////////////////////////////////////////////////////////////////
//return false,调用时判断了,解锁...
bool CUser::UseItem(OBJID idItem, int nPosition, bool bSynchro)
{
	CHECKF(idItem);
	int nStep = 0;
	DEBUG_TRY
	if(!this->IsAlive())
		return false;

	nStep = 1;
	if( this->QueryStatus( STATUS_SLEEP ) || this->QueryStatus( STATUS_DREAMING ) )//眩晕不能使用物品
	{
		return false;
	}

	nStep = 2;
	if (!this->TryItem(idItem, nPosition))
		return false;
	
	nStep = 3;
	CItemPtr pItem = GetItem(idItem,false);
	if (pItem->IsLocked())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "此物品已锁定,不能使用");
		return false;
	}
	if(!pItem)
		return false;

	nStep = 4;
	if (!CanUseItem(pItem))
		return false;

	nStep = 5;
	if (!ChkUseItem(pItem, this->QueryRole()))
		return false;


	nStep = 6;
	OBJID typeID = pItem->GetTypeID();
 	int nDelayTimeMS = 0;
	int nDelayTimeType = 0;
 	nDelayTimeMS = pItem->GetInt( ITEMDATA_ITEMCD ) % 10000 * 1000;
	nDelayTimeType = pItem->GetInt( ITEMDATA_ITEMCD ) / 10000;
 	CTimeOutMS* pTimeOutMS = NULL;
 	if( nDelayTimeMS )
	{
		MAP_ITEMDELAY_Iter it = m_setItemDelay.find(nDelayTimeType);
		if( it != m_setItemDelay.end() )
		{
			nStep = 7;
			pTimeOutMS = it->second;
			if( pTimeOutMS )
			{
				if( pTimeOutMS->IsActive() )
				{
					if( !pTimeOutMS->IsTimeOut() && pTimeOutMS->GetRemain() <= nDelayTimeMS)
						return false;
				}
			}
		}
		else //没在这里,添加
		{
			nStep = 8;
			pTimeOutMS = new CTimeOutMS();
			if( pTimeOutMS )
			{
				pTimeOutMS->SetInterval( nDelayTimeMS );
				m_setItemDelay[ /*nQueryIndex*/ nDelayTimeType] = pTimeOutMS;
					//使用后调用update函数
			}
		}	
// 		}
	}
//---------------------------------------------------end----------------------------------------
	nStep = 9;
	if((pItem->GetTypeID() / 10000 == 32 ) || (pItem->GetTypeID() / 10000 == 33)  )
	{
		if(pItem->GetTypeID() / 10000 == 32) //宠物
		{
			nStep = 10;
			if( pItem->IsNewRemedy() )//特别的吃不完的大红和大蓝。。呵呵
			{
				DEBUG_TRY
					DWORD dwItemType = pItem->GetTypeID();
				DWORD dwAmountLimit = 0;
				if (pItem->GetTypeID() / 1000 == 321)
					dwAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
				else if (pItem->GetTypeID() / 1000 == 322)
					dwAmountLimit = pItem->GetInt(ITEMDATA_BASE2);
				
				if (pItem->GetItemType()==32)//宠物加血药
				{
					if ((GetPetAdd()+dwAmountLimit)>6000000)//不能超过6百万
					{
						SendSysMsg(_TXTATR_TOPSHOW, "可回复总量超过上限，不能使用");
						return false;
					}
					SetPetAdd(GetPetAdd() + dwAmountLimit);
					if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
						::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, pItem->GetTypeID());
					ASSERT(SpendItem(pItem, 1));
				}
				DEBUG_CATCH("UseItem") 
					CMsgUserAttrib	msg;
				if (msg.Create(this->GetID(), _USERATTRIB_DRUG_ADDHP, GetHpAdd())
					&& msg.Append(_USERATTRIB_DRUG_ADDMP, GetMpAdd())
					&& msg.Append(_USERATTRIB_DRUG_ADDPET, GetPetAdd()))
					SendMsg(&msg);
				return true;
			}
			SendSysMsg(_TXTATR_TOPSHOW, "宠物药品不能直接使用");
			return false;
		}
	}

	nStep = 11;
	if(pItem->GetTypeID() / 10000 == 39)
	{
		if( pItem->IsWeaponSoulExpItem() )//特别的吃不完的大红和大蓝。。呵呵
		{
			nStep = 12;
			if(FullWeaponLifespan())
			{
				ASSERT(SpendItem(pItem, 1));
				return true;
			}
			
			SendSysMsg(_TXTATR_TOPSHOW, "当前魂魄寿命是满的");
			return false;
		}
	}
	nStep = 13;
	if(pItem->IsActionItem())
	{
		nStep = 14;

		OBJID itemID = pItem->GetID();
		if(!pItem->GetInt(ITEMDATA_ACTION))
		{
			LOGWARNING("数据错误：物品[%d]type[%d]是任务物品,但没有填写对应的action_id。", idItem, pItem->GetInt(ITEMDATA_TYPE));
		}
		
		SetTaskItemID(itemID);
		SendUnlockItemMsg(itemID);
		m_idItem = pItem->GetID();
		//goto 删除物品改在程序里执行，不通过脚本调用，提高安全性
		if (GameAction()->ProcessAction(pItem->GetInt(ITEMDATA_ACTION), this, NULL, pItem, NULL, true))
		{
			pItem = GetItem(idItem,false);
			if(pItem)
			{
				bool bIsCostly = pItem->IsCostlyItem();
				int nType = pItem->GetTypeID();
				if(!SpendItem(pItem))
					::LogSave("CUser::UseItem action物品使用后删除失败,user:%u,itemID:%u,itemType:%u", GetID(), idItem, pItem->GetTypeID());
				else
				{
					if (bIsCostly)//goto 贵重物品记录
						::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, nType);
				}
			}
		}

		return true;
	}
	else if( pItem->IsEquipEnable() )
	{
		nStep = 15;
		if( 0 != pItem->GetInt( ITEMDATA_ITEMCD ) )
		{
			nStep = 16;
			if( pTimeOutMS )
			{
				//-------彭峰---2007/05/30-----
				ResetItemCD( nDelayTimeMS, typeID, pTimeOutMS );
			}


			//-----彭峰 2007/03/10--- 
			int nQueryIndex = CItem::GetDelayGroupIndex( pItem->GetTypeID() );
			CMsgCoolDown msg;
			if( msg.Create( this->GetID(),MAGICCOOLDOWN_BEGIN,ITEM_INFO,nQueryIndex,0, pTimeOutMS->GetInterval()) ) //公共CD
			this->SendMsg( &msg );
			pTimeOutMS->Update();

		}

		if(  pItem->IsEnableMount() )// 坐骑 [陈剑飞7/30/2007]
		{
			nStep = 17;
			if(GetMap()->IsNoMount())
			{
			//	SendSysMsg("该地图不能使用坐骑");
				this->SendSysMsg( g_objStrRes.GetStr(19101) );
				return false;
			}

			return EquipItem( pItem, nPosition, bSynchro );
		}
		nStep = 18;
		return EquipItem(pItem, nPosition, bSynchro);
	}
 	else if( pItem->IsNewRemedy() )//特别的吃不完的大红和大蓝。。呵呵
	{
		nStep = 19;
		DEBUG_TRY
		DWORD dwItemType = pItem->GetTypeID();
		DWORD dwAmountLimit = 0;
		if (dwItemType / 1000 == 301 || dwItemType / 1000 == 311)
			dwAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
		else if (dwItemType / 1000 == 302 || dwItemType / 1000 == 312)
			dwAmountLimit = pItem->GetInt(ITEMDATA_BASE2);
		int   nBigType = dwItemType%10;
		if (pItem->GetItemType()==30)//人物加血药
		{
			if (nBigType==1)//大瓶
			{
				if (GetHpAdd()!=0 && GetHpAdd()%10==2)//已经吃过小瓶
				{
					SendSysMsg(_TXTATR_TOPSHOW, "您已经拥有更强大的本类回复状态");
					return false;
				}
				if ((GetHpAdd()/10+dwAmountLimit)>6000000)//不能超过6百万
				{
					SendSysMsg(_TXTATR_TOPSHOW, "可回复总量超过上限，不能使用");
					return false;
				}
			}
			if (nBigType==2)//小瓶
			{
				if (GetHpAdd()!=0 && GetHpAdd()%10==1)//已经吃过大瓶,覆该
					SetHpAdd(0);

				if ((GetHpAdd()/10+dwAmountLimit)>6000000)//不能超过6百万
				{
					SendSysMsg(_TXTATR_TOPSHOW, "可回复总量超过上限，不能使用");
					return false;
				}
			}
			SetHpAdd((GetHpAdd()/10 + dwAmountLimit)*10 + nBigType);
			if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, pItem->GetTypeID());
			ASSERT(SpendItem(pItem, 1));
		}
		else if (pItem->GetItemType()==31)//人物加蓝药
		{
			if (nBigType==1)//大瓶
			{
				if (GetMpAdd()!=0 && GetMpAdd()%10==2)//已经吃过小瓶
				{
					SendSysMsg(_TXTATR_TOPSHOW, "您已经拥有更强大的本类回复状态");
					return false;
				}
				if ((GetMpAdd()/10+dwAmountLimit)>6000000)//不能超过6百万
				{
					SendSysMsg(_TXTATR_TOPSHOW, "可回复总量超过上限，不能使用");
					return false;
				}
			}
			if (nBigType==2)//小瓶
			{
				if (GetMpAdd()!=0 && GetMpAdd()%10==1)//已经吃过大瓶,覆盖
					SetMpAdd(0);
				
				if ((GetMpAdd()/10+dwAmountLimit)>6000000)//不能超过6百万
				{
					SendSysMsg(_TXTATR_TOPSHOW, "可回复总量超过上限，不能使用");
					return false;
				}
			}
			SetMpAdd((GetMpAdd()/10 + dwAmountLimit)*10 + nBigType);
			if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, pItem->GetTypeID());
			ASSERT(SpendItem(pItem, 1));
		}

 		DEBUG_CATCH("UseItem") 
		CMsgUserAttrib	msg;
		if (msg.Create(this->GetID(), _USERATTRIB_DRUG_ADDHP, GetHpAdd())
			&& msg.Append(_USERATTRIB_DRUG_ADDMP, GetMpAdd())
			&& msg.Append(_USERATTRIB_DRUG_ADDPET, GetPetAdd()))
				SendMsg(&msg);
		return true;
 	}
	else if (pItem->IsEnableDress())//  [4/5/2007] 哈雷 外装
	{
		nStep = 20;
		return EquipItem(pItem, nPosition, bSynchro);
	}

	else if(pItem->IsEatEnable() )
	{
		nStep = 21;
		int	nAddLife	= pItem->GetInt(ITEMDATA_LIFE);
		int	nAddMana	= pItem->GetInt(ITEMDATA_MANA);
		int nAddJinmeiValue = pItem->GetInt(ITEMDATA_BASE1);
		int nTypeID		= pItem->GetInt( ITEMDATA_TYPE );
		int nThirdType  = nTypeID / 1000;
		const int TYPE_ZHENYUAN = 381;
		if (nThirdType != TYPE_ZHENYUAN)
		{
			if(nAddLife != 0 && nAddMana != 0)
			{
				if(GetLife()==GetMaxLife() && GetMana()==GetMaxMana())
				{
					if(GetLife()==GetMaxLife() )
					{
						SendSysMsg("生命值已满，无需恢复");
					}
					
					else
					{
						SendSysMsg("气已满，无需恢复");
					}
					return false;
				}
			}
			else if(nAddLife != 0)
			{
				if(GetLife()==GetMaxLife() )
				{
					SendSysMsg("生命值已满，无需恢复");
					return false;
				}
			}
			else if(nAddMana != 0)
			{
				if( GetMana()==GetMaxMana())
				{
					SendSysMsg("气已满，无需恢复");
					return false;
				}
			}
		}

		int nTimes		= /*pItem->GetInt( ITEMDATA_ATKSPEED )*/1;	// 补血\补蓝的次数

		DEBUG_TRY	// VVVVVVVVVVVVVV	
		const int SP_TYPE = 32;
		bool bAddSp = false;
		if(  SP_TYPE == pItem->GetItemType() )
			bAddSp = true;
		if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, pItem->GetTypeID());
		if( !SpendItem( pItem ) )//??? 能吃的一定能删除, 先删除，防作弊
			return false;
		if( nAddLife != 0  && nThirdType != TYPE_ZHENYUAN)//加血
		{
			if( bAddSp )
				AddAttrib( _USERATTRIB_SP, nAddLife, SYNCHRO_TRUE );	
			else	//加红
			{
				// [7/16/2007]-------王玉波修改，按DOT加的ATKSPEED > 1 ----------------------
				if( nTimes > 1 )
				{
					int nFirstValue = nAddLife / 2;//第一次补一半
					AddAttrib( _USERATTRIB_LIFE,nFirstValue,SYNCHRO_TRUE );//加一次
					
					int nEachValue = nFirstValue / (nTimes - 1);
					//再添加nTimes - 1
					int nInterval = 2;	//间隔时间,秒
					int nThisTimes = nTimes - 1;	
					CRole::AttachStatus( this->QueryRole(),STATUS_DOT_ADDLIFE,nEachValue,nInterval,nThisTimes );
					
				}
				else // 0 或 1都瞬加
				{
					AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
				}
			}
		}
		if( nAddMana != 0  && nThirdType != TYPE_ZHENYUAN) //加蓝
		{
			if( nTimes > 1 )
			{
				int nFirstValue = nAddMana / 2;//第一次补一半
				AddAttrib( _USERATTRIB_MANA,nFirstValue,SYNCHRO_TRUE );//加一次
				
				//再添加DOT
				int nEachValue = nFirstValue / (nTimes - 1);
				int nInterval = 2;	//间隔时间,秒
				int nThisTime = nTimes - 1;		//作用次数	
				CRole::AttachStatus( this->QueryRole(),STATUS_DOT_ADDMANA,nEachValue,nInterval,nThisTime );				
			}
			else
			{
				AddAttrib(_USERATTRIB_MANA, nAddMana, SYNCHRO_TRUE);
			}
			
		}
       
		//增加玩家经脉值
		if (nAddJinmeiValue != 0 && nThirdType == TYPE_ZHENYUAN)
		{
			this->IncJinmeiValue(nAddJinmeiValue);
		}

		if( pTimeOutMS )
		{
			ResetItemCD( nDelayTimeMS, nDelayTimeType, pTimeOutMS );
		}


		DEBUG_CATCH("UseItem") // AAAAAAAAAAAAAAAAA

		return true;
	}
	else if(pItem->GetTypeID()==_SPCITEM_VASEXCHANGE)//goto 元宝票,这里不删物品，确认元宝领取成功后再删
	{
		nStep = 22;
		if(pItem->GetItemAmount()>2000)//元宝票上限2000
		{
			::LogSave("user(%u) vas ticket > 2000. itemid=%u,amount=%u", GetID(), pItem->GetID(), pItem->GetItemAmount());
			return false;
		}

		CMsgVAS msg;
		if (msg.Create(pItem->GetItemAmount(), GetAccountID(), GetID(), _MSGVAS_ADD_VAS, _SPCITEM_VASEXCHANGE, pItem->GetItemAmount(), idItem))
 			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
		pItem->setLocked(true);
		return true;
	}
	return false;
	DEBUG_CATCH2("CUser::UseItem() %d",nStep)
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::UseItemTo(OBJID idTarget, OBJID idItem)
{
	CHECKF(idTarget);
	CHECKF(idItem);
	if(!this->IsAlive())
		return false;

	int nStep = 0;
	try{
	if(!m_pUserPet)
		return false;

	if (idTarget == ID_NONE)
		return false;
		
	CItemPtr pItem = GetItem(idItem,false);

	if(!pItem)
		return false;

	nStep = 1;
	if (pItem->IsLocked())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "此物品已锁定,不能使用");
		return false;
	}

	nStep = 2;
	if(pItem->GetTypeID() / 1000 == 322) //宠物滋补丹,不应该进这里,且与宠物药typeid冲突了,为了防止当成普通宠物药吃掉,只好这里特殊处理一下
	{
//		::LogSave("宠物滋补丹进了不该进的流程,user=%u,itemID=%u",GetID(), idItem);
		return false;
	}
	if(pItem->GetTypeID() / 10000 == 39)
	{
//		::LogSave("魂魄延寿丹进了不该进的流程,user=%u,itemID=%u",GetID(), idItem);
		return false;
	}
	if(pItem->GetTypeID() / 10000 == 32) //宠物
	{
		CUserPetData* pData = m_pUserPet->QueryData(idTarget);
		if(!pData)
			return false;
		EudemonData data;
		if(!pData->GetData(data))
			return false;
		m_pUserPet->fillType(data,2);
		m_pUserPet->fillAtt(data,2);

		nStep = 3;

		int	nAddLife	= pItem->GetInt(ITEMDATA_LIFE);		
		if(nAddLife != 0)
		{
			if(data.life >= data.dwMaxLife)
			{
				SendSysMsg("宠物生命已满");
				return false;
			}
		}

		nStep = 4;
		int life = data.life + nAddLife;
		if(life > data.dwMaxLife)
			life = data.dwMaxLife;

		pData->SetInt(EUDEMONDATA_life,life,true);
		CMsgEudemonAttrib msg;
		if(msg.Create(idTarget,EUDATTR_LIFE,life))
			SendMsg(&msg);	
		nStep = 5;
		if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, pItem->GetTypeID());
		SpendItem(pItem,1);
		nStep = 6;

		return true;
	}
	else if(pItem->GetTypeID()  / 10000 == 33)
	{
		CUserPetData* pData = m_pUserPet->QueryData(idTarget);
		if(!pData)
			return false;
		EudemonData data;
		if(!pData->GetData(data))
			return false;
		nStep = 7;
		int	nAddHappy	= pItem->GetInt(ITEMDATA_MANA);		
		if(nAddHappy != 0)
		{
			if(data.happy >= 100)
			{
				SendSysMsg("宠物快乐值已满");
				return false;
			}
		}
		int happy = data.happy + nAddHappy;
		if(happy > 100)
			happy = 100;
		nStep = 8;
		pData->SetInt(EUDEMONDATA_happy,happy,true);
		CMsgEudemonAttrib msg;
		if(msg.Create(idTarget,EUDATTR_HAPPY,happy))
			SendMsg(&msg);	
		
		if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, pItem->GetTypeID());
		SpendItem(pItem,1);

		nStep = 9;
		return true;
	}
	else if(pItem->GetTypeID()  / 10000 == 34)
	{
		CUserPetData* pData = m_pUserPet->QueryData(idTarget);
		if(!pData)
			return false;
		EudemonData data;
		LoadEudemon(idTarget,data,2);
		nStep = 7;
		int	nLifespan	= pItem->GetInt(ITEMDATA_LIFE);		
		if(nLifespan != 0)
		{
			if(data.lifespan >= data.maxlifespan)
			{
				SendSysMsg("宠物寿命值已满");
				return false;
			}
		}
		int Lifespan = data.lifespan + nLifespan;
		if(Lifespan > data.maxlifespan)
			Lifespan = data.maxlifespan;
		nStep = 8;
		pData->SetInt(EUDEMONDATA_lifespan,Lifespan,true);
		CMsgEudemonAttrib msg;
		if(msg.Create(idTarget,EUDATTR_LIFESPAN,Lifespan))
			SendMsg(&msg);	
		
		if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, pItem->GetTypeID());
		SpendItem(pItem,1);
		
		nStep = 9;
		return true;
	}
	else if(pItem->GetTypeID()  / 100 == 6500)
	{
		nStep = 10;
		ItemInfoStruct infoItemType;
		ASSERT( ItemType()->GetInfo(pItem->GetTypeID(), &infoItemType ) );
		int nFriendShip = infoItemType.dwLife;

		CFriend* pMyFriend = GetFriend(idTarget);
		if(!pMyFriend)
		{
			SendSysMsg("对方不是你的好友");
			return false;
		}

		nStep = 11;
		CUserPtr pUserTarget = UserManager()->GetUser(idTarget);
		if(!pUserTarget)
		{
			SendSysMsg("对方离你太远");
			return false;
		}
		nStep = 12;
		char serverName[64]= "";
		char UserName[128] = "";
		char itemName[128] = "";
		char TarName[128]  = "";
		sprintf(UserName, "%d_[%s]_%d",  SNAPSHOTUSER, this->GetName(), COLORYELLOW);
		sprintf(TarName, "%d_[%s]_%d",  SNAPSHOTUSER, pUserTarget->GetName(), COLORYELLOW);
		sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
		SNAPSHOTOBJ,  ItemType()->QueryItemType(pItem->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());
		CFriend* pTargetFriend = pUserTarget->GetFriend(GetID());
		if(pTargetFriend)
		{
			//相互为好友  互相加好友值
			pTargetFriend->SetFriendShip(idTarget,GetID(),Database(),nFriendShip + pTargetFriend->GetFriendShip());
			pMyFriend->SetFriendShip(GetID(),idTarget,Database(),nFriendShip + pMyFriend->GetFriendShip());
			SendSysMsg("对方已加你为好友：你与%s的友好度增加了%u点",pUserTarget->GetName(),nFriendShip);
			pUserTarget->SendSysMsg("你的好友%s送给你%s，你们的友好度增加了%u点",GetName(),pItem->GetStr(ITEMDATA_NAME),nFriendShip);			
			if (650002 == pItem->GetTypeID())
			{
				char szMsg[512] = "";
				sprintf(szMsg, "<%s>向<%s>使用了<%s>，让我们一起祝福他们吧！", UserName, TarName, itemName);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, pItem->GetID(), pItem->GetTypeID(), NULL , ALLUSERS_NAME);
			}
			else if (650003 == pItem->GetTypeID())
			{
				char szMsg[512] = "";
				sprintf(szMsg, "<%s>向<%s>使用了<%s>，幸福有约，情约江湖！", UserName, TarName, itemName);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE, this, pItem->GetID(), pItem->GetTypeID(), NULL , ALLUSERS_NAME);
			}
		}
		else
		{
			pMyFriend->SetFriendShip(GetID(),idTarget,Database(),nFriendShip + pMyFriend->GetFriendShip());
			SendSysMsg("对方未加你为好友：你对%s的友好度增加了%u点",pUserTarget->GetName(),nFriendShip);
			pUserTarget->SendSysMsg("%s送给你%s，对你的友好度增加了%u点",GetName(),pItem->GetStr(ITEMDATA_NAME),nFriendShip);
			if (650002 == pItem->GetTypeID())
			{
				char szMsg[512] = "";
				sprintf(szMsg, "<%s>向<%s>使用了<%s>，让我们一起祝福他们吧！", UserName, TarName, itemName);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, pItem->GetID(), pItem->GetTypeID(), NULL , ALLUSERS_NAME);
			}
			else if (650003 == pItem->GetTypeID())
			{
				char szMsg[512] = "";
				sprintf(szMsg, "<%s>向<%s>使用了<%s>，幸福有约，情约江湖！", UserName, TarName, itemName);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE, this, pItem->GetID(), pItem->GetTypeID(), NULL , ALLUSERS_NAME);
			}
		}
		
		nStep = 13;
		DWORD dwItemType = pItem->GetTypeID();
		if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, dwItemType);
		SpendItem(pItem,1);
		//广播客户端播特效
		if (dwItemType==650003)
		{
			CMsgAction msg;
			if (msg.Create(GetID(), 0,0,0,actionFireWorks,dwItemType))
			{
				BroadcastMapMsg(&msg, INCLUDE_SELF);
				if (GetMapID() != pUserTarget->GetMapID())//如果两人不在同一场景,两个场景都要广播
					pUserTarget->BroadcastMapMsg(&msg, INCLUDE_SELF);
			}
		}

		nStep = 14;
		return true;
	}
	}catch(...)
	{
		LogSave("CUser::UseItemTo(OBJID idTarget, OBJID idItem) error! step = %d",nStep);
	}
	return false;
}

bool CUser::UnEquipItem(int nPosition, bool bSynchro, int nWant2Position,int nWant2PackageIndex)
{	
	int nStep = 0;
	DEBUG_TRY
	if(!( nPosition >= ITEMPOSITION_EQUIPBEGIN && nPosition < ITEMPOSITION_EQUIPEND ))
		return false;

	nStep = 1;
	if (QueryMagic())
		QueryMagic()->AbortMagic();

	CItem* pEquip = GetEquipItemByPos(nPosition);
	if(!pEquip)
		return false;

	nStep =2;
	if(!(pEquip->GetPosition() >=ITEMPOSITION_EQUIPBEGIN && pEquip->GetPosition() < ITEMPOSITION_EQUIPEND ))
	{
		LOGWARNING("错误的装备位置");
		return false;
	}

// 	if( !IsBackPackSpare(1, 0, ID_NONE, ITEMPOSITION_BACKPACK1 )
// 		&& !IsBackPackSpare(1, 0, ID_NONE, ITEMPOSITION_BACKPACK2 )
// 		&& !IsBackPackSpare(1, 0, ID_NONE, ITEMPOSITION_BACKPACK3 ))
// 		return false;

	if(m_pPackage->IsPackFull(1,pEquip->GetTypeID()))
	{
		SendSysMsg("背包已满");
		return false;
	}


	nStep =3;
	CUserPackage* pUserPack = ( CUserPackage* )QueryPackage();
	int nHasFlag = pUserPack->GetIndexState( nWant2Position,nWant2PackageIndex );
	if( nHasFlag != 0)
	{
		nWant2Position = -1;
		nWant2PackageIndex = -1;
	}

	nStep =4;
	CItemPtr pItem = UnEquipOnly(nPosition,nWant2Position,nWant2PackageIndex);
	CHECKF(pItem);

	if(GetEquipItem(pItem->GetID()))
		LOGWARNING("脱下去的装备身上还有，userid = %u itemid = %u outPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());

	nStep =41;
	if((pItem->GetPosition() < ITEMPOSITION_PACK_BEGIN) || (pItem->GetPosition() >= ITEMPOSITION_PACK_END))
		LOGWARNING("脱下去的装备位置不对，userid = %u itemid = %u outPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());

	nStep =5;
	int oldPosition = nPosition;
	nPosition = nPosition * 1000 + pItem->GetPosition();/*MAKELONG( nPosition,pItem->GetPosition());*/ //原来的POS和现在的POS
	if(bSynchro)
	{
		CMsgItem msg;
		if(msg.Create(ITEMACT_UNEQUIP,0, 0, nPosition,pItem->GetPackageIndex(),pItem->GetID()))//06.10.7王玉波修改
			this->SendMsg(&msg);

		nStep =6;
		if(/* oldPosition == ITEMPOSITION_ARMOR ||*/ oldPosition == ITEMPOSITION_WEAPONR ||oldPosition == ITEMPOSITION_DRESS)
		{
			CMsgPlayer msgPlayer;
			if (msgPlayer.Create(this->QueryRole()))
				this->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
		else if(nPosition / 1000 == ITEMPOSITION_WEAPONSOUL)
		{
			//武魂
			ST_WEAPONSOULDATA wpslInfo;
			CWeaponSoulData* pData = pItem->QueryWeapensoulData();
			if(pData)
			{
				if(pData->GetInfo(wpslInfo))
				{
					CMsgWeapensoulShow msgwpsl;
					if(msgwpsl.Create(wpslInfo,EM_MSGWPSLSHOW_OUT,GetID(),pItem->GetID()))
						this->BroadcastRoomMsg(&msgwpsl, INCLUDE_SELF);
				}
			}
		}

		nStep =7;
		if( oldPosition == ITEMPOSITION_MOUNT && GetMountState() )
		{
			CRole::DetachStatus((IRole*)this,STATUS_MOUNTSPEED_NUMBER);
			SetUnMount();
			CMsgPlayer msgPlayer;// 发送消息到客户端更新 [陈剑飞8/25/2007]
			if (msgPlayer.Create(this->QueryRole()))
				BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

			SetAttrib(_USERATTRIB_SPEED, AdjustSpeed(GetSpeed()), SYNCHRO_TRUE);
		}
	}

	nStep =8;
	UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
	return true;
	DEBUG_CATCH3("unequipitem error!! userid = %u nstep = %u",GetID(),nStep)
	return false;
}

//////////////////////////////////////////////////////////////////////
//06.3.13王玉波修改
CItem* CUser::UnEquipOnly(int nPosition,int nWant2Position,int nWant2PackageIndex)
{ 

	CItem* pItem = NULL;
	CItemPtr& refpEquip = GetEquipItemRef(nPosition);
	if (refpEquip == NULL)
		return NULL;

	if(GetItem(refpEquip->GetID()))
	{
		LogSave("脱装备前，发现背包有同样物品了userid = %u , itemid = %u itemtype = %u",GetID(),refpEquip->GetID(),refpEquip->GetTypeID());
		return NULL;
	}

	if(refpEquip)
	{
		pItem = refpEquip;

		int nStep = 0;
		DEBUG_TRY
		if(m_pPackage->IsPackFull(1,pItem->GetTypeID()))
		{
			SendSysMsg("背包已满");
			return NULL;
		}

		nStep = 1;
		if (-1 != nWant2Position && -1 !=nWant2PackageIndex)
		{
			pItem->SetPosition( nWant2Position );
			pItem->SetPackageIndex( nWant2PackageIndex);
		}		

		nStep = 2;
		if(!m_pPackage->AddItem(pItem, true))
		{
			LogSave("脱装备的时候，物品无法放入背包，有待考证");
			return NULL;
		}
		
		DEBUG_CATCH3("脱装备的时候异常，userid = %u step = %u",GetID(),nStep)
		refpEquip = NULL;
	}
	return pItem;
}

//////////////////////////////////////////////////////////////////////
// msgitem
//////////////////////////////////////////////////////////////////////
//BUY A ITEM FROM SHOP
//06.9.15王玉波修改
int CUser::GetRandShopMoney()
{
	CItem* pItemMoney = GetItemByType(_SPCITEM_MONEYFORSHOP);
	if(!pItemMoney)
		return 0;
	
	int nHasMoney = pItemMoney->GetItemAmount();
	int nMoney1 = 0;
	int nMoney2 = 0;

	if(GetLev() < 50)
	{
		if(nHasMoney >= 50000)
		{
			nMoney1 = 30000;
			nMoney2 = nHasMoney - 50000;
		}
	}
	else if(GetLev() < 70)
	{
		if(nHasMoney >= 100000)
		{
			nMoney1 = 60000;
			nMoney2 = nHasMoney - 100000;
		}
	}
	else if(GetLev() < 90)
	{
		if(nHasMoney >= 150000)
		{
			nMoney1 = 100000;
			nMoney2 = nHasMoney - 150000;
		}
	}
	else
	{
		if(nHasMoney >= 180000)
		{
			nMoney1 = 120000;
			nMoney2 = nHasMoney - 180000;
		}
	}

	if(nMoney1 > 0)
		GainMoney(nMoney1,true,1);
	if(nMoney2 > 0)
		GainMoney(nMoney2,true,2);

	if (GetLev()<50 && nMoney1>30000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::GetRandShopMoney user:%u(%s)获得银两%d",GetID(),GetName(),nMoney1);
	else if (GetLev()>=50 && GetLev()<70 && nMoney1>60000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::GetRandShopMoney user:%u(%s)获得银两%d",GetID(),GetName(),nMoney1);
	else if (GetLev()>=70 && nMoney1>100000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::GetRandShopMoney user:%u(%s)获得银两%d",GetID(),GetName(),nMoney1);

	return nMoney1 + nMoney2;
}

bool CUser::CheckRandShopFull()
{
	CItem* pItemMoney = GetItemByType(_SPCITEM_MONEYFORSHOP);
	if(!pItemMoney)
		return false;
	
	
	int nHasMoney = pItemMoney->GetItemAmount();

	bool bFull = false;
	if(GetLev() < 50)
	{
		if(nHasMoney >= 50000)
			bFull = true;
	}
	else if(GetLev() < 70)
	{
		if(nHasMoney >= 100000)
			bFull = true;
	}
	else if(GetLev() < 90)
	{
		if(nHasMoney >= 150000)
			bFull = true;
	}
	else
	{
		if(nHasMoney >= 180000)
			bFull = true;
	}

	return bFull;
}

void CUser::BuyItem ( OBJID idNpc, OBJID idType,int nAmount, int nBuyType,int nPrice/* = 0*/)
{	
	if(idNpc>=PORTABLESHOP_DOWN && idNpc <=PORTABLESHOP_UP)
	{
		int nStep = 0;
		DEBUG_TRY
		nStep = 1;
		CPortableShop* pPShop=CPortableShop::CreateNew();
		CHECK(pPShop);
		pPShop=GetPPortableShopManager()->QueryShopByOwerid(idNpc);
		if (!pPShop)
			return;
		nStep = 2;
		int nSpendType = pPShop->QueryItemTypeSpend(idType);
		if(nSpendType == -1)
		{
			//20061030修罗
			::LogSave("pPShop->QueryItemTypeSpend(idType) idType=%u , idnpc=%u",idType,idNpc);
			return;
		} 

		nStep = 3;
		CItemTypeData* pItemTypeData = ItemType()->QueryItemType( idType );
		if( !pItemTypeData )
			return;
		nStep = 4;
		if(nSpendType < 4)
		{
			if((nSpendType & nBuyType) == 0)
				return;

			if (m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
			{
				this->SendSysMsg("背包已满");
				return;
			}

			int nPrice = pItemTypeData->GetInt(ITEMTYPEDATA_PRICE);

			nStep = 41;
			if( nAmount > 1 )
			{
				int nAmountLimit = pItemTypeData->GetInt( ITEMTYPEDATA_AMOUNT_LIMIT );
				if( nAmount >nAmountLimit )
					nAmount = nAmountLimit;
				nPrice = nPrice * nAmount;
			}

			nStep = 42;
			if(this->GetMoneyByType(nBuyType) < nPrice)
			{
				this->SendSysMsg(_TXTATR_TOPSHOW,"身上银两不足");
				return;
			}
			if (nBuyType==7)
			{
				if (this->GetSynCon()<nPrice)
				{
					this->SendSysMsg(_TXTATR_TOPSHOW,"帮贡不足");
					return;
				}
			}
		
			CItem*	pItem = NULL;
			if(nAmount < 1)
				nAmount = 1;
			int nKindnap = 0;
			if(nBuyType == 2)
				nKindnap = 1;
			pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, false, nAmount, false,true,false,nKindnap);
			
			nStep = 43;
			if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop", GetID(), GetName(), nAmount, idType);

			ASSERT(this->SpendMoneyByType(nPrice,nBuyType, SYNCHRO_TRUE));
			nStep = 44;
		}
		else if (nSpendType == 7)
		{
			nStep =71;
			if (m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
			{
				this->SendSysMsg("背包已满");
				return;
			}
			
			int nPrice = pItemTypeData->GetInt(ITEMTYPEDATA_PRICE);
			
			nStep = 72;
			if( nAmount > 1 )
			{
				int nAmountLimit = pItemTypeData->GetInt( ITEMTYPEDATA_AMOUNT_LIMIT );
				if( nAmount >nAmountLimit )
					nAmount = nAmountLimit;
				nPrice = nPrice * nAmount;
			}
			
			nStep = 73;
			if( this->GetSynCon()< nPrice)
			{
				this->SendSysMsg(_TXTATR_TOPSHOW,"帮贡不足");
				return;
			}
			
			CItem*	pItem = NULL;
			if(nAmount < 1)
				nAmount = 1;
			pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, false, nAmount, false,true,false,1);
			
			nStep = 74;
			if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop", GetID(), GetName(), nAmount, idType);
			if (nPrice >= 2000)
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) oldcon=%u spend %u from shop to buy %u", GetID(), GetName(),GetSynCon(), nPrice, idType);
			this->SetSynCon(GetSynCon()-nPrice);
		}
		else
		{
			nStep = 61;
			if (IsItemLockedOpen())
			{
				SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return;
			}

			int nSpendNum = pPShop->QueryItemAmountSpend(idType);
			if(nSpendNum < 1)
				return;

			nStep = 62;
			CItemTypeData* pItemTypeNeedData = ItemType()->QueryItemType( nSpendType );
			if( !pItemTypeNeedData )
				return;

			if (m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
			{
				this->SendSysMsg("背包已满");
				return;
			}

			nStep = 63;
			if( nAmount > 1 )
			{
				int nAmountLimit = pItemTypeData->GetInt( ITEMTYPEDATA_AMOUNT_LIMIT );
				if( nAmount >nAmountLimit )
					nAmount = nAmountLimit;
				nSpendNum = nSpendNum * nAmount;
			}

			int itemHasNum = GetItemNumByType(nSpendType);
			if(itemHasNum < nSpendNum)
			{
				this->SendSysMsg(_TXTATR_TOPSHOW,"%s不足",pItemTypeNeedData->GetStr(ITEMTYPEDATA_NAME));
				return;
			}

			nStep = 64;
			CItem*	pItem = NULL;
			if(nAmount < 1)
				nAmount = 1;

			int nKindnap = 1;
			pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, true, nAmount, false,true,false,nKindnap);
			
			nStep = 65;
			if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop by %d %u", GetID(), GetName(), nAmount, idType, nSpendNum, nSpendType);
			nStep = 66;
			if(pItem)
			{
				while (nSpendNum > 0)
				{
					CItem* pSpendItem = GetItemByType(nSpendType);
					if(!pSpendItem)
					{
						LogSave("buyitem by item error %u %u",nSpendType,nSpendNum);
						break;
					}

					if(nSpendNum <= pSpendItem->GetItemAmount())
					{
						ASSERT(this->SpendItem(pSpendItem,nSpendNum));
						nSpendNum = 0;
					}
					else
					{
						int nThisSpend = pSpendItem->GetItemAmount();
						ASSERT(this->SpendItem(pSpendItem,nThisSpend));
						nSpendNum -= nThisSpend;
					}
				}
	//modify code by 林德渊 begin 2011.5.21
				CItem *pitem_data = GetItemByType(nSpendType);
				if( pitem_data )
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) %u total (%d)", GetID(), GetName(), nSpendType,pitem_data->GetItemAmount());
				else
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) %u total (%d)", GetID(), GetName(), nSpendType,0);
	//modify code by 林德渊 end
			}
		}
		return;
		DEBUG_CATCH3("BuyItemFromPortableShop error step = %u idNpc= %u ",nStep,idNpc)
	}
//////////////////////////////////////////////////////////////////////////////////////////////////
// 	//如果不是商店NPC卖的
// 	if(!isShopNpcSell)
// 	{
// 	  //参数检测
// 	  if(idType <= 0 || nAmount <= 0)
// 		return;
// 	
// 	   CItem* pItem = NULL;
// 
// 	  //道具类型检查
// 	   CItemTypeData* pItemType = ItemType()->QueryItemType(idType);
// 	   if(!pItemType)
// 		   return;
// 
// 	   if(!CItem::IsCountable(idType))//不可叠加
// 	   {
// 		   //钱检查
// 		   int nPrice = pItemType->GetInt(ITEMTYPEDATA_PRICE);
// 		   
// 		   if(GetMoney() < nPrice* nAmount)
// 		   {
// 			   char szMsg[1024];
// 			   sprintf(szMsg,"%s","你的金币不够！");
// 			   szMsg[255] = '\0';
// 			   CMsgTalk msg;
// 			   if(msg.Create(SYSTEM_NAME,this->GetName(),szMsg,NULL,0x00ffff00,_TXTATR_NORMAL))
// 				   this->SendMsg(&msg);
// 			   return;
// 		   }
// 		   
// 		   //花钱
// 		   if(!this->SpendMoney(nPrice * nAmount, SYNCHRO_TRUE))
// 			   return;
// 		   
// 		   if((this->IsBackPackSpare(nAmount, 0, 920002, 0)))	//	920002无意义，只是指定一个TYPEID就可以
// 		   {
// 			   for (int i= 0;i<nAmount;i++)
// 			   {
// 				   pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, false, 1, false,true,true);
// 				   if(!pItem)
// 					   return;	
// 			   }
// 		   }
// 	   }
// 	   return;
// 	}

	int nStep = 0;
	DEBUG_TRY
	CNpc* pNpc=NULL; 
	CHECK(GetMap());
	if(!GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, idNpc, IPP_OF(pNpc)))
	{
		if(idNpc == 610)
		{
			pNpc = NpcManager()->GetNpc(idNpc);
			if(!pNpc)
				return;	
		}
		else
			return;
	}
	nStep = 1;

	if(!pNpc->IsShopNpc() && !pNpc->IsTaskShopNpc() && !pNpc->IsRandTaskShopNpc())
		return;
	if(!pNpc->QueryShop()->IsOpen())
		return;
	
	nStep = 2;
	int nSpendType = pNpc->QueryShop()->QueryItemTypeSpend(idType);
	if(nSpendType == -1)
	{
		//20061030修罗
		::LogSave("User_Item.cpp Ln 1215.IF_NOT(pType)...idNpc =%d  ||idType =%d  ||nAmount =%d ||nMapID =%u ||nNPCPosX =%d ||nNPCPosY =%d",idNpc,idType,nAmount,GetMap()->GetID(),pNpc->GetPosX(),pNpc->GetPosY());
		return;
	} 

	nStep = 3;
	CItemTypeData* pItemTypeData = ItemType()->QueryItemType( idType );
	if( !pItemTypeData )
		return;

	//	20070712	朱斌	唯一物品检查
// 	if(IsUserHaveAnExclusiveItem(idType, 
// 		NULL, this))
// 	{
// 		char szTemp[128];
// 		sprintf(szTemp,g_objStrRes.GetStr(16013),pType->GetStr(ITEMTYPEDATA_NAME));//"购买失败.不允许拥有多个[%s]!"
// 		SendSysMsg(szTemp);
// 		return;
// 	}

	nStep = 4;
	if(nSpendType < 4)
	{
		if((nSpendType & nBuyType) == 0)
			return;

		if (m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
		{
			this->SendSysMsg("背包已满");
			return;
		}

		int nPrice = pItemTypeData->GetInt(ITEMTYPEDATA_PRICE);

		nStep = 41;
		if( nAmount > 1 )
		{
			int nAmountLimit = pItemTypeData->GetInt( ITEMTYPEDATA_AMOUNT_LIMIT );
			if( nAmount >nAmountLimit )
				nAmount = nAmountLimit;
			nPrice = nPrice * nAmount;
		}

		nStep = 42;
		if(this->GetMoneyByType(nBuyType) < nPrice)
		{
			this->SendSysMsg(_TXTATR_TOPSHOW,"身上银两不足");
			return;
		}
		if (nBuyType==7)
		{
			if (this->GetSynCon()<nPrice)
			{
				this->SendSysMsg(_TXTATR_TOPSHOW,"帮贡不足");
				return;
			}
		}
		
		CItem*	pItem = NULL;
		if(nAmount < 1)
			nAmount = 1;
		int nKindnap = 0;
		if(nBuyType == 2)
			nKindnap = 1;
		pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, false, nAmount, false,true,false,nKindnap);
		
		nStep = 43;
		
// 		if(pItem->IsCostlyItem())
// 		{
// 			ItemInfoStruct info;
// 			ItemType()->GetInfo(pItem->GetTypeID(), &info);
// 			MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 				this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_BUY, 
// 				pItem->GetTypeID(), pItem->GetID(), info.szName,   
// 				CItem::IsCountable(pItem->GetTypeID()) ? nAmount : 1);  
// 		}
		if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop", GetID(), GetName(), nAmount, idType);

		ASSERT(this->SpendMoneyByType(nPrice,nBuyType, SYNCHRO_TRUE));
		nStep = 44;
		//	20070710	朱斌	大额度金钱记录
// 		if(CItem::IsLogSaveMoney(nPrice))
// 		{
// 			MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 				this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_BUY, 
// 				0, 0, "时空币", nPrice);
// 		}
	}
	else if(nSpendType == 4)
	{
		nStep = 51;
		CItem* pItemMoney = GetItemByType(_SPCITEM_MONEYFORSHOP);
		if(!pItemMoney)
			return;

		if (m_pPackage->IsPackFull(nAmount,idType,m_pPackage->GetPositionByItemType(idType)))
		{
			this->SendSysMsg("背包空间不足");
			return;
		}

		int nHasMoney = pItemMoney->GetItemAmount();

		nStep = 52;
	
		bool bFull = CheckRandShopFull();

		if(bFull)
		{
			SendSysMsg("银票金额已满，不能再买进了，快去交任务吧");
			return;
		}

		nStep = 53;
		if(pNpc->QueryShop()->QueryRandShopSellType(idType) != 1)
			return;

		if(pNpc->QueryShop()->QueryRandShopLeave(idType) < nAmount)
		{
			SendSysMsg("物品数量不足，请刷新");
			return;
		}

		nStep = 54;
		int nPerPrice = pNpc->QueryShop()->QueryRandShopPrice(idType);
		if(nPerPrice <= 0)
		{
			SendSysMsg("商店价格已改变，请重新打开商店");
			LOGWARNING("跑商物品价格出错,userid=%u,itemtype=%u,npcid=%u",GetID(),idType,idNpc);
			return;
		}

		if(nPrice != nPerPrice)
		{
			SendSysMsg("商店价格已改变，请重新打开商店");
			return;
		}

		DWORD nPrice = nAmount * nPerPrice;

		
		nStep = 55;
		if(nHasMoney < nPrice)
		{
			SendSysMsg("银票金额不足或价格已刷新，请重新打开商店");
			return;
		}
		if(AwardItemCount(idType,  nAmount,true,0,0,0,nPerPrice))
		{
			pItemMoney->SetInt(ITEMDATA_AMOUNT, nHasMoney - nPrice);
			
			CMsgItem	msg;
			if(msg.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pItemMoney->GetID(),0,pItemMoney->GetItemAmount()))
				this->SendMsg(&msg);
		}
		else
		{
			SendSysMsg("背包已满，请先卖掉一下商品后来来购买");
		}

		nStep = 56;
	}
	else if (nSpendType == 7)
	{
		nStep =71;
		if (m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
		{
			this->SendSysMsg("背包已满");
			return;
		}
		
		int nPrice = pItemTypeData->GetInt(ITEMTYPEDATA_PRICE);
		
		nStep = 72;
		if( nAmount > 1 )
		{
			int nAmountLimit = pItemTypeData->GetInt( ITEMTYPEDATA_AMOUNT_LIMIT );
			if( nAmount >nAmountLimit )
				nAmount = nAmountLimit;
			nPrice = nPrice * nAmount;
		}
		
		nStep = 73;
		if( this->GetSynCon()< nPrice)
		{
			this->SendSysMsg(_TXTATR_TOPSHOW,"帮贡不足");
			return;
		}
		
		CItem*	pItem = NULL;
		if(nAmount < 1)
			nAmount = 1;
		pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, false, nAmount, false,true,false,1);
		
		nStep = 74;
		if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop", GetID(), GetName(), nAmount, idType);
		if (nPrice >= 2000)
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) oldcon=%u spend %u from shop to buy %u", GetID(), GetName(),GetSynCon(), nPrice, idType);
		this->SetSynCon(GetSynCon()-nPrice);

	}
	else if(nSpendType == 8)//竞技场积分商店
	{
		if (IsItemLockedOpen())
		{
			SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
			return;
		}

		CHECK(pNpc->QueryShop());
		int nSpendScore = pNpc->QueryShop()->QueryItemAmountSpend(idType);
		CHECK(nSpendScore > 0);

		CItemTypeData* pItemTypeNeedData = ItemType()->QueryItemType( idType );
		CHECK(pItemTypeNeedData);

		if (m_pPackage && m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
		{
			this->SendSysMsg("背包已满");
			return;
		}

		if( nAmount > 1 )
		{
			int nAmountLimit = pItemTypeData->GetInt( ITEMTYPEDATA_AMOUNT_LIMIT );
			if( nAmount >nAmountLimit )
				nAmount = nAmountLimit;
			nSpendScore = nSpendScore * nAmount;
		}

		int nCurScore = GetArenaScore();
		if(nCurScore < nSpendScore)
		{
			this->SendSysMsg(_TXTATR_TOPSHOW,"荣誉值不足");
			return;
		}

		CItem*	pItem = NULL;
		if(nAmount < 1)
			nAmount = 1;
		
		int nKindnap = 1;
		pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, true, nAmount, false,true,false,nKindnap);

		if(pItem)
			AddAttrib(_USERATTRIB_ARENA_SCORE, -1*nSpendScore, SYNCHRO_TRUE);

		if (pItem/* && pItem->IsCostlyItem()*/)//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop by %d arena score", GetID(), GetName(), nAmount, idType, nSpendScore, nSpendType);
	}
	else
	{
		nStep = 61;
		if (IsItemLockedOpen())
		{
			SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
			return;
		}
		CHECK(pNpc->QueryShop());
		int nSpendNum = pNpc->QueryShop()->QueryItemAmountSpend(idType);
		if(nSpendNum < 1)
			return;

		nStep = 62;
		CItemTypeData* pItemTypeNeedData = ItemType()->QueryItemType( nSpendType );
		if( !pItemTypeNeedData )
			return;

		if (m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
		{
			this->SendSysMsg("背包已满");
			return;
		}

		nStep = 63;
		if( nAmount > 1 )
		{
			int nAmountLimit = pItemTypeData->GetInt( ITEMTYPEDATA_AMOUNT_LIMIT );
			if( nAmount >nAmountLimit )
				nAmount = nAmountLimit;
			nSpendNum = nSpendNum * nAmount;
		}

		int itemHasNum = GetItemNumByType(nSpendType);
		if(itemHasNum < nSpendNum)
		{
			this->SendSysMsg(_TXTATR_TOPSHOW,"%s不足",pItemTypeNeedData->GetStr(ITEMTYPEDATA_NAME));
			return;
		}

		nStep = 64;
		CItem*	pItem = NULL;
		if(nAmount < 1)
			nAmount = 1;

		int nKindnap = 1;
		pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, true, nAmount, false,true,false,nKindnap);
		
		nStep = 65;
// 		if(pItem->IsCostlyItem())
// 		{
// 			ItemInfoStruct info;
// 			ItemType()->GetInfo(pItem->GetTypeID(), &info);
// 			MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 				this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_BUY, 
// 				pItem->GetTypeID(), pItem->GetID(), info.szName,   
// 				CItem::IsCountable(pItem->GetTypeID()) ? nAmount : 1);  
// 		}
		if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop by %d %u", GetID(), GetName(), nAmount, idType, nSpendNum, nSpendType);
		nStep = 66;
		if(pItem)
		{
			while (nSpendNum > 0)
			{
				CItem* pSpendItem = GetItemByType(nSpendType);
				if(!pSpendItem)
				{
					LogSave("buyitem by item error %u %u",nSpendType,nSpendNum);
					break;
				}

				if(nSpendNum <= pSpendItem->GetItemAmount())
				{
					ASSERT(this->SpendItem(pSpendItem,nSpendNum));
					nSpendNum = 0;
				}
				else
				{
					int nThisSpend = pSpendItem->GetItemAmount();
					ASSERT(this->SpendItem(pSpendItem,nThisSpend));
					nSpendNum -= nThisSpend;
				}
			}
//modify code by 林德渊 begin 2011.5.21
			CItem *pitem_data = GetItemByType(nSpendType);
			if( pitem_data )
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) %u total (%d)", GetID(), GetName(), nSpendType,pitem_data->GetItemAmount());
			else
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) %u total (%d)", GetID(), GetName(), nSpendType,0);
//modify code by 林德渊 end
		}
	}
	DEBUG_CATCH2("buyitem error step = %u",nStep)
}
//////////////////////////////////////////////////////////////////////

bool CUser::EndYabiao(int nType)
{
	if(this->GetYabiaoState() == 0)
		return false;

	if(nType == 1)
	{
		IRole* pRole = RoleManager()->QueryRole(this->GetYabiaoID());
		if (pRole)
		{
			CMonster* pMonster = NULL;
			if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
			{
				if(pMonster)
				{
					NpcManager()->DelMonster(pMonster);
				}
			}
		}
	}

	this->SetYabiaoState(0);
	this->SetYabiaoID(0);
	this->SetYabiaoRule(0);
	return true;
}

bool CUser::BegainYabiao(int nType,int nRule)
{
	if(this->GetYabiaoState() > 0)
		return false;

	if(nType == 1)
	{		
		ST_CREATENEWNPC	info;
		memset(&info, 0, sizeof(ST_CREATENEWNPC));
		info.idMap			= GetMapID();
		info.idData			= 0;
		info.idOwner		= this->GetID();		
		info.usPosX			= GetPosX();
		info.usPosY			= GetPosY();
		info.usType			= NpcManager()->GetYabiaoMonsterID(nRule);
		if(info.usType == 0)
			return false;
		info.ucOwnerType	= 0;
		info.nData			= 1000 + nRule;
		OBJID flagID = NpcManager()->CreateSynPetReturnID(&info,this->GetName());

		IRole* pRole = RoleManager()->QueryRole(flagID);
		if (pRole)
		{
			CMonster* pMonster = NULL;
			if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
			{
				if(pMonster)
				{					
					this->SetYabiaoState(1);
					this->SetYabiaoID(flagID);
					this->SetYabiaoRule(nRule);					
					return true;
				}
			}
		}
	}
	else if(nType == 2)
	{
		this->SetYabiaoState(2);
		this->SetYabiaoRule(nRule);
		return true;
	}
	return false;
}

bool CUser::SellItem(OBJID idNpc, OBJID idItem,int nPriceSell/* = 0*/)
{
	if (idNpc>=PORTABLESHOP_DOWN && idNpc <= PORTABLESHOP_UP)
	{
		CPortableShop *pPShop=CPortableShop::CreateNew();
		CHECKF(pPShop);
		pPShop=GetPPortableShopManager()->QueryShopByOwerid(idNpc);
		if (!pPShop)
		 return false;
		CItem* pItem = GetItem( idItem ,false);
		if(!pItem)
		{
			this->SendSysMsg("找不到物品");
			return false;
		}
		if(GetEquipItem(idItem))
		{
			this->SendSysMsg("不能出售穿在身上的装备");
			return false;
		}
		if(pItem->IsLocked())
			return false;
		if(!pItem->IsSellEnable())
			return false;
		if(pItem->GetWpSlLev() >= 3)
			return false;
	//打了孔的、镶了宝石的、强过化的、升过星的、完美及完美以上的、蓝色以及以上的,需要判断安全锁
		if (IsItemLockedOpen())
		{
			if (pItem->GetEmbedCountLimit()>0 || pItem->GetEmbedGemCount()>0 || pItem->GetEquipAddLev()>0 || pItem->GetEquipStar()>0
				|| pItem->GetEquipQual()>3 || pItem->GetColour()>2 || pItem->IsSoulMark())
			{
				SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return false;
			}
		}
		if (pItem->IsNonsuchItem())
		{
			::MyLogSave("gmlog/sell_item", "%s(%u) sell item:[id=%u, type=%u], dur=%d, max_dur=%d", 
					this->GetName(),
					this->GetID(),
					pItem->GetID(), 
					pItem->GetInt(ITEMDATA_TYPE),
					pItem->GetInt(ITEMDATA_AMOUNT),
					pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
		}

		DEBUG_TRY	// VVVVVVVVVVVVVV

		BOOL bIsCostlyItem = pItem->IsCostlyItem();
		int nItemTypeID = 0, nItemID = 0, nItemAmount = 0;
		ItemInfoStruct info;
		NAMESTR szItemName;
		BOOL bIsItemCountable = FALSE;
		if(bIsCostlyItem)
		{
			nItemTypeID = pItem->GetTypeID();
			nItemID = pItem->GetID();
			ItemType()->GetInfo(pItem->GetTypeID(), &info);		
			strcpy(szItemName, info.szName);
			nItemAmount = pItem->GetInt(ITEMDATA_AMOUNT);
			bIsItemCountable = CItem::IsCountable(pItem->GetTypeID());
		}

		DWORD	dwMoney = pItem->GetSellPrice();
		if(pItem->GetTypeID() / 100 == 6500)
		{
			if (GetMoneyByType(1) + dwMoney > _MAX_MONEYLIMIT)
			{
				this->SendSysMsg("卖出失败,你的金钱超过上限,请减少所携带的金钱再试");
				return false;
			}
		}
		else
		{
			if (GetMoneyByType(2) + dwMoney > _MAX_MONEYLIMIT)
			{
				this->SendSysMsg("卖出失败,你的金钱超过上限,请减少所携带的金钱再试");
				return false;
			}
		}
	
		DWORD nItemType = pItem->GetTypeID();
		int nAmount = pItem->GetItemAmount();
		const char* itemName = pItem->GetStr(ITEMDATA_NAME);
		if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from selling", GetID(), GetName(), CItem::IsCountable(nItemType)?pItem->GetItemAmount():1, nItemType);
		if(!this->EraseItem(idItem, SYNCHRO_TRUE))
		{
			this->SendSysMsg("删除物品出错");
			return false;
		}
		if(bIsCostlyItem)
		{
			MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
				this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_SELL, 
				nItemTypeID, nItemID, szItemName,   
				bIsItemCountable ? nItemAmount : 1);  
		}
		BOOL bRet = FALSE;
		if(nItemType / 100 == 6500)
			bRet = this->GainMoney(dwMoney, SYNCHRO_TRUE,1);
		else
			bRet = this->GainMoney(dwMoney, SYNCHRO_TRUE,2);
		if (dwMoney>30000)
			::MyLogSave(VALUABLESLOG_FILENAME, "CUser::SellItem user:%u(%s)出售%d个%s获得银两%d",GetID(),GetName(),nAmount,itemName,dwMoney);
		if(!bRet)
			this->SendSysMsg("卖出失败,你的金钱超过上限,请减少所携带的金钱再试");//STR_MONEYBAG_FULL

	//////////////////////////////////////////////////////////////////////////
	//	20070710	朱斌	大额度金钱记录
		if(bRet && CItem::IsLogSaveMoney(dwMoney))
		{
			MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
				this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_SELL, 
				0, 0,g_objStrRes.GetStr(15072), dwMoney);// "时空币"
		}
		DEBUG_CATCH("SellItem") // AAAAAAAAAAAAAAAAA
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	CNpc* pNpc; 
	if(!GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, idNpc, IPP_OF(pNpc)))
	{
		if(idNpc == 5000)//随身药店
		{
		}
		pNpc = NpcManager()->QueryShopNpcByNPCType(idNpc);
		if (!pNpc)
			return false;		
	}
	
	if(!pNpc)
		return false;

	if(pNpc->IsRandTaskShopNpc())
	{
		CItem* pItemMoney = GetItemByType(_SPCITEM_MONEYFORSHOP);
		if(!pItemMoney)
			return false;
		
		CItem* pItem = GetItem( idItem,false );
		if(!pItem)
		{
			this->SendSysMsg("找不到物品");
			return false;
		}
		
		if(pItem->IsLocked())
			return false;

		int nHasMoney = pItemMoney->GetItemAmount();
		
						
		if(pNpc->QueryShop()->QueryRandShopSellType(pItem->GetTypeID()) != 2)
			return false;
						
		int nPrice = pNpc->QueryShop()->QueryRandShopPrice(pItem->GetTypeID());
		if(nPrice <= 0)
		{
			SendSysMsg("商店价格已改变，请重新打开商店");
			LOGWARNING("跑商物品价格出错,userid=%u,itemtype=%u,npcid=%u",GetID(),pItem->GetTypeID(),idNpc);
			return false;
		}
		
		if(nPrice != nPriceSell)
		{
			SendSysMsg("商店价格已改变，请重新打开商店");
			return false;
		}

		nPrice *= pItem->GetItemAmount();
						
						
		if(nHasMoney + nPrice > 300000)
			nPrice = 300000 - nHasMoney;

		if(this->EraseItem(idItem, SYNCHRO_TRUE))
		{
			pItemMoney->SetInt(ITEMDATA_AMOUNT, nHasMoney + nPrice);
			
			CMsgItem	msg;
			if(msg.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pItemMoney->GetID(),0,pItemMoney->GetItemAmount()))
				this->SendMsg(&msg);
		}
		UpdateMissionMsgByItemType(_SPCITEM_MONEYFORSHOP);
		return true;
	}

	if(pNpc->IsShopNpc() && pNpc->GetInt(NPCDATA_TYPE) != 0)
		return false;
	
	CItem* pItem = GetItem( idItem ,false);
	if(!pItem)
	{
		this->SendSysMsg("找不到物品");
		return false;
	}

	if(GetEquipItem(idItem))
	{
		this->SendSysMsg("不能出售穿在身上的装备");
		return false;
	}
	
	if(pItem->IsLocked())
		return false;

	if(!pItem->IsSellEnable())
		return false;

	if(pItem->GetWpSlLev() >= 3)
		return false;
	//打了孔的、镶了宝石的、强过化的、升过星的、完美及完美以上的、蓝色以及以上的,需要判断安全锁
	if (IsItemLockedOpen())
	{
		if (pItem->GetEmbedCountLimit()>0 || pItem->GetEmbedGemCount()>0 || pItem->GetEquipAddLev()>0 || pItem->GetEquipStar()>0
			|| pItem->GetEquipQual()>3 || pItem->GetColour()>2 || pItem->IsSoulMark())
		{
			SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
			return false;
		}
	}
	if (pItem->IsNonsuchItem())
	{
		::MyLogSave("gmlog/sell_item", "%s(%u) sell item:[id=%u, type=%u], dur=%d, max_dur=%d", 
				this->GetName(),
				this->GetID(),
				pItem->GetID(), 
				pItem->GetInt(ITEMDATA_TYPE),
				pItem->GetInt(ITEMDATA_AMOUNT),
				pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
	}

	DEBUG_TRY	// VVVVVVVVVVVVVV

	BOOL bIsCostlyItem = pItem->IsCostlyItem();
	int nItemTypeID = 0, nItemID = 0, nItemAmount = 0;
	ItemInfoStruct info;
	NAMESTR szItemName;
	BOOL bIsItemCountable = FALSE;
	if(bIsCostlyItem)
	{
		nItemTypeID = pItem->GetTypeID();
		nItemID = pItem->GetID();
		ItemType()->GetInfo(pItem->GetTypeID(), &info);		
		strcpy(szItemName, info.szName);
		nItemAmount = pItem->GetInt(ITEMDATA_AMOUNT);
		bIsItemCountable = CItem::IsCountable(pItem->GetTypeID());
	}

	DWORD	dwMoney = pItem->GetSellPrice();
	if(pItem->GetTypeID() / 100 == 6500)
	{
		if (GetMoneyByType(1) + dwMoney > _MAX_MONEYLIMIT)
		{
			this->SendSysMsg("卖出失败,你的金钱超过上限,请减少所携带的金钱再试");
			return false;
		}
	}
	else
	{
		if (GetMoneyByType(2) + dwMoney > _MAX_MONEYLIMIT)
		{
			this->SendSysMsg("卖出失败,你的金钱超过上限,请减少所携带的金钱再试");
			return false;
		}
	}
	
	DWORD nItemType = pItem->GetTypeID();
	int nAmount = pItem->GetItemAmount();
	const char* itemName = pItem->GetStr(ITEMDATA_NAME);
	if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from selling", GetID(), GetName(), CItem::IsCountable(nItemType)?pItem->GetItemAmount():1, nItemType);
	if(!this->EraseItem(idItem, SYNCHRO_TRUE))
	{
		this->SendSysMsg("删除物品出错");
		return false;
	}
	if(bIsCostlyItem)
	{
		MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
			this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_SELL, 
			nItemTypeID, nItemID, szItemName,   
			bIsItemCountable ? nItemAmount : 1);  
	}
	BOOL bRet = FALSE;
	if(nItemType / 100 == 6500)
		bRet = this->GainMoney(dwMoney, SYNCHRO_TRUE,1);
	else
		bRet = this->GainMoney(dwMoney, SYNCHRO_TRUE,2);
	if (dwMoney>30000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::SellItem user:%u(%s)出售%d个%s获得银两%d",GetID(),GetName(),nAmount,itemName,dwMoney);
	if(!bRet)
		this->SendSysMsg("卖出失败,你的金钱超过上限,请减少所携带的金钱再试");//STR_MONEYBAG_FULL

	//////////////////////////////////////////////////////////////////////////
	//	20070710	朱斌	大额度金钱记录
	if(bRet && CItem::IsLogSaveMoney(dwMoney))
	{
		MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
			this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_SELL, 
			0, 0,g_objStrRes.GetStr(15072), dwMoney);// "时空币"
	}
	DEBUG_CATCH("SellItem") // AAAAAAAAAAAAAAAAA
	return true;
}

//////////////////////////////////////////////////////////////////////
//王玉波修改,增加默认参数,支持丢弃叠加物品的数量问题
bool CUser::DropItem( OBJID idItem, bool bIsMustDrop/* = false*/)
{
	CItem* pFindItem = this->GetItem( idItem,false );
	if( !pFindItem  )
		return false;

	if(!pFindItem->IsDropEnable())
		return false;

	if(pFindItem->GetWpSlLev() >= 3)
		return false;
	
	//打了孔的、镶了宝石的、强过化的、升过星的、完美及完美以上的、蓝色以及以上的,需要判断安全锁
	if (IsItemLockedOpen() && !bIsMustDrop)
	{
		if (pFindItem->GetEmbedCountLimit()>0 || pFindItem->GetEmbedGemCount()>0 || pFindItem->GetEquipAddLev()>0 || pFindItem->GetEquipStar()>0
			|| pFindItem->GetEquipQual()>3 || pFindItem->GetColour()>2 || pFindItem->IsSoulMark())
		{
			SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
			return false;
		}
	}
	if( false == pFindItem->IsDropEnable() && this->IsAlive() )
	{
		this->SendSysMsg( g_objStrRes.GetStr(15073) );//"此物品不可丢弃!"
		return false;
	}
	if (pFindItem && pFindItem->IsCostlyItem())//goto 贵重物品记录
//modify code by 林德渊 begin 2011.5.21
	{
		CItem *pitem_data = GetItemByType(pFindItem->GetTypeID());
		if( pitem_data )
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from destroy %u total (%d)", GetID(), GetName(), CItem::IsCountable(pFindItem->GetTypeID())?pFindItem->GetItemAmount():1, pFindItem->GetTypeID() ,pFindItem->GetTypeID(),pitem_data->GetItemAmount());
		else
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from destroy %u total (%d)", GetID(), GetName(), CItem::IsCountable(pFindItem->GetTypeID())?pFindItem->GetItemAmount():1, pFindItem->GetTypeID() ,pFindItem->GetTypeID(),0);
	}
//modify code by 林德渊 end
	BOOL bRet = EraseItem(idItem, SYNCHRO_TRUE);

//	UpdateWeight();
	return bRet;
}
//掉时空之泪专用函数
bool CUser::DropItemBySys( OBJID idType,int x,int y )
{
// 	CItem* pItem = NULL;
// 	ItemInfoStruct info;
// 	//搜索物品(时空之泪)
// 	for( int i = 0 ;i < 128; i++ )//一个玩家最多三个// 修改很多个 [9/14/2007 %PENGFENG%]
// 	{
// 		pItem = this->GetItemByType( idType );
// 		if( !pItem )
// 			break;
// 		
// 		pItem->GetInfo( &info );
// 		EraseItem( pItem->GetID(), SYNCHRO_TRUE, false);
// 		POINT	pos;
// 		pos.x	= x;
// 		pos.y	= y;
// 		if ( GetMap()->FindDropItemCell( MONSTERDROPITEM_RANGE, &pos ) )  //得到位置
// 		{
// 			CMapItem* pMapItem = CMapItem::CreateNew();
// 			if( pMapItem )
// 			{
// 				IF_OK(pMapItem->Create(MapManager()->SpawnMapItemID(), GetMap(), pos, &info))
// 				{
// 					MapManager()->QueryMapItemSet()->AddObj(pMapItem);
// 				}
// 				else
// 				{
// 					//this->SendSysMsg(STR_MAKE_ITEM_FAILED, pMapItem->GetInfo()->idType);
// 					pMapItem->ReleaseByOwner();
// 				}
// 			}
// 		}
// 	}
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUser::DropMoney (DWORD dwMoney, int x, int y)
{
//	CHECKF(nMoney > 0);
// 	CHECKF(GetMap()->IsLayItemEnable(x, y));
// 	if(this->GetMoney() < dwMoney)
// 		return false;
// 
// 	POINT	pos;
// 	pos.x	= x;
// 	pos.y	= y;
// 	CMapItem* pMapItem = CMapItem::CreateNew();
// 	if(pMapItem)
// 	{
// 		if(pMapItem->CreateMoney(MapManager()->SpawnMapItemID(), GetMap(), pos, dwMoney,this->GetID(),0,(char*)this->GetName()))
// 		{
// 			BOOL bRet = FALSE;	
// 			bRet = this->SpendMoney(dwMoney, SYNCHRO_TRUE);				// 先删除，防止作弊
// 			//////////////////////////////////////////////////////////////////////////
// 			//	20070710	朱斌	大额度金钱记录
// 			if(bRet && CItem::IsLogSaveMoney(dwMoney))
// 			{
// 				MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 					this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_DROP, 
// 					0, 0, "时空币", dwMoney);
// 			}
// 			//	结束 20070710
// 			//////////////////////////////////////////////////////////////////////////
// 			MapManager()->QueryMapItemSet()->AddObj(pMapItem);
// 			if (this->IsAlive())
// 				this->SendSysMsg(g_objStrRes.GetStr(14049), dwMoney);//STR_DROP_MONEY_SUCC
// 			else
// 				this->SendSysMsg(g_objStrRes.GetStr(14050), dwMoney);//STR_DIE_DROP_MONEY
// 		}
// 		else
// 		{
// 			this->SendSysMsg(g_objStrRes.GetStr(12043), dwMoney);//STR_MAKE_MONEY_FAILED
// 			pMapItem->ReleaseByOwner();
// 		}
// 	}

	return true;
}

bool CUser::OrderPackage(int package)
{
	CUserPackage* pUserPack = ( CUserPackage* )QueryPackage();
	pUserPack->OrderPackage(package);
//	int nHasFlag = pUserPack->GetIndexState( package,nWant2PackageIndex );
	return true;
}
//////////////////////////////////////////////////////////////////////
//06.9.21															//
//王玉波修改														//
//此函数为玩家拆分物品												//
//////////////////////////////////////////////////////////////////////
bool CUser::SplitItem( OBJID idItem, int nNum )
{
	int nstep = 0;
	DEBUG_TRY
	CHECKF(idItem);
	CItemPtr pItem = GetItem(idItem,false);
	if(!pItem || !pItem->IsPileEnable() || nNum <= 0 || nNum >= pItem->GetInt(ITEMDATA_AMOUNT))
	{
		SendUnlockItemMsg(idItem);
		return false;
	}
	nstep = 1;
	if (pItem->IsLocked())
		return false;
	// 检查背包是否满
	if (this->IsItemFull(0, pItem->GetInt(ITEMDATA_TYPE), /*pItem->GetInt(ITEMDATA_AMOUNTLIMIT)*/1))
	{
		this->SendSysMsg(g_objStrRes.GetStr(15074)); //"背包已满,无法拆分物品!"
		SendUnlockItemMsg(idItem);
		return false;
	}

	nstep = 2;

	CItemPtr pOther = pItem->Split(nNum, Database());
	CHECKF(pOther);
	AddItem(pOther, SYNCHRO_FALSE);

	nstep = 3;
	CMsgItem	msg;
	if (msg.Create(ITEMACT_ADD,0)
		&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour())
		&& msg.Add(pOther->GetID(),pOther->GetTypeID(),pOther->GetItemAmount(),pOther->GetInt(ITEMDATA_AMOUNTLIMIT), pOther->GetPosition(),pOther->GetKidnap(),pOther->GetPackageIndex(),pOther->GetInt(ITEMDATA_BUYPRICE),pOther->GetColour()))
		SendMsg(&msg);
	nstep = 4;
	SendUnlockItemMsg(idItem);	
	return true;
	DEBUG_CATCH3("splititem error!! userid = %u nstep = %u",GetID(),nstep)
	return false;
}

//////////////////////////////////////////////////////////////////////
//06.9.21
//王玉波修改
//此函数为合并可叠加物品
bool CUser::CombineItem( OBJID idItem, OBJID idOther )
{
	CHECKF(idItem);
	CHECKF(idOther);
	if( idItem == idOther )
	{
		SendUnlockItemMsg(idItem);
		SendUnlockItemMsg(idOther);
		return false;
	}
	CItemPtr pItem = GetItem(idItem,false);
	CItemPtr pOther = GetItem(idOther,false);


	if(!pItem || !pOther)
	{
		SendUnlockItemMsg(idItem);
		SendUnlockItemMsg(idOther);
		return false;
	}
		
	if (pItem->IsLocked() || pOther->IsLocked())//只要有一个物品是锁定状态就不能合并
	{
		SendSysMsg(_TXTATR_TOPSHOW, "锁定的物品不能进行此操作");
		if (!pItem->IsLocked())//给没锁定的物品解锁
			SendUnlockItemMsg(idItem);
		if (!pOther->IsLocked())//给没锁定的物品解锁
			SendUnlockItemMsg(idOther);
		return false;
	}

	if(!pItem || !pOther || !pItem->IsPileEnable() || pItem->GetInt(ITEMDATA_TYPE) != pOther->GetInt(ITEMDATA_TYPE)
		|| pItem->GetKidnap() != pOther->GetKidnap() )
	{
		SendUnlockItemMsg(idItem);
		SendUnlockItemMsg(idOther);
		return false;
	}

	int nMaxNum = pItem->GetInt(ITEMDATA_AMOUNT_LIMIT_INIT);
	int nNewNum		= pItem->GetInt(ITEMDATA_AMOUNT) + pOther->GetInt(ITEMDATA_AMOUNT);
	if(nNewNum > nMaxNum)
	{
// 		if(pOther->GetInt(ITEMDATA_BUYPRICE) != pItem->GetInt(ITEMDATA_BUYPRICE))
// 		{
// 			DWORD dwPer = (pOther->GetInt(ITEMDATA_BUYPRICE) * (pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - pItem->GetInt(ITEMDATA_AMOUNT)) + pItem->GetInt(ITEMDATA_AMOUNT) * pItem->GetInt(ITEMDATA_BUYPRICE)) / pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
// 			pItem->SetInt(ITEMDATA_BUYPRICE,dwPer);
// 		}

		pOther->SetInt(ITEMDATA_AMOUNT, nNewNum - nMaxNum);
		if(pOther->GetItemAmountLimit() != nMaxNum)
			pOther->SetInt(ITEMDATA_AMOUNTLIMIT,nMaxNum);
		pItem->SetInt(ITEMDATA_AMOUNT, nMaxNum);
		if(pItem->GetItemAmountLimit() != nMaxNum)
			pItem->SetInt(ITEMDATA_AMOUNTLIMIT,nMaxNum);
		

		CMsgItem	msg;
		if (msg.Create(ITEMACT_ADD,0)
			&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour())
			&& msg.Add(pOther->GetID(),pOther->GetTypeID(),pOther->GetItemAmount(),pOther->GetInt(ITEMDATA_AMOUNTLIMIT), pOther->GetPosition(),pOther->GetKidnap(),pOther->GetPackageIndex(),pOther->GetInt(ITEMDATA_BUYPRICE),pOther->GetColour()))
			SendMsg(&msg);

		SendUnlockItemMsg(idItem);
		SendUnlockItemMsg(idOther);
	}
	else	//数量小于可叠加最大限制
	{
// 		if(pOther->GetInt(ITEMDATA_BUYPRICE) != pItem->GetInt(ITEMDATA_BUYPRICE))
// 		{
// 			DWORD dwPer = (pOther->GetInt(ITEMDATA_BUYPRICE) * (nNewNum - pItem->GetInt(ITEMDATA_AMOUNT)) + pItem->GetInt(ITEMDATA_AMOUNT) * pItem->GetInt(ITEMDATA_BUYPRICE)) / nNewNum;
// 			pItem->SetInt(ITEMDATA_BUYPRICE,dwPer);
// 		}

		EraseItem(idOther, SYNCHRO_TRUE, true, false, true);
		pItem->SetInt(ITEMDATA_AMOUNT, nNewNum);
		if(pItem->GetItemAmountLimit() != nMaxNum)
			pItem->SetInt(ITEMDATA_AMOUNTLIMIT,nMaxNum);

		CMsgItem	msg;
		if (msg.Create(ITEMACT_ADD,0)
			&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour()))
			SendMsg(&msg);

		SendUnlockItemMsg(idItem);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::CombineNewItem	(CItem* pItem, ItemInfoStruct* pInfo, bool bSynchro)
{
	CHECKF(CItem::IsCountable(pInfo->idType));
	CHECKF(pItem->GetInt(ITEMDATA_TYPE) == pInfo->idType);

	int nMaxNum = pItem->GetInt(ITEMDATA_AMOUNT_LIMIT_INIT);
	int nNewNum		= pItem->GetInt(ITEMDATA_AMOUNT) + pInfo->nAmount;
	if(nNewNum > nMaxNum)
	{
		// other
		pInfo->nAmount	= nNewNum - nMaxNum;

		CItemPtr pOther = CItem::CreateNew();
		CHECKF(pOther);
		if(!pOther->Create(Database(), 
			GameDataDefault()->GetGameItemData(), 
			pInfo, 
			GameDataDefault()->GetEudemonData(),
			/*GameDataDefault()->GetWeaponSoulData()*/NULL//20071113修罗:武魂
			))
		{
			pOther->ReleaseByOwner();
			return NULL;
		}

		pOther->SetInt(ITEMDATA_BUYPRICE,pInfo->dwPerPrice);
		if(pOther->GetItemAmountLimit() != nMaxNum)
			pOther->SetInt(ITEMDATA_AMOUNTLIMIT,nMaxNum);

		this->AddItem(pOther, bSynchro);

		// item
		if(pInfo->dwPerPrice != pItem->GetInt(ITEMDATA_BUYPRICE))
		{
			int nAddAmount = nMaxNum - pItem->GetInt(ITEMDATA_AMOUNT);
			DWORD dwPer = (pInfo->dwPerPrice * nAddAmount + pItem->GetInt(ITEMDATA_AMOUNT) * pItem->GetInt(ITEMDATA_BUYPRICE)) / (nAddAmount + pItem->GetInt(ITEMDATA_AMOUNT));
			pItem->SetInt(ITEMDATA_BUYPRICE,dwPer);
		}

		pItem->SetInt(ITEMDATA_AMOUNT, nMaxNum);
		if(pItem->GetItemAmountLimit() != nMaxNum)
			pItem->SetInt(ITEMDATA_AMOUNTLIMIT,nMaxNum);

		if(bSynchro)
		{
			CMsgItem	msg;
			if (msg.Create(ITEMACT_SYNCHRO_AMOUNT,0)
				&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour()))
			SendMsg(&msg);
// 			CMsgItem	msg;
// 			IF_OK(msg.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pItem->GetID(),0,pItem->GetItemAmount()))
// 				SendMsg(&msg);
		}

		return pOther;
	}
	else
	{
		// item
		if(pInfo->dwPerPrice != pItem->GetInt(ITEMDATA_BUYPRICE))
		{
			int nAddAmount = pInfo->nAmount;
			DWORD dwPer = (pInfo->dwPerPrice * nAddAmount + pItem->GetInt(ITEMDATA_AMOUNT) * pItem->GetInt(ITEMDATA_BUYPRICE)) / nNewNum;
			pItem->SetInt(ITEMDATA_BUYPRICE,dwPer);
		}
		
		pItem->SetInt(ITEMDATA_AMOUNT, nNewNum);
		if(pItem->GetItemAmountLimit() != nMaxNum)
			pItem->SetInt(ITEMDATA_AMOUNTLIMIT,nMaxNum);

		UpdateMissionMsgByItemType(pItem->GetTypeID());
		if(bSynchro)
		{
			CMsgItem	msg;
			if (msg.Create(ITEMACT_SYNCHRO_AMOUNT,0)
				&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour()))
			SendMsg(&msg);
// 			CMsgItem	msg;
// 			IF_OK(msg.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pItem->GetID(),0,pItem->GetItemAmount()))
// 				SendMsg(&msg);
		}

		return pItem;
	}
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::FindCombineItem	(OBJID idType)
{
	return m_pPackage->FindCombineItem(idType);
}

//////////////////////////////////////////////////////////////////////
bool CUser::SpendArrow()
{
	return true;
}

bool CUser::SpendOneItem(OBJID idItem)//消费一个道具,供脚本使用
{
	CItem* pItem=GetItem(idItem,false);
	CHECKF(pItem);
	return SpendItem(pItem);
}

//////////////////////////////////////////////////////////////////////
bool CUser::SpendItem(CItem* pItem, int nNum/*=1*/, int nPosition/*=ITEMPOSITION_BACKPACK*/, bool bSynchro/*=true*/)	// may be erase
{
	if(pItem->IsPileEnable() && pItem->GetInt(ITEMDATA_AMOUNT) > nNum)
	{
		pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT) - nNum);

		if(bSynchro)
		{
			CMsgItem	msg;
			if(msg.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pItem->GetID(),0,pItem->GetItemAmount()))
				this->SendMsg(&msg);
		}
	}
	else
	{
		if(pItem->IsPileEnable())
			ASSERT(pItem->GetInt(ITEMDATA_AMOUNT) == nNum);

		if(nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition <= ITEMPOSITION_PACK_END)
			EraseItem(pItem->GetID(), bSynchro);
		else
		{
			LOGWARNING("为什么要消耗装备？？");
			ASSERT(nPosition >= ITEMPOSITION_EQUIPBEGIN && nPosition <= ITEMPOSITION_EQUIPEND);
			EraseEquip(nPosition, bSynchro);
		}
	}
	return true;
}
BOOL CUser::SpendNewEdemonRemedy( CItem* pItem, int nPosition, bool bSynchro)
{
	CHECKF(pItem);
	if( 0 == pItem->GetInt(ITEMDATA_AMOUNT) )
		return false;
	
	if(!GetMap())
		return false;
	if (GetMap()->IsNotAllowNewRemedy())
		return false;

	if( this->QueryStatus(STATUS_BIGBLOOD_USERNOT) )
		return false;
	
	
	int	nLife	= pItem->GetInt(ITEMDATA_LIFE);
	int nMana	= pItem->GetInt(ITEMDATA_MANA);
	if( 0 == nLife && 0 == nMana)
		return false;
	int nTypeID		= pItem->GetInt( ITEMDATA_TYPE );
	int nKind = ( nTypeID / 100 ) % 10;
	int nAdd  = pItem->GetInt( ITEMDATA_AMOUNT );

// 	switch( nKind )//区分种类
// 	{
// 	case 0://红
// 		{
// 			int need = 0;
// 			int nTempData = 0;
// 
// 			bool bFirst = false;
// 			int firstNeed = 0;
// 			bool bSecond = false;
// 			int secondNeed = 0;
// 			if(m_eudemonData[0].id && m_eudemonData[0].pMonster)
// 			{
// 				nTempData = m_eudemonData[0].pMonster->GetMaxLife() - m_eudemonData[0].pMonster->GetLife();
// 				if(m_eudemonData[0].pMonster->GetLife() > 0 && nTempData > 0)
// 				{
// 					bFirst = true;
// 					firstNeed = nTempData;
// 					need = nTempData;
// 				}
// 			}
// 				
// 			if(need <= 0)
// 			{
// 				return false;
// 			}
// 
// 			if( nAdd >= need )
// 			{
// 				nAdd -= need;
// 				if(bFirst)
// 					m_eudemonData[0].pMonster->AddAttrib(_USERATTRIB_LIFE, firstNeed, SYNCHRO_TRUE);
// 				pItem->SetInt( ITEMDATA_AMOUNT, nAdd );
// 			}
// 			else
// 			{
// 				if(bFirst)
// 					m_eudemonData[0].pMonster->AddAttrib(_USERATTRIB_LIFE, nAdd, SYNCHRO_TRUE);
// 				pItem->SetInt( ITEMDATA_AMOUNT, 0 );
// 			}
// 		}
// 		break;
// 	case 1://蓝
// 		{
// 			int need = 0;
// 			int nTempData = 0;
// 			
// 			bool bFirst = false;
// 			int firstNeed = 0;
// 			bool bSecond = false;
// 			int secondNeed = 0;
// 			if(m_eudemonData[0].id && m_eudemonData[0].pMonster)
// 			{
// 				nTempData = m_eudemonData[0].pMonster->GetMaxMana() - m_eudemonData[0].pMonster->GetMana();
// 				if(m_eudemonData[0].pMonster->GetLife() > 0 && nTempData > 0)
// 				{
// 					bFirst = true;
// 					firstNeed = nTempData;
// 					need = nTempData;
// 				}
// 			}
// 			
// 			if(need <= 0)
// 			{
// 				return false;
// 			}
// 			
// 			if( nAdd >= need )
// 			{
// 				nAdd -= need;
// 				if(bFirst)
// 					m_eudemonData[0].pMonster->AddAttrib(_USERATTRIB_MANA, firstNeed, SYNCHRO_TRUE);
// 				pItem->SetInt( ITEMDATA_AMOUNT, nAdd );
// 			}
// 			else
// 			{
// 				if(bFirst)
// 					m_eudemonData[0].pMonster->AddAttrib(_USERATTRIB_MANA, nAdd, SYNCHRO_TRUE);
// 				pItem->SetInt( ITEMDATA_AMOUNT, 0 );
// 			}
// 		}
// 		break;
// 	}
	CMsgItem	msg;
	IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
		SendMsg(&msg);

	if( 0 == pItem->GetInt(ITEMDATA_AMOUNT) )
	{
		EraseItem(pItem->GetID(), bSynchro);
	}
	return true;
}
BOOL CUser::SpendNewRemedy( CItem* pItem, int nPosition/*=ITEMPOSITION_BACKPACK*/, bool bSynchro/*=true*/,CUserPetData* pData,int nMaxLife /*= 0*/)	//---彭峰-----2007/03/26----针对喝不完的血瓶写的使用函数-----------
{
	CHECKF(pItem);
	if( 0 == pItem->GetInt(ITEMDATA_AMOUNT) )// 彭峰 [7/3/2007]
		return false;

	if(!GetMap())
		return false;
	if (GetMap()->IsNotAllowNewRemedy())
		return false;

	// 新BUF,不能使用增值药水 [1/15/2008 陈剑飞]
	if( this->QueryStatus(STATUS_BIGBLOOD_USERNOT) )
		return false;
	
	int	nLife	= pItem->GetInt(ITEMDATA_LIFE);
	int nMana	= pItem->GetInt(ITEMDATA_MANA);
	if( 0 == nLife && 0 == nMana)
		return false;
	int nTypeID		= pItem->GetInt( ITEMDATA_TYPE );
	int nKind = ( nTypeID / 10000 ) % 10;
	int nAdd  = pItem->GetInt( ITEMDATA_AMOUNT );

	if(nPosition == ITEMPOSITION_PACK_BEGIN)
	{
		switch( nKind )//区分种类
		{
		case 0://红
			{
				int need = min(nMana , GetMaxLife() - GetLife());
				//2007106修罗:bug大红剩余量会增加-------
				if(need <= 0)
				{
					return false;
				}
				//--------------------------------------
				if( nAdd >= need )
				{
					nAdd -= need;
					AddAttrib(_USERATTRIB_LIFE, need, SYNCHRO_TRUE);
					pItem->SetInt( ITEMDATA_AMOUNT, nAdd);
					this->BroadcastTeamLife();
				}
				else
				{
					pItem->SetInt( ITEMDATA_AMOUNT, 0 );
					AddAttrib(_USERATTRIB_LIFE, nAdd, SYNCHRO_TRUE);
					this->BroadcastTeamLife();
				}
			}
			break;
		case 1://蓝
			{
				int need = min(nLife , GetMaxMana() - GetMana());
				//2007106修罗:bug大红剩余量会增加-------
				if(need <= 0)
				{
					return false;
				}
				//--------------------------------------
				if( nAdd >= need )
				{
					nAdd -= need;
					pItem->SetInt( ITEMDATA_AMOUNT, nAdd );
					AddAttrib(_USERATTRIB_MANA, need, SYNCHRO_TRUE);
					this->BroadcastTeamMana();
				}
				else
				{
					pItem->SetInt( ITEMDATA_AMOUNT, 0 );
					AddAttrib(_USERATTRIB_MANA, nAdd, SYNCHRO_TRUE);
					this->BroadcastTeamMana();
				}
			}
			break;
		case 2://宠物红
			{
				int need = min(nMana , nMaxLife - pData->GetInt(EUDEMONDATA_life));
				int amount = nMaxLife;
				//2007106修罗:bug大红剩余量会增加-------
				if(need <= 0)
				{
					return false;
				}
				//--------------------------------------
				if( nAdd >= need )
				{
					nAdd -= need;
					if(m_pWarPet && pData->GetID() == m_pWarPet->GetID() )
					{
						m_pWarPet->AddAttrib(_USERATTRIB_LIFE, need, SYNCHRO_TRUE);
					}
					else
					{
						pData->SetInt(EUDEMONDATA_life,amount,true);	

						CMsgEudemonAttrib msg;
						if(msg.Create(pData->GetID(),EUDATTR_LIFE,pData->GetInt(EUDEMONDATA_life)))
							SendMsg(&msg);
					}
					pItem->SetInt( ITEMDATA_AMOUNT, nAdd );
				}
				else
				{
					amount = pData->GetInt(EUDEMONDATA_life) + nAdd;
					if(m_pWarPet && pData->GetID() == m_pWarPet->GetID() )
					{
						m_pWarPet->AddAttrib(_USERATTRIB_LIFE, nAdd, SYNCHRO_TRUE);
					}
					else
					{
						pData->SetInt(EUDEMONDATA_life,amount,true);	

						CMsgEudemonAttrib msg;
						if(msg.Create(pData->GetID(),EUDATTR_LIFE,pData->GetInt(EUDEMONDATA_life)))
							SendMsg(&msg);
					}
					pItem->SetInt( ITEMDATA_AMOUNT, 0 );
				}
			}
			break;
		}
	}

	if( 0 == pItem->GetInt(ITEMDATA_AMOUNT) )
	{
		EraseItem(pItem->GetID(), bSynchro);
	}

//	UpdateWeight();
	return true;
}
//////////////////////////////////////////////////////////////////////
//[2010-08-27 goto]//特殊大红大蓝，自动回复，不需手动使用
bool CUser::SpendNewRemedy()
{
	if (!IsAlive())
		return false;
	int nAddLife = GetHpAdd();
	int nAddMana = GetMpAdd();
	int nAddPet  = GetPetAdd();
	bool bNeedSend = false;
	
	if (nAddLife>0)
	{
		int nLostLife = GetMaxLife()-GetLife();
		if (nLostLife>0)
		{
			if (nAddLife%10==1 && nLostLife>5000)//大瓶最多加5000
				nLostLife = 5000;
			if (nAddLife%10==2 && nLostLife>10000)//小瓶最多加10000
				nLostLife = 10000;
			if (nAddLife/10 > nLostLife)
			{
				AddAttrib(_USERATTRIB_LIFE, nLostLife, SYNCHRO_TRUE);
				this->BroadcastTeamLife();
// 				bool bUpdate = false;
// 				if (nAddLife%10000 < nLostLife)//每过一万同步一次数据库
// 					bUpdate = true;
				SetHpAdd((nAddLife/10-nLostLife)*10+nAddLife%10);
				//同步客户端
				bNeedSend = true;
			}
			else
			{
				AddAttrib(_USERATTRIB_LIFE, nAddLife/10, SYNCHRO_TRUE);
				this->BroadcastTeamLife();
				SetHpAdd(0);
				//同步客户端
				bNeedSend = true;
			}
		}
	}
	if (nAddMana>0)
	{
		int nLostMana = GetMaxMana()-GetMana();
		if (nLostMana>0)
		{
			if (nAddMana%10==1 && nLostMana>2000)//大瓶最多加2000
				nLostMana = 2000;
			if (nAddMana%10==2 && nLostMana>4000)//小瓶最多加4000
				nLostMana = 4000;
			if (nAddMana/10 > nLostMana)
			{
				AddAttrib(_USERATTRIB_MANA, nLostMana, SYNCHRO_TRUE);
				this->BroadcastTeamMana();			
				SetMpAdd((nAddMana/10-nLostMana)*10+nAddMana%10);
				//同步客户端
				bNeedSend = true;
			}
			else
			{
				AddAttrib(_USERATTRIB_MANA, nAddMana/10, SYNCHRO_TRUE);
				this->BroadcastTeamMana();
				SetMpAdd(0);
				//同步客户端
				bNeedSend = true;
			}
		}
	}
	if (nAddPet>0)
	{
		CMonster* pMonster = QueryCallPet();//获得出战的宠物
		if (pMonster)//没有出战的宠物
		{
			int nLostLife = pMonster->GetMaxLife()-pMonster->GetLife();
			if (nLostLife>0)
			{
				if (nLostLife>2500)//最多只能加2500
					nLostLife = 2500;
				if (nAddPet > nLostLife)
				{
					pMonster->AddAttrib(_USERATTRIB_LIFE, nLostLife, SYNCHRO_TRUE);
					SetPetAdd(nAddPet-nLostLife);
					//同步客户端
					bNeedSend = true;
				}
				else
				{
					pMonster->AddAttrib(_USERATTRIB_LIFE, nAddPet, SYNCHRO_TRUE);
					SetPetAdd(0);
					//同步客户端
					bNeedSend = true;
				}
			}
		}
	}
	if (bNeedSend)
	{
		CMsgUserAttrib	msg;
		if (msg.Create(this->GetID(), _USERATTRIB_DRUG_ADDHP, GetHpAdd())
			&& msg.Append(_USERATTRIB_DRUG_ADDMP, GetMpAdd())
			&& msg.Append(_USERATTRIB_DRUG_ADDPET, GetPetAdd()))
			SendMsg(&msg);
	}
	return true;
}


bool CUser::RepairItem( OBJID NpcID,OBJID itemID,bool bSynchro )
{
	return false;
	CNpc* pNpc; 
	if(!GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, NpcID, IPP_OF(pNpc)))
	{
		if(NpcID == 5000)//随身药店
		{
		}
		return false;		
	}

	if(!pNpc || pNpc->GetInt(NPCDATA_TYPE) != 0)
		return false;
	
	
	CItem* pItem = GetItem( itemID );
	if(!( pItem && pItem->CanRepair() && pItem->GetInt(ITEMDATA_AMOUNTLIMIT) > pItem->GetInt(ITEMDATA_AMOUNT) ))
	{
		this->SendSysMsg("修理失败");	//修理失败STR_REPAIR_FAILED
		return false;
	}

	if(pItem->IsLocked())
		return false;

	BOOL bIsValidOld = pItem->IsValidEquip();
	
	int nMoneyNeed		= pItem->CalcRepairMoney();
	int	nRepair		= pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - pItem->GetInt(ITEMDATA_AMOUNT);
	
	if(nMoneyNeed <= 0 || nRepair <= 0)
		return false;
	
	if (!this->SpendMoney(nMoneyNeed, SYNCHRO_TRUE))
	{
		this->SendSysMsg("身上银两不足");
		return false;	
	}

	int nAmount = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
	pItem->SetInt(ITEMDATA_AMOUNT, nAmount);
	if ( bSynchro )
	{
		CMsgItem msg;
		if (msg.Create(ITEMACT_SYNCHRO_AMOUNT,0)
			&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex()))
			this->SendMsg(&msg);
	}	


	char szTemp[1024];
	char szMoney[1024] = "";
	int nMoney = nMoneyNeed / 10000;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d金",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = (nMoneyNeed % 10000) / 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d银",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = nMoneyNeed % 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d铜",nMoney);
		strcat(szMoney,szTemp);
	}
	SendSysMsg(_TXTATR_TOPSHOW,"修理成功,花费%s", szMoney);

	if(bIsValidOld != pItem->IsValidEquip())
	{
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//void CUser::ProcessFixHammer(OBJID itemID)
//{
//	CItem* pItem = GetItem( itemID );
//	if (!pItem)
//	{
//		return;
//	}
//	int nAmount	 = pItem->GetInt(ITEMDATA_AMOUNT);
//	//For Test
//	int i = pItem->GetInt(ITEMDATA_TYPE);
//	if (pItem->GetInt(ITEMDATA_TYPE)/*GetTypeID*/ == 930011)
//	{
//		//DelItem(itemID,true);
//		EraseItem(itemID,true,true);
//		return;
//		//pItem->SetInt(ITEMDATA_AMOUNT, --nAmount, true);
//	}
//	if (pItem->GetInt(ITEMDATA_TYPE)/*GetTypeID*/ == 930010)
//	{
//		if (nAmount > 1)
//		{
//			pItem->SetInt(ITEMDATA_AMOUNT, --nAmount, true);
//		}
//		else
//		{
//			//DelItem(itemID,true);
//			EraseItem(itemID,true,true);
//			return;
//		}
//	}
//}
////////////////////////////////////////////////////////////////////////

bool CUser::RepairOneSpecial(OBJID nEquipPos,bool bSynchro)//独孤求败[2008/11/18]单件特修身上的装备
{
	//CItem* pItem = GetItem( itemID );
	return false;
	CItem* pItem = GetEquipItemByPos(nEquipPos);
			
	//20070705修罗:可修理判断
	//if(!( pItem && pItem->IsRepairEnable() && pItem->GetInt(ITEMDATA_AMOUNTLIMIT) > pItem->GetInt(ITEMDATA_AMOUNT) ))
	if(!( pItem && pItem->CanRepair() && pItem->GetInt(ITEMDATA_AMOUNTLIMIT) > pItem->GetInt(ITEMDATA_AMOUNT) ))
	{
		this->SendSysMsg(g_objStrRes.GetStr(12089));	//修理失败STR_REPAIR_FAILED
		return false;
	}

	//2007918修罗:有效性转变----
	BOOL bIsValidOld = pItem->IsValidEquip();
	//--------------------------
	
	int nMoney		= pItem->CalcRepairMoney();
	int	nRepair		= pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - pItem->GetInt(ITEMDATA_AMOUNT);
	
	if(nMoney <= 0 || nRepair <= 0)
		return false;
	
	if (!this->SpendMoney(nMoney, SYNCHRO_TRUE))
	{
		this->SendSysMsg(g_objStrRes.GetStr(12090), nMoney);//金钱不够，直接return false;STR_REPAIR_NO_MONEY_d
		return false;	
	}
	int nAmount = pItem->GetInt(ITEMDATA_AMOUNTLIMIT)/**0.95*/;// 还原持久上限值[2008/11/18]独孤求败
	pItem->SetInt(ITEMDATA_AMOUNT, nAmount); //设定新的持久为持久上限
	//  [4/16/2007] 增加 普通修理时装备修理 改变物品的耐久 哈雷
	//[游途道标 2009.05.04]不动上限
	//pItem->SetInt(ITEMDATA_AMOUNTLIMIT,nAmount,true);
	
	if ( bSynchro )
	{
		//		CMsgItemInfo msg;
		//		CHECKF(msg.Create(pItem, ITEMINFO_UPDATE));
		//		SendMsg(&msg);
		/////////////////////////////////////王玉波注释以上并修改成:
		CMsgItem	msg;
		IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
			SendMsg(&msg);
		CMsgItem    msg1;// 陈剑飞 发送武器上限消息给客户端[4/29/2007]
		IF_OK(msg1.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNTLIMIT, pItem->GetInt(ITEMDATA_AMOUNTLIMIT)))
			SendMsg(&msg1);
	}	

	//2007918修罗:有效性转变----
	if(bIsValidOld != pItem->IsValidEquip())
	{
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//更新角色卡
	}
	//--------------------------
	
	return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

bool CUser::RepairAllSpecial(bool bSynchro,bool bWithHammer,OBJID hammerID)//独孤求败[2008/11/18]全身特修
{
	return false;
	if (!bWithHammer)//如果不用特修锤修理，就要游戏币[2008/11/20]
	{
		DWORD dwNeedMoney = 0;
		//计算修理共需花费多少钱
		for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND - 5; i++)//[游途道标 2009.04.17]排除马弹药之类的
		{
			CItem* pEquip = GetEquipItemByPos(i);
			if(pEquip)
				dwNeedMoney	+= pEquip->CalcRepairMoney();
		}
		//如果没有足够的钱
		if( false == this->SpendMoney( dwNeedMoney,SYNCHRO_TRUE ) )
		{
			this->SendSysMsg( g_objStrRes.GetStr(15075) );//"金钱不足,修理失败!"
			return false;
		}
		//如果有足够的钱
		else
		{
			if(bSynchro)//如果同步就传回客户端总共要花去的金钱
			{
				CMsgItem	msg;
				CHECKF(msg.Create(dwNeedMoney, ITEMACT_REPAIRALL));
				SendMsg(&msg);	
			}
		}
	}
	else
	{		
		//获取修理锤
		CItem* pHammer = GetItem(hammerID);
		if (!pHammer)
		{
			this->SendSysMsg("修理锤错误");
			return false;
		}
		int nAmount	 = pHammer->GetInt(ITEMDATA_AMOUNT);	
		if (pHammer->GetInt(ITEMDATA_TYPE) == 900300)
		{
			//[游途道标 2009.05.08]
			bool bHasKind = false;
			DelItemByTypeMulti(hammerID, 1,bHasKind, true);
			/*ASSERT(nAmount == 1);
			EraseItem(hammerID,true,true);*/
		}
		else if (pHammer->GetInt(ITEMDATA_TYPE) == 900400)
		{
			if (nAmount > 1)
			{
				pHammer->SetInt(ITEMDATA_AMOUNT, --nAmount);
				CMsgItem	msg;
				IF_OK(msg.Create(pHammer->GetID(), ITEMACT_SYNCHRO_AMOUNT, pHammer->GetInt(ITEMDATA_AMOUNT)))
					SendMsg(&msg);
				this->SendSysMsg("修理成功");
			}
			else
			{
				//DelItem(itemID,true);
				EraseItem(hammerID,true,true);
				this->SendSysMsg("修理成功");
			}
		}
	}
	//2007918修罗:有效性转变----
	int nIsValidOldBit = 0;//有效性掩码
	int nIsValidNewBit = 0;
	//--------------------------
	for( int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND-5; i++)//[游途道标 2009.04.17]//20060913修罗:弹药不修复
	{
		CItem* pEquip = GetEquipItemByPos(i);
		if(pEquip && pEquip->CalcRepairMoney() > 0)
		{
			//2007918修罗:有效性转变----
			//统计有效性
			//有任何一件装备有所改变,则更新角色卡
			BOOL bIVO = pEquip->IsValidEquip();
			nIsValidOldBit |= bIVO<<i;
			//--------------------------

			pEquip->SetInt(ITEMDATA_AMOUNT, (int)(pEquip->GetInt(ITEMDATA_AMOUNTLIMIT)/**0.95*/));// 特殊修理:还原为上限值[2008/11/18]独孤求败
			pEquip->SetInt( ITEMDATA_AMOUNTLIMIT,pEquip->GetInt(ITEMDATA_AMOUNT) );
			//  [4/16/2007] 哈雷 增加普通修理时，修改装备耐久上限。
			CMsgItem	msg1;
			IF_OK(msg1.Create( pEquip->GetID() , ITEMACT_SYNCHRO_AMOUNT, pEquip->GetInt(ITEMDATA_AMOUNT)))
			SendMsg(&msg1);
			CMsgItem    msg2;// 陈剑飞 发送武器上限消息给客户端//  [5/23/2007]
			IF_OK(msg2.Create( pEquip->GetID(), ITEMACT_SYNCHRO_AMOUNTLIMIT, pEquip->GetInt(ITEMDATA_AMOUNTLIMIT)))
			SendMsg(&msg2);

			//2007918修罗:有效性转变----
			BOOL bIVN = pEquip->IsValidEquip();
			nIsValidNewBit |= bIVN<<i;
			//--------------------------
		}					
	}
	//2007918修罗:有效性转变----
	if(nIsValidOldBit != nIsValidNewBit)
	{
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
	}
	//--------------------------
	return true;
}
//////////////////////////////////////////////////////////////////////////

bool CUser::RepairSpecial( OBJID itemID, bool bSynchro )//  [4/16/2007] 哈雷 装备特殊修理
{
	return false;
	CItem* pItem = GetItem( itemID );	
	if(!( pItem && pItem->IsRepairEnable() && pItem->GetInt(ITEMDATA_AMOUNTLIMIT) > pItem->GetInt(ITEMDATA_AMOUNT) ))
	{
		this->SendSysMsg(g_objStrRes.GetStr(12089));	//修理失败STR_REPAIR_FAILED
		return false;
	}

	//2007918修罗:有效性转变----
	BOOL bIsValidOld = pItem->IsValidEquip();
	//--------------------------


	int	nRepair		= pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - pItem->GetInt(ITEMDATA_AMOUNT);
	if( nRepair <= 0)
		return false;
	pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNTLIMIT)); //设定新的持久为持久上限
	if ( bSynchro )
	{
		//		CMsgItemInfo msg;
		//		CHECKF(msg.Create(pItem, ITEMINFO_UPDATE));
		//		SendMsg(&msg);
		/////////////////////////////////////王玉波注释以上并修改成:
		CMsgItem	msg;
		IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
			SendMsg(&msg);
	}	


	//2007918修罗:有效性转变----
	if(bIsValidOld != pItem->IsValidEquip())
	{
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//更新角色卡
	}
	//--------------------------

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::RepairAll(OBJID NpcID, bool bSynchro )
{
	return false;
	CNpc* pNpc; 
	if(!GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, NpcID, IPP_OF(pNpc)))
	{
		if(NpcID == 5000)//随身药店
		{
		}
		return false;		
	}
	
	if(!pNpc || pNpc->GetInt(NPCDATA_TYPE) != 0)
		return false;

	
	DWORD dwNeedMoney = 0;
	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		CItem* pEquip = GetEquipItemByPos(i);
		if(pEquip)
			dwNeedMoney	+= pEquip->CalcRepairMoney();
	}
	if( false == this->SpendMoney( dwNeedMoney,SYNCHRO_TRUE ) )
	{
		this->SendSysMsg("金钱不足,修理失败");
		return false;
	}

	int nIsValidOldBit = 0;
	int nIsValidNewBit = 0;
	for( i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND-1; i++)
	{
		CItem* pEquip = GetEquipItemByPos(i);
		if(pEquip && pEquip->CalcRepairMoney() > 0)
		{
			BOOL bIVO = pEquip->IsValidEquip();
			nIsValidOldBit |= bIVO<<i;
			pEquip->SetInt(ITEMDATA_AMOUNT, pEquip->GetInt(ITEMDATA_AMOUNTLIMIT));

			CMsgItem msg;
			if (msg.Create(ITEMACT_SYNCHRO_AMOUNT,0)
				&& msg.Add(pEquip->GetID(),pEquip->GetTypeID(),pEquip->GetItemAmount(),pEquip->GetInt(ITEMDATA_AMOUNTLIMIT), pEquip->GetPosition(),pEquip->GetKidnap(),pEquip->GetPackageIndex()))
				this->SendMsg(&msg);

			BOOL bIVN = pEquip->IsValidEquip();
			nIsValidNewBit |= bIVN<<i;
		}
	}

	char szTemp[1024];
	char szMoney[1024] = "";
	int nMoney = dwNeedMoney / 10000;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d金",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = (dwNeedMoney % 10000) / 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d银",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = dwNeedMoney % 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d铜",nMoney);
		strcat(szMoney,szTemp);
	}
	SendSysMsg(_TXTATR_TOPSHOW,"修理成功,花费%s", szMoney);

	if(nIsValidOldBit != nIsValidNewBit)
	{
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
//[2010-07-21 goto]改为100%成功，只需要装备、镶嵌石和宝石
bool CUser::EmbedGem( OBJID idEquip, OBJID idStone, OBJID idGem )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return false;
	}
	//获得装备
	CItem* pEquip = GetItem( idEquip ,false);
	if(!pEquip)
		pEquip = GetEquipItem(idEquip);

	if(!pEquip || !pEquip->IsCanEmbedEquip() || pEquip->IsLocked())
		return false;
	if (pEquip->GetTypeID()==144000)//湛卢剑
		return false;
	//获得宝石
	CItem* pGem	= GetItem( idGem,false );
	if(!pGem || !pGem->IsGem() || pGem->IsLocked())
		return false;
// 	int icolor = pGem->GetColour();
// 	char Ccolor[32];
// 	if (0 == icolor)
// 	{
// 		strcpy(Ccolor, "<font color='#ffffff'>");
// 	}
// 	else if(1 == icolor)
// 	{
// 		strcpy(Ccolor,"<font color='#00ff00'>");
// 	}
// 	else if (2 == icolor)
// 	{
// 		strcpy(Ccolor, "<font color='#0098FF'>");
// 	}
// 	else if (3 == icolor)
// 	{
// 		strcpy(Ccolor, "<font color='#9727ff'>");
// 	}
// 	else if (4 == icolor)
// 	{
// 		strcpy(Ccolor, "<font color='#FF6532'>");
// 	}
	//镶嵌石
	CItem* pStone = GetItem(idStone,false);
	if (!pStone || pStone->GetTypeID()!=610018 || pStone->IsLocked())
		return false;

	int nPos = pEquip->GetEmbedPos();
	if( 0 == nPos )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "装备上没有可镶嵌宝石的孔");
		return false;
	}

	//攻击类的装备只能镶嵌攻击类的宝石，防御类的装备只能镶嵌防御类的宝石
	if (!pEquip->IsRightGemType(pGem->GetTypeID()))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "宝石种类与装备种类不符");
		return false;
	}
	
	if (pEquip->IsHaveSameGemEmbed(pGem->GetTypeID()))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "一件装备上不能镶嵌两个同类的宝石");
		return false;
	}
	if (GetMoney()<COMP_EMBEDGEM_COST)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return false;
	}
	int nGemLev = pGem->GetTypeID()%100;
	int nGemType = pGem->GetTypeID();
	//扣钱
	if (!SpendMoney(COMP_EMBEDGEM_COST, SYNCHRO_TRUE))
		return false;

	pEquip->SetGemStatus(nPos, pGem->GetTypeID());
	if (!pEquip->GetKidnap() && pGem->GetKidnap())//如果宝石是绑定的，将镶嵌后的装备也绑定
		pEquip->SetKidnap(1);
	bool bIsCostly = pGem->IsCostlyItem();
	bool bIsCostStone = pStone->IsCostlyItem();
	int nStoneType = pStone->GetTypeID();
// 	ASSERT(EraseItem(idGem, SYNCHRO_TRUE));//删除宝石
// 	ASSERT(EraseItem(idStone, SYNCHRO_TRUE));//删除镶嵌石
	if (!SpendItem(pGem) || !SpendItem(pStone))
		return false;

	CMsgCompound msg;//发送成功消息
	IF_OK( msg.Create( MSGCOMPOUND_EMBED_SUC, idEquip, 0 ) )
		this->SendMsg(&msg);
	SendSysMsg("镶嵌成功");
	int nPosEquip = pEquip->GetPosition();
	if(nPosEquip >= ITEMPOSITION_EQUIPBEGIN && nPosEquip < ITEMPOSITION_EQUIPEND)
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
	if (nGemLev>2)
	{
		char szMsg[255];
		char UserName[128] = "";
		char itemName[128] = "";
		int GemColor = ItemType()->QueryItemType(nGemType)->GetInt(ITEMTYPEDATA_COLOR_INIT);
		sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
		if(pEquip->IsWeaponSoul())
		{
			if(pEquip->GetWpSlType() == 1)
				sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阳之魄", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			else
				sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阴之魂", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());		
		}
		else
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());

		sprintf(szMsg, "<%s>向<%s>镶嵌了一颗<1_[%s]_0_%d_0_0_%d>，大幅提升了装备的能力!",
			UserName, itemName , ItemType()->QueryItemType(nGemType)->GetStr(ITEMTYPEDATA_NAME), nGemType, GemColor, GemColor);
//		char szItem[128];
//		sprintf(szItem,"%d_%d_%s_%d_%d_%d",
//			pEquip->GetID(), pEquip->GetTypeID(), ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), this->GetID(), pEquip->GetKidnap(),pEquip->GetColour());
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);
		if (nGemLev>=4)
		{
			CMsgPlayer msgPlayer;
			msgPlayer.Create(this);
			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
	}
	if (bIsCostly)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), nGemType);
	if (bIsCostStone)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), nStoneType);
	//通知客户端更新物品信息
// 	CMsgItem	msgItem;
// 	msgItem.Create(ITEMACT_SENDINFO,0);
// 	msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 	this->SendMsg(&msgItem);
	return true;
}

//////////////////////////////////////////////////////////////////////
//从孔中取出最后一次镶入的宝石..
//06.6.6王玉波修改
//[2010-07-21 goto]从指定的孔中取出宝石
bool CUser::TakeOutGem( OBJID idEquip, OBJID stoneID, int nPos )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return false;
	}

	CItem* pEquip = GetItem(idEquip,false);
	if(!pEquip)
		pEquip = GetEquipItem(idEquip);

	if( !pEquip)
		return false;
	if(!pEquip->IsWeaponSoul())
	{
		if(!pEquip->IsCanUpLevel() || pEquip->IsLocked())
		return false;
	}
	if (pEquip->GetTypeID()==144000)//湛卢剑
		return false;

	CItem* pStone = GetItem(stoneID,false);
	if (!pStone || pStone->GetTypeID()!=610019 || pStone->IsLocked())//是否是取出石
		return false;
	bool bIsCostly = pStone->IsCostlyItem();
	int nStoneType = pStone->GetTypeID();
	int nStatus = 0;
	if(nPos==4)
		nStatus = pEquip->GetInt(ITEMDATA_GEM4);
	else
		nStatus = pEquip->GetGemStatus(nPos);
	if (nStatus==_HOLE || nStatus==_NOT_HOLE || nStatus==0)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "指定的孔中没有宝石");
		return false;
	}

	int nNeedMoney = COMP_TAKEOUTGEM_COST;
// 	if(nPos == 4)
// 		nNeedMoney = 100000;//10J

	if (GetMoney()<nNeedMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return false;
	}
	if (m_pPackage && m_pPackage->IsPackFull(1, nStatus))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "背包已满");
		return false;
	}
	if (!SpendMoney(nNeedMoney, SYNCHRO_TRUE))
		return false;

	if (!SpendItem(pStone))
		return false;
	if(nPos==4)
		pEquip->SetInt(ITEMDATA_GEM4, _HOLE);
	else
		pEquip->SetGemStatus(nPos, _HOLE);
	CItem* pGem = AwardItem(nStatus, true, false, true, false, 1, false, true, false, 1);//取出的宝石是绑定的
	if (pGem)
	{
		CMsgCompound msg;//发送成功消息
		IF_OK( msg.Create( MSGCOMPOUND_EMBED_TAKEOUTGEM_SUC, idEquip, 0 ) )
			this->SendMsg(&msg);
		SendSysMsg("取出宝石成功");
		int nPos = pEquip->GetPosition();
		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
		if (nStatus%100>2)
		{
			char szMsg[255]		   ;
			char UserName[128] = "";
			char itemName[128] = "";
			char gemName[128]  = "";

			if(pEquip->IsWeaponSoul())//武魂特殊处理
			{
				if(pEquip->GetWpSlType() == 1)
					sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阳之魄", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
				else
					sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阴之魂", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());		
			}
			else//非武魂
			{
				sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
					SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			//			char szItem[128];
//			sprintf(szItem,"%d_%d_%s_%d_%d_%d&%d_%d_%s_%d_%d_%d",
//				idEquip, pEquip->GetTypeID(), ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), this->GetID(), pEquip->GetKidnap(),pEquip->GetColour(),
//				pGem->GetID(), pGem->GetTypeID(), ItemType()->QueryItemType(pGem->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), this->GetID(), pGem->GetKidnap(),pGem->GetColour());
			}
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
			sprintf(gemName,  "%d_[%s]_%d_%d_%d_%d_%d",
					SNAPSHOTOBJ,  ItemType()->QueryItemType(pGem->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pGem->GetID(), pGem->GetTypeID(), this->GetID(), pGem->GetKidnap(), pGem->GetColour());
			sprintf(szMsg, "<%s>将<%s>上的<%s>摘取下时，璀璨的光辉照亮了天空!",
				UserName, itemName, gemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, idEquip&pGem->GetID(), pEquip->GetTypeID()&pGem->GetTypeID(), NULL, ALLUSERS_NAME);
		}
		if (pGem->IsCostlyItem())//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from gem_remove", GetID(), GetName(), nStatus);
		if (bIsCostly)//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from gem_remove", GetID(), GetName(), nStoneType);
		//通知客户端更新物品信息
// 		CMsgItem	msgItem;
// 		msgItem.Create(ITEMACT_SENDINFO,0);
// 		msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 		this->SendMsg(&msgItem);
		if (nStatus%10>=4)
		{
			CMsgPlayer msgPlayer;
			msgPlayer.Create(this);
			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
		return true;
	}
	else
		::LogSave("摘宝石后给宝石失败.userid=%u,itemtype=%u", GetID(), nStatus);
	return false;
}

//////////////////////////////////////////////////////////////////////
void CUser::UpdateEquipmentDurability(void)
{
	for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
	{
		if (i == ITEMPOSITION_RINGT || 
				i == ITEMPOSITION_RINGB ||
					i == ITEMPOSITION_WEAPONR )
			continue;

		this->AddEquipmentDurability(i, -1);
	}
}

//////////////////////////////////////////////////////////////////////
//06.8.30王玉波修改
//当耐久低于最大值的10%时,每一次消耗耐久都会给玩家提示
//当持久为零时,不销毁,但是不加攻,防等等......
void CUser::AddEquipmentDurability(int nPosition, int nAddValue)
{
	return;

	if( nAddValue >= 0 )
		return;

	CItem* refpEquip = GetEquipItemByPos(nPosition);
	if (!refpEquip || refpEquip->IsEquipEnable() )
		return;

	BOOL bIsValidOld = refpEquip->IsValidEquip();//修改前有效性

	int nMaxDur = refpEquip->GetInt( ITEMDATA_AMOUNTLIMIT );
	int nOldDur	= refpEquip->GetInt( ITEMDATA_AMOUNT );
	int nDurability = __max( 0, nOldDur + nAddValue );
	refpEquip->SetInt( ITEMDATA_AMOUNT, nDurability);

	if ( nDurability <= nMaxDur / 10  )
	{
		//20070625修罗.用system频道.
		//[2009.4.2--张冬]马不掉持久
		if (99 != int(refpEquip->GetTypeID()/10000))
		{
			this->SendSysMsg( _TXTATR_SYSTEM,g_objStrRes.GetStr(15077) , refpEquip->GetStr( ITEMDATA_NAME ),nDurability/100 );//"装备%s的耐久还剩%d点,请尽快修理!"	
		}	
	}
	
	//-----彭峰---2007/06/07----
	//if ( nOldDur != nDurability || nDurability/100 == 0 )// should update info to client (nDurability%100) == 0
	if(nDurability/100 != nOldDur/100)//20070629修罗.nDurability/100时才同步
	{
		CMsgItem msg;	// if nDurability equal zero, the client will auto delete this equipment
		if (msg.Create(nPosition, ITEMACT_DURABILITY, nDurability))
			this->SendMsg(&msg);		

		//2007829修罗:外装耐久为0时销毁
		//2007918修罗:屏蔽此功能
//		if(!refpEquip->IsValidEquip() && refpEquip->IsDress())
//		{
//			
//			::MyLogSave("gmlog/wareout_item", "%s(%u) wear out item:[id=%u, type=%u], dur=%d, max_dur=%d", 
//					this->GetName(),
//					this->GetID(),
//					refpEquip->GetID(), 
//					refpEquip->GetInt(ITEMDATA_TYPE),
//					refpEquip->GetInt(ITEMDATA_AMOUNT),
//					refpEquip->GetInt(ITEMDATA_AMOUNTLIMIT));
//
//			EraseEquip(nPosition, SYNCHRO_TRUE);//同步
//
//			UpdateCalculatedProperty();//重置角色卡
//
//		}
//		else
		{
			
			if(refpEquip->IsValidEquip() != bIsValidOld)//20070607修罗:有效性的转变
				UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
			
			refpEquip->SaveInfo();
		}
	}
}

//////////////////////////////////////////////////////////////////////
//20061023修罗.快速拾取时,不管物品距离.以客户端判断为主.
//nDonotCareDistance为快速拾取范围.与客户端定成一致.
//20070206修罗:考虑组队..判断结构不做改变.
bool CUser::PickMapItem(OBJID id,OBJID idItem,int nDonotCareDistance/*=0*/)//idItem 有可能不是物品的真实type，，，，坑爹啊
{
 	int step = 1;
	DEBUG_TRY
	CMapItem* pMapItem = MapManager()->QueryMapItem(id);
	if (!pMapItem)
		return false;
	if (this->isFatigue() && this->isZeroP())
	{
		MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());
		SendSysMsg("您已进入不健康游戏时间，不通过防沉迷验证无法获得物品！");
		return false;
	}
	step = 2;
 	if (this->GetDistance(pMapItem->QueryMapThing()) > nDonotCareDistance)
	{
		this->SendSysMsg("物品距离太远");//STR_FAR_CANNOT_PICK
		return false;
	}

	step = 3;

	CHECKF(pMapItem->GetOwner());
	//if(pMapItem->GetOwner()->GetID() != GetID())
	if (pMapItem->GetOwnerID() != GetID())
	{
		this->SendSysMsg("不应该看到这个错误");
		return false;
	}

	step = 4;
	if(idItem == 999998)
	{
		MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());
		return true;
	}

	step = 5;
	int nLeave = 0;
	for(int i = 0 ; i < pMapItem->m_nItemAmount ; i ++)
	{
		if(pMapItem->m_setBePick[i])
			continue;
		if(idItem == 999999 || idItem == pMapItem->m_setItemType[i])
		{
			bool bPickup = false;
			if(pMapItem->m_setItemType[i] / 100000 == 7)
			{
				step = 6;
				CItemTypeData* pItemType = ItemType()->QueryItemType( pMapItem->m_setItemType[i] );
				if (!pItemType)
				{
					::LogSave("PickMapItem:Invalid EudemonType(%d)", pMapItem->m_setItemType[i]);
					return false;
				}
				EudemonData eudData;
				bPickup = AwardEudemon(pItemType->GetInt(ITEMTYPEDATA_BASE1_INIT),eudData);
			}
			else
			{

				step = 7;
				//特殊兑换物品 打怪获得绑定
				bool bKindnap = false;
				if(pMapItem->m_setItemType[i] >= 610030 && pMapItem->m_setItemType[i] <= 610400)
					bKindnap = true;

				if (m_pPackage->IsPackFull(1,pMapItem->m_setItemType[i],m_pPackage->GetAvailablePosition(pMapItem->m_setItemType[i])))
				{
					this->SendSysMsg("背包已满");
					return false;
				}
			//	bPickup = AwardItemCount(pMapItem->m_setItemType[i],1,true,0,bKindnap,pMapItem->m_nBossType);
				CItemPtr pItem =AwardItem(pMapItem->m_setItemType[i],true,false,true,true,1,true,true,false,bKindnap,pMapItem->m_nBossType,0);
				if(pItem)
				{	
					bPickup=true;
					if ((ItemType()->GetMonoly(pMapItem->m_setItemType[i])  & COSTLY_ITEM) != 0)//goto 贵重物品记录
						::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from monster", GetID(), GetName(), pMapItem->m_setItemType[i]);
					//CItemPtr pItem = GetItemByType(pMapItem->m_setItemType[i]);
					DEBUG_TRY
					IgotAnOrangeWeapen(pMapItem->m_setItemType[i], pItem,pMapItem->m_pDropMonster);
					DEBUG_CATCH3("IgotAnOrangeWeapen(idItem, pItem), type = %u, userid = %u", idItem, GetID());
				}
			}

			if(bPickup)
			{
				pMapItem->m_setBePick[i] = true;
			}
			else
				nLeave ++;
			continue;
		}
		nLeave ++;
	}

	step = 8;
	if(idItem == 999999 || nLeave <= 0)
	{
		MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());
		return true;
	}
	DEBUG_CATCH2("PickMapItem %u",step);


// 	ItemInfoStruct infoBook;
// 	memcpy(&infoBook, pMapItem->GetInfo(), sizeof(infoBook));
// 	
// 	if(this->GetAutoFightFlag() && m_pAutoFightData->IsNonPickUpBook())
// 	{
// 		//拦截挂机时未设置拾取技能书而偶尔捡起技能书
// 		if(infoBook.idType<605000 && infoBook.idType>=601000)
// 		{
// 			pMapItem->SetLife();
// 			return false;
// 		}
// 	}
// 
// 	//20061023修罗------------------------------
// 	int pickMapItemDistLimitTemp=PICKMAPITEMDIST_LIMIT;
// 	if(nDonotCareDistance)
// 		pickMapItemDistLimitTemp=nDonotCareDistance;
// 	//if (this->GetDistance(pMapItem->QueryMapThing()) > PICKMAPITEMDIST_LIMIT)
// 	if (this->GetDistance(pMapItem->QueryMapThing()) > pickMapItemDistLimitTemp)
// 	//-------------------------------------------
// 	{
// 		this->SendSysMsg(g_objStrRes.GetStr(12046));//STR_FAR_CANNOT_PICK
// 		return false;
// 	}
// 
// 	if (!pMapItem->IsMoney() 
// 		&& this->IsItemFull(pMapItem->GetInfo()))
// 	{
// 	
// 		this->SendSysMsg(g_objStrRes.GetStr(12047));//STR_FULL_CANNOT_PICK
// 		return false;
// 	}
// 
// 	OBJID idOwner = pMapItem->GetPlayerID();
// 	if (pMapItem->IsPriv() && idOwner != GetID())//捡东西优先时间未结束，并且不是你打的物品
// 	{
// 		//20070206修罗:小队拾取策略
// 		//如果是小队物品,且你属于此小队---------------
// 		OBJID teamID = pMapItem->GetTeamID();
// 		CTeam *pTeam = this->GetTeam();
// 		if(pTeam //有队
// 			&& pTeam->GetID() == teamID//自己也属于此队
// 			&& ( pMapItem->IsMoney()//钱...怎么都能拾
// 				|| (!pMapItem->IsMoney() && (pTeam->GetItemMode()<<4) & 0x10)//非钱,物品 随机分配 或 物品 自由拾取
// 				) 
// 			)
// 		{
// 			//随机分配算法...在后面.此处仅将错误的情况去掉
// 		}
// 		else
// 		{
// 			SendSysMsg(g_objStrRes.GetStr(12050));//STR_OTHERS_ITEM
// 			return false;
// 		}
// 		//--------------------------------------------
// 		
// 	}
// 	if (pMapItem->IsMoney())
// 	{
// 		//20070206修罗:考虑组队-----------------------------------
// 		OBJID teamID = pMapItem->GetTeamID();
// 		CTeam *pTeam = this->GetTeam();
// 		if(pTeam //有队
// 			&& pTeam->GetID() == teamID//自己也属于此队
// 			)
// 		{
// 			//金钱平均分配算法--------------------
// 			//理应放在更高层去处理,比如UserManager.以下为同级操作.
// 			//一个User操作其他的User
// 			int memberAmount = pTeam->GetMemberAmount();
// 			int nMoney = pMapItem->GetAmount()/memberAmount;//每个成员获得的钱
// 			MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());//先删除，以防作弊
// 
// 			
// 			for(int i=0;i<memberAmount;i++)
// 			{
// 				CUser *pUser =UserManager()->GetUser(pTeam->GetMemberByIndex(i));
// 				if(pUser)//在本地图组
// 				{
// //					if (this->GetDistance(pUser->GetPosX(),pUser->GetPosY())>_RANGE_EXPSHARE)
// //						continue;
// 					if (pUser->GetMoney() + nMoney > _MAX_MONEYLIMIT)
// 					{
// 						pUser->SendSysMsg(g_objStrRes.GetStr(14043));//STR_TOOMUCH_MONEY
// 						continue;//下一个
// 					}
// 					//获得金钱
// 					BOOL bRet = pUser->GainMoney(nMoney, SYNCHRO_TRUE);
// 					//////////////////////////////////////////////////////////////////////////
// 					//	20070710	朱斌	大社会额度金钱记录
// 					if(bRet && CItem::IsLogSaveMoney(nMoney))
// 					{
// 						MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 							this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_PICK, 
// 							0, 0, "时空币", nMoney);
// 					}
// 					//	结束 20070710
// 					//////////////////////////////////////////////////////////////////////////
// 					if(nMoney >= PICK_MORE_MONEY)
// 					{
// 						CMsgAction	msg;
// 						IF_OK(msg.Create(pUser->GetID(), 
// 							pUser->GetPosX(), 
// 							pUser->GetPosY(), 
// 							pUser->GetDir(), 
// 							actionGetMoney, 
// 							nMoney))
// 						{
// 							pUser->SendMsg(&msg);
// 						}
// 					}
// 					pUser->SendSysMsg(g_objStrRes.GetStr(12048), nMoney);//STR_PICK_MONEY
// 				}
// 				else//不在本地图组
// 				{
// 					MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_PICKMONEY, 
// 			    	pTeam->GetID(), 
// 			    	pTeam->GetMemberByIndex(i),//idUser
// 		        	nMoney);//nData  表示金钱
// 				}
// 			}
// 			//金钱平均分配算法end----------------------
// 		}
// 		else//传统金钱拾取模式
// 		{
// 			int nMoney = pMapItem->GetAmount();
// 			
// 			if (this->GetMoney() + nMoney > _MAX_MONEYLIMIT)
// 			{
// 				this->SendSysMsg(g_objStrRes.GetStr(14043));//STR_TOOMUCH_MONEY
// 				return false;
// 			}
// 			MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());		//??? 先删除，以防作弊
// 			BOOL	bRet = this->GainMoney(nMoney, SYNCHRO_TRUE);
// 			//////////////////////////////////////////////////////////////////////////
// 			//	20070710	朱斌	大社会额度金钱记录
// 			if(bRet && CItem::IsLogSaveMoney(nMoney))
// 			{
// 				MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 					this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_PICK, 
// 					0, 0, "时空币", nMoney);
// 			}
// 			//	结束 20070710
// 			//////////////////////////////////////////////////////////////////////////
// 			if(nMoney >= PICK_MORE_MONEY)
// 			{
// 				CMsgAction	msg;
// 				IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionGetMoney, nMoney))
// 					this->SendMsg(&msg);
// 			}
// 			this->SendSysMsg(g_objStrRes.GetStr(12048), nMoney);//STR_PICK_MONEY
// 		}
// 		//-------------------------------------------------------
// 	}
// 	else			//不是钱...
// 	{
// 		//20070207修罗:考虑组队-----------------------------
// 		CUser *pUser=NULL;//算法之后的拥有者
// 
// 		OBJID teamID = pMapItem->GetTeamID();
// 		CTeam *pTeam = this->GetTeam();
// 		if(pTeam //有队
// 			&& pTeam->GetID() == teamID//自己也属于此队
// 			)
// 		{
// 			if((pTeam->GetItemMode()<<4) & 0x40)//随机分配
// 			{
// 				//物品 随机分配算法
// 				//随机分配到小队成员
// 				//如果此成员不在本地图组,则分给this
// 				int rndTemp = ::RandGet(pTeam->GetMemberAmount());
// 				pUser = UserManager()->GetUser(pTeam->GetMemberByIndex(rndTemp));
// 				if(!pUser || GetDistance(pUser->GetPosX(),pUser->GetPosY()>_RANGE_EXPSHARE))//不在本地图组
// 					pUser = this;
// 			}
// 			else if((pTeam->GetItemMode()<<4) & 0x20)//自由拾取
// 			{
// 				//物品 自由拾取算法
// 				//任何人都能拾.自己可以拾
// 				pUser = this;
// 			}
// 			else if((pTeam->GetItemMode()<<4) & 0x10)
// 			{
// 				//物品 各自取得算法
// 				//[各自取得]能走到这里,前面已经做过判断.ln:2634
// 				//没什么特别的
// 				pUser = this;
// 			}
// 			else
// 			{
// 				//error!
// 				LOGERROR( "CUser::PickMapItem函数出错.玩家名:%s GetItemMode:%d",this->GetName(),pTeam->GetItemMode() );
// 				return FALSE;
// 			}
// 
// 		}
// 		else//传统分配模式
// 		{
// 			pUser = this;
// 		}
// 		//保险起见
// 		CHECKF(pUser);
// 		//-------------------------------------------------
// 
// 		
// 
// 		CItem* pItem=NULL;
// 		ItemInfoStruct info;
// 		memcpy(&info, pMapItem->GetInfo(), sizeof(info));
// 		info.idOwner	= pUser->GetID();
// 		info.idPlayer	= pUser->GetID();
// 		info.ucPosition	= pUser->QueryPackage()->GetAvailablePosition();
// 
// 		//	20070712	朱斌	检查是否是独占物品
// 		//	ItemInfoStruct info1;
// 		//	ItemType()->GetInfo(pMapItem->GetInfo()->idType, &info1);
// 		if(IsUserHaveAnExclusiveItem(pMapItem->GetInfo()->idType,
// 			NULL, this))
// 			return false;
// 
// 		DEBUG_TRY{
// 			//[游途道标 2009.04.21]
// 			DWORD dwStrengthanTimes = pMapItem->GetStrengthanTimes();
// 
// 			//[游途道标 2009.05.14]提前
// 			MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());		//??? 先删除，以防作弊
// 			
// 			pItem = pUser->AwardItem(&info, 
// 				SYNCHRO_TRUE,
// 				true,
// 				false,
// 				false,
// 				true,
// 				true);//20070707修罗:不修改创造者
// 			
// 
// 			//[游途道标 2009.05.08]
// 			if (!pItem)
// 			{
// 				char szTemp[256];
// 				sprintf(szTemp, "CUser::PickMapItem 产生物品失败 TypeId = %d name = %s id = %d", info.idType, info.szName, info.id);
// 				::LogSave(szTemp);
// 				return false;
// 			}
// 			else
// 			{
// 				//[游途道标 2009.04.21]
// //				pItem->SetInt(ITEMDATA_STRENGTHAN_TIMES, dwStrengthanTimes, true);
// 			}
// 
// 			//////////////////////////////////////////////////////////////////////////
// 			//	20070709	朱斌	新增贵重物品流向日志 捡拾
// 			if(pItem && pItem->IsCostlyItem())
// 			{
// 				ItemInfoStruct info;
// 				ItemType()->GetInfo(pItem->GetTypeID(), &info);
// 				MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 					this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_PICK, 
// 					pItem->GetTypeID(), pItem->GetID(), info.szName,   
// 					CItem::IsCountable(pItem->GetTypeID()) ? info.nAmount : 1);  
// 			}
// 			//	结束 20070709
// 			//////////////////////////////////////////////////////////////////////////
// 
// 			//  [3/5/2008 朱斌]	写完日志后再检查,解决物品被删除后空指针写日志的问题
// 
// 			//////////////////////////////////////////////////////////////////////////
// 			//  [2007-9-19] 朱斌	马上更新生命周期
// 			CheckItemLifeTime(pItem, TRUE, TRUE);
// 		}DEBUG_CATCH("CUser::PickMapItem")
// 
// 		if(pItem)	//从地图上捡物品
// 		{
// 			///捡到东西后如果有组,就进行全组说话广播
// 			if (pTeam)
// 			{
// 				CMsgItem msg;
// 				UINT nItemCount=1;
// 				if (CItem::IsCountable(pItem->GetTypeID()))
// 					nItemCount=info.nAmount;
// 				if (msg.Create(this->GetID(),ITEMACT_TEAMPICKUP,nItemCount,pItem->GetTypeID(), nItemCount, pItem->GetID()))
// 					pTeam->BroadcastTeamMsg(&msg);
// //				char strTeamMsg[255]="";
// //				sprintf(strTeamMsg,"%s捡到了%s!",pUser->GetName(),pItem->GetStr(ITEMDATA_NAME));
// //				CMsgTalk msg;
// //				if (msg.Create((char*)pUser->GetName(),"ALLUSERS",strTeamMsg,NULL, 0XFF0080C0, _TXTATR_TEAM))
// //					pTeam->BroadcastTeamMsg(&msg,pUser);
// 			}
// 
// 			//如果捡的时空之泪
// 			if( this->GetMapID() == g_ResourceWar.GetWarMapID() )
// 			{
// 				if( pItem->GetInt( ITEMDATA_TYPE ) ==  g_ResourceWar.GetSKZLType() )//表示捡起时空之泪
// 				{
// 				
// 					//本地图广播消息
// 					char szMsg[ 128 ] = { 0 };
// 					char szPhyle[ 32 ] = { 0 };
// 
// 					// 种族修改   08.6.12  桂佳 
// //					DWORD dwProfession = this->GetProfession();
// //					if( dwProfession & PROF_DRAGON )
// //					{
// //						strcpy( szPhyle, g_objStrRes.GetStr(15046));//"龙族"
// //					}
// //					else if( dwProfession & PROF_GENE )
// //					{
// //						strcpy( szPhyle, g_objStrRes.GetStr(15048));//"基因族"
// //					}
// //					else
// //					{
// //						strcpy( szPhyle, g_objStrRes.GetStr(15047));//"天人族"
// //					}
// 					sprintf( szMsg,g_objStrRes.GetStr(15078),szPhyle,this->GetName() );//"时空之泪碎片被%s的%s获得。"
// 					g_ResourceWar.BroadcastLocalMapMsg( szMsg );
// 					//时空之泪碎片被XX族的XX（玩家）获得。"
// 
// 				}
// 			}
// 			if (pItem->IsNonsuchItem())
// 			{
// 				::MyLogSave("gmlog/pick_item", "%s(%u) pick item:[id=%u, type=%u], dur=%d, max_dur=%d", 
// 						this->GetName(),
// 						this->GetID(),
// 						pItem->GetID(), 
// 						pItem->GetInt(ITEMDATA_TYPE),
// 						info.nAmount,
// 						info.nAmountLimit);
// 			}
// 			pUser->SendSysMsg(g_objStrRes.GetStr(12051), pItem->GetStr(ITEMDATA_NAME));//STR_GOT_ITEM
// 		}
// 	}
// 
// 	// perform action of pick
// 	CMsgMapItem msg;
// 	IF_OK (msg.Create(this->GetID(), this->GetPosX(), this->GetPosY(), MSGMAPITEM_PICK))
// 		this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);
// 	

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// squear deal
/////////////////////////////////////////////////////////////////////////////
bool CUser::CreateSquareDeal(CUser* pTarget)
{
	ISquareDeal* pSquareDeal = CSquareDeal::CreateNew();
	CHECKF(pSquareDeal);

	if (pSquareDeal->Create(&m_ifaceSquareDeal, &pTarget->m_ifaceSquareDeal))
		return true;

	pSquareDeal->Release();
	return false;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::XUserSquareDeal::CheckDistance()
{
	if(!m_pSquareDeal)
		return ;

	if(m_pSquareDeal->GetTarget()->GetMapID() == This()->GetMapID() 
			&& m_pSquareDeal->GetTarget()->GetDistance(This()->GetPosX(), This()->GetPosY()) <= CELLS_PER_VIEW)
		return ;

	m_pSquareDeal->Release();		// 关闭交易系统
	return ;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::XUserSquareDeal::Create(ISquareDeal* pSquareDeal)
{
	if(m_pSquareDeal)
		return false;

	m_pSquareDeal = pSquareDeal;

	// msg
	CMsgTrade	msg;
	CHECKF(msg.Create(_TRADE_OPEN, m_pSquareDeal->GetTarget()->GetID()));
	This()->SendMsg(&msg);

	return true;
}

/////////////////////////////////////////////////////////////////////////////
ULONG CUser::XUserSquareDeal::Release()
{
	CHECKF(m_pSquareDeal);

	// msg

	m_pSquareDeal = NULL;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::XUserSquareDeal::AddItem(CItem* pItem, int nAmount, int nAllWeight)
{
	CHECKF(m_pSquareDeal);
	CHECKF (pItem);

//检查对方背包是否有空间,06.9.22王玉波注释以下(此时还未交易）
/*	IUserPackage* pPackage = This()->QueryPackage();
	if (!pPackage)
		return false;
	int nPosition = pPackage->GetAvailablePosition();
	if(!This()->IsBackPackSpare(nAmount, nAllWeight, ID_NONE, nPosition))
		return false;*/
/*
	// msg
	CMsgItemInfo	msg;
	CHECKF(msg.Create(pItem, ITEMINFO_TRADE));
	This()->SendMsg(&msg);
*/
	//[游途道标 2009.05.14]强行改变物品位置导致服务器状态位出错,屏蔽,建议到客户端修改.
	/*const int iPostion = 100;//用于在交易时显示的 位置为[他]
	pItem->SetPosition(iPostion,false);*/
	//[2010-06-03 goto] 发送交易物品的信息
	CMsgTrade msg;
	CHECKF(msg.Create(pItem, _TRADE_ADDITEMOP));
	This()->SendMsg(&msg);
	return true;

	This()->SynchroItem(pItem, ITEMINFO_TRADE);

	return true;
}

bool CUser::XUserSquareDeal::DelItem(CItem* pItem, int nAmount, int nAllWeight)
{
	CHECKF (pItem);

	CMsgTrade msg;
	//msg.Create(_TRADE_BACK_WUOP, pItem->GetID(), pItem->GetInt(ITEMDATA_TYPE));
	msg.Create(pItem, _TRADE_BACK_WUOP);
	This()->SendMsg(&msg);

	return true;
}

bool CUser::XUserSquareDeal::TradeLock()
{
	CMsgTrade msg;
	msg.Create(_TRADE_LOCK, This()->GetID());
	This()->SendMsg(&msg);

	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::XUserSquareDeal::AddMoney(int nAllMoney)
{
	CHECKF(m_pSquareDeal);

	//[游途道标 2009.06.05]下面代码干什么?有点无聊.
	/*if(This()->GetMoney() + nAllMoney > _MAX_MONEYLIMIT)
		return false;*/
	// msg
	CMsgTrade	msg;
	CHECKF(msg.Create(_TRADE_MONEYALL, nAllMoney));
	This()->SendMsg(&msg);

	return true;
}

bool CUser::XUserSquareDeal::AddTradeVAS(int nAllMoney)
{
	CHECKF(m_pSquareDeal);

	if(This()->GetVas() + nAllMoney > _MAX_MONEYLIMIT)
		return false;

	// msg
	CMsgTrade	msg;
	CHECKF(msg.Create(_TRADE_VASOTHER, nAllMoney));
	This()->SendMsg(&msg);

	return true;
}
bool CUser::XUserSquareDeal::AddMonster(CUserPetData* pPetData, int nAmount)
{
	CHECKF(m_pSquareDeal);
	CHECKF (pPetData);

	EudemonData Data;
	m_pSquareDeal->GetTarget()->LoadEudemon(pPetData->GetID(),Data,2);
	CMsgEudemon msg;
	if(msg.Create(PETACTION_TradePet,&Data))//action=11 交易中对方放入了宠物
		This()->SendMsg(&msg);

	return true;
}
bool CUser::XUserSquareDeal::DelMonster(CUserPetData* pPetData, int nAmount)
{
	CHECKF (pPetData);
	
	CMsgTrade msg;
	msg.Create(_TRADE_DELMONSTER_OP, pPetData->GetID());
	This()->SendMsg(&msg);
	
	return true;
}
/////////////////////////////////////////////////////////////////////////////
void CUser::XUserSquareDeal::ClickOK()
{
	CHECK(m_pSquareDeal);

	// msg
	CMsgTrade	msg;
	CHECK(msg.Create(_TRADE_OK, 0));
	This()->SendMsg(&msg);

	return ;
}


// IStorage
/////////////////////////////////////////////////////////////////////////////
bool CUser::SendStorageInfo(OBJID idUser, int nIndex, int nPosition /*= ITEMPOSITION_STORAGE*/)
{
	if(!m_pStorage)
		return false;
// 		m_pStorage	= CPackage::CreateNew();
// 	CHECKF(m_pStorage);

   	//idUser = STORAGE_NPC_ID;		//仓库互通
	//CHECKF(m_pStorage->Create(nPosition, idUser, GetID(), nIndex, Database()));
	return m_pStorage->SendInfo(this, nIndex);
}
/////////////////////////////////////////////////////////////////////////////
bool CUser::CheckIn(OBJID idNpc, CItem* pItem, bool bUpdate /*= true*/, int nPosition/*= ITEMPOSITION_STORAGE*/)
{
	if(!m_pStorage)
		return false;
// 		m_pStorage	= CPackage::CreateNew();
// 	CHECKF(m_pStorage);	
	//idNpc	= STORAGE_NPC_ID;		//仓库互通
// 	int nIndex = 1;
// 	if (idNpc>0 && idNpc<=3*PACKAGE_LIMIT)
// 		nIndex = (idNpc-1)/PACKAGE_LIMIT +1;//算出背包页码
// 	else
// 		nIndex=idNpc%10;//取个位,个位是当前仓库页码
//  	CHECKF(m_pStorage->Create(ITEMPOSITION_STORAGE,GetID(),GetID(),nIndex, Database()));

	//  [3/5/2008 朱斌]	放入仓库时不处理,在调用CHECKIN是地,物品已经从背包袱里弹出,所以检查就算过期,也无法删除
	//	统一大业在物品从背包拿出时处理
	
	//  [2007-10-9] 朱斌	检查生命周期
// 	BOOL bItemDeleted = FALSE;
// 	this->CheckItemLifeTime(pItem, FALSE, FALSE, bItemDeleted);
// 
// 	if(bItemDeleted == TRUE)
// 		return false;

	return m_pStorage->CheckIn(this, pItem, idNpc, bUpdate);
}
bool CUser::CombineItem(CUser* pUser, CItem* pItem1, CItem* pItem2)
{
	if(!m_pStorage)
		return false;
	CHECKF(pItem1);
	CHECKF(pItem2);
	CHECKF(pItem1->GetID() != pItem2->GetID());
// 		m_pStorage	= CPackage::CreateNew();
// 	CHECKF(m_pStorage);	

	return m_pStorage->CombineItem(pUser, pItem1, pItem2);
}
bool CUser::ExChangeIndex(CUser* pUser, CItem* pItem1, CItem* pItem2)
{
	if(!m_pStorage)
		return false;
	CHECKF(pItem1);
	CHECKF(pItem2);
	CHECKF(pItem1->GetID() != pItem2->GetID());
// 		m_pStorage	= CPackage::CreateNew();
// 	CHECKF(m_pStorage);	
	
	return m_pStorage->ExChangeIndex(pUser, pItem1, pItem2);
}
bool CUser::MoveStorageItem(CUser* pUser, CItem* pItem, int nIndex)
{
	if(!m_pStorage)
		return false;
// 		m_pStorage	= CPackage::CreateNew();
// 	CHECKF(m_pStorage);	
	
	return m_pStorage->MoveStorageItem(pUser, pItem, nIndex);
}
bool CUser::DelItem(CUser* pUser, CItem* pItem)
{
	if(!m_pStorage)
		return false;
// 		m_pStorage	= CPackage::CreateNew();
// 	CHECKF(m_pStorage);	
	
	return m_pStorage->DelItem(pUser, pItem);
}
//仓库物品的合并，无需进行仓库容量的检测
bool   CUser::CombineItemInStorage(OBJID idNpc,CItem *pItem,bool bUpdate /*= true*/,int nPosition /*= ITEMPOSITION_STORAGE*/)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
CItem* CUser::CheckOut(OBJID idNpc, OBJID idItem, int nIndex, bool bUpdate, int nPosition/*= ITEMPOSITION_STORAGE*/)
{
	if(!m_pStorage)
		return false;
// 		m_pStorage	= CPackage::CreateNew();
// 	CHECKF(m_pStorage);

//	idNpc   = STORAGE_NPC_ID;//仓库互通
// 	CItem* pItemTarget = this->GetItemByIdItem(idItem);
// 	if (!pItemTarget)
// 		return NULL;
// 	int nPackageIndex = (pItemTarget->GetPackageIndex()-1)/PACKAGE_LIMIT +1;//算出背包页码
 	//CHECKF(m_pStorage->Create(nPosition, idNpc, GetID(), Database()));
	//CHECKF(m_pStorage->Create(ITEMPOSITION_STORAGE,GetID(),GetID(),nPackageIndex, Database()));

//	return m_pStorage->CheckOut(this, idItem, bUpdate);
	CItem* pItem = m_pStorage->CheckOut(this, idItem, nIndex, bUpdate);

	//  [3/5/2008 朱斌]	不作检查,因为,在调用CHECKOUT时,物品还未加入背包,所以检查就算过期,也无法删除
	//	EraseItem是删除背包物品
	//	改在CMsgPackage.Process.中处理

// 	BOOL	bItemDeleted = FALSE;
// 
// 	if(pItem)	//  [2007-10-9] 朱斌	检查生命周期
// 		this->CheckItemLifeTime(pItem, TRUE, FALSE, bItemDeleted);
// 
// 	if(bItemDeleted)
// 		return NULL;

	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::Mine()
{
	CHECKF(GetMap());

	if (!this->IsAlive())
		return false;

	if (m_tMine.IsActive())
		return false;

	if(m_data.GetVit() < 1)
		return false;

	int nLev = GetMap()->GetDocID() % 100;
	m_nMineType = 0;
	if(GetMap()->isCutMap())
	{
		//技能
		IMagicData* pMagic = QueryMagic(6001);
		if(!pMagic || pMagic->GetMagicLevel() < nLev)
			return false;

		m_nMineType = 1;
	}
	else if(GetMap()->isMineMap())
	{
		//技能
		IMagicData* pMagic = QueryMagic(6002);
		if(!pMagic || pMagic->GetMagicLevel() < nLev)
			return false;

		m_nMineType = 2;
	}
	else if(GetMap()->isHuntMap())
	{
		//技能
		IMagicData* pMagic = QueryMagic(6003);
		if(!pMagic || pMagic->GetMagicLevel() < nLev)
			return false;

		m_nMineType = 3;
	}

	if(m_nMineType == 0)
		return false;

	

	CRole::DetachStatus(this->QueryRole(), STATUS_HIDDEN);
	m_tMine.Startup(15000);

	return true;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::StopMine()
{
	if (m_tMine.IsActive())
	{
		m_tMine.Clear();
		m_nMineType = 0;

		CMsgAction msg;
		if (msg.Create(GetID(), 0, 0, 0, actionStopMine))
			this->SendMsg(&msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
void CUser::ProcessMineTimer(void)
{
	CHECK(GetMap());
	
	if (!m_tMine.IsActive())
	{
		return;
	}

	if(m_tMine.ToNextTime())
	{
		if (!this->IsAlive())
		{
			this->StopMine();
			return;
		}

		int nVit = m_data.GetVit();
		if(nVit < 1)
		{
			SendSysMsg("活力不足，停止采集");
			this->StopMine();
			return;
		}

		//获得采集物
		//等级
		int nLev = GetMap()->GetDocID() % 100;
		int nSkill = -1;

		switch(m_nMineType)
		{
		case 1:
			{
				if(!GetMap()->isCutMap())
				{
					this->StopMine();
					return;
				}
				//技能
				IMagicData* pMagic = QueryMagic(6001);
				if(!pMagic || pMagic->GetMagicLevel() < nLev)
				{
					this->StopMine();
					return;
				}
				else
					nSkill = 6001;
			}
			break;
		case 2:
			{
				if(!GetMap()->isMineMap())
				{
					this->StopMine();
					return;
				}

				//技能
				IMagicData* pMagic = QueryMagic(6002);
				if(!pMagic || pMagic->GetMagicLevel() < nLev)
				{
					this->StopMine();
					return;
				}
				else
					nSkill = 6002;
			}
			break;
		case 3:
			{
				if(!GetMap()->isHuntMap())
				{
					this->StopMine();
					return;
				}

				//技能
				IMagicData* pMagic = QueryMagic(6003);
				if(!pMagic || pMagic->GetMagicLevel() < nLev)
				{
					this->StopMine();
					return;
				}
				else
					nSkill = 6003;
			}
			break;
		default:
			this->StopMine();
			return;
		}

		
		if(nSkill == -1)
			return;

		//普通获得
		OBJID idNormal = (660 + m_nMineType) * 1000 + nLev;

		//特殊获得
		OBJID idOther = (660 + m_nMineType) * 1000 + 100 + ((nLev - 1) / 2 + 1);

		if(RandGet(10000) < 8000)
		{
			CItem* pItem = NULL;
			m_pMagic->AwardExp(nSkill);
			pItem = AwardItem(idNormal,true);
			if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from mine", GetID(), GetName(), idNormal);
			if(RandGet(10000) < 100)
			{
				pItem = AwardItem(idOther,true);
				if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from mine", GetID(), GetName(), idOther);
			}
		}
		else
			SendSysMsg("采集失败");

		

		m_data.SetVit(nVit-1);

		CMsgUserAttrib msgAtt;
		if(msgAtt.Create(GetID(),_USERATTRIB_VIT,m_data.GetVit()))
			SendMsg(&msgAtt);

		if(nVit < 1)
		{
			SendSysMsg("活力不足，停止采集");
			this->StopMine();
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::MultiDelItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum)
{
	int nItems	= m_pPackage->GetAmount();
	if (nItems > 0)
	{
		int nCount = 0;
		OBJID* id = new OBJID[nItems];

		nCount = m_pPackage->MultiGetItem(idTypeFirst, idTypeLast, nNum, id, nItems);

		if (nCount >= nNum)
		{
			for (int i=0; i<nCount; i++)
				this->EraseItem(id[i], SYNCHRO_TRUE);

			delete [] id;
			return true;
		}
		else
			delete [] id;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum)
{
	return m_pPackage->MultiCheckItem(idTypeFirst, idTypeLast, nNum);
}


int CUser::GetArmorTypeID()
{
	if(m_pArmor) 
		return m_pArmor->GetInt(ITEMDATA_TYPE);
	int armorID = ID_NONE;

	//种族修改   08.6.12  桂佳
	switch( this->GetProfession() )
	{
	case PROF_MALE:	//龙族弓手	1
		armorID = 131010;
		break;
	}
	return armorID;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.10.28王玉波
//函数说明:处理客户端请求改变同一背包中的物品的位置
//参数说明:idItem 操作的物品id,nWhichPackage在哪一个背包中,nWant2PackageIndex目地index
void CUser::ProcessChgItemPackIndex( OBJID idItem,int nWant2PackageIndex )
{
	CHECK(idItem);
	DEBUG_TRY
	CItem* pItem = this->GetItem( idItem,false );
	if( pItem )
	{
		UCHAR ucPosition		= pItem->GetPosition();
		UCHAR ucOldPackageIndex = pItem->GetPackageIndex();

		CUserPackage* pUserPack = ( CUserPackage* )QueryPackage();
		int nHasFlag = pUserPack->GetIndexState( ucPosition,nWant2PackageIndex );

		if( nHasFlag != 0 )
		{
			SendUnlockItemMsg(idItem);
			return;
		}

		pUserPack->SetIndexState( ucPosition,ucOldPackageIndex,0 );//设置原来位置为空
		pUserPack->SetIndexState( ucPosition,nWant2PackageIndex,1 );//设置新位置为真

		pItem->SetPackageIndex( nWant2PackageIndex );

		CMsgItem	msg;
		if (msg.Create(ITEMACT_ADD,0)
			&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour()))
			SendMsg(&msg);

		SendUnlockItemMsg(idItem);
	}
	DEBUG_CATCH( "CUser::ProcessChgItemPackIndex" )	

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.10.30王玉波
//函数说明:处理客户端请求,改变一个物品在不同的背包中的位置(实际为改变Position and Package_index )
//参数说明:idItem 物品ID,nWhichPackage从哪一个背包中取出(0为第一个背包)
//nWant2Position请求放入背包的Position,nWant2PackageIndex请求放入背包的位置
void CUser::ProcessChgItemPosAndIdx( OBJID idItem,int nWhichPackage,int nWant2Position,int nWant2PackageIndex )
{
	DEBUG_TRY
	CUserPackage* pPackage = ( CUserPackage* )m_pPackage;
	int nHasFlag =0;
// 	if (nWant2Position==ITEMPOSITION_MAGICSTONEBOX)
// 	{
// 		int nItemCount=pPackage->QueryItemSet(ITEMPOSITION_MAGICSTONEBOX-ITEMPOSITION_BACKPACK1)->GetAmount();
// 		nWant2PackageIndex=nItemCount+1;
// 	}
// 	else
	nHasFlag=pPackage->GetIndexState( nWant2Position,nWant2PackageIndex );
	if( nHasFlag != 0 )//目的地已有物品( > 0 or < 0 both error)
	{
//		int nPackageIndex = pPackage->GetFreeIndex( nWant2Position );
//		if( nPackageIndex <1)
//		{
			CMsgItem exceptionMsg;
			IF_OK( exceptionMsg.Create( idItem,ITEMACT_SYNCHRO_UNLOCK ) )
				SendMsg( &exceptionMsg );
			return;
//		}
//		else
//		{
//			nWant2PackageIndex=nPackageIndex;
//		}
	}
	CItem* pItem  = m_pPackage->PopItem( idItem );
	if( !pItem )//操作失败
	{
		CMsgItem exceptionMsg;
		IF_OK( exceptionMsg.Create( idItem,ITEMACT_SYNCHRO_UNLOCK ) )
			SendMsg( &exceptionMsg );
		return;
	}
	pItem->SetPosition( nWant2Position );
	pItem->SetPackageIndex( nWant2PackageIndex);
	if( !AddItem( pItem,false,false ) )
	{
		CMsgItem exceptionMsg;
		IF_OK( exceptionMsg.Create( idItem,ITEMACT_SYNCHRO_UNLOCK ) )
			SendMsg( &exceptionMsg );
	}
	else
	{
		//成功
		CMsgItem msg;
		IF_OK( msg.Create( idItem,ITEMACT_EXCHANGEPOSOK,pItem->GetPosition(),pItem->GetPackageIndex()/*nWant2Position,nWant2PackageIndex*/ ) )
			SendMsg( &msg );
	}
	DEBUG_CATCH( "CUser::ProcessChgItemPosAndIdx Catch an Error!" )
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//09.3.26
//张冬
//函数说明:处理客户端请求,交换物品在不同的背包中的位置(实际为改变Position and Package_index )
//参数说明:idItem1 物品ID,idItem2 物品ID,nWhichPackage从哪一个背包中取出(0为第一个背包)
//nWant2Position请求放入背包的Position,nWant2PackageIndex请求放入背包的位置
void CUser::ProcessTradeItemPosAndIdx( OBJID idItem1,int nWhichPackage,int nWant2Position,int nWant2PackageIndex , OBJID idItem2)
{
	DEBUG_TRY
	CUserPackage* pPackage = ( CUserPackage* )m_pPackage;
	int nHasFlag =0;
// 	if (nWant2Position==ITEMPOSITION_MAGICSTONEBOX)
// 	{
// 		int nItemCount=pPackage->QueryItemSet(ITEMPOSITION_MAGICSTONEBOX-ITEMPOSITION_BACKPACK1)->GetAmount();
// 		nWant2PackageIndex=nItemCount+1;
// 	}
// 	else
	nHasFlag=pPackage->GetIndexState( nWant2Position,nWant2PackageIndex );
	
	CItem* pItem1  = m_pPackage->PopItem( idItem1 );
	CItem* pItem2 = m_pPackage->PopItem(idItem2 );
	if( !pItem1 || !pItem2 )//操作失败
	{
		CMsgItem exceptionMsg;
		IF_OK( exceptionMsg.Create( idItem1,ITEMACT_SYNCHRO_UNLOCK ) )
			SendMsg( &exceptionMsg );
		CMsgItem exceptionMsg1;
		IF_OK( exceptionMsg1.Create( idItem2,ITEMACT_SYNCHRO_UNLOCK ) )
			SendMsg( &exceptionMsg1 );
		return;
	}
	UCHAR ucIndex1 = pItem1->GetPackageIndex();
	UCHAR ucIndex2 = pItem2->GetPackageIndex();
	short position1 = nWant2Position;
	short position2 = nWhichPackage+ITEMPOSITION_BACKPACK1;
	

	pItem1->SetPosition( position2);
	pItem1->SetPackageIndex( ucIndex2);
	pItem2->SetPosition( position1);
	pItem2->SetPackageIndex( ucIndex1);

	if( !AddItem( pItem1,false,false ) || !AddItem( pItem2,false,false ) )
	{
		CMsgItem exceptionMsg;
		IF_OK( exceptionMsg.Create( idItem1,ITEMACT_SYNCHRO_UNLOCK ) )
			SendMsg( &exceptionMsg );
		CMsgItem exceptionMsg1;
		IF_OK( exceptionMsg1.Create( idItem2,ITEMACT_SYNCHRO_UNLOCK ) )
			SendMsg( &exceptionMsg1 );
	}
	else
	{
		//成功
		CMsgItem msg;
		IF_OK( msg.Create( idItem1,ITEMACT_EXCHANGEPOSOK,position2,ucIndex2 ) )
			SendMsg( &msg );
		
		IF_OK( msg.Create( idItem2,ITEMACT_EXCHANGEPOSOK,position1,ucIndex1 ) )
			SendMsg( &msg );
	}
	DEBUG_CATCH( "CUser::ProcessTradeItemPosAndIdx Catch an Error!" )
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.10.31
//王玉波
//函数说明:处理客户端请求,交换两个背包中的物品位置(暂只支持同一个背包中互相操作)
//参数说明:两个物品标识
void CUser::ProcessTradeItemPackageIndex( OBJID idItem1,OBJID idItem2 )
{
	if( idItem1 == idItem2 )
	{
		SendUnlockItemMsg(idItem1);
		return;
	}

	CItem* pItem1 = GetItem( idItem1,false );
	CItem* pItem2 = GetItem( idItem2,false );
	if( !pItem1 || !pItem2  || (pItem1->GetPosition() != pItem2->GetPosition()))
	{
		SendUnlockItemMsg(idItem1);
		SendUnlockItemMsg(idItem2);
		return;
	}

	UCHAR ucIndex1 = pItem1->GetPackageIndex();
	UCHAR ucIndex2 = pItem2->GetPackageIndex();
	//交换PACKAGE_INDEX
	pItem1->SetPackageIndex( ucIndex2 );
	pItem2->SetPackageIndex( ucIndex1 );

	//同步客户端
	CMsgItem	msg;
	if (msg.Create(ITEMACT_ADD,0)
		&& msg.Add(pItem1->GetID(),pItem1->GetTypeID(),pItem1->GetItemAmount(),pItem1->GetInt(ITEMDATA_AMOUNTLIMIT), pItem1->GetPosition(),pItem1->GetKidnap(),pItem1->GetPackageIndex(),pItem1->GetInt(ITEMDATA_BUYPRICE),pItem1->GetColour())
		&& msg.Add(pItem2->GetID(),pItem2->GetTypeID(),pItem2->GetItemAmount(),pItem2->GetInt(ITEMDATA_AMOUNTLIMIT), pItem2->GetPosition(),pItem2->GetKidnap(),pItem2->GetPackageIndex(),pItem2->GetInt(ITEMDATA_BUYPRICE),pItem2->GetColour()))
		SendMsg(&msg);

	SendUnlockItemMsg(idItem1);
	SendUnlockItemMsg(idItem2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CUser::EquipCreateNew(OBJID idPeifang, OBJID idAdditem) 
{
	//检查学会配方否
	CUserPeifangData* pPeifang = m_pUserPeifang->QueryData(idPeifang);
	int bKindnap = 0;	
	if(!pPeifang)
	{
		SendSysMsg("你还没有学会对应的配方");
		return - 1;
	}


	//检查配方
	CPeifangTypeData*	pPeifangType = PeifangType()->GetObj(idPeifang);
	if(!pPeifangType)
	{
		SendSysMsg("配方不存在");
		return - 1;
	}

	//检查ene
	int nSpendVit = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDVIT);
	if(GetVit() < nSpendVit)
	{
		SendSysMsg("活力不足");
		return - 1;
	}

	//检查材料
	OBJID idSpend1 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM1);
	OBJID idSpend2 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM2);
	OBJID idSpend3 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM3);
	OBJID idSpend4 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM4);

	//蓝色以及以上的,需要判断安全锁
	DEBUG_TRY
	if (IsItemLockedOpen())
	{
		if(idSpend1)
		{
			CItem* pItemTemp1 = GetItemByType(idSpend1);
			if(pItemTemp1 == NULL)
			{
				::LogSave("收到一个不存在的合成材料, UserName = %s, type = %u", GetName(), idSpend1);
				return -1;
			}
			if ( pItemTemp1->GetColour() >2 )
			{
				SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return -1;
			}
		}
		if(idSpend2)
		{
			CItem* pItemTemp2 = GetItemByType(idSpend2);
			if(pItemTemp2 == NULL)
			{
				::LogSave("收到一个不存在的合成材料, UserName = %s, type = %u", GetName(), idSpend2);
				return -1;
			}
			if ( pItemTemp2->GetColour() >2 )
			{
				SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return -1;
			}
		}
		if(idSpend3)
		{
			CItem* pItemTemp3 = GetItemByType(idSpend3);
			if(pItemTemp3 == NULL)
			{
				::LogSave("收到一个不存在的合成材料, UserName = %s, type = %u", GetName(), idSpend3);
				return -1;
			}
			if ( pItemTemp3->GetColour() >2 )
			{
				SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return -1;
			}
		}
		if(idSpend4)
		{
			CItem* pItemTemp4 = GetItemByType(idSpend4);
			if(pItemTemp4 == NULL)
			{
				::LogSave("收到一个不存在的合成材料, UserName = %s, type = %u", GetName(), idSpend4);
				return -1;
			}
			if ( pItemTemp4->GetColour() >2 )
			{
				SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return -1;
			}
		}
	}
	DEBUG_CATCH("Judge IsItemLockedOpen in EquipCreateNew ");
	
	int  spendNum1 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDNUM1);
	int  spendNum2 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDNUM2);
	int  spendNum3 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDNUM3);
	int  spendNum4 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDNUM4);
	
	OBJID idGet1 = pPeifangType->GetInt(PEIFANGTYPEDATA_GETITEM1);
	OBJID idGet2 = pPeifangType->GetInt(PEIFANGTYPEDATA_GETITEM1);
	
	int  getNum1 = pPeifangType->GetInt(PEIFANGTYPEDATA_GETNUM1);
	int  getNum2 = pPeifangType->GetInt(PEIFANGTYPEDATA_GETNUM2);

	if((idSpend1 != ID_NONE) && (spendNum1 > 0))
	{
		if(GetItemNumByType(idSpend1) < spendNum1)
		{
			SendSysMsg("材料不足");
			return - 1;
		}
	}

	if((idSpend2 != ID_NONE) && (spendNum2 > 0))
	{
		if(GetItemNumByType(idSpend2) < spendNum2)
		{
			SendSysMsg("材料不足");
			return - 1;
		}
	}

	if((idSpend3 != ID_NONE) && (spendNum3 > 0))
	{
		if(GetItemNumByType(idSpend3) < spendNum3)
		{
			SendSysMsg("材料不足");
			return - 1;
		}
	}

	if((idSpend4 != ID_NONE) && (spendNum4 > 0))
	{
		if(GetItemNumByType(idSpend4) < spendNum4)
		{
			SendSysMsg("材料不足");
			return - 1;
		}
	}

	//附加材料&&技能
	int nAddType = 0;
	int nSkillType = 0;
	int nSkillLev = pPeifangType->GetInt(PEIFANGTYPEDATA_CARFTLEV);
	int nSort = idPeifang / 1000;
	switch(nSort)
	{
	case PEIFANG_SORT_JINGONG1:
		{
			nSkillType = 6006;
		}
		break;
	case PEIFANG_SORT_JINGONG2:
		{
			nSkillType = 6006;
			if(idAdditem != ID_NONE)
			{
				if(idAdditem / 100 == 6622)
					nAddType = idAdditem % 100;
				else
					idAdditem = ID_NONE;
			}
		}
		break;
	case PEIFANG_SORT_DUANZAO1:
		{
			nSkillType = 6004;
		}
		break;
	case PEIFANG_SORT_DUANZAO2:
		{
			nSkillType = 6004;
			if(idAdditem != ID_NONE)
			{
				if(idAdditem / 100 == 6612)
					nAddType = idAdditem % 100;
				else
					idAdditem = ID_NONE;
			}
		}
		break;
	case PEIFANG_SORT_ZHIPI1:
		{
			nSkillType = 6005;
		}
		break;
	case PEIFANG_SORT_ZHIPI2:
		{
			nSkillType = 6005;
			if(idAdditem != ID_NONE)
			{
				if(idAdditem / 100 == 6632)
					nAddType = idAdditem % 100;
				else
					idAdditem = ID_NONE;
			}
		}
		break;
	}
	

	if((idAdditem != ID_NONE) && (GetItemNumByType(idAdditem) < 1))
	{
		SendSysMsg("附加材料不足");
		return - 1;
	}

	//给物品之前先判断一下该不该绑定
	for (int i=1;i<5;i++)
	{
		if(bKindnap)
			break;
		bKindnap = IsPEIFANGNeedKidnap(i, pPeifangType);
		if(bKindnap == -1)
		{
			return -1;
		}
	}
	//给物品
	if(AwardItemByCreate(idGet1,true,true,true,getNum1,bKindnap,nAddType))
	{
		bool bHasKind = false;
		//删除物品
		if(idSpend1 > 0)
			DelItemByTypeMulti(idSpend1,spendNum1,bHasKind,false);
		if(idSpend2 > 0)
			DelItemByTypeMulti(idSpend2,spendNum2,bHasKind,false);
		if(idSpend3 > 0)
			DelItemByTypeMulti(idSpend3,spendNum3,bHasKind,false);
		if(idSpend4 > 0)
			DelItemByTypeMulti(idSpend4,spendNum4,bHasKind,false);
		if(idAdditem > 0)
			DelItemByTypeMulti(idAdditem,1,bHasKind,false);

		//扣活力
		if(nSpendVit > 0)
			AddVit(-1 * nSpendVit);

		//技能熟练度
		IMagicData* pMagic = QueryMagic(nSkillType);
		if(pMagic && pMagic->GetMagicLevel() <= nSkillLev)
		{
			m_pMagic->AwardExp(nSkillType);
		}

		if (idAdditem && (ItemType()->GetMonoly(idAdditem)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from craft", GetID(), GetName(), idAdditem);
		if (idGet1 && (ItemType()->GetMonoly(idGet1)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from craft", GetID(), GetName(), idGet1);

		return 1;
	}
	return -1;
}
// 
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BOOL CUser::WeaponKillEgg(  OBJID equipID, OBJID stoneID )// 激活武魂 pengfeng [11/6/2007]
// {
// 	DEBUG_TRY
// 	CItem* pWeapon = GetItem( equipID );
// 	if( !pWeapon )
// 		return false;
// 	CItem* pStoneItem = GetItem( stoneID );
// 	if( !pStoneItem )
// 		return false;
// 	if( pStoneItem->GetTypeID() != _SPCITEM_ACTIVATIONSTONE  )
// 		return false;
// 	
// 	pWeapon->WeaponActivation();
// 
// 	ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );//-------彭峰---2007/04/10-删除石头！----
// 
// 	//20071214修罗:同步修改-----------
// //	CMsgItemInfo msg;
// //	IF_OK (msg.Create(pWeapon, ITEMINFO_UPDATE))
// //	this->SendMsg(&msg);
// 	this->SynchroItem(pWeapon,ITEMINFO_UPDATE);
// 	//--------------------------------
// 
// 	DEBUG_CATCH( "CUser::WeaponKillEgg( OBJID equipID,OBJID stoneID )" );
// 	return true;
// }


BOOL CUser::IsPickUpKidnap( int nItemType )//-------彭峰---2007/05/09-----是否拾取绑定----------
{
	if( 6001 == nItemType / 100)
		return true;
	else if(530 == nItemType / 1000)
		return true;
	else
		return false;
}

//BOOL CUser::EquipMakeHole( OBJID equipID, OBJID stoneID,OBJID* pDataSet,int nSize )// 装备打洞 [6/20/2007]
BOOL CUser::EquipMakeHole( OBJID equipID, OBJID stoneID )// 装备打洞 [2010-07-20 goto]
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return FALSE;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return FALSE;
	}
	DEBUG_TRY
	CItem* pEquipItem = GetItem( equipID ,false);
	if(!pEquipItem)
		pEquipItem = GetEquipItem(equipID);

	if(!pEquipItem)
		return false;
	if(!pEquipItem->IsWeaponSoul())
	{
		if(!pEquipItem->IsCanUpLevel() || pEquipItem->IsLocked())
			return false;
	}

	if (pEquipItem->GetTypeID()==144000)//湛卢剑
		return false;
	CItem* pStone = GetItem(stoneID,false);
	if (!pStone || !pStone->IsMakeHoleStone() || pStone->IsLocked())
		return false;
	bool bIsCostly = pStone->IsCostlyItem();
	int nStoneType = pStone->GetTypeID();
	int nHoleCount = pEquipItem->GetEmbedCountLimit();
	if (nHoleCount>=3)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "此装备上已没有可打孔的位置");
		return false;
	}

	bool bNoHole = false;
	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
	{
		if (pEquipItem->GetGemStatus(i)==_NOT_HOLE)
		{
			bNoHole = true;
			break;
		}
	}
	if (!bNoHole)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "此装备上已没有可打孔的位置");
		return false;
	}

	int nCostMoney = COMP_MAKEHOLE_COST * (nHoleCount+1);
	if (GetMoney()<nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return false;
	}

	if (pEquipItem->GetReqLev() > (pStone->GetTypeID()%100+1)*10)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "打孔针级别不符");
		return false;
	}
// 	if (pEquipItem->GetInt(ITEMDATA_COLOR)!=(pStone->GetTypeID()%10+1))
// 	{
// 		SendSysMsg(_TXTATR_TOPSHOW, "打孔石的颜色与装备的颜色不符");
// 		return false;
// 	}
	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//扣除银两
		return false;

	//计算成功率
	int nSucceedProbability = 0;
	if (nHoleCount==0) nSucceedProbability=100;//第一个孔100%
	if (nHoleCount==1) nSucceedProbability=50;//第二个孔50%
	if (nHoleCount==2) nSucceedProbability=25;//第三个个孔25%

	bool bBind = false;
	if (pStone->GetKidnap())
		bBind = true;
	//ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );//删除打孔石
	if (!SpendItem(pStone))
		return false;
	if (::RandGet(100)<nSucceedProbability)//成功
	{
		if (pEquipItem->CreateHole())
		{
			if (bBind && !pEquipItem->GetKidnap())
				pEquipItem->SetKidnap(1);
			//通知客户端更新物品信息
// 			CMsgItem	msgItem;
// 			msgItem.Create(ITEMACT_SENDINFO,0);
// 			msgItem.Add(equipID,0,0,0, 0,0,0);
// 			this->SendMsg(&msgItem);
			SendSysMsg("恭喜你，打孔成功");
			return nHoleCount+1;
		}
	}
	else
		SendSysMsg("很遗憾，打孔失败");

	if (bIsCostly)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, nStoneType);
	//if( 0 == pEquipItem->GetTypeID()%10 )//不可以是白装
	//	return false;
	//[游途道标 2009.04.08]屏蔽
	/*
	CItem* pStoneItem = GetItem( stoneID );
	if( !pStoneItem )
		return false;
	if( !pStoneItem->IsMakeHoleStone() )
		return false;
	int lev = pEquipItem->GetReqLev();
	int nStoneTypeID = pStoneItem->GetTypeID();

	//  [1/23/2008 朱斌]	幸运宝石
	CItemPtr pLuckyStone = NULL;
	BOOL	bHasLuckyGem = FALSE;
	for(int k = 0; k < nSize; k++)
	{
		pLuckyStone = GetItem(pDataSet[k]);
		if(pLuckyStone != NULL)
		{
			if(pLuckyStone->GetTypeID() == _SPCITEM_LUCKY_GEM)
			{
				bHasLuckyGem = TRUE;
				break;
			}
		}
	}

	CItem* pOmnipotence = NULL;
	for( int i = 0;i < nSize;i++ )
	{
		pOmnipotence = GetItem( pDataSet[ i ] );
	}
	int nSucAdd = 5;
	int  nOmnID = 0;
	int  nTmpID = 0;
	if( pOmnipotence )
	{
		nOmnID = pOmnipotence->GetTypeID();// 判断第四星位的物品是否为万能石 [陈剑飞11/6/2007]
		nTmpID = pOmnipotence->GetID();
	}
	*/

	// 出版本之前应策划强烈要求更改！不判断品质。彭峰 [6/27/2007]
	/*
	BOOL bTemp = false;
	if(   ( lev >= 1  && lev <= 20  && nStoneTypeID%10 == 1 )
		||( lev >= 21 && lev <= 40  && nStoneTypeID%10 == 2 )
		||( lev >= 41 && lev <= 60  && nStoneTypeID%10 == 3 )
		||( lev >= 61 && lev <= 80  && nStoneTypeID%10 == 4 ) )
	{
		bTemp = true;
	}
	if( bTemp )
	{*/
//		int nEmbedCountLimit = pEquipItem->GetEmbedCountLimit();

		//[游途道标 2009.04.08]屏蔽
		//int nSucceedProbability = 0;

		//----初级灵魂石头的对应几率--------------
		/*检测当前装备、武器已有的孔，
		0个（50％成功率），
		1个（35％成功率），
		2个（25％成功率），
		3个（10％成功率），
		4个（不能打孔）。*/

		//[游途道标 2009.04.08]屏蔽
		//const int  nSmallSucceedProbabilityArray[5] = { 50, 30, 10, 2, 0 };

		//----高级灵魂石头的对应几率--------------
		/*检测当前装备、武器已有的孔，
		0个（100％成功率），
		1个（70％成功率），
		2个（50％成功率），
		3个（25％成功率），
		4个（不能打孔）。*/

		//[游途道标 2009.04.08]屏蔽
		/*
		const int  nBigSucceedProbabilityArray[5] = { 100, 60, 20, 5, 0 };
		
		if( pStoneItem->IsMakeHoleStoneSmall() )
		{
			if(pOmnipotence)
			{
				if( nOmnID == _SPCITEM_OMNIPOTENCE )
					nSucceedProbability = nSmallSucceedProbabilityArray[nEmbedCountLimit] + nSucAdd;
			}
			else
				nSucceedProbability = nSmallSucceedProbabilityArray[nEmbedCountLimit];
		}
		else if( pStoneItem->IsMakeHoleStoneBig() )
		{
			if( pOmnipotence )
			{
				if( nOmnID == _SPCITEM_OMNIPOTENCE )
					nSucceedProbability = nBigSucceedProbabilityArray[nEmbedCountLimit] + nSucAdd;
			}
			else
				nSucceedProbability = nBigSucceedProbabilityArray[nEmbedCountLimit];
		}
		*/
		
	//	if( RandGet( 100 ) < nSucceedProbability && nEmbedCountLimit < 4 )//成功了

		//[游途道标 2009.04.08]修改
		//bHasLuckyGem = true; //100%成功

// 		const int _maxSize=120;
// 		
// 		int sourceItemAmount = 0;
// 		int sourceItemAmount2 = 0;
// 		int sourceItemSize=_maxSize;
// 		int sourceItemSize2=_maxSize;
// 		OBJID sourceItemID[_maxSize];
// 		OBJID sourceItemID2[_maxSize];
// 
// 		sourceItemAmount = this->GetItemByTypeMulti(900401, sourceItemSize, sourceItemID);
// 		sourceItemAmount2 = this->GetItemByTypeMulti(900402, sourceItemSize2, sourceItemID2);

		/*int iType = pEquipItem->GetTypeID() % 10;
		iType = iType ? (iType + 1) : 2;*/
// 		int iNum = pEquipItem->GetItemPhase() /*double(iType) * pow(double(pEquipItem->GetItemPhase()), (nEmbedCountLimit + 1) * 0.3) + 0.5*/;
// 		if (nEmbedCountLimit > 2 || pEquipItem->GetItemPhase() > 3)
// 		{
// 			if (iNum > sourceItemAmount)
// 			{
// 				return false;
// 			}
// 			if (iNum)
// 			{
// 				DelItemByTypeMulti(sourceItemID, sourceItemSize, iNum);
// 			}
// 		}
// 		else
// 		{
// 			if (iNum > sourceItemAmount + sourceItemAmount2)
// 			{
// 				return false;
// 			}
// 			if (sourceItemAmount2 >= iNum)
// 			{
// 				if (iNum)
// 				{
// 					DelItemByTypeMulti(sourceItemID2, sourceItemSize2, iNum);
// 				}
// 			}
// 			else
// 			{
// 				if (sourceItemAmount2)
// 				{
// 					DelItemByTypeMulti(sourceItemID2, sourceItemSize2, sourceItemAmount2);
// 				}
// 				if (iNum - sourceItemAmount2)
// 				{
// 					DelItemByTypeMulti(sourceItemID, sourceItemSize, iNum - sourceItemAmount2);
// 				}
// 			}
// 		}
// 
// 		if(/*bHasLuckyGem || (RandGet( 100 ) < nSucceedProbability &&*/ nEmbedCountLimit < 4 )/*)*/
// 		{
// 			if( !pEquipItem->CreateHole() )
// 			{
// 				//删除宝石
// 				//ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );
// 				//[游途道标 2009.04.08]屏蔽
// 				//ASSERT(DelItemByTypeMulti(stoneID, 1, true));
// 				return false;
// 			}
			//[游途道标 2009.04.08]屏蔽
			/*
			DEBUG_TRY		// VVVVVVVVVV
				//删除宝石
				//ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );
				ASSERT(DelItemByTypeMulti(stoneID, 1, true));
			DEBUG_CATCH("stoneID")// AAAAAAAAAAA
			*/

			//[游途道标 2009.04.08]屏蔽
			/*if( pOmnipotence )//  [11/24/2007 dio]
			{
				ASSERT( EraseItem( nTmpID, SYNCHRO_TRUE ) );
			}*/
		
			// update item info at client
			//20071214修罗:同步修改-----------
//			CMsgItemInfo msg;
//			IF_OK (msg.Create(pEquipItem, ITEMINFO_UPDATE))
//				this->SendMsg(&msg);			
// 			this->SynchroItem(pEquipItem,ITEMINFO_UPDATE);
// 			//--------------------------------
// 			return true;
// 		}
// 		else
//		{
			//删除宝石
			//ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );
			//[游途道标 2009.04.08]屏蔽
			//ASSERT(DelItemByTypeMulti(stoneID, 1, true));
			//[游途道标 2009.04.08]屏蔽
			/*if( pOmnipotence )
			{
				ASSERT( EraseItem( nTmpID, SYNCHRO_TRUE ) );
			}*/
		
//			return false;
//		}
//	}
	DEBUG_CATCH( "CUser::EquipMakeHole( OBJID equipID,OBJID stoneID )" );
	return false;
}


//--------------------------------------------------
//20070226修罗:背包内装备耐久损失一定百分比.返回实际损失耐久装备的个数
int	CUser::PackageDurationLose(int durationLosePercent)
{
	return m_pPackage->PackageDurationLose(durationLosePercent);
}


//-----彭峰 2007/03/02--- 武器升级
//[2010-07-19 goto]修改此函数为装备强化
//goto 新增天运宝符 610048 ,bUseLuckyCharm表示是否使用
void CUser::WeaponLevUp( OBJID idWeapon,short nGemCount,bool bUseLuckyCharm )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return;
	}
	//获得装备
	CItem* pEquip = GetItem(idWeapon,false);
	if(!pEquip)
		pEquip = GetEquipItem(idWeapon);

	if (!pEquip || !pEquip->IsCanUpLevel())
		return;
	if (pEquip->GetTypeID()==144000)//湛卢剑
		return;
	if (pEquip->IsWeaponSoul())
		return;
	if (pEquip->IsLocked())
		return;

	int nOldLev = pEquip->GetEquipAddLev();
	if (nOldLev<0 || nOldLev >9)
	{
		if (nOldLev==COMP_MAX_EQUIPLEV)
			SendSysMsg(_TXTATR_TOPSHOW, "此装备已经是最高强化等级，无法再强化");
		return;
	}
	int nCostMoney = pEquip->GetEquipAddLevCost(1);
	//判断银两
	if (GetMoney()<nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return;
	}

	int nNeedGemLev = pEquip->GetNeedGemLev(nOldLev);//需要的符等级
	//强化需要的强化符type
	OBJID idGemtype = 0;
	if (nNeedGemLev==1)
		idGemtype = 610020;
	else if (nNeedGemLev==2)
		idGemtype = 610021;
	else if (nNeedGemLev==3)
		idGemtype = 610022;
// 	else if (nNeedGemLev==4)
// 		idGemtype = 610023;
	if (!idGemtype)
		return;
	
	int nHasGemSize = 37;
	OBJID idGemArr[37] = {0};
	int nCoseGem = nGemCount;//需要扣除的强化符个数
	int nHasGemCount = GetItemByTypeMulti(idGemtype, nHasGemSize, idGemArr, true);
	if (!nHasGemCount)//背包中没有强化符
	{
		//if (!bAutoBuy)//暂时不做自动购买功能
		{
			SendSysMsg(_TXTATR_TOPSHOW, "包裹中没有相应的强化符");
			return;
		}
// 		else
// 		{
// 			//扣元宝买强化符强化
// 			nCoseGem = 0;//全部用元宝，不扣符
// 			vasItem* pVasItem = VASItem()->GetItemByType(idGemtype);
// 			if (!pVasItem)
// 				return;
// 			int nPriceTotal = 0;//物品价格
// 			if (pVasItem->discount==0)//没有折扣
// 				nPriceTotal = (pVasItem->price*nGemCount);
// 			else//有折扣
// 				nPriceTotal = (pVasItem->price*nGemCount)*(pVasItem->discount/100.0);
// 
// 			if (GetMoneyByType(5)<nPriceTotal)
// 			{
// 				SendSysMsg(_TXTATR_TOPSHOW, "元宝不足");
// 				return;
// 			}
// 
// 			SpendVas(nPriceTotal);
// 		}
	}
	if (nHasGemCount)//背包中有强化符
	{
		if (nHasGemCount<nGemCount)
		{
			//if (!bAutoBuy)//暂时没有自动买符功能
			{
				SendSysMsg(_TXTATR_TOPSHOW, "包裹中强化符数量不足");
				return;
			}
// 			else//自动购买强化符
// 			{
// 				nCoseGem = nHasGemCount;//部分用元宝，部分用符
// 				vasItem* pVasItem = VASItem()->GetItemByType(idGemtype);
// 				if (!pVasItem)
// 					return;
// 				int nPriceTotal = 0;//物品价格
// 				if (pVasItem->discount==0)//没有折扣
// 					nPriceTotal = (pVasItem->price*(nGemCount-nHasGemCount));
// 				else//有折扣
// 					nPriceTotal = (pVasItem->price*(nGemCount-nHasGemCount))*(pVasItem->discount/100.0);
// 				
// 				if (GetMoneyByType(5)<nPriceTotal)
// 				{
// 					SendSysMsg(_TXTATR_TOPSHOW, "元宝不足");
// 					return;
// 				}
// 				
// 				SpendVas(nPriceTotal);
// 			}
		}
	}

	//计算成功率
	float nSucceedProbability = pEquip->GetSucRate(1)*nGemCount;

	float nFailRate = pEquip->GetEquipAddLevFailRate()*1.0/1000;
	nSucceedProbability += nFailRate;//加上失败加成
	if (bUseLuckyCharm)//使用天运宝符 610048
	{
		CItem *pExtItem = GetItemByType(610048);
		if (!pExtItem)
			return;
		if (SpendItem(pExtItem))
			nSucceedProbability += 10;
		else
			return;
		if ((ItemType()->GetMonoly(610048)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, 610048);
	}
	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//扣除银两
		return;
	bool bBind = false;//强化后是否绑定，如果使用了强化的绑定符则绑定装备
	for(int nkidnap=0; nkidnap<2; nkidnap++)
	{
		if (nCoseGem)//优先使用绑定的
		{
			DEBUG_TRY//try
			//删除强化符
			for (int i=0; i<nHasGemSize; i++)
			{
				CItem* pGemItemTemp = GetItem(idGemArr[i],false);
				if (!pGemItemTemp)
					continue;
				if (!pGemItemTemp->GetKidnap() && nkidnap==0)//第一次循环时只找绑定的符
					continue;
				int nAmountTemp = pGemItemTemp->GetItemAmount();
				if (nAmountTemp<=nCoseGem)
				{
					if (pGemItemTemp->GetKidnap())
						bBind = true;
					ASSERT( EraseItem( idGemArr[i], SYNCHRO_TRUE ) );
					nCoseGem-=nAmountTemp;
				}
				else if (nCoseGem>0 && pGemItemTemp)
				{
					pGemItemTemp->SetInt(ITEMDATA_AMOUNT, pGemItemTemp->GetInt(ITEMDATA_AMOUNT) - nCoseGem);
					nCoseGem=0;
					if (pGemItemTemp->GetKidnap())
						bBind = true;
					CMsgItem	msg;
					if(msg.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pGemItemTemp->GetID(),0,pGemItemTemp->GetItemAmount()))
						this->SendMsg(&msg);
					break;
				}
				else if (nCoseGem==0)
					break;
			}
			DEBUG_CATCH("WeaponLevUp,EraseItem")//end try...catch
		}
	}
	int nVipLev = 0;
	if (IsVipActive())
		nVipLev = GetVipLev();
	if (nVipLev==2)//VIP成功率加成
		nSucceedProbability += COMP_VIP_SUC_ADD2;
	else if (nVipLev==3)
		nSucceedProbability += COMP_VIP_SUC_ADD3;
	if (RandGet(100000) < nSucceedProbability*1000)//强化成功，考虑到成功率是小数，所以取100000内随机数，并把成功率乘以1000
	{
		pEquip->SetEquipAddLev(nOldLev+1);
		pEquip->UpdateEquipBaseProperty(nOldLev);//更新装备基础属性
		if (nOldLev==3 || nOldLev==6/* || nOldLev==8*/)
			pEquip->SetEquipAddLevFailRate(0);//失败加成清零

		if (bBind && !pEquip->GetKidnap())
			pEquip->SetKidnap(1);
		CMsgCompound msg;//发送成功消息
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_LEVUP_SUC, idWeapon, pEquip->GetEquipAddLev() ) )
			this->SendMsg(&msg);
		
		this->SendSysMsg(_TXTATR_TOPSHOW,"恭喜你，装备强化成功");
		if (nOldLev+1>4)
		{
			char szMsg[255];
			char UserName[128] = "";
			char itemName[128] = "";
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			sprintf(szMsg, "<%s>历尽千辛万苦，终于将<%s>强化到<3_+%d_6>！",
				UserName, itemName ,nOldLev+1);
		
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);
		}
		if (nOldLev+1 == 7)//武器特效
		{
			CMsgPlayer msgPlayer;
			msgPlayer.Create(this);
			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
		int nPos = pEquip->GetPosition();
		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
		//通知客户端更新物品信息
// 		CMsgItem	msgItem;
// 		msgItem.Create(ITEMACT_SENDINFO,0);
// 		msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 		this->SendMsg(&msgItem);
	}
	else//强化失败
	{
		if (nOldLev>4 && nOldLev!=7)//失败后降级
		{
			if (nOldLev>7)//失败后降级
				pEquip->SetEquipAddLev(7);
			else if (nOldLev > 4 && nOldLev < 7)
				pEquip->SetEquipAddLev(4);

			pEquip->UpdateEquipBaseProperty(nOldLev);//更新装备基础属性
			int nPos = pEquip->GetPosition();
			if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
			//通知客户端更新物品信息
// 			CMsgItem	msgItem;
// 			msgItem.Create(ITEMACT_SENDINFO,0);
// 			msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 			this->SendMsg(&msgItem);
		}

		pEquip->SetEquipAddLevFailRate(nFailRate*1000+0.5+COMP_SUC_FAIL_RATE*nGemCount);//失败加成
		CMsgCompound msg;//发送失败消息
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_LEVUP_FAIL, idWeapon, pEquip->GetEquipAddLev() ) )
			this->SendMsg(&msg);
		SendSysMsg("很遗憾，装备强化失败");
	}
	if ((ItemType()->GetMonoly(idGemtype)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), nGemCount, idGemtype);
// 	//获得武器
// 	CItem* pWeapon = GetItem( idWeapon);
// 	CHECK( pWeapon);
// 	if( !pWeapon->IsWeapon() )
// 		return;
// 	if( !pWeapon->IsCanUpLevel() )
// 		return;
// 	//获得宝石
// 	CItem* pGem	= GetItem( idGem );
// 	CHECK( pGem );
// 	if( !pGem->IsWeaponLevUpProperty() )
// 		return;
// 	if( pGem->GetWeaponPropertyLevel() !=  pWeapon->GetWeaponRank() )//如果武器等级和石头等级不一样不可以升级
// 		return;
// 	if( pWeapon->GetExp() < pWeapon->GetWeaponNeedExp() )//-----彭峰 2007/03/07--- 武器经验不够
// 		return;
// 
// 	//  [1/23/2008 朱斌]	幸运宝石
// 	CItemPtr pLuckyStone = NULL;
// 	BOOL	bHasLuckyGem = FALSE;
// 	for(int k = 0; k < nSize; k++)
// 	{
// 		pLuckyStone = GetItem(pDataSet[k]);
// 		if(pLuckyStone != NULL)
// 		{
// 			if(pLuckyStone->GetTypeID() == _SPCITEM_LUCKY_GEM)
// 			{
// 				bHasLuckyGem = TRUE;
// 				break;
// 			}
// 		}
// 	}
// 
// 	//-----------成功机率判断------------------------------
// 	//原始几率=（最大可升级次数-当前级数）/最大可升级次数//---彭峰-----2007/03/30---------------
// 	//魔晶增加的成功几率计算公式： 
// 	//武器需求等级：X//武器当前等级：Y//升级成功需要魔晶数量=2^(（X-1）/10)* Y
// 	//该公式含义是：2^(（X-1）/10) `代表，装备每提升一个需求档次，则要求的魔晶数量翻倍。*Y代表，武器提升N级，则要求的魔晶数量为第一次升级的N倍。
// 	//-------------------------------------------------
// 	int nowLevel = pWeapon->GetLevel();
// 	int nX = pWeapon->GetReqLev();
// 	int nMaxLevel = pWeapon->GetWeaponMaxLevel();
// /*	float nBarbarismSucceed = (float)( nMaxLevel - nowLevel )/(float)nMaxLevel;
// 	int nSureSucceed =  pow( 2,( nX - 1 ) / 10 ) * nowLevel;
// 
// 	//统计放了多少个增加机率的魂石
// 	int nStoneCount1Lev = 0;//换算1级魂石个数
// 	for( int i = 0;i < nSize;i++ )
// 	{
// 		CItem* pTempItem = GetItem( pDataSet[ i ] );
// 		if( pTempItem )
// 		{
// 			int nNowLevel = CItem::GetSpiritStoneLev( pTempItem->GetTypeID() );
// 			nStoneCount1Lev += CItem::ConvertMinLevSpiritStoneAmount( nNowLevel,1 );
// 		}
// 	}
// 	float nSucceedProbability = nBarbarismSucceed * 100 + MulDiv( nStoneCount1Lev,100,nSureSucceed )*(float)( 1 - nBarbarismSucceed );
// 	*/
// 	CItem* pOmnipotence = NULL;
// 	for( int i = 0;i < nSize;i++ )
// 	{
// 		CItem* pTmp = GetItem( pDataSet[ i ]);
// 		if( pTmp && pTmp->GetTypeID() == _SPCITEM_OMNIPOTENCE)
// 		{
// 			pOmnipotence = pTmp;
// 		}	
// 	}
// 	int nSucAdd = 5;
// 	int  nOmnID = 0;
// 	int  nTmpID = 0;
// 	if( pOmnipotence )
// 	{
// 		nOmnID = pOmnipotence->GetTypeID();// 判断第四星位的物品是否为万能石 [陈剑飞11/6/2007]
// 		nTmpID = pOmnipotence->GetID();
// 	}
// 	float nSucceedProbability = 0;
// 	const int kindOne[4] = { 100, 90, 80, 70 };
// 	const int kindTwo[4] = { 90, 80, 70, 60 };
// 	const int kindThree[4] = { 80, 70, 60, 50 };
// 	if( nX >= 20 && nX < 40 )
// 	{
// 		for( int i = 0;i < nSize;i++ )
// 		{
// 			CItem* pTempItem = GetItem( pDataSet[ i ] );
// 			if( pTempItem )
// 			{
// 				int nNowLevel = CItem::GetSpiritStoneLev( pTempItem->GetTypeID() );
// 				if( 2 == nNowLevel )
// 				{
// 					if(pOmnipotence)
// 					{
// 						if( nOmnID == _SPCITEM_OMNIPOTENCE )
// 							nSucceedProbability = kindOne[ pWeapon->GetWeaponRank() - 1 ] + nSucAdd;
// 					}
// 					else
// 						nSucceedProbability = kindOne[ pWeapon->GetWeaponRank() - 1 ];
// 					break;
// 				}
// 			}
// 		}
// 	}
// 	else if( nX >= 40 && nX < 60 )
// 	{
// 		for( int i = 0;i < nSize;i++ )
// 		{
// 			CItem* pTempItem = GetItem( pDataSet[ i ] );
// 			if( pTempItem )
// 			{
// 				int nNowLevel = CItem::GetSpiritStoneLev( pTempItem->GetTypeID() );
// 				if( 3 == nNowLevel )
// 				{
// 					if(pOmnipotence)
// 					{
// 						if( nOmnID == _SPCITEM_OMNIPOTENCE )
// 							nSucceedProbability = kindTwo[ pWeapon->GetWeaponRank() - 1 ] + nSucAdd;
// 					}
// 					else
// 						nSucceedProbability = kindTwo[ pWeapon->GetWeaponRank() - 1 ];
// 					break;
// 				}
// 			}
// 		}
// 	}	
// 	else if( nX >= 60 )
// 	{
// 		for( int i = 0;i < nSize;i++ )
// 		{
// 			CItem* pTempItem = GetItem( pDataSet[ i ] );
// 			if( pTempItem )
// 			{
// 				int nNowLevel = CItem::GetSpiritStoneLev( pTempItem->GetTypeID() );
// 				if( 4 == nNowLevel )
// 				{
// 					if(pOmnipotence)
// 					{
// 						if( nOmnID == _SPCITEM_OMNIPOTENCE )
// 							nSucceedProbability = kindThree[ pWeapon->GetWeaponRank() - 1 ] + nSucAdd;
// 					}
// 					else
// 						nSucceedProbability = kindThree[ pWeapon->GetWeaponRank() - 1 ];
// 					break;
// 				}
// 			}
// 		}
// 	}
// //	if( RandGet( 100 ) < nSucceedProbability )//成功了
// 	if(bHasLuckyGem || RandGet( 100 ) < nSucceedProbability )
// 	{
// 		pWeapon->SetWeaponUpStatus();//修改为升级以后的属性
// 		DEBUG_TRY		// VVVVVVVVVV
// 		//删除宝石
// 		ASSERT( EraseItem( idGem, SYNCHRO_TRUE ) );
// 		if( pOmnipotence )
// 		{
// 			ASSERT( EraseItem( nTmpID, SYNCHRO_TRUE ) );//  [陈剑飞11/6/2007]
// 		}
// 		
// 		//删除魂石
// 		for( int i = 0;i < nSize;i++ )
// 		{
// 			ASSERT( EraseItem( pDataSet[ i ],SYNCHRO_TRUE ) );
// 		}
// 		DEBUG_CATCH("EmbedGem2")// AAAAAAAAAAA
// 
// 		// update item info at client
// 		//20071214修罗:同步修改-----------
// //		CMsgItemInfo msg;
// //		IF_OK (msg.Create(pWeapon, ITEMINFO_UPDATE))
// //			this->SendMsg(&msg);
// 		this->SynchroItem(pWeapon,ITEMINFO_UPDATE);
// 		//--------------------------------
// 		// 发送升级成功信息
// 		CMsgCompound msg2;
// 		IF_OK( msg2.Create( MSGCOMPOUND_WEAPON_LEVUP_SUC,0 ) )
// 			this->SendMsg( &msg2 );
// 	}
// 	else
// 	{
// 		//删除武器
// //		ASSERT( EraseItem( idWeapon, SYNCHRO_TRUE ) );
// 		//删除宝石
// 		ASSERT( EraseItem( idGem, SYNCHRO_TRUE ) );
// 		if( pOmnipotence )
// 		{
// 			ASSERT( EraseItem( nTmpID, SYNCHRO_TRUE ) );//  [陈剑飞11/6/2007]
// 		}
// 		
// 		//删除魂石
// 		for( int  i = 0;i < nSize;i++ )
// 		{
// 			ASSERT( EraseItem( pDataSet[ i ],SYNCHRO_TRUE ) );
// 		}
// 		CMsgCompound msg2;
// 		IF_OK( msg2.Create( MSGCOMPOUND_WEAPON_LEVUP_FAL,0 ) )
// 			this->SendMsg( &msg2 );
// 	}
}
//[2010-07-20 goto]装备升星
void CUser::EquipAddStar( OBJID idEquip,short nGemCount,bool bUseLuckyCharm )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return;
	}
	//获得装备
	CItem* pEquip = GetItem(idEquip,false);
	if(!pEquip)
		pEquip = GetEquipItem(idEquip);

	if (!pEquip || !pEquip->IsCanUpLevel())
		return;
	if (pEquip->GetTypeID()==144000)//湛卢剑
		return;
	if (pEquip->IsWeaponSoul())
		return;
	if (pEquip->IsLocked())
		return;
	
	int nOldLev = pEquip->GetEquipStar();
	if (nOldLev<0 || nOldLev >9)
	{
		if (nOldLev==COMP_MAX_EQUIPLEV)
			SendSysMsg(_TXTATR_TOPSHOW, "此装备已经是最高星等");
		return;
	}
	int nCostMoney = pEquip->GetEquipAddLevCost(2);
	//判断银两
	if (GetMoney()<nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return;
	}
	
	int nNeedGemLev = pEquip->GetNeedGemLev(nOldLev);//需要的符等级
	//强化需要的符type
	OBJID idGemtype = 0;
	if (nNeedGemLev==1)
		idGemtype = 610012;
	else if (nNeedGemLev==2)
		idGemtype = 610013;
	else if (nNeedGemLev==3)
		idGemtype = 610014;
// 	else if (nNeedGemLev==4)
// 		idGemtype = 610015;
	if (!idGemtype)
		return;

	int nHasGemSize = 37;
	OBJID idGemArr[37] = {0};
	int nCoseGem = nGemCount;//需要扣除的升星符个数
	int nHasGemCount = GetItemByTypeMulti(idGemtype, nHasGemSize, idGemArr, true);
	if (!nHasGemCount)//背包中没有升星符
	{
		//if (!bAutoBuy)//不自动购买
		{
			SendSysMsg(_TXTATR_TOPSHOW, "包裹中没有相应的升星符");
			return;
		}
// 		else
// 		{
// 			//扣元宝买升星符强化
// 			nCoseGem = 0;//全部用元宝，不扣符
// 			vasItem* pVasItem = VASItem()->GetItemByType(idGemtype);
// 			if (!pVasItem)
// 				return;
// 			int nPriceTotal = 0;//物品价格
// 			if (pVasItem->discount==0)//没有折扣
// 				nPriceTotal = (pVasItem->price*nGemCount);
// 			else//有折扣
// 				nPriceTotal = (pVasItem->price*nGemCount)*(pVasItem->discount/100.0);
// 			
// 			if (GetMoneyByType(5)<nPriceTotal)
// 			{
// 				SendSysMsg(_TXTATR_TOPSHOW, "元宝不足");
// 				return;
// 			}
// 			
// 			SpendVas(nPriceTotal);
// 		}
	}
	if (nHasGemCount)//背包中有升星符
	{
		if (nHasGemCount<nGemCount)
		{
			//if (!bAutoBuy)
			{
				SendSysMsg(_TXTATR_TOPSHOW, "包裹中升星符数量不足");
				return;
			}
// 			else//自动购买升星符
// 			{
// 				nCoseGem = nHasGemCount;//部分用元宝，部分用符
// 				vasItem* pVasItem = VASItem()->GetItemByType(idGemtype);
// 				if (!pVasItem)
// 					return;
// 				int nPriceTotal = 0;//物品价格
// 				if (pVasItem->discount==0)//没有折扣
// 					nPriceTotal = (pVasItem->price*(nGemCount-nHasGemCount));
// 				else//有折扣
// 					nPriceTotal = (pVasItem->price*(nGemCount-nHasGemCount))*(pVasItem->discount/100.0);
// 				
// 				if (GetMoneyByType(5)<nPriceTotal)
// 				{
// 					SendSysMsg(_TXTATR_TOPSHOW, "元宝不足");
// 					return;
// 				}
// 				
// 				SpendVas(nPriceTotal);
// 			}
		}
	}
	//计算成功率
	float nSucceedProbability = pEquip->GetSucRate(2)*nGemCount;
	
	float nFailRate = pEquip->GetEquipStarFailRate()*1.0/1000;
	nSucceedProbability += nFailRate;//加上失败加成
	if (bUseLuckyCharm)//使用天运宝符 610048
	{
		CItem *pExtItem = GetItemByType(610048);
		if (!pExtItem)
			return;
		if (SpendItem(pExtItem))
			nSucceedProbability += 10;
		else
			return;
		if ((ItemType()->GetMonoly(610048)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, 610048);
	}
	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//扣除银两
		return;
	bool bBind = false;//强化后是否绑定，如果使用了强化的绑定符则绑定装备
	for(int nkidnap=0; nkidnap<2; nkidnap++)
	{
		if (nCoseGem)//优先使用绑定的
		{
			DEBUG_TRY//try
			//删除升星符
			for (int i=0; i<nHasGemSize; i++)
			{
				CItem* pGemItemTemp = GetItem(idGemArr[i],false);
				if (!pGemItemTemp)
					continue;
				if (!pGemItemTemp->GetKidnap() && nkidnap==0)//第一次循环时只找绑定的符
					continue;
				int nAmountTemp = pGemItemTemp->GetItemAmount();
				if (nAmountTemp<=nCoseGem)
				{
					if (pGemItemTemp->GetKidnap())
						bBind = true;
					ASSERT( EraseItem( idGemArr[i], SYNCHRO_TRUE ) );
					nCoseGem-=nAmountTemp;
				}
				else if (nCoseGem>0 && pGemItemTemp)
				{
					pGemItemTemp->SetInt(ITEMDATA_AMOUNT, pGemItemTemp->GetInt(ITEMDATA_AMOUNT) - nCoseGem);
					nCoseGem=0;
					if (pGemItemTemp->GetKidnap())
						bBind = true;
					CMsgItem	msg;
					if(msg.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pGemItemTemp->GetID(),0,pGemItemTemp->GetItemAmount()))
						this->SendMsg(&msg);
					break;
				}
				else if (nCoseGem==0)
					break;
			}
			DEBUG_CATCH("EquipAddStar,EraseItem")//end try...catch
		}
	}
	if (RandGet(100000) < nSucceedProbability*1000)//升星成功，考虑到成功率是小数，所以取100000内随机数，并把成功率乘以1000
	{
		pEquip->SetEquipStar(nOldLev+1);
		pEquip->UpdateEquipAdditionProperty(nOldLev);//更新装备附加属性
		if (nOldLev==3 || nOldLev==6/* || nOldLev==8*/)
			pEquip->SetEquipStarFailRate(0);//失败加成清零
		
		if (bBind && !pEquip->GetKidnap())
			pEquip->SetKidnap(1);

		CMsgCompound msg;//发送成功消息
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_ADDSTAR_SUC, idEquip, pEquip->GetEquipStar() ) )
			this->SendMsg(&msg);
		SendSysMsg("恭喜你，装备升星成功");

		if (nOldLev+1>4)
		{
			char szMsg[255];
			char UserName[128] = "";
			char itemName[128] = "";
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			sprintf(szMsg, "<%s>将<%s>提升到<3_%d_6>星时，天空忽闪现异彩，仿佛日月都失去了光华!",
				UserName, itemName ,nOldLev+1);
//			char szItem[128];
//			sprintf(szItem,"%d_%d_%s_%d_%d_%d",
//				pEquip->GetID(), pEquip->GetTypeID(), ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);
		}
		int nPos = pEquip->GetPosition();
		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
		//通知客户端更新物品信息
// 		CMsgItem	msgItem;
// 		msgItem.Create(ITEMACT_SENDINFO,0);
// 		msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 		this->SendMsg(&msgItem);
	}
	else//升星失败
	{
		if (nOldLev>4 && nOldLev!=7)//失败后降星
		{
			if (nOldLev>7)//失败后降星
				pEquip->SetEquipStar(7);
			else if (nOldLev > 4 && nOldLev < 7)
				pEquip->SetEquipStar(4);
			
			pEquip->UpdateEquipAdditionProperty(nOldLev);//更新装备附加属性
			int nPos = pEquip->GetPosition();
			if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
			//通知客户端更新物品信息
// 			CMsgItem	msgItem;
// 			msgItem.Create(ITEMACT_SENDINFO,0);
// 			msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 			this->SendMsg(&msgItem);
		}
		
		pEquip->SetEquipStarFailRate(nFailRate*1000+0.5+COMP_SUC_FAIL_RATE*nGemCount);//失败加成
		CMsgCompound msg;//发送失败消息
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_ADDSTAR_FAIL, idEquip, pEquip->GetEquipStar() ) )
			this->SendMsg(&msg);
		SendSysMsg("很遗憾，装备升星失败");
	}
	if ((ItemType()->GetMonoly(idGemtype)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), nGemCount, idGemtype);
}
//goto 魂印 610047(魂印道具ID) 花费10G
bool CUser::EquipSoulMark(OBJID idEquip)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return false;
	}
	//获得装备
	CItem* pEquip = GetItem(idEquip,false);
	if(!pEquip)
		pEquip = GetEquipItem(idEquip);

	if (!pEquip || !pEquip->IsCanUpLevel())
		return false;
	if (pEquip->GetTypeID()==144000)//湛卢剑
		return false;
	if (pEquip->IsWeaponSoul())
		return false;
	if (pEquip->IsSoulMark())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "物品已经是魂印状态");
		return false;
	}
	if (pEquip->IsLocked())
		return false;
	int nEquipColor = pEquip->GetColour();

	int nCostRes = 1;//需要的材料个数
	if(nEquipColor==4)
		nCostRes = 2;
	else if(nEquipColor==5)
		nCostRes = 3;

	int nCostMoney = 100000;
	if (GetMoney()<nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return false;
	}
	CItem *pItem = GetItemByType(610047);
	if (!pItem)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中没有魂印需要的材料");
		return false;
	}
	bool bIsCostly = pItem->IsCostlyItem();
// 	if (!SpendMoney(nCostMoney, true))
// 		return false;
	bool bHasKind = false;
	if (DelItemByTypeMulti(610047, nCostRes,bHasKind, false))
	{
		SpendMoney(nCostMoney, true);
		pEquip->SetKidnap(2);
		pEquip->UpdateEquipBaseProperty(pEquip->GetEquipAddLev(),1);
		SendSysMsg(_TXTATR_TOPSHOW, "装备魂印操作成功");
		CMsgCompound msg;//发送消息
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_SOULMARK_SUC, idEquip, 0 ) )
			this->SendMsg(&msg);
		int nPos = pEquip->GetPosition();
		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
		if (bIsCostly)//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), nCostRes, 610047);
		return true;
	}

	return false;
}
//[2010-08-04 goto]宝石合成
bool CUser::GemLevUp(OBJID idCharm, set<OBJID> pDataSet,int nSize)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return false;
	}
	int nGemType = 0;
	CItem* pGem = NULL;
	int nCountGem = 0;
	bool bKidnap = false;//合成后是否绑定
	set<OBJID>::iterator it = pDataSet.begin();
	for (; it!=pDataSet.end(); it++)
	{
		pGem = GetItem(*it,false);
		if (pGem)
		{
			if (pGem->GetItemType()!=41 && pGem->GetItemType()!=42)//不是宝石
				return false;
			if (pGem->IsLocked())
			{
				SendSysMsg(_TXTATR_TOPSHOW, "锁定的物品不能进行此操作");
				return false;
			}
			if (nGemType==0)
			{
				nGemType = pGem->GetTypeID();
			}
			else if (pGem->GetTypeID()!=nGemType)//宝石种类不同
			{
				SendSysMsg(_TXTATR_TOPSHOW, "只有相同的宝石才能合成");
				return false;
			}
			if (pGem->GetKidnap())//只有原料中有一个是绑定的，合成后就为绑定
				bKidnap = true;
			nCountGem+=pGem->GetItemAmount();
		}
		else
			return false;
	}
	if (nSize>nCountGem || nCountGem==0)//宝石个数不对或者根本没有宝石
		return false;

	if (nGemType%10==9 || nGemType%10==0)//目前最高9级
	{
		SendSysMsg(_TXTATR_TOPSHOW, "宝石已经是最高等级");
		return false;
	}
	int nCostMoney = 5000;
	if (nCostMoney>GetMoney())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return false;
	}
	bool bIsCostly = pGem->IsCostlyItem();
	int nSucceedProbability = (nSize-2)*25;//成功率,3个宝石合成的成功率是25%,增加一个额外的相同宝石，可以增加额外25%的成功率

	CItem* pItemCharm = NULL;
	bool bIsCostlyCharm = false;
	int nCharmType = 0;
	if (idCharm)
	{
		pItemCharm = GetItem(idCharm,false);
		if (!pItemCharm)
			return false;

		nCharmType = pItemCharm->GetTypeID();
		if (nCharmType!=610016 && nCharmType!=610017)
			return false;

		if (nCharmType==610016 && nGemType%10>3)//低级合成符只能合成3级(包括3级)以下的宝石
		{
			SendSysMsg(_TXTATR_TOPSHOW, "宝石合成符的等级不符");
			return false;
		}

		nSucceedProbability+=25;
		bIsCostlyCharm = pItemCharm->IsCostlyItem();
		if (pItemCharm->GetKidnap())
			bKidnap = true;
	}
	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//扣除银两
		return false;
	DEBUG_TRY//try
		//删除宝石
		it = pDataSet.begin();
		int nNeedGem = nSize;
		for (; it!=pDataSet.end(); it++)
		{
			CItem* pGem = GetItem(*it,false);
			if (!pGem)
				continue;
			int nHasAmount = pGem->GetItemAmount();
			if (nHasAmount >= nNeedGem)
			{
				if (SpendItem(pGem, nNeedGem))
					nNeedGem = 0;
				else
					return false;
			}
			else
			{
				if (SpendItem(pGem, pGem->GetItemAmount()))
					nNeedGem -= nHasAmount;
				else
					return false;
			}
			if (nNeedGem == 0)
				break;
			//ASSERT( EraseItem( *it, SYNCHRO_TRUE ) );
		}
		if (nNeedGem != 0)
			return false;
		if (pItemCharm)
			ASSERT( SpendItem( pItemCharm ) );
	DEBUG_CATCH("GemLevUp,EraseItem")//end try...catch

	if (RandGet(100) < nSucceedProbability)//合成成功
	{
		CItem* pNewGem = NULL;
		if (!bKidnap)
			pNewGem = AwardItem(nGemType+1);
		else
			pNewGem = AwardItem(nGemType+1, true, false, true, false, 1, false, true, false, 1);

		if (pNewGem)
		{
			CMsgCompound msg;//发送成功消息
			IF_OK( msg.Create( MSGCOMPOUND_GEM_LEVUP_SUC, 0, 0 ) )
				this->SendMsg(&msg);
			SendSysMsg("恭喜你，宝石合成成功");
			if ((nGemType%10+1)>2)
			{

				char szMsg[255];
				char UserName[128] = "";
				char itemName[128] = "";
				char gemName[128]  = "";
				sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
				sprintf(gemName,  "%d_[%s]_%d_%d_%d_%d_%d",
					SNAPSHOTOBJ,  ItemType()->QueryItemType(nGemType+1)->GetStr(ITEMTYPEDATA_NAME), pNewGem->GetID(), pNewGem->GetTypeID(), this->GetID(), pNewGem->GetKidnap(), pNewGem->GetColour());
				sprintf(szMsg, "<%s>合成了一颗<%s>，真是令人羡慕！",
					UserName, gemName);
//				char szItem[128];
//				sprintf(szItem,"%d_%d_%s_%d_%d_%d",
//					pNewGem->GetID(), pNewGem->GetTypeID(), ItemType()->QueryItemType(pNewGem->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), this->GetID(), pNewGem->GetKidnap(), pNewGem->GetColour());
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pNewGem->GetID(), pNewGem->GetTypeID(), NULL, ALLUSERS_NAME);
			}
			if (pNewGem->IsCostlyItem())//goto 贵重物品记录
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from gem_upgrade", GetID(), GetName(), nGemType+1);
		}
	}
	else
	{
		SendSysMsg("很遗憾，宝石合成失败");
		CMsgCompound msg;//发送失败消息
		IF_OK( msg.Create( MSGCOMPOUND_GEM_LEVUP_FAIL, 0, 0 ) )
			SendMsg(&msg);
	}
	if (bIsCostly)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from gem_upgrade", GetID(), GetName(), nSize, nGemType);
	if (bIsCostlyCharm)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from gem_upgrade", GetID(), GetName(), 1, nCharmType);
	return true;
}

BOOL CUser::EnterInstance(OBJID idMapInstance,UINT nMapType,UINT nInstanceScope,UINT nDomainMapID,int x,int y)///进入指定类型的副本
{
	BOOL hr=false;
	//int nPosX=0,nPosY=0;
// 	POINT ptPortal;
	if(nInstanceScope==0 ||nInstanceScope==1 ||nInstanceScope==3)
	{
		if (g_SpecilLine==0)
			SetRecordPos(GetMapID(),GetPosX(),GetPosY());///保存玩家位置信息,以便在掉线可以在副本门口
		else if (g_SpecilLine==1)
			SetSpecilPos(GetMapID(),GetPosX(),GetPosY(),STATE_SPECILLINE_NORMAL);
		hr=FlyMap(idMapInstance,x, y);
	}
//modify code by 林德渊 begin 2011.5.24
	::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) enter enterinstance(%u) maptype(%d) scope(%d) mapid(%d) ", GetID(), GetName(), idMapInstance, nMapType, nInstanceScope, nDomainMapID);
//modify code by 林德渊 end
// 	else if(nInstanceScope>1 && nInstanceScope<6)
// 	{
// 		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///保存玩家位置信息,以便在掉线可以在副本门口
// 		hr=FlyMap(idMapInstance,x, y);
// 	}
//	else if(nInstanceScope==2)
//	{
//		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///保存玩家位置信息,以便在掉线可以在副本门口
//		hr=FlyMap(idMapInstance,x, y);
//	}
//	else if(nInstanceScope==3)
//	{
//		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///保存玩家位置信息,以便在掉线可以在副本门口
//		hr=FlyMap(idMapInstance,x, y);
//	}
//	else if(nInstanceScope==4)
//	{
//		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///保存玩家位置信息,以便在掉线可以在副本门口
//		hr=FlyMap(idMapInstance,x, y);
//	}
// 	else if(nInstanceScope==6)
// 	{
// 		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///保存玩家位置信息,以便在掉线可以在副本门口
// 		hr=FlyMap(idMapInstance,x, y);
// 	}
	return hr;
}


// bool	CUser::EnterArena(UINT idArenaMap,int x,int y)
// {
// 
// }

void CUser::AddTeamLeader(OBJID leaderId)
{
	UserManager()->AddTeamLeader(leaderId);
}

void CUser::SendTeamMsg(int nChannel,const char* pMsg)
{
	CTeam* pTeam = GetTeam();
	if(pTeam)
	{
	
		CMsgTalk msg;
		if (msg.Create(SYSTEM_NAME, GetName(), pMsg, NULL, 0x00ffff00, nChannel))
			pTeam->BroadcastTeamMsg(&msg);
	}
	
}

//BOOL CUser::RequestRefreshInstance(OBJID idOwn,UINT nInstanceType)///请求进入指定类型的副本
//{
//	BOOL hr=false;
//	///向5号地图组发请求,告之有一个User想去指定类型的副本
//	CMsgMapInstance msg;
//	msg.CreateRequestEnterMsg(CurrentMapGroup()->GetID(),GetID(),idOwn,nInstanceType);///生成请求进入副本的消息
//	CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_INSTANCE,&msg);
//	return hr;
//}

bool CUser::RequestEnterInstance(UINT nInstanceType,UINT nDomainMapID,UINT nInstanceScope,UINT nCellx ,UINT nCelly)///请求进入指定类型的副本
{
	if(!CanBeSee())
		return false;
	
	///向副本地图组发请求,告之有一个User想去指定类型的副本
	OBJID idOwn=ID_NONE;
	if(nInstanceScope==INSTANCETYPE_USER)	//普通Team副本
	{
		idOwn = GetID();
		nDomainMapID = (GetLev() - 1) / 5 + 1;
	}
	if(nInstanceScope==INSTANCETYPE_TEAM)	//普通Team副本
	{	
		if (GetTeam())
		{
			idOwn=GetTeam()->GetID();
			if(nDomainMapID < 1000000)
			{
				//记录进副本标志以防掉线补登、
				GetTeam()->SetLocation(GetID(),nInstanceType + 100000,nCellx,nCelly,true);
				if (nInstanceType==23)//帮派新副本--jay
				{
					CSyndicate *psyn= SynManager()->QuerySyndicate(GetSynID());
					if (psyn)
					{
						if (nDomainMapID==0)
						{
							SendSysMsg("您的帮派还没有开启青龙堂，无法进入该地区");
							return false;
						}
						else if (nDomainMapID > 10)
						{
							if(IsGM())
								SendSysMsg("脚本传入一个不允许的值!");
							return false;
						}
					}
					else
					{
						SendSysMsg("你没有帮!");
						return false;
					}
				}
				else
					nDomainMapID = (GetTeam()->GetMemberLev() - 1) / 5 + 1;
			}
		}
		else
		{
			SendSysMsg("队伍副本没有找到队伍");
			return false;	
		}
	}
// 	else if(nInstanceScope==INSTANCETYPE_SYN)	//普通军团副本
// 	{
// 		if(GetSynID())
// 			idOwn=GetSynID();
// 		else
// 		{
// 			SendSysMsg("军团副本没有找到军团");
// 			return;
// 		}
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_CITYWAR)	//城战副本
// 	{
// 		idOwn = nDomainMapID;
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_ACTIVE)	
// 	{
// 		idOwn = nDomainMapID;
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_MARRY) //婚姻副本
// 	{
// 		CItem* pItem = NULL;
// 		pItem = GetMarryPass();  //获得婚礼通行证
// 		if (!pItem)
// 		{
// 			pItem = GetMarryPaper();  //获得婚礼喜帖
// 			if (!pItem)		
// 				return;
// 		}
// #ifdef _DEBUG
// 		char szIDItem[32];
// 		sprintf(szIDItem,"%d",m_idItem);
// 		this->SendSysMsg(szIDItem);
// #endif
// 		pItem = this->GetItem(m_idItem);
// 		if(!pItem)
// 			return;
// 
// 		OBJID idInstance = pItem->GetMarryId();
// 		if(!idInstance)
// 			return;
// 					
// 		int nMarryPassId1 = atoi(g_objStrRes.GetStr(600008));//婚礼通行证ID,豪华婚礼通行证
// 		int nMarryPassId2 = atoi(g_objStrRes.GetStr(600010));//婚礼通行证ID,普通婚礼通行证
// 
// 		OBJID idItemType = pItem->GetInt(ITEMDATA_TYPE);
// 		if(idItemType != nMarryPassId1&&idItemType != nMarryPassId2)
// 		   EraseItem(pItem->GetID(),SYNCHRO_TRUE);
// 
// 		idOwn = idInstance;
// 	}
// 	else if(INSTANCETYPE_SYNWAR == nInstanceScope)
// 	{
// 		idOwn = GetSynWarInstanceDefenceID();
// 	}

	CMsgMapInstance msg;
	msg.CreateRequestEnterMsg(CurrentMapGroup()->GetID(),GetID(),idOwn,nInstanceType,nInstanceScope,nDomainMapID,nCellx,nCelly,NULL);///生成请求进入副本的消息
	CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_INSTANCE,&msg);
	return true;
}
//-------------------------------------------

//-------彭峰---2007/05/30-----
BOOL	CUser::ResetItemCD( UINT nDelayTimeMS, UINT TypeID, CTimeOutMS* pTimeOutMS  )
{
	CHECKF( pTimeOutMS );
	CHECKF( nDelayTimeMS > 0 );
// 	int nQueryIndex = CItem::GetDelayGroupIndex( TypeID );
	CMsgCoolDown msg;
	if( msg.Create( this->GetID(),MAGICCOOLDOWN_BEGIN,ITEM_INFO,TypeID/*nQueryIndex*/,nDelayTimeMS, 0 ) ) //公共CD
		this->SendMsg( &msg );
	pTimeOutMS->SetInterval(nDelayTimeMS);
	pTimeOutMS->Update();
	
	return true;
}

bool CUser::RebornInstance(bool bInstance /* = true */)
{
	if( this->IsAlive() )
		return false;
	if(m_idProcess != MSGPORT_MAPGROUP_INSTANCE)
		return false;
	if(m_nInstanceRebornX == 0 && m_nInstanceRebornY == 0)
		return false;

	DetachStatus( QueryRole(),STATUS_DIE );// 添加清除死亡状态

	int nMaxLife = this->GetMaxLife();
	int nMaxMana = this->GetMaxMana();
	int nMaxSp = this->GetMaxSp();		
	this->SetLife( nMaxLife /2);
	this->SetMana(nMaxMana/2);
	this->SetSp( nMaxSp/2 );

	CMsgUserAttrib	msg;
	if( msg.Create( GetID(),_USERATTRIB_LIFE,nMaxLife/2 )
		&& msg.Append( _USERATTRIB_MANA,nMaxMana/2 )
		&& msg.Append( _USERATTRIB_SP,nMaxSp/2 )
		)
	{
		BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
//-----------------------------------------------------------------------------------
	SetAttrib( _USERATTRIB_KEEPEFFECT, GetEffect(), SYNCHRO_BROADCAST );
	
	this->BroadcastTeamLife(true);


	m_bGhost	= false;

	int nFlyMapResult = -1;
	nFlyMapResult = this->FlyMap(/*nMapID*/0, m_nInstanceRebornX, m_nInstanceRebornY);
	
	CMsgAction msgAction;
	if(msgAction.Create(this->GetID(), this->GetPosX(), this->GetPosY(), this->GetDir(), actionStandBy))
	{
		this->SendMsg(&msgAction);
	}

	CRole::AttachStatus(this, STATUS_CITYWAR_SAFETIME, 50, 7);

	return true;
}
//-----------------------------------------------------------------------------
//  [6/15/2007]
//	王玉波
//  使用复活卷轴复活
bool CUser::RebornUseScroll(BOOL bUseScroll /*=TRUE*/,bool bForce/* = false*/)
{
	if( this->IsAlive() )
		return false;

	bool bReborn = false;
	if (bUseScroll)
	{
		if(GetLev() > 20)
		{
			CItem* pItem = NULL;
			pItem = GetItemByType( REBORN_SCROLL );
			if( pItem )
			{
				if(this->SpendItem( pItem ))
				{
					this->SendSysMsg("原地满血满魔复活了");
					bReborn = true;
				}
			}
		}
		else
		{
			this->SendSysMsg("原地满血满魔复活了");
			bReborn = true;
		}
		
	}

	DetachStatus( QueryRole(),STATUS_DIE );
	m_bGhost	= false;

	int nMaxLife = 0;
	int nMaxMana = 0;
	int nMaxSp = 0;
	int nMap = 0;
	int nPosX = 0;
	int nPosY = 0;

	if (!this->GetMap())
	{
		return false;
	}
	if (this->GetMap()->IsPkGameMap())
	{
		if(this->GetMap()->GetType()==3)//专线静态地图
		{
			IStatus* pStatus = QueryStatus(STATUS_WARGAME);//竞技场
			if(pStatus)
			{
				WargameManager()->ProcessReborn(pStatus->GetPower(), GetID());
				if(!WargameManager()->GetRebornPos(pStatus->GetPower(), GetID(), nMap, nPosX, nPosY))
				{
					nMap = 1026;
					nPosX = 34;
					nPosY = 31;
				}
			}
			else
			{
				nMap = 1026;
				nPosX = 34;
				nPosY = 31;
			}
		}
		else
		{
			nMap = GetMapID();
			nPosX = 77;
			nPosY = 36;
		}
	}
	else if (this->GetMap()->IsSynMap())
	{
		nMap = this->GetMapID();
		nPosX = 79;
		nPosY = 33;
	}
	else
	{
		nMap = 1026;
		nPosX = 34;
		nPosY = 31;
	}

	bool bRebornInInstance = false;
	if(!bForce)
	{
		if(this->GetMap()->IsDynaMap())
		{
			CMapInstance* pMapInstance = MapInstanceMgr()->FindInstance(this->GetMapID());
			if(pMapInstance && pMapInstance->GetBornX() != 0 && pMapInstance->GetBornY() != 0)
			{
				nMap = 0;
				nPosX = pMapInstance->GetBornX();
				nPosY = pMapInstance->GetBornY();

				m_bInstanceReborn=true;
			}
		}
	}

	if(bReborn)
	{
		nMaxLife = GetMaxLife();
		nMaxMana = GetMaxMana();
		nMaxSp = 0;
	}
	else
	{
		nMaxLife = GetMaxLifeUserOnly()/10;
		nMaxMana = GetMaxManaUserOnly()/10;
		nMaxSp = 0;

		if(QueryStatus(STATUS_WARGAME))
		{
			nMaxLife = GetMaxLife()/2;
			nMaxMana = GetMaxMana()/2;
			nMaxSp = 0;
		}
	}

	this->SetLife( nMaxLife );
	this->SetMana(nMaxMana);
	this->SetSp( nMaxSp );
	
	CMsgUserAttrib	msg;
	if( msg.Create( GetID(),_USERATTRIB_LIFE,nMaxLife )
		&& msg.Append( _USERATTRIB_MANA,nMaxMana )
		&& msg.Append( _USERATTRIB_SP,nMaxSp )
		)
	{
		BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
	
	const bool bMaxLife = true;
	this->BroadcastTeamLife(bMaxLife);
	
	CMsgAction msgAction;
	if(msgAction.Create(this->GetID(), this->GetPosX(), this->GetPosY(), this->GetDir(), actionStandBy))
	{
		BroadcastRoomMsg(&msgAction, INCLUDE_SELF,EM_SENDNPCFLAG_ENFORCE);
	}

	
	if(!bReborn)
		FlyMap(nMap,nPosX,nPosY);

	return true;
}

//	20070712	朱斌
BOOL CUser::IsUserHaveAnExclusiveItem(OBJID idType, const char* pszItemName, CUser* pUser)
{
	if(!pUser)
		return FALSE;
	
	CItemTypeData* pItemType = ItemType()->QueryItemType(idType);
	CHECKF(pItemType);
	unsigned int uMonopoly = pItemType->GetInt(ITEMTYPEDATA_MONOPOLY);
	if(CItem::IsUserExclusive(uMonopoly)
		&& pUser->GetUserAllItemByType(idType))
	{
		char str[512];
		sprintf(str,g_objStrRes.GetStr(14090) , pItemType->GetStr(ITEMTYPEDATA_NAME));//STR_ITEM_EXCLUSIVE
		pUser->SendSysMsg(str);
		return TRUE;			
	}

	return FALSE;
}

//	20070712	朱斌	
//	查找仓库
CItemPtr CUser::GetStorageItemByType(OBJID idType)	
{
	if(!m_pStorage)
		return NULL;
// 		m_pStorage	= CPackage::CreateNew();
// 	CHECKF(m_pStorage);
// 
//    	//仓库互通
//  	CHECKF(m_pStorage->Create(ITEMPOSITION_STORAGE, GetID(), GetID(), 0, Database()));
// 
// 	if(!m_pStorage)
// 		return NULL;

	return m_pStorage->GetItemByTypeID(idType);
}
//--------------------------
//20070716修罗:在自己脚边掉落物品.
//物品非自身,而是系统创建的
//mapItem创建时,任何人都可以拾取
BOOL CUser::DropItemCreatedBySys(DWORD itemType,//itemtype or money
						  BOOL bIsMoney/*=false*///true:money.false:item						  
						  )
{
// 	CHECKF(itemType);
// 	
// 	if(bIsMoney)//掉落金钱
// 	{		
// 		POINT	pos;
// 		pos.x	= GetPosX();
// 		pos.y	= GetPosY();
// 		if(GetMap()->FindDropItemCell(MONSTERDROPITEM_RANGE, &pos))
// 		{
// 			CMapItem* pMapItem = CMapItem::CreateNew();
// 			if(pMapItem)
// 			{
// 				if(pMapItem->CreateMoney(MapManager()->SpawnMapItemID(), 
// 					GetMap(), 
// 					pos, 
// 					itemType,//dwMoney, 
// 					NULL,//idOwner,
// 					ID_NONE//teamID
// 					))//teamID
// 					//-----------------------------------------------------
// 				{
// 					MapManager()->QueryMapItemSet()->AddObj(pMapItem);
// 					return true;
// 				}
// 				else
// 					pMapItem->ReleaseByOwner();
// 			}
// 		}
// 	}
// 	else//掉落物品
// 	{		
// 		POINT	pos;
// 		pos.x	= GetPosX();
// 		pos.y	= GetPosY();
// 		if( GetMap()->FindDropItemCell(MONSTERDROPITEM_RANGE, &pos))
// 		{
// 			CMapItem* pMapItem = CMapItem::CreateNew();
// 			if(pMapItem)
// 			{
// 				bool bUnIdent = false; 
// 				//参考monster掉落物品
// 				if(pMapItem->Create(MapManager()->SpawnMapItemID(), 
// 					GetMap(), 
// 					pos, 
// 					itemType, 
// 					NULL,//idOwner,任何人可以拾取 
// 					0,//nMagic2, 
// 					0,//nMagic3, 
// 					bUnIdent, 
// 					0,//nUserLuck,
// 					ID_NONE//teamID
// 					))//新加teamID
// 					//-----------------------------------------------------------------------
// 				{
// 					MapManager()->QueryMapItemSet()->AddObj(pMapItem);
// 					return true;
// 				}
// 				else
// 					pMapItem->ReleaseByOwner();
// 			}
// 		}
// 	}
	
	return false;


	
}

void CUser::ReadAutoFightData()
{
// 	IDatabase* pDatabase=Database();
// 	if(m_pAutoFightData)
// 	{
// 		string strDefaultSkill;
// 		if(this->GetProfession()&PROF_MALE)
// 		{
// 			strDefaultSkill = "急速射击";
// 		}
// 		else if(this->GetProfession()&PROF_MALE)
// 		{
// 			strDefaultSkill = "虎牙突刺";
// 		}
// 		else if(this->GetProfession()&PROF_MALE)
// 		{
// 			strDefaultSkill = "寒冰诀";
// 		}
// 		else
// 		{
// 		}
// 
// 		m_pAutoFightData->AutoFightReadDB(GetID(), pDatabase, strDefaultSkill.c_str());
// 	}
}

void CUser::WriteAutoFightData(struct AutoFightInfo &AutoInfo)
{
	if(m_pAutoFightData.AutoFightWriteDB(AutoInfo, NULL))
		SendSysMsg("挂机信息保存成功！");
}

void CUser::ReplyAutoFightData()
{
// 	if(NULL == m_pAutoFightData)
// 		return ;
// 
// 	const CAutoFightInfo *pInfo = m_pAutoFightData->GetAutoFightInfo();
// 	if(NULL != pInfo)
// 	{
// 		CMsgAutoFight msg;
// 		if(msg.Create(_AutoFight_READ, pInfo))
// 		{
// 			this->SendMsg(&msg);
// 		}
// 	}

}

void CUser::SetAutoFightPickUpMode(int nMode)
{
// 	if(NULL == m_pAutoFightData)
// 		return ;
// 
// 	m_pAutoFightData->SetPickUpMode(nMode);
}



void CUser::SaveEudemon()
{
	m_pUserPet->SaveData();
}

void CUser::SendASalesItem()
{
	CMsgVAS msg;
	if (!msg.Create(0, 0, GetID(), _MSGVAS_SYN_ASALEITEM))
		return;
	
	vasItem* pVasItem = NULL;
	for (int i=0; i<VASItem()->GetASaleItemAmount(); i++)
	{
		pVasItem = VASItem()->GetASaleItemByIndex(i);
		if (pVasItem)
			msg.Add(pVasItem->itemType, pVasItem->aSaleCount, pVasItem->price);
	}
	SendMsg(&msg);
}
void CUser::SendLimitItem()
{
	CMsgVAS msg;
	if (!msg.Create(0, 0, GetID(), _MSGVAS_SYN_LIMITITEM))
		return;
	
	vasLimitItem* pVasItem = NULL;
	for (int i=0; i<VASItem()->GetLimitItemAmount(); i++)
	{
		pVasItem = VASItem()->GetLimitItemByIndex(i);
		if (pVasItem)
			msg.Add(pVasItem->itemType, pVasItem->aSaleCount, 0);
	}
	SendMsg(&msg);
}

bool CUser::AwardWeaponExp(DWORD dwExp)
{
	CItem* pItem = GetEquipItemByPos(ITEMPOSITION_WEAPONSOUL);
	if(!pItem)
	{
		SendSysMsg("你没有装备魂魄");
		return false;
	}
	if(!pItem->IsWeaponSoul())
	{
		SendSysMsg("你没有装备魂魄");
		return false;
	}


	ST_WEAPONSOULDATA wpslInfo;
	CWeaponSoulData* pData = pItem->QueryWeapensoulData();
	if(!pData)
	{
		SendSysMsg("你没有装备魂魄");
		return false;
	}
	if(!pData->GetInfo(wpslInfo))
	{
		SendSysMsg("你没有装备魂魄");
		return false;
	}


	DWORD nExp = wpslInfo.exp;
	DWORD nExpleave = dwExp;
	DWORD nAddLev = 0;
	while(nExpleave > 0)
	{
		DWORD dwNeedExp = CItem::GetWeaponLevExp(wpslInfo.level + nAddLev);
		if(dwNeedExp == 0)
			return false;
		else if(dwNeedExp > nExpleave + nExp)
		{
			nExp += nExpleave;
			nExpleave = 0;
			break;
		}
		else
		{
			//升级了
			if(wpslInfo.level >= GetLev())
			{
				SendSysMsg("魂魄经验已满，无法继续使用");
				return false;
			}

			nAddLev ++;
			nExpleave -= dwNeedExp-wpslInfo.exp;
			nExp = 0;
		}
	}

	wpslInfo.level += nAddLev;
	wpslInfo.exp = nExp;

	int nOldType = wpslInfo.nType % 10;
	pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_EXP);

	if(nAddLev > 0)
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);

	
	if(pData->GetInfo(wpslInfo))
	{
		if(nOldType != wpslInfo.nType % 10)
		{
			CMsgWeapensoulShow msgwpsl;
			if(msgwpsl.Create(wpslInfo,EM_MSGWPSLSHOW_IN,GetID(),pItem->GetID()))
				this->BroadcastRoomMsg(&msgwpsl, INCLUDE_SELF);
		}
		CMsgWeaponSoul msgSoul;
		if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,GetID(),pItem->GetID()))
			SendMsg(&msgSoul);
	}

	return true;
}

bool CUser::FullWeaponLifespan()
{
	CItem* pItem = GetEquipItemByPos(ITEMPOSITION_WEAPONSOUL);
	if(!pItem)
		return false;
	if(!pItem->IsWeaponSoul())
		return false;
	
	CWeaponSoulData* pData = pItem->QueryWeapensoulData();
	if(!pData)
		return false;
	
	if(pData->GetInt(WEAPONSOULDATA_LIFESPAN) >=  300)
		return false;

	pData->SetInt(WEAPONSOULDATA_LIFESPAN,300);
	
	ST_WEAPONSOULDATA wpslInfo;
	if(pData->GetInfo(wpslInfo))
	{
		CMsgWeaponSoul msgSoul;
		if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,GetID(),pItem->GetID()))
			SendMsg(&msgSoul);
	}
	
	return true;
}

//宝石雕琢
OBJID CUser::GemCutwork(OBJID idGem, OBJID idCharm)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return ID_NONE;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return ID_NONE;
	}
	//获得装备
	CItem* pGem = GetItem(idGem,false);
	if(!pGem || !pGem->IsGem())
		return ID_NONE;
	//获得雕琢符
	CItem* pCharm = GetItem(idCharm, false);
	if(!pCharm || pCharm->GetTypeID()!=610056)
		return ID_NONE;
	
	if(!pGem->CanCutWork())
		return ID_NONE;

	if (pGem->IsLocked() || pCharm->IsLocked())
		return ID_NONE;

	int nGemType = pGem->GetTypeID();
	int nGemLev = nGemType%100;
	int nCostMoney = 0;
	if(nGemLev==0)
		nCostMoney = 500000;
	else
		nCostMoney = nGemLev * 50000;

	if (GetMoney()<nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return ID_NONE;
	}

	bool bIsCostly = pCharm->IsCostlyItem();
	bool bKindnap = false;
	if(pCharm->GetKidnap() || pGem->GetKidnap())
		bKindnap = true;

	OBJID idNewGem = ID_NONE;
	DEBUG_TRY
	char szCharm[128] = "";
	sprintf(szCharm, "%d_[%s]_0_%d_0_0_%d", SNAPSHOTOBJ, pCharm->GetStr(ITEMDATA_NAME), pCharm->GetTypeID(), pCharm->GetColour());
	char szOldGem[128] = "";
	sprintf(szOldGem, "%d_[%s]_0_%d_0_0_%d", SNAPSHOTOBJ, pGem->GetStr(ITEMDATA_NAME), nGemType, pGem->GetColour());
	if (SpendItem(pCharm))
	{
		CItem* pNewGem = NULL;
		SpendMoney(nCostMoney, true);

		if(SpendItem(pGem))
		{
			if(!bKindnap)
				pNewGem = AwardItem(nGemType+10);
			else
				pNewGem = AwardItem(nGemType+10, true, false, true, false, 1, false, true, false, 1);

			if(pNewGem)
			{
				idNewGem = pNewGem->GetID();
				if(nGemLev>2)
				{
					//系统公告
					char szMsg[255] = "";
					char UserName[128] = "";
					char gemName[128]  = "";
					sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, GetName());
					sprintf(gemName,  "%d_[%s]_%d_%d_%d_%d_%d",
						SNAPSHOTOBJ,  pNewGem->GetStr(ITEMDATA_NAME), pNewGem->GetID(), pNewGem->GetTypeID(), GetID(), pNewGem->GetKidnap(), pNewGem->GetColour());
					sprintf(szMsg, "<%s>使用<%s>将<%s>雕琢成<%s>，大幅提升了宝石的属性。",UserName, szCharm, szOldGem,gemName);

					UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pNewGem->GetID(), pNewGem->GetTypeID(), NULL, ALLUSERS_NAME);
				}
				if(pNewGem->IsCostlyItem())//goto 贵重物品记录
				{
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from gem_cutwork", GetID(), GetName(), 1, nGemType);
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from gem_cutwork", GetID(), GetName(), 1, nGemType+10);
				}
			}
			else
			{
				::LogSave("CUser::GemCutwork awarditem failed!userid=%u,itemtype=%d", GetID(), nGemType+10);
			}
		}
		else
		{
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, 610056);
			return ID_NONE;
		}

		if (bIsCostly)//goto 贵重物品记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, 610056);
	}
	else
		return ID_NONE;
	DEBUG_CATCH3("CUser::GemCutwork error!userid=%u,gemtype=%d", GetID(),nGemType)
	return idNewGem;
}
//刷新宠物蛋数据
bool CUser::FlushPetegg(OBJID idData, OBJID idItem, int nGrow, int nNextFlush)
{
	CHECKF(m_pPetEgg);
	CHECKF(idItem);

	CItem* pItem = GetItem(idItem, false, true);//包括仓库里的
	CHECKF(pItem);

	if(nGrow<0 || nGrow>100)
	{
		::LogSave("CUser::FlushPetegg err grow!nGrow=%d", nGrow);
		nGrow = RandGet(100)+1;
	}

	if(idData==ID_NONE)//新加数据
		return m_pPetEgg->AddNewPetData(pItem, nGrow, nNextFlush);
	else
		return m_pPetEgg->FlushPetData(idData, pItem, nGrow, nNextFlush);

	return true;
}

void CUser::SendPeteggData(OBJID idItem)
{
	CHECK(idItem);
	
	CItem* pItemEgg = GetItem(idItem, false, true);
	if(!pItemEgg)
		return;
	
	if(m_pPetEgg)
		m_pPetEgg->SendToClient(pItemEgg, PETEGG_QUERYINFO);
}

//modify code by 林德渊 begin 2011.5.30
void CUser::SetVasLvQuestionAnswer(int data,int dataid)
{
	vaslv_question_answer = data;
	vaslv_question_answerid = dataid;
}

int CUser::GetVasLvQuestionAnswer(void)
{
	return vaslv_question_answer;
}

int CUser::GetVasLvQuestionAnswerId(void)
{
	return vaslv_question_answerid;
}

void CUser::SetVasLvid(int data)
{
	vaslvid = data;
}

int  CUser::GetVasLvid(void)
{
	return vaslvid;
}
//modify code by 林德渊 end

void CUser::initCompensationManager()
{
	CHECK(!m_pCompensationManager);
	m_pCompensationManager = CCompensationManager::CreateNew();
	CHECK(m_pCompensationManager);

	if(!m_pCompensationManager->Create(Database(),PID,this))
	{
		SAFE_RELEASE(m_pCompensationManager);
	}
}

void CUser::SendCompentationInfo()
{
	CHECK(m_pCompensationManager);
	m_pCompensationManager->SendCompentationInfo();
}

void CUser::SendCompentationInfoMsg()
{
	CHECK(m_pCompensationManager);
	m_pCompensationManager->SendCompentationInfoMsg();
}

bool CUser::PickCompentation(OBJID id,bool& bOther)
{
	CHECKF(m_pCompensationManager);
	return m_pCompensationManager->PickCompentation(id,bOther);
}

bool CUser::CheckVerificationState()
{
	if(m_data.GetSynCon() < STATE_VERIFICATION_BEGIN*10 || m_data.GetSynCon() >= STATE_VERIFICATION_END*10) 
	{
		if(m_data.GetSynCon() >= STATE_VERIFICATION_END*10)
			LOGERROR("CheckVerificationState() oldval=%u,userid=%u",m_data.GetSynCon(),GetID());
		m_data.SetSynCon(STATE_VERIFICATION_NORMAL*10);
	}
	return m_pVerification->CheckVerificationState();
}

void CUser::VerificationGivepupTask()
{
	SetVerificationNeedChk(false);
	if (QueryStatus(STATUS_VERIFICATION_RUNNING))
		DetachStatus(QueryRole(),STATUS_VERIFICATION_RUNNING);
	if (QueryStatus(STATUS_VERIFICATION_WAIT))
		DetachStatus(QueryRole(),STATUS_VERIFICATION_WAIT);
	SetVerificationState(STATE_VERIFICATION_NORMAL);
	SetVerificationTimes(0);
	SetVerificationCoding(false);
}
int CUser::GetVerificationState()
{
	int nState=0;
	nState=m_data.GetSynCon()/10;
	if(nState<=0)
	{
		LOGERROR("GetVerificationState() nState=%u,userid=%u",nState,GetID());
		nState=STATE_VERIFICATION_NORMAL;
	}
	return nState;
}
int CUser::GetVerificationTimes()
{
	return m_data.GetSynCon()%10;
}
void CUser::SetVerificationState(int nState,bool bUpdate)
{
	if(nState<=0)
	{
		LOGERROR("SetVerificationState() nState=%u,userid=%u",nState,GetID());
		nState=STATE_VERIFICATION_NORMAL;
	}
	m_data.SetSynCon(nState*10+GetVerificationTimes());
}
void CUser::SetVerificationTimes(int nTimes,bool bUpdate)
{
	m_data.SetSynCon(GetVerificationState()*10+nTimes);
}
void CUser::AddVerificationTimes()
{
	m_data.SetSynCon(GetVerificationState()*10+GetVerificationTimes()+1);
}

//modify code by 林德渊 begin 2011.6.22
void  CUser::SetNewValue(int new_value)
{
	newvalue = new_value;
}
int  CUser::GetNewValue(void)
{
	return newvalue;
}
void CUser::SetNewValueTrageid(OBJID newtrageid)
{
	newvaluetrageid = newtrageid;
}
OBJID CUser::GetNewValueTrageid(void)
{
	return newvaluetrageid;
}
//modify code by 林德渊 end
bool CUser::EquipSoulAdd(OBJID idWeapon, OBJID nCount,bool bUseBook)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return false;
	}
	//获得装备
	CItem* pEquip = GetItem(idWeapon,false);
	if (!pEquip || !pEquip->IsCanUpLevel())
		return false;
	if (pEquip->GetTypeID()==144000)//湛卢剑
		return false;
	if (pEquip->IsWeaponSoul())
		return false;
	if (pEquip->IsLocked())
		return false;


	bool bUp=false;
	if (bUseBook)//使用卷轴
	{
		CItem* pBook=GetItem(nCount,false);
		if (!pBook)
		{
			SendSysMsg("你没有铸灵卷轴");
			return false;
		}
		else
		{
			DEBUG_TRY
			if ((pEquip->IsActEquipment() && !pBook->IsActSoulBook()) || (pEquip->IsDefEquipment() && !pBook->IsDefSoulBook()))
			{
				SendSysMsg("卷轴与装备不符");
				return false;
			}
			int nBookType=pBook->GetSoulBookType();
			if (nBookType==pEquip->GetEquipSoulType())
			{
				SendSysMsg("武器与卷轴属性相同不能铸灵");
				return false;

			}
			int nBind=pBook->GetKidnap();

			if (!SpendItem(pBook))//扣卷轴
				return false;
			
			if (pEquip->GetEquipSoulLev()==0)
				pEquip->SetEquipSoulLev(1);//从无到有
			
			pEquip->SetEquipSoulType(nBookType);
			
 			if (nBind==1 && !pEquip->GetKidnap())
				pEquip->SetKidnap(1);
			DEBUG_CATCH("EquipSoulAdd() usebook");
		}
	}
	else//不使用
	{
		if (pEquip->GetEquipSoulLev() <= 0 || pEquip->GetEquipSoulLev() >= 10)
		{
			if (pEquip->GetEquipSoulLev() == 0)
				SendSysMsg("此装备还没附魔,请使用卷轴附魔");
			else
				SendSysMsg("此装备已经是最高级");
			return false;
		}
		int nSpendNum=nCount;
		bool bBind=false;
		if(!DelItemMulti(610061,bBind,nCount))
		{
			SendSysMsg("魔灵不足!");
			return false;
		}
		DEBUG_TRY
		if (bBind && !pEquip->GetKidnap())
			pEquip->SetKidnap(1);
		int nOldLev=pEquip->GetEquipSoulLev();
		int nPoints=pEquip->GetEquipSoulPoint()+nSpendNum;
		int newlev=GetEquipSoulLevByPoint(nPoints,nOldLev);
		if (newlev>nOldLev)
		{
			pEquip->SetEquipSoulLev(newlev);
			bUp=true;
		}
		if (newlev==10)
			pEquip->SetEquipSoulPoint(0);
		else
			pEquip->SetEquipSoulPoint(nPoints);
		::MyLogSave(VALUABLESLOG_FILENAME,"%u(%s) use %d 610061 to make %u from %u %u to %u %u",GetID(),GetName(),nSpendNum,pEquip->GetTypeID(),nOldLev,nPoints-nSpendNum,newlev,pEquip->GetEquipSoulPoint());
		DEBUG_CATCH("EquipSoulAdd() not usebook");
	}
	if (bUp)
	{
		if (pEquip->GetEquipSoulLev()==10)
		{
			char szMsg[255];
			char UserName[128] = "";
			char itemName[128] = "";
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			sprintf(szMsg, "<%s>踏入江湖后奇遇连连，汇集无数魔灵后将<%s>的铸灵等级提升到了<3_%d_6>级，在炼器史上冠绝古今，试问天下还有谁能及！",
				UserName, itemName ,pEquip->GetEquipSoulLev());
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);

		}
		if (pEquip->GetEquipSoulLev()>3 && pEquip->GetEquipSoulLev()<10 )
		{
			char szMsg[255];
			char UserName[128] = "";
			char itemName[128] = "";
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			sprintf(szMsg, "<%s>踏入江湖后奇遇连连，汇集无数魔灵后将<%s>的铸灵等级提升到了<3_%d_6>级，装备能力再度大幅提升！",
				UserName, itemName ,pEquip->GetEquipSoulLev());
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);
		}
	}
		SendSysMsg("装备铸灵成功");
		return true;
}
bool CUser::EquipSoulExchange(OBJID idWeapon, OBJID idExWeapon)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return false;
	}
	//获得装备
	CItem* pEquip = GetItem(idWeapon,false);
	CItem* pEquip2= GetItem(idExWeapon,false);
	
	if (!pEquip || !pEquip->IsCanUpLevel() || !pEquip2 || !pEquip2->IsCanUpLevel())
		return false;
	if (pEquip->GetTypeID()==144000 || pEquip2->GetTypeID()==144000)//湛卢剑
		return false;
	if (pEquip->IsWeaponSoul()||pEquip2->IsWeaponSoul())
		return false;
	if (pEquip->IsLocked() || pEquip2->IsLocked())
		return false;

	CItem* pNeedItem=GetItemByType(610059);//铸灵转移符
	if (!pNeedItem)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "你没有铸灵转移符");
		return false;
	}

	if (pEquip->GetItemType()!=pEquip2->GetItemType())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "装备不符");
		return false;
	}

	if (pEquip->GetEquipSoulLev() <= pEquip2->GetEquipSoulLev())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "转移目标铸灵等级必须高于被转移目标铸灵等级");
		return false;
	}

	DEBUG_TRY
	if (!SpendItem(pNeedItem))//扣取铸灵转移符
		return false;
	
	pEquip2->SetEquipSoulPoint(pEquip->GetEquipSoulPoint());
	pEquip2->SetEquipSoulType(pEquip->GetEquipSoulType());
	pEquip2->SetEquipSoulLev(pEquip->GetEquipSoulLev());

	pEquip->SetEquipSoulLev(0);
	pEquip->SetEquipSoulType(0);
	pEquip->SetEquipSoulPoint(0);

	if(pEquip->GetKidnap() && !pEquip2->GetKidnap())
	{
		pEquip2->SetKidnap(1);
	}
	DEBUG_CATCH("EquipSoulExchange()");
	
	SendSysMsg("铸灵交换成功");
	return true;
}
int CUser::GetEquipSoulDeadLine(int nType,int nLev)
{
	if (nLev>10 || nLev<0)
		return 0;

	switch (nType)
	{
	case 1:
			return EquipSoulPoints[nLev];
		break;
	case 2:
			return EquipSoulAdds1[nLev];
		break;
	case 3:
			return EquipSoulAdds2[nLev];
		break;
	default:
		return 0;
	}
}
int CUser::GetEquipSoulLevByPoint(int nPoints,int nLev)
{
	for (int i=nLev;i!=11;i++)
	{
		if (nPoints>=GetEquipSoulDeadLine(1,i))
		{
			if (i==10)
				return 10;
		//	nPoints-=GetEquipSoulDeadLine(1,i);
		}
		else
			return i;
	}
	return 0;
}
bool CUser::DelItemMulti(OBJID itemType ,bool &bBind ,int nCostNum,bool bKindnapFirst)
{
	CHECKF(nCostNum>0);
	CHECKF(itemType);
	int nItemSize = 37;
	OBJID idItemArr[37] = {0};
	int nItemCount = GetItemByTypeMulti(itemType, nItemSize, idItemArr, true);
	bBind = false;
	if (!nItemCount)
		return false;
	if (nItemCount<nCostNum)
		return false;
	for(int nkidnap=0; nkidnap<2; nkidnap++)
	{
		if (nCostNum)//优先使用绑定的
		{
			DEBUG_TRY//try
			for (int i=0; i<nItemSize; i++)
			{
				CItem* pItemTemp = GetItem(idItemArr[i],false);
				if (!pItemTemp)
					continue;
				if (pItemTemp->GetTypeID() != itemType)
					continue;
				if (bKindnapFirst)
				{
					if (!pItemTemp->GetKidnap() && nkidnap==0)//第一次循环时只找绑定的
						continue;
				}
				else
				{
					if (pItemTemp->GetKidnap() && nkidnap==0)//第一次循环时只找未绑定的
						continue;
				}
				int nAmountTemp = pItemTemp->GetItemAmount();
				if (nAmountTemp<=nCostNum)
				{
					if (pItemTemp->GetKidnap())
						bBind = true;
					ASSERT( EraseItem( idItemArr[i], SYNCHRO_TRUE ) );
					nCostNum-=nAmountTemp;
				}
				else if (nCostNum>0 && pItemTemp)
				{
					pItemTemp->SetInt(ITEMDATA_AMOUNT, pItemTemp->GetInt(ITEMDATA_AMOUNT) - nCostNum);
					nCostNum=0;
					if (pItemTemp->GetKidnap())
						bBind = true;
					CMsgItem	msg;
					if(msg.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pItemTemp->GetID(),0,pItemTemp->GetItemAmount()))
						this->SendMsg(&msg);
					break;
				}
				else if (nCostNum==0)
					break;
			}
			DEBUG_CATCH("EquipSoulAdd,EraseItem")//end try...catch
		}
	}
	return true;
}

//modify code by 林德渊 begin 2011.7.13
bool CUser::EquipArtific(OBJID idEquip)//炼化
{
	if( QuerySquareDeal() )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return false;
	}
	if( QueryBooth() )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return false;
	}
	//获得装备
	CItem* pItem = GetItem(idEquip,false);
	if( !pItem || !(pItem->GetColour() > 3) )
		return false;

	if( pItem->GetTypeID()==144000 )//湛卢剑
		return false;
	if( pItem->IsWeaponSoul() )
		return false;
	if( pItem->IsLocked() )
		return false;

	if( !( pItem->IsShoulder() || pItem->IsWeapon() || pItem->IsHelmet() || pItem->IsNecklace() ||
		   pItem->IsArmor() || pItem->IsRing() || pItem->IsBangle() || pItem->IsGlove() ||
		   pItem->IsCaestus() || pItem->IsShoes() || pItem->IsAdorn() || pItem->IsDress() ) )
		return false;

	if( m_pPackage && m_pPackage->IsPackFull(1,610012,m_pPackage->GetPositionByItemType(610012)) )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "材料背包已满");
		return false;
	}

	int aoumnt = 0;
	srand(time(NULL));//产生随机数
	int randnum = 0;
	if( pItem->GetColour() == 4 )//紫
	{
		randnum = rand()%9;
		if( pItem->GetReqLev() >= 50 && pItem->GetReqLev() <= 60 )
			aoumnt = 8 + randnum;
		else if( pItem->GetReqLev() >= 61 && pItem->GetReqLev() <= 70 )
			aoumnt = 10 + randnum;
		else if( pItem->GetReqLev() >= 71 && pItem->GetReqLev() <= 80 )
			aoumnt = 12 + randnum;
		else if( pItem->GetReqLev() >= 81 && pItem->GetReqLev() <= 90 )
			aoumnt = 14 + randnum;
		else if( pItem->GetReqLev() >= 91 && pItem->GetReqLev() <= 100 )
			aoumnt = 16 + randnum;
	}
	else if( pItem->GetColour() == 5 )//橙
	{
		randnum = rand()%49;
		if( pItem->GetReqLev() >= 50 && pItem->GetReqLev() <= 60 )
			aoumnt = 48 + randnum;
		else if( pItem->GetReqLev() >= 61 && pItem->GetReqLev() <= 70 )
			aoumnt = 60 + randnum;
		else if( pItem->GetReqLev() >= 71 && pItem->GetReqLev() <= 80 )
			aoumnt = 72 + randnum;
		else if( pItem->GetReqLev() >= 81 && pItem->GetReqLev() <= 90 )
			aoumnt = 84 + randnum;
		else if( pItem->GetReqLev() >= 91 && pItem->GetReqLev() <= 100 )
			aoumnt = 96 + randnum;
	}

	if( 0 == aoumnt )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "装备等级或品质不够");
		return false;
	}

	int kindnap = pItem->GetKidnap();
	if(pItem->IsCostlyItem())//贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) EquipArtific %d %u",GetID(),GetName(),1,pItem->GetTypeID());

	//删除装备同时给客户端发消息
	if( !EraseItem(idEquip,true) )
		return false;
	
	//给物品
	if( kindnap > 0 )
	{
		if( !AwardItemCount(610061,aoumnt,true,0,1,0,0) )
		{//失败做记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) NOT GET %d %u kindnap",GetID(),GetName(),aoumnt,610061);
		}
	}
	else
	{
		if( !AwardItemCount(610061,aoumnt,true,0,0,0,0) )
		{//失败做记录
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) NOT GET %d %u",GetID(),GetName(),aoumnt,610061);
		}
	}
	return true;
}

bool CUser::EquipMove(OBJID idEquip,OBJID idCharm,int &firstaddlv,int &secondaddlv)//强化转移
{
	if( QuerySquareDeal() )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return false;
	}
	if( QueryBooth() )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return false;
	}
	//获得装备
	CItem* pItem = GetItem(idEquip,false);
	if( !pItem )
		return false;

	if( pItem->GetTypeID()==144000 )//湛卢剑
		return false;
	if( pItem->IsWeaponSoul() )
		return false;
	if( pItem->IsLocked() )
		return false;

	CItem* pGem = GetItem(idCharm,false);
	if( !pGem )
		return false;
	
	if( pGem->GetTypeID()==144000 )//湛卢剑
		return false;
	if( pGem->IsWeaponSoul() )
		return false;
	if( pGem->IsLocked() )
		return false;

	//判断是否同类型装备
	if( !( (pItem->IsShoulder() && pGem->IsShoulder()) ||
		(pItem->IsWeapon() && pGem->IsWeapon()) ||
		(pItem->IsHelmet() && pGem->IsHelmet()) ||
		(pItem->IsNecklace() && pGem->IsNecklace()) ||
		(pItem->IsArmor() && pGem->IsArmor()) ||
		(pItem->IsRing() && pGem->IsRing()) ||
		(pItem->IsBangle() && pGem->IsBangle()) ||
		(pItem->IsGlove() && pGem->IsGlove()) ||
		(pItem->IsCaestus() && pGem->IsCaestus()) ||
		(pItem->IsShoes() && pGem->IsShoes()) ||
		(pItem->IsAdorn() && pGem->IsAdorn()) ||
		(pItem->IsDress() && pGem->IsDress()) ) )
		return false;

	int nfirstLev = pItem->GetEquipAddLev();
	int nsecondLev = pGem->GetEquipAddLev();
	if( (nfirstLev < 1) || (nsecondLev < nsecondLev) )
		return false;
	
	//删除挪移护符
	bool bHasKind = false;
	if (!DelItemByTypeMulti(610060,1,bHasKind, false))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "没有足够的挪移护符");
		return false;
	}

	if(pItem->GetKidnap() && !pGem->GetKidnap())
		pGem->SetKidnap(1);

	pItem->SetEquipAddLev(0);
	if( pItem->IsSoulMark() )
	{
		float nRate = pItem->GetEquipAddRate(0);
		float nLastRate=1;
		if( nfirstLev > 1 )//如果是1级以上的要先还原成0级的属性再乘以系数
			nLastRate = pItem->GetEquipAddRate(nfirstLev);
		
		//如果不是魂印后更新属性，必须还原成魂印前的属性 魂印 +10%
		nRate += 0.1f;
		if (nRate==0.1f)
			nRate=1.1f;
		nLastRate += 0.1f;
		
		int nSort1 = pItem->GetInt( ITEMDATA_BASE1 ) / 10000;
		int nSort2 = pItem->GetInt( ITEMDATA_BASE2 ) / 10000;
		int nOldBase1 = pItem->GetInt( ITEMDATA_BASE1 )%10000*1.0/nLastRate+0.5;//四舍五入，减小误差
		int nOldBase2 = pItem->GetInt( ITEMDATA_BASE2 )%10000*1.0/nLastRate+0.5;
		
		pItem->SetInt(ITEMDATA_BASE1, nSort1*10000+nOldBase1*nRate+0.5);
		pItem->SetInt(ITEMDATA_BASE2, nSort2*10000+nOldBase2*nRate+0.5);
	}
	else
	{
		float nRate = pItem->GetEquipAddRate(0);
		float nLastRate=1;
		if( nfirstLev > 1 )//如果是1级以上的要先还原成0级的属性再乘以系数
			nLastRate = pItem->GetEquipAddRate(nfirstLev);
		
		int nSort1 = pItem->GetInt( ITEMDATA_BASE1 ) / 10000;
		int nSort2 = pItem->GetInt( ITEMDATA_BASE2 ) / 10000;
		int nOldBase1 = pItem->GetInt( ITEMDATA_BASE1 )%10000*1.0/nLastRate+0.5;//四舍五入，减小误差
		int nOldBase2 = pItem->GetInt( ITEMDATA_BASE2 )%10000*1.0/nLastRate+0.5;
		
		pItem->SetInt(ITEMDATA_BASE1, nSort1*10000+nOldBase1+0.5);
		pItem->SetInt(ITEMDATA_BASE2, nSort2*10000+nOldBase2+0.5);
	}
//	pItem->UpdateEquipBaseProperty(nfirstLev);
	for( int i = nsecondLev + 1; i <= nfirstLev; i++ )
	{
		if( i <= 0 || i >10 )
			break;
		pGem->SetEquipAddLev(i);
		pGem->UpdateEquipBaseProperty(i - 1);
	}
	
	firstaddlv = 0;
	secondaddlv = nfirstLev;

	return true;
}
//modify code by 林德渊 end

//打特殊孔－－混沌之孔
BOOL CUser::EquipMakeChaosHole(OBJID idEquip, OBJID idStone)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return FALSE;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return FALSE;
	}
	DEBUG_TRY
	CItem* pEquipItem = GetItem( idEquip ,true);
	if(!pEquipItem)
		pEquipItem = GetEquipItem(idEquip);
	if(!pEquipItem)
		return FALSE;

	if(pEquipItem->GetInt(ITEMDATA_GEM4)!=0)
		return FALSE;
	if(!pEquipItem->IsWeaponSoul())
	{
		if(!pEquipItem->IsCanUpLevel() || pEquipItem->IsLocked())
			return FALSE;
	}

	if (pEquipItem->GetTypeID()==144000)//湛卢剑
		return FALSE;

	CItem* pStone = GetItem(idStone,false);
	if (!pStone || pStone->GetTypeID()!=610057 || pStone->IsLocked())//虚空破碎针（610057）
		return FALSE;

	BOOL bIsCostly = pStone->IsCostlyItem();
	int nStoneType = pStone->GetTypeID();
	int nHoleCount = pEquipItem->GetEmbedCountLimit();
	if (nHoleCount != 3)//必须是打了三个孔的装备才可以打第四个孔
		return FALSE;

	int nCostMoney = 20000;//2J
	if (GetMoney() < nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return FALSE;
	}

	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//扣除银两
		return FALSE;

	//道具是绑定的，使用后装备也要变成绑定
	bool bBind = false;
	if (pStone->GetKidnap())
		bBind = true;

	if (!SpendItem(pStone))
		return FALSE;

	pEquipItem->SetInt(ITEMDATA_GEM4, _HOLE, false);

	if (bBind && !pEquipItem->GetKidnap())
		pEquipItem->SetKidnap(1);

	SendSysMsg("恭喜你，打孔成功");

	if (bIsCostly)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, nStoneType);

	//公告
	char szMsg[255] = "";
	char UserName[128] = "";
	char itemName[128] = "";
	sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
	sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquipItem->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), idEquip, pEquipItem->GetTypeID(), GetID(), pEquipItem->GetKidnap(), pEquipItem->GetColour());

	sprintf(szMsg, "天地气运十斗，<%s>独占八斗，竟然将<%s>的混沌之孔发掘了出来，实在是百年难得一见的武林奇才！", UserName, itemName);
	UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquipItem->GetID(), pEquipItem->GetTypeID(), NULL, ALLUSERS_NAME);

	return nHoleCount+1;

	DEBUG_CATCH( "CUser::EquipMakeChaosHole" );
	return FALSE;
}

bool CUser::EmbedGemInChaos( OBJID idEquip, OBJID idStone, OBJID idGem )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "交易中不能进行此操作");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能进行此操作");
		return false;
	}
	//获得装备
	CItem* pEquip = GetItem( idEquip ,true);
	if(!pEquip)
		pEquip = GetEquipItem(idEquip);
	if(!pEquip)
		return false;

	if(!pEquip->IsCanEmbedEquip() || pEquip->IsLocked())
		return false;

	if (pEquip->GetTypeID()==144000)//湛卢剑
		return false;
	//获得宝石
	CItem* pGem	= GetItem( idGem,false );
	if(!pGem || !pGem->IsGem() || pGem->IsLocked())
		return false;

	//镶嵌石
	CItem* pStone = GetItem(idStone,false);
	if (!pStone || pStone->GetTypeID()!=610018 || pStone->IsLocked())//"补天符" ID610058 又改回镶嵌符了....
		return false;

	if(pEquip->GetInt(ITEMDATA_GEM4) != _HOLE)
		return false;

	//攻击类的装备只能镶嵌攻击类的宝石，防御类的装备只能镶嵌防御类的宝石
	if (!pEquip->IsRightGemType(pGem->GetTypeID()))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "宝石种类与装备种类不符");
		return false;
	}
	
	int nNeedMoney = COMP_EMBEDGEM_COST;
	if (GetMoney() < nNeedMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
		return false;
	}
	
	//扣钱
	if (!SpendMoney(nNeedMoney, SYNCHRO_TRUE))
		return false;

	bool bKindnap = false;
	if (!pEquip->GetKidnap() && pGem->GetKidnap())//如果宝石是绑定的，将镶嵌后的装备也绑定
		bKindnap = true;

	int nGemLev = pGem->GetTypeID()%100;
	int nGemType = pGem->GetTypeID();
	BOOL bIsCostly = pGem->IsCostlyItem();
	BOOL bIsCostStone = pStone->IsCostlyItem();
	int nStoneType = pStone->GetTypeID();
	
	if (!SpendItem(pGem) || !SpendItem(pStone))
		return false;

	pEquip->SetInt(ITEMDATA_GEM4, nGemType);

	if (bKindnap)//如果宝石是绑定的，将镶嵌后的装备也绑定
		pEquip->SetKidnap(1);

	int nPosEquip = pEquip->GetPosition();
	if(nPosEquip >= ITEMPOSITION_EQUIPBEGIN && nPosEquip < ITEMPOSITION_EQUIPEND)
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);

	CMsgCompound msg;//发送成功消息
	IF_OK( msg.Create( MSGCOMPOUND_EMBED_SUC, idEquip, 0 ) )
		this->SendMsg(&msg);
	SendSysMsg("镶嵌成功");

	if (nGemLev>2)
	{
		char szMsg[255] = "";
		char UserName[128] = "";
		char itemName[128] = "";
		int GemColor = ItemType()->QueryItemType(nGemType)->GetInt(ITEMTYPEDATA_COLOR_INIT);
		sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
		if(pEquip->IsWeaponSoul())
		{
			if(pEquip->GetWpSlType() == 1)
				sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阳之魄", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			else
				sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "九阴之魂", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());		
		}
		else
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());

		sprintf(szMsg, "<%s>向<%s>镶嵌了一颗<1_[%s]_0_%d_0_0_%d>，大幅提升了装备的能力!",
			UserName, itemName , ItemType()->QueryItemType(nGemType)->GetStr(ITEMTYPEDATA_NAME), nGemType, GemColor, GemColor);

		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);
		if (nGemLev>=4)
		{
			CMsgPlayer msgPlayer;
			msgPlayer.Create(this);
			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
	}
	if (bIsCostly)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), nGemType);
	if (bIsCostStone)//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), nStoneType);

	return true;
}
