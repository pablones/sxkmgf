// MsgItem.h: interface for the CMsgItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_)
#define AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

WORLDKERNEL_BEGIN
enum {	ITEMACT_NONE			=0, 
		ITEMACT_BUY,					// to server, id: idNpc, data: idItemType
		ITEMACT_SELL,	 				// to server, id: idNpc, data: idItem
		ITEMACT_DROP,					// to server, x, y
		ITEMACT_USE,					// to server, data: position
		ITEMACT_EQUIP			=5,	    // to client
		ITEMACT_UNEQUIP,				// to server, data: position
		ITEMACT_REQUEST_SPLITITEM,				// to server, data: num
		ITEMACT_REQUEST_COMBINEITEM,			// to server, data: id
		ITEMACT_QUERYMONEYSAVED,		// to server/client, id: idNpc, data: nMoney
		ITEMACT_SAVEMONEY		=10,	// to server, id: idNpc, data: nMoney
		ITEMACT_DRAWMONEY,				// to server, id: idNpc, data: nMoney
		ITEMACT_DROPMONEY,	 			// to server, x, y
		ITEMACT_SPENDMONEY,				// 无意义，暂保留
		ITEMACT_REPAIR,					// to server, return MsgItemInfo
		ITEMACT_REPAIRALL		=15,	// to server, return ITEMACT_REPAIRAll, or not return if no money
		ITEMACT_IDENT,					// to server, return MsgItemInfo
		ITEMACT_DURABILITY,				// to client, update durability
		ITEMACT_DROPEQUIPMENT,			// to client, delete equipment
		ITEMACT_IMPROVE,				// to server, improve equipment
		ITEMACT_UPLEV			=20,	// to server, upleve equipment
		ITEMACT_BOOTH_QUERY,			// to server, open booth, data is npc id
		ITEMACT_BOOTH_ADD,				// to server/client(broadcast in table), id is idItem, data is money
		ITEMACT_BOOTH_DEL,				// to server/client(broadcast), id is idItem, data is npc id
		ITEMACT_BOOTH_BUY,				// to server, id is idItem, data is npc id
		ITEMACT_SYNCHRO_AMOUNT	=25,	// to client(data is amount)
		ITEMACT_FIREWORKS,				
		
		ITEMACT_COMPLETE_TASK,			// to server, 完成佣兵任务, id=任务ID, dwData=提交的物品，如果没有则为ID_NONE
		ITEMACT_EUDEMON_EVOLVE,			// to server, 幻兽进化, id=幻兽物品ID，dwData=进化成的类型，取值范围1~2
		ITEMACT_EUDEMON_REBORN,			// to server, 幻兽复活，id=幻兽物品ID, data=宝石ID
		ITEMACT_EUDEMON_ENHANCE	=30,	// to server, 幻兽强化，id=幻兽物品ID, data=宝石ID
		ITEMACT_CALL_EUDEMON,			// to server, 召唤幻兽，id=幻兽物品ID
		ITEMACT_KILL_EUDEMON,			// to server, 收回幻兽，id=幻兽物品ID
										// to client, 收回幻兽的光效表现，id=幻兽物品ID, dwData=幻兽ID
		ITEMACT_EUDEMON_EVOLVE2,		// to server, 幻兽二次进化, id=幻兽物品ID，dwData=进化成的类型，取值范围1~8
		ITEMACT_EUDEMON_ATKMODE,		// to server, 设置幻兽攻击模式, id=幻兽物品ID，dwData=攻击模式，取值范围1~3
		ITEMACT_ATTACH_EUDEMON	=35,	// 幻兽与人合体, id=幻兽物品ID
		ITEMACT_DETACH_EUDEMON,			// 解除幻兽与人合体状态, id=幻兽物品ID
		ITEMACT_EUDEMON_EVO_CHECKIN,	// 幻兽送入进化npc, to server,client id=NPCid dwData=幻兽物品id
		ITEMACT_EUDEMON_EVO_CHECKOUT,	// 从进化NPC处取出, to server id=NPCid dwData=幻兽物品id	
		ITEMACT_EXCHANGEPOSOK	=39,	//	06.10.30王玉波修改,to client,改变物品在不同背包中的位置,成功.
		ITEMACT_SAVEDIAMOND		= 40,			//存钻石
		ITEMACT_DRAWDIAMOND		= 41,			//取钻石
//		ITEMACT_ICON_DROP_IN_SAME_PACKAGE = 42,	//20060921修罗:同一背包iconDrop.
		ITEMACT_REQUEST_TRADEPACKIDX	= 43,		//06.10.31王玉波添加,to server,请求互换两个物品的位置(暂只支持同一背包中操作)
		ITEMACT_REQUEST_CHGPACKIDX		= 44,		//06.10.28王玉波添加,to server,请求改变物品的package_index属性,在同一背包中
		ITEMACT_SYNCHRO_PACKAGEINDEX	= 45,		//06.10.28王玉波添加,to client( data is packageIndex )
		ITEMACT_REQUEST_CHGPOSANDIDX	= 46,		//06.10.30王玉波添加,to server,请求改变物品的position and package_index
		ITEMACT_SYNCHRO_UNLOCK			= 47,		//06.10.30王玉波添加,to client,出错同步客户端,解锁物品 
		ITEMACT_SYNTHESIS_REQUEST		= 48,		//20061103修罗.C2S合成请求
		ITEMACT_SYNTHESIS_SUCCESS		= 49,		//20061103修罗.S2C合成成功
		ITEMACT_SYNTHESIS_FAIL			= 50,		//20061103修罗.S2C合成失败
		ITEMACT_STRENGTHAN				= 51,		//06.12.26王玉波添加,to server,请求装备强化
		ITEMACT_UPDATE_CALCULATED_PROPERTY = 52,	//20070110修罗.S2C.更新客户端hero属性.客户端单独计算.
		ITEMACT_TEAMPICKUP				=53,			//softworms,组队拾取物品
		ITEMACT_SYNCHRO_AMOUNTLIMIT	        =54,    //  [4/29/2007]陈剑飞添加 to client(data is amountLIMIT)
		ITEMACT_SYNCHRO_GEM		        =55, //2007828修罗:S2C同步附加属性.
		ITEMACT_SYNCHRO_UNENCHANT		=56, //2007828修罗:S2C清除附魔.
		ITEMACT_USE_OK					=57, //[游途道标 2008/10/24]物品使用成功

		ITEMACT_SAVEITEMMONEYGET        =59, //获得玩家道具,用钱赎回
		ITEMACT_SAVEITEMWSAGET          =60, //用增值道具赎回
		ITEMACT_SAVEITEMOK              =61, //获得挽救道具成功
		ITEMACT_BOOTHBUY_ADD			=62,	//[游途道标 2008/12/19]摆摊收购物品
		ITEMACT_BOOTH_BUYBUY			=63,//[游途道标 2008/12/24]收购物品
		ITEMACT_BOOTH_DELBUY			=64,//[游途道标 2008/12/24]删除收购物品
		ITEMACT_VIEWITEMINFO			=65, //[独孤求败 2008/12/03]查看聊天窗口中物品信息
		ITEMACT_VIEWITEMAMOUNT          =66, //查看添加物品的材料
		ITEMACT_REQUEST_TRADEPOSANDIDX  =67, //[2009.3.26--张冬]跨包交换物品
		ITEMACT_SAVEITEMGIVEUP          =68, //放弃赎回道具
		ITEMACT_PET_EXCHANGE            =69, //宠物交换

		
		ITEMACT_ADD						=100, 
		ITEMACT_GETINFO					=101,
		ITEMACT_ORDER					=102,
		ITEMACT_BOOTH_CHGMONEY          =103,
		ITEMACT_BOOTH_CLRLEAVEWORD      =104,
		ITEMACT_GETEUDEMONINFO			=105,
		ITEMACT_GETEUDEMONLIST			=106,

		
		ITEMACT_BOOTH_ADDEUDE			=107,
		ITEMACT_BOOTH_DELEUDE			=108,
		ITEMACT_BOOTH_BUYEUDE			=109,
		ITEMACT_BOOTH_CHGEUDE			=110,
		ITEMACT_QUERYEUDEMONINFO		=111,///跨服查询宠物信息
		ITEMACT_QUERYITEMINFO			=112,///跨服查询道具信息

};

class CMsgItem : public world_kernel::CNetMsg  
{
public:
	CMsgItem();
	virtual ~CMsgItem();

	BOOL	Create		(USHORT nAction,DWORD dwData, OBJID idTarget=ID_NONE,USHORT usPosX = 0,USHORT usPosY = 0, OBJID id = ID_NONE, OBJID	typeId = ID_NONE, DWORD	nAmount = 1, DWORD	nMaxAmount = 1, UCHAR	ucPosition = 0, UCHAR	ucBind = 0, USHORT	usIndex = 0,DWORD dwMoney = 0);
	BOOL	Add			(OBJID id,OBJID typeId,DWORD	nAmount,DWORD nMaxAmount, UCHAR ucPosition, UCHAR ucBind, USHORT	usIndex,DWORD dwMoney = 0);
public:	
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

//protected:
	struct itemMiniInfo
	{
		OBJID	id;
		OBJID	typeId;
		DWORD	nAmount;
		DWORD	nMaxAmount;
		UCHAR	ucPosition;
		UCHAR	ucBind;
		USHORT	usIndex;
		DWORD	dwMoney;
	};
	typedef struct {
		MSGHEAD_DEFINE
		USHORT		usAction;
		USHORT		usAmount;
		DWORD		dwData;
		OBJID		idTarget;
		USHORT		usPosX;
		USHORT		usPosY;
		char        szTargetName[16];
		itemMiniInfo m_itemInfo[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};
WORLDKERNEL_END
#endif // !defined(AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_)
