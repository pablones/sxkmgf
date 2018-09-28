// MsgIssue.h: interface for the CMsgIssue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGISSUE_H__06764C89_F374_42A1_BD8A_FDED275BE356__INCLUDED_)
#define AFX_MSGISSUE_H__06764C89_F374_42A1_BD8A_FDED275BE356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// #include "winsock.h"
#include "NetMsg.h"
WORLDKERNEL_BEGIN

const int _MAX_ISSUETITLESIZE =100;///50汉字标题
const int _MAX_ISSUETEXTSIZE =800;///400汉字正文

enum IssueType
{
	itNone=-1,///未指定
	itRealtime,///即时要求
	itItemReplicate,///道具复制
	itValueExplode,///恶意刷数值
	itAssware,///外挂
	itServerUnstead,///服务器状态异常
	itValueExcept,///关键数据异常
	itBug,///重大Bug(游戏中断)
	itOther,///其他重大紧急异常
	it1v1chat,///聊天整合一起
};

enum 
{
	ISSUEACT_NONE=-1, 
	ISSUEACT_SENDISSUE,				// 客户端发出问题
	ISSUEACT_REQUESTREPLYCHAT,		// GM请求玩家1V1聊天包括应答,在Result中提示结果
 	ISSUEACT_SYSMAIL,				// 问题系统专用的系统邮件
 	ISSUEACT_CHATTEXT, // 返回其他玩家装备// id is user id
};

class CMsgIssue : public world_kernel::CNetMsg  
{
public:
	CMsgIssue();
	virtual ~CMsgIssue();

	///组包函数
	//BOOL	Create	(const char* pszAccount, const char* pszPsw, const char* pszServer);
public:	
	BOOL	SendIssue(LPCTSTR strPlayerName,UINT nPlayerID,LPCTSTR strTitle,IssueType itType,LPCTSTR strText);

	///解包函数
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

protected:
	BOOL	CreateIssue(LPCTSTR strPlayerName,UINT nPlayerID,LPCTSTR strTitle,IssueType itType,LPCTSTR strText);

	typedef struct {
// 		unsigned short	unMsgSize;
// 		unsigned short	unMsgType;
		UINT		usAction;			//消息类型,请求服务,回应接触,聊天
		UINT		usPlayerID;			//玩家playerid
		UINT		usRequestID;		///请求者id
		UINT		usType;				//问题类型
		UINT		usResult;			//保存结果
		union
		{
			struct
			{
				char		usTitle[_MAX_ISSUETITLESIZE];			//标题
				char		usText[_MAX_ISSUETEXTSIZE];			//正文
				char		usPlayerName[_MAX_NAMESIZE];	//玩家名
			};
			char	usChatText[_MAX_ISSUETITLESIZE+_MAX_ISSUETEXTSIZE+_MAX_NAMESIZE];	//玩家名
		};
	}MSG_Info;

	MSG_Info*	m_pInfo;

};

WORLDKERNEL_END
#endif // !defined(AFX_MSGISSUE_H__06764C89_F374_42A1_BD8A_FDED275BE356__INCLUDED_)
