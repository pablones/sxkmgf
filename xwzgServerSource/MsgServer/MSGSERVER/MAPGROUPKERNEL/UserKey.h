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
	SYSKEY_ID = 0,				    // ϵͳ����Id

	
//	SYSKEY_nAction,					// ������Ϣ�Ķ���
	SYSKEY_FeelSwitch,				// ���鿪��
	SYSKEY_MusicSwitch,				// ���ֿ���
	SYSKEY_OvlSwitch,				// ��Ч����
	SYSKEY_USERID,			        // �û�ID

	SYSKEY_KEY1,				    // �û�����1
	SYSKEY_KEY2,				    // �û�����2
	SYSKEY_KEY3,				    // �û�����3
	SYSKEY_KEY4,				    // �û�����4
	SYSKEY_KEY5,				    // �û�����5
	SYSKEY_KEY6,				    // �û�����6
	SYSKEY_KEY7,				    // �û�����7
	SYSKEY_KEY8,				    // �û�����8
	
	SYSKEY_KEYF1,				    // �û�����F1
	SYSKEY_KEYF2,				    // �û�����F2
	SYSKEY_KEYF3,				    // �û�����F3
	SYSKEY_KEYF4,				    // �û�����F4
	SYSKEY_KEYF5,				    // �û�����F5
	SYSKEY_KEYF6,				    // �û�����F6
	SYSKEY_KEYF7,				    // �û�����F7
	SYSKEY_KEYF8,				    // �û�����F8
 
	SYSKEY_SEC_KEY1,				    // �û������������1
	SYSKEY_SEC_KEY2,				    // �û������������2
	SYSKEY_SEC_KEY3,				    // �û������������3
	SYSKEY_SEC_KEY4,				    // �û������������4
	SYSKEY_SEC_KEY5,				    // �û������������5
	SYSKEY_SEC_KEY6,				    // �û������������6
	SYSKEY_SEC_KEY7,				    // �û������������7
	SYSKEY_SEC_KEY8,				    // �û������������8
	
	SYSKEY_SEC_KEYF1,				    // �û������������F1
	SYSKEY_SEC_KEYF2,				    // �û������������F2
	SYSKEY_SEC_KEYF3,				    // �û������������F3
	SYSKEY_SEC_KEYF4,				    // �û������������F4
	SYSKEY_SEC_KEYF5,				    // �û������������F5
	SYSKEY_SEC_KEYF6,				    // �û������������F6
	SYSKEY_SEC_KEYF7,				    // �û������������F7
	SYSKEY_SEC_KEYF8,				    // �û������������F8

	SYSKEY_MapSwitch1,				// ̽����ͼ1
	SYSKEY_Mapswitch2,				// ̽����ͼ2

//  SYSKEY_KEYA1,				    // �û�����A1
//  SYSKEY_KEYA2,				    // �û�����A2
//  SYSKEY_KEYA3,				    // �û�����A3
//  SYSKEY_KEYA4,				    // �û�����A4
// 	SYSKEY_KEYA5,				    // �û�����A5
// 	SYSKEY_KEYA6,				    // �û�����A6
// 	SYSKEY_KEYA7,				    // �û�����A7
// 	SYSKEY_KEYA8,				    // �û�����A8
// 
// 	SYSKEY_KEYPROTERY,				// �û�����
// 	SYSKEY_KEYFRIEND,				// �û�����
// 	SYSKEY_KEYITEM,				    // �û�����
// 	SYSKEY_KEYBIGMAP,				// ���ͼ
// 	SYSKEY_KEYFACTION,				// ����
// 	SYSKEY_KEYSKILL,				// ����
// 	SYSKEY_KEYMAIL,				    // �ʼ�
// 	SYSKEY_KEYTASK,				    // ����
// 	SYSKEY_KEYPICK,				    // ʰȡ��Ʒ
// 	SYSKEY_KEYPHYATT,				// ������
// 	SYSKEY_KEYRUNTOWALK,			// �����л�
// 	SYSKEY_KEYCHANGETARGET,			// �л�Ŀ��
// 	SYSKEY_KEYSYSSETTING,			// ϵͳ����
// 	SYSKEY_KEYTEAM,				    // ���
// 	SYSKEY_KEYWISDOMSETTING,		// �����
// 	SYSKEY_KEYAUTIATT,				// ����ָ��
// 	SYSKEY_KEYGIFT,				    // �츳
// 
// 
 	SYSKEY_DLGSTATES,				// DLG���
	SYSKEY_CHANNEL,

	SYSKEY_AUTOFIRSKILL,			//�Զ��ͷż�����ְҵ
	SYSKEY_AUTOSECSKILL,			//�Զ��ͷż��ܸ�ְҵ
	SYSKEY_AUTOSYNSKILL,			//�Զ��ͷŰ��ɼ���
	SYSKEY_SYSTEMSETTING,			//ϵͳ����
	
	//////////////////////////�һ�ҩƷ
	SYSKEY_DRESS,
	SYSKEY_FATIGUE,
	SYSKEY_IDHIGH,
	SYSKEY_IDLOW,
	SYSKEY_REALNAME,
	SYSKEY_AUTOFMPMED3,
	SYSKEY_AUTOFEUDHPMED1,
	SYSKEY_AUTOFEUDHPMED2,
	SYSKEY_AUTOFEUDHPMED3,
	//////////////////////////�һ�Ѫ�����ڶ��ٵ�ʱ���ҩ
	SYSKEY_AUTOFEATHP,
	SYSKEY_AUTOFEATMP,
	SYSKEY_AUTOFEATEUDHP,
	SYSKEY_TREASURECOST,//��¼��ҿ����ӿ��˶���Ԫ����Ȼû�г�����֮��,��̨���ݣ�����ͬ�����ͻ���
	SYSKEY_VASTICKET,//����һ���Ԫ��Ʊ����
// 	SYSKEY_KEYAUTOFIGHT,            //��ʼ�Զ����
// 	SYSKEY_KEYBEGINFIGHT,           //��ʼ�һ�

//	SYSKEY_KEYF9,				    // �û�����F9
//	SYSKEY_KEYF10,				    // �û�����F10
//	SYSKEY_KEYF11,				    // �û�����F11
};

typedef struct UserKey
{

	char			nAction;
	char			FeelSwitch;
	char			MusicSwitch;
	char			OvlSwitch;
	OBJID			userId;

	//��һ����
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

	//�ڶ�����
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
	//��ͼ̽�����
	OBJID			MapSwitch1;
	OBJID			MapSwitch2;//Ԥ������ͼ��������32ʱ����

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

	OBJID  AutoFIRSkill;	//Ĭ��ֵ 32��1
	OBJID  AutoSECSkill;
	OBJID  AutoSYNSkill;
	OBJID  SystemSetting;  //0
	//////////////////////////�һ�ҩƷ
	int    dress;
	int    fatigue;
	char   idHigh[20];
	int    idLow;
	char   realname[15];
	int    autofmpmed3;
	int    autofeudhpmed1;
	int    autofeudhpmed2;
	int    autofeudhpmed3;
		//////////////////////////�һ�Ѫ�����ڶ��ٵ�ʱ���ҩ
	int    autofeathp;
	int    autofeatmp;
	int    autofeateudhp;
	int    TreasureCost;
	int	   nVasTicket;
	DWORD  id;
}SYSTEMUSERKEY;

//�û�����,��Ӧ��λ1��ʾ������0��ʾ�ر�
enum USERCONFIG
{
	USERCONFIG_SOUND		= 0x0001,	//��Ч
	USERCONFIG_MUSIC		= 0x0002,	//����
	USERCONFIG_TEAM			= 0x0004,	//���
	USERCONFIG_TRADE		= 0x0008,	//����
	USERCONFIG_PK			= 0x0010,	//�д�
	USERCONFIG_FRIEND		= 0x0020,	//����
	USERCONFIG_ITEMLOCK		= 0x0040,	//��ȫ��
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
	bool      SendKey(CUser *pUser);  //���û�������Ϣ
	bool      SaveKey(struct UserKey &KeyInfo,bool bUpdate = false,bool bInsert = true);  //�����û�������Ϣ

	void      SetInt(SYSKEY idx,int nData,bool bUpdate = false);
    void      SetStr(SYSKEY idx, LPCTSTR szData, int nSize, bool bUpdate = false);

	int       GetDlgStates() { return m_SysKey.dlgStates; }
	int       GetFeelSwitch() { return m_SysKey.FeelSwitch; }

	//goto �û��������
	int		  GetFatigue(){return m_pUserKey->GetInt(SYSKEY_FATIGUE);}
	int		  GetUserSysSet()		{ return m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING); }
	bool	  IsAgreeTeamApply()	{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_TEAM) == 0; }
	bool	  IsAgreeTradeApply()	{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_TRADE) == 0; }
	bool	  IsAgreePKApply()		{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_PK) == 0; }
	bool	  IsAgreeFriendApply()	{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_FRIEND) == 0; }
	bool	  IsLocked()			{ return (m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING) & USERCONFIG_ITEMLOCK) != 0; }
	int		  GetTreasureCost()		{ return m_pUserKey->GetInt(SYSKEY_TREASURECOST); }//��¼��ҿ����ӿ��˶���Ԫ����Ȼû�г�����֮��
	void	  SetTreasureCost(int nCost, bool bUpdate=false)	{ SetInt(SYSKEY_TREASURECOST, nCost, bUpdate); }//��¼��ҿ����ӿ��˶���Ԫ����Ȼû�г�����֮��
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
