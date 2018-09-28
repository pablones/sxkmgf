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
#include "EquipCreate.h"		//20061218����
#include "SKPostOffice.h"
#include "DataInspectManage.h"
#include "SettledAction.h"// ���� [1/29/2008]
#include "MapInstanceMgr.h"
#include "LoverHome.h"
#include "PortableShopManager.h"
//modify code by �ֵ�Ԩ begin 2011.5.30
#include ".\\..\\msgserver\\mapgroupkernel\\Network\\MsgVasLv.h"
//modify code by �ֵ�Ԩ end
const int	MINI_ADDLIFE_DRAG				= 100;
//const int	ADDLIFE_FIRST_PERCENT			= 40;
//const int	ADDLIFE_NEXT_PERCENT			= 20;
const int	STORAGE_NPC_ID					= 1005;		//? �����ֿ⻥ͨ
const int	_MINE_RANGE						= 1;		// �ڿ����

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

const OBJID REBORN_SCROLL = 630000;//ԭ�ظ������TYPEID
extern int g_SpecilLine;

int EquipSoulPoints[] = {0,10,30,90,270,810,2430,7290,21870,65610};//��������ħ����
int EquipSoulAdds1[] = {0,19,34,66,101,216,325,518,645,766,896};//����������������
int EquipSoulAdds2[] = {0,12,22,43,67,144,216,346,430,509,640};//�����嶨��

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

	//����ı�������
	DEBUG_TRY
	//���˺ŷ�������ȡ�û���Ԫ�����
	CMsgVAS MsgVAS;
	if (MsgVAS.Create(0, GetAccountID(), GetID(), _MSGVAS_QUERY_BALANCE))
	{
		//MsgVAS.m_pInfo->mtExtraInfo.usAction = 3;
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	//���˺ŷ�������ȡ�û��ķ�������Ϣ  
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
	//goto ��ѯ�����״̬
	if (MsgVAS.Create(3, GetAccountID(), GetID(), _MSGVAS_QUERY_ACTIVITY))
	{
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	DEBUG_CATCH("��������˺���Ϣ")
//	UpdateWeight();
	//��Ʒ����  ���Ը�����   �Ժ���츳��ʼ����������һ����ǰ
// 	UpdateCalculatedProperty();
	return true;
}

bool CUser::CreateAllItem(IDatabase* pDb)
{
	ASSERT(m_pPackage);

	int nStep = 0;
	DEBUG_TRY
	// ���뱳����Ʒ
	if (!m_pPackage->Create(this, pDb,true))
	{
		LOGWARNING("������Ʒʧ�ܣ�");
		return false;
	}

	nStep = 1;
	// ����װ����Ʒ,��ֹ�ظ�λ�ÿ����̵���Ʒ
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
						LOGWARNING("����װ����ʱ��,�Ѿ��ж�Ӧid��Ʒ������ userid = %u itemid = %u inPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());
						pRes->MoveNext();
						continue;
					}
					if(GetItem(pItem->GetID()))
					{
						pItem->ReleaseByOwner();
						LOGWARNING("����װ����ʱ��,�Ѿ��ж�Ӧid��Ʒ�ڱ��� userid = %u itemid = %u inPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());
						pRes->MoveNext();
						continue;
					}

					nStep = 3;					
					CItemPtr& refpEquip = GetEquipItemRef(nPosition);
					if(!refpEquip)				//? ��ֹ������Ʒ��POSITION��ͬ paled
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
							sprintf(err,"Found same position of user equip.The Item ID: %d,pos��Ϊ%d",pItem->GetID(),nPos );
							pItem->SetPosition( nPos );
							if(!m_pPackage->AddItem( pItem ))
							{
								DEBUG_TRY
								m_pPackage->PopItem(pItem->GetID());
								pItem->ReleaseByOwner();
								DEBUG_CATCH("�ظ���װ�����뱳��ʧ��,��ͼж�ص�ʱ�����")

								LOGWARNING("����װ����ʱ��,�Ѿ��ж�Ӧλ����װ��������δ������ʧ�� userid = %u itemid = %u inPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());
								pRes->MoveNext();
								continue;
							}
						}
						else
						{
							pItem->ReleaseByOwner();
							LOGWARNING("����װ����ʱ��,�Ѿ��ж�Ӧλ����װ������������ userid = %u itemid = %u inPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());
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
	DEBUG_CATCH3("��Ҽ�����Ʒ��userid = %u,step = %u",GetID(),nStep);
	
	DEBUG_TRY
	//���˺ŷ�������ȡ�û���Ԫ�����
	CMsgVAS MsgVAS;
	if (MsgVAS.Create(0, GetAccountID(), GetID(), _MSGVAS_QUERY_BALANCE))
	{
		//MsgVAS.m_pInfo->mtExtraInfo.usAction = 3;
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	//���˺ŷ�������ȡ�û��ķ�������Ϣ  
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
	//goto ��ѯ�����״̬
	if (MsgVAS.Create(3, GetAccountID(), GetID(), _MSGVAS_QUERY_ACTIVITY))
	{
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
	}
	//�鿴�ֻ���ֵ���״̬
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
//modify code by �ֵ�Ԩ begin 2011.5.30
	CMsgVasLv2  MsgVASLv2;
	if( MsgVASLv2.Create(ACTION_LOGINUPDATE,GetID(),0,0,0,0,0,0,0,0,GetAccountID(),0) )
		MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVASLv2 );
//modify code by �ֵ�Ԩ end

	DEBUG_CATCH("��������˺���Ϣ")
//	UpdateWeight();
	//��Ʒ����  ���Ը�����   �Ժ���츳��ʼ����������һ����ǰ
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


	//���
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
// 		//������ ��ս����
// 		CHECK(m_pUserPet);
// 		m_pUserPet->sendInfo();		
// 		if(m_idEudemonCur !=ID_NONE)
// 		{
// 			CallEudemon(m_idEudemonCur);
// 		}
// 		//���鿪��
// 		CMsgUserAttrib msgAtt;
// 		msgAtt.Create(this->GetID(), _USERATTRIB_FEEL_SWITH, GetFeelSwitch());
// 		this->BroadcastRoomMsg(&msgAtt, INCLUDE_SELF);
// 
// 		//����ݼ�
// 		m_pUserKey.SendKey(this);
// 
// 		//���������
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
// 		//��Ʒ
// 		m_pPackage->SendAllItemInfo();
// 		//�����б�
// 		SendAllMagicInfo();//�����б�
// //		InitEudemon(Database());
// 		//����
// 		sendMissionMsg();
// 		//��ҩ
// 		CMsgUserAttrib	msg;//�ƶ��ٶ��Լ�������
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
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pHelmet, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pHelmet,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pNecklace)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pNecklace, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pNecklace,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pArmor)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pArmor, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pArmor,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pWeaponR)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pWeaponR, ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pWeaponR,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 
// 	if(m_pRingR)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pRingR, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pRingR,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pRingL)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pRingL, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pRingL,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 
// 	if(m_pGlove)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pGlove, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pGlove,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pBangle)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pBangle, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pBangle,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pCaestus)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pCaestus, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pCaestus,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pShoes)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pShoes, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pShoes,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if(m_pAmmo1)
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pAmmo1, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pAmmo1,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if (m_pDress)//  [4/5/2007] ���� ��װ
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pDress, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pDress,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if (m_pWing)//  [4/5/2007] ���� ���
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		if(msg.Create(m_pWing, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pWing,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if( m_pMount )// ����  [�½��� 7/30/2007]
// 	{
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo  msg;
// //		if(msg.Create(m_pMount,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID()))
// //			pRecv->SendMsg(&msg);
// 		pRecv->SynchroItem(m_pMount,ITEMINFO_OTHERPLAYER_EQUIPMENT,GetID());
// 		//--------------------------------
// 	}
// 	if( m_pTearSprite )// ����  [�½��� 7/30/2007]
// 	{
// 		//20071214����:ͬ���޸�-----------
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

//20061010����.������ѯby TypeID.��ָ���������idItem[]
//�����ò�������ʵ�ʵ�size.�ɺ�������amount
//����-1ʱ��ʾidItem[]��ʵ��size����maxSize.��������������maxSize֮�ڵ�����
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
	else if( m_pAdornT  && m_pAdornT ->GetID() == idItem )// ���� [�½���7/31/2007]
		return m_pAdornT ;
	else if( m_pAdornB  && m_pAdornB ->GetID() == idItem )// ���� [�½���7/31/2007]
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
//20061209����.ɾ�������Ʒ.
//���ǵ�ɾ��������������Ʒ����������,������Ҫɾ�����item
//��ɾ��֮ǰ,�ȵ���GetItemByTypeMulti�õ�user�д�����Ʒ����,�������Ϸ�ʱ����false
BOOL	CUser::DelItemByTypeMulti (OBJID idItem,int nAmount,bool& bHasDelKidnapItem, bool isID, bool bIgnoreLock/*=false*/)//�¼Ӻ��԰�ȫ������
{
	CHECKF(idItem);

	//�ȼ���Ƿ���
	const int _maxItemSize = 120;
	int itemSize = _maxItemSize;//item�ĸ���,������1��
	int itemAmount = 0;
	OBJID itemId[_maxItemSize];//Ŀǰ���������120��
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

	if(itemAmount < nAmount/* && CItem::IsCountable(idItem)*/)//������
	{
		return FALSE;
	}

	//��ʼɾ��///////////////////
	int needDelAmount = nAmount;
	for(int j=0;j<itemSize;j++)
	{
		CItem *pItemTemp = GetItem(itemId[j],false);
		CHECKF(pItemTemp);

		//goto ������������Ʒ
		if (pItemTemp->IsLocked())
			continue;

		if(pItemTemp->GetTypeID() != idItem)
		{
			LOGWARNING("ɾ������ȷ����Ʒ��");
			continue;
		}
		if (!bIgnoreLock)
		{
			//���˿׵ġ����˱�ʯ�ġ�ǿ�����ġ������ǵġ��������������ϵġ���ɫ�Լ����ϵ�,��Ҫ�жϰ�ȫ��
			if (IsItemLockedOpen())
			{
				if (pItemTemp->GetEmbedCountLimit()>0 || pItemTemp->GetEmbedGemCount()>0 || pItemTemp->GetEquipAddLev()>0 || pItemTemp->GetEquipStar()>0
					|| pItemTemp->GetEquipQual()>3 || pItemTemp->GetColour()>2 || pItemTemp->IsSoulMark())
				{
					SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
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
		else if(CItem::IsCountable(idItem))//�ɵ���
		{
			int itemAmountTemp= pItemTemp->GetInt(ITEMDATA_AMOUNT);//����
			if(needDelAmount>0
				&& needDelAmount >= itemAmountTemp )
			{//��ǰ��Ʒ���������� ��ɾ������.������Ʒɾ��
				EraseItem(itemId[j],true);//ͬʱ���ͻ��˷���Ϣ
				needDelAmount-=itemAmountTemp;
			}
			else if(needDelAmount>0
				&& needDelAmount < itemAmountTemp)
			{//��ǰ��Ʒ���������� ��ɾ������.�ı䵱ǰ��Ʒamount
				SpendItem(pItemTemp, needDelAmount );
				needDelAmount=0;//�Ѿ�ɾ������
			}
		}
		else//���ɵ���
		{
			if(needDelAmount>0)
			{
				EraseItem(itemId[j],true);//ͬʱ���ͻ��˷���Ϣ
				needDelAmount--;
			}
		}
		
		if(nAmount != 0)
		{
			if(needDelAmount == 0)//ɾ��������
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
//20061209����.ɾ�������Ʒ.����
//���ǵ�Ч������,����.
//����Ҫ��ɾ��ǰ����GetItemByTypeMulti().���ⲿ���ȵ���,�ж�ͨ��֮���������ô˺���.
BOOL CUser::DelItemByTypeMulti (OBJID *itemIDArray,int size,int amountNeedToDel)
{
	CHECKF(itemIDArray && size>0 && amountNeedToDel>0);

	
	//��ʼɾ��///////////////////
	int needDelAmount = amountNeedToDel;
	for(int j=0;j<size;j++)
	{
		CItem *pItemTemp = GetItem(itemIDArray[j],false);
		CHECKF(pItemTemp);

		if(CItem::IsCountable(pItemTemp->GetTypeID()))//�ɵ���
		{
			int itemAmountTemp= pItemTemp->GetInt(ITEMDATA_AMOUNT);//����
			if(needDelAmount>0
				&& needDelAmount >= itemAmountTemp )
			{//��ǰ��Ʒ���������� ��ɾ������.������Ʒɾ��
				EraseItem(itemIDArray[j],true);//ͬʱ���ͻ��˷���Ϣ
				needDelAmount-=itemAmountTemp;
			}
			else if(needDelAmount>0
				&& needDelAmount < itemAmountTemp)
			{//��ǰ��Ʒ���������� ��ɾ������.�ı䵱ǰ��Ʒamount
				SpendItem(pItemTemp, needDelAmount );
				needDelAmount=0;//�Ѿ�ɾ������
			}
		}
		else//���ɵ���
		{
			if(needDelAmount>0)
			{
				EraseItem(itemIDArray[j],true);//ͬʱ���ͻ��˷���Ϣ
				needDelAmount--;
			}
		}
		
		if(needDelAmount == 0)//ɾ��������
			break;
		else if(needDelAmount < 0)
		{
			//ɾ�ɸ���.����
#ifdef _DEBUG
			SendSysMsg("ɾ����Ʒ����");
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
		//20071214����:ͬ���޸�-----------
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
		SendSysMsg(_TXTATR_HELP,"������[%s]",pItem->GetStr(ITEMDATA_NAME));
	else
		SendSysMsg(_TXTATR_HELP,"������[%s] x %u",pItem->GetStr(ITEMDATA_NAME),amountShow);

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


CUserPetData* CUser::PopEudemon (OBJID idEudemon, bool bSynchro, bool bUpdate)			// �������OWNER
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
CItem* CUser::PopItem (OBJID idItem, bool bSynchro, bool bUpdate)			// �������OWNER
{
	CItemPtr pItem = m_pPackage->PopItem(idItem);
	if (pItem)
	{
		pItem->SetInt(ITEMDATA_PLAYERID, ID_NONE);
		pItem->SetInt(ITEMDATA_OWNERID, ID_NONE);			// �������OWNER
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
// CUserPetData* CUser::PopEudemon(OBJID idEudemon, bool bSynchro, bool bUpdate)			// �������OWNER
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

BOOL CUser::AwardItemCount(OBJID idItemType,UINT nItemAmount, bool bChgattribute,int nJuan,int nKindnap,int nBossType,int nPerPrice)///softworms-���û�ָ����������Ʒ.2007-11-10
{
	BOOL hr=false;
	UINT nItemCountLimit=0;
	if(CItem::IsCountable(idItemType)==true)
	{
		///���Ե���
		CItemTypeData* p = ItemType()->QueryItemType( idItemType );
		CHECKF(p);
		nItemCountLimit=p->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);///�õ�������
	}
	else
	{
		///���ɵ���
		nItemCountLimit=1;
	}
// 	if(IsBackPackSpare(nItemAmount/nItemCountLimit+((nItemAmount%nItemCountLimit)>0), 0, 131010, 0)==false)	//	131010�����壬ֻ��ָ��һ��TYPEID�Ϳ���
// 		return false;

	if(idItemType==_SPCITEM_VASEXCHANGE)//Ԫ��Ʊ
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
		SendSysMsg("��������");
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
							SendSysMsg(_TXTATR_HELP,"������[%s] x %u",pNewItem->GetStr(ITEMDATA_NAME),info.nAmount);
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
			//  [2007-9-14] ���	��ú󼤻�
			if(pItem->IsActivatePickup())
			{
				//	����
				ActivateItem(pItem);
			}

			//------ʰȡ��---------//-------����
			if (pItem->IsKidnapWhenPickup())
			{
				pItem->SetKidnap(1);
			}
			//------ʰȡ��---------//-------����
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
///softworms-2007-11-08-���ϵͳ������Ʒ�����,�����������ϵͳ�ʼ�
BOOL CUser::AwardItemBySys(int nItemType, bool bSynchro, bool bIdentOK/*=*/, bool bAutoCombine/*=true*/,bool bChgattribute/*=false*/,
						   int nAmmount/*=1*/,int nSynID /*= 0*/,int nJuan,int nKindnap,int nBossType,DWORD dwPerPrice/* = 0*/,OBJID idAddItem)		// ͬʱ�������ݿ�
{
	ItemInfoStruct info;
	memset(&info,0,sizeof(ItemInfoStruct));
	CHECKF(ItemType()->GetInfo(nItemType, &info));

	//  [1/17/2008 ���]	����int nSynID = 0,PK������,����ҵ��볡��
	BOOL hr=false;
	CItemPtr pItem=NULL;
	if (m_pPackage->IsPackFull(1,nItemType,m_pPackage->GetPositionByItemType(nItemType)))
	{
		///��������,������ϵͳ�ʼ�������
// 		ItemInfoStruct info;
// 		ASSERT(ItemType()->GetInfo(nItemType, &info));
// 		info.idOwner	= GetID();
// 		info.idPlayer	= GetID();
// 		if( CItem::IsNewRemedyByItemType( nItemType ) || CItem::IsNewEudemonRemedyByItemType(nItemType))//����Ƕ�Ӧ��IsNewRemedy()���жϣ����IsNewRemedy()�޸ģ�����ոģ�
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
// 		//  [2007-8-10] ��� �����ħ��оƬ nAmount ��ʼΪ0
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
// 			else if( IsMountFluctuate( nItemType ) )// �ı��������� [�½���8/17/2007]
// 			{
// 				MountChgAttribute( &info );
// 			}
// 		}
// 		//------ʰȡ��---------//-------���---2007/05/09-----	
// 		if( IsPickUpKidnap( nItemType )  || nJuan)
// 		{
// 			info.nKidnap = 1;
// 		}
// 	//------------------------------------------------------
// 		//[��;���� 2008/9/18]������������������ָ�Ϊ����Ӧ�ֶ����.
// 		if(bChgattribute)//20070707����:��¼������
// 		{
// 			SafeCopy(info.szName,GetName(),_MAX_NAMESIZE);		//�����������
// 		}
// 		else
// 		{
// 			SafeCopy(info.szName, "", _MAX_NAMESIZE);//[��;���� 2008/9/18]�������ֶ�����Ϊ��
// 		}
// 
// 		if(bAutoCombine && CItem::IsCountable(nItemType)) ////[��;���� 2009.07.16]������Ժϲ�
// 		{
// 			if(/*!CItem::IsNeedIdent(info.ucIdent) && */nAmmount < info.nAmountLimit)
// 			{
// 				
// 				//[��;���� 2009.04.25]
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
// 					//[��;���� 2009.04.25]��ϲ����������һ��
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
// 			GameDataDefault()->GetWeaponSoulData(),//20071113����:
// 			true,
// 			info.id))
// 		{
// 			pItem->ReleaseByOwner();
// 			ASSERT(!"Create");
// 			this->SendSysMsg(g_objStrRes.GetStr(12049));//STR_FAILED_GENERATE_ITEM
// 			return NULL;
// 		}
// 		//////////////////////////////////////////////////////////////////////////
// 		//  [2007-9-14] ���	��������
// 		if(pItem)
// 		{
// 			//  [2007-9-14] ���	��ú󼤻�
// 			if(pItem->IsActivatePickup())
// 			{
// 				//	����
// 				ActivateItem(pItem);
// 			}	
// 		}
// 		if(CItem::IsCountable(pItem->GetTypeID()))
// 		{
// 			if(nAmmount > 1)//�������1,��ô�Ǹ�����Ʒ����
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
// 				int nAmmountLimt = 	p->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);///�õ�������
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
// 		//  [1/17/2008 ���]
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
		///����û��,����������		
		pItem=AwardItem(nItemType,bSynchro,bIdentOK,bAutoCombine,bChgattribute,nAmmount,bChgattribute,true,false,nKindnap,nBossType,dwPerPrice);
// 		if(pItem)
// 			UpdateMissionMsgByItemType(pItem->GetTypeID());
		//  [1/17/2008 ���]
//		if(pItem != NULL /*&& nSynID != ID_NONE*/)
//		{
			//IgotAnOrangeWeapen(nItemType, pItem); �������ﲻǡ�� ����
			//pItem->SetInt64(ITEMDATA_EXP, nSynID, true);
//		}		
		
		hr=pItem!=NULL;
	}
	return hr;
}
////////////////////////////////////////////////////��һ��װ��ʱ������
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

	if (640009 < ItemTypeId && 640413 > ItemTypeId && GetInstenceType() >= 2020 && GetInstenceType() <= 2025)//���е���
	{
		if (pItem->GetColour() > 2)
		{
			RandNum = RandGet(3);
			char szMsg[255]		   ;
			if (1 == RandNum)
				sprintf(szMsg, "<%s>����һ����ս�������ɳ����о�Ȼ�õ���<%s>�������亱�еı���������˾�̾��",UserName, itemName);
			else if(2== RandNum)
				sprintf(szMsg, "<%s>ʹ��������һ�ټ�����������ͽ����ǰһ�����Ӳӵı��䣬��һ����Ȼ��<%s>��",UserName, itemName);
			else 
				sprintf(szMsg, "<%s>�����Ŀ�����ǰ������ͽ�ı������������þò���ƽ��������������Ѿõ�<%s>�������������ţ�",UserName, itemName);
				
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		}
	}

	if ((((ItemTypeId /100) % 10) == 9) && (pItem->GetColour() == 4) &&  (2002<GetInstenceType()) && (2006>GetInstenceType()))//���ϸ�������
	{
		char szMsg[255] = "";
		sprintf(szMsg, " <%s>����һ����ս�������ں��ϸ�����ܺ���ħ�����ǽ�Ӳӵı����У���Ȼ�����亱�е�װ��<%s>��", UserName, itemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
	}
	if (ItemTypeId == 250002)//���ϸ����������
	{
		DEBUG_TRY
			char szMsg[255] = "";
		CHECK(pMonsterName);
		if(pItem->GetWpSlType() == 1)
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());
		else
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());		
		
		sprintf(szMsg, "���ϸ�����<3_%s_6>�Ѿ������ǵ�Ӣ��<%s>�ɵ��ˣ��Ὺʬ�巢����һ��<%s>����һ�������Ļ���˭��",pMonsterName,UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 250002");		
	}

	if (610000 <= ItemTypeId && ItemTypeId < 610012 && GetInstenceType() == 2001 || (ItemTypeId > 630007 && ItemTypeId < 630011 && GetInstenceType() == 2001))//ɽ������
	{
		char szMsg[255] = "";
		if (1 == RandNum)
			sprintf(szMsg, "<%s>һ�д���ɽ��ͷĿ���ϣ�ɽ��ͷĿ��Ķ��ӣ�������һ��<%s>��",UserName, itemName);
		else if(2== RandNum)
			sprintf(szMsg, "<%s>һ������ɽ��ͷĿ��С�����ϣ�ɽ��ͷĿ�����ͳ�һ��<%s>������ġ�",UserName, itemName);
		else 
			 sprintf(szMsg, "<%s>��ɽ��ͷĿ�����ڵأ���Ȼ����ʬ�����ѳ�һ��<%s>��",UserName, itemName);
		
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
	}
	if (ItemTypeId == 591003)//ɽ��ͷĿ����
	{
		DEBUG_TRY
			char szMsg[255] = "";
		CHECK(pMonsterName);
		sprintf(szMsg, "<%s>�ʶ�һ�ٵ�ƽ<3_ɽ��ͷĿ_6>���ϳ���������ɽ�������з�����һ��<%s>��",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 591003");		
	}

// 	if(ItemTypeId == 500204)
// 	{
// 		DEBUG_TRY
// 		char szMsg[255] = "";		
// 		sprintf(szMsg, "<%s>�����¸����ڽ���<3_��������(69,98)_6>����ȡ��ÿ��<%s>���ں���<3_��������_6>�͸����䱦��",UserName, itemName);
// 		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
// 		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 2038");
// 	}
	if(ItemTypeId == 500205)
	{
		DEBUG_TRY
		char szMsg[255] = "";		
		sprintf(szMsg, "<%s>���˲������ڽ���<3_��������(69,98)_6>����ȡ��ÿ��<%s>���ں���<3_��������_6>�͸����䱦��",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 2038");
	}
	if(ItemTypeId == 500206)
	{
		DEBUG_TRY
		char szMsg[255] = "";		
		sprintf(szMsg, "<%s>���Ǹ��գ��ڽ���<3_��������(69,98)_6>����ȡ��ÿ��<%s>���ں���<3_��������_6>�͸����䱦��",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 2038");		
	}
	if (ItemTypeId >= 2000000 && ItemTypeId <=2999999)//��Ϊ��Ըף������Ʒid������2000000�����жϴ���20WΪ��Ըף������Ʒ
	{
		DEBUG_TRY
			char szMsg[255] = "";		
		sprintf(szMsg, "<%s>�ڿ���<3_��Ե��ʹ(43,137)_6>��������Ը����Ȼ���������ڴ��Ѿõ�<%s>��������ɷ���ˡ�",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName()2000000");		
	}
	if (ItemTypeId == 591002)//�������������
	{
		DEBUG_TRY
			char szMsg[255] = "";
		CHECK(pMonsterName);
		sprintf(szMsg, "�ʼ�Χ���е�<3_���������_6>ɵ��ɵ���Ĵ���<%s>��ҫ��ȴ��<%s>������ֻ�ý���������ġ�", itemName,UserName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 591002");		
	}

	else if( ItemTypeId == 500203)
	{
		DEBUG_TRY
		char szMsg[255] = "";		
		sprintf(szMsg, "<%s>������״γ�ֵ���ӽ���<3_��������(69,98)_6>����ȡ��<%s>һ�ݡ�",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH("UserManager()->BroadCastMsgAndReceiveName() 500203");
	}
//modify code by �ֵ�Ԩ begin 2011.5.30
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
		sprintf(szMsg, "<%s>�����ں��ţ��Բ�η����̤�뽭��������ǰ��������<%s>һ����",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH2("UserManager()->BroadCastMsgAndReceiveName() %d",ItemTypeId);
	}
//modify code by �ֵ�Ԩ end
//modify code by �ֵ�Ԩ begin 2011.6.22
	else if( ItemTypeId == 630035 && (GetInstenceType() == 2016 || GetInstenceType() == 2018) )
	{
		DEBUG_TRY
			char szMsg[255] = "";
		sprintf(szMsg, "���ž��鵺��ȥ�޻أ�<%s>ȴ�ʶӷ��»������⿪�������⿪�����Ͻɻ��<%s>���貶�Ŀ������������̾��",UserName, itemName);
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
		sprintf(szMsg, "���鵺����ͨ���ȴ�վ��в���<%s>�����Ӣ����ս��<%s>���鵺�ı��أ�����Ӣ��֮�֣�",UserName, itemName);
		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pItem->GetID(), pItem->GetTypeID(), NULL, ALLUSERS_NAME);
		DEBUG_CATCH2("UserManager()->BroadCastMsgAndReceiveName() %d",ItemTypeId);
	}
//modify code by �ֵ�Ԩ end
}

bool GetGBKchar(int n,char* szTemp)
{
	CHECKF(szTemp);
	szTemp[0] = 0;
	switch(n)
	{
	case 1:
		strcpy(szTemp,"һ");
		break;
	case 2:
		strcpy(szTemp,"��");
		break;
	case 3:
		strcpy(szTemp,"��");
		break;
	case 4:
		strcpy(szTemp,"��");
		break;
	case 5:
		strcpy(szTemp,"��");
		break;
	case 6:
		strcpy(szTemp,"��");
		break;
	case 7:
		strcpy(szTemp,"��");
		break;
	case 8:
		strcpy(szTemp,"��");
		break;
	case 9:
		strcpy(szTemp,"��");
		break;
	case 10:
		strcpy(szTemp,"ʮ");
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
	case 1: //���еȼ�
		{
			CHECK(nData > 0 && nData <= 90);
			DEBUG_TRY
			char szTop1[128] = "";
			switch((nData - 1) / 30)
			{
			case 1:
				strcpy(szTop1,"�桤");
				break;
			case 2:
				strcpy(szTop1,"���ϡ�");
				break;
			}
			char szTopTemp1[128] = "";
			char szTopTemp2[128] = "";
			int nEnd = (nData - 1) % 30 + 1;
			if(nEnd < 11)
			{
				GetGBKchar(nEnd,szTopTemp1);
				sprintf(szTempName,"3_%s%s��_6",szTop1,szTopTemp1);
			}
			else if(nEnd % 10 == 0)
			{
				GetGBKchar(nEnd / 10,szTopTemp1);
				sprintf(szTempName,"3_%s%sʮ��_6",szTop1,szTopTemp1);
			}
			else
			{
				GetGBKchar(nEnd / 10,szTopTemp1);
				GetGBKchar(nEnd % 10,szTopTemp2);
				sprintf(szTempName,"3_%s%sʮ%s��_6",szTop1,szTopTemp1,szTopTemp2);
			}
			
			sprintf(szMsg, "<%s>���ϳ˾�ѧ�ڻ��ͨ����ת���������̶���۵�����ڽ�ȫ������������<%s>��",UserName, szTempName);
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
				strcpy(szTempName,"3_����_6");
				break;
			case USERJINMEITYPE_YINGQIAO:
				strcpy(szTempName,"3_����_6");
				break;
			case USERJINMEITYPE_YANGWEI:
				strcpy(szTempName,"3_��ά_6");
				break;
			case USERJINMEITYPE_YINGWEI:
				strcpy(szTempName,"3_��ά_6");
				break;
			case USERJINMEITYPE_DAIMEI:
				strcpy(szTempName,"3_����_6");
				break;
			case USERJINMEITYPE_CHONGMEI:
				strcpy(szTempName,"3_����_6");
				break;
			case USERJINMEITYPE_DUMEI:
				strcpy(szTempName,"3_����_6");
				break;
			case USERJINMEITYPE_RENMEI:
				strcpy(szTempName,"3_����_6");
				break;
			}
			
			sprintf(szMsg, "<%s>�չ�ǿ�����������ڽ�<%s>ǿ����<3_%d_6>��",UserName, szTempName,nData);
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
		sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());
	else
		sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pItem->GetID(), pItem->GetTypeID(), this->GetID(), pItem->GetKidnap(), pItem->GetColour());
	switch(nMsgtype)
	{
	case WEAPENSOULCAST_GROW:
		{
			char szGrow[64] = "";
			if(WPSLData.grow >= 500 && WPSLData.grow < 650)
				strcpy(szGrow,"3_��ͨ_6");
			else if(WPSLData.grow >= 650 && WPSLData.grow < 750)
				strcpy(szGrow,"3_����_6");
			else if(WPSLData.grow >= 750 && WPSLData.grow < 850)
				strcpy(szGrow,"3_�ܳ�_6");
			else if(WPSLData.grow >= 850 && WPSLData.grow < 950)
				strcpy(szGrow,"3_׿Խ_6");
			else if(WPSLData.grow >= 950 && WPSLData.grow < 1000)
				strcpy(szGrow,"3_����_6");
			else
				return;
			
			int nRand = RandGet(2);
			if(nRand == 0)
				sprintf(szMsg, "<%s>����һ���������ɹ���<%s>�ĳɳ���������<%s>�������ۺ��������������",UserName, itemName,szGrow);
			else
				sprintf(szMsg, "<%s>��ϸ���������е������飬һ��С�ľ�Ȼ������Я����<%s>������<%s>�ɳ��ʡ�",UserName, itemName,szGrow);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_ADDTIONOPEN:
		{
			sprintf(szMsg, "<%s>ʹ��<3_���ļ�_6>��<%s>����չ���Բ����ӵ�<3_%d��_6>��",UserName, itemName,nData);
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
			sprintf(szAddtion,"3_%s(%u��)_6",pType->GetStr(MAGICTYPEDATA_NAME),idSkill % 1000);
			sprintf(szMsg, "<%s>ͨ��Ŭ��ʹ<%s>ѧ������չ����<%s>���𽥰����˲������С�",UserName, itemName,szAddtion);
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
				sprintf(szMsg, "<%s>������и��Ŭ����<%s>��<%s>������<3_%u��_6>��������������������",UserName, itemName,szAddtion,idSkill % 1000);
			else
				sprintf(szMsg, "����<%s>�ɹ���<%s>��<%s>������<3_%u��_6>����������һ����Ʒ���ǡ�",UserName, itemName,szAddtion,idSkill % 1000);
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
					strcpy(szType,"��");
					break;
				case 2:
					strcpy(szType,"ˮ");
					break;
				case 3:
					strcpy(szType,"��");
					break;
				case 4:
					strcpy(szType,"��");
					break;
				default:
					return;
				}
				sprintf(szMsg, "<%s>�ѵ��˼�ǧ�꣬�������ڽ�<%s>�ĺϳɵȼ�������<3_%u��_6>���������<3_%s����_6>��",UserName, itemName,nData,szType);
			}
			else
				sprintf(szMsg, "<%s>����ǧ����࣬���ڽ�<%s>�ĺϳɵȼ�������<3_%u��_6>���Ӵ˲������ĵ����������ˡ�",UserName, itemName,nData);
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
			sprintf(szSkill,"3_%s(%u��)_6",pType->GetStr(MAGICTYPEDATA_NAME),idSkill % 1000);
			sprintf(szMsg, "<%s>��<%s>��Ȼƽ���޹�������<%s>���������еĸ������ס�",UserName, itemName,szSkill);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_SKILLRAND:
		{
			sprintf(szMsg, "<%s>ʹ��<3_ϴ���_6>��ϴ��<%s>�����м��ܡ�",UserName, itemName);
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
				sprintf(szMsg, "<%s>����ǧ����࣬ͷ��������һ�룬���ڽ�<%s>��<%s>����������<3_%u��_6>��",UserName, itemName,szSkill,idSkill % 1000);
			else
				sprintf(szMsg, "��������·��<%s>���������˵��£����ǽ�<%s>��<%s>����������<3_%u��_6>��",UserName, itemName,szSkill,idSkill % 1000);
			bSet = true;
		}
		break;
	case WEAPENSOULCAST_TYPE:
		{
			char szType[64] = "";
			switch(WPSLData.nType % 10)
			{
			case 1:
				strcpy(szType,"��");
				break;
			case 2:
				strcpy(szType,"ˮ");
				break;
			case 3:
				strcpy(szType,"��");
				break;
			case 4:
				strcpy(szType,"��");
				break;
			default:
				return;
			}
			sprintf(szMsg, "<%s>ʹ��<3_�������൤_6>��<%s>������ı�Ϊ<3_%s����_6>��",UserName, itemName,szType);
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
//[��;���� 2008/9/18]����BOOL bSaveCreater =true����,����ѡ���Ƿ�Ҫ��¼����������.
CItem* CUser::AwardItem(int nItemType, bool bSynchro, bool bIdentOK/*=false*/, bool bAutoCombine/*=true*/,bool bChgattribute/*=false*/,int nAmmount/*=1*/, BOOL bSaveCreater/*=true*/,BOOL bInsert/*=true*/,BOOL bMarry/* = false*/, DWORD nKidnap,int nBossType/* = 0*/,DWORD dwPerPrice/* = 0*/)		// ͬʱ�������ݿ�
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
			//�¼ӵĿɵ��ӵĴ�����
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
	if (bChgattribute)//�����Ҫ�ı�����
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
						BOOL bSaveCreater/*=true*/,BOOL bInsert/*=true*/,BOOL bRoleCreate /*= false*/,BOOL bCreateEudemon/* = false*/,int nBossType/* = 0*/)		// ͬʱ�������ݿ�
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
							SendSysMsg(_TXTATR_HELP,"������[%s]",pNewItem->GetStr(ITEMDATA_NAME));
						else
							SendSysMsg(_TXTATR_HELP,"������[%s] x %u",pNewItem->GetStr(ITEMDATA_NAME),pInfo->nAmount);
					}
					return pNewItem;
				}
			}
		}
	}

	//�־ü���
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


	//[��;���� 2009.04.25]����������
	//------ʰȡ��---------//-------���---2007/05/09-----	
	//if( IsPickUpKidnap( nItemType ) )
	//{
	//	pInfo->nKidnap = 1;
	//}
//------------------------------------------------------
	//[��;���� 2008/9/18]����¼����������ʱ����Ӧ�ֶ�����Ϊ��
	//�����ֵ�޸Ŀ�ʼ
	if(bSaveCreater)//20070707����:��¼������
	{
		SafeCopy(pInfo->szName,GetName(),_MAX_NAMESIZE);//�����������
	}	
	//[2009.4.17--�Ŷ�]��¼���ͼ����Ʒ������
	else if (bRoleCreate)
	{
		
	}
	else
	{
		SafeCopy(pInfo->szName,"",_MAX_NAMESIZE);
	}

	//�����ֵ�޸Ľ���
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
	//  [2007-9-14] ���	��������
	if(pItem)
	{
		pItem->SetInt(ITEMDATA_BUYPRICE,pInfo->dwPerPrice);
		//  [2007-9-14] ���	��ú󼤻�
		if(pItem->IsActivatePickup())
		{
			//	����
			ActivateItem(pItem);
		}

		//------ʰȡ��---------//-------����
		if (pItem->IsKidnapWhenPickup())
		{
			pItem->SetKidnap(1);
		}
		//------ʰȡ��---------//-------����
	}


	// end  [2007-9-14] ��� ��������
	//////////////////////////////////////////////////////////////////////////
	
	if (!AddItem(pItem, bSynchro))
	{
// 		if(IETMSORT_EUDEMON == CItem::GetItemSort(pInfo->idType))
// 		{
// 			m_pUserPet->DeleteData(pItem->GetInt(ITEMDATA_BASE1));
// 		}
		LOGWARNING("�����Ʒʧ�ܣ��޷����뱳����userid = %u itemtype = %u",GetID(),pItem->GetTypeID());
		pItem->DeleteRecord(Database());
		pItem->ReleaseByOwner();
		return NULL;
	}

	int  amountShow = 1;
	if(pItem->IsCountable(pItem->GetTypeID()))
	{
		amountShow = pItem->GetItemAmount();
	}
// 	SendSysMsg(_TXTATR_SYSTEM,"������[%s] x %u",pItem->GetStr(ITEMDATA_NAME),amountShow);
// 	UpdateMissionMsgByItemType(pItem->GetTypeID());

	if(pItem->IsPetEgg())//���ﵰ
	{
		if(m_pPetEgg)
			m_pPetEgg->FlushItemAllPetData(pItem);
	}

	return pItem;
}

//////////////////////////////////////////////////////////////////////
bool CUser::EraseItem(OBJID idItem, bool bSynchro, bool bDelEudemon/*=true*/,bool bIsPlayerKill /*= false*/,bool bComBin)		// ͬʱ�������ݿ�
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
	SendMsg(&msgAtt);//������һ�û��canbesee�����Ըĳɲ��㲥�Լ����������Լ���
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
		pUser->SendSysMsg("���ﱻ����������");
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
bool CUser::EraseEquip(int nPosition, bool bSynchro)		// ͬʱ�������ݿ�
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
				|| nPosition == ITEMPOSITION_DRESS //2007829����:��װ
				|| nPosition == ITEMPOSITION_MOUNT //2007829����:����
				)
		{
			CMsgPlayer msgPlayer;
			if (msgPlayer.Create(this->QueryRole()))
				this->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
		else if(nPosition == ITEMPOSITION_WEAPONSOUL)
		{
			//���
			CMsgWeapensoulShow msgwpsl;
			if(msgwpsl.Create(wpslInfo,EM_MSGWPSLSHOW_OUT,GetID(),idItem))
				this->BroadcastRoomMsg(&msgwpsl, INCLUDE_SELF);

		}

		//  [2007-10-10] ��� ��CUser::UnEquipItem�п�������,������BUF
// 		if( nPosition == ITEMPOSITION_MOUNT )
// 		{
// 			CRole::DetachStatus((IRole*)this,STATUS_MOUNTSPEED_NUMBER);
// 			CMsgPlayer msgPlayer;// ������Ϣ���ͻ��˸��� [�½���8/25/2007]
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
		LOGWARNING("װ�������װ����");
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
// 					CMsgPlayer msgPlayer;// ������Ϣ���ͻ��˸��� [�½���8/25/2007]
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
	case	ITEMPOSITION_DRESS:	//��װ
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
			LOGWARNING("����ȥ��װ��λ�ò���position = %u userid = %u itemid = %u",nPosition,GetID(),pItem->GetID());
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
					LOGWARNING("��������װ��,���ϻ���position = %u userid = %u itemid = %u",bagPosition,GetID(),pItemAddToBag->GetTypeID());

				if ((bagPosition < ITEMPOSITION_PACK_BEGIN) && (bagPosition >= ITEMPOSITION_PACK_END))
				{
					LOGWARNING("��������װ��λ�ò���position = %u userid = %u itemid = %u",bagPosition,GetID(),pItemAddToBag->GetID());
				}
				DEBUG_CATCH3("�Ա���������װ�����������⣬position = %u userid = %u",nPosition,GetID())
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
				//���
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
// 			}  // �߻���C����_20062008012802_����sk_settledaction��type ���� [1/29/2008]
// 		}
		return true;
	}
	else
	{	
		nstep = 23;
		LOGWARNING("��װ��װ����ȷ��λ�� itemid=%u,position=%u,userid=%u",pItem->GetID(),nPosition,GetID());
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
		LogSave("ʹ��ǰ������װ����ͬ����Ʒuserid = %u , itemid = %u itemtype = %u",GetID(),pItem->GetID(),pItem->GetTypeID());
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
// 	// PKģʽ���ж���ʾ�ɿͻ������
// 
// 	CUser* pUser = NULL;
// 	if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
// 	{
// 		if (usTarget&TARGET_ONLY)
// 			return false;
// 		// ����ʹ�ö���
// 		if ((usTarget&TARGET_SELF) && pUser->GetID() != this->GetID()
// 			|| (usTarget&TARGET_OTHERS) && pUser->GetID() == this->GetID())
// 			return false;
// 
// 		//�������в���ʹ�ûػ��
// // 		if(IsInArena() && pItem->GetTypeID()==750040)
// // 		{
// // 			return false;
// // 		}
// 
// 		// �Ƿ��ʬ��ʹ�õ�����
// 		if (((usTarget&TARGET_BODY) && pUser->IsAlive())
// 			|| (!(usTarget&TARGET_BODY) && !pUser->IsAlive()))
// 		{
// 			CMsgItem msg;
// 			IF_OK( msg.Create( pItem->GetID(),ITEMACT_SYNCHRO_UNLOCK ) )
// 				SendMsg( &msg );// �½������,�������Ʒ���� [5/25/2007]
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
// 		// �ж�Ŀ������
// 		if (!((usTarget&TARGET_MONSTER) && pMonster->IsMonster())
// 			&& !((usTarget&TARGET_EUDEMON) && pMonster->IsEudemon()))
// 			return false;
// 
// 		// �ж���������
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
//return false,����ʱ�ж���,����...
bool CUser::UseItem(OBJID idItem, int nPosition, bool bSynchro)
{
	CHECKF(idItem);
	int nStep = 0;
	DEBUG_TRY
	if(!this->IsAlive())
		return false;

	nStep = 1;
	if( this->QueryStatus( STATUS_SLEEP ) || this->QueryStatus( STATUS_DREAMING ) )//ѣ�β���ʹ����Ʒ
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
		SendSysMsg(_TXTATR_TOPSHOW, "����Ʒ������,����ʹ��");
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
		else //û������,���
		{
			nStep = 8;
			pTimeOutMS = new CTimeOutMS();
			if( pTimeOutMS )
			{
				pTimeOutMS->SetInterval( nDelayTimeMS );
				m_setItemDelay[ /*nQueryIndex*/ nDelayTimeType] = pTimeOutMS;
					//ʹ�ú����update����
			}
		}	
// 		}
	}
//---------------------------------------------------end----------------------------------------
	nStep = 9;
	if((pItem->GetTypeID() / 10000 == 32 ) || (pItem->GetTypeID() / 10000 == 33)  )
	{
		if(pItem->GetTypeID() / 10000 == 32) //����
		{
			nStep = 10;
			if( pItem->IsNewRemedy() )//�ر�ĳԲ���Ĵ��ʹ��������Ǻ�
			{
				DEBUG_TRY
					DWORD dwItemType = pItem->GetTypeID();
				DWORD dwAmountLimit = 0;
				if (pItem->GetTypeID() / 1000 == 321)
					dwAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
				else if (pItem->GetTypeID() / 1000 == 322)
					dwAmountLimit = pItem->GetInt(ITEMDATA_BASE2);
				
				if (pItem->GetItemType()==32)//�����Ѫҩ
				{
					if ((GetPetAdd()+dwAmountLimit)>6000000)//���ܳ���6����
					{
						SendSysMsg(_TXTATR_TOPSHOW, "�ɻظ������������ޣ�����ʹ��");
						return false;
					}
					SetPetAdd(GetPetAdd() + dwAmountLimit);
					if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
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
			SendSysMsg(_TXTATR_TOPSHOW, "����ҩƷ����ֱ��ʹ��");
			return false;
		}
	}

	nStep = 11;
	if(pItem->GetTypeID() / 10000 == 39)
	{
		if( pItem->IsWeaponSoulExpItem() )//�ر�ĳԲ���Ĵ��ʹ��������Ǻ�
		{
			nStep = 12;
			if(FullWeaponLifespan())
			{
				ASSERT(SpendItem(pItem, 1));
				return true;
			}
			
			SendSysMsg(_TXTATR_TOPSHOW, "��ǰ��������������");
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
			LOGWARNING("���ݴ�����Ʒ[%d]type[%d]��������Ʒ,��û����д��Ӧ��action_id��", idItem, pItem->GetInt(ITEMDATA_TYPE));
		}
		
		SetTaskItemID(itemID);
		SendUnlockItemMsg(itemID);
		m_idItem = pItem->GetID();
		//goto ɾ����Ʒ���ڳ�����ִ�У���ͨ���ű����ã���߰�ȫ��
		if (GameAction()->ProcessAction(pItem->GetInt(ITEMDATA_ACTION), this, NULL, pItem, NULL, true))
		{
			pItem = GetItem(idItem,false);
			if(pItem)
			{
				bool bIsCostly = pItem->IsCostlyItem();
				int nType = pItem->GetTypeID();
				if(!SpendItem(pItem))
					::LogSave("CUser::UseItem action��Ʒʹ�ú�ɾ��ʧ��,user:%u,itemID:%u,itemType:%u", GetID(), idItem, pItem->GetTypeID());
				else
				{
					if (bIsCostly)//goto ������Ʒ��¼
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
				//-------���---2007/05/30-----
				ResetItemCD( nDelayTimeMS, typeID, pTimeOutMS );
			}


			//-----��� 2007/03/10--- 
			int nQueryIndex = CItem::GetDelayGroupIndex( pItem->GetTypeID() );
			CMsgCoolDown msg;
			if( msg.Create( this->GetID(),MAGICCOOLDOWN_BEGIN,ITEM_INFO,nQueryIndex,0, pTimeOutMS->GetInterval()) ) //����CD
			this->SendMsg( &msg );
			pTimeOutMS->Update();

		}

		if(  pItem->IsEnableMount() )// ���� [�½���7/30/2007]
		{
			nStep = 17;
			if(GetMap()->IsNoMount())
			{
			//	SendSysMsg("�õ�ͼ����ʹ������");
				this->SendSysMsg( g_objStrRes.GetStr(19101) );
				return false;
			}

			return EquipItem( pItem, nPosition, bSynchro );
		}
		nStep = 18;
		return EquipItem(pItem, nPosition, bSynchro);
	}
 	else if( pItem->IsNewRemedy() )//�ر�ĳԲ���Ĵ��ʹ��������Ǻ�
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
		if (pItem->GetItemType()==30)//�����Ѫҩ
		{
			if (nBigType==1)//��ƿ
			{
				if (GetHpAdd()!=0 && GetHpAdd()%10==2)//�Ѿ��Թ�Сƿ
				{
					SendSysMsg(_TXTATR_TOPSHOW, "���Ѿ�ӵ�и�ǿ��ı���ظ�״̬");
					return false;
				}
				if ((GetHpAdd()/10+dwAmountLimit)>6000000)//���ܳ���6����
				{
					SendSysMsg(_TXTATR_TOPSHOW, "�ɻظ������������ޣ�����ʹ��");
					return false;
				}
			}
			if (nBigType==2)//Сƿ
			{
				if (GetHpAdd()!=0 && GetHpAdd()%10==1)//�Ѿ��Թ���ƿ,����
					SetHpAdd(0);

				if ((GetHpAdd()/10+dwAmountLimit)>6000000)//���ܳ���6����
				{
					SendSysMsg(_TXTATR_TOPSHOW, "�ɻظ������������ޣ�����ʹ��");
					return false;
				}
			}
			SetHpAdd((GetHpAdd()/10 + dwAmountLimit)*10 + nBigType);
			if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, pItem->GetTypeID());
			ASSERT(SpendItem(pItem, 1));
		}
		else if (pItem->GetItemType()==31)//�������ҩ
		{
			if (nBigType==1)//��ƿ
			{
				if (GetMpAdd()!=0 && GetMpAdd()%10==2)//�Ѿ��Թ�Сƿ
				{
					SendSysMsg(_TXTATR_TOPSHOW, "���Ѿ�ӵ�и�ǿ��ı���ظ�״̬");
					return false;
				}
				if ((GetMpAdd()/10+dwAmountLimit)>6000000)//���ܳ���6����
				{
					SendSysMsg(_TXTATR_TOPSHOW, "�ɻظ������������ޣ�����ʹ��");
					return false;
				}
			}
			if (nBigType==2)//Сƿ
			{
				if (GetMpAdd()!=0 && GetMpAdd()%10==1)//�Ѿ��Թ���ƿ,����
					SetMpAdd(0);
				
				if ((GetMpAdd()/10+dwAmountLimit)>6000000)//���ܳ���6����
				{
					SendSysMsg(_TXTATR_TOPSHOW, "�ɻظ������������ޣ�����ʹ��");
					return false;
				}
			}
			SetMpAdd((GetMpAdd()/10 + dwAmountLimit)*10 + nBigType);
			if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
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
	else if (pItem->IsEnableDress())//  [4/5/2007] ���� ��װ
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
						SendSysMsg("����ֵ����������ָ�");
					}
					
					else
					{
						SendSysMsg("������������ָ�");
					}
					return false;
				}
			}
			else if(nAddLife != 0)
			{
				if(GetLife()==GetMaxLife() )
				{
					SendSysMsg("����ֵ����������ָ�");
					return false;
				}
			}
			else if(nAddMana != 0)
			{
				if( GetMana()==GetMaxMana())
				{
					SendSysMsg("������������ָ�");
					return false;
				}
			}
		}

		int nTimes		= /*pItem->GetInt( ITEMDATA_ATKSPEED )*/1;	// ��Ѫ\�����Ĵ���

		DEBUG_TRY	// VVVVVVVVVVVVVV	
		const int SP_TYPE = 32;
		bool bAddSp = false;
		if(  SP_TYPE == pItem->GetItemType() )
			bAddSp = true;
		if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, pItem->GetTypeID());
		if( !SpendItem( pItem ) )//??? �ܳԵ�һ����ɾ��, ��ɾ����������
			return false;
		if( nAddLife != 0  && nThirdType != TYPE_ZHENYUAN)//��Ѫ
		{
			if( bAddSp )
				AddAttrib( _USERATTRIB_SP, nAddLife, SYNCHRO_TRUE );	
			else	//�Ӻ�
			{
				// [7/16/2007]-------�����޸ģ���DOT�ӵ�ATKSPEED > 1 ----------------------
				if( nTimes > 1 )
				{
					int nFirstValue = nAddLife / 2;//��һ�β�һ��
					AddAttrib( _USERATTRIB_LIFE,nFirstValue,SYNCHRO_TRUE );//��һ��
					
					int nEachValue = nFirstValue / (nTimes - 1);
					//�����nTimes - 1
					int nInterval = 2;	//���ʱ��,��
					int nThisTimes = nTimes - 1;	
					CRole::AttachStatus( this->QueryRole(),STATUS_DOT_ADDLIFE,nEachValue,nInterval,nThisTimes );
					
				}
				else // 0 �� 1��˲��
				{
					AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
				}
			}
		}
		if( nAddMana != 0  && nThirdType != TYPE_ZHENYUAN) //����
		{
			if( nTimes > 1 )
			{
				int nFirstValue = nAddMana / 2;//��һ�β�һ��
				AddAttrib( _USERATTRIB_MANA,nFirstValue,SYNCHRO_TRUE );//��һ��
				
				//�����DOT
				int nEachValue = nFirstValue / (nTimes - 1);
				int nInterval = 2;	//���ʱ��,��
				int nThisTime = nTimes - 1;		//���ô���	
				CRole::AttachStatus( this->QueryRole(),STATUS_DOT_ADDMANA,nEachValue,nInterval,nThisTime );				
			}
			else
			{
				AddAttrib(_USERATTRIB_MANA, nAddMana, SYNCHRO_TRUE);
			}
			
		}
       
		//������Ҿ���ֵ
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
	else if(pItem->GetTypeID()==_SPCITEM_VASEXCHANGE)//goto Ԫ��Ʊ,���ﲻɾ��Ʒ��ȷ��Ԫ����ȡ�ɹ�����ɾ
	{
		nStep = 22;
		if(pItem->GetItemAmount()>2000)//Ԫ��Ʊ����2000
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
		SendSysMsg(_TXTATR_TOPSHOW, "����Ʒ������,����ʹ��");
		return false;
	}

	nStep = 2;
	if(pItem->GetTypeID() / 1000 == 322) //�����̲���,��Ӧ�ý�����,�������ҩtypeid��ͻ��,Ϊ�˷�ֹ������ͨ����ҩ�Ե�,ֻ���������⴦��һ��
	{
//		::LogSave("�����̲������˲��ý�������,user=%u,itemID=%u",GetID(), idItem);
		return false;
	}
	if(pItem->GetTypeID() / 10000 == 39)
	{
//		::LogSave("�������ٵ����˲��ý�������,user=%u,itemID=%u",GetID(), idItem);
		return false;
	}
	if(pItem->GetTypeID() / 10000 == 32) //����
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
				SendSysMsg("������������");
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
		if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
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
				SendSysMsg("�������ֵ����");
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
		
		if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
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
				SendSysMsg("��������ֵ����");
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
		
		if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
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
			SendSysMsg("�Է�������ĺ���");
			return false;
		}

		nStep = 11;
		CUserPtr pUserTarget = UserManager()->GetUser(idTarget);
		if(!pUserTarget)
		{
			SendSysMsg("�Է�����̫Զ");
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
			//�໥Ϊ����  ����Ӻ���ֵ
			pTargetFriend->SetFriendShip(idTarget,GetID(),Database(),nFriendShip + pTargetFriend->GetFriendShip());
			pMyFriend->SetFriendShip(GetID(),idTarget,Database(),nFriendShip + pMyFriend->GetFriendShip());
			SendSysMsg("�Է��Ѽ���Ϊ���ѣ�����%s���Ѻö�������%u��",pUserTarget->GetName(),nFriendShip);
			pUserTarget->SendSysMsg("��ĺ���%s�͸���%s�����ǵ��Ѻö�������%u��",GetName(),pItem->GetStr(ITEMDATA_NAME),nFriendShip);			
			if (650002 == pItem->GetTypeID())
			{
				char szMsg[512] = "";
				sprintf(szMsg, "<%s>��<%s>ʹ����<%s>��������һ��ף�����ǰɣ�", UserName, TarName, itemName);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, pItem->GetID(), pItem->GetTypeID(), NULL , ALLUSERS_NAME);
			}
			else if (650003 == pItem->GetTypeID())
			{
				char szMsg[512] = "";
				sprintf(szMsg, "<%s>��<%s>ʹ����<%s>���Ҹ���Լ����Լ������", UserName, TarName, itemName);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE, this, pItem->GetID(), pItem->GetTypeID(), NULL , ALLUSERS_NAME);
			}
		}
		else
		{
			pMyFriend->SetFriendShip(GetID(),idTarget,Database(),nFriendShip + pMyFriend->GetFriendShip());
			SendSysMsg("�Է�δ����Ϊ���ѣ����%s���Ѻö�������%u��",pUserTarget->GetName(),nFriendShip);
			pUserTarget->SendSysMsg("%s�͸���%s��������Ѻö�������%u��",GetName(),pItem->GetStr(ITEMDATA_NAME),nFriendShip);
			if (650002 == pItem->GetTypeID())
			{
				char szMsg[512] = "";
				sprintf(szMsg, "<%s>��<%s>ʹ����<%s>��������һ��ף�����ǰɣ�", UserName, TarName, itemName);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, this, pItem->GetID(), pItem->GetTypeID(), NULL , ALLUSERS_NAME);
			}
			else if (650003 == pItem->GetTypeID())
			{
				char szMsg[512] = "";
				sprintf(szMsg, "<%s>��<%s>ʹ����<%s>���Ҹ���Լ����Լ������", UserName, TarName, itemName);
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_TOPANNOUNCE, this, pItem->GetID(), pItem->GetTypeID(), NULL , ALLUSERS_NAME);
			}
		}
		
		nStep = 13;
		DWORD dwItemType = pItem->GetTypeID();
		if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", GetID(), GetName(), 1, dwItemType);
		SpendItem(pItem,1);
		//�㲥�ͻ��˲���Ч
		if (dwItemType==650003)
		{
			CMsgAction msg;
			if (msg.Create(GetID(), 0,0,0,actionFireWorks,dwItemType))
			{
				BroadcastMapMsg(&msg, INCLUDE_SELF);
				if (GetMapID() != pUserTarget->GetMapID())//������˲���ͬһ����,����������Ҫ�㲥
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
		LOGWARNING("�����װ��λ��");
		return false;
	}

// 	if( !IsBackPackSpare(1, 0, ID_NONE, ITEMPOSITION_BACKPACK1 )
// 		&& !IsBackPackSpare(1, 0, ID_NONE, ITEMPOSITION_BACKPACK2 )
// 		&& !IsBackPackSpare(1, 0, ID_NONE, ITEMPOSITION_BACKPACK3 ))
// 		return false;

	if(m_pPackage->IsPackFull(1,pEquip->GetTypeID()))
	{
		SendSysMsg("��������");
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
		LOGWARNING("����ȥ��װ�����ϻ��У�userid = %u itemid = %u outPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());

	nStep =41;
	if((pItem->GetPosition() < ITEMPOSITION_PACK_BEGIN) || (pItem->GetPosition() >= ITEMPOSITION_PACK_END))
		LOGWARNING("����ȥ��װ��λ�ò��ԣ�userid = %u itemid = %u outPosition = %u itemPosition = %u",GetID(),pItem->GetID(),nPosition,pItem->GetPosition());

	nStep =5;
	int oldPosition = nPosition;
	nPosition = nPosition * 1000 + pItem->GetPosition();/*MAKELONG( nPosition,pItem->GetPosition());*/ //ԭ����POS�����ڵ�POS
	if(bSynchro)
	{
		CMsgItem msg;
		if(msg.Create(ITEMACT_UNEQUIP,0, 0, nPosition,pItem->GetPackageIndex(),pItem->GetID()))//06.10.7�����޸�
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
			//���
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
			CMsgPlayer msgPlayer;// ������Ϣ���ͻ��˸��� [�½���8/25/2007]
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
//06.3.13�����޸�
CItem* CUser::UnEquipOnly(int nPosition,int nWant2Position,int nWant2PackageIndex)
{ 

	CItem* pItem = NULL;
	CItemPtr& refpEquip = GetEquipItemRef(nPosition);
	if (refpEquip == NULL)
		return NULL;

	if(GetItem(refpEquip->GetID()))
	{
		LogSave("��װ��ǰ�����ֱ�����ͬ����Ʒ��userid = %u , itemid = %u itemtype = %u",GetID(),refpEquip->GetID(),refpEquip->GetTypeID());
		return NULL;
	}

	if(refpEquip)
	{
		pItem = refpEquip;

		int nStep = 0;
		DEBUG_TRY
		if(m_pPackage->IsPackFull(1,pItem->GetTypeID()))
		{
			SendSysMsg("��������");
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
			LogSave("��װ����ʱ����Ʒ�޷����뱳�����д���֤");
			return NULL;
		}
		
		DEBUG_CATCH3("��װ����ʱ���쳣��userid = %u step = %u",GetID(),nStep)
		refpEquip = NULL;
	}
	return pItem;
}

//////////////////////////////////////////////////////////////////////
// msgitem
//////////////////////////////////////////////////////////////////////
//BUY A ITEM FROM SHOP
//06.9.15�����޸�
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
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::GetRandShopMoney user:%u(%s)�������%d",GetID(),GetName(),nMoney1);
	else if (GetLev()>=50 && GetLev()<70 && nMoney1>60000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::GetRandShopMoney user:%u(%s)�������%d",GetID(),GetName(),nMoney1);
	else if (GetLev()>=70 && nMoney1>100000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::GetRandShopMoney user:%u(%s)�������%d",GetID(),GetName(),nMoney1);

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
			//20061030����
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
				this->SendSysMsg("��������");
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
				this->SendSysMsg(_TXTATR_TOPSHOW,"������������");
				return;
			}
			if (nBuyType==7)
			{
				if (this->GetSynCon()<nPrice)
				{
					this->SendSysMsg(_TXTATR_TOPSHOW,"�ﹱ����");
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
			if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop", GetID(), GetName(), nAmount, idType);

			ASSERT(this->SpendMoneyByType(nPrice,nBuyType, SYNCHRO_TRUE));
			nStep = 44;
		}
		else if (nSpendType == 7)
		{
			nStep =71;
			if (m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
			{
				this->SendSysMsg("��������");
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
				this->SendSysMsg(_TXTATR_TOPSHOW,"�ﹱ����");
				return;
			}
			
			CItem*	pItem = NULL;
			if(nAmount < 1)
				nAmount = 1;
			pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, false, nAmount, false,true,false,1);
			
			nStep = 74;
			if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
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
				SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
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
				this->SendSysMsg("��������");
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
				this->SendSysMsg(_TXTATR_TOPSHOW,"%s����",pItemTypeNeedData->GetStr(ITEMTYPEDATA_NAME));
				return;
			}

			nStep = 64;
			CItem*	pItem = NULL;
			if(nAmount < 1)
				nAmount = 1;

			int nKindnap = 1;
			pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, true, nAmount, false,true,false,nKindnap);
			
			nStep = 65;
			if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
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
	//modify code by �ֵ�Ԩ begin 2011.5.21
				CItem *pitem_data = GetItemByType(nSpendType);
				if( pitem_data )
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) %u total (%d)", GetID(), GetName(), nSpendType,pitem_data->GetItemAmount());
				else
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) %u total (%d)", GetID(), GetName(), nSpendType,0);
	//modify code by �ֵ�Ԩ end
			}
		}
		return;
		DEBUG_CATCH3("BuyItemFromPortableShop error step = %u idNpc= %u ",nStep,idNpc)
	}
//////////////////////////////////////////////////////////////////////////////////////////////////
// 	//��������̵�NPC����
// 	if(!isShopNpcSell)
// 	{
// 	  //�������
// 	  if(idType <= 0 || nAmount <= 0)
// 		return;
// 	
// 	   CItem* pItem = NULL;
// 
// 	  //�������ͼ��
// 	   CItemTypeData* pItemType = ItemType()->QueryItemType(idType);
// 	   if(!pItemType)
// 		   return;
// 
// 	   if(!CItem::IsCountable(idType))//���ɵ���
// 	   {
// 		   //Ǯ���
// 		   int nPrice = pItemType->GetInt(ITEMTYPEDATA_PRICE);
// 		   
// 		   if(GetMoney() < nPrice* nAmount)
// 		   {
// 			   char szMsg[1024];
// 			   sprintf(szMsg,"%s","��Ľ�Ҳ�����");
// 			   szMsg[255] = '\0';
// 			   CMsgTalk msg;
// 			   if(msg.Create(SYSTEM_NAME,this->GetName(),szMsg,NULL,0x00ffff00,_TXTATR_NORMAL))
// 				   this->SendMsg(&msg);
// 			   return;
// 		   }
// 		   
// 		   //��Ǯ
// 		   if(!this->SpendMoney(nPrice * nAmount, SYNCHRO_TRUE))
// 			   return;
// 		   
// 		   if((this->IsBackPackSpare(nAmount, 0, 920002, 0)))	//	920002�����壬ֻ��ָ��һ��TYPEID�Ϳ���
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
		//20061030����
		::LogSave("User_Item.cpp Ln 1215.IF_NOT(pType)...idNpc =%d  ||idType =%d  ||nAmount =%d ||nMapID =%u ||nNPCPosX =%d ||nNPCPosY =%d",idNpc,idType,nAmount,GetMap()->GetID(),pNpc->GetPosX(),pNpc->GetPosY());
		return;
	} 

	nStep = 3;
	CItemTypeData* pItemTypeData = ItemType()->QueryItemType( idType );
	if( !pItemTypeData )
		return;

	//	20070712	���	Ψһ��Ʒ���
// 	if(IsUserHaveAnExclusiveItem(idType, 
// 		NULL, this))
// 	{
// 		char szTemp[128];
// 		sprintf(szTemp,g_objStrRes.GetStr(16013),pType->GetStr(ITEMTYPEDATA_NAME));//"����ʧ��.������ӵ�ж��[%s]!"
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
			this->SendSysMsg("��������");
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
			this->SendSysMsg(_TXTATR_TOPSHOW,"������������");
			return;
		}
		if (nBuyType==7)
		{
			if (this->GetSynCon()<nPrice)
			{
				this->SendSysMsg(_TXTATR_TOPSHOW,"�ﹱ����");
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
		if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop", GetID(), GetName(), nAmount, idType);

		ASSERT(this->SpendMoneyByType(nPrice,nBuyType, SYNCHRO_TRUE));
		nStep = 44;
		//	20070710	���	���Ƚ�Ǯ��¼
// 		if(CItem::IsLogSaveMoney(nPrice))
// 		{
// 			MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 				this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_BUY, 
// 				0, 0, "ʱ�ձ�", nPrice);
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
			this->SendSysMsg("�����ռ䲻��");
			return;
		}

		int nHasMoney = pItemMoney->GetItemAmount();

		nStep = 52;
	
		bool bFull = CheckRandShopFull();

		if(bFull)
		{
			SendSysMsg("��Ʊ�������������������ˣ���ȥ�������");
			return;
		}

		nStep = 53;
		if(pNpc->QueryShop()->QueryRandShopSellType(idType) != 1)
			return;

		if(pNpc->QueryShop()->QueryRandShopLeave(idType) < nAmount)
		{
			SendSysMsg("��Ʒ�������㣬��ˢ��");
			return;
		}

		nStep = 54;
		int nPerPrice = pNpc->QueryShop()->QueryRandShopPrice(idType);
		if(nPerPrice <= 0)
		{
			SendSysMsg("�̵�۸��Ѹı䣬�����´��̵�");
			LOGWARNING("������Ʒ�۸����,userid=%u,itemtype=%u,npcid=%u",GetID(),idType,idNpc);
			return;
		}

		if(nPrice != nPerPrice)
		{
			SendSysMsg("�̵�۸��Ѹı䣬�����´��̵�");
			return;
		}

		DWORD nPrice = nAmount * nPerPrice;

		
		nStep = 55;
		if(nHasMoney < nPrice)
		{
			SendSysMsg("��Ʊ�����۸���ˢ�£������´��̵�");
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
			SendSysMsg("������������������һ����Ʒ����������");
		}

		nStep = 56;
	}
	else if (nSpendType == 7)
	{
		nStep =71;
		if (m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
		{
			this->SendSysMsg("��������");
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
			this->SendSysMsg(_TXTATR_TOPSHOW,"�ﹱ����");
			return;
		}
		
		CItem*	pItem = NULL;
		if(nAmount < 1)
			nAmount = 1;
		pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, false, nAmount, false,true,false,1);
		
		nStep = 74;
		if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop", GetID(), GetName(), nAmount, idType);
		if (nPrice >= 2000)
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) oldcon=%u spend %u from shop to buy %u", GetID(), GetName(),GetSynCon(), nPrice, idType);
		this->SetSynCon(GetSynCon()-nPrice);

	}
	else if(nSpendType == 8)//�����������̵�
	{
		if (IsItemLockedOpen())
		{
			SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
			return;
		}

		CHECK(pNpc->QueryShop());
		int nSpendScore = pNpc->QueryShop()->QueryItemAmountSpend(idType);
		CHECK(nSpendScore > 0);

		CItemTypeData* pItemTypeNeedData = ItemType()->QueryItemType( idType );
		CHECK(pItemTypeNeedData);

		if (m_pPackage && m_pPackage->IsPackFull(1,idType,m_pPackage->GetPositionByItemType(idType)))
		{
			this->SendSysMsg("��������");
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
			this->SendSysMsg(_TXTATR_TOPSHOW,"����ֵ����");
			return;
		}

		CItem*	pItem = NULL;
		if(nAmount < 1)
			nAmount = 1;
		
		int nKindnap = 1;
		pItem = AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK, true, true, nAmount, false,true,false,nKindnap);

		if(pItem)
			AddAttrib(_USERATTRIB_ARENA_SCORE, -1*nSpendScore, SYNCHRO_TRUE);

		if (pItem/* && pItem->IsCostlyItem()*/)//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from shop by %d arena score", GetID(), GetName(), nAmount, idType, nSpendScore, nSpendType);
	}
	else
	{
		nStep = 61;
		if (IsItemLockedOpen())
		{
			SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
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
			this->SendSysMsg("��������");
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
			this->SendSysMsg(_TXTATR_TOPSHOW,"%s����",pItemTypeNeedData->GetStr(ITEMTYPEDATA_NAME));
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
		if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
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
//modify code by �ֵ�Ԩ begin 2011.5.21
			CItem *pitem_data = GetItemByType(nSpendType);
			if( pitem_data )
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) %u total (%d)", GetID(), GetName(), nSpendType,pitem_data->GetItemAmount());
			else
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) %u total (%d)", GetID(), GetName(), nSpendType,0);
//modify code by �ֵ�Ԩ end
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
			this->SendSysMsg("�Ҳ�����Ʒ");
			return false;
		}
		if(GetEquipItem(idItem))
		{
			this->SendSysMsg("���ܳ��۴������ϵ�װ��");
			return false;
		}
		if(pItem->IsLocked())
			return false;
		if(!pItem->IsSellEnable())
			return false;
		if(pItem->GetWpSlLev() >= 3)
			return false;
	//���˿׵ġ����˱�ʯ�ġ�ǿ�����ġ������ǵġ��������������ϵġ���ɫ�Լ����ϵ�,��Ҫ�жϰ�ȫ��
		if (IsItemLockedOpen())
		{
			if (pItem->GetEmbedCountLimit()>0 || pItem->GetEmbedGemCount()>0 || pItem->GetEquipAddLev()>0 || pItem->GetEquipStar()>0
				|| pItem->GetEquipQual()>3 || pItem->GetColour()>2 || pItem->IsSoulMark())
			{
				SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
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
				this->SendSysMsg("����ʧ��,��Ľ�Ǯ��������,�������Я���Ľ�Ǯ����");
				return false;
			}
		}
		else
		{
			if (GetMoneyByType(2) + dwMoney > _MAX_MONEYLIMIT)
			{
				this->SendSysMsg("����ʧ��,��Ľ�Ǯ��������,�������Я���Ľ�Ǯ����");
				return false;
			}
		}
	
		DWORD nItemType = pItem->GetTypeID();
		int nAmount = pItem->GetItemAmount();
		const char* itemName = pItem->GetStr(ITEMDATA_NAME);
		if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from selling", GetID(), GetName(), CItem::IsCountable(nItemType)?pItem->GetItemAmount():1, nItemType);
		if(!this->EraseItem(idItem, SYNCHRO_TRUE))
		{
			this->SendSysMsg("ɾ����Ʒ����");
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
			::MyLogSave(VALUABLESLOG_FILENAME, "CUser::SellItem user:%u(%s)����%d��%s�������%d",GetID(),GetName(),nAmount,itemName,dwMoney);
		if(!bRet)
			this->SendSysMsg("����ʧ��,��Ľ�Ǯ��������,�������Я���Ľ�Ǯ����");//STR_MONEYBAG_FULL

	//////////////////////////////////////////////////////////////////////////
	//	20070710	���	���Ƚ�Ǯ��¼
		if(bRet && CItem::IsLogSaveMoney(dwMoney))
		{
			MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
				this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_SELL, 
				0, 0,g_objStrRes.GetStr(15072), dwMoney);// "ʱ�ձ�"
		}
		DEBUG_CATCH("SellItem") // AAAAAAAAAAAAAAAAA
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	CNpc* pNpc; 
	if(!GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, idNpc, IPP_OF(pNpc)))
	{
		if(idNpc == 5000)//����ҩ��
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
			this->SendSysMsg("�Ҳ�����Ʒ");
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
			SendSysMsg("�̵�۸��Ѹı䣬�����´��̵�");
			LOGWARNING("������Ʒ�۸����,userid=%u,itemtype=%u,npcid=%u",GetID(),pItem->GetTypeID(),idNpc);
			return false;
		}
		
		if(nPrice != nPriceSell)
		{
			SendSysMsg("�̵�۸��Ѹı䣬�����´��̵�");
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
		this->SendSysMsg("�Ҳ�����Ʒ");
		return false;
	}

	if(GetEquipItem(idItem))
	{
		this->SendSysMsg("���ܳ��۴������ϵ�װ��");
		return false;
	}
	
	if(pItem->IsLocked())
		return false;

	if(!pItem->IsSellEnable())
		return false;

	if(pItem->GetWpSlLev() >= 3)
		return false;
	//���˿׵ġ����˱�ʯ�ġ�ǿ�����ġ������ǵġ��������������ϵġ���ɫ�Լ����ϵ�,��Ҫ�жϰ�ȫ��
	if (IsItemLockedOpen())
	{
		if (pItem->GetEmbedCountLimit()>0 || pItem->GetEmbedGemCount()>0 || pItem->GetEquipAddLev()>0 || pItem->GetEquipStar()>0
			|| pItem->GetEquipQual()>3 || pItem->GetColour()>2 || pItem->IsSoulMark())
		{
			SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
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
			this->SendSysMsg("����ʧ��,��Ľ�Ǯ��������,�������Я���Ľ�Ǯ����");
			return false;
		}
	}
	else
	{
		if (GetMoneyByType(2) + dwMoney > _MAX_MONEYLIMIT)
		{
			this->SendSysMsg("����ʧ��,��Ľ�Ǯ��������,�������Я���Ľ�Ǯ����");
			return false;
		}
	}
	
	DWORD nItemType = pItem->GetTypeID();
	int nAmount = pItem->GetItemAmount();
	const char* itemName = pItem->GetStr(ITEMDATA_NAME);
	if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from selling", GetID(), GetName(), CItem::IsCountable(nItemType)?pItem->GetItemAmount():1, nItemType);
	if(!this->EraseItem(idItem, SYNCHRO_TRUE))
	{
		this->SendSysMsg("ɾ����Ʒ����");
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
		::MyLogSave(VALUABLESLOG_FILENAME, "CUser::SellItem user:%u(%s)����%d��%s�������%d",GetID(),GetName(),nAmount,itemName,dwMoney);
	if(!bRet)
		this->SendSysMsg("����ʧ��,��Ľ�Ǯ��������,�������Я���Ľ�Ǯ����");//STR_MONEYBAG_FULL

	//////////////////////////////////////////////////////////////////////////
	//	20070710	���	���Ƚ�Ǯ��¼
	if(bRet && CItem::IsLogSaveMoney(dwMoney))
	{
		MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
			this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_SELL, 
			0, 0,g_objStrRes.GetStr(15072), dwMoney);// "ʱ�ձ�"
	}
	DEBUG_CATCH("SellItem") // AAAAAAAAAAAAAAAAA
	return true;
}

//////////////////////////////////////////////////////////////////////
//�����޸�,����Ĭ�ϲ���,֧�ֶ���������Ʒ����������
bool CUser::DropItem( OBJID idItem, bool bIsMustDrop/* = false*/)
{
	CItem* pFindItem = this->GetItem( idItem,false );
	if( !pFindItem  )
		return false;

	if(!pFindItem->IsDropEnable())
		return false;

	if(pFindItem->GetWpSlLev() >= 3)
		return false;
	
	//���˿׵ġ����˱�ʯ�ġ�ǿ�����ġ������ǵġ��������������ϵġ���ɫ�Լ����ϵ�,��Ҫ�жϰ�ȫ��
	if (IsItemLockedOpen() && !bIsMustDrop)
	{
		if (pFindItem->GetEmbedCountLimit()>0 || pFindItem->GetEmbedGemCount()>0 || pFindItem->GetEquipAddLev()>0 || pFindItem->GetEquipStar()>0
			|| pFindItem->GetEquipQual()>3 || pFindItem->GetColour()>2 || pFindItem->IsSoulMark())
		{
			SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
			return false;
		}
	}
	if( false == pFindItem->IsDropEnable() && this->IsAlive() )
	{
		this->SendSysMsg( g_objStrRes.GetStr(15073) );//"����Ʒ���ɶ���!"
		return false;
	}
	if (pFindItem && pFindItem->IsCostlyItem())//goto ������Ʒ��¼
//modify code by �ֵ�Ԩ begin 2011.5.21
	{
		CItem *pitem_data = GetItemByType(pFindItem->GetTypeID());
		if( pitem_data )
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from destroy %u total (%d)", GetID(), GetName(), CItem::IsCountable(pFindItem->GetTypeID())?pFindItem->GetItemAmount():1, pFindItem->GetTypeID() ,pFindItem->GetTypeID(),pitem_data->GetItemAmount());
		else
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from destroy %u total (%d)", GetID(), GetName(), CItem::IsCountable(pFindItem->GetTypeID())?pFindItem->GetItemAmount():1, pFindItem->GetTypeID() ,pFindItem->GetTypeID(),0);
	}
//modify code by �ֵ�Ԩ end
	BOOL bRet = EraseItem(idItem, SYNCHRO_TRUE);

//	UpdateWeight();
	return bRet;
}
//��ʱ��֮��ר�ú���
bool CUser::DropItemBySys( OBJID idType,int x,int y )
{
// 	CItem* pItem = NULL;
// 	ItemInfoStruct info;
// 	//������Ʒ(ʱ��֮��)
// 	for( int i = 0 ;i < 128; i++ )//һ������������// �޸ĺܶ�� [9/14/2007 %PENGFENG%]
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
// 		if ( GetMap()->FindDropItemCell( MONSTERDROPITEM_RANGE, &pos ) )  //�õ�λ��
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
// 			bRet = this->SpendMoney(dwMoney, SYNCHRO_TRUE);				// ��ɾ������ֹ����
// 			//////////////////////////////////////////////////////////////////////////
// 			//	20070710	���	���Ƚ�Ǯ��¼
// 			if(bRet && CItem::IsLogSaveMoney(dwMoney))
// 			{
// 				MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 					this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_DROP, 
// 					0, 0, "ʱ�ձ�", dwMoney);
// 			}
// 			//	���� 20070710
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
//�����޸�														//
//�˺���Ϊ��Ҳ����Ʒ												//
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
	// ��鱳���Ƿ���
	if (this->IsItemFull(0, pItem->GetInt(ITEMDATA_TYPE), /*pItem->GetInt(ITEMDATA_AMOUNTLIMIT)*/1))
	{
		this->SendSysMsg(g_objStrRes.GetStr(15074)); //"��������,�޷������Ʒ!"
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
//�����޸�
//�˺���Ϊ�ϲ��ɵ�����Ʒ
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
		
	if (pItem->IsLocked() || pOther->IsLocked())//ֻҪ��һ����Ʒ������״̬�Ͳ��ܺϲ�
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��������Ʒ���ܽ��д˲���");
		if (!pItem->IsLocked())//��û��������Ʒ����
			SendUnlockItemMsg(idItem);
		if (!pOther->IsLocked())//��û��������Ʒ����
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
	else	//����С�ڿɵ����������
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
			/*GameDataDefault()->GetWeaponSoulData()*/NULL//20071113����:���
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

bool CUser::SpendOneItem(OBJID idItem)//����һ������,���ű�ʹ��
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
			LOGWARNING("ΪʲôҪ����װ������");
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

// 	switch( nKind )//��������
// 	{
// 	case 0://��
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
// 	case 1://��
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
BOOL CUser::SpendNewRemedy( CItem* pItem, int nPosition/*=ITEMPOSITION_BACKPACK*/, bool bSynchro/*=true*/,CUserPetData* pData,int nMaxLife /*= 0*/)	//---���-----2007/03/26----��ԺȲ����Ѫƿд��ʹ�ú���-----------
{
	CHECKF(pItem);
	if( 0 == pItem->GetInt(ITEMDATA_AMOUNT) )// ��� [7/3/2007]
		return false;

	if(!GetMap())
		return false;
	if (GetMap()->IsNotAllowNewRemedy())
		return false;

	// ��BUF,����ʹ����ֵҩˮ [1/15/2008 �½���]
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
		switch( nKind )//��������
		{
		case 0://��
			{
				int need = min(nMana , GetMaxLife() - GetLife());
				//2007106����:bug���ʣ����������-------
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
		case 1://��
			{
				int need = min(nLife , GetMaxMana() - GetMana());
				//2007106����:bug���ʣ����������-------
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
		case 2://�����
			{
				int need = min(nMana , nMaxLife - pData->GetInt(EUDEMONDATA_life));
				int amount = nMaxLife;
				//2007106����:bug���ʣ����������-------
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
//[2010-08-27 goto]//������������Զ��ظ��������ֶ�ʹ��
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
			if (nAddLife%10==1 && nLostLife>5000)//��ƿ����5000
				nLostLife = 5000;
			if (nAddLife%10==2 && nLostLife>10000)//Сƿ����10000
				nLostLife = 10000;
			if (nAddLife/10 > nLostLife)
			{
				AddAttrib(_USERATTRIB_LIFE, nLostLife, SYNCHRO_TRUE);
				this->BroadcastTeamLife();
// 				bool bUpdate = false;
// 				if (nAddLife%10000 < nLostLife)//ÿ��һ��ͬ��һ�����ݿ�
// 					bUpdate = true;
				SetHpAdd((nAddLife/10-nLostLife)*10+nAddLife%10);
				//ͬ���ͻ���
				bNeedSend = true;
			}
			else
			{
				AddAttrib(_USERATTRIB_LIFE, nAddLife/10, SYNCHRO_TRUE);
				this->BroadcastTeamLife();
				SetHpAdd(0);
				//ͬ���ͻ���
				bNeedSend = true;
			}
		}
	}
	if (nAddMana>0)
	{
		int nLostMana = GetMaxMana()-GetMana();
		if (nLostMana>0)
		{
			if (nAddMana%10==1 && nLostMana>2000)//��ƿ����2000
				nLostMana = 2000;
			if (nAddMana%10==2 && nLostMana>4000)//Сƿ����4000
				nLostMana = 4000;
			if (nAddMana/10 > nLostMana)
			{
				AddAttrib(_USERATTRIB_MANA, nLostMana, SYNCHRO_TRUE);
				this->BroadcastTeamMana();			
				SetMpAdd((nAddMana/10-nLostMana)*10+nAddMana%10);
				//ͬ���ͻ���
				bNeedSend = true;
			}
			else
			{
				AddAttrib(_USERATTRIB_MANA, nAddMana/10, SYNCHRO_TRUE);
				this->BroadcastTeamMana();
				SetMpAdd(0);
				//ͬ���ͻ���
				bNeedSend = true;
			}
		}
	}
	if (nAddPet>0)
	{
		CMonster* pMonster = QueryCallPet();//��ó�ս�ĳ���
		if (pMonster)//û�г�ս�ĳ���
		{
			int nLostLife = pMonster->GetMaxLife()-pMonster->GetLife();
			if (nLostLife>0)
			{
				if (nLostLife>2500)//���ֻ�ܼ�2500
					nLostLife = 2500;
				if (nAddPet > nLostLife)
				{
					pMonster->AddAttrib(_USERATTRIB_LIFE, nLostLife, SYNCHRO_TRUE);
					SetPetAdd(nAddPet-nLostLife);
					//ͬ���ͻ���
					bNeedSend = true;
				}
				else
				{
					pMonster->AddAttrib(_USERATTRIB_LIFE, nAddPet, SYNCHRO_TRUE);
					SetPetAdd(0);
					//ͬ���ͻ���
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
		if(NpcID == 5000)//����ҩ��
		{
		}
		return false;		
	}

	if(!pNpc || pNpc->GetInt(NPCDATA_TYPE) != 0)
		return false;
	
	
	CItem* pItem = GetItem( itemID );
	if(!( pItem && pItem->CanRepair() && pItem->GetInt(ITEMDATA_AMOUNTLIMIT) > pItem->GetInt(ITEMDATA_AMOUNT) ))
	{
		this->SendSysMsg("����ʧ��");	//����ʧ��STR_REPAIR_FAILED
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
		this->SendSysMsg("������������");
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
		sprintf(szTemp,"%d��",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = (nMoneyNeed % 10000) / 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d��",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = nMoneyNeed % 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%dͭ",nMoney);
		strcat(szMoney,szTemp);
	}
	SendSysMsg(_TXTATR_TOPSHOW,"����ɹ�,����%s", szMoney);

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

bool CUser::RepairOneSpecial(OBJID nEquipPos,bool bSynchro)//�������[2008/11/18]�����������ϵ�װ��
{
	//CItem* pItem = GetItem( itemID );
	return false;
	CItem* pItem = GetEquipItemByPos(nEquipPos);
			
	//20070705����:�������ж�
	//if(!( pItem && pItem->IsRepairEnable() && pItem->GetInt(ITEMDATA_AMOUNTLIMIT) > pItem->GetInt(ITEMDATA_AMOUNT) ))
	if(!( pItem && pItem->CanRepair() && pItem->GetInt(ITEMDATA_AMOUNTLIMIT) > pItem->GetInt(ITEMDATA_AMOUNT) ))
	{
		this->SendSysMsg(g_objStrRes.GetStr(12089));	//����ʧ��STR_REPAIR_FAILED
		return false;
	}

	//2007918����:��Ч��ת��----
	BOOL bIsValidOld = pItem->IsValidEquip();
	//--------------------------
	
	int nMoney		= pItem->CalcRepairMoney();
	int	nRepair		= pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - pItem->GetInt(ITEMDATA_AMOUNT);
	
	if(nMoney <= 0 || nRepair <= 0)
		return false;
	
	if (!this->SpendMoney(nMoney, SYNCHRO_TRUE))
	{
		this->SendSysMsg(g_objStrRes.GetStr(12090), nMoney);//��Ǯ������ֱ��return false;STR_REPAIR_NO_MONEY_d
		return false;	
	}
	int nAmount = pItem->GetInt(ITEMDATA_AMOUNTLIMIT)/**0.95*/;// ��ԭ�־�����ֵ[2008/11/18]�������
	pItem->SetInt(ITEMDATA_AMOUNT, nAmount); //�趨�µĳ־�Ϊ�־�����
	//  [4/16/2007] ���� ��ͨ����ʱװ������ �ı���Ʒ���;� ����
	//[��;���� 2009.05.04]��������
	//pItem->SetInt(ITEMDATA_AMOUNTLIMIT,nAmount,true);
	
	if ( bSynchro )
	{
		//		CMsgItemInfo msg;
		//		CHECKF(msg.Create(pItem, ITEMINFO_UPDATE));
		//		SendMsg(&msg);
		/////////////////////////////////////����ע�����ϲ��޸ĳ�:
		CMsgItem	msg;
		IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
			SendMsg(&msg);
		CMsgItem    msg1;// �½��� ��������������Ϣ���ͻ���[4/29/2007]
		IF_OK(msg1.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNTLIMIT, pItem->GetInt(ITEMDATA_AMOUNTLIMIT)))
			SendMsg(&msg1);
	}	

	//2007918����:��Ч��ת��----
	if(bIsValidOld != pItem->IsValidEquip())
	{
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//���½�ɫ��
	}
	//--------------------------
	
	return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

bool CUser::RepairAllSpecial(bool bSynchro,bool bWithHammer,OBJID hammerID)//�������[2008/11/18]ȫ������
{
	return false;
	if (!bWithHammer)//����������޴�������Ҫ��Ϸ��[2008/11/20]
	{
		DWORD dwNeedMoney = 0;
		//���������軨�Ѷ���Ǯ
		for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND - 5; i++)//[��;���� 2009.04.17]�ų���ҩ֮���
		{
			CItem* pEquip = GetEquipItemByPos(i);
			if(pEquip)
				dwNeedMoney	+= pEquip->CalcRepairMoney();
		}
		//���û���㹻��Ǯ
		if( false == this->SpendMoney( dwNeedMoney,SYNCHRO_TRUE ) )
		{
			this->SendSysMsg( g_objStrRes.GetStr(15075) );//"��Ǯ����,����ʧ��!"
			return false;
		}
		//������㹻��Ǯ
		else
		{
			if(bSynchro)//���ͬ���ʹ��ؿͻ����ܹ�Ҫ��ȥ�Ľ�Ǯ
			{
				CMsgItem	msg;
				CHECKF(msg.Create(dwNeedMoney, ITEMACT_REPAIRALL));
				SendMsg(&msg);	
			}
		}
	}
	else
	{		
		//��ȡ����
		CItem* pHammer = GetItem(hammerID);
		if (!pHammer)
		{
			this->SendSysMsg("��������");
			return false;
		}
		int nAmount	 = pHammer->GetInt(ITEMDATA_AMOUNT);	
		if (pHammer->GetInt(ITEMDATA_TYPE) == 900300)
		{
			//[��;���� 2009.05.08]
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
				this->SendSysMsg("����ɹ�");
			}
			else
			{
				//DelItem(itemID,true);
				EraseItem(hammerID,true,true);
				this->SendSysMsg("����ɹ�");
			}
		}
	}
	//2007918����:��Ч��ת��----
	int nIsValidOldBit = 0;//��Ч������
	int nIsValidNewBit = 0;
	//--------------------------
	for( int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND-5; i++)//[��;���� 2009.04.17]//20060913����:��ҩ���޸�
	{
		CItem* pEquip = GetEquipItemByPos(i);
		if(pEquip && pEquip->CalcRepairMoney() > 0)
		{
			//2007918����:��Ч��ת��----
			//ͳ����Ч��
			//���κ�һ��װ�������ı�,����½�ɫ��
			BOOL bIVO = pEquip->IsValidEquip();
			nIsValidOldBit |= bIVO<<i;
			//--------------------------

			pEquip->SetInt(ITEMDATA_AMOUNT, (int)(pEquip->GetInt(ITEMDATA_AMOUNTLIMIT)/**0.95*/));// ��������:��ԭΪ����ֵ[2008/11/18]�������
			pEquip->SetInt( ITEMDATA_AMOUNTLIMIT,pEquip->GetInt(ITEMDATA_AMOUNT) );
			//  [4/16/2007] ���� ������ͨ����ʱ���޸�װ���;����ޡ�
			CMsgItem	msg1;
			IF_OK(msg1.Create( pEquip->GetID() , ITEMACT_SYNCHRO_AMOUNT, pEquip->GetInt(ITEMDATA_AMOUNT)))
			SendMsg(&msg1);
			CMsgItem    msg2;// �½��� ��������������Ϣ���ͻ���//  [5/23/2007]
			IF_OK(msg2.Create( pEquip->GetID(), ITEMACT_SYNCHRO_AMOUNTLIMIT, pEquip->GetInt(ITEMDATA_AMOUNTLIMIT)))
			SendMsg(&msg2);

			//2007918����:��Ч��ת��----
			BOOL bIVN = pEquip->IsValidEquip();
			nIsValidNewBit |= bIVN<<i;
			//--------------------------
		}					
	}
	//2007918����:��Ч��ת��----
	if(nIsValidOldBit != nIsValidNewBit)
	{
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
	}
	//--------------------------
	return true;
}
//////////////////////////////////////////////////////////////////////////

bool CUser::RepairSpecial( OBJID itemID, bool bSynchro )//  [4/16/2007] ���� װ����������
{
	return false;
	CItem* pItem = GetItem( itemID );	
	if(!( pItem && pItem->IsRepairEnable() && pItem->GetInt(ITEMDATA_AMOUNTLIMIT) > pItem->GetInt(ITEMDATA_AMOUNT) ))
	{
		this->SendSysMsg(g_objStrRes.GetStr(12089));	//����ʧ��STR_REPAIR_FAILED
		return false;
	}

	//2007918����:��Ч��ת��----
	BOOL bIsValidOld = pItem->IsValidEquip();
	//--------------------------


	int	nRepair		= pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - pItem->GetInt(ITEMDATA_AMOUNT);
	if( nRepair <= 0)
		return false;
	pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNTLIMIT)); //�趨�µĳ־�Ϊ�־�����
	if ( bSynchro )
	{
		//		CMsgItemInfo msg;
		//		CHECKF(msg.Create(pItem, ITEMINFO_UPDATE));
		//		SendMsg(&msg);
		/////////////////////////////////////����ע�����ϲ��޸ĳ�:
		CMsgItem	msg;
		IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
			SendMsg(&msg);
	}	


	//2007918����:��Ч��ת��----
	if(bIsValidOld != pItem->IsValidEquip())
	{
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//���½�ɫ��
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
		if(NpcID == 5000)//����ҩ��
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
		this->SendSysMsg("��Ǯ����,����ʧ��");
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
		sprintf(szTemp,"%d��",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = (dwNeedMoney % 10000) / 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d��",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = dwNeedMoney % 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%dͭ",nMoney);
		strcat(szMoney,szTemp);
	}
	SendSysMsg(_TXTATR_TOPSHOW,"����ɹ�,����%s", szMoney);

	if(nIsValidOldBit != nIsValidNewBit)
	{
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
//[2010-07-21 goto]��Ϊ100%�ɹ���ֻ��Ҫװ������Ƕʯ�ͱ�ʯ
bool CUser::EmbedGem( OBJID idEquip, OBJID idStone, OBJID idGem )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return false;
	}
	//���װ��
	CItem* pEquip = GetItem( idEquip ,false);
	if(!pEquip)
		pEquip = GetEquipItem(idEquip);

	if(!pEquip || !pEquip->IsCanEmbedEquip() || pEquip->IsLocked())
		return false;
	if (pEquip->GetTypeID()==144000)//տ¬��
		return false;
	//��ñ�ʯ
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
	//��Ƕʯ
	CItem* pStone = GetItem(idStone,false);
	if (!pStone || pStone->GetTypeID()!=610018 || pStone->IsLocked())
		return false;

	int nPos = pEquip->GetEmbedPos();
	if( 0 == nPos )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "װ����û�п���Ƕ��ʯ�Ŀ�");
		return false;
	}

	//�������װ��ֻ����Ƕ������ı�ʯ���������װ��ֻ����Ƕ������ı�ʯ
	if (!pEquip->IsRightGemType(pGem->GetTypeID()))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��ʯ������װ�����಻��");
		return false;
	}
	
	if (pEquip->IsHaveSameGemEmbed(pGem->GetTypeID()))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "һ��װ���ϲ�����Ƕ����ͬ��ı�ʯ");
		return false;
	}
	if (GetMoney()<COMP_EMBEDGEM_COST)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	int nGemLev = pGem->GetTypeID()%100;
	int nGemType = pGem->GetTypeID();
	//��Ǯ
	if (!SpendMoney(COMP_EMBEDGEM_COST, SYNCHRO_TRUE))
		return false;

	pEquip->SetGemStatus(nPos, pGem->GetTypeID());
	if (!pEquip->GetKidnap() && pGem->GetKidnap())//�����ʯ�ǰ󶨵ģ�����Ƕ���װ��Ҳ��
		pEquip->SetKidnap(1);
	bool bIsCostly = pGem->IsCostlyItem();
	bool bIsCostStone = pStone->IsCostlyItem();
	int nStoneType = pStone->GetTypeID();
// 	ASSERT(EraseItem(idGem, SYNCHRO_TRUE));//ɾ����ʯ
// 	ASSERT(EraseItem(idStone, SYNCHRO_TRUE));//ɾ����Ƕʯ
	if (!SpendItem(pGem) || !SpendItem(pStone))
		return false;

	CMsgCompound msg;//���ͳɹ���Ϣ
	IF_OK( msg.Create( MSGCOMPOUND_EMBED_SUC, idEquip, 0 ) )
		this->SendMsg(&msg);
	SendSysMsg("��Ƕ�ɹ�");
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
				sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			else
				sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());		
		}
		else
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());

		sprintf(szMsg, "<%s>��<%s>��Ƕ��һ��<1_[%s]_0_%d_0_0_%d>�����������װ��������!",
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
	if (bIsCostly)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), nGemType);
	if (bIsCostStone)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), nStoneType);
	//֪ͨ�ͻ��˸�����Ʒ��Ϣ
// 	CMsgItem	msgItem;
// 	msgItem.Create(ITEMACT_SENDINFO,0);
// 	msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 	this->SendMsg(&msgItem);
	return true;
}

//////////////////////////////////////////////////////////////////////
//�ӿ���ȡ�����һ������ı�ʯ..
//06.6.6�����޸�
//[2010-07-21 goto]��ָ���Ŀ���ȡ����ʯ
bool CUser::TakeOutGem( OBJID idEquip, OBJID stoneID, int nPos )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
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
	if (pEquip->GetTypeID()==144000)//տ¬��
		return false;

	CItem* pStone = GetItem(stoneID,false);
	if (!pStone || pStone->GetTypeID()!=610019 || pStone->IsLocked())//�Ƿ���ȡ��ʯ
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
		SendSysMsg(_TXTATR_TOPSHOW, "ָ���Ŀ���û�б�ʯ");
		return false;
	}

	int nNeedMoney = COMP_TAKEOUTGEM_COST;
// 	if(nPos == 4)
// 		nNeedMoney = 100000;//10J

	if (GetMoney()<nNeedMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	if (m_pPackage && m_pPackage->IsPackFull(1, nStatus))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��������");
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
	CItem* pGem = AwardItem(nStatus, true, false, true, false, 1, false, true, false, 1);//ȡ���ı�ʯ�ǰ󶨵�
	if (pGem)
	{
		CMsgCompound msg;//���ͳɹ���Ϣ
		IF_OK( msg.Create( MSGCOMPOUND_EMBED_TAKEOUTGEM_SUC, idEquip, 0 ) )
			this->SendMsg(&msg);
		SendSysMsg("ȡ����ʯ�ɹ�");
		int nPos = pEquip->GetPosition();
		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
		if (nStatus%100>2)
		{
			char szMsg[255]		   ;
			char UserName[128] = "";
			char itemName[128] = "";
			char gemName[128]  = "";

			if(pEquip->IsWeaponSoul())//������⴦��
			{
				if(pEquip->GetWpSlType() == 1)
					sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
				else
					sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());		
			}
			else//�����
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
			sprintf(szMsg, "<%s>��<%s>�ϵ�<%s>ժȡ��ʱ���貵Ĺ�����������!",
				UserName, itemName, gemName);
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, idEquip&pGem->GetID(), pEquip->GetTypeID()&pGem->GetTypeID(), NULL, ALLUSERS_NAME);
		}
		if (pGem->IsCostlyItem())//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from gem_remove", GetID(), GetName(), nStatus);
		if (bIsCostly)//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from gem_remove", GetID(), GetName(), nStoneType);
		//֪ͨ�ͻ��˸�����Ʒ��Ϣ
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
		::LogSave("ժ��ʯ�����ʯʧ��.userid=%u,itemtype=%u", GetID(), nStatus);
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
//06.8.30�����޸�
//���;õ������ֵ��10%ʱ,ÿһ�������;ö���������ʾ
//���־�Ϊ��ʱ,������,���ǲ��ӹ�,���ȵ�......
void CUser::AddEquipmentDurability(int nPosition, int nAddValue)
{
	return;

	if( nAddValue >= 0 )
		return;

	CItem* refpEquip = GetEquipItemByPos(nPosition);
	if (!refpEquip || refpEquip->IsEquipEnable() )
		return;

	BOOL bIsValidOld = refpEquip->IsValidEquip();//�޸�ǰ��Ч��

	int nMaxDur = refpEquip->GetInt( ITEMDATA_AMOUNTLIMIT );
	int nOldDur	= refpEquip->GetInt( ITEMDATA_AMOUNT );
	int nDurability = __max( 0, nOldDur + nAddValue );
	refpEquip->SetInt( ITEMDATA_AMOUNT, nDurability);

	if ( nDurability <= nMaxDur / 10  )
	{
		//20070625����.��systemƵ��.
		//[2009.4.2--�Ŷ�]�����־�
		if (99 != int(refpEquip->GetTypeID()/10000))
		{
			this->SendSysMsg( _TXTATR_SYSTEM,g_objStrRes.GetStr(15077) , refpEquip->GetStr( ITEMDATA_NAME ),nDurability/100 );//"װ��%s���;û�ʣ%d��,�뾡������!"	
		}	
	}
	
	//-----���---2007/06/07----
	//if ( nOldDur != nDurability || nDurability/100 == 0 )// should update info to client (nDurability%100) == 0
	if(nDurability/100 != nOldDur/100)//20070629����.nDurability/100ʱ��ͬ��
	{
		CMsgItem msg;	// if nDurability equal zero, the client will auto delete this equipment
		if (msg.Create(nPosition, ITEMACT_DURABILITY, nDurability))
			this->SendMsg(&msg);		

		//2007829����:��װ�;�Ϊ0ʱ����
		//2007918����:���δ˹���
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
//			EraseEquip(nPosition, SYNCHRO_TRUE);//ͬ��
//
//			UpdateCalculatedProperty();//���ý�ɫ��
//
//		}
//		else
		{
			
			if(refpEquip->IsValidEquip() != bIsValidOld)//20070607����:��Ч�Ե�ת��
				UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
			
			refpEquip->SaveInfo();
		}
	}
}

//////////////////////////////////////////////////////////////////////
//20061023����.����ʰȡʱ,������Ʒ����.�Կͻ����ж�Ϊ��.
//nDonotCareDistanceΪ����ʰȡ��Χ.��ͻ��˶���һ��.
//20070206����:�������..�жϽṹ�����ı�.
bool CUser::PickMapItem(OBJID id,OBJID idItem,int nDonotCareDistance/*=0*/)//idItem �п��ܲ�����Ʒ����ʵtype���������ӵ���
{
 	int step = 1;
	DEBUG_TRY
	CMapItem* pMapItem = MapManager()->QueryMapItem(id);
	if (!pMapItem)
		return false;
	if (this->isFatigue() && this->isZeroP())
	{
		MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());
		SendSysMsg("���ѽ��벻������Ϸʱ�䣬��ͨ����������֤�޷������Ʒ��");
		return false;
	}
	step = 2;
 	if (this->GetDistance(pMapItem->QueryMapThing()) > nDonotCareDistance)
	{
		this->SendSysMsg("��Ʒ����̫Զ");//STR_FAR_CANNOT_PICK
		return false;
	}

	step = 3;

	CHECKF(pMapItem->GetOwner());
	//if(pMapItem->GetOwner()->GetID() != GetID())
	if (pMapItem->GetOwnerID() != GetID())
	{
		this->SendSysMsg("��Ӧ�ÿ����������");
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
				//����һ���Ʒ ��ֻ�ð�
				bool bKindnap = false;
				if(pMapItem->m_setItemType[i] >= 610030 && pMapItem->m_setItemType[i] <= 610400)
					bKindnap = true;

				if (m_pPackage->IsPackFull(1,pMapItem->m_setItemType[i],m_pPackage->GetAvailablePosition(pMapItem->m_setItemType[i])))
				{
					this->SendSysMsg("��������");
					return false;
				}
			//	bPickup = AwardItemCount(pMapItem->m_setItemType[i],1,true,0,bKindnap,pMapItem->m_nBossType);
				CItemPtr pItem =AwardItem(pMapItem->m_setItemType[i],true,false,true,true,1,true,true,false,bKindnap,pMapItem->m_nBossType,0);
				if(pItem)
				{	
					bPickup=true;
					if ((ItemType()->GetMonoly(pMapItem->m_setItemType[i])  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
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
// 		//���عһ�ʱδ����ʰȡ�������ż����������
// 		if(infoBook.idType<605000 && infoBook.idType>=601000)
// 		{
// 			pMapItem->SetLife();
// 			return false;
// 		}
// 	}
// 
// 	//20061023����------------------------------
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
// 	if (pMapItem->IsPriv() && idOwner != GetID())//��������ʱ��δ���������Ҳ���������Ʒ
// 	{
// 		//20070206����:С��ʰȡ����
// 		//�����С����Ʒ,�������ڴ�С��---------------
// 		OBJID teamID = pMapItem->GetTeamID();
// 		CTeam *pTeam = this->GetTeam();
// 		if(pTeam //�ж�
// 			&& pTeam->GetID() == teamID//�Լ�Ҳ���ڴ˶�
// 			&& ( pMapItem->IsMoney()//Ǯ...��ô����ʰ
// 				|| (!pMapItem->IsMoney() && (pTeam->GetItemMode()<<4) & 0x10)//��Ǯ,��Ʒ ������� �� ��Ʒ ����ʰȡ
// 				) 
// 			)
// 		{
// 			//��������㷨...�ں���.�˴�������������ȥ��
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
// 		//20070206����:�������-----------------------------------
// 		OBJID teamID = pMapItem->GetTeamID();
// 		CTeam *pTeam = this->GetTeam();
// 		if(pTeam //�ж�
// 			&& pTeam->GetID() == teamID//�Լ�Ҳ���ڴ˶�
// 			)
// 		{
// 			//��Ǯƽ�������㷨--------------------
// 			//��Ӧ���ڸ��߲�ȥ����,����UserManager.����Ϊͬ������.
// 			//һ��User����������User
// 			int memberAmount = pTeam->GetMemberAmount();
// 			int nMoney = pMapItem->GetAmount()/memberAmount;//ÿ����Ա��õ�Ǯ
// 			MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());//��ɾ�����Է�����
// 
// 			
// 			for(int i=0;i<memberAmount;i++)
// 			{
// 				CUser *pUser =UserManager()->GetUser(pTeam->GetMemberByIndex(i));
// 				if(pUser)//�ڱ���ͼ��
// 				{
// //					if (this->GetDistance(pUser->GetPosX(),pUser->GetPosY())>_RANGE_EXPSHARE)
// //						continue;
// 					if (pUser->GetMoney() + nMoney > _MAX_MONEYLIMIT)
// 					{
// 						pUser->SendSysMsg(g_objStrRes.GetStr(14043));//STR_TOOMUCH_MONEY
// 						continue;//��һ��
// 					}
// 					//��ý�Ǯ
// 					BOOL bRet = pUser->GainMoney(nMoney, SYNCHRO_TRUE);
// 					//////////////////////////////////////////////////////////////////////////
// 					//	20070710	���	������Ƚ�Ǯ��¼
// 					if(bRet && CItem::IsLogSaveMoney(nMoney))
// 					{
// 						MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 							this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_PICK, 
// 							0, 0, "ʱ�ձ�", nMoney);
// 					}
// 					//	���� 20070710
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
// 				else//���ڱ���ͼ��
// 				{
// 					MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_PICKMONEY, 
// 			    	pTeam->GetID(), 
// 			    	pTeam->GetMemberByIndex(i),//idUser
// 		        	nMoney);//nData  ��ʾ��Ǯ
// 				}
// 			}
// 			//��Ǯƽ�������㷨end----------------------
// 		}
// 		else//��ͳ��Ǯʰȡģʽ
// 		{
// 			int nMoney = pMapItem->GetAmount();
// 			
// 			if (this->GetMoney() + nMoney > _MAX_MONEYLIMIT)
// 			{
// 				this->SendSysMsg(g_objStrRes.GetStr(14043));//STR_TOOMUCH_MONEY
// 				return false;
// 			}
// 			MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());		//??? ��ɾ�����Է�����
// 			BOOL	bRet = this->GainMoney(nMoney, SYNCHRO_TRUE);
// 			//////////////////////////////////////////////////////////////////////////
// 			//	20070710	���	������Ƚ�Ǯ��¼
// 			if(bRet && CItem::IsLogSaveMoney(nMoney))
// 			{
// 				MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 					this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_PICK, 
// 					0, 0, "ʱ�ձ�", nMoney);
// 			}
// 			//	���� 20070710
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
// 	else			//����Ǯ...
// 	{
// 		//20070207����:�������-----------------------------
// 		CUser *pUser=NULL;//�㷨֮���ӵ����
// 
// 		OBJID teamID = pMapItem->GetTeamID();
// 		CTeam *pTeam = this->GetTeam();
// 		if(pTeam //�ж�
// 			&& pTeam->GetID() == teamID//�Լ�Ҳ���ڴ˶�
// 			)
// 		{
// 			if((pTeam->GetItemMode()<<4) & 0x40)//�������
// 			{
// 				//��Ʒ ��������㷨
// 				//������䵽С�ӳ�Ա
// 				//����˳�Ա���ڱ���ͼ��,��ָ�this
// 				int rndTemp = ::RandGet(pTeam->GetMemberAmount());
// 				pUser = UserManager()->GetUser(pTeam->GetMemberByIndex(rndTemp));
// 				if(!pUser || GetDistance(pUser->GetPosX(),pUser->GetPosY()>_RANGE_EXPSHARE))//���ڱ���ͼ��
// 					pUser = this;
// 			}
// 			else if((pTeam->GetItemMode()<<4) & 0x20)//����ʰȡ
// 			{
// 				//��Ʒ ����ʰȡ�㷨
// 				//�κ��˶���ʰ.�Լ�����ʰ
// 				pUser = this;
// 			}
// 			else if((pTeam->GetItemMode()<<4) & 0x10)
// 			{
// 				//��Ʒ ����ȡ���㷨
// 				//[����ȡ��]���ߵ�����,ǰ���Ѿ������ж�.ln:2634
// 				//ûʲô�ر��
// 				pUser = this;
// 			}
// 			else
// 			{
// 				//error!
// 				LOGERROR( "CUser::PickMapItem��������.�����:%s GetItemMode:%d",this->GetName(),pTeam->GetItemMode() );
// 				return FALSE;
// 			}
// 
// 		}
// 		else//��ͳ����ģʽ
// 		{
// 			pUser = this;
// 		}
// 		//�������
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
// 		//	20070712	���	����Ƿ��Ƕ�ռ��Ʒ
// 		//	ItemInfoStruct info1;
// 		//	ItemType()->GetInfo(pMapItem->GetInfo()->idType, &info1);
// 		if(IsUserHaveAnExclusiveItem(pMapItem->GetInfo()->idType,
// 			NULL, this))
// 			return false;
// 
// 		DEBUG_TRY{
// 			//[��;���� 2009.04.21]
// 			DWORD dwStrengthanTimes = pMapItem->GetStrengthanTimes();
// 
// 			//[��;���� 2009.05.14]��ǰ
// 			MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());		//??? ��ɾ�����Է�����
// 			
// 			pItem = pUser->AwardItem(&info, 
// 				SYNCHRO_TRUE,
// 				true,
// 				false,
// 				false,
// 				true,
// 				true);//20070707����:���޸Ĵ�����
// 			
// 
// 			//[��;���� 2009.05.08]
// 			if (!pItem)
// 			{
// 				char szTemp[256];
// 				sprintf(szTemp, "CUser::PickMapItem ������Ʒʧ�� TypeId = %d name = %s id = %d", info.idType, info.szName, info.id);
// 				::LogSave(szTemp);
// 				return false;
// 			}
// 			else
// 			{
// 				//[��;���� 2009.04.21]
// //				pItem->SetInt(ITEMDATA_STRENGTHAN_TIMES, dwStrengthanTimes, true);
// 			}
// 
// 			//////////////////////////////////////////////////////////////////////////
// 			//	20070709	���	����������Ʒ������־ ��ʰ
// 			if(pItem && pItem->IsCostlyItem())
// 			{
// 				ItemInfoStruct info;
// 				ItemType()->GetInfo(pItem->GetTypeID(), &info);
// 				MyLogSaveCostlyItem(NULL, this->GetAccountID(), this->GetID(),
// 					this->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_PICK, 
// 					pItem->GetTypeID(), pItem->GetID(), info.szName,   
// 					CItem::IsCountable(pItem->GetTypeID()) ? info.nAmount : 1);  
// 			}
// 			//	���� 20070709
// 			//////////////////////////////////////////////////////////////////////////
// 
// 			//  [3/5/2008 ���]	д����־���ټ��,�����Ʒ��ɾ�����ָ��д��־������
// 
// 			//////////////////////////////////////////////////////////////////////////
// 			//  [2007-9-19] ���	���ϸ�����������
// 			CheckItemLifeTime(pItem, TRUE, TRUE);
// 		}DEBUG_CATCH("CUser::PickMapItem")
// 
// 		if(pItem)	//�ӵ�ͼ�ϼ���Ʒ
// 		{
// 			///�񵽶������������,�ͽ���ȫ��˵���㲥
// 			if (pTeam)
// 			{
// 				CMsgItem msg;
// 				UINT nItemCount=1;
// 				if (CItem::IsCountable(pItem->GetTypeID()))
// 					nItemCount=info.nAmount;
// 				if (msg.Create(this->GetID(),ITEMACT_TEAMPICKUP,nItemCount,pItem->GetTypeID(), nItemCount, pItem->GetID()))
// 					pTeam->BroadcastTeamMsg(&msg);
// //				char strTeamMsg[255]="";
// //				sprintf(strTeamMsg,"%s����%s!",pUser->GetName(),pItem->GetStr(ITEMDATA_NAME));
// //				CMsgTalk msg;
// //				if (msg.Create((char*)pUser->GetName(),"ALLUSERS",strTeamMsg,NULL, 0XFF0080C0, _TXTATR_TEAM))
// //					pTeam->BroadcastTeamMsg(&msg,pUser);
// 			}
// 
// 			//������ʱ��֮��
// 			if( this->GetMapID() == g_ResourceWar.GetWarMapID() )
// 			{
// 				if( pItem->GetInt( ITEMDATA_TYPE ) ==  g_ResourceWar.GetSKZLType() )//��ʾ����ʱ��֮��
// 				{
// 				
// 					//����ͼ�㲥��Ϣ
// 					char szMsg[ 128 ] = { 0 };
// 					char szPhyle[ 32 ] = { 0 };
// 
// 					// �����޸�   08.6.12  ��� 
// //					DWORD dwProfession = this->GetProfession();
// //					if( dwProfession & PROF_DRAGON )
// //					{
// //						strcpy( szPhyle, g_objStrRes.GetStr(15046));//"����"
// //					}
// //					else if( dwProfession & PROF_GENE )
// //					{
// //						strcpy( szPhyle, g_objStrRes.GetStr(15048));//"������"
// //					}
// //					else
// //					{
// //						strcpy( szPhyle, g_objStrRes.GetStr(15047));//"������"
// //					}
// 					sprintf( szMsg,g_objStrRes.GetStr(15078),szPhyle,this->GetName() );//"ʱ��֮����Ƭ��%s��%s��á�"
// 					g_ResourceWar.BroadcastLocalMapMsg( szMsg );
// 					//ʱ��֮����Ƭ��XX���XX����ң���á�"
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

	m_pSquareDeal->Release();		// �رս���ϵͳ
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

//���Է������Ƿ��пռ�,06.9.22����ע������(��ʱ��δ���ף�
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
	//[��;���� 2009.05.14]ǿ�иı���Ʒλ�õ��·�����״̬λ����,����,���鵽�ͻ����޸�.
	/*const int iPostion = 100;//�����ڽ���ʱ��ʾ�� λ��Ϊ[��]
	pItem->SetPosition(iPostion,false);*/
	//[2010-06-03 goto] ���ͽ�����Ʒ����Ϣ
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

	//[��;���� 2009.06.05]��������ʲô?�е�����.
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
	if(msg.Create(PETACTION_TradePet,&Data))//action=11 �����жԷ������˳���
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

   	//idUser = STORAGE_NPC_ID;		//�ֿ⻥ͨ
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
	//idNpc	= STORAGE_NPC_ID;		//�ֿ⻥ͨ
// 	int nIndex = 1;
// 	if (idNpc>0 && idNpc<=3*PACKAGE_LIMIT)
// 		nIndex = (idNpc-1)/PACKAGE_LIMIT +1;//�������ҳ��
// 	else
// 		nIndex=idNpc%10;//ȡ��λ,��λ�ǵ�ǰ�ֿ�ҳ��
//  	CHECKF(m_pStorage->Create(ITEMPOSITION_STORAGE,GetID(),GetID(),nIndex, Database()));

	//  [3/5/2008 ���]	����ֿ�ʱ������,�ڵ���CHECKIN�ǵ�,��Ʒ�Ѿ��ӱ������ﵯ��,���Լ��������,Ҳ�޷�ɾ��
	//	ͳһ��ҵ����Ʒ�ӱ����ó�ʱ����
	
	//  [2007-10-9] ���	�����������
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
//�ֿ���Ʒ�ĺϲ���������вֿ������ļ��
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

//	idNpc   = STORAGE_NPC_ID;//�ֿ⻥ͨ
// 	CItem* pItemTarget = this->GetItemByIdItem(idItem);
// 	if (!pItemTarget)
// 		return NULL;
// 	int nPackageIndex = (pItemTarget->GetPackageIndex()-1)/PACKAGE_LIMIT +1;//�������ҳ��
 	//CHECKF(m_pStorage->Create(nPosition, idNpc, GetID(), Database()));
	//CHECKF(m_pStorage->Create(ITEMPOSITION_STORAGE,GetID(),GetID(),nPackageIndex, Database()));

//	return m_pStorage->CheckOut(this, idItem, bUpdate);
	CItem* pItem = m_pStorage->CheckOut(this, idItem, nIndex, bUpdate);

	//  [3/5/2008 ���]	�������,��Ϊ,�ڵ���CHECKOUTʱ,��Ʒ��δ���뱳��,���Լ��������,Ҳ�޷�ɾ��
	//	EraseItem��ɾ��������Ʒ
	//	����CMsgPackage.Process.�д���

// 	BOOL	bItemDeleted = FALSE;
// 
// 	if(pItem)	//  [2007-10-9] ���	�����������
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
		//����
		IMagicData* pMagic = QueryMagic(6001);
		if(!pMagic || pMagic->GetMagicLevel() < nLev)
			return false;

		m_nMineType = 1;
	}
	else if(GetMap()->isMineMap())
	{
		//����
		IMagicData* pMagic = QueryMagic(6002);
		if(!pMagic || pMagic->GetMagicLevel() < nLev)
			return false;

		m_nMineType = 2;
	}
	else if(GetMap()->isHuntMap())
	{
		//����
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
			SendSysMsg("�������㣬ֹͣ�ɼ�");
			this->StopMine();
			return;
		}

		//��òɼ���
		//�ȼ�
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
				//����
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

				//����
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

				//����
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

		//��ͨ���
		OBJID idNormal = (660 + m_nMineType) * 1000 + nLev;

		//������
		OBJID idOther = (660 + m_nMineType) * 1000 + 100 + ((nLev - 1) / 2 + 1);

		if(RandGet(10000) < 8000)
		{
			CItem* pItem = NULL;
			m_pMagic->AwardExp(nSkill);
			pItem = AwardItem(idNormal,true);
			if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from mine", GetID(), GetName(), idNormal);
			if(RandGet(10000) < 100)
			{
				pItem = AwardItem(idOther,true);
				if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from mine", GetID(), GetName(), idOther);
			}
		}
		else
			SendSysMsg("�ɼ�ʧ��");

		

		m_data.SetVit(nVit-1);

		CMsgUserAttrib msgAtt;
		if(msgAtt.Create(GetID(),_USERATTRIB_VIT,m_data.GetVit()))
			SendMsg(&msgAtt);

		if(nVit < 1)
		{
			SendSysMsg("�������㣬ֹͣ�ɼ�");
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

	//�����޸�   08.6.12  ���
	switch( this->GetProfession() )
	{
	case PROF_MALE:	//���幭��	1
		armorID = 131010;
		break;
	}
	return armorID;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//06.10.28����
//����˵��:����ͻ�������ı�ͬһ�����е���Ʒ��λ��
//����˵��:idItem ��������Ʒid,nWhichPackage����һ��������,nWant2PackageIndexĿ��index
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

		pUserPack->SetIndexState( ucPosition,ucOldPackageIndex,0 );//����ԭ��λ��Ϊ��
		pUserPack->SetIndexState( ucPosition,nWant2PackageIndex,1 );//������λ��Ϊ��

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
//06.10.30����
//����˵��:����ͻ�������,�ı�һ����Ʒ�ڲ�ͬ�ı����е�λ��(ʵ��Ϊ�ı�Position and Package_index )
//����˵��:idItem ��ƷID,nWhichPackage����һ��������ȡ��(0Ϊ��һ������)
//nWant2Position������뱳����Position,nWant2PackageIndex������뱳����λ��
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
	if( nHasFlag != 0 )//Ŀ�ĵ�������Ʒ( > 0 or < 0 both error)
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
	if( !pItem )//����ʧ��
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
		//�ɹ�
		CMsgItem msg;
		IF_OK( msg.Create( idItem,ITEMACT_EXCHANGEPOSOK,pItem->GetPosition(),pItem->GetPackageIndex()/*nWant2Position,nWant2PackageIndex*/ ) )
			SendMsg( &msg );
	}
	DEBUG_CATCH( "CUser::ProcessChgItemPosAndIdx Catch an Error!" )
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//09.3.26
//�Ŷ�
//����˵��:����ͻ�������,������Ʒ�ڲ�ͬ�ı����е�λ��(ʵ��Ϊ�ı�Position and Package_index )
//����˵��:idItem1 ��ƷID,idItem2 ��ƷID,nWhichPackage����һ��������ȡ��(0Ϊ��һ������)
//nWant2Position������뱳����Position,nWant2PackageIndex������뱳����λ��
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
	if( !pItem1 || !pItem2 )//����ʧ��
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
		//�ɹ�
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
//����
//����˵��:����ͻ�������,�������������е���Ʒλ��(��ֻ֧��ͬһ�������л������)
//����˵��:������Ʒ��ʶ
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
	//����PACKAGE_INDEX
	pItem1->SetPackageIndex( ucIndex2 );
	pItem2->SetPackageIndex( ucIndex1 );

	//ͬ���ͻ���
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
	//���ѧ���䷽��
	CUserPeifangData* pPeifang = m_pUserPeifang->QueryData(idPeifang);
	int bKindnap = 0;	
	if(!pPeifang)
	{
		SendSysMsg("�㻹û��ѧ���Ӧ���䷽");
		return - 1;
	}


	//����䷽
	CPeifangTypeData*	pPeifangType = PeifangType()->GetObj(idPeifang);
	if(!pPeifangType)
	{
		SendSysMsg("�䷽������");
		return - 1;
	}

	//���ene
	int nSpendVit = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDVIT);
	if(GetVit() < nSpendVit)
	{
		SendSysMsg("��������");
		return - 1;
	}

	//������
	OBJID idSpend1 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM1);
	OBJID idSpend2 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM2);
	OBJID idSpend3 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM3);
	OBJID idSpend4 = pPeifangType->GetInt(PEIFANGTYPEDATA_SPENDITEM4);

	//��ɫ�Լ����ϵ�,��Ҫ�жϰ�ȫ��
	DEBUG_TRY
	if (IsItemLockedOpen())
	{
		if(idSpend1)
		{
			CItem* pItemTemp1 = GetItemByType(idSpend1);
			if(pItemTemp1 == NULL)
			{
				::LogSave("�յ�һ�������ڵĺϳɲ���, UserName = %s, type = %u", GetName(), idSpend1);
				return -1;
			}
			if ( pItemTemp1->GetColour() >2 )
			{
				SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
				return -1;
			}
		}
		if(idSpend2)
		{
			CItem* pItemTemp2 = GetItemByType(idSpend2);
			if(pItemTemp2 == NULL)
			{
				::LogSave("�յ�һ�������ڵĺϳɲ���, UserName = %s, type = %u", GetName(), idSpend2);
				return -1;
			}
			if ( pItemTemp2->GetColour() >2 )
			{
				SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
				return -1;
			}
		}
		if(idSpend3)
		{
			CItem* pItemTemp3 = GetItemByType(idSpend3);
			if(pItemTemp3 == NULL)
			{
				::LogSave("�յ�һ�������ڵĺϳɲ���, UserName = %s, type = %u", GetName(), idSpend3);
				return -1;
			}
			if ( pItemTemp3->GetColour() >2 )
			{
				SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
				return -1;
			}
		}
		if(idSpend4)
		{
			CItem* pItemTemp4 = GetItemByType(idSpend4);
			if(pItemTemp4 == NULL)
			{
				::LogSave("�յ�һ�������ڵĺϳɲ���, UserName = %s, type = %u", GetName(), idSpend4);
				return -1;
			}
			if ( pItemTemp4->GetColour() >2 )
			{
				SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
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
			SendSysMsg("���ϲ���");
			return - 1;
		}
	}

	if((idSpend2 != ID_NONE) && (spendNum2 > 0))
	{
		if(GetItemNumByType(idSpend2) < spendNum2)
		{
			SendSysMsg("���ϲ���");
			return - 1;
		}
	}

	if((idSpend3 != ID_NONE) && (spendNum3 > 0))
	{
		if(GetItemNumByType(idSpend3) < spendNum3)
		{
			SendSysMsg("���ϲ���");
			return - 1;
		}
	}

	if((idSpend4 != ID_NONE) && (spendNum4 > 0))
	{
		if(GetItemNumByType(idSpend4) < spendNum4)
		{
			SendSysMsg("���ϲ���");
			return - 1;
		}
	}

	//���Ӳ���&&����
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
		SendSysMsg("���Ӳ��ϲ���");
		return - 1;
	}

	//����Ʒ֮ǰ���ж�һ�¸ò��ð�
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
	//����Ʒ
	if(AwardItemByCreate(idGet1,true,true,true,getNum1,bKindnap,nAddType))
	{
		bool bHasKind = false;
		//ɾ����Ʒ
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

		//�ۻ���
		if(nSpendVit > 0)
			AddVit(-1 * nSpendVit);

		//����������
		IMagicData* pMagic = QueryMagic(nSkillType);
		if(pMagic && pMagic->GetMagicLevel() <= nSkillLev)
		{
			m_pMagic->AwardExp(nSkillType);
		}

		if (idAdditem && (ItemType()->GetMonoly(idAdditem)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from craft", GetID(), GetName(), idAdditem);
		if (idGet1 && (ItemType()->GetMonoly(idGet1)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from craft", GetID(), GetName(), idGet1);

		return 1;
	}
	return -1;
}
// 
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BOOL CUser::WeaponKillEgg(  OBJID equipID, OBJID stoneID )// ������� pengfeng [11/6/2007]
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
// 	ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );//-------���---2007/04/10-ɾ��ʯͷ��----
// 
// 	//20071214����:ͬ���޸�-----------
// //	CMsgItemInfo msg;
// //	IF_OK (msg.Create(pWeapon, ITEMINFO_UPDATE))
// //	this->SendMsg(&msg);
// 	this->SynchroItem(pWeapon,ITEMINFO_UPDATE);
// 	//--------------------------------
// 
// 	DEBUG_CATCH( "CUser::WeaponKillEgg( OBJID equipID,OBJID stoneID )" );
// 	return true;
// }


BOOL CUser::IsPickUpKidnap( int nItemType )//-------���---2007/05/09-----�Ƿ�ʰȡ��----------
{
	if( 6001 == nItemType / 100)
		return true;
	else if(530 == nItemType / 1000)
		return true;
	else
		return false;
}

//BOOL CUser::EquipMakeHole( OBJID equipID, OBJID stoneID,OBJID* pDataSet,int nSize )// װ���� [6/20/2007]
BOOL CUser::EquipMakeHole( OBJID equipID, OBJID stoneID )// װ���� [2010-07-20 goto]
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return FALSE;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
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

	if (pEquipItem->GetTypeID()==144000)//տ¬��
		return false;
	CItem* pStone = GetItem(stoneID,false);
	if (!pStone || !pStone->IsMakeHoleStone() || pStone->IsLocked())
		return false;
	bool bIsCostly = pStone->IsCostlyItem();
	int nStoneType = pStone->GetTypeID();
	int nHoleCount = pEquipItem->GetEmbedCountLimit();
	if (nHoleCount>=3)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��װ������û�пɴ�׵�λ��");
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
		SendSysMsg(_TXTATR_TOPSHOW, "��װ������û�пɴ�׵�λ��");
		return false;
	}

	int nCostMoney = COMP_MAKEHOLE_COST * (nHoleCount+1);
	if (GetMoney()<nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}

	if (pEquipItem->GetReqLev() > (pStone->GetTypeID()%100+1)*10)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "����뼶�𲻷�");
		return false;
	}
// 	if (pEquipItem->GetInt(ITEMDATA_COLOR)!=(pStone->GetTypeID()%10+1))
// 	{
// 		SendSysMsg(_TXTATR_TOPSHOW, "���ʯ����ɫ��װ������ɫ����");
// 		return false;
// 	}
	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//�۳�����
		return false;

	//����ɹ���
	int nSucceedProbability = 0;
	if (nHoleCount==0) nSucceedProbability=100;//��һ����100%
	if (nHoleCount==1) nSucceedProbability=50;//�ڶ�����50%
	if (nHoleCount==2) nSucceedProbability=25;//����������25%

	bool bBind = false;
	if (pStone->GetKidnap())
		bBind = true;
	//ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );//ɾ�����ʯ
	if (!SpendItem(pStone))
		return false;
	if (::RandGet(100)<nSucceedProbability)//�ɹ�
	{
		if (pEquipItem->CreateHole())
		{
			if (bBind && !pEquipItem->GetKidnap())
				pEquipItem->SetKidnap(1);
			//֪ͨ�ͻ��˸�����Ʒ��Ϣ
// 			CMsgItem	msgItem;
// 			msgItem.Create(ITEMACT_SENDINFO,0);
// 			msgItem.Add(equipID,0,0,0, 0,0,0);
// 			this->SendMsg(&msgItem);
			SendSysMsg("��ϲ�㣬��׳ɹ�");
			return nHoleCount+1;
		}
	}
	else
		SendSysMsg("���ź������ʧ��");

	if (bIsCostly)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, nStoneType);
	//if( 0 == pEquipItem->GetTypeID()%10 )//�������ǰ�װ
	//	return false;
	//[��;���� 2009.04.08]����
	/*
	CItem* pStoneItem = GetItem( stoneID );
	if( !pStoneItem )
		return false;
	if( !pStoneItem->IsMakeHoleStone() )
		return false;
	int lev = pEquipItem->GetReqLev();
	int nStoneTypeID = pStoneItem->GetTypeID();

	//  [1/23/2008 ���]	���˱�ʯ
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
		nOmnID = pOmnipotence->GetTypeID();// �жϵ�����λ����Ʒ�Ƿ�Ϊ����ʯ [�½���11/6/2007]
		nTmpID = pOmnipotence->GetID();
	}
	*/

	// ���汾֮ǰӦ�߻�ǿ��Ҫ����ģ����ж�Ʒ�ʡ���� [6/27/2007]
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

		//[��;���� 2009.04.08]����
		//int nSucceedProbability = 0;

		//----�������ʯͷ�Ķ�Ӧ����--------------
		/*��⵱ǰװ�����������еĿף�
		0����50���ɹ��ʣ���
		1����35���ɹ��ʣ���
		2����25���ɹ��ʣ���
		3����10���ɹ��ʣ���
		4�������ܴ�ף���*/

		//[��;���� 2009.04.08]����
		//const int  nSmallSucceedProbabilityArray[5] = { 50, 30, 10, 2, 0 };

		//----�߼����ʯͷ�Ķ�Ӧ����--------------
		/*��⵱ǰװ�����������еĿף�
		0����100���ɹ��ʣ���
		1����70���ɹ��ʣ���
		2����50���ɹ��ʣ���
		3����25���ɹ��ʣ���
		4�������ܴ�ף���*/

		//[��;���� 2009.04.08]����
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
		
	//	if( RandGet( 100 ) < nSucceedProbability && nEmbedCountLimit < 4 )//�ɹ���

		//[��;���� 2009.04.08]�޸�
		//bHasLuckyGem = true; //100%�ɹ�

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
// 				//ɾ����ʯ
// 				//ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );
// 				//[��;���� 2009.04.08]����
// 				//ASSERT(DelItemByTypeMulti(stoneID, 1, true));
// 				return false;
// 			}
			//[��;���� 2009.04.08]����
			/*
			DEBUG_TRY		// VVVVVVVVVV
				//ɾ����ʯ
				//ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );
				ASSERT(DelItemByTypeMulti(stoneID, 1, true));
			DEBUG_CATCH("stoneID")// AAAAAAAAAAA
			*/

			//[��;���� 2009.04.08]����
			/*if( pOmnipotence )//  [11/24/2007 dio]
			{
				ASSERT( EraseItem( nTmpID, SYNCHRO_TRUE ) );
			}*/
		
			// update item info at client
			//20071214����:ͬ���޸�-----------
//			CMsgItemInfo msg;
//			IF_OK (msg.Create(pEquipItem, ITEMINFO_UPDATE))
//				this->SendMsg(&msg);			
// 			this->SynchroItem(pEquipItem,ITEMINFO_UPDATE);
// 			//--------------------------------
// 			return true;
// 		}
// 		else
//		{
			//ɾ����ʯ
			//ASSERT( EraseItem( stoneID, SYNCHRO_TRUE ) );
			//[��;���� 2009.04.08]����
			//ASSERT(DelItemByTypeMulti(stoneID, 1, true));
			//[��;���� 2009.04.08]����
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
//20070226����:������װ���;���ʧһ���ٷֱ�.����ʵ����ʧ�;�װ���ĸ���
int	CUser::PackageDurationLose(int durationLosePercent)
{
	return m_pPackage->PackageDurationLose(durationLosePercent);
}


//-----��� 2007/03/02--- ��������
//[2010-07-19 goto]�޸Ĵ˺���Ϊװ��ǿ��
//goto �������˱��� 610048 ,bUseLuckyCharm��ʾ�Ƿ�ʹ��
void CUser::WeaponLevUp( OBJID idWeapon,short nGemCount,bool bUseLuckyCharm )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return;
	}
	//���װ��
	CItem* pEquip = GetItem(idWeapon,false);
	if(!pEquip)
		pEquip = GetEquipItem(idWeapon);

	if (!pEquip || !pEquip->IsCanUpLevel())
		return;
	if (pEquip->GetTypeID()==144000)//տ¬��
		return;
	if (pEquip->IsWeaponSoul())
		return;
	if (pEquip->IsLocked())
		return;

	int nOldLev = pEquip->GetEquipAddLev();
	if (nOldLev<0 || nOldLev >9)
	{
		if (nOldLev==COMP_MAX_EQUIPLEV)
			SendSysMsg(_TXTATR_TOPSHOW, "��װ���Ѿ������ǿ���ȼ����޷���ǿ��");
		return;
	}
	int nCostMoney = pEquip->GetEquipAddLevCost(1);
	//�ж�����
	if (GetMoney()<nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return;
	}

	int nNeedGemLev = pEquip->GetNeedGemLev(nOldLev);//��Ҫ�ķ��ȼ�
	//ǿ����Ҫ��ǿ����type
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
	int nCoseGem = nGemCount;//��Ҫ�۳���ǿ��������
	int nHasGemCount = GetItemByTypeMulti(idGemtype, nHasGemSize, idGemArr, true);
	if (!nHasGemCount)//������û��ǿ����
	{
		//if (!bAutoBuy)//��ʱ�����Զ�������
		{
			SendSysMsg(_TXTATR_TOPSHOW, "������û����Ӧ��ǿ����");
			return;
		}
// 		else
// 		{
// 			//��Ԫ����ǿ����ǿ��
// 			nCoseGem = 0;//ȫ����Ԫ�������۷�
// 			vasItem* pVasItem = VASItem()->GetItemByType(idGemtype);
// 			if (!pVasItem)
// 				return;
// 			int nPriceTotal = 0;//��Ʒ�۸�
// 			if (pVasItem->discount==0)//û���ۿ�
// 				nPriceTotal = (pVasItem->price*nGemCount);
// 			else//���ۿ�
// 				nPriceTotal = (pVasItem->price*nGemCount)*(pVasItem->discount/100.0);
// 
// 			if (GetMoneyByType(5)<nPriceTotal)
// 			{
// 				SendSysMsg(_TXTATR_TOPSHOW, "Ԫ������");
// 				return;
// 			}
// 
// 			SpendVas(nPriceTotal);
// 		}
	}
	if (nHasGemCount)//��������ǿ����
	{
		if (nHasGemCount<nGemCount)
		{
			//if (!bAutoBuy)//��ʱû���Զ��������
			{
				SendSysMsg(_TXTATR_TOPSHOW, "������ǿ������������");
				return;
			}
// 			else//�Զ�����ǿ����
// 			{
// 				nCoseGem = nHasGemCount;//������Ԫ���������÷�
// 				vasItem* pVasItem = VASItem()->GetItemByType(idGemtype);
// 				if (!pVasItem)
// 					return;
// 				int nPriceTotal = 0;//��Ʒ�۸�
// 				if (pVasItem->discount==0)//û���ۿ�
// 					nPriceTotal = (pVasItem->price*(nGemCount-nHasGemCount));
// 				else//���ۿ�
// 					nPriceTotal = (pVasItem->price*(nGemCount-nHasGemCount))*(pVasItem->discount/100.0);
// 				
// 				if (GetMoneyByType(5)<nPriceTotal)
// 				{
// 					SendSysMsg(_TXTATR_TOPSHOW, "Ԫ������");
// 					return;
// 				}
// 				
// 				SpendVas(nPriceTotal);
// 			}
		}
	}

	//����ɹ���
	float nSucceedProbability = pEquip->GetSucRate(1)*nGemCount;

	float nFailRate = pEquip->GetEquipAddLevFailRate()*1.0/1000;
	nSucceedProbability += nFailRate;//����ʧ�ܼӳ�
	if (bUseLuckyCharm)//ʹ�����˱��� 610048
	{
		CItem *pExtItem = GetItemByType(610048);
		if (!pExtItem)
			return;
		if (SpendItem(pExtItem))
			nSucceedProbability += 10;
		else
			return;
		if ((ItemType()->GetMonoly(610048)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, 610048);
	}
	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//�۳�����
		return;
	bool bBind = false;//ǿ�����Ƿ�󶨣����ʹ����ǿ���İ󶨷����װ��
	for(int nkidnap=0; nkidnap<2; nkidnap++)
	{
		if (nCoseGem)//����ʹ�ð󶨵�
		{
			DEBUG_TRY//try
			//ɾ��ǿ����
			for (int i=0; i<nHasGemSize; i++)
			{
				CItem* pGemItemTemp = GetItem(idGemArr[i],false);
				if (!pGemItemTemp)
					continue;
				if (!pGemItemTemp->GetKidnap() && nkidnap==0)//��һ��ѭ��ʱֻ�Ұ󶨵ķ�
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
	if (nVipLev==2)//VIP�ɹ��ʼӳ�
		nSucceedProbability += COMP_VIP_SUC_ADD2;
	else if (nVipLev==3)
		nSucceedProbability += COMP_VIP_SUC_ADD3;
	if (RandGet(100000) < nSucceedProbability*1000)//ǿ���ɹ������ǵ��ɹ�����С��������ȡ100000������������ѳɹ��ʳ���1000
	{
		pEquip->SetEquipAddLev(nOldLev+1);
		pEquip->UpdateEquipBaseProperty(nOldLev);//����װ����������
		if (nOldLev==3 || nOldLev==6/* || nOldLev==8*/)
			pEquip->SetEquipAddLevFailRate(0);//ʧ�ܼӳ�����

		if (bBind && !pEquip->GetKidnap())
			pEquip->SetKidnap(1);
		CMsgCompound msg;//���ͳɹ���Ϣ
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_LEVUP_SUC, idWeapon, pEquip->GetEquipAddLev() ) )
			this->SendMsg(&msg);
		
		this->SendSysMsg(_TXTATR_TOPSHOW,"��ϲ�㣬װ��ǿ���ɹ�");
		if (nOldLev+1>4)
		{
			char szMsg[255];
			char UserName[128] = "";
			char itemName[128] = "";
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			sprintf(szMsg, "<%s>����ǧ����࣬���ڽ�<%s>ǿ����<3_+%d_6>��",
				UserName, itemName ,nOldLev+1);
		
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);
		}
		if (nOldLev+1 == 7)//������Ч
		{
			CMsgPlayer msgPlayer;
			msgPlayer.Create(this);
			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
		int nPos = pEquip->GetPosition();
		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
		//֪ͨ�ͻ��˸�����Ʒ��Ϣ
// 		CMsgItem	msgItem;
// 		msgItem.Create(ITEMACT_SENDINFO,0);
// 		msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 		this->SendMsg(&msgItem);
	}
	else//ǿ��ʧ��
	{
		if (nOldLev>4 && nOldLev!=7)//ʧ�ܺ󽵼�
		{
			if (nOldLev>7)//ʧ�ܺ󽵼�
				pEquip->SetEquipAddLev(7);
			else if (nOldLev > 4 && nOldLev < 7)
				pEquip->SetEquipAddLev(4);

			pEquip->UpdateEquipBaseProperty(nOldLev);//����װ����������
			int nPos = pEquip->GetPosition();
			if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
			//֪ͨ�ͻ��˸�����Ʒ��Ϣ
// 			CMsgItem	msgItem;
// 			msgItem.Create(ITEMACT_SENDINFO,0);
// 			msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 			this->SendMsg(&msgItem);
		}

		pEquip->SetEquipAddLevFailRate(nFailRate*1000+0.5+COMP_SUC_FAIL_RATE*nGemCount);//ʧ�ܼӳ�
		CMsgCompound msg;//����ʧ����Ϣ
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_LEVUP_FAIL, idWeapon, pEquip->GetEquipAddLev() ) )
			this->SendMsg(&msg);
		SendSysMsg("���ź���װ��ǿ��ʧ��");
	}
	if ((ItemType()->GetMonoly(idGemtype)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), nGemCount, idGemtype);
// 	//�������
// 	CItem* pWeapon = GetItem( idWeapon);
// 	CHECK( pWeapon);
// 	if( !pWeapon->IsWeapon() )
// 		return;
// 	if( !pWeapon->IsCanUpLevel() )
// 		return;
// 	//��ñ�ʯ
// 	CItem* pGem	= GetItem( idGem );
// 	CHECK( pGem );
// 	if( !pGem->IsWeaponLevUpProperty() )
// 		return;
// 	if( pGem->GetWeaponPropertyLevel() !=  pWeapon->GetWeaponRank() )//��������ȼ���ʯͷ�ȼ���һ������������
// 		return;
// 	if( pWeapon->GetExp() < pWeapon->GetWeaponNeedExp() )//-----��� 2007/03/07--- �������鲻��
// 		return;
// 
// 	//  [1/23/2008 ���]	���˱�ʯ
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
// 	//-----------�ɹ������ж�------------------------------
// 	//ԭʼ����=��������������-��ǰ������/������������//---���-----2007/03/30---------------
// 	//ħ�����ӵĳɹ����ʼ��㹫ʽ�� 
// 	//��������ȼ���X//������ǰ�ȼ���Y//�����ɹ���Ҫħ������=2^(��X-1��/10)* Y
// 	//�ù�ʽ�����ǣ�2^(��X-1��/10) `����װ��ÿ����һ�����󵵴Σ���Ҫ���ħ������������*Y������������N������Ҫ���ħ������Ϊ��һ��������N����
// 	//-------------------------------------------------
// 	int nowLevel = pWeapon->GetLevel();
// 	int nX = pWeapon->GetReqLev();
// 	int nMaxLevel = pWeapon->GetWeaponMaxLevel();
// /*	float nBarbarismSucceed = (float)( nMaxLevel - nowLevel )/(float)nMaxLevel;
// 	int nSureSucceed =  pow( 2,( nX - 1 ) / 10 ) * nowLevel;
// 
// 	//ͳ�Ʒ��˶��ٸ����ӻ��ʵĻ�ʯ
// 	int nStoneCount1Lev = 0;//����1����ʯ����
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
// 		nOmnID = pOmnipotence->GetTypeID();// �жϵ�����λ����Ʒ�Ƿ�Ϊ����ʯ [�½���11/6/2007]
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
// //	if( RandGet( 100 ) < nSucceedProbability )//�ɹ���
// 	if(bHasLuckyGem || RandGet( 100 ) < nSucceedProbability )
// 	{
// 		pWeapon->SetWeaponUpStatus();//�޸�Ϊ�����Ժ������
// 		DEBUG_TRY		// VVVVVVVVVV
// 		//ɾ����ʯ
// 		ASSERT( EraseItem( idGem, SYNCHRO_TRUE ) );
// 		if( pOmnipotence )
// 		{
// 			ASSERT( EraseItem( nTmpID, SYNCHRO_TRUE ) );//  [�½���11/6/2007]
// 		}
// 		
// 		//ɾ����ʯ
// 		for( int i = 0;i < nSize;i++ )
// 		{
// 			ASSERT( EraseItem( pDataSet[ i ],SYNCHRO_TRUE ) );
// 		}
// 		DEBUG_CATCH("EmbedGem2")// AAAAAAAAAAA
// 
// 		// update item info at client
// 		//20071214����:ͬ���޸�-----------
// //		CMsgItemInfo msg;
// //		IF_OK (msg.Create(pWeapon, ITEMINFO_UPDATE))
// //			this->SendMsg(&msg);
// 		this->SynchroItem(pWeapon,ITEMINFO_UPDATE);
// 		//--------------------------------
// 		// ���������ɹ���Ϣ
// 		CMsgCompound msg2;
// 		IF_OK( msg2.Create( MSGCOMPOUND_WEAPON_LEVUP_SUC,0 ) )
// 			this->SendMsg( &msg2 );
// 	}
// 	else
// 	{
// 		//ɾ������
// //		ASSERT( EraseItem( idWeapon, SYNCHRO_TRUE ) );
// 		//ɾ����ʯ
// 		ASSERT( EraseItem( idGem, SYNCHRO_TRUE ) );
// 		if( pOmnipotence )
// 		{
// 			ASSERT( EraseItem( nTmpID, SYNCHRO_TRUE ) );//  [�½���11/6/2007]
// 		}
// 		
// 		//ɾ����ʯ
// 		for( int  i = 0;i < nSize;i++ )
// 		{
// 			ASSERT( EraseItem( pDataSet[ i ],SYNCHRO_TRUE ) );
// 		}
// 		CMsgCompound msg2;
// 		IF_OK( msg2.Create( MSGCOMPOUND_WEAPON_LEVUP_FAL,0 ) )
// 			this->SendMsg( &msg2 );
// 	}
}
//[2010-07-20 goto]װ������
void CUser::EquipAddStar( OBJID idEquip,short nGemCount,bool bUseLuckyCharm )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return;
	}
	//���װ��
	CItem* pEquip = GetItem(idEquip,false);
	if(!pEquip)
		pEquip = GetEquipItem(idEquip);

	if (!pEquip || !pEquip->IsCanUpLevel())
		return;
	if (pEquip->GetTypeID()==144000)//տ¬��
		return;
	if (pEquip->IsWeaponSoul())
		return;
	if (pEquip->IsLocked())
		return;
	
	int nOldLev = pEquip->GetEquipStar();
	if (nOldLev<0 || nOldLev >9)
	{
		if (nOldLev==COMP_MAX_EQUIPLEV)
			SendSysMsg(_TXTATR_TOPSHOW, "��װ���Ѿ�������ǵ�");
		return;
	}
	int nCostMoney = pEquip->GetEquipAddLevCost(2);
	//�ж�����
	if (GetMoney()<nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return;
	}
	
	int nNeedGemLev = pEquip->GetNeedGemLev(nOldLev);//��Ҫ�ķ��ȼ�
	//ǿ����Ҫ�ķ�type
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
	int nCoseGem = nGemCount;//��Ҫ�۳������Ƿ�����
	int nHasGemCount = GetItemByTypeMulti(idGemtype, nHasGemSize, idGemArr, true);
	if (!nHasGemCount)//������û�����Ƿ�
	{
		//if (!bAutoBuy)//���Զ�����
		{
			SendSysMsg(_TXTATR_TOPSHOW, "������û����Ӧ�����Ƿ�");
			return;
		}
// 		else
// 		{
// 			//��Ԫ�������Ƿ�ǿ��
// 			nCoseGem = 0;//ȫ����Ԫ�������۷�
// 			vasItem* pVasItem = VASItem()->GetItemByType(idGemtype);
// 			if (!pVasItem)
// 				return;
// 			int nPriceTotal = 0;//��Ʒ�۸�
// 			if (pVasItem->discount==0)//û���ۿ�
// 				nPriceTotal = (pVasItem->price*nGemCount);
// 			else//���ۿ�
// 				nPriceTotal = (pVasItem->price*nGemCount)*(pVasItem->discount/100.0);
// 			
// 			if (GetMoneyByType(5)<nPriceTotal)
// 			{
// 				SendSysMsg(_TXTATR_TOPSHOW, "Ԫ������");
// 				return;
// 			}
// 			
// 			SpendVas(nPriceTotal);
// 		}
	}
	if (nHasGemCount)//�����������Ƿ�
	{
		if (nHasGemCount<nGemCount)
		{
			//if (!bAutoBuy)
			{
				SendSysMsg(_TXTATR_TOPSHOW, "���������Ƿ���������");
				return;
			}
// 			else//�Զ��������Ƿ�
// 			{
// 				nCoseGem = nHasGemCount;//������Ԫ���������÷�
// 				vasItem* pVasItem = VASItem()->GetItemByType(idGemtype);
// 				if (!pVasItem)
// 					return;
// 				int nPriceTotal = 0;//��Ʒ�۸�
// 				if (pVasItem->discount==0)//û���ۿ�
// 					nPriceTotal = (pVasItem->price*(nGemCount-nHasGemCount));
// 				else//���ۿ�
// 					nPriceTotal = (pVasItem->price*(nGemCount-nHasGemCount))*(pVasItem->discount/100.0);
// 				
// 				if (GetMoneyByType(5)<nPriceTotal)
// 				{
// 					SendSysMsg(_TXTATR_TOPSHOW, "Ԫ������");
// 					return;
// 				}
// 				
// 				SpendVas(nPriceTotal);
// 			}
		}
	}
	//����ɹ���
	float nSucceedProbability = pEquip->GetSucRate(2)*nGemCount;
	
	float nFailRate = pEquip->GetEquipStarFailRate()*1.0/1000;
	nSucceedProbability += nFailRate;//����ʧ�ܼӳ�
	if (bUseLuckyCharm)//ʹ�����˱��� 610048
	{
		CItem *pExtItem = GetItemByType(610048);
		if (!pExtItem)
			return;
		if (SpendItem(pExtItem))
			nSucceedProbability += 10;
		else
			return;
		if ((ItemType()->GetMonoly(610048)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, 610048);
	}
	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//�۳�����
		return;
	bool bBind = false;//ǿ�����Ƿ�󶨣����ʹ����ǿ���İ󶨷����װ��
	for(int nkidnap=0; nkidnap<2; nkidnap++)
	{
		if (nCoseGem)//����ʹ�ð󶨵�
		{
			DEBUG_TRY//try
			//ɾ�����Ƿ�
			for (int i=0; i<nHasGemSize; i++)
			{
				CItem* pGemItemTemp = GetItem(idGemArr[i],false);
				if (!pGemItemTemp)
					continue;
				if (!pGemItemTemp->GetKidnap() && nkidnap==0)//��һ��ѭ��ʱֻ�Ұ󶨵ķ�
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
	if (RandGet(100000) < nSucceedProbability*1000)//���ǳɹ������ǵ��ɹ�����С��������ȡ100000������������ѳɹ��ʳ���1000
	{
		pEquip->SetEquipStar(nOldLev+1);
		pEquip->UpdateEquipAdditionProperty(nOldLev);//����װ����������
		if (nOldLev==3 || nOldLev==6/* || nOldLev==8*/)
			pEquip->SetEquipStarFailRate(0);//ʧ�ܼӳ�����
		
		if (bBind && !pEquip->GetKidnap())
			pEquip->SetKidnap(1);

		CMsgCompound msg;//���ͳɹ���Ϣ
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_ADDSTAR_SUC, idEquip, pEquip->GetEquipStar() ) )
			this->SendMsg(&msg);
		SendSysMsg("��ϲ�㣬װ�����ǳɹ�");

		if (nOldLev+1>4)
		{
			char szMsg[255];
			char UserName[128] = "";
			char itemName[128] = "";
			sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			sprintf(szMsg, "<%s>��<%s>������<3_%d_6>��ʱ����պ�������ʣ��·����¶�ʧȥ�˹⻪!",
				UserName, itemName ,nOldLev+1);
//			char szItem[128];
//			sprintf(szItem,"%d_%d_%s_%d_%d_%d",
//				pEquip->GetID(), pEquip->GetTypeID(), ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);
		}
		int nPos = pEquip->GetPosition();
		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
		//֪ͨ�ͻ��˸�����Ʒ��Ϣ
// 		CMsgItem	msgItem;
// 		msgItem.Create(ITEMACT_SENDINFO,0);
// 		msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 		this->SendMsg(&msgItem);
	}
	else//����ʧ��
	{
		if (nOldLev>4 && nOldLev!=7)//ʧ�ܺ���
		{
			if (nOldLev>7)//ʧ�ܺ���
				pEquip->SetEquipStar(7);
			else if (nOldLev > 4 && nOldLev < 7)
				pEquip->SetEquipStar(4);
			
			pEquip->UpdateEquipAdditionProperty(nOldLev);//����װ����������
			int nPos = pEquip->GetPosition();
			if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
			//֪ͨ�ͻ��˸�����Ʒ��Ϣ
// 			CMsgItem	msgItem;
// 			msgItem.Create(ITEMACT_SENDINFO,0);
// 			msgItem.Add(pEquip->GetID(),0,0,0, 0,0,0);
// 			this->SendMsg(&msgItem);
		}
		
		pEquip->SetEquipStarFailRate(nFailRate*1000+0.5+COMP_SUC_FAIL_RATE*nGemCount);//ʧ�ܼӳ�
		CMsgCompound msg;//����ʧ����Ϣ
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_ADDSTAR_FAIL, idEquip, pEquip->GetEquipStar() ) )
			this->SendMsg(&msg);
		SendSysMsg("���ź���װ������ʧ��");
	}
	if ((ItemType()->GetMonoly(idGemtype)  & COSTLY_ITEM) != 0)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), nGemCount, idGemtype);
}
//goto ��ӡ 610047(��ӡ����ID) ����10G
bool CUser::EquipSoulMark(OBJID idEquip)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return false;
	}
	//���װ��
	CItem* pEquip = GetItem(idEquip,false);
	if(!pEquip)
		pEquip = GetEquipItem(idEquip);

	if (!pEquip || !pEquip->IsCanUpLevel())
		return false;
	if (pEquip->GetTypeID()==144000)//տ¬��
		return false;
	if (pEquip->IsWeaponSoul())
		return false;
	if (pEquip->IsSoulMark())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��Ʒ�Ѿ��ǻ�ӡ״̬");
		return false;
	}
	if (pEquip->IsLocked())
		return false;
	int nEquipColor = pEquip->GetColour();

	int nCostRes = 1;//��Ҫ�Ĳ��ϸ���
	if(nEquipColor==4)
		nCostRes = 2;
	else if(nEquipColor==5)
		nCostRes = 3;

	int nCostMoney = 100000;
	if (GetMoney()<nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	CItem *pItem = GetItemByType(610047);
	if (!pItem)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "������û�л�ӡ��Ҫ�Ĳ���");
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
		SendSysMsg(_TXTATR_TOPSHOW, "װ����ӡ�����ɹ�");
		CMsgCompound msg;//������Ϣ
		IF_OK( msg.Create( MSGCOMPOUND_EQUIP_SOULMARK_SUC, idEquip, 0 ) )
			this->SendMsg(&msg);
		int nPos = pEquip->GetPosition();
		if(nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND)
			UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);
		if (bIsCostly)//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), nCostRes, 610047);
		return true;
	}

	return false;
}
//[2010-08-04 goto]��ʯ�ϳ�
bool CUser::GemLevUp(OBJID idCharm, set<OBJID> pDataSet,int nSize)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return false;
	}
	int nGemType = 0;
	CItem* pGem = NULL;
	int nCountGem = 0;
	bool bKidnap = false;//�ϳɺ��Ƿ��
	set<OBJID>::iterator it = pDataSet.begin();
	for (; it!=pDataSet.end(); it++)
	{
		pGem = GetItem(*it,false);
		if (pGem)
		{
			if (pGem->GetItemType()!=41 && pGem->GetItemType()!=42)//���Ǳ�ʯ
				return false;
			if (pGem->IsLocked())
			{
				SendSysMsg(_TXTATR_TOPSHOW, "��������Ʒ���ܽ��д˲���");
				return false;
			}
			if (nGemType==0)
			{
				nGemType = pGem->GetTypeID();
			}
			else if (pGem->GetTypeID()!=nGemType)//��ʯ���಻ͬ
			{
				SendSysMsg(_TXTATR_TOPSHOW, "ֻ����ͬ�ı�ʯ���ܺϳ�");
				return false;
			}
			if (pGem->GetKidnap())//ֻ��ԭ������һ���ǰ󶨵ģ��ϳɺ��Ϊ��
				bKidnap = true;
			nCountGem+=pGem->GetItemAmount();
		}
		else
			return false;
	}
	if (nSize>nCountGem || nCountGem==0)//��ʯ�������Ի��߸���û�б�ʯ
		return false;

	if (nGemType%10==9 || nGemType%10==0)//Ŀǰ���9��
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��ʯ�Ѿ�����ߵȼ�");
		return false;
	}
	int nCostMoney = 5000;
	if (nCostMoney>GetMoney())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	bool bIsCostly = pGem->IsCostlyItem();
	int nSucceedProbability = (nSize-2)*25;//�ɹ���,3����ʯ�ϳɵĳɹ�����25%,����һ���������ͬ��ʯ���������Ӷ���25%�ĳɹ���

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

		if (nCharmType==610016 && nGemType%10>3)//�ͼ��ϳɷ�ֻ�ܺϳ�3��(����3��)���µı�ʯ
		{
			SendSysMsg(_TXTATR_TOPSHOW, "��ʯ�ϳɷ��ĵȼ�����");
			return false;
		}

		nSucceedProbability+=25;
		bIsCostlyCharm = pItemCharm->IsCostlyItem();
		if (pItemCharm->GetKidnap())
			bKidnap = true;
	}
	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//�۳�����
		return false;
	DEBUG_TRY//try
		//ɾ����ʯ
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

	if (RandGet(100) < nSucceedProbability)//�ϳɳɹ�
	{
		CItem* pNewGem = NULL;
		if (!bKidnap)
			pNewGem = AwardItem(nGemType+1);
		else
			pNewGem = AwardItem(nGemType+1, true, false, true, false, 1, false, true, false, 1);

		if (pNewGem)
		{
			CMsgCompound msg;//���ͳɹ���Ϣ
			IF_OK( msg.Create( MSGCOMPOUND_GEM_LEVUP_SUC, 0, 0 ) )
				this->SendMsg(&msg);
			SendSysMsg("��ϲ�㣬��ʯ�ϳɳɹ�");
			if ((nGemType%10+1)>2)
			{

				char szMsg[255];
				char UserName[128] = "";
				char itemName[128] = "";
				char gemName[128]  = "";
				sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
				sprintf(gemName,  "%d_[%s]_%d_%d_%d_%d_%d",
					SNAPSHOTOBJ,  ItemType()->QueryItemType(nGemType+1)->GetStr(ITEMTYPEDATA_NAME), pNewGem->GetID(), pNewGem->GetTypeID(), this->GetID(), pNewGem->GetKidnap(), pNewGem->GetColour());
				sprintf(szMsg, "<%s>�ϳ���һ��<%s>������������Ľ��",
					UserName, gemName);
//				char szItem[128];
//				sprintf(szItem,"%d_%d_%s_%d_%d_%d",
//					pNewGem->GetID(), pNewGem->GetTypeID(), ItemType()->QueryItemType(pNewGem->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), this->GetID(), pNewGem->GetKidnap(), pNewGem->GetColour());
				UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pNewGem->GetID(), pNewGem->GetTypeID(), NULL, ALLUSERS_NAME);
			}
			if (pNewGem->IsCostlyItem())//goto ������Ʒ��¼
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get 1 %u from gem_upgrade", GetID(), GetName(), nGemType+1);
		}
	}
	else
	{
		SendSysMsg("���ź�����ʯ�ϳ�ʧ��");
		CMsgCompound msg;//����ʧ����Ϣ
		IF_OK( msg.Create( MSGCOMPOUND_GEM_LEVUP_FAIL, 0, 0 ) )
			SendMsg(&msg);
	}
	if (bIsCostly)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from gem_upgrade", GetID(), GetName(), nSize, nGemType);
	if (bIsCostlyCharm)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from gem_upgrade", GetID(), GetName(), 1, nCharmType);
	return true;
}

BOOL CUser::EnterInstance(OBJID idMapInstance,UINT nMapType,UINT nInstanceScope,UINT nDomainMapID,int x,int y)///����ָ�����͵ĸ���
{
	BOOL hr=false;
	//int nPosX=0,nPosY=0;
// 	POINT ptPortal;
	if(nInstanceScope==0 ||nInstanceScope==1 ||nInstanceScope==3)
	{
		if (g_SpecilLine==0)
			SetRecordPos(GetMapID(),GetPosX(),GetPosY());///�������λ����Ϣ,�Ա��ڵ��߿����ڸ����ſ�
		else if (g_SpecilLine==1)
			SetSpecilPos(GetMapID(),GetPosX(),GetPosY(),STATE_SPECILLINE_NORMAL);
		hr=FlyMap(idMapInstance,x, y);
	}
//modify code by �ֵ�Ԩ begin 2011.5.24
	::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) enter enterinstance(%u) maptype(%d) scope(%d) mapid(%d) ", GetID(), GetName(), idMapInstance, nMapType, nInstanceScope, nDomainMapID);
//modify code by �ֵ�Ԩ end
// 	else if(nInstanceScope>1 && nInstanceScope<6)
// 	{
// 		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///�������λ����Ϣ,�Ա��ڵ��߿����ڸ����ſ�
// 		hr=FlyMap(idMapInstance,x, y);
// 	}
//	else if(nInstanceScope==2)
//	{
//		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///�������λ����Ϣ,�Ա��ڵ��߿����ڸ����ſ�
//		hr=FlyMap(idMapInstance,x, y);
//	}
//	else if(nInstanceScope==3)
//	{
//		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///�������λ����Ϣ,�Ա��ڵ��߿����ڸ����ſ�
//		hr=FlyMap(idMapInstance,x, y);
//	}
//	else if(nInstanceScope==4)
//	{
//		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///�������λ����Ϣ,�Ա��ڵ��߿����ڸ����ſ�
//		hr=FlyMap(idMapInstance,x, y);
//	}
// 	else if(nInstanceScope==6)
// 	{
// 		SetRecordPos(GetMapID(),GetPosX(),GetPosY());///�������λ����Ϣ,�Ա��ڵ��߿����ڸ����ſ�
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

//BOOL CUser::RequestRefreshInstance(OBJID idOwn,UINT nInstanceType)///�������ָ�����͵ĸ���
//{
//	BOOL hr=false;
//	///��5�ŵ�ͼ�鷢����,��֮��һ��User��ȥָ�����͵ĸ���
//	CMsgMapInstance msg;
//	msg.CreateRequestEnterMsg(CurrentMapGroup()->GetID(),GetID(),idOwn,nInstanceType);///����������븱������Ϣ
//	CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_INSTANCE,&msg);
//	return hr;
//}

bool CUser::RequestEnterInstance(UINT nInstanceType,UINT nDomainMapID,UINT nInstanceScope,UINT nCellx ,UINT nCelly)///�������ָ�����͵ĸ���
{
	if(!CanBeSee())
		return false;
	
	///�򸱱���ͼ�鷢����,��֮��һ��User��ȥָ�����͵ĸ���
	OBJID idOwn=ID_NONE;
	if(nInstanceScope==INSTANCETYPE_USER)	//��ͨTeam����
	{
		idOwn = GetID();
		nDomainMapID = (GetLev() - 1) / 5 + 1;
	}
	if(nInstanceScope==INSTANCETYPE_TEAM)	//��ͨTeam����
	{	
		if (GetTeam())
		{
			idOwn=GetTeam()->GetID();
			if(nDomainMapID < 1000000)
			{
				//��¼��������־�Է����߲��ǡ�
				GetTeam()->SetLocation(GetID(),nInstanceType + 100000,nCellx,nCelly,true);
				if (nInstanceType==23)//�����¸���--jay
				{
					CSyndicate *psyn= SynManager()->QuerySyndicate(GetSynID());
					if (psyn)
					{
						if (nDomainMapID==0)
						{
							SendSysMsg("���İ��ɻ�û�п��������ã��޷�����õ���");
							return false;
						}
						else if (nDomainMapID > 10)
						{
							if(IsGM())
								SendSysMsg("�ű�����һ���������ֵ!");
							return false;
						}
					}
					else
					{
						SendSysMsg("��û�а�!");
						return false;
					}
				}
				else
					nDomainMapID = (GetTeam()->GetMemberLev() - 1) / 5 + 1;
			}
		}
		else
		{
			SendSysMsg("���鸱��û���ҵ�����");
			return false;	
		}
	}
// 	else if(nInstanceScope==INSTANCETYPE_SYN)	//��ͨ���Ÿ���
// 	{
// 		if(GetSynID())
// 			idOwn=GetSynID();
// 		else
// 		{
// 			SendSysMsg("���Ÿ���û���ҵ�����");
// 			return;
// 		}
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_CITYWAR)	//��ս����
// 	{
// 		idOwn = nDomainMapID;
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_ACTIVE)	
// 	{
// 		idOwn = nDomainMapID;
// 	}
// 	else if(nInstanceScope==INSTANCETYPE_MARRY) //��������
// 	{
// 		CItem* pItem = NULL;
// 		pItem = GetMarryPass();  //��û���ͨ��֤
// 		if (!pItem)
// 		{
// 			pItem = GetMarryPaper();  //��û���ϲ��
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
// 		int nMarryPassId1 = atoi(g_objStrRes.GetStr(600008));//����ͨ��֤ID,��������ͨ��֤
// 		int nMarryPassId2 = atoi(g_objStrRes.GetStr(600010));//����ͨ��֤ID,��ͨ����ͨ��֤
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
	msg.CreateRequestEnterMsg(CurrentMapGroup()->GetID(),GetID(),idOwn,nInstanceType,nInstanceScope,nDomainMapID,nCellx,nCelly,NULL);///����������븱������Ϣ
	CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(MSGPORT_MAPGROUP_INSTANCE,&msg);
	return true;
}
//-------------------------------------------

//-------���---2007/05/30-----
BOOL	CUser::ResetItemCD( UINT nDelayTimeMS, UINT TypeID, CTimeOutMS* pTimeOutMS  )
{
	CHECKF( pTimeOutMS );
	CHECKF( nDelayTimeMS > 0 );
// 	int nQueryIndex = CItem::GetDelayGroupIndex( TypeID );
	CMsgCoolDown msg;
	if( msg.Create( this->GetID(),MAGICCOOLDOWN_BEGIN,ITEM_INFO,TypeID/*nQueryIndex*/,nDelayTimeMS, 0 ) ) //����CD
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

	DetachStatus( QueryRole(),STATUS_DIE );// ����������״̬

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
//	����
//  ʹ�ø�����Ḵ��
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
					this->SendSysMsg("ԭ����Ѫ��ħ������");
					bReborn = true;
				}
			}
		}
		else
		{
			this->SendSysMsg("ԭ����Ѫ��ħ������");
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
		if(this->GetMap()->GetType()==3)//ר�߾�̬��ͼ
		{
			IStatus* pStatus = QueryStatus(STATUS_WARGAME);//������
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

//	20070712	���
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

//	20070712	���	
//	���Ҳֿ�
CItemPtr CUser::GetStorageItemByType(OBJID idType)	
{
	if(!m_pStorage)
		return NULL;
// 		m_pStorage	= CPackage::CreateNew();
// 	CHECKF(m_pStorage);
// 
//    	//�ֿ⻥ͨ
//  	CHECKF(m_pStorage->Create(ITEMPOSITION_STORAGE, GetID(), GetID(), 0, Database()));
// 
// 	if(!m_pStorage)
// 		return NULL;

	return m_pStorage->GetItemByTypeID(idType);
}
//--------------------------
//20070716����:���Լ��űߵ�����Ʒ.
//��Ʒ������,����ϵͳ������
//mapItem����ʱ,�κ��˶�����ʰȡ
BOOL CUser::DropItemCreatedBySys(DWORD itemType,//itemtype or money
						  BOOL bIsMoney/*=false*///true:money.false:item						  
						  )
{
// 	CHECKF(itemType);
// 	
// 	if(bIsMoney)//�����Ǯ
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
// 	else//������Ʒ
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
// 				//�ο�monster������Ʒ
// 				if(pMapItem->Create(MapManager()->SpawnMapItemID(), 
// 					GetMap(), 
// 					pos, 
// 					itemType, 
// 					NULL,//idOwner,�κ��˿���ʰȡ 
// 					0,//nMagic2, 
// 					0,//nMagic3, 
// 					bUnIdent, 
// 					0,//nUserLuck,
// 					ID_NONE//teamID
// 					))//�¼�teamID
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
// 			strDefaultSkill = "�������";
// 		}
// 		else if(this->GetProfession()&PROF_MALE)
// 		{
// 			strDefaultSkill = "����ͻ��";
// 		}
// 		else if(this->GetProfession()&PROF_MALE)
// 		{
// 			strDefaultSkill = "������";
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
		SendSysMsg("�һ���Ϣ����ɹ���");
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
		SendSysMsg("��û��װ������");
		return false;
	}
	if(!pItem->IsWeaponSoul())
	{
		SendSysMsg("��û��װ������");
		return false;
	}


	ST_WEAPONSOULDATA wpslInfo;
	CWeaponSoulData* pData = pItem->QueryWeapensoulData();
	if(!pData)
	{
		SendSysMsg("��û��װ������");
		return false;
	}
	if(!pData->GetInfo(wpslInfo))
	{
		SendSysMsg("��û��װ������");
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
			//������
			if(wpslInfo.level >= GetLev())
			{
				SendSysMsg("���Ǿ����������޷�����ʹ��");
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

//��ʯ����
OBJID CUser::GemCutwork(OBJID idGem, OBJID idCharm)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return ID_NONE;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return ID_NONE;
	}
	//���װ��
	CItem* pGem = GetItem(idGem,false);
	if(!pGem || !pGem->IsGem())
		return ID_NONE;
	//��õ�����
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
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
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
					//ϵͳ����
					char szMsg[255] = "";
					char UserName[128] = "";
					char gemName[128]  = "";
					sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, GetName());
					sprintf(gemName,  "%d_[%s]_%d_%d_%d_%d_%d",
						SNAPSHOTOBJ,  pNewGem->GetStr(ITEMDATA_NAME), pNewGem->GetID(), pNewGem->GetTypeID(), GetID(), pNewGem->GetKidnap(), pNewGem->GetColour());
					sprintf(szMsg, "<%s>ʹ��<%s>��<%s>������<%s>����������˱�ʯ�����ԡ�",UserName, szCharm, szOldGem,gemName);

					UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pNewGem->GetID(), pNewGem->GetTypeID(), NULL, ALLUSERS_NAME);
				}
				if(pNewGem->IsCostlyItem())//goto ������Ʒ��¼
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

		if (bIsCostly)//goto ������Ʒ��¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, 610056);
	}
	else
		return ID_NONE;
	DEBUG_CATCH3("CUser::GemCutwork error!userid=%u,gemtype=%d", GetID(),nGemType)
	return idNewGem;
}
//ˢ�³��ﵰ����
bool CUser::FlushPetegg(OBJID idData, OBJID idItem, int nGrow, int nNextFlush)
{
	CHECKF(m_pPetEgg);
	CHECKF(idItem);

	CItem* pItem = GetItem(idItem, false, true);//�����ֿ����
	CHECKF(pItem);

	if(nGrow<0 || nGrow>100)
	{
		::LogSave("CUser::FlushPetegg err grow!nGrow=%d", nGrow);
		nGrow = RandGet(100)+1;
	}

	if(idData==ID_NONE)//�¼�����
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

//modify code by �ֵ�Ԩ begin 2011.5.30
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
//modify code by �ֵ�Ԩ end

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

//modify code by �ֵ�Ԩ begin 2011.6.22
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
//modify code by �ֵ�Ԩ end
bool CUser::EquipSoulAdd(OBJID idWeapon, OBJID nCount,bool bUseBook)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return false;
	}
	//���װ��
	CItem* pEquip = GetItem(idWeapon,false);
	if (!pEquip || !pEquip->IsCanUpLevel())
		return false;
	if (pEquip->GetTypeID()==144000)//տ¬��
		return false;
	if (pEquip->IsWeaponSoul())
		return false;
	if (pEquip->IsLocked())
		return false;


	bool bUp=false;
	if (bUseBook)//ʹ�þ���
	{
		CItem* pBook=GetItem(nCount,false);
		if (!pBook)
		{
			SendSysMsg("��û���������");
			return false;
		}
		else
		{
			DEBUG_TRY
			if ((pEquip->IsActEquipment() && !pBook->IsActSoulBook()) || (pEquip->IsDefEquipment() && !pBook->IsDefSoulBook()))
			{
				SendSysMsg("������װ������");
				return false;
			}
			int nBookType=pBook->GetSoulBookType();
			if (nBookType==pEquip->GetEquipSoulType())
			{
				SendSysMsg("���������������ͬ��������");
				return false;

			}
			int nBind=pBook->GetKidnap();

			if (!SpendItem(pBook))//�۾���
				return false;
			
			if (pEquip->GetEquipSoulLev()==0)
				pEquip->SetEquipSoulLev(1);//���޵���
			
			pEquip->SetEquipSoulType(nBookType);
			
 			if (nBind==1 && !pEquip->GetKidnap())
				pEquip->SetKidnap(1);
			DEBUG_CATCH("EquipSoulAdd() usebook");
		}
	}
	else//��ʹ��
	{
		if (pEquip->GetEquipSoulLev() <= 0 || pEquip->GetEquipSoulLev() >= 10)
		{
			if (pEquip->GetEquipSoulLev() == 0)
				SendSysMsg("��װ����û��ħ,��ʹ�þ��ḽħ");
			else
				SendSysMsg("��װ���Ѿ�����߼�");
			return false;
		}
		int nSpendNum=nCount;
		bool bBind=false;
		if(!DelItemMulti(610061,bBind,nCount))
		{
			SendSysMsg("ħ�鲻��!");
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
			sprintf(szMsg, "<%s>̤�뽭���������������㼯����ħ���<%s>������ȼ���������<3_%d_6>����������ʷ�Ϲھ��Ž��������»���˭�ܼ���",
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
			sprintf(szMsg, "<%s>̤�뽭���������������㼯����ħ���<%s>������ȼ���������<3_%d_6>����װ�������ٶȴ��������",
				UserName, itemName ,pEquip->GetEquipSoulLev());
			UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);
		}
	}
		SendSysMsg("װ������ɹ�");
		return true;
}
bool CUser::EquipSoulExchange(OBJID idWeapon, OBJID idExWeapon)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return false;
	}
	//���װ��
	CItem* pEquip = GetItem(idWeapon,false);
	CItem* pEquip2= GetItem(idExWeapon,false);
	
	if (!pEquip || !pEquip->IsCanUpLevel() || !pEquip2 || !pEquip2->IsCanUpLevel())
		return false;
	if (pEquip->GetTypeID()==144000 || pEquip2->GetTypeID()==144000)//տ¬��
		return false;
	if (pEquip->IsWeaponSoul()||pEquip2->IsWeaponSoul())
		return false;
	if (pEquip->IsLocked() || pEquip2->IsLocked())
		return false;

	CItem* pNeedItem=GetItemByType(610059);//����ת�Ʒ�
	if (!pNeedItem)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��û������ת�Ʒ�");
		return false;
	}

	if (pEquip->GetItemType()!=pEquip2->GetItemType())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "װ������");
		return false;
	}

	if (pEquip->GetEquipSoulLev() <= pEquip2->GetEquipSoulLev())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "ת��Ŀ������ȼ�������ڱ�ת��Ŀ������ȼ�");
		return false;
	}

	DEBUG_TRY
	if (!SpendItem(pNeedItem))//��ȡ����ת�Ʒ�
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
	
	SendSysMsg("���齻���ɹ�");
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
		if (nCostNum)//����ʹ�ð󶨵�
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
					if (!pItemTemp->GetKidnap() && nkidnap==0)//��һ��ѭ��ʱֻ�Ұ󶨵�
						continue;
				}
				else
				{
					if (pItemTemp->GetKidnap() && nkidnap==0)//��һ��ѭ��ʱֻ��δ�󶨵�
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

//modify code by �ֵ�Ԩ begin 2011.7.13
bool CUser::EquipArtific(OBJID idEquip)//����
{
	if( QuerySquareDeal() )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return false;
	}
	if( QueryBooth() )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return false;
	}
	//���װ��
	CItem* pItem = GetItem(idEquip,false);
	if( !pItem || !(pItem->GetColour() > 3) )
		return false;

	if( pItem->GetTypeID()==144000 )//տ¬��
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
		SendSysMsg(_TXTATR_TOPSHOW, "���ϱ�������");
		return false;
	}

	int aoumnt = 0;
	srand(time(NULL));//���������
	int randnum = 0;
	if( pItem->GetColour() == 4 )//��
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
	else if( pItem->GetColour() == 5 )//��
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
		SendSysMsg(_TXTATR_TOPSHOW, "װ���ȼ���Ʒ�ʲ���");
		return false;
	}

	int kindnap = pItem->GetKidnap();
	if(pItem->IsCostlyItem())//������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) EquipArtific %d %u",GetID(),GetName(),1,pItem->GetTypeID());

	//ɾ��װ��ͬʱ���ͻ��˷���Ϣ
	if( !EraseItem(idEquip,true) )
		return false;
	
	//����Ʒ
	if( kindnap > 0 )
	{
		if( !AwardItemCount(610061,aoumnt,true,0,1,0,0) )
		{//ʧ������¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) NOT GET %d %u kindnap",GetID(),GetName(),aoumnt,610061);
		}
	}
	else
	{
		if( !AwardItemCount(610061,aoumnt,true,0,0,0,0) )
		{//ʧ������¼
			::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) NOT GET %d %u",GetID(),GetName(),aoumnt,610061);
		}
	}
	return true;
}

bool CUser::EquipMove(OBJID idEquip,OBJID idCharm,int &firstaddlv,int &secondaddlv)//ǿ��ת��
{
	if( QuerySquareDeal() )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return false;
	}
	if( QueryBooth() )
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return false;
	}
	//���װ��
	CItem* pItem = GetItem(idEquip,false);
	if( !pItem )
		return false;

	if( pItem->GetTypeID()==144000 )//տ¬��
		return false;
	if( pItem->IsWeaponSoul() )
		return false;
	if( pItem->IsLocked() )
		return false;

	CItem* pGem = GetItem(idCharm,false);
	if( !pGem )
		return false;
	
	if( pGem->GetTypeID()==144000 )//տ¬��
		return false;
	if( pGem->IsWeaponSoul() )
		return false;
	if( pGem->IsLocked() )
		return false;

	//�ж��Ƿ�ͬ����װ��
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
	
	//ɾ��Ų�ƻ���
	bool bHasKind = false;
	if (!DelItemByTypeMulti(610060,1,bHasKind, false))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "û���㹻��Ų�ƻ���");
		return false;
	}

	if(pItem->GetKidnap() && !pGem->GetKidnap())
		pGem->SetKidnap(1);

	pItem->SetEquipAddLev(0);
	if( pItem->IsSoulMark() )
	{
		float nRate = pItem->GetEquipAddRate(0);
		float nLastRate=1;
		if( nfirstLev > 1 )//�����1�����ϵ�Ҫ�Ȼ�ԭ��0���������ٳ���ϵ��
			nLastRate = pItem->GetEquipAddRate(nfirstLev);
		
		//������ǻ�ӡ��������ԣ����뻹ԭ�ɻ�ӡǰ������ ��ӡ +10%
		nRate += 0.1f;
		if (nRate==0.1f)
			nRate=1.1f;
		nLastRate += 0.1f;
		
		int nSort1 = pItem->GetInt( ITEMDATA_BASE1 ) / 10000;
		int nSort2 = pItem->GetInt( ITEMDATA_BASE2 ) / 10000;
		int nOldBase1 = pItem->GetInt( ITEMDATA_BASE1 )%10000*1.0/nLastRate+0.5;//�������룬��С���
		int nOldBase2 = pItem->GetInt( ITEMDATA_BASE2 )%10000*1.0/nLastRate+0.5;
		
		pItem->SetInt(ITEMDATA_BASE1, nSort1*10000+nOldBase1*nRate+0.5);
		pItem->SetInt(ITEMDATA_BASE2, nSort2*10000+nOldBase2*nRate+0.5);
	}
	else
	{
		float nRate = pItem->GetEquipAddRate(0);
		float nLastRate=1;
		if( nfirstLev > 1 )//�����1�����ϵ�Ҫ�Ȼ�ԭ��0���������ٳ���ϵ��
			nLastRate = pItem->GetEquipAddRate(nfirstLev);
		
		int nSort1 = pItem->GetInt( ITEMDATA_BASE1 ) / 10000;
		int nSort2 = pItem->GetInt( ITEMDATA_BASE2 ) / 10000;
		int nOldBase1 = pItem->GetInt( ITEMDATA_BASE1 )%10000*1.0/nLastRate+0.5;//�������룬��С���
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
//modify code by �ֵ�Ԩ end

//������ף�������֮��
BOOL CUser::EquipMakeChaosHole(OBJID idEquip, OBJID idStone)
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return FALSE;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
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

	if (pEquipItem->GetTypeID()==144000)//տ¬��
		return FALSE;

	CItem* pStone = GetItem(idStone,false);
	if (!pStone || pStone->GetTypeID()!=610057 || pStone->IsLocked())//��������루610057��
		return FALSE;

	BOOL bIsCostly = pStone->IsCostlyItem();
	int nStoneType = pStone->GetTypeID();
	int nHoleCount = pEquipItem->GetEmbedCountLimit();
	if (nHoleCount != 3)//�����Ǵ��������׵�װ���ſ��Դ���ĸ���
		return FALSE;

	int nCostMoney = 20000;//2J
	if (GetMoney() < nCostMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return FALSE;
	}

	if (!SpendMoney(nCostMoney, SYNCHRO_TRUE))//�۳�����
		return FALSE;

	//�����ǰ󶨵ģ�ʹ�ú�װ��ҲҪ��ɰ�
	bool bBind = false;
	if (pStone->GetKidnap())
		bBind = true;

	if (!SpendItem(pStone))
		return FALSE;

	pEquipItem->SetInt(ITEMDATA_GEM4, _HOLE, false);

	if (bBind && !pEquipItem->GetKidnap())
		pEquipItem->SetKidnap(1);

	SendSysMsg("��ϲ�㣬��׳ɹ�");

	if (bIsCostly)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from use", GetID(), GetName(), 1, nStoneType);

	//����
	char szMsg[255] = "";
	char UserName[128] = "";
	char itemName[128] = "";
	sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, this->GetName());
	sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquipItem->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), idEquip, pEquipItem->GetTypeID(), GetID(), pEquipItem->GetKidnap(), pEquipItem->GetColour());

	sprintf(szMsg, "�������ʮ����<%s>��ռ�˶�����Ȼ��<%s>�Ļ���֮�׷����˳�����ʵ���ǰ����ѵ�һ����������ţ�", UserName, itemName);
	UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquipItem->GetID(), pEquipItem->GetTypeID(), NULL, ALLUSERS_NAME);

	return nHoleCount+1;

	DEBUG_CATCH( "CUser::EquipMakeChaosHole" );
	return FALSE;
}

bool CUser::EmbedGemInChaos( OBJID idEquip, OBJID idStone, OBJID idGem )
{
	if (QuerySquareDeal())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
		return false;
	}
	if (QueryBooth())
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
		return false;
	}
	//���װ��
	CItem* pEquip = GetItem( idEquip ,true);
	if(!pEquip)
		pEquip = GetEquipItem(idEquip);
	if(!pEquip)
		return false;

	if(!pEquip->IsCanEmbedEquip() || pEquip->IsLocked())
		return false;

	if (pEquip->GetTypeID()==144000)//տ¬��
		return false;
	//��ñ�ʯ
	CItem* pGem	= GetItem( idGem,false );
	if(!pGem || !pGem->IsGem() || pGem->IsLocked())
		return false;

	//��Ƕʯ
	CItem* pStone = GetItem(idStone,false);
	if (!pStone || pStone->GetTypeID()!=610018 || pStone->IsLocked())//"�����" ID610058 �ָĻ���Ƕ����....
		return false;

	if(pEquip->GetInt(ITEMDATA_GEM4) != _HOLE)
		return false;

	//�������װ��ֻ����Ƕ������ı�ʯ���������װ��ֻ����Ƕ������ı�ʯ
	if (!pEquip->IsRightGemType(pGem->GetTypeID()))
	{
		SendSysMsg(_TXTATR_TOPSHOW, "��ʯ������װ�����಻��");
		return false;
	}
	
	int nNeedMoney = COMP_EMBEDGEM_COST;
	if (GetMoney() < nNeedMoney)
	{
		SendSysMsg(_TXTATR_TOPSHOW, "�����е���������");
		return false;
	}
	
	//��Ǯ
	if (!SpendMoney(nNeedMoney, SYNCHRO_TRUE))
		return false;

	bool bKindnap = false;
	if (!pEquip->GetKidnap() && pGem->GetKidnap())//�����ʯ�ǰ󶨵ģ�����Ƕ���װ��Ҳ��
		bKindnap = true;

	int nGemLev = pGem->GetTypeID()%100;
	int nGemType = pGem->GetTypeID();
	BOOL bIsCostly = pGem->IsCostlyItem();
	BOOL bIsCostStone = pStone->IsCostlyItem();
	int nStoneType = pStone->GetTypeID();
	
	if (!SpendItem(pGem) || !SpendItem(pStone))
		return false;

	pEquip->SetInt(ITEMDATA_GEM4, nGemType);

	if (bKindnap)//�����ʯ�ǰ󶨵ģ�����Ƕ���װ��Ҳ��
		pEquip->SetKidnap(1);

	int nPosEquip = pEquip->GetPosition();
	if(nPosEquip >= ITEMPOSITION_EQUIPBEGIN && nPosEquip < ITEMPOSITION_EQUIPEND)
		UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);

	CMsgCompound msg;//���ͳɹ���Ϣ
	IF_OK( msg.Create( MSGCOMPOUND_EMBED_SUC, idEquip, 0 ) )
		this->SendMsg(&msg);
	SendSysMsg("��Ƕ�ɹ�");

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
				sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());
			else
				sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",SNAPSHOTOBJ, "����֮��", pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());		
		}
		else
			sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d",
				SNAPSHOTOBJ,  ItemType()->QueryItemType(pEquip->GetTypeID())->GetStr(ITEMTYPEDATA_NAME), pEquip->GetID(), pEquip->GetTypeID(), this->GetID(), pEquip->GetKidnap(), pEquip->GetColour());

		sprintf(szMsg, "<%s>��<%s>��Ƕ��һ��<1_[%s]_0_%d_0_0_%d>�����������װ��������!",
			UserName, itemName , ItemType()->QueryItemType(nGemType)->GetStr(ITEMTYPEDATA_NAME), nGemType, GemColor, GemColor);

		UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE,this, pEquip->GetID(), pEquip->GetTypeID(), NULL, ALLUSERS_NAME);
		if (nGemLev>=4)
		{
			CMsgPlayer msgPlayer;
			msgPlayer.Create(this);
			BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
		}
	}
	if (bIsCostly)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), nGemType);
	if (bIsCostStone)//goto ������Ʒ��¼
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost 1 %u from use", GetID(), GetName(), nStoneType);

	return true;
}
