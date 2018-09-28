// MsgAutoFight.h: interface for the CMsgAutoFight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGAUTOFIGHT_H__176F519E_5D50_4759_A1CA_ECEB5C92D4BB__INCLUDED_)
#define AFX_MSGAUTOFIGHT_H__176F519E_5D50_4759_A1CA_ECEB5C92D4BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"
#include "../share/protocol.h"

enum _AutoFight{
	_AutoFight_WRITE	= 0,
	_AutoFight_READ,
	_AutoFight_PICKUP,
	_AutoFight_OUTOFRANGE,
};

class CMsgAutoFight :public CNetMsg 
{
public:
	CMsgAutoFight();
	virtual ~CMsgAutoFight();
	
	virtual BOOL Create		(char* pMsgBuf, DWORD dwSize);
	virtual void Process	(void *pInfo);
	
	
	BOOL Create(int nAction, struct AutoFightInfo &pInfo);
	BOOL Create(_AutoFight nAction, int nData = 0);
private:
	typedef struct
	{
		MSGHEAD_DEFINE
		UINT nAction;
		
//		OBJID id  ;                 //自动增长的索引       
		OBJID idUser ;              //自动挂机保存设置的玩家id
		//开关
		OBJID AutoSwitch;		   //所有开关
		unsigned short PercentHp;
		unsigned short PercentMp;
		unsigned short SkillHp;
		unsigned short SkillMp;
		unsigned short SkillTeamhp;
		unsigned short EudHp;
		unsigned short EudHappy;
		unsigned short HitRange;
		unsigned short DoubleExp;
		unsigned short DoubleEudExp;
		unsigned short LIYUCHUN;
		unsigned short temp;
		OBJID Hpmed1;
		OBJID Hpmed2;
		OBJID Hpmed3;
		OBJID Mpmed1;
		OBJID Mpmed2;
		OBJID Mpmed3;
		OBJID EudHpmed1;
		OBJID EudHpmed2;
		OBJID EudHpmed3;
		OBJID EudToy1;
		OBJID EudToy2;

// 		//  以下为自动恢复设置字段：
// 		UINT autoRestoration_hp ;  //是否开启自动恢复保护中的"开启生命保护"，开启为"1","2"，未开启为"0"。
// 		char aRhp_item[_MAX_NAMESIZE] ;           //使用何种道具以自动恢复血量。填写道具ID。
// 		char aRhp_item2[_MAX_NAMESIZE] ;          //使用何种道具以自动恢复血量。填写道具ID。
// 		UINT aRhpPercentage ;      //当血量低至最大值百分之多少时，触发自动回血。填写百分数。
// 		UINT aRhpPercentage2 ;      //当血量低至最大值百分之多少时，触发自动回血。填写百分数。
// 		UINT autoRestoration_mp;   //是否开启自动恢复保护中的"开启魔法值保护"，开启为"1","2"，未开启为"0"。
// 		char aRmp_Item[_MAX_NAMESIZE];            //使用何种道具以自动恢复魔法量。填写道具ID。
// 		char aRmp_Item2[_MAX_NAMESIZE];            //使用何种道具以自动恢复魔法量。填写道具ID。
// 		UINT armpPercentage ;      //当魔法量低至最大值百分之多少时，触发自动回魔。填写百分数。
// 		UINT armpPercentage2 ;      //当魔法量低至最大值百分之多少时，触发自动回魔。填写百分数。
// 
// 
// // 		UINT autoRestoration_hp ;  //是否开启自动恢复保护中的"开启生命保护"，开启为"1"，未开启为"0"。
// // 		char aRhp_item[_MAX_NAMESIZE] ;           //使用何种道具以自动恢复血量。填写道具ID。
// // 		UINT aRhpPercentage ;      //当血量低至最大值百分之多少时，触发自动回血。填写百分数。
// // 		UINT autoRestoration_mp;   //是否开启自动恢复保护中的"开启魔法值保护"，开启为"1"，未开启为"0"。
// // 		char aRmp_Item[_MAX_NAMESIZE];            //使用何种道具以自动恢复魔法量。填写道具ID。
// // 		UINT armpPercentage ;      //当魔法量低至最大值百分之多少时，触发自动回魔。填写百分数。
// 		// 以下为自动挂机设置字段：
// 		UINT AutoAttack_hp  ;      //是否开启自动挂机中的"开启生命保护"，开启为"1"，未开启为"0"。
// 		char AAhp_item[_MAX_NAMESIZE] ;           //使用何种道具以自动恢复血量。填写道具ID。
// 		UINT AahpPercentage ;      //当血量低至最大值百分之多少时，触发自动回血。填写百分数。
// 		UINT AutoAttack_mp ;      //是否开启自动挂机中的"开启魔法值保护"，开启为"1"，未开启为"0"。
// 		char AAmp_item[_MAX_NAMESIZE]   ;        //使用何种道具以自动恢复魔法量。填写道具ID。
// 		UINT AampPercentage ;     //当血量低至最大值百分之多少时，触发自动回魔。填写百分数。
// 		UINT Pickup ;             //自动拾取设置，1开启自动拾取，0未开启自动拾取，2拾取药品，4拾取材料，8拾取宝石，16拾取技能书，32拾取优秀品质以上，64拾取完美品质以上，128拾取卓越品质以上。
// 		UINT autousevas  ;         //自动使用增值道具，开启为"1"，未开启为"0"，2为自动使用复活卷 4为自动使用神将锤，为自动使用双倍经验药。
// 		UINT autoshopping ;       //自动购买设置，1为开启自动购买，0为未开启，2为自动购买红，4为自动购买蓝，8为自动购买复活卷。该字段填值为以上各设定值相加总数。
// 		UINT num_hp ;             //自动购买红药的数量。
// 		UINT num_mp ;             //自动购买蓝药的数量。
// 		UINT num_fh  ;            //自动购买复活卷的数量。
// 		UINT num_double  ;        //自动使用双倍经验药数量
// 		UINT goal  ;              //自动打怪目标的选择，0为最近的怪，1为正在攻击自己的怪。
// 		UINT way  ;               //自动打怪方式的选择，0为普通攻击，1为技能攻击。
// 		UINT autoUseAssit;        //自动使用辅助魔法
//      	UINT autoUseAttack;       //自动使用攻击魔法
// 
// 		//宠物部分
// 		OBJID	dwEudemon1;
// 		OBJID	dwEudemon2;
// 		DWORD	dwEudemonDrinkType;				
// 		DWORD	dwEudemonDrinkRedID;			
// 		DWORD	dwEudemonDrinkBlueID;			
// 		DWORD	dwEudemonDrinkRedTypeID;			
// 		DWORD	dwEudemonDrinkBlueTypeID;			
// 		DWORD	dwEudemonPercentRed;			
// 		DWORD	dwEudemonPercentBlue;
// 
// 		char skills1[_MAX_NAMESIZE];          //自动打怪使用技能设定，skills1~3为增益技能，skills4~7为攻击技能，填写技能ID。
// 		char skills2[_MAX_NAMESIZE];
// 		char skills3[_MAX_NAMESIZE];
// 		char skills4[_MAX_NAMESIZE];
// 		char skills5[_MAX_NAMESIZE];
// 		char skills6[_MAX_NAMESIZE];
// 		char skills7[_MAX_NAMESIZE];
		
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
	
};

#endif // !defined(AFX_MSGAUTOFIGHT_H__176F519E_5D50_4759_A1CA_ECEB5C92D4BB__INCLUDED_)
