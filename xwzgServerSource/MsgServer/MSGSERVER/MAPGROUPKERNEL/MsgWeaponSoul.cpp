// MsgWeaponSoul.cpp: implementation of the CMsgWeaponSoul class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MapGroup.h"
#include "MsgAction.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWeaponSoul::CMsgWeaponSoul()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgWeaponSoul::~CMsgWeaponSoul()
{

}

//------------------------------
BOOL CMsgWeaponSoul::Create(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_WEAPONSOUL != this->GetType())
		return false;

	return true;
}

BOOL CMsgWeaponSoul::Create(ST_WEAPONSOULDATA stWSLData,int nAction/* = EM_MSGWPSL_UPDATE*/,OBJID idUser/*=ID_NONE*/,OBJID idItem/*=ID_NONE*/)
{
	// init
	this->Init();
	
	// fill info now
	m_pInfo->usAction = nAction;
	m_pInfo->idItem = idItem;
	m_pInfo->idUser = idUser;

	memcpy(m_pInfo->stWpSl,&stWSLData,sizeof(ST_WEAPONSOULDATA));
	for(int i = 0 ; i < 10 ; i ++)
	{
		if(m_pInfo->stWpSl[0].addition[i] == 0)
			continue;
		else if(m_pInfo->stWpSl[0].addition[i] == 88888)
			continue;
		else
		{
			CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(m_pInfo->stWpSl[0].addition[i] / 1000);
			if(!pMagicData)
				m_pInfo->stWpSl[0].addition[i] = 0;
			else
			{
				m_pInfo->stWpSl[0].addition[i] = pMagicData->GetInt(EM_SOULMAGIC_dataID) * 1000 + m_pInfo->stWpSl[0].addition[i] % 1000;
			}
		}
	}

	for(i = 0 ; i < 3 ; i ++)
	{
		if(m_pInfo->stWpSl[0].skill[i] == 0)
			continue;
		else if(m_pInfo->stWpSl[0].skill[i] == 88888)
			continue;
		else
		{
			CSoulMagicData* pMagicData = g_pNpcMagicManager->QuerySoulData(m_pInfo->stWpSl[0].skill[i] / 1000);
			if(!pMagicData)
				m_pInfo->stWpSl[0].skill[i] = 0;
			else
			{
				m_pInfo->stWpSl[0].skill[i] = pMagicData->GetInt(EM_SOULMAGIC_dataID) * 1000 + m_pInfo->stWpSl[0].skill[i] % 1000;
			}
		}
	}
	m_unMsgType	=_MSG_WEAPONSOUL;
	m_unMsgSize	=sizeof(MSG_Info);
	return true;
}

//--------------------------------
void CMsgWeaponSoul::Process(void *pInfo)
{
	CUserPtr pUser	= UserManager()->GetUser(this);
	if(!pUser)
		return;
	if(!pUser->CanBeSee())
		return;
	if(m_pInfo->idItem  == ID_NONE)
		return;

	switch(m_pInfo->usAction)
	{
	case EM_MSGWPSL_UPDATE_LINE:
		{
			m_pInfo->usAction = EM_MSGWPSL_UPDATE;
			pUser->SendMsg(this);
		}
		break;
	case EM_MSGWPSL_QUERY_LINE:
		{
			if(m_pInfo->idItem == ID_NONE)
				return;

			CItem* pItem = pUser->GetItem(m_pInfo->idItem,true,true);
			if(!pItem)
			{
				pItem = pUser->GetEquipItem(m_pInfo->idItem);
				if(!pItem)
				{
					CHECK(pUser->QueryStorage());
					pItem = pUser->QueryStorage()->GetItemByIdItem(m_pInfo->idItem);
				}
			}
			
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(pData)
				{
					if(pData->GetInfo(wpslInfo))
					{
						CMsgWeaponSoul msgSoul;
						wpslInfo.idItem = m_pInfo->stWpSl[0].idItem;
						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msgSoul, m_pInfo->stWpSl[0].idItem);
					}
				}
			} 
			else
			{
				///道具已经不存在了,尝试通知查询者
				CMsgTalk msg;
				if (msg.Create(SYSTEM_NAME, "", "无法找到此物品", NULL, 0xff0000, _TXTATR_TOPSHOW))
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pInfo->stWpSl[0].idItem);
			}
		}
		break;
	case EM_MSGWPSL_QUERY:
		{
			CUserPtr pThisUser = NULL;
			if(m_pInfo->idUser == ID_NONE)
			{
				pThisUser = pUser;
			}
			else
			{
				pThisUser = UserManager()->GetUser(m_pInfo->idUser);
			}

			if(!pThisUser)
			{
				CMsgWeaponSoul msgWorld;
				ST_WEAPONSOULDATA stWSLData;
				stWSLData.idItem = pUser->GetID();
				msgWorld.Create(stWSLData,EM_MSGWPSL_QUERY,m_pInfo->idUser,m_pInfo->idItem);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msgWorld,(OBJID)0);
				return;
			}
				

			CItem* pItem = pThisUser->GetItem(m_pInfo->idItem,true,true);
			if(!pItem)
			{
				pItem = pThisUser->GetEquipItem(m_pInfo->idItem);
				if(!pItem)
				{
					CHECK(pThisUser->QueryStorage());
					pItem = pThisUser->QueryStorage()->GetItemByIdItem(m_pInfo->idItem);
				}
			}

			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(pData)
				{
					if(pData->GetInfo(wpslInfo))
					{
						CMsgWeaponSoul msgSoul;
						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pThisUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
			} 
		}
		break;
	case EM_MSGWPSL_MIX:
		{
			CItem* pItem1 = pUser->GetItem(m_pInfo->idItem,true,true);
			CItem* pItem2 = pUser->GetItem(m_pInfo->idUser,true,true);
			

			if(pItem1 && pItem1->IsWeaponSoul() && pItem2 && pItem2->IsWeaponSoul())
			{
				if(pItem1->IsLocked())
					return;
				if(pItem2->IsLocked())
					return;

				ST_WEAPONSOULDATA wpslInfo1;
				ST_WEAPONSOULDATA wpslInfo2;
				CWeaponSoulData* pData1 = pItem1->QueryWeapensoulData();
				if(!pData1)
					return;
				if(!pData1->GetInfo(wpslInfo1,false))
					return;
				CWeaponSoulData* pData2 = pItem2->QueryWeapensoulData();
				if(!pData2)
					return;
				if(!pData2->GetInfo(wpslInfo2,false))
					return;

				//按需求来把物品类型给换了
				OBJID idItemType = pItem1->GetTypeID();
				if(idItemType < pItem2->GetReqLev())
					idItemType = pItem2->GetTypeID();


				if(wpslInfo1.mixlev != wpslInfo2.mixlev)
					pUser->SendSysMsg("合成等级不相同");

				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}

				CHECK(pUser->QueryPackage());
				if(pUser->QueryPackage()->IsPackFull(1,idItemType))
				{
					pUser->SendSysMsg("背包已满");
					return;
				}

				int nMoney = 0;
				nMoney = wpslInfo1.mixlev * 20000;
// 				switch(wpslInfo1.mixlev)
// 				{
// 				case 1:
// 				case 2:
// 					nMoney = 10000;
// 					break;
// 				case 3:
// 				case 4:
// 					nMoney = 20000;
// 					break;
// 				case 5:
// 				case 6:
// 				case 7:
// 				case 8:
// 				case 9:
// 					nMoney = 30000;
// 					break;
// 				}

				if(pUser->GetMoney() < nMoney)
					return;
				
				//合成
				wpslInfo1.mixlev += 1;
				wpslInfo1.ID = 0;
				wpslInfo1.idItem = 0;
				int nKindnap = 0;
				if(pItem1->GetKidnap() || pItem2->GetKidnap())
					nKindnap = 1;

				if(wpslInfo1.mixlev >= 3)
					nKindnap = 1;

				if(wpslInfo1.mixlev == 5)
				{
					wpslInfo1.nType = wpslInfo1.nType / 10 * 10 + RandGet(4) + 1;
				}
				CItem* pSoulGet = pUser->CreateNewSoul(idItemType,wpslInfo1,nKindnap,pItem1->GetGemStatus(EM_GEMPOS_GEM1),pItem1->GetGemStatus(EM_GEMPOS_GEM2),pItem1->GetGemStatus(EM_GEMPOS_GEM3));
				if(pSoulGet)
				{
					//消耗
					pUser->SpendMoney(nMoney,true);
					pUser->SpendItem(pItem1);
					pUser->SpendItem(pItem2);

					CHECK(pSoulGet->IsWeaponSoul());
					CWeaponSoulData* pDataSend = pSoulGet->QueryWeapensoulData();
					CMsgWeaponSoul msgSoul;
					if(pDataSend && pDataSend->GetInfo(wpslInfo1))
					{
						if(wpslInfo1.mixlev > 3)
							pUser->WeapensoulBroadCast(WEAPENSOULCAST_MIXLEV,pSoulGet,wpslInfo1,wpslInfo1.mixlev);

						if(msgSoul.Create(wpslInfo1,EM_MSGWPSL_UPDATE,pUser->GetID(),pSoulGet->GetID()))
							pUser->SendMsg(&msgSoul);
					}

					
				}
			}
		}
		break;
	case EM_MSGWPSL_PROPOPEN:
		{
			CItem* pItem = pUser->GetEquipItem(m_pInfo->idItem);
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(!pData)
					return;
				if(!pData->GetInfo(wpslInfo))
					return;

				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}

				if(pUser->GetMoney() < 50000)
					return;

				CItem* pItemSpend = pUser->GetItemByType(591003);
				if(!pItemSpend)
					return;
				if(pItemSpend->IsLocked())
					return;

				//取槽
				for(int i = 0 ; i < wpslInfo.mixlev ; i ++)
				{
					if(wpslInfo.addition[i] == 0)
						break;
				}
				
				if(i >= wpslInfo.mixlev )
					return;

				int nRand = 10000;
				switch(i)
				{
				case 0:
					return;
				case 1:
					nRand = 8000;
					break;
				case 2:
					nRand = 6500;
					break;
				case 3:
					nRand = 5000;
					break;
				case 4:
					nRand = 4000;
					break;
				case 5:
					nRand = 3000;
					break;
				case 6:
					nRand = 2000;
					break;
				case 7:
					nRand = 1500;
					break;
				case 8:
					nRand = 1000;
					break;
				case 9:
					nRand = 500;
					break;
				}
				
				if(RandGet(10000) > nRand)//失败
				{
					pUser->SpendMoney(50000,true);
					pUser->SpendItem(pItemSpend);
					pUser->SendSysMsg("操作失败");

					CMsgWeaponSoul msgSoul;
					if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
						pUser->SendMsg(&msgSoul);
					return;
				}
				//开属性条
				if(pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_PROPOPEN))
				{
					pUser->SpendMoney(50000,true);
					pUser->SpendItem(pItemSpend);

					CMsgWeaponSoul msgSoul;
					if(pData->GetInfo(wpslInfo))
					{
						if(i > 2)
							pUser->WeapensoulBroadCast(WEAPENSOULCAST_ADDTIONOPEN,pItem,wpslInfo,i + 1);

						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
				else
					pUser->SendSysMsg("系统操作失败");
			}
		}
		break;
	case EM_MSGWPSL_PROPRAND:
		{
			CItem* pItem = pUser->GetEquipItem(m_pInfo->idItem);
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(!pData)
					return;
				if(!pData->GetInfo(wpslInfo))
					return;
				
				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}

				//属性条随机
				CHECK(m_pInfo->idUser > 0 && m_pInfo->idUser < 11);
				if(wpslInfo.addition[m_pInfo->idUser - 1] == 0)
				{
					pUser->SendSysMsg("条件不满足");
					return;
				}
				if(wpslInfo.addition[m_pInfo->idUser - 1] == 88888)
				{
					pUser->SendSysMsg("条件不满足");
					return;
				}
				
				if(pUser->GetMoney() < 50000)
					return;
				
				CItem* pItemSpend = pUser->GetItemByType(591004);
				if(!pItemSpend)
					return;
				if(pItemSpend->IsLocked())
					return;
				
				if(pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_PROPRAND,m_pInfo->idUser - 1))
				{
					pUser->SpendMoney(50000,true);
					pUser->SpendItem(pItemSpend);

					DEBUG_TRY
					pUser->UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//更新角色卡
					DEBUG_CATCH("EM_MSGWPSL_PROPRAND")

					CMsgWeaponSoul msgSoul;
					if(pData->GetInfo(wpslInfo))
					{
						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
				else
					pUser->SendSysMsg("操作失败");
			}
		}
		break;
	case EM_MSGWPSL_PROPLEAN:
		{
			CItem* pItem = pUser->GetEquipItem(m_pInfo->idItem);
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(!pData)
					return;
				if(!pData->GetInfo(wpslInfo))
					return;
				
				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}
				
				//属性条随机
				CHECK(m_pInfo->idUser > 0 && m_pInfo->idUser < 11);
				if(wpslInfo.addition[m_pInfo->idUser - 1] != 88888)
				{
					pUser->SendSysMsg("条件不满足");
					return;
				}

				
				if(pUser->GetMoney() < 50000)
					return;
				
				CItem* pItemSpend = pUser->GetItemByType(591000);
				if(!pItemSpend)
					return;
				if(pItemSpend->IsLocked())
					return;
			
				
				if(pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_PROPRAND,m_pInfo->idUser - 1))
				{
					pUser->SpendMoney(50000,true);
					pUser->SpendItem(pItemSpend);
					
					DEBUG_TRY
					pUser->UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//更新角色卡
					DEBUG_CATCH("EM_MSGWPSL_PROPLEAN")

					CMsgWeaponSoul msgSoul;
					if(pData->GetInfo(wpslInfo))
					{
						pUser->WeapensoulBroadCast(WEAPENSOULCAST_ADDTIONLEAN,pItem,wpslInfo,m_pInfo->idUser - 1);

						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
				else
					pUser->SendSysMsg("操作失败");
			}
		}
		break;
	case EM_MSGWPSL_PROPUPLEV:
		{
			CItem* pItem = pUser->GetEquipItem(m_pInfo->idItem);
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(!pData)
					return;
				if(!pData->GetInfo(wpslInfo))
					return;
				
				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}
				

				//属性条随机
				CHECK(m_pInfo->idUser > 0 && m_pInfo->idUser < 11);
				if(wpslInfo.addition[m_pInfo->idUser - 1] == 0)
				{
					pUser->SendSysMsg("条件不满足");
					return;
				}

				OBJID itemSpend = ID_NONE;
				DWORD moneySpend = wpslInfo.addition[m_pInfo->idUser - 1] % 1000 * 20000;
				switch(wpslInfo.addition[m_pInfo->idUser - 1] % 1000)
				{
				case 1:
					itemSpend = 591101;
					break;
				case 2:
					itemSpend = 591102;
					break;
				case 3:
					itemSpend = 591103;
					break;
				case 4:
					itemSpend = 591104;
					break;
				case 5:
					itemSpend = 591105;
					break;
				case 6:
					itemSpend = 591106;
					break;
				case 7:
					itemSpend = 591107;
					break;
				case 8:
					itemSpend = 591108;
					break;
				case 9:
					itemSpend = 591109;
					break;
				default:
					return;
				}
				
				if(pUser->GetMoney() < moneySpend)
					return;

				CItem* pItemSpend = pUser->GetItemByType(itemSpend);
				if(!pItemSpend)
					return;
				if(pItemSpend->IsLocked())
					return;
				
				if(pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_PROPLEVUP,m_pInfo->idUser - 1))
				{
					pUser->SpendMoney(moneySpend,true);
					pUser->SpendItem(pItemSpend);
					

					DEBUG_TRY
					pUser->UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//更新角色卡
					DEBUG_CATCH("EM_MSGWPSL_PROPUPLEV")

					CMsgWeaponSoul msgSoul;
					if(pData->GetInfo(wpslInfo))
					{
						if(wpslInfo.addition[m_pInfo->idUser - 1] % 1000 > 3)
							pUser->WeapensoulBroadCast(WEAPENSOULCAST_ADDTIONLEV,pItem,wpslInfo,m_pInfo->idUser - 1);

						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
				else
					pUser->SendSysMsg("操作失败");
			}
		}
		break;
	case EM_MSGWPSL_SKILLLEAN:
		{
			CItem* pItem = pUser->GetEquipItem(m_pInfo->idItem);
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(!pData)
					return;
				if(!pData->GetInfo(wpslInfo))
					return;
				
				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}
				
				int nSkill = -1;
				int nMoney = 0;
				for(int i = 0; i < 3 ; i ++)
				{
					if(wpslInfo.skill[i] == 0)
					{
						nSkill = -1;
						break;
					}
					else if(wpslInfo.skill[i] == 88888)
					{
						nSkill = i;
						break;
					}
				}

				if(nSkill < 0 || nSkill > 2)
					return;

				switch(nSkill)
				{
				case 0:
					nMoney = 100000;
					break;
				case 1:
					nMoney = 500000;
					break;
				case 2:
					nMoney = 1000000;
					break;
				}
				
//				nMoney = 50000 * (nSkill + 1);
				if(pUser->GetMoney() < nMoney)
					return;
			
				//开技能条
				if(pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_SKILLRAND,nSkill))
				{
					pUser->SpendMoney(nMoney,true);
					

					if(nSkill == 0)
					{
						DEBUG_TRY
						pUser->UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//更新角色卡
						DEBUG_CATCH("EM_MSGWPSL_SKILLLEAN")
					}

					CMsgWeaponSoul msgSoul;
					if(pData->GetInfo(wpslInfo))
					{
						pUser->WeapensoulBroadCast(WEAPENSOULCAST_SKILLLEAN,pItem,wpslInfo,nSkill);

						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
				else
					pUser->SendSysMsg("操作失败");
			}
		}
		break;
	case EM_MSGWPSL_SKILLRAND:
		{
			CItem* pItem = pUser->GetEquipItem(m_pInfo->idItem);
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(!pData)
					return;
				if(!pData->GetInfo(wpslInfo))
					return;
				
				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}

				//一个技能都没有
				if(wpslInfo.skill[0] == 0)
					return;
				if(wpslInfo.skill[0] == 88888)
					return;
				
				if(pUser->GetMoney() < 50000)
					return;

				CItem* pItemSpend = pUser->GetItemByType(591001);
				if(!pItemSpend)
					return;
				if(pItemSpend->IsLocked())
					return;
				
				//开技能条
				if(pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_SKILLRAND,-1))
				{
					pUser->SpendMoney(50000,true);
					pUser->SpendItem(pItemSpend);
					

					DEBUG_TRY
					pUser->UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//更新角色卡
					DEBUG_CATCH("EM_MSGWPSL_SKILLRAND")

					CMsgWeaponSoul msgSoul;
					if(pData->GetInfo(wpslInfo))
					{
						pUser->WeapensoulBroadCast(WEAPENSOULCAST_SKILLRAND,pItem,wpslInfo,0);

						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
				else
					pUser->SendSysMsg("操作失败");
			}
		}
		break;
	case EM_MSGWPSL_SKILLLEVUP:
		{
			CItem* pItem = pUser->GetEquipItem(m_pInfo->idItem);
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(!pData)
					return;
				if(!pData->GetInfo(wpslInfo))
					return;
				
				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}
				
				//属性条随机
				CHECK(m_pInfo->idUser > 0 && m_pInfo->idUser < 4);
				if(wpslInfo.skill[m_pInfo->idUser - 1] == 0)
				{
					pUser->SendSysMsg("条件不满足");
					return;
				}
				if(wpslInfo.skill[m_pInfo->idUser - 1] == 88888)
				{
					pUser->SendSysMsg("条件不满足");
					return;
				}
				
				OBJID itemSpend = ID_NONE;
				DWORD moneySpend = wpslInfo.skill[m_pInfo->idUser - 1] % 1000 * 20000;
				switch(wpslInfo.skill[m_pInfo->idUser - 1] % 1000)
				{
				case 1:
					itemSpend = 591201;
					break;
				case 2:
					itemSpend = 591202;
					break;
				case 3:
					itemSpend = 591203;
					break;
				case 4:
					itemSpend = 591204;
					break;
				case 5:
					itemSpend = 591205;
					break;
				case 6:
					itemSpend = 591206;
					break;
				case 7:
					itemSpend = 591207;
					break;
				case 8:
					itemSpend = 591208;
					break;
				case 9:
					itemSpend = 591209;
					break;
				default:
					return;
				}

				if(pUser->GetMoney() < moneySpend)
					return;

				CItem* pItemSpend = pUser->GetItemByType(itemSpend);
				if(!pItemSpend)
					return;
				if(pItemSpend->IsLocked())
					return;
				
				//开技能条
				if(pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_SKILLLEVUP,m_pInfo->idUser - 1))
				{
					pUser->SpendMoney(moneySpend,true);
					pUser->SpendItem(pItemSpend);
					

					if(m_pInfo->idUser == 1)
					{
						DEBUG_TRY
						pUser->UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//更新角色卡
						DEBUG_CATCH("EM_MSGWPSL_SKILLLEVUP")
					}

					CMsgWeaponSoul msgSoul;
					if(pData->GetInfo(wpslInfo))
					{
						if(wpslInfo.skill[m_pInfo->idUser - 1] % 1000 > 3)
							pUser->WeapensoulBroadCast(WEAPENSOULCAST_SKILLLEAN,pItem,wpslInfo,m_pInfo->idUser - 1);

						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
				else
					pUser->SendSysMsg("操作失败");
			}
		}
		break;
	case EM_MSGWPSL_TYPERAND:
		{
			CItem* pItem = pUser->GetEquipItem(m_pInfo->idItem);
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(!pData)
					return;
				if(!pData->GetInfo(wpslInfo))
					return;
				CHECK(m_pInfo->idUser > 0 && m_pInfo->idUser <= 4);

				if(wpslInfo.nType % 10  == 0)
					return;

				if(wpslInfo.nType % 10  == m_pInfo->idUser)
					return;

				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}

				
				if(pUser->GetMoney() < 500000)
					return;
				
				CItem* pItemSpend = pUser->GetItemByType(591005);
				if(!pItemSpend)
					return;
				if(pItemSpend->IsLocked())
					return;

				int nOldType = wpslInfo.nType % 10;
				//开技能条
				if(pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_TYPERAND,m_pInfo->idUser))
				{
					pUser->SpendMoney(500000,true);
					pUser->SpendItem(pItemSpend);
				
					CMsgWeaponSoul msgSoul;
					if(pData->GetInfo(wpslInfo))
					{
						if(nOldType != wpslInfo.nType % 10)
						{
							CMsgWeapensoulShow msgwpsl;
							if(msgwpsl.Create(wpslInfo,EM_MSGWPSLSHOW_IN,pUser->GetID(),pItem->GetID()))
								pUser->BroadcastRoomMsg(&msgwpsl, INCLUDE_SELF);
						}

						pUser->WeapensoulBroadCast(WEAPENSOULCAST_TYPE,pItem,wpslInfo,0);

						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
				else
					pUser->SendSysMsg("操作失败");
			}
		}
		break;
	case EM_MSGWPSL_GROWUP:
		{
			CItem* pItem = pUser->GetEquipItem(m_pInfo->idItem);
			if(pItem && pItem->IsWeaponSoul())
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData* pData = pItem->QueryWeapensoulData();
				if(!pData)
					return;
				if(!pData->GetInfo(wpslInfo))
					return;
				
				CHECK(wpslInfo.grow >= 500);
				if(wpslInfo.grow  >= 1000)
					return;

				int nData = (wpslInfo.grow - 500) / 10;
				OBJID itemSpend = 591005;
				DWORD moneySpend = 0;
				if(nData >= 0 && nData < 15)//普通
					moneySpend = 20000;
				if(nData >= 15 && nData < 25)//优秀
					moneySpend = 30000;
				if(nData >= 25 && nData < 35)//杰出
					moneySpend = 40000;
				if(nData >= 35 && nData < 45)//卓越
					moneySpend = 50000;
				if(nData >= 45 && nData < 50)//完美
					moneySpend = 60000;

				int nChange = 5000 - nData * 100;
				
				if (pUser->IsItemLockedOpen())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
					return;
				}
				
				if(pUser->GetMoney() < moneySpend)
					return;
				
				CItem* pItemSpend = pUser->GetItemByType(591002);
				if(!pItemSpend)
					return;
				if(pItemSpend->IsLocked())
					return;

				if(RandGet(10000) > nChange)//失败
				{
					pUser->SpendMoney(moneySpend,true);
					pUser->SpendItem(pItemSpend);
					pUser->SendSysMsg("操作失败");

					CMsgWeaponSoul msgSoul;
					if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
						pUser->SendMsg(&msgSoul);
					return;
				}
				
				//开技能条
				if(pData->UpdateAll(wpslInfo,WEAPONSOULUPDATE_GROW,wpslInfo.grow + 10))
				{
					pUser->SpendMoney(moneySpend,true);
					pUser->SpendItem(pItemSpend);
					

					DEBUG_TRY
					pUser->UpdateCalculatedProperty(UPDATECALTYPE_EQUIP);//更新角色卡
					DEBUG_CATCH("EM_MSGWPSL_GROWUP")

					CMsgWeaponSoul msgSoul;
					if(pData->GetInfo(wpslInfo))
					{
						if(wpslInfo.grow == 750
							|| wpslInfo.grow == 850
							|| wpslInfo.grow == 950)
							pUser->WeapensoulBroadCast(WEAPENSOULCAST_GROW,pItem,wpslInfo,0);

						if(msgSoul.Create(wpslInfo,EM_MSGWPSL_UPDATE,pUser->GetID(),pItem->GetID()))
							pUser->SendMsg(&msgSoul);
					}
				}
				else
					pUser->SendSysMsg("系统操作失败");
			}
		}
		break;
	case EM_MSGWPSL_MIXITEM:
		{
			OBJID nNeedItem = ID_NONE;
			if(m_pInfo->idItem > 591101 && m_pInfo->idItem <= 591109)
				nNeedItem = m_pInfo->idItem - 1;
			else if(m_pInfo->idItem > 591201 && m_pInfo->idItem <= 591209)
				nNeedItem = m_pInfo->idItem - 1;

			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return;
			}

			if(nNeedItem == ID_NONE)
				return;

			int nLev = nNeedItem % 10;
			DWORD moneySpend = nLev * 20000;
		

			if(pUser->GetMoney() < moneySpend)
				return;
			CHECK(pUser->QueryPackage());
			if(pUser->QueryPackage()->IsPackFull(1,m_pInfo->idItem))
			{
				pUser->SendSysMsg("背包已满");
				return;
			}

			bool bHasDelKidnapItem = false;
			if(pUser->DelItemByTypeMulti(nNeedItem,3,bHasDelKidnapItem))
			{
				CMsgAction msgAction;
				if(msgAction.Create(pUser->GetID(), 0, 0, 0, actionWeapensoulMixSuc))
					pUser->SendMsg(&msgAction);

				pUser->SpendMoney(moneySpend,true);
				pUser->AwardItemBySys(m_pInfo->idItem,true,false,true,false,1,0,0,bHasDelKidnapItem);
			}
		}
		break;
	default:
		LOGWARNING("CMsgWeaponSoul::Process(void *pInfo) nAction = %u",m_pInfo->usAction);
		break;
	}
}
