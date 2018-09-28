////////////////////////////////////////////////////////////////
//
//		MsgPackage.cpp
//

#include "AllMsg.h"
#include "package.h"
#include "MapGroup.h"
#include "Npc.h"
#include "LoverHome.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgPackage::CMsgPackage()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgPackage::~CMsgPackage()
{

}

BOOL CMsgPackage::Create(int nAction, OBJID id, OBJID idType, UINT usAmount, UINT usMaxAmount, int nIndex, int nPosition/*=MSGPACKAGE_TYPE_STORAGE*/, int isBind/*=0*/)
{
	this->Init();

	m_unMsgSize			= sizeof(MSG_Info);
	m_unMsgType			= _MSG_PACKAGE;

	m_pInfo->id			= id;
	m_pInfo->idType		= idType;
	m_pInfo->usAmount	= usAmount;
	m_pInfo->usMaxAmount= usMaxAmount;
	m_pInfo->usIndex	= nIndex;
	m_pInfo->ucPosition	= nPosition;
	m_pInfo->ucIsBind	= isBind;
	m_pInfo->ucAction	= nAction;

	return true;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgPackage::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_PACKAGE != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgPackage::Process(CGameSocket* pSocket)
{
	DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVVVVVVVV
	CUserPtr pUser = UserManager()->GetUser(this);
	CHECK(pUser);

	switch(m_pInfo->ucPosition)
	{
	case	MSGPACKAGE_TYPE_STORAGE:
		switch(m_pInfo->ucAction)
		{
		case	MSGPACKAGE_QUERYLIST:
			{
				if(m_pInfo->id!=pUser->GetID())
					return;
				if( m_pInfo->usIndex<1 || m_pInfo->usIndex>4 )//仓库最多只有3页,第4页表示宠物栏
					return;

				if (m_pInfo->usIndex==4)//查询仓库宠物栏
				{
					CUserPet* pUserPet = pUser->QueryUserPetSet();
					if (!pUserPet)
						return;
					if (!pUserPet->sendStorageInfo())
						pUser->SendSysMsg("同步仓库宠物栏失败");
					return;
				}
 				if( pUser->QueryStorage()->SendStorageInfo(m_pInfo->id, m_pInfo->usIndex) )//发送物品列表
				{
					CMsgPackage msg;
					msg.Create(MSGPACKAGE_MONEY_AMOUNT,pUser->GetMoneySaved(),0,0,0,0,MSGPACKAGE_TYPE_STORAGE,0);//发送存款
					pUser->SendMsg(&msg);
				}
				else
				{
					pUser->InitStorage(Database());
					if( pUser->QueryStorage()->SendStorageInfo(m_pInfo->id, m_pInfo->usIndex) )//发送物品列表
					{
						CMsgPackage msg;
						msg.Create(MSGPACKAGE_MONEY_AMOUNT,pUser->GetMoneySaved(),0,0,0,0,MSGPACKAGE_TYPE_STORAGE,0);//发送存款
						pUser->SendMsg(&msg);
					}
					else
						pUser->SendSysMsg("同步仓库物品失败");
				}
			}
			break;
		case	MSGPACKAGE_CHECKIN_MONEY://[2010-06-16 goto]存钱
			{
				if( m_pInfo->id > 0 )
				{
					pUser->SaveMoney(m_pInfo->id, SYNCHRO_TRUE);
				}

				CMsgPackage msg;
				msg.Create(MSGPACKAGE_CHECKIN_MONEY,pUser->GetMoneySaved(),0,0,0,0,MSGPACKAGE_TYPE_STORAGE,0);//发送存款
				pUser->SendMsg(&msg);
			}
			break;
		case	MSGPACKAGE_CHECKOUT_MONEY://[2010-06-16 goto]取钱
			{
				if( m_pInfo->id > 0 )
				{
					pUser->DrawMoney(m_pInfo->id ,SYNCHRO_TRUE);
				}
				CMsgPackage msg;
				msg.Create(MSGPACKAGE_CHECKOUT_MONEY,pUser->GetMoneySaved(),0,0,0,0,MSGPACKAGE_TYPE_STORAGE,0);//发送存款
				pUser->SendMsg(&msg);
			}
			break;
		case	MSGPACKAGE_CHECKIN:
			{
				OBJID	idItem = m_pInfo->id;
				CItemPtr	pItem = pUser->QueryPackage()->GetItem(idItem);
				if(!pItem)
					return;
//modify code by 林德渊 begin 2011.5.24
				bool b = pItem->IsCostlyItem();
				int itemtypeid = pItem->GetTypeID();
				int itemamount = pItem->GetItemAmount();
//modify code by 林德渊 end
				if (!pItem->IsStorageEnable())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "此物品不能存仓库");
					return;
				}
				int nPackageSize = PACKAGE_ORIGIN+pUser->GetBanklev(1)*6;
				if ((m_pInfo->usIndex<1 || m_pInfo->usIndex>nPackageSize) && m_pInfo->usIndex!=PACKAGE_DEFAULTINDEX)//如果是PACKAGE_DEFAULTINDEX,查找第一个空位放物品
					return;
				if (m_pInfo->usIndex==PACKAGE_DEFAULTINDEX)
				{
					if( !pUser->QueryStorage()->CheckIn(m_pInfo->usIndex+m_pInfo->idType, pItem, true) )
					{
						//	pUser->SendSysMsg("存入物品失败");
						CMsgItem msg;
						IF_OK( msg.Create( ITEMACT_SYNCHRO_UNLOCK, 0, 0, 0, 0, idItem ) )
							SendMsg( &msg );
					}
//modify code by 林德渊 begin 2011.5.24
					if ( b )
						::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) checkin item %u(%d) to Storage", pUser->GetID(), pUser->GetName(), itemtypeid ,itemamount);
//modify code by 林德渊 end
					return;
				}
				if( !pUser->QueryStorage()->CheckIn(m_pInfo->usIndex, pItem) )
				{
				//	pUser->SendSysMsg("存入物品失败");
					CMsgItem msg;
					IF_OK( msg.Create( ITEMACT_SYNCHRO_UNLOCK, 0, 0, 0, 0, idItem ) )
					SendMsg( &msg );
				}
//modify code by 林德渊 begin 2011.5.24
				if ( b )
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) checkin item %u(%d) to Storage", pUser->GetID(), pUser->GetName(), itemtypeid ,itemamount);
//modify code by 林德渊 end
 			}
			break;
		case	MSGPACKAGE_CHECKOUT:
 			{
				OBJID	idItem = m_pInfo->id;
				CItemPtr	pItem = pUser->QueryStorage()->GetItemByIdItem(idItem);
				if (!pItem)
					return;
//modify code by 林德渊 begin 2011.5.24
				bool b = pItem->IsCostlyItem();
				int itemtypeid = pItem->GetTypeID();
				int itemamount = pItem->GetItemAmount();
//modify code by 林德渊 end
				int nPosition = pUser->QueryPackage()->GetPositionByItemType(pItem->GetTypeID());
				if (nPosition<ITEMPOSITION_BACKPACK1 || nPosition>ITEMPOSITION_BACKPACK3)
					return;

				int nPackage = (m_pInfo->usIndex-1)/36 + 1;//算出页码
				int nPackageSize = PACKAGE_ORIGIN+pUser->GetPacklev(4-nPackage);
				if (m_pInfo->usIndex<1 || m_pInfo->usIndex>(nPackage-1)*36+nPackageSize || m_pInfo->usIndex>72)
					return;
				pUser->QueryStorage()->CheckOut(0, idItem, m_pInfo->usIndex);
//modify code by 林德渊 begin 2011.5.24
				if ( b )
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) checkout item %u(%d) from Storage", pUser->GetID(), pUser->GetName(), itemtypeid ,itemamount);
//modify code by 林德渊 end
 			}
			break;
		case MSGPACKAGE_MOVE://[2010-06-16 goto]单个物品移动位置
			{
				CItemPtr pItem = pUser->QueryStorage()->GetItemByIdItem(m_pInfo->id);
				if (!pItem)
					return;
				int nPackageSize = PACKAGE_ORIGIN+pUser->GetBanklev(1)*6;
				if (m_pInfo->usIndex<1 || m_pInfo->usIndex>nPackageSize)
					return;
				if (!pUser->QueryStorage()->MoveStorageItem(pUser, pItem, m_pInfo->usIndex))
				{
					pUser->SendSysMsg("移动物品失败");
				}
			}
			break;
		case MSGPACKAGE_EXCHANGE://[2010-06-16 goto]交换位置或合并
			{
				CItemPtr pItem = pUser->QueryStorage()->GetItemByIdItem(m_pInfo->id);
				CItemPtr pTargetItem = pUser->QueryStorage()->GetItemByIdItem(m_pInfo->idType);
				if (!pItem || !pTargetItem)
					return;
				if (pItem->GetTypeID() == pTargetItem->GetTypeID()
					&& pItem->GetKidnap()==pTargetItem->GetKidnap())//合并
				{
					pUser->QueryStorage()->CombineItem(pUser, pItem, pTargetItem);
				}
				else//交换位置
				{
					if (!pUser->QueryStorage()->ExChangeIndex(pUser, pItem, pTargetItem))
					{
						pUser->SendSysMsg("交换位置失败");
					}
				}
			}
			break;
		case MSGPACKAGE_CHECKIN_EUDEMON:
			{
				CUserPet* pUserPet = pUser->QueryUserPetSet();
				if (!pUserPet)
					return;
				
				if (pUserPet->GetStorageAmount()>=2 + pUser->GetBanklev(2))
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "仓库宠物栏已满");
					return;
				}

				CUserPetData* pPetData = pUserPet->QueryData(m_pInfo->id);
				if (!pPetData)
					return;

				if (pPetData->GetInt(EUDEMONDATA_state)!=PETSTATE_REST)//只有休息状态的宠物才可以存
				{
					return;
				}

				pPetData->SetInt(EUDEMONDATA_state, PETSTATE_INSTORAGE, true);

				DEBUG_TRY
				pUser->UpdateMissionMsgByItemType(pPetData->GetInt(EUDEMONDATA_typeid));
				DEBUG_CATCH("pUser->UpdateMissionMsgByItemType eudemon")

				EudemonData data;
				pUser->LoadEudemon(pPetData->GetID(),data,2);
				CMsgEudemon msg;
				if(msg.Create(PETACTION_DelPet,&data,pUser->GetID()))
					pUser->SendMsg(&msg);//背包减宠物

				msg.Create(PETACTION_CheckinPet,&data,pUser->GetID());//仓库加宠物
				pUser->SendMsg(&msg);
//modify code by 林德渊 begin 2011.5.24
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) checkin pet %u(grow:%u)(understand:%u)(level:%u)(sex:%u)(exp:%u) to Storage", pUser->GetID(), pUser->GetName(), pPetData->GetID(), data.grow, data.understand, data.level, data.sex, data.exp );
//modify code by 林德渊 end
			}
			break;
		case MSGPACKAGE_CHECKOUT_EUDEMON:
			{
				CUserPet* pUserPet = pUser->QueryUserPetSet();
				if (!pUserPet)
					return;

				if (pUserPet->GetShowAmount()>=3 + pUser->GetPacklev(4))
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "宠物栏已满");
					return;
				}

				CUserPetData* pPetData = pUserPet->QueryData(m_pInfo->id);
				if (!pPetData)
					return;

				if (pPetData->GetInt(EUDEMONDATA_state)!=PETSTATE_INSTORAGE)
				{
					return;
				}
				
				pPetData->SetInt(EUDEMONDATA_state, PETSTATE_REST, true);

				DEBUG_TRY
				pUser->UpdateMissionMsgByItemType(pPetData->GetInt(EUDEMONDATA_typeid));
				DEBUG_CATCH("pUser->UpdateMissionMsgByItemType eudemon")

				EudemonData data;
				pUser->LoadEudemon(pPetData->GetID(),data,2);
				CMsgEudemon msg;
				if(msg.Create(PETACTION_PetInfo,&data,pUser->GetID()))
					pUser->SendMsg(&msg);//背包加宠物

				msg.Create(PETACTION_CheckoutPet,&data,pUser->GetID());//仓库减宠物
				pUser->SendMsg(&msg);
//modify code by 林德渊 begin 2011.5.24
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) checkout pet %u(grow:%u)(understand:%u)(level:%u)(sex:%u)(exp:%u) from Storage", pUser->GetID(), pUser->GetName(), pPetData->GetID(), data.grow, data.understand, data.level, data.sex, data.exp );
//modify code by 林德渊 end
			}
			break;
		case MSGPACKAGE_SYNCHRO_AMOUNT:
			break;
		default:
			ASSERT(!"action");
		}
		break;
	default:
		ASSERT(!"type");
	}
	DEBUG_CATCH3("MsgPackage. type[%d], subtype[%d]", m_pInfo->ucPosition, m_pInfo->ucAction)	// AAAAAAAAAAAAAAA
}
