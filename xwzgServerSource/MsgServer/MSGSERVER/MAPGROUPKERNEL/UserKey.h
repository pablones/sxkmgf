// UserKey.h: interface for the CUserKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERKEY_H__26D8B3DA_F8D6_4A9B_A19E_D6A27F2AEEF0__INCLUDED_)
#define AFX_USERKEY_H__26D8B3DA_F8D6_4A9B_A19E_D6A27F2AEEF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"

enum SYSKEY{
	SYSKEY_ID = 0,				    // 系统按键Id

	
//	SYSKEY_nAction,					// 发送消息的动作
	SYSKEY_FeelSwitch,				// 心情开关
	SYSKEY_MusicSwitch,				// 音乐开关
	SYSKEY_OvlSwitch,				// 音效开关
	SYSKEY_USERID,			        // 用户ID

	SYSKEY_KEY1,				    // 用户按键1
	SYSKEY_KEY2,				    // 用户按键2
	SYSKEY_KEY3,				    // 用户按键3
	SYSKEY_KEY4,				    // 用户按键4
	SYSKEY_KEY5,				    // 用户按键5
	SYSKEY_KEY6,				    // 用户按键6
	SYSKEY_KEY7,				    // 用户按键7
	SYSKEY_KEY8,				    // 用户按键8
	
	SYSKEY_KEYF1,				    // 用户按键F1
	SYSKEY_KEYF2,				    // 用户按键F2
	SYSKEY_KEYF3,				    // 用户按键F3
	SYSKEY_KEYF4,				    // 用户按键F4
	SYSKEY_KEYF5,				    // 用户按键F5
	SYSKEY_KEYF6,				    // 用户按键F6
	SYSKEY_KEYF7,				    // 用户按键F7
	SYSKEY_KEYF8,				    // 用户按键F8
 
	SYSKEY_SEC_KEY1,				    // 用户副快捷栏按键1
	SYSKEY_SEC_KEY2,				    // 用户副快捷栏按键2
	SYSKEY_SEC_KEY3,				    // 用户副快捷栏按键3
	SYSKEY_SEC_KEY4,				    // 用户副快捷栏按键4
	SYSKEY_SEC_KEY5,				    // 用户副快捷栏按键5
	SYSKEY_SEC_KEY6,				    // 用户副快捷栏按键6
	SYSKEY_SEC_KEY7,				    // 用户副快捷栏按键7
	SYSKEY_SEC_KEY8,				    // 用户副快捷栏按键8
	
	SYSKEY_SEC_KEYF1,				    // 用户副快捷栏按键F1
	SYSKEY_SEC_KEYF2,				    // 用户副快捷栏按键F2
	SYSKEY_SEC_KEYF3,				    // 用户副快捷栏按键F3
	SYSKEY_SEC_KEYF4,				    // 用户副快捷栏按键F4
	SYSKEY_SEC_KEYF5,				    // 用户副快捷栏按键F5
	SYSKEY_SEC_KEYF6,				    // 用户副快捷栏按键F6
	SYSKEY_SEC_KEYF7,				    // 用户副快捷栏按键F7
	SYSKEY_SEC_KEYF8,				    // 用户副快捷栏按键F8

	SYSKEY_MapSwitch1,				// 探索地图1
	SYSKEY_Mapswitch2,				// 探索地图2

//  SYSKEY_KEYA1,				    // 用户按键A1
//  SYSKEY_KEYA2,				    // 用户按键A2
//  SYSKEY_KEYA3,				    // 用户按键A3
//  SYSKEY_KEYA4,				    // 用户按键A4
// 	SYSKEY_KEYA5,				    // 用户按键A5
// 	SYSKEY_KEYA6,				    // 用户按键A6
// 	SYSKEY_KEYA7,				    // 用户按键A7
// 	SYSKEY_KEYA8,				    // 用户按键A8
// 
// 	SYSKEY_KEYPROTERY,				// 用户属性
// 	SYSKEY_KEYFRIEND,				// 用户好友
// 	SYSKEY_KEYITEM,				    // 用户背包
// 	SYSKEY_KEYBIGMAP,				// 大地图
// 	SYSKEY_KEYFACTION,				// 军团
// 	SYSKEY_KEYSKILL,				// 技能
// 	SYSKEY_KEYMAIL,				    // 邮件
// 	SYSKEY_KEYTASK,				    // 任务
// 	SYSKEY_KEYPICK,				    // 拾取物品
// 	SYSKEY_KEYPHYATT,				// 物理攻击
// 	SYSKEY_KEYRUNTOWALK,			// 走跑切换
// 	SYSKEY_KEYCHANGETARGET,			// 切换目标
// 	SYSKEY_KEYSYSSETTING,			// 系统设置
// 	SYSKEY_KEYTEAM,				    // 组队
// 	SYSKEY_KEYWISDOMSETTING,		// 生活技能
// 	SYSKEY_KEYAUTIATT,				// 问天指南
// 	SYSKEY_KEYGIFT,				    // 天赋
// 
// 
 	SYSKEY_DLGSTATES,				// DLG相关
	SYSKEY_CHANNEL,

	SYSKEY_AUTOFIRSKILL,			//自动释放技能主职业
	SYSKEY_AUTOSECSKILL,			//自动释放技能副职业
	SYSKEY_AUTOSYNSKILL,			//自动释放帮派技能
	SYSKEY_SYSTEMSETTING,			//系统设置
	
	//////////////////////////挂机药品
	SYSKEY_DRESS,
	SYSKEY_FATIGUE,
	SYSKEY_IDHIGH,
	SYSKEY_IDLOW,
	SYSKEY_REALNAME,
	SYSKEY_AUTOFMPMED3,
	SYSKEY_AUTOFEUDHPMED1,
	SYSKEY_AUTOFEUDHPMED2,
	SYSKEY_AUTOFEUDHPMED3,
	//////////////////////////挂机血量低于多少的时候吃药
	SYSKEY_AUTOFEATHP,
	SYSKEY_AUTOFEATMP,
	SYSKEY_AUTOFEATEUDHP,
	SYSKEY_TREASURECOST,//记录玩家开箱子开了多少元宝仍然没有出麒麟之魂,后台数据，不用同步到客户端
	SYSKEY_VASTICKET,//当天兑换的元宝票数量
// 	SYSKEY_KEYAUTOFIGHT,            //开始自动打怪
// 	SYSKEY_KEYBEGINFIGHT,           //开始挂机

//	SYSKEY_KEYF9,				    // 用户按键F9
//	SYSKEY_KEYF10,				    // 用户按键F10
//	SYSKEY_KEYF11,				    // 用户按键F11
};

typedef struct UserKey
{

	char			nAction;
	char			FeelSwitch;
	char			MusicSwitch;
	char			OvlSwitch;
	OBJID			userId;

	//第一门派
	OBJID           key1;
	OBJID           key2;
	OBJID           key3;
	OBJID           key4;
	OBJID           key5;
	OBJID           key6;
	OBJID           key7;
	OBJID           key8;
	
	OBJID           keyF1;
	OBJID           keyF2;
	OBJID           keyF3;
	OBJID           keyF4;
	OBJID           keyF5;
	OBJID           keyF6;
	OBJID           keyF7;
	OBJID           keyF8;

	//第二门派
	OBJID           sec_key1;
	OBJID           sec_key2;
	OBJID           sec_key3;
	OBJID           sec_key4;
	OBJID           sec_key5;
	OBJID           sec_key6;
	OBJID           sec_key7;
	OBJID           sec_key8;
	
	OBJID           sec_keyF1;
	OBJID           sec_keyF2;
	OBJID           sec_keyF3;
	OBJID           sec_keyF4;
	OBJID           sec_keyF5;
	OBJID           sec_keyF6;
	OBJID           sec_keyF7;
	OBJID           sec_keyF8;
	//地图探索相关
	OBJID			MapSwitch1;
	OBJID			MapSwitch2;//预留，地图数量超过32时开放

// 	OBJID  keyF9;
// 	OBJID  keyF10;
// 	OBJID  keyF11;
// 
// 	OBJID  keyA1;
// 	OBJID  keyA2;
// 	OBJID  keyA3;
// 	OBJID  keyA4;
// 	OBJID  keyA5;
// 	OBJID  keyA6;
// 	OBJID  keyA7;
// 	OBJID  keyA8;	
// 
// 	char   szProtery[11];
// 	char   szFriend[11];
// 	char   szItem[11];
// 	char   szBigMap[11];
// 	char   szFaction[11];
// 	char   szSkill[11];
// 	char   szMail[11];
// 	char   szTask[11];
// 	char   szPick[11];
// 	char   szPhyAtt[11];
// 	char   szRunToWalk[11];
// 	char   szChgTarget[11];
// 	char   szSysSetting[11];
// 	char   szTeam[11];
// 	char   szWisdomSetting[11];
// 	char   szAutoAtt[11];
// 	char   szGift[11];
// 	char   szAutoFight[11];
// 	char   szBeginFight[11];
// 
 	OBJID  dlgStates;
	OBJID  Channel;

	OBJID  AutoFIRSkill;	//默认值 32个1
	OBJID  AutoSECSkill;
	OBJID  AutoSYNSkill;
	OBJID  SystemSetting;  //0
	//////////////////////////挂机药品
	int    dress;
	int    fatigue;
	char   idHigh[20];
	int    idLow;
	char   realname[15];
	int    autofmpmed3;
	int    autofeudhpmed1;
	int    autofeudhpmed2;
	int    autofeudhpmed3;
		//////////////////////////挂机血量低于多少的时候吃药
	int    autofeathp;
	int    autofeatmp;
	int    autofeateudhp;
	int    TreasureCost;
	int	   nVasTicket;
	DWORD  id;
}SYSTEMUSERKEY;

//用户设置,相应的位1表示开启，0表示关闭
enum USERCONFIG
{
	USERCONFIG_SOUND		= 0x0001,	//音效
	USERCONFIG_MUSIC		= 0x0002,	//音乐
	USERCONFIG_TEAM			= 0x0004,	//组队
	USERCONFIG_TRADE		= 0x0008,	//交易
	USERCONFIG_PK			= 0x0010,	//切磋
	USERCONFIG_FRIEND		= 0x0020,	//好友
	USERCONFIG_ITEMLOCK		= 0x0040,	//安全锁
};

char    szKeyTable[];
char    szUserId[];
typedef CGameData<SYSKEY,szKeyTable,szUserId>     CGameUserKey;

//class IRecord;
//class IRecordset; 

class IDatabase;

class CUserKey : public CGameObj
{
//protected:
public:
	CUserKey();
	virtual ~CUserKey();

	CUserKey* CreateNew(){ return new CUserKey; }
	bool      ReleaseOwner() { delete this;}

	void	SendObjInfo	(PROCESS_ID idProcess);
	void	SetObjInfo(SYSTEMUSERKEY* pStInfo);
public:
	bool      Create(IDatabase* pDb,CUser* pUser,bool bLogin,IRecordset* pRec);
	bool	  LoadInfo();
	bool	  SaveInfo();
	bool      SendKey(CUser *pUser);  //送用户按键信息
	bool      SaveKey(struct UserKey &KeyInfo,bool bUpdate = false,bool bInsert = true);  //保存用户按键信息

	void      SetInt(SYSKEY idx,int nData,bool bUpdate = false);
    void      SetStr(SYSKEY idx, LPCTSTR szData, int nSize, bool bUpdate = false);

	int       GetDlgStates() { return m_SysKey.dlgStates; }
	int       GetFeelSwitch() { return m_SysKey.FeelSwitch; }

	//goto 用户设置相关
	int		  GetFatigue(){return m_pUserKey->GetInt(SYSKEY_FATIGUE);}
	int		  GetUserSysSet()		{ return m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING); }
	bool	  IsAgreeTeamApply()	{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_TEAM) == 0; }
	bool	  IsAgreeTradeApply()	{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_TRADE) == 0; }
	bool	  IsAgreePKApply()		{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_PK) == 0; }
	bool	  IsAgreeFriendApply()	{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_FRIEND) == 0; }
	bool	  IsLocked()			{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_ITEMLOCK) != 0; }
	int		  GetTreasureCost()		{ return m_pUserKey->GetInt(SYSKEY_TREASURECOST); }//记录玩家开箱子开了多少元宝仍然没有出麒麟之魂
	void	  SetTreasureCost(int nCost, bool bUpdate=false)	{ SetInt(SYSKEY_TREASURECOST, nCost, bUpdate); }//记录玩家开箱子开了多少元宝仍然没有出麒麟之魂
	int		  GetVasTicketAmount()	{ return m_pUserKey->GetInt(SYSKEY_VASTICKET); }
	void	  SetVasTicketAmount(int nAmount, bool bUpdate=false)	{ SetInt(SYSKEY_VASTICKET, nAmount, bUpdate); }

public:
	void SetFatigue(int nFatigue, bool bUpdate = false);
	void SetRealName(char* RealName)	{m_pUserKey->SetStr(SYSKEY_REALNAME, RealName, _MAX_NAMESIZE); m_pUserKey->Update();}
	DWORD GetDress();
	bool SetDress(OBJID nDress, bool uPdate = false);
	void SetAutoSkill(OBJID AutoFirSkill, OBJID AutoSecSkill, OBJID AutoSynSkill, CUser* pUser, bool bUpdate=false);
	bool SendAutofight(CUser* pUser);
	bool    DeleteUserKey(IDatabase *pDb,CUser* pUser);
	BOOL bInit;
protected:
	bool	m_bNeedUpdate;
	CGameUserKey *m_pUserKey;
    SYSTEMUSERKEY m_SysKey;
	CUser* m_pUser;
};

#endif // !defined(AFX_USERKEY_H__26D8B3DA_F8D6_4A9B_A19E_D6A27F2AEEF0__INCLUDED_)
