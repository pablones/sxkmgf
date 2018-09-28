// SquareDeal.cpp: implementation of the CSquareDeal class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "windows.h"
#include "SquareDeal.h"
#include "Item.h"
#include "User.h"
#include <algorithm>
#include "MapGroup.h"
#include "networkdef.h"
using namespace std;

MYHEAP_IMPLEMENTATION(CSquareDeal,s_heap)
//////////////////////////////////////////////////////////////////////
// constant
const int	MAX_SQUAREDEALITEMS		= 5;//20;[2010-06-04 goto] 最多只允许交易5个物品
const DWORD	MAX_SQUAREDEALMONEY		= 900000000;	// 100 m


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSquareDeal::CSquareDeal()
{
	m_iface1.m_pParent	= this;
	m_iface2.m_pParent	= this;

	m_pUser1			= NULL;
	m_pUser2			= NULL;
//	m_setItem1;
//	m_setItem2;
	m_nMoney1			= 0;
	m_nMoney2			= 0;
	m_nVas1		    	= 0;
	m_nVas2     		= 0;
	m_bClickOK1			= false;
	m_bClickOK2			= false;
	m_bLocked1			= false;//[2010-06-16 goto]判断交易时是否已经点击了锁定
	m_bLocked2			= false;//[2010-06-16 goto]判断交易时是否已经点击了锁定
	m_bTradeSuccess		= false;
}

//////////////////////////////////////////////////////////////////////
CSquareDeal::~CSquareDeal()
{
	if(!m_bTradeSuccess)
	{
		CUser*	pUser1	= m_pUser1->QueryUser();
		CUser*	pUser2	= m_pUser2->QueryUser();
		IF_OK(pUser1 && pUser2)
		{
			CMsgTrade	msg;
			IF_OK(msg.Create(_TRADE_FALSE, 0))
			{
				pUser1->SendMsg(&msg);
				pUser2->SendMsg(&msg);
			}
//			pUser1->SendSysMsg(g_objStrRes.GetStr( 11093 ));//STR_TRADE_FAIL
//			pUser2->SendSysMsg(g_objStrRes.GetStr( 11093 ));//STR_TRADE_FAIL
		}
	}

	if(m_pUser1)
		m_pUser1->Release();
	if(m_pUser2)
		m_pUser2->Release();
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::TradeOK()
{
	CHECKF(m_pUser1 && m_pUser2);

	STPROPERTYSTREAMLOG stLog;

	CUser*	pUser1	= m_pUser1->QueryUser();
	CUser*	pUser2	= m_pUser2->QueryUser();
	IF_NOT(pUser1 && pUser2)
		return false;
	IF_NOT(pUser1->GetMapID() == pUser2->GetMapID())
		return false;
	IF_NOT(pUser1->GetDistance(pUser2->GetPosX(), pUser2->GetPosY()) <= CELLS_PER_VIEW)
		return false;

	CUserPet* pUserPet1 = NULL;
	CUserPet* pUserPet2 = NULL;

	pUserPet1 = pUser1->QueryUserPetSet();//pUser1的宠物集合
	pUserPet2 = pUser2->QueryUserPetSet();//pUser2的宠物集合
	if (!pUserPet1 || !pUserPet2)
		return false;

	stLog.idSAccount = pUser1->GetAccountID();
	stLog.idSource = pUser1->GetID();
	stLog.idTAccount = pUser2->GetAccountID();
	stLog.idTarget = pUser2->GetID();

	// check money
	bool	bRet = true;
	if(	(m_nMoney1 && (m_nMoney1 > pUser1->GetMoneyByType() || m_nMoney1 + pUser2->GetMoneyByType() > _MAX_MONEYLIMIT))
			|| (m_nMoney2 && (m_nMoney2 > pUser2->GetMoneyByType() || m_nMoney2 + pUser1->GetMoneyByType() > _MAX_MONEYLIMIT)) )
		bRet = false;

	// check items
//06.9.22王玉波修改----------------------------begin---------------------------------------------------
	int	nAmountCount1_47	= 0;//pUser1的背包第一页
	int	nAmountCount1_48	= 0;
	int	nAmountCount1_49	= 0;
	int nAmountCount2_47	= 0;
	int nAmountCount2_48	= 0;
	int nAmountCount2_49	= 0;
	//统计需要的格子数和重量,重量暂时忽略
	for(int i = 0; i < m_setItem1.size(); i++)	//pUser1拿出去的物品集合
	{
		if(m_setItem1[i] == ID_NONE)
			continue;

		CItemPtr pItem = pUser1->GetItem(m_setItem1[i],false);
		if(!pItem)
			return false;
		//////////////////////////////////////////////////////////////////////////
		//	20070712	朱斌	唯一物品判断
		//	ItemInfoStruct	info;
		//	ItemType()->GetInfo(pItem->GetTypeID(), &info);
		if(CUser::IsUserHaveAnExclusiveItem(pItem->GetTypeID(), NULL, pUser2))
			return false;
		//	结束 20070712 朱斌
		//////////////////////////////////////////////////////////////////////////
		int nPosition = pUser1->QueryPackage()->GetPositionByItemType(pItem->GetTypeID());
		if (nPosition==ITEMPOSITION_BACKPACK1)		nAmountCount1_47++;
		else if (nPosition==ITEMPOSITION_BACKPACK2)	nAmountCount1_48++;
		else if (nPosition==ITEMPOSITION_BACKPACK3)	nAmountCount1_49++;
	}
	for( i = 0; i < m_setItem2.size(); i++) //pUser2拿出去的物品ID集合
	{
		if(m_setItem2[i] == ID_NONE)
			continue;

		CItemPtr pItem = pUser2->GetItem(m_setItem2[i],false);
		if(!pItem)
			return false;

		//////////////////////////////////////////////////////////////////////////
		//	20070712	朱斌	唯一物品判断
		//	ItemInfoStruct	info;
		//	ItemType()->GetInfo(pItem->GetTypeID(), &info);
		if(CUser::IsUserHaveAnExclusiveItem(pItem->GetTypeID(), NULL, pUser1))
			return false;
		//	结束 20070712 朱斌
		//////////////////////////////////////////////////////////////////////////

		int nPosition = pUser2->QueryPackage()->GetPositionByItemType(pItem->GetTypeID());
		if (nPosition==ITEMPOSITION_BACKPACK1)		nAmountCount2_47++;
		else if (nPosition==ITEMPOSITION_BACKPACK2)	nAmountCount2_48++;
		else if (nPosition==ITEMPOSITION_BACKPACK3)	nAmountCount2_49++;
	}

	IUserPackage* pUserPackage = pUser1->QueryPackage();//user1的背包
	if (!pUserPackage)
		return false;
	//检查是否有空间
	if( (nAmountCount2_47 - nAmountCount1_47>0 && pUserPackage->IsPackFull( nAmountCount2_47 - nAmountCount1_47, 0, ITEMPOSITION_BACKPACK1))
		|| (nAmountCount2_48 - nAmountCount1_48>0 && pUserPackage->IsPackFull( nAmountCount2_48 - nAmountCount1_48, 0, ITEMPOSITION_BACKPACK2))
		|| (nAmountCount2_49 - nAmountCount1_49>0 && pUserPackage->IsPackFull( nAmountCount2_49 - nAmountCount1_49, 0, ITEMPOSITION_BACKPACK3)))
	{
		pUser1->SendSysMsg(_TXTATR_TOPSHOW,"交易失败，你没有足够的包裹空间");
		pUser2->SendSysMsg(_TXTATR_TOPSHOW,"交易失败，对方没有足够的包裹空间");
		if (pUser1->FetchApply(CUser::APPLY_TRADE, pUser2->GetID()))
		{
			pUser1->SetApply(CUser::APPLY_NONE, pUser2->GetID());
		}
		if (pUser2->FetchApply(CUser::APPLY_TRADE, pUser1->GetID()))
		{
			pUser2->SetApply(CUser::APPLY_NONE, pUser1->GetID());
		}
		return false;
	}
	pUserPackage = pUser2->QueryPackage();	//user2的背包
	if (!pUserPackage)
		return false;
	if( (nAmountCount1_47 - nAmountCount2_47>0 && pUserPackage->IsPackFull( nAmountCount1_47 - nAmountCount2_47, 0, ITEMPOSITION_BACKPACK1 ))
		|| (nAmountCount1_48 - nAmountCount2_48>0 && pUserPackage->IsPackFull( nAmountCount1_48 - nAmountCount2_48, 0, ITEMPOSITION_BACKPACK2 ))
		|| (nAmountCount1_49 - nAmountCount2_49>0 && pUserPackage->IsPackFull( nAmountCount1_49 - nAmountCount2_49, 0, ITEMPOSITION_BACKPACK3 )))
	{
		pUser2->SendSysMsg(_TXTATR_TOPSHOW,"交易失败，你没有足够的包裹空间");
		pUser1->SendSysMsg(_TXTATR_TOPSHOW,"交易失败，对方没有足够的包裹空间");
		if (pUser1->FetchApply(CUser::APPLY_TRADE, pUser2->GetID()))
		{
			pUser1->SetApply(CUser::APPLY_NONE, pUser2->GetID());
		}
		if (pUser2->FetchApply(CUser::APPLY_TRADE, pUser1->GetID()))
		{
			pUser2->SetApply(CUser::APPLY_NONE, pUser1->GetID());
		}
		return false;
	}

	if (m_setPet2.size()>0)
	{
		if ( (3+pUser1->GetPacklev(4)-pUserPet1->GetShowAmount())+m_setPet1.size() < m_setPet2.size() )
		{
			pUser1->SendSysMsg(_TXTATR_TOPSHOW,"交易失败，你没有足够的宠物栏空间");
			pUser2->SendSysMsg(_TXTATR_TOPSHOW,"交易失败，对方没有足够的宠物栏空间");
			if (pUser1->FetchApply(CUser::APPLY_TRADE, pUser2->GetID()))
			{
				pUser1->SetApply(CUser::APPLY_NONE, pUser2->GetID());
			}
			if (pUser2->FetchApply(CUser::APPLY_TRADE, pUser1->GetID()))
			{
				pUser2->SetApply(CUser::APPLY_NONE, pUser1->GetID());
			}
			return false;
		}
	}
	if (m_setPet1.size()>0)
	{
		if ( (3+pUser2->GetPacklev(4)-pUserPet2->GetShowAmount())+m_setPet2.size() < m_setPet1.size())
		{
			pUser2->SendSysMsg(_TXTATR_TOPSHOW,"交易失败，你没有足够的宠物栏空间");
			pUser1->SendSysMsg(_TXTATR_TOPSHOW,"交易失败，对方没有足够的宠物栏空间");
			if (pUser1->FetchApply(CUser::APPLY_TRADE, pUser2->GetID()))
			{
				pUser1->SetApply(CUser::APPLY_NONE, pUser2->GetID());
			}
			if (pUser2->FetchApply(CUser::APPLY_TRADE, pUser1->GetID()))
			{
				pUser2->SetApply(CUser::APPLY_NONE, pUser1->GetID());
			}
			return false;
		}
	}
	
//----------------------end--------------------------------------------------------------------------------
	if(bRet)
	{
		DEBUG_TRY		// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
		// exchange, 客户端自动同步
		if(m_nMoney1)
		{
			BOOL bRet = FALSE;
			ASSERT(bRet = pUser1->SpendMoneyByType(m_nMoney1, 1, SYNCHRO_TRUE));
			ASSERT(bRet = pUser2->GainMoney(m_nMoney1, SYNCHRO_TRUE, 1));
			if (m_nMoney1>30000)
				::MyLogSave(VALUABLESLOG_FILENAME, "CSquareDeal::TradeOK() user:%u(%s)从user:%u(%s)获得银两%d",pUser2->GetID(),pUser2->GetName(), pUser1->GetID(),pUser1->GetName(), m_nMoney1);
			//////////////////////////////////////////////////////////////////////////
			stLog.dwMoney = m_nMoney1;
			stLog.dwType = TRADE_ONE;
			stLog.dwSMoney = pUser1->GetMoneyByType();
			stLog.dwTMoney = pUser2->GetMoneyByType();
			::PropertyStreamLog(stLog);
		}
		if(m_nMoney2)
		{
			BOOL bRet = FALSE;
			ASSERT(bRet = pUser2->SpendMoneyByType(m_nMoney2, 1, SYNCHRO_TRUE));
			ASSERT(bRet = pUser1->GainMoney(m_nMoney2, SYNCHRO_TRUE, 1));
			if (m_nMoney2>30000)
				::MyLogSave(VALUABLESLOG_FILENAME, "CSquareDeal::TradeOK() user:%u(%s)从user:%u(%s)获得银两%d",pUser1->GetID(),pUser1->GetName(), pUser2->GetID(),pUser2->GetName(), m_nMoney2);
			//////////////////////////////////////////////////////////////////////////
			stLog.dwMoney = m_nMoney2;
			stLog.dwType = TRADE_TWO;
			stLog.dwSMoney = pUser1->GetMoneyByType();
			stLog.dwTMoney = pUser2->GetMoneyByType();
			::PropertyStreamLog(stLog);
		}

		stLog.dwMoney = NULL;
		stLog.dwType = NULL;
		stLog.dwSMoney = NULL;
		stLog.dwTMoney = NULL;

//         if(m_nVas1)
// 		{
// 			
// 			CMsgVAS MsgVAS;
// 			CMsgVAS::MsgTraffic stTraffic;
// 			stTraffic.dwData = 0;
// 			stTraffic.id = pUser1->GetID();
// 			stTraffic.itemID = 0;
// 			stTraffic.usAction = 0;
// 			stTraffic.itemAmount = pUser1->GetAccountID();
// 			MsgVAS.Create(&stTraffic, m_nVas1, pUser2->GetAccountID(), pUser2->GetID(), _MSGVAS_CHG_TRADE);
// 			MapGroup(pUser1->GetProcessID())->QueryIntraMsg()->TransmitWorldMsg(&MsgVAS);
// 			
// 			//金币交易，写记录
// 			/*MyLogSaveCostlyItem(NULL, pUser1->GetAccountID(), pUser1->GetID(),
// 				pUser1->GetName(), pUser2->GetAccountID(), pUser2->GetID(), 
// 				pUser2->GetName(), COSTLY_ITEM_OPER_TYPE_BOOTH_BUY, 
// 				0, 0, "时空币", m_nVas1);*/
// 
// 			stLog.dwVas = m_nVas1;
// 			stLog.dwType = TRADE_ONE;
// 			stLog.dwSVas = pUser1->GetVas();
// 			stLog.dwTVas = pUser2->GetVas();
// 			::PropertyStreamLog(stLog);
// 			pUser1->SetVas(pUser1->GetVas() - m_nVas1);
// 			pUser2->SetVas(pUser2->GetVas() + m_nVas1);
// 			
// 		}
		
// 		if(m_nVas2)
// 		{
// 			
// 			CMsgVAS MsgVAS;
// 			CMsgVAS::MsgTraffic stTraffic;
// 			stTraffic.dwData = 0;
// 			stTraffic.id = pUser2->GetID();
// 			stTraffic.itemID = 0;
// 			stTraffic.usAction = 0;
// 			stTraffic.itemAmount = pUser2->GetAccountID();
// 			MsgVAS.Create(&stTraffic, m_nVas2, pUser1->GetAccountID(), pUser1->GetID(), _MSGVAS_CHG_TRADE);
// 			MapGroup(pUser1->GetProcessID())->QueryIntraMsg()->TransmitWorldMsg(&MsgVAS);
// 			
// 			//金币交易，写记录
// 			/*MyLogSaveCostlyItem(NULL, pUser2->GetAccountID(), pUser2->GetID(),
// 				pUser2->GetName(), pUser1->GetAccountID(), pUser1->GetID(), 
// 				pUser1->GetName(), COSTLY_ITEM_OPER_TYPE_BOOTH_BUY, 
// 				0, 0, "时空币", m_nVas2);*/
// 			stLog.dwVas = m_nVas2;
// 			stLog.dwType = TRADE_TWO;
// 			stLog.dwSVas = pUser1->GetVas();
// 			stLog.dwTVas = pUser2->GetVas();
// 			::PropertyStreamLog(stLog);
// 			
// 		}

		stLog.dwVas = NULL;
		stLog.dwType = NULL;
		stLog.dwSVas = NULL;
		stLog.dwTVas = NULL;

		//06.9.22王玉波添加--------begin----------------------------------------
		vector< CItem* >	giveUser1Set;
		vector< CItem* >	giveUser2Set;
		giveUser1Set.clear();
		giveUser2Set.clear();
		//-------------------------end------------------------------------------
		for(int i = 0; i < m_setItem1.size(); i++)
		{
			if(m_setItem1[i] == ID_NONE)
				continue;

			CItemPtr pItem = pUser1->GetItem(m_setItem1[i],false);
			ASSERT(pItem);
			if(pItem)
			{
				CItemPtr pItemTemp = NULL;
				//此处注意，王玉波暂时修改以下一句
				bool bSynchro = true;
				ASSERT(pItemTemp = pUser1->PopItem(pItem->GetID(), SYNCHRO_TRUE));
				pItem->SetPosition( 0 );
				pItem->SetPackageIndex( 0 );
				giveUser2Set.push_back( pItem );

				stLog.idItemType = pItem->GetTypeID();
				stLog.idTItem = stLog.idSItem = pItem->GetID();
				if (CItem::IsCountable(stLog.idItemType) || stLog.idItemType==_SPCITEM_VASEXCHANGE)
				{
					stLog.dwSNumber = stLog.dwTNumber = pItem->GetItemAmount();
				}
				else
				{
					stLog.dwSNumber = 1;
				}
				
				stLog.dwType = TRADE_ONE;
				::PropertyStreamLog(stLog);
				if (pItem->IsCostlyItem())//goto 贵重物品记录
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) transfer %d %u to %u(%s)", pUser1->GetID(), pUser1->GetName(), stLog.dwSNumber, stLog.idItemType, pUser2->GetID(), pUser2->GetName());
			}
		}
		for( i = 0; i < m_setItem2.size(); i++)
		{
			if(m_setItem2[i] == ID_NONE)
				continue;

			CItemPtr pItem = pUser2->GetItem(m_setItem2[i],false);
			ASSERT(pItem);
			if(pItem)
			{
				CItemPtr pItemTemp = NULL;
				bool bSynchro = true;
				ASSERT(pItemTemp = pUser2->PopItem(pItem->GetID(), SYNCHRO_TRUE));
				pItem->SetPosition( 0,false );		//06.9.22王玉波添加
				pItem->SetPackageIndex( 0,false );	//06.9.22王玉波添加
				giveUser1Set.push_back( pItem ); 

				stLog.idItemType = pItem->GetTypeID();
				stLog.idTItem = stLog.idSItem = pItem->GetID();
				if (CItem::IsCountable(stLog.idItemType) || stLog.idItemType==_SPCITEM_VASEXCHANGE)
				{
					stLog.dwSNumber = stLog.dwTNumber = pItem->GetItemAmount();
				}
				else
				{
					stLog.dwSNumber = 1;
				}
				
				stLog.dwType = TRADE_TWO;
				::PropertyStreamLog(stLog);
				if (pItem->IsCostlyItem())//goto 贵重物品记录
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) transfer %d %u to %u(%s)", pUser2->GetID(), pUser2->GetName(), stLog.dwSNumber, stLog.idItemType, pUser1->GetID(), pUser1->GetName());
			}
		}
		//先删除各自背包中确认交易的物品,如果按以前的设计,在双方背包都满了,交易同样数量的物品会出问题
		//----------------------06.9.22王玉波添加--------begin---------------------------------
		for( i = 0;i < giveUser2Set.size();i++ )
		{
			BOOL bRet = FALSE;
			CItem* pItem = giveUser2Set[ i ];
			ASSERT(bRet = pUser2->AddItem( pItem,true,true ) ); 
			pItem->setLocked(false);
			pItem->SetEquipAddLevFailRate(0);//[2010-07-20 goto]交易后装备强化的失败加成清零
			pItem->SetEquipStarFailRate(0);//[2010-07-20 goto]交易后装备升星的失败加成清零
		}
		for( i = 0;i < giveUser1Set.size();i++ )
		{
			BOOL bRet = FALSE;
			CItem* pItem = giveUser1Set[ i ];
			ASSERT(bRet = pUser1->AddItem( pItem,true,true ) );
			pItem->setLocked(false);
			pItem->SetEquipAddLevFailRate(0);//[2010-07-20 goto]交易后装备强化的失败加成清零
			pItem->SetEquipStarFailRate(0);//[2010-07-20 goto]交易后装备升星的失败加成清零
			//////////////////////////////////////////////////////////////////////////
			//	20070709	朱斌	新增贵重物品流向日志	交易 失去物品
			/*if(bRet && pItem->IsCostlyItem())
			{
				ItemInfoStruct info;
				ItemType()->GetInfo(pItem->GetTypeID(), &info);
				MyLogSaveCostlyItem(NULL, pUser1->GetAccountID(), pUser1->GetID(),
					pUser1->GetName(), pUser2->GetAccountID(), pUser2->GetID(),
					pUser2->GetName(), COSTLY_ITEM_OPER_TYPE_TRADE_GET, 
					pItem->GetTypeID(), pItem->GetID(), info.szName,   
					CItem::IsCountable(pItem->GetTypeID()) ? info.nAmount : 1);  
			}*/
			//	结束 20070709
			//////////////////////////////////////////////////////////////////////////
		}
		//------------------------------------------------end--------------------------------------
//		pUser1->SendSysMsg(g_objStrRes.GetStr( 11092 ));//STR_TRADE_SUCCEED
//		pUser2->SendSysMsg(g_objStrRes.GetStr( 11092 ));//STR_TRADE_SUCCEED

		//[2010-07-30 goto]宠物交易--------begin----------------------------------------
		vector< CUserPetData* >	giveUser1PetSet;
		vector< CUserPetData* >	giveUser2PetSet;
		giveUser1PetSet.clear();
		giveUser2PetSet.clear();

		for(i = 0; i < m_setPet1.size(); i++)
		{
			CUserPetData* pEudemon = pUserPet1->QueryData(m_setPet1[i]);
			ASSERT(pEudemon);
			if(pEudemon)
			{
				CUserPetData* pEudemonTemp = NULL;
				bool bSynchro = true;
				ASSERT(pEudemonTemp = pUser1->PopEudemon(pEudemon->GetID(), SYNCHRO_TRUE, UPDATE_FALSE));
				giveUser2PetSet.push_back( pEudemon );

				stLog.idItemType = pEudemon->GetInt(EUDEMONDATA_typeid);
				stLog.idTItem = stLog.idSItem = pEudemon->GetID();
				stLog.dwSNumber = 1;
				
				stLog.dwType = TRADE_ONE;
				::PropertyStreamLog(stLog);
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) transfer pet %u(%d) to %u(%s)", pUser1->GetID(), pUser1->GetName(), stLog.idTItem, stLog.idItemType, pUser2->GetID(), pUser2->GetName());
			}
		}
		for( i = 0; i < m_setPet2.size(); i++)
		{
			CUserPetData* pEudemon = pUserPet2->QueryData(m_setPet2[i]);
			ASSERT(pEudemon);
			if(pEudemon)
			{
				CUserPetData* pEudemonTemp = NULL;
				bool bSynchro = true;
				ASSERT(pEudemonTemp = pUser2->PopEudemon(pEudemon->GetID(), SYNCHRO_TRUE, UPDATE_FALSE));
				giveUser1PetSet.push_back( pEudemon ); 

				stLog.idItemType = pEudemon->GetInt(EUDEMONDATA_typeid);
				stLog.idTItem = stLog.idSItem = pEudemon->GetID();
				stLog.dwSNumber = 1;
				
				stLog.dwType = TRADE_TWO;
				::PropertyStreamLog(stLog);
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) transfer pet %u(%d) to %u(%s)", pUser2->GetID(), pUser2->GetName(), stLog.idTItem, stLog.idItemType, pUser1->GetID(), pUser1->GetName());
			}
		}
		//先删除各自背包中确认交易的物品,如果按以前的设计,在双方背包都满了,交易同样数量的物品会出问题
		for( i = 0;i < giveUser2PetSet.size();i++ )
		{
			BOOL bRet = FALSE;
			CUserPetData* pEudemon = giveUser2PetSet[ i ];
			ASSERT(bRet = pUser2->AddEudemon( pEudemon,true,true ) ); 
			pEudemon->SetLock(false);
		}
		for( i = 0;i < giveUser1PetSet.size();i++ )
		{
			BOOL bRet = FALSE;
			CUserPetData* pEudemon = giveUser1PetSet[ i ];
			ASSERT(bRet = pUser1->AddEudemon( pEudemon,true,true ) );
			pEudemon->SetLock(false);
		}
		//-------------------------宠物交易end------------------------------------------
		DEBUG_CATCH("★公平交易出错★")		// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

		CMsgTrade	msg;
		CHECKF(msg.Create(_TRADE_SUCCESS, 0));
		pUser1->SendMsg(&msg);
		pUser2->SendMsg(&msg);
		m_bTradeSuccess	= true;
		if (pUser1->FetchApply(CUser::APPLY_TRADE, pUser2->GetID()))
		{
			pUser1->SetApply(CUser::APPLY_NONE, pUser2->GetID());
		}
		if (pUser2->FetchApply(CUser::APPLY_TRADE, pUser1->GetID()))
		{
			pUser2->SetApply(CUser::APPLY_NONE, pUser1->GetID());
		}
// 		pUser1->SaveInfo();
// 		pUser2->SaveInfo();

		return true;
	}
	else
	{
		CMsgTrade	msg;
		CHECKF(msg.Create(_TRADE_FALSE, 0));
		pUser1->SendMsg(&msg);
		pUser2->SendMsg(&msg);
// 		pUser1->SendSysMsg(STR_TRADE_FAIL);
// 		pUser2->SendSysMsg(STR_TRADE_FAIL);
		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////
bool CSquareDeal::UserCancelTrade()
{
	CHECKF(m_pUser1 && m_pUser2);
	
	CUser*	pUser1	= m_pUser1->QueryUser();
	CUser*	pUser2	= m_pUser2->QueryUser();
	int i=0;
	for( i = 0;i < m_setItem1.size();i++ )
	{
		if(m_setItem1[i] == ID_NONE)
			continue;

		CItemPtr pItem = pUser1->GetItem(m_setItem1[i],false);
		if (pItem)
			pItem->setLocked(false);
	}
	for( i = 0;i < m_setItem2.size();i++ )
	{
		if(m_setItem2[i] == ID_NONE)
			continue;

		CItemPtr pItem = pUser2->GetItem(m_setItem2[i],false);
		if (pItem)
			pItem->setLocked(false);
	}
	if (m_setPet1.size()>0)
	{
		CUserPet* pUserPet1 = pUser1->QueryUserPetSet();//pUser1的宠物集合
		if (pUserPet1)
		{
			for( i = 0;i < m_setPet1.size();i++ )
			{
				CUserPetData* pEudemon = pUserPet1->QueryData(m_setPet1[i]);
				if (pEudemon)
					pEudemon->SetLock(false);
			}
		}
	}
	if (m_setPet2.size()>0)
	{
		CUserPet* pUserPet2 = pUser2->QueryUserPetSet();//pUser2的宠物集合
		if (pUserPet2)
		{
			for( i = 0;i < m_setPet2.size();i++ )
			{
				CUserPetData* pEudemon = pUserPet2->QueryData(m_setPet2[i]);
				if (pEudemon)
					pEudemon->SetLock(false);
			}
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////
// ISquareDeal 1
//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal1::Create(IUserSquareDeal* pUser, IUserSquareDeal* pTarget)
{
	This()->m_pUser1		= pUser;
	This()->m_pUser2		= pTarget;

	if(!pUser->Create(&This()->m_iface1))			//? 好象多余
		return false;
	if(!pTarget->Create(&This()->m_iface2))
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal1::AddItem(CItem* pItem)
{
	if (This()->m_setItem1.size()>=5)
	{
		This()->m_pUser1->QueryUser()->SendSysMsg(_TXTATR_TOPSHOW, "一次最多只能交易5件物品");
		return false;
	}
	if (This()->m_bLocked1)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return false;
	if (This()->m_bLocked2)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);

		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);

		This()->m_bLocked2=false;
	}
	CHECKF(pItem && pItem->GetID());

	if(This()->m_setItem1.size() >= MAX_SQUAREDEALITEMS)
		return false;

	// 检查重复加入
	if(find(This()->m_setItem1.begin(), This()->m_setItem1.end(), pItem->GetID()) != This()->m_setItem1.end())
		return false;

	if(!This()->m_pUser2->AddItem(pItem, This()->m_setItem1.size()+1, 0))
		return false;

	This()->m_setItem1.push_back(pItem->GetID());
	return true;
}

bool CSquareDeal::XSquareDeal1::DelItem(CItem* pItem)
{
	if (This()->m_bLocked1)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return false;
	if (This()->m_bLocked2)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);

		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);

		This()->m_bLocked2=false;
	}
	CHECKF(pItem && pItem->GetID());

	This()->m_pUser2->DelItem(pItem, This()->m_setItem1.size()+1, 0);


	ITEM_SET::iterator itt = This()->m_setItem1.begin();
	for(; itt != This()->m_setItem1.end(); itt++)
	{
		if(*itt == pItem->GetID())
		{
			This()->m_setItem1.erase(itt);
			break;
		}
	}
	
	return true;
}
bool CSquareDeal::XSquareDeal1::AddMonster(CUserPetData* pPetData)
{
	if (This()->m_setPet1.size()>=3)
	{
		This()->m_pUser1->QueryUser()->SendSysMsg(_TXTATR_TOPSHOW, "一次最多只能交易3只宠物");
		return false;
	}
	if (This()->m_bLocked1)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return false;
	if (This()->m_bLocked2)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);
		
		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);
		
		This()->m_bLocked2=false;
	}
	CHECKF(pPetData && pPetData->GetID());

	CEudemonTypeData* pEudemonType = EudemonType()->GetObj(pPetData->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType)
		return false;
	if (This()->m_pUser2->QueryUser()->GetLev() < pEudemonType->GetInt(EUDEMONTYPEDATA_needLev))
	{
		This()->m_pUser1->QueryUser()->SendSysMsg(_TXTATR_TOPSHOW, "宠物携带等级过高，对方无法携带");
		return false;
	}
	if (pPetData->GetInt(EUDEMONDATA_level) > This()->m_pUser2->QueryUser()->GetLev()+5)
	{
		This()->m_pUser1->QueryUser()->SendSysMsg(_TXTATR_TOPSHOW, "宠物等级超过对方人物等级5级以上，对方无法携带");
		return false;
	}
	
	// 检查重复加入
	if(find(This()->m_setPet1.begin(), This()->m_setPet1.end(), pPetData->GetID()) != This()->m_setPet1.end())
		return false;
	
	if(!This()->m_pUser2->AddMonster(pPetData, This()->m_setItem1.size()+1))
		return false;

	This()->m_setPet1.push_back(pPetData->GetID());
	return true;
}
bool CSquareDeal::XSquareDeal1::DelMonster(CUserPetData* pPetData)
{
	if (This()->m_bLocked1)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return false;
	if (This()->m_bLocked2)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);
		
		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);
		
		This()->m_bLocked2=false;
	}
	CHECKF(pPetData && pPetData->GetID());
	
	This()->m_pUser2->DelMonster(pPetData, This()->m_setPet1.size()+1);
	
	ITEM_SET::iterator itt = This()->m_setPet1.begin();
	for(; itt != This()->m_setPet1.end(); itt++)
	{
		if(*itt == pPetData->GetID())
		{
			This()->m_setPet1.erase(itt);
			break;
		}
	}
	
	return true;
}
bool CSquareDeal::XSquareDeal1::TradeLock()
{
	This()->m_bLocked1	=	true;//[2010-06-16 goto]判断交易时是否已经点击了锁定
	This()->m_pUser2->TradeLock();
	return true;
}

//////////////////////////////////////////////////////////////////////
int CSquareDeal::XSquareDeal1::AddMoney(int nMoney)		// return allmoney
{
	if (This()->m_bLocked1)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return -1;
	if (This()->m_bLocked2)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);

		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);

		This()->m_bLocked2=false;
	}
	This()->m_nMoney1 = 0;
	ASSERT(nMoney > 0);
	//if(nMoney <= 0)
	if(nMoney < 0)//[2010-06-03 goto] 金额允许输入0
		return This()->m_nMoney1;

	if(This()->m_nMoney1 + nMoney > MAX_SQUAREDEALMONEY)
		return This()->m_nMoney1;

	if(!This()->m_pUser2->AddMoney(This()->m_nMoney1 + nMoney))
		return This()->m_nMoney1;
	This()->m_nMoney1	+= nMoney;
	return This()->m_nMoney1;
}

int CSquareDeal::XSquareDeal1::AddVAS(int nMoney)		// return allmoney
{
	This()->m_nVas1 = 0;
	ASSERT(nMoney > 0);
	if(nMoney <= 0)
		return This()->m_nVas1;

	if(This()->m_nVas1 + nMoney > MAX_SQUAREDEALMONEY)
		return This()->m_nVas1;

	if(!This()->m_pUser2->AddTradeVAS(This()->m_nVas1 + nMoney))
		return This()->m_nVas1;
	This()->m_nVas1	+= nMoney;
	return This()->m_nVas1;
}

int CSquareDeal::XSquareDeal1::DelMoney(int nMoney)
{
	return 0;
}

int CSquareDeal::XSquareDeal1::DelVAS(int nMoney)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal1::ClickOK()
{
	if (!This()->m_bLocked1 || !This()->m_bLocked2)//双方都锁定后才能确定
		return false;
	CUser *pUser = This()->m_pUser1->QueryUser();
	CUser *pTarget = This()->m_pUser2->QueryUser();

	if(NULL == pTarget || NULL == pUser)
	{
		return false;
	}

	//有人钱不够，交易失败
	if (This()->m_nVas1 > pUser->GetVas()
		|| This()->m_nMoney1 > pUser->GetMoneyByType())
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足,交易失败");
		pTarget->SendSysMsg(_TXTATR_TOPSHOW, "对方银两不足,交易失败");
		return true;
	}
	if (This()->m_nVas2 > pTarget->GetVas()
		|| This()->m_nMoney2 > pTarget->GetMoneyByType())
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足,交易失败");
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方银两不足,交易失败");
		return true;
	}

	This()->m_bClickOK1		= true;
	This()->m_pUser2->ClickOK();

	if(This()->m_bClickOK2 == true)
	{
		if(!This()->TradeOK())
			This()->UserCancelTrade();

		return true;		// release by outside
	}

	return false;
}
//////////////////////////////////////////////////////////////////////
void CSquareDeal::XSquareDeal1::CancelTrade()
{
	This()->UserCancelTrade();
}
//////////////////////////////////////////////////////////////////////
CUser* CSquareDeal::XSquareDeal1::GetTarget()
{
	return This()->m_pUser2->QueryUser();
}

//////////////////////////////////////////////////////////////////////
// ISquareDeal 2
//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal2::Create(IUserSquareDeal* pUser, IUserSquareDeal* pTarget)
{
	This()->m_pUser2		= pUser;
	This()->m_pUser1		= pTarget;

	if(!pUser->Create(&This()->m_iface2))			//? 好象多余
		return false;
	if(!pTarget->Create(&This()->m_iface1))
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal2::AddItem(CItem* pItem)
{
	if (This()->m_setItem2.size()>=5)
	{
		This()->m_pUser2->QueryUser()->SendSysMsg(_TXTATR_TOPSHOW, "一次最多只能交易5件物品");
		return false;
	}
	if (This()->m_bLocked2)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return false;
	if (This()->m_bLocked1)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);

		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);

		This()->m_bLocked1=false;
	}
	CHECKF(pItem);

	if(This()->m_setItem2.size() >= MAX_SQUAREDEALITEMS)
		return false;

	// 检查重复加入
	if(find(This()->m_setItem2.begin(), This()->m_setItem2.end(), pItem->GetID()) != This()->m_setItem2.end())
		return false;

	if(!This()->m_pUser1->AddItem(pItem, This()->m_setItem2.size()+1, 0))
		return false;

	This()->m_setItem2.push_back(pItem->GetID());
	return true;
}

bool CSquareDeal::XSquareDeal2::DelItem(CItem* pItem)
{
	if (This()->m_bLocked2)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return false;
	if (This()->m_bLocked1)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);

		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);

		This()->m_bLocked1=false;
	}
	CHECKF(pItem && pItem->GetID());

	This()->m_pUser1->DelItem(pItem, This()->m_setItem2.size()+1, 0);

	ITEM_SET::iterator itt = This()->m_setItem2.begin();
	for(; itt != This()->m_setItem2.end(); itt++)
	{
		if(*itt == pItem->GetID())
		{
			This()->m_setItem2.erase(itt);
			break;
		}
	}
	
	return true;
}
bool CSquareDeal::XSquareDeal2::AddMonster(CUserPetData* pPetData)
{
	if (This()->m_setPet2.size()>=3)
	{
		This()->m_pUser2->QueryUser()->SendSysMsg(_TXTATR_TOPSHOW, "一次最多只能交易3只宠物");
		return false;
	}
	if (This()->m_bLocked2)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return false;
	if (This()->m_bLocked1)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);
		
		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);
		
		This()->m_bLocked1=false;
	}
	CHECKF(pPetData);
	CEudemonTypeData* pEudemonType = EudemonType()->GetObj(pPetData->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType)
		return false;
	if (This()->m_pUser1->QueryUser()->GetLev() < pEudemonType->GetInt(EUDEMONTYPEDATA_needLev))
	{
		This()->m_pUser2->QueryUser()->SendSysMsg(_TXTATR_TOPSHOW, "宠物携带等级过高，对方无法携带");
		return false;
	}
	if (pPetData->GetInt(EUDEMONDATA_level) > This()->m_pUser1->QueryUser()->GetLev()+5)
	{
		This()->m_pUser2->QueryUser()->SendSysMsg(_TXTATR_TOPSHOW, "宠物等级超过对方人物等级5级以上，对方无法携带");
		return false;
	}
	// 检查重复加入
	if(find(This()->m_setPet2.begin(), This()->m_setPet2.end(), pPetData->GetID()) != This()->m_setPet2.end())
		return false;
	
	if(!This()->m_pUser1->AddMonster(pPetData, This()->m_setPet2.size()+1))
		return false;
	
	This()->m_setPet2.push_back(pPetData->GetID());
	return true;
}
bool CSquareDeal::XSquareDeal2::DelMonster(CUserPetData* pPetData)
{
	if (This()->m_bLocked2)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return false;
	if (This()->m_bLocked1)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);
		
		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);
		
		This()->m_bLocked1=false;
	}
	CHECKF(pPetData && pPetData->GetID());
	
	This()->m_pUser1->DelMonster(pPetData, This()->m_setPet2.size()+1);
	
	ITEM_SET::iterator itt = This()->m_setPet2.begin();
	for(; itt != This()->m_setPet2.end(); itt++)
	{
		if(*itt == pPetData->GetID())
		{
			This()->m_setPet2.erase(itt);
			break;
		}
	}
	
	return true;
}
bool CSquareDeal::XSquareDeal2::TradeLock()
{
	This()->m_bLocked2	=	true;
	This()->m_pUser1->TradeLock();
	return true;
}

//////////////////////////////////////////////////////////////////////
int CSquareDeal::XSquareDeal2::AddMoney(int nMoney)		// return allmoney
{
	if (This()->m_bLocked2)//[2010-06-16 goto]锁定后不允许再修改交易物品
		return -1;
	if (This()->m_bLocked1)////[2010-06-16 goto]对方如果是锁定状态，则通知对方解锁
	{
		CMsgTrade msg;
		msg.Create(_TRADE_UNLOCK,0,0);
		This()->m_pUser1->QueryUser()->SendMsg(&msg);

		msg.Create(_TRADE_UNLOCK_OP,0,0);
		This()->m_pUser2->QueryUser()->SendMsg(&msg);

		This()->m_bLocked1=false;
	}
	This()->m_nMoney2 = 0;
	ASSERT(nMoney > 0);
	//if(nMoney <= 0)
	if(nMoney < 0)//[2010-06-03 goto] 金额允许输入0
		return This()->m_nMoney2;

	if(This()->m_nMoney2 + nMoney > MAX_SQUAREDEALMONEY)
		return This()->m_nMoney2;

	if(!This()->m_pUser1->AddMoney(This()->m_nMoney2 + nMoney))
		return This()->m_nMoney2;
	This()->m_nMoney2	+= nMoney;
	return This()->m_nMoney2;
}

int CSquareDeal::XSquareDeal2::AddVAS(int nMoney)		// return allmoney
{
	This()->m_nVas2 = 0;
	ASSERT(nMoney > 0);
	if(nMoney <= 0)
		return This()->m_nVas2;

	if(This()->m_nVas2 + nMoney > MAX_SQUAREDEALMONEY)
		return This()->m_nVas2;

	if(!This()->m_pUser1->AddTradeVAS(This()->m_nVas2 + nMoney))
		return This()->m_nVas2;
	This()->m_nVas2	+= nMoney;
	return This()->m_nVas2;
}

int CSquareDeal::XSquareDeal2::DelMoney(int nMoney)
{
	return 0;
}

int CSquareDeal::XSquareDeal2::DelVAS(int nMoney)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
bool CSquareDeal::XSquareDeal2::ClickOK()
{
	if (!This()->m_bLocked1 || !This()->m_bLocked2)//双方都锁定时才能确定
		return false;
	//[游途道标 2009.04.15]摊位判断
	CUser *pUser = This()->m_pUser1->QueryUser();
	CUser *pTarget = This()->m_pUser2->QueryUser();

	if(NULL == pTarget || NULL == pUser)
	{
		return false;
	}

	if (pUser->QueryBooth())
	{
		const int _maxSize=120;
		int sourceItemAmount = 0;
		int sourceItemSize=_maxSize;
		OBJID sourceItemID[_maxSize] = {0};
		sourceItemAmount = pUser->GetItemByTypeMulti(880100, sourceItemSize, sourceItemID);
		
		if (!sourceItemAmount)
		{
			pTarget->SendSysMsg("对方正在摆摊无法顺利完成交易.");
			return false;
		}
	}
	
	if (pTarget->QueryBooth())
	{
		const int _maxSize=120;
		int sourceItemAmount = 0;
		int sourceItemSize=_maxSize;
		OBJID sourceItemID[_maxSize] = {0};
		sourceItemAmount = pTarget->GetItemByTypeMulti(880100, sourceItemSize, sourceItemID);
		
		if (!sourceItemAmount)
		{
			pUser->SendSysMsg("对方正在摆摊无法顺利完成交易.");
			return false;
		}
	}
	
	//有人钱不够，交易失败
	if (This()->m_nVas1 > pUser->GetVas()
	   || This()->m_nMoney1 > pUser->GetMoneyByType())
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足,交易失败");
		pTarget->SendSysMsg(_TXTATR_TOPSHOW, "对方银两不足,交易失败");
		return true;
	}
	if (This()->m_nVas2 > pTarget->GetVas()
	   || This()->m_nMoney2 > pTarget->GetMoneyByType())
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW, "包裹中的银两不足,交易失败");
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方银两不足,交易失败");
		return true;
	}

	This()->m_bClickOK2		= true;
	This()->m_pUser1->ClickOK();

	if(This()->m_bClickOK1 == true)
	{
		if(!This()->TradeOK())
		{
			This()->UserCancelTrade();
		}

		return true;		// release by outside
	}

	return false;
}
//////////////////////////////////////////////////////////////////////
void CSquareDeal::XSquareDeal2::CancelTrade()
{
	This()->UserCancelTrade();
}
//////////////////////////////////////////////////////////////////////
CUser* CSquareDeal::XSquareDeal2::GetTarget()
{
	return This()->m_pUser1->QueryUser();
}






