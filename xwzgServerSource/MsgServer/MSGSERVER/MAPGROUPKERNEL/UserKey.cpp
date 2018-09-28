// UserKey.cpp: implementation of the CUserKey class.
//
//////////////////////////////////////////////////////////////////////

#include "UserKey.h"
#include "User.h"
#include "MsgUserKey.h"
#include "MapGroup.h"
//当角色刚刚建立的时候使用的默认值
const OBJID Z_SKILL = 1101; //战士 虎牙突刺
const OBJID G_SKILL = 1302; //弓箭手 急速箭
const OBJID F_SKILL = 7015; //法师  寒冰决

const OBJID medical1_Default = 300001; //止血草
//const OBJID medical2_Default = 310001; //鼠儿果

char    szKeyTable[] = _TBL_USERKEY;
char    szUserId[]   = "userid";
const  int szKeyMaxLen = 11; 

CUserKey::CUserKey()
{
	m_pUserKey = NULL;
	m_pUser = NULL;
	bInit = false;
	m_bNeedUpdate = false;
}


CUserKey::~CUserKey()
{
	if(m_pUserKey)
		m_pUserKey->Release();

}

bool  CUserKey::Create(IDatabase* pDb,CUser* pUser,bool bLogin,IRecordset* pRec)
{
	CHECKF(!m_pUserKey);
	CHECKF(pDb);
	CHECKF(pUser);

	m_pUser = pUser;
	m_pUserKey = CGameUserKey::CreateNew();
	IF_NOT(m_pUserKey)
		return false;
	if(!bLogin)
	{
		IF_NOT(m_pUserKey->Create(pRec, pUser->GetID()))
			return false;

		return true;
	}

	IF_NOT(m_pUserKey->Create(pUser->GetID(),pDb))
		return false;
	LoadInfo();
    return true;
}

bool  CUserKey::LoadInfo()
{
	CHECKF(m_pUserKey);
	CHECKF(m_pUser);

	if(!m_pUserKey->GetID())  //不存在该用户的信息
	{
		m_SysKey.userId = m_pUser->GetID();
		
		m_pUserKey->SetInt(SYSKEY_USERID, m_SysKey.userId);
		if(!m_pUserKey->InsertRecord())
			return false;
	}

	memset(&m_SysKey,0L,sizeof(SYSTEMUSERKEY));

	m_SysKey.nAction = 1;
	m_SysKey.FeelSwitch = m_pUserKey->GetInt(SYSKEY_FeelSwitch);
	if ((m_SysKey.FeelSwitch != 1) && (m_SysKey.FeelSwitch != 0 ))
	{
		m_SysKey.FeelSwitch = 0;
	}
	m_SysKey.MusicSwitch = m_pUserKey->GetInt(SYSKEY_MusicSwitch);
	m_SysKey.OvlSwitch = m_pUserKey->GetInt(SYSKEY_OvlSwitch);

	m_SysKey.userId = m_pUserKey->GetInt(SYSKEY_USERID);
	m_SysKey.key1 = m_pUserKey->GetInt(SYSKEY_KEY1);
	m_SysKey.key2 = m_pUserKey->GetInt(SYSKEY_KEY2);
	m_SysKey.key3 = m_pUserKey->GetInt(SYSKEY_KEY3);
	m_SysKey.key4 = m_pUserKey->GetInt(SYSKEY_KEY4);
	m_SysKey.key5 = m_pUserKey->GetInt(SYSKEY_KEY5);
	m_SysKey.key6 = m_pUserKey->GetInt(SYSKEY_KEY6);
	m_SysKey.key7 = m_pUserKey->GetInt(SYSKEY_KEY7);
	m_SysKey.key8 = m_pUserKey->GetInt(SYSKEY_KEY8);
	
	m_SysKey.keyF1 = m_pUserKey->GetInt(SYSKEY_KEYF1);
	m_SysKey.keyF2 = m_pUserKey->GetInt(SYSKEY_KEYF2);
	m_SysKey.keyF3 = m_pUserKey->GetInt(SYSKEY_KEYF3);
	m_SysKey.keyF4 = m_pUserKey->GetInt(SYSKEY_KEYF4);
	m_SysKey.keyF5 = m_pUserKey->GetInt(SYSKEY_KEYF5);
	m_SysKey.keyF6 = m_pUserKey->GetInt(SYSKEY_KEYF6);
	m_SysKey.keyF7 = m_pUserKey->GetInt(SYSKEY_KEYF7);
	m_SysKey.keyF8 = m_pUserKey->GetInt(SYSKEY_KEYF8);

	m_SysKey.sec_key1 = m_pUserKey->GetInt(SYSKEY_SEC_KEY1);
	m_SysKey.sec_key2 = m_pUserKey->GetInt(SYSKEY_SEC_KEY2);
	m_SysKey.sec_key3 = m_pUserKey->GetInt(SYSKEY_SEC_KEY3);
	m_SysKey.sec_key4 = m_pUserKey->GetInt(SYSKEY_SEC_KEY4);
	m_SysKey.sec_key5 = m_pUserKey->GetInt(SYSKEY_SEC_KEY5);
	m_SysKey.sec_key6 = m_pUserKey->GetInt(SYSKEY_SEC_KEY6);
	m_SysKey.sec_key7 = m_pUserKey->GetInt(SYSKEY_SEC_KEY7);
	m_SysKey.sec_key8 = m_pUserKey->GetInt(SYSKEY_SEC_KEY8);
	
	m_SysKey.sec_keyF1 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF1);
	m_SysKey.sec_keyF2 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF2);
	m_SysKey.sec_keyF3 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF3);
	m_SysKey.sec_keyF4 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF4);
	m_SysKey.sec_keyF5 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF5);
	m_SysKey.sec_keyF6 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF6);
	m_SysKey.sec_keyF7 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF7);
	m_SysKey.sec_keyF8 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF8);

	m_SysKey.MapSwitch1 = m_pUserKey->GetInt(SYSKEY_MapSwitch1);
	m_SysKey.MapSwitch2 = m_pUserKey->GetInt(SYSKEY_Mapswitch2);

 	m_SysKey.dlgStates = m_pUserKey->GetInt(SYSKEY_DLGSTATES);
	m_SysKey.Channel = m_pUserKey->GetInt(SYSKEY_CHANNEL);

	m_SysKey.AutoFIRSkill = m_pUserKey->GetInt(SYSKEY_AUTOFIRSKILL);
	m_SysKey.AutoSECSkill = m_pUserKey->GetInt(SYSKEY_AUTOSECSKILL);
	m_SysKey.AutoSYNSkill = m_pUserKey->GetInt(SYSKEY_AUTOSYNSKILL);
	m_SysKey.SystemSetting = m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING);
	m_SysKey.dress = m_pUserKey->GetInt(SYSKEY_DRESS);
	m_SysKey.fatigue = m_pUserKey->GetInt(SYSKEY_FATIGUE);
	SafeCopy(m_SysKey.idHigh, m_pUserKey->GetStr(SYSKEY_IDHIGH), 20);
	m_SysKey.idLow = m_pUserKey->GetInt(SYSKEY_IDLOW);
	SafeCopy(m_SysKey.realname, m_pUserKey->GetStr(SYSKEY_REALNAME), 15);
	m_SysKey.autofmpmed3 = m_pUserKey->GetInt(SYSKEY_AUTOFMPMED3);
	m_SysKey.autofeudhpmed1 = m_pUserKey->GetInt(SYSKEY_AUTOFEUDHPMED1);
	m_SysKey.autofeudhpmed2 = m_pUserKey->GetInt(SYSKEY_AUTOFEUDHPMED2);
	m_SysKey.autofeudhpmed3 = m_pUserKey->GetInt(SYSKEY_AUTOFEUDHPMED3);
	//////////////////////////挂机血量低于多少的时候吃药
	m_SysKey.autofeathp = m_pUserKey->GetInt(SYSKEY_AUTOFEATHP);
	m_SysKey.autofeatmp = m_pUserKey->GetInt(SYSKEY_AUTOFEATMP);
	m_SysKey.autofeateudhp = m_pUserKey->GetInt(SYSKEY_AUTOFEATEUDHP);
	m_SysKey.TreasureCost  = m_pUserKey->GetInt(SYSKEY_TREASURECOST);
	m_SysKey.nVasTicket = m_pUserKey->GetInt(SYSKEY_VASTICKET);
	m_SysKey.id         = m_pUserKey->GetID();

	return true;
}

bool  CUserKey::SendKey(CUser *pUser)
{
	if(!pUser)
		return false;

	CHECKF(m_pUserKey);

	CMsgUserKey msg;
	if(msg.Create(USERKEY_SEND,m_SysKey))
		pUser->SendMsg(&msg);
	
	if(pUser->isFatigue())
	{
		if ((pUser->GetOnlineTime()-pUser->GetAllOnlineTime()) > 8*60*60)
		{
			pUser->setFatiTime(300);
		}
	}

	return true;
}

bool CUserKey::SaveKey(struct UserKey &KeyInfo,bool bUpdate/* = false*/,bool bInsert)
{
	if(!m_pUserKey)
		return false;

	if(!bInsert)
	{
		m_pUserKey->SetInt(SYSKEY_FeelSwitch, KeyInfo.FeelSwitch);
		m_pUserKey->SetInt(SYSKEY_MusicSwitch, KeyInfo.MusicSwitch);
		m_pUserKey->SetInt(SYSKEY_OvlSwitch, KeyInfo.OvlSwitch);
		m_pUserKey->SetInt( SYSKEY_USERID,KeyInfo.userId );
		m_pUserKey->SetInt( SYSKEY_KEY1,KeyInfo.key1 );
		m_pUserKey->SetInt( SYSKEY_KEY2,KeyInfo.key2 );
		m_pUserKey->SetInt( SYSKEY_KEY3,KeyInfo.key3 );
		m_pUserKey->SetInt( SYSKEY_KEY4,KeyInfo.key4 );
		m_pUserKey->SetInt( SYSKEY_KEY5,KeyInfo.key5 );
		m_pUserKey->SetInt( SYSKEY_KEY6,KeyInfo.key6 );
		m_pUserKey->SetInt( SYSKEY_KEY7,KeyInfo.key7 );
		m_pUserKey->SetInt( SYSKEY_KEY8,KeyInfo.key8 );
		m_pUserKey->SetInt( SYSKEY_KEYF1,KeyInfo.keyF1 );
		m_pUserKey->SetInt( SYSKEY_KEYF2,KeyInfo.keyF2 );
		m_pUserKey->SetInt( SYSKEY_KEYF3,KeyInfo.keyF3 );
		m_pUserKey->SetInt( SYSKEY_KEYF4,KeyInfo.keyF4 );
		m_pUserKey->SetInt( SYSKEY_KEYF5,KeyInfo.keyF5 );
		m_pUserKey->SetInt( SYSKEY_KEYF6,KeyInfo.keyF6 );
		m_pUserKey->SetInt( SYSKEY_KEYF7,KeyInfo.keyF7 );
		m_pUserKey->SetInt( SYSKEY_KEYF8,KeyInfo.keyF8 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEY1,KeyInfo.sec_key1 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEY2,KeyInfo.sec_key2 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEY3,KeyInfo.sec_key3 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEY4,KeyInfo.sec_key4 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEY5,KeyInfo.sec_key5 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEY6,KeyInfo.sec_key6 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEY7,KeyInfo.sec_key7 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEY8,KeyInfo.sec_key8 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF1,KeyInfo.sec_keyF1 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF2,KeyInfo.sec_keyF2 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF3,KeyInfo.sec_keyF3 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF4,KeyInfo.sec_keyF4 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF5,KeyInfo.sec_keyF5 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF6,KeyInfo.sec_keyF6 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF7,KeyInfo.sec_keyF7 );
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF8,KeyInfo.sec_keyF8 );
		m_pUserKey->SetInt( SYSKEY_MapSwitch1, KeyInfo.MapSwitch1);
		m_pUserKey->SetInt( SYSKEY_Mapswitch2, KeyInfo.MapSwitch2);
		m_pUserKey->SetInt( SYSKEY_DLGSTATES, KeyInfo.dlgStates);
		m_pUserKey->SetInt(SYSKEY_AUTOFIRSKILL, KeyInfo.AutoFIRSkill);
		m_pUserKey->SetInt(SYSKEY_AUTOSECSKILL, KeyInfo.AutoSECSkill);
		m_pUserKey->SetInt(SYSKEY_AUTOSYNSKILL, KeyInfo.AutoSYNSkill);
		m_pUserKey->SetInt( SYSKEY_SYSTEMSETTING, KeyInfo.SystemSetting);

		m_bNeedUpdate = false;
		m_pUserKey->ClearUpdateFlags();
		return true;
	}

	if(KeyInfo.FeelSwitch != m_pUserKey->GetInt(SYSKEY_FeelSwitch))
	{
		m_pUserKey->SetInt(SYSKEY_FeelSwitch, KeyInfo.FeelSwitch);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.MusicSwitch != m_pUserKey->GetInt(SYSKEY_MusicSwitch))
	{
		m_pUserKey->SetInt(SYSKEY_MusicSwitch, KeyInfo.MusicSwitch);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.OvlSwitch != m_pUserKey->GetInt(SYSKEY_OvlSwitch))
	{
		m_pUserKey->SetInt(SYSKEY_OvlSwitch, KeyInfo.OvlSwitch);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.userId != m_pUserKey->GetInt(SYSKEY_USERID))
	{
		m_pUserKey->SetInt( SYSKEY_USERID,KeyInfo.userId );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.key1 != m_pUserKey->GetInt(SYSKEY_KEY1))
	{
		m_pUserKey->SetInt( SYSKEY_KEY1,KeyInfo.key1 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.key2 != m_pUserKey->GetInt(SYSKEY_KEY2))
	{
		m_pUserKey->SetInt( SYSKEY_KEY2,KeyInfo.key2 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.key3 != m_pUserKey->GetInt(SYSKEY_KEY3))
	{
		m_pUserKey->SetInt( SYSKEY_KEY3,KeyInfo.key3 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.key4 != m_pUserKey->GetInt(SYSKEY_KEY4))
	{
		m_pUserKey->SetInt( SYSKEY_KEY4,KeyInfo.key4 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.key5 != m_pUserKey->GetInt(SYSKEY_KEY5))
	{
		m_pUserKey->SetInt( SYSKEY_KEY5,KeyInfo.key5 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.key6 != m_pUserKey->GetInt(SYSKEY_KEY6))
	{
		m_pUserKey->SetInt( SYSKEY_KEY6,KeyInfo.key6 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.key7 != m_pUserKey->GetInt(SYSKEY_KEY7))
	{
		m_pUserKey->SetInt( SYSKEY_KEY7,KeyInfo.key7 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.key8 != m_pUserKey->GetInt(SYSKEY_KEY8))
	{
		m_pUserKey->SetInt( SYSKEY_KEY8,KeyInfo.key8 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.keyF1 != m_pUserKey->GetInt(SYSKEY_KEYF1))
	{
		m_pUserKey->SetInt( SYSKEY_KEYF1,KeyInfo.keyF1 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.keyF2 != m_pUserKey->GetInt(SYSKEY_KEYF2))
	{
		m_pUserKey->SetInt( SYSKEY_KEYF2,KeyInfo.keyF2 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.keyF3 != m_pUserKey->GetInt(SYSKEY_KEYF3))
	{
		m_pUserKey->SetInt( SYSKEY_KEYF3,KeyInfo.keyF3 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.keyF4 != m_pUserKey->GetInt(SYSKEY_KEYF4))
	{
		m_pUserKey->SetInt( SYSKEY_KEYF4,KeyInfo.keyF4 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.keyF5 != m_pUserKey->GetInt(SYSKEY_KEYF5))
	{
		m_pUserKey->SetInt( SYSKEY_KEYF5,KeyInfo.keyF5 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.keyF6 != m_pUserKey->GetInt(SYSKEY_KEYF6))
	{
		m_pUserKey->SetInt( SYSKEY_KEYF6,KeyInfo.keyF6 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.keyF7 != m_pUserKey->GetInt(SYSKEY_KEYF7))
	{
		m_pUserKey->SetInt( SYSKEY_KEYF7,KeyInfo.keyF7 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.keyF8 != m_pUserKey->GetInt(SYSKEY_KEYF8))
	{
		m_pUserKey->SetInt( SYSKEY_KEYF8,KeyInfo.keyF8 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_key1 != m_pUserKey->GetInt(SYSKEY_SEC_KEY1))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEY1,KeyInfo.sec_key1 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_key2 != m_pUserKey->GetInt(SYSKEY_SEC_KEY2))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEY2,KeyInfo.sec_key2 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_key3 != m_pUserKey->GetInt(SYSKEY_SEC_KEY3))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEY3,KeyInfo.sec_key3 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_key4 != m_pUserKey->GetInt(SYSKEY_SEC_KEY4))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEY4,KeyInfo.sec_key4 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_key5 != m_pUserKey->GetInt(SYSKEY_SEC_KEY5))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEY5,KeyInfo.sec_key5 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_key6 != m_pUserKey->GetInt(SYSKEY_SEC_KEY6))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEY6,KeyInfo.sec_key6 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_key7 != m_pUserKey->GetInt(SYSKEY_SEC_KEY7))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEY7,KeyInfo.sec_key7 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_key8 != m_pUserKey->GetInt(SYSKEY_SEC_KEY8))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEY8,KeyInfo.sec_key8 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_keyF1 != m_pUserKey->GetInt(SYSKEY_SEC_KEYF1))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF1,KeyInfo.sec_keyF1 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.FeelSwitch != m_pUserKey->GetInt(SYSKEY_FeelSwitch))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF2,KeyInfo.sec_keyF2 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_keyF3 != m_pUserKey->GetInt(SYSKEY_SEC_KEYF3))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF3,KeyInfo.sec_keyF3 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_keyF4 != m_pUserKey->GetInt(SYSKEY_SEC_KEYF4))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF4,KeyInfo.sec_keyF4 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_keyF5 != m_pUserKey->GetInt(SYSKEY_SEC_KEYF5))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF5,KeyInfo.sec_keyF5 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_keyF6 != m_pUserKey->GetInt(SYSKEY_SEC_KEYF6))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF6,KeyInfo.sec_keyF6 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_keyF7 != m_pUserKey->GetInt(SYSKEY_SEC_KEYF7))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF7,KeyInfo.sec_keyF7 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.sec_keyF8 != m_pUserKey->GetInt(SYSKEY_SEC_KEYF8))
	{
		m_pUserKey->SetInt( SYSKEY_SEC_KEYF8,KeyInfo.sec_keyF8 );
		m_bNeedUpdate = true;
	}
	if(KeyInfo.MapSwitch1 != m_pUserKey->GetInt(SYSKEY_MapSwitch1))
	{
		m_pUserKey->SetInt(SYSKEY_MapSwitch1, KeyInfo.MapSwitch1);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.MapSwitch2 != m_pUserKey->GetInt(SYSKEY_Mapswitch2))
	{
		m_pUserKey->SetInt(SYSKEY_Mapswitch2, KeyInfo.MapSwitch2);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.dlgStates != m_pUserKey->GetInt(SYSKEY_DLGSTATES))
	{
		m_pUserKey->SetInt(SYSKEY_DLGSTATES, KeyInfo.dlgStates);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.dlgStates != m_pUserKey->GetInt(SYSKEY_DLGSTATES))
	{
		m_pUserKey->SetInt(SYSKEY_DLGSTATES, KeyInfo.dlgStates);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.Channel != m_pUserKey->GetInt(SYSKEY_CHANNEL))
	{
		m_pUserKey->SetInt(SYSKEY_CHANNEL, KeyInfo.Channel);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.AutoFIRSkill != m_pUserKey->GetInt(SYSKEY_AUTOFIRSKILL))
	{
		m_pUserKey->SetInt(SYSKEY_AUTOFIRSKILL, KeyInfo.AutoFIRSkill);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.AutoSECSkill != m_pUserKey->GetInt(SYSKEY_AUTOSECSKILL))
	{
		m_pUserKey->SetInt(SYSKEY_AUTOSECSKILL, KeyInfo.AutoSECSkill);
		m_bNeedUpdate = true;
	}
	if(KeyInfo.AutoSYNSkill != m_pUserKey->GetInt(SYSKEY_AUTOSYNSKILL))
	{
		m_pUserKey->SetInt(SYSKEY_AUTOSYNSKILL, KeyInfo.AutoSYNSkill);
		m_bNeedUpdate = true;
	}

	//goto 不能在这里改动物品锁开关，不安全
	OBJID temp = KeyInfo.SystemSetting;
	if (IsLocked())
		temp |= USERCONFIG_ITEMLOCK;
	else
		temp &= ~USERCONFIG_ITEMLOCK;
	if(temp != m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING))
	{
		m_pUserKey->SetInt( SYSKEY_SYSTEMSETTING, temp);
		m_bNeedUpdate = true;
	}


	if(bUpdate)
		m_pUserKey->Update();

	return true;
}

void  CUserKey::SetInt(SYSKEY idx,int nData,bool bUpdate /*= false*/)
{
	switch(idx)
	{
	case SYSKEY_FeelSwitch:				// 心情开关
		if (m_SysKey.FeelSwitch==nData)
			return;
		
		m_SysKey.FeelSwitch = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_MusicSwitch:			// 音乐开关
		if (m_SysKey.MusicSwitch==nData)
			return;
		
		m_SysKey.MusicSwitch = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_OvlSwitch:				// 音效开关
		if (m_SysKey.OvlSwitch==nData)
			return;
		
		m_SysKey.OvlSwitch = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_USERID:			        // 用户ID
		if (m_SysKey.userId==nData)
			return;
		
		m_SysKey.userId = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEY1:				    // 用户按键1
		if (m_SysKey.key1==nData)
			return;
		
		m_SysKey.key1 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEY2:				    // 用户按键2
		if (m_SysKey.key2==nData)
			return;
		
		m_SysKey.key2 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEY3:				    // 用户按键3
		if (m_SysKey.key3==nData)
			return;
		
		m_SysKey.key3 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEY4:				    // 用户按键4
		if (m_SysKey.key4==nData)
			return;
		
		m_SysKey.key4 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEY5:				    // 用户按键5
		if (m_SysKey.key5==nData)
			return;
		
		m_SysKey.key5 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEY6:				    // 用户按键6
		if (m_SysKey.key6==nData)
			return;
		
		m_SysKey.key6 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEY7:				    // 用户按键7
		if (m_SysKey.key7==nData)
			return;
		
		m_SysKey.key7 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEY8:				    // 用户按键8
		if (m_SysKey.key8==nData)
			return;
		
		m_SysKey.key8 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEYF1:				    // 用户按键F1
		if (m_SysKey.keyF1==nData)
			return;
		
		m_SysKey.keyF1 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEYF2:				    // 用户按键F2
		if (m_SysKey.keyF2==nData)
			return;
		
		m_SysKey.keyF2 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEYF3:				    // 用户按键F3
		if (m_SysKey.keyF3==nData)
			return;
		
		m_SysKey.keyF3 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEYF4:				    // 用户按键F4
		if (m_SysKey.keyF4==nData)
			return;
		
		m_SysKey.keyF4 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEYF5:				    // 用户按键F5
		if (m_SysKey.keyF5==nData)
			return;
		
		m_SysKey.keyF5 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEYF6:				    // 用户按键F6
		if (m_SysKey.keyF6==nData)
			return;
		
		m_SysKey.keyF6 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEYF7:				    // 用户按键F7
		if (m_SysKey.keyF7==nData)
			return;
		
		m_SysKey.keyF7 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_KEYF8:				    // 用户按键F8
		if (m_SysKey.keyF8==nData)
			return;
		
		m_SysKey.keyF8 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEY1:				// 用户副快捷栏按键1
		if (m_SysKey.sec_key1==nData)
			return;
		
		m_SysKey.sec_key1 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEY2:				// 用户副快捷栏按键2
		if (m_SysKey.sec_key2==nData)
			return;
		
		m_SysKey.sec_key2 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEY3:				// 用户副快捷栏按键3
		if (m_SysKey.sec_key3==nData)
			return;
		
		m_SysKey.sec_key3 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEY4:				// 用户副快捷栏按键4
		if (m_SysKey.sec_key4==nData)
			return;
		
		m_SysKey.sec_key4 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEY5:				// 用户副快捷栏按键5
		if (m_SysKey.sec_key5==nData)
			return;
		
		m_SysKey.sec_key5 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEY6:				// 用户副快捷栏按键6
		if (m_SysKey.sec_key6==nData)
			return;
		
		m_SysKey.sec_key6 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEY7:				// 用户副快捷栏按键7
		if (m_SysKey.sec_key7==nData)
			return;
		
		m_SysKey.sec_key7 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEY8:				// 用户副快捷栏按键8
		if (m_SysKey.sec_key8==nData)
			return;
		
		m_SysKey.sec_key8 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEYF1:				// 用户副快捷栏按键F1
		if (m_SysKey.sec_keyF1==nData)
			return;
		
		m_SysKey.sec_keyF1 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEYF2:				// 用户副快捷栏按键F2
		if (m_SysKey.sec_keyF2==nData)
			return;
		
		m_SysKey.sec_keyF2 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEYF3:				// 用户副快捷栏按键F3
		if (m_SysKey.sec_keyF3==nData)
			return;
		
		m_SysKey.sec_keyF3 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEYF4:				// 用户副快捷栏按键F4
		if (m_SysKey.sec_keyF4==nData)
			return;
		
		m_SysKey.sec_keyF4 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEYF5:				// 用户副快捷栏按键F5
		if (m_SysKey.sec_keyF5==nData)
			return;
		
		m_SysKey.sec_keyF5 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEYF6:				// 用户副快捷栏按键F6
		if (m_SysKey.sec_keyF6==nData)
			return;
		
		m_SysKey.sec_keyF6 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEYF7:				// 用户副快捷栏按键F7
		if (m_SysKey.sec_keyF7==nData)
			return;
		
		m_SysKey.sec_keyF7 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SEC_KEYF8:				// 用户副快捷栏按键F8
		if (m_SysKey.sec_keyF8==nData)
			return;
		
		m_SysKey.sec_keyF8 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_MapSwitch1:				// 探索地图1
		if (m_SysKey.MapSwitch1==nData)
			return;
		
		m_SysKey.MapSwitch1 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_Mapswitch2:				// 探索地图2
		if (m_SysKey.MapSwitch2==nData)
			return;
		
		m_SysKey.MapSwitch2 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_DLGSTATES:				// DLG相关
		if (m_SysKey.dlgStates==nData)
			return;
		
		m_SysKey.dlgStates = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_CHANNEL:
		if (m_SysKey.Channel==nData)
			return;
		
		m_SysKey.Channel = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOFIRSKILL:			//自动释放技能主职业
		if (m_SysKey.AutoFIRSkill==nData)
			return;
		
		m_SysKey.AutoFIRSkill = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOSECSKILL:			//自动释放技能副职业
		if (m_SysKey.AutoSECSkill==nData)
			return;
		
		m_SysKey.AutoSECSkill = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOSYNSKILL:			//自动释放帮派技能
		if (m_SysKey.AutoSYNSkill==nData)
			return;
		
		m_SysKey.AutoSYNSkill = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_SYSTEMSETTING:			//系统设置
		if (m_SysKey.SystemSetting==nData)
			return;
		
		m_SysKey.SystemSetting = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_DRESS:
		if (m_SysKey.dress==nData)
			return;
		
		m_SysKey.dress = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_FATIGUE:
		if (m_SysKey.fatigue==nData)
			return;
		
		m_SysKey.fatigue = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_IDLOW:
		if (m_SysKey.idLow==nData)
			return;
		
		m_SysKey.idLow = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOFMPMED3:
		if (m_SysKey.autofmpmed3==nData)
			return;
		
		m_SysKey.autofmpmed3 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOFEUDHPMED1:
		if (m_SysKey.autofeudhpmed1==nData)
			return;
		
		m_SysKey.autofeudhpmed1 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOFEUDHPMED2:
		if (m_SysKey.autofeudhpmed2==nData)
			return;
		
		m_SysKey.autofeudhpmed2 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOFEUDHPMED3:
		if (m_SysKey.autofeudhpmed3==nData)
			return;
		
		m_SysKey.autofeudhpmed3 = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOFEATHP:
		if (m_SysKey.autofeathp==nData)
			return;
		
		m_SysKey.autofeathp = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOFEATMP:
		if (m_SysKey.autofeatmp==nData)
			return;
		
		m_SysKey.autofeatmp = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_AUTOFEATEUDHP:
		if (m_SysKey.autofeateudhp==nData)
			return;
		
		m_SysKey.autofeateudhp = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_TREASURECOST://记录玩家开箱子开了多少元宝仍然没有出麒麟之魂,后台数据，不用同步到客户端
		if (m_SysKey.TreasureCost==nData)
			return;
		
		m_SysKey.TreasureCost = nData;
		m_bNeedUpdate = true;
		break;
	case SYSKEY_VASTICKET:
		if (m_SysKey.nVasTicket==nData)
			return;
		
		m_SysKey.nVasTicket = nData;
		m_bNeedUpdate = true;
	default:
		break;
	}

	m_pUserKey->SetInt(idx,nData);

	if(bUpdate)
		m_pUserKey->Update();

}

void CUserKey::SetStr(SYSKEY idx, LPCTSTR szData, int nSize, bool bUpdate/* = false*/)
{
	switch(idx)
	{
	case SYSKEY_IDHIGH:
		if(strcmp(m_SysKey.idHigh, szData) == 0)
			return;
		
		strcpy(m_SysKey.idHigh, szData);
		m_bNeedUpdate = true;
		break;
	case SYSKEY_REALNAME:
		if(strcmp(m_SysKey.realname, szData) == 0)
			return;
		
		strcpy(m_SysKey.realname, szData);
		m_bNeedUpdate = true;
		break;
	default:
		break;
	}

	m_pUserKey->SetStr(idx,szData,nSize);
	if(bUpdate)
		m_pUserKey->Update();

}

bool CUserKey::DeleteUserKey(IDatabase *pDb,CUser* pUser)
{
   return m_pUserKey->DeleteRecord();
}

bool CUserKey::SendAutofight(CUser* pUser)
{
// 		m_SysKey.nAction = 2;
// 		m_SysKey.FeelSwitch = m_pUserKey->GetInt(SYSKEY_FeelSwitch);
// 		if ((m_SysKey.FeelSwitch != 1) && (m_SysKey.FeelSwitch != 0 ))
// 		{
// 			m_SysKey.FeelSwitch = 0;
// 		}
// 		m_SysKey.MusicSwitch = m_pUserKey->GetInt(SYSKEY_MusicSwitch);
// 		m_SysKey.OvlSwitch = m_pUserKey->GetInt(SYSKEY_OvlSwitch);
// 	
// 		m_SysKey.userId = m_pUserKey->GetInt(SYSKEY_USERID);
// 		m_SysKey.key1 = m_pUserKey->GetInt(SYSKEY_AUTOFHPMED1);
// 		m_SysKey.key2 = m_pUserKey->GetInt(SYSKEY_AUTOFHPMED2);
// 		m_SysKey.key3 = m_pUserKey->GetInt(SYSKEY_AUTOFHPMED3);
// 		m_SysKey.key4 = m_pUserKey->GetInt(SYSKEY_AUTOFMPMED1);
// 		m_SysKey.key5 = m_pUserKey->GetInt(SYSKEY_AUTOFMPMED2);
// 		m_SysKey.key6 = m_pUserKey->GetInt(SYSKEY_AUTOFMPMED3);
// 		m_SysKey.key7 = m_pUserKey->GetInt(SYSKEY_AUTOFEUDHPMED1);
// 		m_SysKey.key8 = m_pUserKey->GetInt(SYSKEY_AUTOFEUDHPMED2);
// 		
// 		m_SysKey.keyF1 = m_pUserKey->GetInt(SYSKEY_AUTOFEUDHPMED3);
// 		m_SysKey.keyF2 = m_pUserKey->GetInt(SYSKEY_AUTOFEATHP);
// 		m_SysKey.keyF3 = m_pUserKey->GetInt(SYSKEY_AUTOFEATMP);
// 		m_SysKey.keyF4 = m_pUserKey->GetInt(SYSKEY_AUTOFEATEUDHP);
// 
// 
// 		m_SysKey.keyF5 = m_pUserKey->GetInt(SYSKEY_KEYF5);
// 		m_SysKey.keyF6 = m_pUserKey->GetInt(SYSKEY_KEYF6);
// 		m_SysKey.keyF7 = m_pUserKey->GetInt(SYSKEY_KEYF7);
// 		m_SysKey.keyF8 = m_pUserKey->GetInt(SYSKEY_KEYF8);
// 
// 		m_SysKey.sec_key1 = m_pUserKey->GetInt(SYSKEY_SEC_KEY1);
// 		m_SysKey.sec_key2 = m_pUserKey->GetInt(SYSKEY_SEC_KEY2);
// 		m_SysKey.sec_key3 = m_pUserKey->GetInt(SYSKEY_SEC_KEY3);
// 		m_SysKey.sec_key4 = m_pUserKey->GetInt(SYSKEY_SEC_KEY4);
// 		m_SysKey.sec_key5 = m_pUserKey->GetInt(SYSKEY_SEC_KEY5);
// 		m_SysKey.sec_key6 = m_pUserKey->GetInt(SYSKEY_SEC_KEY6);
// 		m_SysKey.sec_key7 = m_pUserKey->GetInt(SYSKEY_SEC_KEY7);
// 		m_SysKey.sec_key8 = m_pUserKey->GetInt(SYSKEY_SEC_KEY8);
// 		
// 		m_SysKey.sec_keyF1 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF1);
// 		m_SysKey.sec_keyF2 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF2);
// 		m_SysKey.sec_keyF3 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF3);
// 		m_SysKey.sec_keyF4 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF4);
// 		m_SysKey.sec_keyF5 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF5);
// 		m_SysKey.sec_keyF6 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF6);
// 		m_SysKey.sec_keyF7 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF7);
// 		m_SysKey.sec_keyF8 = m_pUserKey->GetInt(SYSKEY_SEC_KEYF8);
// 
// 		m_SysKey.MapSwitch1 = m_pUserKey->GetInt(SYSKEY_MapSwitch1);
// 		m_SysKey.MapSwitch2 = m_pUserKey->GetInt(SYSKEY_Mapswitch2);
// 
//  		m_SysKey.dlgStates = m_pUserKey->GetInt(SYSKEY_DLGSTATES);
// 		m_SysKey.Channel = m_pUserKey->GetInt(SYSKEY_CHANNEL);
// 		m_SysKey.AutoFIRSkill = m_pUserKey->GetInt(SYSKEY_AUTOFIRSKILL);
// 		m_SysKey.AutoSECSkill = m_pUserKey->GetInt(SYSKEY_AUTOSECSKILL);
// 		m_SysKey.AutoSYNSkill = m_pUserKey->GetInt(SYSKEY_AUTOSYNSKILL);
// 		m_SysKey.SystemSetting = m_pUserKey->GetInt(SYSKEY_SYSTEMSETTING);
// 
// 		CMsgUserKey msg;
// 		if(msg.Create(USERKEY_REC,m_SysKey))
// 			pUser->SendMsg(&msg);
		
		return true;
}

void CUserKey::SetAutoSkill(OBJID AutoFirSkill, OBJID AutoSecSkill, OBJID AutoSynSkill, CUser* pUser, bool bUpdate)
{
	if(m_pUserKey->GetInt(SYSKEY_AUTOFIRSKILL) != AutoFirSkill)
	{
		m_pUserKey->SetInt(SYSKEY_AUTOFIRSKILL, AutoFirSkill);
		m_bNeedUpdate = true;
	}
	if(m_pUserKey->GetInt(SYSKEY_AUTOFIRSKILL) != AutoSecSkill)
	{
		m_pUserKey->SetInt(SYSKEY_AUTOSECSKILL, AutoSecSkill);
		m_bNeedUpdate = true;
	}
	if(m_pUserKey->GetInt(SYSKEY_AUTOFIRSKILL) != AutoSynSkill)
	{
		m_pUserKey->SetInt(SYSKEY_AUTOSYNSKILL, AutoSynSkill);
		m_bNeedUpdate = true;
	}

	if (bUpdate)
		m_pUserKey->Update();

	SendKey(pUser);
}

void CUserKey::SetFatigue(int nFatigue, bool bUpdate/* = false*/)
{
	if (nFatigue != 0 && nFatigue != 1)
	{
		::LogSave("奇怪的防沉迷验证通过信息！");
		return;	
	}
	if (m_pUserKey)
	{
		if(m_pUserKey->GetInt(SYSKEY_FATIGUE) == nFatigue)
			return;
		
		m_pUserKey->SetInt(SYSKEY_FATIGUE, nFatigue);
		m_bNeedUpdate = true;
	}

	if(bUpdate)
		m_pUserKey->Update();
}

bool CUserKey::SetDress(OBJID nDress, bool uPdate)
{
	if (nDress < 0)
		return false;
	if (m_pUserKey)
	{
		if(m_pUserKey->GetInt(SYSKEY_DRESS) == nDress)
			return true;

		m_pUserKey->SetInt(SYSKEY_DRESS, nDress);
		m_bNeedUpdate = true;
	}

	if(uPdate)
		m_pUserKey->Update();
	return true;
}

DWORD CUserKey::GetDress()
{
	if(m_pUserKey)
		return (m_pUserKey->GetInt(SYSKEY_DRESS));

	return 0;
}

bool CUserKey::SaveInfo()
{
	CHECKF(m_pUserKey);

	if(m_bNeedUpdate)
	{
		m_bNeedUpdate = false;
		return m_pUserKey->Update();
	}
	return true;
}

void CUserKey::SendObjInfo(PROCESS_ID idProcess)
{
	CHECK(m_pUser);
	LoadInfo();
	m_pUser->SendObjInfo(idProcess,m_pUser->GetID(),INFO_KEY,&m_SysKey,sizeof(m_SysKey));
}

void CUserKey::SetObjInfo(SYSTEMUSERKEY* pStInfo)
{
	CHECK(pStInfo);
	CHECK(m_pUser);
	SaveKey(*pStInfo,false,false);
}