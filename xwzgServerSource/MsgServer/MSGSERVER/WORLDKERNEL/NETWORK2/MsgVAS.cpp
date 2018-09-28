// MsgVAS.cpp: implementation of the CMsgVAS class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "WorldKernel.h"
using namespace world_kernel;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgVAS::CMsgVAS()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
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
// 	m_pInfo->mtExtraInfo.itemID = pMsgTraffic->itemID;
// 	m_pInfo->mtExtraInfo.itemAmount = pMsgTraffic->itemAmount;
// 	m_pInfo->usVASPoint = usVASPoint;///增值点数
// 	m_pInfo->AccountID = AccountID;///帐号ID
// 	m_pInfo->PlayerID = PlayerID;///角色ID
// 
// 	m_pInfo->unMsgSize = sizeof( MSG_Info );
// 	m_pInfo->unMsgType = _MSG_VAS;
// 	
// 	m_unMsgSize	=sizeof( MSG_Info );
// 	m_unMsgType	=_MSG_VAS;

// 	return true;
// }

// BOOL CMsgVAS::Create( void* pInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID, int usAction )///发消息顺带说明消息的来历或类型
// {
// 	// init
// 	this->Init();
// 
// 	// fill info now
// 	MsgTraffic* pMsgTraffic=(MsgTraffic*)pInfo;
// 	//  [2007-9-22] 朱斌	使用账号服务器传来的值
// //	m_pInfo->mtExtraInfo.id = 0;	
// 	m_pInfo->mtExtraInfo.id = pMsgTraffic->id;
// 	m_pInfo->mtExtraInfo.usAction = usAction;
// 	m_pInfo->mtExtraInfo.itemID = pMsgTraffic->itemID;
// 	m_pInfo->mtExtraInfo.itemAmount = pMsgTraffic->itemAmount;
// 	m_pInfo->usVASPoint = usVASPoint;///增值点数
// 	m_pInfo->AccountID = AccountID;///帐号ID
// 	m_pInfo->PlayerID = PlayerID;///角色ID
// 
// 	m_pInfo->unMsgSize = sizeof( MSG_Info );
// 	m_pInfo->unMsgType = _MSG_VAS;
// 	
// 	m_unMsgSize	=sizeof( MSG_Info );
// 	m_unMsgType	=_MSG_VAS;

// 	return true;
//  }
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
	
// 	switch( m_pInfo->usAction )
// 	{
// 	case _MSGVAS_BUY_ITEM:
// 		{
// 			CShopVAS* pVASShop = GameWorld()->QueryShopVAS();
//  			if(!pVASShop)
//  				return;
// 			int nCostVAS = 0;
// 			for (int i=0; i<m_pInfo->usGoodSize; i++)
// 			{
// 				vasItemInfo* pVasItem = pVASShop->GetItemByType(m_pInfo->nGoods[i].itemID);
// 				if (!pVasItem)
// 					return;
// 
// 				int nPriceTotal = 0;//物品价格
// 				if (pVasItem->discount==0)//没有折扣
// 					nPriceTotal = (pVasItem->price*m_pInfo->nGoods[i].itemAmount);
// 				else//有折扣
// 					nPriceTotal = (pVasItem->price*m_pInfo->nGoods[i].itemAmount)*(pVasItem->discount/100.0);
// 
// 				if (pVasItem->payType==0)//元宝商品，只能用元宝或珠宝支付
// 				{
// 					if (m_pInfo->nGoods[i].dwData==_PAY_TYPE_VAS)//元宝支付
// 						nCostVAS += nPriceTotal;
// 					else if (m_pInfo->nGoods[i].dwData==_PAY_TYPE_GEM)//珠宝支付
// 						m_pInfo->nGoods[i].dwData += (nPriceTotal*10+m_pInfo->nGoods[i].dwData);//价格的个位是支付类型
// 					else//支付方式不对
// 						return;
// 				}
// 				else if(pVasItem->payType==1)//点券商品，只能用点券支付
// 				{
// 					if (m_pInfo->nGoods[i].dwData!=_PAY_TYPE_POINT)//支付方式不对
// 						return;
// 
// 					m_pInfo->nGoods[i].dwData += (nPriceTotal*10+m_pInfo->nGoods[i].dwData);//价格的个位是支付类型
// 				}
// 			}
// 			m_pInfo->usVASPoint = nCostVAS;
// 		}
// 		break;
// 	case 0:   //交易 寻找物品和价值
// 		{
// 			CShopVAS* pVASShop = GameWorld()->QueryShopVAS();
// 			if(!pVASShop)
// 				return;
// 			
// 			int nPriRate = pVASShop->GetPriRate();
// 
// 			vasItemInfo info;
// 			for(int i = 0 ; i < pVASShop->GetShopAmount(); i ++)
// 			{
// 				info = pVASShop->GetItemByIndex(i); 
// 				if(info.id == m_pInfo->nRealID)
// 				{
// 					if(info.index1 == 6)
// 						m_pInfo->nJuan = 1;
// 					else
// 						m_pInfo->nJuan = 0;
// 					m_pInfo->usVASPoint = info.pri  * nPriRate * m_pInfo->mtExtraInfo.itemAmount / 100;
// 					m_pInfo->mtExtraInfo.itemID = info.type;
// 					break;
// 				}
// 			}
// 			if(i == pVASShop->GetShopAmount())
// 				return;
// 		}
// 		break;
//	}
// 	
// 	GameWorld()->SendVASRequest( m_pInfo->mtExtraInfo.id, 
// 		m_pInfo->mtExtraInfo.itemAmount, 
// 		m_pInfo->mtExtraInfo.itemID, 
// 		m_pInfo->mtExtraInfo.dwData, 
// 		m_pInfo->mtExtraInfo.usAction, 
// 		m_pInfo->AccountID, 
// 		m_pInfo->PlayerID, 
// 		m_pInfo->usVASPoint, 
// 		m_unMsgType, 
// 		m_unMsgSize, 
// 		m_pInfo->nJuan,
// 		m_pInfo->nRealID);
	GameWorld()->SendVASRequest(m_unMsgType, m_unMsgSize, m_pInfo->usVASPoint, m_pInfo->AccountID, m_pInfo->PlayerID, m_pInfo->usAction, m_pInfo->usGoodSize, m_pInfo->nGoods);
}

