///////////////////////////////////////////////////////////
//
//		CMsgPackage.h
//

#ifndef		MSGPACKAGE_H
#define		MSGPACKAGE_H

#include "NetMsg.h"

enum{
		MSGPACKAGE_QUERYLIST			= 0,
		MSGPACKAGE_CHECKIN				= 1,
		MSGPACKAGE_CHECKOUT				= 2,
		MSGPACKAGE_QUERYLIST2			= 3,	// 客户端收到此消息表示追加仓库物品，不清空原来的
		MSGPACKAGE_SYNCHRO_AMOUNT       = 4,    //同步仓库中物品的数量
		MSGPACKAGE_CHECKIN_MONEY        = 5,    //存钱
		MSGPACKAGE_CHECKOUT_MONEY       = 6,    //取钱
		MSGPACKAGE_MONEY_AMOUNT         = 7,    //发送存款金额
		MSGPACKAGE_MOVE		            = 8,    //单个物品移动位置
		MSGPACKAGE_EXCHANGE	            = 9,    //两个物品交换位置或合并 toserver
		MSGPACKAGE_CHECKIN_EUDEMON		= 10,	//宠物存到仓库
		MSGPACKAGE_CHECKOUT_EUDEMON		= 11,	//从仓库取出宠物
};

enum{	// type
		MSGPACKAGE_TYPE_NONE			= 0,
		MSGPACKAGE_TYPE_STORAGE			= 10,
		MSGPACKAGE_TYPE_TRUNK			= 20,
		MSGPACKAGE_TYPE_CHEST			= 30,
		MSGPACKAGE_TYPE_EUDEMON_BROODER	= 40,	// [改用在MsgEudPackage里]
		MSGPACKAGE_TYPE_EUDEMON_STORAGE	= 50,	// [改用在MsgEudPackage里]
		MSGPACKAGE_TYPE_AUCTION_STORAGE = 60,	// 拍买所得到的物品LW
		MSGPACKAGE_TYPE_EUD_BROOD_PACKAGE = 70,	// 玩家身上的幻兽孵化器 // [改用在MsgEudPackage里]
		PACKAGE_MAGICSTONEBOX	 = 80,	// 魔晶盒
		MSGPACKAGE_TYPE_MARRYSTORAGE = 90,  //爱心家园仓库
		MSGPACKAGE_TYPE_BED_ADDMATERIAL1 = 100, //床--添加材料1
		MSGPACKAGE_TYPE_BED_ADDMATERIAL2 = 110, //床--添加材料2
		MSGPACKAGE_TYPE_BED_ADDMATERIAL3 = 120, //床--添加材料3

		MSGPACKAGE_TYPE_DEPORT_ADDMATERIAL1 = 130, //柜子--添加材料1
		MSGPACKAGE_TYPE_DEPORT_ADDMATERIAL2 = 140, //柜子--添加材料2
		MSGPACKAGE_TYPE_DEPORT_ADDMATERIAL3 = 150, //柜子--添加材料3

		MSGPACKAGE_TYPE_FLOWER_ADDMATERIAL1 = 160, //花盆--添加材料1
		MSGPACKAGE_TYPE_FLOWER_ADDMATERIAL2 = 170, //花盆--添加材料2
		MSGPACKAGE_TYPE_FLOWER_ADDMATERIAL3 = 180, //花盆--添加材料3

		MSGPACKAGE_TYPE_CABINET_ADDMATERIAL1 = 190, //厨房--添加材料1
		MSGPACKAGE_TYPE_CABINET_ADDMATERIAL2 = 200, //厨房--添加材料2
		MSGPACKAGE_TYPE_CABINET_ADDMATERIAL3 = 210, //厨房--添加材料3
};


class CMsgPackage : public CNetMsg
{
public:
	CMsgPackage();
	virtual ~CMsgPackage();

public:		//接口函数
	// modified by zlong 2003-11-24 ---- Create函数添加ucType参数指定类别
// 	BOOL			Create(int nAction, OBJID id, const MsgPackageItemInfo* buf, int nSize, UCHAR ucType);
// 	BOOL			Create(int nAction, OBJID id, OBJID idItem, UCHAR ucType);
	BOOL			Create(int nAction, OBJID id, OBJID idType, UINT usAmount, UINT usMaxAmount, int nIndex, int nPosition=MSGPACKAGE_TYPE_STORAGE, int isBind=0);//[2010-06-11 goto]
public:
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(CGameSocket* pSocket);

private:
// 	typedef struct
// 	{
// 		MSGHEAD_DEFINE
// 
// 		OBJID		id;						// userid, npcid, trunkid
// 		UCHAR		ucAction;
// 		UCHAR		ucType;
// 		union {
// 			struct{							// to client: size
// 				USHORT		usSize;	
// 				MsgPackageItemInfo	setItemInfo[1];
// 			};
// 			OBJID		idItem;			// to client/server: action
// 		};
// 	}MSG_Info;
	typedef struct 
	{
		MSGHEAD_DEFINE

		OBJID	id;			//物品ID
		OBJID	idType;		//物品type
		UINT	usAmount;	//数量
		UINT	usMaxAmount;//数量上限
		UCHAR	ucPosition;	//位置
		UCHAR	ucIsBind;	//是否绑定
		USHORT	usIndex;	//仓库内的位置(页码)
		UCHAR	ucAction;	//功能
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // MSGPACKAGE_H