// MsgAccLock.h: interface for the CMsgAccLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGACCLOCK_H__7C2588AD_2392_4F95_9BDF_5166CDCA6EEF__INCLUDED_)
#define AFX_MSGACCLOCK_H__7C2588AD_2392_4F95_9BDF_5166CDCA6EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

const USHORT _MSGACCLOCK_CHGPWD	= 1;//修改密码
const USHORT _MSGACCLOCK_LOCK	= 2;//加锁
const USHORT _MSGACCLOCK_UNLOCK	= 3;//解锁
const USHORT _MSGACCLOCK_KEY	= 4;//加锁种子
const char	 _MSGACCLOCK_KEYCHAR[24] = "10316E028C8F3B5Q";
const USHORT _MSGACCLOCK_RENAME = 5;//改名
const USHORT _MSGACCLOCK_RENAME_WORLD=6;//世界发的改名
const USHORT _MSGACCLOCK_RENAME_OTHER=7;
const USHORT _MSGACCLOCK_VERIFICATION_CODE =8;		//跑商验证码返回
const USHORT _MSGACCLOCK_VERIFICATION_QUERY=9;		//跑商验证码请求	

enum
{
	_MSGACCLOCK_RENAME_NAME=0,
	_MSGACCLOCK_RENAME_SYNNAME=1,
};
class CMsgAccLock : public CNetMsg  
{
public:
	CMsgAccLock();
	virtual ~CMsgAccLock();

	BOOL Create(OBJID idUser, USHORT usAction, USHORT usData);
	BOOL Create(OBJID idUser, USHORT usAction, USHORT usData, char* szOldPwd, char* szNewPwd);
	void Process(void *pInfo);
private:
	typedef struct {
		MSGHEAD_DEFINE
		
		OBJID	idUser;
		USHORT	usAction;
		USHORT	usData;
		char	szOldPwd[24];
		char	szNewPwd[24];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGACCLOCK_H__7C2588AD_2392_4F95_9BDF_5166CDCA6EEF__INCLUDED_)
