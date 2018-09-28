// RoleManager.h: interface for the RoleManager class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ROLEMANAGER_H__86E7410A_E8FB_40C9_AAF7_21CCCA873389__INCLUDED_)
#define AFX_ROLEMANAGER_H__86E7410A_E8FB_40C9_AAF7_21CCCA873389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AllHeads.h"
#include "MsgRelay.h"

///缓冲玩家角色数据状态
class Role
{
public:
	OBJID		idRole;///角色编号
	NAMESTR		strRoleName;///角色名字
	OBJID		idServer;///所在线路服务器ID，其实是数组索引
};

typedef map<OBJID,Role> RoleContainer;
typedef RoleContainer::iterator RoleIter;
///管理所有注册的角色
class RoleManager  
{
public:
	static RoleManager& Instance()
	{
		static RoleManager object;
		return object;
	}
	~RoleManager();

public:
	Role* GetRole(LPCSTR strRoleName);///通过角色名查取角色信息
	Role* GetRole(OBJID idRole);///通过角色名查取角色信息
	void OnlineRole(OBJID idRole,LPCSTR	strRoleName,OBJID idServer);///玩家上线
	void QueryRole(int nServerIndex,MsgRelayAction msgAction,OBJID idDstRole,OBJID idSrcRole);///查询玩家在线情况
	void OfflineRole(OBJID idRole);///玩家下线;
	void PureServerRoleList(OBJID idServer);///当服务器掉线重连后清空它拥有的玩家
	void PrintAllUser();
	UINT GetRoleCountAtServer(OBJID idServer);///查询指定服中的玩家数
	//void UpdateRole(OBJID idRole,LPCSTR	strRoleName,OBJID idServer);///玩家更新，非定时，相互请求时更新自己
	int GetRoleLine(LPCTSTR szServer);
private:
	RoleManager();
	RoleContainer m_roleContainer;
};

#endif // !defined(AFX_ROLEMANAGER_H__86E7410A_E8FB_40C9_AAF7_21CCCA873389__INCLUDED_)
