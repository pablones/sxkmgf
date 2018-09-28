// MsgTalk.h: interface for the CMsgTalk class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MSGTALK_H
#define _MSGTALK_H

#include "NetMsg.h"
#include "NetStringPacker.h"

WORLDKERNEL_BEGIN
class CMsgTalk : public CNetMsg  
{
public:
	CMsgTalk();
	virtual ~CMsgTalk();
	
	BOOL	Create	(char* pszSpeaker, char* pszHearer, char* pszWords, 
						char* pszEmotion=NULL, DWORD dwColor=0xffffff, 
						USHORT unAttribute=_TXTATR_NORMAL, USHORT unStyle=_TXT_NORMAL,DWORD dwItemID = 0,DWORD dwItemTypeID = 0, char* pszItemName = NULL);

public:
	BOOL			Create		(char* pbufMsg, DWORD dwMsgSize);
	BOOL			Create		(USHORT unAttribute, char* pszWords)
								{ return Create(SYSTEM_NAME, ALLUSERS_NAME, pszWords, NULL, 0xFFFFFF, unAttribute); }
	void			Process		(void *pInfo);
	static void     Dump(char* pbufMsg, DWORD dwMsgSize);///当此消息出现错误时将消息全部内容写到日志中
//private:
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
WORLDKERNEL_END

#endif 
