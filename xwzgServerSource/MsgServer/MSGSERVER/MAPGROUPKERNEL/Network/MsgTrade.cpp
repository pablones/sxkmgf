// MsgTrade.cpp: implementation of the CMsgTrade class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "AllMsg.h"
#include "User.h"
#include "SquareDeal.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTrade::CMsgTrade()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgTrade::~CMsgTrade()
{

}

//-----------------------------------------------------
BOOL CMsgTrade::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_TRADE != this->GetType())
		return false;

	return true;
}

//-----------------------------------------------------
BOOL CMsgTrade::Create(unsigned short usAction, OBJID id, int nType)
{
	// reset
	this->Init();
	
	// fill
	m_unMsgType		= _MSG_TRADE;
	m_unMsgSize		= sizeof(MSG_Info);
	
	m_pInfo->usAction	= usAction;
	m_pInfo->id			= id;
	m_pInfo->type       = nType;
	return true;
}

//[2010-05-31 goto] 返回交易物品信息
BOOL CMsgTrade::Create(CItemPtr pItem, unsigned short nAction/*=_TRADE_ADDITEM*/)
{
	CHECKF(pItem);
	ItemInfoStruct info;
	memset(&info,0,sizeof(ItemInfoStruct));
	CHECKF(pItem->GetInfo(&info));
	
	// init
	this->Init();
	
	// fill info now
	m_unMsgType		= _MSG_TRADE;
	m_unMsgSize		= sizeof(MSG_Info);
	
	m_pInfo->id			= info.id;			//物品ID号
	m_pInfo->nData		= info.idType;		//物品种类
	m_pInfo->idType		= info.nAmount;		//物品数量
	m_pInfo->type		= info.nAmountLimit;//数量上限
	m_pInfo->usAction	= nAction;

	return true;
}
//-----------------------------------------------------
void CMsgTrade::Process(CGameSocket* pSocket)
{
#ifdef _MYDEBUG
	::LogSave("Process CMsgTrade, id:%u, action:%u", 
		m_pInfo->id,
		m_pInfo->usAction);
#endif
//	DEBUG_PROCESSMSG("TRADE",m_pInfo->id,m_pInfo->usAction,"",m_pInfo->nData,0);

	CUserPtr pUser	= UserManager()->GetUser(this);
	if (!pUser)
		return;

/*	if(pUser->GetLockKey())
	{
		pUser->SendSysMsg("请先打开保护锁。");
		return;
	}

	if (pUser->IsFighting())
	{
		pUser->SendSysMsg("错误：战斗中不能执行交易指令！");
		return;
	}

	if (pUser->GetTeam())
	{
		pUser->SendSysMsg("错误：组队状态不能执行交易指令！");
		return;
	}
*/
	DEBUG_TRY		// VVVVVVVVVVVVVVVVVVVVVVVVVVV
	switch(m_pInfo->usAction)
	{
	case _TRADE_APPLY:
		{
			if (!pUser->IsAlive())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"死亡状态不能进行交易");
				return;
			}
			if (m_pInfo->id == ID_NONE)
				return;

			OBJID	idTarget = m_pInfo->id;

			if(pUser->GetID()==m_pInfo->id)//[2010-06-03 goto] 自己和自己交易,直接忽略
				return;

			CUser* pTarget = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pTarget))
				|| abs(pUser->GetPosX()- pTarget->GetPosX()) > 8     //X距离
				|| abs(pUser->GetPosY()- pTarget->GetPosY()) > 10)    //Y距离
			{
				//pUser->SendSysMsg(g_objStrRes.GetStr(13046));//STR_NO_TRADE_TARGET
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"距离太远，无法进行交易");
				return;
			}
			if (!pTarget->IsAlive())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"对方已死亡，无法进行交易");
				return;
			}
			//[游途道标 2009.04.15]摊位判断
			if (pUser->QueryBooth())
			{
// 				const int _maxSize=120;
// 				int sourceItemAmount = 0;
// 				int sourceItemSize=_maxSize;
// 				OBJID sourceItemID[_maxSize] = {0};
// 				sourceItemAmount = pUser->GetItemByTypeMulti(880100, sourceItemSize, sourceItemID);
// 				
// 				if (!sourceItemAmount)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "摆摊中无法进行交易");
					return;
				}
			}
			if (pTarget->QueryBooth())
			{
// 				const int _maxSize=120;
// 				int sourceItemAmount = 0;
// 				int sourceItemSize=_maxSize;
// 				OBJID sourceItemID[_maxSize] = {0};
// 				sourceItemAmount = pTarget->GetItemByTypeMulti(880100, sourceItemSize, sourceItemID);
// 				
// 				if (!sourceItemAmount)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方正在摆摊无法进行交易");
					return;
				}
			}
			//人物锁的判断
// 			if(pTarget&&pTarget->IsSetUseLock())
// 			{
// 				pUser->SendSysMsg("对方开启了人物锁，无法交易.");
// 				return;
// 			}
			if (!pTarget->IsAgreeTradeApply())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已打开拒绝交易开关");
				return;
			}
			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return;
			}
			ISquareDeal* pSquareDeal = pUser->QuerySquareDeal();
			if(pSquareDeal)
			{
				// 已经做了处理，不需要在写日志拉 [7/3/2007]
			//	LOGWARNING("玩家重复交易!");
				pSquareDeal->Release();
				return ;
			}
// 			if(!pTarget->GetIsAllowBuy())
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(13044));//STR_NOTALLOW_BUY
// 				return;
// 			}
			if(pTarget->QuerySquareDeal())
			{
				CMsgTrade msg;//对方正在交易中
				msg.Create(_TRADE_TARGETTRADEING,pTarget->GetID());
				pUser->SendMsg(&msg);
				return ;
			}
			if (pUser->FetchApply(CUser::APPLY_TRADE, idTarget))//防止重复申请
			{
				pUser->SetApply(CUser::APPLY_TRADE, idTarget);
				return;
			}
			if (!pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID()))
			{
				pUser->SetApply(CUser::APPLY_TRADE, idTarget);
				IF_NOT(this->Create(_TRADE_APPLY, pUser->GetID()))
					return;
				pTarget->SendMsg(this);
				return;
			}
		}
		break;
	case _TEADE_ACCEPT_APPLY:
		{
			int nStep = 0;
			DEBUG_TRY
			if (!pUser->IsAlive())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"死亡状态不能进行交易");
				return;
			}
			if (m_pInfo->id == ID_NONE)
				return;

			if(pUser->GetID()==m_pInfo->id)
				return;
			nStep = 1;
			OBJID idTarget = m_pInfo->id;
			CUser* pTarget = NULL;
			if (!pUser->GetMap())
				return;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pTarget))
				|| abs(pUser->GetPosX()- pTarget->GetPosX()) > 8     //X距离
				|| abs(pUser->GetPosY()- pTarget->GetPosY()) > 10)    //Y距离
			{
				if (!pTarget)
					return;
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"距离太远，无法进行交易");

				pUser->FetchApply(CUser::APPLY_TRADE, pTarget->GetID());
				pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID());
				return;
			}
			nStep = 2;
			if (!pTarget->IsAlive())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"对方已死亡，无法进行交易");

				pUser->FetchApply(CUser::APPLY_TRADE, pTarget->GetID());
				pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID());
				return;
			}
			nStep = 3;
			if (pUser->QueryBooth())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "摆摊中无法进行交易");

				pUser->FetchApply(CUser::APPLY_TRADE, pTarget->GetID());
				pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID());
				return;
			}
			nStep = 4;
			if (pTarget->QueryBooth())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方正在摆摊无法进行交易");

				pUser->FetchApply(CUser::APPLY_TRADE, pTarget->GetID());
				pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID());
				return;
			}
			nStep = 5;
			ISquareDeal* pSquareDeal = pUser->QuerySquareDeal();
			if(pSquareDeal)
			{
				pSquareDeal->Release();
				return ;
			}
			nStep = 6;
			if(pTarget->QuerySquareDeal())
			{
				//pUser->SendSysMsg(g_objStrRes.GetStr(13047));//STR_TARGET_TRADING
				CMsgTrade msg;//对方正在交易中
				msg.Create(_TRADE_TARGETTRADEING,pTarget->GetID());
				pUser->SendMsg(&msg);

				pUser->FetchApply(CUser::APPLY_TRADE, pTarget->GetID());
				pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID());
				return ;
			}
			nStep = 7;
			if (!pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID()))
				return;
			nStep = 8;
			if (!pTarget->IsAgreeTradeApply())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已打开拒绝交易开关");
				return;
			}
			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return;
			}
			if(!pUser->CreateSquareDeal(pTarget))		// 同时发送消息
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "创建交易失败");
				pUser->FetchApply(CUser::APPLY_TRADE, pTarget->GetID());
				pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID());
				return;
			}
			DEBUG_CATCH2("CMsgTrade::_TEADE_ACCEPT_APPLY step=%d", nStep);
		}
		break;
	case _TRADE_QUIT:
		{
			ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
			if(pSquareDeal)
			{
				// inform target
				CUserPtr pTarget	= pSquareDeal->GetTarget();
				if(pTarget)
				{
					if (pUser->FetchApply(CUser::APPLY_TRADE, pTarget->GetID()))
					{
						pUser->SetApply(CUser::APPLY_NONE, pTarget->GetID());
					}
					if (pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID()))
					{
						pTarget->SetApply(CUser::APPLY_NONE, pUser->GetID());
					}
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"你取消了交易");
					pTarget->SendSysMsg(_TXTATR_TOPSHOW,"对方取消了交易");
				}
				pSquareDeal->CancelTrade();
				// feed back
//				{
//					CHECK(this->Create(_TRADE_FALSE, 0));
//					pUser->SendMsg(this);
//				}

				pSquareDeal->Release();
			}
		}
		break;

	case _TRADE_OPEN:
		{
			ASSERT(!"_TRADE_OPEN");
		}
		break;

	case _TRADE_SUCCESS:
		{
			ASSERT(!"_TRADE_SUCCESS");
		}
		break;

	case _TRADE_FALSE:
		{
			ASSERT(!"_TRADE_FALSE");
		}
		break;

	case _TRADE_ADDITEM:
		{
			ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
			if(!pSquareDeal)
				return;

			CHECK(m_pInfo->id);

			CItemPtr pItem	= pUser->GetItem(m_pInfo->id,false);
			if (!pItem || !pItem->IsExchangeEnable() || pItem->GetKidnap())
			{
				//pUser->SendSysMsg(g_objStrRes.GetStr(13049));//STR_NOT_FOR_TRADE
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "此物品不能交易");//STR_NOT_FOR_TRADE

				IF_OK(this->Create(_TRADE_ADDITEMFAIL, m_pInfo->id))
					pUser->SendMsg(this);
				//发送物品解锁消息
				CMsgItem msg;
				IF_OK( msg.Create( ITEMACT_SYNCHRO_UNLOCK, 0, 0, 0, 0, m_pInfo->id ) )
					SendMsg( &msg );
				return;
			}
		
			if(!pSquareDeal->AddItem(pItem))
			{
				IF_OK(this->Create(_TRADE_ADDITEMFAIL, m_pInfo->id))
					pUser->SendMsg(this);
				//发送物品解锁消息
				CMsgItem msg;
				IF_OK( msg.Create( ITEMACT_SYNCHRO_UNLOCK, 0, 0, 0, 0, m_pInfo->id ) )
					SendMsg( &msg );
				return;
			}
			CHECK(this->Create(pItem));
			pUser->SendMsg(this);
			pItem->setLocked(true);//将交易中的物品锁定
		}
		break;

	case _TRADE_BACK_WU:
		{
			ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
			if(!pSquareDeal)
				return;

			CHECK(m_pInfo->id);

			CItemPtr pItem	= pUser->GetItem(m_pInfo->id,false);
			if(NULL == pItem)
				return ;

			if(!pSquareDeal->DelItem(pItem))
			{
				IF_OK(this->Create(_TRADE_DELITEMFAIL, m_pInfo->id))
					pUser->SendMsg(this);
				return;
			}
			CHECK(this->Create(pItem, _TRADE_BACK_WU));
			pUser->SendMsg(this);
			pItem->setLocked(false);
		}
		break;
	case _TRADE_ADDMONSTER://增加宠物
		{
			ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
			if(!pSquareDeal)
				return;

			CUserPet* pUserPet = pUser->QueryUserPetSet();
			if (!pUserPet)
				return;

			CUserPetData* pPetData = pUserPet->QueryData(m_pInfo->id);
			if (!pPetData)
				return;
			
			if (pPetData->GetInt(EUDEMONDATA_state)!=PETSTATE_REST)
				return;

			if(!pSquareDeal->AddMonster(pPetData))
			{
				IF_OK(this->Create(_TRADE_ADDMONSTERFAIL, m_pInfo->id))
					pUser->SendMsg(this);
				return;
			}
			CHECK(this->Create(_TRADE_ADDMONSTERSUC, m_pInfo->id));
			pUser->SendMsg(this);
			pPetData->SetLock(true);//将交易中的物品锁定
		}
		break;
	case _TRADE_DELMONSTER://减少宠物
		{
			ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
			if(!pSquareDeal)
				return;
			
			CUserPet* pUserPet = pUser->QueryUserPetSet();
			if (!pUserPet)
				return;
			
			CUserPetData* pPetData = pUserPet->QueryData(m_pInfo->id);
			if (!pPetData)
				return;
			
			if(!pSquareDeal->DelMonster(pPetData))
			{
				IF_OK(this->Create(_TRADE_DELMONSTERFAIL, m_pInfo->id))
					pUser->SendMsg(this);
				return;
			}
			CHECK(this->Create(_TRADE_DELMONSTERSUC, m_pInfo->id));
			pUser->SendMsg(this);
			pPetData->SetLock(false);
		}
		break;
	case _TRADE_LOCK:
		{
			
			ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
			if(!pSquareDeal)
				return;

			pSquareDeal->TradeLock();
		}
		break;

	case _TRADE_ADDMONEY:
		{
			ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
			if(!pSquareDeal)
				return;
			if (m_pInfo->id > pUser->GetMoneyByType())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足");
				return;
			}
			//[游途道標 2009.03.11]錢居然放到了標識里面.
			//int	nAllMoney = pSquareDeal->AddMoney(m_pInfo->nData);		// return allmoney
			int	nAllMoney = pSquareDeal->AddMoney(m_pInfo->id);		// return allmoney
			if (nAllMoney==-1)
				return;
			CHECK(this->Create(_TRADE_SELFMONEYALL, nAllMoney));
			pUser->SendMsg(this);

			if(nAllMoney <= 0)
				pUser->AddAttrib(_USERATTRIB_MONEY, 0, SYNCHRO_TRUE);		// recruit money(note: because set money once, so no conflict
		}
		break;

	case _TRADE_ADDVAS:
		{
			ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
			if(!pSquareDeal)
				return;

			int	nAllMoney = pSquareDeal->AddVAS(m_pInfo->id);		// return allmoney
			CHECK(this->Create(_TRADE_VASSELF, nAllMoney));
			pUser->SendMsg(this);

		}
		break;

	case _TRADE_OK:
		{
			ISquareDeal* pSquareDeal =	pUser->QuerySquareDeal();
			if(!pSquareDeal)
				return;

			if(pSquareDeal->ClickOK())		// return true: 交易完成
				pSquareDeal->Release();
		}
		break;
	case _TRADE_NOTALLOW:
		{
			pUser->SetIsAllowBuy(m_pInfo->id);
		}
		break;
	case _TRADE_REFUSE://[2010-06-01 goto] 拒绝交易
		{
			CUserPtr pTarget = UserManager()->GetUser(m_pInfo->id);//被拒绝的user
			if(!pTarget)
				return;

			CMsgTrade msg;
			msg.Create(_TRADE_REFUSE, pUser->GetID());

			pTarget->SendMsg(&msg);

			if (pUser->FetchApply(CUser::APPLY_TRADE, pTarget->GetID()))
			{
				pUser->SetApply(CUser::APPLY_NONE, pTarget->GetID());
			}
			if (pTarget->FetchApply(CUser::APPLY_TRADE, pUser->GetID()))
			{
				pTarget->SetApply(CUser::APPLY_NONE, pUser->GetID());
			}
		}
		break;
	default:
		break;
	}
	DEBUG_CATCH2("switch(MSGTRADE) [%d]", m_pInfo->usAction)		// AAAAAAAAAAAAAAAA
}
