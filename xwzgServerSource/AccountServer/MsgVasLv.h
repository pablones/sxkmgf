//////////////////////////////////////////////////////////////////////

#ifndef _MSGVASLV_H_
#define _MSGVASLV_H_

#include "NetMsg.h"

enum {	ACTION_NONE				=0, 
		ACTION_QUERYVASLVTOSER,		//客户端查询充值元宝等级数值
		ACTION_QUERYVASLVTOACC,	 	//msg查询充值元宝等级数值
		ACTION_QUERYVASLVTOMSG,		//account返回充值元宝等级数值
		ACTION_QUERYVASLVTOCLI,		//msg返回给客户端充值元宝等级数值
		ACTION_GETITEM			=5,	//客户端获取元宝等级奖励
		ACTION_UNEQU1,				//创建一条wb_vaslv记录
		ACTION_LOGINUPDATE,			//一个登录更新元宝等级信息
		ACTION_UNEQU3,				//更新掩码最高游历次数信息
		ACTION_UNEQU4,				//刷新所有角色元宝等级信息
		ACTION_QUERYVISITTOSER	=10,//客户端查询游历数值
		ACTION_QUERYVISITTOCLI,		//msg返回给客户端游历数值
		ACTION_REDUCEVAS,			//扣元宝
		ACTION_UNEQU6,				//暂保留
		ACTION_REDUCEVASUPDATE,		//扣元宝后返回
		ACTION_VISITFREETOSER	=15,//客户端提交游历一次
		ACTION_VISITFREETOCLI,		//msg返回给客户端游历信息
		ACTION_VISITVASTOSER,		//客户端提交神游千里一次
		ACTION_VISITVASTOCLI,		//msg返回给客户端神游千里信息
		ACTION_UNEQU8,				//暂保留
		ACTION_SENDANSWER		=20,//客户端提交问题答案
};
#pragma pack(1)
class CMsgVasLv : public CNetMsg
{
public:
	CMsgVasLv();
	virtual ~CMsgVasLv();

	BOOL	Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan);
	BOOL	Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem);
public:
	BOOL	Create(char* pMsgBuf, DWORD dwSize);

//private:
	typedef struct {
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		unsigned short	usAction;//
		OBJID			id;//角色id
		DWORD			vastotal;//元宝总数
		DWORD			lvtotal;//显示条元宝总数
		DWORD			dwcount;//还需充值元宝数
		DWORD			vaslv;//元宝VIP等级
		DWORD			awarditem;//奖励掩码

		DWORD			viscou;//每日游历次数
		DWORD			maxvis;//每日最高游历次数
		DWORD			vascount;//神游千里所需元宝数

		OBJID			questionid;//问题集id
		DWORD			questionan;//问题的答案
	}MSG_Info;

	MSG_Info*	m_pInfo;
};
#pragma pack(pop)
#endif
