// MsgVAS.cpp: implementation of the CMsgVAS class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdisp.h>
#include "MsgVAS.h"
#include "MapGroup.h"
#include "MsgUserAttrib.h"
#include "AdjustRate.h"
#include "MsgItemAttrib.h"

#include "WargameManager.h"
#include "MapGroup.h"
#include "SKPostOffice.h"
#include "MapGroup.h"
#include "MsgMainServer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgVAS::CMsgVAS()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
//	m_StrPacker.SetBuf(m_pInfo->szRegard, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgVAS::~CMsgVAS()
{
}

// BOOL CMsgVAS::Create( void* pInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID )///生成增值服务请求消息
// {
	// init
// 	this->Init();
// 
// 	// fill info now
// 	MsgTraffic* pMsgTraffic=(MsgTraffic*)pInfo;
// 	m_pInfo->unMsgType = _MSG_VAS;
// 	m_pInfo->mtExtraInfo.itemID = pMsgTraffic->itemID;
// 	m_pInfo->mtExtraInfo.itemAmount = pMsgTraffic->itemAmount;
// 	m_pInfo->usVASPoint = usVASPoint;///增值点数
// 	m_pInfo->AccountID = AccountID;///帐号ID
// 	m_pInfo->PlayerID = PlayerID;///角色ID
// 
// 	m_unMsgType	=_MSG_VAS;
// 	m_unMsgSize	=sizeof( MSG_Info );

// 	return true;
// }

BOOL CMsgVAS::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_VAS != this->GetType())
		return false;

	return true;
}

void CMsgVAS::Process(void *pInfo)
{
	bool overTenFlag=false;
	
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;

	switch( m_pInfo->usAction )
	{
	//[2010-07-26 goto]查询元宝余额
	case _MSGVAS_QUERY_BALANCE:
		{
			if (m_pInfo->PlayerID!=pUser->GetID())
				return;
			CMsgVAS msg;
			msg.Create(m_pInfo->usVASPoint, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_QUERY_BALANCE);
			msg.m_pInfo->unMsgType = m_pInfo->unMsgType;
			msg.m_pInfo->unMsgSize = m_pInfo->unMsgSize;
			//给世界线程发消息
 			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
		}
		break;
	case _MSGVAS_CARD_STATES:
		{
			pUser->SetCardState(m_pInfo->usVASPoint / 1000 ,m_pInfo->usVASPoint % 1000);
		}
		break;
	case _MSGVAS_RETURN_BALANCE://账号服务器返回的余额消息，直接转发给客户端即可
		{
			if (m_pInfo->usVASPoint==(ULONG)-1)//账户服务器查询出错
				return;

			pUser->SetMoneyByType(m_pInfo->usVASPoint, 5, false);//同步到用户对象中
			pUser->SetGoldPoint(m_pInfo->AccountID);
			CMsgUserAttrib	msg;//同步到客户端
			if(msg.Create(m_pInfo->PlayerID, _USERATTRIB_MONEY5, m_pInfo->usVASPoint))
			{
				msg.Append(_USERATTRIB_MONEY3, pUser->GetMoneyByType(3)+pUser->GetGoldPoint());
				pUser->SendMsg(&msg);
			}
// 			CMsgVAS msg;
// 			msg.Create(m_pInfo->usVASPoint, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_RETURN_BALANCE);
// 			pUser->SendMsg(&msg);
		}
		break;
	case _MSGVAS_BUY_ITEM:
		{
			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				CMsgVAS msgVAS;
				msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 				pUser->SendMsg(&msgVAS);
				return;
			}
			int nItemCount = (m_pInfo->unMsgSize-24)/sizeof(MsgTraffic);
			if (nItemCount!=m_pInfo->usGoodSize || nItemCount>1 || m_pInfo->usGoodSize>1)
			{
				CMsgVAS msgVAS;
				msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 				pUser->SendMsg(&msgVAS);
				return;
			}

			if (m_pInfo->PlayerID!=pUser->GetID())
			{
				CMsgVAS msgVAS;
				msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 				pUser->SendMsg(&msgVAS);
				return;
			}
			if (m_pInfo->usGoodSize==0 || m_pInfo->usGoodSize>10)//购物车里最多10件商品
			{
				CMsgVAS msgVAS;
				msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 				pUser->SendMsg(&msgVAS);
				return;
			}
			CMsgVAS msg;
			msg.Create(m_pInfo->usVASPoint, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_ITEM);
			int nCostVAS = 0;
			int nCostGem = 0;
			int nCostPoint = 0;
			int nPak1=0, nPak2=0,nPak3=0,nPakPet=0;
			for (int i=0; i<m_pInfo->usGoodSize && i<10; i++)
			{
				CItemTypeData* pItemTypeData = ItemType()->QueryItemType( m_pInfo->nGoods[i].itemID );
				if (!pItemTypeData)
				{
					CMsgVAS msgVAS;
					msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 					pUser->SendMsg(&msgVAS);
					return;
				}

				msg.Add(m_pInfo->nGoods[i].itemID, m_pInfo->nGoods[i].itemAmount, m_pInfo->nGoods[i].dwData);
				//背包空间计算
				int nPosition = pUser->QueryPackage()->GetPositionByItemType(m_pInfo->nGoods[i].itemID);
				if (nPosition==ITEMPOSITION_BACKPACK1)
				{
					if (CItem::IsCountable(m_pInfo->nGoods[i].itemID))
					{
						int nCountTemp = m_pInfo->nGoods[i].itemAmount%pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)>0 ? 1 : 0;//一个格子叠加后还有多的
						nPak1=nPak1+m_pInfo->nGoods[i].itemAmount/pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)+nCountTemp;
					}
					else
						nPak1+=m_pInfo->nGoods[i].itemAmount;
				}
				else if (nPosition==ITEMPOSITION_BACKPACK2)
				{
					if (CItem::IsCountable(m_pInfo->nGoods[i].itemID))
					{
						int nCountTemp = m_pInfo->nGoods[i].itemAmount%pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)>0 ? 1 : 0;//一个格子叠加后还有多的
						nPak2=nPak2+m_pInfo->nGoods[i].itemAmount/pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)+nCountTemp;
					}
					else
						nPak2+=m_pInfo->nGoods[i].itemAmount;
				}
				else if (nPosition==ITEMPOSITION_BACKPACK3)
				{
					if (CItem::IsCountable(m_pInfo->nGoods[i].itemID))
					{
						int nCountTemp = m_pInfo->nGoods[i].itemAmount%pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)>0 ? 1 : 0;//一个格子叠加后还有多的
						nPak3=nPak3+m_pInfo->nGoods[i].itemAmount/pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)+nCountTemp;
					}
					else
						nPak3+=m_pInfo->nGoods[i].itemAmount;
				}
				else if (nPosition==ITEMPOSITION_PETPACK)
					nPakPet+=m_pInfo->nGoods[i].itemAmount;
				else if (nPosition==-1)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "包裹空间不足");
					CMsgVAS msgVAS;
					msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 					pUser->SendMsg(&msgVAS);
					return;
				}

				vasItem* pVasItem = NULL;
				OBJID itemType = 0;
				if (m_pInfo->nGoods[i].dwData==_PAY_TYPE_POINT)
					itemType = m_pInfo->nGoods[i].itemID*10;
				else
					itemType = m_pInfo->nGoods[i].itemID;

				pVasItem = VASItem()->GetItemByType(itemType);

				if (!pVasItem)
				{
					CMsgVAS msgVAS;
					msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 					pUser->SendMsg(&msgVAS);
					return;
				}
				if (!VASItem()->IsItemOnSale(pVasItem->time))
				{
					CMsgVAS msgVAS;
					msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
					pUser->SendMsg(&msgVAS);
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "此商品已下架");
					return;
				}
				if (VASItem()->IsSaledOut(m_pInfo->nGoods[i].itemID, m_pInfo->nGoods[i].itemAmount))
				{
					CMsgVAS msgVAS;
					msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
					pUser->SendMsg(&msgVAS);
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "库存的%s数量不足", pItemTypeData->GetStr(ITEMTYPEDATA_NAME));
					return;
				}
				pVasItem = VASItem()->GetItemByType(itemType);//不知道为什么必须重新赋一次值，否则数据就全是错的
				CHECK(pVasItem);

				if (m_pInfo->nGoods[i].dwData==_PAY_TYPE_VAS)
				{
					if (pVasItem->payType!=0)//非元宝商品
					{
						CMsgVAS msgVAS;
						msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 						pUser->SendMsg(&msgVAS);
						return;
					}
					int nPriceTotal = 0;//物品价格
					if (pVasItem->discount==0)//没有折扣
						nPriceTotal = (pVasItem->price*m_pInfo->nGoods[i].itemAmount);
					else//有折扣
						nPriceTotal = (pVasItem->price*m_pInfo->nGoods[i].itemAmount)*(pVasItem->discount/100.0);

					nCostVAS += nPriceTotal;
					if(pVasItem->price>10)
					{
						overTenFlag=true;
					}
				}
				else if (m_pInfo->nGoods[i].dwData==_PAY_TYPE_GEM)
				{
					if (pVasItem->payType!=0)//非元宝商品
					{
						CMsgVAS msgVAS;
						msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 						pUser->SendMsg(&msgVAS);
						return;
					}
					int nPriceTotal = 0;//物品价格
					if (pVasItem->discount==0)//没有折扣
						nPriceTotal = (pVasItem->price*m_pInfo->nGoods[i].itemAmount);
					else//有折扣
						nPriceTotal = (pVasItem->price*m_pInfo->nGoods[i].itemAmount)*(pVasItem->discount/100.0);

					nCostGem += nPriceTotal;
					msg.m_pInfo->nGoods[i].dwData += nPriceTotal*10;//价格的个位是支付类型
				}
				else if (m_pInfo->nGoods[i].dwData==_PAY_TYPE_POINT)
				{
					if (pVasItem->payType!=1)//非点券商品
					{
						CMsgVAS msgVAS;
						msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 						pUser->SendMsg(&msgVAS);
						return;
					}
					int nPriceTotal = 0;//物品价格
					if (pVasItem->discount==0)//没有折扣
						nPriceTotal = (pVasItem->price*m_pInfo->nGoods[i].itemAmount);
					else//有折扣
						nPriceTotal = (pVasItem->price*m_pInfo->nGoods[i].itemAmount)*(pVasItem->discount/100.0);
					
					nCostPoint += nPriceTotal;
					msg.m_pInfo->nGoods[i].dwData += nPriceTotal*10;//价格的个位是支付类型
				}
				else//支付方式有问题
				{
					CMsgVAS msgVAS;
					msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 					pUser->SendMsg(&msgVAS);
					return;
				}
			}
			if (pUser->GetMoneyByType(4)<nCostGem)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "珠宝不足");
				CMsgVAS msgVAS;
				msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 				pUser->SendMsg(&msgVAS);
				return;
			}
			if (pUser->GetMoneyByType(3)+pUser->GetGoldPoint()<nCostPoint)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "点券不足");
				CMsgVAS msgVAS;
				msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 				pUser->SendMsg(&msgVAS);
				return;
			}
			if (pUser->QueryPackage()->GetFreeSpaceCount(ITEMPOSITION_BACKPACK1, true)<nPak1
				|| pUser->QueryPackage()->GetFreeSpaceCount(ITEMPOSITION_BACKPACK2, true)<nPak2
				|| pUser->QueryPackage()->GetFreeSpaceCount(ITEMPOSITION_BACKPACK3, true)<nPak3
				/*|| pUser->QueryPackage()->GetFreeSpaceCount(ITEMPOSITION_PETPACK, true)<nPakPet*/)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "包裹空间不足");
				CMsgVAS msgVAS;
				msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 				pUser->SendMsg(&msgVAS);
				return;
			}
			if (pUser->GetMoneyByType(5)<nCostVAS)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "元宝不足");
				CMsgVAS msgVAS;
				msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 				pUser->SendMsg(&msgVAS);
				return;
			}
			if (nCostVAS)//有元宝消费时才需要发送到账号服务器
			{
				//预留背包位置
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_BACKPACK1, nPak1);
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_BACKPACK2, nPak2);
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_BACKPACK3, nPak3);
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_PETPACK, nPakPet);

				msg.m_pInfo->usVASPoint = nCostVAS;
				msg.m_pInfo->unMsgType = m_pInfo->unMsgType;
				msg.m_pInfo->unMsgSize = m_pInfo->unMsgSize;
									
				if(overTenFlag)
				{
					if(pUser->ProcessAction(1151))
					{
						pUser->ProcessAction(1150);
						overTenFlag=false;
					}
				}

 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );//给世界线程发消息

			}
			else if (nCostPoint && pUser->GetMoneyByType(3)<nCostPoint)//点券
			{
				//预留背包位置
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_BACKPACK1, nPak1);
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_BACKPACK2, nPak2);
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_BACKPACK3, nPak3);
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_PETPACK, nPakPet);
				
				msg.m_pInfo->usAction = _MSGVAS_POINT_BUY;
				msg.m_pInfo->usVASPoint = nCostPoint-pUser->GetMoneyByType(3);
				msg.m_pInfo->unMsgType = m_pInfo->unMsgType;
				msg.m_pInfo->unMsgSize = m_pInfo->unMsgSize;
 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );//给世界线程发消息
				if (pUser->GetMoneyByType(3)>0)
					pUser->SetMoneyByType(0,3,true);
			}
			else//无元宝消费,不需要发送到账号服务器处理
			{
				for (int i=0; i<m_pInfo->usGoodSize; i++)
				{
					CItemTypeData* pItemTypeData = ItemType()->QueryItemType( m_pInfo->nGoods[i].itemID );
					if (!pItemTypeData)
					{
						CMsgVAS msgVAS;
						msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 						pUser->SendMsg(&msgVAS);
						return;
					}

					//vasItem* pVasItem = VASItem()->GetItemByType(m_pInfo->nGoods[i].itemID*10);
					vasItem* pVasItem = NULL;
					if (m_pInfo->nGoods[i].dwData==_PAY_TYPE_POINT)
						pVasItem = VASItem()->GetItemByType(m_pInfo->nGoods[i].itemID*10);
					else
						pVasItem = VASItem()->GetItemByType(m_pInfo->nGoods[i].itemID);
					if (!pVasItem)
					{
						CMsgVAS msgVAS;
						msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 						pUser->SendMsg(&msgVAS);
						return;
					}
					if (m_pInfo->nGoods[i].dwData%10==_PAY_TYPE_GEM)//珠宝支付,物品获取后绑定
					{
						if (pVasItem->payType!=0)
						{
							CMsgVAS msgVAS;
							msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 							pUser->SendMsg(&msgVAS);
							return;
						}
						if(pUser->SpendMoneyByType(msg.m_pInfo->nGoods[i].dwData/10, 4, false))
						{
							CItem* pItem = NULL;
							int nAmountLimit = pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
							if (CItem::IsCountable(m_pInfo->nGoods[i].itemID))
							{
								int nItemCount = m_pInfo->nGoods[i].itemAmount;
								while (nItemCount>0)
								{
									if (nItemCount>nAmountLimit)
									{
										pItem = pUser->AwardItem(m_pInfo->nGoods[i].itemID, true, false, true, true, nAmountLimit, false, true, false, 1);
										nItemCount-=nAmountLimit;
										if (!pItem)
											::LogSave("商城购物给物品失败(7) user=%u,itemtype=%u,amount=%d", pUser->GetID(), m_pInfo->nGoods[i].itemID, nAmountLimit);
									}
									else
									{
										pItem = pUser->AwardItem(m_pInfo->nGoods[i].itemID, true, false, true, true, nItemCount, false, true, false, 1);
										if (!pItem)
											::LogSave("商城购物给物品失败(8) user=%u,itemtype=%u,amount=%d", pUser->GetID(), m_pInfo->nGoods[i].itemID, nItemCount);
										nItemCount=0;
									}
								}
							}
							else
							{
								for (int j=0; j<m_pInfo->nGoods[i].itemAmount; j++)
								{
									pItem = pUser->AwardItem(m_pInfo->nGoods[i].itemID, true, false, true, true, m_pInfo->nGoods[i].itemAmount, false, true, false, 1);
									if (!pItem)
										::LogSave("商城购物给物品失败(9) user=%u,itemtype=%u,amount=%d", pUser->GetID(), m_pInfo->nGoods[i].itemID, m_pInfo->nGoods[i].itemAmount);
								}
							}
							pUser->SendSysMsg(_TXTATR_TOPSHOW, "花费%d珠宝购买了%d个[%s]", msg.m_pInfo->nGoods[i].dwData/10, m_pInfo->nGoods[i].itemAmount, pItemTypeData->GetStr(ITEMTYPEDATA_NAME));
 							if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
								::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from mart by %d jewel", pUser->GetID(), pUser->GetName(), m_pInfo->nGoods[i].itemAmount, m_pInfo->nGoods[i].itemID, msg.m_pInfo->nGoods[i].dwData/10);

							vasLimitItem* pLimitItem = VASItem()->GetLimitItemByType(m_pInfo->nGoods[i].itemID);
							if(pLimitItem)
							{
								int nAmount = max(0, pLimitItem->aSaleCount - m_pInfo->nGoods[i].itemAmount);
								VASItem()->SetLimitItemAmount(m_pInfo->nGoods[i].itemID, nAmount, true);
								pUser->SendLimitItem();
								CMsgVipList msg;//借用这个协议广播到其他线
								if (msg.Create(pUser->GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_VASLIMIT))
								{
									msg.Append(m_pInfo->nGoods[i].itemID, nAmount, 0, 0, 0, pUser->GetName());
									MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
								}
							}
						}
					}
					else if (m_pInfo->nGoods[i].dwData%10==_PAY_TYPE_POINT)//点券支付
					{
						if (pVasItem->payType!=1)
						{
							CMsgVAS msgVAS;
							msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 							pUser->SendMsg(&msgVAS);
							return;
						}
						if(pUser->SpendMoneyByType(msg.m_pInfo->nGoods[i].dwData/10, 3, false))
						{
							CItem* pItem = NULL;
							int nAmountLimit = pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
							if (CItem::IsCountable(m_pInfo->nGoods[i].itemID))
							{
								int nItemCount = m_pInfo->nGoods[i].itemAmount;
								while (nItemCount>0)
								{
									if (nItemCount>nAmountLimit)
									{
										pItem = pUser->AwardItem(m_pInfo->nGoods[i].itemID, true, false, true, true, nAmountLimit, false, true, false, 1);
										nItemCount-=nAmountLimit;
										if (!pItem)
											::LogSave("商城购物给物品失败(10) user=%u,itemtype=%u,amount=%d", pUser->GetID(), m_pInfo->nGoods[i].itemID, nAmountLimit);
									}
									else
									{
										pItem = pUser->AwardItem(m_pInfo->nGoods[i].itemID, true, false, true, true, nItemCount, false, true, false, 1);
										if (!pItem)
											::LogSave("商城购物给物品失败(11) user=%u,itemtype=%u,amount=%d", pUser->GetID(), m_pInfo->nGoods[i].itemID, nItemCount);
										nItemCount=0;
									}
								}
							}
							else
							{
								for (int j=0; j<m_pInfo->nGoods[i].itemAmount; j++)
								{
									pItem = pUser->AwardItem(m_pInfo->nGoods[i].itemID, true, false, true, true, m_pInfo->nGoods[i].itemAmount, false, true, false, 1);
									if (!pItem)
										::LogSave("商城购物给物品失败(12) user=%u,itemtype=%u,amount=%d", pUser->GetID(), m_pInfo->nGoods[i].itemID, m_pInfo->nGoods[i].itemAmount);
								}
							}
							pUser->SendSysMsg(_TXTATR_TOPSHOW, "花费%d点券购买了%d个[%s]", msg.m_pInfo->nGoods[i].dwData/10, m_pInfo->nGoods[i].itemAmount, pItemTypeData->GetStr(ITEMTYPEDATA_NAME));
							if (pItem && pItem->IsCostlyItem())
								::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from mart by %d ticket", pUser->GetID(), pUser->GetName(), m_pInfo->nGoods[i].itemAmount, m_pInfo->nGoods[i].itemID, msg.m_pInfo->nGoods[i].dwData/10);
							
							vasLimitItem* pLimitItem = VASItem()->GetLimitItemByType(m_pInfo->nGoods[i].itemID);
							if(pLimitItem)
							{
								int nAmount = max(0, pLimitItem->aSaleCount - m_pInfo->nGoods[i].itemAmount);
								VASItem()->SetLimitItemAmount(m_pInfo->nGoods[i].itemID, nAmount, true);
								pUser->SendLimitItem();
								CMsgVipList msg;//借用这个协议广播到其他线
								if (msg.Create(pUser->GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_VASLIMIT))
								{
									msg.Append(m_pInfo->nGoods[i].itemID, nAmount, 0, 0, 0, pUser->GetName());
									MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
								}
							}
						}
					}
				}
				CMsgUserAttrib	msg;
				if(msg.Create(m_pInfo->PlayerID, _USERATTRIB_MONEY4, pUser->GetMoneyByType(4)))
				{
					msg.Append(_USERATTRIB_MONEY3, pUser->GetMoneyByType(3)+pUser->GetGoldPoint());
					SendMsg(&msg);
				}
// 				CMsgVAS msgVas;
// 				msgVas.Create(m_pInfo->usVASPoint, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_SUC);
// 				pUser->SendMsg(&msgVas);
			}
		}
		break;
	case _MSGVAS_ADD_VAS_FAIL:
		{
			pUser->SendSysMsg("领取失败");
		}
		break;
	case _MSGVAS_SPEND_VAS_FAIL:
		{
			pUser->SendSysMsg("元宝不足");
		}
		break;
	case _MSGVAS_BUY_SUC://账号服务器返回购买成功
		{
		}
		break;
	case _MSGVAS_BUY_FAIL://账号服务器返回失败
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "元宝不足");
			CMsgVAS msgVAS;
 			msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 			pUser->SendMsg(&msgVAS);
			return;
		}
		break;
	case _MSGVAS_POINT_BUY_SUC:
		{
 		}
		break;
	case _MSGVAS_POINT_BUY_FAIL:
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "点券不足");
			CMsgVAS msgVAS;
			msgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_FAIL);
 			pUser->SendMsg(&msgVAS);
			return;
		}
		break;
	case _MSGVAS_SYN_ASALEITEM://同步特价商品
		{
			pUser->SendASalesItem();
			pUser->SendLimitItem();
		}
		break;
	case _MSGVAS_BUY_ASALE://买特价商品
		{
			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先关闭账号安全锁");
				return;
			}
			int nItemCount = (m_pInfo->unMsgSize-24)/sizeof(MsgTraffic);
			if (nItemCount!=m_pInfo->usGoodSize || nItemCount>1 || m_pInfo->usGoodSize>1)
				return;

			if (m_pInfo->PlayerID!=pUser->GetID())
				return;

			if (m_pInfo->usGoodSize==0 || m_pInfo->usGoodSize>ASALE_ITEM_AMOUNT)//特价商品，每天只有那么几种
				return;

			CMsgVAS msg;
			msg.Create(m_pInfo->usVASPoint, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_ASALE);
			int nCostVAS = 0, nCostGem = 0;
			int nPak1=0, nPak2=0,nPak3=0,nPakPet=0;
			for (int i=0; i<m_pInfo->usGoodSize && i<10; i++)
			{
				CItemTypeData* pItemTypeData = ItemType()->QueryItemType( m_pInfo->nGoods[i].itemID );
				if (!pItemTypeData)
					return;

				if (m_pInfo->nGoods[i].itemAmount != 1)//只能买一个
					m_pInfo->nGoods[i].itemAmount = 1;

				msg.Add(m_pInfo->nGoods[i].itemID, m_pInfo->nGoods[i].itemAmount, m_pInfo->nGoods[i].dwData);
				//背包空间计算
				int nPosition = pUser->QueryPackage()->GetPositionByItemType(m_pInfo->nGoods[i].itemID);
				if (nPosition==ITEMPOSITION_BACKPACK1)
				{
					if (CItem::IsCountable(m_pInfo->nGoods[i].itemID))
					{
						int nCountTemp = m_pInfo->nGoods[i].itemAmount%pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)>0 ? 1 : 0;//一个格子叠加后还有多的
						nPak1=nPak1+m_pInfo->nGoods[i].itemAmount/pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)+nCountTemp;
					}
					else
						nPak1+=m_pInfo->nGoods[i].itemAmount;
				}
				else if (nPosition==ITEMPOSITION_BACKPACK2)
				{
					if (CItem::IsCountable(m_pInfo->nGoods[i].itemID))
					{
						int nCountTemp = m_pInfo->nGoods[i].itemAmount%pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)>0 ? 1 : 0;//一个格子叠加后还有多的
						nPak2=nPak2+m_pInfo->nGoods[i].itemAmount/pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)+nCountTemp;
					}
					else
						nPak2+=m_pInfo->nGoods[i].itemAmount;
				}
				else if (nPosition==ITEMPOSITION_BACKPACK3)
				{
					if (CItem::IsCountable(m_pInfo->nGoods[i].itemID))
					{
						int nCountTemp = m_pInfo->nGoods[i].itemAmount%pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)>0 ? 1 : 0;//一个格子叠加后还有多的
						nPak3=nPak3+m_pInfo->nGoods[i].itemAmount/pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)+nCountTemp;
					}
					else
						nPak3+=m_pInfo->nGoods[i].itemAmount;
				}
				else if (nPosition==ITEMPOSITION_PETPACK)
					nPakPet+=m_pInfo->nGoods[i].itemAmount;
				else if (nPosition==-1)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "包裹空间不足");
					return;
				}

				vasItem* pVasItem = VASItem()->GetASaleItemByType(m_pInfo->nGoods[i].itemID);
				if (!pVasItem || pVasItem->aSaleCount<m_pInfo->nGoods[i].itemAmount)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "特价商品已售完或已刷新");
					return;
				}
				//检查玩家是否已经买过此特价商品
				int nTemp = 1;
				char szText[64] = "";
				for (int nCount = 0; nCount<ASALE_ITEM_AMOUNT; nCount++)
				{
					if ((pVasItem->id-10000)==pUser->GetASales()/nTemp % 100)
					{
						sprintf(szText, "你今天已经买过特价商品[%s]", pItemTypeData->GetStr(ITEMTYPEDATA_NAME));
						pUser->SendSysMsg(_TXTATR_TOPSHOW, szText);
						return;
					}
					nTemp *= 100;
				}

				if (m_pInfo->nGoods[i].dwData==_PAY_TYPE_VAS)
				{
					int nPriceTotal = pVasItem->price*m_pInfo->nGoods[i].itemAmount;
					nCostVAS += nPriceTotal;
				}
				else if (m_pInfo->nGoods[i].dwData==_PAY_TYPE_GEM)
				{
					int nPriceTotal = pVasItem->price*m_pInfo->nGoods[i].itemAmount;//物品价格
					nCostGem += nPriceTotal;
					msg.m_pInfo->nGoods[i].dwData += nPriceTotal*10;//价格的个位是支付类型
				}
				else//支付方式有问题
				{
					return;
				}
			}
			if (pUser->GetMoneyByType(4)<nCostGem)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "珠宝不足");
				return;
			}
			if (pUser->QueryPackage()->GetFreeSpaceCount(ITEMPOSITION_BACKPACK1, true)<nPak1
				|| pUser->QueryPackage()->GetFreeSpaceCount(ITEMPOSITION_BACKPACK2, true)<nPak2
				|| pUser->QueryPackage()->GetFreeSpaceCount(ITEMPOSITION_BACKPACK3, true)<nPak3
				/*|| pUser->QueryPackage()->GetFreeSpaceCount(ITEMPOSITION_PETPACK, true)<nPakPet*/)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "包裹空间不足");
				return;
			}
			if (pUser->GetMoneyByType(5)<nCostVAS)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "元宝不足");
				return;
			}
			if (nCostVAS)//有元宝消费时才需要发送到账号服务器
			{
				//预留背包位置
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_BACKPACK1, nPak1);
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_BACKPACK2, nPak2);
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_BACKPACK3, nPak3);
				pUser->QueryPackage()->SetIndexOrderAmount(ITEMPOSITION_PETPACK, nPakPet);

				msg.m_pInfo->usVASPoint = nCostVAS;
				msg.m_pInfo->unMsgType = m_pInfo->unMsgType;
				msg.m_pInfo->unMsgSize = m_pInfo->unMsgSize;
 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );//给世界线程发消息
			}
			else//无元宝消费,不需要发送到账号服务器处理
			{
				for (int i=0; i<m_pInfo->usGoodSize; i++)
				{
					CItemTypeData* pItemTypeData = ItemType()->QueryItemType( m_pInfo->nGoods[i].itemID );
					if (!pItemTypeData)
						return;

					vasItem* pVasItem = VASItem()->GetASaleItemByType(m_pInfo->nGoods[i].itemID);
					if (!pVasItem || pVasItem->aSaleCount<m_pInfo->nGoods[i].itemAmount)
					{
						pUser->SendSysMsg(_TXTATR_TOPSHOW, "特价商品已售完或已刷新");
						return;
					}
					//检查玩家是否已经买过此特价商品
					int nTemp = 1;
					char szText[64] = "";
					for (int nCount = 0; nCount<ASALE_ITEM_AMOUNT; nCount++)
					{
						if ((pVasItem->id-10000)==pUser->GetASales()/nTemp % 100)
						{
							sprintf(szText, "你今天已经买过特价商品[%s]", pItemTypeData->GetStr(ITEMTYPEDATA_NAME));
							pUser->SendSysMsg(_TXTATR_TOPSHOW, szText);
							return;
						}
						nTemp *= 100;
					}
					if (m_pInfo->nGoods[i].itemAmount!=1)//只能买一个
						m_pInfo->nGoods[i].itemAmount=1;
					if (m_pInfo->nGoods[i].dwData%10==_PAY_TYPE_GEM)//珠宝支付,物品获取后绑定
					{
						if(pUser->SpendMoneyByType(msg.m_pInfo->nGoods[i].dwData/10, 4, false))
						{
							CItem* pItem = NULL;
							int nAmountLimit = pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
							if (CItem::IsCountable(m_pInfo->nGoods[i].itemID))
							{
								int nItemCount = m_pInfo->nGoods[i].itemAmount;
								while (nItemCount>0)
								{
									if (nItemCount>nAmountLimit)
									{
										pItem = pUser->AwardItem(m_pInfo->nGoods[i].itemID, true, false, true, true, nAmountLimit, false, true, false, 1);
										nItemCount-=nAmountLimit;
										if (!pItem)
											::LogSave("商城购物给特价物品失败(1) user=%u,itemtype=%u,amount=%d", pUser->GetID(), m_pInfo->nGoods[i].itemID, nAmountLimit);
									}
									else
									{
										pItem = pUser->AwardItem(m_pInfo->nGoods[i].itemID, true, false, true, true, nItemCount, false, true, false, 1);
										if (!pItem)
											::LogSave("商城购物给特价物品失败(2) user=%u,itemtype=%u,amount=%d", pUser->GetID(), m_pInfo->nGoods[i].itemID, nItemCount);
										nItemCount=0;
									}
								}
							}
							else
							{
								for (int j=0; j<m_pInfo->nGoods[i].itemAmount; j++)
								{
									pItem = pUser->AwardItem(m_pInfo->nGoods[i].itemID, true, false, true, true, m_pInfo->nGoods[i].itemAmount, false, true, false, 1);
									if (!pItem)
										::LogSave("商城购物给特价物品失败(3) user=%u,itemtype=%u,amount=%d", pUser->GetID(), m_pInfo->nGoods[i].itemID, m_pInfo->nGoods[i].itemAmount);
								}
							}
							pUser->SendSysMsg(_TXTATR_TOPSHOW, "花费%d珠宝购买了%d个[%s]", msg.m_pInfo->nGoods[i].dwData/10, m_pInfo->nGoods[i].itemAmount, pItemTypeData->GetStr(ITEMTYPEDATA_NAME));
 							if (pItem && pItem->IsCostlyItem())//goto 贵重物品记录
								::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from mart by %d jewel", pUser->GetID(), pUser->GetName(), m_pInfo->nGoods[i].itemAmount, m_pInfo->nGoods[i].itemID, msg.m_pInfo->nGoods[i].dwData/10);
							//设置玩家已经购买的特价商品记录
							if (pItem)
							{
								int nTemp = 1;
								for (int nCount = 0; nCount<ASALE_ITEM_AMOUNT; nCount++)
								{
									if (pUser->GetASales()/nTemp % 100 == 0)
									{
										pUser->SetASales(pUser->GetASales()+(pVasItem->id-10000)*nTemp);
										break;
									}
									nTemp *= 100;
								}
								VASItem()->SetASaleItemAmount(pVasItem->itemType, pVasItem->aSaleCount-m_pInfo->nGoods[i].itemAmount, true);
								CMsgMainServer msg2;//同步到其他线路
								msg2.Create(4,pVasItem->itemType,true);
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg2, (OBJID)0);
								pUser->SendASalesItem();
							}

						}
					}
				}
				CMsgUserAttrib	msg;
				if(msg.Create(m_pInfo->PlayerID, _USERATTRIB_MONEY4, pUser->GetMoneyByType(4)))
					pUser->SendMsg(&msg);
			}
		}
		break;
	case _MSGVAS_BUY_ASALE_FAIL:
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "元宝不足");
			return;
		}
		break;
	case _MSGVAS_SERVER_FAL:
		{
			pUser->SendSysMsg("服务操作失败");
		}
		break;
	default:
		{
			char strTemp[128];
			sprintf(strTemp,"CMsgVAS::Process::switch(m_pInfo->usAction) default...m_pInfo->usAction=%d,userid:%u,username:%s",m_pInfo->usAction,pUser->GetID(), pUser->GetName());
			::LogSave(strTemp);
		}
		break;
	}
	return ;
}
BOOL CMsgVAS::Create( int usVASPoint, OBJID AccountID, OBJID PlayerID, USHORT usAction, OBJID itemID, int itemAmount, DWORD dwData/*=0*/)
{
	this->Init();

	m_pInfo->usVASPoint = usVASPoint;
	m_pInfo->AccountID = AccountID;
	m_pInfo->PlayerID = PlayerID;
	m_pInfo->usAction = usAction;

	if(itemID == ID_NONE)
		m_pInfo->usGoodSize			= 0;
	else
	{
		m_pInfo->usGoodSize			= 1;
		m_pInfo->nGoods[0].itemID=itemID;
		m_pInfo->nGoods[0].itemAmount=itemAmount;
		m_pInfo->nGoods[0].dwData=dwData;
	}
	
	m_unMsgType	=_MSG_VAS;
	m_unMsgSize	=sizeof(MSG_Info);

	return TRUE;
}
BOOL CMsgVAS::Add(OBJID itemID, int itemAmount, DWORD dwData)
{
	if(itemID == 0)
		return FALSE;
	if(m_pInfo->usGoodSize != 0)
	{
		if(m_unMsgSize + sizeof(MsgTraffic) > MAX_PACKETSIZE)
			return FALSE;
		if(m_pInfo->usGoodSize >= 10)
			return FALSE;
		m_unMsgSize		+= sizeof(MsgTraffic);
	}
	
	// fill info now
	m_pInfo->nGoods[m_pInfo->usGoodSize].itemID=itemID;
	m_pInfo->nGoods[m_pInfo->usGoodSize].itemAmount=itemAmount;
	m_pInfo->nGoods[m_pInfo->usGoodSize].dwData=dwData;
	m_pInfo->usGoodSize++;

	return TRUE;
}
// BOOL CMsgVAS::Create( void* pInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID, int usAction )///发消息顺带说明消息的来历或类型
// {
// 	// init
// 	this->Init();
// 
// 	// fill info now
// 	MsgTraffic* pMsgTraffic=(MsgTraffic*)pInfo;
// 	m_pInfo->mtExtraInfo.usAction = usAction;
// 	m_pInfo->mtExtraInfo.itemID = pMsgTraffic->itemID;
// 	m_pInfo->mtExtraInfo.itemAmount = pMsgTraffic->itemAmount;
// 	m_pInfo->usVASPoint = usVASPoint;///增值点数
// 	m_pInfo->AccountID = AccountID;///帐号ID
// 	m_pInfo->PlayerID = PlayerID;///角色ID
// 
// 	//  [2007-9-22] 朱斌
// 	m_pInfo->mtExtraInfo.id = pMsgTraffic->id;
// 
// 	m_unMsgType	=_MSG_VAS;
// 	m_unMsgSize	=sizeof( MSG_Info );

// 	return true;
// }

// BOOL CMsgVAS::Create( void* pInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID, int usAction, int idx )///发消息顺带说明消息的来历或类型
// {
	// init
// 	this->Init();
// 
// 	// fill info now
// 	MsgTraffic* pMsgTraffic=(MsgTraffic*)pInfo;
// 	m_pInfo->mtExtraInfo.usAction = usAction;
// 	m_pInfo->mtExtraInfo.itemID = pMsgTraffic->itemID;
// 	m_pInfo->mtExtraInfo.itemAmount = idx;
// 	m_pInfo->usVASPoint = usVASPoint;///增值点数
// 	m_pInfo->AccountID = AccountID;///帐号ID
// 	m_pInfo->PlayerID = PlayerID;///角色ID
// 
// 	//  [2007-9-22] 朱斌
// 	m_pInfo->mtExtraInfo.id = pMsgTraffic->id;
// 
// 	m_unMsgType	=_MSG_VAS;
// 	m_unMsgSize	=sizeof( MSG_Info );

// 	return true;
// }

// BOOL CMsgVAS::Create( void* pInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID, int usAction, int nJuan,int nRealID  )///发消息顺带说明消息的来历或类型
// {
	// init
// 	this->Init();
// 	
// 	// fill info now
// 	MsgTraffic* pMsgTraffic=(MsgTraffic*)pInfo;
// 	m_pInfo->mtExtraInfo.usAction = usAction;
// 	m_pInfo->mtExtraInfo.itemID = pMsgTraffic->itemID;
// 	m_pInfo->usVASPoint = usVASPoint;///增值点数
// 	m_pInfo->AccountID = AccountID;///帐号ID
// 	m_pInfo->PlayerID = PlayerID;///角色ID
// 	m_pInfo->nJuan = nJuan;
// 	m_pInfo->nRealID = nRealID;
// 	
// 	//  [2007-9-22] 朱斌
// 	m_pInfo->mtExtraInfo.id = pMsgTraffic->id;
// 	
// 	m_unMsgType	=_MSG_VAS;
// 	m_unMsgSize	=sizeof( MSG_Info );
	
// 	return true;
// }


// BOOL CMsgVAS::Create(char* pInfo)
// {
// 	this->Init();
// 	MsgTraffic* pMsgTraffic = (MsgTraffic*)pInfo;
// 
// 	m_pInfo->mtExtraInfo.id = pMsgTraffic->id;
// 	m_pInfo->mtExtraInfo.itemID = pMsgTraffic->itemID;
// 	m_pInfo->mtExtraInfo.usAction = pMsgTraffic->usAction;
// 	m_pInfo->mtExtraInfo.itemAmount = pMsgTraffic->itemAmount;
// 
// 	m_pInfo->unMsgSize = sizeof( MSG_Info );
// 	m_pInfo->unMsgType = _MSG_VAS;
// 	
// 	m_unMsgSize	=sizeof( MSG_Info );
// 	m_unMsgType	=_MSG_VAS;

// 	return true;
// }
/*
void CMsgVAS::IsNewPlayerExam(CUser* pUser , int num)//  num试卷的套数 [9/22/2007 Administrator]
{
	CMsgVAS MsgVAS;
	m_pInfo->mtExtraInfo.usAction = num;
	MsgVAS.Create( ( char* )&m_pInfo->mtExtraInfo, 3, pUser->GetAccountID(), pUser->GetID(),10 );///发消息顺带说明消息的来历或类型
	MsgVAS.AppendInfo( m_idProcess,m_idSocket,ID_NONE );//借用AppendInfo函数，07.1.20修改
	//给世界线程发消息
	MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
}
*/

//  [2007-9-22] 朱斌	时间戳
void CMsgVAS::AppendPresendTickCount(DWORD dwTick)
{
//	m_pInfo->mtExtraInfo.id = dwTick;
}
