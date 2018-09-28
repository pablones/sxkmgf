//		MSGHEAD_DEFINE

// MsgTalk.h: interface for the CMsgTalk class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MSGTALK_H
#define _MSGTALK_H

#include "NetMsg.h"
#include "NetStringPacker.h"

#define		SYSTEM_NAME 		"SYSTEM"			// �㲥��Ϣʱ����ϵͳ�������ִ�
#define		ALLUSERS_NAME		"ALLUSER"			// TALK�д�����ҡ������ִ�
#define		REPLAY_OK_STR		"ANSWER_OK"			// �������ʺŻ�LOGINʱ���صĳɹ���־����
#define		REPLAY_AGAIN_STR	"ANSWER_AGAIN"		// NPC_SERVER LOGINʱ���ص��ظ���¼��־����
#define		SYS_SCORL			"SYSSCORL"
#define		SYS_ANNOUNCE		"SYSANNOUNCE"		//ȫ��ϵͳ����
#define		STR_TARGET_OFFLINE	"�Է������ߣ����ڸ����С�"
const int _MAX_FEELSIZE = 16;
const unsigned short _TXTATR_MSG_SERVERLIST	= _TXTATR_NORMAL + 208;//�ڿ�ʼ��Ϸʱ�õ�..
const unsigned short _TXTATR_CHATWITH	=_TXTATR_NORMAL+19;//�������[2008/12/23]1:1����

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
		DWORD  dwUseGMToolPsw;		//�Ƿ���GMTOOL��TALK���� ����070602
		DWORD  dwItemID;
		DWORD  dwItemTypeID;
		char	szBuf[1];
	
	}MSG_Info;

	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;

};

#endif 
