// MsgItem.cpp: interface for the CMsgItem class.
//
//////////////////////////////////////////////////////////////////////

#include "ItemData.h"
#include "MsgItem.h"
#include "Npc.h"
#include "MapGroup.h"
#include "ItemType.h"
#include "Console.h"
#include "MsgEudemonInfo.h"
#include "MsgTalk.h"
#include  "msgiteminfo.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgItem::CMsgItem()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgItem::~CMsgItem()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItem::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_ITEM != this->GetType())
		return false;

	return true;
}

BOOL CMsgItem::Add(OBJID id,OBJID typeId,DWORD	nAmount,DWORD nMaxAmount,UCHAR ucPosition, UCHAR ucBind, USHORT	usIndex,DWORD dwMoney,DWORD dwColor/* = 0*/)
{
	if(id == 0)
		return false;
	if(m_pInfo->usAmount != 0)
	{
		if(m_unMsgSize + sizeof(itemMiniInfo) > MAX_PACKETSIZE)
			return false;
		if(m_pInfo->usAmount > 25)
			return false;
		m_unMsgSize		+= sizeof(itemMiniInfo);
	}
	

	// fill info now
	m_pInfo->m_itemInfo[m_pInfo->usAmount].id				= id;
	m_pInfo->m_itemInfo[m_pInfo->usAmount].typeId			= typeId;
	m_pInfo->m_itemInfo[m_pInfo->usAmount].nAmount			= nAmount;
	m_pInfo->m_itemInfo[m_pInfo->usAmount].nMaxAmount		= nMaxAmount;
	m_pInfo->m_itemInfo[m_pInfo->usAmount].ucPosition		= ucPosition;
	m_pInfo->m_itemInfo[m_pInfo->usAmount].ucBind			= ucBind;
	m_pInfo->m_itemInfo[m_pInfo->usAmount].usIndex			= usIndex;
	m_pInfo->m_itemInfo[m_pInfo->usAmount].dwMoney			= dwMoney;
	m_pInfo->m_itemInfo[m_pInfo->usAmount].dwColor			= dwColor;
	m_pInfo->usAmount++;
	return true;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgItem::Create(USHORT nAction,DWORD dwData, OBJID idTarget,USHORT usPosX,USHORT usPosY, OBJID id, OBJID typeId, DWORD nAmount, DWORD	nMaxAmount, UCHAR	ucPosition, UCHAR	ucBind, USHORT	usIndex ,DWORD dwMoney,DWORD dwColor/* = 0*/)
{
	CHECKF(nAction);	

	// init
	this->Init();
	// fill info now

	m_pInfo->usAction			= nAction;
	m_pInfo->dwData				= dwData;
	m_pInfo->idTarget			= idTarget;	
	m_pInfo->usPosX				= usPosX;
	m_pInfo->usPosY				= usPosY;

	if(id == ID_NONE)
		m_pInfo->usAmount			= 0;
	else
	{
		m_pInfo->usAmount			= 1;
		m_pInfo->m_itemInfo[0].id			= id;
		m_pInfo->m_itemInfo[0].typeId		= typeId;
		m_pInfo->m_itemInfo[0].nAmount		= nAmount;
		m_pInfo->m_itemInfo[0].nMaxAmount	= nMaxAmount;
		m_pInfo->m_itemInfo[0].ucPosition	= ucPosition;
		m_pInfo->m_itemInfo[0].ucBind		= ucBind;
		m_pInfo->m_itemInfo[0].usIndex		= usIndex;
		m_pInfo->m_itemInfo[0].dwMoney      = dwMoney;
		m_pInfo->m_itemInfo[0].dwColor		= dwColor;
	}

	
	m_unMsgType	=_MSG_ITEM;
	m_unMsgSize	=sizeof(MSG_Info);

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgItem::Process(void *pInfo)
{
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;

// 	if(!pUser->IsAlive())
// 	{
// 		pUser->SendSysMsg(g_objStrRes.GetStr(12069));//STR_DIE
// 		CMsgItem msg;
// 		IF_OK( msg.Create( m_pInfo->m_itemInfo[0].id,ITEMACT_SYNCHRO_UNLOCK ) )
// 			pUser->SendMsg( &msg );
// 		if(m_pInfo->usAction != ITEMACT_SAVEITEMWSAGET && m_pInfo->usAction != ITEMACT_SAVEITEMMONEYGET)
// 		        return ;
//	}

	try {
	switch(m_pInfo->usAction)
	{
	//������װ�� Ӧ�ô���Ҵ�������������ȥ����ȫ��װ��
	case ITEMACT_VIEWITEMINFO:
		{
			pUser->GetItemInfoByItemid(m_pInfo->m_itemInfo[0].id);
		}
		break;
	case ITEMACT_DROP:
		{
			if(!pUser->DropItem(m_pInfo->m_itemInfo[0].id))
			{
				pUser->SendUnlockItemMsg( m_pInfo->m_itemInfo[0].id );
			}
		}
		break;
	case ITEMACT_ORDER:
		{
			pUser->OrderPackage(m_pInfo->dwData);
		}
		break;
// 	case ITEMACT_DROPMONEY:
// 		{
// 			IF_NOT(pUser->GetDistance(m_pInfo->usPosX, m_pInfo->usPosY) <= USERDROPITEM_RANGE2)
// 				return ;
// 
// 			pUser->DropMoney(m_pInfo->m_itemInfo[0].id, m_pInfo->usPosX, m_pInfo->usPosY);
// 		}
// 		break;
	case ITEMACT_USE:
		{
			if(m_pInfo->m_itemInfo[0].id == ID_NONE)
				return;

			if(!pUser || !pUser->CanBeSee())
				return;

			if (m_pInfo->idTarget == ID_NONE || m_pInfo->idTarget == pUser->GetID())
			{
				int nstep = 0;
				DEBUG_TRY
				CItemPtr pItem = pUser->GetItem(m_pInfo->m_itemInfo[0].id,false);
				if (!pItem)
				{
					break;
				}
				nstep = 1;
				OBJID idItem = pItem->GetTypeID();
				nstep = 11;
				if(pUser->UseItem(m_pInfo->m_itemInfo[0].id, m_pInfo->dwData, SYNCHRO_TRUE))
				{
// 					nstep = 2;
// 					CItemPtr pItemSend = pUser->GetItem(m_pInfo->m_itemInfo[0].id);
// 					if(!pItemSend)
// 						return;

					nstep = 3;
					CMsgItem msg;
					IF_OK(msg.Create(ITEMACT_USE_OK ,0,0,0,0,m_pInfo->m_itemInfo[0].id))
					{
						pUser->SendMsg(&msg);
					}
				}
				else
				{
					nstep = 4;
					CMsgItem msg;
					IF_OK( msg.Create( ITEMACT_SYNCHRO_UNLOCK ,0,0,0,0,m_pInfo->m_itemInfo[0].id) )
						pUser->SendMsg( &msg );
				}
				DEBUG_CATCH2("ITEMACT_USE error!! step = %u",nstep);
			}
			else
			{
				DEBUG_TRY
				if (!pUser->UseItemTo(m_pInfo->idTarget, m_pInfo->m_itemInfo[0].id))
				{
					//pUser->SendSysMsg(/*g_objStrRes.GetStr(12083)*/"Ŀ�����");//STR_UNABLE_USE_ITEM
					CMsgItem msg;
					IF_OK( msg.Create( ITEMACT_SYNCHRO_UNLOCK ,0,0,0,0,m_pInfo->m_itemInfo[0].id ) )
						SendMsg( &msg );
				}
				DEBUG_CATCH2("ITEMACT_USE error!! step = %u",5);
			}
		}
		break;
	case ITEMACT_UNEQUIP:
		{
			if(!pUser->UnEquipItem(m_pInfo->dwData, SYNCHRO_TRUE, m_pInfo->usPosX, m_pInfo->usPosY))
				pUser->SendSysMsg(g_objStrRes.GetStr(12084));//STR_YOUR_BAG_FULL
		}
		break;
	case ITEMACT_QUERYITEMINFO:
		{
			if(pUser && pUser->CanBeSee()==false)
				return;

			if(m_pInfo->m_itemInfo[0].id == ID_NONE)
				return;

			///�յ�һ���ӱ�������Ĳ�ѯ��Ϣ
			CItem* pItem = pUser->GetItem(m_pInfo->m_itemInfo[0].id,true,true);
			if(!pItem)
			{
				pItem = pUser->GetEquipItem(m_pInfo->m_itemInfo[0].id);
				if(!pItem)
				{
					CHECK(pUser->QueryStorage());
					pItem = pUser->QueryStorage()->GetItemByIdItem(m_pInfo->m_itemInfo[0].id);
				}
			}

			if(pItem)
			{
				CMsgItemInfo msg;
				if(msg.Create(pItem))
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pInfo->m_itemInfo[0].typeId);
			}
			else
			{
				///�����Ѿ���������,����֪ͨ��ѯ��
				CMsgTalk msg;
				if (msg.Create(SYSTEM_NAME, m_pInfo->szTargetName, "�޷��ҵ�����Ʒ", NULL, 0xff0000, _TXTATR_TOPSHOW))
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pInfo->m_itemInfo[0].typeId);
			}

		}
		break;
	case ITEMACT_QUERYEUDEMONINFO:
		{
			if(pUser && pUser->CanBeSee()==false)
				return;
			///�յ�һ���ӱ�������Ĳ�ѯ��Ϣ
			EudemonData eudemonData={0};
			if(pUser->LoadEudemon(m_pInfo->m_itemInfo[0].id,eudemonData))
			{
				///�ɹ���ѯ
				CMsgEudemonInfo msg;
				if(msg.Create(eudemonData))
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pInfo->m_itemInfo[0].typeId);
			}
			else
			{
				///�����Ѿ���������,����֪ͨ��ѯ��
				CMsgTalk msg;
				if (msg.Create(SYSTEM_NAME, m_pInfo->szTargetName, "���ﱻ����������", NULL, 0xff0000, _TXTATR_TOPSHOW))
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pInfo->m_itemInfo[0].typeId);
			}
// 			CUserPetData* pData = m_pUserPet->QueryData(idEudemon);
// 			if(!pData)
// 			{
// 				pUser->SendSysMsg("���ﱻ����������");
// 				return;
// 			}
// 			
// 			EudemonData data;
// 			pData->GetData(data);
// 			m_pUserPet->fillType(data,3);
// 			m_pUserPet->fillAtt(data,3);
// 			if(data.life > data.dwMaxLife)
// 			{
// 				pData->SetInt(EUDEMONDATA_life,data.dwMaxLife,true);
// 				data.life = data.dwMaxLife;
// 			}
			

		}
		break;
	case ITEMACT_GETEUDEMONINFO:
		{
			CUserPtr pNeedUser = NULL;
			
			if(m_pInfo->idTarget != ID_NONE)
				pNeedUser = UserManager()->GetUser(m_pInfo->idTarget);
			if (!pNeedUser)
				pNeedUser = UserManager()->GetUser(m_pInfo->szTargetName);
			
			if(pNeedUser && pNeedUser->CanBeSee()==false)
				return;

			if(pNeedUser)
			{
				pNeedUser->SendEudemonTo(pUser,m_pInfo->m_itemInfo[0].id);
			}
			else
			{
				///����itemtype����������Դ�����û���id,��Ӳ
				m_pInfo->m_itemInfo[0].typeId=pUser->GetID();
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this,(OBJID)0);
//				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է�����̫Զ������");
			}
			pUser->SendMsg(this);
		}
		break;
	case ITEMACT_GETEUDEMONLIST:
		{
			CUserPtr pNeedUser = NULL;
			
			if(m_pInfo->idTarget != ID_NONE)
				pNeedUser = UserManager()->GetUser(m_pInfo->idTarget);
			if (!pNeedUser)
				pNeedUser = UserManager()->GetUser(m_pInfo->szTargetName);
			
			if(pNeedUser)
			{
				pNeedUser->SendEudemonListTo(pUser);
			}
			else
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է�����̫Զ������");
			}
			pUser->SendMsg(this);
		}
		break;
	case ITEMACT_GETINFO:
		{
			CUserPtr pNeedUser = NULL;

			if(m_pInfo->idTarget != ID_NONE)
				pNeedUser = UserManager()->GetUser(m_pInfo->idTarget);
			if (!pNeedUser)
				pNeedUser = UserManager()->GetUser(m_pInfo->szTargetName);

			if(pNeedUser && pNeedUser->CanBeSee()==false)
				return;

			if(pNeedUser)
			{
				CItem* pItem = pNeedUser->GetItem(m_pInfo->m_itemInfo[0].id,true,true);
				if(!pItem)
				{
					pItem = pNeedUser->GetEquipItem(m_pInfo->m_itemInfo[0].id);
					if(!pItem)
					{
						CHECK(pNeedUser->QueryStorage());
						pItem = pNeedUser->QueryStorage()->GetItemByIdItem(m_pInfo->m_itemInfo[0].id);
					}
				}
				if(pItem)
				{
					pUser->SendDetailItem(pItem);
				}
				else
				{
					pUser->SendSysMsg("�޷��ҵ�����Ʒ");
				}
			}
			else
			{
				///����itemtype����������Դ�����û���id,��Ӳ
				m_pInfo->m_itemInfo[0].typeId=pUser->GetID();
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this,(OBJID)0);

//				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��ڸ���������");
			}
			pUser->SendMsg(this);
		}
		break;

// 	case ITEMACT_PET_EXCHANGE:
// 		{
// 			pUser->ExchangePet();
// 		}
// 		break;
// 
	case ITEMACT_REQUEST_SPLITITEM:
		{
			pUser->SplitItem(m_pInfo->m_itemInfo[0].id, m_pInfo->dwData);	//true
		}
		break;
	case ITEMACT_REQUEST_COMBINEITEM:
		{
			pUser->CombineItem( m_pInfo->m_itemInfo[0].id,m_pInfo->dwData );//true
		}
		break;
	case ITEMACT_SELL:
		{
			if(pUser)
				pUser->SellItem(m_pInfo->idTarget,m_pInfo->m_itemInfo[0].id,m_pInfo->m_itemInfo[0].dwMoney);//true
		}
		break;
	case ITEMACT_BUY:
		{
			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
				return;
			}
			if(pUser)
				pUser->BuyItem(m_pInfo->idTarget,m_pInfo->m_itemInfo[0].typeId,m_pInfo->m_itemInfo[0].nAmount,m_pInfo->dwData,m_pInfo->m_itemInfo[0].dwMoney);//true
		}
		break;
	case ITEMACT_REPAIR:
		{
			if(pUser)
				pUser->RepairItem(m_pInfo->idTarget,m_pInfo->m_itemInfo[0].id,true);
		}
		break;
	case ITEMACT_REPAIRALL:
		{
			if(pUser)
				pUser->RepairAll(m_pInfo->idTarget ,true);//true
		}
		break;
// 	case ITEMACT_QUERYMONEYSAVED:
// 		{
// //			CNpc* pNpc; 
// //			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_NPC, m_pInfo->id, IPP_OF(pNpc)))
// //				return ;
// //			if(!pNpc->IsStorageNpc())
// //				return;
// 			if(m_pInfo->id != 1051)
// 				return ;
// 
// 			m_pInfo->dwData		= pUser->GetMoneySaved();
// 			m_pInfo->idTarget	= pUser->GetDiamondSaved();
// 			pUser->SendMsg(this);
// 		}
// 		break;
// 	case ITEMACT_SAVEMONEY:		//����
// 		{
// //			CNpc* pNpc; 
// //			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_NPC, m_pInfo->id, IPP_OF(pNpc)))
// //				return ;
// //			if(!pNpc->IsStorageNpc())
// //				return;
// 			if(m_pInfo->id != 1051)
// 				return ;
// 
// 			pUser->SaveMoney(m_pInfo->dwData, SYNCHRO_TRUE);
// 		}
// 		break;
// 	case ITEMACT_DRAWMONEY:		//ȡ���
// 		{
// //			CNpc* pNpc; 
// //			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_NPC, m_pInfo->id, IPP_OF(pNpc)))
// //				return ;
// //			if(!pNpc->IsStorageNpc())
// //				return;
// 			if(m_pInfo->id != 1051)
// 				return ;
// 
// 			pUser->DrawMoney(m_pInfo->dwData, SYNCHRO_TRUE);
// 		}
// 		break;
// 	case ITEMACT_SAVEDIAMOND:   //����ʯ
// 		{
// 			CNpc* pNpc; 
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_NPC, m_pInfo->id, IPP_OF(pNpc)))
// 				return ;
// 			if(!pNpc->IsStorageNpc())
// 				return;
// 			pUser->SaveDiamond( m_pInfo->dwData,true );
// 		}
// 		break;
// 	case ITEMACT_DRAWDIAMOND:  //ȡ��ʯ
// 		{
// 			CNpc* pNpc; 
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_NPC, m_pInfo->id, IPP_OF(pNpc)))
// 				return ;
// 			if(!pNpc->IsStorageNpc())
// 				return;
// 			pUser->DrawDiamond( m_pInfo->dwData,true );
// 		}
// 		break;
// 	case ITEMACT_SPENDMONEY:
// 		{
// 			ASSERT(!"ITEMACT_SPENDMONEY");
// 		}
// 		break;
// 	case ITEMACT_IDENT:
// 		{
// 			pUser->IdentItem(m_pInfo->id, SYNCHRO_TRUE);
// 		}
// 		break;
// 	case ITEMACT_IMPROVE:
// 		{
// 			pUser->UpEquipmentQuality(m_pInfo->id, m_pInfo->dwData);
// 		}
// 		break;
// 	case ITEMACT_UPLEV:
// 		{
// 			pUser->UpEquipmentLevel(m_pInfo->id, m_pInfo->dwData);
// 		}
// 		break;
	case ITEMACT_BOOTH_ADDEUDE:
		{
			if(pUser->QueryBooth())
			{
				if(pUser->QueryBooth()->AddEudemon(m_pInfo->m_itemInfo[0].id, m_pInfo->dwData))
					pUser->SendMsg(this);
			}
		}
		break;
	case ITEMACT_BOOTH_DELEUDE:
		{
			if(pUser->QueryBooth())
			{
				if(pUser->QueryBooth()->DelEudemon(m_pInfo->m_itemInfo[0].id))
					pUser->SendMsg(this);
			}
		}
		break;
	case ITEMACT_BOOTH_BUYEUDE:
		{
			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
				return;
			}
			CBooth* pBooth;
			if(!pUser->FindAroundObj(OBJ_BOOTH, m_pInfo->dwData, IPP_OF(pBooth)))		// npc id
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"̯λ�Ѿ�����.");
				return ;
			}

			pBooth->BuyEudemon(pUser, m_pInfo->m_itemInfo[0].id,m_pInfo->m_itemInfo[0].dwMoney);
			pUser->SendMsg(this);
		}
		break;
	case ITEMACT_BOOTH_CHGEUDE:
		{
			if(pUser->QueryBooth())
			{
				if(pUser->QueryBooth()->ChgEudemon(m_pInfo->m_itemInfo[0].id, m_pInfo->dwData))
					pUser->SendMsg(this);
			}
		}
		break;
	case ITEMACT_BOOTH_QUERY:
		{
			CBooth* pBooth;
			if(!pUser->FindAroundObj(OBJ_BOOTH, m_pInfo->dwData, IPP_OF(pBooth)))		// npc id
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"̯λ�Ѿ�����,�����̫Զ.");
				return ;
			}

			DEBUG_TRY
			if(pBooth)
				pBooth->SendGoods(pUser);
			DEBUG_CATCH("ITEMACT_BOOTH_QUERY");
			pUser->SendMsg(this);
		}
		break;
	case ITEMACT_BOOTH_ADD: //��Ӱ�̯����Ʒ
		{
			if(pUser->QueryBooth())
			{
				if(pUser->QueryBooth()->AddItem(m_pInfo->m_itemInfo[0].id, m_pInfo->dwData, /*m_pInfo->dwType*/0))
					pUser->SendMsg(this);
				else
					pUser->SendUnlockItemMsg(m_pInfo->m_itemInfo[0].id);
			}
		}
		break;
	case ITEMACT_BOOTH_DEL:
		{
			if(pUser->QueryBooth())
			{
				if(pUser->QueryBooth()->DelItem(m_pInfo->m_itemInfo[0].id))
					pUser->SendMsg(this);
			}
		}
		break;
	case ITEMACT_BOOTH_BUY:
		{
			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
				return;
			}
			CBooth* pBooth;
			if(!pUser->FindAroundObj(OBJ_BOOTH, m_pInfo->dwData, IPP_OF(pBooth)))		// npc id
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"̯λ�Ѿ�����.");
				return ;
			}

			pBooth->BuyItem(pUser, m_pInfo->m_itemInfo[0].id,m_pInfo->m_itemInfo[0].dwMoney, m_pInfo->m_itemInfo[0].nAmount);
		}
		break;
	case ITEMACT_BOOTH_CHGMONEY:
		{
			if(pUser->QueryBooth())
			{
				if(pUser->QueryBooth()->ChgItem(m_pInfo->m_itemInfo[0].id, m_pInfo->dwData))
					pUser->SendMsg(this);
			}
		}
		break;
	case ITEMACT_BOOTH_CLRLEAVEWORD:
		{
			if(pUser->QueryBooth())
			{
				pUser->QueryBooth()->ClrLeaveWord();
				pUser->SendMsg(this);
			}
		}
		break;		
	case ITEMACT_REQUEST_CHGPACKIDX:
		{
			if(m_pInfo->m_itemInfo[0].id != ID_NONE)
				pUser->ProcessChgItemPackIndex( m_pInfo->m_itemInfo[0].id,m_pInfo->dwData);
		}
		break;
// 	case ITEMACT_REQUEST_CHGPOSANDIDX:
// 		{
// 			pUser->ProcessChgItemPosAndIdx( m_pInfo->id,m_pInfo->usPosX,m_pInfo->usPosY,m_pInfo->idTarget );
// 		}
// 		break;
// 	//[2009.3.26--�Ŷ�]���������Ʒ	
// 	case ITEMACT_REQUEST_TRADEPOSANDIDX:
// 		{
// 			pUser->ProcessTradeItemPosAndIdx(m_pInfo->id,m_pInfo->usPosX,m_pInfo->usPosY,m_pInfo->idTarget,m_pInfo->idOldTarget );
// 		}
// 		break;
	case ITEMACT_REQUEST_TRADEPACKIDX:
		{
			if(m_pInfo->m_itemInfo[0].id != ID_NONE && m_pInfo->dwData != ID_NONE)
				pUser->ProcessTradeItemPackageIndex( m_pInfo->m_itemInfo[0].id,m_pInfo->dwData );
		}
		break;
// 	case ITEMACT_SYNTHESIS_REQUEST:
// 		{
// 			CMsgItem msg;
// 			if( pUser->GemCreate(m_pInfo->id,//��ʯ��ʯtype
// 				m_pInfo->idTarget)//�߻���type
// 				)
// 			{//���ɱ�ʯ�ɹ�				
// 				IF_OK(msg.Create(m_pInfo->id,
// 					ITEMACT_SYNTHESIS_SUCCESS))
// 					pUser->SendMsg(&msg);
// 			}
// 			else
// 			{//���ɱ�ʯʧ��
// 				IF_OK(msg.Create(m_pInfo->id,
// 					ITEMACT_SYNTHESIS_FAIL))
// 					pUser->SendMsg(&msg);
// 			}
// 		}
// 		break;
// 	//--------------06.12.26����-----begin---------------------
// 	case ITEMACT_STRENGTHAN://װ��ǿ��
// 		{
// 			//			if( pUser->StrengThanEquipment( m_pInfo->id,m_pInfo->dwData ) )
// 			//			{
// 			//				CMsgItem msg;
// 			//				if( msg.Create( m_pInfo->id,ITEMACT_SYNCHRO_UNLOCK ) )
// 			//					pUser->SendMsg( &msg );
// 			//				
// 			//			}
// 			//			else//����������
// 			//			{
// 			//				CMsgItem msg;
// 			//				if( msg.Create( m_pInfo->id,ITEMACT_SYNCHRO_UNLOCK ) )		//����װ��
// 			//					pUser->SendMsg( &msg );
// 			//				if( msg.Create( m_pInfo->dwData,ITEMACT_SYNCHRO_UNLOCK ) )//����ǿ����ʯ
// 			//					pUser->SendMsg( &msg );
// 			//
// 			//			}
// 		}
// 		break;
//     case ITEMACT_SAVEITEMWSAGET:  //����ֵ�������
// 		{
// 			if(pUser->FindItem(m_pInfo->id))
// 			if(pUser->ProcessSaveItem(m_pInfo->id,m_pInfo->dwData,false,true))
// 			{
// 				CMsgItem msg;
// 				if(msg.Create(m_pInfo->dwData,ITEMACT_SAVEITEMOK))
// 					pUser->SendMsg(&msg);
// 			}
// 		}
// 		break;
// 	case ITEMACT_SAVEITEMMONEYGET:          //�����ҵ�����Ǯ���
// 		{
// 			if(pUser->FindItem(m_pInfo->id))
// 			if(pUser->ProcessSaveItem(m_pInfo->id,m_pInfo->dwData))
// 			{
// 				CMsgItem msg;
// 				if(msg.Create(m_pInfo->dwData,ITEMACT_SAVEITEMOK))
// 					pUser->SendMsg(&msg);
// 			}
// 		}
// 		break;
// 	case ITEMACT_SAVEITEMOK:          //�����ȵ��߳ɹ�
// 		{
// 			
// 		}
// 		break;
// 	case ITEMACT_SAVEITEMGIVEUP:      //�����������
// 		{
// 			if(!pUser->IsSetUseLock()) //û�п���������
// 			if(pUser->FindItem(m_pInfo->id)) //����ҵ��˸ĵ���
// 			{
// 				bool bResult = pUser->GiveUpSaveItem(m_pInfo->id);
// 				if(!bResult)
// 					pUser->SendSysMsg("������ص���ʧ��");
// 			}
// 		}
// 		break;
	}
	}catch(...)
	{
		::LogSave("switch(MSGITEM) Action [%d], idTarget [%u], idType [%u], id [%u], data [%u] ", m_pInfo->usAction,m_pInfo->idTarget,m_pInfo->m_itemInfo[0].typeId, m_pInfo->m_itemInfo[0].id, m_pInfo->dwData);
	}
}

