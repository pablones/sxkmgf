// MsgBuff.h: interface for the CMsgBuff class.
//	20060922	修罗
//	Buff消息
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGBUFF_H__89528D88_2C03_48C3_904A_6CE4F828568F__INCLUDED_)
#define AFX_MSGBUFF_H__89528D88_2C03_48C3_904A_6CE4F828568F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

//------------------------07.4.25王玉波注释--------
//enum BuffType//Buff类型,由user status修改.以后再做扩展
//{
//	BUFFTYPE_NONE			=	0,
//	BUFFTYPE_HITRATE_UP = 1,			//54 命中率提升HIT_RATE_UP	提升命中d%\%,持续d%秒
//	BUFFTYPE_HITRATE_DOWN = 2,		
//	BUFFTYPE_DODGE_UP = 3,				//47 闪避提升DODGE_UPd	提升闪避d%\%,持续d%秒
//	BUFFTYPE_DODGE_DOWN = 4,		
//	BUFFTYPE_POWER_UP = 5,				//12 战斗力提升POWER_UP	提升最终伤害d%\%,持续d%秒
//	BUFFTYPE_POWER_DWON = 6,
//	BUFFTYPE_DEFENSE_UP = 7,			//26 防御力提升DEFENSE_UP	减少受到伤害d%\%,持续d%秒
//	BUFFTYPE_DEFENSE_DOWN = 8,
//	BUFFTYPE_MOVE_SPEED_UP = 9,			//51 疾步FAST_MOVE	加快移动速度d%\%,持续d%秒
//	BUFFTYPE_MOVE_SPEED_DOWN = 10,		//27 降低移d动速度 d%\%,持续N秒
//	BUFFTYPE_WISDOM_UP = 11,			//56 顿悟之术WISDOM_UP	技能间隔时间降低d%\%,持续d%秒
//	BUFFTYPE_TRANSFER_SHIELD = 12,		//108 魔法盾	抵消伤害d%点,持续d%秒
//	BUFFTYPE_BLESS	=13,				//111 恩泽天赐BLESS	提高力量,敏捷,智力,体力d%\%,持续d%秒
//	BUFFTYPE_PK_PROTECTED = 20,			//50 pk报保护
//	BUFFTYPE_IMMUNE = 27,				//无敌
//	BUFFTYPE_FIRMNESSMAGIC = 28,		//坚定施法
//	BUFFTYPE_FRENZY = 29,				//狂暴
//	BUFFTYPE_MULTIPLE_EXP = 30,			//双倍经验BUF//---彭峰-----2007/03/29---------------
//
//	BUFFTYPE_BAD=100,
//	BUFFTYPE_PALSY = BUFFTYPE_BAD+14,				//30 麻痹	暂时麻痹,原地不能移动,持续d%秒
//	BUFFTYPE_POISON = BUFFTYPE_BAD+15,				//3  中毒	每4秒减去生命值d%点,持续d%秒
//	BUFFTYPE_BLEB = BUFFTYPE_BAD+16,					//58 流血	每4秒减去生命值d%点,持续d%秒
//	BUFFTYPE_MANA_DRINK = BUFFTYPE_BAD+17,			//59 噬法 	每4秒减去魔法值d%点,持续d%秒
//	BUFFTYPE_TIRED = BUFFTYPE_BAD+18,				//60 乏力   暗黄TIRED	每4秒减去体力值d%点,持续d%秒
//	BUFFTYPE_TAKE_SKZL =BUFFTYPE_BAD+19,				//57 获得1颗时空之泪TAKE_SKZL 增加100的物理和魔法防御
//	BUFFTYPE_FANJI	=BUFFTYPE_BAD+	21,				//109 反击状态
//	BUFFTYPE_FAINT	=	BUFFTYPE_BAD+22,				//101 眩晕
//	BUFFTYPE_CONFUSION	=BUFFTYPE_BAD+ 23,			//混乱
//	BUFFTYPE_SILENCE	=BUFFTYPE_BAD+ 24,			//沉默
//	BUFFTYPE_BILE		=BUFFTYPE_BAD+ 25,			//愤怒
//	BUFFTYPE_BURNING	= BUFFTYPE_BAD+26,			//灼伤
//	 
//	
//};

//struct BuffInfo 
//{
//	int					buffPower;				//具体生效的数值.正数为数值.负数为百分比
//};
//-----------------07.4.25王玉波添加------------------------------
enum
{
	ACTION_ADDSTATUS	=	1,			//添加一个状态,buff or debuff or dot
	ACTION_DELSTATUS	=	2,			//删除一个状态,buff or debuff or dot
	ACTION_UPDATESTATUS	=	3,			//更新一个状态............
};
//-------------------------end-------------------------------------

class CMsgBuff : public CNetMsg  
{
public:
	CMsgBuff();
	~CMsgBuff();

	//仅用在服务端
	BOOL	Create( int nAction,OBJID idUser,DWORD dwTypeID,int nTime,int nCount,int nPower,int nParam1,int nParam2,int nParam3 );
	BOOL	Create(char* pMsgBuf, DWORD dwSize);
	void	Process(void *pInfo);

private:
	typedef struct
	{
		MSGHEAD_DEFINE
		int				nAction;
		OBJID			idUser;
		DWORD			dwTypeID;
		int				nTime;			//持续时间或者间隔时间
		int				nCount;			//多次状态的次数
		
		int				nPower;
		int				nParam1;
		int				nParam2;
		int				nParam3;
		

	}MSG_Info;
	
	MSG_Info *m_pInfo;
};

#endif // !defined(AFX_MSGBUFF_H__89528D88_2C03_48C3_904A_6CE4F828568F__INCLUDED_)
//238,239,230 (#E6EFEE)
