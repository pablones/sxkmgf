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
const unsigned short _TXTATR_WORLDSERVER		=_TXTATR_NORMAL+30;//С����
const unsigned short _TXTATR_BOOTHNAME			=_TXTATR_NORMAL+31;//̯λ����
const unsigned short _TXTATR_BOOTHDES			=_TXTATR_NORMAL+32;//̯λ��Ϣ
const unsigned short _TXTATR_BOOTHLEAVEWORD		=_TXTATR_NORMAL+33;//̯λ����
const unsigned short _TXTATR_TOPSHOW			=_TXTATR_NORMAL+34;//top show
const unsigned short _TXTATR_ANNOUNCE			=_TXTATR_NORMAL+35;//ϵͳ����
const unsigned short _TXTATR_TOPANNOUNCE		=_TXTATR_NORMAL+36;//��������
const unsigned short _TXTATR_HELP				=_TXTATR_NORMAL+37;//����
struct RelayData
{
	unsigned short	unMsgSize;
	unsigned short	unMsgType;
	char buffer[_MAX_MSGSIZE];///�м̰�
};
typedef map<OBJID,UINT> SyndicateLockStateList;///ǰ��Ϊ����id,����Ϊ��ʱʱ��,0Ϊ����
///��������Ϣ��������Ϣ�Դ���Ļ��Ҿ���
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
	void ProcessMsgServerDisconnect(int nServerIndex);///������Ϸ���������ߵ����
	void BroadcastGameServerListMsg(int nIgnoreServerIndex=0);
	void OnBreathe();///ר����һЩ��ʱ����
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
	///������а����ѳ�ʱ����
	void BreakTimeoutSynLock();
	void BreakTimeoutUserLock();
protected:
	bool Lock(OBJID idSyndicate);///��������,trueΪ�����ɹ�,falseΪ����ʧ��
	void UnLock(OBJID idSynDicate);///��������
	bool LockUser(OBJID idUser);///������ɫ,trueΪ�����ɹ�,falseΪ����ʧ��
	void UnLockUser(OBJID idUser);///������ɫ

private:
	SyndicateLockStateList m_synLockStateList;///������״̬
	SyndicateLockStateList m_userLockStateList;///��ɫ��״̬
	//void ProcessAccountMsg(int nServerIndex,CMsgAccount* pMsg);
private:
	MsgManager();
};

#endif // !defined(AFX_MSGMANAGER_H__26E339FB_DAC3_41C9_AD17_76489AB3320B__INCLUDED_)
