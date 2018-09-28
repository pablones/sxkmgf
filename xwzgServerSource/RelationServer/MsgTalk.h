//		MSGHEAD_DEFINE

// MsgTalk.h: interface for the CMsgTalk class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MSGTALK_H
#define _MSGTALK_H

#include "NetMsg.h"
#include "NetStringPacker.h"

#define		SYSTEM_NAME 		"SYSTEM"			// 广播消息时代表“系统”的名字串
#define		ALLUSERS_NAME		"ALLUSER"			// TALK中代表“大家”的名字串
#define		REPLAY_OK_STR		"ANSWER_OK"			// 申请新帐号或LOGIN时返回的成功标志串。
#define		REPLAY_AGAIN_STR	"ANSWER_AGAIN"		// NPC_SERVER LOGIN时返回的重复登录标志串。
#define		SYS_SCORL			"SYSSCORL"
#define		SYS_ANNOUNCE		"SYSANNOUNCE"		//全服系统公告
#define		STR_TARGET_OFFLINE	"对方不在线，或在副本中。"
const int _MAX_FEELSIZE = 16;
const unsigned short _TXTATR_MSG_SERVERLIST	= _TXTATR_NORMAL + 208;//在开始游戏时用到..
const unsigned short _TXTATR_CHATWITH	=_TXTATR_NORMAL+19;//独孤求败[2008/12/23]1:1聊天

class CMsgTalk : public CNetMsg  
{
public:
	CMsgTalk();
	virtual ~CMsgTalk();
	
	BOOL	Create	(LPCTSTR pszWords)
					{ return Create(SYSTEM_NAME, ALLUSERS_NAME, pszWords, NULL, 0xFFFFFF, _TXTATR_NORMAL); }
	BOOL	Create	(USHORT unAttribute, LPCTSTR pszWords)
					{ return Create(SYSTEM_NAME, ALLUSERS_NAME, pszWords, NULL, 0xFFFFFF, unAttribute); }
	BOOL	Create	(LPCTSTR pszSpeaker, LPCTSTR pszHearer, LPCTSTR pszWords, 
						LPCTSTR pszEmotion=NULL, DWORD dwColor=0xffffff, 
						USHORT unAttribute=_TXTATR_NORMAL, USHORT unStyle=_TXT_NORMAL,DWORD dwItemID = 0,DWORD dwItemTypeID = 0, LPCTSTR pszItemName = NULL, DWORD dwUseGMToolPsw=0);
	BOOL	Create	(LPCTSTR pszSpeaker, LPCTSTR pszHearer, LPCTSTR pszWords, 
						LPCTSTR pszEmotion, DWORD dwColor, unsigned short unAttribute,
						unsigned short unStyle,DWORD dwItemID,DWORD dwItemTypeID,LPCTSTR pszItemName,DWORD dwUseGMToolPsw,LPCTSTR szFeel);
public:
	BOOL			Create		(char* pbufMsg, DWORD dwMsgSize);
	void			Process		(void *pInfo);

	void			ReviseMsgType()	{ m_unMsgType	=_MSG_TALK; }
//private:
	typedef struct {

 		unsigned short	unMsgSize;
 		unsigned short	unMsgType;

		DWORD	dwWordsColor;
		USHORT	unTxtAttribute;
		USHORT	unTxtStyle;
		union{
			DWORD	dwTime;
			OBJID	idTransmit;
		};
		DWORD  dwUseGMToolPsw;		//是否是GMTOOL的TALK回送 哈雷070602
		DWORD  dwItemID;
		DWORD  dwItemTypeID;
		char	szBuf[1];
	
	}MSG_Info;

	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;

};

#endif 
