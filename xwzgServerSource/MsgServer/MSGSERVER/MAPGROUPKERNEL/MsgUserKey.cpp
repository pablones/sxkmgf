// MsgUserKey.cpp: implementation of the CMsgUserKey class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgUserKey.h"
#include "UserKey.h"
#include "MapGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgUserKey::CMsgUserKey()
{
	Init();
	m_pInfo = (MSG_Info*)m_bufMsg;

//	m_StrPacker.SetBuf(m_pInfo->szBuf,_MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgUserKey::~CMsgUserKey()
{

}

BOOL CMsgUserKey::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!CNetMsg::Create(pbufMsg,dwMsgSize))
		return false;

	if(_MSG_USERKEY != this->GetType())
		return false;

	return true;
}

 BOOL CMsgUserKey::Create(int nAction,SYSTEMUSERKEY &Info)
{
	if( sizeof(SYSTEMUSERKEY) > 1024)
		return false;
	//_MSG_GENERAL

	m_unMsgType = _MSG_USERKEY;
	BOOL bSucMake	=true;
	 	
//  bSucMake	&= m_StrPacker.AddString((char*)Info.nAction);
// 	bSucMake	&= m_StrPacker.AddString((char*)Info.FeelSwitch);
// 	bSucMake	&= m_StrPacker.AddString((char*)Info.MusicSwitch);
// 	bSucMake	&= m_StrPacker.AddString((char*)Info.OvlSwitch);

	m_pInfo->nAction = Info.nAction;
	m_pInfo->FeelSwitch = Info.FeelSwitch;
	m_pInfo->MusicSwitch = Info.MusicSwitch;
	m_pInfo->OvlSwitch = Info.OvlSwitch;

	m_pInfo->idUser = Info.userId;
	m_pInfo->key1 = Info.key1;
	m_pInfo->key2 = Info.key2;
	m_pInfo->key3 = Info.key3;
	m_pInfo->key4 = Info.key4;
	m_pInfo->key5 = Info.key5;
	m_pInfo->key6 = Info.key6;
	m_pInfo->key7 = Info.key7;
	m_pInfo->key8 = Info.key8;

	m_pInfo->keyF1 = Info.keyF1;
	m_pInfo->keyF2 = Info.keyF2;
	m_pInfo->keyF3 = Info.keyF3;
	m_pInfo->keyF4 = Info.keyF4;
	m_pInfo->keyF5 = Info.keyF5;
	m_pInfo->keyF6 = Info.keyF6;
	m_pInfo->keyF7 = Info.keyF7;
	m_pInfo->keyF8 = Info.keyF8;

	m_pInfo->sec_key1 = Info.sec_key1;
	m_pInfo->sec_key2 = Info.sec_key2;
	m_pInfo->sec_key3 = Info.sec_key3;
	m_pInfo->sec_key4 = Info.sec_key4;
	m_pInfo->sec_key5 = Info.sec_key5;
	m_pInfo->sec_key6 = Info.sec_key6;
	m_pInfo->sec_key7 = Info.sec_key7;
	m_pInfo->sec_key8 = Info.sec_key8;
	
	m_pInfo->sec_keyF1 = Info.sec_keyF1;
	m_pInfo->sec_keyF2 = Info.sec_keyF2;
	m_pInfo->sec_keyF3 = Info.sec_keyF3;
	m_pInfo->sec_keyF4 = Info.sec_keyF4;
	m_pInfo->sec_keyF5 = Info.sec_keyF5;
	m_pInfo->sec_keyF6 = Info.sec_keyF6;
	m_pInfo->sec_keyF7 = Info.sec_keyF7;
	m_pInfo->sec_keyF8 = Info.sec_keyF8;

	m_pInfo->MapSwitch1 = Info.MapSwitch1;
	m_pInfo->MapSwitch2 = Info.MapSwitch2;
	
	//预留
	//m_pInfo->keyF9 = Info.keyF9;
	//m_pInfo->keyF10 = Info.keyF10;
	//m_pInfo->keyF11 = Info.keyF11;

// 	m_pInfo->keyA1 = Info.keyA1;
// 	m_pInfo->keyA2 = Info.keyA2;
// 	m_pInfo->keyA3 = Info.keyA3;
// 	m_pInfo->keyA4 = Info.keyA4;
// 	m_pInfo->keyA5 = Info.keyA5;
// 	m_pInfo->keyA6 = Info.keyA6;
// 	m_pInfo->keyA7 = Info.keyA7;
// 	m_pInfo->keyA8 = Info.keyA8;
 	m_pInfo->dlgStates = Info.dlgStates;
	m_pInfo->Channel = Info.Channel;

	m_pInfo->AutoFIRSkill = Info.AutoFIRSkill;
	m_pInfo->AutoSECSkill = Info.AutoSECSkill;
	m_pInfo->AutoSYNSkill = Info.AutoSYNSkill;
	m_pInfo->SystemSetting = Info.SystemSetting;
// 
// 	BOOL bSucMake	=true;
// 	
// 	bSucMake	&= m_StrPacker.AddString((char*)Info.szProtery);     //任务属性
// 	bSucMake	&= m_StrPacker.AddString((char*)Info.szFriend);      //社交
// 	bSucMake	&= m_StrPacker.AddString( ( char* )Info.szItem );   //物品栏
// 	bSucMake	&= m_StrPacker.AddString( ( char* )Info.szBigMap ); //大地图
// 	bSucMake	&= m_StrPacker.AddString( ( char* )Info.szFaction); //军团
// 	bSucMake	&= m_StrPacker.AddString( ( char* )Info.szSkill);   //技能
// 	bSucMake	&= m_StrPacker.AddString( ( char* )Info.szMail);    //邮件
// 	bSucMake	&= m_StrPacker.AddString( ( char* )Info.szTask );   //任务
//  bSucMake    &= m_StrPacker.AddString( (char*)Info.szPick );     //拾取
// 	bSucMake    &= m_StrPacker.AddString( (char*)Info.szPhyAtt  );  //物理攻击
// 	bSucMake    &= m_StrPacker.AddString( (char*)Info.szRunToWalk  ); //走跑切换
// 	bSucMake    &= m_StrPacker.AddString( (char*)Info.szChgTarget  ); //改变攻击目标
// 	bSucMake    &= m_StrPacker.AddString( (char*)Info.szSysSetting  ); //系统设置
// 	bSucMake    &= m_StrPacker.AddString( (char*)Info.szTeam  );        //组队
// 	bSucMake    &= m_StrPacker.AddString( (char*)Info.szWisdomSetting);  //生活技能
// 	bSucMake    &= m_StrPacker.AddString( (char*)Info.szAutoAtt );     //问天指南
// 	bSucMake    &= m_StrPacker.AddString( (char*)Info.szGift  );       //天赋
// 	bSucMake    &= m_StrPacker.AddString((char*)Info.szAutoFight);      //开始自动打怪
// 	bSucMake    &= m_StrPacker.AddString((char*)Info.szBeginFight);    //开始挂机
	m_unMsgSize = sizeof(MSG_Info)/*-1+m_StrPacker.GetSize()*/;
	return true;

}
 
void CMsgUserKey::Process(void *pInfo) 
{
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;
// 		  CGameUserKey *m_pUserKey;
	if(!pUser->CanBeSee())
		return;

	switch (m_pInfo->nAction)
	{
	case USERKEY_SEND:
		{
		  struct UserKey keyInfo;
		  keyInfo.FeelSwitch = pUser->GetFeelSwitch();
		  keyInfo.MusicSwitch = m_pInfo->MusicSwitch;
		  keyInfo.OvlSwitch = m_pInfo->OvlSwitch;

		  keyInfo.userId = m_pInfo->idUser;
		  keyInfo.key1 = m_pInfo->key1;
		  keyInfo.key2 = m_pInfo->key2;
		  keyInfo.key3 = m_pInfo->key3;
		  keyInfo.key4 = m_pInfo->key4;
		  keyInfo.key5 = m_pInfo->key5;
		  keyInfo.key6 = m_pInfo->key6;
		  keyInfo.key7 = m_pInfo->key7;
		  keyInfo.key8 = m_pInfo->key8;

		  keyInfo.keyF1 = m_pInfo->keyF1;
		  keyInfo.keyF2 = m_pInfo->keyF2;
		  keyInfo.keyF3 = m_pInfo->keyF3;
		  keyInfo.keyF4 = m_pInfo->keyF4;
		  keyInfo.keyF5 = m_pInfo->keyF5;
		  keyInfo.keyF6 = m_pInfo->keyF6;
		  keyInfo.keyF7 = m_pInfo->keyF7;
		  keyInfo.keyF8 = m_pInfo->keyF8;

		  keyInfo.sec_key1 = m_pInfo->sec_key1;
		  keyInfo.sec_key2 = m_pInfo->sec_key2;
		  keyInfo.sec_key3 = m_pInfo->sec_key3;
		  keyInfo.sec_key4 = m_pInfo->sec_key4;
		  keyInfo.sec_key5 = m_pInfo->sec_key5;
		  keyInfo.sec_key6 = m_pInfo->sec_key6;
		  keyInfo.sec_key7 = m_pInfo->sec_key7;
		  keyInfo.sec_key8 = m_pInfo->sec_key8;
		  
		  keyInfo.sec_keyF1 = m_pInfo->sec_keyF1;
		  keyInfo.sec_keyF2 = m_pInfo->sec_keyF2;
		  keyInfo.sec_keyF3 = m_pInfo->sec_keyF3;
		  keyInfo.sec_keyF4 = m_pInfo->sec_keyF4;
		  keyInfo.sec_keyF5 = m_pInfo->sec_keyF5;
		  keyInfo.sec_keyF6 = m_pInfo->sec_keyF6;
		  keyInfo.sec_keyF7 = m_pInfo->sec_keyF7;
		  keyInfo.sec_keyF8 = m_pInfo->sec_keyF8;
		
		  keyInfo.MapSwitch1 = m_pInfo->MapSwitch1;
		  keyInfo.MapSwitch2 = m_pInfo->MapSwitch2;
				  

 		  keyInfo.dlgStates = m_pInfo->dlgStates;
		  keyInfo.Channel = m_pInfo->Channel;

		  keyInfo.AutoFIRSkill = m_pInfo->AutoFIRSkill;
		  keyInfo.AutoSECSkill = m_pInfo->AutoSECSkill;
		  keyInfo.AutoSYNSkill = m_pInfo->AutoSYNSkill;
		  keyInfo.SystemSetting = m_pInfo->SystemSetting;
		  DEBUG_TRY
	      pUser->SaveUserKey(keyInfo);
		  DEBUG_CATCH("pUser->SaveUserKey(keyInfo,true);")
 		}
 		break; 
 	case USERKEY_MODIFY:
 /*		pUser -> SetUserKey(0, 0, true);*/
 		break;
	}

}
