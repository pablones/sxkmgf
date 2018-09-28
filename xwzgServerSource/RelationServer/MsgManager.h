// MsgManager.h: interface for the MsgManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMANAGER_H__26E339FB_DAC3_41C9_AD17_76489AB3320B__INCLUDED_)
#define AFX_MSGMANAGER_H__26E339FB_DAC3_41C9_AD17_76489AB3320B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable: 4786)
#include "NetMsg.h"
#include "MsgRelay.h"
#include "MsgTalk.h"
#include "MsgAccount.h"
#include "map"
using namespace std;
#define DEBUG_TRY try{
#define	DEBUG_CATCH(x)	}catch(...){ LOGERROR("CATCH: *** " x " crash()! ***\n"); }

#define URLSTRSIZE		64
typedef char MSGBUF[1024];
const unsigned short _TXTATR_WORLDSERVER		=_TXTATR_NORMAL+30;//小喇叭
const unsigned short _TXTATR_BOOTHNAME			=_TXTATR_NORMAL+31;//摊位名字
const unsigned short _TXTATR_BOOTHDES			=_TXTATR_NORMAL+32;//摊位信息
const unsigned short _TXTATR_BOOTHLEAVEWORD		=_TXTATR_NORMAL+33;//摊位留言
const unsigned short _TXTATR_TOPSHOW			=_TXTATR_NORMAL+34;//top show
const unsigned short _TXTATR_ANNOUNCE			=_TXTATR_NORMAL+35;//系统公告
const unsigned short _TXTATR_TOPANNOUNCE		=_TXTATR_NORMAL+36;//滚屏公告
const unsigned short _TXTATR_HELP				=_TXTATR_NORMAL+37;//帮助
struct RelayData
{
	unsigned short	unMsgSize;
	unsigned short	unMsgType;
	char buffer[_MAX_MSGSIZE];///中继包
};
typedef map<OBJID,UINT> SyndicateLockStateList;///前面为帮派id,后面为超时时间,0为无锁
///处理新消息，结束消息自处理的混乱局面
class MsgManager  
{
public:
	~MsgManager();
	static MsgManager& Instance()
	{
		static MsgManager object;
		return object;
	}
public:
	void ProcessMsg(int nServerIndex,  char * pBuf, int nLen);
	void SendRelayMsg(int nServerIndex,CNetMsg &msg);
	void SendRelationReplyMsg(int nServerIndex,MsgRelayAction msgAction,OBJID srcRoleID,OBJID dstRoleID,LPCTSTR szName);
	void BroadcastRelayMsg(CNetMsg &msg,int nIgnoreServerIndex=0);
	void ProcessMsgServerDisconnect(int nServerIndex);///处理游戏服务器掉线的情况
	void BroadcastGameServerListMsg(int nIgnoreServerIndex=0);
	void OnBreathe();///专门做一些定时任务
	void AppointMainServer(int nIgnoreServerIndex);
protected:
	void SendRelationRelayMsg(int nServerIndex,OBJID idMsg,const char* pack, int nLen,int nExtra=0);
	void SendSysMsg(int nServerIndex,const char* szRever,int nChannel,const char* fmt, ...);
	void ProcessItemMsg(int nServerIndex,CNetMsg& msg);
	void ProcessWeapenSoulMsg(int nServerIndex,CNetMsg& msg);
	void ProcessSyndicateMemberInfoMsg(int nServerIndex,CNetMsg& msg);
	void ProcessSyndicateInfoMsg(int nServerIndex,CNetMsg& msg);
	void ProcessEudemoniInfoMsg(int nServerIndex,CNetMsg& msg);
	void UpServerListUI();
	bool KickoutRole(OBJID idRole,int nOldServerIndex);
	void ProcessRelayMsg(int nServerIndex,CNetMsg& msg);
	void ProcessTalkMsg(int nServerIndex,CNetMsg& msg);
	void ProcessFriendMsg(int nServerIndex,CNetMsg& msg);
	void ProcessSyndicateMsg(int nServerIndex,CNetMsg& msg);
	void MakeGameServerList2TalkMsg(CMsgTalk& msg);
	void ProcessMainServerMsg(int nServerIndex,CNetMsg& msg);
	void ProcessRegisterMsg(int nServerIndex,CNetMsg& msg);
	void ProcessTutorMsg(int nServerIndex,CNetMsg& msg);
	///解除所有帮派已超时的锁
	void BreakTimeoutSynLock();
	void BreakTimeoutUserLock();
protected:
	bool Lock(OBJID idSyndicate);///锁定帮派,true为锁定成功,false为锁定失败
	void UnLock(OBJID idSynDicate);///解锁帮派
	bool LockUser(OBJID idUser);///锁定角色,true为锁定成功,false为锁定失败
	void UnLockUser(OBJID idUser);///解锁角色

private:
	SyndicateLockStateList m_synLockStateList;///帮派锁状态
	SyndicateLockStateList m_userLockStateList;///角色锁状态
	//void ProcessAccountMsg(int nServerIndex,CMsgAccount* pMsg);
private:
	MsgManager();
};

#endif // !defined(AFX_MSGMANAGER_H__26E339FB_DAC3_41C9_AD17_76489AB3320B__INCLUDED_)
