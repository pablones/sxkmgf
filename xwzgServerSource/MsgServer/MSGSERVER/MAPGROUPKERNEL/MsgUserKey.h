// MsgUserKey.h: interface for the CMsgUserKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGUSERKEY_H__B5CD0262_02F1_454D_8342_F2B91E962474__INCLUDED_)
#define AFX_MSGUSERKEY_H__B5CD0262_02F1_454D_8342_F2B91E962474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

enum {
	USERKEY_SEND,
	USERKEY_REC,
	USERKEY_MODIFY,
};

struct UserKey;

class CMsgUserKey : public CNetMsg  
{
public:
	CMsgUserKey();
	virtual ~CMsgUserKey();


	BOOL Create(char* pbufMsg, DWORD dwMsgSize);
	BOOL Create(int nAction,struct UserKey &Info);
	void Process(void *pInfo);
private:
	typedef struct
	{
		MSGHEAD_DEFINE
		char			nAction;
		char			FeelSwitch;
		char			MusicSwitch;
		char			OvlSwitch;
		OBJID			idUser;
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

//预留
//		OBJID           keyF9;
//		OBJID           keyF10;
//		OBJID           keyF11;

// 		OBJID           keyA1;
// 		OBJID           keyA2;
// 		OBJID           keyA3;
// 		OBJID           keyA4;
// 		OBJID           keyA5;
// 		OBJID           keyA6;
// 		OBJID           keyA7;
// 		OBJID           keyA8;
 		OBJID			dlgStates;
		OBJID			Channel;

		OBJID  AutoFIRSkill;	//默认值 32个1
		OBJID  AutoSECSkill;
		OBJID  AutoSYNSkill;
		OBJID  SystemSetting;  //0
//		char            szBuf[1];
	}MSG_Info;
	
	MSG_Info *m_pInfo;

// private:
// 	CNetStringPacker    m_StrPacker;
};

#endif // !defined(AFX_MSGUSERKEY_H__B5CD0262_02F1_454D_8342_F2B91E962474__INCLUDED_)
