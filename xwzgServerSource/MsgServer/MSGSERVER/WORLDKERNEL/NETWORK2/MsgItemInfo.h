// MsgItemInfo.h: interface for the CMsgItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEMINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGITEMINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"
//#include "WeaponSoulData.h"//20071113修罗:


enum {	ITEMINFO_NONE=0, 
		ITEMINFO_ADDITEM,				// 添加物品
		ITEMINFO_TRADE,					// 公平交易对方添加的物品
		ITEMINFO_UPDATE,				// update item info
		ITEMINFO_OTHERPLAYER_EQUIPMENT, // 返回其他玩家装备// id is user id
		ITEMINFO_AUCTION,               // 拍买查看物品
		ITEMINFO_MAILITEMINFO,			// 邮件中道具详细信息
	    ITEMACT_SAVEITEM,                //道具挽救
		ITEMACT_VIEWCHATITEMINFO,		 //[独孤求败 2008/12/3]聊天窗口显示的物品信息
		ITEMACT_SENDSAVEITEM_END,       //发送赎回物品结束
};

class CItem;
class CMsgItemInfo : public world_kernel::CNetMsg  
{
public:
	CMsgItemInfo();
	virtual ~CMsgItemInfo();

	BOOL	Create		(CItem* pItem, int nAction = ITEMINFO_ADDITEM,OBJID idUser=ID_NONE);	
//	BOOL    Create      (int nAction, OBJID idUser);
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);
 
protected:
	typedef struct {
		MSGHEAD_DEFINE
		OBJID		id;
		DWORD		dwType;
		DWORD		dwAmount;
		DWORD 		dwAmountLimit;
		USHORT		usLevel;
		USHORT		usActiveType;
		DWORD		dwLifeTime;
		USHORT		usQuality;
		USHORT		usStar;
		USHORT		usKind;
		USHORT		usColor;
		DWORD		dwGemType[4];
		DWORD		dwBasic[2];
		DWORD		dwAddtion[8];
		USHORT		usEquipSoulLev;
		USHORT		usEquipSoulType;
		INT			nEquipSoulPoint;
		char		szBuf[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGITEMINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
