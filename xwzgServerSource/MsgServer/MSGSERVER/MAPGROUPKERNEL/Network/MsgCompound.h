// MsgCompound.h: interface for the CMsgCompound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCOMPOUND_H__9FE3B9EF_9DD9_4F14_902B_8E7003F5B884__INCLUDED_)
#define AFX_MSGCOMPOUND_H__9FE3B9EF_9DD9_4F14_902B_8E7003F5B884__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"

const int COMP_CREAT = 8001;
const int COMP_STRENG = 8002;
const int COMP_MAGIC = 8003;
const int COMP_DICK = 8004;
const int COMP_GEN = 8005;
const int COMP_SUC_LEV1 = 20;//1级强化宝石强化成功率 20%
const int COMP_SUC_LEV2 = 8;//2级强化宝石强化成功率 8%
const int COMP_SUC_LEV3 = 3;//3级强化宝石强化成功率 3%
const int COMP_SUC_LEV4 = 1;//4级强化宝石强化成功率 1%
const int COMP_SUC_FAIL_RATE = 10;//强化或者加星失败后每个强化宝石的成功率加成 0.01%
const int COMP_MAX_EQUIPLEV	 = 10;//强化或者加星的最高等级

const int COMP_MAKEHOLE_COST	= 30000;//装备打孔的银两花费
const int COMP_EMBEDGEM_COST	= 5000;//装备镶嵌的银两花费
const int COMP_TAKEOUTGEM_COST	= 5000;//装备取出宝石的银两花费

//VIP成功率加成
const int COMP_VIP_SUC_ADD2		= 1;//2级VIP
const int COMP_VIP_SUC_ADD3		= 2;

enum
{
	MSGCOMPOUND_EQUIPCREATE		= 5,
	MSGCOMPOUND_EMBEDEQUIP		= 6,
	MSGCOMPOUND_EMBED_SUC		= 9,
	MSGCOMPOUND_EMBED_FAL		= 10,
	MSGCOMPOUND_EQUIPCREATE_SUC  = 14,
	MSGCOMPOUND_MAKEHOLE			= 23,
	MSGCOMPOUND_MAKEHOLE_SUC		= 24,
	MSGCOMPOUND_MAKEHOLE_FAL		= 25,
	MSGCOMPOUND_EQUIPCREATE_FAL		= 26,
	MSGCOMPOUND_EQUIP_DECOMPOUND		= 60,
	MSGCOMPOUND_EQUIP_TRANS				= 61,
	//[2010-07-19 goto]添加以下
	MSGCOMPOUND_EQUIP_LEVUP				= 62,//[2010-07-19 goto] 装备强化		C-S
	MSGCOMPOUND_EQUIP_LEVUP_SUC			= 63,//[2010-07-19 goto] 装备强化成功	S-C
	MSGCOMPOUND_EQUIP_LEVUP_FAIL		= 64,//[2010-07-19 goto] 装备强化失败	S-C
	
	MSGCOMPOUND_EQUIP_ADDSTAR			= 65,//[2010-07-19 goto] 装备加星		C-S
	MSGCOMPOUND_EQUIP_ADDSTAR_SUC		= 66,//[2010-07-19 goto] 装备加星成功	S-C
	MSGCOMPOUND_EQUIP_ADDSTAR_FAIL		= 67,//[2010-07-19 goto] 装备加星失败	S-C

	MSGCOMPOUND_PUT_EQUIP				= 68,//[2010-07-20 goto] 用户放入要强化的装备，此时给客户端发送装备的失败加成
	MSGCOMPOUND_EQUIP_FAILRATE			= 69,//[2010-07-20 goto] 装备的失败加成

	MSGCOMPOUND_EMBED_TAKEOUTGEM		= 70,//[2010-07-21 goto] 取出宝石
	MSGCOMPOUND_EMBED_TAKEOUTGEM_SUC	= 71,//[2010-07-21 goto] 取出宝石成功
	MSGCOMPOUND_EMBED_TAKEOUTGEM_FAIL	= 72,//[2010-07-21 goto] 取出宝石失败
	
	MSGCOMPOUND_GEM_LEVUP				= 73,//[2010-08-04 goto] 宝石合成，3-5个低级宝石合成一个高级宝石
	MSGCOMPOUND_GEM_LEVUP_SUC			= 74,//[2010-08-04 goto] 宝石合成成功
	MSGCOMPOUND_GEM_LEVUP_FAIL			= 75,//[2010-08-04 goto] 宝石合成失败

	MSGCOMPOUND_EQUIP_SOULMARK			= 76,//goto 魂印
	MSGCOMPOUND_EQUIP_SOULMARK_SUC		= 77,
	MSGCOMPOUND_EQUIP_SOULMARK_FAIL		= 78,

	MSGCOMPOUND_GEM_CUTWORK				= 79,//宝石雕琢
	MSGCOMPOUND_GEM_CUTWORK_SUC			= 80,
	MSGCOMPOUND_GEM_CUTWORK_FAIL		= 81,

	MSGCOMPOUND_EQUIP_SOULADD				= 82,//铸灵
	MSGCOMPOUND_EQUIP_SOULADD_SUC			= 83,
	MSGCOMPOUND_EQUIP_SOULADD_FAIL		= 84,
	
	MSGCOMPOUND_EQUIP_SOULEXCHANGE				= 85,//铸灵转换
	MSGCOMPOUND_EQUIP_SOULEXCHANGE_SUC			= 86,
	MSGCOMPOUND_EQUIP_SOULEXCHANGE_FAIL		= 87,
//modify code by 林德渊 begin 2011.7.13
	MSGCOMPOUND_ARTIFIC					= 88,//装备炼化
	MSGCOMPOUND_ARTIFIC_SUC				= 89,
	MSGCOMPOUND_ARTIFIC_FAIL			= 90,
	
	MSGCOMPOUND_EQUIP_MOVE				= 91,//强化转移
	MSGCOMPOUND_EQUIP_MOVE_SUC			= 92,
	MSGCOMPOUND_EQUIP_MOVE_FAIL			= 93,
//modify code by 林德渊 end

	MSGCOMPOUND_SENDPEIFANG				= 101,
	MSGCOMPOUND_PEIFANGADD              = 102,
	MSGCOMPOUND_PEIFANGDEL              = 103,
};
class CMsgCompound : public CNetMsg  
{
public:
	CMsgCompound();
	virtual ~CMsgCompound();

	BOOL	Create(char* pbufMsg, DWORD dwMsgSize);
	BOOL	Create(int nAction,OBJID idItem, int data=0);
	void	Process(void *pInfo);
	bool	CheckSameItemID (int* array,int ArraySize);
	BOOL    AppendPeifang(OBJID idPeifang);
	typedef struct
	{
		int nItemID;
		int nAmount;
	}MATERIAL,LPMATERIAL;
private:
	typedef struct
	{
		MSGHEAD_DEFINE;
		int nAction;
		int nItemTypeID;//进行操作的装备ID，不是type
		int nMaterialSize;//MATERIAL个数.包括MSG_Info本身的那个 [2010-07-19 goto] 修改为强化石个数，不包括装备本身
		MATERIAL Material[1]; //强化或升星时改成强化符个数和是否自动买符
	}MSG_Info;
	
	MSG_Info *m_pInfo;

};

#endif // !defined(AFX_MSGCOMPOUND_H__9FE3B9EF_9DD9_4F14_902B_8E7003F5B884__INCLUDED_)
