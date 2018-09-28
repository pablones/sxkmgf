		MSGHEAD_DEFINE

// MsgTalk.h: interface for the CMsgTalk class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MSGTALK_H
#define _MSGTALK_H

#include "NetMsg.h"
#include "NetStringPacker.h"

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
	static void		Dump(char* pbufMsg, DWORD dwMsgSize);///当此消息出现错误时将消息全部内容写到日志中
private:
	typedef struct {
		MSGHEAD_DEFINE
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
public:
	MSG_Info& GetInfo()
	{
		return *m_pInfo;
	}
};

#endif 
