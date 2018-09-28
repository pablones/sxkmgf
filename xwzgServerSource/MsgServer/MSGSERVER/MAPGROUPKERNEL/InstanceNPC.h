// InstanceNPC.h: interface for the CInstanceNPC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTANCENPC_H__8A7A9BB2_5A21_4A87_A3BB_9943EF3B3A9A__INCLUDED_)
#define AFX_INSTANCENPC_H__8A7A9BB2_5A21_4A87_A3BB_9943EF3B3A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//class CNpc;
//class CInstanceNPC;
///副本NPC,专为副本服务的,可能抽象度有点低,先用着吧

///InstanceType,从道具那里得到副本类型
#include "InstanceRule.h"
#include "MapInstance.h"
#include "MapGroupObject.h"

class CInstanceNPC : public CNpc,public CInstanceRule,public has_slots<>
{
public:
	CInstanceNPC();
	virtual ~CInstanceNPC();
	static CInstanceNPC* CreateNew() { return new CInstanceNPC; }
	void StartInstance();
	virtual bool InitNpc(int nParam1,int nParam2);
	void RequestJoinRoom(OBJID idPlayer);///处理请求加入副本房间
	BOOL ExitRoom(OBJID idPlayer);///退出房间
	BOOL CreateRoom(OBJID idPlayer);///创建房间
	BOOL CloseRoom(OBJID  idPlayer);///关闭房间
	void KickPlayer(OBJID idRequest,OBJID idPlayer);///踢出玩家
	void ChangeOwn(OBJID idRequest,OBJID idPlayer);///改变副本所有者信息
	void ChangeRoomTitle(OBJID idRequest,const CString& strRoomTitle);///改变副本房间标题
	void ChangeRoomPassword(OBJID idRequest,const CString& strRoomPwd);///改变副本房间密码
	void CheckRoomPassword(OBJID idRequest,const CString& strRoomPwd);///检查副本房间密码
	BOOL ChangeTeamGroup(OBJID idPlayer,UINT nTeamGroup);///改变玩家队伍
	void ReplyCreateInstance(OBJID idInstanceMap,UINT nInstanceMapType);
	void RefreshClientInstanceInfo(OBJID idPlayer,UINT nCurrentPage=1);///刷新客户端副本信息
	void RefreshInstanceInfo(OBJID idPlayer);///刷新客户端副本信息
	void RequestExitRoom(OBJID idRequest);///刷新客户端副本信息
	void SetInstanceType(UINT nInstanceType){m_nInstanceType=nInstanceType;}
	bool IsRoomOwn(OBJID idRequest){return m_idOwn==idRequest;}
	UINT GetInstanceType(){return m_nInstanceType;}
	UINT GetInstanceMapType(){return m_nInstanceMapType;}
	void SetInstanceMapType(UINT nInstanceMapType){m_nInstanceMapType=nInstanceMapType;}
	UINT GetInstanceUserSize(){return m_InstanceUserList.size();}
	bool HasPassword(){return m_strRoomPwd.GetLength()>0;}
	OBJID	GetOwnID(){return m_idOwn;}
	void	SetOwnID(OBJID idOwd){m_idOwn=idOwd;}
	OBJID	GetInstanceMapID(){return m_idInstanceMap;}
	void	SetInstanceMapID(OBJID idInstanceMap){m_idInstanceMap=idInstanceMap;}
	CInstanceUser* GetInstanceUser(UINT nIndex);
	CInstanceUser* FindInstanceUser(OBJID idInstanceUser);
	void	StartInstance(OBJID idRequest);///处理客户端发过来的开始副本的请求
	const CString& GetRoomTitle(){return m_strRoomTitle;}
	void	SetRoomTitle(CString strRoomTitle){m_strRoomTitle=strRoomTitle;}
	void	SetRoomPassword(CString strRoomPwd){m_strRoomPwd=strRoomPwd;}
	void	BroadcastMsg(CNetMsg& msg);///全部成员进行广播
	signal1<OBJID>	slotPlayerEnterInstance;///用户正入副本前触发的事件,系统可以在该处做一些准备工作,比如关闭对话框.
protected:
	void CloseRoom();///关闭整个副本
	BOOL JoinRoom(OBJID idPlayer);///加入房间
	void AutoCreateTeam();
	void AddPlayer(CInstanceUser& instanceuser);
	void RemovePlayer(OBJID idPlayer);
	void AddPlayer(OBJID idPlayer,UINT nTeamGroup=1);
	void TransmitAllPlayer();
	void TransmitPlayer(CInstanceUser& player);
	void OnPlayerEnterInstance(OBJID idPlayer);///处理自己产生的用户进入副本事件,准备关闭用户的副本对话框
	void NotifyUserStartInstance(OBJID idPlayer);///通知客户端副本开始了
private:
	void MakeTestData();///生成虚假的测试数据
	CInstanceTypeData* m_pData;///副本类型字典数据
	OBJID m_idInstanceMap;//房间地图ID
	UINT  m_nInstanceMapType;///地图类型
	OBJID m_idOwn;//房主
	UINT  m_nInstanceType;///副本类型
	CString m_strRoomTitle;///副本房间标题
	CString m_strRoomPwd;///副本房间密码
	InstanceUserList m_InstanceUserList;
};

#endif // !defined(AFX_INSTANCENPC_H__8A7A9BB2_5A21_4A87_A3BB_9943EF3B3A9A__INCLUDED_)
