// MsgTrade.h: interface for the CMsgTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTRADE_H__F3900087_7C9F_4D0D_906A_10E4EB5B5E40__INCLUDED_)
#define AFX_MSGTRADE_H__F3900087_7C9F_4D0D_906A_10E4EB5B5E40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

const unsigned short _TRADE_APPLY			= 1;		// 请求交易		to client/server
const unsigned short _TRADE_QUIT			= 2;		// 取消交易		to server
const unsigned short _TRADE_OPEN			= 3;		// 打开交易窗	to client
const unsigned short _TRADE_SUCCESS			= 4;		// 交易成功(同时关闭)		to client
const unsigned short _TRADE_FALSE			= 5;		// 交易失败(同时关闭)		to client
const unsigned short _TRADE_ADDITEM			= 6;		// 加物品		to client/server(对方收到MSGITEMINFO消息)
const unsigned short _TRADE_ADDMONEY		= 7;		// 加钱			to server
const unsigned short _TRADE_MONEYALL		= 8;		// 对方钱总数	to client(对方收到金钱总数)
const unsigned short _TRADE_SELFMONEYALL	= 9;		// 自己钱总钱	to client(金钱总数)
const unsigned short _TRADE_OK				= 10;		// 同意交易		to client/server
const unsigned short _TRADE_ADDITEMFAIL		= 11;		// 加物品失败	to client
const unsigned short _TRADE_NOTALLOW		= 12;		//自己不允许交易to server
const unsigned short _TRADE_BACK_WU		    = 13;		//减物品		to client/server
const unsigned short _TRADE_BACK_MONEY		= 14;
const unsigned short _TRADE_LOCK    		= 15;
const unsigned short _TRADE_ADDVAS	    	= 16;       //加vas
const unsigned short _TRADE_VASOTHER		= 17;		// 对方vas
const unsigned short _TRADE_VASSELF     	= 18;		// 自己vas
const unsigned short _TRADE_REFUSE      	= 19;		//[2010-06-01 goto] 拒绝交易	to client/server
const unsigned short _TRADE_DELITEMFAIL		= 20;		//[2010-06-01 goto] 减物品失败	to client
const unsigned short _TRADE_ADDITEMOP		= 21;		//[2010-06-01 goto] 对方增加物品	to client
const unsigned short _TRADE_BACK_WUOP		= 22;		//[2010-06-01 goto] 对方减少物品	to client
const unsigned short _TRADE_UNLOCK			= 23;		//[2010-06-16 goto] 自己解除锁定	to client
const unsigned short _TRADE_UNLOCK_OP		= 24;		//[2010-06-16 goto] 对方解除锁定	to client
const unsigned short _TRADE_TARGETTRADEING	= 25;		//[2010-06-22 goto] 对方正在交易中	to client

const unsigned short _TRADE_ADDMONSTER		= 26;		//[2010-07-30 goto] 增加宠物		to server
const unsigned short _TEADE_ADDMONSTER_OP	= 27;		//[2010-07-30 goto] 对方增加宠物	to client //未使用，直接给客户端发宠物快照
const unsigned short _TRADE_ADDMONSTERSUC	= 28;		//[2010-07-30 goto] 加宠物成功		to client
const unsigned short _TRADE_ADDMONSTERFAIL	= 29;		//[2010-07-30 goto] 加宠物失败		to client
const unsigned short _TRADE_DELMONSTER		= 30;		//[2010-07-30 goto] 减少宠物		to server
const unsigned short _TRADE_DELMONSTERSUC	= 31;		//[2010-07-30 goto] 减少宠物成功	to client
const unsigned short _TRADE_DELMONSTERFAIL	= 32;		//[2010-07-30 goto] 减少宠物失败	to client
const unsigned short _TRADE_DELMONSTER_OP	= 33;		//[2010-07-30 goto] 对方减少宠物	to client

const unsigned short _TEADE_ACCEPT_APPLY	= 34;		//[2010-08-26 goto] 同意交易请求,为解决两个玩家同时互相请求交易出现的BUG添加此action

class CMsgTrade : public CNetMsg   
{
public:
	CMsgTrade();
	virtual ~CMsgTrade();

public:
	BOOL	Create		(unsigned short usAction, OBJID id, int nType = 0);

	BOOL	Create		(char* pbufMsg, DWORD dwMsgSize);
	BOOL	Create		(CItem* pItem, unsigned short nAction=_TRADE_ADDITEM);//[2010-05-31 goto] 返回交易物品信息
	void	Process		(CGameSocket* pSocket);

private:
	typedef struct 
	{
		MSGHEAD_DEFINE

		OBJID			id;
		DWORD			nData;
		OBJID           idType;
		
		UINT type;
		unsigned short	usAction;
	}MSG_Info;

	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTRADE_H__F3900087_7C9F_4D0D_906A_10E4EB5B5E40__INCLUDED_)
