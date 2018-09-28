// MsgVAS.h: interface for the CMsgVAS class.
//	usAction值说明：1表示购买成功，2表示购买失败，3表示余额查询，100表示消息的来源是世界线程
//  10表示地推号状态查询，11表示领1级新手装备，12表示领20级装备，13表示领40级装备。
//	1000表示地推号状态查询返回
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGVAS_H__A4FB6DD4_D125_4D95_9F92_61A2E0E294B9__INCLUDED_)
#define AFX_MSGVAS_H__A4FB6DD4_D125_4D95_9F92_61A2E0E294B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

WORLDKERNEL_BEGIN
//[2010-07-27 goto]商品的支付方式
const int _PAY_TYPE_VAS		=	1;//元宝
const int _PAY_TYPE_GEM		=	2;//珠宝
const int _PAY_TYPE_POINT	=	3;//点券
enum
{
	_MSGVAS_QUERY_BALANCE	=	1001,//查询余额 C-S
	_MSGVAS_ADD_VAS			=	1002,//请求加黄金 MS-AS
	_MSGVAS_BUY_ITEM		=	1003,//购买商城物品 C-S	
	_MSGVAS_SPEND_VAS		=	1004,//减少黄金 MS-AS
	_MSGVAS_QUERY_CARD		=	1005,//查询卡 C-S
	_MSGVAS_OPRATE_CARD		=	1006,//操作卡 C-S
	_MSGVAS_POINT_BUY		=	1007,//点券购买商品
	_MSGVAS_QUERY_ACTIVITY	=	1008,//查询活动奖励状态
	_MSGVAS_OPRATE_ACTIVITY	=	1009,//操作活动奖励状态
	_MSGVAS_BUY_LOTTERY		=	1010,//买彩票,开箱子
	_MSGVAS_BUY_ASALE		=	1011,//买特价商品
	_MSGVAS_QUERY_MOBCHARGE =   1012,//查询手机充值礼包
	_MSGVAS_GET_MOBCHARGE   =   1013,//获得手机充值礼包
	_MSGVAS_ADD_VAS_CPST	=	1014,
	
	
	_MSGVAS_RETURN_BALANCE	=	2002,//账号服务器返回余额
	_MSGVAS_BUY_SUC			=	2004,//购买成功
	_MSGVAS_BUY_FAIL		=	2005,//购买失败
	_MSGVAS_ADD_VAS_SUC		=	2006,//请求加黄金 AS-MS
	_MSGVAS_ADD_VAS_FAIL	=	2007,//请求加黄金 AS-MS
	_MSGVAS_SPEND_VAS_SUC	=	2008,//减少黄金 AS-MS
	_MSGVAS_SPEND_VAS_FAIL	=	2009,//减少黄金 AS-MS
	_MSGVAS_CARD_STATES     =   2010,//返回卡状态
	_MSGVAS_POINT_BUY_SUC	=	2011,//点券购买商品成功
	_MSGVAS_POINT_BUY_FAIL	=	2012,//点券购买商品失败
	_MSGVAS_ACTIVITY_STATES	=	2013,//返回活动奖励状态
	_MSGVAS_BUY_LOTTERY_SUC	=	2014,//买彩票成功
	_MSGVAS_BUY_LOTTERY_FAIL=	2015,//买彩票失败
	_MSGVAS_BUY_ASALE_SUC	=	2016,//买特价商品成功
	_MSGVAS_BUY_ASALE_FAIL	=	2017,//买特价商品失败
	_MSGVAS_MOBCHARGE_STATE   = 2018,
	_MSGVAS_ADD_VAS_FAIL_CPST = 2019,
	_MSGVAS_ADD_VAS_SUC_CPST = 2020,

};
///增值服务消息
class CMsgVAS : public CNetMsg  
{
public:
	CMsgVAS();
	virtual ~CMsgVAS();

// 	typedef struct tag_MsgTraffic
// 	{
// //		MSGHEAD_DEFINE
// 		OBJID		id;
// 		USHORT		usAction;
// 		OBJID		itemID;//20060915修罗
// 		int			itemAmount;//20060915修罗
// 		DWORD		dwData;	//  [2/16/2008 朱斌]	附加属性
	// 	}MsgTraffic;
	//[2010-07-26 goto]
	typedef struct tag_MsgTraffic
	{
		OBJID		itemID;//20060915修罗
		int			itemAmount;//20060915修罗
		DWORD		dwData;	//  [2/16/2008 朱斌]	附加属性 支持方式:元宝、珠宝、点券
	}MsgTraffic;
		
// 	BOOL Create( void* pInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID, int usAction );///发消息顺带说明消息的来历或类型
// 	BOOL Create( void* pInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID );///生成增值服务请求消息
	BOOL Create( int usVASPoint, OBJID AccountID, OBJID PlayerID, USHORT usAction, OBJID itemID=ID_NONE, int itemAmount=0, DWORD dwData=0);
	BOOL Add(OBJID itemID, int itemAmount, DWORD dwData);
public:	
	BOOL Create(char* pMsgBuf,DWORD dwSize);
	void Process(void *pInfo);
//	void			Process		(CGameSocket* pSocket);



//private:


// 	typedef struct tag_MSG_Info
// 	{
// 		unsigned short	unMsgSize;
// 		unsigned short	unMsgType;
// 		MsgTraffic		mtExtraInfo;///附加交易包信息
// 		int				usVASPoint;///增值点数
// 		OBJID			AccountID;///帐号ID
// 		OBJID			PlayerID;///角色ID
// 		int				nJuan;///赠劵
// 		int				nRealID;///赠劵
// 		char			szRegard[1];	//  [2007-9-21] 朱斌	赠送语
	// 	}MSG_Info;
	//[2010-07-26 goto]
	typedef struct tag_MSG_Info
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		
		ULONG			usVASPoint;///增值点数
		OBJID			AccountID;///帐号ID
		OBJID			PlayerID;///角色ID
		
		USHORT			usAction;
		USHORT			usGoodSize;//购物车内物品数量
		MsgTraffic		nGoods[1];//商品
	}MSG_Info;
	MSG_Info*	m_pInfo;
};
WORLDKERNEL_END
#endif // !defined(AFX_MSGVAS_H__A4FB6DD4_D125_4D95_9F92_61A2E0E294B9__INCLUDED_)
